#ifndef ATN_STRING_UNK_H
#define ATN_STRING_UNK_H

#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: string owned with new[] (heap 1); a null pointer reads as "" (lbl_8087EF00, also
// used by the inlines of Event/DebugInfo_unk and game/GameTask). Also the name of the event nodes
// (Event::Node, set by Event/EventParser_unk.cpp)
struct String_unk {
    String_unk() : mPtr(0) {}
    ~String_unk() {
        if (mPtr != 0) {
            delete[] mPtr;
            mPtr = 0;
        }
    }
    const char* c_str() const { return mPtr != 0 ? mPtr : &sEmpty_unk; }
    void assign(const char* s, unsigned long len) {
        if (s != 0 && len != 0) {
            char* p = new ((MemoryTag)1, "", "", 0) char[len + 1];
            memcpy(p, s, len);
            p[len] = 0;
            if (mPtr != 0) {
                delete[] mPtr;
                mPtr = 0;
            }
            mPtr = p;
        } else if (mPtr != 0) {
            delete[] mPtr;
            mPtr = 0;
        }
    }
    String_unk& operator=(const char* s) {
        assign(s, s != 0 ? strlen(s) : 0);
        return *this;
    }
    String_unk& operator=(const String_unk& o) { return *this = o.c_str(); }

    char* mPtr;
    static char sEmpty_unk; // lbl_8087EF00
};

#endif
