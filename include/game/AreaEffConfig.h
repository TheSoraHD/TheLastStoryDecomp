#ifndef GAME_AREAEFFCONFIG_H
#define GAME_AREAEFFCONFIG_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"
#include "atn/D2A_Common.h"
#include "atn/Array_unk.h"
#include "game/MotionCmdElemD_unk.h"

// inferred: row of the area effect configuration CSV (0x20 bytes): hash of the first column,
// group (0: none, 1: "ATTR", 2: "MAGIC", 3: "MAGIC_CIRCLE"), color and the names of the rest of
// the row. Name invented.
struct AreaEffEntry_unk {
    unsigned int mHash;
    int mType;
    atn::D2A_DrawColor mColor;                     // 0x08
    atn::Buffer_unk<MotionCmdElemD_unk> mNames;    // 0x18
};

// inferred: name from the RTTI (vtable 0x807772E8; vf10/vf14 are atn::Task's own empty functions,
// whose weak copies 0x8004312C/0x80043128 the original emits in this file). Loads a CSV of colors per group (created by
// the map parts, fn_80494354; read by MapManager and MapPartsGimmickLoc).
class AreaEffConfig : public atn::Task {
public:
    AreaEffConfig(atn::Task* parent, const char* path) : atn::Task(parent) {
        mBinary.vf0C(path);
        for (int i = 0; i < 4; i++) {
            mDefault[i].r = 1.0f;
            mDefault[i].g = 1.0f;
            mDefault[i].b = 1.0f;
            mDefault[i].a = 1.0f;
        }
    }
    virtual ~AreaEffConfig();
    virtual int vf0C();

    static AreaEffConfig* fn_80041A80(atn::Task* parent, const char* path); // inferred: creates the task
    void fn_80041D44(); // inferred: parses the loaded CSV
    // inferred: color of the group `type` for `name` (the default color of the group when not found)
    const atn::D2A_DrawColor* fn_80043014(int type, const char* name);

    atn::LH_Binary mBinary;                   // 0x48
    atn::Buffer_unk<AreaEffEntry_unk> mEntries; // 0x50
    atn::D2A_DrawColor mDefault[4];           // 0x58
};

#endif
