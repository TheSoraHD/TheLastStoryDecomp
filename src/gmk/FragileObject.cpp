#include "gmk/FragileObject.h"
#include "atn/Csv.h"
#include "atn/Material_unk.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" double sqrt(double x);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" int fn_80208748(const char* name); // inferred: character type from its id string
extern "C" void fn_80092A00(atn::Model* model, float blend); // inferred: light map blend
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length

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
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087F610; // inferred: arena mode
// inferred: bullets; m3E39C is set while a gimmick fires one
struct BulletMgr_unk {
    unsigned char m00000[0x3E39C];
    int m3E39C;
};
extern "C" BulletMgr_unk* lbl_8087F048;
struct HitInfo_unk;
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_800F8548(BulletMgr_unk* mgr);
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, int arg1, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
// inferred: map id test of the "IF" blocks (see game/GimmickStateCommand.cpp)
extern "C" bool fn_80216AFC(const char* id, int* type, int* map, int* point);
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
    int m50;
};
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);
// debug menu (see gmk/Cannon.cpp)
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))
#define __MIN_unk(a, b) ((a) < (b) ? (a) : (b))

// inferred: link of the character lists of the managers (CharaTask + 0x14AC)
struct FragileCharaNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
// inferred: float color (as in game/GimmickCommon.cpp)
struct FragileColor_unk {
    FragileColor_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

inline void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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

// inferred: listeners (see gmk/Item.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

namespace gmk {

GimmickCommon* FragileObject::fn_803F1BB4(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) FragileObject(parent, id, arg);
    }
    return 0;
}

FragileObject::FragileObject(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mColliBeforeFrame(0.0f), mColliAfterFrame(0.0f), mHasColliBefore(0),
      mHasColliAfter(0), mHasTarget(0), mExplode(0), mExplodeScale(0.01f), mExplodeElza(0), mCrushFrames(0),
      mCrushFrame(0), mDrawOffWait(0.0f), mCrushFlags(0) {
    m54 = 0;
    memset(&mTarget, 0, sizeof(mTarget));
    unsigned int mask = 0x808F8;
    if (mId == 1110) {
        mask |= 6;
    }
    if (lbl_8087F610 != 0) {
        mask |= 4;
    }
    mSphere.m20 = mask;
}

FragileObject::~FragileObject() {}

int FragileObject::vf0C() {
    if (fn_803EC91C() == 0) {
        mSphere.m0C = (int)this;
        mSphere.mFlags |= 8;
        if (mTarget.mNoneActive != 0) {
            mSphere.mFlags &= ~1;
        }
        fn_803EDB18(&mModelBefore);
        fn_803EDB18(&mModelAfter);
        if (strlen(mBoundNode.mStr) != 0 && mModelAfter.mB0 < 0) {
            mModelAfter.fn_80095D44(mModelAfter.GetNodeNoByName(mBoundNode.mStr, 0), -1);
            if (mId == 1010) {
                mModelAfter.mAC = 10.0f;
            }
        }
        return 1;
    }
    return 0;
}

inline void FragileObject::UpdateColli_unk() {
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    atn::Matrix3x4 mtx(*(atn::Matrix3x4*)mModelBefore.mMtx);
    mColliBefore.SetMtx_unk(mtx);
    mColliAfter.SetMtx_unk(mtx);
    atn::Matrix3x4 rot;
    atn::Vector3 base = mtx.GetTrans_unk();
    rot.fn_8013A13C(m78.y);
    atn::Vector3 offset(mTarget.mOffset);
    PSMTXMultVec(rot.m, &offset, &offset);
    mSphere.mSphere = atn::ColliSphere::Sphere_unk(base + offset, mTarget.mSize);
}

// NONMATCHING: 97.9% - stack slots of the target sphere: the original keeps the Sphere temporary
// lowest, then the translation, the sum and the offset (as gmk::MotionObject::fn_804137F4, 97%).
// Tried: helper returning the position, named sphere, rotation helper returning by value, the
// placement written in Start, base/rot declaration order
void FragileObject::Start() {
    vf2C();
    UpdateColli_unk();
    GimmickMsg_unk msg(1);
    if (m58 == 1) {
        msg.mType = 7;
    } else if (m58 == 2) {
        msg.mType = 8;
    } else if (m58 == 3) {
        msg.mType = 6;
    }
    vf40(&msg);
}

// NONMATCHING: 99.1% - the target sphere stack slots (see Start) and the counter/offset registers of
// the loops swapped (r30/r29 in the original). Tried: int/unsigned indices, size(), mData[i], loops in
// inline helpers. The flag accessor and the position from GetTrans_unk fixed the rest
inline bool IsColliOn_unk(const atn::ColliBase& c) {
    return (c.mFlags & 1) == 1;
}

void FragileObject::vf10() {
    if (m54 == 7) {
        return;
    }
    if (m54 == 0) {
        Start();
        mCrushFrame = 0;
        mTarget.mHp++;
        GimmickMsg_unk msg(1001);
        vf40(&msg);
    }
    if (m54 == 2) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
        msg.mType = 1000;
        vf40(&msg);
    }
    UpdateColli_unk();
    if (mBlendAfter.mModel != 0) {
        int changed = 0;
        if (mBlendAfter.mUp != 0) {
            if (mBlendAfter.mFrame < mBlendAfter.mFrames) {
                mBlendAfter.mFrame++;
                changed = 1;
            }
        } else if (mBlendAfter.mFrame > 0) {
            mBlendAfter.mFrame--;
            changed = 1;
        }
        if (changed != 0) {
            fn_80092A00(mBlendAfter.mModel, 1.0f - (float)mBlendAfter.mFrame / (float)mBlendAfter.mFrames);
        }
    }
    if (mBlendBefore.mModel != 0) {
        int changed = 0;
        if (mBlendBefore.mUp != 0) {
            if (mBlendBefore.mFrame < mBlendBefore.mFrames) {
                mBlendBefore.mFrame++;
                changed = 1;
            }
        } else if (mBlendBefore.mFrame > 0) {
            mBlendBefore.mFrame--;
            changed = 1;
        }
        if (changed != 0) {
            fn_80092A00(mBlendBefore.mModel, 1.0f - (float)mBlendBefore.mFrame / (float)mBlendBefore.mFrames);
        }
    }
    if (m54 == 1 || m54 == 8) {
        if (mModelBefore.fn_80097D40(2) != 0 && vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        for (unsigned int i = 0; i < mGlows.size(); i++) {
            mGlows[i].fn_803F42C8(&mModelBefore);
        }
    } else if (m54 == 3) {
        if (mModelBefore.fn_80097D40(1) != 0 && vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        for (unsigned int i = 0; i < mGlows.size(); i++) {
            mGlows[i].fn_803F42C8(&mModelBefore);
        }
        if ((mCrushFlags & 1) != 1 && ++mCrushFrame >= mCrushFrames) {
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
    } else if (m54 >= 4 && 6 > m54) {
        float frame = mModelAfter.mTracks[0].m08;
        if (IsColliOn_unk(mColliBefore) && mColliBeforeFrame < frame) {
            mColliBefore.mFlags &= ~1;
            m54 = 5;
            atn::Vector3 pos = ((atn::Matrix3x4*)mModelBefore.mMtx)->GetTrans_unk();
            pos.y += 15.0f;
            if (lbl_8087F048 != 0 && mExplode > 0) {
                int chara = 0;
                if (mExplodeElza == 1) {
                    chara = (int)lbl_8087F8A0->mHead;
                }
                lbl_8087F048->m3E39C = 1;
                BulletMgr_unk* mgr = lbl_8087F048;
                fn_800FAB80(mgr, chara, fn_80219E6C(mExplode), fn_800F8548(mgr), &pos, &m78, 0, 30, -1, -1,
                            mExplodeScale, 1.0f);
                lbl_8087F048->m3E39C = 0;
            }
        }
        if (!IsColliOn_unk(mColliAfter) && mColliAfterFrame < frame) {
            if (mHasColliAfter != 0) {
                mColliAfter.mFlags |= 1;
            } else {
                mColliAfter.mFlags &= ~1;
            }
        }
        if (frame >= mModelAfter.fn_80097D7C(0)) {
            m54 = 6;
            if ((mCrushFlags & 2) == 2) {
                mDrawOffWait = 150.0f;
            }
        }
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        for (unsigned int i = 0; i < mFades.size(); i++) {
            mFades[i].fn_803EBE74(&mModelAfter);
        }
        for (unsigned int i = 0; i < mSwitches.size(); i++) {
            mSwitches[i].fn_803EC0F8(&mModelAfter);
        }
    } else if (m54 == 6) {
        for (unsigned int i = 0; i < mFades.size(); i++) {
            mFades[i].fn_803EBE74(&mModelAfter);
        }
        for (unsigned int i = 0; i < mSwitches.size(); i++) {
            mSwitches[i].fn_803EC0F8(&mModelAfter);
        }
        if ((mCrushFlags & 2) == 2) {
            if (mDrawOffWait > 0.0f) {
                mDrawOffWait -= 1.0f;
            } else if (!mModelAfter.mFlag2_unk && m9C != 0) {
                m9C = 0;
            }
        }
    }
}

void FragileObject::vf14() {
    if (m54 != 7 && lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int FragileObject::vf1C() {
    if (mModelAfter.fn_8008B140() != 0 || mModelBefore.fn_8008B140() != 0) {
        return 1;
    }
    if (mEfpWait.IsLoading() || mEfp.IsLoading() || mEfpBefore.IsLoading()) {
        return 1;
    }
    if (mColliBefore.fn_800580BC() != 0 || mColliAfter.fn_800580BC() != 0) {
        return 1;
    }
    return 0;
}

void FragileObject::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    unsigned int fades = 0;
    unsigned int switches = 0;
    unsigned int glows = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL_BEFORE") == 0) {
            fn_8008AD4C(&mModelBefore, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModelBefore, &csv);
        } else if (strcmp(field, "MODEL_LOD_BEFORE") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModelBefore, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MODEL_AFTER") == 0) {
            fn_8008AD4C(&mModelAfter, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModelAfter, &csv);
        } else if (strcmp(field, "MODEL_LOD_AFTER") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModelAfter, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModelAfter, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "MOTION_BEFORE") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModelBefore, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "MOTION_WAIT") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModelBefore, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLISION_BEFORE") == 0) {
            mColliBeforeFrame = fn_800DC288(csv.fn_8005B9CC());
            mColliBefore.fn_80058078(csv.fn_8005B9CC());
            mHasColliBefore = 1;
        } else if (strcmp(field, "COLLISION_AFTER") == 0) {
            mColliAfterFrame = fn_800DC288(csv.fn_8005B9CC());
            mColliAfter.fn_80058078(csv.fn_8005B9CC());
            mHasColliAfter = 1;
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_BEFORE") == 0) {
            mEfpBefore.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_WAIT") == 0) {
            mEfpWait.LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "TARGET_COLLISION") == 0) {
            mTarget.mOffset.x = fn_800DC288(csv.fn_8005B9CC());
            mTarget.mOffset.y = fn_800DC288(csv.fn_8005B9CC());
            mTarget.mOffset.z = fn_800DC288(csv.fn_8005B9CC());
            mTarget.mSize = fn_800DC288(csv.fn_8005B9CC());
            mTarget.mHp = atoi(csv.fn_8005B9CC());
            mTarget.mValidDamage = atoi(csv.fn_8005B9CC());
            mTarget.mNoneActive = strcmp(csv.fn_8005B9CC(), "NONE_ACTIVE") == 0;
            float size = mTarget.mSize;
            mSphere.mSphere.mCenter = mTarget.mOffset;
            mSphere.mSphere.mRadius = size;
            mHasTarget = 1;
        } else if (strcmp(field, "BOUND_CENTER_NODE") == 0) {
            mBoundNode.Set_unk(csv.fn_8005B9CC());
        } else if (strcmp(field, "MATALPHA") == 0) {
            fades++;
        } else if (strcmp(field, "NODEVIS") == 0) {
            switches++;
        } else if (strcmp(field, "MATGLOW") == 0) {
            glows++;
        }
    } while (csv.fn_8005BBF8() != 0);
    mFades.Resize_unk(fades, 1);
    mSwitches.Resize_unk(switches, 1);
    mGlows.Resize_unk(glows, 1);
}

inline void FragileGlow_unk::Read_unk(atn::CsvTab* csv, atn::Model* model) {
    mMaterial = model->fn_80092954(csv->fn_8005B9CC());
    if (mMaterial != 0) {
        mFrom = fn_800DC288(csv->fn_8005B9CC());
        mTo = fn_800DC288(csv->fn_8005B9CC());
        mRange = fn_800DC288(csv->fn_8005B9CC());
        const char* chara = csv->fn_8005B9CC();
        if (chara != 0 && strlen(chara) >= 5) {
            mType = fn_80208748(chara);
            mId = atoi(chara + 2);
            mMode = mMaterial->m4D;
        }
    }
}

void FragileObject::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    const char* flag = csv.mField;
    int fade = 0;
    int sw = 0;
    int glow = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EXPLODE_PARAM") == 0) {
            mExplode = atoi(csv.fn_8005B9CC());
            mExplodeScale = fn_800DC288(csv.fn_8005B9CC());
            if (strcmp(csv.fn_8005B9CC(), "ELZA") == 0) {
                mExplodeElza = 1;
            }
        } else if (strcmp(field, "CRUSH_PARAM") == 0) {
            mCrushFrames = atoi(csv.fn_8005B9CC());
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
            while (strlen(csv.fn_8005B9CC()) != 0) {
                if (strcmp(flag, "NO_AUTO_CRUSH") == 0) {
                    mCrushFlags |= 1;
                } else if (strcmp(flag, "AUTO_DRAW_OFF") == 0) {
                    mCrushFlags |= 2;
                } else if (strcmp(flag, "LOOP_CRUSH") == 0) {
                    mCrushFlags |= 4;
                }
            }
        } else if (strcmp(field, "MATALPHA") == 0) {
            mFades[fade].fn_803EBC44(&csv, &mModelAfter);
            fade++;
        } else if (strcmp(field, "NODEVIS") == 0) {
            mSwitches[sw].fn_803EC0A4(&csv, &mModelAfter);
            sw++;
        } else if (strcmp(field, "MATGLOW") == 0) {
            mGlows[glow].Read_unk(&csv, &mModelBefore);
            glow++;
        } else if (strcmp(field, "LIGHTMAP_BLEND") == 0) {
            int frames = atoi(csv.fn_8005B9CC());
            mBlendAfter.mModel = &mModelAfter;
            mBlendAfter.mFrames = frames;
            mBlendBefore.mModel = &mModelBefore;
            mBlendBefore.mFrames = frames;
        } else if (strcmp(field, "IF") == 0) {
            int type;
            int map;
            int point;
            if (fn_80216AFC(csv.fn_8005B9CC(), &type, &map, &point) && lbl_8087F430 != 0 &&
                fn_80373148(lbl_8087F430) != 0) {
                MapInfo_unk* info = fn_80373148(lbl_8087F430);
                if (type != info->m48 || map != info->m4C || point != info->m50) {
                    while (csv.fn_8005BBF8() != 0) {
                        const char* s = csv.fn_8005B9CC();
                        if (*s == ';') {
                            continue;
                        }
                        if (strcmp(s, "ENDIF") == 0 || strcmp(s, "ELSE") == 0) {
                            break;
                        }
                    }
                }
            }
        } else if (strcmp(field, "ELSE") == 0) {
            while (csv.fn_8005BBF8() != 0) {
                const char* s = csv.fn_8005B9CC();
                if (*s == ';') {
                    continue;
                }
                if (strcmp(s, "ENDIF") == 0) {
                    break;
                }
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int FragileObject::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    int type = msg->mType;
    switch (type) {
    case 0:
    case 1:
    case 8:
        m54 = type;
        if (mModelBefore.fn_80097D40(2) != 0) {
            mModelBefore.fn_80097C08(0, 2, 1, 1, 1, 1.0f, 0.1f);
            fn_803ED5D0(&mModelBefore);
            fn_803ED0D4(&mModelBefore, 1);
        } else if (mModelBefore.fn_80097D40(1) != 0) {
            mModelBefore.fn_80097C08(0, 1, 1, 1, 1, 1.0f, 0.1f);
            fn_803ED5D0(&mModelBefore);
            fn_803ED0D4(&mModelBefore, 1);
        }
        mModelAfter.fn_80097C08(0, 0, (mCrushFlags >> 2) & 1, 1, 1, 1.0f, 0.1f);
        fn_803ED5D0(&mModelAfter);
        fn_803ED0D4(&mModelAfter, 1);
        mModelAfter.mTracks[0].m08 = 0.0f;
        if (mHasColliBefore != 0) {
            mColliBefore.mFlags |= 1;
        } else {
            mColliBefore.mFlags &= ~1;
        }
        mColliAfter.mFlags &= ~1;
        int active = 0;
        if (mHasTarget != 0 && mTarget.mNoneActive == 0) {
            active = 1;
        }
        if (active != 0) {
            mSphere.mFlags |= 1;
        } else {
            mSphere.mFlags &= ~1;
        }
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        if (mEfpWait.mResource != 0) {
            fn_80232B7C(this, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfpWait, &mModelBefore);
        }
        if (m9C == 0) {
            m9C = 1;
        }
        break;
    case 2:
    case 3:
        m54 = type;
        mTarget.mHp = 0;
        mSphere.mFlags &= ~1;
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        mModelBefore.fn_80097C08(0, 1, 1, 0, 1, 0.0f, 0.1f);
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        if (mEfpBefore.mResource != 0) {
            fn_80232B7C(this, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfpBefore, &mModelBefore);
        }
        break;
    case 4:
        m54 = type;
        mCrushFrame = 0;
        mTarget.mHp = 0;
        mSphere.mFlags &= ~1;
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        mModelAfter.fn_80097C08(0, 0, (mCrushFlags >> 2) & 1, 1, 1, 1.0f, 0.1f);
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        if (mEfp.mResource != 0) {
            fn_80232B7C(this, 0);
            PlayEfp_unk(lbl_8087F3C0, &mEfp, &mModelAfter);
        }
        break;
    case 5:
    case 6:
    case 7:
        m54 = type;
        mSphere.mFlags &= ~1;
        mColliBefore.mFlags &= ~1;
        if ((type != 7 ? mHasColliAfter : 0) != 0) {
            mColliAfter.mFlags |= 1;
        } else {
            mColliAfter.mFlags &= ~1;
        }
        mModelAfter.fn_80097C08(0, 0, (mCrushFlags >> 2) & 1, 0, 1, 1.0f, 0.1f);
        mModelAfter.mTracks[0].m08 = mModelAfter.fn_80097D7C(0);
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        break;
    case 1000:
        mBlendAfter.mUp = 1;
        mBlendBefore.mUp = 1;
        break;
    case 1001:
        mBlendAfter.mUp = 0;
        mBlendBefore.mUp = 0;
        break;
    }
    CharaListeners_unk* listeners = fn_8014F8F4();
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || listener.mId == 11) {
                listener.mFunc(11, (CharaTask*)this, msg);
            }
        }
    }
    return m54;
}

int FragileObject::vf44(GimmickHitMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (mHasTarget == 0) {
        return 0;
    }
    if (mId == 1051 && msg->mSender != 0 && msg->mSender->mType == 2) {
        return 0;
    }
    if (m54 < 4) {
        if (mTarget.mValidDamage <= msg->mType) {
            mTarget.mHp -= msg->mType;
        }
        if (mTarget.mHp <= 0) {
            GimmickMsg_unk crush(4);
            vf40(&crush);
            return 1;
        }
    }
    return 0;
}

void FragileObject::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    void* target = fn_8008937C(group, "targetCollision");
    fn_80087E9C(target, "offset", &mTarget.mOffset, 0, 0, -9999.0f, 9999.0f, 1.0f);
    fn_8008771C(target, "size", &mTarget.mSize, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_800874C8(target, "HP", &mTarget.mHp, 0, 9999, 1, 0, 0);
    fn_800874C8(target, "ValidDamage", &mTarget.mValidDamage, 0, 9999, 1, 0, 0);
    mB0.fn_803F11F8(group);
}

void FragileObject::vf70(int state) {
    m54 = state;
    switch (state) {
    case 0:
    case 1:
    case 8: {
        GimmickMsg_unk msg(state);
        vf40(&msg);
        break;
    }
    case 2:
    case 3:
    case 4:
    case 5:
    case 6: {
        GimmickMsg_unk msg(6);
        vf40(&msg);
        break;
    }
    case 7: {
        GimmickMsg_unk msg(state);
        vf40(&msg);
        break;
    }
    }
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

// NONMATCHING: 99.85% - stack slots of the distance vectors and the colour, and f1/f3 after sqrt.
// Tried: translation temporary, named position, sqrt in a local, colour declared first
void FragileGlow_unk::fn_803F42C8(atn::Model* model) {
    CharaTask* nearest = 0;
    float best = 1e16f;
    if (lbl_8087F8A0 != 0) {
        for (CharaTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = ((FragileCharaNode_unk*)chara)->mNext) {
            if ((!chara->IsFlag_unk(4) || chara->m55C == 8) && mType == chara->fn_80179AB4() &&
                mId == chara->fn_80179A34() / 10000) {
                atn::Vector3 d = ((atn::Matrix3x4*)model->mMtx)->GetTrans_unk() - chara->mPos;
                float dist = PSVECSquareMag(&d);
                if (dist < best) {
                    best = dist;
                    nearest = chara;
                }
            }
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (CharaTask* chara = EnemyManager::sInstance->mHead; chara != 0;
             chara = ((FragileCharaNode_unk*)chara)->mNext) {
            if ((!chara->IsFlag_unk(4) || chara->m55C == 8) && mType == chara->fn_80179AB4() &&
                mId == chara->fn_80179A34() / 10000) {
                atn::Vector3 d = ((atn::Matrix3x4*)model->mMtx)->GetTrans_unk() - chara->mPos;
                float dist = PSVECSquareMag(&d);
                if (dist < best) {
                    best = dist;
                    nearest = chara;
                }
            }
        }
    }
    float t = 0.0f;
    if (nearest != 0) {
        float dist = (float)sqrt(best) - nearest->m5B0;
        if (mRange > 0.0f) {
            t = __MIN_unk(__MAX_unk(1.0f - dist / mRange, 0.0f), 1.0f);
        }
    }
    float a = mFrom + t * (mTo - mFrom);
    FragileColor_unk color(mMaterial->mColor);
    color.a = a;
    mMaterial->mColor = color.ToGX();
    if (a < 0.99f) {
        mMaterial->fn_8007FAF0(3);
        model->mFlags |= 0x10;
    } else {
        mMaterial->fn_8007FAF0(mMode);
        model->mFlags &= ~0x10;
    }
}

void FragileObject::vf3C(void* lens) {
    mModelAfter.mFlags |= 0x10000;
    mModelAfter.m1DC = (Lens_unk*)lens;
    mModelBefore.mFlags |= 0x10000;
    mModelBefore.m1DC = (Lens_unk*)lens;
}

atn::Model* FragileObject::vf38() {
    int state = m54;
    if (state == 7) {
        return 0;
    }
    bool broken = false;
    if (state >= 4 && 6 >= state) {
        broken = true;
    }
    if (broken) {
        return &mModelAfter;
    }
    return &mModelBefore;
}

int FragileObject::vf5C() {
    return m54 < 4;
}

float FragileObject::vf58() {
    return mSphere.mSphere.mRadius;
}

atn::Vector3 FragileObject::vf54() {
    return mSphere.mSphere.mCenter;
}

atn::ColliBase* FragileObject::vf50() {
    return &mSphere;
}

} // namespace gmk
