#ifndef ATN_SMALLALLOCATOR_H
#define ATN_SMALLALLOCATOR_H

// atn::SmallAllocator<N> (pools of small blocks of the engine memory manager, atn/SmallAllocator_8.cpp)
#include "IAllocator.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

namespace atn {

// inferred: {block size, number of 32-block groups} of each pool
struct SmallPoolConfig_unk {
    unsigned long mSize;
    unsigned long mCount;
};

// inferred: one pool of a SmallAllocator (a bitmap of used blocks and its blocks)
struct SmallPool_unk {
    SmallPool_unk() {
        mUsed = 0;
        mPeak = 0;
        mBitmap = 0;
        mBase = 0;
    }
    unsigned long* mBitmap; // 0x00
    unsigned char* mBase;   // 0x04
    int mUsed;              // 0x08
    int mPeak;              // 0x0C
};

// inferred: allocator of small blocks in N size classes
template <int N>
class SmallAllocator : public IAllocator {
public:
    SmallAllocator(const SmallPoolConfig_unk* config) : m04(0), mConfig(config), mCount(N) {}
    // NONMATCHING: <8> matches; <6> 0% - its operator delete inlines the manager constructor,
    // the original calls it (fn_80082970), same as __dl__FPv
    virtual ~SmallAllocator() {}
    virtual void* vf0C(unsigned long size, unsigned long align, int heap, const char* name);
    virtual void vf10(void* p);
    // NONMATCHING: 97.9% - the original keeps mCount - 1 in its own register
    virtual void* vf14() {
        const SmallPoolConfig_unk& config = mConfig[mCount - 1];
        return mPools[mCount - 1].mBase + config.mCount * (config.mSize << 5);
    }
    virtual void* vf18() { return mPools[0].mBase; }
    virtual void vf1C() {}

    // inferred: size of the bitmaps and blocks of all the pools
    unsigned long GetSize_unk() const {
        unsigned long total = 0;
        for (int i = 0; i < mCount; i++) {
            total += mConfig[i].mCount * (mConfig[i].mSize << 5) + mConfig[i].mCount * 4;
        }
        return total;
    }
    // inferred: places the bitmaps, then the blocks, in buf
    void Setup_unk(void* buf) {
        unsigned char* p = (unsigned char*)buf;
        m04 = p;
        for (int i = 0; i < mCount; i++) {
            mPools[i].mBitmap = (unsigned long*)p;
            memset(p, 0, mConfig[i].mCount * 4);
            p += mConfig[i].mCount * 4;
        }
        for (int i = 0; i < mCount; i++) {
            mPools[i].mBase = p;
            p += mConfig[i].mCount * (mConfig[i].mSize << 5);
        }
    }

    void* m04;                          // inferred: buffer of the bitmaps and the blocks
    const SmallPoolConfig_unk* mConfig; // 0x08
    int mCount;                         // 0x0C
    SmallPool_unk mPools[N];            // 0x10
};

} // namespace atn

#endif
