#ifndef ATN_RENDERVIEW_UNK_H
#define ATN_RENDERVIEW_UNK_H

#include "atn/Camera_unk.h"
#include "atn/Texture_unk.h"
#include "atn/Sphere.h"
#include "atn/Color.h"
#include "atn/DrawCmd_unk.h"
#include "std/string.h"
#include <boost/function.hpp>

struct NewCampScene2_unk;

namespace atn {
class Scene;
class IRender;
class IRenderLayer;
struct AABB_unk;
struct FogSettings;
} // namespace atn

// inferred: callback run around the frame (0x18 bytes; constructor 0x800BD94C in atn/RenderSystem_unk.cpp)
struct RenderCallback_unk {
    RenderCallback_unk(boost::function<void(void*)> func, int arg);

    boost::function<void(void*)> mFunc;
    int mArg;
};

// inferred: camera callback: the scene of the model hit by a sphere (set by MapManager)
typedef boost::function<atn::Scene*(atn::Sphere, void*)> MapHitFunc_unk;

namespace atn {

// inferred: render system (0x54968 bytes; built by fn_800BC798 into lbl_8087EFB4 with the constructor
// 0x800BC874, atn/RenderSystem_unk.cpp). It is passed to the IRender draw virtuals. Name invented; the
// layout comes from the constructor and from its users.
struct RenderView_unk {
    RenderView_unk(); // 0x800BC874

    void fn_800BC468(GXTexObj* tex);              // inferred: builds the light texture of the normals
    void fn_800BC618(GXTexObj* tex, int offset);  // inferred: builds the shadow map texture
    void fn_800BD9C0(int size, const std::string& name); // inferred: allocates the queues and the textures
    void fn_800BDB40(NewCampScene2_unk* scene);   // inferred: sets the scene (null: the default one)
    void fn_800BDB58(IRender* render, float order, int pass); // inferred: queues a draw in a pass
    void fn_800BDBC8();                           // inferred: draws the frame
    void fn_800BFAC4();                           // inferred: empty
    void fn_800BFB70();                           // inferred: draws a full screen quad
    void fn_800BFC18(Color color, float a, float b); // inferred: full screen quad of a color
    void fn_800BFCB4(Color c0, Color c1, Color c2, Color c3, unsigned int width, unsigned int height, float a,
                     float b);
    void fn_800C0508(int clear);                  // inferred: copies the frame buffer
    void fn_800C0590(int clear, int format);
    void fn_800C06B0(const FogSettings* fog);     // inferred: sets the fog
    void fn_800C08B0();                           // inferred: resets the fog
    Matrix3x4 fn_800C0900(int mode, int a, int b, int c); // inferred: texture matrix (0x7A0..0x860)
    int fn_800C0A50(const AABB_unk* box);         // inferred: occlusion test (layer 0)
    Texture_unk* fn_800C0A8C();                   // inferred: texture of layer 4 (reflection)
    Texture_unk* fn_800C0A98();                   // inferred: texture of layer 5
    NewCampScene2_unk* fn_800C122C(Sphere sphere); // inferred: scene of a sphere (mMapHit, else mScene)
    NewCampScene2_unk* fn_800C1424();             // inferred: scene at the camera
    void fn_800C1660(Texture_unk* tex, int format); // inferred: queues a copy of the frame buffer
    float fn_800C169C(const AABB_unk* box);       // inferred: visible fraction (layer 0)
    NewCampScene2_unk* fn_800C16B4();             // inferred: default scene (lbl_8087EFB8)
    void fn_800C16BC();                           // inferred: empty

    // inferred: weak copies 0x80479EA0 and 0x8047A158 (defined in game/main.cpp)
    inline void fn_80479EA0(boost::function<void(void*)> func, int arg);
    inline void fn_8047A158(boost::function<void(void*)> func, int arg);
    // inferred: weak copy 0x8049103C (takes the function by value, so it is never inlined; defined in
    // game/MapManager.cpp); name invented
    inline void SetMapHit_unk(MapHitFunc_unk func, void* arg);
    inline void Push_unk(const DrawCmd_unk& cmd); // inferred: queues a draw command (defined in atn/ModelDraw_unk.cpp)
    inline int GetPass_unk() const; // inferred: weak copy 0x8008E2D4 (defined in atn/Model.cpp); name invented
    void SetCamera_unk(const Camera& camera) { mCamera = camera; } // inferred: name invented

    IRenderLayer* mLayers[0x14];   // 0x000: inferred: render layers (0: occlusion, 3: shadow, 4: reflection,
                                   // 6: 3D object, 14/17: post effects, 18: 2D object...)
    IRender* mPassHeads[0x14];     // 0x050: inferred: list of the draws queued in each pass
    int mPassCounts[0x14];         // 0x0A0
    unsigned char* mQueueBuffer;   // 0x0F0: inferred: entries (0x10 bytes) of the pass lists
    int mQueueCapacity;            // 0x0F4
    int mQueueUsed;                // 0x0F8
    float m0FC;
    float m100;
    Camera mCamera;                // 0x104: camera of the game
    int m2F8;                      // 0x2F8: inferred: render pass (3: shadow pass; 4: one matrix per item)
    NewCampScene2_unk* mScene;     // 0x2FC: inferred: scene drawn (lights, fog...)
    MapHitFunc_unk mMapHit;        // 0x300: inferred: scene of a sphere
    void* mMapHitArg;              // 0x314
    Camera mShadowCamera;          // 0x318: inferred: light camera of the shadow and light map
    GXTexObj m50C;                 // 0x50C: inferred: shadow map (fn_800BC618)
    int m52C;
    GXTexObj m530;                 // 0x530: inferred: light of the normals (fn_800BC468)
    int m550;
    int m554;
    Camera m558;                   // 0x558
    Texture_unk m74C;              // 0x74C: inferred: copy of the frame buffer
    GXTexObj m77C;
    void* m79C;                    // 0x79C: inferred: copy texture buffer
    Matrix3x4 m7A0[5];             // 0x7A0: inferred: texture matrices (fn_800C0900)
    int m890;                      // 0x890: inferred: fog set (fn_800C06B0)
    int m894;
    float m898;
    float m89C;
    float m8A0;
    float m8A4;
    float m8A8;
    float m8AC;
    RenderCallback_unk m8B0;       // 0x8B0: inferred: callbacks run around the frame
    RenderCallback_unk m8C8;
    RenderCallback_unk m8E0;
    RenderCallback_unk m8F8;
    int m910;                      // 0x910: inferred: queued frame buffer copies (fn_800C1660)
    struct {
        Texture_unk* mTex;
        int mFormat;
    } m914[8];
    int m954;                      // 0x954: inferred: queueing of the model draws enabled
    DrawQueue_unk<DrawSortKey_unk, 0x1000> mKeys; // 0x958
    DrawQueue_unk<DrawCmd_unk, 0x1000> mCmds;     // 0x895C
    int m54960;                    // 0x54960: inferred: set when a model needs the screen copy
    int m54964;                    // 0x54964: inferred: camera target used as the scene position (fn_800C1424)
};

} // namespace atn

// inferred: the render system (created by atn/RenderSystem_unk.cpp; its camera at 0x104 is the camera of
// the game, its scene at 0x2FC the scene drawn). One type for all the users.
extern "C" atn::RenderView_unk* lbl_8087EFB4;

// inferred: world to screen position with the camera of a render view (the callers declare the result
// first and pass its address, so it is not a member returning by value)
extern "C" void fn_800BFAC8(atn::Vector3* out, atn::RenderView_unk* view, const atn::Vector3* pos);

#endif
