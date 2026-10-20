#ifndef ATN_SEQUENCECONTROL_H
#define ATN_SEQUENCECONTROL_H

#include "atn/Task.h"

namespace atn {

// inferred: entry of the sequence table; its real class name is unknown.
class SequenceEntry_unk {
public:
    virtual void vf08();
    virtual Task* vf0C(Task* parent); // inferred: creates the sequence task

    int m04;
    unsigned char m08[1]; // 0x08: inferred: name, its address is stored in SequenceControl::m58
};

// inferred: the table type is unknown; these are its lookup methods.
extern "C" SequenceEntry_unk* fn_800C3E84(void* table, int id);     // find by id
extern "C" SequenceEntry_unk* fn_800C3FB0(void* table, int index);  // get by index

class SequenceControl : public Task {
public:
    SequenceControl(Task* parent, void* table) : Task(parent), mCurrent(0), mTable(table), mNextId(0), mPending(0) {
        sInstance = this;
    }
    virtual ~SequenceControl();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(Task* child);

    // inferred: creates the singleton under `parent` if it does not exist yet
    static SequenceControl* fn_800C3610(Task* parent, void* table);
    int fn_800C37D8(); // inferred: non-virtual, always returns 0
    int fn_800C37FC(const char* name); // inferred: id of the sequence `name` (atn/SequenceHolder.cpp)

    Task* mCurrent;               // 0x48
    void* mTable;                 // 0x4C
    int mNextId;                  // 0x50
    SequenceEntry_unk* mPending;  // 0x54
    unsigned char* mName;         // 0x58
    int m5C;

    static SequenceControl* sInstance; // inferred name (0x8087EFC0)
};

} // namespace atn

#endif
