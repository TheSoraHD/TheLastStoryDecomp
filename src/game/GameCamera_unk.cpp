// Camera of the field (GameCamera, GameTask + 0x6C). Only a few members are written; the constructor and
// the update (0x8037F9AC) are still missing, so the unit is not linked.
#include "game/GameCamera.h"
#include "game/GameScene.h"
#include "game/CharaManager.h"
#include "game/GameConfig_unk.h"
#include "game/PlayerTask.h"
#include "atn/RenderSetting_unk.h"
#include "atn/RenderManager.h"
#include "atn/DrawHelper.h"
#include "macros.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "gmk/Lemon.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" RenderSetting_unk* lbl_8087EFA8;
extern "C" int sprintf(char* buf, const char* fmt, ...);

static inline atn::Vector3 Negate_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: starts the parameters of a camera mode (blend weight 1, decreased by the speed)
void GameCameraModeParam_unk::fn_8037E89C(int mode, const GameCameraParam_unk& param, float step) {
    if (mMode != mode && mMode != 4) {
        mMode = mode;
        mParam = param;
        mStep = step;
        mWeight = 1.0f;
    }
}

// inferred: debug menu items (atn debug menu, as in gmk/SwingObject.cpp and gmk/Ocean.cpp)
extern "C" void* fn_8008937C(void* menu, const char* name); // inferred: group of a menu
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087858(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: angle item

// inferred: debug menu of the parameters of a camera mode
// NONMATCHING: 94.3% - offsets of the strings until the rest of the pool is written; named floats
void GameCamera::fn_8037ECF0(const char* name, void* menu, GameCameraParam_unk* param) {
    void* group = fn_8008937C(menu, name);
    fn_8008771C(group, "Length", &param->m00, 0, 0, 1.0f, 10000.0f, 5.0f);
    fn_80087858(group, "AngleOfView", &param->m04, 0, 0, 0.0f, 90.0f, 5.0f);
    fn_80087858(group, "Pitch", &param->m08, 0, 0, -90.0f, 90.0f, 5.0f);
    fn_8008771C(group, "OffsetX", &param->m0C, 0, 0, -1000.0f, 1000.0f, 1.0f);
    fn_8008771C(group, "OffsetY", &param->m10, 0, 0, -1000.0f, 1000.0f, 1.0f);
    fn_8008771C(group, "OffsetZ", &param->m14, 0, 0, -1000.0f, 1000.0f, 1.0f);
    fn_8008771C(group, "CrouchOffset", &param->m18, 0, 0, -1000.0f, 1000.0f, 1.0f);
    fn_8008771C(group, "MoveSpeed", &param->m1C, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(group, "PitchSpeed", &param->m20, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(group, "ZoomSpeed", &param->m24, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(group, "RollSpeed", &param->m28, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(group, "AutoRollSpeed", &param->m2C, 0, 0, 0.0f, 1.0f, 0.001f);
    fn_8008771C(group, "AutoLockOnSpeed", &param->m30, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80087858(group, "OffsetRotY", &param->m34, 0, 0, -180.0f, 180.0f, 1.0f);
}

void GameCamera::fn_8037EF30(int a, int b, float f) {
    fn_8037EF48(lbl_8087F8A0->mHead, a, b, f);
}

void GameCamera::fn_8037F690(const atn::Vector3* pos) {
    m840 = *pos;
    m838 = 1;
    m83C = 0;
    m84C = mTarget;
    m85C = mFovy;
    m860 = 0;
}

void GameCamera::fn_8037F6D8(const atn::Vector3* pos) {
    m840 = *pos;
    m858 = 1;
    m838 = 1;
    m83C = 0;
    m84C = mTarget;
    m85C = mFovy;
    m860 = 0;
}

void GameCamera::fn_8037F724(const atn::Vector3* pos) {
    m840 = *pos;
    m860 = 0;
}

// inferred: member function of a camera mode
typedef void (GameCamera::*GameCameraMode_unk)(const CharaTask* chara);

// inferred: first frame of a mode (the mode changed) (21 modes; name invented)
static const GameCameraMode_unk sModeStart_unk[21] = {
    &GameCamera::fn_80389A00, &GameCamera::fn_8038A200, &GameCamera::fn_8038A5F0,
    &GameCamera::fn_8038B148, &GameCamera::fn_8038B650, &GameCamera::fn_8038BD78,
    &GameCamera::fn_8038C070, &GameCamera::fn_8038C5B0, &GameCamera::fn_8038A1D8,
    &GameCamera::fn_8038A1B8, &GameCamera::fn_8038C928, &GameCamera::fn_8038CD94,
    &GameCamera::fn_8038D1C0, &GameCamera::fn_8038D4D4, &GameCamera::fn_8038D854,
    &GameCamera::fn_8038DBE4, &GameCamera::fn_80389A00, &GameCamera::fn_80389A00,
    &GameCamera::fn_8038DDE0, &GameCamera::fn_8038DDE0, &GameCamera::fn_8038DF5C,
};

// inferred: update of a mode (21 modes; name invented)
static const GameCameraMode_unk sModeUpdate_unk[21] = {
    &GameCamera::fn_8037F9AC, &GameCamera::fn_803830A8, &GameCamera::fn_8038378C,
    &GameCamera::fn_80384188, &GameCamera::fn_80385B48, &GameCamera::fn_80387548,
    &GameCamera::fn_80387910, &GameCamera::fn_803884B8, &GameCamera::fn_8037F9AC,
    &GameCamera::fn_80382A1C, &GameCamera::fn_80388A88, &GameCamera::fn_80388C44,
    &GameCamera::fn_80389060, &GameCamera::fn_80389210, &GameCamera::fn_803893A0,
    &GameCamera::fn_8038952C, &GameCamera::fn_8037F9AC, &GameCamera::fn_8037F9AC,
    &GameCamera::fn_80389678, &GameCamera::fn_80389678, &GameCamera::fn_803897EC,
};

void GameCamera::fn_8037F744() {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    mNear = lbl_8087EFA8->m1C0;
    mFar = lbl_8087EFA8->m1C4;
    if (m800 != m7FC) {
        (this->*sModeStart_unk[m7FC])(leader);
    } else {
        (this->*sModeUpdate_unk[m7FC])(leader);
    }
    if (mShake.mFrame < mShake.mLength) {
        mShake.mFrame++;
    }
    if (m910 != 0) {
        m914 = m914 + 1 < m918 ? m914 + 1 : m918;
    } else {
        int n = m914 - 2;
        m914 = n > 0 ? n : 0;
    }
}

// inferred: direction the camera looks at (Event/Manager.cpp)
extern "C" atn::Vector3 fn_80383728(const Camera_unk* camera) {
    atn::Vector3 dir;
    dir = *camera->GetTarget_unk() - *camera->GetPosition_unk();
    dir.Normalize_unk();
    return dir;
}

// ---- fn_80387910: camera of the tactical view (over the inlining budget: the accessors are weak copies) ----
extern "C" double sin(double x);
extern "C" double tan(double x);
extern "C" double atan(double x);
extern "C" double sqrt(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// inferred: math helpers (weak copies; as in game/Em309Ghoul.cpp, game/BattleTask.cpp, gmk/Cannonball.cpp)
extern "C" inline float fn_80011220(float x) {
    return __fabs(x);
}
extern "C" inline float fn_8037F664(float x) {
    return sin(x);
}
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
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}
extern "C" inline float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
extern "C" inline float fn_800F8524(const float* a, const float* b, float t) {
    return t * (*b - *a) + *a;
}
extern "C" inline void fn_8032EC94(int* p) {
    *p = 0;
}
// inferred: camera accessors (weak copies; C names: the method names are unknown)
extern "C" inline void fn_8037F688(atn::Camera* camera, float fovy) {
    camera->mFovy = fovy;
}
extern "C" inline float fn_80385B40(atn::Camera* camera) {
    return camera->mDist;
}

// inferred: update of the side camera mode (see fn_8038BD78): the target follows the target of the character
// (10 further in front while it is in control 14 of state 6), the eye stays beside it out of the walls; with
// the debug option m28C, button 7 gives the default camera
// NONMATCHING: 99.3% - stack slots of the halfway point and of the front vector
void GameCamera::fn_80387548(const CharaTask* chara) {
    if (lbl_8087F0A8->m28C != 0 && lbl_8087F0A8->mInput.fn_801240B4(7)) {
        fn_8037F9AC(chara);
        return;
    }
    m808++;
    atn::Vector3 pos = mPos;
    atn::Vector3 target = mTarget;
    atn::Vector3 up = mUp;
    atn::Vector3 point = chara->mF6C;
    if (lbl_8087F0A8->m040 == 0) {
        point = (chara->mF6C + chara->mPos) * 0.5f;
    }
    if (chara->m55C == 6 && chara->m560 == 14) {
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', chara->mRot.y);
        PSMTXMultVec(m, &front, &front);
        point += front * 10.0f;
    }
    target += (point - target) * m664;
    atn::Vector3 dir = target - chara->mPos;
    PSVECNormalize(&dir, &dir);
    pos = chara->mPos + dir * m65C;
    pos.y += m658;
    float side = m654;
    atn::Vector3 cross;
    PSVECCrossProduct(&dir, &up, &cross);
    pos += cross * side;
    fn_8038E540(chara, &m648, &pos, &target, &chara->mPos);
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
}

// DRAFT: not checked against the original (draft without a description)
// NONMATCHING: draft
void GameCamera::fn_80387910(const CharaTask* chara) {
    GameCameraParam_unk param(m684);
    if (m4B4.fn_803829C0()) {
        m4B4.fn_803829D4();
        param.fn_8037F830(m684, m4C0, m4B4.mTime);
        if (fn_80011220(m4B4.mTime) < 1.1920929e-7f) {
            fn_8032EC94(&m4B4.mActive);
        }
    }
    atn::Vector3 pos(*GetPosition_unk());
    atn::Vector3 target(*GetTarget_unk());
    atn::Vector3 dir = target - pos;
    atn::Vector3 flat(dir);
    flat.y = 0.0f;
    if (fn_80011220(flat.LengthSq_unk()) < 1.1920929e-7f) {
        flat.Set_unk(0.0f, 0.0f, 1.0f);
    } else {
        flat.Normalize_unk();
    }
    float dist = dir.Length_unk();
    float fovy = GetFovy_unk();
    dir.Normalize_unk();
    atn::Vector3 rot = (-dir).ToRotation_unk();
    rot.y = m8A0;
    float delta = -param.m08 - rot.x;
    rot.x += delta;
    pos.y -= dist * fn_8037F664(delta);
    float len = (m8A4 - pos).Length_unk();
    float d = len + (param.m00 - len) * param.m1C;
    target = m8A4;
    m868 = target;
    dir.Set_unk(0.0f, 0.0f, -d);
    dir.fn_80011410(fn_800109E0(-rot));
    pos = target + dir;
    fovy = param.m24 * (param.m04 - fovy) + fovy;
    fn_8038E9B0(chara, &param, &pos, &target, &target);
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 yaw;
    yaw.fn_8013A13C(-m8A0);
    front.fn_80011410(yaw);
    atn::Vector3 toTarget = target - pos;
    if (fn_801C3910(toTarget) < 0.01f) {
        target += front * 0.02f;
    } else {
        toTarget.y = 0.0f;
        toTarget.Normalize_unk();
        if (fn_801A03E8(toTarget, front) < 0.0f) {
            atn::Vector3 t(pos);
            t.y = target.y;
            t += front * 0.02f;
            target = t;
        }
    }
    dir = target - pos;
    dir.y = 0.0f;
    if (fn_80011220(flat.LengthSq_unk()) < 1.1920929e-7f) {
        target.z += 0.1f;
    }
    dist = (target - pos).Length_unk();
    if (dist > 0.1f) {
        float camDist = fn_80385B40(this);
        dist = fn_800F8524(&camDist, &dist, 0.4f);
        float pitch = fn_80383728(this).ToRotation_unk().x;
        atn::Vector3 r = (target - pos).ToRotation_unk();
        r.x = fn_800F8524(&pitch, &r.x, 0.4f);
        atn::Matrix3x4 m = fn_800109E0(r);
        dir = atn::Vector3(0.0f, 0.0f, 1.0f).Transform_unk(m);
        pos = target - dir * dist;
    }
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8037F688(this, fovy);
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: camera mode at m744 looking at the character (m750 above it); with a camera box of size m30
// the fovy is narrowed to keep that size in view
void GameCamera::fn_80388A88(const CharaTask* chara) {
    SetPosition(m744);
    atn::Vector3 offset(0.0f, m750, 0.0f);
    SetTarget_unk(const_cast<CharaTask*>(chara)->fn_80178088() + offset);
    mFovy = m73C;
    if (mC20 != 0 && mC20->m30 > 0) {
        atn::Vector3 d = mPos - mTarget;
        float dist = PSVECMag(&d);
        float fovy = 2.0f * (float)atan((float)mC20->m30 * (float)tan(m73C * 0.5f) / dist);
        mFovy = fovy < m73C ? fovy : m73C;
    }
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: camera mode at a fixed offset of the character looking at a point in front of it
void GameCamera::fn_80389060(const CharaTask* chara) {
    atn::Vector3 target = chara->mPos;
    atn::Vector3 front(0.0f, 0.0f, 10.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &front, &front);
    target += front;
    atn::Vector3 pos = chara->mPos;
    atn::Vector3 offset(0.0f, 35.0f, -20.0f);
    Mtx m2;
    PSMTXRotRad(m2, 'y', chara->mRot.y);
    PSMTXMultVec(m2, &offset, &offset);
    pos += offset;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: camera mode 40 beyond the character, on the line from a point 5 below its own position
void GameCamera::fn_80389210(const CharaTask* chara) {
    atn::Vector3 target = chara->mPos;
    atn::Vector3 offset(0.0f, -5.0f, 0.0f);
    atn::Vector3 from = const_cast<CharaTask*>(chara)->fn_80178088() + offset;
    atn::Vector3 dir = target - from;
    atn::Vector3 d = dir;
    PSVECNormalize(&d, &d);
    SetPosition(target + d * 40.0f);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: camera mode 30 away behind and above the character, along (0.4, 0.2, 1) turned with it
void GameCamera::fn_803893A0(const CharaTask* chara) {
    atn::Vector3 target = chara->mPos;
    target.y += 20.451f;
    atn::Vector3 dir(0.4f, 0.2f, 1.0f);
    PSVECNormalize(&dir, &dir);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    atn::Vector3 d = dir;
    PSMTXMultVec(m, &d, &d);
    d *= 30.0f;
    SetPosition(target - d);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 0);
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: camera mode looking at the hips of the character from 50 above its position
void GameCamera::fn_8038952C(const CharaTask* chara) {
    const atn::Model* model = &chara->mMotion;
    const atn::Matrix3x4* node = (const atn::Matrix3x4*)model->GetNodeMtx_unk(model->GetNodeNoByName("hips", 0));
    atn::Vector3 target = node->GetTrans_unk();
    atn::Vector3 offset(0.0f, 50.0f, 0.0f);
    atn::Vector3 pos = const_cast<CharaTask*>(chara)->fn_80178088() + offset;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 0);
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: fixed camera mode (position m4FC, target m508, fovy m514; turned and moved with the
// character when m518 is set)
void GameCamera::fn_80389678(const CharaTask* chara) {
    atn::Vector3 pos = m4FC;
    atn::Vector3 target = m508;
    if (m518 != 0) {
        Mtx m;
        PSMTXRotRad(m, 'y', chara->mRot.y);
        PSMTXMultVec(m, &pos, &pos);
        PSMTXMultVec(m, &target, &target);
        pos += chara->mPos;
        target += chara->mPos;
    }
    SetPosition(pos);
    SetTarget_unk(target);
    mFovy = m514;
    fn_8004B378();
    fn_80392964(1, 1, 1, 0);
}

void GameCamera::fn_803897EC(const CharaTask* chara) {
    m8F0 = chara->mMotion.mTracks[0].m08 < 67.0f;
    fn_8037F9AC(chara);
    m8F0 = 0;
}

void GameCamera::fn_80389838(int mode) {
    if (m7FC == mode) {
        return;
    }
    m800 = m7FC;
    m7FC = mode;
    m808 = 0;
    m838 = 0;
    m880 = 0;
    mA0C[0] = 0.0f;
    mA0C[1] = 0.0f;
    mA0C[2] = 0.0f;
    mA0C[3] = 0.0f;
    mA0C[4] = 0.0f;
    lbl_8087F0A8->mInput.m10 = 3;
}

// inferred: applies the depth of field of the current camera box (restores the saved one when it has
// none) and sets m804
void GameCamera::fn_80389884(int mode) {
    if (mC20 != 0 && (mC20->m34 & 1U) == 1) {
        if (mC84 == 0) {
            mC24 = lbl_8087EFA8->mDof;
            lbl_8087EFA8->mDof = mC54;
            mC84 = 1;
        }
    } else if (mC84 != 0) {
        lbl_8087EFA8->mDof = mC24;
        mC84 = 0;
    }
    m804 = mode;
}

void GameCamera::fn_8038A1B8(const CharaTask* chara) {
    m8EC = 1;
    m800 = m7FC;
    m834 = 0;
    m7F4 = 0;
    fn_80382A1C(chara);
}

void GameCamera::fn_8038A1D8(const CharaTask* chara) {
    if (m800 == 9) {
        m8EC = 1;
    }
    m800 = m7FC;
    m834 = 0;
}

// inferred: camera at the head of the character (m568 above it) looking m558 in front of it, with the fovy m55C;
// blended in 5 frames from the saved camera
// NONMATCHING: 89.7% - scheduling of the three blends
void GameCamera::fn_8038A200(const CharaTask* chara) {
    atn::Vector3 pos = mPos;
    atn::Vector3 target = mTarget;
    if (m808 == 0) {
        m80C = pos;
        m818 = target;
        m830 = mFovy;
    }
    atn::Vector3 eye = chara->mPos;
    atn::Vector3 offset(0.0f, m568, 0.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &offset, &offset);
    eye += offset;
    offset = atn::Vector3(0.0f, m568, m558);
    PSMTXMultVec(m, &offset, &offset);
    atn::Vector3 look = offset + eye;
    pos = (eye - m80C) * ((float)m808 / 5.0f) + m80C;
    target = (look - m818) * ((float)m808 / 5.0f) + m818;
    SetPosition(pos);
    SetTarget_unk(target);
    mFovy = ((float)m808 / 5.0f) * (m55C - m830) + m830;
    fn_8004B378();
    fn_80392964(1, 1, 0, 0);
    if (++m808 > 5) {
        m80C = pos;
        m818 = target;
        m800 = m7FC;
        m808 = 0;
    }
    m834 = 0;
}

// NONMATCHING: 89.3% - order of the blend temporaries and float registers
void GameCamera::fn_8038AEF0(const atn::Vector3* pos, const atn::Vector3* target, const CharaTask* chara,
                             int collide, float fovy, float t) {
    atn::Vector3 eye = (*pos - m80C) * t + m80C;
    atn::Vector3 look = (*target - m818) * t + m818;
    float angle = t * (fovy - m830) + m830;
    if (collide) {
        fn_8038E540(chara, &m648, &eye, &look, &chara->mPos);
    }
    atn::Vector3 d = look - eye;
    if (d.x * d.x + d.z * d.z < 1.0f) {
        atn::Vector3 h(d.x, 0.0f, d.z);
        if (PSVECSquareMag(&h) < 1.0e-6f) {
            h = atn::Vector3(0.0f, 0.0f, 1.0f);
        } else {
            PSVECNormalize(&h, &h);
        }
        look += h;
    }
    SetPosition(eye);
    SetTarget_unk(look);
    mFovy = angle;
    fn_8004B378();
}

// inferred: side camera mode: from beside the character (m654 to the side, m658 up, m65C behind its target
// mF6C, or halfway to it), blended in 15 frames; then back to the sub mode with m83C = 6
// NONMATCHING: 92.2% - temporaries of the point and the cross product; fn_8008B964 (see fn_80388A88)
void GameCamera::fn_8038BD78(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    atn::Vector3 point = chara->mF6C;
    if (lbl_8087F0A8->m040 == 0) {
        point = (chara->mF6C + chara->mPos) * 0.5f;
    }
    atn::Vector3 target = point;
    atn::Vector3 dir = (target - chara->mPos).GetNormalize();
    atn::Vector3 pos = chara->mPos + dir * m65C;
    pos.y += m658;
    float side = m654;
    atn::Vector3 cross;
    PSVECCrossProduct(&dir, &mUp, &cross);
    pos += cross * side;
    fn_8038AEF0(&pos, &target, chara, 1, m64C, (float)m808 / 15.0f);
    fn_80392964(0, 1, 0, 0);
    if (++m808 > 15) {
        m80C = pos;
        m818 = target;
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

// inferred: low camera beside the character (50 above it, m654 to the side) looking at 12 above it; the target and
// the fovy (m7B4) are blended in 10 frames; then back to the sub mode with m83C = 6. The eye position is
// computed but not used
// NONMATCHING: 48% - here fn_80392964 and its callees (fn_803920C8, fn_803928C0...) are all inlined, while the
// original calls them as in fn_8038BD78
void GameCamera::fn_8038C5B0(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
        m890 = chara->mF6C;
    }
    atn::Vector3 pos = mPos;
    atn::Vector3 target = mTarget;
    atn::Vector3 up = mUp;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &front, &front);
    front = Negate_unk(front);
    pos = chara->mPos;
    pos.y += 50.0f;
    float side = m654;
    atn::Vector3 cross;
    PSVECCrossProduct(&front, &up, &cross);
    float t = MIN((float)m808 / 10.0f, 1.0f);
    pos += cross * side;
    target = chara->mPos;
    target.y += 12.0f;
    target = (target - m818) * t + m818;
    SetTarget_unk(target);
    mFovy = MIN((float)m808 / 10.0f, 1.0f) * (m7B4 - m830) + m830;
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
    if (++m808 > 10) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

extern "C" double cos(double x);

// inferred: event camera at m780 from the head of the character (turned by m79C degrees, facing it), looking
// m774 away with the pitch m77C and the fovy m778; blended with an eased curve while the timer m424 runs
// NONMATCHING: 91.5% - scheduling of the front vector and of the blends; fn_8008B964 (see fn_80388A88)
void GameCamera::fn_8038CD94(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    float yaw = 0.017453292f * m79C;
    atn::Vector3 offset(m780.x, m780.y, m780.z);
    Mtx m;
    PSMTXRotRad(m, 'y', 3.1415927f + yaw);
    PSMTXMultVec(m, &offset, &offset);
    atn::Vector3 head = const_cast<CharaTask*>(chara)->fn_80178088();
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Vector3 pos = head + offset;
    Mtx m2;
    PSMTXRotRad(m2, 'x', -m77C);
    PSMTXMultVec(m2, &front, &front);
    Mtx m3;
    PSMTXRotRad(m3, 'y', 3.1415927f + yaw);
    PSMTXMultVec(m3, &front, &front);
    atn::Vector3 target = pos + front * m774;
    float t = 1.0f;
    if (m424.fn_803829C0()) {
        m424.fn_803829D4();
        t = (1.0f - (float)cos(3.1415927f * (1.0f - m424.mTime))) * 0.5f;
        if (m424.mTime < m424.mStep) {
            m424.mActive = 0;
        }
    }
    SetPosition((pos - m80C) * t + m80C);
    SetTarget_unk((target - m818) * t + m818);
    mFovy = t * (m778 - m830) + m830;
    fn_8004B378();
    fn_80392964(0, 1, 1, 1);
    m808++;
    if (t >= 1.0f) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

// inferred: blends in 30 frames from the saved camera to the camera of fn_80389060 (eye 35 up and 20 behind
// the character, looking 10 in front of it); then back to the sub mode with m83C = 6
// NONMATCHING: 87.8% - scheduling of the two blends (the angle of the rotation is loaded before the matrix)
void GameCamera::fn_8038D1C0(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    atn::Vector3 target = chara->mPos;
    atn::Vector3 front(0.0f, 0.0f, 10.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &front, &front);
    target += front;
    target = (target - m818) * ((float)m808 / 30.0f) + m818;
    atn::Vector3 pos = chara->mPos;
    atn::Vector3 offset(0.0f, 35.0f, -20.0f);
    Mtx m2;
    PSMTXRotRad(m2, 'y', chara->mRot.y);
    PSMTXMultVec(m2, &offset, &offset);
    pos += offset;
    pos = (pos - m80C) * ((float)m808 / 30.0f) + m80C;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 0, 0);
    if (++m808 > 30) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

// inferred: blends in 10 frames to a camera 40 behind the target (the character) along the line from the point
// 5 under its head; then back to the sub mode with m83C = 6
// NONMATCHING: 80.6% - the original adds the offset to the head after the first blend (more float registers)
void GameCamera::fn_8038D4D4(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    atn::Vector3 target = chara->mPos;
    atn::Vector3 offset(0.0f, -5.0f, 0.0f);
    atn::Vector3 head = const_cast<CharaTask*>(chara)->fn_80178088() + offset;
    target = (target - m818) * ((float)m808 / 10.0f) + m818;
    atn::Vector3 dir = (target - head).GetNormalize();
    atn::Vector3 pos = target + dir * 40.0f;
    pos = (pos - m80C) * ((float)m808 / 10.0f) + m80C;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 0, 0);
    if (++m808 > 10) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

// inferred: blends in 23 frames to a camera 30 away from the target (20.451 above the character) along a
// fixed direction turned with the character; then back to the sub mode with m83C = 6
// NONMATCHING: 84% - order of the two blends and temporaries of the direction
void GameCamera::fn_8038D854(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m824 = m818 - m80C;
        m830 = mFovy;
    }
    atn::Vector3 target = chara->mPos;
    target.y += 20.451f;
    atn::Vector3 dir = atn::Vector3(0.4f, 0.2f, 1.0f).GetNormalize();
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    atn::Vector3 back = dir;
    PSMTXMultVec(m, &back, &back);
    back *= 30.0f;
    atn::Vector3 pos = target - back;
    target = (target - m818) * ((float)m808 / 23.0f) + m818;
    pos = (pos - m80C) * ((float)m808 / 23.0f) + m80C;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 0, 0);
    if (++m808 > 23) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: one frame of the "hips" camera after saving the current camera in m80C-m830; then goes back
// to the sub mode with m83C = 6
void GameCamera::fn_8038DBE4(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m824 = m818 - m80C;
        m830 = mFovy;
    }
    const atn::Model* model = &chara->mMotion;
    const atn::Matrix3x4* node = (const atn::Matrix3x4*)model->GetNodeMtx_unk(model->GetNodeNoByName("hips", 0));
    atn::Vector3 target = node->GetTrans_unk();
    atn::Vector3 offset(0.0f, 50.0f, 0.0f);
    atn::Vector3 pos = const_cast<CharaTask*>(chara)->fn_80178088() + offset;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 0, 0);
    if (++m808 > 1) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

// NONMATCHING: inlines fn_80389678 (and through it fn_80392964, see there)
// inferred: fixed camera mode that keeps the current sub mode as the previous one
void GameCamera::fn_8038DDE0(const CharaTask* chara) {
    m800 = m7FC;
    fn_80389678(chara);
}

// NONMATCHING: fn_80392964 is inlined raw as in the original, but the getter fn_8008B964 (extern "C" in
// atn/Model.cpp) is called where the original inlines it; other differences of its own
// inferred: blends over 30 frames from the camera saved on the first frame to the default mode
void GameCamera::fn_8038DF5C(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    m834 = 0;
    m8F0 = 1;
    fn_8037F9AC(chara);
    m8F0 = 0;
    float t = (float)m808 / 30.0f;
    float k = 0.5f * (1.0f + (float)sin(3.1415927f * t - 1.5707964f));
    atn::Vector3 pos = (mPos - m80C) * k + m80C;
    atn::Vector3 target = (mTarget - m818) * k + m818;
    SetPosition(pos);
    SetTarget_unk(target);
    mFovy = k * (mFovy - m830) + m830;
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
    if (++m808 >= 30) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
}

extern "C" GameTask* lbl_8087F430; // inferred: the field
extern "C" void* lbl_8087EE98;     // inferred: collision manager
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// inferred: keeps the eye out of the walls: on maps with ceilings (MapTask::mEC) the eye is put under the ceiling
// above it, then a ray from the character to the eye (at the same height) pulls it in front of the wall; while it
// keeps hitting (m89C, up to 8 frames) the eye is blended back towards the current camera position
// NONMATCHING: 92.4% - order of the first loads (radius, top) and stack slots of the ray ends
void GameCamera::fn_8038E540(const CharaTask* chara, int* state, atn::Vector3* pos, atn::Vector3* target,
                             const atn::Vector3* charaPos) {
    float radius = lbl_8087F0A8->m08C;
    float top = 50.0f + pos->y;
    float height = 30.0f;
    CollResult_unk result;
    bool map = false;
    bool ceiling = false;
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
        map = true;
    }
    if (map && lbl_8087F430->GetCurrentMap()->mEC != 0) {
        ceiling = true;
    }
    if (ceiling) {
        atn::Vector3 from = *charaPos;
        atn::Vector3 to = *charaPos;
        to.y = 2.0f + pos->y;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x08000000, 0, 0)) {
            atn::Vector3 p = *pos;
            p.y = result.m10.y - 2.0f;
            *pos = p;
            height = MIN(height, pos->y - charaPos->y);
            top = MIN(top, pos->y);
        }
    }
    atn::Vector3 from = *charaPos;
    atn::Vector3 to = *pos;
    from.y += height;
    to.y = from.y;
    atn::Vector3 dir = (to - from).GetNormalize();
    to += dir * radius;
    unsigned int mask = 0x08000000;
    bool map2 = false;
    bool ceiling2 = false;
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
        map2 = true;
    }
    if (map2 && lbl_8087F430->GetCurrentMap()->mEC != 0) {
        ceiling2 = true;
    }
    if (!ceiling2) {
        mask = 0x80000008;
    }
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, mask, 0, 0)) {
        atn::Vector3 hit = result.m10 - dir * radius;
        hit.y = pos->y;
        atn::Vector3 d = hit - *pos;
        hit.y += MIN(20.0f, PSVECMag(&d));
        hit.y = MIN(top, hit.y);
        *pos = hit;
        m89C++;
    } else {
        m89C--;
    }
    m89C = MIN(8, MAX(0, m89C));
    if (m89C > 0) {
        float k = 0.5f * ((float)m89C * 0.125f);
        *pos = (mPos - *pos) * k + *pos;
    }
}

void GameCamera::fn_80390374() {
    fn_80389838(m804);
}

#include "game/SetConfig.h"

// inferred: xsca camera curves (as in game/NewCampTask.cpp and game/Em309Ghoul.cpp)
struct XsfCurveSet_unk {
    void fn_805BF410(float time, float* out, int x, int y, int z);
    unsigned char m00[0x14];
    float mLength; // 0x14
};
struct Xsca_unk {
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
};

// inferred: ambient sway of the camera m1F4 played from the xsca of the map (m8F4, frame m8F8, scale m8FC;
// off unless bit 0x10 of the SetConfig flags is set): channels 4-6 turn the view and the up vector, channels
// 1-3 move the eye in the yaw of the view
// NONMATCHING: 97.1% - stack slots of the direction temporaries and float registers of the scaled angles
void GameCamera::fn_803918EC() {
    if (m8F4 == 0) {
        return;
    }
    atn::Vector3 pos = m1F4.mPos;
    atn::Vector3 dir = (m1F4.mTarget - m1F4.mPos).GetNormalize();
    atn::Vector3 up = m1F4.mUp;
    float dist = m1F4.mDist;
    float scale = m8FC;
    if (SetConfig::sInstance != 0 && ((unsigned int)SetConfig::sInstance->m50 & 0x10) != 0x10) {
        scale = 0.0f;
    }
    XsfCurveSet_unk* curve = ((Xsca_unk*)m8F4)->fn_805BDCBC(0);
    if (m8F8 >= curve->mLength) {
        m8F8 = 0.0f;
    }
    atn::Vector3 angle;
    curve->fn_805BF410(m8F8, &angle.x, 4, 5, 6);
    angle.x = 0.017453292f * angle.x * scale;
    angle.y = 0.017453292f * angle.y * scale;
    angle.z = 0.017453292f * angle.z * scale;
    atn::Matrix3x4 m = fn_800109E0(angle);
    PSMTXMultVec(m.m, &up, &up);
    atn::Vector3 view = dir.ToRotationZ_unk();
    angle += view;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m2 = fn_800109E0(atn::Vector3(angle.x, angle.y, 0.0f));
    PSMTXMultVec(m2.m, &front, &front);
    dir = front.GetNormalize();
    atn::Vector3 move;
    curve->fn_805BF410(m8F8, &move.x, 1, 2, 3);
    move.x *= scale;
    move.y *= scale;
    move.z *= scale;
    Mtx r;
    PSMTXRotRad(r, 'y', angle.y);
    PSMTXMultVec(r, &move, &move);
    pos += move;
    atn::Vector3 target = pos + dir * dist;
    m1F4.mPos = pos;
    m1F4.mTarget = target;
    m1F4.mUp = up;
    m8F8 += 1.0f;
}

// inferred: shakes the target of m1F4 while mShake runs: the offset of the shake turned with the yaw of the
// direction of view
// NONMATCHING: 93.4% - order of the loads of the shake frame/length and float registers of the inlined
// GetOffset_unk
void GameCamera::fn_803920C8() {
    if (mShake.mFrame >= mShake.mLength) {
        return;
    }
    atn::Vector3 target = m1F4.mTarget;
    atn::Vector3 norm = (m1F4.mTarget - m1F4.mPos).GetNormalize();
    atn::Vector3 offset = mShake.GetOffset_unk();
    atn::Vector3 rot = norm.ToRotationZ_unk();
    Mtx m;
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &offset, &offset);
    target += offset;
    m1F4.mTarget = target;
}


// NONMATCHING: 95.9% - stack slots of the vector temporaries and the copy of the target through a
// register-held temporary
// inferred: blends the target and the fovy of m1F4 towards m91C/m934 over m918 frames (eased with a
// cosine; from m928/m938 when m910 and m911 are set, else from the current camera)
void GameCamera::fn_80392510() {
    float k = 0.5f * (1.0f - (float)cos(3.1415927f * ((((float)m914 / (float)m918 > 0.0f ? (float)m914 / (float)m918 : 0.0f) < 1.0f) ? ((float)m914 / (float)m918 > 0.0f ? (float)m914 / (float)m918 : 0.0f) : 1.0f)));
    if (m910 != 0 && m911 != 0) {
        atn::Vector3 d = m91C - m928;
        m928 = d * k + m928;
    } else {
        atn::Vector3 d = m91C - m1F4.mTarget;
        m928 = d * k + m1F4.mTarget;
    }
    atn::Vector3 target = m928;
    m1F4.SetTarget_unk(target);
    float fovy = m1F4.mFovy;
    float k2 = 0.5f * (1.0f - (float)cos(3.1415927f * ((((float)m914 / (float)m918 > 0.0f ? (float)m914 / (float)m918 : 0.0f) < 1.0f) ? ((float)m914 / (float)m918 > 0.0f ? (float)m914 / (float)m918 : 0.0f) : 1.0f)));
    if (m910 != 0 && m911 != 0) {
        if (m934 > 0.0f) {
            m938 = k2 * (m934 - m938) + m938;
        } else {
            m938 = k2 * (fovy - m938) + m938;
        }
    } else if (m934 > 0.0f) {
        m938 = k2 * (m934 - fovy) + fovy;
    } else {
        m938 = fovy;
    }
    m1F4.mFovy = m938;
}

// inferred: const (the original loads every member before the first store)
void GameCamera::fn_803928C0() {
    m1F4.mPos = m1F4.mPos + m93C;
    m1F4.mTarget = m1F4.mTarget + m948;
    float fovy = m1F4.mFovy;
    m1F4.mFovy = m954 + fovy;
}

// inferred: copies a camera (also called by game/TutorialTask)
extern "C" void fn_80392A04(atn::Camera* dst, const atn::Camera* src);

// NONMATCHING: the original is over the inlining budget and calls fn_80392A04 and RenderManager::SetCamera_unk;
// here both are inlined
void GameCamera::fn_80392964(int a, int b, int c, int d) {
    fn_80392A04(&m1F4, this);
    if (c != 0) {
        fn_80392510();
    }
    if (d != 0) {
        fn_80392FE8();
    }
    if (a != 0) {
        fn_803918EC();
    }
    if (b != 0) {
        fn_803920C8();
    }
    fn_803928C0();
    m1F4.fn_8004B378();
    fn_8008B964()->SetCamera_unk(m1F4);
}

// NONMATCHING: 98% - registers and order of the copy of the frustum planes
extern "C" void fn_80392A04(atn::Camera* dst, const atn::Camera* src) {
    *dst = *src;
}

void GameCamera::fn_80392C94(GameCameraState_unk* state) const {
    state->m0C = m80C;
    state->mMode = m7FC;
    state->mPrevMode = m800;
    state->m08 = m808;
    state->m18 = m818;
    state->m24 = m830;
}

void GameCamera::fn_80392CE0(const GameCameraState_unk* state) {
    m80C = state->m0C;
    m7FC = state->mMode;
    m800 = state->mPrevMode;
    m808 = state->m08;
    m818 = state->m18;
    m830 = state->m24;
}

// NONMATCHING: 98.8% - the copy of the camera, as fn_80392A04
// inferred: makes the render camera blend back from `from` (weight 1, decreased by `rate` every frame);
// called with the camera of the field (EnemyQuarkStand, GameEventScript)
extern "C" void fn_80392D2C(GameCamera* camera, const atn::Camera* from, float rate) {
    camera->mA20 = *from;
    camera->mC14 = 1.0f;
    camera->mC18 = rate;
}

// NONMATCHING: 83% - stack slots of the (a - b) and (a - b) * t temporaries swapped and the float registers
// of the three lerps (tried statement orders, Lerp_unk, named differences and a static inline helper)
// inferred: blends the render camera from the saved one (mA20) while mC14 is positive
void GameCamera::fn_80392FE8() {
    if (mC14 > 0.0f) {
        float t = mC14;
        atn::Vector3 pos = (mA20.mPos - m1F4.mPos) * t + m1F4.mPos;
        atn::Vector3 target = (mA20.mTarget - m1F4.mTarget) * t + m1F4.mTarget;
        atn::Vector3 up = (mA20.mUp - m1F4.mUp) * t + m1F4.mUp;
        float fovy = t * (mA20.mFovy - m1F4.mFovy) + m1F4.mFovy;
        up.Normalize_unk();
        m1F4.mPos = pos;
        m1F4.mTarget = target;
        m1F4.mUp = up;
        m1F4.mFovy = fovy;
        mC14 -= mC18;
    }
}

// inferred: debug display of the camera (position, target and the point of the tactical view)
// NONMATCHING: only the offsets of the two formats in the file pool differ (0x1F0/0x20D in the original:
// the strings of the mode functions before them are not written yet)
void GameCamera::fn_8039328C() {
    if (m7F0 != 0) {
        char buf[256];
        atn::DrawHelper* draw = lbl_8087EEB0;
        sprintf(buf, "pos    (%4.2f, %4.2f, %4.2f)", mPos.x, mPos.y, mPos.z);
        draw->fn_800616C0(40.0f, 20.0f, 0.0f, 16.0f, 16.0f, buf, 0xFFFFFFFF, 0.0f, 0, 0, 0);
        sprintf(buf, "target (%4.2f, %4.2f, %4.2f)", mTarget.x, mTarget.y, mTarget.z);
        draw->fn_800616C0(40.0f, 36.0f, 0.0f, 16.0f, 16.0f, buf, 0xFFFFFFFF, 0.0f, 0, 0, 0);
        if (m7FC == 6) {
            draw->fn_80063D3C(m8A4, lbl_8087F0A8->m08C, 0.0f, 0xFFFFFF00);
        }
    }
}

void GameCamera::fn_803933A4() {
    m834 = 0;
    m910 = 0;
    m914 = 0;
    m858 = 0;
    m838 = 0;
    m46C.mMode = 0;
    mShake.mFrame = mShake.mFrame % 2;
    mShake.mLength = 0;
    mShake.mWidth = 0.0f;
    mShake.mHeight = 0.0f;
    mC20 = 0;
    mC84 = 0;
}

extern "C" int rand();


// inferred: random offset of the shake, mirrored every other frame and fading out
atn::Vector3 CameraShake_unk::GetOffset_unk() const {
    atn::Vector3 v(2.0f * mWidth * ((float)(rand() % 1001) / 1000.0f) - mWidth,
                   0.7f * mHeight + 0.3f * mHeight * ((float)(rand() % 1001) / 1000.0f), 0.0f);
    if (mFrame % 2 == 0) {
        v = Negate_unk(v);
    }
    float t = (float)(mLength - mFrame) / (float)mLength;
    v.x *= t;
    v.y *= t;
    v.z *= t;
    return v;
}
