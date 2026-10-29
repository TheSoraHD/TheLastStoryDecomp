#ifndef ATN_EFPHANDLE_UNK_H
#define ATN_EFPHANDLE_UNK_H

#include "atn/LH_Efp.h"

namespace atn {

// atn::EfpFile: effect resource holder (0xC bytes); class name and Destroy from the RSO (the
// file keeps its old name). LoadRequest (0x8023780C) and IsLoading (0x80237874) from the RSO, by the
// same order as in atn::EffFile.
class EfpFile {
public:
    EfpFile();                            // 0x802377B8
    ~EfpFile() { Destroy(); }
    void LoadRequest(const char* path);   // 0x8023780C
    void Destroy();                       // 0x8023781C: releases the resource
    int IsLoading();                      // 0x80237874: nonzero while loading

    void* mResource;  // 0x00
    LH_Efp mLoader;   // 0x04
};

} // namespace atn

#endif
