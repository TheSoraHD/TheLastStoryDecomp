#include "game/LogMessageTask.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "macros.h"
#include "atn/SoundRef.h"

// inferred: debug settings (only the members used here)
struct LogSettings_unk {
    unsigned char m00[0x6C];
    int mLogMode;  // 0x6C: inferred: 2 = messages expire
    int mLogLevel; // 0x70
};
extern "C" LogSettings_unk* lbl_8087F0A8;

// inferred: colours of the message kinds (names invented)
struct LogColor_unk {
    LogColor_unk() {}
    LogColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    LogColor_unk operator+(const LogColor_unk& o) const;
    LogColor_unk operator-(const LogColor_unk& o) const;
    LogColor_unk operator*(float f) const;
    float r, g, b, a;
};
static const char sSeLog_unk[] = "SE_UICOM_012";
static const char* const sSeLogName_unk = sSeLog_unk;

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

// inferred: field (only the pause state is used here)
struct LogField_unk {
    unsigned char m00[0x54E4];
    int m54E4;
};
extern "C" LogField_unk* lbl_8087F430;
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_80061824(void* draw, float x, float y, float z, float w, float h, float f6, float f7, float f8,
                            const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2); // inferred: text
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" double sin(double x);

inline bool HasFlag_unk(unsigned int flags, unsigned int flag) {
    return (flags & flag) == flag;
}
// inferred: float colour component to a byte (names invented)
inline unsigned int ToByte_unk(float x) {
    return x >= 1.0f ? 255 : (x <= 0.0f ? 0 : (unsigned int)(255.0f * x + 0.5f));
}
inline unsigned int ToRGBA_unk(const LogColor_unk& c) {
    return (ToByte_unk(c.a) << 24) | (ToByte_unk(c.r) << 16) | (ToByte_unk(c.g) << 8) | ToByte_unk(c.b);
}

// NONMATCHING: 87.7% - stack slots of the colour temporaries and the scheduling of the lerps, constants of the
// mode 2 loop are reloaded instead of kept in saved registers.
// inferred: draws the last eight lines (mode 1, sliding and flashing) or every line (mode 2)
void LogMessageTask::vf14() {
    if (lbl_8087F430 == 0 || lbl_8087F430->m54E4 != 0) {
        return;
    }
    if (mAlpha <= 0.0f) {
        return;
    }
    int mode = lbl_8087F0A8->mLogMode;
    if (mode == 1) {
        unsigned int lines = 8;
        for (unsigned int i = 0; i < 8; i++) {
            if (i >= mEntries.mSize) {
                return;
            }
            LogEntry_unk& entry = mEntries.mData[mEntries.mSize - 1 - i];
            float x = 380.0f;
            if (entry.mFrames < 10) {
                x += 640.0f * ((10 - entry.mFrames) / 10.0f);
            }
            float t = MAX((6 - mTimer) / 6.0f, 0.0f);
            float y = 14.0f * (8 - i) + 30.0f + 14.0f * t;
            LogColor_unk color(0.800000072f, 0.800000072f, 0.800000072f, 1.0f);
            if (!HasFlag_unk(entry.mFlags, 1)) {
                if (10 <= entry.mFrames && entry.mFrames < 40) {
                    int frames = entry.mFrames - 10;
                    if (frames < 5) {
                        color = (sColor1_unk - sColor0_unk) * (frames / 5.0f) + sColor0_unk;
                    } else if (5 <= frames && frames < 30) {
                        color = (sColor0_unk - sColor1_unk) * ((frames - 5) / 25.0f) + sColor1_unk;
                    }
                }
            } else {
                if (10 <= entry.mFrames && entry.mFrames < 41) {
                    int frames = entry.mFrames - 10;
                    if (frames < 2) {
                        color = (sColor2_unk - sColor0_unk) * (frames / 2.0f) + sColor0_unk;
                    } else if (2 <= frames && frames < 4) {
                        color = (sColor0_unk - sColor2_unk) * ((frames - 2) / 2.0f) + sColor2_unk;
                    } else if (4 <= frames && frames < 6) {
                        color = (sColor2_unk - sColor0_unk) * ((frames - 4) / 2.0f) + sColor0_unk;
                    } else if (6 <= frames && frames < 31) {
                        color = (sColor0_unk - sColor2_unk) * ((frames - 6) / 25.0f) + sColor2_unk;
                    }
                    if (frames == 2) {
                        fn_800C31F4(sSeLogName_unk, 0, -1, 0.7f);
                    }
                }
            }
            color.a = mAlpha * (float)sin(0.5f * (3.1415927f * ((float)(lines - i) / (float)lines)));
            fn_80061824(lbl_8087EEB0, 1.0f + x, 1.0f + y, 200.0f, 12.0f, 14.0f, 0.0f, 0.0f, 0.0f, entry.mText.mText,
                        ToRGBA_unk(color) & 0xFF000000, 1, 1, 0, 0, 0xFF000000);
            fn_80061824(lbl_8087EEB0, x, y, 200.0f, 12.0f, 14.0f, 0.0f, 0.0f, 0.0f, entry.mText.mText,
                        ToRGBA_unk(color), 1, 1, 0, 0, 0xFF000000);
        }
    } else if (mode == 2) {
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            LogEntry_unk& entry = mEntries.mData[i];
            fn_80061824(lbl_8087EEB0, 400.0f, 14.0f * i + 85.0f, 200.0f, 12.0f, 14.0f, 0.0f, 0.0f, 0.0f,
                        entry.mText.mText, (entry.mKind & 0xFF000000) | 0xCCCCCC, 1, 1, 0, 0, 0xFF000000);
        }
    }
}

extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
extern "C" int fn_806846FC(const wchar_t* a, const wchar_t* b, unsigned long n); // inferred: wcsncmp
extern "C" void fn_803B3D2C(std::basic_string<wchar_t>* s); // inferred: converts the message tags

// inferred: text of a wide buffer, the empty text for null (names invented)
inline const wchar_t* CStr_unk(const wchar_t* s) {
    return s != 0 ? s : lbl_808813D0;
}


// NONMATCHING: 89.7% - the parameters take r25-r28 in the original (loop registers above them), and the
// c_str checks of the temporary text are not CSE-d there (pointer kept in r24).
void LogMessageTask::fn_8047961C(const wchar_t* text, int kind, int flags, int level) {
    if (lbl_8087F0A8->mLogMode > 0 && level >= lbl_8087F0A8->mLogLevel && text != 0) {
        if (HasFlag_unk(flags, 2) && mAlpha > 0.25f) {
            bool same = mEntries.mSize != 0 &&
                        mEntries.mData[mEntries.mSize - 1].mText == text;
            if (same) {
                return;
            }
        }
        if (mEntries.mSize >= 16) {
            mEntries.erase(mEntries.begin());
        }
        std::basic_string<wchar_t> str(text);
        fn_803B3D2C(&str);
        LogEntry_unk entry;
        entry.mText.Assign_unk(str.c_str());
        entry.mKind = kind;
        entry.mFlags = flags;
        mEntries.push_back(entry);
        mTimer = 0;
    }
}

// inferred: not inline; auto-inlined in fn_8047961C (defined after it, so the address of the temporary is
// kept in a register)
template <int N>
bool FixedWString_unk<N>::operator==(const wchar_t* s) const {
    FixedWString_unk<N> o(s);
    const wchar_t* a = mText;
    const wchar_t* b = o.mText;
    unsigned long n = MAX(wcslen(CStr_unk(b)), wcslen(a));
    return fn_806846FC(CStr_unk(b), a, n) == 0;
}

// inferred: not inline (auto-inlined in vf14, which is defined before them)
LogColor_unk LogColor_unk::operator+(const LogColor_unk& o) const {
    return LogColor_unk(r + o.r, g + o.g, b + o.b, a + o.a);
}
LogColor_unk LogColor_unk::operator-(const LogColor_unk& o) const {
    return LogColor_unk(r - o.r, g - o.g, b - o.b, a - o.a);
}
LogColor_unk LogColor_unk::operator*(float f) const {
    return LogColor_unk(r * f, g * f, b * f, a * f);
}
