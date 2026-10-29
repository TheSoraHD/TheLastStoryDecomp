#ifndef ATN_RESLOADER_UNK_H
#define ATN_RESLOADER_UNK_H

#include "atn/LH_Effect.h"

namespace atn {

// atn::EffFile (name from the RSO: atn::MotionCommandData holds SimpleVector<EffFile>,
// SimpleVector<EfpFile>, ... and this is the member that is not an EfpFile; methods LoadRequest,
// IsLoading, Destroy as for EfpFile). Effect resource with its loader (0xC bytes): resource at 0x00,
// loader at 0x04. The file name keeps the old invented class name.
struct EffFile {
    EffFile();                          // 0x80237518
    ~EffFile();                         // 0x802375C4
    void LoadRequest(const char* path); // 0x80237654
    int IsLoading();                    // 0x802376D0: nonzero while loading
    void Destroy();                     // 0x8023772C: releases the resource

    void* mResource;   // 0x00
    LH_Effect mLoader; // 0x04
};

} // namespace atn

#endif
