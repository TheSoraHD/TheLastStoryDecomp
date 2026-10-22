#ifndef ATN_LH_MATERIAL_H
#define ATN_LH_MATERIAL_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred from __vt__Q23atn11LH_Material (0x8078FDA8); no destructor declared (weak copy
// 0x8008A714)
class LH_Material : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x8047462C

    void* fn_80474860();        // inferred: returns the list of loaded materials (no null check)
    unsigned int fn_8047486C(); // inferred: number of loaded materials (0 if nothing is loaded)
};

} // namespace atn

#endif
