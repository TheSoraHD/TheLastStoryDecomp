#ifndef GAME_BATTLECHARATASK_H
#define GAME_BATTLECHARATASK_H

#include "game/CharaTask.h"

// inferred from __vt__15BattleCharaTask (0x80779B68)
class BattleCharaTask : public CharaTask {
public:
    BattleCharaTask(atn::Task* parent, int type, void* data); // 0x800E2AF8
    virtual ~BattleCharaTask();                               // 0x800E2D4C
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf2C();
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf44();
    virtual int vfA4(); // inferred: returns 1/0 (0x800EE494)
    virtual void vfE4();
    virtual void vfE8();
    virtual void vfEC();
    virtual void vfF0();
};

#endif
