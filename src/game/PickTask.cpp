#include "game/PickTask.h"
#include "atn/Memory.h"

PickTask* PickTask::sInstance;

PickTask* PickTask::fn_80548D54(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (3, "", "", 0) PickTask(parent);
    }
    return sInstance;
}

PickTask::~PickTask() {
    sInstance = 0;
}

int PickTask::vf0C() {
    return fn_800D3FA4() != 0;
}

// NONMATCHING: vf10 (0x80548E70, 0xF1C bytes: builds the pick tables from the character lists) is not written yet
