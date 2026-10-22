#include "game/EnemyQuarkStand.h"
#include "atn/Csv.h"
#include "atn/D2A_Task.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
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
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
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
    unsigned char m8A4[0x97C - 0x8A4];
    unsigned char m97C;
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
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: effect/bullet manager (lbl_8087F048)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
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
extern "C" CharaManager<PlayerTask>* fn_8000D9E8();                  // inferred: player manager
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
extern "C" void fn_803EA77C(void* mgr, CharaTask* chara, int id, int arg, float a, float b); // inferred: voice

// inferred: inline constructors of the helper members (weak copies 0x8034585C-0x803458E0)
inline QuarkStandState_unk::QuarkStandState_unk()
    : m00(0), m04(0), m0C(0), m10(0), m14(0), m18(0), m1C(0), m20(0), m24(0), m28(0), m30(0), m34(0), m38(0),
      m3C(0.0f), m40(0), m44(0.0f), m48(0), m4C(0), m50(0), m54(0), m58(0), m5C(0) {}
inline QuarkSlot_unk::QuarkSlot_unk() : m14(0) {}
inline QuarkDir_unk::QuarkDir_unk() {}
inline Mtx34_unk::Mtx34_unk() {}
inline QuarkShot_unk::QuarkShot_unk() {}

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

atn::Vector3 EnemyQuarkStand::vf64(int center) {
    return m624.mData->m04.mCenter;
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

// inferred: state 20 (motion 0x144)
void EnemyQuarkStand::fn_803519C4() {
    fn_8034F384();
    fn_8016E970(3);
    m58C = 20;
    QuarkPlayMotion_unk(&mMotion, 0x144, 0, 0, 1.0f);
    m1CB8 = 0;
    m1CBC = 0;
}

// inferred: plays motion `motion` at `speed` (out of line in vf10)
void EnemyQuarkStand::fn_803524AC(int motion, int arg3, float speed) {
    QuarkPlayMotion_unk(&mMotion, motion, arg3, 0, speed);
}

// inferred: shows the two direction arrows of the d2anime
void EnemyQuarkStand::fn_80352EA4() {
    m17F4->mFlags |= 4;
    m17F8->mFlags |= 4;
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

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetTransOf_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
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
