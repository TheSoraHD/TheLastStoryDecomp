#include "game/Em310Gargoyle.h"
#include "game/EnemyManager.h"
#include "math/Vec.h"

extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets a counter
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects

// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

Em310Gargoyle::~Em310Gargoyle() {}

// NONMATCHING: 98.41% - r3/r4 of the flag word and of the string pool (the original computes "em310" first)
// and r3/r4/r5 of the stores at the end; ~30 variants (load helper, statement order)
int Em310Gargoyle::vf0C() {
    if (fn_8013655C() == 0 && !m15D4.fn_80237874() && !m15F4.fn_80237874()) {
        for (int i = 0; i < 3; i++) {
            m1500[i] = EnemyManager::sInstance->Find_unk(m14F4[i]);
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        void* group = m1600.Load("em310");
        fn_80087994(group, "DebugDisp", &m1604, 0, 0);
        fn_800874C8(group, "DamageCalcMode", &m15E0, 0, 2, 1, 0, 0);
        fn_8008771C(group, "BulletRotX", &m1608, 0, 0, -3.14f, 3.14f, 0.01f);
        fn_8008771C(group, "BulletRotY", &m160C, 0, 0, -3.14f, 3.14f, 0.01f);
        mRoute.mA0 = 2;
        m56C = 1.0f;
        m14A8 &= ~0x04000000;
        m58C = 6;
        fn_8016E970(3);
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.99% - only the offset of "head" in the string pool (strings of functions not written yet)
void Em310Gargoyle::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    switch (m15E0) {
    case 1:
        if (msg->mData != 0 && msg->mData->m04 == 0x13C) {
            msg->m0C |= 0x880;
        }
        CharaTask::vf3C(hit);
        break;
    case 2:
    case 0: {
        if (msg->mData != 0 && (msg->mData->m04 == 0x13C || msg->mData->m04 == 0x232C)) {
            msg->m0C |= 0x800;
        } else {
            msg->m68 = 0;
            msg->m90 = 1;
            break;
        }
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        atn::Vector3 d = msg->m1C - GetNodePos_unk(head);
        if (fn_805F9920(&d) < 0.05f) {
            msg->m0C |= 0x80000000;
        }
        fn_80151448(msg);
        vf40(hit);
        if (m15E0 == 0) {
            mStatus.mParamB.m08 = 1000.0f;
        }
        break;
    }
    }
}

void Em310Gargoyle::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (m14F1 == 0) {
            m14F1 = 1;
            fn_80370AE4(lbl_8087F430, 0xCF, 1);
            fn_80370AE4(lbl_8087F430, 0x66, fn_80370A78(lbl_8087F430, 0x66) + 1);
            mMotion.fn_80097C08(0, 0x33, 0, 0, 1, 0.0f, 150.0f);
            if (fn_80370A78(lbl_8087F430, 0x66) <= 3) {
                fn_80232B7C(this, 100);
                Color4f_unk color;
                atn::Vector3 rot;
                atn::Vector3 pos;
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
                fn_8023A8B4(lbl_8087F3C0, &m15D4, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
            }
        }
    } else if ((float)msg->m68 > 0.25f * (float)mStatus.mParamB.m84) {
        mMotion.fn_80097C08(0, 0x33, 0, 0, 1, 0.0f, 150.0f);
    }
}

// inferred: the damage sphere follows the collision sphere (m614/m620)
void Em310Gargoyle::vf54() {
    if (m624.mSize != 0) {
        Item14_unk* item = m624.mData;
        item->m04.mCenter = m614;
        item->m04.mRadius = m620;
    }
}

// inferred: unused file-scope object with an empty constructor: only its (empty) static
// initializer remains (fn_8030DA1C, the .ctors entry of this unit)
static Color4f_unk sUnused_unk;
