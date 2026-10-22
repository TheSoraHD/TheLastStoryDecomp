#include "game/Em301LShade.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "std/new.h"
#include "game/FriendManager.h"
#include "game/PlayerTask.h"
#include "game/SkillEffectManager.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: field object (as in gmk/FlamingArrow.cpp)
// inferred: field object (lbl_8087F430): route tables (fn_8013C504, as in gmk/FlamingArrow.cpp) and a
// camera shake at 0x96C (names invented)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
    unsigned char m000[0x96C];
    int m96C;
    int m970;   // inferred: shake frames
    float m974; // inferred: shake strength
    float m978;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" Field_unk* fn_80121F00();
extern "C" CharaManager<EnemyTask>* fn_801A03E0(); // inferred: enemy manager
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct ShadeMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" ShadeMsgTable_unk* lbl_8087F1E4;
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects

extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80103600(Effect_unk* mgr, CharaTask* chara);
extern "C" int fn_8021A888(HitInfo_unk* hit);
extern "C" void fn_805991E0(Bullet_unk* bullet); // inferred: stops an effect of a skill
extern "C" SkillEffectManager* lbl_8087F9E8;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
// inferred: map data of the field (as in gmk/Cannonball.cpp)
struct MapInfo_unk {
    unsigned char m00[0x4C];
    int m4C; // inferred: area
    unsigned char m50[0xE0 - 0x50];
    int mE0; // inferred: BGM kind
    unsigned char mE4[0xE8 - 0xE4];
    int mE8;
};
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: collision world and its ray test (as in game/Em305VampireMp.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: hit position
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

// inferred: plays an effect on the model (same as in game/Em305Vampire.cpp; names invented)
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

// inferred: same, with the locals declared in the other order (as in game/Em003LesserDaemon.cpp)
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

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

extern "C" int rand();
extern "C" void fn_804A04AC(MapInfo_unk* map, int kind, int arg2, int arg3, int arg4); // inferred: map BGM
// inferred: battle state (fn_80103F60 returns a record per party member; m128: weights)
struct BattleSlots_unk {
    unsigned char m000[0x128];
    float m128[1];
};
extern "C" BattleSlots_unk* fn_80103F60(Effect_unk* mgr, CharaTask* chara);
extern "C" int fn_80108378(BattleSlots_unk* slots, CharaTask* chara);

// inferred: float color converted from/to GXColor (as in game/GimmickCommon.cpp); names invented
struct ShadeColor_unk {
    float r, g, b, a;
};
inline void ToColorF_unk(ShadeColor_unk* c, GXColor gx) {
    c->r = gx.r / 255.0f;
    c->g = gx.g / 255.0f;
    c->b = gx.b / 255.0f;
    c->a = gx.a / 255.0f;
}
// inferred: second color of the material (0x1C), read with a cast to keep include/atn/Model_unk.h unchanged
inline GXColor& MaterialColor2_unk(Material_unk* m) {
    return *(GXColor*)&m->m1C[0];
}
inline GXColor ToGX_unk(const ShadeColor_unk& c) {
    GXColor gx;
    gx.r = (unsigned char)(255.0f * c.r);
    gx.g = (unsigned char)(255.0f * c.g);
    gx.b = (unsigned char)(255.0f * c.b);
    gx.a = (unsigned char)(255.0f * c.a);
    return gx;
}

// inferred: link of the character lists of the managers (CharaTask + 0x14AC, as in game/CharaTask.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}

// NONMATCHING: draft - std::string constructors and the inlined Vector_unk/vector code
Em301LShade::Em301LShade(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14D8(0), m14DC(0), m14E0(1), m14E4(0), m14E8(0), m14EC(0),
      m14F0(10), m14F4(0), m14F8(900), m14FC(0), m1500(0), m1504(0), m1508(0), m150C(0), m152C(1), m1530(0x451),
      m1534(0x450), m1538(0), m1560(0), m1564(0.5f), m1568(0.1f), m156C(0.05f), m1570(0.0f), m1574(0.0f),
      m1578(0.0f), m15B8(0), m15BC(2.0f), m15C0(45.0f), m15C4(33.0f), m15C8(0), m15E8(0), m15EC(0), m1620(0),
      m1624(1), m1628(0), m16AC(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    std::string dir("database/chara/special/");
    std::string name("db_em301_00_00.csv");
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
    m1650.vf0C(path.c_str());
    m1514.fn_8023780C("/effect/ef_sk006a.efp");
    m1520.fn_8023780C("/effect/ef_sk006c.efp");
    m1548.fn_8023780C("/effect/ef_sk012a.efp");
    m1554.fn_8023780C("/effect/ef_sk012c.efp");
    m157C.fn_80237654("/effect/ef_mb001b.eff");
    m1588.fn_8023780C("/effect/ef_sk013a.efp");
    m1594.fn_8023780C("/effect/ef_sk013d.efp");
    m15A0.fn_8023780C("/effect/ef_sk013c.efp");
    m15CC.fn_8023780C("/effect/ef_uc020d.efp");
    m15D8.fn_8023780C("/effect/ef_uc020a.efp");
    m1608.fn_8023780C("/effect/ef_uc015.efp");
    m1614.fn_8023780C("ef_bd013.efp");
    m15F0[0].fn_8023780C("ef_uc063.efp");
    m15F0[1].fn_8023780C("ef_uc064.efp");
    m15AC.clear();
    m162C.clear();
    m1638.clear();
    m1644.clear();
}

// NONMATCHING: 0% - inline budget: the original calls every inline candidate out of line (Csv ctor,
// FindPlace_unk, the three push_back, GetNodeMtx_unk, IsSet_unk), ours inlines them all. With an extra
// candidate of about 50 statements the body comes out at 99.16% (only the names of the push_back weak
// copies, fn_80244CCC/fn_80244FC0, differ), so the original has more inline code somewhere (open case,
// as LoadSeTableTask::vf0C in docs/inlining.md). Tried: the reading in an inline helper.
int Em301LShade::vf0C() {
    if (fn_8013655C() == 0 && m1514.fn_80237874() == 0 && m1520.fn_80237874() == 0 &&
        m1548.fn_80237874() == 0 && m1554.fn_80237874() == 0 && m157C.fn_802376D0() == 0 &&
        m1588.fn_80237874() == 0 && m1594.fn_80237874() == 0 && m15A0.fn_80237874() == 0 &&
        m15CC.fn_80237874() == 0 && m15D8.fn_80237874() == 0 && m1608.fn_80237874() == 0 &&
        m1614.fn_80237874() == 0 && m15F0[0].fn_80237874() == 0 && m15F0[1].fn_80237874() == 0 &&
        m1650.fn_80473F50() == 0) {
    if (m1650.fn_80470580() != 0 && fn_80121F00() != 0 && fn_80121F00()->fn_8013C504() != 0) {
        atn::Csv csv((const char*)m1650.fn_80470580(), m1650.fn_8047059C());
        do {
            const char* key = csv.fn_8005B9CC();
            if (strcmp(key, "appear_point") == 0) {
                int id;
                do {
                    id = fn_800DC12C(csv.fn_8005B9CC());
                    RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                    if (place != 0) {
                        m162C.push_back(place);
                    }
                } while (id != 0);
            } else if (strcmp(key, "first_appear_number") == 0) {
                m1628 = fn_800DC12C(csv.fn_8005B9CC());
            } else if (strcmp(key, "reinforce_point") == 0) {
                int id;
                do {
                    id = fn_800DC12C(csv.fn_8005B9CC());
                    RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                    if (place != 0) {
                        m1638.push_back(place);
                    }
                } while (id != 0);
            } else if (strcmp(key, "reinforce_chara") == 0) {
                int id;
                do {
                    id = fn_800DC12C(csv.fn_8005B9CC());
                    EnemyTask* enemy = fn_801A03E0()->Find_unk(id);
                    if (enemy != 0) {
                        m1644.push_back(enemy);
                    }
                } while (id != 0);
            } else if (strcmp(key, "absorb_rate") == 0) {
                m1564 = fn_800DC288(csv.fn_8005B9CC());
            } else if (strcmp(key, "absorb_subtract_rate") == 0) {
                m1568 = fn_800DC288(csv.fn_8005B9CC());
            } else if (strcmp(key, "absorb_min_rate") == 0) {
                m156C = fn_800DC288(csv.fn_8005B9CC());
            } else if (strcmp(key, "swing_skill_id") == 0) {
                m1530 = fn_800DC12C(csv.fn_8005B9CC());
            } else if (strcmp(key, "shades_skill_id") == 0) {
                m1534 = fn_800DC12C(csv.fn_8005B9CC());
            } else if (strcmp(key, "damaged_count_max") == 0) {
                m14F0 = fn_800DC12C(csv.fn_8005B9CC());
            } else if (strcmp(key, "head_objectlock_notify_flag") == 0) {
                fn_805A4980((SpecialFlag_unk*)&m1658.mFlag, &csv);
            } else if (strcmp(key, "disappear_notify_flag") == 0) {
                fn_805A4980((SpecialFlag_unk*)&m1678, &csv);
            } else if (strcmp(key, "hit_magic_notify_flag") == 0) {
                fn_805A4980((SpecialFlag_unk*)&m1680, &csv);
            } else if (strcmp(key, "attack_miss_notify_flag") == 0) {
                fn_805A4980((SpecialFlag_unk*)&m1688, &csv);
            } else if (strcmp(key, "friend_shade_notify_flag") == 0) {
                fn_805A4980((SpecialFlag_unk*)&m1690, &csv);
            } else if (strcmp(key, "absorb_notify_flag") == 0) {
                fn_805A4980((SpecialFlag_unk*)&m1698, &csv);
            } else if (strcmp(key, "reinforce_notify_flag") == 0) {
                fn_805A4980((SpecialFlag_unk*)&m16A0, &csv);
            } else if (strcmp(key, "big_shade") == 0) {
                m14D4 = fn_800DC12C(csv.fn_8005B9CC());
            } else if (strcmp(key, "dead_type") == 0) {
                m15B8 = fn_800DC12C(csv.fn_8005B9CC());
            } else if (strcmp(key, "dead_camera") == 0) {
                m15BC = fn_800DC288(csv.fn_8005B9CC());
                m15C0 = fn_800DC288(csv.fn_8005B9CC());
                m15C4 = fn_800DC288(csv.fn_8005B9CC());
            }
        } while (csv.fn_8005BBF8());
    }
    mStatus.m18 |= 0x1C0;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC219;
    mStatus.m18 |= 0x3800000;
    mStatus.m18 |= 0x4;
    mStatus.m18 |= 0x80000;
    if (m1658.mFlag.IsSet_unk()) {
        m1658.mNode = mMotion.GetNodeMtx_unk("head");
        m1658.mRadius = 5.0f;
        m1658.mEnabled = 1;
    }
    m14D8 = m1628;
    return 1;
    }
    return 0;
}

// inferred: names invented
inline float MIN_unk(float a, float b) {
    return a < b ? a : b;
}
inline float MAX_unk(float a, float b) {
    return a > b ? a : b;
}
inline void Em301LShade::SetState_unk(int state) {
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}

inline void Em301LShade::ShowMessage_unk(int id) {
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(id), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

inline bool Em301LShade::IsMotionEnd_unk() {
    float frame = mMotion.mTracks[0].m08;
    return frame >= mMotion.fn_80097D7C(0);
}

// NONMATCHING: draft
void Em301LShade::vf10() {
    if (m58C != 8) {
        m1620 = mC58.mC4;
    }
    if (mC58.mC0 == 0 || mC58.mC4 == 0) {
        fn_8016E970(3);
        m14E8++;
        if (m58C == 7) {
            fn_802D4238();
        }
        if (m58C == 0 && m14E0 == 3) {
            fn_802D3370();
        }
    } else {
        int frame = ++m14E8;
        switch (m58C) {
        case 6:
            if (frame == 60) {
                vf98();
                m15E8 = 0;
                m15EC = 0;
                m15C8 = 0;
            }
            if (m14E8 > 90) {
                m14E8 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 0;
                fn_8016E970(3);
            }
            break;
        case 7:
            fn_802D4238();
            break;
        case 8:
            fn_802D4378();
            break;
        case 9:
            fn_802D4628();
            break;
        case 10: {
            HitInfo_unk* skill = fn_80219E6C(m1530);
            float end = mMotion.mTracks[0].m08;
            if (end >= mMotion.fn_80097D7C(0)) {
                m14E8 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 0;
                fn_8016E970(3);
            }
            if ((65.0f <= mMotion.mTracks[0].m08 && mMotion.mTracks[0].m08 <= 80.0f) ||
                (115.0f <= mMotion.mTracks[0].m08 && mMotion.mTracks[0].m08 <= 130.0f)) {
                fn_80144710();
                atn::Vector3 pos = mPos;
                pos.y += 13.0f * m540.x;
                fn_800FAB80(lbl_8087F048, this, skill, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            }
            break;
        }
        case 11:
            fn_802D4E9C();
            break;
        case 12:
            fn_802D5020();
            break;
        case 2:
            vfEC();
            break;
        default: {
            fn_8016E970(3);
            atn::Vector3 rot = mRot;
            float zero = 0.0f;
            float one = 1.0f;
            if (m55C == 6) {
                fn_8013A258();
            } else {
                fn_8013CB68(&rot, 0, zero, m568 * one);
            }
            fn_802D3370();
            break;
        }
        }
    }
    if (m14D4 != 0 && m1620 != 0) {
        MapInfo_unk* map = fn_80373148(lbl_8087F430);
        if (map != 0) {
            int kind = m14E0 != 3;
            if (kind != map->mE0) {
                fn_804A04AC(map, kind, 20, -1, 0);
                map->mE8 = kind;
            }
        }
    }
    m1500++;
    if (m15E8 > 0) {
        m15E8--;
        m15EC--;
        m15EC = m15EC < m15E8 ? m15EC : m15E8;
        if (m15E8 <= 0) {
            fn_80239DAC(lbl_8087F3C0, &m15F0[m15E4], 0, 1);
            vf98();
        }
        if (m15EC <= 0) {
            m1508 = 1;
        }
    {
        ShadeColor_unk c;
        ToColorF_unk(&c, mMotion.fn_80092954("em301_inner")->mColor);
        if (m15E4 == 0) {
            c.r = MIN_unk(1.0f, 0.02f + c.r);
        } else {
            c.r = MIN_unk(0.05f, 0.005f + c.r);
        }
        if (m15E4 == 0) {
            c.g = MIN_unk(0.5f, 0.01f + c.g);
        } else {
            c.g = MIN_unk(0.2f, 0.01f + c.g);
        }
        mMotion.fn_80092954("em301_inner")->mColor = ToGX_unk(c);
        ShadeColor_unk half;
        half.r = c.r * 0.5f;
        half.g = c.g * 0.5f;
        half.b = c.b * 0.5f;
        half.a = c.a * 0.5f;
        MaterialColor2_unk(mMotion.fn_80092954("em301_inner")) = ToGX_unk(half);
    }
    {
        ShadeColor_unk c;
        ToColorF_unk(&c, mMotion.fn_80092954("em301_outer")->mColor);
        if (m15E4 == 0) {
            c.r = MIN_unk(1.0f, 0.02f + c.r);
        } else {
            c.r = MIN_unk(0.05f, 0.005f + c.r);
        }
        if (m15E4 == 0) {
            c.g = MIN_unk(0.5f, 0.01f + c.g);
        } else {
            c.g = MIN_unk(0.2f, 0.01f + c.g);
        }
        mMotion.fn_80092954("em301_outer")->mColor = ToGX_unk(c);
        ShadeColor_unk half;
        half.r = c.r * 0.5f;
        half.g = c.g * 0.5f;
        half.b = c.b * 0.5f;
        half.a = c.a * 0.5f;
        MaterialColor2_unk(mMotion.fn_80092954("em301_outer")) = ToGX_unk(half);
    }
    } else if (m58C == 2) {
    {
        ShadeColor_unk c;
        ToColorF_unk(&c, mMotion.fn_80092954("em301_inner")->mColor);
        if (m15B8 == 0) {
            c.r = MIN_unk(1.0f, 0.02f + c.r);
        } else {
            c.r = MIN_unk(0.05f, 0.005f + c.r);
        }
        if (m15B8 == 0) {
            c.g = MIN_unk(0.5f, 0.01f + c.g);
        } else {
            c.g = MIN_unk(0.2f, 0.01f + c.g);
        }
        mMotion.fn_80092954("em301_inner")->mColor = ToGX_unk(c);
        ShadeColor_unk half;
        half.r = c.r * 0.5f;
        half.g = c.g * 0.5f;
        half.b = c.b * 0.5f;
        half.a = c.a * 0.5f;
        MaterialColor2_unk(mMotion.fn_80092954("em301_inner")) = ToGX_unk(half);
    }
    {
        ShadeColor_unk c;
        ToColorF_unk(&c, mMotion.fn_80092954("em301_outer")->mColor);
        if (m15B8 == 0) {
            c.r = MIN_unk(1.0f, 0.02f + c.r);
        } else {
            c.r = MIN_unk(0.05f, 0.005f + c.r);
        }
        if (m15B8 == 0) {
            c.g = MIN_unk(0.5f, 0.01f + c.g);
        } else {
            c.g = MIN_unk(0.2f, 0.01f + c.g);
        }
        mMotion.fn_80092954("em301_outer")->mColor = ToGX_unk(c);
        ShadeColor_unk half;
        half.r = c.r * 0.5f;
        half.g = c.g * 0.5f;
        half.b = c.b * 0.5f;
        half.a = c.a * 0.5f;
        MaterialColor2_unk(mMotion.fn_80092954("em301_outer")) = ToGX_unk(half);
    }
    } else {
    {
        ShadeColor_unk c;
        ToColorF_unk(&c, mMotion.fn_80092954("em301_inner")->mColor);
        if (m15E4 == 0) {
            c.r = MAX_unk(0.0f, c.r - 0.02f);
        } else {
            c.r = MAX_unk(0.0f, c.r - 0.005f);
        }
        if (m15E4 == 0) {
            c.g = MAX_unk(0.0f, c.g - 0.01f);
        } else {
            c.g = MAX_unk(0.0f, c.g - 0.01f);
        }
        mMotion.fn_80092954("em301_inner")->mColor = ToGX_unk(c);
        ShadeColor_unk half;
        half.r = c.r * 0.5f;
        half.g = c.g * 0.5f;
        half.b = c.b * 0.5f;
        half.a = c.a * 0.5f;
        MaterialColor2_unk(mMotion.fn_80092954("em301_inner")) = ToGX_unk(half);
    }
    {
        ShadeColor_unk c;
        ToColorF_unk(&c, mMotion.fn_80092954("em301_outer")->mColor);
        if (m15E4 == 0) {
            c.r = MAX_unk(0.0f, c.r - 0.02f);
        } else {
            c.r = MAX_unk(0.0f, c.r - 0.005f);
        }
        if (m15E4 == 0) {
            c.g = MAX_unk(0.0f, c.g - 0.01f);
        } else {
            c.g = MAX_unk(0.0f, c.g - 0.01f);
        }
        mMotion.fn_80092954("em301_outer")->mColor = ToGX_unk(c);
        ShadeColor_unk half;
        half.r = c.r * 0.5f;
        half.g = c.g * 0.5f;
        half.b = c.b * 0.5f;
        half.a = c.a * 0.5f;
        MaterialColor2_unk(mMotion.fn_80092954("em301_outer")) = ToGX_unk(half);
    }
        if (m58C != 6 && m58C != 7 && m58C != 9) {
            if (++m14F4 >= m14F8) {
                m1508 = 1;
            }
        }
    }
    if (m14D8 == -1) {
        mPos = lbl_807C7030;
        mPos.y -= 10000.0f;
    } else {
        mPos = m162C[m14D8]->mPos;
    }
    mPos.y -= 13.0f * m540.y;
    CharaTask* target = m1620;
    if (target != 0) {
        atn::Vector3 dir;
        dir.x = target->mPos.x - mPos.x;
        dir.z = target->mPos.z - mPos.z;
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 rot = dir.ToRotation_unk();
        float d = atn::WrapAngle_unk(rot.y - mRot.y);
        float step = 0.08726647f;
        if ((float)__fabs(d) > step) {
            if (d < 0.0f) {
                mRot.y -= step;
            } else {
                mRot.y += step;
            }
        } else {
            mRot.y = rot.y;
        }
    }
    fn_8014C540();
    fn_80145334();
    BattleSlots_unk* slots = fn_80103F60(lbl_8087F048, this);
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
        int i = fn_80108378(slots, c);
        if (c->fn_8016E484() == 0) {
            slots->m128[i] = 0.0f;
        } else {
            slots->m128[i] = 1.0f;
        }
    }
    if (fn_805A4A54((SpecialAim_unk*)&m1658)) {
        fn_805A4A1C((SpecialFlag_unk*)&m1658.mFlag, 1);
        m1658.mEnabled = 0;
    }
    m150C--;
}

void Em301LShade::vf14() {
    if (m15C8 != 0) {
        return;
    }
    fn_80149A30();
}

void Em301LShade::vf40(HitInfo_unk* hit) {
    m14EC++;
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802D57B8();
    }
}

void Em301LShade::vf108() {}
void Em301LShade::vf104() {}
void Em301LShade::vf100(int state) {}
void Em301LShade::vfFC() {}

void Em301LShade::vf20() {
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += m5B4 - 2.0f * m620;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m620;
}

void Em301LShade::vf50() {
    Item14_unk item;
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += 13.0f * m540.x + item.m04.mRadius;
    m624.push_back(item);
    MtxPtr head = mMotion.GetNodeMtx_unk("head");
    item.m04.mCenter = GetNodePos_unk(head) + m5A4;
    item.m04.mCenter.y += 3.0f;
    m624.push_back(item);
    mFlagB29 = 0;
}

void Em301LShade::vf54() {
    if (m624.mSize != 0) {
        Item14_unk* item = m624.mData;
        item->m04.mCenter = mPos + m5A4;
        m624.mData[0].m04.mCenter.y += 13.0f * m540.x + m624.mData[0].m04.mRadius;
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        m624.mData[1].m04.mCenter = GetNodePos_unk(head) + m5A4;
        m624.mData[1].m04.mCenter.y += 3.0f;
    }
}

atn::Vector3 Em301LShade::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center != 0) {
        result = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
    }
    return result;
}

int Em301LShade::fn_802D404C() {
    if (mC58.mC0 == 1) {
        for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
            c->mC58.mC4 = 0;
            c->mC58.mC8 = 0;
            fn_80103600(lbl_8087F048, c);
            if (fn_8021A888(c->m638)) {
                c->fn_80160170(c->m638, c->mC58.mC4);
            }
        }
    }
    mC58.mC0 = 0;
    m54C |= 0x2000;
    if (m14E4 == 0) {
        fn_80232B7C(this, 0x15);
        PlayEffectB_unk(&m15D8, (Model_unk*)&mMotion);
        SetState_unk(7);
        ShowMessage_unk(0x121);
    }
    if (m14E4 < 390 && fn_802D7058()) {
        m14E4 = 390;
    }
    if (m14E4 > 450) {
        mC58.mC0 = 1;
        m54C &= ~0x2000;
        m14E0 = 0;
        m14E4 = 0;
        return 0;
    }
    return 1;
}

void Em301LShade::fn_802D4238() {
    if (m14E8 == 30) {
        vf9C(1);
        fn_80239DAC(lbl_8087F3C0, m15F0, 0, 1);
        for (int i = 0; i < 32; i++) {
            Bullet_unk* bullet = lbl_8087F9E8->GetBullet_unk(i);
            if (bullet->IsActive_unk() && (bullet->m08 == 0 || bullet->m08->mType != 2)) {
                fn_805991E0(bullet);
            }
        }
        m15C8 = 1;
    }
    if (m14E8 > 60) {
        m14D8 = -1;
        SetState_unk(0);
        if (m1678.mGlobal >= 0) {
            fn_805A4A1C((SpecialFlag_unk*)&m1678, 1);
        }
    }
}

void Em301LShade::fn_802D4E9C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
    }
    if (m14E8 == 15) {
        Field_unk* field = lbl_8087F430;
        field->m96C = field->m96C % 2;
        field->m970 = 25;
        field->m974 = 0.2f;
        field->m978 = 0.2f;
    }
    if (m14E8 == 70) {
        Field_unk* field = lbl_8087F430;
        field->m96C = field->m96C % 2;
        field->m970 = 40;
        field->m974 = 1.0f;
        field->m978 = 1.0f;
        for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
            if ((c->mStatus.m0C & 0x20) != 0x20 && c->mC58.mC0 == 1) {
                c->fn_8016CDB8(40, 0);
            }
        }
        ShowMessage_unk(0x11B);
    }
}

void Em301LShade::fn_802D5020() {
    if (m14E8 == 55) {
        atn::Vector3 pos(0.0f, 13.0f * m540.x, 0.0f);
        fn_80232B7C(this, 0x65);
        Color4f_unk color;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m1514, (Model_unk*)&mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, 1.0f);
        ShowMessage_unk(0x11D);
    }
    if (m14E8 == 80) {
        for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
            fn_80232B7C(c, 1000);
            PlayEffect_unk(&m1520, (Model_unk*)&c->mMotion);
        }
    }
    if (m14E8 > 120) {
        if (m152C == 0) {
            fn_802D67F8((int)(3.0f * (1.0f - mStatus.m04 / (float)mStatus.mParamB.m84)));
            if (m1510 == 3 && m16A0.mGlobal >= 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m16A0, 1);
            }
        }
        fn_802D5D18(m1510);
        m152C = 0;
        SetState_unk(0);
    }
}

void Em301LShade::fn_802D5674(int count) {
    fn_80232B7C(this, 20);
    PlayEffect_unk(&m15CC, (Model_unk*)&mMotion);
    SetState_unk(6);
    if (count >= (int)m162C.size()) {
        m14D8 = m1628;
    } else {
        m14D8 = count;
    }
    ShowMessage_unk(0x11F);
    m1504 = 0;
    m1508 = 0;
    m15E8 = 0;
    m15EC = 0;
    m14F4 = 0;
}

// inferred: names invented
inline bool ShadeInactive_unk(CharaTask* c) {
    bool r = false;
    if ((c->mFlags & 4) == 4 || (c->mFlags & 1) == 1) {
        r = true;
    }
    return r;
}
inline bool ShadeGrabbed_unk(CharaTask* c) {
    bool r = false;
    if (c->m55C == 6 && c->m560 == 0x1C) {
        r = true;
    }
    return r;
}
inline bool ShadeFree_unk(CharaTask* c) {
    bool r = false;
    if (!ShadeInactive_unk(c) && (c->mStatus.m0C & 0x20) != 0x20 && !ShadeGrabbed_unk(c)) {
        r = true;
    }
    return r;
}

// NONMATCHING: 92.9% - the original rereads the status word for the 0x20 test inside the "free" check
// and keeps the character in r29; the random pick keeps the count in r26 (8 variants: helpers, single
// condition, nested ifs, CanAct_unk, list capacity)
// inferred: reinforcement kind 0 seals the first party member of kind 2, kind 1 seals one to three
// random members (sets the status flag 0x100)
int Em301LShade::fn_802D5D18(int kind) {
    int done = 0;
    switch (m1510) {
    case 0:
        for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
            if (fn_80219558(c->m50) == 2) {
                if ((c->mStatus.m0C & 0x40) != 0x40 && (c->mStatus.m0C & 0x100) != 0x100) {
                    c->mStatus.fn_8013310C(0x100, 0);
                    c->fn_80166D3C(1);
                    done = 1;
                }
                break;
            }
        }
        break;
    case 1: {
        FixedArray_unk<CharaTask*, 16> list;
        for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
            if ((c->mStatus.m0C & 0x100) == 0x100 || (c->mStatus.m0C & 0x40) == 0x40) {
                continue;
            }
            if (ShadeFree_unk(c) && c->fn_8016E484()) {
                if (m14D4 == 0 || fn_80219558(c->m50) == 2 || c->mStatus.m224 > 1) {
                    list.PushBack_unk(c);
                }
            }
        }
        int count = rand() % 3 + 1;
        if (m14D4 != 0) {
            count = rand() % 100 < 60;
        }
        for (int i = 0; i < count; i++) {
            if (list.mCount != 0) {
                unsigned int index = rand() % list.mCount;
                list.mData[index]->mStatus.fn_8013310C(0x100, 0);
                list.mData[index]->fn_80166D3C(1);
                list.erase(&list.mData[index]);
                done = 1;
            }
        }
        break;
    }
    }
    if (done && m1690.mGlobal >= 0) {
        fn_805A4A1C((SpecialFlag_unk*)&m1690, 1);
    }
    return done;
}

int Em301LShade::fn_802D7058() {
    for (unsigned int i = 0; i < m1644.size(); i++) {
        EnemyTask* enemy = m1644[i];
        if ((enemy->mStatus.m0C & 0x20) != 0x20 && enemy->mC58.mC0 == 1) {
            return 0;
        }
    }
    for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
        if ((c->mStatus.m0C & 0x20) != 0x20 && (c->mStatus.m0C & 0x100) == 0x100) {
            return 0;
        }
    }
    return 1;
}

// inferred: starts the absorb effect of element `kind` (names invented)
inline void Em301LShade::StartAbsorb_unk(int kind) {
    m15E4 = kind;
    vf9C(1);
    fn_80239DAC(lbl_8087F3C0, m15F0, 0, 1);
    fn_80232B7C(m15F0, 0);
    PlayEffectB_unk(&m15F0[m15E4], (Model_unk*)&mMotion);
    m15EC = 300;
    if (m1680.mGlobal >= 0 && fn_805A49E4((SpecialFlag_unk*)&m1680) == 0) {
        fn_805A4A1C((SpecialFlag_unk*)&m1680, 1);
    }
}

// NONMATCHING: draft
void Em301LShade::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->mData->m04 == 10000) {
        fn_80151448(msg);
        vf40(hit);
        return;
    }
    if (m14E0 != 1 || m58C == 9 || m58C == 6 || m58C == 7 || m58C == 12) {
        msg->m68 = 0;
        msg->m90 = 1;
        msg->m84 = 2;
        msg->m8C = 0;
        return;
    }
    if (msg->mData->m74.m00 > 0.0f) {
        if (m15E8 <= 0) {
            StartAbsorb_unk(0);
        }
        m15E8 = 450;
    } else if (msg->mData->m74.m0C > 0.0f) {
        if (m15E8 <= 0) {
            StartAbsorb_unk(1);
        }
        m15E8 = 450;
    }
    int ok = 0;
    if (m15E8 > 0) {
        ok = 1;
        if (fn_80373148(lbl_8087F430)->m4C == 3 && msg->mData->m01 == 0 &&
            fn_80370A78(lbl_8087F430, 0x6E) == 0) {
            fn_80370AE4(lbl_8087F430, 0x6E, 1);
        }
    }
    if (ok == 0) {
        // inferred: the seven element rates of the hit (HitRange_unk as an array)
        for (int i = 0; i < 7; i++) {
            if ((&msg->mData->m74.m00)[i] > 0.01f) {
                ok = 1;
                break;
            }
        }
    }
    if (ok == 0 && msg->mAttacker != 0 && msg->mData->m01 == 0 && msg->mAttacker->fn_8014FCC4()) {
        unsigned int flags = msg->mAttacker->fn_801799BC();
        if ((flags & 0x20) == 0x20 || (flags & 0x40) == 0x40 || (flags & 0x200) == 0x200 ||
            (flags & 0x400) == 0x400) {
            ok = 1;
        }
    }
    if (ok == 0) {
        msg->m68 = 0;
        msg->m90 = 1;
        msg->m84 = 2;
        msg->m8C = 0;
        if (m1688.mGlobal >= 0 && fn_805A49E4((SpecialFlag_unk*)&m1688) == 0) {
            fn_805A4A1C((SpecialFlag_unk*)&m1688, 1);
        }
        return;
    }
    // inferred: HitMsg_unk+0x40 is the index of the damage sphere hit (read with a cast, as in
    // game/Em101ElderZesya.cpp, to keep the shared header CharaTask.h unchanged)
    int index = *(int*)&msg->m3C[4];
    atn::Vector3 saved = msg->m1C;
    msg->m1C = m624.mData[index].m04.mCenter;
    atn::Vector3 d = msg->m28 * m624.mData[index].m04.mRadius;
    msg->m1C += d;
    msg->m1C.y += m624.mData[index].m04.mRadius;
    msg->m50 |= 0x10;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    vf40(hit);
    msg->m1C = saved;
}

void Em301LShade::fn_802D4378() {
    HitInfo_unk* skill = fn_80219E6C(m1534);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
    }
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    if (m14E8 == 15) {
        m153C = m1620->mPos;
        m153C.y += 1.0f;
        atn::Vector3 to = m153C;
        to.y -= 100.0f;
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &m153C, &to, 0x80000000, 0, 0)) {
            m153C = result.m10;
        }
        CharaTask* target = m1620;
        atn::Vector3 offset(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', target->mRot.y);
        PSMTXMultVec(m, &offset, &offset);
        m153C += offset;
        fn_80232B7C(this, 10);
        Color4f_unk color;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m1554, 0, 0, m153C, lbl_807C7030, color, -1, -1, 1, 1.0f);
    }
    if (m14E8 == 65) {
        fn_800FAB80(lbl_8087F048, this, skill, m590, &m153C, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x115), *(const wchar_t**)((char*)m60 + 4));
        fn_80109828(lbl_8087F048, text);
        fn_800C344C("SE_EM301_002", &m153C, 0, -1, 1.0f);
    }
}
