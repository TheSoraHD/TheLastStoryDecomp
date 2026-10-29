#ifndef ATN_GRAPHICSDEVICE_H
#define ATN_GRAPHICSDEVICE_H

namespace atn {

class Texture_unk;

// inferred: graphics device / GX render state (lbl_8087EEE0). Name and fields from the functions of
// atn/GraphicsDevice.cpp (0x80071D60-0x80076F88) and its users (game/HomeButton_unk.cpp,
// atn/Layer3D.cpp). One type for every file: do not add another partial view of this object.
struct GraphicsDevice_unk {
    // inferred: one display-list allocation shared by several users (fn_80076F88)
    struct DisplayListRef_unk {
        void* mData; // 0x00
        int mRefs;   // 0x04
    };

    unsigned char m00[0x19];
    unsigned char mCopyAntialias;         // 0x19: GXSetCopyFilter arguments (fn_80076174)
    unsigned char mCopySamples[12][2];    // 0x1A
    unsigned char mCopyVertFilter[7];     // 0x32
    unsigned char m39[0x3C - 0x39];
    int mWidth;                           // 0x3C: EFB size used by the texture copy (fn_80071D80)
    int mHeight;                          // 0x40
    int mWide;                            // 0x44: inferred: 16:9 (game/HomeButton_unk.cpp)
    unsigned char m48[0x60 - 0x48];
    unsigned int mScissor[4];             // 0x60: left, top, width, height (fn_800760D0)
    unsigned char m70[0x80 - 0x70];
    const void* mTexObjs[8];              // 0x80: GXTexObj loaded in each map (fn_800763C0)
    unsigned char mA0[0xCC - 0xA0];
    int mCC;                              // 0xCC (fn_80071D60)
    unsigned char mD0[0xE0 - 0xD0];
    DisplayListRef_unk mDisplayLists[1];  // 0xE0: inferred: indexed by fn_80076F88, count unknown
};

// inferred: cached GX state record compared by fn_80071088 (0x30 bytes; 9 of them, then 9 records
// of six words at 0x1B0)
struct GraphicsDeviceState_unk {
    int m00[3];
    unsigned char m0C;
    int m10[3];
    unsigned char m1C;
    int m20[3];
    unsigned char m2C;
};

} // namespace atn

// The class name is not known (no RTTI), so the functions keep their C names.
extern "C" int fn_80071088(const atn::GraphicsDeviceState_unk* a, const atn::GraphicsDeviceState_unk* b);
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
extern "C" void fn_80076F88(atn::GraphicsDevice_unk* device, unsigned int index);

#endif
