#ifndef ATN_RENDERMANAGER_H
#define ATN_RENDERMANAGER_H

#include "atn/Camera_unk.h"
#include "atn/Texture_unk.h"
#include "atn/Sphere.h"
#include "atn/Color.h"
#include "atn/DrawCmd_unk.h"
#include "std/string.h"
#include <boost/function.hpp>

namespace atn {
class Scene;
class IRender;
class IRenderLayer;
struct AABB_unk;
struct FogSettings;
} // namespace atn

// inferred: camera callback: the scene of the model hit by a sphere (set by MapManager)
typedef boost::function<atn::Scene*(atn::Sphere, void*)> MapHitFunc_unk;

namespace atn {

// inferred: entry of the draw list of a pass (0x10 bytes, taken from the queue buffer by AddRender)
struct RenderEntry_unk {
    IRender* mRender;
    float mOrder;
    RenderEntry_unk* mNext;
    int m0C;
};

// Render manager (0x54968 bytes). The names atn::RenderManager, its static instance_ (lbl_8087EFB4),
// AddRender, AddResolveTexture, GetScreenPosition, Order, GetScene, GetKeyLight and the nested types
// FrameCallbackEntry and ResolveCommand (static_vector<ResolveCommand, 8>) come from the import and
// dwarf tables of LastWorld_tools.rso (USA); the other names are invented. Built by fn_800BC798 with
// the constructor 0x800BC874 (atn/RenderSystem_unk.cpp); it is passed to the IRender draw virtuals.
class RenderManager {
public:
    // inferred: callback run around the frame (0x18 bytes; constructor 0x800BD94C)
    struct FrameCallbackEntry {
        FrameCallbackEntry(boost::function<void(void*)> func, int arg);

        boost::function<void(void*)> mFunc;
        int mArg;
    };
    // inferred: copy of the frame buffer queued by AddResolveTexture (8 bytes)
    struct ResolveCommand {
        Texture_unk* mTex;
        int mFormat;
    };

    RenderManager(); // 0x800BC874

    void fn_800BC468(GXTexObj* tex);              // inferred: builds the light texture of the normals
    void fn_800BC618(GXTexObj* tex, int offset);  // inferred: builds the shadow map texture
    void fn_800BD9C0(int size, const std::string& name); // inferred: allocates the queues and the textures
    void fn_800BDB40(Scene* scene);   // inferred: sets the scene (null: the default one)
    void AddRender(IRender* render, int pass, float order); // 0x800BDB58: queues a draw in a pass
    inline void* AllocQueue_unk(int size); // inferred: defined in atn/RenderSystem_unk.cpp
    void fn_800BDBC8();                           // inferred: draws the frame
    void fn_800BFAC4();                           // inferred: empty
    Vector3 GetScreenPosition(const Vector3& pos) const; // 0x800BFAC8
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
    Scene* fn_800C122C(Sphere sphere); // inferred: scene of a sphere (mMapHit, else mScene)
    Scene* fn_800C1424();             // inferred: scene at the camera
    void AddResolveTexture(Texture_unk* tex, int format); // 0x800C1660: queues a copy of the frame buffer
    float fn_800C169C(const AABB_unk* box);       // inferred: visible fraction (layer 0)
    Scene* fn_800C16B4();             // inferred: default scene (lbl_8087EFB8)
    void fn_800C16BC();                           // inferred: empty

    // inferred: weak copies 0x80479EA0 and 0x8047A158 (defined in game/main.cpp)
    inline void fn_80479EA0(boost::function<void(void*)> func, int arg);
    inline void fn_8047A158(boost::function<void(void*)> func, int arg);
    // inferred: weak copy 0x8049103C (takes the function by value, so it is never inlined; defined in
    // game/MapManager.cpp); name invented
    inline void SetMapHit_unk(MapHitFunc_unk func, void* arg);
    inline void Push_unk(const DrawCmd_unk& cmd); // inferred: queues a draw command (defined in atn/ModelDraw_unk.cpp)
    inline int GetPass_unk() const; // inferred: weak copy 0x8008E2D4 (defined in atn/Model.cpp); name invented
    // inferred: weak copy 0x80116BD4 (game/Event/Path area); the callers that do not inline it call it on
    // fn_8008B964(); name invented
    void SetCamera_unk(const Camera& camera) { mCamera = camera; }
    Camera* fn_800F7258(); // inferred: camera (weak copy 0x800F7258, game/BattleTask)
    Scene* GetScene() { return mScene; } // RSO name; weak copy 0x804923AC (game/MapPartsBase.cpp)

    IRenderLayer* mLayers[0x14];   // 0x000: inferred: render layers (0: occlusion, 3: shadow, 4: reflection,
                                   // 6: 3D object, 14/17: post effects, 18: 2D object...)
    RenderEntry_unk* mPassHeads[0x14]; // 0x050: inferred: list of the draws queued in each pass
    int mPassCounts[0x14];         // 0x0A0
    unsigned char* mQueueBuffer;   // 0x0F0: inferred: entries (0x10 bytes) of the pass lists
    int mQueueCapacity;            // 0x0F4
    int mQueueUsed;                // 0x0F8
    float m0FC;
    float m100;
    Camera mCamera;                // 0x104: camera of the game
    int m2F8;                      // 0x2F8: inferred: render pass (3: shadow pass; 4: one matrix per item)
    Scene* mScene;     // 0x2FC: inferred: scene drawn (lights, fog...)
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
    FrameCallbackEntry m8B0;       // 0x8B0: inferred: callbacks run around the frame
    FrameCallbackEntry m8C8;
    FrameCallbackEntry m8E0;
    FrameCallbackEntry m8F8;
    int mResolveCount;             // 0x910: static_vector<ResolveCommand, 8> (AddResolveTexture)
    ResolveCommand mResolve[8];    // 0x914
    int m954;                      // 0x954: inferred: queueing of the model draws enabled
    DrawQueue_unk<DrawSortKey_unk, 0x1000> mKeys; // 0x958
    DrawQueue_unk<DrawCmd_unk, 0x1000> mCmds;     // 0x895C
    int m54960;                    // 0x54960: inferred: set when a model needs the screen copy
    int m54964;                    // 0x54964: inferred: camera target used as the scene position (fn_800C1424)

    static RenderManager* instance_; // lbl_8087EFB4 (set by fn_800BC798)
};

} // namespace atn

// inferred: the render manager (atn::RenderManager::instance_; defined in atn/Model.cpp)
extern "C" atn::RenderManager* fn_8008B964();


#endif
