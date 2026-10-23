#include "game/Em204Mulue.h"
#include "atn/Csv.h"
#include "atn/ColliBase.h"
#include "game/CharaManager.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/Bullet.h"
#include "game/GameUI.h"
#include "math/Vec.h"
#include "macros.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
// inferred: debug menu helpers (as in game/CharaTask.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em302Kraken.cpp)
struct MulueMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" MulueMsgTable_unk* lbl_8087F1E4;
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: attack hit
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet
extern "C" Bullet* fn_801010A0(Effect_unk* mgr); // inferred: a free bullet
extern "C" void fn_8010EE78(Bullet* bullet, BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir,
                            CharaTask* owner, int id, int arg, float speed); // inferred: fires the bullet
// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (two flags at 0xC4/0xC8 are
// set around the spin effect, as in game/Em209SmallGolem.cpp)
struct EffectMgr_unk {
    unsigned char m00[0xC4];
    int mC4;
    int mC8;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" void fn_8004D124(void* world); // inferred: rebuilds the active shapes
extern "C" void fn_805A5078(const atn::Vector3* pos, const atn::Vector3* dir, int id, float radius, float angle);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: pad input (lbl_8087EF70, as in game/CampSaveLoad.cpp)
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" GameUI* lbl_8087F490;
extern "C" void fn_803E3384(GameUI* ui); // inferred: closes the button prompt
// inferred: point the camera looks at (lbl_8087F430 + 0x97C, as in game/Em302Kraken.cpp); names invented
struct MulueLook_unk {
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
    unsigned char m0000[0x8A0];
    CharaTask* m8A0; // inferred: character the camera follows
    unsigned char m8A4[0x97C - 0x8A4];
    MulueLook_unk m97C;
    unsigned char m9A4[0x10D8 - 0x9A4];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
// inferred: damage record sent to the effect manager (as in game/Em302Kraken.cpp)
struct MulueDamage_unk {
    MulueDamage_unk() : m04(0), m08(0), m0C(0), m10(0), m14(-1) {
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
extern "C" void fn_80108C10(Effect_unk* effect, MulueDamage_unk* damage, const atn::Vector3& pos, CharaTask* a,
                            CharaTask* chara, int b, int c);
extern "C" void fn_8003EA3C(MulueDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const void* param, int arg, int arg2); // inferred: computes the damage
struct HitParam_unk;
extern "C" HitParam_unk lbl_807C6B90;
// inferred: ray test against the map (as in game/Em208Queen.cpp)
struct MulueColl_unk {
    MulueColl_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: hit position
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28; // inferred: offset added to the hit position
    int m34;
    atn::ColliBase* m38; // inferred: object hit
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004ED34(void* world, MulueColl_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7, CharaTask* arg8); // inferred: ray test
// inferred: hint of the HUD (GameUI::m764) built as a temporary (as in game/Em209SmallGolem.cpp)
struct MulueUIHint_unk : public GameUIElem1C_unk {
    MulueUIHint_unk(int a, int b, int c) {
        m00 = a;
        m04 = b;
        m08 = c;
        m0C = 0;
        m10 = 0;
        m14 = 0;
        m18 = 1;
    }
};
// inferred: shot of a sting (BulletShot_unk with defaults; name invented)
struct MulueShot_unk : public BulletShot_unk {
    MulueShot_unk() {
        m00 = 2.0f;
        m04 = 0;
        m08 = 0;
        m10 = 0;
        m14 = 0.0f;
        m18 = 1.0f;
    }
};

// inferred: angle helpers (names invented; out of line copies right after fn_802A74A4, also used by
// game/Em403WarLion.cpp): wrap to [-180, 180] and radians to degrees
extern "C" inline float fn_802A7910(float deg) {
    float d = fmod(deg, 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}
extern "C" inline float fn_802A7964(float rad) {
    return 57.295776f * rad;
}

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: plays an effect on the model (as in game/Em302Kraken.cpp; names invented)
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

// inferred: the motion of track 0 reached its end (frame read before the length; name invented)
inline bool Em204Mulue::IsMotionEnd_unk() {
    float frame = mMotion.mTracks[0].m08;
    return frame >= mMotion.fn_80097D7C(0);
}
// inferred: starts state `state` (names invented)
inline void Em204Mulue::SetState_unk(int state) {
    m14C0 = 0;
    m58C = state;
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
}
// inferred: plays a motion on track 0 (as in game/Em208Queen.cpp)
inline void Em204Mulue::PlayMotion_unk(int motion, int loop, float speed) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
    mMotion.fn_80097C08(0, motion, loop, 0, 1, 0.0f, 0.1f);
}
inline void Em204Mulue::SetPartsFlag_unk() {
    m168C.mFlags |= 1;
    m16E4.mFlags |= 1;
    m173C.mFlags |= 1;
    m1794.mFlags |= 1;
    m17EC.mFlags |= 1;
}
inline void Em204Mulue::ClearPartsFlag_unk() {
    m168C.mFlags &= ~1;
    m16E4.mFlags &= ~1;
    m173C.mFlags &= ~1;
    m1794.mFlags &= ~1;
    m17EC.mFlags &= ~1;
}
inline void Em204Mulue::StopEffects_unk() {
    fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x67, 0);
}
// inferred: back to state 6 (idle)
inline void Em204Mulue::Rest_unk() {
    m58C = 6;
    fn_8016E970(3);
    StopEffects_unk();
    fn_800CB5C8(&m1538, 0, 0);
    SetPartsFlag_unk();
}
// inferred: releases the caught character when it is free again
inline void Em204Mulue::Release_unk() {
    if (m1644 != 0 && m1644->m560 != 17 && m1644->m55C == 6) {
        m1644->fn_8015AC48(-1);
        m1644 = 0;
        fn_80097CCC(&mMotion, 0, 0.0f);
        m14C8 = 0;
        Rest_unk();
    }
}
inline void Em204Mulue::Turn_unk() {
    mRot.y += m15EC != 0 ? 0.034906585f : -0.034906585f;
    m15E8 -= 1.0f;
}
// inferred: state 24 (the player thrusts the mulue)
inline void Em204Mulue::StartThrust_unk() {
    SetState_unk(24);
    PlayMotion_unk(0x3D, 0, 0.0f);
    mMotion.mTracks[0].m0C = 0.0f;
    if (m1854 != 0) {
        fn_80370AE4(lbl_8087F430, m1684, 1);
    }
}
inline void Em204Mulue::ShowMessage_unk(int id) {
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(id), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}
// inferred: turn toward `target` (frames and direction)
inline void Em204Mulue::SetTurn_unk(atn::Vector3 target) {
    atn::Vector3 d = target - mPos;
    atn::Vector3 rot = d.GetNormalized_unk().ToRotation_unk();
    float a = fn_802A7910(fn_802A7964(rot.y - mRot.y));
    if (a < 0.0f) {
        m15EC = 0;
        m15E8 = -a / 2.0f;
    } else {
        m15EC = 1;
        m15E8 = a / 2.0f;
    }
}

// NONMATCHING: draft
Em204Mulue::Em204Mulue(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(600), m14C0(0), m14C4(1), m14C8(0), m14CC(0), m14D0(0),
      m14D4(-1), m14D8(-1), m14DC(0.5f), m14E0(0), m14E4(0), m14E8(0), m14EC(0), m14F0(0), m14F4(0),
      m15D0(1e16f), m15E4(0), m15E8(0.0f), m15EC(1), m1644(0), m1648(1), m167C(0), m1680(100), m1684(101),
      m1688(0), m1850(0), m1854(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m1614.SetIdentity();
    m14FC.fn_8023780C("ef_sk015a.efp");
    m1508.fn_80237654("ef_sk002b.eff");
    m1514.fn_8023780C("ef_sk008d.efp");
    m1520.fn_8023780C("ef_uc011.efp");
    m152C.fn_8023780C("ef_cm009.efp");
    m1654.fn_8023780C("ef_sc012.efp");
    std::string dir("database/chara/special/");
    std::string name("db_em204_00_00.csv");
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
    m1844.vf0C(path.c_str());
}

Em204Mulue::~Em204Mulue() {}

// NONMATCHING: draft
int Em204Mulue::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m14FC.fn_80237874() == 0 &&
        m1508.fn_802376D0() == 0 && m1514.fn_80237874() == 0 && m1520.fn_80237874() == 0 &&
        m152C.fn_80237874() == 0 && m1654.fn_80237874() == 0 && m1844.fn_80473F50() == 0) {
        mColli.mFlags &= ~0x80000000;
        m168C.mFlags |= 3;
        m168C.m0C = (int)this;
        m16E4.mFlags |= 3;
        m16E4.m0C = (int)this;
        m173C.mFlags |= 3;
        m173C.m0C = (int)this;
        m1794.mFlags |= 3;
        m1794.m0C = (int)this;
        m17EC.mFlags |= 3;
        m17EC.m0C = (int)this;
        fn_802A7970();
        m153C.mSize = 0;
        if (m1844.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m1844.fn_80470580(), m1844.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "StaggTime") == 0) {
                    m164C = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "DownTime") == 0) {
                    m1660 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "MoveTime") == 0) {
                    m14BC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ReleaseDamage") == 0) {
                    m1650 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "TacklePoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m153C.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "SpinPoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m1580.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "TackleID") == 0) {
                    m1664 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BackID") == 0) {
                    m1668 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BiteID") == 0) {
                    m166C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "PressID") == 0) {
                    m1670 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "WaveID") == 0) {
                    m1674 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "StingID") == 0) {
                    m1678 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SpinTackleID") == 0) {
                    m167C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "Commentary") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m1854 = 1;
                    }
                } else if (strcmp(key, "StunFlag") == 0) {
                    m1680 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThrustFlag") == 0) {
                    m1684 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "HintFlag") == 0) {
                    m1688 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "MoveDistMax") == 0) {
                    m15D0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpinTackleHpRate") == 0) {
                    m14DC = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpinTackleMax") == 0) {
                    m14E4 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpinTackleInterval") == 0) {
                    m14EC = fn_800DC12C(csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        m54C |= 0x200;
        if (m1580.mSize != 0) {
            m15C4 = m1580.mData[0]->mPos;
        }
        GXColor c = mMotion.fn_80092954("em204_koura")->mColor;
        m15F4 = MulueColor_unk(c);
        void* group = m184C.Load("em204");
        fn_80087994(group, "DebugDisp", &m1850, 0, 0);
        fn_80087994(group, "Commentary", &m1854, 0, 0);
        fn_800874C8(group, "ToStaggTime", &m1858, 5, 100, 1, 0, 0);
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: draft
void Em204Mulue::vf10() {
    fn_8014C0B4(0x1B, 0x14);
    fn_8014C0B4(0x1C, 0x1C7);
    fn_8014C0B4(0x1D, 0x1C7);
    fn_8014C0B4(0x1E, 0x1C7);
    m50C = 0.7f;
    if ((m58C < 9 || m58C > 13) && m58C != 26) {
        m14B0 = mC58.mC4;
    }
    m14C0++;
    if (mC58.mC0 == 0 || m14B0 == 0) {
        fn_8016E970(3);
        fn_80097CCC(&mMotion, 0, 0.0f);
        PlayMotion_unk(2, 1, 1.0f);
        Rest_unk();
    } else {
        switch (m58C) {
        case 7:
            fn_802A820C();
            break;
        case 8:
            fn_802A8718();
            break;
        case 9:
            if (fn_80158BB4()) {
                SetPartsFlag_unk();
                fn_8004D124(lbl_8087EE98);
                Rest_unk();
            }
            if (mMotion.mTracks[0].m08 >= 23.0f && mMotion.mTracks[0].m08 <= 27.0f) {
                fn_800F8C6C(lbl_8087F048, 0, 0, this, m166C, m590, 30, -1, 0.0f);
            }
            break;
        case 10:
            fn_802A8BBC();
            break;
        case 11:
            if (IsMotionEnd_unk()) {
                SetState_unk(12);
                PlayMotion_unk(0x14C, 0, 0.5f);
            }
            Release_unk();
            break;
        case 12:
            fn_802A91D0();
            break;
        case 13:
            fn_802A94C8();
            break;
        case 14:
            fn_802A97C8();
            break;
        case 15:
            if (IsMotionEnd_unk()) {
                switch (m14F8) {
                case 0:
                    fn_802AB794(2);
                    break;
                case 2:
                    m14C0 = 0;
                    m14B8 = 0;
                    m58C = 18;
                    fn_8016E970(6);
                    m560 = 4;
                    PlayMotion_unk(0x140, 1, 1.0f);
                    ClearPartsFlag_unk();
                    m14F0 = 0;
                    m14D8 = -1;
                    m15D4 = m15C4;
                    break;
                default:
                    fn_802AB5E8();
                    break;
                }
            }
            break;
        case 16:
            fn_802AA534();
            break;
        case 17:
            if ((float)m14C0 > 35.0f) {
                fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
                if (m1648 != 1 || (float)m14C0 > 70.0f) {
                    fn_800CB5C8(&m1538, 0, 0);
                    SetState_unk(19);
                    PlayMotion_unk(0x141, 0, 1.0f);
                    break;
                }
            }
            if (m1648 > 0 && (float)m14C0 <= 35.0f) {
                fn_802A9ADC(m14C0 % 10 == 5, m1648 >= 1);
            }
            break;
        case 18:
            fn_802AA1B8();
            break;
        case 19:
            if (mMotion.mTracks[0].m08 >= 15.0f) {
                fn_802AB97C();
            }
            break;
        case 20:
            if (IsMotionEnd_unk()) {
                Rest_unk();
            }
            break;
        case 21:
            if (m14B8 == 0) {
                if (IsMotionEnd_unk()) {
                    PlayMotion_unk(0x37, 0, 0.5f);
                    m14B8 = 1;
                    m14C0 = 0;
                }
            } else if (m14B8 == 1 && m14C0 > m164C && (m1648 != 0 || m14C0 > m164C * 2)) {
                Rest_unk();
                m15E4 = 0;
            }
            break;
        case 22:
            if (IsMotionEnd_unk()) {
                SetState_unk(23);
                PlayMotion_unk(0x3B, 1, 1.0f);
                ShowMessage_unk(0xEF);
            }
            break;
        case 23:
            if (m14C0 > m1660) {
                SetState_unk(25);
                PlayMotion_unk(0x3E, 0, 1.0f);
            }
            break;
        case 24: {
            float frame = m14B4->mMotion.mTracks[0].m08;
            mMotion.mTracks[0].m08 = frame;
            if (frame >= mMotion.fn_80097D7C(0) || m14B4->m55C != 6 || m14B4->m560 != 75) {
                SetState_unk(25);
                PlayMotion_unk(0x3E, 0, 1.0f);
            }
            break;
        }
        case 25:
            if (IsMotionEnd_unk()) {
                Rest_unk();
                m15E4 = 0;
            }
            break;
        case 26:
            if (m15E8 < 0.0f) {
                Rest_unk();
            } else {
                Turn_unk();
            }
            break;
        case 27:
            vfEC();
            break;
        default: {
            if (m55C != 6 && m55C != 7) {
                fn_8016E970(3);
                fn_80170A20(m14B0, 0, -1.0f);
            }
            fn_802A711C();
            atn::Vector3 pos;
            pos = fn_801781B0();
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            fn_805A5078(&pos, &dir, 100, 1.2f * m5B0, 3.1415927f);
            fn_802A74A4();
            break;
        }
        }
    }
    m15E4++;
    fn_8014C540();
    fn_80145334();
    fn_802A7970();
    if (m58C >= 11 && m58C <= 13) {
        atn::Matrix3x4 m(mMotion.GetNodeMtx_unk("catchpoint"));
        atn::Vector3 t(m.m[0][3], m.m[1][3], m.m[2][3]);
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = 0.0f;
        atn::Vector3 s(1.0f / m540.x, 1.0f / m540.y, 1.0f / m540.z);
        Mtx sm;
        PSMTXScale(sm, s.x, s.y, s.z);
        Mtx r;
        PSMTXConcat(m.m, sm, r);
        m = atn::Matrix3x4(r);
        m1614 = m;
        m1614.m[0][3] = t.x;
        m1614.m[1][3] = t.y;
        m1614.m[2][3] = t.z;
    }
    if (m58C == 23) {
        PlayerTask* player = lbl_8087F8A0->mHead;
        atn::Vector3 d = mPos - player->mPos;
        if (PSVECMag(&d) < 50.0f && !player->mFlag6 && !player->mFlag31 &&
            (player->m55C != 6 || player->m560 == 4)) {
            GameUI* ui = lbl_8087F490;
            if (ui != 0) {
                ui->m764 = MulueUIHint_unk(6, 0x11, -1);
            }
            if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
                player->fn_8016BEDC(this, atn::Vector3(0.0f, 24.0f, -20.0f));
                m14B4 = player;
                StartThrust_unk();
                fn_800C31F4("SE_UICOM_011", 0, -1, 1.0f);
                if (lbl_8087F490 != 0) {
                    fn_803E3384(lbl_8087F490);
                }
            }
        }
    }
}

// NONMATCHING: draft
void Em204Mulue::vf14() {
    if (mMotion.fn_80092954("em204_koura") != 0) {
        MulueColor_unk target = m15F4;
        if (m58C == 18) {
            target = MulueColor_unk(0.95f, 0.1f, 0.1f, 1.0f);
        }
        m1604 = (target - m1604) * 0.1f + m1604;
        mMotion.fn_80092954("em204_koura")->mColor = m1604.ToGX();
    }
    fn_80149A30();
}

// NONMATCHING: draft
void Em204Mulue::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 22) {
        return;
    }
    msg->m10 = lbl_807C7030;
    msg->m50 |= 0x10;
    if (m58C != 21 && m58C != 24 && m58C != 23) {
        int weak = mStatus.IsState_unk(0x80000) || mStatus.IsState_unk(0x1000000);
        if (!weak) {
            msg->m48 = 1;
        }
    }
    if (msg->mData != 0 && msg->mData->m04 == 0xCC) {
        msg->m50 |= 0x80;
    }
    fn_80151448(msg);
    vf40(hit);
    if (msg->mData != 0 && msg->mData->m04 == 0xCC) {
        msg->m94 |= 0x80;
    }
}

// NONMATCHING: draft
void Em204Mulue::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->m68 != 0) {
        m14D0 += msg->m68;
        if (m58C == 21) {
            if (msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
                SetState_unk(22);
                fn_80239DAC(lbl_8087F3C0, this, 0x67, 0);
                PlayMotion_unk(0x38, 0, 1.0f);
                if (m1644 != 0) {
                    m1644->fn_8015AC48(-1);
                    m1644 = 0;
                }
                m1648++;
            }
        } else if (m58C == 11 || m58C == 12) {
            m14C8 += msg->m68;
            if (m14C8 > m1650 && m1644 != 0) {
                m1644->fn_8015AC48(-1);
                m1644 = 0;
                m14C8 = 0;
                fn_80097CCC(&mMotion, 0, 0.0f);
                Rest_unk();
            }
        }
        if (m14D0 >= mStatus.mParamB.m84 / 2 && m14C4 == 1) {
            m14C4 = 3;
            m14CC = 0;
        }
        if (mStatus.m04 <= 0.0f && (m58C == 12 || m58C == 10) && m1644 != 0) {
            m1644->fn_8015AC48(-1);
            m1644 = 0;
            m14C8 = 0;
        }
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802ABF6C();
    }
}

void Em204Mulue::vf7C(int arg) {}

// NONMATCHING: draft
void Em204Mulue::fn_802A711C() {
    float dist = 0.0f;
    float scale = 1.0f;
    atn::Vector3 rot(mRot);
    if (m55C == 7) {
        atn::Vector3 d = m14B0->mPos - mPos;
        dist = d.Length_unk();
        if (dist < 0.0001f) {
            rot = mRot;
        } else {
            rot = d.GetNormalized_unk().ToRotation_unk();
        }
        scale = 0.5f;
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    ClearPartsFlag_unk();
    fn_8004D124(lbl_8087EE98);
    fn_8013CB68(&rot, 1, dist, m568 * scale);
    SetPartsFlag_unk();
    fn_8004D124(lbl_8087EE98);
}

// NONMATCHING: draft
void Em204Mulue::fn_802A74A4() {
    if (m14B0 == 0) {
        return;
    }
    atn::Vector3 home = mPos - m15C4;
    if (home.Length_unk() >= m15D0) {
        m14F4 = 6;
        m15E4 = 0;
        fn_802AC0E0();
        return;
    }
    atn::Vector3 target(m14B0->mPos);
    atn::Vector3 d = target - mPos;
    float dist = d.Length_unk();
    atn::Vector3 rot = d.GetNormalized_unk().ToRotation_unk();
    float angle = fn_802A7910(fn_802A7964(rot.y - mRot.y));
    PlayerTask* player = lbl_8087F8A0->mHead;
    if (m15E4 >= 60 && (player->fn_801789D8(0x9C, 0) || player->fn_801789D8(0xA0, 0))) {
        if (fn_80370A78(lbl_8087F430, 0xDC) == 0) {
            fn_80370AE4(lbl_8087F430, 0xDC, 1);
        }
        m14F4 = 6;
        m15E4 = 0;
    } else if (m15E4 < m14BC) {
        switch (m14CC % 5) {
        case 0:
            if (70.0f < dist && dist < m15D0) {
                m14F4 = 2;
            } else {
                m14F4 = 1;
            }
            break;
        case 1:
        case 2:
        case 4:
            m14F4 = 1;
            break;
        case 3:
            m14F4 = 3;
            break;
        }
    } else {
        m14F4 = 6;
        m15E4 = 0;
    }
    fn_802AC0E0();
}

// NONMATCHING: draft
void Em204Mulue::fn_802A7970() {
    atn::Vector3 a;
    atn::Vector3 b;
    m614 = mPos + m5A4;
    m620 = m5B0;
    m614.y += m620;
    a = m614;
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    b.y -= 1.0f;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m620;
    a = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftforearm"));
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftmiddle1"));
    m168C.m3C = a;
    m168C.m48 = b;
    m168C.m54 = 7.0f;
    a = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightforearm"));
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightmiddle1"));
    m16E4.m3C = a;
    m16E4.m48 = b;
    m16E4.m54 = 7.0f;
    a = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftleg"));
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftfoot"));
    m173C.m3C = a;
    m173C.m48 = b;
    m173C.m54 = 5.0f;
    a = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightleg"));
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightfoot"));
    m1794.m3C = a;
    m1794.m48 = b;
    m1794.m54 = 5.0f;
    a = GetNodePos_unk(mMotion.GetNodeMtx_unk("joint2"));
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("joint4"));
    m17EC.m3C = a;
    m17EC.m48 = b;
    m17EC.m54 = 9.0f;
}

// NONMATCHING: r3/r4 of the copy of the center (__VecCopy of a temporary, docs/mwcc-blockers.md)
void Em204Mulue::vf54() {
    if (m624.mSize != 0) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        m624.mData[0].m04.mRadius = 1.5f * m5B0;
        m624.mData[0].m04.mCenter = pos + m5A4;
        m624.mData[0].m04.mCenter.y = mPos.y + m624.mData[0].m04.mRadius;
    }
}

// NONMATCHING: draft
atn::Vector3 Em204Mulue::vf64(int center) {
    atn::Vector3 r(mPos);
    if (center == 0) {
        r = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        r.y = mPos.y;
    } else {
        r = mPos;
    }
    return r;
}

int Em204Mulue::vf68(CharaTask* attacker) {
    return 0;
}

// NONMATCHING: draft
void Em204Mulue::vf6C(int kind, CharaTask* other) {
    if (kind == 2 && m58C != 24) {
        StartThrust_unk();
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802A820C() {
    if (m14B8 == 0) {
        if (m15E8 < 0.0f) {
            m14B8 = 1;
            fn_80232B7C(this, 0x65);
            PlayEffect_unk(&m14FC, &mMotion);
            m14C0 = 0;
        } else {
            Turn_unk();
        }
    } else if (m14B8 == 1) {
        float speed = 0.8f;
        atn::Vector3 d = m15D4 - mPos;
        d.y = 0.0f;
        float dist = d.Length_unk();
        d.Normalize_unk();
        d.fn_80139F60();
        if (dist < 10.0f) {
            mFlag4 = 0;
            Rest_unk();
        } else {
            mFlag4 = 1;
            ClearPartsFlag_unk();
            fn_8004D124(lbl_8087EE98);
            fn_801426A4(&d, dist, m568 * speed);
            SetPartsFlag_unk();
            fn_8004D124(lbl_8087EE98);
        }
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
        fn_800FAB80(lbl_8087F048, this, m1664, 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802A8718() {
    if (m14B8 == 0) {
        if (m15E8 < 0.0f) {
            m14B8 = 1;
            m14C0 = 0;
        } else {
            Turn_unk();
        }
    } else if (m14B8 == 1) {
        float speed = 0.8f;
        atn::Vector3 d = m15D4 - mPos;
        d.y = 0.0f;
        float dist = d.Length_unk();
        d.Normalize_unk();
        d.fn_80139F60();
        if (dist < 10.0f) {
            mFlag4 = 0;
            Rest_unk();
        } else {
            mFlag4 = 1;
            ClearPartsFlag_unk();
            fn_8004D124(lbl_8087EE98);
            fn_801426A4(&d, dist, m568 * speed);
            SetPartsFlag_unk();
            fn_8004D124(lbl_8087EE98);
        }
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        fn_800FAB80(lbl_8087F048, this, m1668, 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802A8BBC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        CharaTask* found = 0;
        float best = 1e16f;
        for (PlayerTask* c = lbl_8087F8A0->mHead; c != 0; c = c->mNext) {
            if (c->CanAct_unk()) {
                atn::Vector3 d = c->mPos - mPos;
                atn::Vector3 rot = d.GetNormalized_unk().ToRotation_unk();
                float angle = fn_802A7910(fn_802A7964(rot.y - mRot.y));
                float dist = fn_805F9920(&d);
                if (-50.0f < angle && angle < 50.0f && dist < 2500.0f) {
                    if (c == m14B0) {
                        found = m14B0;
                        break;
                    }
                    if (dist < best) {
                        found = c;
                        best = dist;
                    }
                }
            }
        }
        if (found != 0) {
            if (found->mStatus.IsState_unk(0x80)) {
                found->mStatus.fn_8013322C(0x80);
                found->fn_80166A54();
            }
            found->fn_801595BC((int)&m1614, 20, -1, -1);
            m1644 = found;
            m14B0 = found;
            SetState_unk(11);
            PlayMotion_unk(0x14B, 0, 0.5f);
            wchar_t text[256];
            memset(text, 0, sizeof(text));
            if (lbl_8087F8A0->mHead->m560 == 17) {
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xE5), *(const wchar_t**)((char*)m60 + 4));
                fn_80109828(lbl_8087F048, text);
            } else {
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xE7), *(const wchar_t**)((char*)m60 + 4));
                fn_80109828(lbl_8087F048, text);
            }
        } else {
            SetState_unk(20);
            if (m1644 != 0) {
                m1644->fn_8015AC48(-1);
                m1644 = 0;
            }
            PlayMotion_unk(0x149, 0, 0.5f);
        }
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802A91D0() {
    if (m14C0 > 160) {
        SetState_unk(13);
        PlayMotion_unk(0x148, 0, 0.5f);
        ClearPartsFlag_unk();
    }
    if (m1644 != 0 && m14C0 % 30 == 0) {
        int hp = (int)m1644->mStatus.m04;
        int power = mStatus.mParamB.m78 * 20;
        int dmg = MIN(power, hp - 1) < 0 ? 0 : MIN(power, hp - 1);
        m1644->mStatus.fn_8012DF7C(dmg, 0, 0);
        atn::Vector3 pos = m1644->fn_80148B0C(0)->m04.mCenter + atn::Vector3(0.0f, 15.0f, 0.0f);
        MulueDamage_unk damage;
        damage.m04 = dmg;
        fn_80108C10(lbl_8087F048, &damage, pos, this, m1644, 0, 0);
    }
    Release_unk();
}

// NONMATCHING: draft
void Em204Mulue::fn_802A94C8() {
    if (fn_80158BB4()) {
        Rest_unk();
    }
    if (m1644 != 0 && mMotion.mTracks[0].m08 > 31.0f && mMotion.mTracks[0].m08 <= 32.0f) {
        MulueDamage_unk damage;
        HitInfo_unk* skill = fn_80219E6C(0x4B4);
        fn_80144710();
        fn_8003EA3C(&damage, skill, this, m14B0, 0, &lbl_807C6B90, 0, 0);
        atn::Vector3 pos = m14B0->fn_80148B0C(0)->m04.mCenter + atn::Vector3(0.0f, 15.0f, 0.0f);
        fn_80108C10(lbl_8087F048, &damage, pos, this, m1644, 0, 0);
        atn::Vector3 dir(0.0f, 0.0f, 10.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &dir, &dir);
        atn::Vector3 from(mPos);
        from.y += 3.0f;
        atn::Vector3 to(m1644->mPos);
        to.y += 3.0f;
        MulueColl_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0, m1644) != 0 &&
            (result.m38->mFlags & 4) == 4) {
            result.m10 += result.m28;
            m1644->mPos = result.m10;
        }
        m1644->fn_8015A46C(dir);
        m1644 = 0;
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802A97C8() {
    if (m14B8 == 0) {
        if (m14C0 > 60) {
            PlayMotion_unk(3, 1, 1.0f);
            m14B8 = 1;
        } else {
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            atn::Vector3 v(atn::Vector3(0.0f, 0.0f, -0.25f));
            PSMTXMultVec(m, &v, &v);
            mPos += v;
        }
    } else if (m14B8 == 1) {
        if (m14C0 > 10) {
            PlayMotion_unk(0x145, 0, 0.5f);
            m14B8 = 2;
        }
    } else if (m14B8 == 2) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        if (fn_80158BB4()) {
            Rest_unk();
        }
        if (mMotion.mTracks[0].m08 >= 17.0f && mMotion.mTracks[0].m08 <= 19.0f) {
            fn_800FAB80(lbl_8087F048, this, m1670, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        if (mMotion.mTracks[0].m08 >= 20.0f && mMotion.mTracks[0].m08 <= 22.0f) {
            fn_800FAB80(lbl_8087F048, this, m1674, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
    }
}

// inferred: identity rotated by z, y and x (as QueenRotZYX_unk of game/Em208Queen.cpp; name invented)
inline void MulueRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802A9ADC(int aimed, int skipKind3) {
    if (m1678 == 0) {
        return;
    }
    atn::Matrix3x4 m;
    MulueRotZYX_unk(m, mRot);
    atn::Vector3 hips = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
    PlayerTask* player = ((PlayerManager*)lbl_8087F8A0)->fn_8054A33C((const char*)1);
    atn::Vector3 dir;
    if (player != 0) {
        // inferred: the original normalizes a shadowing local, `dir` stays unset
        atn::Vector3 pos(player->mPos);
        atn::Vector3 dir(pos.x - hips.x, 0.0f, pos.z - hips.z);
        dir.Normalize_unk();
    }
    if (aimed) {
        fn_800C344C("SE_EM204_014", &mPos, 0, -1, 1.0f);
        for (PlayerTask* c = lbl_8087F8A0->mHead; c != 0; c = c->mNext) {
            if (c->CanAct_unk() && !(skipKind3 != 0 && c->mType == 3)) {
                atn::Vector3 pos = GetNodePos_unk(c->mMotion.GetNodeMtx_unk("hips"));
                atn::Vector3 to = (pos - hips).GetNormalized_unk();
                Bullet* bullet = fn_801010A0(lbl_8087F048);
                if (bullet == 0) {
                    return;
                }
                MulueShot_unk shot;
                shot.m04 = (int)&m1508;
                shot.m08 = 0;
                shot.m0C = m1678;
                fn_8010EE78(bullet, &shot, &hips, &to, this, -1, 0, -1.0f);
            }
        }
    } else {
        for (int i = 0; i < m1678->m48; i++) {
            Bullet* bullet = fn_801010A0(lbl_8087F048);
            if (bullet == 0) {
                return;
            }
            float yaw = 0.5235988f * (mMotion.mTracks[0].m08 - 17.0f);
            atn::Vector3 v(0.0f, 0.0f, 1.0f);
            Mtx m1;
            PSMTXRotRad(m1, 'y', 0.5235988f * ((rand() % 1001) / 1000.0f));
            PSMTXMultVec(m1, &v, &v);
            Mtx m2;
            PSMTXRotRad(m2, 'z', 6.2831855f * ((rand() % 1001) / 1000.0f));
            PSMTXMultVec(m2, &v, &v);
            Mtx m3;
            PSMTXRotRad(m3, 'y', yaw);
            PSMTXMultVec(m3, &v, &v);
            PSMTXMultVec(m.m, &v, &v);
            if (player != 0) {
                atn::Vector3 h(v);
                h.y = 0.0f;
                h.Normalize_unk();
                if (PSVECDotProduct(&dir, &h) > 0.9659258f) {
                    continue;
                }
            }
            MulueShot_unk shot;
            shot.m04 = i % 3 == 1 ? (int)&m1508 : 0;
            shot.m08 = 0;
            shot.m0C = m1678;
            fn_8010EE78(bullet, &shot, &hips, &v, this, -1, 0, -1.0f);
        }
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802AA1B8() {
    atn::Vector3 d = m15D4 - mPos;
    d.Length_unk();
    if (fn_805F9920(&d) < 25.0f) {
        int done = 0;
        if (m14B8 == 0) {
            if (m14C0 > 60) {
                int n = m153C.mSize;
                m14C0 = 0;
                m14B8 = 1;
                if (n < 1) {
                    done = 1;
                } else {
                    int i = rand() % m153C.size();
                    m14D4 = i;
                    m14D8 = i;
                    m15D4 = m153C.mData[i]->mPos;
                }
            }
        } else if (m14D8 == -1) {
            if (++m14F0 >= 2) {
                done = 1;
            } else {
                int i = rand() % m153C.size();
                m14D4 = i;
                m14D8 = i;
                m15D4 = m153C.mData[i]->mPos;
            }
        } else {
            m14D8++;
            if (m153C.mSize <= m14D8) {
                m14D8 = 0;
            }
            if (m14D8 == m14D4) {
                m14D8 = -1;
            }
            if (m14D8 == -1) {
                m15D4 = m15C4;
            } else {
                m15D4 = m153C.mData[m14D8]->mPos;
            }
        }
        if (done) {
            fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
            fn_800CB5C8(&m1538, 0, 0);
            SetState_unk(19);
            PlayMotion_unk(0x141, 0, 1.0f);
        }
    } else {
        atn::Vector3 n = d.GetNormalized_unk();
        n.y = 0.0f;
        mPos += n * 10.0f;
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        fn_800FAB80(lbl_8087F048, this, m167C, 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
    fn_800CB6E4(&m1538, &mPos);
}

// NONMATCHING: draft
void Em204Mulue::fn_802AA534() {
    atn::Vector3 d = m15C4 - mPos;
    d.Length_unk();
    if (fn_805F9920(&d) < 100.0f) {
        if ((float)m14C0 > 35.0f) {
            fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
            if (m1648 != 1 || (float)m14C0 > 70.0f) {
                fn_800CB5C8(&m1538, 0, 0);
                SetState_unk(19);
                PlayMotion_unk(0x141, 0, 1.0f);
                PlayerTask* player = lbl_8087F8A0->mHead;
                if (player->fn_801789D8(0x9C, 0) || player->fn_801789D8(0xA0, 0)) {
                    if (fn_80370A78(lbl_8087F430, 0xDC) == 0) {
                        fn_80370AE4(lbl_8087F430, 0xDC, 1);
                    }
                }
                return;
            }
        }
    } else {
        atn::Vector3 n = d.GetNormalized_unk();
        n.y = 0.0f;
        mPos += n * 10.0f;
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        fn_800FAB80(lbl_8087F048, this, fn_80219E6C(0x4B5), 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
    if (m1648 > 0 && (float)m14C0 <= 35.0f) {
        fn_802A9ADC(m14C0 % 10 == 5, m1648 >= 1);
    }
    fn_800CB6E4(&m1538, &mPos);
}

// NONMATCHING: draft
void Em204Mulue::vfEC() {
    if (IsMotionEnd_unk()) {
        mColli.mFlags &= ~1;
        ClearPartsFlag_unk();
        mFlag25 = 1;
        if (m1434 > 0) {
            if (--m1434 == 30) {
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m1520, &mMotion);
                atn::SoundRef sound = fn_800C344C("SE_BTCOM_023", &mPos, 0, -1, 1.2f);
                fn_800CB688(&sound, 0, 0.5f);
                vf9C(1);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 55;
        if (mMotion.mTracks[0].m00 == 46) {
            if (mMotion.mTracks[0].m08 < 70.0f) {
                atn::Vector3 v(0.0f, 0.0f, -35.0f / mMotion.fn_80097D7C(0));
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &v, &v);
                mPos += v;
            }
            Field_unk* field = lbl_8087F430;
            if (mMotion.mTracks[0].m08 < 60.0f) {
                atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
                if (m15F0 == 0) {
                    field->m8A0 = this;
                    field->m97C.Set_unk(pos, false);
                    m15F0 = 1;
                } else {
                    field->m8A0 = this;
                    field->m97C.mPos = pos;
                }
            } else if (m15F0 != 0) {
                field->m8A0 = 0;
                field->m97C.m00 = 0;
                m15F0 = 0;
            }
        }
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802AABD8() {
    m14C0 = 0;
    m58C = 7;
    fn_8016E970(3);
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x14, 1, 3.0f);
    m14B8 = 0;
    SetTurn_unk(m15D4);
}

// NONMATCHING: draft
void Em204Mulue::fn_802AAF2C() {
    m14C0 = 0;
    m58C = 8;
    fn_8016E970(3);
    m590 = fn_800F8548(lbl_8087F048);
    PlayMotion_unk(0x1C7, 1, 3.0f);
    m14B8 = 0;
    SetTurn_unk(m15D4);
}

// NONMATCHING: draft
void Em204Mulue::fn_802AB280() {
    SetState_unk(9);
    PlayMotion_unk(0x156, 0, 0.75f);
    ClearPartsFlag_unk();
    fn_8004D124(lbl_8087EE98);
}

// NONMATCHING: draft
void Em204Mulue::fn_802AB354() {
    SetState_unk(10);
    PlayMotion_unk(0x14A, 0, 0.5f);
    ShowMessage_unk(0xE3);
}

// NONMATCHING: draft
void Em204Mulue::fn_802AB42C() {
    SetState_unk(14);
    PlayMotion_unk(0x1C7, 1, 0.5f);
    m14B8 = 0;
    ShowMessage_unk(0xE9);
}

// NONMATCHING: draft
void Em204Mulue::fn_802AB510(int kind) {
    SetState_unk(15);
    PlayMotion_unk(0x13F, 0, 1.0f);
    m1538 = fn_800C344C("SE_EM204_003", &mPos, 0, -1, 1.0f);
    m14F8 = kind;
}

// NONMATCHING: draft
void Em204Mulue::fn_802AB5E8() {
    m14C0 = 0;
    m58C = 17;
    fn_8016E970(6);
    m560 = 4;
    PlayMotion_unk(0x140, 1, 1.0f);
    if (m1648 > 0) {
        lbl_8087F3C0->mC4 = 1;
        lbl_8087F3C0->mC8 = 1;
        fn_80232B7C(this, 0x66);
        PlayEffect_unk(&m1514, &mMotion);
        lbl_8087F3C0->mC4 = 0;
        lbl_8087F3C0->mC8 = 0;
        ClearPartsFlag_unk();
        ShowMessage_unk(0xEB);
    }
}

// NONMATCHING: draft
void Em204Mulue::fn_802AB794(int arg) {
    m14C0 = 0;
    m58C = 16;
    fn_8016E970(6);
    m560 = 4;
    PlayMotion_unk(0x140, 1, 1.0f);
    if (m1648 > 0) {
        lbl_8087F3C0->mC4 = 1;
        lbl_8087F3C0->mC8 = 1;
        fn_80232B7C(this, 0x66);
        PlayEffect_unk(&m1514, &mMotion);
        lbl_8087F3C0->mC4 = 0;
        lbl_8087F3C0->mC8 = 0;
        ClearPartsFlag_unk();
        ShowMessage_unk(0xEB);
    }
    m15D4 = m1580.mData[rand() % m1580.size()]->mPos;
    m15E0 = 0;
}

// NONMATCHING: draft
void Em204Mulue::fn_802AB97C() {
    SetState_unk(21);
    fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
    fn_80232B7C(this, 0x67);
    {
        Color4f_unk color;
        atn::Vector3 pos;
        pos.x = 0.0f;
        pos.y = -8.0f;
        pos.z = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m1654, &mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, 10.0f);
    }
    PlayMotion_unk(0x35, 0, 0.03f);
    m14B8 = 0;
    fn_80370AE4(lbl_8087F430, 0x50, 2);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m152C, &mMotion);
    ShowMessage_unk(0xED);
    if (m1854 != 0) {
        fn_80370AE4(lbl_8087F430, m1680, 1);
    }
    if (m1688 != 0 && fn_80370A78(lbl_8087F430, m1688) == 0) {
        fn_80370AE4(lbl_8087F430, m1688, 1);
    }
    if (m1644 != 0) {
        m1644->fn_8015AC48(-1);
        m1644 = 0;
    }
    fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
    fn_800CB5C8(&m1538, 0, 0);
    SetPartsFlag_unk();
}

// NONMATCHING: draft
void Em204Mulue::fn_802ABC50(int arg) {
    SetState_unk(26);
    SetTurn_unk(m14B0->mPos);
}

// NONMATCHING: draft
void Em204Mulue::fn_802ABF6C() {
    int motion = 0x2E;
    m14C0 = 0;
    if (m58C == 24) {
        motion = 0x31;
    }
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 27;
    PlayMotion_unk(motion, 0, 1.0f);
    StopEffects_unk();
    fn_800CB5C8(&m1538, 0, 0);
    SetPartsFlag_unk();
    if (m1644 != 0) {
        m1644->fn_8015AC48(-1);
        m1644 = 0;
    }
    fn_800EB7A0(this);
}

// NONMATCHING: 99.81% - f30/f31 swapped for the distance and the yaw (9 variants of declaration
// order and helpers)
void Em204Mulue::fn_802AC0E0() {
    atn::Vector3 target(*m14B0->fn_8013C38C());
    atn::Vector3 d = target - mPos;
    float ry;
    float dist = d.Length_unk();
    ry = GetRot_unk()->y;
    atn::Vector3 rot = d.GetNormalized_unk().ToRotation_unk();
    float angle = fn_802A7910(fn_802A7964(rot.y - ry));
    int front = 0;
    if (angle <= 22.5f && angle >= -22.5f) {
        front = 1;
    }
    switch (m14F4) {
    case 1:
        if (dist < 40.0f) {
            if (front) {
                fn_802AB280();
                m14CC++;
            } else {
                fn_802ABC50(1);
            }
        }
        break;
    case 2:
        if (dist < 80.0f) {
            if (front) {
                fn_802AB354();
                m14CC++;
            } else {
                fn_802ABC50(1);
            }
        }
        break;
    case 3:
        if (dist < 70.0f) {
            if (front) {
                fn_802AB42C();
                m14CC++;
            } else {
                fn_802ABC50(1);
            }
        }
        break;
    case 7:
        fn_802AB510(1);
        break;
    case 4:
        fn_802AABD8();
        break;
    case 5:
        fn_802AAF2C();
        break;
    case 6:
        if (mStatus.m04 / (float)mStatus.mParamB.m84 < m14DC && m14E0 < m14E4 && m14E8 >= m14EC) {
            fn_802AB510(2);
            m14E8 = 0;
            m14E0++;
        } else {
            m14E8++;
            fn_802AB510(0);
        }
        break;
    }
}
