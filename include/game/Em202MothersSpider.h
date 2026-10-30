#ifndef GAME_EM202MOTHERSSPIDER_H
#define GAME_EM202MOTHERSSPIDER_H

#include "game/Em202ForestSpider.h"

// inferred from __vt__18Em202MothersSpider (0x807852E8); the enemy factory (fn_805A5374) allocates 0x19C8 bytes.
// The mother spider eats the characters caught by its threads and spits them out at "SpitOutPoint".
class Em202MothersSpider : public Em202ForestSpider {
public:
    Em202MothersSpider(atn::Task* parent, EnemyInfo_unk* info); // 0x80279860
    virtual ~Em202MothersSpider();                              // 0x80279930
    virtual int vf0C();                                         // 0x802799C0
    virtual void vf10();                                        // 0x8027A478
    virtual void vf3C(HitInfo_unk* hit);                        // 0x8027AC38
    virtual void vf40(HitInfo_unk* hit);                        // 0x8027AF4C
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir); // 0x8027B118
    virtual void vfF8();   // 0x8027B1A4 inferred: chooses the target
    virtual void vfFC();   // 0x8027B21C inferred: movement of the frame
    virtual void vf100();  // 0x8027B748 inferred: chooses the next action
    virtual void vf118();  // 0x8027BE3C inferred: state 11 (eats the caught character)
    virtual void vf14C();  // 0x8027B8C8 inferred: starts state 11
    virtual RoutePlace_unk* vf170(); // 0x8027C80C inferred: thread point nearest to the party
    virtual CharaTask* vf174();      // 0x8027C8E4 inferred: target of the threads
    virtual void vf178();  // 0x8027C1D4 inferred: starts state 19 (goes to the spit out point)
    virtual void vf17C();  // 0x8027C244 inferred: state 19
    virtual void vf180();  // 0x8027C2E0 inferred: starts state 20 (spits out the eaten characters)
    virtual void vf184();  // 0x8027C38C inferred: state 20

    RoutePlace_unk* m1968; // inferred: "SpitOutPoint"
    atn::Matrix3x4 m196C;  // inferred: matrix the eaten characters follow (CharaTask::mF1C)
    int m199C;             // inferred: the eaten characters must be spat out
    int m19A0;             // inferred: frames of step 1 of state 20
    int m19A4;             // inferred: hits taken while full
    float m19A8;           // inferred: 0.05f ("DamageRate", taken from the eaten characters every frame)
    int m19AC;             // inferred: 5 ("SpitOutNum")
    int m19B0;             // inferred: 300 ("SpitOutTime")
    atn::EfpFile m19B4;    // inferred: "/effect/ef_sk002d.efp"
    atn::SoundRef m19C0;
};

#endif
