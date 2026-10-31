#include "game/Em209SmallGolem.h"
#include "math/Vec.h"
#include "game/GameUI.h"
#include "chr/Motion_unk.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>
#include "game/Input_unk.h"
#include "atn/DrawHelper.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
// inferred: character data by name (game/CharaTask.cpp declares it with an int id; same C symbol)
extern "C" chr::CharaData_unk* fn_8020A81C(const char* name);
// inferred: debug menu entries (as in game/Em101LittleZesya.cpp and game/NewCampPartsHint.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_80088FAC(void* group, const char* name, float* color, int arg3, int arg4, float min, float max,
                            float step);
// inferred: effect manager (as in game/CharaTask.cpp); mB8 is 2 while an effect of vf98 starts
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
    unsigned char mBC[0xC4 - 0xBC];
    int mC4; // inferred: set to 1 while the effect of the death sequence starts (vfEC)
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int a, int b); // inferred: stops the effects
// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: field (lbl_8087F430) and its event counters (as in gmk/Coffin.cpp)
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index); // inferred: reads a counter
// inferred: player manager (lbl_8087F8A0; first character at 0x48, as in game/CharaTask.cpp)
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
extern "C" GameUI* lbl_8087F490;
extern "C" void fn_803E3384(GameUI* ui);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg, int arg2, float volume); // inferred: plays a sound
extern "C" void* lbl_8087F0A8; // inferred: settings (pad at 0x48C)
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)
// inferred: hint of the HUD (GameUI::m764) built as a temporary (name invented)
struct GolemUIHint_unk : public GameUIElem1C_unk {
    GolemUIHint_unk(int a, int b, int c) {
        m00 = a;
        m04 = b;
        m08 = c;
        m0C = 0;
        m10 = 0;
        m14 = 0;
        m18 = 0;
    }
};

// inferred: view of Material_unk with the colors at 0x1C/0x20 (as LeoMaterial_unk of
// game/Em401_01_WhiteLeoPard.cpp; the shared header only names the one at 0x18)
struct GolemMaterial_unk {
    unsigned char m00[0x18];
    GXColor m18;
    GXColor m1C;
    GXColor m20;
};
// inferred: material color as four floats (as in game/Em401_01_WhiteLeoPard.cpp)
struct GolemMatColor_unk {
    GolemMatColor_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    float r, g, b, a;
};

// inferred: colors of the golem (static array of the file, built by the __sinit 0x802CFFC8)
struct GolemColor_unk {
    GolemColor_unk() {}
    GolemColor_unk(float r_, float g_, float b_, float a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    GolemColor_unk operator-(const GolemColor_unk& o) const {
        return GolemColor_unk(r - o.r, g - o.g, b - o.b, a - o.a);
    }
    GolemColor_unk operator*(float f) const { return GolemColor_unk(r * f, g * f, b * f, a * f); }
    GolemColor_unk operator+(const GolemColor_unk& o) const {
        return GolemColor_unk(r + o.r, g + o.g, b + o.b, a + o.a);
    }
    float r, g, b, a;
};
// inferred: names invented
inline GolemColor_unk GolemLerp_unk(const GolemColor_unk& a, const GolemColor_unk& b, float t) {
    return (b - a) * t + a;
}
static GolemColor_unk sColor0_unk(0.03f, 0.08f, 0.04f, 1.0f);
static GolemColor_unk sColor1_unk(0.14f, 0.56f, 0.7f, 1.0f);
static GolemColor_unk sColor2_unk(0.8f, 0.28f, 0.085f, 1.0f);

// inferred: sets flags of the parameters (weak copy 0x802660FC of game/Em201ZanGurgFinal.cpp); name invented
inline void SetParamFlag_unk(CharaParam_unk* param, unsigned int flag) {
    *(unsigned int*)((char*)param + 0x9C) |= flag;
}

// NONMATCHING: the original is over the inlining budget and calls the Vector3, color, ResHandle and
// parameter setters out of line, and the std::string, tokenizer and iterator members (same open case
// as LastCocoonChrysalis and CharaTask::vf2C, docs/inlining.md). Written for the order of the strings.
Em209SmallGolem::Em209SmallGolem(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m150C(0), m1510(0x407),
      m1514(0x406), m1518(10.0f), m151C(30.0f), m1520(60), m1524(150), m1540(0), m1544(0), m1548(0), m1549(0),
      m154C(0), m1554(0) {
    m14FC[0] = sColor0_unk.r;
    m14FC[1] = sColor0_unk.g;
    m14FC[2] = sColor0_unk.b;
    m14FC[3] = sColor0_unk.a;
    SetParamFlag_unk(&GetStatus_unk()->mParamB, 0x10);
    SetParamFlag_unk(&GetStatus_unk()->mParamB, 0x2);
    SetParamFlag_unk(&GetStatus_unk()->mParamB, 0x8);
    SetParamFlag_unk(&GetStatus_unk()->mParamB, 0x20);
    mStatus.m18 |= 0x40;
    std::string dir("database/chara/special/");
    std::string name("db_last_cocoon.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp((*it).c_str(), "db_name=", 8) == 0) {
            name = (*it).c_str() + 8;
        } else if (strcmp((*it).c_str(), "enemy_reboot") == 0) {
            m1549 = 1;
        }
    }
    m14B0.vf0C(fn_8006AFF8(dir, name).c_str());
    m1528.LoadRequest("/effect/ef_uc058.efp");
    m1534.LoadRequest("/effect/ef_uc051a.efp");
}

Em209SmallGolem::~Em209SmallGolem() {}

int Em209SmallGolem::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m14B0.fn_80473F50() == 0 &&
        m1528.IsLoading() == 0 && m1534.IsLoading() == 0) {
        void* group = m1550.Load("Em209SmallGolem");
        fn_80087994(group, "DebugDisp", &m1554, 0, 0);
        fn_80088FAC(group, "StoneColor", m14FC, 0, 0, 0.0f, 1.0f, 0.01f);
        m1540 = m5C;
        mStatus.m18 |= 0xC2D8;
        mStatus.m18 |= 0x3800000;
        if (m14B0.fn_80470580() != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "AttackSkill") == 0) {
                    m1510 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleSkill") == 0) {
                    m1514 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackDistance") == 0) {
                    m1518 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleDistance") == 0) {
                    m151C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackInterval") == 0) {
                    m1520 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleInterval") == 0) {
                    m1524 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "AIType") == 0) {
                    m150C = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "FriendsParam") == 0) {
                    m1544 = fn_8020A81C(csv.fn_8005B9CC());
                } else if (strcmp(key, "NoneRevive") == 0) {
                    m1548 = fn_800DC12C(csv.fn_8005B9CC()) == 0 ? 1 : 0;
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

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: starts state `state` (names invented)
inline void Em209SmallGolem::SetState_unk(int state) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}

// inferred: revival (state 7): the motion 0x14D and the hit counters reset (name invented)
inline void Em209SmallGolem::StartRevive_unk(CharaTask* by) {
    SetState_unk(7);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    vf98();
    mColli.mFlags |= 1;
    vf38(1);
    m154C = (int)by;
}

// NONMATCHING: 99.53% - stack slots of the HUD hint temporary and of the needle and state-9 temporaries (the
// original keeps them below the colors of the materials), r0/r3 of the counter of m14C0 and of the two
// loads of the skill call; 12 variants (helpers, temporaries, named colors, reversed arguments)
void Em209SmallGolem::vf10() {
    m14B8 = mC58.mC4;
    if ((mStatus.m0C & 0x20) == 0x20 && m1548 != 0 && m58C == 2 && fn_80370A78(lbl_8087F430, 0xC9) == 1) {
        CharaTask* player = lbl_8087F8A0->mHead;
        atn::Vector3 d = mPos - player->mPos;
        float dist = PSVECMag(&d);
        if (m1549 != 0 && fn_80370A78(lbl_8087F430, 0xC8) == 1) {
            StartRevive_unk(0);
        } else if (dist < 50.0f && player->fn_8016DDB0(0) && player->m55C != 6) {
            GameUI* ui = lbl_8087F490;
            if (ui != 0) {
                ui->m764 = GolemUIHint_unk(6, 0x14, -1);
            }
            if (((Input_unk*)((char*)lbl_8087F0A8 + 0x48C))->fn_801231D0(2) != 0) {
                StartRevive_unk(player);
                fn_800C31F4("SE_UICOM_011", 0, -1, 1.0f);
                if (lbl_8087F490 != 0) {
                    fn_803E3384(lbl_8087F490);
                }
            }
        }
    }
    m14C0++;
    if (mC58.mC0 == 0 || m14B8 == 0) {
        fn_8016E970(3);
        if (m58C == 7) {
            fn_802CF728();
        } else if (m58C == 6) {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
            }
            if ((float)__fabs(mMotion.mTracks[0].m08 - 28.0f) < 1.1920929e-7f) {
                atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightwepoint"));
                m14CC = (m14D8 - pos).GetNormalize();
                Effect_unk* mgr = lbl_8087F048;
                fn_800F8574(mgr, this, fn_80219E6C(m1514), &pos, &m14CC, 0, 0, 0, 0.0f, 1.0f);
            }
        } else if (m58C == 2) {
            vfEC();
        } else if (m58C == 8) {
            m58C = 0;
        } else {
            fn_802CF054();
        }
    } else if (m14B8 != 0) {
        switch (m58C) {
        case 6: {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    SetState_unk(0);
                }
                if ((float)__fabs(mMotion.mTracks[0].m08 - 28.0f) < 1.1920929e-7f) {
                    atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightwepoint"));
                    m14CC = (m14D8 - pos).GetNormalize();
                    Effect_unk* mgr = lbl_8087F048;
                    fn_800F8574(mgr, this, fn_80219E6C(m1514), &pos, &m14CC, 0, 0, 0, 0.0f, 1.0f);
                }
            }
            break;
        case 7:
            fn_802CF728();
            break;
        case 8: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
            }
            float f = mMotion.mTracks[0].m08;
            if (f >= 17.0f && f <= 19.0f) {
                fn_80144710();
                fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(m1510), m590, 30, -1, 0.0f);
            }
            if (m1554 != 0) {
                HitInfo_unk* skill = fn_80219E6C(m1510);
                lbl_8087EEB0->fn_80063D3C(mPos, mStatus.mParamB.m28 + skill->m40, 0.0f, 0xFFFF0000);
            }
            break;
        }
        case 9:
            if (m14C0 >= 60) {
                mPos = m14F0;
                SetState_unk(0);
            } else {
                float t = (float)m14C0 / 60.0f;
                mPos = (m14F0 - m14E4) * t + m14E4;
            }
            break;
        case 10:
            if (m14C0 > 1) {
                SetState_unk(0);
            }
            break;
        case 2:
            vfEC();
            break;
        default:
            if (m55C != 6) {
                if (m150C == 0) {
                    fn_8016E970(3);
                    fn_80170A20(m14B8, 0, -1.0f);
                } else if (m150C == 1) {
                    fn_8016E970(9);
                }
            }
            fn_802CF054();
            if (m55C != 6) {
                fn_802CF3AC();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    {
        GolemMaterial_unk* material = (GolemMaterial_unk*)mMotion.fn_80092954("em209_core");
        if (material != 0) {
            mMotion.fn_80094958("em209_core", m14FC, &GolemMatColor_unk(material->m20),
                                &GolemMatColor_unk(material->m1C));
        }
    }
    {
        GolemMaterial_unk* material = (GolemMaterial_unk*)mMotion.fn_80092954("em209_core_light");
        if (material != 0) {
            mMotion.fn_80094958("em209_core_light", m14FC, &GolemMatColor_unk(material->m20),
                                &GolemMatColor_unk(material->m1C));
        }
    }
    if (CanAct_unk() && (m54C & 0x2000) != 0x2000 && mC58.mC0 != 0) {
        CharaTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (player != 0) {
            atn::Vector3 d = mPos - player->mPos;
            if (d.y < -400.0f) {
                vf44();
                m14BC = 0;
                m14C0 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                vf48();
                fn_80178A6C();
                fn_8015495C(0, 1, 1, 1);
                fn_8016DA4C();
                m58C = 2;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
                m574 = lbl_807C7030;
                m1434 = 30;
                vf9C(1);
            }
        }
    }
}

void Em209SmallGolem::vf14() {
    fn_80149A30();
}

void Em209SmallGolem::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 7) {
        msg->m68 = 0;
        msg->m6C = 0;
        msg->m70 = 0;
        msg->m74 = 0;
        *(int*)&msg->m78[0] = -1;
        msg->mFlag31 = 0;
        msg->mFlag30 = 0;
        msg->mFlag29 = 0;
        msg->mFlag28 = 0;
        *(int*)&msg->m78[4] = -1;
        msg->m64 = 3;
        msg->m90 = 0;
        return;
    }
    if (m150C == 1) {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em209SmallGolem::vf40(HitInfo_unk* hit) {
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 0;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        m14BC = 0;
        m14C0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        vf48();
        fn_80178A6C();
        fn_8015495C(0, 1, 1, 1);
        fn_8016DA4C();
        m58C = 2;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
        m574 = lbl_807C7030;
        m1434 = 30;
        vf9C(1);
    }
}

// NONMATCHING: 92.73% - stack slots of pos/rot/color and of the safe-bool temporary of m121C: same open case
// as CharaTask::vf98 (temporaries in the arguments give the opposite order; named locals move the ptmf)
// inferred: starts the effect m121C on the model and m1534 on the part in hand (CharaTask::vf98 starts only
// the first one)
void Em209SmallGolem::vf98() {
    if (mFlag18) {
        return;
    }
    if (m121C) {
        fn_80232B7C(m121C.get(), 0);
        lbl_8087F3C0->mB8 = 2;
        atn::Vector3 pos;
        atn::Vector3 rot;
        Color4f_unk color;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, m121C.get(), &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        lbl_8087F3C0->mB8 = 0;
    }
    if (m648 != 0) {
        fn_80232B7C(&m1534, 0);
        lbl_8087F3C0->mB8 = 2;
        atn::Vector3 pos;
        atn::Vector3 rot;
        Color4f_unk color;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m1534, (atn::Model*)m648->m010, 0, pos, rot, color, -1, -1, 1, 1.0f);
        lbl_8087F3C0->mB8 = 0;
    }
    mFlag18 = 1;
}

// inferred: stops the effects started by vf98
void Em209SmallGolem::vf9C(int arg) {
    if (!mFlag18) {
        return;
    }
    if (m121C) {
        fn_80239DAC(lbl_8087F3C0, m121C.get(), 0, arg);
    }
    fn_80239DAC(lbl_8087F3C0, &m1534, 0, arg);
    mFlag18 = 0;
}

void Em209SmallGolem::vf50() {
    m624.clear();
    Item14_unk item;
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += item.m04.mRadius;
    m624.push_back(item);
    mFlagB29 = 1;
}

// NONMATCHING: 98.87% - r4/r5 of the copy of the center (temporary of mPos + m5A4, __VecCopy family)
void Em209SmallGolem::vf54() {
    if (m624.mSize != 0) {
        m624.mData[0].m04.mRadius = m5B0;
        m624.mData[0].m04.mCenter = mPos + m5A4;
        m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
    }
}

// NONMATCHING: ~97% - only r3/r5 swapped for the two psq addresses (m14E4 / temporary); 8 variants
void Em209SmallGolem::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 9;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    m14E4 = pos;
    m14F0 = atn::Vector3(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
    mPos = pos;
}

void Em209SmallGolem::vf84(int motion, float blend) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 10;
    fn_8016E970(3);
}

// NONMATCHING: 98.49% - f6-f9 of the radius and the base position (8 variants: order of radius/m614,
// operands of 0.5f, a helper for the max, separate copies)
void Em209SmallGolem::vf20() {
    float radius = m5B0;
    m614 = mPos + m5A4;
    m620 = 0.5f * radius;
    m614.y += m620;
    atn::Vector3 a;
    atn::Vector3 b;
    b = a = mPos + m5A4;
    float h = m5B4 - 2.0f * radius;
    b.y += h > 1.0f ? h : 1.0f;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = radius;
}

void Em209SmallGolem::fn_802CF054() {
    float speed = 0.0f;
    float scale = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        atn::Vector3 d = m14B8->mPos - mPos;
        atn::Vector3 dir = d.GetNormalize();
        d.y = 0.0f;
        if (d.Length_unk() < 0.8f * m1518) {
            speed = 0.0f;
        } else {
            speed = dir.Length_unk();
        }
        rot = dir.ToRotation_unk();
    } else if (m55C == 9) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = scale;
        mMotion.mTracks[0].m0C = scale;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, speed, 0.1f);
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(rot, 0, speed, m568 * scale);
}

// inferred: turns to the target on the ground plane (name invented)
inline void Em209SmallGolem::FaceTarget_unk() {
    atn::Vector3 d = m14B8->mPos - mPos;
    d.y = 0.0f;
    if (!d.IsZero_unk()) {
        d.Normalize_unk();
    }
    mRot = d.ToRotation_unk();
}



// inferred: chooses the next attack (state 8 close to the target, else the needles)
void Em209SmallGolem::fn_802CF3AC() {
    if (m14B8 == 0) {
        return;
    }
    if (m150C == 0) {
        if (m14C0 > m1520) {
            atn::Vector3 d = m14B8->mPos - mPos;
            d.y = 0.0f;
            float dist = d.Length_unk();
            if (dist < m1518) {
                m14BC = 0;
                m14C0 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 8;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
                FaceTarget_unk();
            } else if (dist > m151C && m14C0 > m1524) {
                fn_802CFB24();
            }
        }
    } else if (m150C == 1 && m14C0 > m1524) {
        fn_802CFB24();
    }
}

extern "C" void fn_801028E4(Effect_unk* mgr, int a, CharaTask* chara, int b); // inferred: revives the character

// inferred: turns back from stone (or to the color of a friend) and revives
void Em209SmallGolem::fn_802CF728() {
    bool own = true;
    if (m154C != 0 && ((CharaTask*)m154C)->mType != 2) {
        own = false;
    }
    float frame0 = mMotion.mTracks[0].m08;
    float t = frame0 / mMotion.fn_80097D7C(0);
    if (own) {
        *(GolemColor_unk*)m14FC = GolemLerp_unk(sColor0_unk, sColor2_unk, t);
    } else {
        *(GolemColor_unk*)m14FC = GolemLerp_unk(sColor0_unk, sColor1_unk, t);
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_801028E4(lbl_8087F048, 0, this, 0);
        fn_8015495C(0, 1, 1, 1);
        mC58.mC4 = 0;
        if (own) {
            mF18 = mF14;
            mF14 = 1;
        } else {
            mF18 = mF14;
            mF14 = 0;
            if (m1544 != 0) {
                fn_8017A300(m1544);
            }
        }
        mStatus.fn_8012D8B8();
        mStatus.m224 = mStatus.mParamB.m98;
        m14BC = 0;
        m14C0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 0;
        fn_8016E970(3);
    }
}

// inferred: plays the effect of the death sequence (locals in ascending order as in an inline; name invented)
inline void PlayDeathEffect_unk(atn::EfpFile* efp, atn::Model* model, int flag) {
    lbl_8087F3C0->mC4 = flag;
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, flag, 1.0f);
    lbl_8087F3C0->mC4 = 0;
}

void Em209SmallGolem::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m1434 > 0) {
            if (m1434 == 30) {
                fn_800EB7A0(this);
            } else if (m1434 == 15) {
                PlayDeathEffect_unk(&m1528, &mMotion, 1);
                vf9C(1);
            }
            m1434--;
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360();
        }
    }
}

extern "C" void fn_802CFAE4(Em209SmallGolem* golem) {
    golem->m14BC = 0;
    golem->m14C0 = 0;
    golem->m590 = fn_800F8548(lbl_8087F048);
}

// inferred: starts the needle attack (state 6) aimed at the hips of the target
void Em209SmallGolem::fn_802CFB24() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 d = m14B8->mPos - mPos;
    d.y = 0.0f;
    if (!d.IsZero_unk()) {
        d.Normalize_unk();
    }
    mRot = d.ToRotation_unk();
    MtxPtr hips = m14B8->mMotion.GetNodeMtx_unk("hips");
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    m14D8 = m14B8->mPos;
    if (hips != 0) {
        m14D8 = GetNodePos_unk(hips);
    }
}

// inferred: turns the node "head" (rotation around y by -50 degrees and around z by 180 degrees, keeping its
// translation) while m14C8 is 1
int Em209SmallGolem::vf1C(atn::Matrix3x4* mtx, ModelNodeDesc_unk* node) {
    if (m14C8 == 1 && strcmp(node->mName, "head") == 0) {
        atn::Matrix3x4 m;
        atn::Matrix3x4 r;
        PSMTXRotRad(r.m, 'y', -0.87266463f);
        m = r;
        Mtx z;
        PSMTXRotRad(z, 'z', 3.1415927f);
        PSMTXConcat(z, m.m, m.m);
        atn::Vector3 trans = mtx->GetTrans_unk();
        mtx->m[0][3] = 0.0f;
        mtx->m[1][3] = 0.0f;
        mtx->m[2][3] = 0.0f;
        atn::Matrix3x4 out;
        PSMTXConcat(mtx->m, m.m, out.m);
        *mtx = out;
        mtx->m[0][3] = trans.x;
        mtx->m[1][3] = trans.y;
        mtx->m[2][3] = trans.z;
        return 1;
    }
    return 0;
}
