#include "atn/Layer2DBlur.h"
#include "atn/GXDecl.h"

extern "C" double sin(double x);
extern "C" double cos(double x);

// inferred: globals of unknown classes
struct RenderMgr_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
struct DebugParams_unk {
    unsigned char m000[0x418];
    float m418; // inferred: blur scale X
    float m41C; // inferred: blur scale Y
    float m420; // inferred: blur angle
};
struct FrameBuffer_unk {
    unsigned char m000[0x74C];
    GXTexObj m74C;
};
extern "C" RenderMgr_unk* lbl_8087EEE0;
extern "C" DebugParams_unk* lbl_8087EFA8;
extern "C" FrameBuffer_unk* lbl_8087EFB4;
extern "C" void* lbl_8087EF8C;
extern "C" void fn_800C0508(FrameBuffer_unk* fb, int arg);
extern "C" void fn_800BFB70(FrameBuffer_unk* fb);
extern "C" void* fn_800A96AC(void* pool, int width, int height, int format); // inferred: temporary texture
extern "C" void fn_800A981C(GXTexObj* src, void* dst, int width, int height, int format, int arg5, int arg6,
                            float offsetX, float offsetY); // inferred: directional blur pass
extern "C" void fn_800763FC(RenderMgr_unk* mgr, GXTexObj* obj, int map);
extern "C" void fn_80075F58(RenderMgr_unk* mgr, int x, int y, int w, int h);
extern "C" void fn_80075DEC(RenderMgr_unk* mgr);

namespace atn {

Layer2DBlur::Layer2DBlur() : Layer2D("Blur2D") {}

void Layer2DBlur::vf1C() {
    Layer2D::vf1C();
}

// NONMATCHING: 89.6% - our compiler treats sin/cos as pure: it keeps lbl_8087EFA8 and m418 across
// the calls (target reloads both), and it also shares w * 2 between the viewport and copy calls,
// which costs two extra saved GPRs. Possibly a compiler version difference (see the psq copies).
void Layer2DBlur::vf20() {
    int w = lbl_8087EEE0->mWidth;
    int h = lbl_8087EEE0->mHeight;
    GXPixModeSync();
    fn_800C0508(lbl_8087EFB4, 1);
    w /= 4;
    h /= 4;
    GXTexObj obj0;
    void* image0 = fn_800A96AC(lbl_8087EF8C, w, h, 6);
    GXInitTexObj(&obj0, image0, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&obj0, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXTexObj obj1;
    void* image1 = fn_800A96AC(lbl_8087EF8C, w, h, 6);
    GXInitTexObj(&obj1, image1, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&obj1, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevDirect(0);
    fn_800763FC(lbl_8087EEE0, &lbl_8087EFB4->m74C, 0);
    fn_80075F58(lbl_8087EEE0, 0, 0, w * 2, h * 2);
    fn_800BFB70(lbl_8087EFB4);
    GXSetTexCopySrc(0, 0, w * 2, h * 2);
    GXSetTexCopyDst(w, h, 6, 1);
    GXCopyTex(image0, 1);
    GXPixModeSync();
    fn_800A981C(&obj0, image1, w, h, 6, 0, 0, (float)cos(lbl_8087EFA8->m420) * lbl_8087EFA8->m418,
                -(float)sin(lbl_8087EFA8->m420) * lbl_8087EFA8->m418);
    fn_800A981C(&obj1, image0, w, h, 6, 0, 0, -(float)sin(lbl_8087EFA8->m420) * lbl_8087EFA8->m41C,
                (float)cos(lbl_8087EFA8->m420) * lbl_8087EFA8->m41C);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    fn_800763FC(lbl_8087EEE0, &obj0, 0);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTevDirect(0);
    GXSetBlendMode(0, 1, 0, 3);
    fn_80075DEC(lbl_8087EEE0);
    fn_800BFB70(lbl_8087EFB4);
    Layer2D::vf20();
}

} // namespace atn
