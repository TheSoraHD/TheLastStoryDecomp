#include "game/Em203GreaterDaemon.h"
#include "atn/Csv.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "game/HomingBullet.h"
#include "std/string.h"
#include "std/string_members.h"
#include "atn/DrawHelper.h"

// inferred: empty release assert (some state functions of the original are not auto-inlined)
#define ASSERT_UNK(x) ((void)0)

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: input state of the field (as in game/EnemyJill.cpp); the rumble block at 0x900
struct PlayerInput_unk {
    unsigned char m000[0x900];
    int m900;
    int m904;
    float m908;
    float m90C;
};
// inferred: point the camera looks at (lbl_8087F430 + 0x97C, as KrakenLook_unk of game/Em302Kraken.cpp)
struct DaemonLook_unk {
    void Set_unk(const atn::Vector3& pos, bool flag) {
        m01 = m00;
        m00 = 1;
        mPos = pos;
        m24 = 0.0f;
        if (flag) {
            m04 = 1;
        } else {
            m04 = 0;
        }
    }
    void SetPos_unk(const atn::Vector3& pos) { mPos = pos; }
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[2];
    int m04;
    unsigned char m08[4];
    atn::Vector3 mPos;
    unsigned char m18[0x24 - 0x18];
    float m24;
};
// inferred: field (lbl_8087F430): route tables at 0x10D8 (fn_8013C504 returns them) and a camera shake
// at 0x96C (as in game/Em301LShade.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
    unsigned char m0000[0x8A0];
    CharaTask* m8A0; // inferred: character the camera follows
    unsigned char m08A4[0x96C - 0x8A4];
    int m96C;
    int m970;   // inferred: shake frames
    float m974; // inferred: shake strength
    float m978;
    DaemonLook_unk m97C;
    unsigned char m09A4[0x10D8 - 0x9A4];
    RouteTable_unk* m10D8;
};
// inferred: input of the field (weak copy 0x80122550 of another file)
extern "C" inline PlayerInput_unk* fn_80122550(Field_unk* field) {
    return (PlayerInput_unk*)((char*)field + 0x6C);
}
// inferred: starts a rumble (weak copy 0x8028CFB8, also called by Em309Ghoul and EnemyQuarkSecond)
extern "C" inline void fn_8028CFB8(PlayerInput_unk* input, int frames, float a, float b) {
    input->m904 = frames;
    input->m908 = a;
    input->m900 = input->m900 % 2;
    input->m90C = b;
}
extern "C" Field_unk* lbl_8087F430;
// inferred: field accessor (weak copy 0x80121F00 of another file)
extern "C" inline Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
// inferred: player manager accessor (weak copy 0x8000D9E8 of another file)
extern "C" CharaManager<PlayerTask>* fn_8000D9E8();
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
extern "C" double sqrt(double x);
extern "C" int rand();
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
// inferred: frame time (lbl_8087EFA8, as FrameTime_unk in game/CharaStatus.cpp)
struct FrameTime_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" FrameTime_unk* lbl_8087EFA8;
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
// inferred: skill effect manager (class unknown, as in gmk/HealPoint.cpp)
class SkillEffect;
extern "C" void* lbl_8087F9E8;
extern "C" SkillEffect* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                    const atn::Vector3* pos, const atn::Vector3* rot,
                                    boost::function<void(SkillEffect*)> callback, float rate);
extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6); // inferred: nearest route point
// inferred: route between two points (8 bytes; declared in game/CharaStatus.h, as in gmk/MeetingSpot.cpp)
struct RoutePath_unk {
    int m00;
    int m04;
};
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: link between two points
extern "C" float fn_800F8524(const float* a, const float* b, float t); // inferred: lerp of two floats
// inferred: length in the xz plane (weak copy 0x801C3910, as in chr/ControlMoveCursor.cpp)
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}
// inferred: debug drawing (lbl_8087EEB0, as in gmk/Brazier.cpp)
// inferred: accessors of IntVector_unk (weak copies 0x800E0AA8/0x800E0AB0 of atn/WordWrap_unk.cpp)
extern "C" inline unsigned int fn_800E0AA8(const IntVector_unk* v) {
    return v->mSize;
}
extern "C" inline bool fn_800E0AB0(const IntVector_unk* v) {
    return v->mSize == 0;
}
// inferred: element i of a list of ids (weak copy 0x8028B74C, also called by Em203GreaterDaemonRed,
// EnemyQuarkSecond and GameScene)
extern "C" inline int* fn_8028B74C(IntVector_unk* v, int i) {
    return &v->mData[i];
}
// inferred: rotation matrix from euler angles, z then y then x (weak copy 0x800109E0 of
// Event/Puppeteer_CharaTask.cpp)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
// inferred: Matrix3x4 assignment out of line (weak copy 0x8008CD1C of atn/Model.cpp)
extern "C" void fn_8008CD1C(atn::Matrix3x4* dst, const atn::Matrix3x4* src);
// inferred: collision world and its ray test (as in game/Em003LesserDaemon.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: effect/bullet manager (weak copy 0x8013A194 of another file)
inline Effect_unk* fn_8013A194() {
    return lbl_8087F048;
}
// inferred: sets CharaTask::m560 (weak copy 0x801C0730 of another file)
extern "C" inline void fn_801C0730(CharaTask* chara, int value) {
    chara->m560 = value;
}
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: weak copies 0x800F7FA0 and 0x800F7FA8 of another file (as in game/EnemyQuarkSecond.cpp)
extern "C" inline EffectMgr_unk* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" inline void fn_800F7FA8(EffectMgr_unk* mgr, void* owner, int arg) {
    fn_80232B7C(owner, arg);
}
extern "C" void fn_8026607C(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, void* res, int a, int b, void* mtx, const atn::Vector3* pos,
                            const atn::Vector3* rot, const float* color, float scale, int e, int f, int g);
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em101ElderZesya.cpp)
struct DaemonMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" DaemonMsgTable_unk* lbl_8087F1E4;
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
// inferred: homing parameters with their defaults (the constants are pooled before the values set by
// the callers); name invented
struct DaemonHoming_unk : public HomingParam_unk {
    DaemonHoming_unk() {
        mTarget = 0;
        mAccel = 0.0f;
        m08 = -1.0f;
        mMaxSpeed = 10.0f;
        mTurn = 0.09424778f;
        mTime = 300.0f;
        mDelay = 5.0f;
        mNode = 0;
        mCount = -1;
    }
};
extern "C" void PSMTXIdentity(Mtx m);
// inferred: flag of the effect manager set while the hate lines start (name invented)
struct EffectMgrD0_unk {
    unsigned char m00[0xD0];
    int mD0;
};
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: moves a bullet with the character
// inferred: damage record shown by fn_80108C10 (as StatusDamage_unk of game/CharaStatus.cpp)
struct DaemonDamage_unk {
    DaemonDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
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
extern "C" void fn_80108C10(Effect_unk* effect, DaemonDamage_unk* damage, const atn::Vector3& pos, CharaTask* attacker,
                            CharaTask* chara, int b, int c);

// NONMATCHING: 99.86% - register swap on the second std::string temporary (known)
Em203GreaterDaemon::Em203GreaterDaemon(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14E4(0),
      m1500(0), m1510(0.0f), m1514(0.0f), m1518(1695.0f), m1544(0), m1548(0), m154C(0), m1558(0.0f), m15D4(0),
      m15E4(0.0f), m1628(0), m162C(0), m1640(0), m1974(0), m1990(0), m1994(0.1f), m199C(3.0f), m19A4(3), m19A8(1),
      m19AC(3), m19B0(10), m19BC(20), m19C0(5), m19C4(35), m19C8(35) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em203_00_00.csv");
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
    m1984.vf0C(path.c_str());
    m1504 = 52;
    m1508 = 51;
    m152C = 0;
    m1530 = 0;
    m151C.x = 0.0f;
    m151C.y = 0.0f;
    m151C.z = 0.0f;
    m1944.m00.LoadRequest("/effect/hateline/hate_line_06a.eff");
    m1944.m0C.LoadRequest("/effect/hateline/ef_ui003_01.eff");
    m192C.m00.LoadRequest("/effect/hateline/hate_line_06a.eff");
    m192C.m0C.LoadRequest("/effect/hateline/hate_line_06b.eff");
    m195C.m00.LoadRequest("/effect/hateline/hate_line_03a.eff");
    m195C.m0C.LoadRequest("/effect/hateline/hate_line_03b.eff");
    m15B8.LoadRequest("/effect/hateline/ef_bs014a.efp");
    m155C.LoadRequest("/effect/hateline/ef_bs012a.efp");
    m1568.LoadRequest("/effect/hateline/ef_bs012b_01.eff");
    m1574.LoadRequest("ef_ev093.efp");
    m15D8.LoadRequest("/effect/ef_mb005c.efp");
    m1634.LoadRequest("/effect/ef_bs034c.efp");
}

Em203GreaterDaemon::~Em203GreaterDaemon() {}

int Em203GreaterDaemon::vf0C() {
    if (fn_8013655C() == 0 && m1984.fn_80473F50() == 0 && m15B8.IsLoading() == 0 && m155C.IsLoading() == 0 &&
        m1568.IsLoading() == 0 && m1574.IsLoading() == 0 && fn_80288DD8(&m1944) == 0 &&
        fn_80288DD8(&m192C) == 0 && fn_80288DD8(&m195C) == 0 && m15D8.IsLoading() == 0 &&
        m1634.IsLoading() == 0) {
        mRoute.SetA0_unk(2);
        fn_80288E30(1.0f);
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        RouteTable_unk* table = fn_80121F00()->fn_8013C504();
        if (m1984.fn_80470580() != 0 && table != 0) {
            atn::Csv csv((const char*)m1984.fn_80470580(), m1984.fn_8047059C());
            int skill;
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "FlyingDist") == 0) {
                    m1518 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "FlySpeed") == 0) {
                    m1994 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "HoverSpeed") == 0) {
                    m1998 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "BulletSpeed") == 0) {
                    m199C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "LimitAngle") == 0) {
                    m19A0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "AngryAttachWaitTime") == 0) {
                    m19A4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "AbsorbTime") == 0) {
                    m19A8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "AbsorbNum") == 0) {
                    m19AC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "AbsorbValue") == 0) {
                    m19B0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "StunTime") == 0) {
                    m19B4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "StunAttackNum") == 0) {
                    m19B8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "BreathExplodeDelay") == 0) {
                    m19BC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandTime") == 0) {
                    m19C0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandAttackTime") == 0) {
                    m19C4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandBreathTime") == 0) {
                    m19C8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "Arganan") == 0) {
                    int id = fn_800DC12C(csv.fn_8005B9CC());
                    m1630 = fn_8000D9E8()->Find_unk(id);
                } else if (strcmp(key, "FlyPoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                        if (place != 0) {
                            m14F4.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "HoverPoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                        if (place != 0) {
                            m14E8.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "CenterPoint") == 0) {
                    int id = fn_800DC12C(csv.fn_8005B9CC());
                    m1500 = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
                } else if (strcmp(key, "1stStepSkill") == 0) {
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m15EC[0].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "2ndStepSkill") == 0) {
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m15EC[1].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "3rdStepSkill") == 0) {
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m15EC[2].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "4thStepSkill") == 0) {
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m15EC[3].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "5thStepSkill") == 0) {
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m15EC[4].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "LaserID") == 0) {
                    m14CC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BreathID") == 0) {
                    m14D0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "FlareID") == 0) {
                    m14D4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "AbsorbSkill") == 0) {
                    m14D8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "TackleID") == 0) {
                    m14DC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "ClawID") == 0) {
                    m14E0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SpecialBreathID") == 0) {
                    m14E4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                }
            } while (csv.fn_8005BBF8());
        }
        fn_802914F8();
        RoutePlace_unk* center = fn_80121F00()->fn_8013C504()->FindPlace_unk(1001);
        if (center != 0) {
            m1978 = center->mPos;
        } else {
            m1978.Set_unk(0.0f, 200.0f, 0.0f);
        }
        mRot.y = (m1978 - mPos).GetNormalize().ToRotation_unk().y;
        return 1;
    }
    return 0;
}

// inferred: the "vector length error" of push_back is in the string pool right after the keys of vf0C
template void std::vector<RoutePlace_unk*>::push_back(RoutePlace_unk* const& x);

// inferred: translation of a node matrix (as in game/Em208Queen.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: hit point ratio (name invented)
inline float Em203GreaterDaemon::HpRate_unk() {
    return mStatus.m04 / (float)mStatus.mParamB.m84;
}

// inferred: nonzero when the motion of track 0 has reached its end minus `margin` (name invented)
inline bool Em203GreaterDaemon::IsMotionNearEnd_unk(float margin) {
    return mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0) - margin;
}

// inferred: clears the counters of the state and takes a new bullet id (name invented)
inline void Em203GreaterDaemon::ResetState_unk() {
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m1510 = 0.0f;
    m15D4 = 0;
}

// inferred: plays a motion on track 0 (name invented)
inline void Em203GreaterDaemon::PlayMotion_unk(int motion, int loop, int arg, float speed) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
    mMotion.fn_80097C08(0, motion, loop, arg, 1, 0.0f, 0.1f);
}

// inferred: turns to the centre of the arena (name invented)
inline void Em203GreaterDaemon::FaceCenter_unk() {
    mRot.y = (m1978 - mPos).GetNormalize().ToRotation_unk().y;
}

// inferred: starts state 17 (takes off to the nearest route point; name invented)
inline void Em203GreaterDaemon::StartFly_unk() {
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m1510 = 0.0f;
    m15D4 = 0;
    m58C = 17;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 10, 0, 0, 1, 0.0f, 0.1f);
    fn_8016E970(6);
    m560 = 4;
    RouteTable_unk* table = lbl_8087F430->m10D8;
    int index = fn_803C1560(table, &mPos, fn_80179D44(), 0, 1, 1, -1.0f) - 1;
    atn::Vector3& point = table->mPoints[index].mPos;
    m151C = point - mPos;
    m1528 = atn::WrapAngle_unk((m1978 - point).ToRotationZ_unk().y - mRot.y);
    m153C = 0;
}

// NONMATCHING: 96.51% - stack slot order of the vector temporaries
void Em203GreaterDaemon::vf10() {
    fn_8028ADF4();
    if (mC58.mC0 == 0) {
        fn_8016E970(3);
    } else {
        if (m1630 != 0) {
            m1630->fn_800D246C(1);
            m1630->fn_80145334();
            m1630->mColli.mFlags &= ~1;
        }
        CharaTask* target = m14B0;
        m14B8++;
        if (target != 0) {
            switch (m58C) {
            case 7:
                fn_8028BFE0();
                break;
            case 9:
                fn_8028C834();
                break;
            case 10:
                fn_8028CB38();
                break;
            case 21:
                fn_8028F1D0();
                break;
            case 11:
                fn_8028CFE0();
                break;
            case 12:
                fn_8028D5C8();
                break;
            case 13:
                fn_8028DF34();
                break;
            case 15:
                fn_8028E1A0();
                m153C++;
                break;
            case 16:
                fn_8028E488();
                m153C++;
                break;
            case 14: {
                atn::Vector3 pos = target->mPos;
                fn_801C3910(pos - mPos);
                if ((mStatus.m0C & 4) != 4) {
                    if (HpRate_unk() < 0.4f) {
                        m14C0 = 4;
                    } else {
                        m14C0 = 0;
                    }
                    StartFly_unk();
                } else {
                    fn_8028B75C(1);
                }
                m153C++;
                break;
            }
            case 17: {
                float start = 24.999f;
                float length = 20.0f;
                if (mMotion.mTracks[0].m08 >= start && mMotion.mTracks[0].m08 < start + length) {
                    atn::Vector3 move = m151C * (1.0f / length) * lbl_8087EFA8->m3A4;
                    mPos += move;
                    mRot.y += m1528 / length * lbl_8087EFA8->m3A4;
                } else if (mMotion.mTracks[0].m08 > start + length) {
                    FaceCenter_unk();
                }
                if (mMotion.mTracks[0].m08 > mMotion.fn_80097D7C(0) - 2.0f) {
                    ResetState_unk();
                    m58C = 6;
                    fn_8016E970(3);
                    PlayMotion_unk(2, 1, 0, 1.0f);
                    fn_80239DAC(lbl_8087F3C0, this, -2, 1);
                }
                break;
            }
            case 18:
                fn_8028E800();
                break;
            case 19:
                fn_8028EDC4();
                break;
            case 20:
                fn_8028EF20();
                break;
            default: {
                int end;
                if (IsMotionNearEnd_unk(2.0f)) {
                    FaceCenter_unk();
                    end = 1;
                } else {
                    end = 0;
                }
                if (end) {
                    fn_8028B47C();
                }
                break;
            }
            }
        }
    }
    fn_8014C540();
    fn_80145334();
}

void Em203GreaterDaemon::vf14() {
    fn_80149A30();
}

// inferred: the daemon is not knocked back; the absorb skill and the hits after the 10% rage do no damage
// NONMATCHING: 97.00% - stack slots / register allocation of the damage vectors
void Em203GreaterDaemon::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m10 *= 0.0f;
    int down = (mStatus.m0C >> 2) & 1;
    if (m14C8 != 0 || msg->mData == m14D8) {
        msg->m68 = 0;
        msg->m4C = 0;
        return;
    }
    if (m58C == 18) {
        msg->m0C |= 0x80;
    }
    fn_80151448(msg);
    vf40(hit);
    if (m58C != 21 && msg->mData != 0) {
        if ((msg->mData->mAC & 0x40000) == 0x40000) {
            if (CanFall_unk()) {
                m58C = 20;
                ResetState_unk();
                fn_8016E970(6);
                m560 = 4;
                m590 = fn_800F8548(lbl_8087F048);
                PlayMotion_unk(0x37, 0, 0, 0.5f);
                m151C = atn::Vector3(0.0f, -80.0f, 0.0f);
                m14B4 = 0;
                m1528 = 0.0f;
                m1974 = 0;
                fn_80239DAC(lbl_8087F3C0, this, -2, 1);
            }
        } else if (down == 0 && (mStatus.m0C & 4) == 4) {
            ResetState_unk();
            m58C = 19;
            fn_8016E970(3);
            PlayMotion_unk(0x37, 0, 0, 1.0f);
            fn_80239DAC(lbl_8087F3C0, this, -2, 1);
            mStatus.fn_80133130(4, 600, 0);
            m14C0 = 3;
            if (fn_80370A78(lbl_8087F430, 0xE6) == 0) {
                fn_80370AE4(lbl_8087F430, 0xE6, 1);
            }
        }
    }
}

// inferred: nonzero when a hit of the "fall" kind may knock the daemon down (name invented)
inline int Em203GreaterDaemon::CanFall_unk() {
    if (m14C0 != 3) {
        return 0;
    }
    if (m58C == 13) {
        return 0;
    }
    if (m58C == 19) {
        return 0;
    }
    if (m58C == 11 && m14B4 == 0 && mMotion.mTracks[0].m08 <= 50.0f) {
        return 0;
    }
    return 1;
}

// inferred: kind of the skill, read through an int accessor (name invented)
static inline int SkillType_unk(HitInfo_unk* skill) {
    return skill->m02;
}
// NONMATCHING: 99.90% - r6/r7 crossing on the psq copy of a temporary vector (known blocker)
void Em203GreaterDaemon::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (HpRate_unk() < 0.1f && m14C8 == 0) {
        m14C8 = 1;
        if (mStatus.IsState_unk(4)) {
            mStatus.fn_8013322C(4);
            fn_80168E60();
        }
    }
    if (m58C >= 6 && m58C <= 12 && msg->mData != 0 && msg->m44 == 2) {
        HitInfo_unk* skill = fn_80219E6C(msg->mData->m6C);
        if (msg->mAttacker != 0 && skill != 0 && skill->m04 != 0 && SkillType_unk(skill) != 7) {
            msg->m94 |= 0x20000000;
            atn::Vector3 pos = mPos + (msg->mAttacker->mPos - mPos) * 0.5f;
            if (m1500 != 0) {
                atn::Vector3 dir = pos - m1500->mPos;
                if (dir.Length_unk() > 100.0f) {
                    dir.Normalize_unk();
                    dir *= 20.0f * ((float)(rand() % 1001) / 1000.0f) + 80.0f;
                    pos = m1500->mPos + dir;
                    pos.y += 100.0f;
                }
            }
            fn_8059B66C(lbl_8087F9E8, skill, msg->mAttacker, 0, &pos, &lbl_807C7030,
                        boost::function<void(SkillEffect*)>(), 1.0f);
        }
    }
}

int Em203GreaterDaemon::vfE4() {
    if (m58C == 11 && m14B4 == 0) {
        float frame = mMotion.mTracks[0].m08;
        if (220.0f <= frame && frame <= 225.0f) {
            return 1;
        }
    } else if (m58C == 9 && m14B4 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (27.0f <= frame && frame <= 33.0f) {
            return 1;
        }
    }
    return 0;
}

// inferred: damage spheres on the body (tail, head, middle legs, spine, feet)
void Em203GreaterDaemon::vf50() {
    Item14_unk item;
    item.m04.mRadius = 5.0f;
    item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("tail"))->GetTrans_unk();
    m624.push_back(item);
    item.m04.mRadius = 10.0f;
    item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    m624.push_back(item);
    item.m04.mRadius = 5.0f;
    item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightmiddle1"))->GetTrans_unk();
    m624.push_back(item);
    item.m04.mRadius = 5.0f;
    item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftmiddle1"))->GetTrans_unk();
    m624.push_back(item);
    item.m04.mRadius = 20.0f;
    item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
    m624.push_back(item);
    item.m04.mRadius = 10.0f;
    item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftfoot"))->GetTrans_unk();
    m624.push_back(item);
    item.m04.mRadius = 10.0f;
    item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightfoot"))->GetTrans_unk();
    m624.push_back(item);
    mFlagB29 = 1;
}

void Em203GreaterDaemon::vf54() {
    if (m624.mSize != 0) {
        unsigned int i = 0;
        m624[i].m04.mRadius = 25.0f;
        m624[i].m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("tail"))->GetTrans_unk();
        i++;
        m624[i].m04.mRadius = 12.0f;
        m624[i].m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
        i++;
        m624[i].m04.mRadius = 8.0f;
        m624[i].m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightmiddle1"))->GetTrans_unk();
        i++;
        m624[i].m04.mRadius = 8.0f;
        m624[i].m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftmiddle1"))->GetTrans_unk();
        i++;
        m624[i].m04.mRadius = 20.0f;
        m624[i].m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
        i++;
        m624[i].m04.mRadius = 10.0f;
        m624[i].m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftfoot"))->GetTrans_unk();
        i++;
        m624[i].m04.mRadius = 10.0f;
        m624[i].m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightfoot"))->GetTrans_unk();
    }
}

atn::Vector3 Em203GreaterDaemon::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else {
        result = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("tail"))->GetTrans_unk();
    }
    return result;
}

// inferred: point of a Catmull-Rom spline through p1 and p2 (weak copy 0x8028AC70; name invented)
// NONMATCHING: 80.46% - float register allocation of the spline terms (f28 spill)
extern "C" inline atn::Vector3 fn_8028AC70(const atn::Vector3& p0, const atn::Vector3& p1, const atn::Vector3& p2,
                                          const atn::Vector3& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;
    return (p0 * (-t3 + 2.0f * t2 - t) + p1 * (3.0f * t3 - 5.0f * t2 + 2.0f) + p2 * (-3.0f * t3 + 4.0f * t2 + t) +
            p3 * (t3 - t2)) * 0.5f;
}

// inferred: unit vector rotated around y (name invented)
static inline atn::Vector3 Front_unk(const float& angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', angle);
    PSMTXMultVec(m, &front, &front);
    return front;
}

// inferred: same rotation built with atn::Matrix3x4 (as Front_unk of game/Em203GreaterDaemonRed.cpp; name
// invented)
static inline atn::Vector3 FrontM_unk(const float& angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(angle);
    PSMTXMultVec(m.m, &front, &front);
    return front;
}

// inferred: moves along the spline of the four route points `points` (t: progress 0..1)
void Em203GreaterDaemon::fn_8028AA38(const atn::Vector3* points, float t) {
    atn::Vector3 pos = fn_8028AC70(points[0], points[1], points[2], points[3], t < 1.0f ? t : 1.0f);
    atn::Vector3 dir = pos - mPos;
    dir.Normalize_unk();
    atn::Vector3 rot = dir.ToRotation_unk();
    if (m58C == 7) {
        m1514 += (pos - mPos).Length_unk();
    }
    mPos = pos;
    atn::Vector3 a = points[1] - points[0];
    atn::Vector3 b = points[2] - points[1];
    atn::Vector3 c = points[3] - points[2];
    if (a.LengthSq_unk() < 0.0001f) {
        a = b;
    }
    if (c.LengthSq_unk() < 0.0001f) {
        c = b;
    }
    a.Normalize_unk();
    b.Normalize_unk();
    c.Normalize_unk();
    // inferred: dead value (likely a removed debug term); it places 0.8f in .sdata2 after 0.0001f
    float unused_unk = t * 0.8f;
    float ra = a.ToRotation_unk().y;
    float rb = b.ToRotation_unk().y;
    float rc = c.ToRotation_unk().y;
    float d1 = atn::WrapAngle_unk(rb - ra);
    float d2 = atn::WrapAngle_unk(rc - rb);
    fn_800F8524(&d1, &d2, t);
    if (m58C == 7) {
        mRot.y = atn::WrapAngle_unk((m1978 - mPos).ToRotation_unk().y) - 1.5707964f;
    } else {
        mRot.y = atn::WrapAngle_unk((m1978 - mPos).ToRotation_unk().y);
    }
}

// inferred: chooses the target and refreshes the hate lines when it changes
void Em203GreaterDaemon::fn_8028ADF4() {
    CharaTask* prev = m14B0;
    int prevMode = m1640;
    if (m58C == 11) {
        if (lbl_8087F8A0->mHead->mFlag28) {
            m1640 = 3;
        } else {
            m1640 = 2;
        }
        m14B0 = lbl_8087F8A0->mHead;
    } else if (m58C == 12) {
        m1640 = 1;
        m14B0 = m1630;
    } else {
        m1640 = 0;
        PlayerTask* player = lbl_8087F8A0->mHead;
        if (player->mFlag28) {
            m14B0 = player;
        } else {
            m14B0 = mC58.mC4;
        }
    }
    if (m14B0 == 0) {
        m14B0 = lbl_8087F8A0->mHead;
    }
    if (m1640 != prevMode || m14B0 != prev) {
        fn_80239DAC(lbl_8087F3C0, m1674, 0, 0);
        fn_802914F8();
        fn_8029207C();
    }
    fn_802921F8();
}

// inferred: flies along the route (moving: the spline is followed; otherwise only the passing of
// the point is tested)
// NONMATCHING: 85.57% - the original recomputes the offsets of mPaths[cur - 1]; register allocation
void Em203GreaterDaemon::fn_8028AFD8(int moving) {
    int next = 0;
    float speed;
    int cur;
    int prev;
    int p2;
    int p3;
    if (m58C == 7) {
        speed = 1.0f / m1994;
    } else {
        speed = 1.0f / m1998;
    }
    if (m58C == 7) {
        RouteTable_unk* t1 = lbl_8087F430->m10D8;
        int a = fn_803C1560(t1, &m152C->mPos, fn_80179D44(), 0, 2, 1, -1.0f);
        RouteTable_unk* t2 = lbl_8087F430->m10D8;
        int b = fn_803C1560(t2, &m1530->mPos, fn_80179D44(), 0, 2, 1, -1.0f);
        cur = m1504;
        prev = m1508;
        p2 = fn_803CD958(&lbl_8087F430->m10D8->mPaths[cur - 1], b - 1);
        if (p2 == 0) {
            p2 = fn_803CD958(&lbl_8087F430->m10D8->mPaths[cur - 1], a - 1);
        }
        m150C = p2;
        p3 = fn_803CD958(&lbl_8087F430->m10D8->mPaths[p2 - 1], b - 1);
        if (p3 == 0) {
            p3 = fn_803CD958(&lbl_8087F430->m10D8->mPaths[cur - 1], a - 1);
        }
    } else if (m58C == 8) {
        RouteTable_unk* t3 = lbl_8087F430->m10D8;
        int a = fn_803C1560(t3, &m152C->mPos, fn_80179D44(), 0, 1, 1, -1.0f);
        RouteTable_unk* t4 = lbl_8087F430->m10D8;
        int b = fn_803C1560(t4, &m1530->mPos, fn_80179D44(), 0, 1, 1, -1.0f);
        cur = m1504;
        prev = m1508;
        p2 = fn_803CD958(&lbl_8087F430->m10D8->mPaths[cur - 1], b - 1);
        if (p2 == 0) {
            p2 = a;
        }
        m150C = p2;
        p3 = fn_803CD958(&lbl_8087F430->m10D8->mPaths[p2 - 1], b - 1);
        if (p3 == 0) {
            p3 = a;
        }
    }
    atn::Vector3 points[4];
    points[0] = lbl_8087F430->m10D8->mPoints[prev - 1].mPos;
    points[1] = lbl_8087F430->m10D8->mPoints[cur - 1].mPos;
    points[2] = lbl_8087F430->m10D8->mPoints[p2 - 1].mPos;
    points[3] = lbl_8087F430->m10D8->mPoints[p3 - 1].mPos;
    if (moving) {
        (points[2] - points[1]).Length_unk();
        m1510 += speed;
        fn_8028AA38(points, m1510);
        if (m1510 >= 1.0f) {
            next = 1;
        }
    } else {
        float d0 = fn_801C3910(points[0] - mPos);
        if (fn_801C3910(points[2] - mPos) < d0) {
            next = 1;
        }
    }
    if (next) {
        m1508 = m1504;
        m1504 = p2;
        m1510 = 0.0f;
    }
    if (m1990 != 0) {
        RouteTable_unk* t5 = lbl_8087F430->m10D8;
        int point = fn_803C1560(t5, &m14B0->mPos, fn_80179D44(), 0, 1, 1, -1.0f);
        lbl_8087EEB0->fn_80063D3C(lbl_8087F430->m10D8->mPoints[point - 1].mPos, 10.0f, 10000.0f, 0xFF0000FF);
    }
}

// inferred: chooses the next action (m14C0: 0 flight, 1 hovering, 2 skills of the step, 3 rest,
// 4 grand attack)
// NONMATCHING: 77.21% - same calls; shape of the inlined fn_800E0AA8 differs
void Em203GreaterDaemon::fn_8028B47C() {
    if (m14B0 == 0) {
        return;
    }
    (atn::Vector3(*m14B0->fn_8013C38C()) - mPos).Length_unk();
    if (m14C8 != 0) {
        fn_80292C9C();
        fn_8028F560();
        return;
    }
    int state = m14C0;
    if (state == 0) {
        int found = fn_80292C9C();
        if (m152C == m1530 || found == 0) {
            m14C0 = 1;
            return;
        }
        fn_8028F560();
        if (m14C4 == 0) {
            m14C0 = 1;
            m14C4 = 1;
        } else {
            m14C0 = 2;
        }
    } else if (state == 1) {
        fn_8028FB30();
        m14C0 = 0;
    } else if (state == 3) {
        m14C0 = 0;
    } else if (state == 4) {
        fn_8029055C();
        m14C0 = 0;
    } else if (state == 2) {
        IntVector_unk* list = &m15EC[0];
        if (!fn_800E0AB0(list)) {
            switch (*fn_8028B74C(list, m1628)) {
            case 1:
                if (HpRate_unk() < 0.4f) {
                    fn_8029055C();
                } else if (fn_80291390(m14D4)) {
                    fn_8028F8E4();
                }
                break;
            case 5:
                fn_8028FC28();
                break;
            case 2:
                fn_8028FA60();
                break;
            case 3:
                if (HpRate_unk() < 0.5f) {
                    fn_8028F998();
                } else if (fn_80291390(m14D4)) {
                    fn_8028F8E4();
                }
                break;
            }
            m1628++;
            if (m1628 >= (int)fn_800E0AA8(&m15EC[m14BC])) {
                m1628 = 0;
            }
            if (m1628 == 0) {
                state = 1;
                if (fn_80370A78(fn_80121F00(), 0x9B) == 0) {
                    state = 2;
                }
            }
        }
        m14C0 = state;
    }
}

// inferred: attacks the target with the claws when it is in front and near, else takes off
void Em203GreaterDaemon::fn_8028B75C(int wait) {
    if (m14C8 != 0) {
        m14C0 = 0;
        StartFly_unk();
        return;
    }
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 spine = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
    atn::Vector3 dir = target - spine;
    dir.y = 0.0f;
    float dist = dir.Length_unk();
    if (dist < 0.00001f) {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 1.0f;
        dist = 0.1f;
    } else {
        dir.Normalize_unk();
    }
    float limit = -0.5f;
    atn::Vector3 front = Front_unk(mRot.y);
    if (PSVECDotProduct(&dir, &front) > limit) {
        if (dist <= 45.0f + m14E0->m40) {
            if (wait == 0 || m14B8 > m19C4) {
                fn_8028FDB0();
            }
        } else if ((mStatus.m0C & 4) != 4) {
            if (HpRate_unk() < 0.4f) {
                m14C0 = 4;
            } else {
                m14C0 = 0;
            }
            StartFly_unk();
        } else {
            fn_80290088();
        }
    } else {
        fn_80290088();
    }
}

// inferred: state 7 (flies around the arena along the route; m14B4: 0 take off, 1 flight, 2 landing)
// NONMATCHING: 99.20% - f0/f3 order after the motion length and stack slots
void Em203GreaterDaemon::fn_8028BFE0() {
    if (m14B4 == 0) {
        float length = mMotion.fn_80097D7C(0) - 2.0f;
        if (mMotion.mTracks[0].m08 >= length) {
            PlayMotion_unk(0x143, 1, 0, 1.0f);
            m14B4 = 1;
            mPos = m152C->mPos;
            m14B8 = 0;
        } else {
            mPos += m151C * (1.0f / length);
            mRot.y += m1528 / length;
        }
    } else if (m14B4 == 1) {
        RouteTable_unk* table = lbl_8087F430->m10D8;
        if (m150C == fn_803C1560(table, &m1530->mPos, fn_80179D44(), 0, 2, 1, -1.0f) && m1514 > m1518) {
            if (m14C8 != 0) {
                if (fn_80370A78(lbl_8087F430, 0x14) == 0) {
                    fn_80370AE4(lbl_8087F430, 0x14, 1);
                }
            } else {
                PlayMotion_unk(0x144, 0, 0, 1.0f);
                m14B4 = 2;
                m1514 = 0.0f;
                RouteTable_unk* table2 = lbl_8087F430->m10D8;
                int index = fn_803C1560(table2, &m1530->mPos, fn_80179D44(), 0, 1, 1, -1.0f) - 1;
                RoutePoint_unk& point = table2->mPoints[index];
                m151C = point.mPos - mPos;
                m1528 = atn::WrapAngle_unk((m1978 - m1530->mPos).ToRotationZ_unk().y - mRot.y);
                return;
            }
        }
        fn_8028AFD8(1);
    } else if (m14B4 == 2) {
        float length = mMotion.fn_80097D7C(0) - 2.0f;
        if (mMotion.mTracks[0].m08 >= length) {
            ResetState_unk();
            m58C = 6;
            fn_8016E970(3);
            PlayMotion_unk(2, 1, 0, 1.0f);
            fn_80239DAC(lbl_8087F3C0, this, -2, 1);
            FaceCenter_unk();
            RouteTable_unk* table = lbl_8087F430->m10D8;
            int index = fn_803C1560(table, &m1530->mPos, fn_80179D44(), 0, 1, 1, -1.0f) - 1;
            mPos = table->mPoints[index].mPos;
        } else {
            mPos += m151C * (1.0f / length);
            mRot.y += m1528 / length;
        }
    }
}

// inferred: state 9 (casts the flare at the target)
void Em203GreaterDaemon::fn_8028C834() {
    if (m14B4 == 0) {
        if (fn_802913B0() && mMotion.mTracks[0].m08 >= 28.0f) {
            fn_8016E970(6);
            m560 = 30;
            m14B4 = 1;
            PlayMotion_unk(0x145, 0, 0, 1.0f);
        }
    } else if (m14B4 == 1) {
        if (mMotion.mTracks[0].m08 >= 31.0f) {
            wchar_t text[256];
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xBD), *(const wchar_t**)((char*)m60 + 4));
            fn_80109828(lbl_8087F048, text);
            atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("chin"))->GetTrans_unk();
            atn::Vector3 dir = m14B0->mPos - pos;
            dir.GetNormalize(); // inferred: the result is not used (the original fires along dir)
            fn_800F8574(lbl_8087F048, this, m15D0, &pos, &dir, 0, 0x100, 0, 0.0f, 1.0f);
            m14B4 = 2;
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0) - 2.0f) {
        ResetState_unk();
        m58C = 6;
        fn_8016E970(3);
        PlayMotion_unk(2, 1, 0, 1.0f);
        fn_80239DAC(lbl_8087F3C0, this, -2, 1);
        }
    }
}

// inferred: rotation around x by value (weak copy 0x8028CFB0)
extern "C" inline atn::Matrix3x4 fn_8028CFB0(float rad) {
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'x', rad);
    return m;
}

static float lbl_8087DC38 = -15.0f; // inferred: start and end angles of the breath sweep (degrees)
static float lbl_8087DC3C = -70.0f;

// inferred: state 10 (breath: sweeps the flames from the neck while the motion runs)
// NONMATCHING: 0% - original is over the inline budget (calls fn_802913B0, fn_800109E0, rotX out of line); ours inlines them (0xE4C vs 0x478)
void Em203GreaterDaemon::fn_8028CB38() {
    if (m14B4 == 0) {
        if (fn_802913B0() != 0 && mMotion.GetMotionFrame(0) >= 28.0f) {
            fn_8016E970(6);
            fn_801C0730(this, 30);
            m14B4 = 1;
            mMotion.fn_80139F24(1);
            mMotion.fn_8013C554(0, 1.0f);
            mMotion.SetMotionSpeed(0, 1.0f);
            mMotion.fn_80097C08(0, 0x14F, 0, 0, 1, 0.0f, 0.1f);
            fn_80290674();
            m14B8 = 0;
            m1550 = fn_800F8548(fn_8013A194());
            m1554 = fn_800F8548(fn_8013A194());
            if (fn_80370A78(fn_80121F00(), 0x96) == 0) {
                fn_80370AE4(fn_80121F00(), 0x96, 1);
            }
            m1558 = fn_801C3910(*m14B0->fn_8013C38C() - mPos);
            fn_800F7FA8(fn_800F7FA0(), this, 1000);
            fn_8026607C(fn_800F7FA0(), &m155C, &mMotion, 1);
        }
    } else if (m14B4 == 1) {
        float length = mMotion.fn_80097D7C(0);
        if (mMotion.GetMotionFrame(0) > length) {
            fn_8028F4AC();
        }
        if (mMotion.GetMotionFrame(0) >= 75.0f && mMotion.GetMotionFrame(0) < 150.0f) {
            if (m154C == 0) {
                m154C = 1;
                m1548 = 0;
                fn_80239DAC(fn_800F7FA0(), this, 1000, 1);
                fn_800F7FA8(fn_800F7FA0(), this, 1000);
                fn_8023A680(fn_800F7FA0(), &m1568, -1, 5, &m1580, 0, 0, 0, 1.0f, 0, -1, 1);
            }
        } else if (m154C != 0) {
            m154C = 0;
            m1548 = 0;
            fn_80239DAC(fn_800F7FA0(), this, 1000, 1);
        }
        if (m154C != 0) {
            float t = (mMotion.GetMotionFrame(0) - 75.0f) / 75.0f;
            atn::Vector3 dir(0.0f, -1.0f, 0.0f);
            dir.fn_80011410(fn_8028CFB0(atn::DegToRad_unk(fn_800F8524(&lbl_8087DC38, &lbl_8087DC3C, t))));
            dir.fn_80011410(fn_800109E0(mRot));
            atn::Matrix3x4 m = fn_800109E0(dir.ToRotation_unk());
            fn_8008CD1C(&m1580, &m);
            MtxPtr neck = mMotion.GetNodeMtx_unk("chin");
            atn::Vector3 pos(neck != 0 ? ((atn::Matrix3x4*)neck)->GetTrans_unk() : mPos);
            m1580.fn_801479E4(pos);
            if (++m1548 % 3 == 0) {
                atn::Vector3 from;
                atn::Vector3 to;
                from = pos;
                to = from + dir * 1000.0f;
                CollResult_unk result;
                if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000000, 0, 0) != 0) {
                    atn::Vector3 hit(result.m04);
                    fn_800FAB80(fn_8013A194(), this, m14D0, m1550, &hit, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
                }
            }
            fn_8028CFB8(fn_80122550(fn_80121F00()), 1, 0.1f, 0.1f);
        }
    }
}

// inferred: state 11 (absorbs: m14B4 0 rises, then hovers and drains the target)
// NONMATCHING: 94.44% - stack slots of the matrix temporaries
void Em203GreaterDaemon::fn_8028CFE0() {
    if (m14B4 != 0) {
        return;
    }
    if (mMotion.mTracks[0].m08 > mMotion.fn_80097D7C(0) - 1.0f) {
        FaceCenter_unk();
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        ResetState_unk();
        m58C = 6;
        fn_8016E970(3);
        PlayMotion_unk(2, 1, 0, 1.0f);
        fn_80239DAC(lbl_8087F3C0, this, -2, 1);
        return;
    }
    if (mMotion.mTracks[0].m08 >= 10.0f && mMotion.mTracks[0].m08 < 11.0f) {
        if (fn_80370A78(lbl_8087F430, 0x97) == 0) {
            fn_80370AE4(lbl_8087F430, 0x97, 1);
        }
    }
    if (mMotion.mTracks[0].m08 >= 32.0f && mMotion.mTracks[0].m08 < 33.0f) {
        atn::Vector3 pos(0.0f, 0.0f, 30.0f);
        fn_80232B7C(this, 0);
        Color4f_unk color;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m15B8, &mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, 2.0f);
    }
    if (mMotion.mTracks[0].m08 >= 212.0f && mMotion.mTracks[0].m08 < 213.0f) {
        fn_8016E970(6);
        m560 = 30;
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        fn_80290AB4();
    }
    if (mMotion.mTracks[0].m08 <= 50.0f) {
        float rate = 400.0f / mMotion.fn_80097D7C(0);
        mRot.y += 0.06283186f;
        atn::Vector3 move = m151C * 0.02f;
        mPos += move;
        (void)rate;
    } else if (mMotion.mTracks[0].m08 >= 50.0f) {
        fn_802913B0();
    }
    if (mMotion.mTracks[0].m08 >= 20.0f && mMotion.mTracks[0].m08 <= 32.0f) {
        if (mPos.y > 30.0f) {
            mPos.y += -15.833333f;
        } else {
            mPos.y = 30.0f;
        }
    }
    if (mMotion.mTracks[0].m08 >= 40.0f && mMotion.mTracks[0].m08 <= 55.0f) {
        if (mPos.y < 220.0f) {
            mPos.y += 12.666667f;
        } else {
            mPos.y = 220.0f;
        }
    }
}

// inferred: state 12 (drains Arganan: every AbsorbTime seconds a damage is applied to him and a
// homing magic is fired from his head; m162C counts the drains)
// NONMATCHING: 76.81% - grand attack; register allocation and stack layout
void Em203GreaterDaemon::fn_8028D5C8() {
    int done = 1;
    if (m162C < m19AC) {
        done = 0;
        if (m14B8 > m19A8 * 30) {
            m14B8 = 0;
            DaemonDamage_unk damage;
            damage.m04 = m19B0;
            damage.m00 = 1;
            CharaStatusBase status(m1630->mStatus);
            status.m04 -= (float)damage.m04;
            fn_80108C10(lbl_8087F048, &damage, m1630->mPos + atn::Vector3(0.0f, 15.0f, 0.0f), this, m1630, 0, 0);
            atn::Vector3 head = ((atn::Matrix3x4*)m1630->mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
            atn::Vector3 spine = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
            atn::Vector3 dir = spine - head;
            Mtx m;
            PSMTXRotRad(m, 'z', 0.7853982f);
            PSMTXMultVec(m, &dir, &dir);
            DaemonHoming_unk param;
            param.mTarget = this;
            param.mAccel = 0.3f;
            param.mMaxSpeed = m199C;
            param.mTime = (float)m14CC->m5C;
            param.mTurn = 0.017453292f * m19A0;
            fn_800F8574(lbl_8087F048, m1630, m14D8, &head, &dir, &param, 4, 0, 0.0f, 1.0f);
            fn_80232B7C(0, 0);
            atn::Vector3 pos(0.0f, 0.0f, 0.0f);
            atn::Vector3 rot(0.0f, 0.0f, 0.0f);
            Color4f_unk color;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &m1634, &m1630->mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
            m162C++;
        }
    }
    if (mMotion.mTracks[0].m00 == 0x13F) {
        done = 0;
        if (mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0)) {
            PlayMotion_unk(2, 1, 0, 1.0f);
        }
        if (mMotion.mTracks[0].m08 > 38.0f && mMotion.mTracks[0].m08 <= 39.0f) {
            Field_unk* field = lbl_8087F430;
            field->m96C = field->m96C % 2;
            field->m970 = 10;
            field->m974 = 3.0f;
            field->m978 = 3.0f;
        }
    }
    if (done && IsMotionNearEnd_unk(2.0f)) {
        m162C = 0;
        ResetState_unk();
        m58C = 6;
        fn_8016E970(3);
        PlayMotion_unk(2, 1, 0, 1.0f);
        fn_80239DAC(lbl_8087F3C0, this, -2, 1);
    }
}

// inferred: shakes the camera (name invented)
inline void ShakeCamera_unk(int frames, float a, float b) {
    Field_unk* field = lbl_8087F430;
    field->m96C = field->m96C % 2;
    field->m970 = frames;
    field->m974 = a;
    field->m978 = b;
}

// inferred: starts state 14 (tackle; name invented)
inline void Em203GreaterDaemon::StartTackle_unk() {
    m58C = 14;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x153, 1, 0, 1.0f);
}

// inferred: state 13 (dives to the ground, then tackles)
void Em203GreaterDaemon::fn_8028DF34() {
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartTackle_unk();
        return;
    }
    if (mMotion.mTracks[0].m08 <= 42.0f) {
        float k = 0.023809524f;
        mPos += m151C * k;
    }
    if (m14DC != 0 && mMotion.mTracks[0].m08 < 41.0f && m14DC != 0 && mMotion.mTracks[0].m08 >= 40.0f) {
        ShakeCamera_unk(10, 1.0f, 2.0f);
        if (fn_80370A78(lbl_8087F430, 0xA0) == 0) {
            fn_80370AE4(lbl_8087F430, 0xA0, 1);
        }
    }
    if (m14DC != 0 && mMotion.mTracks[0].m08 < 44.0f) {
        atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk();
        fn_800FAB80(lbl_8087F048, this, m14DC, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

// inferred: state 15 (turns to the target and strikes with the claws)
void Em203GreaterDaemon::fn_8028E1A0() {
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    fn_801C3910(dir);
    PSVECNormalize(&dir, &dir);
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartTackle_unk();
        return;
    }
    if (mMotion.mTracks[0].m08 <= 20.0f) {
        mRot.y += atn::WrapAngle_unk(m1538 - m1534) / 20.0f;
    }
    if ((mMotion.mTracks[0].m08 > 36.0f && mMotion.mTracks[0].m08 < 40.0f) ||
        (mMotion.mTracks[0].m08 > 56.0f && mMotion.mTracks[0].m08 < 60.0f)) {
        atn::Vector3 spine = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
        spine += FrontM_unk(mRot.y) * 20.0f;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, m14E0, m590, 30, -1, 0.0f);
    } else {
        m590 = fn_800F8548(lbl_8087F048);
    }
}

// inferred: state 16 (walks to the target, then strikes)
void Em203GreaterDaemon::fn_8028E488() {
    ASSERT_UNK(m14B0 != 0);
    ASSERT_UNK(m14B0 != 0);
    ASSERT_UNK(m14B0 != 0);
    ASSERT_UNK(m14B0 != 0);
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    float dist = dir.Length_unk();
    dir.Normalize_unk();
    float frame = mMotion.mTracks[0].m08;
    if (frame > 21.0f && frame <= 38.0f) {
        mRot.y += atn::WrapAngle_unk(m1538 - m1534) / 17.0f;
        if (dist > 50.0f) {
            mPos += dir * 50.0f * 0.2f;
        }
    } else {
        m1538 = dir.ToRotation_unk().y;
    }
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_8028B75C(0);
    }
}

// inferred: state 18 (knocked down: falls, then stays stunned for StunTime seconds)
// NONMATCHING: 99.57% - camera look store order r4/r5 (known, as Em101LittleZesyaSecond)
void Em203GreaterDaemon::fn_8028E800() {
    if (m14B4 == 0) {
        Field_unk* field = lbl_8087F430;
        if (mMotion.mTracks[0].m08 < 24.0f) {
            atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk();
            if (m1540 == 0) {
                field->m8A0 = this;
                field->m97C.Set_unk(pos, false);
                m1540 = 1;
            } else {
                field->m8A0 = this;
                field->m97C.mPos = pos;
            }
        } else if (m1540 != 0) {
            field->m8A0 = 0;
            field->m97C.m00 = 0;
            m1540 = 0;
        }
        if (m14DC != 0 && mMotion.mTracks[0].m08 < 38.0f && m14DC != 0 && mMotion.mTracks[0].m08 >= 35.0f) {
            ShakeCamera_unk(10, 1.0f, 2.0f);
        }
        mPos += m151C * (1.0f / mMotion.fn_80097D7C(0));
        float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
            PlayMotion_unk(0x1E2, 1, 0, 1.0f);
            m14B8 = 0;
            m14B4 = 1;
        }
    } else if (m14B4 == 1 && m14B8 > m19B4 * 30) {
        m14C0 = 2;
        m1974 = 0;
        StartFly_unk();
    }
}

// inferred: state 19 (stunned by the cannon: falls and stays down)
// NONMATCHING: 98.24% - camera look store order r4/r5 (known)
void Em203GreaterDaemon::fn_8028EDC4() {
    Field_unk* field = lbl_8087F430;
    if (mMotion.mTracks[0].m08 < 20.0f) {
        atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk();
        if (m1540 == 0) {
            field->m8A0 = this;
            field->m97C.Set_unk(pos, false);
            m1540 = 1;
        } else {
            field->m8A0 = this;
            field->m97C.mPos = pos;
        }
    } else if (m1540 != 0) {
        field->m8A0 = 0;
        field->m97C.m00 = 0;
        m1540 = 0;
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m1974 = 0;
        fn_8028FC28();
    }
}

// inferred: state 20 (falls to the ground and goes back up)
// NONMATCHING: 98.73% - camera look store order r4/r5 (known)
void Em203GreaterDaemon::fn_8028EF20() {
    Field_unk* field = lbl_8087F430;
    if (mMotion.mTracks[0].m08 < 30.0f) {
        atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk();
        if (m1540 == 0) {
            field->m8A0 = this;
            field->m97C.Set_unk(pos, false);
            m1540 = 1;
        } else {
            field->m8A0 = this;
            field->m97C.mPos = pos;
        }
    } else if (m1540 != 0) {
        field->m8A0 = 0;
        field->m97C.m00 = 0;
        m1540 = 0;
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ResetState_unk();
        m58C = 6;
        fn_8016E970(3);
        PlayMotion_unk(2, 1, 0, 1.0f);
        fn_80239DAC(lbl_8087F3C0, this, -2, 1);
    }
    frame = mMotion.mTracks[0].m08;
    if (frame <= 20.0f) {
        mPos += m151C * (1.0f / (20.0f / mMotion.mTracks[0].m0C));
    } else if (20.0f < frame && frame <= 40.0f) {
        mPos -= m151C * (1.0f / (20.0f / mMotion.mTracks[0].m0C));
    }
}

// inferred: state 21 (grand attack: breathes on all the party, one after the other)
// NONMATCHING: 94.77% - register allocation
void Em203GreaterDaemon::fn_8028F1D0() {
    if (m14B4 == 0) {
        if (mMotion.mTracks[0].m08 > mMotion.fn_80097D7C(0)) {
            m14B4 = 1;
            PlayMotion_unk(0x14F, 0, 0, 1.0f);
            fn_80232B7C(this, 1000);
            atn::Vector3 pos(0.0f, 0.0f, 0.0f);
            atn::Vector3 rot(0.0f, 0.0f, 0.0f);
            Color4f_unk color;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &m155C, &mMotion, 0, rot, pos, color, -1, 1, -1, 1.0f);
        }
        return;
    }
    if (mMotion.mTracks[0].m08 > mMotion.fn_80097D7C(0)) {
        ResetState_unk();
        m58C = 6;
        fn_8016E970(3);
        PlayMotion_unk(2, 1, 0, 1.0f);
        fn_80239DAC(lbl_8087F3C0, this, -2, 1);
    }
    if (80.0f <= mMotion.mTracks[0].m08 && mMotion.mTracks[0].m08 < 150.0f) {
        mMotion.mTracks[0].m0C = 1.0f;
        if (m154C == 0) {
            m154C = 1;
            m1548 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
            m15B4 = 0.0f;
            m15B0 = lbl_8087F8A0->mHead;
        }
    } else if (m154C != 0) {
        m154C = 0;
        m1548 = 0;
    }
    if (m154C != 0) {
        if (++m1548 % 4 == 0 && m15B0 != 0) {
            fn_8029101C(m15B0, m15B4);
            m15B0 = m15B0->mNext;
            if (m15B0 == 0) {
                m15B0 = lbl_8087F8A0->mHead;
            }
            m15B4 += 1.8849558f;
        }
        ShakeCamera_unk(1, 0.1f, 0.1f);
    }
}

// inferred: back to state 6 (idle in the air)
void Em203GreaterDaemon::fn_8028F4AC() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ResetState_unk();
    m58C = 6;
    fn_8016E970(3);
    PlayMotion_unk(2, 1, 0, 1.0f);
    fn_80239DAC(lbl_8087F3C0, this, -2, 1);
}

// inferred: starts state 7 (takes off for the flight around the arena)
void Em203GreaterDaemon::fn_8028F560() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ResetState_unk();
    m58C = 7;
    fn_8016E970(3);
    PlayMotion_unk(0x142, 0, 0, 1.0f);
    RouteTable_unk* table = lbl_8087F430->m10D8;
    m1504 = fn_803C1560(table, &m152C->mPos, fn_80179D44(), 0, 2, 1, -1.0f);
    table = lbl_8087F430->m10D8;
    m1508 = fn_803C1560(table, &m152C->mPos, fn_80179D44(), 0, 2, 1, -1.0f);
    m151C = m152C->mPos - mPos;
    mRot.y = (m1978 - mPos).GetNormalize().ToRotation_unk().y;
    m1528 = -1.5707964f;
}

// inferred: starts an action of the step skills (name invented)
inline void Em203GreaterDaemon::StartAction_unk(int state) {
    m58C = state;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 29;
    m590 = fn_800F8548(lbl_8087F048);
}

// inferred: starts state 9 (flare)
void Em203GreaterDaemon::fn_8028F8E4() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 9;
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m1510 = 0.0f;
    m15D4 = 0;
    fn_8016E970(6);
    m560 = 29;
    m590 = fn_800F8548(lbl_8087F048);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x66, 1, 0, 1, 0.0f, 0.1f);
}

// inferred: starts state 10 (breath)
void Em203GreaterDaemon::fn_8028F998() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 10;
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m1510 = 0.0f;
    m15D4 = 0;
    fn_8016E970(6);
    m560 = 29;
    m590 = fn_800F8548(lbl_8087F048);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x66, 1, 0, 1, 0.0f, 0.1f);
    m15D0 = m14D0;
    if (m15D0 != 0) {
        m638 = m15D0;
    }
}

// inferred: starts state 11 (absorb)
void Em203GreaterDaemon::fn_8028FA60() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 11;
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m1510 = 0.0f;
    m15D4 = 0;
    fn_8016E970(6);
    m560 = 29;
    m590 = fn_800F8548(lbl_8087F048);
    m14B4 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 1.0f, 0.1f);
    fn_80292DB4();
    m15D0 = m14CC;
    if (m15D0 != 0) {
        m638 = m15D0;
    }
}

// inferred: starts state 12 (drains Arganan) and shows its message
void Em203GreaterDaemon::fn_8028FB30() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    if (m1630 == 0) {
        return;
    }
    m58C = 12;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x13F, 0, 0, 1.0f);
    wchar_t text[256];
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xCD), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

// inferred: starts state 13 (dives to the centre of the arena)
// NONMATCHING: 99.80% - r6/r7 crossing on the psq copy into m151C (known blocker)
void Em203GreaterDaemon::fn_8028FC28() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 13;
    ResetState_unk();
    m153C = 0;
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x15E, 0, 0, 1.0f);
    atn::Vector3 target = m1978;
    target.y = 200.0f;
    m151C = target - mPos;
    atn::Vector3 dir(m151C.x, 0.0f, m151C.z);
    dir.Normalize_unk();
    m151C -= dir * 55.0f;
}

// inferred: starts state 15 (claws)
void Em203GreaterDaemon::fn_8028FDB0() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 15;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x15F, 0, 0, 1.0f);
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    m1534 = mRot.y;
    m1538 = dir.ToRotation_unk().y;
}

// inferred: starts state 16 (turns and walks to the target)
void Em203GreaterDaemon::fn_80290088() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 16;
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m1510 = 0.0f;
    m15D4 = 0;
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    float angle = atn::WrapAngle_unk(dir.ToRotation_unk().y - mRot.y);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    if (angle < 0.0f) {
        mMotion.fn_80097C08(0, 0x158, 0, 1, 1, 0.0f, 0.1f);
    } else {
        mMotion.fn_80097C08(0, 0x159, 0, 1, 1, 0.0f, 0.1f);
    }
    m1534 = mRot.y;
    m1538 = dir.ToRotation_unk().y;
}

// inferred: starts state 21 (grand attack)
void Em203GreaterDaemon::fn_8029055C() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 21;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    m14B8 = 0;
    m14B4 = 0;
    PlayMotion_unk(0x13F, 0, 0, 0.8f);
    fn_80232B7C(0, 0);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, &m1574, 0, 0, mPos, mRot, color, -1, -1, 1, 1.0f);
}

// inferred: aims the breath: rotation from the head (10 units forward) to the target (m1534/m1538)
// NONMATCHING: 97.33% - stack slots
void Em203GreaterDaemon::fn_80290674() {
    atn::Vector3 from;
    atn::Vector3 front(0.0f, 0.0f, 10.0f);
    PSMTXMultVec(fn_800109E0(mRot).m, &front, &front);
    atn::Vector3 rot;
    from = mPos + front;
    rot = (m14B0->mPos - from).ToRotation_unk();
}

// inferred: fires the homing lasers of the absorb at every free party member, from the neck
// NONMATCHING: 89.74% - register allocation and stack slots
void Em203GreaterDaemon::fn_80290AB4() {
    if (m14CC == 0) {
        return;
    }
    int i = 0;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
        atn::Vector3 offset(0.0f, 0.0f, 30.0f);
        atn::Matrix3x4 m(mMotion.GetNodeMtx_unk("spine"));
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = 0.0f;
        PSMTXMultVec(m.m, &offset, &offset);
        pos += offset;
        atn::Vector3 dir(10.0f, 0.0f, 2.0f);
        Mtx rz;
        PSMTXRotRad(rz, 'z', 0.017453292f * (360.0f / (float)lbl_8087F8A0->mCount * (float)i));
        PSMTXMultVec(rz, &dir, &dir);
        PSMTXMultVec(fn_800109E0(mRot).m, &dir, &dir);
        DaemonHoming_unk param;
        param.mTarget = 0;
        param.mAccel = 0.0f;
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader->mFlag28) {
            if (p != leader) {
                continue;
            }
            param.mTarget = leader;
        } else {
            bool ok = false;
            bool alive = false;
            if ((p->mFlags & 4) != 4 && (p->mFlags & 1) != 1) {
                alive = true;
            }
            bool free = false;
            if (alive && (p->mStatus.m0C & 0x20) != 0x20) {
                free = true;
            }
            if (free && !p->IsGrappled_unk()) {
                ok = true;
            }
            param.mTarget = ok ? (CharaTask*)p : (CharaTask*)leader;
        }
        param.mDelay = 10.0f;
        param.mAccel = 0.6f;
        param.mMaxSpeed = m199C;
        param.mTime = (float)m14CC->m5C;
        param.mTurn = 0.017453292f * m19A0;
        fn_800F8574(lbl_8087F048, this, m14CC, &pos, &dir, &param, 0x504, 0, 0.0f, 1.0f);
        i++;
    }
}

// inferred: fires the special breath at `target` (grand attack; angle: rotation of the shot)
// NONMATCHING: 96.23% - register allocation
void Em203GreaterDaemon::fn_8029101C(PlayerTask* target, float angle) {
    if (m14E4 == 0) {
        return;
    }
    atn::Matrix3x4 m(mMotion.GetNodeMtx_unk("chin"));
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = 0.0f;
    atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("chin"))->GetTrans_unk();
    atn::Vector3 dir(2.0f, 0.0f, 2.0f);
    Mtx rz;
    PSMTXRotRad(rz, 'z', angle);
    PSMTXMultVec(rz, &dir, &dir);
    PSMTXMultVec(fn_800109E0(mRot).m, &dir, &dir);
    DaemonHoming_unk param;
    param.mTarget = target;
    param.mAccel = 0.6f;
    param.mDelay = 10.0f;
    param.mMaxSpeed = m199C;
    param.mTime = (float)m14CC->m5C;
    param.mTurn = 0.017453292f * m19A0;
    fn_800F8574(lbl_8087F048, this, m14E4, &pos, &dir, &param, 4, 0, 0.0f, 1.0f);
}

// inferred: sets the magic in use (m15D0, and the one of CharaTask when it is not null)
int Em203GreaterDaemon::fn_80291390(HitInfo_unk* skill) {
    m15D0 = skill;
    if (skill != 0) {
        m638 = skill;
        return 1;
    }
    return 0;
}

// inferred: counts the chant of the magic m15D0; nonzero when it is over
int Em203GreaterDaemon::fn_802913B0() {
    mFB8 = 0.0f;
    mFBC = 0.0f;
    if (m15D0 == 0) {
        return 1;
    }
    if (m15D4 > m15D0->mC0) {
        return 1;
    }
    m15D4++;
    mFB8 = (float)m15D4;
    mFBC = (float)m15D0->mC0;
    return 0;
}

// inferred: position above the spine (head of the hate lines)
atn::Vector3 Em203GreaterDaemon::vf58() {
    return ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk() + atn::Vector3(0.0f, 5.0f, 0.0f);
}

// inferred: clears the hate lines
void Em203GreaterDaemon::fn_802914F8() {
    for (int i = 0; i < 6; i++) {
        m1674[i].Clear_unk();

    }
}

// inferred: sets the hate line `index` from the spine of the daemon to the head of `chara` (kind:
// nonzero for the red line) and starts its effects
// NONMATCHING: 89.23% - hate line matrices; stack slots and register allocation
void Em203GreaterDaemon::fn_802915F4(CharaTask* chara, int index, int kind) {
    atn::Vector3 head = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    atn::Vector3 target = GetNodePos_unk(chara->mMotion.GetNodeMtx_unk("spine1"));
    m1674[index].m70 = chara;
    m1674[index].m00 = target - head;
    PSVECNormalize(&m1674[index].m00, &m1674[index].m00);
    m1674[index].m00 *= 30.0f;
    m1674[index].m0C.SetIdentity();
    atn::Vector3 rot = m1674[index].m00.ToRotation_unk();
    atn::Matrix3x4 r = fn_800109E0(rot);
    Mtx t;
    PSMTXConcat(m1674[index].m0C.m, r.m, t);
    __MtxCopy(m1674[index].m0C.m, t);
    m1674[index].m0C.m[0][3] = target.x;
    m1674[index].m0C.m[1][3] = target.y;
    m1674[index].m0C.m[2][3] = target.z;
    atn::Vector3 dir = target - head;
    atn::Vector3 rot2 = dir.ToRotation_unk();
    float len = dir.Length_unk();
    m1674[index].m3C.SetIdentity();
    m1674[index].m3C.m[0][3] = head.x;
    m1674[index].m3C.m[1][3] = head.y;
    m1674[index].m3C.m[2][3] = head.z;
    atn::Matrix3x4 r2 = fn_800109E0(rot2);
    Mtx t2;
    PSMTXConcat(m1674[index].m3C.m, r2.m, t2);
    __MtxCopy(m1674[index].m3C.m, t2);
    Mtx s;
    PSMTXScale(s, 1.0f, 1.0f, len);
    Mtx t3;
    PSMTXConcat(m1674[index].m3C.m, s, t3);
    __MtxCopy(m1674[index].m3C.m, t3);
    fn_80232B7C(m1674, 0);
    if (kind != 0) {
        fn_8023A680(lbl_8087F3C0, &m1944.m0C, -1, 5, &m1674[index].m0C, 0, 0, 0, 1.0f, 0, -1, 1);
    } else {
        fn_8023A680(lbl_8087F3C0, &m192C.m0C, -1, 5, &m1674[index].m0C, 0, 0, 0, 1.0f, 0, -1, 1);
    }
    fn_8023A680(lbl_8087F3C0, &m192C.m00, -1, 5, &m1674[index].m3C, 0, 0, 0, 1.0f, 0, -1, 1);
}

// inferred: draws the hate lines of the current target mode (m1640: 0 none, 1 Arganan, 2 every free
// party member, 3 the leader)
void Em203GreaterDaemon::fn_8029207C() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    if (m1640 == 0) {
        m14A8 |= 0x4000000;
    } else if (m1640 == 1 || m1640 == 3) {
        m14A8 &= ~0x4000000;
        fn_802915F4(m14B0, 0, m1640 == 3);
    } else if (m1640 == 2) {
        m14A8 &= ~0x4000000;
        int i = 0;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (p->CanAct_unk() && (p->mStatus.m0C & 0x100) != 0x100 && p != m1630) {
                fn_802915F4(p, i, 1);
                i++;
            }
        }
    }
}

// inferred: updates the hate lines (matrices from the head to each character, effects shown while
// vfE4 is set)
// NONMATCHING: 86.05% - hate line matrices; stack slots and register allocation
void Em203GreaterDaemon::fn_802921F8() {
    int arg[5];
    arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;
    arg[3] = 0;
    arg[4] = 0;
    for (unsigned int i = 0; i < 6; i++) {
        if (m1674[i].m70 == 0) {
            continue;
        }
        atn::Vector3 head = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
        atn::Vector3 target = GetNodePos_unk(m1674[i].m70->mMotion.GetNodeMtx_unk("spine1"));
        atn::Vector3 dir = target - head;
        m1674[i].m00 = dir;
        atn::Vector3 rot = dir.ToRotation_unk();
        float len = dir.Length_unk() / 50.0f;
        m1674[i].m0C.SetIdentity();
        atn::Matrix3x4 r = fn_800109E0(m1674[i].m00.ToRotation_unk());
        Mtx t;
        PSMTXConcat(m1674[i].m0C.m, r.m, t);
        __MtxCopy(m1674[i].m0C.m, t);
        m1674[i].m0C.m[0][3] = target.x;
        m1674[i].m0C.m[1][3] = target.y;
        m1674[i].m0C.m[2][3] = target.z;
        m1674[i].m3C.SetIdentity();
        m1674[i].m3C.m[0][3] = head.x;
        m1674[i].m3C.m[1][3] = head.y;
        m1674[i].m3C.m[2][3] = head.z;
        atn::Matrix3x4 r2 = fn_800109E0(rot);
        Mtx t2;
        PSMTXConcat(m1674[i].m3C.m, r2.m, t2);
        __MtxCopy(m1674[i].m3C.m, t2);
        Mtx s;
        PSMTXScale(s, 3.0f, 3.0f, len);
        Mtx t3;
        PSMTXConcat(m1674[i].m3C.m, s, t3);
        __MtxCopy(m1674[i].m3C.m, t3);
        if (vfE4()) {
            if (m1674[i].m6C == 0) {
                ((EffectMgrD0_unk*)lbl_8087F3C0)->mD0 = 1;
                fn_80232B7C(&m1674[i], 0);
                fn_8023A680(lbl_8087F3C0, &m195C.m00, -1, 5, &m1674[i].m3C, 0, 0, 0, 1.0f, arg[0], -1, 1);
                fn_8023A680(lbl_8087F3C0, &m195C.m0C, -1, 5, &m1674[i].m0C, 0, 0, 0, 2.0f, arg[1], -1, 1);
                fn_8023A680(lbl_8087F3C0, &m195C.m00, -1, 5, &m1674[i].m3C, 0, 0, 0, 1.0f, arg[2], -1, 1);
                ((EffectMgrD0_unk*)lbl_8087F3C0)->mD0 = arg[3];
            }
            m1674[i].m6C = 1;
        } else {
            m1674[i].m6C = arg[4];
        }
    }
}

// inferred: chooses the route points of the flight: the nearest FlyPoint and the one two places after it
int Em203GreaterDaemon::fn_80292C9C() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    if (m14F4.size() == 0) {
        return 0;
    }
    float best = 1.0e16f;
    int nearest = -1;
    for (unsigned int i = 0; i < m14F4.size(); i++) {
        float dist = fn_801C3910(m14F4[i]->mPos - mPos);
        if (dist < best) {
            best = dist;
            nearest = i;
        }
    }
    unsigned int next = nearest + 2;
    if (next >= m14F4.size()) {
        next -= m14F4.size() - 1;
    }
    m152C = m14F4[nearest];
    m1530 = m14F4[next];
    return 1;
}

// inferred: chooses the HoverPoint farthest from the leader and moves towards it
// NONMATCHING: 96.59% - stack slots
void Em203GreaterDaemon::fn_80292DB4() {
    if (m14E8.size() == 0) {
        return;
    }
    atn::Vector3 leader = lbl_8087F8A0->mHead->mPos;
    int far = -1;
    float best = 0.0f;
    for (unsigned int i = 0; i < m14E8.size(); i++) {
        float dist = fn_801C3910(m14E8[i]->mPos - mPos);
        if (dist > best) {
            far = i;
            best = dist;
        }
    }
    (void)leader;
    float a = atn::WrapAngle_unk((m1978 - m14E8[far]->mPos).ToRotationZ_unk().y);
    float b = atn::WrapAngle_unk((m1978 - mPos).ToRotationZ_unk().y);
    (void)a;
    (void)b;
    m1530 = m14E8[far];
    m151C = m1530->mPos - mPos;
    m151C.y = 0.0f;
}

// inferred: the four points of the claws (left and right shoulder, head, chin)
void Em203GreaterDaemon::vfCC(atn::Vector3* out, const atn::Vector3& dir) {
    out[0] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightshoulder"))->GetTrans_unk();
    out[1] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
    out[2] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftshoulder"))->GetTrans_unk();
    out[3] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
    (void)dir;
}

atn::Vector3 Em203GreaterDaemon::vfD0(float angle) {
    return ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk();
}

atn::Vector3 Em203GreaterDaemon::vfD8(CharaTask* target) {
    atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    pos.y += 5.0f;
    pos += Front_unk(mRot.y) * 7.0f;
    return pos;
}

float Em203GreaterDaemon::vfE0(const atn::Vector3& dir) {
    return 0.94247788f + mRot.y;
}
