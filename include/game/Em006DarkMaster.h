#ifndef GAME_EM006DARKMASTER_H
#define GAME_EM006DARKMASTER_H

#include "atn/EfpHandle_unk.h"
#include "game/CharaStatus.h"
#include "game/SpecialEnemyBase.h"

// inferred from __vt__15Em006DarkMaster (0x807844E8): enemy that commands the enemies of kind 42
// (m14DC); it is defeated (state 9) when none of them can act any more
class Em006DarkMaster : public SpecialEnemyBase {
public:
    Em006DarkMaster(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em006DarkMaster();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf40(HitInfo_unk* hit);
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual int vf10C(HitMsg_unk* msg);

    void fn_80254808(const char* text, int length); // inferred: reads the data file
    void fn_802549C8(); // inferred: counts the servants that can still act

    int m14D4;                            // inferred: servants that can act
    FixedArray_unk<EnemyTask*, 8> m14D8;  // inferred: servants (enemies of kind 42)
    atn::Vector3 m14FC;                   // inferred: position where the master reappears
    atn::EfpFile m1508;
};

#endif
