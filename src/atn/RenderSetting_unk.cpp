// atn/RenderSetting_unk.cpp: render settings edited from the debug menu (lbl_8087EFA8) and the
// parameter blocks of the post effects (god ray, rain, haze, shadow, color matrix, gamma) that the
// map parts also load from their csv (file name invented).
#include "atn/RenderSetting_unk.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);    // inferred: string to integer (atoi)

// inferred: debug menu (class unknown): groups, toggles and sliders
extern "C" void* fn_8008937C(void* parent, const char* name); // inferred: adds a group
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4); // inferred: toggle
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087858(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: angle
extern "C" void fn_80087BB4(void* group, const char* name, atn::Vector2_unk* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_8008826C(void* group, const char* name, RsVector4_unk* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088AF4(void* group, const char* name, RsColor_unk* value, int arg3, int arg4, float min,
                            float max, float step); // inferred: color
extern "C" void fn_80088FAC(void* group, const char* name, RsColor_unk* value, int arg3, int arg4, float min,
                            float max, float step); // inferred: color (rgb)

// inferred: owner of the debug menu (lbl_8087EF10; menu at 0x10)
struct RsDebug_unk {
    unsigned char m00[0x10];
    unsigned char m10[1];
};
extern "C" RsDebug_unk* lbl_8087EF10;

// inferred: render manager (lbl_8087EEE0): screen size
struct RsRenderMgr_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
extern "C" RsRenderMgr_unk* lbl_8087EEE0;

extern "C" RenderSetting_unk* lbl_8087EFA8;

// inferred: creates the render settings
extern "C" void fn_800B95C4() {
    if (lbl_8087EFA8 == 0) {
        lbl_8087EFA8 = new ((MemoryTag)6, "", "", 0) RenderSetting_unk;
    }
}

// NONMATCHING: 96.92% - the three white bloom colors get their stack temporaries in the opposite
// order and the scheduler places some constant loads and stores differently (a helper returning the
// color by value is slightly closer than the temporaries in the initializer list)
// inferred: white color (helper, name invented)
static inline RsColor_unk sWhite_unk() {
    return RsColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
}

RenderSetting_unk::RenderSetting_unk()
    : m000(0), m004(0), m008(0), m00C(0), m010(0), m014(0), m018(0), m01C(0), m020(0), m024(0), m028(0), m02C(0),
      m030(0), m034(10), m038(0), m03C(0.0f, 0.0f, 0.0f, 1.0f), m050(0), m054(1), m058(0), m05C(3), m060(3),
      m064(1), m068(1.0f), m06C(1.25f), m070(1.25f), m074(0.0f), m078(2.0f), m07C(1.0f), m080(0.5f), m084(1.0f),
      m088(0.5f), m08C(sWhite_unk()), m09C(sWhite_unk()),
      m0AC(sWhite_unk()), m0BC(0), m0C0(0), m0C4(0), m0C8(0), m0CC(0), m0D0(0), m0D4(1),
      m0D8(0), m0DC(0), m0E0(1), m0E4(200.0f), m0E8(500.0f), m0EC(30.0f), m0F0(10.0f), m0F4(320), m0F8(224),
      m0FC(1.5f), m100(1.5f), m12C(0), m138(1), m13C(0), m140(1), m144(1), m148(0), m14C(0), m150(1), m154(0),
      m158(lbl_8087EEE0->mWidth), m15C(lbl_8087EEE0->mHeight), m160(200.0f), m164(-200.0f), m168(200.0f),
      m16C(-200.0f), m170(200.0f), m174(-200.0f), m178(2), m17C(1), m180(10.0f), m184(0), m188(0), m18C(1),
      m190(10.0f), m194(0), m198(0.0f), m19C(500.0f), m1A0(0.6f), m1A4(0.6f), m1A8(0.5f), m1AC(1.0f), m1B0(0.0f),
      m1B4(0), m1B8(200.0f), m1BC(0), m1C0(1.0f), m1C4(5000.0f), m1C8(0), m1CC(2), m1D0(1), m1D4(1),
      m1D8(0.0001f), m1DC(0), m1E0(0), m1E4(0), m1E8(6), m1EC(1000.0f), m1F0(25.0f), m1F4(50.0f),
      m1F8(1.0f, 1.0f, 1.0f), m204(1), m208(0), m224(1), m228(1), m22C(0.0f), m230(0), m234(0), m238(0.0f),
      m23C(0.0f), m240(0), m244(0), m248(0), m24C(1.0f), m250(0.0f), m254(0), m258(0), m25C(0.02f), m260(0.02f),
      m2F8(1), m2FC(256.0f), m300(0.5f), m304(0.5f), m308(0), m30C(0), m310(0), m314(320), m318(224), m31C(0),
      m320(0), m398(1), m39C(1), m3A0(1), m3A4(1.0f), m3A8(2), m3D0(0), m3D4(0), m3D8(0.0f), m410(1), m414(0),
      m418(0.5f), m41C(0.5f), m420(0.0f) {
    m20C[0] = 40.0f;
    m20C[1] = 120.0f;
    m20C[2] = 250.0f;
    m20C[3] = 400.0f;
    m20C[4] = 150.0f;
    m220 = 2;
    m0F4 = lbl_8087EEE0->mWidth / 2;
    m0F8 = lbl_8087EEE0->mHeight / 2;
    void* root;
    if (m000 == 0 && lbl_8087EF10 != 0) {
        root = m000 = fn_8008937C(&lbl_8087EF10->m10, "Render");
    } else {
        root = 0;
    }
    m04C = root;
    void* g1 = fn_8008937C(m04C, "Debug");
    void* g2 = fn_8008937C(g1, "VFilter");
    fn_80087994(g1, "ScreenShot", &m038, 0, 0);
    fn_80087994(g1, "Fps15", &m414, 0, 0);
    fn_800874C8(g1, "CacheMode", &m3A8, 0, 2, 1, 0, 0);
    fn_800874C8(g1, "Gamma", &m030, 0, 2, 1, 0, 0);
    fn_800874C8(g1, "Gamma", &m034, 1, 30, 1, 0, 0);
    fn_8008771C(g1, "Camera Near", &m1C0, 0, 0, 1.0f, 10000.0f, 1.0f);
    fn_8008771C(g1, "Camera Far", &m1C4, 0, 0, 1.0f, 10000.0f, 1.0f);
    fn_80087994(g1, "Deferred", &m398, 0, 0);
    fn_80087994(g1, "DeferredSort", &m39C, 0, 0);
    fn_80087994(g1, "Material Delta", &m3A0, 0, 0);
    m3AC = 1;
    m3B0[0] = 8;
    m3B0[1] = 8;
    m3B0[2] = 10;
    m3B0[3] = 12;
    m3B0[4] = 10;
    m3B0[5] = 8;
    m3B0[6] = 8;
    m3B0[7] = 64;
    fn_80087994(g2, "Filter", &m3AC, 0, 0);
    fn_800874C8(g2, "-3", &m3B0[0], 0, 63, 1, 0, 0);
    fn_800874C8(g2, "-2", &m3B0[1], 0, 63, 1, 0, 0);
    fn_800874C8(g2, "-1", &m3B0[2], 0, 63, 1, 0, 0);
    fn_800874C8(g2, " 0", &m3B0[3], 0, 63, 1, 0, 0);
    fn_800874C8(g2, " 1", &m3B0[4], 0, 63, 1, 0, 0);
    fn_800874C8(g2, " 2", &m3B0[5], 0, 63, 1, 0, 0);
    fn_800874C8(g2, " 3", &m3B0[6], 0, 63, 1, 0, 0);
    fn_800874C8(g2, "SUM", &m3B0[7], 0, 999, 1, 0, 0);
    fn_80087994(g1, "View Reflection", &m308, 0, 0);
    fn_80087994(g1, "View Mask", &m31C, 0, 0);
    fn_80087994(g1, "View Mask Full", &m320, 0, 0);
    fn_80087994(g1, "Disable LightMap", &m028, 0, 0);
    fn_80087994(g1, "Disable Fog", &m02C, 0, 0);
    fn_80087994(g1, "Simple Shaders", &m050, 0, 0);
    fn_80087994(g1, "Delta Shader", &m204, 0, 0);
    fn_80087994(g1, "DrawLight", &m004, 0, 0);
    fn_80087994(g1, "Force Lighting Off", &m208, 0, 0);
    fn_80087994(g1, "UnuseModelFrustum", &m008, 0, 0);
    fn_80087994(g1, "Show GPU Info", &m1C8, 0, 0);
    fn_800874C8(g1, "GPU Info Depth", &m1CC, 0, 10, 1, 0, 0);
    fn_80087994(g1, "DrawModelBone", &m00C, 0, 0);
    fn_80087994(g1, "DrawModelBounding", &m010, 0, 0);
    fn_80087994(g1, "ModelDrawOff", &m014, 0, 0);
    fn_80087994(g1, "DrawSafeArea", &m018, 0, 0);
    fn_80087994(g1, "Draw4:3Area", &m01C, 0, 0);
    fn_80087994(g1, "Wireframe", &m020, 0, 0);
    fn_8008771C(g1, "FrameSpeed", &m3A4, 0, 0, 0.1f, 100.0f, 0.1f);
    void* g3 = fn_8008937C(m04C, "PostFX");
    void* g4 = fn_8008937C(m04C, "Textures");
    fn_80087994(g4, "Force No Mips", &m230, 0, 0);
    fn_80087994(g4, "Show Mips", &m234, 0, 0);
    fn_800874C8(g4, "Aniso", &m224, 0, 3, 1, 0, 0);
    fn_80087994(g4, "Edge Filter", &m228, 0, 0);
    fn_8008771C(g4, "Mip Bias", &m22C, 0, 0, -10.0f, 10.0f, 0.01f);
    void* g5 = fn_8008937C(m04C, "LOD");
    fn_8008771C(g5, "Distance 1", &m20C[0], 0, 0, -1000.0f, 1000.0f, 0.5f);
    fn_8008771C(g5, "Distance 2", &m20C[1], 0, 0, -1000.0f, 1000.0f, 0.5f);
    fn_8008771C(g5, "Distance 3", &m20C[2], 0, 0, -1000.0f, 1000.0f, 0.5f);
    fn_8008771C(g5, "No Motion", &m20C[3], 0, 0, -1000.0f, 1000.0f, 0.5f);
    fn_8008771C(g5, "Half Motion", &m20C[4], 0, 0, -1000.0f, 1000.0f, 0.5f);
    fn_800874C8(g5, "Half Rate", &m220, 1, 1000, 1, 0, 0);
    fn_8008771C(g5, "Shader Dist", &m1B0, 0, 0, -1000.0f, 1000.0f, 0.5f);
    fn_800874C8(g5, "Shader Bias", &m1B4, -10, 10, 1, 0, 0);
    fn_8008771C(g5, "Node Area", &m1B8, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_80087994(g5, "Node Debug", &m1BC, 0, 0);
    void* g6 = fn_8008937C(m04C, "Occlusion");
    fn_80087994(g6, "Enabled", &m1D0, 0, 0);
    fn_80087994(g6, "Enabled AABB", &m1D4, 0, 0);
    fn_8008771C(g6, "ZBias", &m1D8, 0, 0, -1000.0f, 1000.0f, 0.01f);
    fn_80087994(g6, "Debug Enabled", &m1DC, 0, 0);
    fn_80087994(g6, "Debug Enabled 3D", &m1E4, 0, 0);
    fn_80087994(g6, "Debug Model", &m1E0, 0, 0);
    fn_80087E9C(g6, "Scale", &m1F8, 0, 0, 0.0f, 10.0f, 0.001f);
    fn_8008771C(g6, "Model Area", &m1F4, 0, 0, 0.0f, 10000.0f, 5.0f);
    fn_8008771C(g6, "Cull Dist", &m1EC, 0, 0, 0.0f, 10000.0f, 5.0f);
    fn_8008771C(g6, "Cull Area", &m1F0, 0, 0, 0.0f, 10000.0f, 5.0f);
    void* g7 = fn_8008937C(g3, "ColorMatrix");
    m324.fn_800BC194(g7);
    void* g8 = fn_8008937C(g3, "Gamma");
    fn_80087994(g8, "Enabled", &m3DC.mEnable, 0, 0);
    fn_80087994(g8, "Debug", &m3DC.mDebug, 0, 0);
    fn_8008771C(g8, "Priority", &m3DC.mDepth, 0, 0, -10000.0f, 10000.0f, 0.01f);
    fn_8008771C(g8, "LUT Scale", &m3DC.mIndX, 0, 0, -10000.0f, 10000.0f, 0.01f);
    fn_8008771C(g8, "Gamma Scale", &m3DC.mIndY, 0, 0, -10000.0f, 10000.0f, 0.01f);
    for (int i = 0; i < 8; i++) {
        char name[256];
        sprintf(name, "Gamma %d", i);
        fn_8008771C(g8, name, &m3DC.mGamma[i], 0, 0, -10.0f, 10.0f, 0.01f);
    }
    void* g9 = fn_8008937C(g3, "GodRay");
    m264.fn_800BB4FC(g9);
    void* g10 = fn_8008937C(g3, "Bloom");
    fn_80087994(g10, "*Bloom Enabled", &m054, 0, 0);
    fn_80087994(g10, "Bloom Debug", &m058, 0, 0);
    fn_80088AF4(g10, "*Color 1", &m08C, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088AF4(g10, "*Color 2", &m09C, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088AF4(g10, "*Color 3", &m0AC, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_800874C8(g10, "*Bias 1", &m0BC, 0, 2, 1, 0, 0);
    fn_800874C8(g10, "*Bias 2", &m0C0, 0, 2, 1, 0, 0);
    fn_800874C8(g10, "*Bias 3", &m0C4, 0, 2, 1, 0, 0);
    fn_800874C8(g10, "*Scale 1", &m0C8, 0, 3, 1, 0, 0);
    fn_800874C8(g10, "*Scale 2", &m0CC, 0, 3, 1, 0, 0);
    fn_800874C8(g10, "*Scale 3", &m0D0, 0, 3, 1, 0, 0);
    fn_8008771C(g10, "*Blur Width", &m06C, 0, 0, 0.0f, 10.0f, 0.1f);
    fn_8008771C(g10, "*Blur Height", &m070, 0, 0, 0.0f, 10.0f, 0.1f);
    fn_800874C8(g10, "BloomSize", &m05C, 1, 8, 1, 0, 0);
    fn_800874C8(g10, "Strength", &m064, 0, 128, 1, 0, 0);
    fn_8008771C(g10, "Strength Scale", &m068, 0, 0, 0.0f, 1.0f, 0.02f);
    fn_800874C8(g10, "Dark Steps", &m060, 0, 64, 1, 0, 0);
    fn_80087858(g10, "Blur Angle", &m074, 0, 0, -180.0f, 180.0f, 2.0f);
    fn_8008771C(g10, "Kernel0", &m078, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(g10, "Kernel1", &m07C, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(g10, "Kernel2", &m080, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(g10, "Kernel3", &m084, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(g10, "Kernel4", &m088, 0, 0, 0.0f, 100.0f, 0.1f);
    void* g11 = fn_8008937C(g3, "DoF");
    fn_80087994(g11, "*Dof Enabled", &m0D4, 0, 0);
    fn_80087994(g11, "Dof Debug", &m0D8, 0, 0);
    fn_80087994(g11, "Dof View", &m0DC, 0, 0);
    fn_80087994(g11, "Clear Far", &m0E0, 0, 0);
    fn_8008771C(g11, "*Dof Start", &m0E4, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_8008771C(g11, "*Dof End", &m0E8, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_8008771C(g11, "*Dof Near Start", &m0EC, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_8008771C(g11, "*Dof Near End", &m0F0, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_800874C8(g11, "*Width", &m0F4, 16, 320, 16, 0, 0);
    fn_800874C8(g11, "*Height", &m0F8, 16, lbl_8087EEE0->mHeight / 2, 16, 0, 0);
    fn_8008771C(g11, "Blur Width", &m0FC, 0, 0, 0.0f, 10.0f, 0.1f);
    fn_8008771C(g11, "Blur Height", &m100, 0, 0, 0.0f, 10.0f, 0.1f);
    void* g12 = fn_8008937C(g3, "MotionBlur");
    fn_80087994(g12, "Enabled", &m240, 0, 0);
    fn_80087994(g12, "Debug", &m244, 0, 0);
    fn_80087994(g12, "Frozen", &m248, 0, 0);
    fn_8008771C(g12, "Scale", &m24C, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(g12, "Shift", &m250, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_80087994(g12, "StartBlend", &m254, 0, 0);
    fn_80087994(g12, "EndBlend", &m258, 0, 0);
    fn_8008771C(g12, "StartBlendSpeed", &m25C, 0, 0, 0.0f, 100.0f, 0.01f);
    fn_8008771C(g12, "EndBlendSpeed", &m260, 0, 0, 0.0f, 100.0f, 0.01f);
    void* g13 = fn_8008937C(g3, "Damage");
    fn_80087994(g13, "Enabled", &m374.mEnabled, 0, 0);
    fn_80087994(g13, "Debug", &m374.mDebug, 0, 0);
    fn_80087858(g13, "Angle", &m374.mAngle, 0, 0, -360.0f, 360.0f, 0.05f);
    fn_8008771C(g13, "Scale", &m374.mScale, 0, 0, -180.0f, 180.0f, 0.05f);
    fn_8008771C(g13, "Zoom", &m374.mZoom, 0, 0, -0.9999999f, 100.0f, 0.05f);
    fn_80088AF4(g13, "Color", &m374.mColor, 0, 0, 0.0f, 4.0f, 0.01f);
    void* g14 = fn_8008937C(g3, "Rain");
    m2AC.fn_800BB8D8(g14);
    void* g15 = fn_8008937C(g3, "Haze");
    m2BC.fn_800BBA64(g15);
    void* g16 = fn_8008937C(m04C, "Shadow");
    fn_80087994(g16, "Shadow LOD", &m410, 0, 0);
    m104.fn_800BBEFC(g16);
    fn_80087994(g16, "Shadow Debug", &m12C, 0, 0);
    fn_80087994(g16, "No Depth", &m134, 0, 0);
    fn_80087994(g16, "Force depth", &m184, 0, 0);
    fn_80087994(g16, "Force project", &m188, 0, 0);
    fn_80087994(g16, "Simple project", &m18C, 0, 0);
    fn_8008771C(g16, "AABB bias", &m190, 0, 0, 0.0f, 10000.0f, 0.01f);
    fn_80087994(g16, "Shadow Clamp", &m17C, 0, 0);
    fn_8008771C(g16, "Clamp", &m180, 0, 0, 1.0f, 1000.0f, 1.0f);
    fn_800874C8(g16, "Lod Bias", &m178, 0, 5, 1, 0, 0);
    fn_80087994(g16, "Caster AABB", &m138, 0, 0);
    fn_80087994(g16, "Generate Frustum", &m140, 0, 0);
    fn_80087994(g16, "Enable Coverage", &m144, 0, 0);
    fn_80087994(g16, "Debug Coverage", &m14C, 0, 0);
    fn_80087994(g16, "Force Coverage", &m148, 0, 0);
    fn_80087994(g16, "Cull shadow shaders", &m150, 0, 0);
    fn_800874C8(g16, "Map Width", &m158, 16, 640, 16, 0, 0);
    fn_800874C8(g16, "Map Height", &m15C, 16, 528, 16, 0, 0);
    fn_8008771C(g16, "Min X", &m174, 0, 0, -5000.0f, 5000.0f, 10.0f);
    fn_8008771C(g16, "Max X", &m170, 0, 0, -5000.0f, 5000.0f, 10.0f);
    fn_8008771C(g16, "Min Y", &m16C, 0, 0, -5000.0f, 5000.0f, 10.0f);
    fn_8008771C(g16, "Max Y", &m168, 0, 0, -5000.0f, 5000.0f, 10.0f);
    fn_8008771C(g16, "Min Z", &m164, 0, 0, -5000.0f, 5000.0f, 10.0f);
    fn_8008771C(g16, "Max Z", &m160, 0, 0, -5000.0f, 5000.0f, 10.0f);
    fn_80087994(g16, "Use Depth Map", &m130, 0, 0);
    fn_80087994(g16, "Self Shadow", &m13C, 0, 0);
    fn_800874C8(g16, "Bias", &m154, -30000, 30000, 8, 0, 0);
    void* g17 = fn_8008937C(m04C, "Weather");
    fn_80087994(g17, "Clouds Enabled", &m2F8, 0, 0);
    fn_8008771C(g17, "Cloud Size", &m2FC, 0, 0, 1.0f, 100000.0f, 1.0f);
    fn_8008771C(g17, "Cloud Speed X", &m300, 0, 0, -10000.0f, 10000.0f, 1.0f);
    fn_8008771C(g17, "Cloud Speed Y", &m304, 0, 0, -10000.0f, 10000.0f, 1.0f);
    fn_80088FAC(m04C, "ClearColor", &m03C, 0, 0, 0.0f, 1.0f, 0.01f);
    void* g18 = fn_8008937C(m04C, "2DBlur");
    fn_8008771C(g18, "Blur Width", &m418, 0, 0, 0.0f, 10.0f, 0.1f);
    fn_8008771C(g18, "Blur Height", &m41C, 0, 0, 0.0f, 10.0f, 0.1f);
    fn_80087858(g18, "Blur Angle", &m420, 0, 0, -180.0f, 180.0f, 2.0f);
}

void GodRayParam_unk::fn_800BB4FC(void* menu) {
    fn_80087994(menu, "*Enabled", &mEnabled, 0, 0);
    fn_80087994(menu, "Debug", &mDebug, 0, 0);
    fn_800874C8(menu, "*Mode", &mMode, 0, 2, 1, 0, 0);
    fn_80087994(menu, "*Directional", &mDirectional, 0, 0);
    fn_80087E9C(menu, "*Position", &mPos, 0, 0, -10000.0f, 10000.0f, 0.01f);
    fn_80088AF4(menu, "*Color", &mColor, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(menu, "*BlurWidth", &mBlurWidth, 0, 0, -100.0f, 100.0f, 0.01f);
    fn_8008771C(menu, "*BlurHeight", &mBlurHeight, 0, 0, -100.0f, 100.0f, 0.01f);
    fn_80087994(menu, "Use Color", &mUseColor, 0, 0);
    fn_8008771C(menu, "Radius", &mRadius, 0, 0, 0.0f, 1000.0f, 0.01f);
    fn_8008771C(menu, "RadialScale", &mRadialScale, 0, 0, -100.0f, 100.0f, 0.01f);
    fn_800874C8(menu, "Iterations", &mIterations, 0, 64, 1, 0, 0);
}

void GodRayParam_unk::fn_800BB6C0(RsCsv_unk* csv) {
    mEnabled = strcmp(csv->fn_8005B9CC(), "ON") == 0;
    if (mEnabled) {
        const char* tok = csv->fn_8005B9CC();
        do {
            if (strcmp("COLOR", tok) == 0) {
                mColor.r = fn_800DC288(csv->fn_8005B9CC());
                mColor.g = fn_800DC288(csv->fn_8005B9CC());
                mColor.b = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("POS", tok) == 0) {
                mDirectional = 0;
                mPos.x = fn_800DC288(csv->fn_8005B9CC());
                mPos.y = fn_800DC288(csv->fn_8005B9CC());
                mPos.z = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("DIR", tok) == 0) {
                mDirectional = 1;
                mPos.x = fn_800DC288(csv->fn_8005B9CC());
                mPos.y = fn_800DC288(csv->fn_8005B9CC());
                mPos.z = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("SIZE", tok) == 0) {
                mBlurWidth = fn_800DC288(csv->fn_8005B9CC());
                mBlurHeight = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("MODE", tok) == 0) {
                tok = csv->fn_8005B9CC();
                if (strcmp("POINT", tok) == 0) {
                    mMode = 0;
                } else if (strcmp("DIRECTION", tok) == 0) {
                    mMode = 1;
                } else if (strcmp("CAMERA", tok) == 0) {
                    mMode = 2;
                }
            } else {
                tok = csv->fn_8005B9CC();
            }
        } while (strcmp("", tok) != 0);
    }
}

void RainParam_unk::fn_800BB8D8(void* menu) {
    fn_80087994(menu, "Enabled", &mEnabled, 0, 0);
    fn_80087994(menu, "Debug", &mDebug, 0, 0);
    fn_8008771C(menu, "Rate", &mRate, 0, 0, 0.0f, 10.0f, 0.01f);
    fn_8008771C(menu, "Warp", &mWarp, 0, 0, -100.0f, 100.0f, 0.01f);
}

void RainParam_unk::fn_800BB994(RsCsv_unk* csv) {
    mEnabled = strcmp(csv->fn_8005B9CC(), "ON") == 0;
    if (mEnabled) {
        const char* tok = csv->fn_8005B9CC();
        do {
            if (strcmp("WARP", tok) == 0) {
                mWarp = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("RATE", tok) == 0) {
                mRate = fn_800DC288(csv->fn_8005B9CC());
            } else {
                tok = csv->fn_8005B9CC();
            }
        } while (strcmp("", tok) != 0);
    }
}

void HazeParam_unk::fn_800BBA64(void* menu) {
    fn_80087994(menu, "Enabled", &mEnabled, 0, 0);
    fn_80087994(menu, "Debug", &mDebug, 0, 0);
    fn_8008771C(menu, "Warp", &mWarp, 0, 0, -100.0f, 100.0f, 0.01f);
    fn_80087BB4(menu, "Scale", &mScale, 0, 0, 0.0f, 100.0f, 0.01f);
    fn_80087BB4(menu, "Scroll", &mScroll, 0, 0, 0.0f, 100.0f, 0.001f);
    fn_80087BB4(menu, "Position", &mPos, 0, 0, 0.0f, 640.0f, 0.001f);
    fn_80087BB4(menu, "Size", &mSize, 0, 0, 1.0f, 640.0f, 0.001f);
    fn_8008826C(menu, "Fade", &mFade, 0, 0, 0.0f, 1.0f, 0.001f);
}

void HazeParam_unk::fn_800BBBB0(RsCsv_unk* csv) {
    mEnabled = strcmp(csv->fn_8005B9CC(), "ON") == 0;
    if (mEnabled) {
        const char* tok = csv->fn_8005B9CC();
        do {
            if (strcmp("WARP", tok) == 0) {
                mWarp = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("SCALE", tok) == 0) {
                mScale.x = fn_800DC288(csv->fn_8005B9CC());
                mScale.y = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("SCROLL", tok) == 0) {
                mScroll.x = fn_800DC288(csv->fn_8005B9CC());
                mScroll.y = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("POS", tok) == 0) {
                mPos.x = fn_800DC288(csv->fn_8005B9CC());
                mPos.y = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("SIZE", tok) == 0) {
                mSize.x = fn_800DC288(csv->fn_8005B9CC());
                mSize.y = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("FADE", tok) == 0) {
                mFade.x = fn_800DC288(csv->fn_8005B9CC());
                mFade.y = fn_800DC288(csv->fn_8005B9CC());
                mFade.z = fn_800DC288(csv->fn_8005B9CC());
                mFade.w = fn_800DC288(csv->fn_8005B9CC());
            } else {
                tok = csv->fn_8005B9CC();
            }
        } while (strcmp("", tok) != 0);
    }
}

void ShadowParam_unk::fn_800BBD90(RsCsv_unk* csv) {
    if (strcmp(csv->fn_8005B9CC(), "ON") == 0) {
        mEnabled = 1;
        mCullDist = fn_800DC288(csv->fn_8005B9CC());
        float r = fn_800DC288(csv->fn_8005B9CC());
        float g = fn_800DC288(csv->fn_8005B9CC());
        float b = fn_800DC288(csv->fn_8005B9CC());
        mColor = RsColor_unk(r, g, b, 1.0f);
        float x = fn_800DC288(csv->fn_8005B9CC());
        float y = fn_800DC288(csv->fn_8005B9CC());
        float z = fn_800DC288(csv->fn_8005B9CC());
        mDir = atn::Vector3(x, y, z);
        if (strcmp(csv->fn_8005B9CC(), "ADD") == 0) {
            mMode = 1;
        }
    } else {
        mEnabled = 0;
    }
}

void ShadowParam_unk::fn_800BBEFC(void* menu) {
    fn_80087994(menu, "Shadow Enabled", &mEnabled, 0, 0);
    fn_80087E9C(menu, "Direction", &mDir, 0, 0, -1.0f, 1.0f, 0.01f);
    fn_8008771C(menu, "Cull Distance", &mCullDist, 0, 0, 0.0f, 5000.0f, 10.0f);
    fn_80088AF4(menu, "Color", &mColor, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_800874C8(menu, "Mode", &mMode, 0, 1, 1, 0, 0);
}

void ColorMatrixParam_unk::fn_800BBFE8(RsCsv_unk* csv) {
    mEnabled = strcmp(csv->fn_8005B9CC(), "ON") == 0;
    mRows[0].x = fn_800DC288(csv->fn_8005B9CC());
    mRows[0].y = fn_800DC288(csv->fn_8005B9CC());
    mRows[0].z = fn_800DC288(csv->fn_8005B9CC());
    mRows[0].w = fn_800DC288(csv->fn_8005B9CC());
    mRows[1].x = fn_800DC288(csv->fn_8005B9CC());
    mRows[1].y = fn_800DC288(csv->fn_8005B9CC());
    mRows[1].z = fn_800DC288(csv->fn_8005B9CC());
    mRows[1].w = fn_800DC288(csv->fn_8005B9CC());
    mRows[2].x = fn_800DC288(csv->fn_8005B9CC());
    mRows[2].y = fn_800DC288(csv->fn_8005B9CC());
    mRows[2].z = fn_800DC288(csv->fn_8005B9CC());
    mRows[2].w = fn_800DC288(csv->fn_8005B9CC());
    mRows[3].x = fn_800DC288(csv->fn_8005B9CC());
    mRows[3].y = fn_800DC288(csv->fn_8005B9CC());
    mRows[3].z = fn_800DC288(csv->fn_8005B9CC());
    mRows[3].w = fn_800DC288(csv->fn_8005B9CC());
    mMono = strcmp(csv->fn_8005B9CC(), "ON") == 0;
    if (mMono) {
        mEnabled = 1;
    }
    mMonoRate = fn_800DC288(csv->fn_8005B9CC());
    fn_800DC12C(csv->fn_8005B9CC());
    mPriority = 0;
}

void ColorMatrixParam_unk::fn_800BC194(void* menu) {
    fn_80087994(menu, "Enabled", &mEnabled, 0, 0);
    fn_800874C8(menu, "Priority", &mPriority, -10, 10, 1, 0, 0);
    fn_8008826C(menu, "Row 0", &mRows[0], 0, 0, 0.0f, 4.0f, 0.01f);
    fn_8008826C(menu, "Row 1", &mRows[1], 0, 0, 0.0f, 4.0f, 0.01f);
    fn_8008826C(menu, "Row 2", &mRows[2], 0, 0, 0.0f, 4.0f, 0.01f);
    fn_8008826C(menu, "Row 3", &mRows[3], 0, 0, -0.5f, 0.5f, 0.01f);
    fn_80087994(menu, "MonoEnable", &mMono, 0, 0);
    fn_8008771C(menu, "MonoRate", &mMonoRate, 0, 0, 0.0f, 4.0f, 0.01f);
}

GammaParams_unk::GammaParams_unk() : mDebug(0), mEnable(0), mDepth(0.0f), mIndX(0.08f), mIndY(1.0f) {
    for (int i = 0; i < 8; i++) {
        mGamma[i] = 1.0f;
    }
}

void GammaParams_unk::fn_800BC328(RsCsv_unk* csv) {
    mEnable = strcmp(csv->fn_8005B9CC(), "ON") == 0;
    if (mEnable) {
        const char* tok = csv->fn_8005B9CC();
        do {
            if (strcmp("LUT", tok) == 0) {
                mIndX = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("PRIORITY", tok) == 0) {
                mDepth = fn_800DC288(csv->fn_8005B9CC());
            } else if (strcmp("GAMMA", tok) == 0) {
                for (int i = 0; i < 8; i++) {
                    mGamma[i] = fn_800DC288(csv->fn_8005B9CC());
                }
            } else {
                tok = csv->fn_8005B9CC();
            }
        } while (strcmp("", tok) != 0);
    }
}

// inferred: draw sort key of a model (called by atn/ModelDraw_unk): 6 low bits of m04, bits 4-26
// of the address of m08 and the priority (m08->m44) in the top 4 bits; names invented
struct RsSortObj_unk {
    unsigned char m00[0x44];
    int m44;
};
struct RsSortKey_unk {
    unsigned int mKey;
    unsigned int m04;
    RsSortObj_unk* m08;
};
extern "C" void fn_800BC438(RsSortKey_unk* k) {
    k->mKey = ((((unsigned int)k->m08 >> 4) & 0x7FFFFF) << 6) | (k->m04 & 0x3F);
    if (k->m08 != 0) {
        k->mKey |= k->m08->m44 << 28;
    }
}
