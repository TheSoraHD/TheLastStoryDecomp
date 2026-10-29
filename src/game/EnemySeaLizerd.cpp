#include "game/EnemySeaLizerd.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" double sqrt(double x);
// inferred: std::string operator+(const char*, const std::string&) (0x8006AF38)
extern "C" std::string fn_8006AF38(const char* a, const std::string& b);
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" void fn_800E2FE0(BattleCharaTask* chara); // inferred: BattleCharaTask per-frame update
// inferred: bullets (as in gmk/LandMine.cpp)
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" int fn_800F8548(BulletMgr_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg6, int arg7, int arg8,
                            int arg9, float arg10, float arg11);
extern "C" void fn_800F8574(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, const atn::Vector3* pos,
                            const atn::Vector3* vel, int arg5, int arg6, int arg7, float arg8,
                            float arg9); // inferred: throws a bullet
extern "C" void fn_8010A308(BulletMgr_unk* mgr, const atn::Vector3* from, const atn::Vector3* to,
                            float arg); // inferred: splash effect
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
// inferred: game settings (m30: frames per second)
struct Settings_unk {
    unsigned char m00[0x30];
    int m30;
};
extern "C" Settings_unk* lbl_8087F0A8;

EnemySeaLizerd::EnemySeaLizerd(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(120), m14C0(0), m14C4(0), m14C8(2), m14CC(0), m14D0(0),
      m14D4(0), m14D8(0.0f, 0.0f, 0.0f), m14F0(0) {
    std::string path("/database/chara/special/db_em_sea_lizerd.csv");
    const char* name = strstr(info->m2C, "db_name=");
    if (name != 0) {
        path = name + 8;
        path = fn_8006AF38("/database/chara/special/", path);
    }
    m14B0.vf0C(path.c_str());
    fn_80097A88(&mMotion, 0x13F, "em000_apa00a_00.motion");
    fn_80097A88(&mMotion, 0x140, "em000_apa00b_00.motion");
    fn_80097A88(&mMotion, 0x142, "em000_skd03_01.motion");
    mFlagB12 = 1;
}

int EnemySeaLizerd::vf0C() {
    if (m14B0.fn_80473F50() != 0) {
        return 0;
    }
    if (BattleCharaTask::vf0C() != 0) {
        fn_80354910();
        m14B8 = m14BC / 2;
        m14A8 |= 0x01000000;
        m54C |= 0x400;
        mStatus.m18 |= 4;
        return 1;
    }
    return 0;
}

void EnemySeaLizerd::fn_80354910() {
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] != 0 && field[0] != ';') {
            if (strcmp(field, "ThrowWaitTime") == 0) {
                m14BC = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "SwimDamageCount") == 0) {
                m14C8 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "ThrowSkillId") == 0) {
                m14D4 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(field, "AppearSkillId") == 0) {
                m14F0 = atoi(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

// NONMATCHING: 99.9% - r28/r29 swapped between the bullet manager and the bullet data
void EnemySeaLizerd::vf10() {
    if (mFlag2) {
        fn_800E2FE0(this);
        return;
    }
    switch (m58C) {
    case 6: {
        mFlag5 = 0;
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m58C = 0;
            fn_8015495C(0, 1, 1, 1);
            fn_8016EB48(0, 0, 0);
            m570 = 0.0f;
            m574.x = 0.0f;
            m574.y = 0.0f;
            m574.z = 0.0f;
            m6B8.x = 0.0f;
            m6B8.y = 0.0f;
            m6B8.z = 0.0f;
            fn_80161570(0x140, 0, 0, -1.0f, -1.0f);
            HitInfo_unk* bullet = fn_80219E6C(m14F0);
            atn::Vector3 pos(mPos);
            BulletMgr_unk* mgr = lbl_8087F048;
            pos.y += 5.0f;
            fn_800FAB80(mgr, this, bullet, fn_800F8548(mgr), &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        fn_80145334();
        fn_8014C540();
        break;
    }
    case 7:
        fn_80354EE4();
        fn_80145334();
        fn_8014C540();
        break;
    default:
        if (CanAct_unk() && m55C == 2 && mC58.mC0 == 1 && mC58.mC4 != 0) {
            if (m14CC > 0) {
                m14CC--;
            } else if (++m14B8 >= m14BC) {
                fn_80354DA4();
            } else if (m14C4 >= m14C8 && mFlag3) {
                fn_80355390();
            }
        }
        if (m58C < 6) {
            if (m55C == 6 && m560 == 0x3B) {
                m14C4 = m14C8;
                m14CC = 60;
            }
            fn_800E2FE0(this);
        } else {
            fn_80145334();
        }
        break;
    }
}

// inferred: position of a model node (name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

void EnemySeaLizerd::fn_80354DA4() {
    atn::Vector3 head = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    atn::Vector3 target(mC58.mC4->mPos);
    target.y += 11.0f;
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &head, &target, 0x80000006, 0, 0) != 0) {
        m14B8 -= 30;
    } else {
        m58C = 7;
        m14D0 = 0;
        mMotion.fn_80097C08(0, 0x142, 0, 1, 1, 0.0f, 0.2f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        m14C0 = mC58.mC4;
        m14B8 = 0;
    }
}

// inferred: gravity per frame squared (name invented)
inline float Gravity_unk() {
    return -98.1f / (lbl_8087F0A8->m30 * lbl_8087F0A8->m30);
}

// NONMATCHING: 98.4% - store order of the first direction, scheduling of dir.y += 8, f0/f5 at the end
void EnemySeaLizerd::fn_80354EE4() {
    mFlag5 = 0;
    if (m14D0 == 0) {
        atn::Vector3 dir(m14C0->mPos.x - mPos.x, 0.0f, m14C0->mPos.z - mPos.z);
        dir.fn_80139F60();
        float turn = atn::WrapAngle_unk(dir.y - mRot.y);
        turn *= 0.3f;
        mRot.y = atn::WrapAngle_unk(mRot.y + turn);
        if (mMotion.mTracks[0].m08 >= 21.0f) {
            m14D0 = 1;
            HitInfo_unk* bullet = fn_80219E6C(m14D4);
            atn::Vector3 head = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
            CharaTask* target = m14C0;
            dir = target->mPos - head;
            dir.y += 8.0f;
            float dist = sqrt(dir.x * dir.x + dir.z * dir.z);
            float time = dist / bullet->m4C;
            float vy = dir.y / time + -0.5f * time * Gravity_unk();
            dir.y = 0.0f;
            PSVECNormalize(&dir, &dir);
            float speed = bullet->m4C;
            dir.x *= speed;
            dir.y = vy;
            dir.z *= speed;
            dir *= 1.0f / bullet->m4C;
            fn_800F8574(lbl_8087F048, this, bullet, &head, &dir, 0, 0, 0, 0.0f, 1.0f);
        }
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m58C = 0;
        fn_8015495C(0, 1, 1, 1);
        fn_8016EB48(0, 0, 0);
    }
}

// inferred: position one unit in front of a character (name invented)
inline atn::Vector3 GetFrontPos_unk(CharaTask* const& chara) {
    atn::Vector3 offset(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &offset, &offset);
    return chara->mPos + offset;
}

// NONMATCHING: 98.9% - the target character is loaded before the offset is built (r6), f4/f6
void EnemySeaLizerd::fn_80355390() {
    atn::Vector3 from;
    atn::Vector3 to;
    atn::Vector3 dir;
    m58C = 6;
    m14D0 = 0;
    mMotion.fn_80097C08(0, 0x13F, 0, 1, 1, 0.0f, 0.2f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m08 = 6.0f;
    m14C4 = 0;
    m14E4 = mPos;
    m14D8 = GetFrontPos_unk(mC58.mC4);
    from = m14D8;
    void* colli = lbl_8087EE98;
    from.y += 5.0f;
    to = m14D8;
    to.y -= 20.0f;
    CollResult_unk result;
    if (fn_8004ED34(colli, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) != 0) {
        m14D8 = result.m04;
    }
    dir.z = m14D8.z - mPos.z;
    dir.x = m14D8.x - mPos.x;
    dir.y = 0.0f;
    dir.fn_80139F60();
    mRot = dir;
    from = mPos;
    to = mPos;
    from.y += 30.0f;
    fn_8010A308(lbl_8087F048, &from, &to, 5.0f);
}

// inferred: linear interpolation (name invented)
inline atn::Vector3 LerpBA_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

void EnemySeaLizerd::vf28() {
    if (m58C == 6) {
        float frame = mMotion.mTracks[0].m08;
        float t = frame / mMotion.fn_80097D7C(0);
        atn::Model::Node_unk* node = &mMotion.m220[m524];
        atn::Vector3* v = &node->m04;
        mPos = *v + LerpBA_unk(m14E4, m14D8, t);
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
    }
}

void EnemySeaLizerd::vf3C(HitInfo_unk* hit) {
    if (m58C != 6) {
        fn_80151448((HitMsg_unk*)hit);
        vf40(hit);
    }
}

void EnemySeaLizerd::vf40(HitInfo_unk* hit) {
    BattleCharaTask::vf40(hit);
    if (!mFlag2) {
        if (IsFlag3_unk()) {
            m14C4++;
        }
        if (m58C >= 6 && m55C != 2) {
            m58C = 0;
        }
    }
}

bool EnemySeaLizerd::vfC4() {
    if (m58C >= 6) {
        return false;
    }
    return CharaTask::vfC4();
}

int EnemySeaLizerd::vfE4() {
    if (m58C == 7 && mMotion.mTracks[0].m08 > 5.0f && mMotion.mTracks[0].m08 < 21.0f) {
        return 1;
    }
    return BattleCharaTask::vfE4();
}
