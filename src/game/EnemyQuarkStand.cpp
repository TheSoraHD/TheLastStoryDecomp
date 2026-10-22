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
// inferred: {id, value} pair of CharaStatus::m2D0 (as CharaStatusPair_unk); 0x80339718 builds it
struct QuarkPair_unk {
    int m00;
    int m04;
};
extern "C" QuarkPair_unk* fn_80339718(QuarkPair_unk* pair, int id);
extern "C" void fn_803396FC(CharaStatus* status, int index, const QuarkPair_unk* pair);
extern "C" HitInfo_unk* fn_80219E6C(int id);

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
