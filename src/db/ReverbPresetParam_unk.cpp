#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

struct ReverbPreset_unk;

// inferred: sound engine (lbl_8087EE90): clears its reverb presets / adds one
extern "C" void* lbl_8087EE90;
extern "C" void fn_8004A03C(void* sound);
extern "C" void fn_80049E54(void* sound, ReverbPreset_unk* preset);

// inferred: one row of /boot/reverb_preset_param.csv (0x24 bytes, no constructor)
struct ReverbPreset_unk {
    int mId;
    int mType; // inferred: index in sTypes (-1 if not found)
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
};

// inferred: chara ids of the players (fn_80219544) and a second id list searched by fn_80219558
// (db/PlayerParam_unk uses fn_80219558 as "player index of a chara id")
static const int sMapIds[18] = {
    1, 0x18705, 0x18769, 0x187CD, 0x18831, 0x18895, 0x188F9, 0x1895D, 0x189C1,
    0x18706, 0x18707, 0x18AED, 0x18709, 0x1876B, 0x18AEE, 2, 3, 4,
};
static const int sOtherIds[18] = {
    0xAAE60, 0xAAE66, 0xAAE61, 0xAAE62, 0xAAE63, 0xAAE65, 0xAAE64, 0xAAE68, 0xAAE67,
    0, 0, 0, 0, 0, 0xAAE68, 0, 0, 0,
};
static const char sFileNameStr[] = "/boot/reverb_preset_param.csv";
static const char* const sTypes[] = {"normal", "metal", "smallroom", "largeroom", "hall", "cave", 0};
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static ReverbPreset_unk* sEntries;
static int sCount;

extern "C" int fn_80219544(int index) {
    return sMapIds[index];
}

// inferred: player index of a chara id (-1 if none)
extern "C" int fn_80219558(int id) {
    for (int i = 0; i < 18; i++) {
        if (id == sMapIds[i]) {
            if (i >= 15 && i <= 17) {
                return 0;
            }
            if (i == 13) {
                return 2;
            }
            return i;
        }
    }
    if (id == 0x1870E) {
        return 1;
    }
    if (id == 0x30F7A) {
        return 9;
    }
    for (int i = 0; i < 18; i++) {
        if (id == sOtherIds[i]) {
            return i;
        }
    }
    return -1;
}

static inline int GetType_unk(const char* name) {
    for (int i = 0; sTypes[i] != 0; i++) {
        const char* type = sTypes[i];
        if (strncmp(name, type, strlen(type)) == 0) {
            return i;
        }
    }
    return -1;
}

// inferred: loads /boot/reverb_preset_param.csv and gives the presets to the sound engine
extern "C" void fn_802196D0() {
    atn::Csv csv;
    if (sEntries == 0 && lbl_8087EE90 != 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.vf08((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strlen(field) == 0) {
                continue;
            }
            sCount++;
        }
        sEntries = new (12, "", "", 0) ReverbPreset_unk[sCount];
        fn_8004A03C(lbl_8087EE90);
        csv.vf08((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strlen(field) == 0) {
                continue;
            }
            ReverbPreset_unk* entry = &sEntries[i];
            entry->mId = atoi(csv.mField);
            entry->mType = GetType_unk(csv.fn_8005B9CC());
            entry->m08 = fn_800DC288(csv.fn_8005B9CC());
            entry->m0C = fn_800DC288(csv.fn_8005B9CC());
            entry->m10 = fn_800DC288(csv.fn_8005B9CC());
            entry->m14 = fn_800DC288(csv.fn_8005B9CC());
            entry->m18 = fn_800DC288(csv.fn_8005B9CC());
            entry->m1C = fn_800DC288(csv.fn_8005B9CC());
            entry->m20 = fn_800DC288(csv.fn_8005B9CC());
            fn_80049E54(lbl_8087EE90, entry);
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}
