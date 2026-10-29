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
    void fn_80709AB0_unk();
    void SetVolume(float volume, int frames);
    void fn_8070AD40_unk(float value);
    void fn_8070AD50_unk(float value);
    void fn_8070AD60_unk(int type, float value);
    void fn_8070AD80_unk();
    void fn_8070AD90_unk(int bus, float value);
    detail::SoundParam_unk* GetParam_unk() const { return IsAttachedSound() ? &mSound->m30 : 0; }
    void DetachSound();

    detail::BasicSound* mSound;
};
} // namespace snd
} // namespace nw4r

// NW4R functions without a symbol name yet
extern "C" void fn_80715B20(void* self);                    // inferred: nw4r::snd::Sound3DListener::Sound3DListener
extern "C" void fn_80715BA0(void* self, const Mtx m);       // inferred: nw4r::snd::Sound3DListener::SetMatrix
extern "C" void fn_80714930(void* self, void* player, void* manager); // inferred: nw4r::snd::Sound3DActor ctor
extern "C" void fn_80709AD0(void* sound, int frames);       // inferred: nw4r::snd::detail::BasicSound::Stop
extern "C" void fn_80709AB0(void* sound);
extern "C" bool fn_8070A030(void* sound);                   // inferred: nw4r::snd::detail::BasicSound::IsPause
extern "C" void fn_80709CC0(void* sound, bool flag, int frames); // inferred: nw4r::snd::detail::BasicSound::Pause
extern "C" void fn_8070ACB0(void* sound, float volume, int frames); // inferred: nw4r::snd::detail::BasicSound::SetVolume
extern "C" void fn_8070AD40(void* sound, float value);
extern "C" void fn_8070AD50(void* sound, float value);
extern "C" void fn_8070AD60(void* sound, float value, int type);
extern "C" void fn_8070AD80(void* sound);
extern "C" void fn_8070AD90(void* sound, int bus, float value);
extern "C" void fn_80714C50(void* actor, const atn::Vector3* pos); // inferred: nw4r::snd::Sound3DActor::SetPosition
extern "C" int fn_80718470(void* actor, void* handle, unsigned int id, void* info); // inferred: nw4r::snd::SoundActor start
extern "C" int fn_807184D0(void* actor, void* handle, unsigned int id, void* info); // inferred: nw4r::snd::SoundActor prepare
extern "C" void fn_8071D0E0(void* strmHandle, void* handle);   // inferred: nw4r::snd::StrmSoundHandle::StrmSoundHandle
extern "C" void fn_8071D1E0(void* strmHandle);                 // inferred: nw4r::snd::StrmSoundHandle::DetachSound
extern "C" bool fn_8071A680(void* player, void* info);         // inferred: StrmPlayer: reads the stream data info
extern "C" int fn_8071A720(void* player);                      // inferred: StrmPlayer: play position (samples)
extern "C" float fn_8071A800(void* player);
extern "C" void fn_8071CD20(void* sound, unsigned long mask, float volume, int frames); // inferred: StrmSound track volume
extern "C" void fn_80721890(void* waveHandle, void* handle);   // inferred: nw4r::snd::WaveSoundHandle::WaveSoundHandle
extern "C" void fn_80721990(void* waveHandle);                 // inferred: nw4r::snd::WaveSoundHandle::DetachSound
extern "C" bool fn_80722440(void* player, void* info);         // inferred: WsdPlayer: reads the wave data info
extern "C" int fn_80722500(void* player);                      // inferred: WsdPlayer: play position (samples)
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void C_MTXLookAt(Mtx m, const atn::Vector3* camPos, const atn::Vector3* camUp, const atn::Vector3* target);

// inferred: initial values of the three SoundCore fades (.sdata: 1.0f, 0.0f, 0.0f). They are left
// outside the split: MWCC places zero-initialised variables in .sbss, not in .sdata.
extern "C" float lbl_8087D930;
extern "C" float lbl_8087D934;
extern "C" float lbl_8087D938;

namespace nw4r {
namespace snd {

// inferred: 0x6C bytes, list node at 0x64. The constructor has no symbol name yet, so it calls
// fn_80715B20 directly.
class Sound3DListener {
public:
    Sound3DListener() { fn_80715B20(this); }

    unsigned char m00[0x64];
    ut::LinkListNode mLinkNode; // 0x64
};

// inferred: 0x84 bytes. The constructor has no symbol name yet (fn_80714930).
class Sound3DActor {
public:
    Sound3DActor(void* player, void* manager) { fn_80714930(this, player, manager); }
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
        fn_80709AD0(mSound, frames);
    }
}

inline bool nw4r::snd::SoundHandle::IsPrepared() const {
    return IsAttachedSound() && mSound->IsPrepared();
}

inline bool nw4r::snd::SoundHandle::IsPause() const {
    return IsAttachedSound() && fn_8070A030(mSound);
}

inline void nw4r::snd::SoundHandle::Pause(bool flag, int frames) {
    if (IsAttachedSound()) {
        fn_80709CC0(mSound, flag, frames);
    }
}

inline void nw4r::snd::SoundHandle::fn_80709AB0_unk() {
    if (IsAttachedSound()) {
        fn_80709AB0(mSound);
    }
}

inline void nw4r::snd::SoundHandle::SetVolume(float volume, int frames) {
    if (IsAttachedSound()) {
        fn_8070ACB0(mSound, volume, frames);
    }
}

inline void nw4r::snd::SoundHandle::fn_8070AD40_unk(float value) {
    if (IsAttachedSound()) {
        fn_8070AD40(mSound, value);
    }
}

inline void nw4r::snd::SoundHandle::fn_8070AD50_unk(float value) {
    if (IsAttachedSound()) {
        fn_8070AD50(mSound, value);
    }
}

inline void nw4r::snd::SoundHandle::fn_8070AD60_unk(int type, float value) {
    (void)((type >= 0 && type <= 127) ||
           (nw4r::db::Panic("SoundHandle.h", 0x80, "type is out of bounds(%d)\n%d <= type <= %d not satisfied.", type, 0, 127), 0));
    if (IsAttachedSound()) {
        fn_8070AD60(mSound, value, type);
    }
}

inline void nw4r::snd::SoundHandle::fn_8070AD80_unk() {
    if (IsAttachedSound()) {
        fn_8070AD80(mSound);
    }
}

inline void nw4r::snd::SoundHandle::fn_8070AD90_unk(int bus, float value) {
    if (IsAttachedSound()) {
        fn_8070AD90(mSound, bus, value);
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
    unsigned char m00[0x1D8];
    unsigned char mPlayer[0x364 - 0x1D8];  // 0x1D8: nw4r::snd::SoundArchivePlayer
    Snd3DManager_unk m364;                 // 0x364: nw4r::snd::Sound3DManager
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
    int m00;
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
    bool ReadStrmDataInfo(SoundDataInfo_unk* info) const { return fn_8071A680((void*)&mPlayer, info); }
    long GetStrmPlaySamplePosition() const { return fn_8071A720((void*)&mPlayer); }
    float fn_8071A800_unk() const { return fn_8071A800((void*)&mPlayer); }
    bool ReadWaveSoundDataInfo(SoundDataInfo_unk* info) const { return fn_80722440((void*)&mPlayer, info); }
    long GetWavePlaySamplePosition() const { return fn_80722500((void*)&mPlayer); }
    void SetTrackVolume(unsigned long mask, float volume, int frames) { fn_8071CD20(this, mask, volume, frames); }

    unsigned char m00[0x110];
    SoundPlayer_unk mPlayer; // 0x110
};

// inferred: nw4r::snd::StrmSoundHandle / WaveSoundHandle
struct StrmSoundHandle_unk {
    StrmSoundHandle_unk(nw4r::snd::SoundHandle* h) { fn_8071D0E0(this, h); }
    ~StrmSoundHandle_unk() { fn_8071D1E0(this); }
    bool IsAttachedSound() const { return mSound != 0; }
    bool ReadStrmDataInfo(SoundDataInfo_unk* info) const {
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
    float fn_8071A800_unk() const {
        if (mSound == 0) {
            return 0.0f;
        }
        return mSound->fn_8071A800_unk();
    }
    void SetTrackVolume(unsigned long mask, float volume, int frames) {
        if (IsAttachedSound()) {
            mSound->SetTrackVolume(mask, volume, frames);
        }
    }

    HandleSound_unk* mSound;
};
struct WaveSoundHandle_unk {
    WaveSoundHandle_unk(nw4r::snd::SoundHandle* h) { fn_80721890(this, h); }
    ~WaveSoundHandle_unk() { fn_80721990(this); }
    bool IsAttachedSound() const { return mSound != 0; }
    bool ReadWaveSoundDataInfo(SoundDataInfo_unk* info) const {
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

    HandleSound_unk* mSound;
};

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
    int mB4;
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
