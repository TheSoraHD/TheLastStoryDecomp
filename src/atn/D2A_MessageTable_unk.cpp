#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "atn/Csv.h"
#include "std/vector.h"
#include "std/string_members.h"
#include "atn/D2A_MessageTable_unk.h"

// Table of texts by id of the d2anime menus: a UTF-16 CSV (id, text) read with atn::CsvU into one
// text buffer. Names invented (file between atn_d2a_message and atn_d2a_reference).

extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int (atn/Util_unk.cpp)

// inferred: line of the CSV while reading (0x10 bytes)
struct D2A_TextLine_unk {
    D2A_TextLine_unk() : mId(0) {}
    int mId;
    std::basic_string<wchar_t> mText;
};

namespace std {
// inferred: the lines are moved as raw bytes when push_back reallocates and the buffer destroys
// its elements before releasing them (same as the wstring buffer of game/MenuCommonTask.cpp)
template <>
struct __vec_split_unk<D2A_TextLine_unk> : public __vec_storage_unk<D2A_TextLine_unk> {
    enum { __clear_src_unk = 1 }; // inferred: see std/vector.h
    typedef D2A_TextLine_unk T;
    __vec_split_unk(void* alloc) : mAlloc(alloc), mStart(0) {}
    ~__vec_split_unk() {
        T* b = begin();
        T* p = b + this->mSize;
        while (p > b) {
            (--p)->~T();
        }
        this->mSize = 0;
    }
    T* begin() { return this->mData + mStart; }
    void push_back(const T& x) {
        new (begin() + this->mSize) T(x);
        this->mSize++;
    }
    void __append_copies_unk(unsigned int n, const T& x) {
        for (; n != 0; n--) {
            push_back(x);
        }
    }
    void __push_front_unk(T* first, T* last) {
        int n = last - first;
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(T));
        memset(first, 0, n * sizeof(T));
        this->mSize += n;
    }
    void* mAlloc;
    unsigned int mStart;
};
} // namespace std

// inferred: names invented

D2A_MessageTable_unk::D2A_MessageTable_unk() : mState(0), mText(0) {}

D2A_MessageTable_unk::~D2A_MessageTable_unk() {
    mEntries.Clear();
    if (mText != 0) {
        delete[] mText;
        mText = 0;
    }
}

extern "C" void fn_801F36C8(D2A_MessageTable_unk* table, const wchar_t* text, int length);

// inferred: loads the table; `wait` loads synchronously (the text is then not read here)
extern "C" void fn_801F35B4(D2A_MessageTable_unk* table, const char* path, int wait) {
    if (wait != 0) {
        table->mLoader.fn_80473FCC(path);
        if (table->mLoader.fn_80470580() != 0) {
            table->mState = 2;
        } else {
            table->mState = 0;
        }
    } else {
        table->mState = 1;
        table->mLoader.vf0C(path);
    }
}

// inferred: reads the table when its file is loaded; nonzero while loading
extern "C" int fn_801F362C(D2A_MessageTable_unk* table) {
    if (table->mState == 1) {
        if (table->mLoader.fn_80473F50() != 0) {
            return 1;
        }
        if (table->mLoader.fn_80470580() != 0) {
            unsigned int length = (unsigned int)table->mLoader.fn_8047059C() / 2;
            fn_801F36C8(table, (const wchar_t*)table->mLoader.fn_80470580(), length - 1);
        }
        table->mLoader.fn_80473F88();
        table->mState = 2;
    }
    return 0;
}

// NONMATCHING: 94.57% - register allocation (the vector size/capacity in memory in push_back, the
// other saved registers shifted); the order of the basic_string messages in the pool also differs.
// The CsvU address is in a saved register as in the original (empty ~CsvBase())
// inferred: reads the CSV text (two header lines, then "id,text"; '#', ';' and empty lines are
// skipped) into one buffer of texts
extern "C" void fn_801F36C8(D2A_MessageTable_unk* table, const wchar_t* text, int length) {
    table->mEntries.Clear();
    if (table->mText != 0) {
        delete[] table->mText;
        table->mText = 0;
    }
    std::vector<D2A_TextLine_unk> lines;
    int total = 0;
    atn::CsvU csv(text, length);
    csv.fn_8005BBF8();
    csv.fn_8005BBF8();
    do {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == L'#' || *field == 0 || *field == L';') {
            continue;
        }
        D2A_TextLine_unk line;
        line.mId = fn_800DC1DC(field);
        line.mText = csv.fn_8005B9CC();
        total += line.mText.size() + 1;
        lines.push_back(line);
    } while (csv.fn_8005BBF8());
    table->mEntries.Alloc(lines.size());
    table->mText = new ((MemoryTag)8, "", "", 0) wchar_t[total];
    int pos = 0;
    for (unsigned int i = 0; i < table->mEntries.mCount; i++) {
        table->mEntries.mEntries[i].mId = lines[i].mId;
        table->mEntries.mEntries[i].mText = table->mText + pos;
        wcscpy(table->mText + pos, lines[i].mText.c_str());
        pos += lines[i].mText.size() + 1;
    }
}

// inferred: copies the text of `id` to `out`; 0 if there is none
extern "C" int fn_801F3E20(D2A_MessageTable_unk* table, std::basic_string<wchar_t>* out, unsigned int id) {
    for (unsigned int i = 0; i < table->mEntries.mCount; i++) {
        if (id == table->mEntries.mEntries[i].mId) {
            *out = table->mEntries.mEntries[i].mText;
            return 1;
        }
    }
    return 0;
}
