#include "atn/D2A_Task.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"
#include "atn/DrawHelper.h"

// inferred: string hash and file helpers (automatic names)
extern "C" unsigned int fn_800DC6B4(const char* s);
extern "C" void fn_8006AD24(std::string& path, const std::string& ext); // inferred: sets the extension
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...);   // inferred: wide sprintf

// inferred: functions of the d2anime layout at D2A_Data + 0x10 (automatic names)
extern "C" void fn_801FEF40(void* layout, int frame);
extern "C" void fn_801FED24(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FED70(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FEDBC(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int id, const wchar_t* text);
extern "C" void fn_801FD5A0(atn::D2A_Common* element, int arg, int arg2);
extern "C" void fn_801FDA20(atn::D2A_Common* element, int arg, int arg2);
extern "C" void fn_801FFA04(void* data, int size, atn::D2A_Data* d2a, void* textures, int arg);

// inferred: d2anime manager (lbl_8087F138) and text table (lbl_8087EEC8)
struct D2AManager_unk {
    unsigned char m000[0x168];
    unsigned char m168[0x1A8 - 0x168]; // inferred: texture list
    atn::D2A_Task* mCurrent;            // 0x1A8
};
extern "C" D2AManager_unk* lbl_8087F138;
extern "C" int fn_801F362C(void* textures); // inferred: nonzero while busy
struct TextMgr_unk;
extern "C" TextMgr_unk* lbl_8087EEC8;
extern "C" void fn_8006F548(TextMgr_unk* mgr, std::basic_string<wchar_t>& out, int id); // inferred: text by id

inline void GetText_unk(std::basic_string<wchar_t>& out, int id) {
    fn_8006F548(lbl_8087EEC8, out, id);
}



atn::D2A_Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop) {
    if (parent != 0) {
        return new ((MemoryTag)8, "", "", 0) atn::D2A_Task(parent, path, loop);
    }
    return 0;
}

namespace atn {

// inferred: per-element parameter setters of the layout (names invented)
inline void SetParamA_unk(D2A_Task* task, const char* name, int index, float value) {
    fn_801FED24(&task->mData.m10, fn_800DC6B4(name), index, value);
}
inline void SetParamB_unk(D2A_Task* task, const char* name, int index, float value) {
    fn_801FED70(&task->mData.m10, fn_800DC6B4(name), index, value);
}
inline void SetParamC_unk(D2A_Task* task, const char* name, int index, float value) {
    fn_801FEDBC(&task->mData.m10, fn_800DC6B4(name), index, value);
}

// NONMATCHING: 99.59% - the path is kept in r26 instead of r28 (one more saved register); tried
// member orders and initializer lists
D2A_Task::D2A_Task(Task* parent, const char* path, int loop) : Task(parent) {
    mState = 1;
    mLoop = loop;
    mVisible = 1;
    mPlaying = 1;
    mRepeat = 1;
    mLoaded = 0;
    mPath = 0;
    mFrame = 0.0f;
    mSpeed = 1.0f;
    mBlend = 0x12;
    std::string name(path);
    fn_8006AD24(name, std::string("d2b"));
    mPath = 1;
    mFile.vf0C(name.data());
}

D2A_Task::~D2A_Task() {}

int D2A_Task::vf0C() {
    switch (mState) {
    case 0:
        mState = 1;
        break;
    case 1:
        if (mFile.fn_80473F50() == 0 && fn_801F362C(lbl_8087F138->m168) == 0) {
            if (mFile.fn_80470580() != 0) {
                mLoaded = 1;
                int size = mFile.fn_8047059C();
                fn_801FFA04(mFile.fn_80470580(), size, &mData, lbl_8087F138->m168, 1);
                mData.fn_801EFB00();
                mFile.fn_80473F88();
                if (mLoop) {
                    mState = 3;
                } else {
                    mState = 2;
                }
            } else {
                mState = 4;
                return 1;
            }
        }
        break;
    case 2:
        if (mData.fn_801EF9B4() == 0) {
            mState = 3;
            return 1;
        }
        break;
    case 3:
        return 1;
    }
    return 0;
}

} // namespace atn

// inferred: frame update shared by vf10 and fn_801F4484 (inline, name invented)
inline void UpdateFrame_unk(atn::D2A_Task* task) {
    if (!task->mLoaded) {
        return;
    }
    if (!task->mPlaying) {
        return;
    }
    float end = task->mData.mEndFrame;
    task->mFrame += task->mSpeed;
    if (task->mFrame >= end) {
        if (task->mRepeat) {
            task->mFrame -= end;
        } else {
            task->mFrame = end;
        }
    }
    if (task->mFrame < 0.0f) {
        if (task->mRepeat) {
            task->mFrame += task->mData.mEndFrame;
        } else {
            task->mFrame = 0.0f;
        }
    }
}

namespace atn {

void D2A_Task::vf10() {
    if (mVisible) {
        UpdateFrame_unk(this);
    }
}

} // namespace atn

void fn_801F4484(atn::D2A_Task* task) {
    UpdateFrame_unk(task);
}

namespace atn {

void D2A_Task::vf14() {
    if (!mVisible) {
        return;
    }
    int blend = lbl_8087EEB0->mA0;
    lbl_8087EEB0->mA0 = mBlend;
    if (mLoaded) {
        if (lbl_8087F138 != 0) {
            lbl_8087F138->mCurrent = this;
        }
        ApplyFrame();
        mData.vf10();
        if (lbl_8087F138 != 0) {
            lbl_8087F138->mCurrent = 0;
        }
    }
    lbl_8087EEB0->mA0 = blend;
}

void D2A_Task::ApplyFrame() {
    if (mLoaded) {
        mData.mFrame = mFrame;
        fn_801FEF40(&mData.m10, mFrame);
        mData.vf0C();
    }
}

} // namespace atn

void fn_801F465C(atn::D2A_Task* task, int setCurrent) {
    if (!task->mLoaded) {
        return;
    }
    if (lbl_8087F138 != 0 && setCurrent != 0) {
        lbl_8087F138->mCurrent = task;
    }
    task->ApplyFrame();
    task->mData.vf10();
    if (lbl_8087F138 != 0 && setCurrent != 0) {
        lbl_8087F138->mCurrent = 0;
    }
}

// NONMATCHING: 96.67% - v[0] is loaded at the top instead of right before the first hash call
void fn_801F4728(atn::D2A_Task* task, const char* name, const float* v) {
    atn::SetParamA_unk(task, name, 0, v[0]);
    atn::SetParamA_unk(task, name, 1, v[1]);
    atn::SetParamA_unk(task, name, 2, v[2]);
    atn::SetParamA_unk(task, name, 3, v[3]);
    atn::SetParamA_unk(task, name, 4, v[4]);
}

namespace atn {

// NONMATCHING: 95.45% - same early load of v[0] as fn_801F4728
void D2A_Task::SetParam1_unk(const char* name, const float* v) {
    SetParamA_unk(this, name, 0, v[0]);
    SetParamA_unk(this, name, 1, v[1]);
    SetParamA_unk(this, name, 4, v[4]);
}

// NONMATCHING: 96.15% - same early load of v[0] as fn_801F4728
void D2A_Task::SetParam2_unk(const char* name, const float* v) {
    SetParamB_unk(this, name, 0, v[0]);
    SetParamB_unk(this, name, 1, v[1]);
    SetParamB_unk(this, name, 2, v[2]);
    SetParamB_unk(this, name, 3, v[3]);
}

void D2A_Task::SetColor_unk(const char* name, unsigned int color) {
    fn_801FEDBC(&mData.m10, fn_800DC6B4(name), 0, color >> 24);
    fn_801FEDBC(&mData.m10, fn_800DC6B4(name), 1, (color >> 16) & 0xFF);
    fn_801FEDBC(&mData.m10, fn_800DC6B4(name), 2, (color >> 8) & 0xFF);
    fn_801FEDBC(&mData.m10, fn_800DC6B4(name), 3, color & 0xFF);
}

void D2A_Task::SetColor_unk(const char* name, float r, float g, float b) {
    fn_801FEDBC(&mData.m10, fn_800DC6B4(name), 1, r);
    fn_801FEDBC(&mData.m10, fn_800DC6B4(name), 2, g);
    fn_801FEDBC(&mData.m10, fn_800DC6B4(name), 3, b);
}

// NONMATCHING: 92% - same early load/multiply of rgb[0] as fn_801F4728
void D2A_Task::SetColor255_unk(const char* name, const float* rgb) {
    SetParamC_unk(this, name, 1, 255.0f * rgb[0]);
    SetParamC_unk(this, name, 2, 255.0f * rgb[1]);
    SetParamC_unk(this, name, 3, 255.0f * rgb[2]);
}

// NONMATCHING: 92.25% - the original computes &text into r6 and copies it to r4 (one extra mr)
void D2A_Task::SetText_unk(const char* name, int textId) {
    std::basic_string<wchar_t> text;
    GetText_unk(text, textId);
    fn_801FEE08(&mData.m10, fn_800DC6B4(name), text.data());
}

void D2A_Task::SetNumber_unk(const char* name, int value, int digits) {
    // inferred: each wide literal of the original pool ends with two terminators
    wchar_t format[0x20];
    wchar_t text[0x100];
    if (digits > 0) {
        fn_800DD3FC(format, L"%%%dd\0", digits);
    } else if (digits < 0) {
        fn_800DD3FC(format, L"%%0%dd\0", -digits);
    } else {
        fn_800DD3FC(format, L"%%d\0");
    }
    fn_800DD3FC(text, format, value);
    fn_801FEE08(&mData.m10, fn_800DC6B4(name), text);
}

void D2A_Task::fn_801F4D80(const char* name, int arg) {
    D2A_Common* element = mData.fn_801EFA78(fn_800DC6B4(name));
    if (element != 0) {
        fn_801FD5A0(element, arg, 1);
    }
}

void D2A_Task::fn_801F4DDC(const char* name, float arg) {
    D2A_Common* element = mData.fn_801EFA78(fn_800DC6B4(name));
    if (element != 0) {
        *(float*)((unsigned char*)element + 0x130) = arg;
    }
}

void D2A_Task::fn_801F4E30(const char* name, int arg) {
    D2A_Common* element = mData.fn_801EFA78(fn_800DC6B4(name));
    if (element != 0) {
        fn_801FDA20(element, arg, 1);
    }
}

} // namespace atn

atn::D2A_DrawRect fn_801F4E8C(atn::D2A_Task* task, unsigned int id) {
    atn::D2A_DrawRect rect;
    atn::D2A_Common* element = task->mData.fn_801EFA78(id);
    if (element != 0) {
        atn::D2A_DrawColor color;
        atn::D2A_DrawExtra extra;
        int work[6];
        element->fn_801ED928(&rect, &color, &extra, work);
    }
    return rect;
}

namespace atn {

D2A_DrawColor D2A_Task::GetColor_unk(unsigned int id) {
    D2A_DrawColor color;
    D2A_DrawRect rect;
    D2A_Common* element = mData.fn_801EFA78(id);
    if (element != 0) {
        D2A_DrawExtra extra;
        int work[6];
        element->fn_801ED928(&rect, &color, &extra, work);
    }
    return color;
}

void D2A_Task::GetDrawParams_unk(unsigned int id, D2A_DrawRect* rect, D2A_DrawColor* color, D2A_DrawExtra* extra,
                                 void* work) {
    D2A_Common* element = mData.fn_801EFA78(id);
    if (element != 0) {
        element->fn_801ED928(rect, color, extra, work);
    }
}

// inferred: removed by the linker (its format L"%f\0" is left in the wide string pool)
void D2A_Task::SetFloat_unk(const char* name, float value) {
    wchar_t text[0x100];
    fn_800DD3FC(text, L"%f\0", value);
    fn_801FEE08(&mData.m10, fn_800DC6B4(name), text);
}

} // namespace atn
