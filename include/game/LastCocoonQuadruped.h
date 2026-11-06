#ifndef GAME_LASTCOCOONQUADRUPED_H
#define GAME_LASTCOCOONQUADRUPED_H

#include "game/EnemyTask.h"

// inferred from __vt__19LastCocoonQuadruped (0x8078FBC0): quadruped form of the last cocoon; the enemy
// factory (fn_805A5374) allocates 0x18F8 bytes. Members keep their offsets (names unknown).
class LastCocoonQuadruped : public EnemyTask {
public:
    LastCocoonQuadruped(atn::Task* parent, EnemyInfo_unk* info); // 0x8045FAB0
    virtual void vf14();                                         // 0x80460A90
    virtual void vf3C(HitInfo_unk* hit);                         // 0x80460A94
    virtual atn::Vector3 vf64(int center);                       // 0x80460EF0
    virtual float vf90();                                        // 0x80468D40
    virtual int vfE4();                                          // 0x80468D18

    unsigned char m14B0[0x14BC - 0x14B0];
    int m14BC;
    unsigned char m14C0[0x14C4 - 0x14C0];
    int m14C4; // inferred: state (7: no hit reaction)
    unsigned char m14C8[0x18F8 - 0x14C8];
};

#endif
