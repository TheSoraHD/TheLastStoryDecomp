#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/hate_param.csv (0x6C bytes; the empty constructor is out of line,
// fn_8021021C, called through __construct_new_array)
struct HateParamEntry_unk {
    HateParamEntry_unk();

    int m00;
    int m04;
    int m08;
    int m0C;
    float m10;
    float m14;
    int m18;
    int m1C;
    int m20;
    float m24;
    int m28;
    int m2C;
    float m30;
    float m34;
    float m38;
    float m3C;
    int m40;
    int m44;
    int m48;
    int m4C;
    int m50;
    float m54;
    float m58;
    int m5C;
    float m60;
    float m64;
    float m68;
};

static const char sFileNameStr[] = "/boot/hate_param.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static HateParamEntry_unk* sEntries;
static int sCount;

// inferred: loads /boot/hate_param.csv
extern "C" void fn_8020FDEC() {
    atn::Csv csv;
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.vf08((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            if (strlen(csv.fn_8005B9CC()) != 0) {
                sCount++;
            }
        }
        sEntries = new (3, "", "", 0) HateParamEntry_unk[sCount];
        csv.vf08((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            if (strlen(csv.fn_8005B9CC()) == 0) {
                continue;
            }
            memset(&sEntries[i], 0, sizeof(HateParamEntry_unk));
            csv.fn_8005B9CC(); // inferred: a column that is not used
            sEntries[i].m00 = atoi(csv.fn_8005B9CC());
            sEntries[i].m04 = atoi(csv.fn_8005B9CC());
            sEntries[i].m08 = atoi(csv.fn_8005B9CC());
            sEntries[i].m0C = atoi(csv.fn_8005B9CC());
            sEntries[i].m10 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m14 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m18 = atoi(csv.fn_8005B9CC());
            sEntries[i].m1C = atoi(csv.fn_8005B9CC());
            sEntries[i].m20 = atoi(csv.fn_8005B9CC());
            sEntries[i].m24 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m28 = atoi(csv.fn_8005B9CC());
            sEntries[i].m2C = atoi(csv.fn_8005B9CC());
            sEntries[i].m30 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m34 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m38 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m3C = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m40 = atoi(csv.fn_8005B9CC());
            sEntries[i].m44 = atoi(csv.fn_8005B9CC());
            sEntries[i].m48 = atoi(csv.fn_8005B9CC());
            sEntries[i].m4C = atoi(csv.fn_8005B9CC());
            sEntries[i].m50 = atoi(csv.fn_8005B9CC());
            sEntries[i].m54 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m58 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m5C = atoi(csv.fn_8005B9CC());
            sEntries[i].m60 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m64 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m68 = fn_800DC288(csv.fn_8005B9CC());
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

HateParamEntry_unk::HateParamEntry_unk() {}

extern "C" HateParamEntry_unk* fn_80210220(int index) {
    if (index < sCount) {
        return &sEntries[index];
    }
    return sEntries;
}
