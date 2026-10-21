// atn/Camera_unk.cpp: engine camera (view, projection and frustum) and the free debug camera
// moved with the pad (file name invented; probably atn_camera.cpp).
#include "atn/Camera_unk.h"

extern "C" void C_MTXLookAt(Mtx m, const atn::Vector3* camPos, const atn::Vector3* camUp, const atn::Vector3* target);
extern "C" void C_MTXPerspective(float m[4][4], float fovy, float aspect, float n, float f);
extern "C" void C_MTXOrtho(float m[4][4], float t, float b, float l, float r, float n, float f);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" double cos(double x);
extern "C" double sin(double x);

extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A58D0(void* pad, int port);
extern "C" int fn_800A58F8(void* pad, int port);
extern "C" int fn_800A55D4(void* pad, int port, int button);           // inferred: button held
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg);   // inferred: stick axis
extern "C" float fn_800A5648(void* pad, int port, int button);          // inferred: analog button

using namespace atn;

// inferred: same helper as in game/CharaTask.cpp
inline float LengthSq_unk(const Vector3& v) {
    return fn_805F9920(&v);
}

// inferred: view matrix (names invented)
inline Matrix3x4 LookAt_unk(const Vector3& pos, const Vector3& up, const Vector3& target) {
    Matrix3x4 m;
    C_MTXLookAt(m.m, &pos, &up, &target);
    return m;
}

// inferred: projection matrices (names invented)
inline void Perspective_unk(Matrix44_unk& out, float fovy, float aspect, float n, float f) {
    Matrix44_unk m;
    C_MTXPerspective(m.m, fovy, aspect, n, f);
    out = m;
}
inline void Ortho_unk(Matrix44_unk& out, float t, float b, float l, float r, float n, float f) {
    Matrix44_unk m;
    C_MTXOrtho(m.m, t, b, l, r, n, f);
    out = m;
}

// inferred: *this = *this * rot(axis, rad) (as in game/CharaTask.cpp; name invented)
inline void Rotate_unk(Matrix3x4& mtx, char axis, float rad) {
    Mtx r;
    Mtx t;
    PSMTXRotRad(r, axis, rad);
    PSMTXConcat(mtx.m, r, t);
    __MtxCopy(mtx.m, t);
}

// inferred: rotation rot_z * rot_y * rot_x (names invented)
inline Matrix3x4 RotZYX_unk(const Vector3& rot) {
    Matrix3x4 m;
    Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != rot.z) {
        Rotate_unk(*p, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        Rotate_unk(*p, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        Rotate_unk(*p, 'x', rot.x);
    }
    return m;
}

// inferred: rotation rot_z * rot_x * rot_y (name invented)
// (the work matrices are declared at the top in this order: their stack slots match the original)
inline void RotZXY_unk(Matrix3x4* p, float x, float y, float z) {
    Mtx ry;
    Mtx ty;
    Mtx rx;
    Mtx tx;
    Mtx rz;
    Mtx tz;
    Vector3 rot(x, y, z);
    p->SetIdentity();
    if (0.0f != rot.z) {
        PSMTXRotRad(rz, 'z', rot.z);
        PSMTXConcat(p->m, rz, tz);
        __MtxCopy(p->m, tz);
    }
    if (0.0f != rot.x) {
        PSMTXRotRad(rx, 'x', rot.x);
        PSMTXConcat(p->m, rx, tx);
        __MtxCopy(p->m, tx);
    }
    if (0.0f != rot.y) {
        PSMTXRotRad(ry, 'y', rot.y);
        PSMTXConcat(p->m, ry, ty);
        __MtxCopy(p->m, ty);
    }
}

// inferred: sets the rows and the translation of a view matrix (name invented)
inline void SetView_unk(Matrix3x4& m, const Vector3& x, const Vector3& y, const Vector3& z, float tx, float ty, float tz) {
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    m.m[0][0] = x.x;
    m.m[0][1] = x.y;
    m.m[0][2] = x.z;
    m.m[1][0] = y.x;
    m.m[1][1] = y.y;
    m.m[1][2] = y.z;
    m.m[2][0] = z.x;
    m.m[2][1] = z.y;
    m.m[2][2] = z.z;
}

// inferred: name invented
inline float RadToDeg_unk(float rad) {
    return 57.295776f * rad;
}

// inferred: m * v by value (name invented)
inline Vector3 MultVec_unk(const Mtx m, const Vector3& v) {
    Vector3 r(v);
    PSMTXMultVec(m, &r, &r);
    return r;
}

// inferred: name invented
inline Vector3 Direction_unk(const Vector3& from, const Vector3& to) {
    return (to - from).GetNormalized_unk();
}

// inferred: name invented
inline Vector3 Neg_unk(const Vector3& v) {
    return Vector3(-v.x, -v.y, -v.z);
}

// inferred: float wrappers of the math library (names invented)
inline float CosF_unk(float x) {
    return cos(x);
}
inline float SinF_unk(float x) {
    return sin(x);
}

// inferred: cross product by value (name invented)
inline Vector3 Cross_unk(const Vector3& a, const Vector3& b) {
    Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

Camera_unk::Camera_unk(int type, int projType)
    : mType(type), mProjType(projType), mPos(1.0f, 1.0f, 1.0f), mTarget(0.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f),
      mRot(0.7853982f, 0.7853982f, 0.0f), mRoll(0.0f), mTop(1.0f), mBottom(-1.0f), mLeft(-1.0f), mRight(1.0f),
      mDist(1.0f), mFovy(0.5235988f), mAspect(1.0f), mNear(1.0f), mFar(10000.0f) {
    fn_8004B378();
}

Camera_unk::~Camera_unk() {}

// NONMATCHING: 99.97% - r3/r4 swapped in the copy of the cross product (__VecCopy, known blocker)
void Camera_unk::fn_8004B378() {
    switch (mType) {
    case 0: {
        if (Abs_unk(LengthSq_unk(mPos - mTarget)) < 1.1920929e-7f) {
            mTarget.z += 0.1f;
        }
        mView = LookAt_unk(mPos, mUp, mTarget);
        mRot = Neg_unk(Direction_unk(mPos, mTarget));
        mRot.fn_80139F60();
        mDist = (mTarget - mPos).Length_unk();
        break;
    }
    case 2: {
        Vector3 d = mPos - mTarget;
        mView.SetIdentity();
        if (d.x == 0.0f && d.z == 0.0f) {
            Vector3 x(1.0f, 0.0f, 0.0f);
            Vector3 t(-mPos.x, -mPos.z, -mPos.y);
            Vector3 z(0.0f, 0.0f, 1.0f);
            Vector3 y(0.0f, 1.0f, 0.0f);
            if (d.y <= 0.0f) {
                y.y *= -1.0f;
                t.y *= -1.0f;
            } else {
                z.z *= -1.0f;
                t.z *= -1.0f;
            }
            SetView_unk(mView, x, z, d, t.x, t.y, t.z);
        } else {
            Vector3 side(d.z, 0.0f, -d.x);
            PSVECNormalize(&d, &d);
            PSVECNormalize(&side, &side);
            Vector3 u;
            u = Cross_unk(d, side);
            float cs = CosF_unk(mRoll);
            float sn = SinF_unk(mRoll);
            Vector3 r;
            r.x = sn * u.x + cs * side.x;
            r.y = sn * u.y;
            r.z = sn * u.z + cs * side.z;
            Vector3 v;
            v.x = cs * u.x - sn * side.x;
            v.y = cs * u.y;
            v.z = cs * u.z - sn * side.z;
            float tx;
            float ty;
            float tz;
            tz = -PSVECDotProduct(&mPos, &d);
            ty = -PSVECDotProduct(&mPos, &v);
            tx = -PSVECDotProduct(&mPos, &r);
            SetView_unk(mView, r, v, d, tx, ty, tz);
        }
        break;
    }
    case 1: {
        Matrix3x4 m;
        Matrix3x4 t;
        PSMTXTrans(t.m, mPos.x, mPos.y, mPos.z);
        mView = t;
        Matrix3x4 rot = RotZYX_unk(mRot);
        PSMTXConcat(rot.m, mView.m, mView.m);
        float rz = mRot.z;
        mUp.Set_unk(0.0f, 1.0f, 0.0f);
        PSMTXRotRad(m.m, 'z', rz);
        PSMTXMultVec(m.m, &mUp, &mUp);
        mTarget.Set_unk(0.0f, 0.0f, mDist);
        Matrix3x4 rot2;
        Matrix3x4* p = &rot2;
        RotZXY_unk(p, mRot.x, mRot.y, 0.0f);
        m = *p;
        PSMTXMultVec(m.m, &mTarget, &mTarget);
        break;
    }
    }

    switch (mProjType) {
    case 0:
        Perspective_unk(mProj, RadToDeg_unk(mFovy), mAspect, mNear, mFar);
        break;
    case 1:
        Ortho_unk(mProj, mTop, mBottom, mLeft, mRight, mNear, mFar);
        break;
    }

    Matrix3x4 inv(mView);
    PSMTXInverse(inv.m, inv.m);
    mInvView = inv;

    switch (mProjType) {
    case 0:
        fn_8004F50C(&mFrustum, mView.m, mFovy, mAspect, mNear, mFar);
        break;
    case 1:
        fn_8004F5B8(&mFrustum, mView.m, 1, mTop, mBottom, mLeft, mRight, mNear, mFar, 1.0f);
        break;
    }
}

// NONMATCHING: 99.94% - r3/r4 swapped in the two vector copies after the rotations
bool Camera_unk::fn_8004BF64(float speed, float rotSpeed) {
    Vector3 pos(mPos);
    Vector3 target(mTarget);
    bool moved = false;
    Vector3* up = &mUp;
    Vector3 fwd = Direction_unk(mPos, mTarget);
    Vector3 side;
    PSVECCrossProduct(&fwd, up, &side);
    if (fn_805F9920(&side) < 0.01f) {
        side.Set_unk(0.0f, 0.0f, 1.0f);
    }

    int button1 = 0x1F;
    int button2 = 0x20;
    if (!fn_800A58D0(lbl_8087EF70, 0)) {
        button1 = 0xC;
        button2 = 0xD;
    } else if (fn_800A58F8(lbl_8087EF70, 0)) {
        button1 = 0xA;
        button2 = 0xB;
    }

    if (fn_800A55D4(lbl_8087EF70, 0, button1) && fn_800A55D4(lbl_8087EF70, 0, button2)) {
        float f = fn_800A56A8(lbl_8087EF70, 0, 1, 1);
        if (Abs_unk(f) > 0.1f) {
            moved = true;
            pos += fwd * f * speed;
        }
    } else {
        float f = fn_800A56A8(lbl_8087EF70, 0, 0, 1);
        if (Abs_unk(f) > 0.1f) {
            moved = true;
            pos += side * f * speed;
            target += side * f * speed;
        }
        f = fn_800A56A8(lbl_8087EF70, 0, 1, 1);
        if (Abs_unk(f) > 0.1f) {
            moved = true;
            pos += fwd * f * speed;
            target += fwd * f * speed;
        }
        float a = fn_800A5648(lbl_8087EF70, 0, button1);
        float h = a - fn_800A5648(lbl_8087EF70, 0, button2);
        if (Abs_unk(h) > 0.1f) {
            moved = true;
            pos.y += speed * h;
            target.y += speed * h;
        }
    }

    Vector3 rot = fwd.ToRotation_unk();

    float f = fn_800A56A8(lbl_8087EF70, 0, 2, 1);
    if (Abs_unk(f) > 0.1f) {
        Mtx m;
        PSMTXRotAxisRad(m, up, 0.05f * -f * rotSpeed / 3.0f);
        target = MultVec_unk(m, target - pos) + pos;
        moved = true;
    }
    f = fn_800A56A8(lbl_8087EF70, 0, 3, 1);
    if (Abs_unk(f) > 0.1f) {
        Mtx m;
        PSMTXRotAxisRad(m, &side, 0.05f * f * rotSpeed / 3.0f);
        target = MultVec_unk(m, target - pos) + pos;
        moved = true;
    }

    Vector3 flat;
    flat.z = target.z - pos.z;
    flat.x = target.x - pos.x;
    flat.y = 0.0f;
    if (fn_805F9920(&flat) < 0.01f) {
        flat.Set_unk(0.0f, 0.0f, 1.0f);
    } else {
        flat.Normalize_unk();
    }
    Vector3 move(0.0f, 0.0f, 0.0f);
    if (fn_800A55D4(lbl_8087EF70, 0, 0)) {
        move += flat * speed;
    }
    if (fn_800A55D4(lbl_8087EF70, 0, 1)) {
        move -= flat * speed;
    }
    if (fn_800A55D4(lbl_8087EF70, 0, 3)) {
        move += Cross_unk(flat, Vector3(0.0f, 1.0f, 0.0f)) * speed;
    }
    if (fn_800A55D4(lbl_8087EF70, 0, 2)) {
        move -= Cross_unk(flat, Vector3(0.0f, 1.0f, 0.0f)) * speed;
    }
    pos += move;
    target += move;
    mPos = pos;
    mTarget = target;
    fn_8004B378();
    return moved;
}
