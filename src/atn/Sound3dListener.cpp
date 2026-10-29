#include "math/Vec.h"

#include "atn/SoundCore.h"

extern "C" atn::SoundEngine_unk* lbl_8087EE90;

// inferred: engine functions (atn/SoundEngine_unk); names unknown
extern "C" unsigned int fn_80049B74(atn::SoundEngine_unk* engine, const char* label); // sound id of a label (0: none)
extern "C" void fn_80049E3C(atn::SoundEngine_unk* engine, unsigned int id);
extern "C" int fn_80049AE4(atn::SoundEngine_unk* engine, nw4r::snd::SoundHandle* handle, unsigned int id, void* info);
extern "C" int fn_80049B08(atn::SoundEngine_unk* engine, nw4r::snd::SoundHandle* handle, unsigned int id, void* info);
extern "C" const char* fn_80049CDC(atn::SoundEngine_unk* engine, unsigned int id); // label of a sound id
extern "C" int fn_80049B2C(atn::SoundEngine_unk* engine, unsigned int id);

inline atn::Sound3dActor::Sound3dActor() : mActor(lbl_8087EE90->mPlayer, &lbl_8087EE90->m364) {}

// inferred: debug message output (lbl_8087EEB8)
struct DebugMessages_unk {
    void fn_800697D8(const char* msg);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;

// inferred: nw4r::snd::SoundStartable::StartResult names
static const char* const sSoundErrors[] = {
    "success", "low priority", "invalid label", "invalid id", "not loaded", "not enough heap",
    "cannot open file", "not available", "cannot allocate track", "not enough instance", "invalid parameter",
    "invalid seq start label",
};
static const char* const sUnknownError = "";

static inline const char* GetSoundError_unk(int code) {
    if (0 <= code && code <= 11) {
        return sSoundErrors[code];
    }
    return sUnknownError;
}

static inline void PrintSoundError_unk(int code, const char* label) {
    DebugMessages_unk* dbg = lbl_8087EEB8;
    if (dbg != 0) {
        const char* error = GetSoundError_unk(code);
        char buf[256];
        sprintf(buf, "SoundError: %s [%s]", error, label);
        lbl_8087EEB8->fn_800697D8(buf);
    }
}

namespace atn {

Sound3dListener::Sound3dListener()
    : mPos(0.0f, 0.0f, 0.0f), mRot(0.0f, 0.0f, 0.0f) {
    lbl_8087EE90->m364.PushBack(&mListener);
}

Sound3dListener::~Sound3dListener() {
    lbl_8087EE90->m364.Erase(&mListener);
}

void Sound3dListener::fn_800C7F08(const Vector3& pos, const Vector3& rot) {
    mPos = pos;
    mRot = rot;
    Matrix3x4 m;
    Matrix3x4 rotY;
    Matrix3x4 view;
    Vector3 dir(0.0f, 0.0f, 1.0f);
    rotY.fn_8013A13C(mRot.y);
    PSMTXMultVec(rotY.m, &dir, &dir);
    Vector3 target;
    Vector3 up(0.0f, 1.0f, 0.0f);
    target.Set_unk(mPos.x + dir.x, mPos.y + dir.y, mPos.z + dir.z);
    C_MTXLookAt(view.m, &mPos, &up, &target);
    m = view;
    fn_80715BA0(&mListener, m.m);
}

Sound3dActor::~Sound3dActor() {}

SoundCore::SoundCore()
    : m90(-1), m98(0), m9C(0), mA0(0), mA4(0), mA8(0), mAC(0), mB0(0), mB4(0), mB8(1.0f),
      mBC(1.0f), mC0(0.0f), mC8(0), mCC(0.0f), mD4(0),
      mD8(0.0f, 0.0f, 0.0f), mE4(0.0f), mE8(0), mEC(lbl_8087D930), m10C(lbl_8087D934),
      m12C(lbl_8087D938) {}

SoundCore::~SoundCore() {
    mHandle.Stop(0);
    mHandle.DetachSound();
    m9C = 0;
    mA0 = 0;
    mA4 = 0;
}

bool SoundCore::fn_800C8278() const {
    return mA4 <= 0 && !mHandle.IsAttachedSound();
}

void SoundCore::fn_800C82A0(int stop) {
    if (stop) {
        mHandle.Stop(0);
    }
    mHandle.DetachSound();
    m9C = 0;
    mA0 = 0;
    mA4 = 0;
}

void SoundCore::fn_800C82FC() {
    mEC.Update();
    m10C.Update();
    m12C.Update();
    mHandle.fn_8070AD40_unk(mEC.m18);
    mHandle.fn_8070AD50_unk(m10C.m18);
    mHandle.fn_8070AD60_unk(mC8, m12C.m18);
    if (mD4 != 0) {
        mD8 = m08.mActor.mPosition;
    }
}

// inferred: sets the start offset (enableFlag ENABLE_START_OFFSET, offset type MSEC)
static inline void SetStartOffset_unk(SoundStartInfo_unk& info, int offset) {
    info.startOffset = offset;
    info.enableFlag |= 1;
    info.startOffsetType = 0;
}

inline void SoundCore::StartSetup_unk(unsigned int id) {
    m9C = 0;
    fn_800CA2FC(0.0f, 0.0f, 0.0f);
    fn_800CA42C(1.0f, 0);
    fn_800CA5E8(0.0f, 0);
    mC8 = 0;
    mCC = 0.0f;
    m12C.Start(mCC, 0.0f);
    mHandle.fn_8070AD80_unk();
    fn_800CA798(0, 0.0f);
    mD0 = 0;
    m98 = id;
}

void SoundCore::fn_800C8780(const char* label) {
    unsigned int id = fn_80049B74(lbl_8087EE90, label);
    if (id != 0) {
        Start_unk(id);
    } else {
        PrintSoundError_unk(2, label);
    }
}

void SoundCore::fn_800C8BA4(const char* label, const Vector3* pos) {
    unsigned int id = fn_80049B74(lbl_8087EE90, label);
    if (id != 0) {
        fn_800CA7E8(pos);
        m08.mActor.mStartOffset_unk = mD0;
        fn_80049E3C(lbl_8087EE90, id);
        SoundStartInfo_unk info;
        SetStartOffset_unk(info, m08.mActor.mStartOffset_unk);
        if ((m90 = fn_80718470(&m08.mActor, &mHandle, id - 1, &info)) != 0) {
            PrintSoundError_unk(m90, fn_80049CDC(lbl_8087EE90, id));
        }
        StartSetup_unk(id);
        m9C |= 2;
        if (fn_80049B2C(lbl_8087EE90, id)) {
            m9C |= 8;
        }
    } else {
        PrintSoundError_unk(2, label);
    }
}

void SoundCore::fn_800C9010(unsigned int id) {
    Start_unk(id);
}

inline void SoundCore::Start_unk(unsigned int id) {
    SoundStartInfo_unk info;
    SetStartOffset_unk(info, mD0);
    fn_80049E3C(lbl_8087EE90, id);
    if ((m90 = fn_80049AE4(lbl_8087EE90, &mHandle, id, &info)) != 0) {
        PrintSoundError_unk(m90, fn_80049CDC(lbl_8087EE90, id));
    }
    StartSetup_unk(id);
    if (fn_80049B2C(lbl_8087EE90, id)) {
        m9C |= 8;
    }
}

void SoundCore::fn_800C93D4(const char* label) {
    unsigned int id = fn_80049B74(lbl_8087EE90, label);
    if (id != 0) {
        Prepare_unk(id);
    } else {
        PrintSoundError_unk(2, label);
    }
}

void SoundCore::fn_800C97F8(const char* label, const Vector3* pos) {
    unsigned int id = fn_80049B74(lbl_8087EE90, label);
    if (id != 0) {
        fn_800CA7E8(pos);
        m08.mActor.mStartOffset_unk = mD0;
        fn_80049E3C(lbl_8087EE90, id);
        SoundStartInfo_unk info;
        SetStartOffset_unk(info, m08.mActor.mStartOffset_unk);
        if ((m90 = fn_807184D0(&m08.mActor, &mHandle, id - 1, &info)) != 0) {
            PrintSoundError_unk(m90, fn_80049CDC(lbl_8087EE90, id));
        }
        StartSetup_unk(id);
        m9C |= 2;
        if (fn_80049B2C(lbl_8087EE90, id)) {
            m9C |= 8;
        }
    } else {
        PrintSoundError_unk(2, label);
    }
}

void SoundCore::fn_800C9C64(unsigned int id) {
    Prepare_unk(id);
}

inline void SoundCore::Prepare_unk(unsigned int id) {
    SoundStartInfo_unk info;
    SetStartOffset_unk(info, mD0);
    fn_80049E3C(lbl_8087EE90, id);
    if ((m90 = fn_80049B08(lbl_8087EE90, &mHandle, id, &info)) != 0) {
        PrintSoundError_unk(m90, fn_80049CDC(lbl_8087EE90, id));
    }
    StartSetup_unk(id);
    if (fn_80049B2C(lbl_8087EE90, id)) {
        m9C |= 8;
    }
}

bool SoundCore::fn_800CA028() {
    return mHandle.IsAttachedSound() && !mHandle.IsPrepared();
}

void SoundCore::fn_800CA098() {
    mHandle.fn_80709AB0_unk();
}

void SoundCore::fn_800CA0AC(float volume, int frames) {
    mB8 = volume;
    fn_800CA144(frames);
}

void SoundCore::fn_800CA144(int frames) {
    mHandle.SetVolume(GetCatVolume_unk(), frames);
}

void SoundCore::fn_800CA1D4(int pause, unsigned long mask, int frames) {
    if (pause) {
        mA0 |= mask;
    } else {
        mA0 &= ~mask;
    }
    if (pause && mA0 != 0 && !mHandle.IsPause()) {
        mHandle.Pause(pause, frames);
    } else if (!pause && mA0 == 0 && mHandle.IsPause()) {
        mHandle.Pause(pause, frames);
    }
}

void SoundCore::fn_800CA2FC(float a, float b, float c) {
    mC4 = a;
    fn_800CA3D4();
    mHandle.fn_8070AD90_unk(1, b);
    mHandle.fn_8070AD90_unk(2, c);
}

void SoundCore::fn_800CA3D4() {
    float v = mC4;
    if (mA8 != 0) {
        v *= mA8->mFxSend;
    }
    if (mAC != 0) {
        v *= mAC->mFxSend;
    }
    if (mB0 != 0) {
        v *= mB0->mFxSend;
    }
    mHandle.fn_8070AD90_unk(0, v);
}

void SoundCore::fn_800CA42C(float pitch, int frames) {
    if (pitch < 0.01f) {
        pitch = 0.01f;
    }
    mBC = pitch;
    fn_800CA514(frames);
}

void SoundCore::fn_800CA514(int frames) {
    float v = mBC;
    if (mA8 != 0) {
        v *= mA8->mPitch;
    }
    if (mAC != 0) {
        v *= mAC->mPitch;
    }
    if (mB0 != 0) {
        v *= mB0->mPitch;
    }
    mEC.Start(v, frames);
}

void SoundCore::fn_800CA5E8(float pan, int frames) {
    mC0 = -pan;
    fn_800CA6C4(frames);
}

void SoundCore::fn_800CA6C4(int frames) {
    float v = mC0;
    if (mA8 != 0) {
        v -= mA8->mPan;
    }
    if (mAC != 0) {
        v -= mAC->mPan;
    }
    if (mB0 != 0) {
        v -= mB0->mPan;
    }
    m10C.Start(v, frames);
}

void SoundCore::fn_800CA798(int enable, float value) {
    mD4 = enable;
    mD8 = m08.mActor.mPosition;
    mE4 = value;
    if (enable) {
        m08.mActor.mUserParam_unk = &mD4;
    } else {
        m08.mActor.mUserParam_unk = 0;
    }
}

void SoundCore::fn_800CA7D4(int offset) {
    if (offset < 0) {
        offset = 0;
    }
    mD0 = offset;
}

void SoundCore::fn_800CA7E8(const Vector3* pos) {
    fn_80714C50(&m08.mActor, pos);
    if (mD4 != 0) {
        mD8 = m08.mActor.mPosition;
    }
}

const char* SoundCore::fn_800CA834() {
    return fn_80049CDC(lbl_8087EE90, m98);
}

float SoundCore::fn_800CA844() const {
    float v = mB8;
    if (mA8 != 0) {
        v *= mA8->mVolume;
    }
    if (mAC != 0) {
        v *= mAC->mVolume;
    }
    if (mB0 != 0) {
        v *= mB0->mVolume;
    }
    nw4r::snd::detail::SoundParam_unk* p = mHandle.GetParam_unk();
    if (p != 0) {
        v *= p->m00;
    }
    return v;
}

float SoundCore::fn_800CA8B0() {
    nw4r::snd::detail::SoundParam_unk* p = mHandle.GetParam_unk();
    if (p != 0) {
        return p->m08;
    }
    return 0.0f;
}

float SoundCore::fn_800CA8E0() {
    nw4r::snd::detail::SoundParam_unk* p = mHandle.GetParam_unk();
    if (p != 0) {
        return p->m0C;
    }
    return 1.0f;
}

int SoundCore::fn_800CA910() {
    StrmSoundHandle_unk strm(&mHandle);
    SoundDataInfo_unk info;
    if (strm.ReadStrmDataInfo(&info)) {
        float rate = info.mSampleRate;
        return 1000.0f * ((float)info.mLength / rate);
    }
    WaveSoundHandle_unk wave(&mHandle);
    SoundDataInfo_unk winfo;
    if (wave.ReadWaveSoundDataInfo(&winfo)) {
        float rate = winfo.mSampleRate;
        return 1000.0f * ((float)winfo.mLength / rate);
    }
    return -1;
}

int SoundCore::fn_800CAA88() {
    StrmSoundHandle_unk strm(&mHandle);
    SoundDataInfo_unk info;
    if (strm.ReadStrmDataInfo(&info)) {
        float rate = info.mSampleRate;
        return 1000.0f * ((float)(unsigned long)strm.GetPlaySamplePosition() / rate);
    }
    WaveSoundHandle_unk wave(&mHandle);
    SoundDataInfo_unk winfo;
    if (wave.ReadWaveSoundDataInfo(&winfo)) {
        float rate = winfo.mSampleRate;
        return 1000.0f * ((float)(unsigned long)wave.GetPlaySamplePosition() / rate);
    }
    return -1;
}

void SoundCore::fn_800CAC48(int track, float volume, int frames) {
    StrmSoundHandle_unk strm(&mHandle);
    SoundDataInfo_unk info;
    if (strm.ReadStrmDataInfo(&info)) {
        unsigned long mask = 1 << track;
        strm.SetTrackVolume(mask, volume, frames);
    }
}

float SoundCore::fn_800CACEC() {
    if (fn_800C8278() || (m9C & 8) != 8) {
        return 0.0f;
    }
    StrmSoundHandle_unk strm(&mHandle);
    return strm.fn_8071A800_unk() / 100.0f;
}

} // namespace atn
