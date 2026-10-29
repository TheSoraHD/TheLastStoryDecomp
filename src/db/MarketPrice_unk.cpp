#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one column of /boot/market_price.csv (58 prices, 0xE8 bytes); the inline constructor
// is emitted out of line (fn_802178C4) for __construct_new_array
struct MarketPrice_unk {
    MarketPrice_unk() {
        for (int i = 0; i < 58; i++) {
            mPrice[i] = 100.0f;
        }
    }

    float mPrice[58];
};

static const char sFileNameStr[] = "/boot/market_price.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static MarketPrice_unk* sEntries;

// inferred: loads /boot/market_price.csv (58 rows of 10 columns, no header)
extern "C" void fn_80217778() {
    atn::Csv csv;
    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    csv.SetData((const char*)data, size);
    sEntries = new ((MemoryTag)1, "", "", 0) MarketPrice_unk[10];
    int row = 0;
    do {
        if (row >= 58) {
            break;
        }
        for (int col = 0; col < 10; col++) {
            MarketPrice_unk* entry = &sEntries[col];
            entry->mPrice[row] = fn_800DC288(csv.fn_8005B9CC());
        }
        row++;
    } while (csv.fn_8005BBF8() != 0);
    fn_8046DD20(lbl_8087F518, data);
}

// inferred: number of columns
extern "C" int fn_802179B4() {
    return 10;
}

// inferred: price of a column/row (1.0f out of range)
extern "C" float fn_802179BC(int col, int row) {
    if (0 <= col && col < 10 && 0 <= row && row < 58) {
        return sEntries[col].mPrice[row];
    }
    return 1.0f;
}
