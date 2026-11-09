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

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" RenderSetting_unk* lbl_8087EFA8;
extern "C" int sprintf(char* buf, const char* fmt, ...);

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
    if (m900 < m904) {
        m900++;
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

void GameCamera::fn_80390374() {
    fn_80389838(m804);
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

// NONMATCHING: 83% - stack slots of the (a - b) and (a - b) * t temporaries swapped and the float registers
// of the three lerps (tried statement orders, Lerp_unk, named differences and a static inline helper)
// inferred: blends the render camera from the saved one (mA28) while mC14 is positive
void GameCamera::fn_80392FE8() {
    if (mC14 > 0.0f) {
        float t = mC14;
        atn::Vector3 pos = (mA28 - m1F4.mPos) * t + m1F4.mPos;
        atn::Vector3 target = (mA34 - m1F4.mTarget) * t + m1F4.mTarget;
        atn::Vector3 up = (mA40 - m1F4.mUp) * t + m1F4.mUp;
        float fovy = t * (mA70 - m1F4.mFovy) + m1F4.mFovy;
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
    m46C = 0;
    m900 = m900 % 2;
    m904 = 0;
    m908 = 0.0f;
    m90C = 0.0f;
    mC20 = 0;
    mC84 = 0;
}

extern "C" int rand();

// inferred: shake of the camera (GameCamera::m900-m90C: frame, length, width and height; names invented;
// the caller is not written yet, the type should move to GameCamera.h with it)
struct CameraShake_unk {
    atn::Vector3 GetOffset_unk() const;
    int mFrame;
    int mLength;
    float mWidth;
    float mHeight;
};

static inline atn::Vector3 Negate_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

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
