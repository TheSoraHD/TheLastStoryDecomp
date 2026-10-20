// Placeholder (batch-auto-10kb fase 0): only the weak destructor that follows the __sinit of this file
// (0x8021CC50, moved here from db/StrategyAiTable_unk). The rest of the unit is still to be decompiled.
#include "atn/Memory.h"

// inferred: element of the global buffers of db/SpText_unk.cpp (its destructor, fn_8021BE40, is
// defined there)
struct SpTextEntry_unk {
    ~SpTextEntry_unk();
};

// inferred: weak copy of atn::Buffer_unk<T>::~Buffer_unk for the six global buffers of this file,
// written as a non-template class for now (name invented)
struct SpTextBuffer_unk {
    ~SpTextBuffer_unk();

    int mSize;
    SpTextEntry_unk* mData;
};

SpTextBuffer_unk::~SpTextBuffer_unk() {
    if (mData != 0) {
        delete[] mData;
    }
    mData = 0;
    mSize = 0;
}

