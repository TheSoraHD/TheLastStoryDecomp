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

class GameSaveLoadBackgroundCommon : public atn::Task {
public:
    GameSaveLoadBackgroundCommon(atn::Task* parent); // 0x803B80F4
    virtual ~GameSaveLoadBackgroundCommon() {}

    void fn_803B8200(int arg); // inferred
    void fn_803B8270();        // inferred
    static void fn_803B83A4(char* name, int* out, int arg); // inferred: builds the save file name

    char mName[0x100];  // 0x48: inferred: strcpy destination
    unsigned char m148;
    int m14C;
    int m150;
    unsigned char m154; // inferred: active flag
};

#endif
