#ifndef ATN_LOADMOTION_H
#define ATN_LOADMOTION_H

#include "atn/LoadCore.h"

namespace atn {

// Declared after LH_Motion (included last in LH_Motion.cpp): the vtables are emitted in reverse order
// of declaration.
// inferred: motion resource (path stored without directory and extension)
class LoadMotion : public LoadCore {
public:
    LoadMotion(const char* path); // 0x80476220
    virtual ~LoadMotion();        // 0x8047656C
    virtual void vf0C();          // 0x804765C4

    int m5C; // inferred: result of fn_800DC500 on the motion name
    int m60; // 4 for "fc_*" files, -1 otherwise
};

} // namespace atn

#endif
