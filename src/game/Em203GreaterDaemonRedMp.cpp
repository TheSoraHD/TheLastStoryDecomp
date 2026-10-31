#include "game/Em203GreaterDaemonRedMp.h"
#include "atn/Csv.h"
#include "atn/SoundRef.h"
#include "atn/Keyboard.h"
#include "game/CharaManager.h"
#include "game/PlayerManager.h"
#include "math/Vec.h"
#include "game/GameBgm.h"
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
    void Set_unk(const atn::Vector3& pos, int flag) {
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
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
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

// inferred: hit points ratio (weak copy 0x8029F3AC, also called by game/EnemyQuarkSecond.cpp)
extern "C" inline float fn_8029F3AC(CharaStatus* status) {
    if (status->mParamB.m84 > 0) {
        return status->m04 / status->mParamB.m84;
    }
    return 0.0f;
}

// inferred: resets the counters of the state (names invented)
inline void Em203GreaterDaemonRedMp::ResetState_unk() {
    m14D8 = 0;
    m14DC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m155C = 0.0f;
    m15FC = 0;
}

// inferred: plays a motion on track 0
inline void Em203GreaterDaemonRedMp::PlayMotion_unk(int motion, int loop, int arg, float speed) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
    mMotion.fn_80097C08(0, motion, loop, arg, 1, 0.0f, 0.1f);
}

// inferred: back to state 6 (hovering)
inline void Em203GreaterDaemonRedMp::ToIdle_unk() {
    ResetState_unk();
    m58C = 6;
    fn_8016E970(3);
    PlayMotion_unk(2, 1, 0, 1.5f);
    fn_80239DAC(lbl_8087F3C0, this, -2, 1);
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: linear interpolation (name invented)
inline atn::Vector3 LerpV_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: enemy manager (not inlined here)
extern "C" CharaManager<EnemyTask>* fn_801A03E0();

// NONMATCHING: 99.8% - register swap of the second std::string temporary (known, as in Em203GreaterDaemonRed)
Em203GreaterDaemonRedMp::Em203GreaterDaemonRedMp(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14D8(0), m14DC(0), m14E0(0), m14E4(2), m14E8(0), m14EC(0),
      m14F0(0), m14FC(0), m1500(0), m1504(0), m1550(0), m1554(0), m155C(0.0f), m156C(0), m1570(0), m15C0(0),
      m15C4(0), m15C8(0), m15FC(0), m160C(0), m1610(0), m1614(0), m1618(0), m161C(0), m1620(0), m1630(0),
      m1634(0), m1644(0), m1648(0), m164C(0), m1650(0), m1654(90), m1658(0), m165C(0), m1660(0), m1664(0),
      m1674(0), m1678(0), m1688(0), m1690(3.0f), m16A8(10), m16AC(35), m16B0(35) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em203_02_00.csv");
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
    m167C.vf0C(path.c_str());
    m1668.LoadRequest("/effect/ef_uc015.efp");
    m1560.x = 0.0f;
    m1560.y = 0.0f;
    m1560.z = 0.0f;
    mColli.mFlags &= ~0x80000000;
}

int Em203GreaterDaemonRedMp::vf0C() {
    const char* key;
    if (fn_8013655C() == 0 && m167C.fn_80473F50() == 0 && m1668.IsLoading() == 0) {
        fn_80288E30(this, 1.0f);
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x2C9;
        mStatus.m18 |= 0x3800000;
        mStatus.m18 |= 0x8410;
        mStatus.m18 |= 0x80000;
        mStatus.m18 |= 0x4;
        RouteTable_unk* table;
        if (fn_80121F00() != 0) {
            table = fn_80121F00()->fn_8013C504();
        } else {
            table = 0;
        }
        RoutePlace_unk* center = 0;
        if (m167C.fn_80470580() != 0 && table != 0) {
            atn::Csv csv((const char*)m167C.fn_80470580(), m167C.fn_8047059C());
            float zero = 0.0f;
            int none = 0;
            do {
                key = csv.fn_8005B9CC();
                if (strcmp(key, "HoverSpeed") == 0) {
                    m168C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "BulletSpeed") == 0) {
                    m1690 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "LimitAngle") == 0) {
                    m1694 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "AtackedMax") == 0) {
                    m1698 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GunPerAttac") == 0) {
                    m169C = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "MagicPerAttack") == 0) {
                    m16A0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "StunTime") == 0) {
                    m16A4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandTime") == 0) {
                    m16A8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandAttackTime") == 0) {
                    m16AC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "GrandBreathTime") == 0) {
                    m16B0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBoltAreaRadius") == 0) {
                    m16B4 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBoltNum") == 0) {
                    m16BC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBoltMax") == 0) {
                    m16B8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderBrankTime") == 0) {
                    m16C0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderChargeTime") == 0) {
                    m16C4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThunderTimeLag") == 0) {
                    m16C8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "1stHoverSkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1508[0][0].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "2ndHoverSkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1508[0][1].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "3rdHoverSkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1508[0][2].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "1stFlySkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1508[1][0].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "2ndFlySkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1508[1][1].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "3rdFlySkill") == 0) {
                    int skill = none;
                    do {
                        skill = fn_800DC12C(csv.fn_8005B9CC());
                        m1508[1][2].push_back(skill);
                    } while (skill != 0);
                } else if (strcmp(key, "StunHPRate") == 0) {
                    float rate = zero;
                    for (int i = 0; i < 3; i++) {
                        rate = fn_800DC288(csv.fn_8005B9CC());
                        m1624.push_back(rate);
                    }
                } else if (strcmp(key, "CenterPoint") == 0) {
                    center = table->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
                } else if (strcmp(key, "HighInsideHoverPoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = table->FindPlace_unk(id);
                        if (place != 0) {
                            m15A8.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "HoverDirection") == 0) {
                    int dir = none;
                    do {
                        dir = fn_800DC12C(csv.fn_8005B9CC());
                        m15B4.push_back(dir);
                    } while (dir != 0);
                } else if (strcmp(key, "LaserID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m14E8 = hit;
                    }
                } else if (strcmp(key, "TackleID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m14EC = hit;
                    }
                } else if (strcmp(key, "ClawID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m14F0 = hit;
                    }
                } else if (strcmp(key, "BreathID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m14F4 = hit;
                    }
                } else if (strcmp(key, "FlareID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m14F8 = hit;
                    }
                } else if (strcmp(key, "ThunderBoltID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m1500 = hit;
                    }
                } else if (strcmp(key, "ThunderBoltPreID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m1504 = hit;
                    }
                } else if (strcmp(key, "DamageCircleID") == 0) {
                    HitInfo_unk* hit = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                    if (hit != 0) {
                        m14FC = hit;
                    }
                } else if (strcmp(key, "OwnerDaemon") == 0) {
                    m1644 = 1;
                    m1648 = (Em203GreaterDaemonRedMp*)fn_801A03E0()->Find_unk(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SubDaemon") == 0) {
                    m164C = fn_801A03E0()->Find_unk(atoi(csv.fn_8005B9CC()));
                }
            } while (csv.fn_8005BBF8());
        }
        if (center != 0) {
            m1638 = center->mPos;
        } else {
            m1638.Set_unk(0.0f, 0.0f, 0.0f);
        }
        vf11C();
        return 1;
    }
    return 0;
}

// inferred: the "vector length error" of push_back is in the string pool right after the keys of vf0C
template void std::vector<RoutePlace_unk*>::push_back(RoutePlace_unk* const& x);

// inferred: the owner daemon looks at its neck for 60 frames in state 2; the sub daemon looks at the head of
// the other daemon while m1654 counts down; then the map BGM follows the height of the target
// NONMATCHING: 96.19% - this/field registers swapped and the look point flag store folded (as Red fn_802992F8)
void Em203GreaterDaemonRedMp::vf10() {
    if (mFlag2 == 0) {
        vf104();
    } else {
        if (m58C == 2) {
            m14DC++;
            fn_802A2070();
        }
        if (m1664 != m1660) {
            fn_802A3D80(1);
            m1664 = m1660;
        }
    }
    if (m1650 != 0) {
        m1654--;
        if (fn_80370A78(lbl_8087F430, 100) == 0) {
            fn_80370AE4(lbl_8087F430, 100, 1);
        }
    }
    if (m58C == 2) {
        Field_unk* field = lbl_8087F430;
        m1678++;
        if (m1678 < 60) {
            atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
            if (m1554 == 0) {
                field->m8A0 = this;
                field->m97C.Set_unk(pos, false);
                m1554 = 1;
            } else {
                field->m8A0 = this;
                field->m97C.mPos = pos;
            }
        } else if (m1554 != 0) {
            field->m8A0 = 0;
            field->m97C.m00 = 0;
            m1554 = 0;
        }
    }
    if (m1644 == 0 && m1650 != 0 && m1658 == 0 && m164C != 0) {
        Field_unk* field = lbl_8087F430;
        if (m1654 > 0) {
            atn::Vector3 pos = GetNodePos_unk(m164C->mMotion.GetNodeMtx_unk("head"));
            if (m1554 == 0) {
                field->m8A0 = this;
                field->m97C.Set_unk(pos, false);
                m1554 = 1;
            } else {
                field->m8A0 = this;
                field->m97C.mPos = pos;
            }
        } else {
            m1658 = 1;
            if (m1554 != 0) {
                field->m8A0 = 0;
                field->m97C.m00 = 0;
                m1554 = 0;
            }
        }
    }
    fn_8014C540();
    fn_80145334();
    vf11C();
    RedMap_unk* map = lbl_8087F430 != 0 ? fn_80373148(lbl_8087F430) : 0;
    if (map != 0 && m1644 == 0) {
        int kind;
        if (atn::Abs_unk(m14D4->mPos.y - mPos.y) < 20.0f) {
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

void Em203GreaterDaemonRedMp::fn_8029DB54() {}

// inferred: keeps the height of the flight at 60
void Em203GreaterDaemonRedMp::fn_8029DB58() {
    float target = 60.0f;
    float d = target - mPos.y;
    if (mPos.y > target) {
        if (d < 0.0f) {
            d = -d;
        }
        if (d < 1.635f) {
            mPos.y = 60.0f;
        } else {
            mPos.y -= 1.635f;
        }
    } else if (mPos.y < target) {
        if (d < 0.0f) {
            d = -d;
        }
        if (d < 0.6666667f) {
            mPos.y = 60.0f;
        } else {
            mPos.y += 0.6666667f;
        }
    }
}

// inferred: turns towards the target at 3 degrees per frame times `scale`
void Em203GreaterDaemonRedMp::fn_8029DBE0(float scale) {
    CharaTask* t = m14D4;
    float yaw = (t->mPos - mPos).ToRotationZ_unk().y;
    float diff = atn::WrapAngle_unk(yaw - mRot.y);
    float step = scale;
    step *= 0.05235988f;
    if ((diff < 0.0f ? -diff : diff) <= step) {
        mRot.y = yaw;
    } else if (diff <= 0.0f) {
        mRot.y -= step;
    } else {
        mRot.y += step;
    }
}

int Em203GreaterDaemonRedMp::vf10C(HitMsg_unk* msg) {
    msg->m10 = lbl_807C7030;
    msg->m50 |= 0x10;
    return 1;
}

// inferred: dies when the hit kills it, otherwise moves to the next "StunHPRate" threshold
void Em203GreaterDaemonRedMp::vf40(HitInfo_unk* hit) {
    if (mFlag2 == 0) {
        if ((mStatus.m0C & 0x20) == 0x20) {
            if (m1674 == 0) {
                m1674 = 1;
                vf48();
                fn_80178A6C();
                fn_8015495C(0, 1, 1, 1);
                fn_8016DA4C();
                m58C = 2;
                ResetState_unk();
                fn_8016E970(6);
                m560 = 0x17;
                PlayMotion_unk(0x1E1, 0, 0, 1.0f);
                m1560 = m1638 - mPos;
            }
        } else {
            int i = m1614;
            if (mStatus.m04 < mStatus.mParamB.m84 * m1624[i]) {
                int last = m1624.mSize - 1;
                m1614 = i + 1 < last ? i + 1 : last;
            }
        }
    }
}

// NONMATCHING: 85% - stack slots and scheduling of the capsule vectors (as Red vfF8)
void Em203GreaterDaemonRedMp::vf11C() {
    m614 = mPos + m5A4;
    m620 = m5B0;
    m614.y += m5B0;
    atn::Vector3 center = (GetNodePos_unk(mMotion.GetNodeMtx_unk("leftfoot")) + GetNodePos_unk(mMotion.GetNodeMtx_unk("rightfoot"))) * 0.5f;
    atn::Vector3 head = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    mColli.m3C = center;
    mColli.m48 = head;
    mColli.m54 = m620;
}

void Em203GreaterDaemonRedMp::vf50() {
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

void Em203GreaterDaemonRedMp::vf54() {
    if (m624.mSize != 0) {
        unsigned int i = 0;
        m624[i].m04.mRadius = 28.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail"));
        m624[i].m04.mCenter.y -= 2.0f;
        i++;
        m624[i].m04.mRadius = 12.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
        i++;
        m624[i].m04.mRadius = 10.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightmiddle1"));
        i++;
        m624[i].m04.mRadius = 10.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftmiddle1"));
        i++;
        m624[i].m04.mRadius = 25.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        i++;
        m624[i].m04.mRadius = 15.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftfoot"));
        i++;
        m624[i].m04.mRadius = 15.0f;
        m624[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightfoot"));
    }
}

atn::Vector3 Em203GreaterDaemonRedMp::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    } else {
        result = GetNodePos_unk(mMotion.GetNodeMtx_unk("tail"));
    }
    return result;
}

// NONMATCHING: 99.89% - stack slots of the spine and target vectors swapped
// inferred: picks the next ground attack: claw when close (alternating with fn_802A2F48 every 3 times),
// tackle when far, or turns when the target is behind; `wait` waits for GrandAttackTime/GrandBreathTime
void Em203GreaterDaemonRedMp::fn_8029F148(int wait) {
    if (m1650 == 0 && m1644 == 0 && fn_8029F3AC(&mStatus) < 0.6f) {
        fn_802A3520();
        return;
    }
    if (m1648 != 0 && m1648->CanAct_unk() && m1648->fn_802A49C8() == 0) {
        m14E4 = 5;
        fn_802A35B0(5);
        return;
    }
    atn::Vector3 target(*m14D4->fn_8013C38C());
    atn::Vector3 spine = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine"))->GetTrans_unk();
    atn::Vector3 d = spine - target;
    d.y = 0.0f;
    float limit;
    float len = d.Length_unk();
    if (len < 0.00001f) {
        d.Set_unk(0.0f, 0.0f, 1.0f);
        len = 0.1f;
    } else {
        d.Normalize_unk();
    }
    if (m58C == 16) {
        limit = 0.0f;
    } else {
        limit = 0.5f;
    }
    if (fn_801A03E8(d, fn_8014052C()) > limit) {
        if (len <= 40.0f + m14F0->m40) {
            if (wait == 0 || m14DC > m16AC) {
                if (m1630 >= 3) {
                    fn_802A2F48();
                    m1630 = 0;
                } else {
                    fn_802A2790();
                    m1630++;
                }
                m1634 = 0;
            }
        } else if (wait == 0 || m14DC > m16B0) {
            fn_802A325C();
            m1630 = 0;
            m1634 = 0;
        }
    } else {
        if (m1634 >= 1) {
            fn_802A2F48();
            m1634 = 0;
        } else {
            fn_802A2A68();
            m1634++;
        }
    }
}

int Em203GreaterDaemonRedMp::fn_8029F3F4() {
    return 1;
}

// inferred: state 6/7 (hovers to the route point m1570)
// NONMATCHING: 99.72% - r3/r4 swap on the psq copy into m1560 and two stack slots (as Red fn_80296DB8)
void Em203GreaterDaemonRedMp::fn_8029F3FC() {
    if (m14D8 == 0) {
        atn::Vector3 point = m1570->mPos;
        fn_801C3910(point - mPos);
        if (m1558 == m1570->m10) {
            m1560 = point - mPos;
            ToIdle_unk();
        } else {
            m155C += 1.0f / m168C;
            float t = 0.5f * (1.0f + (float)sin(3.1415927f * m155C - 1.5707964f));
            mPos = (m1570->mPos - m159C) * t + m159C;
            if (m155C >= 1.0f) {
                m1558 = m1570->m10;
            }
        }
    } else if (m14D8 == 1) {
        if (m14DC <= (int)m168C) {
            mPos += m1560 * (1.0f / m168C);
        } else {
            ToIdle_unk();
        }
    }
}

// inferred: result of fn_8003EA3C (as TashaDamage_unk of game/EnemyTasha.cpp); names invented
struct RedMpDamage_unk {
    RedMpDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlags31_28 = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags31_28 : 4;
    unsigned int mFlags27_0 : 28;
};
extern "C" void fn_8003EA3C(RedMpDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const void* param, int arg, int arg2); // inferred: computes the damage
extern "C" char lbl_807C6B90[0x18]; // inferred: work area of the damage computation

// NONMATCHING: 98.38% - stack slots of the direction and its temporary; m15F8 kept in r3 by the original
// inferred: state 9 (flare or damage circle: m14D8 0 chant, 1 casts)
void Em203GreaterDaemonRedMp::fn_8029F738() {
    if (m14D8 == 0) {
        fn_8016E970(6);
        m560 = 29;
        if (Chant_unk() && mMotion.mTracks[0].m08 >= 28.0f) {
            fn_8016E970(3);
            m14D8 = 1;
            PlayMotion_unk(0x145, 0, 0, 1.0f);
        }
    } else if (m14D8 == 1) {
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            ToIdle_unk();
        }
        if (mMotion.mTracks[0].m08 >= 31.0f && mMotion.mTracks[0].m08 < 32.0f) {
            HitInfo_unk* skill = m15F8;
            if (skill == m14F8) {
                atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("chin"));
                atn::Vector3 target = m14D4->mPos;
                if (m15F8 == m14FC) {
                    target = m1638;
                }
                atn::Vector3 dir = (target - pos).GetNormalize();
                mStatus.mParamB.m2C = 7.0f;
                m12CC = 0.0f;
                fn_800F8574(lbl_8087F048, this, m15F8, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
            } else if (skill == m14FC) {
                RedMpDamage_unk result;
                fn_8003EA3C(&result, m15F8, this, this, 0, lbl_807C6B90, 0, 0);
            }
        }
    }
}

// inferred: counts the chant frames of the skill in use (m15F8); nonzero when the chant is over
inline int Em203GreaterDaemonRedMp::Chant_unk() {
    if (m15F8 == 0) {
        return 1;
    }
    if (m15FC > m15F8->mC0) {
        return 1;
    }
    m15FC++;
    mFB8 = (float)m15FC;
    mFBC = (float)m15F8->mC0;
    return 0;
}

// inferred: state 10 (thunderbolt: m14D8 0 chant, 1 casts, 2 back to hovering)
void Em203GreaterDaemonRedMp::fn_8029FAC0() {
    if (m14D8 == 0) {
        fn_8016E970(6);
        m560 = 29;
        if (Chant_unk() && mMotion.mTracks[0].m08 >= 28.0f) {
            fn_8016E970(3);
            PlayMotion_unk(0x13F, 0, 0, 1.0f);
            m14D8 = 1;
        }
    } else if (m14D8 == 1) {
        float end = mMotion.mTracks[0].m08;
        if (end > mMotion.fn_80097D7C(0)) {
            m14D8 = 2;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.5f;
            mMotion.fn_80097C08(0, 2, 1, 0, 1, 1.0f, 0.1f);
            m14DC = 0;
        } else if (mMotion.mTracks[0].m08 >= 47.0f && mMotion.mTracks[0].m08 < 48.0f) {
            fn_802A3D80(0);
            m1660++;
        }
    } else if (m14D8 == 2) {
        ToIdle_unk();
    }
}

// inferred: unit vector rotated around y (name invented)
static inline atn::Vector3 Front_unk(const float& angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(angle);
    PSMTXMultVec(m.m, &front, &front);
    return front;
}

// inferred: camera shake (as in game/Em203GreaterDaemonRed.cpp)
inline void ShakeCamera_unk(int frames, float a, float b) {
    Field_unk* field = lbl_8087F430;
    field->m96C = field->m96C % 2;
    field->m970 = frames;
    field->m974 = a;
    field->m978 = b;
}

// inferred: starts state 13 (lands on the ground)
inline void Em203GreaterDaemonRedMp::StartState13_unk() {
    m58C = 13;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x153, 1, 0, 1.0f);
}

// inferred: state 11 (dives on the target)
void Em203GreaterDaemonRedMp::fn_802A057C() {
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    fn_801C3910(dir);

    if (m14D8 == 0) {
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            StartState13_unk();
            return;
        }
        if (m14DC <= 42) {
            float k = 0.023809524f;
            mPos += m1560 * k;
        }
        // inferred: the original tests m14EC twice (two `beq cr1`)
        if (m14EC != 0 && mMotion.mTracks[0].m08 < 41.0f && m14EC != 0 && mMotion.mTracks[0].m08 >= 40.0f) {
            ShakeCamera_unk(10, 1.0f, 2.0f);
        }
        if (m14EC != 0 && mMotion.mTracks[0].m08 < 44.0f) {
            atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
            fn_800FAB80(lbl_8087F048, this, m14EC, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
    }
}

// inferred: state 14 (turns to the target and strikes with the claws)
void Em203GreaterDaemonRedMp::fn_802A0818() {
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    fn_801C3910(dir);
    PSVECNormalize(&dir, &dir);
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartState13_unk();
        return;
    }
    if (mMotion.mTracks[0].m08 <= 20.0f) {
        mRot.y += atn::WrapAngle_unk(m1578 - m1574) / 20.0f;
    }
    if ((mMotion.mTracks[0].m08 > 36.0f && mMotion.mTracks[0].m08 < 40.0f) ||
               (mMotion.mTracks[0].m08 > 56.0f && mMotion.mTracks[0].m08 < 60.0f)) {
        atn::Vector3 spine = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine"));
        spine += Front_unk(mRot.y) * 35.0f;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, m14F0, m590, 30, -1, 0.0f);
    } else {
        m590 = fn_800F8548(lbl_8087F048);
    }
}

// inferred: state of the multiplayer copy (names invented)
struct RedMpState_unk {
    int m00;
    int m04;
    short m08;
    short m0A;
    int m0C;
};

// inferred: state 16 (turns on the ground; with m1594 also dashes from m157C to m1588)
void Em203GreaterDaemonRedMp::fn_802A0B00() {
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    float dist = dir.Length_unk();
    dir.Normalize_unk();
    float frame = mMotion.mTracks[0].m08;
    if (frame > 21.0f && frame <= 38.0f) {
        mRot.y += atn::WrapAngle_unk(m1578 - m1574) / 17.0f;
        if (m1594 != 0) {
            mPos = LerpV_unk(m157C, m1588, (frame - 21.0f) / 17.0f);
        } else if (dist < 85.0f) {
            mPos += dir * -85.0f * 0.2f;
        }
    } else {
        m1578 = dir.ToRotation_unk().y;
    }
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m1598 != 0) {
            fn_802A325C();
        } else {
            fn_8029F148(0);
        }
    }
}

// inferred: state 17 (breath on the ground: turns to the target, then starts the breath at frame 42)
void Em203GreaterDaemonRedMp::fn_802A118C() {
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartState13_unk();
        return;
    }
    if (mMotion.mTracks[0].m08 <= 28.0001f) {
        fn_8029DBE0(1.0f);
    }
    if (m15C8 == 0 && mMotion.mTracks[0].m08 > 42.0f) {
        m15C8 = 1;
        m15C4 = 33;
        m15CC = fn_800F8548(lbl_8087F048);
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("chin"));
        pos += Front_unk(mRot.y) * 20.0f;
        pos.y = 0.0f;
        m15D0 = pos;
        m15DC = mRot;
    }
}

// inferred: state 16 (rises; starts the battle BGM once above 30 frames)
void Em203GreaterDaemonRedMp::fn_802A16A4() {
    if (m1650 == 0 && mMotion.mTracks[0].m08 > 30.0f) {
        m1650 = 1;
        if (GameBgm::sInstance != 0) {
            GameBgm::sInstance->fn_8037D4C0(0x8B8, 15, 1, 0, 1.0f);
        }
    }
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        StartState13_unk();
    }
}

// inferred: state 15 (moves from m157C to m1588 in 54 frames, then hovers)
// NONMATCHING: 99.13% - float registers of the interpolation off by one
void Em203GreaterDaemonRedMp::fn_802A17CC() {
    float t = mMotion.mTracks[0].m08 / 54.0f;
    mPos = LerpV_unk(m157C, m1588, 1.0f < t ? 1.0f : t);
    if (mMotion.mTracks[0].m08 > 70.0f && mMotion.mTracks[0].m08 < 80.0f) {
        mMotion.mTracks[0].m08 = 190.0f;
    }
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        m165C = 1;
        ToIdle_unk();
    }
}

// NONMATCHING: 99.08% - r5/r6 of the inlined owner checks and the Vector3 copy crossing of fn_802A35B0
// inferred: state 13 (on the ground): flies up after GrandTime seconds unless the owner daemon is
// flying, otherwise attacks
void Em203GreaterDaemonRedMp::fn_802A19B0() {
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    fn_801C3910(dir);
    if (m1610 > m16A8 * 30 && (m1648 == 0 || !m1648->fn_802A49C8() || !m1648->CanAct_unk())) {
        m14E4 = 5;
        fn_802A35B0(5);
    } else {
        fn_8029F148(1);
    }
}

// inferred: state 14 (turns to the route point m1570 and flies there in 35 frames)
void Em203GreaterDaemonRedMp::fn_802A1BFC() {
    if (m14D8 == 0) {
        float end = mMotion.mTracks[0].m08;
        if (end > mMotion.fn_80097D7C(0)) {
            m14D8 = 1;
            PlayMotion_unk(0x13F, 0, 0, 1.0f);
            return;
        }
        if (m14DC > 25) {
            float k = 0.028571429f;
            mPos += m1560 * k;
            fn_8029DBE0(3.0f);
        }
        // inferred: block of the original left without effect (it tests m14EC twice, as in fn_802A057C,
        // and keeps the frame comparison)
        if (m14EC != 0 && mMotion.mTracks[0].m08 < 22.0f && m14EC != 0) {
            float frame = mMotion.mTracks[0].m08;
        }
    } else if (m14D8 == 1) {
        float end = mMotion.mTracks[0].m08;
        if (end > mMotion.fn_80097D7C(0)) {
            m14E4 = 1;
            ToIdle_unk();
        }
    }
}

// NONMATCHING: 98.2% - load order of the motion speed in the first clamp
// inferred: state 2 (dies: falls to the ground, then vanishes after 120 frames)
void Em203GreaterDaemonRedMp::fn_802A2070() {
    if (m14D8 == 0) {
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            if (m58C == 2) {
                m14D8 = 2;
                mPos.y = 0.0f;
                fn_80232B7C(0, 0);
                Color4f_unk color;
                color.r = 1.0f;
                color.g = 1.0f;
                color.b = 1.0f;
                color.a = 1.0f;
                fn_8023A8B4(lbl_8087F3C0, &m1668, 0, 0, mPos, mRot, color, -1, -1, 1, 2.5f);
            } else {
                m14D8 = 1;
                PlayMotion_unk(0x1E2, 1, 0, 1.0f);
            }
            m14DC = 0;
        }
        if (m58C == 2) {
            if (mMotion.mTracks[0].m08 >= 25.0f) {
                float speed = mMotion.mTracks[0].m0C + 0.05f;
                mMotion.mTracks[0].m0C = 1.0f < speed ? 1.0f : speed;
            } else if (mMotion.mTracks[0].m08 >= 8.0f) {
                float speed = mMotion.mTracks[0].m0C - 0.05f;
                mMotion.mTracks[0].m0C = 0.2f > speed ? 0.2f : speed;
            }
        }
        if (mMotion.mTracks[0].m08 < 36.0f && mMotion.mTracks[0].m08 >= 35.0f) {
            ShakeCamera_unk(10, 1.0f, 2.0f);
        }
        mPos.y += m1560.y / mMotion.fn_80097D7C(0);
        if (mPos.y < 0.0f) {
            mPos.y = 0.0f;
        }
    } else if (m14D8 == 1) {
        if (m14DC > m16A4 * 30) {
            StartState13_unk();
        }
    } else if (m14D8 == 2) {
        if (m14DC >= 120) {
            mColli.mFlags &= ~1;
            mFlag25 = 1;
            mFlag15 = 1;
            fn_800D246C(1);
            fn_801765D8();
            if (m1554 != 0) {
                Field_unk* field = lbl_8087F430;
                field->m8A0 = 0;
                field->m97C.m00 = 0;
                m1554 = 0;
            }
        }
    }
}

// inferred: starts the hovering to the route point m1570 (kind 0: state 7, 1: state 8)
void Em203GreaterDaemonRedMp::fn_802A239C(int kind) {
    if (kind == 0) {
        m58C = 7;
        m14E0 = 0;
    } else if (kind == 1) {
        m58C = 8;
        m14E0 = 1;
    }
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(2, 1, 0, 1.5f);
    m159C = mPos;
}

// inferred: starts state 9
void Em203GreaterDaemonRedMp::fn_802A2494() {
    m58C = 9;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x66, 1, 0, 1.5f);
}

// inferred: starts state 11
void Em203GreaterDaemonRedMp::fn_802A254C() {
    m58C = 11;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x13F, 0, 0, 1.0f);
}

// NONMATCHING: 99.8% - r5/r6 crossing on the psq copy into m1560 (known blocker, as Red fn_8029A788)
// inferred: starts state 12 (dives on the target, stopping 85 units before it)
void Em203GreaterDaemonRedMp::fn_802A2600() {
    m58C = 12;
    ResetState_unk();
    m1618 = 0;
    m1610 = 0;
    m1630 = 0;
    m1634 = 0;
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x15E, 0, 0, 1.0f);
    atn::Vector3 target = m14D4->mPos;
    target.y = 0.0f;
    m1560 = target - mPos;
    atn::Vector3 dir(m1560.x, 0.0f, m1560.z);
    dir.Normalize_unk();
    m1560 -= dir * 85.0f;
}

// inferred: starts state 15 (claw: turns from m1574 to the direction of the target m1578)
void Em203GreaterDaemonRedMp::fn_802A2790() {
    m58C = 15;
    ResetState_unk();
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x15F, 0, 0, 1.0f);
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    m1574 = mRot.y;
    m1578 = dir.ToRotation_unk().y;
}

// inferred: starts state 16 (turns to the target on the ground)
void Em203GreaterDaemonRedMp::fn_802A2A68() {
    m58C = 16;
    ResetState_unk();
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    float diff = atn::WrapAngle_unk(dir.ToRotation_unk().y - mRot.y);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    if (diff < 0.0f) {
        mMotion.fn_80097C08(0, 0x158, 0, 1, 1, 0.0f, 0.1f);
    } else {
        mMotion.fn_80097C08(0, 0x159, 0, 1, 1, 0.0f, 0.1f);
    }
    m1574 = mRot.y;
    m1578 = dir.ToRotation_unk().y;
    m1594 = 0;
    m1598 = 0;
}

// inferred: starts the turn of state 16 and a dash 200 units behind the centre point, away from the target
void Em203GreaterDaemonRedMp::fn_802A2F48() {
    fn_802A2A68();
    m157C = mPos;
    CharaTask* t = m14D4;
    atn::Vector3 dir = t->mPos - m1638;
    dir.y = 0.0f;
    if (dir.LengthSq_unk() > 0.000001f) {
        dir.Normalize_unk();
    } else {
        dir.Set_unk(0.0f, 0.0f, 1.0f);
    }
    m1588 = m1638 - dir * 200.0f;
    m1578 = atn::Vector3(-dir.x, -dir.y, -dir.z).ToRotation_unk().y;
    m1594 = 1;
    m1598 = 1;
}

// inferred: starts state 17 (breath on the ground)
void Em203GreaterDaemonRedMp::fn_802A325C() {
    m58C = 17;
    ResetState_unk();
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x160, 0, 0, 1.0f);
    atn::Vector3 target = m14D4->mPos;
    atn::Vector3 dir = target - mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    m1574 = mRot.y;
    m1578 = dir.ToRotation_unk().y;
}

// inferred: starts state 18
void Em203GreaterDaemonRedMp::fn_802A3520() {
    m58C = 18;
    ResetState_unk();
    PlayMotion_unk(0x14D, 0, 0, 1.0f);
}

// inferred: starts state 14 (turns towards the route point m1570)
// NONMATCHING: 99.69% - r4/r5 crossing of the Vector3 copy into m1560 (known blocker)
void Em203GreaterDaemonRedMp::fn_802A35B0(int kind) {
    ResetState_unk();
    m58C = 14;
    PlayMotion_unk(10, 0, 0, 1.0f);
    fn_8016E970(6);
    m560 = 4;
    fn_802A42CC();
    atn::Vector3 d = m1570->mPos - mPos;
    m1560 = d;
    m1610 = 0;
    m1630 = 0;
    m1634 = 0;
}

int Em203GreaterDaemonRedMp::fn_802A3D6C(HitInfo_unk* skill) {
    m15F8 = skill;
    return skill != 0;
}

// NONMATCHING: 99.29% - loop counter registers swapped (r29/r30, as Red fn_8029C564)
// inferred: the route point nearest to the daemon among the high hover points (m156C)
int Em203GreaterDaemonRedMp::fn_802A4194(int arg) {
    if (m15A8.size() == 0) {
        return 0;
    }
    float best = 1.0e16f;
    int nearest = -1;
    for (unsigned int i = 0; i < m15A8.size(); i++) {
        atn::Vector3 d = m15A8[i]->mPos - mPos;
        float dist = sqrt(d.x * d.x + d.z * d.z);
        if (dist < best) {
            best = dist;
            nearest = i;
        }
    }
    RouteTable_unk* table = lbl_8087F430->m10D8;
    int index = fn_803C1560(table, &m15A8[nearest]->mPos, fn_80179D44(), 0, 1, 1, -1.0f) - 1;
    m156C = &table->mPoints[index];
    return 1;
}

// inferred: picks the high hover point farthest from the target as the next route point (m1570);
// nonzero when it differs from the nearest one (m156C)
int Em203GreaterDaemonRedMp::fn_802A42CC() {
    if (m15A8.size() == 0) {
        return 0;
    }
    float best = 0.0f;
    int farthest = -1;
    RouteTable_unk* table;
    for (unsigned int i = 0; i < m15A8.size(); i++) {
        atn::Vector3 target = m14D4->mPos;
        atn::Vector3 d = m15A8[i]->mPos - target;
        float dist = sqrt(d.x * d.x + d.z * d.z);
        if (dist > best) {
            best = dist;
            farthest = i;
        }
    }
    if (farthest < 0 || farthest >= (int)m15A8.size()) {
        return 0;
    }
    fn_802A4194(1);
    table = lbl_8087F430->m10D8;
    int index = fn_803C1560(table, &m15A8[farthest]->mPos, fn_80179D44(), 0, 1, 1, -1.0f) - 1;
    m1570 = &table->mPoints[index];
    if (m156C == 0 || m1570 == 0) {
        return 0;
    }
    return m156C != m1570;
}

void Em203GreaterDaemonRedMp::vf110(SpecialState_unk* state) {
    RedMpState_unk* s = (RedMpState_unk*)state;
    s->m00 = m58C;
    s->m04 = m14BC;
    s->m08 = m1650;
    s->m0C = m1660;
}

void Em203GreaterDaemonRedMp::vf114(const SpecialState_unk* state) {
    const RedMpState_unk* s = (const RedMpState_unk*)state;
    if (s->m00 == 2 && m1674 == 0) {
        m1674 = 1;
        vf48();
        fn_80178A6C();
        fn_8015495C(0, 1, 1, 1);
        fn_8016DA4C();
        m58C = 2;
        ResetState_unk();
        fn_8016E970(6);
        m560 = 0x17;
        PlayMotion_unk(0x1E1, 0, 0, 1.0f);
        m1560 = m1638 - mPos;
    }
    if (m1650 == 0 && s->m08 != 0 && GameBgm::sInstance != 0) {
        GameBgm::sInstance->fn_8037D4C0(0x8B8, 15, 1, 0, 1.0f);
    }
    m58C = s->m00;
    m14BC = s->m04;
    m1650 = s->m08;
    m1660 = s->m0C;
}

void Em203GreaterDaemonRedMp::vfFC() {
    ToIdle_unk();
}

void Em203GreaterDaemonRedMp::vf100(int state) {}

// inferred: player manager accessors (weak copies 0x8000D9E8 and 0x802A36B0, as in gmk/Cannonball.cpp)
extern "C" inline CharaManager<PlayerTask>* fn_8000D9E8() {
    return lbl_8087F8A0;
}
extern "C" inline CharaTask* fn_802A36B0(CharaManager<PlayerTask>* mgr) {
    return mgr->mHead;
}

// inferred: update of the daemon controlled by this machine: picks the target and runs the state
void Em203GreaterDaemonRedMp::vf104() {
    CharaTask* player = fn_802A36B0(fn_8000D9E8());
    if (m15C8 != 0 && fn_802A36B8() != 0) {
        m15C8 = 0;
        m15C4 = 0;
    }
    m14D4 = mC58.mC4;
    if (m14D4 == 0) {
        m14D4 = player;
    }
    m14DC++;
    if (mC58.mC0 == 0 || m14D4 == 0) {
        fn_8016E970(3);
    } else if (m14D4 != 0) {
        switch (m58C) {
        case 7:
        case 8:
            fn_8029DBE0(2.0f);
            fn_8029F3FC();
            break;
        case 10:
            fn_8029FAC0();
            break;
        case 9:
            fn_8029DBE0(1.0f);
            fn_8029F738();
            break;
        case 11:
            fn_8029DBE0(1.0f);
            fn_8029FD34();
            break;
        case 12:
            fn_802A057C();
            break;
        case 15:
            fn_802A0818();
            m1610++;
            break;
        case 16:
            fn_802A0B00();
            m1610++;
            break;
        case 17:
            fn_802A118C();
            m1610++;
            break;
        case 18:
            fn_802A16A4();
            break;
        case 19:
            fn_802A17CC();
            break;
        case 13:
            fn_802A19B0();
            m1610++;
            break;
        case 14:
            fn_802A1BFC();
            break;
        case 2:
            fn_802A2070();
            m1610++;
            break;
        default:
            fn_8029DBE0(1.0f);
            fn_8029DB58();
            if (fn_8029F3F4() != 0) {
                fn_8029E950();
            }
            break;
        }
    }
    fn_8029DB54();
    if (m1688 != 0) {
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x54)) {
            fn_802A2600();
        }
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4C)) {
            fn_802A254C();
        }
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4D) && fn_802A3D6C(m14FC) != 0) {
            fn_802A2494();
        }
    }
}
