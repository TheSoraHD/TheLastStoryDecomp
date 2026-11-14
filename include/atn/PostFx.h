#ifndef ATN_POSTFX_H
#define ATN_POSTFX_H

#include "atn/GXDecl.h"
#include "atn/RenderManager.h"
#include "atn/PostFxManager.h"
#include "atn/IRender.h"

// inferred: render manager (lbl_8087EEE0; see atn/Layer2DBlur.cpp)
struct PostFxRenderMgr_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};

extern "C" PostFxRenderMgr_unk* lbl_8087EEE0;
extern "C" void fn_800763FC(PostFxRenderMgr_unk* mgr, GXTexObj* obj, int map); // inferred: binds a texture
extern "C" void fn_80075F58(PostFxRenderMgr_unk* mgr, int x, int y, int w, int h); // inferred: viewport
extern "C" void fn_80075DEC(PostFxRenderMgr_unk* mgr);  // inferred: restores the viewport

namespace atn {

// inferred: base of the post effects (only its RTTI is known, emitted by atn/PostFxBloom.cpp)
class IPostFx : public IRender {};


} // namespace atn


#endif
