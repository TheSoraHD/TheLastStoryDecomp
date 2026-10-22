#ifndef ATN_LH_XSCA_H
#define ATN_LH_XSCA_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred from __vt__Q23atn7LH_Xsca (0x80790000); no destructor declared (weak copy 0x800EF7A0)
class LH_Xsca : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x804784CC

    void* fn_804786F8(); // inferred: returns the loaded animation (LoadXsca::m5C)
};

} // namespace atn

#endif
