// inferred file name: from the RTTI of BootTask (vtable 0x80779ED8): the first sequence of the game. Its own
// file: the RTTI name and a string pool of its own come before the strings of game/Bullet.cpp in .rodata.
#include "game/BootTask.h"
#include "game/SetConfig.h"
#include "atn/LoadManager.h"
#include "atn/Memory.h"
#include "atn/SequenceControl.h"
#include "atn/SoundManager.h"

// inferred: sound engine (lbl_8087EE90)
extern "C" atn::SoundEngine_unk* lbl_8087EE90;
extern "C" void fn_80046EE0(atn::SoundEngine_unk* engine, const char* path); // inferred: opens the sound archive
extern "C" int fn_800499D0(atn::SoundEngine_unk* engine);                    // inferred: output mode
extern "C" void fn_80047B54(atn::SoundEngine_unk* engine, int arg);          // inferred
extern "C" void fn_802144A8();                                               // inferred: loads the sound tables
extern "C" int fn_803CE148();
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task

// inferred: game settings (lbl_8087F0A8); only the fields read here
struct BootSettings_unk {
    unsigned char m000[0x30];
    int m30; // inferred: frames per second
    unsigned char m034[0x1B4 - 0x34];
    char m1B4[1]; // inferred: name of the sequence started after the boot
};
extern "C" BootSettings_unk* lbl_8087F0A8;

// inferred: creator of the boot sequence (registered as "BootTask" by atn/SequenceUnitParam1_const_int)
atn::Task* fn_8010EB58(atn::Task* const parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) BootTask(parent);
    }
    return 0;
}

// inferred: opens the sound archive once the loader is running, then waits for the loads
int BootTask::vf0C() {
    switch (mState) {
    case 0:
        if (!lbl_8087F518->IsFlag_unk(2)) {
            break;
        }
        lbl_8087F518->fn_8046EC7C();
        fn_80046EE0(lbl_8087EE90, "/sound/LastWorld_xx.brsar");
        lbl_8087EFE8->mOutputMode = fn_800499D0(lbl_8087EE90);
        if (SetConfig::sInstance != 0) {
            SetConfig::sInstance->fn_8057F0F8();
        }
        fn_802144A8();
        {
            atn::SoundEngine_unk* engine = lbl_8087EE90;
            fn_80047B54(engine, fn_803CE148());
        }
        mState = 1;
    case 1:
        if (lbl_8087F518->fn_8046ECC8() == 0) {
            mState = 3;
        }
        break;
    }
    return mState == 3;
}

// inferred: removes itself and starts the first sequence
void BootTask::vf10() {
    fn_800D2338(this);
    atn::SequenceControl::sInstance->mNextId = atn::SequenceControl::sInstance->fn_800C37FC(lbl_8087F0A8->m1B4);
}

BootTask::~BootTask() {}

