#ifndef ATN_SCENE_H
#define ATN_SCENE_H

#include "math/Vec.h"
#include "std/vector.h"
#include "atn/Color.h"
#include "atn/LH_Scene.h"
#include "atn/Camera_unk.h"
#include "atn/Plane_unk.h"
#include "atn/Texture_unk.h"
#include "atn/CharaLightData_unk.h"
#include "atn/ColorMatrixParam_unk.h"

namespace atn {

struct Light;

// inferred: render setting overridden by a scene: flag + value (the scene uses mValue instead of the one of
// the render settings when mSet is nonzero; game/TownTask.cpp copies them with Set_unk). Name invented.
template <typename T>
struct Override_unk {
    Override_unk() { mSet = 0; }
    void Set_unk(const T& v) {
        mSet = 1;
        mValue = v;
    }
    int mSet;
    T mValue;
};

// Scene of the engine (0x2B4 bytes, atn/Scene.cpp): scene file, lights and cameras, the projected
// character/floor lights, the reflection plane and the render settings it overrides (god ray, rain,
// haze, shadow, color matrix). Real name from the RSO (__ct__Q23atn5SceneFv, __dt__Q23atn5SceneFv,
// GetLight__Q23atn5SceneFi, GetFog__Q23atn5SceneFv, GetAmbientLight__Q23atn5SceneFv); the
// boost::function of RenderManager::SetMapHit_unk returns one. MapArea has it as its base at 0x4C.
// Member names invented unless noted.
class Scene {
public:
    Scene();  // 0x800C1A1C
    ~Scene(); // 0x800C1FB4

    int fn_800C2108();                 // inferred: true while the scene or its textures are loading
    void fn_800C2274();                // inferred: ambient color from the lights of the file
    int fn_800C23E8();                 // inferred: number of lights
    int fn_800C2418();                 // inferred: number of cameras
    Light* GetLight(int index);        // 0x800C2448 (RSO name)
    const Light* GetLight(int index) const; // 0x800C24B4 (same code; inferred: const overload)
    Camera* fn_800C2520(int index);    // inferred: camera `index`
    CharaLightData_unk* fn_800C258C(const Vector3* pos); // inferred: floor light at `pos`
    void fn_800C2634(int count);       // inferred: character light with `count` textures
    void fn_800C289C(const char* path); // inferred: character light texture (null: off)
    void fn_800C2B24(const Vector2_unk* pos, const Vector2_unk* size, const char* path); // inferred: floor light
    void fn_800C2C20(const Plane_unk* plane); // inferred: sets the reflection plane
    void fn_800C2D88();                // inferred: debug drawing of the lights
    void fn_800C2E30(void* menu);      // inferred: debug menu

    // inferred: weak copies 0x80185188, 0x801856A4 and 0x801856AC (game/GameConfig_unk.cpp). AddLight_unk is
    // inline, defined in game/GameConfig_unk.cpp (its only user; atn::Light is only declared here)
    void AddLight_unk(Light light);
    Color& GetAmbientLight() { return mAmbient; } // RSO name
    Color& GetLS_unk() { return mLS; }
    // inferred: weak copies 0x8047C770-0x8047CB6C (Event/Manager.cpp)
    FogSettings* GetFog() { return &mFog; } // RSO name
    Color* fn_8047C778() { return &m02C; }
    GodRayParam_unk* fn_8047C780() { return &mGodRay.mValue; }
    ShadowParam_unk* fn_8047C7D4() { return &mShadow.mValue; }
    ColorMatrixParam_unk* fn_8047C7DC(); // inferred: own matrix or the one of the render settings (defined in Event/Manager.cpp)
    HazeParam_unk* fn_8047C848() { return &mHaze.mValue; }
    void fn_8047CB6C(int value) { mReflectChara = value; }
    // inferred: mask of mLightMask (gmk::RoomLight sets -1); name invented
    void SetLightMask_unk(int index) { mLightMask = index; }
    // inferred: sets the light of gmk::CharaLight/RoomLight (inlined in Event::Manager fn_80483714)
    void SetLight_unk(CharaLightData_unk* light) {
        m074 = light;
        mLightOn = m074 != 0 || mLightOn != 0;
    }
    // inferred: light of the scene: the one set by SetLight_unk or its own (mLight); null when off
    // (inlined in game/TownTask.cpp and atn/ModelDraw_unk.cpp); name invented
    CharaLightData_unk* GetLight_unk() const {
        return mLightOn != 0 ? (m074 != 0 ? m074 : (CharaLightData_unk*)&mLight) : 0;
    }

    LH_Scene mFile;                     // 0x000
    int mReady;                         // 0x008: inferred: fn_800C2274 done
    Color mAmbient;                     // 0x00C: debug menu "Ambient"
    Color mLS;                          // 0x01C: debug menu "LS"
    Color m02C;                         // 0x02C
    FogSettings mFog;                   // 0x03C
    std::vector<Light> mLights;         // 0x058: inferred: extra lights (after those of the file)
    std::vector<Camera> mCameras;       // 0x064: inferred: extra cameras (after those of the file)
    int mLightOn;                       // 0x070: inferred: character light on
    CharaLightData_unk* m074;           // 0x074: inferred: light of gmk::CharaLight/RoomLight
    CharaLightData_unk mLight;          // 0x078: inferred: own character light
    unsigned int mLightMask;            // 0x0C8: inferred: bits 0/1 tested by atn/ModelDraw_unk (IsShadowA/B); gmk::RoomLight sets -1
    int mFloorLightOn;                  // 0x0CC
    CharaLightData_unk mFloorLight;     // 0x0D0
    int mReflectChara;                  // 0x120: debug menu "Reflect Chara" (Event::ActionMapReflect)
    Matrix3x4 mReflect;                 // 0x124: inferred: reflection matrix of mPlane
    Plane_unk mPlane;                   // 0x154: debug menu "Reflect Plane" / "Reflect Y"
    Texture_unk mWhite;                 // 0x164: "common/sys_white.texture"
    Override_unk<GodRayParam_unk> mGodRay;            // 0x194
    Override_unk<RainParam_unk> mRain;                // 0x1E0
    Override_unk<HazeParam_unk> mHaze;                // 0x1F4
    Override_unk<ShadowParam_unk> mShadow;            // 0x234
    Override_unk<ColorMatrixParam_unk> mColorMatrix;  // 0x260
};

} // namespace atn

#endif
