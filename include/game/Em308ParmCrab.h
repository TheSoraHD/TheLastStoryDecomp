#ifndef GAME_EM308PARMCRAB_H
#define GAME_EM308PARMCRAB_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as ChaserRouteList_unk in
// game/ChaserGuard.h); name invented
struct CrabList_unk : public std::vector<int> {};

// inferred from __vt__13Em308ParmCrab (0x80787A00). The factory allocates 0x1620 bytes
// (SpecialEnemyBase.cpp). Members keep their offsets (names unknown).
class Em308ParmCrab : public EnemyTask {
public:
    Em308ParmCrab(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em308ParmCrab();
    virtual void vf20();
    virtual void vf40(HitInfo_unk* hit);
    virtual float vf90();

    void fn_802FC188(); // inferred: death
    inline void ResetAttack_unk();
    inline void StopAttack_unk();

    int m14B0;
    int m14B4;
    unsigned char m14B8[0x14D8 - 0x14B8];
    atn::SoundRef m14D8;
    atn::EfpHandle_unk m14DC;
    int m14E8;
    int m14EC;
    float m14F0;
    HitInfo_unk* m14F4;       // inferred: skill 0x3F7 (its m68 gives the wait frames)
    float m14F8;
    unsigned char m14FC[0x151C - 0x14FC];
    atn::SoundRef m151C;
    atn::EfpHandle_unk m1520;
    float m152C;
    HitInfo_unk* m1530;       // inferred: skill 0x3F8
    float m1534;
    int m1538;
    unsigned char m153C[0x1540 - 0x153C];
    int m1540;
    float m1544;
    float m1548;
    unsigned char m154C[0x1558 - 0x154C];
    CrabList_unk m1558;
    unsigned char m1564[0x1574 - 0x1564];
    int m1574;
    HitInfo_unk* m1578;       // inferred: skill 0x400
    int m157C;
    int m1580;
    float m1584;
    float m1588;
    float m158C;
    int m1590;
    float m1594;
    unsigned char m1598[0x15AC - 0x1598];
    CrabList_unk m15AC;
    int m15B8;
    int m15BC;
    unsigned char m15C0[0x15C8 - 0x15C0];
    int m15C8;
    int m15CC;
    int m15D0;
    int m15D4;
    unsigned char m15D8[0x15DC - 0x15D8];
    int m15DC;
    float m15E0;
    float m15E4;
    float m15E8;
    float m15EC;
    float m15F0;
    atn::EfpHandle_unk m15F4;
    atn::EfpHandle_unk m1600;
    atn::LH_Binary m160C;     // inferred: data file
    ResHandle m1614;          // inferred: debug menu group
    int m1618;
    unsigned char m161C[0x1620 - 0x161C];
};

#endif
