#ifndef GAME_TOWNTPCLOADER_H
#define GAME_TOWNTPCLOADER_H

#include "atn/Task.h"
#include "atn/FixedArray.h"

// inferred: town NPC loader task (class name from RTTI, vtable at 0x80797928).
// Collects the characters placed in grid cell (mCellX, mCellZ) once mWait has finished.
class TownTpcLoader : public atn::Task {
public:
    TownTpcLoader(atn::Task* parent, int cellX, int cellZ, atn::Task* wait)
        : atn::Task(parent), mCellX(cellX), mCellZ(cellZ), mState(0), mWait(wait) {}
    virtual ~TownTpcLoader();
    virtual int vf0C();

    static TownTpcLoader* fn_805B9B08(atn::Task* parent, int cellX, int cellZ, atn::Task* wait);
    void fn_805B9D58(); // inferred: releases the collected characters and the task

    atn::FixedArray_unk<atn::Task*, 64> mCharas; // 0x48
    int mCellX;              // 0x14C
    int mCellZ;              // 0x150
    int mState;              // 0x154
    atn::Task* mWait;        // 0x158
    unsigned char m15C[0x160 - 0x15C];
};

#endif
