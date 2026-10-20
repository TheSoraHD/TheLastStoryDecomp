#include "atn/GXDecl.h"
#include "atn/Memory.h"
#include "math/Vec.h"

extern "C" int rand();

// inferred: GX functions not in atn/GXDecl.h (enums as int)
extern "C" {
void GXClearVtxDesc();
void GXSetVtxAttrFmt(int fmt, int attr, int cnt, int type, unsigned char frac);
void GXSetVtxDesc(int attr, int type);
void GXSetChanMatColor(int chan, GXColor color);
void GXSetChanAmbColor(int chan, GXColor color);
void GXSetCurrentMtx(unsigned int id);
void GXLoadPosMtxImm(const Mtx m, unsigned int id);
void GXLoadNrmMtxImm(const Mtx m, unsigned int id);
void GXSetTevKColor(int id, GXColor color);
void GXSetTevKColorSel(int stage, int sel);
void GXSetTevKAlphaSel(int stage, int sel);
void GXSetTevColorIn(int stage, int a, int b, int c, int d);
void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
void GXSetTevColorOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void GXSetPointSize(unsigned char size, int texOffsets);
void GXSetLineWidth(unsigned char width, int texOffsets);
void GXEnableTexOffsets(int coord, unsigned char line, unsigned char point);
void GXBegin(int type, int fmt, unsigned short count);
void PSMTXInverse(const Mtx src, Mtx inv);
void PSMTXTranspose(const Mtx src, Mtx xpose);
}

// inferred: write gather pipe of the GX FIFO
union PPCWGPipe_unk {
    float f;
    unsigned int u;
};
#define GXWGFifo (*(volatile PPCWGPipe_unk*)0xCC008000)

// inferred: GXPosition3f32 of the SDK (inline)
inline void GXPosition3f32_unk(float x, float y, float z) {
    GXWGFifo.f = x;
    GXWGFifo.f = y;
    GXWGFifo.f = z;
}

// inferred: render state, camera and debug menu functions (automatic names)
struct RenderState_unk;
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" void fn_80076760(RenderState_unk* rs, int state, int value);
extern "C" void fn_800761A8(RenderState_unk* rs);
struct Camera_unk {
    unsigned char m000[0x15C];
    atn::Matrix3x4 m15C; // inferred: view matrix
    unsigned char m18C[0x2F8 - 0x18C];
    int m2F8; // inferred: current pass
};
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void fn_800BDB58(Camera_unk* camera, void* render, float order, int pass); // inferred: queues a render
extern "C" int fn_800502A8(void* frustum, const void* box); // inferred: box visibility test
// inferred: ray test against the map (start, end, hit), returns nonzero on hit
extern "C" int fn_8004ECC0(void* map, atn::Vector3* hit, const atn::Vector3* start, const atn::Vector3* end,
                           unsigned int mask, int arg5, int arg6);
extern "C" void* lbl_8087EE98;
extern "C" void* fn_8008937C(void* parent, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: 3D vector
extern "C" void fn_80088AF4(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: color

// inferred: draws the drops as lines (1) or points (0)
static int sRainLines = 1;

namespace atn {

// inferred: renderer interface (see atn/VideoTHP.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C(); // 0x80046218
    virtual void vf10(Camera_unk* camera) = 0;
    virtual int vf14(void* arg1, void* arg2, void* frustum) = 0;
    virtual void vf18() = 0;
    virtual void vf1C(); // 0x800461FC
};

// inferred: RGBA color with by-value operators (name invented)
struct RainColor_unk {
    RainColor_unk() {}
    RainColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    RainColor_unk operator-(const RainColor_unk& o) const { return RainColor_unk(r - o.r, g - o.g, b - o.b, a - o.a); }
    RainColor_unk operator+(const RainColor_unk& o) const { return RainColor_unk(r + o.r, g + o.g, b + o.b, a + o.a); }
    RainColor_unk operator*(float f) const { return RainColor_unk(r * f, g * f, b * f, a * f); }
    float r, g, b, a;
};

// inferred: one drop (0x18 bytes)
struct RainDrop_unk {
    RainDrop_unk() {}
    Vector3 mPos;  // 0x00
    float mSpeed;  // 0x0C
    float mFloor;  // 0x10: inferred: height where it hits the ground
    float mLength; // 0x14
};

// inferred: array of drops {capacity, count, data}
struct RainDrops_unk {
    // inferred: name invented
    void Resize_unk(unsigned int count) {
        RainDrop_unk* drops;
        if (count != 0) {
            drops = new (6, "", "", 0) RainDrop_unk[count];
        } else {
            drops = 0;
        }
        if (mData != 0) {
            unsigned int n = (count > mCapacity) ? mCapacity : count;
            for (unsigned int i = 0; i < n; i++) {
                drops[i] = mData[i];
            }
            delete[] mData;
        }
        mData = drops;
        mCapacity = count;
        mCount = count;
    }
    ~RainDrops_unk() {
        if (mData != 0) {
            delete[] mData;
        }
    }
    unsigned int mCapacity;
    unsigned int mCount;
    RainDrop_unk* mData;
};

// inferred from __vt__Q23atn4Rain (0x80778EF8)
class Rain : public IRender {
public:
    Rain();
    virtual ~Rain();
    virtual void vf10(Camera_unk* camera);
    virtual int vf14(void* arg1, void* arg2, void* frustum) { return fn_800502A8(frustum, &mAreaMin); }
    virtual void vf18() {}

    void fn_800B1C1C(); // inferred: queues the render
    void fn_800B2180(); // inferred: allocates the drops
    void fn_800B23C8(void* menu); // inferred: debug menu
    void fn_800B257C(RainDrop_unk* drop, int randomY); // inferred: respawns a drop
    void fn_800B28F8(Rain* a, Rain* b, float t); // inferred: interpolates two settings
    void fn_800B2D2C(); // inferred: moves the drops

    int m04;
    RainDrops_unk mDrops; // 0x08
    Vector3 mDirection;   // 0x14
    RainColor_unk mColor; // 0x20
    Vector3 mAreaMin;     // 0x30
    Vector3 mAreaMax;     // 0x3C
    int mCount;           // 0x48
    float mMinSpeed;      // 0x4C
    float mMaxSpeed;      // 0x50
    float mSize;          // 0x54
    float mLength;        // 0x58
    float mLengthVar;     // 0x5C
};

// inferred: linear interpolation (inline template, name invented)
template <class T>
inline T Lerp_unk(const T& a, const T& b, float t) {
    return (b - a) * t + a;
}
inline float Lerp_unk(float a, float b, float t) {
    return t * (b - a) + a;
}

// inferred: random value in [min, max] with a step of 1/1000 (inline, name invented)
inline float RandRange_unk(float min, float max) {
    return (max - min) * ((float)(rand() % 1001) / 1000.0f) + min;
}

inline GXColor ToGXColor_unk(const RainColor_unk& c) {
    GXColor gx;
    gx.r = (unsigned char)(255.0f * c.r);
    gx.g = (unsigned char)(255.0f * c.g);
    gx.b = (unsigned char)(255.0f * c.b);
    gx.a = (unsigned char)(255.0f * c.a);
    return gx;
}

Rain::Rain() {
    mDrops.mCapacity = 0;
    mDrops.mCount = 0;
    mDrops.mData = 0;
    mDirection.x = 0.25f;
    mDirection.y = -1.0f;
    mDirection.z = 0.0f;
    mColor.r = 0.2f;
    mColor.g = 0.3f;
    mColor.b = 0.4f;
    mColor.a = 0.5f;
    mCount = -1;
    mMinSpeed = 3.0f;
    mMaxSpeed = 5.0f;
    mSize = 2.5f;
    mLength = 2.5f;
    mLengthVar = 1.0f;
}

Rain::~Rain() {}

void Rain::fn_800B1C1C() {
    fn_800BDB58(lbl_8087EFB4, this, -1.0f, 8);
}

void fn_800B1C44(Rain* rain, Camera_unk* camera);

void Rain::vf10(Camera_unk* camera) {
    if (camera->m2F8 == 8) {
        fn_800B1C44(this, camera);
        return;
    }
}

// NONMATCHING: 99.67% - volatile registers of the two vertex loops differ
void fn_800B1C44(Rain* rain, Camera_unk* camera) {
    if (rain->mCount <= 0) {
        return;
    }
    RenderState_unk* rs = lbl_8087EEE0;
    fn_80076760(rs, 8, 1);
    fn_80076760(rs, 9, 2);
    fn_80076760(rs, 10, 0);
    fn_80076760(rs, 6, 0);
    fn_800761A8(rs);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxDesc(9, 1);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 1, 0, 2);
    GXSetChanMatColor(4, ToGXColor_unk(rain->mColor));
    GXSetChanAmbColor(4, ToGXColor_unk(RainColor_unk(1.0f, 1.0f, 1.0f, 1.0f)));
    GXSetCurrentMtx(0);
    Matrix3x4 view(camera->m15C);
    GXLoadPosMtxImm(view.m, 0);
    Matrix3x4 normal(view);
    PSMTXInverse(normal.m, normal.m);
    PSMTXTranspose(normal.m, normal.m);
    GXLoadNrmMtxImm(normal.m, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetTevKColor(0, ToGXColor_unk(rain->mColor));
    GXSetTevKColorSel(0, 0xC);
    GXSetTevKAlphaSel(0, 0x1C);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevDirect(0);
    GXSetTevAlphaIn(0, 7, 7, 7, 5);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    float size = 6.0f * rain->mSize;
    float max = (255.0f <= size) ? 255.0f : size;
    float clamped = (0.0f >= max) ? 0.0f : max;
    int width = (int)clamped;
    GXSetPointSize(width, 4);
    GXSetLineWidth(width, 4);
    GXEnableTexOffsets(0, 0, 1);
    GXSetBlendMode(1, 4, 5, 3);
    if (sRainLines == 0) {
        GXBegin(0xB8, 0, rain->mCount);
        for (unsigned int i = 0; i < rain->mCount; i++) {
            RainDrop_unk* drop = &rain->mDrops.mData[i];
            GXPosition3f32_unk(drop->mPos.x, drop->mPos.y, drop->mPos.z);
        }
    } else {
        GXBegin(0xA8, 0, rain->mCount * 2);
        for (unsigned int i = 0; i < rain->mCount; i++) {
            RainDrop_unk* drop = &rain->mDrops.mData[i];
            Vector3 pos(drop->mPos);
            Vector3 end = pos + rain->mDirection * rain->mLength;
            GXPosition3f32_unk(pos.x, pos.y, pos.z);
            GXPosition3f32_unk(end.x, end.y, end.z);
        }
    }
}

void Rain::fn_800B2180() {
    mDrops.Resize_unk(mCount);
    for (int i = 0; i < mCount; i++) {
        fn_800B257C(&mDrops.mData[i], 1);
    }
}

void Rain::fn_800B23C8(void* menu) {
    void* group = fn_8008937C(menu, "RainDrops");
    fn_800874C8(group, "Count", &mCount, 0, mCount, 1, 0, 0);
    fn_80088AF4(group, "Color", &mColor.r, 0, 0, -255.0f, 255.0f, 0.1f);
    fn_80087E9C(group, "Direction", &mDirection.x, 0, 0, -1.0f, 1.0f, 0.1f);
    fn_8008771C(group, "Min Spd", &mMinSpeed, 0, 0, 0.0f, 1000.0f, 0.01f);
    fn_8008771C(group, "Max Spd", &mMaxSpeed, 0, 0, 0.0f, 1000.0f, 0.01f);
    fn_8008771C(group, "Size", &mSize, 0, 0, 1.0f, 42.0f, 1.0f / 6.0f);
    fn_8008771C(group, "Length", &mLength, 0, 0, 0.0f, 42.0f, 1.0f / 6.0f);
    fn_8008771C(group, "Length Var", &mLengthVar, 0, 0, 0.0f, 42.0f, 1.0f / 6.0f);
    fn_80087E9C(group, "Area Min", &mAreaMin.x, 0, 0, -10000.0f, 10000.0f, 0.1f);
    fn_80087E9C(group, "Area Max", &mAreaMax.x, 0, 0, -10000.0f, 10000.0f, 0.1f);
}

// NONMATCHING: 89.93% - stack slots and scheduling of the vector temporaries of the ray test
void Rain::fn_800B257C(RainDrop_unk* drop, int randomY) {
    float x = RandRange_unk(mAreaMin.x, mAreaMax.x);
    float y = mAreaMax.y;
    float z = RandRange_unk(mAreaMin.z, mAreaMax.z);
    Vector3 down = mDirection * 1000.0f;
    Vector3 pos(x, y, z);
    Vector3 up = mDirection * 250.0f;
    Vector3 top(x, y, z);
    Vector3 hit;
    if (fn_8004ECC0(lbl_8087EE98, &hit, &(top - up), &(pos + down), 0x80000000, 0, 0) == 0) {

        drop->mFloor = mAreaMin.y;
    } else {
        drop->mFloor = (hit.y >= mAreaMin.y) ? hit.y : mAreaMin.y;
    }
    drop->mPos = Vector3(x, y, z);
    drop->mSpeed = RandRange_unk(mMinSpeed, mMaxSpeed);
    float var = 0.5f * mLengthVar;
    float length = mLength + RandRange_unk(-0.5f * mLengthVar, var);
    length = (42.0f <= length) ? 42.0f : length;
    drop->mLength = (0.0f >= length) ? 0.0f : length;
    if (randomY) {
        float h = drop->mPos.y - drop->mFloor;
        drop->mPos.y = mDirection.y * (h * ((float)(rand() % 1001) / 1000.0f)) + drop->mPos.y;
    }
}

// NONMATCHING: 27% - same operations; the original computes most values before storing them (all
// the float registers are saved) and keeps the vector temporaries on the stack
void Rain::fn_800B28F8(Rain* a, Rain* b, float t) {
    int count = (int)Lerp_unk((float)a->mCount, (float)b->mCount, t);
    mCount = count;
    mCount = (count < (int)mDrops.mCapacity) ? count : (int)mDrops.mCapacity;
    mAreaMin = Lerp_unk(a->mAreaMin, b->mAreaMin, t);
    mAreaMax = Lerp_unk(a->mAreaMax, b->mAreaMax, t);
    mColor = Lerp_unk(a->mColor, b->mColor, t);
    mMinSpeed = Lerp_unk(a->mMinSpeed, b->mMinSpeed, t);
    mMaxSpeed = Lerp_unk(a->mMaxSpeed, b->mMaxSpeed, t);
    mSize = Lerp_unk(a->mSize, b->mSize, t);
    mLength = Lerp_unk(a->mLength, b->mLength, t);
    mLengthVar = Lerp_unk(a->mLengthVar, b->mLengthVar, t);
    mDirection = Lerp_unk(a->mDirection, b->mDirection, t);
}

void Rain::fn_800B2D2C() {
    for (int i = 0; i < mCount; i++) {
        RainDrop_unk* drop = &mDrops.mData[i];
        Vector3 step = mDirection * drop->mSpeed;
        drop->mPos += step;
        float y = drop->mPos.y - drop->mFloor;
        if (y < drop->mLength) {
            drop->mLength = y;
            if (y < 0.1f) {
                fn_800B257C(drop, 0);
            }
        }
    }
}

} // namespace atn
