#ifndef ATN_D2A_TEXTURE_H
#define ATN_D2A_TEXTURE_H

#include "atn/D2A_Common.h"
#include "atn/Texture_unk.h"

namespace atn {

class D2A_Data;

// inferred: texture animation element; member offsets follow its constructor at 0x801F94F4.
// inferred: four expression pointers applied to one four-float texture value.
struct D2A_TextureExpr_unk {
    D2A_TextureExpr_unk() : mCode(0) {
        for (unsigned int* p = mTail; p < mTail + 3; p++) {
            *p = 0;
        }
    }

    unsigned int mCode;
    unsigned int mTail[3];
};

class D2A_Texture : public D2A_Common {
public:
    D2A_Texture(D2A_Data* owner, unsigned short id);
    virtual ~D2A_Texture();
    virtual void vf0C();
    virtual void vf14();
    virtual void vf18();

    // inferred: two sets of four animated texture coordinates/scales, followed by texture state.
    D2A_Anim_unk<4> mTexRect;  // 0x12C
    D2A_Anim_unk<4> mTexScale; // 0x160
    Texture_unk mTexture0;     // 0x194
    Texture_unk mTexture1;     // 0x1C4
    D2A_TextureExpr_unk mExpr0; // 0x1F4
    D2A_TextureExpr_unk mExpr1; // 0x204
};

} // namespace atn

#endif
