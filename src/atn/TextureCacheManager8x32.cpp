#include "atn/TextureCacheManager.h"

extern "C" int GXGetTexObjFmt(const GXTexObj* obj);
extern "C" unsigned char GXGetTexObjMipMap(const GXTexObj* obj);
extern "C" unsigned int GXGetTexObjData(const GXTexObj* obj);

namespace atn {

// inferred: region of a texture: the slot that already holds its image, or `id` modulo the count;
// no region for the color index formats (8-10)
GXTexRegion* ITextureCacheManager::fn_800D6898(GXTexObj* obj, unsigned int id) {
    int format = GXGetTexObjFmt(obj);
    int mipmap = GXGetTexObjMipMap(obj);
    unsigned int data = GXGetTexObjData(obj);
    int slot = -1;
    for (int i = 0; i < mCount; i++) {
        if (data == mIds[i]) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        slot = (int)id % mCount;
        mIds[slot] = data;
    }
    GXTexRegion* region = 0;
    if ((unsigned int)(format - 8) > 2) {
        if (format == 6) {
            if (mipmap) {
                region = &mRegionC[slot];
            } else {
                region = &mRegionA[slot];
            }
        } else if (mipmap) {
            region = &mRegionA[slot];
        } else {
            region = &mRegionB[slot];
        }
    }
    return region;
}

// The loop goes through an inline that receives the manager: in the constructor body the index and the
// region offset take swapped registers.
// inferred: the regions of every slot
static inline void InitRegions8x32_unk(TextureCacheManager8x32* m) {
    unsigned int even;
    unsigned int odd;
    int i;
    for (i = 0, odd = 0x80000, even = 0; i < m->mCount; i++, odd += 0x10000, even += 0x10000) {
        GXInitTexCacheRegion(&m->mRegionA[i], 0, even, 0, odd, 0);
        GXInitTexCacheRegion(&m->mRegionB[i], 0, even, 0, 0, 3);
        GXInitTexCacheRegion(&m->mRegionC[i], 1, even, 0, odd, 0);
    }
}

TextureCacheManager8x32::TextureCacheManager8x32() {
    m04 = 0;
    mCount = 8;
    InitRegions8x32_unk(this);
}

GXTexRegion* TextureCacheManager8x32::vf0C(GXTexObj* obj, unsigned int id) {
    return fn_800D6898(obj, id);
}

// inferred: the regions of every slot
static inline void InitRegions4x128_unk(TextureCacheManager4x128* m) {
    unsigned int even;
    unsigned int odd;
    int i;
    for (i = 0, odd = 0x80000, even = 0; i < m->mCount; i++, odd += 0x20000, even += 0x20000) {
        GXInitTexCacheRegion(&m->mRegionA[i], 0, even, 1, odd, 1);
        GXInitTexCacheRegion(&m->mRegionB[i], 0, even, 1, 0, 3);
        GXInitTexCacheRegion(&m->mRegionC[i], 1, even, 0, odd, 0);
    }
}

TextureCacheManager4x128::TextureCacheManager4x128() {
    m04 = 0;
    mCount = 4;
    InitRegions4x128_unk(this);
}

GXTexRegion* TextureCacheManager4x128::vf0C(GXTexObj* obj, unsigned int id) {
    return fn_800D6898(obj, id);
}

} // namespace atn
