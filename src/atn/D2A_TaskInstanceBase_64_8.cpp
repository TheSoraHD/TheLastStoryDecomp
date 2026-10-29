#include "atn/D2A_TaskInstanceC.h"
#include "atn/D2A_Task.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

// Instances of d2anime files: each one keeps its own frame and pane values and applies them to the
// D2A_Task of the file (shared by all the instances of the same path) before drawing it. Three
// instantiations: <64, 8> (GameUI), <32, 8> (menus, D3AnimeInstance) and <8, 2> (GameUI).

extern "C" unsigned int fn_800DC6B4(const char* s);                  // inferred: string hash
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" void fn_800D2338(atn::Task* task); // inferred: kills the task

// inferred: functions of the d2anime layout at D2A_Data + 0x10 (automatic names)
extern "C" void fn_801FECE0(void* layout, unsigned int id, float value);
extern "C" void fn_801FED24(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FED70(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FEDBC(void* layout, unsigned int id, int index, float value);
extern "C" void fn_801FEEFC(void* layout, unsigned int id, int value);
extern "C" void fn_801FEE08(void* layout, unsigned int id, const wchar_t* text);
extern "C" void* fn_801FEB9C(void* layout, const char* name); // inferred: track of a pane value

// inferred: setters of the layout of a task (names invented; the extra inline level gives the
// order of the argument loads of the original)
inline void SetLayoutValue_unk(atn::D2A_Task* task, unsigned int id, float value) {
    fn_801FECE0(&task->mData.m10, id, value);
}
inline void SetLayoutValue1_unk(atn::D2A_Task* task, unsigned int id, int index, float value) {
    fn_801FED24(&task->mData.m10, id, index, value);
}
inline void SetLayoutValue2_unk(atn::D2A_Task* task, unsigned int id, int index, float value) {
    fn_801FED70(&task->mData.m10, id, index, value);
}
inline void SetLayoutValue3_unk(atn::D2A_Task* task, unsigned int id, int index, float value) {
    fn_801FEDBC(&task->mData.m10, id, index, value);
}
inline void SetLayoutInt_unk(atn::D2A_Task* task, unsigned int id, int value) {
    fn_801FEEFC(&task->mData.m10, id, value);
}

// inferred: d2anime manager (lbl_8087F138): the instance being drawn
struct D2AInstanceManager_unk {
    unsigned char m000[0x1AC];
    atn::ID2A_TaskInstance* mInstance; // 0x1AC
};
extern "C" D2AInstanceManager_unk* lbl_8087F138;

namespace atn {

ID2A_TaskInstance::Cache_unk* ID2A_TaskInstance::sCache_unk;

template <int N, int M>
ID2A_TaskInstance::Cache_unk* D2A_TaskInstanceBase<N, M>::GetCache_unk() {
    if (sCache_unk == 0) {
        sCache_unk = new ((MemoryTag)1, "", "", 0) Cache_unk;
    }
    return sCache_unk;
}

template <int N, int M>
D2A_Task* D2A_TaskInstanceBase<N, M>::Acquire_unk(Task* parent, const char* path) {
    Cache_unk* cache = GetCache_unk();
    unsigned int hash = fn_800DC6B4(path);
    for (unsigned int i = 0; i < cache->size(); i++) {
        if (hash == (*cache)[i].mHash) {
            (*cache)[i].mRef++;
            return (*cache)[i].mTask;
        }
    }
    CacheEntry_unk entry;
    entry.mHash = hash;
    entry.mTask = fn_801F3FF8(parent->m24, path, 0);
    entry.mTask->fn_800D246C(1);
    entry.mRef = 1;
    cache->push_back(entry);
    return entry.mTask;
}

template <int N, int M>
D2A_TaskInstanceBase<N, M>* D2A_TaskInstanceBase<N, M>::Create(Task* parent, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)8, "", "", 0) D2A_TaskInstanceBase(parent, path);
    }
    return 0;
}

template <int N, int M>
inline D2A_TaskInstanceBase<N, M>::D2A_TaskInstanceBase(Task* parent, const char* path)
    : ID2A_TaskInstance(parent), mTask(Acquire_unk(parent, path)), mPlaying(1), mLoop(1), mVisible(1), mFrame(0.0f),
      mSpeed(1.0f) {}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::Release_unk(D2A_TaskInstanceBase* inst) {
    Cache_unk* cache = GetCache_unk();
    D2A_Task* task = inst->mTask;
    for (unsigned int i = 0; i < cache->size(); i++) {
        if ((*cache)[i].mTask == task) {
            CacheEntry_unk& entry = (*cache)[i];
            if (--entry.mRef <= 0) {
                fn_800D2338(task);
                cache->erase(&entry);
            }
            return;
        }
    }
}

template <int N, int M>
D2A_TaskInstanceBase<N, M>::~D2A_TaskInstanceBase() {
    Release_unk(this);
}

template <int N, int M>
int D2A_TaskInstanceBase<N, M>::vf0C() {
    if (mTask->IsFlag_unk(2)) {
        mTask->fn_800D246C(0);
        mTask->mFlags |= 4;
        return 1;
    }
    return 0;
}

// NONMATCHING: 97.18% (all three) - Update inlined: the original loads mTask again after the
// frame update instead of reusing the value of the test
template <int N, int M>
void D2A_TaskInstanceBase<N, M>::vf10() {
    if (mVisible) {
        Update();
    }
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::vf14() {
    if (mVisible) {
        Draw();
    }
}

// inferred: GetData/GetEndFrame/GetTrack_unk only exist for <32, 8> (called by other files)
template <int N, int M>
D2A_Data* D2A_TaskInstanceBase<N, M>::GetData() {
    if (mTask != 0) {
        return &mTask->mData;
    }
    return 0;
}

template <int N, int M>
float D2A_TaskInstanceBase<N, M>::GetEndFrame() {
    return mTask->mData.mEndFrame;
}

// NONMATCHING (fn_801F69A8): 96.94% - same reload of mTask as vf10
template <int N, int M>
void D2A_TaskInstanceBase<N, M>::Update() {
    if (mTask != 0 && mPlaying) {
        mFrame += mSpeed;
        if (mFrame >= GetEndFrame()) {
            if (mLoop) {
                mFrame -= GetEndFrame();
            } else {
                mFrame = GetEndFrame();
            }
        }
        if (mFrame < 0.0f) {
            if (mLoop) {
                mFrame += GetEndFrame();
            } else {
                mFrame = 0.0f;
            }
        }
    }
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::Draw() {
    if (lbl_8087F138 != 0) {
        lbl_8087F138->mInstance = this;
    }
    Apply();
    fn_801F465C(mTask, 1);
    if (lbl_8087F138 != 0) {
        lbl_8087F138->mInstance = 0;
    }
}

template <int N, int M>
inline void* D2A_TaskInstanceBase<N, M>::GetLayout_unk() {
    return &mTask->mData.m10;
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::Apply() {
    mTask->mFrame = mFrame;
    for (unsigned int i = 0; i < mParams.size(); i++) {
        Param_unk& p = mParams[i];
        switch (p.mType) {
        case 0:
            SetLayoutValue_unk(mTask, p.mHash, p.mValue.f);
            break;
        case 1:
            SetLayoutValue1_unk(mTask, p.mHash, p.mIndex, p.mValue.f);
            break;
        case 2:
            SetLayoutValue2_unk(mTask, p.mHash, p.mIndex, p.mValue.f);
            break;
        case 3:
            SetLayoutValue3_unk(mTask, p.mHash, p.mIndex, p.mValue.f);
            break;
        case 5:
            SetLayoutInt_unk(mTask, p.mHash, p.mValue.i);
            break;
        }
    }
    for (unsigned int i = 0; i < mTexts.size(); i++) {
        fn_801FEE08(GetLayout_unk(), mTexts[i].mHash, mTexts[i].mText.data());
    }
}

template <int N, int M>
void* D2A_TaskInstanceBase<N, M>::GetTrack_unk(const char* name) {
    Apply();
    return fn_801FEB9C(&mTask->mData.m10, name);
}

// inferred: replaces the value of the same pane parameter or adds it (names invented)
template <int N, int M>
inline void AddParam_unk(D2A_TaskInstanceBase<N, M>* inst, const typename D2A_TaskInstanceBase<N, M>::Param_unk& p) {
    for (unsigned int i = 0; i < inst->mParams.size(); i++) {
        if (p.mType == inst->mParams[i].mType && p.mIndex == inst->mParams[i].mIndex &&
            p.mHash == inst->mParams[i].mHash) {
            inst->mParams[i].mValue.i = p.mValue.i;
            return;
        }
    }
    inst->mParams.push_back(p);
}

template <int N, int M>
inline void SetParam_unk(D2A_TaskInstanceBase<N, M>* inst, signed char type, signed char index, const char* name,
                         float value) {
    typename D2A_TaskInstanceBase<N, M>::Param_unk p;
    p.mType = type;
    p.mIndex = index;
    p.mHash = fn_800DC6B4(name);
    p.mValue.f = value;
    AddParam_unk(inst, p);
}

template <int N, int M>
inline void SetParam_unk(D2A_TaskInstanceBase<N, M>* inst, signed char type, signed char index, const char* name,
                         int value) {
    typename D2A_TaskInstanceBase<N, M>::Param_unk p;
    p.mType = type;
    p.mIndex = index;
    p.mHash = fn_800DC6B4(name);
    p.mValue.i = value;
    AddParam_unk(inst, p);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetValue0_unk(const char* name, float value) {
    SetParam_unk(this, 0, 0, name, value);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetValue1_unk(const char* name, int index, float value) {
    SetParam_unk(this, 1, index, name, value);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetValue3_unk(const char* name, int index, float value) {
    SetParam_unk(this, 3, index, name, value);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetValue2_unk(const char* name, int index, float value) {
    SetParam_unk(this, 2, index, name, value);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetValues1_unk(const char* name, D2A_DrawRect r) {
    SetValue1_unk(name, 0, r.x);
    SetValue1_unk(name, 1, r.y);
    SetValue1_unk(name, 2, r.w);
    SetValue1_unk(name, 3, r.h);
    SetValue1_unk(name, 4, r.z);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetValues1Rect_unk(const char* name, D2A_DrawRect r) {
    SetValue1_unk(name, 0, r.x);
    SetValue1_unk(name, 1, r.y);
    SetValue1_unk(name, 4, r.z);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetValues2_unk(const char* name, D2A_DrawColor c) {
    SetValue2_unk(name, 0, c.r);
    SetValue2_unk(name, 1, c.g);
    SetValue2_unk(name, 2, c.b);
    SetValue2_unk(name, 3, c.a);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetColor_unk(const char* name, unsigned int color) {
    SetValue3_unk(name, 0, (float)(color >> 24));
    SetValue3_unk(name, 1, (float)((color >> 16) & 0xFF));
    SetValue3_unk(name, 2, (float)((color >> 8) & 0xFF));
    SetValue3_unk(name, 3, (float)(color & 0xFF));
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetColor_unk(const char* name, float r, float g, float b) {
    SetValue3_unk(name, 1, r);
    SetValue3_unk(name, 2, g);
    SetValue3_unk(name, 3, b);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetColor255_unk(const char* name, D2A_RGB_unk rgb) {
    SetValue3_unk(name, 1, 255.0f * rgb.r);
    SetValue3_unk(name, 2, 255.0f * rgb.g);
    SetValue3_unk(name, 3, 255.0f * rgb.b);
}

// inferred: replaces the text of the same pane or adds it (name invented)
template <int N, int M>
inline void AddText_unk(D2A_TaskInstanceBase<N, M>* inst, const typename D2A_TaskInstanceBase<N, M>::Text_unk& t) {
    for (unsigned int i = 0; i < inst->mTexts.size(); i++) {
        if (t.mHash == inst->mTexts[i].mHash) {
            inst->mTexts[i].mText = t.mText;
            return;
        }
    }
    inst->mTexts.push_back(t);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetText_unk(const char* name, const wchar_t* text) {
    Text_unk t;
    t.mHash = fn_800DC6B4(name);
    t.mText = text;
    AddText_unk(this, t);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetNumber_unk(const char* name, int value, int digits) {
    // inferred: each wide literal of the original pool ends with two terminators
    wchar_t text[0x100];
    wchar_t format[0x20];
    if (digits > 0) {
        fn_800DD3FC(format, L"%%%dd\0", digits);
    } else if (digits < 0) {
        fn_800DD3FC(format, L"%%0%dd\0", -digits);
    } else {
        fn_800DD3FC(format, L"%%d\0");
    }
    fn_800DD3FC(text, format, value);
    SetText_unk(name, text);
}

template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetInt_unk(const char* name, int value) {
    SetParam_unk(this, 5, 0, name, value);
}

template <int N, int M>
D2A_DrawRect D2A_TaskInstanceBase<N, M>::GetRect_unk(const char* name) {
    D2A_DrawRect rect;
    if (GetData() == 0) {
        return rect;
    }
    Apply();
    mTask->ApplyFrame();
    D2A_Common* element = GetData()->fn_801EFA78(fn_800DC6B4(name));
    if (element != 0) {
        D2A_DrawColor color;
        D2A_DrawExtra extra;
        int work[6];
        element->fn_801ED928(&rect, &color, &extra, work);
    }
    return rect;
}

// inferred: removed by the linker (its format L"%f\0" is left in the wide string pool)
template <int N, int M>
void D2A_TaskInstanceBase<N, M>::SetFloat_unk(const char* name, float value) {
    wchar_t text[0x100];
    fn_800DD3FC(text, L"%f\0", value);
    SetText_unk(name, text);
}

template D2A_TaskInstanceBase<64, 8>* D2A_TaskInstanceBase<64, 8>::Create(Task* parent, const char* path);
template D2A_TaskInstanceBase<64, 8>::~D2A_TaskInstanceBase();
template int D2A_TaskInstanceBase<64, 8>::vf0C();
template void D2A_TaskInstanceBase<64, 8>::vf10();
template void D2A_TaskInstanceBase<64, 8>::vf14();
template void D2A_TaskInstanceBase<64, 8>::Apply();
template void D2A_TaskInstanceBase<64, 8>::SetValue0_unk(const char* name, float value);
template void D2A_TaskInstanceBase<64, 8>::SetValue1_unk(const char* name, int index, float value);
template void D2A_TaskInstanceBase<64, 8>::SetValues2_unk(const char* name, D2A_DrawColor c);
template void D2A_TaskInstanceBase<64, 8>::SetColor255_unk(const char* name, D2A_RGB_unk rgb);
template void D2A_TaskInstanceBase<64, 8>::SetText_unk(const char* name, const wchar_t* text);
template void D2A_TaskInstanceBase<64, 8>::SetNumber_unk(const char* name, int value, int digits);
template void D2A_TaskInstanceBase<64, 8>::SetInt_unk(const char* name, int value);

} // namespace atn

// The members of D2A_TaskInstanceBase<32, 8> that other files call keep their automatic names
// (extern "C" wrappers around the inlined member; the callers still use these names).
typedef atn::D2A_TaskInstanceBase<32, 8> D2A_TaskInstance32_unk;

D2A_TaskInstance32_unk* fn_801F64D0(atn::Task* parent, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)8, "", "", 0) D2A_TaskInstance32_unk(parent, path);
    }
    return 0;
}

template atn::D2A_TaskInstanceBase<32, 8>::~D2A_TaskInstanceBase();
template int atn::D2A_TaskInstanceBase<32, 8>::vf0C();
template void atn::D2A_TaskInstanceBase<32, 8>::vf10();
template void atn::D2A_TaskInstanceBase<32, 8>::vf14();

void fn_801F69A8(D2A_TaskInstance32_unk* inst) {
    inst->Update();
}

void fn_801F6A38(D2A_TaskInstance32_unk* inst) {
    inst->Draw();
}

template void atn::D2A_TaskInstanceBase<32, 8>::Apply();

atn::D2A_Data* fn_801F6C10(D2A_TaskInstance32_unk* inst) {
    return inst->GetData();
}

float fn_801F6C2C(D2A_TaskInstance32_unk* inst) {
    return inst->GetEndFrame();
}

void* fn_801F6C38(D2A_TaskInstance32_unk* inst, const char* name) {
    return inst->GetTrack_unk(name);
}

void fn_801F6C80(D2A_TaskInstance32_unk* inst, const char* name, float value) {
    inst->SetValue0_unk(name, value);
}

void fn_801F6D7C(D2A_TaskInstance32_unk* inst, const char* name, int index, float value) {
    inst->SetValue1_unk(name, index, value);
}

void fn_801F6E78(D2A_TaskInstance32_unk* inst, const char* name, atn::D2A_DrawRect r) {
    inst->SetValue1_unk(name, 0, r.x);
    inst->SetValue1_unk(name, 1, r.y);
    inst->SetValue1_unk(name, 2, r.w);
    inst->SetValue1_unk(name, 3, r.h);
    inst->SetValue1_unk(name, 4, r.z);
}

template void atn::D2A_TaskInstanceBase<32, 8>::SetValues1Rect_unk(const char* name, D2A_DrawRect r);

void fn_801F7590(D2A_TaskInstance32_unk* inst, const char* name, atn::D2A_DrawColor c) {
    inst->SetValue2_unk(name, 0, c.r);
    inst->SetValue2_unk(name, 1, c.g);
    inst->SetValue2_unk(name, 2, c.b);
    inst->SetValue2_unk(name, 3, c.a);
}

void fn_801F791C(D2A_TaskInstance32_unk* inst, const char* name, int index, float value) {
    inst->SetValue3_unk(name, index, value);
}

template void atn::D2A_TaskInstanceBase<32, 8>::SetColor_unk(const char* name, unsigned int color);

void fn_801F7DF0(D2A_TaskInstance32_unk* inst, const char* name, float r, float g, float b) {
    inst->SetValue3_unk(name, 1, r);
    inst->SetValue3_unk(name, 2, g);
    inst->SetValue3_unk(name, 3, b);
}

template void atn::D2A_TaskInstanceBase<32, 8>::SetColor255_unk(const char* name, D2A_RGB_unk rgb);

void fn_801F837C(D2A_TaskInstance32_unk* inst, const char* name, const wchar_t* text) {
    D2A_TaskInstance32_unk::Text_unk t;
    t.mHash = fn_800DC6B4(name);
    t.mText = text;
    atn::AddText_unk(inst, t);
}

void fn_801F8598(D2A_TaskInstance32_unk* inst, const char* name, int value, int digits) {
    // inferred: each wide literal of the original pool ends with two terminators
    wchar_t text[0x100];
    wchar_t format[0x20];
    if (digits > 0) {
        fn_800DD3FC(format, L"%%%dd\0", digits);
    } else if (digits < 0) {
        fn_800DD3FC(format, L"%%0%dd\0", -digits);
    } else {
        fn_800DD3FC(format, L"%%d\0");
    }
    fn_800DD3FC(text, format, value);
    fn_801F837C(inst, name, text);
}

template atn::D2A_DrawRect atn::D2A_TaskInstanceBase<32, 8>::GetRect_unk(const char* name);

namespace atn {

template D2A_TaskInstanceBase<8, 2>* D2A_TaskInstanceBase<8, 2>::Create(Task* parent, const char* path);
template D2A_TaskInstanceBase<8, 2>::~D2A_TaskInstanceBase();
template int D2A_TaskInstanceBase<8, 2>::vf0C();
template void D2A_TaskInstanceBase<8, 2>::vf10();
template void D2A_TaskInstanceBase<8, 2>::vf14();
template void D2A_TaskInstanceBase<8, 2>::Apply();
template void D2A_TaskInstanceBase<8, 2>::SetValue0_unk(const char* name, float value);
template void D2A_TaskInstanceBase<8, 2>::SetValue1_unk(const char* name, int index, float value);
template void D2A_TaskInstanceBase<8, 2>::SetNumber_unk(const char* name, int value, int digits);
template void D2A_TaskInstanceBase<8, 2>::SetFloat_unk(const char* name, float value);

} // namespace atn
