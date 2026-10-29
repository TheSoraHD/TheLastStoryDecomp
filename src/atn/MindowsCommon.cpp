#include "atn/MindowsCommon.h"
#include "atn/Keyboard.h"
#include "atn/Memory.h"
#include <stdio.h>

// atn/MindowsCommon.cpp: debug menu items (atn::MindowsCommon and subclasses) and the menu itself.

using namespace atn;

extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void* lbl_8087EEB0;               // inferred: debug drawing
extern "C" void fn_80060D58(void* draw, float x, float y, float z, float w, unsigned int color); // inferred: rectangle
extern "C" void fn_800616C0(void* draw, const char* text, unsigned int color, int a, int b, int c, float x, float y,
                            float z, float sx, float sy, float f6); // inferred: text

atn::Mindows_unk* lbl_8087EF10;

extern "C" void fn_8008510C(char* buf);

// inferred: a key of the keyboard is pressed
static inline int IsKey_unk(unsigned short key) {
    return Keyboard::Order()->Press(Keyboard::PORT4_unk, key);
}

// inferred: appends `c` when its key or its keypad key is pressed
static inline void AddKey_unk(char* buf, char c) {
    if (IsKey_unk(c) || IsKey_unk(0xF100 + c)) {
        if (strlen(buf) < 0x3E) {
            char* p = buf + strlen(buf);
            p[0] = c;
            p[1] = 0;
        }
    }
}

namespace atn {

MindowsCommon::~MindowsCommon() {}

void MindowsInt::vf0C() {
    mEdit[0] = 0;
}

void MindowsInt::vf10() {
    if (strlen(mEdit) != 0) {
        *mValue = fn_800DC12C(mEdit);
    }
    if (*mValue < mMin) {
        *mValue = mMin;
    } else if (*mValue > mMax) {
        *mValue = mMax;
    }
    if (mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsInt::vf14() {
    fn_8008510C(mEdit);
    if (m4C != 0 && mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

} // namespace atn

// inferred: decimal number typed with the keyboard
extern "C" void fn_8008510C(char* buf) {
    AddKey_unk(buf, '0');
    AddKey_unk(buf, '1');
    AddKey_unk(buf, '2');
    AddKey_unk(buf, '3');
    AddKey_unk(buf, '4');
    AddKey_unk(buf, '5');
    AddKey_unk(buf, '6');
    AddKey_unk(buf, '7');
    AddKey_unk(buf, '8');
    AddKey_unk(buf, '9');
    AddKey_unk(buf, '.');
    AddKey_unk(buf, '-');
    if (IsKey_unk(0xF1AE) || IsKey_unk(0xF1C8)) {
        if (strlen(buf) != 0) {
            char* p = buf + strlen(buf);
            p[-1] = 0;
        }
    }
}

namespace atn {

// The draw functions (vf18) are NONMATCHING (90-97%): float register allocation of the line
// position (16.0f in f5 and the int->float constant in f1 in the original) and the order of the
// sprintf arguments.
#define MINDOWS_DRAW_BEGIN(ctx, line, color)                                                                       \
    if (*line < ctx->mBottom) {                                                                               \
        if (ctx->mTop <= *line) {                                                                             \
            float y = 16.0f * (float)(*line - ctx->mTop) + ctx->mY;                                           \
            if (ctx->mCursor == *line) {                                                                      \
                fn_80060D58(lbl_8087EEB0, ctx->mX, y, -1000000.0f, ctx->mW,                                     \
                            color);                                                                                                                                            \
            }                                                                                                 \
            fn_800616C0(lbl_8087EEB0, mName, -1, 0, 0, 0, 16.0f * (float)depth + ctx->mX, y, -1000000.0f,     \
                        16.0f, 16.0f, 0.0f);

#define MINDOWS_DRAW_VALUE(text)                                                                              \
    fn_800616C0(lbl_8087EEB0, text, -1, 0, 0, 0, (ctx->mX + ctx->mW) - 8.0f * (float)strlen(text), y,        \
                -1000000.0f, 16.0f, 16.0f, 0.0f)

#define MINDOWS_DRAW_END(line)                                                                                \
        }                                                                                                     \
        (*line)++;                                                                                            \
    }

void MindowsInt::vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) {
    MINDOWS_DRAW_BEGIN(ctx, line, ctx->mEditing ? 0xFF808000 : 0xFF8080FF)
    if (ctx->mCursor == *line && ctx->mEditing != 0) {
        MINDOWS_DRAW_VALUE(mEdit);
    } else {
        char buf[32];
        sprintf(buf, "%d", *mValue);
        MINDOWS_DRAW_VALUE(buf);
    }
    MINDOWS_DRAW_END(line)
}

void MindowsUInt::vf0C() {
    mEdit[0] = 0;
}

void MindowsUInt::vf10() {
    if (strlen(mEdit) != 0) {
        sscanf(mEdit, "%x", mValue);
    }
    if (*mValue < mMin) {
        *mValue = mMin;
    } else if (*mValue > mMax) {
        *mValue = mMax;
    }
    if (mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

// inferred: appends `c` when one of two keys is pressed (no length check)
#define MINDOWS_HEX_KEY(k1, k2, c)                                                                            \
    if (IsKey_unk(k1) || IsKey_unk(k2)) {                                                                     \
        char* p = mEdit + strlen(mEdit);                                                                      \
        p[0] = c;                                                                                             \
        p[1] = 0;                                                                                             \
    }

void MindowsUInt::vf14() {
    MINDOWS_HEX_KEY('0', 0xF130, '0')
    MINDOWS_HEX_KEY('1', 0xF131, '1')
    MINDOWS_HEX_KEY('2', 0xF132, '2')
    MINDOWS_HEX_KEY('3', 0xF133, '3')
    MINDOWS_HEX_KEY('4', 0xF134, '4')
    MINDOWS_HEX_KEY('5', 0xF135, '5')
    MINDOWS_HEX_KEY('6', 0xF136, '6')
    MINDOWS_HEX_KEY('7', 0xF137, '7')
    MINDOWS_HEX_KEY('8', 0xF138, '8')
    MINDOWS_HEX_KEY('9', 0xF139, '9')
    MINDOWS_HEX_KEY('A', 'a', 'a')
    MINDOWS_HEX_KEY('B', 'b', 'b')
    MINDOWS_HEX_KEY('C', 'c', 'c')
    MINDOWS_HEX_KEY('D', 'd', 'd')
    MINDOWS_HEX_KEY('E', 'e', 'e')
    MINDOWS_HEX_KEY('F', 'f', 'f')
    if (IsKey_unk(0xF1AE) || IsKey_unk(0xF1C8)) {
        char* p = mEdit + strlen(mEdit);
        p[-1] = 0;
    }
    if (m4C != 0 && mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsUInt::vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) {
    MINDOWS_DRAW_BEGIN(ctx, line, ctx->mEditing ? 0xFF808000 : 0xFF8080FF)
    if (ctx->mCursor == *line && ctx->mEditing != 0) {
        MINDOWS_DRAW_VALUE(mEdit);
    } else {
        char buf[32];
        sprintf(buf, "%08x", *mValue);
        MINDOWS_DRAW_VALUE(buf);
    }
    MINDOWS_DRAW_END(line)
}

void MindowsFloat::vf0C() {
    mEdit[0] = 0;
}

void MindowsFloat::vf10() {
    if (strlen(mEdit) != 0) {
        *mValue = fn_800DC288(mEdit);
    }
    if (*mValue < mMin) {
        *mValue = mMin;
    } else if (*mValue > mMax) {
        *mValue = mMax;
    }
    if (mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsFloat::vf14() {
    fn_8008510C(mEdit);
    if (m4C != 0 && mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsFloat::vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) {
    MINDOWS_DRAW_BEGIN(ctx, line, ctx->mEditing ? 0xFF808000 : 0xFF8080FF)
    if (ctx->mCursor == *line && ctx->mEditing != 0) {
        MINDOWS_DRAW_VALUE(mEdit);
    } else {
        char buf[32];
        sprintf(buf, "%.4f", *mValue);
        MINDOWS_DRAW_VALUE(buf);
    }
    MINDOWS_DRAW_END(line)
}

void MindowsRadian::vf0C() {
    mEdit[0] = 0;
}

void MindowsRadian::vf10() {
    if (strlen(mEdit) != 0) {
        *mValue = 0.017453292f * fn_800DC288(mEdit);
    }
    if (*mValue < mMin) {
        *mValue = mMin;
    } else if (*mValue > mMax) {
        *mValue = mMax;
    }
    if (mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsRadian::vf14() {
    fn_8008510C(mEdit);
    if (m4C != 0 && mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsRadian::vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) {
    MINDOWS_DRAW_BEGIN(ctx, line, ctx->mEditing ? 0xFF808000 : 0xFF8080FF)
    if (ctx->mCursor == *line && ctx->mEditing != 0) {
        MINDOWS_DRAW_VALUE(mEdit);
    } else {
        char buf[32];
        sprintf(buf, "%.4f", 57.295776f * *mValue);
        MINDOWS_DRAW_VALUE(buf);
    }
    MINDOWS_DRAW_END(line)
}

void MindowsBool::vf14() {
    *mValue = *mValue == 0;
    if (m4C != 0 && mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsBool::vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) {
    MINDOWS_DRAW_BEGIN(ctx, line, 0xFF8080FF)
    char buf[32];
    sprintf(buf, "%s", *mValue ? "TRUE" : "FALSE");
    MINDOWS_DRAW_VALUE(buf);
    MINDOWS_DRAW_END(line)
}

void MindowsFlag::vf14() {
    if (mMask == (mMask & *mValue)) {
        *mValue &= ~mMask;
    } else {
        *mValue |= mMask;
    }
    if (m4C != 0 && mFunc != 0) {
        mFunc(mArg, &mValue);
    }
}

void MindowsFlag::vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) {
    MINDOWS_DRAW_BEGIN(ctx, line, 0xFF8080FF)
    char buf[32];
    sprintf(buf, "%s", mMask == (mMask & *mValue) ? "TRUE" : "FALSE");
    MINDOWS_DRAW_VALUE(buf);
    MINDOWS_DRAW_END(line)
}

MindowsFolder::~MindowsFolder() {
    MindowsCommon* item = mFirst;
    while (item != 0) {
        MindowsCommon* next = item->mNext;
        delete item;
        item = next;
    }
}

void MindowsFolder::vf14() {
    mOpen = mOpen == 0;
}

void MindowsFolder::vf18(const MindowsDrawCtx_unk* ctx, int depth, int* line) {
    if (*line < ctx->mBottom) {
        if (ctx->mTop <= *line) {
            float y = 16.0f * (float)(*line - ctx->mTop) + ctx->mY;
            if (ctx->mCursor == *line) {
                fn_80060D58(lbl_8087EEB0, ctx->mX, y, -1000000.0f, ctx->mW, 0xFF8080FF);
            }
            char buf[32];
            sprintf(buf, "%c%s", mOpen ? '-' : '+', mName);
            fn_800616C0(lbl_8087EEB0, buf, -1, 0, 0, 0, 16.0f * (float)depth + ctx->mX, y, -1000000.0f, 16.0f,
                        16.0f, 0.0f);
        }
        (*line)++;
        if (mOpen != 0) {
            for (MindowsCommon* item = mFirst; item != 0; item = item->mNext) {
                item->vf18(ctx, depth + 1, line);
            }
        }
    }
}

} // namespace atn

extern "C" void fn_800874C8(MindowsFolder* self, const char* name, int* value, int min, int max, int step, MindowsFunc_unk func,
                           void* arg) {
    MindowsInt* item = new ((MemoryTag)0xE, "", "", 0) MindowsInt(self, name, value, min, max, step);
    if (item != 0) {
        item->vf2C(func, arg);
        self->Append_unk(item);
    }
}

extern "C" void fn_800875F8(MindowsFolder* self, const char* name, unsigned int* value, unsigned int min, unsigned int max,
                            unsigned int step, MindowsFunc_unk func, void* arg) {
    MindowsUInt* item = new ((MemoryTag)0xE, "", "", 0) MindowsUInt(self, name, value, min, max, step);
    if (item != 0) {
        item->vf2C(func, arg);
        self->Append_unk(item);
    }
}

extern "C" void fn_8008771C(MindowsFolder* self, const char* name, float* value, float min, float max, float step, MindowsFunc_unk func,
                             void* arg) {
    MindowsFloat* item = new ((MemoryTag)0xE, "", "", 0) MindowsFloat(self, name, value, min, max, step);
    if (item != 0) {
        item->vf2C(func, arg);
        self->Append_unk(item);
    }
}

extern "C" void fn_80087858(MindowsFolder* self, const char* name, float* value, float min, float max, float step,
                              MindowsFunc_unk func, void* arg) {
    MindowsRadian* item = new ((MemoryTag)0xE, "", "", 0) MindowsRadian(self, name, value, min, max, step);
    if (item != 0) {
        item->vf2C(func, arg);
        self->Append_unk(item);
    }
}

extern "C" void fn_80087994(MindowsFolder* self, const char* name, int* value, MindowsFunc_unk func, void* arg) {
    MindowsBool* item = new ((MemoryTag)0xE, "", "", 0) MindowsBool(self, name, value);
    if (item != 0) {
        item->vf2C(func, arg);
        self->Append_unk(item);
    }
}

extern "C" void fn_80087AA0(MindowsFolder* self, const char* name, unsigned int* value, unsigned int mask, MindowsFunc_unk func,
                            void* arg) {
    MindowsFlag* item = new ((MemoryTag)0xE, "", "", 0) MindowsFlag(self, name, value, mask);
    if (item != 0) {
        item->vf2C(func, arg);
        self->Append_unk(item);
    }
}

extern "C" void fn_80087BB4(MindowsFolder* self, const char* name, float* value, float min, float max, float step, MindowsFunc_unk func,
                            void* arg) {
    MindowsFolder* folder = new ((MemoryTag)0xE, "", "", 0) MindowsFolder(self, name);
    if (folder != 0) {
        fn_8008771C(folder, "x", value, min, max, step, func, arg);
        fn_8008771C(folder, "y", value + 1, min, max, step, func, arg);
        self->Append_unk(folder);
    }
}

extern "C" void fn_80087E9C(MindowsFolder* self, const char* name, float* value, float min, float max, float step, MindowsFunc_unk func,
                            void* arg) {
    MindowsFolder* folder = new ((MemoryTag)0xE, "", "", 0) MindowsFolder(self, name);
    if (folder != 0) {
        fn_8008771C(folder, "x", value, min, max, step, func, arg);
        fn_8008771C(folder, "y", value + 1, min, max, step, func, arg);
        fn_8008771C(folder, "z", value + 2, min, max, step, func, arg);
        self->Append_unk(folder);
    }
}

extern "C" void fn_8008826C(MindowsFolder* self, const char* name, float* value, float min, float max, float step, MindowsFunc_unk func,
                            void* arg) {
    MindowsFolder* folder = new ((MemoryTag)0xE, "", "", 0) MindowsFolder(self, name);
    if (folder != 0) {
        fn_8008771C(folder, "x", value, min, max, step, func, arg);
        fn_8008771C(folder, "y", value + 1, min, max, step, func, arg);
        fn_8008771C(folder, "z", value + 2, min, max, step, func, arg);
        fn_8008771C(folder, "w", value + 3, min, max, step, func, arg);
        self->Append_unk(folder);
    }
}

extern "C" void fn_80088724(MindowsFolder* self, const char* name, float* value, float min, float max, float step,
                                  MindowsFunc_unk func, void* arg) {
    MindowsFolder* folder = new ((MemoryTag)0xE, "", "", 0) MindowsFolder(self, name);
    if (folder != 0) {
        fn_80087858(folder, "x", value, min, max, step, func, arg);
        fn_80087858(folder, "y", value + 1, min, max, step, func, arg);
        fn_80087858(folder, "z", value + 2, min, max, step, func, arg);
        self->Append_unk(folder);
    }
}

extern "C" void fn_80088AF4(MindowsFolder* self, const char* name, float* value, float min, float max, float step,
                              MindowsFunc_unk func, void* arg) {
    MindowsFolder* folder = new ((MemoryTag)0xE, "", "", 0) MindowsFolder(self, name);
    if (folder != 0) {
        fn_8008771C(folder, "R", value, min, max, step, func, arg);
        fn_8008771C(folder, "G", value + 1, min, max, step, func, arg);
        fn_8008771C(folder, "B", value + 2, min, max, step, func, arg);
        fn_8008771C(folder, "A", value + 3, min, max, step, func, arg);
        self->Append_unk(folder);
    }
}

extern "C" void fn_80088FAC(MindowsFolder* self, const char* name, float* value, float min, float max, float step,
                              MindowsFunc_unk func, void* arg) {
    MindowsFolder* folder = new ((MemoryTag)0xE, "", "", 0) MindowsFolder(self, name);
    if (folder != 0) {
        fn_8008771C(folder, "R", value, min, max, step, func, arg);
        fn_8008771C(folder, "G", value + 1, min, max, step, func, arg);
        fn_8008771C(folder, "B", value + 2, min, max, step, func, arg);
        self->Append_unk(folder);
    }
}

extern "C" MindowsFolder* fn_8008937C(MindowsFolder* self, const char* name) {
    MindowsFolder* folder = new ((MemoryTag)0xE, "", "", 0) MindowsFolder(self, name);
    if (folder != 0) {
        self->Append_unk(folder);
        return folder;
    }
    return &lbl_8087EF10->mTemp;
}

namespace atn {

MindowsCommon* MindowsFolder::vf20(int index, int* counter) {
    if (index == *counter) {
        return this;
    }
    (*counter)++;
    if (mOpen != 0) {
        for (MindowsCommon* item = mFirst; item != 0; item = item->mNext) {
            MindowsCommon* r = item->vf20(index, counter);
            if (r != 0) {
                return r;
            }
        }
    }
    return 0;
}

int MindowsFolder::vf1C() const {
    int n = 1;
    if (mOpen != 0) {
        for (MindowsCommon* item = mFirst; item != 0; item = item->mNext) {
            n += item->vf1C();
        }
    }
    return n;
}

int MindowsFolder::vf24(MindowsCommon* child) {
    MindowsCommon* prev = 0;
    for (MindowsCommon* item = mFirst; item != 0; item = item->mNext) {
        if (item == child) {
            if (prev == 0) {
                mFirst = item->mNext;
            } else {
                prev->mNext = item->mNext;
            }
            if (item == mLast) {
                mLast = prev;
            }
            return 1;
        }
        prev = item;
    }
    return 0;
}

} // namespace atn

namespace atn {

Mindows_unk::Mindows_unk(void* owner)
    : mOwner(owner), m04(0), m08(0), m0C(0), mRoot(0, "Mindows Root"), mTemp(0, "Mindows Temp") {
    mD8 = 340.0f;
    mDC = 40.0f;
    mE0 = 260.0f;
    mE4 = 160.0f;
    mE8 = 0;
    mEC = 0;
    mF0 = 0;
    mRoot.vf14();
}

} // namespace atn

// inferred: creates the debug menu
extern "C" void fn_80089620(void* owner) {
    if (lbl_8087EF10 == 0) {
        lbl_8087EF10 = new ((MemoryTag)1, "", "", 0) Mindows_unk(owner);
    }
}

// inferred: removes an item from its folder and deletes it
extern "C" void fn_800897D8(Mindows_unk* self, MindowsCommon* item) {
    if (item->mParent != 0 && &self->mTemp != item) {
        item->mParent->vf24(item);
        delete item;
    }
}
