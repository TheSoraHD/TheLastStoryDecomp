#ifndef ATN_IRENDER_H
#define ATN_IRENDER_H

#include "atn/Camera_unk.h"

struct NewCampScene2_unk;

namespace atn {

// inferred: render system passed to the IRender draw virtuals (lbl_8087EFB4, atn/RenderSystem_unk.cpp);
// only the camera used by Event::DialogHead is declared. Name invented.
struct RenderView_unk {
    unsigned char m000[0x104];
    Camera_unk mCamera; // 0x104
    unsigned char m2F8[4];
    NewCampScene2_unk* mScene; // 0x2FC: inferred: lights and cameras of the scene (atn/TextureDraw_unk.cpp)
    void SetCamera_unk(const Camera_unk& camera) { mCamera = camera; } // inferred: name invented
};

// inferred: renderer interface (RTTI __RTTI__Q23atn7IRender; base of atn::ModelBase and Event::DialogHead).
// Other files still keep local copies (atn/LensFlare.h, atn/PostFx.h, atn/VideoTHP.cpp, ...). The first
// virtual is named GetRenderType_unk instead of vf0C: Event::DialogHead overrides it apart from
// atn::Task::vf0C, which has the same signature (the base symbol 0x80046218 keeps the name
// vf0C__Q23atn7IRenderFv; no file that includes this header uses it).
class IRender {
public:
    virtual ~IRender() {}
    virtual int GetRenderType_unk();                          // 0x80046218
    virtual void vf10(RenderView_unk* view) = 0;              // inferred: draw
    virtual int vf14(void* arg1, void* arg2, void* frustum); // 0x80046200
    virtual void vf18(RenderView_unk* view);                  // 0x80046214: inferred: second draw pass
    virtual void vf1C();                                      // 0x800461FC
};

} // namespace atn

#endif
