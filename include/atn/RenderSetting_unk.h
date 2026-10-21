#ifndef ATN_RENDERSETTING_UNK_H
#define ATN_RENDERSETTING_UNK_H

#include "math/Vec.h"
#include "atn/Csv.h"

// inferred: render settings edited from the debug menu (lbl_8087EFA8, created by fn_800B95C4).
// Class and member names are invented; the comments give the debug menu labels.

typedef atn::CsvBase<char, '\t', '\r', '"'> RsCsv_unk;

// inferred: float color (plain struct, copied member by member); name invented
struct RsColor_unk {
    RsColor_unk() {}
    RsColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: 4 float vector copied with two paired singles (rows of the color matrix); name invented
struct RsVector4_unk {
    RsVector4_unk() {}
    RsVector4_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    RsVector4_unk(const RsVector4_unk& v) {
        __Vec4Copy(&x, &v.x);
    }
    RsVector4_unk& operator=(const RsVector4_unk& v) {
        __Vec4Copy(&x, &v.x);
        return *this;
    }
    float x, y, z, w;
};

// inferred: shadow parameters (also loaded from a map csv by fn_800BBD90)
struct ShadowParam_unk {
    ShadowParam_unk()
        : mEnabled(1), mMode(0), mCullDist(250.0f), mColor(0.5f, 0.5f, 0.5f, 1.0f),
          mDir(atn::Vector3(1.0f, -1.0f, 1.0f).GetNormalized_unk()) {}
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
    int m054;                 // *Bloom Enabled
    int m058;                 // Bloom Debug
    int m05C;                 // BloomSize
    int m060;                 // Dark Steps
    int m064;                 // Strength
    float m068;               // Strength Scale
    float m06C;               // *Blur Width
    float m070;               // *Blur Height
    float m074;               // Blur Angle
    float m078;               // Kernel0
    float m07C;               // Kernel1
    float m080;               // Kernel2
    float m084;               // Kernel3
    float m088;               // Kernel4
    RsColor_unk m08C;         // *Color 1
    RsColor_unk m09C;         // *Color 2
    RsColor_unk m0AC;         // *Color 3
    int m0BC;                 // *Bias 1
    int m0C0;                 // *Bias 2
    int m0C4;                 // *Bias 3
    int m0C8;                 // *Scale 1
    int m0CC;                 // *Scale 2
    int m0D0;                 // *Scale 3
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
    int m240;                 // MotionBlur: Enabled
    int m244;                 // Debug
    int m248;                 // Frozen
    float m24C;               // Scale
    float m250;               // Shift
    int m254;                 // StartBlend
    int m258;                 // EndBlend
    float m25C;               // StartBlendSpeed
    float m260;               // EndBlendSpeed
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
