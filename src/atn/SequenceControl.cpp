#include "atn/SequenceControl.h"
#include "atn/Memory.h"

namespace atn {

SequenceControl* SequenceControl::fn_800C3610(Task* parent, void* table) {
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
        SequenceEntry_unk* entry = mPending;
        if (entry == 0) {
            SequenceEntry_unk* found = fn_800C3E84(mTable, mNextId);
            if (found != 0) {
                mNextId = 0;
                entry = found;
            } else {
                entry = fn_800C3FB0(mTable, 0);
            }
        } else {
            mPending = 0;
        }
        mCurrent = entry->vf0C(this);
        mName = entry->m08;
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
