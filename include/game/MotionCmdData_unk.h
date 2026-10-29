#ifndef GAME_MOTIONCMDDATA_UNK_H
#define GAME_MOTIONCMDDATA_UNK_H

#include "atn/Array_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/static_string.h"
#include "math/Vec.h"

namespace atn {

// atn::MotionCommandData (name from the RSO: its arrays are SimpleVector<MotionCommandData::Command>,
// SimpleVector<EffFile>, SimpleVector<EfpFile> and SimpleVector<static_string<32>>); built from the
// loaded file (0x30 bytes). The file keeps its old name.
struct MotionCommandData {
    // inferred: one line of a motion command file (0x34 bytes; parsed by fn_8011FFF8). Member names
    // invented.
    struct Command {
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

    int fn_800A091C(); // inferred: nonzero while a resource is loading
    atn::SimpleVector<Command> mEntries;             // 0x00
    atn::SimpleVector<atn::EffFile> mEffects;       // 0x0C: "EFF"
    atn::SimpleVector<atn::EfpFile> mEfps;           // 0x18: "EFP", "EFP_LOOP"
    atn::SimpleVector<atn::static_string<32> > mNames; // 0x24: "EFF_ATR", "SE_ATR", "SE", "SE_LOOP"
};

} // namespace atn

#endif
