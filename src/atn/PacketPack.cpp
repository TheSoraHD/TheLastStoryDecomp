// MP::Manager2 / MP::ManagerMP (multiplayer manager) and the static helpers around them; the
// atn::net::PacketPack methods themselves are in atn/PPDATAU.cpp. One translation unit: a single
// string pool and a single .sdata2 pool for the whole range (see docs/auto-20261102.md).
#include "atn/Task.h"
#include "atn/Model_unk.h"
#include "atn/Keyboard.h"

extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes a task
extern "C" long long OSGetTime();

// inferred: packed motion state of a model slot (9 bytes, sent over the network); names invented
struct PackedMotion_unk {
    unsigned short mMotion; // 0x00
    unsigned short mFrame;  // 0x02: frame * 63
    unsigned char m04;      // 0x04: weights * 255
    unsigned char m05;
    unsigned char m06;
    unsigned char mLoop : 1; // 0x07
    unsigned char m07 : 7;
    signed char mSpeed;     // 0x08: speed * 12.7
};

// inferred: motion slot of an atn::Model (0x30 bytes from 0x22C); names invented
struct ModelSlot_unk {
    int mMotion;   // 0x00
    int m04;
    float mFrame;  // 0x08
    float mSpeed;  // 0x0C
    float m10;
    float m14;
    unsigned char mLoop; // 0x18
    unsigned char m19[0x20 - 0x19];
    float m20;
    unsigned char m24[0x30 - 0x24];
};

// inferred: the slots of a model (only the part used here)
struct ModelSlots_unk {
    unsigned char m000[0x22C];
    ModelSlot_unk mSlots[16];
};

// inferred: game field (lbl_8087F430), only the members used here
struct PPField_unk {
    unsigned char m0000[0x54E4];
    int m54E4; // inferred: game mode
};
extern "C" PPField_unk* lbl_8087F430;

extern "C" void fn_804CF190(atn::Task* task) {
    fn_800D2338(task);
}

extern "C" long long fn_804CF194() {
    return OSGetTime();
}

// inferred: applies a packed motion state to slot `index` of a model
extern "C" void fn_804CF198(PackedMotion_unk* in, atn::Model* model, int index) {
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 10) {
        return;
    }
    ModelSlot_unk& slot = ((ModelSlots_unk*)model)->mSlots[index];
    int prev = slot.mMotion;
    model->fn_80097C08(index, in->mMotion, in->mLoop, 0, 1, 0.0f, 0.1f);
    float frame = in->mFrame / 63.0f;
    float speed = in->mSpeed / 12.7f;
    if (in->mMotion != prev || (!in->mLoop && (float)__fabs(frame - slot.mFrame) > 8.0f * speed)) {
        slot.mFrame = frame;
    } else if (!in->mLoop) {
        float d = frame - slot.mFrame;
        if ((float)__fabs(d) > 4.0f * speed) {
            if (d > 4.0f * speed) {
                slot.mFrame = 2.0f * speed + slot.mFrame;
            } else {
                slot.mFrame = slot.mFrame - 2.0f * speed;
            }
        }
    }
    slot.mSpeed = speed;
    slot.m10 = in->m04 / 255.0f;
    slot.m14 = in->m05 / 255.0f;
    slot.m20 = in->m06 / 255.0f;
}

// inferred: packs the motion state of slot `index` of a model
extern "C" void fn_804CF374(PackedMotion_unk* out, atn::Model* model, int index) {
    ModelSlot_unk& slot = ((ModelSlots_unk*)model)->mSlots[index];
    out->mMotion = slot.mMotion;
    out->mFrame = slot.mFrame * 63.0f;
    out->mSpeed = slot.mSpeed * 12.7f;
    out->m04 = slot.m10 * 255.0f;
    out->m05 = slot.m14 * 255.0f;
    out->m06 = slot.m20 * 255.0f;
    out->mLoop = slot.mLoop;
}

// inferred: slot with the largest weight
struct ModelSlotsN_unk {
    unsigned char m000[0x22C];
    ModelSlot_unk mSlots[(0x34C - 0x22C) / 0x30];
    int mCount; // 0x34C
};
extern "C" int fn_804CF424(ModelSlotsN_unk* model) {
    float max = 0.0f;
    int best = 0;
    for (int i = 0; i < model->mCount; i++) {
        if (max < model->mSlots[i].m20) {
            max = model->mSlots[i].m20;
            best = i;
        }
    }
    return best;
}

// NONMATCHING: stub
extern "C" void fn_804CF468(void* out, void* chara) {}

// inferred: chara flags (CharaTask::m12A8), only the member used here
struct PPChara_unk {
    unsigned char m0000[0x12A8];
    unsigned int m12A8;
};

extern "C" int fn_804CF634(PPChara_unk* chara) {
    return (chara->m12A8 >> 1) & 1;
}

// NONMATCHING: stub
extern "C" void fn_804CF640(void* in, void* chara) {}

// inferred: object with a flag word at 0x08 (name invented)
struct PPFlags_unk {
    unsigned char m00[8];
    unsigned int m08;
};

extern "C" void fn_804D0090(PPFlags_unk* obj, int on) {
    if (on) {
        obj->m08 |= 1;
    } else {
        obj->m08 &= ~1;
    }
}

extern "C" void fn_80370AE4(PPField_unk* field, int index, int value); // inferred: sets a counter

extern "C" void fn_804D00B8(int index, int value) {
    if (lbl_8087F430 != 0) {
        fn_80370AE4(lbl_8087F430, index, value);
    }
}

// NONMATCHING: stub (debug keyboard test; the original passes the keyboard itself as the key state)
extern "C" int fn_804D00DC(unsigned short key) {
    return 0;
}

extern "C" void* lbl_8087EEA8;
extern "C" int fn_8005C220(void* obj, int a, int b);

extern "C" int fn_804D0140(int arg) {
    if (lbl_8087EEA8 != 0) {
        return fn_8005C220(lbl_8087EEA8, 0, arg);
    }
    return 0;
}
