#include "game/Em003LesserDaemon.h"
#include "chr/ControlChant.h"
#include "math/Vec.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "game/HomingBullet.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"
#include "macros.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
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

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: collision world and its tests (see game/ChaserNpc.cpp and chr/ControlJumpAt.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep (as in gmk/Lemon.h)
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
struct Effect_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em101LittleZesyaSecond.cpp)
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int rand();
// inferred: fields of the running magic control (class unknown; names invented)
struct ControlMagic_unk {
    unsigned char m00[0x14];
    CharaTask* m14; // inferred: target
    unsigned char m18[0x1D - 0x18];
    unsigned char m1D; // inferred: nonzero once the bullets are fired
};
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: moves a bullet with the character
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" int fn_8004D388(void* world, CollResult_unk* result, const atn::Vector3* center, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length

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
// inferred: starts an effect resource at a position (argument layout of this unit; see game/Em301LShade.cpp)
extern "C" void fn_8023A680(EffectMgr_unk* mgr, void* res, int a, int b, void* mtx, const atn::Vector3* pos,
                            const atn::Vector3* rot, const float* color, float scale, int e, int f, int g);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" int fn_80370174(Field_unk* field, int id); // inferred: global event flag
// inferred: AI weights by enemy (fn_80103F60, as in game/Em403WarLion.cpp)
struct DaemonAi_unk {
    unsigned char m000[0x128];
    float m128[1];
};
extern "C" DaemonAi_unk* fn_80103F60(Effect_unk* mgr, CharaTask* chara);
extern "C" int fn_80108378(DaemonAi_unk* ai, CharaTask* chara); // inferred: index of `chara`, < 0 if missing
// inferred: arena/UI (lbl_8087F490); fn_803E6850 shows a counter (as in game/Em305VampireMp.cpp)
struct MpArena_unk;
extern "C" MpArena_unk* lbl_8087F490;
extern "C" void fn_803E6850(MpArena_unk* arena, CharaTask* chara, int count, int max, int arg);
extern "C" double fmod(double x, double y);
extern "C" double sin(double x);
// inferred: frame time (lbl_8087EFA8, as FrameTime_unk in game/CharaStatus.cpp)
struct FrameTime_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
    float GetTime_unk() const { return m3A4; } // inferred: accessor (name invented)
};
extern "C" FrameTime_unk* lbl_8087EFA8;
// inferred: material colors of a model (returned by fn_80094B6C; class unknown, names invented)
struct MatColor_unk {
    unsigned char m00[0x1C];
    GXColor m1C;
    GXColor m20;
};
extern "C" MatColor_unk* fn_80094B6C(chr::Motion_unk* model, const char* name); // inferred: material by name
// inferred: color as four floats (as ColorF_unk in game/Em315Coffin.cpp)
struct ColorF_unk {
    ColorF_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    ColorF_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
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
    mPoints.mSize = 0;
    m1624 = 0;
    m1640 = 0;
    m164C = 0;
    m1670 = 0;
    m16BC = 0;
    m16C4 = 0;
    mLinks.mSize = 0;
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
    m14EC.LoadRequest("/effect/ef_cm007.efp");
    m14F8.LoadRequest("/data/effect/ef_sk047a.efp");
    m1504.LoadRequest("/data/effect/ef_sk047b.efp");
    m1510.LoadRequest("/data/effect/ef_sk047d.efp");
    m151C.LoadRequest("/data/effect/ef_sk047e.efp");
    m1540.LoadRequest("/data/effect/ef_sk047f.efp");
    m154C.LoadRequest("/data/effect/ef_sk045a.efp");
    m1558.LoadRequest("/data/effect/ef_sk046a.efp");
    m1564.LoadRequest("/data/effect/ef_sk040.efp");
    m1570.LoadRequest("/data/effect/ef_sk021a.efp");
    m1534.LoadRequest("/effect/ef_bu040g_01.eff");
    m1528.LoadRequest("/effect/ef_bu040h_01.eff");
    m157C.LoadRequest("/effect/ef_bs007a.efp");
}

Em003LesserDaemon::~Em003LesserDaemon() {}

int Em003LesserDaemon::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1588.fn_80473F50() == 0 &&
        m14EC.IsLoading() == 0 && m14F8.IsLoading() == 0 && m1504.IsLoading() == 0 &&
        m1510.IsLoading() == 0 && m151C.IsLoading() == 0 && m1540.IsLoading() == 0 &&
        m154C.IsLoading() == 0 && m1558.IsLoading() == 0 && m1564.IsLoading() == 0 &&
        m1570.IsLoading() == 0 && m1534.IsLoading() == 0 && m1528.IsLoading() == 0 &&
        m157C.IsLoading() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x820D;
        mStatus.m18 |= 0x3800000;
        RouteTable_unk* table = lbl_8087F430->m10D8;
        if (m1588.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m1588.fn_80470580(), m1588.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "NormalAttackID") == 0) {
                    m14C8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "HomingLaserID") == 0) {
                    m14D0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "PenetrateLaserID") == 0) {
                    m14D4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "DeadExplodeSkillID") == 0) {
                    m14D8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "DashAttackID") == 0) {
                    m14CC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "EnableBarrier") == 0) {
                    m1620 = atoi(csv.fn_8005B9CC());
                    if (m1620 != 0) {
                        m1648 = 1.0f;
                    } else {
                        m1648 = 0.0f;
                    }
                } else if (strcmp(key, "BarrierHitCount") == 0) {
                    int i = atoi(csv.fn_8005B9CC()) * 2 - 1;
                    int count = atoi(csv.fn_8005B9CC());
                    m1628[i - 1] = count;
                    m1628[i] = count;
                } else if (strcmp(key, "AttackRange") == 0) {
                    m1660 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackCount") == 0) {
                    m161C = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "ApproachRange") == 0) {
                    m1664 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "ArrowRange") == 0) {
                    m1668 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "DashRange") == 0) {
                    m166C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "MoveSpeed") == 0) {
                    m568 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "Boss") == 0) {
                    mFlag22 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "WaitCount") == 0) {
                    int i = atoi(csv.fn_8005B9CC()) * 2 - 1;
                    int count = atoi(csv.fn_8005B9CC());
                    m168C[i - 1] = count;
                    m168C[i] = count;
                } else if (strcmp(key, "AngryWaitCount") == 0) {
                    int i = atoi(csv.fn_8005B9CC()) * 2 - 1;
                    int count = atoi(csv.fn_8005B9CC());
                    m16A4[i - 1] = count;
                    m16A4[i] = count;
                } else if (strcmp(key, "AngryCount") == 0) {
                    int i = atoi(csv.fn_8005B9CC()) * 2 - 1;
                    int count = atoi(csv.fn_8005B9CC());
                    m1674[i - 1] = count;
                    m1674[i] = count;
                } else if (strcmp(key, "PiyoBarrierBreak") == 0) {
                    m16BC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "PlayerHateRatio") == 0) {
                    m16C0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "WarpPointID") == 0) {
                    while (true) {
                        int id = atoi(csv.fn_8005B9CC());
                        if (id <= 0) {
                            break;
                        }
                        RoutePlace_unk* place = table->FindPlace_unk(id);
                        if (place != 0) {
                            mPoints.push_back(place->mPos);
                        }
                    }
                } else if (strcmp(key, "EventEndGlobalFlag") == 0) {
                    m16CC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "BarrierLocalFlag") == 0) {
                    m16D0 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "BarrierBreakFlag") == 0) {
                    m16D4 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "Children") == 0) {
                    for (unsigned int i = 0; i < 2; i++) {
                        Em003LesserDaemon* child =
                            (Em003LesserDaemon*)EnemyManager::sInstance->Find_unk(atoi(csv.fn_8005B9CC()));
                        if (child != 0) {
                            child->m16E4 = 1;
                            mLinks.push_back(child);
                        }
                    }
                }
            } while (csv.fn_8005BBF8());
        }
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// inferred: plays an effect on the model (locals in the order of vf88; name invented)
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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
inline void PlayEffectA_unk(atn::EfpFile* efp, atn::Model* model) {
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

// inferred: same as PlayEffect_unk with the manager read first (name invented)
inline void PlayEffectM_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: same as PlayEffectA_unk with the manager read first (name invented)
inline void PlayEffectAM_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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

// inferred: plays an effect at a position (name invented)
inline void PlayEffectAt_unk(atn::EfpFile* efp, const atn::Vector3& pos, const atn::Vector3& rot) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: starts the warp motion (name invented)
inline void Em003LesserDaemon::StartWarp_unk() {
    m58C = 6;
    fn_8016E970(3);
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
}

// inferred: index of the warp point farthest from this daemon (name invented)
inline int Em003LesserDaemon::FarthestFromSelf_unk() {
    float bestDist = 0.0f;
    int best = -1;
    for (unsigned int i = 0; i < mPoints.mSize; i++) {
        atn::Vector3 d = mPos - GetPoint_unk(i);
        float dist = PSVECSquareMag(&d);
        if (dist > bestDist) {
            best = i;
            bestDist = dist;
        }
    }
    return best;
}

// inferred: same search with the index declared first (the register order of fn_8024D120)
inline int Em003LesserDaemon::FarthestFromSelfB_unk() {
    unsigned int i;
    float bestDist = 0.0f;
    int best = -1;
    for (i = 0; i < mPoints.mSize; i++) {
        atn::Vector3 d = mPos - GetPoint_unk(i);
        float dist = PSVECSquareMag(&d);
        if (dist > bestDist) {
            best = i;
            bestDist = dist;
        }
    }
    return best;
}

// inferred: starts the normal attack (state 1; as case 1 of fn_8024E1C0, name invented)
inline void Em003LesserDaemon::StartAttack_unk() {
    m58C = 1;
    fn_8016E970(3);
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(0, 0);
    PlayEffectA_unk(&m1564, &mMotion);
    m14C4++;
}

// inferred: starts state 8 (as case 8 of fn_8024E1C0, name invented)
inline void Em003LesserDaemon::StartLaser_unk() {
    m58C = 8;
    fn_8016E970(3);
    m14B8 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(0, 0);
    PlayEffectA_unk(&m154C, &mMotion);
    m164C = m1624 == 0 ? 3 : 1;
    m1650 = -1;
}

// inferred: starts the homing magic (state 7; as case 7 of fn_8024E1C0, name invented)
inline void Em003LesserDaemon::StartMagic_unk() {
    m58C = 7;
    fn_8016E970(3);
    m14B8 = 0;
    fn_8015F568(mC58.mC4->mPos, mC58.mC4, m14D0);
    fn_80232B7C(this, 0);
    PlayEffectA_unk(&m1558, &mMotion);
}

// inferred: warps to the point farthest from itself (as case 13 of fn_8024E1C0, name invented)
inline void Em003LesserDaemon::WarpFar_unk() {
    int best = FarthestFromSelfB_unk();
    if (best >= 0 && best < (int)mPoints.mSize) {
        mWarpDest = mPoints.mData[best];
        StartWarp_unk();
    }
    m14C4 = 0;
}

// inferred: index of the warp point farthest from the target (name invented)
inline int Em003LesserDaemon::FarthestPoint_unk() {
    float bestDist = 0.0f;
    int best = 0;
    if (m14B0 != 0) {
        for (unsigned int i = 0; i < mPoints.mSize; i++) {
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

// inferred: the battle AI ignores the other enemies (name invented)
inline void ClearHate_unk(CharaTask* self) {
    DaemonAi_unk* ai = fn_80103F60(lbl_8087F048, self);
    if (ai != 0) {
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            int index = fn_80108378(ai, e);
            if (index >= 0) {
                ai->m128[index] = 0.0f;
            }
        }
    }
}

// inferred: shows the barrier hits left (name invented)
// (the original reads m1624 again after the test: `*(volatile int*)&`, as in the notes of the q5 queue)
inline void Em003LesserDaemon::ShowHits_unk() {
    fn_803E6850(lbl_8087F490, this, *(volatile int*)&m1624, m1628[m14C0], 0);
}

// inferred: turns towards a position (name invented)
inline void Em003LesserDaemon::TurnTo_unk(const atn::Vector3& pos) {
    atn::Vector3 d = pos - mPos;
    float t = 0.3f * atn::WrapAngle_unk(d.ToRotationZ_unk().y - mRot.y);
    mRot.y += t;
}

// inferred: back to the default state (name invented)
inline void Em003LesserDaemon::ResetAction_unk() {
    m58C = 0;
    fn_8016E970(3);
    m14B8 = 0;
    m14BC = 0;
    m14DC = 0;
}

// NONMATCHING: 99.72% - stack slots of the two effect locals and of the dash vectors (state 9), and f9/f10
// of the dash factor (8 variants); the pool offsets depend on the unwritten functions
void Em003LesserDaemon::vf10() {
    ClearHate_unk(this);
    m14B4 = 0;
    mC58.mC8 = (int)mC58.mC4;
    if (m14DC != 0) {
        m1454 = 0;
        m14B0 = m14DC;
    } else {
        m1454 = 1;
        m14B0 = mC58.mC4;
    }
    if (m14B0 == 0) {
        m14B0 = lbl_8087F8A0->mHead;
    }
    mC58.mC4 = m14B0;
    if ((CharaTask*)mC58.mC8 != m14B0) {
        m14B4 = 1;
    }
    if (m16C4 == 0 && m16CC >= 0 && fn_80370174(lbl_8087F430, m16CC) == 1) {
        m16C4 = 1;
        if (mStatus.IsState_unk(0x20)) {
            fn_8024FCDC();
            fn_8016E970(3);
        } else {
            if (m1624 != 0) {
                fn_80239DAC(lbl_8087F3C0, &m1504, 0, 0);
                fn_80232B7C(&m1504, 0);
                PlayEffectAM_unk(lbl_8087F3C0, &m1504, &mMotion);
            }
            fn_8016DA4C();
            ResetAction_unk();
            fn_8016E970(3);
        }
    }
    if (mC58.mC0 == 0) {
        if (m58C == 0 || m58C == 2) {
            goto update;
        }
        return;
    }
    if (m1624 != 0) {
        ShowHits_unk();
    }
    if (m55C == 2) {
        fn_8016E970(3);
    }
    bool broken = m1624 == 0;
    m14BC++;
    if (broken) {
        m1670++;
    } else {
        m1670 = 0;
    }
    switch (m58C) {
    case 1: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 58.0f && frame <= 68.0f) {
            fn_800F8C6C(lbl_8087F048, 0, 0, this, m14C8, m590, 30, -1,
                        atn::WrapAngle_unk(6.2831855f * ((frame - 58.0f) / 10.0f) - 0.47123894f));
        }
        float frame2 = mMotion.mTracks[0].m08;
        if (frame2 >= mMotion.fn_80097D7C(0)) {
            ResetAction_unk();
        }
        break;
    }
    case 2:
        vfEC();
        break;
    case 6:
        fn_8024E7E4();
        break;
    case 7:
        fn_8024ED68();
        break;
    case 8:
        fn_8024F1DC();
        break;
    case 9: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            fn_80232B7C(&m1504, 0);
            PlayEffectM_unk(lbl_8087F3C0, &m1504, &mMotion);
            ResetAction_unk();
            if (mFlag22 == 1 && m1640 >= 2 && m16D0 >= 0 && fn_80370A78(lbl_8087F430, m16D0) == 0) {
                fn_80370AE4(lbl_8087F430, m16D0, 1);
            }
            fn_800C344C("SE_SP007_001", &mPos, 0, -1, 1.0f);
        }
        m160C = MIN(1.0f, MAX(0.0f, (mMotion.mTracks[0].m08 - 16.0f) / 12.0f));
        mPos = (mWarpDest - m1600) * m160C + m1600;
        break;
    }
    case 10:
        if (m55C != 6) {
            if (m55C == 7) {
                if ((mWarpDest - mPos).Length_unk() < 20.0f || m14BC > 600) {
                    fn_8016E970(3);
                    ResetAction_unk();
                }
            } else {
                ResetAction_unk();
            }
        }
        mFlag4 = 1;
        fn_80139560();
        break;
    case 12:
        if (m55C - 6U > 1) {
            fn_8016E970(3);
            fn_80170A20(m14B0, 0, -1.0f);
        }
        mFlag4 = 1;
        fn_80139560();
        break;
    default:
        if (m55C - 6U > 1 && m14B0 != 0) {
            TurnTo_unk(m14B0->mPos);
        }
        mFlag4 = 1;
        fn_80139560();
        fn_8024D120();
        break;
    }
update:
    fn_8014C540();
    fn_80145334();
    fn_8024BE94();
}

// NONMATCHING: 99.89% - f2/f3 of the sine and the frame in the death light (8 variants of the expression)
void Em003LesserDaemon::fn_8024BE94() {
    static const char* const sMaterials[6] = {"em003_eye",   "em003_skin01", "em003_skin02",
                                              "em003_skin03", "em003_wep01",  "em003_wep02"};
    static const char* const sLight = "em003_light";
    static const char* const sWepLight = "em003_wep_light";
    if (mC58.mC0 == 0) {
        return;
    }
    if (m1624 == 0 && m58C != 12) {
        m1648 = MAX(0.1f, m1648 - 0.05f * lbl_8087EFA8->GetTime_unk());
    } else {
        m1648 = MIN(1.0f, m1648 + 0.05f * lbl_8087EFA8->GetTime_unk());
    }
    for (unsigned int i = 0; i < 6; i++) {
        MatColor_unk* mat = fn_80094B6C(&mMotion, sMaterials[i]);
        ColorF_unk color(m1648, m1648, m1648, 1.0f);
        mMotion.fn_80094958(sMaterials[i], &color, &ColorF_unk(mat->m20), &ColorF_unk(mat->m1C));
    }
    float light;
    if (m58C == 2) {
        light = 0.5f * (float)sin(atn::DegToRad_unk(m1644)) + 0.5f;
        light *= 1.0f - MIN(1.0f, mMotion.mTracks[0].m08 / 20.0f);
    } else if (m58C == 12) {
        m1644 += 20.0f * lbl_8087EFA8->GetTime_unk();
        if (m1644 >= 360.0f) {
            m1644 -= 360.0f;
        }
        light = 0.5f * (float)sin(atn::DegToRad_unk(m1644)) + 0.5f;
    } else {
        m1644 += 4.0f * lbl_8087EFA8->GetTime_unk();
        if (m1644 >= 360.0f) {
            m1644 -= 360.0f;
        }
        light = 0.5f * (float)sin(atn::DegToRad_unk(m1644)) + 0.5f;
    }
    {
        const char* name = sLight;
        MatColor_unk* mat = fn_80094B6C(&mMotion, name);
        ColorF_unk color(light, light, light, 1.0f);
        mMotion.fn_80094958(name, &color, &ColorF_unk(mat->m20), &ColorF_unk(mat->m1C));
    }
    if (m58C == 1 || m58C == 12) {
        m16C8 += 0.02f;
    } else {
        m16C8 -= 0.02f;
    }
    m16C8 = MAX(0.0f, MIN(1.0f, m16C8));
    {
        const char* name = sWepLight;
        MatColor_unk* mat = fn_80094B6C(&mMotion, name);
        ColorF_unk color(m16C8, m16C8, m16C8, 1.0f);
        mMotion.fn_80094958(name, &color, &ColorF_unk(mat->m20), &ColorF_unk(mat->m1C));
    }
}

void Em003LesserDaemon::vf88() {
    fn_8024FCDC();
}

void Em003LesserDaemon::vf14() {
    fn_80149A30();
}

void Em003LesserDaemon::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 12) {
        return;
    }
    if (m1624 != 0) {
        CharaTask* attacker = msg->mAttacker;
        m14E4 = attacker;
        atn::Vector3 d = attacker->mPos - mPos;
        if (PSVECMag(&d) < m1660) {
            m14E8 = 1;
        } else {
            m14E8 = 6;
        }
        int damage = 0;
        if (m1624 > 0) {
            if (msg->m44 == 2) {
                damage = 10;
            } else {
                damage = 3;
                if (msg->m44 == 1) {
                    damage = 1;
                }
            }
            if (msg->mData != 0) {
                bool pierce = false;
                if (msg->mAttacker != 0 && msg->mAttacker->mStatus.fn_80133EE8(0x3F) != 0) {
                    CharaStatusEntry_unk* e = msg->mAttacker->mStatus.mEntries.mData;
                    for (unsigned int i = 0; i < msg->mAttacker->mStatus.mEntries.mCount; i++, e++) {
                        if (e->mId == 0x3F && e->mValue == 7) {
                            pierce = true;
                        }
                    }
                }
                if (((float*)&msg->mData->m74)[6] > 0.0f || pierce) {
                    *(int*)msg->m78 = 6;
                    damage *= 2;
                }
            }
            m1624 -= damage;
        }
        atn::Vector3 pos = msg->m1C;
        atn::Vector3 rot = msg->m28.ToRotation_unk();
        if (m1624 <= 0) {
            m1624 = 0;
            m6D0.PushBack_unk(CharaPair8_unk(msg->m34, msg->m38));
            fn_80239DAC(lbl_8087F3C0, &m1504, 0, 0);
            fn_80232B7C(0, 0);
            PlayEffectA_unk(&m1540, &mMotion);
            fn_800C344C("SE_SP007_003", &mPos, 0, -1, 1.0f);
            m1640++;
            if (m16D4 > 0) {
                fn_80370AE4(lbl_8087F430, m16D4, 1);
            }
            for (unsigned int i = 0; i < mLinks.mSize; i++) {
                if (mLinks.mData[i]->m58C != 2) {
                    mLinks.mData[i]->fn_8024FE18(0);
                }
            }
            fn_8023A680(lbl_8087F3C0, &m1528, -1, 0, 0, &pos, &rot, 0, 1.0f, 0, -1, 1);
            if (m16BC == 0 || msg->mData == 0 || (msg->mData->m90.mFlags & 0x4000U) != 0x4000U) {
                return;
            }
        } else {
            m6D0.PushBack_unk(CharaPair8_unk(msg->m34, msg->m38));
            switch (msg->m44) {
            case 0:
            case 1:
                fn_80232B7C(0, 0);
                PlayEffectAt_unk(&m1510, mPos, mRot);
                break;
            case 2:
                fn_80232B7C(0, 0);
                PlayEffectAt_unk(&m151C, mPos, mRot);
                break;
            }
            fn_8023A680(lbl_8087F3C0, &m1534, -1, 0, 0, &pos, &rot, 0, 1.0f, 0, -1, 1);
            fn_800C344C("SE_UI001_008", &mPos, 0, -1, 1.0f);
            msg->m10 *= 0.0f;
            msg->m64 = 4;
            msg->m90 = 1;
            msg->m84 = 1;
            msg->m68 = damage * 10;
            msg->m94 |= 0x40000000;
            return;
        }
    }
    if (m58C != 0 || m55C == 7) {
        msg->m10 *= 0.0f;
    }
    float sq = PSVECSquareMag(&msg->m10);
    if (!(atn::Abs_unk(sq) < 1.1920929e-7f) && sq >= 25.0f) {
        PSVECNormalize(&msg->m10, &msg->m10);
        msg->m10 *= 4.9f;
    }
    if (m16E4 != 0) {
        m58C = 12;
        fn_8016E970(3);
        m14B8 = 0;
        m14BC = 0;
        fn_80170A20(m14B0, 0, -1.0f);
        mStatus.fn_80133130(0x80000000, 100, 0);
        return;
    }
    fn_80151448(msg);
    vf40(hit);
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

// inferred: walk motions by the direction of the movement (as CharaTask::vf24)
// NONMATCHING: 99.52% - f3/f4 swapped for the 0.0f of ToRotation_unk (CharaTask::vf24 has the same
// form at 97%); the .rodata doubles (cos constants and 2*pi) are placed by the unwritten functions
void Em003LesserDaemon::vf24() {
    if (m55C == 6) {
        return;
    }
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    float speed = m574.Length_unk();
    if (m570 <= 0.01f || speed <= 0.01f) {
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        return;
    }
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(mRot.y);
    PSMTXMultVec(m.m, &fwd, &fwd);
    atn::Vector3 dir = m574.GetNormalize();
    float dot = PSVECDotProduct(&fwd, &dir);
    if (dot > (float)cos(0.7853982f)) {
        mMotion.fn_80097C08(0, 0x1C6, 1, 0, 1, 0.0f, 0.1f);
    } else if (dot > (float)cos(2.3561945f)) {
        if (atn::WrapAngle_unk(dir.ToRotation_unk().y - mRot.y) > 0.0f) {
            mMotion.fn_80097C08(0, 0x1C8, 1, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, 0x1C9, 1, 0, 1, 0.0f, 0.1f);
        }
    } else {
        mMotion.fn_80097C08(0, 0x1C7, 1, 0, 1, 0.0f, 0.1f);
    }
    mMotion.mTracks[0].m0C = 1.0f;
}

// inferred: chooses the next action by phase (m14C0: even phases change the barrier, odd ones fight)
void Em003LesserDaemon::fn_8024D120() {
    if (m14B0 == 0) {
        return;
    }
    if (m55C == 6) {
        return;
    }
    if (m14E4 != 0) {
        m14B0 = m14E4;
    }
    atn::Vector3 pos = m14B0->mPos;
    atn::Vector3 d = pos - mPos;
    float dist = PSVECMag(&d);
    if (m1654 != 11 && m14E4 != 0) {
        m14DC = m14E4;
        m14E0 = m14E8;
        if (dist < m1660) {
            m1654 = 1;
        } else {
            m1654 = 6;
        }
        m14E4 = 0;
    } else if (fn_80250884(45.0f) >= 3) {
        m1654 = 13;
    }
    int wait;
    bool broken = m1624 == 0;
    if (broken) {
        wait = m16A4[m14C0];
    } else {
        wait = m168C[m14C0];
    }
    switch (m14C0) {
    case 0:
    case 2:
    case 4:
        m14C0++;
        if (m1620 != 0) {
            if (m14C0 == 2) {
                m1654 = 7;
            } else if (m14C0 == 4) {
                m1654 = 6;
            }
            fn_8024FFE8();
            m14C4 = 0;
        }
        break;
    case 1:
        if (m1654 >= 0) {
            fn_8024E1C0();
            break;
        }
        if (m1670 > m1674[m14C0]) {
            fn_802506AC();
            m14C0++;
            break;
        }
        m14DC = 0;
        m14E4 = 0;
        if (m14BC <= wait) {
            break;
        }
        if (m14C4 >= m161C) {
            WarpFar_unk();
        } else if (dist < m1660) {
            StartAttack_unk();
        } else if (dist < m1664) {
            if (m55C != 7 || m14B4 != 0) {
                fn_80170A20(m14B0, 0, -1.0f);
            }
        } else {
            int n = 1;
            if (m1624 == 0) {
                n = 2;
            }
            if (rand() % n == 0) {
                StartLaser_unk();
            } else {
                StartWarp_unk();
            }
        }
        break;
    case 3:
        if (m1654 >= 0) {
            fn_8024E1C0();
            break;
        }
        if (m1670 > m1674[m14C0]) {
            fn_802506AC();
            m14C0++;
            break;
        }
        if (m14BC <= wait) {
            break;
        }
        if (m14C4 >= m161C) {
            WarpFar_unk();
        } else if (dist < m1660) {
            StartAttack_unk();
        } else if (dist < m1664) {
            if (m55C != 7 || m14B4 != 0) {
                fn_80170A20(m14B0, 0, -1.0f);
            }
        } else if (dist < m1668) {
            int n = 2;
            if (broken) {
                n = 4;
            }
            if (rand() % n == 0) {
                StartLaser_unk();
            } else {
                StartWarp_unk();
            }
            StartLaser_unk();
        } else {
            int n = 1;
            if (broken) {
                n = 2;
            }
            if (rand() % n == 0) {
                StartMagic_unk();
            } else {
                StartWarp_unk();
            }
        }
        break;
    case 5:
        if (m1654 >= 0) {
            fn_8024E1C0();
            break;
        }
        if (m1670 > m1674[m14C0]) {
            fn_802506AC();
            m14C0 = 4;
            break;
        }
        if (m14BC <= wait) {
            break;
        }
        if (m14C4 >= m161C) {
            WarpFar_unk();
        } else if (dist < m1660) {
            StartAttack_unk();
        } else if (dist < m1664) {
            if (m55C != 7 || m14B4 != 0) {
                fn_80170A20(m14B0, 0, -1.0f);
            }
        } else {
            int n = 2;
            if (broken && fn_8024F9B8() != 0) {
                n = 5;
            }
            switch (rand() % n) {
            case 0:
                StartLaser_unk();
                break;
            case 1:
                StartMagic_unk();
                break;
            default:
                StartWarp_unk();
                break;
            }
        }
        break;
    }
}

void Em003LesserDaemon::fn_8024E1C0() {
    switch (m1654) {
    case 7:
        m58C = 7;
        fn_8016E970(3);
        m14B8 = 0;
        fn_8015F568(mC58.mC4->mPos, mC58.mC4, m14D0);
        fn_80232B7C(this, 0);
        PlayEffect_unk(&m1558, &mMotion);
        break;
    case 8: {
        m58C = 8;
        fn_8016E970(3);
        m14B8 = 0;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m154C, &mMotion);
        m164C = m1624 == 0 ? 3 : 1;
        m1650 = -1;
        break;
    }
    case 6:
        if (fn_8024F9B8() != 0) {
            StartWarp_unk();
        }
        break;
    case 11:
        mWarpDest = m1610;
        StartWarp_unk();
        break;
    case 13: {
        int best = FarthestFromSelf_unk();
        if (best >= 0 && best < (int)mPoints.mSize) {
            mWarpDest = mPoints.mData[best];
            StartWarp_unk();
        }
        break;
    }
    case 1:
        m58C = 1;
        fn_8016E970(3);
        m14B8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1564, &mMotion);
        m14C4++;
        break;
    }
    m1654 = m1658;
    m1658 = -1;
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
            fn_800EE360();
        }
    } else {
        m1434 = 30;
    }
}

// NONMATCHING: 99.39% - r4/r5 swapped in the copies of mWarpDest (the __VecCopy family) and f3/f4 for
// the 0.0f of ToRotationZ_unk (as vf24)
void Em003LesserDaemon::fn_8024E7E4() {
    switch (m14B8) {
    case 0: {
        if (m14B0 != 0) {
            atn::Vector3 dest;
            dest = mWarpDest;
            atn::Vector3 d = dest - mPos;
            float t = 0.3f * atn::WrapAngle_unk(d.ToRotationZ_unk().y - mRot.y);
            mRot.y += t;
        }
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.fn_80097C08(0, 0x14B, 1, 0, 1, 1.0f, 0.1f);
            m14B8 = 1;
            fn_80232B7C(0, 0);
            PlayEffectA_unk(&m1570, &mMotion);
        }
        break;
    }
    case 1: {
        atn::Vector3 dest;
            dest = mWarpDest;
        atn::Vector3 d = dest - mPos;
        float dist = PSVECMag(&d);
        if (dist < 3.0f || m14BC >= 90) {
            mMotion.fn_80097C08(0, 0x14C, 0, 0, 1, 1.0f, 0.1f);
            m14B8 = 2;
            break;
        }
        PSVECNormalize(&d, &d);
        d.x *= MIN(dist, 5.0f);
        d.y *= MIN(dist, 5.0f);
        d.z *= MIN(dist, 5.0f);
        CollResult_unk result;
        Sphere_unk sphere = fn_80176548(mPos);
        fn_8004D388(lbl_8087EE98, &result, &sphere.mCenter, &d, 0x80000000, &mColli, 0, sphere.mRadius);
        atn::Vector3 pos = result.m10;
        pos -= m5A4;
        pos.y -= sphere.mRadius;
        mPos = pos;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, m14CC, m590, 30, -1, 0.0f);
        break;
    }
    case 2: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m58C = 0;
            fn_8016E970(3);
            m14B8 = 0;
            m14BC = 0;
            m14DC = 0;
        }
        break;
    }
    }
}

// NONMATCHING: 99.61% - r27/r28 of the forward and the bullet direction swapped and f20/f21 for the
// frame (about 10 variants of declaration order); the @stringBase0 offset of "SE_EM003_003" waits
// for the unwritten functions
void Em003LesserDaemon::fn_8024ED68() {
    int done = 1;
    if (m14B8 == 0 && m55C == 6) {
        switch (m560) {
        case 0x1D:
            done = 0;
            mFlag4 = 1;
            fn_80139560();
            break;
        case 0x1E: {
            ControlMagic_unk* control = (ControlMagic_unk*)mControl.get();
            HitInfo_unk* hit = m638;
            bool fire = false;
            if (control->m1D == 0 && mMotion.mTracks[0].m08 >= 24.0f) {
                fire = true;
            }
            if (fire) {
                HomingParam_unk param;
                Mtx rot;
                Mtx m;
                Mtx m2;
                fn_8016DA4C();
                control->m1D = 1;
                atn::Vector3 pos = mPos;
                pos.y += 30.0f;
                atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &fwd, &fwd);
                fwd.y += 1.0f;
                PSVECNormalize(&fwd, &fwd);
                int count = 3;
                int n = count - 1;
                for (int i = 0; i < count; i++) {
                    atn::Vector3 dir = fwd;
                    float t = ((float)i - 0.5f * (float)n) / (0.5f * (float)n);
                    atn::Vector3 axis(0.0f, 0.0f, 1.0f);
                    PSMTXRotRad(m2, 'y', mRot.y);
                    PSMTXMultVec(m2, &axis, &axis);
                    PSMTXRotAxisRad(rot, &axis, 0.7853982f * t);
                    PSMTXMultVec(rot, &dir, &dir);
                    param.mTarget = 0;
                    param.mAccel = 0.0f;
                    param.m08 = -1.0f;
                    param.mMaxSpeed = 10.0f;
                    param.mTurn = 0.09424778f;
                    param.mTime = 300.0f;
                    param.mDelay = 5.0f;
                    param.mNode = 0;
                    param.mCount = -1;
                    param.mTarget = control->m14;
                    param.mDelay = rand() % 3 + 2;
                    param.mTurn = 0.18849556f;
                    fn_800F8574(lbl_8087F048, this, hit, &pos, &dir, &param, 0, 0, 0.0f, 1.0f);
                }
                fn_800C344C("SE_EM003_003", &pos, 0, -1, 1.0f);
            } else {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    fn_80178208(new ((MemoryTag)1, "", "", 0) chr::ControlMagicEnd(this));
                }
            }
            done = 0;
            break;
        }
        case 0x1F:
            done = 0;
            mFlag4 = 1;
            fn_80139560();
            break;
        }
    }
    if (done) {
        mFlag4 = 1;
        fn_80139560();
        m58C = 0;
        fn_8016E970(3);
        m14B8 = 0;
        m14BC = 0;
        m14DC = 0;
    }
}

// NONMATCHING: 98.46% - saved registers of the fan loop (count/index/vectors) and of the farthest point
// search, f29/f30 of the frame (10 variants: declaration orders, pointer loop, locals)
// inferred: state 8 (aims at the target from the left hand, fires a fan of penetrating lasers and warps
// between the points while m164C lasts)
void Em003LesserDaemon::fn_8024F1DC() {
    switch (m14B8) {
    case 0: {
        if (m14B0 == 0) {
            break;
        }
        MtxPtr m = mMotion.GetNodeMtx_unk("lefthand");
        atn::Vector3 hand(m[0][3], m[1][3], m[2][3]);
        CharaTask* target = m14B0;
        atn::Vector3 center = (target->mColli.m3C + target->mColli.m48) * 0.5f;
        atn::Vector3 rot = (center - hand).ToRotationZ_unk();
        rot.x = MAX(-0.31415927f, MIN(0.31415927f, rot.x));
        float end = mMotion.fn_80097D7C(0) - 32.0f;
        float frame = mMotion.mTracks[0].m08;
        if (!(frame >= mMotion.fn_80097D7C(0) - 18.0f)) {
            if (mMotion.mTracks[0].m08 >= end) {
                rot.x *= MIN(1.0f, (mMotion.mTracks[0].m08 - end) / 14.0f);
            } else {
                rot.x = 0.0f;
            }
        }
        float f;
        if (m1650 >= 0 && (f = mMotion.mTracks[0].m08, 5.0f <= f) && mMotion.mTracks[0].m08 <= 8.0f) {
            atn::Vector3 pos = mPoints.mData[m1650];
            mPos = pos;
        }
        mRot = rot;
        float frame2 = mMotion.mTracks[0].m08;
        if (frame2 >= mMotion.fn_80097D7C(0) - 8.0f) {
            int count = 5;
            if (m16E4 != 0) {
                count = 3;
            }
            atn::Vector3 dir = center - hand;
            PSVECNormalize(&dir, &dir);
            int n = count - 1;
            atn::Vector3 d;
            for (int i = 0; i < count; i++) {
                d = dir;
                Mtx rm;
                PSMTXRotRad(rm, 'y', 0.7853982f * (((float)i - 0.5f * (float)n) / (0.5f * (float)n)));
                PSMTXMultVec(rm, &d, &d);
                fn_800F8574(lbl_8087F048, this, m14D4, &hand, &d, 0, 0, 0, 0.0f, 1.0f);
            }
            m14B8 = 1;
        }
        break;
    }
    case 1: {
        float frame = mMotion.mTracks[0].m08;
        if (!(frame >= mMotion.fn_80097D7C(0))) {
            break;
        }
        if (--m164C > 0) {
            if (mPoints.mSize != 0) {
                if (m1650 < 0) {
                    float bestDist = 0.0f;
                    const atn::Vector3* point = mPoints.mData;
                    int best = -1;
                    for (unsigned int i = 0; i < mPoints.mSize; point++, i++) {
                        atn::Vector3 d = mPos - *point;
                        float dist = PSVECSquareMag(&d);
                        if (dist > bestDist) {
                            best = i;
                            bestDist = dist;
                        }
                    }
                    m1650 = best;
                } else {
                    m1650 = (m1650 + 1) % mPoints.mSize;
                }
                PlayEffectA_unk(&m157C, &mMotion);
            }
            mMotion.fn_80097C08(0, 0x146, 0, 1, 1, 1.0f, 0.1f);
            m14B8 = 0;
        } else {
            mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 1.0f, 0.1f);
            m14B8 = 3;
        }
        break;
    }
    case 3: {
        atn::Vector3 rot = mRot;
        if (rot.x > 0.0f) {
            rot.x -= MIN(rot.x, 0.05f);
        } else if (rot.x < 0.0f) {
            rot.x -= MIN(rot.x, -0.05f);
        }
        mRot = rot;
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            rot.x = 0.0f;
            mRot = rot;
            ResetAction_unk();
        }
        break;
    }
    }
}

int Em003LesserDaemon::fn_8024F9B8() {
    atn::Vector3 from = mPos;
    atn::Vector3 to = m14B0->mPos;
    from.y += 10.0f;
    to.y = from.y;
    atn::Vector3 d = to - from;
    if (PSVECMag(&d) < m166C) {
        return 0;
    }
    PSVECNormalize(&d, &d);
    atn::Vector3 pos;
    atn::Vector3 off = d * 8.0f;
    to -= off;
    CollResult_unk result;
    void* colli = lbl_8087EE98;
    if (fn_8004ED34(colli, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
        from = to;
        from.y += 5.0f;
        to.y -= 20.0f;
        void* colli2 = lbl_8087EE98;
        if (fn_8004ED34(colli2, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) != 0) {
            if ((float)__fabs(result.m04.y - mPos.y) < 5.0f) {
                from = result.m04;
                from.y += 0.5f + m620;
                atn::Vector3 move(0.0f, 0.0f, 0.0f);
                void* world = lbl_8087EE98;
                int hit = fn_8004D388(world, &result, &from, &move, fn_80179D44() | 0x80000000, 0, 0, m620);
                pos = result.m10;
                pos.y -= 0.5f + m620;
                if (hit == 0) {
                    mWarpDest = pos;
                    return 1;
                }
                from = mPos;
                to = pos;
                from.y += 10.0f;
                to.y += 10.0f;
                void* colli3 = lbl_8087EE98;
                if (fn_8004ED34(colli3, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
                    mWarpDest = pos;
                    return 1;
                }
            }
        }
    }
    return 0;
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
    for (unsigned int i = 0; i < mLinks.mSize; i++) {
        if (mLinks.mData[i]->m58C != 2) {
            mLinks.mData[i]->fn_8024FE18(0);
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

// inferred: index of the warp point nearest to `pos` (name invented)
inline int Em003LesserDaemon::NearestPoint_unk(const atn::Vector3& pos) {
    float bestDist = 1e16f;
    int best = -1;
    for (unsigned int i = 0; i < mPoints.mSize; i++) {
        atn::Vector3 d = pos - GetPoint_unk(i);
        float dist = PSVECSquareMag(&d);
        if (dist < bestDist) {
            best = i;
            bestDist = dist;
        }
    }
    return best;
}

// inferred: event of the phase change: warps to the barrier, restores it and places the linked daemons
// around in a triangle (their next action is the warp of action 11)
void Em003LesserDaemon::fn_8024FFE8() {
    m58C = 9;
    fn_8016E970(3);
    m14B8 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(&m1504, 0);
    PlayEffectAM_unk(lbl_8087F3C0, &m14F8, &mMotion);
    m1624 = m1628[m14C0];
    m1600 = mPos;
    mWarpDest = mPos;
    m160C = 0.0f;
    if (mLinks.size() == 0) {
        return;
    }
    atn::Vector3 pts[3];
    atn::Vector3 dests[3];
    int r = rand() % 3;
    int next = m1654;
    Mtx m2;
    Mtx m;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    PSMTXRotRad(m, 'y', mRot.y);
    PSMTXMultVec(m, &front, &front);
    atn::Vector3 side(20.0f, 0.0f, 0.0f);
    PSMTXRotRad(m2, 'y', mRot.y);
    PSMTXMultVec(m2, &side, &side);
    pts[r] = m1600;
    pts[(r + 1) % 3] = m1600 + side + front;
    pts[(r + 2) % 3] = m1600 - side + front;
    mWarpDest = pts[0];
    int best = NearestPoint_unk(m1600);
    if (best < 3) {
        dests[r] = GetPoint_unk(best + 3);
        dests[(r + 1) % 3] = GetPoint_unk((best + 1) % 3 + 3);
        dests[(r + 2) % 3] = GetPoint_unk((best + 2) % 3 + 3);
    } else {
        best -= 3;
        dests[r] = GetPoint_unk(best);
        dests[(r + 1) % 3] = GetPoint_unk((best + 1) % 3);
        dests[(r + 2) % 3] = GetPoint_unk((best + 2) % 3);
    }
    m1610 = dests[0];
    m1654 = 11;
    m1658 = rand() % 2 != 0 ? next : -1;
    for (unsigned int i = 0; i < mLinks.mSize; i++) {
        mLinks.mData[i]->fn_80176ACC();
        mLinks.mData[i]->mStatus.fn_8012D8B8();
        mLinks.mData[i]->mStatus.m04 = mStatus.m04;
        mLinks.mData[i]->mPos = mPos;
        mLinks.mData[i]->mRot = mRot;
        mLinks.mData[i]->m14C0 = m14C0;
        mLinks.mData[i]->fn_8024FFE8();
        mLinks.mData[i]->mWarpDest = pts[i + 1];
        mLinks.mData[i]->m1610 = dests[i + 1];
        mLinks.mData[i]->m1654 = 11;
        mLinks.mData[i]->m1658 = rand() % 2 != 0 ? next : -1;
    }
    if (fn_80370A78(lbl_8087F430, 0xFA) == 0) {
        fn_80370AE4(lbl_8087F430, 0xFA, 1);
    }
}

// NONMATCHING: 99.49% - the point pointer and the index of the loop get r28/r30 swapped (12 variants:
// pointer walk, index, while, helper inline, declaration orders)
void Em003LesserDaemon::fn_802506AC() {
    if (mPoints.mSize == 0) {
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
    for (unsigned int i = 0; i < mLinks.mSize; i++) {
        if (mLinks.mData[i]->m58C != 2) {
            mLinks.mData[i]->fn_8024FE18(0);
        }
    }
    mWarpDest = mPoints.mData[best];
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
            if (PSVECSquareMag(&d) < radius * radius) {
                count++;
            }
        }
    }
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if (!e->mStatus.IsState_unk(0x20) && !e->fn_8016F3D0(this, 1)) {
            atn::Vector3 d = e->mPos - pos;
            if (PSVECSquareMag(&d) < radius * radius) {
                count++;
            }
        }
    }
    return count;
}

