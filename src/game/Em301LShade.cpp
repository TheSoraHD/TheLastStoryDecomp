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
// inferred: camera settings block of the field (as CameraParam_unk of chr/ControlBreakOpen.cpp)
struct ShadeCamera_unk {
    atn::Vector3 mPos;
    atn::Vector3 mTarget;
    float mFov;
    int mFlag;
};
// inferred: camera mode switch of the field (same name as in chr/ControlBreakOpen.cpp)
struct CameraSub_unk {
    void fn_80389838(int mode);
};
struct Field_unk {
    RouteTable_unk* fn_8013C504();
    unsigned char m000[0x6C];
    CameraSub_unk m06C;   // inferred: camera mode (fn_80389838)
    unsigned char m06D[0x568 - 0x6D];
    ShadeCamera_unk m568;      // inferred: camera of the events
    unsigned char m588[0x870 - 0x588];
    int m870;                  // inferred: previous camera mode (read at m06C + 0x804)
    unsigned char m874[0x96C - 0x874];
    int m96C;
    int m970;   // inferred: shake frames
    float m974; // inferred: shake strength
    float m978;
};
struct Effect_unk;
extern "C" void fn_801092C8(Effect_unk* effect, int msg, CharaTask* chara, int value, int arg);
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed (as in game/Arms.cpp)
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
// inferred: copy of the event camera (as CameraMgr_unk::SetParam_unk of chr/ControlBreakOpen.cpp: the two
// vectors through a Vector3 pointer); name invented
inline void SetCamera_unk(Field_unk* field, const ShadeCamera_unk& c) {
    const atn::Vector3* src = &c.mPos;
    atn::Vector3* dst = &field->m568.mPos;
    dst[0] = src[0];
    dst[1] = src[1];
    field->m568.mFov = c.mFov;
    field->m568.mFlag = c.mFlag;
}
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
// inferred: game state (lbl_8087EFA8): a block of the camera blur at 0x240 and the time scale at 0x3A4
struct ShadeBlur_unk {
    int m00;
    int m04;
    int m08;
    float m0C;
    float m10;
    int m14;
    int m18;
    float m1C;
    float m20;
};
struct ShadeGame_unk {
    unsigned char m000[0x240];
    ShadeBlur_unk m240;
    unsigned char m264[0x3A4 - 0x264];
    float m3A4;
};
extern "C" ShadeGame_unk* lbl_8087EFA8;
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

// inferred: same as PlayEffect_unk with the manager read last (as in game/Em302KrakenMp.cpp); name invented
inline void PlayEffect2_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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
    EffectMgr_unk* mgr = lbl_8087F3C0;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
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

// inferred: same as PlayEffect_unk with the locals in the order color, rot, pos and the manager read last
// (name invented)
inline void PlayEffectC_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
    atn::Vector3 rot;
    atn::Vector3 pos;
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
    EffectMgr_unk* mgr = lbl_8087F3C0;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
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

void Em301LShade::vf40(HitInfo_unk* hit) {
    m14EC++;
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802D57B8();
    }
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

atn::Vector3 Em301LShade::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center != 0) {
        result = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
    }
    return result;
}

// inferred: release assert of the original (empty; without the two of fn_802D5674 MWCC inlines it into
// fn_802D3370, which the original calls out of line; as in game/ColliAttrManager.cpp)
#define ASSERT_UNK(x) ((void)0)

// inferred: plays a motion on track 0 after stopping it (names invented)
inline void RestartMotion_unk(Em301LShade* e, int motion) {
    fn_80097CCC((Model_unk*)&e->mMotion, 0, 0.0f);
    e->mMotion.m34C = 1;
    e->mMotion.mTracks[0].m20 = 1.0f;
    e->mMotion.mTracks[0].m0C = 1.0f;
    e->mMotion.fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
}

// inferred: starts state 9 (absorbs, looped motion 0x33) with its effect (names invented)
inline void StartAbsorbAttack_unk(Em301LShade* e) {
    fn_80232B7C(e, 13);
    PlayEffect_unk(&e->m1588, (Model_unk*)&e->mMotion);
    e->SetState_unk(9);
    e->mMotion.m34C = 1;
    e->mMotion.mTracks[0].m20 = 1.0f;
    e->mMotion.mTracks[0].m0C = 1.0f;
    e->mMotion.fn_80097C08(0, 0x33, 1, 0, 1, 0.0f, 0.1f);
    fn_80370AE4(lbl_8087F430, 0x32, 3);
}

// inferred: starts state 12 (hides) with the hide kind m1510 (names invented)
inline void StartHide_unk(Em301LShade* e, int kind) {
    e->m1510 = kind;
    e->SetState_unk(12);
    RestartMotion_unk(e, 0x14A);
    e->m15E8 = 0;
    e->m15EC = 0;
}

// inferred: behaviour of the shade by rounds (m14DC: 0 and 2 appear at "first_appear_number", 1 and 3
// at a random point); in each round m14E0 is the phase (0: appears and absorbs if it can, 1: attacks,
// 2: hides, 3: calls the reinforcements)
void Em301LShade::fn_802D3370() {
    if (m14DC == 0) {
        switch (m14E0) {
        case 0:
            if (m14E4 == 0) {
                fn_802D5674(m1628);
            }
            if (m14E4 == 1) {
                if (fn_802D6004()) {
                    StartAbsorbAttack_unk(this);
                }
                m14E0 = 1;
                m14E4 = 0;
                m1500 = 0;
                return;
            }
            break;
        case 1:
            if (fn_802D3C74() == 0) {
                return;
            }
            break;
        case 2:
            if (m14E4 == 0) {
                StartHide_unk(this, 0);
            }
            if (m14E4 == 1) {
                m14E0 = 3;
                m14E4 = 0;
                return;
            }
            break;
        case 3:
            if (fn_802D404C() == 0) {
                m14DC = 1;
                return;
            }
            break;
        }
    } else if (m14DC == 1) {
        switch (m14E0) {
        case 0:
            if (m14E4 == 0) {
                fn_802D5674(rand() % m162C.size());
            }
            if (m14E4 == 1) {
                if (fn_802D6004()) {
                    StartAbsorbAttack_unk(this);
                }
                m14E0 = 1;
                m14E4 = 0;
                m1500 = 0;
                return;
            }
            break;
        case 1:
            if (fn_802D3C74() == 0) {
                return;
            }
            break;
        case 2:
            if (m14E4 == 0) {
                StartHide_unk(this, 3);
            }
            if (m14E4 == 1) {
                m14E0 = 3;
                m14E4 = 0;
                return;
            }
            break;
        case 3:
            if (fn_802D404C() == 0) {
                m14DC = 2;
                return;
            }
            break;
        }
    } else if (m14DC == 2) {
        switch (m14E0) {
        case 0:
            if (m14E4 == 0) {
                fn_802D5674(m1628);
            }
            if (m14E4 == 1) {
                if (fn_802D6004()) {
                    StartAbsorbAttack_unk(this);
                }
                m14E0 = 1;
                m14E4 = 0;
                m1500 = 0;
                return;
            }
            break;
        case 1:
            if (fn_802D3C74() == 0) {
                return;
            }
            break;
        case 2:
            if (m14E4 == 0) {
                StartHide_unk(this, 1);
            }
            if (m14E4 == 1) {
                m14E0 = 3;
                m14E4 = 0;
                return;
            }
            break;
        case 3:
            if (fn_802D404C() == 0) {
                m14DC = 3;
                return;
            }
            break;
        }
    } else if (m14DC == 3) {
        switch (m14E0) {
        case 0:
            if (m14E4 == 0) {
                fn_802D5674(rand() % m162C.size());
            }
            if (m14E4 == 1) {
                if (fn_802D6004()) {
                    StartAbsorbAttack_unk(this);
                }
                m14E0 = 1;
                m14E4 = 0;
                m1500 = 0;
                return;
            }
            break;
        case 1:
            if (fn_802D3C74() == 0) {
                return;
            }
            break;
        case 2:
            if (m14E4 == 0) {
                StartHide_unk(this, 1);
            }
            if (m14E4 == 1) {
                m14E0 = 3;
                m14E4 = 0;
                return;
            }
            break;
        case 3:
            if (fn_802D404C() == 0) {
                m14DC = 2;
                return;
            }
            break;
        }
    }
    m14E4++;
}

// inferred: party members alive and the ones inside the absorb range (38 * scale around the body)
inline void CountNear_unk(Em301LShade* e, int* outNear, int* outCount) {
    atn::Vector3 center = e->mPos;
    int near = 0;
    int count = 0;
    float radius = 38.0f * e->m540.x;
    center.y += 13.0f * e->m540.x;
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
        if ((c->mStatus.m0C & 0x20) != 0x20) {
            atn::Vector3 d = c->mPos - center;
            count++;
            if (fn_805F9920(&d) < radius * radius) {
                near++;
            }
        }
    }
    *outNear = near;
    *outCount = count;
}

// inferred: choice of the next attack (wait state): returns 0 when the shade leaves (hit too often or
// absorbed); after two attacks it waits, with 40% of the party near it absorbs (state 10), otherwise it
// swings (state 8)
// NONMATCHING: 99.9% - scheduling of the count increment in the party loop, and the int-to-float
// constant lbl_80747090, which is in the .rodata split of game/Em209SmallGolem (other queue, pending)
int Em301LShade::fn_802D3C74() {
    if (m14E4 == 0) {
        m14EC = 0;
    }
    if (m14EC > m14F0 || m1508 != 0) {
        m14EC = 0;
        m14E0 = 2;
        m14E4 = 0;
        m14F4 = 0;
        return 0;
    }
    if (m1500 > 0) {
        if (m14FC >= 2) {
            SetState_unk(11);
            RestartMotion_unk(this, 0x153);
            m14FC = 0;
        } else {
            int near = 0;
            int count = 0;
            CountNear_unk(this, &near, &count);
            if ((float)near / (float)count >= 0.4f) {
                SetState_unk(10);
                RestartMotion_unk(this, 0x145);
                ShowMessage_unk(0x123);
            } else {
                fn_80232B7C(this, 11);
                PlayEffectB_unk(&m1548, (Model_unk*)&mMotion);
                SetState_unk(8);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
                fn_80370AE4(lbl_8087F430, 0x32, 2);
                m1538 = 1;
            }
            m14FC++;
        }
        m1500 = 0;
    }
    return 1;
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

// inferred: the event camera looks at the head from "dead_camera"; when the death motion ends the
// enemy is removed and the camera and the time scale go back to normal
// NONMATCHING: 99.1% - registers r5/r6 of the copy of "dead_camera" (the __VecCopy family) and the
// string pool offsets
void Em301LShade::vfEC() {
    ShadeCamera_unk c;
    atn::Vector3 dir;
    c.mPos = *(atn::Vector3*)&m15BC;
    atn::Vector3 head = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    c.mTarget = head;
    c.mFov = 0.7853982f;
    c.mFlag = 0;
    SetCamera_unk(lbl_8087F430, c);
    dir.Set_unk(m15BC - mPos.x, 0.0f, m15C4 - mPos.z);
    atn::Vector3 rot = dir.ToRotationZ_unk();
    mRot.y = rot.y;
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        fn_80370320(lbl_8087F430, 0xD0, 0, 0);
        fn_800EE360(this);
        mFlag15 = 1;
        fn_801765D8();
        CameraSub_unk* sub = &lbl_8087F430->m06C;
        sub->fn_80389838(*(int*)((char*)sub + 0x804));
        ShadeGame_unk* game = lbl_8087EFA8;
        ShadeBlur_unk blur;
        blur.m04 = game->m240.m04;
        blur.m08 = game->m240.m08;
        blur.m0C = game->m240.m0C;
        blur.m10 = game->m240.m10;
        blur.m14 = game->m240.m14;
        blur.m18 = game->m240.m18;
        blur.m1C = game->m240.m1C;
        blur.m20 = game->m240.m20;
        blur.m00 = 0;
        game->m240 = blur;
        lbl_8087EFA8->m3A4 = 1.0f;
    }
}

void Em301LShade::fn_802D5674(int count) {
    ASSERT_UNK(count >= 0);
    ASSERT_UNK(this != 0);
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

// inferred: starts state 2 (dead): the death effect (big shade or not), the event camera on the head,
// half speed and the body turned to the camera
// NONMATCHING: 99.2% - stack slots of the effect temporaries and of the blur copy (registers r3/r9),
// string pool offsets; 4 variants
void Em301LShade::fn_802D57B8() {
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_8016E970(3);
    fn_80097CCC((Model_unk*)&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 1000);
    if (m14D4 != 0) {
        PlayEffect2_unk(&m1614, (Model_unk*)&mMotion);
    } else {
        Color4f_unk color;
        atn::Vector3 pos(0.0f, 13.0f * m540.x, 0.0f);
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        EffectMgr_unk* mgr = lbl_8087F3C0;
        fn_8023A8B4(mgr, &m1608, (Model_unk*)&mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, 1.0f);
    }
    if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) != 0 && mType == 2) {
        fn_801092C8(lbl_8087F048, 6, this, 0, 0);
    }
    fn_800EB7A0(this);
    m15E8 = 0;
    m15EC = 0;
    ShadeCamera_unk c;
    atn::Vector3 dir;
    c.mPos = *(atn::Vector3*)&m15BC;
    atn::Vector3 head = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    c.mTarget = head;
    c.mFov = 0.9599311f;
    c.mFlag = 0;
    SetCamera_unk(lbl_8087F430, c);
    lbl_8087F430->m06C.fn_80389838(0x13);
    ShadeGame_unk* game = lbl_8087EFA8;
    ShadeBlur_unk blur;
    blur.m04 = game->m240.m04;
    blur.m08 = game->m240.m08;
    blur.m0C = game->m240.m0C;
    blur.m10 = game->m240.m10;
    blur.m14 = game->m240.m14;
    blur.m18 = game->m240.m18;
    blur.m1C = game->m240.m1C;
    blur.m20 = game->m240.m20;
    blur.m00 = 1;
    game->m240 = blur;
    lbl_8087EFA8->m3A4 = 0.5f;
    fn_80370320(lbl_8087F430, 0xD0, 1, 0);
    dir.Set_unk(c.mPos.x - mPos.x, 0.0f, c.mPos.z - mPos.z);
    atn::Vector3 rot = dir.ToRotationZ_unk();
    mRot.y = rot.y;
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

void Em301LShade::vf108() {}

void Em301LShade::vf104() {}

void Em301LShade::vf100(int state) {}

void Em301LShade::vfFC() {}
