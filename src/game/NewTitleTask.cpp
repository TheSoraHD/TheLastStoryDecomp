#include "game/NewTitleTask.h"
#include "atn/Memory.h"

// game/NewTitleTask.cpp (0x80531ED0-0x80537708): the title screen. Partial: only the creator is written.

NewTitleTask* fn_80531ED0(atn::Task* parent, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewTitleTask(parent, arg);
    }
    return 0;
}
