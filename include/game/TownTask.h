#ifndef GAME_TOWNTASK_H
#define GAME_TOWNTASK_H

#include "game/MapTask.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "atn/LH_Binary.h"
#include "std/string.h"
#include "std/vector.h"

// inferred: map part shown in a block (only the flags are used here)
struct TownBlockParts_unk {
    // inferred: name invented
    bool IsTown_unk() const { return (m38 & 2) == 2; }
    void SetHidden_unk() { m38 |= 4; }

    unsigned char m00[0x38];
    unsigned int m38; // inferred: bit 1: the block belongs to the town area, bit 2: hidden
    unsigned char m3C[0xF4 - 0x3C];
    int mF4;
};
class TownTpcLoader;

// inferred: one of the 3x3 loaded map blocks around the player (0x14 bytes); names invented
struct TownBlock_unk {
    TownBlock_unk() : mX(-1), mZ(-1), mParts(0), m10(0) {}
    int m00;
    int mX;  // 0x04
    int mZ;  // 0x08
    TownBlockParts_unk* mParts; // 0x0C
    TownTpcLoader* m10;         // 0x10: inferred: NPC loader of the block
};

// inferred: value of the town trees (0x28 bytes: key and data, copied word by word); names invented
struct TownValue_unk {
    int mKey;
    int m04[9];
};
// inferred: red-black tree node (0x34 bytes); names invented
struct TownNode_unk {
    TownNode_unk* mLeft;   // 0x00
    TownNode_unk* mRight;  // 0x04
    TownNode_unk* mParent; // 0x08
    TownValue_unk mValue;  // 0x0C
};
struct TownTree_unk;
extern "C" void fn_805B99A8(TownTree_unk* tree, TownNode_unk* node); // inferred: frees a subtree
// inferred: tree header (size, root, ?, leftmost; the end node is &mRoot); nodes are freed by fn_805B99A8.
// The destructor is inlined through five levels (five null checks of the address in ~TownTask); the class
// names are invented.
struct TownTreeBase_unk {
    TownTreeBase_unk() : mSize(0), mRoot(0), mBegin((TownNode_unk*)&mRoot) {}
    ~TownTreeBase_unk() {
        if (mRoot != 0) {
            fn_805B99A8((TownTree_unk*)this, mRoot);
        }
    }
    unsigned int mSize;
    TownNode_unk* mRoot;
    int m08;
    TownNode_unk* mBegin;
};
struct TownTree1_unk : public TownTreeBase_unk {
    ~TownTree1_unk() {}
};
struct TownTree2_unk : public TownTree1_unk {
    ~TownTree2_unk() {}
};
struct TownTree3_unk : public TownTree2_unk {
    ~TownTree3_unk() {}
};
struct TownTree_unk : public TownTree3_unk {
    ~TownTree_unk() {}
};
// inferred: vector of the town (destructor inlined through four levels); element type unknown
struct TownList_unk : public std::vector<void*> {
    ~TownList_unk() {}
};

// inferred from __vt__8TownTask (0x807978C8): the map task of the towns
class TownTask : public MapTask {
public:
    static TownTask* Create_unk(atn::Task* parent); // inferred: stripped by the linker
    TownTask(atn::Task* parent); // 0x805B6C5C
    virtual ~TownTask();         // 0x805B6EAC
    virtual int vf0C();          // 0x805B721C
    virtual void vf10();         // 0x805B7398
    virtual void vf14();         // 0x805B7B20
    void fn_805B7B04(const atn::Vector3& pos); // inferred: sets the position
    void fn_805B7B24();
    void fn_805B8DB4(int arg);
    // inferred: the loader at 0x58 holds the minimap csv (name invented)
    atn::LH_Binary* GetBinary_unk() { return (atn::LH_Binary*)m58; }

    ResHandle m11C; // inferred: debug menu group
    int m120;       // inferred: initialized flag
    int m124;
    int m128;
    int m12C;
    int m130;
    float m134;
    int m138;
    int m13C;
    atn::Vector3 m140;
    std::string m14C;
    std::string m158;
    float m164;
    atn::Vector3 m168;
    int m174;
    int m178;
    TownBlock_unk mBlocks[9]; // 0x17C
    TownTree_unk m230;
    TownTree_unk m240;
    int mWidth;  // 0x250
    int mHeight; // 0x254
    void* m258;  // inferred: minimap csv data
    TownList_unk m25C;
};

// inferred: TownTask::sInstance (kept under the label name: CharaTask, ChaserGuard, GimmickCommon, MarketTask and
// TownTpcLoader use it)
extern "C" TownTask* lbl_8087FA20;

#endif
