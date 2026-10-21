#ifndef GAME_SWORD_H
#define GAME_SWORD_H

#include "game/Arms.h"

// inferred: from RTTI (vtable at 0x807975E8)
class Sword : public Arms {
public:
    Sword(void* arg04, int arg08, int arg0C, int arg270);
    virtual ~Sword();
    virtual int vf0C();
    virtual void vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d);
};

#endif
