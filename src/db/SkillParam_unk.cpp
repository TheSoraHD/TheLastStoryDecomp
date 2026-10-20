#include "atn/Csv.h"
#include "atn/Memory.h"
#include "db/ParseUtil_unk.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" int atoi(const char* s);
extern "C" unsigned long wcslen(const wchar_t* s);

extern "C" void* fn_8006BA8C(const char* path, unsigned int* size, int flag); // inferred: loads a file
extern "C" void fn_8006BB6C(void* data, int flag);                           // inferred: frees it

// inferred: one row of /boot/skill_param.csv (0xD0 bytes); the constructor (0x80219EC4) is not
// inline
struct SkillParam_unk : public BulletParam_unk {
    SkillParam_unk();

    int mBC;
    int mC0; // inferred: chant time (HitInfo_unk::mC0)
    int mC4; // inferred: from a field with a two letter prefix, -1 if it is empty
    int mC8;
    int mCC;
};

static const char sTextFileNameStr[] = "/boot/skill_param_text_xx.u16";
static const char* const sTextFileName = sTextFileNameStr; // inferred: .rodata array + .sdata2 pointer

static SkillParam_unk* sEntries;
static int sCount;
static char* sBuf;
static wchar_t* sTexts;

extern "C" void fn_80219C30();

// inferred: loads /boot/skill_param.csv (the first line may hold the "Version" of the format)
extern "C" void fn_8021996C() {
    unsigned int size = 0;
    void* data = fn_8006BA8C("/boot/skill_param.csv", &size, 0);
    atn::Csv csv((const char*)data, size);
    const char* id; // inferred: the current field without its two letter prefix
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
    sEntries = new (1, "", "", 0) SkillParam_unk[sCount];
    sBuf = new (1, "", "", 0) char[a];
    csv.vf08((const char*)data, size);
    csv.fn_8005BBF8();
    a = 0;
    b = 0;
    int i = 0;
    id = csv.mField + 2;
    while (csv.fn_8005BBF8() != 0) {
        SkillParam_unk* entry = &sEntries[i];
        entry->m00 = 0;
        fn_80219F84(entry, &csv, version, sBuf, sTexts, &a, &b);
        entry->mBC = atoi(csv.fn_8005B9CC());
        entry->mC0 = atoi(csv.fn_8005B9CC());
        if (strcmp(csv.fn_8005B9CC(), "") == 0) {
            entry->mC4 = -1;
        } else {
            entry->mC4 = atoi(id);
        }
        entry->mC8 = atoi(csv.fn_8005B9CC());
        entry->mCC = atoi(csv.fn_8005B9CC());
        entry->m70 = atoi(csv.fn_8005B9CC());
        entry->m60 = atoi(csv.fn_8005B9CC());
        csv.fn_8005B9CC(); // inferred: a column that is not used
        i++;
    }
    fn_8006BB6C(data, 0);
    fn_80219C30();
}

extern "C" void fn_80219F1C(atn::CsvU* csv, int* a, int* b);

// inferred: loads the texts of the skills (/boot/skill_param_text_xx.u16)
// NONMATCHING: 87.77% - the original calls fn_80219F1C out of line (here it is inlined: the inline
// budget is not exceeded, as in db/CharaIdTable_unk) and keeps the address of the atn::CsvU in a
// saved register while its inline constructor runs (as in db/TutorialTable_unk).
extern "C" void fn_80219C30() {
    if (sEntries != 0) {
        if (sTexts != 0) {
            delete[] sTexts;
            sTexts = 0;
        }
        unsigned int size = 0;
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

// inferred: bullet/skill data by id (0 if there is none)
extern "C" SkillParam_unk* fn_80219E6C(int id) {
    for (int i = 0; i < sCount; i++) {
        if (id == sEntries[i].m04) {
            return &sEntries[i];
        }
    }
    return 0;
}

extern "C" SkillParam_unk* fn_80219EB4() {
    return sEntries;
}

extern "C" int fn_80219EBC() {
    return sCount;
}

SkillParam_unk::SkillParam_unk() {
    mBC = 0;
    mC0 = 0;
    mC4 = 1;
    mCC = 0;
    m00 = 0;
    m01 = 0;
    m64 = 4;
}

// inferred: adds the lengths of the two texts of a row (with room for the terminators)
extern "C" void fn_80219F1C(atn::CsvU* csv, int* a, int* b) {
    *b += wcslen(csv->fn_8005B9CC()) + 2;
    *b += wcslen(csv->fn_8005B9CC()) + 2;
}
