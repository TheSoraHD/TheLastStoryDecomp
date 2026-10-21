#ifndef GAME_GUN_H
#define GAME_GUN_H

#include "game/Arms.h"

// inferred: from RTTI "Gun" (vtable at 0x8078F228, base Arms): the crossbow; m2A8 is the arrow
// model ("it090_00.model") shown on it.
class Gun : public Arms {
public:
    Gun(void* arg04, int arg08, int arg0C, int arg270); // 0x8043F028
    virtual ~Gun();                                     // 0x8043F0F4
    virtual int vf0C();                                 // 0x8043F178
    virtual void vf14();                                // 0x8043F24C
    virtual void vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d); // 0x8043F244

    boost::scoped_ptr<EffectModel_unk> m2A8; // inferred: arrow model
};

#endif
