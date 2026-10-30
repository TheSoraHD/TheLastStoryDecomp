#ifndef GAME_EM401_01_WHITELEOPARD_H
#define GAME_EM401_01_WHITELEOPARD_H

#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__21Em401_01_WhiteLeoPard (0x80788A10): white leopard. The factory allocates 0x15A8
// bytes (SpecialEnemyBase.cpp, case 47). Members keep their offsets (names unknown).
class Em401_01_WhiteLeoPard : public EnemyTask {
public:
    Em401_01_WhiteLeoPard(atn::Task* parent, const EnemyInfo_unk* info);
    virtual ~Em401_01_WhiteLeoPard();
    virtual int vf0C();                  // 0x8031ECDC
    virtual void vf10();                 // 0x8031F654
    virtual void vf14();                 // 0x8031F810
    virtual void vf20();                 // 0x80321BCC
    virtual void vf24();                 // 0x8032130C
    virtual void vf34(const atn::Vector3& rot, int arg, float a, float b); // 0x803209D0
    virtual void vf3C(HitInfo_unk* hit); // 0x8031FBFC
    virtual void vf50();                 // 0x80321E20
    virtual void vf54();                 // 0x8032268C

    void fn_8031FAA4(); // inferred: picks the target (the leader or a held player)
    void fn_8031FD34(HitMsg_unk* msg); // inferred: reaction to a hit
    void fn_8031FE4C(); // inferred: state machine (m58C)
    inline void LookAt_unk(CharaTask* target);
    void fn_803202E0(); // inferred: moves towards the target or along the route (states 0, 1, 3, 4)
    void fn_803209E0(const atn::Vector3& rot, int arg, float speed, float rate); // inferred: as Em403WarLion::fn_80324A38
    void fn_DebugMenu_unk(); // inferred: removed by the linker (only its strings are left)
    void fn_803227A0(); // inferred: starts the jump (state 7)
    void fn_80322AD4(); // inferred: state 7: jump
    void fn_80321724(atn::MotionBuffer* nodes, int count); // inferred: node pose callback
    void fn_80321900(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk); // inferred: node matrix callback

    unsigned char m14B0[0x14C0 - 0x14B0];
    atn::Vector3 m14C0; // inferred: y is the lean of the waist
    unsigned char m14CC[0x14F4 - 0x14CC];
    float m14F4;
    float m14F8;
    float m14FC;
    float m1500;
    float m1504;
    int m1508;
    int m150C;
    int m1510;
    int m1514;
    float m1518;
    float m151C;
    float m1520;
    float m1524;
    float m1528;
    float m152C;
    unsigned char m1530[0x155C - 0x1530];
    int m155C; // inferred: frame counter (0-119) of the glow
    int m1560;
    int m1564;
    int m1568; // inferred: route place to run to (9999 = none)
    int m156C; // inferred: the leader had mFlag28 (field flag 0x3C)
    int m1570;          // inferred: frames left of the jump
    CharaTask* m1574;   // inferred: target of the jump
    float m1578;        // inferred: distance of the jump
    atn::Vector3 m157C; // inferred: direction of the jump (y is the vertical speed)
    float m1588;        // inferred: vertical speed
    ResHandle m158C; // inferred: debug menu group
    int m1590;       // inferred: "DebugDisp"
    int m1594;
    float m1598;
    int m159C;
    int m15A0;
    int m15A4;
};

#endif
