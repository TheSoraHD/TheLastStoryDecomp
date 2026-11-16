#ifndef ATN_TEXTURECACHEMANAGER_H
#define ATN_TEXTURECACHEMANAGER_H

#include "atn/GXDecl.h"

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

// inferred: no destructor declared in the derived classes: the implicit ones are emitted in the order of
// the original (4x128 first, 0x800D6B60); user-declared empty ones come out the other way round
class TextureCacheManager4x128 : public ITextureCacheManager {
public:
    TextureCacheManager4x128(); // 0x800D6A84
    virtual GXTexRegion* vf0C(GXTexObj* obj, unsigned int id);
};

class TextureCacheManager8x32 : public ITextureCacheManager {
public:
    TextureCacheManager8x32(); // 0x800D69A8
    virtual GXTexRegion* vf0C(GXTexObj* obj, unsigned int id);
};

} // namespace atn

#endif
