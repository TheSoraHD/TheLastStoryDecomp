// Placeholder (batch-auto-10kb fase 0): only the weak destructors that follow the __sinit of this file
// (0x80216A2C, 0x80216A94, moved here from db/MapIdTable_unk). The rest of the unit is still to be decompiled.
#include "atn/Memory.h"

// inferred: element of the global lists of db/MapLink_unk.cpp (its constructor makes new[] keep a
// cookie)
struct MapLinkItemA_unk {
    MapLinkItemA_unk();
    int m00;
};
struct MapLinkItemB_unk {
    MapLinkItemB_unk();
    int m00;
};

// inferred: weak copies of atn::Vector_unk<T>::~Vector_unk for the global lists of this file, written
// as non-template classes for now (names invented)
struct MapLinkListA_unk {
    ~MapLinkListA_unk();
    unsigned int mSize;
    unsigned int mCapacity;
    MapLinkItemA_unk* mData;
};
MapLinkListA_unk::~MapLinkListA_unk() {
    if (mData != 0) {
        delete[] mData;
    }
}

struct MapLinkListB_unk {
    ~MapLinkListB_unk();
    unsigned int mSize;
    unsigned int mCapacity;
    MapLinkItemB_unk* mData;
};
MapLinkListB_unk::~MapLinkListB_unk() {
    if (mData != 0) {
        delete[] mData;
    }
}

