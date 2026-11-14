#include "game/FinishCamera_unk.h"
#include "game/GameCamera.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "gmk/Lemon.h"
#include "atn/RenderManager.h"
#include "math/VecAssign_unk.h"
#include "game/GameUI.h"
#include "game/GameVoice.h"
#include "game/MenuCommonTask.h"
#include "game/PlayerTask.h"
#include "game/CharaManager.h"
#include "game/HitInfo_unk.h"
#include "atn/RenderSetting_unk.h"
#include "macros.h"

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

extern "C" GameConfig_unk* lbl_8087F0A8;

// inferred: xsca camera curves (as in game/EnemyQuarkStand.cpp)
struct XsfCurveSet_unk {
    int fn_805BF1E8(int channel);                 // inferred: the channel has keys
    float fn_805BF204(float time, int channel);
    void fn_805BF410(float time, float* out, int x, int y, int z);
    unsigned char m00[0x14];
    float m14; // inferred: last frame of the curves
};
struct Xsca_unk {
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
};

// inferred: values of the slow motion of the finishing blow 0x179A (fovy blends of the modes 2 and 3)
float lbl_8087DCE0 = 1.0f;
float lbl_8087DCE4 = 0.3f;
float lbl_8087DCE8 = 1.0f;
float lbl_8087DCEC = 0.3f;

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" RenderSetting_unk* lbl_8087EFA8;
extern "C" GameUI* lbl_8087F490;
extern "C" GameVoice* lbl_8087F498;
extern "C" void* lbl_8087F610; // inferred: multiplayer session (only tested against 0 here)
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_804A5E40(MenuCommonTask* menu, int type, atn::Vector3 ofs, int id); // inferred: info window
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* r);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
extern "C" double sin(double x);
extern "C" double tan(double x);
extern "C" double atan(double x);
extern "C" double atan2(double y, double x);

// inferred: rotation matrix z, then y, then x (weak copy; as in game/GameCamera_unk.cpp)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}

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


void FinishCamera_unk::fn_803786FC(int frames) {
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
            atn::VecAssign_unk(eye, hit);
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
            atn::VecAssign_unk(eye, hit);
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
            atn::VecAssign_unk(target, atn::RenderManager::instance_->mCamera.mTarget);
            eye = atn::RenderManager::instance_->mCamera.mPos;
        }
        break;
    case 5:
        atn::VecAssign_unk(target, atn::RenderManager::instance_->mCamera.mTarget);
        eye = atn::RenderManager::instance_->mCamera.mPos;
        m394 = lbl_8087F0A8->m0D0 == 1;
        break;
    case 6:
        atn::VecAssign_unk(target, atn::RenderManager::instance_->mCamera.mTarget);
        eye = atn::RenderManager::instance_->mCamera.mPos;
        m394 = lbl_8087F0A8->m0D0 == 1;
        break;
    }
    m354 = frames;
    m350 = frames;
    m32C = eye;
    m344 = target;
    mCamera.mTarget = target;
    mCamera.mPos = eye;
    mCamera.mFovy = fovy;
    mCamera.fn_8004B378();
    m358 = 0;
    m364 = 0;
    if (m394 != 0) {
        m368->fn_80392C94(&m36C);
        m368->fn_80389838(6);
        m368->fn_803903E0(&target, 3.1415927f, 1);
    }
    m398 = 0;
    m3A0 = 1.0f;
    m2F8 = 0;
    m3A4 = 0;
    m3A8 = 0;
}

void FinishCamera_unk::fn_80378CAC() {
    if (m394 != 0) {
        m368->fn_80392C94(&m36C);
    }
}

void FinishCamera_unk::fn_80378CCC() {
    m00 = 0;
    m35C = 0;
    m360 = 0;
    m3AC = 0.0f;
    m3B0 = 0.0f;
    if (m394 != 0) {
        m368->fn_80392CE0(&m36C);
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
// inferred: called with bl by fn_803792F0, where MWCC would inline it (accepted __declspec(noinline), rule 2)
__declspec(noinline) float FinishCamera_unk::fn_80379104(XsfCurveSet_unk* curve, const atn::Vector3* pos, float time, float angle) {
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

// inferred: update of the finishing camera. While m350 counts down (set by GameTask for the slow motion of the
// last blow) the camera turns from m32C/m338 around the character and back; otherwise by mode (m04):
// 0 = from the field camera to the target mTarget in 30 frames, keeping 60 units away; 1 = free camera turned with
// the stick around the character for 160 frames; 2 and 3 = xsca camera curves around mPos/the character (m35C, or
// m360 seen from the target); 5 = curves around each character of mEntries in turn (m2F8), with the motion blur at
// the ends; 6 = the same around all of them at once. The camera is then set to the render manager (or the field
// camera is moved to the target when it is taken over, m394). The curves are added by hand (AddCurve_unk as a
// candidate puts the function over the inlining budget)
// NONMATCHING: 94.4% - stack slots and float registers of the temporaries (the original keeps the curve matrices
// in r29), five more saved gprs here
void FinishCamera_unk::fn_803792F0() {
    if (m00 == 0) {
        return;
    }
    atn::Vector3 eye;
    atn::Vector3 target;
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    float fovy = mAngleOfView;
    if (m350 > 0) {
        atn::Vector3 dir;
        if (m350 == m354) {
            m338 = lbl_8087F8A0->mHead->mMotion.mMtx.GetTrans_unk();
            m338.y += mOffsetY;
            m344 = mChara->mMotion.mMtx.GetTrans_unk();
            m344.y += mOffsetY;
            dir = m32C - m344;
            dir.fn_80139F60();
            m320.x = 0.0f;
            m320.y = 0.0f;
            m320.z = mLength;
            Mtx rot;
            PSMTXRotRad(rot, 'y', 0.31415927f + (dir.y + 0.62831855f * ((float)(rand() % 1001) / 1000.0f)));
            PSMTXMultVec(rot, &m320, &m320);
            m320 += m338;
            if (m398 == 0) {
                dir.x = 0.0f;
                dir.y = 0.0f;
                dir.z = 1.0f;
                atn::Matrix3x4 m(mChara->mMotion.mMtx);
                m.m[0][3] = 0.0f;
                m.m[1][3] = 0.0f;
                m.m[2][3] = 0.0f;
                PSMTXMultVec(m.m, &dir, &dir);
                dir.y = 0.0f;
                float angle = 0.0f;
                if ((float)__fabs(dir.x) > (float)__fabs(dir.z)) {
                    angle = 1.5707964f * (dir.x >= 0.0f ? -1.0f : 1.0f);
                } else if (dir.z < 0.0f) {
                    angle = 3.1415927f;
                }
                m39C = angle - 0.62831855f;
                m368->fn_803903E0(&m368->m8A4, m39C, 1);
                m398 = 1;
            }
        }
        target = mChara->mMotion.mMtx.GetTrans_unk();
        target.y += mOffsetY;
        float t = 0.5f * (float)sin(0.5f * (3.1415927f * (2.0f * ((float)(m354 - m350) / (float)m354) - 1.0f))) +
                  0.5f;
        eye = (m32C - m320) * t + m320;
        target = (target - m338) * t + m338;
        m368->fn_803903E0(&m368->m8A4, 0.62831855f * (1.0f - m3A0) + m39C, 1);
        m350--;
    } else {
        switch (m04) {
        case 0:
            if (m358 < 20) {
                target = mChara->mMotion.mMtx.GetTrans_unk();
                target.y += mOffsetY;
                eye = mCamera.mPos;
                m338 = target;
            } else if (m358 < 50) {
                float t = (float)(m358 - 20) / 30.0f;
                target = (mTarget - m338) * t + m338;
                eye = mCamera.mPos;
                atn::Vector3 d = target - eye;
                float len = PSVECMag(&d);
                if (len > 60.0f) {
                    PSVECNormalize(&d, &d);
                    eye = target - d * (0.5f * (60.0f + len));
                }
            } else {
                target = mCamera.mTarget;
                eye = mCamera.mPos;
                if (m394 != 0) {
                    return;
                }
            }
            m358++;
            break;
        case 1:
            if (m358 < 160) {
                target = mChara->mMotion.mMtx.GetTrans_unk();
                target.y += mOffsetY;
                float sx = fn_800A56A8(lbl_8087EF70, 0, 0, 0);
                float sy = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
                eye = mCamera.mPos;
                atn::Vector3 d = target - eye;
                float len = PSVECMag(&d);
                PSVECNormalize(&d, &d);
                atn::Vector3 rot(d);
                rot.fn_80139F60();
                if ((float)__fabs(sx) > 0.4f) {
                    float v = (float)__fabs(sx) * 0.2f;
                    v *= v;
                    rot.y += sx > 0.0f ? v : -v;
                }
                if ((float)__fabs(sy) > 0.4f) {
                    float v = (float)__fabs(sy) * 0.2f;
                    v *= v;
                    rot.x += sy > 0.0f ? v : -v;
                }
                d = atn::Vector3(0.0f, 0.0f, 1.0f);
                atn::Matrix3x4 m = fn_800109E0(rot);
                PSMTXMultVec(m.m, &d, &d);
                eye = d * -len + target;
            } else if (m394 != 0) {
                return;
            }
            m358++;
            break;
        case 2:
            if (m35C != 0) {
                atn::Vector3 start(mCamera.mPos);
                XsfCurveSet_unk* curve = m35C->fn_805BDCBC(0);
                float time = (float)m358 > curve->m14 ? curve->m14 : (float)m358;
                float angle;
                if (m3B4 != 0) {
                    angle = fn_80379104(curve, &mPos, 5.0f, mRot.y);
                } else {
                    angle = 0.0f;
                }
                target = mPos;
                eye = mPos;
                Mtx rot;
                PSMTXRotRad(rot, 'y', mRot.y + angle);
                {
                    atn::Vector3 v;
                    curve->fn_805BF410(time, &v.x, 7, 8, 9);
                    atn::Vector3 w = v;
                    PSMTXMultVec(rot, &w, &w);
                    target += w;
                }
                Mtx rot2;
                PSMTXRotRad(rot2, 'y', mRot.y + angle);
                {
                    atn::Vector3 v;
                    curve->fn_805BF410(time, &v.x, 1, 2, 3);
                    atn::Vector3 w = v;
                    PSMTXMultVec(rot2, &w, &w);
                    eye += w;
                }
                if (curve->fn_805BF1E8(0)) {
                    fovy = 2.0f * (float)atan((float)tan(0.5f * (0.017453292f * curve->fn_805BF204(time, 0))) /
                                              mCamera.mAspect);
                }
                if (time > 50.0f && mChara->m638 != 0 && mChara->m638->m04 == 0x179A) {
                    float k = (time - 50.0f) * 0.25f;
                    k = 1.0f < k ? 1.0f : k;
                    eye.y += 2.0f * k;
                    target.y += 2.5f * k;
                    fovy *= k * (lbl_8087DCE4 - lbl_8087DCE0) + lbl_8087DCE0;
                    lbl_8087EFA8->m3A4 = 0.5f;
                }
                fn_80378D34(&eye, &(mPos + atn::Vector3(0.0f, 5.0f, 0.0f)), &start, m358 > 0);
                atn::Vector3 roll;
                curve->fn_805BF410(time, &roll.x, 4, 5, 6);
                Mtx rotZ;
                PSMTXRotRad(rotZ, 'z', 0.017453292f * roll.z);
                PSMTXMultVec(rotZ, &up, &up);
                if ((m360 != 0 && (float)m358 > curve->m14 - 30.0f) || (m360 == 0 && (float)m358 > curve->m14)) {
                    if (mChara->m638 != 0 && mChara->m638->m04 == 0x179A) {
                        lbl_8087EFA8->m3A4 = 1.0f;
                    }
                    if (m360 != 0) {
                        m35C = 0;
                        m358 = -1;
                    } else {
                        fn_80378CCC();
                    }
                }
            } else if (m360 != 0) {
                atn::Vector3 start(mCamera.mPos);
                XsfCurveSet_unk* curve = ((Xsca_unk*)m360)->fn_805BDCBC(0); // inferred: m360 holds a second camera animation
                float time = (float)m358 > curve->m14 ? curve->m14 : (float)m358;
                float angle = fn_80379104(curve, &(mTarget + atn::Vector3(0.0f, 5.0f, 0.0f)), 5.0f,
                                          3.1415927f + mRot.y);
                float swing = 1.5f * (float)sin(0.5f * angle);
                if (mChara->m638 != 0 && mChara->m638->m04 == 0x179A) {
                    time = MIN(curve->m14, 0.6f * (float)m358);
                }
                target = mTarget;
                eye = mTarget;
                Mtx rot;
                PSMTXRotRad(rot, 'y', angle + (3.1415927f + mRot.y));
                {
                    atn::Vector3 v;
                    curve->fn_805BF410(time, &v.x, 7, 8, 9);
                    atn::Vector3 w = v;
                    PSMTXMultVec(rot, &w, &w);
                    target += w;
                }
                Mtx rot2;
                PSMTXRotRad(rot2, 'y', angle + (3.1415927f + mRot.y));
                {
                    atn::Vector3 v;
                    curve->fn_805BF410(time, &v.x, 1, 2, 3);
                    atn::Vector3 w = v;
                    PSMTXMultVec(rot2, &w, &w);
                    eye += w;
                }
                atn::Vector3 d(target.x - eye.x, 0.0f, target.z - eye.z);
                eye -= d * (m3AC + swing);
                if (curve->fn_805BF1E8(0)) {
                    fovy = 2.0f * (float)atan((float)tan(0.5f * (0.017453292f * curve->fn_805BF204(time, 0))) /
                                              mCamera.mAspect);
                }
                if (mChara->m638 != 0 && mChara->m638->m04 == 0x179A) {
                    fovy *= 1.2f;
                }
                fn_80378D34(&eye, &(mTarget + atn::Vector3(0.0f, 5.0f, 0.0f)), &start, m358 > 0);
                atn::Vector3 roll;
                curve->fn_805BF410(time, &roll.x, 4, 5, 6);
                Mtx rotZ;
                PSMTXRotRad(rotZ, 'z', 0.017453292f * roll.z);
                PSMTXMultVec(rotZ, &up, &up);
                Mtx rotY;
                PSMTXRotRad(rotY, 'y', 3.1415927f + mRot.y);
                PSMTXMultVec(rotY, &up, &up);
                if (m368->mShake.mFrame < m368->mShake.mLength) {
                    atn::Vector3 shake = m368->mShake.GetOffset_unk();
                    atn::Vector3 view = target - eye;
                    if (!((float)__fabs(PSVECSquareMag(&view)) < 1.1920929e-7f)) {
                        PSVECNormalize(&view, &view);
                    }
                    atn::Vector3 r = view.ToRotation_unk();
                    Mtx m;
                    PSMTXRotRad(m, 'y', r.y);
                    PSMTXMultVec(m, &shake, &shake);
                    target += shake;
                }
                if (mChara->m638 != 0 && mChara->m638->m04 == 0x179A) {
                    if (0.6f * (float)m358 > curve->m14 && (float)m358 > m3B0) {
                        fn_80378CCC();
                    }
                } else if ((float)m358 > curve->m14 && (float)m358 > m3B0) {
                    fn_80378CCC();
                }
            }
            m358++;
            break;
        case 3:
            if (m35C != 0) {
                atn::Vector3 start(mCamera.mPos);
                XsfCurveSet_unk* curve = m35C->fn_805BDCBC(0);
                float time = (float)m358 > curve->m14 ? curve->m14 : (float)m358;
                float angle;
                if (time >= 61.0f) {
                    angle = fn_80379104(curve, (mChara != 0 ? &mChara->mPos : &mPos), curve->m14, mRot.y);
                } else {
                    angle = fn_80379104(curve, (mChara != 0 ? &mChara->mPos : &mPos), 5.0f, mRot.y);
                }
                target = *(mChara != 0 ? &mChara->mPos : &mPos);
                eye = *(mChara != 0 ? &mChara->mPos : &mPos);
                Mtx rot;
                PSMTXRotRad(rot, 'y', mRot.y + angle);
                {
                    atn::Vector3 v;
                    curve->fn_805BF410(time, &v.x, 7, 8, 9);
                    atn::Vector3 w = v;
                    PSMTXMultVec(rot, &w, &w);
                    target += w;
                }
                Mtx rot2;
                PSMTXRotRad(rot2, 'y', mRot.y + angle);
                {
                    atn::Vector3 v;
                    curve->fn_805BF410(time, &v.x, 1, 2, 3);
                    atn::Vector3 w = v;
                    PSMTXMultVec(rot2, &w, &w);
                    eye += w;
                }
                if (time >= 61.0f) {
                    atn::Vector3 d(target.x - eye.x, 0.0f, target.z - eye.z);
                    eye -= d * m3AC;
                }
                if (curve->fn_805BF1E8(0)) {
                    fovy = 2.0f * (float)atan((float)tan(0.5f * (0.017453292f * curve->fn_805BF204(time, 0))) /
                                              mCamera.mAspect);
                }
                if (time < 61.0f && time > 50.0f) {
                    float k = (time - 50.0f) / 5.0f;
                    k = 1.0f < k ? 1.0f : k;
                    fovy *= k * (lbl_8087DCEC - lbl_8087DCE8) + lbl_8087DCE8;
                }
                fn_80378D34(&eye, &(*(mChara != 0 ? &mChara->mPos : &mPos) + atn::Vector3(0.0f, 5.0f, 0.0f)), &start, m358 > 0);
                atn::Vector3 roll;
                curve->fn_805BF410(time, &roll.x, 4, 5, 6);
                Mtx rotZ;
                PSMTXRotRad(rotZ, 'z', 0.017453292f * roll.z);
                PSMTXMultVec(rotZ, &up, &up);
                if ((float)m358 > curve->m14) {
                    fn_80378CCC();
                }
            }
            m358++;
            break;
        case 5: {
            int shown = m3A4;
            atn::Vector3 start(mCamera.mPos);
            FinishCameraEntry_unk& entry = mEntries.mData[m2F8];
            atn::Vector3 pos(entry.m04);
            atn::Vector3 rot(entry.m10);
            int frames = entry.m1C * 30 + 120;
            XsfCurveSet_unk* curve = m35C->fn_805BDCBC(0);
            float time = mEntries.mData[m2F8].m1C > 0 ? 0.7f * (float)m358 : (float)m358;
            time = time > curve->m14 ? curve->m14 : time;
            target = pos;
            Mtx mt;
            PSMTXRotRad(mt, 'y', rot.y);
            {
                atn::Vector3 v;
                curve->fn_805BF410(time, &v.x, 7, 8, 9);
                atn::Vector3 w = v;
                PSMTXMultVec(mt, &w, &w);
                target += w;
            }
            eye = pos;
            Mtx me;
            PSMTXRotRad(me, 'y', rot.y);
            {
                atn::Vector3 v;
                curve->fn_805BF410(time, &v.x, 1, 2, 3);
                atn::Vector3 w = v;
                PSMTXMultVec(me, &w, &w);
                eye += w;
            }
            atn::Vector3 roll;
            curve->fn_805BF410(time, &roll.x, 4, 5, 6);
            Mtx rotZ;
            PSMTXRotRad(rotZ, 'z', roll.z);
            PSMTXMultVec(rotZ, &up, &up);
            atn::Vector3 side(target.x - eye.x, 0.0f, target.z - eye.z);
            atn::Vector3 n(side);
            PSVECNormalize(&n, &n);
            PSVECCrossProduct(&n, &up, &side);
            fn_80378D34(&eye, &pos, &start, m358 > 0);
            if (m358 < 3) {
                float k = (float)(3 - m358) / 3.0f;
                target -= side * k * k * 6.0f;
                MotionBlurParam_unk blur = lbl_8087EFA8->m240;
                blur.mEnabled = 1;
                lbl_8087EFA8->m240 = blur;
            } else if (m358 > frames - 5) {
                float k = (float)(m358 - (frames - 5)) / 5.0f;
                target += side * k * k * 10.0f;
                MotionBlurParam_unk blur = lbl_8087EFA8->m240;
                blur.mEnabled = 1;
                lbl_8087EFA8->m240 = blur;
            } else {
                MotionBlurParam_unk blur = lbl_8087EFA8->m240;
                blur.mEnabled = 0;
                lbl_8087EFA8->m240 = blur;
                if (m3A4 == 0) {
                    m3A4 = 1;
                    lbl_8087F490->fn_803E41AC(mChara);
                }
            }
            m3A8 = mChara;
            if (m3A8->mMotion.mTracks[0].m08 >= m3A8->mMotion.fn_80097D7C(0)) {
                if (lbl_8087F610 != 0) {
                    m3A8->mMotion.fn_80097C08(0, m3A8->m484[0], 1, 0, 1, 0.0f, 0.1f);
                } else if (m3A8->m5C->m11C == 2) {
                    m3A8->mMotion.fn_80097C08(0, 4, 1, 0, 1, 1.0f, 0.1f);
                } else if (m3A8->m50 == 1) {
                    m3A8->mMotion.fn_80097C08(0, 5, 1, 0, 1, 1.0f, 0.1f);
                } else {
                    m3A8->mMotion.fn_80097C08(0, 3, 1, 0, 1, 1.0f, 0.1f);
                }
            }
            m3A8->m1220.fn_8012111C();
            m3A8->mMotion.mTracks[0].m0C = 1.0f;
            m3A8->mMotion.fn_80097E80(1);
            m3A8->mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
            m3A8->fn_80148B38(10.0f);
            if (shown == 0 && m3A4 != 0 && lbl_8087F498 != 0) {
                lbl_8087F498->fn_803EA77C(m3A8, 40, 15, 1.0f, 10.0f);
            }
            if (m358 > frames) {
                if (++m2F8 >= mEntries.mCount) {
                    fn_80378CCC();
                } else {
                    m358 = 0;
                    mChara = mEntries.mData[m2F8].mChara;
                    m3A4 = 0;
                }
                break;
            }
            m358++;
            if (m358 >= 3 && m358 < frames - 5 && mEntries.mData[m2F8].m1C <= 0 &&
                lbl_8087F0A8->mInput.fn_801231D0(0) && lbl_8087F610 == 0) {
                m358 = frames - 5;
            }
            if (lbl_8087F490->fn_803E43A4()) {
                if (m358 == frames - 5) {
                    if (lbl_8087F490->fn_803E43A4()) {
                        if (lbl_8087F0A8->mInput.fn_801231D0(0) || lbl_8087F610 != 0) {
                            m358 = frames - 5;
                        } else {
                            m358 = frames - 6;
                            lbl_8087F490->m112C = 1;
                            fn_804A5E40(MenuCommonTask::sInstance, 14, lbl_807C7030, 0);
                        }
                    }
                } else if (m358 < frames - 5) {
                    lbl_8087F490->m112C = 1;
                }
            }
            break;
        }
        case 6: {
            int shown = m3A4;
            FinishCameraEntry_unk& entry = mEntries.mData[m2F8];
            atn::Vector3 pos(entry.m04);
            atn::Vector3 rot(entry.m10);
            XsfCurveSet_unk* curve = m35C->fn_805BDCBC(0);
            float time = 0.6f * (float)m358 > curve->m14 ? curve->m14 : 0.6f * (float)m358;
            target = pos;
            Mtx mt;
            PSMTXRotRad(mt, 'y', rot.y);
            {
                atn::Vector3 v;
                curve->fn_805BF410(time, &v.x, 7, 8, 9);
                atn::Vector3 w = v;
                PSMTXMultVec(mt, &w, &w);
                target += w;
            }
            eye = pos;
            Mtx me;
            PSMTXRotRad(me, 'y', rot.y);
            {
                atn::Vector3 v;
                curve->fn_805BF410(time, &v.x, 1, 2, 3);
                atn::Vector3 w = v;
                PSMTXMultVec(me, &w, &w);
                eye += w;
            }
            atn::Vector3 roll;
            curve->fn_805BF410(time, &roll.x, 4, 5, 6);
            Mtx rotZ;
            PSMTXRotRad(rotZ, 'z', roll.z);
            PSMTXMultVec(rotZ, &up, &up);
            atn::Vector3 side(target.x - eye.x, 0.0f, target.z - eye.z);
            atn::Vector3 n(side);
            PSVECNormalize(&n, &n);
            PSVECCrossProduct(&n, &up, &side);
            if (m358 < 3) {
                float k = (float)(3 - m358) / 3.0f;
                target -= side * k * k * 6.0f;
                MotionBlurParam_unk blur = lbl_8087EFA8->m240;
                blur.mEnabled = 1;
                lbl_8087EFA8->m240 = blur;
            } else if (m358 > 40) {
                float k = (float)(m358 - 40) / 15.0f;
                k = 1.0f < k ? 1.0f : k;
                float s = 0.5f * (1.0f + (float)sin(3.1415927f * k - 1.5707964f));
                side = eye - target;
                PSVECNormalize(&side, &side);
                side.y = -side.y;
                eye += side * 30.0f * s;
                MotionBlurParam_unk blur = lbl_8087EFA8->m240;
                blur.mEnabled = s < 0.9f;
                lbl_8087EFA8->m240 = blur;
            } else {
                MotionBlurParam_unk blur = lbl_8087EFA8->m240;
                blur.mEnabled = 0;
                lbl_8087EFA8->m240 = blur;
                if (m3A4 == 0) {
                    m3A4 = 1;
                    lbl_8087F490->fn_803E41AC(mChara);
                }
            }
            for (unsigned int i = 0; i < mEntries.mCount; i++) {
                CharaTask* chara = mEntries.mData[i].mChara;
                if (chara->mMotion.mTracks[0].m08 >= chara->mMotion.fn_80097D7C(0)) {
                    chara->mMotion.fn_80097C08(0, chara->m484[0], 1, 0, 1, 0.0f, 0.05f);
                }
                chara->m1220.fn_8012111C();
                chara->mMotion.mTracks[0].m0C = 1.0f;
                chara->mMotion.fn_80097E80(1);
                chara->mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
                chara->fn_80148B38(10.0f);
                if (i == 0 && shown == 0 && m3A4 != 0 && lbl_8087F498 != 0) {
                    lbl_8087F498->fn_803EA77C(chara, 40, 15, 1.0f, 10.0f);
                }
            }
            m358++;
            break;
        }
        }
    }
    mCamera.mPos = eye;
    mCamera.mTarget = target;
    mCamera.mFovy = fovy;
    mCamera.mUp = up;
    mCamera.fn_8004B378();
    if (m394 != 0) {
        m368->fn_803903E0(&target, m368->m8A0, 1);
        m368->fn_8037F744();
        atn::RenderManager::instance_->SetCamera_unk(*m368);
    } else {
        atn::RenderManager::instance_->SetCamera_unk(mCamera);
    }
}


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
