#include "gmk/FallingTower.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" double sin(double x);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: limits of the wave blend factor
static float sBlendMin = 0.0f;
static float sBlendMax = 1.0f;

// inferred: copies a string unless it is the destination itself (name invented)
inline void CopyString_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}
// inferred: translation matrix by value (name invented)
inline atn::Matrix3x4 TransMtx_unk(const atn::Vector3& v) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, v.x, v.y, v.z);
    return m;
}

namespace gmk {

// inferred: height of the wave at time t (name invented)
inline float FallingTower::Wave_unk::Height_unk(float t) const {
    return mAmp * (float)sin(atn::WrapAngle_unk(mSpeed * t + mPhase));
}

GimmickCommon* FallingTower::fn_8040FAC0(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) FallingTower(parent, id, arg);
    }
    return 0;
}

FallingTower::FallingTower(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mFrame3(0), mFrame4(0), mSplash0(0), mSplash1(0), mAlpha(1.0f),
      mWaveTime(0.0f),
      mBlendStart(0.0f), mBlendDur(0.0f), mCur(mWaves[0]), mNext(mWaves[0]) {
    m54 = 0;
}

FallingTower::~FallingTower() {}

int FallingTower::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        return 1;
    }
    return 0;
}

void FallingTower::vf10() {
    if (m54 == 0) {
        Start();
    } else if (m54 == 2) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    switch (m54) {
    case 3:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (mFrame3 >= mSplash0) {
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
        mFrame3++;
        break;
    case 4:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (mFrame4 >= mSplash1) {
            GimmickMsg_unk msg(5);
            vf40(&msg);
        }
        mFrame4++;
        break;
    }
    fn_8041044C();
}

void FallingTower::vf14() {
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int FallingTower::vf1C() {
    int ret = 0;
    if (mModel.fn_8008B140() != 0 || mColli.fn_800580BC() != 0 || mEfp.fn_80237874() != 0) {
        ret = 1;
    }
    return ret;
}

void FallingTower::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "SPLASH") == 0) {
            mSplash0 = atoi(csv.fn_8005B9CC());
            mSplash1 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(field, "MATALPHA") == 0) {
            CopyString_unk(mMaterial.mStr, csv.fn_8005B9CC());
            mAlpha = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "WAVE") == 0) {
            int state = fn_800DC12C(csv.fn_8005B9CC());
            unsigned int index = fn_800DC12C(csv.fn_8005B9CC());
            if (index <= 1) {
                mWaves[state][index].mAmp = fn_800DC288(csv.fn_8005B9CC());
                mWaves[state][index].mSpeed = fn_800DC288(csv.fn_8005B9CC());
                mWaves[state][index].mPhase = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            }
        } else if (strcmp(field, "WAVEBLEND") == 0) {
            int index = fn_800DC12C(csv.fn_8005B9CC());
            mBlend[index] = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void FallingTower::Start() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// NONMATCHING: 99.7% - float register allocation (time/sine after the first wave, and the 0.0 of the
// second loop).
void FallingTower::fn_8041044C() {
    int nodes[2];
    nodes[0] = mModel.fn_80092814("joint1", 0);
    nodes[1] = mModel.fn_80092814("joint2", 0);
    float heights[2];
    for (int i = 0; i < 2; i++) {
        float a = mCur->Height_unk(mWaveTime);
        float b = mNext->Height_unk(mWaveTime);
        float blend = (mWaveTime - mBlendStart) / mBlendDur;
        blend = blend < sBlendMin ? sBlendMin : (blend > sBlendMax ? sBlendMax : blend);
        heights[i] = blend * (a - b) + b;
    }
    for (int i = 0; i < 2; i++) {
        if (nodes[i] > -1) {
            MtxPtr mtx = mModel.mNodeMtx[nodes[i]];
            *(atn::Matrix3x4*)mtx = TransMtx_unk(atn::Vector3(0.0f, heights[i], 0.0f));
            PSMTXConcat(mModel.mMtx, mtx, mtx);
        }
    }
    mWaveTime += 0.033333335f;
}

int FallingTower::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
        mFrame3 = 0;
        mFrame4 = 0;
        mWaveTime = 0.0f;
        mBlendDur = 1.0f;
        mBlendStart = 0.0f;
        mCur = mWaves[0];
        mNext = mWaves[0];
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        mColli.mFlags &= ~1;
        break;
    case 3: {
        mModel.fn_80097C08(0, 0, 0, 1, 1, 0.0f, 0.1f);
        Color4f_unk color;
        atn::Vector3 rot;
        atn::Vector3 pos;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &mEfp, &mModel, 0, pos, rot, color, -1, -1, 1, 1.0f);
        mColli.mFlags &= ~1;
        break;
    }
    case 4:
        mCur = mWaves[1];
        mNext = mWaves[0];
        mBlendDur = mBlend[0] / 30.0f;
        mBlendStart = mWaveTime;
        mColli.mFlags |= 1;
        break;
    case 5:
        mCur = mWaves[0];
        mNext = mWaves[1];
        mBlendDur = mBlend[1] / 30.0f;
        mBlendStart = mWaveTime;
        mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        mColli.mFlags |= 1;
        break;
    }
    return m54 = msg->mType;
}

void FallingTower::vf70(int state) {
    GimmickMsg_unk msg(0);
    switch (state) {
    case 0:
    case 1:
        msg.mType = 1;
        vf40(&msg);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        msg.mType = 5;
        vf40(&msg);
        break;
    }
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

Model_unk* FallingTower::vf38() {
    return &mModel;
}

} // namespace gmk
