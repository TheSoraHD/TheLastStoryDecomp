#ifndef GAME_NEWTITLETASK_H
#define GAME_NEWTITLETASK_H

#include "atn/Task.h"

// RTTI name (vtable 0x80793A20): the title screen (0xBC0 bytes; code in game/NewTitleTask.cpp). Members
// unknown.
class NewTitleTask : public atn::Task {
public:
    NewTitleTask(atn::Task* parent, int arg); // 0x80531F44
    unsigned char m48[0xBC0 - 0x48];
};

// inferred: creator of the title sequence (C++ linkage: the typeid names of the creators have no extern "C")
NewTitleTask* fn_80531ED0(atn::Task* parent, int arg);

#endif
