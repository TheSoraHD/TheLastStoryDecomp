#ifndef ATN_RESLOADER_UNK_H
#define ATN_RESLOADER_UNK_H

#include "atn/LoadHandle.h"

// inferred: resource with its loader (0xC bytes): resource at 0x00, loader at 0x04. Destructor at
// 0x802375C4 (also declared locally in atn/LH_MotionCommandData.cpp). Name invented.
struct ResLoader_unk {
    ResLoader_unk();                    // 0x80237518
    ~ResLoader_unk();                   // 0x802375C4
    void fn_80237654(const char* path); // inferred: starts loading
    int fn_802376D0();                  // inferred: nonzero while loading
    void fn_8023772C();                 // inferred: releases the resource (game/ColliAttrManager.cpp)

    void* mResource;
    unsigned char m04[0x0C - 0x04];
};

#endif
