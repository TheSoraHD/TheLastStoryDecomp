#ifndef GAME_EM203GREATERDAEMONREDMP_H
#define GAME_EM203GREATERDAEMONREDMP_H

#include "game/SpecialEnemyBase.h"

// inferred from __vt__23Em203GreaterDaemonRedMp (0x80785B38); the enemy factory (fn_805A5374) allocates 0x16D0 bytes.
// Only the constructor and the overrides of the pure virtuals are declared until the unit is decompiled.
class Em203GreaterDaemonRedMp : public SpecialEnemyBase {
public:
    Em203GreaterDaemonRedMp(atn::Task* parent, EnemyInfo_unk* info);
    // inferred: the pure virtuals of SpecialEnemyBase (state machine)
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108(); // 0x8029CB00

    unsigned char m14D4[0x16D0 - 0x14D4]; // inferred: members not decompiled yet
};

#endif
