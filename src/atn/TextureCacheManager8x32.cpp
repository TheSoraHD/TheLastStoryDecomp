#include "atn/TextureCacheManager.h"

extern "C" int GXGetTexObjFmt(const GXTexObj* obj);
extern "C" unsigned char GXGetTexObjMipMap(const GXTexObj* obj);
extern "C" unsigned int fn_806163B0(const GXTexObj* obj); // inferred: GXGetTexObjData (image address)

namespace atn {

// inferred: region of a texture: the slot that already holds its image, or `id` modulo the count;
// no region for the color index formats (8-10)
GXTexRegion* ITextureCacheManager::fn_800D6898(GXTexObj* obj, unsigned int id) {
    int format = GXGetTexObjFmt(obj);
    int mipmap = GXGetTexObjMipMap(obj);
    unsigned int data = fn_806163B0(obj);
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

// NONMATCHING: 99.1% - only the registers of the loop index and the region offset are swapped
// (target: index r31, offset r28). Loop shapes tried: index/shift/multiply forms, explicit
// TMEM variables in every declaration/initialisation order, while loop.
TextureCacheManager8x32::TextureCacheManager8x32() {
    m04 = 0;
    mCount = 8;
    unsigned int odd;
    unsigned int even;
    int i;
    for (i = 0, odd = 0x80000, even = 0; i < mCount; i++, odd += 0x10000, even += 0x10000) {
        GXInitTexCacheRegion(&mRegionA[i], 0, even, 0, odd, 0);
        GXInitTexCacheRegion(&mRegionB[i], 0, even, 0, 0, 3);
        GXInitTexCacheRegion(&mRegionC[i], 1, even, 0, odd, 0);
    }
}

GXTexRegion* TextureCacheManager8x32::vf0C(GXTexObj* obj, unsigned int id) {
    return fn_800D6898(obj, id);
}

// NONMATCHING: same register swap as TextureCacheManager8x32::TextureCacheManager8x32.
TextureCacheManager4x128::TextureCacheManager4x128() {
    m04 = 0;
    mCount = 4;
    unsigned int odd;
    unsigned int even;
    int i;
    for (i = 0, odd = 0x80000, even = 0; i < mCount; i++, odd += 0x20000, even += 0x20000) {
        GXInitTexCacheRegion(&mRegionA[i], 0, even, 1, odd, 1);
        GXInitTexCacheRegion(&mRegionB[i], 0, even, 1, 0, 3);
        GXInitTexCacheRegion(&mRegionC[i], 1, even, 0, odd, 0);
    }
}

GXTexRegion* TextureCacheManager4x128::vf0C(GXTexObj* obj, unsigned int id) {
    return fn_800D6898(obj, id);
}

} // namespace atn
