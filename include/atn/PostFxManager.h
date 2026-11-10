#ifndef ATN_POSTFXMANAGER_H
#define ATN_POSTFXMANAGER_H

#include "atn/LinearAllocator.h"

namespace atn {

// inferred: owner of the post effects (see atn/LinearAllocator.cpp, which defines the whole class: a task with the
// effects as members); only the allocator is seen here
class PostFxManager {
public:
    void* fn_800A96AC(int width, int height, int format); // inferred: temporary texture

    unsigned char m000[0x48];
    LinearAllocator mAlloc; // 0x048: inferred: temporary buffers of the effects
};

} // namespace atn

extern "C" atn::PostFxManager* lbl_8087EF8C;

#endif
