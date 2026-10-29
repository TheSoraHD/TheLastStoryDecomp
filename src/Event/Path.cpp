// Event cameras (EventCamera_unk, name invented) and their paths (Event::Path). The camera of an
// event looks from an eye target to a look target (actors, nodes, offsets or paths), moves
// towards them over the time curve of the event manager, can be turned and zoomed with the pad,
// shaken, and driven by a camera animation (Xsca).
#include "Event/Path.h"
#include "Event/MotionData.h"
#include "atn/Model_unk.h"
#include "game/SetConfig.h"
#include "atn/SoundCore.h"
#include "game/Input_unk.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

namespace Event {

struct Actor_unk {
    atn::Model* fn_80012A1C();   // inferred: model of the actor (0 if none)
    atn::Vector3 fn_80010374();  // inferred: position of the actor
    atn::Vector3 fn_80010B68();  // inferred: rotation of the actor
};

} // namespace Event

// inferred: the event manager (lbl_8087F540, Event/Manager.cpp); names invented
struct EventMgr_unk;
extern "C" int fn_8048B048(EventMgr_unk* mgr, int id);              // inferred: time curve by id
extern "C" int fn_8048B3B8(EventMgr_unk* mgr, int id);
extern "C" float fn_8048BCA4(EventMgr_unk* mgr, int curve, float t); // inferred: value of a time curve
extern "C" float fn_8048BD04(EventMgr_unk* mgr, int curve, float t);
struct EventMgr_unk {
    // inferred: weak copies 0x80112F54 and 0x801130F4
    Event::DataOwner_unk* fn_80112F54() { return m70; }
    int fn_801130F4() { return mC4; }

    unsigned char m00[0x70];
    Event::DataOwner_unk* m70; // inferred: current scene
    unsigned char m74[0xB4 - 0x74];
    float mB4;   // inferred: time of a frame
    unsigned char mB8[0xC4 - 0xB8];
    int mC4;     // inferred: the cameras are updated at once
    unsigned char mC8[0x1EAC - 0xC8];
    int m1EAC;   // inferred: frame counter
};
extern "C" EventMgr_unk* lbl_8087F540;
extern "C" EventMgr_unk* fn_8000DB1C(); // inferred: returns lbl_8087F540 (weak copy of another file)

// inferred: screen (lbl_8087EEE0)
struct Screen_unk;
extern "C" Screen_unk* lbl_8087EEE0;
extern "C" float fn_80076FF8(Screen_unk* screen); // inferred: aspect ratio

// inferred: game state (lbl_8087EFA8); names invented
struct EventGameState_unk {
    unsigned char m000[0xD4];
    int mD4;    // inferred: the fog of the event is used
    unsigned char m0D8[0xE0 - 0xD8];
    int mE0;
    float mE4;
    float mE8;
    float mEC;
    float mF0;
    unsigned char m0F4[0xFC - 0xF4];
    float mFC;
    float m100;
    unsigned char m104[0x1C0 - 0x104];
    float m1C0; // inferred: near plane
    float m1C4; // inferred: far plane
};
extern "C" EventGameState_unk* lbl_8087EFA8;

// inferred: camera animation curves (as in game/Em309Ghoul.cpp)
struct XsfCurveSet_unk {
    void fn_805BF410(float time, float* out, int x, int y, int z);
    unsigned char m00[0x14];
    float m14; // inferred: length
};
struct Xsca_unk {
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
};

extern "C" int rand();
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" double atan2(double y, double x);
extern "C" void fn_805F9AB0(atn::Quaternion_unk* q, const atn::Vector3* axis, float rad); // inferred: C_QUATRotAxisRad
extern "C" void PSQUATMultiply(const atn::Quaternion_unk* a, const atn::Quaternion_unk* b, atn::Quaternion_unk* ab);
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length

// inferred: pad (lbl_8087EF70)
extern "C" void* lbl_8087EF70;
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" int fn_800A58D0(void* pad, int port);                     // inferred: second stick available

// inferred: camera part of SetConfig (from mAttackTurnSpeed); names invented
struct CameraConfig_unk {
    // inferred: weak copies 0x801156CC, 0x801156E0 and 0x801156F8
    int IsInvertX_unk() { return (m10 & 4) == 4; }
    float GetRotScale_unk() { return 0.3f * (m0C - 1.0f) + 1.0f; }
    int IsInvertY_unk() { return (m10 & 8) == 8; }
    float m00;
    float m04;
    float m08;
    float m0C; // inferred: SetConfig::mCameraRotSpeed
    unsigned int m10;
};
// inferred: weak copies 0x801156BC, 0x801156C4 and 0x801156D8
inline void* GetPad_unk() {
    return lbl_8087EF70;
}
inline SetConfig* GetSetConfig_unk() {
    return SetConfig::sInstance;
}
inline CameraConfig_unk* GetCameraConfig_unk(SetConfig* config) {
    return (CameraConfig_unk*)&config->mAttackTurnSpeed;
}
// inferred: fabs (weak copy of another file)
extern "C" inline float fn_80011220(float x) {
    return __fabs(x);
}

// inferred: settings (lbl_8087F0A8) and their pad input
struct Settings_unk {
    // inferred: weak copy 0x80116BAC
    Input_unk* fn_80116BAC() { return (Input_unk*)m48C; }
    unsigned char m000[0x48C];
    unsigned char m48C[1];
};
extern "C" Settings_unk* fn_800F52F8(); // inferred: returns lbl_8087F0A8 (weak copy of another file)

// inferred: lerp of two floats
extern "C" float fn_800F8524(const float* a, const float* b, float t);
// inferred: rotation matrix from angles (x, y, z)
extern "C" atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot);
// inferred: current camera of the game
extern "C" Camera_unk* fn_8008B964();

// inferred: sound manager (lbl_8087EFE8); names invented
struct SoundMgr_unk {
    // inferred: weak copy 0x80116E74
    void SetListener_unk(const atn::Vector3& pos, const atn::Vector3& rot) { m2984.fn_800C7F08(pos, rot); }
    unsigned char m0000[0x2984];
    atn::Sound3dListener m2984;
};
extern "C" SoundMgr_unk* lbl_8087EFE8;
// inferred: weak copy 0x80116E64
inline SoundMgr_unk* GetSoundMgr_unk() {
    return lbl_8087EFE8;
}

// inferred: tuning of the pad zoom (lbl_8087D980/lbl_8087D984)
static float sZoomMin = 0.2f;
static float sZoomMax = 1.0f;

// inferred: path helpers of game/PathData_unk.cpp
extern "C" atn::Vector3 fn_8054856C(Event::Path* path, float t);
extern "C" void fn_80547EA0(Event::Path* path, Event::PathPoint_unk* point);
extern "C" void fn_8054875C(Event::Path* path);

// inferred: translation of a matrix (name invented)
inline atn::Vector3 GetMtxPos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: splits `v` into its absolute value and its sign
void fn_80112220(const float& v, float& abs, float& sign) {
    sign = v > 0.0f ? 1.0f : -1.0f;
    abs = atn::Abs_unk(v);
}

namespace Event {

// inferred: not inline (its "" is in the @stringBase0 pool of this file); inlined everywhere and
// stripped by the linker
CommonData::CommonData() : m04(0), m08(""), m14(0) {}

// NONMATCHING: 99.8% - the original addresses the string pool by its label (lis order of the vtable and the pool swapped)
EventCamera_unk::EventCamera_unk()
    : m274(0.0f, 0.0f, 1.0f), m3A4(), m400(0), m404(1.0f), m408(1.0f), m40C(1.0f), m410(1.0f), m414(0.0f),
      m418(0.0f), m41C(0), m420(0), m424(0.0f), m428(0.0f), m42C(0.0f), m430(0.0f), m434(1.5f), m438(1.5f),
      m43C(0), m638(0.0f), m63C(-1.0f) {
    m204 = lbl_807C7030;
    m228 = lbl_807C7030;
    m25C = lbl_807C7030;
    m268 = lbl_807C7030;
    m258 = 0.0f;
    m1F4 = 0;
    m1F8 = 0;
    m280 = 0;
    m282 = 0;
    m284 = 0;
    m286 = 0;
    m288 = 0.0f;
    m3FC = -1.0f;
    m3F0 = lbl_807C7030;
    m1FC = 1.0471976f;
    m200 = 0.05f;
    mPos.x = 200.0f;
    mPos.y = 200.0f;
    mPos.z = 200.0f;
    mTarget.x = 0.0f;
    mTarget.y = 0.0f;
    mTarget.z = 0.0f;
    fn_80112840();
    m28C = -1;
    m290 = -1;
    m640 = 0;
}

void EventCamera_unk::fn_80112784() {
    m1F4 = 0;
    fn_80112840();
    m400 = 0;
    m404 = 1.0f;
    m40C = 1.0f;
    m410 = 1.0f;
    m408 = 0.0f;
    m41C = 0;
    m3FC = -1.0f;
    mBits.mFlag2 = 0;
}

void EventCamera_unk::fn_80112840() {
    if (lbl_8087EEE0) {
        mAspect = fn_80076FF8(lbl_8087EEE0);
    }
    if (lbl_8087EFA8) {
        mNear = lbl_8087EFA8->m1C0;
        mFar = lbl_8087EFA8->m1C4;
    }
    mFovy = m1FC;
    m644 = m648 = m64C = 0.0f;
    m650 = m654 = m658 = 0.0f;
}

void EventCamera_unk::fn_801128C4(int resetAngles) {
    m288 = 0.0f;
    m280 = m284;
    m282 = m286;
    m284 = 0;
    m286 = 0;
    mBits.mFlag0 = 0;
    if (resetAngles) {
        m25C = lbl_807C7030;
    }
}

void EventCamera_unk::fn_80112918(short a, short b, int id) {
    m284 = a;
    m286 = b;
    m28C = fn_8048B3B8(lbl_8087F540, id);
}

void EventCamera_unk::fn_80112974(int on, float a, float b) {
    mBits.mFlag2 = on != 0;
    m3A4 = lbl_807C7030;
    m3B0 = a;
    m3B4 = b;
}

void EventCamera_unk::fn_801129B4() {
    m1F4 = 0;
}

void EventCamera_unk::fn_801129C0(float t) {
    m294 = MAX(0.0f, MIN(1.0f, t));
}

// NONMATCHING: 99.8% - src/dst registers of a Vector3 copy swapped (mwcc-blockers q3)
atn::Vector3 EventCamera_unk::fn_80112A00(const CameraTarget_unk& target, float t) {
    if (target.mPath) {
        return fn_8054856C(target.mPath, t);
    }
    atn::Model* model = target.mActor->fn_80012A1C();
    MtxPtr mtx = model ? model->GetNodeMtx_unk(model->fn_800928B0(target.mNode, 0)) : 0;
    atn::Vector3 base;
    if (mtx) {
        base = GetMtxPos_unk(mtx);
    } else {
        base = target.mActor->fn_80010374();
    }
    atn::Vector3 offset = target.mOffset;
    Mtx m;
    PSMTXRotRad(m, 'y', target.mActor->fn_80010B68().y);
    PSMTXMultVec(m, &offset, &offset);
    return base + offset;
}

// NONMATCHING: 93.2% - push_back of the path points: the store of the size is scheduled after the capacity check
void EventCamera_unk::fn_80112B64(const atn::Vector3& pos, Path path, int keep) {
    DataOwner_unk* owner = lbl_8087F540->fn_80112F54();
    if (owner == 0) {
        return;
    }
    if (!keep) {
        int n = path.mPoints.size();
        for (int i = 0; i < n; i++) {
            fn_80547EA0(&path, &path.mPoints[0]);
        }
    }
    path.m04 = owner;
    PathPoint_unk point;
    point.mPos = pos;
    point.mRotY = 0.0f;
    path.mPoints.push_back(point);
    fn_8054875C(&path);
    if (path.mPoints.size() > 2) {
        fn_80547EA0(&path, &path.mPoints[0]);
    }
}

void EventCamera_unk::fn_80112F5C(const CameraTarget_unk& eye, const CameraTarget_unk& at, int resetAngles, int keep,
                                  int id, float roll) {
    fn_801128C4(resetAngles);
    m290 = fn_8048B048(fn_8000DB1C(), id);
    m1F4 = 1;
    m210 = eye;
    m240 = at;
    fn_80112B64(fn_80112A00(m210, 0.0f), m29C, keep);
    fn_80112B64(fn_80112A00(m240, 0.0f), m320, keep);
    m294 = 0.0f;
    float t = fn_8048BCA4(fn_8000DB1C(), m290, m294);
    m204 = fn_80112A00(m210, t);
    m228 = fn_80112A00(m240, t);
    m258 = roll;
    m234 = *GetTarget_unk();
    if (fn_8000DB1C()->fn_801130F4()) {
        fn_801162A4(0);
    }
}

} // namespace Event

namespace Event {

// NONMATCHING: 97.7% - register allocation of the Vector3 temporaries
void EventCamera_unk::fn_8011375C(const CameraTarget_unk& eye, int resetAngles, float a, float b, float c) {
    fn_801128C4(resetAngles);
    m1F4 = 2;
    m210 = eye;
    atn::Vector3 pos = fn_80112A00(m210, 0.0f);
    m204 = pos;
    if (a > 1.5533431f) {
        a = 1.5533431f;
    }
    m268 = atn::Vector3(a, b, 0.0f);
    m200 = c;
    if (lbl_8087F540->mC4) {
        fn_801162A4(0);
    }
}

// NONMATCHING: 99.8% - Vector3 temporary at another stack slot and src/dst copy registers swapped
void EventCamera_unk::fn_801139E4(const CameraTarget_unk& at, int resetAngles, float dist, float roll) {
    fn_801128C4(resetAngles);
    m1F4 = 3;
    m240 = at;
    m228 = fn_80112A00(m240, 0.0f);
    atn::Vector3 dir;
    dir = mPos - m228;
    dir.Normalize_unk();
    m204 = m228 + dir * dist;
    m258 = roll;
    if (lbl_8087F540->mC4) {
        fn_801162A4(0);
    }
}

// NONMATCHING: 99.8% - src/dst registers of Vector3 copies swapped (mwcc-blockers q3)
void EventCamera_unk::fn_80113CD4(const CameraTarget_unk& eye, const CameraTarget_unk& at, int id, int resetAngles,
                                  float roll) {
    fn_801128C4(resetAngles);
    m290 = fn_8048B048(lbl_8087F540, id);
    m1F4 = 4;
    m294 = 0.0f;
    float t = fn_8048BCA4(lbl_8087F540, m290, m294);
    m210 = eye;
    m204 = fn_80112A00(m210, t);
    m240 = at;
    m228 = fn_80112A00(m240, t);
    m258 = roll;
    if (lbl_8087F540->mC4) {
        fn_801162A4(0);
    }
}

// NONMATCHING: 99.6% - fmr instead of frsp for the float argument; src/dst copy registers swapped
void EventCamera_unk::fn_801140A0(Path* path, const CameraTarget_unk& at, int id, int resetAngles, float speed, float t,
                                  float roll) {
    fn_801128C4(resetAngles);
    m290 = fn_8048B048(lbl_8087F540, id);
    m1F4 = 5;
    m298 = speed;
    m294 = t;
    float time = fn_8048BCA4(lbl_8087F540, m290, t);
    m210.mPath = path;
    m204 = fn_80112A00(m210, time);
    m240 = at;
    m228 = fn_80112A00(m240, time);
    m258 = roll;
    if (lbl_8087F540->mC4) {
        fn_801162A4(0);
    }
}

void EventCamera_unk::fn_80114468() {
    m420 = 0;
    lbl_8087EFA8->mD4 = 0;
}

void EventCamera_unk::fn_8011447C(float a, float b, float c, float d, float e, float f, int frames, int arg) {
    m420 = 1;
    m424 = a;
    m428 = b;
    m42C = c;
    m430 = d;
    m434 = e;
    m438 = f;
    m43C = frames;
    m440 = arg;
}

// NONMATCHING: 89.6% - scheduling and register allocation of the six lerps
void EventCamera_unk::fn_801144A8() {
    if (lbl_8087EFA8 && m420) {
        float t;
        if (m43C > 0) {
            t = 1.0f / m43C--;
        } else {
            t = 1.0f;
        }
        EventGameState_unk* gs = lbl_8087EFA8;
        float d = t * (m430 - gs->mF0) + gs->mF0;
        float a = t * (m424 - gs->mE4) + gs->mE4;
        float b = t * (m428 - gs->mE8) + gs->mE8;
        float c = t * (m42C - gs->mEC) + gs->mEC;
        float f = t * (m434 - gs->m100) + gs->m100;
        float e = t * (m438 - gs->mFC) + gs->mFC;
        float farClip = mFar;
        float nearClip = mNear;
        if (farClip <= d) {
            d = farClip;
        }
        if (nearClip >= d) {
            d = nearClip;
        }
        if (farClip <= c) {
            c = farClip;
        }
        if (nearClip >= c) {
            c = nearClip;
        }
        if (farClip <= a) {
            a = farClip;
        }
        if (nearClip >= a) {
            a = nearClip;
        }
        b = farClip <= b ? farClip : b;
        b = nearClip >= b ? nearClip : b;
        gs->mD4 = 1;
        gs->mE4 = a;
        gs->mE8 = b;
        gs->mEC = c;
        gs->mF0 = d;
        gs->m100 = f;
        gs->mFC = e;
        gs->mE0 = m440;
    }
}

void EventCamera_unk::fn_8011462C(int on, const atn::Vector3& amp, const atn::Vector3& speed, float a, float b) {
    if (on) {
        m3FC = 0.0f;
        m3F0 = lbl_807C7030;
        m3B8 = amp;
        m3D8 = amp;
        m3D0 = a;
        m3C4 = speed;
        m3E4 = speed;
        m3D4 = b;
        return;
    }
    m3FC = -1.0f;
}

} // namespace Event

namespace Event {

// NONMATCHING: 81.7% - one more saved float register (f26) than the original; scheduling of the matrix rows
void EventCamera_unk::fn_801146A4() {
    if (-1.0f == m3FC) {
        return;
    }
    atn::Vector3 x;
    atn::Vector3 y;
    atn::Vector3 z;
    atn::Vector3 shake;
    atn::Vector3 pos = mPos;
    atn::Vector3 target = mTarget;
    float* amp = &m3B8.x;
    float* curAmp = &m3D8.x;
    float* speed = &m3C4.x;
    float* curSpeed = &m3E4.x;
    float* out = &shake.x;
    for (int i = 0; i < 3; i++) {
        float range = m3D0 * (0.5f * *amp);
        float c = *curAmp += (range - -range) * (rand() / 32767.0f) + -range;
        float v = *amp <= c ? *amp : c;
        *curAmp = 0.0f >= v ? 0.0f : v;
        range = m3D4 * (0.5f * *speed);
        c = *curSpeed += (range - -range) * (rand() / 32767.0f) + -range;
        v = *speed <= c ? *speed : c;
        *curSpeed = 0.0f >= v ? 0.0f : v;
        float s = *curSpeed;
        float a = *curAmp;
        *out = a * (float)sin(m3FC * s * lbl_8087F540->mB4);
        amp++;
        curAmp++;
        speed++;
        curSpeed++;
        out++;
    }
    x = mView.GetRow_unk(0) * shake.x;
    y = mView.GetRow_unk(1) * shake.y;
    z = mView.GetRow_unk(2) * shake.z;
    shake = x + y + z;
    mPos = pos + shake;
    mTarget = target + shake;
    m3FC += 1.0;
}

// NONMATCHING: 87.3% - register allocation (r30/r31) and scheduling of the animation curves
void EventCamera_unk::fn_80114AB4() {
    if (m400 == 0 || 0.0f == lbl_8087F540->mB4) {
        return;
    }
    float scalePos = m40C;
    float scaleRot = m410;
    if (m414 > 0.0f) {
        float r = m418 / m414;
        scalePos *= r;
        scaleRot *= r;
    } else if (m414 < 0.0f) {
        float r = 1.0f - m418 / -m414;
        scalePos *= r;
        scaleRot *= r;
    }
    m418 = m404 * lbl_8087F540->mB4 + m418;
    if (m418 > atn::Abs_unk(m414)) {
        m414 = 0.0f;
    }
    atn::Vector3 pos = mPos;
    atn::Vector3 target = mTarget;
    XsfCurveSet_unk* curves = m400->fn_805BDCBC(0);
    if (m408 >= curves->m14) {
        m408 -= curves->m14;
    }
    if (m41C) {
        atn::Vector3 rot;
        curves->fn_805BF410(m408, &rot.x, 4, 5, 6);
        rot.z = atn::DegToRad_unk(rot.z) * scaleRot;
        rot.x = atn::DegToRad_unk(rot.x) * scaleRot;
        rot.y = atn::DegToRad_unk(rot.y) * scaleRot;
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        atn::Vector3 r = rot;
        atn::Matrix3x4 m;
        m.SetIdentity();
        __MtxRotate(m.m, 'z', rot.z);
        __MtxRotate(m.m, 'y', r.y);
        __MtxRotate(m.m, 'x', r.x);
        PSMTXMultVec(m.m, &dir, &dir);
        target = pos + dir;
        atn::Vector3 p;
        curves->fn_805BF410(m408, &p.x, 1, 2, 3);
        pos = p * scalePos;
    } else {
        atn::Vector3 rot;
        curves->fn_805BF410(m408, &rot.x, 4, 5, 6);
        rot.z = atn::DegToRad_unk(rot.z) * scaleRot;
        rot.x = atn::DegToRad_unk(rot.x) * scaleRot;
        rot.y = atn::DegToRad_unk(rot.y) * scaleRot;
        atn::Vector3 dir = target - pos;
        atn::Vector3 r = rot;
        atn::Matrix3x4 m;
        m.SetIdentity();
        __MtxRotate(m.m, 'z', rot.z);
        __MtxRotate(m.m, 'y', r.y);
        __MtxRotate(m.m, 'x', r.x);
        PSMTXMultVec(m.m, &dir, &dir);
        target = pos + dir;
        atn::Vector3 prev;
        curves->fn_805BF410(m408 >= 1.0f ? m408 - 1.0f : 0.0f, &prev.x, 1, 2, 3);
        atn::Vector3 cur;
        curves->fn_805BF410(m408, &cur.x, 1, 2, 3);
        pos += (cur - prev) * scalePos;
    }
    mPos = pos;
    mTarget = target;
    m408 = m404 * lbl_8087F540->mB4 + m408;
}

} // namespace Event

namespace Event {

// NONMATCHING: 99.0% - the original does not fuse fmuls+fadds in the two angle updates
void EventCamera_unk::fn_801151A4() {
    float stick = -fn_800A56A8(GetPad_unk(), 0, 1, 0);
    if (fn_800A58D0(GetPad_unk(), 0)) {
        stick += -fn_800A56A8(GetPad_unk(), 0, 3, 0);
    }
    if (GetSetConfig_unk() && GetCameraConfig_unk(GetSetConfig_unk())->IsInvertX_unk()) {
        stick *= -1.0f;
    }
    float v = MIN(1.0f, MAX(-1.0f, stick));
    if (atn::Abs_unk(v) > 0.2f) {
        float sign = v > 0.0f ? 1.0f : -1.0f;
        float s = (atn::Abs_unk(v) - 0.2f) / 0.8f;
        if (GetSetConfig_unk()) {
            s *= GetCameraConfig_unk(GetSetConfig_unk())->GetRotScale_unk();
        }
        m25C.x += s * (sign * m200);
        m25C.x = atn::WrapAngle_unk(m25C.x);
        if (atn::Abs_unk(m25C.x) > m268.x) {
            m25C.x = m268.x * (m25C.x > 0.0f ? 1.0f : -1.0f);
        }
    }
    stick = -fn_800A56A8(GetPad_unk(), 0, 0, 0);
    if (fn_800A58D0(GetPad_unk(), 0)) {
        stick += -fn_800A56A8(GetPad_unk(), 0, 2, 0);
    }
    if (GetSetConfig_unk() && GetCameraConfig_unk(GetSetConfig_unk())->IsInvertY_unk()) {
        stick *= -1.0f;
    }
    v = MIN(1.0f, MAX(-1.0f, stick));
    if (atn::Abs_unk(v) > 0.2f) {
        float sign = v > 0.0f ? 1.0f : -1.0f;
        float s = (atn::Abs_unk(v) - 0.2f) / 0.8f;
        if (GetSetConfig_unk()) {
            s *= GetCameraConfig_unk(GetSetConfig_unk())->GetRotScale_unk();
        }
        m25C.y += s * (sign * m200);
        m25C.y = atn::WrapAngle_unk(m25C.y);
        if (atn::Abs_unk(m25C.y) > m268.y) {
            m25C.y = m268.y * (m25C.y > 0.0f ? 1.0f : -1.0f);
        }
    }
    atn::Vector3 dir = m274;
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    atn::Vector3 side;
    PSVECCrossProduct(&up, &dir, &side);
    float wobble = sin(3.0f * atn::DegToRad_unk(lbl_8087F540->m1EAC % 360 - 180));
    atn::Quaternion_unk qx;
    fn_805F9AB0(&qx, &side, 0.01f * wobble + m25C.x);
    atn::Quaternion_unk qy;
    fn_805F9AB0(&qy, &up, m25C.y);
    atn::Quaternion_unk q;
    PSQUATMultiply(&qy, &qx, &q);
    atn::Matrix3x4 m;
    PSMTXQuat(m.m, &q);
    m.m[0][3] = lbl_807C7030.x;
    m.m[1][3] = lbl_807C7030.y;
    m.m[2][3] = lbl_807C7030.z;
    atn::Vector3 v2 = m274;
    PSMTXMultVec(m.m, &v2, &v2);
    PSVECNormalize(&v2, &v2);
    v2 *= 50.0f;
    v2 += m204;
    m228 = v2;
}

// NONMATCHING: 99.8% - register of the second fabs
void EventCamera_unk::fn_80115704() {
    float stick = -fn_800A56A8(lbl_8087EF70, 0, 0, 0);
    if (atn::Abs_unk(stick) > 0.2f) {
        float sign = stick > 0.0f ? 1.0f : -1.0f;
        float s = (atn::Abs_unk(stick) - 0.2f) / 0.8f;
        s *= m298 * (s * sign);
        m294 += s;
    }
    float t = m294;
    float hi = 1.0f;
    t = hi <= t ? hi : t;
    float lo = 0.0f;
    m294 = lo >= t ? lo : t;
}

} // namespace Event

namespace Event {

inline void EventCamera_unk::MoveTarget_unk(const atn::Vector3& goal) {
    if (mBits.mFlag2 == 1) {
        atn::Vector3 cur = mTarget;
        atn::Vector3 g = goal;
        atn::Vector3 d = g - cur;
        float len = PSVECMag(&d);
        PSVECNormalize(&d, &d);
        d *= len * m3B0;
        mTarget = cur + d;
    } else {
        mTarget = goal;
    }
}

// NONMATCHING: 84.5% - stack layout of the Vector3 temporaries (0x48 bytes higher in the original)
void EventCamera_unk::fn_801157DC() {
    if (lbl_8087F540->mB4 <= 0.0f) {
        return;
    }
    if (mBits.mFlag0) {
        MoveTarget_unk(m228);
        mPos = m204;
        m25C.z = m258;
        m288 = m282;
        return;
    }
    float total = m282;
    float left = total - m288;
    if (left <= 0.0f) {
        MoveTarget_unk(m228);
        mPos = m204;
        m25C.z = m258;
    } else {
        float posDiv = total - fn_8048BD04(lbl_8087F540, m28C, 1.0f - left / total) * total;
        float tgtDiv = total - fn_8048BCA4(lbl_8087F540, m290, 1.0f - left / total) * total;
        posDiv /= lbl_8087F540->mB4;
        tgtDiv /= lbl_8087F540->mB4;
        if (posDiv <= 1.0f) {
            posDiv = 0.0f;
        }
        if (tgtDiv <= 1.0f) {
            tgtDiv = 0.0f;
        }
        atn::Vector3 target;
        if (posDiv <= 0.0f) {
            target = m228;
        } else {
            target = mTarget + (m228 - mTarget) * (1.0f / posDiv);
        }
        MoveTarget_unk(target);
        atn::Vector3 pos;
        if (m280 == 3) {
            float d0 = sqrt((mPos.x - mTarget.x) * (mPos.x - mTarget.x) + (mPos.z - mTarget.z) * (mPos.z - mTarget.z));
            float d1 = sqrt((m204.x - m228.x) * (m204.x - m228.x) + (m204.z - m228.z) * (m204.z - m228.z));
            if (!(posDiv <= 0.0f)) {
                d1 = d0 + (d1 - d0) / posDiv;
            }
            atn::Vector3 a(mPos.x - mTarget.x, 0.0f, mPos.z - mTarget.z);
            PSVECNormalize(&a, &a);
            atn::Vector3 b(m204.x - m228.x, 0.0f, m204.z - m228.z);
            PSVECNormalize(&b, &b);
            float angA = atan2(a.x, a.z);
            float angB = atan2(b.x, b.z);
            if (!(posDiv <= 0.0f)) {
                angB = angA + (angB - angA) / posDiv;
            }
            float y;
            if (posDiv <= 0.0f) {
                y = m204.y;
            } else {
                y = mPos.y + (m204.y - mPos.y) / posDiv;
            }
            pos.x = cos(angB);
            pos.y = 0.0f;
            pos.z = sin(angB);
            pos.x = pos.x * d1 + target.x;
            pos.y = y;
            pos.z = pos.z * d1 + target.z;
        } else if (m280 == 2) {
            float d0 = sqrt((mPos.x - mTarget.x) * (mPos.x - mTarget.x) + (mPos.z - mTarget.z) * (mPos.z - mTarget.z));
            float d1 = sqrt((m204.x - m228.x) * (m204.x - m228.x) + (m204.z - m228.z) * (m204.z - m228.z));
            if (!(posDiv <= 0.0f)) {
                d1 = d0 + (d1 - d0) / posDiv;
            }
            float y;
            if (posDiv <= 0.0f) {
                y = m204.y;
            } else {
                y = mPos.y + (m204.y - mPos.y) / posDiv;
            }
            atn::Vector3 a = mPos - mTarget;
            PSVECNormalize(&a, &a);
            atn::Vector3 b = m204 - m228;
            PSVECNormalize(&b, &b);
            atn::Vector3 pa = mTarget + a;
            atn::Vector3 pb = m228 + b;
            atn::Vector3 p;
            if (posDiv <= 0.0f) {
                p = pb;
            } else {
                p = pa + (pb - pa) * (1.0f / posDiv);
            }
            pos = p;
            pos = pos - target;
            pos.y = 0.0f;
            PSVECNormalize(&pos, &pos);
            pos.x = pos.x * d1 + target.x;
            pos.y = y;
            pos.z = pos.z * d1 + target.z;
        } else {
            atn::Vector3 p;
            if (posDiv <= 0.0f) {
                p = m204;
            } else {
                p = mPos + (m204 - mPos) * (1.0f / posDiv);
            }
            pos = p;
        }
        mPos = pos;
        if (posDiv <= 0.0f) {
            m25C.z = m258;
        } else {
            m25C.z = m25C.z + (m258 - m25C.z) / posDiv;
        }
        m288 += lbl_8087F540->mB4;
    }
    atn::Vector3 dir = mTarget - mPos;
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
        Mtx m;
        PSMTXRotAxisRad(m, &dir, m25C.z);
        PSMTXMultVec(m, &up, &up);
    }
    mUp = up;
}

// NONMATCHING: 98.5% - one float temporary at 0x14 instead of 0x18; the original calls ToRotation_unk/Length_unk/WrapAngle_unk without emitting their weak copies here (poolcheck: +-pi/2 floats)
void EventCamera_unk::fn_801162A4(int arg) {
    int padControl = 0;
    if (fn_8000DB1C()->fn_80112F54() && fn_8000DB1C()->fn_80112F54()->IsPadCamera_unk()) {
        padControl = 1;
    }
    switch (m1F4) {
    case 0:
        return;
    case 1: {
        float t = fn_8048BCA4(fn_8000DB1C(), m290, m294);
        m204 = fn_80112A00(m210, t);
        m228 = fn_80112A00(m240, t);
        fn_801157DC();
        break;
    }
    case 3:
        m228 = fn_80112A00(m240, 0.0f);
        fn_801157DC();
        break;
    case 2:
        if (0.0f == m274.LengthSq_unk()) {
            m274.z = 1.0f;
        }
    case 6:
        m204 = fn_80112A00(m210, 0.0f);
        fn_801151A4();
        fn_801157DC();
        padControl = 0;
        break;
    case 5:
        fn_80115704();
        padControl = 0;
    case 4: {
        float t = fn_8048BCA4(fn_8000DB1C(), m290, m294);
        m204 = fn_80112A00(m210, t);
        m228 = fn_80112A00(m240, t);
        fn_801157DC();
        break;
    }
    }
    fn_80114AB4();
    fn_801146A4();
    fn_801144A8();
    atn::Vector3 savedTarget(*GetTarget_unk());
    float savedFovy = GetFovy_unk();
    float sx = -fn_800A56A8(GetPad_unk(), 0, 0, 0);
    if (fn_800A58D0(GetPad_unk(), 0)) {
        sx += -fn_800A56A8(GetPad_unk(), 0, 2, 0);
    }
    if (GetSetConfig_unk() && GetCameraConfig_unk(GetSetConfig_unk())->IsInvertY_unk()) {
        sx *= -1.0f;
    }
    sx = MIN(1.0f, MAX(-1.0f, sx));
    float sign;
    if (fn_80011220(sx) > 0.2f) {
        fn_80112220(sx, sx, sign);
        sx = (sx - 0.2f) / 0.8f;
        if (GetSetConfig_unk()) {
            sx *= GetCameraConfig_unk(GetSetConfig_unk())->GetRotScale_unk();
        }
        sx *= sign;
    }
    float sy = -fn_800A56A8(GetPad_unk(), 0, 1, 0);
    if (fn_800A58D0(GetPad_unk(), 0)) {
        sy += -fn_800A56A8(GetPad_unk(), 0, 3, 0);
    }
    if (GetSetConfig_unk() && GetCameraConfig_unk(GetSetConfig_unk())->IsInvertX_unk()) {
        sy *= -1.0f;
    }
    sy = MIN(1.0f, MAX(-1.0f, sy));
    if (fn_80011220(sy) > 0.2f) {
        fn_80112220(sy, sy, sign);
        sy = (sy - 0.2f) / 0.8f;
        if (GetSetConfig_unk()) {
            sy *= GetCameraConfig_unk(GetSetConfig_unk())->GetRotScale_unk();
        }
        sy *= sign;
    }
    float zoom = 0.0f;
    if (fn_800F52F8()->fn_80116BAC()->fn_801240B4(7)) {
        zoom = 1.0f;
    }
    m644 = 0.075f * (sx - m644) + m644;
    m64C = 0.075f * (zoom - m64C) + m64C;
    m648 = 0.075f * (sy - m648) + m648;
    if (padControl) {
        atn::Vector2_unk ang;
        float k = MAX(0.0f, MIN(1.0f, (GetFovy_unk() - atn::DegToRad_unk(15.0f)) / atn::DegToRad_unk(25.0f)));
        float scale = fn_800F8524(&sZoomMin, &sZoomMax, k);
        ang.x = scale * (m648 * atn::DegToRad_unk(8.0f));
        ang.y = scale * (m644 * atn::DegToRad_unk(8.0f));
        atn::Vector3 pos(*GetPosition_unk());
        atn::Vector3 target(*GetTarget_unk());
        atn::Vector3 d = target - pos;
        float len = d.Length_unk();
        atn::Vector3 rot = d.ToRotation_unk();
        rot.x = MAX(-1.5550884f, MIN(1.5550884f, atn::WrapAngle_unk(rot.x + ang.x)));
        rot.y = atn::WrapAngle_unk(rot.y + ang.y);
        atn::Matrix3x4 m = fn_800109E0(rot);
        target = pos + atn::Vector3(0.0f, 0.0f, len).Transform_unk(m);
        SetTarget_unk(target);
        SetFovy_unk(GetFovy_unk() * (1.0f - 0.5f * m64C));
    }
    if (m638 < m63C && m63C > 0.0f) {
        float t = 1.0f;
        m638 += t;
        t = MIN(t, m638 / m63C);
        SetPosition(atn::Lerp_unk(*m444.GetPosition_unk(), *GetPosition_unk(), t));
        SetTarget_unk(atn::Lerp_unk(*m444.GetTarget_unk(), *GetTarget_unk(), t));
        float fovy = GetFovy_unk();
        float fovy2 = m444.GetFovy_unk();
        SetFovy_unk(fn_800F8524(&fovy2, &fovy, t));
    }
    fn_8004B378();
    if (arg && !IsFlag1_unk()) {
        *fn_8008B964() = *this;
        if (GetSoundMgr_unk()) {
            atn::Vector3 rot(*GetRot_unk());
            rot.y = atn::WrapAngle_unk(3.1415927f + rot.y);
            atn::Vector3 pos(*GetPosition_unk());
            GetSoundMgr_unk()->SetListener_unk(pos, rot);
        }
    }
    SetTarget_unk(savedTarget);
    SetFovy_unk(savedFovy);
}

int EventCamera_unk::fn_80116E7C() {
    int result = 0;
    if (m1F4 != 0 && mBits.mFlag1 != 1) {
        result = 1;
    }
    return result;
}

} // namespace Event
