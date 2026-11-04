#ifndef ATN_COLORMATRIXPARAM_UNK_H
#define ATN_COLORMATRIXPARAM_UNK_H

// inferred: types of the render settings shared with the event manager (from atn/RenderSetting_unk.h)

#include "math/Vec.h"
#include "atn/Csv.h"

typedef atn::CsvBase<char, '\t', '\r', '"'> RsCsv_unk;

// inferred: float color (plain struct, copied member by member); name invented
struct RsColor_unk {
    RsColor_unk() {}
    RsColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: 4 float vector copied with two paired singles (rows of the color matrix); name invented
struct RsVector4_unk {
    RsVector4_unk() {}
    RsVector4_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    RsVector4_unk(const RsVector4_unk& v) {
        __Vec4Copy(&x, &v.x);
    }
    RsVector4_unk& operator=(const RsVector4_unk& v) {
        __Vec4Copy(&x, &v.x);
        return *this;
    }
    float x, y, z, w;
};

// inferred: color matrix post effect parameters
struct ColorMatrixParam_unk {
    ColorMatrixParam_unk() : mEnabled(0), mPriority(0), mMono(0), mMonoRate(1.0f) {
        mRows[0] = RsVector4_unk(1.0f, 0.0f, 0.0f, 0.0f);
        mRows[1] = RsVector4_unk(0.0f, 1.0f, 0.0f, 0.0f);
        mRows[2] = RsVector4_unk(0.0f, 0.0f, 1.0f, 0.0f);
        mRows[3] = RsVector4_unk(0.0f, 0.0f, 0.0f, 0.0f);
    }
    void fn_800BBFE8(RsCsv_unk* csv);
    void fn_800BC194(void* menu);

    int mEnabled;            // 0x00
    RsVector4_unk mRows[4];  // 0x04: Row 0-3
    int mPriority;           // 0x44
    int mMono;               // 0x48: MonoEnable
    float mMonoRate;         // 0x4C
};

#endif
