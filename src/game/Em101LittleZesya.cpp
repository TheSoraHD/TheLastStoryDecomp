#include "game/Em101LittleZesya.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: debug menu entries (as in game/Em101ElderZesya.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects

// inferred: plays an effect on the model (locals in the order of game/Em003LesserDaemon.cpp)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect

// NONMATCHING: 99.74% - r26/r27 of the second std::string (address and literal swapped) and r25/r26 of the
// strlen of the parameters: same as Em101ElderZesya and Em201ZanGurg (3 variants: literal in a local,
// declaration order, one declaration)
Em101LittleZesya::Em101LittleZesya(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14D0(0), m14EC(0), m14F0(0), m14F4(0), m14F8(0),
      m1514(0), m151C(0), m1520(0), m1528(0), m152C(0), m1530(0), m1534(120), m1538(50), m1540(0.0f), m1548(0),
      m154C(0.0f), m1550(0.0f), m1554(0.0f), m1558(0.0f), m155C(0.0f), m1560(0.0f), m1564(0.0f), m1568(0.05f),
      m15C0(90), m15C4(9.0f), m15CC(0) {
    mFlag22 = 1;
    mFlagB11 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em101_01_00.csv");
    atn::Csv csv(info->m2C, strlen(info->m2C));
    do {
        const char* s = csv.fn_8005B9CC();
        if (s == 0 || strcmp(s, "") == 0) {
            break;
        }
        if (strncmp(s, "db_name=", 8) == 0) {
            name = s + 8;
        }
    } while (csv.fn_8005BBF8());
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    m5C->mParam.m9C |= 0x1000;
    void* group = m15C8.Load("Em101LittleZesya");
    fn_80087994(group, "DebugDisp", &m15CC, 0, 0);
    fn_800874C8(group, "WaitFrame", &m15C0, 0, 1000000, 1, 0, 0);
    fn_8008771C(group, "AttackChanceFrame", &m15C4, 0, 0, 0.0f, 100000.0f, 1.0f);
    m156C.fn_8023780C("/effect/ef_bs007a.efp");
    m1578.fn_8023780C("cm002_00.efp");
    m1584.fn_8023780C("/effect/ef_sc019a.efp");
    mFlagB5 = 1;
}

Em101LittleZesya::~Em101LittleZesya() {}

int Em101LittleZesya::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m156C.fn_80237874() == 0 &&
        m1578.fn_80237874() == 0 && m1584.fn_80237874() == 0 && m14B0.fn_80473F50() == 0) {
        mStatus.m18 |= 0x1C0;
        m58C = 0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        fn_8016E970(3);
        fn_8025CFF4();
        m1518 = m568;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

void Em101LittleZesya::fn_8025CFF4() {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == ';' || *key == '\0') {
            continue;
        }
        if (strcmp(key, "WaitFrame") == 0) {
            m15C0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackChanceFrame") == 0) {
            m15C4 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "WaitPointID") == 0) {
            m14BC = table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
        } else if (strcmp(key, "ShootPointID") == 0) {
            m14C0 = table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
        } else if (strcmp(key, "HighJumpAttackPointID") == 0) {
            while (true) {
                int id = atoi(csv.fn_8005B9CC());
                if (id <= 0) {
                    break;
                }
                RoutePlace_unk* place = table->FindPlace_unk(id);
                if (place != 0) {
                    m14C4.push_back(place->mPos);
                }
            }
        } else if (strcmp(key, "NormalAttackID") == 0) {
            m151C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "HighJumpAttackID") == 0) {
            m1520 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "LightSwordID") == 0) {
            m1524 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WarpSlashID") == 0) {
            m1528 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "RecoverRate") == 0) {
            m1568 = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

// inferred: name invented
inline float Em101LittleZesya::HpRate_unk() {
    return (float)mStatus.m04 / (float)mStatus.mParamB.m84;
}

void Em101LittleZesya::vf10() {
    m14B8 = mC58.mC4;
    if (m14B8 == 0) {
        m14B8 = lbl_8087F8A0->mHead;
    }
    if (mC58.mC0 != 0 && m14B8 != 0) {
        m14D0++;
        fn_8025D7B0();
    }
    m14F0 = m14EC;
    fn_8014C540();
    fn_80145334();
}

extern "C" void fn_8025D6D0(Em101LittleZesya* zesya) {
    zesya->m14FC = zesya->mPos;
    zesya->m14F8 = 1;
    zesya->m1508 = zesya->mRot;
}

inline void Em101LittleZesya::Idle_unk() {
    m14D0 = 0;
    m14F4 = 0;
    m58C = 0;
    fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, this, 500, 1);
    fn_80239DAC(lbl_8087F3C0, this, 102, 1);
}

extern "C" void fn_8025D704(Em101LittleZesya* zesya) {
    zesya->mColli.mFlags |= 1;
    zesya->fn_8016DA4C();
    zesya->Idle_unk();
    zesya->fn_8016E970(3);
    zesya->fn_8015495C(0, 1, 1, 1);
    zesya->m14F8 = 0;
}

void Em101LittleZesya::fn_8025DE54() {
    if (m55C - 6U > 1) {
        mFlag5 = 1;
        mFC0 = m14B8;
    }
    fn_80139560();
}

void Em101LittleZesya::fn_8025DE7C() {
    if (fn_80158BB4()) {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 0;
        fn_8016E970(3);
        fn_80239DAC(lbl_8087F3C0, this, 500, 1);
        fn_80239DAC(lbl_8087F3C0, this, 102, 1);
    } else {
        fn_80139560();
        if (lbl_8087F048 != 0 && fn_80158CA4() != 0) {
            if (mControl && mControl->vf30() > m598) {
                m598 = mControl->vf30();
                m594 = 1;
                m590 = fn_800F8548(lbl_8087F048);
            }
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, m638, m590, 30, -1, 0.0f);
        }
    }
}

void Em101LittleZesya::fn_8025E008() {
    m14D0 = 0;
    m14F4 = 0;
    m58C = 11;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m156C, &mMotion);
    m1548 = 2;
}

void Em101LittleZesya::fn_8025E748() {
    float frame = mMotion.mTracks[0].m08;
    if (17.0f < frame && frame <= 19.0f) {
        fn_800F8C6C(lbl_8087F048, 0, 0, this, m1528, m590, 30, -1, 0.0f);
    } else {
        frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            Idle_unk();
        }
    }
    m1540 = mMotion.mTracks[0].m08;
}

// NONMATCHING: ~95% - f1/f2 of the int->float conversion of the maximum HP (4 variants)
void Em101LittleZesya::fn_8025E834() {
    if (m14D0 > 10) {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 13;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(this, 500);
        PlayEffect_unk(&m1578, &mMotion);
    }
}

// inferred: back to state 0 (name invented; the same block is inlined in fn_8025DE7C and fn_8025E9E4)
inline void ResetState_unk(Em101LittleZesya* z) {
    z->m14D0 = 0;
    z->m14F4 = 0;
    z->m58C = 0;
    z->fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, z, 500, 1);
    fn_80239DAC(lbl_8087F3C0, z, 102, 1);
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetTrans_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 96.3% - only the offset of "hips" in @stringBase0 and the .sdata2 floats: the strings and
// floats of the unwritten functions are missing from the pools
void Em101LittleZesya::fn_8025E9E4() {
    if (m14F4 == 0) {
        if (mMotion.mTracks[0].m08 > 60.0f) {
            m12CC = 1.0f;
            atn::Vector3 pos = GetTrans_unk(mMotion.GetNodeMtx_unk("hips"));
            fn_8025EAE4();
            m14F4 = 1;
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 100.0f) {
            ResetState_unk(this);
        }
    }
}

void Em101LittleZesya::fn_8025F35C() {
    switch (m55C) {
    case 6:
        if (m560 != 2) {
            ResetState_unk(this);
            m14D0 = m15C0 / 2;
        }
        break;
    case 7:
        break;
    default:
        ResetState_unk(this);
        m14D0 = m15C0 / 2;
        break;
    }
    fn_80139560();
}

void Em101LittleZesya::fn_8025E930() {
    mStatus.mParamB.m8C += m1568;
    if (HpRate_unk() >= 0.99f) {
        Idle_unk();
    }
}

void Em101LittleZesya::fn_8025F754() {
    if (m55C != 6) {
        Idle_unk();
    }
    fn_80139560();
}

void Em101LittleZesya::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C - 11U > 2) {
        if (m58C == 10) {
            msg->m10 *= 0.0f;
            msg->m64 = 3;
            msg->m90 = 0;
            msg->m84 = 2;
            msg->m88 = msg->mData->mC4;
            msg->m68 = 0;
            return;
        }
    } else {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
    m14D0 += 30;
}

extern "C" void fn_8025FE30(Em101LittleZesya* zesya, const ZesyaWarp_unk* warp) {
    zesya->m15BC = 1;
    zesya->m15A8 = *warp;
}
