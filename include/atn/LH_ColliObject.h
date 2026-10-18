#ifndef ATN_LH_COLLIOBJECT_H
#define ATN_LH_COLLIOBJECT_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred from __vt__Q23atn14LH_ColliObject (0x8078FC38); no destructor declared (weak copy
// 0x80058020)
class LH_ColliObject : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x80472A50

    void* fn_80472A34(); // inferred: returns the loaded collision object (LoadColliObject::m5C)
};

} // namespace atn

#endif
