// inferred file name (splits-auto): frame timing and heap statistics shown by main (debug);
// the object is created by fn_8047A4C8 (new of 0x98 bytes, lbl_8087F534).
#include "atn/DLAllocator.h"
#include "atn/MemoryManager.h"
#include "game/HeapInfo_unk.h"

// inferred: OS_BUS_CLOCK / OSTicksToMicroseconds of the SDK
#define OS_BUS_CLOCK_UNK (*(unsigned long*)0x800000F8)
#define OS_TIMER_CLOCK_UNK (OS_BUS_CLOCK_UNK / 4)
#define OS_TICKS_TO_USEC_UNK(ticks) (((ticks) * 8) / (OS_TIMER_CLOCK_UNK / 125000))


HeapInfo_unk::HeapInfo_unk() {}

void HeapInfo_unk::fn_805A6BD4() {}

void HeapInfo_unk::fn_805A6BD8() {
    mUsec[0] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer0.mElapsed);
    mUsec[1] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer1.mElapsed);
    mUsec[2] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer2.mElapsed);
    mUsec[3] = (double)(unsigned long)OS_TICKS_TO_USEC_UNK((unsigned long)mTimer3.mElapsed);
    mHeapInfo[0] = atn::MemoryManager::Instance()->mMem1.fn_80084F84();
    mHeapInfo[1] = atn::MemoryManager::Instance()->mMem2.fn_80084F84();
    mHeapInfo[2] = atn::MemoryManager::Instance()->mOverflow.fn_80084F84();
}

float HeapInfo_unk::fn_805A6D10() {
    return 1000000.0f / mUsec[0]; // inferred: frames per second
}
