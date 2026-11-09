#include "atn/SequenceControl.h"
#include "atn/Memory.h"

namespace atn {

SequenceControl* SequenceControl::fn_800C3610(Task* parent, SequenceHolder* table) {
    if (sInstance == 0) {
        return new ((MemoryTag)1, "", "", 0) SequenceControl(parent, table);
    }
    return sInstance;
}

SequenceControl::~SequenceControl() {
    sInstance = 0;
}

int SequenceControl::vf0C() {
    return 1;
}

void SequenceControl::vf10() {
    if (mCurrent == 0) {
        SequenceHolder::SequenceUnitBase* entry = mPending;
        if (entry == 0) {
            SequenceHolder::SequenceUnitBase* found = mTable->fn_800C3E84(mNextId);
            if (found != 0) {
                mNextId = 0;
                entry = found;
            } else {
                entry = mTable->fn_800C3FB0(0);
            }
        } else {
            mPending = 0;
        }
        mCurrent = entry->vf0C(this);
        mName = entry->mName;
        fn_800D3F28(mCurrent);
    }
}

int SequenceControl::fn_800C37D8() {
    return 0;
}

void SequenceControl::vf14() {}

void SequenceControl::vf18(Task* child) {
    if (mCurrent == child) {
        mCurrent = 0;
    }
}

} // namespace atn
