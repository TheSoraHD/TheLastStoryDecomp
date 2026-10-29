#include "game/Em203GreaterDaemonRed.h"
#include "atn/Csv.h"
#include "atn/SoundRef.h"
#include "game/PlayerManager.h"
#include "math/Vec.h"
#include "game/HomingBullet.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" double sqrt(double x);
extern "C" double sin(double x);
// inferred: empty release assert (some functions of the original are not auto-inlined)
#define ASSERT_UNK(x) ((void)0)

extern "C" int rand();

// inferred: point the camera looks at (lbl_8087F430 + 0x97C, as in game/Em203GreaterDaemon.cpp)
struct RedLook_unk {
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
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[2];
    int m04;
    unsigned char m08[4];
    atn::Vector3 mPos;
    unsigned char m18[0x24 - 0x18];
    float m24;
};
// inferred: map of the field (fn_80373148, weak copy of game/GameTask.cpp) and its BGM
struct RedMap_unk {
    unsigned char m00[0xE0];
    int mE0; // inferred: kind of BGM
    int mE4;
    int mE8;
};
// inferred: field (lbl_8087F430): position of the camera at 0x80, the camera target at 0x8A0, a camera
// shake at 0x96C and the route tables at 0x10D8
struct Field_unk {
    RouteTable_unk* fn_8013C504();
    unsigned char m0000[0x80];
    atn::Vector3 m080; // inferred: camera position
    unsigned char m008C[0x8A0 - 0x8C];
    CharaTask* m8A0; // inferred: character the camera follows
    unsigned char m08A4[0x96C - 0x8A4];
    int m96C;
    int m970;   // inferred: shake frames
    float m974; // inferred: shake strength
    float m978;
    RedLook_unk m97C;
    unsigned char m09A4[0x10D8 - 0x9A4];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: field accessor (weak copy 0x80121F00 of another file)
extern "C" inline Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
extern "C" RedMap_unk* fn_80373148(Field_unk* field); // inferred: map of the field
extern "C" void fn_804A04AC(RedMap_unk* map, int kind, int arg2, int arg3, int arg4); // inferred: map BGM
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
extern "C" void fn_80370B78(Field_unk* field, int arg1, int arg2, int arg3, float f); // inferred: fade
extern "C" PlayerManager* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030;   // inferred: zero vector
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a homing bullet
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: moves a bullet with the character
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em101ElderZesya.cpp)
struct RedMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" RedMsgTable_unk* lbl_8087F1E4;
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
// inferred: frame time and the motion blur parameters (lbl_8087EFA8, as in atn/PostFxMotionBlur.cpp)
struct RedBlur_unk {
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
struct FrameTime_unk {
    unsigned char m000[0x240];
    RedBlur_unk m240; // inferred: motion blur
    unsigned char m264[0x3A4 - 0x264];
    float m3A4; // inferred: time of a frame
};
extern "C" FrameTime_unk* lbl_8087EFA8;
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6); // inferred: nearest route point
// inferred: skill effect manager (lbl_8087F9E8): 32 effects of 0x140 bytes at 0x48
struct RedSkill_unk {
    int m00; // inferred: state (3 = finished)
    HitInfo_unk* m04;
    struct RedSkillOwner_unk* m08;
    unsigned char m0C[0x10 - 0x0C];
    atn::Vector3 m10;
    atn::Vector3 m1C;
    unsigned char m28[0x140 - 0x28];
};
struct RedSkillOwner_unk {
    unsigned char m00[0x48];
    int m48;
};
struct RedSkillMgr_unk {
    unsigned char m00[0x48];
    RedSkill_unk mSkills[32];
};
extern "C" RedSkillMgr_unk* lbl_8087F9E8;
// inferred: collision world and its ray test (as in game/Em003LesserDaemon.cpp)
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
// inferred: weak copies of other files, called out of line
extern "C" void fn_80288E30(CharaTask* chara, float value);
extern "C" bool fn_800E0AB0(const IntVector_unk* v);
extern "C" unsigned int fn_800E0AA8(const IntVector_unk* v);
extern "C" int* fn_8028B74C(IntVector_unk* v, int i);
extern "C" float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b); // inferred: dot product
// inferred: bit 2 of the status flags (weak copy 0x8029597C); name invented
extern "C" inline int fn_8029597C(CharaStatus* status) {
    return (status->m0C >> 2) & 1;
}

// inferred: length in the xz plane (weak copy 0x801C3910, as in chr/ControlMoveCursor.cpp)
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}

// NONMATCHING: 99.85% - register swap of the second std::string temporary (known, as in Em203GreaterDaemon)
Em203GreaterDaemonRed::Em203GreaterDaemonRed(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(0), m14C4(2), m14C8(0),
      m14CC(0), m14D0(0), m151C(0), m1520(0), m1524(0), m152C(0.0f), m153C(0), m1540(0), m1558(0.0f, 0.0f, 0.0f),
      m157C(0), m1580(0), m1584(0), m15B8(0), m15E0(0), m15FC(0), m1600(0), m1604(0), m1608(0), m160C(0),
      m1638(0), m163C(0.0f, 0.0f, 0.0f), m1648(0.0f, 0.0f, 0.0f), m166C(0), m1674(3.0f), m1680(10),
      m1684(35), m1688(35), m16A4(100), m16A8(100), m16B0(1.0f), m16B4(1.0f),
      m16C0(0.2f, 0.0f, 0.0f, 1.0f) {
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
    m1660.vf0C(path.c_str());
    m1530.x = 0.0f;
    m1530.y = 0.0f;
    m1530.z = 0.0f;
    m162C.fn_8023780C("/effect/ef_mb005c.efp");
    m15E4.fn_8023780C("/effect/ef_bs049e.efp");
    for (int i = 0; i < 2; i++) {
        m14D4[i] = 0;
        m14DC[i] = 0;
        m14E4[i] = 0;
        m14EC[i] = 0;
        m14F4[i] = 0;
        m14FC[i] = 0;
    }
    mColli.mFlags &= ~0x80000000;
}

Em203GreaterDaemonRed::~Em203GreaterDaemonRed() {}

int Em203GreaterDaemonRed::vf0C() {
    if (fn_8013655C() == 0 && m1660.fn_80473F50() == 0 && m15BC.fn_80237874() == 0 && m15C8.fn_80237874() == 0 &&
        m162C.fn_80237874() == 0 && m15E4.fn_80237874() == 0) {
        fn_80288E30(this, 1.0f);
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        RouteTable_unk* table = fn_80121F00() != 0 ? fn_80121F00()->fn_8013C504() : 0;
        if (m1660.fn_80470580() != 0 && table != 0) {
            atn::Csv csv((const char*)m1660.fn_80470580(), m1660.fn_8047059C());
            float zero = 0.0f;
            int none = 0;
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "HoverSpeed") == 0) {
                    m1670 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "BulletSpeed") == 0) {
                    m1674 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "LimitAngle") == 0) {
                    m1678 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "StunTime") == 0) {
                    m167C = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandTime") == 0) {
                    m1680 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandAttackTime") == 0) {
                    m1684 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandBreathTime") == 0) {
                    m1688 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBoltAreaRadius") == 0) {
                    m168C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBoltNum") == 0) {
                    m1694 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBoltMax") == 0) {
                    m1690 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBrankTime") == 0) {
                    m1698 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderChargeTime") == 0) {
                    m169C = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderTimeLag") == 0) {
                    m16A0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "PlayerHateValueSky") == 0) {
                    m16A4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "PlayerHateValueGrand") == 0) {
                    m16A8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "PlayerHateValueAttHate") == 0) {
                    m16AC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "PlayerHateRatioSky") == 0) {
                    m16B0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "PlayerHateRatioGrand") == 0) {
                    m16B4 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "PlayerHateRatioAttHate") == 0) {
                    m16B8 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "RecoverHPRateForStun") == 0) {
                    m16BC = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "FlySkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1504[0].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "GroundSkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1504[1].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "StunHPRate") == 0) {
                    float rate = zero;
                    do {
                        rate = fn_800DC288(csv.fn_8005B9CC());
                        m1610.push_back(rate);
                    } while (zero != rate);
                } else if (strcmp(key, "RecoverHPRate") == 0) {
                    float rate = zero;
                    do {
                        rate = fn_800DC288(csv.fn_8005B9CC());
                        m161C.push_back(rate);
                    } while (zero != rate);
                } else if (strcmp(key, "HighInsideHoverPoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = table->FindPlace_unk(id);
                        if (place != 0) {
                            m1564.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "HoverDirection") == 0) {
                    int dir = none;
                    do {
                        dir = fn_800DC12C(csv.fn_8005B9CC());
                        m1570.push_back(dir);
                    } while (dir != 0);
                } else if (strcmp(key, "CenterPoint") == 0) {
                    RoutePlace_unk* place = table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                    if (place != 0) {
                        m1558 = place->mPos;
                    }
                } else if (strcmp(key, "LaserID") == 0) {
                    m14D4[0] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m14D4[1] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "TackleID") == 0) {
                    m14DC[0] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m14DC[1] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "ClawID") == 0) {
                    m14E4[0] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m14E4[1] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BreathID") == 0) {
                    m14EC[0] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m14EC[1] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "FlareID") == 0) {
                    m14F4[0] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m14F4[1] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "ThunderBoltID") == 0) {
                    m14FC[0] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    m14FC[1] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "LevelOfsBefore") == 0) {
                    m14CC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "LevelOfsAfter") == 0) {
                    m14D0 = atoi(csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        RoutePlace_unk* center = table != 0 ? table->FindPlace_unk(600) : 0;
        if (center != 0) {
            m1654 = center->mPos;
        } else {
            m1654.Set_unk(0.0f, 0.0f, 0.0f);
        }
        vfF8();
        return 1;
    }
    return 0;
}

// inferred: the "vector length error" of push_back is in the string pool right after the keys of vf0C
template void std::vector<RoutePlace_unk*>::push_back(RoutePlace_unk* const& x);

// inferred: clears the counters of the state, takes a new bullet id and stops the effects 2000/1000
inline void Em203GreaterDaemonRed::ResetState_unk() {
    m14B4 = 0;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m152C = 0.0f;
    m15B8 = 0;
    fn_80239DAC(lbl_8087F3C0, this, 2000, 1);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
}

// inferred: plays a motion on track 0
inline void Em203GreaterDaemonRed::PlayMotion_unk(int motion, int loop, int arg, float speed) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
    mMotion.fn_80097C08(0, motion, loop, arg, 1, 0.0f, 0.1f);
}

// inferred: back to state 0 (hovering)
inline void Em203GreaterDaemonRed::ToIdle_unk() {
    ResetState_unk();
    m58C = 0;
    fn_8016E970(3);
    PlayMotion_unk(2, 1, 0, 1.5f);
}

// inferred: starts state 12 (lands)
inline void Em203GreaterDaemonRed::StartState12_unk() {
    m58C = 12;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x153, 1, 0, 1.0f);
}

// inferred: keeps the height of the flight at 70
inline void Em203GreaterDaemonRed::AdjustHeight_unk() {
    float target = 70.0f;
    float d = target - mPos.y;
    if (mPos.y > target) {
        if (atn::Abs_unk(d) < 1.635f) {
            mPos.y = 70.0f;
        } else {
            mPos.y -= 1.635f;
        }
    } else if (mPos.y < target) {
        if (atn::Abs_unk(d) < 0.6666667f) {
            mPos.y = 70.0f;
        } else {
            mPos.y += 0.6666667f;
        }
    }
}

// inferred: starts state 13 (takes off to the high hover point farthest from the target)
inline void Em203GreaterDaemonRed::StartState13_unk() {
    m14C4 = 5;
    ResetState_unk();
    m58C = 13;
    PlayMotion_unk(10, 0, 0, 1.0f);
    fn_8016E970(6);
    m560 = 4;
    SelectHover_unk();
    m1530 = m1540->mPos - mPos;
    m15FC = 0;
}

// inferred: chooses the high hover point farthest from the target (route point in m1540)
inline int Em203GreaterDaemonRed::SelectHover_unk() {
    if (m1564.size() == 0) {
        return 0;
    }
    float best = 0.0f;
    int far = -1;
    for (unsigned int i = 0; i < m1564.size(); i++) {
        atn::Vector3 d = m1564[i]->mPos - m14B0->mPos;
        float dist = sqrt(d.x * d.x + d.z * d.z);
        if (dist > best) {
            best = dist;
            far = i;
        }
    }
    if (far < 0 || far >= (int)m1564.size()) {
        return 0;
    }
    fn_8029C564(1);
    RouteTable_unk* table = lbl_8087F430->m10D8;
    int index = fn_803C1560(table, &m1564[far]->mPos, fn_80179D44(), 0, 1, 1, -1.0f) - 1;
    m1540 = &table->mPoints[index];
    if (m153C == 0 || m1540 == 0 || m153C == m1540) {
        return 0;
    }
    return 1;
}

// inferred: starts state 22 (down)
inline void Em203GreaterDaemonRed::StartState22_unk() {
    m58C = 22;
    ResetState_unk();
    fn_8016E970(3);
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x1E2, 1, 0, 1.0f);
}

// inferred: counts the chant of the magic m15B4; nonzero when it is over
inline int Em203GreaterDaemonRed::Chant_unk() {
    mFB8 = 0.0f;
    mFBC = 0.0f;
    if (m15B4 == 0) {
        return 1;
    }
    if (m15B8 > m15B4->mC0) {
        return 1;
    }
    m15B8++;
    mFB8 = (float)m15B8;
    mFBC = (float)m15B4->mC0;
    return 0;
}

// NONMATCHING: 89.88% - state switch; register allocation and branch layout of the inlined helpers
void Em203GreaterDaemonRed::vf10() {
    if (fn_80370A78(lbl_8087F430, 0xE6) == 1) {
        fn_80370AE4(lbl_8087F430, 0xE6, 2);
        Field_unk* field = lbl_8087F430;
        field->m8A0 = 0;
        field->m97C.m00 = 0;
        mPos.z = m1558.z;
        mPos = m1558;
        mRot = atn::Vector3(0.0f, 0.0f, 0.0f);
        m1524 = 0;
        m14C8 = 1;
        if (m58C == 22) {
            StartState22_unk();
            mStatus.fn_8013322C(0x20);
            mStatus.m04 = 1.0f;
        } else {
            StartState12_unk();
            m14C4 = 6;
            CharaTask* player = lbl_8087F8A0->fn_8054A33C((const char*)1);
            if (player == 0) {
                player = lbl_8087F8A0->mHead;
            }
            Effect_unk* effect = lbl_8087F048;
            int id = fn_800F8548(effect);
            fn_800FAB80(effect, player, fn_80219E6C(0x6C2), id, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
    }
    m1520--;
    if (m1584 != 0 && fn_8029B704() != 0) {
        m1584 = 0;
        m1580 = 0;
    }
    m14B0 = mC58.mC4;
    if (m14B0 == 0) {
        m14B0 = lbl_8087F8A0->mHead;
    }
    m14B8++;
    if (lbl_8087F8A0 != 0) {
        PlayerTask* player = lbl_8087F8A0->fn_8054A33C((const char*)1);
        if (player != 0) {
            if ((mStatus.m0C & 0x20) == 0x20) {
                *(float*)((char*)player + 0xAD0) = 1.0f;
            } else {
                player->mStatus.fn_8012F034(0.0002f);
                *(float*)((char*)player + 0xAD0) = 3.0f;
            }
        }
    }
    if (m14C8 != 0) {
        mStatus.fn_80133B30(m14CC);
    } else {
        mStatus.fn_80133B30(m14D0);
    }
    if (mC58.mC0 == 0 || m14B0 == 0) {
        fn_8016E970(3);
    } else if (m14B0 != 0) {
        switch (m58C) {
        case 6:
        case 7:
            fn_802953C8();
            AdjustHeight_unk();
            fn_80296DB8();
            break;
        case 9:
            fn_80297474();
            break;
        case 8:
            fn_802953C8();
            fn_80297114();
            break;
        case 10:
            fn_8029795C();
            break;
        case 11:
            fn_80298580();
            break;
        case 14:
            fn_80298850();
            m15FC++;
            break;
        case 15:
            fn_80298B10();
            m15FC++;
            break;
        case 16:
            fn_80298E98();
            m15FC++;
            break;
        case 12:
            if ((mStatus.m0C & 4) != 4 && m14C8 == 0) {
                StartState13_unk();
            } else {
                fn_80296B8C(1);
            }
            m15FC++;
            break;
        case 13:
            fn_80299134();
            break;
        case 17:
            fn_802992F8();
            break;
        case 19:
            fn_802995D4();
            break;
        case 20:
            fn_802997B8();
            break;
        case 21:
            fn_80299A7C();
            break;
        case 2:
            vfEC();
            break;
        case 23:
            fn_8029A0A8();
            break;
        default:
            fn_802953C8();
            AdjustHeight_unk();
            if (m1520 <= 0) {
                fn_80296268();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    vfF8();
    RedMap_unk* map = lbl_8087F430 != 0 ? fn_80373148(lbl_8087F430) : 0;
    if (map != 0) {
        int kind;
        if (atn::Abs_unk(m14B0->mPos.y - mPos.y) < 20.0f) {
            kind = 1;
        } else {
            kind = 2;
        }
        if (kind != map->mE0) {
            fn_804A04AC(map, kind, 15, -1, 0);
            map->mE8 = kind;
        }
    }
}

// inferred: the wing parts hidden after the level up (vf14)
static const char* const lbl_807457C0[12] = {
    "op_backwing_L1",   "op_backwing_L2",   "op_backwing_R1",   "op_backwing_R2",
    "op_frontwing_L1",  "op_frontwing_L2",  "op_frontwing_R1",  "op_frontwing_R2",
    "op_middlewing_L1", "op_middlewing_L2", "op_middlewing_R1", "op_middlewing_R2",
};

void Em203GreaterDaemonRed::vf14() {
    if (m14C8 != 0) {
        mMotion.fn_80092954("em203_bodylight")->mColor = m16C0.ToGX();
        mMotion.fn_80092954("em203_wing1light")->mColor = m16C0.ToGX();
        mMotion.fn_80092954("em203_wing2light")->mColor = m16C0.ToGX();
    } else {
        DaemonColor_unk c(0.5f, 0.5f, 0.5f, 1.0f);
        mMotion.fn_80092954("em203_bodylight")->mColor = c.ToGX();
        mMotion.fn_80092954("em203_wing1light")->mColor = c.ToGX();
        mMotion.fn_80092954("em203_wing2light")->mColor = c.ToGX();
    }
    for (int i = 0; i < 12; i++) {
        mMotion.fn_8009373C(lbl_807457C0[i], m14C8 == 0);
    }
    if (m14C8 != 0) {
        vf9C(1);
    }
    fn_80149A30();
}

// inferred: turns towards the target (yaw limited to 3 degrees a frame)
void Em203GreaterDaemonRed::fn_802953C8() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    atn::Vector3 d(m14B0->mPos.x - mPos.x, 0.0f, m14B0->mPos.z - mPos.z);
    if (d.IsZero_unk()) {
        mRot = lbl_807C7030;
        return;
    }
    float yaw = d.ToRotation_unk().y;
    float diff = atn::WrapAngle_unk(yaw - mRot.y);
    if ((diff < 0.0f ? -diff : diff) <= 0.05235988f) {
        mRot.y = yaw;
    } else if (diff <= 0.0f) {
        mRot.y -= 0.05235988f;
    } else {
        mRot.y += 0.05235988f;
    }
}

void Em203GreaterDaemonRed::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 22 || m58C == 2 || m58C == 23) {
        return;
    }
    bool down = mStatus.IsState_unk(4);
    msg->m10 = lbl_807C7030;
    msg->m50 |= 0x10;
    fn_80151448(msg);
    vf40(hit);
    if (msg->mData == 0) {
        return;
    }
    if (m14C8 != 0) {
        return;
    }
    if ((msg->mData->mAC & 0x40000) == 0x40000) {
        if (m14C4 == 6) {
            return;
        }
        m58C = 20;
        ResetState_unk();
        fn_8016E970(6);
        m560 = 4;
        m590 = fn_800F8548(lbl_8087F048);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 0.5f;
        mMotion.fn_80097C08(0, 0x37, 0, 0, 1, 0.0f, 0.1f);
        m1530 = atn::Vector3(0.0f, -80.0f, 0.0f);
        fn_80239DAC(lbl_8087F3C0, this, -2, 1);
    } else if (down == 0 && (mStatus.m0C & 4) == 4) {
        ResetState_unk();
        m58C = 19;
        fn_8016E970(3);
        PlayMotion_unk(0x37, 0, 0, 1.0f);
        mStatus.fn_80133130(4, 600, 0);
        m14C4 = 6;
    }
}

void Em203GreaterDaemonRed::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (m58C != 22 && m58C != 2) {
            fn_8029B59C();
        }
        mStatus.fn_8013322C(0x20);
        mStatus.m04 = 1.0f;
    }
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 77.86% - stack slots and scheduling of the capsule vectors
void Em203GreaterDaemonRed::vfF8() {
    m614 = mPos + m5A4;
    m620 = m5B0;
    m614.y += m5B0;
    atn::Vector3 right = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightfoot"))->GetTrans_unk();
    atn::Vector3 left = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftfoot"))->GetTrans_unk();
    atn::Vector3 center(atn::Vector3(left + right) * 0.5f);
    atn::Vector3 head(((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk());
    mColli.m3C = center;
    mColli.m48 = head;
    mColli.m54 = m620;
}

// inferred: damage spheres on the body
void Em203GreaterDaemonRed::vf50() {
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

void Em203GreaterDaemonRed::vf54() {
    if (m624.mSize != 0) {
        unsigned int i = 0;
        m624[i].m04.mRadius = 25.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail"));
        i++;
        m624[i].m04.mRadius = 12.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
        i++;
        m624[i].m04.mRadius = 8.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightmiddle1"));
        i++;
        m624[i].m04.mRadius = 8.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftmiddle1"));
        i++;
        m624[i].m04.mRadius = 20.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        i++;
        m624[i].m04.mRadius = 10.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftfoot"));
        i++;
        m624[i].m04.mRadius = 10.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightfoot"));
    }
}

atn::Vector3 Em203GreaterDaemonRed::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    } else {
        result = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail"));
    }
    return result;
}

// inferred: function removed by the linker (no reference in the DOL): its constants 4.0f and 2.0f
// stay in .sdata2 between those of vf64 and fn_80296268. Name and body invented.
float Em203GreaterDaemonRed::Stripped1_unk() {
    float a = 4.0f;
    float b = 2.0f;
    return mMotion.mTracks[0].m08 * a + b;
}

// inferred: chooses the next skill: the first high hover point (state 4/5 of m14C4), FlySkill or
// GroundSkill
// NONMATCHING: 92.74% - skill selection; register allocation and inlining of the hover search
void Em203GreaterDaemonRed::fn_80296268() {
    if (m14B0 == 0) {
        return;
    }
    if (m14C4 == 0) {
        if (SelectHover_unk()) {
            fn_8029A674(0);
        }
        m14C4 = 2;
        m14C0 = 0;
    } else if (m14C4 == 1) {
        if (SelectHover_unk()) {
            fn_8029A674(1);
        }
        m14C4 = 2;
        m14C0 = 1;
    } else if (m14C4 == 6) {
        fn_8029A788();
    } else if (m14C4 == 2) {
        int skill = 0;
        if (m1504[0].mSize != 0) {
            switch (m1504[0].mData[m151C]) {
            case 1:
                m15B4 = m14F4[m14C8];
                if (m15B4 != 0) {
                    StartSkill_unk(8);
                }
                break;
            case 3:
                StartSkill_unk(9);
                m14B4 = 0;
                m15B4 = m14D4[m14C8];
                m638 = m15B4;
                break;
            case 5:
                fn_8029A788();
                break;
            case 4:
                StartSkill_unk(10);
                m15B4 = m14FC[m14C8];
                m638 = m15B4;
                break;
            }
            m151C++;
            if (m151C >= (int)m1504[0].mSize) {
                m151C = 0;
            }
            skill = m1504[0].mData[m151C];
        }
        if (m14C0 == 0) {
            if (skill != 5) {
                m14C4 = 0;
            }
        } else if (m14C0 == 1) {
            if (skill != 5) {
                m14C4 = 1;
            }
        }
    } else if (m14C4 == 4) {
        StartState12_unk();
    } else if (m14C4 == 5) {
        StartState13_unk();
    }
}

// inferred: starts a flying skill (name invented)
inline void Em203GreaterDaemonRed::StartSkill_unk(int state) {
    m58C = state;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 29;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.5f;
    mMotion.fn_80097C08(0, 0x66, 1, 0, 1, 0.0f, 0.1f);
}

// inferred: chooses the next ground skill (wait: only after GrandAttackTime/GrandBreathTime)
// NONMATCHING: 99.72% - stack slots of the direction temporaries
void Em203GreaterDaemonRed::fn_80296B8C(int wait) {
    atn::Vector3 target(*m14B0->fn_8013C38C());
    atn::Vector3 dir = target - ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
    dir.y = 0.0f;
    float dist = dir.Length_unk();
    if (dist < 0.00001f) {
        dir.Set_unk(0.0f, 0.0f, 1.0f);
        dist = 0.1f;
    } else {
        dir.Normalize_unk();
    }
    int stun = 0;
    if (!fn_800E0AB0(&m1504[1]) && *fn_8028B74C(&m1504[1], m151C) == 4 && fn_8029597C(&mStatus) == 0) {
        stun = 1;
    }
    int next = 0;
    if (stun) {
        fn_8029B24C();
        next = 1;
    } else {
        float limit;
        if (m58C == 15) {
            limit = 0.0f;
        } else {
            limit = 0.5f;
        }
        if (fn_801A03E8(dir, fn_8014052C()) > limit) {
            if (dist <= 40.0f + m14E4[m14C8]->m40) {
                if (wait == 0 || m14B8 > m1684) {
                    fn_8029A928();
                    next = 1;
                }
            } else if (wait == 0 || m14B8 > m1688) {
                fn_8029AF6C();
                next = 1;
            }
        } else {
            fn_8029AC1C();
            next = 1;
        }
    }
    if (next) {
        m151C++;
        if (m151C >= (int)fn_800E0AA8(&m1504[1])) {
            m151C = 0;
        }
    }
}

// inferred: state 6/7 (hovers to the route point m1540)
// NONMATCHING: 99.78% - r3/r4 swap on the psq copy into m1530 (known blocker)
void Em203GreaterDaemonRed::fn_80296DB8() {
    if (m14B4 == 0) {
        atn::Vector3 point = m1540->mPos;
        fn_801C3910(point - mPos);
        if (m1528 == m1540->m10) {
            m1530 = point - mPos;
            ToIdle_unk();
        } else {
            m152C += 1.0f / m1670;
            float t = 0.5f * (1.0f + (float)sin(3.1415927f * m152C - 1.5707964f));
            mPos = (m1540->mPos - m154C) * t + m154C;
            if (m152C >= 1.0f) {
                m1528 = m1540->m10;
            }
        }
    } else if (m14B4 == 1) {
        if (m14B8 <= (int)m1670) {
            mPos += m1530 * (1.0f / m1670);
        } else {
            ToIdle_unk();
        }
    }
}

// inferred: state 8 (casts the flare)
// NONMATCHING: 92.62% - flare message; stack slots and register allocation
void Em203GreaterDaemonRed::fn_80297114() {
    if (m14B4 == 0) {
        if (Chant_unk() && mMotion.mTracks[0].m08 >= 28.0f) {
            fn_8016E970(6);
            m560 = 30;
            m14B4 = 1;
            PlayMotion_unk(0x145, 0, 0, 1.0f);
        }
    } else if (m14B4 == 1) {
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            ToIdle_unk();
        } else if (mMotion.mTracks[0].m08 >= 31.0f && mMotion.mTracks[0].m08 < 32.0f &&
                   m15B4 == m14F4[m14C8]) {
            wchar_t text[256];
            memset(text, 0, sizeof(text));
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xD5), *(const wchar_t**)((char*)m60 + 4));
            fn_80109828(lbl_8087F048, text);
            atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("chin"));
            atn::Vector3 dir = m14B0->mPos - pos;
            PSVECNormalize(&dir, &dir);
            mStatus.mParamB.m2C = 7.0f;
            m12CC = 0.0f;
            fn_800F8574(lbl_8087F048, this, m15B4, &pos, &dir, 0, 0x100, 0, 0.0f, 1.0f);
        }
    }
}

// inferred: unit vector rotated around y (name invented)
// inferred: same rotation with the angle by value (the angle is loaded before the node position is
// stored; name invented)
static inline atn::Vector3 FrontV_unk(float angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', angle);
    PSMTXMultVec(m, &front, &front);
    return front;
}
static inline atn::Vector3 Front_unk(const float& angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', angle);
    PSMTXMultVec(m, &front, &front);
    return front;
}

// inferred: shakes the camera (name invented)
inline void ShakeCamera_unk(int frames, float a, float b) {
    Field_unk* field = lbl_8087F430;
    field->m96C = field->m96C % 2;
    field->m970 = frames;
    field->m974 = a;
    field->m978 = b;
}

// inferred: state 9 (laser: m14B4 0 chant, 1 fires, 2 back to hovering)
// NONMATCHING: 97.48% - stack slots of the effect temporaries
void Em203GreaterDaemonRed::fn_80297474() {
    if (m14B4 == 0) {
        if (Chant_unk() && mMotion.mTracks[0].m08 >= 28.0f) {
            fn_8016E970(6);
            m560 = 30;
            PlayMotion_unk(0x13F, 0, 0, 1.0f);
            m14B4 = 1;
        }
    } else if (m14B4 == 1) {
        float end = mMotion.mTracks[0].m08;
        if (end > mMotion.fn_80097D7C(0)) {
            m14B4 = 2;
            PlayMotion_unk(2, 1, 0, 1.5f);
            m14B8 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 1002, 1);
            m14B4 = 2;
        } else if (mMotion.mTracks[0].m08 >= 10.0f && mMotion.mTracks[0].m08 < 11.0f) {
            if (fn_80370A78(lbl_8087F430, 0x97) == 0) {
                fn_80370AE4(lbl_8087F430, 0x97, 1);
            }
        } else if (mMotion.mTracks[0].m08 >= 32.0f && mMotion.mTracks[0].m08 < 33.0f) {
            fn_80232B7C(this, 1002);
            atn::Vector3 pos(0.0f, 0.0f, 30.0f);
            Color4f_unk color;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &m15BC, &mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, 2.0f);
        } else if (mMotion.mTracks[0].m08 >= 47.0f && mMotion.mTracks[0].m08 < 48.0f) {
            wchar_t text[256];
            memset(text, 0, sizeof(text));
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xD9), *(const wchar_t**)((char*)m60 + 4));
            fn_80109828(lbl_8087F048, text);
            fn_80239DAC(lbl_8087F3C0, this, 1002, 1);
            fn_80232B7C(this, 1002);
            atn::Vector3 pos(0.0f, 0.0f, 0.0f);
            atn::Vector3 rot(0.0f, 0.0f, 0.0f);
            Color4f_unk color;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &m15C8, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
            fn_8029BE7C();
            for (unsigned int i = 0; i < 32; i++) {
                RedSkill_unk* skill = i < 32 ? &lbl_8087F9E8->mSkills[i] : 0;
                bool active = false;
                if (skill->m04 != 0 && skill->m00 != 3) {
                    active = true;
                }
                if (active && skill->m08->m48 == 3) {
                    Color4f_unk c;
                    c.r = 1.0f;
                    c.g = 1.0f;
                    c.b = 1.0f;
                    c.a = 1.0f;
                    fn_8023A8B4(lbl_8087F3C0, &m162C, 0, 0, skill->m10, skill->m1C, c, -1, -1, 1, 1.0f);
                }
            }
        }
    } else if (m14B4 == 2) {
        ToIdle_unk();
    }
}

// inferred: state 10 (thunder: m14B4 0 chant, 1 cast, 2 wait, 3 bolts around the target)
// NONMATCHING: 89.76% - thunder bolts; inlined vector push_back and register allocation
void Em203GreaterDaemonRed::fn_8029795C() {
    if (m14C4 != 6) {
        fn_802953C8();
    }
    if (m14B4 == 0) {
        if (Chant_unk() && (m14C4 == 6 || mMotion.mTracks[0].m08 >= 28.0f)) {
            fn_8016E970(6);
            m560 = 30;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            if (m14C4 == 6) {
                mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
            } else {
                mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
            }
            m14B4 = 1;
        }
    } else if (m14B4 == 1) {
        if (m14C4 == 6 || mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0)) {
            fn_8016E970(6);
            m15D4.mSize -= m15D4.mSize;
            m560 = 4;
            m14B4 = 2;
            m14B8 = 0;
        }
    } else if (m14B4 == 2) {
        if (m14B8 >= m1698) {
            m14B4 = 3;
            m14B8 = 0;
        }
    } else if (m14B4 == 3) {
        if (m1690 * m1694 >= (int)m15D4.mSize && m14B8 % m16A0 == 0) {
            DaemonThunder_unk thunder;
            if (m15D4.mSize == 0) {
                thunder.mPos = m14B0->mPos;
                thunder.mFrame = m15E0;
            } else {
                float step = 2.0f * m168C / 6.0f;
                int x = rand() % (int)(2.0f * m168C / step);
                int z = rand() % (int)(2.0f * m168C / step);
                atn::Vector3 target = m14B0->mPos;
                atn::Vector3 mid = target + (lbl_8087F430->m080 - target) * 0.5f;
                atn::Vector3 pos(step * (float)x + (mid.x - m168C), 0.0f, step * (float)z + (mid.z - m168C));
                thunder.mPos = pos;
                thunder.mFrame = m15E0;
                fn_80232B7C(this, m15D4.mSize + 999);
                Color4f_unk color;
                color.r = 1.0f;
                color.g = 1.0f;
                color.b = 1.0f;
                color.a = 1.0f;
                fn_8023A8B4(lbl_8087F3C0, &m15E4, 0, 0, m15D4[m15D4.mSize - 1].mPos, mRot, color, -1, -1, 1, 1.0f);
                fn_800C31F4("SE_SP016_001", 0, -1, 1.0f);
            }
            m15D4.push_back(thunder);
        }
        if (m15D4.mSize != 0 && m1690 % m15D4.mSize == 0 && m14B8 > m16A0) {
            m14B8 = 0;
            m14B4 = 2;
        }
    }
    if (mMotion.mTracks[0].m00 == 0x14D || mMotion.mTracks[0].m00 == 0x13F) {
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            if (m14C4 == 6) {
                mMotion.fn_80097C08(0, 0x153, 1, 0, 1, 0.0f, 0.1f);
            } else {
                mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
            }
        } else if (mMotion.mTracks[0].m08 >= 30.0f && m14C4 == 6) {
            mMotion.mTracks[0].m0C = 0.65f;
        }
    }
    for (unsigned int i = 0; i < m15D4.mSize; i++) {
        if (m15E0 == m169C + m15D4[i].mFrame) {
            fn_80239DAC(lbl_8087F3C0, this, i + 1000, 1);
            Effect_unk* effect = lbl_8087F048;
            int id = fn_800F8548(effect);
            fn_800FAB80(effect, this, m14FC[m14C8], id, &m15D4[i].mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            if (i % 3 == 0) {
                ShakeCamera_unk(5, 1.0f, 3.0f);
                fn_800C31F4("SE_EM203_004", 0, -1, 1.0f);
            }
        } else if (m15E0 == m169C + m15D4[i].mFrame + 30) {
            fn_80239DAC(lbl_8087F3C0, this, i + 2000, 1);
        }
    }
    int max = m1690 * m1694;
    if ((int)m15D4.mSize >= max && m15E0 >= m169C + m15D4[max - 1].mFrame + 30) {
        for (unsigned int i = 0; i < m15D4.mSize; i++) {
            fn_80239DAC(lbl_8087F3C0, this, i + 2000, 1);
            fn_80239DAC(lbl_8087F3C0, this, i + 1000, 0);
        }
        m15D4.mSize -= m15D4.mSize;
        if (m14C4 == 6) {
            StartState12_unk();
        } else {
            ToIdle_unk();
        }
        m15E0 = 0;
    }
    m15E0++;
}

// inferred: state 11 (dives on the target)

void Em203GreaterDaemonRed::fn_80298580() {
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    fn_801C3910(dir);

    if (m14B4 == 0) {
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            StartState12_unk();
            return;
        }
        if (m14B8 <= 42) {
            float k = 0.023809524f;
            mPos += m1530 * k;
        }
        if (m14DC[m14C8] != 0) {
            if (mMotion.mTracks[0].m08 < 41.0f && mMotion.mTracks[0].m08 >= 40.0f) {
                ShakeCamera_unk(10, 1.0f, 2.0f);
            } else if (mMotion.mTracks[0].m08 < 44.0f) {
                atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
                fn_800FAB80(lbl_8087F048, this, m14DC[m14C8], m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            }
        }
    }
}

// inferred: state 14 (turns to the target and strikes with the claws)
// NONMATCHING: 98.86% - load of mRot.y scheduled before the node position store
void Em203GreaterDaemonRed::fn_80298850() {
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartState12_unk();
        return;
    }
    if (mMotion.mTracks[0].m08 <= 20.0f) {
        mRot.y += atn::WrapAngle_unk(m1548 - m1544) / 20.0f;
    } else if ((mMotion.mTracks[0].m08 > 36.0f && mMotion.mTracks[0].m08 < 40.0f) ||
               (mMotion.mTracks[0].m08 > 56.0f && mMotion.mTracks[0].m08 < 60.0f)) {
        atn::Vector3 spine = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
        spine += Front_unk(mRot.y) * 20.0f;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, m14E4[m14C8], m590, 30, -1, 0.0f);
    } else {
        m590 = fn_800F8548(lbl_8087F048);
    }
}

// inferred: state 15 (walks to the target)
// NONMATCHING: 96.91% - register allocation of the walk vectors
void Em203GreaterDaemonRed::fn_80298B10() {
    atn::Vector3 dir = m14B0->mPos - mPos;
    dir.y = 0.0f;
    float dist = dir.Length_unk();
    dir.Normalize_unk();
    float frame = mMotion.mTracks[0].m08;
    if (frame > 21.0f && frame <= 38.0f) {
        mRot.y += atn::WrapAngle_unk(m1548 - m1544) / 17.0f;
        if (dist < 70.0f || dist > 90.0f) {
            mPos += dir * 80.0f * 0.2f;
        }
    } else {
        m1548 = dir.ToRotation_unk().y;
    }
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80296B8C(0);
    }
}

// inferred: state 16 (breath on the ground: starts the bolts of fn_8029B704)
// NONMATCHING: 98.80% - load of mRot.y scheduled before the node position store
void Em203GreaterDaemonRed::fn_80298E98() {
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartState12_unk();
        return;
    }
    if (mMotion.mTracks[0].m08 <= 28.0001f) {
        fn_802953C8();
    }
    if (m1584 == 0 && mMotion.mTracks[0].m08 > 42.0f) {
        m1584 = 1;
        m1580 = 33;
        m1588 = fn_800F8548(lbl_8087F048);
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("chin"));
        pos += Front_unk(mRot.y) * 20.0f;
        pos.y = 0.0f;
        m158C = pos;
        m1598 = mRot;
    }
}

// inferred: state 13 (takes off to the high hover point)
void Em203GreaterDaemonRed::fn_80299134() {
    if (m14B4 == 0) {
        float end = mMotion.mTracks[0].m08;
        if (end > mMotion.fn_80097D7C(0)) {
            m14B4 = 1;
            PlayMotion_unk(0x13F, 0, 0, 1.0f);
        } else if (m14B8 > 25) {
            float k = 0.028571429f;
            mPos += m1530 * k;
        }
    } else if (m14B4 == 1) {
        float end = mMotion.mTracks[0].m08;
        if (end > mMotion.fn_80097D7C(0)) {
            m14C4 = 1;
            ToIdle_unk();
        }
    }
}

// inferred: state 17 (knocked down: falls, then stays stunned for StunTime seconds)
// NONMATCHING: 99.16% - r4/r5 order of the camera look stores (known, as Em203GreaterDaemon)
void Em203GreaterDaemonRed::fn_802992F8() {
    if (m14B4 == 0) {
        Field_unk* field = lbl_8087F430;
        if (mMotion.mTracks[0].m08 < 24.0f) {
            atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
            if (m1524 == 0) {
                field->m8A0 = this;
                field->m97C.Set_unk(pos, false);
                m1524 = 1;
            } else {
                field->m8A0 = this;
                field->m97C.mPos = pos;
            }
        } else if (m1524 != 0) {
            field->m8A0 = 0;
            field->m97C.m00 = 0;
            m1524 = 0;
        }
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            PlayMotion_unk(0x1E2, 1, 0, 1.0f);
            m14B8 = 0;
            m14B4 = 1;
        }
        if (mMotion.mTracks[0].m08 < 36.0f && mMotion.mTracks[0].m08 >= 35.0f) {
            ShakeCamera_unk(10, 1.0f, 2.0f);
        }
        mPos.y += m1530.y / mMotion.fn_80097D7C(0);
    } else if (m14B4 == 1 && m14B8 > m167C * 30) {
        StartState12_unk();
    }
}

// inferred: state 19 (stunned)
// NONMATCHING: 98.74% - r4/r5 order of the camera look stores (known)
void Em203GreaterDaemonRed::fn_802995D4() {
    Field_unk* field = lbl_8087F430;
    if (mMotion.mTracks[0].m08 < 20.0f) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
        if (m1524 == 0) {
            field->m8A0 = this;
            field->m97C.Set_unk(pos, false);
            m1524 = 1;
        } else {
            field->m8A0 = this;
            field->m97C.mPos = pos;
        }
    } else if (m1524 != 0) {
        field->m8A0 = 0;
        field->m97C.m00 = 0;
        m1524 = 0;
    }
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    }
}

// inferred: state 20 (falls to the ground and goes back up)
// NONMATCHING: 98.77% - r4/r5 order of the camera look stores (known)
void Em203GreaterDaemonRed::fn_802997B8() {
    Field_unk* field = lbl_8087F430;
    if (mMotion.mTracks[0].m08 < 30.0f) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
        if (m1524 == 0) {
            field->m8A0 = this;
            field->m97C.Set_unk(pos, false);
            m1524 = 1;
        } else {
            field->m8A0 = this;
            field->m97C.mPos = pos;
        }
    } else if (m1524 != 0) {
        field->m8A0 = 0;
        field->m97C.m00 = 0;
        m1524 = 0;
    }
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame <= 20.0f) {
        mPos += m1530 * (1.0f / (20.0f / mMotion.mTracks[0].m0C));
    } else if (20.0f < frame && frame <= 40.0f) {
        mPos -= m1530 * (1.0f / (20.0f / mMotion.mTracks[0].m0C));
    }
}

// inferred: state 21 (walks to the centre of the arena, then casts the thunder)
// NONMATCHING: 97.38% - register allocation of the walk vectors
void Em203GreaterDaemonRed::fn_80299A7C() {
    atn::Vector3 dir = m1558 - mPos;
    dir.y = 0.0f;
    float dist = dir.Length_unk();
    float frame = mMotion.mTracks[0].m08;
    if (frame > 21.0f && frame <= 38.0f) {
        mRot.y += atn::WrapAngle_unk(m1548 - m1544) / 17.0f;
        if (dist > 10.0f) {
            dir.Normalize_unk();
            mPos += dir * 50.0f * 0.2f;
        }
    } else {
        atn::Vector3 d(m14B0->mPos.x - mPos.x, 0.0f, m14B0->mPos.z - mPos.z);
        if (!d.IsZero_unk()) {
            m1548 = d.ToRotation_unk().y;
        }
    }
    if (frame >= mMotion.fn_80097D7C(0)) {
        m58C = 10;
        ResetState_unk();
        fn_8016E970(6);
        m560 = 29;
        PlayMotion_unk(0x153, 1, 0, 1.0f);
        m15B4 = m14FC[m14C8];
        m638 = m15B4;
    }
}

// inferred: state 2 (dead: falls to the ground, then state 22)
// NONMATCHING: 89.96% - order of the lerp terms and frame reloads
void Em203GreaterDaemonRed::vfEC() {
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartState22_unk();
    } else if (mMotion.mTracks[0].m08 < mMotion.fn_80097D7C(0) - 10.0f) {
        float t = mMotion.mTracks[0].m08 / (mMotion.fn_80097D7C(0) - 10.0f);
        mPos = (m1648 - m163C) * t + m163C;
    }
}

// inferred: state 23 (finished by `m1638`: slow motion and motion blur, then the event 0x15)
// NONMATCHING: 81.60% - finishing blow; motion blur copy and camera look stores
void Em203GreaterDaemonRed::fn_8029A0A8() {
    if (m14B4 >= 2) {
        return;
    }
    float speed = 1.0f;
    if (m14B4 == 0) {
        RedBlur_unk blur = lbl_8087EFA8->m240;
        blur.m00 = 1;
        lbl_8087EFA8->m240 = blur;
        if (m14B8 >= 55) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 0.5f;
            mMotion.fn_80097C08(0, 0x1E1, 0, 0, 1, 0.0f, 0.01f);
            Field_unk* field = lbl_8087F430;
            atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
            field->m8A0 = this;
            field->m97C.Set_unk(pos, false);
            m14B4 = 1;
        } else if (m14B8 < 40) {
            speed = 0.1f;
        }
    } else {
        Field_unk* field = lbl_8087F430;
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
        field->m8A0 = this;
        field->m97C.mPos = pos;
        float frame = mMotion.mTracks[0].m08;
        if (frame > 30.0f) {
            field->m8A0 = 0;
            field->m97C.m00 = 0;
            RedBlur_unk blur = lbl_8087EFA8->m240;
            blur.m00 = 0;
            lbl_8087EFA8->m240 = blur;
            lbl_8087EFA8->m3A4 = 1.0f;
            fn_80370AE4(lbl_8087F430, 0x15, 1);
            m14B4++;
        } else if (21.0f <= frame && frame <= 22.0f) {
            fn_80370B78(lbl_8087F430, -1, 10, 0, -5.0f);
            speed = 0.75f;
        } else if (frame < 15.0f) {
            atn::Vector3 d(m1638->mPos.x - mPos.x, 0.0f, m1638->mPos.z - mPos.z);
            PSVECNormalize(&d, &d);
            mPos -= d * 0.5f;
            mPos.y += 2.5f;
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &front, &front);
            d = front + (d - front) * 0.3f;
            if (frame > 10.0f) {
                mMotion.mTracks[0].m0C = 0.25f;
                speed = 0.75f;
            }
        } else {
            mPos.y += 2.5f;
            mMotion.mTracks[0].m0C = 0.25f;
            speed = 0.75f;
        }
    }
    lbl_8087EFA8->m3A4 = speed;
    fn_80370320(lbl_8087F430, 0x38D, 0, 0);
}

int Em203GreaterDaemonRed::vf68(CharaTask* attacker) {
    return m58C == 22 ? 9 : 0;
}

void Em203GreaterDaemonRed::vf6C(int kind, CharaTask* other) {
    atn::Vector3 d = mPos - other->mPos;
    d.y = 0.0f;
    PSVECNormalize(&d, &d);
    if (kind == 9) {
        m1638 = other;
        m58C = 23;
        ResetState_unk();
        fn_8016E970(3);
        fn_80370320(lbl_8087F430, 0x38D, 0, 0);
    }
}

// inferred: starts the hovering to the route point m1540 (kind 0: state 6, 1: state 7)
void Em203GreaterDaemonRed::fn_8029A674(int kind) {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    if (kind == 0) {
        m58C = 6;
        m14C0 = 0;
    } else if (kind == 1) {
        m58C = 7;
        m14C0 = 1;
    }
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    PlayMotion_unk(2, 1, 0, 1.5f);
    m154C = mPos;
}

// inferred: starts state 11 (dives on the target)
// NONMATCHING: 99.81% - r5/r6 crossing on the psq copy into m1530 (known blocker)
void Em203GreaterDaemonRed::fn_8029A788() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 11;
    ResetState_unk();
    m15FC = 0;
    fn_8016E970(6);
    m560 = 4;
    PlayMotion_unk(0x15E, 0, 0, 1.0f);
    atn::Vector3 target = m14B0->mPos;
    target.y = 0.0f;
    m1530 = target - mPos;
    atn::Vector3 dir(m1530.x, 0.0f, m1530.z);
    dir.Normalize_unk();
    m1530 -= dir * 85.0f;
}

// inferred: starts state 14 (claws)
// NONMATCHING: 98.70% - stack slots of the ToRotation temporaries
void Em203GreaterDaemonRed::fn_8029A928() {
    m58C = 14;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    PlayMotion_unk(0x15F, 0, 0, 1.0f);
    m1544 = mRot.y;
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    m1548 = dir.ToRotation_unk().y;
}

// inferred: starts state 15 (turns and walks to the target)
// NONMATCHING: 86.69% - stack slots of the ToRotation temporaries and WrapAngle form
void Em203GreaterDaemonRed::fn_8029AC1C() {
    m58C = 15;
    ResetState_unk();
    m1544 = mRot.y;
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    m1548 = dir.ToRotation_unk().y;
    if (atn::WrapAngle_unk(m1548 - m1544) < 0.0f) {
        PlayMotion_unk(0x158, 0, 1, 1.0f);
    } else {
        PlayMotion_unk(0x159, 0, 1, 1.0f);
    }
}

// inferred: starts state 16 (breath on the ground)
// NONMATCHING: 98.67% - stack slots of the ToRotation temporaries
void Em203GreaterDaemonRed::fn_8029AF6C() {
    m58C = 16;
    ResetState_unk();
    PlayMotion_unk(0x160, 0, 0, 1.0f);
    m1544 = mRot.y;
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    m1548 = dir.ToRotation_unk().y;
}

// inferred: starts state 21 (walks to the centre of the arena)
// NONMATCHING: 86.41% - stack slots of the ToRotation temporaries and WrapAngle form
void Em203GreaterDaemonRed::fn_8029B24C() {
    ResetState_unk();
    m58C = 21;
    m1544 = mRot.y;
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    m1548 = dir.ToRotation_unk().y;
    if (atn::WrapAngle_unk(m1548 - m1544) < 0.0f) {
        PlayMotion_unk(0x158, 0, 1, 1.0f);
    } else {
        PlayMotion_unk(0x159, 0, 1, 1.0f);
    }
}

// inferred: starts state 2 (dead: falls to the ground under the daemon)
void Em203GreaterDaemonRed::fn_8029B59C() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    m58C = 2;
    ResetState_unk();
    fn_8016E970(3);
    PlayMotion_unk(0x1E1, 0, 0, 1.0f);
    m163C = mPos;
    m1648 = mPos;
    CollResult_unk result;
    atn::Vector3 from = mPos;
    atn::Vector3 to = mPos;
    from.y += 5.0f;
    to.y -= 10000.0f;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x20000000, 0, 0) != 0) {
        m1648 = result.m10;
    }
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

// inferred: bolts of the ground breath (from frame 33 to 120); nonzero when they are over
// NONMATCHING: 88.43% - breath bolts; inlined vector push_back and matrix temporaries
int Em203GreaterDaemonRed::fn_8029B704() {
    if (m14EC[m14C8] == 0) {
        return 1;
    }
    if (m1580 >= 120) {
        return 1;
    }
    if (m1580 >= 33) {
        atn::Vector3 dir(0.0f, 0.0f, 30.0f);
        PSMTXMultVec(fn_800109E0(m1598).m, &dir, &dir);
        atn::Vector3 pos = m158C + dir * (float)(m1580 - 33);
        fn_800FAB80(lbl_8087F048, this, m14EC[m14C8], m1588, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        DaemonBolt_unk bolt;
        bolt.mFrame = m1580 + 1;
        bolt.mPos = pos;
        m15A8.push_back(bolt);
        if (m1580 % 4 == 0) {
            fn_800C344C("SE_MB003_003", &pos, 0, -1, 1.0f);
        }
    }
    for (unsigned int i = 0; i < m15A8.mSize; i++) {
        if (m1580 == m15A8[i].mFrame) {
            ShakeCamera_unk(1, 0.2f, 0.1f);
        }
    }
    m1580++;
    return 0;
}

atn::Vector3 Em203GreaterDaemonRed::vf58() {
    return GetNodePos_unk(mMotion.GetNodeMtx_unk("neck")) + atn::Vector3(0.0f, 0.0f, 0.0f);
}

// inferred: homing parameters with their defaults; name invented
struct RedHoming_unk : public HomingParam_unk {
    RedHoming_unk() {
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

// inferred: fires the homing lasers at every free party member, from the spine
// NONMATCHING: 86.02% - homing lasers; register allocation and stack slots
void Em203GreaterDaemonRed::fn_8029BE7C() {
    if (m14D4 == 0) {
        return;
    }
    int i = 0;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
        atn::Vector3 offset(0.0f, 0.0f, 30.0f);
        PSMTXMultVec(fn_800109E0(mRot).m, &offset, &offset);
        atn::Matrix3x4 m(mMotion.GetNodeMtx_unk("spine"));
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = 0.0f;
        PSMTXMultVec(m.m, &offset, &offset);
        pos += offset;
        atn::Vector3 dir(0.0f, 0.0f, 10.0f);
        Mtx ry;
        PSMTXRotRad(ry, 'y', 1.5707964f);
        PSMTXMultVec(ry, &dir, &dir);
        float step = 360.0f / (float)lbl_8087F8A0->mCount;
        Mtx rz;
        PSMTXRotRad(rz, 'z', 0.017453292f * (step * (float)i));
        PSMTXMultVec(rz, &dir, &dir);
        PSMTXMultVec(fn_800109E0(mRot).m, &dir, &dir);
        RedHoming_unk param;
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader->mFlag28) {
            if (p != leader) {
                continue;
            }
            param.mTarget = p;
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
        param.mAccel = 0.3f;
        param.mMaxSpeed = m1674;
        param.mTime = (float)m14D4[m14C8]->m5C;
        param.mTurn = 0.017453292f * m1678;
        fn_800F8574(lbl_8087F048, this, m14D4[m14C8], &pos, &dir, &param, 0x504, 0, 0.0f, 1.0f);
        i++;
    }
}

// inferred: the route point nearest to the daemon among the high hover points (m153C)
// NONMATCHING: 99.42% - loop counter registers swapped (r29/r30)
int Em203GreaterDaemonRed::fn_8029C564(int arg) {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    if (m1564.size() == 0) {
        return 0;
    }
    float best = 1.0e16f;
    int nearest = -1;
    for (unsigned int i = 0; i < m1564.size(); i++) {
        atn::Vector3 d = m1564[i]->mPos - mPos;
        float dist = sqrt(d.x * d.x + d.z * d.z);
        if (dist < best) {
            best = dist;
            nearest = i;
        }
    }
    RouteTable_unk* table = lbl_8087F430->m10D8;
    int index = fn_803C1560(table, &m1564[nearest]->mPos, fn_80179D44(), 0, 1, 1, -1.0f) - 1;
    m153C = &table->mPoints[index];
    return 1;
}

int Em203GreaterDaemonRed::vfE4() {
    if (m58C == 9 && m14B4 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (44.0f <= frame && frame <= 49.0f) {
            return 1;
        }
    } else if (m58C == 8 && m14B4 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (30.0f <= frame && frame <= 33.0f) {
            return 1;
        }
    }
    return 0;
}

// inferred: function removed by the linker (no reference in the DOL): its constant 100.0f stays in
// .sdata2 between those of vfE4 and vfE0. Name and body invented.
float Em203GreaterDaemonRed::Stripped2_unk() {
    return 100.0f * mMotion.mTracks[0].m08;
}

// inferred: the four points of the claws (right shoulder, spine1, left shoulder, spine)
void Em203GreaterDaemonRed::vfCC(atn::Vector3* out, const atn::Vector3& dir) {
    out[0] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightshoulder"))->GetTrans_unk();
    out[1] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
    out[2] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftshoulder"))->GetTrans_unk();
    out[3] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
    (void)dir;
}

atn::Vector3 Em203GreaterDaemonRed::vfD0(float angle) {
    return ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk();
}


atn::Vector3 Em203GreaterDaemonRed::vfD8(CharaTask* target) {
    atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    pos.y += 10.0f;
    pos += Front_unk(mRot.y) * 10.0f;
    return pos;
}
