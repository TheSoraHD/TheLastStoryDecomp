#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one priority of a player: kind 1-5 = ATK/DEF/MATK/MDEF/AGI, 6 = a number in mValue
struct PlayerPriorityItem_unk {
    int mKind;
    int mValue;
};

// inferred: one row of /boot/player_*_priority.csv (0xF4 bytes, 7 rows per table)
struct PlayerPriority_unk {
    int mId;
    PlayerPriorityItem_unk mItems[30];
};

static const char sProtectorFileNameStr[] = "/boot/player_protector_priority.csv";
static const char sWeaponFileNameStr[] = "/boot/player_weapon_priority.csv";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sProtectorFileName = sProtectorFileNameStr;
static const char* const sWeaponFileName = sWeaponFileNameStr;

static PlayerPriority_unk* sProtectors;
static PlayerPriority_unk* sWeapons;

// inferred: reads the rest of a row (the id and up to 30 priorities, until an empty field)
extern "C" void fn_802188B4(atn::Csv* csv, PlayerPriority_unk* entry) {
    const char* field;
    entry->mId = fn_800DC12C(csv->fn_8005B9CC());
    for (int i = 0; i < 30; i++) {
        field = csv->fn_8005B9CC();
        if (*field == '\0') {
            break;
        }
        if (strncmp(field, "ATK", 3) == 0) {
            entry->mItems[i].mKind = 1;
        } else if (strncmp(field, "DEF", 3) == 0) {
            entry->mItems[i].mKind = 2;
        } else if (strncmp(field, "MATK", 4) == 0) {
            entry->mItems[i].mKind = 3;
        } else if (strncmp(field, "MDEF", 4) == 0) {
            entry->mItems[i].mKind = 4;
        } else if (strncmp(field, "AGI", 3) == 0) {
            entry->mItems[i].mKind = 5;
        } else {
            int value = fn_800DC12C(field);
            if (value > 0) {
                entry->mItems[i].mKind = 6;
                entry->mItems[i].mValue = value;
            }
        }
    }
}

// inferred: loads /boot/player_weapon_priority.csv and /boot/player_protector_priority.csv
extern "C" void fn_802189EC() {
    atn::Csv csv;
    sProtectors = new (1, "", "", 0) PlayerPriority_unk[7];
    sWeapons = new (1, "", "", 0) PlayerPriority_unk[7];
    memset(sWeapons, 0, sizeof(PlayerPriority_unk) * 7);
    memset(sProtectors, 0, sizeof(PlayerPriority_unk) * 7);

    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, sWeaponFileName, &size, 0x20);
    csv.SetData((const char*)data, size);
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        csv.fn_8005B9CC(); // inferred: a column that is not used
        fn_802188B4(&csv, &sWeapons[i]);
        i++;
    }
    fn_8046DD20(lbl_8087F518, data);

    data = fn_8046DC5C(lbl_8087F518, sProtectorFileName, &size, 0x20);
    csv.SetData((const char*)data, size);
    i = 0;
    while (csv.fn_8005BBF8() != 0) {
        csv.fn_8005B9CC(); // inferred: a column that is not used
        fn_802188B4(&csv, &sProtectors[i]);
        i++;
    }
    fn_8046DD20(lbl_8087F518, data);
}

extern "C" PlayerPriority_unk* fn_80218BB4(int index) {
    return &sProtectors[index];
}

extern "C" PlayerPriority_unk* fn_80218BC4(int index) {
    return &sWeapons[index];
}
