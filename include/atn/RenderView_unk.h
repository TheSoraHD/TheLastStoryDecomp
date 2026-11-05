#ifndef ATN_RENDERVIEW_UNK_H
#define ATN_RENDERVIEW_UNK_H

#include "atn/Camera_unk.h"
#include "atn/IRender.h"

struct NewCampScene2_unk;

namespace atn {

// inferred: render system passed to the IRender draw virtuals (lbl_8087EFB4, atn/RenderSystem_unk.cpp);
// only the camera used by Event::DialogHead is declared. Name invented.
struct RenderView_unk {
    unsigned char m000[0x104];
    Camera_unk mCamera; // 0x104
    int m2F8; // inferred: render pass (3: shadow pass, ModelBase::fn_80091684; 4: one matrix per item)
    inline int GetPass_unk() const; // inferred: weak copy 0x8008E2D4 (defined in atn/Model.cpp); name invented
    NewCampScene2_unk* mScene; // 0x2FC: inferred: lights and cameras of the scene (atn/TextureDraw_unk.cpp)
    unsigned char m300[0x54964 - 0x300];
    int m54964; // inferred: set by Event::ActionCheck (type 1)
    void SetCamera_unk(const Camera_unk& camera) { mCamera = camera; } // inferred: name invented
};

} // namespace atn

// inferred: the render system (created by atn/RenderSystem_unk.cpp; its camera at 0x104 is the camera of
// the game, its scene at 0x2FC the scene drawn). One type for all the users.
extern "C" atn::RenderView_unk* lbl_8087EFB4;

#endif
