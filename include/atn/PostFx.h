#ifndef ATN_POSTFX_H
#define ATN_POSTFX_H

#include "atn/GXDecl.h"

// inferred: render manager (lbl_8087EEE0; see atn/Layer2DBlur.cpp)
struct PostFxRenderMgr_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
struct PostFxFrameBuffer_unk;

extern "C" PostFxRenderMgr_unk* lbl_8087EEE0;
extern "C" PostFxFrameBuffer_unk* lbl_8087EFB4;
extern "C" void fn_800763FC(PostFxRenderMgr_unk* mgr, GXTexObj* obj, int map); // inferred: binds a texture
extern "C" void fn_80075F58(PostFxRenderMgr_unk* mgr, int x, int y, int w, int h); // inferred: viewport
extern "C" void fn_80075DEC(PostFxRenderMgr_unk* mgr);  // inferred: restores the viewport
extern "C" void fn_800BFB70(PostFxFrameBuffer_unk* fb); // inferred: draws a full screen quad

namespace atn {

// inferred: renderer interface (local copies in other files, e.g. atn/LinearAllocator.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C(); // 0x80046218
    virtual void vf10(void* camera) = 0;
    virtual int vf14(void* arg1, void* arg2, void* frustum);
    virtual void vf18();
    virtual void vf1C(); // 0x800461FC
};

// inferred: base of the post effects (only its RTTI is known, emitted by atn/PostFxBloom.cpp)
class IPostFx : public IRender {};

// inferred: owner of the post effects (see atn/LinearAllocator.cpp)
class PostFxManager {
public:
    void* fn_800A96AC(int width, int height, int format); // inferred: temporary texture
};

} // namespace atn

extern "C" atn::PostFxManager* lbl_8087EF8C;

#endif
