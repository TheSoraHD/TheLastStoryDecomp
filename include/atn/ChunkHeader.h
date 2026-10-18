#ifndef ATN_CHUNKHEADER_H
#define ATN_CHUNKHEADER_H

namespace atn {

// The name is in the DOL: the type of the loader callbacks is
// "void (*)(void *, const atn::ChunkHeader *, void *)" (0x80755D80). Layout inferred.
struct ChunkHeader {
    unsigned int mTag; // four-character code, e.g. 'modl'
};

} // namespace atn

#endif
