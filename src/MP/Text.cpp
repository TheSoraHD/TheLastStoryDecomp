// MP::Text (RTTI "MP::Text", vtable 0x80792DC0): texts of the online mode read from
// "/database/online/message_table_xx.u16", plus the tables of the online mode read by the packet
// code (atn/PacketPack): rewards per rank ("begin"/"PVP"/"RAID"/"end") and titles/points
// ("title"/"point"). Names of the table structs invented.
#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/FixedArray.h"
#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "atn/Task.h"

extern "C" int atoi(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf

extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text (shared by the text tables)

namespace atn {

// inferred: pointer to a string owned elsewhere (see atn/ptr_string_base_wchar_t_0.cpp)
template <class T, int N>
class ptr_string_base {
public:
    ptr_string_base() : mPtr(0) {}
    virtual ~ptr_string_base() {}
    const T* c_str() const { return mPtr != 0 ? mPtr : lbl_808813D0; }

    T* mPtr;
};

} // namespace atn

namespace MP {

class Text : public atn::Task {
public:
    static Text* fn_80505DD0(atn::Task* parent); // inferred: creates the instance
    Text(atn::Task* parent);
    virtual ~Text();
    virtual int vf0C();
    void fn_80505FC4(const wchar_t* data, unsigned long size); // inferred: reads the texts

    atn::ptr_string_base<wchar_t, 0> mTexts[0x139]; // 0x48
    atn::LH_Binary mLoader;                          // 0xA10
};

} // namespace MP

extern "C" MP::Text* lbl_8087F86C; // inferred: instance
extern "C" wchar_t* lbl_8087F868;  // inferred: buffer of all the texts

static const char* const sPath = "/database/online/message_table_xx.u16";

namespace MP {

inline Text::Text(atn::Task* parent) : atn::Task(parent) {
    mLoader.vf0C(sPath);
}

// NONMATCHING: 99.7% - the empty name of new is a separate 4-byte .rodata object (0x8075B034), not
// the "" of @stringBase0
Text* Text::fn_80505DD0(atn::Task* parent) {
    if (lbl_8087F86C == 0) {
        lbl_8087F86C = new (1, "", "", 0) Text(parent);
    }
    return lbl_8087F86C;
}

Text::~Text() {
    lbl_8087F86C = 0;
    if (lbl_8087F868 != 0) {
        delete[] lbl_8087F868;
        lbl_8087F868 = 0;
    }
}

int Text::vf0C() {
    if (!mLoader.fn_80473F50()) {
        fn_80505FC4((const wchar_t*)mLoader.fn_80470580(), mLoader.fn_8047059C());
        return 1;
    }
    return 0;
}

// NONMATCHING: 92.6% - the original keeps the half size in r30 and reuses the size register for the
// total (one saved register less), and tests the size with bne/b; same empty-name object as above
void Text::fn_80505FC4(const wchar_t* data, unsigned long size) {
    if (data == 0) {
        return;
    } else if (size == 0) {
        return;
    }
    atn::CsvU csv;
    csv.SetData(data, size / 2);
    unsigned long total = 0;
    while (csv.fn_8005BBF8()) {
        const wchar_t* f = csv.fn_8005B9CC();
        if (f[0] == 0 || f[0] == L';' || f[0] == L'#') {
            continue;
        }
        total += wcslen(csv.fn_8005B9CC()) + 1;
    }
    lbl_8087F868 = new (1, "", "", 0) wchar_t[total];
    unsigned long pos = 0;
    int i = 0;
    csv.SetData(data, size / 2);
    while (csv.fn_8005BBF8()) {
        const wchar_t* f = csv.fn_8005B9CC();
        if (f[0] == 0 || f[0] == L';' || f[0] == L'#') {
            continue;
        }
        const wchar_t* s = csv.fn_8005B9CC();
        fn_800DD3FC(lbl_8087F868 + pos, L"%s", s);
        mTexts[i].mPtr = lbl_8087F868 + pos;
        i++;
        pos += wcslen(s) + 1;
        if (i >= 0x139) {
            break;
        }
    }
}

} // namespace MP

// ---------------------------------------------------------------------------------------------
// inferred: rewards of the online mode ("begin" <id> ... "end" rows, "PVP"/"RAID" lists)

// inferred: row of the reward table (0x104 bytes)
// inferred: 32 values (a struct: the row copy copies each array with its own loop)
struct MpRewardValues_unk {
    int m[32];
};
struct MpRewardRow_unk {
    int mId;
    MpRewardValues_unk m04;
    MpRewardValues_unk m84; // inferred: weights (10 when not positive)
};

struct MpRewardTable_unk {
    atn::FixedArray_unk<MpRewardRow_unk, 32> mRows; // 0x0000
    atn::FixedArray_unk<int, 16> mPvp[6];           // 0x2084
    atn::FixedArray_unk<int, 16> mRaid[6];          // 0x221C
};

// NONMATCHING: 93.2% - string pool offsets (the pool of the original has no "" first but one
// after "RAID", used by fn_80506D94, not decompiled) and register allocation
extern "C" void fn_805061C8(MpRewardTable_unk* table, const char* data, int size) {
    atn::Csv csv(data, size);
    while (csv.fn_8005BBF8()) {
        const char* f = csv.fn_8005B9CC();
        if (f[0] == ';') {
            continue;
        }
        if (strcmp(f, "begin") != 0) {
            continue;
        }
        const char* name = csv.fn_8005B9CC();
        if (strcmp(name, "PVP") == 0) {
            int k = 0;
            while (csv.fn_8005BBF8()) {
                const char* f = csv.fn_8005B9CC();
                if (f[0] == ';') {
                    continue;
                }
                if (strcmp(f, "end") == 0 || k >= 6) {
                    break;
                }
                for (;;) {
                    int v = atoi(csv.fn_8005B9CC());
                    if (v <= 0 || table->mPvp[k].size() >= 16) {
                        break;
                    }
                    table->mPvp[k].push_back(v);
                }
                k++;
            }
        } else if (strcmp(name, "RAID") == 0) {
            int k = 0;
            while (csv.fn_8005BBF8()) {
                const char* f = csv.fn_8005B9CC();
                if (f[0] == ';') {
                    continue;
                }
                if (strcmp(f, "end") == 0 || k >= 6) {
                    break;
                }
                for (;;) {
                    int v = atoi(csv.fn_8005B9CC());
                    if (v <= 0 || table->mRaid[k].size() >= 16) {
                        break;
                    }
                    table->mRaid[k].push_back(v);
                }
                k++;
            }
        } else {
            MpRewardRow_unk row;
            memset(&row, 0, sizeof(row));
            row.mId = atoi(name);
            int k = 0;
            while (csv.fn_8005BBF8()) {
                const char* f = csv.fn_8005B9CC();
                if (f[0] == ';') {
                    continue;
                }
                if (strcmp(f, "end") == 0 || k >= 32) {
                    break;
                }
                row.m04.m[k] = atoi(csv.fn_8005B9CC());
                row.m84.m[k] = atoi(csv.fn_8005B9CC());
                if (row.m84.m[k] <= 0) {
                    row.m84.m[k] = 10;
                }
                k++;
            }
            table->mRows.push_back(row);
        }
    }
}

// inferred: row of the reward table by id
extern "C" MpRewardRow_unk* fn_8050652C(MpRewardTable_unk* table, int id) {
    MpRewardRow_unk* row = table->mRows.mData;
    for (unsigned int i = 0; i < table->mRows.mSize; i++, row++) {
        if (row->mId == id) {
            return row;
        }
    }
    return 0;
}

// inferred: pair of values of a reward row (both > 0 when set)
struct MpRewardPair_unk {
    int m00;
    int m04;
    int m08;
};

extern "C" int rand();

// inferred: shuffles the values of the 16 rows of `pairs` (fields +4/+8, stopping at the first one
// that is not positive) into `out`; returns 0 if there were none. fn_8050655C (not decompiled)
// is between this function and fn_8050652C in the original.
// NONMATCHING: 94.5% - the original loads each value once into r5 before the size check and keeps
// an unused counter in r7; registers of the erase loop (tried value locals, index/pointer loops,
// pair struct forms: 10 attempts)
extern "C" int fn_80506C10(atn::FixedArray_unk<int, 16>* out, MpRewardPair_unk* pairs) {
    atn::FixedArray_unk<int, 16> values;
    for (int i = 0; i < 16; ++i, pairs = (MpRewardPair_unk*)((char*)pairs + 8)) {
        if (pairs->m04 <= 0) {
            break;
        }
        if (values.size() < 16) {
            values.push_back(pairs->m04);
        }
        if (pairs->m08 <= 0) {
            break;
        }
        if (values.size() < 16) {
            values.push_back(pairs->m08);
        }
    }
    if (values.size() == 0) {
        return 0;
    }
    out->clear();
    unsigned int n = values.size();
    for (unsigned int i = 0; i < n; i++) {
        unsigned int k = rand() % values.size();
        if (out->size() < 16) {
            out->push_back(values[k]);
        }
        int* it = &values[k];
        for (unsigned int j = (it - &values[0]); j < values.size() - 1; j++) {
            values.mData[j] = values.mData[j + 1];
        }
        values.mSize = values.size() - 1;
    }
    return 1;
}

// ---------------------------------------------------------------------------------------------
// inferred: entry with a text (0x1C bytes; constructor 0x805072C4, which clears the whole object
// after constructing the string member)
struct MpTextEntry_unk {
    MpTextEntry_unk();
    int m00[5];
    atn::ptr_string_base<wchar_t, 0> mText; // 0x14
};

MpTextEntry_unk::MpTextEntry_unk() {
    memset(this, 0, sizeof(*this));
}

// ---------------------------------------------------------------------------------------------
// inferred: titles by experience ("title" rows: rank, experience, a, b) and points ("point" rows:
// three values per row)
struct MpLevel_unk {
    int mRank;  // 0x00
    int mExp;   // 0x04
    int mNext;  // 0x08: inferred: experience to the next rank
    int m0C;
    int m10;
};

struct MpLevelTable_unk {
    atn::Buffer_unk<MpLevel_unk> mLevels; // 0x00
    int mPoints[1][3];                    // 0x08
};

// NONMATCHING: 98.6% - string pool offsets (see fn_805061C8) and registers of the point rows
extern "C" void fn_80507310(MpLevelTable_unk* table, const char* data, int size) {
    atn::Csv csv(data, size);
    while (csv.fn_8005BBF8()) {
        const char* f = csv.fn_8005B9CC();
        if (f[0] == ';') {
            continue;
        }
        if (strcmp(f, "begin") != 0) {
            continue;
        }
        const char* name = csv.fn_8005B9CC();
        if (strcmp(name, "title") == 0) {
            int n = atoi(csv.fn_8005B9CC());
            table->mLevels.Resize_unk(n, 0);
            int last;
            while (csv.fn_8005BBF8()) {
                const char* f2 = csv.fn_8005B9CC();
                if (f2[0] == ';') {
                    continue;
                }
                if (strcmp(f2, "end") == 0) {
                    break;
                }
                last = atoi(csv.fn_8005B9CC()) - 1;
                if (last >= n) {
                    break;
                }
                MpLevel_unk* e = &table->mLevels.mData[last];
                e->mRank = last;
                e->mExp = atoi(csv.fn_8005B9CC());
                e->m0C = atoi(csv.fn_8005B9CC());
                e->m10 = atoi(csv.fn_8005B9CC());
            }
            for (unsigned int i = 0; i < (unsigned int)table->mLevels.mSize; i++) {
                MpLevel_unk* e = &table->mLevels.mData[i];
                if (i + 1 < (unsigned int)table->mLevels.mSize) {
                    e->mNext = e[1].mExp - e->mExp;
                } else {
                    e->mNext = 0;
                }
            }
        } else if (strcmp(name, "point") == 0) {
            int(*row)[3] = table->mPoints;
            while (csv.fn_8005BBF8()) {
                const char* f2 = csv.fn_8005B9CC();
                if (f2[0] == ';') {
                    continue;
                }
                if (strcmp(f2, "end") == 0) {
                    break;
                }
                csv.fn_8005B9CC();
                for (int k = 2; k >= 0; k--) {
                    (*row)[k] = atoi(csv.fn_8005B9CC());
                }
                row++;
            }
        }
    }
}

// inferred: level reached with `exp` (the first one above it, else the last)
// NONMATCHING: 98.2% - r5/r7 of the count (close-units qD: size local, separate assignment, pointer
// arithmetic, unsigned/int index forms: no change)
extern "C" MpLevel_unk* fn_805075C4(MpLevelTable_unk* table, int exp) {
    unsigned int n = table->mLevels.mSize - 1;
    for (unsigned int i = 0; i < n; i++) {
        // inferred: the data pointer is reloaded every iteration (volatile read; visible where the
        // function is inlined twice in fn_80507654)
        MpLevel_unk* e = &(*(MpLevel_unk* volatile*)&table->mLevels.mData)[i];
        if (exp < e->mExp) {
            return e;
        }
    }
    return &table->mLevels.mData[n];
}

// inferred: last level whose experience is not above `exp`
extern "C" MpLevel_unk* fn_80507610(MpLevelTable_unk* table, int exp) {
    MpLevel_unk* found = 0;
    unsigned int n = table->mLevels.mSize - 1;
    for (unsigned int i = 0; i < n; i++) {
        MpLevel_unk* e = &table->mLevels.mData[i];
        if (exp < e->mExp) {
            break;
        }
        found = e;
    }
    return found;
}

// inferred: the two experiences are in different levels
extern "C" int fn_80507654(MpLevelTable_unk* table, int a, int b) {
    MpLevel_unk* la = fn_805075C4(table, a);
    MpLevel_unk* lb = fn_805075C4(table, b);
    return la != 0 && lb != 0 && la != lb;
}

// inferred: points of a result (kind 0/1: first row, kind 2: row 5 + rank; index clamped to 0..2)
extern "C" int fn_805076F8(MpLevelTable_unk* table, int kind, int rank, int index) {
    int i = (index > 0 ? index : 0) < 2 ? (index > 0 ? index : 0) : 2;
    if ((unsigned int)kind <= 1) {
        return table->mPoints[0][i];
    }
    if (kind == 2) {
        return table->mPoints[rank + 5][i];
    }
    return 0;
}

// inferred: experience clamped to 0..99999
extern "C" int fn_80507764(MpLevelTable_unk* table, int exp) {
    int max = 99999;
    if ((exp > 0 ? exp : 0) >= max) {
        return max;
    }
    return exp > 0 ? exp : 0;
}
