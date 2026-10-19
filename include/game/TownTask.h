#ifndef GAME_TOWNTASK_H
#define GAME_TOWNTASK_H

#include "game/MapTask.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "std/string.h"

// inferred: 0x14-byte entry of the array at 0x190 (class unknown)
struct TownBlock_unk {
    void Init_unk() {
        m04 = -1;
        m08 = -1;
        m0C = 0;
        m10 = 0;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// inferred: fixed-size array whose ctor constructs its elements in a loop (name invented)
template <typename T, int N>
struct FixedArray_unk {
    FixedArray_unk() {
        T* p = mData;
        T* end = mData + N;
        while (p < end) {
            p->Init_unk();
            p++;
        }
    }
    T mData[N];
};

// inferred: binary tree header (Dinkumware-like; nodes are freed by fn_805B99A8)
struct TownTree_unk {
    TownTree_unk() : m00(0), mRoot(0), mBegin(&mRoot) {}
    int m00;
    void* mRoot;
    int m08;
    void** mBegin;
};

// inferred from __vt__8TownTask (0x807978C8): the map task of the towns
class TownTask : public MapTask {
public:
    TownTask(atn::Task* parent); // 0x805B6C5C
    virtual ~TownTask();         // 0x805B6EAC
    virtual int vf0C();          // 0x805B721C
    virtual void vf10();         // 0x805B7398
    virtual void vf14();         // 0x805B7B20
    void fn_805B7B04(const atn::Vector3& pos); // inferred: sets the position
    void fn_805B7B24();
    void fn_805B8DB4(int arg);

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
    int m17C;
    int m180;
    int m184;
    int m188;
    int m18C;
    FixedArray_unk<TownBlock_unk, 8> m190;
    TownTree_unk m230;
    TownTree_unk m240;
    int mWidth;  // 0x250
    int mHeight; // 0x254
    void* m258;  // inferred: minimap csv data
    int m25C;
    int m260;
    int m264;
};

// inferred: TownTask::sInstance (kept under the label name: CharaTask, ChaserGuard, GimmickCommon, MarketTask and
// TownTpcLoader use it)
extern "C" TownTask* lbl_8087FA20;

#endif
