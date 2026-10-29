#ifndef GAME_EM203GREATERDAEMON_H
#define GAME_EM203GREATERDAEMON_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/Texture_unk.h"
#include "atn/Csv.h"
#include "game/EnemyTask.h"
#include "game/IntVector_unk.h"
#include "game/ResHandle.h"
#include "std/vector.h"

struct RoutePlace_unk;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
class PlayerTask;

// inferred: helpers shared by the three greater daemons (game/Em203GreaterDaemon.cpp,
// game/Em203GreaterDaemonRed.cpp, game/Em203GreaterDaemonRedMp.cpp). Names invented.

// inferred: list of route places, a class derived from std::vector (its destructor tests the address
// four times, as RoutePlaceList_unk of game/Em305Vampire.h)
struct DaemonPlaceList_unk : public std::vector<RoutePlace_unk*> {};

// inferred: two effect resources loaded together (0x18 bytes, implicit destructor)
struct DaemonResPair_unk {
    atn::EffFile m00;
    atn::EffFile m0C;
};

// inferred: nonzero while either resource of the pair is loading (weak copy 0x80288DD8)
extern "C" inline int fn_80288DD8(DaemonResPair_unk* pair) {
    int result = 0;
    if (pair->m00.IsLoading() != 0 || pair->m0C.IsLoading() != 0) {
        result = 1;
    }
    return result;
}

// inferred: sets CharaTask::m56C (weak copy 0x80288E30, also called by Em203GreaterDaemonRed,
// Em203GreaterDaemonRedMp, EnemyQuarkSecond and EnemyQuarkStand)
extern "C" inline void fn_80288E30(CharaTask* chara, float value) {
    chara->m56C = value;
}

// inferred: 0x74-byte entry of the hate lines (Em203GreaterDaemon::m1674): a direction, two matrices,
// a counter and a character. Names invented.
// inferred: zeroed matrix used to clear the hate lines (name invented)
struct DaemonZeroMtx_unk : public atn::Matrix3x4 {
    DaemonZeroMtx_unk() { memset(m, 0, sizeof(m)); }
};
struct DaemonLine_unk {
    DaemonLine_unk() : m6C(0), m70(0) {}
    // inferred: clears the line (name invented)
    void Clear_unk() {
        m0C = DaemonZeroMtx_unk();
        m3C = DaemonZeroMtx_unk();
        m70 = 0;
        m00.Assign_unk(lbl_807C7030);
        m6C = 0;
    }
    atn::Vector3 m00;
    atn::Matrix3x4 m0C; // inferred: matrix of the head of the line (at the character)
    atn::Matrix3x4 m3C; // inferred: matrix of the line (from the daemon, scaled by the length)
    int m6C;
    CharaTask* m70;
};

// inferred from __vt__18Em203GreaterDaemon (0x807857E0): the greater daemon (flying boss of the
// arena, enemy id 17). Data file "database/chara/special/" + db_name (default "db_em203_00_00.csv").
// The factory allocates 0x19D0 bytes (SpecialEnemyBase.cpp, case 17). Members keep their offsets
// (names invented).
class Em203GreaterDaemon : public EnemyTask {
public:
    Em203GreaterDaemon(atn::Task* parent, EnemyInfo_unk* info); // 0x80287E7C
    virtual ~Em203GreaterDaemon();                              // 0x802883A4
    virtual int vf0C();                                         // 0x802885EC
    virtual void vf10();                                        // 0x80288E38
    virtual void vf14();                                        // 0x80289974
    virtual void vf3C(HitInfo_unk* hit);                        // 0x80289978
    virtual void vf40(HitInfo_unk* hit);                        // 0x80289C98
    virtual int vfE4();                                         // 0x80289F98
    virtual void vf50();                                        // 0x8028A024
    virtual void vf54();                                        // 0x8028A5D4
    virtual atn::Vector3 vf64(int center);                      // 0x8028A97C
    virtual atn::Vector3 vf58();                                // 0x80291440
    // inferred: inline (weak copies at the end of the file, in reverse order)
    virtual float vf90() { return 5.0f; } // 0x80293730
    // inferred: position of the "neck" node (0x802936B4; the literal is in the .data pool of the
    // inline member)
    virtual atn::Vector3 GetTacViewPosition() const {
        MtxPtr m = mMotion.GetNodeMtx_unk("neck");
        // inferred: the original loads the translation from the last row up (z, y, x)
        float z = m[2][3];
        float y = m[1][3];
        float x = m[0][3];
        return atn::Vector3(x, y, z);
    }
    virtual float vfD4() { return -6.0f; } // 0x802936AC
    virtual float vfDC() { return 0.4f; }  // 0x802936A4
    virtual float vfE0();                  // 0x80293694
    virtual atn::Vector3 vfD8(CharaTask* target);        // 0x8029358C
    virtual atn::Vector3 vfD0(float angle);              // 0x8029350C
    virtual void vfCC(atn::Vector3* out, const atn::Vector3& dir); // 0x80293368

    inline float HpRate_unk();
    inline bool IsMotionNearEnd_unk(float margin);
    inline void ResetState_unk();
    inline void PlayMotion_unk(int motion, int loop, int arg, float speed);
    inline void FaceCenter_unk();
    inline void StartFly_unk();
    inline int CanFall_unk();
    inline void StartTackle_unk();
    inline void StartAction_unk(int state);
    void fn_8028AA38(const atn::Vector3* points, float t);
    void fn_8028ADF4();
    void fn_8028AFD8(int moving);
    void fn_8028B47C();
    void fn_8028B75C(int wait);
    void fn_8028BFE0();
    void fn_8028C834();
    void fn_8028CB38();
    void fn_8028CFE0();
    void fn_8028D5C8();
    void fn_8028DF34();
    void fn_8028E1A0();
    void fn_8028E488();
    void fn_8028E800();
    void fn_8028EDC4();
    void fn_8028EF20();
    void fn_8028F1D0();
    void fn_8028F4AC();
    void fn_8028F560();
    void fn_8028F8E4();
    void fn_8028F998();
    void fn_8028FA60();
    void fn_8028FB30();
    void fn_8028FC28();
    void fn_8028FDB0();
    void fn_80290088();
    void fn_8029055C();
    void fn_80290674();
    void fn_80290AB4();
    void fn_8029101C(PlayerTask* target, float angle);
    int fn_80291390(HitInfo_unk* skill);
    int fn_802913B0();
    void fn_802914F8(); // inferred: clears the hate lines
    void fn_802915F4(CharaTask* chara, int index, int kind);
    void fn_8029207C();
    void fn_802921F8();
    int fn_80292C9C();
    void fn_80292DB4();

    CharaTask* m14B0; // inferred: target
    int m14B4;        // inferred: step of the state
    int m14B8;        // inferred: frame counter of the state
    int m14BC;
    int m14C0;
    int m14C4;
    int m14C8;          // inferred: set once the hit points fall under 10% (vf40)
    HitInfo_unk* m14CC; // inferred: "LaserID"
    HitInfo_unk* m14D0; // inferred: "BreathID"
    HitInfo_unk* m14D4; // inferred: "FlareID"
    HitInfo_unk* m14D8; // inferred: "AbsorbSkill"
    HitInfo_unk* m14DC; // inferred: "TackleID"
    HitInfo_unk* m14E0; // inferred: "ClawID"
    HitInfo_unk* m14E4; // inferred: "SpecialBreathID"
    DaemonPlaceList_unk m14E8; // inferred: "HoverPoint"
    DaemonPlaceList_unk m14F4; // inferred: "FlyPoint"
    RoutePlace_unk* m1500;     // inferred: "CenterPoint"
    int m1504;                 // inferred: route points of the flight
    int m1508;
    int m150C;
    float m1510;
    float m1514;
    float m1518; // inferred: "FlyingDist"
    atn::Vector3 m151C;
    float m1528;
    RoutePlace_unk* m152C;
    RoutePlace_unk* m1530;
    float m1534;
    float m1538;
    int m153C;
    int m1540;
    int m1544;
    int m1548;
    int m154C;
    int m1550; // inferred: bullet ids
    int m1554;
    float m1558;
    atn::EfpFile m155C; // inferred: "/effect/hateline/ef_bs014a.efp"
    atn::EffFile m1568;      // inferred: "/effect/hateline/ef_bs012b_01.eff"
    atn::EfpFile m1574; // inferred: "ef_ev093.efp"
    atn::Matrix3x4 m1580;
    PlayerTask* m15B0; // inferred: next character of the grand attack
    float m15B4;
    atn::EfpFile m15B8; // inferred: "/effect/hateline/ef_bs012a.efp"
    atn::EfpFile m15C4;
    HitInfo_unk* m15D0;
    int m15D4;
    atn::EfpFile m15D8; // inferred: "/effect/ef_mb005c.efp"
    float m15E4;
    int m15E8;
    IntVector_unk m15EC[5]; // inferred: "1stStepSkill".."5thStepSkill"
    int m1628;
    int m162C;
    PlayerTask* m1630;        // inferred: "Arganan"
    atn::EfpFile m1634; // inferred: "/effect/ef_bs034c.efp"
    int m1640;
    atn::Texture_unk m1644;
    DaemonLine_unk m1674[6];
    DaemonResPair_unk m192C; // inferred: "/effect/hateline/hate_line_06a.eff", "hate_line_06b.eff"
    DaemonResPair_unk m1944; // inferred: "/effect/hateline/hate_line_06a.eff", "ef_ui003_01.eff"
    DaemonResPair_unk m195C; // inferred: "/effect/hateline/hate_line_03a.eff", "hate_line_03b.eff"
    int m1974;
    atn::Vector3 m1978; // inferred: centre of the arena (route place 1001)
    atn::LH_Binary m1984; // inferred: data file
    ResHandle m198C;      // inferred: debug menu group
    int m1990;            // inferred: "DebugDisp"
    float m1994;          // inferred: "FlySpeed"
    float m1998;          // inferred: "HoverSpeed"
    float m199C;          // inferred: "BulletSpeed"
    float m19A0;          // inferred: "LimitAngle"
    int m19A4;            // inferred: "AngryAttachWaitTime"
    int m19A8;            // inferred: "AbsorbTime"
    int m19AC;            // inferred: "AbsorbNum"
    int m19B0;            // inferred: "AbsorbValue"
    int m19B4;            // inferred: "StunTime"
    int m19B8;            // inferred: "StunAttackNum"
    int m19BC;            // inferred: "BreathExplodeDelay"
    int m19C0;            // inferred: "GrandTime"
    int m19C4;            // inferred: "GrandAttackTime"
    int m19C8;            // inferred: "GrandBreathTime"
    int m19CC;
};

#endif
