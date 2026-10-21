#ifndef GAME_LIBRARYTASK_H
#define GAME_LIBRARYTASK_H

#include "atn/Array_unk.h"
#include "atn/D2A_Task.h"
#include "atn/LH_Binary.h"
#include "atn/Task.h"
#include "atn/Texture_unk.h"
#include "game/MenuCommonTask.h"
#include "game/ResHandle.h"
#include "std/string.h"
#include "std/vector.h"

// inferred: entry of the book list (0x98 bytes, read from book_table_xx.u16); names invented
struct LibraryBook_unk {
    int mId;            // 0x00: inferred: event flag of the book (0: always present)
    char mName[0x10];   // 0x04: inferred: name of the book file
    wchar_t mTitle[0x40]; // 0x14
    int m94;
};

namespace std {
// inferred: the books are stored by assignment, the size being incremented first, as for pointer
// elements (0x80469D60); moving them into a new buffer still copy-constructs them
template <>
struct __vec_imp_unk<LibraryBook_unk> : public __vec_storage_unk<LibraryBook_unk> {
    ~__vec_imp_unk() {}
    void __push_fast_unk(unsigned int n, const LibraryBook_unk& x) {
        this->mSize = n + 1;
        this->mData[this->mSize - 1] = x;
    }
};
template <>
struct __vec_split_unk<LibraryBook_unk> : public __vec_split_base_unk<LibraryBook_unk> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<LibraryBook_unk>(alloc), mStart(0) {}
    LibraryBook_unk* begin() { return this->mData + mStart; }
    void push_back(const LibraryBook_unk& x) {
        begin()[this->mSize] = x;
        this->mSize++;
    }
    void __push_front_unk(LibraryBook_unk* first, LibraryBook_unk* last) {
        LibraryBook_unk* p = begin();
        while (last > first) {
            new (--p) LibraryBook_unk(*--last);
            mStart--;
            this->mSize++;
        }
    }
    unsigned int mStart;
};
} // namespace std

// inferred: page of a book (0x54 bytes, read from <book>_xx.u16); names invented
struct LibraryPage_unk {
    LibraryPage_unk();  // 0x8046BB60
    ~LibraryPage_unk(); // 0x8046A514
    int m00;
    int mLeft;                       // 0x04: inferred: kind of the left page (2: picture)
    int mRight;                      // 0x08: inferred: kind of the right page (2: picture)
    atn::Texture_unk mPicture;       // 0x0C
    std::basic_string<wchar_t> m3C;
    std::basic_string<wchar_t> mText; // 0x48
};

// inferred from __vt__8BookTask (0x8078FAB0): reader of one book
class BookTask : public atn::Task {
public:
    BookTask(atn::Task* parent, const char* name, const wchar_t* title); // 0x8046A370
    virtual ~BookTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(int state); // inferred: sets the state

    // inferred: opening or closing (name invented)
    bool IsBusy_unk() const { return mState == 2 || mState == 5; }
    void fn_8046A9D0(); // inferred: animation of the page
    void fn_8046ACDC(); // inferred: input
    void fn_8046AE7C(); // inferred: draws the page
    void fn_8046B798(const void* data, unsigned int size); // inferred: reads the pages

    atn::LH_Binary mFile;          // 0x48
    atn::D2A_Task* m50;            // inferred: library_book_base.d2a
    atn::D2A_Task* m54[3];         // inferred: left page (library_book_list2/list/pic)
    atn::D2A_Task* m60[3];         // inferred: right page
    int mPrevState;                // 0x6C
    int mState;                    // 0x70: inferred: 0 open, 1 idle, 2 close, 3 next, 4 previous, 5 closed
    int mLoad;                     // 0x74
    float mFade;                   // 0x78
    float mTextFade;               // 0x7C
    int mFlashL;                   // 0x80: inferred: flash of the left arrow (LibraryTask::vf14)
    int mFlashR;                   // 0x84
    std::basic_string<wchar_t> mTitle; // 0x88
    int mPage;                     // 0x94
    atn::Buffer_unk<LibraryPage_unk> mPages; // 0x98
};

// inferred from __vt__11LibraryTask (0x8078FAF0): book list of the library; singleton
class LibraryTask : public atn::Task {
public:
    static LibraryTask* fn_80468F54(atn::Task* parent); // inferred: creates the instance
    LibraryTask(atn::Task* parent);
    virtual ~LibraryTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(int state); // inferred: sets the state

    void fn_80469AB8(); // inferred: draws the list
    void fn_80469C64(); // inferred: draws the list behind a book
    void fn_80469D60(const void* data, unsigned int size, int unlock); // inferred: reads the list
    void fn_8046A20C(); // inferred: updates the entries

    static LibraryTask* sInstance;

    atn::LH_Binary mFile;            // 0x48
    atn::D2A_Task* m50;              // inferred: library_logo.d2a
    atn::D2A_Task* m54;              // inferred: library_list_base.d2a
    MenuLayout_unk* m58[10];         // inferred: library_list.d2a
    MenuLayout_unk* m80;             // inferred: scroll_bar.d2a
    atn::D2A_Task* m84;              // inferred: library_arrow_l.d2a
    atn::D2A_Task* m88;              // inferred: library_arrow_r.d2a
    int mPrevState;                  // 0x8C
    int mState;                      // 0x90: inferred: 1 end, 2 list, 3 reading
    int mCursor;                     // 0x94
    int mTop;                        // 0x98
    std::vector<LibraryBook_unk> mBooks; // 0x9C
    BookTask* mBook;                 // 0xA8
    ResHandle mMenu;                 // 0xAC: inferred: debug menu group
};

#endif
