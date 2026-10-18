#ifndef ATN_LOADER_UNK_H
#define ATN_LOADER_UNK_H

#include <boost/function.hpp>

// inferred: 4-byte helper built on the stack by the atn::Load* vf0C functions (LoadMotion,
// LoadTexture); names unknown
struct Loader_unk {
    int m00;
};
extern "C" void fn_804714A4(Loader_unk* self);
extern "C" void fn_804714B0(Loader_unk* self, void* a, void* b, boost::function<void()> f, int c);

#endif
