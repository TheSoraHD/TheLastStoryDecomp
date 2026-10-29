#include "atn/Csv.h"
#include "atn/FileList.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" wchar_t* wcsncpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" int fn_800DC12C(const char* s);    // inferred: string to int
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int
extern "C" int fn_805A52F4(const char* s);    // inferred: value of a name (hash or id)
extern "C" int fn_80219544(void* member);     // inferred: character id of a party member
extern "C" int fn_8006BA30();                 // inferred: language check (the short name is the long one)

// inferred: the font (lbl_8087EEC8) converts UTF-16 text to the game encoding
extern "C" void* lbl_8087EEC8;
extern "C" int fn_8006F420(void* font, char* dst, const wchar_t* src, int size);

// inferred: loads a whole file / releases it
extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int arg);

typedef std::basic_string<wchar_t> WString_unk;

// inferred: one row of /boot/chara_id_table_xx.u16 (0x30 bytes); the texts point into the two
// buffers sWideTexts and sTexts
struct CharaIdEntry_unk {
    int mId;
    const wchar_t* mName;      // inferred
    const wchar_t* mShortName; // inferred: the name again when fn_8006BA30 is set
    const char* m0C;
    const char* m10;
    const char* m14;
    const char* m18;
    const char* m1C;
    const char* m20;
    int m24;
    const char* m28;
    int m2C;
};

static const char sFileNameStr[] = "/boot/chara_id_table_xx.u16";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static CharaIdEntry_unk* sEntries;
static unsigned int sCount;
static wchar_t* sWideTexts;
static char* sTexts;

// inferred: kind of a character id from its prefix
extern "C" int fn_80208748(const char* id) {
    if (strncmp(id, "np", 2) == 0 || strncmp(id, "an", 2) == 0) {
        return 1;
    }
    if (strncmp(id, "em", 2) == 0) {
        return 2;
    }
    if (strncmp(id, "fr", 2) == 0) {
        return 3;
    }
    return strncmp(id, "pc", 2) ? 5 : 0;
}

// inferred: conversion of a wide string without a font: always empty
extern "C" std::string fn_80208814(const WString_unk& str) {
    return std::string("");
}

// inferred: converts UTF-16 text to the game encoding, returns the length
extern "C" int fn_802088A4(char* dst, const wchar_t* src, int size) {
    if (lbl_8087EEC8 != 0) {
        return fn_8006F420(lbl_8087EEC8, dst, src, size);
    }
    std::string str = fn_80208814(WString_unk(src));
    strcpy(dst, str.c_str());
    return strlen(dst);
}

// inferred: rounds a length up to an even number
static inline int Align2_unk(int len) {
    if (len % 2 == 1) {
        len++;
    }
    return len;
}

// inferred: copies a wide text to buf + pos and returns the next position (even)
extern "C" int fn_802089C4(const wchar_t** out, const wchar_t* src, wchar_t* buf, int pos) {
    *out = buf + pos;
    wcsncpy(buf + pos, src, 0x100);
    return pos + Align2_unk(wcslen(*out) + 1);
}

// inferred: converts a wide text to buf + pos and returns the next position (even)
extern "C" int fn_80208A3C(const char** out, const wchar_t* src, char* buf, int pos) {
    *out = buf + pos;
    fn_802088A4((char*)*out, src, 0x100);
    return pos + Align2_unk(strlen(*out) + 1);
}

// inferred: copies a text to buf + pos and returns the next position (even)
extern "C" int fn_80208B70(const char** out, const char* src, char* buf, int pos) {
    *out = buf + pos;
    strcpy(buf + pos, src);
    return pos + Align2_unk(strlen(*out) + 1);
}

extern "C" void fn_80208BE8(const char* path);
extern "C" int fn_80209184(const wchar_t* id);

extern "C" void fn_80208BE0() {
    fn_80208BE8(sFileName);
}

// inferred: loads the character id table (UTF-16 CSV)
// NONMATCHING: the original calls fn_802088A4, fn_802089C4, fn_80208A3C, fn_80209184 and the
// atn::CsvU constructor (weak copy fn_80209048) out of line: the inlining budget of this function is
// exceeded there but not here (three more fn_80208A3C calls are enough to exceed it; probably the
// std::string reconstruction has fewer statements than the original). Same statements otherwise.
extern "C" void fn_80208BE8(const char* path) {
    unsigned long size = 0;
    void* data = fn_8006BA8C(path, &size, 0);
    atn::CsvU csv((const wchar_t*)data, size / 2);
    char id[0x20];
    char text[0x100];
    unsigned int count = 0;
    int wideSize = 0;
    int textSize = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (wcscmp(csv.fn_8005B9CC(), L"") != 0) {
            continue;
        }
        fn_802088A4(id, csv.fn_8005B9CC(), 0x20);
        if (strcmp(id, "") == 0) {
            continue;
        }
        count++;
        wideSize += wcslen(csv.fn_8005B9CC()) + 2;
        wideSize += wcslen(csv.fn_8005B9CC()) + 2;
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        textSize += strlen(text) + 2;
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        textSize += strlen(text) + 2;
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        textSize += strlen(text) + 2;
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        textSize += strlen(text) + 2;
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        textSize += strlen(text) + 2;
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        textSize += strlen(text) + 2;
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        fn_802088A4(text, csv.fn_8005B9CC(), 0x100);
        textSize += strlen(text) + 2;
    }
    sEntries = new ((MemoryTag)3, "", "", 0) CharaIdEntry_unk[count];
    sCount = count;
    csv.SetData((const wchar_t*)data, size / 2);
    sWideTexts = new ((MemoryTag)3, "", "", 0) wchar_t[wideSize];
    sTexts = new ((MemoryTag)3, "", "", 0) char[textSize];
    int widePos = 0;
    int textPos = 0;
    int i = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (wcscmp(csv.fn_8005B9CC(), L"") != 0) {
            continue;
        }
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == 0) {
            continue;
        }
        CharaIdEntry_unk* entry = &sEntries[i];
        entry->mId = fn_80209184(field);
        widePos = fn_802089C4(&entry->mName, csv.fn_8005B9CC(), sWideTexts, widePos);
        widePos = fn_802089C4(&entry->mShortName, csv.fn_8005B9CC(), sWideTexts, widePos);
        if (fn_8006BA30() != 0) {
            entry->mShortName = entry->mName;
        }
        textPos = fn_80208A3C(&entry->m0C, csv.fn_8005B9CC(), sTexts, textPos);
        textPos = fn_80208A3C(&entry->m10, csv.fn_8005B9CC(), sTexts, textPos);
        textPos = fn_80208A3C(&entry->m14, csv.fn_8005B9CC(), sTexts, textPos);
        textPos = fn_80208A3C(&entry->m18, csv.fn_8005B9CC(), sTexts, textPos);
        textPos = fn_80208A3C(&entry->m1C, csv.fn_8005B9CC(), sTexts, textPos);
        textPos = fn_80208A3C(&entry->m20, csv.fn_8005B9CC(), sTexts, textPos);
        fn_802088A4(text, csv.fn_8005B9CC(), 0x40);
        entry->m24 = fn_805A52F4(text);
        textPos = fn_80208A3C(&entry->m28, csv.fn_8005B9CC(), sTexts, textPos);
        entry->m2C = fn_800DC1DC(csv.fn_8005B9CC());
        i++;
    }
    atn::FileFreeBuffer((char*)data, false);
}

// inferred: number of a character id ("em_123_45" -> 12345)
extern "C" int fn_80209120(const char* id) {
    int value = 0;
    if (id != 0 && strlen(id) > 7) {
        value = fn_800DC12C(id + 2) * 100;
        value += fn_800DC12C(id + 7);
    }
    return value;
}

// inferred: as fn_80209120 for a wide string
extern "C" int fn_80209184(const wchar_t* id) {
    int value = 0;
    if (id != 0 && wcslen(id) > 7) {
        value = fn_800DC1DC(id + 2) * 100;
        value += fn_800DC1DC(id + 7);
    }
    return value;
}

// inferred: entry of a party member
extern "C" CharaIdEntry_unk* fn_8020924C(int id);

extern "C" CharaIdEntry_unk* fn_802091E8(void* member) {
    return fn_8020924C(fn_80219544(member));
}

extern "C" CharaIdEntry_unk* fn_8020924C(int id) {
    for (unsigned int i = 0; i < sCount; i++) {
        if (id == sEntries[i].mId) {
            return &sEntries[i];
        }
    }
    return 0;
}

extern "C" CharaIdEntry_unk* fn_80209294(int index) {
    if (index < 0 || index >= (int)sCount) {
        return 0;
    }
    return &sEntries[index];
}

// inferred: path of the character file of a name
extern "C" bool fn_802092C0(char* dst, const char* name) {
    sprintf(dst, "/data/character/%s.chr", name);
    return true;
}

extern "C" unsigned int fn_802092F8() {
    return sCount;
}
