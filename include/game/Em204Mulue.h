#ifndef GAME_EM204MULUE_H
#define GAME_EM204MULUE_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

struct RoutePlace_unk;

// inferred: route places read from "TacklePoint"/"SpinPoint" (count at 0, up to 16 places; the
// count is compared as a signed int and used unsigned for rand() %); names invented
struct MulueRoute_unk {
    MulueRoute_unk() : mSize(0) {}
    unsigned int size() const { return mSize; }
    void push_back(RoutePlace_unk* const& place) {
        new (&mData[mSize]) RoutePlace_unk*(place);
        mSize++;
    }
    int mSize;
    RoutePlace_unk* mData[16];
};

// inferred: color of the shell material "em204_koura" (r, g, b, a; starts white); names invented
struct MulueColor_unk {
    MulueColor_unk() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    MulueColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    MulueColor_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    MulueColor_unk operator-(const MulueColor_unk& o) const {
        return MulueColor_unk(r - o.r, g - o.g, b - o.b, a - o.a);
    }
    MulueColor_unk operator+(const MulueColor_unk& o) const {
        return MulueColor_unk(r + o.r, g + o.g, b + o.b, a + o.a);
    }
    MulueColor_unk operator*(float f) const { return MulueColor_unk(r * f, g * f, b * f, a * f); }
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

// inferred from __vt__10Em204Mulue (0x80785D28): the mulue (single player). Data file
// "database/chara/special/" + db_name (default "db_em204_00_00.csv"). The factory allocates 0x1860
// bytes (SpecialEnemyBase.cpp, case 20). Five capsules follow the arms, legs and tail (m168C-m17EC).
// Members keep their offsets (names invented).
class Em204Mulue : public EnemyTask {
public:
    Em204Mulue(atn::Task* parent, EnemyInfo_unk* info); // 0x802A4BBC
    virtual ~Em204Mulue();                              // 0x802A5054
    virtual int vf0C();                                 // 0x802A51F4
    virtual void vf10();                                // 0x802A5A1C
    virtual void vf14();                                // 0x802A6B90
    virtual void vf3C(HitInfo_unk* hit);                // 0x802A6D58
    virtual void vf40(HitInfo_unk* hit);                // 0x802A6E80
    virtual void vf7C(int arg);                         // 0x802A7118
    virtual void vf54();                                // 0x802A7F9C
    virtual atn::Vector3 vf64(int center);              // 0x802A807C
    virtual int vf68(CharaTask* attacker);              // 0x802A8140
    virtual void vf6C(int kind, CharaTask* other);      // 0x802A8148
    virtual void vfEC();                                // 0x802AA89C

    void fn_802A711C(); // inferred: moves toward the target
    void fn_802A74A4(); // inferred: chooses the next action (m14F4)
    void fn_802A7970(); // inferred: updates the collision capsules from the nodes
    void fn_802A820C(); // inferred: state 7 (turn, then tackle)
    void fn_802A8718(); // inferred: state 8 (turn, then back attack)
    void fn_802A8BBC(); // inferred: state 10 (catches the nearest character in front)
    void fn_802A91D0(); // inferred: state 12 (holds the caught character)
    void fn_802A94C8(); // inferred: state 13 (throws the caught character)
    void fn_802A97C8(); // inferred: state 14 (wave)
    void fn_802A9ADC(int aimed, int skipKind3); // inferred: fires the stings
    void fn_802AA1B8(); // inferred: state 18 (spin tackle along the spin points)
    void fn_802AA534(); // inferred: state 16 (spin tackle back to the first point)
    void fn_802AABD8(); // inferred: starts state 7
    void fn_802AAF2C(); // inferred: starts state 8
    void fn_802AB280(); // inferred: starts state 9 (bite)
    void fn_802AB354(); // inferred: starts state 10 (press)
    void fn_802AB42C(); // inferred: starts state 14
    void fn_802AB510(int kind); // inferred: starts state 15
    void fn_802AB5E8(); // inferred: starts state 17
    void fn_802AB794(int arg); // inferred: starts state 16
    void fn_802AB97C(); // inferred: starts state 21 (down)
    void fn_802ABC50(int arg); // inferred: starts state 26 (turn to the target)
    void fn_802ABF6C(); // inferred: death
    void fn_802AC0E0(); // inferred: starts the action m14F4

    inline bool IsMotionEnd_unk();
    inline void SetState_unk(int state);
    inline void PlayMotion_unk(int motion, int loop, float speed);
    inline void SetPartsFlag_unk();
    inline void ClearPartsFlag_unk();
    inline void StopEffects_unk();
    inline void Rest_unk();
    inline void Release_unk();
    inline void Turn_unk();
    inline void StartThrust_unk();
    inline void ShowMessage_unk(int id);
    inline void SetTurn_unk(atn::Vector3 target);

    CharaTask* m14B0; // inferred: target (mC58.mC4)
    CharaTask* m14B4; // inferred: character that thrusts (state 24)
    int m14B8;        // inferred: step of the state
    int m14BC;        // inferred: "MoveTime"
    int m14C0;        // inferred: frame counter of the state
    int m14C4;
    int m14C8;        // inferred: damage taken while holding (state 11/12)
    int m14CC;        // inferred: count of actions
    int m14D0;        // inferred: damage taken
    int m14D4;        // inferred: first spin point
    int m14D8;        // inferred: current spin point (-1: back to m15C4)
    float m14DC;      // inferred: "SpinTackleHpRate"
    int m14E0;        // inferred: spin tackles done
    int m14E4;        // inferred: "SpinTackleMax"
    int m14E8;        // inferred: actions since the last spin tackle
    int m14EC;        // inferred: "SpinTackleInterval"
    int m14F0;        // inferred: laps of the spin tackle
    int m14F4;        // inferred: next action (fn_802AC0E0)
    int m14F8;        // inferred: kind of state 15
    atn::EfpHandle_unk m14FC; // inferred: "ef_sk015a.efp"
    ResLoader_unk m1508;      // inferred: "ef_sk002b.eff" (stings)
    atn::EfpHandle_unk m1514; // inferred: "ef_sk008d.efp"
    atn::EfpHandle_unk m1520; // inferred: "ef_uc011.efp"
    atn::EfpHandle_unk m152C; // inferred: "ef_cm009.efp"
    atn::SoundRef m1538;
    MulueRoute_unk m153C;     // inferred: "TacklePoint"
    MulueRoute_unk m1580;     // inferred: "SpinPoint"
    atn::Vector3 m15C4;       // inferred: home (first spin point)
    float m15D0;              // inferred: "MoveDistMax"
    atn::Vector3 m15D4;       // inferred: destination
    int m15E0;
    int m15E4;                // inferred: frame counter
    float m15E8;              // inferred: frames of turn left
    int m15EC;                // inferred: turn direction (1: left)
    int m15F0;                // inferred: the camera looks at the mulue (vfEC)
    MulueColor_unk m15F4;     // inferred: color of the shell material
    MulueColor_unk m1604;     // inferred: current color of the shell material
    atn::Matrix3x4 m1614;     // inferred: matrix of "catchpoint" (followed by the caught character)
    CharaTask* m1644;         // inferred: caught character
    int m1648;
    int m164C;                // inferred: "StaggTime"
    int m1650;                // inferred: "ReleaseDamage"
    atn::EfpHandle_unk m1654; // inferred: "ef_sc012.efp"
    int m1660;                // inferred: "DownTime"
    HitInfo_unk* m1664;       // inferred: "TackleID"
    HitInfo_unk* m1668;       // inferred: "BackID"
    HitInfo_unk* m166C;       // inferred: "BiteID"
    HitInfo_unk* m1670;       // inferred: "PressID"
    HitInfo_unk* m1674;       // inferred: "WaveID"
    HitInfo_unk* m1678;       // inferred: "StingID"
    HitInfo_unk* m167C;       // inferred: "SpinTackleID"
    int m1680;                // inferred: "StunFlag"
    int m1684;                // inferred: "ThrustFlag"
    int m1688;                // inferred: "HintFlag"
    atn::ColliCapsule m168C;  // inferred: left arm
    atn::ColliCapsule m16E4;  // inferred: right arm
    atn::ColliCapsule m173C;  // inferred: left leg
    atn::ColliCapsule m1794;  // inferred: right leg
    atn::ColliCapsule m17EC;  // inferred: tail
    atn::LH_Binary m1844;     // inferred: data file
    ResHandle m184C;          // inferred: debug menu group
    int m1850;                // inferred: "DebugDisp"
    int m1854;                // inferred: "Commentary"
    int m1858;                // inferred: "ToStaggTime"
    int m185C;
};

#endif
