// inferred file name (splits-auto): cubic paths of the events (Event/Path), built from the "pos_x",
// "pos_y", "pos_z" and "rot_y" keys of their points.
#include "Event/Node.h"
#include "Event/Sequence.h"
#include "Event/PathSpline.h"
#include "Event/Path.h"
#include "math/Vec.h"
#include "std/vector.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: cubic of one segment and axis (the class of Event::PathSpline_unk: the weak vector code of the
// spline is shared with game/Em403WarLion.cpp)
using Event::PathCubic_unk;
// inferred: value of the cubic at `u` (this file evaluates it from the highest power)
inline float EvalCubic_unk(const PathCubic_unk& c, float u) { return u * (u * (c.m3 * u + c.m2) + c.m1) + c.m0; }
// inferred: clear of a vector of elements without destructor (as PathVec_unk::Clear_unk)
template <class T>
inline void ClearVec_unk(std::vector<T>& v) {
    unsigned int n = v.mSize;
    unsigned int& size = v.mSize;
    size -= n;
}

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

extern "C" void fn_8054875C(Event::Path* path);
using Event::PathPoint_unk;
using Event::PathSpline_unk;
// inferred: helpers of the spline (another unit; C names)
extern "C" void fn_8032B7F0(PathSpline_unk* spline, const float* values, PathCubic_unk* cubics); // inferred: solve
extern "C" float fn_8032C1EC(PathSpline_unk* spline, int i);       // inferred: length of a segment

// inferred: natural cubic spline through the points (declared in Event/PathSpline.h)
inline void Event::PathSpline_unk::Build_unk(const atn::Vector3* pts, int n) {
    mNumSegments = n;
    mX.resize(n - 1);
    mY.resize(n - 1);
    mZ.resize(n - 1);
    Event::PathLengths_unk xs(n);
    Event::PathLengths_unk ys(n);
    Event::PathLengths_unk zs(n);
    for (int i = 0; i < n; i++) {
        xs.begin()[i] = pts[i].x;
        ys.begin()[i] = pts[i].y;
        zs.begin()[i] = pts[i].z;
    }
    fn_8032B7F0(this, xs.begin(), mX.begin());
    fn_8032B7F0(this, ys.begin(), mY.begin());
    fn_8032B7F0(this, zs.begin(), mZ.begin());
    mStart = pts[0];
    mEnd = pts[n - 1];
    mLength = 0.0f;
    mLengths.resize(n - 1);
    for (int i = 0; i < n - 1; i++) {
        mLengths[i] = fn_8032C1EC(this, i);
        mLength += mLengths[i];
    }
}
inline void Event::PathSpline_unk::Clear_unk() {
    mNumSegments = 0;
    mLength = 0.0f;
    mStart = lbl_807C7030;
    mEnd = lbl_807C7030;
    ClearVec_unk(mX);
    ClearVec_unk(mY);
    ClearVec_unk(mZ);
    ClearVec_unk(mLengths);
}

// inferred: point of segment `i` at `u` of the path
inline atn::Vector3 GetPoint_unk(const Event::Path* path, int i, float u) {
    if (i < 0) {
        return path->mSpline.mStart;
    }
    if (i >= path->mSpline.mNumSegments - 1) {
        return path->mSpline.mEnd;
    }
    if (path->mSpline.mNumSegments < 2) {
        return lbl_807C7030;
    }
    atn::Vector3 p;
    p.x = EvalCubic_unk(path->mSpline.mX.mData[i], u);
    p.y = EvalCubic_unk(path->mSpline.mY.mData[i], u);
    p.z = EvalCubic_unk(path->mSpline.mZ.mData[i], u);
    return p;
}

// inferred: removes the point `point` and rebuilds the path
// NONMATCHING: 95.7% - same differences as fn_8054875C (inlined here) plus the register of the
// erase loop.
extern "C" void fn_80547EA0(Event::Path* path, PathPoint_unk* point) {
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
inline atn::Vector3 GetLocal_unk(Event::Path* path, float t) {
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
            return GetPoint_unk(path, i, u);
        }
        d -= len;
    }
    return path->mSpline.mStart;
}

// inferred: world position at `t` (0..1) along the path
// NONMATCHING: 89.9% - order of the z store in the Vector3 copies (paired-single helper of
// math/Vec.h) and the reload of the segment count.
extern "C" atn::Vector3 fn_8054856C(Event::Path* path, float t) {
    if (path->mPoints.mSize < 2) {
        return lbl_807C7030;
    }
    return path->m04->fn_80541BD8(GetLocal_unk(path, t));
}

// inferred: rebuilds the path from its points (also inlined by fn_80547EA0)
// NONMATCHING: 97.8% - the original rebuilds the address of the default Vector3 of the
// vector(n, value) constructor after the allocation (here kept in r29) and saves r27-r31.
extern "C" void fn_8054875C(Event::Path* path) {
        if (path->mPoints.mSize >= 2) {
            std::vector<atn::Vector3> pts(path->mPoints.mSize, atn::Vector3());
            for (int i = 0; i < (int)path->mPoints.mSize; i++) {
                pts[i] = path->mPoints.mData[i].mPos;
            }
            PathSpline_unk* spline = &path->mSpline;
            spline->Build_unk(&pts[0], pts.size());
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
