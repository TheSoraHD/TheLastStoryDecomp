#ifndef ATN_LH_EFP_H
#define ATN_LH_EFP_H

#include "atn/LoadHandle.h"

namespace atn {

class LH_Efp : public LoadHandle {
public:
    LH_Efp() {}
    virtual ~LH_Efp() {}
    virtual void vf0C(const char* path);

    void* fn_80473C20(); // inferred: returns the loaded effect resource
};

} // namespace atn

#endif
