#include "gmk/FlyObjManager.h"
#include "game/Inventory.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int rand();
extern "C" int swprintf(wchar_t* buf, unsigned long size, const wchar_t* format, ...);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
// inferred: debug settings
struct FlyDebug_unk {
    unsigned char m00[0x18];
    int m18; // inferred: draw
};
extern "C" FlyDebug_unk* lbl_8087F0A8;
// inferred: item manager and popup text (classes unknown)
extern "C" void fn_80449A18(void* mgr, int arg1, int arg2, int arg3, int arg4);
extern "C" void fn_804491F4(int* a, int* b, int arg2, int arg3, int arg4, float rate);
extern "C" void* lbl_8087F8A8;
extern "C" void fn_8054D794(void* obj, int arg1, const wchar_t* text, const atn::Vector3* pos, int arg4, int arg5,
                            int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12);

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
inline void __SetTrans_unk(atn::Matrix3x4& m, float x, float y, float z) {
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
}
inline void __Vec3Set_unk(atn::Vector3& v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}
// inferred: random value in [0, 1] and interpolation (names invented)
inline float Rand_unk() {
    return rand() % 1001 / 1000.0f;
}
inline float RandRange_unk(float a, float b) {
    return a + (b - a) * Rand_unk();
}
inline bool IsLoading_unk(atn::Model* model) {
    return model->fn_8008B140() != 0;
}
inline void PlayMotion_unk(atn::Model* model, int motion, float blend) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->fn_80097C08(0, motion, 1, 1, 1, blend, 0.1f);
    model->mTracks[0].m0C = 1.0f;
}

// inferred: largest scale axis of a matrix (name invented)
inline float MaxScale_unk(const atn::Matrix3x4& mtx) {
    atn::Vector3 s;
    atn::Vector3 az;
    atn::Vector3 ay;
    atn::Vector3 ax;
    __Vec3Set_unk(az, mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
    float sz = PSVECMag(&az);
    __Vec3Set_unk(ay, mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
    float sy = PSVECMag(&ay);
    __Vec3Set_unk(ax, mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
    float sx = PSVECMag(&ax);
    __Vec3Set_unk(s, sx, sy, sz);
    return __MAX_unk(s.x, __MAX_unk(s.y, s.z));
}

namespace gmk {

inline void FlyObj::Start_unk(const atn::Vector3& pos, const atn::Vector3& vel) {
    mState = 1;
    mPos = pos;
    mVel = vel;
    mColli.mFlags = (mColli.mFlags & ~1) | 8;
    mColli.m0C = (int)mOwner;
}

inline int FlyObjManager::FindNearest_unk(const atn::Vector3& pos) {
    float best = 1e16f;
    int index = -1;
    for (int i = 0; i < (int)mCount; i++) {
        atn::Vector3 d = pos - mObjs.mData[(unsigned int)i].mPos;
        if (PSVECSquareMag(&d) < best) {
            best = PSVECSquareMag(&d);
            index = i;
        }
    }
    return index;
}

inline void FlyObj::LoadMotion_unk(const char* name, const char* path) {
    fn_80097A88(&mModel, fn_802180A8(name), path);
}

FlyObj::FlyObj() : mRadius(3.0f), mModel(8, 32), mPos(0.0f, 0.0f, 0.0f), mRot(0.0f, 0.0f, 0.0f), mVel(0.0f, 0.0f, 1.0f) {
    // inferred: the original copies the vectors onto themselves (psq_l/psq_st of the same member)
    mPos = mPos;
    mColli.m20 = 0x18;
    mState = 1;
    mVel = mVel;
    mColli.mFlags = (mColli.mFlags & ~1) | 8;
    mColli.m0C = (int)mOwner;
}

FlyObj::~FlyObj() {}

// NONMATCHING: 98.8% - the sphere temporaries at the end sit below the state 3 temporaries here (above
// in the original) and a few float registers. The fall is applied before the rotation reset.
void FlyObj::fn_80427870() {
    if (mState <= 1) {
        return;
    }
    if (mState == 2) {
        mPos += mVel;
        mRot = mVel.ToRotation_unk();
    } else if (mState == 3) {
        if (--mTimer < 0) {
            return;
        }
        mPos += atn::Vector3(0.0f, -2.0f, 0.0f);
        mRot = atn::Vector3(0.0f, 0.0f, 0.0f);
        mRot.x = 1.5707964f;
    }
    mModel.fn_80092A4C();
    atn::Matrix3x4 mtx = lbl_807C7060;
    atn::Matrix3x4 rot;
    __MakeRotation_unk(rot, mRot);
    PSMTXConcat(rot.m, mtx.m, mtx.m);
    __SetTrans_unk(mtx, mPos.x, mPos.y, mPos.z);
    __MtxCopy(mModel.mMtx, mtx.m);
    mModel.m54 = MaxScale_unk(mtx);
    mModel.CalcMatrixStack(atn::ModelNodeCallback_unk());
    mModel.fn_80097E80(1);
    mColli.mSphere = atn::ColliSphere::Sphere_unk(atn::Vector3(mPos), mRadius);
}

GimmickCommon* FlyObjManager::fn_80427EB4(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) FlyObjManager(parent, desc);
    }
    return 0;
}

FlyObjManager::FlyObjManager(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mRadius(3.0f), mDesc(desc), mLoadStep(0), mPhase(0) {
    mCount = desc->m20;
    mObjs.Resize_unk(mCount, 0);
    mRange = mDesc->m40;
    m6C = mDesc->mPos;
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', mDesc->mRotY);
    atn::Vector3 dir;
    dir = atn::Vector3(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(mtx, &dir, &dir);
    mVel = dir;
    mVel.y = mDesc->m48;
    PSVECNormalize(&mVel, &mVel);
    mVel *= mDesc->m44;
}

FlyObjManager::~FlyObjManager() {}

int FlyObjManager::vf0C() {
    if (mLoadStep == 0) {
        if (fn_803EC91C() == 0) {
            for (unsigned int i = 0; i < mCount; i++) {
                mObjs.mData[i].mOwner = this;
                fn_8008AD4C(&mObjs.mData[i].mModel, mModelName, 0);
                for (int j = 0; j < 2; j++) {
                    mObjs.mData[i].LoadMotion_unk(mMotionName[j], mMotionPath[j]);
                }
            }
            mLoadStep = 1;
        }
    } else if (mLoadStep == 1) {
        for (unsigned int i = 0; i < mCount; i++) {
            if (IsLoading_unk(&mObjs.mData[i].mModel)) {
                return 0;
            }
        }
        mLoadStep = 2;
    } else if (mLoadStep == 2) {
        for (unsigned int i = 0; i < mCount; i++) {
            mObjs.mData[i].mRadius = mRadius;
        }
        mPhase = 1;
        return 1;
    }
    return 0;
}

void FlyObjManager::vf10() {
    if (mDesc->m7C != 0 && mPhase == 3) {
        for (unsigned int i = 0; i < mCount; i++) {
            mObjs.mData[i].fn_80427870();
        }
    }
}

void FlyObjManager::vf14() {
    if (mDesc->m7C != 0 && mPhase == 3) {
        for (unsigned int i = 0; i < mCount; i++) {
            FlyObj* obj = &mObjs.mData[i];
            if (obj->mState > 1 && (obj->mState != 3 || obj->mTimer >= 0) && lbl_8087F0A8->m18 != 0) {
                obj->mModel.fn_8008CD60();
            }
        }
    }
}

void FlyObjManager::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    char* path = mMotionPath[0];
    char* name = mMotionName[0];
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            strcpy(mModelName, csv.fn_8005B9CC());
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            strcpy(name, csv.fn_8005B9CC());
            path += 0x100;
            name += 0x100;
        } else if (strcmp(field, "COLLI_RADIUS") == 0) {
            mRadius = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

int FlyObjManager::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 0:
    case 1:
        for (unsigned int i = 0; i < mCount; i++) {
            mObjs.mData[i].Start_unk(m6C, mVel);
        }
        break;
    case 2:
        for (unsigned int i = 0; i < mCount; i++) {
            atn::Vector3 pos(RandRange_unk(-1.0f, 1.0f), RandRange_unk(-1.0f, 1.0f),
                             RandRange_unk(-1.0f, 1.0f));
            PSVECNormalize(&pos, &pos);
            pos *= RandRange_unk(0.0f, mRange);
            pos += m6C;
            mObjs.mData[i].Start_unk(pos, mVel);
            FlyObj* obj = &mObjs.mData[i];
            obj->mState = 2;
            PlayMotion_unk(&obj->mModel, 0, 1.0f);
            float len = obj->mModel.fn_80097D7C(0);
            obj->mModel.mTracks[0].m08 = RandRange_unk(0.0f, len - 1.0f);
        }
        msg->mType = 3;
        break;
    }
    return mPhase = msg->mType;
}

// NONMATCHING: 99.9% - the speed 1.0f goes through f1 (and 0.1f is before 1000.0f in .sdata2 in the
// original); the unsigned index of FindNearest_unk fixed the index/offset registers
int FlyObjManager::vf44(GimmickHitMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    int index = FindNearest_unk(msg->mPos);
    if (index >= 0 && index < (int)mCount) {
        FlyObj* obj = &mObjs.mData[index];
        obj->mState = 3;
        PlayMotion_unk(&obj->mModel, 1, 0.0f);
        obj->mTimer = 60;
        fn_80449A18(Inventory::instance_, 0, 1, 0, 0);
        int money, count;
        money = count = 0;
        fn_804491F4(&count, &money, 1, 0, 0, 1.0f);
        if (lbl_8087F8A8 != 0) {
            wchar_t text[0x20];
            swprintf(text, 0x20, L"%d", count);
            fn_8054D794(lbl_8087F8A8, 2, text, &obj->mPos, 0, 0, 1, 0, 0, 0, 1, -1, -1);
        }
    }
    return 0;
}

} // namespace gmk
