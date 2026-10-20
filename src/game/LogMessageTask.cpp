#include "game/LogMessageTask.h"
#include "atn/Memory.h"

// inferred: debug settings (only the members used here)
struct LogSettings_unk {
    unsigned char m00[0x6C];
    int mLogMode;  // 0x6C: inferred: 2 = messages expire
    int mLogLevel; // 0x70
};
extern "C" LogSettings_unk* lbl_8087F0A8;

// inferred: colours of the message kinds (names invented)
struct LogColor_unk {
    LogColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
static LogColor_unk sColor0_unk(0.800000072f, 0.800000072f, 0.800000072f, 1.0f);
static LogColor_unk sColor1_unk(1.0f, 0.800000072f, 0.200000018f, 1.0f);
static LogColor_unk sColor2_unk(1.0f, 0.0f, 0.0f, 1.0f);

LogMessageTask* LogMessageTask::sInstance;

LogMessageTask* LogMessageTask::fn_80478714(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (3, "", "", 0) LogMessageTask(parent);
    }
    return sInstance;
}

// inferred: not inline; auto-inlined in fn_80478714 and stripped by the linker
LogMessageTask::LogMessageTask(atn::Task* parent) : atn::Task(parent), mAlpha(0.0f), mTimer(300) {}

LogMessageTask::~LogMessageTask() {
    sInstance = 0;
}

int LogMessageTask::vf0C() {
    return 1;
}

// inferred: clamps (names invented)
inline float Max_unk(float a, float b) {
    return a > b ? a : b;
}
inline float Min_unk(float a, float b) {
    return a < b ? a : b;
}

// NONMATCHING: 97.7% - callee-saved registers of the update/erase loop are assigned differently (this in r27 in the
// target); tried for/while forms, erase returning the iterator, while-based erase
void LogMessageTask::vf10() {
    LogEntry_unk* entry = mEntries.begin();
    while (entry != mEntries.end()) {
        entry->mFrames++;
        if (lbl_8087F0A8->mLogMode == 2 && entry->mFrames > 150) {
            mEntries.erase(entry);
        } else {
            entry++;
        }
    }
    if (++mTimer > 300) {
        if (mAlpha > 0.0f) {
            mAlpha = Max_unk(mAlpha - 0.1f, 0.0f);
            if (mAlpha <= 0.0f) {
                mEntries.mSize = 0;
            }
        }
    } else if (mAlpha < 1.0f) {
        mAlpha = Min_unk(0.1f + mAlpha, 1.0f);
    }
}

// NONMATCHING: vf14 (0x80478A70, 0xBAC bytes: draws the log) and fn_8047961C (0x324 bytes: adds a message) are
// not written yet
