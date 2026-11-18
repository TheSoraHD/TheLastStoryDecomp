#ifndef ATN_TEXTURECOMPOSITOR_UNK_H
#define ATN_TEXTURECOMPOSITOR_UNK_H

#include "atn/Color.h"

// inferred: interface of the texture compositors (real name from the RTTI: __RTTI__Q23atn18ITextureCompositor
// 0x80779860) and its reference to the cache of composed textures. Used by atn/Model.cpp (composite textures
// of a model) and atn/CompositeIntensityTintMulti.cpp. The bodies of the inline members are in
// atn/CompositeIntensityTintMulti.cpp (an inline with a body here would renumber @N in every includer).

namespace atn {

class Texture;
typedef Texture Texture_unk; // atn/Texture_unk.h
struct CompositeCacheEntry_unk;

// inferred: reference to an entry of the composite cache (constructor 0x800D87A4, AddRef_unk 0x800D87B0,
// Release_unk 0x800D87C4 are plain functions of atn/CompositeIntensityTintMulti.cpp; fn_800D8808 is inline
// there, atn/Model.cpp calls its weak copy)
struct CompositeCacheHandle_unk {
    CompositeCacheHandle_unk();
    void AddRef_unk() const;
    void Release_unk();
    Texture_unk* fn_800D8808(); // inferred: texture of the entry
    CompositeCacheEntry_unk* mEntry;
};

class ITextureCompositor {
public:
    ITextureCompositor();
    virtual ~ITextureCompositor();
    virtual int vf0C() = 0;                // inferred: still loading
    virtual int vf10() = 0;                // inferred: finished
    virtual void vf14() = 0;               // inferred: starts composing
    virtual Color4f_unk* vf18(int) = 0;    // inferred: colour of a layer

    CompositeCacheHandle_unk mHandle; // 0x04
    int m08;                          // inferred: nonzero once started
};

} // namespace atn

#endif
