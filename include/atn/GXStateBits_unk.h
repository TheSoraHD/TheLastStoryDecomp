#ifndef ATN_GXSTATEBITS_UNK_H
#define ATN_GXSTATEBITS_UNK_H

namespace atn {

// inferred: GX pixel state cached by the device and sent by fn_800761A8 (bits from the most
// significant; values are indices into the translation tables of fn_800761A8)
struct GXStateBits_unk {
    unsigned int mAlphaTest : 1;  // dirty bits 0-2: GXSetAlphaCompare / GXSetZCompLoc
    unsigned int mAlphaFunc : 3;
    unsigned int mAlphaRef : 8;
    unsigned int mBlend : 1;      // dirty bits 3-5: GXSetBlendMode
    unsigned int mBlendSrc : 3;
    unsigned int mBlendDst : 3;
    unsigned int mCull : 1;       // dirty bit 6: GXSetCullMode
    unsigned int m20 : 1;
    unsigned int mZTest : 1;      // dirty bits 8-10: GXSetZMode
    unsigned int mZFunc : 3;
    unsigned int mZWrite : 1;
    unsigned int mUpdateMask : 2; // dirty bit 11: bit 1 GXSetColorUpdate, bit 0 GXSetAlphaUpdate
    unsigned int m28 : 4;
};

// (also the state word of each material: Material_unk::mState, passed to fn_80076A28)

} // namespace atn

#endif
