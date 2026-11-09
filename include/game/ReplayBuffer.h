#ifndef GAME_REPLAYBUFFER_H
#define GAME_REPLAYBUFFER_H

#include "math/Vec.h"

class CharaTask;

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

// inferred: object of GameTask + 0x5590 (constructor 0x8056BF8C and destructor 0x8056C398 in
// game/ReplayBuffer.cpp; other members in game/GameSystem_unk.cpp)
struct GameTaskData_unk {
    GameTaskData_unk(); // 0x8056BF8C
    ~GameTaskData_unk();
    unsigned char m00000[0x04];
    int m00004;
    unsigned char m00008[0x4517C - 0x8];
    int m4517C; // inferred: state (45 = ...)
    unsigned char m45180[0x5BDA8 - 0x45180];
    int m5BDA8;
    int m5BDAC;
    int m5BDB0;
    unsigned char m5BDB4[0x5BDBC - 0x5BDB4]; // inferred: size from the new of GameTask::fn_8036CA60
};

#endif
