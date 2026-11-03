// game/GameLoadTask.cpp: debug load menu (0x803B27F8-0x803B33F8). It starts at 0x803B27F8; the
// virtual-base destructor thunks of basic_istream/basic_iostream/basic_stringstream<char>
// (0x803B27AC-0x803B27F8) belong to game/GameEventScript.
#include "game/GameLoadTask.h"
#include "game/GameLoadBackground.h"
#include "MP/SaveLoad.h"
#include "atn/SequenceBoot_unk.h"
#include "atn/FileList.h"
#include "atn/DrawHelper.h"
#include "std/ios_init.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int sprintf(char* dst, const char* fmt, ...);

extern "C" void* lbl_8087EF70;                               // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" void fn_800D2338(atn::Task* task);                // inferred: removes the task
extern "C" void* fn_8006BA8C(const char* path, int* size, int flag); // inferred: loads a file
extern "C" MP::SaveData_unk* lbl_8087F460; // inferred: current save data (game/GameSaveData_unk)
extern "C" int lbl_8087DD60;               // inferred: slot of the current save data
extern "C" int lbl_8087DD64;               // inferred: format of the current save data
extern "C" int lbl_8087F630;               // inferred: number of LAN match players
// inferred: called after the save data is loaded (game/GameSaveData_unk.cpp)
extern "C" void fn_803BE850(MP::SaveData_unk* data);

// inferred: name shown for an empty slot (.sdata 0x8087DD68)
static const char* sNewName_unk = "(New)";

// inferred: header of the slot that was loaded last (read by MP::SaveLoad); kept with its dtk name
extern "C" {
GameSaveSlot_unk lbl_807C8508;
}

// inferred: text of the LAN match line (0x807C85C0)
static char sLanText_unk[0x40];

// inferred: replaces the current save data with an empty one (inlined three times)
// inferred: size of the save data (MP::SaveData_unk only declares its first 0xFF54 bytes)
static const unsigned long kSaveDataSize_unk = 0x134A8;

static inline void ResetSaveData_unk() {
    if (lbl_8087F460 != 0) {
        delete lbl_8087F460;
        lbl_8087F460 = 0;
    }
    lbl_8087F460 = (MP::SaveData_unk*)new ((MemoryTag)1, "", "", 0) GameSaveData_unk;
}

// inferred: format of a slot (GameSaveSlot_unk::m04)
static inline int SlotFormat_unk(const GameSaveSlot_unk& slot) {
    return slot.m04;
}

// inferred: the option names the "Multiplayer" sequence
static inline bool RangeEq_unk(const char* first, const char* last, const char* lit) {
    for (; first != last; first++, lit++) {
        if (*first != *lit) {
            return false;
        }
    }
    return true;
}

static inline bool IsMultiplayer_unk(const char* name) {
    const char* lit = "Multiplayer";
    int n = strlen(name);
    bool eq = lit[n] == 0;
    if (eq) {
        eq = RangeEq_unk(name, name + n, lit);
    }
    return eq;
}

GameLoadTask* GameLoadTask::fn_803B27F8(atn::Task* parent, const Option& option) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameLoadTask(parent, option);
    }
    return 0;
}

GameLoadTask::GameLoadTask(atn::Task* parent, const Option& option)
    : GameSaveLoadCommon(parent), mD68(0), mD6C(option), mDB0(0), mDB1(0) {
    m14F60 = GameLoadBackground::fn_803B33F8(this);
    if (strlen(mD6C.mNext) != 0) {
        fn_803B85EC();
    } else if (mD6C.mFlag != 0) {
        fn_803B8758(-1);
    }
}

GameLoadTask::OptionTmp_unk::~OptionTmp_unk() {}

GameLoadTask::~GameLoadTask() {}

int GameLoadTask::vf0C() {
    return fn_803B87A8() == 0;
}

// NONMATCHING: 99.95% - in the "Debug Data" branch the file buffer is in r29 and the deleted save data in
// r30 (swapped); declaring the buffer earlier or scoping the reset does not change it
void GameLoadTask::vf10() {
    if (mD64 == 0) {
        return;
    }
    switch (mD68) {
    case 0:
        mD68 = 1;
        break;
    case 1:
        IsMultiplayer_unk(mD6C.mNext);
        if (fn_800A55AC(lbl_8087EF70, 0, 0) || fn_800A55AC(lbl_8087EF70, 0, 0x1A)) {
            if (IsMultiplayer_unk(mD6C.mNext)) {
                m48 = m48 >= 0 ? m48 - 1 : 16;
            } else {
                m48 = m48 != 0 ? m48 - 1 : 16;
            }
        } else if (fn_800A55AC(lbl_8087EF70, 0, 1) || fn_800A55AC(lbl_8087EF70, 0, 0x19)) {
            m48 = (m48 + 1) % 17;
        }
        if (fn_800A555C(lbl_8087EF70, 0, 4)) {
            if (m48 < 0) {
                mD68 = 4;
            } else if (mDB0 != 0) {
                if (mDB4[m48].fn_803BEAB4()) {
                    int offset;
                    int size;
                    char path[0x100];
                    sprintf(path, "/test/savedata/savedata.dat");
                    ResetSaveData_unk();
                    char* buf = (char*)fn_8006BA8C(path, &size, 0);
                    GameSaveLoadBackgroundCommon::fn_803B83A4(path, &offset, m48);
                    memcpy(lbl_8087F460, buf + offset + sizeof(GameSaveSlot_unk), kSaveDataSize_unk);
                    atn::FileFreeBuffer(buf, false);
                    fn_803BE850(lbl_8087F460);
                    mD68 = 4;
                }
            } else if (mSlots[m48].fn_803BEAB4()) {
                fn_803B3270(m48);
            }
        }
        if (fn_800A555C(lbl_8087EF70, 0, 5)) {
            sSequenceBoot_unk->SetNext_unk("Title");
            fn_800D2338(this);
        }
        break;
    case 3:
        if (m14F60->m154 == 0) {
            mD68 = 4;
        }
        break;
    case 4:
        if (mDB0 == 0 && m48 >= 0) {
            ResetSaveData_unk();
            lbl_8087DD60 = m48;
            lbl_8087DD64 = SlotFormat_unk(mSlots[m48]);
            if (lbl_8087DD64 < 4) {
                memcpy(lbl_8087F460, m1A00.mHeader + 0x38, kSaveDataSize_unk);
            } else {
                memcpy(lbl_8087F460, &m1A00.mData, kSaveDataSize_unk);
            }
            fn_803BE850(lbl_8087F460);
        }
        sSequenceBoot_unk->SetNext_unk(mD6C.mNext);
        fn_800D2338(this);
        break;
    }
}

void GameLoadTask::fn_803B2FBC(int slot) {
    if (slot >= 0) {
        ResetSaveData_unk();
        lbl_8087DD60 = slot;
        lbl_8087DD64 = SlotFormat_unk(mSlots[slot]);
        if (lbl_8087DD64 < 4) {
            memcpy(lbl_8087F460, m1A00.mHeader + 0x38, kSaveDataSize_unk);
        } else {
            memcpy(lbl_8087F460, &m1A00.mData, kSaveDataSize_unk);
        }
        fn_803BE850(lbl_8087F460);
    }
}

void GameLoadTask::vf14() {
    if (mD64 != 0) {
        if (mDB0 != 0) {
            fn_803B8B08(mDB4);
        } else {
            fn_803B8B08(mSlots);
        }
        if (IsMultiplayer_unk(mD6C.mNext)) {
            const char* text;
            if (lbl_8087F630 == 0) {
                text = sNewName_unk;
            } else {
                sprintf(sLanText_unk, "(New) *** Enable LAN MATCH players:%d ***", lbl_8087F630);
                text = sLanText_unk;
            }
            lbl_8087EEB0->fn_800616C0(100.0f, 62.0f, 0.0f, 16.0f, 16.0f, text, m48 == -1 ? 0xFFFFFF00 : 0xFFFFFFFF, 0.0f, 1, 1, 0);
        }
        if (mDB0 != 0) {
            lbl_8087EEB0->fn_800616C0(60.0f, 40.0f, 0.0f, 16.0f, 16.0f, "Debug Data", 0xFF00FFFF, 0.0f, 1, 1, 0);
        }
    }
}

void GameLoadTask::fn_803B3270(int slot) {
    char name[0x100];
    int size;
    GameSaveLoadBackgroundCommon::fn_803B83A4(name, &size, slot);
    memset(&m1A00, 0, sizeof(GameSaveFile_unk));
    m14F60->fn_803B3530(name, (int)&m1A00, sizeof(GameSaveFile_unk), size);
    memcpy(&lbl_807C8508, &mSlots[slot], sizeof(GameSaveSlot_unk));
    mD68 = 3;
}

void GameLoadTask::fn_803B3310(int slot) {
    GameLoadBackground* bg = m14F60;
    if (bg == 0) {
        mD68 = 1;
    } else {
        if (bg != 0) {
            bg->fn_803B8160(slot);
            memset(&mSlots[slot], 0, sizeof(GameSaveSlot_unk));
        }
        mD68 = 5;
    }
}
