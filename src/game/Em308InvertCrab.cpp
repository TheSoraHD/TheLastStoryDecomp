#include "game/Em308InvertCrab.h"
#include "atn/Csv.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

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
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, Model_unk* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale); // inferred: plays an effect (as in gmk/PlayEffect.cpp)
// inferred: player manager (lbl_8087F8A0, as in game/CharaTask.cpp)
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
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
    m15AC.fn_8023780C("/effect/ef_uc001.efp");
    m1518.fn_8023780C("/effect/ef_sk002d.efp");
    m14D8.fn_8023780C("/effect/ef_uc007.efp");
    m15B8.fn_80237654("/effect/ef_sp008_01.eff");
}

Em308InvertCrab::~Em308InvertCrab() {}

void Em308InvertCrab::vf14() {
    fn_80149A30();
}

// inferred: once loaded reads the data file: the attack flags, the route of the "BasePoints", the
// parameters of the needle and the escape, the sounds; then the status flags and the debug menu "em308_ParmCrab"
// NONMATCHING: 99.94% - r3/r4 of the inlined std::vector::push_back (known blocker) and the message of
// vector::__allocate (lbl_80775A88, a pool of another file, as in ChaserNpc)
int Em308InvertCrab::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m15AC.fn_80237874() == 0 &&
        m1518.fn_80237874() == 0 && m14D8.fn_80237874() == 0 && m15B8.fn_802376D0() == 0 &&
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

// inferred: the ground under the crab (ray from 5 above to 100 below) has an attribute 0x14, 0x15 or
// 0x1A-0x1D (sand/water?)
int Em308InvertCrab::fn_802F2C50() {
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

// inferred: plays an effect on the model (as in game/Em305Vampire.cpp)
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

// inferred: gravity per frame (as fn_802F0998 of gmk/Cannonball.cpp)
inline float Gravity_unk(CrabSettings_unk* s) {
    return -98.1f / (float)(s->mFps * s->mFps);
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

// inferred: end of the death motion; while not down it gets up again (state 14)
// inferred: flight of the jump (state 13): gravity into m574, landing on the ground below (state 16, motion
// 0x14B and the sound SE_EM308_005)
// NONMATCHING: 99.99% - only the offset of "SE_EM308_005" in @stringBase0: "SE_UICOM_011" and "hips" before it
// are used by vf10 and fn_802F3BE8, not written yet; the code itself matches
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
            fn_800EE360(this);
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
    Model_unk* model = (Model_unk*)player->m648->m010;
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
