#ifndef GAME_GAMESAVEDATA_H
#define GAME_GAMESAVEDATA_H

// inferred: serialized game state (game/GameSaveData_unk.cpp, 0x803B9804-0x803BEBAC; 0x134A8 bytes).
// MP::SaveData_unk (include/MP/SaveLoad.h) is another view of the same buffer. Names invented.
#include "game/CharaStatus.h"
#include "game/GimmickCommon.h"

// inferred: character, enemy or player of the map saved with its state (8 bytes; type 1: npc,
// 2: enemy, 3: player)
struct GameSaveChara_unk {
    int mId;
    unsigned char mType;  // 0x4
    unsigned char mFlag;  // 0x5: inferred: bit 20 of the character flags (0x12A8)
    unsigned char m6[2];
};

// inferred: game flags of the save data (at +0x04 of GameSaveData_unk): values and work values of the
// flag manager (lbl_8087F430), the route spots and the characters of the map
struct GameSaveFlags_unk {
    void fn_803B9804(); // inferred: collects the flags
    void fn_803B9C98(); // inferred: restores the flags
    void fn_803B9D78(); // inferred: restores the work values and the routes

    int mState;                      // 0x0000: inferred: copy of the field 0x10D0
    int mFlags[0x1000];              // 0x0004
    int mWork[0x100];                // 0x4004
    unsigned short mSpots[0x80];     // 0x4404: inferred: route spots not reached (fn_803ACD50)
    unsigned short mSpotCount;       // 0x4504
    unsigned char m4506[2];
    GameSaveChara_unk mCharas[0x100];    // 0x4508
    unsigned short mCharaCount;      // 0x4D08
    unsigned char m4D0A[2];
    unsigned char mMap;              // 0x4D0C
    unsigned char mArea;             // 0x4D0D
    unsigned short mPoint;           // 0x4D0E
    int m4D10;
    int mLeader;                     // 0x4D14: inferred: character id of the first player
    unsigned char m4D18[0x200];      // inferred: copy of 0x154 of the route tables
};

// inferred: status of the seven party members (0xFC4 bytes; its constructor 0x803BAA90 is called by
// game/GameTask)
struct GameSaveParty_unk {
    GameSaveParty_unk(); // 0x803BAA90
    void fn_803BAD6C(); // inferred: copies the saved status to the players

    CharaStatusBase mStatus[7];
    unsigned int mMask; // 0xFC0: inferred: bit i set when member i is saved
};

// inferred: gimmick saved with its state (0x18 bytes)
struct GameSaveGimmick_unk {
    int mId;                // 0x00: GimmickCommon::mId
    unsigned short m04;     // 0x04: GimmickCommon::m4C
    unsigned char mFlags;   // 0x06: bit 0: used
    unsigned char mState;   // 0x07
    unsigned char m08[0x18 - 0x08];
};

// inferred: blocks restored by their own function; contents not known yet
struct GameSaveBlock4F5C_unk {
    void fn_803BA620(); // inferred: restores the positions of the party
    unsigned char m000[0x440];
};
struct GameSaveEnemies_unk {
    void fn_803BB0E0(); // inferred: restores the enemies
    unsigned char m000[0xB00];
};
struct GameSaveInventory_unk {
    void fn_803BC2D4(); // inferred: restores the inventory
    unsigned char m0000[0x94C8];
    int m94C8; // inferred: shown in the slot header (GameSaveSlot_unk::m70)
    unsigned char m94CC[0x9548 - 0x94CC];
};
struct GameSaveSettings_unk {
    void fn_803BD79C(); // inferred: restores the settings
    unsigned char m00[0xFC];
};

struct GameSaveData_unk {
    void fn_803BDEE4(); // inferred: collects the current game state
    void fn_803BE590(); // inferred: restores the party, the inventory and the flags
    void fn_803BE670(); // inferred: restores the work values
    void fn_803BE6B8(); // inferred: restores the characters and the gimmicks
    // inferred: index of the saved state of `gimmick` (0x200 if not found; name invented)
    int FindGimmick_unk(GimmickCommon* gimmick) {
        int i;
        for (i = 0; i < 0x200; i++) {
            if ((mGimmicks[i].mFlags & 1) != 1) {
                break;
            }
            if (mGimmicks[i].mId == gimmick->mId && mGimmicks[i].m04 == gimmick->m4C) {
                break;
            }
        }
        return i;
    }

    int m0000;
    GameSaveFlags_unk mFlags;              // 0x0004
    unsigned char m4F1C[0x4F5C - 0x4F1C];
    GameSaveBlock4F5C_unk m4F5C;           // 0x4F5C
    GameSaveParty_unk mParty;        // 0x539C
    GameSaveEnemies_unk mEnemies;          // 0x6360
    GameSaveGimmick_unk mGimmicks[0x200];  // 0x6E60
    GameSaveInventory_unk mInventory;      // 0x9E60
    GameSaveSettings_unk mSettings;        // 0x133A8
    unsigned int mChecksum;            // 0x134A4
};

#endif
