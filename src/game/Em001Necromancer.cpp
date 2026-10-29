#include "game/Em001Necromancer.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/HitInfo_unk.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "std/new.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: field object (route tables through fn_8013C504, as in gmk/FlamingArrow.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
};
extern "C" Field_unk* fn_80121F00();
extern "C" CharaManager<EnemyTask>* fn_801A03E0(); // inferred: enemy manager
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80105BD8(Effect_unk* mgr, atn::Model* model);
extern "C" float fn_8010CB2C(Effect_unk* mgr, CharaTask* chara, float rate); // inferred: chant speed
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
// inferred: constructor of chr::ControlMagicEnd (chr/ControlChant.cpp, 0x34 bytes; still in asm)
extern "C" chr::ControlBase* fn_801C02F4(void* self, CharaTask* chara);
// inferred: skill effects (as in gmk/HealPoint.cpp); the callback is empty here
struct Bullet_unk;
extern "C" void* lbl_8087F9E8;
extern "C" Bullet_unk* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                   const atn::Vector3* pos, const atn::Vector3* rot,
                                   boost::function<void(Bullet_unk*)> callback, float rate);
// inferred: game state (lbl_8087EFA8): time of a frame at 0x3A4 (as in game/Em301LShade.cpp)
struct NecroGame_unk {
    float GetTime_unk() const { return m3A4; } // inferred: accessor (as in game/CharaStatus.cpp)
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" NecroGame_unk* lbl_8087EFA8;
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: collision world and its ray test (see game/Em101ElderZesyaSecond.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
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
// inferred: camera and projection to the screen (as in game/Em208Queen.cpp)
extern "C" void* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos);
// inferred: panes of a d2anime layout (as in include/MP/D2AMpMenu_unk.h)
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct NecroMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" NecroMsgTable_unk* lbl_8087F1E4;
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop);

// inferred: d2anime task made by fn_801F3FF8 (same layout as atn::D2A_Task, as QueenD2A_unk of
// game/Em208Queen.cpp); only the members used here
struct NecroD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xFC - 0x58]; // 0x58
    unsigned int mLoop : 1;             // 0xFC
    unsigned int mVisible : 1;
    unsigned int mPlaying : 1;
    unsigned int mRepeat : 1;
    unsigned int mFC_rest : 28;
    float mFrame; // 0x100
};

// inferred: small inline helpers of other files whose weak copies are first emitted here (vf0C is over
// the inline budget and calls them out of line; they are also called by other files, which keep the
// automatic names)
extern "C" inline int fn_80244CA0(atn::Task* task) {
    return (task->mFlags >> 1) & 1;
}
extern "C" inline void fn_80244CAC(atn::Task* task) {
    task->mFlags |= 4;
}
extern "C" inline void fn_80244CBC(NecroD2A_unk* task, int repeat) {
    task->mRepeat = repeat;
}

namespace atn {
// inferred: fixed-size path (0x100 bytes) with an empty destructor; the destructor is the weak copy
// kept in atn/NandManager.cpp (0x800A2D20, named NandEmpty_unk there)
struct NandEmpty_unk {
    NandEmpty_unk(const char* str) { *this = str; }
    ~NandEmpty_unk();
    NandEmpty_unk& operator=(const char* str) {
        if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
        return *this;
    }
    const char* c_str() const { return mStr; }
    char mStr[0x100];
};
} // namespace atn

// inferred: plays an effect on the model (as in game/Em302KrakenMp.cpp; names invented)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, atn::Model* model) {
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

// inferred: same with the manager read last (name invented)
inline void PlayEffect2_unk(atn::EfpHandle_unk* efp, atn::Model* model) {
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

// inferred: flag accessor (as in gmk/ReinforcePoint.cpp; name invented)
inline unsigned int IsFlag25_unk(const CharaTask* chara) {
    return chara->mFlag25;
}
// inferred: names invented
inline void PlayMotion_unk(atn::Model* model, int motion, int loop, float speed) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = 1.0f;
    model->fn_80097C08(0, motion, loop, 0, 1, 0.0f, speed);
}
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline NecroD2A_unk* GetSlash_unk(Em001Necromancer* necro) {
    return (NecroD2A_unk*)necro->m15BC;
}
// inferred: death sequence (names invented)
inline void Die_unk(Em001Necromancer* necro) {
    necro->vf48();
    necro->fn_80178A6C();
    necro->fn_8015495C(0, 1, 1, 1);
    necro->fn_8016DA4C();
    necro->m58C = 2;
    necro->mMotion.m34C = 1;
    necro->mMotion.mTracks[0].m20 = 1.0f;
    necro->mMotion.mTracks[0].m0C = 1.0f;
    if (necro->m14B8 == 2) {
        necro->vf44();
        necro->mMotion.fn_80097C08(0, 0x33, 0, 0, 1, 0.0f, 0.1f);
    } else {
        necro->mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
        fn_800EB7A0(necro);
    }
}

// NONMATCHING: draft - registers of the second std::string constructor (as in game/Em201ZanGurg.cpp)
Em001Necromancer::Em001Necromancer(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m150C(0), m1510(0), m1514(0), m1518(0), m151C(0), m152C(1),
      m1530(0), m154C(0), m1550(0), m1554(0), m1570(0), m15A8(0), m15AC(0), m15C4(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    mStatus.m18 |= 0x140;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC005;
    mStatus.m18 |= 0x1000000;
    m14C0 = 1e16f;
    m14C4.clear();
    m14D0.clear();
    m14BC = 0;
    std::string dir("database/chara/special/");
    std::string name("db_em001_04_00.csv");
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
    m14B0.vf0C(path.c_str());
    m1578.fn_8023780C("/effect/cm002/cm002_00.efp");
    m1584.fn_8023780C("/effect/ef_sk026a.efp");
    m1590.fn_8023780C("/effect/ef_bs007a.efp");
    m121C.reset(new (1, "", "", 0) atn::EfpHandle_unk());
    m121C->fn_8023780C("/effect/ef_bu013.efp");
    (m15BC = fn_801F3FF8(this, "ui_point_slash.d2a", 0))->fn_800D246C(1);
    m1520[0] = fn_80219E6C(0x65);
    m1520[1] = fn_80219E6C(0x65);
    m1520[2] = fn_80219E6C(0x65);
}

Em001Necromancer::~Em001Necromancer() {}

// NONMATCHING: 0% - inline budget (same open case as Em301LShade::vf0C): the original calls every inline
// candidate out of line (the path ctor, operator= and c_str 0x80247674/0x802452B4/0x8024533C, the three
// push_back, PushBack_unk 0x80245310, the Task flag helpers 0x80244CA0-0x80244CBC); ours inlines them.
// With an extra inline candidate of about 60 statements the body is identical except for the names.
int Em001Necromancer::vf0C() {
    if (m15A8 == 0) {
        if (m14B0.fn_80473F50() == 0) {
            atn::NandEmpty_unk effect("/effect/ef_sk036e.efp");
            if (m14B0.fn_80470580() != 0 && fn_80121F00() != 0 && fn_80121F00()->fn_8013C504() != 0) {
                atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
                do {
                    const char* key = csv.fn_8005B9CC();
                    if (strcmp(key, "warp_count") == 0) {
                        m14BC = fn_800DC12C(csv.fn_8005B9CC());
                    } else if (strcmp(key, "warp_count_dist") == 0) {
                        m14C0 = fn_800DC288(csv.fn_8005B9CC());
                    } else if (strcmp(key, "warp_point") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                            if (place != 0) {
                                m14C4.push_back(place);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "reinforce_point") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                            if (place != 0) {
                                m14D0.push_back(place);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "reinforce_chara") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            EnemyTask* enemy = fn_801A03E0()->Find_unk(id);
                            if (enemy != 0) {
                                m14DC.push_back(enemy);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "appear_effect") == 0) {
                        effect = csv.fn_8005B9CC();
                    } else if (strcmp(key, "necro_type") == 0) {
                        m14B8 = atoi(csv.fn_8005B9CC());
                        if (m14B8 == 2) {
                            mFlag22 = 0;
                        }
                    } else if (strcmp(key, "alter") == 0) {
                        int id;
                        do {
                            id = fn_800DC12C(csv.fn_8005B9CC());
                            Em001Necromancer* alter = (Em001Necromancer*)fn_801A03E0()->Find_unk(id);
                            if (alter != 0) {
                                m14E8.PushBack_unk(alter);
                            }
                        } while (id != 0);
                    } else if (strcmp(key, "magic_skill") == 0) {
                        for (int i = 0; i < 3; i++) {
                            m1520[i] = fn_80219E6C(fn_800DC12C(csv.fn_8005B9CC()));
                        }
                    }
                } while (csv.fn_8005BBF8());
            }
            m15A8 = 1;
            m159C.fn_8023780C(effect.c_str());
        }
    } else if (m15A8 == 1) {
        if (fn_8013655C() == 0 && (m1438 == 0 || fn_80244CA0(m1438) != 0) && m1578.fn_80237874() == 0 &&
            m1590.fn_80237874() == 0 && m159C.fn_80237874() == 0 && fn_80244CA0(m15BC) != 0) {
            m15BC->fn_800D246C(0);
            fn_80244CAC(m15BC);
            fn_80244CBC((NecroD2A_unk*)m15BC, 0);
            if (m1438 != 0) {
                m1438->fn_800D246C(0);
                fn_80244CAC(m1438);
            }
            return 1;
        }
    }
    return 0;
}

// inferred: starts state `state` (names invented)
inline void Em001Necromancer::SetState_unk(int state) {
    m1514 = 0;
    m1518 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}

// inferred: number of players within `dist` (name invented, as in game/Em101ElderZesyaSecond.cpp)
inline int Em001Necromancer::CountNear_unk(float dist) {
    int count = 0;
    atn::Vector3 pos = mPos;
    PlayerTask* p = lbl_8087F8A0->mHead;
    for (; p != 0; p = p->mNext) {
        if (!p->mStatus.IsState_unk(0x20)) {
            atn::Vector3 d = p->mPos - pos;
            if (fn_805F9920(&d) < dist * dist) {
                count++;
            }
        }
    }
    return count;
}

void Em001Necromancer::vf10() {
    m1510 = mC58.mC4;
    m1518++;
    if (m1550 >= 0) {
        m1550--;
    }
    if (m58C != 10 && m58C != 11) {
        float dist = m14C0;
        int count;
        if (m1570 == 0 && m14B8 != 0) {
            count = 0;
        } else {
            count = CountNear_unk(dist);
        }
        m1554 += count;
    }
    if (mC58.mC0 == 0) {
        if (m58C != 0 && m58C != 2) {
            return;
        }
    } else if (m1510 != 0) {
        switch (m58C) {
        case 2:
            vfEC();
            break;
        case 14: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag25 = 1;
                mColli.mFlags &= ~1;
                if (m1434 > 0) {
                    if (--m1434 == 40) {
                        fn_80105BD8(lbl_8087F048, &mMotion);
                    }
                } else {
                    vf44();
                    mFlag15 = 1;
                    fn_801765D8();
                    if (m14B8 != 2) {
                        fn_800EE360(this);
                    }
                }
            }
            break;
        }
        case 7:
            fn_80246594();
            break;
        case 8: {
            float frame = mMotion.mTracks[0].m08;
            m638 = m1520[m152C];
            if ((float)__fabs(frame - 30.0f) < 1.1920929e-7f) {
                fn_8059B66C(lbl_8087F9E8, m638, this, 0, &m1534, &mRot, boost::function<void(Bullet_unk*)>(),
                            1.0f);
                if (m152C == 1) {
                    m1550 = 900;
                }
            } else if (frame >= mMotion.fn_80097D7C(0)) {
                mFB8 = 0.0f;
                SetState_unk(0);
                void* p = ::operator new(0x34, 0, "", "", 0);
                chr::ControlBase* c = (chr::ControlBase*)p;
                if (p != 0) {
                    c = fn_801C02F4(p, this);
                }
                fn_80178208(c);
            }
            break;
        }
        case 9:
            if (m1574 == 0 && mMotion.mTracks[0].m08 >= 0.0f) {
                fn_80247258();
                m1574 = 1;
            } else {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    SetState_unk(0);
                }
            }
            break;
        case 10: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                fn_80246E48();
            }
            break;
        }
        case 11: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
            }
            break;
        }
        case 12:
            m156C += lbl_8087EFA8->GetTime_unk();
            if (m156C > 0.0f) {
                mPos.y += 30.0f / mMotion.fn_80097D7C(0);
                mRot.y += 6.2831855f / mMotion.fn_80097D7C(0);
                if (m156C >= mMotion.fn_80097D7C(0)) {
                    fn_80246C30(0);
                }
            }
            break;
        case 13:
            break;
        default:
            if (m55C != 6) {
                fn_8016E970(3);
            }
            fn_80245F14();
            if (m55C == 3) {
                fn_80246244();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
}

// NONMATCHING: 98.9% - r28/r29 of the text and of the layout address swapped (same open case as
// Em208Queen::vf14)
void Em001Necromancer::vf14() {
    fn_80149A30();
    if (m15BC->IsFlag_unk(4)) {
        GetSlash_unk(this)->mFrame = 0.0f;
    }
    m15BC->mFlags |= 4;
    if (!mStatus.IsState_unk(0x20) && m15AC != 0 && m58C != 14) {
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        float z = head[2][3];
        float y = head[1][3];
        float x = head[0][3];
        atn::Vector3 pos(x, y, z);
        pos.y += 2.0f;
        atn::Vector3 screen;
        fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
        if (0.0f <= screen.z && screen.z <= 1.0f) {
            m15BC->mFlags &= ~4;
            SetPaneValue_unk(GetSlash_unk(this)->mLayout, "base_point", 0, screen.x);
            SetPaneValue_unk(GetSlash_unk(this)->mLayout, "base_point", 1, screen.y);
            const wchar_t* text = lbl_8087F1E4->Get_unk(0x2D9);
            if (text != 0) {
                SetPaneText_unk(GetSlash_unk(this)->mLayout, "string0001(1)", text);
                SetPaneText_unk(GetSlash_unk(this)->mLayout, "string0005(1)", text);
                SetPaneText_unk(GetSlash_unk(this)->mLayout, "string0006(1)", text);
            }
        }
    }
}

void Em001Necromancer::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m15AC != 0) {
        goto ignore;
    }
    switch (m58C) {
    case 9:
    case 10:
    case 11:
    case 12:
    ignore:
        msg->m90 = 0;
        msg->m84 = 0;
        msg->m68 = 0;
        msg->m8C = 0;
        return;
    case 0:
        if (m55C == 6 && (m560 == 0x17 || m560 == 0x1B)) {
            msg->m10 *= 0.0f;
        }
        break;
    }
    fn_80151448(msg);
    vf40(hit);

}

void Em001Necromancer::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C != 14 && m55C == 6 && m560 - 22U <= 1) {
        m58C = 0;
        m1514 = 0;
        m1518 = 0;
    }
    m1570 = 1;
    if (m14B8 == 2) {
        CharaTask* attacker = msg->mAttacker;
        if (attacker != 0) {
            atn::Vector3 pos;
            atn::Vector3 d = attacker->mPos - mPos;
            if (PSVECMag(&d) < 30.0f) {
                pos = mPos;
                pos.y += 8.0f;
                Effect_unk* mgr = lbl_8087F048;
                int id = fn_800F8548(mgr);
                fn_800FAB80(mgr, this, fn_80219E6C(0x69D), id, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
                Die_unk(this);
            }
        }
    } else {
        Em001Necromancer* alter;
        for (unsigned int i = 0; i < m14E8.mCount; i++) {
            alter = m14E8.mData[i];
            if (alter != 0 && !alter->mStatus.IsState_unk(0x20) && alter->mC58.mC0 != 0) {
                Die_unk(alter);
            }
        }
        if (mStatus.IsState_unk(0x20)) {
            mStatus.m04 = 1.0f;
            mStatus.fn_8013322C(0x20);
            m15AC = 1;
        }
    }
}

void Em001Necromancer::fn_80245F14() {
    float dist = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    atn::Vector3 target = m1510->mPos;
    atn::Vector3 d = target - mPos;
    PSVECMag(&d);
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 move = mRoute.m58;
        dist = PSVECMag(&move);
        if (dist > 0.0001f) {
            rot = move.GetNormalize().ToRotation_unk();
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(&rot, 1, dist, m568 * rate);
}

// inferred: nonzero when at most one "reinforce_chara" enemy is up (name invented)
inline int Em001Necromancer::NeedReinforce_unk() {
    if (m14DC.size() == 0) {
        return 0;
    }
    unsigned int alive = 0;
    for (unsigned int i = 0; i < m14DC.size(); i++) {
        EnemyTask* enemy = m14DC[i];
        if (enemy != 0 && !enemy->mStatus.IsState_unk(0x20) && enemy->mC58.mC0 != 0) {
            alive++;
        }
    }
    return alive <= 1;
}

void Em001Necromancer::fn_80246244() {
    if (m15AC != 0) {
        SetState_unk(13);
        PlayMotion_unk(&mMotion, 0x28, 1, 0.05f);
    } else if (m1554 >= m14BC && m14B8 != 2) {
        m1554 = 0;
        m1570 = 0;
        fn_80246C30(1);
    } else if (NeedReinforce_unk()) {
        SetState_unk(9);
        PlayMotion_unk(&mMotion, 0x6B, 0, 0.1f);
        m1574 = 0;
    } else if (m1510 != 0) {
        if (m1530 >= 2) {
            fn_8024675C(2);
            m1530 = 0;
        } else {
            fn_8024675C(0);
            m1530++;
        }
    }
}

int Em001Necromancer::vf68(CharaTask* attacker) {
    if (m15AC != 0 && m58C != 14) {
        return 9;
    }
    return 0;
}

void Em001Necromancer::vf6C(int kind, CharaTask* other) {
    atn::Vector3 d = mPos - other->mPos;
    d.y = 0.0f;
    d.Normalize_unk();
    if (kind == 9) {
        fn_80246914(d);
    }
}

void Em001Necromancer::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        mFlag15 = 1;
        fn_801765D8();
        if (m14B8 != 2) {
            fn_800EE360(this);
        }
    }
}

void Em001Necromancer::fn_80246594() {
    int done = 0;
    HitInfo_unk* hit = m638 = m1520[m152C];
    mFBC = (float)hit->mC0;
    if (mFB8 < (float)hit->mC0) {
        float r = fn_8010CB2C(lbl_8087F048, this, 1.0f);
        mFB8 += lbl_8087EFA8->GetTime_unk() * (r * r);
        if (mFB8 >= (float)m638->mC0) {
            done = 1;
        }
    } else {
        done = 1;
    }
    float limit = 12.0f;
    if (mPos.y - m1540.y < limit) {
        mPos.y += 0.08f;
    }
    if (done != 0) {
        SetState_unk(0);
        mFB8 = 0.0f;
        m638 = m1520[m152C];
        mF7C = m1510;
        mF6C = m1510->mPos;
        fn_801603CC(1.0f, false);
        if (m152C == 1) {
            m1550 = 900;
        }
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
    }
}

void Em001Necromancer::fn_8024675C(int index) {
    m1514 = 0;
    m1518 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 7;
    fn_8016E970(6);
    m560 = 29;
    m590 = fn_800F8548(lbl_8087F048);
    m152C = index;
    m638 = m1520[index];
    m1540 = mPos;
    PlayMotion_unk(&mMotion, 0x66, 1, 0.1f);
    mFB8 = 0.0f;
    fn_80232B7C(this, 0);
    if (m152C == 2) {
        PlayEffect2_unk(&m1584, &mMotion);
    } else {
        PlayEffect2_unk(&m1578, &mMotion);
    }
}

void Em001Necromancer::fn_80246914(atn::Vector3 dir) {
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 14;
    fn_8016E970(6);
    m560 = 0x8E;
    m1434 = 0x41;
    PlayMotion_unk(&mMotion, 0x171, 0, 0.05f);
    atn::Vector3 n = dir;
    n.Normalize_unk();
    atn::Vector3 v = dir;
    v.y *= 0.3f;
    m6B8 = v;
    m15B0 = atn::Vector3(-n.x, -n.y, -n.z).ToRotation_unk();
    fn_800EB7A0(this);
}

void Em001Necromancer::fn_80246C30(int arg) {
    SetState_unk(10);
    PlayMotion_unk(&mMotion, 0x1EA, 0, 0.1f);
    fn_80232B7C(this, 101);
    PlayEffect2_unk(&m1590, &mMotion);
    float rate = mStatus.mParamB.m84 > 0 ? mStatus.m04 / mStatus.mParamB.m84 : 0.0f;
    if (rate < 0.5f) {
        for (unsigned int i = 0; i < m14E8.mCount; i++) {
            Em001Necromancer* alter = m14E8.mData[i];
            alter->mFlag15 = 0;
            alter->fn_80176ACC();
            alter->fn_800D246C(0);
            if (IsFlag25_unk(alter)) {
                alter->mFlag25 = 0;
            }
            alter->mStatus.fn_8012D8B8();
            alter->m58C = 0;
            alter->fn_8015495C(0, 1, 1, 1);
            alter->mPos = mPos;
            alter->mRot = mRot;
            alter->fn_80145334();
            alter->mC58.mC0 = 1;
            alter->m150C = this;
            alter->fn_80246C30(0);
            alter->m1558 = -1;
        }
    }
}

void Em001Necromancer::fn_80246E48() {
    if (m150C != 0 && m1558 == -1) {
        return;
    }
    SetState_unk(11);
    PlayMotion_unk(&mMotion, 0x1EA, 0, 0.1f);
    if (m150C == 0) {
        fn_802470CC(&m1558, &m155C, &m1568);
        for (unsigned int i = 0; i < m14E8.mCount; i++) {
            Em001Necromancer* alter = m14E8.mData[i];
            alter->m1558 = (unsigned int)(alter->m1558 + 1) % m14C4.size();
            alter->m155C = m14C4[alter->m1558]->mPos;
            alter->m1568 = m14C4[alter->m1558]->m14;
        }
    }
    fn_800C344C("SE_SP014_001", &mPos, 0, -1, 1.0f);
    CollResult_unk result;
    mPos = m155C;
    atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + m155C;
    if (fn_8004ED34(lbl_8087EE98, &result, &m155C, &to, 0x80000000, &mColli, 0) != 0) {
        mPos = result.m10;
    }
    mRot.y = m1568;
    fn_80232B7C(this, 101);
    PlayEffect2_unk(&m1590, &mMotion);
}

// inferred: std::swap of two pointers (name invented)
template <class T>
inline void Swap_unk(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

int Em001Necromancer::fn_802470CC(int* index, atn::Vector3* pos, float* rot) {
    if (m14C4.size() == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < m14C4.size() - 1; i++) {
        for (unsigned int j = m14C4.size() - 1; j > i; j--) {
            atn::Vector3 a = m14C4[j]->mPos - mPos;
            float da = fn_805F9920(&a);
            unsigned int k = j - 1;
            atn::Vector3 b = m14C4[k]->mPos - mPos;
            if (da > fn_805F9920(&b)) {
                Swap_unk(m14C4[j], m14C4[k]);
            }
        }
    }
    unsigned int n = m14C4.size() > 3 ? 3 : m14C4.size();
    int k = rand() % (int)n;
    *pos = m14C4[k]->mPos;
    *rot = m14C4[k]->m14;
    *index = k;
    return 1;
}

void Em001Necromancer::fn_80247258() {
    wchar_t text[32];
    unsigned int count = 1;
    EnemyTask* first = 0;
    int down = 0;
    for (unsigned int i = 0; i < m14DC.size(); i++) {
        EnemyTask* enemy = m14DC[i];
        if (enemy != 0 && (enemy->mStatus.IsState_unk(0x20) || enemy->mC58.mC0 == 0)) {
            down++;
        }
    }
    for (int i = 0; i < down; i++) {
        EnemyTask* enemy = m14DC[i];
        if (enemy != 0 && (enemy->mStatus.IsState_unk(0x20) || enemy->mC58.mC0 == 0)) {
            atn::Vector3 pos = mPos;
            atn::Vector3 rot = mRot;
            atn::Vector3 off(0.0f, 0.0f, 14.0f);
            if (down > 1) {
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y - 1.2566371f + (float)i * (2.5132742f / (float)(down - 1)));
                PSMTXMultVec(m, &off, &off);
            }
            pos += off;
            atn::Vector3 from;
            atn::Vector3 to;
            CollResult_unk result;
            from = pos;
            from.y += 10.0f;
            to = pos;
            to.y -= 10.0f;

            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0) {
                pos = result.m04;
            }
            enemy->mFlag15 = 0;
            enemy->fn_80176ACC();
            enemy->fn_800D246C(0);
            if (IsFlag25_unk(enemy)) {
                enemy->mFlag25 = 0;
            }
            enemy->mStatus.fn_8012D8B8();
            enemy->mStatus.m224 = 0;
            enemy->m58C = 0;
            enemy->fn_8015495C(0, 1, 1, 1);
            enemy->mPos = pos;
            enemy->mRot = rot;
            enemy->fn_80145334();
            enemy->mC58.mC0 = 1;
            enemy->vf80(pos, rot);
            fn_80232B7C(0, 0);
            PlayEffect2_unk(&m159C, &enemy->mMotion);
            if (first == 0) {
                first = enemy;
            }
            count++;
        }
    }
    if (count > 1) {
        CharaInfo_unk* info = first->m60;
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x93), *(const wchar_t**)((char*)m60 + 4),
                    *(const wchar_t**)((char*)info + 4));
        fn_80109828(lbl_8087F048, text);
    }
}
