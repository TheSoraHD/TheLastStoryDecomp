#include "game/StrapTask.h"
#include "atn/Memory.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned char SCGetLanguage();
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path); // inferred: loads a texture
extern "C" void fn_800D2338(atn::Task* task);                              // inferred: deletes a task

// inferred: fn_800D59B8 is a member of the texture class, declared in include/game/GimmickCommon.h
// under this name (nonzero while the texture is loading)
struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

// inferred: globals of unknown classes
struct Unk_8087F4E8 {
    unsigned char m00[0x88];
    int m88;
    unsigned char m8C[0x90 - 0x8C];
    int m90;
};
extern "C" Unk_8087F4E8* lbl_8087F4E8;
struct Unk_8087F018 {
    unsigned char m0000[0x40E0];
    int m40E0;
};
extern "C" Unk_8087F018* lbl_8087F018;
struct Pad_unk {
    unsigned char m00[0x10];
    unsigned short m10; // inferred: buttons
};
extern "C" Pad_unk* lbl_8087EEE0;
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A5920(void* obj, int a, int b); // inferred: a button was pressed
extern "C" void* lbl_8087F9AC;
extern "C" void fn_80572B6C(void* obj, const char* name); // inferred: goes to the named sequence
extern "C" void* lbl_8087F9A0;
extern "C" void fn_8057155C(void* obj, int arg);

StrapTask* StrapTask::fn_805A6208(atn::Task* parent) {
    if (parent != 0) {
        return new (1, "", "", 0) StrapTask(parent);
    }
    return 0;
}

StrapTask::StrapTask(atn::Task* parent) : atn::Task(parent), m48(1), m4C(0), m50(0) {
    unsigned char language = SCGetLanguage();
    for (int i = 0; i < 2; i++) {
        char path[0x100];
        if (language == 6) {
            sprintf(path, "/strap/strap%c_du.texture", 'A' + i);
        } else {
            sprintf(path, "/strap/strap%c_xx.texture", 'A' + i);
        }
        fn_800D5908(&m54[i], path);
    }
    lbl_8087F4E8->m88 = 0;
    lbl_8087F4E8->m90 = 0;
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0 = 0;
    }
}

static void fn_set(int v) {
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0 = v;
    }
}
StrapTask::~StrapTask() {
    fn_set(1);
}

int StrapTask::vf0C() {
    int ready = 1;
    for (int i = 0; i < 2; i++) {
        if (((GimmickUvOwner_unk*)&m54[i])->fn_800D59B8() != 0) {
            ready = 0;
        }
    }
    return ready;
}

void StrapTask::vf10() {
    switch (m48) {
    case 0: {
        int done = 0;
        if (++m50 >= 30 && fn_800A5920(lbl_8087EF70, 0, 0) != 0) {
            done = 1;
        }
        if (m50 >= 599) {
            done = 1;
        }
        if (done != 0) {
            m48 = 2;
            m4C = 0;
        }
        break;
    }
    case 1:
        if ((float)(int)lbl_8087EEE0->m10 > 0.0f) {
            m4C = 0;
            m48 = 3;
        }
        if (++m4C >= 15) {
            m48 = 0;
        }
        break;
    case 2:
        if (++m4C >= 15) {
            fn_800D2338(this);
            lbl_8087F4E8->m88 = 1;
            lbl_8087F4E8->m90 = 1;
            fn_80572B6C(lbl_8087F9AC, "Title");
        }
        break;
    case 3:
        if (++m4C > 45 && fn_800A5920(lbl_8087EF70, 0, 0) != 0) {
            m4C = 15;
            m48 = 4;
            fn_8057155C(lbl_8087F9A0, 0);
        }
        break;
    }
}

// NONMATCHING: vf14 (0x805A6610, 0x470 bytes: draws the strap screen and debug texts) is not written yet
