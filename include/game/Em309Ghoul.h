#ifndef GAME_EM309GHOUL_H
#define GAME_EM309GHOUL_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Xsca.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

class GimmickCommon;

// inferred from __vt__10Em309Ghoul: ghoul. The factory allocates 0x1700 bytes (SpecialEnemyBase.cpp).
// Members keep their offsets (names unknown).
class Em309Ghoul : public EnemyTask {
public:
    Em309Ghoul(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em309Ghoul();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf44();
    virtual void vf50();
    virtual void vf54();
    virtual atn::Vector3 vf64(int center);
    virtual void vfEC();

    void fn_803057A8(); // inferred: debug menu entries
    void fn_802FF6C4(); // inferred: picks the target (the party leader when m1524 reaches m16F8)
    int fn_80304B40();  // inferred: nonzero while the players stay grouped
    inline void Die_unk(int motion, int flag);
    void fn_80300654(); // inferred: starts state 1 (shows message 0x141)
    void fn_8030073C(); // inferred: starts state 6 (motion 0x145) facing the target
    void fn_803009F4(); // inferred: starts state 9 (shows message 0x145)
    void fn_80302B84(); // inferred: update of state 7 (throws bullets 0x64E at the leader, frames 32-45)
    void fn_80304718(); // inferred: update of the appearance (m14F0 0: rising from the coffin, 1: jump out)
    void fn_80300AD8(); // inferred: starts state 10 (motion 0x14A, message 0x147) keeping the state 11 rotation
    void fn_80300DF4(); // inferred: starts state 11 (motion 0x14A, effect m15A0)
    void fn_803010BC(); // inferred: starts state 12 (messages 0x149/0x14B)
    void fn_80301200(); // inferred: starts state 13
    void fn_80301DDC(); // inferred: update of state 6
    void fn_803026F0(); // inferred: update of state 8
    void fn_80302F94(); // inferred: update of state 10
    void fn_803041C8(); // inferred: update of state 11
    void fn_80304FC0(); // inferred: update while m15B8 is set
    void fn_802FF800(); // inferred: chooses the next state (states without update)
    void fn_802FFFF8();
    inline void ResetState_unk();
    inline void StartAttack_unk(); // inferred: name invented // inferred: back to state 0 (name invented)

    unsigned char m14B0[0x14C4 - 0x14B0];
    int m14C4[2][2];          // inferred: place ids 0x321-0x324 of the four corners
    int m14D4[6];             // inferred: place ids 0x44D-0x452
    CharaTask* m14EC;         // inferred: target
    int m14F0;
    int m14F4;
    int m14F8;
    float m14FC;              // inferred: damage taken
    int m1500[3];             // inferred: place ids "base1="-"base3="
    RoutePlace_unk* m150C[3]; // inferred: places of m1500 (m150C[0]: home of the ghoul)
    RoutePlace_unk* m1518;    // inferred: copy of m150C[0] (debug sphere of the home place)
    int m151C;
    int m1520;                // inferred: frames with a control running
    int m1524;                // inferred: frames the players stayed grouped
    atn::EfpFile m1528;
    atn::EfpFile m1534;
    atn::EfpFile m1540;
    atn::EffFile m154C;
    atn::EfpFile m1558;
    atn::EfpFile m1564;
    atn::EfpFile m1570;
    atn::EfpFile m157C;
    atn::EffFile m1588;
    atn::EfpFile m1594;
    atn::EfpFile m15A0;
    atn::SoundRef m15AC;
    int m15B0;
    int m15B4;
    unsigned char m15B8;      // inferred: runs fn_80304FC0 every frame
    int m15BC;
    unsigned char m15C0[0x15C4 - 0x15C0];
    unsigned char m15C4;
    int m15C8;
    atn::Vector3 m15CC;       // inferred: rotation when the state 11 started
    atn::Vector3 m15D8;       // inferred: debug line direction
    int m15E4;
    unsigned char m15E8[0x15EC - 0x15E8];
    int m15EC;
    int m15F0;
    atn::Vector3 m15F4;       // inferred: debug line offset from m1618
    RoutePlace_unk* m1600;    // inferred: place 0x385
    RoutePlace_unk* m1604;    // inferred: place 0x450
    atn::Vector3 m1608;       // inferred: debug sphere center
    GimmickCommon* m1614;     // inferred: gimmick with id 0x80A
    atn::Vector3 m1618;       // inferred: debug sphere and line start
    int m1624;
    int m1628;
    unsigned char m162C[0x1638 - 0x162C];
    atn::Vector2_unk m1638[2][2]; // inferred: x/z of the corners m14C4
    CharaTask* m1658;         // inferred: leader seen across the segments m1638 (fn_80304B40)
    unsigned char m165C[0x1668 - 0x165C];
    unsigned char m1668;      // inferred: draws the home place sphere
    unsigned char m1669;
    unsigned char m166A;      // inferred: hides the debug sphere of m1608
    unsigned char m166B;
    chr::CharaData_unk* m166C; // inferred: passed to fn_8017A300 when the waiting time runs out
    chr::CharaData_unk* m1670; // inferred: passed to fn_8017A300
    atn::LH_Xsca m1674[2];
    unsigned char m1684;      // inferred: set at death (1 if the ghoul dies in state 12)
    float m1688;
    ResHandle m168C;          // inferred: debug menu group
    int m1690;                // inferred: "DebugDisp"
    float m1694;              // inferred: "BaseRadius"
    int m1698;                // inferred: "WaitTime"
    int m169C;                // inferred: "StayTime"
    int m16A0;                // inferred: "ChargeTime"
    float m16A4;              // inferred: "ShakeRot"
    float m16A8;              // inferred: "ShakeGap"
    float m16AC;              // inferred: "ShakeLength"
    int m16B0;                // inferred: "AllDirCharaNum"
    float m16B4;
    float m16B8;              // inferred: "AttackLenge"
    float m16BC;              // inferred: "DamageRollingSpeed" (z of the beam direction of state 10)
    float m16C0;              // inferred: "DamageRollingRot"
    float m16C4;              // inferred: "AttackRollingSpeed"
    float m16C8;              // inferred: "AttackRollingRot"
    int m16CC;                // inferred: "AttackKey"
    int m16D0;                // inferred: "RollTime"
    int m16D4;                // inferred: "AttackTime"
    int m16D8;                // inferred: "RollWait"
    int m16DC;                // inferred: "FitTime": seconds of the wait (x30 frames)
    float m16E0;              // inferred: "ColliRadius" (debug sphere radius)
    int m16E4;                // inferred: "NoHitFrame"
    int m16E8;                // inferred: "HitLag"
    float m16EC;              // inferred: "headSpeareRadius": radius of the damage spheres
    float m16F0;              // inferred: "headSpeareOfs": height offset of the damage spheres
    float m16F4;              // inferred: "JumpTgtOfsetY": height subtracted from the hand when aiming
    int m16F8;                // inferred: frames before the leader becomes the target
    unsigned char m16FC[0x1700 - 0x16FC];
};

#endif
