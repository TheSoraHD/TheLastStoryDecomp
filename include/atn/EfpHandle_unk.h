#ifndef ATN_EFPHANDLE_UNK_H
#define ATN_EFPHANDLE_UNK_H

#include "atn/LH_Efp.h"

namespace atn {

// atn::EfpFile: effect resource holder (0xC bytes); class name and Destroy from the RSO (the
// file keeps its old name). fn_8023780C/fn_80237874 are probably LoadRequest/IsLoading (RSO).
class EfpFile {
public:
    EfpFile();                            // 0x802377B8
    ~EfpFile() { Destroy(); }
    void fn_8023780C(const char* path);   // inferred: starts loading
    void Destroy();                       // 0x8023781C: releases the resource
    int fn_80237874();                    // inferred: nonzero while loading

    void* mResource;  // 0x00
    LH_Efp mLoader;   // 0x04
};

} // namespace atn

#endif
