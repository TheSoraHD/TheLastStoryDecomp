#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" void* fn_8006BA8C(const char* path, unsigned int* size, int flag); // inferred: loads a file
extern "C" void fn_8006BB6C(void* data, int flag);                           // inferred: frees it
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...);         // inferred: wide sprintf
extern "C" unsigned long wcslen(const wchar_t* s);

// The strings used below are outside the split of this unit: the pool 0x8073F5C4 ("",
// "/boot/common_text_xx.u16", "/boot/shop_name_table_xx.u16") and L"%s" (0x80783008).
extern "C" const char lbl_8073F5C4[];
extern "C" const wchar_t lbl_80783008[];

namespace atn {

// inferred: pointer to a string owned elsewhere (vptr + pointer; constructor 0x8020C4B0)
template <class T, int N>
class ptr_string_base {
public:
    ptr_string_base() : mPtr(0) {}
    virtual ~ptr_string_base() {}

    T* mPtr;
};

} // namespace atn

// inferred: table of the game texts read from two UTF-16 CSV files (0x1018 bytes); names invented
class TextTable_unk {
public:
    TextTable_unk();
    static void fn_8020C028(); // inferred: creates the singleton

    atn::ptr_string_base<wchar_t, 0> mStrings[0x203];
};

extern "C" TextTable_unk* lbl_8087F1E4; // inferred: singleton
extern "C" wchar_t* lbl_8087F1E0;       // inferred: buffer holding all the texts

// NONMATCHING: 99.8% - only the name of the constructor differs (fn_8020C0BC in symbols.txt;
// pairnames.py cannot pair it because the constructor does not match yet)
void TextTable_unk::fn_8020C028() {
    if (lbl_8087F1E4 == 0) {
        lbl_8087F1E4 = new (1, lbl_8073F5C4, lbl_8073F5C4, 0) TextTable_unk;
    }
}

// NONMATCHING: the original keeps the address of each atn::CsvU in a saved register while it is
// constructed (as in CampSaveLoad::fn_8011B8D4), so the registers shift; the callees
// fn_8005B710, fn_8005B8F8, fn_8005B9AC and fn_8020C4B0 have unnamed symbols
TextTable_unk::TextTable_unk() {
    unsigned int size0 = 0;
    void* data0 = fn_8006BA8C(lbl_8073F5C4 + 1, &size0, 0);
    unsigned int size1 = 0;
    void* data1 = fn_8006BA8C(lbl_8073F5C4 + 0x1A, &size1, 0);
    atn::CsvU csv0((const wchar_t*)data0, size0 / 2);
    atn::CsvU csv1((const wchar_t*)data1, size1 / 2);

    int total = 0;
    do {
        const wchar_t* field = csv0.fn_8005B9CC();
        if (field[0] != L'#' && field[0] != 0) {
            total += wcslen(csv0.fn_8005B9CC()) + 1;
        }
    } while (csv0.fn_8005BBF8());
    do {
        const wchar_t* field = csv1.fn_8005B9CC();
        if (field[0] != L'#' && field[0] != 0) {
            total += wcslen(csv1.fn_8005B9CC()) + 1;
        }
    } while (csv1.fn_8005BBF8());

    lbl_8087F1E0 = new (1, lbl_8073F5C4, lbl_8073F5C4, 0) wchar_t[total];
    csv0.vf08((const wchar_t*)data0, size0 / 2);
    csv1.vf08((const wchar_t*)data1, size1 / 2);

    int count = 0;
    int pos = 0;
    int i = 0;
    do {
        const wchar_t* field = csv0.fn_8005B9CC();
        if (field[0] != L'#' && field[0] != 0) {
            const wchar_t* text = csv0.fn_8005B9CC();
            fn_800DD3FC(lbl_8087F1E0 + pos, lbl_80783008, text);
            mStrings[i].mPtr = lbl_8087F1E0 + pos;
            pos += wcslen(text) + 1;
            i++;
            count++;
        }
    } while (csv0.fn_8005BBF8());
    count++;
    i = count;
    do {
        const wchar_t* field = csv1.fn_8005B9CC();
        if (field[0] != L'#' && field[0] != 0) {
            const wchar_t* text = csv1.fn_8005B9CC();
            fn_800DD3FC(lbl_8087F1E0 + pos, lbl_80783008, text);
            mStrings[i].mPtr = lbl_8087F1E0 + pos;
            pos += wcslen(text) + 1;
            i++;
        }
    } while (csv1.fn_8005BBF8());

    fn_8006BB6C(data0, 0);
    fn_8006BB6C(data1, 0);
}
