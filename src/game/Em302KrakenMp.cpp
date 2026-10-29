#include "game/Em302KrakenMp.h"
#include "atn/Memory.h"
#include "chr/ControlWallUp.h"
#include <boost/bind.hpp>
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/Em303KrakenFootMp.h"
#include "game/GimmickManager.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em302Kraken.cpp)
struct KrakenMpMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" KrakenMpMsgTable_unk* lbl_8087F1E4;
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: multiplayer session (lbl_8087F610, as in game/Em305VampireMp.cpp)
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DA48C(MpSession_unk* session, int arg);
// inferred: field object (lbl_8087F430): camera shake at 0x96C (names invented, as in game/Em301LShade.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504(); // inferred: route tables (as in gmk/FlamingArrow.cpp)
    unsigned char m000[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" Field_unk* fn_80121F00();

// inferred: route tables of the field (as in gmk/FlamingArrow.cpp)
extern "C" CharaManager<EnemyTask>* fn_801A03E0(); // inferred: enemy manager
// inferred: size and empty of a vector, out of line (weak copies kept in atn/WordWrap_unk.cpp)
struct Count4_unk;
extern "C" int fn_800E0AA8(const Count4_unk* c);
extern "C" bool fn_800E0AB0(const Count4_unk* c);
// inferred: element `i` of a vector (weak copies 0x802D8550/0x802DEE18); names invented
template <class T>
T* At_unk(std::vector<T>* v, unsigned int i) {
    return &v->mData[i];
}
// inferred: clears flags of the model (weak copy 0x802D8540 of game/Em302Kraken.cpp); name invented
inline void ClearModelFlag_unk(atn::Model* model, unsigned int flag) {
    model->mFlags &= ~flag;
}

extern "C" void fn_801092C8(Effect_unk* effect, int msg, CharaTask* chara, int value, int arg);
extern "C" void fn_80097CCC(atn::Model* model, int track, float value); // inferred: stops a track
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed (as in game/Arms.cpp)
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
// inferred: ray test against the map (as in gmk/MotionObject.cpp); layout of gmk/Lemon.h
struct KrakenMpColl_unk {
    KrakenMpColl_unk() : m34(0), m38(0), m3C(0), m40(0) {}
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
extern "C" int fn_8004ED34(void* colli, KrakenMpColl_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: gimmicks (lbl_8087F4A0, as in game/Em302Kraken.cpp); names invented
struct KrakenMpGimmick_unk {
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
    unsigned char m4C[0x54 - 0x4C];
    int m54;
    unsigned char m58[0x5C - 0x58];
    KrakenMpGimmick_unk* mNext;
};
extern "C" GimmickManager* lbl_8087F4A0;
inline KrakenMpGimmick_unk* FirstGimmick_unk() {
    return (KrakenMpGimmick_unk*)lbl_8087F4A0->mHead;
}
// inferred: message sent to the gimmicks (0x20 bytes); names invented
struct KrakenMpGimmickMsg_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    float m14;
    float m18;
    float m1C;
};
// inferred: hit parameters (as in game/CharaTask.cpp); names invented
struct HitParam_unk {
    int m00;
    float m04;
    float m08;
    int m0C;
    float m10;
    int m14;
};
extern "C" HitParam_unk lbl_807C6B90; // inferred: default parameters
// inferred: damage result of fn_8003EA3C (0x20 bytes); names invented
struct KrakenMpDamage_unk {
    KrakenMpDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlags = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags : 4;
    unsigned int m1C_0 : 28;
};
extern "C" void fn_8003EA3C(KrakenMpDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const HitParam_unk* param, int arg, int arg2);

// inferred: commands of "act_command" ({name, id}, terminated by a negative id; names invented)
struct KrakenMpCommandName_unk {
    char mName[0x40];
    int mId;
};
// inferred: static vector of the file built by the __sinit (offset of the thunder)
static atn::Vector3 sOffset_unk(0.0f, 6.0f, 0.0f);

static const KrakenMpCommandName_unk sCommands_unk[] = {
    {"swing", 8}, {"pound", 9}, {"wait", 10}, {"", -1},
};

// inferred: id of the command `name`, -1 if unknown
static int fn_802DDD1C(const char* name) {
    for (int i = 0; sCommands_unk[i].mId >= 0; i++) {
        if (strcmp(name, sCommands_unk[i].mName) == 0) {
            return sCommands_unk[i].mId;
        }
    }
    return -1;
}

// inferred: link of the character lists (CharaTask + 0x14AC, as in game/CharaTask.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: plays an effect on the model (same as in game/Em305Vampire.cpp; names invented)
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

// inferred: message to the gimmick `g` (names invented)
inline void SendGimmick_unk(KrakenMpGimmick_unk* g, int kind) {
    KrakenMpGimmickMsg_unk msg;
    msg.m00 = kind;
    msg.m04 = 0;
    msg.m08 = 0;
    msg.m0C = 0;
    msg.m10 = 0;
    msg.m14 = 0.0f;
    msg.m18 = 0.0f;
    msg.m1C = 0.0f;
    g->vf40(&msg);
}

// inferred: back to state 0 (names invented)
inline void Em302KrakenMp::Reset_unk() {
    m14EC = 0;
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
}

// inferred: plays an effect at a world position (same code in game/Em302Kraken.cpp); name invented
inline void PlayEffectAt_unk(void* owner, int id, atn::EfpFile* efp, const atn::Vector3& pos, float scale) {
    fn_80232B7C(owner, id);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, pos, lbl_807C7030, color, -1, -1, 1, scale);
}


// inferred: same as PlayEffect_unk with the manager read first; name invented
inline void PlayEffect2_unk(atn::EfpFile* efp, atn::Model* model) {
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

// NONMATCHING: draft - registers of the second std::string constructor (as in game/Em201ZanGurg.cpp)
Em302KrakenMp::Em302KrakenMp(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14DC(2), m14E0(2), m14E4(0), m14E8(0), m14EC(0), m14F0(-70.0f),
      m14F4(0), m14F8(0), m14FC(0), m1500(0x450), m151C(0), m1538(0), m153C(0), m1540(0), m1544(0), m1548(1),
      m154C(0), m1550(0), m1554(0), m1558(0), m155C(0x712), m15A0(0.0f, 0.0f, 0.0f), m15AC(0), m15B0(-1),
      m15B4(1420.0f), m15B8(20.0f), m15BC(-1720.0f), m15D8(0), m15DC(0), m15E0(0), m15E4(0), m1608(0),
      m1648(0), m164C(0), m1650(0), m165C(0) {
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
    m1630.vf0C(path.c_str());
    m1510.LoadRequest("/effect/ef_bs030c.efp");
    m1520.LoadRequest("/effect/ef_bu022.efp");
    m152C.LoadRequest("/effect/ef_bu023.efp");
    m1564.LoadRequest("/effect/ef_bs028.efp");
    m1570.LoadRequest("/effect/ef_bs029a.efp");
    m157C.LoadRequest("/effect/ef_bs029c.efp");
    m15CC.LoadRequest("/effect/ef_bd005.efp");
    m15E8.clear();
    m15F4.clear();
    m1624.clear();
    m160C.clear();
    m1618.clear();
    m1638.clear();
    m1644 = 0;
}

// NONMATCHING: draft (same reader as Em302Kraken::vf0C)
int Em302KrakenMp::vf0C() {
    if (m14D4 == 0) {
        if (fn_8013655C() == 0 && m1510.IsLoading() == 0 && m1520.IsLoading() == 0 &&
            m152C.IsLoading() == 0 && m1564.IsLoading() == 0 && m1570.IsLoading() == 0 &&
            m157C.IsLoading() == 0 && m15CC.IsLoading() == 0 && m1630.fn_80473F50() == 0) {
            if (m1630.fn_80470580() != 0 && fn_80121F00() != 0 && fn_80121F00()->fn_8013C504() != 0) {
                atn::Csv csv((const char*)m1630.fn_80470580(), m1630.fn_8047059C());
                int zero = 0;
                do {
                    const char* key = csv.fn_8005B9CC();
                    if (strcmp(key, "appear_point") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                            if (place != 0) {
                                m15E8.push_back(place);
                            }
                        } while (id != 0);
                        if (!fn_800E0AB0((const Count4_unk*)&m15E8)) {
                            m15E0 = *At_unk(&m15E8, m15E4);
                        }
                    } else if (strcmp(key, "pillar_id") == 0) {
                        int id = zero;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            if (id != 0) {
                                m15F4.push_back(id);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "foot_chara") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            Em303KrakenFootMp* foot = (Em303KrakenFootMp*)fn_801A03E0()->Find_unk(id);
                            if (id != 0 && foot != 0) {
                                m1624.push_back(foot);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "reinforce_chara") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            EnemyTask* enemy = fn_801A03E0()->Find_unk(id);
                            if (id != 0 && enemy != 0) {
                                m1618.push_back(enemy);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "reinforce_point") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                            if (id != 0 && place != 0) {
                                m160C.push_back(place);
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
                            KrakenMpCommand_unk cmd;
                            cmd.mId = fn_802DDD1C(name);
                            if (cmd.mId > 0) {
                                cmd.mValue = fn_800DC12C(csv.fn_8005B9CC());
                                m1638.push_back(cmd);
                            }
                        }
                    } else if (strcmp(key, "charge_point") == 0) {
                        RoutePlace_unk* place =
                            fn_80121F00()->fn_8013C504()->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                        if (place != 0) {
                            m15AC = place;
                        }
                    } else if (strcmp(key, "shades_skill_id") == 0) {
                        m1500 = fn_800DC12C(csv.fn_8005B9CC());
                    } else if (strcmp(key, "laser_skill_id") == 0) {
                        m155C = fn_800DC12C(csv.fn_8005B9CC());
                    } else if (strcmp(key, "pos_y_offset") == 0) {
                        m14F0 = fn_800DC288(csv.fn_8005B9CC());
                    } else if (strcmp(key, "explode_center") == 0) {
                        m15B4 = fn_800DC288(csv.fn_8005B9CC());
                        m15B8 = fn_800DC288(csv.fn_8005B9CC());
                        m15BC = fn_800DC288(csv.fn_8005B9CC());
                    } else if (strcmp(key, "anger_efp") == 0) {
                        m1588.LoadRequest(csv.fn_8005B9CC());
                    }
                } while (csv.fn_8005BBF8());
            }
            mStatus.m18 |= 0x1C0;
            mStatus.m18 |= 0x10000;
            mStatus.m18 |= 0xC21D;
            mStatus.m18 |= 0x3800000;
            for (unsigned int i = 0; i < fn_800E0AA8((const Count4_unk*)&m1624); i++) {
                (*At_unk(&m1624, i))->fn_802E5B30((int)this);
            }
            fn_802E1A0C();
            m14D4 = 1;
            ClearModelFlag_unk((atn::Model*)&mMotion, 4);
        }
    } else if (m14D4 == 1) {
        if (m1588.IsLoading() == 0) {
            m14D4 = 2;
            return 1;
        }
    } else if (m14D4 == 2) {
        return 1;
    }
    return 0;
}

// inferred: the original emits these copies right after vf0C (0x802DE80C/0x802DEB00), before vf10
template void std::vector<Em303KrakenFootMp*>::push_back(Em303KrakenFootMp* const& x);
template void std::vector<KrakenMpCommand_unk>::push_back(const KrakenMpCommand_unk& x);

// inferred: sends message 2 to every raised pillar except the one of the current point (as in
// game/Em302Kraken.cpp; names invented)
inline void BreakPillarsMp_unk(Em302KrakenMp* k) {
    unsigned int i;
    unsigned int skip = k->m15E4;
    for (i = 0; i < k->m15F4.size(); i++) {
        KrakenMpGimmick_unk* g = (KrakenMpGimmick_unk*)lbl_8087F4A0->fn_803EEE10(0x6DD6, k->m15F4[i]);
        if (g != 0) {
            int state = g->m54;
            if (i != skip && state == 1) {
                SendGimmick_unk(g, 2);
            }
        }
    }
}

// inferred: sends message 1 to the pillar of the current point and returns it (names invented)
inline KrakenMpGimmick_unk* RaisePillar_unk(Em302KrakenMp* k) {
    int index = k->m15E4;
    if (k->m15F4.size() == 0) {
        return 0;
    }
    KrakenMpGimmick_unk* g = (KrakenMpGimmick_unk*)lbl_8087F4A0->fn_803EEE10(0x6DD6, k->m15F4[index]);
    if (g != 0) {
        SendGimmick_unk(g, 1);
        return g;
    }
    return 0;
}

// NONMATCHING: 99.8% - registers of the inlined CanAct_unk (r4/r0/r3 in the original, the order of
// Em303KrakenFoot::vf40; 5 variants of the call site)
void Em302KrakenMp::vf10() {
    if (m1540 != m1544) {
        if (m1540 == 1) {
            fn_80370AE4(lbl_8087F430, 11, 1);
            m1648++;
        } else if (m1540 == 2) {
            fn_80370AE4(lbl_8087F430, 10, 1);
            m1648++;
        } else if (m1540 == 3) {
            fn_80370AE4(lbl_8087F430, 11, 1);
            m1648++;
        } else if (m1540 == 4) {
            fn_80370AE4(lbl_8087F430, 10, 1);
            m1648++;
        }
        m1544 = m1540;
    }
    if (m154C != m1550) {
        for (KrakenMpGimmick_unk* g = FirstGimmick_unk(); g != 0; g = g->mNext) {
            if (g->m48 == 0x444) {
                KrakenMpGimmickMsg_unk msg;
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
        m1550 = m154C;
    }
    if (m1554 != m1558) {
        if (m1554 == 1) {
            RaisePillar_unk(this);
        } else if (m1554 == 2) {
            BreakPillarsMp_unk(this);

        }
        m1558 = m1554;
    }
    if (!mFlag2) {
        if (mC58.mC4 == 0) {
            mC58.mC4 = lbl_8087F8A0->mHead;
        }
        if (CanAct_unk() && m58C != 0 && m14DC != 3 && vfF8() == 0) {
            vf100(0);
        }
        if (m164C == 0 && m1648 > 3) {
            m164C = 1;
            KrakenMpDamage_unk result;
            fn_8003EA3C(&result, fn_80219E6C(0x271), this, this, 0, &lbl_807C6B90, 0, 0);
        }
        vf104();
        RoutePlace_unk* place = m15E0;
        if (place != 0) {
            mPos = place->mPos;
            mRot.y = 3.1415927f + place->m14;
        } else {
            mPos = atn::Vector3(0.0f, -1000.0f, 0.0f);
        }
        mPos += m15A0;
        mPos.y += m14F0;
        if (m14DC == 3 && m58C != 7) {
            mPos.y -= 1000.0f;
        }
    } else if (m58C == 2) {
        vfEC();
    }
    mC58.mC0 = m14DC != 3;
    if (m14DC == 3) {
        m54C |= 0x2000;
    } else {
        m54C &= ~0x2000;
    }
    m14FC++;
    fn_8014C540();
    fn_80145334();
}

void Em302KrakenMp::vf14() {
    if (m151C != 0) {
        return;
    }
    fn_80149A30();
}

void Em302KrakenMp::vfFC() {
    Reset_unk();
    fn_800CB5C8(&m1560, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
}

void Em302KrakenMp::vf100(int state) {
    if (mC58.mC4 == 0) {
        return;
    }
    switch (state) {
    case 7:
        fn_802E1290();
        break;
    case 8:
        m14EC = 0;
        m14E8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 8;
        fn_8016E970(3);
        fn_800C344C("SE_EM302_013", &m614, 0, -1, 1.0f);
        break;
    case 9:
        fn_802E1414();
        break;
    case 10:
        fn_802E1624();
        break;
    case 2:
        fn_802E177C();
        break;
    default:
        Reset_unk();
        fn_800CB5C8(&m1560, 0, 0);
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        fn_80239DAC(lbl_8087F3C0, this, 101, 1);
        break;
    }
}

// inferred: update of the remote copy (multiplayer): only the states started by vf100 run here
// NONMATCHING: draft
void Em302KrakenMp::vf104() {
    if (mC58.mC0 == 0 || mC58.mC4 == 0) {
        fn_8016E970(3);
        if ((mStatus.m0C & 0x20) != 0x20 && m14DC != 1) {
            fn_802E03F0();
        }
        if (m58C == 7) {
            if (mMotion.mTracks[0].m08 <= 0.0f) {
                vf9C(1);
                m151C = 1;
                Reset_unk();
                fn_800CB5C8(&m1560, 0, 0);
                fn_80239DAC(lbl_8087F3C0, this, 100, 1);
                fn_80239DAC(lbl_8087F3C0, this, 101, 1);
            }
        } else if (m58C == 2) {
            vfEC();
        }
    } else {
        if (m15B0 > 0 && --m15B0 == 0) {
            m154C = m154C == 0;
        }
        m14E8++;
        switch (m58C) {
        case 6: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                Reset_unk();
                fn_800CB5C8(&m1560, 0, 0);
                fn_80239DAC(lbl_8087F3C0, this, 100, 1);
                fn_80239DAC(lbl_8087F3C0, this, 101, 1);
            }
            break;
        }
        case 7:
            if (mMotion.mTracks[0].m08 <= 0.0f) {
                vf9C(1);
                m151C = 1;
                Reset_unk();
                fn_800CB5C8(&m1560, 0, 0);
                fn_80239DAC(lbl_8087F3C0, this, 100, 1);
                fn_80239DAC(lbl_8087F3C0, this, 101, 1);
            }
            break;
        case 8:
            fn_802E094C();
            break;
        case 9:
            if (mMotion.mTracks[0].m00 == 0x145) {
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
            } else if (m14E8 < 20) {
                // inferred: an unused value (the original converts m14E8 to float here and pools 15.0f
                // between the constants of vf104 and vf50)
                float rate = m14E8 / 15.0f;
            } else if (m14E8 >= 45) {
                fn_802E1624();
            }
            break;
        case 10:
            fn_802E0C60();
            break;
        case 11:
            fn_802E0EC8();
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
            fn_802E03F0();
            break;
        }
        }
    }
    m14F8++;
}

void Em302KrakenMp::vf108() {}

void Em302KrakenMp::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14DC == 3) {
        msg->m68 = 0;
        msg->m88 = msg->mData->mC4;
        msg->m84 = 1;
        msg->m90 = 0;
        if (!mFlag2) {
            vf40(hit);
        } else {
            vf118(hit);
        }
        return;
    }
    msg->m50 |= 0x10;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    if (!mFlag2) {
        vf40(hit);
    } else {
        vf118(hit);
    }
}

void Em302KrakenMp::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20 && m1650 == 0) {
        fn_802E177C();
    }
}

// NONMATCHING: 99.9% - the "" of SimpleVector::reserve (lbl_8087D9E4/lbl_8087D9E0 in the original, as in
// CharaTask::vf50)
void Em302KrakenMp::vf50() {
    float d = 5.0f;
    Item14_unk item;
    item.m04.mRadius = m5B0 - d;
    MtxPtr head = mMotion.GetNodeMtx_unk("head");
    item.m04.mCenter = GetNodePos_unk(head) + m5A4;
    m624.push_back(item);
    mFlagB29 = 0;
}

void Em302KrakenMp::vf54() {
    if (m624.mSize != 0) {
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        m624.mData[0].m04.mCenter = GetNodePos_unk(head) + m5A4;
    }
}

void Em302KrakenMp::vf20() {
    m620 = m5B0 - 3.0f;
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
int Em302KrakenMp::vfB0(atn::Vector3* out) {
    if (m14DC != 1) {
        return 0;
    }
    Item14_unk* it = m624.mData;
    *out = it->m04.mCenter + atn::Vector3(0.0f, 1.0f, 0.0f) * mColli.m54 * 0.6f;
    return 1;
}

// inferred: legs not in state 12; name invented
inline int CountActiveFeet_unk(Em302KrakenMp* k) {
    int count = 0;
    for (unsigned int i = 0; i < k->m1624.size(); i++) {
        if (k->m1624[i]->m58C != 12) {
            count++;
        }
    }
    return count;
}

// inferred: picks the next attack kind (4 under 40% of the hit points, otherwise 1 and 2 by turns);
// name invented
inline void NextAttack_unk(Em302KrakenMp* k) {
    k->m1540 = k->m1548;
    if (k->mStatus.m04 / k->mStatus.mParamB.m84 < 0.4f) {
        k->m1548 = 4;
    } else {
        int next = 1;
        if (k->m1540 == 1) {
            next = 2;
        }
        k->m1548 = next;
    }
}

// inferred: changes the behaviour step m14DC (0: appears at the next point, 1: attacks, 2: sinks,
// 3: waits for the reinforcements, 4: anger); m14E4 counts the frames of the step
void Em302KrakenMp::fn_802E03F0() {
    switch (m14DC) {
    case 0:
        if (m14E4 == 0) {
            if (m15E8.size() != 0) {
                if ((int)m15E8.size() <= (int)++m15E4) {
                    m15E4 = 0;
                }
                m15E0 = m15E8[m15E4];
                RoutePlace_unk* place = m15E0;
                if (place != 0) {
                    mPos = place->mPos;
                    mRot.y = 3.1415927f + place->m14;
                } else {
                    mPos = atn::Vector3(0.0f, -1000.0f, 0.0f);
                }
                mPos += m15A0;
                mPos.y += m14F0;
                if (m14DC == 3 && m58C != 7) {
                    mPos.y -= 1000.0f;
                }
            }
            fn_802E10D8(0);
        }
        if (m14E4 == 1) {
            m14E0 = m14DC;
            m14DC = 4;
            m14E4 = 0;
            m14F8 = 0;
            return;
        }
        break;
    case 1:
        if (m153C != 0 || m14F4 >= 5) {
            m14E0 = m14DC;
            m14DC = 3;
            m14E4 = 0;
            NextAttack_unk(this);
            return;
        } else {
            int next;
            if (m14E4 > 300) {
                m14E0 = m14DC;
                m14DC = 3;
                m14E4 = 0;
                next = 0;
            } else {
                if (m14F8 > 0) {
                    m14EC = 0;
                    m14E8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    m58C = 8;
                    fn_8016E970(3);
                    fn_800C344C("SE_EM302_013", &m614, 0, -1, 1.0f);
                    m14F8 = 0;
                    m14F4++;
                }
                next = 1;
            }
            if (next == 0) {
                return;
            }
        }
        break;
    case 2:
        if (m14E4 == 0) {
            fn_802E1290();
        }
        if (m14E4 == 1) {
            NextAttack_unk(this);
            m14E0 = m14DC;
            m14DC = 3;
            m14E4 = 0;
            return;
        }
        break;
    case 3: {
        int count = CountActiveFeet_unk(this);
        if (count <= 0) {
            m15E0 = 0;
            m14E0 = m14DC;
            m14DC = 0;
            m14E4 = 0;
            return;
        }
        if (fn_802E0890() == 0) {
            return;
        }
        break;
    }
    case 4:
        if (m14E4 == 0 && m14E0 == 3) {
            fn_802E10D8(1);
        }
        if (m14E4 == 1) {
            fn_80370AE4(lbl_8087F430, 0xCC, 1);
            fn_802E1414();
        }
        if (m14E4 == 2) {
            float rate = mStatus.m04 / mStatus.mParamB.m84;
            if (rate < 0.4f) {
                m1540 = m1548;
                m1548 = 3;
            }
            m14E0 = m14DC;
            m14DC = 1;
            m14E4 = 0;
            m14F8 = 0;
            m14F4 = 0;
            m153C = 0;
            return;
        }
        break;
    }
    m14E4++;
}

// inferred: action of a character that climbed the wall next to the kraken: jumps onto the point
// above the head (see vfB0)
boost::function<void()> Em302KrakenMp::vfB4(CharaTask* chara) {
    Item14_unk* it = m624.mData;
    atn::Vector3 target = it->m04.mCenter + atn::Vector3(0.0f, 1.0f, 0.0f) * mColli.m54 * 0.6f;
    chr::ControlWallSlash* next = new (0, "", "", 0) chr::ControlWallSlash(chara, target, 0);
    return boost::bind(&CharaTask::fn_80178208, chara, next);
}

// inferred: waits for the reinforcements (returns 0 when the wait ends)
int Em302KrakenMp::fn_802E0890() {
    if (m14E4 == 0) {
        fn_802E1290();
        return 1;
    }
    int count = 0;
    for (CharaTask* c = EnemyManager::sInstance->mHead; c != 0; c = NextChara_unk(c)) {
        if (c->mType == 2 && c != this && (c->mFlags & 4) != 4) {
            count++;
        }
    }
    if (count <= 0) {
        m14E4 += 4;
    }
    if (m14E4 > 900) {
        m15E0 = 0;
        m14E0 = m14DC;
        m14DC = 0;
        m14E4 = 0;
        return 0;
    }
    return 1;
}

// inferred: state 4 (shades): marks the ground under the target at frame 15 (and fires a bullet there),
// fires the shades at frame 65 (as Em302Kraken::fn_802DA718)
// NONMATCHING: 81.8% - same open case as Em302Kraken::fn_802DA718 (frsp of x/z after the +0.3, constants of
// the effect kept in saved registers)
void Em302KrakenMp::fn_802E094C() {
    HitInfo_unk* skill = fn_80219E6C(m1500);
    if (m14E8 > 120) {
        Reset_unk();
        fn_800CB5C8(&m1560, 0, 0);
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    }
    if (m14E8 == 15) {
        m1504 = mC58.mC4->mPos;
        m1504.y += 0.1f;
        atn::Vector3 to = m1504;
        to.y -= 100.0f;
        KrakenMpColl_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &m1504, &to, 0, &mC58.mC4->mColli, 0) != 0) {
            m1504 = result.m10;
        }
        m1504.x += 0.3f;
        m1504.z += 0.3f;
        atn::Vector3 pos = m1504 + sOffset_unk;
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, fn_80219E6C(0x716), 0, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        PlayEffectAt_unk(this, 500, &m152C, m1504, 1.0f);
    }
    if (m14E8 == 30) {
        fn_80239DAC(lbl_8087F3C0, this, 500, 1);
    }
    if (m14E8 == 65) {
        atn::Vector3 pos = m1504 + sOffset_unk;
        fn_800FAB80(lbl_8087F048, this, skill, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        fn_800C344C("SE_EM301_002", &m1504, 0, -1, 1.0f);
    }
}

// inferred: state 8 (laser): shakes the camera and wakes the pillars at frame 20, fires three lasers on
// frames 100-114 (explode_center and two fixed points)
void Em302KrakenMp::fn_802E0C60() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0) && m14E8 > 180) {
        Reset_unk();
        fn_800CB5C8(&m1560, 0, 0);
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    }
    if (m14E8 == 20) {
        Field_unk* field = lbl_8087F430;
        field->m96C = field->m96C % 2;
        field->m970 = 120;
        field->m974 = 1.0f;
        field->m978 = 1.0f;
        for (KrakenMpGimmick_unk* g = FirstGimmick_unk(); g != 0; g = g->mNext) {
            if (g->m48 == 0x444) {
                KrakenMpGimmickMsg_unk msg;
                msg.m00 = 2;
                msg.m04 = 0;
                msg.m08 = 0;
                msg.m0C = 0;
                msg.m10 = 0;
                msg.m14 = 0.0f;
                msg.m18 = 0.0f;
                msg.m1C = 0.0f;
                g->vf40(&msg);
                m15B0 = 300;
            }
        }
    }
    if (m14E8 - 100U <= 14) {
        HitInfo_unk* skill = fn_80219E6C(m155C);
        fn_800FAB80(lbl_8087F048, this, skill, m590, (atn::Vector3*)&m15B4, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        atn::Vector3 pos;
        pos.Set_unk(1530.0f, 20.0f, -1760.0f);
        fn_800FAB80(lbl_8087F048, this, skill, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        pos.Set_unk(1400.0f, 20.0f, -1810.0f);
        fn_800FAB80(lbl_8087F048, this, skill, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        m1554 = 2;
    }
}

// inferred: state 10 (anger, as Em302Kraken::fn_802DBC68)
void Em302KrakenMp::fn_802E0EC8() {
    if (m14E8 == 50) {
        fn_800C344C("SE_EM302_019", &m614, 0, -1, 1.0f);
    }
    if (mMotion.mTracks[0].m00 == 0x14D) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m153C = 1;
            Reset_unk();
            fn_800CB5C8(&m1560, 0, 0);
            fn_80239DAC(lbl_8087F3C0, this, 100, 1);
            fn_80239DAC(lbl_8087F3C0, this, 101, 1);
        }
    } else if (m14E8 > 45) {
        fn_800CB5C8(&m1560, 0, 0);
        fn_80239DAC(lbl_8087F3C0, this, 100, 1);
        fn_80239DAC(lbl_8087F3C0, this, 101, 1);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    }
}

void Em302KrakenMp::vfEC() {
    m1654--;
    if (mMotion.mTracks[0].m08 <= 0.0f || m1654 < 0) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        fn_800EE360(this);
        mFlag15 = 1;
        fn_801765D8();
    }
}

// inferred: starts state 6 (rises from the water; kind 0 or 1 picks the motion)
void Em302KrakenMp::fn_802E10D8(int kind) {
    m14EC = 0;
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(3);
    m1538 = kind;
    vf98();
    m151C = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    switch (m1538) {
    case 0:
        mMotion.fn_80097C08(0, 0x2B, 0, 0, 1, 0.0f, 0.1f);
        break;
    case 1:
        mMotion.fn_80097C08(0, 0x2D, 0, 0, 1, 0.0f, 0.1f);
        break;
    }
    m15A0 = lbl_807C7030;
    PlayEffectAt_unk(0, 0, &m1520, mPos + atn::Vector3(0.0f, 70.0f, 0.0f), 1.0f);
    if (lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 0);
    }
}

// inferred: starts state 7
void Em302KrakenMp::fn_802E1290() {
    m14EC = 0;
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 7;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = -2.0f;
    mMotion.fn_80097C08(0, 0x2D, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m08 = mMotion.fn_80097D7C(0);
    fn_800C344C("SE_EM302_014", &m614, 0, -1, 1.0f);
    PlayEffectAt_unk(0, 0, &m152C, mPos + atn::Vector3(0.0f, 70.0f, 0.0f), 1.0f);
    if (m14FC >= 150 && lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 1);
    }
}

// inferred: starts state 9 (anger): effects, sound and the camp message
void Em302KrakenMp::fn_802E1414() {
    m14EC = 0;
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 9;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    m1560 = fn_800C344C("SE_EM302_011", &m614, 0, -1, 1.0f);
    fn_80232B7C(this, 100);
    PlayEffect2_unk(&m1564, (atn::Model*)&mMotion);
    fn_80232B7C(this, 101);
    PlayEffect2_unk(&m1588, (atn::Model*)&mMotion);
    m15A0 = lbl_807C7030;
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x137), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

// inferred: starts state 10
void Em302KrakenMp::fn_802E1624() {
    m14EC = 0;
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 10;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
    fn_800C344C("SE_EM302_012", &m614, 0, -1, 1.0f);
    fn_800CB5C8(&m1560, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1570, (atn::Model*)&mMotion);
    m1554 = 1;
}

// inferred: starts state 2 (dead)
void Em302KrakenMp::fn_802E177C() {
    m1650 = 1;
    m14EC = 0;
    m14E8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_8016E970(6);
    m560 = 23;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = -0.5f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m08 = mMotion.fn_80097D7C(0);
    m1654 = (int)mMotion.fn_80097D7C(0) + 30;
    for (unsigned int i = 0; i < m1624.size(); i++) {
        Em303KrakenFootMp* foot = m1624[i];
        foot->vf44();
        foot->fn_802E8084(3);
    }
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m15CC, (atn::Model*)&mMotion);
    fn_800CB5C8(&m1560, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) != 0 && mType == 2) {
        fn_801092C8(lbl_8087F048, 6, this, 0, 0);
    }
    fn_800C31F4("SE_EM302_020", 0, -1, 1.0f);
    fn_800EB7A0(this);
}

void Em302KrakenMp::fn_802E1A0C() {
    fn_80087994(m1658.Load("Em302KrakenMp"), "DebugDisp", &m165C, 0, 0);
}

void Em302KrakenMp::vf110(SpecialState_unk* state) {
    state->m00 = m58C;
    state->m04 = m14BC;
    state->m08 = m151C;
    *((signed char*)state + 0xC) = m14DC;
    *((signed char*)state + 0xD) = m1540;
    *((signed char*)state + 0xE) = m154C;
    *((signed char*)state + 0xF) = m1554;
    *(short*)&state->m0A = m15E4;
}

void Em302KrakenMp::vf114(const SpecialState_unk* state) {
    if (state->m00 == 10 && m58C != 10) {
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1570, (atn::Model*)&mMotion);
        Field_unk* field = lbl_8087F430;
        field->m96C = field->m96C % 2;
        field->m970 = 140;
        field->m974 = 1.0f;
        field->m978 = 1.0f;
    }
    if (state->m00 == 2 && m1650 == 0) {
        vf44();
        fn_802E177C();
        return;
    }
    if (state->m00 == 6 && m58C != 6 && lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 0);
    }
    if (state->m00 == 7 && m58C != 7 && m14FC >= 150 && lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 1);
    }
    m58C = state->m00;
    m14BC = state->m04;
    m151C = state->m08;
    m14DC = *((const signed char*)state + 0xC);
    m1540 = *((const signed char*)state + 0xD);
    m154C = *((const signed char*)state + 0xE);
    m1554 = *((const signed char*)state + 0xF);
    m15E4 = *(const short*)&state->m0A;
}

// inferred: the named offset of the second return takes the upper stack slot, as in the original
atn::Vector3 Em302KrakenMp::vfAC() {
    if (m624.mSize == 0) {
        return mPos + atn::Vector3(0.0f, 15.0f, 0.0f);
    }
    atn::Vector3 offset(0.0f, 15.0f, 0.0f);
    return m624.mData[0].m04.mCenter + offset;
}

Em302KrakenMp::~Em302KrakenMp() {}
