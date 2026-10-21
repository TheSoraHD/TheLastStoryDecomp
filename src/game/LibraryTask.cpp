#include "game/LibraryTask.h"
#include "atn/LoadHandle.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "std/string_members.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char* strcat(char* dst, const char* src);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path); // inferred: creates a layout anime
extern "C" void fn_804A39EC(atn::D2A_Task* anime, int arg1, int repeat, float frame, float speed);
extern "C" void fn_804A3A68(MenuLayout_unk* layout, int arg1, int repeat, float frame, float speed);
extern "C" void fn_804A4494(int* cursor, int* top, int count, int page, int arg4, int sound);
extern "C" void fn_804A4930(MenuLayout_unk* layout, int pos, int count, int page);
extern "C" void fn_804A55FC(MenuCommonTask* menu, MenuLayout_unk* layout, int arg2, const atn::D2A_DrawRect* rect,
                            int arg4);
extern "C" void fn_800C3094(const char* group); // inferred: loads a sound group
extern "C" void fn_800C3124(const char* group); // inferred: releases a sound group
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
extern "C" atn::SoundRef fn_80117228(int id);
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
extern "C" void fn_801F6E78(MenuLayout_unk* layout, const char* name, const atn::D2A_DrawRect* rect);
extern "C" void fn_801F6C80(MenuLayout_unk* layout, const char* name, float value);
extern "C" void fn_801F837C(MenuLayout_unk* layout, const char* name, const wchar_t* text);
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width
extern "C" void fn_8006F420(void* font, char* out, const wchar_t* in, int size); // inferred: wide to multibyte
// inferred: word wrap of a text
extern "C" std::basic_string<wchar_t> fn_800DFB40(const wchar_t* text, float width, float size, float f3, int a,
                                                   int b, int c, int d);
extern "C" void fn_800D5908(atn::Texture_unk* tex, const char* path); // inferred: loads a texture
// inferred: the same function is named as a member of GimmickUvOwner_unk elsewhere
struct GimmickUvOwner_unk {
    int fn_800D59B8(); // inferred: nonzero while loading
};

// inferred: event flags of the field
struct LibraryField_unk;
extern "C" LibraryField_unk* lbl_8087F430;
extern "C" int fn_80370174(LibraryField_unk* field, int id);
extern "C" void fn_80370320(LibraryField_unk* field, int id, int value, int arg3);
extern "C" void fn_8036EA3C(LibraryField_unk* field); // inferred: leaves the library

// inferred: debug file watcher (names invented)
struct LibraryWatch_unk {
    unsigned char m000[0x34];
    unsigned char m034[0xD90 - 0x34];
    int mD90;
};
extern "C" LibraryWatch_unk* lbl_8087EEF0;
extern "C" int fn_8007708C(LibraryWatch_unk* watch, void* name, int size); // inferred: file changed

// inferred: parameters of the text (debug menu "Library")
extern "C" float lbl_8087E030; // FontW
extern "C" float lbl_8087E034; // FontH
extern "C" float lbl_8087E038; // FontWindowW
extern "C" int lbl_8087E03C;   // LineNum
extern "C" float lbl_8087E040; // LineSpace
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

static const char* const sSeTable[] = {"SE_EV1605_001", "SE_EV1605_003", "SE_EV1605_002"};

inline void SetLayoutValue_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetLayoutValueAt_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetLayoutText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline bool IsEnd_unk(atn::D2A_Task* task) {
    float last = task->mData.mEndFrame;
    return task->mFrame >= last;
}
inline atn::D2A_DrawRect GetPanePos_unk(atn::D2A_Task* task, const char* name) {
    return fn_801F4E8C(task, fn_800DC6B4(name));
}

LibraryTask* LibraryTask::sInstance;

LibraryTask* LibraryTask::fn_80468F54(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) LibraryTask(parent);
    }
    return sInstance;
}

// NONMATCHING: 99.69% - registers of the list path and the loop offset swapped (r30/r31)
LibraryTask::LibraryTask(atn::Task* parent) : atn::Task(parent), mPrevState(2), mState(2), mCursor(0), mTop(0),
    mBook(0) {
    m50 = fn_801F3FF8(this, "/data/d2anime/library_logo.d2a", 0);
    m50->fn_800D246C(1);
    m54 = fn_801F3FF8(this, "/data/d2anime/library_list_base.d2a", 0);
    m54->fn_800D246C(1);
    const char* list = "/data/d2anime/library_list.d2a";
    for (int i = 0; i < 10; i++) {
        MenuLayout_unk* item = (MenuLayout_unk*)fn_801F64D0(this, list);
        m58[i] = item;
        item->fn_800D246C(1);
    }
    m84 = fn_801F3FF8(this, "/data/d2anime/library_arrow_l.d2a", 0);
    m84->fn_800D246C(1);
    m88 = fn_801F3FF8(this, "/data/d2anime/library_arrow_r.d2a", 0);
    m88->fn_800D246C(1);
    m80 = (MenuLayout_unk*)fn_801F64D0(this, "scroll_bar.d2a");
    m80->fn_800D246C(1);
    mFile.vf0C("database/library/book_table_xx.u16");
    fn_800C3094("GROUP_EV1605");
}

LibraryTask::~LibraryTask() {
    if (sInstance != 0) {
        sInstance = 0;
    }
    fn_800C3124("GROUP_EV1605");
}

int LibraryTask::vf0C() {
    if (mFile.fn_80473F50()) {
        return 0;
    }
    if (!fn_800D3FA4()) {
        return 0;
    }
    fn_80469D60(mFile.fn_80470580(), mFile.fn_8047059C(), 0);
    fn_804A39EC(m50, 1, 0, 0.0f, 1.0f);
    m50->mFlags |= 4;
    fn_804A39EC(m54, 1, 0, 0.0f, 1.0f);
    fn_804A39EC(m84, 1, 0, 0.0f, -1.0f);
    fn_804A39EC(m88, 1, 0, 0.0f, -1.0f);
    fn_804A3A68(m80, 1, 0, 0.0f, 1.0f);
    m54->mFlags |= 4;
    for (int i = 0; i < 10; i++) {
        fn_804A3A68(m58[i], 1, 0, 0.0f, 1.0f);
        m58[i]->mFlags |= 4;
    }
    void* menu = mMenu.Load("Library");
    fn_8008771C(menu, "FontW", &lbl_8087E030, 0, 0, -1000.0f, 1000.0f, 0.1f);
    fn_8008771C(menu, "FontH", &lbl_8087E034, 0, 0, -1000.0f, 1000.0f, 0.1f);
    fn_8008771C(menu, "FontWindowW", &lbl_8087E038, 0, 0, -1000.0f, 1000.0f, 0.1f);
    fn_800874C8(menu, "LineNum", &lbl_8087E03C, 0, 10000, 1, 0, 0);
    fn_8008771C(menu, "LineSpace", &lbl_8087E040, 0, 0, -1000.0f, 1000.0f, 0.1f);
    return 1;
}

void LibraryTask::vf10() {
    LibraryWatch_unk* watch = lbl_8087EEF0;
    if (watch != 0) {
        int changed = 0;
        if (watch->mD90 != 0 && fn_8007708C(watch, watch->m034, 0x34) != 0) {
            changed = 1;
        }
        if (changed) {
            fn_80469D60(mFile.fn_80470580(), mFile.fn_8047059C(), 1);
        }
    }
    switch (mState) {
    case 2: {
        fn_804A4494(&mCursor, &mTop, mBooks.size(), 10, 1, 3);
        void* pad = lbl_8087EF70;
        if (fn_800A555C(pad, 0, 4)) {
            fn_800C31F4(sSeTable[0], 0, -1, 1.0f);
            vf1C(3);
        } else if (fn_800A555C(pad, 0, 5)) {
            fn_80117228(2);
            vf1C(1);
        }
        break;
    }
    case 3:
        if (mBook == 0 || mBook->mState == 5) {
            if (mBook != 0) {
                fn_800D2338(mBook);
                mBook = 0;
            }
            vf1C(2);
        }
        break;
    }
}

// NONMATCHING: 99.51% - the layout temporary of the inline and the flash counter swap registers
void LibraryTask::vf14() {
    m50->mFlags |= 4;
    m54->mFlags |= 4;
    for (int i = 0; i < 10; i++) {
        m58[i]->mFlags |= 4;
    }
    m80->mFlags |= 4;
    m84->mSpeed = -2.0f;
    m88->mSpeed = -2.0f;
    if (mBook != 0) {
        int l = mBook->mFlashL;
        int r = mBook->mFlashR;
        if (l > 7) {
            SetLayoutValue_unk(&m84->mData.m10, "flash", (10 - l) / 3.0f);
        } else if (l < 3) {
            SetLayoutValue_unk(&m84->mData.m10, "flash", l / 3.0f);
        } else {
            SetLayoutValue_unk(&m84->mData.m10, "flash", 1.0f);
        }
        if (r > 7) {
            SetLayoutValue_unk(&m88->mData.m10, "flash", (10 - r) / 3.0f);
        } else if (r < 3) {
            SetLayoutValue_unk(&m88->mData.m10, "flash", r / 3.0f);
        } else {
            SetLayoutValue_unk(&m88->mData.m10, "flash", 1.0f);
        }
    } else {
        SetLayoutValue_unk(&m84->mData.m10, "flash", 0.0f);
        SetLayoutValue_unk(&m88->mData.m10, "flash", 0.0f);
    }
    switch (mState) {
    case 2:
        fn_80469AB8();
        break;
    case 3:
        fn_80469C64();
        break;
    }
}

void LibraryTask::vf1C(int state) {
    mPrevState = mState;
    mState = state;
    switch (state) {
    case 1:
        if (lbl_8087F430 != 0) {
            fn_8036EA3C(lbl_8087F430);
        }
        break;
    case 3:
        if (mCursor < (int)mBooks.size()) {
            LibraryBook_unk* book = &mBooks[mCursor];
            if (mBook != 0) {
                fn_800D2338(mBook);
                mBook = 0;
            }
            mBook = new (1, "", "", 0) BookTask(this, book->mName, book->mTitle);
            if (fn_80370174(lbl_8087F430, book->mId) == 1) {
                fn_80370320(lbl_8087F430, book->mId, 2, 0);
            }
        } else {
            vf1C(2);
        }
        break;
    }
}

void LibraryTask::fn_80469AB8() {
    m50->mFlags &= ~4;
    fn_8046A20C();
    int cursor = mCursor;
    if (mBooks.size() != 0) {
        if (IsEnd_unk(m50)) {
            char name[0x40];
            sprintf(name, "list_cursor%d", cursor - mTop + 1);
            atn::D2A_DrawRect rect = GetPanePos_unk(m54, name);
            fn_804A55FC(MenuCommonTask::sInstance, m58[mCursor - mTop], 0, &rect, 0);
        }
        atn::D2A_DrawRect pos = GetPanePos_unk(m54, "sb_null");
        fn_801F6E78(m80, "basePos", &pos);
        fn_801F6C80(m80, "s_hightMax", 250.0f);
        m80->mFlags &= ~4;
        fn_804A4930(m80, mTop, mBooks.size(), 10);
    }
    m50->mSpeed = 1.0f;
    for (int i = 0; i < 10; i++) {
        m58[i]->mSpeed = 1.0f;
    }
}

void LibraryTask::fn_80469C64() {
    m50->mFlags &= ~4;
    fn_8046A20C();
    m50->mSpeed = -1.0f;
    for (int i = 0; i < 10; i++) {
        m58[i]->mSpeed = -1.0f;
    }
    BookTask* book = mBook;
    if (book != 0) {
        if (!book->IsBusy_unk()) {
            int count = book->mPages.mSize;
            int page = book->mPage;
            if (page < count - 1) {
                m88->mSpeed = 2.0f;
            }
            if (page > 0) {
                m84->mSpeed = 2.0f;
            }
        }
    }
}

// NONMATCHING: 98.9% - push_back loads the size before the capacity (see std/vector.h) and the
// registers of the book pointer and the string pool shift; the CsvU address is now in a saved
// register as in the original (empty ~CsvBase())
void LibraryTask::fn_80469D60(const void* data, unsigned int size, int unlock) {
    mBooks.__shrink_unk(0);
    atn::CsvU csv((const wchar_t*)data, size / 2);
    do {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == L';' || *field == L'#') {
            continue;
        }
        LibraryBook_unk book;
        book.mId = fn_800DC1DC(field);
        if (book.mId != 0 && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, book.mId) == 0) {
            if (unlock == 0) {
                continue;
            }
            fn_80370320(lbl_8087F430, book.mId, 1, 0);
        }
        void* font = lbl_8087EEC8;
        fn_8006F420(font, book.mName, csv.fn_8005B9CC(), 0x10);
        wcscpy(book.mTitle, csv.fn_8005B9CC());
        mBooks.push_back(book);
    } while (csv.fn_8005BBF8());
}

void LibraryTask::fn_8046A20C() {
    m54->mFlags &= ~4;
    atn::D2A_DrawRect pos;
    int isNew;
    for (int i = 0; i < 10; i++) {
        char name[0x40];
        sprintf(name, "list_point%d", i + 1);
        pos = GetPanePos_unk(m54, name);
        fn_801F6E78(m58[i], "basePos", &pos);
        int index = i + mTop;
        if (index < (int)mBooks.size()) {
            LibraryBook_unk* book = &mBooks[index];
            isNew = book->mId > 0 && fn_80370174(lbl_8087F430, book->mId) == 1;
            fn_801F837C(m58[i], "bookName", book->mTitle);
            fn_801F6C80(m58[i], "newIcon", isNew ? 1.0f : 0.0f);
            m58[i]->mFlags &= ~4;
        }
    }
}

static const char* const sBookD2A[] = {"library_book_list2.d2a", "library_book_list.d2a", "library_book_pic.d2a"};

BookTask::BookTask(atn::Task* parent, const char* name, const wchar_t* title)
    : atn::Task(parent), mPrevState(0), mState(0), mLoad(0), mFade(0.0f), mTextFade(0.0f), mFlashL(0), mFlashR(0),
      mPage(0) {
    m50 = fn_801F3FF8(this, "/data/d2anime/library_book_base.d2a", 0);
    m50->fn_800D246C(1);
    for (int i = 0; i < 3; i++) {
        m54[i] = fn_801F3FF8(this, sBookD2A[i], 0);
        m54[i]->fn_800D246C(1);
        m60[i] = fn_801F3FF8(this, sBookD2A[i], 0);
        m60[i]->fn_800D246C(1);
    }
    mTitle = title;
    char path[0x100];
    sprintf(path, "database/library/%s_xx.u16", name);
    mFile.vf0C(path);
}

LibraryPage_unk::~LibraryPage_unk() {}

BookTask::~BookTask() {
    mPages.Free_unk();
}

int BookTask::vf0C() {
    if (mLoad == 0) {
        if (mFile.fn_80473F50()) {
            return 0;
        }
        if (!fn_800D3FA4()) {
            return 0;
        }
        fn_8046B798(mFile.fn_80470580(), mFile.fn_8047059C());
        fn_804A39EC(m50, 1, 0, 0.0f, 1.0f);
        SetLayoutValue_unk(&m50->mData.m10, "fade", 0.0f);
        m50->mFlags |= 4;
        for (int i = 0; i < 3; i++) {
            fn_804A39EC(m54[i], 1, 0, 0.0f, 1.0f);
            m54[i]->mFlags |= 4;
            fn_804A39EC(m60[i], 1, 0, 0.0f, 1.0f);
            m60[i]->mFlags |= 4;
        }
        mLoad = 1;
    } else if (mLoad == 1) {
        for (unsigned int i = 0; i < mPages.mSize; i++) {
            if (((GimmickUvOwner_unk*)&mPages.mData[i].mPicture)->fn_800D59B8()) {
                return 0;
            }
        }
        LibraryPage_unk* page = &mPages[0];
        if (page->mLeft == 2) {
            m54[2]->fn_801F4E30("picture", (int)((atn::LoadHandle*)(&page->mPicture.m20))->fn_80473F18());
        }
        if (page->mRight == 2) {
            m60[2]->fn_801F4E30("picture", (int)((atn::LoadHandle*)(&page->mPicture.m20))->fn_80473F18());
        }
        mLoad = 2;
        return 1;
    }
    return 0;
}

void BookTask::vf10() {
    int l = mFlashL - 1;
    mFlashL = l < 0 ? 0 : l;
    int r = mFlashR - 1;
    mFlashR = r < 0 ? 0 : r;
    if (mState == 1) {
        fn_8046ACDC();
    }
    fn_8046A9D0();
}

void BookTask::vf14() {
    m50->mFlags |= 4;
    for (int i = 0; i < 3; i++) {
        m54[i]->mFlags |= 4;
        m60[i]->mFlags |= 4;
    }
    fn_8046AE7C();
}

void BookTask::vf1C(int state) {
    mPrevState = mState;
    mState = state;
    switch (state) {
    case 3:
        mFlashR = 10;
        m50->mFrame = 0.0f;
        break;
    case 4:
        mFlashL = 10;
        m50->mFrame = m50->mData.mEndFrame;
        break;
    }
}

inline float LibMin_unk(float a, float b) {
    return (b < a) ? b : a;
}
inline float LibMax_unk(float a, float b) {
    return (b > a) ? b : a;
}

void BookTask::fn_8046A9D0() {
    switch (mState) {
    case 0:
        m50->mFrame = m50->mData.mEndFrame;
        m50->mSpeed = 0.0f;
        mFade = LibMin_unk(mFade + 0.1f, 1.0f);
        if (mFade >= 1.0f) {
            vf1C(1);
        }
        break;
    case 2:
        m50->mFrame = m50->mData.mEndFrame;
        m50->mSpeed = 0.0f;
        mFade = LibMax_unk(mFade - 0.1f, 0.0f);
        if (mFade <= 0.0f) {
            vf1C(5);
        }
        break;
    case 1:
        m50->mFrame = m50->mData.mEndFrame;
        m50->mSpeed = 0.0f;
        mFade = 1.0f;
        break;
    case 3:
        m50->mSpeed = 2.0f;
        if (IsEnd_unk(m50)) {
            vf1C(1);
        }
        break;
    case 4:
        m50->mSpeed = -2.0f;
        if (m50->mFrame <= 0.0f) {
            vf1C(1);
        }
        break;
    }
    int state = mState;
    switch (mState) {
    case 0:
    case 1:
    case 2:
        mTextFade = mFade;
        break;
    case 3:
    case 4: {
        float frame = m50->mFrame;
        if (frame < 6.0f) {
            mTextFade = 1.0f - frame / 6.0f;
        } else if ((float)__fabs(frame - 16.0f) < 1.1920929e-07f) {
            mTextFade = 0.0f;
            if (state == 3) {
                mPage++;
            } else if (state == 4) {
                mPage--;
            }
            LibraryPage_unk* page = &mPages[mPage];
            if (page->mLeft == 2) {
                m54[2]->fn_801F4E30("picture", (int)((atn::LoadHandle*)(&page->mPicture.m20))->fn_80473F18());
            }
            if (page->mRight == 2) {
                m60[2]->fn_801F4E30("picture", (int)((atn::LoadHandle*)(&page->mPicture.m20))->fn_80473F18());
            }
        } else if (frame > m50->mData.mEndFrame - 6.0f) {
            mTextFade = 1.0f - (m50->mData.mEndFrame - frame) / 6.0f;
        } else {
            mTextFade = 0.0f;
        }
        break;
    }
    }
}

void BookTask::fn_8046ACDC() {
    void* pad = lbl_8087EF70;
    if (fn_800A555C(pad, 0, 5)) {
        fn_800C31F4(sSeTable[1], 0, -1, 1.0f);
        vf1C(2);
    } else if (fn_800A555C(pad, 0, 0x18) || fn_800A555C(pad, 0, 3)) {
        if (mPage + 1 < mPages.mSize) {
            fn_800C31F4(sSeTable[2], 0, -1, 1.0f);
            vf1C(3);
        }
    } else if (fn_800A555C(pad, 0, 0x17) || fn_800A555C(pad, 0, 2)) {
        if (mPage - 1 >= 0) {
            fn_800C31F4(sSeTable[2], 0, -1, 1.0f);
            vf1C(4);
        }
    }
}

// NONMATCHING: 98.89% - registers of the layout/text temporaries of the inlines swapped, 20.0f
// loaded first, stack slots of the iterator tags
void BookTask::fn_8046AE7C() {
    m50->mFlags &= ~4;
    float width = fn_8006EF48(lbl_8087EEC8, mTitle.c_str(), 20.0f, 0.0f, 1, 1);
    SetLayoutValue_unk(&m50->mData.m10, "strLength", width);
    SetLayoutValue_unk(&m50->mData.m10, "fade", mFade);
    SetLayoutText_unk(&m50->mData.m10, "bookName", mTitle.c_str());
    m50->SetNumber_unk("page", mPage + 1, 0);
    m50->SetNumber_unk("pageSdw", mPage + 1, 0);
    m50->SetNumber_unk("pageMax", mPages.mSize, 0);
    m50->SetNumber_unk("pageMaxSdw", mPages.mSize, 0);
    atn::D2A_DrawRect left;
    atn::D2A_DrawRect right;
    left = GetPanePos_unk(m50, "page_l");
    right = GetPanePos_unk(m50, "page_r");
    for (int i = 0; i < 3; i++) {
        SetLayoutValue_unk(&m54[i]->mData.m10, "fade", mTextFade);
        fn_801F4728(m54[i], "basePos", (const float*)&left);
        SetLayoutValue_unk(&m60[i]->mData.m10, "fade", mTextFade);
        fn_801F4728(m60[i], "basePos", (const float*)&right);
    }
    LibraryPage_unk* page = &mPages[mPage];
    m54[page->mLeft]->mFlags &= ~4;
    m60[page->mRight]->mFlags &= ~4;
    std::basic_string<wchar_t> text =
        fn_800DFB40(page->mText.c_str(), lbl_8087E038, lbl_8087E030, 0.0f, 1, 1, 0, 1);
    std::basic_string<wchar_t> lines[3];
    int count = 0;
    int side = 0;
    unsigned int pos = 0;
    while (side < 2) {
        unsigned int end = text.find(L'\n', pos);
        if (end != std::basic_string<wchar_t>::npos) {
            lines[side] += std::basic_string<wchar_t>(text, pos, end + 1 - pos);
            pos = end + 1;
            if (++count >= lbl_8087E03C) {
                count = 0;
                side++;
            }
        } else {
            lines[side] += std::basic_string<wchar_t>(text, pos);
            break;
        }
    }
    if (page->mLeft != 2) {
        SetLayoutText_unk(&m54[0]->mData.m10, "text", lines[0].c_str());
        SetLayoutText_unk(&m54[1]->mData.m10, "text", lines[0].c_str());
        SetLayoutText_unk(&m60[0]->mData.m10, "text", lines[1].c_str());
        SetLayoutText_unk(&m60[1]->mData.m10, "text", lines[1].c_str());
    } else {
        SetLayoutText_unk(&m54[0]->mData.m10, "text", lines[0].c_str());
        SetLayoutText_unk(&m54[1]->mData.m10, "text", lines[0].c_str());
        SetLayoutText_unk(&m60[0]->mData.m10, "text", lines[0].c_str());
        SetLayoutText_unk(&m60[1]->mData.m10, "text", lines[0].c_str());
    }
    SetLayoutValueAt_unk(&m54[0]->mData.m10, "textPos", 2, lbl_8087E030);
    SetLayoutValueAt_unk(&m54[0]->mData.m10, "textPos", 3, lbl_8087E034);
    SetLayoutValueAt_unk(&m54[1]->mData.m10, "textPos", 2, lbl_8087E030);
    SetLayoutValueAt_unk(&m54[1]->mData.m10, "textPos", 3, lbl_8087E034);
    SetLayoutValueAt_unk(&m60[0]->mData.m10, "textPos", 2, lbl_8087E030);
    SetLayoutValueAt_unk(&m60[0]->mData.m10, "textPos", 3, lbl_8087E034);
    SetLayoutValueAt_unk(&m60[1]->mData.m10, "textPos", 2, lbl_8087E030);
    SetLayoutValueAt_unk(&m60[1]->mData.m10, "textPos", 3, lbl_8087E034);
    m54[0]->fn_801F4DDC("textObj", lbl_8087E040);
    m54[1]->fn_801F4DDC("textObj", lbl_8087E040);
    m60[0]->fn_801F4DDC("textObj", lbl_8087E040);
    m60[1]->fn_801F4DDC("textObj", lbl_8087E040);
}

// NONMATCHING: 99.1% - offset of ".texture" in the pool (the original emits the basic_string
// strings in another order) and the register of the page counter; the CsvU address is now in a
// saved register as in the original (empty ~CsvBase())
void BookTask::fn_8046B798(const void* data, unsigned int size) {
    mPages.Free_unk();
    atn::CsvU csv((const wchar_t*)data, size / 2);
    int count = 0;
    do {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field != L'#' && *field != L';') {
            count++;
        }
    } while (csv.fn_8005BBF8());
    mPages.Resize_unk(count, 0);
    csv.atn::CsvU::vf08((const wchar_t*)data, size / 2);
    int i = 0;
    do {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == L'#' || *field == L';') {
            continue;
        }
        LibraryPage_unk* page = &mPages[i];
        page->m00 = fn_800DC1DC(field);
        page->mLeft = fn_800DC1DC(csv.fn_8005B9CC());
        page->mRight = fn_800DC1DC(csv.fn_8005B9CC());
        char path[0x100];
        memset(path, 0, sizeof(path));
        void* font = lbl_8087EEC8;
        fn_8006F420(font, path, csv.fn_8005B9CC(), 0x100);
        if (path[0] != 0) {
            strcat(path, ".texture");
            fn_800D5908(&page->mPicture, path);
        }
        page->m3C = csv.fn_8005B9CC();
        page->mText = std::basic_string<wchar_t>(csv.fn_8005B9CC());
        i++;
    } while (csv.fn_8005BBF8());
}

LibraryPage_unk::LibraryPage_unk() {}
