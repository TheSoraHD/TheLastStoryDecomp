// atn/Scene.cpp: projected light (CharaLightData_unk) and atn::Scene, which owns the scene file, extra
// lights/cameras, two projected lights, the reflection plane and its render setting overrides.
// File name: atn_Scene.cpp sorts between atn_RenderManager.cpp and atn_Se_unk.cpp, and the RSO imports
// __ct__Q23atn5SceneFv for the constructor at 0x800C1A1C.
#include "atn/Scene.h"
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
        a.mData = new ((MemoryTag)heap, "", "", 0) Texture_unk[a.mCount];
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
void CharaLightData_unk::fn_800C18BC() {
    float rot = m1C + m20;
    m04.x += m14.x;
    m04.y += m14.y;
    m1C = WrapDeg_unk(rot);
    m4C++;
    if (m48 != 0 && m4C > m48) {
        m4C = 0;
        m44 = (m44 + 1) % mTextures.mCount;
    }
}

int CharaLightData_unk::fn_800C1990() {
    int busy = 0;
    for (unsigned int i = 0; i < mTextures.mCount; i++) {
        busy = busy || ((GimmickUvOwner_unk*)&mTextures.mData[i])->fn_800D59B8();
    }
    return busy;
}

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
inline Matrix3x4 Reflect_unk(const Plane_unk& p) {
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

// NONMATCHING: 67.54% - stack order of the temporaries (light direction, colors, plane) and FPR allocation
Scene::Scene()
    : mReady(0), mLightOn(0), m074(0), mLightMask(-1), mFloorLightOn(0), mReflectChara(0) {
    mAmbient = Color(0.0f, 0.0f, 0.0f, 1.0f);
    m02C = Color(1.0f, 1.0f, 1.0f, 1.0f);
    mLS = Color(0.3f, 0.3f, 0.3f, 1.0f);
    Plane_unk plane;
    plane.mNormal = Vector3(0.0f, 1.0f, 0.0f);
    plane.mDist = 0.0f;
    fn_800C2C20(&plane);
}

Scene::~Scene() {}

// inferred: true while the scene or its textures are loading
int Scene::fn_800C2108() {
    if (mFile.fn_80473F50() != 0) {
        return 1;
    }
    if (mReady == 0) {
        fn_800C2274();
    }
    bool loaded = mWhite.mImage != 0 || (((GimmickUvOwner_unk*)&mWhite)->fn_800D59B8() == 0 &&
                                         ((LoadHandle*)&mWhite.m20)->fn_80473EFC() == 3);
    if (!loaded) {
        fn_800D5908(&mWhite, "common/sys_white.texture");
    }
    int busy = ((GimmickUvOwner_unk*)&mWhite)->fn_800D59B8();
    if (mFloorLightOn != 0 && mFloorLight.fn_800C1990()) {
        busy = 1;
    }
    if (mLightOn != 0 && mLight.fn_800C1990()) {
        busy = 1;
    }
    return busy;
}

// inferred: ambient color from the lights of the file
void Scene::fn_800C2274() {
    mReady = 1;
    if (mFile.fn_80473EFC() == 3) {
        mAmbient = Color(0.0f, 0.0f, 0.0f, 1.0f);
        for (int i = 0; i < fn_80478014(&mFile); i++) {
            SceneLightData_unk* d = fn_80478034(&mFile)[i];
            float r = __EvalTrack_unk(d->mColor[0]) / 255.0f;
            float g = __EvalTrack_unk(d->mColor[1]) / 255.0f;
            float b = __EvalTrack_unk(d->mColor[2]) / 255.0f;
            float a = __EvalTrack_unk(d->mColor[3]) / 255.0f;
            mAmbient.r += r;
            mAmbient.g += g;
            mAmbient.b += b;
            mAmbient.a += a;
        }
    }
}

// inferred: number of lights (name invented)
inline int NumLights_unk(Scene* scene) {
    int n = scene->mLights.mSize;
    int m = fn_80477FDC(&scene->mFile);
    return m + n;
}

int Scene::fn_800C23E8() {
    return NumLights_unk(this);
}

int Scene::fn_800C2418() {
    int n = mCameras.mSize;
    int m = fn_80477FF8(&mFile);
    return m + n;
}

// inferred: light i (the lights of the file first)
inline Light* LightAt_unk(Scene* scene, int i) {
    if (i < fn_80477FDC(&scene->mFile)) {
        return &fn_80477FA4(&scene->mFile)[i];
    }
    i -= fn_80477FDC(&scene->mFile);
    return &scene->mLights.mData[i];
}

Light* Scene::GetLight(int i) {
    return LightAt_unk(this, i);
}

const Light* Scene::GetLight(int i) const {
    return LightAt_unk(const_cast<Scene*>(this), i);
}

Camera* Scene::fn_800C2520(int i) {
    if (i < fn_80477FF8(&mFile)) {
        return &fn_80477FC0(&mFile)[i];
    }
    int j = i - fn_80477FF8(&mFile);
    return &mCameras.mData[j];
}

// inferred: 2D vector operators (names invented)
inline Vector2_unk Mul2_unk(const Vector2_unk& v, float f) {
    return Vector2_unk(v.x * f, v.y * f);
}
inline Vector2_unk Sub2_unk(const Vector2_unk& a, const Vector2_unk& b) {
    return Vector2_unk(a.x - b.x, a.y - b.y);
}

// inferred: the floor light when pos is inside its rectangle
// NONMATCHING: 99.4% - FPR of the 0.5 constant
CharaLightData_unk* Scene::fn_800C258C(const Vector3* pos) {
    if (mFloorLightOn != 0) {
        Vector2_unk p(pos->x, pos->z);
        Vector2_unk min = Sub2_unk(mFloorLight.m04, Mul2_unk(mFloorLight.mSize, 0.5f));
        if (p.x >= min.x && p.x <= min.x + mFloorLight.mSize.x && p.y >= min.y &&
            p.y <= min.y + mFloorLight.mSize.y) {
            return &mFloorLight;
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
inline void EnableLight_unk(Scene* scene) {
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
void Scene::fn_800C2634(int n) {
    EnableLight_unk(this);
    ResizeTextures_unk(mLight.mTextures, n, 6);
    SetWrap_unk(mLight.mTextures, n);
}

// NONMATCHING: 85.85% - same wrap loop as fn_800C2634
void Scene::fn_800C289C(const char* path) {
    if (path == 0) {
        mLightOn = 0;
        return;
    }
    int n = 1;
    EnableLight_unk(this);
    ResizeTextures_unk(mLight.mTextures, n, 6);
    SetWrap_unk(mLight.mTextures, n);
    fn_800D5908(mLight.mTextures.mData, path);
}

void Scene::fn_800C2B24(const Vector2_unk* pos, const Vector2_unk* size, const char* path) {
    mFloorLightOn = 1;
    mFloorLight.mDir = Vector3(0.0f, 0.0f, 0.0f);
    mFloorLight.m04 = *pos;
    mFloorLight.mSize = *size;
    mFloorLight.m00 = 1;
    ResizeTextures_unk(mFloorLight.mTextures, 1, 6);
    fn_800D5908(mFloorLight.mTextures.mData, path);
    mFloorLight.m14 = Vector2_unk(0.0f, 0.0f);
}

// NONMATCHING: 67.43% - FPR allocation and order of the reflection matrix products
void Scene::fn_800C2C20(const Plane_unk* plane) {
    mPlane.mNormal = plane->mNormal;
    mPlane.mDist = plane->mDist;
    mReflect = Reflect_unk(*plane);
}

void Scene::fn_800C2D88() {
    for (int i = 0; i < NumLights_unk(this); i++) {
        LightAt_unk(this, i)->fn_800795DC();
    }
}

// inferred: callback of the debug menu entries of the reflection plane (a free function with the scene as
// its argument)
extern "C" void fn_800C2EE0(Scene* scene);

void Scene::fn_800C2E30(void* menu) {
    fn_80087994(menu, "Reflect Chara", &mReflectChara, 0, 0);
    fn_80087E9C(menu, "Reflect Plane", &mPlane.mNormal, (void (*)(void*))fn_800C2EE0, this, -1.0f, 1.0f, 0.1f);
    fn_8008771C(menu, "Reflect Y", &mPlane.mDist, (void (*)(void*))fn_800C2EE0, this, -10000.0f, 100000.0f, 0.01f);
}

// NONMATCHING: 71.86% - fn_800C2C20 inlined with a different FPR allocation (the original converts n.z for m23)
extern "C" void fn_800C2EE0(Scene* scene) {
    scene->mPlane.mNormal.Normalize_unk();
    scene->fn_800C2C20(&scene->mPlane);
}
