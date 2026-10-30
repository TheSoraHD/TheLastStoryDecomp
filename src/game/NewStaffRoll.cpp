#include "game/NewStaffRoll.h"
#include "atn/FadeCommon.h"
#include "game/GimmickCommon.h"
#include "macros.h"
#include "atn/DrawHelper.h"

extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" float fn_800DC3CC(const wchar_t* s); // inferred: wide string to float
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4); // inferred: debug switch
extern "C" void fn_8008771C(void* menu, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: debug float slider
extern "C" void fn_800D5908(atn::Texture_unk* tex, const char* path); // inferred: loads a texture
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task

// inferred: field (lbl_8087F430)
extern "C" void* lbl_8087F430;
// inferred: game state (lbl_8087EFA8)
struct StaffRollState_unk {
    unsigned char m00[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" StaffRollState_unk* lbl_8087EFA8;
// inferred: render manager (lbl_8087EEE0)
struct StaffRollScreen_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
extern "C" StaffRollScreen_unk* lbl_8087EEE0;
// inferred: scene manager (lbl_8087F540)
struct StaffRollScene_unk {
    unsigned char m00[0x190];
    int m190; // inferred: id of the current event
};
struct StaffRollGame_unk {
    unsigned char m00[0x70];
    StaffRollScene_unk* m70;
};
extern "C" StaffRollGame_unk* lbl_8087F540;
extern "C" void fn_8048169C(StaffRollGame_unk* game, int arg);
extern "C" void fn_8047E528(StaffRollGame_unk* game);
extern "C" void fn_8047F8B0(StaffRollGame_unk* game);
// inferred: debug drawing (lbl_8087EEB0) and the font used to measure the text (lbl_8087EEC8)
extern "C" void* lbl_8087EEC8;
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width

// inferred: type of a line (name invented)
static inline int __GetType_unk(const wchar_t* s) {
    if (wcscmp(s, L"FONT") == 0) {
        return 1;
    }
    if (wcscmp(s, L"END") == 0) {
        return 4;
    }
    if (wcscmp(s, L"BRAKE") == 0) {
        return 3;
    }
    return wcscmp(s, L"COPYRIGHT") != 0 ? 0 : 2;
}

// inferred: horizontal alignment of a line (name invented)
static inline int __GetAlign_unk(const wchar_t* s) {
    if (wcscmp(s, L"CENTER") == 0) {
        return 0;
    }
    if (wcscmp(s, L"LEFT") == 0) {
        return 1;
    }
    if (wcscmp(s, L"RIGHT") == 0) {
        return 2;
    }
    if (wcscmp(s, L"LEFT_R") == 0) {
        return 3;
    }
    return wcscmp(s, L"RIGHT_L") != 0 ? 0 : 4;
}

// inferred: x of the logo of the given width (name invented)
static inline float __GetLogoX_unk(int align, float screen, float width) {
    float x = 0.0f;
    switch (align) {
    case 0:
        x = 0.5f * (screen - width) - 100.0f;
        break;
    case 1:
        x = 0.5f * (screen - width) - 0.25f * (screen - 40.0f);
        break;
    case 2:
        x = 0.25f * (screen - 40.0f) + 0.5f * (screen - width);
        break;
    case 3:
        x = (0.5f * screen - 20.0f) - width;
        break;
    case 4:
        x = 0.5f * screen + 20.0f;
        break;
    }
    return x;
}

// inferred: x of a line of the given width (name invented)
static inline float __GetX_unk(int align, float screen, float width) {
    float x = 0.0f;
    switch (align) {
    case 0:
        x = 0.5f * (screen - width);
        break;
    case 1:
        x = 0.5f * (screen - width) - 0.25f * (screen - 40.0f);
        break;
    case 2:
        x = 0.25f * (screen - 40.0f) + 0.5f * (screen - width);
        break;
    case 3:
        x = (0.5f * screen - 20.0f) - width;
        break;
    case 4:
        x = 0.5f * screen + 20.0f;
        break;
    }
    return x;
}

// inferred: copies a string unless it is the destination itself (name invented)
inline void __CopyString_unk(wchar_t* dst, const wchar_t* src) {
    if (src != dst) {
        wmemcpy(dst, src, wcslen(src) + 1);
    }
}

NewStaffRoll::NewStaffRoll(atn::Task* parent)
    : atn::Task(parent), mPos(0.0f), mSpeed(1.3812156f), mBrake(0.0f), mWait(0.0f), mStopPos(0.0f),
      mStopTime(0.0f), mEndTime(0.0f), mDrawOff(0), mFade(0) {
    mLoader.vf0C("database/staffroll/staffroll_type1_xx.u16");
    void* menu = mMenu.Load("StaffRoll");
    fn_80087994(menu, "DrawOff", &mDrawOff, 0, 0);
    fn_8008771C(menu, "Speed", &mSpeed, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_800D5908(&mLogo, "ui_logo_nm.texture");
}

NewStaffRoll::~NewStaffRoll() {
    if (lbl_8087F430 != 0) {
        mFade = (StaffRollFade_unk*)atn::FadeColor::Create((atn::Task*)lbl_8087F430, 15, 0xFF000000, 0);
        mFade->m58 = 99999;
        mFade->m74 = 10.0f;
        mFade->m68 = 1;
        mFade->m48 = 1;
    }
}

int NewStaffRoll::vf0C() {
    if (mLoader.fn_80473F50() != 0 || ((GimmickUvOwner_unk*)&mLogo)->fn_800D59B8() != 0) {
        return 0;
    }
    fn_80531394();
    return 1;
}

// NONMATCHING: 99.3% - registers only: the original gives r26 to this, r27 to text and r28 to length
// (ours: r27, r24, r23) and swaps the registers of i and the line offset in the second loop. Tried
// declaring count/i earlier, computing the arguments inside the constructor call and a reference
// to mLoader. The csv address is in a saved register as in the original (empty ~CsvBase()).
void NewStaffRoll::fn_80531394() {
    unsigned int length = (unsigned int)mLoader.fn_8047059C() / 2;
    const wchar_t* text = (const wchar_t*)mLoader.fn_80470580();
    atn::CsvU csv(text, length);
    unsigned int count = 0;
    while (csv.fn_8005BBF8()) {
        if (__GetType_unk(csv.fn_8005B9CC()) != 0) {
            count++;
        }
    }
    csv.Reset_unk();
    mLines.Resize_unk(count, 0);
    float y = 0.0f;
    unsigned int i = 0;
    while (csv.fn_8005BBF8()) {
        if (i >= mLines.mSize) {
            break;
        }
        mLines[i].mType = __GetType_unk(csv.fn_8005B9CC());
        Line_unk& line = mLines[i];
        if (mLines[i].mType != 0) {
            line.mY = 3.0f * fn_800DC3CC(csv.fn_8005B9CC()) + y;
            mLines[i].mSize = wcscmp(csv.fn_8005B9CC(), L"SML") == 0 ? 16.0f : 16.0f;
            mLines[i].mAlign = __GetAlign_unk(csv.fn_8005B9CC());
            __CopyString_unk(mLines[i].mText, csv.fn_8005B9CC());
            y = mLines[i].mY;
            if (mLines[i].mType == 3) {
                mBrake = y;
            }
            i++;
        }
    }
}

// inferred: time of a frame (name invented)
static inline float __GetDt_unk() {
    return lbl_8087EFA8->m3A4;
}

void NewStaffRoll::vf10() {
    if (mWait > 0.0f) {
        mWait -= __GetDt_unk();
        if (mWait <= 0.0f) {
            mWait = 0.0f;
        }
        return;
    }
    if (mPos >= mStopPos && mStopTime > 0.0f) {
        mStopTime -= __GetDt_unk();
        if (mStopTime <= 0.0f) {
            mStopTime = 0.0f;
        }
        return;
    }
    bool end;
    if (mLines.mSize == 0) {
        end = true;
    } else {
        Line_unk& last = mLines[mLines.mSize - 1];
        if (last.mType == 4 && mPos >= last.mY) {
            end = true;
        } else {
            end = false;
        }
    }
    if (end) {
        if (lbl_8087F540 != 0 && lbl_8087F540->m70 != 0 && lbl_8087F540->m70->m190 == 5406) {
            return;
        }
        mEndTime += __GetDt_unk();
        if (mEndTime > 420.0f) {
            fn_8048169C(lbl_8087F540, 1);
            fn_8047E528(lbl_8087F540);
            fn_8047F8B0(lbl_8087F540);
            fn_800D2338(this);
        }
    } else {
        if (mBrake > 0.0f && mPos >= mBrake) {
            mSpeed *= 0.95f;
        }
        mPos += mSpeed * __GetDt_unk();
    }
}

void NewStaffRoll::vf14() {
    if (mDrawOff != 0) {
        return;
    }
    float width = lbl_8087EEE0->mWidth;
    float height = lbl_8087EEE0->mHeight;
    for (unsigned int i = 0; i < mLines.mSize; i++) {
        Line_unk& line = mLines.mData[i];
        float y = height - (mPos - line.mY);
        if (y >= -line.mSize && y <= height) {
            if (line.mType == 2) {
                float w = 0.95f * (int)GXGetTexObjWidth(&mLogo.mObj);
                float h = 0.95f * (int)GXGetTexObjHeight(&mLogo.mObj);
                float x = __GetLogoX_unk(line.mAlign, width, w);
                lbl_8087EEB0->mA0 = 19;
                lbl_8087EEB0->fn_8005E7F4(-1, 0, &mLogo, x, y, -1.05f, w, h, 0.0f, 0.0f, 1.0f, 1.0f);
                lbl_8087EEB0->mA0 = 18;
            } else {
                float w = fn_8006EF48(lbl_8087EEC8, line.mText, line.mSize, 0.0f, 1, 1);
                float x = __GetX_unk(line.mAlign, width, w);
                lbl_8087EEB0->mA0 = 19;
                lbl_8087EEB0->fn_80061824(1.0f + x, 1.0f + y, -1.05f, line.mSize, line.mSize, 0.0f, 0.0f, 0.0f, line.mText, 0xFF000000, 1, 1, 0, 0, 0xFF000000);
                lbl_8087EEB0->fn_80061824(x, y, -1.05f, line.mSize, line.mSize, 0.0f, 0.0f, 0.0f, line.mText, -1, 1, 1, 0, 0, 0xFF000000);
                lbl_8087EEB0->mA0 = 18;
            }
        }
    }
    if (mEndTime > 300.0f) {
        float t = MIN(MAX((mEndTime - 300.0f) / 90.0f, 0.0f), 1.0f);
        unsigned int color = (unsigned int)(255.0f * t) << 24;
        lbl_8087EEB0->mA0 = 19;
        lbl_8087EEB0->fn_80060D58(0.0f, 0.0f, -3.0f, lbl_8087EEE0->mWidth, lbl_8087EEE0->mHeight, color);
        lbl_8087EEB0->mA0 = 18;
    }
}

// inferred: the staff roll reached its END line
extern "C" int fn_80531E80(NewStaffRoll* roll) {
    if (roll->mLines.mSize == 0) {
        return 1;
    }
    NewStaffRoll::Line_unk& last = roll->mLines.mData[roll->mLines.mSize - 1];
    if (last.mType == 4 && roll->mPos >= last.mY) {
        return 1;
    }
    return 0;
}
