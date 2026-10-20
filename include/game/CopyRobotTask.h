#ifndef GAME_COPYROBOTTASK_H
#define GAME_COPYROBOTTASK_H

#include "game/EnemyTask.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_MotionSet.h"

// inferred from __vt__13CopyRobotTask (0x807824D8): an enemy that copies the pose of another character
class CopyRobotTask : public EnemyTask {
public:
    CopyRobotTask(atn::Task* parent, EnemyInfo_unk* info); // 0x801CC0AC
    virtual ~CopyRobotTask();                              // 0x801CC3AC
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit) {} // 0x801CCCFC
    void fn_801CCD00(int state); // inferred: changes the copy mode

    int m14B0; // inferred: copy mode
    CharaTask* m14B4; // inferred: copied character (mode 0)
    CharaTask* m14B8; // inferred: copied character (modes 1-3)
    atn::EfpHandle_unk m14BC;
    atn::EfpHandle_unk m14C8;
    atn::LH_MotionSet m14D4;
};

#endif
