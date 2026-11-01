#include <math.h>
#include <revolution/MTX.h>

void C_MTXFrustum(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
    f32 invrange;

    invrange = 1.0f / (r - l);
    mtx[0][0] = 2.0f * n * invrange;
    mtx[0][1] = 0.0f;
    mtx[0][2] = invrange * (r + l);
    mtx[0][3] = 0.0f;

    invrange = 1.0f / (t - b);
    mtx[1][0] = 0.0f;
    mtx[1][1] = 2.0f * n * invrange;
    mtx[1][2] = invrange * (t + b);
    mtx[1][3] = 0.0f;

    invrange = 1.0f / (f - n);
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = -n * invrange;
    mtx[2][3] = invrange * -(f * n);

    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = -1.0f;
    mtx[3][3] = 0.0f;
}

DECOMP_FORCELITERAL(mtx44_c, 0.5f);

void C_MTXPerspective(Mtx44 mtx, f32 fovy, f32 aspect, f32 n, f32 f) {
    f32 rad, cot;
    f32 invrange;

    rad = M_PI / 180.0f * (0.5f * fovy);
    cot = 1.0f / tanf(rad);

    mtx[0][0] = cot / aspect;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;

    mtx[1][0] = 0.0f;
    mtx[1][1] = cot;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;

    invrange = 1.0f / (f - n);
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = -n * invrange;
    mtx[2][3] = invrange * -(f * n);

    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = -1.0f;
    mtx[3][3] = 0.0f;
}

void C_MTXOrtho(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
    f32 invrange;

    invrange = 1.0f / (r - l);
    mtx[0][0] = 2.0f * invrange;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = invrange * -(r + l);

    invrange = 1.0f / (t - b);
    mtx[1][0] = 0.0f;
    mtx[1][1] = 2.0f * invrange;
    mtx[1][2] = 0.0f;
    mtx[1][3] = invrange * -(t + b);

    invrange = 1.0f / (f - n);
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = -1.0f * invrange;
    mtx[2][3] = -f * invrange;

    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;
}

//unused
void C_MTX44Identity(){
}

//unused
asm void PSMTX44Identity(){
}

//unused
void C_MTX44Copy(){
}

//unused
asm void PSMTX44Copy(Mtx44 mtx1, Mtx44 mtx2){
}

//unused
void C_MTX44Concat(){
}

//unused
// inferred: written from main.dol (0x805F9640)
asm void PSMTX44Concat(register const Mtx44 a, register const Mtx44 b, register Mtx44 ab) {
    // clang-format off
    nofralloc
    psq_l f0, 0x0(r3), 0, 0
    psq_l f2, 0x0(r4), 0, 0
    ps_muls0 f6, f2, f0
    psq_l f3, 0x10(r4), 0, 0
    psq_l f4, 0x20(r4), 0, 0
    ps_madds1 f6, f3, f0, f6
    psq_l f1, 0x8(r3), 0, 0
    psq_l f5, 0x30(r4), 0, 0
    ps_madds0 f6, f4, f1, f6
    psq_l f0, 0x10(r3), 0, 0
    ps_madds1 f6, f5, f1, f6
    psq_l f1, 0x18(r3), 0, 0
    ps_muls0 f8, f2, f0
    ps_madds1 f8, f3, f0, f8
    psq_l f0, 0x20(r3), 0, 0
    ps_madds0 f8, f4, f1, f8
    ps_madds1 f8, f5, f1, f8
    psq_l f1, 0x28(r3), 0, 0
    ps_muls0 f10, f2, f0
    ps_madds1 f10, f3, f0, f10
    psq_l f0, 0x30(r3), 0, 0
    ps_madds0 f10, f4, f1, f10
    ps_madds1 f10, f5, f1, f10
    psq_l f1, 0x38(r3), 0, 0
    ps_muls0 f12, f2, f0
    psq_l f2, 0x8(r4), 0, 0
    ps_madds1 f12, f3, f0, f12
    psq_l f0, 0x0(r3), 0, 0
    ps_madds0 f12, f4, f1, f12
    psq_l f3, 0x18(r4), 0, 0
    ps_madds1 f12, f5, f1, f12
    psq_l f1, 0x8(r3), 0, 0
    ps_muls0 f7, f2, f0
    psq_l f4, 0x28(r4), 0, 0
    ps_madds1 f7, f3, f0, f7
    psq_l f5, 0x38(r4), 0, 0
    ps_madds0 f7, f4, f1, f7
    psq_l f0, 0x10(r3), 0, 0
    ps_madds1 f7, f5, f1, f7
    psq_l f1, 0x18(r3), 0, 0
    ps_muls0 f9, f2, f0
    psq_st f6, 0x0(r5), 0, 0
    ps_madds1 f9, f3, f0, f9
    psq_l f0, 0x20(r3), 0, 0
    ps_madds0 f9, f4, f1, f9
    psq_st f8, 0x10(r5), 0, 0
    ps_madds1 f9, f5, f1, f9
    psq_l f1, 0x28(r3), 0, 0
    ps_muls0 f11, f2, f0
    psq_st f10, 0x20(r5), 0, 0
    ps_madds1 f11, f3, f0, f11
    psq_l f0, 0x30(r3), 0, 0
    ps_madds0 f11, f4, f1, f11
    psq_st f12, 0x30(r5), 0, 0
    ps_madds1 f11, f5, f1, f11
    psq_l f1, 0x38(r3), 0, 0
    ps_muls0 f13, f2, f0
    psq_st f7, 0x8(r5), 0, 0
    ps_madds1 f13, f3, f0, f13
    psq_st f9, 0x18(r5), 0, 0
    ps_madds0 f13, f4, f1, f13
    psq_st f11, 0x28(r5), 0, 0
    ps_madds1 f13, f5, f1, f13
    psq_st f13, 0x38(r5), 0, 0
    blr
    // clang-format on
}

//unused
void C_MTX44Transpose(){
}

//unused
asm void PSMTX44Transpose(){
}

//unused
void C_MTX44Inverse(){
}

//unused
void C_MTX44Trans(){
}

//unused
asm void PSMTX44Trans(){
}

//unused
void C_MTX44TransApply(){
}

//unused
asm void PSMTX44TransApply(){
}

//unused
void C_MTX44Scale(){
}

//unused
asm void PSMTX44Scale(){
}

//unused
void C_MTX44ScaleApply(){
}

//unused
asm void PSMTX44ScaleApply(){
}

//unused
void C_MTX44RotRad(){
}

//unused
asm void PSMTX44RotRad(){
}

//unused
void C_MTX44RotTrig(){
}

//unused
asm void PSMTX44RotTrig(){
}

//unused
void C_MTX44RotAxisRad(){
}

//unused
asm void __PSMTX44RotAxisRadInternal(){
}

//unused
void PSMTX44RotAxisRad(){
}
