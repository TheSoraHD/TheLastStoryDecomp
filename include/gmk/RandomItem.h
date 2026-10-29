#ifndef GMK_RANDOMITEM_H
#define GMK_RANDOMITEM_H

#include "atn/Array_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/FixedArray.h"
#include "game/GimmickCommon.h"
#include "gmk/ItemSlotAnime.h"

struct RoutePoint_unk;

namespace gmk {

// inferred: place where an item can appear ("LOC": position, rotation in degrees, half size of the
// box); 0x1C bytes
struct RandomItemLoc_unk {
    RandomItemLoc_unk() {}
    RandomItemLoc_unk& operator=(const RandomItemLoc_unk& o) {
        mPos = o.mPos;
        mRotY = o.mRotY;
        mSize = o.mSize;
        return *this;
    }
    // inferred: center of the box (name invented)
    atn::Vector3 GetCenter_unk() const { return mPos + atn::Vector3(0.0f, mSize.y, 0.0f); }
    atn::Vector3 mPos;  // 0x00
    float mRotY;        // 0x0C
    atn::Vector3 mSize; // 0x10
};

// inferred: item entry ("ITEM": id and parameters, then its places); 0x830 bytes. The route
// points inside its boxes are collected by RandomItem::vf28.
struct RandomItemDrop_unk {
    RandomItemDrop_unk() {}

    int mId;                                              // 0x00
    int m04;                                              // 0x04
    atn::SimpleVector<RandomItemLoc_unk> mLocs;             // 0x08
    atn::FixedArray_unk<RoutePoint_unk*, 0x200> mPoints;  // 0x14
    float m818;
    int m81C;
    int m820;
    int m824;
    int m828;
    int m82C;
};

// inferred: item that appears at a random place ("ITEM"/"LOC" entries); effects "EFP",
// "EFP_ITEM", "EFP_GET". The name is in the DOL (__vt__Q23gmk10RandomItem).
class RandomItem : public GimmickCommon {
public:
    RandomItem(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_80432CF0(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~RandomItem();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);
    // inferred: defined after vf70 (0x8043465C-0x80435CE4, outside the split of this unit)
    void fn_8043465C();
    void fn_804348C4();
    void fn_80435684(int state);

    atn::EfpFile mEfp;     // 0xF4: "EFP"
    atn::EfpFile mEfpItem; // 0x100: "EFP_ITEM"
    atn::EfpFile mEfpGet;  // 0x10C: "EFP_GET"
    int m118;
    int m11C;                    // inferred: state
    float m120;                  // inferred: "DROP" 1
    int m124;                    // inferred: "DROP" 2
    int m128;                    // inferred: debug draw
    atn::SimpleVector<atn::Vector3> m12C; // inferred: element type unknown (constructor, no destructor)
    atn::SimpleVector<RandomItemDrop_unk> mDrops; // 0x138
    RandomItemDrop_unk* m144;    // inferred: current entry
    int m148;
    int m14C;
    int m150;
    atn::Vector3 m154;           // inferred: position of the item
    int m160;
    int m164;
    int m168;
    ItemSlotAnime mAnime;        // 0x16C
    int m260;
    int m264;                    // inferred: unused (the object is 0x268 bytes)
};

} // namespace gmk

#endif
