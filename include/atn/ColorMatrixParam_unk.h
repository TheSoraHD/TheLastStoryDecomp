#ifndef ATN_COLORMATRIXPARAM_UNK_H
#define ATN_COLORMATRIXPARAM_UNK_H

// inferred: types of the render settings shared with the event manager (from atn/RenderSetting_unk.h)

#include "math/Vec.h"
#include "atn/Csv.h"
#include "atn/Color.h"
#include "atn/Vector4_unk.h"

typedef atn::CsvBase<char, '\t', '\r', '"'> RsCsv_unk;

// inferred: float color of the render settings: atn::Color (Event::Manager lerps both with one function)
typedef atn::Color RsColor_unk;

// inferred: rows of the color matrix: atn::Vector4_unk (same copy as ButtonUv_unk)
typedef atn::Vector4_unk RsVector4_unk;

// inferred: color matrix post effect parameters
struct ColorMatrixParam_unk {
    ColorMatrixParam_unk() : mEnabled(0), mPriority(0), mMono(0), mMonoRate(1.0f) {
        mRows[0] = RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f);
        mRows[1] = RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f);
        mRows[2] = RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f);
        mRows[3] = RsVector4_unk(0.0f, 0.0f, 0.0f, 0.0f);
    }
    void fn_800BBFE8(RsCsv_unk* csv);
    void fn_800BC194(void* menu);

    int mEnabled;            // 0x00
    RsVector4_unk mRows[4];  // 0x04: Row 0-3
    int mPriority;           // 0x44
    int mMono;               // 0x48: MonoEnable
    float mMonoRate;         // 0x4C
};

namespace atn {

// fog of a scene (real name from the RSO: __as__Q23atn11FogSettingsFRCQ23atn11FogSettings, weak copy
// 0x8047C850); member names invented
struct FogSettings {
    int mMode;    // 0x00
    float mStart; // 0x04
    float mEnd;   // 0x08
    Color mColor; // 0x0C
};

} // namespace atn

// inferred: shadow parameters (also loaded from a map csv by fn_800BBD90)
struct ShadowParam_unk {
    ShadowParam_unk()
        : mEnabled(1), mMode(0), mCullDist(250.0f), mColor(0.5f, 0.5f, 0.5f, 1.0f),
          mDir(atn::Vector3(1.0f, -1.0f, 1.0f).GetNormalize()) {}
    void fn_800BBD90(RsCsv_unk* csv); // inferred: loads "ON", distance, color, direction, "ADD"
    void fn_800BBEFC(void* menu);       // inferred: adds the debug menu entries

    int mEnabled;        // 0x00: Shadow Enabled
    int mMode;           // 0x04: Mode
    float mCullDist;     // 0x08: Cull Distance
    RsColor_unk mColor;  // 0x0C: Color
    atn::Vector3 mDir;   // 0x1C: Direction
};

// inferred: god ray parameters (also in the map parts, see game/MapPartsBase)
struct GodRayParam_unk {
    GodRayParam_unk()
        : mEnabled(0), mDebug(0), m08(2), mIterations(3), mDirectional(1), mUseColor(0), mMode(0),
          mBlurWidth(-3.0f), mBlurHeight(-3.0f), mRadialScale(1.0f), mColor(0.75f, 0.75f, 0.75f, 1.0f),
          mPos(0.0f, 1.0f, 0.0f), mRadius(100.0f) {}
    void fn_800BB4FC(void* menu);
    void fn_800BB6C0(RsCsv_unk* csv);

    int mEnabled;        // 0x00: *Enabled
    int mDebug;          // 0x04: Debug
    int m08;
    int mIterations;     // 0x0C: Iterations
    int mDirectional;    // 0x10: *Directional
    int mUseColor;       // 0x14: Use Color
    int mMode;           // 0x18: *Mode (POINT, DIRECTION, CAMERA)
    float mBlurWidth;    // 0x1C: *BlurWidth
    float mBlurHeight;   // 0x20: *BlurHeight
    float mRadialScale;  // 0x24: RadialScale
    RsColor_unk mColor;  // 0x28: *Color
    atn::Vector3 mPos;   // 0x38: *Position
    float mRadius;       // 0x44: Radius
};

// inferred: haze parameters
struct HazeParam_unk {
    HazeParam_unk()
        : mEnabled(0), mDebug(0), mWarp(0.2f), mScroll(0.0f, 0.01f), mScale(10.0f, 5.0f), mPos(0.0f, 224.0f),
          mSize(640.0f, 224.0f), mFade(0.0f, 0.0f, 1.0f, 1.0f) {}
    void fn_800BBA64(void* menu);
    void fn_800BBBB0(RsCsv_unk* csv);

    int mEnabled;                // 0x00
    int mDebug;                  // 0x04
    float mWarp;                 // 0x08
    atn::Vector2_unk mScroll;    // 0x0C
    atn::Vector2_unk mScale;     // 0x14
    atn::Vector2_unk mPos;       // 0x1C
    atn::Vector2_unk mSize;      // 0x24
    RsVector4_unk mFade;         // 0x2C
};

// inferred: motion blur parameters (names of the "MotionBlur" menu; Event::ActionCameraEffect copies
// a whole one, 0x8055D0B8)
struct MotionBlurParam_unk {
    MotionBlurParam_unk()
        : mEnabled(0), mDebug(0), mFrozen(0), mScale(1.0f), mShift(0.0f), mStartBlend(0), mEndBlend(0),
          mStartBlendSpeed(0.02f), mEndBlendSpeed(0.02f) {}
    int mEnabled;           // 0x00
    int mDebug;             // 0x04
    int mFrozen;            // 0x08
    float mScale;           // 0x0C
    float mShift;           // 0x10
    int mStartBlend;        // 0x14
    int mEndBlend;          // 0x18
    float mStartBlendSpeed; // 0x1C
    float mEndBlendSpeed;   // 0x20
};

#endif
