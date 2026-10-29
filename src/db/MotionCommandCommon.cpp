#include "db/MotionCommandCommon.h"
#include "atn/Memory.h"

namespace db {

static const char sPathStr[] = "/boot/motion_command_common.csv";
static const char* const sPath = sPathStr; // inferred: separate .rodata array + .sdata2 pointer

inline MotionCommandCommon::MotionCommandCommon(atn::Task* parent) : atn::Task(parent) {
    atn::LoadHandle* loader = &mLoader;
    mData = 0;
    loader->vf0C(sPath);
}

MotionCommandCommon* MotionCommandCommon::fn_80218720(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) MotionCommandCommon(parent);
    }
    return sInstance;
}

MotionCommandCommon::~MotionCommandCommon() {
    sInstance = 0;
}

int MotionCommandCommon::vf0C() {
    if (mLoader.fn_80473F50()) {
        return 0;
    }
    mData = mLoader.fn_80476CE4();
    mFlags |= 4;
    return 1;
}

} // namespace db
