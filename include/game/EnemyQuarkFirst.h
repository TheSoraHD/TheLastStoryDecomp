#ifndef GAME_ENEMYQUARKFIRST_H
#define GAME_ENEMYQUARKFIRST_H

#include "atn/EfpHandle_unk.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

// inferred from __vt__15EnemyQuarkFirst: first fight of Quark. Only the small functions are
// written (game/EnemyQuarkFirst.cpp); the factory allocates 0x1588 bytes. Names invented.
class EnemyQuarkFirst : public SpecialEnemyBase {
public:
    EnemyQuarkFirst(atn::Task* parent, EnemyInfo_unk* info); // 0x80334CD8
    virtual ~EnemyQuarkFirst();                               // 0x803387DC
    virtual int vf0C();                                       // 0x80334E74
    virtual void vf10();                                      // 0x80335374
    virtual void vf30(void* menu);                            // 0x80338568
    virtual void vf40(HitInfo_unk* hit);                      // 0x80335788
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir); // 0x803386FC
    virtual int vfA8(int kind);                               // 0x80338770
    virtual void vfFC();                                      // 0x80335940
    virtual void vf100(int state);                            // 0x80335988
    virtual void vf104();                                     // 0x80336518
    virtual void vf108();                                     // 0x80336C8C
    virtual int vf10C(struct HitMsg_unk* msg);                // 0x80335600

    int m14D4;
    int m14D8; // inferred: mode (1 = guards)
    int m14DC;
    float m14E0;
    unsigned char m14E4[0x14E8 - 0x14E4];
    float m14E8;
    unsigned char m14EC[0x1504 - 0x14EC];
    int m1504;
    int m1508;
    int m150C;
    int m1510;
    unsigned char m1514[0x1518 - 0x1514];
    int m1518;
    int m151C;
    unsigned char m1520[0x1528 - 0x1520];
    atn::EfpHandle_unk m1528;
    atn::EfpHandle_unk m1534;
    int m1540[11];
    ResHandle m156C; // inferred: debug menu group
    int m1570[5];
    unsigned char m1584[0x1588 - 0x1584];
};

#endif
