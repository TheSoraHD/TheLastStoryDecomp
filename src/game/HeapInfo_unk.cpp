// inferred file name (splits-auto): frame timing and heap statistics shown by main (debug);
// the object is created by fn_8047A4C8 (new of 0x98 bytes, lbl_8087F534).
#include "atn/DLAllocator.h"

typedef long long OSTime_unk;

// inferred: OS_BUS_CLOCK / OSTicksToMicroseconds of the SDK
#define OS_BUS_CLOCK_UNK (*(unsigned long*)0x800000F8)
#define OS_TIMER_CLOCK_UNK (OS_BUS_CLOCK_UNK / 4)
#define OS_TICKS_TO_USEC_UNK(ticks) (((ticks) * 8) / (OS_TIMER_CLOCK_UNK / 125000))

// inferred: memory manager of atn (fn_800827E0); only its three DLAllocator heaps are used here
struct MemoryManager_unk {
    char m00[0x138];
    atn::DLAllocator mHeap0; // 0x138
    atn::DLAllocator mHeap1; // 0x150
    atn::DLAllocator mHeap2; // 0x168
};
extern "C" MemoryManager_unk* fn_800827E0();

// inferred: start/end/elapsed time of one part of the frame (main writes them with OSGetTime)
struct FrameTimer_unk {
    FrameTimer_unk() {
        mStart = mEnd = 0;
        mElapsed = 0;
    }
    OSTime_unk mStart;   // 0x00
    OSTime_unk mEnd;     // 0x08
    OSTime_unk mElapsed; // 0x10
};

// inferred: name invented
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

HeapInfo_unk::HeapInfo_unk() {}

void HeapInfo_unk::fn_805A6BD4() {}

void HeapInfo_unk::fn_805A6BD8() {
    mUsec[0] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer0.mElapsed);
    mUsec[1] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer1.mElapsed);
    mUsec[2] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer2.mElapsed);
    mUsec[3] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer3.mElapsed);
    mHeapInfo[0] = fn_800827E0()->mHeap0.fn_80084F84();
    mHeapInfo[1] = fn_800827E0()->mHeap1.fn_80084F84();
    mHeapInfo[2] = fn_800827E0()->mHeap2.fn_80084F84();
}

float HeapInfo_unk::fn_805A6D10() {
    return 1000000.0f / mUsec[0]; // inferred: frames per second
}
