// atn/CharaLightData_unk.cpp: projected light (CharaLightData_unk) and the scene wrapper that owns the
// scene file, extra lights/cameras, two projected lights and the reflection plane (file name invented).
#include "gmk/CharaLight.h"
#include "atn/LH_Scene.h"
#include "atn/Light_unk.h"
#include "atn/Camera_unk.h"
#include "atn/Memory.h"
#include "std/vector.h"

extern "C" double fmod(double x, double y);
extern "C" void fn_800D5908(atn::Texture_unk* tex, const char* path); // inferred: loads a texture


// inferred: debug menu entries (see game/GimmickCommon.cpp)
extern "C" void fn_80087994(void* menu, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_80087E9C(void* menu, const char* name, atn::Vector3* value, void (*cb)(void*), void* arg,
                            float min, float max, float step);
extern "C" void fn_8008771C(void* menu, const char* name, float* value, void (*cb)(void*), void* arg, float min,
                            float max, float step);

using namespace atn;

// inferred: resizes the texture array (names invented)
inline void ResizeTextures_unk(TextureArray_unk& a, int n, int heap) {
    if (a.mData != 0) {
        delete[] a.mData;
    }
    a.mCount = n;
    if (a.mCount != 0) {
        a.mData = new (heap, "", "", 0) Texture_unk[a.mCount];
    } else {
        a.mData = 0;
    }
}

CharaLightData_unk::CharaLightData_unk()
    : m00(1), m04(0.0f, 0.0f), mSize(0.0f, 0.0f), m14(0.0f, 0.0f), m1C(0.0f), m20(0.0f), mDir(0.0f, 0.0f, 0.0f), m30(0.0f),
      m34(0.0f), m38(0.0f), m44(0), m48(0), m4C(0) {
}

CharaLightData_unk::CharaLightData_unk(const char* texture)
    : m00(1), m04(0.0f, 0.0f), mSize(0.0f, 0.0f), m14(0.0f, 0.0f), m1C(0.0f), m20(0.0f), mDir(0.0f, 0.0f, 0.0f), m30(0.0f),
      m34(0.0f), m38(0.0f), m44(0), m48(0), m4C(0) {
    ResizeTextures_unk(mTextures, 1, 0);
    fn_800D5908(mTextures.mData, texture);
}

void CharaLightData_unk::fn_800C1814(const char* path) {
    ResizeTextures_unk(mTextures, 1, 0);
    fn_800D5908(mTextures.mData, path);
}

// inferred: name invented
inline float WrapDeg_unk(float a) {
    a = fmod(a, 360.0);
    if (a > 180.0f) {
        a -= 360.0f;
    }
    if (a < -180.0f) {
        a += 360.0f;
    }
    return a;
}

// inferred: updates the light (scroll, rotation and texture animation)
extern "C" void fn_800C18BC(CharaLightData_unk* light) {
    float rot = light->m1C + light->m20;
    light->m04.x += light->m14.x;
    light->m04.y += light->m14.y;
    light->m1C = WrapDeg_unk(rot);
    light->m4C++;
    if (light->m48 != 0 && light->m4C > light->m48) {
        light->m4C = 0;
        light->m44 = (light->m44 + 1) % light->mTextures.mCount;
    }
}

int CharaLightData_unk::fn_800C1990() {
    int busy = 0;
    for (unsigned int i = 0; i < mTextures.mCount; i++) {
        busy = busy || ((GimmickUvOwner_unk*)&mTextures.mData[i])->fn_800D59B8();
    }
    return busy;
}

// inferred: color (names invented)
struct SceneColor_unk {
    SceneColor_unk() {}
    SceneColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: fog of the scene (names invented)
struct SceneFog_unk {
    SceneFog_unk() : mType(0), mStart(150.0f), mEnd(500.0f), mColor(0.5f, 0.5f, 0.5f, 1.0f) {}
    int mType;
    float mStart;
    float mEnd;
    SceneColor_unk mColor;
};

// inferred: plane (normal, distance)
struct ScenePlane_unk {
    Vector3 mNormal;
    float mDist;
};

// inferred: 4-float row (names invented)
struct SceneRow_unk {
    SceneRow_unk() {}
    SceneRow_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    SceneRow_unk(const SceneRow_unk& o) {
        __Vec4Copy(&x, &o.x);
    }
    SceneRow_unk& operator=(const SceneRow_unk& o) {
        __Vec4Copy(&x, &o.x);
        return *this;
    }
    float x, y, z, w;
};

// inferred: light data of the scene file (only the color tracks)
struct SceneLightData_unk {
    unsigned char m00[0x18];
    SceneTrack_unk mColor[4]; // 0x18
};

extern "C" int fn_80477FDC(LH_Scene* scene);        // inferred: number of lights of the file
extern "C" Light_unk* fn_80477FA4(LH_Scene* scene); // inferred: lights of the file
extern "C" int fn_80477FF8(LH_Scene* scene);        // inferred: number of cameras of the file
extern "C" Camera_unk* fn_80477FC0(LH_Scene* scene); // inferred: cameras of the file
extern "C" int fn_80478014(LH_Scene* scene);        // inferred: number of light records of the file
extern "C" SceneLightData_unk** fn_80478034(LH_Scene* scene); // inferred: light records of the file

// inferred: value of a track at frame 0 (as in atn/LH_Scene.cpp)
static inline float __EvalTrack_unk(SceneTrack_unk& track) {
    short frame = 0;
    return track.fn_8047202C(frame, 0.0f);
}

// inferred: rows and translation of a matrix (names invented)
inline void SetRow_unk(Matrix3x4& m, int i, const Vector3& v) {
    m.m[i][0] = v.x;
    m.m[i][1] = v.y;
    m.m[i][2] = v.z;
}
inline void SetTrans_unk(Matrix3x4& m, const Vector3& v) {
    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;
}

// inferred: reflection matrix of a plane (name invented)
inline Matrix3x4 Reflect_unk(const ScenePlane_unk& p) {
    float a = -2.0f * p.mNormal.x;
    float c = -2.0f * p.mNormal.z;
    float b = -2.0f * p.mNormal.y;
    Matrix3x4 m;
    Vector3 r0(a * p.mNormal.x + 1.0f, a * p.mNormal.y, a * p.mNormal.z);
    Vector3 r1(b * p.mNormal.x, b * p.mNormal.y + 1.0f, b * p.mNormal.z);
    Vector3 r2(c * p.mNormal.x, c * p.mNormal.y, c * p.mNormal.z + 1.0f);
    Vector3 t(a * p.mDist, b * p.mDist, c * p.mDist);
    SetRow_unk(m, 0, r0);
    SetRow_unk(m, 1, r1);
    SetRow_unk(m, 2, r2);
    SetTrans_unk(m, t);
    return m;
}

// inferred: scene of the camp and the map (0x2B4 bytes; class name unknown, used by atn/RenderSystem_unk,
// game/GameConfig_unk and game/MapPartsBase)
struct NewCampScene2_unk {
    NewCampScene2_unk();
    ~NewCampScene2_unk();

    LH_Scene mFile;                     // 0x000
    int mReady;                         // 0x008
    SceneColor_unk mAmbient;            // 0x00C
    SceneColor_unk m1C;                 // 0x01C
    SceneColor_unk m2C;                 // 0x02C
    SceneFog_unk mFog;                  // 0x03C
    std::vector<Light_unk> mLights;     // 0x058
    std::vector<Camera_unk> mCameras;   // 0x064
    int mLightOn;                       // 0x070
    int m074;
    CharaLightData_unk mLight;          // 0x078
    int m0C8;
    int mFloorLightOn;                  // 0x0CC
    CharaLightData_unk mFloorLight;     // 0x0D0
    int mReflectChara;                  // 0x120
    Matrix3x4 mReflect;                 // 0x124
    ScenePlane_unk mPlane;              // 0x154
    Texture_unk mWhite;                 // 0x164
    int m194;
    int m198;
    int m19C;
    int m1A0;
    int m1A4;
    int m1A8;
    int m1AC;
    int m1B0;
    float m1B4;
    float m1B8;
    float m1BC;
    float m1C0;
    float m1C4;
    float m1C8;
    float m1CC;
    float m1D0;
    float m1D4;
    float m1D8;
    float m1DC;
    int m1E0;
    int m1E4;
    int m1E8;
    float m1EC;
    float m1F0;
    int m1F4;
    int m1F8;
    int m1FC;
    float m200;
    float m204;
    float m208;
    float m20C;
    float m210;
    float m214;
    float m218;
    float m21C;
    float m220;
    float m224;
    float m228;
    float m22C;
    float m230;
    int m234;
    int m238;
    int m23C;
    float m240;
    float m244;
    float m248;
    float m24C;
    float m250;
    Vector3 m254;
    int m260;
    int m264;
    SceneRow_unk m268[4];
    int m2A8;
    int m2AC;
    float m2B0;
};

// inferred: name invented
inline Vector3 Normalized_unk(const Vector3& v) {
    return v.GetNormalized_unk();
}

// inferred: identity 4x4 matrix as rows (name invented)
inline void SetIdentityRows_unk(SceneRow_unk* r) {
    r[0] = SceneRow_unk(1.0f, 0.0f, 0.0f, 0.0f);
    r[1] = SceneRow_unk(0.0f, 1.0f, 0.0f, 0.0f);
    r[2] = SceneRow_unk(0.0f, 0.0f, 1.0f, 0.0f);
    r[3] = SceneRow_unk(0.0f, 0.0f, 0.0f, 1.0f);
}

extern "C" void fn_800C2274(NewCampScene2_unk* scene);
extern "C" void fn_800C2C20(NewCampScene2_unk* scene, const ScenePlane_unk* plane);

// NONMATCHING: 67.54% - stack order of the temporaries (light direction, colors, plane) and FPR allocation
NewCampScene2_unk::NewCampScene2_unk()
    : mReady(0), mLightOn(0), m074(0), m0C8(-1), mFloorLightOn(0), mReflectChara(0), m194(0), m198(0), m19C(0),
      m1A0(2), m1A4(3), m1A8(1), m1AC(0), m1B0(0), m1B4(-3.0f), m1B8(-3.0f), m1BC(1.0f), m1C0(0.75f), m1C4(0.75f),
      m1C8(0.75f), m1CC(1.0f), m1D0(0.0f), m1D4(1.0f), m1D8(0.0f), m1DC(100.0f), m1E0(0), m1E4(0), m1E8(0),
      m1EC(1.0f), m1F0(0.05f), m1F4(0), m1F8(0), m1FC(0), m200(0.2f), m204(0.0f), m208(0.01f), m20C(10.0f),
      m210(5.0f), m214(0.0f), m218(224.0f), m21C(640.0f), m220(224.0f), m224(0.0f), m228(0.0f), m22C(1.0f),
      m230(1.0f), m234(0), m238(1), m23C(0), m240(250.0f), m244(0.5f), m248(0.5f), m24C(0.5f), m250(1.0f),
      m260(0), m264(0),
      m2A8(0), m2AC(0), m2B0(1.0f) {
    m254 = Vector3(1.0f, -1.0f, 1.0f).GetNormalized_unk();
    SetIdentityRows_unk(m268);
    mAmbient = SceneColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
    m2C = SceneColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
    m1C = SceneColor_unk(0.3f, 0.3f, 0.3f, 1.0f);
    ScenePlane_unk plane;
    plane.mNormal = Vector3(0.0f, 1.0f, 0.0f);
    plane.mDist = 0.0f;
    fn_800C2C20(this, &plane);
}

NewCampScene2_unk::~NewCampScene2_unk() {}

// inferred: true while the scene or its textures are loading
extern "C" int fn_800C2108(NewCampScene2_unk* scene) {
    if (scene->mFile.fn_80473F50() != 0) {
        return 1;
    }
    if (scene->mReady == 0) {
        fn_800C2274(scene);
    }
    bool loaded = scene->mWhite.mImage != 0 || (((GimmickUvOwner_unk*)&scene->mWhite)->fn_800D59B8() == 0 &&
                                                ((LoadHandle*)&scene->mWhite.m20)->fn_80473EFC() == 3);
    if (!loaded) {
        fn_800D5908(&scene->mWhite, "common/sys_white.texture");
    }
    int busy = ((GimmickUvOwner_unk*)&scene->mWhite)->fn_800D59B8();
    if (scene->mFloorLightOn != 0 && scene->mFloorLight.fn_800C1990()) {
        busy = 1;
    }
    if (scene->mLightOn != 0 && scene->mLight.fn_800C1990()) {
        busy = 1;
    }
    return busy;
}

// inferred: ambient color from the lights of the file
extern "C" void fn_800C2274(NewCampScene2_unk* scene) {
    scene->mReady = 1;
    if (scene->mFile.fn_80473EFC() == 3) {
        scene->mAmbient = SceneColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
        for (int i = 0; i < fn_80478014(&scene->mFile); i++) {
            SceneLightData_unk* d = fn_80478034(&scene->mFile)[i];
            float r = __EvalTrack_unk(d->mColor[0]) / 255.0f;
            float g = __EvalTrack_unk(d->mColor[1]) / 255.0f;
            float b = __EvalTrack_unk(d->mColor[2]) / 255.0f;
            float a = __EvalTrack_unk(d->mColor[3]) / 255.0f;
            scene->mAmbient.r += r;
            scene->mAmbient.g += g;
            scene->mAmbient.b += b;
            scene->mAmbient.a += a;
        }
    }
}

// inferred: number of lights (name invented)
inline int NumLights_unk(NewCampScene2_unk* scene) {
    int n = scene->mLights.mSize;
    int m = fn_80477FDC(&scene->mFile);
    return m + n;
}

extern "C" int fn_800C23E8(NewCampScene2_unk* scene) {
    return NumLights_unk(scene);
}

extern "C" int fn_800C2418(NewCampScene2_unk* scene) {
    int n = scene->mCameras.mSize;
    int m = fn_80477FF8(&scene->mFile);
    return m + n;
}

// inferred: light i (the lights of the file first)
inline Light_unk* GetLight_unk(NewCampScene2_unk* scene, int i) {
    if (i < fn_80477FDC(&scene->mFile)) {
        return &fn_80477FA4(&scene->mFile)[i];
    }
    i -= fn_80477FDC(&scene->mFile);
    return &scene->mLights.mData[i];
}

extern "C" Light_unk* fn_800C2448(NewCampScene2_unk* scene, int i) {
    return GetLight_unk(scene, i);
}

extern "C" const Light_unk* fn_800C24B4(NewCampScene2_unk* scene, int i) {
    return GetLight_unk(scene, i);
}

extern "C" Camera_unk* fn_800C2520(NewCampScene2_unk* scene, int i) {
    if (i < fn_80477FF8(&scene->mFile)) {
        return &fn_80477FC0(&scene->mFile)[i];
    }
    int j = i - fn_80477FF8(&scene->mFile);
    return &scene->mCameras.mData[j];
}

// inferred: the floor light when pos is inside its rectangle
// inferred: 2D vector operators (names invented)
inline Vector2_unk Mul2_unk(const Vector2_unk& v, float f) {
    return Vector2_unk(v.x * f, v.y * f);
}
inline Vector2_unk Sub2_unk(const Vector2_unk& a, const Vector2_unk& b) {
    return Vector2_unk(a.x - b.x, a.y - b.y);
}

// NONMATCHING: 99.4% - FPR of the 0.5 constant
extern "C" CharaLightData_unk* fn_800C258C(NewCampScene2_unk* scene, const Vector3* pos) {
    if (scene->mFloorLightOn != 0) {
        Vector2_unk p(pos->x, pos->z);
        Vector2_unk min = Sub2_unk(scene->mFloorLight.m04, Mul2_unk(scene->mFloorLight.mSize, 0.5f));
        if (p.x >= min.x && p.x <= min.x + scene->mFloorLight.mSize.x && p.y >= min.y &&
            p.y <= min.y + scene->mFloorLight.mSize.y) {
            return &scene->mFloorLight;
        }
    }
    return 0;
}

// inferred: sets the geometry of a light (name invented)
inline void SetLight_unk(CharaLightData_unk& l, const Vector2_unk& speed, const Vector3& dir, const Vector2_unk& pos,
                         const Vector2_unk& size) {
    l.mSize = size;
    l.m04 = pos;
    l.mDir = dir;
    l.m14 = speed;
}

// inferred: turns the character light on with default values (name invented)
inline void EnableLight_unk(NewCampScene2_unk* scene) {
    if (scene->mLightOn == 0) {
        scene->mLightOn = 1;
        scene->mLight.m00 = 1;
        SetLight_unk(scene->mLight, Vector2_unk(0.5f, 0.5f), Vector3(0.0f, 0.0f, 0.0f), Vector2_unk(0.0f, 0.0f),
                     Vector2_unk(1.0f, 1.0f));
    }
}

// inferred: wrap mode of the textures (name invented)
inline void SetWrap_unk(TextureArray_unk& a, int n) {
    for (int i = 0; i < n; i++) {
        Texture_unk& t = a.mData[i];
        t.m2D = 1;
        t.m2E = 1;
    }
}

// NONMATCHING: 85.55% - the original indexes the textures with mulli in the unrolled wrap loop
extern "C" void fn_800C2634(NewCampScene2_unk* scene, int n) {
    EnableLight_unk(scene);
    ResizeTextures_unk(scene->mLight.mTextures, n, 6);
    SetWrap_unk(scene->mLight.mTextures, n);
}

// NONMATCHING: 85.85% - same wrap loop as fn_800C2634
extern "C" void fn_800C289C(NewCampScene2_unk* scene, const char* path) {
    if (path == 0) {
        scene->mLightOn = 0;
        return;
    }
    int n = 1;
    EnableLight_unk(scene);
    ResizeTextures_unk(scene->mLight.mTextures, n, 6);
    SetWrap_unk(scene->mLight.mTextures, n);
    fn_800D5908(scene->mLight.mTextures.mData, path);
}

extern "C" void fn_800C2B24(NewCampScene2_unk* scene, const Vector2_unk* pos, const Vector2_unk* size,
                            const char* path) {
    scene->mFloorLightOn = 1;
    scene->mFloorLight.mDir = Vector3(0.0f, 0.0f, 0.0f);
    scene->mFloorLight.m04 = *pos;
    scene->mFloorLight.mSize = *size;
    scene->mFloorLight.m00 = 1;
    ResizeTextures_unk(scene->mFloorLight.mTextures, 1, 6);
    fn_800D5908(scene->mFloorLight.mTextures.mData, path);
    scene->mFloorLight.m14 = Vector2_unk(0.0f, 0.0f);
}

// NONMATCHING: 67.43% - FPR allocation and order of the reflection matrix products
extern "C" void fn_800C2C20(NewCampScene2_unk* scene, const ScenePlane_unk* plane) {
    scene->mPlane.mNormal = plane->mNormal;
    scene->mPlane.mDist = plane->mDist;
    scene->mReflect = Reflect_unk(*plane);
}

extern "C" void fn_800C2D88(NewCampScene2_unk* scene) {
    for (int i = 0; i < NumLights_unk(scene); i++) {
        GetLight_unk(scene, i)->fn_800795DC();
    }
}

extern "C" void fn_800C2EE0(NewCampScene2_unk* scene);

extern "C" void fn_800C2E30(NewCampScene2_unk* scene, void* menu) {
    fn_80087994(menu, "Reflect Chara", &scene->mReflectChara, 0, 0);
    fn_80087E9C(menu, "Reflect Plane", &scene->mPlane.mNormal, (void (*)(void*))fn_800C2EE0, scene, -1.0f, 1.0f,
                0.1f);
    fn_8008771C(menu, "Reflect Y", &scene->mPlane.mDist, (void (*)(void*))fn_800C2EE0, scene, -10000.0f, 100000.0f,
                0.01f);
}

// NONMATCHING: 71.86% - fn_800C2C20 inlined with a different FPR allocation (the original converts n.z for m23)
extern "C" void fn_800C2EE0(NewCampScene2_unk* scene) {
    scene->mPlane.mNormal.Normalize_unk();
    fn_800C2C20(scene, &scene->mPlane);
}
