#include "game/Em308InvertCrab.h"
#include "atn/Csv.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/function.hpp>
#include "atn/DrawHelper.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4); // inferred: debug flag
// inferred: field (lbl_8087F430); m10D8: route tables (as in game/Em305VampireMp.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);
// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_80097CCC(atn::Model* model, int track, float value); // inferred: stops a track
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* res, int arg2, int arg3, atn::Model* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale); // inferred: plays an effect (as in gmk/PlayEffect.cpp)
// inferred: player manager (lbl_8087F8A0, as in game/CharaTask.cpp)
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
// inferred: game settings (lbl_8087F0A8); frames per second at 0x30 (as in gmk/Cannonball.cpp)
struct CrabSettings_unk {
    unsigned char m00[0x30];
    int mFps;
};
extern "C" CrabSettings_unk* lbl_8087F0A8;
// inferred: ray test result (as in game/Em303KrakenFoot.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: hit position
    unsigned char m1C[0x34 - 0x1C];
    int* m34; // inferred: attribute of the hit
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
// inferred: skill effect manager (lbl_8087F9E8, as in gmk/HealPoint.cpp); fn_8059C6DC tests whether the
// skill is already running for the user; the callback is empty here
struct CrabSkill_unk;
extern "C" void* lbl_8087F9E8;
extern "C" int fn_8059C6DC(void* mgr, HitInfo_unk* skill, CharaTask* user);
extern "C" CrabSkill_unk* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                      const atn::Vector3* pos, const atn::Vector3* rot,
                                      boost::function<void(CrabSkill_unk*)> callback, float rate);
// inferred: parameters of fn_8003EA3C (defined in CharaTask.cpp); result as DamageResult_unk of
// game/Em002Skelton.cpp
struct HitParam_unk;
extern "C" HitParam_unk lbl_807C6B90;
struct CrabDamage_unk {
    CrabDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlags31_28 = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags31_28 : 4;
    unsigned int mFlags27_0 : 28;
};
extern "C" void fn_80040B40(CrabDamage_unk* result, HitInfo_unk* data, CharaStatus* status, CharaStatus* owner,
                            unsigned int flags, const HitParam_unk* param); // inferred: applies the skill
// inferred: tutorial message request (as DropTutorial_unk of gmk/DropItem.cpp)
struct CrabTutorial_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
};
struct CrabMessage_unk {
    unsigned char m000[0x764];
    CrabTutorial_unk m764;
};
extern "C" CrabMessage_unk* lbl_8087F490;
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: attack hit
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, int a, int b, int c, float f1, float f2); // inferred: bullet
extern "C" double sqrt(double x);
extern "C" int fn_8004D314(void* world, int arg, const atn::Vector3* pos, const atn::Vector3* dir, unsigned int mask,
                           void* ignore, int arg7, float radius); // inferred: sphere/ray test (as in gmk/Brazier.cpp)
// inferred: per-player weights of an effect (floats at +0x128, as in game/Em309Ghoul.cpp); names invented
struct CrabWeights_unk {
    unsigned char m00[0x128];
    float mWeight[72]; // inferred: cleared in a loop of 72 (unrolled 8 x 9)
};
extern "C" CrabWeights_unk* fn_80103F60(Effect_unk* mgr, CharaTask* owner);
extern "C" int fn_80108378(CrabWeights_unk* w, CharaTask* chara); // inferred: index of `chara`, < 0 if missing
// inferred: debug drawing (lbl_8087EEB0, as in gmk/Brazier.cpp)
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// NONMATCHING: 99.81% - r26/r27 of the second std::string constructor (known blocker of the string
// constructors, as in Em101LittleZesya/Em305Vampire)
Em308InvertCrab::Em308InvertCrab(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14E4(60), m14E8(70.0f), m14F0(180.0f), m1524(30.0f),
      m152C(180.0f), m1534(0), m1538(0.0f), m153C(0.0f), m1554(0), m157C(0), m1580(0), m1584(0), m158C(0),
      m1590(0), m1594(0), m1598(0), m159C(90), m15A0(90), m15D0(0) {
    std::string dir("database/chara/special/");
    std::string name("db_em308_00_00.csv");
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
    m15C4.vf0C(path.c_str());
    m568 = 0.5f;
    mColli.mFlags &= ~0x80000000;
    m1528 = fn_80219E6C(0x3F8);
    m14EC = fn_80219E6C(0x3F7);
    m1550 = fn_80219E6C(0x400);
    m15A8 = 10;
    m15AC.LoadRequest("/effect/ef_uc001.efp");
    m1518.LoadRequest("/effect/ef_sk002d.efp");
    m14D8.LoadRequest("/effect/ef_uc007.efp");
    m15B8.LoadRequest("/effect/ef_sp008_01.eff");
}

Em308InvertCrab::~Em308InvertCrab() {}

// inferred: once loaded reads the data file: the attack flags, the route of the "BasePoints", the
// parameters of the needle and the escape, the sounds; then the status flags and the debug menu "em308_ParmCrab"
// NONMATCHING: 99.94% - r3/r4 of the inlined std::vector::push_back (known blocker) and the message of
// vector::__allocate (lbl_80775A88, a pool of another file, as in ChaserNpc)
int Em308InvertCrab::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m15AC.IsLoading() == 0 &&
        m1518.IsLoading() == 0 && m14D8.IsLoading() == 0 && m15B8.IsLoading() == 0 &&
        m15C4.fn_80473F50() == 0) {
        if (m15C4.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m15C4.fn_80470580(), m15C4.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "NormalAttack") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m157C |= 2;
                    }
                } else if (strcmp(key, "Needle") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m157C |= 4;
                    }
                } else if (strcmp(key, "Recover") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m157C |= 8;
                    }
                } else if (strcmp(key, "BasePoints") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m1570.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "NeedleChargeTime") == 0) {
                    m14E4 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackDist") == 0) {
                    m1524 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleDist") == 0) {
                    m14E8 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "EscapeCount") == 0) {
                    m1558 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "EscapeSpeed") == 0) {
                    m155C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "GuardTheta") == 0) {
                    m152C = fn_800DC288(csv.fn_8005B9CC());
                    if (atn::Abs_unk(m152C - 180.0f) < 1.1920929e-7f) {
                        m152C = 179.0f;
                    }
                } else if (strcmp(key, "NeedleTheta") == 0) {
                    m14F0 = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "TackleID") == 0) {
                    m1528 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "NeedleID") == 0) {
                    m14EC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BuildUpSkillId") == 0) {
                    m1550 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "NeedleChargeSE") == 0) {
                    sprintf(m14F4, "%s", csv.fn_8005B9CC());
                } else if (strcmp(key, "MoveSE") == 0) {
                    sprintf(m14B4, "%s", csv.fn_8005B9CC());
                } else if (strcmp(key, "EffectCharaScale") == 0) {
                    m1258 = fn_800DC288(csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4218;
        mStatus.m18 |= 0x3800000;
        m159C += rand() % 60;
        mRoute.mA0 = 2;
        void* group = m15CC.Load("em308_ParmCrab");
        fn_80087994(group, "DebugDisp", &m15D0, 0, 0);
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// inferred: plays an effect on the model (as in game/Em305Vampire.cpp)
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

// inferred: gravity per frame (as fn_802F0998 of gmk/Cannonball.cpp)
inline float Gravity_unk(CrabSettings_unk* s) {
    return -98.1f / (float)(s->mFps * s->mFps);
}

// inferred: resets the attack sub-state (name invented)
inline void Em308InvertCrab::ResetAttack_unk() {
    m1590 = 0;
    m15A0 = m14EC != 0 ? m14EC->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
}

// inferred: state machine of the crab
// NONMATCHING: 99.43% - stack slots of the vectors of states 13/17 and of the tutorial request, registers of the
// route table search and of the damage result (~8 forms)
void Em308InvertCrab::vf10() {
    m1580 = (int)mC58.mC4;
    if (m1580 == 0) {
        m1580 = (int)lbl_8087F8A0->mHead;
    }
    if (m1584 == 0) {
        m1584 = m1580;
    }
    if (mC58.mC0 == 0 || m1580 == 0) {
        fn_8016E970(3);
        if (m58C == 2) {
            vfEC();
        } else {
            fn_802F33A0();
        }
    } else {
        switch (m58C) {
        case 7:
            if (fn_80158BB4()) {
                ResetAttack_unk();
                m58C = 6;
                fn_8016E970(3);
            }
            if (mMotion.mTracks[0].m08 >= 12.0f && mMotion.mTracks[0].m08 <= 17.0f) {
                fn_80144710();
                fn_800F8C6C(lbl_8087F048, 0, 0, this, m1528, m590, 30, -1, 0.0f);
            }
            break;
        case 8:
            fn_802F3BE8();
            break;
        case 2:
            vfEC();
            break;
        case 9:
            fn_802F42CC();
            break;
        case 10:
            fn_802F469C();
            break;
        case 11:
            fn_802F4A98();
            break;
        case 12:
            fn_802F5370();
            break;
        case 13:
            if (m1590 == 0) {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    mColli.mFlags |= 1;
                    m1590 = 0;
                    m15A0 = m14EC != 0 ? m14EC->m68 : 90;
                    m590 = fn_800F8548(lbl_8087F048);
                    m58C = 6;
                    fn_8016E970(3);
                } else {
                    atn::Vector3 prev = mPos;
                    mPos += m1540;
                    m1540.y += Gravity_unk(lbl_8087F0A8);
                }
            }
            break;
        case 14:
            if (fn_802F2C50()) {
                if (lbl_8087F430->m10D8->FindPlace_unk(601) != 0) {
                    if (m55C != 7) {
                        m55C = 3;
                        fn_8017039C(601, m15A8);
                    }
                    fn_802F33A0();
                } else {
                    ResetAttack_unk();
                    m58C = 6;
                    fn_8016E970(3);
                }
            } else {
                ResetAttack_unk();
                m58C = 15;
                m574 = lbl_807C7030;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14A, 1, 0, 1, 0.0f, 0.1f);
            }
            break;
        case 15:
            fn_802F566C();
            break;
        case 16: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetAttack_unk();
                m58C = 17;
                m54C &= ~0x2000;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14C, 1, 0, 1, 0.0f, 0.1f);
                m1594 = 1;
                m1598 = 0;
            }
            break;
        }
        case 17:
            if (++m1598 > 600) {
                fn_802F66CC();
            }
            break;
        case 18:
            fn_802F58A0();
            break;
        default:
            if (m55C - 6U > 1) {
                m55C = 3;
                int id = m15A8;
                fn_80170A20((CharaTask*)m1580, id, -1.0f);
            }
            fn_802F33A0();
            fn_802F389C();
            break;
        }
    }
    if ((m157C & 8) == 8 && !mStatus.IsState_unk(0x20) && fn_802F2C50() &&
        fn_8059C6DC(lbl_8087F9E8, m1550, this) == 0) {
        if ((unsigned char)(m1550->m01 - 4) <= 2) {
            fn_8059B66C(lbl_8087F9E8, m1550, this, this, &mPos, &mRot, boost::function<void(CrabSkill_unk*)>(),
                        1.0f);
        } else {
            CrabDamage_unk result;
            fn_80040B40(&result, m1550, &mStatus, &mStatus, 0, &lbl_807C6B90);
        }
    }
    fn_8014C540();
    fn_80145334();
    if (m58C == 17) {
        CharaTask* player = lbl_8087F8A0->mHead;
        if ((mPos - player->mPos).Length_unk() < 40.0f && !player->mFlag6 && !player->mFlag31 &&
            !(player->m55C == 6 && player->m560 != 4)) {
            if (lbl_8087F490 != 0) {
                CrabTutorial_unk tutorial;
                tutorial.m00 = 6;
                tutorial.m04 = 17;
                tutorial.m08 = -1;
                tutorial.m0C = 0;
                tutorial.m10 = 0;
                tutorial.m14 = 0;
                tutorial.m18 = 1;
                lbl_8087F490->m764 = tutorial;
            }
            if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
                player->fn_8016C1E0(this);
                ResetAttack_unk();
                m58C = 18;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 0.5f;
                mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
                fn_800C31F4("SE_UICOM_011", 0, -1, 1.0f);
            }
        }
    }
}

void Em308InvertCrab::vf14() {
    fn_80149A30();
}

// inferred: capsule from the centre down by 0.7 * radius; in state 8 it is moved 10 units forwards
// NONMATCHING: 99.77% - r3/r4 of the __VecCopy of `off` (known blocker); the front vector as inline
// helper with mRot.y in a local fixed the PSMTXRotRad scheduling (~15 forms)
inline atn::Vector3 CrabFront_unk(const atn::Vector3& rot) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = rot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &front, &front);
    return front;
}

void Em308InvertCrab::vf20() {
    atn::Vector3 pos = mPos;
    atn::Vector3 off(0.0f, 0.0f, 0.0f);
    atn::Vector3 bottom;
    if (m58C == 8) {
        off = CrabFront_unk(mRot) * 10.0f;
    }
    pos += off;
    m620 = 0.7f * m5B0;
    m614 = pos;
    bottom = pos;
    m614.y -= 0.7f * m5B0;
    bottom.y = mPos.y - 0.7f * m5B0;
    mColli.m3C = pos;
    mColli.m48 = bottom;
    mColli.m54 = m620;
}

// inferred: the ground under the crab (ray from 5 above to 100 below) has an attribute 0x14, 0x15 or
// 0x1A-0x1D (sand/water?)
// inferred: the original calls it out of line from vf10: empty release asserts keep it over the auto-inline
// threshold
int Em308InvertCrab::fn_802F2C50() {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    int ret = 0;
    atn::Vector3 to;
    atn::Vector3 from = mPos;
    from.y += 5.0f;
    to = mPos;
    to.y -= 100.0f;
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x20000000, 0, 0) != 0 && result.m34 != 0) {
        unsigned int attr = *result.m34;
        if (attr == 0x14 || attr == 0x15 || attr - 0x1A <= 3) {
            ret = 1;
        }
    }
    return ret;
}

// NONMATCHING: 98.80% - r5/r6 of the down test and of the direction temporaries, stack slots of the two
// vectors (~10 forms)
// inferred: the crab only takes knockback while it is down or from behind (guard angle m152C)
void Em308InvertCrab::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->mData->m04 != 0xD1 && m58C - 14U <= 4) {
        msg->m68 = 0;
        msg->m8C = 0;
        msg->m84 = 1;
        msg->m90 = 0;
        return;
    }
    if (!IsDown_unk()) {
        if (m58C == 8) {
            msg->m10 = lbl_807C7030;
        }
        if (!(atn::Abs_unk(m152C) < 1.1920929e-7f) && msg->mData->m04 != 0xD1) {
            atn::Vector3 dir;
            dir = msg->m1C - mPos;
            PSVECNormalize(&dir, &dir);
            if (m58C == 8) {
                dir *= -1.0f;
            }
            if (msg->mData->m01 != 1) {
                atn::Vector3 front(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &front, &front);
                float dot = PSVECDotProduct(&dir, &front);
                if (dot < (float)cos(atn::DegToRad_unk(m152C * 0.5f))) {
                    msg->m48 = 1;
                    msg->m10 = lbl_807C7030;
                }
            }
        }
    }
    if (msg->m48 == 0 && m58C == 8 && m1590 == 1) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        fn_800CB5C8(&m1514, 0, 0);
    }
    fn_80151448(msg);
    vf40(hit);
}

// inferred: dead with the skill 0xD1 (needle): state 17 (motion 0x14C) if m15D0, else fn_802F66CC; with
// another attack: state 14 (motion 0x14)
void Em308InvertCrab::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m1554++;
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 6;
        m1590 = 0;
        m15A0 = m14EC != 0 ? m14EC->m68 : 90;
        m590 = fn_800F8548(lbl_8087F048);
        fn_800CB5C8(&m1514, 15, 0);
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (msg->mData->m04 != 0xD1) {
            mStatus.fn_8013322C(0x20);
            mStatus.m04 = 1.0f;
            m1590 = 0;
            m15A0 = m14EC != 0 ? m14EC->m68 : 90;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 14;
            m54C |= 0x2000;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 0.5f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
        } else if (m15D0 != 0) {
            mStatus.fn_8013322C(0x20);
            mStatus.m04 = 1.0f;
            m1590 = 0;
            m15A0 = m14EC != 0 ? m14EC->m68 : 90;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 17;
            m54C &= ~0x2000;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14C, 1, 0, 1, 0.0f, 0.1f);
            m1594 = 1;
            m1598 = 0;
        } else {
            fn_802F66CC();
        }
    }
}

// inferred: jump to pos + offset (state 13): flight time from the motion length, gravity from the frame rate
// NONMATCHING: 94.64% - stack slots of the destination temporary (0x8 vs 0x20) and f-register
// allocation of the jump velocity; ~13 forms tried.
void Em308InvertCrab::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    m1590 = 0;
    m15A0 = m14EC != 0 ? m14EC->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 13;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x68, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 dest = pos + offset;
    float frames = mMotion.fn_80097D7C(0);
    m1540 = (dest - mPos) * (1.0f / frames);
    m1540.y -= 0.5f * (frames * Gravity_unk(lbl_8087F0A8));
    m159C = 0;
    mColli.mFlags &= ~1;
}

// inferred: movement: along the route while walking (state 7 of m55C), turning to the target m1580
void Em308InvertCrab::fn_802F33A0() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 v = mRoute.m58;
        speed = v.Length_unk();
        if (speed > 0.0001f) {
            rot = v.GetNormalize().ToRotation_unk();
        }
        CharaTask* target = (CharaTask*)m1580;
        v = target->mPos - mPos;
        v.y = 0.0f;
        v.Normalize_unk();
        v.fn_80139F60();
        mRot.y = v.y;
        if (m58C == 12) {
            rate *= m155C;
        }
        fn_801426A4(&rot, speed, m568 * rate);
    } else if (m55C == 6) {
        fn_8013A258();
    } else {
        fn_8013CB68(rot, 0, speed, m568 * rate);
    }
}

// inferred: chooses the next action against the target m1580 (escape, needle, tackle, turn)
void Em308InvertCrab::fn_802F389C() {
    m15A0--;
    if (m1554 >= m1558 && m1570.size() != 0) {
        fn_802F693C((CharaTask*)m1580);
    }
    if ((unsigned int)m1580 != (unsigned int)m1584) {
        fn_8016E970(3);
        m15A0 = m14EC != 0 ? m14EC->m68 : 90;
        if (fn_802F62A0((CharaTask*)m1580, 6) != 0) {
            return;
        }
    }
    CharaTask* target = (CharaTask*)m1580;
    atn::Vector3 d = target->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    if (m15A0 < 0) {
        fn_8016E970(3);
        if (dist <= m14E8) {
            if (fn_802F62A0((CharaTask*)m1580, 8) == 0) {
                fn_802F5F20((CharaTask*)m1580);
            }
        } else {
            m15A0 = m14EC != 0 ? m14EC->m68 : 90;
        }
    } else {
        atn::Vector3 down(0.0f, -3.0f, 0.0f);
        if (fn_8004D314(lbl_8087EE98, 0, &mPos, &down, 0x80000000, 0, 0, 3.0f) != 0) {
            float reach = 30.0f;
            if (m1528 != 0) {
                reach = m1528->m40;
            }
            if (dist < reach) {
                fn_8016E970(3);
                if (fn_802F62A0((CharaTask*)m1580, 11) == 0) {
                    ResetAttack_unk();
                    m58C = 11;
                    fn_80097CCC(&mMotion, 0, 0.0f);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                    m159C = m1528 != 0 ? m1528->m68 : 60;
                    m1584 = m1580;
                }
            } else if (dist < m1524) {
                fn_8016E970(3);
                if (fn_802F62A0((CharaTask*)m1580, 10) == 0) {
                    ResetAttack_unk();
                    m58C = 10;
                    fn_80097CCC(&mMotion, 0, 0.0f);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                    m1584 = m1580;
                }
            }
        }
    }
}

// inferred: rotation z, y, x of the identity (as ChaserRotZYX_unk of game/ChaserNpc.cpp; name invented)
inline void CrabRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// inferred: state 8 (needle): charge effect and sound, wait m14E4 frames, then shoot the needle from the
// node "hips" towards the target (the yaw limited to +-m14F0 / 2)
void Em308InvertCrab::fn_802F3BE8() {
    switch (m1590) {
    case 0: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m1590 = 1;
            fn_80232B7C(this, 0);
            PlayEffect_unk(&m1518, &mMotion);
            m1514 = fn_800C344C(m14F4, &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x143, 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    }
    case 1:
        if (++m15A4 >= m14E4) {
            m1590 = 2;
            m15A4 = 0;
            fn_8016E970(6);
            m560 = 4;
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            fn_800CB5C8(&m1514, 15, 0);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
        }
        break;
    case 2:
        if (fn_80158BB4()) {
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
        } else if (mMotion.mTracks[0].m08 >= 7.0f && mMotion.mTracks[0].m08 < 8.0f) {
            atn::Model* model = &mMotion;
            atn::Vector3 pos = ((atn::Matrix3x4*)model->GetNodeMtx_unk("hips"))->GetTrans_unk();
            atn::Vector3 rot = (((CharaTask*)m1580)->fn_80148B0C(0)->m04.mCenter - pos).ToRotationZ_unk();
            float d = rot.y - mRot.y;
            if (d >= m14F0 / 2.0f) {
                rot.y = m14F0 / 2.0f;
            } else if (d <= -m14F0 / 2.0f) {
                rot.y = -m14F0 / 2.0f;
            }

            atn::Matrix3x4 m;
            CrabRotZYX_unk(m, rot);
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            PSMTXMultVec(m.m, &dir, &dir);
            if (m14EC != 0) {
                fn_800F8574(lbl_8087F048, this, m14EC, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
            }
        }
        break;
    }
}

// inferred: state 9 (turn): between the frames 5 and 16 turns from m1538 to m153C in 12 steps; at the end starts
// the state m1530 against m1588 / m1580 and resets the effect weights to 1
void Em308InvertCrab::fn_802F42CC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        switch (m1530) {
        case 6:
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
            break;
        case 7:
            fn_802F5BCC(m1588);
            break;
        case 8:
            fn_802F5F20(m1588);
            break;
        case 10:
            if (fn_802F62A0(m1588, 10) == 0) {
                ResetAttack_unk();
                m58C = 10;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                m1584 = m1580;
            }
            break;
        case 11:
            if (fn_802F62A0((CharaTask*)m1580, 11) == 0) {
                ResetAttack_unk();
                m58C = 11;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                m159C = m1528 != 0 ? m1528->m68 : 60;
                m1584 = m1580;
            }
            break;
        case 12:
            fn_802F693C((CharaTask*)m1580);
            break;
        }
        CrabWeights_unk* w = fn_80103F60(lbl_8087F048, this);
        for (int j = 0; j < 72; j++) {
            w->mWeight[j] = 1.0f;
        }
    } else {
        float f = mMotion.mTracks[0].m08;
        if (5.0f <= f && f <= 16.0f) {
            m1534 += 2;
            float d = atn::WrapAngle_unk(m153C - m1538);
            mRot.y = atn::WrapAngle_unk(m1538 + (float)m1534 * d / 12.0f);
        }
    }
}

// inferred: state 10 (tackle run): tackles when the target is within the reach of the skill m1528, shoots the
// needle when it gets too far, else keeps chasing it. inferred: the rotation towards the target is computed and
// not used (the original keeps the call, ToRotation_unk has atan2 calls)
// NONMATCHING: 99.22% - only the load of m15A8 for fn_80170A20 is scheduled before the -1.0f (6 forms)
void Em308InvertCrab::fn_802F469C() {
    atn::Vector3 rot;
    CharaTask* target = (CharaTask*)m1580;
    atn::Vector3 d = target->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    rot = d.ToRotation_unk();
    float reach = 30.0f;
    if (m1528 != 0) {
        reach = m1528->m40;
    }
    if (m1554 >= m1558 && m1570.size() != 0) {
        fn_802F693C((CharaTask*)m1580);
    }
    if (dist < reach) {
        fn_8016E970(3);
        if (fn_802F62A0((CharaTask*)m1580, 11) == 0 && fn_802F62A0((CharaTask*)m1580, 11) == 0) {
            ResetAttack_unk();
            m58C = 11;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            m159C = m1528 != 0 ? m1528->m68 : 60;
            m1584 = m1580;
        }
    } else if (dist > m1524) {
        fn_8016E970(3);
        if (fn_802F62A0((CharaTask*)m1580, 8) == 0) {
            fn_802F5F20((CharaTask*)m1580);
        }
    } else {
        if (m55C - 6U > 1) {
            m55C = 3;
            fn_80170A20((CharaTask*)m1580, m15A8, -1.0f);
        }
        fn_802F33A0();
    }
}

// inferred: state 11 (keeps the distance): needle when the target is far, tackle run (state 10) beyond 0.8 of the
// attack distance, walk (fn_802F5BCC) when m159C runs out; else keeps 0.8-1.2 of the tackle reach, backing away
// (m1590 0), standing (1) or closing in (2) while facing the target
// NONMATCHING: 97.05% - stack layout and order of the escape rotation (written as a reference to mRot or to a
// temporary); first form
void Em308InvertCrab::fn_802F4A98() {
    float rate = 1.0f;
    CharaTask* target = (CharaTask*)m1580;
    atn::Vector3 d = target->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    atn::Vector3 rot = d.ToRotation_unk();
    if (m1554 >= m1558 && m1570.size() != 0) {
        fn_802F693C((CharaTask*)m1580);
        return;
    }
    if (dist > m1524) {
        fn_8016E970(3);
        if (fn_802F62A0((CharaTask*)m1580, 8) == 0) {
            fn_802F5F20((CharaTask*)m1580);
        }
    } else if (dist > 0.8f * m1524) {
        fn_8016E970(3);
        if (fn_802F62A0((CharaTask*)m1580, 10) == 0 && fn_802F62A0((CharaTask*)m1580, 10) == 0) {
            ResetAttack_unk();
            m58C = 10;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            m1584 = m1580;
        }
    } else if (m159C < 0) {
        fn_8016E970(3);
        if (fn_802F62A0((CharaTask*)m1580, 7) == 0) {
            fn_802F5BCC((CharaTask*)m1580);
        }
    } else {
        float reach = 30.0f;
        if (m1528 != 0) {
            reach = m1528->m40;
        }
        if (dist >= 0.8f * reach && dist <= 1.2f * reach) {
            m1590 = 1;
        } else if (dist < 0.8f * reach) {
            m1590 = 0;
        } else if (dist > 1.2f * reach) {
            m1590 = 2;
        }
        if (m1590 == 0) {
            dist = reach * (1.0f - dist / reach);
            const atn::Vector3& src = dist <= 0.0f ? mRot : d.GetNormalize().ToRotation_unk();
            rot = src;
            rot.y -= 3.1415927f;
        } else if (m1590 == 1) {
            dist = 0.0f;
        } else if (m1590 == 2) {
            dist *= dist / reach;
        }
        mRot.y = (((CharaTask*)m1580)->mPos - mPos).ToRotation_unk().y;
        fn_801426A4(&rot, dist, m568 * rate);
        m159C--;
    }
}

// inferred: state 12 (escape): runs to the escape point m1564 (route place m1560) and attacks the target when it
// arrives (within 20); debug sphere with m15D0. inferred: the rotation is computed and not used (as fn_802F469C)
void Em308InvertCrab::fn_802F5370() {
    atn::Vector3 rot;
    atn::Vector3 d = m1564 - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    rot = d.ToRotation_unk();
    if (dist < 20.0f) {
        fn_8016E970(3);
        m1554 = 0;
        if (fn_802F62A0((CharaTask*)m1580, 8) == 0) {
            fn_802F5F20((CharaTask*)m1580);
        }
    } else {
        if (m55C - 6U > 1) {
            m55C = 3;
            fn_8017039C(m1560->mId, m15A8);
        }
        fn_802F33A0();
        if (m15D0 != 0) {
            lbl_8087EEB0->fn_80063D3C(m1564, 20.0f, 100000.0f, 0xFFFF0000);
        }
    }
}


// inferred: flight of the jump (state 13): gravity into m574, landing on the ground below (state 16, motion
// 0x14B and the sound SE_EM308_005)
void Em308InvertCrab::fn_802F566C() {
    int landed = 0;
    atn::Vector3 vel = m574;
    vel.y += Gravity_unk(lbl_8087F0A8);
    CollResult_unk result;
    Sphere_unk sphere = fn_80176548(mPos);
    atn::Vector3 next = mPos + vel;
    if (next.y <= sphere.mRadius) {
        landed = 1;
        result.m10 = atn::Vector3(next.x, next.y, next.z);
        result.m10.y = sphere.mRadius;
    }
    if (landed) {
        mPos = result.m10;
        mPos.y += sphere.mRadius;
        m1590 = 0;
        m15A0 = m14EC != 0 ? m14EC->m68 : 90;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 16;
        fn_80097CCC(&mMotion, 0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14B, 0, 0, 1, 0.0f, 0.1f);
        fn_800C344C("SE_EM308_005", &mPos, 0, -1, 1.0f);
    } else {
        mPos += vel;
        m574 = vel;
    }
}

// inferred: end of the death motion (state 17): counts m1434 down, the effect at 29
void Em308InvertCrab::fn_802F58A0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            if (--m1434 == 29) {
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m15AC, &mMotion);
                vf9C(1);
                fn_800EB7A0(this);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
        }
    } else {
        m1434 = 30;
    }
}

void Em308InvertCrab::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            if (--m1434 == 29) {
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m15AC, &mMotion);
                vf9C(1);
                fn_800EB7A0(this);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360();
        }
    } else if (m1594 == 0) {
        mStatus.fn_8013322C(0x20);
        mStatus.m04 = 1.0f;
        m1590 = 0;
        m15A0 = m14EC != 0 ? m14EC->m68 : 90;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 14;
        m54C |= 0x2000;
        fn_80097CCC(&mMotion, 0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 0.5f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    } else {
        m1434 = 30;
    }
}

// inferred: without the flag 2 of m157C attacks the target (fn_802F5F20), with it walks towards it (state 7,
// motion 0x145); both turn first when needed (as Em308ParmCrab::fn_802FB6AC)
// NONMATCHING: 99.96% - stack slots of dir and of the ToRotation_unk temporary swapped (as in ParmCrab)
void Em308InvertCrab::fn_802F5BCC(CharaTask* target) {
    if ((m157C & 2) != 2) {
        if (fn_802F62A0(target, 8) == 0) {
            fn_802F5F20((CharaTask*)m1580);
        }
    } else if (fn_802F62A0(target, 7) == 0) {
        ResetAttack_unk();
        m58C = 7;
        fn_8016E970(6);
        m560 = 4;
        m590 = fn_800F8548(lbl_8087F048);
        atn::Vector3 tpos = target->mPos;
        atn::Vector3 dir = atn::Vector3(tpos - mPos);
        PSVECNormalize(&dir, &dir);
        atn::Vector3 rot = dir.ToRotation_unk();
        mRot = rot;
        mRot.x = 0.0f;
        mRot.z = 0.0f;
        fn_80097CCC(&mMotion, 0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    }
}

// inferred: without the flag 4 of m157C starts state 6, with it shoots the needle (state 8, motion 0x142); both
// turn first when needed (as Em308ParmCrab::fn_802FBA30)
// NONMATCHING: 99.96% - stack slots of dir and of the ToRotation_unk temporary swapped (as in ParmCrab)
void Em308InvertCrab::fn_802F5F20(CharaTask* target) {
    if ((m157C & 4) != 4) {
        if (fn_802F62A0(target, 6) == 0) {
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
        }
    } else {
        if (fn_802F62A0(target, 8) == 0) {
            m58C = 8;
            fn_8016E970(3);
            ResetAttack_unk();
            atn::Vector3 tpos = target->mPos;
            atn::Vector3 dir = atn::Vector3(tpos - mPos);
            PSVECNormalize(&dir, &dir);
            atn::Vector3 rot = dir.ToRotation_unk();
            mRot = rot;
            mRot.x = 0.0f;
            mRot.z = 0.0f;
            m15A4 = 0;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
        }
    }
}

// inferred: starts a turn (state 9, motion 0x14D) towards target unless it is within 22.5 degrees; the state
// `next` follows the turn; the effect weights of the target are cleared (as Em308ParmCrab::fn_802FBE00)
int Em308InvertCrab::fn_802F62A0(CharaTask* target, int next) {
    if (m58C == 9) {
        return 0;
    }
    m1538 = mRot.y;
    atn::Vector3 rot = (target->mPos - mPos).ToRotationZ_unk();
    m153C = rot.y;
    float d = atn::WrapAngle_unk(rot.y - m1538);
    if (-0.3926991f < d && d < 0.3926991f) {
        return 0;
    }
    m1534 = 0;
    m1530 = next;
    m1588 = target;
    CrabWeights_unk* w = fn_80103F60(lbl_8087F048, this);
    if (w != 0) {
        int i = fn_80108378(w, target);
        for (int j = 0; j < 72; j++) {
            if (j != i) {
                w->mWeight[j] = 0.0f;
            }
        }
    }
    ResetAttack_unk();
    m58C = 9;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    return 1;
}

// inferred: death: effect ef_sp008_01 at the node "hit_top" of the player (6 above), or 5 below the crab
// NONMATCHING: 99.86% - r3/r4 of the __VecCopy of the node translation (known blocker); ~8 forms
void Em308InvertCrab::fn_802F66CC() {
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800CB5C8(&m1514, 0, 0);
    m1590 = 0;
    m15A0 = m14EC != 0 ? m14EC->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    CharaTask* player = lbl_8087F8A0->mHead;
    atn::Model* model = (atn::Model*)player->m648->m010;
    MtxPtr node = model->GetNodeMtx_unk("hit_top");
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    if (node != 0) {
        pos = ((atn::Matrix3x4*)model->GetNodeMtx_unk("hit_top"))->GetTrans_unk();
        pos.y += 6.0f;
    } else {
        pos = mPos;
        pos.y -= 5.0f;
    }
    fn_80232B7C(this, 0);
    EffectMgr_unk* mgr = lbl_8087F3C0;
    fn_8023A680(mgr, &m15B8, -1, 0, 0, &pos, &player->mRot, 0, 0, -1, 1, 1.0f);
    m6B8 = lbl_807C7030;
}

// inferred: horizontal length; the vector is taken by value (copy on the stack, as in the original); name
// invented
inline float CrabLengthXZ_unk(atn::Vector3 d) {
    return sqrt(d.x * d.x + d.z * d.z);
}

// inferred: starts the escape (state 12, motion 0x14): picks the base point m1570 farthest from the crab (the
// distance counts double behind it) and runs there; without base points goes back to state 6
void Em308InvertCrab::fn_802F693C(CharaTask* target) {
    if (fn_802F62A0(target, 12) == 0) {
        ResetAttack_unk();
        m58C = 12;
        fn_80097CCC(&mMotion, 0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
        float best = 0.0f;
        m1584 = m1580;
        m1560 = 0;
        int id = -1;
        for (unsigned int i = 0; i < m1570.size(); i++) {
            float dist = CrabLengthXZ_unk(m1570[i]->mPos - mPos);
            atn::Vector3 dir = m1570[i]->mPos - mPos;
            float a = atn::WrapAngle_unk(dir.GetNormalize().ToRotation_unk().y - mRot.y);
            if (a > 1.5707964f || a < -1.5707964f) {
                dist *= 2.0f;
            }
            if (dist > best) {
                best = dist;
                id = m1570[i]->mId;
                m1564 = m1570[i]->mPos;
                m1560 = m1570[i];
            }
        }
        if (m1560 != 0) {
            fn_8017039C(id, m15A8);
        } else {
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
        }
    }
}
