#include "game/FinishCamera_unk.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "gmk/Lemon.h"

// Everything below is inferred from the finishing camera (0x80378474..0x8037C71C); names are invented.

extern "C" int rand();
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);

// inferred: graphics device (lbl_8087EEE0)
struct GraphicsDevice_unk;
extern "C" GraphicsDevice_unk* lbl_8087EEE0;
extern "C" float fn_80076FF8(GraphicsDevice_unk* device); // inferred: screen aspect ratio

// inferred: debug menu entries
typedef void (*FinishMenuCallback_unk)(void* arg);
extern "C" void fn_8008771C(void* group, const char* name, float* value, FinishMenuCallback_unk callback, void* arg,
                            float min, float max, float step);
extern "C" void fn_80087858(void* group, const char* name, float* value, FinishMenuCallback_unk callback, void* arg,
                            float min, float max, float step); // inferred: angle shown in degrees

// inferred: collision world (lbl_8087EE98)
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, int a, int b); // inferred: segment test, hit position
extern "C" int fn_8004ED34(void* world, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, int a, int b);
extern "C" int fn_8004D388(void* world, CollResult_unk* result, const atn::Vector3* center, const atn::Vector3* move,
                           unsigned int mask, int a, int b, float radius); // inferred: sphere test

// inferred: global camera (lbl_8087EFB4) with its Camera_unk at 0x104
struct FinishView_unk {
    unsigned char m000[0x104];
    Camera_unk mCamera;
};
extern "C" FinishView_unk* lbl_8087EFB4;
extern "C" GameConfig_unk* lbl_8087F0A8;

// inferred: xsca camera curves (as in game/EnemyQuarkStand.cpp)
struct XsfCurveSet_unk {
    float fn_805BF204(float time, int channel);
    void fn_805BF410(float time, float* out, int x, int y, int z);
};
struct Xsca_unk {
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
};
// inferred: camera of the field (lbl_8087F430 + 0x6C)
struct CameraSub_unk {
    void fn_80389838(int arg);
};
extern "C" void fn_80392C94(CameraSub_unk* camera, void* state); // inferred: saves the camera state
extern "C" void fn_80392CE0(CameraSub_unk* camera, void* state); // inferred: restores the camera state
extern "C" void fn_803903E0(CameraSub_unk* camera, const atn::Vector3* target, float angle, int arg);

// inferred: adds the rotated value of a curve to `pos` (name invented)
inline void AddCurve_unk(atn::Vector3& pos, Mtx rot, XsfCurveSet_unk* curve, float time, int x, int y, int z) {
    atn::Vector3 v;
    curve->fn_805BF410(time, &v.x, x, y, z);
    atn::Vector3 w = v;
    PSMTXMultVec(rot, &w, &w);
    pos += w;
}

FinishCamera_unk::FinishCamera_unk()
    : m00(0), mCamera(0, 0), mPos(0.0f, 0.0f, 0.0f), mRot(0.0f, 0.0f, 0.0f), m35C(0), m360(0), m368(0), m394(0),
      m3A8(0), m3AC(0.0f), m3B0(0.0f), m3B4(0) {
    m2F8 = 0;
    m3A4 = 0;
    mCamera.mAspect = fn_80076FF8(lbl_8087EEE0);
    mAngleOfView = 0.87266463f;
    mLength = 25.0f;
    mTargetRatio = 0.5f;
    mOffsetY = 12.0f;
    void* menu = mMenu.Load("FinishCamera");
    fn_8008771C(menu, "Length", &mLength, 0, 0, 0.0f, 1000.0f, 1.0f);
    fn_8008771C(menu, "TargetRatio", &mTargetRatio, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_8008771C(menu, "OffsetY", &mOffsetY, 0, 0, 0.0f, 100.0f, 1.0f);
    fn_80087858(menu, "AngleOfView", &mAngleOfView, 0, 0, 0.0f, 360.0f, 1.0f);
}

void FinishCamera_unk::fn_80378610(int mode, CharaTask* chara, const atn::Vector3& target) {
    m04 = mode;
    mChara = chara;
    mTarget = target;
    mTarget.y += mOffsetY;
    mPos = chara->mPos;
    mRot = chara->mRot;
}

// NONMATCHING: 99.2% - r8/r9 swapped in the temporaries of the entry copy
void FinishCamera_unk::fn_8037865C(CharaTask* chara, const atn::Vector3& a, const atn::Vector3& b, int arg) {
    FinishCameraEntry_unk entry(chara, a, b, arg);
    if (mEntries.mCount < 7) {
        mEntries.PushBack_unk(entry);
    }
}

void FinishCamera_unk::fn_803786F0() {
    mEntries.mCount = 0;
}

// NONMATCHING: 99.5% - Vector3 copy with r3/r4 crossed (known blocker)
void FinishCamera_unk::fn_803786FC(CharaTask* chara) {
    m00 = 1;
    m3B4 = 0;
    float fovy = mAngleOfView;
    atn::Vector3 eye;
    atn::Vector3 target;
    atn::Vector3 hit;
    switch (m04) {
    case 0: {
        target = mPos;
        target.y += 6.0f + mOffsetY;
        eye.z = mLength;
        eye.x = 0.0f;
        eye.y = 0.0f;
        Mtx rot;
        PSMTXRotRad(rot, 'y', (3.7699115f + mRot.y) - 1.2566371f * ((float)(rand() % 1001) / 1000.0f));
        PSMTXMultVec(rot, &eye, &eye);
        eye += target;
        if (fn_8004ECC0(lbl_8087EE98, &hit, &target, &eye, 0x80000000, 0, 0) != 0) {
            eye = hit;
        }
        break;
    }
    case 1: {
        target = mChara->mPos;
        target.y += mOffsetY;
        eye.z = mLength;
        eye.x = 0.0f;
        eye.y = 0.0f;
        Mtx rot;
        PSMTXRotRad(rot, 'y', 6.2831855f * ((float)(rand() % 1001) / 1000.0f));
        PSMTXMultVec(rot, &eye, &eye);
        eye += target;
        if (fn_8004ECC0(lbl_8087EE98, &hit, &target, &eye, 0x80000000, 0, 0) != 0) {
            eye = hit;
        }
        break;
    }
    case 2:
    case 3:
        if (m35C != 0) {
            XsfCurveSet_unk* curve = m35C->fn_805BDCBC(0);
            target = mPos;
            Mtx rot;
            PSMTXRotRad(rot, 'y', mRot.y);
            AddCurve_unk(target, rot, curve, 0.0f, 7, 8, 9);
            eye = mPos;
            Mtx rot2;
            PSMTXRotRad(rot2, 'y', mRot.y);
            AddCurve_unk(eye, rot2, curve, 0.0f, 1, 2, 3);
        } else {
            target = lbl_8087EFB4->mCamera.mTarget;
            eye = lbl_8087EFB4->mCamera.mPos;
        }
        break;
    case 5:
        target = lbl_8087EFB4->mCamera.mTarget;
        eye = lbl_8087EFB4->mCamera.mPos;
        m394 = lbl_8087F0A8->m0D0 == 1;
        break;
    case 6:
        target = lbl_8087EFB4->mCamera.mTarget;
        eye = lbl_8087EFB4->mCamera.mPos;
        m394 = lbl_8087F0A8->m0D0 == 1;
        break;
    }
    m354 = chara;
    m350 = chara;
    m32C = eye;
    m344 = target;
    mCamera.mTarget = target;
    mCamera.mPos = eye;
    mCamera.mFovy = fovy;
    mCamera.fn_8004B378();
    m358 = 0;
    m364 = 0;
    if (m394 != 0) {
        fn_80392C94(m368, m36C);
        m368->fn_80389838(6);
        fn_803903E0(m368, &target, 3.1415927f, 1);
    }
    m398 = 0;
    m3A0 = 1.0f;
    m2F8 = 0;
    m3A4 = 0;
    m3A8 = 0;
}

void FinishCamera_unk::fn_80378CAC() {
    if (m394 != 0) {
        fn_80392C94(m368, m36C);
    }
}

void FinishCamera_unk::fn_80378CCC() {
    m00 = 0;
    m35C = 0;
    m360 = 0;
    m3AC = 0.0f;
    m3B0 = 0.0f;
    if (m394 != 0) {
        fn_80392CE0(m368, m36C);
        m394 = 0;
    }
}

// NONMATCHING: 79.9% - frame 0x1A0 vs 0x150 (more temporaries in the original: hit at sp+0xE0)
void FinishCamera_unk::fn_80378D34(atn::Vector3* eye, const atn::Vector3* target, const atn::Vector3* start,
                                   int keep) {
    CollResult_unk hit;
    atn::Vector3 to = *target;
    atn::Vector3 from = *eye;
    if (fn_8004ED34(lbl_8087EE98, &hit, &from, &to, 0x80000008, 0, 0) != 0) {
        *eye = hit.m04 + hit.m28;
    }
    float step = PSVECMag(&(*target - *eye)) * 0.25f;
    atn::Vector3 cur = *target;
    int hits = 0;
    for (int i = 1; i <= 4; i++) {
        atn::Vector3 dir = *eye - cur;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 move = dir * step;
        atn::Vector3 center = cur + move;
        atn::Vector3 zero(0.0f, 0.0f, 0.0f);
        if (fn_8004D388(lbl_8087EE98, &hit, &center, &zero, 0x80000008, 0, 0, 3.0f) != 0) {
            hits++;
            cur.x = hit.m10.x;
            cur.z = hit.m10.z;
            cur.y = (center.y + hit.m10.y) * 0.5f;
        } else {
            cur = center;
        }
    }
    *eye = cur;
    if (hits > 0 && keep != 0) {
        if (PSVECSquareMag(&(*eye - *start)) > 0.00001f) {
            *eye = *start + (*eye - *start) * 0.25f;
        }
    }
}

// NONMATCHING: 99.7% - register order of the offset computation
float FinishCamera_unk::fn_80379104(XsfCurveSet_unk* curve, const atn::Vector3* pos, float time, float angle) {
    CollResult_unk hit;
    float offset;
    for (unsigned int i = 0; i < 8; i++) {
        Mtx rot;
        atn::Vector3 from = *pos;
        atn::Vector3 to = *pos;
        offset = 3.1415927f * (float)i / 4.0f;
        PSMTXRotRad(rot, 'y', angle + offset);
        AddCurve_unk(to, rot, curve, time, 1, 2, 3);
        atn::Vector3 a = from;
        atn::Vector3 b = to;
        if (fn_8004ED34(lbl_8087EE98, &hit, &a, &b, 0x80000008, 0, 0) == 0) {
            break;
        }
    }
    return offset;
}

// NOT DECOMPILED: fn_803792F0 (0x803792F0, 0x33A0 bytes, camera update state machine) is still
// missing from this file, so the unit cannot link yet.

void FinishCamera_unk::fn_8037C690(Xsca_unk* xsca, int arg) {
    m35C = xsca;
    m360 = arg;
}

void FinishCamera_unk::fn_8037C69C() {
    if (m3A8 != 0) {
        m3A8->fn_80149A30();
    }
    if (m04 == 6) {
        for (unsigned int i = 0; i < mEntries.mCount; i++) {
            mEntries.mData[i].mChara->fn_80149A30();
        }
    }
}
