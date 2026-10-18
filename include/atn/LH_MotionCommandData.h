#ifndef ATN_LH_MOTIONCOMMANDDATA_H
#define ATN_LH_MOTIONCOMMANDDATA_H

#include "atn/LoadHandle.h"

extern "C" void fn_8011FFF8(); // inferred: motion command data parser (callback)

namespace atn {

class LH_MotionCommandData : public LoadHandle {
public:
    LH_MotionCommandData() { mCallback = (void*)&fn_8011FFF8; }
    virtual ~LH_MotionCommandData() {}
    virtual void vf0C(const char* path); // 0x80476A74

    void* fn_80476CE4(); // inferred: returns the loaded data
};

} // namespace atn

#endif
