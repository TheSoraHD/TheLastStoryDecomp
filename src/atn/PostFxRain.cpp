// atn/PostFxRain.cpp: rain post effect (drops on the lens drawn into an indirect texture that warps the
// frame, plus scrolling heat waves).
#include "atn/PostFx.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"
#include "atn/Memory.h"
#include "atn/DrawHelper.h"

extern "C" int rand();
extern "C" void PSMTXScale(float m[3][4], float x, float y, float z);
extern "C" void GXSetIndTexOrder(int stage, int coord, int map);
extern "C" void GXSetIndTexCoordScale(int stage, int scaleS, int scaleT);
extern "C" void GXSetTevIndWarp(int stage, int indStage, unsigned char signedOffset, unsigned char replaceMode,
                                int matrix);
extern "C" void GXSetIndTexMtx(int id, const float offset[2][3], signed char scaleExp);
extern "C" void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
extern "C" void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);

// inferred: drop settings (lbl_8087EFA8 + 0x2AC, or the override of fn_800C1424 + 0x1E4)
struct RainDropSet_unk {
    int mEnable; // 0x00
    int mDebug;  // 0x04
    float mRate; // 0x08: inferred: drops per frame
    float mSize; // 0x0C
};
// inferred: heat wave settings (lbl_8087EFA8 + 0x2BC, or the override of fn_800C1424 + 0x1F8)
struct RainHeatSet_unk {
    int mEnable;          // 0x00
    int mDebug;           // 0x04
    float mAlpha;         // 0x08
    float mScrollU;       // 0x0C
    float mScrollV;       // 0x10
    atn::Vector2_unk mScale; // 0x14
    float mX, mY, mW, mH; // 0x1C: inferred: viewport
    float mAlphas[4];     // 0x2C: inferred: alpha of the four corners
};
struct RainParams_unk {
    unsigned char m00[0x2AC];
    RainDropSet_unk m2AC;
    RainHeatSet_unk m2BC;
};
extern "C" RainParams_unk* lbl_8087EFA8;

// inferred: settings that override the parameters (returned by fn_800C1424, see atn/PostFxColorMatrix.cpp)
struct RainOverride_unk {
    unsigned char m00[0x1E0];
    int m1E0;
    RainDropSet_unk m1E4;
    int m1F4;
    RainHeatSet_unk m1F8;
};
extern "C" RainOverride_unk* fn_800C1424(PostFxFrameBuffer_unk* fb);

// inferred: frame buffer object (lbl_8087EFB4); only the member used here
struct RainFrameBuffer_unk {
    unsigned char m00[0x74C];
    GXTexObj m74C; // 0x74C: inferred: color texture
};

// inferred: render manager with its clear color (ARGB) at 0x48
struct RainRenderMgr_unk {
    unsigned char m00[0x48];
    unsigned int mClearColor;
};

// inferred: RGBA color passed by value (copied with float loads/stores)
struct RainColor_unk {
    RainColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    RainColor_unk(const RainColor_unk& c) {
        float r_ = c.r;
        float g_ = c.g;
        float b_ = c.b;
        float a_ = c.a;
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    float r, g, b, a;
};

extern "C" void fn_800C0508(PostFxFrameBuffer_unk* fb, int arg);
extern "C" void fn_800BFC18(PostFxFrameBuffer_unk* fb, RainColor_unk color, float a, float b);
extern "C" void fn_800BFCB4(PostFxFrameBuffer_unk* fb, RainColor_unk c0, RainColor_unk c1, RainColor_unk c2,
                            RainColor_unk c3, int width, int height, float a, float b);
extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path); // inferred: loads a texture and waits

// inferred: rain drop on the lens (0x1C bytes)
struct PostFxRainDrop_unk {
    PostFxRainDrop_unk();
    float mX, mY, mW, mH; // 0x00
    float mAlpha;         // 0x10
    float mTime;          // 0x14
    int mState;           // 0x18: 0 free, 1 growing, 2 sliding
};

// inferred: array of rain drops (see atn/LinearAllocator.cpp)
struct PostFxRainDrops_unk {
    PostFxRainDrops_unk() : mCount(0), mData(0) {}
    // inferred: its "" literals are the weak @STRING@ pool of .sdata 0x8087D870/0x8087D874; the
    // "!= 0" test is folded in the constructor but the store of the else branch stays
    void Alloc_unk(unsigned int n) {
        if (mData != 0) {
            delete[] mData;
        }
        mCount = n;
        if (mCount != 0) {
            mData = new ((MemoryTag)0, "", "", 0) PostFxRainDrop_unk[mCount];
        } else {
            mData = 0;
        }
    }
    PostFxRainDrop_unk& operator[](unsigned int i) { return mData[i]; }
    unsigned int mCount;
    PostFxRainDrop_unk* mData;
};

namespace atn {

class PostFxRain : public IPostFx {
public:
    PostFxRain();
    virtual ~PostFxRain() {}
    virtual void vf10(void* camera);
    int fn_800B0A48(); // inferred: drops or heat waves active
    void fn_800B0A70();
    void fn_800B0CD4();
    void fn_800B1554();
    void fn_800B1598();

    int m04;              // 0x04: inferred: drops active
    int m08;
    float m0C;            // 0x0C: inferred: drop rate (smoothed)
    float m10;            // 0x10: inferred: drop size (smoothed)
    RainDropSet_unk* m14; // 0x14
    int m18;              // 0x18: inferred: heat waves active
    int m1C;
    float m20;            // 0x20: inferred: heat wave alpha (smoothed)
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    float m38;
    float m3C;
    float m40;
    float m44;
    float m48;
    float m4C;
    float m50;
    RainHeatSet_unk* m54; // 0x54
    float m58;            // 0x58: inferred: heat wave scroll
    float m5C;
    GXTexObj m60;         // 0x60: inferred: drops (indirect texture)
    PostFxRainDrops_unk m80;
    Texture_unk m88;      // 0x88: "drop_warp.texture"
    Texture_unk mB8;      // 0xB8: "heat_waves01.texture"
};

// inferred: the dead store after __construct_new_array is the folded else branch of Alloc_unk; the clearing
// loop is written here, not as an inline method of PostFxRainDrops_unk (that swaps its registers)
PostFxRain::PostFxRain()
    : m04(0), m08(0), m0C(1.0f), m10(0.05f), m14(0), m18(0), m1C(0), m20(0.2f), m24(0.0f), m28(0.01f), m2C(10.0f),
      m30(5.0f), m34(0.0f), m38(224.0f), m3C(640.0f), m40(224.0f), m44(0.0f), m48(0.0f), m4C(1.0f), m50(1.0f),
      m54(0), m58(0.0f), m5C(0.0f) {
    m80.Alloc_unk(0x100);
    for (unsigned int i = 0; i < m80.mCount; i++) {
        m80[i].mState = 0;
    }
    mB8.m2D = 1;
    mB8.m2E = 1;
    fn_800D594C(&mB8, "heat_waves01.texture");
    fn_800D594C(&m88, "drop_warp.texture");
    m20 = m0C = m10 = 0.0f;
}

} // namespace atn

PostFxRainDrop_unk::PostFxRainDrop_unk() {}

namespace atn {

int PostFxRain::fn_800B0A48() {
    return m04 != 0 || m18 != 0;
}

// inferred: draws the drops into the indirect texture
// inferred: the drop pointer declared before the loop, the rectangle through int locals and operator[] give
// the registers of the original
void PostFxRain::fn_800B0A70() {
    GXInvalidateTexAll();
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    Matrix3x4 m;
    m.SetIdentity();
    GXLoadTexMtxImm(m.m, 0x1E, 1);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    fn_800763FC(lbl_8087EEE0, &m88.mObj, 0);
    GXSetBlendMode(1, 4, 5, 3);
    PostFxRainDrop_unk* drop;
    for (unsigned int i = 0; i < m80.mCount; i++) {
        drop = &m80[i];
        if (drop->mState != 0) {
            int x = drop->mX;
            int y = drop->mY;
            int w = drop->mW;
            int h = drop->mH;
            fn_80075F58(lbl_8087EEE0, x, y, w, h);
            fn_800BFC18(lbl_8087EFB4, RainColor_unk(1.0f, 1.0f, 1.0f, drop->mAlpha * m0C), 0.0f, 0.0f);
        }
    }
    fn_80075DEC(lbl_8087EEE0);
    GXSetBlendMode(0, 1, 0, 3);
}

// inferred: scale matrix (its vector is a local of the inline)
static inline void ScaleMtx_unk(Matrix3x4& m, const Vector2_unk& scale) {
    Vector3 s(scale.x, scale.y, 1.0f);
    PSMTXScale(m.m, s.x, s.y, s.z);
}

// inferred: sets the translation column of a matrix
static inline void SetTrans_unk(Matrix3x4& m, float x, float y, float z) {
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
}

// inferred: draws the scrolling heat waves into the indirect texture
// inferred: the four colors are an array (ascending on the stack), the scale vector a local of an inline, the
// screen size is read into locals before the call and the translation is set by an inline with parameters
void PostFxRain::fn_800B0CD4() {
    GXInvalidateTexAll();
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    RainHeatSet_unk* heat = m54;
    Vector2_unk scale = heat->mScale;
    m58 += heat->mScrollU;
    m5C += heat->mScrollV;
    if (m5C > scale.y) {
        m5C -= scale.y;
    }
    if (m58 > scale.x) {
        m58 -= scale.x;
    }
    Matrix3x4 m;
    ScaleMtx_unk(m, scale);
    SetTrans_unk(m, m58, m5C, 0.0f);
    GXLoadTexMtxImm(m.m, 0x1E, 1);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    fn_800763FC(lbl_8087EEE0, &mB8.mObj, 0);
    GXSetBlendMode(1, 4, 5, 3);
    RainColor_unk c[4] = {
        RainColor_unk(1.0f, 1.0f, 1.0f, m20 * m54->mAlphas[0]),
        RainColor_unk(1.0f, 1.0f, 1.0f, m20 * m54->mAlphas[1]),
        RainColor_unk(1.0f, 1.0f, 1.0f, m20 * m54->mAlphas[2]),
        RainColor_unk(1.0f, 1.0f, 1.0f, m20 * m54->mAlphas[3]),
    };
    fn_80075F58(lbl_8087EEE0, m54->mX, m54->mY, m54->mW, m54->mH);
    int sh = lbl_8087EEE0->mHeight;
    int sw = lbl_8087EEE0->mWidth;
    fn_800BFCB4(lbl_8087EFB4, c[0], c[1], c[2], c[3], sw, sh, 0.0f, 0.0f);
    fn_80075DEC(lbl_8087EEE0);
    GXSetBlendMode(0, 1, 0, 3);
}

template <class T>
inline const T& Max_unk(const T& a, const T& b) {
    return a < b ? b : a;
}

// inferred: color helpers (their results are temporaries of the caller)
static inline GXColor RainGXColor_unk(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    GXColor c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}
static inline GXColor RainARGB_unk(unsigned int argb) {
    GXColor c;
    c.b = argb;
    c.r = argb >> 16;
    c.g = argb >> 8;
    c.a = argb >> 24;
    return c;
}

// NONMATCHING: 99.72% - `this`, the screen size and the 0xFFFFFF of GXSetCopyClear in other saved registers
// (~16 variants; the colors built by inline helpers give the stack slots of the original)
void PostFxRain::vf10(void* camera) {
    if (!fn_800B0A48()) {
        return;
    }
    int fullW = lbl_8087EEE0->mWidth;
    int fullH = lbl_8087EEE0->mHeight;
    int w = fullW >> 1;
    int h = fullH >> 1;
    void* image = lbl_8087EF8C->fn_800A96AC(fullW, fullH, 1);
    GXInitTexObj(&m60, image, w, h, 1, 0, 0, 0);
    GXInitTexObjLOD(&m60, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXPixModeSync();
    GXSetCopyClear(RainGXColor_unk(0x7F, 0x7F, 0x7F, 0x7F), 0xFFFFFF);
    fn_800C0508(lbl_8087EFB4, 1);
    GXSetCopyClear(RainARGB_unk(((RainRenderMgr_unk*)lbl_8087EEE0)->mClearColor), 0xFFFFFF);
    fn_800B0CD4();
    fn_800B0A70();
    GXSetTexCopySrc(0, 0, (unsigned short)w * 2, (unsigned short)h * 2);
    GXSetTexCopyDst(w, h, 0x28, 1);
    GXCopyTex(image, 1);
    GXPixModeSync();
    GXInvalidateTexAll();
    // inferred: draws the frame warped by the indirect texture
    GXSetBlendMode(0, 1, 0, 3);
    GXSetNumTexGens(1);
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetTevOp(0, 0);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetNumIndStages(1);
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    Matrix3x4 m;
    m.SetIdentity();
    GXLoadTexMtxImm(m.m, 0x21, 1);
    GXLoadTexMtxImm(m.m, 0x1E, 1);
    GXSetIndTexOrder(0, 1, 1);
    GXSetIndTexCoordScale(0, 0, 0);
    fn_800763FC(lbl_8087EEE0, &((RainFrameBuffer_unk*)lbl_8087EFB4)->m74C, 0);
    fn_800763FC(lbl_8087EEE0, &m60, 1);
    GXSetTevIndWarp(0, 0, 1, 0, 1);
    float ind[2][3];
    ind[0][0] = 1.0f;
    ind[0][1] = 0.0f;
    ind[0][2] = 0.0f;
    ind[1][0] = 0.0f;
    ind[1][1] = 1.0f;
    ind[1][2] = 0.0f;
    // inferred: brings the matrix into [-1, 1) with the scale exponent
    int exp = 0;
    bool first = true;
    bool again;
    do {
        again = false;
        float mx = ind[0][0];
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                mx = Max_unk(mx, ind[i][j]);
            }
        }
        if (mx >= 1.0f || mx < -1.0f) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    ind[i][j] *= 0.5f;
                }
            }
            again = true;
            if (!first) {
                exp++;
            }
            first = false;
        }
    } while (again);
    GXSetIndTexMtx(1, ind, exp);
    fn_80075DEC(lbl_8087EEE0);
    fn_800BFB70(lbl_8087EFB4);
    GXSetBlendMode(0, 1, 0, 3);
    if (lbl_8087EFA8->m2AC.mDebug != 0 || lbl_8087EFA8->m2BC.mDebug != 0) {
        lbl_8087EEB0->fn_8005DF90(-1, &m60, 0, 16.0f, 16.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
    }
}

// inferred: removes every drop
void PostFxRain::fn_800B1554() {
    m20 = m0C = m10 = 0.0f;
    for (unsigned int i = 0; i < m80.mCount; i++) {
        m80[i].mState = 0;
    }
}

// inferred: random value in [lo, hi]
inline float Rand_unk(float lo, float hi) {
    return (hi - lo) * ((rand() % 1001) / 1000.0f) + lo;
}

// inferred: update: follows the settings and moves the drops
void PostFxRain::fn_800B1598() {
    PostFxFrameBuffer_unk* fb = lbl_8087EFB4;
    m14 = &lbl_8087EFA8->m2AC;
    m54 = &lbl_8087EFA8->m2BC;
    RainOverride_unk* ovr = fn_800C1424(fb);
    if (ovr->m1E0 != 0) {
        m14 = &ovr->m1E4;
    }
    if (ovr->m1F4 != 0) {
        m54 = &ovr->m1F8;
    }
    int enable = m14->mEnable;
    float rate = enable != 0 ? m14->mRate : 0.0f;
    float size = enable != 0 ? m14->mSize : 0.0f;
    if (m04 == 0) {
        m04 = enable;
    } else if (enable == 0) {
        m04 = m0C > 0.0f;
    }
    int heatEnable = m54->mEnable;
    float heat = heatEnable != 0 ? m54->mAlpha : 0.0f;
    if (m18 == 0) {
        m18 = heatEnable;
    } else if (heatEnable == 0) {
        m18 = m20 > 0.0f;
    }
    if (fn_800B0A48()) {
        float diff = rate - m0C;
        if ((float)__fabs(diff) < 0.01f) {
            m0C = rate;
        } else {
            m0C = 0.97f * m0C + 0.03f * diff;
        }
        if ((float)__fabs(size - m10) < 0.01f) {
            m10 = size;
        } else {
            m10 = 0.97f * m10 + 0.03f * size;
        }
        if ((float)__fabs(heat - m20) < 0.01f) {
            m20 = heat;
        } else {
            m20 = 0.97f * m20 + 0.03f * heat;
        }
        float n = m10;
        do {
            if (Rand_unk(0.0f, 1.0f) < n) {
                for (unsigned int i = 0; i < m80.mCount; i++) {
                    PostFxRainDrop_unk* drop = &m80[i];
                    if (drop->mState == 0) {
                        drop->mState = 1;
                        drop->mX = Rand_unk(0.0f, lbl_8087EEE0->mWidth);
                        drop->mY = Rand_unk(0.0f, lbl_8087EEE0->mHeight);
                        drop->mH = drop->mW = Rand_unk(10.0f, 64.0f);
                        drop->mTime = Rand_unk(0.0f, 1.0f);
                        drop->mAlpha = 1.0f;
                        break;
                    }
                }
            }
            n -= 1.0f;
        } while (n > 0.0f);
        PostFxRainDrop_unk* drop;
        for (unsigned int i = 0; i < m80.mCount; i++) {
            drop = &m80[i];
            if (drop->mState != 0) {
                drop->mTime += 1.0f / 30.0f;
                if (drop->mState == 1) {
                    if (drop->mTime > 0.8f) {
                        drop->mTime = 0.0f;
                        drop->mState = 2;
                    }
                } else if (drop->mState == 2) {
                    drop->mAlpha *= 0.92f;
                    float t = drop->mTime / 5.0f;
                    drop->mY = 2.0f * (t * t) + drop->mY;
                    float w = drop->mW;
                    float nw = 0.98f * w;
                    drop->mW = nw >= 12.0f ? nw : 12.0f;
                    drop->mX -= 0.5f * (drop->mW - w);
                    float nh = 1.04f * drop->mH;
                    drop->mH = nh <= 256.0f ? nh : 256.0f;
                    if (drop->mAlpha < 0.01f) {
                        drop->mState = 0;
                    }
                }
            }
        }
    }
}

} // namespace atn
