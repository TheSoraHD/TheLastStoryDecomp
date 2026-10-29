#include "gmk/Bat.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: debug settings
struct BatDebug_unk {
    unsigned char m00[0x18];
    int m18; // inferred: draw
};
extern "C" BatDebug_unk* lbl_8087F0A8;
// inferred: unknown manager; fn_80370A78(mgr, 0) == 1 starts the bats (class unknown)
extern "C" void* lbl_8087F430;
extern "C" int fn_80370A78(void* mgr, int arg);

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))

// inferred: rotation matrix z * y * x (same as GimmickCommon.cpp)
inline void __MakeRotation_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}
inline void __SetTransV_unk(atn::Matrix3x4& m, const atn::Vector3& v) {
    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;
}
inline void __Vec2Set_unk(atn::Vector2_unk& v, float x, float y) {
    v.x = x;
    v.y = y;
}
inline void __Vec3Set_unk(atn::Vector3& v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}
// inferred: random value in [0, 1] (name invented)
inline float Rand_unk() {
    return rand() % 1001 / 1000.0f;
}
inline atn::Vector3 GetCol_unk(const atn::Matrix3x4& m, int i) {
    return atn::Vector3(m.m[0][i], m.m[1][i], m.m[2][i]);
}
// inferred: largest scale axis of a matrix (name invented)
inline float MaxScale_unk(const atn::Matrix3x4& mtx) {
    atn::Vector3 s;
    atn::Vector3 az = GetCol_unk(mtx, 2);
    float sz = PSVECMag(&az);
    atn::Vector3 ay = GetCol_unk(mtx, 1);
    float sy = PSVECMag(&ay);
    atn::Vector3 ax = GetCol_unk(mtx, 0);
    float sx = PSVECMag(&ax);
    __Vec3Set_unk(s, sx, sy, sz);
    return __MAX_unk(s.x, __MAX_unk(s.y, s.z));
}
// inferred: squared distance (name invented)
inline float DistSq_unk(const atn::Vector3& p, atn::Vector3 q) {
    atn::Vector3 d = p - q;
    return fn_805F9920(&d);
}
inline void RandScale_unk(float& v) {
    v *= 0.6f + 0.8f * Rand_unk();
}
inline float RandMax_unk(float max) {
    return max * Rand_unk();
}
inline float RandRange_unk(float a, float b) {
    return a + (b - a) * Rand_unk();
}
inline void SetRand_unk(float& v, float max) {
    v = max * Rand_unk();
}
inline void CopyStr_unk(char* dst, const char* src) {
    memcpy(dst, src, strlen(src) + 1);
}
inline void PlayMotion_unk(atn::Model* model, int motion) {
    model->fn_80097C08(0, motion, 1, 0, 1, 1.0f, 0.1f);
}

namespace gmk {

inline Bat::Bat(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDelay(10), mTimer(0), mDesc(desc) {
    m6C = desc->mPos;
    m78.y = desc->mRotY;
}

GimmickCommon* Bat::fn_80402978(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) Bat(parent, desc);
    }
    return 0;
}

Bat::~Bat() {}

inline bool Bat::IsLoading_unk() {
    if (fn_803EC91C() != 0) {
        return true;
    }
    for (int i = 0; i < mNum; i++) {
        if (mBats.mData[(unsigned int)i].mModel.fn_8008B140() != 0) {
            return true;
        }
    }
    return false;
}

int Bat::vf0C() {
    if (!IsLoading_unk()) {
        fn_80403308();
        return 1;
    }
    return 0;
}

inline void Bat::UpdateModel_unk(int i) {
    atn::Matrix3x4 mtx = lbl_807C7060;
    atn::Matrix3x4 rot;
    __MakeRotation_unk(rot, m78 + mBats.mData[i].mRot);
    PSMTXConcat(rot.m, mtx.m, mtx.m);
    BatUnit_unk& b = mBats.mData[i];
    __SetTransV_unk(mtx, m6C + b.mPos);
    __MtxCopy(b.mModel.mMtx, mtx.m);
    b.mModel.m54 = MaxScale_unk(mtx);
    mBats.mData[i].mModel.fn_80097E80(1);
    mBats.mData[i].mModel.CalcMatrixStack(atn::ModelNodeCallback_unk());
}

// inferred: per frame update of the model of a bat (name invented)
inline void Bat::Update_unk(int i) {
    mBats.mData[i].mModel.fn_80092A4C();
    if (mBats.mData[i].mStateNo != 1) {
        UpdateModel_unk(i);
    }
}

// NONMATCHING: 99.77% - the two position locals of the nearest search sit below their copies (above
// in the original). The unsigned subscripts of the loops over mBats give the index/offset order
void Bat::vf10() {
    for (int i = 0; i < mNum; i++) {
        (this->*mBats.mData[(unsigned int)i].mState)(i);
    }
    if (mStarted == 1) {
        if (mTimer > 0 && --mTimer > 0) {
            return;
        }
        atn::Vector3 cam = lbl_8087F8A0->mHead->mPos;
        int i;
        int best = -1;
        for (i = 0; i < mNum; i++) {
            if (mBats.mData[i].mStateNo == 1) {
                if (best == -1) {
                    best = i;
                } else {
                    atn::Vector3 a = m6C + mBats.mData[best].mPos;
                    atn::Vector3 b = m6C + mBats.mData[i].mPos;
                    if (DistSq_unk(cam, a) > DistSq_unk(cam, b)) {
                        best = i;
                    }
                }
            }
        }
        if (best != -1) {
            mBats.mData[best].mStart = mBats.mData[best].mPos;
            mBats.mData[best].mT = 0.0f;
            mBats.mData[best].mStateNo = 2;
            mBats.mData[best].mState = &Bat::fn_80403BFC;
            PlayMotion_unk(&mBats.mData[best].mModel, 1);
        }
    }
    if (fn_80370A78(lbl_8087F430, 0) == 1 && mStarted == 0) {
        vf40(0);
    }
    for (int i = 0; i < mNum; i++) {
        Update_unk(i);
    }
}

void Bat::vf14() {
    for (int i = 0; i < mNum; i++) {
        BatUnit_unk* bat = &mBats.mData[(unsigned int)i];
        if (bat->mStateNo != 4 && lbl_8087F0A8->m18 != 0) {

            fn_803ED774(&bat->mModel);
        }
    }
}

// NONMATCHING: 99.97% - .sdata2 order: 1000 before 40 (the original has 40 first)
void Bat::fn_80403308() {
    mStarted = 0;
    mTimer = mDelay;
    for (int i = 0; i < mNum; i++) {
        mBats.mData[i].mState = &Bat::fn_80403BF8;
        mBats.mData[i].mModel.m34C = 1;
        mBats.mData[i].mModel.mTracks[0].m20 = 1.0f;
        mBats.mData[i].mModel.mTracks[0].m0C = 1.2f;
        PlayMotion_unk(&mBats.mData[i].mModel, 0);
        mBats.mData[i].mModel.mTracks[0].m08 = mBats.mData[i].mModel.fn_80097D7C(0);
        mBats.mData[i].mPos.Set_unk(40.0f * Rand_unk() - 20.0f, 0.0f, 40.0f * Rand_unk() - 20.0f);
        mBats.mData[i].mRot.Set_unk(0.0f, 0.0f, 0.0f);
        mBats.mData[i].mStateNo = 1;
        mBats.mData[i].mModel.fn_80092A4C();
        UpdateModel_unk(i);
    }
}

void Bat::vf20() {
    int i;
    char model[0x100];
    char path[0x100];
    char name[0x100];

    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            CopyStr_unk(model, csv.fn_8005B9CC());
        }
        if (strcmp(field, "MOTION") == 0) {
            CopyStr_unk(path, csv.fn_8005B9CC());
            CopyStr_unk(name, csv.fn_8005B9CC());
        }
        if (strcmp(field, "NUM") == 0) {
            mNum = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
    mBats.Resize_unk(mNum, 0);
    for (i = 0; i < mNum; i++) {
        fn_8008AD4C(&mBats.mData[(unsigned int)i].mModel, model, 0);
        fn_80097A88(&mBats.mData[(unsigned int)i].mModel, 0, path);
        fn_80097A88(&mBats.mData[(unsigned int)i].mModel, 1, name);
    }
}

int Bat::vf40(GimmickMsg_unk* msg) {
    for (int i = 0; i < mNum; i++) {
        fn_80404290(i);
    }
    mStarted = 1;
    return 0;
}

void Bat::fn_80403BF4(int index) {}

void Bat::fn_80403BF8(int index) {}

// inferred: moves a bat along its quadratic curve mStart -> mStart + mEnd (control point
// mStart + mCtrl); returns the curve parameter (1 at the end)
inline float Bat::Bezier_unk(int i) {
    atn::Vector3 p0 = mBats.mData[i].mStart;
    atn::Vector3 p1 = mBats.mData[i].mStart + mBats.mData[i].mCtrl;
    atn::Vector3 p2 = mBats.mData[i].mStart + mBats.mData[i].mEnd;
    mBats.mData[i].mT += mBats.mData[i].mStep;
    float t = mBats.mData[i].mT;
    mBats.mData[i].mPos = p0 * ((1.0f - t) * (1.0f - t)) + p1 * (2.0f * t * (1.0f - t)) + p2 * (t * t);
    return t;
}

inline float Bat::Move_unk(int i) {
    mBats.mData[i].mStep = 2.5f / PSVECMag(&mBats.mData[i].mEnd);
    return Bezier_unk(i);
}

// NONMATCHING: 93.98% - float registers of the inlined curve (one more saved register, f27)
void Bat::fn_80403BFC(int i) {
    if (Move_unk(i) >= 1.0f) {
        mBats.mData[i].mStart = mBats.mData[i].mPos;
        mBats.mData[i].mCtrl = mBats.mData[i].mEnd - mBats.mData[i].mCtrl;
        RandScale_unk(mBats.mData[i].mCtrl.x);
        RandScale_unk(mBats.mData[i].mCtrl.z);
        PSVECNormalize(&mBats.mData[i].mCtrl, &mBats.mData[i].mCtrl);
        mBats.mData[i].mCtrl *= 80.0f;
        mBats.mData[i].mCtrl.y += 50.0f;
        mBats.mData[i].mEnd = mBats.mData[i].mCtrl * 2.0f;
        mBats.mData[i].mEnd.y += 30.0f;
        mBats.mData[i].mT = 0.0f;
        mBats.mData[i].mStateNo = 3;
        mBats.mData[i].mState = &Bat::fn_80404064;
        mBats.mData[i].mStep = 2.5f / PSVECMag(&mBats.mData[i].mEnd);
        PlayMotion_unk(&mBats.mData[i].mModel, 1);
    }
}

// NONMATCHING: 88.68% - float registers of the inlined curve (one more saved register, f27)
void Bat::fn_80404064(int i) {
    if (Move_unk(i) >= 1.0f) {
        mBats.mData[i].mStateNo = 4;
        mBats.mData[i].mState = &Bat::fn_80403BF4;
    }
}

// inferred: random turn and height of the flight (name invented)
inline void Bat::InitFly_unk(int i) {
    mBats.mData[i].mAngle = atn::DegToRad_unk(20.0f * Rand_unk() - 10.0f);
    SetRand_unk(mBats.mData[i].mHeight, 10.0f);
}

// inferred: direction of the gimmick on the ground (x = sin, y = cos of the yaw; name invented)
inline atn::Vector2_unk Bat::Dir_unk() {
    return atn::Vector2_unk(sin(m78.y), cos(m78.y));
}

// inferred: control point of the first curve, between the bat and the player (name invented)
inline void Bat::SetCtrl_unk(int i, const atn::Vector3& cam, float h, atn::Vector2_unk& dir) {
    atn::Vector3 pos = m6C + mBats.mData[i].mPos;
    mBats.mData[i].mCtrl = cam - pos;
    mBats.mData[i].mCtrl *= 0.4f;
    mBats.mData[i].mCtrl.y = cam.y - m6C.y;
    mBats.mData[i].mCtrl.x += 3.0f * mBats.mData[i].mAngle;
    mBats.mData[i].mCtrl.y += h / 2.5f + mBats.mData[i].mHeight;
    mBats.mData[i].mCtrl.z += 3.0f * mBats.mData[i].mAngle;
    __Vec2Set_unk(dir, sin(m78.y), cos(m78.y));
    float len = PSVECMag(&mBats.mData[i].mCtrl);
    float dx = dir.x * len;
    float dz = dir.y * len;
    mBats.mData[i].mCtrl.x = 0.5f * mBats.mData[i].mCtrl.x + 0.5f * dx;
    mBats.mData[i].mCtrl.z = 0.5f * mBats.mData[i].mCtrl.z + 0.5f * dz;
}

// inferred: end of the first curve, above the player (name invented)
inline void Bat::SetEnd_unk(int i, atn::Vector3& cam, float h, atn::Vector2_unk& v) {
    cam.y += h + mBats.mData[i].mHeight;
    atn::Vector3 pos = m6C + mBats.mData[i].mStart;
    mBats.mData[i].mEnd = cam - pos;
    float s = sin(mBats.mData[i].mAngle);
    BatUnit_unk& b = mBats.mData[i];
    float c = cos(b.mAngle);
    mBats.mData[i].mEnd.x = c * mBats.mData[i].mEnd.x - b.mEnd.z * s;
    s = sin(mBats.mData[i].mAngle);
    BatUnit_unk& b2 = mBats.mData[i];
    c = cos(b2.mAngle);
    mBats.mData[i].mEnd.z = c * mBats.mData[i].mEnd.z + b2.mEnd.x * s;

    __Vec2Set_unk(v, mBats.mData[i].mEnd.x, mBats.mData[i].mEnd.z);
    if (v.x * v.x + v.y * v.y < 0.1f) {
        mBats.mData[i].mEnd.x *= 10.0f;
        mBats.mData[i].mEnd.z *= 10.0f;
    }
    float inv = 1.0f / (float)sqrt(v.x * v.x + v.y * v.y);
    v.x *= inv;
    v.y *= inv;
    mBats.mData[i].mEnd.x += 10.0f * v.x;
    mBats.mData[i].mEnd.z += 10.0f * v.y;
    atn::Vector2_unk dir = Dir_unk();
    if (v.x * dir.x + v.y * dir.y < 0.0f) {
        mBats.mData[i].mEnd.x *= -1.0f;
        mBats.mData[i].mEnd.z *= -1.0f;
    }
}

// NONMATCHING: 88.47% - registers of the control point blend and of the rotation of the end (the
// original addresses mCtrl through base + offset, not indexed), float register order
void Bat::fn_80404290(int i) {
    // inferred: empty statements (as empty asserts) so that the function is not inlined in vf40,
    // which calls it out of line
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    mBats.mData[i].mStart = mBats.mData[i].mPos;
    InitFly_unk(i);
    CharaTask* player = lbl_8087F8A0->mHead;
    atn::Vector3 cam = player->mPos;
    float h = 25.0f + player->mPos.y;
    atn::Vector2_unk v;
    SetCtrl_unk(i, cam, h, v);
    cam = player->mPos;
    SetEnd_unk(i, cam, h, v);
}

} // namespace gmk
