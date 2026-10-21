#ifndef GAME_EM305VAMPIREMP_H
#define GAME_EM305VAMPIREMP_H

#include "game/Em305Vampire.h"

// inferred from __vt__14Em305VampireMp (0x80787568): multiplayer version of the vampire; its skills
// depend on the hit points left (three levels) and its state is synchronized with vf110/vf114
class Em305VampireMp : public Em305Vampire {
public:
    Em305VampireMp(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em305VampireMp();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual void vf110(SpecialState_unk* state);
    virtual void vf114(const SpecialState_unk* state);
    virtual void vf11C();

    HitInfo_unk* m1608[3]; // inferred: "tackle_skill" per level
    HitInfo_unk* m1614[3]; // inferred: "rolling_skill" per level
    HitInfo_unk* m1620[3]; // inferred: "shine_skill" per level
    int m162C;
};

#endif
