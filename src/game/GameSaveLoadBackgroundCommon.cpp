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
extern "C" int NANDSimpleSafeOpen(const char* path, NandFileInfo_unk* info, unsigned char mode, void* buf, unsigned int len);
extern "C" int NANDSimpleSafeClose(NandFileInfo_unk* info);

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
// inferred: text (the float after the color is evaluated after it, see game/GameSaveTask.cpp)

// inferred: 8-byte zero object at the start of .rodata, passed as name and file to new[]
static const char sEmpty_unk[8] = "";

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

// fn_803B83A4 is defined in game/GameSaveLoadCommon.cpp, so it is called out of line here
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

