// atn/PostFxColorMatrix.cpp: efecto de post-proceso de matriz de color (transicion suave entre matrices
// de color y desaturacion).
#include "atn/PostFx.h"
#include "math/Vec.h"

// inferred: 4-component vector copied with paired singles (name invented)
struct ColorVec4_unk {
    ColorVec4_unk() {}
    ColorVec4_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    ColorVec4_unk(const ColorVec4_unk& v) { __Vec4Copy(&x, &v.x); }
    ColorVec4_unk& operator=(const ColorVec4_unk& v) {
        __Vec4Copy(&x, &v.x);
        return *this;
    }
    ColorVec4_unk operator*(float f) const { return ColorVec4_unk(x * f, y * f, z * f, w * f); }
    ColorVec4_unk operator+(const ColorVec4_unk& v) const { return ColorVec4_unk(x + v.x, y + v.y, z + v.z, w + v.w); }
    float x, y, z, w;
};

// inferred: color matrix with its parameters (0x50 bytes, names invented)
struct ColorMatrix_unk {
    ColorMatrix_unk() : mEnable(0), m44(0), m48(0), m4C(1.0f) {
        mRows[0] = ColorVec4_unk(1.0f, 0.0f, 0.0f, 0.0f);
        mRows[1] = ColorVec4_unk(0.0f, 1.0f, 0.0f, 0.0f);
        mRows[2] = ColorVec4_unk(0.0f, 0.0f, 1.0f, 0.0f);
        mRows[3] = ColorVec4_unk(0.0f, 0.0f, 0.0f, 0.0f);
    }
    int mEnable;
    ColorVec4_unk mRows[4];
    int m44;
    int m48;
    float m4C;
};

// inferred: render parameters (lbl_8087EFA8)
struct ColorMatrixParams_unk {
    unsigned char m000[0x324];
    ColorMatrix_unk m324; // 0x324
    unsigned char m374[0x36C - 0x374 + 0x8];
};
struct ColorMatrixParams2_unk {
    unsigned char m000[0x36C];
    int m36C;   // 0x36C: inferred: desaturation enabled
    float m370; // 0x370: inferred: desaturation amount
};
extern "C" ColorMatrixParams_unk* lbl_8087EFA8;

// inferred: object with its own color matrix (returned by fn_800C1424)
struct ColorMatrixOwner_unk {
    unsigned char m000[0x260];
    int m260;
    ColorMatrix_unk m264;
};
extern "C" ColorMatrixOwner_unk* fn_800C1424(PostFxFrameBuffer_unk* fb);
extern "C" void fn_800C0508(PostFxFrameBuffer_unk* fb, int arg);

// inferred: clamp limits (0.0f, 1.0f) of the time and of the four components, kept as globals in .sdata.
// The original keeps the zeros in .sdata, which MWCC only does with explicit_zero_data (approved exception,
// only around the zero definitions; without it they go to .sbss).
extern "C" {
#pragma push
#pragma explicit_zero_data on
float lbl_8087D828 = 0.0f;
#pragma pop
float lbl_8087D82C = 1.0f;
#pragma push
#pragma explicit_zero_data on
float lbl_8087D830 = 0.0f;
#pragma pop
float lbl_8087D834 = 1.0f;
#pragma push
#pragma explicit_zero_data on
float lbl_8087D838 = 0.0f;
#pragma pop
float lbl_8087D83C = 1.0f;
#pragma push
#pragma explicit_zero_data on
float lbl_8087D840 = 0.0f;
#pragma pop
float lbl_8087D844 = 1.0f;
#pragma push
#pragma explicit_zero_data on
float lbl_8087D848 = 0.0f;
#pragma pop
float lbl_8087D84C = 1.0f;
}

#define CM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CM_MAX(a, b) ((a) > (b) ? (a) : (b))

static inline float Clamp_unk(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

namespace atn {

class PostFxColorMatrix : public IPostFx {
public:
    PostFxColorMatrix();
    virtual ~PostFxColorMatrix() {}
    virtual void vf10(void* camera);
    void fn_800AA8DC();
    void fn_800AAE20();

    ColorMatrix_unk mCurrent; // 0x04
    ColorMatrix_unk mFrom;    // 0x54
    ColorMatrix_unk mTo;      // 0xA4
    ColorMatrixOwner_unk* mOwner; // 0xF4
    float mTime;              // 0xF8
};

PostFxColorMatrix::PostFxColorMatrix() : mOwner(0), mTime(0.0f) {}

// NONMATCHING: 96.85% - FPR allocation of the interpolation factors and scheduling of the desaturation
void PostFxColorMatrix::fn_800AA8DC() {
    ColorMatrixOwner_unk* owner = fn_800C1424(lbl_8087EFB4);
    ColorMatrix_unk* src = owner->m260 != 0 ? &owner->m264 : &lbl_8087EFA8->m324;
    mTime += 0.05f;
    if (mTime >= 1.0f) {
        mOwner = 0;
    }
    mTime = mTime < lbl_8087D828 ? lbl_8087D828 : (mTime > lbl_8087D82C ? lbl_8087D82C : mTime);
    if (mOwner == 0 || lbl_8087EFA8->m324.mEnable == 0) {
        mTime = 0.0f;
        mOwner = owner;
        mCurrent = *src;
        mTo = mCurrent;
        mFrom = mTo;
    }
    if (mOwner != owner) {
        mTime = 0.0f;
        mOwner = owner;
        mFrom = mCurrent;
    }
    mTo = *src;
    float t = mTime;
    float t2 = t * t;
    float s = 3.0f * t2 - 2.0f * (t * t2);
    float u = 1.0f - s;
    for (int i = 0; i < 4; i++) {
        mCurrent.mRows[i] = mFrom.mRows[i] * u + mTo.mRows[i] * s;
    }
    ColorMatrixParams2_unk* p = (ColorMatrixParams2_unk*)lbl_8087EFA8;
    if (p->m36C != 0) {
        float g = CM_MAX(0.0f, CM_MIN(1.0f, p->m370));
        float g2 = g * g;
        float k = 3.0f * g2 - 2.0f * (g * g2);
        float lumScale = CM_MAX(1.0f, p->m370);
        float lum[4] = {0.298912f * lumScale, 0.586611f * lumScale, 0.114478f * lumScale};
        float inv = 1.0f - k;
        for (int i = 0; i < 4; i++) {
            float l = lum[i];
            ColorVec4_unk gray(l, l, l, l);
            mCurrent.mRows[i] = mCurrent.mRows[i] * CM_MAX(0.0f, inv) + gray * k;
        }
    }
}

void PostFxColorMatrix::fn_800AAE20() {
    mOwner = 0;
}

static inline GXColor ToGXColor_unk(const ColorVec4_unk& v) {
    GXColor c = {(unsigned char)(255.0f * v.x), (unsigned char)(255.0f * v.y), (unsigned char)(255.0f * v.z),
                 (unsigned char)(255.0f * v.w)};
    return c;
}

// NONMATCHING: 76% - structure approximated (scale search and K colors through Vec4 temporaries)
void PostFxColorMatrix::vf10(void* camera) {
    fn_800C0508(lbl_8087EFB4, 1);
    GXInvalidateTexAll();
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(4);
    GXSetNumIndStages(0);
    fn_800763FC(lbl_8087EEE0, (GXTexObj*)((char*)lbl_8087EFB4 + 0x74C), 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    // inferred: columns of the matrix (rgb rows transposed, alpha 0)
    ColorVec4_unk cols[4] = {
        ColorVec4_unk(mCurrent.mRows[0].x, mCurrent.mRows[0].y, mCurrent.mRows[0].z, 0.0f),
        ColorVec4_unk(mCurrent.mRows[1].x, mCurrent.mRows[1].y, mCurrent.mRows[1].z, 0.0f),
        ColorVec4_unk(mCurrent.mRows[2].x, mCurrent.mRows[2].y, mCurrent.mRows[2].z, 0.0f),
        ColorVec4_unk(mCurrent.mRows[3].x, mCurrent.mRows[3].y, mCurrent.mRows[3].z, 0.0f),
    };
    int scale = 0;
    for (int i = 0; i < 4; i++) {
        ColorVec4_unk c = cols[i];
        for (int j = 0; j < 4; j++) {
            float v = (&c.x)[j];
            if (v > 2.0f) {
                scale = CM_MAX(scale, 2);
            } else if (v > 1.0f) {
                scale = CM_MAX(scale, 1);
            }
        }
    }
    float inv = 1.0f / (1 << scale);
    ColorVec4_unk k[4];
    for (int i = 0; i < 4; i++) {
        ColorVec4_unk c = cols[i];
        c.x = Clamp_unk(c.x * inv, lbl_8087D830, lbl_8087D834);
        c.y = Clamp_unk(c.y * inv, lbl_8087D838, lbl_8087D83C);
        c.z = Clamp_unk(c.z * inv, lbl_8087D840, lbl_8087D844);
        c.w = Clamp_unk(c.w * inv, lbl_8087D848, lbl_8087D84C);
        GXColor b = ToGXColor_unk(c);
        k[i] = ColorVec4_unk(b.r / 255.0f, b.g / 255.0f, b.b / 255.0f, b.a / 255.0f);
    }
    ColorVec4_unk bias(0.5f + mCurrent.mRows[3].x, 0.5f + mCurrent.mRows[3].y, 0.5f + mCurrent.mRows[3].z,
                       0.5f + mCurrent.mRows[3].w);
    k[3] = bias;
    GXSetTevKColor(0, ToGXColor_unk(k[0]));
    GXSetTevKColor(1, ToGXColor_unk(k[1]));
    GXSetTevKColor(2, ToGXColor_unk(k[2]));
    GXSetTevKColor(3, ToGXColor_unk(k[3]));
    GXSetTevSwapModeTable(0, 0, 0, 0, 0);
    GXSetTevSwapModeTable(1, 1, 1, 1, 1);
    GXSetTevSwapModeTable(2, 2, 2, 2, 2);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevColorIn(0, 0xF, 0xE, 8, 0xF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetTevSwapMode(1, 1, 1);
    GXSetTevKColorSel(1, 0xD);
    GXSetTevColorIn(1, 0xF, 0xE, 8, 0);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevOrder(1, 0, 0, 0xFF);
    GXSetTevDirect(1);
    GXSetTevSwapMode(2, 2, 2);
    GXSetTevKColorSel(2, 0xE);
    GXSetTevColorIn(2, 0xF, 0xE, 8, 0);
    GXSetTevColorOp(2, 0, 0, scale, 1, 0);
    GXSetTevOrder(2, 0, 0, 0xFF);
    GXSetTevDirect(2);
    GXSetTevKColorSel(3, 0xF);
    GXSetTevColorIn(3, 0xE, 0xF, 0xF, 0);
    GXSetTevColorOp(3, 0, 2, 0, 1, 0);
    GXSetTevOrder(3, 0xFF, 0xFF, 0xFF);
    GXSetTevDirect(3);
    GXSetBlendMode(0, 4, 1, 3);
    fn_800BFB70(lbl_8087EFB4);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
    GXSetTevSwapModeTable(2, 0, 1, 2, 3);
    GXSetTevSwapModeTable(3, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevSwapMode(2, 0, 0);
    GXSetTevSwapMode(3, 0, 0);
}

} // namespace atn
