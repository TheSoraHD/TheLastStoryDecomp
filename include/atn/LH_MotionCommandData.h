#ifndef ATN_LH_MOTIONCOMMANDDATA_H
#define ATN_LH_MOTIONCOMMANDDATA_H

#include "atn/LoadHandle.h"

struct MotionCmdData_unk;
// inferred: motion command data parser (callback, game/MotionCommand_unk.cpp)
extern "C" void fn_8011FFF8(MotionCmdData_unk* data, void* buffer, int size);

namespace atn {

class LH_MotionCommandData : public LoadHandle {
public:
    LH_MotionCommandData() { mCallback = (void*)&fn_8011FFF8; }
    virtual ~LH_MotionCommandData() {}
    virtual void vf0C(const char* path); // 0x80476A74

    void* fn_80476CE4(); // inferred: returns the loaded data

    void* mCallback; // 0x08
};

} // namespace atn

#endif
