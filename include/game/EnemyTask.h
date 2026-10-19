#ifndef GAME_ENEMYTASK_H
#define GAME_ENEMYTASK_H

#include "game/BattleCharaTask.h"

// inferred: spawn description passed to the enemy constructors (class unknown)
struct EnemyInfo_unk {
    unsigned char m00[0x20];
    void* m20;     // inferred: character data passed to CharaTask
    unsigned char m24[0x2C - 0x24];
    char m2C[1];   // inferred: parameter text ("key=value;..."), length unknown
};

// inferred from __vt__9EnemyTask (0x80789A08)
class EnemyTask : public BattleCharaTask {
public:
    EnemyTask(atn::Task* parent, void* data); // 0x8035B694
    virtual ~EnemyTask();                     // 0x8035B78C
    virtual void vf68();
    virtual void vf6C();
    virtual int vfF4();

    EnemyTask* mNext; // 0x14AC: inferred: next enemy in the EnemyManager list
};

#endif
