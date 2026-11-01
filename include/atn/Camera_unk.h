#ifndef ATN_CAMERA_UNK_H
#define ATN_CAMERA_UNK_H

#include "math/Vec.h"
#include "atn/Sphere.h"

// inferred: plane (normal, distance); names invented
struct Plane_unk {
    atn::Vector3 mNormal;
    float mDist;
};

// inferred: view frustum of the engine camera (0xF4 bytes at Camera_unk+0x100, atn/Frustum_unk.cpp).
// Names invented.
struct Frustum_unk {
    atn::Matrix3x4 mView;  // 0x00
    int mOrtho;            // 0x30
    float mNear;           // 0x34: -near (view space z)
    float mFar;            // 0x38: -far
    Plane_unk mSides[4];   // 0x3C: view space side planes
    atn::Vector3 mMin;     // 0x7C: world space bounds of the corners
    atn::Vector3 mMax;     // 0x88
    Plane_unk mPlanes[6];  // 0x94: world space planes
};

// inferred: perspective frustum (fovy in radians)
extern "C" void fn_8004F50C(Frustum_unk* fr, const Mtx view, float fovy, float aspect, float n, float f);
// inferred: frustum from the projection bounds (ortho = 1 for an orthographic camera; scale = far / near)
extern "C" void fn_8004F5B8(Frustum_unk* fr, const Mtx view, int ortho, float t, float b, float l, float r, float n,
                            float f, float scale);
extern "C" int fn_8004FF58(const Frustum_unk* fr, const atn::Sphere* sphere); // inferred: sphere test
extern "C" int fn_800500FC(const Frustum_unk* fr, const atn::Vector3* box); // inferred: box: 0 out, 1 partly, 2 in
extern "C" int fn_800502A8(const Frustum_unk* fr, const atn::Vector3* box); // inferred: box: 0 out, 1 in
extern "C" int fn_80050420(const Frustum_unk* fr, const atn::Vector3* seg, atn::Vector3* out); // inferred: clips a segment

namespace atn {

// engine camera atn::Camera (0x1F4 bytes; name from SetPosition__Q23atn6Camera of the RSO; the
// other member names are invented). Camera_unk is kept as a typedef for the existing code.
struct Camera {
    Camera(int type = 0, int projType = 0);
    ~Camera();
    void fn_8004B378(); // inferred: updates the view, projection and frustum
    // inferred: free debug camera moved with the pad; true when it moved
    bool fn_8004BF64(float speed, float rotSpeed);
    // inferred: accessors (weak copies 0x80113CCC, 0x80112958, 0x80112960, 0x80114AA0, 0x80116E6C and
    // 0x80116BA4 in Event/Path.cpp; the one at 0x80114AA0 is SetPosition__Q23atn6Camera in the RSO,
    // so this class is atn::Camera)
    const Vector3* GetPosition_unk() const { return &mPos; }
    const Vector3* GetTarget_unk() const { return &mTarget; }
    void SetTarget_unk(const Vector3& v) { mTarget = v; }
    void SetPosition(const Vector3& v) { mPos = v; }
    const Vector3* GetRot_unk() const { return &mRot; }
    float GetFovy_unk() const { return mFovy; }

    int mType;              // 0x00: 0 = look at, 1 = position and angles, 2 = look at with roll
    int mProjType;          // 0x04: 0 = perspective, 1 = orthographic
    Vector3 mPos;      // 0x08
    Vector3 mTarget;   // 0x14
    Vector3 mUp;       // 0x20
    Vector3 mRot;      // 0x2C: angles (x, y, z)
    float mRoll;            // 0x38
    float mTop;             // 0x3C
    float mBottom;          // 0x40
    float mLeft;            // 0x44
    float mRight;           // 0x48
    float mDist;            // 0x4C
    float mFovy;            // 0x50
    float mAspect;          // 0x54
    Matrix3x4 mView;   // 0x58
    Matrix44_unk mProj; // 0x88
    float mNear;            // 0xC8
    float mFar;             // 0xCC
    Matrix3x4 mInvView; // 0xD0
    Frustum_unk mFrustum;   // 0x100
};

} // namespace atn

typedef atn::Camera Camera_unk;

// inferred: camera parameter applied to a model by fn_80095F10 (game/CharaTask.cpp, Event/DialogHead.cpp);
// names invented
struct ModelCameraParam_unk {
    ModelCameraParam_unk() : m00(0) {}
    void SetCamera_unk(const Camera_unk& camera) { mCamera = camera; } // inferred: name invented
    int m00;
    void* m04;
    Camera_unk mCamera; // 0x08
};
// inferred: applies a camera to a model
extern "C" void fn_80095F10(void* model, ModelCameraParam_unk* param);

#endif
