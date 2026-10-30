#include "game/GameSaveLoadBackgroundCommon.h"
#include "atn/Memory.h"
#include "atn/DrawHelper.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int sprintf(char* dst, const char* fmt, ...);

// inferred: NAND file info (0xA0 bytes reserved on the stack)
struct NandFileInfo_unk {
    unsigned char m00[0xA0];
};
struct NandStatus_unk {
    unsigned char m00[0x8];
};
extern "C" int NANDGetHomeDir(char* out);
extern "C" int NANDGetStatus(const char* path, NandStatus_unk* stat);
extern "C" int NANDSeek(NandFileInfo_unk* info, int offset, int whence);
extern "C" int NANDRead(NandFileInfo_unk* info, void* buf, unsigned int len);
// inferred: NANDSafeOpen / NANDSafeClose
extern "C" int fn_8061FC70(const char* path, NandFileInfo_unk* info, unsigned char mode, void* buf, unsigned int len);
extern "C" int fn_80620030(NandFileInfo_unk* info);

// inferred: game settings (lbl_8087F0A8), only the field read here
struct SaveSettings_unk {
    unsigned char m000[0x198];
    int m198; // inferred: saving disabled
};
extern "C" SaveSettings_unk* lbl_8087F0A8;
extern "C" int lbl_8087DCC8; // inferred: some slot after the first one is used
extern "C" int lbl_8087DCCC; // inferred: the first slot is used
extern "C" int lbl_8087F434;
extern "C" void fn_800A4BE4(SaveLoadMgr_unk* mgr, const char* name, void* work); // inferred: open
extern "C" void fn_800A4D00(SaveLoadMgr_unk* mgr, void* buf, int size, int offset, void* work); // inferred: read
extern "C" void fn_800A4E08(SaveLoadMgr_unk* mgr, void* work); // inferred: close
extern "C" void* lbl_8087F518;
extern "C" void fn_8046ECDC(void* obj, int arg);
extern "C" void fn_8049D52C(char* out, int map, int arg1, int arg2); // inferred: map name
// inferred: text (the float after the color is evaluated after it, see game/GameSaveTask.cpp)

// inferred: 8-byte zero object at the start of .rodata, passed as name and file to new[]
static const char sEmpty_unk[8] = "";
const char* const lbl_80885C78 = "savedata.dat"; // inferred: name of the save file
const char* const lbl_80885C7C = "banner.bin";   // inferred: name of the banner file

inline void GameSaveLoadBackgroundCommon::AllocBuffer_unk() {
    m150 = new ((MemoryTag)1, sEmpty_unk, sEmpty_unk, 0) unsigned char[sizeof(GameSaveFile_unk)];
    memset(m150, 0, sizeof(GameSaveFile_unk));
}

GameSaveLoadBackgroundCommon::GameSaveLoadBackgroundCommon(atn::Task* parent)
    : atn::Task(parent), m148(0), m14C(0), m150(0), m154(0) {}

int GameSaveLoadBackgroundCommon::fn_803B8144() {
    if (lbl_8087EF68 != 0) {
        return lbl_8087EF68->m168;
    }
    return 0;
}

// NONMATCHING: 14.9% - the original calls fn_803B83A4 out of line; here it is inlined (it is inlined in
// fn_803B85EC/fn_803B87A8 in the original too). The rest of the function is identical.
void GameSaveLoadBackgroundCommon::fn_803B8160(int slot) {
    char name[0x100];
    int offset;
    int work;
    fn_803B83A4(name, &offset, slot);
    if (lbl_8087EF68 != 0) {
        m154 = 1;
        AllocBuffer_unk();
        fn_800A48B8(lbl_8087EF68, name, m150, sizeof(GameSaveFile_unk), offset, &work);
    }
}

void GameSaveLoadBackgroundCommon::fn_803B8200(int arg) {
    m154 = 1;
    if (arg != 0) {
        AllocBuffer_unk();
    }
}

void GameSaveLoadBackgroundCommon::fn_803B8270() {
    m154 = 0;
    if (m150 != 0) {
        delete[] m150;
        m150 = 0;
    }
}

GameSaveLoadCommon::GameSaveLoadCommon(atn::Task* parent)
    : atn::Task(parent), m48(0), mC84(0), mC88(0), mC8C(0), mD60(0), mD64(1) {}

GameSaveLoadCommon::~GameSaveLoadCommon() {}

void GameSaveLoadBackgroundCommon::fn_803B83A4(char* name, int* offset, int slot) {
    char home[0x40];
    if (lbl_8087F0A8->m198 != 0) {
        name[0] = 0;
        return;
    }
    if (NANDGetHomeDir(home) != 0) {
        return;
    }
    sprintf(name, "%s/%s", home, lbl_80885C78);
    if (slot >= 0) {
        *offset = slot * sizeof(GameSaveFile_unk) + 0x20;
    } else {
        *offset = 0;
    }
}

void GameSaveLoadBackgroundCommon::fn_803B8454(char* name, int* size) {
    char home[0x40];
    if (lbl_8087F0A8->m198 != 0) {
        name[0] = 0;
    } else if (NANDGetHomeDir(home) == 0) {
        sprintf(name, "%s/%s", home, lbl_80885C78);
        *size = 17 * sizeof(GameSaveFile_unk) + 0x20;
    }
}

extern "C" void fn_803B84DC(char* name) {
    char home[0x40];
    if (lbl_8087F0A8->m198 != 0) {
        name[0] = 0;
    } else if (NANDGetHomeDir(home) == 0) {
        sprintf(name, "%s/%s", home, lbl_80885C7C);
    }
}

// inferred: name of the save file (if `name` is given) and size reserved for it
extern "C" void fn_803B854C(char* name, int* size) {
    char home[0x40];
    if (lbl_8087F0A8->m198 != 0) {
        if (name != 0) {
            name[0] = 0;
        }
        *size = 0;
    } else {
        if (name != 0) {
            if (NANDGetHomeDir(home) != 0) {
                return;
            }
            sprintf(name, "%s/%s", home, lbl_80885C78);
        }
        *size = 0x149580;
    }
}

// NONMATCHING: 98.8% - in the inlined fn_803B83A4 of the loop the original tests NANDGetHomeDir with
// `beq body; b skip` instead of `bne skip` (the same inline gives `bne` in fn_803B87A8)
void GameSaveLoadCommon::fn_803B85EC() {
    unsigned char header[0xC0] __attribute__((aligned(32)));
    NandFileInfo_unk info;
    char name[0x100];
    char home[0x40];
    unsigned char buffer[0x8000] __attribute__((aligned(32)));
    if (lbl_8087F0A8->m198 != 0 || NANDGetHomeDir(home) != 0) {
        return;
    }
    for (int i = 0; i < 17; i++) {
        int offset;
        GameSaveLoadBackgroundCommon::fn_803B83A4(name, &offset, i);
        if (fn_8061FC70(name, &info, 1, buffer, sizeof(buffer)) == 0) {
            int pos = NANDSeek(&info, offset, 0);
            if (pos == offset) {
                if (NANDRead(&info, header, sizeof(header)) > 0) {
                    memcpy(&mSlots[i], header, sizeof(GameSaveSlot_unk));
                }
                fn_80620030(&info);
            }
        }
    }
    mD60 = 1;
}

void GameSaveLoadCommon::fn_803B8758(int slot) {
    if (lbl_8087F0A8->m198 != 0) {
        return;
    }
    mC84 = 1;
    if (slot >= 0) {
        mC88 = slot;
        mC8C = slot;
    } else {
        mC8C = 16;
        mC88 = 0;
        lbl_8087DCCC = 0;
        lbl_8087DCC8 = 0;
    }
    mD60 = 0;
}

int GameSaveLoadCommon::fn_803B87A8() {
    switch (mC84) {
    case 0:
        return 0;
    case 1: {
        memset(mCA0, 0, sizeof(mCA0));
        char name[0x100];
        char home[0x40];
        if (lbl_8087F0A8->m198 != 0) {
            name[0] = 0;
        } else if (NANDGetHomeDir(home) == 0) {
            sprintf(name, "%s/%s", home, lbl_80885C78);
        }
        int work;
        if (lbl_8087EF68 != 0) {
            fn_800A4BE4(lbl_8087EF68, name, &work);
        }
        mC84 = 2;
        break;
    }
    case 2: {
        int result = 0;
        if (lbl_8087EF68 == 0 || fn_800A4228(lbl_8087EF68, &result) == 0) {
            if (result == 0) {
                mC84 = 3;
            } else {
                mC84 = 7;
            }
        }
        break;
    }
    case 3: {
        char name[0x100];
        int offset;
        GameSaveLoadBackgroundCommon::fn_803B83A4(name, &offset, mC88);
        int work;
        if (lbl_8087EF68 != 0) {
            fn_800A4D00(lbl_8087EF68, mCA0, sizeof(mCA0), offset, &work);
        }
        mC84 = 4;
        break;
    }
    case 4: {
        int result = 0;
        if (lbl_8087EF68 == 0 || fn_800A4228(lbl_8087EF68, &result) == 0) {
            if (result == 0) {
                memcpy(&mSlots[mC88], mCA0, sizeof(GameSaveSlot_unk));
                if (mSlots[mC88].fn_803BEAB4() != 0) {
                    if (mC88 == 0) {
                        if (lbl_8087F434 == 0) {
                            lbl_8087DCCC = 1;
                        }
                    } else {
                        lbl_8087DCC8 = 1;
                    }
                }
                if (++mC88 <= mC8C) {
                    mC84 = 3;
                } else {
                    mC84 = 5;
                }
            } else {
                mC84 = 7;
            }
        }
        break;
    }
    case 5: {
        int work;
        if (lbl_8087EF68 != 0) {
            fn_800A4E08(lbl_8087EF68, &work);
        }
        mC84 = 6;
        break;
    }
    case 6: {
        int result = 0;
        if (lbl_8087EF68 == 0 || fn_800A4228(lbl_8087EF68, &result) == 0) {
            if (result == 0) {
                mC84 = 0;
                mD60 = 1;
            } else {
                mC84 = 7;
            }
        }
        break;
    }
    case 7:
        return 0;
    }
    if (mC88 < mC8C) {
        fn_8046ECDC(lbl_8087F518, 0);
    }
    return 1;
}

// inferred: nonzero if `path` exists in the NAND
extern "C" int fn_803B8AC4(const char* path) {
    NandStatus_unk stat;
    if (lbl_8087F0A8->m198 != 0) {
        return 0;
    }
    return NANDGetStatus(path, &stat) == 0;
}

// NONMATCHING: 99.9% - only the offsets of the two format strings in @stringBase0: the original pool has
// "", "%s/save%02d.dat", "", "%s/save_sys.dat", "" between "%s/%s" and them (literals of functions
// stripped by the linker, not reconstructed)
void GameSaveLoadCommon::fn_803B8B08(GameSaveSlot_unk* slots) {
    char text[0x100];
    char map[0x100];
    for (int i = 0; i < 17; i++) {
        if (slots[i].fn_803BEAB4() == 0) {
            sprintf(text, "%02d  セーブされていません", i);
        } else {
            fn_8049D52C(map, slots[i].m30, slots[i].m32, slots[i].m31);
            sprintf(text, "%02d  %d/%d/%d %d:%d:%d  Map %s", i, slots[i].m1C, slots[i].m18 + 1, slots[i].m14,
                    slots[i].m10, slots[i].m0C, slots[i].m08, map);
        }
        lbl_8087EEB0->fn_800616C0(100.0f, 18.0f * i + 80.0f, 0.0f, 16.0f, 16.0f, text, i == m48 ? 0xFFFFFF00 : 0xFFFFFFFF, 0.0f, 1, 1, 0);
    }
}
