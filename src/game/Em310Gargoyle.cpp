#include "game/Em310Gargoyle.h"
#include "atn/Csv.h"
#include "game/EnemyManager.h"
#include "math/Vec.h"
#include <string.h>

extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78
struct Field_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8; // inferred: route tables (as in gmk/ReinforcePoint.cpp)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_803C17FC(RouteTable_unk* table, int point); // inferred: route point -> index
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: per type data, indexed by type - 0x385
extern "C" Em310Gargoyle::Data_unk lbl_80787F68[];
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
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (as in game/Em309Ghoul.cpp)

// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 99.93% - the field pointer is kept in r28 and the type read into r9 (here r9/r12); ~8 variants
// (field/table locals in several orders, type read apart)
Em310Gargoyle::Em310Gargoyle(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14C4(1.0f), m14C8(0), m14CC(0.0f), m14D0(0),
      m14D4(0), m14DC(0), m14E0(0), m14E4(0), m14EC(0), m14F0(0), m14F1(0), m150D(1), m1588(0), m158C(0),
      m15D0(0.0f), m15E0(2), m15F0(0), m15F1(0), m1604(0), m1608(1.3f), m160C(0.5f) {
    int type = *(int*)info - 0x385;
    RouteTable_unk* table = lbl_8087F430->m10D8;
    m14C0 = type;
    mData = lbl_80787F68[type];
    for (int i = 0; i < 4; i++) {
        mData.mOfs[i] = table->FindPlace_unk(mData.mSeats[i].m04)->mPos - table->FindPlace_unk(mData.mSeats[i].m00)->mPos;
    }
    for (int i = 0; i < 3; i++) {
        m1500[i] = 0;
        m14F4[i] = 0;
    }
    atn::Csv csv(info->m2C, strlen(info->m2C));
    for (;;) {
        const char* tok = csv.fn_8005B9CC();
        if (tok == 0 || strcmp(tok, "") == 0) {
            break;
        }
        if (strncmp(tok, "ride1", 5) == 0) {
            m14F4[0] = fn_800DC12C(tok + 6);
        } else if (strncmp(tok, "ride2", 5) == 0) {
            m14F4[1] = fn_800DC12C(tok + 6);
        } else if (strncmp(tok, "ride3", 5) == 0) {
            m14F4[2] = fn_800DC12C(tok + 6);
        } else if (strncmp(tok, "sp", 2) == 0) {
            mData.m14 = fn_800DC12C(tok + 3);
        } else if (strncmp(tok, "ep", 2) == 0) {
            mData.m18 = fn_800DC12C(tok + 3);
        }
    }
    m150C = 0;
    m15D4.fn_8023780C("ef_uc037.efp");
    m15F4.fn_8023780C("ef_cm008.efp");
}

Em310Gargoyle::~Em310Gargoyle() {}

// NONMATCHING: 99.91% - r4/r5 of the flag word m14A8 (the original loads it straight into r5); the flags
// are read into a local before the other stores (~35 variants in total)
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
        int flags = m14A8;
        mRoute.mA0 = 2;
        m56C = 1.0f;
        m14A8 = flags & ~0x04000000;
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

// inferred: the capsule goes from the "hips" node one unit along the facing direction
// NONMATCHING: 99.71% - stack order of the three rotation blocks (the original puts the y block lowest and the
// matrix highest); ~12 variants (inline helper with Mtx r/t in both orders, by-value matrix helpers, function
// scope matrices in four orders, pointer to the matrix)
void Em310Gargoyle::vf20() {
    m620 = m5B0;
    m614 = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
    atn::Vector3 from = m614;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    Mtx rz;
    Mtx tz;
    Mtx rx;
    Mtx tx;
    Mtx ry;
    Mtx ty;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != mRot.y) {
        PSMTXRotRad(ry, 'y', mRot.y);
        PSMTXConcat(p->m, ry, ty);
        __MtxCopy(p->m, ty);
    }
    if (0.0f != mRot.x) {
        PSMTXRotRad(rx, 'x', mRot.x);
        PSMTXConcat(p->m, rx, tx);
        __MtxCopy(p->m, tx);
    }
    if (0.0f != mRot.z) {
        PSMTXRotRad(rz, 'z', mRot.z);
        PSMTXConcat(p->m, rz, tz);
        __MtxCopy(p->m, tz);
    }
    PSMTXMultVec(m.m, &dir, &dir);
    dir += from;
    mColli.m3C = from;
    mColli.m48 = dir;
    mColli.m54 = m620;
}

// inferred: route point by 1-based index (weak copy fn_8030C190; name invented)
inline RoutePoint_unk* GetRoutePoint_unk(RouteTable_unk* table, int index) {
    return &table->mPoints[index - 1];
}

void Em310Gargoyle::fn_8030CF84(int warp) {
    m58C = 7;
    if (warp != 0) {
        int start = fn_803C17FC(lbl_8087F430->m10D8, mData.m14);
        m158C = fn_803C17FC(lbl_8087F430->m10D8, mData.m18);
        m1588 = start;
        m1590 = 0.0f;
        mPos = GetRoutePoint_unk(lbl_8087F430->m10D8, start)->mPos;
        m15D0 = 0.0f;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.mTracks[0].m10 = 1.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    }
    int down = 1;
    if (m1500[1] != 0 && m1500[2] != 0 &&
        (((m1500[1]->mFlags & 4) != 4 && (m1500[1]->mStatus.m0C & 0x20) != 0x20) ||
         ((m1500[2]->mFlags & 4) != 4 && (m1500[2]->mStatus.m0C & 0x20) != 0x20))) {
        down = 0;
    }
    fn_80370AE4(lbl_8087F430, mData.m08, down);
    m150D = 0;
    m14EC = 0;
    m150C = 0;
}


// NONMATCHING: 99.63% - stack slots of the direction and of the sum temporary swapped, r4/r5 of the copy of
// the neck position (__VecCopy family); ~10 variants (local order, named sum, += form, scale helper)
void Em310Gargoyle::fn_8030D0D8() {
    m58C = 8;
    fn_8016E970(3);
    RouteTable_unk* table = lbl_8087F430->m10D8;
    m1594 = GetRoutePoint_unk(table, m158C)->mPos;
    m15A0 = mPos;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    atn::Vector3 front = dir * 150.0f;
    m15AC = mPos + front;
    m15AC.y += 20.0f;
    int id = mData.mSeats[m14E8].m00;
    m15B8 = table->FindPlace_unk(id)->mPos;
    id = mData.mSeats[m14E8].m04;
    m15C4 = table->FindPlace_unk(id)->mPos;
    m1590 = 0.0f;
    m14B0 = 0;
    m15E4 = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    fn_800EB7A0(this);
}
// inferred: unused file-scope object with an empty constructor: only its (empty) static
// initializer remains (fn_8030DA1C, the .ctors entry of this unit)
static Color4f_unk sUnused_unk;
