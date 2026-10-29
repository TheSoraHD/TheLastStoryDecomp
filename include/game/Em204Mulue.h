#ifndef GAME_EM204MULUE_H
#define GAME_EM204MULUE_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"
#include "std/string.h"
#include "std/vector.h"

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
    inline void MoveBack_unk();

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
    atn::EfpFile m14FC; // inferred: "ef_sk015a.efp"
    atn::EffFile m1508;      // inferred: "ef_sk002b.eff" (stings)
    atn::EfpFile m1514; // inferred: "ef_sk008d.efp"
    atn::EfpFile m1520; // inferred: "ef_uc011.efp"
    atn::EfpFile m152C; // inferred: "ef_cm009.efp"
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
    atn::EfpFile m1654; // inferred: "ef_sc012.efp"
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

// inferred: "Collision" line of the data file of Em204BigMulue: a capsule between two nodes and its
// radius (scaled by the collision radius m5B0); names invented
struct BigMulueCapsule_unk {
    BigMulueCapsule_unk() {}
    std::string mFrom;
    std::string mTo;
    float mRadius;
};

// inferred: "DamageSphere" line: a damage sphere on a node (m14 is the node position of the frame);
// names invented
struct BigMulueSphere_unk {
    BigMulueSphere_unk() {}
    std::string mNode;
    float mScale;
    float m10;
    atn::Vector3 m14;
};

// inferred: notify flag of the data file (global flag, index; same layout as SpecialFlag_unk of
// game/SpecialEnemyBase.cpp); names invented
struct BigMulueFlag_unk {
    BigMulueFlag_unk() : mGlobal(-1), mIndex(0) {}
    int mGlobal;
    int mIndex;
};

// inferred from __vt__13Em204BigMulue (0x80785F0C): the big mulue, driven by the SpecialEnemyBase state
// machine. Data file "/database/chara/special/" + db_name (default "db_em204_01_00.csv"). The factory
// allocates 0x1AD0 bytes (SpecialEnemyBase.cpp, case 21). Members keep their offsets (names invented).
class Em204BigMulue : public SpecialEnemyBase {
public:
    Em204BigMulue(atn::Task* parent, EnemyInfo_unk* info); // 0x802AC364
    virtual ~Em204BigMulue();                              // 0x802B21FC
    virtual int vf0C();                                    // 0x802AC590
    virtual void vf10();                                   // 0x802AD930
    virtual void vf14();                                   // 0x802ADE54
    virtual int vf10C(HitMsg_unk* msg);                    // 0x802ADE58
    virtual void vf40(HitInfo_unk* hit);                   // 0x802ADF34
    virtual void vfFC();                                   // 0x802AE070
    virtual void vf50();                                   // 0x802AE0B8
    virtual void vf54();                                   // 0x802AE558
    virtual void vf20();                                   // 0x802AE674
    virtual atn::Vector3 vf64(int center);                 // 0x802AEAC4
    virtual void vf100(int state);                         // 0x802AEB08
    virtual void vf104();                                  // 0x802AF264
    virtual void vf108();                                  // 0x802AF7D8
    virtual int vfB0(atn::Vector3* out);                   // 0x802B12C4
    virtual boost::function<void()> vfB4(CharaTask* chara); // 0x802B12CC
    virtual void vf30(void* menu);                         // 0x802B1F2C
    virtual float vf90();                                  // 0x802B21F4

    void fn_802AC8E8(const char* text, int length); // inferred: reads the data file
    void fn_802AE954(); // inferred: breaks the gimmicks touched by the damage spheres
    int fn_802B0430(int state, float* over); // inferred: 1 too far from the target, -1 a wall in front
    int fn_802B07F0(int range); // inferred: moves around the target point m1500
    int fn_802B0D90(atn::Vector3* out); // inferred: finds the center of the room (four rays)
    int fn_802B12D8(const atn::Vector3* target, float rate, float limit, float step); // inferred: turns
    void fn_802B1744(int motion, int loop, float speed, float blend); // inferred: plays a motion
    void fn_802B17AC(float dist, float frames, float speed); // inferred: moves forward
    void fn_802B19EC(int kind); // inferred: plays an effect
    void fn_802B1B08(int on);   // inferred: enables the part capsules
    void fn_802B1B50(int fire); // inferred: fires the stings

    int m14D4;        // inferred: angry (hit points <= 40%)
    int m14D8;        // inferred: copy of the state
    int m14DC;        // inferred: step of the next state
    int m14E0;        // inferred: state after the turn (9)
    int m14E4;
    unsigned char m14E8; // inferred: the battle event was started
    CharaTask* m14EC; // inferred: target when state 8 starts
    int m14F0;
    int m14F4;
    int m14F8;
    float m14FC;      // inferred: "AttackDistance"
    atn::Vector3 m1500; // inferred: position of the target
    int m150C;        // inferred: direction of the move (-1, 0, 1)
    int m1510;        // inferred: "WaitFrame"
    int m1514;        // inferred: "AttackChanceFrame"
    int m1518;        // inferred: "TumbleFrame"
    int m151C;        // inferred: "StaggerFrame"
    int m1520;
    float m1524;      // inferred: "BaseMotionSpeed"
    float m1528;      // inferred: "BaseMotionBlendSpeed"
    HitInfo_unk* m152C; // inferred: "BiteID"
    HitInfo_unk* m1530; // inferred: "PressID"
    HitInfo_unk* m1534; // inferred: "WaveID"
    HitInfo_unk* m1538; // inferred: "TackleID"
    HitInfo_unk* m153C; // inferred: "CollisionID"
    HitInfo_unk* m1540; // inferred: "StingID"
    HitInfo_unk* m1544; // inferred: "WallSlashID"
    int m1548;        // inferred: attacks done
    float m154C;      // inferred: "TumbleDamageRate"
    float m1550;      // inferred: damage counted for the tumble
    int m1554;
    std::vector<BigMulueCapsule_unk> m1558; // inferred: "Collision"
    atn::ColliCapsule m1564[14];            // inferred: one capsule per "Collision" line
    std::vector<BigMulueSphere_unk> m1A34;  // inferred: "DamageSphere"
    unsigned char m1A40;
    int m1A44;        // inferred: "CollisionOnOff"
    atn::Vector3 m1A48; // inferred: position of the player when it thrusts
    atn::EfpFile m1A54;
    atn::EfpFile m1A60; // inferred: "/effect/ef_sk008d.efp"
    atn::EffFile m1A6C;      // inferred: "/effect/ef_sk002b.eff" (stings)
    atn::EfpFile m1A78; // inferred: "/effect/ef_uc011.efp"
    BigMulueFlag_unk m1A84;   // inferred: "StaggerNotifyFlag"
    BigMulueFlag_unk m1A8C;   // inferred: "TumbleNotifyFlag"
    float m1A94;      // inferred: "BodyPressOffset"
    ResHandle m1A98;  // inferred: debug menu group
    void* m1A9C;      // inferred: debug menu group (copy)
    int m1AA0;        // inferred: "DebugDisp"
    int m1AA4;        // inferred: "DebugLog"
    atn::Vector3 m1AA8; // inferred: last wall hit
    atn::Vector3 m1AB4; // inferred: "WallSlashPos"
    int m1AC0;
    atn::Vector3 m1AC4; // inferred: "CenterPos"
};

// inferred: color of a material of Em204MulueMp (MulueColor_unk without the white default: the
// constructor of Em204MulueMp does not initialize its colors); names invented
struct MulueMpColor_unk {
    MulueMpColor_unk() {}
    MulueMpColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    MulueMpColor_unk operator-(const MulueMpColor_unk& o) const {
        return MulueMpColor_unk(r - o.r, g - o.g, b - o.b, a - o.a);
    }
    MulueMpColor_unk operator+(const MulueMpColor_unk& o) const {
        return MulueMpColor_unk(r + o.r, g + o.g, b + o.b, a + o.a);
    }
    MulueMpColor_unk operator*(float f) const { return MulueMpColor_unk(r * f, g * f, b * f, a * f); }
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

// inferred: colors of a material of Em204MulueMp per mode (normal, fire, ice; table lbl_807860A0 filled by
// the static initializer 0x802B753C); names invented
struct MulueMpMaterial_unk {
    const char* mName;
    MulueMpColor_unk mColor[3];
};

// inferred: state saved by Em204MulueMp::vf110 (its own layout over SpecialState_unk); names invented
struct MulueMpState_unk {
    int m00;   // inferred: state (m58C)
    int m04;   // inferred: step (m14BC)
    short m08; // inferred: mode (m1518)
    short m0A; // inferred: part capsules enabled (m151C)
    short m0C; // inferred: battle music started (m1608)
};

// inferred from __vt__12Em204MulueMp (0x80786204): the mulue of the multiplayer mode, driven by the
// SpecialEnemyBase state machine. Data file "database/chara/special/" + db_name (default
// "db_em204_02_00.csv"). The factory allocates 0x1860 bytes (SpecialEnemyBase.cpp, case 22). Members keep
// their offsets (names invented).
class Em204MulueMp : public SpecialEnemyBase {
public:
    Em204MulueMp(atn::Task* parent, EnemyInfo_unk* info); // 0x802B23E8
    virtual ~Em204MulueMp();                              // 0x802B73FC
    virtual int vf0C();                                   // 0x802B28BC
    virtual void vf10();                                  // 0x802B2F64
    virtual void vf14();                                  // 0x802B3248
    virtual void vf3C(HitInfo_unk* hit);                  // 0x802B3498
    virtual void vf40(HitInfo_unk* hit);                  // 0x802B3534
    virtual void vfFC();                                  // 0x802B3684
    virtual void vf104();                                 // 0x802B3764
    virtual void vf100(int state);                        // 0x802B43C0
    virtual void vf108();                                 // 0x802B43C4
    virtual void vf54();                                  // 0x802B5134
    virtual atn::Vector3 vf64(int center);                // 0x802B5214
    virtual void vfEC();                                  // 0x802B6758
    virtual void vf110(SpecialState_unk* state);          // 0x802B7230
    virtual void vf114(const SpecialState_unk* state);    // 0x802B725C

    void fn_802B43C8(); // inferred: moves toward the target
    void fn_802B475C(); // inferred: chooses the next action (m1510)
    void fn_802B4B08(); // inferred: updates the collision capsules from the nodes
    void fn_802B52D8(); // inferred: state 8 (wave)
    void fn_802B55B0(int aimed, int arg); // inferred: fires a sting at every player of the session
    void fn_802B596C(); // inferred: state 12 (ice from above the home)
    void fn_802B5DA4(); // inferred: state 13 (spin tackle along the tackle points)
    void fn_802B6104(); // inferred: state 14 (spin tackle, then fire or ice from above the home)
    void fn_802B6928(); // inferred: starts state 7 (bite)
    void fn_802B69C0(); // inferred: starts state 8
    void fn_802B6A5C(int kind); // inferred: starts state 9
    void fn_802B6AF8(int arg);  // inferred: starts state 10 (spin tackle along the spin points)
    void fn_802B6C64(); // inferred: starts state 16 (turn to the target)
    void fn_802B6F80(); // inferred: starts the action m1510

    inline bool IsMotionEnd_unk();
    inline void SetState_unk(int state);
    inline void PlayMotion_unk(int motion, int loop, float speed);
    inline void SetPartsFlag_unk();
    inline void ClearPartsFlag_unk();
    inline void StopEffects_unk();
    inline void Rest_unk();
    inline void Die_unk();
    inline void SetMode_unk(int mode);
    inline void StartSpin_unk(int state);
    inline void MoveBack_unk();
    inline void SetTurn_unk(atn::Vector3 target);

    CharaTask* m14D4; // inferred: target (mC58.mC4)
    int m14D8;
    int m14DC;        // inferred: step of the state
    int m14E0;        // inferred: frame counter of the state
    int m14E4;        // inferred: 1, then 3 once half of the hit points are lost
    int m14E8;        // inferred: count of actions
    int m14EC;        // inferred: damage taken
    int m14F0;        // inferred: first tackle point
    int m14F4;        // inferred: current tackle point (-1: back to m15D4)
    float m14F8;      // inferred: "SpinTackleHpRate"
    int m14FC;        // inferred: spin tackles since the last change of mode
    int m1500;        // inferred: "SpinTackleMax"
    int m1504;        // inferred: actions of kind 3
    int m1508;        // inferred: "SpinTackleInterval"
    int m150C;        // inferred: laps of the spin tackle
    int m1510;        // inferred: next action (fn_802B6F80)
    int m1514;        // inferred: kind of state 9
    int m1518;        // inferred: mode (0 normal, 1 fire, 2 ice): material colors and spin attacks
    int m151C;        // inferred: the part capsules are enabled
    int m1520;        // inferred: "MoveTime"
    atn::EffFile m1524;      // inferred: "ef_sk002b.eff" (stings)
    atn::EfpFile m1530; // inferred: "ef_sk008d.efp"
    atn::EfpFile m153C; // inferred: "ef_uc011.efp"
    atn::SoundRef m1548;      // inferred: "SE_EM204_003" while spinning
    MulueRoute_unk m154C;     // inferred: "TacklePoint"
    MulueRoute_unk m1590;     // inferred: "SpinPoint"
    atn::Vector3 m15D4;       // inferred: home (first spin point)
    float m15E0;              // inferred: "MoveDistMax"
    atn::Vector3 m15E4;       // inferred: destination
    int m15F0;
    int m15F4;
    int m15F8;                // inferred: frame counter of the actions
    float m15FC;              // inferred: frames of turn left
    int m1600;                // inferred: turn direction (1: left)
    int m1604;                // inferred: 0x4650
    int m1608;                // inferred: the battle music was started
    MulueMpColor_unk m160C[6]; // inferred: current colors of the materials
    HitInfo_unk* m166C;       // inferred: "TackleID"
    HitInfo_unk* m1670;       // inferred: "BackID"
    HitInfo_unk* m1674;       // inferred: "BiteID"
    HitInfo_unk* m1678;       // inferred: "PressID"
    HitInfo_unk* m167C;       // inferred: "WaveID"
    HitInfo_unk* m1680;       // inferred: "StingID"
    HitInfo_unk* m1684;       // inferred: "SpinTackleID"
    HitInfo_unk* m1688;       // inferred: "SpinIceID"
    HitInfo_unk* m168C;       // inferred: "SpinIceWeakID"
    HitInfo_unk* m1690;       // inferred: "SpinFrareID"
    HitInfo_unk* m1694;       // inferred: "SpinFrareWeakID"
    atn::ColliCapsule m1698;  // inferred: left arm
    atn::ColliCapsule m16F0;  // inferred: right arm
    atn::ColliCapsule m1748;  // inferred: left leg
    atn::ColliCapsule m17A0;  // inferred: right leg
    atn::ColliCapsule m17F8;  // inferred: tail
    atn::LH_Binary m1850;     // inferred: data file
    ResHandle m1858;          // inferred: debug menu group
    int m185C;                // inferred: "DebugDisp"
};

#endif
