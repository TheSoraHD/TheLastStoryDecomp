#include "game/Em005DoppelGanger.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/EnemyManager.h"
#include "game/SkillEffectManager.h"
#include "chr/ControlBase.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: character data by name (game/CharaTask.cpp declares it with an int id; same C symbol)
extern "C" chr::CharaData_unk* fn_8020A81C(const char* name);
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" double acos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" int rand();
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xEC];
    unsigned int mEC; // inferred: flags (bit 3 set while a doppelganger exists)
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" void fn_8023A02C(EffectMgr_unk* mgr, void* owner, int arg2, int arg3);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: plays an effect on the model (locals in the order of game/Em305Vampire.cpp)
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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

// inferred: same as PlayEffect_unk with the locals in the order of game/Em003LesserDaemon.cpp
inline void PlayEffectB_unk(atn::EfpFile* efp, atn::Model* model) {
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

inline void PlayEffectM_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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

// inferred: word at 0x1C of the running control (class unknown)
struct Control1C_unk {
    unsigned char m00[0x1C];
    int m1C;
};
// inferred: 0x1C-byte record at 0x1470 of the battle characters (as PlayerState_unk in
// game/CharaTask.cpp; names invented)
struct DoppelState_unk {
    DoppelState_unk(short id) : m00(id), m02(0), m04(0), m08(150), m0C(0.0f, 0.0f, 0.0f), m18(0) {}
    short m00;
    short m02;
    int m04;
    int m08;
    atn::Vector3 m0C;
    int m18;
};
// inferred: the doppelganger as a battle character with that record (names invented)
struct DoppelChara_unk : public CharaTask {
    void SetState_unk(const DoppelState_unk& state) { m1470 = state; }

    unsigned char m1424[0x1470 - 0x1424];
    DoppelState_unk m1470;
};

extern "C" int lbl_8087F3D8; // inferred: cleared by vf14 (file static)

// inferred: offsets of the hit sphere (static constructors at 0x80254530)
static atn::Vector3 sSphereOffset_unk(0.0f, 0.0f, 1.0f);
static atn::Vector3 sOffsetB_unk(0.0f, 0.0f, 26.81f);
static atn::Vector3 sOffsetC_unk(-130.0f, 0.0f, 26.81f);

// inferred: through an inline taking `this`; with the test written in the destructor body the vtable
// store of the original is dropped (name invented)
inline void ClearDoppel_unk(Em005DoppelGanger* self) {
    if (lbl_8087F3C0 != 0) {
        lbl_8087F3C0->mEC &= ~8;
    }
}

// inferred: field events and the doppelganger helpers called by vf10 (classes unknown)
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg3); // inferred: sets a field flag
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" SkillEffectManager* lbl_8087F9E8;
extern "C" void fn_8059C8E8(SkillEffectManager* mgr, CharaTask* chara); // inferred: traps the character
extern "C" void fn_8059C98C(SkillEffectManager* mgr, CharaTask* chara); // inferred: frees the character
extern "C" atn::EfpFile* fn_800EFC64(int id); // inferred: effect of a skill (game/BattleEfp_unk.cpp)
extern "C" void fn_805991E0(Bullet_unk* bullet); // inferred: stops an effect of a skill
extern "C" void fn_800E9ED8(CharaTask* chara); // inferred: BattleCharaTask update while copying
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, void* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)
// inferred: AI weights by party member (fn_80103F60, as in game/Em403WarLion.cpp)
struct DoppelAi_unk {
    unsigned char m000[0x128];
    float m128[1];
};
extern "C" DoppelAi_unk* fn_80103F60(Effect_unk* mgr, CharaTask* chara);
extern "C" int fn_80108378(DoppelAi_unk* ai, CharaTask* chara); // inferred: index of `chara`, < 0 if missing
// inferred: game settings (lbl_8087F0A8, class unknown); m80 is the attack setup id
struct Settings_unk {
    unsigned char m00[0x80];
    int m80;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: collision world and its tests (as in game/Em003LesserDaemon.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
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
extern "C" int fn_8004D388(void* world, CollResult_unk* result, const atn::Vector3* center, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test

// NONMATCHING: 99.6% - r26/r27 of the second std::string and r25/r26 of the strlen of the parameters (same
// as Em101ElderZesya / Em201ZanGurg)
Em005DoppelGanger::Em005DoppelGanger(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(240), m14C4(0), m14F8(0),
      m14FC(0), m1500(0), m1504(0), m1510(0), m1514(0), m1518(0), m151C(1), m1520(0), m1524(4), m1528(0),
      m152C(0), m1540(0), m1544(0), m1548(0), m154C(0), m1554(0) {
    mFlag22 = 1;
    mFlagB15 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em005_00_00.csv");
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
    m1508.vf0C(path.c_str());
    m14E0.LoadRequest("/effect/ef_bs024a.efp");
    m14C8.LoadRequest("/effect/ef_cm007.efp");
    m14EC.LoadRequest("/effect/ef_bs038a.efp");
    m14D4.LoadRequest("/effect/ef_cm007.efp");
}

Em005DoppelGanger::~Em005DoppelGanger() {
    ClearDoppel_unk(this);
}

int Em005DoppelGanger::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1508.fn_80473F50() == 0 &&
        m14C8.IsLoading() == 0 && m14E0.IsLoading() == 0 && m14EC.IsLoading() == 0 &&
        m14D4.IsLoading() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4011;
        mC10 |= 2;
        RouteTable_unk* table = lbl_8087F430->m10D8;
        if (m1508.fn_80470580() != 0 && table != 0) {
            atn::Csv csv((const char*)m1508.fn_80470580(), m1508.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "JumpPoint") == 0) {
                    RoutePlace_unk* place = table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                    if (place != 0) {
                        m1528 = place;
                    }
                } else if (strcmp(key, "WarpPoint") == 0) {
                    RoutePlace_unk* place = table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                    if (place != 0) {
                        m152C = place;
                    }
                } else if (strcmp(key, "SrcCopyCharaId") == 0) {
                    m1500 = atoi(csv.fn_8005B9CC());
                    if (m1500 < 0) {
                        m1504 = lbl_8087F8A0->mHead;
                    } else {
                        m1504 = lbl_8087F8A0->Find_unk(m1500);
                    }
                } else if (strcmp(key, "DstCopyRobotId") == 0) {
                    m14F8 = atoi(csv.fn_8005B9CC());
                    m14FC = (CopyRobotTask*)EnemyManager::sInstance->Find_unk(m14F8);
                }
            } while (csv.fn_8005BBF8());
        }
        *(CharaTask**)((char*)m14FC + 0x14B4) = m1504;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        switch (fn_80219558(m1504->m50)) {
        case 1:
            m5C = fn_8020A81C("ch2017_51");
            break;
        case 2:
            m5C = fn_8020A81C("ch2017_52");
            break;
        case 3:
            m5C = fn_8020A81C("ch2017_53");
            break;
        case 4:
            m5C = fn_8020A81C("ch2017_54");
            break;
        case 5:
            m5C = fn_8020A81C("ch2017_55");
            break;
        case 6:
            m5C = fn_8020A81C("ch2017_56");
            break;
        }
        mStatus.fn_8012B3E8();
        mStatus.fn_8012B988();
        for (int i = 0; i < 4; i++) {
            mStatus.m2D0[i] = m5C->mE8[i];
        }
        fn_8016E4C4(0);
        return 1;
    }
    return 0;
}


// inferred: resets the step and the bullet id (name invented)
inline void Em005DoppelGanger::ResetStep_unk() {
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
}

// inferred: moves along the route, or updates the control (names invented)
inline void Em005DoppelGanger::Move_unk() {
    atn::Vector3 rot = mRot;
    float dist = 0.0f;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 move = mRoute.m58;
        dist = PSVECMag(&move);
        if (dist > 0.0001f) {
            rot = move.GetNormalize().ToRotation_unk();
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(rot, 1, dist, m568);
}

inline int Em005DoppelGanger::IsHiddenPhase_unk() {
    int r = 1;
    if (m14BC == 2) {
        r = 0;
    }
    return r;
}

// inferred: hit points over the maximum (name invented)
inline float Em005DoppelGanger::HpRate_unk() {
    float rate;
    if (mStatus.mParamB.m84 > 0) {
        rate = mStatus.m04 / (float)mStatus.mParamB.m84;
    } else {
        rate = 0.0f;
    }
    return rate;
}

// inferred: the copy robot is shown (m54C bit 3 clear); name invented
inline bool IsShown_unk(CharaTask* chara) {
    return (chara->m54C & 8) != 8;
}

// inferred: timer of the running control (class unknown)
struct ControlTime_unk {
    unsigned char m00[0x08];
    float m08;
};
// inferred: the running control ran out of time (name invented)
inline bool IsControlTimeUp_unk(CharaTask* chara) {
    return ((ControlTime_unk*)chara->mControl.get())->m08 <= 0.0f;
}

namespace {

// inferred from the RTTI string "@unnamed@em005_doppel_ganger_cpp@::ControlCall" (vtable 0x80784340):
// the doppelganger calls the other enemies to the area of its bullet (motion 0x141)
class ControlCall : public chr::ControlBase {
public:
    ControlCall(CharaTask* chara) : chr::ControlBase(chara) {
        chara->m560 = 0x25;
        chr::Motion_unk* motion = &mChara->mMotion;
        motion->m34C = 1;
        motion->mTracks[0].m20 = 1.0f;
        motion->fn_80097C08(0, 0x141, 0, 0, 1, 0.0f, 0.1f);
        motion->mTracks[0].m0C = 1.0f;
    }
    virtual int vf0C();
};

} // namespace

// NONMATCHING: 96.3% - the (m54C & 8) != 8 tests of IsShown_unk are materialized in the original
// (subfic/or/srwi., as in RouteWalker_unk::fn_80127EF4 and CharaTask::fn_80149A30; ~25 forms tried),
// stack slots of the rotation matrices of case 9/route and FPR of the hit point rate (r4/r5)
void Em005DoppelGanger::vf10() {
    if (m151C != 0) {
        m151C = 0;
        fn_80370320(lbl_8087F430, 0xD9, 3, 0);
        switch (fn_80219558(m1504->m50)) {
        case 0:
        case 2:
        case 3:
            fn_80154654(mC50, 0x238D);
            m1524 = 4;
            break;
        case 1:
        case 5:
            fn_80154654(mC50, 0x2390);
            m1524 = 2;
            break;
        case 4:
        case 6:
            fn_80154654(mC50, 0x238F);
            m1524 = 3;
            break;
        }
    }
    if (fn_80370A78(lbl_8087F430, 0xC9) == 1) {
        RoutePlace_unk* jump = m1528;
        if (jump != 0) {
            mPos = jump->mPos;
            m1504->mPos = jump->mPos;
        }
        m14BC = 1;
        fn_8016E4C4(1);
        ResetStep_unk();
        m58C = 0;
        atn::Vector3 pos = mPos;
        fn_801644D4(pos, m1504);
        m1504->fn_80161570(0x1D9, 0, 0, -1.0f, -1.0f);
    }
    int wasTrapped = m154C;
    m1548 = 0;
    m154C = 0;
    if (m1540 > 0) {
        m1540--;
        CopyRobotTask* robot = m14FC;
        if (robot != 0) {
            atn::Vector3 d = robot->mPos - m1530;
            if (PSVECMag(&d) < m153C) {
                m1544++;
                if (HpRate_unk() > 0.5f) {
                    mStatus.m04 -= 0.02f * (float)mStatus.mParamB.m84 / 30.0f;
                }
                if (m154C == 0 && m14FC != 0) {
                    m154C = 1;
                }
                m1548 = 1;
            } else {
                m1544 = 0;
                m1548 = 0;
            }
            m1544 = m1544 < 0 ? 0 : m1544;
        }
        if (m1540 == 0) {
            m1544 = 0;
            fn_80239DAC(lbl_8087F3C0, &m1530, 0, 1);
        }
    }
    if (m154C != 0 && wasTrapped == 0) {
        fn_8059C8E8(lbl_8087F9E8, m14FC);
        fn_8023A02C(lbl_8087F3C0, m14FC, 11, 16);
        fn_8023A02C(lbl_8087F3C0, m14FC, 11, 8);
    } else if (m154C == 0 && wasTrapped != 0) {
        fn_8059C98C(lbl_8087F9E8, m14FC);
    }
    if (m14BC == 0) {
        fn_80370320(lbl_8087F430, 0x389, 1, 0);
        if (m14FC != 0) {
            m14FC->fn_80179AD0(0x20, 1);
            m14FC->fn_80179AD0(0x10, 1);
            m14FC->fn_80179AD0(4, 1);
            m14FC->vf9C(0);
            m14FC->m14B8 = this;
            int state = m58C;
            if (state - 9U <= 2 || state - 5U <= 1 || state == 1) {
                m14FC->fn_801CCD00(1);
            } else {
                m14FC->fn_801CCD00(0);
            }
        }
        for (int i = 0; i < 32; i++) {
            Bullet_unk* b = lbl_8087F9E8->GetBullet_unk(i);
            if (b->IsActive_unk() && (b->m08 == 0 || b->m08->mType != 2) && b->m04 != 0 && b->m04->m01 == 4) {
                if (b->m10.z > sOffsetB_unk.z) {
                    atn::EfpFile* efp = fn_800EFC64(b->m04->mB0);
                    if (efp != 0) {
                        fn_80239DAC(lbl_8087F3C0, &m1530, 0, 1);
                        m1540 = 180;
                        m153C = 0.8f * (b->m5C * b->m28);
                        m1530 = b->m10;
                        m1530.z = 2.0f * sOffsetB_unk.z - m1530.z;
                        if (m1530.z + m153C > sOffsetB_unk.z) {
                            m1530.z = sOffsetB_unk.z - m153C;
                        }
                        CopyRobotTask* robot = m14FC;
                        if (robot != 0) {
                            atn::Vector3 d = m1530 - robot->mPos;
                            if (PSVECMag(&d) <= m153C) {
                                if (sOffsetC_unk.x > mPos.x) {
                                    m1530.x += m153C;
                                } else {
                                    m1530.x -= m153C;
                                }
                            }
                        }
                        fn_80232B7C(&m1530, 0);
                        float scale = m153C / 15.0f;
                        fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, m1530, chr::lbl_807C7030,
                                    Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, scale);
                        fn_8023A02C(lbl_8087F3C0, &m1530, 0, 16);
                        fn_8023A02C(lbl_8087F3C0, &m1530, 0, 8);
                        if (fn_80370A78(lbl_8087F430, 0xE6) == 0) {
                            fn_80370AE4(lbl_8087F430, 0xE6, 1);
                        }
                    }
                }
                fn_805991E0(b);
            }
        }
    } else if (m14BC == 1) {
        fn_80239DAC(lbl_8087F3C0, &m1530, 0, 0);
        fn_80370320(lbl_8087F430, 0x389, 0, 0);
        CopyRobotTask* robot = m14FC;
        m1540 = 0;
        if (robot != 0 && IsShown_unk(robot)) {
            robot->fn_8016E4C4(0);
        }
        if (m55C != 6 || (m560 != 0x7C && m560 != 0x7E && m560 != 0x7B)) {
            PlayEffect_unk(&m14C8, &mMotion);
            fn_800C344C("SE_EM005_010_1", &mPos, 0, -1, 1.0f);
            if (m152C != 0) {
                mPos = m152C->mPos;
            }
            m14BC = 2;
            fn_8016E4C4(IsHiddenPhase_unk());
        }
    } else if (m14BC == 2) {
        fn_80370320(lbl_8087F430, 0x38A, 1, 0);
        CopyRobotTask* robot = m14FC;
        if (robot != 0) {
            if (m1520 < m1524) {
                if (!IsShown_unk(robot)) {
                    robot->fn_8016E4C4(1);
                }
                if (IsShown_unk(this)) {
                    fn_8016E4C4(0);
                }
            } else {
                if (IsShown_unk(robot)) {
                    robot->fn_8016E4C4(0);
                }
                if (!IsShown_unk(this)) {
                    fn_8016E4C4(1);
                }
            }
            m14FC->m14B8 = this;
            m14FC->fn_801CCD00(2);
            m14FC->fn_80179AD0(0x20, 0);
            m14FC->fn_80179AD0(0x10, 0);
            m14FC->fn_80179AD0(4, 0);
            CharaStatus* status = lbl_8087F8A0->mHead->GetStatus_unk();
            if (!status->IsState_unk(2)) {
                status->fn_80133130(0x200000, 999999999, 1);
            }
        }
        int state = m1470;
        if (state != 0 && --m1474 < 0) {
            if (m55C == 2) {
                switch (state) {
                case 13: {
                    ControlCall* control = new ((MemoryTag)3, "", "", 0) ControlCall(this);
                    fn_80178208(control);
                    m1470 = 0;
                    break;
                }
                case 1:
                    fn_80155A88(((DoppelChara_unk*)this)->m1470.m0C, 0);
                    m1470 = 2;
                    m1478 = 60;
                    break;
                case 2:
                    fn_80161570(0x140, 0, 0, -1.0f, -1.0f);
                    m1470 = 0;
                    break;
                case 14:
                    ResetStep_unk();
                    m58C = 8;
                    fn_8016E970(3);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x1EA, 0, 1, 1, 0.0f, 0.1f);
                    if (m14FC != 0) {
                        fn_80232B7C(0, 0);
                        PlayEffectB_unk(&m14EC, &m14FC->mMotion);
                    }
                    m1470 = 0;
                    break;
                }
            } else if (--m1478 < 0) {
                m1470 = 0;
            }
        }
    } else if (m14BC == 3) {
        CopyRobotTask* robot = m14FC;
        if (robot != 0) {
            if (IsShown_unk(robot)) {
                robot->fn_8016E4C4(0);
            }
            m14FC->fn_801CCD00(3);
        }
    }
    m14B8++;
    if (mC58.mC0 == 0) {
        fn_8016E970(3);
    } else if (!(m14BC != 2 && m14BC != 3)) {
        if (m58C == 8) {
            fn_80253C9C();
        } else {
            if (m55C == 3) {
                fn_8015495C(0, 1, 1, 1);
                fn_8016EB48(0, 0, 0);
            }
            if (mStatus.IsState_unk(0x20) && m58C != 2) {
                vf44();
            }
            fn_800E9ED8(this);
        }
    } else {
        if (m55C == 2) {
            fn_8016E970(3);
        }
        switch (m58C) {
        case 5:
            if (mMotion.mTracks[0].m00 == 0x145) {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    fn_80158EF0(lbl_8087F0A8->m80);
                }
                break;
            }
            Move_unk();
            if (m55C == 6 && m560 == 12) {
                if (IsControlTimeUp_unk(this)) {
                    ResetStep_unk();
                    m58C = 1;
                    fn_801561E4(fn_80219E6C(200));
                }
            } else {
                ResetStep_unk();
                m58C = 0;
                fn_8016E970(3);
            }
            break;
        case 1:
            fn_80253884();
            break;
        case 6: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetStep_unk();
                m58C = 0;
                fn_8016E970(3);
            }
            break;
        }
        case 9: {
            CharaTask* target = mC58.mC4;
            if (target != 0) {
                mRot = (target->mPos - mPos).ToRotationZ_unk();
            }
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetStep_unk();
                m58C = 0;
                fn_8016E970(3);
            } else if ((float)__fabs(mMotion.mTracks[0].m08 - 58.0f) < 1.1920929e-7f) {
                HitInfo_unk* data = fn_80219E6C(0x6D6);
                atn::Vector3 pos = mPos;
                CopyRobotTask* robot = m14FC;
                if (robot != 0) {
                    MtxPtr m = robot->GetModel_unk()->GetNodeMtx_unk("rightwepoint");
                    atn::Vector3 trans(m[0][3], m[1][3], m[2][3]);
                    pos = trans;
                    pos.z = 2.0f * sOffsetB_unk.z - trans.z;
                }
                atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                Mtx rm;
                PSMTXRotRad(rm, 'y', mRot.y);
                PSMTXMultVec(rm, &dir, &dir);
                fn_800F8574(lbl_8087F048, this, data, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
            }
            break;
        }
        case 10:
            fn_802531F4();
            if (m55C != 6) {
                ResetStep_unk();
                m58C = 0;
                fn_8016E970(3);
            }
            break;
        case 11: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetStep_unk();
                m58C = 0;
                fn_8016E970(3);
            }
            break;
        }
        default:
            if (m55C == 3) {
                fn_80252E6C();
            }
            if (m58C == 0) {
                fn_802531F4();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    if (!(m14BC != 2 && m14BC != 3)) {
        CopyRobotTask* robot = m14FC;
        if (robot != 0) {
            mColli.m3C = robot->mColli.m3C;
            mColli.m48 = robot->mColli.m48;
            mColli.m54 = robot->mColli.m54;
        }
    }
    DoppelAi_unk* ai = fn_80103F60(lbl_8087F048, this);
    if (ai != 0) {
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            int index = fn_80108378(ai, p);
            if (index >= 0) {
                if (m14BC == 0) {
                    ai->m128[index] = p == m1504 ? 1.0f : 0.0f;
                } else {
                    ai->m128[index] = 1.0f;
                }
            }
        }
    }
    if (lbl_8087F3D8 == 0) {
        if (fn_80370A78(lbl_8087F430, 0xEA) == 0) {
            for (unsigned int i = 0; i < 32; i++) {
                Bullet_unk* b = lbl_8087F9E8->GetBullet_unk(i);
                if (b != 0 && b->IsActive_unk() && b->fn_8059A264()) {
                    fn_80370AE4(lbl_8087F430, 0xEA, 1);
                    break;
                }
            }
        }
        lbl_8087F3D8 = 1;
    }
    if (m14BC != 0 && mStatus.IsState_unk(4)) {
        if (fn_80370A78(lbl_8087F430, 0xEB) == 0) {
            fn_80370AE4(lbl_8087F430, 0xEB, 1);
        }
    }
}

void Em005DoppelGanger::vf54() {
    CharaTask::vf54();
    if (m624.mSize != 0) {
        if (m14BC == 0) {
            m624.mData[0].m04.mRadius = 5.0f;
            m624.mData[0].m04.mCenter += sSphereOffset_unk * -3.0f;
        } else {
            m624.mData[0].m04.mRadius = 5.0f;
        }
    }
}

void Em005DoppelGanger::vf14() {
    lbl_8087F3D8 = 0;
    fn_80149A30();
}

// NONMATCHING: 98.98% - the original keeps an unreachable `b` after the `return` of the id test (5
// variants: switch, else if, goto, empty branch, double return)
void Em005DoppelGanger::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14BC == 0) {
        if (msg->mAttacker != m1504) {
            return;
        }
        if (m58C == 11 || m58C == 10 || m58C == 6) {
            if (fn_805F9920(&msg->m10) > 0.0001f) {
                PSVECNormalize(&msg->m10, &msg->m10);
            }
            msg->m10 *= 8.0f;
            if (fn_80370A78(lbl_8087F430, 0xE9) == 0) {
                fn_80370AE4(lbl_8087F430, 0xE9, 1);
            }
        } else {
            HitInfo_unk* data = msg->mData;
            if (data != 0) {
                int id = data->m04;
                if (id == 200) {
                    m14B4 = 0;
                    m14B8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    m58C = 1;
                    fn_8015DEC0((int)&msg->m10);
                    ((Control1C_unk*)mControl.get())->m1C = 1;
                    msg->mAttacker->vf7C(1);
                    msg->m90 = 0;
                    msg->m84 = 2;
                    return;
                } else if (id != 208 && id != 10000) {
                    return;
                }
            }
        }
    } else if (m14BC == 2 && m58C == 8) {
        return;
    }
    fn_80151448(msg);
    vf40(hit);
}

// NONMATCHING: 99.58% - r5/r6 of the copy of the record at 0x1470 (Vector3 copy from a temporary of an
// inline: __VecCopy family, documented)
void Em005DoppelGanger::vf40(HitInfo_unk* hit) {
    if (m14BC == 0 && m55C == 6 && m560 - 20U <= 3) {
        m58C = 10;
        m14B4 = 0;
        m14B8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
    }
    if (mStatus.IsState_unk(0x20)) {
        if (m14BC == 0) {
            mStatus.m04 = 1.0f;
            mStatus.fn_8013322C(0x20);
        } else {
            vf44();
            m58C = 2;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
            fn_800EB7A0(this);
        }
    } else if (m14BC == 2) {
        int kind = m560;
        if (kind - 20U <= 3 && ++m1520 >= m1524 && m1470 != 14) {
            ((DoppelChara_unk*)this)->SetState_unk(DoppelState_unk(14));
            if (kind != 23 && PSVECMag(&m6B8) > 0.01f) {
                PSVECNormalize(&m6B8, &m6B8);
                m6B8 *= 5.0f;
                mFlag5 = 0;
                fn_8015E4B0(m6B8, -1);
            }
        }
    }
}

void Em005DoppelGanger::vf44() {
    if (m14BC == 0) {
        mStatus.m04 = 1.0f;
        mStatus.fn_8013322C(0x20);
    } else {
        BattleCharaTask::vf44();
    }
}

void Em005DoppelGanger::fn_80252E6C() {
    if (m14C4 == 0) {
        int wait = m1548;
        m14C0--;
        if (wait == 0 && m14C0 < 0) {
            m14C0 = 240;
            m14C4 = 1;
            if (m1518 != 0) {
                m1514 = 0;
                m1518 = 0;
                ResetStep_unk();
                m58C = 6;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.fn_80097C08(0, 0x140, 0, 1, 1, 0.0f, 0.1f);
                mMotion.mTracks[0].m0C = 1.3f;
                return;
            }
            float dist = 1e16f;
            if (mC58.mC4 != 0) {
                atn::Vector3 d = mC58.mC4->mPos - mPos;
                d.y = 0.0f;
                dist = PSVECMag(&d);
            }
            if (dist < 40.0f) {
                ResetStep_unk();
                m58C = 5;
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
                return;
            }
            ResetStep_unk();
            m58C = 9;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x142, 0, 1, 1, 0.0f, 0.1f);
            if (m14FC != 0) {
                fn_80232B7C(this, 100);
                PlayEffectB_unk(&m14E0, &m14FC->mMotion);
                fn_8023A02C(lbl_8087F3C0, this, 100, 16);
                fn_8023A02C(lbl_8087F3C0, this, 100, 8);
            }
            return;
        }
        if (m1540 > 0 && m1544 > 30) {
            ResetStep_unk();
            m58C = 11;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14B, 0, 1, 1, 0.0f, 0.1f);
            m1544 = 0;
            m1540 = m1540 < 60 ? m1540 : 60;
            if (fn_80370A78(lbl_8087F430, 0xE7) == 0) {
                fn_80370AE4(lbl_8087F430, 0xE7, 1);
            }
        }
    } else {
        m14C4 = 0;
        ResetStep_unk();
        m58C = 0;
        fn_8016E970(3);
    }
}

// NONMATCHING: 99.1% - stack slots of the rotation matrix of the route branch against the front of the
// source, and the li r4 of the hide call (10 variants)
void Em005DoppelGanger::fn_802531F4() {
    if (m55C == 6) {
        Move_unk();
        if (m560 == 126) {
            if ((float)__fabs(mMotion.mTracks[0].m08 - 43.0f) < 1.1920929e-7f) {
                EffectMgr_unk* mgr = lbl_8087F3C0;
                PlayEffectM_unk(mgr, &m14C8, &mMotion);
                fn_800C344C("SE_EM005_010_1", &mPos, 0, -1, 1.0f);
            }
            if ((float)__fabs(mMotion.mTracks[0].m08 - 55.0f) < 1.1920929e-7f) {
                if (m152C != 0) {
                    mPos = m152C->mPos;
                }
                m14BC = 2;
                fn_8016E4C4(IsHiddenPhase_unk());
            }
        }
        return;
    }
    fn_8016E970(3);
    CharaTask* src = m1504;
    if (src != 0) {
        mPos = src->mPos;
        mPos.z = 2.0f * sOffsetB_unk.z - mPos.z;
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', src->mRot.y);
        PSMTXMultVec(m, &front, &front);
        float angle = acos(-1.0f * PSVECDotProduct(&front, &sSphereOffset_unk));
        if (front.x < 0.0f) {
            angle *= -1.0f;
        }
        mRot.y = angle;
    }
}

// NONMATCHING: 98% - li r4 / mr r3 of the last call swapped (4 variants)
void Em005DoppelGanger::vfF0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
            if (m1434 == 29) {
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m14D4, &mMotion);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
        }
        return;
    }
    m1434 = 30;
    if ((float)__fabs(mMotion.mTracks[0].m08 - 40.0f) < 1.1920929e-7f) {
        PlayEffect_unk(&m14C8, &mMotion);
        fn_800C344C("SE_EM005_010_1", &mPos, 0, -1, 1.0f);
    }
    if ((float)__fabs(mMotion.mTracks[0].m08 - 50.0f) < 1.1920929e-7f) {
        m14BC = 3;
        fn_8016E4C4(1);
    }
}

void Em005DoppelGanger::fn_80253884() {
    atn::Vector3 rot0 = mRot;
    if (fn_80158BB4()) {
        int kind = m560;
        if (kind == 10) {
            ResetStep_unk();
            m58C = 6;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, 0x140, 0, 1, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.3f;
        } else if (kind - 21U <= 1) {
            m58C = 10;
        } else {
            ResetStep_unk();
            m58C = 0;
            fn_8016E970(3);
        }
    } else {
        Move_unk();
        if (lbl_8087F048 != 0 && fn_80158CA4() != 0) {
            fn_80144710();
            HitInfo_unk* data;
            if (m560 == 10) {
                data = fn_80219E6C(0xE5);
            } else {
                data = fn_80219E6C(0xC8);
            }
            fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
        }
    }
}

// inferred: list of party members (names invented)
struct DoppelList_unk {
    DoppelList_unk() : mCount(0) {}
    unsigned int size() const { return mCount; }
    void push_back(PlayerTask* const& x) {
        if (mCount < 8) {
            new (&mData[mCount]) PlayerTask*(x);
            mCount++;
        }
    }
    unsigned int mCount;
    PlayerTask* mData[8];
};

// NONMATCHING: 99.54% - r0/r3/r5 of the three bools of the inlined CanAct_unk (6 variants of the bool
// helpers)
void Em005DoppelGanger::fn_80253C9C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        DoppelList_unk list;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (p->mType != 0 && p->CanAct_unk()) {
                if (p->m55C == 6) {
                    int c = p->m560;
                    bool busy = false;
                    if (c == 4 || c == 29 || c == 2 || c == 20 || c == 22) {
                        busy = true;
                    }
                    if (busy) {
                        continue;
                    }
                } else if (p->m55C != 2) {
                    continue;
                }
                list.push_back(p);
            }
        }
        if (list.size() != 0) {
            PlayerTask* p = list.mData[rand() % list.size()];
            atn::Vector3 pos = p->mPos;
            atn::Vector3 rot = p->mRot;
            p->mPos = mPos;
            p->mRot = mRot;
            p->fn_801656A4(90);
            mPos = pos;
            mRot = rot;
        }
        m1520 = 0;
        m14B4 = 0;
        m14B8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 0;
        fn_8016E970(3);
    }
}

namespace {

// NONMATCHING: 99.44% - r7/r8 of mChara/the bullet data in the search loop and r26/r27 of two stack
// vectors (6 variants: unsigned/int index, chara local, result order, search inline, temporaries)
int ControlCall::vf0C() {
    int result = 0;
    CharaTask* chara = mChara;
    Bullet_unk* bullet = 0;
    for (int i = 0; i < 32; i++) {
        Bullet_unk* b = lbl_8087F9E8->GetBullet_unk(i);
        if (b->IsActive_unk() && b->m08 == chara && b->m04 != 0 && b->m04->m01 == 5) {
            bullet = b;
            break;
        }
    }
    if (bullet == 0) {
        result = 1;
    }
    if (bullet != 0 && chara->mMotion.mTracks[0].m08 >= 110.0f) {
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            if (e == mChara) {
                continue;
            }
            if (!e->CanAct_unk() || (e->m54C & 0x2000) == 0x2000) {
                continue;
            }
            atn::Vector3 d(e->mPos.x - bullet->m10.x, 0.0f, e->mPos.z - bullet->m10.z);
            if (PSVECMag(&d) > bullet->m5C * bullet->m28) {
                PSVECNormalize(&d, &d);
                DoppelState_unk st(1);
                st.m04 = rand() % 10;
                st.m0C = bullet->m10 + d * (bullet->m5C * bullet->m28) * 0.7f;
                atn::Vector3 from = bullet->m10;
                atn::Vector3 to = st.m0C;
                from.y += 10.0f;
                to.y += 10.0f;
                CollResult_unk res;
                void* colli = lbl_8087EE98;
                if (fn_8004ED34(colli, &res, &from, &to, e->fn_80179D44() | 0x80000000, 0, 0) != 0) {
                    st.m0C = res.m04;
                }
                from = st.m0C;
                to = st.m0C;
                from.y += 10.0f;
                to.y -= 10.0f;
                void* colli2 = lbl_8087EE98;
                if (fn_8004ED34(colli2, &res, &from, &to, e->fn_80179D44() | 0x80000000, 0, 0) != 0) {
                    st.m0C = res.m04;
                }
                from = st.m0C;
                from.y += 5.0f;
                void* world = lbl_8087EE98;
                if (fn_8004D388(world, &res, &from, &atn::Vector3(0.0f, 0.0f, 0.0f), e->fn_80179D44() | 0x80000000, 0,
                                0, 5.0f) != 0) {
                    st.m0C = res.m10;
                    st.m0C.y -= 5.0f;
                }
                ((DoppelChara_unk*)e)->SetState_unk(st);
            } else {
                DoppelState_unk st(1);
                st.m04 = rand() % 10;
                st.m0C = mChara->mPos;
                ((DoppelChara_unk*)e)->SetState_unk(st);
            }
        }
        result = 1;
    }
    mChara->fn_8013CB68(mChara->mRot, 0, 0.0f, 1.0f);
    return result;
}

} // namespace
