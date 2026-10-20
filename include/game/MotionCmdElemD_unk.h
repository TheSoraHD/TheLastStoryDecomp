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
    // inferred: weak copy 0x80043130 (game/AreaEffConfig.cpp)
    MotionCmdElemD_unk(const char* str) { *this = str; }
    MotionCmdElemD_unk& operator=(const char* str) {
        if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
        return *this;
    }
    // inferred: weak copy 0x8004276C (game/AreaEffConfig.cpp); the plain copy constructor copies
    // the 0x20 bytes
    MotionCmdElemD_unk& operator=(const MotionCmdElemD_unk& o) { return *this = o.mStr; }
    // inferred: inlined in AreaEffConfig::fn_80043014 (strlen of the stored string, then the
    // characters are compared one by one)
    bool operator==(const char* str) const {
        unsigned long n = strlen(mStr);
        return str[n] == '\0' && __Equal_unk(mStr, mStr + n, str);
    }
    static bool __Equal_unk(const char* first, const char* last, const char* first2) {
        for (; first != last; ++first, ++first2) {
            if (*first != *first2) {
                return false;
            }
        }
        return true;
    }
    char mStr[0x20];
};

#endif
