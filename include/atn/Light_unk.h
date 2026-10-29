#ifndef ATN_LIGHT_UNK_H
#define ATN_LIGHT_UNK_H

#include "math/Vec.h"
#include "atn/Color4f_unk.h"

struct GXLightObj;

// inferred: animation track of the scene file (12 bytes, also in atn/LH_Scene.cpp); evaluates the
// value at `frame`
struct SceneTrack_unk {
    float fn_8047202C(const short& frame, float defaultValue);
    unsigned char m00[0xC];
};

// inferred: light of the scene file. Names invented.
struct LightData_unk {
    unsigned char m00[0x14];
    int mType; // 0x14
    unsigned char m18[0x28 - 0x18];
    SceneTrack_unk mPos[3];    // 0x28
    SceneTrack_unk mTarget[3]; // 0x4C
    SceneTrack_unk mColor[4];  // 0x70
    SceneTrack_unk mA0[4];
    SceneTrack_unk mAngle;     // 0xD0
    SceneTrack_unk mRefDist;   // 0xDC
    SceneTrack_unk mRefBright; // 0xE8
};

// inferred: RGBA color of a light (0.0-1.0): atn::Color4f_unk (game/GameConfig_unk.cpp passes one to
// fn_800790D0)
typedef atn::Color4f_unk LightColor_unk;

// inferred: engine light (0x44 bytes, atn/Light_unk.cpp). Names invented.
struct Light_unk {
    Light_unk();
    Light_unk(LightData_unk* data); // inferred: light of a scene file (0x80079090)
    void fn_800790D0(const atn::Vector3& dir, const LightColor_unk& color); // inferred: directional
    void fn_800791B4(const atn::Vector3& pos, const LightColor_unk& color, float radius); // inferred: point
    void fn_80079210(const atn::Vector3& pos, const atn::Vector3& dir, const LightColor_unk& color, float radius,
                     float angle); // inferred: spot
    void fn_80079280(float frame);  // inferred: reads the scene tracks
    void fn_800795DC();             // inferred: debug drawing
    void fn_800798D8(float* a, float* b, float* c); // inferred: distance attenuation coefficients
    void fn_80079994(GXLightObj* obj, Mtx viewMtx); // inferred: GX light in view space
    void fn_80079BC8(void* menu);   // inferred: debug menu

    LightData_unk* mData; // 0x00: inferred: scene file data
    int mType;            // 0x04: inferred: 0 off, 1 directional, 2 spot, 3 point, 4 ambient
    atn::Vector3 mPos;    // 0x08
    atn::Vector3 mDir;    // 0x14
    int mAttnType;        // 0x20: inferred: 0 none, 1-3 distance attenuation
    float mRadius;        // 0x24
    float mRefDist;       // 0x28
    float mRefBright;     // 0x2C
    float mAngle;         // 0x30
    LightColor_unk mColor; // 0x34
};

// inferred: list of the scene lights (singleton lbl_8087EEF8, created by fn_8007A100)
struct LightList_unk {
    LightList_unk() : mData(0), mCount(0), mCapacity(0) {}
    void fn_8007A154(int index); // inferred: removes a light
    void fn_8007A4CC();          // inferred: debug drawing of every light

    Light_unk* mData;
    unsigned int mCount;
    unsigned int mCapacity;
};

// inferred: up to four lights sorted by distance (0x24 bytes)
struct LightSet_unk {
    LightSet_unk();
    void Clear_unk() {
        mCount = 0;
        for (int i = 0; i < 4; i++) {
            mLight[i] = 0;
            mDist[i] = 1e16f;
        }
    }
    void fn_80079EFC(Light_unk* light, float dist); // inferred: inserts a light
    void fn_8007A36C(LightList_unk* list, const atn::Vector3& pos, float radius, int arg);
    void fn_8007A41C(LightList_unk* list);

    int mCount;
    Light_unk* mLight[4];
    float mDist[4];
};

extern "C" LightList_unk* lbl_8087EEF8;

#endif
