#ifndef ATN_LH_MOTIONSET_H
#define ATN_LH_MOTIONSET_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred from __vt__Q23atn12LH_MotionSet (0x8078FEF0); no destructor declared (weak copy
// 0x8013606C). mCallback is passed to the loaded resource (as in LH_MotionCommandData).
extern "C" int fn_802180A8(const char* name); // inferred: default motion name to id callback

class LH_MotionSet : public LoadHandle {
public:
    LH_MotionSet() : mCallback((void*)fn_802180A8) {} // inferred: inlined in CharaTask::CharaTask
    virtual void vf0C(const char* path); // 0x80477448

    void* mCallback; // 0x08: inferred: motion name to id
};

} // namespace atn

#endif
