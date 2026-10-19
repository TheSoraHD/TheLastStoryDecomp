#ifndef GAME_EM209BIGGOLEM_H
#define GAME_EM209BIGGOLEM_H

#include "game/Em209SmallGolem.h"

// inferred from __vt__13Em209BigGolem (0x80786990)
class Em209BigGolem : public Em209SmallGolem {
public:
    Em209BigGolem(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em209BigGolem();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf20();
    virtual void vfEC();
    // inline: weak copy emitted at the end of Em209BigGolem.cpp
    virtual float vf90() { return 3.0f; }
};

#endif
