#include "atn/Csv.h"
#include "atn/Memory.h"
#include "gmk/SeTable.h"
#include "std/string.h"

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int

// inferred: the font (lbl_8087EEC8) converts UTF-16 text to the game encoding
extern "C" void* lbl_8087EEC8;
extern "C" void fn_8006F5E8(void* font, std::string* dst, const wchar_t* src);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: game settings (lbl_8087F0A8); only the member used here
struct TutorialSettings_unk {
    char m000[0x194];
    int mDemo; // inferred: 0x194, demo version (selects the tutorial table)
};
extern "C" TutorialSettings_unk* lbl_8087F0A8;

// inferred: wide fixed-size string (as atn::FixedString_unk, with wcslen + wmemcpy). Name invented.
template <int N>
struct WFixedString_unk {
    WFixedString_unk() { mStr[0] = 0; }
    WFixedString_unk& operator=(const wchar_t* str) {
        if (str != mStr) {
            wmemcpy(mStr, str, wcslen(str) + 1);
        }
        return *this;
    }
    wchar_t mStr[N];
};

// inferred: one row of /boot/tutorial_table_xx.u16 (0xEC bytes); constructor and empty destructor
// are emitted out of line for __construct_new_array (fn_8021F040, fn_8021F054)
struct TutorialEntry_unk {
    TutorialEntry_unk() {}
    ~TutorialEntry_unk() {}

    int mId;
    int m04;
    atn::FixedString_unk<0x20> m08;
    WFixedString_unk<0x40> m28;
    int mA8;
    atn::FixedString_unk<0x40> mAC;
};

static const char sFileNameStr[] = "/boot/tutorial_table_xx.u16";
static const char sDemoFileNameStr[] = "/boot/tutorial_table_demo_xx.u16";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sFileName = sFileNameStr;
static const char* const sDemoFileName = sDemoFileNameStr;

static TutorialEntry_unk* sEntries;
static int sCount;

// inferred: loads the tutorial table (UTF-16 CSV, the demo has its own table)
extern "C" void fn_8021ED08() {
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, lbl_8087F0A8->mDemo != 0 ? sDemoFileName : sFileName, &size, 0x20);
        atn::CsvU csv((const wchar_t*)data, (int)size / 2);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != L';') {
                sCount++;
            }
        }
        sEntries = new ((MemoryTag)12, "", "", 0) TutorialEntry_unk[sCount];
        std::string text;
        csv.Reset_unk();
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            const wchar_t* field = csv.fn_8005B9CC();
            if (*field == L';') {
                continue;
            }
            TutorialEntry_unk* entry = &sEntries[i];
            entry->mId = fn_800DC1DC(field);
            entry->m04 = fn_800DC1DC(csv.fn_8005B9CC());
            void* font = lbl_8087EEC8;
            fn_8006F5E8(font, &text, csv.fn_8005B9CC());
            entry->m08 = text.c_str();
            entry->m28 = csv.fn_8005B9CC();
            entry->mA8 = fn_800DC1DC(csv.fn_8005B9CC());
            font = lbl_8087EEC8;
            fn_8006F5E8(font, &text, csv.fn_8005B9CC());
            entry->mAC = text.c_str();
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

extern "C" int fn_8021F094() {
    return sCount;
}

// inferred: entry by id (0 if there is none)
extern "C" TutorialEntry_unk* fn_8021F09C(int id) {
    TutorialEntry_unk* entry = sEntries;
    for (int i = 0; i < sCount; i++, entry++) {
        if (entry->mId == id) {
            return entry;
        }
    }
    return 0;
}

extern "C" TutorialEntry_unk* fn_8021F0D4(int index) {
    if (index >= 0 && index < sCount) {
        return &sEntries[index];
    }
    return 0;
}
