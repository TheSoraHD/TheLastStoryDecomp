#ifndef ATN_SOUNDMANAGER_H
#define ATN_SOUNDMANAGER_H

#include "atn/SoundCore.h"
#include "game/ResHandle.h"
#include "atn/FixedArray.h"

// Forward declarations instead of <boost/function.hpp>: the full header brings in <string.h>, whose
// intrinsic strlen changes the code of the callers that declare strlen themselves (atn/Bgm, atn/Se_unk).
// The default allocator argument is added by boost/function/function_base.hpp.
namespace std {
template <class T> class allocator;
}
namespace boost {
template <typename Signature, typename Allocator> class function;
}

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long size);

namespace atn {

// inferred: fixed size string (strings of the debug sound list)
template <int N>
struct SoundString_unk {
    SoundString_unk() { mText[0] = '\0'; }
    SoundString_unk(const char* text) {
        mText[0] = '\0';
        *this = text;
    }
    SoundString_unk& operator=(const char* text) {
        if (text != mText) {
            memcpy(mText, text, strlen(text) + 1);
        }
        return *this;
    }
    SoundString_unk& operator=(const SoundString_unk& other) { return *this = other.mText; }

    char mText[N];
};

// inferred: line of the debug sound list (0x90 bytes): a sound started recently (DupLimitFrame)
struct SoundLogEntry_unk {
    SoundLogEntry_unk() {}
    SoundLogEntry_unk(const char* text, unsigned int id, int age, float volume, SoundCore* sound)
        : mText(text), mId(id), mAge(age), mVolume(volume), mSound(sound) {}
    ~SoundLogEntry_unk() {}

    SoundString_unk<0x80> mText; // 0x00
    unsigned int mId;            // 0x80: sound id (SoundCore::m98)
    int mAge;                    // 0x84: frames since it started
    float mVolume;               // 0x88
    SoundCore* mSound;           // 0x8C
};

// inferred: name in the RTTI (atn::SoundManager); singleton lbl_8087EFE8 (fn_800CCBA4), 0x46E8 bytes
class SoundManager {
public:
    typedef boost::function<void(SoundCore&), std::allocator<void> > Func;

    SoundManager();
    virtual ~SoundManager();

    void fn_800CDF84();                                  // inferred: reset (stops every sound)
    void fn_800CE368();                                  // inferred: restores the default parameters
    void fn_800CE6E0();                                  // inferred: per frame update
    void fn_800CECF0();                                  // inferred: debug display
    void fn_800CF45C(int frames, int onlyUnused);         // inferred: stops every sound
    void fn_800CF680(int category, int frames, int onlyUnused);
    void fn_800CF7DC(int pause, unsigned int mask, int frames); // inferred: pauses every sound
    void fn_800CFA28(int player, int pause, unsigned int mask, int frames);
    void fn_800CFBA0(int category, int pause, unsigned int mask, int frames);
    void fn_800CFD18(const void* owner, int frames);              // inferred: stops the sounds of an owner
    SoundCategory_unk* fn_800CFDA0(int player);
    SoundCategory_unk* fn_800CFDC8(int category);
    void fn_800CFDF0(const SoundCategory_unk& param);     // inferred: category parameters
    void fn_800CFFB8(const SoundCategory_unk& param);     // inferred: scope parameters
    void fn_800D0240(int player, float volume, int frames);
    void fn_800D03AC(int category, float volume, int frames);
    void fn_800D0518(int scope, float volume, int frames);
    void fn_800D0684(int player, float fxSend);
    int fn_800D089C(int category, int onlyIdle);           // inferred: number of sounds playing
    void fn_800D0974(int category, int frames);
    SoundCore* fn_800D0AB0(unsigned int id);              // inferred: free sound for a new start
    int fn_800D0C1C(SoundCore* sound, float volume);      // inferred: duplicate check (DupLimitFrame)
    void fn_800D0DB0(Func func, int player);
    void fn_800D0F34(Func func, int category);
    void fn_800D10B8(Func func, int scope);
    void fn_800D123C(Func func, int player, int category, int scope);
    void fn_800D1430();                                  // inferred: debug menu
    void fn_800D19FC(SoundCore* sound, float volume);     // inferred: adds a line to the debug list

    // inferred: setters of the parameter tables (defined in atn/SoundManager.cpp; only fn_800D0240 and
    // fn_800D0684 are also emitted out of line)
    inline void SetPlayerMute_unk(int player, int mute, int frames);
    inline void SetPlayerPitch_unk(int player, float pitch, int frames);
    inline void SetPlayerLpf_unk(int player, float lpf, int frames);
    inline void SetCategoryMute_unk(int category, int mute, int frames);
    inline void SetCategoryPitch_unk(int category, float pitch, int frames);
    inline void SetCategoryLpf_unk(int category, float lpf, int frames);
    inline void SetCategoryFxSend_unk(int category, float fxSend);
    inline void SetScopeMute_unk(int scope, int mute, int frames);
    inline void SetScopePitch_unk(int scope, float pitch, int frames);
    inline void SetScopeLpf_unk(int scope, float lpf, int frames);
    inline void SetScopeFxSend_unk(int scope, float fxSend);
    inline void SetPlayerParam_unk(const SoundCategory_unk& param);

    SoundCategory_unk* GetPlayer_unk(int player) {
        if (player < 0 || 8 <= player) {
            return 0;
        } else {
            return &mPlayers[player];
        }
    }
    SoundCategory_unk* GetCategory_unk(int category) {
        if (category < 0 || 32 <= category) {
            return 0;
        } else {
            return &mCategories[category];
        }
    }
    SoundCategory_unk* GetScope_unk(int scope) {
        if (scope < 0 || 16 <= scope) {
            return 0;
        } else {
            return &mScopes[scope];
        }
    }

    SoundCore mSounds[32];                 // 0x0004
    Sound3dListener mListener;             // 0x2984
    int mNext;                             // 0x2A0C: last sound allocated (fn_800D0AB0)
    int mOutputMode;                       // 0x2A10: OutputMode
    float mMasterVolume;                   // 0x2A14: MasterVolume
    SoundCategory_unk mPlayers[8];         // 0x2A18: PlayerParam
    SoundCategory_unk mDefPlayers[8];      // 0x2AD8: copies restored by fn_800CE368
    SoundCategory_unk mCategories[32];     // 0x2B98: CategoryParam
    SoundCategory_unk mDefCategories[32];  // 0x2E98
    SoundCategory_unk mScopes[16];         // 0x3198: ScopeParam
    SoundCategory_unk mDefScopes[16];      // 0x3318
    int mEnabled;                          // 0x3498: Enabled
    int mStreamEnabled;                    // 0x349C: StreamEnabled
    int m3dEnabled;                        // 0x34A0: 3DSound/Enabled
    float mMaxVolumeDistance;              // 0x34A4
    float mUnitDistance;                   // 0x34A8
    float mInteriorSize;                   // 0x34AC
    int mFxEnabled;                        // 0x34B0: SoundFx/Enabled
    int mFx2dEnabled;                      // 0x34B4: SoundFx/2D Enabled
    int m34B8;
    int mCompressor;                       // 0x34BC: Compressor
    int mDupLimitFrame;                    // 0x34C0: DupLimitFrame
    float mMinimumVolume;                  // 0x34C4: MinimumVolume
    int m34C8;                             // 0x34C8: duplicate check enabled (game/AttractHatePoint)
    float mRandomPitch;                    // 0x34CC: RandomPitch
    int m34D0;                             // 0x34D0: category of the next sound
    int m34D4;                             // 0x34D4: scope of the next sound
    const void* m34D8;                     // 0x34D8: owner of the next sound (a CharaTask)
    int mDebugLevel;                       // 0x34DC: DebugLevel
    FixedArray_unk<SoundLogEntry_unk, 32> mLog; // 0x34E0
    ResHandle mMenu;                       // 0x46E4: debug menu "Sound"
};

} // namespace atn

extern "C" atn::SoundManager* lbl_8087EFE8;
extern "C" void fn_800CCBA4(); // inferred: creates the SoundManager singleton

#endif
