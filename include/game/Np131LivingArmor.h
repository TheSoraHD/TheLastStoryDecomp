#ifndef GAME_NP131LIVINGARMOR_H
#define GAME_NP131LIVINGARMOR_H

#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"

class GimmickCommon;

// inferred from __vt__16Np131LivingArmor (0x80794608): a living armor that moves between the armor
// stand gimmicks (id 0x1391)
class Np131LivingArmor : public EnemyTask {
public:
    Np131LivingArmor(atn::Task* parent, const EnemyInfo_unk* info);
    virtual ~Np131LivingArmor();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vfC0(void* draw);
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);

    void fn_80546B84(); // inferred: moves to the armor stand m14DC (state 6)
    void fn_80547468(); // inferred: lets the player use the nearest armor stand

    int m14B0;
    int m14B4;
    atn::EffFile m14B8;      // inferred: "ef_uc069.eff"
    atn::EffFile m14C4;      // inferred: "gm005_009a_01.eff"
    atn::EfpFile m14D0; // inferred: "ef_uc068.efp"
    GimmickCommon* m14DC;     // inferred: armor stand the armor moves to
    int m14E0;                // inferred: state of the death sequence (1 fading, 2 done)
    int m14E4;                // inferred: number of armor stands reached
    int m14E8;                // inferred: frame counter of the fade
    float m14EC;              // inferred: 80% of the hit points
    int m14F0;                // inferred: frames spent moving
    int m14F4;
    int m14F8;                // inferred: the player was on a ledge the previous frame (mFlag31)
    int m14FC;                // inferred: the player can use the armor stand
};

#endif
