// inferred: texture projection matrices (versions of the SDK C_MTXLight* functions and the texture
// projection used by the engine passes). File name invented.
#include "math/Vec.h"
#include <string.h>

extern "C" double tan(double x);

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

// inferred: C_MTXLightPerspective
extern "C" void fn_800D5D00(float m[3][4], float fovY, float aspect, float scaleS, float scaleT, float transS,
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

// inferred: C_MTXLightOrtho
extern "C" void fn_800D5E18(float m[3][4], float t, float b, float l, float r, float scaleS, float scaleT,
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

// fn_800D5F68 (0x930 bytes, texture projection of the light/shadow passes with a function-level static
// in .bss 0x807C75F4 and its guard in .sbss 0x8087EFF8): not written.
