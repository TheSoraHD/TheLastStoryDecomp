#include "gmk/MotionObject.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "chr/ControlBreakOpen.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_8023A02C(EffectMgr_unk* mgr, void* owner, int arg2, int arg3);
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" int fn_802378C4(atn::EfpHandle_unk* efp); // inferred: number of loops of the effect
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix
// inferred: collision world and ray test (see gmk::FlamingArrow)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    void* m34;
    void* m38; // inferred: hit object
    unsigned int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: link of the character lists of the managers (CharaTask + 0x14AC)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
// inferred: views of the hit message (names invented)
struct MotionHitSrc_unk {
    unsigned char m00[0x48];
    int m48;
};
struct MotionHitSkill_unk {
    unsigned char m00[0x04];
    int mId;
};
struct MotionHitMsg_unk {
    unsigned char m00[0x28];
    MotionHitSrc_unk* mSrc;     // 0x28
    MotionHitSkill_unk* mSkill; // 0x2C
};

inline void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: plays an effect at a position (name invented)
inline void PlayEfpAt_unk(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, const atn::Vector3& pos,
                          const atn::Vector3& rot, float scale) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, 0, 0, pos, rot, color, -1, -1, 1, scale);
}

namespace gmk {

GimmickCommon* MotionObject::fn_80412AFC(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) MotionObject(parent, id, arg);
    }
    return 0;
}

MotionObject::MotionObject(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mPrevState(0), mFrame(0), mStateDone(0), mInitState(-1) {
    m54 = 0;
    memset(mHasModel, 0, sizeof(mHasModel));
    memset(mActionEnabled, 0, sizeof(mActionEnabled));
}

MotionObject::~MotionObject() {}

int MotionObject::vf0C() {
    if (fn_803EC91C() == 0) {
        for (int i = 0; i < 8; i++) {
            if (mHasModel[i] != 0) {
                fn_803EDB18(&mModels[i]);
                if (strlen(mBoundNode.mStr) != 0 && mModels[i].mB0 < 0) {
                    mModels[i].fn_80095D44(mModels[i].fn_80092814(mBoundNode.mStr, 0), -1);
                }
            }
        }
        return 1;
    }
    return 0;
}

void MotionObject::vf28() {
    GimmickMsg_unk msg;
    if (m58 > 0) {
        msg.mType = m58;
    } else {
        msg.mType = mInitState >= 0 ? mInitState : m58;
    }
    vf40(&msg);
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    fn_804137F4();
}

// NONMATCHING: 97.5% - register allocation (this in r24 instead of r25), the stack of the ray test of
// the character loop, and the original loads 0.0f in f1 before CharaTask::fn_80148B38 (it may take a
// float; the symbol is named without arguments).
void MotionObject::vf10() {
    if (mStateDone != 0) {
        mStateDone = 0;
        GimmickMsg_unk msg(mStates[m54].mNext);
        vf40(&msg);
    }
    atn::Model* model = vf38();
    MotionPlay_unk* play = &mPlay[m54];
    MotionEfp_unk* efp = &mEfp[m54];
    MotionColli_unk* colli = &mColli[m54];
    MotionState_unk* state = &mStates[m54];
    if (m54 != mPrevState) {
        MotionEfp_unk* prevEfp = &mEfp[mPrevState];
        MotionColli_unk* prevColli = &mColli[mPrevState];
        if (mHasModel[m54] != 0) {
            mModels[m54].fn_80097C08(0, play->mMotion, play->mLoop, play->mReverse, 1, play->mBlend, 0.1f);
            mModels[m54].mTracks[0].m08 = play->mFrame;
        } else {
            mB0.fn_803F10AC(0);
        }
        if (prevColli->mObject != 0) {
            prevColli->mObject->mFlags &= ~1;
        }
        if (colli->mObject != 0) {
            colli->mObject->mFlags |= 1;
        }
        fn_80239DAC(lbl_8087F3C0, this, 0, (prevEfp->mFlags & 1) ^ 1);
        if (efp->mEfp.mResource != 0) {
            if (fn_802378C4(&efp->mEfp) <= 0) {
                lbl_8087F3C0->mB8 = 1;
            }
            fn_80232B7C(this, 0);
            if (mHasModel[m54] != 0) {
                PlayEfp_unk(lbl_8087F3C0, &efp->mEfp, &mModels[m54]);
            } else {
                PlayEfpAt_unk(lbl_8087F3C0, &efp->mEfp, m6C, m78, m90.x);
            }
            if ((efp->mFlags & 2) == 2) {
                fn_8023A02C(lbl_8087F3C0, this, 0, 8);
            }
            if (fn_802378C4(&efp->mEfp) <= 0) {
                lbl_8087F3C0->mB8 = 0;
            }
        }
        if (prevColli->mTarget != 0) {
            prevColli->mTarget->mSphere.mFlags &= ~1;
        }
        if (colli->mTarget != 0) {
            colli->mTarget->mSphere.mFlags |= 1;
        }
        mPrevState = m54;
        mFrame = 0;
    }
    if (vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    fn_804137F4();
    fn_804139D8();
    if (model != 0 && state->mNext >= 0 && colli->mTarget == 0) {
        int frames = state->mFrames;
        if (frames > 0) {
            if (model->fn_80097D7C(0) >= frames) {
                if (model->mTracks[0].m08 >= frames) {
                    mStateDone = 1;
                }
            } else if (mFrame >= frames) {
                mStateDone = 1;
            }
        } else if (model->fn_80097D7C(0) > 0.0f) {
            float frame = model->mTracks[0].m08;
            if (frame >= model->fn_80097D7C(0)) {
                mStateDone = 1;
            }
        }
    }
    if (model != 0 && lbl_8087F8A0 != 0 && colli->mObject != 0 && mActionEnabled[m54] != 0) {
        float frame = model->mTracks[0].m08;
        if (frame < model->fn_80097D7C(0)) {
            for (CharaTask* chara = lbl_8087F8A0->mHead; chara != 0;
                 chara = ((CharaListNode_unk*)chara)->mNext) {
                atn::Vector3 pos(chara->mPos);
                atn::Vector3 from(pos);
                atn::Vector3 to(pos);
                from.y += 10.0f;
                to.y -= 10.0f;
                CollResult_unk result;
                if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0 &&
                    result.m38 == colli->mObject) {
                    chara->mPos = result.m04;
                    chara->fn_80144710();
                    fn_8000D430(&chara->mMotion, boost::function<void()>());
                    chara->fn_80148B38();
                }
            }
        }
    }
    mFrame++;
}

atn::Model* MotionObject::vf38() {
    if (mHasModel[m54] != 0) {
        return &mModels[m54];
    }
    return 0;
}

// NONMATCHING: 97.0% - stack layout of the target sphere temporaries (tried RotY_unk/by-value helpers)
void MotionObject::fn_804137F4() {
    atn::Model* model = vf38();
    MotionColli_unk* colli = &mColli[m54];
    if (model != 0 && colli->mObject != 0) {
        if (colli->mNode != 0) {
            colli->mObject->SetMtx_unk(*(atn::Matrix3x4*)colli->mNode);
        } else {
            colli->mObject->SetMtx_unk(*(atn::Matrix3x4*)model->mMtx);
        }
    }
    MotionTarget_unk* target = colli->mTarget;
    if (target != 0) {
        atn::Vector3 base = model != 0 ? ((atn::Matrix3x4*)model->mMtx)->GetTrans_unk() : lbl_807C7060.GetTrans_unk();
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', m78.y);
        atn::Vector3 offset(target->mOffset);
        PSMTXMultVec(mtx, &offset, &offset);
        target->mSphere.mSphere = atn::ColliSphere::Sphere_unk(base + offset, target->mRadius);
    }
}

// The int index compared as unsigned gives the loop registers of the original. The three
// release asserts keep it out of line (the original calls it from vf10 while PlayEfp_unk is inlined).
void MotionObject::fn_804139D8() {
    atn::Model* model = vf38();
    ((void)0);
    ((void)0);
    ((void)0);
    if (model != 0) {
        for (int i = 0; i < (unsigned int)mFades[m54].mSize; i++) {
            mFades[m54][i].fn_803EBE74(model);
        }
        for (int i = 0; i < (unsigned int)mSwitches[m54].mSize; i++) {
            mSwitches[m54][i].fn_803EC0F8(model);
        }
    }
}

void MotionObject::vf14() {
    if (lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int MotionObject::vf1C() {
    for (int i = 0; i < 8; i++) {
        if (mModels[i].fn_8008B140() != 0) {
            return 1;
        }
        if (mColli[i].mObject != 0 && mColli[i].mObject->fn_800580BC() != 0) {
            return 1;
        }
        if (mEfp[i].mEfp.fn_80237874() != 0) {
            return 1;
        }
    }
    return 0;
}

void MotionObject::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    int model = 0;
    unsigned int fades = 0;
    unsigned int switches = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            if (fades != 0) {
                mFades[model].Resize_unk(fades, 1);
                fades = 0;
            }
            if (switches != 0) {
                mSwitches[model].Resize_unk(switches, 1);
                switches = 0;
            }
            model++;
            fn_8008AD4C(&mModels[model], csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModels[model], &csv);
            mHasModel[model] = 1;
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModels[model], path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            MotionPlay_unk* play = &mPlay[model];
            fn_80097A88(&mModels[model], play->mMotion = fn_802180A8(csv.fn_8005B9CC()), path);
            play->mBlend = fn_800DC288(csv.fn_8005B9CC());
            play->mLoop = atoi(csv.fn_8005B9CC()) == 1;
            play->mReverse = atoi(csv.fn_8005B9CC()) == 1;
            play->mFrame = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            MotionColli_unk* colli = &mColli[model];
            colli->mObject = new (12, "", "", 0) atn::ColliObject();
            colli->mObject->fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "TARGET_COLLI") == 0) {
            MotionColli_unk* colli = &mColli[model];
            MotionTarget_unk* target = new (12, "", "", 0) MotionTarget_unk;
            colli->mTarget = target;
            target->mOffset.x = fn_800DC288(csv.fn_8005B9CC());
            target->mOffset.y = fn_800DC288(csv.fn_8005B9CC());
            target->mOffset.z = fn_800DC288(csv.fn_8005B9CC());
            target->mRadius = fn_800DC288(csv.fn_8005B9CC());
            target->mSphere.mSphere.mCenter = target->mOffset;
            target->mSphere.mSphere.mRadius = target->mRadius;
            int mask = 0x808F8;
            if (mId == 28120) {
                mask |= 6;
            }
            target->mSphere.m20 = mask;
            target->mSphere.mFlags |= 8;
            target->mSphere.m0C = (int)this;
        } else if (strcmp(field, "EFP") == 0) {
            MotionEfp_unk* efp = &mEfp[model];
            efp->mEfp.fn_8023780C(csv.fn_8005B9CC());
            const char* opt = csv.fn_8005B9CC();
            while (*opt != '\0') {
                if (strcmp(opt, "EFF_CLEAR_IMMEDIATE") == 0) {
                    efp->mFlags |= 1;
                }
                if (strcmp(opt, "EFF_REFLECT") == 0) {
                    efp->mFlags |= 2;
                }
                opt = csv.fn_8005B9CC();
            }
        } else if (strcmp(field, "PLAYER_ACTION") == 0) {
            MotionAction_unk* action = &mActions[model];
            action->mEnabled = 1;
            action->m04[0] = fn_800DC288(csv.fn_8005B9CC());
            action->m04[1] = fn_800DC288(csv.fn_8005B9CC());
            action->m04[2] = fn_800DC288(csv.fn_8005B9CC());
            action->m04[3] = fn_800DC288(csv.fn_8005B9CC());
            action->mPos.x = fn_800DC288(csv.fn_8005B9CC());
            action->mPos.y = fn_800DC288(csv.fn_8005B9CC());
            action->mPos.z = fn_800DC288(csv.fn_8005B9CC());
            action->mRotY = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            mE8 = 6;
            mEC = 0;
        } else if (strcmp(field, "MATALPHA") == 0) {
            fades++;
        } else if (strcmp(field, "NODEVIS") == 0) {
            switches++;
        } else if (strcmp(field, "INIT_STATE") == 0) {
            mInitState = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
    if (fades != 0) {
        mFades[model].Resize_unk(fades, 1);
    }
    if (switches != 0) {
        mSwitches[model].Resize_unk(switches, 1);
    }
}

// NONMATCHING: 99.94% - the CSV size temporary gets r22 instead of r25 (data/size read into
// locals first fixes the rest)
void MotionObject::vf24() {
    unsigned long size = mBinary.fn_8047059C();
    const char* data = (const char*)mBinary.fn_80470580();
    atn::CsvTab csv(data, size);
    csv.fn_8005BCE8(lbl_8087D710);
    int model = 0;
    int fade = 0;
    int sw = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fade = 0;
            sw = 0;
            model++;
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            csv.fn_8005B9CC();
            mColli[model].mNode = mModels[model].GetNodeMtx_unk(csv.fn_8005B9CC());
            mActionEnabled[model] = atoi(csv.fn_8005B9CC());
        } else if (strcmp(field, "STATE_SETTING") == 0) {
            MotionState_unk* state = &mStates[atoi(csv.fn_8005B9CC())];
            state->mNext = atoi(csv.fn_8005B9CC());
            state->mFrames = atoi(csv.fn_8005B9CC());
        } else if (strcmp(field, "BOUND_CENTER_NODE") == 0) {
            mBoundNode.Set_unk(csv.fn_8005B9CC());
        } else if (strcmp(field, "MATALPHA") == 0) {
            mFades[model][fade].fn_803EBC44(&csv, &mModels[model]);
            fade++;
        } else if (strcmp(field, "NODEVIS") == 0) {
            mSwitches[model][sw].fn_803EC0A4(&csv, &mModels[model]);
            sw++;
        }
    } while (csv.fn_8005BBF8() != 0);
}

int MotionObject::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 7) {
        m54 = 0;
    }
    return m54;
}

int MotionObject::vf44(GimmickHitMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    int state = m54;
    if (mColli[state].mTarget == 0) {
        return 0;
    }
    if (mId == 28120) {
        MotionHitMsg_unk* hit = (MotionHitMsg_unk*)msg;
        if (hit->mSrc == 0 || hit->mSrc->m48 != 0) {
            return 0;
        }
        if (hit->mSkill == 0) {
            return 0;
        }
        if (hit->mSkill->mId != 0xCD && hit->mSkill->mId != 2000) {
            return 0;
        }
    }
    int next = mStates[state].mNext;
    if (next >= 0) {
        GimmickMsg_unk next_msg(next);
        vf40(&next_msg);
    }
    return 0;
}

void MotionObject::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
    atn::Model* model = vf38();
    if (model != 0 && model->mTracks[0].m18 == 0 && model->mTracks[0].m04 != 0) {
        model->mTracks[0].m08 = model->fn_80097D7C(0);
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
    }
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

int MotionObject::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    return 0;
}

void MotionObject::vf4C(CharaTask* chara) {
    if (mActions[m54].mEnabled != 0) {
        atn::Vector3 pos(mActions[m54].mPos);
        PSMTXMultVec(mModels[m54].mMtx, &pos, &pos);
        atn::Vector3 rot(m78);
        rot.y = atn::WrapAngle_unk(rot.y + mActions[m54].mRotY);
        chara->mPos = pos;
        chara->mRot = rot;
        chara->fn_80178208(new (1, "", "", 0) chr::ControlBreakOpen((chr::Chara_unk*)chara, (chr::BreakTarget_unk*)this));
    }
}

int MotionObject::vf5C() {
    return mColli[m54].mTarget != 0;
}

float MotionObject::vf58() {
    if (vf5C() != 0) {
        return mColli[m54].mTarget->mSphere.mSphere.mRadius;
    }
    return 0.0f;
}

atn::Vector3 MotionObject::vf54() {
    return vf5C() != 0 ? mColli[m54].mTarget->mSphere.mSphere.mCenter : m6C;
}

int MotionObject::vf50() {
    if (vf5C() != 0) {
        return (int)mColli[m54].mTarget;
    }
    return 0;
}

} // namespace gmk
