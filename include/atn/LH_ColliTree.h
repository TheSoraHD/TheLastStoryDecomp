#ifndef ATN_LH_COLLITREE_H
#define ATN_LH_COLLITREE_H

#include "atn/LoadHandle.h"

namespace atn {

class LH_ColliTree : public LoadHandle {
public:
    LH_ColliTree() {}
    virtual ~LH_ColliTree() {}
    virtual void vf0C(const char* path); // 0x80472D80

    void* fn_80472D64(); // inferred: returns the loaded collision tree (LoadColliTree::m5C)
};

} // namespace atn

#endif
