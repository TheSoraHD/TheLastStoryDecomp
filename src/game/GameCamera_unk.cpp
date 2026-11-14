// Camera of the field (GameCamera, GameTask + 0x6C). Two stick modes (0x80384188, 0x80385B48) are still missing,
// so the unit is not linked.
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
#include "chr/CharaEquip_unk.h"
#include "math/VecAssign_unk.h"
#include "game/SetConfig.h"
#include "game/HitInfo_unk.h"
#include "game/ChaseManager.h"
#include "game/Input_unk.h"
#include "chr/ControlBase.h"
#include "game/TacView.h"
#include "game/BattleTask.h"
#include "atn/Csv.h"
#include "atn/GraphicsDevice.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" RenderSetting_unk* lbl_8087EFA8;
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" GameTask* lbl_8087F430; // inferred: the field
extern "C" void* lbl_8087EE98;     // inferred: collision manager
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// inferred: limits of the side offset of the ledge camera without blend (fn_8038378C; 0.0f)
extern "C" float lbl_8087DCF0;
extern "C" float lbl_8087DCF4;
// inferred: height of the side camera during the control 11 (50.0f)
extern "C" float lbl_8087DCF8;
// inferred: limits of the side offset of the ledge camera at the ends of the ledge (0.0f; .sdata of this
// unit: a global float initialized to zero only stays in .sdata with explicit_zero_data)
extern "C" float lbl_8087DCFC;
extern "C" float lbl_8087DD00;
// inferred: ends of the blends of the special motions of fn_8038F52C (.sdata of this unit: 0.0f, 0.0f, -6.0f,
// -6.0f, 0.4f, 1.0f, 0.8f, 0.4f, 0.0f, 1.0f, 0.8f, 0.6f, 1.0f)
extern "C" float lbl_8087DD04;
extern "C" float lbl_8087DD08;
extern "C" float lbl_8087DD0C;
extern "C" float lbl_8087DD10;
extern "C" float lbl_8087DD14;
extern "C" float lbl_8087DD18;
extern "C" float lbl_8087DD1C;
extern "C" float lbl_8087DD20;
extern "C" float lbl_8087DD24;
extern "C" float lbl_8087DD28;
extern "C" float lbl_8087DD2C;
extern "C" float lbl_8087DD30;
extern "C" float lbl_8087DD34;
class TownTask;
extern "C" TownTask* lbl_8087FA20; // inferred: town task (game/TownTask.h)
// .sdata of this unit: lbl_8087DCF0..lbl_8087DD00 are global floats (0.0f except lbl_8087DCF8); a zero
// initializer goes to .sbss without explicit_zero_data, so they stay extern for now

extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" double tan(double x);
extern "C" double atan(double x);
extern "C" double sqrt(double x);

// inferred: rotation matrix z, then y, then x (weak copy; as in game/Em309Ghoul.cpp)
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

static inline atn::Vector3 Negate_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: cross product a x b (name invented)
static inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: constructors of the parts of the camera (inline; defined here, their only user)
inline GameCameraTimer_unk::GameCameraTimer_unk() : mActive(0) {}
inline GameCameraParam_unk::GameCameraParam_unk() : m34(0.0f) {}
inline GameCameraModeParam_unk::GameCameraModeParam_unk() : mMode(0) {}
inline CameraShake_unk::CameraShake_unk() : mFrame(0), mLength(0), mWidth(0.0f), mHeight(0.0f) {}

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0; // inferred: graphics device
extern "C" float fn_80076FF8(atn::GraphicsDevice_unk* device); // inferred: aspect ratio of the screen

// inferred: builds the three cameras and the parameters, then resets the camera (fn_8037DD00) and registers its
// debug menu
GameCamera::GameCamera()
    : m7F0(0), m7F4(0), m7FC(0), m804(0), m808(0), m834(0), m838(0), m83C(0), m840(0.0f, 0.0f, 0.0f),
      m84C(0.0f, 0.0f, 0.0f), m858(0), m880(0), m884(0.0f, 0.0f, 0.0f), m890(0.0f, 0.0f, 0.0f), m89C(0),
      m8A0(3.1415927f), m8A4(0.0f, 0.0f, 0.0f), m8B0(0.0f, 0.0f, 0.0f), m8BC(0.0f, 0.0f, 0.0f), m8C8(0.0f), m8CC(0),
      m8D0(-1), m8D4(0), m8D8(0.03f), m8DC(16.0f), m8E0(1), m8E4(0), m8E8(0), m8EC(0), m8F0(0), m8F4(0), m8F8(0.0f),
      m8FC(1.0f), m910(0), m911(0), m914(0), m918(15), m91C(0.0f, 0.0f, 0.0f), m928(0.0f, 0.0f, 0.0f),
      m93C(0.0f, 0.0f, 0.0f), m948(0.0f, 0.0f, 0.0f), m954(0.0f), m958(0.0f), m95C(1.0f), m96C(0.0f), m970(0.0f),
      m974(0.0f), m984(0), m988(0.0f), m9EC(0.25f), m9F0(10), m9F4(1), mA04(0.6981317f), mA08(15), mA0C(0.0f),
      mA10(0.0f), mA14(0.0f), mA18(0.0f), mA1C(0.0f), mC14(-1.0f), mC1C(0.0f), mC20(0), mC84(0), mCA0(0.0f, 0.0f),
      mCA8(1.0f) {
    mAspect = fn_80076FF8(lbl_8087EEE0);
    m800 = m7FC;
    memset(&m4FC, 0, 0x20);
    fn_8037DD00();
    fn_8037E9E0();
    mC54.mStart = 400.0f;
    mC54.mEnd = 500.0f;
    mC54.mNearStart = 30.0f;
    mC54.mNearEnd = 10.0f;
    for (int i = 0; i < 3; i++) {
        mC88[i].x = mC88[i].y = 0.0f;
    }
    for (int i = 0; i < 8; i++) {
        m98C[i].x = m98C[i].y = m98C[i].z = 0.0f;
    }
}

// NONMATCHING: 99.97% - the original destroys the camera at 0x00 with -1, as a member and not as the base class
// (GameCamera would hold an atn::Camera at 0x00 instead of deriving from it)
GameCamera::~GameCamera() {}

// inferred: default parameters of the camera modes (also called by the constructor): the box camera (m738) and the
// event camera (m774) are copies of m51C, m6FC a copy of m6C0 (their m34 is copied, not set)
void GameCamera::fn_8037DD00() {
    m51C.m00 = 32.0f;
    m51C.m04 = 1.134464f;
    m51C.m08 = 0.08726646f;
    m51C.m0C = 5.0f;
    m51C.m10 = 16.0f;
    m51C.m14 = 0.0f;
    m51C.m18 = 0.0f;
    m51C.m1C = 0.4f;
    m51C.m20 = 0.3f;
    m51C.m24 = 0.1f;
    m51C.m28 = 0.3f;
    m51C.m2C = 0.03f;
    m51C.m30 = 0.04f;
    m51C.m38 = 1;
    m558.m00 = 50.0f;
    m558.m04 = 1.0471976f;
    m558.m08 = 0.0f;
    m558.m0C = 0.0f;
    m558.m10 = 15.0f;
    m558.m14 = 0.0f;
    m558.m18 = 0.0f;
    m558.m1C = 0.05f;
    m558.m20 = 0.0f;
    m558.m24 = 0.1f;
    m558.m28 = 0.0f;
    m558.m2C = 0.0f;
    m558.m30 = 0.0f;
    m558.m38 = 0;
    m594.m00 = 15.0f;
    m594.m04 = 0.7853982f;
    m594.m08 = 0.0f;
    m594.m0C = 5.0f;
    m594.m10 = 15.0f;
    m594.m14 = 0.0f;
    m594.m18 = 5.0f;
    m594.m1C = 0.4f;
    m594.m20 = 0.3f;
    m594.m24 = 0.1f;
    m594.m28 = 0.3f;
    m594.m2C = 0.01f;
    m594.m30 = 0.0f;
    m594.m38 = 1;
    m5D0.m00 = 50.0f;
    m5D0.m04 = 0.7853982f;
    m5D0.m08 = 0.0f;
    m5D0.m0C = 4.0f;
    m5D0.m10 = 6.5f;
    m5D0.m14 = -6.0f;
    m5D0.m18 = 0.0f;
    m5D0.m1C = 0.05f;
    m5D0.m20 = 0.0f;
    m5D0.m24 = 0.1f;
    m5D0.m28 = 0.0f;
    m5D0.m2C = 0.01f;
    m5D0.m30 = 0.0f;
    m5D0.m38 = 1;
    m60C.m00 = 50.0f;
    m60C.m04 = 0.7853982f;
    m60C.m08 = 0.0f;
    m60C.m0C = 4.0f;
    m60C.m10 = 18.0f;
    m60C.m14 = -10.0f;
    m60C.m18 = 7.0f;
    m60C.m1C = 0.05f;
    m60C.m20 = 0.0f;
    m60C.m24 = 0.1f;
    m60C.m28 = 0.0f;
    m60C.m2C = 0.01f;
    m60C.m30 = 0.0f;
    m60C.m38 = 1;
    m648.m00 = 20.0f;
    m648.m04 = 0.87266463f;
    m648.m08 = 0.0f;
    m648.m0C = 8.0f;
    m648.m10 = 60.0f;
    m648.m14 = -15.0f;
    m648.m18 = 40.0f;
    m648.m1C = 0.1f;
    m648.m20 = 0.0f;
    m648.m24 = 0.0f;
    m648.m28 = 0.0f;
    m648.m2C = 0.01f;
    m648.m30 = 0.0f;
    m648.m38 = 1;
    m684.m00 = 120.0f;
    m684.m04 = 1.0471976f;
    m684.m08 = 1.2217305f;
    m684.m0C = 0.0f;
    m684.m10 = 0.0f;
    m684.m14 = 0.0f;
    m684.m18 = 0.0f;
    m684.m1C = 0.4f;
    m684.m20 = 0.3f;
    m684.m24 = 0.1f;
    m684.m28 = 0.3f;
    m684.m2C = 0.01f;
    m684.m30 = 0.0f;
    m684.m38 = 0;
    m7B0.m0C = 0.0f;
    m7B0.m10 = 30.0f;
    m7B0.m14 = 20.0f;
    m7B0.m04 = 1.134464f;
    m6C0.m00 = 38.0f;
    m6C0.m04 = 1.134464f;
    m6C0.m08 = 0.34906584f;
    m6C0.m0C = 5.0f;
    m6C0.m10 = 16.0f;
    m6C0.m14 = 0.0f;
    m6C0.m18 = 0.0f;
    m6C0.m1C = 0.4f;
    m6C0.m20 = 0.3f;
    m6C0.m24 = 0.1f;
    m6C0.m28 = 0.3f;
    m6C0.m2C = 0.03f;
    m6C0.m30 = 0.04f;
    m6C0.m38 = 1;
    m6FC = m6C0;
    m6FC.m0C = 0.0f;
    m6FC.m2C = 0.1f;
    m738 = m51C;
    m738.m0C = 0.0f;
    m738.m10 = 30.0f;
    m738.m14 = 0.0f;
    m738.m18 = 16.0f;
    m774 = m51C;
    m774.m28 = 0.0f;
    m8F4 = 0;
    m8FC = 1.0f;
    m8F8 = 0.0f;
    m424.mActive = 0;
}

extern "C" int atoi(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: reads the camera csv of the map (lines "camera, gun|sword, mode, 14 parameters"): the lines of `camera`
// set the parameters of their mode (fn_8038EF94; the default mode also reads the two lock-on lengths of the player,
// 60 added); "fixRotY" blends into the fixed camera over 30 frames (none on the first map load), otherwise over
// `frames` frames from the parameters of the hips mode
// NONMATCHING: 66% - the original copies the player settings of GameConfig_unk (0x244-0x333, a struct there,
// separate members here) into a local and back for the two lock-on lengths (0xF0 more bytes of stack)
void GameCamera::fn_8037E12C(void* data, int size, int camera, int frames) {
    GameCameraParam_unk base(m51C);
    atn::Csv csv((const char*)data, size);
    while (csv.fn_8005BBF8()) {
        if (camera != atoi(csv.fn_8005B9CC())) {
            continue;
        }
        const char* s = csv.fn_8005B9CC();
        int weapon = -1;
        int mode = -1;
        if (strcmp(s, "gun") == 0) {
            weapon = -1;
        } else if (strcmp(s, "sword") == 0) {
            weapon = 1;
        }
        if (weapon < 0) {
            continue;
        }
        s = csv.fn_8005B9CC();
        if (strcmp(s, "default") == 0) {
            mode = 0;
        } else if (strcmp(s, "hide") == 0) {
            mode = 2;
        } else if (strcmp(s, "shoot") == 0) {
            mode = 3;
        } else if (strcmp(s, "crouch") == 0) {
            mode = 4;
        } else if (strcmp(s, "control") == 0) {
            mode = 1;
        } else if (strcmp(s, "chant") == 0) {
            mode = 5;
        } else if (strcmp(s, "birdView") == 0) {
            mode = 6;
        } else if (strcmp(s, "chase") == 0) {
            mode = 8;
        } else if (strcmp(s, "chaseRear") == 0) {
            mode = 9;
        } else if (strcmp(s, "fix") == 0) {
            mode = 10;
        } else if (strcmp(s, "fixRotY") == 0) {
            mode = 11;
        }
        if (mode < 0) {
            continue;
        }
        GameCameraParam_unk param;
        param.m00 = fn_800DC288(csv.fn_8005B9CC());
        param.m04 = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
        param.m08 = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
        param.m0C = fn_800DC288(csv.fn_8005B9CC());
        param.m10 = fn_800DC288(csv.fn_8005B9CC());
        param.m14 = fn_800DC288(csv.fn_8005B9CC());
        param.m18 = fn_800DC288(csv.fn_8005B9CC());
        param.m1C = fn_800DC288(csv.fn_8005B9CC());
        param.m20 = fn_800DC288(csv.fn_8005B9CC());
        param.m24 = fn_800DC288(csv.fn_8005B9CC());
        param.m28 = fn_800DC288(csv.fn_8005B9CC());
        param.m2C = fn_800DC288(csv.fn_8005B9CC());
        param.m30 = fn_800DC288(csv.fn_8005B9CC());
        param.m34 = fn_800DC288(csv.fn_8005B9CC());
        param.m38 = mode != 6;
        fn_8038EF94(&param, mode);
        if (mode == 0) {
            lbl_8087F0A8->mLockOnLen = 60.0f + fn_800DC288(csv.fn_8005B9CC());
            lbl_8087F0A8->mLockOnReleaseLen = 60.0f + fn_800DC288(csv.fn_8005B9CC());
        }
        if (mode == 11) {
            frames = 30;
            if (lbl_8087F430 != 0 && lbl_8087F430->m54EC == 0) {
                frames = 0;
            }
            m424.mActive = 0;
        }
        if (m7FC != mode) {
            m424.mActive = 0;
        }
    }
    if (frames > 0 && m424.mActive != 1 && m424.mActive != 4) {
        m424.mActive = 1;
        m430 = base;
        m424.mStep = 1.0f / (float)frames;
        m424.mTime = 1.0f;
    }
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
extern "C" void fn_80087994(void* group, const char* name, int* flag, int arg3, int arg4); // inferred: switch item
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step, int arg6,
                            int arg7); // inferred: integer item
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step); // inferred: vector item

// inferred: debug menu of the camera: the parameters of every mode and the global settings
void GameCamera::fn_8037E9E0() {
    void* group = m7EC.Load("GameCamera");
    fn_8037ECF0("Default", group, &m51C);
    fn_8037ECF0("Hide", group, &m594);
    fn_8037ECF0("Shoot", group, &m5D0);
    fn_8037ECF0("Crouch", group, &m60C);
    fn_8037ECF0("Control", group, &m558);
    fn_8037ECF0("Chant", group, &m648);
    fn_8037ECF0("BirdView", group, &m684);
    fn_8037ECF0("WallUp", group, &m7B0);
    fn_8037ECF0("Chase", group, &m6C0);
    fn_8037ECF0("ChaseRear", group, &m6FC);
    fn_8037ECF0("Fix", group, &m738);
    fn_8037ECF0("FixRotY", group, &m774);
    fn_80087994(group, "DrawDebugInfo", &m7F0, 0, 0);
    fn_80087858(group, "PadDisableAngle", &mA04, 0, 0, 0.0f, 180.0f, 1.0f);
    fn_800874C8(group, "PadDisableFrame", &mA08, 0, 300, 1, 0, 0);
    fn_8008771C(group, "BirdViewLockOnSpeed", &m8D8, 0, 0, 0.0f, 100.0f, 0.01f);
    fn_8008771C(group, "BirdViewLockOnLength", &m8DC, 0, 0, 0.0f, 100.0f, 0.01f);
    fn_800874C8(group, "CollisionMode", &m9F4, 0, 2, 1, 0, 0);
    fn_800874C8(group, "InterpFrame", &m9F0, 1, 999, 1, 0, 0);
    fn_8008771C(group, "InterpSpeed", &m9EC, 0, 0, 0.0f, 1.0f, 0.001f);
    void* fix = fn_8008937C(group, "FixProgram");
    fn_80087E9C(fix, "Position", &m4FC, 0, 0, -10000.0f, 10000.0f, 1.0f);
    fn_80087E9C(fix, "Target", &m508, 0, 0, -10000.0f, 10000.0f, 1.0f);
    fn_80087858(fix, "AngleOfView", &m514, 0, 0, -10000.0f, 10000.0f, 1.0f);
    fn_80087994(fix, "PlayerOffset", &m518, 0, 0);
}

// inferred: debug menu of the parameters of a camera mode
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

// inferred: camera behind the character (or looking from it to its target mFC4/mFC0) at the parameters m51C;
// with `place` the camera is put there at once (out of the walls), with `reset` the turn speeds mA0C are cleared;
// `speed` < 0 takes SetConfig::mTurnSpeed
// NONMATCHING: 99.2% - stack slots: the original puts the temporaries of fn_800109E0 (rotation vector and the
// z/y/x matrices) in the reverse order (as fn_8038C070)
void GameCamera::fn_8037EF48(CharaTask* chara, int place, int reset, float speed) {
    if (m7FC != 0 && m7FC != 8) {
        return;
    }
    m7F4 = 1;
    mCA8 = speed;
    if (mCA8 < 0.0f) {
        mCA8 = SetConfig::sInstance->mTurnSpeed;
    }
    m7F8 = chara->mRot.y;
    if (chara->mFlag31 || chara->mMotion.mTracks[0].m00 == 0x55) {
        m7F8 += 3.1415927f;
    }
    CharaTask* other = chara->mFC4 != 0 ? chara->mFC4 : chara->mFC0;
    if (other != 0) {
        atn::Vector3 dir = other->mPos - chara->mPos;
        if (PSVECSquareMag(&dir) > 1.0e-5f) {
            PSVECNormalize(&dir, &dir);
            dir.fn_80139F60();
            m7F8 = dir.y;
        }
    }
    m7F8 = atn::WrapAngle_unk(m7F8);
    if (reset) {
        mA0C = 0.0f;
        mA10 = 0.0f;
        mA14 = 0.0f;
    }
    if (lbl_8087F0A8->mAlwaysBirdview != 0 && !reset) {
        m7F4 = 0;
    }
    if (place) {
        atn::Vector3 pos = mPos;
        atn::Vector3 target = mTarget;
        target = chara->mPos;
        target.y += m51C.m10;
        m868 = target;
        target.x += m51C.m0C * (float)cos(3.1415927f + chara->mRot.y);
        target.z += -m51C.m0C * (float)sin(3.1415927f + chara->mRot.y);
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 1.0f;
        atn::Matrix3x4 m = fn_800109E0(atn::Vector3(-m51C.m08, 3.1415927f + chara->mRot.y, 0.0f));
        PSMTXMultVec(m.m, &pos, &pos);
        pos *= m51C.m00;
        pos += target;
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &target, &pos, 0x80000008, 0, 0)) {
            pos = result.m10;
        }
        SetPosition(pos);
        SetTarget_unk(target);
        mFovy = m51C.m04;
        fn_8004B378();
        fn_80392964(1, 1, 1, 1);
        lbl_8087F0A8->mInput.fn_80124BE4();
    }
    m880 = 0;
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

// inferred: blend of the camera parameters from a to b (m38 is taken from a)
void GameCameraParam_unk::fn_8037F830(const GameCameraParam_unk& a, const GameCameraParam_unk& b, float t) {
    float v00 = t * (b.m00 - a.m00) + a.m00;
    float v04 = t * (b.m04 - a.m04) + a.m04;
    float v08 = t * (b.m08 - a.m08) + a.m08;
    float v0C = t * (b.m0C - a.m0C) + a.m0C;
    float v10 = t * (b.m10 - a.m10) + a.m10;
    float v14 = t * (b.m14 - a.m14) + a.m14;
    float v18 = t * (b.m18 - a.m18) + a.m18;
    float v1C = t * (b.m1C - a.m1C) + a.m1C;
    float v20 = t * (b.m20 - a.m20) + a.m20;
    float v24 = t * (b.m24 - a.m24) + a.m24;
    float v28 = t * (b.m28 - a.m28) + a.m28;
    float v2C = t * (b.m2C - a.m2C) + a.m2C;
    float v30 = t * (b.m30 - a.m30) + a.m30;
    float v34 = t * (b.m34 - a.m34) + a.m34;
    m00 = v00;
    m04 = v04;
    m08 = v08;
    m0C = v0C;
    m10 = v10;
    m14 = v14;
    m18 = v18;
    m1C = v1C;
    m20 = v20;
    m24 = v24;
    m28 = v28;
    m2C = v2C;
    m30 = v30;
    m38 = a.m38;
    m34 = v34;
}

extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis (as in Em306GurgBeast)

// inferred: interpolation (b - a) * t + a (as in RedMp)
static inline atn::Vector3 LerpV_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// inferred: math helpers (weak copies; as in game/Em309Ghoul.cpp, game/BattleTask.cpp, gmk/Cannonball.cpp)
extern "C" inline float fn_80011220(float x) {
    return __fabs(x);
}
extern "C" inline float fn_8037F664(float x) {
    return sin(x);
}
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}
extern "C" inline float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
extern "C" inline float fn_800F8524(const float& a, const float& b, float t) {
    return t * (b - a) + a;
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

// inferred: sweep test of a sphere (here the camera eye) against the collision world
extern "C" int fn_8004D388(void* world, CollResult_unk* result, atn::Vector3* pos, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius);
extern "C" inline TownTask* fn_803830A0() {
    return lbl_8087FA20;
}
// inferred: up vector of the camera (weak copy 0x80382A08)
inline void atn::Camera::SetUpVector(const atn::Vector3& v) {
    mUp = v;
}
// inferred: collision world (weak copy; as in gmk/Cannonball.cpp)
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" inline float fn_8037F640(float x) {
    return cos(x);
}
extern "C" atn::Vector3 fn_805B81F0(const TownTask* town, int x, int z); // inferred: centre of a cell (game/TownTask.cpp)

extern "C" double asin(double x);
extern "C" int fn_800A58D0(void* pad, int port);             // inferred: classic controller connected
extern "C" int fn_800A55D4(void* pad, int port, int button); // inferred: button held
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610; // inferred: multiplayer session (only tested against 0 here)
extern "C" atn::Vector3 lbl_807C7030;   // inferred: zero vector

// inferred: the stick s left the direction of one of the last three frames (name invented)
static inline bool StickTurned_unk(const atn::Vector2_unk& s, const atn::Vector2_unk* last) {
    for (int i = 0; i < 3; i++) {
        if (s.x * last[i].x + s.y * last[i].y > 0.0f) {
            continue;
        }
        return true;
    }
    return false;
}

// inferred: default camera behind the leader (mode 0, also 8, 16 and 17; parameters m51C, m6C0 in mode 8, blended
// with m430 while the event timer m424 runs and with the parameters of m46C). The target is m10 above the head of
// the character; the eye turns towards the character it looks at (mFC4/mFC0 or m834), back behind it after
// fn_8037EF48 (m7F4, yaw m7F8), behind it while mFlag5/mFlag4, or else slowly behind it with its speed (auto
// roll); the right stick (or the buttons 2/3) turns the eye (mA14) and the pitch (mA10 -> mA0C). The eye is then
// pulled out of the walls (CollisionMode m9F4: 1 = eight sweeps from the target, 2 = the same smoothed over the
// last eight eyes m98C with a cubic B-spline) and the target moved m0C to the side
// NONMATCHING: 93.3% - stack slots of the inlined temporaries (the frame is 0x10 smaller), two more saved gprs in
// the original (it keeps the addresses of the sweep temporaries in r21-r25) and the layout of the stick loop
void GameCamera::fn_8037F9AC(const CharaTask* chara) {
    CharaTask* c = const_cast<CharaTask*>(chara);
    if (lbl_8087F0A8->mAlwaysBirdview != 0) {
        fn_80387DE4(chara);
        return;
    }
    const GameCameraParam_unk* param = &m51C;
    if (m7FC == 8) {
        param = &m6C0;
    }
    if (m424.fn_803829C0()) {
        m424.fn_803829D4();
        m3E8.fn_8037F830(*param, m430, m424.mTime);
        param = &m3E8;
        if (m424.mTime < m424.mStep) {
            m424.mActive = 0;
        }
    }
    if (m46C.mMode != 0) {
        m46C.mWeight = m46C.mWeight - m46C.mStep;
        m46C.mWeight = 0.0f > m46C.mWeight ? 0.0f : m46C.mWeight;
        m3E8.fn_8037F830(m46C.mParam, *param, m46C.mWeight);
        param = &m3E8;
    }

    atn::Vector3 cur(mPos);
    atn::Vector3 pos(mPos);
    for (int i = 0; i < 8; i++) {
        if (m98C[i].x == 0.0f) {
            m98C[i] = cur;
        }
    }
    for (int i = 7; i > 0; i--) {
        m98C[i] = m98C[i - 1];
    }
    if (m880 > 0) {
        pos = m978;
    }
    atn::Vector3 target(m868);
    atn::Vector3 base;
    atn::Vector3 dir = target - pos;
    float fovy = mFovy;
    if (dir.Length_unk() > 0.0001f) {
        dir.Normalize_unk();
    } else {
        pos.z += 1.0f;
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = -1.0f;
    }
    atn::Vector3 rot = Negate_unk(dir).ToRotation_unk();
    float fov = param->m24 * (param->m04 - fovy) + fovy;
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    atn::Vector3 head = c->fn_80178088();
    atn::Vector3 charaRot(chara->mRot);
    target = head;
    target.y += param->m10;
    m868 = target;
    base = target;
    if (chara->mFlag31 || chara->mMotion.mTracks[0].m00 == 0x55) {
        charaRot.y += 3.1415927f;
    }
    if (m8EC != 0) {
        rot.y += 3.1415927f;
    }

    atn::Vector3 r(-(param->m08 + mA0C), rot.y, 0.0f);
    atn::Vector3 offset(0.0f, 0.0f, param->m00);
    {
        atn::Matrix3x4 m = fn_800109E0(r);
        PSMTXMultVec(m.m, &offset, &offset);
    }
    atn::Vector3 eye;
    eye = offset + target;

    float sx = fn_800A56A8(lbl_8087EF70, 0, 0, 0);
    float sy = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
    if ((float)__fabs(sx) > 0.2f || (float)__fabs(sy) > 0.2f) {
        if (++m984 >= 40) {
            m984 = 40;
        }
    } else if (--m984 <= 0) {
        m984 = 0;
    }

    CharaTask* other = chara->mFC4 != 0 ? chara->mFC4 : chara->mFC0;
    if (other != 0 || m834 != 0) {
        CharaTask* look = m834 != 0 ? m834 : other;
        atn::Vector3 d(look->mPos.x - chara->mPos.x, 0.0f, look->mPos.z - chara->mPos.z);
        float len = d.Length_unk();
        if (len > 0.001f && (float)__fabs(0.5f * param->m0C / len) < 1.0f) {
            d.Normalize_unk();
            d.fn_80139F60();
            float a = 4.0f * (float)asin(0.5f * param->m0C / len);
            a = MIN(0.2617994f, MAX(-0.2617994f, a));
            d.y += a;
            float turn = -atn::WrapAngle_unk(r.y - atn::WrapAngle_unk(3.1415927f + d.y));
            atn::Vector3 e = pos - target;
            Mtx m;
            PSMTXRotRad(m, 'y', -r.y);
            PSMTXMultVec(m, &e, &e);
            float speed = m7F4 != 0 ? param->m28 : param->m30;
            if (m834 != 0) {
                speed *= 1.5f;
            }
            mCA0.y += 1.0f / (SetConfig::sInstance->mRotDelay / 10.0f);
            if (mCA0.y > 3.1415927f) {
                mCA0.y = 3.1415927f;
            }
            if ((float)__fabs(turn) > mCA0.y) {
                turn = turn >= 0.0f ? mCA0.y : -mCA0.y;
            }
            m96C = turn * speed;
            m970 = 0.0f;
            r.y += m96C;
            Mtx m2;
            PSMTXRotRad(m2, 'y', r.y);
            PSMTXMultVec(m2, &e, &e);
            eye = e + target;
            if (m7F4 != 0 && (float)__fabs(turn) < 0.2f) {
                m7F4 = 0;
            }
        }
    } else if (m7F4 != 0) {
        float turn = -atn::WrapAngle_unk(r.y - atn::WrapAngle_unk(3.1415927f + m7F8));
        atn::Vector3 e = eye - target;
        Mtx m;
        PSMTXRotRad(m, 'y', -r.y);
        PSMTXMultVec(m, &e, &e);
        mCA0.y += 1.0f / (SetConfig::sInstance->mRotDelay / 10.0f);
        if (mCA0.y > 3.1415927f) {
            mCA0.y = 3.1415927f;
        }
        if ((float)__fabs(turn) > mCA0.y) {
            turn = turn >= 0.0f ? mCA0.y : -mCA0.y;
        }
        m970 = 0.0f;
        m96C = mCA8 * (turn * param->m28);
        r.y += m96C;
        Mtx m2;
        PSMTXRotRad(m2, 'y', r.y);
        PSMTXMultVec(m2, &e, &e);
        eye = e + target;
        if ((float)__fabs(turn) < 0.05f) {
            m7F4 = 0;
            mCA0.y = 0.0f;
        }
    } else if (chara->mFlag5 || chara->mFlag4) {
        float turn = -atn::WrapAngle_unk(r.y - atn::WrapAngle_unk(3.1415927f + charaRot.y));
        atn::Vector3 e = eye - target;
        Mtx m;
        PSMTXRotRad(m, 'y', -r.y);
        PSMTXMultVec(m, &e, &e);
        mCA0.y += 1.0f / (SetConfig::sInstance->mRotDelay / 10.0f);
        if (mCA0.y > 3.1415927f) {
            mCA0.y = 3.1415927f;
        }
        if ((float)__fabs(turn) > mCA0.y) {
            turn = turn >= 0.0f ? mCA0.y : -mCA0.y;
        }
        m970 = 0.0f;
        m96C = turn * param->m30;
        r.y = r.y + m96C + turn * param->m30;
        Mtx m2;
        PSMTXRotRad(m2, 'y', r.y);
        PSMTXMultVec(m2, &e, &e);
        eye = e + target;
    } else {
        m988 = 0.2f * (c->m570 - m988) + m988;
        float t = (m988 - 0.5f) * 1.6666666f;
        t = MAX(0.0f, MIN(1.0f, t));
        float roll = 0.0375f * (0.5f * t + 0.65f);
        if (c->m570 < 0.2f) {
            roll = 0.0f;
        }
        float turn = -atn::WrapAngle_unk(r.y - atn::WrapAngle_unk(3.1415927f + charaRot.y));
        float dead = 1.0f - 1.0f;
        if ((unsigned int)m984 <= 20) {
            roll *= 0.25f;
        }
        if (SetConfig::sInstance != 0) {
            roll *= SetConfig::sInstance->mRollbackSpeed;
        }
        float x = fn_800A56A8(lbl_8087EF70, 0, 0, 0);
        if ((float)__fabs(x) <= 0.2f) {
            x = 0.0f;
        }
        float y = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
        if ((float)__fabs(y) <= 0.2f) {
            y = 0.0f;
        }
        atn::Vector2_unk s(x, y);
        if ((float)sqrt(x * x + y * y) != 0.0f) {
            float inv = 1.0f / (float)sqrt(s.x * s.x + s.y * s.y);
            s.x = x * inv;
            s.y = y * inv;
        }
        if (!StickTurned_unk(s, mC88)) {
            mCA0.x += 1.0f / SetConfig::sInstance->mRotDelay;
        } else {
            mCA0.x = 0.0f;
        }
        if (mCA0.x >= 1.0f) {
            mCA0.x = 1.0f;
        }
        mC88[2] = mC88[1];
        mC88[1] = mC88[0];
        mC88[0] = s;
        roll *= mCA0.x;
        if ((float)__fabs(turn) < 0.017453292f * (30.0f * dead)) {
            turn = 0.0f;
        }
        if ((float)__fabs(turn) > 0.017453292f * (180.0f - 30.0f)) {
            turn = 0.0f;
        }
        if ((float)__fabs(turn) > (float)__fabs(m974)) {
            m974 = 0.2f * (turn - m974) + m974;
        } else {
            m974 = 0.4f * (turn - m974) + m974;
        }
        roll *= m974;
        if ((float)__fabs(m96C) < (float)__fabs(roll)) {
            if (roll >= 0.0f) {
                m970 += 0.1f;
                if (m970 > 1.0f) {
                    m970 = 1.0f;
                }
                m96C = m970 * 0.017453292f + m96C;
                if (m96C > roll) {
                    m96C = roll;
                    m970 = 0.0f;
                }
            } else {
                m970 -= 0.1f;
                if (m970 < -1.0f) {
                    m970 = -1.0f;
                }
                m96C = m970 * 0.017453292f + m96C;
                if (m96C < roll) {
                    m96C = roll;
                    m970 = 0.0f;
                }
            }
        } else if (roll >= 0.0f) {
            m970 -= 0.1f;
            if (m970 < -1.0f) {
                m970 = -1.0f;
            }
            m96C = m970 * 0.017453292f + m96C;
            if (m96C < roll) {
                m96C = roll;
                m970 = 0.0f;
            }
        } else {
            m970 += 0.1f;
            if (m970 > 1.0f) {
                m970 = 1.0f;
            }
            m96C = m970 * 0.017453292f + m96C;
            if (m96C > roll) {
                m96C = roll;
                m970 = 0.0f;
            }
        }
        if (m96C > 0.07853982f) {
            m96C = 0.07853982f;
        }
        if (m96C < -0.07853982f) {
            m96C = -0.07853982f;
        }
        atn::Vector3 e = eye - target;
        Mtx m;
        PSMTXRotRad(m, 'y', -r.y);
        PSMTXMultVec(m, &e, &e);
        r.y += m96C;
        Mtx m2;
        PSMTXRotRad(m2, 'y', r.y);
        PSMTXMultVec(m2, &e, &e);
        eye = e + target;
    }

    if (0.0f != param->m34) {
        atn::Vector3 e = eye - target;
        Mtx m;
        PSMTXRotRad(m, 'y', -r.y);
        PSMTXMultVec(m, &e, &e);
        r.y += param->m34;
        Mtx m2;
        PSMTXRotRad(m2, 'y', r.y);
        PSMTXMultVec(m2, &e, &e);
        eye = e + target;
    }

    float rx = 0.0f;
    float ry = 0.0f;
    if (fn_800A58D0(lbl_8087EF70, 0)) {
        rx = fn_800A56A8(lbl_8087EF70, 0, 2, 0);
        ry = fn_800A56A8(lbl_8087EF70, 0, 3, 0);
    }
    if (lbl_8087F610 != 0 && chara->m58C == 2 && c->fn_8015EB2C()) {
        rx += fn_800A56A8(lbl_8087EF70, 0, 0, 0);
        ry += fn_800A56A8(lbl_8087EF70, 0, 1, 0);
    }
    if (fn_800A55D4(lbl_8087EF70, 0, 2)) {
        rx -= 0.9f;
    } else if (fn_800A55D4(lbl_8087EF70, 0, 3)) {
        rx += 0.9f;
    }
    if (((unsigned int)SetConfig::sInstance->m50 & 2) == 2) {
        rx *= -1.0f;
    }
    if (((unsigned int)SetConfig::sInstance->m50 & 1) == 1) {
        ry *= -1.0f;
    }
    float ax = (float)__fabs(rx);
    if (ax > 0.2f) {
        float v = (rx > 0.0f ? 1.0f : -1.0f) * ((ax - 0.2f) / 0.8f);
        mA14 = -v * 0.017453292f + mA14;
        mA14 = MAX(-0.08726646f, MIN(0.08726646f, mA14)) * (float)__fabs(v);
    } else {
        mA14 *= 0.7f;
        if ((float)__fabs(mA14) < 0.00017453292f) {
            mA14 = 0.0f;
        }
    }
    if (0.0f != mA14) {
        atn::Vector3 e = eye - target;
        Mtx m;
        PSMTXRotRad(m, 'y', -r.y);
        PSMTXMultVec(m, &e, &e);
        r.y += mA14;
        Mtx m2;
        PSMTXRotRad(m2, 'y', r.y);
        PSMTXMultVec(m2, &e, &e);
        eye = e + target;
    }
    float ay = (float)__fabs(ry);
    if (ay > 0.2f) {
        float v = (ry > 0.0f ? 1.0f : -1.0f) * ((ay - 0.2f) / 0.8f);
        mA10 = -v * 0.013962634f + mA10;
        mA10 = MAX(-0.06981317f, MIN(0.06981317f, mA10)) * (float)__fabs(v);
    } else {
        mA10 *= 0.6f;
        if ((float)__fabs(mA10) < 0.00017453292f) {
            mA10 = 0.0f;
        }
    }
    mA0C += mA10;
    float pitch = MAX(-0.43633232f, MIN(0.7853982f, param->m08 + mA0C));
    if (chara->mFlag3 || (chara->m54C & 0x200000) == 0x200000) {
        if (pitch < 0.08726646f) {
            pitch = MIN(0.08726646f, pitch + 0.08726646f);
        }
    }
    mA0C = pitch - param->m08;

    offset = target - eye;
    if (offset.Length_unk() > 0.00001f) {
        offset.Normalize_unk();
    } else {
        eye.z += 1.0f;
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = -1.0f;
    }

    CollResult_unk result;
    CollResult_unk result2;
    if (m9F4 >= 1 && !chara->mFlag5) {
        if (fn_8004ED34(lbl_8087EE98, &result, &target, &eye, 0x80000008, 0, 0) && m8F0 == 0 &&
            !fn_8004ED34(lbl_8087EE98, &result2, &pos, &result.m10, 0x80000008, 0, 0)) {
            eye = result.m10;
            atn::Vector3 d = target - eye;
            if (d.LengthSq_unk() > 0.01f) {
                eye += d.GetNormalize() * 0.1f;
            }
            offset = target - eye;
            offset.Length_unk();
            offset.Normalize_unk();
        }
    } else if (fn_8004ED34(lbl_8087EE98, &result, &target, &eye, 0x80000008, 0, 0) && m8F0 == 0) {
        eye = result.m10;
        atn::Vector3 d = target - eye;
        if (d.LengthSq_unk() > 0.01f) {
            eye += d.GetNormalize() * 0.1f;
        }
    }

    offset = target - eye;
    r = Negate_unk(offset).ToRotation_unk();
    rot.x += (-(param->m08 + mA0C) - rot.x) * param->m20;
    rot.y = r.y;
    rot.z = 0.0f;
    offset = atn::Vector3(0.0f, 0.0f, param->m00);
    {
        atn::Matrix3x4 m = fn_800109E0(rot);
        PSMTXMultVec(m.m, &offset, &offset);
    }
    pos = offset + target;

    // inferred: control 75 of state 6 (m958/m95C/m960 set by that control)
    if (chara->m55C == 6 && chara->m560 == 0x4B) {
        atn::Vector3 v(m960);
        Mtx m;
        PSMTXRotRad(m, 'y', m958 * 2.6179938f);
        PSMTXMultVec(m, &v, &v);
        atn::Vector3 s = v * (0.5f * m958);
        v.x = v.x + s.x;
        v.y = -3.0f * m958 + (v.y + s.y);
        v.z = v.z + s.z;
        pos = target + v * m95C;
    }

    m978 = pos;
    if (m9F4 >= 1 && !chara->mFlag5) {
        atn::Vector3 d = base - pos;
        float step = d.Length_unk() * 0.125f;
        atn::Vector3 p(base);
        int hits = 0;
        for (int i = 1; i <= 8; i++) {
            atn::Vector3 v = pos - p;
            if ((float)__fabs(v.LengthSq_unk()) < 1.1920929e-7f) {
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = 0.1f;
            }
            atn::Vector3 move = v.GetNormalize() * step;
            if (fn_8004D388(lbl_8087EE98, &result, &p, &move, 0x80000008, 0, 0, 3.5f)) {
                hits++;
                p.x = result.m10.x;
                p.y = (p.y + result.m10.y) * 0.5f;
                p.z = result.m10.z;
            } else {
                p = result.m10;
            }
        }
        pos = p;
        if (m9F4 == 2) {
            m98C[0] = pos;
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            for (int i = -2; i < 10; i++) {
                int idx = i;
                if (i < 0) {
                    idx = 0;
                }
                if (i > 7) {
                    idx = 7;
                }
                float t = 1.0f - (float)i;
                if (t < 0.0f) {
                    t = -t;
                }
                float w;
                if (t < 1.0f) {
                    w = (4.0f + (t * (3.0f * t * t) - 6.0f * t * t)) / 6.0f;
                } else if (t < 2.0f) {
                    float u = t - 2.0f;
                    w = (u * (-u * u)) / 6.0f;
                } else {
                    w = 0.0f;
                }
                x = w * m98C[idx].x + x;
                y = w * m98C[idx].y + y;
                z = w * m98C[idx].z + z;
            }
            pos.x = x;
            pos.y = y;
            pos.z = z;
        } else if (hits > 0) {
            m880 = m9F0;
            if ((pos - cur).LengthSq_unk() > 0.00001f) {
                pos = (pos - cur) * m9EC + cur;
            }
        } else if (m880 > 0) {
            m880--;
            if ((pos - cur).LengthSq_unk() > 0.00001f) {
                pos = (pos - cur) * (1.0f - (float)m880 / (float)m9F0) + cur;
            }
        }
        offset = target - pos;
        r = Negate_unk(offset).ToRotation_unk();
        rot.y = r.y;
        rot.z = 0.0f;
    } else if (fn_8004ED34(lbl_8087EE98, &result, &target, &pos, 0x80000008, 0, 0) && m8F0 == 0) {
        atn::Vector3 d = target - result.m10;
        if (d.LengthSq_unk() > 0.0001f) {
            pos = result.m10;
            pos += (target - pos).GetNormalize() * 0.1f;
            m880 = m9F0;
            if ((pos - cur).LengthSq_unk() > 0.00001f) {
                pos = (pos - cur) * m9EC + cur;
            }
        }
        if (fn_8004D388(lbl_8087EE98, &result, &pos, &lbl_807C7030, 0x80000008, 0, 0, 3.0f)) {
            pos = result.m10;
        }
    } else if (m880 > 0) {
        m880--;
        if ((pos - cur).LengthSq_unk() > 0.00001f) {
            pos = (pos - cur) * (1.0f - (float)m880 / (float)m9F0) + cur;
        }
    }

    float dist = (target - pos).Length_unk();
    target.x += (float)cos(rot.y) * (param->m0C * MIN(dist / param->m00, 1.0f));
    target.z += (float)sin(rot.y) * (-param->m0C * MIN(dist / param->m00, 1.0f));
    SetPosition(pos);
    SetTarget_unk(target);
    mFovy = fov;
    SetUpVector(up);
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
    m8EC = 0;
}

// inferred: camera of the town (m6FC): looks at the character m6FC.m10 above it from m6FC.m00 away, turning towards
// its yaw at the speed m6FC.m2C (immediately with m8EC); lower pitch near the centre cell of the town; the eye is
// moved in front of the walls (m880 frames back out), and the target goes m6FC.m0C beside the character
// (over the inlining budget: the accessors are weak copies)
// NONMATCHING: 64.6% - the original gets the two y rotations from fn_8013A13C as a function returning the matrix
// by value (here atn::Matrix3x4::RotY_unk; `Matrix3x4 m; m.fn_8013A13C()` adds a call to its empty constructor)
// and float registers of the pitch blend
void GameCamera::fn_80382A1C(const CharaTask* chara) {
    CharaTask* c = const_cast<CharaTask*>(chara);
    atn::Vector3 pos(*GetPosition_unk());
    atn::Vector3 target(m868);
    atn::Vector3 dir = target - pos;
    dir.Length_unk();
    float fovy = GetFovy_unk();
    dir.Normalize_unk();
    atn::Vector3 rot = (-dir).ToRotation_unk();
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    float fov = m6FC.m24 * (m6FC.m04 - fovy) + fovy;
    const atn::Vector3& head = c->fn_80178088();
    atn::Vector3 charaRot(*c->GetRot_unk());
    target = head;
    target.y += m6FC.m10;
    m868 = target;
    if (m8EC != 0) {
        rot.y = charaRot.y;
    }
    float pitch = -m6FC.m08;
    if (fn_803830A0()) {
        atn::Vector3 d = head - fn_805B81F0(fn_803830A0(), 3, 2);
        d.y = 0.0f;
        if (head.y < 30.0f && d.LengthSq_unk() < 122500.0f) {
            pitch *= 0.25f;
        }
    }
    atn::Vector3 r(pitch, rot.y, 0.0f);
    atn::Vector3 offset(0.0f, 0.0f, m6FC.m00);
    offset.fn_80011410(fn_800109E0(r));
    atn::Vector3 eye;
    eye = offset + target;
    float turn = -atn::WrapAngle_unk(r.y - atn::WrapAngle_unk(charaRot.y));
    atn::Vector3 e = eye - target;
    e.fn_80011410(atn::Matrix3x4::RotY_unk(-r.y));
    r.y = turn * m6FC.m2C + r.y;
    e.fn_80011410(atn::Matrix3x4::RotY_unk(r.y));
    eye = e + target;
    offset = target - eye;
    offset.Length_unk();
    offset.Normalize_unk();
    CollResult_unk result;
    offset = target - eye;
    r = (-offset).ToRotation_unk();
    rot.x += pitch - rot.x;
    rot.y = r.y;
    rot.z = r.z;
    dir = atn::Vector3(0.0f, 0.0f, m6FC.m00);
    dir.fn_80011410(fn_800109E0(rot));
    pos = dir + target;
    if (fn_8004ED34(fn_801404F8(), &result, &target, &pos, 0x80000008, 0, 0)) {
        atn::Vector3 hit = target - result.m10;
        if (hit.LengthSq_unk() > 0.0001f) {
            pos = result.m10;
            pos += (target - pos).GetNormalize() * 0.1f;
            m880 = 10;
            if ((pos - *GetPosition_unk()).LengthSq_unk() > 0.00001f && m8EC == 0) {
                pos = atn::Lerp_unk(*GetPosition_unk(), pos, 0.25f);
            }
        }
    } else if (m880 > 0) {
        m880--;
        if ((pos - *GetPosition_unk()).LengthSq_unk() > 0.00001f) {
            pos = atn::Lerp_unk(*GetPosition_unk(), pos, 1.0f - (float)m880 / 10.0f);
        }
    }
    if (fn_8004D388(fn_801404F8(), &result, &pos, &lbl_807C7030, 0x80000008, 0, 0, 3.0f)) {
        pos = result.m10;
    }
    float dist = (target - pos).Length_unk();
    target.x += m6FC.m0C * (dist / m6FC.m00) * fn_8037F640(rot.y);
    target.z += -m6FC.m0C * (dist / m6FC.m00) * fn_8037F664(rot.y);
    if (fn_801C3910(target - pos) < 1.0f) {
        if (offset.Length_unk() > 0.1f) {
            pos -= offset.GetNormalize();
        } else {
            pos.z += 1.0f;
        }
    }
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8037F688(this, fov);
    SetUpVector(up);
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
    m8EC = 0;
}

// inferred: free camera moved with the stick (dead zone 0.2, squared response, turned around the up vector and the
// side vector at the speed m558.m1C) from frame 15 on, and kept m558.m10 above the character looking m558.m00 away;
// with m838 set it blends instead the target from m84C to m840 and the fovy from m85C to half m558.m04 (6 steps)
// NONMATCHING: 97.4% - stack slots of the temporaries of the two stick rotations and of the final offset
void GameCamera::fn_803830A8(const CharaTask* chara) {
    if (++m808 < 15) {
        return;
    }
    if (m838 != 0) {
        if (m83C < 6) {
            m83C++;
            float t = (float)m83C / 6.0f;
            SetTarget_unk(LerpV_unk(m84C, m840, t));
            float fovy = 0.5f * m558.m04;
            mFovy = t * (fovy - m85C) + m85C;
        }
    } else {
        atn::Vector3 pos = mPos;
        atn::Vector3 target = mTarget;
        atn::Vector3 up = mUp;
        atn::Vector3 dir;
        dir = mTarget - mPos;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 side;
        PSVECCrossProduct(&dir, &up, &side);
        atn::Vector3 back(m818.x - m80C.x, m818.y - m80C.y, m818.z - m80C.z);
        PSVECNormalize(&back, &back);
        if (PSVECSquareMag(&side) < 0.01f) {
            side.x = 0.0f;
            side.y = 0.0f;
            side.z = 1.0f;
        }
        float x = fn_800A56A8(lbl_8087EF70, 0, 0, 0);
        if ((float)__fabs(x) > 0.2f) {
            float v = (x - (x > 0.0f ? 0.2f : -0.2f)) / 0.8f;
            v *= (float)__fabs(v);
            Mtx m;
            PSMTXRotAxisRad(m, &up, -v * m558.m1C);
            atn::Vector3 off;
            off = target - pos;
            PSMTXMultVec(m, &off, &off);
            target = off + pos;
        }
        float y = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
        if ((float)__fabs(y) > 0.2f) {
            float v = (y - (y > 0.0f ? 0.2f : -0.2f)) / 0.8f;
            v *= (float)__fabs(v);
            Mtx m;
            PSMTXRotAxisRad(m, &side, v * m558.m1C);
            atn::Vector3 off;
            off = target - pos;
            PSMTXMultVec(m, &off, &off);
            target = off + pos;
        }
        dir = target - pos;
        PSVECNormalize(&dir, &dir);
        if ((float)__fabs(dir.y) > 0.9f) {
            pos = mPos;
            target = mTarget;
            dir = (mTarget - mPos).GetNormalize();
        }
        atn::Vector3 head = chara->mPos;
        atn::Vector3 rot = chara->mRot;
        atn::Vector3 offset(0.0f, m558.m10, 0.0f);
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXMultVec(r, &offset, &offset);
        head += offset;
        pos = head;
        target = pos + dir * m558.m00;
        mPos = pos;
        mTarget = target;
    }
    fn_8004B378();
    fn_80392964(1, 1, 0, 0);
}

// inferred: direction the camera looks at (Event/Manager.cpp)
extern "C" atn::Vector3 fn_80383728(const Camera_unk* camera) {
    atn::Vector3 dir;
    dir = *camera->GetTarget_unk() - *camera->GetPosition_unk();
    dir.Normalize_unk();
    return dir;
}


// inferred: camera of the ledge without the blend (see fn_8038A5F0; side offset limits lbl_8087DCF0/F4): the eye
// is turned a third of the way towards the current view direction; with the bird view setting it is the bird view
// camera (fn_80387DE4)
void GameCamera::fn_8038378C(const CharaTask* chara) {
    if (lbl_8087F0A8->mAlwaysBirdview != 0) {
        fn_80387DE4(chara);
        return;
    }
    atn::Vector3 side = Cross_unk(chara->mC14, atn::Vector3(0.0f, 1.0f, 0.0f));
    side.Normalize_unk();
    atn::Vector3 rot = side.ToRotation_unk();
    float offset = 0.0f;
    rot.x += m594.m08;
    unsigned int flags = chara->mC44;
    float length = (chara->mC20 - chara->mC2C).Length_unk();
    float start = (chara->mPos - chara->mC20).Length_unk();
    float end = (chara->mPos - chara->mC2C).Length_unk();
    float t = start / length;
    t = 1.0f < t ? 1.0f : t;
    if ((flags & 1) == 1) {
        if ((flags & 2) == 2) {
            offset = fn_800F8524(-m594.m0C, m594.m0C, t);
        } else {
            offset = t * (lbl_8087DCF0 - -m594.m0C) + -m594.m0C;
        }
    } else if ((flags & 2) == 2) {
        offset = t * (m594.m0C - lbl_8087DCF4) + lbl_8087DCF4;
    }
    float yaw;
    float height = m594.m10;
    if (chara->mFlag30) {
        height -= m594.m18;
    }
    atn::Vector3 target = chara->mPos;
    yaw = rot.y;
    target.y += height;
    m868 = target;
    target.x += offset * (float)cos(yaw);
    target.z += -offset * (float)sin(rot.y);
    atn::Vector3 eye(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m = fn_800109E0(rot);
    PSMTXMultVec(m.m, &eye, &eye);
    eye *= -m594.m00;
    eye += target;
    atn::Vector3 dir = target - eye;
    float dy = dir.y;
    dir.y = 0.0f;
    float dist = PSVECMag(&dir);
    PSVECNormalize(&dir, &dir);
    atn::Vector3 cur = fn_80383728(this);
    cur.y = 0.0f;
    PSVECNormalize(&cur, &cur);
    dir += cur * 3.0f;
    PSVECNormalize(&dir, &dir);
    dir.x *= -dist;
    dir.z *= -dist;
    dir.y = -dy;
    eye = target + dir;
    float fovy = m594.m04;
    fn_8038E23C(chara, &m594, &eye, &target);
    SetPosition(eye);
    SetTarget_unk(target);
    mFovy = fovy;
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
}

// inferred: name invented (the second operand is evaluated twice, as a macro)
#define GAMECAMERA_MAX_UNK(a, b) ((a) > (b) ? (a) : (b))
#define GAMECAMERA_MIN_UNK(a, b) ((a) < (b) ? (a) : (b))

// inferred: turns `pos` around `center` on `axis` with the stick value `input` (dead zone 0.2, speed m1C
// of the parameters, half with `slow`); `angle` keeps the speed of the turn, clamped to the input and
// slowed down by 0.01 * m1C each frame (fn_80384188, fn_80385B48)
// NONMATCHING: 98.4% - stack slots of the offset and of the turned position (v/r swapped) and the float
// registers of the clamp; tried v at the top, r before v, the rotation through an inline taking v by value
extern "C" void fn_80383F7C(float* angle, atn::Vector3* pos, const atn::Vector3* center, const atn::Vector3* axis,
                            const GameCameraParam_unk* param, int slow, float input, float rate) {
    const float dead = 0.2f;
    if ((float)__fabs(input) > dead) {
        float speed = (input - (input > 0.0f ? dead : -dead)) / 0.8f;
        speed *= rate;
        speed *= slow ? 0.5f : 1.0f;
        speed *= param->m1C;
        speed = 1.2f * speed;
        *angle = 0.15f * speed + *angle;
        *angle = GAMECAMERA_MIN_UNK((float)__fabs(speed), GAMECAMERA_MAX_UNK(-(float)__fabs(speed), *angle));
        Mtx m;
        PSMTXRotAxisRad(m, axis, *angle);
        atn::Vector3 v;
        v = *pos - *center;
        PSMTXMultVec(m, &v, &v);
        atn::Vector3 r = v + *center;
        *pos = r;
        if (*angle > 0.0f) {
            *angle = GAMECAMERA_MAX_UNK(0.0f, *angle - 0.01f * param->m1C);
        } else if (*angle < 0.0f) {
            *angle = GAMECAMERA_MIN_UNK(0.0f, *angle + 0.01f * param->m1C);
        }
    } else {
        *angle = 0.0f;
    }
}

extern "C" BattleTask* lbl_8087F048; // inferred: battle manager
// inferred: closest point of the line (point, direction) to `p`
extern "C" void fn_80050900(const atn::Vector3* p, const atn::Vector3* line, atn::Vector3* out);
// inferred: sphere (center, radius) against a segment (as in gmk/WindyItem.cpp)
struct CameraSphere_unk {
    atn::Vector3 mCenter;
    float mRadius;
};
extern "C" int fn_80051B70(const atn::Vector3* line, const CameraSphere_unk* sphere);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// inferred: hips mode (m5D0): the eye stays m874/m5D0.m14 (+ the zoom mC1C, growing while the field flag 0x6A is
// set) behind the hips m9F8 and turns with both sticks (fn_80383F7C, mA18/mA1C; the inverted axes of the options);
// the pitch is limited to 70 degrees, the eye is kept out of the walls and of the head of the character, and the
// held button 0x24 zooms (fovy down to 0.6 m5D0.m04) or, in control 29 of state 6, locks on the nearest enemy
// (m860, blended in m864); with m858 or in state 5 it only blends the target from m84C to m840 (m838)
// NONMATCHING: ~33% - over the inlining budget here (every Vector3 operator is called), while the original inlines
// everything: the copies of Vector3 (__VecCopy) count as candidates here; inline helpers for the repeated
// target and rotation code were not enough
void GameCamera::fn_80384188(const CharaTask* chara) {
    if (++m808 < 0) {
        return;
    }
    if (m858 != 0 || chara->m55C == 5) {
        if (m838 != 0) {
            if (++m83C > 6) {
                m83C = 6;
            }
            float t = (float)m83C / 6.0f;
            SetTarget_unk((m840 - m84C) * t + m84C);
            mFovy = t * (0.6f * m5D0.m04 - m85C) + m85C;
            fn_8004B378();
            fn_80392964(1, 1, 0, 0);
        }
        return;
    }
    int zoom = 0;
    if (lbl_8087F0A8->mInput.fn_801240B4(0x24) && !lbl_8087F0A8->mInput.fn_801231D0(0x24)) {
        zoom = 1;
    }
    atn::Vector3 pos(mPos);
    atn::Vector3 target(mTarget);
    atn::Vector3 up(mUp);
    atn::Vector3 dir;
    dir = mTarget - mPos;
    PSVECNormalize(&dir, &dir);
    atn::Vector3 side;
    PSVECCrossProduct(&dir, &up, &side);
    atn::Vector3 view = m818 - m80C;
    atn::Vector3 rot(chara->mFA4);
    rot.fn_80139F60();
    PSVECNormalize(&view, &view);
    if (PSVECSquareMag(&side) < 0.01f) {
        side.x = 0.0f;
        side.y = 0.0f;
        side.z = 1.0f;
    }
    float x = fn_800A56A8(lbl_8087EF70, 0, 0, 0);
    if (fn_800A58D0(lbl_8087EF70, 0)) {
        x += fn_800A56A8(lbl_8087EF70, 0, 2, 0);
    }
    if (((unsigned int)SetConfig::sInstance->m50 & 8) == 8) {
        x *= -1.0f;
    }
    if (SetConfig::sInstance != 0) {
        x *= 0.3f * (SetConfig::sInstance->mCameraRotSpeed - 1.0f) + 1.0f;
    }
    x = GAMECAMERA_MIN_UNK(1.0f, GAMECAMERA_MAX_UNK(-1.0f, x));
    float y = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
    if (fn_800A58D0(lbl_8087EF70, 0)) {
        y += fn_800A56A8(lbl_8087EF70, 0, 3, 0);
    }
    if (((unsigned int)SetConfig::sInstance->m50 & 4) == 4) {
        y *= -1.0f;
    }
    if (SetConfig::sInstance != 0) {
        y *= 0.3f * (SetConfig::sInstance->mCameraRotSpeed - 1.0f) + 1.0f;
    }
    y = GAMECAMERA_MIN_UNK(1.0f, GAMECAMERA_MAX_UNK(-1.0f, y));
    float rate = x * x + y * y;
    atn::Vector3 down(-up.x, -up.y, -up.z);
    fn_80383F7C(&mA18, &target, &pos, &down, &m60C, zoom, x, rate);
    fn_80383F7C(&mA1C, &target, &pos, &side, &m60C, zoom, y, rate);
    if (chara->m55C == 6 && chara->m560 == 29 && lbl_8087F048 != 0 && lbl_8087F0A8->mInput.fn_801231D0(0x24)) {
        m860 = lbl_8087F048->fn_801002DC(const_cast<CharaTask*>(chara), &pos, &dir, 1, 1000000.0f, 0.9424779f);
        if (m860 != 0) {
            m840 = (m860->mColli.m3C + m860->mColli.m48) * 0.5f;
            m864 = 0.0f;
        }
    }
    if (m860 != 0) {
        m864 += 0.1f;
        float t = 1.0f < m864 ? 1.0f : m864;
        target = (m840 - target) * t + target;
        if (m864 > 1.0f) {
            m860 = 0;
        }
    }
    dir = target - pos;
    PSVECNormalize(&dir, &dir);
    atn::Vector3 angle(dir);
    angle.fn_80139F60();
    float limit = 1.2217305f;
    if ((float)__fabs(angle.x) > limit) {
        angle.x = angle.x > 0.0f ? limit : -limit;
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 1.0f;
        atn::Matrix3x4 m = fn_800109E0(angle);
        PSMTXMultVec(m.m, &dir, &dir);
        target = dir * mDist;
    }
    atn::Vector3 offset(m874.x, m874.y, m5D0.m14);
    atn::Vector3 hips(m9F8);
    float step = -2.0f;
    if (lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x6A)) {
        step = 2.0f;
    }
    mC1C += step;
    mC1C = GAMECAMERA_MIN_UNK(GAMECAMERA_MAX_UNK(0.0f, mC1C), 10.0f);
    offset.z += mC1C;
    {
        atn::Matrix3x4 m = fn_800109E0(angle);
        PSMTXMultVec(m.m, &offset, &offset);
    }
    pos = hips + offset;
    target = dir * m5D0.m00 + pos;
    CollResult_unk result;
    atn::Vector3 from(hips);
    from.y += m874.y;
    atn::Vector3 beside(m874.x > 0.0f ? 3.0f + m874.x : m874.x - 3.0f, 0.0f, 0.0f);
    {
        Mtx m;
        PSMTXRotRad(m, 'y', angle.y);
        PSMTXMultVec(m, &beside, &beside);
    }
    atn::Vector3 to = from + beside;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000008, 0, 0)) {
        float len = 0.01f > PSVECMag(&(result.m04 - from)) - 3.0f ? 0.01f : PSVECMag(&(result.m04 - from)) - 3.0f;
        float z = (m5D0.m14 + mC1C) * (float)__fabs(len / m874.x);
        offset.x = m874.x > 0.0f ? len : -len;
        offset.y = m874.y;
        offset.z = z;
        {
        atn::Matrix3x4 m = fn_800109E0(angle);
        PSMTXMultVec(m.m, &offset, &offset);
    }
        pos = hips + offset;
        target = dir * m5D0.m00 + pos;
    }
    atn::Vector3 flat(dir.x, 0.0f, dir.z);
    if (PSVECSquareMag(&flat) > 0.00001f) {
        PSVECNormalize(&flat, &flat);
        atn::Vector3 line[2];
        line[0] = pos;
        line[1] = flat;
        atn::Vector3 near;
        fn_80050900(&hips, line, &near);
        from = near;
        to = pos;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000008, 0, 0)) {
            pos = result.m04 + result.m28;
            target = dir * m5D0.m00 + pos;
        }
        if (fn_8004D388(lbl_8087EE98, &result, &pos, &lbl_807C7030, 0x80000008, 0, 0, 2.0f)) {
            pos = result.m10;
            target = dir * m5D0.m00 + pos;
        }
    }
    from = pos + dir * -(m5D0.m14 + mC1C);
    to = pos;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000008, 0, 0)) {
        pos = result.m04 + result.m28;
        target = dir * m5D0.m00 + pos;
    }
    if (fn_8004D388(lbl_8087EE98, &result, &pos, &lbl_807C7030, 0x80000008, 0, 0, 3.0f)) {
        pos = result.m10;
        target = dir * m5D0.m00 + pos;
    }
    if (chara->m520 >= 0) {
        atn::Vector3 eye(pos);
        atn::Vector3 line[2];
        line[0] = pos;
        line[1] = target;
        MtxPtr node = chara->mMotion.GetNodeMtx_unk(chara->m520);
        CameraSphere_unk head;
        head.mCenter = atn::Vector3(node[0][3], node[1][3], node[2][3]);
        head.mRadius = 2.1f;
        if (fn_80051B70(line, &head)) {
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            float radius = head.mRadius;
            Mtx m;
            PSMTXRotRad(m, 'y', chara->mRot.y);
            PSMTXMultVec(m, &front, &front);
            atn::Vector3 p = head.mCenter + front * radius;
            atn::Vector3 d = p - pos;
            float len = PSVECMag(&d);
            if (len > 0.0001f) {
                PSVECNormalize(&d, &d);
                if (!fn_8004ED34(lbl_8087EE98, 0, &eye, &pos, 0x80000008, 0, 0)) {
                    float k = PSVECDotProduct(&dir, &d);
                    pos += dir * len * k;
                    target = dir * m5D0.m00 + pos;
                }
            }
        }
    }
    mPos = pos;
    mTarget = target;
    float fovy;
    if (zoom) {
        fovy = mFovy - 0.1f * m5D0.m04;
        if (fovy < 0.6f * m5D0.m04) {
            fovy = 0.6f * m5D0.m04;
        }
    } else {
        fovy = 0.1f * m5D0.m04 + mFovy;
        if (fovy > m5D0.m04) {
            fovy = m5D0.m04;
        }
    }
    mFovy = fovy;
    fn_8004B378();
    fn_80392964(1, 1, 0, 0);
}

// ---- fn_80387910: camera of the tactical view (over the inlining budget: the accessors are weak copies) ----
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
    target += (point - target) * m648.m1C;
    atn::Vector3 dir = target - chara->mPos;
    PSVECNormalize(&dir, &dir);
    pos = chara->mPos + dir * m648.m14;
    pos.y += m648.m10;
    float side = m648.m0C;
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
        dist = fn_800F8524(camDist, dist, 0.4f);
        float pitch = fn_80383728(this).ToRotation_unk().x;
        atn::Vector3 r = (target - pos).ToRotation_unk();
        r.x = fn_800F8524(pitch, r.x, 0.4f);
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

// inferred: the map has a ceiling (MapTask::mEC; written out as in fn_8038E540)
static inline bool HasCeiling_unk() {
    bool ceiling = false;
    bool map = false;
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
        map = true;
    }
    if (map && lbl_8087F430->GetCurrentMap()->mEC != 0) {
        ceiling = true;
    }
    return ceiling;
}

// inferred: height of the view over the ground on maps without a ceiling (10 on map 3, 8 on map 17-2, else 30)
static inline float ViewHeight_unk() {
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
        MapTask* map = lbl_8087F430->GetCurrentMap();
        if (map->mId == 3) {
            return 10.0f;
        }
        if (map->mId == 17 && map->mSubId == 2) {
            return 8.0f;
        }
    }
    return 30.0f;
}

static inline void AddViewHeight_unk(atn::Vector3* p) {
    if (!HasCeiling_unk()) {
        p->y += ViewHeight_unk();
    }
}

extern "C" void* lbl_8087EF70; // inferred: pads
extern "C" int fn_800A58D0(void* pad, int port);                          // inferred: classic controller connected
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis

// inferred: bird view camera: turns m8A0 towards the leader's yaw while m7F4 is set, turns with the right stick of
// the classic controller (speed mA14), follows the character over the floor (m8A4, swept on maps with a
// ceiling) and runs the tactical view camera (fn_80387910)
// NONMATCHING: 90.6% - stack slots of the ray ends (the original keeps from/to of the floor ray lowest) and
// the order of the first loads
void GameCamera::fn_80387DE4(const CharaTask* chara) {
    atn::Vector3 top = m8A4;
    top.y += lbl_8087F0A8->m08C;
    AddViewHeight_unk(&top);
    float radius = lbl_8087F0A8->m08C;
    if (m7F4 != 0) {
        float turn = -atn::WrapAngle_unk(m8A0 - atn::WrapAngle_unk(-m7F8));
        m96C = 0.3f * turn;
        m8A0 = atn::WrapAngle_unk(m96C + m8A0);
        if ((float)__fabs(turn) < 0.05f) {
            m7F4 = 0;
        }
    }
    if (fn_800A58D0(lbl_8087EF70, 0)) {
        float x = -fn_800A56A8(lbl_8087EF70, 0, 2, 0);
        float ax = (float)__fabs(x);
        if (ax > 0.2f) {
            float speed = (x > 0.0f ? 1.0f : -1.0f) * ((ax - 0.2f) / 0.8f);
            mA14 = -speed * 0.017453292f + mA14;
            mA14 = GAMECAMERA_MAX_UNK(-0.08726646f, GAMECAMERA_MIN_UNK(0.08726646f, mA14)) *
                      (float)__fabs(speed);
        } else {
            mA14 *= 0.7f;
            if ((float)__fabs(mA14) < 0.00017453292f) {
                mA14 = 0.0f;
            }
        }
        if (0.0f != mA14) {
            m8A0 = atn::WrapAngle_unk(mA14 + m8A0);
        }
    }
    if (!HasCeiling_unk()) {
        m8A4 = chara->mPos;
    } else {
        CollResult_unk result;
        CollResult_unk floor;
        int floorId = -1;
        atn::Vector3 from = chara->mPos;
        atn::Vector3 to = chara->mPos;
        from.y += 5.0f;
        to.y -= 5.0f;
        if (fn_8004ED34(lbl_8087EE98, &floor, &from, &to, 0x08000000, 0, 0)) {
            if (floor.m34 != 0) {
                floorId = *(int*)floor.m34;
            }
        } else {
            atn::Vector3 zero(0.0f, 0.0f, 0.0f);
            if (fn_8004D388(lbl_8087EE98, &floor, const_cast<atn::Vector3*>(&chara->mPos), &zero, 0x08000000, 0, 0,
                            20.0f)) {
                if (floor.m34 != 0) {
                    floorId = *(int*)floor.m34;
                }
            }
        }
        atn::Vector3 move = chara->mPos - m8A4;
        if (PSVECMag(&move) > 20.0f) {
            m8A4 = chara->mPos;
            move = atn::Vector3(0.0f, 0.0f, 0.0f);
        }
        move.y = 0.5f * -radius;
        if (fn_8004D388(lbl_8087EE98, &result, &top, &move, 0x08000000, 0, floorId, radius)) {
            atn::Vector3 d = result.m10 - top;
            if ((result.m3C & 1) != 1) {
                d.y = 0.0f;
            }
            m8A4 += d;
        } else {
            move.y = 0.0f;
            m8A4 += move;
        }
        atn::Vector3 up = m8A4;
        atn::Vector3 down = m8A4;
        up.y += 10.0f + radius;
        down.y -= 5.0f;
        if (fn_8004ED34(lbl_8087EE98, &result, &up, &down, 0x08000000, 0, (int)floorId)) {
            m8A4.y = result.m10.y;
        }
        m8B0 = m8A4;
    }
    fn_80387910(chara);
}

// inferred: frame of the first motion track of the character (name invented)
static inline float MotionFrame_unk(CharaTask* c) {
    return c->mMotion.mTracks[0].m08;
}

// inferred: side camera (m648): stays m7B0.m14 behind, m7B0.m10 above and m7B0.m0C beside the character, looking at a
// point in front of it; the control ids 15, 11 and 16 (m560) move it in while their motion plays
void GameCamera::fn_803884B8(const CharaTask* chara) {
    m808++;
    atn::Vector3 pos = mPos;
    atn::Vector3 target = mTarget;
    atn::Vector3 up = mUp;
    atn::Vector3 offset(0.0f, 0.0f, 0.0f);
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(chara->mRot.y);
    PSMTXMultVec(m.m, &front, &front);
    float side = m7B0.m0C;
    float height = m7B0.m10;
    float dist = m7B0.m14;
    float speed = m648.m1C;
    CharaTask* c = const_cast<CharaTask*>(chara);
    switch (chara->m560) {
    case 15:
        atn::VecAssign_unk(front, Negate_unk(front));
        atn::VecAssign_unk(offset, (chara->mF6C - m890) * 0.3f);
        break;
    case 11: {
        float h;
        side *= 1.0f - GAMECAMERA_MIN_UNK(0.9f, 2.5f * MotionFrame_unk(c) / c->mMotion.fn_80097D7C(0));
        float t = GAMECAMERA_MIN_UNK(1.0f, MotionFrame_unk(c) / 10.0f);
        h = t * (lbl_8087DCF8 - height) + height;
        height = h * (1.0f + GAMECAMERA_MIN_UNK(0.5f, 2.0f * GAMECAMERA_MAX_UNK(0.0f, MotionFrame_unk(c) - 10.0f) /
                                                      (c->mMotion.fn_80097D7C(0) - 10.0f)));
        dist *= 1.0f - GAMECAMERA_MIN_UNK(0.9f, 2.5f * MotionFrame_unk(c) / c->mMotion.fn_80097D7C(0));
        speed = 1.0f;
        break;
    }
    case 16:
        dist *= 1.0f - GAMECAMERA_MIN_UNK(0.9f, 2.0f * MotionFrame_unk(c) / c->mMotion.fn_80097D7C(0));
        speed = 0.3f;
        break;
    }
    atn::Vector3 look = c->fn_80178088() + front * dist + offset;
    target += (look - target) * speed;
    pos = c->fn_80178088();
    pos.y += height;
    pos += Cross_unk(front, up) * side;
    fn_8038E23C(chara, &m648, &pos, &target);
    SetPosition(pos);
    SetTarget_unk(target);
    if (lbl_8087F430->fn_80370174(0x65) && chara->mPos.y > 90.0f) {
        mFovy = 0.4f * (0.87266463f - mFovy) + mFovy;
    } else {
        mFovy = m7B0.m04;
    }
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
}

// NONMATCHING: 92.5% - stack slots and registers of the temporaries
// inferred: camera mode at m738.m0C looking at the character (m738.m18 above it); with a camera box of size m30
// the fovy is narrowed to keep that size in view
void GameCamera::fn_80388A88(const CharaTask* chara) {
    SetPosition(atn::Vector3(m738.m0C, m738.m10, m738.m14));
    atn::Vector3 offset(0.0f, m738.m18, 0.0f);
    SetTarget_unk(const_cast<CharaTask*>(chara)->fn_80178088() + offset);
    mFovy = m738.m04;
    if (mC20 != 0 && mC20->m30 > 0) {
        atn::Vector3 d = mPos - mTarget;
        float dist = PSVECMag(&d);
        float fovy = 2.0f * (float)atan((float)mC20->m30 * (float)tan(m738.m04 * 0.5f) / dist);
        mFovy = fovy < m738.m04 ? fovy : m738.m04;
    }
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
}

// inferred: event camera (see fn_8038CD94) kept out of the walls: when the ray from the target to the eye hits,
// the eye is moved to the wall over 10 frames (m880), and back when it stops hitting
// NONMATCHING: 96.3% - stack slots of the temporaries
void GameCamera::fn_80388C44(const CharaTask* chara) {
    float yaw = 0.017453292f * m774.m28;
    atn::Vector3 offset(m774.m0C, m774.m10, m774.m14);
    Mtx m;
    PSMTXRotRad(m, 'y', 3.1415927f + yaw);
    PSMTXMultVec(m, &offset, &offset);
    atn::Vector3 target = const_cast<CharaTask*>(chara)->fn_80178088() + offset;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m2;
    PSMTXRotRad(m2, 'x', -m774.m08);
    PSMTXMultVec(m2, &front, &front);
    Mtx m3;
    PSMTXRotRad(m3, 'y', 3.1415927f + yaw);
    PSMTXMultVec(m3, &front, &front);
    atn::Vector3 pos = target + front * m774.m00;
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &target, &pos, 0x80000008, 0, 0)) {
        pos = result.m10;
        pos += Negate_unk(front) * 0.1f;
        if (m880 < 10) {
            m880++;
        }
        pos = (pos - mPos) * ((float)m880 / 10.0f) + mPos;
    } else {
        if (m880 > 0) {
            m880--;
        }
        pos = (pos - mPos) * (1.0f - (float)m880 / 10.0f) + mPos;
    }
    SetPosition(pos);
    SetTarget_unk(target);
    mFovy = m774.m04;
    fn_8004B378();
    fn_80392964(0, 1, 1, 1);
}

// inferred: camera mode at a fixed offset of the character looking at a point in front of it
void GameCamera::fn_80389060(const CharaTask* chara) {
    atn::Vector3 target = chara->mPos;
    atn::Vector3 front(0.0f, 0.0f, 10.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(chara->mRot.y);
    PSMTXMultVec(m.m, &front, &front);
    target += front;
    atn::Vector3 pos = chara->mPos;
    atn::Vector3 offset(0.0f, 35.0f, -20.0f);
    atn::Matrix3x4 m2;
    m2.fn_8013A13C(chara->mRot.y);
    PSMTXMultVec(m2.m, &offset, &offset);
    pos += offset;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
}

// inferred: camera mode 40 beyond the character, on the line from a point 5 below its own position
void GameCamera::fn_80389210(const CharaTask* chara) {
    atn::Vector3 target = chara->mPos;
    atn::Vector3 dir = target - (const_cast<CharaTask*>(chara)->fn_80178088() + atn::Vector3(0.0f, -5.0f, 0.0f));
    atn::Vector3 pos = target + dir.GetNormalize() * 40.0f;
    SetPosition(pos);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
}

// NONMATCHING: 92.3% - stack slots: the original keeps the normalized direction below the temporaries
// inferred: camera mode 30 away behind and above the character, along (0.4, 0.2, 1) turned with it
void GameCamera::fn_803893A0(const CharaTask* chara) {
    atn::Vector3 target = chara->mPos;
    target.y += 20.451f;
    const atn::Vector3& dir = atn::Vector3(0.4f, 0.2f, 1.0f).GetNormalize();
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    atn::Vector3 d = dir;
    PSMTXMultVec(m, &d, &d);
    SetPosition(target - d * 30.0f);
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 0);
}

// NONMATCHING: 84.8% - stack slots and registers of the temporaries
// inferred: camera mode looking at the hips of the character from 50 above its position
void GameCamera::fn_8038952C(const CharaTask* chara) {
    const atn::Model* model = &chara->mMotion;
    const atn::Matrix3x4* node = (const atn::Matrix3x4*)model->GetNodeMtx_unk(model->GetNodeNoByName("hips", 0));
    atn::Vector3 target = node->GetTrans_unk();
    SetPosition(const_cast<CharaTask*>(chara)->fn_80178088() + atn::Vector3(0.0f, 50.0f, 0.0f));
    SetTarget_unk(target);
    fn_8004B378();
    fn_80392964(1, 1, 1, 0);
}

// inferred: fixed camera mode (position m4FC, target m508, fovy m514; turned and moved with the
// character when m518 is set)
void GameCamera::fn_80389678(const CharaTask* chara) {
    atn::Vector3 pos = m4FC;
    atn::Vector3 target = m508;
    if (m518 != 0) {
        Mtx m;
        float rotY = chara->mRot.y;
        PSMTXRotRad(m, 'y', rotY);
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
    mA0C = 0.0f;
    mA10 = 0.0f;
    mA14 = 0.0f;
    mA18 = 0.0f;
    mA1C = 0.0f;
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

// inferred: default mode: when coming from mode 4 with the camera box flag m910 (and the character without
// m648) the render camera blends back from the current one (mA20, weight 1 decreased by 0.1); when coming from
// the event camera (mode 10) with its timer m424 running, the default camera is blended from the saved one (at
// once when the two horizontal directions of view differ by more than 45 degrees); m98C is cleared
// NONMATCHING: 90.7% - registers of the copy of the camera (as fn_80392D2C) and stack slots and registers of the
// two blends (as fn_8038C928)
void GameCamera::fn_80389A00(const CharaTask* chara) {
    if (m800 == 4 && chara->m648 == 0 && m910 != 0) {
        mA20 = *this;
        mC14 = 1.0f;
        mC18 = 0.1f;
        m800 = m7FC;
        fn_8037EF48(const_cast<CharaTask*>(chara), 1, 1, -1.0f);
        m834 = 0;
        m914 = m918;
        fn_80392964(1, 1, 1, 1);
    } else if (m800 != 10 || !m424.fn_803829C0()) {
        if ((unsigned int)(m800 - 13) > 2 && (unsigned int)(m800 - 10) > 1 && m800 != 2) {
            fn_8037EF48(const_cast<CharaTask*>(chara), 1, 1, -1.0f);
            m800 = m7FC;
        } else {
            m800 = m7FC;
        }
        m834 = 0;
        fn_8037F9AC(chara);
    } else {
        if (m808 == 0) {
            m80C = mPos;
            m818 = mTarget;
            m830 = mFovy;
        }
        m834 = 0;
        fn_8037F9AC(chara);
        m424.fn_803829D4();
        float t = 1.0f - m424.mTime;
        if (m424.mTime < m424.mStep) {
            m424.mActive = 0;
        }
        atn::Vector3 a(mPos.x - mTarget.x, 0.0f, mPos.z - mTarget.z);
        if (PSVECSquareMag(&a) > 0.0f) {
            PSVECNormalize(&a, &a);
        }
        atn::Vector3 b(m80C.x - m818.x, 0.0f, m80C.z - m818.z);
        if (PSVECSquareMag(&b) > 0.0f) {
            PSVECNormalize(&b, &b);
        }
        if (PSVECDotProduct(&a, &b) < 0.70710677f) {
            m424.mActive = 0;
            t = 1.0f;
        }
        SetPosition((mPos - m80C) * t + m80C);
        SetTarget_unk((mTarget - m818) * t + m818);
        mFovy = t * (mFovy - m830) + m830;
        fn_8004B378();
        fn_80392964(1, 1, 1, 1);
    }
    for (int i = 0; i < 8; i++) {
        m98C[i].x = m98C[i].y = m98C[i].z = 0.0f;
    }
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

// inferred: camera at the head of the character (m558.m10 above it) looking m558.m00 in front of it, with the fovy m558.m04;
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
    atn::Vector3 offset(0.0f, m558.m10, 0.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(chara->mRot.y);
    PSMTXMultVec(m.m, &offset, &offset);
    eye += offset;
    offset = atn::Vector3(0.0f, m558.m10, m558.m00);
    PSMTXMultVec(m.m, &offset, &offset);
    atn::Vector3 look = offset + eye;
    pos = (eye - m80C) * ((float)m808 / 5.0f) + m80C;
    target = (look - m818) * ((float)m808 / 5.0f) + m818;
    SetPosition(pos);
    SetTarget_unk(target);
    mFovy = ((float)m808 / 5.0f) * (m558.m04 - m830) + m830;
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


// inferred: camera of the ledge (CharaTask::mC14-mC44): looks along the wall from m594.m00 away, turned by
// m594.m08 and moved sideways by up to m594.m0C towards the free ends of the ledge; blended from the saved camera
// in 15 frames (5 from the sub mode 3)
void GameCamera::fn_8038A5F0(const CharaTask* chara) {
    int frames = 15;
    if (m800 == 3) {
        frames = 5;
    }
    if (m808 == 0) {
        if (m800 == 3) {
            m884 = mPos;
            m890 = mTarget;
        }
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
        m96C = 0.0f;
        m970 = 0.0f;
        m7F4 = 0;
    }
    if (lbl_8087F0A8->mAlwaysBirdview != 0) {
        m800 = m7FC;
        m808 = 0;
        m834 = 0;
        return;
    }
    atn::Vector3 side = Cross_unk(chara->mC14, atn::Vector3(0.0f, 1.0f, 0.0f));
    side.Normalize_unk();
    atn::Vector3 rot = side.ToRotation_unk();
    float offset = 0.0f;
    rot.x += m594.m08;
    unsigned int flags = chara->mC44;
    float length = (chara->mC20 - chara->mC2C).Length_unk();
    float start = (chara->mPos - chara->mC20).Length_unk();
    float end = (chara->mPos - chara->mC2C).Length_unk();
    float t = start / length;
    t = 1.0f < t ? 1.0f : t;
    if ((flags & 1) == 1) {
        if ((flags & 2) == 2) {
            offset = fn_800F8524(-m594.m0C, m594.m0C, t);
        } else {
            offset = t * (lbl_8087DCFC - -m594.m0C) + -m594.m0C;
        }
    } else if ((flags & 2) == 2) {
        offset = t * (m594.m0C - lbl_8087DD00) + lbl_8087DD00;
    }
    float height = m594.m10;
    if (chara->mFlag30) {
        height -= m594.m18;
    }
    atn::Vector3 target = chara->mPos;
    target.y += height;
    target.x += offset * (float)cos(rot.y);
    target.z += -offset * (float)sin(rot.y);
    atn::Vector3 eye(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m = fn_800109E0(rot);
    PSMTXMultVec(m.m, &eye, &eye);
    eye *= -m594.m00;
    eye += target;
    float fovy = m594.m04;
    float k = 0.5f * (1.0f + (float)sin(3.1415927f * ((float)m808 / (float)frames) - 1.5707964f));
    eye = LerpV_unk(m80C, eye, k);
    target = LerpV_unk(m818, target, k);
    float angle = k * (fovy - m830) + m830;
    fn_8038E23C(chara, &m594, &eye, &target);
    SetPosition(eye);
    SetTarget_unk(target);
    mFovy = angle;
    fn_8004B378();
    fn_80392964(1, 1, 1, 1);
    if (++m808 > frames) {
        m80C = eye;
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
        if (PSVECSquareMag(&h) < 0.0001f) {
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

// inferred: camera behind the hips of the character (offset m874, zoomed by mC1C while the global flag 0x6A
// says so), blended in 5 frames through fn_8038AEF0; with mFlag14 clear it uses the fixed m884/m890
// NONMATCHING: 93.7% - copies of m884/m890 and stack slots of the offsets
void GameCamera::fn_8038B148(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
        float x = -m5D0.m0C;
        if (chara->mC48 == 2 || (chara->mC48 == 3 && chara->mFlag29)) {
            x = -x - 0.5f;
        }
        m874.x = x;
        m874.y = m5D0.m10;
        m874.z = 0.0f;
        int id = chara->m50;
        if ((unsigned int)(id - 700013) <= 7 || (unsigned int)(id - 700022) <= 1) {
            m874.x = m874.x > 0.0f ? 6.5f : -6.5f;
        } else if (chara->m680[0] != 0 && chara->m680[0]->m004 % 1000 / 10 == 8) {
            m874.x = x > 0.0f ? 6.5f : -6.5f;
        }
    }
    float fovy = m5D0.m04;
    atn::Vector3 pos;
    atn::Vector3 target;
    if (!chara->mFlag14) {
        pos = m884;
        target = m890;
    } else {
        float ox = m874.x;
        float oy = m874.y;
        const atn::Model* model = &chara->mMotion;
        const atn::Matrix3x4* node =
            (const atn::Matrix3x4*)model->GetNodeMtx_unk(model->GetNodeNoByName("hips", 0));
        atn::Vector3 hips = node->GetTrans_unk();
        atn::Vector3 offset(ox, oy, m5D0.m14);
        float speed = -2.0f;
        if (lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x6A)) {
            speed = 2.0f;
        }
        mC1C += speed;
        mC1C = MIN(MAX(0.0f, mC1C), 10.0f);
        offset.z += mC1C;
        Mtx m;
        PSMTXRotRad(m, 'y', chara->mRot.y);
        PSMTXMultVec(m, &offset, &offset);
        pos = offset + hips;
        offset = atn::Vector3(ox, oy, m5D0.m00);
        PSMTXMultVec(m, &offset, &offset);
        target = offset + hips;
        target.y += m5D0.m18;
    }
    fn_8038AEF0(&pos, &target, chara, 0, fovy, (float)m808 / 5.0f);
    fn_80392964(1, 1, 0, 0);
    if (++m808 > 5) {
        m80C = pos;
        m818 = target;
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
        const atn::Model* model = &chara->mMotion;
        const atn::Matrix3x4* node =
            (const atn::Matrix3x4*)model->GetNodeMtx_unk(model->GetNodeNoByName("hips", 0));
        m9F8 = node->GetTrans_unk();
    }
    m834 = 0;
    m860 = 0;
}

// inferred: aim camera mode: over the shoulder of the character (offset m874 from m60C.m0C/m60C.m10/m60C.m14, set by the
// character type and the weapon; zoomed by mC1C while the global flag 0x6A says so) looking m60C.m00 in front of it,
// blended in 5 frames through fn_8038AEF0. Coming from mode 3 it keeps looking at the point the previous
// camera was aiming at (m890: hit of a ray of 800 along its view)
// NONMATCHING: 92.2% - stack slots: the original keeps the difference of the view of m1F4 lowest on the stack
// (as if the ray were in an inline) and rounds the z of its start (frsp)
void GameCamera::fn_8038B650(const CharaTask* chara) {
    if (m808 == 0) {
        if (m800 == 3) {
            atn::Vector3 dir = (m1F4.mTarget - m1F4.mPos).GetNormalize();
            atn::Vector3 from = m1F4.mPos;
            atn::Vector3 to = from + dir * 800.0f;
            CollResult_unk result;
            atn::Vector3 point = m1F4.mTarget;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 4, (void*)&chara->mColli, 0)) {
                point = result.m04;
            } else {
                point = from + dir * 800.0f;
            }
            m890 = point;
            if (m874.x < 0.0f) {
                m874.x = -m60C.m0C;
            } else {
                m874.x = m60C.m0C;
            }
            m874.y = m60C.m10;
        } else {
            m874.x = -m60C.m0C;
            m874.y = m60C.m10;
        }
        m874.z = m60C.m14;
        if (!chara->mFlag6) {
            m874.x = m874.x > 0.0f ? 10.0f : -10.0f;
            m874.y = 18.0f;
            m874.z -= 20.0f;
        } else {
            switch (chara->m50) {
            case 700013:
            case 700014:
            case 700015:
            case 700016:
            case 700017:
            case 700018:
            case 700019:
            case 700020:
                m874.x = m874.x > 0.0f ? 7.0f : -7.0f;
                break;
            case 700022:
                m874.x = m874.x > 0.0f ? 6.0f : -6.0f;
                m874.y += 10.0f;
                break;
            case 700023:
                m874.x = 3.0f;
                m874.y += 12.0f;
                break;
            default:
                if (chara->m680[0] != 0 && chara->m680[0]->m004 % 1000 / 10 == 8) {
                    m874.x = m874.x > 0.0f ? 6.5f : -6.5f;
                }
                break;
            }
        }
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    float ox = m874.x;
    float oy = m874.y;
    if (chara->mFlag6 && chara->m674 >= 0) {
        oy -= m60C.m18;
    }
    atn::Vector3 offset(ox, oy, m874.z);
    float speed = -2.0f;
    if (lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(0x6A)) {
        speed = 2.0f;
    }
    mC1C += speed;
    mC1C = MIN(MAX(0.0f, mC1C), 10.0f);
    offset.z += mC1C;
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &offset, &offset);
    atn::Vector3 pos = offset + chara->mPos;
    offset = atn::Vector3(ox, oy, m60C.m00);
    PSMTXMultVec(m, &offset, &offset);
    atn::Vector3 target = offset + chara->mPos;
    float fovy = !chara->mFlag6 ? 1.0471976f : m60C.m04;
    if (m800 == 3) {
        atn::Vector3 d = target - pos;
        float dist = PSVECMag(&d);
        atn::Vector3 dir = (m890 - pos).GetNormalize();
        target = dir * dist + pos;
    }
    fn_8038AEF0(&pos, &target, chara, 0, fovy, (float)m808 / 5.0f);
    fn_80392964(1, 1, 0, 0);
    if (++m808 > 5) {
        m80C = pos;
        m818 = target;
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
    m860 = 0;
}

// inferred: side camera mode: from beside the character (m648.m0C to the side, m648.m10 up, m648.m14 behind its target
// mF6C, or halfway to it), blended in 15 frames; then back to the sub mode with m83C = 6
// NONMATCHING: 92.2% - temporaries of the point and the cross product
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
    atn::Vector3 pos = chara->mPos + dir * m648.m14;
    pos.y += m648.m10;
    float side = m648.m0C;
    atn::Vector3 cross;
    PSVECCrossProduct(&dir, &mUp, &cross);
    pos += cross * side;
    fn_8038AEF0(&pos, &target, chara, 1, m648.m04, (float)m808 / 15.0f);
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

// inferred: camera going to the tactical view: m684.m00 behind the point m8A4 (plus the offset m690) turned by
// the pitch m684.m08 and the yaw -m8A0, looking at m8A4; blended with an eased curve in 45 frames (10 without the
// field or in its states 7 and 14); then back to the sub mode with m83C = 6
// NONMATCHING: 98% - stack slots: the original puts the z rotation of fn_800109E0 highest and the x one lowest
// (reversed here; swapping the temporaries of fn_800109E0 only fixes y and costs fn_803918EC), one more saved fpr
void GameCamera::fn_8038C070(const CharaTask* chara) {
    int frames = 45;
    if (lbl_8087F430 == 0 || lbl_8087F430->m54E4 == 7 || lbl_8087F430->m54E4 == 14) {
        frames = 10;
    }
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    atn::Vector3 pos(0.0f, 0.0f, -m684.m00);
    atn::Vector3 rot(m684.m08, -m8A0, 0.0f);
    atn::Matrix3x4 m = fn_800109E0(rot);
    PSMTXMultVec(m.m, &pos, &pos);
    pos += atn::Vector3(m684.m0C, m684.m10, m684.m14);
    pos += m8A4;
    float t = (1.0f - (float)cos(3.1415927f * (float)m808 / (float)frames)) * 0.5f;
    atn::Vector3 eye = (pos - m80C) * t + m80C;
    atn::Vector3 look = (m8A4 - m818) * t + m818;
    float fovy = t * (m684.m04 - m830) + m830;
    if (m8E4 != 0) {
        fn_8038E9B0(chara, &m684, &eye, &look, &look);
    }
    if (eye.x == look.x && eye.z == look.z) {
        eye.x += 0.1f;
    }
    SetPosition(eye);
    SetTarget_unk(look);
    mFovy = fovy;
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
    if (++m808 > frames) {
        m80C = eye;
        m818 = look;
        m868 = look;
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    m834 = 0;
}

// inferred: low camera beside the character (50 above it, m648.m0C to the side) looking at 12 above it; the target and
// the fovy (m7B0.m04) are blended in 10 frames; then back to the sub mode with m83C = 6. The eye position is
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
    atn::Matrix3x4 m;
    m.fn_8013A13C(chara->mRot.y);
    PSMTXMultVec(m.m, &front, &front);
    front = Negate_unk(front);
    pos = chara->mPos;
    pos.y += 50.0f;
    float side = m648.m0C;
    atn::Vector3 cross;
    PSVECCrossProduct(&front, &up, &cross);
    float t = MIN((float)m808 / 10.0f, 1.0f);
    pos += cross * side;
    target = chara->mPos;
    target.y += 12.0f;
    target = (target - m818) * t + m818;
    SetTarget_unk(target);
    mFovy = MIN((float)m808 / 10.0f, 1.0f) * (m7B0.m04 - m830) + m830;
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


// inferred: blends from the current camera back to the box camera of fn_80388A88 while the timer m424 runs (at
// once when the two horizontal directions of view differ by more than 45 degrees)
// NONMATCHING: 89.5% - scheduling of the blends and stack slots
void GameCamera::fn_8038C928(const CharaTask* chara) {
    float t = 1.0f;
    m80C = mPos;
    m818 = mTarget;
    m830 = mFovy;
    if (m424.fn_803829C0()) {
        m424.fn_803829D4();
        t = 1.0f - m424.mTime;
        if (m424.mTime < m424.mStep) {
            m424.mActive = 0;
        }
    }
    atn::Vector3 pos(m738.m0C, m738.m10, m738.m14);
    atn::Vector3 offset(0.0f, m738.m18, 0.0f);
    atn::Vector3 target = const_cast<CharaTask*>(chara)->fn_80178088() + offset;
    float fovy = m738.m04;
    if (mC20 != 0 && mC20->m30 > 0) {
        atn::Vector3 d = pos - target;
        float dist = PSVECMag(&d);
        fovy = 2.0f * (float)atan((float)mC20->m30 * (float)tan(m738.m04 * 0.5f) / dist);
        fovy = fovy < m738.m04 ? fovy : m738.m04;
    }
    atn::Vector3 a(pos.x - target.x, 0.0f, pos.z - target.z);
    if (PSVECSquareMag(&a) > 0.0f) {
        PSVECNormalize(&a, &a);
    }
    atn::Vector3 b(m80C.x - m818.x, 0.0f, m80C.z - m818.z);
    if (PSVECSquareMag(&b) > 0.0f) {
        PSVECNormalize(&b, &b);
    }
    if (PSVECDotProduct(&a, &b) < 0.70710677f) {
        t = 1.0f;
    }
    m808++;
    if (t >= 1.0f) {
        m800 = m7FC;
        m838 = 0;
        m83C = 6;
        m808 = 0;
    }
    float f = t * (fovy - m830) + m830;
    SetPosition((pos - m80C) * t + m80C);
    SetTarget_unk((target - m818) * t + m818);
    mFovy = f;
    fn_8004B378();
    fn_80392964(0, 1, 0, 0);
}

// inferred: event camera at m774.m0C from the head of the character (turned by m774.m28 degrees, facing it), looking
// m774.m00 away with the pitch m774.m08 and the fovy m774.m04; blended with an eased curve while the timer m424 runs
// NONMATCHING: 91.5% - scheduling of the front vector and of the blends
void GameCamera::fn_8038CD94(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    float yaw = 0.017453292f * m774.m28;
    atn::Vector3 offset(m774.m0C, m774.m10, m774.m14);
    Mtx m;
    PSMTXRotRad(m, 'y', 3.1415927f + yaw);
    PSMTXMultVec(m, &offset, &offset);
    atn::Vector3 head = const_cast<CharaTask*>(chara)->fn_80178088();
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Vector3 pos = head + offset;
    Mtx m2;
    PSMTXRotRad(m2, 'x', -m774.m08);
    PSMTXMultVec(m2, &front, &front);
    Mtx m3;
    PSMTXRotRad(m3, 'y', 3.1415927f + yaw);
    PSMTXMultVec(m3, &front, &front);
    atn::Vector3 target = pos + front * m774.m00;
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
    mFovy = t * (m774.m04 - m830) + m830;
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
    atn::Matrix3x4 m;
    float rotY = chara->mRot.y;
    m.fn_8013A13C(rotY);
    PSMTXMultVec(m.m, &front, &front);
    target += front;
    target = (target - m818) * ((float)m808 / 30.0f) + m818;
    atn::Vector3 pos = chara->mPos;
    atn::Vector3 offset(0.0f, 35.0f, -20.0f);
    atn::Matrix3x4 m2;
    m2.fn_8013A13C(chara->mRot.y);
    PSMTXMultVec(m2.m, &offset, &offset);
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
// NONMATCHING: 80.8% - the original adds the offset to the head after the first blend (more float registers)
void GameCamera::fn_8038D4D4(const CharaTask* chara) {
    if (m808 == 0) {
        m80C = mPos;
        m818 = mTarget;
        m830 = mFovy;
    }
    atn::Vector3 target = chara->mPos;
    atn::Vector3 offset(0.0f, -5.0f, 0.0f);
    atn::Vector3 head = const_cast<CharaTask*>(chara)->fn_80178088();
    target = (target - m818) * ((float)m808 / 10.0f) + m818;
    atn::Vector3 dir = (target - (head + offset)).GetNormalize();
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

// NONMATCHING: 89.3% - stack slots and registers of the temporaries
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

// inferred: fixed camera mode that keeps the current sub mode as the previous one
void GameCamera::fn_8038DDE0(const CharaTask* chara) {
    m800 = m7FC;
    fn_80389678(chara);
}

// NONMATCHING: 84.9% - stack slots and registers of the temporaries
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



// inferred: pulls the eye in front of the walls between the character and the eye: the hit point is pushed off the
// wall and swept as a sphere; while it keeps hitting (m89C, up to 8 frames) the eye is blended to it
// NONMATCHING: ~98% - order of the load of param->m10 and the copies of the ray ends; r27/r28 of the hit point
// and result.m10
void GameCamera::fn_8038E23C(const CharaTask* chara, GameCameraParam_unk* param, atn::Vector3* pos,
                             atn::Vector3* target) {
    if (param->m38 == 0) {
        return;
    }
    atn::Vector3 to = *pos;
    atn::Vector3 from = chara->mPos;
    from.y += param->m10;
    if (m7FC == 2 && chara->mFlag30) {
        from.y -= param->m18;
    }
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000008, 0, 0)) {
        m89C++;
        atn::Vector3 p = result.m10;
        p += result.m28;
        atn::Vector3 move(0.0f, 0.0f, 0.0f);
        fn_8004D388(lbl_8087EE98, &result, &p, &move, 0x80000008, 0, 0, 1.0f);
        p = result.m10;
        if (m89C >= 8) {
            *pos = p;
        } else {
            *pos = LerpV_unk(*pos, p, (float)m89C / 8.0f);
        }
    } else if (m7FC == 2) {
        if (m89C > 8) {
            m89C = 8;
        }
        if (m89C > 0) {
            m89C--;
        }
        if (m89C > 0) {
            *pos = LerpV_unk(*pos, mPos, (float)m89C / 8.0f);
        }
    } else {
        m89C = 0;
    }
}

// inferred: keeps the eye out of the walls: on maps with ceilings (MapTask::mEC) the eye is put under the ceiling
// above it, then a ray from the character to the eye (at the same height) pulls it in front of the wall; while it
// keeps hitting (m89C, up to 8 frames) the eye is blended back towards the current camera position
// NONMATCHING: 92.4% - order of the first loads (radius, top) and stack slots of the ray ends
void GameCamera::fn_8038E540(const CharaTask* chara, GameCameraParam_unk* param, atn::Vector3* pos, atn::Vector3* target,
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

// inferred: keeps the eye out of the walls for the bird view (fn_8038C928 and the camera of the default mode): a ray
// from target2 (30 above it, or the collision radius on maps with a ceiling) to the eye at the same height pulls
// the eye in front of the wall; on maps with a ceiling it is tried a second time 10 higher, and while the target
// cannot see the eye (m89C, up to 8 frames) the eye is blended towards the wall
// NONMATCHING: 90.5% - order of the loads of the ray ends (the original reloads from.z, here to.z is forwarded
// with frsp; as fn_8038E540) and the float registers of the radius and the step
void GameCamera::fn_8038E9B0(const CharaTask* chara, GameCameraParam_unk* param, atn::Vector3* pos,
                             atn::Vector3* target, atn::Vector3* target2) {
    bool ceiling = false;
    bool map = false;
    if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0) {
        map = true;
    }
    if (map && lbl_8087F430->GetCurrentMap()->mEC != 0) {
        ceiling = true;
    }
    if (!ceiling) {
        float radius = lbl_8087F0A8->m08C;
        CollResult_unk result;
        atn::Vector3 from = *target2;
        float top = 50.0f + pos->y;
        from.y += 30.0f;
        atn::Vector3 to = *pos;
        to.y = from.y;
        atn::Vector3 dir = to - from;
        PSVECNormalize(&dir, &dir);
        to += dir * radius;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000008, 0, 0)) {
            atn::Vector3 hit = result.m10 - dir * radius;
            hit.y = pos->y;
            atn::Vector3 d = hit - *pos;
            hit.y += MIN(20.0f, PSVECMag(&d));
            hit.y = MIN(top, hit.y);
            *pos = hit;
        }
    } else {
        float radius = lbl_8087F0A8->m08C;
        CollResult_unk result;
        atn::Vector3 from = *target2;
        atn::Vector3 to;
        atn::Vector3 seen = *target;
        from.y += radius;
        to = *pos;
        to.y = from.y;
        atn::Vector3 dir = to - from;
        PSVECNormalize(&dir, &dir);
        to += dir * radius;
        for (int i = 0; i < 2; i++) {
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x08000000, 0, 0)) {
                atn::Vector3 hit = result.m10 - dir * radius;
                hit.y = pos->y;
                atn::Vector3 d = hit - *pos;
                hit.y += MIN(20.0f, PSVECMag(&d));
                *pos = hit;
                break;
            }
            from.y += 10.0f;
            to.y = from.y;
        }
        from = *pos;
        to = *target;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x08000000, 0, 0)) {
            seen = result.m10;
        }
        from = seen;
        to = *pos;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x08000000, 0, 0)) {
            m89C++;
            atn::Vector3 p = result.m10;
            if (m89C < 8) {
                p = LerpV_unk(*pos, p, (float)m89C / 8.0f);
            }
            *pos = p;
        } else {
            m89C = 0;
        }
    }
}

// inferred: sets the parameters of a camera mode (read from the camera csv of the map by fn_8037E12C)
void GameCamera::fn_8038EF94(const GameCameraParam_unk* param, int mode) {
    switch (mode) {
    case 0:
        m51C = *param;
        break;
    case 2:
        m594 = *param;
        break;
    case 3:
        m5D0 = *param;
        break;
    case 4:
        m60C = *param;
        break;
    case 1:
        m558 = *param;
        break;
    case 5:
        m648 = *param;
        break;
    case 8:
        m6C0 = *param;
        break;
    case 9:
        m6FC = *param;
        break;
    case 6:
        m684 = *param;
        break;
    case 10:
        m738 = *param;
        lbl_8087F0A8->mInput.m10 = 3;
        break;
    case 11:
        if (m7FC == mode) {
            m800 = 0;
            m808 = 0;
        }
        m774 = *param;
        break;
    }
}

// ---- fn_8038F52C: camera mode from the action of the character (over the inlining budget: the accessors
// are weak copies) ----
// inferred: accessors of the character and of the singletons (weak copies; C names: the method names are
// unknown)
extern "C" inline int fn_8038B648(CharaTask* chara) {
    return chara->mC48;
}
extern "C" inline int fn_80267B20(CharaTask* chara) {
    return chara->m55C;
}
extern "C" inline int fn_80267B28(CharaTask* chara) {
    return chara->m560;
}
extern "C" inline atn::Model* fn_800F84C8(CharaTask* chara) {
    return (atn::Model*)&chara->mMotion;
}
extern "C" inline chr::ControlBase* fn_80387540(CharaTask* chara) {
    return chara->mControl.get();
}
extern "C" inline HitInfo_unk* fn_80339F6C(CharaTask* chara) {
    return chara->m638;
}
extern "C" inline int fn_80382A00(CharaTask* chara) {
    return chara->m58C;
}
extern "C" inline ChaseManager* fn_803903C4() {
    return lbl_8087F098;
}
extern "C" inline int fn_803903CC(ChaseManager* mgr) {
    return mgr->mState != 0;
}
// inferred: camera part of SetConfig (from mAttackTurnSpeed; as in Event/Path.cpp, weak copies 0x801156C4 and
// 0x801156D8); names invented
struct CameraConfig_unk {
    float m00; // inferred: SetConfig::mAttackTurnSpeed
};
inline SetConfig* GetSetConfig_unk() {
    return SetConfig::sInstance;
}
inline CameraConfig_unk* GetCameraConfig_unk(SetConfig* config) {
    return (CameraConfig_unk*)&config->mAttackTurnSpeed;
}
// inferred: settings (lbl_8087F0A8) and field (lbl_8087F430) as the accessors of other files see them (as in
// game/CharaTask.cpp and game/GameUI.cpp)
struct Settings_unk {
    Input_unk* fn_80116BAC(); // inferred: pad (this + 0x48C)
};
extern "C" Settings_unk* fn_800F52F8(); // inferred: returns lbl_8087F0A8
struct Field_unk {
    GameScene* fn_8013C504(); // inferred: scene of the field
};
extern "C" Field_unk* fn_80121F00(); // inferred: returns lbl_8087F430

inline void GameCamera::fn_80390374() {
    fn_80389838(m804);
}

// inferred: chooses the camera mode from the state (m55C) and the control (m560) of `chara` (GameTask): hanging,
// attacks, special motions; some controls change the parameters of the hips mode (m51C) through m46C
void GameCamera::fn_8038F52C(CharaTask* chara) {
    int set = 0;
    if (m7FC != 6 && m7FC != 19) {
        if (chara->fn_8013C3A8()) {
            if (fn_8038B648(chara)) {
                fn_80389838(3);
            } else {
                fn_80389838(2);
            }
        } else if (chara->fn_801479D8()) {
            fn_80389838(4);
        } else if (fn_80267B20(chara) == 6) {
            switch (fn_80267B28(chara)) {
            case 13:
            case 33:
            case 34:
                fn_80389838(5);
                break;
            case 14: {
                float frame = fn_800F84C8(chara)->GetMotionFrame(0);
                float len = fn_800F84C8(chara)->fn_80097D7C(0);
                GameCameraParam_unk p(m51C);
                p.m00 *= 1.6f;
                p.m04 *= 1.1f;
                p.m08 = atn::DegToRad_unk(25.0f);
                m46C.fn_8037E89C(3, p, 1.0f);
                m46C.mParam.m00 = p.m00;
                set = 1;
                m46C.mParam.m04 = p.m04;
                m46C.mParam.m08 = p.m08;
                fn_80390374();
                fn_8037EF30(0, 0, -1.0f);
                break;
            }
            case 29:
                if (fn_80387540(chara) && fn_80387540(chara)->vf58()) {
                    fn_80389838(4);
                } else {
                    fn_80389838(5);
                }
                break;
            case 30:
            case 31:
                if (fn_80339F6C(chara) && (int)fn_80339F6C(chara)->m02 == 4) {
                    fn_80390374();
                }
                break;
            case 11:
            case 15:
            case 16:
                fn_80389838(7);
                break;
            case 8:
            case 24:
            case 25:
            case 26:
            case 27:
            case 41:
            case 58:
            case 84:
                fn_80390374();
                fn_8037EF30(0, 1, -1.0f);
                break;
            case 23:
                fn_80390374();
                if (fn_80382A00(chara) != 2 || !chara->fn_8015EB2C()) {
                    fn_8037EF30(0, 0, -1.0f);
                }
                break;
            case 40: {
                GameCameraParam_unk p(m51C);
                p.m0C = 0.0f;
                m46C.fn_8037E89C(2, p, 0.1f);
                set = 1;
                fn_80390374();
                fn_8037EF30(0, 0, -1.0f);
                break;
            }
            case 4:
            case 48:
            case 50:
                fn_80390374();
                fn_8037EF30(0, 0, GetCameraConfig_unk(GetSetConfig_unk())->m00);
                break;
            case 113:
            case 115:
                fn_80389838(13);
                break;
            case 76:
                fn_80389838(14);
                break;
            case 77:
                fn_80389838(15);
                break;
            case 81:
                fn_80389838(20);
                fn_8037EF30(1, 1, -1.0f);
                break;
            case 110:
            case 111:
            case 112:
                fn_80390374();
                fn_8037EF30(1, 1, -1.0f);
                break;
            case 9:
            case 10:
            case 95: {
                GameCameraParam_unk p(m51C);
                p.m04 *= 0.7f;
                m46C.fn_8037E89C(3, p, 0.1f);
                set = 1;
                fn_80390374();
                fn_8037EF30(0, 0, -1.0f);
                break;
            }
            case 141: {
                float frame = fn_800F84C8(chara)->GetMotionFrame(0);
                float len = fn_800F84C8(chara)->fn_80097D7C(0);
                GameCameraParam_unk p(m51C);
                if (fn_800F84C8(chara)->fn_80139F2C(0) == 0x16E) {
                    float from = -atn::DegToRad_unk(20.0f);
                    float to = atn::DegToRad_unk(10.0f);
                    if (frame > 33.0f) {
                        p.m08 += fn_800F8524(from, to, GAMECAMERA_MIN_UNK(1.0f, (frame - 23.0f) / 4.0f));
                    } else if (frame > 14.0f) {
                        p.m08 += fn_800F8524(lbl_8087DD04, from, GAMECAMERA_MIN_UNK(1.0f, (frame - 14.0f) / 3.0f));
                    }
                    p.m04 *= 0.7f;
                } else if (fn_800F84C8(chara)->fn_80139F2C(0) == 0x170) {
                    float from = -atn::DegToRad_unk(10.0f);
                    float to = atn::DegToRad_unk(5.0f);
                    if (frame > 33.0f) {
                        p.m08 += fn_800F8524(from, to, GAMECAMERA_MIN_UNK(1.0f, (frame - 23.0f) / 4.0f));
                    } else if (frame > 14.0f) {
                        p.m08 += fn_800F8524(lbl_8087DD08, from, GAMECAMERA_MIN_UNK(1.0f, (frame - 14.0f) / 3.0f));
                    }
                    if (frame > len - 13.0f) {
                        float t = GAMECAMERA_MIN_UNK(1.0f, GAMECAMERA_MAX_UNK(0.0f, (frame - (len - 13.0f)) / 12.0f));
                        p.m0C = fn_800F8524(lbl_8087DD0C, p.m0C, t);
                        p.m34 = fn_800F8524(-atn::DegToRad_unk(7.0f), p.m34, t);
                    } else {
                        float t = GAMECAMERA_MAX_UNK(0.0f, GAMECAMERA_MIN_UNK(1.0f, (frame - 16.0f) / 10.0f));
                        p.m0C = fn_800F8524(p.m0C, lbl_8087DD10, t);
                        p.m34 = fn_800F8524(p.m34, -atn::DegToRad_unk(7.0f), t);
                    }
                    p.m04 *= 0.6f;
                } else {
                    float from = -atn::DegToRad_unk(15.0f);
                    float to = atn::DegToRad_unk(0.0f);
                    if (frame > len - 10.0f) {
                        p.m00 *= fn_800F8524(lbl_8087DD14, lbl_8087DD18,
                                             GAMECAMERA_MIN_UNK(1.0f, GAMECAMERA_MAX_UNK(0.0f, (frame - (len - 10.0f)) / 9.0f)));
                        p.m08 = fn_800F8524(m46C.mParam.m08, p.m08,
                                            GAMECAMERA_MIN_UNK(1.0f, (frame - (len - 10.0f)) / 6.0f));
                    } else if (frame > 33.0f) {
                        p.m08 += fn_800F8524(from, to, GAMECAMERA_MIN_UNK(1.0f, (frame - 23.0f) / 4.0f));
                        p.m00 *= fn_800F8524(lbl_8087DD1C, lbl_8087DD20,
                                             GAMECAMERA_MIN_UNK(1.0f, GAMECAMERA_MAX_UNK(0.0f, (frame - 33.0f) / 5.0f)));
                    } else {
                        p.m08 += fn_800F8524(lbl_8087DD24, from,
                                             GAMECAMERA_MIN_UNK(1.0f, GAMECAMERA_MAX_UNK(0.0f, (frame - 14.0f) / 3.0f)));
                        p.m00 *= fn_800F8524(lbl_8087DD28, lbl_8087DD2C,
                                             GAMECAMERA_MIN_UNK(1.0f, GAMECAMERA_MAX_UNK(0.0f, frame / 5.0f)));
                    }
                }
                m46C.fn_8037E89C(3, p, 0.1f);
                m46C.mParam.m08 = GAMECAMERA_MAX_UNK(-atn::DegToRad_unk(25.0f), p.m08);
                set = 1;
                m46C.mParam.m04 = p.m04;
                m46C.mParam.m00 = p.m00;
                m46C.mParam.m0C = p.m0C;
                m46C.mParam.m34 = p.m34;
                fn_80390374();
                fn_8037EF30(0, 1, -1.0f);
                break;
            }
            case 121: {
                GameCameraParam_unk p(m51C);
                p.m00 = 45.0f;
                p.m28 = 0.3f;
                p.m0C = 25.0f;
                p.m10 = 10.0f;
                m46C.fn_8037E89C(3, p, 0.1f);
                set = 1;
                fn_80390374();
                break;
            }
            case 138: {
                GameCameraParam_unk p(m51C);
                p.m00 = 45.0f;
                p.m28 = 0.3f;
                p.m0C = 25.0f;
                p.m10 = 10.0f;
                m46C.fn_8037E89C(3, p, 0.1f);
                set = 1;
                fn_80390374();
                break;
            }
            case 132: {
                GameCameraParam_unk p(m51C);
                if (fn_800F84C8(chara)->GetMotionFrame(0) < 22.0f) {
                    p.m00 *= 0.6f;
                } else if (fn_800F84C8(chara)->GetMotionFrame(0) > 41.0f) {
                    p.m00 *= GAMECAMERA_MAX_UNK(1.0f, 1.2f - 0.03f * (fn_800F84C8(chara)->GetMotionFrame(0) - 41.0f));
                } else {
                    p.m00 *= GAMECAMERA_MIN_UNK(1.2f, 0.15f * (fn_800F84C8(chara)->GetMotionFrame(0) - 22.0f) + 0.6f);
                }
                m46C.fn_8037E89C(3, p, 0.1f);
                m46C.mParam.m00 = p.m00;
                set = 1;
                m46C.mParam.m0C = p.m0C;
                fn_80390374();
                fn_8037EF30(0, 0, -1.0f);
                break;
            }
            case 139: {
                GameCameraParam_unk p(m51C);
                if (fn_800F84C8(chara)->GetMotionFrame(0) > fn_800F84C8(chara)->fn_80097D7C(0) - 8.0f) {
                    p.m00 *= fn_800F8524(lbl_8087DD30, lbl_8087DD34,
                                         GAMECAMERA_MIN_UNK(1.0f, (fn_800F84C8(chara)->GetMotionFrame(0) -
                                                                   (fn_800F84C8(chara)->fn_80097D7C(0) - 8.0f)) /
                                                                      8.0f));
                } else {
                    p.m00 *= 0.6f;
                }
                m46C.fn_8037E89C(3, p, 0.1f);
                m46C.mParam.m00 = p.m00;
                set = 1;
                m46C.mParam.m0C = p.m0C;
                fn_80390374();
                fn_8037EF30(0, 0, -1.0f);
                break;
            }
            case 129:
                break;
            default:
                fn_80390374();
                break;
            }
        } else if (fn_803830A0() && fn_800F52F8()->fn_80116BAC()->fn_801240B4(0x24)) {
            fn_80389838(9);
        } else if (fn_803903C4() && fn_803903CC(fn_803903C4())) {
            fn_80389838(8);
        } else if (fn_80121F00() && fn_80121F00()->fn_8013C504()) {
            gs::CameraBox* box = fn_80121F00()->fn_8013C504()->fn_803C2048(chara);
            if (box) {
                if (box->m2C == 1) {
                    fn_80389838(10);
                } else if (box->m2C == 2) {
                    fn_80389838(11);
                } else {
                    fn_80390374();
                }
            } else {
                fn_80390374();
            }
        } else {
            fn_80390374();
        }
    }
    if (!set && m46C.mMode != 4) {
        fn_8032EC94(&m46C.mMode);
    }
}


// inferred: starts the tactical view camera at `target` (m8A4) turned by `angle` (m8A0) around the raised point;
// with `arg` (m8CC) and a ceiling, the start point is pulled out of the nearest wall found by 8 rays of 30
// around it (the wall must not be crossed from the other side as well)
// NONMATCHING: 98.4% - register allocation (the map/ceiling flags of the second check, the loop pointers) and
// stack slots of the rotated offset and of the start point
void GameCamera::fn_803903E0(const atn::Vector3* target, float angle, int arg) {
    m8A4 = *target;
    atn::Vector3 head = *target;
    m8A0 = angle;
    m8CC = arg;
    m8BC = *target;
    m8C8 = target->y;
    head.y += lbl_8087F0A8->m08C;
    AddViewHeight_unk(&head);
    atn::Vector3 d(m8A4.x - head.x, 0.0f, m8A4.z - head.z);
    float len = PSVECMag(&d);
    d.x = 0.0f;
    d.y = 0.0f;
    d.z = len;
    Mtx m;
    PSMTXRotRad(m, 'y', -m8A0);
    PSMTXMultVec(m, &d, &d);
    m8A4 = atn::Vector3(head.x + d.x, m8A4.y, head.z + d.z);
    if (m8CC != 0) {
        m8CC = 0;
        if (HasCeiling_unk()) {
            atn::Vector3 from = m8A4;
            float radius = lbl_8087F0A8->m08C;
            from.y += radius;
            AddViewHeight_unk(&from);
            bool found = false;
            CollResult_unk best;
            float bestDist = 1.0e16f;
            unsigned int mask = 0x08000000;
            const atn::Vector3* start = &m8A4;
            for (int i = 0; i < 8; i++) {
                atn::Vector3 dir(0.0f, 0.0f, 30.0f);
                Mtx r;
                PSMTXRotRad(r, 'y', 0.7853982f * (float)i);
                PSMTXMultVec(r, &dir, &dir);
                CollResult_unk back;
                CollResult_unk result;
                atn::Vector3 a = *start;
                atn::Vector3 b = *start + dir;
                if (fn_8004ED34(lbl_8087EE98, &result, &a, &b, mask, 0, 0)) {
                    int hit = fn_8004ED34(lbl_8087EE98, &back, &b, &a, mask, 0, 0);
                    atn::Vector3 e = back.m04 - a;
                    float backDist = PSVECMag(&e);
                    atn::Vector3 f = result.m04 - a;
                    float dist = PSVECMag(&f);
                    if (hit == 0 || dist < backDist - 1.0f) {
                        found = true;
                        if (bestDist > dist) {
                            bestDist = dist;
                            best = result;
                        }
                    }
                }
            }
            if (found) {
                m8A4 = best.m04 + best.m28 * radius + atn::Vector3(m8A4.x - from.x, 0.0f, m8A4.z - from.z);
            }
        }
    }
}

// ---- fn_80390A88: free camera of the tactical view ----
extern "C" int fn_800A55D4(void* pad, int port, int button); // inferred: button held
// inferred: parts of TacView not declared in game/TacView.h (names invented): the object aimed at by the cursor
// keeps its position at 0x10, and m15AC is a task with a state at 0x48
struct TacViewTargetPos_unk {
    unsigned char m00[0x10];
    atn::Vector3 m10;
};
struct TacViewTask_unk {
    int IsIdle_unk() const { return m48 == 0; }
    unsigned char m00[0x48];
    int m48;
};

// inferred: normalizes `v` unless it is (almost) zero
static inline void SafeNormalize_unk(atn::Vector3* v) {
    if ((float)__fabs(v->LengthSq_unk()) < 1.1920929e-07f) {
        return;
    }
    v->Normalize_unk();
}

// inferred: the view moves to its point (TacView::m159C)
static inline bool TacViewMoving_unk(TacView* view) {
    bool moving = false;
    if (view->m15D8 == 1 && view->m15A8 > 0) {
        moving = true;
    }
    return moving;
}

// inferred: `p` raised by the radius of the eye (and by the view height on maps without a ceiling)
static inline atn::Vector3 RaisedPos_unk(const atn::Vector3& p) {
    atn::Vector3 r = p;
    r.y += lbl_8087F0A8->m08C;
    AddViewHeight_unk(&r);
    return r;
}

// inferred: free camera of the tactical view (m8E0): the left stick moves the point m8A4 (relative to the yaw m8A0)
// within m8E8 ? 800 : 400 of m8BC (the view point of TacView while it moves it), swept against the walls (three
// tries up the steps on maps with a ceiling); while the view is open the point follows the cursor; the right
// stick and the buttons 2/3 turn the yaw
// NONMATCHING: ~63% - stack layout (the original has 0x10 more bytes of locals: Mtx at 0x168, move at 0x158,
// the copy of the cursor split over 0x140/0x0C/0x1C) and the scheduling of the first vector sums
void GameCamera::fn_80390A88() {
    if (m8E0 == 0) {
        return;
    }
    atn::Vector3 move;
    move.x = -fn_800A56A8(lbl_8087EF70, 0, 0, 0);
    move.y = 0.0f;
    move.z = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
    if ((float)__fabs(move.x) < 0.3f) {
        move.x = 0.0f;
    }
    if ((float)__fabs(move.z) < 0.3f) {
        move.z = 0.0f;
    }
    int free = lbl_8087F9F8->mA70 == 0;
    float radius = lbl_8087F0A8->m08C;
    atn::Vector3 pos = m8A4;
    pos.y += radius;
    AddViewHeight_unk(&pos);
    if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
        TacViewSel_unk sel = lbl_8087F9F8->m48;
        if (lbl_8087F9F8->m15D8 == 1) {
            free = 1;
        }
        lbl_8087F9F8->fn_805AA94C(&m8A4, &move, free, m8D4);
        if (m8D4 != 0) {
            m8D4 = 0;
        }
    }
    Mtx m;
    PSMTXRotRad(m, 'y', -m8A0);
    PSMTXMultVec(m, &move, &move);
    move *= lbl_8087F0A8->m064;
    if (free) {
        float minDist = 0.001f;
        float maxDist = m8E8 ? 800.0f : 400.0f;
        if (m8E8) {
            maxDist *= 2.0f;
        }
        atn::Vector3 center = m8BC;
        int follow = 1;
        if (lbl_8087F9F8->m15D8 == 1) {
            if (!TacViewMoving_unk(lbl_8087F9F8)) {
                minDist = 0.001f;
                maxDist = lbl_8087F9F8->m15ED ? 600.0f : 200.0f;
                center = lbl_8087F9F8->m15DC;
            } else {
                follow = 0;
            }
        }
        if (follow) {
            atn::Vector3 eye = m8A4;
            atn::Vector3 dir = move;
            atn::Vector3 d = eye + dir - center;
            if (PSVECMag(&d) > 0.0f) {
                eye += move;
            } else {
                dir = eye + dir - center;
                dir.y = 0.0f;
                SafeNormalize_unk(&dir);
                eye = dir * (0.00001f + minDist) + center;
            }
            dir = eye - center;
            if (maxDist < PSVECMag(&dir)) {
                PSVECNormalize(&dir, &dir);
                eye = center + dir * maxDist;
            }
            eye.y = GAMECAMERA_MAX_UNK(center.y - 10.0f, GAMECAMERA_MIN_UNK(10.0f + center.y, eye.y));
            pos = RaisedPos_unk(m8A4);
            move = eye - pos;
            move.y = 0.0f;
        }
    }
    unsigned int mask = 0x80000008;
    if (HasCeiling_unk()) {
        mask = 0x08000000;
        pos.y += m8C8 - m8A4.y;
        move.y = 0.5f * -radius + move.y;
    }
    if (free) {
        if (lbl_8087F9F8 != 0 && TacViewMoving_unk(lbl_8087F9F8)) {
            lbl_8087F9F8->fn_805AA82C(&m8A4);
        } else {
            CollResult_unk result;
            if (HasCeiling_unk()) {
                int tries = 0;
                bool aimed = false;
                bool sub = false;
                if (lbl_8087F9F8 != 0 && lbl_8087F9F8->m15AC != 0) {
                    sub = true;
                }
                if (sub && !((TacViewTask_unk*)lbl_8087F9F8->m15AC)->IsIdle_unk()) {
                    aimed = true;
                }
                int floor = aimed ? lbl_8087F9F8->m15DC.y : m8BC.y;
                do {
                    if (fn_8004D388(lbl_8087EE98, &result, &pos, &move, mask, 0, -1, radius)) {
                        atn::Vector3 d = result.m10 - pos;
                        if (result.m10.y - (float)floor > 20.0f) {
                            atn::Vector3 from = result.m10;
                            atn::Vector3 to = result.m10;
                            to.y -= 2.0f * radius;
                            tries++;
                            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, mask, 0, -1) &&
                                result.m28.x * result.m28.x + result.m28.z * result.m28.z > 0.001f &&
                                move.x * move.x + move.z * move.z > 0.001f) {
                                atn::Vector3 n(-result.m28.x, 0.0f, -result.m28.z);
                                PSVECNormalize(&n, &n);
                                move.y = 0.0f;
                                float dot = PSVECDotProduct(&n, &move);
                                move -= n * dot;
                                move.y = 0.2f * -radius;
                            } else {
                                move *= 0.5f;
                            }
                        } else {
                            m8C8 += d.y;
                            if (m8C8 < m8A4.y) {
                                m8C8 = m8A4.y;
                            }
                            d.y = 0.0f;
                            m8A4 += d;
                            break;
                        }
                    } else {
                        move.y = 0.0f;
                        m8A4 += move;
                        break;
                    }
                    pos = m8A4;
                    pos.y = m8C8 + radius;
                } while (tries < 3);
            } else {
                fn_8004D388(lbl_8087EE98, &result, &pos, &move, mask, 0, 0, radius);
                atn::Vector3 d = result.m10 - pos;
                d.y = 0.0f;
                m8A4 += d;
            }
        }
    } else if (lbl_8087F9F8->m48.mState == 0) {
        m8A4 += (lbl_8087F9F8->m48.m04->GetTacViewPosition() - m8A4) * 0.2f;
    } else if (lbl_8087F9F8->m48.mState == 1) {
        m8A4 += (((TacViewTargetPos_unk*)lbl_8087F9F8->m48.mTarget)->m10 - m8A4) * 0.2f;
    }
    float turn = 0.0f;
    if (fn_800A58D0(lbl_8087EF70, 0)) {
        float x = -fn_800A56A8(lbl_8087EF70, 0, 2, 0);
        if ((float)__fabs(x) < 0.3f) {
            x = 0.0f;
        }
        turn = x * -0.08726646f;
    }
    if (lbl_8087F9F8 == 0 || lbl_8087F9F8->mA70 == 0 || lbl_8087F9F8->mA78 != 0) {
        if (fn_800A55D4(lbl_8087EF70, 0, 2)) {
            turn -= 0.08726646f;
        }
        if (fn_800A55D4(lbl_8087EF70, 0, 3)) {
            turn += 0.08726646f;
        }
    }
    if ((float)__fabs(turn) > 0.001f) {
        m8A0 = atn::WrapAngle_unk(turn + m8A0);
    }
}


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
