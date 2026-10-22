#include "game/Em101ElderZesya.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "chr/ControlBase.h"
#include "game/CharaStatus.h"
#include "game/EnemyManager.h"
#include "game/HomingBullet.h"
#include "game/Em101LittleZesya.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: kind of each barrier (rodata table 0x80743CF0: indexes into the HitRange_unk values)
static const int sBarrierKinds_unk[4] = {3, 0, 1, 4};
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: debug menu entries (as in gmk/SwingObject.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80088FAC(void* group, const char* name, ZesyaColor_unk* value, int arg3, int arg4, float min,
                            float max, float step);

struct Gimmick_unk {
    unsigned char m00[0x04];
    atn::Vector3 mPos;
    unsigned char m10[0x14 - 0x10];
    float mRotY_unk;
};

inline const atn::Vector3& Em101ElderZesya::Barrier_unk::GetPos_unk() const {
    return m04.mDesc->mPos;
}

// inferred: difference on the ground plane (name invented)
inline atn::Vector3 FlatDiff_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return atn::Vector3(a.x - b.x, 0.0f, a.z - b.z);
}

// inferred: point the camera looks at (lbl_8087F430 + 0x97C, as in game/EnemyJill.cpp); names invented
struct ElderLook_unk {
    void Set_unk(const atn::Vector3& pos, bool flag) {
        m01 = m00;
        m00 = 1;
        mPos = pos;
        m24 = 0.0f;
        if (flag) {
            m04 = 1;
        } else {
            m04 = 0;
        }
    }
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[2];
    int m04;
    unsigned char m08[4];
    atn::Vector3 mPos;
    unsigned char m18[0x24 - 0x18];
    float m24;
};
// inferred: camera parameters of the field (lbl_8087F430 + 0x588; as JillCamParam_unk in game/EnemyJill.cpp)
struct ElderCamParam_unk {
    ElderCamParam_unk(const ElderCamParam_unk& o)
        : m00(o.m00), m04(o.m04), m08(o.m08), m0C(o.m0C), m10(o.m10), m14(o.m14), m18(o.m18), m1C(o.m1C),
          m20(o.m20), m24(o.m24), m28(o.m28), m2C(o.m2C), m30(o.m30), m34(o.m34), m38(o.m38) {}
    ElderCamParam_unk& operator=(const ElderCamParam_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08 = o.m08;
        m0C = o.m0C;
        m10 = o.m10;
        m14 = o.m14;
        m18 = o.m18;
        m1C = o.m1C;
        m20 = o.m20;
        m24 = o.m24;
        m28 = o.m28;
        m2C = o.m2C;
        m30 = o.m30;
        m34 = o.m34;
        m38 = o.m38;
        return *this;
    }
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    int m38;
};
// inferred: camera mode of the field (lbl_8087F430 + 0x4D8); names invented
struct ElderCamera_unk {
    void Set_unk(int mode, const ElderCamParam_unk& param) {
        if (mMode != mode) {
            mMode = mode;
            mParam = param;
            m08 = 0.05f;
            m04 = 1.0f;
        }
    }
    int mMode;
    float m04;
    float m08;
    ElderCamParam_unk mParam;
};
// inferred: field (lbl_8087F430): camera shake at 0x96C (as in game/Em101ElderZesyaSecond.cpp), look point at
// 0x97C, route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x4D8];
    ElderCamera_unk m4D8;
    unsigned char m520[0x588 - 0x520];
    ElderCamParam_unk m588;
    unsigned char m5C4[0x8A0 - 0x5C4];
    CharaTask* m8A0; // inferred: character the camera follows
    unsigned char m8A4[0x96C - 0x8A4];
    int m96C; // inferred: camera shake
    int m970;
    float m974;
    float m978;
    ElderLook_unk m97C;
    unsigned char m9A4[0x10D8 - 0x9A4];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: target redirection of the party (lbl_8087EE68, as MagicTarget_unk in game/CharaTask.cpp)
struct ElderRedirect_unk {
    unsigned char m00[0x08];
    int m08;
    CharaTask* m0C;
};
extern "C" void* lbl_8087EE68;
extern "C" ElderRedirect_unk* fn_800185B4(void* manager, CharaTask* chara, HitInfo_unk* hit);
struct Effect_unk;
extern "C" void fn_801028E4(Effect_unk* mgr, CharaTask* chara, CharaTask* enemy, int arg); // inferred: hate
// inferred: result of fn_8003EA3C (as DamageResult_unk in game/Em002Skelton.cpp); names invented
struct ElderDamage_unk {
    ElderDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
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
struct HitParam_unk;
extern "C" HitParam_unk lbl_807C6B90; // inferred: default parameters
extern "C" void fn_8003EA3C(ElderDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const HitParam_unk* param, int arg, int arg2); // inferred: damage
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80102EAC(Effect_unk* mgr, int id, CharaTask* attacker, int arg, int arg2);

// inferred: collision world and its ray test (see game/ChaserNpc.cpp)
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
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an effect on the model (locals in the order of game/Em305Vampire.cpp)
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

extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
// inferred: constructor of chr::ControlMagicEnd (chr/ControlChant.cpp, 0x34 bytes; still in asm)
extern "C" chr::ControlBase* fn_801C02F4(void* self, CharaTask* chara);

extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct ElderMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" ElderMsgTable_unk* lbl_8087F1E4;
// inferred: order sent to the little Zesya (game/Em101LittleZesya.h ZesyaWarp_unk)
struct ElderOrder_unk {
    int m00;
    int m04;
    atn::Vector3 m08;
};
extern "C" void fn_8025FE30(int zesya, const ElderOrder_unk* order); // inferred: Em101LittleZesya order
extern "C" void fn_8025D6D0(int zesya); // inferred: Em101LittleZesya saves its position (game/Em101LittleZesya.cpp)
extern "C" void fn_8025D704(int zesya); // inferred: Em101LittleZesya goes back to idle

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em101LittleZesyaSecond.cpp)
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" int rand();
// inferred: fields of the running magic control (class unknown; as in game/Em003LesserDaemon.cpp)
struct ControlMagic_unk {
    unsigned char m00[0x14];
    CharaTask* m14; // inferred: target
    unsigned char m18[0x1D - 0x18];
    unsigned char m1D; // inferred: nonzero once the bullets are fired
};
// inferred: list of free players (up to 16; names invented)
struct ElderTargets_unk {
    ElderTargets_unk() : mCount(0) {}
    unsigned int size() const { return mCount; }
    void push_back(PlayerTask* const& x) {
        new (&mData[mCount]) PlayerTask*(x);
        mCount++;
    }
    PlayerTask*& operator[](unsigned int i) { return mData[i]; }
    unsigned int mCount;
    PlayerTask* mData[16];
};
// inferred: alive, not hidden and not grappled (written out as in the original; name invented)
inline bool IsTarget_unk(CharaTask* p) {
    bool ok = false;
    bool dead = false;
    if ((p->mFlags & 4) == 4 || (p->mFlags & 1) == 1) {
        dead = true;
    }
    if (!dead && (p->mStatus.m0C & 0x20) != 0x20 && !p->IsGrappled_unk()) {
        ok = true;
    }
    return ok;
}
extern "C" double sqrt(double x);

// inferred: barrier sizes (radius, height); static vector of the file built by the __sinit (0x8025A03C)
static atn::Vector3 sChantOffset_unk(35.0f, 80.0f, 1.0f);

// NONMATCHING: 99.6% - registers r25/r26 of the strlen of the parameters and of the debug menu group
// (same as the second std::string of Em201ZanGurg::Em201ZanGurg)
Em101ElderZesya::Em101ElderZesya(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14C8(0), m14CC(0), m14D0(0), m14D4(0), m14F0(0), m14F4(0),
      m14F8(0), m14FC(1), m1500(0), m1510(0), m15EC(0), m15F0(0), m15F4(0), m164C(0), m1650(0),
      m1654(0.0f, 0.0f, 0.0f), m1660(0), m1668(0.0f, 0.0f, 0.0f), m1674(0.0f, 0.0f, 0.0f), m1680(0),
      m1684(0.0f, 0.0f, 0.0f), m1690(0.0f), m1694(0.0f, 0.0f, 0.0f), m16A4(0), m16A8(0), m16AC(1), m16B0(120),
      m16B4(9.0f), m16BC(0), m16C0(ZesyaColor_unk(0.98039216f, 0.3137255f, 0.23529412f, 1.0f)),
      m16D0(ZesyaColor_unk(0.078431375f, 0.11764706f, 0.9411765f, 1.0f)),
      m16E0(ZesyaColor_unk(1.0f, 1.0f, 1.0f, 1.0f)),
      m16F0(ZesyaColor_unk(0.23529412f, 0.39215687f, 0.27450982f, 1.0f)),
      m1700(ZesyaColor_unk(0.9019608f, 0.8235294f, 0.19607843f, 1.0f)) {
    mFlag22 = 1;
    mFlagB11 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em101_00_00.csv");
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
    m14B4.vf0C(path.c_str());
    void* group = m16B8.Load("Em101ElderZesya");
    fn_80087994(group, "DebugDisp", &m16BC, 0, 0);
    fn_800874C8(group, "WaitFrame", &m16B0, 0, 1000000, 1, 0, 0);
    fn_8008771C(group, "AttackChanceFrame", &m16B4, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_800874C8(group, "ComboWaitTime", &m14FC, 0, 100000, 1, 0, 0);
    fn_800874C8(group, "bossActPhase_", &m14CC, 3, 5, 1, 0, 0);
    fn_80088FAC(group, "FireBarrier", &m16C0, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088FAC(group, "IceBarrier", &m16D0, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088FAC(group, "ForestBarrier", &m16F0, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088FAC(group, "HolyBarrier", &m1700, 0, 0, 0.0f, 1.0f, 0.01f);
    m15F8.fn_8023780C("ef_bs007a.efp");
    m1604.fn_8023780C("ef_bs066e.efp");
    m1610.fn_8023780C("ef_bs066c.efp");
    m161C.fn_8023780C("ef_sk053a.efp");
    m1628.fn_8023780C("ef_sk053b.efp");
    m1634.fn_8023780C("ef_sk053c.efp");
    m1640.fn_8023780C("ef_sk054d.efp");
    mFlagB5 = 1;
}

Em101ElderZesya::Barrier_unk::Barrier_unk() {
    m04.mState = 2;
    m04.mKind = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m04.mObjects[i][j] = 0;
        }
    }
    m04.mDesc = 0;
}

Em101ElderZesya::Barrier_unk::~Barrier_unk() {}

Em101ElderZesya::~Em101ElderZesya() {}

int Em101ElderZesya::vf0C() {
    if (m14B0 == 0) {
        if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m14B4.fn_80473F50() == 0 &&
            m15F8.fn_80237874() == 0 && m1604.fn_80237874() == 0 && m1610.fn_80237874() == 0 &&
            m161C.fn_80237874() == 0 && m1628.fn_80237874() == 0 && m1634.fn_80237874() == 0) {
        mStatus.m18 |= 0x1C0;
        m58C = 0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        fn_8016E970(2);
        fn_80256384();
        m14C8 = 100000;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        m14B0 = 1;
        }
    } else if (m14B0 == 1) {
        for (unsigned int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (m151C[i].m04.mObjects[j][k] != 0 && m151C[i].m04.mObjects[j][k]->fn_800580BC() != 0) {
                        return 0;
                    }
                }
            }
        }
        m14B0 = 2;
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.85% - r24/r25 of the route table and the barrier pointer (8 variants: declaration orders,
// field local, table copy, pointer/offset forms); the push_back of the points is the inlined Vector_unk one
void Em101ElderZesya::fn_80256384() {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    int barrier = 0;
    atn::Csv csv((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == ';' || *key == '\0') {
            continue;
        }
        if (strcmp(key, "WaitFrame") == 0) {
            m16B0 = atoi(csv.fn_8005B9CC());
        }
        if (strcmp(key, "ComboWaitTime") == 0) {
            m14FC = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HighJumpAttackPointID") == 0) {
            while (true) {
                int id = atoi(csv.fn_8005B9CC());
                if (id <= 0) {
                    break;
                }
                RoutePlace_unk* place = table->FindPlace_unk(id);
                if (place != 0) {
                    m14BC.push_back(place->mPos);
                }
            }
        } else if (strncmp(key, "ChantPoint", 10) == 0) {
            if (barrier < 4) {
                Barrier_unk* b = &m151C[barrier];
                b->m00 = (Gimmick_unk*)table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                b->m04.mDesc = (Gimmick_unk*)table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                for (int i = 0; i < 3; i++) {
                    b->m04.mObjects[0][i] = new (12, "", "", 0) atn::ColliObject();
                    b->m04.mObjects[0][i]->fn_80058078("ef_col02n.hcb");
                    b->m04.mObjects[1][i] = new (12, "", "", 0) atn::ColliObject();
                    b->m04.mObjects[1][i]->fn_80058078("ef_col02n.hcb");
                    for (int j = 0; j < 3; j++) {
                        if (b->m04.mObjects[j][i] != 0) {
                            b->m04.mObjects[j][i]->mFlags &= ~1;
                            b->m04.mObjects[j][i]->mFlags |= 2;
                            b->m04.mObjects[j][i]->mFlags |= 0x40000000;
                            b->m04.mObjects[j][i]->m0C = (int)this;
                            atn::Matrix3x4 m;
                            m.SetIdentity();
                            b->m04.mObjects[j][i]->SetMtx_unk(m);
                        }
                    }
                }
                b->m04.mKind = sBarrierKinds_unk[barrier];
                barrier++;
            }
        } else if (strcmp(key, "LittleID") == 0) {
            EnemyManager* mgr = EnemyManager::sInstance;
            m16A0 = (int)mgr->Find_unk(atoi(csv.fn_8005B9CC()));
            *(Em101ElderZesya**)((char*)m16A0 + 0x1530) = this;
        } else if (strcmp(key, "NormalAttackID") == 0) {
            m1500 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "HighJumpAttackID") == 0) {
            m1510 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "FlareID") == 0) {
            m1504 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "ThunderID") == 0) {
            m1518 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "FlarelusID") == 0) {
            m1508 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "LightSwordID") == 0) {
            m150C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "HealID") == 0) {
            m1514 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        }
    } while (csv.fn_8005BBF8());
}

// inferred: rotation z, y, x of a matrix (as ChaserRotZYX_unk in game/ChaserNpc.cpp)
inline void ElderRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    __MtxRotate(m.m, 'z', rot.z);
    __MtxRotate(m.m, 'y', rot.y);
    __MtxRotate(m.m, 'x', rot.x);
}

// NONMATCHING: 97.84% - stack slots of the barrier vectors/matrices and of the player block, a frsp of the z
// rotation and r0/r3 of two stores (8 variants: temporaries, case order, rotation inline, array init)
void Em101ElderZesya::vf10() {
    m16A4++;
    if (fn_80370A78(lbl_8087F430, 0x65) == 1) {
        return;
    }
    if (m16A8 > 0) {
        if (--m16A8 <= 0) {
            fn_80370AE4(lbl_8087F430, 0x65, 1);
            m58C = 6;
            m14C8 = 0;
            fn_8016DA4C();
            m14D4 = 1;
            mColli.mFlags |= 1;
            fn_8025D6D0(m16A0);
            m14D8 = mPos;
            m14E4 = mRot;
            return;
        }
    } else if (m16A8 == 0 && m14D4 == 0) {
        Em101LittleZesya* little = (Em101LittleZesya*)m16A0;
        bool ready = false;
        if (little->m58C == 9 && little->m14F4 == 1) {
            ready = true;
        }
        if (ready) {
            m16A8 = 20;
        }
    }
    if (mC58.mC4 == 0) {
        mC58.mC4 = lbl_8087F8A0->mHead;
    }
    if (m15F0 != 0) {
        if (m1680 < 4) {
            Barrier_unk* b = &m151C[m1680];
            if (m15EC == 0) {
                b->m04.mState = 0;
                if (b->m04.mDesc != 0) {
                    fn_80259AB4(m1680, 0);
                }
            }
            if (m15EC % 16 == 15) {
                Field_unk* field = lbl_8087F430;
                field->m96C %= 2;
                field->m970 = 10;
                field->m974 = 0.15f;
                field->m978 = 0.15f;
                fn_800C31F4("SE_DG036_014", 0, -1, 1.0f);
            }
            m1650 = 1;
            m15EC++;
            if (b->m04.mDesc != 0) {
                m1654 = b->m04.mDesc->mPos;
                m1654.y += 0.5f * sChantOffset_unk.y;
            }
        }
        if (m15EC >= 20) {
            m15F0 = 0;
        }
    }
    if (m15F4 != 0) {
        if (m1680 < 4) {
            m1650 = 1;
            Barrier_unk* b = &m151C[m1680];
            if (b->m04.mDesc != 0) {
                m1654 = b->m04.mDesc->mPos;
                m1654.y += 0.5f * sChantOffset_unk.y;
            }
        }
        if (++m15EC >= 20) {
            m15F4 = 0;
        }
    }
    Barrier_unk::Colli_unk* c = &m151C[m1680].m04;
    for (int i = 0; i < 3; i++) {
        atn::Vector3 pos;
        atn::Vector3 rot;
        if (c->mDesc != 0) {
            pos = atn::Vector3(c->mDesc->mPos);
            pos.y += -10.0f;
            rot = atn::Vector3(0.0f, c->mDesc->mRotY_unk, 0.0f);
        } else {
            pos = atn::Vector3(0.0f, 10000.0f, 0.0f);
            rot = atn::Vector3(0.0f, 0.0f, 0.0f);
        }
        atn::Vector3 offs[3];
        offs[0] = atn::Vector3(0.0f, 0.0f, 0.0f);
        offs[1] = atn::Vector3(-sChantOffset_unk.x, 0.0f, -sChantOffset_unk.x);
        Mtx m;
        PSMTXRotRad(m, 'y', rot.y);
        PSMTXMultVec(m, &offs[1], &offs[1]);
        offs[2] = atn::Vector3(sChantOffset_unk.x, 0.0f, -sChantOffset_unk.x);
        Mtx m2;
        PSMTXRotRad(m2, 'y', rot.y);
        PSMTXMultVec(m2, &offs[2], &offs[2]);
        float angles[3];
        angles[0] = 0.0f;
        angles[1] = -1.5707964f;
        angles[2] = 1.5707964f;
        for (int j = 0; j < 3; j++) {
            if (c->mObjects[i][j] == 0) {
                continue;
            }
            atn::Vector3 r = rot;
            r.y += angles[j];
            atn::Vector3 p = pos + offs[j];
            atn::Matrix3x4 t;
            t.SetIdentity();
            atn::Matrix3x4 m3;
            ElderRotZYX_unk(m3, r);
            PSMTXConcat(m3.m, t.m, t.m);
            t.m[0][3] = p.x;
            t.m[1][3] = p.y;
            t.m[2][3] = p.z;
            c->mObjects[i][j]->SetMtx_unk(t);
            if (CanAct_unk() && c->mState == i) {
                c->mObjects[i][j]->mFlags |= 1;
            } else {
                c->mObjects[i][j]->mFlags &= ~1;
            }
        }
    }
    if (mC58.mC0 != 0 && mC58.mC4 != 0) {
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            ElderRedirect_unk* r = lbl_8087EE68 != 0 ? fn_800185B4(lbl_8087EE68, p, 0) : 0;
            if (r != 0 && r->m08 == 0 && r->m0C == this) {
                fn_801028E4(lbl_8087F048, p, this, 0x30D40);
            } else {
                fn_801028E4(lbl_8087F048, p, (CharaTask*)m16A0, 0x30D40);
            }
        }
        int wait = m14D4;
        m14C8++;
        if (wait > 0) {
            m14D4 = 0;
            fn_8016DA4C();
            m58C = 6;
            m14CC = 2;
            m14C8 = m16B0;
            fn_8025D704(m16A0);
            PlayerTask* player = lbl_8087F8A0->mHead;
            player->fn_80166A54();
            ElderDamage_unk result;
            fn_8003EA3C(&result, fn_80219E6C(0xFAA), this, player, 0, &lbl_807C6B90, 0, 0);
            if (!player->mStatus.IsState_unk(0x20)) {
                player->fn_8015ECC4();
            } else {
                atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', player->mRot.y);
                PSMTXMultVec(m, &dir, &dir);
                atn::Vector3 back(-dir.x, -dir.y, -dir.z);
                player->fn_8015E7A0(back, -1, 0);
                player->vf44();
            }
        }
        if (m14F8 != 0) {
            m14CC = 2;
            m14C8 = m16B0;
            m14F8 = 0;
        }
        if (m55C == 2) {
            fn_8016E970(3);
        }
        switch (m58C) {
        case 7:
        case 8:
        case 15:
            fn_80258A50();
            break;
        case 10: {
            m638 = m1518;
            float y = mPos.y;
            float wait2 = (float)m1518->mC0;
            mFB8 += 1.0f;
            mFBC = wait2;
            if (y - m1668.y < 12.0f) {
                mPos.y = y + 0.08f;
            }
            if (mFB8 >= (float)m638->mC0) {
                mFB8 = 0.0f;
                mF7C = mC58.mC4;
                mF6C = mC58.mC4->mPos;
                m14C8 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m14D0 = 0;
                m1660 = 0;
                m58C = 11;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x6B, 0, 0, 1, 0.0f, 0.1f);
                fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            }
            break;
        }
        case 11:
            fn_80258F1C();
            break;
        case 12: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                fn_8025941C();
            }
            break;
        }
        case 13: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                m58C = 6;
                m14C8 = 0;
            }
            break;
        }
        case 9:
            fn_8025922C();
            break;
        case 14: {
            if (mMotion.mTracks[0].m08 < 25.0f) {
                m1654 = mPos;
                m1650 = 1;
                m1654.y += 25.0f;
            }
            float frame = mMotion.mTracks[0].m08;
            if ((float)__fabs(frame - 24.0f) < 1.1920929e-7f) {
                m15F0 = 1;
                m15EC = 0;
            } else if (frame >= mMotion.fn_80097D7C(0)) {
                m58C = 6;
                m14C8 = 0;
            }
            break;
        }
        case 16:
            if (mC58.mC4 != 0) {
                fn_80139560();
            }
            break;
        default:
            if (m55C != 6) {
                if (m55C == 7) {
                    fn_8016E970(3);
                } else if (m14C8 > m16B0) {
                    fn_80258574();
                }
            }
            if (m55C != 3) {
                fn_80139560();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    Field_unk* field = lbl_8087F430;
    if (m1650 != 0) {
        if (m164C == 0) {
            field->m8A0 = this;
            field->m97C.Set_unk(m1654, false);
            m164C = 1;
        } else {
            field->m8A0 = this;
            field->m97C.mPos = m1654;
        }
    } else if (m164C != 0) {
        field->m8A0 = 0;
        field->m97C.m00 = 0;
        m164C = 0;
        field->m4D8.mMode = 0;
    }
    m1650 = 0;
}

atn::Vector3 Em101ElderZesya::vf64(int center) {
    atn::Vector3 result = mPos;
    Barrier_unk* barrier = &m151C[m1680];
    if (barrier->m04.mState != 2 && barrier->m04.mDesc != 0) {
        result = barrier->GetPos_unk();
        result.y += 0.8f * sChantOffset_unk.y;

    } else {
        result = m624.mData[0].m04.mCenter;
    }
    return result;
}

// inferred: the third argument is a position here
int Em101ElderZesya::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    const atn::Vector3* pos = (const atn::Vector3*)arg;
    Barrier_unk* barrier = &m151C[m1680];
    if (barrier->m04.mState != 2 && barrier->m04.mDesc != 0) {
        atn::Vector3 d = *pos - barrier->m04.mDesc->mPos;
        d.y = 0.0f;
        if (PSVECMag(&d) < sChantOffset_unk.x) {
            if (((float*)&hit->m74)[barrier->m04.mKind] < 0.5f) {
                return 4;
            }
        }
    }
    return 0;
}


void Em101ElderZesya::vf50() {
    Item14_unk item;
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += item.m04.mRadius;
    m624.push_back(item);
    item.m04.mRadius = 1.5f;
    item.m04.mCenter = mPos;
    m624.push_back(item);
    mFlagB29 = 1;
}

// NONMATCHING: 99.9% - the original reloads m624.mData before the copy of the center (one lwz more,
// same open case as CharaTask::vf54)
// inferred: damage sphere of the body; while a barrier is up a second sphere covers it
void Em101ElderZesya::vf54() {
    if (m624.mSize != 0) {
        Barrier_unk* barrier = &m151C[m1680];
        Item14_unk* item = m624.mData;
        item->m04.mRadius = m5B0;
        item->m04.mCenter = mPos + m5A4;
        if (((unsigned int)mStatus.mParamB.m9C & 0x40) == 0x40) {
            m624.mData[0].m04.mCenter.y -= m624.mData[0].m04.mRadius;
        } else {
            m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
        }
        if (barrier->m04.mState != 2) {
            m624.mData[0].m04.mCenter.y -= 10000.0f;
            mFlagB29 = 0;
        } else {
            mFlagB29 = 1;
        }
        if (barrier->m04.mDesc != 0) {
            m624.mData[1].m04.mRadius = 30.0f;
            m624.mData[1].m04.mCenter = barrier->m04.mDesc->mPos;
            m624.mData[1].m04.mCenter.y += 30.0f;
            m624.mData[1].m04.mCenter.y += -10.0f;
            atn::Vector3 off(0.0f, 0.0f, -1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', barrier->m04.mDesc->mRotY_unk);
            PSMTXMultVec(m, &off, &off);
            m624.mData[1].m04.mCenter += off;
            m624.mData[1].m00 = 0;
            if (barrier->m04.mState == 2) {
                m624.mData[1].m04.mCenter.y += 10000.0f;
                m624.mData[1].m00 |= 8;
            }
        }
    }
}

// inferred: warp point of the current barrier (name invented)
inline void Em101ElderZesya::GetWarp_unk(atn::Vector3* pos, float* rot) {
    Gimmick_unk* desc = 0;
    if (m1680 < 4) {
        desc = m151C[m1680].m00;
    }
    if (desc != 0) {
        *pos = desc->mPos;
        *rot = desc->mRotY_unk;
    }
}

// NONMATCHING: 99.95% in the direct objdiff (100% in report.json) - only the labels of two .sdata2 floats
// inferred: same as PlayEffect_unk with the locals in the order of game/Em003LesserDaemon.cpp
inline void PlayEffectB_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

// inferred: starts the magic of state 10 (name invented)
inline void Em101ElderZesya::StartMagic_unk() {
    m14C8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14D0 = 0;
    m1660 = 0;
    m58C = 10;
    fn_8016E970(6);
    m560 = 29;
    m638 = m1518;
    m1668 = mPos;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x66, 1, 0, 1, 0.0f, 0.1f);
    mFB8 = 0.0f;
}

void Em101ElderZesya::fn_80258574() {
    if (m14F4 != 0) {
        if (++m1680 >= 4) {
            m1680 = 0;
        }
        m14CC = 0;
        m14F4 = 0;
        m16AC = 0;
    }
    atn::Vector3 target = mC58.mC4->mPos;
    atn::Vector3 d = target - mPos;
    PSVECMag(&d);
    switch (m14CC) {
    case 0:
        m14C8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m14D0 = 0;
        m1660 = 0;
        m58C = 12;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(this, 101);
        PlayEffectB_unk(&m15F8, &mMotion);
        m14CC = 1;
        break;
    case 1:
        m14C8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m14D0 = 0;
        m1660 = 0;
        m58C = 14;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x67, 0, 0, 1, 0.0f, 0.1f);
        m14CC = 3;
        break;
    case 2: {
        m58C = 9;
        m14C8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m14D0 = 0;
        m1660 = 0;
        wchar_t text[256];
        memset(text, 0, sizeof(text));
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x9F), *(const wchar_t**)((char*)m60 + 4));
        fn_80109828(lbl_8087F048, text);
        fn_802598DC();
        fn_8016D74C(m1510, 0, (int)&m1694);
        ElderOrder_unk order;
        order.m00 = 1;
        order.m08 = m1694;
        order.m04 = 2;
        fn_8025FE30(m16A0, &order);
        break;
    }
    case 3:
        StartMagic_unk();
        break;
    case 4:
        StartMagic_unk();
        m14CC = 5;
        break;
    case 5:
        StartMagic_unk();
        m14CC = 3;
        break;
    }
    *(int*)(m16A0 + 0x14EC) = m14CC;
}

// NONMATCHING: 99.19% - nonvolatile registers shifted by one (r24-r30: the original keeps the list base in
// r24 and the control in r25) and f20/f21 of the frame; same layout as Em003LesserDaemon::fn_8024ED68
// (8 variants: helper forms of the player test, list hoisted, control hoisted)
void Em101ElderZesya::fn_80258A50() {
    if (m55C == 6) {
        if (m560 == 0x1E) {
        HitInfo_unk* hit = m638;
        if (hit == m150C) {
            ControlMagic_unk* control = (ControlMagic_unk*)mControl.get();
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
                int count = 10;
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
                    if (i > 0) {
                        ElderTargets_unk list;
                        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                            if (IsTarget_unk(p)) {
                                list.push_back(p);
                                if (list.size() >= 16) {
                                    break;
                                }
                            }
                        }
                        if (list.size() != 0) {
                            param.mTarget = list[rand() % list.size()];
                        }
                    }
                    param.mDelay = rand() % 3 + 5;
                    param.mTurn = 0.18849556f;
                    fn_800F8574(lbl_8087F048, this, hit, &pos, &dir, &param, 0x44, 0, 0.0f, 1.0f);
                }
            } else {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    void* p = ::operator new(0x34, 1, "", "", 0);
                    chr::ControlBase* c = (chr::ControlBase*)p;
                    if (p != 0) {
                        c = fn_801C02F4(p, this);
                    }
                    fn_80178208(c);
                }
            }
        }
        }
    } else {
        m58C = 6;
        m14C8 = 0;
    }
    fn_80139560();
}

void Em101ElderZesya::fn_80258F1C() {
    m638 = m1518;
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
        mFB8 = 0.0f;
        m58C = 6;
        m14C8 = 0;
        void* p = ::operator new(0x34, 0, "", "", 0);
        chr::ControlBase* c = (chr::ControlBase*)p;
        if (p != 0) {
            c = fn_801C02F4(p, this);
        }
        fn_80178208(c);
        return;
    }
    if ((float)__fabs(mMotion.mTracks[0].m08 - 12.0f) < 1.1920929e-7f) {
        m1674 = mC58.mC4->mPos;
        atn::Vector3 from = m1674 + atn::Vector3(0.0f, 1.0f, 0.0f);
        atn::Vector3 to = m1674 + atn::Vector3(0.0f, -100.0f, 0.0f);
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0) {
            m1674 = result.m10;
        }
        m1674 += atn::Vector3(0.0f, 0.0f, 1.0f);
        fn_80232B7C(this, 1000);
        fn_8023A8B4(lbl_8087F3C0, &m1604, 0, 0, m1674, mRot, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
        fn_800C31F4("SE_SP016_001_2", 0, -1, 1.0f);
    } else if ((float)__fabs(mMotion.mTracks[0].m08 - 62.0f) < 1.1920929e-7f) {
        fn_80232B7C(this, 1000);
        fn_8023A8B4(lbl_8087F3C0, &m1610, 0, 0, m1674, mRot, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
        fn_800FAB80(lbl_8087F048, this, m638, m590, &m1674, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

// NONMATCHING: 94.9% - the original addresses the camera mode (0x4D8) through a base register (addi) while
// loading the default parameters; same open case as EnemyJill::fn_80330D74 (3 variants)
// inferred: state 9 (jump: the camera follows it with the parameters of mode 4)
void Em101ElderZesya::fn_8025922C() {
    Field_unk* field = lbl_8087F430;
    if (m55C == 6) {
        if (m560 == 14) {
            atn::Vector3 pos = mPos;
            pos.y += 15.0f;
            if (m164C == 0) {
                ElderCamera_unk* cam = &field->m4D8;
                ElderCamParam_unk param = field->m588;
                param.m08 = -0.43633232f;
                param.m04 = 0.7853982f;
                param.m00 = 15.0f;
                param.m10 = 8.0f;
                field->m8A0 = this;
                cam->Set_unk(4, param);
            }
            m1654 = mPos;
            m1650 = 1;
            m1654.y += 25.0f;
        }
    } else {
        if (m164C != 0) {
            field->m8A0 = 0;
            field->m4D8.mMode = 0;
            field->m97C.m00 = 0;
            m164C = 0;
        }
        m14F4 = 1;
        m58C = 6;
        m14C8 = 0;
    }
    fn_80139560();
}

void Em101ElderZesya::fn_8025941C() {
    m14C8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14D0 = 0;
    m1660 = 0;
    m58C = 13;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
    GetWarp_unk(&m1684, &m1690);
    CollResult_unk result;
    mPos = m1684;
    atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + m1684;
    if (fn_8004ED34(lbl_8087EE98, &result, &m1684, &to, 0x80000000, &mColli, 0) != 0) {
        mPos = result.m10;
    }
    mRot.y = m1690;
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m15F8, &mMotion);
}

// inferred: hit handling: in states 9, 12 and 13 the hits are ignored; while a barrier is up a hit on the
// barrier (m40 != 0) breaks it if the attack matches its kind
void Em101ElderZesya::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 12 || m58C == 13 || m58C == 9) {
        msg->m10 *= 0.0f;
        msg->m64 = 3;
        msg->m90 = 0;
        msg->m84 = 2;
        msg->m88 = msg->mData->mC4;
        msg->m68 = 0;
        return;
    }
    if (*(int*)&msg->m3C[4] != 0) {
        int index = m1680;
        Barrier_unk* barrier = &m151C[index];
        if (barrier->m04.mState != 0) {
            return;
        }
        if (msg->m44 == 0) {
            msg->mAttacker->vf7C(1);
            msg->m64 = 4;
            msg->m90 = 0;
            msg->m68 = 0;
        } else if (msg->m44 == 2) {
            if (((float*)&msg->mData->m74)[barrier->m04.mKind] > 0.5f) {
                fn_80259AB4(index, 2);
                m15F4 = 1;
                m15EC = 0;
                if (fn_80370A78(lbl_8087F430, 200) == 0) {
                    fn_80370AE4(lbl_8087F430, 200, 1);
                }
            }
        }
        return;
    }
    CharaTask::vf3C(hit);
    m14C8 += 30;
    m14F0 += msg->m68;
    if ((float)m14F0 / (float)mStatus.mParamB.m84 > 0.1f) {
        m14F8 = 1;
        m14F0 = 0;
    }
    float hp = mStatus.m04;
    if (hp / (float)mStatus.mParamB.m84 < 0.1f) {
        fn_80370AE4(lbl_8087F430, 210, 1);
        for (unsigned int i = 0; i < 4; i++) {
            fn_80259AB4(i, 2);
        }
    }
    if (msg->m68 > 0) {
        fn_80102EAC(lbl_8087F048, m16A0, msg->mAttacker, 100, -1);
    }
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8, -1);
        case 20:
        reset:
            m58C = 0;
            m14C8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m14D0 = 0;
            m1660 = 0;
            break;
        }
    }
}

// inferred: names invented
inline int PartyCount_unk() {
    return lbl_8087F8A0->mCount;
}
inline atn::Vector3 PartyCenter_unk() {
    atn::Vector3 center = lbl_807C7030;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        center += p->mPos;
    }
    center *= 1.0f / (float)PartyCount_unk();
    return center;
}

// NONMATCHING: 98.8% - r3/r4 of the vector copies (the zero vector and the chosen point; __VecCopy family,
// docs/mwcc-blockers-q3.md) and the order of the two increments of the loop
// inferred: picks the chant point nearest to the center of the party (m1694)
void Em101ElderZesya::fn_802598DC() {
    atn::Vector3 center = lbl_807C7030;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        center += p->mPos;
    }
    center *= 1.0f / (float)PartyCount_unk();
    int best = -1;
    float dist = 1e16f;
    for (unsigned int i = 0; i < m14BC.mSize; i++) {
        atn::Vector3 d = center - m14BC.mData[i];
        float len = sqrt(d.x * d.x + d.z * d.z);
        if (dist > len) {
            dist = len;
            best = i;
        }
    }
    m1694 = m14BC.mData[best];
}

// inferred: plays a barrier effect with the color of its kind (name invented)
inline void Em101ElderZesya::PlayBarrier_unk(atn::EfpHandle_unk* efp, const atn::Vector3& pos, const atn::Vector3& rot,
                                             int kind) {
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, pos, rot, *(const Color4f_unk*)&(&m16C0)[kind], -1, -1, 1, 1.5f);
}

// inferred: same with the manager read first (name invented)
inline void Em101ElderZesya::PlayBarrierM_unk(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, const atn::Vector3& pos,
                                              const atn::Vector3& rot, int kind) {
    fn_8023A8B4(mgr, efp, 0, 0, pos, rot, *(const Color4f_unk*)&(&m16C0)[kind], -1, -1, 1, 1.5f);
}

// NONMATCHING: 98.70% - the argument scheduling of the second and third barrier effects (r4/r5/r7/r8; 4
// variants); the dead `b` of the look point flag comes from an if/else in ElderLook_unk::Set_unk
// inferred: changes the state of a barrier (0 up, 1 hit, 2 broken) and plays its three effects
void Em101ElderZesya::fn_80259AB4(int index, int state) {
    Barrier_unk::Colli_unk* c = &m151C[index].m04;
    int old = c->mState;
    c->mState = state;
    atn::Vector3 pos = c->mDesc->mPos;
    pos.y += -10.0f;
    atn::Vector3 rot(0.0f, c->mDesc->mRotY_unk, 0.0f);
    atn::Vector3 off(-sChantOffset_unk.x, 0.0f, -sChantOffset_unk.x);
    Mtx m;
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &off, &off);
    atn::Vector3 pos1 = pos + off;
    atn::Vector3 rot1(0.0f, c->mDesc->mRotY_unk - 1.5707964f, 0.0f);
    atn::Vector3 off2(sChantOffset_unk.x, 0.0f, -sChantOffset_unk.x);
    Mtx m2;
    PSMTXRotRad(m2, 'y', rot.y);
    PSMTXMultVec(m2, &off2, &off2);
    atn::Vector3 pos2 = pos + off2;
    atn::Vector3 rot2(0.0f, 1.5707964f + c->mDesc->mRotY_unk, 0.0f);
    fn_80239DAC(lbl_8087F3C0, c, 0, 1);
    fn_80232B7C(c, 0);
    switch (state) {
    case 0:
        if (old == 1) {
            PlayBarrier_unk(&m1640, pos, rot, c->mKind);
            PlayBarrier_unk(&m1640, pos1, rot1, c->mKind);
            PlayBarrier_unk(&m1640, pos2, rot2, c->mKind);
        } else {
            EffectMgr_unk* mgr = lbl_8087F3C0;
            PlayBarrierM_unk(mgr, &m161C, pos, rot, c->mKind);
            EffectMgr_unk* mgr2 = lbl_8087F3C0;
            PlayBarrierM_unk(mgr2, &m161C, pos1, rot1, c->mKind);
            EffectMgr_unk* mgr3 = lbl_8087F3C0;
            PlayBarrierM_unk(mgr3, &m161C, pos2, rot2, c->mKind);
            Field_unk* field = lbl_8087F430;
            field->m97C.Set_unk(pos, false);
        }
        fn_80370AE4(lbl_8087F430, 0xCA, 1);
        if (c->mKind == 3) {
            fn_80370AE4(lbl_8087F430, 0xCB, 1);
        } else if (c->mKind == 0) {
            fn_80370AE4(lbl_8087F430, 0xCB, 2);
        } else if (c->mKind == 1) {
            fn_80370AE4(lbl_8087F430, 0xCB, 3);
        } else if (c->mKind == 4) {
            fn_80370AE4(lbl_8087F430, 0xCB, 4);
        }
        break;
    case 1:
        PlayBarrier_unk(&m1628, pos, rot, c->mKind);
        PlayBarrier_unk(&m1628, pos1, rot1, c->mKind);
        PlayBarrier_unk(&m1628, pos2, rot2, c->mKind);
        break;
    case 2:
        PlayBarrier_unk(&m1634, pos, rot, c->mKind);
        PlayBarrier_unk(&m1634, pos1, rot1, c->mKind);
        PlayBarrier_unk(&m1634, pos2, rot2, c->mKind);
        break;
    }
}
