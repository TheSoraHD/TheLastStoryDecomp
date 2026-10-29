#include "atn/Csv.h"
#include "atn/FileList.h"
#include "atn/Memory.h"
#include "db/EquipParam_unk.h"
#include "gmk/SeTable.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);
// inferred: loads a whole file / releases it
extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int arg);

static const char* const sKinds[5] = {"qu", "ql", "qg", "qw", "qs"};
static const char* const sSlots[5] = {"up", "lo", "ga", "wa", "qs"};
static const char sLetters[] = "ABDFGJLMWXY";
static const char sFileNameStr[] = "/boot/equip_param.csv";
static const char sTextFileNameStr[] = "/boot/equip_param_text_xx.u16";
static const char* const sFileName = sFileNameStr;         // inferred: .rodata array + .sdata2 pointer
static const char* const sTextFileName = sTextFileNameStr; // inferred: .rodata array + .sdata2 pointer

static EquipParamEntry_unk* sEntries;
static wchar_t* sTexts;
static int sCount;

extern "C" void fn_8020EB0C();

typedef boost::tokenizer<boost::char_separator<char> > Tokenizer_unk;

// inferred: loads /boot/equip_param.csv (and then the texts)
extern "C" void fn_8020DF88() {
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
    sEntries = new ((MemoryTag)3, "", "", 0) EquipParamEntry_unk[count];
    sCount = count;
    csv.SetData((const char*)data, size);
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (strcmp(csv.fn_8005B9CC(), "") != 0) {
            continue;
        }
        EquipParamEntry_unk* entry = &sEntries[i];
        entry->mKind = -1;
        entry->mId = 0;
        const char* id = csv.fn_8005B9CC();
        for (int k = 0; k < 5; k++) {
            if (strncmp(id, sKinds[k], 2) == 0) {
                entry->mKind = k;
                entry->mId = atoi(id + 2);
                break;
            }
        }
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        entry->mModel = csv.fn_8005B9CC();
        entry->m00 = fn_800DC288(csv.fn_8005B9CC());
        entry->m04 = fn_800DC288(csv.fn_8005B9CC());
        entry->m08 = fn_800DC288(csv.fn_8005B9CC());
        entry->m0C = fn_800DC288(csv.fn_8005B9CC());
        entry->m10 = fn_800DC288(csv.fn_8005B9CC());
        entry->m14 = fn_800DC288(csv.fn_8005B9CC());
        entry->m28 = fn_800DC288(csv.fn_8005B9CC());
        entry->m2C = fn_800DC288(csv.fn_8005B9CC());
        entry->m18 = fn_800DC288(csv.fn_8005B9CC());
        entry->m1C = fn_800DC288(csv.fn_8005B9CC());
        entry->m30 = atoi(csv.fn_8005B9CC());
        entry->m34 = atoi(csv.fn_8005B9CC());
        std::string str(csv.fn_8005B9CC());
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
        for (int j = 0; j < 4; j++) {
            entry->mAC[j] = atoi(csv.fn_8005B9CC());
        }
        entry->mBC = atoi(csv.fn_8005B9CC());
        entry->mC0 = csv.fn_8005B9CC();
        entry->mD0 = csv.fn_8005B9CC();
        entry->mE0 = atoi(csv.fn_8005B9CC());
        for (int j = 0; j < 2; j++) {
            entry->mE4[j].m00 = atoi(csv.fn_8005B9CC());
            entry->mE4[j].m04 = atoi(csv.fn_8005B9CC());
            entry->mE4[j].m08 = atoi(csv.fn_8005B9CC());
            entry->mE4[j].m0C = atoi(csv.fn_8005B9CC());
            entry->mE4[j].m10 = atoi(csv.fn_8005B9CC());
        }
        csv.fn_8005B9CC();
        for (int j = 0; j < 4; j++) {
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
        }
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        entry->mA8 = -1;
        if (entry->mModel.mStr[0] == 'p' && entry->mModel.mStr[1] == 'c') {
            switch (entry->mModel.mStr[9]) {
            case 'a':
                entry->mA8 = 0;
                break;
            case 'b':
                entry->mA8 = 1;
                break;
            case 'd':
                entry->mA8 = 2;
                break;
            case 'f':
                entry->mA8 = 3;
                break;
            case 'g':
                entry->mA8 = 4;
                break;
            case 'j':
                entry->mA8 = 5;
                break;
            case 'l':
                entry->mA8 = 6;
                break;
            case 'm':
                entry->mA8 = 7;
                break;
            case 'w':
                entry->mA8 = 8;
                break;
            case 'x':
                entry->mA8 = 9;
                break;
            case 'y':
                entry->mA8 = 10;
                break;
            default:
                entry->mA8 = -1;
                break;
            }
        }
        i++;
    }
    for (; i < sCount; i++) {
        sEntries[i].mId = -1;
    }
    fn_8046DD20(lbl_8087F518, data);
    fn_8020EB0C();
}

// inferred: loads the names and texts (UTF-16) of the equipment
extern "C" void fn_8020EB0C() {
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
    sTexts = new ((MemoryTag)3, "", "", 0) wchar_t[length];
    int pos = 0;
    csv.Reset_unk();
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == 0 || *field == L'#') {
            continue;
        }
        EquipParamEntry_unk* entry = &sEntries[i];
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

// inferred: entry of a kind and id; ids above 1000 are reduced for some kinds
extern "C" EquipParamEntry_unk* fn_8020ED84(int kind, int id) {
    if (id >= 1000) {
        if (kind >= 2) {
            id = id % 1000;
        } else if (kind == 1) {
            int rest = id % 1000;
            if (450 <= rest && 480 >= rest) {
                id = id % 1000;
            }
        }
    }
    for (unsigned int i = 0; i < (unsigned int)sCount; i++) {
        if (kind == sEntries[i].mKind && id == sEntries[i].mId) {
            return &sEntries[i];
        }
    }
    return 0;
}

extern "C" int fn_8020EE58() {
    return sCount;
}

extern "C" EquipParamEntry_unk* fn_8020EE60(int index) {
    if (index < 0 || index >= sCount) {
        return 0;
    }
    return &sEntries[index];
}

// inferred: slot of a two-letter name, -1 if unknown
extern "C" int fn_8020EE8C(const char* name) {
    for (int i = 0; i < 5; i++) {
        if (strncmp(sSlots[i], name, 2) == 0) {
            return i;
        }
    }
    return -1;
}

extern "C" bool fn_8020EF04(int code) {
    return code / 100000 % 10 == 2;
}

extern "C" bool fn_8020EF4C(EquipParamEntry_unk* entry) {
    if (entry == 0) {
        return false;
    }
    return (unsigned int)(entry->mA8 - 8) <= 2;
}

// inferred: item code of an entry
extern "C" int fn_8020EF80(EquipParamEntry_unk* entry) {
    if (entry == 0) {
        return 0;
    }
    int code = 200000;
    code += entry->mKind * 1000;
    code += entry->mId % 1000;
    code += entry->mId / 1000 * 1000000;
    return code;
}

// inferred: entry of an item code
extern "C" EquipParamEntry_unk* fn_8020EFEC(int code) {
    code -= 200000;
    return fn_8020ED84(code / 1000 % 10, code % 1000 + code / 1000000 * 1000);
}

extern "C" int fn_8020F064(const char* letter) {
    for (int i = 0; i < 11; i++) {
        if (sLetters[i] == *letter) {
            return i;
        }
    }
    return -1;
}

// inferred: whether the entry can be equipped in a slot
extern "C" bool fn_8020F0AC(EquipParamEntry_unk* entry, int slot) {
    if (entry == 0) {
        return false;
    }
    bool result = false;
    switch (slot) {
    case 0:
    case 2:
    case 6:
    case 4:
        if (entry->mA8 != 9 && entry->mA8 != 10) {
            result = true;
        }
        break;
    case 3:
    case 1:
    case 5:
        if (entry->mA8 != 8) {
            result = true;
        }
        break;
    }
    return result;
}
