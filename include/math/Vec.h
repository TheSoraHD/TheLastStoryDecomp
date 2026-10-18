#ifndef MATH_VEC_H
#define MATH_VEC_H

// Engine 3D vector. Copies move x/y with one paired-single load/store and z with lfs/stfs; no compiler generates psq_l/psq_st for a plain struct copy, so the original
// header must have used inline asm here, like JGeometry::TVec3 in SMG2 (CLAUDE.md, rule 2).
// inferred: pattern at 0x801C8F74 (ctor copy) and 0x801C9068 (copy to a local and back).

typedef float Mtx[3][4];
typedef float (*MtxPtr)[4];

// The original uses fixed registers (f1/f2 here, f1-f6 for matrices): the compiler then keeps
// its own temporaries out of them in the whole function (0x801C900C uses f7-f11).
inline void __VecCopy(register float* dst, register const float* src) {
    asm {
        psq_l fp1, 0(src), 0, 0
        lfs fp2, 8(src)
        psq_st fp1, 0(dst), 0, 0
        stfs fp2, 8(dst)
    }
}

// inferred: 3x4 matrix copy with six paired-single loads then six stores (0x801C915C)
inline void __MtxCopy(register Mtx dst, register const Mtx src) {
    asm {
        psq_l fp1, 0x00(src), 0, 0
        psq_l fp2, 0x08(src), 0, 0
        psq_l fp3, 0x10(src), 0, 0
        psq_l fp4, 0x18(src), 0, 0
        psq_l fp5, 0x20(src), 0, 0
        psq_l fp6, 0x28(src), 0, 0
        psq_st fp1, 0x00(dst), 0, 0
        psq_st fp2, 0x08(dst), 0, 0
        psq_st fp3, 0x10(dst), 0, 0
        psq_st fp4, 0x18(dst), 0, 0
        psq_st fp5, 0x20(dst), 0, 0
        psq_st fp6, 0x28(dst), 0, 0
    }
}

namespace atn {

class Matrix3x4;

// The engine's 3D vector (the name is in the DOL: boost::_bi::value<atn::Vector3>).
class Vector3 {
public:
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3(const Vector3& v) {
        __VecCopy(&x, &v.x);
    }
    Vector3& operator=(const Vector3& v) {
        __VecCopy(&x, &v.x);
        return *this;
    }
    // inferred: by-value operators (temporaries kept on the stack, 0x801C9C68)
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }

    // Out-of-line members (called with bl from many files):
    Vector3& fn_8000D114(float x_, float y_, float z_); // inferred: sets x, y, z and returns *this
    void fn_80011410(const Matrix3x4& m);              // inferred: *this = m * *this (PSMTXMultVec)

    float x, y, z;
};

} // namespace atn

extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);

namespace atn {

// The engine's 3x4 matrix (the name is in the DOL: atn::Matrix3x4 * in bound callbacks).
// Small members are inlined (the matrix address stays in a register through
// 0x801C9130-0x801C924C); others are called out of line.
class Matrix3x4 {
public:
    void SetIdentity() {
        m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = 0.0f;
        m[0][0] = m[1][1] = m[2][2] = 1.0f;
    }
    void fn_8013A13C(float rad); // inferred: rotation around y (PSMTXRotRad(m, 'y', rad))

    Mtx m;
};

} // namespace atn

// inferred: rotate `m` around `axis` when the angle is not zero (name invented)
inline void __MtxRotate(Mtx m, char axis, float rad) {
    if (rad != 0.0f) {
        Mtx r;
        PSMTXRotRad(r, axis, rad);
        Mtx t;
        PSMTXConcat(m, r, t);
        __MtxCopy(m, t);
    }
}

#endif
