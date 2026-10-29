#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int atoi(const char* s);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: hash of a name
// inferred: copy of a path with the language suffix ("_xx") replaced
extern "C" std::string fn_8006B53C(const std::string& path);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/voice_length_table_xx.csv (8 bytes, no constructor)
struct VoiceLengthEntry_unk {
    unsigned int mHash; // inferred: hash of the voice name
    int mLength;
};

static const char sFileNameStr[] = "/boot/voice_length_table_xx.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static VoiceLengthEntry_unk* sEntries;
static int sCount;

// inferred: loads /boot/voice_length_table_xx.csv (localised file name)
extern "C" void fn_8021F100() {
    atn::Csv csv;
    if (sEntries == 0) {
        std::string path(sFileName);
        path = fn_8006B53C(path);
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, path.c_str(), &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            char c = *csv.fn_8005B9CC();
            if (c == ';' || c == '#') {
                continue;
            }
            sCount++;
        }
        sEntries = new (12, "", "", 0) VoiceLengthEntry_unk[sCount];
        csv.SetData((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            char c = *csv.fn_8005B9CC();
            if (c == ';' || c == '#') {
                continue;
            }
            VoiceLengthEntry_unk* entry = &sEntries[i];
            entry->mHash = fn_800DC6B4(csv.mField);
            // inferred: columns that are not used
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
            entry->mLength = atoi(csv.fn_8005B9CC());
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

static inline VoiceLengthEntry_unk* Find_unk(const char* name) {
    if (name == 0 || *name == '\0') {
        return 0;
    }
    unsigned int hash = fn_800DC6B4(name);
    for (int i = 0; i < sCount; i++) {
        if (hash == sEntries[i].mHash) {
            return &sEntries[i];
        }
    }
    return 0;
}

// inferred: length of a voice (-1 if it is not in the table)
extern "C" int fn_8021F3E8(const char* name) {
    VoiceLengthEntry_unk* entry = Find_unk(name);
    if (entry != 0) {
        return entry->mLength;
    }
    return -1;
}
