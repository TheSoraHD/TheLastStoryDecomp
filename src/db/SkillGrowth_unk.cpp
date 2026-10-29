#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf

struct HitInfo_unk;
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id (db/SkillParam_unk)

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

namespace atn {

// inferred: pointer to a string owned elsewhere (vptr + pointer); same declaration as in
// atn/ptr_string_base_wchar_t_0.cpp
template <class T, int N>
class ptr_string_base {
public:
    ptr_string_base() : mPtr(0) {}
    virtual ~ptr_string_base() {}

    T* mPtr;
};

} // namespace atn

// inferred: one row of /boot/skill_growth.csv (0x18 bytes) with its two texts of
// /boot/skill_growth_text_xx.u16; constructor and empty destructor emitted out of line for
// __construct_new_array (fn_8021AEF0, fn_8021AF10)
struct SkillGrowth_unk {
    SkillGrowth_unk() {}
    ~SkillGrowth_unk() {}

    int mId;
    int mSkill; // inferred: skill id (fn_80219E6C)
    atn::ptr_string_base<wchar_t, 0> mName;
    atn::ptr_string_base<wchar_t, 0> mText;
};

static const char sFileNameStr[] = "/boot/skill_growth.csv";
static const char sTextFileNameStr[] = "/boot/skill_growth_text_xx.u16";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sFileName = sFileNameStr;
static const char* const sTextFileName = sTextFileNameStr;

static SkillGrowth_unk* sEntries;
static int sCount;
static wchar_t* sTexts;

// inferred: loads /boot/skill_growth.csv (rows whose first column is empty) and the texts
extern "C" void fn_8021AAB8() {
    atn::Csv csv;
    atn::CsvU wcsv;
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "") == 0) {
                sCount++;
            }
        }
        sEntries = new ((MemoryTag)12, "", "", 0) SkillGrowth_unk[sCount];
        csv.SetData((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "") != 0) {
                continue;
            }
            SkillGrowth_unk* entry = &sEntries[i];
            entry->mId = atoi(csv.fn_8005B9CC());
            csv.fn_8005B9CC(); // inferred: a column that is not used
            entry->mSkill = atoi(csv.fn_8005B9CC());
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);

        data = fn_8046DC5C(lbl_8087F518, sTextFileName, &size, 0x20);
        int total = 0;
        wcsv.SetData((const wchar_t*)data, (int)size / 2);
        while (wcsv.fn_8005BBF8() != 0) {
            if (*wcsv.fn_8005B9CC() != 0) {
                continue;
            }
            wcsv.fn_8005B9CC(); // inferred: a column that is not used
            total += wcslen(wcsv.fn_8005B9CC()) + 1;
            total += wcslen(wcsv.fn_8005B9CC()) + 1;
        }
        sTexts = new ((MemoryTag)12, "", "", 0) wchar_t[total];
        int pos;
        int ofs = 0; // inferred: byte offset of the row (not an index: its 0 is set before vf08)
        pos = 0;
        wcsv.SetData((const wchar_t*)data, (int)size / 2);
        while (wcsv.fn_8005BBF8() != 0) {
            if (*wcsv.fn_8005B9CC() != 0) {
                continue;
            }
            wcsv.fn_8005B9CC(); // inferred: a column that is not used
            SkillGrowth_unk* entry = (SkillGrowth_unk*)((char*)sEntries + ofs);
            const wchar_t* text = wcsv.fn_8005B9CC();
            fn_800DD3FC(&sTexts[pos], L"%s", text);
            entry->mName.mPtr = &sTexts[pos];
            pos += wcslen(text) + 1;
            text = wcsv.fn_8005B9CC();
            fn_800DD3FC(&sTexts[pos], L"%s", text);
            entry->mText.mPtr = &sTexts[pos];
            pos += wcslen(text) + 1;
            ofs += sizeof(SkillGrowth_unk);
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: entry by id (0 if there is none)
extern "C" SkillGrowth_unk* fn_8021AF50(int id) {
    for (int i = 0; i < sCount; i++) {
        if (id == sEntries[i].mId) {
            return &sEntries[i];
        }
    }
    return 0;
}

extern "C" int fn_8021AF98(int id) {
    SkillGrowth_unk* entry = fn_8021AF50(id);
    if (entry != 0) {
        return entry->mSkill;
    }
    return 0;
}

extern "C" HitInfo_unk* fn_8021AFF4(int id) {
    int skill = fn_8021AF98(id);
    if (skill > 0) {
        return fn_80219E6C(skill);
    }
    return 0;
}
