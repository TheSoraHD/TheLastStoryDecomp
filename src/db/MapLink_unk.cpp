#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int atoi(const char* s);
extern "C" char* strchr(const char* s, int c);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: splits a map id into its kind and two numbers (db/MapIdTable_unk.cpp)
extern "C" bool fn_80216AFC(const char* id, int* type, int* map, int* point);

// inferred: destination of a map link: map id (kind and two numbers) and an entry point (8 bytes)
struct MapLinkJump_unk {
    MapLinkJump_unk() : mType(0), mMap(0), mPoint(0), mEntry(0) {}
    bool operator==(const MapLinkJump_unk& other) const {
        return mType == other.mType && mMap == other.mMap && mPoint == other.mPoint && mEntry == other.mEntry;
    }

    unsigned char mType;
    unsigned char mMap;
    unsigned char mPoint;
    int mEntry; // inferred: number after ':'
};

// inferred: how a map link is shown (2 bytes)
struct MapLinkDisp_unk {
    MapLinkDisp_unk() : mKind(0), mBold(0) {}
    bool operator==(const MapLinkDisp_unk& other) const { return mKind == other.mKind && mBold == other.mBold; }

    signed char mKind;
    unsigned char mBold : 1; // inferred: "bold"
};

// inferred: one row of /boot/maplink_jump.csv or /boot/maplink_disp.csv (0x78 bytes): the indices of
// the cell of each column
struct MapLinkJumpRow_unk {
    MapLinkJumpRow_unk() : m00(0), m04(0) { memset(mCells, 0, sizeof(mCells)); }

    int m00;
    int m04;
    short mCells[0x38];
};
struct MapLinkDispRow_unk {
    MapLinkDispRow_unk() : m00(0), m04(0) { memset(mCells, 0, sizeof(mCells)); }

    int m00;
    int m04;
    short mCells[0x38];
};

// inferred: one row of /boot/maplink_name_xx.u16 (0x88 bytes)
struct MapLinkName_unk {
    MapLinkName_unk() : mId(0) { memset(mName, 0, sizeof(mName)); }

    int mId;
    wchar_t mName[0x40];
    int m84;
};

static const char sJumpFileNameStr[] = "/boot/maplink_jump.csv";
static const char sDispFileNameStr[] = "/boot/maplink_disp.csv";
static const char sNameFileNameStr[] = "/boot/maplink_name_xx.u16";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sJumpFileName = sJumpFileNameStr;
static const char* const sDispFileName = sDispFileNameStr;
static const char* const sNameFileName = sNameFileNameStr;

static atn::SimpleVector<int> sColumns; // inferred: ids of the columns (header of maplink_jump.csv)
static atn::SimpleVector<MapLinkJump_unk> sJumps;
static atn::SimpleVector<MapLinkDisp_unk> sDisps;

static MapLinkJumpRow_unk* sJumpRows;
static int sJumpRowCount;
static MapLinkDispRow_unk* sDispRows;
static int sDispRowCount;
static MapLinkName_unk* sNames;
static int sNameCount;

// inferred: index of a value in a vector, -1 if none
template <class T>
static inline int IndexOf_unk(const atn::SimpleVector<T>& vector, const T& value) {
    int index = -1;
    for (unsigned int i = 0; i < vector.mSize; i++) {
        if (vector.mData[i] == value) {
            index = i;
            break;
        }
    }
    return index;
}

// inferred: loads the map link tables
// NONMATCHING: 98.58% - register numbering; the searches in the vectors of cells use an offset (the
// original: lbzx + add) instead of a pointer and the original reuses the loaded size as the new index
extern "C" void fn_802150F0() {
    atn::Csv csv;
    unsigned long size;
    if (sJumpRows == 0) {
        void* data = fn_8046DC5C(lbl_8087F518, sJumpFileName, &size, 0x20);
        csv.vf08((const char*)data, size);
        for (int i = 0; i < 3; i++) {
            csv.fn_8005BBF8();
        }
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        const char* field;
        do {
            field = csv.fn_8005B9CC();
            sColumns.push_back(atoi(field));
        } while (*field != 0);
        while (csv.fn_8005BBF8() != 0) {
            field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#' || *field == 0) {
                continue;
            }
            sJumpRowCount++;
        }
        sJumpRows = new (1, "", "", 0) MapLinkJumpRow_unk[sJumpRowCount];
        csv.vf08((const char*)data, size);
        csv.fn_8005BBF8();
        csv.fn_8005BBF8();
        csv.fn_8005BBF8();
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#' || *field == 0) {
                continue;
            }
            MapLinkJumpRow_unk* row = &sJumpRows[i];
            row->m00 = atoi(csv.mField);
            row->m04 = atoi(csv.fn_8005B9CC());
            int k = 0;
            for (;;) {
                field = csv.fn_8005B9CC();
                if (*field == 0) {
                    break;
                }
                int type;
                int map;
                int point;
                int entry = 0;
                if (fn_80216AFC(field, &type, &map, &point)) {
                    MapLinkJump_unk jump;
                    jump.mType = type;
                    jump.mMap = map;
                    jump.mPoint = point;
                    const char* colon = strchr(field, ':');
                    if (colon != 0) {
                        entry = atoi(colon + 1);
                    }
                    jump.mEntry = entry;
                    int index = IndexOf_unk(sJumps, jump);
                    if (index < 0) {
                        index = sJumps.mSize;
                        sJumps.push_back(jump);
                    }
                    row->mCells[k] = index;
                }
                k++;
            }
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
    if (sDispRows == 0) {
        void* data = fn_8046DC5C(lbl_8087F518, sDispFileName, &size, 0x20);
        csv.vf08((const char*)data, size);
        for (int i = 0; i < 3; i++) {
            csv.fn_8005BBF8();
        }
        const char* field;
        while (csv.fn_8005BBF8() != 0) {
            field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#' || *field == 0) {
                continue;
            }
            sDispRowCount++;
        }
        sDispRows = new (1, "", "", 0) MapLinkDispRow_unk[sDispRowCount];
        csv.vf08((const char*)data, size);
        csv.fn_8005BBF8();
        csv.fn_8005BBF8();
        csv.fn_8005BBF8();
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#' || *field == 0) {
                continue;
            }
            MapLinkDispRow_unk* row = &sDispRows[i];
            row->m00 = atoi(csv.mField);
            row->m04 = atoi(csv.fn_8005B9CC());
            int k = 0;
            for (;;) {
                field = csv.fn_8005B9CC();
                if (*field == 0) {
                    break;
                }
                MapLinkDisp_unk disp;
                disp.mKind = atoi(field);
                disp.mBold = strstr(field, "bold") != 0;
                int index = IndexOf_unk(sDisps, disp);
                if (index < 0) {
                    index = sDisps.mSize;
                    sDisps.push_back(disp);
                }
                row->mCells[k] = index;
                k++;
            }
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
    if (sNames == 0) {
        atn::CsvU csvu;
        void* data = fn_8046DC5C(lbl_8087F518, sNameFileName, &size, 0x20);
        csvu.vf08((const wchar_t*)data, (int)size / 2);
        while (csvu.fn_8005BBF8() != 0) {
            const wchar_t* field = csvu.fn_8005B9CC();
            if (*field == L';' || *field == L'#' || *field == 0) {
                continue;
            }
            sNameCount++;
        }
        sNames = new (1, "", "", 0) MapLinkName_unk[sNameCount];
        csvu.vf08((const wchar_t*)data, (int)size / 2);
        int i = 0;
        while (csvu.fn_8005BBF8() != 0) {
            const wchar_t* field = csvu.fn_8005B9CC();
            if (*field == L';' || *field == L'#' || *field == 0) {
                continue;
            }
            MapLinkName_unk* name = &sNames[i];
            name->mId = fn_800DC1DC(field);
            wcscpy(name->mName, csvu.fn_8005B9CC());
            csvu.fn_8005B9CC();
            name->m84 = fn_800DC1DC(csvu.fn_8005B9CC());
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: row of a table for a value of the first column (the last row whose value is not above
// it) and of the second column, 0 if none
template <class T>
T* FindRow_unk(int a, int b, T* rows, int count) {
    int found = -1;
    int n = 0;
    for (int i = 1; i < count; i++) {
        if (a < rows[i].m00) {
            found = i - 1;
            break;
        }
    }
    if (found < 0) {
        found = count - 1;
    }
    for (int i = found; i >= 0; i--) {
        if (rows[i].m00 != rows[found].m00) {
            break;
        }
        n++;
    }
    found -= n - 1;
    if (found >= 0) {
        T* row = &rows[found];
        for (int i = 0; i < n; i++) {
            if (row->m04 == b) {
                found += i;
                break;
            }
            row++;
        }
        return &rows[found];
    }
    return 0;
}

// inferred: index of a column id, -1 if none
static inline int FindColumn_unk(int id) {
    int i;
    int index = -1;
    for (i = 0; i < (int)sColumns.mSize; i++) {
        if (id == sColumns.mData[i]) {
            index = i;
            break;
        }
    }
    return index;
}

// inferred: destination of a link (column id, then the two row keys)
// NONMATCHING: 92.23% - the data pointer and the counter of the column search get swapped registers
extern "C" MapLinkJump_unk* fn_80216544(int id, int a, int b) {
    int column = FindColumn_unk(id);
    if (column >= 0) {
        MapLinkJumpRow_unk* row = FindRow_unk(a, b, sJumpRows, sJumpRowCount);
        if (row != 0) {
            return &sJumps.mData[row->mCells[column]];
        }
        return &sJumps.mData[sJumpRows->mCells[column]];
    }
    return 0;
}

// inferred: display of a link (column id, then the two row keys)
// NONMATCHING: 92.23% - as fn_80216544
extern "C" MapLinkDisp_unk* fn_80216700(int id, int a, int b) {
    int column = FindColumn_unk(id);
    if (column >= 0) {
        MapLinkDispRow_unk* row = FindRow_unk(a, b, sDispRows, sDispRowCount);
        if (row != 0) {
            return &sDisps.mData[row->mCells[column]];
        }
        return &sDisps.mData[sDispRows->mCells[column]];
    }
    return 0;
}

// inferred: name of a link id, 0 if none
extern "C" const wchar_t* fn_802168BC(int id) {
    for (int i = 0; i < sNameCount; i++) {
        if (id == sNames[i].mId) {
            return sNames[i].mName;
        }
    }
    return 0;
}

extern "C" int fn_80216908(int id) {
    for (int i = 0; i < sNameCount; i++) {
        if (id == sNames[i].mId) {
            return sNames[i].m84;
        }
    }
    return 0;
}

// inferred: column id of an index, -1 if none
extern "C" int fn_80216954(int index) {
    if (0 > index || (int)sColumns.mSize <= index) {
        return -1;
    }
    return sColumns.mData[index];
}

extern "C" int fn_80216988() {
    return sColumns.mSize;
}
