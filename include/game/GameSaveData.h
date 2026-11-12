#ifndef GAME_GAMESAVEDATA_H
#define GAME_GAMESAVEDATA_H

// inferred: serialized game state (game/GameSaveData_unk.cpp, 0x803B9804-0x803BEBAC; 0x134A8 bytes).
// MP::SaveData_unk (include/MP/SaveLoad.h) is another view of the same buffer. Names invented.
#include "game/CharaStatus.h"

#include "game/GimmickCommon.h"
#include "game/Inventory.h"
#include "MP/SaveLoad.h"
#include "game/SetConfig.h"

// inferred: character, enemy or player of the map saved with its state (8 bytes; type 1: npc,
// 2: enemy, 3: player)
struct GameSaveChara_unk {
    int mId;
    unsigned char mType;  // 0x4
    unsigned char mFlag;  // 0x5: inferred: bit 20 of the character flags (0x12A8)
    unsigned char m6[2];
};

// inferred: 8-byte entry copied from 0x154 of the route tables
struct GameSaveRoute_unk {
    unsigned int m00; // inferred: name hash of the map part (as ScriptPartsFlag_unk, unsigned compares)
    unsigned int m04; // inferred: draw flags
};

// inferred: game flags of the save data (at +0x04 of GameSaveData_unk): values and work values of the
// flag manager (lbl_8087F430), the route spots and the characters of the map
struct GameSaveFlags_unk {
    inline GameSaveFlags_unk();
    void fn_803B9804(); // inferred: collects the flags
    void fn_803B9C98(); // inferred: restores the flags
    void fn_803B9D78(); // inferred: restores the work values and the routes
    void fn_803BA120(int mode); // inferred: reset for a new game (mode 2: from a cleared save)

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
    unsigned int mLeader;            // 0x4D14: inferred: character id of the first player
    GameSaveRoute_unk m4D18[64];     // inferred: copy of 0x154 of the route tables
};

// inferred: 0x40 bytes saved per party member next to the equipment (copied whole into packets)
struct GameSaveEquipExtra_unk {
    inline GameSaveEquipExtra_unk();

    int mIds[8];
    unsigned char m20[0x20];
};

// inferred: item of the bag in the save data (8 bytes, format 10 and later)
struct GameSaveItem_unk {
    int mCode;              // inferred: item code (kind * 1000000 + id)
    unsigned char mCount;   // 0x04
    unsigned char m05;
    unsigned short mAge;    // 0x06: inferred: number of items obtained since this one
};
// inferred: item of the bag in the save data (0xC bytes, formats before 10)
struct GameSaveItemOld_unk {
    int mCode;
    unsigned short mCount;  // 0x04
    unsigned short m06;
    int m08;
};


// inferred: equipment entry of a network packet (atn/PacketPack fn_804D8910); names invented
struct GameSavePacketEquip_unk {
    int mSlot;                   // 0x00
    int mItem;                   // 0x04
    unsigned short mValid : 1;   // 0x08
    unsigned short mMarked : 1;  // inferred: GameSaveParty_unk m230 bit
    unsigned short mColor : 8;   // inferred: CharaStatusBase::m231
    unsigned short m08_5 : 6;
    unsigned char m0A[2];
    int m0C;
    GameSaveEquipExtra_unk mExtra; // 0x10
};

// inferred: status of the seven party members (0xFC4 bytes; its constructor 0x803BAA90 is called by
// game/GameTask)
struct GameSaveParty_unk {
    GameSaveParty_unk(); // 0x803BAA90
    void fn_803BAAE0(); // inferred: collects the status of the party (players or inventory)
    void fn_803BAD6C(); // inferred: copies the saved status to the players
    int fn_803BAE54(GameSavePacketEquip_unk* equip, int chara, int bit) const; // inferred: costume flags of a packet

    CharaStatusBase mStatus[7];
    unsigned int mMask; // 0xFC0: inferred: bit i set when member i is saved
};

// inferred: gimmick saved with its state (0x18 bytes)
struct GameSaveGimmick_unk {
    inline GameSaveGimmick_unk();

    // inferred: stores the state of `gimmick` (inlined twice in fn_803BB248; name invented)
    inline void Set_unk(GimmickCommon* gimmick);

    int mId;                // 0x00: GimmickCommon::mId
    unsigned short m04;     // 0x04: GimmickCommon::m4C
    unsigned char mFlags;   // 0x06: bit 0: used
    unsigned char mState;   // 0x07: GimmickCommon::m54
    unsigned char m08[0x18 - 0x08];
};

// inferred: saved gimmicks (at 0x6E60 of GameSaveData_unk)
struct GameSaveGimmicks_unk {
    void fn_803BB248(); // inferred: collects the state of the gimmicks
    // inferred: index of the saved state of `gimmick` (0x200 if not found; name invented)
    inline int FindGimmick_unk(GimmickCommon* gimmick);

    GameSaveGimmick_unk mEntries[0x200];
};

// inferred: blocks restored by their own function; contents not known yet
// inferred: player saved with its position and route (0x44 bytes)
struct GameSavePlayer_unk {
    inline GameSavePlayer_unk();

    unsigned short mFlags;  // bit 0: used, 1: dead, 2: on a ledge, 3: character flag, 4: walking a route
    unsigned char m02;      // inferred: CharaC58_unk::mC0
    unsigned char m03;      // inferred: CharaTask::m674
    int m04;                // inferred: CharaTask::m58
    atn::Vector3 mPos;      // 0x08
    atn::Vector3 mRot;      // 0x14
    int m20;                // inferred: CharaC58_unk::mB4
    short mRouteMode;       // 0x24: inferred: RouteWalker_unk::m44
    short mRouteFlag;       // 0x26: inferred: RouteWalker_unk::m48
    int mRouteTarget;       // 0x28: inferred: point, link, place or character of the route
    int mRouteRadius;       // 0x2C
    unsigned char m30[0x44 - 0x30];
};
struct GameSavePlayers_unk {
    void fn_803BA408(); // inferred: collects the players
    void fn_803BA620(); // inferred: restores the players
    GameSavePlayer_unk mEntries[16];
};
// inferred: enemy saved with its state (0x2C bytes)
struct GameSaveEnemy_unk {
    inline GameSaveEnemy_unk();

    unsigned short mFlags;  // bit 0: used, 1: dead, 2: on a ledge, 3..6: character flags
    unsigned char m02;      // inferred: CharaC58_unk::mC0
    unsigned char m03;      // inferred: CharaTask::m674
    int m04;                // inferred: CharaTask::m58
    int m08;                // inferred: CharaC58_unk::mB4
    unsigned char m0C[0x2C - 0x0C];
};
struct GameSaveEnemies_unk {
    void fn_803BAEF8(); // inferred: collects the enemies
    void fn_803BB0E0(); // inferred: restores the enemies
    GameSaveEnemy_unk mEntries[64];
};
struct GameSaveInventory_unk {
    void fn_803BB584(); // inferred: collects the inventory
    void fn_803BC2D4(); // inferred: restores the inventory
    void fn_803BD6C0(int mode); // inferred: resets the bag for a new game (mode 2)
    int fn_803BD0FC(GameSavePacketEquip_unk* equip, int chara, int slot) const; // inferred: protector of a packet
    int fn_803BD22C(GameSavePacketEquip_unk* equip, int chara, int slot) const; // inferred: weapon of a packet
    int fn_803BD2B0(int code, int count); // inferred: adds items (at most 99, and 10 of each weapon or protector)
    int fn_803BD49C(int code) const;      // inferred: number of items `code` (bag and equipment)
    int m0000;
    GameSaveEquipExtra_unk m0004[16];
    GameSaveItemOld_unk mOldItems[0x100]; // 0x404: inferred: items 0x400.. of the old format
    GameSaveEquipExtra_unk mExtras[7]; // 0x1004
    MP::SaveElemB_unk mUpgrades[28];   // 0x11C4: inferred: upgraded equipment
    GameSaveItem_unk mItems[0x600];    // 0x60F4
    Inventory::Equip_unk mEquips[7]; // 0x90F4: inferred: copy of Inventory::mEquips
    unsigned char m92B4[0x94C8 - 0x92B4];
    int m94C8; // inferred: shown in the slot header (GameSaveSlot_unk::m70)
    unsigned char m94CC[0x9548 - 0x94CC];
};
// inferred: settings of the save data (0xFC bytes): copy of SetConfig 0x24..0xA8
struct GameSaveSettings_unk {
    inline GameSaveSettings_unk();
    void fn_803BD79C(); // inferred: restores the settings

    unsigned int mMagic;        // 0x00: 0xDEADBEEF
    int mAlwaysBirdview;        // 0x04: inferred: GameConfig_unk::mAlwaysBirdview != 0
    int m08;                    // SetConfig::m24
    int mAttackType;            // 0x0C
    int mDiveType;              // 0x10
    int mInTacviewType;         // 0x14
    int mGatheringType;         // 0x18
    unsigned char m1C[8];       // SetConfig::m38
    float mAttackTurnSpeed;     // 0x24
    float mTurnSpeed;           // 0x28
    float mRollbackSpeed;       // 0x2C
    float mCameraRotSpeed;      // 0x30
    int m34;                    // SetConfig::m50
    float mRotDelay;            // 0x38
    int mBattleDrawHateLine;    // 0x3C
    int mDrawEnemyHP;           // 0x40
    int mDrawEnemyPopup;        // 0x44
    int mDrawFriendPopup;       // 0x48
    unsigned char m4C[0x10];    // SetConfig::m68
    int mShowDialog;            // 0x5C
    int mShowTutorial;          // 0x60
    int mEventSkipType;         // 0x64
    int mEventCameraControl;    // 0x68
    unsigned char m6C[0xC];     // SetConfig::m88
    SoundSettings_unk mSound;   // 0x78
    int m88;                    // SetConfig::mA4
    unsigned char m8C[0xFC - 0x8C];
};

struct GameSaveData_unk {
    GameSaveData_unk(); // 0x803BDB60
    void fn_803BDEE4(); // inferred: collects the current game state
    void fn_803BE590(); // inferred: restores the party, the inventory and the flags
    void fn_803BE670(); // inferred: restores the work values
    void fn_803BE6B8(); // inferred: restores the characters and the gimmicks

    int m0000;
    GameSaveFlags_unk mFlags;              // 0x0004
    unsigned char m4F1C[0x4F5C - 0x4F1C];
    GameSavePlayers_unk mPlayers;      // 0x4F5C
    GameSaveParty_unk mParty;        // 0x539C
    GameSaveEnemies_unk mEnemies;          // 0x6360
    GameSaveGimmicks_unk mGimmicks;    // 0x6E60
    GameSaveInventory_unk mInventory;      // 0x9E60
    GameSaveSettings_unk mSettings;        // 0x133A8
    unsigned int mChecksum;            // 0x134A4
};

#endif
