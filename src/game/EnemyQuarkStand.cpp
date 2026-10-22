#include "game/EnemyQuarkStand.h"
#include "atn/Csv.h"
#include "chr/ControlStickByQuark.h"
#include <boost/bind.hpp>
#include "atn/D2A_Task.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" double sqrt(double x);
extern "C" double acos(double x);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: field (lbl_8087F430): camera taken by the boss (m8A0/m4D8/m97C, as in game/EnemyTrista.cpp)
struct RouteTable_unk;
struct Field_unk {
    RouteTable_unk* fn_8013C504(); // inferred: route tables (as in game/Em302Kraken.cpp)
    unsigned char m0000[0x4D8];
    int m4D8;
    unsigned char m4DC[0x8A0 - 0x4DC];
    CharaTask* m8A0;
    unsigned char m8A4[0x96C - 0x8A4];
    int m96C;   // inferred: camera shake (as in game/Em101ElderZesyaSecond.cpp)
    int m970;   // inferred: shake frames
    float m974;
    float m978;
    unsigned char m97C;
    unsigned char m97D[0x10D8 - 0x97D];
    RouteTable_unk* m10D8; // inferred: route tables (as in game/ArenaTask.cpp)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" Field_unk* fn_80121F00(); // inferred: field accessor
extern "C" void fn_80370320(Field_unk* field, int id, int a, int b); // inferred: event flag of the area
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound
// inferred: effect manager (lbl_8087F3C0) and its methods (as in game/EnemyTasha.cpp)
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
namespace atn {
// inferred: effect manager (atn/EffectManager.cpp); only the member called here is declared
class EffectManager {
public:
    void fn_8023A254(unsigned int owner, int group, float rate); // inferred: speed of the effects
};
} // namespace atn
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: effect/bullet manager (lbl_8087F048)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
Effect_unk* fn_8013A194(); // inferred: bullet manager (returns lbl_8087F048)
extern "C" Bullet* fn_801010A0(Effect_unk* mgr); // inferred: a free bullet
extern "C" float fn_8010B250(Effect_unk* mgr, int kind, int arg); // inferred: scale of the bullet
extern "C" int fn_800EFD04(int id); // inferred: effect resources of a bullet
extern "C" int fn_800EFDA0(int id);
extern "C" void fn_8010EE78(Bullet* bullet, BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir,
                            CharaTask* owner, int id, int arg, float speed); // inferred: fires the bullet
extern "C" float fn_800F8524(const float* a, const float* b, float t); // inferred: lerp of two floats
extern "C" EffectMgr_unk* fn_800F7FA0(); // inferred: returns lbl_8087F3C0
extern "C" void fn_800F7FA8(EffectMgr_unk* mgr, void* owner, int arg); // inferred: fn_80232B7C(owner, arg)
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, void* mtx, int arg5,
                            int arg6, int arg7, int arg8, int arg9, int arg10, float scale);
extern "C" void fn_80276AE4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, void* model, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int b, int c, float scale);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet at a point
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume); // 3D sound
// inferred: game state (lbl_8087EFA8): the frame time at 0x3A4 (as in game/Em301LShade.cpp)
struct QuarkGame_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" QuarkGame_unk* lbl_8087EFA8;
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_80063D3C(void* dbg, const atn::Vector3* pos, unsigned int color, float radius, float arg);
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* colli, void* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: segment test
extern "C" void fn_8023A108(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: effect fade
// inferred: weak copy of an accessor of game/EnemyQuarkSecond (bit 31 of the word at 8 of a model
// part); name kept
extern "C" void fn_80338CDC(void* part, int on);
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const struct QColor_unk& color,
                            int arg7, int arg8, int arg9, float scale);
// inferred: color as four floats (as Color4f_unk of game/EnemyTrista.cpp)
struct QColor_unk {
    QColor_unk() {}
    float r, g, b, a;
};

// inferred: game settings (lbl_8087F0A8): m30 is the frame rate (as in chr/ControlJumpAt.cpp)
struct QuarkSettings_unk {
    unsigned char m000[0x30];
    int mFrameRate;
};
extern "C" QuarkSettings_unk* lbl_8087F0A8;
// inferred: weak copies of small inline helpers of other files, called out of line because vf0C is over
// the inline budget (names kept)
extern "C" void fn_803396EC(CharaTask* chara, unsigned int flags);   // inferred: m54C |= flags
extern "C" void fn_80288E30(CharaTask* chara, float value);          // inferred: m56C = value
extern "C" void fn_80244CAC(atn::Task* task);                        // inferred: mFlags |= 4
extern "C" CharaManager<PlayerTask>* fn_8000D9E8();
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager                  // inferred: player manager
// inferred: more weak copies of small inline helpers called out of line by vf10 (names kept)
extern "C" void fn_80339F34(CharaTask* chara, int on);           // inferred: bit 0x2000 of m54C
extern "C" void fn_80339F5C(Model_unk* model, int track, float speed); // inferred: speed of a track
extern "C" void fn_80339F74(CharaTask* chara, int value);        // inferred: m1454 = value
struct QuarkMapGroup_unk;
extern "C" QuarkMapGroup_unk* fn_80373148(Field_unk* field);     // inferred: map group of the field
extern "C" int fn_80339F7C(QuarkMapGroup_unk* group);            // inferred: returns m0E0
extern "C" void fn_80339F84(QuarkMapGroup_unk* group, int value); // inferred: m0E8 = value
extern "C" void fn_804A04AC(QuarkMapGroup_unk* group, int mode, int a, int b, int c); // inferred: map light mode
extern "C" float fn_8029F3AC(CharaStatus* status);               // inferred: hit point ratio
extern "C" void fn_80265804(CharaStatus* status, float value);   // inferred: m128 = value
extern "C" CharaTask* fn_8000DCF4(CharaManager<PlayerTask>* mgr); // inferred: leader
extern "C" CharaTask* fn_802A36B0(CharaManager<PlayerTask>* mgr); // inferred: first player
extern "C" CharaTask* fn_802A4094(CharaTask* chara);             // inferred: next player
extern "C" int fn_80267B28(CharaTask* chara);                    // inferred: returns m560
struct QuarkKeyboard_unk;
extern "C" QuarkKeyboard_unk* fn_802A49BC();                     // inferred: debug keyboard
extern "C" int fn_802A4968(QuarkKeyboard_unk* kb, int pad, int key); // inferred: key pressed
class EnemyQuarkStand;
extern "C" void fn_80353EE4(EnemyQuarkStand* self); // inferred: releases the camera
extern "C" void fn_80353F0C(EnemyQuarkStand* self); // inferred: frees the character stuck by the sword
// inferred: {id, value} pair of CharaStatus::m2D0 (as CharaStatusPair_unk); 0x80339718 builds it
struct QuarkPair_unk {
    int m00;
    int m04;
};
extern "C" QuarkPair_unk* fn_80339718(QuarkPair_unk* pair, int id);
extern "C" void fn_803396FC(CharaStatus* status, int index, const QuarkPair_unk* pair);
extern "C" HitInfo_unk* fn_80219E6C(int id);
extern "C" void* lbl_8087F498; // inferred: voice manager
extern "C" void fn_803EBAC8(void* mgr, int a, int b); // inferred: voice mode (as in Event/Dialog.cpp)
extern "C" void* lbl_8087EFE8; // inferred: sound manager
extern "C" void fn_800CF45C(void* mgr, int a, int b); // inferred: sound mode
extern "C" void fn_803EA77C(void* mgr, CharaTask* chara, int id, int arg, float a, float b); // inferred: voice

// inferred: inline constructors of the helper members (weak copies 0x8034585C-0x803458E0)
inline QuarkStandState_unk::QuarkStandState_unk()
    : m00(0), m04(0), m0C(0), m10(0), m14(0), m18(0), m1C(0), m20(0), m24(0), m28(0), m30(0), m34(0), m38(0),
      m3C(0.0f), m40(0), m44(0.0f), m48(0), m4C(0), m50(0), m54(0), m58(0), m5C(0) {}
inline QuarkSlot_unk::QuarkSlot_unk() : m14(0) {}
inline QuarkDir_unk::QuarkDir_unk() {}
inline Mtx34_unk::Mtx34_unk() {}
inline QuarkShot_unk::QuarkShot_unk() {}
inline QuarkBulletShot_unk::QuarkBulletShot_unk() {
    m00 = 0.1f;
    m04 = 0;
    m08 = 0;
    m10 = 0;
    m14 = 0.0f;
    m18 = 1.0f;
}

EnemyQuarkStand::EnemyQuarkStand(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m153C(0),
      m1550(0), m15CC(0), m15D0(0), m15D4(0), m15D8(0), m16D8(0), m16DC(0.0f), m17E4(0), m17E8(0), m17F0(0),
      m17FC(150), m1800(10), m1804(0), m1808(0), m1809(0), m180A(0), m180B(0), m1814(0), m1818(15), m1CB8(0),
      m1CBC(0), m1CC4(0), m1CC8(8.0f), m1CCC(300.0f), m1CD4(10), m1CD8(150.0f), m1CDC(30) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_enemy_quark_stand.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "db_name=", 8) == 0) {
            name = it->c_str() + 8;
        }
    }
    std::string path = fn_8006AFF8(dir, name);
    m180C.vf0C(path.c_str());
    m1624.fn_8023780C("ef_bs072f.efp");
    m1630.fn_8023780C("ef_bs070d.efp");
    m163C.fn_8023780C("ef_bs070a.efp");
    m1648.fn_80237654("ef_bs070e_01.eff");
    m16C0.fn_8023780C("ef_bs058a.efp");
    m16CC.fn_8023780C("ef_bs058c.efp");
    m17D8.fn_80237654("ef_bs051b.eff");
    m1618.fn_8023780C("ef_bd008b.efp");
    m1690.fn_8023780C("ef_bd009a.efp");
    m169C.fn_8023780C("ef_bd009b.efp");
    m16A8.fn_8023780C("ef_bs078c.efp");
    m16B4.fn_8023780C("ef_bs080a.efp");
    m1CAC.fn_8023780C("ef_bs049e.efp");
    m1704.vf0C("/camera/battle/cm000_em211_00.xsca");
    m170C.vf0C("/camera/battle/cm000_em211_01.xsca");
    m17F4 = fn_801F3FF8(this, "/data/d2anime/ui_direction_l.d2a", 0);
    m17F4->fn_800D246C(1);
    m17F8 = fn_801F3FF8(this, "/data/d2anime/ui_direction_r.d2a", 0);
    m17F8->fn_800D246C(1);
    fn_80370320(fn_80121F00(), 0x65, 1, 0);
    fn_80370320(fn_80121F00(), 0x68, 1, 0);
    fn_80338CDC(&mColli, 0);
    m160C.Set_unk(0.0f, 0.0f, 0.0f);
    mFlagB11 = 1;
    mFlag24 = 0;
}

// inferred: reads the skill ids of a line of the data file until a zero (names invented)
inline void ReadIds_unk(IntVector_unk* list, atn::Csv* csv) {
    while (true) {
        int id = fn_800DC12C(csv->fn_8005B9CC());
        if (id == 0) {
            break;
        }
        list->push_back(id);
    }
}
// NONMATCHING: 99.73% - inlined vector<int>::push_back (size/capacity loads swapped, known family) and the
// "Memory allocation failure" literal of another file (lbl_80775A88)
extern "C" void fn_803458EC(IntVector_unk* list, atn::Csv* csv) {
    ReadIds_unk(list, csv);
}

int EnemyQuarkStand::vf0C() {
    if (fn_8013655C() == 0 && m1624.fn_80237874() == 0 && m1630.fn_80237874() == 0 &&
        m163C.fn_80237874() == 0 && m1648.fn_802376D0() == 0 && m16CC.fn_80237874() == 0 &&
        m16C0.fn_80237874() == 0 && m17D8.fn_802376D0() == 0 && m1704.fn_80473F50() == 0 &&
        m170C.fn_80473F50() == 0 && m180C.fn_80473F50() == 0 && m1618.fn_80237874() == 0 &&
        m1690.fn_80237874() == 0 && m169C.fn_80237874() == 0 && m16A8.fn_80237874() == 0 &&
        m16B4.fn_80237874() == 0 && m1CAC.fn_80237874() == 0 && fn_800D3FA4() != 0) {
        fn_80353370();
        fn_803396EC(this, 0x200);
        fn_80288E30(this, 0.03f);
        mMotion.fn_80099E9C("hips", "em211", 4.3f);
        m17F4->fn_800D246C(0);
        fn_80244CAC(m17F4);
        m17F8->fn_800D246C(0);
        fn_80244CAC(m17F8);
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x15;
        mStatus.m18 |= 0x400000;
        mStatus.m18 |= 0xC208;
        mStatus.m18 |= 0x3880000;
        mStatus.m18 |= 0x400;
        RouteTable_unk* routes = fn_80121F00() != 0 ? fn_80121F00()->fn_8013C504() : 0;
        if (m180C.fn_80470580() != 0 && routes != 0) {
            atn::Csv csv((const char*)m180C.fn_80470580(), m180C.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "Manamia") == 0) {
                    int id = fn_800DC12C(csv.fn_8005B9CC());
                    m14C4 = fn_8000D9E8()->Find_unk(id);
                } else if (strcmp(key, "ShockExpandSnicWave") == 0) {
                    m1CC8 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackWaitTime") == 0) {
                    m1CD0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GatheringBurstCount") == 0) {
                    m1818 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "DownCount") == 0) {
                    m17FC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "StundUpDamageCount") == 0) {
                    m1800 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackedMaxInStick") == 0) {
                    m1CD4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "SwordDirectionLine") == 0) {
                    m1CD8 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "SwordThrowTime") == 0) {
                    m1CDC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ShortRange") == 0) {
                    m1CE0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "MiddleRange") == 0) {
                    m1CE4 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "LongRange") == 0) {
                    m1CE8 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "ShortRangeTableMode1") == 0) {
                    fn_803458EC(&m1560[0], &csv);
                } else if (strcmp(key, "MiddleRangeTableMode1") == 0) {
                    fn_803458EC(&m1560[1], &csv);
                } else if (strcmp(key, "LongRangeTableMode1") == 0) {
                    fn_803458EC(&m1560[2], &csv);
                } else if (strcmp(key, "ShortRangeTableMode2") == 0) {
                    fn_803458EC(&m1560[3], &csv);
                } else if (strcmp(key, "MiddleRangeTableMode2") == 0) {
                    fn_803458EC(&m1560[4], &csv);
                } else if (strcmp(key, "LongRangeTableMode2") == 0) {
                    fn_803458EC(&m1560[5], &csv);
                } else if (strcmp(key, "ShortRangeTableMode3") == 0) {
                    fn_803458EC(&m1560[6], &csv);
                } else if (strcmp(key, "MiddleRangeTableMode3") == 0) {
                    fn_803458EC(&m1560[7], &csv);
                } else if (strcmp(key, "LongRangeTableMode3") == 0) {
                    fn_803458EC(&m1560[8], &csv);
                } else if (strcmp(key, "LaserSkillID") == 0) {
                    m152C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "LaserExplodeSkillID") == 0) {
                    m1530 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "TackleSkillID") == 0) {
                    m1534 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m1538 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SonicSkillID") == 0) {
                    m153C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "HomingSkillID") == 0) {
                    m1540 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "AttackSkill") == 0) {
                    m1544 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m1548 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "StickSkill") == 0) {
                    m154C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "HoneSlashSkill") == 0) {
                    m1550 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BurstSkill") == 0) {
                    m1528 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "ThunderSkill") == 0) {
                    m155C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "RainSkill") == 0) {
                    // inferred: the original advances a copy of `this` by 4 and stores at 0x1554 (an
                    // index or a pointer into m1554 give other registers)
                    EnemyQuarkStand* p = this;
                    while (true) {
                        int id = fn_800DC12C(csv.fn_8005B9CC());
                        if (id == 0) {
                            break;
                        }
                        p->m1554[0] = fn_80219E6C(id);
                        p = (EnemyQuarkStand*)((char*)p + 4);
                    }
                }
            } while (csv.fn_8005BBF8());
        }
        if (m153C != 0) {
            QuarkPair_unk pair;
            fn_803396FC(&mStatus, 3, fn_80339718(&pair, m153C->m04));
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 41.9% - the original is over the inline budget and calls the state helpers
// (fn_8034F41C, fn_8034F384, ...) and the accessors out of line; ours stays under it while most of the
// states are still in asm
void EnemyQuarkStand::vf10() {
    if (m1809 != 0) {
        if (mMotion.fn_80139550(0) >= 110.0f) {
            fn_8016E4C4(1);
            float end = mMotion.fn_80097D7C(0);
            if (mMotion.fn_80139550(0) >= end) {
                m1809 = 0;
                fn_80339F34(this, 0);
                fn_8034F41C();
            }
        }
        fn_80145334();
        fn_803470F0();
        return;
    }
    if (m16D8 != 0) {
        fn_8035223C();
    }
    fn_803524E4();
    fn_803526A8();
    m500 = 0.01f;
    m504 = 0.01f;
    m508 = 1.0f;
    m14B8++;
    m14C0++;
    m14B0 = (int)mC58.mC4;
    fn_8035359C();
    if (m14B0 == 0) {
        CharaTask* leader = fn_8000DCF4(fn_8000D9E8());
        m14B0 = (int)leader;
        mC58.mC4 = leader;
    }
    fn_80352EA4();
    if (fn_80370A78(fn_80121F00(), 0x8C) != 0) {
        if (m58C == 15) {
            fn_8016E970(3);
            fn_803524AC(0x3E, 1, 1.0f);
            fn_80339F5C(&mMotion, 0, 1.0f);
            m14B4 = 1;
            m14B8 = 0;
        } else {
            fn_8016E970(3);
            fn_8034F41C();
            fn_8034F384();
            fn_80353EE4(this);
            fn_80353F0C(this);
            for (CharaTask* p = fn_802A36B0(fn_8000D9E8()); p != 0; p = fn_802A4094(p)) {
                if (fn_80267B28(p) == 0x53 && p->CanAct_unk()) {
                    p->fn_8015ECC4();
                }
            }
        }
        fn_80370AE4(fn_80121F00(), 0x8C, 0);
    }
    if (fn_8029F3AC(&mStatus) < 0.5f) {
        m15CC = 2;
    } else if (fn_8029F3AC(&mStatus) < 0.8f) {
        m15CC = 1;
    } else {
        m15CC = 0;
    }
    if (m15CC > 1) {
        fn_80265804(&mStatus, 1.2f);
    } else if (m15CC > 0) {
        fn_80265804(&mStatus, 0.7f);
    } else {
        fn_80265804(&mStatus, 0.0f);
    }
    CharaTask* leader = fn_8000DCF4(fn_8000D9E8());
    if (m1808 == 0 && leader->fn_8013C38C()->y >= 25.0f) {
        m1808 = 1;
    }
    int camera = 0;
    if (mC58.mC0 == 0 || m14B0 == 0) {
        if (m58C == 13) {
            fn_8034DE90();
        } else {
            fn_8016E970(3);
            fn_80339F74(this, 1);
        }
    } else {
        switch (m58C) {
        case 6:
            fn_80348FF4();
            break;
        case 7:
            fn_803494AC();
            break;
        case 8:
            fn_80349BA0();
            break;
        case 9:
            fn_8034A194();
            break;
        case 10:
            fn_8034A644();
            break;
        case 11:
            fn_8034AE48();
            camera = 1;
            break;
        case 12:
            fn_8034B8CC();
            camera = 1;
            break;
        case 13:
            fn_8034DE90();
            break;
        case 14:
            fn_8034E898();
            break;
        case 15:
            fn_8034EA50();
            break;
        case 16:
            fn_8034ECE8();
            break;
        case 18:
            fn_8034C714();
            camera = 1;
            break;
        case 19:
            fn_8034CF48();
            camera = 1;
            break;
        case 20:
            fn_8034DA0C();
            camera = 1;
            break;
        default:
            fn_80348568();
            fn_80346D8C();
            camera = 1;
            break;
        }
        fn_80339F74(this, m58C - 6U > 7);
        QuarkMapGroup_unk* group = fn_80373148(fn_80121F00());
        int mode;
        if (mPos.y - fn_8000DCF4(fn_8000D9E8())->fn_8013C38C()->y > 20.0f) {
            mode = 3;
        } else {
            mode = 1;
        }
        if (mode != fn_80339F7C(group)) {
            fn_804A04AC(group, mode, 15, -1, 0);
            fn_80339F84(group, mode);
        }
    }
    fn_8014C540();
    fn_80145334();
    fn_803470F0();
    fn_80352C3C(100, camera, 45.0f, 6.2831855f);
    if (m58C == 12 && m14B4 == 1 && m17E4 != 0 && m17EC == 0) {
        fn_80144710();
        fn_8000D430(m17E4->GetModel_unk(), boost::function<void()>());
        m17E4->fn_80148B38();
    }
    if (m1CC4 != 0) {
        if (fn_802A4968(fn_802A49BC(), 0, 0x4C)) {
            fn_8034F53C();
        }
        if (fn_802A4968(fn_802A49BC(), 0, 0x44)) {
            fn_8034F788();
        }
        if (fn_802A4968(fn_802A49BC(), 0, 0x4D)) {
            fn_80350F64();
        }
        if (fn_802A4968(fn_802A49BC(), 0, 0x48)) {
            fn_803520D8();
            mStatus.m04 = 1.0f;
        }
    }
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetTransOf_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: plays an effect on the model (as PlayEffect_unk of game/EnemyTrista.cpp)
inline void QuarkPlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    QColor_unk color;
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

// inferred: the state 1 of the vf3C reaction (hits while rising); the hit is reduced to nothing
void EnemyQuarkStand::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m1809 == 0) {
        msg->m10 *= 0.0f;
        msg->m50 |= 0x10;
        if (m14BC != 1) {
            *(int*)&msg->m3C[4] = 0; // inferred: HitMsg_unk+0x40 (as in game/Em101ElderZesya.cpp)
            fn_80151448(msg);
            vf40(hit);
        }
    }
}

// inferred: plays motion `motion` from the start (name invented)
inline void QuarkPlayMotion_unk(Model_unk* model, int motion, int arg3, int arg4, float speed) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = speed;
    model->fn_80097C08(0, motion, arg3, arg4, 1, 0.0f, 0.1f);
}

void EnemyQuarkStand::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 15) {
        if (msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
            m1804++;
        }
    } else if (m58C == 12) {
        m17E8++;
    }
    switch (m14BC) {
    case 0:
        m1814++;
        if (mStatus.m04 <= 0.0f) {
            m14BC = 1;
            ReleaseCamera_unk();
            mStatus.m04 = 1.0f;
            mStatus.fn_8013322C(0x20);
            fn_8034F384();
            fn_8016E970(3);
            m58C = 16;
            QuarkPlayMotion_unk(&mMotion, 0x38, 0, 0, 0.25f);
            m14BC = 1;
            if (fn_80370A78(lbl_8087F430, 0xC9) == 0) {
                fn_80370AE4(lbl_8087F430, 0xC9, 1);
            }
            fn_80370320(lbl_8087F430, 0x389, 1, 0);
            m14A8 &= ~0x04000000;
            fn_800C31F4("SE_EM211_028", 0, -1, 1.0f);
        } else if (msg->mData->m04 == 0x1791) {
            fn_8034F384();
            fn_8016E970(3);
            m58C = 15;
            QuarkPlayMotion_unk(&mMotion, 0x3D, 0, 0, 1.0f);
            ReleaseCamera_unk();
            if (m17E4 != 0) {
                m17E4->fn_8015ECC4();
                m17E4 = 0;
            }
            m574.y = 0.0f;
        }
        break;
    case 1:
        if (mStatus.m04 < 1.0f) {
            mStatus.m04 = 1.0f;
            mStatus.fn_8013322C(0x20);
        }
        break;
    }
}

// inferred: next entry of an attack table, wrapping around; the original reads the table of the mode
// again for every access (names invented)
// inferred: table `range` of the current mode (the original indexes a [3][3] array: mode * 0x24)
inline IntVector_unk& EnemyQuarkStand::Table_unk(int range) {
    return ((IntVector_unk(*)[3])m1560)[m15CC][range];
}
inline int EnemyQuarkStand::NextShort_unk() {
    int i = Table_unk(0).size() - 1;
    if (m15D0 < i) {
        i = m15D0;
    }
    m15D0 = i;
    int attack = Table_unk(0)[i];
    m15D0 = (unsigned int)(i + 1) % Table_unk(0).size();
    return attack;
}
inline int EnemyQuarkStand::NextMiddle_unk() {
    int i = Table_unk(1).size() - 1;
    if (m15D4 < i) {
        i = m15D4;
    }
    m15D4 = i;
    int attack = Table_unk(1)[i];
    m15D4 = (unsigned int)(i + 1) % Table_unk(1).size();
    return attack;
}
inline int EnemyQuarkStand::NextLong_unk() {
    int i = Table_unk(2).size() - 1;
    if (m15D8 < i) {
        i = m15D8;
    }
    m15D8 = i;
    int attack = Table_unk(2)[i];
    m15D8 = (unsigned int)(i + 1) % Table_unk(2).size();
    return attack;
}

// inferred: picks the next attack from the tables of the current mode by the distance to the target
void EnemyQuarkStand::fn_80346D8C() {
    CharaTask* target = (CharaTask*)m14B0;
    if (target == 0) {
        return;
    }
    atn::Vector3 t = target->mPos;
    atn::Vector3 d = mPos - t;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    int attack = 0;
    if (m17E4 != 0) {
        attack = 12;
    } else if (m14C0 >= m1CD0 && rand() % 8 == 0) {
        atn::Vector3 c(mPos.x - m160C.x, mPos.y - m160C.y, mPos.z - m160C.z);
        if ((float)sqrt(c.x * c.x + c.z * c.z) >= 380.0f) {
            attack = rand() % 2 == 0 ? 4 : 6;
        } else if (dist <= m1CE0) {
            attack = NextShort_unk();
        } else if (dist <= m1CE4) {
            attack = NextMiddle_unk();
        } else {
            attack = NextLong_unk();
        }
    }
    fn_8034701C(attack);
}

// inferred: starts attack `kind` (table values of the data file)
void EnemyQuarkStand::fn_8034701C(int kind) {
    switch (kind) {
    case 1:
        fn_80350070();
        break;
    case 2:
        fn_80350464();
        break;
    case 3:
        fn_8035083C();
        break;
    case 10:
        fn_8034F788();
        break;
    case 5:
        fn_8034F53C();
        break;
    case 6:
        fn_8034FB50();
        break;
    case 12:
        fn_80351480(1);
        break;
    case 4:
        fn_80350F64();
        break;
    case 7:
        if (m180A == 0) {
            fn_80351480(0);
        }
        break;
    case 8:
        if (m180A == 0) {
            fn_80351480(1);
        }
        break;
    case 9:
        fn_803519C4();
        break;
    }
    m180A = m58C == 19;
}

// inferred: front direction for a rotation around y (name invented)
inline atn::Vector3 QuarkFront_unk(float rotY) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rotY);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}
// inferred: front direction of the character (reads the rotation inside; name invented)
inline atn::Vector3 QuarkFrontOf_unk(CharaTask* c) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', c->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}

// inferred: turns toward the target by 12 degrees per frame scaled by `rate` (name invented)
inline void EnemyQuarkStand::TurnTo_unk(float rate) {
    CharaTask* target = (CharaTask*)m14B0;
    atn::Vector3 rot = (target->mPos - mPos).ToRotationZ_unk();
    float y = rot.y;
    float diff = atn::WrapAngle_unk(y - mRot.y);
    float step = 12.0f * rate;
    float abs = diff < 0.0f ? -diff : diff;
    step = 0.017453292f * step;
    if (abs <= step) {
        fn_80097CCC(&mMotion, 1, 0.0f);
        mRot.y = y;
    } else if (diff <= 0.0f) {
        mRot.y -= step;
    } else {
        mRot.y += step;
    }
}

// inferred: walks toward the target, turning 12 degrees per frame, until it is in front within the
// short range
void EnemyQuarkStand::fn_80348568() {
    CharaTask* target = (CharaTask*)m14B0;
    atn::Vector3 d(target->mPos.x - mPos.x, target->mPos.y, target->mPos.z - mPos.z);
    d.y = 0.0f;
    float len = PSVECMag(&d);
    int walk = 1;
    if (len < m1CE0 && len > 0.0f) {
        PSVECNormalize(&d, &d);
        atn::Vector3 front = QuarkFrontOf_unk(this);
        if (PSVECDotProduct(&d, &front) > 0.25881904f) {
            walk = 0;
        }
    }
    if (walk != 0 || mMotion.mTracks[0].m00 == 20) {
        TurnTo_unk(0.1f);
        QuarkPlayMotion_unk(&mMotion, 0x14, 1, 0, 1.0f);
        mPos += QuarkFrontOf_unk(this) * 0.3f;
    } else {
        m14C0++;
    }
}

// NONMATCHING: 99.50% - stack slots of the front direction and its temporaries (the original builds the
// direction before the item); the rest only differs in the pool offsets
// inferred: damage spheres: one in front of the capsule and three on the nodes "head", "spine" and
// "spine1"
void EnemyQuarkStand::vf50() {
    Item14_unk item;
    item.m04.mCenter = m614 + QuarkFrontOf_unk(this) * 10.0f;
    item.m04.mRadius = m620;
    item.m04.mCenter.y -= 10.0f;
    m624.push_back(item);
    item.m04.mRadius = 10.0f;
    item.m04.mCenter = GetTransOf_unk(mMotion.GetNodeMtx_unk("head"));
    m624.push_back(item);
    item.m04.mRadius = 18.0f;
    item.m04.mCenter = GetTransOf_unk(mMotion.GetNodeMtx_unk("spine"));
    m624.push_back(item);
    item.m04.mRadius = 18.0f;
    item.m04.mCenter = GetTransOf_unk(mMotion.GetNodeMtx_unk("spine1"));
    m624.push_back(item);
    mFlagB29 = 1;
}

// inferred: as QuarkFrontOf_unk, copying the rotation first (name invented)
inline atn::Vector3 QuarkFrontY_unk(CharaTask* c) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    float rotY = c->mRot.y;
    PSMTXRotRad(m, 'y', rotY);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}
// inferred: capsule of the body collision (names invented)
inline atn::ColliCapsuleShape_unk QuarkGetCapsule_unk(atn::ColliCapsule& c) {
    atn::ColliCapsuleShape_unk cap;
    cap.mStart = c.m3C;
    cap.mEnd = c.m48;
    cap.mRadius = c.m54;
    return cap;
}
inline void QuarkSetCapsule_unk(atn::ColliCapsule& c, const atn::ColliCapsuleShape_unk& cap) {
    c.m3C = cap.mStart;
    c.m48 = cap.mEnd;
    c.m54 = cap.mRadius;
}

// inferred: moves the collision capsule start in front of the body
void EnemyQuarkStand::fn_803470F0() {
    atn::ColliCapsuleShape_unk cap = QuarkGetCapsule_unk(mColli);
    cap.mStart = m614 + QuarkFrontY_unk(this) * 10.0f;
    cap.mStart.y -= 10.0f;
    QuarkSetCapsule_unk(mColli, cap);
}

// inferred: updates the damage spheres of vf50
// NONMATCHING: 95.35% - registros de la copia de los nodos "spine"/"spine1" (r3/r5 del original frente a
// r5/r6, familia __VecCopy); ~10 variantes (indices, temporales, inline del frente)
void EnemyQuarkStand::vf54() {
    if (m624.mSize == 0) {
        return;
    }
    unsigned int i = 0;
    m624[i].m04.mRadius = m620;
    m624[i].m04.mCenter = m614 + QuarkFrontY_unk(this) * 10.0f;
    m624[i++].m04.mCenter.y -= 10.0f;
    m624[i].m04.mRadius = 10.0f;
    m624[i++].m04.mCenter = GetTransOf_unk(mMotion.GetNodeMtx_unk("head"));
    m624[i].m04.mRadius = 18.0f;
    m624[i++].m04.mCenter = GetTransOf_unk(mMotion.GetNodeMtx_unk("spine"));
    m624[i].m04.mRadius = 18.0f;
    m624[i].m04.mCenter = GetTransOf_unk(mMotion.GetNodeMtx_unk("spine1"));
}

atn::Vector3 EnemyQuarkStand::vf64(int center) {
    return m624.mData->m04.mCenter;
}

// inferred: range of the laser (lbl_8087DC80/84) and lerp ends passed by address to fn_800F8524
// (lbl_8087DC88/8C); names invented
static float sLaserNear_unk = 100.0f;
static float sLaserFar_unk = 500.0f;
static float sBurstNear_unk = 100.0f;
static float sBurstFar_unk = 500.0f;
// inferred: height of the sonic wave while it charges (lbl_8087DC90/94) and while it ends (lbl_8087DC98/9C)
static float sSonicFrom_unk = 90.0f;
static float sSonicTo_unk = 0.0f;
static float sSonicEndFrom_unk = 90.0f;
static float sSonicEndTo_unk = 0.0f;

#define QS_MAX_unk(a, b) ((a) > (b) ? (a) : (b))
#define QS_MIN_unk(a, b) ((a) < (b) ? (a) : (b))

// inferred: rotation matrix z * y * x from euler angles (as __MakeRotationZYX_unk of
// game/MapPartsModel.cpp; name invented)
inline atn::Matrix3x4& QuarkRotationZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
inline void QuarkRotateZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    Mtx t;
    atn::Matrix3x4 rot;
    PSMTXConcat(m.m, QuarkRotationZYX_unk(rot, r).m, t);
    __MtxCopy(m.m, t);
}
inline atn::Matrix3x4 QuarkScaleMtx_unk(const atn::Vector3& s) {
    atn::Matrix3x4 r;
    PSMTXScale(r.m, s.x, s.y, s.z);
    return r;
}
inline atn::Matrix3x4 QuarkMtxMul_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

inline void QuarkScale_unk(atn::Matrix3x4& m, const atn::Vector3& s) {
    m = QuarkMtxMul_unk(m, QuarkScaleMtx_unk(s));
}
inline void QuarkScaleF_unk(atn::Matrix3x4& m, float s) {
    QuarkScale_unk(m, atn::Vector3(s, s, s));
}
// inferred: m1660 seen as a matrix (name invented)
inline atn::Matrix3x4& QuarkLaserMtx_unk(EnemyQuarkStand* e) {
    return *(atn::Matrix3x4*)&e->m1660;
}

// inferred: matrix of the laser beam: from `target` toward the point in front of the body, scaled
// by its length
void EnemyQuarkStand::fn_803489F4(const atn::Vector3& target) {
    float t = QS_MAX_unk(0.0f, QS_MIN_unk(1.0f, (mMotion.mTracks[0].m08 - 60.0f) / 20.0f));
    float dist = (sLaserFar_unk - sLaserNear_unk) * t + sLaserNear_unk;
    atn::Vector3 pos = mPos + QuarkFrontY_unk(this) * dist;
    m1654 = pos - target;
    float scale = QS_MAX_unk(1.0f, m1654.Length_unk() / 250.0f);
    m1654.Normalize_unk();
    QuarkLaserMtx_unk(this).SetIdentity();
    QuarkRotateZYX_unk(QuarkLaserMtx_unk(this), m1654.ToRotation_unk());
    QuarkScaleF_unk(QuarkLaserMtx_unk(this), scale);
    QuarkLaserMtx_unk(this).m[0][3] = target.x;
    QuarkLaserMtx_unk(this).m[1][3] = target.y;
    QuarkLaserMtx_unk(this).m[2][3] = target.z;
}

// inferred: laser attack: aims, starts the beam effect at frame 56, fires the laser bullet between frames
// 58.9 and 80.1 and queues the explosions along the beam
void EnemyQuarkStand::fn_80348FF4() {
    if (m14B4 == 0) {
        fn_80352938(0.7f);
        if (mMotion.fn_80139550(0) >= 56.0f) {
            fn_80239DAC(fn_800F7FA0(), this, 0x3E8, 1);
            fn_800F7FA8(fn_800F7FA0(), this, 0x3E8);
            fn_80276AE4(fn_800F7FA0(), &m163C, &mMotion, &lbl_807C7030, &lbl_807C7030, -1, -1, 1, 1.0f);
            m14B4++;
        }
    } else if (m14B4 == 1) {
        if (mMotion.fn_80139550(0) >= 58.9f) {
            atn::Vector3 pos(0.0f, 0.0f, 20.0f);
            pos.fn_80011410(*(atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"));
            fn_803489F4(pos);
            fn_800F7FA8(fn_800F7FA0(), this, 0x3E8);
            fn_8023A680(fn_800F7FA0(), &m1648, -1, 5, &m1660, 0, 0, 0, 0, -1, 1, 1.0f);
            fn_800C31F4("SE_EM211_009_2", 0, -1, 1.0f);
            m1A24 = 0;
            m1A20 = fn_800F8548(fn_8013A194());
            m14B4++;
        }
    } else if (m14B4 == 2) {
        if (mMotion.fn_80139550(0) <= 80.1f) {
            atn::Vector3 pos(0.0f, 0.0f, 20.0f);
            pos.fn_80011410(*(atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"));
            fn_803489F4(pos);
            Bullet* bullet = fn_801010A0(fn_8013A194());
            if (bullet != 0) {
                QuarkBulletShot_unk shot;
                shot.m00 = fn_8010B250(fn_8013A194(), m152C->m3C, 0);
                shot.m04 = fn_800EFD04(m152C->mB0);
                shot.m08 = fn_800EFDA0(m152C->mB0);
                shot.m0C = m152C;
                shot.m10 = 0;
                shot.m14 = 0.0f;
                fn_8010EE78(bullet, &shot, &pos, &m1654, this, m590, 0, -1.0f);
            }
            float t = QS_MAX_unk(0.0f, QS_MIN_unk(1.0f, (mMotion.fn_80139550(0) - 60.0f) / 20.0f));
            float dist = fn_800F8524(&sBurstNear_unk, &sBurstFar_unk, t);
            atn::Vector3 at = mPos + fn_8014052C() * dist;
            if (!m181C.full()) {
                QuarkShot_unk item;
                item.m00 = 25.0f;
                item.m04 = at;
                m181C.PushBack_unk(item);
            }
        } else {
            fn_80239DAC(fn_800F7FA0(), this, 0x3E8, 1);
            m14B4++;
        }
    } else {
        float end = mMotion.fn_80097D7C(0);
        if (mMotion.fn_80139550(0) >= end) {
            fn_8034F41C();
        }
    }
}

// inferred: sonic wave attack (motions 0x16B-0x16D): rises while charging for SonicSkillID->mC0 frames,
// then shakes the camera and fires the wave 65 units in front
// NONMATCHING: 97.78% - el original tiene 32 bytes mas de pila (stores muertos de 0, 90 y 0 a 0x18/0x28/0x98
// junto al vector de 65) y numera distinto los FPR del lerp; ~6 variantes (inline del ratio, orden del
// producto, helpers de rotacion)
void EnemyQuarkStand::fn_803494AC() {
    if (m14B4 == 0) {
        fn_8034C060(1);
        mPos.y = QS_MIN_unk(90.0f, 5.0f + mPos.y);
        if (mMotion.mTracks[0].m00 == 0x141) {
            m14B4++;
            QuarkPlayMotion_unk(&mMotion, 0x16B, 0, 0, 1.0f);
            fn_80232B7C(this, 0x3EA);
            QuarkPlayEffect_unk(&m16C0, &mMotion);
            m14B8 = 0;
        }
    } else if (m14B4 == 1) {
        if (m14B8 >= m153C->mC0) {
            m14B4++;
            mMotion.fn_80097C08(0, 0x16D, 0, 0, 1, 0.0f, 0.1f);
            fn_80239DAC(lbl_8087F3C0, this, 0x3EA, 0);
            fn_80232B7C(this, 0x3E9);
            QuarkPlayEffect_unk(&m16CC, &mMotion);
        }
        mPos.y = QS_MIN_unk(90.0f, 5.0f + mPos.y);
        if (mMotion.mTracks[0].m00 == 0x16B) {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mMotion.fn_80097C08(0, 0x16C, 1, 0, 1, 1.0f, 0.1f);
            }
        }
    } else if (m14B4 == 2) {
        float t = QS_MIN_unk(1.0f, QS_MAX_unk(0.0f, (mMotion.mTracks[0].m08 - 45.0f) / 4.0f));
        mPos.y = (sSonicTo_unk - sSonicFrom_unk) * t + sSonicFrom_unk;
        Field_unk* field = lbl_8087F430;
        if (mMotion.mTracks[0].m08 >= 49.0f) {
            m14B4++;
            field->m96C %= 2;
            field->m970 = 10;
            field->m974 = 7.0f;
            field->m978 = 8.0f;
            atn::Vector3 off(0.0f, 0.0f, 65.0f);
            atn::Matrix3x4 m;
            QuarkRotationZYX_unk(m, mRot);
            PSMTXMultVec(m.m, &off, &off);
            m16D8 = 1;
            m16EC = fn_800F8548(lbl_8087F048);
            m16E0 = mPos + off;
            m16E0.y = 0.0f;
            mStatus.m228 = 0.0f;
            fn_800C31F4("SE_EM211_010", 0, -1, 1.0f);
        }
    } else {
        float t = QS_MIN_unk(1.0f, QS_MAX_unk(0.0f, (mMotion.mTracks[0].m08 - 45.0f) / 4.0f));
        mPos.y = (sSonicEndTo_unk - sSonicEndFrom_unk) * t + sSonicEndFrom_unk;
        if (mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0)) {
            fn_8034F41C();
            mPos.y = 0.0f;
        }
    }
}

// inferred: lets the camera go back to the field (name invented)
inline void EnemyQuarkStand::ReleaseCamera_unk() {
    if (m180B != 0) {
        Field_unk* field = lbl_8087F430;
        field->m8A0 = 0;
        field->m4D8 = 0;
        field->m97C = 0;
        m180B = 0;
    }
}

// inferred: frame of track 0 at least `frame` (name invented)
inline bool QuarkFrameAtLeast_unk(Model_unk* model, float frame) {
    bool ok = false;
    if (model->mTracks[0].m08 >= frame) {
        ok = true;
    }
    return ok;
}

// inferred: the motion of track 0 is within `frames` of its end (name invented)
inline bool QuarkNearEnd_unk(Model_unk* model, float frames) {
    float end = model->fn_80097D7C(0);
    bool ok = false;
    if (model->mTracks[0].m08 >= end - frames) {
        ok = true;
    }
    return ok;
}

// inferred: horizontal and full distance between two points (names invented)
inline float QuarkDistXZ_unk(const atn::Vector3& to, const atn::Vector3& from) {
    atn::Vector3 d(to.x - from.x, to.y - from.y, to.z - from.z);
    return sqrt(d.x * d.x + d.z * d.z);
}
inline float QuarkDist_unk(const atn::Vector3& to, const atn::Vector3& from) {
    atn::Vector3 d = to - from;
    return PSVECMag(&d);
}

// inferred: flight toward the aim m15DC (motions 0x13F take off, 0x140 flight, 0x141 landing): the speed
// m570 grows by 0.5 per frame up to the distance (3D when `full`), then follows the aim and turns toward the
// target; the action ends when the landing motion ends
void EnemyQuarkStand::fn_8034C060(int full) {
    float len = full != 0 ? QuarkDistXZ_unk(m15DC.mTarget, mPos) : QuarkDist_unk(m15DC.mTarget, mPos);
    float speed = QS_MIN_unk(10.0f, len);
    int motion = mMotion.mTracks[0].m00;
    if (motion == 0x13F) {
        mMotion.mTracks[0].m0C = 2.0f;
        if (QuarkNearEnd_unk(&mMotion, 5.0f)) {
            mMotion.fn_80097C08(0, 0x140, 0, 0, 1, 0.0f, 0.1f);
        }
        if (QuarkFrameAtLeast_unk(&mMotion, 15.0f)) {
            m570 = QS_MIN_unk(speed, 0.5f + m570);
        }
    } else if (motion == 0x140) {
        mMotion.mTracks[0].m0C = 1.0f;
        if (len < 20.0f) {
            mMotion.fn_80097C08(0, 0x141, 0, 0, 1, 0.0f, 0.1f);
        }
        m570 = QS_MIN_unk(speed, 0.5f + m570);
    } else if (motion == 0x141) {
        mMotion.mTracks[0].m0C = 2.0f;
        atn::Vector3 d = m15DC.mTarget - mPos;
        d.y = 0.0f;
        m570 = QS_MIN_unk(speed, 0.2f * len);
        PSVECNormalize(&d, &d);
        if (PSVECDotProduct(&d, &m15DC.mDir) < 0.0f) {
            m570 = 0.0f;
        }
        if (QuarkNearEnd_unk(&mMotion, 8.0f)) {
            fn_8034F41C();
            return;
        }
    }
    if (m570 > 0.5f) {
        TurnTo_unk(1.0f);
    }
    mPos += m15DC.mDir * m570;
}

// inferred: state 17 (falls with gravity until the ground, then waits) (the state helpers are inlined)
void EnemyQuarkStand::fn_8034E898() {
    m574.y += -98.1f / (float)(lbl_8087F0A8->mFrameRate * lbl_8087F0A8->mFrameRate);
    mPos.y += m574.y;
    if (mPos.y < 0.0f) {
        mPos.y = 0.0f;
        m574.y = 0.0f;
    }
    float frame;
    if (m14B8 >= 150 || (frame = mMotion.mTracks[0].m08, frame >= mMotion.fn_80097D7C(0))) {
        fn_8034F41C();
    }
}

// inferred: state 15 (stunned): falls, then waits "DownCount" frames or "StundUpDamageCount" hits
void EnemyQuarkStand::fn_8034EA50() {
    m574.y += -98.1f / (float)(lbl_8087F0A8->mFrameRate * lbl_8087F0A8->mFrameRate);
    mPos.y += m574.y;
    if (mPos.y < 0.0f) {
        mPos.y = 0.0f;
        m574.y = 0.0f;
    }
    if (m14B4 == 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            QuarkPlayMotion_unk(&mMotion, 0x3E, 1, 0, 1.0f);
            m14B4++;
            m14B8 = 0;
            m1804 = 0;
        }
    } else if (m14B4 == 1) {
        if (m14B8 >= m17FC || m1804 > m1800) {
            QuarkPlayMotion_unk(&mMotion, 0x3F, 0, 0, 1.0f);
            m14B4++;
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            fn_8034F41C();
        }
    }
}

// inferred: leaves the current action: clears the counters, takes a new bullet id and stops the effects
// of the attacks (names invented)
void EnemyQuarkStand::fn_8034F384() {
    m14B4 = 0;
    m14B8 = 0;
    m14C0 = 0;
    m17E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80239DAC(lbl_8087F3C0, this, 0x3EA, 1);
    fn_80239DAC(lbl_8087F3C0, this, 0x3E8, 1);
    fn_80239DAC(lbl_8087F3C0, this, 0x3ED, 1);
    fn_80239DAC(lbl_8087F3C0, this, 0x3EF, 1);
}

// inferred: state 0 (wait): waits 30 +- 30 frames
void EnemyQuarkStand::fn_8034F41C() {
    fn_8034F384();
    m14C0 += 30 - rand() % 60;
    fn_8016E970(3);
    m58C = 0;
    QuarkPlayMotion_unk(&mMotion, 2, 1, 0, 1.0f);
}

// inferred: plays an effect on the model at the origin (the zero vector of lbl_807C7030; name invented)
inline void QuarkPlayEffectAt_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
    QColor_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    EffectMgr_unk* mgr = lbl_8087F3C0;
    fn_8023A8B4(mgr, efp, model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

// inferred: starts attack 5 (state 6, motion 0x147): laser from the "spine" of the target toward the
// "head" of Quark
void EnemyQuarkStand::fn_8034F53C() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 6;
    fn_80232B7C(this, 0x3E8);
    QuarkPlayMotion_unk(&mMotion, 0x147, 0, 0, 1.0f);
    QuarkPlayEffectAt_unk(&m1630, &mMotion);
    atn::Vector3 spine = GetTransOf_unk(((CharaTask*)m14B0)->mMotion.GetNodeMtx_unk("spine"));
    atn::Vector3 head = GetTransOf_unk(mMotion.GetNodeMtx_unk("head"));
    m1654 = spine - head;
    PSVECNormalize(&m1654, &m1654);
    fn_800C31F4("SE_EM211_009_1", 0, -1, 1.0f);
}

// inferred: aims the attack at the target (names invented)
inline void EnemyQuarkStand::AimAt_unk() {
    CharaTask* target = (CharaTask*)m14B0;
    atn::Vector3 pos = target->mPos;
    m15DC.SetTarget_unk(pos);
    m15DC.mRot = (pos - mPos).ToRotationZ_unk();
    m15DC.mDir = pos - mPos;
    m15DC.mDir.y = 0.0f;
    PSVECNormalize(&m15DC.mDir, &m15DC.mDir);
    m15DC.mStart = mPos;
    m574.x = 0.0f;
}

// inferred: starts attack 10 (state 7, motion 0x13F): flies to the centre of the arena; frees the character
// stuck by the sword
// NONMATCHING: 98.28% - same stack slots as AimAt_unk (fn_80350070): the copy of the target and the
// temporaries of ToRotationZ_unk sit in other slots; ~3 forms
void EnemyQuarkStand::fn_8034F788() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 7;
    m16DC = 0.0f;
    m16D8 = 0;
    if (m17E4 != 0) {
        m17E4->fn_8015ECC4();
        m17E4 = 0;
    }
    QuarkPlayMotion_unk(&mMotion, 0x13F, 0, 0, 1.0f);
    atn::Vector3 pos = m160C;
    m15DC.SetTarget_unk(pos);
    m15DC.mRot = (pos - mPos).ToRotationZ_unk();
    m15DC.mDir = pos - mPos;
    m15DC.mDir.y = 0.0f;
    PSVECNormalize(&m15DC.mDir, &m15DC.mDir);
    m15DC.mStart = mPos;
    m570 = 0.0f;
}

// inferred: starts attack 6 (state 8, motion 0x14D): flies to the route place 3 or 4, the one farther from
// the leader
// NONMATCHING: 96.11% - the original keeps the two places in r5/r4 and the copies higher in the frame
// (0x74/0x80, ours 0xBC/0xB0) and the usual AimAt_unk slots; ~6 forms (inline, FindPlace_unk, ternary)
void EnemyQuarkStand::fn_8034FB50() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 8;
    m590 = fn_800F8548(lbl_8087F048);
    QuarkPlayMotion_unk(&mMotion, 0x14D, 0, 0, 0.7f);
    RouteTable_unk* table = lbl_8087F430->m10D8;
    RoutePlace_unk* place3 = table->FindPlace_unk(3);
    RoutePlace_unk* place4 = table->FindPlace_unk(4);
    atn::Vector3 a = place3->mPos;
    atn::Vector3 b = place4->mPos;
    atn::Vector3 leader = lbl_8087F8A0->mHead->mPos;
    atn::Vector3 da(a.x - leader.x, a.y - leader.y, a.z - leader.z);
    float la = sqrt(da.x * da.x + da.z * da.z);
    atn::Vector3 db(b.x - leader.x, b.y - leader.y, b.z - leader.z);
    atn::Vector3* far = la < (float)sqrt(db.x * db.x + db.z * db.z) ? &b : &a;
    atn::Vector3 pos = *far;
    m15DC.SetTarget_unk(pos);
    m15DC.mRot = (pos - mPos).ToRotationZ_unk();
    m15DC.mDir = pos - mPos;
    m15DC.mDir.y = 0.0f;
    PSVECNormalize(&m15DC.mDir, &m15DC.mDir);
    m15DC.mStart = mPos;
    m570 = 0.0f;
}

// inferred: starts attack 1 (state 9, motion 0x15C)
// NONMATCHING: 98.62% - stack slots of the target copy and of the temporaries of AimAt_unk; ~8 forms
void EnemyQuarkStand::fn_80350070() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 9;
    m590 = fn_800F8548(lbl_8087F048);
    QuarkPlayMotion_unk(&mMotion, 0x15C, 0, 0, 1.0f);
    AimAt_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, this, 0, 0, 1.0f, 10.0f);
    }
}

// inferred: starts attack 2 (state 10, motion 0x156)
// NONMATCHING: same stack slots as fn_80350070
void EnemyQuarkStand::fn_80350464() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 10;
    m590 = fn_800F8548(lbl_8087F048);
    QuarkPlayMotion_unk(&mMotion, 0x156, 0, 0, 1.0f);
    AimAt_unk();
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, this, 0, 0, 1.0f, 10.0f);
    }
}

// inferred: starts attack 3 (state 11, motion 0x142)
// NONMATCHING: same stack slots as fn_80350070
void EnemyQuarkStand::fn_8035083C() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 11;
    m590 = fn_800F8548(lbl_8087F048);
    QuarkPlayMotion_unk(&mMotion, 0x142, 0, 0, 1.0f);
    AimAt_unk();
}

// inferred: state 12 (motion 0x14A): Quark grabs the character stuck by the sword; the side of the jump
// (m17F0) is random, reversed when the wall blocks it
// NONMATCHING: 99.66% - stack slots of the cross product and of the sum in the first branch (the
// original puts them above the other branch, 0x8C/0xA4); ~3 forms (inline, shared temporaries)
void EnemyQuarkStand::fn_80350BF0() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 12;
    m590 = fn_800F8548(lbl_8087F048);
    QuarkPlayMotion_unk(&mMotion, 0x14A, 0, 0, 1.0f);
    mMotion.mTracks[0].m08 = 5.0f;
    m14B0 = (int)m17E4;
    mC58.mC4 = m17E4;
    m17EC = 0;
    m17F0 = rand() % 2;
    atn::Vector3 from = m17E4->mPos;
    from.y += 5.0f;
    atn::Vector3 to = from;
    if (m17F0 == 0) {
        atn::Vector3 dir = m15DC.mDir;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 side;
        PSVECCrossProduct(&dir, &up, &side);
        to = from + side * 20.0f;
    } else {
        atn::Vector3 dir = m15DC.mDir;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 side;
        PSVECCrossProduct(&dir, &up, &side);
        to = from - side * 20.0f;
    }
    if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0) != 0) {
        m17F0 = m17F0 == 0;
    }
    fn_80232B7C(&m16A8, 0);
    QuarkPlayEffect_unk(&m16A8, &mMotion);
    fn_8023A108(lbl_8087F3C0, &m16A8, 0, 5);
}

// inferred: starts attack 4 (state 18, motion 0x13F): flies to the route place 3 or 4, the one farther from
// the leader (as fn_8034FB50)
// NONMATCHING: same stack slots as fn_8034FB50
void EnemyQuarkStand::fn_80350F64() {
    fn_8034F384();
    fn_8016E970(3);
    fn_8016E970(3);
    m58C = 18;
    QuarkPlayMotion_unk(&mMotion, 0x13F, 0, 0, 1.0f);
    RouteTable_unk* table = lbl_8087F430->m10D8;
    RoutePlace_unk* place3 = table->FindPlace_unk(3);
    RoutePlace_unk* place4 = table->FindPlace_unk(4);
    atn::Vector3 a = place3->mPos;
    atn::Vector3 b = place4->mPos;
    atn::Vector3 leader = lbl_8087F8A0->mHead->mPos;
    atn::Vector3 da(a.x - leader.x, a.y - leader.y, a.z - leader.z);
    float la = sqrt(da.x * da.x + da.z * da.z);
    atn::Vector3 db(b.x - leader.x, b.y - leader.y, b.z - leader.z);
    atn::Vector3* far = la < (float)sqrt(db.x * db.x + db.z * db.z) ? &b : &a;
    atn::Vector3 pos = *far;
    m15DC.SetTarget_unk(pos);
    m15DC.mRot = (pos - mPos).ToRotationZ_unk();
    m15DC.mDir = pos - mPos;
    m15DC.mDir.y = 0.0f;
    PSVECNormalize(&m15DC.mDir, &m15DC.mDir);
    m15DC.mStart = mPos;
    m570 = 0.0f;
}

// inferred: starts attack 12 / 7 (state 19, motion 0x143): warps in front of the target (at "ShortRange" - 10
// when `near`, else "LongRange" - 10) and plays "ef_bs080a.efp" at the speed of the motion
// NONMATCHING: 88.77% - order of the products of dir * dist and the stack slots of the aim (as
// fn_80350070); ~5 forms
void EnemyQuarkStand::fn_80351480(int near) {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 19;
    QuarkPlayMotion_unk(&mMotion, 0x143, 0, 0, 1.5f);
    if (m17E4 != 0) {
        m14B0 = (int)m17E4;
    }
    float dist;
    if (near != 0) {
        dist = m1CE0 - 10.0f;
    } else {
        dist = m1CE8 - 10.0f;
    }
    CharaTask* target = (CharaTask*)m14B0;
    atn::Vector3 dir = m160C - target->mPos;
    dir.y = 0.0f;
    if (fn_805F9920(&dir) < 0.0001f) {
        dir = QuarkFrontOf_unk((CharaTask*)m14B0);
    } else {
        PSVECNormalize(&dir, &dir);
    }
    atn::Vector3 pos = ((CharaTask*)m14B0)->mPos + dir * dist;
    m15DC.SetTarget_unk(pos);
    m15DC.mRot = (pos - mPos).ToRotationZ_unk();
    m15DC.mDir = pos - mPos;
    m15DC.mDir.y = 0.0f;
    PSVECNormalize(&m15DC.mDir, &m15DC.mDir);
    m15DC.mStart = mPos;
    m570 = 0.0f;
    fn_80232B7C(&m16B4, 0);
    QuarkPlayEffect_unk(&m16B4, &mMotion);
    ((atn::EffectManager*)lbl_8087F3C0)->fn_8023A254((unsigned int)&m16B4, 0, mMotion.mTracks[0].m0C);
}

// inferred: state 20 (motion 0x144)
void EnemyQuarkStand::fn_803519C4() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 20;
    QuarkPlayMotion_unk(&mMotion, 0x144, 0, 0, 1.0f);
    m1CB8 = 0;
    m1CBC = 0;
}

// inferred: state 16 (down): the sword can be grabbed (m14BC = 1), sets the field flags 0xC9 and 0x389
void EnemyQuarkStand::fn_803520D8() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 16;
    QuarkPlayMotion_unk(&mMotion, 0x38, 0, 0, 0.25f);
    m14BC = 1;
    if (fn_80370A78(lbl_8087F430, 0xC9) == 0) {
        fn_80370AE4(lbl_8087F430, 0xC9, 1);
    }
    fn_80370320(lbl_8087F430, 0x389, 1, 0);
    m14A8 &= ~0x4000000;
    fn_800C31F4("SE_DG040_109", 0, -1, 1.0f);
}

// inferred: plays motion `motion` at `speed` (out of line in vf10)
void EnemyQuarkStand::fn_803524AC(int motion, int arg3, float speed) {
    QuarkPlayMotion_unk(&mMotion, motion, arg3, 0, speed);
}

// inferred: sonic wave ring: 180 bullets of SonicSkillID on a circle of radius m16DC around m16E0 (with the
// debug spheres when "DebugDisp"); the ring grows by "ShockExpandSnicWave" until m1CCC
void EnemyQuarkStand::fn_8035223C() {
    atn::Vector3 off(0.0f, 0.0f, m16DC);
    float rot = mRot.y;
    for (int i = 0; i < 180; i++) {
        float step = 0.017453292f * (2.0f * (float)i);
        rot += step;
        rot = atn::WrapAngle_unk(rot);
        Mtx m;
        PSMTXRotRad(m, 'y', rot);
        PSMTXMultVec(m, &off, &off);
        fn_800FAB80(lbl_8087F048, this, m153C, m16EC, &(m16E0 + off), &atn::Vector3(0.0f, rot, 0.0f), 0, 30, -1,
                    -1, 3.0f, 1.0f);
        if (m1CC4 != 0) {
            fn_80063D3C(lbl_8087EEB0, &(m16E0 + off), 0xFF000000, m153C->m58, 1000.0f);
        }
    }
    if (m16DC >= m1CCC) {
        fn_80239DAC(lbl_8087F3C0, this, 0x3E9, 0);
        m16D8 = 0;
    }
    m16DC += m1CC8;
}

// inferred: counts down the delayed explosions: each one fires LaserExplodeSkillID at its position, every
// fourth plays "SE_MB003_003" and shakes the camera
void EnemyQuarkStand::fn_803524E4() {
    QuarkShot_unk* it = m181C.begin();
    while (it != m181C.end()) {
        if (it->m00 > 0.0f) {
            float dt = lbl_8087EFA8->m3A4;
            it->m00 -= dt;
            if (it->m00 <= 0.0f) {
                fn_800FAB80(lbl_8087F048, this, m1530, m1A20, &it->m04, &lbl_807C7030, 0, 30, -1, -1, 0.0f, 1.0f);
                m1A24++;
                if (m1A24 % 4 == 0) {
                    fn_800C344C("SE_MB003_003", &it->m04, 0, -1, 1.0f);
                    Field_unk* field = lbl_8087F430;
                    field->m96C %= 2;
                    field->m970 = 3;
                    field->m974 = 0.0f;
                    field->m978 = 1.0f;
                }
            }
            it++;
        } else {
            m181C.erase(it);
        }
    }
}

// inferred: counts down the delayed waves: under 20 the effect of the wave starts; at zero it stops, fires
// ThunderSkill at its position and every third plays "SE_EM203_004" and shakes the camera
void EnemyQuarkStand::fn_803526A8() {
    QuarkWave_unk* it = m1A28.begin();
    while (it != m1A28.end()) {
        if (it->m0C > 0.0f) {
            float dt = lbl_8087EFA8->m3A4;
            it->m0C -= dt;
            if (it->m0C <= 20.0f && it->m10 == 0) {
                it->m10 = 1;
                fn_80232B7C(&m1CAC, it->m11);
                QColor_unk color;
                color.r = 1.0f;
                color.g = 1.0f;
                color.b = 1.0f;
                color.a = 1.0f;
                fn_8023A8B4(lbl_8087F3C0, &m1CAC, 0, 0, it->m00, lbl_807C7030, color, -1, -1, 1, 1.0f);
            }
            if (it->m0C <= 0.0f) {
                fn_80239DAC(lbl_8087F3C0, &m1CAC, it->m11, 1);
                Effect_unk* mgr = lbl_8087F048;
                fn_800FAB80(mgr, this, m155C, fn_800F8548(mgr), &it->m00, &lbl_807C7030, 0, 30, -1, -1, 0.0f,
                            1.0f);
                m1CB8++;
                if (m1CB8 % 3 == 0) {
                    fn_800C344C("SE_EM203_004", &it->m00, 0, -1, 1.0f);
                    Field_unk* field = lbl_8087F430;
                    field->m96C %= 2;
                    field->m970 = 3;
                    field->m974 = 0.0f;
                    field->m978 = 1.0f;
                }
            }
            it++;
        } else {
            m1A28.erase(it);
        }
    }
}

// inferred: a gimmick that can be pushed by the shock (value at 0x920 cleared in state 3; as gmk::PassBoard)
struct QuarkGimmick_unk : public GimmickCommon {
    unsigned char m0F4[0x920 - 0xF4];
    float m920;
};

// inferred: shock of the body against the gimmicks 48001 and 48004 around the "spine" node: a gimmick
// within `range` and inside the cone `angle` in front gets a hit message of kind `kind` (when `hit`); one
// within 2 * range in state 3 clears its value at 0x920
void EnemyQuarkStand::fn_80352C3C(int kind, int hit, float range, float angle) {
    for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
        int id = g->mId;
        if (id == 48004 || id == 48001) {
            atn::Vector3 pos = g->vf54();
            atn::Vector3 d = pos - GetTransOf_unk(mMotion.GetNodeMtx_unk("spine"));
            d.y = 0.0f;
            float dist = sqrt(d.x * d.x + d.z * d.z);
            int colli = g->vf50();
            if (colli != 0) {
                dist -= *(float*)(colli + 0x48);
            }
            if (dist <= range) {
                PSVECNormalize(&d, &d);
                atn::Vector3 front = QuarkFrontY_unk(this);
                front.y = 0.0f;
                if ((float)acos(PSVECDotProduct(&d, &front)) <= 0.5f * angle && hit != 0) {
                    GimmickHitMsg_unk msg;
                    memset(&msg, 0, sizeof(msg));
                    msg.mType = kind;
                    *(EnemyQuarkStand**)&msg.m28[0] = this;
                    g->vf44(&msg);
                }
            }
            if (dist <= 2.0f * range && g->m54 == 3) {
                ((QuarkGimmick_unk*)g)->m920 = 0.0f;
            }
        }
    }
}

// inferred: out-of-line turn toward the target (the same body as TurnTo_unk)
// NONMATCHING: 99.11% - slots de pila de la diferencia y de la copia de ToRotationZ_unk intercambiados
// (0x50/0x5C); ~8 variantes (inline TurnTo_unk, temporales, bloques)
void EnemyQuarkStand::fn_80352938(float rate) {
    CharaTask* target = (CharaTask*)m14B0;
    atn::Vector3 d(target->mPos - mPos);
    float y = d.ToRotationZ_unk().y;
    float diff = atn::WrapAngle_unk(y - mRot.y);
    float step = 12.0f * rate;
    float abs = diff < 0.0f ? -diff : diff;
    step = 0.017453292f * step;
    if (abs <= step) {
        fn_80097CCC(&mMotion, 1, 0.0f);
        mRot.y = y;
    } else if (diff <= 0.0f) {
        mRot.y -= step;
    } else {
        mRot.y += step;
    }
}

// inferred: shows the two direction arrows of the d2anime
void EnemyQuarkStand::fn_80352EA4() {
    m17F4->mFlags |= 4;
    m17F8->mFlags |= 4;
}

// inferred: position of the "spine" node while Quark is down (state 16, step 1, after two frames)
int EnemyQuarkStand::vfB0(atn::Vector3* out) {
    if (m14BC == 1 && m58C == 16 && m14B4 >= 2) {
        *out = GetTransOf_unk(mMotion.GetNodeMtx_unk("spine"));
        return 1;
    }
    return 0;
}

// inferred: a party member grabs the sword: the field event 0x6F starts and the character thrusts
// (chr::ControlThrustForQuarkStand); the return is a boost::function of fn_80178208
boost::function<void()> EnemyQuarkStand::vfB4(CharaTask* chara) {
    if (m14BC == 1) {
        fn_80370AE4(lbl_8087F430, 0x6F, 1);
        fn_80351AB8(chara, 2);
        chara->SetHit_unk(m1550);
        if (lbl_8087EFE8 != 0) {
            fn_800CF45C(lbl_8087EFE8, 0x5A, 0);
        }
        if (lbl_8087F498 != 0) {
            fn_803EBAC8(lbl_8087F498, 15, 0);
        }
        fn_800C31F4("SE_EM211_012", 0, -1, 1.0f);
        mC58.mC0 = 0;
        chr::ControlThrustForQuarkStand* next = new (0, "", "", 0) chr::ControlThrustForQuarkStand(chara, this, 2);
        return boost::bind(&CharaTask::fn_80178208, chara, next);
    }
    return boost::function<void()>();
}

// inferred: debug menu "Quark"
void EnemyQuarkStand::fn_80353370() {
    void* group = m1CC0.Load("Quark");
    fn_80087994(group, "DebugDisp", &m1CC4, 0, 0);
    fn_800874C8(group, "AttackWaitTime", &m1CD0, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "GatheringBurstCount", &m1818, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "DownCount", &m17FC, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "StundUpDamageCount", &m1800, 1, 999999, 1, 0, 0);
    fn_800874C8(group, "AttakedInStick", &m1CD4, 1, 999, 1, 0, 0);
    fn_8008771C(group, "ShortRange", &m1CE0, 0, 0, 0.0f, 9999.0f, 0.01f);
    fn_8008771C(group, "MiddleRange", &m1CE4, 0, 0, 0.0f, 9999.0f, 0.01f);
    fn_8008771C(group, "LongRange", &m1CE8, 0, 0, 0.0f, 9999.0f, 0.01f);
    fn_8008771C(group, "SwordDirectionLine", &m1CD8, 0, 0, 0.0f, 1000.0f, 0.01f);
    fn_800874C8(group, "SwordThrowTime", &m1CDC, 0, 999999, 1, 0, 0);
    fn_8008771C(group, "ShockExpandSonicWave", &m1CC8, 0, 0, 0.0f, 100.0f, 0.01f);
}

int EnemyQuarkStand::vf78() {
    float frame;
    if (m14B0 != 0) {
        switch (m58C) {
    case 6:
        frame = mMotion.mTracks[0].m08;
        return frame >= 23.0f && frame <= 24.0f;
    case 7:
        frame = mMotion.mTracks[0].m08;
        return frame >= 43.0f && frame <= 45.0f;
    case 9:
        frame = mMotion.mTracks[0].m08;
        return frame >= 61.0f && frame <= 63.0f;
    case 11:
        frame = mMotion.mTracks[0].m08;
        return frame >= 7.0f && frame <= 9.0f;
    }
    }
    return 0;
}

extern "C" void fn_80353EE4(EnemyQuarkStand* self) {
    self->ReleaseCamera_unk();
}

extern "C" void fn_80353F0C(EnemyQuarkStand* self) {
    if (self->m17E4 != 0) {
        self->m17E4->fn_8015ECC4();
        self->m17E4 = 0;
    }
}

void EnemyQuarkStand::vf98() {
    if ((m54C & 8) == 8) {
        return;
    }
    if (mFlag18) {
        return;
    }
    mMotion.fn_8009373C("op_01_horn_l", 0);
    mMotion.fn_8009373C("op_01_hornlight_l", 0);
    lbl_8087F3C0->mB8 = 2;
    fn_80232B7C(m121C.get(), 0);
    QuarkPlayEffect_unk(m121C.get(), &mMotion);
    QuarkPlayEffect_unk(&m1618, &mMotion);
    lbl_8087F3C0->mB8 = 0;
    mFlag18 = 1;
}

void EnemyQuarkStand::vf9C(int arg) {
    if (mFlag18) {
        fn_80239DAC(lbl_8087F3C0, m121C.get(), 0, arg);
        mFlag18 = 0;
    }
}

extern "C" void fn_80354110(EnemyQuarkStand* self) {
    self->m1809 = 1;
    self->fn_8016E4C4(0);
    self->m54C |= 0x2000;
    self->mMotion.m34C = 1;
    self->mMotion.mTracks[0].m20 = 1.0f;
    self->mMotion.fn_80097C08(0, 0x1E1, 0, 1, 1, 0.0f, 0.1f);
    self->mMotion.mTracks[0].m0C = 0.01f;
}

// NONMATCHING: 99.45% - the original loads 0.6f into f0 and m5B4 into f3 (ours the other way round), same
// open case as CharaTask::vfD0; ~8 forms of the offset
atn::Vector3 EnemyQuarkStand::vfD0(float angle) {
    Mtx m;
    atn::Vector3 up(0.0f, m5B4 * 0.6f, 0.0f);
    atn::Vector3 pos = mPos + up;
    atn::Vector3 dir;
    PSMTXRotRad(m, 'y', angle);
    dir = atn::Vector3(9.0f, 8.0f, 0.0f);
    PSMTXMultVec(m, &dir, &dir);
    pos += dir;
    return pos;
}


// NONMATCHING: 99.53% - stack slots of the front direction and of the scaled temporary swapped; ~8 forms
atn::Vector3 EnemyQuarkStand::vfD8(CharaTask* target) {
    MtxPtr m = mMotion.GetNodeMtx_unk("head");
    atn::Vector3 pos = GetTransOf_unk(m);
    pos.y += 5.0f;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx r;
    PSMTXRotRad(r, 'y', mRot.y);
    PSMTXMultVec(r, &dir, &dir);
    pos += dir * 5.0f;
    return pos;
}

float EnemyQuarkStand::vfE0() {
    return 1.5707964f + mRot.y;
}

float EnemyQuarkStand::vfDC() {
    return 0.7f;
}

float EnemyQuarkStand::vfD4() {
    return 1.0f;
}

EnemyQuarkStand::~EnemyQuarkStand() {}
