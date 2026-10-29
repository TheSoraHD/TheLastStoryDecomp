#include "atn/Csv.h"
#include "game/Inventory.h"
#include "atn/FileList.h"
#include "atn/Memory.h"
#include "db/WeaponParam_unk.h"
#include "gmk/SeTable.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to number

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);
// inferred: loads a whole file / releases it
extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int arg);

// inferred: field (lbl_8087F430) and its flags
struct WeaponField_unk;
extern "C" WeaponField_unk* lbl_8087F430;
extern "C" int fn_80370174(WeaponField_unk* field, int flag);

// inferred: one row of /boot/weapon_effect.csv (0x20 bytes)
struct WeaponEffect_unk {
    WeaponEffect_unk() : mKind(-1), mId(0), mCount(0) {}
    ~WeaponEffect_unk();

    int mKind;
    int mId;
    int mCount;
    int m0C;
    atn::FixedString_unk<0x10> mName;
};

static const char* const sRanks[5] = {"\x81\x7C", "S", "A", "B", "C"};
static const char sFileNameStr[] = "/boot/weapon_param.csv";
static const char sTextFileNameStr[] = "/boot/weapon_param_text_xx.u16";
static const char* const sFileName = sFileNameStr;         // inferred: .rodata array + .sdata2 pointer
static const char* const sTextFileName = sTextFileNameStr; // inferred: .rodata array + .sdata2 pointer
static const char* const sTypes[8] = {"", "DAGGER", "SWORD", "S.SWORD", "G.SWORD", "AXE", "HAMMER", "X-BOLT"};

static WeaponParamEntry_unk* sEntries;
static wchar_t* sTexts;
static int sCount;

extern "C" void fn_8020689C();

typedef boost::tokenizer<boost::char_separator<char> > Tokenizer_unk;

// inferred: index of a name in a table, 0 if not found
static inline int FindName_unk(const char* name, const char* const* table, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(name, table[i]) == 0) {
            return i;
        }
    }
    return 0;
}

// inferred: kind ("ws", "wg" or other) and number of a weapon id
static inline void ParseId_unk(const char* id, int* kind, int* number) {
    if (id != 0 && strlen(id) > 2) {
        if (strncmp(id, "ws", 2) == 0) {
            *kind = 0;
        } else if (strncmp(id, "wg", 2) == 0) {
            *kind = 1;
        } else {
            *kind = 2;
        }
        *number = atoi(id + 2);
    }
}

// inferred: loads /boot/weapon_param.csv (and then the texts)
extern "C" void fn_80204E54() {
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    int count = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (strcmp(csv.fn_8005B9CC(), "") == 0) {
            count++;
        }
    }
    count += 0x80;
    sEntries = new (3, "", "", 0) WeaponParamEntry_unk[count];
    sCount = count;
    csv.SetData((const char*)data, size);
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (strcmp(csv.fn_8005B9CC(), "") != 0) {
            continue;
        }
        WeaponParamEntry_unk* entry = &sEntries[i];
        ParseId_unk(csv.fn_8005B9CC(), &entry->mKind, &entry->mId);
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        entry->mModel = csv.fn_8005B9CC();
        entry->m7C = atoi(csv.fn_8005B9CC());
        entry->m00 = fn_800DC288(csv.fn_8005B9CC());
        entry->m04 = fn_800DC288(csv.fn_8005B9CC());
        entry->m08 = fn_800DC288(csv.fn_8005B9CC());
        entry->m0C = fn_800DC288(csv.fn_8005B9CC());
        entry->m10 = fn_800DC288(csv.fn_8005B9CC());
        entry->m14 = fn_800DC288(csv.fn_8005B9CC());
        entry->m28 = fn_800DC288(csv.fn_8005B9CC());
        entry->m2C = fn_800DC288(csv.fn_8005B9CC());
        entry->m20 = atoi(csv.fn_8005B9CC());
        entry->m24 = atoi(csv.fn_8005B9CC());
        entry->m18 = fn_800DC288(csv.fn_8005B9CC());
        entry->m1C = fn_800DC288(csv.fn_8005B9CC());
        entry->m30 = atoi(csv.fn_8005B9CC());
        entry->m34 = atoi(csv.fn_8005B9CC());
        std::string str(csv.fn_8005B9CC());
        // NONMATCHING (weak copy emitted here, 0x80205BE8): the post-increment of the token iterator
        // (boost::char_separator, libs/boost) is at 99.23% - register numbering only
        Tokenizer_unk tok(str, boost::char_separator<char>(":"));
        int n = 0;
        for (Tokenizer_unk::iterator it = tok.begin(); it != tok.end() && n < 7; it++) {
            entry->m38.mA[n] = fn_800DC288(it->c_str());
            n++;
        }
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        entry->mAC[0] = atoi(csv.fn_8005B9CC());
        entry->mAC[1] = atoi(csv.fn_8005B9CC());
        entry->mAC[2] = atoi(csv.fn_8005B9CC());
        entry->mType = FindName_unk(csv.fn_8005B9CC(), sTypes, 8);
        const char* users = csv.fn_8005B9CC();
        if (*users == 0) {
            entry->mFlags = 0x1F;
        }
        if (strcmp(users, "elza") == 0) {
            entry->mFlags = 0x8000;
        } else if (strcmp(users, "yuris") == 0) {
            entry->mFlags = 0x10000;
        } else if (strcmp(users, "kanan") == 0) {
            entry->mFlags = 0x20000;
        } else if (strcmp(users, "quark") == 0) {
            entry->mFlags = 0x40000;
        } else if (strcmp(users, "seiren") == 0) {
            entry->mFlags = 0x80000;
        } else if (strcmp(users, "manamia") == 0) {
            entry->mFlags = 0x100000;
        } else if (strcmp(users, "jackal") == 0) {
            entry->mFlags = 0x200000;
        } else {
            for (int k = 0; k < strlen(users); k++) {
                if (users[k] != '-') {
                    entry->mFlags |= 1 << k;
                }
            }
        }
        entry->mC0 = atoi(csv.fn_8005B9CC());
        for (int j = 0; j < 2; j++) {
            entry->mC4[j].m00 = atoi(csv.fn_8005B9CC());
            entry->mC4[j].m04 = atoi(csv.fn_8005B9CC());
            entry->mC4[j].m08 = atoi(csv.fn_8005B9CC());
            entry->mC4[j].m0C = atoi(csv.fn_8005B9CC());
            entry->mC4[j].m10 = atoi(csv.fn_8005B9CC());
        }
        entry->mEC = atoi(csv.fn_8005B9CC());
        entry->mF0 = atoi(csv.fn_8005B9CC());
        entry->mF4 = csv.fn_8005B9CC();
        entry->m104 = csv.fn_8005B9CC();
        entry->m114 = atoi(csv.fn_8005B9CC());
        entry->m118 = FindName_unk(csv.fn_8005B9CC(), sRanks, 5);
        const char* link = csv.fn_8005B9CC();
        if (*link != 0) {
            entry->m120 = atoi(link + 2);
            if (strncmp(link, "ws", 2) == 0) {
                entry->m11C = 0;
            } else if (strncmp(link, "wg", 2) == 0) {
                entry->m11C = 1;
            } else {
                entry->m11C = 2;
            }
        }
        csv.fn_8005B9CC();
        for (int j = 0; j < 4; j++) {
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
        }
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        entry->m124 = atoi(csv.fn_8005B9CC());
        entry->m128 = atoi(csv.fn_8005B9CC());
        csv.fn_8005B9CC();
        entry->m12C = fn_800DC12C(csv.fn_8005B9CC());
        i++;
    }
    for (; i < sCount; i++) {
        sEntries[i].mId = -1;
    }
    fn_8046DD20(lbl_8087F518, data);
    fn_8020689C();
}

// inferred: defined after the loader (its weak copy follows the one of the tokenizer iterator)
inline WeaponParamEntry_unk::~WeaponParamEntry_unk() {}

// inferred: loads the names and texts (UTF-16) of the weapons
// NONMATCHING: 96.22% - the original keeps the address of the atn::CsvU in a saved register while its
// inlined constructor runs (same as db/EquipParam_unk.cpp)
extern "C" void fn_8020689C() {
    if (sEntries == 0) {
        return;
    }
    unsigned long size = 0;
    void* data = fn_8006BA8C(sTextFileName, &size, 0);
    atn::CsvU csv((const wchar_t*)data, size / 2);
    int length = 0;
    while (csv.fn_8005BBF8() != 0) {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == 0 || *field == L'#') {
            continue;
        }
        length += wcslen(csv.fn_8005B9CC()) + 1;
        csv.fn_8005B9CC();
        length += wcslen(csv.fn_8005B9CC()) + 1;
    }
    sTexts = new (3, "", "", 0) wchar_t[length];
    int pos = 0;
    csv.Reset_unk();
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == 0 || *field == L'#') {
            continue;
        }
        WeaponParamEntry_unk* entry = &sEntries[i];
        wcscpy(sTexts + pos, csv.fn_8005B9CC());
        entry->mName = sTexts + pos;
        pos += wcslen(sTexts + pos) + 1;
        csv.fn_8005B9CC();
        wcscpy(sTexts + pos, csv.fn_8005B9CC());
        entry->mText = sTexts + pos;
        pos += wcslen(sTexts + pos) + 1;
        i++;
    }
    atn::FileFreeBuffer((char*)data, false);
}

// inferred: entry of a kind and id
extern "C" WeaponParamEntry_unk* fn_80206B14(int kind, int id) {
    for (unsigned int i = 0; i < (unsigned int)sCount; i++) {
        if (kind == sEntries[i].mKind && id == sEntries[i].mId) {
            return &sEntries[i];
        }
    }
    return 0;
}

extern "C" int fn_80206B68() {
    return sCount;
}

extern "C" WeaponParamEntry_unk* fn_80206B70(int index) {
    if (index < 0 || index >= sCount) {
        return 0;
    }
    return &sEntries[index];
}

extern "C" bool fn_80206B9C(int code) {
    return code / 100000 % 10 == 1;
}

// inferred: item code of an entry
extern "C" int fn_80206BE4(WeaponParamEntry_unk* entry) {
    if (entry == 0) {
        return 0;
    }
    int code = 100000;
    code += entry->mKind * 1000;
    code += entry->mId % 1000;
    code += entry->mId / 1000 * 1000000;
    return code;
}

// inferred: entry of an item code
extern "C" WeaponParamEntry_unk* fn_80206C50(int code) {
    code -= 100000;
    int kind = code / 1000 % 10;
    int id = code % 1000 + code / 1000000 * 1000;
    return fn_80206B14(kind, id);
}

// inferred: whether a member can equip the weapon
extern "C" bool fn_80206D18(WeaponParamEntry_unk* entry, int member, int arg) {
    if (entry == 0) {
        return false;
    }
    bool result = false;
    switch (member) {
    case 0:
        if (entry->mFlags & 0x8001) {
            if (arg == 0) {
                if (entry->mKind != 1) {
                    result = true;
                }
                fn_80206B14(0, Inventory::instance_->mEquip[0].mWeapons[arg].mId);
                if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x395) && fn_80370174(lbl_8087F430, 0x396)) {
                    result = false;
                }
            } else if (entry->mKind == 1) {
                result = true;
            }
        }
        break;
    case 3:
        if (entry->mFlags & 0x80010) {
            if (entry->mKind != 1) {
                result = true;
            }
        }
        break;
    case 2:
        if (entry->mFlags & 0x40001) {
            if (arg == 0 && entry->mKind != 1) {
                result = true;
            }
        }
        break;
    case 6:
        if (entry->mFlags & 0x200008) {
            if (arg == 0 && entry->mKind != 1) {
                result = true;
            }
        }
        break;
    case 1:
        if (entry->mFlags & 0x20002) {
            if (arg == 0 && entry->mKind != 1) {
                result = true;
            }
        }
        break;
    case 5:
        if (entry->mFlags & 0x100004) {
            if (arg == 0 && entry->mKind != 1) {
                result = true;
            }
        }
        break;
    case 4:
        if (entry->mFlags & 0x10002) {
            if (arg == 0 && entry->mKind != 1) {
                result = true;
            }
        }
        break;
    }
    return result;
}

static const char sEffectFileNameStr[] = "/boot/weapon_effect.csv";
static const char* const sEffectFileName = sEffectFileNameStr; // inferred: .rodata array + .sdata2 pointer

static WeaponEffect_unk* sEffects;
static int sEffectCount;

// inferred: loads /boot/weapon_effect.csv
extern "C" void fn_80206F44() {
    if (sEffects != 0) {
        return;
    }
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, sEffectFileName, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    int count = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != 0) {
            continue;
        }
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        if (atoi(csv.fn_8005B9CC()) <= 0) {
            continue;
        }
        if (*csv.fn_8005B9CC() == 0) {
            continue;
        }
        count++;
    }
    sEffects = new (3, "", "", 0) WeaponEffect_unk[count];
    sEffectCount = count;
    csv.SetData((const char*)data, size);
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != 0) {
            continue;
        }
        WeaponEffect_unk effect;
        ParseId_unk(csv.fn_8005B9CC(), &effect.mKind, &effect.mId);
        csv.fn_8005B9CC();
        effect.mCount = atoi(csv.fn_8005B9CC());
        if (effect.mCount <= 0) {
            continue;
        }
        effect.mName = csv.fn_8005B9CC();
        if (strlen(effect.mName.c_str()) == 0) {
            continue;
        }
        sEffects[i] = effect;
        i++;
    }
    fn_8046DD20(lbl_8087F518, data);
}

// inferred: defined after its loader (its weak copy follows the one of the constructor)
inline WeaponEffect_unk::~WeaponEffect_unk() {}

// inferred: effect of a weapon kind and id
extern "C" WeaponEffect_unk* fn_802072AC(int kind, int id) {
    id %= 1000;
    for (unsigned int i = 0; i < (unsigned int)sEffectCount; i++) {
        WeaponEffect_unk* effect = &sEffects[i];
        if (effect->mKind == kind && effect->mId == id) {
            return effect;
        }
    }
    return 0;
}
