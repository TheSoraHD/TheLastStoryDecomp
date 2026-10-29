#ifndef ATN_GRAPHICSDEVICE_H
#define ATN_GRAPHICSDEVICE_H

// SDK structures used by the graphics device (the SDK headers are not on the game include path;
// same layout and tags as revolution/gx and revolution/vi).
struct GXRenderModeObj {
    int viTVmode;                         // 0x00
    unsigned short fbWidth;               // 0x04
    unsigned short efbHeight;             // 0x06
    unsigned short xfbHeight;             // 0x08
    unsigned short viXOrigin;             // 0x0A
    unsigned short viYOrigin;             // 0x0C
    unsigned short viWidth;               // 0x0E
    unsigned short viHeight;              // 0x10
    int xFBmode;                          // 0x14
    unsigned char field_rendering;        // 0x18
    unsigned char aa;                     // 0x19
    unsigned char sample_pattern[12][2];  // 0x1A
    unsigned char vfilter[7];             // 0x32
};
struct GXVtxDescList {
    int attr; // 0x00
    int type; // 0x04
};
struct GXVtxAttrFmtList {
    int attr;           // 0x00
    int cnt;            // 0x04
    int type;           // 0x08
    unsigned char frac; // 0x0C
};
struct GXFifoObj;

struct Light_unk;

namespace atn {

class Texture_unk;
class ITextureCacheManager;

// inferred: vertex format of a model (attribute formats, then descriptors; 26 entries + GX_VA_NULL),
// built by LoadModel (atn/LH_Model) and compared before loading it (fn_80071088)
struct VtxAttrFmtArray_unk {
    VtxAttrFmtArray_unk() {}
    VtxAttrFmtArray_unk(const VtxAttrFmtArray_unk& other) {
        for (int i = 0; i < 27; i++) {
            mList[i] = other.mList[i];
        }
    }
    GXVtxAttrFmtList mList[27];
};
struct VtxDescArray_unk {
    VtxDescArray_unk() {}
    VtxDescArray_unk(const VtxDescArray_unk& other) {
        for (int i = 0; i < 27; i++) {
            mList[i] = other.mList[i];
        }
    }
    GXVtxDescList mList[27];
};
struct VtxFormat_unk {
    VtxFormat_unk(const VtxDescArray_unk& desc, const VtxAttrFmtArray_unk& fmt); // 0x80070E14
    VtxFormat_unk(const VtxFormat_unk& other) {
        for (int i = 0; i < 27; i++) {
            mAttrFmt.mList[i] = other.mAttrFmt.mList[i];
        }
        for (int i = 0; i < 27; i++) {
            mDesc.mList[i] = other.mDesc.mList[i];
        }
    }

    VtxAttrFmtArray_unk mAttrFmt; // 0x000
    VtxDescArray_unk mDesc;       // 0x1B0
};

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

// inferred: one dirty bit per field of GXStateBits_unk (bits 0-11, in field order)
struct GXDirtyBits_unk {
    GXDirtyBits_unk() : mMask(0) {}
    unsigned int mMask : 12;
    unsigned int mLow : 20;
};

// inferred: graphics device / GX render state (lbl_8087EEE0, 0x8E8 bytes; atn_graphics_device.cpp).
// Name and fields from the functions of atn/GraphicsDevice.cpp (0x80070E14-0x8007708C) and its users
// (game/main.cpp, game/HomeButton_unk.cpp, atn/Layer3D.cpp). One type for every file: do not add
// another partial view of this object.
struct GraphicsDevice_unk {
    // inferred: vertex format shared by the models that use it (fn_80076C44, fn_80076F88)
    struct VtxFormatRef_unk {
        VtxFormatRef_unk() : mFormat(0), mRefs(0) {}
        VtxFormat_unk* mFormat; // 0x00
        int mRefs;              // 0x04
    };

    GraphicsDevice_unk(); // 0x80071274

    GXRenderModeObj mRMode;               // 0x00: adjusted for overscan (fn_800713D4)
    int mWidth;                           // 0x3C: EFB size used by the texture copy (fn_80071D80)
    int mHeight;                          // 0x40
    int mWide;                            // 0x44: SCGetAspectRatio (16:9)
    unsigned int mClearColor;             // 0x48: inferred: GXColor as a word
    unsigned int mClearZ;                 // 0x4C
    GXFifoObj* mFifo;                     // 0x50
    void* mFifoBuffer;                    // 0x54
    float mOffsetX;                       // 0x58: inferred: added to the viewport and scissor
    float mOffsetY;                       // 0x5C
    int mViewX;                           // 0x60: viewport/scissor rectangle (fn_800760D0)
    int mViewY;                           // 0x64
    int mViewW;                           // 0x68
    int mViewH;                           // 0x6C
    void* mXfb0;                          // 0x70
    void* mXfb1;                          // 0x74
    void* mCurrentXfb;                    // 0x78
    void* mNextXfb;                       // 0x7C
    const void* mTexObjs[8];              // 0x80: GXTexObj loaded in each map (fn_800763C0)
    Light_unk* mLights[8];                // 0xA0: light loaded in each GX light (fn_8007642C)
    int mC0;
    int mC4;
    int mC8;
    int mCC;                              // 0xCC (fn_80071D60)
    GXStateBits_unk mState;               // 0xD0: requested state
    GXStateBits_unk mAppliedState;        // 0xD4: state last sent to GX (fn_800761A8)
    GXDirtyBits_unk mDirty;               // 0xD8: fields of mState that differ from mAppliedState
    const VtxFormat_unk* mVtxFormat;      // 0xDC: current vertex format (fn_80076BE4)
    VtxFormatRef_unk mVtxFormats[256];    // 0xE0: cache of vertex formats (fn_80076C44)
    ITextureCacheManager* mTexCache;      // 0x8E0: texture cache of the region callback (fn_80076FD4)
    void* mPrevRegionCallback;            // 0x8E4
};

} // namespace atn

// The class name is not known (no RTTI), so the functions keep their C names (callers in other units
// declare them so).
extern "C" int fn_80071088(const atn::VtxFormat_unk* a, const atn::VtxFormat_unk* b);
extern "C" void fn_80071220();
extern "C" int fn_800713D4(atn::GraphicsDevice_unk* device, int width, int height);
extern "C" void fn_80071D60(atn::GraphicsDevice_unk* device, int value);
extern "C" void fn_80071D68(atn::GraphicsDevice_unk* device, const float projection[4][4]);
extern "C" void fn_80071D74(atn::GraphicsDevice_unk* device, const float projection[4][4]);
extern "C" void fn_80071D80(atn::GraphicsDevice_unk* device, atn::Texture_unk* texture);
extern "C" void fn_800760BC(atn::GraphicsDevice_unk* device, unsigned int left, unsigned int top,
                            unsigned int width, unsigned int height);
extern "C" void fn_800760D0(atn::GraphicsDevice_unk* device);
extern "C" void fn_80076174(atn::GraphicsDevice_unk* device, int enabled);
extern "C" void fn_800763C0(atn::GraphicsDevice_unk* device, unsigned int map, const void* texObj);
extern "C" void fn_800763FC(atn::GraphicsDevice_unk* device, const void* texObj, unsigned int map);
extern "C" void fn_8007642C(atn::GraphicsDevice_unk* device, int index, Light_unk* light);
extern "C" int fn_80076C44(atn::GraphicsDevice_unk* device, const atn::VtxFormat_unk* format);
extern "C" void fn_80076F88(atn::GraphicsDevice_unk* device, unsigned int index);

#endif
