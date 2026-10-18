#ifndef ATN_LH_BINARY_H
#define ATN_LH_BINARY_H

#include "atn/LoadHandle.h"

namespace atn {

// inferred: handle to a binary resource; no destructor declared (the implicit one is emitted
// first at 0x80014798)
class LH_Binary : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x804705B8
    int fn_80470580();   // inferred: size of the data
    void* fn_8047059C(); // inferred: data
};

} // namespace atn

#endif
