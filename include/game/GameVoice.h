#ifndef GAME_GAMEVOICE_H
#define GAME_GAMEVOICE_H

#include "atn/Task.h"
#include "atn/SoundRef.h"
#include "atn/LH_Binary.h"
#include "atn/LipSync_unk.h"
#include "atn/FixedArray.h"
#include "gmk/SeTable.h"
#include "math/Vec.h"

// game/GameVoice.cpp (0x803E836C-0x803EBC44): the voices of the characters, with their lip sync
// (class name from the RTTI at 0x8078C690; member names invented).

class CharaTask;

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach); // inferred: stops the sound
extern "C" atn::Vector3 lbl_807C7030;                                     // inferred: zero vector

// inferred: one voice channel (0x30 bytes; constructor 0x803E8698 and destructor 0x803E877C, called
// by __construct_array)
struct GameVoiceChannel_unk {
    GameVoiceChannel_unk();
    ~GameVoiceChannel_unk() {}
    // inferred: stops the voice and the lip sync of its character (inlined everywhere, defined in
    // game/GameVoice.cpp; name invented)
    inline void Stop_unk(int frames, int keepChara = 0);
    inline void DetachLip_unk();
    // inferred: stops the prepared voice if it is not `name` (inlined in fn_803E907C and the other
    // players, removed by the linker; name invented)
    void CheckPrepared_unk(const char* name); // inferred: the character stops using this lip sync (name invented)

    atn::SoundRef mSound;     // 0x00
    LipSyncData_unk mLipSync; // 0x04
    atn::LH_Binary mFile;     // 0x10: inferred: /database/lip/<name>.lip
    CharaTask* mChara;        // 0x18
    atn::Vector3 mPos;        // 0x1C
    int mTime;                // 0x28: inferred: frames since the start
    bool mPlaying;            // 0x2C
    bool mPrepared;           // 0x2D
};

// inferred: voice waiting `mDelay` frames to be spoken (0x28 bytes; GameVoice::fn_803EA77C)
struct GameVoiceDelayed_unk {
    GameVoiceDelayed_unk(CharaTask* chara, int kind, const char* name, float volume, float time, int a, int b, int delay)
        : mChara(chara), m04(0), mKind(kind), mName(name), mVolume(volume), mTime(time), m18(a), m1C(b), mDelay(delay), mTimer(0) {}
    GameVoiceDelayed_unk() : mChara(0), m04(0), mKind(-1), mName(0), mVolume(1.0f), mTime(0.0f), m18(1), m1C(0), mDelay(0), mTimer(0) {}

    CharaTask* mChara; // 0x00
    int m04;
    int mKind;         // 0x08: inferred: kind of voice of the character (fn_80179EBC)
    const char* mName; // 0x0C
    float mVolume;     // 0x10
    float mTime;       // 0x14
    int m18;
    int m1C;
    int mDelay;        // 0x20
    int mTimer;        // 0x24
};

// inferred: pair of values of a voice request (8 bytes)
struct GameVoicePair_unk {
    int m00;
    int m04;
};

// inferred: second part of a voice request (0x38 bytes; the copy of GameVoiceRequest_unk copies it with
// its own pointer, so it is a member of its own; name invented)
struct GameVoiceRequestInfo_unk {
    GameVoiceRequestInfo_unk() : m00(0), m04(0), m08(0), m0C(0), m30(0), mPairCount(0) {}

    int m00;                          // 0x28 of the request
    int m04;
    int m08;
    int m0C;
    atn::FixedString_unk<0x20> mName; // 0x10 (0x38)
    int m30;
    int mPairCount;                   // 0x34 (0x5C): inferred: number of mPairs of the request
};

// inferred: voice request (0xE0 bytes; constructor 0x803E8820, destructor 0x803E8908)
struct GameVoiceRequest_unk {
    // NONMATCHING (0x803E8820): r4/r5 swapped in the dead code of Reset_unk
    GameVoiceRequest_unk() : m00(0), mChannel(0) { Reset_unk(); }
    ~GameVoiceRequest_unk() {}
    // inferred: clears the request and stops its channel (name invented)
    void Reset_unk() {
        mState = 0;
        mChara = 0;
        m0C = 0;
        m10 = -1;
        m14 = 0.0f;
        m18 = 1;
        mLength = 0;
        mTimer = 0;
        if (mChannel != 0) {
            mChannel->Stop_unk(0);
            mChannel = 0;
        }
    }

    int m00;
    int mState;                       // 0x04: 1 waits for the channel, 2 cancels, 3 plays, 0 done
    CharaTask* mChara;                // 0x08
    int m0C;
    int m10;
    float m14;
    int m18;
    int mLength;                      // 0x1C: inferred: frames to play (0: until the end)
    int mTimer;                       // 0x20
    GameVoiceChannel_unk* mChannel;   // 0x24
    GameVoiceRequestInfo_unk mInfo;   // 0x28
    GameVoicePair_unk mPairs[16];     // 0x60
};

// inferred: the requests; the destructor of GameVoice destroys them with two inline levels (name invented)
struct GameVoiceRequests_unk : public atn::FixedArray_unk<GameVoiceRequest_unk, 32> {
    ~GameVoiceRequests_unk() {}
};

// inferred from __vt__9GameVoice (0x8078C690): voice manager (0x1F98 bytes; instance lbl_8087F498)
class GameVoice : public atn::Task {
public:
    static GameVoice* fn_803E836C(atn::Task* parent); // inferred: creates the instance
    GameVoice(atn::Task* parent); // inferred: inlined in fn_803E836C, removed by the linker
    virtual ~GameVoice();         // 0x803E8948
    virtual int vf0C();           // 0x803E8A24
    virtual void vf10();          // 0x803E8A50

    // inferred: plays the voice `name` of `chara`, with its lip sync
    atn::SoundRef fn_803EA09C(CharaTask* chara, const char* name, int a, int b, int c, float volume);
    // inferred: plays the voice `name` at `pos`
    atn::SoundRef fn_803E907C(const char* name, const atn::Vector3* pos, int offset, int steal, float volume);
    // inferred: says the voice `kind` of `chara` (or of a random member of the party) after `delay` frames
    void fn_803EA77C(CharaTask* chara, int kind, int delay, float volume, float time);
    void fn_803EAA7C(CharaTask* chara, int kind, float time); // inferred: shows the message of a voice
    CharaTask* fn_803EAC20(); // inferred: speaker of the HUD message (GameUI::m2658)
    void fn_803EAF60(CharaTask* chara); // inferred: removes the delayed voices of `chara` (0: all)
    int fn_803EB038(const char* name, int offset); // inferred: prepares the voice `name`
    int fn_803EB234();                   // inferred: a voice is playing or loading
    int fn_803EB2A4(const char* name);   // inferred: the voice `name` is prepared
    int fn_803EB338();                   // inferred: a voice is prepared
    int fn_803EB394(const char* name);   // inferred: stops the prepared voice `name`
    void fn_803EB4A8(int keepChara, int keepRequests); // inferred: stops the voices
    GameVoiceChannel_unk* fn_803EB668(CharaTask* chara, const char* name, int steal); // inferred: channel for a voice of `chara`
    GameVoiceChannel_unk* fn_803EB7E4(unsigned int id, int steal); // inferred: channel for the voice `id`
    int fn_803EBA54();                   // inferred: a channel is free
    void fn_803EBAC8(int frames, int keepPrepared); // inferred: stops all the voices
    void fn_803EBBCC(CharaTask* chara);  // inferred: forgets the character in the requests
    int fn_803EBC04(const char* name);   // inferred: the voice `name` exists

    void StopInvalid_unk();
    void CheckPrepared_unk(GameVoiceChannel_unk* channel, const char* name); // inferred: inlined, removed by the linker // inferred: inlined in fn_803EB7E4, removed by the linker

    GameVoiceChannel_unk mChannels[4]; // 0x48
    int m108;
    int mNumChannels;                  // 0x10C
    atn::FixedArray_unk<GameVoiceDelayed_unk, 16> mDelayed;   // 0x110
    GameVoiceRequests_unk mRequests;                          // 0x394
};

extern "C" GameVoice* lbl_8087F498; // inferred: the instance (fn_803E836C)

#endif
