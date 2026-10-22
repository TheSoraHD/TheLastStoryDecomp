#include "game/Em302Kraken.h"
#include "chr/ControlStickByQuark.h"
#include <boost/bind.hpp>
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/Em303KrakenFoot.h"
#include "game/PlayerTask.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
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
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct KrakenMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" KrakenMsgTable_unk* lbl_8087F1E4;
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" float fn_802E2360(Em303KrakenFoot* foot);
extern "C" int fn_802E2328(Em303KrakenFoot* foot); // inferred: nonzero when the leg can act
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: point the camera looks at (lbl_8087F430 + 0x97C, as in game/EnemyTrista.cpp); names invented
struct KrakenLook_unk {
    void Set_unk(const atn::Vector3& pos, bool flag) {
        m01 = m00;
        m00 = 1;
        mPos = pos;
        m24 = 0.0f;
        m04 = flag ? 1 : 0;
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
// inferred: field (lbl_8087F430); names invented
struct Field_unk {
    RouteTable_unk* fn_8013C504(); // inferred: route tables (as in gmk/FlamingArrow.cpp)
    unsigned char m0000[0x8A0];
    CharaTask* m8A0; // inferred: character the camera follows
    unsigned char m8A4[0x97C - 0x8A4];
    KrakenLook_unk m97C;
    atn::Vector3 m988;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_801092C8(Effect_unk* effect, int msg, CharaTask* chara, int value, int arg);
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed (as in game/Arms.cpp)
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
// inferred: Em303KrakenFoot method of another queue (starts a state of the leg); kept extern "C"
extern "C" void fn_802E55B0(Em303KrakenFoot* foot, int kind);
// inferred: ray test against the map (as in gmk/MotionObject.cpp); layout of gmk/Lemon.h
struct KrakenColl_unk {
    KrakenColl_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    void* m34;
    void* m38;
    unsigned int m3C;
    int m40;
    atn::Vector3 m44;
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, KrakenColl_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);
// inferred: damage record sent to the effect manager (layout of game/CharaStatus.cpp)
struct KrakenDamage_unk {
    KrakenDamage_unk() : m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
        m00 = 1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C : 4;
    unsigned int m1C_rest : 28;
};
extern "C" void fn_80108C10(Effect_unk* effect, KrakenDamage_unk* damage, const atn::Vector3& pos, int a, CharaTask* chara,
                            int b, int c);
extern "C" void fn_80107850(Effect_unk* effect, Model_unk* model);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, void* res, int a, int b, atn::Vector3* pos, int c, int d, const float* color,
                            float scale, int e, int f, int g);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: link of the character lists (CharaTask + 0x14AC, as in game/CharaTask.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}

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
// inferred: field camera shake (lbl_8087F430 + 0x96C); names invented
struct KrakenShake_unk {
    int m00;
    int m04;
    float m08;
    float m0C;
};
inline KrakenShake_unk* Shake_unk() {
    return (KrakenShake_unk*)((char*)lbl_8087F430 + 0x96C);
}
// inferred: gimmicks (lbl_8087F4A0, as in game/SpecialEnemyBase.cpp); names invented
struct KrakenGimmick_unk {
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
    virtual void vf40(void* msg);
    unsigned char m04[0x48 - 0x04];
    int m48;
    unsigned char m4C[0x5C - 0x4C];
    KrakenGimmick_unk* mNext;
};
extern "C" GimmickManager* lbl_8087F4A0;
inline KrakenGimmick_unk* FirstGimmick_unk() {
    return (KrakenGimmick_unk*)lbl_8087F4A0->mHead;
}
// inferred: message sent to the gimmicks (0x20 bytes); names invented
struct KrakenGimmickMsg_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    float m14;
    float m18;
    float m1C;
};
// inferred: game state (lbl_8087EFA8, see game/CharaTask.cpp): screen blur parameters at 0x2AC
struct KrakenBlur_unk {
    int m00;
    int m04;
    float m08;
    float m0C;
};
struct KrakenGame_unk {
    unsigned char m000[0x2AC];
    KrakenBlur_unk m2AC;
};
extern "C" KrakenGame_unk* lbl_8087EFA8;

// inferred: default command of fn_802DC8B0 (first object of .data)
static KrakenCommand_unk sDefaultCommand_unk = {8, 0};

// inferred: commands of "act_command" ({name, id}, terminated by a negative id; names invented)
struct KrakenCommandName_unk {
    char mName[0x40];
    int mId;
};
// inferred: static vector of the file built by the __sinit (offset of the shades effect, fn_802DA718)
static atn::Vector3 sOffset_unk(0.0f, 6.0f, 0.0f);

static const KrakenCommandName_unk sCommands_unk[] = {
    {"swing", 12}, {"pound", 13}, {"stick", 14}, {"wait", 15}, {"", -1},
};

// inferred: id of the command `name`, -1 if unknown
static int fn_802D73E4(const char* name) {
    for (int i = 0; sCommands_unk[i].mId >= 0; i++) {
        if (strcmp(name, sCommands_unk[i].mName) == 0) {
            return sCommands_unk[i].mId;
        }
    }
    return -1;
}

extern "C" Field_unk* fn_80121F00();
extern "C" CharaManager<EnemyTask>* fn_801A03E0(); // inferred: enemy manager
extern "C" void* fn_8020A81C(const char* name); // inferred: character data by name
// inferred: element `i` of a vector (weak copies 0x802D8550/0x802D8B6C); names invented
template <class T>
T* At_unk(std::vector<T>* v, unsigned int i) {
    return &v->mData[i];
}
// inferred: size and empty of a vector, out of line (weak copies kept in atn/WordWrap_unk.cpp)
struct Count4_unk;
extern "C" int fn_800E0AA8(const Count4_unk* c);
extern "C" bool fn_800E0AB0(const Count4_unk* c);
// inferred: clears flags of the model (weak copy 0x802D8540); name invented
inline void ClearModelFlag_unk(Model_unk* model, unsigned int flag) {
    model->mFlags &= ~flag;
}

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: back to state 0 (names invented)
inline void Em302Kraken::Reset_unk() {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    fn_800CB5C8(&m1588, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
}

// inferred: plays a motion on track 0 (names invented, as in game/EnemyTrista.cpp)
inline void PlayMotion_unk(Model_unk* model, int motion, int loop) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = 1.0f;
    model->fn_80097C08(0, motion, loop, 0, 1, 0.0f, 0.1f);
}

// inferred: plays an effect at a world position (same code in game/Em302KrakenMp.cpp); name invented
inline void PlayEffectAt_unk(void* owner, int id, atn::EfpHandle_unk* efp, const atn::Vector3& pos, float scale) {
    fn_80232B7C(owner, id);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, pos, lbl_807C7030, color, -1, -1, 1, scale);
}

// inferred: same as PlayEffect_unk with the locals in the other order (color, rot, pos; as in
// fn_802DB7C8); name invented
inline void PlayEffectC_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

// inferred: legs alive and free (mC58.mC0 == 1); name invented
inline int CountFreeFeet_unk(Em302Kraken* k) {
    int count = 0;
    for (unsigned int i = 0; i < k->m1678.size(); i++) {
        Em303KrakenFoot* foot = k->m1678[i];
        if ((foot->mStatus.m0C & 0x20) != 0x20 && foot->mC58.mC0 == 1) {
            count++;
        }
    }
    return count;
}


// inferred: same as PlayEffect_unk with the manager read first (as in game/Em302KrakenMp.cpp); name
// invented
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

// NONMATCHING: 98% - registers of the second std::string constructor (r26/r27, as in
// game/Em201ZanGurg.cpp) and the load order of the eight vector clears
Em302Kraken::Em302Kraken(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(2), m14BC(2), m14C0(0), m14C4(0), m14C8(0),
      m14CC(-70.0f), m14D0(0), m14D4(0), m14D8(10), m14DC(-1), m14F8(0x450), m1514(0.5f), m1518(0.0f, 0.0f, 0.0f),
      m1560(0), m157C(0), m1580(0), m1584(0x712), m15C8(0.0f, 0.0f, 0.0f), m15D4(0), m15D8(-1),
      m15DC(1420.0f), m15E0(20.0f), m15E4(-1720.0f), m1600(0), m1604(0), m1608(0x717), m1630(0), m1634(0),
      m1638(0), m1654(0), m1658(0), m165C(0), m169C(0), m16A8(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x210;
    std::string dir("database/chara/special/");
    std::string name("db_em302_00_00.csv");
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
    m1684.vf0C(path.c_str());
    m14E0.fn_8023780C("/effect/ef_bs033a.efp");
    m14EC.fn_8023780C("/effect/ef_bs033c.efp");
    m1508.fn_8023780C("/effect/ef_bs030c.efp");
    m15F4.fn_8023780C("/effect/ef_bd004.efp");
    m1524.fn_80237654("/effect/ef_bs034b_01.eff");
    m1530.fn_8023780C("/effect/ef_bs034c.efp");
    m153C.fn_8023780C("/effect/ef_bs034a.efp");
    m1548.fn_8023780C("/effect/ef_bs034e.efp");
    m1564.fn_8023780C("/effect/ef_bu022.efp");
    m1570.fn_8023780C("/effect/ef_bu023.efp");
    m158C.fn_8023780C("/effect/ef_bs028.efp");
    m1598.fn_8023780C("/effect/ef_bs029a.efp");
    m15A4.fn_8023780C("/effect/ef_bs029c.efp");
    m15E8.fn_8023780C("/effect/ef_bd005.efp");
    m160C.fn_8023780C("/effect/ef_bs081a.efp");
    m1618.fn_8023780C("/effect/ef_bs081b.efp");
    m1624.fn_8023780C("/effect/ef_bs049c.efp");
    m1554.clear();
    m163C.clear();
    m1648.clear();
    m1660.clear();
    m166C.clear();
    m1678.clear();
    m168C.clear();
    m15BC.clear();
    m1698 = 0;
}

Em302Kraken::~Em302Kraken() {}

// NONMATCHING: 99.4% - saved registers of the reading loop rotated (key/pool/zero: r28/r29/r30 in the
// original, r30/r28/r29 here; 8 variants of the declarations and of the pillar_id loop)
int Em302Kraken::vf0C() {
    if (m14B0 == 0) {
        if (fn_8013655C() == 0 && m14E0.fn_80237874() == 0 && m14EC.fn_80237874() == 0 &&
            m1508.fn_80237874() == 0 && m15F4.fn_80237874() == 0 && m1524.fn_802376D0() == 0 &&
            m1530.fn_80237874() == 0 && m153C.fn_80237874() == 0 && m1548.fn_80237874() == 0 &&
            m1564.fn_80237874() == 0 && m1570.fn_80237874() == 0 && m15E8.fn_80237874() == 0 &&
            m158C.fn_80237874() == 0 && m1598.fn_80237874() == 0 && m15A4.fn_80237874() == 0 &&
            m160C.fn_80237874() == 0 && m1618.fn_80237874() == 0 && m1624.fn_80237874() == 0 &&
            m1684.fn_80473F50() == 0) {
            if (m1684.fn_80470580() != 0 && fn_80121F00() != 0 && fn_80121F00()->fn_8013C504() != 0) {
                atn::Csv csv((const char*)m1684.fn_80470580(), m1684.fn_8047059C());
                int zero = 0;
                do {
                    const char* key = csv.fn_8005B9CC();
                    if (strcmp(key, "appear_point") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                            if (place != 0) {
                                m163C.push_back(place);
                            }
                        } while (id != 0);
                        if (!fn_800E0AB0((const Count4_unk*)&m163C)) {
                            m1634 = *At_unk(&m163C, m1638);
                        }
                    } else if (strcmp(key, "reinforce_point") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                            if (place != 0) {
                                m1660.push_back(place);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "pillar_id") == 0) {
                        int id = zero;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            if (id != 0) {
                                m1648.push_back(id);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "reinforce_chara") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            EnemyTask* enemy = fn_801A03E0()->Find_unk(id);
                            if (enemy != 0) {
                                m166C.push_back(enemy);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "absorb_rate") == 0) {
                        m1514 = fn_800DC288(csv.fn_8005B9CC());
                    } else if (strcmp(key, "foot_chara") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            Em303KrakenFoot* foot = (Em303KrakenFoot*)fn_801A03E0()->Find_unk(id);
                            if (foot != 0) {
                                m1678.push_back(foot);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "act_command") == 0) {
                        while (csv.fn_8005BBF8()) {
                            const char* name = csv.fn_8005B9CC();
                            if (*name == ';' || *name == '#') {
                                continue;
                            }
                            if (strcmp(name, "/act_command") == 0) {
                                break;
                            }
                            KrakenCommand_unk cmd;
                            cmd.mId = fn_802D73E4(name);
                            if (cmd.mId > 0) {
                                cmd.mValue = fn_800DC12C(csv.fn_8005B9CC());
                                m168C.push_back(cmd);
                            }
                        }
                    } else if (strcmp(key, "charge_point") == 0) {
                        RoutePlace_unk* place =
                            fn_80121F00()->fn_8013C504()->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                        if (place != 0) {
                            m15D4 = place;
                        }
                    } else if (strcmp(key, "shades_skill_id") == 0) {
                        m14F8 = fn_800DC12C(csv.fn_8005B9CC());
                    } else if (strcmp(key, "laser_skill_id") == 0) {
                        m1584 = fn_800DC12C(csv.fn_8005B9CC());
                    } else if (strcmp(key, "thunder_skill_id") == 0) {
                        m1608 = fn_800DC12C(csv.fn_8005B9CC());
                    } else if (strcmp(key, "shoot_thunder") == 0) {
                        m1604 = fn_800DC12C(csv.fn_8005B9CC());
                    } else if (strcmp(key, "pos_y_offset") == 0) {
                        m14CC = fn_800DC288(csv.fn_8005B9CC());
                    } else if (strcmp(key, "explode_center") == 0) {
                        m15DC = fn_800DC288(csv.fn_8005B9CC());
                        m15E0 = fn_800DC288(csv.fn_8005B9CC());
                        m15E4 = fn_800DC288(csv.fn_8005B9CC());
                    } else if (strcmp(key, "week_param") == 0) {
                        void* data = fn_8020A81C(csv.fn_8005B9CC());
                        if (data != 0) {
                            m16B0 = data;
                        }
                    } else if (strcmp(key, "anger_efp") == 0) {
                        m15B0.fn_8023780C(csv.fn_8005B9CC());
                    }
                } while (csv.fn_8005BBF8());
            }
            mStatus.m18 |= 0x1C0;
            mStatus.m18 |= 0x10000;
            mStatus.m18 |= 0xC21D;
            mStatus.m18 |= 0x3800000;
            for (unsigned int i = 0; i < fn_800E0AA8((const Count4_unk*)&m1678); i++) {
                fn_802E2468(*At_unk(&m1678, i), (int)this);
            }
            fn_802DDBB0();
            m14B0 = 1;
            ClearModelFlag_unk((Model_unk*)&mMotion, 4);
        }
    } else if (m14B0 == 1) {
        if (m15B0.fn_80237874() == 0) {
            m14B0 = 2;
            return 1;
        }
    } else if (m14B0 == 2) {
        return 1;
    }
    return 0;
}

// inferred: the original emits these copies right after vf0C (0x802D8560/0x802D8854), before vf10
template void std::vector<Em303KrakenFoot*>::push_back(Em303KrakenFoot* const& x);
template void std::vector<KrakenCommand_unk>::push_back(const KrakenCommand_unk& x);

// NONMATCHING: 97.6% - registers of the camera block (the original addresses the look object through a
// pointer, as EnemyTrista::fn_8035D294) and stack slots of the gimmick message
void Em302Kraken::vf10() {
    if (m58C != 8) {
        mC58.mC8 = (int)mC58.mC4;
        CharaTask* leader = lbl_8087F8A0->mHead;
        if (leader->mFlag28 && !(leader->m55C == 6 && leader->m560 == 0x11)) {
            m1454 = 0;
            m1630 = leader;
        } else {
            m1454 = 1;
            m1630 = mC58.mC4;
        }
        mC58.mC4 = m1630;
    }
    if (m1658 > 0) {
        m1658--;
        CharaTask* target = (CharaTask*)m1654;
        if (target != 0) {
            Field_unk* field = lbl_8087F430;
            if (m1658 > 0) {
                atn::Vector3 pos = *(atn::Vector3*)((char*)target + 0x6C);
                pos.y += 20.0f;
                if (m165C == 0) {
                    field->m8A0 = this;
                    field->m97C.Set_unk(pos, false);
                    m165C = 1;
                } else {
                    field->m8A0 = this;
                    field->m988 = pos;
                }
            } else if (m165C != 0) {
                field->m8A0 = 0;
                field->m97C.m00 = 0;
                m165C = 0;
            }
        }
    }
    if (mC58.mC0 == 0 || mC58.mC4 == 0) {
        fn_8016E970(3);
        if (m58C == 7 && m14B8 == 3) {
            fn_802DA174();
        }
    } else {
        if (m15D8 > 0 && --m15D8 == 0) {
            for (KrakenGimmick_unk* g = FirstGimmick_unk(); g != 0; g = g->mNext) {
                if (g->m48 == 0x444) {
                    KrakenGimmickMsg_unk msg;
                    msg.m00 = 0;
                    msg.m04 = 0;
                    msg.m08 = 0;
                    msg.m0C = 0;
                    msg.m10 = 0;
                    msg.m14 = 0.0f;
                    msg.m18 = 0.0f;
                    msg.m1C = 0.0f;
                    g->vf40(&msg);
                }
            }
        }
        if (m169C > 0) {
            m169C--;
        }
        int frame = ++m14C4;
        switch (m58C) {
        case 6: {
            float f = mMotion.mTracks[0].m08;
            if (f >= mMotion.fn_80097D7C(0)) {
                Reset_unk();
            }
            break;
        }
        case 7:
            if (mMotion.mTracks[0].m08 <= 0.0f) {
                vf9C(1);
                m1560 = 1;
                Reset_unk();
            }
            break;
        case 8:
            fn_802DA718();
            break;
        case 9:
            fn_802DAA38();
            break;
        case 10:
            fn_802DB2D0();
            break;
        case 11:
            if (mMotion.mTracks[0].m00 == 0x145) {
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
                fn_80370AE4(lbl_8087F430, 0x5B, 1);
            } else if (frame < 20) {
                // inferred: an unused value (the original converts the frame to float here and pools
                // 15.0f right after 0.1f, as in Em302KrakenMp::vf104)
                float rate = frame / 15.0f;
            } else if (frame >= 45) {
                fn_802DC3D0();
            }
            break;
        case 12:
            fn_802DB534();
            break;
        case 13:
            fn_802DB7C8();
            break;
        case 14:
            fn_802DBA3C();
            break;
        case 15:
            fn_802DBC68();
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
            fn_802DA174();
            break;
        }
        }
    }
    m14D4++;
    RoutePlace_unk* place = m1634;
    if (place != 0) {
        mPos = place->mPos;
        mRot.y = 3.1415927f + place->m14;
    }
    mPos += m15C8;
    mPos.y += m14CC;
    if (m14B8 == 3) {
        mPos.y -= 1000.0f;
    }
    fn_8014C540();
    fn_80145334();
    m16A0 -= 0.1f;
    if (m16A0 < 0.0f) {
        m16A0 = 0.0f;
    }
    if ((mStatus.m0C & 0x20) != 0x20) {
        CharaTask* leader = lbl_8087F8A0->mHead;
        if (m58C == 12 && m14C4 > 45) {
            m16A0 += 0.5f;
        }
        for (unsigned int i = 0; i < m1678.size(); i++) {
            m16A0 += fn_802E2360(m1678[i]);
        }
        m16A0 = m16A0 < 1.0f ? m16A0 : 1.0f;
        KrakenBlur_unk blur;
        KrakenGame_unk* game = lbl_8087EFA8;
        blur.m04 = game->m2AC.m04;
        blur.m00 = 1;
        blur.m0C = m16A0;
        blur.m08 = 5.0f;
        if (leader->m570 > 1.0f) {
            blur.m0C = m16A0 + 0.1f;
        }
        game->m2AC = blur;
    }
}

void Em302Kraken::vf14() {
    if (m1560 != 0) {
        return;
    }
    fn_80149A30();
}

void Em302Kraken::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m50 |= 0x10;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    vf40(hit);
}

void Em302Kraken::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802DC618();
    }
}

void Em302Kraken::vf50() {
    Item14_unk item;
    item.m04.mRadius = m5B0;
    MtxPtr head = mMotion.GetNodeMtx_unk("head");
    item.m04.mCenter = GetNodePos_unk(head) + m5A4;
    m624.push_back(item);
    mFlagB29 = 0;
}

void Em302Kraken::vf54() {
    if (m624.mSize != 0) {
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        m624.mData[0].m04.mCenter = GetNodePos_unk(head) + m5A4;
    }
}

void Em302Kraken::vf20() {
    m620 = m5B0;
    MtxPtr head = mMotion.GetNodeMtx_unk("head");
    m614 = GetNodePos_unk(head) + m5A4;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += m5B4;
    float r = m620;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = r;
}

// inferred: point to aim at while the head is up (state kind 1): the first collision sphere raised
// by 0.6 of the radius
int Em302Kraken::vfB0(atn::Vector3* out) {
    if (m14B8 != 1) {
        return 0;
    }
    Item14_unk* it = m624.mData;
    *out = it->m04.mCenter + atn::Vector3(0.0f, 1.0f, 0.0f) * mColli.m54 * 0.6f;
    return 1;
}

// inferred: action of a character that climbed onto the kraken: the kraken starts state 15 and the
// character thrusts its weapon (chr::ControlThrustForKraken)
boost::function<void()> Em302Kraken::vfB4(CharaTask* chara) {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    m1600 = 0;
    chr::ControlThrustForKraken* next =
        new (0, "", "", 0) chr::ControlThrustForKraken(chara, (int)this, chara->mPos);
    return boost::bind(&CharaTask::fn_80178208, chara, next);
}

// inferred: the point transformed by the matrix (as in game/CharaTask.cpp); name invented
inline atn::Vector3 TransformVec_unk(const Mtx m, const atn::Vector3& v) {
    atn::Vector3 r(v);
    PSMTXMultVec(m, &r, &r);
    return r;
}

// inferred: four points around the head (the places the party can climb to), rotated with the kraken
// NONMATCHING: 96.4% - the original stores z of the sum temporary after the paired copy to out[i] and
// reloads mRot.y for the last rotation only; 6 variants
void Em302Kraken::vfCC(atn::Vector3* out, const atn::Vector3& dir) {
    Mtx m0;
    Mtx m1;
    Mtx m2;
    Mtx m3;
    atn::Vector3 base = GetNodePos_unk(mMotion.GetNodeMtx_unk("head")) + m5A4;
    float ry = mRot.y;
    PSMTXRotRad(m0, 'y', ry);
    out[0] = base + TransformVec_unk(m0, atn::Vector3(16.0f, 15.0f, 16.0f));
    PSMTXRotRad(m1, 'y', mRot.y);
    out[1] = base + TransformVec_unk(m1, atn::Vector3(-12.0f, 15.0f, 12.0f));
    PSMTXRotRad(m2, 'y', mRot.y);
    out[2] = base + TransformVec_unk(m2, atn::Vector3(20.0f, 15.0f, 12.0f));
    PSMTXRotRad(m3, 'y', mRot.y);
    out[3] = base + TransformVec_unk(m3, atn::Vector3(-16.0f, 15.0f, 16.0f));
}

atn::Vector3 Em302Kraken::vfD0(float angle) {
    atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("head")) + m5A4;
    pos.y += 25.0f;
    return pos;
}

atn::Vector3 Em302Kraken::vfD8(CharaTask* target) {
    atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("head")) + m5A4;
    pos.y += 18.0f;
    return pos;
}

// inferred: changes the behaviour step m14B8 (0: appears at the next point, 1: attacks, 2: sinks,
// 3: waits while the legs are free, 4: anger/thunder); m14C0 counts the frames of the step (as
// Em302KrakenMp::fn_802E03F0)
void Em302Kraken::fn_802DA174() {
    switch (m14B8) {
    case 0:
        if (m14C0 == 0) {
            if (m163C.size() != 0) {
                if ((int)m163C.size() <= (int)++m1638) {
                    m1638 = 0;
                }
                m1634 = m163C[m1638];
                RoutePlace_unk* place = m1634;
                if (place != 0) {
                    mPos = place->mPos;
                    mRot.y = 3.1415927f + place->m14;
                }
                mPos += m15C8;
                mPos.y += m14CC;
                if (m14B8 == 3) {
                    mPos.y -= 1000.0f;
                }
            }
            fn_802DBE70(0);
        }
        if (m14C0 == 1) {
            m14BC = m14B8;
            m14B8 = 4;
            m14C0 = 0;
            m14D4 = 0;
            return;
        }
        break;
    case 1:
        if (m1580 != 0 || m14D0 >= 5) {
            m14BC = m14B8;
            m14B8 = 3;
            m14C0 = 0;
            fn_802DDA90();
            return;
        } else {
            int next;
            if (m14C0 > 300) {
                mC58.mC0 = 1;
                m54C &= ~0x2000;
                m14BC = m14B8;
                m14B8 = 3;
                m14C0 = 0;
                next = 0;
            } else {
                if (m14D4 > 0) {
                    m14C8 = 0;
                    m14C4 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    m58C = 8;
                    fn_8016E970(3);
                    fn_800C344C("SE_EM302_013", &m614, 0, -1, 1.0f);
                    m14D4 = 0;
                    m14D0++;
                }
                next = 1;
            }
            if (next == 0) {
                return;
            }
        }
        break;
    case 2:
        if (m14C0 == 0) {
            int n = m14DC + 1;
            if (n >= 1) {
                n = 0;
            }
            m14DC = n;
            m14C8 = 0;
            m14C4 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 10;
            fn_8016E970(3);
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
            fn_802DC014();
        }
        if (m14C0 == 1) {
            fn_802DDA90();
            m14BC = m14B8;
            m14B8 = 3;
            m14C0 = 0;
            return;
        }
        break;
    case 3: {
        int count = CountFreeFeet_unk(this);
        if (count <= 0) {
            mC58.mC0 = 1;
            m54C &= ~0x2000;
            m14BC = m14B8;
            m14B8 = 0;
            m14C0 = 0;
            return;
        }
        mC58.mC0 = 0;
        m54C |= 0x2000;
        if (m14C0 == 0) {
            fn_802DC014();
        }
        int next;
        if (m14C0 > 900) {
            mC58.mC0 = 1;
            m54C &= ~0x2000;
            m14BC = m14B8;
            m14B8 = 0;
            m14C0 = 0;
            next = 0;
        } else {
            next = 1;
        }
        if (next == 0) {
            return;
        }
        break;
    }
    case 4:
        if (m14C0 == 0 && m14BC == 3) {
            fn_802DBE70(1);
        }
        if (m14C0 == 1) {
            if (m1604 != 0) {
                m14C8 = 0;
                m14C4 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 13;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
                fn_80232B7C(this, 100);
                PlayEffectC_unk(&m160C, (Model_unk*)&mMotion);
                fn_800C31F4("SE_SP016_001", 0, -1, 1.0f);
            } else {
                fn_80370AE4(lbl_8087F430, 0xCC, 1);
                fn_802DC1C0();
            }
        }
        if (m14C0 == 2) {
            m14BC = m14B8;
            m14B8 = 1;
            m14C0 = 0;
            m14D4 = 0;
            m14D0 = 0;
            m1580 = 0;
            return;
        }
        break;
    }
    m14C0++;
}

// inferred: state 4 (shades): marks the ground under the target at frame 15 and fires at frame 65
// NONMATCHING: 90.2% - the original rounds x/z with frsp after the +0.3 (double somewhere) and keeps
// the constants of the two effects in saved registers; stack slots of the fire position
void Em302Kraken::fn_802DA718() {
    HitInfo_unk* skill = fn_80219E6C(m14F8);
    if (m14C4 > 120) {
        Reset_unk();
    }
    if (m14C4 == 15) {
        m14FC = m1630->mPos;
        m14FC.y += 0.1f;
        atn::Vector3 to = m14FC;
        to.y -= 100.0f;
        KrakenColl_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &m14FC, &to, 0, &m1630->mColli, 0) != 0) {
            m14FC = result.m10;
        }
        m14FC.x += 0.3f;
        m14FC.z += 0.3f;
        atn::Vector3 pos = m14FC + sOffset_unk;
        Color4f_unk color;
        float one = 1.0f;
        fn_80232B7C(this, 10);
        color.r = one;
        color.g = one;
        color.b = one;
        color.a = one;
        fn_8023A8B4(lbl_8087F3C0, &m1508, 0, 0, pos, lbl_807C7030, color, -1, -1, 1, one);
        fn_80232B7C(this, 500);
        Color4f_unk color2;
        color2.r = one;
        color2.g = one;
        color2.b = one;
        color2.a = one;
        fn_8023A8B4(lbl_8087F3C0, &m1570, 0, 0, m14FC, lbl_807C7030, color2, -1, -1, 1, one);
    }
    if (m14C4 == 30) {
        fn_80239DAC(lbl_8087F3C0, this, 500, 1);
    }
    if (m14C4 == 65) {
        atn::Vector3 pos = m14FC + sOffset_unk;
        fn_800FAB80(lbl_8087F048, this, skill, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        fn_800C344C("SE_EM301_002", &m14FC, 0, -1, 1.0f);
    }
}

// NONMATCHING: 86.5% - draft: order of the loads in the pull loop, the stack slots of the damage record
// and the rotation of the knockback (inlined ToRotation_unk)
// inferred: state 10 (absorbs the party): binds the targets of m1554 at frame 10 and pulls them to the
// head (frames 21-90), then deals the damage, pushes them away and back to state 0
void Em302Kraken::fn_802DAA38() {
    if (m14C4 == 10) {
        m1518 = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
        fn_80232B7C(this, 12);
        for (unsigned int i = 0; i < m1554.size(); i++) {
            KrakenTarget_unk* t = &m1554[i];
            t->m10 = GetNodePos_unk(m1554[i].mChara->mMotion.GetNodeMtx_unk("hips"));
            m1554[i].m04 = m1554[i].m10;
            float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            fn_8023A680(lbl_8087F3C0, &m1524, -1, 6, &m1554[i].m04, 0, 0, 0, 1.0f, 0, -1, 1);
            PlayEffect_unk(&m1530, (Model_unk*)&m1554[i].mChara->mMotion);
        }
        wchar_t text[256];
        memset(text, 0, sizeof(text));
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x12D), *(const wchar_t**)((char*)m60 + 4));
        fn_80109828(lbl_8087F048, text);
    }
    if (m14C4 > 20 && m14C4 <= 90) {
        float t = (m14C4 - 20) / 70.0f;
        float a = 1.0f - t * t;
        float b = t * t;
        float c = 1.0f - t;
        for (unsigned int i = 0; i < m1554.size(); i++) {
            KrakenTarget_unk* e = &m1554[i];
            atn::Vector3 from = e->m10 * a;
            atn::Vector3 to = m1518 * b;
            e->m04 = to + from;
            m1554[i].m04.y = m1518.y * t + c * m1554[i].m10.y;
        }
    }
    if (m14C4 > 100) {
        fn_80239DAC(lbl_8087F3C0, this, 102, 0);
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1548, (Model_unk*)&mMotion);
        KrakenDamage_unk damage;
        int total = 0;
        for (unsigned int i = 0; i < m1554.size(); i++) {
            CharaTask* chara = m1554[i].mChara;
            int hp = (int)chara->mStatus.m04;
            damage.m04 = hp + 1;
            total -= (int)(m1514 * chara->mStatus.m04) + 1;
            chara->mStatus.fn_8012DF7C(hp + 1, 0, 0);
            fn_80108C10(lbl_8087F048, &damage, chara->mPos + atn::Vector3(0.0f, 15.0f, 0.0f), (int)this, chara, 0, 0);
            fn_80107850(lbl_8087F048, &chara->mMotion);
            atn::Vector3 rot = chara->mRot.ToRotation_unk();
            atn::Vector3 dir = rot * -1.0f;
            dir.Normalize_unk();
            dir *= 0.01f;
            chara->fn_8015E7A0(dir, -1, 0);
        }
        if (total < 0) {
            damage.m04 = total;
            atn::Vector3 pos = fn_80148B0C(0)->m04.mCenter + atn::Vector3(0.0f, 15.0f, 0.0f);
            mStatus.fn_8012DF7C(total, 0, 0);
            fn_80108C10(lbl_8087F048, &damage, pos, 0, this, 0, 0);
            wchar_t text[256];
            memset(text, 0, sizeof(text));
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x12F), *(const wchar_t**)((char*)m60 + 4));
            fn_80109828(lbl_8087F048, text);
        }
        Reset_unk();
    }
}

// inferred: state 9 (absorbs the party): effect on every member at frame 50, then the reinforcements
// (the count grows as the hit points go down) and back to state 0
void Em302Kraken::fn_802DB2D0() {
    if (m14C4 == 3) {
        atn::Vector3 pos(0.0f, 10.0f, 0.0f);
        Color4f_unk color;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m14E0, (Model_unk*)&mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, 3.0f);
    }
    if (m14C4 == 50) {
        for (CharaTask* c = (CharaTask*)lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
            fn_80232B7C(c, 1000);
            PlayEffect_unk(&m14EC, (Model_unk*)&c->mMotion);
        }
    }
    if (m14C4 > 60) {
        fn_802DD1B4((int)(3.0f * (1.0f - mStatus.m04 / mStatus.mParamB.m84)));
        int kind = 1;
        switch (m14DC) {
        case 2:
            kind = 3;
            break;
        case 0:
            kind = 3;
            break;
        case 3:
            kind = 1;
            break;
        }
        fn_802DCA34(m14DC, kind);
        Reset_unk();
    }
}

// inferred: state 12 (big laser)
void Em302Kraken::fn_802DB534() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0) && m14C4 > 180) {
        Reset_unk();
        m1658 = 45;
        fn_80370AE4(lbl_8087F430, 0xC9, 1);
    }
    if (m14C4 == 20) {
        KrakenShake_unk* shake = Shake_unk();
        shake->m00 = shake->m00 % 2;
        shake->m04 = 120;
        shake->m08 = 1.0f;
        shake->m0C = 1.0f;
        for (KrakenGimmick_unk* g = FirstGimmick_unk(); g != 0; g = g->mNext) {
            if (g->m48 == 0x444) {
                KrakenGimmickMsg_unk msg;
                msg.m00 = 2;
                msg.m04 = 0;
                msg.m08 = 0;
                msg.m0C = 0;
                msg.m10 = 0;
                msg.m14 = 0.0f;
                msg.m18 = 0.0f;
                msg.m1C = 0.0f;
                g->vf40(&msg);
                m15D8 = 300;
            }
        }
        if (fn_80370A78(lbl_8087F430, 0xCC) == 0) {
            fn_80370AE4(lbl_8087F430, 0xCC, 1);
        }
    }
    if (m14C4 - 100U <= 14) {
        HitInfo_unk* skill = fn_80219E6C(m1584);
        fn_800FAB80(lbl_8087F048, this, skill, m590, (atn::Vector3*)&m15DC, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        unsigned int skip = m1638;
        for (unsigned int i = 0; i < m1648.size(); i++) {
            GimmickCommon* g = lbl_8087F4A0->fn_803EEE10(0x6DD6, m1648[i]);
            if (g != 0 && i != skip && g->m54 == 1) {
                KrakenGimmickMsg_unk msg;
                msg.m00 = 2;
                msg.m04 = 0;
                msg.m08 = 0;
                msg.m0C = 0;
                msg.m10 = 0;
                msg.m14 = 0.0f;
                msg.m18 = 0.0f;
                msg.m1C = 0.0f;
                ((KrakenGimmick_unk*)g)->vf40(&msg);
            }
        }
    }
}

// inferred: state 13 (charges the thunder)
void Em302Kraken::fn_802DB7C8() {
    if (mMotion.mTracks[0].m00 == 0x145) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
        }
    }
    if (m14C4 == 45) {
        fn_80239DAC(lbl_8087F3C0, this, 100, 0);
        fn_80232B7C(this, 100);
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
        fn_8023A8B4(lbl_8087F3C0, &m160C, (Model_unk*)&mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        fn_800C31F4("SE_SP016_001", 0, -1, 1.0f);
    } else if (m14C4 >= 90) {
        m14C8 = 0;
        m14C4 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 14;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
        Color4f_unk color;
        atn::Vector3 pos(0.0f, 1.0f, 0.0f);
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        fn_80232B7C(this, 101);
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m1618, (Model_unk*)&mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, 1.0f);
        fn_800C31F4("SE_SP016_002", 0, -1, 1.0f);
    }
}

// inferred: state 14 (thunder)
void Em302Kraken::fn_802DBA3C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0) && m14C4 > 200) {
        Reset_unk();
        m1658 = 45;
    }
    if (m14C4 - 20U <= 159) {
        if (m14C4 == 23) {
            atn::Vector3 at = mPos + atn::Vector3(0.0f, 70.0f, 0.0f);
            PlayEffectAt_unk(0, 0, &m1624, at, 2.5f);
        }
        HitInfo_unk* skill = fn_80219E6C(m1608);
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        atn::Vector3 pos = GetNodePos_unk(head);
        fn_800FAB80(lbl_8087F048, this, skill, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    } else if (m14C4 >= 180) {
        fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    }
}

// inferred: state 15 (anger)
void Em302Kraken::fn_802DBC68() {
    if (m14C4 == 50) {
        fn_80370AE4(lbl_8087F430, 0x5C, 1);
        fn_800C344C("SE_EM302_019", &m614, 0, -1, 1.0f);
    }
    if (mMotion.mTracks[0].m00 == 0x14D) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m1580 = 1;
            Reset_unk();
        }
    } else if (m14C4 > 45) {
        fn_800CB5C8(&m1588, 0, 0);
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        fn_80239DAC(lbl_8087F3C0, this, 101, 1);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    }
}

void Em302Kraken::vfEC() {
    if (mMotion.mTracks[0].m08 <= 0.0f) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        fn_800EE360(this);
        mFlag15 = 1;
        fn_801765D8();
    }
}

// inferred: starts state 6 (rises from the water; kind 0 or 1 picks the motion)
void Em302Kraken::fn_802DBE70(int kind) {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(3);
    m157C = kind;
    vf98();
    m1560 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    switch (m157C) {
    case 0:
        mMotion.fn_80097C08(0, 0x2B, 0, 0, 1, 0.0f, 0.1f);
        break;
    case 1:
        mMotion.fn_80097C08(0, 0x2D, 0, 0, 1, 0.0f, 0.1f);
        break;
    }
    m15C8 = lbl_807C7030;
    PlayEffectAt_unk(this, 20, &m1564, mPos + atn::Vector3(0.0f, 70.0f, 0.0f), 1.0f);
}

// inferred: starts state 7 (sinks): sound, effect and the camp message
void Em302Kraken::fn_802DC014() {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 7;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = -1.0f;
    mMotion.fn_80097C08(0, 0x2D, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m08 = mMotion.fn_80097D7C(0);
    fn_800C344C("SE_EM302_014", &m614, 0, -1, 1.0f);
    PlayEffectAt_unk(this, 21, &m1570, mPos + atn::Vector3(0.0f, 70.0f, 0.0f), 1.0f);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x135), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

// inferred: starts state 11 (anger): effects, sound and the camp message
void Em302Kraken::fn_802DC1C0() {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 11;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    m1588 = fn_800C344C("SE_EM302_011", &m614, 0, -1, 1.0f);
    fn_80232B7C(this, 100);
    PlayEffect2_unk(&m158C, (Model_unk*)&mMotion);
    fn_80232B7C(this, 101);
    PlayEffect2_unk(&m15B0, (Model_unk*)&mMotion);
    m15C8 = lbl_807C7030;
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x137), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

// inferred: starts state 12 (dives): asks the pillar of the current appear point to sink
void Em302Kraken::fn_802DC3D0() {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 12;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
    fn_800C344C("SE_EM302_012", &m614, 0, -1, 1.0f);
    fn_800CB5C8(&m1588, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1598, (Model_unk*)&mMotion);
    GimmickCommon* g;
    unsigned int index = m1638;
    if (m1648.empty()) {
        g = 0;
    } else {
        g = lbl_8087F4A0->fn_803EEE10(0x6DD6, m1648[index]);
        if (g != 0) {
            KrakenGimmickMsg_unk msg;
            msg.m00 = 1;
            msg.m04 = 0;
            msg.m08 = 0;
            msg.m0C = 0;
            msg.m10 = 0;
            msg.m14 = 0.0f;
            msg.m18 = 0.0f;
            msg.m1C = 0.0f;
            ((KrakenGimmick_unk*)g)->vf40(&msg);
        } else {
            g = 0;
        }
    }
    m1654 = g;
}

void Em302Kraken::fn_802DC5B4(int arg) {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    m1600 = arg;
}

// inferred: starts state 2 (dead)
void Em302Kraken::fn_802DC618() {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_8016E970(3);
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = -0.5f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m08 = mMotion.fn_80097D7C(0);
    for (unsigned int i = 0; i < m1678.size(); i++) {
        Em303KrakenFoot* foot = m1678[i];
        foot->vf44();
        fn_802E55B0(foot, 3);
    }
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m15E8, (Model_unk*)&mMotion);
    fn_800CB5C8(&m1588, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) != 0 && mType == 2) {
        fn_801092C8(lbl_8087F048, 6, this, 0, 0);
    }
    KrakenBlur_unk blur;
    KrakenGame_unk* game = lbl_8087EFA8;
    blur.m04 = game->m2AC.m04;
    blur.m08 = game->m2AC.m08;
    blur.m00 = 0;
    blur.m0C = m16A0;
    game->m2AC = blur;
    fn_800C31F4("SE_EM302_020", 0, -1, 1.0f);
    fn_800EB7A0(this);
}

// inferred: next command of "act_command" for the legs (called by Em303KrakenFoot); the default
// command (id 8) when no leg is busy. The limit of idle legs depends on the hit points.
KrakenCommand_unk* Em302Kraken::fn_802DC8B0() {
    if (m168C.size() == 0) {
        return 0;
    }
    int busy = 0;
    int idle = 0;
    for (unsigned int i = 0; i < m1678.size(); i++) {
        Em303KrakenFoot* foot = m1678[i];
        if (fn_802E2328(foot) == 0) {
            idle++;
        }
        if ((foot->mStatus.m0C & 0x20) != 0x20) {
            if (foot->m58C - 8U <= 3) {
                busy++;
            } else if (foot->m14C4 - 8U <= 3) {
                busy++;
            }
        }
    }
    float rate = mStatus.m04 / mStatus.mParamB.m84;
    int limit;
    if (rate < 0.5f) {
        limit = 8;
    } else if (rate < 0.8f) {
        limit = 6;
    } else {
        limit = 4;
    }
    if (idle >= limit) {
        return 0;
    }
    if (busy < 1 && m169C <= 0) {
        m169C = 0;
        return &sDefaultCommand_unk;
    }
    if (m168C.size() <= m1698) {
        m1698 = 0;
    }
    return &m168C[m1698++];
}

void Em302Kraken::fn_802DDA90() {
    for (unsigned int i = 0; i < m1678.size(); i++) {
        Em303KrakenFoot* foot = m1678[i];
        if ((foot->mStatus.m0C & 0x20) == 0x20 || foot->mC58.mC0 != 1) {
            foot->mFlag15 = 0;
            foot->mFlags &= ~4;
            foot->fn_800D246C(0);
            if (foot->mFlag25) {
                foot->mFlag25 = 0;
            }
            foot->mStatus.fn_8012D8B8();
            foot->mC58.mC0 = 1;
            foot->fn_8015495C(0, 1, 1, 1);
            foot->fn_802E3D60();
        }
    }
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x13F), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

void Em302Kraken::fn_802DDBB0() {
    fn_80087994(m16A4.Load("Em302Kraken"), "DebugDisp", &m16A8, 0, 0);
}
