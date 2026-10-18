// NONMATCHING: ctor, vf14, vf10 and dtor match; vf1C (78%) and vf20 (47%) differ only in the
// 8-byte float pair copies, which the original emits as psq_l/psq_st. Wii/1.0-1.3 and GC/3.0a*
// emit lfs/stfs pairs; to be retried with Wii/1.5+.
#include "atn/LayerPostFx.h"

// inferred: unknown callees and globals (automatic names)
struct RenderObj {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10(void* arg);
};
struct RenderNode {
    RenderObj* mObj;   // 0x00
    void* m04;
    RenderNode* mNext; // 0x08
};
struct RenderState {
    char pad[0x58];
    atn::Vec2f m58;
};

extern "C" void* lbl_8087EFB4;
extern "C" void* lbl_8087EF8C;
extern "C" RenderState* lbl_8087EEE0;
extern "C" void fn_800A9788(void*);
extern "C" void fn_800A9794(void*);
extern "C" void fn_80071E04(RenderState*);
extern "C" void fn_80076174(RenderState*, int);
extern "C" void fn_800C0590(void*, int, int);

namespace atn {

LayerPostFx::~LayerPostFx() {}

void LayerPostFx::vf10() {}

void LayerPostFx::vf20() {
    lbl_8087EEE0->m58 = mSaved;
    fn_80076174(lbl_8087EEE0, 1);
}

void LayerPostFx::vf1C() {
    Vec2f zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    mSaved = lbl_8087EEE0->m58;
    lbl_8087EEE0->m58 = zero;
    fn_80071E04(lbl_8087EEE0);
    fn_80076174(lbl_8087EEE0, 0);
    fn_800C0590(lbl_8087EFB4, 0, 0x13);
}

void LayerPostFx::vf14(void* arg) {
    void* p = lbl_8087EFB4;
    for (RenderNode* node = (RenderNode*)arg; node != 0; node = node->mNext) {
        fn_800A9788(lbl_8087EF8C);
        node->mObj->vf10(p);
        fn_800A9794(lbl_8087EF8C);
    }
}

LayerPostFx::LayerPostFx() : IRenderLayer("PostFX") {}

} // namespace atn
