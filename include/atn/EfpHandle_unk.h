#ifndef ATN_EFPHANDLE_UNK_H
#define ATN_EFPHANDLE_UNK_H

#include "atn/LH_Efp.h"

namespace atn {

// inferred: effect resource holder (0xC bytes); the real class name is unknown.
class EfpHandle_unk {
public:
    EfpHandle_unk();                      // 0x802377B8
    ~EfpHandle_unk() { fn_8023781C(); }
    void fn_8023780C(const char* path);   // inferred: starts loading
    void fn_8023781C();                   // inferred: releases the resource
    int fn_80237874();                    // inferred: nonzero while loading

    void* mResource;  // 0x00
    LH_Efp mLoader;   // 0x04
};

} // namespace atn

#endif
