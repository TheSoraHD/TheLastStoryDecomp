// MP::Manager2 / MP::ManagerMP (multiplayer manager) and the static helpers around them; the
// atn::net::PacketPack methods themselves are in atn/PPDATAU.cpp. One translation unit: a single
// string pool and a single .sdata2 pool for the whole range (see docs/auto-20261102.md).
#include "atn/Task.h"
#include "atn/Model_unk.h"
#include "atn/Keyboard.h"
#include "game/CharaTask.h"
#include "atn/RenderSetting_unk.h"
#include "game/GameTask.h"

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
    ModelSlots_unk* slots = (ModelSlots_unk*)model;
    int prev = slots->mSlots[index].mMotion;
    model->fn_80097C08(index, in->mMotion, in->mLoop, 0, 1, 0.0f, 0.1f);
    float frame = in->mFrame / 63.0f;
    float speed = in->mSpeed / 12.7f;
    if (in->mMotion != prev || (!in->mLoop && (float)__fabs(frame - slots->mSlots[index].mFrame) > 8.0f * speed)) {
        slots->mSlots[index].mFrame = frame;
    } else if (!in->mLoop) {
        float d = frame - slots->mSlots[index].mFrame;
        if ((float)__fabs(d) > 4.0f * speed) {
            if (d > 4.0f * speed) {
                slots->mSlots[index].mFrame = 2.0f * speed + slots->mSlots[index].mFrame;
            } else {
                slots->mSlots[index].mFrame = slots->mSlots[index].mFrame - 2.0f * speed;
            }
        }
    }
    slots->mSlots[index].mSpeed = speed;
    slots->mSlots[index].m10 = in->m04 / 255.0f;
    slots->mSlots[index].m14 = in->m05 / 255.0f;
    slots->mSlots[index].m20 = in->m06 / 255.0f;
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

// inferred: CharaTask, only the members used here (names invented)
struct PPChara_unk;
struct PPCharaRef_unk {
    unsigned char m00[4];
    int m04;
};
struct PPChara_unk {
    unsigned char m0000[0x3FC];
    int m3FC; // inferred: set when a motion state arrives (fn_804EE834)
    unsigned char m0400[0x55C - 0x400];
    int m55C;
    int m560;
    unsigned char m0564[0x638 - 0x564];
    PPCharaRef_unk* m638;
    unsigned char m063C[0x674 - 0x63C];
    int m674;
    unsigned char m0678[0x7E0 - 0x678];
    unsigned int m7E0;
    unsigned char m07E4[0xC48 - 0x7E4];
    int mC48;
    unsigned char m0C4C[0xD1C - 0xC4C];
    int mD1C;
    unsigned char m0D20[0xFB8 - 0xD20];
    float mFB8;
    float mFBC;
    unsigned char m0FC0[0x12A4 - 0xFC0];
    unsigned int m12A4;
    unsigned int m12A8;
};

// inferred: two bytes of a network id (copied byte by byte)
struct PPId_unk {
    unsigned char m00;
    unsigned char m01;
};

// inferred: packed state of a character (0x14 bytes); names invented
struct PackedChara_unk {
    int m00;
    short m04;
    short m06;
    unsigned short m08;
    unsigned short m0A;
    int m0C;
    PPId_unk m10;
    unsigned char m12_7 : 1;
    unsigned char m12_6 : 1;
    unsigned char m12_5 : 1;
    unsigned char m12_4 : 1;
    unsigned char m12_3 : 1;
    unsigned char m12_2 : 1;
    unsigned char m12_1 : 1;
    unsigned char m12_0 : 1;
    unsigned char m13_7 : 1;
    unsigned char m13_6 : 1;
    unsigned char m13_5 : 1;
    unsigned char m13_4 : 1;
    unsigned char m13_3 : 1;
    unsigned char m13_2 : 1;
    unsigned char m13_1 : 1;
    unsigned char m13_0 : 1;
};

extern "C" void* lbl_8087F610; // inferred: MP::Manager2 instance
extern "C" unsigned int fn_804EAEE0(void* mgr, int id);

// NONMATCHING: 99.91% - stack frame 0x30 instead of 0x20: the original shares one slot for the two
// float to integer conversions
// inferred: packs the state of a character
extern "C" void fn_804CF468(PackedChara_unk* out, CharaTask* task) {
    PPChara_unk* chara = (PPChara_unk*)task;
    out->m00 = chara->m674;
    out->m04 = chara->m55C;
    out->m06 = chara->m560;
    out->m08 = task->fn_80160324() ? (unsigned short)chara->mFB8 : 0;
    out->m0A = task->fn_80160324() ? (unsigned short)chara->mFBC : 0;
    out->m0C = chara->m638 != 0 ? chara->m638->m04 : 0;
    out->m12_7 = chara->m12A4 >> 5;
    out->m12_6 = chara->m12A4 >> 6;
    out->m12_5 = chara->m12A4 >> 31;
    out->m12_4 = chara->m12A4 >> 10;
    out->m12_3 = chara->m12A8 >> 8;
    out->m12_2 = chara->m12A8 >> 1;
    out->m12_0 = (chara->m12A4 >> 31) && chara->mC48 != 0;
    out->m12_1 = 0;
    unsigned short id = fn_804EAEE0(lbl_8087F610, chara->mD1C) >> 16;
    out->m10 = *(PPId_unk*)&id;
    out->m13_7 = chara->m7E0 >> 19;
    out->m13_6 = chara->m7E0 >> 2;
    out->m13_5 = chara->m7E0 >> 25;
    out->m13_4 = chara->m7E0 >> 22;
    out->m13_3 = chara->m7E0;
    out->m13_2 = chara->m7E0 >> 15;
    out->m13_1 = chara->m7E0 >> 5;
    out->m13_0 = chara->m7E0 >> 7;
}

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

extern "C" void fn_804D00B8(int index, int value) {
    if (lbl_8087F430 != 0) {
        ((GameTask*)lbl_8087F430)->fn_80370AE4(index, value);
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

extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (atn::RenderConfig)

// inferred: render settings of the multiplayer game: sepia-like color matrix, no bloom and no depth
// of field, grey clear color (SetClearColor__Q23atn12RenderConfigFRCQ23atn5Color inlined)
extern "C" void fn_804D0164() {
    ColorMatrixParam_unk cm;
    cm.mEnabled = 1;
    cm.mRows[0].x = 0.9f;
    cm.mRows[0].y = 0.1f;
    cm.mRows[0].z = 0.1f;
    cm.mRows[0].w = 0.0f;
    cm.mRows[1].x = 0.1f;
    cm.mRows[1].y = 0.9f;
    cm.mRows[1].z = 0.1f;
    cm.mRows[1].w = 0.0f;
    cm.mRows[2].x = 0.1f;
    cm.mRows[2].y = 0.1f;
    cm.mRows[2].z = 0.9f;
    cm.mRows[2].w = 0.0f;
    cm.mRows[3].x = 0.0f;
    cm.mRows[3].y = 0.0f;
    cm.mRows[3].z = 0.0f;
    cm.mRows[3].w = 0.0f;
    lbl_8087EFA8->m324 = cm;
    lbl_8087EFA8->mDof.mEnabled = 0;
    lbl_8087EFA8->m054.mEnabled = 0;
    lbl_8087EFA8->m03C = RsColor_unk(0.8627f, 0.8627f, 0.8627f, 1.0f);
}

// ---- player slots of MP::Manager2 and the message handlers of the receive loop (fn_804EE040, switch
// over the message id, jumptable_807912DC) ----
#include "atn/MemoryManager.h"
#include "atn/net/PacketPack.h"
#include "MP/D2AMpMenuLobby.h"
#include "game/GameVoice.h"

// inferred: flags of a player slot (MSB first); names invented
struct PPPlayerFlags_unk {
    unsigned int mActive : 1; // inferred: slot in use
    unsigned int m30 : 1;
    unsigned int m29 : 1;
    unsigned int m28 : 1;     // inferred: set and cleared by messages 0x10 and 0x11
    unsigned int m27 : 1;     // inferred: set by message 0x17
    unsigned int m26 : 1;
    unsigned int m22 : 4;     // inferred: value of message 0x12
    unsigned int m00 : 22;
};

// inferred: player slot of MP::Manager2 (0xD5C bytes, array at +0x5E4); names invented
struct PPPlayer_unk {
    CharaTask* mChara;                // 0x00
    unsigned char m004[0xCC - 0x4];
    unsigned char mAid;               // 0xCC: inferred: network id of the player
    unsigned char m0CD[0xD0 - 0xCD];
    PPPlayerFlags_unk mFlags;         // 0xD0
    unsigned char m0D4[0xD38 - 0xD4];
    int mD38[9];                      // inferred: values of message 0x15

};

// inferred: MP::Manager2 (lbl_8087F610), only the members used here; names invented
struct PPManager2_unk {
    unsigned char m0000[0x4FC];
    int mState;                       // 0x4FC: inferred: state (30 = in game)
    unsigned char m0500[0x55C - 0x500];
    int m55C;                         // inferred: values of messages 0x0C and 0x0E
    int m560;                         // inferred: settings of message 0x0F (m560-m598)
    int m564;
    int m568;
    unsigned char m56C[0x20];
    unsigned char m58C[0xC];
    int m598;
    unsigned char m059C[0x5A0 - 0x59C];
    int m5A0;
    unsigned char m05A4[0x5A8 - 0x5A4];
    int m5A8;
    int m5AC;                         // inferred: the settings of message 0x0F arrived
    unsigned char m05B0[0x5E4 - 0x5B0];
    PPPlayer_unk* mPlayers;           // 0x5E4
    int mPlayerNum;                   // 0x5E8
    unsigned char m05EC[0x9694 - 0x5EC];
    int m9694[8];                     // inferred: per aid values of message 0x35
    unsigned char m96B4[0x9984 - 0x96B4];
    unsigned char m9984[8];           // inferred: two 8-byte blocks of message 0x28
    unsigned char m998C[8];
    unsigned char m9994[0x9A14 - 0x9994];
    int m9A14[8];                     // inferred: per aid flags set by messages 0x38 and 0x3B
    int m9A34[8];
};

// inferred: the player slot of network id `aid` (slots in use only)
extern "C" PPPlayer_unk* fn_804DD158(PPManager2_unk* mgr, unsigned int aid) {
    for (int i = 0; i < mgr->mPlayerNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1 && aid == player->mAid) {
            return player;
        }
    }
    return 0;
}

// inferred: getter of the packer (out of line, see MP/CryptMP.cpp)
atn::net::PacketPack* fn_804AE3BC();
extern "C" void* fn_800838B8(atn::MemoryManager* mgr, unsigned long size, unsigned long align, int heap,
                             const char* name, const char* file, int line);
extern "C" void* lbl_8087F600;
extern "C" const char lbl_8087E1B0[1];
extern "C" const char lbl_8087E1AC[1];
inline void* atn::net::PacketPack::GetWorkBuffer_unk() {
    if (lbl_8087F600 == 0) {
        lbl_8087F600 = fn_800838B8(atn::MemoryManager::Instance(), 0x1000, 4, 5, lbl_8087E1B0, lbl_8087E1AC, 0);
    }
    return lbl_8087F600;
}
extern "C" void* lbl_8087F628; // inferred: atn::net::Manager instance
// inferred: the network manager and the packer exist and the data of the message is ready
#define PP_NO_DATA_UNK (lbl_8087F628 == 0 || fn_804AE3BC() == 0 || fn_804AE3BC()->GetWorkBuffer_unk() == 0)

namespace atn {

namespace net {

void fn_8050A7F4(unsigned int aid, const void* data);
void fn_8050A8A4(unsigned int aid, const void* data);
void fn_8050A960(unsigned int aid, const void* data);
void fn_8050A980(unsigned int aid, const void* data);
struct NetPlayerInfo_unk;
void fn_8050A72C(unsigned int aid, const NetPlayerInfo_unk* info);
void fn_8050A990(unsigned int aid, const unsigned char* data);
} // namespace net
} // namespace atn

// inferred: empty traces of the receive loop, called on the packer (their arguments are still evaluated);
// fn_804EE4E4 gives the data back unchanged
extern "C" void fn_804EE4E0(atn::net::PacketPack* pack, unsigned int aid, unsigned short size) {}

extern "C" const void* fn_804EE4E4(const void* data) {
    return data;
}

extern "C" void fn_804EE4E8(atn::net::PacketPack* pack, unsigned int aid, unsigned short size) {}

extern "C" void fn_804EE4EC(atn::net::PacketPack* pack, const void* data, unsigned short size) {}

extern "C" unsigned int fn_804EE4F0(PPPlayer_unk* player, const void* data); // inferred: applies a character state

// inferred: message 4: state of the character of a player, then its motion (at +0x2C of the data)
extern "C" void fn_804EE834(PPPlayer_unk* player) {
    if (fn_804EE4F0(player, fn_804AE3BC()->GetWorkBuffer_unk()) == 1) {
        void* data = fn_804AE3BC()->GetWorkBuffer_unk();
        CharaTask* chara = player->mChara;
        if (chara != 0) {
            fn_804CF198((PackedMotion_unk*)((unsigned char*)data + 0x2C), chara->GetModel_unk(), 0);
            ((PPChara_unk*)player->mChara)->m3FC = 1;
        }
    }
}

// inferred: message 9: a voice of the data (id, offset)
extern "C" void fn_804EFE98() {
    if (lbl_8087F628 != 0 && fn_804AE3BC() != 0 && fn_804AE3BC()->GetWorkBuffer_unk() != 0) {
        unsigned int* data = (unsigned int*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (lbl_8087F498 != 0) {
            lbl_8087F498->fn_803E9B7C(data[0], data[1], 1, 1.0f);
        }
    }
}

// inferred: messages 0x0C and 0x0E: values kept by the manager
extern "C" void fn_804F1068() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    int* data = (int*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m55C = data[0];
}

extern "C" void fn_804F12A0() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    int* data = (int*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m568 = data[0];
}

// inferred: data of message 0x0F: settings of the game sent by the host; names invented
struct PPGameSettings_unk {
    int m00;
    int m04;
    int m08;
    unsigned char m0C[0x20];
    unsigned char m2C[0xC];
    int m38;
};
extern "C" void fn_804F135C() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    PPGameSettings_unk* data = (PPGameSettings_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m560 = data->m00;
    ((PPManager2_unk*)lbl_8087F610)->m564 = data->m04;
    ((PPManager2_unk*)lbl_8087F610)->m568 = data->m08;
    memcpy(((PPManager2_unk*)lbl_8087F610)->m56C, data->m0C, sizeof(data->m0C));
    memcpy(((PPManager2_unk*)lbl_8087F610)->m58C, data->m2C, sizeof(data->m2C));
    ((PPManager2_unk*)lbl_8087F610)->m598 = data->m38;
    ((PPManager2_unk*)lbl_8087F610)->m5AC = 1;
}

// inferred: flags of a player slot set by the messages
extern "C" void fn_804F1478(PPPlayer_unk* player) {
    player->mFlags.m28 = 1;
}

extern "C" void fn_804F1488(PPPlayer_unk* player) {
    player->mFlags.m28 = 0;
}


// inferred: message 0x12: a 4-bit value of the player slot (bits 22-25 of the flags)
extern "C" void fn_804F1498(PPPlayer_unk* player) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned int* data = (unsigned int*)fn_804AE3BC()->GetWorkBuffer_unk();
    player->mFlags.m22 = data[0];
}

// inferred: message 0x15: a value of the player slot
extern "C" void fn_804F1CA0(PPPlayer_unk* player) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    int* data = (int*)fn_804AE3BC()->GetWorkBuffer_unk();
    player->mD38[data[0]] = data[1];
}

extern "C" void fn_804F1F24(PPPlayer_unk* player) {
    player->mFlags.m27 = 1;
}

// inferred: message 0x1F: shield parts of the character of the player (count in the data)
extern "C" void fn_804F2C44(PPPlayer_unk* player) {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    int* data = (int*)fn_804AE3BC()->GetWorkBuffer_unk();
    if (player->mChara != 0) {
        if (*data > 0) {
            player->mChara->fn_8014F698(0, *data, 0);
        } else {
            player->mChara->fn_8014F960(0);
        }
    }
}

extern "C" void fn_804F2CE4() {}


extern "C" void fn_804F2CE8(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    atn::net::fn_8050A72C(aid, (const atn::net::NetPlayerInfo_unk*)fn_804AE3BC()->GetWorkBuffer_unk());
}

// inferred: messages 0x22, 0x23, 0x25 and 0x26 forwarded to the network manager handlers (no data of their
// own); 0x21 and 0x24 pass their data
extern "C" void fn_804F2DAC(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    atn::net::fn_8050A960(aid, 0);
}

extern "C" void fn_804F2E3C(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    atn::net::fn_8050A980(aid, 0);
}

extern "C" void fn_804F2ECC(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    atn::net::fn_8050A990(aid, (const unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk());
}

extern "C" void fn_804F2F90(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    atn::net::fn_8050A7F4(aid, 0);
}

extern "C" void fn_804F3020(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    atn::net::fn_8050A8A4(aid, 0);
}

// inferred: message 0x27: a signed byte kept by the manager (m5A0)
extern "C" void fn_804F30B0() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    signed char* data = (signed char*)fn_804AE3BC()->GetWorkBuffer_unk();
    PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
    mgr->m5A0 = *data;
}

// inferred: message 0x28: two 8-byte blocks kept by the manager
extern "C" void fn_804F3170() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned char* data = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    memcpy(((PPManager2_unk*)lbl_8087F610)->m9984, data, 8);
    memcpy(((PPManager2_unk*)lbl_8087F610)->m998C, data + 8, 8);
}

extern "C" void fn_804F36F4(PPPlayer_unk* player, unsigned int aid) {}

// inferred: message 0x30: a byte kept by the manager (m5A8)
extern "C" void fn_804F3AF0() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned char* data = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m5A8 = data[0];
}

extern "C" void fn_804F50DC(PPManager2_unk* mgr, unsigned int aid, unsigned short a, unsigned short b);

// inferred: message 0x32: two halfwords handled by the manager (fn_804F50DC)
extern "C" void fn_804F3D80(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned short* data = (unsigned short*)fn_804AE3BC()->GetWorkBuffer_unk();
    fn_804F50DC((PPManager2_unk*)lbl_8087F610, aid, data[0], data[1]);
}

// inferred: message 0x35: a value per network id
extern "C" void fn_804F4728() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned int* data = (unsigned int*)fn_804AE3BC()->GetWorkBuffer_unk();
    if (data[0] <= 7) {
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        mgr->m9694[data[0]] = data[1];
    }
}

// inferred: messages 0x38 and 0x3B: flags per network id
extern "C" void fn_804F4E1C(unsigned char aid) {
    if (aid < 8) {
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        mgr->m9A14[aid] = 1;
    }
}

extern "C" void fn_804F4F40(unsigned char aid) {
    if (aid < 8) {
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        mgr->m9A34[aid] = 1;
    }
}

// inferred: message 0x3E: the lobby is told to refresh (mF90)
extern "C" void fn_804F50C4() {
    MP::D2AMpMenuLobby* lobby = MP::D2AMpMenuLobby::sInstance;
    if (lobby != 0) {
        lobby->mF90 = 1;
    }
}
