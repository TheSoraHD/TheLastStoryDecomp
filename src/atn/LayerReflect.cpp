#include "atn/LayerReflect.h"

// inferred: render manager (class unknown), global instance at 0x8087EEE0
struct RenderMgr_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
    unsigned char m44[0x48 - 0x44];
    unsigned int mClearColor; // 0x48: ARGB
};
struct DebugFlags_unk {
    unsigned char m000[0x308];
    int m308; // inferred: show debug textures
};
extern "C" RenderMgr_unk* lbl_8087EEE0;
extern "C" DebugFlags_unk* lbl_8087EFA8;
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80075F58(RenderMgr_unk* mgr, int x, int y, int w, int h); // inferred: set viewport
extern "C" void fn_80075DEC(RenderMgr_unk* mgr);
extern "C" void fn_8005DFC8(void* renderer, int color, atn::Texture_unk* tex, int arg, float x, float y, float z,
                            float w, float h); // inferred: draw texture

namespace atn {

// inferred: ARGB word to GXColor (inline, name invented)
inline GXColor fn_ToGXColor(unsigned int argb) {
    GXColor color;
    color.r = argb >> 16;
    color.g = argb >> 8;
    color.b = argb;
    color.a = argb >> 24;
    return color;
}

LayerReflect::LayerReflect() : Layer3D("Reflect", 3, -27) {
    mTexture.fn_800D5B58(lbl_8087EEE0->mWidth / 2, lbl_8087EEE0->mHeight / 2, 4, 0, "ReflectionTexture");
    GXInitTexObjLOD(&mTexture.mObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
}

void LayerReflect::vf1C() {
    RenderMgr_unk* mgr = lbl_8087EEE0;
    Layer3D::vf1C();
    int h = GXGetTexObjHeight(&mTexture.mObj);
    int w = GXGetTexObjWidth(&mTexture.mObj);
    fn_80075F58(mgr, 0, 0, w, h);
}

void LayerReflect::vf20() {
    RenderMgr_unk* mgr = lbl_8087EEE0;
    int w = GXGetTexObjWidth(&mTexture.mObj);
    int h = GXGetTexObjHeight(&mTexture.mObj);
    unsigned int argb = mgr->mClearColor;
    void* image = mTexture.mImage;
    GXSetCopyClear(fn_ToGXColor(argb), 0xFFFFFF);
    GXSetZMode(1, 3, 1);
    GXSetTexCopySrc(0, 0, w, h);
    GXSetTexCopyDst(w, h, 4, 0);
    GXCopyTex(image, 1);
    GXPixModeSync();
    Layer3D::vf20();
    fn_80075DEC(lbl_8087EEE0);
    if (lbl_8087EFA8->m308 != 0) {
        RenderMgr_unk* m = lbl_8087EEE0;
        int height = m->mHeight;
        fn_8005DFC8(lbl_8087EEB0, -1, &mTexture, 0, 32.0f, (float)(height - height / 4) - 16.0f, 0.0f,
                    (float)m->mWidth / 4.0f, (float)height / 4.0f);
    }
}

} // namespace atn
