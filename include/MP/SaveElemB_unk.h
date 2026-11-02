#ifndef MP_SAVEELEMB_UNK_H
#define MP_SAVEELEMB_UNK_H

// inferred: upgraded equipment piece (MP::SaveElemB_unk, 0x2D4 bytes); its own header so that
// game/Inventory.h can hold an array of them without the rest of MP/SaveLoad.h
namespace MP {

// inferred: part of SaveElemB_unk (0x14 bytes): id and four floats (-1, 1, 1, -1); names invented
struct SaveElemBPart_unk {
    inline SaveElemBPart_unk(); // defined in game/GameSaveData_unk.cpp
    short mId;
    short m02;
    float m04;
    float m08;
    float m0C;
    float m10;
};
// inferred: element type destroyed by ~SaveLoad; 28 of them are in the save data (upgraded
// equipment, GameSaveInventory_unk::mUpgrades); constructor in game/GameSaveData_unk.cpp
struct SaveElemB_unk {
    SaveElemB_unk();  // 0x803BB3C0
    ~SaveElemB_unk(); // 0x80119ECC
    int m000;
    int m004;
    int m008;
    int m00C;
    SaveElemBPart_unk m010[32];
    int m290;
    short m294;
    unsigned char m296[0x2D4 - 0x296];
};

} // namespace MP

#endif
