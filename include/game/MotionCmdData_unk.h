#ifndef GAME_MOTIONCMDDATA_UNK_H
#define GAME_MOTIONCMDDATA_UNK_H

#include "atn/Array_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/MotionCmdElemD_unk.h"
#include "math/Vec.h"

// inferred: one line of a motion command file (0x34 bytes; parsed by fn_8011FFF8). Names invented.
struct MotionCmdEntry_unk {
    int mByName;          // 0x00: inferred: nonzero = m04 is the hash of the motion name
    unsigned int m04;     // inferred: motion (resource id or name hash)
    int mFrame;           // 0x08
    int mType;            // 0x0C: index in "EFF_ATR", "EFF", "EFP", "EFP_LOOP", "SE_ATR", "SE", "SE_LOOP", "SHAKE"
    int mArea;            // 0x10: 0 "ALL", 1 "TOWN", 2 "DUNGEON"
    atn::Vector3 mOffset; // 0x14
    float mScale;         // 0x20: inferred: effect scale or sound volume
    int mUseNode;         // 0x24
    unsigned int mNode;   // 0x28: inferred: node name hash
    unsigned int mIndex;  // 0x2C: inferred: index in the resource array of the type
    unsigned int mFlags;  // 0x30
};

// inferred: motion command data built from the loaded file (0x30 bytes; also defined locally in
// atn/LH_MotionCommandData.cpp)
struct MotionCmdData_unk {
    int fn_800A091C(); // inferred: nonzero while a resource is loading
    atn::Vector_unk<MotionCmdEntry_unk> mEntries;  // 0x00
    atn::Vector_unk<ResLoader_unk> mEffects;       // 0x0C: "EFF"
    atn::Vector_unk<atn::EfpHandle_unk> mEfps;     // 0x18: "EFP", "EFP_LOOP"
    atn::Vector_unk<MotionCmdElemD_unk> mNames;    // 0x24: "EFF_ATR", "SE_ATR", "SE", "SE_LOOP"
};

#endif
