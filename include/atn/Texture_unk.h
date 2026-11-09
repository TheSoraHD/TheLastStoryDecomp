#ifndef ATN_TEXTURE_UNK_H
#define ATN_TEXTURE_UNK_H

#include "atn/GXDecl.h"
#include "atn/LH_Texture.h"

namespace atn {

// Texture of the engine (0x30 bytes). Real name atn::Texture: type name "boost::_bi::bind_t<atn::Texture *,
// boost::_mfi::mf1<atn::Texture *, atn::ModelBase, ...>" in .rodata and atn::Texture::operator= (0x8052E8D4,
// the implicit one: declaring it makes game/NewCampPartsGmk lose its weak copy constructors); Texture_unk is
// the older name used in this project.
class Texture {
public:
    Texture();  // 0x800D5738
    ~Texture(); // 0x800D5808
    void fn_800D5908(const char* path); // inferred: loads a texture file (m20.vf0C)
    int fn_800D5B58(int width, int height, int format, int lods, const char* name); // inferred: allocate (returns 1)

    GXTexObj mObj;              // 0x00
    LH_Texture m20;             // 0x20: inferred: stored with the LH_Texture vtable by the copy 0x805143C8
    void* mImage;               // 0x28
    unsigned char m2C;
    signed char m2D; // inferred: wrap mode S (set to 1 by LoadEffect::vf0C); read with extsb
    signed char m2E; // inferred: wrap mode T
    signed char m2F; // inferred: mip levels of an allocated texture (read with extsb)
};
typedef Texture Texture_unk;

} // namespace atn

#endif
