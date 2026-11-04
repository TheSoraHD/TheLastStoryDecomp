#ifndef ATN_RENDERSETTING_UNK_H
#define ATN_RENDERSETTING_UNK_H

#include "math/Vec.h"
#include "atn/Csv.h"
#include "atn/ColorMatrixParam_unk.h"

// inferred: render settings edited from the debug menu (lbl_8087EFA8, created by fn_800B95C4).
// Class and member names are invented; the comments give the debug menu labels.

// inferred: rain parameters
struct RainParam_unk {
    RainParam_unk() : mEnabled(0), mDebug(0), mWarp(1.0f), mRate(0.05f) {}
    void fn_800BB8D8(void* menu);
    void fn_800BB994(RsCsv_unk* csv);

    int mEnabled; // 0x00
    int mDebug;   // 0x04
    float mWarp;  // 0x08
    float mRate;  // 0x0C
};

// inferred: gamma post effect parameters (see atn/PostFxGamma.cpp)
struct GammaParams_unk {
    GammaParams_unk(); // 0x800BC2E0
    void fn_800BC328(RsCsv_unk* csv);

    int mDebug;      // 0x00
    int mEnable;     // 0x04
    float mDepth;    // 0x08: Priority
    float mGamma[8]; // 0x0C: Gamma %d
    float mIndX;     // 0x2C: LUT Scale
    float mIndY;     // 0x30: Gamma Scale
};

// inferred: bloom parameters (names of the "Bloom" menu; Event::ActionCameraEffect copies a whole one,
// 0x8055BCA8)
struct BloomParam_unk {
    BloomParam_unk()
        : mEnabled(1), mDebug(0), mSize(3), mDarkSteps(3), mStrength(1), mStrengthScale(1.0f), mBlurWidth(1.25f),
          mBlurHeight(1.25f), mBlurAngle(0.0f), mColor1(White_unk()), mColor2(White_unk()), mColor3(White_unk()) {
        mKernel[0] = 2.0f;
        mKernel[1] = 1.0f;
        mKernel[2] = 0.5f;
        mKernel[3] = 1.0f;
        mKernel[4] = 0.5f;
        for (int i = 0; i < 3; i++) {
            mBias[i] = 0;
            mScale[i] = 0;
        }
    }
    // inferred: white (returned by value: closer to the original than temporaries; name invented)
    static RsColor_unk White_unk() { return RsColor_unk(1.0f, 1.0f, 1.0f, 1.0f); }

    int mEnabled;         // 0x00: *Bloom Enabled
    int mDebug;           // 0x04: Bloom Debug
    int mSize;            // 0x08: BloomSize
    int mDarkSteps;       // 0x0C: Dark Steps
    int mStrength;        // 0x10: Strength
    float mStrengthScale; // 0x14: Strength Scale
    float mBlurWidth;     // 0x18: *Blur Width
    float mBlurHeight;    // 0x1C: *Blur Height
    float mBlurAngle;     // 0x20: Blur Angle
    float mKernel[5];     // 0x24: Kernel0-4
    RsColor_unk mColor1;  // 0x38: *Color 1
    RsColor_unk mColor2;  // 0x48: *Color 2
    RsColor_unk mColor3;  // 0x58: *Color 3
    int mBias[3];         // 0x68: *Bias 1-3
    int mScale[3];        // 0x74: *Scale 1-3
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

// inferred: damage post effect parameters
struct DamageParam_unk {
    DamageParam_unk() : mEnabled(0), mDebug(0), mColor(1.0f, 1.0f, 1.0f, 1.0f), mAngle(0.0f), mScale(0.0f), mZoom(0.0f) {}
    int mEnabled;        // 0x00
    int mDebug;          // 0x04
    RsColor_unk mColor;  // 0x08: Color
    float mAngle;        // 0x18: Angle
    float mScale;        // 0x1C: Scale
    float mZoom;         // 0x20: Zoom
};

class RenderSetting_unk {
public:
    RenderSetting_unk();

    void* m000;               // 0x000: inferred: "Render" menu
    int m004;                 // DrawLight
    int m008;                 // UnuseModelFrustum
    int m00C;                 // DrawModelBone
    int m010;                 // DrawModelBounding
    int m014;                 // ModelDrawOff
    int m018;                 // DrawSafeArea
    int m01C;                 // Draw4:3Area
    int m020;                 // Wireframe
    int m024;
    int m028;                 // Disable LightMap
    int m02C;                 // Disable Fog
    int m030;                 // Gamma (mode)
    int m034;                 // Gamma (value)
    int m038;                 // ScreenShot
    RsColor_unk m03C;         // ClearColor
    void* m04C;               // inferred: root of the debug menu
    int m050;                 // Simple Shaders
    BloomParam_unk m054;      // Bloom
    int m0D4;                 // *Dof Enabled
    int m0D8;                 // Dof Debug
    int m0DC;                 // Dof View
    int m0E0;                 // Clear Far
    float m0E4;               // *Dof Start
    float m0E8;               // *Dof End
    float m0EC;               // *Dof Near Start
    float m0F0;               // *Dof Near End
    int m0F4;                 // *Width
    int m0F8;                 // *Height
    float m0FC;               // Blur Width
    float m100;               // Blur Height
    ShadowParam_unk m104;     // Shadow
    int m12C;                 // Shadow Debug
    int m130;                 // Use Depth Map
    int m134;                 // No Depth
    int m138;                 // Caster AABB
    int m13C;                 // Self Shadow
    int m140;                 // Generate Frustum
    int m144;                 // Enable Coverage
    int m148;                 // Force Coverage
    int m14C;                 // Debug Coverage
    int m150;                 // Cull shadow shaders
    int m154;                 // Bias
    int m158;                 // Map Width
    int m15C;                 // Map Height
    float m160;               // Max Z
    float m164;               // Min Z
    float m168;               // Max Y
    float m16C;               // Min Y
    float m170;               // Max X
    float m174;               // Min X
    int m178;                 // Lod Bias
    int m17C;                 // Shadow Clamp
    float m180;               // Clamp
    int m184;                 // Force depth
    int m188;                 // Force project
    int m18C;                 // Simple project
    float m190;               // AABB bias
    int m194;
    float m198;
    float m19C;
    float m1A0;
    float m1A4;
    float m1A8;
    float m1AC;
    float m1B0;               // Shader Dist
    int m1B4;                 // Shader Bias
    float m1B8;               // Node Area
    int m1BC;                 // Node Debug
    float m1C0;               // Camera Near
    float m1C4;               // Camera Far
    int m1C8;                 // Show GPU Info
    int m1CC;                 // GPU Info Depth
    int m1D0;                 // Occlusion: Enabled
    int m1D4;                 // Enabled AABB
    float m1D8;               // ZBias
    int m1DC;                 // Debug Enabled
    int m1E0;                 // Debug Model
    int m1E4;                 // Debug Enabled 3D
    int m1E8;
    float m1EC;               // Cull Dist
    float m1F0;               // Cull Area
    float m1F4;               // Model Area
    atn::Vector3 m1F8;        // Scale
    int m204;                 // Delta Shader
    int m208;                 // Force Lighting Off
    float m20C[5];            // LOD: Distance 1-3, No Motion, Half Motion
    int m220;                 // Half Rate
    int m224;                 // Aniso
    int m228;                 // Edge Filter
    float m22C;               // Mip Bias
    int m230;                 // Force No Mips
    int m234;                 // Show Mips
    float m238;
    float m23C;
    MotionBlurParam_unk m240; // MotionBlur
    GodRayParam_unk m264;     // GodRay
    RainParam_unk m2AC;       // Rain
    HazeParam_unk m2BC;       // Haze
    int m2F8;                 // Clouds Enabled
    float m2FC;               // Cloud Size
    float m300;               // Cloud Speed X
    float m304;               // Cloud Speed Y
    int m308;                 // View Reflection
    int m30C;
    int m310;
    int m314;
    int m318;
    int m31C;                 // View Mask
    int m320;                 // View Mask Full
    ColorMatrixParam_unk m324; // ColorMatrix
    DamageParam_unk m374;     // Damage
    int m398;                 // Deferred
    int m39C;                 // DeferredSort
    int m3A0;                 // Material Delta
    float m3A4;               // FrameSpeed
    int m3A8;                 // CacheMode
    int m3AC;                 // VFilter: Filter
    int m3B0[8];              // -3 .. 3, SUM
    int m3D0;
    int m3D4;
    float m3D8;
    GammaParams_unk m3DC;     // Gamma
    int m410;                 // Shadow LOD
    int m414;                 // Fps15
    float m418;               // 2DBlur: Blur Width
    float m41C;               // Blur Height
    float m420;               // Blur Angle
};

#endif
