#ifndef ATN_LH_BINARY_H
#define ATN_LH_BINARY_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred: handle to a binary resource; no destructor declared (the implicit one is emitted
// first at 0x80014798)
class LH_Binary : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x804705B8
    // inferred: data and size: LoadSeTableTask::vf0C (0x803EEEDC) passes them to the atn::Csv
    // constructor as (text, length)
    void* fn_80470580();
    int fn_8047059C();
};

} // namespace atn

#endif
