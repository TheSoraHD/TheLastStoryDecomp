#ifndef GAME_EQUIPITEMDATA_H
#define GAME_EQUIPITEMDATA_H

#include "atn/Csv.h"
#include "atn/LH_Binary.h"
#include "atn/Model_unk.h"
#include "math/Vec.h"

// inferred: model of an equipped item, attached to a node of another model and read from a CSV
// line ("<path> <node> <scale> <flag>") or from a "chr" file. The name is in the DOL
// (__vt__13EquipItemData, 0x80789E98).
class EquipItemData {
public:
    EquipItemData(atn::CsvTab* csv); // 0x80363284
    int fn_8036354C();                // inferred: loads the data, nonzero until done
    void fn_803635E8();               // inferred: reads the "chr" file (model and motions)
    void fn_803637D4();               // inferred: reads the second part (scale, node, node edits)
    void fn_80363960(atn::Model* target); // inferred: attaches the item to `target`
    virtual void vf08();               // inferred: places the model at the node of the target
    virtual void vf0C();               // inferred: draws the model

    atn::Model mModel;      // 0x04
    atn::Model* mTarget;    // 0x3D4
    int mNode;             // 0x3D8: hash of the node name, then the node index
    // inferred: flags in a member with its own constructor (they are set between mNode and mScale)
    struct Flags_unk {
        Flags_unk() : mFlag(1), mCopyMotion(0) {}
        int mFlag : 1;       // bit 31: inferred
        int mCopyMotion : 1; // bit 30: "COPY_MOTION"
    } mFlags;              // 0x3DC
    atn::Vector3 mScale;   // 0x3E0
    atn::LH_Binary mLoader; // 0x3EC
    int mState;            // 0x3F4
};

#endif
