#ifndef ATN_SEQUENCECONTROL_H
#define ATN_SEQUENCECONTROL_H

#include "atn/Task.h"
#include "atn/SequenceHolder.h"

namespace atn {

class SequenceControl : public Task {
public:
    SequenceControl(Task* parent, SequenceHolder* table) : Task(parent), mCurrent(0), mTable(table), mNextId(0), mPending(0) {
        sInstance = this;
    }
    virtual ~SequenceControl();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(Task* child);

    // inferred: creates the singleton under `parent` if it does not exist yet
    static SequenceControl* fn_800C3610(Task* parent, SequenceHolder* table);
    int fn_800C37D8(); // inferred: non-virtual, always returns 0
    int fn_800C37FC(const char* name); // inferred: id of the sequence `name` (atn/SequenceHolder.cpp)

    Task* mCurrent;               // 0x48
    SequenceHolder* mTable;       // 0x4C
    int mNextId;                  // 0x50
    SequenceHolder::SequenceUnitBase* mPending; // 0x54
    char* mName;                  // 0x58: name of the current unit
    int m5C;

    static SequenceControl* sInstance; // inferred name (0x8087EFC0)
};

} // namespace atn

#endif
