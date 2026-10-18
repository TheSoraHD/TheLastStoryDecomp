#ifndef ATN_TEXTURECACHEMANAGER_H
#define ATN_TEXTURECACHEMANAGER_H

// Minimal GX declarations used by the texture cache managers.
struct GXTexObj;
struct GXTexRegion {
    unsigned int dummy[4];
};
extern "C" void GXInitTexCacheRegion(GXTexRegion* region, unsigned char is32bMipmap, unsigned int tmemEven,
                                     int sizeEven, unsigned int tmemOdd, int sizeOdd);

namespace atn {

// inferred: base of the GX texture region allocators (vtable not present in the DOL)
class ITextureCacheManager {
public:
    ITextureCacheManager() {}
    virtual ~ITextureCacheManager() {}
    virtual GXTexRegion* vf0C(GXTexObj* obj, unsigned int id) = 0; // inferred: region callback

    GXTexRegion* fn_800D6898(GXTexObj* obj, unsigned int id); // inferred: shared region lookup

    int m04;
    int mCount;               // 0x08
    GXTexRegion mRegionA[8];  // 0x0C
    GXTexRegion mRegionB[8];  // 0x8C
    GXTexRegion mRegionC[8];  // 0x10C
    unsigned int mIds[8];     // 0x18C
};

class TextureCacheManager4x128 : public ITextureCacheManager {
public:
    TextureCacheManager4x128(); // 0x800D6A84
    virtual ~TextureCacheManager4x128() {}
    virtual GXTexRegion* vf0C(GXTexObj* obj, unsigned int id);
};

class TextureCacheManager8x32 : public ITextureCacheManager {
public:
    TextureCacheManager8x32(); // 0x800D69A8
    virtual ~TextureCacheManager8x32() {}
    virtual GXTexRegion* vf0C(GXTexObj* obj, unsigned int id);
};

} // namespace atn

#endif
