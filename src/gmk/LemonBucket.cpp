#include "gmk/LemonBucket.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087858(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" GimmickCommon* fn_8041524C(atn::Task* parent, int id, int arg); // inferred: gmk::Lemon factory

// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: camera (class unknown); names invented
struct LemonCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" LemonCamera_unk* lbl_8087EFB4;
// inferred: event state (class unknown): m48 nonzero while an event runs
struct LemonEvent_unk {
    unsigned char m00[0x48];
    int m48;
};
extern "C" LemonEvent_unk* lbl_8087F098;
// inferred: player manager and its first character (only the rotation is read)
struct LemonPlayer_unk {
    unsigned char m000[0x538];
    float mRotY; // 0x538
};
struct LemonPlayers_unk {
    unsigned char m00[0x48];
    LemonPlayer_unk* mHead; // 0x48
};
extern "C" LemonPlayers_unk* lbl_8087F8A0;
// inferred: view of gmk::Lemon (velocity at 0x17C)
struct LemonView_unk {
    unsigned char m000[0x17C];
    atn::Vector3 mVel;
};

static const char* const sSe[] = {"SE_EVCOM_013", "SE_DGCOM_010"};

// inferred: squared distance of the camera (name invented)
inline float CameraDistSq_unk(const atn::Vector3& pos) {
    atn::Vector3 d = lbl_8087EFB4->mPos - pos;
    return fn_805F9920(&d);
}
inline float Rand_unk() {
    return rand() % 1001 / 1000.0f;
}
// inferred: *this = *this * rot(axis, rad) (name invented)
inline void Rotate_unk(atn::Matrix3x4& mtx, char axis, float rad) {
    Mtx r;
    Mtx t;
    PSMTXRotRad(r, axis, rad);
    PSMTXConcat(mtx.m, r, t);
    __MtxCopy(mtx.m, t);
}
// inferred: rotation z, then y, then x (name invented)
inline void RotateZYX_unk(atn::Matrix3x4& mtx, const atn::Vector3& rot) {
    if (0.0f != rot.z) {
        Rotate_unk(mtx, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        Rotate_unk(mtx, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        Rotate_unk(mtx, 'x', rot.x);
    }
}

namespace gmk {

GimmickCommon* LemonBucket::fn_8042F7EC(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) LemonBucket(parent, desc);
    }
    return 0;
}

LemonBucket::LemonBucket(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mModel(8, 32), mModel2(8, 32), mPosRand(2.0f),
      mRotRand(1.5707964f), mSpeedRand(1.2f), mSpeedBase(0.6f), mScattered(0), mScatterFrame(0.0f), mRange(300.0f) {
    m54 = 0;
    mE8 = 6;
    mEC = 0;
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    mDir.x = 0.0f;
    mDir.y = 0.0f;
    mDir.z = 1.0f;
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', m78.y);
    PSMTXMultVec(mtx, &mDir, &mDir);
}

// The loop is written here (not HideLemons_unk) with the int index compared as unsigned
int LemonBucket::vf0C() {
    if (fn_803EC91C() == 0 && fn_800D3FA4() != 0) {
        m54 = 1;
        for (int i = 0; i < (unsigned int)mLemons.mSize; i++) {
            GimmickCommon* lemon = mLemons[i];
            lemon->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

void LemonBucket::Start() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// NONMATCHING: 99.77% - loop registers (index and offset swapped in the fade/switch loops; the int
// index compared as unsigned gives the order but not the registers)
void LemonBucket::vf10() {
    if (mDesc->m7C == 0) {
        if (mScattered != 0) {
            mScattered = 0;
            HideLemons_unk();
        }
        return;
    }
    if (CameraDistSq_unk(m6C) > mRange * mRange) {
        if (m54 == 4) {
            Start();
        }
        return;
    }
    if (m54 == 0) {
        Start();
    }
    if (m54 == 2) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
    }
    if (m54 == 3) {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        for (unsigned int i = 0; i < mFades.mSize; i++) {
            mFades[i].fn_803EBE74(&mModel2);
        }
        for (unsigned int i = 0; i < mSwitches.mSize; i++) {
            mSwitches[i].fn_803EC0F8(&mModel2);
        }
        if (mScattered == 0 && mModel2.mTracks[0].m08 >= mScatterFrame) {
            fn_8042FDBC();
        } else {
            float frame = mModel2.mTracks[0].m08;
            if (frame >= mModel2.fn_80097D7C(0)) {
                GimmickMsg_unk msg(4);
                vf40(&msg);
            }
        }
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
    }
}

void LemonBucket::fn_8042FDBC() {
    if (mScattered != 0) {
        return;
    }
    mScattered = 1;
    MtxPtr node = mModel2.GetNodeMtx_unk("lemon");
    if (node == 0) {
        return;
    }
    fn_800C31F4(sSe[1], 0, -1, 1.0f);
    GimmickMsg_unk msg(2);
    for (unsigned int i = 0; i < mLemons.mSize; i++) {
        float z = node[2][3];
        float y = node[1][3];
        float x = node[0][3];
        atn::Vector3 pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        pos.x += mPosRand * Rand_unk() - mPosRand / 2.0f;
        pos.y += mPosRand * Rand_unk() - mPosRand / 2.0f;
        pos.z += mPosRand * Rand_unk() - mPosRand / 2.0f;
        atn::Vector3 rot;
        rot.x = mRotRand * Rand_unk() - mRotRand / 2.0f;
        rot.y = mRotRand * Rand_unk() - mRotRand / 2.0f;
        rot.z = mRotRand * Rand_unk() - mRotRand / 2.0f;
        atn::Vector3 vel = mDir * (mSpeedBase + mSpeedRand * Rand_unk() - mSpeedRand / 2.0f);
        atn::Matrix3x4 mtx;
        mtx.SetIdentity();
        RotateZYX_unk(mtx, rot);
        PSMTXMultVec(mtx.m, &vel, &vel);
        mLemons[i]->vf40(&msg);
        mLemons[i]->m6C = pos;
        ((LemonView_unk*)mLemons[i])->mVel = vel;
        mLemons[i]->mFlags &= ~4;
    }
}

void LemonBucket::vf14() {
    if (mDesc->m7C != 0 && lbl_8087F0A8->m18 != 0 && m54 != 5) {
        if (CameraDistSq_unk(m6C) > 62500.0f) {
            return;
        }
        if (vf38() != 0) {
            fn_803ED774(vf38());
        }
    }
}

int LemonBucket::vf1C() {
    if (mModel.fn_8008B140() != 0 || mModel2.fn_8008B140() != 0) {
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.13% - register allocation (the field pointer and the string pool base swap
// registers); tried several declaration orders of the counters
void LemonBucket::vf20() {
    unsigned long size = mBinary.fn_8047059C();
    const char* data = (const char*)mBinary.fn_80470580();
    atn::CsvTab csv(data, size);
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    unsigned int fades = 0;
    unsigned int switches = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL_BEFORE") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MODEL_AFTER") == 0) {
            fn_8008AD4C(&mModel2, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel2, &csv);
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel2, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
        } else if (strcmp(field, "MATALPHA") == 0) {
            fades++;
        } else if (strcmp(field, "NODEVIS") == 0) {
            switches++;
        } else if (strcmp(field, "SCATTER") == 0) {
            mScatterFrame = fn_800DC288(csv.fn_8005B9CC());
            mRange = fn_800DC288(csv.fn_8005B9CC());
            mLemons.Resize_unk(atoi(csv.fn_8005B9CC()), 0);
            for (unsigned int i = 0; i < mLemons.mSize; i++) {
                mLemons[i] = fn_8041524C(this, 30000, i);
            }
        }
    } while (csv.fn_8005BBF8() != 0);
    mFades.Resize_unk(fades, 0);
    mSwitches.Resize_unk(switches, 0);
}

void LemonBucket::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    int fade = 0;
    int sw = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MATALPHA") == 0) {
            mFades[fade].fn_803EBC44(&csv, &mModel2);
            fade++;
        } else if (strcmp(field, "NODEVIS") == 0) {
            mSwitches[sw].fn_803EC0A4(&csv, &mModel2);
            sw++;
        }
    } while (csv.fn_8005BBF8() != 0);
}

// HideLemons_unk with the int index compared as unsigned gives the loop of the original
int LemonBucket::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    switch (m54) {
    case 1:
        mModel2.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        fn_803ED5D0(&mModel2);
        mModel2.mTracks[0].m08 = 0.0f;
        fn_803ED0D4(&mModel, 1);
        fn_803ED0D4(&mModel2, 1);
        if (mScattered != 0) {
            mScattered = 0;
            HideLemons_unk();
        }
        break;
    case 2:
    case 3:
        mModel2.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel2.mTracks[0].m0C = 1.0f;
        fn_800C31F4(sSe[0], 0, -1, 1.0f);
        if (mScattered != 0) {
            mScattered = 0;
            HideLemons_unk();
        }
        break;
    case 4:
        mModel2.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel2.mTracks[0].m08 = mModel2.fn_80097D7C(0);
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        fn_8042FDBC();
        break;
    }
    return m54;
}

void LemonBucket::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int LemonBucket::vf60() {
    return m54 == 1;
}

int LemonBucket::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() != 0) {
        atn::Vector3 d = m6C - *pos;
        if (atn::Abs_unk(d.y) < 30.0f) {
            d.y = 0.0f;
        }
        float len = fn_805F9920(&d);
        if (len > 0.0f) {
            PSVECNormalize(&d, &d);
        }
        if (lbl_8087F098 != 0 && lbl_8087F098->m48 != 0) {
            if (len < 400.0f) {
                return 1;
            }
        } else {
            LemonPlayer_unk* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
            if (len < 144.0f && player != 0) {
                atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                Mtx mtx;
                PSMTXRotRad(mtx, 'y', player->mRotY);
                PSMTXMultVec(mtx, &fwd, &fwd);
                if (PSVECDotProduct(&fwd, &d) >= 0.70710677f) {
                    atn::Vector3 back(-d.x, -d.y, -d.z);
                    return PSVECDotProduct(&mDir, &back) < 0.9659258f;
                }
            }
        }
    }
    return 0;
}

void LemonBucket::vf4C(CharaTask* chara) {
    if (m54 == 1) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
    }
}

void LemonBucket::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    fn_8008771C(group, "positionRand", &mPosRand, 0, 0, 0.0f, 100.0f, 0.5f);
    fn_80087858(group, "rotationRand", &mRotRand, 0, 0, 0.0f, 90.0f, 1.0f);
    fn_8008771C(group, "speedRand", &mSpeedRand, 0, 0, 0.0f, 100.0f, 0.5f);
    fn_8008771C(group, "speedBase", &mSpeedBase, 0, 0, 0.0f, 100.0f, 0.5f);
}

atn::Model* LemonBucket::vf38() {
    if (m54 == 1) {
        return &mModel;
    }
    return &mModel2;
}

} // namespace gmk
