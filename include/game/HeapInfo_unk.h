#ifndef GAME_HEAPINFO_UNK_H
#define GAME_HEAPINFO_UNK_H

// inferred: frame timing and heap statistics shown by main (debug); the object is created by the
// start-up of game/main (new of 0x98 bytes, lbl_8087F534). Names invented.

typedef long long OSTime_unk;
extern "C" OSTime_unk OSGetTime();

// inferred: start/end/elapsed time of one part of the frame (main writes them with OSGetTime)
struct FrameTimer_unk {
    FrameTimer_unk() {
        mStart = mEnd = 0;
        mElapsed = 0;
    }
    // inferred: inlined by game/main (names invented)
    void Start_unk() { mStart = OSGetTime(); }
    void Stop_unk() {
        mEnd = OSGetTime();
        mElapsed = mEnd - mStart;
    }
    OSTime_unk mStart;   // 0x00
    OSTime_unk mEnd;     // 0x08
    OSTime_unk mElapsed; // 0x10
};

struct HeapInfo_unk {
    HeapInfo_unk();
    void fn_805A6BD4();
    void fn_805A6BD8();
    float fn_805A6D10();

    FrameTimer_unk mTimer0; // 0x00
    FrameTimer_unk mTimer1; // 0x18
    FrameTimer_unk mTimer2; // 0x30
    FrameTimer_unk mTimer3; // 0x48
    FrameTimer_unk mTimer4; // 0x60
    float mUsec[4];            // 0x78
    void* mHeapInfo[3];        // 0x88: inferred: fn_80084F84 of each heap
    int m94;
};

#endif
