#ifndef ATN_SEQUENCEBOOT_UNK_H
#define ATN_SEQUENCEBOOT_UNK_H

#include "atn/Task.h"

namespace atn {
class SequenceHolder;
}

// inferred: owner of the four sequence tables (0x14 bytes, name invented); created by main
// (atn/SequenceUnitParam1_const_int.cpp)
class SequenceBoot_unk {
public:
    SequenceBoot_unk(atn::Task* parent);
    static void Create_unk(atn::Task* parent);
    void SetNext_unk(const char* name); // inferred: goes to the named sequence
    void fn_80572BA0();
    void fn_80574918();

    atn::Task* mParent;                // 0x00
    atn::SequenceHolder* mHolders[4];  // 0x04
};

extern SequenceBoot_unk* sSequenceBoot_unk; // 0x8087F9AC

#endif
