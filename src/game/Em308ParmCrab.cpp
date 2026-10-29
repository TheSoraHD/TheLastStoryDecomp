#include "game/Em308ParmCrab.h"
#include "game/HitInfo_unk.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/function.hpp>

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

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: float color converted to GXColor (as in game/Em305VampireMp.cpp)
struct ColorF_unk {
    ColorF_unk() {}
    ColorF_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
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
extern "C" ColorF_unk lbl_807C8398; // inferred: color of the downed enemies (bss)
// inferred: game settings (lbl_8087F0A8); frames per second at 0x30 (as in game/Em308InvertCrab.cpp)
struct CrabSettings_unk {
    unsigned char m00[0x30];
    int mFps;
};
extern "C" CrabSettings_unk* lbl_8087F0A8;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);
// inferred: player manager (lbl_8087F8A0, as in game/Em308InvertCrab.cpp)
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
// inferred: ray test result (as in game/Em308InvertCrab.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10;
    unsigned char m1C[0x34 - 0x1C];
    int* m34; // inferred: attribute of the hit
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: skill effects (as in game/Em308InvertCrab.cpp)
struct CrabSkill_unk;
extern "C" void* lbl_8087F9E8;
extern "C" CrabSkill_unk* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                      const atn::Vector3* pos, const atn::Vector3* rot,
                                      boost::function<void(CrabSkill_unk*)> callback, float rate);
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
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: attack hit
extern "C" double sqrt(double x);
extern "C" int fn_8004D314(void* world, int arg, const atn::Vector3* pos, const atn::Vector3* dir, unsigned int mask,
                           void* ignore, int arg7, float radius); // inferred: sphere/ray test (as in gmk/Brazier.cpp)
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, int a, int b, int c, float f1, float f2); // inferred: bullet
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_80108F38(Effect_unk* effect, const atn::Vector3& pos, int status); // inferred: status effect

// NONMATCHING: 99.83% - r26/r27 of the second std::string constructor (known blocker, as in Em308InvertCrab)
// inferred: same pattern as Em308InvertCrab's constructor (data file "database/chara/special/" + db_name)
Em308ParmCrab::Em308ParmCrab(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14E8(60), m14EC(15), m14F0(70.0f), m14F8(180.0f),
      m152C(30.0f), m1534(179.0f), m1538(0), m1540(0), m1544(0.0f), m1548(0.0f), m1574(300), m157C(45),
      m1580(45), m1584(25.0f), m1588(50.0f), m158C(90.0f), m1590(0), m1594(0.5f), m15B8(0), m15BC(0),
      m15C8(0), m15CC(0), m15D0(90), m15D4(90), m15DC(0), m15E0(1.0f), m15E4(1.0f), m15E8(1.0f),
      m15EC(1.0f), m15F0(1.0f), m1618(0) {
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
    m160C.vf0C(path.c_str());
    m568 = 0.5f;
    m1530 = fn_80219E6C(0x3F8);
    m14F4 = fn_80219E6C(0x3F7);
    m1578 = fn_80219E6C(0x400);
    m15F4.fn_8023780C("/effect/ef_uc001.efp");
    m1520.fn_8023780C("/effect/ef_sk002d.efp");
    m14DC.fn_8023780C("/effect/ef_uc007.efp");
    m1600.fn_8023780C("/effect/ef_sc003_01b.efp");
}

Em308ParmCrab::~Em308ParmCrab() {}

// inferred: as Em308InvertCrab::vf0C, with the flags Move/RecoverMove/AutoLockon and the recover parameters
// NONMATCHING: 99.95% - r3/r4 of the inlined std::vector::push_back (known blocker) and the message of
// vector::__allocate (lbl_80775A88, a pool of another file)
int Em308ParmCrab::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m15F4.fn_80237874() == 0 &&
        m1520.fn_80237874() == 0 && m14DC.fn_80237874() == 0 && m1600.fn_80237874() == 0 &&
        m160C.fn_80473F50() == 0) {
        if (m160C.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m160C.fn_80470580(), m160C.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "NormalAttack") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m15B8 |= 2;
                    }
                } else if (strcmp(key, "Needle") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m15B8 |= 4;
                    }
                } else if (strcmp(key, "Move") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m15B8 |= 1;
                    }
                } else if (strcmp(key, "Recover") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m15B8 |= 8;
                    }
                } else if (strcmp(key, "RecoverMove") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m15B8 |= 0x10;
                    }
                } else if (strcmp(key, "AutoLockon") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "off") == 0) {
                        mFlagB6 = 1;
                    }
                } else if (strcmp(key, "BasePoints") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m15AC.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "NeedleChargeTime") == 0) {
                    m14E8 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleChargeTime2") == 0) {
                    m14EC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackDist") == 0) {
                    m152C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleDist") == 0) {
                    m14F0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "EscapeHpRate") == 0) {
                    m1594 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "EscapeSpeed") == 0) {
                    m1598 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "GuardTheta") == 0) {
                    m1534 = fn_800DC288(csv.fn_8005B9CC());
                    if (atn::Abs_unk(m1534 - 180.0f) < 1.1920929e-7f) {
                        m1534 = 179.0f;
                    }
                } else if (strcmp(key, "NeedleTheta") == 0) {
                    m14F8 = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "BuildUpTime") == 0) {
                    m1574 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "RecoverInterval") == 0) {
                    m157C = atoi(csv.fn_8005B9CC());
                    m1580 = m157C;
                } else if (strcmp(key, "RecoverRate") == 0) {
                    m1584 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "RecoverStartRate") == 0) {
                    m1588 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "RecoverEndRate") == 0) {
                    m158C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "TackleID") == 0) {
                    m1530 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "NeedleID") == 0) {
                    m14F4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BuildUpSkillId") == 0) {
                    m1578 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "NeedleChargeSE") == 0) {
                    sprintf(m14FC, "%s", csv.fn_8005B9CC());
                } else if (strcmp(key, "MoveSE") == 0) {
                    sprintf(m14B8, "%s", csv.fn_8005B9CC());
                } else if (strcmp(key, "EffectCharaScale") == 0) {
                    m15E0 = fn_800DC288(csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C1;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4010;
        mStatus.m18 |= 0x3000000;
        m15D0 += rand() % 60;
        m1258 = m15E0;
        void* group = m1614.Load("em308_ParmCrab");
        fn_80087994(group, "DebugDisp", &m1618, 0, 0);
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}
// inferred: gravity per frame (as fn_802F0998 of gmk/Cannonball.cpp)
inline float Gravity_unk(CrabSettings_unk* s) {
    return -98.1f / (float)(s->mFps * s->mFps);
}

// inferred: the ground under the crab has an attribute 0x14, 0x15 or 0x1A-0x1D (as
// Em308InvertCrab::fn_802F2C50, inlined here; name invented)
inline int Em308ParmCrab::OnWater_unk() {
    int ret = 0;
    atn::Vector3 to;
    atn::Vector3 from = mPos;
    from.y += 5.0f;
    to = mPos;
    to.y -= 5.0f;
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0 && result.m34 != 0) {
        unsigned int attr = *result.m34;
        if (attr == 0x14 || attr == 0x15 || attr - 0x1A <= 3) {
            ret = 1;
        }
    }
    return ret;
}

// inferred: state machine of the crab (as Em308InvertCrab::vf10)
// NONMATCHING: 98.42% - stack slots of the ray test and of the skill temporaries, scheduling of the chase
// radius min(30, 0.8 * m152C) (~10 forms)
void Em308ParmCrab::vf10() {
    CharaTask* target = mC58.mC4;
    mC58.mC8 = (int)target;
    if (m58C == 9) {
        m1454 = 0;
    } else {
        m1454 = 1;
        m15BC = target;
    }
    if (m15BC == 0) {
        m15BC = lbl_8087F8A0->mHead;
    }
    mC58.mC4 = m15BC;
    m14B0++;
    m1538 = 0;
    if (mC58.mC0 == 0 || m15BC == 0) {
        fn_8016E970(3);
        if (m58C == 2) {
            vfEC();
        } else {
            fn_802F8EBC();
        }
    } else {
        switch (m58C) {
        case 7:
            if (fn_80158BB4()) {
                ResetAttack_unk();
                m58C = 6;
                fn_8016E970(3);
            } else if (mMotion.mTracks[0].m08 >= 12.0f && mMotion.mTracks[0].m08 <= 17.0f) {
                fn_800F8C6C(lbl_8087F048, 0, 0, this, m1530, m590, 30, -1, 0.0f);
            }
            break;
        case 8:
            fn_802F97B8();
            break;
        case 2:
            vfEC();
            break;
        case 9:
            fn_802F9F3C();
            break;
        case 10:
            fn_802FA2F4();
            break;
        case 11:
            fn_802FA764();
            break;
        case 12:
            fn_802FB0C4();
            break;
        case 13:
            if (m15CC == 0) {
                if (++m15D0 >= m15DC) {
                    ResetAttack_unk();
                    m58C = 6;
                    fn_8016E970(3);
                    mColli.mFlags |= 1;
                } else {
                    atn::Vector3 prev = mPos;
                    mPos += m154C;
                    m154C.y += Gravity_unk(lbl_8087F0A8);
                }
            }
            break;
        case 14:
            if (m14B0 >= 450) {
                ResetAttack_unk();
                m58C = 6;
                fn_8016E970(3);
            }
            break;
        default:
            if (m55C - 6U > 1) {
                m55C = 3;
                if (m58C == 12) {
                    fn_8017039C(m1564->mId, 8);
                } else {
                    float r = 0.8f * m152C;
                    fn_80170A20(m15BC, 0, 30.0f < r ? 30.0f : r);
                }
            }
            fn_802F8EBC();
            fn_802F93F0();
            break;
        }
    }
    if ((m15B8 & 8) == 8 && !mStatus.IsState_unk(0x20) && OnWater_unk()) {
        CrabDamage_unk result;
        if ((unsigned char)(m1578->m01 - 4) <= 2) {
            fn_8059B66C(lbl_8087F9E8, m1578, this, this, &mPos, &mRot, boost::function<void(CrabSkill_unk*)>(),
                        1.0f);
        } else {
            fn_80040B40(&result, m1578, &mStatus, &mStatus, 0, &lbl_807C6B90);
        }
    }
    fn_8014C540();
    fn_80145334();
}


// inferred: the colour of "em202_body"/"em202_tail" moves 15% per frame towards white (or the downed colour)
// NONMATCHING: 94.16% - stack order and scheduling of the colour temporaries (target - cur, * 0.15f); ~12 forms
// (field-by-field, operators by reference/value, Lerp helper)
inline ColorF_unk operator-(const ColorF_unk& x, const ColorF_unk& y) {
    return ColorF_unk(x.r - y.r, x.g - y.g, x.b - y.b, x.a - y.a);
}
inline ColorF_unk operator+(const ColorF_unk& x, const ColorF_unk& y) {
    return ColorF_unk(x.r + y.r, x.g + y.g, x.b + y.b, x.a + y.a);
}
inline ColorF_unk operator*(const ColorF_unk& x, float f) {
    return ColorF_unk(x.r * f, x.g * f, x.b * f, x.a * f);
}

void Em308ParmCrab::vf14() {
    if (mMotion.fn_80092954("em202_body") != 0 && mMotion.fn_80092954("em202_tail") != 0) {
        ColorF_unk target;
        target.r = 1.0f;
        target.g = 1.0f;
        target.b = 1.0f;
        target.a = 1.0f;
        if (IsDown_unk()) {
            target = lbl_807C8398;
        }
        ColorF_unk& cur = *(ColorF_unk*)&m15E4;
        ColorF_unk d = target - cur;
        ColorF_unk c = d * 0.15f + cur;
        cur = c;
        mMotion.fn_80092954("em202_body")->mColor = c.ToGX();
        mMotion.fn_80092954("em202_tail")->mColor = cur.ToGX();
    }
    fn_80149A30();
}

// inferred: stops the attack sound and effect 200
inline void Em308ParmCrab::StopAttack_unk() {
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

// inferred: cancels the current attack (wait frames from the skill 0x3F7, sound and effect 200 stopped)
inline void Em308ParmCrab::ResetAttack_unk() {
    m14B0 = 0;
    m15CC = 0;
    m15D4 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    StopAttack_unk();
}

// inferred: capsule as in Em308InvertCrab::vf20 but upwards (0.7 * radius), from 2 units below the centre
// NONMATCHING: 99.78% - r3/r4 of the __VecCopy of `off` (known blocker); the front vector as inline
// helper with mRot.y in a local fixed the PSMTXRotRad scheduling (~15 forms)
inline atn::Vector3 CrabFront_unk(const atn::Vector3& rot) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = rot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &front, &front);
    return front;
}

void Em308ParmCrab::vf20() {
    atn::Vector3 pos = mPos;
    pos.y -= 2.0f;
    atn::Vector3 off(0.0f, 0.0f, 0.0f);
    atn::Vector3 bottom;
    if (m58C == 8) {
        off = CrabFront_unk(mRot) * 10.0f;
    }
    pos += off;
    m620 = 0.7f * m5B0;
    m614 = pos;
    bottom = pos;
    m614.y += 0.7f * m5B0;
    bottom.y = mPos.y + 0.7f * m5B0;
    mColli.m3C = pos;
    mColli.m48 = bottom;
    mColli.m54 = m620;
}

// inferred: as Em308InvertCrab::vf3C: knockback only while down or from behind (guard angle m1534); in
// state 14 (stunned) the knockback is cancelled
// NONMATCHING: 98.84% - r5/r6 of the down test and of the direction temporaries (as Em308InvertCrab::vf3C)
// and the load order of m1534 / 0.5f before cos (~12 forms)
void Em308ParmCrab::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (!IsDown_unk()) {
        if (m58C == 8) {
            msg->m10 = lbl_807C7030;
        }
        if (!(atn::Abs_unk(m1534) < 1.1920929e-7f) && msg->mData->m04 != 0xD1) {
            atn::Vector3 dir;
            atn::Vector3 d = msg->m1C - mPos;
            dir = d;
            PSVECNormalize(&dir, &dir);
            if (m1538 != 0) {
                dir *= -1.0f;
            }
            if (msg->mData->m01 != 1) {
                atn::Vector3 front(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &front, &front);
                float dot = PSVECDotProduct(&dir, &front);
                if (dot > (float)cos(atn::DegToRad_unk(m1534 * 0.5f))) {
                    msg->m48 = 1;
                    msg->m10 = lbl_807C7030;
                }
            }
        }
    }
    if (msg->m48 == 0 && m58C == 8 && m15CC == 1) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        fn_800CB5C8(&m151C, 0, 0);
    }
    if (m58C == 14) {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
}

// inferred: dies when the status 0x20 is set; the stun skill 0x1791 starts state 14 (motion 2)
void Em308ParmCrab::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m1590++;
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 6;
        ResetAttack_unk();
        StopAttack_unk();
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802FC188();
    } else if (msg->mData != 0 && msg->mData->m04 == 0x1791) {
        ResetAttack_unk();
        m58C = 14;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 0.0f;
        mMotion.fn_80097C08(0, 2, 1, 1, 1, 0.0f, 0.1f);
        fn_80232B7C(this, 200);
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
        fn_8023A8B4(lbl_8087F3C0, &m1600, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        Effect_unk* fx = lbl_8087F048;
        fn_80108F38(fx, fn_801781B0(), 0x80);
    }
}


// inferred: jump to pos + offset (state 13), as Em308InvertCrab::vf80; also keeps the flight time in m15DC
// NONMATCHING: 82.04% - stack order of the temporaries (dest at 0x8 in the original) and scheduling of the
// two int->float conversions of the frame count; ~8 forms (inline helper included)
void Em308ParmCrab::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    ResetAttack_unk();
    m58C = 13;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x68, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 dest = pos + offset;
    int frames = (int)mMotion.fn_80097D7C(0);
    m15DC = frames;
    m154C = (dest - mPos) * (1.0f / (float)m15DC);
    m15D0 = 0;
    mColli.mFlags &= ~1;
    m154C.y -= 0.5f * ((float)m15DC * Gravity_unk(lbl_8087F0A8));
}

// inferred: movement (as Em308InvertCrab::fn_802F33A0): slower while 0x8000 is set in the status and scaled by the
// speed rate of the status. inferred: in the route branch `rot` is only written when moving (the original reads
// it uninitialised otherwise; kept as in the binary)
void Em308ParmCrab::fn_802F8EBC() {
    float rate = 1.0f;
    float speed = 0.0f;
    if (mStatus.IsState_unk(0x8000)) {
        rate *= 0.5f;
    }
    rate *= mStatus.fn_8012DB04();
    atn::Vector3 rot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 v = mRoute.m58;
        speed = v.Length_unk();
        if (speed > 0.0001f) {
            rot = v.GetNormalized_unk().ToRotation_unk();
        }
        CharaTask* target = m15BC;
        v = target->mPos - mPos;
        v.y = 0.0f;
        v.Normalize_unk();
        v.fn_80139F60();
        mRot.y = v.y;
        if (m58C == 12) {
            rate *= m1598;
        }
        rate *= mStatus.fn_8012DB04();
        fn_801426A4(&rot, speed, m568 * rate);
    } else if (m55C == 6) {
        fn_8013A258();
    } else {
        rot = mRot;
        fn_8013CB68(&rot, 0, speed, m568 * rate);
    }
}

// inferred: chooses the next action against the target m15BC (as Em308InvertCrab::fn_802F389C): escape under
// "EscapeHpRate", turn, needle, tackle or charge
void Em308ParmCrab::fn_802F93F0() {
    m15D4--;
    if (m15AC.size() != 0 && mStatus.m04 / (float)mStatus.mParamB.m84 < m1594 && m15BC != 0 &&
        !m15BC->mFlag28) {
        fn_802FC2E0(m15BC);
    }
    if ((unsigned int)m15BC != (unsigned int)mC58.mC8) {
        fn_8016E970(3);
        m15D4 = m14F4 != 0 ? m14F4->m68 : 90;
        if (fn_802FBE00(m15BC, 6) != 0) {
            return;
        }
    }
    CharaTask* target = m15BC;
    atn::Vector3 d = target->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    if (m15D4 < 0) {
        fn_8016E970(3);
        if (dist <= m14F0) {
            if (fn_802FBE00(m15BC, 8) == 0) {
                fn_802FBA30(m15BC);
            }
        } else {
            m15D4 = m14F4 != 0 ? m14F4->m68 : 90;
        }
    } else {
        atn::Vector3 down(0.0f, -3.0f, 0.0f);
        if (fn_8004D314(lbl_8087EE98, 0, &mPos, &down, 0x80000000, 0, 0, 3.0f) != 0) {
            float reach = 30.0f;
            if (m1530 != 0) {
                reach = m1530->m40;
            }
            if (dist < reach) {
                fn_8016E970(3);
                if (fn_802FBE00(m15BC, 11) == 0) {
                    ResetAttack_unk();
                    m58C = 11;
                    fn_80097CCC(&mMotion, 0, 0.0f);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                    m15D0 = m1530 != 0 ? m1530->m68 : 60;
                }
            } else if (dist < m152C) {
                fn_8016E970(3);
                if (fn_802FBE00(m15BC, 10) == 0) {
                    ResetAttack_unk();
                    m58C = 10;
                    fn_80097CCC(&mMotion, 0, 0.0f);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                }
            }
        }
    }
}


// inferred: plays an effect on the model (as in game/Em308InvertCrab.cpp)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

// inferred: state 8 (needle, as Em308InvertCrab::fn_802F3BE8): charge effect and sound, wait m14E8 frames, then
// shoots from the node "hips" towards the target between the frames 7 and 8 (repeating 12.5-13.5 while m15D8 <
// m14EC); m1538 is set while charging
void Em308ParmCrab::fn_802F97B8() {
    switch (m15CC) {
    case 0: {
        if (mMotion.mTracks[0].m08 >= 20.0f) {
            m1538 = 1;
        }
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m15CC = 1;
            fn_80232B7C(this, 0);
            PlayEffect_unk(&m1520, &mMotion);
            m151C = fn_800C344C(m14FC, &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x143, 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    }
    case 1:
        if (++m15D8, m1538 = 1, m15D8 >= m14E8) { // inferred: comma form gives the store order
            m15CC = 2;
            m15D8 = 0;
            fn_8016E970(6);
            m560 = 4;
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            fn_800CB5C8(&m151C, 15, 0);
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
        } else {
            float f = mMotion.mTracks[0].m08;
            if (12.5f < f && f < 13.5f) {
                if (++m15D8 < m14EC) {
                    mMotion.mTracks[0].m08 = 12.0f;
                }
            } else if (mMotion.mTracks[0].m08 >= 7.0f && mMotion.mTracks[0].m08 < 8.0f) {
                atn::Vector3 rot = (m15BC->fn_80148B0C(0)->m04.mCenter - mPos).ToRotationZ_unk();
                float d = rot.y - mRot.y;
                if (d >= m14F8 / 2.0f) {
                    rot.y = m14F8 / 2.0f;
                } else if (d <= -m14F8 / 2.0f) {
                    rot.y = -m14F8 / 2.0f;
                }
                atn::Matrix3x4 m;
                CrabRotZYX_unk(m, rot);
                Model_unk* model = &mMotion;
                atn::Vector3 pos = ((atn::Matrix3x4*)model->GetNodeMtx_unk("hips"))->GetTrans_unk();
                atn::Vector3 dir(0.0f, 0.0f, 3.0f);
                PSMTXMultVec(m.m, &dir, &dir);
                if (m14F4 != 0) {
                    fn_800F8574(lbl_8087F048, this, m14F4, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
                }
            }
        }
        if (mMotion.mTracks[0].m08 < 15.0f) {
            m1538 = 1;
        }
        break;
    }
}

// inferred: state 9 (turn, as Em308InvertCrab::fn_802F42CC): turns from m1544 to m1548 in 12 steps between the
// frames 5 and 16; at the end starts the state m153C against m15C4 / m15BC
void Em308ParmCrab::fn_802F9F3C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        switch (m153C) {
        case 6:
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
            break;
        case 7:
            fn_802FB6AC(m15C4);
            break;
        case 8:
            fn_802FBA30(m15C4);
            break;
        case 10:
            if (fn_802FBE00(m15C4, 10) == 0) {
                ResetAttack_unk();
                m58C = 10;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            }
            break;
        case 11:
            if (fn_802FBE00(m15BC, 11) == 0) {
                ResetAttack_unk();
                m58C = 11;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                m15D0 = m1530 != 0 ? m1530->m68 : 60;
            }
            break;
        case 12:
            fn_802FC2E0(m15BC);
            break;
        }
    } else {
        float f = mMotion.mTracks[0].m08;
        if (5.0f <= f && f <= 16.0f) {
            m1540 += 2;
            float d = atn::WrapAngle_unk(m1548 - m1544);
            mRot.y = atn::WrapAngle_unk(m1544 + (float)m1540 * d / 12.0f);
        }
    }
}

// inferred: state 10 (tackle run, as Em308InvertCrab::fn_802F469C): escape under "EscapeHpRate", tackle within
// the reach of m1530, needle beyond m152C, else chase the target. inferred: the rotation is computed and not used
// NONMATCHING: 99.30% - only the li r5, 0 of fn_80170A20 is scheduled before the min(30, 0.8 * m152C) (as vf10;
// 4 forms)
void Em308ParmCrab::fn_802FA2F4() {
    atn::Vector3 rot;
    CharaTask* target = m15BC;
    atn::Vector3 d = target->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    rot = d.ToRotation_unk();
    float reach = 30.0f;
    if (m1530 != 0) {
        reach = m1530->m40;
    }
    if (m15AC.size() != 0 && mStatus.m04 / (float)mStatus.mParamB.m84 < m1594 && m15BC != 0 &&
        !m15BC->mFlag28) {
        fn_802FC2E0(m15BC);
    }
    if (dist < reach) {
        fn_8016E970(3);
        if (fn_802FBE00(m15BC, 11) == 0 && fn_802FBE00(m15BC, 11) == 0) {
            ResetAttack_unk();
            m58C = 11;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            m15D0 = m1530 != 0 ? m1530->m68 : 60;
        }
    } else if (dist > m152C) {
        fn_8016E970(3);
        if (fn_802FBE00(m15BC, 8) == 0) {
            fn_802FBA30(m15BC);
        }
    } else {
        if (m55C - 6U > 1) {
            float r = 0.8f * m152C;
            m55C = 3;
            fn_80170A20(m15BC, 0, 30.0f < r ? 30.0f : r);
        }
        fn_802F8EBC();
    }
}

// inferred: state 11 (keeps the distance, as Em308InvertCrab::fn_802F4A98); slower while 0x2000000 or 0x8000
// is set in the status, scaled by the speed rate of the status
// NONMATCHING: 97.19% - stack layout and order of the escape rotation, as Em308InvertCrab::fn_802F4A98
void Em308ParmCrab::fn_802FA764() {
    float rate = 1.0f;
    CharaTask* target = m15BC;
    atn::Vector3 d = target->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    atn::Vector3 rot = d.ToRotation_unk();
    if (m15AC.size() != 0 && mStatus.m04 / (float)mStatus.mParamB.m84 < m1594 && m15BC != 0 &&
        !m15BC->mFlag28) {
        fn_802FC2E0(m15BC);
    }
    if (dist > m152C) {
        fn_8016E970(3);
        if (fn_802FBE00(m15BC, 8) == 0) {
            fn_802FBA30(m15BC);
        }
    } else if (dist > 0.8f * m152C) {
        fn_8016E970(3);
        if (fn_802FBE00(m15BC, 10) == 0 && fn_802FBE00(m15BC, 10) == 0) {
            ResetAttack_unk();
            m58C = 10;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
        }
    } else if (m15D0 < 0) {
        fn_8016E970(3);
        if (fn_802FBE00(m15BC, 7) == 0) {
            fn_802FB6AC(m15BC);
        }
    } else {
        float reach = 30.0f;
        if (m1530 != 0) {
            reach = m1530->m40;
        }
        if (dist >= 0.8f * reach && dist <= 1.2f * reach) {
            m15CC = 1;
        } else if (dist < 0.8f * reach) {
            m15CC = 0;
        } else if (dist > 1.2f * reach) {
            m15CC = 2;
        }
        if (m15CC == 0) {
            dist = reach * (1.0f - dist / reach);
            const atn::Vector3& src = dist <= 0.0f ? mRot : d.GetNormalized_unk().ToRotation_unk();
            rot = src;
            rot.y -= 3.1415927f;
        } else if (m15CC == 1) {
            dist = 0.0f;
        } else if (m15CC == 2) {
            dist *= dist / reach;
        }
        mRot.y = (m15BC->mPos - mPos).ToRotation_unk().y;
        if (mStatus.IsState_unk(0x2000000) || mStatus.IsState_unk(0x8000)) {
            rate *= 0.5f;
        }
        rate *= mStatus.fn_8012DB04();
        fn_801426A4(&rot, dist, m568 * rate);
        m15D0--;
    }
}

// inferred: state 12 (escape, as Em308InvertCrab::fn_802F5370): runs to the escape point m15A0 (route place m159C)
// and attacks the target when it arrives (within 20). inferred: the rotation is computed and not used
// inferred: the empty release asserts keep it out of line (the original calls it from vf10)
void Em308ParmCrab::fn_802FB0C4() {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    atn::Vector3 rot;
    atn::Vector3 d = m15A0 - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    rot = d.ToRotation_unk();
    if (dist < 20.0f) {
        fn_8016E970(3);
        m1590 = 0;
        if (fn_802FBE00(m15BC, 8) == 0) {
            fn_802FBA30(m15BC);
        }
    } else {
        if (m55C - 6U > 1) {
            m55C = 3;
            fn_8017039C(m159C->mId, 0);
        }
        fn_802F8EBC();
    }
}


// inferred: death motion: at its end counts m1434 down (the effect at 29), else faces the direction m6B8
// NONMATCHING: 98.67% - the original passes &rot to fn_8013CB68 from r31 (the base of the inlined
// ToRotation_unk); ~10 forms (in-place fn_80139F60, RotZ_unk, helper, pointer local)
void Em308ParmCrab::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            if (--m1434 == 29) {
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m15F4, &mMotion);
                vf9C(1);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        atn::Vector3 rot = m6B8.ToRotation_unk();
        fn_8013CB68(&rot, 0, 0.0f, 1.0f);
        m1434 = 30;
    }
}

// inferred: without the flag 2 of m15B8 attacks the current target (fn_802FBA30), with it walks towards
// target (state 7, motion 0x145); both turn first when needed
// NONMATCHING: 99.96% (report) - same dir/rotation stack swap as fn_802FBA30 (helpers tried too)
void Em308ParmCrab::fn_802FB6AC(CharaTask* target) {
    if ((m15B8 & 2) != 2) {
        if (fn_802FBE00(target, 8) == 0) {
            fn_802FBA30(m15BC);
        }
    } else if (fn_802FBE00(target, 7) == 0) {
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
        mMotion.mTracks[0].m0C = fn_8016E864();
        mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    }
}

// inferred: without the flag 4 of m15B8 starts state 6, with it charges towards target (state 8, motion 0x142);
// both turn first when needed
// NONMATCHING: 99.96% (report) - stack slots of dir (0x5c) and of the ToRotation_unk temporary (0x68)
// swapped; ~10 forms
void Em308ParmCrab::fn_802FBA30(CharaTask* target) {
    if ((m15B8 & 4) != 4) {
        if (fn_802FBE00(target, 6) == 0) {
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
        }
    } else {
        if (fn_802FBE00(target, 8) == 0) {
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
            m15D8 = 0;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
        }
    }
}

// inferred: starts a turn (state 9, motion 0x14D) towards target, unless it is within 22.5 degrees; the
// state `next` follows the turn
int Em308ParmCrab::fn_802FBE00(CharaTask* target, int next) {
    if (m58C == 9) {
        return 0;
    }
    m1544 = mRot.y;
    atn::Vector3 rot = (target->mPos - mPos).ToRotationZ_unk();
    m1548 = rot.y;
    float d = atn::WrapAngle_unk(rot.y - m1544);
    if (-0.3926991f < d && d < 0.3926991f) {
        return 0;
    }
    m1540 = 0;
    m153C = next;
    m15C4 = target;
    ResetAttack_unk();
    m58C = 9;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    return 1;
}

void Em308ParmCrab::fn_802FC188() {
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800CB5C8(&m151C, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    ResetAttack_unk();
    m58C = 2;
    fn_8016E970(9);
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0(this);
}

// inferred: horizontal length; the vector is taken by value (copy on the stack, as in the original); name
// invented
inline float CrabLengthXZ_unk(atn::Vector3 d) {
    return sqrt(d.x * d.x + d.z * d.z);
}

// inferred: starts the escape (state 12, motion 0x14): picks the base point m15AC farthest from the crab (the
// distance counts double behind it) and runs there; without base points goes back to state 6 (as
// Em308InvertCrab::fn_802F693C)
void Em308ParmCrab::fn_802FC2E0(CharaTask* target) {
    if (fn_802FBE00(target, 12) == 0) {
        ResetAttack_unk();
        m58C = 12;
        fn_80097CCC(&mMotion, 0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
        float best = 0.0f;
        m159C = 0;
        int id = -1;
        for (unsigned int i = 0; i < m15AC.size(); i++) {
            float dist = CrabLengthXZ_unk(m15AC[i]->mPos - mPos);
            atn::Vector3 dir = m15AC[i]->mPos - mPos;
            float a = atn::WrapAngle_unk(dir.GetNormalized_unk().ToRotation_unk().y - mRot.y);
            if (a > 1.5707964f || a < -1.5707964f) {
                dist *= 2.0f;
            }
            if (dist > best) {
                best = dist;
                id = m15AC[i]->mId;
                m15A0 = m15AC[i]->mPos;
                m159C = m15AC[i];
            }
        }
        if (m159C != 0) {
            fn_8017039C(id, 8);
        } else {
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
        }
    }
}

float Em308ParmCrab::vf90() {
    return 3.5f;
}
