#include "atn/Layer3D.h"
#include "math/Vec.h"
#include "std/new.h"

// inferred: globals of unknown classes (automatic names)
struct RenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
struct Camera_unk {
    unsigned char m000[0x18C];
    unsigned char m18C[0x79C - 0x18C]; // inferred: camera
    void* m79C;                        // inferred: copy texture buffer
    unsigned char m7A0[0x954 - 0x7A0];
    int m954;
};
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" unsigned char lbl_8087EFB0; // inferred: z pre-pass active

extern "C" void fn_80071E04(RenderState_unk* rs);
extern "C" void fn_80071D68(RenderState_unk* rs, void* camera);
extern "C" void fn_80071D74(RenderState_unk* rs, const atn::Matrix44_unk& proj);
extern "C" void fn_80076760(RenderState_unk* rs, int arg1, int arg2);
extern "C" void fn_800761A8(RenderState_unk* rs);
extern "C" void C_MTXOrtho(float m[4][4], float t, float b, float l, float r, float n, float f);
extern "C" void GXSetColorUpdate(unsigned char enable);
extern "C" void GXSetTexCopySrc(unsigned short left, unsigned short top, unsigned short wd, unsigned short ht);
extern "C" void GXSetTexCopyDst(unsigned short wd, unsigned short ht, int fmt, unsigned char mipmap);
extern "C" void GXCopyTex(void* dest, unsigned char clear);

namespace atn {

// inferred from the vtable __vt__Q23atn14LayerPrerender (0x80779010)
class LayerPrerender : public IRenderLayer {
public:
    virtual void vf10() {}
    virtual void vf14(void* arg);
    virtual void vf1C();
    virtual void vf20();
};

// inferred from the vtable __vt__Q23atn13LayerZPrePass (0x80778FD0)
class LayerZPrePass : public Layer3D {
public:
    virtual void vf1C();
    virtual void vf20();
};

// inferred from the vtable __vt__Q23atn9Layer2D3D (0x80778F90)
class Layer2D3D : public Layer3D {
public:
    virtual void vf1C();
};

void Layer3D::vf14(void* arg) {
    void* camera = lbl_8087EFB4;
    for (RenderNode_unk* node = (RenderNode_unk*)arg; node != 0; node = node->mNext) {
        RenderObj_unk* obj = node->mObj;
        if (obj != 0) {
            if (node->m04 <= -10000000.0f) {
                new (&m1C[m18]) RenderNode_unk*(node);
                m18++;
            } else {
                obj->vf10(camera);
            }
        }
    }
}

void Layer3D::vf18() {
    void* camera = lbl_8087EFB4;
    for (unsigned int i = 0; i < m18; i++) {
        m1C[i]->mObj->vf10(camera);
    }
}

void Layer3D::vf1C() {
    RenderState_unk* rs = lbl_8087EEE0;
    fn_80071E04(rs);
    fn_80071D68(rs, lbl_8087EFB4->m18C);
    fn_80076760(rs, 6, 0);
    fn_80076760(rs, 8, 1);
    fn_80076760(rs, 9, 2);
    fn_80076760(rs, 10, 1);
    fn_800761A8(rs);
    lbl_8087EFB4->m954 = m14 & 1;
    m18 = 0;
}

void Layer3D::vf20() {
    lbl_8087EFB4->m954 = 0;
}

void Layer2D::vf14(void* arg) {
    void* camera = lbl_8087EFB4;
    for (RenderNode_unk* node = (RenderNode_unk*)arg; node != 0; node = node->mNext) {
        node->mObj->vf10(camera);
    }
}

void Layer2D::vf1C() {
    RenderState_unk* rs = lbl_8087EEE0;
    fn_80071E04(rs);
    // inferred: the projection is built in an inner block and copied out (stack order of the
    // original: the copy above the ortho matrix)
    Matrix44_unk proj;
    {
        Matrix44_unk ortho;
        int h = rs->mHeight;
        int w = rs->mWidth;
        float fh = h;
        float fw = w;
        float zfar = -100.0f; // inferred: locals give the .sdata2 order of the original (-100.0f, 0.0f)
        C_MTXOrtho(ortho.m, 0.0f, fh, 0.0f, fw, 0.0f, zfar);
        __Mtx44Copy(proj.m, ortho.m);
    }
    fn_80071D74(rs, proj);

    fn_80076760(rs, 6, 0);
    fn_80076760(rs, 8, 0);
    fn_80076760(rs, 9, 7);
    fn_80076760(rs, 10, 0);
    fn_80076760(rs, 4, 4);
    fn_80076760(rs, 5, 5);
}

void Layer2D::vf20() {}

void LayerPrerender::vf14(void* arg) {
    void* camera = lbl_8087EFB4;
    for (RenderNode_unk* node = (RenderNode_unk*)arg; node != 0; node = node->mNext) {
        node->mObj->vf18(camera);
    }
}

void LayerPrerender::vf1C() {}

void LayerPrerender::vf20() {}

void LayerZPrePass::vf1C() {
    Layer3D::vf1C();
    lbl_8087EFB0 = 1;
    RenderState_unk* rs = lbl_8087EEE0;
    fn_80076760(rs, 11, 0);
    fn_80076760(rs, 8, 1);
    fn_80076760(rs, 9, 2);
    fn_80076760(rs, 10, 1);
    fn_800761A8(rs);
}

void LayerZPrePass::vf20() {
    lbl_8087EFB0 = 0;
    Layer3D::vf20();
}

// inferred: copies the frame buffer to the camera copy texture (inline, name invented)
inline void CopyTex_unk(int w, int h) {
    GXSetTexCopySrc(0, 0, w, h);
    GXSetTexCopyDst(w, h, 0x13, 0);
    GXCopyTex(lbl_8087EFB4->m79C, 1);
}

void Layer2D3D::vf1C() {
    Layer3D::vf1C();
    GXSetColorUpdate(0);
    CopyTex_unk(lbl_8087EEE0->mWidth, lbl_8087EEE0->mHeight);
    GXSetColorUpdate(1);
}

} // namespace atn
