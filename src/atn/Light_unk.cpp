// atn/Light_unk.cpp: luz del motor (direccional, foco o puntual), lectura desde el fichero de escena,
// conversion a GXLightObj y dibujo/menu de depuracion (nombre de fichero inventado).
#include "atn/Light_unk.h"
#include "atn/GXDecl.h"

extern "C" double sqrt(double x);
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_80063D3C(void* dbg, const atn::Vector3* pos, unsigned int color, float radius, float arg);
extern "C" void fn_800641CC(void* dbg, const atn::Vector3* pos, const atn::Vector3* dir, unsigned int color,
                            float length, float angle, float arg);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

Light_unk::Light_unk()
    : mData(0), mType(4), mPos(0.0f, 0.0f, 0.0f), mDir(0.0f, 1.0f, 0.0f), mAttnType(0), mRadius(0.0f) {
    mColor.r = 0.0f;
    mColor.g = 0.0f;
    mColor.b = 0.0f;
    mColor.a = 0.0f;
}

Light_unk::Light_unk(LightData_unk* data) : mData(data), mType(data->mType) {
    fn_80079280(0.0f);
}

void Light_unk::fn_800790D0(const atn::Vector3& dir, const LightColor_unk& color) {
    mType = 1;
    atn::Vector3 n(dir);
    n.Normalize_unk();
    mDir.Assign_unk(n);
    mPos = dir * -9999.0f;
    mColor.r = color.r;
    mColor.g = color.g;
    mColor.b = color.b;
    mColor.a = color.a;
    mAttnType = 0;
}

void Light_unk::fn_800791B4(const atn::Vector3& pos, const LightColor_unk& color, float radius) {
    mType = 3;
    mPos = pos;
    mRadius = radius;
    mRefBright = 0.02f;
    mRefDist = (double)radius; // inferred: converted through double (frsp in the original)
    mColor.r = color.r;
    mColor.g = color.g;
    mColor.b = color.b;
    mColor.a = color.a;
    mAttnType = 2;
}

void Light_unk::fn_80079210(const atn::Vector3& pos, const atn::Vector3& dir, const LightColor_unk& color,
                            float radius, float angle) {
    mType = 2;
    mPos = pos;
    mDir = dir;
    mRadius = radius;
    mRefBright = 0.02f;
    mRefDist = (double)radius;
    mAngle = angle;
    mColor.r = color.r;
    mColor.g = color.g;
    mColor.b = color.b;
    mColor.a = color.a;
    mAttnType = 2;
}

// inferred: value of a track (each call has its own frame variable on the stack)
static inline float EvalTrack_unk(SceneTrack_unk& track, float value) {
    short frame = 0;
    return track.fn_8047202C(frame, value);
}

// NONMATCHING: 99.74% - FPR allocation of the radius formula (b2/a2 swapped, (4*c2)*k operand order)
void Light_unk::fn_80079280(float frame) {
    if (mData == 0) {
        return;
    }
    float px = EvalTrack_unk(mData->mPos[0], frame);
    float py = EvalTrack_unk(mData->mPos[1], frame);
    float pz = EvalTrack_unk(mData->mPos[2], frame);
    float tx = EvalTrack_unk(mData->mTarget[0], frame);
    float ty = EvalTrack_unk(mData->mTarget[1], frame);
    float tz = EvalTrack_unk(mData->mTarget[2], frame);
    atn::Vector3 pos(px, py, pz);
    atn::Vector3 target(tx, ty, tz);
    atn::Vector3 dir;
    dir = target - pos;
    dir.Normalize_unk();
    mPos = pos;
    mDir = dir;
    float r = EvalTrack_unk(mData->mColor[0], frame) / 255.0f;
    float g = EvalTrack_unk(mData->mColor[1], frame) / 255.0f;
    float b = EvalTrack_unk(mData->mColor[2], frame) / 255.0f;
    float a = EvalTrack_unk(mData->mColor[3], frame) / 255.0f;
    mColor = LightColor_unk(r, g, b, a);
    if (mType == 2) {
        mAngle = EvalTrack_unk(mData->mAngle, frame);
    }
    if (!(mType != 2 && mType != 3)) {
        mAttnType = 2;
        mRefDist = EvalTrack_unk(mData->mRefDist, frame);
        mRefBright = EvalTrack_unk(mData->mRefBright, frame);
        float c, b, a;
        fn_800798D8(&a, &b, &c);
        float b2 = 0.02f * b;
        float c2 = 0.02f * c;
        float ac4 = 4.0f * c2;
        ac4 *= -0.98f;
        float a2 = 2.0f * c2;
        float bb = b2 * b2;
        float d = sqrt(bb - ac4);
        mRadius = __fabs((-b2 + d) / a2);
    }
}

// inferred: 0.0-1.0 to 0-255
static inline unsigned int ToU8_unk(float v) {
    return v >= 1.0f ? 255 : v <= 0.0f ? 0 : (unsigned int)(255.0f * v + 0.5f);
}

// inferred: ARGB color for the debug drawing
static inline unsigned int ToARGB_unk(const LightColor_unk& c) {
    unsigned int r = ToU8_unk(c.r);
    unsigned int g = ToU8_unk(c.g);
    unsigned int b = ToU8_unk(c.b);
    unsigned int a = ToU8_unk(c.a);
    return (a << 24) | (r << 16) | (g << 8) | b;
}

// NONMATCHING: 96.52% - scheduling of the call arguments after the color conversion
void Light_unk::fn_800795DC() {
    switch (mType) {
    case 3:
        fn_80063D3C(lbl_8087EEB0, &mPos, ToARGB_unk(mColor), mRadius, 0.0f);
        break;
    case 2:
        fn_800641CC(lbl_8087EEB0, &mPos, &mDir, ToARGB_unk(mColor), 2.0f * mRadius, mAngle, 0.0f);
        break;
    }
}

void Light_unk::fn_800798D8(float* a, float* b, float* c) {
    float dist = mRefDist;
    float bright = mRefBright;
    switch (mAttnType) {
    case 1:
        *a = 1.0f;
        *b = (1.0f - bright) / (bright * dist);
        *c = 0.0f;
        break;
    case 2:
        *a = 1.0f;
        *b = 0.5f * (1.0f - bright) / (bright * dist);
        *c = 0.5f * (1.0f - bright) / (dist * (bright * dist));
        break;
    case 3:
        *a = 1.0f;
        *b = 0.0f;
        *c = (1.0f - bright) / (dist * (bright * dist));
        break;
    default:
        *a = 1.0f;
        *b = 0.0f;
        *c = 0.0f;
        break;
    }
}

// inferred: GX color of a light color
static inline GXColor ToGXColor_unk(const LightColor_unk& c) {
    GXColor color = {(unsigned char)(255.0f * c.r), (unsigned char)(255.0f * c.g), (unsigned char)(255.0f * c.b),
                     (unsigned char)(255.0f * c.a)};
    return color;
}

// NONMATCHING: 76% - the original negates mDir with fneg (results kept as double) but -1.0f is still
// in .sdata2 before 1000000.0f; written as -1.0f * x it keeps the pool order (fn_80079BC8 then matches)
// but multiplies; the prologue is also interleaved with the color conversion in the original
void Light_unk::fn_80079994(GXLightObj* obj, Mtx viewMtx) {
    GXInitLightColor(obj, ToGXColor_unk(mColor));
    int type = mType;
    atn::Vector3 pos(mPos);
    if (type == 1) {
        pos = atn::Vector3(-1.0f * mDir.x, -1.0f * mDir.y, -1.0f * mDir.z) * 1000000.0f;
    }
    PSMTXMultVec(viewMtx, &pos, &pos);
    GXInitLightPos(obj, pos.x, pos.y, pos.z);
    switch (mType) {
    case 3:
        GXInitLightAttn(obj, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        GXInitLightDistAttn(obj, mRadius, 0.07f, 2);
        break;
    case 2: {
        atn::Vector3 dir(mDir);
        GXInitLightDir(obj, dir.x, dir.y, dir.z);
        GXInitLightSpot(obj, mAngle, 2);
        GXInitLightDistAttn(obj, 1.0f, 0.07f, 0);
        break;
    }
    case 1:
        GXInitLightAttn(obj, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        GXInitLightDistAttn(obj, 1.0f, 0.07f, 0);
        break;
    }
}

void Light_unk::fn_80079BC8(void* menu) {
    switch (mType) {
    case 3:
        fn_80087E9C(menu, "Position", &mPos, 0, 0, -10000.0f, 10000.0f, 1.0f);
        fn_8008771C(menu, "Radius", &mRadius, 0, 0, 0.0f, 10000.0f, 1.0f);
        fn_8008771C(menu, "Radius2", &mRefDist, 0, 0, 0.0f, 10000.0f, 1.0f);
        fn_8008771C(menu, "Color R", &mColor.r, 0, 0, 0.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Color G", &mColor.g, 0, 0, 0.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Color B", &mColor.b, 0, 0, 0.0f, 1.0f, 0.01f);
        break;
    case 2:
        fn_80087E9C(menu, "Position", &mPos, 0, 0, -10000.0f, 10000.0f, 1.0f);
        fn_80087E9C(menu, "Direction", &mDir, 0, 0, -1.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Length", &mRadius, 0, 0, 0.0f, 10000.0f, 1.0f);
        fn_8008771C(menu, "Length2", &mRefDist, 0, 0, 0.0f, 10000.0f, 1.0f);
        fn_8008771C(menu, "Angle", &mAngle, 0, 0, 0.0f, 360.0f, 1.0f);
        fn_8008771C(menu, "Color R", &mColor.r, 0, 0, 0.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Color G", &mColor.g, 0, 0, 0.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Color B", &mColor.b, 0, 0, 0.0f, 1.0f, 0.01f);
        break;
    case 1:
        fn_80087E9C(menu, "Direction", &mDir, 0, 0, -1.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Color R", &mColor.r, 0, 0, 0.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Color G", &mColor.g, 0, 0, 0.0f, 1.0f, 0.01f);
        fn_8008771C(menu, "Color B", &mColor.b, 0, 0, 0.0f, 1.0f, 0.01f);
        break;
    }
}
