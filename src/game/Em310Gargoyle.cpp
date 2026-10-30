#include "game/Em310Gargoyle.h"
#include "atn/Keyboard.h"
#include "atn/Csv.h"
#include "game/EnemyManager.h"
#include "math/Vec.h"
#include <string.h>
#include "atn/DrawHelper.h"

extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78
struct Field_unk {
    RouteTable_unk* fn_8013C504(); // inferred: returns m10D8
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
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
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
    m15D4.LoadRequest("ef_uc037.efp");
    m15F4.LoadRequest("ef_cm008.efp");
}

Em310Gargoyle::~Em310Gargoyle() {}

// NONMATCHING: 99.91% - r4/r5 of the flag word m14A8 (the original loads it straight into r5); the flags
// are read into a local before the other stores (~35 variants in total)
int Em310Gargoyle::vf0C() {
    if (fn_8013655C() == 0 && !m15D4.IsLoading() && !m15F4.IsLoading()) {
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

// inferred: route point by 1-based index (first emitted here as fn_8030C190)
extern "C" inline RoutePoint_unk* fn_8030C190(RouteTable_unk* table, int index) {
    return &table->mPoints[index - 1];
}

// inferred: unit vector rotated around y (name invented, as Front_unk in game/Em203GreaterDaemon.cpp)
inline atn::Vector3 GargoyleFront_unk(const float& angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', angle);
    PSMTXMultVec(m, &front, &front);
    return front;
}
inline atn::Vector3 GargoyleFront2_unk(float angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', angle);
    PSMTXMultVec(m, &front, &front);
    return front;
}
inline atn::Vector3 GargoyleFront3_unk(const atn::Vector3& rot) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &front, &front);
    return front;
}

// inferred: empty release asserts (rule of the autoinline threshold, as WARLION_ASSERT_UNK in
// game/Em403WarLion.cpp). The original calls every inline helper of fn_8030C490 out of line, so it is over
// the inline budget; this accessor carries the missing cost (fewer than 50 keeps everything inlined).
#define GARGOYLE_ASSERT_UNK(x) ((void)0)
// inferred: field accessor (weak copy 0x80121F00 of another file, as in game/Em203GreaterDaemon.cpp)
extern "C" inline Field_unk* fn_80121F00() {
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0); GARGOYLE_ASSERT_UNK(0);
    return lbl_8087F430;
}
// inferred: debug drawing (lbl_8087EEB0, as in game/ChaserNpc.cpp) and its accessor (weak copy 0x800F7250)
extern "C" inline atn::DrawHelper* fn_800F7250() {
    return lbl_8087EEB0;
}
extern "C" int sprintf(char* buf, const char* fmt, ...);
namespace atn {
// inferred: [0,1] float channel to 0..255 (as in atn/D2A_Window.cpp; name invented)
inline unsigned int GargoyleToU8_unk(float f) {
    if (f >= 1.0f) {
        return 255;
    }
    if (f <= 0.0f) {
        return 0;
    }
    return (unsigned int)(f * 255.0f + 0.5f);
}
// inferred: RGBA color of the engine (ToU32: weak copy 0x800F8348, kept from another file)
struct Color4f_unk {
    Color4f_unk() {}
    // inferred: weak copy 0x800D8814 (atn/CompositeIntensityTintMulti.cpp)
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    // inferred: first emitted here as fn_8030B408 (also called by game/GameUI and game/MapPartsGimmickLoc)
    void Set_unk(float r_, float g_, float b_, float a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    unsigned int ToU32() const {
        unsigned int ur = GargoyleToU8_unk(r);
        unsigned int ug = GargoyleToU8_unk(g);
        unsigned int ub = GargoyleToU8_unk(b);
        unsigned int ua = GargoyleToU8_unk(a);
        return (ua << 24) | (ur << 16) | (ug << 8) | ub;
    }
    float r, g, b, a;
};
} // namespace atn
// inferred: rotation matrix from euler angles (weak copy 0x800109E0, as in game/Em203GreaterDaemon.cpp)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
// inferred: radians to degrees (0x802A7964 of game/Em204Mulue.cpp; its constant is not in this file's pool)
extern "C" float fn_802A7964(float rad);

// inferred: point of a cubic spline through four points (weak copy 0x8028AC70, kept from
// game/Em203GreaterDaemon.cpp)
extern "C" inline atn::Vector3 fn_8028AC70(const atn::Vector3& p0, const atn::Vector3& p1, const atn::Vector3& p2,
                                           const atn::Vector3& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;
    return (p0 * (-t3 + 2.0f * t2 - t) + p1 * (3.0f * t3 - 5.0f * t2 + 2.0f) + p2 * (-3.0f * t3 + 4.0f * t2 + t) +
            p3 * (t3 - t2)) * 0.5f;
}
extern "C" float fn_800F8524(const float* a, const float* b, float t); // inferred: lerp of two floats
// inferred: route between two points (8 bytes, as in game/Em203GreaterDaemon.cpp)
struct RoutePath_unk {
    int m00;
    int m04;
};
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: next point towards `point`
// inferred: bullet manager (lbl_8087F048, class unknown); fn_8010A308 moves a hit capsule (as in
// game/EnemySeaLizerd.cpp)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_8010A308(Effect_unk* mgr, const atn::Vector3* from, const atn::Vector3* to, float radius);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
// inferred: wraps an angle to [-180, 180] (0x802A7910 of game/Em204Mulue.cpp; constants not in this pool)
extern "C" float fn_802A7910(float deg);
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data by id
// inferred: bullet manager accessor (CharaTask.cpp) and bullet launcher (as in game/Em203GreaterDaemon.cpp)
Effect_unk* fn_8013A194();
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet
// inferred: model matrix (weak copy 0x80266154, as in gmk/Cannonball.cpp)
extern "C" inline atn::Matrix3x4* fn_80266154(atn::Model* model) {
    return (atn::Matrix3x4*)model->mMtx;
}
// inferred: enemy list accessors (weak copies of other files, as in gmk/Cannonball.cpp)
extern "C" inline EnemyManager* fn_801A03E0() {
    return EnemyManager::sInstance;
}
extern "C" inline CharaTask* fn_801A0408(EnemyManager* mgr) {
    return mgr->mHead;
}
extern "C" inline CharaTask* fn_801A03EC(CharaTask* chara) {
    return ((EnemyTask*)chara)->mNext;
}
// inferred: accessor of the control data (weak copy 0x800F530C)
extern "C" inline CharaC58_unk* fn_800F530C(CharaTask* chara) {
    return &chara->mC58;
}
// inferred: length in the xz plane (weak copy 0x801C3910, as in game/Em203GreaterDaemon.cpp)
extern "C" double sqrt(double x);
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}
// inferred: rotation matrix from euler angles, z then x then y (first emitted here)
extern "C" inline atn::Matrix3x4 fn_8030CA38(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}

// inferred: a rider sits on the seat node `name` and plays the riding motion
inline int Em310Gargoyle::SeatMtx_unk(const char* name) {
    return (int)mMotion.GetNodeMtx_unk(name);
}

// NONMATCHING: 99.79% - registers of the inlined fn_8030D0D8 (same residue as fn_8030D0D8) and .sdata2 offsets
void Em310Gargoyle::vf10() {
    if (fn_80370A78(lbl_8087F430, mData.m04) == 0) {
        mC58.mC0 = 0;
        return;
    }
    switch (m15E0) {
    case 0:
        m1458 = 1.0f;
        mFlag22 = 0;
        break;
    case 1:
    case 2:
        m1458 = 0.003f;
        mFlag22 = 1;
        mStatus.mParamB.m9C |= 0x10;
        break;
    }
    m500 = 1.0f;
    m504 = 1.0f;
    m508 = 1.0f;
    m14B4++;
    mStatus.fn_8013322C(0x20);
    if (m150D != 0) {
        if (m1500[0] != 0) {
            m1500[0]->mC58.mC0 = 0;
            m1500[0]->fn_801765D8();
        }
        if (m1500[1] != 0) {
            m1500[1]->mC58.mC0 = 0;
            m1500[1]->fn_801765D8();
        }
        if (m1500[2] != 0) {
            m1500[2]->mC58.mC0 = 0;
            m1500[2]->fn_801765D8();
        }
    }
    mC58.mC0 = 1;
    if (mC58.mC0 == 0) {
        fn_8016E970(3);
    } else {
        switch (m58C) {
        case 7:
            fn_8030C490();
            break;
        case 8:
            fn_8030CBC0();
            break;
        default:
            fn_8030CF84(1);
            break;
        }
    }
    fn_80145334();
    int flag = fn_80370A78(lbl_8087F430, mData.m08);
    if (flag == 1) {
        if (m150C == 0 && m1500[1] != 0 && m1500[2] != 0) {
            m1500[1]->mC58.mC0 = 0;
            m1500[1]->fn_801765D8();
            m1500[2]->mC58.mC0 = 0;
            m1500[2]->fn_801765D8();
            m1500[1]->mF1C = SeatMtx_unk("seatpoint_LF");
            m1500[2]->mF1C = SeatMtx_unk("seatpoint_RF");
            for (int i = 1; i <= 2; i++) {
                m1500[i]->mMotion.fn_80097C08(0, 0x28, 1, 0, 1, 0.0f, 0.1f);
                m1500[i]->mMotion.mTracks[0].m0C = 0.0f;
                m1500[i]->mMotion.m34C = 1;
                m1500[i]->mMotion.mTracks[0].m20 = 1.0f;
                m1500[i]->mFlag3 = 1;
                m1500[i]->fn_80145334();
            }
        }
    } else if (flag == 2) {
        if (m150C == 0 && m1500[0] != 0) {
            m1500[0]->mC58.mC0 = 0;
            m1500[0]->fn_801765D8();
            m1500[0]->mF1C = SeatMtx_unk("seatpoint_RF");
            m1500[0]->mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
            m1500[0]->mMotion.mTracks[0].m0C = 0.0f;
            m1500[0]->mMotion.m34C = 1;
            m1500[0]->mMotion.mTracks[0].m20 = 1.0f;
            m1500[0]->fn_80145334();
        }
    }
    if (m1604 != 0) {
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4B)) {
            fn_8030CF84(1);
        }
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4C)) {
            fn_8030D0D8();
        }
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4D)) {
            fn_80370AE4(lbl_8087F430, 0x66, 4);
        }
    }
}

// inferred: debug display (the seats, the flags 0xC9-0xCB and the seat directions when "DebugDisp" is on)
void Em310Gargoyle::vf14() {
    if (fn_80370A78(fn_80121F00(), mData.m04) == 0) {
        return;
    }
    fn_80149A30();
    int flag = fn_80370A78(fn_80121F00(), mData.m08);
    if (flag == 1) {
        if (m150C == 0 && m1500[1] != 0 && m1500[2] != 0) {
            m1500[1]->fn_80149A30();
            m1500[2]->fn_80149A30();
        }
    } else if (flag == 2) {
        if (m150C == 0 && m1500[0] != 0) {
            m1500[0]->fn_80149A30();
        }
    }
    if (m1604 == 0) {
        return;
    }
    for (int i = 0; i < 3; i++) {
        char buf[0x8];
        sprintf(buf, "flag::%d", fn_80370A78(fn_80121F00(), i + 0xC9));
        fn_800F7250()->fn_800616C0(200.0f, 100.0f + i * 10, 10000.0f, 10.0f, 10.0f, buf, atn::Color4f_unk(0.0f, 0.5f, 0.0f, 1.0f).ToU32(), 0.0f, 1, 1, 0);
    }
    atn::Matrix3x4 m1 = fn_800109E0(mRot);
    atn::Matrix3x4 m2 = fn_800109E0(mRot);
    atn::Matrix3x4 m3 = fn_800109E0(mRot);
    atn::Vector3 v1(0.0f, 0.0f, 5000000.0f);
    atn::Vector3 v2(0.0f, 0.0f, 5000000.0f);
    atn::Vector3 v3(0.0f, 0.0f, 5000000.0f);
    v1.fn_80011410(m1);
    fn_800F7250()->fn_80063764(mPos, mPos + v1, 100.0f, atn::Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f).ToU32());
    m2.fn_80149624(80.0f);
    v2.fn_80011410(m2);
    m3.fn_80149624(-80.0f);
    v3.fn_80011410(m3);
    fn_800F7250()->fn_80063764(mPos, mPos + v2, 100.0f, atn::Color4f_unk(1.0f, 0.0f, 0.0f, 1.0f).ToU32());
    fn_800F7250()->fn_80063764(mPos, mPos + v3, 100.0f, atn::Color4f_unk(1.0f, 0.0f, 0.0f, 1.0f).ToU32());
    for (int i = 0; i < 4; i++) {
        RouteTable_unk* table = fn_80121F00()->fn_8013C504();
        atn::Color4f_unk c(0.0f, 0.0f, 0.0f, 0.0f);
        c.Set_unk(0.25f * i, 0.25f * i, 0.25f * i, 1.0f);
        atn::Vector3 rot = mData.mOfs[i].ToRotation_unk().GetNormalize();
        rot.x = 0.0f;
        rot.z = 0.0f;
        atn::Matrix3x4 m = fn_800109E0(rot);
        atn::Vector3 v(0.0f, 0.0f, 1000.0f);
        v.fn_80011410(m);
        fn_800F7250()->fn_80063764(table->FindPlace_unk(mData.mSeats[i].m00)->mPos, (table->FindPlace_unk(mData.mSeats[i].m00)->mPos + v), 1000.0f, c.ToU32());
        char buf[0x20];
        sprintf(buf, "rot[%d]%f", i, fn_802A7964((mData.mOfs[i].ToRotation_unk() - mRot).GetNormalize().y));
        fn_800F7250()->fn_800616C0(300.0f, i * 10 + 100, 1000.0f, 20.0f, 10.0f, buf, c.ToU32(), 0.0f, 1, 1, 0);
    }
}

// NONMATCHING: 99.95% - only .sdata2 offsets: the pool here keeps the constants of the weak copies (ToU32 255,
// fn_8028AC70 -3/3/5, WrapAngle_unk pi/2pi) that the original linker stripped with them
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
        if (PSVECSquareMag(&d) < 900.0f) {
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

// NONMATCHING: 99.85% - only .sdata2 offsets: the pool here keeps the constants of the weak copies (ToU32 255,
// fn_8028AC70 -3/3/5, WrapAngle_unk pi/2pi) that the original linker stripped with them
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
    } else if ((float)msg->m68 > 0.05f * (float)mStatus.mParamB.m84) {
        mMotion.fn_80097C08(0, 0x33, 0, 0, 1, 0.0f, 150.0f);
    }
}

// NONMATCHING: 99.88% - only .sdata2 offsets (the pool keeps the constants of the weak copies, see vf3C)
// inferred: moves along the spline of the four route points `points` (t: progress 0..1) and banks with the
// change of direction (as Em203GreaterDaemon::fn_8028AA38)
void Em310Gargoyle::fn_8030B74C(const atn::Vector3* points, float t) {
    atn::Vector3 pos = fn_8028AC70(points[0], points[1], points[2], points[3], t < 1.0f ? t : 1.0f);
    atn::Vector3 dir = pos - mPos;
    dir.Normalize_unk();
    atn::Vector3 rot = dir.ToRotation_unk();
    mPos = pos;
    atn::Vector3 a = points[1] - points[0];
    atn::Vector3 b = points[2] - points[1];
    atn::Vector3 c = points[3] - points[2];
    if (a.LengthSq_unk() < 0.0001f) {
        a = b;
    }
    if (c.LengthSq_unk() < 0.0001f) {
        c = b;
    }
    a.Normalize_unk();
    b.Normalize_unk();
    c.Normalize_unk();
    float ra = a.ToRotation_unk().y;
    float rb = b.ToRotation_unk().y;
    float rc = c.ToRotation_unk().y;
    float d1 = atn::WrapAngle_unk(rb - ra);
    float d2 = atn::WrapAngle_unk(rc - rb);
    float bank = -fn_800F8524(&d1, &d2, t);
    bank *= 0.8f;
    bank += m15D0;
    m15D0 *= 0.8f;
    mRot.x = rot.x;
    mRot.y = rot.y;
    mRot.z += (bank - mRot.z) * 0.1f;
}

// NONMATCHING: 93.89% - the original reloads the copied points from the stack for the segment length (ours
// forwards z from the copy registers with frsp) and schedules the four copies one after the other; ~8 variants
// (plain/z-first copies, initializer list, pointer, length helpers)
// inferred: flies along the route from m1524 to m1528 (the speed m14C4 eases to 4); dives when the route
// turns down
void Em310Gargoyle::fn_8030B92C() {
    m14C4 += (4.0f - m14C4) * 0.2f;
    int next;
    int cur;
    int prev;
    int end;
    int start;
    start = fn_803C17FC(lbl_8087F430->m10D8, mData.m14);
    end = fn_803C17FC(lbl_8087F430->m10D8, mData.m18);
    cur = m1588;
    prev = m158C;
    next = fn_803CD958(&lbl_8087F430->m10D8->mPaths[cur - 1], end - 1);
    if (next == 0) {
        next = start;
    }
    int next2 = fn_803CD958(&lbl_8087F430->m10D8->mPaths[next - 1], end - 1);
    if (next2 == 0) {
        next2 = start;
    }
    atn::Vector3 points[4];
    points[0].Assign_unk(fn_8030C190(lbl_8087F430->m10D8, prev)->mPos);
    points[1].Assign_unk(fn_8030C190(lbl_8087F430->m10D8, cur)->mPos);
    points[2].Assign_unk(fn_8030C190(lbl_8087F430->m10D8, next)->mPos);
    points[3].Assign_unk(fn_8030C190(lbl_8087F430->m10D8, next2)->mPos);
    m1590 += m14C4 / (points[2] - points[1]).Length_unk();
    fn_8030B74C(points, m1590);
    if (m1590 >= 1.0f) {
        m158C = m1588;
        m1588 = next;
        m1590 = 0.0f;
        if (mMotion.mTracks[0].m00 == 0x14) {
            atn::Vector3 a = points[2] - points[1];
            atn::Vector3 b = points[3] - points[2];
            if (PSVECSquareMag(&a) > 0.001f && PSVECSquareMag(&b) > 0.001f) {
                PSVECNormalize(&a, &a);
                PSVECNormalize(&b, &b);
                if (b.ToRotation_unk().x - a.ToRotation_unk().x < -0.2617994f &&
                    b.ToRotation_unk().x < -0.17453292f) {
                    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
                }
            }
        }
    } else if (mMotion.mTracks[0].m00 != 0x14) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.fn_80097C08(0, m14F1 != 0 ? 0x2E : 0x14, 1, 0, 1, 0.0f, 0.1f);
        }
    }
}

// inferred: accessor of mFlag25 (name invented, as in game/Em302Kraken.cpp)
inline unsigned int IsFlag25_unk(const CharaTask* chara) {
    return chara->mFlag25;
}

// NONMATCHING: 99.81% - only .sdata2 offsets: the pool here keeps the constants of the weak copies (ToU32 255,
// fn_8028AC70 -3/3/5, WrapAngle_unk pi/2pi) that the original linker stripped with them
// inferred: a rider leaves the seat: back on its own (m570 2.0), facing `dir`
void Em310Gargoyle::fn_8030C1A4(EnemyTask* rider, const atn::Vector3* dir) {
    rider->mF1C = 0;
    rider->mFlag15 = 0;
    rider->fn_80176ACC();
    rider->fn_800D246C(0);
    if (IsFlag25_unk(rider)) {
        rider->mFlag25 = 0;
    }
    rider->m570 = 2.0f;
    rider->mStatus.fn_8012D8B8();
    rider->m58C = 0;
    rider->fn_8015495C(0, 1, 1, 1);
    rider->mRot = atn::Vector3(0.0f, dir->GetNormalize().ToRotation_unk().y, 0.0f);
    rider->fn_80145334();
    rider->mC58.mC0 = 1;
    rider->mFlag3 = 0;
    rider->vf8C(lbl_807C7030);
}

// NONMATCHING: 99.92% - only .sdata2 offsets: the pool here keeps the constants of the weak copies (ToU32 255,
// fn_8028AC70 -3/3/5, WrapAngle_unk pi/2pi) that the original linker stripped with them
// inferred: state 7 (flying the route): once defeated it picks the nearest seat in front of it and lands;
// otherwise the riders shoot from the seats, are dropped near the place mData.m20 and the route ends at m18
void Em310Gargoyle::fn_8030C490() {
    RouteTable_unk* table = fn_80121F00()->fn_8013C504();
    fn_8016E970(3);
    fn_8030B92C();
    if (m14F1 != 0) {
        float best = 1e16f;
        int index = -1;
        for (int i = 0; i < 4; i++) {
            RouteTable_unk* t = fn_80121F00()->fn_8013C504();
            atn::Vector3 front = mPos + fn_8014052C() * 150.0f;
            atn::Vector3 d = t->FindPlace_unk(mData.mSeats[i].m00)->mPos - front;

            float len = d.Length_unk();
            float angle = fn_802A7910(fn_802A7964(d.ToRotation_unk().y - mRot.y));
            if (angle < 80.0f && angle > -80.0f && len < best) {
                best = len;
                index = i;
            }
        }
        if (index != -1) {
            m14E8 = index;
            fn_8030D0D8();
        }
        return;
    }
    atn::Vector3 place(table->FindPlace_unk(mData.m24)->mPos);
    atn::Vector3 diff = place - mPos;
    if (fn_801C3910(diff) < 20.0f && m15F0 == 0) {
        m14B8 = 1;
        m14E4 = 1;
        m15F0 = 1;
        fn_80370AE4(fn_80121F00(), 0xD0, 1);
    }
    if (m14E0 > 60) {
        m14D4 = 0;
        m14E0 = 0;
    }
    if (m14D0 > 20 && m14E4 != 0 && m14D4 < 4) {
        HitInfo_unk* skill = fn_80219E6C(0x137);
        MtxPtr node = mMotion.GetNodeMtx_unk(m14D4 % 2 == 0 ? "seatpoint_LF" : "seatpoint_RF");
        if (node == 0) {
            node = fn_80266154(&mMotion)->m;
        }
        atn::Matrix3x4 m(node);
        atn::Vector3 pos = m.GetTrans_unk();
        m.fn_8010F668();
        atn::Vector3 dir(0.0f, -1.0f, 1.0f);
        dir.fn_80011410(m);
        fn_800F8574(fn_8013A194(), this, skill, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
        m14D0 = 0;
        m14D4++;
    }
    m14D0++;
    if (m14D4 == 4) {
        m14E0++;
    }
    if (m150C == 0) {
        atn::Vector3 drop(table->FindPlace_unk(mData.m20)->mPos);
        atn::Vector3 dropDiff = drop - mPos;
        if (fn_801C3910(dropDiff) < 20.0f) {
            // inferred: unused rotation (only its construction remains)
            atn::Vector3 rot(mRot.x, mRot.y + 45.0f, mRot.z);
            atn::Matrix3x4 m = fn_8030CA38(mRot);
            int count = -3;
            for (CharaTask* chara = fn_801A0408(fn_801A03E0()); chara != 0; chara = fn_801A03EC(chara)) {
                if (chara->CanAct_unk() && fn_800F530C(chara)->mC0 != 0) {
                    count++;
                }
            }
            if (fn_80370A78(fn_80121F00(), mData.m08) == 1) {
                if (m1500[1] != 0 && m1500[2] != 0) {
                    atn::Matrix3x4 ma(m.m);
                    atn::Matrix3x4 mb(m.m);
                    atn::Vector3 va(0.0f, 0.0f, 1.0f);
                    atn::Vector3 vb(0.0f, 0.0f, 1.0f);
                    ma.fn_80149624(fn_802A7964(45.0f));
                    mb.fn_80149624(fn_802A7964(-45.0f));
                    va.fn_80011410(ma);
                    vb.fn_80011410(mb);
                    if (count < 4) {
                        fn_8030C1A4(m1500[1], &va);
                    }
                    if (count < 3) {
                        fn_8030C1A4(m1500[2], &vb);
                    }
                }
            } else if (fn_80370A78(fn_80121F00(), mData.m08) == 2) {
                if (m1500[0] != 0) {
                    atn::Matrix3x4 ma(m.m);
                    atn::Vector3 v(0.0f, 0.0f, 1.0f);
                    ma.fn_80149624(fn_802A7964(45.0f));
                    v.fn_80011410(m);
                    fn_8030C1A4(m1500[0], &v);
                }
            }
            m150C = 1;
            fn_80370AE4(fn_80121F00(), mData.m10, 1);
            m14E4 = 0;
            m14B8 = 0;
        }
    }
    atn::Vector3 end(fn_8030C190(table, fn_803C17FC(fn_80121F00()->fn_8013C504(), mData.m18))->mPos);
    atn::Vector3 endDiff = end - mPos;
    if (fn_801C3910(endDiff) < 20.0f) {
        fn_8030CF84(0);
        m15F0 = 0;
    }
}

// NONMATCHING: 99.63% - r3/r4 of the source and destination addresses of the point copies swapped (__VecCopy
// family); ~6 variants (z-first copy, helper, pointer to the path, local step)
// inferred: state 8: lands in three legs (start -> control point -> first seat -> second seat); at the end the
// gargoyle stays on the seat (state 6) and the "neck" hit capsule follows
void Em310Gargoyle::fn_8030CBC0() {
    atn::Vector3 points[4];
    switch (m14B0) {
    case 0:
        m1590 += m14C4 / (m15AC - m15A0).Length_unk();
        points[0] = m1594;
        points[1] = m15A0;
        points[2] = m15AC;
        points[3] = m15B8;
        break;
    case 1:
        m1590 += m14C4 / (m15B8 - m15AC).Length_unk();
        points[0] = m15A0;
        points[1] = m15AC;
        points[2] = m15B8;
        points[3] = m15C4;
        break;
    case 2:
        m1590 += m14C4 / (m15C4 - m15B8).Length_unk();
        points[0] = m15AC;
        points[1] = m15B8;
        points[2] = m15C4;
        points[3] = m15C4;
        break;
    }
    fn_8030B74C(points, m1590);
    if (mMotion.mTracks[0].m00 == 0x33) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.fn_80097C08(0, 0x2E, 1, 0, 1, 0.0f, 0.1f);
        }
    }
    if (m1590 >= 1.0f) {
        m14B0++;
        m1590 = 0.0f;
        if (m14B0 == 3) {
            m58C = 6;
            fn_8016E970(3);
            m14F0 = 0;
            m14F1 = 0;
            m14EC = 0;
            m15F0 = 0;
            m14E8 = -1;
            mStatus.fn_8012D8B8();
            fn_80370AE4(lbl_8087F430, mData.m04, 0);
            fn_80239DAC(lbl_8087F3C0, this, 100, 0);
            mColli.mFlags &= ~1;
        }
    }
    atn::Vector3 neck = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    fn_8010A308(lbl_8087F048, &m15E4, &neck, m5B0);
    m15E4 = neck;
}

void Em310Gargoyle::fn_8030CF84(int warp) {
    m58C = 7;
    if (warp != 0) {
        int start = fn_803C17FC(lbl_8087F430->m10D8, mData.m14);
        m158C = fn_803C17FC(lbl_8087F430->m10D8, mData.m18);
        m1588 = start;
        m1590 = 0.0f;
        mPos = fn_8030C190(lbl_8087F430->m10D8, start)->mPos;
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


// NONMATCHING: 99.71% - r3/r4 of the seat id and the place table in the second FindPlace_unk loop, r4/r5 of the
// copy of the neck position (__VecCopy family); ~12 variants. It must stay within the autoinline threshold:
// the original inlines it in the debug branch of vf10 (two `id` statements already move it out).
// inferred: starts the landing (state 8): route point m158C, control point 150 in front, the two seats of m14E8
void Em310Gargoyle::fn_8030D0D8() {
    m58C = 8;
    fn_8016E970(3);
    RouteTable_unk* table = lbl_8087F430->m10D8;
    m1594 = fn_8030C190(table, m158C)->mPos;
    m15A0 = mPos;
    m15AC = mPos + GargoyleFront_unk(mRot.y) * 150.0f;
    m15AC.y += 20.0f;
    int id = mData.mSeats[m14E8].m00;
    m15B8 = table->FindPlace_unk(id)->mPos;
    m15C4 = table->FindPlace_unk(mData.mSeats[m14E8].m04)->mPos;
    m1590 = 0.0f;
    m14B0 = 0;
    m15E4 = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    fn_800EB7A0(this);
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



// inferred: one damage sphere on the collision sphere
void Em310Gargoyle::vf50() {
    Item14_unk item;
    item.m04.mCenter = m614;
    item.m04.mRadius = m620;
    m624.push_back(item);
    mFlagB29 = 0;
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
