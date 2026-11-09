#ifndef GAME_REPLAYBUFFER_H
#define GAME_REPLAYBUFFER_H

#include "math/Vec.h"
#include "atn/FixedArray.h"
#include "game/Bullet.h"
#include "atn/EffectManagerCallback.h"

class CharaTask;
namespace atn {
class Model;
}

// inferred: character recorded in a frame of the replay (0x8C bytes; names invented)
struct ReplayChara_unk {
    CharaTask* mChara;
    signed char mMotion;  // 0x04: inferred: motion (negative: none)
    signed char mNode;    // 0x05: inferred: value of Model::m50
    signed char m06;      // 0x06: inferred: value of Model::m34C
    unsigned char m07;
    short mMtx[3][4];     // 0x08: inferred: model matrix (half floats)
    unsigned char m20[0x88 - 0x20]; // inferred: node data passed to fn_8056A9E4
    short mAngle;         // 0x88: inferred: angle of the upper body (half float)
    unsigned char m8A[0x8C - 0x8A];
};

// inferred: object owned by GameTask + 0x558C (0x494 bytes; code in game/ReplayBuffer.cpp). Name invented.
struct ReplayRecorder_unk {
    ReplayRecorder_unk();  // 0x8056B388
    ~ReplayRecorder_unk(); // 0x8056B394
    void fn_8056B3D4();    // inferred: records every character relative to the leader
    void fn_8056B558(CharaTask* chara, const atn::Matrix3x4& inv); // inferred: records one character
    void fn_8056BD34(); // inferred: puts every recorded character back
    ReplayChara_unk mCharas[8]; // 0x000
    signed char m460;           // inferred: number of characters in mCharas
    atn::Matrix3x4 m464; // inferred: position of the leader when recording
};

// inferred: model recorded in a frame of the replay (0x38 bytes; names invented)
struct ReplayModel_unk {
    int m00;
    atn::Model* mModel;
    unsigned char m08[0x38 - 0x08];
};

// inferred: one frame of the replay (0x1808 bytes; names invented)
struct ReplayFrame_unk {
    ReplayFrame_unk() : mNumCharas(0), mNumModels(0) {}
    ReplayChara_unk mCharas[15];  // 0x000
    ReplayModel_unk mModels[60];  // 0x834
    signed char mNumCharas;       // 0x1554
    signed char mNumModels;       // 0x1555
    unsigned char m1556[0x1808 - 0x1556];
};

// inferred: event of the replay with the frame it happened (0x48 bytes; a negative frame marks a free slot)
struct ReplayEventEntry_unk {
    ReplayEventEntry_unk() : mFrame(-1) {}
    int mFrame;
    ReplayEvent_unk mEvent;
};

// inferred: effect event of the replay with its frame (0x68 bytes; a negative frame marks a free slot)
struct ReplayEffectEntry_unk {
    ReplayEffectEntry_unk() : mFrame(-1) {}
    int mFrame;
    EffectEvent_unk mEvent;
};

// inferred: object of GameTask + 0x5590 (constructor 0x8056BF8C and destructor 0x8056C398 in
// game/ReplayBuffer.cpp; other members in game/GameSystem_unk.cpp)
class ReplayBuffer : public atn::EffectManagerCallback {
public:
    ReplayBuffer(); // 0x8056BF8C
    ~ReplayBuffer();
    virtual void vf08(EffectEvent_unk* event); // 0x8056F400: inferred: records an effect event
    void fn_8056F2F8(int frames); // inferred: shows the frame `frames` / 2 frames back
    void fn_805706C0(const ReplayEvent_unk* ev); // inferred: records an event (game/Bullet.cpp)
    void fn_8056FC24(int arg);
    void fn_805704AC(); // inferred: replays the effects and events since the frame m5BDAC
    void fn_8056D8C8(int frame, int restore, int replay); // inferred: seeks the replay to the frame
    void fn_8056DACC(int frame);
    void fn_8056E918(int frame);
    void fn_80570254(int frame); // inferred: rebuilds the effects and events up to the frame
    void fn_8057080C(); // inferred: frees the bullet events that ended more than 90 frames ago
    int m00004; // inferred: current frame
    ReplayFrame_unk mFrames[45]; // 0x00008: inferred: ring of the recorded frames
    ReplayFrame_unk mLive;       // 0x43970: inferred: frame shown while m5BDB4 is set
    int m45178;                  // inferred: index of the newest frame in mFrames
    int m4517C;                  // inferred: number of frames recorded (45: full)
    int m45180;
    ReplayEffectEntry_unk mEffectRing[512]; // 0x45184: inferred: ring of the effect events
    atn::FixedArray_unk<ReplayEffectEntry_unk*, 512> mEffects; // 0x52184: inferred: effect events recorded
    int mNextEffect; // 0x52988: inferred: first slot of mEffectRing to try
    int m5298C;
    int m52990; // inferred: first frame of the effects to replay
    ReplayEventEntry_unk mEvents[512]; // 0x52994: inferred: events (fn_805706C0 only uses the first 256)
    atn::FixedArray_unk<ReplayEventEntry_unk*, 256> mNewEvents; // 0x5B994: inferred: events of this frame
    int mNextEvent; // 0x5BD98: inferred: first slot of mEvents to try
    int m5BD9C;
    int m5BDA0; // inferred: first frame of the events to replay
    int m5BDA4; // inferred: set while the events are replayed
    int m5BDA8;
    int m5BDAC;
    int m5BDB0;
    int m5BDB4;
    float m5BDB8; // inferred: time accumulated for the recording (size from the new of GameTask::fn_8036CA60)
};

#endif
