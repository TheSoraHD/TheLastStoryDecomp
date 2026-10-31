#include "atn/Color4f_unk.h"
#include "atn/DrawHelper.h"
#include "game/Em206EggMonster.h"
#include "game/EnemyManager.h"
#include "game/TeamAIManager.h"
#include "game/CharaStatus.h"
#include "math/Vec.h"
#include "macros.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"

// inferred: point the camera looks at (lbl_8087F430 + 0x97C, as in game/Em101ElderZesya.cpp); names invented
struct EggLook_unk {
    void Set_unk(const atn::Vector3& pos, bool flag) {
        m01 = m00;
        m00 = 1;
        mPos = pos;
        m24 = 0.0f;
        if (flag) {
            m04 = 1;
        } else {
            m04 = 0;
        }
    }
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[2];
    int m04;
    unsigned char m08[4];
    atn::Vector3 mPos;
    unsigned char m18[0x24 - 0x18];
    float m24;
};
// inferred: camera parameters of the field (lbl_8087F430 + 0x588; as ElderCamParam_unk in game/Em101ElderZesya.cpp)
struct EggCamParam_unk {
    EggCamParam_unk(const EggCamParam_unk& o)
        : m00(o.m00), m04(o.m04), m08(o.m08), m0C(o.m0C), m10(o.m10), m14(o.m14), m18(o.m18), m1C(o.m1C),
          m20(o.m20), m24(o.m24), m28(o.m28), m2C(o.m2C), m30(o.m30), m34(o.m34), m38(o.m38) {}
    EggCamParam_unk& operator=(const EggCamParam_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08 = o.m08;
        m0C = o.m0C;
        m10 = o.m10;
        m14 = o.m14;
        m18 = o.m18;
        m1C = o.m1C;
        m20 = o.m20;
        m24 = o.m24;
        m28 = o.m28;
        m2C = o.m2C;
        m30 = o.m30;
        m34 = o.m34;
        m38 = o.m38;
        return *this;
    }
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    int m38;
};
// inferred: camera mode of the field (lbl_8087F430 + 0x4D8); names invented
struct EggCamera_unk {
    void Set_unk(int mode, const EggCamParam_unk& param) {
        if (mMode != mode) {
            mMode = mode;
            mParam = param;
            m08 = 0.05f;
            m04 = 1.0f;
        }
    }
    int mMode;
    float m04;
    float m08;
    EggCamParam_unk mParam;
};
// inferred: field (lbl_8087F430); only the members used here (names invented)
struct Field_unk {
    unsigned char m0000[0x4D8];
    EggCamera_unk m4D8;
    unsigned char m520[0x588 - 0x520];
    EggCamParam_unk m588;
    unsigned char m5C4[0x8A0 - 0x5C4];
    CharaTask* m8A0;      // inferred: character the camera follows
    unsigned char m8A4[0x97C - 0x8A4];
    EggLook_unk m97C;
    unsigned char m9A4[0x10D8 - 0x9A4];
    RouteTable_unk* m10D8;
    unsigned char m10DC[0x54F4 - 0x10DC];
    unsigned char m54F4[4]; // inferred: name table (fn_803B3B38)
    unsigned char m54F8[0x5694 - 0x54F8];
    int m5694;            // inferred: nonzero in the multiplayer mode
    unsigned char m5698[0x5744 - 0x5698];
    unsigned int m5744;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: game settings (lbl_8087F0A8); class unknown
struct GameSetting_unk {
    unsigned char m000[0x30];
    int mFrameRate; // 0x30 (as in game/Em208Queen.cpp)
    unsigned char m034[0x194 - 0x34];
    int m194;
};
extern "C" GameSetting_unk* lbl_8087F0A8;
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" chr::CharaData_unk* fn_8020A81C(const char* name);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4); // debug menu
// inferred: name table of the field (lbl_8087F430 + 0x54F4) (as in game/CharaTask.cpp)
struct NameEntry_unk {
    unsigned char m00[0x08];
    const wchar_t* mName;
};
extern "C" NameEntry_unk* fn_803B3B38(void* table, int id);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* eff, int a, int b, const atn::Matrix3x4* mtx, int c,
                            int d, int e, int f, int g, int h, float scale); // inferred: effect on a matrix

// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};

extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" double acos(double x);
extern "C" double fmod(double x, double y);
extern "C" double sqrt(double x);
extern "C" double sin(double x);
extern "C" int rand();
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* a, int b, CharaTask* ignore); // inferred: ray test
// inferred: result of a collision query (0x50 bytes, as in game/Em208Queen.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: hit position
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28;
    int m34;
    atn::ColliBase* m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void fn_8004D388(void* mgr, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                            unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" int fn_803C17FC(RouteTable_unk* table, int point); // inferred: route point -> index (1-based)
// inferred: angle wrap to [-180, 180] (name invented)
inline float WrapDeg_unk(float deg) {
    float d = fmod(deg, 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);

// inferred: scale of the effects of the egg (initialized by the __sinit at 0x802C7BC8)
static atn::Vector3 sEffectScale_unk(1.2f, 1.2f, 1.2f);

// inferred: effect `efp` on a model with no offset (name invented, as in game/Em202ForestSpider.cpp)
static inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: same with the manager passed in (name invented)
static inline void PlayEffectM_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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

// inferred: front direction of the character (name invented)
static inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(rot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}

inline void Em206EggMonster::EndAction_unk() {
    m1590 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
}

// inferred: node position (as in game/Em403WarLion.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 0x40 more bytes of stack in the original (a partly written local next to the colors)
Em206EggMonster::Em206EggMonster(atn::Task* parent, void* data)
    : EnemyTask(parent, data), m14D0(0), m14D4(0), m14D8(0), m1524(0), m1584(0), m1588(0), m158C(0), m1590(0),
      m1594(0), m15C8(0), m15CC(-1), m15F0(0), m15F4(0), m15F8(0), m15FC(0), m1700(0), m1710(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    Color4f_unk add;
    add.r = 0.0f;
    add.g = 0.0f;
    add.b = 0.0f;
    add.a = 1.0f;
    Color4f_unk mul;
    mul.r = 1.0f;
    mul.g = 1.0f;
    mul.b = 1.0f;
    mul.a = 1.0f;
    Color4f_unk mul2;
    mul2.r = 1.0f;
    mul2.g = 1.0f;
    mul2.b = 1.0f;
    mul2.a = 1.0f;
    mMotion.fn_80094958("em206_body_l", &add, &mul, &mul2);
    mMotion.fn_80094958("em206_larm_l", &add, &mul, &mul2);
    mMotion.fn_80094958("em206_rarm_l", &add, &mul, &mul2);
    mMotion.fn_80094958("em206_tail_l", &add, &mul, &mul2);
    m156C.LoadRequest("/effect/ef_bs046b_01.eff");
    m1578.LoadRequest("/effect/ef_sk023a.efp");
    m16A4.LoadRequest("/effect/ef_uc015.efp");
    m16B0.LoadRequest("/effect/ef_sk016a.efp");
    m16BC = 0;
    m16C0 = -1.0f;
    m16C4.SetIdentity();
    m16F4.LoadRequest("/effect/ef_ui010b.eff");
    m14BC = 1772.0f;
    m14C0 = 1835.0f;
    m15D0[0] = 1330.76f;
    m15D0[1] = 1448.36f;
    m15D0[2] = 1358.57f;
    m15D0[3] = 1415.52f;
    m15D0[4] = 1227.72f;
    m15D0[5] = 1343.85f;
    m15D0[6] = 1252.02f;
    m15D0[7] = 1314.57f;
    m15B0 = 0;
    m15B4 = 1;
    m15B8 = 0;
    m15BC = 0;
    m15C0 = 0;
    m15C4 = 0;
    fn_80370AE4(lbl_8087F430, 0xFF, 0);
    m568 = 0.15f;
    m1704 = m5C;
    m1708 = fn_8020A81C("em_dg004Boss_B");
    m14A8 |= 0x80000000;
}

// NONMATCHING: 99% - stack slots of the node positions
int Em206EggMonster::vf0C() {
    if (fn_8013655C() == 0 && m156C.IsLoading() == 0 && m1578.IsLoading() == 0 && m16B0.IsLoading() == 0 &&
        m16A4.IsLoading() == 0 && m16F4.IsLoading() == 0) {
        fn_80087994(m170C.Load("em305"), "DebugDisp", &m1710, 0, 0);
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC218;
        mStatus.m18 |= 0x3800000;
        mStatus.m18 |= 0x401;
        mC10 &= ~0x2000;
        m1600.mFlags |= 3;
        m1600.m0C = (int)this;
        m164C.mFlags |= 3;
        m164C.m0C = (int)this;
        m164C.mFlags &= ~0x80000000;
        m1600.mSphere.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
        m1600.mSphere.mRadius = 20.0f;
        atn::Vector3 a = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail1"));
        atn::Vector3 b = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail3"));
        m164C.m3C = a;
        m164C.m48 = b;
        m164C.m54 = 15.0f;
        m148C = GetNodePos_unk(mMotion.GetNodeMtx_unk("ef_eye"));
        m1498 = 0.01f;
        mRoute.mA0 = 1;
        m56C = 0.03f;
        return 1;
    }
    return 0;
}

// inferred: waves of eggs: team id and the three route points/characters (table lbl_80746C38; names invented)
struct EggWave_unk {
    int mTeam;
    int mIds[3];
};
static const EggWave_unk sWaves_unk[3] = {
    {101, {101, 102, 103}},
    {102, {104, 105, 106}},
    {103, {107, 108, 109}},
};
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10, float arg11);

// inferred: scale matrix and matrix product by value (names invented, as in gmk/Leaf.cpp)
inline atn::Matrix3x4 ScaleMtx_unk(const atn::Vector3& s) {
    atn::Matrix3x4 r;
    PSMTXScale(r.m, s.x, s.y, s.z);
    return r;
}
inline atn::Matrix3x4 MtxMul_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

// NONMATCHING: 98.1% - stack slots of the throw vectors (target, hand, rotation) and of the node
// positions (as in vf0C); registers of the wave loop
void Em206EggMonster::vf10() {
    m500 = 0.03f;
    if (m15F0 == 0) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                CharaTask* c = EnemyManager::sInstance->Find_unk(sWaves_unk[i].mIds[j]);
                if (c != 0) {
                    c->fn_801765D8();
                }
            }
        }
        m15F0++;
    }
    if (mC58.mC0 != 0) {
        fn_802C5F5C();
    }
    int turn = 0;
    m1588 = m1584;
    m1584 = mC58.mC4;
    m1590++;
    m15BC--;
    if (mC58.mC0 == 0 || m1584 == 0) {
        fn_8016E970(3);
    } else if (m1584 != 0) {
        switch (m58C) {
        case 1:
            fn_802C3EDC();
            turn = 1;
            break;
        case 2:
            fn_802C42D4();
            turn = 1;
            break;
        case 3: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                m1590 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 0;
                fn_8016E970(3);
                fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
            } else if ((frame = mMotion.mTracks[0].m08, 90.0f <= frame) && frame <= 95.0f) {
                m14C4 = m1528->mColli.m3C;
            } else if (mMotion.mTracks[0].m08 >= 102.1f && mMotion.mTracks[0].m08 <= 103.0f) {
                m14D0 = 1;
                atn::Vector3 target = m14C4;
                CharaTask* t = m1528;
                fn_80239DAC(lbl_8087F3C0, this, 0x65, 1);
                m14D4 = 0;
                m14D8 = 150;
                atn::Vector3 hand = GetNodePos_unk(mMotion.GetNodeMtx_unk("righthand"));
                m14DC = target - hand;
                m14DC *= 0.15f;
                m1524 = t;
                m14F4.SetIdentity();
                atn::Vector3 rot = m14DC.ToRotation_unk();
                m14F4.RotateZYX_unk(rot);
                atn::Matrix3x4* mtx = &m14F4;
                *mtx = MtxMul_unk(*mtx, ScaleMtx_unk(sEffectScale_unk));
                m14F4.fn_801479E4(hand);
                fn_80232B7C(this, 0x65);
                EffectMgr_unk* mgr = lbl_8087F3C0;
                fn_8023A680(mgr, &m156C, -1, 5, mtx, 0, 0, 0, 0, -1, 1, 1.0f);
            }
            break;
        }
        case 4:
            fn_802C4480();
            turn = 1;
            break;
        case 5: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                m1590 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 0;
                fn_8016E970(3);
                fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
            } else if (mMotion.mTracks[0].m08 >= 55.1f && mMotion.mTracks[0].m08 <= 56.0f) {
                fn_802C653C();
                if (fn_80370A78(lbl_8087F430, 0xF8) == 0) {
                    fn_80370AE4(lbl_8087F430, 0xF8, 1);
                }
                Effect_unk* mgr = lbl_8087F048;
                HitInfo_unk* data = fn_80219E6C(0x57C);
                fn_800FAB80(mgr, this, data, m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            }
            if (mMotion.mTracks[0].m08 >= 50.1f && mMotion.mTracks[0].m08 <= 51.0f && mMotion.mTracks[0].m00 == 0x145) {
                turn = 1;
            }
            break;
        }
        case 6: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                m1590 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 0;
                fn_8016E970(3);
                fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
            }
            break;
        }
        case 7: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                m1590 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 0;
                fn_8016E970(3);
                fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
            }
            break;
        }
        case 8: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                m1590 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 0;
                fn_8016E970(3);
                fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
            }
            break;
        }
        case 9:
            vfEC();
            break;
        case 10:
            fn_802C3EDC();
            turn = 1;
            break;
        case 11: {
            atn::Vector3 d;
            d = m1584->mPos - mPos;
            PSVECNormalize(&d, &d);
            if (PSVECMag(&d) < 70.0f || (CharaTask*)lbl_8087F8A0->mHead == m1584) {
                m58C = 0;
                if (m55C == 6 && m560 == 4) {
                    fn_8016E970(3);
                }
            }
            break;
        }
        default:
            if (m55C != 6) {
                if (m55C == 7) {
                    if (m1594 != 2 && m1584 != (CharaTask*)mC58.mC8) {
                        fn_8016E970(3);
                        fn_80170A20(m1584, 0, -1.0f);
                    }
                } else {
                    fn_8016E970(3);
                    fn_80170A20(m1584, 0, -1.0f);
                }
            }
            fn_802C2BCC();
            fn_802C3154();
            break;
        }
    }
    if (((CharaTask*)lbl_8087F8A0->mHead)->mFlag28) {
        if (fn_80370A78(lbl_8087F430, 0xFD) == 0) {
            fn_80370AE4(lbl_8087F430, 0xFD, 1);
        }
    } else if (fn_80370A78(lbl_8087F430, 0xFD) != 0) {
        fn_80370AE4(lbl_8087F430, 0xFD, 0);
    }
    if (turn != 0) {
        float radius = 20.0f;
        float angle = 3.1415927f;
        if (m58C == 10) {
            radius *= 1.5f;
        }
        if (m58C == 5) {
            radius = 120.0f;
            angle = 4.712389f;
        }
        fn_802C4E80(100, radius, angle);
    }
    fn_8014C540();
    fn_80145334();
    fn_802C1FEC();
    m1600.mSphere.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    m1600.mSphere.mRadius = 20.0f;
    atn::Vector3 a = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail1"));
    atn::Vector3 b = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail3"));
    m164C.m3C = a;
    m164C.m48 = b;
    m164C.m54 = 15.0f;
    m148C = GetNodePos_unk(mMotion.GetNodeMtx_unk("ef_eye"));
    m1498 = 0.01f;
    fn_802C6DF4();
    if (fn_802C69BC() != 0 && m1594 != 2) {
        m16BC = 0;
        fn_80239DAC(lbl_8087F3C0, this, 200, 0);
        m1594 = 2;
        fn_80370AE4(lbl_8087F430, 0xFA, 1);
        m58C = 0;
    }
}

// NONMATCHING: 99.92% - f2/f3 of the sine and the MIN result in the m1700 branch (the other two match)
// inferred: pulses the colour of the body (head chant, hit or idle) and fades it with the motion
// of the attacks (states 2 and 4)
void Em206EggMonster::fn_802C1FEC() {
    float body;
    float rarm;
    float larm;
    if (m1700 != 0) {
        body = 0.2f * MIN(1.0f, 0.2f * (float)sin(0.017453292f * (4.0f * m15FC)) + 0.8f) + 0.8f;
        rarm = body;
        larm = body;
    } else {
        if (m15F4 != 0) {
            body = 0.6f * MIN(1.0f, 0.5f * (float)sin(0.017453292f * (16.0f * m15FC)) + 0.5f) + 0.2f;
            rarm = body;
            larm = body;
        } else {
            body = 0.6f * MIN(1.0f, 0.5f * (float)sin(0.017453292f * (4.0f * m15FC)) + 0.5f) + 0.2f;
            rarm = body;
            larm = body;
        }
        switch (m58C) {
        case 2: {
            float frame = mMotion.mTracks[0].m08;
            if (frame < 20.0f) {
                float k = 1.0f - frame / 20.0f;
                body *= k;
                rarm *= k;
                larm *= k;
            } else if (frame < 76.0f) {
                if (frame < 71.0f) {
                    rarm = MIN(1.0f, MAX(0.0f, (mMotion.mTracks[0].m08 - 30.0f) / 20.0f));
                } else {
                    rarm = MIN(1.0f, MAX(0.0f, 1.0f - (mMotion.mTracks[0].m08 - 71.0f) / 5.0f));
                }
                body = rarm;
                larm = 0.0f;
                m15FC = m15F4 != 0 ? 16 : 67;
            } else {
                float k = MIN(1.0f, (frame - 76.0f) / 10.0f);
                body *= k;
                rarm *= k;
                larm *= k;
                m15FC = m15F4 != 0 ? 16 : 67;
            }
            break;
        }
        case 4: {
            float frame = mMotion.mTracks[0].m08;
            if (frame < 20.0f) {
                float k = 1.0f - frame / 20.0f;
                body *= k;
                rarm *= k;
                larm *= k;
            } else if (frame < 76.0f) {
                if (frame < 70.0f) {
                    rarm = MIN(1.0f, MAX(0.0f, (mMotion.mTracks[0].m08 - 20.0f) / 20.0f));
                } else {
                    rarm = MIN(1.0f, MAX(0.0f, 1.0f - (mMotion.mTracks[0].m08 - 70.0f) / 5.0f));
                }
                body = rarm;
                larm = rarm;
                m15FC = m15F4 != 0 ? 16 : 67;
            } else {
                float k = MIN(1.0f, (frame - 76.0f) / 10.0f);
                body *= k;
                rarm *= k;
                larm *= k;
                m15FC = m15F4 != 0 ? 16 : 67;
            }
            break;
        }
        }
    }
    atn::Color4f_unk c0(body, body, body, 1.0f);
    atn::Color4f_unk c1(rarm, rarm, rarm, 1.0f);
    atn::Color4f_unk c2(larm, larm, larm, 1.0f);
    atn::Color4f_unk one(1.0f, 1.0f, 1.0f, 1.0f);
    mMotion.fn_80094958("em206_body_l", &c0, &one, &one);
    mMotion.fn_80094958("em206_tail_l", &c0, &one, &one);
    mMotion.fn_80094958("em206_rarm_l", &c1, &one, &one);
    mMotion.fn_80094958("em206_larm_l", &c2, &one, &one);
    m15FC++;
}

void Em206EggMonster::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->mAttacker == this) {
        msg->m90 = 0;
        msg->m84 = 2;
        msg->m68 = 0;
        msg->m8C = 1;
        return;
    }
    msg->m10 *= 0.0f;
    if (msg->mData->m01 == 1 && *(int*)&msg->m3C[4] == 1 && msg->mData->m04 != 0x4E88) {
        fn_8017A300(m1708);
        if (fn_80370A78(lbl_8087F430, 0xFC) == 0 && m15F4 == 0) {
            msg->m0C |= 0x200000;
            m15F8++;
            float hp = mStatus.m04;
            if ((hp / mStatus.mParamB.m84 < 0.85f || m15F8 >= 3) && lbl_8087F430->m5694 == 0) {
                m1590 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 7;
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 0.5f;
                mMotion.fn_80097C08(0, 0x37, 0, 0, 1, 0.0f, 0.025f);
                m15B0 = 1;
                m15B4 = 1;
                m15F4 = 1;
                if (m15B0 != 0) {
                    fn_80370AE4(lbl_8087F430, 0xFC, 1);
                } else {
                    fn_80370AE4(lbl_8087F430, 0xFC, 3);
                }
                m1594 = 1;
                m16BC = 1;
                fn_80232B7C(this, 200);
                EffectMgr_unk* mgr = lbl_8087F3C0;
                fn_8023A680(mgr, &m16F4, -1, 5, &m16C4, 0, 0, 0, 0, -1, 1, 1.0f);
            } else {
                m1590 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 6;
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x35, 0, 0, 1, 0.0f, 0.025f);
                m15B4 = 1;
            }
            if (m15F8 <= 3) {
                m15B8 = 1;
            } else {
                m15B8 = 0;
            }
            m1700 = 0;
            m15C4 = 0;
        }
    }
    if (*(int*)&msg->m3C[4] == 1) {
        msg->m0C = 0x80000000;
        msg->m94 = 0x80000000;
    }
    int part = *(int*)&msg->m3C[4];
    msg->m1C = m624.mData[part].m04.mCenter;
    msg->m1C += msg->m28 * m624.mData[part].m04.mRadius;
    msg->m1C.y += m624.mData[part].m04.mRadius;
    fn_80151448(msg);
    vf40(hit);
    fn_8017A300(m1704);
}

void Em206EggMonster::vf40(HitInfo_unk* hit) {
    if (mStatus.IsState_unk(0x20)) {
        fn_802C4B70();
        return;
    }
    if (fn_80370A78(lbl_8087F430, 0xFC) == 0) {
        float hp = mStatus.m04;
        float ratio = hp / mStatus.mParamB.m84;
        if (ratio < 0.5f && lbl_8087F430->m5694 == 0) {
            m1590 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 8;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x33, 0, 0, 1, 0.0f, 0.025f);
            if (m15B0 != 0) {
                fn_80370AE4(lbl_8087F430, 0xFC, 1);
            } else {
                fn_80370AE4(lbl_8087F430, 0xFC, 3);
            }
            m1594 = 1;
            m16BC = 1;
            fn_80232B7C(this, 200);
            fn_8023A680(lbl_8087F3C0, &m16F4, -1, 5, &m16C4, 0, 0, 0, 0, -1, 1, 1.0f);
            m15B8 = 1;
        } else if (m15C8 == 0 && ratio < 0.7f) {
            m15B8 = 1;
        }
    }
}

int Em206EggMonster::vfB8(wchar_t* name) {
    if (lbl_8087F430->m5694 == 0) {
        if (m5C != 0 && m5C->mC4 > 0 && lbl_8087F430 != 0) {
            NameEntry_unk* entry = fn_803B3B38(lbl_8087F430->m54F4, m5C->mC4);
            if (entry != 0) {
                wcscpy(name, entry->mName);
                return 1;
            }
        }
    }
    return 0;
}

void Em206EggMonster::vf24() {
    if (m55C != 6) {
        if (m15B0 != 0) {
            mMotion.fn_80097C08(0, 0x14A, 1, 0, 1, 0.0f, 0.025f);
        } else {
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.025f);
        }
    }
}

// inferred: 2D helpers of the arena limits (names invented)
inline atn::Vector2_unk Sub2_unk(const atn::Vector2_unk& a, const atn::Vector2_unk& b) {
    return atn::Vector2_unk(a.x - b.x, a.y - b.y);
}
inline float Cross2_unk(const atn::Vector2_unk& a, const atn::Vector2_unk& b) {
    return a.x * b.y - a.y * b.x;
}

// inferred: movement (walks the route in state 7, turned back towards the target when it is too far
// from its home point 0x14BC and keeps walking away)
void Em206EggMonster::fn_802C2BCC() {
    float speed = 0.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 vel = mRoute.m58;
        speed = vel.Length_unk();
        if (speed > 0.0001f) {
            rot = vel.GetNormalize().ToRotation_unk();
        }
        if (m1594 == 0) {
            atn::Vector2_unk pos(mPos.x, mPos.z);
            atn::Vector2_unk d = Sub2_unk(pos, *(const atn::Vector2_unk*)&m14BC);
            atn::Vector2_unk v(vel.x, vel.z);
            if ((float)sqrt(d.x * d.x + d.y * d.y) > 90.0f && d.x * v.x + d.y * v.y > 0.0f) {
                speed = 0.0f;
                fn_80170A20(m1584, 0, -1.0f);
            }
            if (speed < 0.0001f && m1584 != 0) {
                atn::Vector3 dir = m1584->mPos - mPos;
                dir.y = 0.0f;
                dir.Normalize_unk();
                speed = 0.1f;
                rot = dir.ToRotation_unk();
            }
        }
    } else if (m55C == 6) {
        if (m560 == 4) {
            m580 = 0.0f;
            m584 = 0.0f;
        } else {
            fn_8013A258();
            return;
        }
    }
    fn_8013CB68(rot, 1, speed, m568);
}

// inferred: starts state 4 (turns to the target; name invented)
inline void Em206EggMonster::StartTurn_unk() {
    m1590 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 4;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.5f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.025f);
    CharaTask* t = m1584;
    atn::Vector3 d = t->mPos - mPos;
    atn::Vector3 r = d.ToRotationZ_unk();
    mRot.y = r.y;
    m15B4 = 0;
    m15C4++;
}

// inferred: starts state 10 (runs to the point m14B0; name invented)
inline void Em206EggMonster::StartRun_unk() {
    m1590 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 10;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 1, 0, 1, 0.0f, 0.025f);
    m158C = -1;
    atn::Vector3 tp = m14B0;
    atn::Vector3 d = tp - mPos;
    m15A4 = PSVECMag(&d);
    atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
    m15A8 = rot.y - mRot.y;
    m15A8 = WrapDeg_unk(57.295776f * m15A8);
    m15B4 = 1;
    m1590 = 0;
}

// inferred: chooses the next action with fn_802C38E4 and starts it
void Em206EggMonster::fn_802C3154() {
    if (m1584 == 0) {
        return;
    }
    switch (fn_802C38E4()) {
    case 1:
        m1590 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 1;
        fn_8016E970(3);
        m158C = -1;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        if (m1594 == 2) {
            mMotion.fn_80097C08(0, 0x14A, 1, 0, 1, 0.0f, 0.025f);
        } else {
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.025f);
        }
        m15B4 = 1;
        break;
    case 2:
        m1590 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 2;
        fn_8016E970(6);
        m560 = 4;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.025f);
        m15B4 = 0;
        m15C4++;
        break;
    case 3:
        fn_802C4858();
        m15C4++;
        break;
    case 4:
        StartTurn_unk();
        break;
    case 10:
        StartRun_unk();
        break;
    case 5:
        m1590 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 5;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.025f);
        m15B4 = 1;
        m15BC = 30;
        m15C0++;
        m15B8 = 0;
        break;
    }
    if (m15C4 >= 10) {
        m1700 = 1;
    }
}

// NONMATCHING: 98.8% - stack slots of the locals (rotation, ray ends) and the frsp before atan2
int Em206EggMonster::fn_802C38E4() {
    int next = -1;
    if (m15B8 != 0 && m15BC < 0 && m1594 != 2 && m15C0 < 2) {
        next = 5;
    } else {
        CharaTask* target = m1584;
        if (target != 0) {
            atn::Vector3 tp = target->mPos;
            atn::Vector3 d = tp - mPos;
            float dist = PSVECMag(&d);
            atn::Vector3 n = d;
            PSVECNormalize(&n, &n);
            atn::Vector3 rot = n.ToRotation_unk();
            WrapDeg_unk(57.295776f * (rot.y - mRot.y));
            atn::Vector3 from = mPos;
            atn::Vector3 to = m1584->mPos;
            from.y += 20.0f;
            to.y += 20.0f;
            int clear = fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0, m1584) == 0;
            switch (m1594) {
            case 0:
                if (dist < 60.0f && m15B4 != 0) {
                    if (m1700 != 0) {
                        next = 4;
                        if (rand() % 2 == 0) {
                            next = 2;
                        }
                    } else {
                        next = 2;
                    }
                } else if (m1700 != 0 && dist < 60.0f && m15B4 != 0) {
                    next = 4;
                } else if (m15B4 != 0) {
                    if (clear) {
                        next = 3;
                    } else {
                        next = 2;
                    }
                } else {
                    next = 1;
                }
                break;
            case 1:
                if (dist < 60.0f && m15B4 != 0) {
                    next = 4;
                } else if (dist < 120.0f && clear && m15B4 != 0) {
                    next = 3;
                } else {
                    next = 10;
                }
                break;
            case 2:
                next = 10;
                break;
            }
        }
    }
    if (next == 4) {
        RouteTable_unk* routes = lbl_8087F430->m10D8;
        int index = fn_803C17FC(routes, 0x13);
        if (index != 0) {
            atn::Vector3 d = routes->mPoints[index - 1].mPos - mPos;
            if (PSVECMag(&d) < 150.0f) {
                next = 3;
            }
        }
    }
    return next;
}

atn::Vector3 Em206EggMonster::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (fn_80370A78(lbl_8087F430, 0xF5) == 0) {
        result = mPos;
    } else {
        result = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    }
    return result;
}

// NONMATCHING: 99.98% - stack slots of the GetNormalize temporary and of the normalized copy swapped
void Em206EggMonster::fn_802C3EDC() {
    if (m55C == 7) {
        if (m1594 != 2 && mC58.mC4 != (CharaTask*)mC58.mC8) {
            fn_8016E970(3);
            fn_80170A20(m1584, 0, -1.0f);
        }
    } else if (m1594 == 2) {
        fn_8016E970(3);
        mRoute.mA0 = 0;
        fn_8016F824(0xAC, 0);
    } else {
        fn_8016E970(3);
        fn_80170A20(m1584, 0, -1.0f);
    }
    if (m1594 == 1) {
        m568 = 0.85f;
        mMotion.mTracks[0].m0C = 3.0f;
    } else {
        m568 = 0.15f;
        mMotion.mTracks[0].m0C = 1.0f;
    }
    fn_802C2BCC();
    if (m1594 != 2) {
        CharaTask* t = m1584;
        atn::Vector3 d = t->mPos - mPos;
        atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
        atn::WrapAngle_unk(rot.y - mRot.y);
        atn::Vector3 m = d;
        PSVECNormalize(&m, &m);
        PSVECMag(&m);
        int limit;
        if (m1594 == 1) {
            limit = 300;
        } else {
            limit = 15;
            int t = 150 - m15C4 * 10;
            if (t >= 15) {
                limit = t;
            }
        }
        if (m1590 > limit && m58C != fn_802C38E4()) {
            m58C = 0;
            fn_8016E970(3);
        }
    }
}

void Em206EggMonster::fn_802C42D4() {
    if (fn_80158BB4() != 0) {
        EndAction_unk();
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= 57.0f && frame <= 63.0f) {
        HitInfo_unk* data;
        if (m1700 != 0) {
            data = fn_80219E6C(0x582);
        } else {
            data = fn_80219E6C(0x578);
        }
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
    } else if (mMotion.mTracks[0].m08 >= 71.0f && mMotion.mTracks[0].m08 <= 76.0f) {
        mMotion.mTracks[0].m0C = 0.3f;
    } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 1.0f) < 1.1920929e-7f) {
        PlayEffect_unk(&m16B0, &mMotion);
    } else {
        mMotion.mTracks[0].m0C = 1.0f;
    }
}

// NONMATCHING: 99.3% - float registers of the jump vector and r3/r4 of a Vector3 copy
// inferred: state 4 (jumps forward and lands on the ground)
void Em206EggMonster::fn_802C4480() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        EndAction_unk();
    } else if (mMotion.mTracks[0].m08 >= 28.0f && mMotion.mTracks[0].m08 <= 60.0f) {
        int fps = lbl_8087F0A8->mFrameRate;
        atn::Vector3 move(0.0f, -98.1f / (float)(fps * fps), 1.5f);
        atn::Matrix3x4 m;
        m.fn_8013A13C(mRot.y);
        PSMTXMultVec(m.m, &move, &move);
        CollResult_unk result;
        CharaTask::Sphere_unk sphere = fn_80176548(mPos);
        fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000000, &mColli, 0, sphere.mRadius);
        atn::Vector3 pos = result.m10;
        pos -= m5A4;
        pos.y -= sphere.mRadius;
        mPos = pos;
    }
    float f = mMotion.mTracks[0].m08;
    if (f >= 36.0f && f <= 60.0f) {
        HitInfo_unk* data;
        if (m1700 != 0) {
            data = fn_80219E6C(0x584);
        } else {
            data = fn_80219E6C(0x57A);
        }
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
    }
}

// NONMATCHING: 99.6% - r3/r4 of the Vector3 copy to the field (docs/mwcc-blockers.md, 3)
void Em206EggMonster::vfEC() {
    Field_unk* field = lbl_8087F430;
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        int done = 1;
        if (lbl_8087F0A8->m194 != 0) {
            if (lbl_8087F430->m5744 < 0x5460) {
                lbl_8087F430->m5744 = 0x5460;
            }
            float f = mMotion.mTracks[0].m08;
            done = f >= 2.0f * mMotion.fn_80097D7C(0);
        }
        if (done) {
            mFlag25 = 1;
            mColli.mFlags &= ~1;
            m1600.mFlags &= ~1;
            m164C.mFlags &= ~1;
            mFlag15 = 1;
            fn_801765D8();
            fn_80370AE4(lbl_8087F430, 0xF9, 1);
            field->m8A0 = 0;
            field->m4D8.mMode = 0;
            field->m97C.m00 = 0;
        }
    } else {
        atn::Vector3 p = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
        if (p.y < 5.0f + mPos.y) {
            p.y = 5.0f + mPos.y;
        }
        field->m8A0 = this;
        field->m97C.mPos = p;
    }
}

void Em206EggMonster::fn_802C4858() {
    m1590 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 3;
    fn_8016E970(6);
    m560 = 4;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.025f);
    fn_80232B7C(this, 1000);
    PlayEffectM_unk(lbl_8087F3C0, &m1578, &mMotion);
    m1528 = m1584;
    atn::Vector3 d = m1528->mPos - mPos;
    atn::Vector3 r = d.ToRotationZ_unk();
    mRot.y = r.y;
    m15B4 = 0;
}

// NONMATCHING: 97% - the original addresses the camera mode (0x4D8) and the look point (0x97C) through base
// registers (same open case as Em101ElderZesya::fn_8025922C)
// inferred: death (the camera follows the egg with the parameters of mode 4)
void Em206EggMonster::fn_802C4B70() {
    m1590 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 9;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.025f);
    m15B4 = 1;
    m16BC = 0;
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    fn_80232B7C(0, 0);
    PlayEffectM_unk(lbl_8087F3C0, &m16A4, &mMotion);
    Field_unk* field = lbl_8087F430;
    atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    EggCamera_unk* cam = &field->m4D8;
    EggCamParam_unk param = field->m588;
    param.m08 = 0.0f;
    param.m04 = 0.61086524f;
    param.m00 = 15.0f;
    param.m10 = 12.0f;
    field->m8A0 = this;
    cam->Set_unk(4, param);
    field->m97C.Set_unk(pos, false);
}

// inferred: sends `id` to the egg gimmicks (0x40F, 0x410, 0x412) in a cone in front (as fn_805A5078)
void Em206EggMonster::fn_802C4E80(int id, float radius, float angle) {
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        int kind = g->mId;
        if (kind == 0x40F || kind == 0x410 || kind == 0x412) {
            atn::Vector3 p = g->vf54();
            atn::Vector3 d(p.x - mPos.x, 0.0f, p.z - mPos.z);
            float dist = PSVECMag(&d);
            atn::ColliSphere* size = (atn::ColliSphere*)g->vf50();
            if (size != 0) {
                dist -= size->mSphere.mRadius;
            }
            if (dist <= radius) {
                PSVECNormalize(&d, &d);
                atn::Vector3 v = FrontDir_unk(mRot);
                v.y = 0.0f;
                if ((float)acos(PSVECDotProduct(&d, &v)) <= 0.5f * angle) {
                    GimmickHitMsg_unk msg;
                    memset(&msg, 0, sizeof(msg));
                    msg.mType = id;
                    g->vf44(&msg);
                }
            }
        }
    }
}

// NONMATCHING: 99.97% - only the pool offsets ("righthand" of the unwritten vf10 comes before "head")
// inferred: three damage spheres: the body and the "head" and "spine1" nodes (as Em309Ghoul::vf50)
void Em206EggMonster::vf50() {
    Item14_unk item;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += item.m04.mRadius;
    item.m04.mRadius = m5B0;
    m624.push_back(item);
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head")) + m5A4;
    item.m04.mCenter.y += 3.0f;
    item.m04.mRadius = m5B0;
    item.m00 = 4; item.m00 |= 8;
    m624.push_back(item);
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1")) + m5A4;
    item.m04.mRadius = m5B0;
    item.m00 = 4; item.m00 |= 0xA;
    m624.push_back(item);
    mFlagB29 = 0;
}

// NONMATCHING: 96% - registers of the Vector3 copies (docs/mwcc-blockers.md, 3); pool offsets wait for vf50
void Em206EggMonster::vf54() {
    if (m624.mSize != 0) {
        m624.mData[0].m04.mCenter = mPos + m5A4;
        m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
        m624.mData[1].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head")) + m5A4;
        m624.mData[1].m04.mCenter.y -= 3.0f;
        atn::Vector3 ofs(0.0f, 0.0f, 7.0f);
        atn::Matrix3x4 m;
        m.fn_8013A13C(mRot.y);
        PSMTXMultVec(m.m, &ofs, &ofs);
        m624.mData[2].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1")) + m5A4;
        m624.mData[2].m04.mCenter += ofs;
    }
}

int Em206EggMonster::vfF4() {
    if (lbl_8087F430 != 0 && fn_80370A78(lbl_8087F430, 0xFF) == 0 && m15B0 == 0) {
        return 1;
    }
    return 0;
}

// NONMATCHING: 97.9% - registers of the inlined RouteTable_unk::FindPlace_unk
// inferred: sends the next wave: the three characters of the table go to their route points (80 units up)
// and the team AI is told to attack
void Em206EggMonster::fn_802C653C() {
    if (m15C8 < 3) {
        atn::Vector3 pos;
        atn::Vector3 up(0.0f, 80.0f, 0.0f);
        RoutePlace_unk* p = lbl_8087F430->m10D8->FindPlace_unk(sWaves_unk[m15C8].mIds[0]);
        if (p != 0) {
            pos = p->mPos + up;
            fn_802C6878(EnemyManager::sInstance->Find_unk(sWaves_unk[m15C8].mIds[0]), pos);
        }
        p = lbl_8087F430->m10D8->FindPlace_unk(sWaves_unk[m15C8].mIds[1]);
        if (p != 0) {
            pos = p->mPos + up;
            fn_802C6878(EnemyManager::sInstance->Find_unk(sWaves_unk[m15C8].mIds[1]), pos);
        }
        p = lbl_8087F430->m10D8->FindPlace_unk(sWaves_unk[m15C8].mIds[2]);
        if (p != 0) {
            pos = p->mPos + up;
            fn_802C6878(EnemyManager::sInstance->Find_unk(sWaves_unk[m15C8].mIds[2]), pos);
        }
        TeamMsgIn_unk msg;
        msg.mType = 2;
        msg.mId = sWaves_unk[m15C8].mTeam;
        msg.m10 = 4;
        msg.m18 = 1;
        msg.m14 = 0;
        lbl_8087FA00->OnMsg24_unk(&msg);
        m15C8++;
    }
}

// NONMATCHING: 95% - the original reloads the flag word before clearing mFlag25, and the stack slots of the zero vectors
// inferred: brings `chara` back to life at `pos`
void Em206EggMonster::fn_802C6878(CharaTask* chara, const atn::Vector3& pos) {
    if (chara != 0 && (chara->mStatus.IsState_unk(0x20) || chara->mC58.mC0 == 0)) {
        chara->mFlag15 = 0;
        chara->mFlags &= ~4;
        chara->fn_80176ACC();
        chara->fn_800D246C(0);
        if (chara->mFlag25) {
            chara->mFlag25 = 0;
        }
        chara->mStatus.fn_8012D8B8();
        chara->m58C = 0;
        chara->fn_8015495C(0, 1, 1, 1);
        chara->mPos = pos;
        chara->mRot = atn::Vector3(0.0f, 0.0f, 0.0f);
        chara->m574 = atn::Vector3(0.0f, -0.1f, 0.0f);
        chara->fn_80145334();
        chara->mC58.mC0 = 1;
        chara->vf8C(lbl_807C7030);
    }
}


// NONMATCHING: 99.43% - r28/r30 of the channels in the inlined Color4f_unk::ToU32
// inferred: draws the two limits of the arena (debug) and tells whether the egg is between them: mPos
// and the point (m14BC, m14C0) on different sides of the first line and on the same side of the second
__declspec(noinline) int Em206EggMonster::fn_802C69BC() {
    if (m1710 != 0) {
        atn::Vector3 pts[4] = {
            atn::Vector3(m15D0[0], -90.0f, m15D0[1]),
            atn::Vector3(m15D0[2], -90.0f, m15D0[3]),
            atn::Vector3(m15D0[4], -90.0f, m15D0[5]),
            atn::Vector3(m15D0[6], -90.0f, m15D0[7]),
        };
        lbl_8087EEB0->fn_80063764(pts[0], pts[1], 1000.0f, atn::Color4f_unk(255.0f, 255.0f, 0.0f, 255.0f).ToU32());
        lbl_8087EEB0->fn_80063764(pts[2], pts[3], 1000.0f, atn::Color4f_unk(255.0f, 0.0f, 255.0f, 255.0f).ToU32());
    }
    atn::Vector2_unk p(mPos.x, mPos.z);
    const atn::Vector2_unk* l = (const atn::Vector2_unk*)m15D0;
    const atn::Vector2_unk& q = *(const atn::Vector2_unk*)&m14BC;
    if (Cross2_unk(Sub2_unk(l[1], l[0]), Sub2_unk(p, l[0])) * Cross2_unk(Sub2_unk(l[1], l[0]), Sub2_unk(q, l[0])) <
            0.0f &&
        Cross2_unk(Sub2_unk(l[3], l[2]), Sub2_unk(p, l[2])) * Cross2_unk(Sub2_unk(l[3], l[2]), Sub2_unk(q, l[2])) >
            0.0f) {
        return 1;
    }
    return 0;
}

int Em206EggMonster::vfE4() {
    if (m58C == 3) {
        float frame = mMotion.mTracks[0].m08;
        if (80.0f <= frame && frame <= 105.0f) {
            return 1;
        }
    }
    return 0;
}

float Em206EggMonster::vf90() {
    return 4.0f;
}

Em206EggMonster::~Em206EggMonster() {}
