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
    out->mFrame = (float)slot.mFrame * 63.0f;
    out->mSpeed = (float)slot.mSpeed * 12.7f;
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
    CharaTask* mD1C; // inferred: target of the character
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
struct PPManager2_unk;
extern "C" unsigned int fn_804EAEE0(PPManager2_unk* mgr, CharaTask* chara);

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
    unsigned short id = fn_804EAEE0((PPManager2_unk*)lbl_8087F610, chara->mD1C) >> 16;
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
// NONMATCHING: stub of the 0xA50-byte function that applies a received character state (noinline so that
// its callers keep the call while it is empty)
extern "C" __declspec(noinline) void fn_804CF640(void* in, void* chara) {}

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
#include "atn/net/Manager.h"
#include "atn/Camera_unk.h"
#include "game/GameScene.h"
#include "game/MapTask.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/EffectManager.h"
#include "atn/Array_unk.h"
#include "game/PlayerManager.h"
#include "game/GameCamera.h"
#include "game/GameSaveData.h"
#include "game/BattleCharaTask.h"
#include "game/CharaStatus.h"
#include "MP/D2ACommonDialog.h"
#include "MP/Popup.h"
#include "MP/D2AMpMenuSetRule.h"
#include "MP/D2AMpMenuMessageList.h"
#include "MP/D2AMpMenuFriendRoom.h"

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

// inferred: block of a player slot (0x314 bytes, four of them from 0xE8; -1 at 0x00 when unused); names invented
struct PPEquip_unk {
    int m00;
    unsigned short mOn : 1;   // inferred: tested by the state 19 of vf10
    unsigned short m04_14 : 1;
    unsigned short mLevel : 8;
    unsigned short m04_0 : 6;
    unsigned char m006[0x314 - 0x6];
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
    unsigned int m12 : 10;
    unsigned int m10 : 2;
    unsigned int m06 : 4;
    unsigned int m02 : 4;
    unsigned int m01 : 1;             // inferred: cleared on the server when the match ends (fn_804FB220)
    unsigned int m00 : 1;
};

// inferred: player slot of MP::Manager2 (0xD5C bytes, array at +0x5E4); names invented
struct PPPlayer_unk {
    MpCharaRef_unk mRef;              // 0x00: inferred: the character of the player
    unsigned char m004[4];            // inferred: copy of the character information (fn_804D8078)
    void* m08;
    void* m0C;
    unsigned char m010[0x34 - 0x10];
    PPPlayerState_unk m34;            // inferred: state sent by message 0x16
    int mB0;
    unsigned char m0B4[0xCC - 0xB4];
    unsigned char mAid;               // 0xCC: inferred: network id of the player
    unsigned char m0CD[0xD0 - 0xCD];
    union {
        PPPlayerFlags_unk mFlags;     // 0xD0
        unsigned int mFlagWord;       // inferred: the flags as a word (cleared at once)
    };
    int mD4;
    int mD8;
    MpCharaRef_unk mDC;               // inferred: a second character reference
    unsigned char m0E0[0xE8 - 0xE0];
    PPEquip_unk mEquips[4];           // 0xE8

    int mD38[5];                      // inferred: values of message 0x15
    unsigned int mD4C;                // inferred: bits 10-11 set by message 0x2E
    short mD50;
    unsigned char m0D52;
    unsigned char mD53;
    int mD54;
    int mD58;                         // inferred: score (players of equal score share a rank)

};

// inferred: array of the player slots of MP::Manager2 (pointer and count); names invented
struct PPPlayerArray_unk {
    PPPlayer_unk* mData;
    int mNum;
};

// inferred: entry of the character list of MP::Manager2 (0xB4 bytes, array at +0x5F0); names invented
struct PPCharaEntry_unk {
    MpCharaRef_unk mRef;
    unsigned char m04[0xB0 - 0x4];
    unsigned int mActive : 1; // 0xB0: inferred: the entry is used (fn_804E7AC0)
    unsigned int mB0_0 : 31;
};

// inferred: array of the character list of MP::Manager2 (pointer and count); names invented
struct PPCharaArray_unk {
    PPCharaEntry_unk* mData;
    int mNum;
};

// inferred: state variable of MP::Manager2 (value and last value set); names invented
struct PPStateVar_unk {
    int mValue;
    int mLast;
};

// inferred: node of the list of MP::Manager2 at 0x99A4 (MSL list); names invented
struct PPListNode_unk {
    PPListNode_unk* mPrev;
    PPListNode_unk* mNext;
};
struct PPList_unk {
    int mCount;
    PPListNode_unk mHead;
};
// inferred: request of the network thread (0x28 bytes at 0x99C0); names invented
struct PPThreadRequest_unk {
    int m00;
    int m04;
    void* mFuncs[8];
};

// inferred: object of MP::Manager2 at 0x2B7C (only m58 is used); names invented
struct PPLimit_unk {
    unsigned char m00[0x58];
    int m58;
};

// inferred: MP::Manager2 (lbl_8087F610), only the members used here; names invented
struct PPManager2_unk : public atn::Task {
    unsigned char m0048[0x348 - 0x48];
    int m348;                         // inferred: room of the next game (fn_804D9BF4)
    int m34C;
    int m350;
    unsigned char m0354[0x4F8 - 0x354];
    int m4F8;
    union {
        int mState;                   // 0x4FC: inferred: state (30 = in game)
        PPStateVar_unk mStateVar;
    };
    int m504;                         // inferred: reset to -1 when the match ends (fn_804FB220)
    short m508;                       // inferred: reset to 3629 when the match ends
    short m50A;                       // inferred: set to 90 when a player slot is added
    int m50C;                         // inferred: sub state of the game (GameUISession_unk::GetM50C_unk)
    unsigned char m0510[0x514 - 0x510];
    MP::SaveLoad* mSaveLoad;          // 0x514
    unsigned char m0518[0x534 - 0x518];
    int m534;
    unsigned char m0538[0x53C - 0x538];
    int m53C;                         // inferred: the room is open (sent to atn::net::Manager::m0C)
    int mMode;                        // 0x540: inferred: game mode
    int m544;
    int m548;                         // inferred: mode of the last frame
    unsigned char m054C[0x550 - 0x54C];
    int m550;
    unsigned char m0554[0x558 - 0x554];
    int m558;
    int m55C;                         // inferred: values of messages 0x0C and 0x0E
    int m560;                         // inferred: settings of message 0x0F (m560-m598)
    int m564;                         // inferred: also the time limit of the game (seconds)
    int m568;
    unsigned char m56C[0x20];
    unsigned char m58C[0xC];
    int m598;
    int m59C;                         // inferred: aid of the server
    int m5A0;
    int m5A4;
    int m5A8;
    int m5AC;                         // inferred: the settings of message 0x0F arrived
    int m5B0;
    unsigned char m05B4[0x5B8 - 0x5B4];
    long long mStartTime;             // 0x5B8: inferred: OSGetTime of the start of the game
    unsigned char m05C0[0x5CC - 0x5C0];
    int mFrame;                       // 0x5CC: inferred: frame counter of vf10
    int m5D0;
    unsigned char m05D4[0x5E0 - 0x5D4];
    GameTask* mGame;                  // 0x5E0
    PPPlayerArray_unk mPlayers;       // 0x5E4
    unsigned char m05EC[0x5F0 - 0x5EC];
    PPCharaArray_unk mCharas;         // 0x5F0
    unsigned char m05F8[0x5FC - 0x5F8];
    PPPlayer_unk* mOwnPlayer;         // 0x5FC: inferred: used when the character has no slot
    unsigned char m0600[0x604 - 0x600];
    int m604;
    int m608;
    unsigned char m060C[0x610 - 0x60C];
    unsigned char m610[0x69C - 0x610]; // inferred: object given to fn_805076F8
    unsigned char m69C[0x2A50 - 0x69C];
    unsigned char m2A50[0x2B50 - 0x2A50];
    atn::LH_Binary m2B50;             // inferred: resources loaded in state 3
    atn::LH_Binary m2B58;
    atn::LH_Binary m2B60;
    atn::LH_Binary m2B68;
    atn::SoundRef m2B70;
    atn::SoundRef m2B74;              // inferred: sounds stopped by fn_804DA398
    atn::SoundRef m2B78;
    PPLimit_unk* m2B7C;              // inferred: object whose m58 is raised during the load (fn_804D2508)
    int m2B80;
    int m2B84;                        // inferred: result state of the game (fn_804E4A60)
    int m2B88;                        // inferred: number of the slots of m2B8C
    PPPlayer_unk m2B8C[8];
    int m966C;
    unsigned char m9670[0x9674 - 0x9670];
    int m9674;                        // inferred: item won in the game (code, count)
    int m9678;
    int m967C;                        // inferred: value for the players marked m30 (fn_804E4D70)
    unsigned char m9680[0x9694 - 0x9680];
    int m9694[8];                     // inferred: per aid values of message 0x35
    atn::EfpFile m96B4;               // inferred: effects of the game (fn_804DF404)
    atn::EffFile m96C0;
    unsigned char m96CC[0x974C - 0x96CC];
    int m974C;                        // inferred: id given to the ranking requests
    int m9750;
    int m9754;
    int m9758;
    int m975C;                        // inferred: reset to 300 when the match ends (fn_804FB220)
    int m9760;
    unsigned char m9764[0x9768 - 0x9764];
    int m9768;
    int m976C;                        // inferred: set by fn_804FB530
    atn::Task* m9770;                 // inferred: MP::Text task
    atn::Camera m9774;                // inferred: camera of the lobby
    atn::EfpFile m9968;
    atn::Task* m9974;
    unsigned char m9978;
    unsigned char m9979;              // inferred: aid of the server of the last frame
    unsigned char m997A[0x997C - 0x997A];
    int m997C;
    int m9980;
    unsigned char m9984[8];           // inferred: two 8-byte blocks of message 0x28
    unsigned char m998C[8];
    unsigned char m9994[8];           // inferred: values of the aids of m9984 (fn_804DD1A8)
    unsigned char m999C;
    unsigned char m999D;
    unsigned char m999E[0x99A0 - 0x999E];
    int m99A0;                        // inferred: count down shown by the lobby
    PPList_unk m99A4;
    unsigned char m99B0;              // inferred: a packet must be sent back after the loop (to m99B1)
    unsigned char m99B1;
    unsigned char m99B2[0x99B4 - 0x99B2];
    int m99B4;
    int m99B8;                        // inferred: timer
    unsigned char m99BC;
    unsigned char m99BD[0x99C0 - 0x99BD];
    PPThreadRequest_unk m99C0;        // inferred: request sent to the network thread
    unsigned char m99E8;
    unsigned char m99E9[0x99EC - 0x99E9];
    int m99EC;                        // inferred: 60 when the state 11 is entered (fn_804FB220)
    unsigned char m99F0[4];
    int m99F4[8];                     // inferred: per team flags of message 0x18

    int m9A14[8];                     // inferred: per aid flags set by messages 0x38 and 0x3B
    int m9A34[8];
    unsigned char m9A54[0x9A74 - 0x9A54];
    unsigned char m9A74[3];
    unsigned char m9A77;
    long long m9A78;
    unsigned char m9A80;
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
// lbl_8087F628 (atn::net::Manager instance): see atn/net/Manager.h
extern "C" void fn_804FB220(PPStateVar_unk* state, int value);
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
extern "C" inline PPPlayer_unk* fn_804D5F0C(PPManager2_unk* mgr, int index) {
    if (index >= 0 && index < mgr->mPlayers.mNum) {
        return &mgr->mPlayers.mData[index];
    }
    return 0;
}
extern "C" __declspec(noinline) inline int fn_804D5F40(PPPlayerArray_unk* players) {
    return players->mNum;
}
extern "C" __declspec(noinline) inline PPPlayer_unk* fn_804D5F48(PPPlayerArray_unk* players, int index) {
    return &players->mData[index];
}
extern "C" __declspec(noinline) inline unsigned char fn_804D6348(atn::net::Manager* net) {
    return net->mC215;
}

// --- MP::Manager2::vf10 (fn_804D25D0) and the weak copies of its inline accessors -----------------
// inferred: the copies 0x804D5F04-0x804D6680 follow fn_804D25D0 in the .dol (names invented)
#include "atn/LH_Binary.h"
#include "atn/Color.h"

// inferred: MP::Manager2 as seen by game/GameUI.cpp (its weak copy GetM50C_unk is the one called here)
struct GameUISession_unk {
    unsigned char m000[0x50C];
    int m50C;
    int GetM50C_unk() const { return m50C; }
};

extern "C" void OSSleepTicks(long long ticks);

class ErrorManager {
public:
    void fn_80364E7C(int code); // inferred: shows an error
    void fn_8036503C();
    void fn_80365320();
    unsigned char m00[0x40];
    int m40;
};
extern "C" ErrorManager* lbl_8087F420;

namespace atn {
class RenderConfig {
public:
    void SetClearColor(const Color& color);
};
namespace net {
struct NetRankData_unk;
// inferred: ranking of the Wi-Fi Connection (lbl_8087F5A4, atn/NetManager.cpp); only the members used here
class NetRanking_unk {
public:
    int fn_8050C2F4();
    void fn_8050C7F0();
    void fn_8050C1D8(void* friends);
    int fn_8050C840(unsigned int kind);
    void fn_8050CCD0(unsigned long id, long score, const wchar_t* name, unsigned long size);
    void fn_8050C898(unsigned long a, unsigned long b, unsigned long c, void (*cb)(int, NetRankData_unk*));
    void fn_8050CB24(unsigned long a, int b, unsigned long c, void (*cb)(int, NetRankData_unk*));
    void fn_8050C9D4(unsigned long a, int b, unsigned long c, void (*cb)(int, NetRankData_unk*));
    void fn_8050CDA8();
    int mState;
    int m004[(0x750 - 0x4) / 4];
    int m750;
};
} // namespace net
} // namespace atn
extern "C" atn::net::NetRanking_unk* lbl_8087F5A4;

namespace MP {
// inferred: views of MP::D2AMpMenu and MP::D2AMpMenuFriend (their headers redeclare lbl_8087EFA8 and
// lbl_8087F628 with other types)
class D2AMpMenu : public atn::Task {
public:
    struct Work_unk {
        int m00;
        int m04;
        int m08;
        int m0C;
    };
    void fn_804AF520(int state);
    static D2AMpMenu* sInstance;
    unsigned char m0048[0xBC - 0x48];
    int mBC;
    unsigned char m00C0[0xC8 - 0xC0];
    int mC8;
    unsigned char m00CC[0xD8 - 0xCC];
    int mD8;
    unsigned char m00DC[0xE8 - 0xDC];
    int mE8;
    unsigned char m00EC[0x299C - 0xEC];
    Work_unk m299C;
};
class D2AMpMenuFriend {
public:
    void fn_804B9D40();
    static D2AMpMenuFriend* sInstance;
};
class Text {
public:
    static Text* fn_80505DD0(atn::Task* parent);
};
// inferred: views of the menus of the multiplayer mode (only the members used here)
class D2AMpMenuCockpit : public atn::Task {
public:
    void fn_804B6944();
    void fn_804B50C0(int state);
    void fn_804B6204(int time);
    void fn_804B691C();
    void fn_804B67E4(int value);
    static D2AMpMenuCockpit* sInstance;
};
class D2AMpMenuResult : public atn::Task {
public:
    void fn_804C8740();
    void fn_804C5E1C();
    void fn_804C6888(int time);
    int fn_804C8204();
    void fn_804C5E9C(int state);
    void fn_804C7C34(int a, int b, int c);
    static D2AMpMenuResult* sInstance;
    unsigned char m048[0xE4 - 0x48];
    int mE4;
    unsigned char m0E8[0x104 - 0xE8];
    unsigned char m104;
    unsigned char m105[0x1A4 - 0x105];
    unsigned int m1A4_31 : 1;
    unsigned int m1A4_0 : 31;
};
} // namespace MP

struct MpRoom_unk {
    unsigned char m0000[0x6B00];
    int m6B00;
    unsigned char m6B04[0x6B08 - 0x6B04];
    int m6B08;
    int m6B0C;
    int m6B10;
};
// inferred: MP::Manager2 as seen by game/GameTask.cpp (same inline GetRoom_unk, with its asserts)
#define PP_ASSERT_UNK(x) ((void)0)
struct MpArena_unk {
    unsigned char m0000[0x2B80];
    MpRoom_unk mRoom; // 0x2B80
    MpRoom_unk* GetRoom_unk() {
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0); PP_ASSERT_UNK(this != 0);
        return &mRoom;
    }
};
class Field_unk {
public:
    void* fn_8013C504();
};

// inferred: entry of the list returned by fn_804D649C (names invented)
struct PPRoute_unk {
    unsigned char m00[0x24];
    int m24;
    unsigned char m28[0x2C - 0x28];
    int m2C;
};
// inferred: weak copies called by vf10 after fn_804D6688 (names invented)
extern "C" __declspec(noinline) inline PPRoute_unk* fn_804D80FC(atn::SimpleArray<PPRoute_unk>* routes, int index) {
    return &routes->mData[index];
}
extern "C" __declspec(noinline) inline int fn_804D810C(PPCharaArray_unk* charas) {
    return charas->mNum;
}

extern "C" void* lbl_8087F518;
inline void* ResMgr_unk() {
    return lbl_8087F518;
}
extern "C" void* lbl_8087EF70;
inline void* GetPad_unk() {
    return lbl_8087EF70;
}
extern "C" void fn_8046ECDC(void* obj, int arg);
extern "C" int fn_800A555C(void* pad, int port, int button);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" inline atn::EffectManager* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" inline PPManager2_unk* fn_800F7F90() {
    return (PPManager2_unk*)lbl_8087F610;
}
extern "C" void fn_8026607C(void* mgr, void* efp, void* owner, int arg);
extern "C" void fn_800F7FA8(void* mgr, void* owner, int id);
extern "C" atn::RenderConfig* fn_802F0990();
extern "C" atn::Task* fn_8036635C();
extern "C" inline void* fn_80122550(void* field) {
    return (char*)field + 0x6C;
}
extern "C" void* lbl_8087F8A0;
extern "C" inline void* fn_8000D9E8() {
    return lbl_8087F8A0;
}
extern "C" int* fn_800F530C(CharaTask* chara);
extern "C" int fn_80206B14(int kind, int id);
extern "C" inline FixedArray_unk<chr::CharaPart_unk*, 8>* fn_80266FA4(CharaTask* chara) {
    return &chara->mParts;
}
extern "C" inline chr::CharaPart_unk** fn_80266FB4(FixedArray_unk<chr::CharaPart_unk*, 8>* parts, int i) {
    return &parts->mData[i];
}
extern "C" int fn_8020ED84(int kind, int id);
extern "C" void fn_803396FC(CharaStatus* status, int index, const void* pair);
extern "C" chr::CharaData_unk* fn_8020A81C(const char* name);
extern "C" int fn_80244CA0(void* game);
extern "C" inline void fn_8037F688(atn::Camera* camera, float fovy) {
    camera->mFovy = fovy;
}
extern "C" bool fn_800CB58C(atn::SoundRef* sound);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach);
extern "C" void fn_805061C8(void* obj, void* data, int size);
extern "C" void fn_80506D94(void* obj, void* data, int size);
extern "C" void fn_80507310(void* obj, void* data, int size);
extern "C" void fn_804F60E0(int result, atn::net::NetRankData_unk* data);
extern "C" int fn_8050EB2C(MpFriendList_unk* list);
extern "C" void fn_8050EAE8(MpFriendList_unk* list);
extern "C" void fn_8050F8C4(MpFriendList_unk* list, int on);
extern "C" void fn_8050E618(int kind);
extern "C" int fn_804F7EC8(void* data, int size);
extern "C" int fn_804F7EF0(void* data, int size);
extern "C" int fn_804F7F18();
extern "C" int fn_804F7F34();
extern "C" void fn_804EE040();
extern "C" void fn_804ED768();
extern "C" void fn_804EDB0C();
extern "C" void fn_804EDCCC();
extern "C" unsigned char lbl_8087F614;
extern "C" const char* const lbl_80887594;
extern "C" const char* const lbl_80887598;
extern "C" const char* const lbl_8088759C;
extern "C" const char* const lbl_808875A0;
extern "C" void fn_804F5A94(PPManager2_unk* mgr);
extern "C" int fn_804DC684();
extern "C" void fn_804FB470(PPManager2_unk* mgr, int code, int arg);
extern "C" void fn_804DC450(PPManager2_unk* mgr, int mode);
extern "C" void fn_804FAD48(PPManager2_unk* mgr, int value);
extern "C" void fn_804DF11C(PPManager2_unk* mgr, int on);
extern "C" void fn_804FB674(PPManager2_unk* mgr);
extern "C" void fn_804DA34C(PPManager2_unk* mgr);
extern "C" void fn_804DA48C(PPManager2_unk* mgr, int value);
extern "C" void fn_804DA4A0(PPManager2_unk* mgr, int value);
extern "C" int fn_804DB3B4(PPManager2_unk* mgr);
extern "C" void fn_804DC358(PPManager2_unk* mgr);
extern "C" void fn_804DC374(PPManager2_unk* mgr, int mode);
extern "C" int fn_804DE884(PPManager2_unk* mgr);
extern "C" int fn_804DF404(PPManager2_unk* mgr);
extern "C" void fn_804D6688(PPPlayerArray_unk* players, int num);
extern "C" void fn_804DF310(PPManager2_unk* mgr);
extern "C" void fn_804FB668(PPManager2_unk* mgr);
extern "C" PPPlayer_unk* fn_804EB870(PPManager2_unk* mgr);
extern "C" void fn_804FAA7C(PPManager2_unk* mgr, unsigned int flush, PPPlayer_unk* player);
extern "C" void fn_804D824C(PPManager2_unk* mgr, PPPlayer_unk* player, int mode);
extern "C" void fn_804FC0D4(PPManager2_unk* mgr, int arg);
extern "C" void fn_804C2268(MP::D2AMpMenuLobby* lobby, int time);
extern "C" int fn_804EB1AC(PPManager2_unk* mgr);
extern "C" void fn_804E9360(PPManager2_unk* mgr);
extern "C" void fn_804FB550(PPManager2_unk* mgr);
extern "C" void fn_804D8610(PPManager2_unk* mgr);
extern "C" void fn_804D8910(PPManager2_unk* mgr);
extern "C" void fn_804D87B0(PPManager2_unk* mgr);
extern "C" void fn_804D97F0(PPManager2_unk* mgr);
extern "C" void fn_804D90E0(PPManager2_unk* mgr);
extern "C" void fn_804D996C(PPManager2_unk* mgr);
extern "C" void fn_804E679C(PPManager2_unk* mgr);
extern "C" void fn_804E6518(PPManager2_unk* mgr);
extern "C" void fn_804EAA84(PPManager2_unk* mgr, CharaTask* chara, int a, int b);
extern "C" int fn_804EB3CC(PPManager2_unk* mgr, int index);
extern "C" bool fn_804EB5D8(PPManager2_unk* mgr);
extern "C" void fn_804E9C64(PPManager2_unk* mgr);
// inferred: copy of the character information of a player (0x30 bytes at PPPlayer_unk + 4); names invented
struct PPCharaInfo_unk {
    int m[12];
};
extern "C" __declspec(noinline) inline PPCharaInfo_unk* fn_804D8078(PPCharaInfo_unk* dst, const PPCharaInfo_unk* src) {
    *dst = *src;
    return dst;
}
extern "C" void fn_804D00B8(int index, int value);
struct PPRequests_unk;
extern "C" int fn_804F5A98(PPRequests_unk* queue, unsigned char a, int b, unsigned char c);
extern "C" void fn_804F5FCC(PPRequests_unk* queue);
extern "C" int fn_804F5E38(PPManager2_unk* mgr, int id);
extern "C" void fn_804F5C9C(PPManager2_unk* mgr, int id);
extern "C" long long* fn_804F5B08(PPManager2_unk* mgr, int id);
extern "C" int fn_804FB968(PPManager2_unk* mgr, void* player);
extern "C" void fn_804EA8B8(PPManager2_unk* mgr, int arg);
extern "C" void fn_804E448C(PPManager2_unk* mgr);
extern "C" int fn_804D9BF4(PPManager2_unk* mgr, int a, int b, int c);
extern "C" void fn_804E49A4(PPManager2_unk* mgr);
extern "C" void fn_804E5BC4(PPManager2_unk* mgr);
extern "C" void fn_804D1F6C(PPPlayer_unk* player);
extern "C" void fn_804FA948(PPManager2_unk* mgr, int a, int b);
extern "C" void fn_804FAB40(PPManager2_unk* mgr, int a, PPPlayer_unk* player, int b, int c);
extern "C" void fn_804EA608(PPManager2_unk* mgr, int a, int b);
extern "C" int fn_804EAFD0(PPManager2_unk* mgr);
extern "C" int fn_804EAA50(PPManager2_unk* mgr);
extern "C" void fn_804DA398(PPManager2_unk* mgr);
extern "C" int fn_804DA2D0(PPManager2_unk* mgr, int keep);
extern "C" int fn_804DBD08(PPManager2_unk* mgr, int mode);
extern "C" long long fn_804CF194();
extern "C" void fn_804CF190(atn::Task* task);
extern "C" void fn_804D0090(PPFlags_unk* obj, int on);
extern "C" int fn_804CF634(PPChara_unk* chara);
extern "C" void fn_804D0164();
extern "C" int fn_804D0140(int arg);
extern "C" int fn_804D00DC(unsigned short key);

// inferred: messages with a size and an id, as in atn/NetManager.cpp (NetMsgStatic_unk 0x804D8188)
struct NetMsgStatic_unk {
    inline ~NetMsgStatic_unk();
    int m00;
};
inline void __NetMsgStatic_unk() {
    static NetMsgStatic_unk s;
    s.m00 = 0;
}
// inferred: N is the size of the payload on the network (the payload structs are not padded there)
template <class T, int N = sizeof(T)>
struct PPNetMsg_unk {
    PPNetMsg_unk(unsigned short id);
    T* GetData_unk();
    unsigned short mSize;
    unsigned short mId;
    T mData;
};
template <class T, int N>
__declspec(noinline) inline PPNetMsg_unk<T, N>::PPNetMsg_unk(unsigned short id) : mSize(2), mId(id) {
    __NetMsgStatic_unk();
    mSize = N + 2;
}
template <class T, int N>
__declspec(noinline) inline T* PPNetMsg_unk<T, N>::GetData_unk() {
    return &mData;
}

// inferred: the same message inlined by the small senders (the drafts call PPNetMsg_unk out of line, see
// the notes of s12h-20261109)
template <class T, int N = sizeof(T)>
struct NetMsgI_unk {
    NetMsgI_unk(unsigned short id) : mSize(2), mId(id) {
        __NetMsgStatic_unk();
        mSize = N + 2;
    }
    unsigned short mSize;
    unsigned short mId;
    T mData;
};

// inferred: the message header sent by vf10 (names invented)
struct PPMsgHeader_unk {
    PPMsgHeader_unk(unsigned short id);
    unsigned short mType;
    unsigned short mId;
};
__declspec(noinline) inline PPMsgHeader_unk::PPMsgHeader_unk(unsigned short id) : mType(2), mId(id) {}
// inferred: payload of the message 0x1043 (characters of the players and m2B80); names invented
struct PPMsgChars_unk {
    unsigned short mIds[8];
    int m10;
};

extern "C" __declspec(noinline) inline atn::net::Manager* fn_804D1694() {
    return lbl_8087F628;
}
extern "C" __declspec(noinline) inline PPStateVar_unk* fn_804D169C(PPStateVar_unk* state, int value) {
    fn_804FB220(state, value);
    return state;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenu* fn_804D16CC() {
    return MP::D2AMpMenu::sInstance;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenuCockpit* fn_804D16D4() {
    return MP::D2AMpMenuCockpit::sInstance;
}
extern "C" __declspec(noinline) inline MP::D2ACommonDialog* fn_804D16DC() {
    return MP::D2ACommonDialog::sInstance;
}
extern "C" __declspec(noinline) inline atn::net::NetThread_unk* fn_804D16E4(atn::net::Manager* net) {
    return &net->mThread;
}
extern "C" __declspec(noinline) inline ErrorManager* fn_804D16F0() {
    return lbl_8087F420;
}
// inferred: clears a player slot
extern "C" void fn_804D1F6C(PPPlayer_unk* player) {
    player->mAid = 0xFF;
    player->mDC.fn_80501288(0);
    player->mFlagWord = 0;
    player->mB0 = 0;
    player->mD38[4] = 0;
    player->mD50 = -1;
    player->mD4C = 0;
    player->mD53 = 0;
    player->mD54 = -1;
    player->mD58 = 0;
    player->mD8 = 0;
    player->mD4 = 0;
}

extern "C" __declspec(noinline) inline int fn_804D24A8(atn::net::NetThread_unk* thread) {
    return thread->mRunning;
}
extern "C" __declspec(noinline) inline int fn_804D5F04(PPStateVar_unk* state) {
    return state->mValue;
}
extern "C" __declspec(noinline) inline int fn_804D5F38(PPManager2_unk* mgr) {
    return mgr->mPlayers.mNum;
}
extern "C" __declspec(noinline) inline void fn_804D5F58(CharaTask* chara, int on) {
    chara->mFlag15 = on;
}
extern "C" __declspec(noinline) inline void fn_804D5F68(PPManager2_unk* mgr) {
    mgr->m99B8--;
}
extern "C" __declspec(noinline) inline bool fn_804D5F7C(atn::net::Manager* net) {
    return net->mState >= 8;
}
extern "C" __declspec(noinline) inline int fn_804D5F98(atn::net::Manager* net) {
    return net->mState;
}
extern "C" __declspec(noinline) inline int fn_804D5FA0(atn::net::NetRanking_unk* ranking) {
    return ranking->mState;
}
extern "C" __declspec(noinline) inline int fn_804D5FA8(PPStateVar_unk* state) {
    return state->mValue;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenuFriend* fn_804D5FB0() {
    return MP::D2AMpMenuFriend::sInstance;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenuFriendRoom* fn_804D5FB8() {
    return MP::D2AMpMenuFriendRoom::sInstance;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenuLobby* fn_804D5FC0() {
    return MP::D2AMpMenuLobby::sInstance;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenuMessageList* fn_804D5FC8() {
    return MP::D2AMpMenuMessageList::sInstance;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenuResult* fn_804D5FD0() {
    return MP::D2AMpMenuResult::sInstance;
}
extern "C" __declspec(noinline) inline MP::D2AMpMenuSetRule* fn_804D5FD8() {
    return MP::D2AMpMenuSetRule::sInstance;
}
extern "C" __declspec(noinline) inline int fn_804D5FE0(atn::net::Manager* net) {
    return net->mBECC;
}
extern "C" __declspec(noinline) inline void fn_804D5FEC(atn::net::Manager* net) {
    net->mErrors.clear();
}
extern "C" __declspec(noinline) inline bool fn_804D5FFC(MP::D2ACommonDialog* dialog) {
    return dialog->m4C == 0;
}
extern "C" __declspec(noinline) inline void fn_804D600C(PPManager2_unk* mgr, int value) {
    mgr->m50C = value;
}
extern "C" __declspec(noinline) inline void fn_804D6014(MP::D2AMpMenu* menu) {
    menu->mD8 = 1;
}
extern "C" __declspec(noinline) inline MpFriendList_unk* fn_804D6024(atn::net::Manager* net) {
    return &net->mFriendList;
}
extern "C" __declspec(noinline) inline void* fn_804D6020(MpFriendList_unk* list) {
    return list;
}
extern "C" __declspec(noinline) inline void fn_804D602C(PPManager2_unk* mgr, int value) {
    fn_804FB220(&mgr->mStateVar, value);
}
extern "C" __declspec(noinline) inline int fn_804D6034(ErrorManager* error) {
    return error->m40;
}
extern "C" __declspec(noinline) inline void fn_804D603C(PPManager2_unk* mgr, unsigned char value) {
    mgr->m99BC = value;
}
extern "C" __declspec(noinline) inline void* fn_804D6048(MpFriendList_unk* list) {
    return list->m7F0;
}
extern "C" __declspec(noinline) inline void fn_804D6050(MpFriendList_unk* list, int value) {
    list->mA00 = value;
}
extern "C" __declspec(noinline) inline void fn_804D6058(PPManager2_unk* mgr) {
    PPListNode_unk* first = mgr->m99A4.mHead.mNext;
    PPListNode_unk* end = &mgr->m99A4.mHead;
    PPList_unk* list = &mgr->m99A4;
    if (first != end) {
        PPListNode_unk* last = end->mPrev;
        first->mPrev->mNext = last->mNext;
        last->mNext->mPrev = first->mPrev;
        while (first != end) {
            PPListNode_unk* node = first;
            first = first->mNext;
            delete node;
            list->mCount--;
        }
    }
}
extern "C" __declspec(noinline) inline wchar_t* fn_804D60E4(MpFriendList_unk* list) {
    return list->mName;
}
extern "C" __declspec(noinline) inline void fn_804D60EC(MP::D2AMpMenu* menu, int value) {
    menu->m299C.m00 = 1;
    menu->m299C.m08 = value;
    menu->m299C.m04 = 0;
    menu->m299C.m0C = 0;
}
extern "C" __declspec(noinline) inline MP::SaveData_unk* fn_804D6108(MP::SaveLoad* save) {
    return save->m48;
}
// inferred: SaveDataSubA_unk with the mask of the valid entries after them
struct PPSaveSlots_unk {
    unsigned char m000[0xFC0];
    unsigned int mFC0;
};
extern "C" __declspec(noinline) inline PPSaveSlots_unk* fn_804D613C(MP::SaveData_unk* data) {
    return (PPSaveSlots_unk*)&data->mSubA;
}
struct PPSaveSlot_unk {
    unsigned char m00[0xA0];
    int mA0;
};
extern "C" __declspec(noinline) inline PPSaveSlot_unk* fn_804D6110(PPSaveSlots_unk* slots, int index) {
    unsigned int bit = 1 << index;
    if ((slots->mFC0 & bit) == bit) {
        return (PPSaveSlot_unk*)((char*)slots + index * 0x240);
    }
    return 0;
}
extern "C" __declspec(noinline) inline int fn_804D6144(MP::D2AMpMenu* menu) {
    return menu->mBC;
}
extern "C" __declspec(noinline) inline bool fn_804D614C(MP::D2AMpMenu* menu) {
    return menu->mBC == 20;
}
extern "C" __declspec(noinline) inline void fn_804D6160(atn::net::Manager* net, int value) {
    net->m0C = value;
}
extern "C" __declspec(noinline) inline bool fn_804D6168(atn::net::Manager* net) {
    bool result = false;
    if (net->mFlags.mB6 == 1 && net->mFlags.mB7 == 1) {
        result = true;
    }
    return result;
}
extern "C" __declspec(noinline) inline unsigned char fn_804D6190(atn::net::Manager* net) {
    return net->m90;
}
extern "C" __declspec(noinline) inline unsigned char fn_804D6198(atn::net::Manager* net) {
    if (net->mC215 == 0) {
        return net->mFlags.mB12 ? 0 : DWC_GetMyAID();
    } else {
        return net->mFlags.mB12 ? 0 : fn_806A8E40();
    }
}
extern "C" __declspec(noinline) inline int fn_804D6200(atn::net::Manager* net) {
    return net->m264;
}
extern "C" __declspec(noinline) inline void fn_804D6208(PPManager2_unk* mgr, int value) {
    mgr->m5B0 = value;
}
extern "C" __declspec(noinline) inline int fn_804D6210(atn::net::Manager* net) {
    return net->m25C;
}
extern "C" __declspec(noinline) inline int fn_804D6218(atn::net::NetThread_unk* thread) {
    return thread->mCmd;
}
extern "C" __declspec(noinline) inline unsigned char fn_804D6220(atn::net::Manager* net) {
    return net->mC0;
}
extern "C" __declspec(noinline) inline unsigned char fn_804D6228(atn::net::Manager* net) {
    if (net->mC215 == 0) {
        return net->mFlags.mB12 ? 0 : (unsigned char)fn_806B1250();
    } else {
        return 0;
    }
}
extern "C" __declspec(noinline) inline bool fn_804D6278(atn::net::Manager* net, unsigned char aid) {
    return aid == net->GetServerAid_unk();
}
extern "C" __declspec(noinline) inline bool fn_804D62E4(MP::D2AMpMenuLobby* lobby) {
    return lobby->mD88 == 8;
}
extern "C" __declspec(noinline) inline void fn_804D62F8(MP::D2AMpMenuLobby* lobby, int value) {
    lobby->mDA4 = value;
}
extern "C" int DWC_GetNumConnectionHost();
extern "C" int fn_806A8E70();
extern "C" __declspec(noinline) inline int fn_804D6300(atn::net::Manager* net) {
    if (net->mC215 == 0) {
        return net->mFlags.mB12 ? 1 : DWC_GetNumConnectionHost();
    } else {
        return net->mFlags.mB12 ? 1 : fn_806A8E70();
    }
}
extern "C" __declspec(noinline) inline int fn_804D6354(MP::D2AMpMenuLobby* lobby) {
    return lobby->mD88;
}
extern "C" __declspec(noinline) inline int fn_804D635C(atn::net::Manager* net) {
    return 0;
}
extern "C" __declspec(noinline) inline int fn_804D6364(MP::D2AMpMenuSetRule* rule) {
    return rule->m5C;
}
struct PPLobbyItem_unk {
    unsigned char m00[0x5C];
    int m5C;
};
extern "C" __declspec(noinline) inline void fn_804D636C(PPLobbyItem_unk* item, int value) {
    item->m5C = value;
}
extern "C" __declspec(noinline) inline PPLobbyItem_unk* fn_804D6374(MP::D2AMpMenuLobby* lobby) {
    return (PPLobbyItem_unk*)lobby->m7C;
}
extern "C" __declspec(noinline) inline int fn_804D637C(PPLobbyItem_unk* item) {
    return item->m5C;
}
extern "C" __declspec(noinline) inline void fn_804D6384(PPManager2_unk* mgr, int value) {
    mgr->m99B8 = value;
}
extern "C" __declspec(noinline) inline bool fn_804D6390(PPManager2_unk* mgr) {
    return mgr->m99B8 <= 0;
}
extern "C" __declspec(noinline) inline void fn_804D63A8(CharaTask* chara, int on) {
    if (on) {
        chara->m54C |= 0x10;
    } else {
        chara->m54C &= ~0x10;
    }
}
extern "C" __declspec(noinline) inline int fn_804D63D0(PPManager2_unk* mgr) {
    return mgr->mMode;
}
// inferred: character data of the player (equipment of the multiplayer mode, names invented)
struct PPCharaData_unk {
    unsigned char m000[0xC8];
    int mC8;
    unsigned char m0CC[0xD4 - 0xCC];
    signed char mD4[4];
    int mD8[4];
    unsigned char m0E8[0x108 - 0xE8];
    int m108[5];
};
extern "C" __declspec(noinline) inline PPCharaData_unk* fn_804D63D8(CharaTask* chara) {
    return (PPCharaData_unk*)chara->m5C;
}
struct PPCharaPart_unk {
    unsigned char m000[0x274];
    int m274;
};
extern "C" __declspec(noinline) inline void fn_804D63E0(PPCharaPart_unk* part, int value) {
    part->m274 = value;
}
struct PPEquipObj_unk {
    unsigned char m000[0x414];
    int m414;
};
extern "C" __declspec(noinline) inline void fn_804D63F8(PPEquipObj_unk* equip, int value) {
    equip->m414 = value;
}
extern "C" __declspec(noinline) inline void fn_804D6400(CharaStatus* status, unsigned char value) {
    ((unsigned char*)status)[0x231] = value;
}
extern "C" __declspec(noinline) inline MP::Popup* fn_804D6408() {
    return MP::Popup::sInstance;
}
extern "C" __declspec(noinline) inline int fn_804D6420(PPManager2_unk* mgr) {
    switch ((unsigned char)mgr->m5A8) {
    case 1:
        return 5;
    case 2:
        return 4;
    case 3:
        return 3;
    case 4:
        return 2;
    case 5:
        return 1;
    }
    return 5;
}
extern "C" __declspec(noinline) inline void fn_804D6484(CharaTask* chara, CharaInfo_unk* info) {
    chara->m60 = info;
}
extern "C" __declspec(noinline) inline atn::SimpleArray<PPRoute_unk>* fn_804D649C(void* route) {
    return (atn::SimpleArray<PPRoute_unk>*)((char*)route + 0xF4);
}
extern "C" __declspec(noinline) inline PPCharaEntry_unk* fn_804D64A4(PPCharaArray_unk* charas, int index) {
    return &charas->mData[index];
}
extern "C" __declspec(noinline) inline unsigned char fn_804D64B4(PPPlayer_unk* player) {
    return player->m0D52++;
}
inline int GetRemain_unk(PPManager2_unk* mgr) {
    if (mgr->m564 >= 0) {
        return mgr->m564 - (int)((OSGetTime() - mgr->mStartTime) / (*(unsigned int*)0x800000F8 / 4));
    }
    return -1;
}
extern "C" __declspec(noinline) inline bool fn_804D64C8(PPManager2_unk* mgr) {
    bool on = !(GetRemain_unk(mgr) < 0);
    if (on) {
        on = GetRemain_unk(mgr) < 60;
    }
    return on;
}
extern "C" __declspec(noinline) inline int fn_804D659C(MpCharaRef_unk* ref) {
    return (int)ref->mChara;
}
extern "C" __declspec(noinline) inline void fn_804D65A4(MP::D2AMpMenuResult* result, unsigned char value) {
    result->m104 = value;
}
extern "C" __declspec(noinline) inline bool fn_804D65AC(MP::D2AMpMenuMessageList* list) {
    return list->m94 == 4;
}
extern "C" __declspec(noinline) inline int fn_804D65C0(MP::D2AMpMenuResult* result) {
    return result->mE4;
}
extern "C" __declspec(noinline) inline unsigned int fn_804D65C8(MP::D2AMpMenuResult* result) {
    return result->m1A4_31;
}
extern "C" __declspec(noinline) inline void fn_804D65D4(MP::D2AMpMenu* menu, int value) {
    menu->mE8 = value;
}
extern "C" __declspec(noinline) inline void fn_804D65DC(atn::net::NetRanking_unk* ranking, int state) {
    if (state == 2) {
        ranking->fn_8050CDA8();
    }
    if (state == 2 && ranking->m750 != -1) {
        ranking->m004[ranking->m750] = -1;
    }
    ranking->mState = state;
}
extern "C" __declspec(noinline) inline int fn_804D6644(MP::D2AMpMenu* menu) {
    return menu->mC8;
}
extern "C" __declspec(noinline) inline bool fn_804D664C(PPManager2_unk* mgr) {
    return mgr->m99EC > 0;
}
extern "C" __declspec(noinline) inline int fn_804D6664(GameBgm* bgm) {
    return bgm->m84 != 0 ? bgm->mNext.mId : bgm->mReq.mId;
}
extern "C" __declspec(noinline) inline atn::net::NetRanking_unk* fn_804D6680() {
    return lbl_8087F5A4;
}

extern "C" int fn_800C310C(int arg);
// inferred: MP::Manager2::vf0C (vtable 0x807916E0): waits for the resources, then shows the menu
extern "C" int fn_804D2508(PPManager2_unk* mgr) {
    mgr->m2B7C->m58 = 99999;
    if (fn_800C310C(0) == 0 && mgr->m9968.IsLoading() == 0 && mgr->fn_800D3FA4() != 0) {
        fn_80232B7C((unsigned int)mgr, 0);
        MP::D2AMpMenu::sInstance->fn_800D246C(0);
        MP::D2AMpMenu* menu = MP::D2AMpMenu::sInstance;
        menu->m299C.m00 = 2;
        menu->m299C.m08 = 0;
        menu->m299C.m04 = 0;
        menu->m299C.m0C = 0;
        MP::D2ACommonDialog::sInstance->fn_800D246C(0);
        mgr->m2B7C->m58 = 0;
        return 1;
    }
    return 0;
}

// inferred: MP::Manager2::vf10 (vtable 0x807916E0): update of the multiplayer mode
// NONMATCHING: draft - written for the weak copies of its accessors (0x804D5F04-0x804D6680)
extern "C" void fn_804D25D0(PPManager2_unk* mgr) {
    if (fn_804D5F04(&mgr->mStateVar) == 30 && ((GameUISession_unk*)mgr)->GetM50C_unk() == 0) {
        for (int i = 0; i < fn_804D5F38(mgr); i++) {
            PPPlayer_unk* player = fn_804D5F0C(mgr, i);
            if (player != 0 && player->mFlags.mActive == 1 && player->mRef.mChara != 0) {
                if (player->mRef.mChara->GetFlag1_unk() == 1) {
                    player->mRef.mChara->fn_800D246C(0);
                }
                if (player->mRef.mChara->IsFrozen_unk() == 1) {
                    player->mRef.mChara->fn_80176ACC();
                    fn_804D5F58(player->mRef.mChara, 0);
                    player->mRef.mChara->fn_800D246C(0);
                }
            }
        }
    }
    for (int i = 0; i < fn_804D5F38(mgr); i++) {
        PPPlayer_unk* player = fn_804D5F0C(mgr, i);
        if (player != 0 && player->mFlags.mActive == 1 && player->mRef.mChara != 0) {
            if (player->mRef.mChara->GetStatus_unk()->fn_80139F4C() == 1 || fn_804CF634((PPChara_unk*)player->mRef.mChara) != 0) {
                fn_804D0090((PPFlags_unk*)player->mRef.mChara->GetColli_unk(), 0);
            } else {
                fn_804D0090((PPFlags_unk*)player->mRef.mChara->GetColli_unk(), 1);
            }
        }
    }
    fn_804D5F68(mgr);
    fn_804F5FCC((PPRequests_unk*)mgr);
    fn_804D1694()->fn_805092FC();
    fn_804D6680()->fn_8050C2F4();
    if (fn_804D5F7C(fn_804D1694()) == 1) {
        if (mgr->m9978 == 0 && fn_804DC684() == 1) {
            mgr->m9978 = 1;
        }
        fn_804D1694();
        fn_804AE3BC()->fn_8050DE5C();
        fn_804F5A94(mgr);
    } else {
        mgr->m9978 = 0;
    }
    if (fn_804D5F98(fn_804D1694()) == 3 || fn_804D5FA0(fn_804D6680()) == 2) {
        if (fn_804D5FA8(&mgr->mStateVar) != -1 && fn_804D5FA8(&mgr->mStateVar) != 43 &&
            fn_804D5FA8(&mgr->mStateVar) != -2 && fn_804D5FA8(&mgr->mStateVar) != -3) {
            fn_804FB470(mgr, -2, 22);
        }
    }
    mgr->m4F8 = 0;
    if (fn_804D5F04(&mgr->mStateVar) == 12 && mgr->mSaveLoad != 0) {
        mgr->mSaveLoad->fn_80502848();
    }
    if (mgr->mGame == 0) {
        fn_804D0164();
    }

    switch (fn_804D5F04(&mgr->mStateVar)) {
    case 43: {
        if (mgr->m9750 != 0x8B) {
            fn_804DC450(mgr, 0);
        }
        if (mgr->m9754 >= 0) {
            fn_804FAD48(mgr, mgr->m9754);
        }
        mgr->m9754 = -1;
        fn_804D16CC()->fn_804AF520(22);
        int reset = 0;
        if ((unsigned int)(fn_804D1694()->fn_8050C0B4() - 2) <= 1) {
            reset = 1;
            if (mgr->m9758 != 22) {
                mgr->m9758 = 22;
            }
        }
        if (mgr->m9758 == 22) {
            if (fn_804D16D4() != 0) {
                fn_804D16D4()->fn_804B6944();
            }
            if (fn_804D5FB0() != 0) {
                fn_804D5FB0()->fn_804B9D40();
            }
            if (fn_804D5FB8() != 0) {
                fn_804D5FB8()->fn_804BBB60();
            }
            if (fn_804D5FC0() != 0) {
                fn_804D5FC0()->fn_804C2F98();
            }
            if (fn_804D5FC8() != 0) {
                fn_804D5FC8()->fn_804C5638();
            }
            if (fn_804D5FD0() != 0) {
                fn_804D5FD0()->fn_804C8740();
            }
            if (fn_804D5FD8() != 0) {
                fn_804CF190(fn_804D5FD8());
            }
        }
        if (fn_804D16E4(fn_804D1694()) != 0) {
            fn_804D16E4(fn_804D1694())->fn_8050CF90();
            while (fn_804D24A8(fn_804D16E4(fn_804D1694())) == 0) {
                OSSleepTicks((long long)100 * (*(unsigned int*)0x800000F8 / 4 / 1000));
            }
        }
        fn_804D169C(&mgr->mStateVar, -1);
        int mode = 1;
        if (fn_804D5FE0(fn_804D1694()) == 3) {
            mode = 3;
        }
        fn_804D5FEC(fn_804D1694());
        fn_804D1694()->fn_80509F08();
        fn_804DBD08(mgr, mode);
        if (reset == 0 && (mgr->m9758 != 22 || mgr->m9750 == 0xA1 || mgr->m9750 == 0xA2)) {
            break;
        }
        fn_804D1694()->fn_80509AF8();
        break;
    }
    case -1: {
        int done = 0;
        if (fn_804D5FFC(fn_804D16DC()) == 0) {
            if (fn_804D16DC()->fn_804AC734() != 0) {
                done = 1;
            }
        } else {
            done = 1;
        }
        if (done == 1) {
            fn_804DBD08(mgr, 2);
            fn_804DF11C(mgr, 1);
            if (mgr->m9758 == 22) {
                fn_804D169C(&mgr->mStateVar, -2);
            } else {
                fn_804D169C(&mgr->mStateVar, -3);
            }
            fn_804D600C(mgr, 0);
        }
        break;
    }
    case -3:
        fn_804D169C(&mgr->mStateVar, 10);
        fn_804D16CC()->fn_804AF520(mgr->m9758);
        mgr->m9758 = -1;
        fn_804FB674(mgr);
        break;
    case -2:
        if (fn_804D24A8(fn_804D16E4(fn_804D1694())) == 1) {
            fn_804D169C(&mgr->mStateVar, 7);
            mgr->m9758 = -1;
            if (mgr->m9750 == 0x8B) {
                fn_804D16CC()->fn_804AF520(5);
            } else {
                fn_804D16CC()->fn_804AF520(2);
                fn_804FB674(mgr);
            }
            fn_804D6014(fn_804D16CC());
        }
        break;
    case 0:
        mgr->mSaveLoad = MP::SaveLoad::fn_805020FC(mgr);
        mgr->mSaveLoad->fn_80502708();
        fn_804F7EC8(fn_804D6020(fn_804D6024(fn_804D1694())), 0x9F0);
        fn_804D602C(mgr, 1);
        break;
    case 1:
        if (fn_804F7F18() == 0) {
            if (fn_8050EB2C(fn_804D6024(fn_804D1694())) != 0) {
                if (fn_804D16F0() != 0) {
                    fn_804D16F0()->fn_80364E7C(12);
                }
                fn_804D602C(mgr, 2);
            } else {
                fn_804D602C(mgr, 3);
            }
        }
        break;
    case 2:
        if (fn_804D16F0() != 0) {
            fn_804D16F0()->fn_8036503C();
        }
        if (fn_800A555C(GetPad_unk(), 0, 4) != 0) {
            if (fn_804D6034(fn_804D16F0()) == 0) {
                if (fn_804D16F0() != 0) {
                    fn_804D16F0()->fn_80365320();
                }
                mgr->m9760 = 1;
                fn_800C31F4("SE_UICOM_001", 0, -1, 1.0f);
                fn_804D602C(mgr, 3);
            } else {
                if (fn_804D16F0() != 0) {
                    fn_804D16F0()->fn_80365320();
                }
                fn_800C31F4("SE_UICOM_002", 0, -1, 1.0f);
                fn_800D2338(mgr);
            }
        } else if (fn_800A555C(GetPad_unk(), 0, 5) != 0) {
            if (fn_804D16F0() != 0) {
                fn_804D16F0()->fn_80365320();
            }
            fn_800C31F4("SE_UICOM_002", 0, -1, 1.0f);
            fn_800D2338(mgr);
        }
        break;
    case 3:
        MP::Text::fn_80505DD0(mgr);
        mgr->m9770 = MapTask::SetTask(mgr, "titlle_white.map");
        mgr->m9770->fn_800D246C(1);
        mgr->m2B58.vf0C(lbl_80887598);
        mgr->m2B60.vf0C(lbl_8088759C);
        mgr->m2B68.vf0C(lbl_808875A0);
        mgr->m2B50.vf0C(lbl_80887594);
        mgr->m99C0.m00 = 0;
        if ((fn_804D0140(6) != 0 && fn_804D0140(5) != 0) || fn_804D00DC(0x5A) != 0) {
            mgr->m99C0.m00 = 1;
        }
        if (mgr->m9760 != 0) {
            mgr->m99C0.m00 = 1;
            mgr->m9760 = 0;
        }
        mgr->m99C0.m04 = 0;
        mgr->m99C0.mFuncs[0] = (void*)fn_804EE040;
        mgr->m99C0.mFuncs[1] = (void*)fn_804ED768;
        mgr->m99C0.mFuncs[2] = (void*)fn_804EDB0C;
        mgr->m99C0.mFuncs[3] = (void*)fn_804EDCCC;
        mgr->m99C0.mFuncs[4] = (void*)fn_804F7EC8;
        mgr->m99C0.mFuncs[5] = (void*)fn_804F7EF0;
        mgr->m99C0.mFuncs[6] = (void*)fn_804F7F18;
        mgr->m99C0.mFuncs[7] = (void*)fn_804F7F34;
        fn_804D16E4(fn_804D1694())->Request(1, &mgr->m99C0);
        fn_804D169C(&mgr->mStateVar, 4);
        break;
    case 4:
        fn_8046ECDC(ResMgr_unk(), 0);
        if (fn_804D24A8(fn_804D16E4(fn_804D1694())) == 1) {
            fn_804D169C(&mgr->mStateVar, 5);
            fn_804D603C(mgr, 0);
            fn_804D600C(mgr, 0);
            MpFriendList_unk* list = fn_804D6024(fn_804D1694());
            fn_804D6050(list, ((int*)fn_804D6048(list))[2]);
            fn_804D6058(mgr);
        }
        break;
    case 5: {
        int ready = 1;
        if (mgr->m2B58.fn_80473F50() != 0 || mgr->m2B60.fn_80473F50() != 0 || mgr->m2B68.fn_80473F50() != 0 ||
            mgr->m2B50.fn_80473F50() != 0) {
            ready = 0;
        }
        if (mgr->fn_800D3FA4() == 0) {
            ready = 0;
        }
        if (ready != 0) {
            int size = mgr->m2B60.fn_8047059C();
            fn_805061C8(mgr->m69C, mgr->m2B60.fn_80470580(), size);
            size = mgr->m2B68.fn_8047059C();
            fn_80506D94(mgr->m2A50, mgr->m2B68.fn_80470580(), size);
            size = mgr->m2B50.fn_8047059C();
            fn_80507310(mgr->m610, mgr->m2B50.fn_80470580(), size);
            mgr->m2B58.fn_80473F88();
            mgr->m2B60.fn_80473F88();
            mgr->m2B68.fn_80473F88();
            mgr->m2B50.fn_80473F88();
            fn_804D169C(&mgr->mStateVar, 7);
            if (wcslen(fn_804D60E4(fn_804D6024(fn_804D1694()))) != 0) {
                fn_804D16CC()->fn_804AF520(4);
            } else {
                fn_804D16CC()->fn_804AF520(5);
            }
            fn_804D60EC(fn_804D16CC(), 15);
            mgr->m9770->fn_800D246C(0);
            fn_8026607C(fn_800F7FA0(), &mgr->m9968, 0, 1);
            if (fn_804D6108(mgr->mSaveLoad) != 0) {
                if (fn_804D6110(fn_804D613C(fn_804D6108(mgr->mSaveLoad)), 0) != 0) {
                    mgr->m550 = fn_804D6110(fn_804D613C(fn_804D6108(mgr->mSaveLoad)), 0)->mA0;
                }
                ((GameSaveData_unk*)fn_804D6108(mgr->mSaveLoad))->fn_803BE590();
            }
            if (mgr->m99C0.m00 != 0) {
                fn_8050EAE8(fn_804D6024(fn_804D1694()));
                fn_804F7EF0(fn_804D6020(fn_804D6024(fn_804D1694())), 0x9F0);
            }
            if (GameBgm::GetInstance_unk() != 0) {
                GameBgm::GetInstance_unk()->fn_8037D4C0(0x548, 0, 1, 0, 0.7f);
            }
        }
        fn_8046ECDC(ResMgr_unk(), 0);
        break;
    }
    case 7:
        if (fn_804D6144(fn_804D16CC()) == 7) {
            fn_804D602C(mgr, 8);
        } else if (fn_804D614C(fn_804D16CC())) {
            fn_802F0990()->SetClearColor(atn::Color(0.0f, 0.0f, 0.0f, 1.0f));
            fn_800D2338(mgr);
        }
        break;
    case 8:
        fn_804D1694()->fn_80509A98();
        fn_804D169C(&mgr->mStateVar, 9);
        mgr->m976C = 0;
        break;
    case 9:
        if (fn_804D5F7C(fn_804D1694()) && mgr->m2B58.fn_80473F50() == 0 && mgr->m2B60.fn_80473F50() == 0) {
            fn_804D169C(&mgr->mStateVar, 10);
            mgr->m53C = 0;
            fn_804D6160(fn_804D1694(), 0);
            fn_804D16CC()->fn_804AF520(9);
        }
        break;
    case 10:
        mgr->m53C = 0;
        fn_804D6160(fn_804D1694(), 1);
        fn_804D1694()->fn_8050A634(0);
        break;
    case 11: {
        mgr->m53C = 0;
        fn_804D6160(fn_804D1694(), 1);
        fn_804D1694()->fn_8050A634(0);
        for (int i = 0; i < 3; i++) {
            mgr->m9A74[i] = 0;
        }
        for (int i = 0; i < 8; i++) {
            mgr->m99F4[i] = -1;
        }
        for (int i = 0; i < 8; i++) {
            mgr->m9A14[i] = -1;
        }
        for (int i = 0; i < 8; i++) {
            mgr->m9A34[i] = -1;
        }
        int joined = 0;
        if (fn_804D6168(fn_804D1694())) {
            joined = 1;
        } else if (fn_804D6190(fn_804D1694()) == 1 && fn_804D6198(fn_804D1694()) == 0) {
            joined = 1;
            fn_804D16E4(fn_804D1694())->fn_8050CF90();
        }
        int friendRoom = 0;
        if (joined == 1) {
            joined = 0;
            fn_804D6688(&mgr->mPlayers, fn_804D6200(fn_804D1694()));
            fn_804DF310(mgr);
            fn_804FB668(mgr);
            fn_804D6208(mgr, 0);
            joined = 1;
            unsigned char aid = fn_804D6198(fn_804D1694());
            if (aid >= fn_804D5F40(&mgr->mPlayers)) {
                fn_804FB470(mgr, 0x9E, 9);
            } else {
                fn_804D5F48(&mgr->mPlayers, aid)->mAid = aid;
                fn_804D5F48(&mgr->mPlayers, aid)->mFlags.mActive = 1;
                fn_804D5F48(&mgr->mPlayers, aid)->mFlags.m30 = 1;
                fn_804D5F48(&mgr->mPlayers, aid)->mD53 = 0;
                fn_804D169C(&mgr->mStateVar, 12);
                fn_800F7F90()->m9979 = 0xFF;
                mgr->m544 = 0;
                if (fn_804D6210(fn_804D1694()) == 1) {
                    fn_804DC358(mgr);
                    if (fn_804D6190(fn_804D1694()) == 1) {
                        fn_804D1694()->fn_8050BDC4();
                    }
                    fn_804D16CC()->fn_804AF520(13);
                } else if (fn_804D6144(fn_804D16CC()) == 12) {
                    fn_804D16CC()->fn_804AF520(13);
                }
                mgr->mFrame = 0;
            }
        } else if (--mgr->m99EC <= 0) {
            if ((fn_804D6218(fn_804D16E4(fn_804D1694())) == 8 || fn_804D24A8(fn_804D16E4(fn_804D1694())) == 1) &&
                fn_800A555C(GetPad_unk(), 0, 5) != 0 && fn_804D6220(fn_804D1694()) == 0) {
                if (fn_804D6210(fn_804D1694()) == 1) {
                    fn_804DC450(fn_800F7F90(), 0);
                }
                fn_804D16E4(fn_804D1694())->fn_8050CF90();
                if (fn_804EAA50(fn_800F7F90()) == 1) {
                    friendRoom = 1;
                } else {
                    fn_804D16CC()->fn_804AF520(10);
                }
                fn_804D6014(fn_804D16CC());
                fn_800C31F4("SE_UICOM_002", 0, -1, 1.0f);
                fn_804DBD08(mgr, 0);
                fn_804D169C(&mgr->mStateVar, 10);
                joined = 1;
            }
        }
        if (joined != 0 && fn_804D6210(fn_804D1694()) == 1) {
            if (friendRoom != 0) {
                fn_804D5FB8()->fn_804BA350(1);
            } else {
                fn_804D5FB8()->fn_804BA350(9);
            }
        }
        break;
    }
    case 12: {
        PPPlayer_unk* own = fn_804EB870(mgr);
        if (own != 0) {
            if (mgr->m548 != mgr->mMode) {
                fn_804DC358(mgr);
            }
            if (fn_804D6210(fn_804D1694()) == 1) {
                unsigned char server = fn_804D6228(fn_804D1694());
                if (fn_800F7F90()->m9979 != 0xFF && fn_804D6190(fn_804D1694()) == 0) {
                    if (server != mgr->m9979 || fn_804D6278(fn_804D1694(), own->mAid) == 1) {
                        fn_804FB470(mgr, 0xA0, 9);
                        break;
                    }
                }
                mgr->m9979 = server;
            }
            if (mgr->mFrame % mgr->m5D0 == 1 || fn_804D5F98(fn_804D1694()) == 13) {
                if (own != 0) {
                    if (fn_804D6278(fn_804D1694(), own->mAid) == 1) {
                        fn_804D824C(mgr, own, 2);
                    } else {
                        fn_804FAA7C(mgr, 0, own);
                    }
                }
                for (unsigned int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
                    PPPlayer_unk* player = fn_804D5F48(&mgr->mPlayers, i);
                    if (player->mFlags.mActive && fn_804D6278(fn_804D1694(), player->mAid)) {
                        mgr->m59C = player->mAid;
                        break;
                    }
                }
            }
            if (fn_804D5F98(fn_804D1694()) == 13) {
                if (fn_804D5FC0() != 0) {
                    if (fn_804D62E4(fn_804D5FC0())) {
                        fn_804D169C(&mgr->mStateVar, 14);
                        mgr->m99A0 = 120;
                        fn_804DC450(mgr, 3);
                        break;
                    }
                    fn_804D62F8(fn_804D5FC0(), 1);
                }
            } else if (fn_804D6278(fn_804D1694(), own->mAid) == 1) {
                int hosts = fn_804D6300(fn_804D1694());
                if (hosts != mgr->m504) {
                    if (hosts <= 1) {
                        if (fn_804D6348(fn_804D1694()) == 0 && fn_804D5FC0() != 0 && fn_804D6354(fn_804D5FC0()) >= 7) {
                            fn_804FB470(mgr, 0x9E, 9);
                        }
                        mgr->m508 = -1;
                    } else if (mgr->m504 < hosts) {
                        mgr->m508 = 3629;
                        mgr->m50A = 0;
                        fn_804FC0D4(mgr, 0);
                    }
                    if (fn_804D6190(fn_804D1694()) == 1) {
                        fn_804DC450(mgr, 1);
                    }
                    mgr->m504 = hosts;
                }
            }
            if (fn_804D5FC0() != 0 && mgr->m508 != -1) {
                fn_804C2268(fn_804D5FC0(), mgr->m508 / 60);
            }
            if (fn_804D635C(fn_804D1694()) == 0 && ((fn_804D5FC0() != 0 && fn_804D5FC0()->fn_804C2F9C() != 0) ||
                                                    (unsigned short)mgr->m508 <= 899)) {
                mgr->m53C = 1;
            } else {
                mgr->m53C = 0;
            }
            if (fn_804D6278(fn_804D1694(), own->mAid) == 1 && fn_804D6210(fn_804D1694()) == 1) {
                int lobby = fn_804D5FC0() != 0 ? fn_804D6354(fn_804D5FC0()) : 0;
                if (lobby != 8 && lobby != 9 && fn_804D6190(fn_804D1694()) == 1) {
                    fn_804DC374(mgr, mgr->m53C == 1 ? 5 : 1);
                }
            }
            if (mgr->m53C != 0) {
                fn_804D6160(fn_804D1694(), mgr->m53C);
                fn_804D1694()->fn_8050A634(mgr->m53C);
            } else {
                fn_804D6160(fn_804D1694(), 1);
                fn_804D1694()->fn_8050A634(0);
            }
            if (mgr->m508 > 0) {
                lbl_8087F614 = 1;
                if (fn_804EB1AC(fn_800F7F90()) == 1 && fn_804D635C(fn_804D1694()) == 0) {
                    if (mgr->m50A > 0) {
                        mgr->m50A--;
                    } else {
                        fn_804FC0D4(mgr, 0);
                        mgr->m508--;
                    }
                }
                if (mgr->m508 < 900) {
                    int rule = fn_804D6364(fn_804D5FD8());
                    if (rule != 4 && rule != 5) {
                        fn_804D5FD8()->fn_804CDE1C(4);
                    }
                }
            } else if (mgr->m508 == 0) {
                if (fn_804EB1AC(fn_800F7F90()) != 0 && lbl_8087F614 == 1) {
                    if (fn_804D6354(fn_804D5FC0()) < 6) {
                        fn_804D5FC0()->fn_804BC628(6);
                    }
                    lbl_8087F614 = 0;
                }
                if (--mgr->m975C <= 0) {
                    fn_804FB470(mgr, 0x9E, 9);
                }
            }
        }
        mgr->m99E8 = 0;
        mgr->m548 = mgr->mMode;
        fn_804E9360(mgr);
        fn_804FB550(mgr);
        break;
    }
    case 14:
        if (fn_804D5FC0() != 0) {
            fn_804D636C(fn_804D6374(fn_804D5FC0()), fn_804D637C(fn_804D6374(fn_804D5FC0())) + 1);
        }
        fn_804DA2D0(mgr, 1);
        fn_804E9360(mgr);
        fn_804D8610(mgr);
        fn_804D169C(&mgr->mStateVar, 15);
        if (GameBgm::GetInstance_unk() != 0) {
            GameBgm::GetInstance_unk()->fn_8037D4C0(0x1B5B, 10, 1, 0, 1.0f);
        }
        break;
    case 15:
        if (fn_804D5FC0() != 0) {
            fn_804D636C(fn_804D6374(fn_804D5FC0()), fn_804D637C(fn_804D6374(fn_804D5FC0())) + 1);
        }
        fn_804DA2D0(mgr, 1);
        fn_804E9360(mgr);
        if (mgr->m9A74[0] == 1) {
            fn_804D8910(mgr);
            fn_804D169C(&mgr->mStateVar, 16);
        } else {
            fn_804D87B0(mgr);
        }
        break;
    case 16:
        if (fn_804D5FC0() != 0) {
            fn_804D636C(fn_804D6374(fn_804D5FC0()), fn_804D637C(fn_804D6374(fn_804D5FC0())) + 1);
        }
        fn_804DA2D0(mgr, 1);
        fn_804E9360(mgr);
        if (mgr->m9A74[1] == 1) {
            fn_804D97F0(mgr);
            fn_804D169C(&mgr->mStateVar, 17);
        } else {
            fn_804D90E0(mgr);
        }
        break;
    case 17:
        if (fn_804D5FC0() != 0) {
            fn_804D636C(fn_804D6374(fn_804D5FC0()), fn_804D637C(fn_804D6374(fn_804D5FC0())) + 1);
        }
        fn_804DA2D0(mgr, 1);
        fn_804E9360(mgr);
        if (mgr->m9A74[2] == 1) {
            unsigned char aid = fn_804D6198(fn_804D1694());
            if (fn_804D6278(fn_804D1694(), aid)) {
                fn_804D6384(mgr, 300);
                fn_804E679C(mgr);
            }
            fn_804D169C(&mgr->mStateVar, 13);
        } else {
            fn_804D996C(mgr);
        }
        break;
    case 13:
        if (fn_804D5FC0() != 0) {
            fn_804D636C(fn_804D6374(fn_804D5FC0()), fn_804D637C(fn_804D6374(fn_804D5FC0())) + 1);
        }
        if (fn_804DA2D0(mgr, 0) == 1) {
            fn_8046ECDC(ResMgr_unk(), 0);
        }
        fn_804E9360(mgr);
        if (mgr->m5AC != 0) {
            fn_804D169C(&mgr->mStateVar, 18);
            if (fn_800F7FA0() != 0) {
                fn_80239DAC((atn::EffectManager*)fn_800F7FA0(), (unsigned int)mgr, 0, 0);
                fn_800D2338((atn::Task*)fn_800F7FA0());
            }
        } else if (fn_804D6390(mgr) == 1) {
            fn_804FB470(mgr, 0x9E, 9);
        }
        break;
    case 18: {
        fn_804E9360(mgr);
        PPPlayer_unk* own = fn_804EB870(mgr);
        if (fn_804DA2D0(mgr, 0) == 1 && fn_800F7FA0() == 0) {
            if (fn_804DE884(mgr) == 0) {
                fn_804FB470(mgr, 0x9F, 22);
                break;
            }
            if (fn_804D5FA8(&mgr->mStateVar) != 43) {
                fn_804D169C(&mgr->mStateVar, 19);
                fn_804D16CC()->fn_804AF520(21);
                fn_804D6384(mgr, 1800);
            }
        }
        if (own->mB0 == -1) {
            fn_804FB470(mgr, 0x9E, 9);
        }
        break;
    }
    case 19: {
        fn_8046ECDC(ResMgr_unk(), 0);
        fn_804E9360(mgr);
        if (fn_804DF404(mgr) == 0) {
            PPPlayer_unk* own = fn_804EB870(mgr);
            if (own != 0) {
                own->mRef.mChara = ((PlayerManager*)fn_8000D9E8())->GetHead_unk();
                fn_804D63A8(own->mRef.mChara, 1);
                own->mFlags.m27 = 1;
            }
            mgr->m608 = -1;
            for (int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
                PPPlayer_unk* player = fn_804D5F48(&mgr->mPlayers, i);
                if (player->mFlags.mActive == 1) {
                    fn_804EAA84(mgr, player->mRef.mChara, 1, 0);
                    if (player->mEquips[0].mOn) {
                        if (fn_804D63D0(mgr) != 2 && fn_804D63D8(player->mRef.mChara) != 0) {
                            for (int j = 0; j < 4; j++) {
                                if (fn_804D63D8(player->mRef.mChara)->mD8[j] > 0) {
                                    int id = fn_80206B14(fn_804D63D8(player->mRef.mChara)->mD4[j],
                                                         fn_804D63D8(player->mRef.mChara)->mD8[j]);
                                    fn_804D63E0((PPCharaPart_unk*)*fn_80266FB4(fn_80266FA4(player->mRef.mChara), j), id);
                                }
                            }
                            for (int j = 0; j < 5; j++) {
                                if (fn_804D63D8(player->mRef.mChara)->m108[j] >= 0 &&
                                    player->mRef.mChara->m680[j] != 0) {
                                    int id = fn_8020ED84(j, fn_804D63D8(player->mRef.mChara)->m108[j]);
                                    fn_804D63F8((PPEquipObj_unk*)player->mRef.mChara->m680[j], id);
                                }
                            }
                        }
                        fn_804D6400(player->mRef.mChara->GetStatus_unk(), player->mEquips[0].mLevel);
                    }
                }
            }
            if (mgr->mGame != 0) {
                ((GameCamera*)fn_80122550(mgr->mGame))->fn_8037EF30(1, 1, -1.0f);
            }
            if (GameBgm::GetInstance_unk() != 0) {
                if (mgr->mMode != 2) {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x66, 0, 1, 0, 0.7f);
                } else if (mgr->m560 == 1) {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0xC9, 0, 1, 0, 0.7f);
                } else {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x8AD, 0, 1, 0, 0.7f);
                }
            }
            if (fn_804D6408() != 0) {
                fn_804D6408()->fn_800D246C(0);
            }
            fn_804D169C(&mgr->mStateVar, 26);
            PPMsgHeader_unk msg(0x1010);
            fn_804D1694();
            fn_804AE3BC()->fn_8050E094(-1, 0x1010, 0, 1);
        } else if (fn_804D6390(mgr) == 1) {
            fn_804FB470(mgr, 0x9E, 9);
        } else if (mgr->mGame != 0 && fn_80244CA0(mgr->mGame) != 0) {
            mgr->mGame->fn_80373118();
        }
        break;
    }
    case 26: {
        fn_8046ECDC(ResMgr_unk(), 0);
        fn_804D16CC()->fn_804AF520(15);
        fn_804E9360(mgr);
        fn_804E6518(mgr);
        if (fn_804D6390(mgr) == 1) {
            fn_804FB470(mgr, 0x9E, 9);
            break;
        }
        if (fn_804EB5D8(mgr)) {
            PPPlayer_unk* own = fn_804EB870(mgr);
            if (fn_804D6278(fn_804D1694(), own->mAid) == 0) {
                fn_804D1694();
                fn_804AE3BC()->fn_8050E094(fn_804D6228(fn_804D1694()), 0x1044, 0, 1);
                fn_804D1694();
                fn_804AE3BC()->fn_8050DDDC(fn_804D6228(fn_804D1694()), 1);
            }
            fn_804D6384(mgr, 900);
            fn_804D169C(&mgr->mStateVar, 27);
        }
        break;
    }
    case 27: {
        fn_8046ECDC(ResMgr_unk(), 0);
        fn_804E9360(mgr);
        if (fn_804D6390(mgr) == 1) {
            fn_804FB470(mgr, 0x9E, 9);
            break;
        }
        PPPlayer_unk* own = fn_804EB870(mgr);
        if (fn_804D6278(fn_804D1694(), own->mAid) == 1) {
            int ready = 0;
            int count = 0;
            for (unsigned int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
                if (fn_804D5F48(&mgr->mPlayers, i)->mFlags.mActive == 1 &&
                    own->mAid != fn_804D5F48(&mgr->mPlayers, i)->mAid && fn_804EB3CC(mgr, i) == 0) {
                    count++;
                    if (mgr->m9A34[fn_804D5F48(&mgr->mPlayers, i)->mAid] == 1) {
                        ready++;
                    }
                }
            }
            if (count == ready) {
                fn_804D169C(&mgr->mStateVar, 29);
                fn_804D16CC()->fn_804AF520(16);
                fn_804D16D4()->fn_800D246C(0);
                fn_804D16D4()->fn_804B50C0(1);
                fn_804D16D4()->fn_804B6204(mgr->m564);
                fn_804D1694();
                fn_804AE3BC()->fn_8050E094(-1, 0x1045, 0, 1);
                fn_804D1694();
                fn_804AE3BC()->fn_8050DDDC(-1, 1);
            }
        }
        break;
    }
    case 29:
        fn_804E9360(mgr);
        if (mgr->m558 >= 50) {
            fn_804D169C(&mgr->mStateVar, 28);
            mgr->m558 = 0;
            fn_804DA34C(mgr);
        } else {
            mgr->m558++;
        }
        break;
    case 28: {
        fn_804E9360(mgr);
        for (int i = 0; i < 8; i++) {
            mgr->m99F4[i] = 0;
        }
        for (int i = 0; i < 8; i++) {
            mgr->m9A14[i] = 0;
        }
        mgr->mStartTime = fn_804CF194();
        mgr->m534 = 0;
        fn_804D169C(&mgr->mStateVar, 30);
        mgr->m544 = mgr->mMode;
        fn_8050F8C4(fn_804D6024(fn_804D1694()), 1);
        fn_8050EAE8(fn_804D6024(fn_804D1694()));
        fn_804F7EF0(fn_804D6020(fn_804D6024(fn_804D1694())), 0x9F0);
        fn_804D00B8(0xFB, 2);
        fn_804E9C64(mgr);
        PPPlayer_unk* own = fn_804EB870(mgr);
        char name[0x100];
        for (int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
            PPPlayer_unk* player = fn_804D5F48(&mgr->mPlayers, i);
            if (player->mFlags.mActive != 1 || player->mRef.mChara == 0) {
                continue;
            }
            int lives = fn_804D6420(mgr);
            ((int*)player->mRef.mChara->GetStatus_unk())[0x224 / 4] = lives;
            if (mgr->m604 != 0) {
                int hp = ((int*)player->mRef.mChara->GetStatus_unk())[0x160 / 4];
                if ((unsigned int)mgr->mMode <= 1) {
                    ((float*)player->mRef.mChara->GetStatus_unk())[0x1B0 / 4] = 60 - hp;
                } else if (mgr->mMode == 2) {
                    ((float*)player->mRef.mChara->GetStatus_unk())[0x1B0 / 4] = 80 - hp;
                }
                CharaInfo_unk* info = (CharaInfo_unk*)fn_804D1694()->vf14(player->mAid);
                if (info != 0 && fn_804EB3CC(mgr, i) == 0) {
                    fn_804D8078((PPCharaInfo_unk*)player->m004, (PPCharaInfo_unk*)player->mRef.mChara->GetInfo_unk());
                    player->m08 = (char*)info + 0x10;
                    player->m0C = (char*)info + 0x10;
                    fn_804D6484(player->mRef.mChara, (CharaInfo_unk*)player->m004);
                }
                PPCharaData_unk* data = fn_804D63D8(player->mRef.mChara);
                if (data != 0 && data->mC8 >= 0) {
                    sprintf(name, "%s", ((const char**)player->mRef.mChara->GetInfo_unk())[0x14 / 4]);
                    int len = strlen(name);
                    if (mgr->mMode == 1) {
                        name[len - 3] += 1;
                    } else if (mgr->mMode == 0) {
                        name[len - 3] += 2;
                    }
                    chr::CharaData_unk* chara = fn_8020A81C(name);
                    if (chara != 0) {
                        player->mRef.mChara->fn_8017A300(chara);
                    }
                }
                for (int j = 0; j < 4; j++) {
                    fn_803396FC(player->mRef.mChara->GetStatus_unk(), j, (char*)fn_804D63D8(player->mRef.mChara) + j * 8 + 0xE8);
                }
                player->mRef.mChara->GetStatus_unk()->fn_8012B3E8();
                player->mRef.mChara->GetStatus_unk()->fn_8012B988();
                player->mRef.mChara->GetStatus_unk()->fn_8012D8B8();
            }
            fn_800F530C(player->mRef.mChara)[0xC0 / 4] = 1;
            fn_800F530C(player->mRef.mChara)[0xB4 / 4] = 0;
            ((BattleCharaTask*)player->mRef.mChara)->SetFlagC26_unk(0);
            player->mRef.mChara->SetSide_unk(player->mFlags.m22);
            if (player->mB0 == 12 || player->mB0 == 21) {
                if (((Field_unk*)mgr->mGame)->fn_8013C504() != 0) {
                    atn::SimpleArray<PPRoute_unk>* routes = fn_804D649C(((Field_unk*)mgr->mGame)->fn_8013C504());
                    for (unsigned int j = 0; j < routes->size(); j++) {
                        PPRoute_unk* route = fn_804D80FC(routes, j);
                        if (route->m2C != 0) {
                            unsigned int team = player->mFlags.m22;
                            if ((team == 1 && route->m24 > 50) || (team == 2 && route->m24 < 50)) {
                                fn_800F530C(player->mRef.mChara)[0xB8 / 4] = route->m24;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (fn_804D63D0(mgr) == 2 && mgr->mGame != 0 && mgr->mGame->GetCurrentMap() != 0 &&
            mgr->mGame->GetCurrentMap()->GetMapSubId() == 6) {
            fn_804DA4A0(mgr, 1);
        }
        if (fn_804D63D0(mgr) == 2 && mgr->mGame != 0 && mgr->mGame->GetCurrentMap() != 0 &&
            mgr->mGame->GetCurrentMap()->GetMapSubId() == 7) {
            fn_8050E618(4);
        } else {
            fn_8050E618(3);
        }
        if (own != 0 && own->mRef.mChara != 0) {
            fn_804D63A8(own->mRef.mChara, 0);
        }
        for (int i = 0; i < fn_804D810C(&mgr->mCharas); i++) {
            PPCharaEntry_unk* entry = fn_804D64A4(&mgr->mCharas, i);
            if (entry->mRef.mChara == 0) {
                continue;
            }
            if (fn_804D63D8(entry->mRef.mChara)->mC8 >= 0) {
                ((float*)entry->mRef.mChara->GetStatus_unk())[0x1B0 / 4] = 56;
            }
            entry->mRef.mChara->GetStatus_unk()->fn_8012B3E8();
            entry->mRef.mChara->GetStatus_unk()->fn_8012B988();
            entry->mRef.mChara->GetStatus_unk()->fn_8012D8B8();
            fn_800F530C(entry->mRef.mChara)[0xC0 / 4] = 1;
            fn_800F530C(entry->mRef.mChara)[0xB4 / 4] = 0;
            entry->mRef.mChara->fn_8014DEE4(0, 0, 0);
            if (entry->mRef.mChara->IsFrozen_unk() == 0) {
                entry->mRef.mChara->fn_8016E4C4(1);
            }
        }
        mgr->m604 = 0;
        mgr->m5AC = 0;
        mgr->m9A77 = 0;
        fn_804E9360(mgr);
        if (fn_804D6278(fn_804D1694(), own->mAid) != 1) {
            PPNetMsg_unk<unsigned char> msg(0x1046);
            unsigned char* data = msg.GetData_unk();
            data[0] = fn_804D64B4(own);
            fn_804D1694();
            fn_804AE3BC()->fn_8050E094(fn_804D6228(fn_804D1694()), 0x1046, data, 1);
            fn_804F5A98((PPRequests_unk*)fn_800F7F90(), own->mAid, 150, data[0]);
            mgr->m9A78 = fn_804CF194();
            mgr->m9A80 = data[0];
        }
        break;
    }
    case 30: {
        fn_804E9360(mgr);
        PPPlayer_unk* own = fn_804EB870(mgr);
        if (mgr->m9A77 == 0 && own != 0 && fn_804D6278(fn_804D1694(), own->mAid) != 1) {
            int result = fn_804F5E38(fn_800F7F90(), mgr->m9A80);
            if ((unsigned int)(result + 2) <= 1) {
                fn_804F5C9C(fn_800F7F90(), mgr->m9A80);
            } else if (result == 1) {
                long long now = fn_804CF194();
                long long half = (now - mgr->m9A78) / 2;
                mgr->mStartTime = now - (*fn_804F5B08(mgr, mgr->m9A80) + half);
                fn_804F5C9C(fn_800F7F90(), mgr->m9A80);
                mgr->m9A77 = 1;
            }
        }
        switch (((GameUISession_unk*)mgr)->GetM50C_unk()) {
        case 0:
            if (mgr->mGame != 0) {
                mgr->mGame->fn_80370AE4(0xFF, fn_804DB3B4(mgr) * 30);
            }
            if (fn_804D63D0(mgr) != 2 && fn_804D64C8(mgr) && GameBgm::GetInstance_unk() != 0 && mgr->m9768 == 0) {
                GameBgm::GetInstance_unk()->fn_8037D4C0(0x8B8, 15, 1, 0, 1.0f);
                fn_804DA48C(mgr, 6);
                mgr->m9768 = 1;
            }
            if (((PlayerManager*)fn_8000D9E8()) != 0 && fn_804FB968(mgr, ((PlayerManager*)fn_8000D9E8())->GetHead_unk()) != 0) {
                mgr->m99F0[3] = 1;
            }
            break;
        case 1:
            if (fn_8036635C() != 0 && fn_8036635C()->IsFrozen_unk() == 0) {
                fn_8036635C()->FreezeTask();
            }
            if (!own->mFlags.m26) {
                fn_804EA8B8(mgr, 0);
                fn_804D00B8(0xFB, 3);
                mgr->m99A0 = 900;
                fn_804D6384(mgr, 300);
            } else if (fn_804D6390(mgr) == 1) {
                fn_804D600C(mgr, 3);
            } else {
                int ready = 0;
                int count = 0;
                for (unsigned int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
                    if (fn_804D5F48(&mgr->mPlayers, i)->mFlags.mActive == 1 &&
                        own->mAid != fn_804D5F48(&mgr->mPlayers, i)->mAid && fn_804EB3CC(mgr, i) == 0) {
                        count++;
                        if (mgr->m99F4[i] == 1) {
                            ready++;
                        }
                    }
                }
                if (count == ready) {
                    if (fn_804D6278(fn_804D1694(), own->mAid) != 1) {
                        fn_804D1694();
                        fn_804AE3BC()->fn_8050E094(fn_804D6228(fn_804D1694()), 0x1042, 0, 1);
                        fn_804D1694();
                        fn_804AE3BC()->fn_8050DDDC(fn_804D6228(fn_804D1694()), 1);
                    }
                    fn_804D600C(mgr, 2);
                    fn_804D6384(mgr, 300);
                }
            }
            break;
        case 2:
            if (fn_804D6390(mgr) == 1) {
                fn_804D600C(mgr, 3);
            } else if (fn_804D6278(fn_804D1694(), own->mAid) == 1) {
                int ready = 0;
                int count = 0;
                for (unsigned int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
                    if (fn_804D5F48(&mgr->mPlayers, i)->mFlags.mActive == 1 &&
                        own->mAid != fn_804D5F48(&mgr->mPlayers, i)->mAid && fn_804EB3CC(mgr, i) == 0) {
                        count++;
                        if (mgr->m9A14[i] == 1) {
                            ready++;
                        }
                    }
                }
                if (count == ready) {
                    PPNetMsg_unk<PPMsgChars_unk> msg(0x1043);
                    PPMsgChars_unk* data = msg.GetData_unk();
                    for (int i = 0; i < 8; i++) {
                        PPPlayer_unk* player = fn_804D5F0C(mgr, i);
                        if (player != 0 && player->mFlags.mActive == 1) {
                            data->mIds[i] = fn_804D659C(&player->mDC);
                        } else {
                            data->mIds[i] = 0;
                        }
                    }
                    data->m10 = mgr->m2B80;
                    fn_804D1694();
                    fn_804AE3BC()->fn_8050E094(-1, 0x1043, data, 1);
                    fn_804D1694();
                    fn_804AE3BC()->fn_8050DDDC(-1, 1);
                    fn_804D600C(mgr, 3);
                }
            }
            break;
        case 3:
            fn_8050F8C4(fn_804D6024(fn_804D1694()), 0);
            mgr->m997C = 0;
            mgr->m9980 = 0;
            if ((unsigned int)mgr->mMode <= 1) {
                mgr->m997C = 1;
                fn_804D600C(mgr, 4);
                fn_804D65A4(fn_804D5FD0(), 1);
                mgr->m99A0 = 900;
            } else if (mgr->mMode == 2) {
                mgr->m997C = 1;
                fn_804D600C(mgr, 4);
                fn_804D65A4(fn_804D5FD0(), 1);
                mgr->m99A0 = 450;
                fn_804D16D4()->fn_804B50C0(3);
            }
            fn_804DA398(mgr);
            if (fn_804D5FC8() != 0 && fn_804D65AC(fn_804D5FC8()) == 0) {
                fn_804D5FC8()->fn_804C35F0(3);
            }
            fn_804D6384(mgr, 150);
            break;
        case 4:
            if (fn_804D6390(mgr) == 1) {
                if (mgr->m9980 == 0) {
                    mgr->m9980 = 1;
                    if (mgr->m997C == 1) {
                        fn_804E448C(mgr);
                    } else {
                        fn_8050EAE8(fn_804D6024(fn_804D1694()));
                        fn_804F7EF0(fn_804D6020(fn_804D6024(fn_804D1694())), 0x9F0);
                    }
                    if (GameBgm::GetInstance_unk() != 0) {
                        if (mgr->m2B84 == 0 || mgr->m2B84 == 3) {
                            GameBgm::GetInstance_unk()->fn_8037D4C0(0x8CB, 15, 1, 0, 0.8f);
                        } else if (mgr->m2B84 == 1 || mgr->m2B84 == 4) {
                            GameBgm::GetInstance_unk()->fn_8037D4C0(0x3F5, 15, 1, 0, 0.8f);
                        } else {
                            GameBgm::GetInstance_unk()->fn_8037D4C0(0x1F6, 15, 1, 0x21FC, 0.8f);
                        }
                    }
                    fn_804D5FD0()->fn_804C5E1C();
                    fn_804D5FD0()->fn_804C6888(mgr->m99A0 / 60 + 1);
                }
                fn_804D600C(mgr, 5);
            }
            break;
        case 5: {
            fn_800F7F90()->m999C = 0;
            if (fn_804D5FD0()->fn_804C8204() != 1) {
                break;
            }
            int state = fn_804D65C0(fn_804D5FD0());
            mgr->m99A0--;
            fn_804D5FD0()->fn_804C6888(mgr->m99A0 / 60 + 1);
            if (mgr->m99A0 >= 750) {
                break;
            }
            if (state != 7 && (unsigned int)(state - 5) <= 1) {
                fn_804D65A4(fn_804D5FD0(), 0);
                int skip = 0;
                if (fn_800A555C(GetPad_unk(), 0, 4) != 0) {
                    skip = 1;
                } else if (mgr->m99A0 < 450) {
                    skip = 1;
                }
                if (skip == 1) {
                    mgr->m99A0 = 450;
                    fn_804D5FD0()->fn_804C5E9C(7);
                    fn_804D65A4(fn_804D5FD0(), 1);
                }
            } else if (mgr->m99A0 < 360) {
                if ((fn_804D5FC8() == 0 || fn_804D65AC(fn_804D5FC8()) != 0) && state != 6 && state != 5 &&
                    (mgr->m99A0 <= 0 || fn_800A555C(GetPad_unk(), 0, 4) != 0)) {
                    MpRoom_unk* room = ((MpArena_unk*)mgr)->GetRoom_unk();
                    if (room->m6B08 != 0) {
                        fn_804D600C(mgr, 6);
                        fn_804D9BF4(mgr, room->m6B00, room->m6B0C, room->m6B10);
                        fn_804D5FD0()->fn_804C5E9C(8);
                        fn_804D5FD0()->fn_804C7C34(room->m6B00, -1, 0);
                    } else {
                        fn_804D600C(mgr, 7);
                        fn_804E49A4(mgr);
                        fn_804D5FD0()->fn_804C5E9C(9);
                    }
                }
                fn_804D65A4(fn_804D5FD0(), 0);
            }
            break;
        }
        case 6:
            if (fn_804D65C8(fn_804D5FD0()) == 1) {
                fn_804D600C(mgr, 7);
                fn_804E49A4(mgr);
                fn_804D5FD0()->fn_804C5E9C(9);
            }
            break;
        case 7:
            if (mgr->mSaveLoad->fn_80502848() == 0) {
                int state = fn_804D65C0(fn_804D5FD0());
                if (state == 9) {
                    fn_804D5FD0()->fn_804C5E9C(10);
                } else if (state == 12) {
                    fn_804D169C(&mgr->mStateVar, 31);
                    mgr->m99E8 = 1;
                    fn_804D600C(mgr, 0);
                    if (fn_804D5FD0() != 0) {
                        fn_800D2338((atn::Task*)fn_804D5FD0());
                    }
                }
            }
            break;
        }
        fn_804E5BC4(mgr);
        fn_804FB550(mgr);
        break;
    }
    case 31:
        fn_804D169C(&mgr->mStateVar, 33);
        fn_804D16D4()->fn_804B50C0(4);
        fn_804D16D4()->fn_804B691C();
        ((atn::Task*)fn_804D16D4())->fn_800D246C(1);
        break;
    case 33:
        if (mgr->m9974 != 0) {
            fn_800D2338(mgr->m9974);
            mgr->m9974 = 0;
        }
        fn_804D1694()->fn_80509C88();
        if (mgr->m99E8 == 0) {
            fn_804DF11C(mgr, 1);
        } else {
            fn_804DF11C(mgr, 0);
        }
        if (mgr->mMode == 2) {
            fn_804EA608(mgr, -1, 0);
        }
        fn_804D0164();
        if (mgr->m99E8 == 0) {
            fn_804DBD08(mgr, 0);
        }
        fn_804D169C(&mgr->mStateVar, 34);
        break;
    case 34:
        if (fn_800F7FA0() == 0 && mgr->m9968.IsLoading() == 0) {
            int next;
            atn::EffectManager::fn_80239798(mgr, 0x240, 0x480, 8, 0x180);
            fn_800F7FA8(fn_800F7FA0(), mgr, 0);
            fn_8026607C(fn_800F7FA0(), &mgr->m9968, 0, 1);
            if (mgr->m99E8 == 0) {
                fn_804DC450(mgr, 0);
                fn_804D16CC()->fn_804AF520(9);
                fn_804D6014(fn_804D16CC());
                fn_804D60EC(fn_804D16CC(), 15);
                next = 10;
            } else {
                next = 12;
                fn_804D600C(mgr, 0);
                fn_804DC358(mgr);
                fn_804D6014(fn_804D16CC());
                fn_804D60EC(fn_804D16CC(), 15);
                for (int i = 0; i < 3; i++) {
                    mgr->m9A74[i] = 0;
                }
                for (int i = 0; i < 8; i++) {
                    mgr->m99F4[i] = -1;
                }
                for (int i = 0; i < 8; i++) {
                    mgr->m9A14[i] = -1;
                }
                for (int i = 0; i < 8; i++) {
                    mgr->m9A34[i] = -1;
                }
                for (int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
                    fn_804D5F48(&mgr->mPlayers, i)->mFlags.m28 = 0;
                    fn_804D5F48(&mgr->mPlayers, i)->mD53 = 0;
                    if (fn_804EB3CC(mgr, i) == 1) {
                        fn_804D1F6C(fn_804D5F48(&mgr->mPlayers, i));
                    }
                    fn_804D5F48(&mgr->mPlayers, i)->mFlags.m10 = 2;
                    if (fn_804D63D0(mgr) == 1 && fn_804D6210(fn_804D1694()) == 0) {
                        fn_804D5F48(&mgr->mPlayers, i)->mFlags.m22 = 0;
                    }
                }
                PPPlayer_unk* own = fn_804EB870(mgr);
                own->mFlags.m22 = own->mD4;
                own->mB0 = own->mD8;
                if (fn_804EB1AC(mgr) == 0) {
                    fn_804FA948(mgr, own->mD8, 0);
                    fn_804FAB40(mgr, own->mFlags.m22, own, 0, 1);
                }
                memset(mgr->m9984, 0, sizeof(mgr->m9984));
                memset(mgr->m998C, 0xFF, sizeof(mgr->m998C));
                fn_804D1694()->fn_8050A3D0(0);
                fn_804D16CC()->fn_804AF520(13);
            }
            mgr->m9774.SetPosition(atn::Vector3(0.0f, -99500.0f, 0.0f));
            mgr->m9774.SetTarget_unk(atn::Vector3(0.0f, -99500.0f, 10.0f));
            fn_8037F688(&mgr->m9774, atn::DegToRad_unk(60.0f));
            if (GameBgm::GetInstance_unk() != 0) {
                GameBgm::GetInstance_unk()->fn_8037D4C0(0x548, 15, 1, 0, 0.7f);
            }
            if (mgr->m99F0[0] == 0 || mgr->m99F0[1] == 0 || mgr->m99F0[2] == 0) {
                if (fn_804EAFD0(mgr) > 1) {
                    fn_804FB470(mgr, 0x9D, 22);
                    if (fn_804D16CC() != 0) {
                        fn_804D65D4(fn_804D16CC(), 4);
                    }
                    fn_804E9360(mgr);
                    break;
                }
            }
            if (mgr->m99F0[3] != 0) {
                fn_804FB470(mgr, 0x9F, 22);
                if (fn_804D16CC() != 0) {
                    fn_804D65D4(fn_804D16CC(), 4);
                }
            } else {
                fn_804D169C(&mgr->mStateVar, next);
            }
        }
        fn_804E9360(mgr);
        break;
    case 35:
        if (fn_804D6680()->fn_8050C840(3) == 1) {
            MpFriendList_unk* list = fn_804D6024(fn_804D1694());
            void* data = fn_804D6048(list);
            wchar_t* name = fn_804D60E4(list);
            fn_804D6680()->fn_8050CCD0(0, ((int*)data)[2], name, 0x20);
        } else {
            fn_804D65DC(fn_804D6680(), 5);
        }
        fn_804D169C(&mgr->mStateVar, 36);
        break;
    case 36:
        if (fn_804D5FA0(fn_804D6680()) == 5) {
            fn_804D169C(&mgr->mStateVar, 37);
        }
        break;
    case 37:
        fn_804D169C(&mgr->mStateVar, 39);
        if (fn_804D16CC() != 0) {
            if (fn_804D6644(fn_804D16CC()) == 0) {
                fn_804D169C(&mgr->mStateVar, 39);
            } else if (fn_804D6644(fn_804D16CC()) == 2) {
                fn_804D169C(&mgr->mStateVar, 40);
            } else {
                fn_804D169C(&mgr->mStateVar, 38);
            }
        }
        break;
    case 38:
    case 39:
    case 40:
        if (fn_804D5F04(&mgr->mStateVar) == 38) {
            fn_804D6680()->fn_8050C898(mgr->m974C, 30, 0, fn_804F60E0);
        } else if (fn_804D5F04(&mgr->mStateVar) == 40) {
            fn_804D6680()->fn_8050CB24(mgr->m974C, 65, 0, fn_804F60E0);
        } else {
            fn_804D6680()->fn_8050C9D4(mgr->m974C, 30, 0, fn_804F60E0);
        }
        fn_804D169C(&mgr->mStateVar, 41);
        break;
    case 41:
        if (fn_804D5FA0(fn_804D6680()) == 8) {
            fn_804D169C(&mgr->mStateVar, 42);
            fn_804D16CC()->fn_804AF520(18);
        }
        break;
    }

    if (mgr->m976C != 0 && fn_804D664C(mgr) == 0) {
        mgr->m976C = 0;
        fn_804FB470(mgr, 0xE6, 22);
    }
    if (fn_800CB58C(&mgr->m2B70) == 0) {
        fn_800CB5C8(&mgr->m2B70, 0, 0);
    }
    if (mgr->mFrame % 300 == 0 && GameBgm::GetInstance_unk() != 0 && fn_804D6664(GameBgm::GetInstance_unk()) == 0) {
        int state = fn_804D5FA8(&mgr->mStateVar);
        if ((state >= 7 && fn_804D5FA8(&mgr->mStateVar) <= 11) ||
            (fn_804D5FA8(&mgr->mStateVar) == 12 && fn_804D5FC0() != 0 && fn_804D6354(fn_804D5FC0()) < 7) ||
            (fn_804D5FA8(&mgr->mStateVar) >= 35 && fn_804D5FA8(&mgr->mStateVar) <= 42)) {
            GameBgm::GetInstance_unk()->fn_8037D4C0(0x548, 30, 1, 0, 0.7f);
        } else if (fn_804D5FA8(&mgr->mStateVar) == 30 && fn_804D63D0(mgr) != 2) {
            if (((GameUISession_unk*)mgr)->GetM50C_unk() == 0) {
                if (mgr->m9768 == 0) {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x66, 30, 1, 0, 0.7f);
                } else {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x8B8, 30, 1, 0, 1.0f);
                }
            } else if (((GameUISession_unk*)mgr)->GetM50C_unk() == 5) {
                if (mgr->m2B84 == 0 || mgr->m2B84 == 3) {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x8CB, 30, 1, 0, 0.8f);
                } else if (mgr->m2B84 == 1 || mgr->m2B84 == 4) {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x3F5, 30, 1, 0, 0.8f);
                } else {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x1F6, 30, 1, 0x21FC, 0.8f);
                }
            }
        }
    }
    mgr->mFrame++;
}

// inferred: defined after its first use, so the static of __NetMsgStatic_unk registers it (weak copy 0x804D8188)
inline NetMsgStatic_unk::~NetMsgStatic_unk() {}

// inferred: the slot holds a player of a team (team in the high nibble of the aid byte)
static inline int IsTeamPlayer_unk(PPPlayer_unk* player) {
    if (player != 0 && player->mAid != 0xFF && (player->mAid & 0xF0) != 0) {
        return 1;
    }
    return 0;
}

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach); // inferred: stops the sound

// inferred: sets the room of the next game and the count down
extern "C" int fn_804D9BF4(PPManager2_unk* mgr, int a, int b, int c) {
    mgr->m348 = a;
    mgr->m34C = b;
    mgr->m350 = c;
    mgr->m99A0 = 90;
    return 1;
}

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

extern "C" const char* const lbl_808875AC;
// inferred: plays a sound when the game is not a mode 2 game
extern "C" void fn_804DA34C(PPManager2_unk* mgr) {
    if (mgr->mMode != 2) {
        fn_800C31F4(lbl_808875AC, 0, -1, 1.0f);
    }
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

extern "C" void fn_804DA478(PPManager2_unk* mgr) {
    if (MP::D2AMpMenuCockpit::sInstance != 0) {
        MP::D2AMpMenuCockpit::sInstance->fn_804B691C();
    }
}

extern "C" void fn_804DA48C(PPManager2_unk* mgr, int value) {
    if (MP::D2AMpMenuCockpit::sInstance != 0) {
        MP::D2AMpMenuCockpit::sInstance->fn_804B67E4(value);
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

// NONMATCHING: 94.82% - the original takes the zero of the loop as a copy of the index (mr r29, r26), as fn_804DF310
// inferred: resets the player slots in the modes 0 and 2 and ends the session (mode 3) or the match (0, 1)
extern "C" int fn_804DBD08(PPManager2_unk* mgr, int mode) {
    if (mode == 0 || mode == 2) {
        for (int i = 0; i < mgr->mPlayers.mNum; i++) {
            PPPlayer_unk* player = &mgr->mPlayers.mData[i];
            player->mAid = 0xFF;
            player->mDC.fn_80501288(0);
            player->mFlagWord = 0;
            player->mB0 = 0;
            player->mD38[4] = 0;
            player->mD50 = -1;
            player->mD4C = 0;
            player->mD53 = 0;
            player->mD54 = -1;
            player->mD58 = 0;
            player->mD8 = 0;
            player->mD4 = 0;
        }
        memset(mgr->m9984, 0, 8);
        memset(mgr->m998C, 0, 8);
        if (mgr->mState > 10) {
            fn_804FB220(&mgr->mStateVar, 10);
        }
    }
    if (mode == 3) {
        return lbl_8087F628->fn_80509DA0();
    }
    if (!(mode != 0 && mode != 1)) {
        return lbl_8087F628->fn_80509CAC();
    }
    return 1;
}

// NONMATCHING: the "," is at +0x19D of the pool of the unit (lbl_80759E48), not written yet
// inferred: splits `text` at the commas into its 6 fields (the last one runs to the end); 0 if a comma is
// missing. Empty fields are stored as null pointers
extern "C" int fn_804DBE24(PPManager2_unk* mgr, char* text, char** fields) {
    char* end;
    char* last = text;
    for (int i = 0; i < 6; i++) {
        if (i == 5) {
            end = last + strlen(last);
        } else {
            end = strstr(text, ",");
            if (end == 0) {
                return 0;
            }
        }
        if (text == end) {
            *fields = 0;
        } else {
            *fields = text;
        }
        *end = 0;
        text = end + 1;
        fields++;
    }
    return 1;
}

// inferred: updates the room status (2 when the network is not local)
extern "C" void fn_804DC358(PPManager2_unk* mgr) {
    int mode = 2;
    if (lbl_8087F628->m90 == 1) {
        mode = 1;
    }
    fn_804DC450(mgr, mode);
}

extern "C" int fn_804DBEC8(PPManager2_unk* mgr, const char* text, char* buf, char** fields, int arg);
extern "C" int atoi(const char* s);
// inferred: fourth and fifth fields of a status string as numbers
extern "C" int fn_804DC7C0(PPManager2_unk* mgr, const char* text) {
    char* fields[6];
    char buf[0x100];
    if (fn_804DBEC8(mgr, text, buf, fields, 0) == 0 || fields[3] == 0) {
        return 0;
    }
    return atoi(fields[3]);
}

extern "C" int fn_804DC80C(PPManager2_unk* mgr, const char* text) {
    char* fields[6];
    char buf[0x100];
    if (fn_804DBEC8(mgr, text, buf, fields, 0) == 0 || fields[4] == 0) {
        return 0;
    }
    return atoi(fields[4]);
}

extern "C" int fn_804DCA4C() {
    return 0;
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

extern "C" int fn_804DD280(PPManager2_unk* mgr, int mode) {
    return mode == 2 ? 21 : 12;
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

extern "C" int fn_804E4C34(PPManager2_unk* mgr);
extern "C" int fn_804E4C5C(PPManager2_unk* mgr);
extern "C" int fn_804E4B44(PPManager2_unk* mgr, PPPlayer_unk* target);

// inferred: the server asks the other players for their state (message 0x1047)
extern "C" void fn_804DDD50(PPManager2_unk* mgr) {
    unsigned char aid = lbl_8087F628->GetMyAid_unk();
    if (aid == lbl_8087F628->GetServerAid_unk()) {
        fn_804AE3BC()->fn_8050E094(-1, 0x1047, 0, 1);
        fn_804AE3BC()->fn_8050DDDC(-1, 1);
    }
}

// NONMATCHING: 90.49% - the original reloads lbl_8087F610 in every iteration and keeps the zero of the index in
// two more registers
// inferred: resets the player slots (and the aid tables m9984/m998C)
extern "C" void fn_804DF310(PPManager2_unk* mgr) {
    memset(mgr->m9984, 0, 8);
    memset(mgr->m998C, 0xFF, 8);
    for (int i = 0; i < ((PPManager2_unk*)lbl_8087F610)->mPlayers.mNum; i++) {
        PPPlayer_unk* player = fn_804D5F0C((PPManager2_unk*)lbl_8087F610, i);
        player->mB0 = 0;
        player->mAid = 0xFF;
        player->mFlags.mActive = 0;
        player->mFlags.m29 = 0;
        player->mFlags.m26 = 1;
        player->mFlags.m22 = 0;
        player->mFlags.m10 = 2;
        player->mFlags.m06 = 0xF;
        player->mFlags.m30 = 0;
        player->mD50 = -1;
        player->mD4C = 0;
        if (player->mRef.mChara != 0) {
            ((PPChara_unk*)player->mRef.mChara)->m9F8 = 0;
        }
    }
}

// inferred: the game and the characters of the players are ready
// NONMATCHING: 99% - r3/r4 of the loop (offset and character) swapped, as in fn_804EB870
extern "C" int fn_804DF404(PPManager2_unk* mgr) {
    if ((mgr->mGame->mFlags & 2) == 2 && mgr->m96B4.IsLoading() == 0 && mgr->m96C0.IsLoading() == 0) {
        for (int i = 0; i < mgr->mPlayers.mNum; i++) {
            CharaTask* chara = mgr->mPlayers.mData[i].mRef.mChara;
            if (chara != 0 && (chara->mFlags & 2) != 2) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

// inferred: comparator of the players by score (higher first)
extern "C" int fn_804E09AC(void* unused, const PPPlayer_unk* a, const PPPlayer_unk* b) {
    return a->mD58 > b->mD58;
}

extern "C" void* fn_80211480(int code); // inferred: entry of an item code
// inferred: adds the items won in the game to the save data and saves
extern "C" void fn_804E49A4(PPManager2_unk* mgr) {
    if (mgr->m9674 > 0 && fn_80211480(mgr->m9674) != 0 && mgr->mSaveLoad->m48 != 0 &&
        ((GameSaveInventory_unk*)&mgr->mSaveLoad->m48->mSub)->fn_803BD2B0(mgr->m9674, mgr->m9678) != 0) {
        mgr->mSaveLoad->fn_8050272C();
    }
}

// inferred: a value of the save data, 0x19640 when there is no save data
extern "C" int fn_804E4A20(PPManager2_unk* mgr) {
    MP::SaveLoad* save = mgr->mSaveLoad;
    bool ok = false;
    if (save != 0 && save->m48 != 0) {
        ok = true;
    }
    if (ok) {
        return ((int*)save->m48)[1];
    }
    return 0x19640;
}

// inferred: result of the game for the player (0, 1 or 2) by game mode (called out of line by fn_804E4D70)
extern "C" __declspec(noinline) int fn_804E4A60(PPManager2_unk* mgr) {
    int result;
    if (mgr->mMode == 0) {
        if (mgr->m2B84 == 0) {
            result = 0;
        } else if (mgr->m2B84 == 1 && mgr->m966C == 5 && fn_804E4C34(mgr) != 0) {
            result = 2;
        } else {
            result = 1;
        }
    } else if (mgr->mMode == 1) {
        if (mgr->m2B84 == 0) {
            result = 0;
        } else if (mgr->m2B84 == 1) {
            result = 2;
        } else {
            result = 1;
        }
    } else {
        result = mgr->m2B84 == 3 ? 0 : 2;
    }
    return result;
}

// NONMATCHING: 89.78% - the original computes the address of the previous slot with mulli (here it is strength
// reduced into the loop pointer)
// inferred: rank of `target` in the ranking m2B8C (game mode 2: m2B80 != 3)
extern "C" int fn_804E4B44(PPManager2_unk* mgr, PPPlayer_unk* target) {
    int result = 0;
    if (mgr->mMode != 2) {
        if (mgr->mMode != 1 || fn_804E4C5C(mgr) != 3) {
            int rank = 0;
            int run = 0;
            for (int i = 0; i < mgr->m2B88; i++) {
                if (i > 0 && mgr->m2B8C[i].mD58 != mgr->m2B8C[i - 1].mD58) {
                    rank += run;
                    run = 0;
                }
                if (mgr->m2B8C[i].mRef.mChara == target->mRef.mChara) {
                    result = rank;
                }
                run++;
            }
        }
    } else {
        result = mgr->m2B80 != 3;
    }
    return result;
}

// inferred: fn_804E4B44 on the last slot of m2B8C (-1 when there is none)
extern "C" int fn_804E4C34(PPManager2_unk* mgr) {
    if (mgr->m2B88 != 0) {
        return fn_804E4B44(mgr, &mgr->m2B8C[mgr->m2B88 - 1]);
    }
    return -1;
}

extern "C" int fn_804E4A60(PPManager2_unk* mgr);
extern "C" int fn_805076F8(void* table, int mode, int value, int arg);

// inferred: value of a player (m967C for the players marked m30)
extern "C" int fn_804E4D70(PPManager2_unk* mgr, PPPlayer_unk* player) {
    if (player != 0 && player->mFlags.m30) {
        return mgr->m967C;
    }
    return fn_805076F8(mgr->m610, mgr->mMode, mgr->m55C, fn_804E4A60(mgr));
}

// inferred: enters the ranking (state 35, or 37 when mode is not 1); not in local play
extern "C" int fn_804E4DD4(PPManager2_unk* mgr, unsigned int kind, unsigned int mode) {
    atn::net::Manager* net = lbl_8087F628;
    if (net->mC215 == 1 && net->mC218 == 1) {
        return 0;
    }
    if (lbl_8087F5A4->fn_8050C840(kind) == 0) {
        return 0;
    }
    lbl_8087F5A4->fn_8050C1D8(&lbl_8087F628->mFriendList);
    fn_804FB220(&mgr->mStateVar, mode == 1 ? 35 : 37);
    return 1;
}

// inferred: leaves the ranking and returns to the state 10
extern "C" void fn_804E4E74(PPManager2_unk* mgr) {
    lbl_8087F5A4->fn_8050C7F0();
    fn_804FB220(&mgr->mStateVar, 10);
}

// inferred: float packed in 16 bits (sign, 5-bit exponent, 10-bit mantissa); names invented
inline short PackHalf_unk(float f) {
    unsigned int bits = *(unsigned int*)&f;
    if (bits == 0) {
        return 0;
    }
    int e = ((bits >> 23) & 0xFF) - 0x70;
    if (e < 0) {
        return 0;
    }
    if (e > 31) {
        e = 31;
    }
    return (short)(((bits >> 16) & 0x8000) | ((e << 10) & 0x7C00) | ((bits >> 13) & 0x3FF));
}
struct PPHalfVec_unk {
    short x;
    short y;
    short z;
};
// inferred: state of a character sent by the messages 0x1017 (own character, 0x35 bytes) and 0x101E
struct PPCharaState_unk {
    atn::Vector3 mPos;
    atn::Vector3 mMove;
    PPHalfVec_unk mNode;
    PPHalfVec_unk mFA4;
    float mRotY;
    float mTurn;
    PackedMotion_unk mMotion;
};
struct PPCharaState2_unk {
    atn::Vector3 mPos;
    atn::Vector3 mMove;
    PPHalfVec_unk mNode;
    PPHalfVec_unk mFA4;
    float mRotY;
    float mTurn;
    int mIndex;
    unsigned char mIsChara;
    PackedMotion_unk mMotion;
};

extern "C" __declspec(noinline) inline atn::Vector3* fn_804E8090(CharaTask* chara, int i) {
    return &chara->m13B8[i];
}
extern "C" __declspec(noinline) inline int fn_804E80A0(CharaTask* chara) {
    return chara->mC4C;
}
extern "C" __declspec(noinline) inline float fn_804E80A8(CharaTask* chara, int i) {
    return chara->m13D0[i];
}
extern "C" __declspec(noinline) inline int fn_804E80B8(CharaTask* chara) {
    return chara->m524;
}
extern "C" __declspec(noinline) inline float fn_804E80C0(CharaTask* chara) {
    return chara->m588;
}
extern "C" __declspec(noinline) inline void fn_804E80C8(PPHalfVec_unk* dst, const atn::Vector3* v) {
    dst->x = PackHalf_unk(v->x);
    dst->y = PackHalf_unk(v->y);
    dst->z = PackHalf_unk(v->z);
}
extern "C" __declspec(noinline) inline atn::Vector3* fn_804E81C4(CharaTask* chara) {
    return &chara->mFA4;
}
extern "C" void fn_804E7434(PPCharaEntry_unk* dst, const PPCharaEntry_unk* src);
extern "C" int fn_804E650C(CharaTask* chara);
extern "C" int fn_804CF424(ModelSlotsN_unk* model);
extern "C" void fn_804CF374(PackedMotion_unk* out, atn::Model* model, int index);

extern "C" int fn_804E650C(CharaTask* chara) {
    return chara->mFlag15;
}

// inferred: sends the state of the own character, of the characters of this aid and of the other
// characters (messages 0x1017 and 0x101E)
// NONMATCHING: draft - written for the weak copies of its accessors (0x804E8090-0x804E82BC)
extern "C" void fn_804E7AC0(PPManager2_unk* mgr) {
    fn_804EB870(mgr);
    CharaTask* own = ((PlayerManager*)fn_8000D9E8())->GetHead_unk();
    int slot = fn_804CF424((ModelSlotsN_unk*)own->GetModel_unk());
    if (own->GetModel_unk()->fn_80139F2C(slot) >= 0) {
        PPNetMsg_unk<PPCharaState_unk, 0x35> msg(0x1017);
        PPCharaState_unk* data = msg.GetData_unk();
        data->mPos = *own->fn_8013C38C();
        data->mMove = *fn_804E8090(own, 0) - *fn_804E8090(own, 1);
        if (own->fn_8013C3A8() != 0 && fn_804E80A0(own) < 3) {
            data->mMove *= 0.1f;
        }
        data->mRotY = own->GetRot_unk()->y;
        data->mTurn = atn::WrapAngle_unk(fn_804E80A8(own, 0) - fn_804E80A8(own, 1));
        int node = fn_804E80B8(own);
        atn::Vector3 pos((&own->GetModel_unk()->fn_80147A00()[node])->mPos);
        pos.z -= fn_804E80C0(own);
        fn_804E80C8(&data->mNode, &pos);
        fn_804E80C8(&data->mFA4, fn_804E81C4(own));
        fn_804CF374(&data->mMotion, own->GetModel_unk(), slot);
        fn_804D1694();
        fn_804AE3BC()->fn_8050E094(-1, 0x1017, data, 0);
    }
    for (int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
        if (fn_804EB3CC(mgr, i) == 0) {
            continue;
        }
        PPPlayer_unk* player = fn_804D5F0C(mgr, i);
        if (!player->mFlags.mActive || (player->mAid & 0xF) != fn_804D6198(fn_804D1694())) {
            continue;
        }
        CharaTask* chara = player->mRef.mChara;
        if (chara == 0 || chara->IsFrozen_unk() != 0) {
            continue;
        }
        int slot = fn_804CF424((ModelSlotsN_unk*)chara->GetModel_unk());
        if (chara->GetModel_unk()->fn_80139F2C(slot) < 0) {
            continue;
        }
        PPNetMsg_unk<PPCharaState2_unk, 0x3A> msg(0x101E);
        PPCharaState2_unk* data = msg.GetData_unk();
        data->mPos = *chara->fn_8013C38C();
        data->mMove = *fn_804E8090(chara, 0) - *fn_804E8090(chara, 1);
        if (chara->fn_8013C3A8() != 0 && fn_804E80A0(chara) < 3) {
            data->mMove *= 0.1f;
        }
        data->mRotY = chara->GetRot_unk()->y;
        data->mTurn = atn::WrapAngle_unk(fn_804E80A8(chara, 0) - fn_804E80A8(chara, 1));
        int node = fn_804E80B8(chara);
        atn::Vector3 pos((&chara->GetModel_unk()->fn_80147A00()[node])->mPos);
        pos.z -= fn_804E80C0(chara);
        fn_804E80C8(&data->mNode, &pos);
        fn_804E80C8(&data->mFA4, fn_804E81C4(chara));
        fn_804CF374(&data->mMotion, chara->GetModel_unk(), slot);
        data->mIndex = player->mAid;
        data->mIsChara = 0;
        fn_804D1694();
        fn_804AE3BC()->fn_8050E094(-1, 0x101E, data, 0);
    }
    for (int i = 0; i < fn_804D810C(&mgr->mCharas); i++) {
        PPCharaEntry_unk entry;
        fn_804E7434(&entry, fn_804D64A4(&mgr->mCharas, i));
        CharaTask* chara = entry.mRef.mChara;
        if (!entry.mActive || chara == 0) {
            continue;
        }
        if (chara->IsFrozen_unk() != 0 && fn_804E650C(chara) == 0) {
            continue;
        }
        int slot = fn_804CF424((ModelSlotsN_unk*)chara->GetModel_unk());
        if (chara->GetModel_unk()->fn_80139F2C(slot) < 0) {
            continue;
        }
        PPNetMsg_unk<PPCharaState2_unk, 0x3A> msg(0x101E);
        PPCharaState2_unk* data = msg.GetData_unk();
        data->mPos = *chara->fn_8013C38C();
        data->mMove = *fn_804E8090(chara, 0) - *fn_804E8090(chara, 1);
        if (chara->fn_8013C3A8() != 0 && fn_804E80A0(chara) < 3) {
            data->mMove *= 0.1f;
        }
        data->mRotY = chara->GetRot_unk()->y;
        data->mTurn = atn::WrapAngle_unk(fn_804E80A8(chara, 0) - fn_804E80A8(chara, 1));
        int node = fn_804E80B8(chara);
        atn::Vector3 pos((&chara->GetModel_unk()->fn_80147A00()[node])->mPos);
        pos.z -= fn_804E80C0(chara);
        fn_804E80C8(&data->mNode, &pos);
        fn_804E80C8(&data->mFA4, fn_804E81C4(chara));
        fn_804CF374(&data->mMotion, chara->GetModel_unk(), slot);
        data->mIndex = i;
        data->mIsChara = 1;
        fn_804D1694();
        fn_804AE3BC()->fn_8050E094(-1, 0x101E, data, 0);
    }
}


extern "C" bool fn_804EA520(PPManager2_unk* mgr) {
    return mgr->mState == 30;
}

// inferred: setters of the game settings, sent to the other players when `send` is set (messages
// 0x1001, 0x1002, 0x1033, 0x1003 and 0x103C)
extern "C" void fn_804EA534(PPManager2_unk* mgr, int value, int send) {
    if (send) {
        NetMsgI_unk<int> msg(0x1001);
        msg.mData = value;
        fn_804AE3BC()->fn_8050E094(-1, 0x1001, &msg.mData, 0);
    }
    mgr->m55C = value;
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

extern "C" void fn_804EA608(PPManager2_unk* mgr, int value, int send) {
    if (send) {
        NetMsgI_unk<int> msg(0x1002);
        msg.mData = value;
        fn_804AE3BC()->fn_8050E094(-1, 0x1002, &msg.mData, 1);
    }
    mgr->m564 = value;
}

extern "C" void fn_804EA6B0(PPManager2_unk* mgr, signed char value, int send) {
    if (send) {
        NetMsgI_unk<signed char> msg(0x1033);
        msg.mData = value;
        fn_804AE3BC()->fn_8050E094(-1, 0x1033, &msg.mData, 0);
    }
    mgr->m5A0 = value;
}

extern "C" void fn_804EA75C(PPManager2_unk* mgr, int value, int send) {
    if (send && mgr->m568 != value) {
        NetMsgI_unk<int> msg(0x1003);
        msg.mData = value;
        fn_804AE3BC()->fn_8050E094(-1, 0x1003, &msg.mData, 0);
    }
    mgr->m568 = value;
}

extern "C" void fn_804EA810(PPManager2_unk* mgr, int value, int send) {
    if (send) {
        NetMsgI_unk<unsigned char> msg(0x103C);
        msg.mData = value;
        fn_804AE3BC()->fn_8050E094(-1, 0x103C, &msg.mData, 0);
    }
    mgr->m5A8 = value;
}

extern "C" int fn_804EAA50(PPManager2_unk* mgr) {
    return lbl_8087F628->m25C;
}

extern "C" void fn_804EAA5C(PPManager2_unk* mgr, int value) {
    lbl_8087F628->m25C = value;
}

extern "C" void fn_804EAA68(PPManager2_unk* mgr, int value) {
    atn::net::Manager* net = lbl_8087F628;
    net->m264 = value >= 8 ? 8 : value;
}

// inferred: kind (1 none, 2 player, 3 character of the list) and index of a character, in the high half
// NONMATCHING: 96.63% - registers of the inlined searches
extern "C" PPPlayer_unk* fn_804EB7C4(PPManager2_unk* mgr, CharaTask* chara);
union PPCharaId_unk {
    struct {
        unsigned char mKind;
        unsigned char mIndex;
    } m;
    unsigned short mRaw;
};
extern "C" unsigned int fn_804EAEE0(PPManager2_unk* mgr, CharaTask* chara) {
    PPCharaId_unk id;
    id.m.mKind = 0;
    id.m.mIndex = 0;
    if (chara == 0) {
        id.m.mKind = 1;
        return id.mRaw << 16;
    }
    PPPlayer_unk* player = fn_804EB7C4(mgr, chara);
    if (player != 0) {
        id.m.mKind = 2;
        id.m.mIndex = player->mAid;
        return id.mRaw << 16;
    }
    for (int i = 0; i < mgr->mCharas.mNum; i++) {
        if (mgr->mCharas.mData[i].mRef.mChara == chara) {
            id.m.mKind = 3;
            id.m.mIndex = i;
            return id.mRaw << 16;
        }
    }
    return id.mRaw << 16;
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

// inferred: this machine is the server
extern "C" int fn_804EB1AC(PPManager2_unk* mgr) {
    unsigned char aid = lbl_8087F628->GetMyAid_unk();
    return aid == lbl_8087F628->GetServerAid_unk();
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

// inferred: block `index` of the slot of a character (0 when unused)
extern "C" PPEquip_unk* fn_804EB650(PPManager2_unk* mgr, CharaTask* chara, int index) {
    PPPlayer_unk* player = FindByChara_unk(mgr, chara);
    if (player == 0) {
        player = mgr->mOwnPlayer;
        if (player == 0) {
            return 0;
        }
    }
    switch (index) {
    case 0:
        if (player->mEquips[0].m00 != -1) {
            return &player->mEquips[0];
        }
        return 0;
    case 1:
        if (player->mEquips[1].m00 != -1) {
            return &player->mEquips[1];
        }
        return 0;
    case 2:
        if (player->mEquips[2].m00 != -1) {
            return &player->mEquips[2];
        }
        return 0;
    case 3:
        if (player->mEquips[3].m00 != -1) {
            return &player->mEquips[3];
        }
        return 0;
    }
    return 0;
}

// inferred: value `index` of the slot of a character (-1 without a slot)
extern "C" int fn_804EB750(PPManager2_unk* mgr, CharaTask* chara, int index) {
    PPPlayer_unk* player = FindByChara_unk(mgr, chara);
    if (player == 0) {
        player = mgr->mOwnPlayer;
        if (player == 0) {
            return -1;
        }
    }
    return player->mD38[index];
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
    for (int i = 0; i < mgr->mCharas.mNum; i++) {
        if (mgr->mCharas.mData[i].mRef.mChara == chara) {
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
        return &mgr->mCharas.mData[index];
    }
    return 0;
}

// inferred: slot of the own player
// NONMATCHING: 99.39% - r4/r5 of the loop (index and aid) swapped
extern "C" PPPlayer_unk* fn_804EB870(PPManager2_unk* mgr) {
    unsigned char aid = lbl_8087F628->GetMyAid_unk();
    for (int i = 0; i < mgr->mPlayers.mNum; i++) {
        PPPlayer_unk* player = &mgr->mPlayers.mData[i];
        if (player->mFlags.mActive == 1 && aid == player->mAid) {
            return player;
        }
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

// inferred: applies the state of a character received from another player (only in the game, state 30)
extern "C" void fn_804EE904(MpCharaRef_unk* ref) {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        unsigned char* data = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (data[0x14] != lbl_8087F628->GetMyAid_unk() && ref->mChara != 0) {
            fn_804CF640(data, ref->mChara);
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

extern "C" int fn_804F1060() {
    return 6;
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
    if (index < 0 || ((PPManager2_unk*)lbl_8087F610)->mCharas.mNum <= index) {
        return;
    }
    PPCharaEntry_unk* entry = &((PPManager2_unk*)lbl_8087F610)->mCharas.mData[index];
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

// inferred: payload of the message 0x1043 as read (signed ids); names invented
struct PPMsgCharsIn_unk {
    short mIds[8];
    int m10;
};
// inferred: message 0x1043: characters of the players and m2B80 (0x10 = team game)
// NONMATCHING: 99.66% - order of the two pointer increments of the loop
extern "C" void fn_804F4E40() {
    PPMsgCharsIn_unk* data = (PPMsgCharsIn_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    for (int i = 0; i < 8; i++) {
        PPPlayer_unk* player = fn_804D5F0C((PPManager2_unk*)lbl_8087F610, i);
        if (player != 0) {
            player->mDC.fn_80501288((CharaTask*)data->mIds[i]);
        }
    }
    ((PPManager2_unk*)lbl_8087F610)->m2B80 = data->m10;
    ((PPManager2_unk*)lbl_8087F610)->m534 = data->m10 == 1;
    ((PPManager2_unk*)lbl_8087F610)->m50C = 3;
}

extern "C" void fn_804F4F40(unsigned int aid) {
    if (aid < 8) {
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        mgr->m9A34[(unsigned char)aid] = 1;
    }
}

// inferred: starts the game (state 29) and the cockpit
extern "C" void fn_804F4F64() {
    fn_804FB220(&((PPManager2_unk*)lbl_8087F610)->mStateVar, 29);
    MP::D2AMpMenu::sInstance->fn_804AF520(16);
    MP::D2AMpMenuCockpit::sInstance->fn_800D246C(0);
    MP::D2AMpMenuCockpit::sInstance->fn_804B50C0(1);
    MP::D2AMpMenuCockpit::sInstance->fn_804B6204(((PPManager2_unk*)lbl_8087F610)->m564);
}

// inferred: payload of the message 0x1035 (9 bytes: a byte and the time of the game); names invented
struct PPMsgTime_unk {
    unsigned char m00[9];
};
// inferred: answers the time of the game to `aid` (message 0x1035)
extern "C" void fn_804F4FC4(int aid) {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    unsigned char* in = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    long long time = OSGetTime() - ((PPManager2_unk*)lbl_8087F610)->mStartTime;
    NetMsgI_unk<PPMsgTime_unk, 9> msg(0x1035);
    unsigned char* data = msg.mData.m00;
    *(long long*)(data + 1) = time;
    data[0] = in[0];
    fn_804AE3BC()->fn_8050E094(aid, 0x1035, data, 1);
}

// inferred: message 0x3E: the lobby is told to refresh (mF90)
extern "C" void fn_804F50C4(unsigned int aid) {
    MP::D2AMpMenuLobby* lobby = MP::D2AMpMenuLobby::sInstance;
    if (lobby != 0) {
        lobby->mF90 = 1;
    }
}

// inferred: queue of 32 requests at 0x48 (0x18 bytes each); names invented
struct PPRequest_unk {
    int mUsed;
    int m04;
    int m08;
    unsigned char m0C[0x14 - 0xC];
    unsigned char m14;
    unsigned char m15;
    unsigned char m16[0x18 - 0x16];
};
struct PPRequests_unk {
    unsigned char m00[0x48];
    PPRequest_unk mRequests[32];
};
// inferred: free request of the queue
static inline PPRequest_unk* FindFreeRequest_unk(PPRequests_unk* queue) {
    for (int i = 0; i < 32; i++) {
        if (queue->mRequests[i].mUsed == 0) {
            return &queue->mRequests[i];
        }
    }
    return 0;
}

extern "C" void fn_804F5A94(PPManager2_unk* mgr) {}

// inferred: adds a request (0 when the queue is full)
extern "C" int fn_804F5A98(PPRequests_unk* queue, unsigned char a, int b, unsigned char c) {
    PPRequest_unk* req = FindFreeRequest_unk(queue);
    if (req == 0) {
        return 0;
    }
    req->mUsed = 1;
    req->m04 = 0;
    req->m14 = a;
    req->m08 = b;
    req->m15 = c;
    return 1;
}

// inferred: ages the requests: after the timer a new request waits 150 more frames, an old one is freed
extern "C" void fn_804F5FCC(PPRequests_unk* queue) {
    for (int i = 0; i < 32; i++) {
        if (queue->mRequests[i].mUsed != 0) {
            if (--queue->mRequests[i].m08 < 0) {
                if (queue->mRequests[i].m04 == 0) {
                    queue->mRequests[i].m04 = -1;
                    queue->mRequests[i].m08 = 150;
                } else {
                    queue->mRequests[i].mUsed = 0;
                }
            }
        }
    }
}

extern "C" void fn_804F60D4() {}

extern "C" void fn_804F60D8() {}

extern "C" void fn_804F60DC() {}

// inferred: requests of the save/load task of the manager (type 0 and 1)
extern "C" int fn_804F7EC8(void* data, int size) {
    MP::SaveLoad* saveLoad = ((PPManager2_unk*)lbl_8087F610)->mSaveLoad;
    if (saveLoad != 0) {
        return saveLoad->fn_80502870(0, data, size);
    }
    return 0;
}

extern "C" int fn_804F7EF0(void* data, int size) {
    MP::SaveLoad* saveLoad = ((PPManager2_unk*)lbl_8087F610)->mSaveLoad;
    if (saveLoad != 0) {
        return saveLoad->fn_80502870(1, data, size);
    }
    return 0;
}

extern "C" int fn_804F7F18() {
    PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
    if (mgr->mSaveLoad != 0) {
        return mgr->mSaveLoad->fn_80502848();
    }
    return 0;
}

// inferred: error of the save/load task of the manager
extern "C" int fn_804F7F34() {
    MP::SaveLoad* saveLoad = ((PPManager2_unk*)lbl_8087F610)->mSaveLoad;
    if (saveLoad != 0) {
        return saveLoad->mError;
    }
    return 0;
}

// inferred: payload of the message 0x1008 (flags of a player slot); names invented
struct PPMsgFlags_unk {
    unsigned char m00[0x1C];
    unsigned int mFlags;
};
// inferred: sends the flags of a player slot
extern "C" void fn_804FAA7C(PPManager2_unk* mgr, unsigned int flush, PPPlayer_unk* player) {
    NetMsgI_unk<PPMsgFlags_unk> msg(0x1008);
    PPMsgFlags_unk* data = &msg.mData;
    data->mFlags = player->mFlagWord;
    fn_804AE3BC()->fn_8050E094(-1, 0x1008, data, 1);
    if (flush == 1) {
        fn_804AE3BC()->fn_8050DDDC(-1, 1);
    }
}

// inferred: flag m976C once the network manager reached state 8
// NONMATCHING: 99.46% - registers of the inlined search fn_804DD158 (aid/offset/count in r4/r3/r5 in the
// original), as fn_804DD1A8
// inferred: sets the state of MP::Manager2; entering 12 (end of the match) resets the match values and
// clears the flag m01 of the server's slot
extern "C" void fn_804FB220(PPStateVar_unk* state, int value) {
    if (value == 12 && state->mLast != 12) {
        ((PPManager2_unk*)lbl_8087F610)->m504 = -1;
        ((PPManager2_unk*)lbl_8087F610)->m508 = 0xE2D;
        ((PPManager2_unk*)lbl_8087F610)->m50A = 0;
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        if (lbl_8087F628->GetMyAid_unk() == lbl_8087F628->GetServerAid_unk()) {
            PPPlayer_unk* player = fn_804DD158(mgr, lbl_8087F628->GetServerAid_unk());
            if (player != 0) {
                player->mFlags.m01 = 0;
            }
        }
        ((PPManager2_unk*)lbl_8087F610)->m975C = 300;
        ((PPManager2_unk*)lbl_8087F610)->m99F0[0] = 0;
        ((PPManager2_unk*)lbl_8087F610)->m99F0[1] = 0;
        ((PPManager2_unk*)lbl_8087F610)->m99F0[2] = 0;
        ((PPManager2_unk*)lbl_8087F610)->m99F0[3] = 0;
        ((PPManager2_unk*)lbl_8087F610)->m999C = 0;
        ((PPManager2_unk*)lbl_8087F610)->m999D = 0;
        lbl_8087F628->fn_8050A5EC();
    }
    if (value == 11) {
        ((PPManager2_unk*)lbl_8087F610)->m99EC = state->mLast != 11 ? 60 : 0;
    }
    if (state->mLast != value) {
        state->mLast = value;
    }
    state->mValue = value;
}

// inferred: ends the game with an error code (state 43)
extern "C" void fn_804FB470(PPManager2_unk* mgr, int code, int arg) {
    if (lbl_8087F628->m25C == 1 && MP::D2AMpMenuFriendRoom::sInstance != 0 && MP::D2AMpMenuFriendRoom::sInstance->m88 == 8) {
        MP::D2AMpMenuFriendRoom::sInstance->fn_804BA350(9);
    }
    mgr->m9750 = -1;
    if (code == -2) {
        fn_804FAD48(mgr, -1);
    }
    if ((unsigned int)(mgr->m9750 - 0xA1) <= 1) {
        arg = 9;
    }
    fn_804FB220(&mgr->mStateVar, 43);
    mgr->m9754 = code;
    mgr->m9758 = arg;
}

extern "C" void fn_804FB530(PPManager2_unk* mgr) {
    if (((PPNet_unk*)lbl_8087F628)->mState < 8) {
        return;
    }
    mgr->m976C = 1;
}

extern "C" int fn_800A5920(void* pad, int port, int all);
extern "C" int fn_800A59F0(void* pad, int port);
extern "C" int fn_800A5AA4(void* pad, int port);
// inferred: marks the inputs of the player (button, stick, other) once they were used
extern "C" void fn_804FB550(PPManager2_unk* mgr) {
    if (mgr->m99F0[0] == 0 && (fn_800A5920(GetPad_unk(), 0, 0) != 0 || fn_800A59F0(GetPad_unk(), 0) != 0)) {
        mgr->m99F0[0] = 1;
    }
    if (mgr->m99F0[1] == 0 && fn_800A59F0(GetPad_unk(), 0) != 0) {
        mgr->m99F0[1] = 1;
    }
    if (mgr->m99F0[2] == 0 && fn_800A5AA4(GetPad_unk(), 0) != 0) {
        mgr->m99F0[2] = 1;
    }
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

extern "C" void fn_804FB668(PPManager2_unk* mgr) {
    mgr->m5A4 = 0;
}

// inferred: flag m01 of the slot of the server
// NONMATCHING: 98.47% - registers of the inlined search (as fn_804DD1A8)
extern "C" unsigned int fn_804FC010(PPManager2_unk* mgr) {
    PPPlayer_unk* player = fn_804DD158(mgr, lbl_8087F628->GetServerAid_unk());
    if (player != 0) {
        return player->mFlags.m01;
    }
    return 0;
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

extern "C" int fn_804FC510() {
    return 0;
}

extern "C" int fn_804FC518() {
    return 0;
}

extern "C" int fn_804FC520() {
    return 0;
}

// inferred: the character finished loading
extern "C" bool fn_804FC580(CharaTask* chara) {
    return chara->fn_8013655C() == 0;
}

extern "C" void fn_804FC7DC(CharaTask* chara) {
    chara->fn_80149A30();
}

// inferred: warp points of the field where a player of team `team` can appear (by game mode)
// NONMATCHING: 96.86% - registers of the loop and one branch of the mode 0 test
extern "C" int fn_804FC7E0(PPManager2_unk* mgr, atn::FixedArray_unk<gs::WarpPoint*, 32>* list, int team) {
    if (mgr->mGame == 0 || mgr->mGame->m10D8 == 0 || (mgr->mGame->m10D8->mFlags & 2) != 2) {
        return 0;
    }
    GameScene* scene = mgr->mGame->m10D8;
    for (unsigned int i = 0; i < scene->m78.mSize; i++) {
        int mode = mgr->mMode;
        gs::WarpPoint* point = &scene->m78.mData[i];
        if (mode == 0) {
            if (point->mId < 100 || point->mId > 120) {
                continue;
            }
        } else {
            if (point->mId >= 100) {
                continue;
            }
            if (mode == 1) {
                if (team == 2) {
                    if (point->mId >= 50) {
                        continue;
                    }
                } else if (team == 1) {
                    if (point->mId < 50) {
                        continue;
                    }
                }
            }
            if (mode == 2 && point->mId >= 10) {
                continue;
            }
        }
        list->push_back(point);
    }
    return 1;
}
