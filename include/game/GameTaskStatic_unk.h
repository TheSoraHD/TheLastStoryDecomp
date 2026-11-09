#ifndef GAME_GAMETASKSTATIC_UNK_H
#define GAME_GAMETASKSTATIC_UNK_H

// Parameters of the next field start, kept between sequences (game/GameTask.cpp; the title screen fills them
// for a new game, game/NewTitleTask.cpp).

// inferred: 0xC-byte entry of sGameTaskStatic_unk: a flag to set when the field starts (m00 1: global,
// else local; -1: unused)
struct GameTaskStaticEntry_unk {
    GameTaskStaticEntry_unk() : m00(-1), m04(0), m08(0) {}
    int m00;
    int m04;
    int m08;
};

// inferred: static object of game/GameTask.cpp (.bss 0x807C8458, 0x98 bytes). The constructor is __sinit_\GameTask_cpp.
// m04 is a GameTask::Option (used instead of the given one by fn_80365708)
struct GameTaskStatic_unk {
    GameTaskStatic_unk()
        : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1), m18(0), m1C(0), m20(0), m24(0), m28(0), m2C(0),
          m30(0), m34(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28; // inferred: added to the level of the party (GameTask::vf0C)
    int m2C;
    int m30;
    int m34;
    GameTaskStaticEntry_unk mEntries[8]; // 0x38
};

extern GameTaskStatic_unk sGameTaskStatic_unk;

#endif
