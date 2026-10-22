#include "game/Em101LittleZesya.h"
#include "chr/ControlBase.h"
#include "chr/ControlDive.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/HomingBullet.h"
#include "math/Vec.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

// inferred: empty release assert (the original functions are not auto-inlined)
#define ASSERT_UNK(x) ((void)0)

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
extern "C" int fn_80370A78(Field_unk* field, int index);         // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6); // inferred: nearest route point (as in gmk/MeetingSpot.cpp)
// inferred: collision world and its ray test (as in game/ChaserNpc.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10;
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
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
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
extern "C" double sqrt(double x);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em101ElderZesya.cpp)
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct ZesyaMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" ZesyaMsgTable_unk* lbl_8087F1E4;
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
// inferred: same with the locals in the order of game/Em305Vampire.cpp (name invented)
inline void PlayEffectA_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

// inferred: rotation matrix z, y, x (as ElderRotZYX_unk in game/Em101ElderZesya.cpp; name invented)
inline void ZesyaRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    __MtxRotate(m.m, 'z', rot.z);
    __MtxRotate(m.m, 'y', rot.y);
    __MtxRotate(m.m, 'x', rot.x);
}

// inferred: center of the party (as in game/Em101ElderZesya.cpp; name invented)
inline atn::Vector3 PartyCenter_unk() {
    atn::Vector3 center = lbl_807C7030;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        center += p->mPos;
    }
    center *= 1.0f / (float)lbl_8087F8A0->mCount;
    return center;
}

// inferred: index of the point of m14C4 nearest to `pos` on the ground plane (name invented)
inline int Em101LittleZesya::Nearest_unk(const atn::Vector3& pos) {
    float dist = 1e16f;
    unsigned int i;
    int best = -1;
    for (i = 0; i < m14C4.mSize; i++) {
        atn::Vector3 d = pos - m14C4.mData[i];
        float len = sqrt(d.x * d.x + d.z * d.z);
        if (dist > len) {
            dist = len;
            best = i;
        }
    }
    return best;
}

// inferred: route point next to `target` (at most 40 from it, dropped to the ground) and the rotation
// looking at the target; name invented
inline void Em101LittleZesya::WarpNear_unk(CharaTask* target, atn::Vector3* dest, float* rot) {
    int point = fn_803C1560(lbl_8087F430->m10D8, &target->mPos, 0, 0, 0, 1, -1.0f);
    *dest = lbl_8087F430->m10D8->mPoints[point - 1].mPos;
    *rot = (target->mPos - *dest).ToRotationZ_unk().y;
    atn::Vector3 e = *dest - target->mPos;
    e.y = 0.0f;
    if (PSVECMag(&e) > 40.0f) {
        *dest = target->mPos + e.GetNormalized_unk() * 40.0f;
        CollResult_unk result;
        atn::Vector3 to = atn::Vector3(0.0f, -500.0f, 0.0f) + *dest;
        if (fn_8004ED34(lbl_8087EE98, &result, dest, &to, 0x80000000, &mColli, 0) != 0) {
            *dest = result.m10;
        }
    }
}

// inferred: Em101ElderZesya seen through m1530 (only the flag at 0x16AC is read; name invented)
struct ElderView_unk {
    unsigned char m0000[0x16AC];
    int m16AC; // inferred: nonzero while the elder keeps the little Zesya waiting
    bool IsFree_unk() const { return !m16AC; }
};

// inferred: homing parameters with their defaults (name invented)
struct ZesyaHoming_unk : public HomingParam_unk {
    ZesyaHoming_unk() {
        mTarget = 0;
        mAccel = 0.0f;
        m08 = -1.0f;
        mMaxSpeed = 10.0f;
        mTurn = 0.09424778f;
        mTime = 300.0f;
        mDelay = 5.0f;
        mNode = 0;
        mCount = -1;
    }
};

// inferred: the state functions are not inlined here in the original; with -ipa file ours were, until each
// got empty release asserts (ASSERT_UNK) that push it over the auto-inline threshold
void Em101LittleZesya::fn_8025D7B0() {
    if (m14F8 > 0) {
        return;
    }
    if (m55C == 2) {
        fn_8016E970(3);
    }
    if (m15BC != 0 && --m15A8.m04 <= 0 && m55C == 3) {
        switch (m15A8.m00) {
        case 0:
            fn_8025F07C();
            break;
        case 1:
            fn_8025F45C();
            break;
        case 2:
            fn_8025E008();
            break;
        }
        m15BC = 0;
        return;
    }
    switch (m58C) {
    case 1:
        fn_8025DE7C();
        break;
    case 11:
        fn_8025E100();
        break;
    case 9:
        fn_8025E748();
        break;
    case 12:
        fn_8025E834();
        break;
    case 13:
        fn_8025E930();
        break;
    case 8:
        fn_8025F35C();
        break;
    case 10:
        fn_8025F754();
        break;
    case 14:
        fn_8025E9E4();
        break;
    default:
        fn_8025DE54();
        if (m55C != 6) {
            fn_8025D90C();
        }
        break;
    }
}

// NONMATCHING: 99.04% - stack slots: the original keeps the rows of the 4x4 matrix of ToRotation_unk in
// descending order and the effect locals higher (8 variants: helper inlines, locals hoisted,
// GetNormalized_unk form); r3/r4/r5 of the first block
void Em101LittleZesya::fn_8025D90C() {
    atn::Vector3 target = m14B8->mPos;
    atn::Vector3 d = target - mPos;
    float dist = PSVECMag(&d);
    if (m151C == 0) {
        return;
    }
    float range = m151C->m40 + mStatus.mParamB.m28;
    m1538 = mStatus.IsState_unk(8) || mStatus.IsState_unk(0x200) ? 150 : 50;
    m153C = mStatus.IsState_unk(8) || mStatus.IsState_unk(0x200) ? 240 : 80;
    if (!((ElderView_unk*)m1530)->IsFree_unk()) {
        if (m14D0 > m153C) {
            m14D0 = 0;
            m14F4 = 0;
            m58C = 14;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
            if (m14C0 != 0) {
                mPos = m14C0->mPos;
            }
            fn_80232B7C(this, 102);
            PlayEffect_unk(&m1584, &mMotion);
        }
    } else if (dist <= range) {
        if (m14D0 > m1538) {
            m14D0 = 0;
            m14F4 = 0;
            m58C = 1;
            fn_801561E4(fn_80219E6C(200));
            m598 = 0;
            atn::Vector3 dir = m14B8->mPos - mPos;
            PSVECNormalize(&dir, &dir);
            atn::Vector3 rot = dir.ToRotation_unk();
            mRot.y = rot.y;
        }
    } else if (m14D0 > m1534) {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 11;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(this, 101);
        PlayEffectA_unk(&m156C, &mMotion);
        m1548 = 1;
    }
}

void Em101LittleZesya::fn_8025DE54() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
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
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
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

// NONMATCHING: 99.89% - r3/r4 of the copy of the route point (__VecCopy family; 6 variants)
// inferred: starts the warp chosen by fn_8025D90C/fn_8025E008 (m1548: 1 next to the target, 3 to the
// shoot point, otherwise to the wait point)
void Em101LittleZesya::fn_8025E100() {
    if (m14D0 <= 10) {
        return;
    }
    if (m1548 == 1) {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 9;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 1.0f, 0.1f);
        // inferred: m154C-m1554 hold the warp position (floats in the header)
        atn::Vector3* dest = (atn::Vector3*)&m154C;
        CharaTask* target = m14B8;
        mMotion.mTracks[0].m08 = 0.0f;
        if (target != 0) {
            int point = fn_803C1560(lbl_8087F430->m10D8, &target->mPos, 0, 0, 0, 1, -1.0f);
            *dest = lbl_8087F430->m10D8->mPoints[point - 1].mPos;
            m1558 = (target->mPos - *dest).ToRotationZ_unk().y;
            atn::Vector3 e = *dest - target->mPos;
            e.y = 0.0f;
            if (PSVECMag(&e) > 40.0f) {
                *dest = target->mPos + e.GetNormalized_unk() * 40.0f;
                CollResult_unk result;
                atn::Vector3 to = atn::Vector3(0.0f, -500.0f, 0.0f) + *dest;
                if (fn_8004ED34(lbl_8087EE98, &result, dest, &to, 0x80000000, &mColli, 0) != 0) {
                    *dest = result.m10;
                }
            }
        }
        mPos = *(atn::Vector3*)&m154C;
        mRot.y = m1558;
        m1540 = 0.0f;
    } else if (m1548 == 3) {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 14;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        if (m14C0 != 0) {
            mPos = m14C0->mPos;
        }
        fn_80232B7C(this, 102);
        PlayEffect_unk(&m1584, &mMotion);
    } else {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 12;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
        if (m14BC != 0) {
            mPos = m14BC->mPos;
            mRot.y = m14BC->m14;
        }
        fn_80232B7C(this, 101);
        PlayEffect_unk(&m156C, &mMotion);
    }
}

void Em101LittleZesya::fn_8025E748() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
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

void Em101LittleZesya::fn_8025E834() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
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

void Em101LittleZesya::fn_8025E930() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    mStatus.mParamB.m8C += m1568;
    if (HpRate_unk() >= 0.99f) {
        Idle_unk();
    }
}

void Em101LittleZesya::fn_8025E9E4() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
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

// inferred: fires one homing light sword at each player, spread in a circle around the spine
// NONMATCHING: 94.30% - the original stores the 0x43300000 words of the two int->float conversions in the
// prologue and keeps one more FPR for the constants (f16-f31 against our f17-f31 plus a GPR), so the saved
// registers shift by one (r20 against r19) and the stack slots differ; the 0.5f of vf40 sits before
// these constants in the original pool (11 variants)
void Em101LittleZesya::fn_8025EAE4() {
    if (m1524 != 0) {
        int i = 0;
        Model_unk* model = GetModel_unk();
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            atn::Vector3 pos = GetTrans_unk(model->GetNodeMtx_unk("spine"));
            atn::Vector3 ofs(0.0f, 0.0f, 30.0f);
            atn::Matrix3x4 m(model->GetNodeMtx_unk("spine"));
            m.m[0][3] = 0.0f;
            m.m[1][3] = 0.0f;
            m.m[2][3] = 0.0f;
            PSMTXMultVec(m.m, &ofs, &ofs);
            pos += ofs;
            float angle = 360.0f / lbl_8087F8A0->mCount * i;
            atn::Vector3 dir(6.0f, 0.0f, 3.0f);
            Mtx rz;
            PSMTXRotRad(rz, 'z', 0.017453292f * angle);
            PSMTXMultVec(rz, &dir, &dir);
            atn::Matrix3x4 r;
            ZesyaRotZYX_unk(r, mRot);
            PSMTXMultVec(r.m, &dir, &dir);
            ZesyaHoming_unk param;
            PlayerTask* leader = lbl_8087F8A0->mHead;
            if (leader->mFlag28) {
                if (p != leader) {
                    continue;
                }
                param.mTarget = leader;
            } else {
                param.mTarget = p->CanAct_unk() ? p : leader;
            }
            param.mDelay = 10.0f;
            param.mAccel = 0.3f;
            param.mMaxSpeed = 2.5f;
            param.mTurn = 0.5235988f;
            param.mTime = (float)m1524->m5C;
            fn_800F8574(lbl_8087F048, this, m1524, &pos, &dir, &param, 0x24, 30, 0.0f, 1.0f);
            i++;
        }
    }
}

void Em101LittleZesya::fn_8025F07C() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 8;
    m14D0 = 0;
    m14F4 = 0;
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xA9), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
    atn::Vector3 dir;
    atn::Vector3 rot = (mPos - m1590).ToRotationZ_unk();
    dir.x = 0.0f;
    dir.y = rot.y;
    dir.z = 0.0f;
    fn_80178208(new (1, "", "", 0) chr::ControlWarp(this, m1590, dir));
}

void Em101LittleZesya::fn_8025F35C() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
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

// NONMATCHING: 98.2% - registers of the loop (index/offset/best swapped and the order of the two
// increments, as in Em101ElderZesya::fn_802598DC) and the stack slot of the party center (10 variants:
// written out, helper, pointer walk, signed index, while, declaration orders, comparison order)
void Em101LittleZesya::fn_8025F45C() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 10;
    m14D0 = 0;
    m14F4 = 0;
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xAB), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
    if (m15BC != 0) {
        m1590 = m14C4.mData[Nearest_unk(m15A8.m08)];
    } else {
        m1590 = m14C4.mData[Nearest_unk(PartyCenter_unk())];
    }
    // inferred: the third argument is the address of the destination (CharaTask declares it int)
    fn_8016D74C(m1520, 0, (int)&m1590);
}

void Em101LittleZesya::fn_8025F754() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
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

// NONMATCHING: 99.53% - r27/r29 of the elder and of the rotation temporary, and the .sdata2 label of 0.5f
// (9 variants: helper shared with fn_8025E100, declaration orders, pointer to the destination, HpRate_unk)
// inferred: on a hit: below half HP the elder is told to warp next to its own route point (warp action 2);
// HP never drops below 20%
void Em101LittleZesya::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8, -1);
        case 20:
        reset:
            m58C = 0;
            m14D0 = 0;
            m14F4 = 0;
            break;
        }
    }
    float rate = mStatus.m04 / (float)mStatus.mParamB.m84;
    if (rate < 0.5f) {
        if (msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
            if (fn_80370A78(lbl_8087F430, 0x6B) == 0) {
                fn_80370AE4(lbl_8087F430, 0x6B, 1);
            }
        }
        if (m15BC == 0) {
            ZesyaWarp_unk warp;
            warp.m00 = 2;
            atn::Vector3* dest = &warp.m08;
            CharaTask* elder = (CharaTask*)m1530;
            if (elder != 0) {
                float rot;
                WarpNear_unk(elder, dest, &rot);
            }
            warp.m04 = 20;
            m15BC = 1;
            m15A8 = warp;
        }
    }
    if (rate < 0.2f) {
        mStatus.m04 = 0.2f * (float)mStatus.mParamB.m84;
    }
    if (mStatus.IsState_unk(0x20)) {
        mStatus.fn_8013322C(0x20);
        fn_8015E4B0(msg->m28 * 2.0f, -1);
    }
}

extern "C" void fn_8025FE30(Em101LittleZesya* zesya, const ZesyaWarp_unk* warp) {
    zesya->m15BC = 1;
    zesya->m15A8 = *warp;
}
