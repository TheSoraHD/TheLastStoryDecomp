#include "atn/RenderSetting_unk.h"
#include "atn/GraphicsDevice.h"
#include "atn/Layer2DBlur.h"
#include "atn/GXDecl.h"
#include "atn/RenderManager.h"

extern "C" double sin(double x);
extern "C" double cos(double x);
// inferred: float wrappers of sin/cos (inlined: the target reloads lbl_8087EFA8 after each call)
static inline float SinF_unk(float x) { return sin(x); }
static inline float CosF_unk(float x) { return cos(x); }

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" RenderSetting_unk* lbl_8087EFA8;
// inferred: blur size (read after the sin/cos call of each argument, as in the target); names invented
static inline float BlurWidth_unk(const RenderSetting_unk* r) { return r->m418; }
static inline float BlurHeight_unk(const RenderSetting_unk* r) { return r->m41C; }
namespace atn {
// inferred: see atn/LinearAllocator.cpp
class PostFxManager {
public:
    void* fn_800A96AC(int width, int height, int format); // inferred: temporary texture
};
} // namespace atn
extern "C" atn::PostFxManager* lbl_8087EF8C;
extern "C" void fn_800A981C(GXTexObj* src, void* dst, int width, int height, int format, void* arg5, int arg6,
                            float offsetX, float offsetY); // inferred: directional blur pass
extern "C" void fn_80075F58(atn::GraphicsDevice_unk* mgr, int x, int y, int w, int h);
extern "C" void fn_80075DEC(atn::GraphicsDevice_unk* mgr);

namespace atn {

Layer2DBlur::Layer2DBlur() : Layer2D("Blur2D") {}

void Layer2DBlur::vf1C() {
    Layer2D::vf1C();
}

// The copy size is (unsigned short)w * 2: with w * 2 it is shared with the viewport call.
void Layer2DBlur::vf20() {
    int w = lbl_8087EEE0->mWidth;
    int h = lbl_8087EEE0->mHeight;
    GXPixModeSync();
    atn::RenderManager::instance_->fn_800C0508(1);
    w /= 4;
    h /= 4;
    GXTexObj obj0;
    void* image0 = lbl_8087EF8C->fn_800A96AC(w, h, 6);
    GXInitTexObj(&obj0, image0, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&obj0, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXTexObj obj1;
    void* image1 = lbl_8087EF8C->fn_800A96AC(w, h, 6);
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
    fn_800763FC(lbl_8087EEE0, &atn::RenderManager::instance_->m74C.mObj, 0);
    fn_80075F58(lbl_8087EEE0, 0, 0, w * 2, h * 2);
    atn::RenderManager::instance_->fn_800BFB70();
    GXSetTexCopySrc(0, 0, (unsigned short)w * 2, (unsigned short)h * 2);
    GXSetTexCopyDst(w, h, 6, 1);
    GXCopyTex(image0, 1);
    GXPixModeSync();
    fn_800A981C(&obj0, image1, w, h, 6, 0, 0, CosF_unk(lbl_8087EFA8->m420) * BlurWidth_unk(lbl_8087EFA8),
                -SinF_unk(lbl_8087EFA8->m420) * BlurWidth_unk(lbl_8087EFA8));
    fn_800A981C(&obj1, image0, w, h, 6, 0, 0, -SinF_unk(lbl_8087EFA8->m420) * BlurHeight_unk(lbl_8087EFA8),
                CosF_unk(lbl_8087EFA8->m420) * BlurHeight_unk(lbl_8087EFA8));
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
    atn::RenderManager::instance_->fn_800BFB70();
    Layer2D::vf20();
}

} // namespace atn
