#ifndef GAME_EM316LASTLASER_H
#define GAME_EM316LASTLASER_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred from __vt__10LaserTrace (0x807889C0): trail of the laser, a list of effect owners
// (allocated with new, released by the effect manager) and a looping sound. Names invented.
struct LaserPoint_unk;
// inferred: a class derived from std::vector (its destructor tests `this` four times, as
// CharaList_unk of game/Em002_01BlackSkelton.h); name invented
struct LaserList_unk : public std::vector<LaserPoint_unk*> {
    LaserPoint_unk** Back_unk(); // 0x8031E060
    void Erase_unk(LaserPoint_unk** it); // inferred: erase of one element (memmove of the rest)
};

class LaserTrace {
public:
    LaserTrace();          // 0x8031D4F4
    virtual ~LaserTrace(); // 0x8031D57C
    inline void Reset_unk();

    int m04;
    int m08;
    int m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    LaserList_unk m28;
    LaserPoint_unk* m34;
    atn::SoundRef m38;
};

// inferred from __vt__14Em316LastLaser (0x80788890): laser enemy. Data file
// "database/chara/special/" + db_name (default "db_em601_00_00.csv"). Only the small functions
// are written (game/Em316LastLaser.cpp). Names invented.
class Em316LastLaser : public EnemyTask {
public:
    Em316LastLaser(atn::Task* parent, EnemyInfo_unk* info); // 0x8031B0A8
    virtual ~Em316LastLaser();                               // 0x8031B3CC
    virtual int vf0C();                                      // 0x8031B6E0
    virtual void vf10();                                     // 0x8031B9B4
    virtual void vf14();                                     // 0x8031C530
    virtual void vf3C(HitInfo_unk* hit);                     // 0x8031C534
    virtual void vf40(HitInfo_unk* hit);                     // 0x8031C584
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset); // 0x8031C754
    virtual void vfEC();                                     // 0x8031C894

    void fn_8031D27C(); // inferred: resets the step and takes a new bullet id
    void fn_8031D2BC(); // inferred: back to state 0, stops the effects 1000, 0x65 and 0x66

    atn::LH_Binary m14B0;
    int m14B8;
    int m14BC; // inferred: step of the state
    int m14C0; // inferred: frame counter
    int m14C4;
    atn::Vector3 m14C8;
    atn::Vector3 m14D4;
    atn::Vector3 m14E0;
    float m14EC; // inferred: "AppearMoveRate"
    atn::Vector3 m14F0;
    float m14FC;
    atn::Vector3 m1500;
    atn::Vector3 m150C;
    atn::Vector3 m1518;
    atn::Vector3 m1524;
    int m1530;
    int m1534;
    LaserTrace m1538;
    ResLoader_unk m1574; // inferred: "ef_sk043f_01.eff"
    ResLoader_unk m1580; // inferred: "ef_sk043c_01.eff"
    atn::EfpHandle_unk m158C;
    atn::EfpHandle_unk m1598;
    atn::EfpHandle_unk m15A4;
    atn::EfpHandle_unk m15B0;
    ResHandle m15BC; // inferred: debug menu group
    int m15C0;       // inferred: "Bit"
    int m15C4;       // inferred: "DebugDisp"
    int m15C8;
    int m15CC;
};

#endif
