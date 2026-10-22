#include "atn/Csv.h"
#include "atn/FixedArray.h"
#include "atn/Memory.h"
#include "gmk/SeTable.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: MIN/MAX macros (the clamped value is computed twice)
#define RI_MAX(a, b) ((a) > (b) ? (a) : (b))
#define RI_MIN(a, b) ((a) < (b) ? (a) : (b))

// inferred: one item of a random slot (0x18 bytes); the destructor is not inline (it is the first
// function of the file), the constructor is emitted out of line (fn_8021D574)
struct RandomItem_unk {
    RandomItem_unk() : mItem(0), mRate(10) {}
    ~RandomItem_unk();

    int mItem;
    atn::FixedString_unk<0x10> mName;
    int mRate; // inferred: relative chance
};

RandomItem_unk::~RandomItem_unk() {}

// NONMATCHING (weak destructor of atn::FixedArray_unk<RandomItem_unk, 8>, 0x8021D58C): 96.30% - the
// original tests the address of the element array again before __destroy_arr (one more beq);
// a user-declared empty destructor in include/atn/FixedArray.h does not change it. A derived
// struct (or a member/base with its own inline destructor) gives the two beq, but MWCC then also
// emits an unreferenced weak copy of the inner destructor (0x68 bytes) that the original lacks
// (same as game/GimmickStateCommand and game/LogMessageTask)
typedef atn::FixedArray_unk<RandomItem_unk, 8> RandomSlot_unk;

// inferred: a "begin,Random" block of /boot/random_item_table.csv (0x3E0 bytes): id, level range
// and five slots of up to eight items
struct RandomTable_unk {
    RandomTable_unk() : mId(0), mMin(0), mMax(0) {}
    ~RandomTable_unk() {}

    int mId;
    int mMin;
    int mMax;
    RandomSlot_unk mSlots[5];
};

// inferred: a "begin,Wind" block of /boot/random_item_table.csv (0x3DC bytes)
struct WindTable_unk {
    WindTable_unk() : mMin(0), mMax(0) {}
    ~WindTable_unk() {}

    int mMin;
    int mMax;
    RandomSlot_unk mSlots[5];
};

// inferred: a "begin" block of /boot/marchant_item_table.csv (0x108 bytes, up to 16 items); the
// constructor is emitted out of line (fn_8021DE50)
struct MerchantTable_unk {
    MerchantTable_unk() { memset(this, 0, sizeof(MerchantTable_unk)); }

    int mMin;
    int mMax;
    int mItems[16];
    int mNums[16];
    int m88[16];
    float mRates[16];
};

static const char sFileNameStr[] = "/boot/random_item_table.csv";
static const char sMerchantFileNameStr[] = "/boot/marchant_item_table.csv";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sFileName = sFileNameStr;
static const char* const sMerchantFileName = sMerchantFileNameStr;

static RandomTable_unk* sRandom;
static int sRandomCount;
static WindTable_unk* sWind;
static int sWindCount;
static MerchantTable_unk* sMerchant;
static int sMerchantCount;

// inferred: loads /boot/random_item_table.csv ("begin,Random" and "begin,Wind" blocks of rows with
// five slots, ended by "end")
extern "C" void fn_8021CF8C() {
    atn::Csv csv;
    if (sRandom == 0 && sWind == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.vf08((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';') {
                continue;
            }
            if (strcmp(field, "begin") != 0) {
                continue;
            }
            const char* kind = csv.fn_8005B9CC();
            if (strcmp(kind, "Random") == 0) {
                sRandomCount++;
            } else if (strcmp(kind, "Wind") == 0) {
                sWindCount++;
            }
        }
        sRandom = new (12, "", "", 0) RandomTable_unk[sRandomCount];
        sWind = new (12, "", "", 0) WindTable_unk[sWindCount];
        csv.vf08((const char*)data, size);
        int r = 0;
        int w = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';') {
                continue;
            }
            if (strcmp(field, "begin") != 0) {
                continue;
            }
            const char* kind = csv.fn_8005B9CC();
            if (strcmp(kind, "Random") == 0) {
                RandomTable_unk* table = &sRandom[r];
                table->mMin = atoi(csv.fn_8005B9CC());
                table->mMax = atoi(csv.fn_8005B9CC());
                table->mId = atoi(csv.fn_8005B9CC());
                while (csv.fn_8005BBF8() != 0) {
                    field = csv.fn_8005B9CC();
                    if (*field == ';') {
                        continue;
                    }
                    if (strcmp(field, "end") == 0) {
                        break;
                    }
                    for (int k = 0; k < 5; k++) {
                        int id = atoi(csv.fn_8005B9CC());
                        if (id > 0 && table->mSlots[k].size() < 8) {
                            RandomItem_unk item;
                            item.mItem = id;
                            item.mName = csv.fn_8005B9CC();
                            item.mRate = atoi(csv.fn_8005B9CC());
                            table->mSlots[k].push_back(item);
                        } else {
                            csv.fn_8005B9CC();
                            csv.fn_8005B9CC();
                        }
                    }
                }
                r++;
            } else if (strcmp(kind, "Wind") == 0) {
                WindTable_unk* table = &sWind[w];
                table->mMin = atoi(csv.fn_8005B9CC());
                table->mMax = atoi(csv.fn_8005B9CC());
                while (csv.fn_8005BBF8() != 0) {
                    field = csv.fn_8005B9CC();
                    if (*field == ';') {
                        continue;
                    }
                    if (strcmp(field, "end") == 0) {
                        break;
                    }
                    for (int k = 0; k < 5; k++) {
                        int id = atoi(csv.fn_8005B9CC());
                        if (id > 0 && table->mSlots[k].size() < 8) {
                            RandomItem_unk item;
                            item.mItem = id;
                            item.mName = csv.fn_8005B9CC();
                            item.mRate = atoi(csv.fn_8005B9CC());
                            table->mSlots[k].push_back(item);
                        } else {
                            csv.fn_8005B9CC();
                            csv.fn_8005B9CC();
                        }
                    }
                }
                w++;
            }
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: element access with an unsigned index (it is not strength reduced like a plain index)
static inline RandomItem_unk& GetItem_unk(RandomSlot_unk* slot, unsigned int i) {
    return slot->mData[i];
}

// inferred: random table of a level and id (0 if none)
extern "C" RandomTable_unk* fn_8021D720(int level, int id) {
    RandomTable_unk* table = sRandom;
    for (int i = 0; i < sRandomCount; i++, table++) {
        if (table->mMin <= level && level <= table->mMax && table->mId == id) {
            return table;
        }
    }
    return 0;
}

// inferred: picks a random item of a slot of a table (its name is written with "%s")
extern "C" bool fn_8021D770(RandomTable_unk* table, int* item, char* name, int index) {
    int k = RI_MIN(RI_MAX(index, 0), 5);
    int total = 0;
    int count = table->mSlots[k].size();
    for (int i = 0; i < count; i++) {
        total += GetItem_unk(&table->mSlots[k], i).mRate;
    }
    if (total <= 0) {
        return false;
    }
    int r = rand() % total;
    for (int i = 0; i < count; i++) {
        if (r < GetItem_unk(&table->mSlots[k], i).mRate) {
            RandomItem_unk& it = GetItem_unk(&table->mSlots[k], i);
            *item = it.mItem;
            if (name != 0) {
                sprintf(name, "%s", it.mName.c_str());
            }
            return true;
        }
        r -= GetItem_unk(&table->mSlots[k], i).mRate;
    }
    return false;
}

// inferred: wind table of a level (0 if none)
extern "C" WindTable_unk* fn_8021D990(int level) {
    WindTable_unk* table = sWind;
    for (int i = 0; i < sWindCount; i++, table++) {
        if (table->mMin <= level && level <= table->mMax) {
            return table;
        }
    }
    return 0;
}

extern "C" bool fn_8021D9D4(WindTable_unk* table, int* item, char* name, int index) {
    int k = RI_MIN(RI_MAX(index, 0), 5);
    int total = 0;
    int count = table->mSlots[k].size();
    for (int i = 0; i < count; i++) {
        total += table->mSlots[k].mData[(unsigned int)i].mRate;
    }
    if (total <= 0) {
        return false;
    }
    int r = rand() % total;
    for (int i = 0; i < count; i++) {
        if (r < table->mSlots[k].mData[(unsigned int)i].mRate) {
            RandomItem_unk& it = table->mSlots[k].mData[(unsigned int)i];
            *item = it.mItem;
            sprintf(name, "%s", it.mName.c_str());
            return true;
        }
        r -= table->mSlots[k].mData[(unsigned int)i].mRate;
    }
    return false;
}

// inferred: loads /boot/marchant_item_table.csv
extern "C" void fn_8021DBEC() {
    atn::Csv csv;
    if (sMerchant == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sMerchantFileName, &size, 0x20);
        csv.vf08((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';') {
                continue;
            }
            if (strcmp(field, "begin") == 0) {
                sMerchantCount++;
            }
        }
        sMerchant = new (12, "", "", 0) MerchantTable_unk[sMerchantCount];
        csv.vf08((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';') {
                continue;
            }
            if (strcmp(field, "begin") != 0) {
                continue;
            }
            MerchantTable_unk* table = &sMerchant[i];
            table->mMin = atoi(csv.fn_8005B9CC());
            table->mMax = atoi(csv.fn_8005B9CC());
            int n = 0;
            while (csv.fn_8005BBF8() != 0) {
                field = csv.fn_8005B9CC();
                if (*field == ';') {
                    continue;
                }
                if (strcmp(field, "end") == 0 || n >= 16) {
                    break;
                }
                table->mItems[n] = atoi(csv.fn_8005B9CC());
                table->mNums[n] = atoi(csv.fn_8005B9CC());
                table->m88[n] = atoi(csv.fn_8005B9CC());
                table->mRates[n] = fn_800DC288(csv.fn_8005B9CC());
                n++;
            }
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: merchant table of a level (0 if none)
extern "C" MerchantTable_unk* fn_8021DE88(int level) {
    MerchantTable_unk* table = sMerchant;
    for (int i = 0; i < sMerchantCount; i++, table++) {
        if (table->mMin <= level && level <= table->mMax) {
            return table;
        }
    }
    return 0;
}

// inferred: picks a random item of a merchant table by its chances
extern "C" bool fn_8021DECC(MerchantTable_unk* table, int* item, int* num, int* arg) {
    float total = 0.0f;
    for (int i = 0; i < 16; i++) {
        if (table->mItems[i] <= 0) {
            break;
        }
        total += table->mRates[i];
    }
    if (total <= 0.0f) {
        return false;
    }
    float r = total * ((float)(rand() % 1001) / 1000.0f);
    for (int i = 0; i < 16; i++) {
        if (table->mItems[i] <= 0) {
            break;
        }
        if (r < table->mRates[i]) {
            *item = table->mItems[i];
            *num = table->mNums[i];
            *arg = table->m88[i];
            return true;
        }
        r -= table->mRates[i];
    }
    return false;
}
