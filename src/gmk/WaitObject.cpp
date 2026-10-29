#include "gmk/WaitObject.h"
#include "atn/Memory.h"
#include "atn/Csv.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/PlayerTask.h"

extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" double cos(double x);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" int rand();
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
// inferred: camera (lbl_8087EFB4) and field (lbl_8087F430); classes unknown
struct WaitCamera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" WaitCamera_unk* lbl_8087EFB4;
struct WaitField_unk;
extern "C" WaitField_unk* lbl_8087F430;
struct WaitMapGroup_unk {
    unsigned char m00[0x48];
    int m48; // inferred: kind (1: the object is only updated near the camera)
};
extern "C" WaitMapGroup_unk* fn_80373148(WaitField_unk* field);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: bullet manager (lbl_8087F048, see gmk/Brazier.cpp)
struct WaitBullets_unk;
extern "C" WaitBullets_unk* lbl_8087F048;
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_800F8548(WaitBullets_unk* mgr);
extern "C" void fn_800FAB80(WaitBullets_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9,
                            float arg10, float arg11);
// inferred: bullet that hit the object (GimmickHitMsg_unk + 0x2C; names invented)
struct WaitHitBullet_unk {
    unsigned char m00[0x04];
    int mId;  // 0x04: inferred: bullet id
    unsigned char m08[0x3C - 0x08];
    int mPower; // 0x3C
};
// inferred: clamps (MAX/MIN macros: the value is computed again; names invented)
#define Max_unk(a, b) ((a) > (b) ? (a) : (b))
#define Min_unk(a, b) ((a) < (b) ? (a) : (b))
// inferred: area of a trigger (WaitTrigger_unk::m08; names invented)
struct WaitArea_unk {
    float mRadius;    // 0x08
    float mValue;     // 0x0C: inferred: minimum of CharaTask::m570 for type 3, frames for type 1
    float mAngle;     // 0x10: inferred: half angle of the view cone in degrees (0: all around)
};
// debug menu (see gmk/Cannon.cpp); lbl_8087EF10 is declared in the headers
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
// inferred: collision target of a state (the collision of mStates[]: position at 0x3C, radius at
// 0x48)
struct WaitTarget_unk {
    unsigned char m00[0x3C];
    atn::Vector3 mPos; // 0x3C
    float mRadius;     // 0x48
};

namespace gmk {

// inferred: plays the effect of a state on the model (the vectors and the color are locals; name
// invented)
inline void WaitObject::PlayEfp_unk() {
    fn_80232B7C(this, m54);
    lbl_8087F3C0->mB8 = 1;
    atn::EfpFile* efp = &mEfp[m54];
    atn::Model* model = &mModel;
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
    lbl_8087F3C0->mB8 = 0;
}

GimmickCommon* WaitObject::fn_80418EAC(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) WaitObject(parent, id, arg);
    }
    return 0;
}

GimmickCommon* WaitObject::fn_80418F30(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        WaitObject* obj = new (12, "", "", 0) WaitObject(parent, desc->mId, desc->mArg);
        if (obj != 0) {
            obj->m6C = desc->mPos;
            obj->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            obj->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
            obj->mDesc = desc;
        }
        return obj;
    }
    return 0;
}

// NONMATCHING: 98.84% - the zero is kept in r31 instead of r30 (register numbers only).
WaitObject::WaitObject(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mNumTriggers(0), m7F8(0), m7FC(0.0f, 0.0f, 0.0f),
      m808(0.0f, 0.0f, 0.0f), mDesc(0) {
    m54 = 0;
    mE8 = 6;
    mEC = 0;
}

WaitObject::~WaitObject() {
    for (unsigned int i = 0; i < 7; i++) {
        if (mStates[i].mColli != 0) {
            delete mStates[i].mColli;
        }
    }
}

int WaitObject::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        for (int i = 0; i < 7; i++) {
            if (mStates[i].mColli != 0) {
                mStates[i].mColli->mFlags |= 8;
                mStates[i].mColli->m0C = (int)this;
                mStates[i].mColli->m20 = 0x808F8;
            }
        }
        m7FC = m6C;
        m808 = m78;
        return 1;
    }
    return 0;
}

// NONMATCHING: 97.56% - the original branches over a `b` for m54 == 0 (bne + b); tried if/else,
// nested ifs and a switch.
void WaitObject::vf14() {
    if (mDesc != 0 && mDesc->m7C == 0) {
        return;
    }
    if (m54 == 0) {
        return;
    }
    if (lbl_8087F0A8->m18 != 0 && (mStates[m54].mFlags & 2) != 2 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

// inferred: per frame update: position (TRANS/TRANSINV move m24 over m24 frames from the initial
// position), rotation (TRANSINV turns by pi/2), model and collision, then the triggers
// NONMATCHING: 99.97% - only the name of the safe-bool member pointer of boost::scoped_ptr
// (lbl_8078DC58 in the original; see vf1C)
void WaitObject::vf10() {
    if (mDesc != 0 && mDesc->m7C == 0) {
        return;
    }
    atn::Vector3 d = lbl_8087EFB4->mPos - m6C;
    float dist = fn_805F9920(&d);
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 && fn_80373148(lbl_8087F430)->m48 == 1 &&
        dist > 360000.0f) {
        return;
    }
    if (m54 == 0) {
        return;
    }
    if ((mStates[m54].mFlags & 8) == 8) {
        float rate = Min_unk(Max_unk((float)m7F8 / mStates[m54].m24, 0.0f), 1.0f);
        m6C = m7FC + mStates[m54].mMove * rate;
    } else if ((mStates[m54].mFlags & 0x10) == 0x10) {
        float rate = Min_unk(Max_unk(1.0f - (float)m7F8 / mStates[m54].m24, 0.0f), 1.0f);
        m6C = m7FC + mStates[m54].mMove * rate;
    } else {
        m6C = m7FC;
    }
    if ((mStates[m54].mFlags & 0x10) == 0x10) {
        m78 = m808 + atn::Vector3(0.0f, 3.1415927f, 0.0f);
    } else {
        m78 = m808;
    }
    if (vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    if (mColli) {
        mColli->SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    }
    m7F8++;
    int trigger = fn_8041AAC0();
    if (trigger >= 0) {
        GimmickMsg_unk msg(mTriggers[trigger].mNext);
        vf40(&msg);
    }
}

// inferred: reads the gimmick file (MODEL, MODEL_LOD, MOTION, COLLI_OBJECT, EFP, STATE blocks ended by
// an empty field, TRIGGER, ROTATION, SCALE)
// NONMATCHING: 99.95% - the two loop flags of STATE are converted with extrwi instead of srwi, the
// COLLI temporaries swap r25/r26 and the TRIGGER values are addressed from the trigger (+8)
void WaitObject::vf20() {
    char path[0x100];
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.reset(new (12, "", "", 0) atn::ColliObject());
            mColli->fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            mEfp[atoi(csv.fn_8005B9CC())].fn_8023780C(path);
        } else if (strcmp(field, "STATE") == 0) {
            WaitState_unk state;
            int index = atoi(csv.fn_8005B9CC());
            state.m00 = fn_802180A8(csv.fn_8005B9CC());
            state.m04 = fn_800DC288(csv.fn_8005B9CC());
            state.m08 = atoi(csv.fn_8005B9CC()) == 1 ? 1 : 0;
            state.m09 = atoi(csv.fn_8005B9CC()) == 1 ? 1 : 0;
            state.m0C = fn_800DC288(csv.fn_8005B9CC());
            for (;;) {
                const char* key = csv.fn_8005B9CC();
                if (*key == 0) {
                    break;
                }
                if (strcmp(key, "COLLI") == 0) {
                    atn::Vector3 pos;
                    pos.x = fn_800DC288(csv.fn_8005B9CC());
                    pos.y = fn_800DC288(csv.fn_8005B9CC());
                    pos.z = fn_800DC288(csv.fn_8005B9CC());
                    atn::Matrix3x4 rot;
                    PSMTXRotRad(rot.m, 'y', m78.y);
                    PSMTXMultVec(rot.m, &pos, &pos);
                    pos += m6C;
                    atn::ColliSphere::Sphere_unk sphere;
                    sphere.mCenter = pos;
                    sphere.mRadius = fn_800DC288(csv.fn_8005B9CC());
                    if (sphere.mRadius > 0.0f) {
                        atn::ColliSphere* colli = new (12, "", "", 0) atn::ColliSphere();
                        state.mColli = colli;
                        colli->mSphere = sphere;
                        state.mFlags |= 1;
                    }
                } else if (strcmp(key, "DRAWOFF") == 0) {
                    state.mFlags |= 2;
                } else if (strcmp(key, "NOMOTION") == 0) {
                    state.mFlags |= 4;
                } else if (strcmp(key, "TRANS") == 0) {
                    state.mMove.x = fn_800DC288(csv.fn_8005B9CC());
                    state.mMove.y = fn_800DC288(csv.fn_8005B9CC());
                    state.mMove.z = fn_800DC288(csv.fn_8005B9CC());
                    state.m24 = fn_800DC288(csv.fn_8005B9CC());
                    atn::Matrix3x4 rot;
                    PSMTXRotRad(rot.m, 'y', m78.y);
                    PSMTXMultVec(rot.m, &state.mMove, &state.mMove);
                    state.mFlags |= 8;
                } else if (strcmp(key, "TRANSINV") == 0) {
                    state.mMove.x = fn_800DC288(csv.fn_8005B9CC());
                    state.mMove.y = fn_800DC288(csv.fn_8005B9CC());
                    state.mMove.z = fn_800DC288(csv.fn_8005B9CC());
                    state.m24 = fn_800DC288(csv.fn_8005B9CC());
                    atn::Matrix3x4 rot;
                    PSMTXRotRad(rot.m, 'y', m78.y);
                    PSMTXMultVec(rot.m, &state.mMove, &state.mMove);
                    state.mFlags |= 0x10;
                } else if (strcmp(key, "PLAYERONLY") == 0) {
                    state.mFlags |= 0x20;
                } else if (strcmp(key, "ATTENTION") == 0) {
                    state.mFlags |= 0x40;
                }
            }
            mStates[index] = state;
        } else if (strcmp(field, "TRIGGER") == 0) {
            WaitTrigger_unk trigger;
            trigger.mState = atoi(csv.fn_8005B9CC());
            trigger.mNext = atoi(csv.fn_8005B9CC());
            trigger.mType = atoi(csv.fn_8005B9CC());
            for (int i = 0; i < 5; i++) {
                ((float*)trigger.m08)[i] = fn_800DC288(csv.fn_8005B9CC());
            }
            WaitTrigger_unk* slot = &mTriggers[mNumTriggers];
            if (slot != 0) {
                *slot = trigger;
            }
            mNumTriggers++;
        } else if (strcmp(field, "ROTATION") == 0) {
            atn::Vector3 rot;
            rot.x = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            rot.y = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            rot.z = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            m78 += rot;
        } else if (strcmp(field, "SCALE") == 0) {
            atn::Vector3 scale;
            scale.x = fn_800DC288(csv.fn_8005B9CC());
            scale.y = fn_800DC288(csv.fn_8005B9CC());
            scale.z = fn_800DC288(csv.fn_8005B9CC());
            m90.x *= scale.x;
            m90.y *= scale.y;
            m90.z *= scale.z;
        }
    } while (csv.fn_8005BBF8() != 0);
}

// inferred: places the collision object with the model and enables it, then enters the start state
// NONMATCHING: 99.9% - stack slots of the two safe-bool temporaries swapped, and the names of the
// safe-bool member pointers of boost::scoped_ptr
// (lbl_8078DC40/lbl_8078DC4C in the original, local objects here; see vf1C)
void WaitObject::vf28() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    if (mColli) {
        mColli->SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    }
    if (mColli) {
        mColli->mFlags |= 1;
    }
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

// NONMATCHING: 99.85% - the safe-bool member pointer of boost::scoped_ptr is lbl_8078DC64 in the
// original (a local object here, @NNNN).
int WaitObject::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli && mColli->fn_800580BC() != 0) {
        return 1;
    }
    for (int i = 0; i < 7; i++) {
        if (mEfp[i].fn_80237874() != 0) {
            return 1;
        }
    }
    return 0;
}

void WaitObject::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

// NONMATCHING: 93.33% - the original reloads m54 in the loop and uses other registers.
int WaitObject::vf60() {
    if (m54 == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk& t = mTriggers[i];
        if (t.mState == m54 && t.mType == 4) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 99.94% - only .sdata2 order (vf10 and others are not decompiled yet).
int WaitObject::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() != 0 && flag == 0) {
        atn::Vector3 d = m6C - *pos;
        float dist = fn_805F9920(&d);
        if (dist > 0.0f) {
            PSVECNormalize(&d, &d);
        }
        for (unsigned int i = 0; i < mNumTriggers; i++) {
            WaitTrigger_unk& t = mTriggers[i];
            if (t.mState == m54 && t.mType == 4) {
                float radius = *(float*)t.m08;
                if (dist < radius * radius) {
                    return PSVECDotProduct(&d, dir) > 0.5f;
                }
            }
        }
    }
    return 0;
}

void WaitObject::vf4C(CharaTask* chara) {
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk& t = mTriggers[i];
        if (t.mState == m54 && t.mType == 4) {
            GimmickMsg_unk msg(t.mNext);
            vf40(&msg);
        }
    }
}

// NONMATCHING: 97.50% - the original reloads the collision pointer for the return.
int WaitObject::vf50() {
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk& t = mTriggers[i];
        if (t.mState == m54 && t.mType == 5 && GetColli_unk(m54) != 0) {
            return (int)GetColli_unk(m54);
        }
    }
    return 0;
}

atn::Vector3 WaitObject::vf54() {
    WaitTarget_unk* target = (WaitTarget_unk*)vf50();
    if (target != 0) {
        return target->mPos;
    }
    return lbl_807C7030;
}

float WaitObject::vf58() {
    WaitTarget_unk* target = (WaitTarget_unk*)vf50();
    return target != 0 ? target->mRadius : 0.0f;
}

int WaitObject::vf5C() {
    return vf58() > 0.0f;
}

atn::Model* WaitObject::vf38() {
    return &mModel;
}

// inferred: debug menu of the object and of the collision targets of the states
// NONMATCHING: 96% - the string pool offsets depend on the strings of vf20 (not decompiled yet) and
// the scheduling of the first item (as gmk::CharaLight::vf6C)
void WaitObject::vf6C(void* menu) {
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
    fn_800874C8(group, "locData", &m58, 0, 99, 1, 0, 0);
    mB0.fn_803F11F8(group);
    for (int i = 0; i < 7; i++) {
        if (mStates[i].mColli != 0) {
            char sub[0x40];
            sprintf(sub, "targetCollision%d", i);
            void* target = fn_8008937C(group, sub);
            fn_80087E9C(target, "pos", &((WaitTarget_unk*)mStates[i].mColli)->mPos, 0, 0, -9999.0f, 9999.0f, 0.25f);
            fn_8008771C(target, "radius", &((WaitTarget_unk*)mStates[i].mColli)->mRadius, 0, 0, -9999.0f, 9999.0f,
                        0.25f);
        }
    }
}

// inferred: index of the trigger of the current state that fires: 1 after mValue frames, 2/3 a
// character in the area, 6 no character in the area
int WaitObject::fn_8041AAC0() {
    int found = -1;
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk* t = &mTriggers[i];
        if (t->mState != m54) {
            continue;
        }
        switch (t->mType) {
        case 1:
            if ((float)m7F8 > ((WaitArea_unk*)t->m08)->mAngle) {
                found = i;
            }
            break;
        case 2:
        case 3:
        case 6: {
            int hit = 0;
            if (lbl_8087F8A0 != 0) {
                hit = InArea_unk(lbl_8087F8A0, t);
            }
            if (hit == 0 && EnemyManager::sInstance != 0 && (mStates[m54].mFlags & 0x20) != 0x20) {
                hit = InArea_unk(EnemyManager::sInstance, t);
            }
            if (hit == 0 && NpcManager::sInstance != 0 && (mStates[m54].mFlags & 0x20) != 0x20) {
                hit = InArea_unk(NpcManager::sInstance, t);
            }
            if ((hit != 0 && t->mType != 6) || (hit == 0 && t->mType == 6)) {
                found = i;
            }
            break;
        }
        }
    }
    return found;
}

// NONMATCHING: 98.5% (each instance) - stack slots of front/dir/d and the float registers of the
// angle, radius, value and distance
template <class T>
int WaitObject::InArea_unk(CharaManager<T>* manager, WaitTrigger_unk* trigger) {
    if (manager == 0) {
        return 0;
    }
    atn::Matrix3x4 rot;
    atn::Vector3 dir;
    PSMTXRotRad(rot.m, 'y', m78.y);
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    dir = front;
    PSMTXMultVec(rot.m, &dir, &dir);
    WaitArea_unk* area = (WaitArea_unk*)trigger->m08;
    float angle = area->mAngle * 0.017453292f;
    float radius = area->mRadius;
    float value = area->mValue;
    if (angle <= 0.0f) {
        angle = 3.1415927f;
    }
    for (T* chara = manager->mHead; chara != 0; chara = chara->mNext) {
        if (chara->CanAct_unk()) {
            atn::Vector3 d = m6C - chara->mPos;
            float dist = fn_805F9920(&d);
            if (dist > 0.0f) {
                PSVECNormalize(&d, &d);
            }
            if (dist < radius * radius && PSVECDotProduct(&dir, &d) >= (float)cos(angle) &&
                (trigger->mType != 3 || chara->m570 > value)) {
                return 1;
            }
        }
    }
    return 0;
}

// inferred: a bullet with enough power switches the state (type 5 trigger) and, when the trigger has a
// bullet id, fires that bullet from the collision target
// NONMATCHING: 91.8% - the original reloads the collision pointer of the state for the position copy
// (one more lwz) and schedules the copy differently
int WaitObject::vf44(GimmickHitMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (vf5C() == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk* t = &mTriggers[i];
        if (t->mState != m54 || t->mType != 5 || GetColli_unk(m54) == 0) {
            continue;
        }
        WaitHitBullet_unk* hit = *(WaitHitBullet_unk**)&msg->m28[4];
        if (hit == 0 || (float)hit->mPower < ((WaitArea_unk*)t->m08)->mRadius) {
            continue;
        }
        WaitBullets_unk* mgr = lbl_8087F048;
        int bullet = (int)((WaitArea_unk*)t->m08)->mValue;
        if (mgr != 0 && bullet > 0 && hit->mId != bullet) {
            atn::Vector3 pos = ((WaitTarget_unk*)GetColli_unk(m54))->mPos;
            atn::Vector3 rot(0.0f, 0.0f, 0.0f);
            fn_800FAB80(mgr, 0, fn_80219E6C(bullet), fn_800F8548(mgr), &pos, &rot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        GimmickMsg_unk next(t->mNext);
        vf40(&next);
        break;
    }
    return m54;
}

// inferred: switches the state: effect and motion of the state, collision of the state, timers of
// the type 1 triggers (mValue frames plus a random part)
// NONMATCHING: 99.2% - order of the stores of the two zero vectors of the effect (as
// gmk::OpenBoard::vf10) and the load of the blend of the state before the motion id
int WaitObject::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    if (msg->mType >= 1) {
        fn_80239DAC(lbl_8087F3C0, this, m54, 1);
        m54 = msg->mType;
        if ((unsigned int)m54 > 6) {
            m54 = 0;
        }
        PlayEfp_unk();
        m7F8 = 0;
        if (mColli) {
            mColli->mFlags |= 1;
        }
        WaitState_unk* state = &mStates[m54];
        mModel.fn_80097C08(0, state->m00, state->m08, state->m09, 1, state->m04, 0.1f);
        mModel.mTracks[0].m08 = state->m0C;
        if ((state->mFlags & 4) == 4) {
            mModel.mTracks[0].m0C = 0.0f;
        } else {
            mModel.mTracks[0].m0C = 1.0f;
        }
        for (int i = 0; i < 7; i++) {
            if (mStates[i].mColli != 0) {
                mStates[i].mColli->mFlags &= ~1;
            }
        }
        if (vf5C() != 0 && state->mColli != 0) {
            state->mColli->mFlags |= 1;
        }
        for (unsigned int i = 0; i < mNumTriggers; i++) {
            WaitTrigger_unk* t = &mTriggers[i];
            if (t->mState == m54 && t->mType == 1) {
                WaitArea_unk* area = (WaitArea_unk*)t->m08;
                area->mAngle = area->mRadius;
                if (area->mValue > 0.0f) {
                    area->mAngle += (float)(rand() % (int)area->mValue);
                }
            }
        }
    } else {
        vf2C();
        fn_80239DAC(lbl_8087F3C0, this, m54, 1);
        m54 = msg->mType;
        if (mColli) {
            mColli->mFlags &= ~1;
        }
    }
    return m54;
}

} // namespace gmk
