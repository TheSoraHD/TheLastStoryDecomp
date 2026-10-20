#include "atn/Csv.h"
#include "atn/Memory.h"
#include "db/EquipParam_unk.h"
#include "db/ParseUtil_unk.h"
#include "db/WeaponParam_unk.h"
#include "gmk/SeTable.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" int atoi(const char* s);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" wchar_t* wcschr(const wchar_t* s, wchar_t c);
extern "C" float fn_800DC288(const char* s);                       // inferred: string to float
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf

extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int flag); // inferred: loads a file
extern "C" void fn_8006BB6C(void* data, int flag);                            // inferred: frees it
struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void* fn_800839EC(MemoryManager_unk* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line, int arg); // inferred: allocation

// inferred: field (lbl_8087F430) and its flags
struct ItemField_unk;
extern "C" ItemField_unk* lbl_8087F430;
extern "C" int fn_80370174(ItemField_unk* field, int flag);

// inferred: text sizes of a row of the UTF-16 text file (db/SkillParam_unk.cpp)
extern "C" void fn_80219F1C(atn::CsvU* csv, int* a, int* b);

// inferred: five numbers
struct ItemParamFive_unk {
    ItemParamFive_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// inferred: one row of /boot/item_param.csv (0x138 bytes); the last 0x100 rows are free slots for the
// upgraded weapons and equipment
struct ItemParamEntry_unk : public BulletParam_unk {
    ItemParamEntry_unk();

    short mBC;
    short mBE;
    unsigned char mItemKind; // inferred: index in sKinds
    unsigned char mUser;  // inferred: index in sUsers
    unsigned char mC2;
    unsigned char mC3;
    int mC4;
    int mC8; // inferred: price
    atn::FixedString_unk<0x10> mCC;
    atn::FixedString_unk<0x10> mDC;
    ItemParamFive_unk mEC[2];
    atn::FixedString_unk<0x8> m114;
    int m11C[4];
    unsigned char m12C[4];
    int m130;
    int m134;
};

static const char* const sKinds[6] = {"menu", "batt", "bowg", "inst", "scri", "trap"};
static const char* const sUsers[2] = {"all", "elza"};
static const char sTextFileNameStr[] = "/boot/item_param_text_xx.u16";
static const char* const sTextFileName = sTextFileNameStr; // inferred: .rodata array + .sdata2 pointer

typedef wchar_t ItemName_unk[0x20];

#define IP_MAX(a, b) ((a) < (b) ? (b) : (a))
#define IP_MIN(a, b) ((a) < (b) ? (a) : (b))

static ItemParamEntry_unk* sEntries;
static int sCount;
static char* sBuf;
static wchar_t* sTexts;
static ItemName_unk* sNames;

extern "C" void fn_80211244();

// inferred: index of a name in a table, 0 if not found
static inline int FindName_unk(const char* name, const char* const* table, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(name, table[i]) == 0) {
            return i;
        }
    }
    return 0;
}

// inferred: std::equal
static inline bool Equal_unk(const char* first, const char* last, const char* first2) {
    for (; first != last; ++first, ++first2) {
        if (!(*first == *first2)) {
            return false;
        }
    }
    return true;
}

// inferred: whether a fixed string is equal to a name
template <int N>
static inline bool IsKey_unk(const atn::FixedString_unk<N>& key, const char* name) {
    unsigned long n = strlen(key.c_str());
    return name[n] == 0 && Equal_unk(key.c_str(), key.c_str() + n, name);
}

// inferred: loads /boot/item_param.csv (the first line may hold the "Version" of the format)
extern "C" void fn_80210ACC() {
    unsigned long size = 0;
    void* data = fn_8006BA8C("/boot/item_param.csv", &size, 0);
    atn::Csv csv((const char*)data, size);
    int version = 0;
    if (strcmp(csv.fn_8005B9CC(), "Version") == 0) {
        version = atoi(csv.fn_8005B9CC());
    }
    csv.fn_8005BBF8();
    sCount = 0;
    int a = 0;
    int b = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (strcmp(csv.fn_8005B9CC(), "") != 0) {
            sCount++;
        }
    }
    sCount += 0x100;
    sEntries = (ItemParamEntry_unk*)fn_800839EC(fn_800827E0(), sCount * sizeof(ItemParamEntry_unk), 0x20, 1, "", "",
                                                 0, 0);
    sBuf = new (1, "", "", 0) char[a];
    csv.vf08((const char*)data, size);
    int i = 0;
    csv.fn_8005BBF8();
    a = 0;
    b = 0;
    while (csv.fn_8005BBF8() != 0) {
        ItemParamEntry_unk* entry = &sEntries[i];
        entry->m00 = 1;
        fn_80219F84(entry, &csv, version, sBuf, sTexts, &a, &b);
        entry->m03 = 100;
        entry->mBC = atoi(csv.fn_8005B9CC());
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        entry->mC4 = atoi(csv.fn_8005B9CC());
        entry->mC8 = atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        atoi(csv.fn_8005B9CC());
        entry->mItemKind = FindName_unk(csv.fn_8005B9CC(), sKinds, 6);
        entry->mUser = FindName_unk(csv.fn_8005B9CC(), sUsers, 2);
        entry->mC2 = atoi(csv.fn_8005B9CC());
        entry->mC3 = atoi(csv.fn_8005B9CC());
        entry->mCC = csv.fn_8005B9CC();
        if (IsKey_unk(entry->mCC, "0") || IsKey_unk(entry->mCC, "-1")) {
            entry->mCC.mStr[0] = 0;
        }
        entry->mDC = csv.fn_8005B9CC();
        if (IsKey_unk(entry->mDC, "0") || IsKey_unk(entry->mDC, "-1")) {
            entry->mDC.mStr[0] = 0;
        }
        for (int j = 0; j < 2; j++) {
            entry->mEC[j].m00 = atoi(csv.fn_8005B9CC());
            entry->mEC[j].m04 = atoi(csv.fn_8005B9CC());
            entry->mEC[j].m08 = atoi(csv.fn_8005B9CC());
            entry->mEC[j].m0C = atoi(csv.fn_8005B9CC());
            entry->mEC[j].m10 = atoi(csv.fn_8005B9CC());
        }
        entry->mB4 = atoi(csv.fn_8005B9CC());
        entry->mB8 = fn_800DC288(csv.fn_8005B9CC());
        entry->m114 = csv.fn_8005B9CC();
        entry->m130 = atoi(csv.fn_8005B9CC());
        for (int j = 0; j < 4; j++) {
            entry->m11C[j] = atoi(csv.fn_8005B9CC());
            entry->m12C[j] = atoi(csv.fn_8005B9CC());
        }
        entry->m134 = atoi(csv.fn_8005B9CC());
        entry->mBE = atoi(csv.fn_8005B9CC());
        i++;
    }
    for (; i < sCount; i++) {
        ItemParamEntry_unk* entry = &sEntries[i];
        entry->m00 = 1;
        entry->mId = -1;
        entry->mBC = 10;
        entry->mCC.mStr[0] = 0;
        entry->mDC.mStr[0] = 0;
    }
    fn_8006BB6C(data, 0);
    fn_80211244();
}

// inferred: loads the texts of the items (/boot/item_param_text_xx.u16)
// NONMATCHING: 94.95% - the original keeps the address of the atn::CsvU in a saved register while
// its inlined constructor runs (known, as in db/SkillParam_unk.cpp)
extern "C" void fn_80211244() {
    if (sEntries != 0) {
        if (sTexts != 0) {
            delete[] sTexts;
            sTexts = 0;
        }
        unsigned long size = 0;
        void* data = fn_8006BA8C(sTextFileName, &size, 0);
        atn::CsvU csv((const wchar_t*)data, size / 2);
        csv.fn_8005BBF8();
        int a = 0;
        int b = 0;
        while (csv.fn_8005BBF8() != 0) {
            if (wcscmp(csv.fn_8005B9CC(), L"") != 0) {
                fn_80219F1C(&csv, &a, &b);
            }
        }
        sTexts = new (1, "", "", 0) wchar_t[b];
        csv.vf08((const wchar_t*)data, size / 2);
        csv.fn_8005BBF8();
        a = 0;
        int i = 0;
        b = 0;
        while (csv.fn_8005BBF8() != 0) {
            const wchar_t* field = csv.fn_8005B9CC();
            if (*field == 0 || *field == L'#') {
                continue;
            }
            fn_8021A5F8(&sEntries[i], &csv, 0, sBuf, sTexts, &a, &b);
            i++;
        }
        fn_8006BB6C(data, 0);
    }
}

extern "C" int fn_8021150C(int id);

// inferred: entry of an item id, 0 if none
extern "C" ItemParamEntry_unk* fn_80211480(int id) {
    int index = fn_8021150C(id);
    if (index >= 0) {
        return &sEntries[index];
    }
    return 0;
}

extern "C" int fn_802114D8() {
    return sCount;
}

extern "C" ItemParamEntry_unk* fn_802114E0(int index) {
    if (0 > index || sCount <= index) {
        return 0;
    }
    return &sEntries[index];
}

// inferred: index of an item id, -1 if none
extern "C" int fn_8021150C(int id) {
    for (int i = 0; i < sCount; i++) {
        if (id == sEntries[i].mId) {
            return i;
        }
    }
    return -1;
}

ItemParamEntry_unk::ItemParamEntry_unk()
    : mBC(1), mBE(0), mItemKind(0), mUser(0), mC2(0), mC3(1), mC4(0), mC8(0), m130(0), m134(0) {
    m00 = 1;
    mKind = 2;
    m64 = 0;
    memset(m11C, 0, sizeof(m11C));
    memset(m12C, 0, sizeof(m12C));
}

// inferred: whether the item can be sold
extern "C" bool fn_80211648(const ItemParamEntry_unk* entry) {
    if (entry->mBC == 0 || entry->mBC == 8) {
        return false;
    }
    if (entry->mId == 101) {
        return false;
    }
    if (fn_80206B9C(entry->mId)) {
        if (((unsigned int)fn_80206C50(entry->mId)->m12C & 2) == 2) {
            return false;
        }
        if (entry->mId == 100621 && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x11B) == 0) {
            return false;
        }
    }
    if (fn_8020EF04(entry->mId) && (unsigned int)(fn_8020EFEC(entry->mId)->mA8 - 8) <= 2) {
        return false;
    }
    return true;
}

extern "C" bool fn_80211734(ItemParamEntry_unk* entry) {
    if (entry->mBC == 0) {
        return true;
    }
    return entry->mId == 101;
}

extern "C" bool fn_8021175C(ItemParamEntry_unk* entry) {
    return entry->mId == 101;
}

// inferred: name of an upgraded item ("<name>+<level>") in the name buffer of a slot
// NONMATCHING: 97.28% - register numbering (slot, entry and level)
extern "C" const wchar_t* fn_80211770(int id, int slot) {
    ItemParamEntry_unk* entry = fn_80211480(id % 1000000);
    if (entry == 0) {
        return 0;
    }
    int level = id / 1000000;
    if (slot < 0) {
        slot = 0x100;
    }
    if (sNames == 0) {
        sNames = new (1, "", "", 0) ItemName_unk[0x101];
    }
    wchar_t* name = sNames[slot];
    wcscpy(name, entry->mName);
    int plus = 0;
    wchar_t* p = wcschr(name, L'+');
    if (p != 0) {
        plus = p[1] - L'0';
        if (p[2] >= L'0' && p[2] <= L'9') {
            plus = plus * 10 + p[2] - L'0';
        }
        *p = 0;
    }
    wchar_t buf[0x20];
    if (plus != 0) {
        fn_800DD3FC(buf, L"%s+%d", name, plus + level);
    } else {
        fn_800DD3FC(buf, L"%s +%d", name, plus + level);
    }
    wcscpy(name, buf);
    return name;
}

extern "C" void fn_80212840(WeaponParamEntry_unk* weapon, WeaponParamEntry_unk* base, int level);
extern "C" void fn_80212C34(EquipParamEntry_unk* equip, EquipParamEntry_unk* base, int level);

// inferred: creates the upgraded item of a level in a free slot; returns its id (0 if none)
// NONMATCHING: 90.59% - the original copies the BulletParam_unk part with blocks (0x14-0x74 in a
// loop, pairs at 0x90 and 0xB4: sub-aggregates not in include/db/ParseUtil_unk.h) and the inlined
// fn_80212840/fn_80212C34 differ as below
extern "C" int fn_80211940(ItemParamEntry_unk* item, int level) {
    int add = level * 1000000;
    for (int i = 0; i < 0x100; i++) {
        ItemParamEntry_unk* entry = fn_802114E0(sCount - 0x100 + i);
        if (entry->mId == item->mId + add) {
            return entry->mId;
        }
    }
    for (int i = 0; i < 0x100; i++) {
        ItemParamEntry_unk* entry = fn_802114E0(sCount - 0x100 + i);
        if (entry->mId < 0) {
            *entry = *item;
            entry->mId += add;
            entry->mName = fn_80211770(entry->mId, i);
            entry->mC8 += level * 5000;
            if (fn_80206B9C(entry->mId)) {
                for (unsigned int j = 0; j < 0x80; j++) {
                    int count = fn_80206B68();
                    WeaponParamEntry_unk* weapon = fn_80206B70(count + j - 0x80);
                    WeaponParamEntry_unk* base = fn_80206C50(item->mId);
                    if (weapon->mId < 0 && base != 0) {
                        fn_80212840(weapon, base, level);
                        return entry->mId;
                    }
                }
            } else if (fn_8020EF04(entry->mId)) {
                for (unsigned int j = 0; j < 0x80; j++) {
                    int count = fn_8020EE58();
                    EquipParamEntry_unk* equip = fn_8020EE60(count + j - 0x80);
                    EquipParamEntry_unk* base = fn_8020EFEC(item->mId);
                    if (equip->mId < 0 && base != 0) {
                        fn_80212C34(equip, base, level);
                        return entry->mId;
                    }
                }
            }
            entry->mId = -1;
            return 0;
        }
    }
    return 0;
}

// inferred: whether the upgraded item of a level exists or can be created
extern "C" bool fn_802124B4(ItemParamEntry_unk* item, int level) {
    for (int i = 0; i < 0x100; i++) {
        ItemParamEntry_unk* entry = fn_802114E0(sCount - 0x100 + i);
        if (entry->mId == item->mId + level * 1000000) {
            return true;
        }
    }
    for (int i = 0; i < 0x100; i++) {
        ItemParamEntry_unk* entry = fn_802114E0(sCount - 0x100 + i);
        if (entry->mId < 0) {
            if (fn_80206B9C(item->mId)) {
                for (unsigned int j = 0; j < 0x80; j++) {
                    int count = fn_80206B68();
                    WeaponParamEntry_unk* weapon = fn_80206B70(count + j - 0x80);
                    WeaponParamEntry_unk* base = fn_80206C50(item->mId);
                    if (weapon->mId < 0 && base != 0) {
                        return true;
                    }
                }
            } else if (fn_8020EF04(item->mId)) {
                for (unsigned int j = 0; j < 0x80; j++) {
                    int count = fn_8020EE58();
                    EquipParamEntry_unk* equip = fn_8020EE60(count + j - 0x80);
                    EquipParamEntry_unk* base = fn_8020EFEC(item->mId);
                    if (equip->mId < 0 && base != 0) {
                        return true;
                    }
                }
            }
            return false;
        }
    }
    return false;
}

// inferred: removes an upgraded item (and its weapon or equipment)
extern "C" void fn_80212714(ItemParamEntry_unk* item) {
    if (item != 0) {
        if (fn_80206B9C(item->mId)) {
            WeaponParamEntry_unk* weapon = fn_80206C50(item->mId);
            weapon->mId = -1;
        } else if (fn_8020EF04(item->mId)) {
            EquipParamEntry_unk* equip = fn_8020EFEC(item->mId);
            equip->mId = -1;
        }
        item->mId = -1;
    }
}

// inferred: removes all the upgraded items
extern "C" void fn_80212790() {
    ItemParamEntry_unk* item;
    for (unsigned int i = 0; i < 0x100; i++) {
        item = &sEntries[sCount - 0x100 + i];
        if (item->mId > 0) {
            fn_80212714(item);
        }
    }
}

// inferred: builds the upgraded weapon of a level from its base weapon
// NONMATCHING: 90.90% - register allocation and scheduling around the copy (the original keeps
// `base` in a fifth saved register)
extern "C" void fn_80212840(WeaponParamEntry_unk* weapon, WeaponParamEntry_unk* base, int level) {
    WeaponParamEntry_unk* orig = fn_80206B14(base->mKind, base->mId % 1000);
    level += base->mId / 1000;
    *weapon = *orig;
    weapon->mId = orig->mId + level * 1000;
    int bonus = IP_MIN(level, 15) * 200 + IP_MIN(IP_MAX(level - 15, 0), 15) * 100 +
                (IP_MAX(level - 50, 0) * 20 + IP_MIN(IP_MAX(level - 30, 0), 20) * 50);
    float sum = orig->m00 + orig->m04;
    if (sum > 0.001f) {
        float ratio = orig->m00 / sum;
        weapon->m00 += bonus * ratio;
        weapon->m04 += bonus * (1.0f - ratio);
    }
}

// inferred: builds the upgraded equipment of a level from its base equipment
// NONMATCHING: 85.25% - as fn_80212840 (the original computes the bonus before the new id and
// keeps `base` in a fifth saved register)
extern "C" void fn_80212C34(EquipParamEntry_unk* equip, EquipParamEntry_unk* base, int level) {
    EquipParamEntry_unk* orig = fn_8020ED84(base->mKind, base->mId % 1000);
    level += base->mId / 1000;
    *equip = *orig;
    int bonus = level * 30;
    equip->mId = orig->mId + level * 1000;
    if (fn_8020EF4C(orig)) {
        bonus *= 2;
    }
    float sum = orig->m08 + orig->m0C;
    if (sum > 0.001f) {
        float ratio = orig->m08 / sum;
        equip->m08 += bonus * ratio;
        equip->m0C += bonus * (1.0f - ratio);
    }
}
