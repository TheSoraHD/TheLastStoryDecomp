#ifndef GAME_STRAPTASK_H
#define GAME_STRAPTASK_H

#include "atn/Task.h"
#include "atn/Texture_unk.h"

// inferred from __vt__9StrapTask (0x807975B0): the strap warning screen shown at boot
class StrapTask : public atn::Task {
public:
    static StrapTask* fn_805A6208(atn::Task* parent); // inferred: creates the task

    StrapTask(atn::Task* parent); // 0x805A626C
    virtual ~StrapTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    int m48; // inferred: state
    int m4C; // inferred: frames in the state
    int m50; // inferred: frames since the start
    atn::Texture_unk m54[2];
    int mB4;
};

#endif
