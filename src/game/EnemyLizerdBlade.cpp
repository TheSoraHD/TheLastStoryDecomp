#include "game/EnemyLizerdBlade.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
// inferred: std::string operator+(const char*, const std::string&) (0x8006AF38)
extern "C" std::string fn_8006AF38(const char* a, const std::string& b);
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion
extern "C" void fn_800E2FE0(BattleCharaTask* chara); // inferred: BattleCharaTask per-frame update
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: bullets (as in gmk/LandMine.cpp)
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" int fn_800F8548(BulletMgr_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg6, int arg7, int arg8,
                            int arg9, float arg10, float arg11);
// inferred: collision world and its ray test (as in game/ChaserNpc.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: game timer (m3A4: frame time)
struct Timer_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" Timer_unk* lbl_8087EFA8;

// inferred: frame time (name invented)
inline float GetFrameTime_unk() {
    return lbl_8087EFA8->m3A4;
}

// inferred: name invented (as in CharaTask.cpp)
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

EnemyLizerdBlade::EnemyLizerdBlade(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(120), m14C4(205), m14C8(0), m14CC(120),
      m14D0(0), m14D8(0) {
    m1420 = 45.0f;
    std::string path("/database/chara/special/db_em_lizerd_blade.csv");
    const char* name = strstr(info->m2C, "db_name=");
    if (name != 0) {
        path = name + 8;
        path = fn_8006AF38("/database/chara/special/", path);
    }
    m14B0.vf0C(path.c_str());
    fn_80097A88(&mMotion, 0x13F, "na000_skd02_00.motion");
    fn_80097A88(&mMotion, 0x142, "na000_skd02a_01.motion");
    fn_80097A88(&mMotion, 0x143, "na000_skd02b_01.motion");
    fn_80097A88(&mMotion, 0x144, "na000_skd02c_01.motion");
    mFlagB4 = 1;
}

int EnemyLizerdBlade::vf0C() {
    if (m14B0.fn_80473F50() != 0) {
        return 0;
    }
    if (BattleCharaTask::vf0C() != 0) {
        fn_803338D8();
        m14A8 |= 0x01000000;
        m54C |= 0x08000000;
        return 1;
    }
    return 0;
}

void EnemyLizerdBlade::fn_803338D8() {
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] != 0 && field[0] != ';') {
            if (strcmp(field, "PressWaitTime") == 0) {
                m14C0 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "PressSkillId") == 0) {
                m14C4 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "TackleWaitTime") == 0) {
                m14CC = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "TackleSkillId") == 0) {
                m14D0 = atoi(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

void EnemyLizerdBlade::vf10() {
    switch (m58C) {
    case 6:
        fn_80334460();
        fn_80145334();
        fn_8014C540();
        break;
    case 7:
        fn_80333D68();
        fn_80145334();
        fn_8014C540();
        break;
    default:
        if (CanAct_unk() && m55C == 2) {
            bool ready = false;
            if (mC58.mC0 != 0 && mC58.m11C.fn_8011D21C() != 0 && mC58.mC4 != 0 && mC58.mC4->mC58.mC0 != 0) {
                ready = true;
            }
            if (ready) {
                m14BC++;
                m14C8++;
                if (m14C4 > 0 && m14BC >= m14C0) {
                    fn_803342EC();
                } else if (m14D0 > 0 && m14C8 >= m14CC) {
                    fn_80333C40();
                }
            }
        }
        if (m58C < 6) {
            fn_800E2FE0(this);
        } else {
            fn_80145334();
            fn_8014C540();
        }
        break;
    }
}

void EnemyLizerdBlade::fn_80333C40() {
    atn::Vector3 from(mPos);
    atn::Vector3 to(mC58.mC4->mPos);
    from.y += 8.0f;
    to.y += 8.0f;
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) != 0) {
        m14C8 -= 30;
    } else {
        m58C = 7;
        m14B8 = 0;
        mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        m14D8 = mC58.mC4;
        m14C8 = 0;
    }
}

// inferred: forward direction of a character (name invented)
inline atn::Vector3 GetFront_unk(CharaTask* c) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', c->mRot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}

void EnemyLizerdBlade::fn_80333D68() {
    if (m55C != 2) {
        m58C = 0;
        return;
    }
    mFlag5 = 0;
    if (m14B8 == 0) {
        atn::Vector3 dir;
        dir.z = m14D8->mPos.z - mPos.z;
        dir.x = m14D8->mPos.x - mPos.x;
        dir.y = 0.0f;
        dir.fn_80139F60();
        float turn = atn::WrapAngle_unk(dir.y - mRot.y);
        turn *= 0.3f;
        mRot.y = atn::WrapAngle_unk(mRot.y + turn);
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.fn_80097C08(0, 0x143, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.5f;
            m14D4 = 0.0f;
            m14B8++;
            m590 = fn_800F8548(lbl_8087F048);
            mC58.m2B0 = 0;
        }
    } else if (m14B8 == 1) {
        m14D4 += GetFrameTime_unk();
        atn::Vector3 move(mPos);
        fn_8013CB68(&mRot, 1, 1.0f, 1.8f);
        atn::Vector3 pos = mPos + GetFront_unk(this) * 14.0f;
        BulletMgr_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, fn_80219E6C(m14D0), m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        move -= mPos;
        move.y = 0.0f;
        if (m570 > 1.0f && LengthSq_unk(move) < 0.2f) {
            mC58.m2B0++;
        } else {
            mC58.m2B0 = 0;
        }
        if (m14D4 > 60.0f || mC58.m2B0 > 2) {
            mC58.m2B0 = 0;
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
            m14B8++;
        }
    } else {
        fn_8013CB68(&mRot, 1, 0.0f, 1.0f);
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m58C = 0;
            fn_8015495C(0, 1, 1, 1);
            fn_8016EB48(0, 0, 0);
        }
    }
}

void EnemyLizerdBlade::fn_803342EC() {
    atn::Vector3 from(mPos);
    atn::Vector3 to(mC58.mC4->mPos);
    from.y += 10.0f;
    to.y += 10.0f;
    if (LengthSq_unk(from - to) > 900.0f) {
        m14BC -= 30;
        return;
    }
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) != 0) {
        m14BC -= 30;
    } else {
        m58C = 6;
        m14B8 = 0;
        mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        m14D8 = mC58.mC4;
        m14BC = 0;
    }
}

// inferred: linear interpolation (name invented)
inline atn::Vector3 LerpBA_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// NONMATCHING: 91.4% - order of the stores of `dir`, scheduling of the copies of m14E8 and of the
// clear of `result` (after fn_80179D44 in the original), float registers of the interpolation
void EnemyLizerdBlade::fn_80334460() {
    if (m55C != 2) {
        m58C = 0;
        return;
    }
    mFlag5 = 0;
    if (m14B8 == 0) {
        if (mMotion.mTracks[0].m08 < 25.0f) {
            atn::Vector3 dir;
            dir.z = m14D8->mPos.z - mPos.z;
            dir.x = m14D8->mPos.x - mPos.x;
            dir.y = 0.0f;
            if (LengthSq_unk(dir) > 0.0001f) {
                dir.Normalize_unk();
            }
            float turn = atn::WrapAngle_unk(dir.ToRotation_unk().y - mRot.y);
            turn *= 0.3f;
            mRot.y = atn::WrapAngle_unk(mRot.y + turn);
            m14DC = mPos;
            m14E8 = m14D8->mPos - dir * 3.0f;
            atn::Vector3 from(m14E8);
            from.y += 5.0f;
            atn::Vector3 to(m14E8);
            to.y -= 5.0f;
            CollResult_unk result;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) != 0) {
                m14E8.y = result.m04.y;
            }
        } else {
            float t = (mMotion.mTracks[0].m08 - 25.0f) / 25.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            mPos = LerpBA_unk(m14DC, m14E8, t);
            if (mMotion.mTracks[0].m08 >= 51.0f) {
                m14B8++;
                BulletMgr_unk* mgr = lbl_8087F048;
                fn_800FAB80(mgr, this, fn_80219E6C(m14C4), fn_800F8548(mgr), &mPos, &mRot, 0, 30, -1, -1, 0.0f,
                            1.0f);
            }
        }
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m58C = 0;
        fn_8015495C(0, 1, 1, 1);
        fn_8016EB48(0, 0, 0);
    }
}

void EnemyLizerdBlade::vf24() {
    if (m58C == 7) {
        return;
    }
    CharaTask::vf24();
}

void EnemyLizerdBlade::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    vf40(hit);
}

void EnemyLizerdBlade::vf40(HitInfo_unk* hit) {
    BattleCharaTask::vf40(hit);
    if (m58C >= 6 && m55C != 2) {
        m58C = 0;
    }
}

bool EnemyLizerdBlade::vfC4() {
    if (m58C >= 6) {
        return false;
    }
    return CharaTask::vfC4();
}
