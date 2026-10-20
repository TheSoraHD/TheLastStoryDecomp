#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/ai_table.csv (0x90 bytes, no constructor)
struct AiTableEntry_unk {
    int mType;       // inferred: from the id prefix ("np", "em", "fr", "pc", "gn"), -1 if unknown
    int mId;         // inferred: number after the prefix, without '_'
    char mName[0x20];
    float m28;
    float m2C;
    float m30;
    float m34;
    float m38;
    int m3C;
    int m40;
    int m44;
    int m48;
    float m4C;
    float m50;
    float m54;
    float m58;
    float m5C;
    float m60;
    float m64[9];
    int m88;
    int m8C;
};

static const char sFileNameStr[] = "/boot/ai_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static AiTableEntry_unk* sEntries;
static unsigned int sCount;

// inferred: type of an id from its two-letter prefix ("em_0012" -> 2), -1 if unknown.
// GetType_unk and GetNumber_unk are not static: they are emitted and stripped by the linker
// (their literals come first in the string pool)
int GetType_unk(const char* id) {
    return strncmp(id, "np", 2) == 0   ? 1
           : strncmp(id, "em", 2) == 0 ? 2
           : strncmp(id, "fr", 2) == 0 ? 3
           : strncmp(id, "pc", 2) == 0 ? 3
           : strncmp(id, "gn", 2) != 0 ? -1
                                       : 5;
}

// inferred: number of an id ("em_0012" -> 12): the text after the prefix without '_'
int GetNumber_unk(const char* id) {
    std::string str(id + 2);
    for (;;) {
        std::string::size_type pos = str.find("_");
        if (pos == std::string::npos) {
            break;
        }
        str.erase(pos, 1);
    }
    return atoi(str.c_str());
}

// inferred: loads /boot/ai_table.csv
extern "C" void fn_80204958() {
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    int count = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (strcmp(csv.fn_8005B9CC(), "") != 0) {
            continue;
        }
        if (strcmp(csv.fn_8005B9CC(), "") != 0) {
            count++;
        }
    }
    sEntries = new (3, "", "", 0) AiTableEntry_unk[count];
    sCount = count;
    csv.vf08((const char*)data, size);
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (strcmp(csv.fn_8005B9CC(), "") != 0) {
            continue;
        }
        const char* field = csv.fn_8005B9CC();
        if (strlen(field) < 3) {
            continue;
        }
        AiTableEntry_unk* entry = &sEntries[i];
        entry->mType = GetType_unk(field);
        if (entry->mType >= 0) {
            entry->mId = GetNumber_unk(field);
            strncpy(entry->mName, csv.fn_8005B9CC(), 0x20);
            csv.fn_8005B9CC(); // inferred: a column that is not used
            entry->m28 = fn_800DC288(csv.fn_8005B9CC());
            entry->m2C = fn_800DC288(csv.fn_8005B9CC());
            entry->m30 = fn_800DC288(csv.fn_8005B9CC());
            entry->m34 = fn_800DC288(csv.fn_8005B9CC());
            entry->m38 = fn_800DC288(csv.fn_8005B9CC());
            entry->m3C = atoi(csv.fn_8005B9CC());
            entry->m40 = atoi(csv.fn_8005B9CC());
            entry->m44 = atoi(csv.fn_8005B9CC());
            entry->m48 = atoi(csv.fn_8005B9CC());
            entry->m4C = fn_800DC288(csv.fn_8005B9CC());
            entry->m50 = fn_800DC288(csv.fn_8005B9CC());
            entry->m54 = fn_800DC288(csv.fn_8005B9CC());
            entry->m58 = fn_800DC288(csv.fn_8005B9CC());
            entry->m5C = fn_800DC288(csv.fn_8005B9CC());
            entry->m60 = fn_800DC288(csv.fn_8005B9CC());
            for (int j = 0; j < 9; j++) {
                entry->m64[j] = fn_800DC288(csv.fn_8005B9CC());
            }
            entry->m88 = atoi(csv.fn_8005B9CC());
            entry->m8C = atoi(csv.fn_8005B9CC());
        } else {
            strncpy(entry->mName, field, 0x20);
        }
        i++;
    }
    fn_8046DD20(lbl_8087F518, data);
}

// inferred: entry by type (0 = 3) and id
extern "C" AiTableEntry_unk* fn_80204E04(int type, int id) {
    if (type == 0) {
        type = 3;
    }
    AiTableEntry_unk* entry = sEntries;
    for (unsigned int i = 0; i < sCount; i++, entry++) {
        if (entry->mType == type && entry->mId == id) {
            return entry;
        }
    }
    return 0;
}
