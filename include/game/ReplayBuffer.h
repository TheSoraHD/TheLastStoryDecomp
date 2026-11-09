#ifndef GAME_REPLAYBUFFER_H
#define GAME_REPLAYBUFFER_H

#include "math/Vec.h"
#include "atn/FixedArray.h"
#include "game/Bullet.h"

class CharaTask;
namespace atn {
class Model;
}

// inferred: object owned by GameTask + 0x558C (0x494 bytes; code in game/ReplayBuffer.cpp). Name invented.
struct ReplayRecorder_unk {
    ReplayRecorder_unk();  // 0x8056B388
    ~ReplayRecorder_unk(); // 0x8056B394
    void fn_8056B3D4();    // inferred: records every character relative to the leader
    void fn_8056B558(CharaTask* chara, const atn::Matrix3x4& inv); // inferred: records one character
    unsigned char m000[0x460];
    unsigned char m460;
    atn::Matrix3x4 m464; // inferred: position of the leader when recording
};

// inferred: character recorded in a frame of the replay (0x8C bytes; names invented)
struct ReplayChara_unk {
    CharaTask* mChara;
    unsigned char m04[0x8C - 0x04];
};

// inferred: model recorded in a frame of the replay (0x38 bytes; names invented)
struct ReplayModel_unk {
    int m00;
    atn::Model* mModel;
    unsigned char m08[0x38 - 0x08];
};

// inferred: one frame of the replay (0x1808 bytes; names invented)
struct ReplayFrame_unk {
    ReplayChara_unk mCharas[15];  // 0x000
    ReplayModel_unk mModels[60];  // 0x834
    signed char mNumCharas;       // 0x1554
    signed char mNumModels;       // 0x1555
    unsigned char m1556[0x1808 - 0x1556];
};

// inferred: event of the replay with the frame it happened (0x48 bytes; a negative frame marks a free slot)
struct ReplayEventEntry_unk {
    int mFrame;
    ReplayEvent_unk mEvent;
};

// inferred: object of GameTask + 0x5590 (constructor 0x8056BF8C and destructor 0x8056C398 in
// game/ReplayBuffer.cpp; other members in game/GameSystem_unk.cpp)
struct GameTaskData_unk {
    GameTaskData_unk(); // 0x8056BF8C
    ~GameTaskData_unk();
    void fn_8056F2F8(int frames); // inferred: shows the frame `frames` / 2 frames back
    void fn_805706C0(const ReplayEvent_unk* ev); // inferred: records an event (game/Bullet.cpp)
    unsigned char m00000[0x04];
    int m00004;
    ReplayFrame_unk mFrames[45]; // 0x00008: inferred: ring of the recorded frames
    ReplayFrame_unk mLive;       // 0x43970: inferred: frame shown while m5BDB4 is set
    int m45178;                  // inferred: index of the newest frame in mFrames
    int m4517C;                  // inferred: number of frames recorded (45: full)
    unsigned char m45180[0x52994 - 0x45180];
    ReplayEventEntry_unk mEvents[256]; // 0x52994: inferred: ring of the events
    unsigned char m57194[0x5B994 - 0x57194];
    atn::FixedArray_unk<ReplayEventEntry_unk*, 256> mNewEvents; // 0x5B994: inferred: events of this frame
    int mNextEvent; // 0x5BD98: inferred: first slot of mEvents to try
    unsigned char m5BD9C[0x5BDA4 - 0x5BD9C];
    int m5BDA4;
    int m5BDA8;
    int m5BDAC;
    int m5BDB0;
    int m5BDB4;
    unsigned char m5BDB8[0x5BDBC - 0x5BDB8]; // inferred: size from the new of GameTask::fn_8036CA60
};

#endif
