#ifndef GAME_PICKTASK_H
#define GAME_PICKTASK_H

#include "atn/Task.h"
#include "string.h"

// inferred from __vt__8PickTask (0x807948E8): computes every frame which characters can be picked
// (targeted) by the player; singleton
class PickTask : public atn::Task {
public:
    static PickTask* fn_80548D54(atn::Task* parent); // inferred: creates the instance
    PickTask(atn::Task* parent) : atn::Task(parent) { memset(m4C, 0, sizeof(m4C)); }
    virtual ~PickTask(); // 0x80548DE4
    virtual int vf0C();  // 0x80548E44
    virtual void vf10(); // 0x80548E70

    static PickTask* sInstance;

    int m48;
    int m4C[0x48][0x48]; // inferred: m4C[i][j] bit 0: i sees j, bit 1: j is near i (cleared every frame by vf10)
    int m514C;
};

#endif
