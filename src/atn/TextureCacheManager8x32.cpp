#include "atn/TextureCacheManager.h"

namespace atn {

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
