#ifndef ATN_LOADER_UNK_H
#define ATN_LOADER_UNK_H

#include "atn/ChunkHeader.h"
#include <boost/function.hpp>

// inferred: 4-byte helper built on the stack by the atn::Load* vf0C functions (LoadMotion,
// LoadTexture, LoadScene, LoadModel); names unknown. fn_804714B0 reads the chunks of `data` and
// passes each one to `callback` with `user` (the callback type is in the DOL, see ChunkHeader.h).
struct Loader_unk {
    int m00;
};
typedef boost::function<void(void*, const atn::ChunkHeader*, void*)> LoaderCallback_unk;
extern "C" void fn_804714A4(Loader_unk* self);
extern "C" void fn_804714B0(Loader_unk* self, void* data, void* size, LoaderCallback_unk callback,
                            void* user);

#endif
