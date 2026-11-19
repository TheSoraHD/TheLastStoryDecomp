#include "atn/Csv.h"
#include "atn/FileList.h"
#include "atn/Memory.h"
#include "atn/TextTable_unk.h"

extern "C" void* fn_8006BA8C(const char* path, unsigned int* size, int flag); // inferred: loads a file
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...);         // inferred: wide sprintf
extern "C" unsigned long wcslen(const wchar_t* s);

// inferred: defined here (just before this unit in the .sbss of the .dol)
extern "C" {
wchar_t* lbl_8087F1E0;       // inferred: buffer holding all the texts
TextTable_unk* lbl_8087F1E4; // inferred: singleton
}

void TextTable_unk::fn_8020C028() {
    if (lbl_8087F1E4 == 0) {
        lbl_8087F1E4 = new ((MemoryTag)1, "", "", 0) TextTable_unk;
    }
}

// inferred: never called, so the linker removed it; it explains why the weak destructor of
// ptr_string_base comes before the constructor (0x8020C07C)
TextTable_unk::~TextTable_unk() {}

// inferred: the locals are declared in the order the original allocates its saved registers
// (pos, then the text of the second pass, then the index, then the total)
TextTable_unk::TextTable_unk() {
    int pos = 0;
    unsigned int size0 = 0;
    void* data0 = fn_8006BA8C("/boot/common_text_xx.u16", &size0, 0);
    unsigned int size1 = 0;
    void* data1 = fn_8006BA8C("/boot/shop_name_table_xx.u16", &size1, 0);
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

    lbl_8087F1E0 = new ((MemoryTag)1, "", "", 0) wchar_t[total];
    csv0.SetData((const wchar_t*)data0, size0 / 2);
    csv1.SetData((const wchar_t*)data1, size1 / 2);

    const wchar_t* text;
    int i = 0;
    pos = 0;
    do {
        const wchar_t* field = csv0.fn_8005B9CC();
        if (field[0] != L'#' && field[0] != 0) {
            text = csv0.fn_8005B9CC();
            fn_800DD3FC(lbl_8087F1E0 + pos, L"%s", text);
            mStrings[i].mPtr = lbl_8087F1E0 + pos;
            pos += wcslen(text) + 1;
            i++;
        }
    } while (csv0.fn_8005BBF8());
    i++; // inferred: one empty slot between the two tables
    do {
        const wchar_t* field = csv1.fn_8005B9CC();
        if (field[0] != L'#' && field[0] != 0) {
            text = csv1.fn_8005B9CC();
            fn_800DD3FC(lbl_8087F1E0 + pos, L"%s", text);
            mStrings[i].mPtr = lbl_8087F1E0 + pos;
            pos += wcslen(text) + 1;
            i++;
        }
    } while (csv1.fn_8005BBF8());

    atn::FileFreeBuffer((char*)data0, false);
    atn::FileFreeBuffer((char*)data1, false);
}
