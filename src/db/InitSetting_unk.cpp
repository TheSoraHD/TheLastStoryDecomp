#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: item and count of the "Item" section
struct InitItem_unk {
    InitItem_unk() : mId(0), mCount(0) {}
    int mId;
    int mCount;
};

// inferred: equipment of the "Protector"/"Weapon" sections: kind (index of the two-letter
// prefix), id (number after the prefix) and level
struct InitEquip_unk {
    InitEquip_unk() : mKind(-1), mId(-1), mLevel(0) {}
    int mKind;
    int mId;
    int mLevel;
};

// inferred: drop rates (a member object: it is initialised between the item and equipment arrays)
struct InitDrop_unk {
    InitDrop_unk() {
        mRare[0] = 5.0f;
        mRare[1] = 5.0f;
        mRare[2] = 5.0f;
        for (int i = 0; i < 10; i++) {
            mRate[i] = 10;
        }
    }
    int mRate[10];  // inferred: "DropItem" section, indexed by the first column
    float mRare[3]; // inferred: "DropItemRare" section
};

// inferred: initial settings of a new game, read from /boot/init_setting.csv (0x434 bytes)
struct InitSetting_unk {
    InitItem_unk mItems[32];
    InitDrop_unk mDrop;
    InitEquip_unk mProtectors[32];
    InitEquip_unk mWeapons[32];
};

static const char* const sProtectorKinds[] = {"qu", "ql", "qg", "qw", "qs"};
static const char* const sWeaponKinds[] = {"ws", "wg", "wm"};
static const char sFileNameStr[] = "/boot/init_setting.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static InitSetting_unk* sInstance;

// inferred: loads /boot/init_setting.csv (sections between "begin,<name>" and "end")
extern "C" void fn_80210244() {
    atn::Csv csv;
    if (sInstance == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        sInstance = new ((MemoryTag)12, "", "", 0) InitSetting_unk;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "begin") != 0) {
                continue;
            }
            const char* section = csv.fn_8005B9CC();
            if (strcmp(section, "Item") == 0) {
                int n = 0;
                while (csv.fn_8005BBF8() != 0) {
                    field = csv.fn_8005B9CC();
                    if (*field == ';' || *field == '#') {
                        continue;
                    }
                    if (strcmp(field, "end") == 0) {
                        break;
                    }
                    int id = atoi(csv.mField);
                    int count = atoi(csv.fn_8005B9CC());
                    if (id > 0 && count > 0) {
                        sInstance->mItems[n].mId = id;
                        sInstance->mItems[n].mCount = count;
                        if (++n >= 32) {
                            break;
                        }
                    }
                }
            } else if (strcmp(section, "DropItem") == 0) {
                while (csv.fn_8005BBF8() != 0) {
                    field = csv.fn_8005B9CC();
                    if (*field == ';' || *field == '#') {
                        continue;
                    }
                    if (strcmp(field, "end") == 0) {
                        break;
                    }
                    int index = atoi(csv.mField);
                    int value = atoi(csv.fn_8005B9CC());
                    if (index >= 0 && 10 > index && value >= 0) {
                        sInstance->mDrop.mRate[index] = value;
                    }
                }
            } else if (strcmp(section, "DropItemRare") == 0) {
                int n = 0;
                while (csv.fn_8005BBF8() != 0) {
                    field = csv.fn_8005B9CC();
                    if (*field == ';' || *field == '#') {
                        continue;
                    }
                    if (strcmp(field, "end") == 0) {
                        break;
                    }
                    if (n == 0) {
                        sInstance->mDrop.mRare[0] = fn_800DC288(csv.mField);
                    } else if (n == 1) {
                        sInstance->mDrop.mRare[1] = fn_800DC288(csv.mField);
                    } else if (n == 2) {
                        sInstance->mDrop.mRare[2] = fn_800DC288(csv.mField);
                    }
                    n++;
                }
            } else if (strcmp(section, "Protector") == 0) {
                int n = 0;
                while (csv.fn_8005BBF8() != 0) {
                    field = csv.fn_8005B9CC();
                    if (*field == ';' || *field == '#' || *field == '\0') {
                        continue;
                    }
                    if (strcmp(field, "end") == 0) {
                        break;
                    }
                    int kind = -1;
                    for (int i = 0; i < 5; i++) {
                        if (strncmp(field, sProtectorKinds[i], 2) == 0) {
                            kind = i;
                            break;
                        }
                    }
                    sInstance->mProtectors[n].mKind = kind;
                    sInstance->mProtectors[n].mId = atoi(field + 2);
                    sInstance->mProtectors[n].mLevel = atoi(csv.fn_8005B9CC());
                    if (++n >= 32) {
                        break;
                    }
                }
            } else if (strcmp(section, "Weapon") == 0) {
                int n = 0;
                while (csv.fn_8005BBF8() != 0) {
                    field = csv.fn_8005B9CC();
                    if (*field == ';' || *field == '#' || *field == '\0') {
                        continue;
                    }
                    if (strcmp(field, "end") == 0) {
                        break;
                    }
                    int kind = -1;
                    for (int i = 0; i < 3; i++) {
                        if (strncmp(field, sWeaponKinds[i], 2) == 0) {
                            kind = i;
                            break;
                        }
                    }
                    sInstance->mWeapons[n].mKind = kind;
                    sInstance->mWeapons[n].mId = atoi(field + 2);
                    sInstance->mWeapons[n].mLevel = atoi(csv.fn_8005B9CC());
                    if (++n >= 32) {
                        break;
                    }
                }
            }
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

extern "C" InitSetting_unk* fn_80210AC4() {
    return sInstance;
}
