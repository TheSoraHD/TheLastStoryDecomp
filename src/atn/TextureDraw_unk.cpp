// inferred: texture projection matrices (versions of the SDK C_MTXLight* functions and the texture
// projection used by the engine passes). File name invented.
#include "math/Vec.h"
#include "atn/IRender.h"
#include "atn/Sphere.h"
#include "game/NewCampScene2_unk.h"
#include <string.h>

extern "C" double tan(double x);

// inferred: matrix product by value (as in atn/LayerShadow.cpp)
inline atn::Matrix3x4 MtxMul_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

// inferred: 2D texture matrix: scale (sx, sy), rotation by (c, s) around the center and offset (tx, ty)
extern "C" void fn_800D5C84(float m[3][4], float sx, float sy, float c, float s, float tx, float ty) {
    m[0][0] = sx * s;
    m[0][1] = sx * c;
    m[0][2] = 0.0f;
    m[0][3] = sx * ((0.5f + (-0.5f * c - 0.5f * s)) - tx);
    m[1][0] = -sy * c;
    m[1][1] = sy * s;
    m[1][2] = 0.0f;
    m[1][3] = sy * (ty + ((0.5f * c - 0.5f * s) - 0.5f)) + 1.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
}

// inferred: C_MTXLightPerspective. noinline: the original calls it from fn_800D5F68 with bl
extern "C" __declspec(noinline) void fn_800D5D00(float m[3][4], float fovY, float aspect, float scaleS, float scaleT, float transS,
                            float transT) {
    float cot = 1.0f / (float)tan(0.017453292f * (0.5f * fovY));
    memset(m, 0, 0x30);
    m[0][0] = scaleS * (cot / aspect);
    m[0][1] = 0.0f;
    m[0][2] = -transS;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = cot * scaleT;
    m[1][2] = -transT;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -1.0f;
    m[2][3] = 0.0f;
}

// inferred: C_MTXLightOrtho. noinline: the original calls it from fn_800D5F68 with bl
extern "C" __declspec(noinline) void fn_800D5E18(float m[3][4], float t, float b, float l, float r, float scaleS, float scaleT,
                            float transS, float transT) {
    memset(m, 0, 0x30);
    float tmp = 1.0f / (r - l);
    m[0][0] = 2.0f * tmp * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = scaleS * (tmp * -(r + l)) + transS;
    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = 2.0f * tmp * scaleT;
    m[1][2] = 0.0f;
    m[1][3] = scaleT * (tmp * -(t + b)) + transT;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 0.0f;
    m[2][3] = 1.0f;
}

extern "C" atn::RenderView_unk* lbl_8087EFB4;
extern "C" int fn_800C2418(NewCampScene2_unk* scene);                 // inferred: number of cameras
extern "C" Camera_unk* fn_800C2520(NewCampScene2_unk* scene, int index); // inferred: camera `index`

// inferred: vector helpers of the projection (names invented)
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}
inline atn::Vector3 Neg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}
// inferred: up vector for a view along `dir` (z when dir is almost vertical)
inline void UpFor_unk(const atn::Vector3& dir, atn::Vector3& up) {
    if (atn::Abs_unk(dir.x) < 1e-6f && atn::Abs_unk(dir.z) < 1e-6f) {
        up.x = up.y = 0.0f;
        if (dir.y <= 0.0f) {
            up.z = -1.0f;
        } else {
            up.z = 1.0f;
        }
    } else {
        up.x = up.z = 0.0f;
        up.y = 1.0f;
    }
}
// inferred: texture bias matrix (x, y from [-1, 1] to [0, 1], y flipped)
inline void SetBias_unk(atn::Matrix3x4& m) {
    m.m[0][1] = m.m[0][2] = m.m[1][0] = m.m[1][2] = m.m[2][0] = m.m[2][1] = 0.0f;
    m.m[0][0] = 0.5f;
    m.m[0][3] = 0.5f;
    m.m[1][1] = -0.5f;
    m.m[1][3] = 0.5f;
    m.m[2][2] = 0.0f;
    m.m[2][3] = 1.0f;
}
// inferred: copy with the arguments in the order of PSMTXCopy (src, dst), as in game/CharaTask.cpp
inline void CopyMtx_unk(const Mtx src, Mtx dst) {
    __MtxCopy(dst, src);
}
// inferred: view direction of a camera (not normalized)
inline atn::Vector3 ViewDir_unk(const Camera_unk& c) {
    atn::Vector3 d = c.mTarget - c.mPos;
    return d;
}
// inferred: projection parameters of a camera (names invented)
inline float GetTop_unk(const Camera_unk* c) { return c->mTop; }
inline float GetBottom_unk(const Camera_unk* c) { return c->mBottom; }
inline float GetLeft_unk(const Camera_unk* c) { return c->mLeft; }
inline float GetRight_unk(const Camera_unk* c) { return c->mRight; }
inline float GetAspect_unk(const Camera_unk* c) { return c->mAspect; }
// inferred: row i of a matrix
inline void SetRow_unk(atn::Matrix3x4& m, int i, const atn::Vector3& v) {
    m.m[i][0] = v.x;
    m.m[i][1] = v.y;
    m.m[i][2] = v.z;
}
// inferred: clears the translation
inline void ClearTrans_unk(atn::Matrix3x4& m) {
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = 0.0f;
}

// inferred: texture projection matrix of a pass: 1 screen, 2 light 0 direction, 3 half vector of
// light 0 and the view, 4 camera `camera` of the scene or an orthographic view of `bounds` from above
// NONMATCHING: 98.0% - register allocation only: the original keeps one more saved GPR (camera r26,
// bounds r27, c r28; here the scene shares a register with bounds and c takes r30), loads the view
// pointer into r3 for the matrix copies of cases 2/3 (r6 here; compiled alone, case 2 gives r3), swaps
// r3/r4 in the copy of the second cross product (docs/mwcc-blockers.md, 3) and places the temporary of
// the eye vector of case 3 last in the frame. Found: the bias matrix is cleared with a chained
// assignment, the view/inverse copies use the PSMTXCopy argument order, the cross products are copied
// from a temporary, -dir is built in the statement of the third row, the projection parameters go
// through getters (arguments loaded right to left) and fn_800D5D00/fn_800D5E18 are not inlined.
extern "C" void fn_800D5F68(Mtx out, int mode, int camera, int unused, const atn::Sphere* bounds) {
    atn::Matrix3x4& m = *(atn::Matrix3x4*)out;
    Camera_unk* cam = &lbl_8087EFB4->mCamera;
    Camera_unk* c;
    m.SetIdentity();
    switch (mode) {
    case 1: {
        atn::Matrix3x4 bias;
        SetBias_unk(bias);
        m = bias;
        break;
    }
    case 2: {
        atn::RenderView_unk* v = lbl_8087EFB4;
        atn::Matrix3x4 bias;
        SetBias_unk(bias);
        atn::Matrix3x4 camView;
        CopyMtx_unk(v->mCamera.mView.m, camView.m);
        atn::Matrix3x4 camInv;
        CopyMtx_unk(v->mCamera.mInvView.m, camInv.m);
        atn::Vector3 dir = fn_800C2448(v->mScene, 0)->mDir;
        atn::Vector3 up;
        UpFor_unk(dir, up);
        atn::Vector3 right;
        right = Cross_unk(dir, up);
        PSVECNormalize(&right, &right);
        atn::Vector3 up2;
        up2 = Cross_unk(right, dir);
        atn::Matrix3x4 look;
        look.m[0][3] = look.m[1][3] = look.m[2][3] = 0.0f;
        SetRow_unk(look, 0, right);
        SetRow_unk(look, 1, up2);
        SetRow_unk(look, 2, Neg_unk(dir));
        ClearTrans_unk(camInv);
        m = MtxMul_unk(bias, MtxMul_unk(look, camInv));
        break;
    }
    case 3: {
        atn::Matrix3x4 bias;
        memset(&bias, 0, sizeof(bias));
        bias.m[0][0] = 0.5f;
        bias.m[0][3] = 0.5f;
        bias.m[1][1] = -0.5f;
        bias.m[1][3] = 0.5f;
        bias.m[2][2] = 0.0f;
        bias.m[2][3] = 1.0f;
        atn::RenderView_unk* v = lbl_8087EFB4;
        atn::Matrix3x4 camView;
        CopyMtx_unk(v->mCamera.mView.m, camView.m);
        atn::Matrix3x4 camInv;
        CopyMtx_unk(v->mCamera.mInvView.m, camInv.m);
        atn::Vector3 dir = fn_800C2448(v->mScene, 0)->mDir;
        if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f) {
            dir.y = -1.0f;
        }
        atn::Vector3 eye;
        eye = ViewDir_unk(lbl_8087EFB4->mCamera);
        PSVECNormalize(&eye, &eye);
        atn::Vector3 sum = dir + eye;
        if (sum.x == 0.0f && sum.y == 0.0f && sum.z == 0.0f) {
            sum.z = -1.0f;
        }
        atn::Vector3 h = sum;
        PSVECNormalize(&h, &h);
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        UpFor_unk(sum, up);
        atn::Vector3 right;
        PSVECCrossProduct(&h, &up, &right);
        PSVECNormalize(&right, &right);
        atn::Vector3 up2;
        PSVECCrossProduct(&right, &h, &up2);
        PSVECNormalize(&up2, &up2);
        atn::Matrix3x4 look;
        look.m[0][3] = look.m[1][3] = look.m[2][3] = 0.0f;
        SetRow_unk(look, 0, right);
        SetRow_unk(look, 1, up2);
        SetRow_unk(look, 2, h);
        ClearTrans_unk(camInv);
        m = MtxMul_unk(bias, MtxMul_unk(look, camInv));
        break;
    }
    case 4: {
        c = cam;
        if (camera > 0) {
            if (bounds != 0) {
                static Camera_unk sCamera(0, 1);
                c = &sCamera;
                c->mPos = bounds->mCenter + atn::Vector3(0.0f, 2.0f * bounds->mRadius, 0.0f);
                c->mTarget = bounds->mCenter;
                c->mUp.x = 0.0f;
                c->mUp.y = 0.0f;
                c->mUp.z = 1.0f;
                c->mTop = bounds->mCenter.y + 2.0f * bounds->mRadius;
                c->mBottom = bounds->mCenter.y - 2.0f * bounds->mRadius;
                c->mLeft = bounds->mCenter.x + 2.0f * bounds->mRadius;
                c->mRight = bounds->mCenter.x - 2.0f * bounds->mRadius;
                c->fn_8004B378();
            } else {
                NewCampScene2_unk* scene = lbl_8087EFB4->mScene;
                if (camera < fn_800C2418(scene)) {
                    c = fn_800C2520(scene, camera);
                }
            }
        }
        atn::Matrix3x4 camInv = cam->mInvView;
        atn::Matrix3x4 camView = c->mView;
        atn::Matrix3x4 proj;
        if (c->mProjType == 1) {
            fn_800D5E18(proj.m, GetTop_unk(c), GetBottom_unk(c), GetLeft_unk(c), GetRight_unk(c), 0.5f, -0.5f, 0.5f, 0.5f);
        } else {
            fn_800D5D00(proj.m, 180.0f * c->GetFovy_unk() / 3.1415927f, GetAspect_unk(c), 0.5f, -0.5f, 0.5f, 0.5f);
        }
        m = MtxMul_unk(proj, MtxMul_unk(camView, camInv));
        break;
    }
    }
}
