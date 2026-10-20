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

// inferred: the same copy with z loaded first. Some functions use this order (0x80135284, and
// about 60 more in the game), so the original had a second copy helper; its name is unknown.
inline void __VecCopyZFirst(register float* dst, register const float* src) {
    asm {
        lfs fp2, 8(src)
        psq_l fp1, 0(src), 0, 0
        psq_st fp1, 0(dst), 0, 0
        stfs fp2, 8(dst)
    }
}

// inferred: 16-byte copy with two paired singles (quaternion copy, 0x80149888)
inline void __Vec4Copy(register float* dst, register const float* src) {
    asm {
        psq_l fp1, 0(src), 0, 0
        psq_l fp2, 8(src), 0, 0
        psq_st fp1, 0(dst), 0, 0
        psq_st fp2, 8(dst), 0, 0
    }
}

// inferred: 2D vector copy with one paired-single load/store (0x803EDCAC)
inline void __Vec2Copy(register float* dst, register const float* src) {
    asm {
        psq_l fp1, 0(src), 0, 0
        psq_st fp1, 0(dst), 0, 0
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

// inferred: 4x4 matrix copy with eight paired-single loads then eight stores (copy of the engine
// camera, 0x80514064 in game/NewCampPartsGmk.cpp)
inline void __Mtx44Copy(register float (*dst)[4], register const float (*src)[4]) {
    asm {
        psq_l fp1, 0x00(src), 0, 0
        psq_l fp2, 0x08(src), 0, 0
        psq_l fp3, 0x10(src), 0, 0
        psq_l fp4, 0x18(src), 0, 0
        psq_l fp5, 0x20(src), 0, 0
        psq_l fp6, 0x28(src), 0, 0
        psq_l fp7, 0x30(src), 0, 0
        psq_l fp8, 0x38(src), 0, 0
        psq_st fp1, 0x00(dst), 0, 0
        psq_st fp2, 0x08(dst), 0, 0
        psq_st fp3, 0x10(dst), 0, 0
        psq_st fp4, 0x18(dst), 0, 0
        psq_st fp5, 0x20(dst), 0, 0
        psq_st fp6, 0x28(dst), 0, 0
        psq_st fp7, 0x30(dst), 0, 0
        psq_st fp8, 0x38(dst), 0, 0
    }
}

namespace atn {

class Matrix3x4;

// inferred: engine quaternion (copied with two paired-single loads/stores, 0x80149888); name invented
struct Quaternion_unk {
    Quaternion_unk() {}
    // inferred: constructor by components (constants of atn/Math_unk.cpp)
    Quaternion_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    Quaternion_unk(const Quaternion_unk& q) {
        __Vec4Copy(&x, &q.x);
    }
    // inferred: assignment with the same copy (weak copy 0x80149888, called out of line by
    // CharaTask::fn_80148CC4 on an already constructed quaternion)
    Quaternion_unk& operator=(const Quaternion_unk& q) {
        __Vec4Copy(&x, &q.x);
        return *this;
    }
    static Quaternion_unk fn_801498F0(const class Vector3& rot); // inferred: from euler angles (weak copy)
    Quaternion_unk& fn_8014989C(const Quaternion_unk& a); // inferred: *this = a * *this
    Quaternion_unk fn_801498D8() const; // inferred: inverse

    float x, y, z, w;
};

// inferred: 2D float vector of the engine (copied with paired singles, 0x803EDCAC); name invented
struct Vector2_unk {
    Vector2_unk() {}
    Vector2_unk(float x_, float y_) : x(x_), y(y_) {}
    Vector2_unk(const Vector2_unk& v) {
        __Vec2Copy(&x, &v.x);
    }
    Vector2_unk& operator=(const Vector2_unk& v) {
        __Vec2Copy(&x, &v.x);
        return *this;
    }
    float x, y;
};

// The engine's 3D vector (the name is in the DOL: boost::_bi::value<atn::Vector3>).
class Vector3 {
public:
    // Weak copies of the members below (kept from the first file that calls them out of line):
    // Vector3() 0x80057A64, Vector3(x, y, z) 0x8000D114, copy 0x8001047C, operator= 0x8000D124,
    // operator+= 0x80012C88, operator- 0x80013338,
    // operator*= 0x8012A190, Set_unk 0x80057A68, IsZero_unk 0x8012A1E0, Apply_unk 0x8012A218,
    // Normalize_unk 0x800F7FF0, ToRotation_unk 0x80011034.
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3(const Vector3& v) {
        __VecCopy(&x, &v.x);
    }
    // inferred: name invented; copy with z loaded first (see __VecCopyZFirst)
    void Assign_unk(const Vector3& v) {
        __VecCopyZFirst(&x, &v.x);
    }
    Vector3& operator=(const Vector3& v) {
        __VecCopy(&x, &v.x);
        return *this;
    }
    // inferred: by-value operators (temporaries kept on the stack, 0x801C9C68)
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }
    // inferred: through a reference, so the other vector is reloaded after the stores (0x80416AA8)
    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    // inferred: same form as operator+=
    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& operator*=(float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
    // inferred: names invented
    void Set_unk(float x_, float y_, float z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    bool IsZero_unk() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
    void Apply_unk(float (*f)(float)) {
        x = f(x);
        y = f(y);
        z = f(z);
    }
    void Normalize_unk();
    // inferred: names invented (weak copies 0x8000D3A4, 0x800F7FD8, 0x801162A0)
    float Length_unk() const;
    float LengthSq_unk() const;
    Vector3 GetNormalized_unk() const;
    Vector3 ToRotation_unk() const; // inferred: x = pitch, y = yaw of the direction
    void fn_80139F60(); // inferred: in-place ToRotation_unk (weak copy 0x80139F60)

    // Out-of-line members (called with bl from many files):
    void fn_80011410(const Matrix3x4& m);              // inferred: *this = m * *this (PSMTXMultVec)
    static Vector3 fn_8013C394(const Vector3& a, const Vector3& b); // inferred: a x b (PSVECCrossProduct)
    Vector3 operator-() const;                          // 0x8013C3B4

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
    Matrix3x4() {}
    // inferred: constructor by elements (identity of atn/Math_unk.cpp)
    Matrix3x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
    }
    // inferred: copies with the paired-single matrix copy (0x803ECC28)
    Matrix3x4(const Matrix3x4& o) {
        __MtxCopy(m, o.m);
    }
    Matrix3x4& operator=(const Matrix3x4& o) {
        __MtxCopy(m, o.m);
        return *this;
    }
    // inferred: copy of a node matrix (weak copy 0x8008CCE8, called out of line by gmk::Cannonball)
    Matrix3x4(const Mtx o) {
        __MtxCopy(m, o);
    }
    void SetIdentity() {
        m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = 0.0f;
        m[0][0] = m[1][1] = m[2][2] = 1.0f;
    }
    // inferred: rotation around y (weak copy 0x8013A13C)
    void fn_8013A13C(float rad) { PSMTXRotRad(m, 'y', rad); }
    void fn_8010F668(); // inferred: clears the translation (weak copy 0x8010F668, game/Bullet.cpp)
    void fn_80144ED4(float rad); // inferred: *this = *this * rot_z(rad)
    void fn_801479E4(const Vector3& v); // inferred: sets the translation
    static Matrix3x4 fn_80147A08(const struct Quaternion_unk& q); // inferred: rotation from a quaternion (PSMTXQuat)
    Matrix3x4& fn_801495E8(const Matrix3x4& a); // inferred: *this = a * *this
    void fn_80149624(float rad); // inferred: *this = *this * rot_y(rad)
    void fn_801496A8(float x, float y, float z); // inferred: *this = *this * rot_z * rot_y * rot_x
    // inferred: translation (weak copy 0x8000D0F8); name invented
    Vector3 GetTrans_unk() const { return Vector3(m[0][3], m[1][3], m[2][3]); }
    // inferred: rotation around y by value (inlined by fn_80139F60); name invented
    static Matrix3x4 RotY_unk(float rad) {
        Matrix3x4 r;
        r.fn_8013A13C(rad);
        return r;
    }
    // inferred: name invented (inlined by fn_80139F60)
    Vector3 GetRow_unk(int i) const { return Vector3(m[i][0], m[i][1], m[i][2]); }
    // inferred: copy without the translation (weak copy 0x8001336C); name invented
    Matrix3x4 GetRotation_unk() const {
        Matrix3x4 r(*this);
        r.m[0][3] = 0.0f;
        r.m[1][3] = 0.0f;
        r.m[2][3] = 0.0f;
        return r;
    }

    Mtx m;
};


} // namespace atn

extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" void PSMTXQuat(Mtx m, const atn::Quaternion_unk* q);
extern "C" void PSQUATMultiply(const atn::Quaternion_unk* p, const atn::Quaternion_unk* q, atn::Quaternion_unk* pq);
extern "C" void PSQUATInverse(const atn::Quaternion_unk* src, atn::Quaternion_unk* inv);
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* axb);
extern "C" void PSMTX44MultVec(const float m[4][4], const atn::Vector3* src, atn::Vector3* dst); // inferred: 0x805F9750
extern "C" double atan2(double y, double x);
extern "C" double fmod(double x, double y);

namespace atn {

inline void Vector3::Normalize_unk() {
    PSVECNormalize(this, this);
}

inline float Vector3::Length_unk() const {
    return PSVECMag(this);
}

inline float Vector3::LengthSq_unk() const {
    return PSVECSquareMag(this);
}

inline Vector3 Vector3::GetNormalized_unk() const {
    Vector3 r(*this);
    r.Normalize_unk();
    return r;
}

// inferred: helpers of the engine (weak copies: Abs_unk 0x80013404, WrapAngle_unk 0x800133B0,
// Lerp_unk 0x800F7260, DegToRad_unk 0x801125F8); names invented
inline float Abs_unk(float x) {
    return __fabs(x);
}
inline float WrapAngle_unk(float a) {
    a = fmod(a, 6.2831855f);
    if (a > 3.1415927f) {
        a -= 6.2831855f;
    }
    if (a < -3.1415927f) {
        a += 6.2831855f;
    }
    return a;
}
inline Vector3 Lerp_unk(const Vector3& a, const Vector3& b, float t) {
    return a + (b - a) * t;
}
inline float DegToRad_unk(float deg) {
    return deg * 0.017453292f;
}

// inferred: 4x4 matrix built from a 3x4 one, row by row (only inlined in fn_80139F60; the rows and
// the translation go through Vector3 temporaries); names invented
struct Matrix44_unk {
    Matrix44_unk() {}
    // inferred: copied with eight paired singles (camera copy, 0x80514064)
    Matrix44_unk(const Matrix44_unk& o) {
        __Mtx44Copy(m, o.m);
    }
    Matrix44_unk(const Matrix3x4& a) {
        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        m[3][3] = 1.0f;
        SetRow_unk(0, a.GetRow_unk(0));
        SetRow_unk(1, a.GetRow_unk(1));
        SetRow_unk(2, a.GetRow_unk(2));
        SetTrans_unk(a.GetTrans_unk());
    }
    void SetRow_unk(int i, const Vector3& v) {
        m[i][0] = v.x;
        m[i][1] = v.y;
        m[i][2] = v.z;
    }
    void SetTrans_unk(const Vector3& v) {
        m[0][3] = v.x;
        m[1][3] = v.y;
        m[2][3] = v.z;
    }
    float m[4][4];
};

// inferred: in-place version of ToRotation_unk (weak copy 0x80139F60, inlined in ToRotation_unk
// 0x80011034).
inline void Vector3::fn_80139F60() {
    Vector3 r;
    if (Abs_unk(z) < 1.1920929e-7f) {
        r.y = x > 0.0f ? 1.5707964f : -1.5707964f;
    } else {
        r.y = atan2(x, z);
    }
    Matrix44_unk m44(Matrix3x4::RotY_unk(-r.y));
    Vector3 v(*this);
    PSMTX44MultVec(m44.m, &v, &v);
    if (Abs_unk(v.z) < 1.1920929e-7f) {
        r.x = -(v.y > 0.0f ? 1.5707964f : -1.5707964f);
    } else {
        r.x = -(float)atan2(v.y, v.z);
    }
    r.z = 0.0f;
    *this = r;
}

inline Vector3 Vector3::ToRotation_unk() const {
    Vector3 r(*this);
    r.fn_80139F60();
    return r;
}

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
