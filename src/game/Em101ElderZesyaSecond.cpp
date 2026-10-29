#include "game/Em101ElderZesyaSecond.h"
#include "game/HitInfo_unk.h"
#include "game/CharaStatus.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "chr/ControlBase.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: field (lbl_8087F430): camera shake at 0x96C (as in game/GimmickStateCommand.cpp) and route
// tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m000[0x96C];
    int m96C; // inferred: camera shake
    int m970;
    float m974;
    float m978;
    unsigned char m97C[0x10D8 - 0x97C];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8; // inferred: effect layer (set to 2 around some effects, as in game/Arms.cpp)
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
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
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
// inferred: constructor of chr::ControlMagicEnd (chr/ControlChant.cpp, 0x34 bytes; still in asm)
extern "C" chr::ControlBase* fn_801C02F4(void* self, CharaTask* chara);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct ElderMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" ElderMsgTable_unk* lbl_8087F1E4;
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void fn_800EE360(CharaTask* chara);     // inferred: BattleCharaTask (enemies only)
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
// inferred: skill effect manager (lbl_8087F9E8, as in gmk/HealPoint.cpp); nonzero while the magic is active
extern "C" void* lbl_8087F9E8;
extern "C" int fn_8059C6DC(void* mgr, HitInfo_unk* skill, CharaTask* owner);
// inferred: per-player weights of an effect (floats at +0x128, as in game/Em309Ghoul.cpp); names invented
struct ElderWeights_unk {
    unsigned char m00[0x128];
    float mWeight[4];
};
extern "C" ElderWeights_unk* fn_80103F60(Effect_unk* mgr, CharaTask* owner);
extern "C" int fn_80108378(ElderWeights_unk* w, CharaTask* chara); // inferred: index of `chara`, < 0 if missing
// inferred: current control while casting (as in game/Em101ElderZesya.cpp)
struct ControlMagic_unk {
    unsigned char m00[0x1D];
    unsigned char m1D; // inferred: nonzero once the magic is fired
};

// inferred: sound effects of the barrier
static const char* const sSounds_unk[] = {"SE_SP007_001", "SE_SP007_003"};

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

// NONMATCHING: 99.74% - r26/r27 of the second std::string, r25/r26 of the strlen of the parameters and of
// the old m121C (same as Em101LittleZesya / Em101ElderZesya / Em201ZanGurg)
Em101ElderZesyaSecond::Em101ElderZesyaSecond(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14CC(0), m14D0(0), m14D4(0), m14D8(0), m14DC(0), m14E0(0), m1504(0),
      m1554(0), m15CC(1), m15D0(1), m15D4(0), m15D8(0), m15DC(0), m15E4(0) {
    mFlag22 = 1;
    m14BC = 1e16f;
    m14C0.clear();
    m14B8 = 0;
    std::string dir("database/chara/special/");
    std::string name("db_em101_03_00.csv");
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
    m1518.fn_8023780C("cm002_00.efp");
    m1524.fn_8023780C("ef_sk026a.efp");
    m1530.fn_8023780C("ef_bs007a.efp");
    m153C.fn_8023780C("ef_bs066e.efp");
    m155C.fn_8023780C("ef_bs067a.efp");
    m1568.fn_8023780C("ef_bs067b.efp");
    m1574.fn_8023780C("ef_ui002b.efp");
    m121C.reset(new (1, "", "", 0) atn::EfpFile());
    m121C->fn_8023780C("ef_bu013.efp");
    m14FC[0] = 0x5E6;
    mFlagB5 = 1;
    m14FC[1] = 0x5EA;
    mFlagB11 = 1;
}

Em101ElderZesyaSecond::~Em101ElderZesyaSecond() {}

// NONMATCHING: 92.8% - the inlined std::vector push_back of the warp points (known open case: registers
// of the growth path and an extra saved register)
int Em101ElderZesyaSecond::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m14B0.fn_80473F50() == 0 &&
        m1518.fn_80237874() == 0 && m153C.fn_80237874() == 0 && m155C.fn_80237874() == 0 &&
        m1568.fn_80237874() == 0 && m1574.fn_80237874() == 0 && m1530.fn_80237874() == 0) {
        if (m14B0.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "warp_count") == 0) {
                    m14B8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "warp_count_dist") == 0) {
                    m14BC = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "warp_point") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m14C0.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "center_warp_point") == 0) {
                    m14CC = lbl_8087F430->m10D8->FindPlace_unk(atoi(csv.fn_8005B9CC()));
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        m1580.mSphere = atn::ColliSphere::Sphere_unk(atn::Vector3(0.0f, -10000.0f, 0.0f), 20.0f);
        m1580.mFlags |= 3;
        m1580.m0C = (int)this;
        m1580.m20 = 0x80;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// inferred: same as PlayEffect_unk with the manager as a parameter (name invented)
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

// inferred: plays a colored effect on the model (name invented)
inline void PlayColorEffect_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, const Color4f_unk& color) {
    fn_8023A8B4(mgr, efp, model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetTrans_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: number of players within `dist` (name invented; as an inline the FPRs f28-f31 match)
inline int Em101ElderZesyaSecond::CountNear_unk(float dist) {
    int count = 0;
    atn::Vector3 pos = mPos;
    PlayerTask* p = lbl_8087F8A0->mHead;
    for (; p != 0; p = p->mNext) {
        if (!p->mStatus.IsState_unk(0x20)) {
            atn::Vector3 d = p->mPos - pos;
            if (fn_805F9920(&d) < dist * dist) {
                count++;
            }
        }
    }
    return count;
}

// NONMATCHING: 99.84% - only the stack offsets of the temporaries (loop vectors, colors, sphere and ptmf
// copy); registers and instructions match
void Em101ElderZesyaSecond::vf10() {
    atn::ColliSphere::Sphere_unk sphere;
    m14D0 = mC58.mC4;
    if (m14D0 == 0) {
        m14D0 = lbl_8087F8A0->mHead;
        mC58.mC4 = m14D0;
    }
    m14D8++;
    if (m58C != 10 && m58C != 11) {
        m1504 += CountNear_unk(m14BC);
    }
    if (m15D8 > 2 || fn_80370A78(lbl_8087F430, 0xE8) == 1) {
        m14DC = 1;
    }
    if (mC58.mC0 == 0 || m14D0 == 0) {
        fn_8016E970(3);
    } else {
        switch (m58C) {
        case 2: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag25 = 1;
                mColli.mFlags &= ~1;
                mFlag15 = 1;
                fn_801765D8();
                fn_800EE360(this);
            }
            break;
        }
        case 7:
            fn_8025BD1C();
            break;
        case 8:
            fn_8025BF58();
            break;
        case 10: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                fn_8025C460();
            }
            break;
        }
        case 11: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                if (m14DC == 1 && m1554 == 0) {
                    m14D4 = 0;
                    m14D8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
                    m58C = 9;
                    fn_8016E970(3);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x6A, 0, 0, 1, 0.0f, 0.1f);
                } else {
                    m14D4 = 0;
                    m14D8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
                    m58C = 0;
                    fn_8016E970(3);
                }
            }
            break;
        }
        case 9: {
            float frame = mMotion.mTracks[0].m08;
            if ((float)__fabs(frame - 24.0f) < 1.1920929e-7f) {
                m1554 = 1;
                m1558 = 0;
                lbl_8087F3C0->mB8 = 2;
                fn_80232B7C(this, 1001);
                Color4f_unk color = m15D4 == 0 ? Color4f_unk(0.0f, 1.0f, 0.35f, 1.0f)
                                               : Color4f_unk(1.0f, 0.3f, 0.0f, 1.0f);
                PlayColorEffect_unk(lbl_8087F3C0, &m155C, &mMotion, color);
                lbl_8087F3C0->mB8 = 0;
                fn_800C344C(sSounds_unk[0], &mPos, 0, -1, 1.0f);
                int flag = m15D4 == 0 ? 0xE9 : 0xEA;
                if (fn_80370A78(lbl_8087F430, flag) == 0) {
                    fn_80370AE4(lbl_8087F430, flag, 1);
                }
                wchar_t text[256];
                memset(text, 0, sizeof(text));
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xA5), *(const wchar_t**)((char*)m60 + 4));
                fn_80109828(lbl_8087F048, text);
            } else if (frame >= mMotion.fn_80097D7C(0)) {
                mFB8 = 0.0f;
                m14D4 = 0;
                m14D8 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
                m58C = 0;
                fn_8016E970(3);
            }
            break;
        }
        default:
            if (m55C == 6) {
                if (m560 == 30 && mControl && ((ControlMagic_unk*)mControl.get())->m1D != 0) {
                    if (fn_80370A78(lbl_8087F430, 0xEC) == 0) {
                        fn_80370AE4(lbl_8087F430, 0xEC, 1);
                    }
                }
            } else {
                fn_8016E970(3);
            }
            fn_8025B9EC();
            if (m55C != 6) {
                if (m1504 >= m14B8) {
                    m1504 = 0;
                    m14D4 = 0;
                    m14D8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
                    m58C = 10;
                    fn_8016E970(3);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
                    fn_80232B7C(this, 101);
                    PlayEffectM_unk(lbl_8087F3C0, &m1530, &mMotion);
                } else if (m14D0 != 0) {
                    int kind = 1;
                    void* mgr = lbl_8087F9E8;
                    if (fn_8059C6DC(mgr, fn_80219E6C(m14FC[1]), this) != 0) {
                        kind = 0;
                    }
                    fn_8025C29C(kind);
                }
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (p->mType != 0 && p->CanAct_unk()) {
            ElderWeights_unk* w = fn_80103F60(lbl_8087F048, p);
            if (w != 0) {
                int i = fn_80108378(w, this);
                if (i >= 0) {
                    w->mWeight[i] = 100.0f;
                }
            }
        }
    }
    sphere.mRadius = 20.0f;
    if (m1554 != 0) {
        atn::Vector3 c = GetTrans_unk(mMotion.GetNodeMtx_unk("spine"));
        sphere.mCenter = c;
    } else {
        atn::Vector3 c(0.0f, -10000.0f, 0.0f);
        sphere.mCenter = c;
    }
    m1580.mSphere = sphere;
}

void Em101ElderZesyaSecond::vf14() {
    fn_80149A30();
}

void Em101ElderZesyaSecond::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C - 10U <= 1) {
        msg->m90 = 1;
        msg->m84 = 2;
        msg->m68 = 0;
        msg->m8C = 0;
        return;
    }
    if (m1554 != 0) {
        if (msg->mAttacker != 0 && msg->mData != 0) {
            int ok = 0;
            if (m15D4 == 0 && msg->mData->m74.m0C > 0.0f) {
                ok = 1;
            }
            if (m15D4 == 1 && msg->mData->m74.m00 > 0.0f) {
                ok = 1;
            }
            if (ok != 0) {
                if (m15D0 != 0) {
                    if (fn_80370A78(lbl_8087F430, 0xE6) == 0) {
                        fn_80370AE4(lbl_8087F430, 0xE6, 1);
                    }
                    m15D0 = 0;
                }
                fn_8025C624();
                return;
            }
        }
        if (m15CC != 0) {
            fn_80370AE4(lbl_8087F430, 0xE7, 1);
            m15CC = 0;
        }
        msg->m64 = 1;
        msg->m90 = 1;
        msg->m84 = 1;
        msg->m68 = 0;
        msg->m8C = 0;
        CharaTask* attacker = msg->mAttacker;
        if (attacker != 0 && msg->m44 == 0 && attacker->m55C == 6 && attacker->m560 - 4U <= 1) {
            attacker->vf7C(0);
        }
        return;
    }
    if (msg->mAttacker != 0 && msg->mAttacker->mType != 0) {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em101ElderZesyaSecond::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m15D8++;
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8, -1);
        reset:
            m58C = 0;
            m14D4 = 0;
            m14D8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
            m1504 = m14B8;
            break;
        }
        if (m560 == 30 && msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
            fn_8015E4B0(msg->m28 * 2.0f, -1);
        }
    }
    if (mStatus.IsState_unk(0x20)) {
        if (fn_80370A78(lbl_8087F430, 0x26) > 0 && msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
            vf48();
            fn_80178A6C();
            fn_8015495C(0, 1, 1, 1);
            fn_8016DA4C();
            m58C = 2;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
            fn_800EB7A0(this);
        } else {
            mStatus.fn_8013322C(0x20);
            mStatus.m04 = 1.0f;
            fn_8015E4B0(msg->m28 * 2.0f, -1);
        }
    }
}

void Em101ElderZesyaSecond::fn_8025C624() {
    m1554 = 0;
    fn_80239DAC(lbl_8087F3C0, this, 1001, 0);
    fn_80239DAC(lbl_8087F3C0, this, 1002, 0);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1574, &mMotion);
    fn_800C344C(sSounds_unk[1], &mPos, 0, -1, 1.0f);
    Field_unk* field = lbl_8087F430;
    field->m96C %= 2;
    field->m970 = 5;
    field->m974 = 1.0f;
    field->m978 = 2.0f;
    m15DC++;
    if (m15DC < 2) {
        m15D4 = 0;
    } else if (m15DC < 4) {
        m15D4 = 1;
    } else {
        m15D4 = (m15D4 + 1) % 2;
    }
}

void Em101ElderZesyaSecond::fn_8025B9EC() {
    float dist = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    atn::Vector3 target = m14D0->mPos;
    atn::Vector3 d = target - mPos;
    PSVECMag(&d);
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
    fn_8013CB68(&rot, 1, dist, m568 * rate);
}

void Em101ElderZesyaSecond::fn_8025BD1C() {
    int done = 0;
    HitInfo_unk* hit = m638 = fn_80219E6C(m14FC[m14E0]);
    mFBC = (float)hit->mC0;
    if (mFB8 < (float)hit->mC0) {
        mFB8 += 1.0f;
        if (mFB8 == (float)hit->mC0) {
            done = 1;
        }
    } else {
        done = 1;
    }
    float limit = 12.0f;
    if (mPos.y - m14F0.y < limit) {
        mPos.y += 0.08f;
    }
    if (done != 0) {
        mFB8 = 0.0f;
        mF7C = m14D0;
        mF6C = m14D0->mPos;
        if (m14E0 == 0) {
            m14D4 = 0;
            m14D8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
            m58C = 8;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x6B, 0, 0, 1, 0.0f, 0.1f);
        } else {
            m14D4 = 0;
            m14D8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
            m58C = 0;
            fn_8016E970(3);
            if (m14CC != 0) {
                mF6C = m14CC->mPos;
                fn_801603CC(1.0f, true);
            } else {
                fn_801603CC(1.0f, false);
            }
        }
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
    }
}

void Em101ElderZesyaSecond::fn_8025BF58() {
    m638 = fn_80219E6C(m14FC[m14E0]);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
        mFB8 = 0.0f;
        m14D4 = 0;
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
        m58C = 0;
        fn_8016E970(3);
        void* p = ::operator new(0x34, 0, "", "", 0);
        chr::ControlBase* c = (chr::ControlBase*)p;
        if (p != 0) {
            c = fn_801C02F4(p, this);
        }
        fn_80178208(c);
        return;
    }
    if ((float)__fabs(mMotion.mTracks[0].m08 - 12.0f) < 1.1920929e-7f) {
        m14E4 = m14D0->mPos;
        atn::Vector3 from = m14E4 + atn::Vector3(0.0f, 1.0f, 0.0f);
        atn::Vector3 to = m14E4 + atn::Vector3(0.0f, -100.0f, 0.0f);
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0) {
            m14E4 = result.m10;
        }
        m14E4 += atn::Vector3(0.0f, 0.0f, 1.0f);
        fn_80232B7C(this, 1000);
        fn_8023A8B4(lbl_8087F3C0, &m153C, 0, 0, m14E4, mRot, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
        fn_800C31F4("SE_SP016_001_2", 0, -1, 1.0f);
    } else if ((float)__fabs(mMotion.mTracks[0].m08 - 62.0f) < 1.1920929e-7f) {
        wchar_t text[256];
        memset(text, 0, sizeof(text));
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xA3), *(const wchar_t**)((char*)m60 + 4));
        fn_80109828(lbl_8087F048, text);
        fn_800FAB80(lbl_8087F048, this, m638, m590, &m14E4, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

void Em101ElderZesyaSecond::fn_8025C29C(int kind) {
    m14D4 = 0;
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    m58C = 7;
    fn_8016E970(6);
    m560 = 29;
    m14E0 = kind;
    m638 = fn_80219E6C(m14FC[kind]);
    m14F0 = mPos;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x66, 1, 0, 1, 0.0f, 0.1f);
    mFB8 = 0.0f;
    fn_80232B7C(this, 0);
    if (m14E0 == 0) {
        PlayEffect_unk(&m1524, &mMotion);
    } else {
        PlayEffect_unk(&m1518, &mMotion);
    }
}

void Em101ElderZesyaSecond::fn_8025C460() {
    m14D4 = 0;
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    m58C = 11;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
    fn_8025C7A8(&m1508, &m1514);
    CollResult_unk result;
    mPos = m1508;
    atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + m1508;
    if (fn_8004ED34(lbl_8087EE98, &result, &m1508, &to, 0x80000000, &mColli, 0) != 0) {
        mPos = result.m10;
    }
    mRot.y = m1514;
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m1530, &mMotion);
}

// inferred: std::swap of two pointers (name invented)
template <class T>
inline void Swap_unk(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

int Em101ElderZesyaSecond::fn_8025C7A8(atn::Vector3* pos, float* rot) {
    if (m14C0.size() == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < m14C0.size() - 1; i++) {
        for (unsigned int j = m14C0.size() - 1; j > i; j--) {
            atn::Vector3 a = m14C0[j]->mPos - mPos;
            float da = fn_805F9920(&a);
            unsigned int k = j - 1;
            atn::Vector3 b = m14C0[k]->mPos - mPos;
            if (da > fn_805F9920(&b)) {
                Swap_unk(m14C0[j], m14C0[k]);
            }
        }
    }
    unsigned int n = m14C0.size() > 3 ? 3 : m14C0.size();
    int index = rand() % (int)n;
    *pos = m14C0[index]->mPos;
    *rot = m14C0[index]->m14;
    return 1;
}

int Em101ElderZesyaSecond::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (m1554 != 0 && hit != 0) {
        int ok = 0;
        if (m15D4 == 0 && hit->m74.m0C > 0.0f) {
            ok = 1;
        }
        if (m15D4 == 1 && hit->m74.m00 > 0.0f) {
            ok = 1;
        }
        if (ok == 0) {
            return 4;
        }
    }
    return 0;
}
