#ifndef ATN_SOUNDCORE_H
#define ATN_SOUNDCORE_H

// atn::SoundCore (RTTI name) and the minimal NW4R declarations it needs; functions defined in
// atn/Sound3dListener.cpp. Other files still reach the object through the partial view
// atn::SoundObject (atn/SoundRef.h) and cast.

#include "math/Vec.h"

// Minimal declarations of the NW4R classes used here (the game build does not include the NW4R
// headers). LinkListImpl::Insert/Erase return an iterator in NW4R; the result is unused here and
// the mangled names do not include it.
namespace nw4r {
namespace math {
struct VEC3;
struct MTX34;
} // namespace math
} // namespace nw4r

namespace nw4r {
namespace db {
void Panic(const char* file, int line, const char* fmt, ...);
} // namespace db

namespace ut {
struct LinkListNode {
    LinkListNode* mNext;
    LinkListNode* mPrev;
};
namespace detail {
class LinkListImpl {
public:
    class Iterator {
    public:
        explicit Iterator(LinkListNode* p) : mPointer(p) {}
        LinkListNode* mPointer;
    };
    Iterator GetEndIter() { return Iterator(&mEndNode); }
    void Insert(Iterator it, LinkListNode* p);
    void Erase(LinkListNode* p);

    unsigned long mSize;
    LinkListNode mEndNode;
};
} // namespace detail
} // namespace ut

namespace snd {
enum AuxBus { AUX_A, AUX_B, AUX_C };
namespace detail {
// inferred: only the virtual used here (IsPrepared at 0x14) and the parameter block at 0x30
struct SoundParam_unk {
    float m00;
    float m04;
    float m08;
    float m0C;
};
class BasicSound {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual bool IsPrepared() const; // 0x14
    void StartPrepared();
    void Stop(int fadeFrames);
    void Pause(bool flag, int fadeFrames);
    bool IsPause() const;
    void SetVolume(float volume, int frames);
    void SetPitch(float pitch);
    void SetLpfFreq(float lpfFreq);
    void SetBiquadFilter(int type, float value);
    void ResetOutputLineFlag();
    void SetFxSend(AuxBus bus, float send);
    void FadeIn(int fadeFrames);
    int GetRemainingFadeFrames() const;
    void SetOutputLineFlag(int flag);

    unsigned char m04[0x30 - 0x04];
    SoundParam_unk m30;
};
} // namespace detail

class SoundHandle {
public:
    SoundHandle() : mSound(0) {}
    ~SoundHandle() { DetachSound(); }
    bool IsAttachedSound() const { return mSound != 0; }
    bool IsPrepared() const;
    bool IsPause() const;
    void Stop(int frames);
    void Pause(bool flag, int frames);
    void StartPrepared();
    void SetVolume(float volume, int frames);
    void SetPitch(float pitch);
    void SetLpfFreq(float lpfFreq);
    void SetBiquadFilter(int type, float value);
    void ResetOutputLineFlag(); // inferred: see BasicSound
    void SetFxSend(AuxBus bus, float send);
    int GetRemainingFadeFrames() const { return IsAttachedSound() ? mSound->GetRemainingFadeFrames() : 0; }
    detail::SoundParam_unk* GetParam_unk() const { return IsAttachedSound() ? &mSound->m30 : 0; }
    void DetachSound();

    detail::BasicSound* mSound;
};
} // namespace snd
} // namespace nw4r

// NW4R functions without a symbol name yet
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void C_MTXLookAt(Mtx m, const atn::Vector3* camPos, const atn::Vector3* camUp, const atn::Vector3* target);

// inferred: initial values of the three SoundCore fades (.sdata: 1.0f, 0.0f, 0.0f). They are left
// outside the split: MWCC places zero-initialised variables in .sbss, not in .sdata.
extern "C" float lbl_8087D930;
extern "C" float lbl_8087D934;
extern "C" float lbl_8087D938;

namespace nw4r {
namespace snd {

class SoundArchivePlayer;
class Sound3DManager;
struct StrmDataInfo;
struct WsdDataInfo;
namespace detail {
class WsdPlayer {
public:
    bool ReadWsdDataInfo(WsdDataInfo* info) const;
    long GetPlaySamplePosition() const;
};
class StrmPlayer {
public:
    bool ReadStrmDataInfo(StrmDataInfo* info) const;
    unsigned long GetPlaySamplePosition() const;
    float GetBufferRatio_unk() const; // inferred: see libs/ss_nw4r
};
class StrmSound {
public:
    void SetTrackVolume(unsigned long trackBitFlag, float volume, int fadeFrames);
};
} // namespace detail

// inferred: 0x6C bytes, list node at 0x64
class Sound3DListener {
public:
    Sound3DListener();
    void SetMatrix(const math::MTX34& mtx);
    void SetInteriorSize(float size);
    void SetMaxVolumeDistance(float distance);
    void SetUnitDistance(float distance);

    unsigned char m00[0x64];
    ut::LinkListNode mLinkNode; // 0x64
};

// inferred: only the start functions called by atn::SoundCore (no data of its own here)
class SoundStartable {
public:
    struct StartInfo;
    int detail_StartSound(SoundHandle* handle, unsigned long soundId, const StartInfo* startInfo);
    int detail_PrepareSound(SoundHandle* handle, unsigned long soundId, const StartInfo* startInfo);
};

// inferred: 0x84 bytes
class Sound3DActor : public SoundStartable {
public:
    Sound3DActor(SoundArchivePlayer& player, Sound3DManager& manager);
    void SetPosition(const math::VEC3& position);
    ~Sound3DActor();

    unsigned char m00[0x60];
    void* mUserParam_unk;   // 0x60
    atn::Vector3 mPosition; // 0x64
    unsigned char m70[0x80 - 0x70];
    int mStartOffset_unk;   // 0x80
};

} // namespace snd
} // namespace nw4r

inline void nw4r::snd::SoundHandle::Stop(int frames) {
    if (mSound != 0) {
        mSound->Stop(frames);
    }
}

inline bool nw4r::snd::SoundHandle::IsPrepared() const {
    return IsAttachedSound() && mSound->IsPrepared();
}

inline bool nw4r::snd::SoundHandle::IsPause() const {
    return IsAttachedSound() && mSound->IsPause();
}

inline void nw4r::snd::SoundHandle::Pause(bool flag, int frames) {
    if (IsAttachedSound()) {
        mSound->Pause(flag, frames);
    }
}

inline void nw4r::snd::SoundHandle::StartPrepared() {
    if (IsAttachedSound()) {
        mSound->StartPrepared();
    }
}

inline void nw4r::snd::SoundHandle::SetVolume(float volume, int frames) {
    if (IsAttachedSound()) {
        mSound->SetVolume(volume, frames);
    }
}

inline void nw4r::snd::SoundHandle::SetPitch(float pitch) {
    if (IsAttachedSound()) {
        mSound->SetPitch(pitch);
    }
}

inline void nw4r::snd::SoundHandle::SetLpfFreq(float lpfFreq) {
    if (IsAttachedSound()) {
        mSound->SetLpfFreq(lpfFreq);
    }
}

inline void nw4r::snd::SoundHandle::SetBiquadFilter(int type, float value) {
    (void)((type >= 0 && type <= 127) ||
           (nw4r::db::Panic("SoundHandle.h", 0x80, "type is out of bounds(%d)\n%d <= type <= %d not satisfied.", type, 0, 127), 0));
    if (IsAttachedSound()) {
        mSound->SetBiquadFilter(type, value);
    }
}

inline void nw4r::snd::SoundHandle::ResetOutputLineFlag() {
    if (IsAttachedSound()) {
        mSound->ResetOutputLineFlag();
    }
}

inline void nw4r::snd::SoundHandle::SetFxSend(AuxBus bus, float send) {
    if (IsAttachedSound()) {
        mSound->SetFxSend(bus, send);
    }
}

namespace atn {

// inferred: nw4r::snd::Sound3DManager (listener list at 0x0C)
struct Snd3DManager_unk {
    unsigned char m00[0x0C];
    nw4r::ut::detail::LinkListImpl mListenerList; // 0x0C

    // inferred: inline ut::LinkList<Sound3DListener, 0x64> members with the NW4R pointer assert
    static nw4r::ut::LinkListNode* GetNodeFromPointer(nw4r::snd::Sound3DListener* p) {
        if (p == 0) {
            nw4r::db::Panic("LinkList.h", 0x233, "NW4R:Pointer must not be NULL (p)");
        }
        return &p->mLinkNode;
    }
    void Insert(nw4r::ut::detail::LinkListImpl::Iterator it, nw4r::snd::Sound3DListener* p) {
        mListenerList.Insert(it, GetNodeFromPointer(p));
    }
    void PushBack(nw4r::snd::Sound3DListener* p) { Insert(mListenerList.GetEndIter(), p); }
    void Erase(nw4r::snd::Sound3DListener* p) { mListenerList.Erase(GetNodeFromPointer(p)); }
};

// inferred: sound engine of the game (lbl_8087EE90); only the members used here
struct SoundEngine_unk {
    unsigned char m00[0x04];
    int m04;                               // 0x004: the sound archive is open
    unsigned char m08[0x1D8 - 0x08];
    unsigned char mPlayer[0x364 - 0x1D8];  // 0x1D8: nw4r::snd::SoundArchivePlayer
    Snd3DManager_unk m364;                 // 0x364: nw4r::snd::Sound3DManager
    unsigned char m37C[0x394 - 0x37C];
    unsigned int mBankCount;               // 0x394: banks loaded (AudioBank_unk, 0x54 bytes)
    unsigned char m398[0x39C - 0x398];
    unsigned char* mBanks;                 // 0x39C
    unsigned char m3A0[0x408 - 0x3A0];
    int m408;                              // 0x408: SoundFx enabled
    unsigned char m40C[0x8BC - 0x40C];
    int m8BC;                              // 0x8BC: reverb (current, target)
    int m8C0;
};

} // namespace atn


namespace atn {

class Sound3dListener {
public:
    Sound3dListener();
    virtual ~Sound3dListener();
    void fn_800C7F08(const Vector3& pos, const Vector3& rot); // inferred: sets the position and rotation

    nw4r::snd::Sound3DListener mListener; // 0x04
    Vector3 mPos;                // 0x70
    Vector3 mRot;                // 0x7C
};

class Sound3dActor {
public:
    Sound3dActor(); // inline, defined in atn/Sound3dListener.cpp
    virtual ~Sound3dActor();

    nw4r::snd::Sound3DActor mActor; // 0x04
};

// inferred: fade value (0x20 bytes); names invented
struct SoundFade_unk {
    SoundFade_unk(const float& value)
        : m00(0.0f), m04(0.0f), m08(1.0f), m0C(false), m10(value), m14(value),
          m18(value), m1C(m14 - m10) {}

    void SetBegin() {
        m18 = m10;
        m00 = 0.0f;
    }
    void SetEnd() {
        m18 = m14;
        m00 = m04;
    }
    void Finish() {
        if (m08 >= 0.0f) {
            SetEnd();
        } else {
            SetBegin();
        }
        m0C = false;
    }
    void Lerp() {
        float t = m00 / m04; float d = t * (m14 - m10); float v = m10 + d; m1C = v - m18; m18 = v;
    }
    void Start(float target, float duration) {
        m00 = 0.0f;
        m04 = duration;
        m08 = 1.0f;
        m0C = true;
        m10 = m18;
        m14 = target;
        m1C = m18 - m10;
        if (m04 <= 0.0f) {
            Finish();
        }
    }
    void Update() {
        if (m0C) {
            m00 += m08;
            if (m08 >= 0.0f) {
                if (m00 < 0.0f) {
                    SetBegin();
                } else if (m00 < m04) {
                    Lerp();
                } else {
                    Finish();
                }
            } else {
                if (m00 < 0.0f) {
                    Finish();
                } else if (m00 < m04) {
                    Lerp();
                } else {
                    SetEnd();
                }
            }
        }
    }

    float m00; // time
    float m04; // duration
    float m08; // step
    bool m0C;  // active
    float m10; // start value
    float m14; // target value
    float m18; // current value
    float m1C; // last change
};

// inferred: sound category (volume group) applied to every SoundCore; names invented
struct SoundCategory_unk {
    SoundCategory_unk() : m00(-1), mMute(0), mVolume(1.0f), mPitch(1.0f), mPan(0.0f), mFxSend(1.0f) {}
    SoundCategory_unk(int id) : m00(id), mMute(0), mVolume(1.0f), mPitch(1.0f), mPan(0.0f), mFxSend(1.0f) {}

    int m00;       // inferred: index (player, category or scope)
    int mMute;     // 0x04
    float mVolume; // 0x08
    float mPitch;  // 0x0C
    float mPan;    // 0x10
    float mFxSend; // 0x14

    float GetVolume() const { return mMute == 0 ? mVolume : 0.0f; }
};

// inferred: nw4r::snd::SoundStartable::StartInfo
struct SoundStartInfo_unk {
    struct SeqSoundInfo {
        SeqSoundInfo() : seqDataAddress(0), startLocationLabel(0) {}

        void* seqDataAddress;
        const char* startLocationLabel;
    };

    SoundStartInfo_unk() : enableFlag(0) {}

    unsigned long enableFlag;
    int startOffsetType;
    int startOffset;
    int playerId;
    int playerPriority;
    int voiceOutputCount;
    SeqSoundInfo seqSoundInfo;
};

// inferred: stream / wave data info read from the playing sound (only the fields used here)
struct SoundDataInfo_unk {
    int m00;
    int mSampleRate;      // 0x04
    int m08;
    unsigned long mLength; // 0x0C
};

// inferred: nw4r::snd::detail::StrmSound / WaveSound (only the player at 0x110)
struct SoundPlayer_unk {
    unsigned char m00[4];
};
struct HandleSound_unk {
    bool ReadStrmDataInfo(SoundDataInfo_unk* info) const {
        return reinterpret_cast<const nw4r::snd::detail::StrmPlayer*>(&mPlayer)->ReadStrmDataInfo(
            reinterpret_cast<nw4r::snd::StrmDataInfo*>(info));
    }
    long GetStrmPlaySamplePosition() const {
        return reinterpret_cast<const nw4r::snd::detail::StrmPlayer*>(&mPlayer)->GetPlaySamplePosition();
    }
    float GetBufferRatio_unk() const {
        return reinterpret_cast<const nw4r::snd::detail::StrmPlayer*>(&mPlayer)->GetBufferRatio_unk();
    }
    bool ReadWaveSoundDataInfo(SoundDataInfo_unk* info) const {
        return reinterpret_cast<const nw4r::snd::detail::WsdPlayer*>(&mPlayer)->ReadWsdDataInfo(
            reinterpret_cast<nw4r::snd::WsdDataInfo*>(info));
    }
    long GetWavePlaySamplePosition() const {
        return reinterpret_cast<const nw4r::snd::detail::WsdPlayer*>(&mPlayer)->GetPlaySamplePosition();
    }
    void SetTrackVolume(unsigned long mask, float volume, int frames) {
        reinterpret_cast<nw4r::snd::detail::StrmSound*>(this)->SetTrackVolume(mask, volume, frames);
    }

    unsigned char m00[0x110];
    SoundPlayer_unk mPlayer; // 0x110
};

} // namespace atn

// inferred: nw4r::snd::StrmSoundHandle (only what atn::SoundCore uses; the sound is seen through
// atn::HandleSound_unk)
namespace nw4r {
namespace snd {
class StrmSoundHandle {
public:
    StrmSoundHandle(SoundHandle* handle);
    ~StrmSoundHandle() { DetachSound(); }
    void DetachSound();
    bool IsAttachedSound() const { return mSound != 0; }
    bool ReadStrmDataInfo(atn::SoundDataInfo_unk* info) const {
        if (mSound == 0) {
            return false;
        }
        return mSound->ReadStrmDataInfo(info);
    }
    long GetPlaySamplePosition() const {
        if (mSound == 0) {
            return -1;
        }
        return mSound->GetStrmPlaySamplePosition();
    }
    float GetBufferRatio_unk() const {
        if (mSound == 0) {
            return 0.0f;
        }
        return mSound->GetBufferRatio_unk();
    }
    void SetTrackVolume(unsigned long mask, float volume, int frames) {
        if (IsAttachedSound()) {
            mSound->SetTrackVolume(mask, volume, frames);
        }
    }

    atn::HandleSound_unk* mSound;
};
} // namespace snd
} // namespace nw4r

namespace atn {
typedef nw4r::snd::StrmSoundHandle StrmSoundHandle_unk;

} // namespace atn

// inferred: nw4r::snd::WaveSoundHandle (only what atn::SoundCore uses)
namespace nw4r {
namespace snd {
class WaveSoundHandle {
public:
    WaveSoundHandle(SoundHandle* handle);
    ~WaveSoundHandle() { DetachSound(); }
    void DetachSound();
    bool IsAttachedSound() const { return mSound != 0; }
    bool ReadWaveSoundDataInfo(atn::SoundDataInfo_unk* info) const {
        if (mSound == 0) {
            return false;
        }
        return mSound->ReadWaveSoundDataInfo(info);
    }
    long GetPlaySamplePosition() const {
        if (mSound == 0) {
            return -1;
        }
        return mSound->GetWavePlaySamplePosition();
    }

    atn::HandleSound_unk* mSound;
};
} // namespace snd
} // namespace nw4r

namespace atn {
typedef nw4r::snd::WaveSoundHandle WaveSoundHandle_unk;


class SoundCore {
public:
    SoundCore();
    virtual ~SoundCore();
    bool fn_800C8278() const;
    void fn_800C82A0(int stop);
    void fn_800C82FC();
    void fn_800C8780(const char* label);
    void fn_800C8BA4(const char* label, const Vector3* pos);
    void fn_800C9010(unsigned int id);
    void fn_800C93D4(const char* label);
    void fn_800C97F8(const char* label, const Vector3* pos);
    void fn_800C9C64(unsigned int id);
    bool fn_800CA028();
    void fn_800CA098();
    void fn_800CA0AC(float volume, int frames);
    void fn_800CA144(int frames);
    void fn_800CA1D4(int pause, unsigned long mask, int frames);
    void fn_800CA2FC(float a, float b, float c);
    void fn_800CA3D4();
    void fn_800CA42C(float pitch, int frames);
    void fn_800CA514(int frames);
    void fn_800CA5E8(float pan, int frames);
    void fn_800CA6C4(int frames);
    void fn_800CA798(int enable, float value);
    void fn_800CA7D4(int offset);
    void fn_800CA7E8(const Vector3* pos);
    const char* fn_800CA834();
    float fn_800CA844() const;
    float fn_800CA8B0();
    float fn_800CA8E0();
    int fn_800CA910();
    int fn_800CAA88();
    void fn_800CAC48(int track, float volume, int frames);
    float fn_800CACEC();

    float GetCatVolume_unk() const {
        float v = mB8;
        if (mA8 != 0) {
            v *= mA8->GetVolume();
        }
        if (mAC != 0) {
            v *= mAC->GetVolume();
        }
        if (mB0 != 0) {
            v *= mB0->GetVolume();
        }
        return v;
    }
    void StartSetup_unk(unsigned int id);
    void Start_unk(unsigned int id);
    void Prepare_unk(unsigned int id);

    nw4r::snd::SoundHandle mHandle; // 0x04
    Sound3dActor m08;
    int m90;
    int m94;
    unsigned int m98;
    unsigned long m9C;
    unsigned long mA0;
    int mA4;
    SoundCategory_unk* mA8;
    SoundCategory_unk* mAC;
    SoundCategory_unk* mB0;
    const void* mB4; // 0xB4: owner (SoundManager::m34D8, compared in fn_800CFD18)
    float mB8;
    float mBC;
    float mC0;
    float mC4;
    int mC8;
    float mCC;
    int mD0;
    int mD4;
    Vector3 mD8;
    float mE4;
    int mE8;
    SoundFade_unk mEC;
    SoundFade_unk m10C;
    SoundFade_unk m12C;
};

} // namespace atn

#endif
