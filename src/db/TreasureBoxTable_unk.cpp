#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: item data (db/ItemParam_unk); only the member used here
struct TreasureItemData_unk {
    unsigned char m00[0xBC];
    short mKind; // inferred: 0xBC
};
extern "C" TreasureItemData_unk* fn_80211480(int id); // inferred: item data by id
extern "C" int fn_80206B9C(int id);                   // inferred: item test (db/WeaponParam_unk)
extern "C" int fn_8020EF04(int id);                   // inferred: item test (db/EquipParam_unk)

// inferred: multiplayer session (lbl_8087F610); only the member used here
struct TreasureMpSession_unk {
    unsigned char m000[0x540];
    int mMode; // inferred: 0x540
};
extern "C" TreasureMpSession_unk* lbl_8087F610;

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one item of a treasure box (0xC bytes); inline constructor emitted out of line
// (fn_8021E42C) for __construct_new_array
struct TreasureItem_unk {
    TreasureItem_unk() : mId(0), mNum(0), mRate(0.0f) {}

    int mId;
    int mNum;
    float mRate; // inferred: relative chance
};

// inferred: one treasure box (0xC bytes, "begin" .. "end" block of up to 16 items); inline
// constructor emitted out of line (fn_8021E41C)
struct TreasureBox_unk {
    TreasureBox_unk() : mId(0), mCount(0) {}

    int mId;
    TreasureItem_unk* mItems;
    unsigned int mCount;
};

static const char sFileNameStr[] = "/boot/treasure_box_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static TreasureBox_unk* sBoxes;
static TreasureItem_unk* sItems;
static int sBoxCount;
static int sItemCount;

// inferred: loads /boot/treasure_box_table.csv (two header lines)
extern "C" void fn_8021E0B0() {
    atn::Csv csv;
    if (sBoxes == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        sBoxCount = 0;
        sItemCount = 0;
        csv.vf08((const char*)data, size);
        csv.fn_8005BBF8();
        csv.fn_8005BBF8();
        do {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "begin") != 0) {
                continue;
            }
            sBoxCount++;
            int n = 0;
            while (csv.fn_8005BBF8() != 0) {
                field = csv.fn_8005B9CC();
                if (*field == ';' || *field == '#') {
                    continue;
                }
                if (strcmp(field, "end") == 0) {
                    break;
                }
                sItemCount++;
                if (++n >= 16) {
                    break;
                }
            }
        } while (csv.fn_8005BBF8() != 0);
        sBoxes = new (3, "", "", 0) TreasureBox_unk[sBoxCount];
        sItems = new (3, "", "", 0) TreasureItem_unk[sItemCount];
        csv.vf08((const char*)data, size);
        csv.fn_8005BBF8();
        csv.fn_8005BBF8();
        int i = 0;
        int k = 0;
        do {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "begin") != 0) {
                continue;
            }
            TreasureBox_unk* box = &sBoxes[i];
            box->mId = atoi(csv.fn_8005B9CC());
            csv.fn_8005B9CC(); // inferred: a column that is not used
            box->mItems = &sItems[k];
            box->mCount = 0;
            int n = 0;
            while (csv.fn_8005BBF8() != 0) {
                field = csv.fn_8005B9CC();
                if (*field == ';' || *field == '#') {
                    continue;
                }
                if (strcmp(field, "end") == 0) {
                    break;
                }
                TreasureItem_unk* item = &sItems[k];
                box->mCount++;
                item->mRate = fn_800DC288(field);
                csv.fn_8005B9CC(); // inferred: a column that is not used
                item->mId = atoi(csv.fn_8005B9CC());
                item->mNum = atoi(csv.fn_8005B9CC());
                k++;
                if (++n >= 16) {
                    break;
                }
            }
            i++;
        } while (csv.fn_8005BBF8() != 0);
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: box by id (0 if there is none)
extern "C" TreasureBox_unk* fn_8021E444(int id) {
    for (int i = 0; i < sBoxCount; i++) {
        if (id == sBoxes[i].mId) {
            return &sBoxes[i];
        }
    }
    return 0;
}

extern "C" int fn_8021E5A4(TreasureBox_unk* box, unsigned int flags);

// inferred: index of a random item of a box (-1 if none)
extern "C" int fn_8021E48C(int id, unsigned int flags) {
    TreasureBox_unk* box = fn_8021E444(id);
    if (box != 0) {
        return fn_8021E5A4(box, flags);
    }
    return -1;
}

// inferred: random item of a box (id and number), 0 if none
extern "C" int fn_8021E4E4(int* itemId, int* num, int id, unsigned int flags) {
    TreasureBox_unk* box = fn_8021E444(id);
    if (box != 0) {
        int index = fn_8021E5A4(box, flags);
        if (index >= 0) {
            TreasureItem_unk* item = &box->mItems[index];
            *itemId = item->mId;
            *num = item->mNum;
            return 1;
        }
    }
    return 0;
}

static inline bool IsSpecial_unk(int id) {
    if ((id >= 0x2714 && id <= 0x2716) || (id >= 0x2718 && id <= 0x271A) || (id >= 0x2711 && id <= 0x2712)) {
        return false;
    }
    if (fn_80206B9C(id) || fn_8020EF04(id)) {
        return false;
    }
    return true;
}

static inline bool IsGil_unk(int id) {
    if (id == 0x2714 || id == 0x2719) {
        return true;
    }
    return false;
}

static inline TreasureItemData_unk* GetItemData_unk(int id) {
    return id > 0 ? fn_80211480(id) : 0;
}

static inline bool IsKind_unk(int id, int kind) {
    TreasureItemData_unk* data = GetItemData_unk(id);
    if (data != 0 && data->mKind == kind) {
        return true;
    }
    return false;
}

// inferred: chance factor of an item with the given flags
static inline float GetRate_unk(TreasureItem_unk* item, unsigned int flags) {
    float rate = 1.0f;
    if ((flags & 0x2) == 0x2 && IsSpecial_unk(item->mId)) {
        rate *= 1.5f;
    }
    if ((flags & 0x4) == 0x4 && IsGil_unk(item->mId)) {
        rate *= 2.0f;
    }
    if ((flags & 0x8) == 0x8 && IsGil_unk(item->mId)) {
        rate = 0.0f;
    }
    if ((flags & 0x10) == 0x10 && IsKind_unk(item->mId, 0)) {
        rate = 0.0f;
    }
    if ((flags & 0x20) == 0x20 && fn_80206B9C(item->mId)) {
        rate = 0.0f;
    }
    if ((flags & 0x40) == 0x40 && IsKind_unk(item->mId, 6)) {
        rate = 0.0f;
    }
    if (lbl_8087F610 != 0 && lbl_8087F610->mMode == 2 && item->mId >= 0x44D && item->mId <= 0x450) {
        rate = 0.0f;
    }
    return rate;
}

// inferred: picks a random item of a box by its chances (-1 if none)
// NONMATCHING: 96.86% - the original computes the flag masks of the inlined GetRate_unk again
// before the second loop (here they are shared with the first loop), so the saved registers
// differ; tried the first loop in its own inline function.
extern "C" int fn_8021E5A4(TreasureBox_unk* box, unsigned int flags) {
    float total = 0.0f;
    int result = -1;
    for (unsigned int i = 0; i < box->mCount; i++) {
        TreasureItem_unk* item = &box->mItems[i];
        total += item->mRate * GetRate_unk(item, flags);
    }
    if (total > 0.0f) {
        if ((flags & 1) != 1 && total < 100.0f) {
            total = 100.0f;
        }
        float r = (float)(int)(100.0f * (total * ((float)(rand() % 1001) / 1000.0f))) / 100.0f;
        for (unsigned int i = 0; i < box->mCount; i++) {
            TreasureItem_unk* item = &box->mItems[i];
            r -= item->mRate * GetRate_unk(item, flags);
            if (r < 0.01f) {
                result = i;
                break;
            }
        }
    }
    return result;
}
