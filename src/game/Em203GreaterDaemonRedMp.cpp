#include "game/Em203GreaterDaemonRedMp.h"
#include "atn/Csv.h"
#include "atn/SoundRef.h"
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

int Em203GreaterDaemonRedMp::vf10C(HitMsg_unk* msg) {
    msg->m10 = lbl_807C7030;
    msg->m50 |= 0x10;
    return 1;
}

int Em203GreaterDaemonRedMp::fn_8029F3F4() {
    return 1;
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

int Em203GreaterDaemonRedMp::fn_802A3D6C(HitInfo_unk* skill) {
    m15F8 = skill;
    return skill != 0;
}

// inferred: state of the multiplayer copy (names invented)
struct RedMpState_unk {
    int m00;
    int m04;
    short m08;
    short m0A;
    int m0C;
};

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
