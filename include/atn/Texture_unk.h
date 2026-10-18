#ifndef ATN_TEXTURE_UNK_H
#define ATN_TEXTURE_UNK_H

#include "atn/GXDecl.h"

namespace atn {

// inferred: render target texture (0x30 bytes); the real class name is unknown.
class Texture_unk {
public:
    Texture_unk();  // 0x800D5738
    ~Texture_unk(); // 0x800D5808
    void fn_800D5B58(int width, int height, int format, int arg, const char* name); // inferred: allocate

    GXTexObj mObj;              // 0x00
    unsigned char m20[0x28 - 0x20];
    void* mImage;               // 0x28
    unsigned char m2C;
    unsigned char m2D; // inferred: flags set after a load (LoadEffect::vf0C)
    unsigned char m2E;
    unsigned char m2F;
};

} // namespace atn

#endif
