// inferred file name (splits-auto): cubic paths of the events (Event/Path), built from the "pos_x",
// "pos_y", "pos_z" and "rot_y" keys of their points.
#include "Event/Node.h"
#include "math/Vec.h"
#include "std/vector.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: cubic of one segment and axis: a0 + a1 u + a2 u^2 + a3 u^3
struct PathCubic_unk {
    float Eval_unk(float u) const { return u * (u * (m3 * u + m2) + m1) + m0; }
    float m0;
    float m1;
    float m2;
    float m3;
};

// inferred: std::vector layout (data, size, capacity)
template <class T>
struct PathVec_unk {
    T* Data_unk() { return mData; }
    T& operator[](int i) { return mData[i]; }
    void Clear_unk() {
        unsigned int n = mSize;
        unsigned int& size = mSize;
        size -= n;
    }
    T* mData;
    unsigned int mSize;
    unsigned int mCap;
};

// inferred: inlined vector(n, value) constructor (the only place that defines it)
template <class T>
std::vector<T>::vector(size_type n, const T& value) {
    if (n != 0) {
        if (n > max_size()) {
            __msl_error("vector length error");
        }
        this->mData = __allocate(n);
        this->mCap = n;
        T* p = this->mData + this->mSize;
        for (; n != 0; n--, p++) {
            new (p) T(value);
            this->mSize++;
        }
    }
}

struct PathOwner_unk;
struct PathData_unk;
extern "C" void fn_8054875C(PathData_unk* path);
// inferred: local point of the path to world (Event/Path)
extern "C" atn::Vector3 fn_80541BD8(PathOwner_unk* owner, const atn::Vector3& pos);

// inferred: point of a path (position and rotation around y)
struct PathPoint_unk {
    atn::Vector3 mPos;
    float mRotY;
};

struct PathSpline_unk;
// inferred: helpers of the spline (another unit; C names)
extern "C" void fn_8032B314(PathVec_unk<PathCubic_unk>* v, int n); // inferred: resize
extern "C" void fn_8032B6F8(PathVec_unk<float>* v, int n);         // inferred: vector<float>(n)
extern "C" void fn_8000D844(PathVec_unk<float>* v, int flag);      // inferred: ~vector<float>
extern "C" void fn_8032B7F0(PathSpline_unk* spline, const float* values, PathCubic_unk* cubics); // inferred: solve
extern "C" void fn_8032BE88(PathVec_unk<float>* v, int n);         // inferred: resize
extern "C" float fn_8032C1EC(PathSpline_unk* spline, int i);       // inferred: length of a segment

// inferred: natural cubic spline through the points (names invented)
struct PathSpline_unk {
    void Build_unk(const atn::Vector3* pts, int n) {
        mNumSegments = n;
        fn_8032B314(&mX, n - 1);
        fn_8032B314(&mY, n - 1);
        fn_8032B314(&mZ, n - 1);
        PathVec_unk<float> zs;
        PathVec_unk<float> ys;
        PathVec_unk<float> xs;
        fn_8032B6F8(&xs, n);
        fn_8032B6F8(&ys, n);
        fn_8032B6F8(&zs, n);
        for (int i = 0; i < n; i++) {
            xs.mData[i] = pts[i].x;
            ys.mData[i] = pts[i].y;
            zs.mData[i] = pts[i].z;
        }
        fn_8032B7F0(this, xs.Data_unk(), mX.Data_unk());
        fn_8032B7F0(this, ys.Data_unk(), mY.Data_unk());
        fn_8032B7F0(this, zs.Data_unk(), mZ.Data_unk());
        mStart = pts[0];
        mEnd = pts[n - 1];
        mLength = 0.0f;
        fn_8032BE88(&mLengths, n - 1);
        for (int i = 0; i < n - 1; i++) {
            mLengths[i] = fn_8032C1EC(this, i);
            mLength += mLengths[i];
        }
        fn_8000D844(&zs, -1);
        fn_8000D844(&ys, -1);
        fn_8000D844(&xs, -1);
    }
    void Clear_unk() {
        mNumSegments = 0;
        mLength = 0.0f;
        mStart = lbl_807C7030;
        mEnd = lbl_807C7030;
        mX.Clear_unk();
        mY.Clear_unk();
        mZ.Clear_unk();
        mLengths.Clear_unk();
    }

    int mNumSegments;                    // 0x00
    float mLength;                       // 0x04
    atn::Vector3 mStart;                 // 0x08
    atn::Vector3 mEnd;                   // 0x14
    PathVec_unk<PathCubic_unk> mX;       // 0x20
    PathVec_unk<PathCubic_unk> mY;       // 0x2C
    PathVec_unk<PathCubic_unk> mZ;       // 0x38
    PathVec_unk<float> mLengths;         // 0x44
};

// inferred: path data (names invented)
struct PathData_unk {
    // inferred: point of segment `i` at `u`
    atn::Vector3 GetPoint_unk(int i, float u) const {
        if (i < 0) {
            return mSpline.mStart;
        }
        if (i >= mSpline.mNumSegments - 1) {
            return mSpline.mEnd;
        }
        if (mSpline.mNumSegments < 2) {
            return lbl_807C7030;
        }
        atn::Vector3 p;
        p.x = mSpline.mX.mData[i].Eval_unk(u);
        p.y = mSpline.mY.mData[i].Eval_unk(u);
        p.z = mSpline.mZ.mData[i].Eval_unk(u);
        return p;
    }
    unsigned char m00[0x04];
    PathOwner_unk* mOwner;               // 0x04
    unsigned char m08[0x28 - 0x08];
    PathVec_unk<PathPoint_unk> mPoints;  // 0x28
    PathSpline_unk mSpline;              // 0x34
};

// inferred: removes the point `point` and rebuilds the path
// NONMATCHING: 95.7% - same differences as fn_8054875C (inlined here) plus the register of the
// erase loop.
extern "C" void fn_80547EA0(PathData_unk* path, PathPoint_unk* point) {
    int index = -1;
    for (int i = 0; i < (int)path->mPoints.mSize; i++) {
        if (&path->mPoints.mData[i] == point) {
            index = i;
            break;
        }
    }
    PathPoint_unk* dst = &path->mPoints.mData[index];
    PathPoint_unk* end = &path->mPoints.mData[path->mPoints.mSize];
    if (end - dst - 1 != 0) {
        for (PathPoint_unk* src = dst + 1; src < end; src++, dst++) {
            *dst = *src;
        }
    }
    path->mPoints.mSize = path->mPoints.mSize - 1;
    fn_8054875C(path);
}

// inferred: local position at `t` (0..1) along the path (name invented)
inline atn::Vector3 GetLocal_unk(PathData_unk* path, float t) {
    if (t <= 0.0f) {
        return path->mSpline.mStart;
    }
    if (t >= 1.0f) {
        return path->mSpline.mEnd;
    }
    float d = path->mSpline.mLength * t;
    for (int i = 0; i < path->mSpline.mNumSegments - 1; i++) {
        float len = path->mSpline.mLengths.mData[i];
        if (d <= len) {
            float u = 0.0f == d ? 0.0f : d / len;
            return path->GetPoint_unk(i, u);
        }
        d -= len;
    }
    return path->mSpline.mStart;
}

// inferred: world position at `t` (0..1) along the path
// NONMATCHING: 89.9% - order of the z store in the Vector3 copies (paired-single helper of
// math/Vec.h) and the reload of the segment count.
extern "C" atn::Vector3 fn_8054856C(PathData_unk* path, float t) {
    if (path->mPoints.mSize < 2) {
        return lbl_807C7030;
    }
    return fn_80541BD8(path->mOwner, GetLocal_unk(path, t));
}

// inferred: rebuilds the path from its points (also inlined by fn_80547EA0)
// NONMATCHING: 97.8% - the original rebuilds the address of the default Vector3 of the
// vector(n, value) constructor after the allocation (here kept in r29) and saves r27-r31.
extern "C" void fn_8054875C(PathData_unk* path) {
        if (path->mPoints.mSize >= 2) {
            std::vector<atn::Vector3> pts(path->mPoints.mSize, atn::Vector3());
            for (int i = 0; i < (int)path->mPoints.mSize; i++) {
                pts[i] = path->mPoints.mData[i].mPos;
            }
            path->mSpline.Build_unk(&pts[0], pts.size());
        } else {
            path->mSpline.Clear_unk();
        }
    }

// inferred: keys of a path point
static Event::ParamDef_unk sParamDefs_unk[] = {
    {"pos_x", fn_800DC880("pos_x"), 4, 0.0f},
    {"pos_y", fn_800DC880("pos_y"), 5, 0.0f},
    {"pos_z", fn_800DC880("pos_z"), 6, 0.0f},
    {"rot_y", fn_800DC880("rot_y"), 3, 0.0f},
};
