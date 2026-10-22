#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Array_unk.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" char* strchr(const char* s, int c);
extern "C" int atoi(const char* s);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

extern "C" void fn_80216AFC(const char* id, int* a, int* b, int* c); // inferred: splits an id

// inferred: tables of other files (only the members read here)
struct ItemData_unk {
    int m00;
    int m04; // inferred: id of the weapon / equipment
    char m08[0xC2 - 0x08];
    signed char mC2;
};
struct WeaponData_unk {
    char m00[0x78];
    int m78;
    int m7C;
    int m80;
    char m84[0x114 - 0x84];
    int m114;
};
struct EquipData_unk {
    char m00[0x78];
    int mKind;
    int mId;
};
extern "C" ItemData_unk* fn_80211480(int id);
extern "C" int fn_80206B9C(int id);
extern "C" WeaponData_unk* fn_80206C50(int id);
extern "C" bool fn_8020EF04(int code);
extern "C" EquipData_unk* fn_8020EFEC(int code);

// inferred: one item of a rare table row: kind (1 "ws"/"wg", 2 "qu"/"ql", 0 other, 3 none), sub kind,
// number and the number after '_' (upper bound of a range)
struct RareItem_unk {
    RareItem_unk() : mKind(3), mSub(0), mNum(0), mNum2(0) {}
    int mKind;
    int mSub;
    int mNum;
    int mNum2;
};

// inferred: atn::Vector_unk::reserve without the capacity check, that also empties the list (another
// instance of the "" literals in .sdata: a separate function). Name invented.
inline void InitItems_unk(atn::Vector_unk<RareItem_unk>& list, unsigned int n) {
    RareItem_unk* data = new (0, "", "", 0) RareItem_unk[n];
    if (list.mData != 0) {
        unsigned int count = (n > list.mSize) ? list.mSize : n;
        for (unsigned int i = 0; i < count; i++) {
            data[i] = list.mData[i];
        }
        delete[] list.mData;
    }
    list.mData = data;
    list.mCapacity = n;
    list.mSize = 0;
}

// inferred: one row of /boot/item_rare.csv (0x18 bytes)
struct RareEntry_unk {
    RareEntry_unk() : m00(-1), m04(-1), m08(-1) { InitItems_unk(mItems, 2); }
    ~RareEntry_unk() {}
    int m00;
    int m04;
    int m08;
    atn::Vector_unk<RareItem_unk> mItems;
};

static const char sFileNameStr[] = "/boot/item_rare.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static RareEntry_unk* sEntries;
static int sCount;

// inferred: kind, sub kind and number of an item id
extern "C" bool fn_80212F68(const char* id, int* kind, int* sub, int* num) {
    if (id == 0) {
        return false;
    }
    if (strlen(id) >= 3 && (strncmp(id, "ws", 2) == 0 || strncmp(id, "wg", 2) == 0)) {
        *kind = 1;
        *sub = id[1] != 's';
        *num = atoi(id + 2);
    } else if (strlen(id) >= 3 && (strncmp(id, "qu", 2) == 0 || strncmp(id, "ql", 2) == 0)) {
        *kind = 2;
        *sub = id[1] != 'u';
        *num = atoi(id + 2);
    } else {
        *kind = 0;
        *sub = 0;
        *num = atoi(id);
    }
    return true;
}

// inferred: loads /boot/item_rare.csv
extern "C" void fn_802130BC() {
    atn::Csv csv;
    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    int count = 0;
    csv.vf08((const char*)data, size);
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() == '\0') {
            count++;
        }
    }
    sEntries = new (1, "", "", 0) RareEntry_unk[count];
    sCount = count;
    csv.vf08((const char*)data, size);
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != '\0') {
            continue;
        }
        const char* id = csv.fn_8005B9CC();
        RareEntry_unk* entry = &sEntries[i];
        fn_80216AFC(id, &entry->m00, &entry->m04, &entry->m08);
        csv.fn_8005B9CC(); // inferred: a column that is not used
        for (;;) {
            const char* field = csv.fn_8005B9CC();
            if (*field == '\0') {
                break;
            }
            RareItem_unk item;
            if (!fn_80212F68(field, &item.mKind, &item.mSub, &item.mNum)) {
                continue;
            }
            const char* upper = strchr(field, '_');
            if (upper != 0) {
                int kind;
                int sub;
                fn_80212F68(upper + 1, &kind, &sub, &item.mNum2);
            }
            entry->mItems.push_back(item);
        }
        i++;
    }
    fn_8046DD20(lbl_8087F518, data);
}

// inferred: rare row of a kind (only 2) and sub kind; sub kind 9 also matches a number
extern "C" RareEntry_unk* fn_80213B08(int kind, int sub, int num) {
    if (kind != 2) {
        return 0;
    }
    RareEntry_unk* entry = sEntries;
    for (int i = 0; i < sCount; i++, entry++) {
        if (entry->m04 == sub) {
            if (entry->m04 == 9) {
                int value = entry->m08;
                if (value == num) {
                    return entry;
                }
                if (value < 10 && value + 10 == num) {
                    return entry;
                }
            } else {
                return entry;
            }
        }
    }
    return 0;
}

// inferred: whether a row has an item of a kind and sub kind whose number (range) contains value
static inline int HasItem_unk(RareEntry_unk* entry, int kind, int sub, int value) {
    for (unsigned int i = 0; i < entry->mItems.mSize; i++) {
        RareItem_unk* item = &entry->mItems.mData[(int)i];

        if (item->mKind == kind && item->mSub == sub) {
            if (item->mNum2 > 0) {
                return item->mNum <= value && value <= item->mNum2;
            }
            return item->mNum == value;
        }
    }
    return false;
}
// inferred: the same with the value before the sub kind (second copy used by fn_80213E60)
static inline int HasItem2_unk(RareEntry_unk* entry, int kind, int value, int sub) {
    for (unsigned int i = 0; i < entry->mItems.mSize; i++) {
        RareItem_unk* item = &entry->mItems.mData[(int)i];

        if (item->mKind == kind && item->mSub == sub) {
            if (item->mNum2 > 0) {
                return item->mNum <= value && value <= item->mNum2;
            }
            return item->mNum == value;
        }
    }
    return false;
}

// inferred: whether an item is rare for a row
// The int index compared as unsigned gives the register order of the original
extern "C" bool fn_80213B78(int id, int kind, int sub, int num) {
    ItemData_unk* item = fn_80211480(id);
    bool result;
    if (item == 0) {
        result = false;
    } else if (fn_80206B9C(item->m04)) {
        WeaponData_unk* weapon = fn_80206C50(item->m04);
        RareEntry_unk* entry = fn_80213B08(kind, sub, num);
        result = weapon->m114 != 0 || (entry != 0 && HasItem_unk(entry, 1, weapon->m78, weapon->m80));
    } else if (fn_8020EF04(item->m04)) {
        EquipData_unk* equip = fn_8020EFEC(item->m04);
        RareEntry_unk* entry = fn_80213B08(kind, sub, num);
        result = entry != 0 && HasItem_unk(entry, 2, equip->mKind, equip->mId);
    } else {
        RareEntry_unk* entry = fn_80213B08(kind, sub, num);
        result = item->mC2 != 0 || (entry != 0 && HasItem_unk(entry, 0, 0, item->m04));
    }
    return result;
}

// inferred: as fn_80213B78 for item data
// NONMATCHING: 99.87% - the original loads the value (0x80/0x7C) before the sub kind (0x78) for the
// inlined search; with HasItem2_unk (value before sub) the registers match but the loads are in the
// other order. Tried: HasItem_unk, locals for the arguments, reference parameters, compare orders.
extern "C" bool fn_80213E60(ItemData_unk* item, int kind, int sub, int num) {
    if (item == 0) {
        return false;
    }
    if (fn_80206B9C(item->m04)) {
        WeaponData_unk* weapon = fn_80206C50(item->m04);
        RareEntry_unk* entry = fn_80213B08(kind, sub, num);
        return weapon->m114 != 0 || (entry != 0 && HasItem2_unk(entry, 1, weapon->m80, weapon->m78));
    }
    if (fn_8020EF04(item->m04)) {
        EquipData_unk* equip = fn_8020EFEC(item->m04);
        RareEntry_unk* entry = fn_80213B08(kind, sub, num);
        return entry != 0 && HasItem2_unk(entry, 2, equip->mId, equip->mKind);
    }
    RareEntry_unk* entry = fn_80213B08(kind, sub, num);
    return item->mC2 != 0 || (entry != 0 && HasItem2_unk(entry, 0, item->m04, 0));
}
