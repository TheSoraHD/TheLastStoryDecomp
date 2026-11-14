#ifndef ATN_COMPOSITECACHE_UNK_H
#define ATN_COMPOSITECACHE_UNK_H

// inferred: entries of the cache of composed textures (atn/CompositeCache_unk.cpp), also read by
// atn/CompositeIntensityTintMulti.cpp. Names invented.
#include "atn/Texture_unk.h"
#include "atn/Color.h"

namespace atn {

// inferred: RGBA color
struct CacheColor_unk {
    float r, g, b, a;
};

// inferred: description of one layer (0x40 bytes; filled by atn::CompositeIntensityTintMulti)
struct CompositeLayerDesc_unk {
    const char* mPath;     // 0x00
    Color mColor[3];       // 0x04
    int mUse[3];           // 0x34
};

// inferred: the same layer with the hash of its path
struct CompositeKeyLayer_unk {
    unsigned int mHash;       // 0x00
    CacheColor_unk mColor[3]; // 0x04
    int mUse[3];              // 0x34
};

// inferred: key of a cached texture (0x188 bytes)
struct CompositeKey_unk {
    CompositeKey_unk(const char* base, const CompositeLayerDesc_unk* layers, int count);
    bool fn_800D82BC(const CompositeKey_unk& other) const;

    unsigned int mHash;               // 0x00
    int mCount;                       // 0x04
    CompositeKeyLayer_unk mLayers[6]; // 0x08
};

// inferred: cached composed texture (refcounted; 0x1BC bytes)
struct CompositeCacheEntry_unk {
    CompositeCacheEntry_unk(const char* base, const CompositeLayerDesc_unk* layers, int count)
        : mRefCount(0), mKey(base, layers, count) {}
    void fn_800D8448();
    void fn_800D8458();

    int mRefCount;          // 0x000
    CompositeKey_unk mKey;  // 0x004
    Texture_unk mTexture;   // 0x18C
};

} // namespace atn

#endif
