#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "std/string.h"

extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int
// inferred: wide string literals of the original file (L"SAVE DATA", L"LOAD DATA", ...), outside
// this split; L"" is at 0x90
extern "C" const wchar_t lbl_8077A0B0[];

// inferred: this split only holds the end of the camp save/load file: the reader of the save
// date list and the weak copies of atn::CsvU.

// inferred: one row of the list read by fn_8011B8D4 (0x18 bytes; constructor 0x8011BBBC,
// destructor 0x80117E88); names invented
struct CampSaveLoadEntry_unk {
    int m00;
    int m04;
    int m08;
    std::basic_string<wchar_t> m0C;
};

// inferred: only the members used here (vtable 0x8077A030)
class CampSaveLoad {
public:
    void fn_8011B8D4(const wchar_t* text, int length);

    unsigned char m00[0x4C0];
    atn::Buffer_unk<CampSaveLoadEntry_unk> m4C0;
};

// NONMATCHING: the original keeps the address of csv in a saved register while it is
// constructed (one more saved register and a 0x10 bytes larger frame), and the callees have
// unnamed symbols (fn_8005B710, fn_8005B8F8, fn_8005B9AC, fn_8006F72C, fn_8011BBBC,
// fn_80117E88) that do not match the mangled names used here
void CampSaveLoad::fn_8011B8D4(const wchar_t* text, int length) {
    atn::CsvU csv(text, length);
    int count = 0;
    while (csv.fn_8005BBF8()) {
        if (wcscmp(csv.fn_8005B9CC(), lbl_8077A0B0 + 0x48) == 0) {
            int a = fn_800DC1DC(csv.fn_8005B9CC());
            int b = fn_800DC1DC(csv.fn_8005B9CC());
            if (a > 0 && b > 0) {
                count++;
            }
        }
    }
    m4C0.Resize_unk(count, 0);
    int i = 0;
    csv.vf08(text, length);
    while (csv.fn_8005BBF8()) {
        if (wcscmp(csv.fn_8005B9CC(), lbl_8077A0B0 + 0x48) == 0) {
            int a = fn_800DC1DC(csv.fn_8005B9CC());
            int b = fn_800DC1DC(csv.fn_8005B9CC());
            int c = fn_800DC1DC(csv.fn_8005B9CC());
            if (a > 0 && b > 0) {
                CampSaveLoadEntry_unk& entry = m4C0[i];
                entry.m00 = a;
                entry.m04 = b;
                entry.m08 = c;
                entry.m0C = csv.fn_8005B9CC();
                i++;
            }
        }
    }
}
