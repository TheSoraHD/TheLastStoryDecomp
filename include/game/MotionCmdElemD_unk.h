#ifndef GAME_MOTIONCMDELEMD_UNK_H
#define GAME_MOTIONCMDELEMD_UNK_H

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: fixed string of 0x20 bytes with out-of-line constructor (0x80042740) and destructor
// (0x80041C0C); the assignment is inlined (strlen + memcpy when the buffers differ).
// Moved here from gmk/Fireworks.h (also used by game/ColliAttrManager.cpp)
struct MotionCmdElemD_unk {
    MotionCmdElemD_unk();
    ~MotionCmdElemD_unk();
    MotionCmdElemD_unk& operator=(const char* str) {
        if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
        return *this;
    }
    char mStr[0x20];
};

#endif
