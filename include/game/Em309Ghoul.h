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
    virtual void vf14();
    virtual void vf20();
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf44();
    virtual void vf54();
    virtual atn::Vector3 vf64(int center);

    void fn_803057A8(); // inferred: reads the data file
    void fn_802FF6C4(); // inferred: picks the target (the party leader when m1524 reaches m16F8)
    int fn_80304B40();  // inferred: nonzero while the players stay grouped
    inline void Die_unk(int motion, int flag);
    void fn_80300654(); // inferred: starts state 1 (shows message 0x141)
    void fn_8030073C(); // inferred: starts state 6 (motion 0x145) facing the target
    void fn_803009F4(); // inferred: starts state 9 (shows message 0x145)
    void fn_80304718(); // inferred: update of the appearance (m14F0 0: rising from the coffin, 1: jump out)
    void fn_80300AD8(); // inferred: starts state 10 (motion 0x14A, message 0x147) keeping the state 11 rotation
    void fn_80300DF4(); // inferred: starts state 11 (motion 0x14A, effect m15A0)
    void fn_803010BC(); // inferred: starts state 12 (messages 0x149/0x14B)
    void fn_80301200(); // inferred: starts state 13

    unsigned char m14B0[0x14EC - 0x14B0];
    CharaTask* m14EC;         // inferred: target
    int m14F0;
    int m14F4;
    int m14F8;
    float m14FC;              // inferred: damage taken
    unsigned char m1500[0x150C - 0x1500];
    int m150C;
    unsigned char m1510[0x1518 - 0x1510];
    int m1518;
    unsigned char m151C[0x1524 - 0x151C];
    int m1524;                // inferred: frames the players stayed grouped
    atn::EfpHandle_unk m1528;
    atn::EfpHandle_unk m1534;
    atn::EfpHandle_unk m1540;
    ResLoader_unk m154C;
    atn::EfpHandle_unk m1558;
    atn::EfpHandle_unk m1564;
    atn::EfpHandle_unk m1570;
    atn::EfpHandle_unk m157C;
    ResLoader_unk m1588;
    atn::EfpHandle_unk m1594;
    atn::EfpHandle_unk m15A0;
    atn::SoundRef m15AC;
    int m15B0;
    unsigned char m15B4[0x15C4 - 0x15B4];
    unsigned char m15C4;
    int m15C8;
    atn::Vector3 m15CC;       // inferred: rotation when the state 11 started
    atn::Vector3 m15D8;       // inferred: debug line direction
    unsigned char m15E4[0x15EC - 0x15E4];
    int m15EC;
    int m15F0;
    unsigned char m15F4[0x1608 - 0x15F4];
    atn::Vector3 m1608;       // inferred: debug sphere center
    GimmickCommon* m1614;     // inferred: gimmick with id 0x80A
    unsigned char m1618[0x166C - 0x1618];
    chr::CharaData_unk* m166C; // inferred: passed to fn_8017A300 when the waiting time runs out
    chr::CharaData_unk* m1670; // inferred: passed to fn_8017A300
    atn::LH_Xsca m1674[2];
    unsigned char m1684;      // inferred: set at death (1 if the ghoul dies in state 12)
    float m1688;
    ResHandle m168C;          // inferred: debug menu group
    int m1690;                // inferred: "DebugDisp"
    unsigned char m1694[0x16BC - 0x1694];
    float m16BC;              // inferred: z of the beam direction of state 10
    unsigned char m16C0[0x16DC - 0x16C0];
    int m16DC;                // inferred: seconds of the wait (x30 frames)
    float m16E0;              // inferred: debug sphere radius
    unsigned char m16E4[0x16EC - 0x16E4];
    float m16EC;              // inferred: radius of the damage spheres
    float m16F0;              // inferred: height offset of the damage spheres
    unsigned char m16F4[0x16F8 - 0x16F4];
    int m16F8;                // inferred: frames before the leader becomes the target
    unsigned char m16FC[0x1700 - 0x16FC];
};

#endif
