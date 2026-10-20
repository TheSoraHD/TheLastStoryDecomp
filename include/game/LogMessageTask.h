#ifndef GAME_LOGMESSAGETASK_H
#define GAME_LOGMESSAGETASK_H

#include "atn/Task.h"
#include "atn/FixedArray.h"

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);

// inferred: fixed-size wide string (copy constructor copies the whole buffer, assignment copies the text);
// name invented
template <int N>
struct FixedWString_unk {
    FixedWString_unk() { mText[0] = 0; }
    FixedWString_unk(const wchar_t* s) { Assign_unk(s); }
    FixedWString_unk& operator=(const FixedWString_unk& o) {
        Assign_unk(o.mText);
        return *this;
    }
    // inferred: compares with `s` (wcsncmp over the longer length); defined in LogMessageTask.cpp
    bool operator==(const wchar_t* s) const;
    void Assign_unk(const wchar_t* s) {
        if (s != mText) {
            wmemcpy(mText, s, wcslen(s) + 1);
        }
    }
    wchar_t mText[N];
};

// inferred: one line of the message log (0x90 bytes); names invented
struct LogEntry_unk {
    LogEntry_unk() : mFrames(0), mKind(-1), mFlags(0), m8C(0) {} // 0x804787CC
    ~LogEntry_unk() {}                                            // 0x804787EC
    int mFrames;
    FixedWString_unk<0x40> mText;
    int mKind;
    int mFlags;
    int m8C;
};

// inferred: list of the log lines (one more destructor level than atn::FixedArray_unk); name invented
struct LogList_unk : public atn::FixedArray_unk<LogEntry_unk, 16> {
    LogEntry_unk* begin() { return &mData[0]; }
    LogEntry_unk* end() { return &mData[mSize]; }
    void erase(LogEntry_unk* it) {
        for (unsigned int i = it - mData; i < mSize - 1; i++) {
            mData[i] = mData[i + 1];
        }
        mSize = mSize - 1;
    }
};

// inferred from __vt__14LogMessageTask (0x80790028): message log shown on the screen; singleton
class LogMessageTask : public atn::Task {
public:
    static LogMessageTask* fn_80478714(atn::Task* parent); // inferred: creates the instance
    LogMessageTask(atn::Task* parent);
    virtual ~LogMessageTask(); // 0x8047882C
    virtual int vf0C();        // 0x804788B4
    virtual void vf10();       // 0x804788BC
    virtual void vf14();       // 0x80478A70
    void fn_8047961C(const wchar_t* text, int kind, int flags, int level); // inferred: adds a message

    static LogMessageTask* sInstance;

    LogList_unk mEntries; // 0x48
    float mAlpha;  // 0x94C
    int mTimer;    // 0x950
    int m954;
};

#endif
