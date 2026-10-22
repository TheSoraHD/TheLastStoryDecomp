#include "game/EnemyQuarkSecond.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/EnemyManager.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"
#include "game/EquipItemData.h"
#include <boost/tokenizer.hpp>

extern "C" double sqrt(double x);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: field (lbl_8087F430); route tables (fn_8013C504) and the camera follow state (names
// invented, as in game/EnemyQuarkFirst.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
    unsigned char m000[0x4D8];
    int m4D8;
    unsigned char m4DC[0x8A0 - 0x4DC];
    CharaTask* m8A0;
    unsigned char m8A4[0x97C - 0x8A4];
    unsigned char m97C;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: field accessor (weak copy 0x80121F00, as in game/EnemyJill.cpp)
extern "C" inline Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" void fn_80370320(Field_unk* field, int index, int value, int arg); // inferred: sets a global flag
// inferred: route tables of the field, null without a field (name invented)
inline RouteTable_unk* GetRouteTable_unk() {
    return fn_80121F00() != 0 ? fn_80121F00()->fn_8013C504() : 0;
}
extern "C" CharaManager<EnemyTask>* fn_801A03E0(); // inferred: enemy manager
// inferred: player manager accessor (weak copy 0x8000D9E8, as in game/EnemyJill.cpp)
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" inline PlayerManager* fn_8000D9E8() {
    return lbl_8087F8A0;
}
// inferred: Model_unk member of another unit (sets a value of the model, weak copy 0x80288E30 of
// game/Em203GreaterDaemon.cpp)
extern "C" void fn_80288E30(CharaTask* chara, float value);

// inferred: flag of the collision capsule (weak copy 0x80338CDC; also called by EnemyQuarkStand)
extern "C" void fn_80338CDC(atn::ColliBase* colli, int upright) {
    if (upright) {
        colli->mFlags |= 0x80000000;
    } else {
        colli->mFlags &= ~0x80000000;
    }
}

EnemyQuarkSecond::EnemyQuarkSecond(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0),
      m14D8(0), m1558(0), m155C(0), m1560(0), m1564(0), m1628(0), m1644(0), m1648(0.0f), m165C(0),
      m1660(0), m1664(0), m16A4(150), m16A8(10), m16AC(0), m16B0(0), m16B1(0), m16B2(0), m16B3(1),
      m16B4(0), m16C8(0), m16CC(15), m16D0(1200), m1758(0), m1760(0), m1764(8.0f), m1768(300.0f),
      m1770(10), m1774(150.0f), m1778(30) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_enemy_quark_second.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "db_name=", 8) == 0) {
            name = it->c_str() + 8;
        }
    }
    std::string path = fn_8006AFF8(dir, name);
    m16B8.vf0C(path.c_str());
    m15A4.fn_8023780C("ef_bs072f.efp");
    m15B0.fn_8023780C("ef_bs054d.efp");
    m15BC.fn_8023780C("ef_bs054a.efp");
    m15C8.fn_80237654("ef_bs054e_01.eff");
    m162C.fn_8023780C("ef_bs058a.efp");
    m1638.fn_8023780C("ef_bs058c.efp");
    m1668.fn_80237654("ef_bs051b.eff");
    m1610.fn_8023780C("ef_bu040a.efp");
    m161C.fn_8023780C("ef_bu040h.efp");
    m1674.fn_8023780C("ef_bd008d.efp");
    m1680.fn_8023780C("ef_bu047b.efp");
    m168C.fn_8023780C("ef_bu047c.efp");
    m1698.fn_8023780C("ef_bs079b.efp");
    fn_80370320(fn_80121F00(), 0x65, 1, 0);
    fn_80370320(fn_80121F00(), 0x68, 1, 0);
    fn_80338CDC(&mColli, 0);
    m1598.Set_unk(0.0f, 0.0f, 0.0f);
    mFlagB11 = 1;
    mFlag24 = 0;
}

// inferred: reads a list of ids into `table` until a zero id (not a member: it gets no `this`)
// NONMATCHING: ~99% - known push_back residue: the inlined fast path loads size/capacity in the other
// registers (docs: push_back in loops)
static void fn_80338D14(QuarkTable_unk* table, atn::Csv* csv) {
    for (;;) {
        int id = fn_800DC12C(csv->fn_8005B9CC());
        if (id == 0) {
            break;
        }
        table->push_back(id);
    }
}

// inferred: inline accessors (weak copies 0x803396EC, 0x803396FC, 0x80339718, called out of line by
// vf0C, which is over the inline budget; game/EnemyQuarkStand.cpp calls the same copies). Names keep
// the addresses: 0x80339718 is the CharaStatusPair_unk(int) constructor of CharaStatus.h (no null test).
extern "C" inline void fn_803396EC(CharaTask* chara, unsigned int flags) {
    chara->m54C |= flags;
}
extern "C" inline void fn_803396FC(CharaStatus* status, int index, const CharaStatusPair_unk& pair) {
    status->m2D0[index] = pair;
}


int EnemyQuarkSecond::vf0C() {
    if (fn_8013655C() == 0 && m15A4.fn_80237874() == 0 && m15B0.fn_80237874() == 0 &&
        m15BC.fn_80237874() == 0 && m15C8.fn_802376D0() == 0 && m1638.fn_80237874() == 0 &&
        m162C.fn_80237874() == 0 && m1668.fn_802376D0() == 0 && m16B8.fn_80473F50() == 0 &&
        m1610.fn_80237874() == 0 && m161C.fn_80237874() == 0 && m1674.fn_80237874() == 0 &&
        m1680.fn_80237874() == 0 && m168C.fn_80237874() == 0 && m1698.fn_80237874() == 0 &&
        fn_800D3FA4() != 0) {
        fn_80344780();
        fn_803396EC(this, 0x200);
        fn_80288E30(this, 0.03f);
        mMotion.fn_80099E9C("hips", "em211", 4.3f);
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x15;
        mStatus.m18 |= 0x400000;
        mStatus.m18 |= 0xC208;
        mStatus.m18 |= 0x3880000;
        mStatus.m18 |= 0x400;
        m16C4 = mStatus.mParamB.m84 / 5;
        RouteTable_unk* table = fn_80121F00() != 0 ? fn_80121F00()->fn_8013C504() : 0;
        if (m16B8.fn_80470580() != 0 && table != 0) {
            atn::Csv csv((const char*)m16B8.fn_80470580(), m16B8.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "ShockExpandSnicWave") == 0) {
                    m1764 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackWaitTime") == 0) {
                    m176C = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "BarrierHp") == 0) {
                    m16C4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GatheringBurstCount") == 0) {
                    m16CC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "DownCount") == 0) {
                    m16A4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "StundUpDamageCount") == 0) {
                    m16A8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackedMaxInStick") == 0) {
                    m1770 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "SwordDirectionLine") == 0) {
                    m1774 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "SwordThrowTime") == 0) {
                    m1778 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ShortRange") == 0) {
                    m177C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "MiddleRange") == 0) {
                    m1780 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "LongRange") == 0) {
                    m1784 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "ShortRangeTableMode1") == 0) {
                    fn_80338D14(&m14EC[0][0], &csv);
                } else if (strcmp(key, "MiddleRangeTableMode1") == 0) {
                    fn_80338D14(&m14EC[0][1], &csv);
                } else if (strcmp(key, "LongRangeTableMode1") == 0) {
                    fn_80338D14(&m14EC[0][2], &csv);
                } else if (strcmp(key, "ShortRangeTableMode2") == 0) {
                    fn_80338D14(&m14EC[1][0], &csv);
                } else if (strcmp(key, "MiddleRangeTableMode2") == 0) {
                    fn_80338D14(&m14EC[1][1], &csv);
                } else if (strcmp(key, "LongRangeTableMode2") == 0) {
                    fn_80338D14(&m14EC[1][2], &csv);
                } else if (strcmp(key, "ShortRangeTableMode3") == 0) {
                    fn_80338D14(&m14EC[2][0], &csv);
                } else if (strcmp(key, "MiddleRangeTableMode3") == 0) {
                    fn_80338D14(&m14EC[2][1], &csv);
                } else if (strcmp(key, "LongRangeTableMode3") == 0) {
                    fn_80338D14(&m14EC[2][2], &csv);
                } else if (strcmp(key, "LaserSkillID") == 0) {
                    m14D0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "PierceSkillID") == 0) {
                    m14D4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SonicSkillID") == 0) {
                    m14D8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SwordSkillID") == 0) {
                    m14DC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "AttackSkill") == 0) {
                    m14E0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BurstSkill") == 0) {
                    m14CC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "ThirdScriptId") == 0) {
                    m1758 = fn_801A03E0()->Find_unk(atoi(csv.fn_8005B9CC()));

                } else if (strcmp(key, "RainSkill") == 0) {
                    for (int i = 0;; i++) {
                        int id = fn_800DC12C(csv.fn_8005B9CC());
                        if (id == 0) {
                            break;
                        }
                        m14E4[i] = fn_80219E6C(id);
                    }
                }
            } while (csv.fn_8005BBF8());
        }
        m16C0 = m16C4;
        m14C4 = fn_8000D9E8()->fn_8054A33C((const char*)4);
        if (m14D8 != 0) {
            fn_803396FC(&mStatus, 3, CharaStatusPair_unk(m14D8->m04));
        }
        return 1;
    }
    return 0;
}

// --- declarations used by the state functions (names invented unless noted) ---
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
// inferred: battle state (fn_80103F60 returns a record per party member; m128: weights), as in
// game/Em301LShade.cpp
struct BattleSlots_unk {
    unsigned char m000[0x128];
    float m128[1];
};
extern "C" BattleSlots_unk* fn_80103F60(Effect_unk* mgr, CharaTask* chara);
extern "C" int fn_80108378(BattleSlots_unk* slots, CharaTask* chara);
// inferred: effect manager (atn::EffectManager, global instance at 0x8087F3C0)
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
// inferred: accessor of the effect manager (weak copy 0x800F7FA0; inlined unless the caller is over
// the inline budget)
extern "C" inline EffectMgr_unk* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" inline void fn_800F7FA8(EffectMgr_unk* mgr, void* owner, int arg) {
    fn_80232B7C(owner, arg);
}
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
// inferred: plays `efp` on `model` (weak copy 0x8026607C of game/Em201ZanGurgFinal.cpp; its body is
// defined after fn_8023A8B4 below)
extern "C" inline void fn_8026607C(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg);
extern "C" void fn_8023A108(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, int id, int arg);
extern "C" void fn_8023A254(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, int arg, float rate);
extern "C" void fn_80276AE4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, const atn::Vector3& pos,
                            const atn::Vector3& rot, int a, int b, int c, float scale);
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" inline void fn_8026607C(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg) {
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, arg, -1, 1.0f);
}
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg, int arg2, float volume); // inferred: plays a sound
extern "C" void* lbl_8087F498; // inferred: camera shake manager
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);
// inferred: accessors inlined by the small callers and called out of line by the functions over the
// inline budget (the original links the weak copies of other units: game/Em201ZanGurgFinal.cpp,
// game/Em203GreaterDaemonRedMp.cpp, game/PlayerInput_unk.cpp, Event/Puppeteer_CharaTask.cpp ...)
extern "C" inline CharaTask* fn_8000DCF4(PlayerManager* mgr) {
    return mgr->mHead;
}
extern "C" inline CharaTask* fn_802A36B0(PlayerManager* mgr) {
    return mgr->mHead;
}
// inferred: link of the character lists of the managers (CharaTask + 0x14AC, as in game/CharaTask.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
extern "C" inline CharaTask* fn_802A4094(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}
extern "C" inline int fn_80267B28(CharaTask* chara) {
    return chara->m560;
}
// inferred: input state of the field (as in game/EnemyJill.cpp); the rumble block at 0x900
struct PlayerInput_unk {
    unsigned char m000[0x900];
    int m900;
    int m904;
    float m908;
    float m90C;
};
extern "C" inline PlayerInput_unk* fn_80122550(Field_unk* field) {
    return (PlayerInput_unk*)((char*)field + 0x6C);
}
// inferred: starts a rumble (weak copy 0x8028CFB8 of game/Em203GreaterDaemon.cpp: toggles bit 0)
extern "C" inline void fn_8028CFB8(PlayerInput_unk* input, int frames, float a, float b) {
    input->m904 = frames;
    input->m908 = a;
    input->m900 = input->m900 % 2;
    input->m90C = b;
}
// inferred: keyboard of the debug build (lbl_8087EEF0, as in game/EnemyJill.cpp)
struct QuarkKeyboard_unk {
    unsigned char m000[0x34];
    unsigned char m034[0xD90 - 0x34];
    int mD90;
};
extern "C" QuarkKeyboard_unk* lbl_8087EEF0;
extern "C" int fn_8007708C(QuarkKeyboard_unk* kb, void* state, int key); // inferred: key pressed
extern "C" inline QuarkKeyboard_unk* fn_802A49BC() {
    return lbl_8087EEF0;
}
extern "C" inline int fn_802A4968(QuarkKeyboard_unk* kb, int pad, int key) {
    int result = 0;
    if (kb->mD90 != 0 && fn_8007708C(kb, &kb->m034[pad * 0x1A], key) != 0) {
        result = 1;
    }
    return result;
}
// inferred: hit points ratio (weak copy 0x8029F3AC of game/Em203GreaterDaemonRedMp.cpp)
extern "C" inline float fn_8029F3AC(CharaStatus* status) {
    return status->mParamB.m84 > 0 ? status->m04 / (float)status->mParamB.m84 : 0.0f;
}
extern "C" inline void fn_80265804(CharaStatus* status, float rate) {
    status->m2FC = rate;
}
// inferred: map of the field (fn_80373148, weak copy of game/GameTask.cpp) and its BGM
struct MapInfo_unk {
    unsigned char m00[0xE0];
    int mE0; // inferred: kind of BGM
    int mE4;
    int mE8;
};
struct MapHolder_unk {
    unsigned char m00[0x64];
    MapInfo_unk* m64;
};
struct FieldMap_unk {
    unsigned char m0000[0x10D8];
    MapHolder_unk* m10D8;
};
extern "C" inline MapInfo_unk* fn_80373148(Field_unk* field) {
    MapHolder_unk* h = ((FieldMap_unk*)field)->m10D8;
    return h != 0 ? h->m64 : 0;
}
extern "C" inline int fn_80339F7C(MapInfo_unk* map) {
    return map->mE0;
}
extern "C" inline void fn_80339F84(MapInfo_unk* map, int kind) {
    map->mE8 = kind;
}
extern "C" void fn_804A04AC(MapInfo_unk* map, int kind, int arg2, int arg3, int arg4); // inferred: map BGM
extern "C" void fn_80354110(EnemyTask* enemy); // inferred: EnemyQuarkStand member of another queue
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, void* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: size of a vector (weak copy 0x800E0AA8 of atn/WordWrap_unk.cpp) and element i (weak copy
// 0x8028B74C of game/Em203GreaterDaemon.cpp)
extern "C" inline unsigned int fn_800E0AA8(const IntVector_unk* v) {
    return v->mSize;
}
extern "C" inline int* fn_8028B74C(IntVector_unk* v, int i) {
    return &v->mData[i];
}
// inferred: length in the xz plane (weak copy 0x801C3910, as in chr/ControlMoveCursor.cpp)
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}

// inferred: weak copies of this file (also called by game/EnemyQuarkStand.cpp and others); names keep
// the addresses
extern "C" inline int fn_80339F04(CharaTask* chara) {
    return ((chara->m54C >> 3) & 1) ^ 1;
}
extern "C" inline void fn_80339F14(EquipItemData* item, int flag) {
    item->mFlags.mFlag = flag;
}
extern "C" inline EquipItemData** fn_80339F24(FixedArray_unk<EquipItemData*, 4>* items, int i) {
    return &items->mData[i];
}
extern "C" inline void fn_80339F34(CharaTask* chara, int on) {
    if (on) {
        chara->m54C |= 0x2000;
    } else {
        chara->m54C &= ~0x2000;
    }
}
extern "C" inline void fn_80339F5C(Model_unk* model, int track, float weight) {
    model->mTracks[track].m10 = weight;
}
extern "C" inline HitInfo_unk* fn_80339F6C(CharaTask* chara) {
    return chara->m638;
}
extern "C" inline void fn_80339F74(BattleCharaTask* chara, int value) {
    chara->m1454 = value;
}

// inferred: sets the event flag `index` once (inline: expanded in vf3C, weak copy 0x8034472C called by
// the functions over the inline budget)
inline void EnemyQuarkSecond::fn_8034472C(int index, int value) {
    if (fn_80370A78(lbl_8087F430, index) == 0) {
        fn_80370AE4(lbl_8087F430, index, value);
    }
}

// NONMATCHING: the original is over the inline budget and calls all the accessors (fn_80339F04 ...,
// fn_8000D9E8, fn_80121F00, CharaTask::GetStatus_unk ...) out of line; this compile inlines them. With
// an artificial 400-statement inline candidate the body is within 87 lines of the original (same open
// case as LoadSeTableTask::vf0C, docs/inlining.md); not added (no invented functions).
void EnemyQuarkSecond::vf10() {
    if (m16B4 != 0) {
        if (mMotion.fn_80139550(0) < 6.0f) {
            fn_8016E4C4(0);
            m14B4 = 0;
        } else {
            if (fn_80339F04(this) == 0) {
                fn_800F7FA8(fn_800F7FA0(), &m1680, 0);
                fn_8026607C(fn_800F7FA0(), &m1680, &mMotion, 1);
                fn_8023A108(fn_800F7FA0(), &m1680, 0, 6);
                fn_8023A254(fn_800F7FA0(), &m1680, 0, 0.5f);
            }
            mMotion.fn_80139F3C(0, 0.5f);
            fn_8016E4C4(1);
            if (mMotion.fn_80139550(0) >= 54.0f && m14B4 == 0) {
                fn_8028CFB8(fn_80122550(fn_80121F00()), 0x14, 0.0f, 10.0f);
                fn_80339F14(*fn_80339F24(&m6A4, 0), 1);
                m14B4++;
            }
        }
        float end = mMotion.fn_80097D7C(0);
        if (mMotion.fn_80139550(0) >= end) {
            m16B4 = 0;
            fn_80339F34(this, 0);
            fn_80341EE4();
        }
        fn_80145334();
        fn_8033AB88();
        return;
    }
    if (m1644 != 0) {
        fn_803441AC();
    }
    m14B8++;
    m14C0++;
    m14B0 = mC58.mC4;
    fn_803449AC();
    if (m14B0 == 0) {
        m14B0 = mC58.mC4 = fn_8000DCF4(fn_8000D9E8());
    }
    if (fn_80370A78(fn_80121F00(), 0x8C) != 0) {
        if (m58C == 13) {
            fn_8016E970(3);
            fn_8034441C(0x3E, 1, 1.0f);
            fn_80339F5C(&mMotion, 0, 1.0f);
            m14B4 = 1;
            m14B8 = 0;
        } else {
            fn_8016E970(3);
            fn_80341EE4();
            fn_80341E14();
            fn_80344BB4();
            for (CharaTask* c = fn_802A36B0(fn_8000D9E8()); c != 0; c = fn_802A4094(c)) {
                if (fn_80267B28(c) == 0x53 && c->CanAct_unk()) {
                    c->fn_8015ECC4();
                }
            }
        }
        fn_80370AE4(fn_80121F00(), 0x8C, 0);
        if (m14BC == 1) {
            fn_80239DAC(fn_800F7FA0(), &m1610, 0, 0);
            fn_800F7FA8(fn_800F7FA0(), &m1610, 0);
            fn_80276AE4(fn_800F7FA0(), &m1610, &mMotion, atn::Vector3(0.0f, 0.0f, 0.0f),
                        atn::Vector3(0.0f, 0.0f, 0.0f), -1, -1, 1, 4.0f);
            fn_80370320(fn_80121F00(), 0x38C, 1, 0);
            m14C4->GetStatus_unk()->fn_8012D180();
            m14C4->GetStatus_unk()->m228 = 1.0f;
            m14C4->GetModel_unk()->fn_8009373C("op_eyepatch", 0);
        }
    }
    if (fn_8029F3AC(&mStatus) < 0.5f) {
        m1558 = 2;
    } else if (fn_8029F3AC(&mStatus) < 0.8f) {
        m1558 = 1;
    } else {
        m1558 = 0;
    }
    if (m1558 > 1) {
        fn_80265804(&mStatus, 1.2f);
    } else if (m1558 > 0) {
        fn_80265804(&mStatus, 0.7f);
    } else {
        fn_80265804(&mStatus, 0.0f);
    }
    if (m14BC == 1 && m14C4->GetStatus_unk()->m228 >= 1.0f) {
        if (m14C8 != 0) {
            if (fn_80267B28(m14C4) == 0x1E) {
                fn_8034472C(0x77, 1);
                m14C8 = 0;
            } else if (fn_80267B28(m14C4) != 0x1D) {
                fn_8034472C(0x75, 1);
                m14C8 = 0;
            }
        } else if (fn_80267B28(m14C4) == 0x1D && fn_80339F6C(m14C4)->m04 == 0x179A) {
            m14C8 = 1;
        }
    }
    fn_8000DCF4(fn_8000D9E8());
    int busy = 0;
    if (mC58.mC0 == 0 || m14B0 == 0) {
        fn_8016E970(3);
        fn_80339F74(this, 1);
    } else {
        if (m14B0 != 0) {
            switch (m58C) {
            case 6:
                fn_8033C490();
                break;
            case 7:
                fn_8033D040();
                break;
            case 8:
                fn_8033EF90();
                break;
            case 9:
                fn_8033D750();
                break;
            case 10:
                fn_8033E058();
                busy = 1;
                break;
            case 12:
                fn_80340E54();
                break;
            case 13:
                fn_8034100C();
                break;
            case 17:
                fn_803412AC();
                break;
            case 14:
                fn_8033DB8C();
                break;
            case 15:
                fn_8033F7FC();
                busy = 1;
                break;
            case 16:
                fn_803405C0();
                busy = 1;
                break;
            case 11:
                fn_80341BEC();
                break;
            default:
                fn_8033C3D8();
                fn_8033A7A0();
                busy = 1;
                break;
            }
        }
        if (m58C == 0) {
            if (m14B0 != 0) {
                mLook.fn_80129978(m14B0->GetModel_unk(), "neck", lbl_807C7030, 0.3f);
            }
        } else {
            mLook.fn_80129A48(0.3f);
        }
        int free = 1;
        if ((unsigned int)(m58C - 6) <= 4) {
            free = 0;
        }
        fn_80339F74(this, free);
        MapInfo_unk* map = fn_80373148(fn_80121F00());
        int kind;
        if (mPos.y - fn_8000DCF4(fn_8000D9E8())->fn_8013C38C()->y > 20.0f) {
            kind = 3;
        } else {
            kind = 0;
        }
        if (kind != fn_80339F7C(map)) {
            fn_804A04AC(map, kind, 0xF, -1, 0);
            fn_80339F84(map, kind);
        }
    }
    fn_8014C540();
    fn_80145334();
    fn_8033AB88();
    fn_80344454(100, busy, 25.0f, 6.2831855f);
    if (m1760 != 0) {
        if (fn_802A4968(fn_802A49BC(), 0, 0x4C) != 0) {
            fn_80342004();
        }
        if (fn_802A4968(fn_802A49BC(), 0, 0x44) != 0) {
            fn_8034228C();
        }
        if (fn_802A4968(fn_802A49BC(), 0, 0x4D) != 0) {
            fn_803435B8();
        }
    }
}

// inferred: resets the step and stops the effects of the attacks (inlined in the state functions)
inline void EnemyQuarkSecond::ResetState_unk() {
    m14B4 = 0;
    m14B8 = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80239DAC(lbl_8087F3C0, this, 0x3E8, 1);
    fn_80239DAC(lbl_8087F3C0, this, 0x3E9, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x3EA, 1);
    fn_80239DAC(lbl_8087F3C0, this, 0x3ED, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x3EF, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x3F0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x3F1, 0);
}

// inferred: plays motion `motion` on track 0 (inlined in the state functions; out of line copy
// fn_8034441C)
inline void EnemyQuarkSecond::PlayMotion_unk(int motion, int loop, float speed) {
    mMotion.mTracks[0].m0C = speed;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, motion, loop, 0, 1, 0.0f, 0.1f);
}

void EnemyQuarkSecond::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m16B4 != 0) {
        return;
    }
    if (m58C == 16 && msg->mData->m01 == 1) {
        return;
    }
    if (m58C == 11) {
        return;
    }
    msg->m10 *= 0.0f;
    msg->m50 |= 0x10;
    *(int*)&msg->m3C[4] = 0; // inferred: HitMsg_unk+0x40 (as in game/Em101ElderZesya.cpp)
    switch (m14BC) {
    case 1:
        if (msg->mData->m04 == 0x179A || msg->mData->m04 == 0x2710) {
            msg->m0C |= 0x80;
            fn_8034472C(0x94, 1);
        } else {
            msg->m0C |= 0x8008;
            fn_8034472C(0x91, 1);
            CharaTask* attacker = msg->mAttacker;
            if (attacker != 0 && msg->m44 == 0 && attacker->m55C == 6 &&
                (unsigned int)(attacker->m560 - 4) <= 1) {
                attacker->vf7C(0);
            }
        }
        break;
    case 2:
        if (msg->mAttacker != lbl_8087F8A0->mHead || msg->m44 != 0) {
            return;
        }
        break;
    }
    fn_80151448(msg);
    vf40(hit);
}

EnemyQuarkSecond::~EnemyQuarkSecond() {}
