#ifndef ATN_TEXTURE_UNK_H
#define ATN_TEXTURE_UNK_H

#include "atn/GXDecl.h"
#include "atn/LH_Texture.h"

namespace atn {

// inferred: render target texture (0x30 bytes); the real class name is unknown.
class Texture_unk {
public:
    Texture_unk();  // 0x800D5738
    ~Texture_unk(); // 0x800D5808
    void fn_800D5B58(int width, int height, int format, int arg, const char* name); // inferred: allocate

    GXTexObj mObj;              // 0x00
    LH_Texture m20;             // 0x20: inferred: stored with the LH_Texture vtable by the copy 0x805143C8
    void* mImage;               // 0x28
    unsigned char m2C;
    unsigned char m2D; // inferred: flags set after a load (LoadEffect::vf0C)
    unsigned char m2E;
    unsigned char m2F;
};

} // namespace atn

#endif
