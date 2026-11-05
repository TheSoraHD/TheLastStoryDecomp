#ifndef GAME_NEWCAMPSCENE2_UNK_H
#define GAME_NEWCAMPSCENE2_UNK_H

#include "atn/Light_unk.h"
#include "std/vector.h"
#include "atn/ColorMatrixParam_unk.h"

// inferred: light setup of a scene (0x2B4 bytes; constructor 0x800C1D98 and destructor 0x800C1FB4 in
// atn/RenderSystem_unk.cpp, which declares its own view). Name invented.
struct NewCampScene2_unk {
    NewCampScene2_unk();
    ~NewCampScene2_unk();
    // inferred: weak copies 0x80185188, 0x801856A4 and 0x801856AC (game/GameConfig_unk.cpp)
    // NONMATCHING: AddLight_unk 95.3% - the growth path of std::vector::push_back (as in Event/Path.cpp)
    void AddLight_unk(Light_unk light) { mLights.push_back(light); }
    atn::D2A_DrawColor& GetAmbient_unk() { return mAmbient; }
    atn::D2A_DrawColor& GetLS_unk() { return mLS; }
    // inferred: weak copies 0x8047C770-0x8047CB6C (Event/Manager.cpp)
    atn::FogSettings* fn_8047C770() { return &mFog; }
    atn::Color* fn_8047C778() { return &m02C; }
    GodRayParam_unk* fn_8047C780() { return &mGodRay; }
    ShadowParam_unk* fn_8047C7D4() { return &mShadow; }
    ColorMatrixParam_unk* fn_8047C7DC(); // inferred: own matrix or the one of the render settings (defined in Event/Manager.cpp)
    HazeParam_unk* fn_8047C848() { return &mHaze; }
    void fn_8047CB6C(int value) { m120 = value; }

    unsigned char m000[0x00C];
    atn::D2A_DrawColor mAmbient;    // 0x00C: debug menu "Ambient"
    atn::D2A_DrawColor mLS;         // 0x01C: debug menu "LS"
    atn::Color m02C;                // 0x02C
    atn::FogSettings mFog;          // 0x03C
    std::vector<Light_unk> mLights; // 0x058
    unsigned char m064[0x070 - 0x064];
    int m070;                       // 0x070: inferred: a light of gmk::CharaLight/RoomLight is set (MapArea)
    void* m074;                     // 0x074: inferred: light of gmk::CharaLight/RoomLight (MapArea)
    unsigned char m078[0x0C8 - 0x078];
    int m0C8;                       // 0x0C8: inferred: set by gmk::RoomLight (-1: none)
    unsigned char m0CC[0x120 - 0x0CC];
    int m120;                       // 0x120: inferred: reflection mode (Event::ActionMapReflect)
    unsigned char m124[0x198 - 0x124];
    GodRayParam_unk mGodRay;        // 0x198
    unsigned char m1E0[0x1F8 - 0x1E0];
    HazeParam_unk mHaze;            // 0x1F8
    unsigned char m234[0x238 - 0x234];
    ShadowParam_unk mShadow;        // 0x238
    int mUseColorMatrix;            // 0x260: inferred: the scene has its own color matrix
    ColorMatrixParam_unk mColorMatrix; // 0x264
};

// inferred: light `index` of the scene (atn/RenderSystem_unk.cpp)
extern "C" Light_unk* fn_800C2448(NewCampScene2_unk* scene, int index);

#endif
