// The 8-byte float pair copies are psq_l/psq_st in the original: Vec2f copies through __Vec2Copy
// (math/Vec.h, approved paired-single helper).
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
namespace atn {
// inferred: see atn/LinearAllocator.cpp
class PostFxManager {
public:
    void fn_800A9788();
    void fn_800A9794();
};
} // namespace atn
extern "C" atn::PostFxManager* lbl_8087EF8C;
extern "C" RenderState* lbl_8087EEE0;
extern "C" void fn_80071E04(RenderState*);
extern "C" void fn_80076174(RenderState*, int);
extern "C" void fn_800C0590(void*, int, int);

namespace atn {

LayerPostFx::LayerPostFx() : IRenderLayer("PostFX") {}

void LayerPostFx::vf14(void* arg) {
    void* p = lbl_8087EFB4;
    for (RenderNode* node = (RenderNode*)arg; node != 0; node = node->mNext) {
        lbl_8087EF8C->fn_800A9788();
        node->mObj->vf10(p);
        lbl_8087EF8C->fn_800A9794();
    }
}

void LayerPostFx::vf1C() {
    Vec2f zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    // inferred: the save is a direct paired-single copy (assigning through Vec2f::operator= swaps r4/r5)
    __Vec2Copy(&mSaved.x, &lbl_8087EEE0->m58.x);
    lbl_8087EEE0->m58 = zero;
    fn_80071E04(lbl_8087EEE0);
    fn_80076174(lbl_8087EEE0, 0);
    fn_800C0590(lbl_8087EFB4, 0, 0x13);
}

void LayerPostFx::vf20() {
    lbl_8087EEE0->m58 = mSaved;
    fn_80076174(lbl_8087EEE0, 1);
}

void LayerPostFx::vf10() {}

LayerPostFx::~LayerPostFx() {}

} // namespace atn
