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
    unsigned char m07E4[0x9F8 - 0x7E4];
    int m9F8; // inferred: value of message 0x2F
    unsigned char m09FC[0xC48 - 0x9FC];
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
#include "game/GameBgm.h"
#include "game/SetConfig.h"
#include "MP/MpCharaRef_unk.h"
#include "MP/SaveLoad.h"

// inferred: state of a player sent by message 0x16 (0x7C bytes); names invented
struct PPPair_unk {
    int m00;
    int m04;
};
struct PPPlayerState_unk {
    int m00;
    int m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    int m20;
    PPPair_unk m24;
    PPPair_unk m2C;
    PPPair_unk m34;
    PPPair_unk m3C;
    PPPair_unk m44;
    PPPair_unk m4C;
    PPPair_unk m54;
    PPPair_unk m5C;
    PPPair_unk m64;
    PPPair_unk m6C;
    PPPair_unk m74;
};

// inferred: flags of a player slot (MSB first); names invented
struct PPPlayerFlags_unk {
    unsigned int mActive : 1; // inferred: slot in use
    unsigned int m30 : 1;
    unsigned int m29 : 1;
    unsigned int m28 : 1;     // inferred: set and cleared by messages 0x10 and 0x11
    unsigned int m27 : 1;     // inferred: set by message 0x17
    unsigned int m26 : 1;     // inferred: set by message 0x18 on the player and its team
    unsigned int m22 : 4;     // inferred: value of message 0x12
    unsigned int m00 : 22;
};

// inferred: player slot of MP::Manager2 (0xD5C bytes, array at +0x5E4); names invented
struct PPPlayer_unk {
    MpCharaRef_unk mRef;              // 0x00: inferred: the character of the player
    unsigned char m004[0x34 - 0x4];
    PPPlayerState_unk m34;            // inferred: state sent by message 0x16
    unsigned char m0B0[0xCC - 0xB0];
    unsigned char mAid;               // 0xCC: inferred: network id of the player
    unsigned char m0CD[0xD0 - 0xCD];
    PPPlayerFlags_unk mFlags;         // 0xD0
    unsigned char m0D4[0xD38 - 0xD4];
    int mD38[5];                      // inferred: values of message 0x15
    unsigned int mD4C;                // inferred: bits 10-11 set by message 0x2E
    unsigned char m0D50[0xD5C - 0xD50];

};

// inferred: array of the player slots of MP::Manager2 (pointer and count); names invented
struct PPPlayerArray_unk {
    PPPlayer_unk* mData;
    int mNum;
};

// inferred: entry of the character list of MP::Manager2 (0xB4 bytes, array at +0x5F0); names invented
struct PPCharaEntry_unk {
    MpCharaRef_unk mRef;
    unsigned char m04[0xB4 - 0x4];
};

// inferred: MP::Manager2 (lbl_8087F610), only the members used here; names invented
struct PPManager2_unk {
    unsigned char m0000[0x4FC];
    int mState;                       // 0x4FC: inferred: state (30 = in game)
    unsigned char m0500[0x50A - 0x500];
    short m50A;                       // inferred: set to 90 when a player slot is added
    unsigned char m050C[0x514 - 0x50C];
    MP::SaveLoad* mSaveLoad;          // 0x514
    unsigned char m0518[0x540 - 0x518];
    int mMode;                        // 0x540: inferred: game mode
    unsigned char m0544[0x55C - 0x544];
    int m55C;                         // inferred: values of messages 0x0C and 0x0E
    int m560;                         // inferred: settings of message 0x0F (m560-m598)
    int m564;                         // inferred: also the time limit of the game (seconds)
    int m568;
    unsigned char m56C[0x20];
    unsigned char m58C[0xC];
    int m598;
    unsigned char m059C[0x5A0 - 0x59C];
    int m5A0;
    unsigned char m05A4[0x5A8 - 0x5A4];
    int m5A8;
    int m5AC;                         // inferred: the settings of message 0x0F arrived
    unsigned char m05B0[0x5B8 - 0x5B0];
    long long mStartTime;             // 0x5B8: inferred: OSGetTime of the start of the game
    unsigned char m05C0[0x5E4 - 0x5C0];
    PPPlayerArray_unk mPlayers;       // 0x5E4
    unsigned char m05EC[0x5F0 - 0x5EC];
    PPCharaEntry_unk* mCharas;        // 0x5F0
    int mCharaNum;                    // 0x5F4
    unsigned char m05F8[0x2B74 - 0x5F8];
    atn::SoundRef m2B74;              // inferred: sounds stopped by fn_804DA398
    atn::SoundRef m2B78;
    unsigned char m2B7C[0x2B88 - 0x2B7C];
    int m2B88;                        // inferred: number of the slots of m2B8C
    PPPlayer_unk m2B8C[8];
    unsigned char m966C[0x9694 - 0x966C];
    int m9694[8];                     // inferred: per aid values of message 0x35
    unsigned char m96B4[0x976C - 0x96B4];
    int m976C;                        // inferred: set by fn_804FB530
    unsigned char m9770[0x9984 - 0x9770];
    unsigned char m9984[8];           // inferred: two 8-byte blocks of message 0x28
    unsigned char m998C[8];
    unsigned char m9994[8];           // inferred: values of the aids of m9984 (fn_804DD1A8)
    unsigned char m999C[0x99A0 - 0x999C];
    int m99A0;                        // inferred: count down shown by the lobby
    unsigned char m99A4[0x99B0 - 0x99A4];
    unsigned char m99B0;              // inferred: a packet must be sent back after the loop (to m99B1)
    unsigned char m99B1;
    unsigned char m99B2[0x99B4 - 0x99B2];
    int m99B4;
    unsigned char m99B8[0x99F4 - 0x99B8];
    int m99F4[8];                     // inferred: per team flags of message 0x18

    int m9A14[8];                     // inferred: per aid flags set by messages 0x38 and 0x3B
    int m9A34[8];
};

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

extern "C" unsigned char DWC_GetMyAID();
extern "C" int fn_806A8E40();
// inferred: atn::net::NetPlayerInfo_unk (0x34 bytes, include/atn/net/Manager.h)
struct PPNetPlayer_unk {
    unsigned char m00[0x34];
};
// inferred: atn::net::Manager (lbl_8087F628), only the members used here (see include/atn/net/Manager.h);
// names invented
struct PPNet_unk {
    inline atn::net::PacketPack* GetPack_unk();
    // inferred: own aid (as atn::net::Manager::GetMyAid_unk)
    unsigned char GetMyAid_unk() {
        if (mC215 == 0) {
            return mDisabled ? 0 : DWC_GetMyAID();
        } else {
            return mDisabled ? 0 : fn_806A8E40();
        }
    }

    unsigned char m000[0xC8];
    int mState;                       // 0xC8
    unsigned char m0CC[0x1F8 - 0xCC];
    unsigned int m1F8_31_20 : 12;
    unsigned int mDisabled : 1;       // inferred: network disabled
    unsigned int m1F8_18_0 : 19;
    unsigned char m1FC[0x270 - 0x1FC];
    unsigned int mPlayerNum;          // 0x270: inferred: atn::net::Manager::mPlayers (FixedArray_unk)
    PPNetPlayer_unk mPlayers[8];      // 0x274
    unsigned char m414[0xC2C - 0x414];
    unsigned int mC2C;  // inferred: bit 0 and bit 1 set by message 0x0D
    unsigned char m0C30[0xC215 - 0xC30];
    unsigned char mC215; // inferred: messages are not checked for an old sequence number
    unsigned char mC216[0xC218 - 0xC216];
    int mC218;
};

// inferred: the packer of the network manager
inline atn::net::PacketPack* PPNet_unk::GetPack_unk() {
    return fn_804AE3BC();
}

// inferred: weak copies used by the receive loop (fn_804EE040) and by fn_804D25D0
extern "C" inline PPNet_unk* fn_804D1694() {
    return (PPNet_unk*)lbl_8087F628;
}
extern "C" inline PPPlayer_unk* fn_804D5F0C(PPManager2_unk* mgr, int index) {
    if (index >= 0 && index < mgr->mPlayers.mNum) {
        return &mgr->mPlayers.mData[index];
    }
    return 0;
}
extern "C" inline int fn_804D5F40(PPPlayerArray_unk* players) {
    return players->mNum;
}
extern "C" inline PPPlayer_unk* fn_804D5F48(PPPlayerArray_unk* players, int index) {
    return &players->mData[index];
}
extern "C" inline unsigned char fn_804D6348(PPNet_unk* net) {
    return net->mC215;
}

// inferred: the slot holds a player of a team (team in the high nibble of the aid byte)
static inline int IsTeamPlayer_unk(PPPlayer_unk* player) {
    if (player != 0 && player->mAid != 0xFF && (player->mAid & 0xF0) != 0) {
        return 1;
    }
    return 0;
}

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach); // inferred: stops the sound

// inferred: counts down m99A0 (shown by the lobby); 1 when it ends (or ended) and `keep` is 0
extern "C" int fn_804DA2D0(PPManager2_unk* mgr, int keep) {
    if (--mgr->m99A0 < 0) {
        mgr->m99A0 = 0;
        if (keep == 0) {
            return 1;
        }
    }
    if (MP::D2AMpMenuLobby::sInstance != 0) {
        MP::D2AMpMenuLobby::sInstance->fn_804C2260(mgr->m99A0);
    }
    return mgr->m99A0 == 0;
}

// inferred: stops the voice and the sound of the count down (not in game mode 2)
extern "C" void fn_804DA398(PPManager2_unk* mgr) {
    if (mgr->mMode != 2) {
        if (lbl_8087F498 != 0) {
            lbl_8087F498->fn_803EBAC8(15, 0);
        }
        fn_800CB5C8(&mgr->m2B74, 15, 0);
    }
}

// inferred: seconds since the start of the game
extern "C" int fn_804DB3B4(PPManager2_unk* mgr) {
    return (OSGetTime() - mgr->mStartTime) / (*(unsigned long*)0x800000F8 / 4);
}

// NONMATCHING: 92.59% - the original loads m564 before saving r31
// inferred: seconds left of the time limit (-1 without one)
extern "C" int fn_804DB408(PPManager2_unk* mgr) {
    if (mgr->m564 >= 0) {
        return mgr->m564 - fn_804DB3B4(mgr);
    }
    return -1;
}

// inferred: the player slot of network id `aid` (slots in use only)
extern "C" PPPlayer_unk* fn_804DD158(PPManager2_unk* mgr, unsigned int aid) {
    for (int i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers.mData[i];
        if (player->mFlags.mActive == 1 && aid == player->mAid) {
            return player;
        }
    }
    return 0;
}

// NONMATCHING: 99.35% - r6/r7 of the inlined search (as fn_804F3880)
// inferred: team of network id `aid` (1 or 2); in game mode 1 with the local play the table m9984/m9994 decides
extern "C" int fn_804DD1A8(PPManager2_unk* mgr, int aid) {
    PPPlayer_unk* player = fn_804DD158(mgr, aid & 0xF);
    if (mgr->mMode == 1) {
        PPNet_unk* net = (PPNet_unk*)lbl_8087F628;
        if (net->mC215 == 1 && net->mC218 == 1) {
            for (int i = 0; i < 8; i++) {
                if (aid == mgr->m9984[i]) {
                    return mgr->m9994[i];
                }
            }
        } else {
            int team = 1;
            if (player->mFlags.m22 == 1) {
                team = 2;
            }
            return team;
        }
    }
    return 2;
}

// inferred: sets the 4-bit value of the player slot; 0 if it does not change
extern "C" unsigned int fn_804DD9CC(PPManager2_unk* mgr, PPPlayer_unk* player, unsigned int value) {
    if (player == 0) {
        return 0;
    }
    if (player->mFlags.m22 == value) {
        return 0;
    }
    player->mFlags.m22 = value;
    return value;
}

// inferred: state of the manager and the value set when a player slot is added (weak copies)
extern "C" inline int fn_804EE030(PPManager2_unk* mgr) {
    return mgr->mState;
}
extern "C" inline void fn_804EE038(PPManager2_unk* mgr, short value) {
    mgr->m50A = value;
}

extern "C" int fn_804E4B44(PPManager2_unk* mgr, PPPlayer_unk* player);

// inferred: fn_804E4B44 on the last slot of m2B8C (-1 when there is none)
extern "C" int fn_804E4C34(PPManager2_unk* mgr) {
    if (mgr->m2B88 != 0) {
        return fn_804E4B44(mgr, &mgr->m2B8C[mgr->m2B88 - 1]);
    }
    return -1;
}

// inferred: number of the message for a kind of player (names invented)
extern "C" int fn_804EA5DC(PPManager2_unk* mgr, int kind) {
    switch (kind) {
    case 0:
    case 1:
        return 9;
    case 2:
        return 5;
    default:
        return 10;
    }
}

// NONMATCHING (and fn_804EB26C, fn_804EB400, fn_804EB5D8): 91-98% - the slot offset and the slot pointer of the
// loops over the player slots take each other's registers (fn_804DD158 has them right)
// inferred: number of the player slots in use
extern "C" int fn_804EAFD0(PPManager2_unk* mgr) {
    int n = 0;
    for (int i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers.mData[i];
        if (player->mFlags.mActive) {
            n++;
        }
    }
    return n;
}

// inferred: every player in use has m28 set; the number of them in `count`
extern "C" int fn_804EB26C(PPManager2_unk* mgr, int* count) {
    int all = 1;
    int n = 0;
    for (int i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers.mData[i];
        if (player->mFlags.mActive == 1) {
            if (!player->mFlags.m28) {
                all = 0;
            }
            if (player->mFlags.m28) {
                n++;
            }
        }
    }
    if (count != 0) {
        *count = n;
    }
    return all;
}

// inferred: the slot `index` holds a player of a team
extern "C" int fn_804EB3CC(PPManager2_unk* mgr, int index) {
    return IsTeamPlayer_unk(&mgr->mPlayers.mData[index]);
}

// inferred: the player slot (in use) of a character
static inline PPPlayer_unk* FindByChara_unk(PPManager2_unk* mgr, CharaTask* chara) {
    for (int i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers.mData[i];
        if (player->mFlags.mActive == 1 && player->mRef.mChara == chara) {
            return player;
        }
    }
    return 0;
}

// inferred: the character is a player of a team
extern "C" int fn_804EB400(PPManager2_unk* mgr, CharaTask* chara) {
    PPPlayer_unk* player = FindByChara_unk(mgr, chara);
    if (player != 0) {
        return IsTeamPlayer_unk(player);
    }
    return 0;
}

extern "C" int fn_804EB480(PPManager2_unk* mgr, PPPlayer_unk* player) {
    return IsTeamPlayer_unk(player);
}

// inferred: every player in use that is not in a team has m27 set
extern "C" bool fn_804EB5D8(PPManager2_unk* mgr) {
    for (int i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers.mData[i];
        if (IsTeamPlayer_unk(player) != 1 && player->mFlags.mActive == 1 && !player->mFlags.m27) {
            return false;
        }
    }
    return true;
}

// inferred: the player slot of a character
extern "C" PPPlayer_unk* fn_804EB7C4(PPManager2_unk* mgr, CharaTask* chara) {
    for (int i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers.mData[i];
        if (player->mFlags.mActive == 1 && player->mRef.mChara == chara) {
            return player;
        }
    }
    return 0;
}

// inferred: index of a character in the list m5F0 (-1 when it is not there)
static inline int FindCharaIndex_unk(PPManager2_unk* mgr, CharaTask* chara) {
    for (int i = 0; i < mgr->mCharaNum; i++) {
        if (mgr->mCharas[i].mRef.mChara == chara) {
            return i;
        }
    }
    return -1;
}

// NONMATCHING: 97.74% - the index and the entry offset of the loop take each other's registers
// inferred: the entry of the character list of a character
extern "C" PPCharaEntry_unk* fn_804EB814(PPManager2_unk* mgr, CharaTask* chara) {
    int index = FindCharaIndex_unk(mgr, chara);
    if (index >= 0) {
        return &mgr->mCharas[index];
    }
    return 0;
}

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
        CharaTask* chara = player->mRef.mChara;
        if (chara != 0) {
            fn_804CF198((PackedMotion_unk*)((unsigned char*)data + 0x2C), chara->GetModel_unk(), 0);
            ((PPChara_unk*)player->mRef.mChara)->m3FC = 1;
        }
    }
}

// inferred: message 9: a voice of the data (id, offset)
extern "C" void fn_804EFE98(PPPlayer_unk* player) {
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

// NONMATCHING: 99.26% - r3/r4/r5 of the network manager, its flags and bit 0 rotated
// inferred: message 0x0D: value m564; in game mode 2 it plays a BGM and sets the flags of the network
// manager (bit 0, then bit 1) and a flag of the game (0x1B0)
extern "C" void fn_804F1124() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    int* data = (int*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m564 = data[0];
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30 && ((PPManager2_unk*)lbl_8087F610)->mMode == 2) {
        if (GameBgm::sInstance != 0) {
            GameBgm::sInstance->fn_8037D4C0(0x53D, 15, 1, 0, 0.7f);
        }
        PPNet_unk* net = (PPNet_unk*)lbl_8087F628;
        unsigned int flags = net->mC2C;
        if ((flags & 1) == 1 && (flags & 2) == 2) {
            return;
        }
        if ((flags & 1) != 1) {
            net->mC2C |= 1;
        } else if ((flags & 2) != 2) {
            net->mC2C |= 2;
        }
        if (lbl_8087F430 != 0) {
            SetConfig::sInstance->mShowTutorial = 1;
            ((GameTask*)lbl_8087F430)->fn_803750E4(0x1B0);
            SetConfig::sInstance->mShowTutorial = 0;
        }
    }
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

// inferred: message 0x16: state of the player
extern "C" void fn_804F1D70(PPPlayer_unk* player) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    player->m34 = *(PPPlayerState_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
}

extern "C" void fn_804F1F24(PPPlayer_unk* player) {
    player->mFlags.m27 = 1;
}

// NONMATCHING: 99.67% - the original increments the index before the slot offset
// inferred: `player` is in the team of `leader`
static inline bool IsSameTeam_unk(PPPlayer_unk* player, PPPlayer_unk* leader) {
    if (leader == 0 || player == 0 || !IsTeamPlayer_unk(player)) {
        return false;
    }
    return (player->mAid & 0xF) == leader->mAid;
}

// inferred: message 0x18: marks the player and the players of its team (m26) and the team flag
extern "C" void fn_804F1F34(PPPlayer_unk* player) {
    int i;
    PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
    player->mFlags.m26 = 1;
    if (player->mRef.mChara != 0) {
        player->mRef.mChara->fn_800D246C(0);
    }
    for (i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* other = &mgr->mPlayers.mData[i];
        if (IsTeamPlayer_unk(other) && (unsigned int)(other->mAid & 0xF) == player->mAid) {
            other->mFlags.m26 = 1;
            if (other->mRef.mChara != 0) {
                other->mRef.mChara->fn_800D246C(0);
            }
        }
    }
    PPManager2_unk* mgr2 = (PPManager2_unk*)lbl_8087F610;
    mgr2->m99F4[player->mAid & 0xF] = 1;
}

// inferred: message 0x1F: shield parts of the character of the player (count in the data)
extern "C" void fn_804F2C44(PPPlayer_unk* player) {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    int* data = (int*)fn_804AE3BC()->GetWorkBuffer_unk();
    if (player->mRef.mChara != 0) {
        if (*data > 0) {
            player->mRef.mChara->fn_8014F698(0, *data, 0);
        } else {
            player->mRef.mChara->fn_8014F960(0);
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

// NONMATCHING: 97.18% (and 99.51% fn_804F39D4) - in the inlined fn_804DD158 the manager takes r5 and the
// slot offset r3 (swapped)
// inferred: message 0x2E: hit (or end of the hit) of skill 0x4FC1 on the character of a player
extern "C" void fn_804F3880() {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    unsigned char* data = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    PPPlayer_unk* player = fn_804DD158((PPManager2_unk*)lbl_8087F610, data[0]);
    if (player != 0 && player->mRef.mChara != 0) {
        if (data[1] != 0) {
            player->mD4C |= 0xC00;
            CharaTask* chara = player->mRef.mChara;
            if (chara != 0) {
                player->mRef.mChara->fn_80178668(fn_80219E6C(0x4FC1), 0, 9999, chara, 0);
            }
        } else {
            player->mD4C &= ~0xC00;
            CharaTask* chara = player->mRef.mChara;
            if (chara != 0) {
                player->mRef.mChara->fn_80178864(fn_80219E6C(0x4FC1), chara);
            }
        }
    }
}

// inferred: message 0x2F: value m9F8 and flag 0x20 of the character of a player (or releases it)
extern "C" void fn_804F39D4() {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    unsigned char* data = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    PPPlayer_unk* player = fn_804DD158((PPManager2_unk*)lbl_8087F610, data[0]);
    if (player == 0) {
        return;
    }
    if (data[1] != 0) {
        if (player->mRef.mChara != 0) {
            ((PPChara_unk*)player->mRef.mChara)->m9F8 = data[2];
            ((PPChara_unk*)player->mRef.mChara)->m7E0 |= 0x20;
        }
    } else if (player->mRef.mChara != 0) {
        player->mRef.fn_805011CC();
        ((PPChara_unk*)player->mRef.mChara)->m7E0 &= ~0x20;
    }
}

// inferred: message 0x30: a byte kept by the manager (m5A8)
extern "C" void fn_804F3AF0() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned char* data = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m5A8 = data[0];
}

extern "C" void fn_804EAA84(PPManager2_unk* mgr, CharaTask* chara, int a, int b);

// inferred: player slot of the own aid
static inline PPPlayer_unk* GetMyPlayer_unk() {
    PPNet_unk* net = (PPNet_unk*)lbl_8087F628;
    PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
    return fn_804DD158(mgr, net->GetMyAid_unk());
}

// NONMATCHING: 96.78% - here the manager pointer is kept in r29 across the loop (the original loads it again
// in every iteration)
// inferred: message 0x31: fn_804EAA84 on the characters of the players marked m30 and of the own team
extern "C" void fn_804F3BAC() {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    for (int i = 0; i < ((PPManager2_unk*)lbl_8087F610)->mPlayers.mNum; i++) {
        PPPlayer_unk* player = fn_804D5F0C((PPManager2_unk*)lbl_8087F610, i);
        if (player->mFlags.mActive && player->mRef.mChara != 0) {
            if (player->mFlags.m30 == 1 || IsSameTeam_unk(player, GetMyPlayer_unk())) {
                fn_804EAA84((PPManager2_unk*)lbl_8087F610, player->mRef.mChara, 1, 0);
            }
        }
    }
}

extern "C" void fn_804F50DC(PPManager2_unk* mgr, PPPlayer_unk* player, unsigned short a, unsigned short b);

// inferred: message 0x32: two halfwords handled by the manager (fn_804F50DC)
extern "C" void fn_804F3D80(PPPlayer_unk* player) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned short* data = (unsigned short*)fn_804AE3BC()->GetWorkBuffer_unk();
    fn_804F50DC((PPManager2_unk*)lbl_8087F610, player, data[0], data[1]);
}

// inferred: message 0x33: releases the character of entry `index` of the list
extern "C" void fn_804F3E50() {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    int* data = (int*)fn_804AE3BC()->GetWorkBuffer_unk();
    int index = data[0];
    if (index < 0 || ((PPManager2_unk*)lbl_8087F610)->mCharaNum <= index) {
        return;
    }
    PPCharaEntry_unk* entry = &((PPManager2_unk*)lbl_8087F610)->mCharas[index];
    if (entry->mRef.mChara != 0) {
        entry->mRef.fn_805011CC();
    }
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
extern "C" void fn_804F4E1C(unsigned int aid) {
    if (aid < 8) {
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        mgr->m9A14[(unsigned char)aid] = 1;
    }
}

extern "C" void fn_804F4F40(unsigned int aid) {
    if (aid < 8) {
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        mgr->m9A34[(unsigned char)aid] = 1;
    }
}

// inferred: message 0x3E: the lobby is told to refresh (mF90)
extern "C" void fn_804F50C4(unsigned int aid) {
    MP::D2AMpMenuLobby* lobby = MP::D2AMpMenuLobby::sInstance;
    if (lobby != 0) {
        lobby->mF90 = 1;
    }
}

// inferred: error of the save/load task of the manager
extern "C" int fn_804F7F34() {
    MP::SaveLoad* saveLoad = ((PPManager2_unk*)lbl_8087F610)->mSaveLoad;
    if (saveLoad != 0) {
        return saveLoad->mError;
    }
    return 0;
}

// inferred: flag m976C once the network manager reached state 8
extern "C" void fn_804FB530(PPManager2_unk* mgr) {
    if (((PPNet_unk*)lbl_8087F628)->mState < 8) {
        return;
    }
    mgr->m976C = 1;
}

// inferred: number of the players of a game mode (-1: the current one)
extern "C" int fn_804FB620(void* unused, int mode) {
    if (mode == -1) {
        mode = lbl_8087F610 != 0 ? ((PPManager2_unk*)lbl_8087F610)->mMode : 0;
    }
    int n;
    if (mode == 2) {
        n = 7;
    } else {
        n = mode == 1 ? 8 : 6;
    }
    return n - 2;
}

// inferred: info of the player of network id `aid` (two copies of the same accessor)
extern "C" PPNetPlayer_unk* fn_804FC240(PPNet_unk* net, unsigned int aid) {
    unsigned int i = aid & 0xF;
    if (i >= net->mPlayerNum) {
        return 0;
    }
    return &net->mPlayers[i];
}

extern "C" PPNetPlayer_unk* fn_804FC268(PPNet_unk* net, unsigned int aid) {
    unsigned int i = aid & 0xF;
    if (i >= net->mPlayerNum) {
        return 0;
    }
    return &net->mPlayers[i];
}

// inferred: the character finished loading
extern "C" bool fn_804FC580(CharaTask* chara) {
    return chara->fn_8013655C() == 0;
}
