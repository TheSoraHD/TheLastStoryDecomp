#ifndef ATN_LH_MOTION_H
#define ATN_LH_MOTION_H

#include "atn/LoadCore.h"
#include "atn/LoadHandle.h"

namespace atn {

// inferred: motion resource (path stored without directory and extension)
class LoadMotion : public LoadCore {
public:
    LoadMotion(const char* path); // 0x80476220
    virtual ~LoadMotion();        // 0x8047656C
    virtual void vf0C();          // 0x804765C4

    int m5C; // inferred: result of fn_800DC500 on the motion name
    int m60; // 4 for "fc_*" files, -1 otherwise
};

// inferred: no destructor declared (the implicit one is emitted first at 0x80097128)
class LH_Motion : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x80476664
};

} // namespace atn

#endif
