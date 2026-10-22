#include "game/Em003LesserDaemon.h"
#include "math/Vec.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// NONMATCHING: 99.74% - li order of the per-phase defaults (r0/r4/r5/r6; 5 orders of the loop body) and the
// second std::string registers (same as Em101ElderZesya / Em201ZanGurg)
Em003LesserDaemon::Em003LesserDaemon(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14D0(0),
      m14D4(0), m14D8(0), m161C(10000), m1620(1), m1644(0.0f), m1648(1.0f), m1654(-1), m1658(-1), m1660(35.0f),
      m1664(50.0f), m1668(80.0f), m166C(60.0f), m16C0(1.0f), m16C8(0.0f), m16CC(-1), m16D0(-1), m16D4(-1) {
    for (int i = 0; i < 6; i++) {
        m168C[i] = 60;
        m16A4[i] = 30;
        m1674[i] = 600;
        m1628[i] = 10;
    }
    mPointCount = 0;
    m1624 = 0;
    m1640 = 0;
    m164C = 0;
    m1670 = 0;
    m16BC = 0;
    m16C4 = 0;
    mLinkCount = 0;
    m16E4 = 0;
    m16EC = 0;
    m14DC = 0;
    m14E4 = 0;
    std::string dir("database/chara/special/");
    std::string name("db_em003_00_00.csv");
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
    m1588.vf0C(path.c_str());
    m14EC.fn_8023780C("/effect/ef_cm007.efp");
    m14F8.fn_8023780C("/data/effect/ef_sk047a.efp");
    m1504.fn_8023780C("/data/effect/ef_sk047b.efp");
    m1510.fn_8023780C("/data/effect/ef_sk047d.efp");
    m151C.fn_8023780C("/data/effect/ef_sk047e.efp");
    m1540.fn_8023780C("/data/effect/ef_sk047f.efp");
    m154C.fn_8023780C("/data/effect/ef_sk045a.efp");
    m1558.fn_8023780C("/data/effect/ef_sk046a.efp");
    m1564.fn_8023780C("/data/effect/ef_sk040.efp");
    m1570.fn_8023780C("/data/effect/ef_sk021a.efp");
    m1534.fn_80237654("/effect/ef_bu040g_01.eff");
    m1528.fn_80237654("/effect/ef_bu040h_01.eff");
    m157C.fn_8023780C("/effect/ef_bs007a.efp");
}

Em003LesserDaemon::~Em003LesserDaemon() {}

void Em003LesserDaemon::vf88() {
    fn_8024FCDC();
}

void Em003LesserDaemon::vf14() {
    fn_80149A30();
}

void Em003LesserDaemon::vfEC() {
    if (m16E4 != 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            fn_801765D8();
        }
        return;
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
            if (m1434 == 29) {
                fn_80232B7C(0, 0);
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
                fn_8023A8B4(lbl_8087F3C0, &m14EC, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 30;
    }
}

void Em003LesserDaemon::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_8024FCDC();
        return;
    }
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8, -1);
        case 54:
        reset:
            m58C = 0;
            m14BC += 30;
            fn_8016E970(3);
            m14B8 = 0;
            m14BC = 0;
            m14DC = 0;
            break;
        }
    }
}

void Em003LesserDaemon::fn_8024FCDC() {
    if (m16E4 != 0) {
        fn_8024FE18(1);
        return;
    }
    m58C = 2;
    fn_8016E970(3);
    m14B8 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    fn_80239DAC(lbl_8087F3C0, &m1504, 0, 0);
    fn_800EB7A0(this);
    for (unsigned int i = 0; i < mLinkCount; i++) {
        if (mLinks[i]->m58C != 2) {
            mLinks[i]->fn_8024FE18(0);
        }
    }
    m14DC = 0;
    m14E4 = 0;
}

void Em003LesserDaemon::fn_8024FE18(int arg) {
    mStatus.fn_8012DD70();
    m58C = 2;
    fn_8016E970(3);
    m14B8 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x33, 0, 0, 1, 0.0f, 0.1f);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    fn_80239DAC(lbl_8087F3C0, &m1504, 0, 0);
    if (arg != 0) {
        m590 = fn_800F8548(lbl_8087F048);
        atn::Vector3 pos = mPos;
        pos.y += 10.0f;
        fn_800FAB80(lbl_8087F048, this, m14D8, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    } else {
        fn_80232B7C(0, 0);
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
        fn_8023A8B4(lbl_8087F3C0, &m14EC, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
    }
    m14DC = 0;
    m14E4 = 0;
}

// inferred: index of the warp point farthest from the target (name invented)
inline int Em003LesserDaemon::FarthestPoint_unk() {
    float bestDist = 0.0f;
    int best = 0;
    if (m14B0 != 0) {
        for (unsigned int i = 0; i < mPointCount; i++) {
            atn::Vector3 d = GetPoint_unk(i) - m14B0->mPos;
            float dist = d.Length_unk();
            if (dist > bestDist) {
                best = i;
                bestDist = dist;
            }
        }
        return best;
    }
    return 0;
}

// NONMATCHING: 99.49% - the point pointer and the index of the loop get r28/r30 swapped (12 variants:
// pointer walk, index, while, helper inline, declaration orders)
void Em003LesserDaemon::fn_802506AC() {
    if (mPointCount == 0) {
        m58C = 0;
        fn_8016E970(3);
        m14B8 = 0;
        m14BC = 0;
        m14DC = 0;
        return;
    }
    fn_8016E970(3);
    m58C = 10;
    m14B8 = 0;
    m14BC = 0;
    int best = FarthestPoint_unk();
    for (unsigned int i = 0; i < mLinkCount; i++) {
        if (mLinks[i]->m58C != 2) {
            mLinks[i]->fn_8024FE18(0);
        }
    }
    mWarpDest = mPoints[best];
    m58C = 6;
    fn_8016E970(3);
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
}

// inferred: counts the players and the enemies of the other side inside `radius` (name invented)
int Em003LesserDaemon::fn_80250884(float radius) {
    int count = 0;
    atn::Vector3 pos = mPos;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (!p->mStatus.IsState_unk(0x20)) {
            atn::Vector3 d = p->mPos - pos;
            if (fn_805F9920(&d) < radius * radius) {
                count++;
            }
        }
    }
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if (!e->mStatus.IsState_unk(0x20) && !e->fn_8016F3D0(this, 1)) {
            atn::Vector3 d = e->mPos - pos;
            if (fn_805F9920(&d) < radius * radius) {
                count++;
            }
        }
    }
    return count;
}
