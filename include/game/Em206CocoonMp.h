#ifndef GAME_EM206COCOONMP_H
#define GAME_EM206COCOONMP_H

#include "game/SpecialEnemyBase.h"

// inferred from __vt__13Em206CocoonMp (0x80786568); the enemy factory (fn_805A5374) allocates 0x1938 bytes.
// Only the constructor and the overrides of the pure virtuals are declared until the unit is decompiled.
class Em206CocoonMp : public SpecialEnemyBase {
public:
    Em206CocoonMp(atn::Task* parent, EnemyInfo_unk* info);
    // inferred: the pure virtuals of SpecialEnemyBase (state machine)
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108(); // 0x802BA92C

    unsigned char m14D4[0x1938 - 0x14D4]; // inferred: members not decompiled yet
};

#endif
