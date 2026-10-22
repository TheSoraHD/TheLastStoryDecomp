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
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, int arg6, int arg7, int arg8, int arg9, float arg10,
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
// inferred: plays `efp` on `model` (weak copy 0x8026607C of game/Em201ZanGurgFinal.cpp; declared without
// a body: its constants are in the pool of that unit, not in this one)
extern "C" void fn_8026607C(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg);
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
// inferred: hit points ratio (weak copy 0x8029F3AC of game/Em203GreaterDaemonRedMp.cpp; declared without
// a body: its constants are in the pool of that unit)
extern "C" float fn_8029F3AC(CharaStatus* status);
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

// inferred: releases the field camera (inline: expanded in vf40, weak copy 0x80344BB4)
inline void EnemyQuarkSecond::fn_80344BB4() {
    Field_unk* field = lbl_8087F430;
    if (field->m8A0 != 0) {
        field->m8A0 = 0;
        field->m4D8 = 0;
        field->m97C = 0;
    }
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
        CharaTask* leader = fn_8000DCF4(fn_8000D9E8());
        m14B0 = leader;
        mC58.mC4 = leader;
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
    int kind;
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
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
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

// inferred: starts state 13 (down; inlined three times in vf40, as the reset of the state)
inline void EnemyQuarkSecond::StartDown_unk() {
    ResetState_unk();
    fn_8016E970(3);
    m58C = 13;
    PlayMotion_unk(0x3D, 0, 1.0f);
    fn_80344BB4();
    m574.y = 0.0f;
    if (m14BC == 2) {
        m14A8 &= ~0x04000000;
    }
}

void EnemyQuarkSecond::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 13 && msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
        m16AC++;
    }
    switch (m14BC) {
    case 0:
        m16C8++;
        if (mStatus.m04 <= 0.0f) {
            mStatus.m04 = 1.0f;
            mStatus.fn_8013322C(0x20);
            m14BC = 2;
            StartDown_unk();
        } else if (msg->mData->m04 == 0x1791) {
            StartDown_unk();
        }
        break;
    case 2:
        if (mStatus.m04 <= 0.0f) {
            if (m58C != 17 || msg->mData->m04 != 0xE2) {
                mStatus.m04 = 1.0f;
                mStatus.fn_8013322C(0x20);
            } else {
                fn_80370AE4(lbl_8087F430, 0xCE, 1);
                mMotion.fn_8009373C("op_01_horn_l", 0);
                mMotion.fn_8009373C("op_01_hornlight_l", 0);
            }
        }
        break;
    case 1:
        mStatus.m04 += (float)msg->m68;
        mStatus.fn_8013322C(0x20);
        if ((m16C0 -= msg->m68) <= 0 || msg->mData->m04 == 0x179A || msg->mData->m04 == 0x2710) {
            m16C0 = 0;
            msg->m94 = (msg->m94 & ~0x100) | 2;
            m14BC = 0;
            fn_80239DAC(lbl_8087F3C0, &m1610, 0, 0);
            fn_80232B7C(0, 0);
            {
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
                fn_8023A8B4(lbl_8087F3C0, &m161C, &mMotion, 0, pos, rot, color, -1, -1, 1, 4.0f);
            }
            fn_800C344C("SE_SP007_003", &mPos, 0, -1, 1.0f);
            if (lbl_8087F498 != 0) {
                fn_803EA77C(lbl_8087F498, this, 6, 0, 1.0f, 10.0f);
            }
            StartDown_unk();
        }
        break;
    }
}

// inferred: next index of a range table (inlined three times in fn_8033A7A0): the current one is clamped
// to the last element, its id is returned and the index wraps around
inline int EnemyQuarkSecond::NextInTable_unk(int range, int& index) {
    int i = index;
    if (i >= (int)fn_800E0AA8(&m14EC[m1558][range]) - 1) {
        i = fn_800E0AA8(&m14EC[m1558][range]) - 1;
    }
    index = i;
    int id = *fn_8028B74C(&m14EC[m1558][range], i);
    index = (unsigned int)(index + 1) % fn_800E0AA8(&m14EC[m1558][range]);
    return id;
}

// NONMATCHING: the original is over the inline budget (fn_8000D9E8, fn_8000DCF4, the Vector3 copy and
// operator-, fn_801C3910, fn_800E0AA8 and fn_8028B74C are called out of line); this compile inlines them
inline void EnemyQuarkSecond::fn_8033A7A0() {
    if (m14B0 == 0) {
        return;
    }
    fn_8000DCF4(fn_8000D9E8());
    float dist = fn_801C3910(mPos - atn::Vector3(*m14B0->fn_8013C38C()));
    int action = 0;
    if (m14BC != 2) {
        if (mStatus.m228 >= 1.0f) {
            action = 6;
        } else if (m1628 == 0 && m1558 > 0) {
            action = 9;
        } else if (m16B1 != 0) {
            m16B1 = 0;
            action = 8;
        } else if (m16C8 >= m16CC) {
            action = 7;
        } else if (m165C != 0) {
            if (m1660 == 0) {
                m1660 = 1;
                action = 5;
            } else {
                m1660 = 0;
                action = 3;
            }
            m165C = 0;
        } else if (m14C0 > m176C) {
            if (fn_803446BC(mPos) != 0) {
                action = 3;
            } else if (dist <= m177C) {
                action = NextInTable_unk(0, m155C);
            } else if (dist <= m1780) {
                action = NextInTable_unk(1, m1560);
            } else {
                action = NextInTable_unk(2, m1564);
            }
            if (++m1664 >= 3) {
                m165C = 1;
                m1664 = 0;
            }
        }
    }
    switch (action) {
    case 1:
        fn_80342BC8();
        break;
    case 2:
        fn_8034315C();
        break;
    case 3:
        fn_803435B8();
        break;
    case 4:
        fn_80342004();
        break;
    case 5:
        fn_80342674();
        break;
    case 6:
        fn_8034228C();
        break;
    case 7:
        fn_80342FB4();
        break;
    case 8:
        fn_80343B0C();
        break;
    case 9:
        fn_80344060();
        break;
    }
}

// inferred: front of the character at `dist` (inlined: its locals come first in the frame); name invented
inline atn::Vector3 Front_unk(float rotY, float dist) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rotY);
    PSMTXMultVec(m, &dir, &dir);
    return atn::Vector3(dir.x * dist, dir.y * dist, dir.z * dist);
}
// inferred: sets the collision capsule (name invented)
inline void SetCapsule_unk(atn::ColliCapsule* colli, const atn::ColliCapsuleShape_unk& c) {
    colli->m54 = c.mRadius;
    colli->m3C = c.mStart;
    colli->m48 = c.mEnd;
}

// NONMATCHING: ~97% - scheduling of the copy of the capsule against the rotation (8 variants)
void EnemyQuarkSecond::fn_8033AB88() {
    atn::ColliCapsuleShape_unk capsule(mColli.m3C, mColli.m48, mColli.m54);
    capsule.mStart = m614 + Front_unk(mRot.y, 10.0f);
    capsule.mStart.y -= 10.0f;
    SetCapsule_unk(&mColli, capsule);
}

// inferred: position of a node (as in game/Em301LShade.cpp); name invented
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: ~95% - stack slots of the temporaries (rotated front, node positions) against the
// Item14_unk constructor temporary; 7 variants
void EnemyQuarkSecond::vf50() {
    Item14_unk item;
    item.m04.mCenter = m614 + Front_unk(mRot.y, 10.0f);
    item.m04.mRadius = m620;
    item.m04.mCenter.y -= 10.0f;
    m624.push_back(item);
    item.m04.mRadius = 10.0f;
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    m624.push_back(item);
    item.m04.mRadius = 18.0f;
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
    m624.push_back(item);
    item.m04.mRadius = 18.0f;
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
    m624.push_back(item);
    mFlagB29 = 1;
}

// NONMATCHING: ~85% - stack order of the temporaries (the original keeps the rotated front in the
// upper slots) and the index register (r29 against r7/r0); 6 variants
void EnemyQuarkSecond::vf54() {
    if (m624.mSize == 0) {
        return;
    }
    int i = 0;
    m624.mData[i].m04.mRadius = m620;
    m624.mData[i].m04.mCenter = m614 + Front_unk(mRot.y, 10.0f);
    m624.mData[i].m04.mCenter.y -= 10.0f;
    i++;
    m624.mData[i].m04.mRadius = 10.0f;
    m624.mData[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    i++;
    m624.mData[i].m04.mRadius = 18.0f;
    m624.mData[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
    i++;
    m624.mData[i].m04.mRadius = 18.0f;
    m624.mData[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
}

atn::Vector3 EnemyQuarkSecond::vf64(int center) {
    return m624.mData->m04.mCenter;
}

// NONMATCHING: ~93% - the original keeps the distance in f31 from the start and the rotation
// temporaries in other slots; 6 variants
void EnemyQuarkSecond::fn_8033C000() {
    float dist = 0.0f;
    float speed = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 move = mRoute.m58;
        speed *= 0.2f;
        atn::Vector3 d = m14B0->mPos - mPos;
        dist = PSVECMag(&d) < 0.7f * m177C ? 0.0f : PSVECMag(&move);
        if (dist > 0.0001f) {
            atn::Vector3 dir = move;
            PSVECNormalize(&dir, &dir);
            rot = dir.ToRotation_unk();
            rot.x = 0.0f;
        } else {
            rot = mRot;
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    mPos.y = 0.0f;
    fn_8013CB68(&rot, 0, dist, m568 * speed);
}

void EnemyQuarkSecond::vf24() {
    if (m55C == 6) {
        return;
    }
    if (m570 <= 0.0f) {
        mMotion.mTracks[0].m0C = 2.0f;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
    } else {
        mMotion.mTracks[0].m0C = 2.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    }
}

void EnemyQuarkSecond::fn_8033C3D8() {
    if (m55C != 6) {
        if (m55C == 7) {
            if ((CharaTask*)mC58.mC8 != m14B0) {
                fn_8016E970(3);
                fn_80170A20(m14B0, 0, -1.0f);
            }
        } else {
            fn_8016E970(3);
            fn_80170A20(m14B0, 0, -1.0f);
        }
    }
    m6B8 = lbl_807C7030;
    m574 = lbl_807C7030;
    fn_8033C000();
}

void EnemyQuarkSecond::fn_80341E14() {
    ResetState_unk();
}

void EnemyQuarkSecond::fn_80341EE4() {
    ResetState_unk();
    fn_8016E970(3);
    m58C = 0;
    PlayMotion_unk(2, 1, 1.0f);
}

void EnemyQuarkSecond::fn_80344060() {
    ResetState_unk();
    fn_8016E970(3);
    m58C = 11;
    PlayMotion_unk(0x14D, 0, 1.0f);
    m1628 = 1;
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, this, 2, 0, 1.0f, 10.0f);
    }
}

extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_80063D3C(void* draw, const atn::Vector3& center, float radius, int color, float arg); // inferred: sphere

void EnemyQuarkSecond::fn_803441AC() {
    atn::Vector3 dir(0.0f, 0.0f, m1648);
    float angle = mRot.y;
    for (int i = 0; i < 180; i++) {
        angle += atn::DegToRad_unk(2.0f * (float)i);
        angle = atn::WrapAngle_unk(angle);
        Mtx m;
        PSMTXRotRad(m, 'y', angle);
        PSMTXMultVec(m, &dir, &dir);
        fn_800FAB80(lbl_8087F048, this, m14D8, m1658, m164C + dir, atn::Vector3(0.0f, angle, 0.0f), 0, 30, -1, -1,
                    3.0f, 1.0f);
        if (m1760 != 0) {
            fn_80063D3C(lbl_8087EEB0, m164C + dir, m14D8->m58, 0xFF000000, 1000.0f);
        }
    }
    if (m1648 >= m1768) {
        fn_80239DAC(lbl_8087F3C0, this, 0x3E9, 0);
        m1644 = 0;
    }
    m1648 += m1764;
}

void EnemyQuarkSecond::fn_8034441C(int motion, int loop, float speed) {
    PlayMotion_unk(motion, loop, speed);
}

extern "C" double acos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// inferred: gimmicks of the field (lbl_8087F4A0, as in game/SpecialEnemyBase.cpp); names invented
struct QuarkGimmickSize_unk {
    unsigned char m00[0x48];
    float m48; // inferred: radius
};
// inferred: message sent to a gimmick (0x30 bytes, kind then the sender at 0x28)
struct QuarkGimmickMsg_unk {
    int mKind;
    unsigned char m04[0x28 - 0x04];
    CharaTask* mSender;
    unsigned char m2C[0x30 - 0x2C];
};
struct QuarkGimmick_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44(QuarkGimmickMsg_unk* msg);
    virtual void vf48();
    virtual void vf4C();
    virtual QuarkGimmickSize_unk* vf50();
    virtual atn::Vector3 vf54();
    unsigned char m04[0x48 - 0x04];
    unsigned int m48; // inferred: kind (0xBB81, 0xBB84 here)
    unsigned char m4C[0x54 - 0x4C];
    int m54;
    unsigned char m58[0x5C - 0x58];
    QuarkGimmick_unk* mNext;
    unsigned char m60[0x920 - 0x60];
    float m920;
};
struct QuarkGimmickList_unk {
    unsigned char m00[0x48];
    QuarkGimmick_unk* mHead;
};
extern "C" QuarkGimmickList_unk* lbl_8087F4A0;

void EnemyQuarkSecond::fn_80344454(int msg, int notify, float radius, float angle) {
    for (QuarkGimmick_unk* g = lbl_8087F4A0->mHead; g != 0; g = g->mNext) {
        int kind = g->m48;
        if (kind == 0xBB84 || kind == 0xBB81) {
            QuarkGimmickMsg_unk m;
            Mtx rot;
            atn::Vector3 pos = g->vf54();
            atn::Vector3 dir;
            atn::Vector3 front;
            atn::Vector3 node = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
            float dz = pos.z - node.z;
            float dx = pos.x - node.x;
            dir.x = dx;
            dir.z = dz;
            dir.y = 0.0f;
            float dist = fn_801C3910(dir);
            QuarkGimmickSize_unk* size = g->vf50();
            if (size != 0) {
                dist -= size->m48;
            }
            if (dist <= radius) {
                PSVECNormalize(&dir, &dir);
                front.Set_unk(0.0f, 0.0f, 1.0f);
                PSMTXRotRad(rot, 'y', mRot.y);
                PSMTXMultVec(rot, &front, &front);
                front.y = 0.0f;
                float a = acos(PSVECDotProduct(&dir, &front));
                if (a <= 0.5f * angle && notify != 0) {
                    memset(&m, 0, sizeof(m));
                    m.mKind = msg;
                    m.mSender = this;
                    g->vf44(&m);
                }
            }
            if (dist <= 2.0f * radius && g->m54 == 3) {
                g->m920 = 0.0f;
            }
        }
    }
}

int EnemyQuarkSecond::fn_803446BC(const atn::Vector3& pos) {
    atn::Vector3 d = pos - m1598;
    return (float)sqrt(d.x * d.x + d.z * d.z) >= 380.0f;
}

// inferred: debug menu entries (as in game/Em315Coffin.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, float min, float max, float step,
                            int arg6, int arg7);

// NONMATCHING: 99.8% - only the offsets of the strings in @stringBase0 (the strings of the state
// functions, not written yet, come before them in the pool)
void EnemyQuarkSecond::fn_80344780() {
    void* group = m175C.Load("Quark");
    fn_80087994(group, "DebugDisp", &m1760, 0, 0);
    fn_800874C8(group, "AttackWaitTime", &m176C, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "GatheringBurstCount", &m16CC, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "DownCount", &m16A4, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "StundUpDamageCount", &m16A8, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "AttakedInStick", &m1770, 1, 999, 1, 0, 0);
    fn_8008771C(group, "ShortRange", &m177C, 0.0f, 9999.0f, 0.01f, 0, 0);
    fn_8008771C(group, "MiddleRange", &m1780, 0.0f, 9999.0f, 0.01f, 0, 0);
    fn_8008771C(group, "LongRange", &m1784, 0.0f, 9999.0f, 0.01f, 0, 0);
    fn_8008771C(group, "SwordDirectionLine", &m1774, 0.0f, 1000.0f, 0.01f, 0, 0);
    fn_800874C8(group, "SwordThrowTime", &m1778, 0, 999999, 1, 0, 0);
    fn_8008771C(group, "ShockExpandSonicWave", &m1764, 0.0f, 100.0f, 0.01f, 0, 0);
}

void EnemyQuarkSecond::fn_803449AC() {
    BattleSlots_unk* slots = fn_80103F60(lbl_8087F048, this);
    if (m14C8 != 0) {
        CharaTask* c = lbl_8087F8A0->mHead;
        if (!c->mFlag28) {
            for (; c != 0; c = fn_802A4094(c)) {
                int i = fn_80108378(slots, c);
                if (c == m14C4) {
                    slots->m128[i] = 100.0f;
                } else {
                    slots->m128[i] = 0.0f;
                }
            }
            return;
        }
    }
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = fn_802A4094(c)) {
        slots->m128[fn_80108378(slots, c)] = 1.0f;
    }
}

int EnemyQuarkSecond::vf78() {
    if (m14B0 != 0) {
        switch (m58C) {
    case 6:
        return mMotion.mTracks[0].m08 >= 23.0f && mMotion.mTracks[0].m08 <= 24.0f;
    case 7:
        return mMotion.mTracks[0].m08 >= 43.0f && mMotion.mTracks[0].m08 <= 45.0f;
    case 9:
        return mMotion.mTracks[0].m08 >= 61.0f && mMotion.mTracks[0].m08 <= 63.0f;
    case 10:
        return mMotion.mTracks[0].m08 >= 7.0f && mMotion.mTracks[0].m08 <= 9.0f;
        }
    }
    return 0;
}

int EnemyQuarkSecond::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    return m14BC == 1 ? 3 : 0;
}

int EnemyQuarkSecond::vf68(CharaTask* attacker) {
    if (m14BC == 2 && attacker->mType == 0) {
        return 9;
    }
    return 0;
}

void EnemyQuarkSecond::vf6C(int kind, CharaTask* other) {
    atn::Vector3 dir(mPos.x - other->mPos.x, mPos.y, mPos.z - other->mPos.z);
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);
    if (kind == 9) {
        ResetState_unk();
        fn_8016E970(3);
        m58C = 17;
        PlayMotion_unk(0x1E2, 0, 1.0f);
        fn_80232B7C(0, 0);
        {
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
            fn_8023A8B4(lbl_8087F3C0, &m1674, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        }
        fn_80354110(m1758);
    }
}

void EnemyQuarkSecond::fn_80344DF4() {
    m16B4 = 1;
    fn_80339F34(this, 1);
    fn_8016E4C4(0);
    (*fn_80339F24(&m6A4, 0))->mFlags.mFlag = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x16E, 0, 1, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 0.05f;
}

extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

atn::Vector3 EnemyQuarkSecond::vfD8(CharaTask* target) {
    atn::Vector3 dir = target->mPos - mPos;
    atn::Vector3 from = target->mPos;
    atn::Vector3 result;
    if (fn_805F9920(&dir) > 0.001f) {
        PSVECNormalize(&dir, &dir);
    } else {
        dir.Set_unk(0.0f, 0.0f, 1.0f);
    }
    result = from - dir * 25.0f;
    for (unsigned int i = 0; i < 8; i++) {
        Mtx m;
        PSMTXRotRad(m, 'y', 0.25f * (3.1415927f * (float)i));
        atn::Vector3 d = dir;
        PSMTXMultVec(m, &d, &d);
        atn::Vector3 to = from;
        to.y += 5.0f;
        atn::Vector3 at = to - d * (5.0f + 25.0f);
        unsigned char hit[0x50];
        if (fn_8004ED34(lbl_8087EE98, hit, &to, &at, 0x80000000, 0, 0) == 0) {
            result = from - d * 25.0f;
            break;
        }
    }
    return result;
}

float EnemyQuarkSecond::vfDC() {
    return 0.7f;
}

// inferred: game settings (lbl_8087F0A8, as in game/CharaTask.cpp): frames per second at 0x30
struct QuarkSettings_unk {
    unsigned char m00[0x30];
    int m30;
};
extern "C" QuarkSettings_unk* lbl_8087F0A8;

// inferred: the motion of track 0 has reached its end (the frame is read before the length); name
// invented
inline bool EnemyQuarkSecond::IsMotionEnd_unk() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        return true;
    }
    return false;
}

// inferred: falls to the ground (inlined in the states 12 and 13); name invented
inline void EnemyQuarkSecond::Fall_unk() {
    int fps = lbl_8087F0A8->m30;
    m574.y += -98.1f / (float)(fps * fps);
    mPos.y += m574.y;
    if (mPos.y < 0.0f) {
        mPos.y = 0.0f;
        m574.y = 0.0f;
    }
}

void EnemyQuarkSecond::fn_80340E54() {
    Fall_unk();
    float frame;
    if (m14B8 >= 150 || (frame = mMotion.mTracks[0].m08, frame >= mMotion.fn_80097D7C(0))) {
        fn_80341EE4();
    }
}

void EnemyQuarkSecond::fn_8034100C() {
    Fall_unk();
    float frame;
    if (m14B4 == 0) {
        if ((frame = mMotion.mTracks[0].m08, frame >= mMotion.fn_80097D7C(0))) {
            PlayMotion_unk(0x3E, 1, 1.0f);
            m14B4 = 1;
            m14B8 = 0;
            m16AC = 0;
        }
    } else if (m14B4 == 1) {
        if (m14BC != 2 && (m14B8 >= m16A4 || m16AC > m16A8)) {
            PlayMotion_unk(0x3F, 0, 1.0f);
            m14B4 = 2;
        }
    } else if (m14B4 == 2) {
        if ((frame = mMotion.mTracks[0].m08, frame >= mMotion.fn_80097D7C(0))) {
            fn_80341EE4();
        }
    }
}

// inferred: plays `efp` on the model with the default color (inlined: its locals are laid out in
// declaration order, as PlayEffectB_unk of game/Em301LShade.cpp); name invented
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model, float scale) {
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, scale);
}

// NONMATCHING: 99.9% - only the offset of "SE_EM211_012" in @stringBase0 (the strings of the
// functions not written yet come before it)
void EnemyQuarkSecond::fn_80341BEC() {
    if (m14B4 == 0) {
        if (mMotion.mTracks[0].m08 >= 35.0f) {
            m14B4++;
            m14BC = 1;
            fn_80232B7C(&m1610, 0);
            PlayEffect_unk(&m1610, &mMotion, 4.0f);
            fn_800C344C("SE_EM211_012", &mPos, 0, -1, 1.0f);
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0) - 1.0f) {
            fn_80341EE4();
        }
    }
}

// NONMATCHING: ~80% - the original frame is 0x10 bytes bigger (the node positions and the
// difference live in other slots); 6 variants
void EnemyQuarkSecond::fn_80342004() {
    ResetState_unk();
    fn_8016E970(3);
    m58C = 6;
    fn_80232B7C(this, 0x3E8);
    PlayMotion_unk(0x147, 0, 2.0f);
    atn::Vector3 target;
    atn::Vector3 dir;
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, &m15B0, &mMotion, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
    target = GetNodePos_unk(m14B0->mMotion.GetNodeMtx_unk("spine"));
    m15D4 = target - GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    PSVECNormalize(&m15D4, &m15D4);
    fn_800C31F4("SE_EM211_009_1", 0, -1, 1.0f);
}

// inferred: starts a jump to `target` (inlined in the jump attacks); name invented
inline void EnemyQuarkSecond::SetJump_unk(const atn::Vector3& target) {
    atn::Vector3 t = target;
    m1568.mTarget = t;
    m1568.mTarget.y = 0.0f;
    atn::Vector3 rot = (t - mPos).ToRotation_unk();
    rot.z = 0.0f;
    m1568.mRot = rot;
    atn::Vector3 dir = t - mPos;
    dir.y = 0.0f;
    m1568.mDir = dir;
    PSVECNormalize(&m1568.mDir, &m1568.mDir);
    m1568.mStart = mPos;
    m570 = 0.0f;
}

// NONMATCHING: ~80% - draft: the original keeps more temporaries of the jump (the target is copied
// through two Vector3 and the rotation is computed with the frame of ToRotation_unk in other slots)
void EnemyQuarkSecond::fn_8034228C() {
    ResetState_unk();
    fn_8016E970(3);
    m58C = 7;
    m1648 = 0.0f;
    m1644 = 0;
    PlayMotion_unk(0x13F, 0, 1.0f);
    SetJump_unk(m1598);
}

extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* owner, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: fires the attack data

// NONMATCHING: 99.6% - f30/f31 of the turn target (the matrix of ToRotationZ_unk takes the other one);
// 10 variants
void EnemyQuarkSecond::fn_8033D750() {
    float frame = mMotion.mTracks[0].m08;
    float target;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80341EE4();
        return;
    }
    if (mMotion.mTracks[0].m08 < 38.0f) {
        CharaTask* t = m14B0;
        target = (t->mPos - mPos).ToRotationZ_unk().y;
        float d = atn::WrapAngle_unk(target - mRot.y);
        if ((d < 0.0f ? -d : d) <= 0.20943952f) {
            mRot.y = target;
        } else if (d <= 0.0f) {
            mRot.y -= 0.20943952f;
        } else {
            mRot.y += 0.20943952f;
        }
    }
    if (mMotion.mTracks[0].m08 >= 47.0f && mMotion.mTracks[0].m08 < 49.0f) {
        fn_800F8C6C(lbl_8087F048, 0, 0, this, m14E0, m590, 30, -1, 0.0f);
    }
}

// inferred: same as PlayEffect_unk with the locals declared pos, rot, color (as PlayEffect_unk of
// game/Em301LShade.cpp); name invented
inline void PlayEffectA_unk(atn::EfpHandle_unk* efp, Model_unk* model, float scale) {
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, scale);
}

void EnemyQuarkSecond::fn_80342FB4() {
    ResetState_unk();
    fn_8016E970(3);
    m58C = 14;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x15C, 0, 1.0f);
    m16C8 = 0;
    fn_80232B7C(this, 0x3F0);
    PlayEffectA_unk(&m15A4, &mMotion, 1.0f);
}
