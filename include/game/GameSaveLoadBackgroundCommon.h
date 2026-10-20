#ifndef GAME_GAMESAVELOADBACKGROUNDCOMMON_H
#define GAME_GAMESAVELOADBACKGROUNDCOMMON_H

#include "atn/Task.h"

// inferred: save/load manager (class unknown), global instance at 0x8087EF68
struct SaveLoadMgr_unk {
    unsigned char m000[0x168];
    int m168; // inferred: error/busy flag
};
extern "C" SaveLoadMgr_unk* lbl_8087EF68;
extern "C" int fn_800A4228(SaveLoadMgr_unk* mgr, void* work);
extern "C" void fn_800A48B8(SaveLoadMgr_unk* mgr, const char* name, void* data, int size, int arg, void* work);
extern "C" void fn_800A49C4(SaveLoadMgr_unk* mgr, const char* name, int arg1, int arg2, int arg3, void* work);

// inferred: calendar time filled by fn_8004B20C (same size as OSCalendarTime, 0x28)
struct SaveTime_unk {
    int m00[10];
};
extern "C" void fn_8004B20C(SaveTime_unk* time); // inferred: current time

// inferred: serialized game state (game/GameSaveData_unk.cpp); name invented
struct GameSaveData_unk {
    void fn_803BDEE4(); // inferred: collects the current game state
    unsigned char m00[0x134A8];
};

// inferred: header of a save slot (0xB8 bytes, game/GameSaveData_unk.cpp); name invented
struct GameSaveSlot_unk {
    GameSaveSlot_unk(); // 0x803BE8B4
    void fn_803BE8C0(GameSaveData_unk* data, SaveTime_unk* time); // inferred: fills the header
    int fn_803BEAB4();                                            // inferred: nonzero if the slot is used

    unsigned char m00[0x08];
    int m08; // inferred: minutes
    int m0C; // inferred: hours
    int m10; // inferred: day
    int m14; // inferred: month (0-based)
    int m18; // inferred: year
    int m1C;
    unsigned char m20[0x30 - 0x20];
    unsigned char m30; // inferred: map
    unsigned char m31;
    unsigned short m32;
    unsigned char m34[0xB8 - 0x34];
};

// inferred: contents of a save file (header + game state, 0x13560 bytes)
struct GameSaveFile_unk {
    unsigned char mHeader[0xB8]; // inferred: copy of a GameSaveSlot_unk
    GameSaveData_unk mData;
};

class GameSaveLoadBackgroundCommon : public atn::Task {
public:
    GameSaveLoadBackgroundCommon(atn::Task* parent); // 0x803B80F4
    virtual ~GameSaveLoadBackgroundCommon() {}

    void fn_803B8160(int slot); // inferred: writes an empty save file in `slot`
    void fn_803B8200(int arg); // inferred
    void fn_803B8270();        // inferred
    static void fn_803B83A4(char* name, int* out, int arg); // inferred: builds the save file name
    int fn_803B8144();                             // inferred: error code of the save/load manager (does not use this)
    static void fn_803B8454(char* name, int* size); // inferred: builds the file name and its size

    char mName[0x100];  // 0x48: inferred: strcpy destination
    unsigned char m148;
    int m14C;
    int m150;
    unsigned char m154; // inferred: active flag
};

// inferred from __vt__18GameSaveLoadCommon (0x8078B588): list of the 17 save slots, read from the NAND
class GameSaveLoadCommon : public atn::Task {
public:
    GameSaveLoadCommon(atn::Task* parent); // 0x803B82C0
    virtual ~GameSaveLoadCommon();         // 0x803B834C

    void fn_803B85EC();                         // inferred: reads every slot header synchronously
    void fn_803B8758(int slot);                 // inferred: starts reading `slot` (-1: all)
    int fn_803B87A8();                          // inferred: read state machine; 0 when done
    int fn_803B8AC4();                          // inferred: nonzero if the home directory exists
    void fn_803B8B08(GameSaveSlot_unk* slots);  // inferred: draws the slot list

    int m48; // inferred: selected slot
    GameSaveSlot_unk mSlots[17]; // 0x4C
    int mC84; // inferred: state of fn_803B87A8
    int mC88; // inferred: slot being read
    int mC8C; // inferred: last slot to read
    unsigned char mC90[0xCA0 - 0xC90];
    unsigned char mCA0[0xC0]; // inferred: read buffer of a slot header
    int mD60; // inferred: set when every slot was read
    int mD64;
};

#endif
