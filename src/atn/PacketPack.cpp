// MP::Manager2 / MP::ManagerMP (multiplayer manager) and the static helpers around them; the
// atn::net::PacketPack methods themselves are in atn/PPDATAU.cpp. One translation unit: a single
// string pool and a single .sdata2 pool for the whole range (see docs/auto-20261102.md).
#include "atn/Task.h"
#include "math/VecAssign_unk.h"
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

extern "C" GameTask* lbl_8087F430;

extern "C" void fn_804CF190(atn::Task* task) {
    fn_800D2338(task);
}

extern "C" long long fn_804CF194() {
    return OSGetTime();
}

// NONMATCHING: 99.37% - FPRs of the two frame differences
// inferred: applies a packed motion state to slot `index` of a model
extern "C" void fn_804CF198(const PackedMotion_unk* in, atn::Model* model, int index) {
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
    const ModelSlot_unk& slot = ((ModelSlots_unk*)model)->mSlots[index];
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
    unsigned char m0678[0x7D8 - 0x678];
    float m7D8;       // inferred: set from the state of message 0x1A (fn_804EE9F0)
    unsigned char m07DC[0x7E0 - 0x7DC];
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
namespace MP {
struct Manager2;
}
typedef MP::Manager2 PPManager2_unk;
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

#include "game/BattleTask.h"
extern "C" BattleTask* lbl_8087F048; // inferred: battle manager (as game/BattleCharaTask.cpp)
// inferred: battle manager (weak copy 0x8013A194 of another file)
inline BattleTask* fn_8013A194() {
    return lbl_8087F048;
}
// inferred: the targets of the slot and of the list entry of `chara` (defined after MP::Manager2, inlined)
static inline void PPSetTargets_unk(CharaTask* chara, const PackedChara_unk* in);
// NONMATCHING: 98.41% - scheduling of the two float values, a branch of the flags and reloads of lbl_8087F610 in
// the inlined searches
// inferred: applies the packed state of a character (fn_804CF468): part, state, values mFB8/mFBC, hit, flags,
// the status bits of m13 (each one with its effect of the battle), the motion mode and the targets
extern "C" void fn_804CF640(const PackedChara_unk* in, CharaTask* chara) {
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 10) {
        return;
    }
    if (in->m00 != chara->m674) {
        if (in->m00 == -1) {
            chara->fn_8014EEC4(0);
        } else {
            chara->fn_8014DEE4(in->m00, 0, 0);
        }
    }
    chara->fn_8016E970((unsigned short)in->m04);
    chara->mFB8 = in->m08;
    chara->mFBC = in->m0A;
    chara->m560 = (unsigned short)in->m06;
    chara->SetHit_unk(in->m0C > 0 ? fn_80219E6C(in->m0C) : 0);
    if (!chara->mStatus.IsState_unk(0x20)) {
        chara->mFlag5 = in->m12_7 == 1;
        chara->fn_80155DAC(in->m12_6 == 1);
        chara->mFlag31 = in->m12_5 == 1;
        chara->mFlag10 = in->m12_4 == 1;
        chara->mFlagB8 = in->m12_3 == 1;
        if (in->m12_2 == 1) {
            chara->mColli.mFlags |= 1;
        } else {
            chara->mColli.mFlags &= ~1;
        }
        int c48 = 0;
        if (in->m12_0 == 1) {
            c48 = 3;
        }
        chara->mC48 = c48;
        if (in->m13_7 && !chara->mStatus.IsState_unk(0x80000)) {
            chara->mStatus.fn_80133130(0x80000, 100000, 0);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107E58(&chara->mMotion);
                fn_8013A194()->fn_80107DA4(&chara->mMotion, &chara->mMotion, chara->vf90(), chara->vf94());
            }
        } else if (!in->m13_7 && chara->mStatus.IsState_unk(0x80000)) {
            chara->mStatus.fn_8013322C(0x80000);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107E58(&chara->mMotion);
            }
        }
        if (in->m13_6 && !chara->mStatus.IsState_unk(4)) {
            chara->mStatus.fn_80133130(4, 100000, 0);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107D94(&chara->mMotion);
                fn_8013A194()->fn_80107CEC(&chara->mMotion, &chara->mMotion, chara->vf90());
            }
        } else if (!in->m13_6 && chara->mStatus.IsState_unk(4)) {
            chara->mStatus.fn_8013322C(4);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107D94(&chara->mMotion);
            }
        }
        if (in->m13_5 && !chara->mStatus.IsState_unk(0x2000000)) {
            chara->mStatus.fn_80133130(0x2000000, 100000, 0);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107FC8(&chara->mMotion);
                fn_8013A194()->fn_80107F20(&chara->mMotion, &chara->mMotion, chara->vf90());
            }
        } else if (!in->m13_5 && chara->mStatus.IsState_unk(0x2000000)) {
            chara->mStatus.fn_8013322C(0x2000000);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107FC8(&chara->mMotion);
            }
        }
        if (in->m13_4 && !chara->mStatus.IsState_unk(0x400000)) {
            chara->mStatus.fn_80133130(0x400000, 100000, 0);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107F10(&chara->mMotion);
                fn_8013A194()->fn_80107E68(&chara->mMotion, &chara->mMotion);
            }
        } else if (!in->m13_4 && chara->mStatus.IsState_unk(0x400000)) {
            chara->mStatus.fn_8013322C(0x400000);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107F10(&chara->mMotion);
            }
        }
        if (in->m13_3 && !chara->mStatus.IsState_unk(1)) {
            chara->mStatus.fn_80133130(1, 100000, 0);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_801079B0(&chara->mMotion);
                fn_8013A194()->fn_80107908(&chara->mMotion, &chara->mMotion);
            }
        } else if (!in->m13_3 && chara->mStatus.IsState_unk(1)) {
            chara->mStatus.fn_8013322C(1);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_801079B0(&chara->mMotion);
            }
        }
        if (in->m13_2 && !chara->mStatus.IsState_unk(0x8000)) {
            chara->mStatus.fn_80133130(0x8000, 100000, 0);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107BD8(&chara->mMotion);
                fn_8013A194()->fn_80107380(chara);
                fn_8013A194()->fn_80107B30(&chara->mMotion, &chara->mMotion);
                fn_8013A194()->fn_80107274(chara);
            }
        } else if (!in->m13_2 && chara->mStatus.IsState_unk(0x8000)) {
            chara->mStatus.fn_8013322C(0x8000);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107BD8(&chara->mMotion);
                fn_8013A194()->fn_80107380(chara);
            }
        }
        if (in->m13_0 && !chara->mStatus.IsState_unk(0x80)) {
            chara->mStatus.fn_80133130(0x80, 100000, 0);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107798(&chara->mMotion, 1);
                fn_8013A194()->fn_801076D0(&chara->mMotion, &chara->mMotion);
            }
        } else if (!in->m13_0 && chara->mStatus.IsState_unk(0x80)) {
            chara->mStatus.fn_8013322C(0x80);
            if (lbl_8087F048 != 0) {
                fn_8013A194()->fn_80107798(&chara->mMotion, 1);
            }
        }
    } else {
        chara->mColli.mFlags &= ~1;
    }
    chara->fn_8017C7F8(in->m12_2);
    if (in->m12_2 == 1) {
        chara->m54C |= 0x2000;
    } else {
        chara->m54C &= ~0x2000;
    }
    chara->mC58.mC0 = in->m12_2 != 1;
    if (in->m12_1 && in->m13_1 && !chara->mStatus.IsState_unk(0x20)) {
        if (chara->mFlag22 == 1) {
            chara->mStatus.fn_8013310C(0x20, 0);
        } else {
            chara->vf44();
        }
    }
    PPSetTargets_unk(chara, in);
}

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
        lbl_8087F430->fn_80370AE4(index, value);
    }
}

// inferred: debug keyboard test; the original passes the keyboard itself as the key state (mKeyState[-2], as
// in game/GameTask.cpp)
extern "C" int fn_804D00DC(unsigned short key) {
    atn::Keyboard* keyboard = atn::Keyboard::instance_;
    if (keyboard != 0) {
        return keyboard->Press((atn::Keyboard::Port)-2, key);
    }
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

namespace atn {
// inferred: real name of the render settings (weak copy SetClearColor__Q23atn12RenderConfigFRCQ23atn5Color
// 0x804D0304, called by vf10); the members are those of RenderSetting_unk
class RenderConfig : public RenderSetting_unk {
public:
    void SetClearColor(const Color& color) { m03C = color; }
};
} // namespace atn

extern "C" atn::RenderConfig* lbl_8087EFA8; // inferred: render settings

inline void RenderSetting_unk::SetColorMatrix(const ColorMatrixParam_unk& p) {
    m324 = p;
}

// inferred: sets a row of the color matrix member by member
inline void SetRow_unk(RsVector4_unk& row, float x, float y, float z, float w) {
    row.x = x;
    row.y = y;
    row.z = z;
    row.w = w;
}

// inferred: render settings of the multiplayer game: sepia-like color matrix, no bloom and no depth
// of field, grey clear color (SetClearColor__Q23atn12RenderConfigFRCQ23atn5Color inlined)
void fn_804D0164() {
    ColorMatrixParam_unk cm;
    cm.mEnabled = 1;
    SetRow_unk(cm.mRows[0], 0.9f, 0.1f, 0.1f, 0.0f);
    SetRow_unk(cm.mRows[1], 0.1f, 0.9f, 0.1f, 0.0f);
    SetRow_unk(cm.mRows[2], 0.1f, 0.1f, 0.9f, 0.0f);
    SetRow_unk(cm.mRows[3], 0.0f, 0.0f, 0.0f, 0.0f);
    lbl_8087EFA8->SetColorMatrix(cm);
    lbl_8087EFA8->mDof.mEnabled = 0;
    lbl_8087EFA8->m054.mEnabled = 0;
    lbl_8087EFA8->SetClearColor(RsColor_unk(0.8627f, 0.8627f, 0.8627f, 1.0f));
}

// ---- player slots of MP::Manager2 and the message handlers of the receive loop (fn_804EE040, switch
// over the message id, jumptable_807912DC) ----
#include "atn/MemoryManager.h"
#include "atn/net/PacketPack.h"
// inferred: the messages of the basic_string members are in the string pool of the original (before
// "CustomData" of __sinit), as game/GimmickManager.cpp
#include "std/string_members.h"
#include "MP/PacketPackMP.h"
#include "MP/CryptMP.h"
#include "game/HomeButtonMenu.h"
#include "game/MapManager.h"
#include "atn/WPad_unk.h"
#include "atn/SoundManager.h"
#include "atn/SoundCore.h"
#include "MP/D2AMpMenuLobby.h"
#include "game/GameVoice.h"
#include "game/GameBgm.h"
#include "game/SetConfig.h"
#include "MP/MpCharaRef_unk.h"
#include "MP/SaveLoad.h"
#include "atn/net/Manager.h"
#include "atn/SingletonNoStatic.h"
#include "atn/Camera_unk.h"
#include "game/GameScene.h"
#include "game/MapTask.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"

namespace MP {
// inferred: network manager of the multiplayer (vtable 0x807916A0, instance lbl_8087F618, also lbl_8087F628)
class ManagerMP : public atn::net::Manager, public atn::SingletonNoStatic<ManagerMP> {
public:
    virtual ~ManagerMP();
    virtual int vf0C();
    virtual atn::net::NetPlayerInfo_unk* vf10(unsigned int aid);
    virtual atn::net::NetPlayerInfo_unk* vf14(unsigned int aid);
};
} // namespace MP
// inferred: object deleted by MP::ManagerMP::vf0C (virtual destructor; type unknown)
struct PPNetObject_unk {
    virtual ~PPNetObject_unk();
};
extern "C" MP::PacketPackMP* lbl_8087F61C; // inferred: instance of MP::PacketPackMP
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
#include "MP/MpLevel_unk.h"
#include "game/ResHandle.h"

// inferred: state of a player sent by message 0x16 (0x7C bytes); names invented
struct PPPair_unk {
    int m00;
    int m04;
};
struct PPPlayerState_unk {
    PPPlayerState_unk() { memset(this, 0, sizeof(*this)); }
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
struct PPEquipBits_unk {
    unsigned short mOn : 1;   // inferred: tested by the state 19 of vf10
    unsigned short m04_14 : 1;
    unsigned short mLevel : 8;
    unsigned short m04_0 : 6;
};
// inferred: 8-byte entry of an equipment (copied as a block by fn_804F1950); names invented
struct PPEquipItem_unk {
    unsigned int m[2];
};
// inferred: the part of an equipment sent by message 0x1C (fn_804F1950); names invented
struct PPEquipHead_unk {
    PPEquipHead_unk() {
        m00 = -1;
        m04.mOn = 0;
        m08 = -1;
        for (int i = 0; i < 8; i++) {
            mIds[i] = -1;
        }
        memset(mExtra, 0, sizeof(mExtra));
    }
    int m00;
    PPEquipBits_unk m04;
    int m08;
    union {
        PPEquipItem_unk mItems[8];
        struct {
            int mIds[8];
            unsigned char mExtra[0x20];
        };
    };
};
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
// inferred: wide name of an equipment (32 characters): the assignment copies the text (wcslen + wmemcpy when
// the buffers differ, as the wide atn::static_string of game/PopupTask.h), the copy constructor the whole
// buffer, no destructor (layout from the swap 0x804E0CE4); name invented
struct PPWideName_unk {
    PPWideName_unk() { mStr[0] = 0; }
    PPWideName_unk& operator=(const PPWideName_unk& o) {
        const wchar_t* str = o.mStr;
        if (str != mStr) {
            wmemcpy(mStr, str, wcslen(str) + 1);
        }
        return *this;
    }
    wchar_t mStr[32];
};
// inferred: entry of an equipment (0x14 bytes, 32 of them at 0x50; layout from the copy in the swap
// 0x804E0CE4); names invented
struct PPEquipSlot_unk {
    PPEquipSlot_unk() : m00(-1), m02(0), m04(-1.0f), m08(1.0f), m0C(1.0f), m10(-1.0f) {}
    short m00;
    short m02;
    float m04;
    float m08;
    float m0C;
    float m10;
};
// inferred: the entries of an equipment (assigned as one block); names invented
struct PPEquipSlots_unk {
    PPEquipSlot_unk m[32];
};
// inferred: second part of an equipment (from 0x4C; the swap 0x804E0CE4 assigns it through its own source
// pointer); names invented
struct PPEquipBody_unk {
    PPEquipBody_unk() : m00(-1), m284(0) {}
    int m00;
    PPEquipSlots_unk m04;
    int m284;
    PPWideName_unk m288;
};
struct PPEquip_unk : PPEquipHead_unk {
    PPEquipBody_unk m4C;
    // inferred: copies the part sent by message 0x1C
    void Set_unk(const PPEquipHead_unk& head) {
        m00 = head.m00;
        m04 = head.m04;
        m08 = head.m08;
        for (int i = 0; i < 8; i++) {
            mItems[i] = head.mItems[i];
        }
    }
};

// inferred: flags of a player slot (MSB first); names invented
struct PPPlayerFlags_unk {
    unsigned int mActive : 1; // inferred: slot in use
    unsigned int m30 : 1;
    unsigned int m29 : 1;
    unsigned int m28 : 1;     // inferred: set and cleared by messages 0x10 and 0x11
    unsigned int m27 : 1;     // inferred: set by message 0x17
    unsigned int m26 : 1;     // inferred: set by message 0x18 on the player and its team
    unsigned int m22 : 4;     // inferred: value of message 0x12 (team, see fn_804E4C5C)
    unsigned int m14 : 8;
    unsigned int m13 : 1;     // inferred: set from the state of fn_804EE9F0
    unsigned int m12 : 1;     // inferred: set from the state of fn_804EE9F0
    unsigned int m10 : 2;
    unsigned int m06 : 4;
    unsigned int m02 : 4;
    unsigned int m01 : 1;             // inferred: cleared on the server when the match ends (fn_804FB220)
    unsigned int m00 : 1;
};

// inferred: the character reference of a player slot (null at construction; the second one, mDC, is set by
// fn_804D1F6C instead); name invented
struct PPCharaRefNull_unk : MpCharaRef_unk {
    PPCharaRefNull_unk() { mChara = 0; }
};
struct PPPlayer_unk;
extern "C" void fn_804D1F6C(PPPlayer_unk* player);
// inferred: player slot of MP::Manager2 (0xD5C bytes, array at +0x5E4); names invented
struct PPPlayer_unk {
    // inferred: weak copy 0x804D16F8 (construction of the array m2B8C in the constructor of MP::Manager2)
    PPPlayer_unk() {
        for (int i = 0; i < 4; i++) {
            mD38[i] = -1;
        }
        fn_804D1F6C(this);
    }
    ~PPPlayer_unk() {} // inferred: weak copy 0x804D1FD8 (destruction of m2B8C in the destructor of MP::Manager2)
    PPCharaRefNull_unk mRef;              // 0x00: inferred: the character of the player
    union {
        int m004[12];                 // inferred: copy of the character information (fn_804D8078)
        struct {
            int m04;
            void* m08;
            void* m0C;
        };
    };
    PPPlayerState_unk m34;            // inferred: state sent by message 0x16
    union {
        int mB0;
        int m0B0[6];
    };
    int mC8;
    unsigned char mAid;               // 0xCC: inferred: network id of the player
    union {
        PPPlayerFlags_unk mFlags;     // 0xD0
        unsigned int mFlagWord;       // inferred: the flags as a word (cleared at once)
    };
    union {
        int mD4;                      // inferred: copy of the flags kept by the server (fn_804DD9CC)
        PPPlayerFlags_unk mD4Flags;
    };
    int mD8;
    union {
        MpCharaRef_unk mDC;           // inferred: a second character reference
        int mDCScore;                 // inferred: summed as the team score in fn_804E4C5C
    };
    int mE0;
    int mE4;
    PPEquip_unk mEquip0;              // 0xE8: inferred: four members, not an array (the constructor 0x804D16F8
    PPEquip_unk mEquip1;              // builds them unrolled and the code uses constant indices only)
    PPEquip_unk mEquip2;
    PPEquip_unk mEquip3;

    int mD38[5];                      // inferred: values of message 0x15
    unsigned int mD4C;                // inferred: bits 10-11 set by message 0x2E
    short mD50;
    unsigned char m0D52;
    unsigned char mD53;
    int mD54;
    int mD58;                         // inferred: score (players of equal score share a rank)

};

namespace std {

// inferred: reallocation of the resize of the player slots (weak copies 0x804FC8CC growth, 0x804FF3D0 storage of
// the buffer, 0x804FCA34 new slots, 0x804FE2DC move of the old slots, 0x804FC9C0 buffer destructor), as the
// push_back of include/Event/EventVector_unk.h. Names invented.
template <class T>
struct __vec_pp_unk {
    static unsigned int __grow_by_unk(vector<T>& v, unsigned int n);
    // inferred: the first argument (the new size) is not used
    static void __reserve_unk(__vec_split_unk<T>& buf, unsigned int, unsigned int cap);
    static void __append_n_unk(__vec_split_unk<T>& buf, unsigned int n);
    static void __push_front_unk(__vec_split_unk<T>& buf, T* first, T* last);
};

// inferred: growth without the length check of the new capacity
template <class T>
unsigned int __vec_pp_unk<T>::__grow_by_unk(vector<T>& v, unsigned int n) {
    if (n > v.max_size() - v.mCap) {
        __msl_error("vector length error");
    }
    unsigned int cap = v.mCap;
    if (cap < v.max_size() / 3) {
        cap += __vec_max_unk((unsigned int)((cap + 1) * 3 / 5), n);
    } else if (cap < v.max_size() / 3 * 2) {
        cap += __vec_max_unk((unsigned int)((cap + 1) / 2), n);
    } else {
        cap = v.max_size();
    }
    return cap;
}

template <class T>
void __vec_pp_unk<T>::__reserve_unk(__vec_split_unk<T>& buf, unsigned int, unsigned int cap) {
    if (cap > (unsigned int)-1 / sizeof(T)) {
        __msl_error("vector length error");
    }
    buf.mData = __vec_alloc_sel_unk<T>::alloc(cap);
    buf.mCap = cap;
}

// NONMATCHING: 94.80% - registers and order of the address computation of the inlined constructor
// inferred: each new slot is a copy of a default constructed one
template <class T>
void __vec_pp_unk<T>::__append_n_unk(__vec_split_unk<T>& buf, unsigned int n) {
    T* p = buf.begin() + buf.mSize;
    for (; n != 0; n--, p++) {
        new (p) T(T());
        buf.mSize++;
    }
}

// NONMATCHING: 97.96% - registers of the member copy
template <class T>
void __vec_pp_unk<T>::__push_front_unk(__vec_split_unk<T>& buf, T* first, T* last) {
    T* p = buf.begin();
    while (last > first) {
        --p;
        --last;
        new (p) T(*last);
        buf.mStart--;
        buf.mSize++;
    }
}

// NONMATCHING: 4.40% - the original inlines the new slot (constructor and copy) and calls only the helpers of the
// reallocation; ours is over the inlining budget and calls everything (an inline helper for the reallocation
// takes the growth, the storage and the move inline and leaves an extra weak function)
// inferred: resize of the player slots (weak copy 0x804D6688, called by MP::Manager2::vf10): the new slots are
// copies of default constructed ones; the reallocation calls the helpers of __vec_pp_unk
template <>
inline void vector<PPPlayer_unk>::resize(size_type n) {
    if (n > this->mSize) {
        size_type m = n - this->mSize;
        if (m > this->mCap || this->mSize > this->mCap - m) {
            __check_grow_unk(__vec_sum_unk(this, m) - this->mCap);
            __recommend_cap_unk(size() + m);
        } else {
            PPPlayer_unk* p = this->mData + this->mSize;
            for (; m != 0; m--, p++) {
                new (p) PPPlayer_unk(PPPlayer_unk());
                this->mSize++;
            }
            return;
        }
        __vec_split_unk<PPPlayer_unk> buf(&this->mCap);
        size_type cap = __vec_pp_unk<PPPlayer_unk>::__grow_by_unk(*this, this->mSize + m - this->mCap);
        __vec_pp_unk<PPPlayer_unk>::__reserve_unk(buf, this->mSize + m, cap);
        buf.mStart = this->mSize;
        __vec_pp_unk<PPPlayer_unk>::__append_n_unk(buf, m);
        __vec_pp_unk<PPPlayer_unk>::__push_front_unk(buf, this->mData, this->mData + this->mSize);
        __vec_swap_unk(this->mCap, buf.mCap);
        __vec_swap_unk(this->mData, buf.mData);
        __vec_swap_unk(this->mSize, buf.mSize);
    } else if (n < this->mSize) {
        this->__shrink_unk(n);
    }
}

} // namespace std

// inferred: the player slots of MP::Manager2 are a std::vector (resize 0x804D6688 and the weak copies
// 0x804FC8CC-0x804FF3D0 of its growth)
typedef std::vector<PPPlayer_unk> PPPlayerArray_unk;
// inferred: slot of the team player of network id `aid` (none for a main player id); name invented
inline PPPlayer_unk* PPFindTeam_unk(PPPlayerArray_unk* players, unsigned int aid) {
    if ((aid & 0xF0) != 0) {
        for (unsigned int i = 0; i < players->size(); i++) {
            PPPlayer_unk* player = &(*players)[i];
            if (aid == player->mAid) {
                return player;
            }
        }
    }
    return 0;
}

// inferred: entry of the character list of MP::Manager2 (0xB4 bytes, array at +0x5F0); names invented
// inferred: character information copied from the character (0x30 bytes, fn_804D8078)
struct PPCharaInfo_unk {
    int m[12];
};
struct PPCharaEntry_unk {
    PPCharaEntry_unk() {} // inferred: declared (the copy of an entry goes member by member, fn_804E6DD0)
    MpCharaRef_unk mRef;
    int m04[14];                  // inferred: layout from the copies fn_804E7434 and fn_804E6DD0 (words, floats,
    float m3C;                    // words)
    float m40;
    float m44;
    float m48;
    float m4C;
    float m50;
    int m54[9];
    int m78;
    int m7C;
    PPCharaInfo_unk m80;
    unsigned int mActive : 1; // 0xB0: inferred: the entry is used (fn_804E7AC0)
    unsigned int mB0_0 : 31;
};

// inferred: the character list of MP::Manager2 is a std::vector (push_back inlined in fn_804E9C64)
typedef std::vector<PPCharaEntry_unk> PPCharaArray_unk;

// inferred: word arrays of MP::Manager2 cleared by their constructor (memset between the member initializations
// of the constructor 0x804D0328); name invented
template <int N>
struct PPWords_unk {
    PPWords_unk() { memset(m, 0, sizeof(m)); }
    int m[N];
};

// inferred: result block of MP::Manager2 at 0x2B80 (game mode, result state of the game fn_804E4A60 and the
// ranking copied by fn_804E4C5C); its destructor and the one of the ranking add the null checks of `this` of
// ~Manager2 (0x804D2308); names invented
struct PPRanking_unk : public atn::FixedArray_unk<PPPlayer_unk, 8> {
    ~PPRanking_unk() {}
};
struct PPResult_unk {
    ~PPResult_unk() {}
    int m00;
    int m04;
    PPRanking_unk m08;
};

// inferred: state variable of MP::Manager2 (value and last value set); names invented
struct PPStateVar_unk {
    PPStateVar_unk() : mValue(0), mLast(0) {}
    int mValue;
    int mLast;
};

// inferred: node of the list of MP::Manager2 at 0x99A4 (MSL list); names invented
struct PPListNode_unk {
    PPListNode_unk* mPrev;
    PPListNode_unk* mNext;
};
// inferred: MSL list: the storage and the clear live two levels below the list (three null checks of `this` in
// ~Manager2, as std::vector); names invented
struct PPListBase_unk {
    PPListBase_unk() : mCount(0) {
        mHead.mNext = &mHead;
        mHead.mPrev = &mHead;
    }
    ~PPListBase_unk() { Clear_unk(); }
    void Clear_unk() { Erase_unk(mHead.mNext, &mHead); }
    void Erase_unk(PPListNode_unk* first, PPListNode_unk* end) {
        if (first != end) {
            PPListNode_unk* last = end->mPrev;
            first->mPrev->mNext = last->mNext;
            last->mNext->mPrev = first->mPrev;
            while (first != end) {
                PPListNode_unk* node = first;
                first = first->mNext;
                delete node;
                mCount--;
            }
        }
    }
    int mCount;
    PPListNode_unk mHead;
};
struct PPListImp_unk : public PPListBase_unk {
    ~PPListImp_unk() {}
};
struct PPList_unk : public PPListImp_unk {
    ~PPList_unk() {}
};
// inferred: level data of MP::Manager2 (0x8C bytes at +0x610, given to fn_805076F8 and fn_80507310); it starts
// with the level table (count and array freed with delete[] by the destructor); names invented
struct PPLevelTable_unk : public MpLevelTable_unk {
    PPLevelTable_unk() {
        m00 = 0;
        m04 = 0;
    }
    ~PPLevelTable_unk() {
        if (m04 != 0) {
            delete[] m04;
        }
        m04 = 0;
        m00 = 0;
    }
};
struct PPLevelData_unk {
    // inferred: the constructor clears the whole object after the table (0x804D0328)
    PPLevelData_unk() { memset(this, 0, sizeof(PPLevelData_unk)); }
    PPLevelTable_unk mTable;
    unsigned char m08[0x8C - 0x8];
};
// inferred: request of the network thread (0x28 bytes at 0x99C0); names invented
struct PPThreadRequest_unk {
    PPThreadRequest_unk() : m00(0), m04(0) {
        for (int i = 0; i < 8; i++) {
            mFuncs[i] = 0;
        }
    }
    int m00;
    int m04;
    void* mFuncs[8];
};

#include "atn/FadeCommon.h"
// inferred: entries of MP::Manager2 built by its ctor (only the cleared members are known); names invented
struct PPSlot18_unk {
    PPSlot18_unk() : m04(0) {}
    int m00;
    int m04;
    unsigned char m08[0x18 - 0x8];
};
struct PPEntry104_unk {
    PPEntry104_unk() { memset(this, 0, sizeof(*this)); }
    unsigned char m00[0x104];
};
struct PPEntry44_unk {
    PPEntry44_unk() : m00(0) {}
    int m00;
    unsigned char m04[0x44 - 0x4];
};

// inferred: element of the arrays new[]-ed by the entries of MP::Manager2::m2A50 (only its empty
// destructor is known: weak copy 0x804D1478)
struct PPTableItem_unk {
    ~PPTableItem_unk() {}
    unsigned char m00[4];
};
// inferred: entry of the table m2A50 (12 bytes; ctor 0x804D13FC, dtor 0x804D1410); names invented
struct PPTableEntry_unk {
    PPTableEntry_unk() : m00(0), m04(0), m08(0) {}
    ~PPTableEntry_unk() {
        if (m08 != 0) {
            delete[] m08;
        }
    }
    int m00;
    int m04;
    PPTableItem_unk* m08;
};
// inferred: table of MP::Manager2 at 0x2A50 (21 entries and a buffer); names invented
struct PPTable_unk {
    PPTable_unk() : mFC(0) {}
    ~PPTable_unk() {
        if (mFC != 0) {
            delete[] mFC;
        }
        mFC = 0;
    }
    PPTableEntry_unk mEntries[21];
    unsigned char* mFC;
};
// inferred: MP::Manager2 (lbl_8087F610), only the members used here; names invented
namespace MP {
// inferred: MP::Manager2 (vtable 0x807916E0: dtor, vf0C, vf10, vf14 and atn::Task::vf18)
struct Manager2 : public atn::Task {
    Manager2(atn::Task* parent);
    virtual ~Manager2();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    PPSlot18_unk m0048[32];           // inferred: 32 entries of 0x18 bytes (m00 cleared at the end of the ctor)
    int m348;                         // inferred: room of the next game (fn_804D9BF4)
    int m34C;
    int m350;
    int m354;
    atn::net::NetPlayerInfo_unk m358[8]; // inferred: built with the NetPlayerInfo_unk ctor (L"<none>")
    int m4F8;
    union {
        int mState;                   // 0x4FC: inferred: state (30 = in game)
        PPStateVar_unk mStateVar;
    };
    int m504;                         // inferred: reset to -1 when the match ends (fn_804FB220)
    short m508;                       // inferred: reset to 3629 when the match ends
    short m50A;                       // inferred: set to 90 when a player slot is added
    int m50C;                         // inferred: sub state of the game (GameUISession_unk::GetM50C_unk)
    ResHandle m510;                   // inferred: folder "Multiplayer" of the debug menu
    MP::SaveLoad* mSaveLoad;          // 0x514
    int m518;                         // inferred: debug menu flags (Game, Debug, Clients, Players, Control info)
    int m51C;
    int m520;
    int m524;
    int m528;
    int m52C;                         // inferred: "Propagate Setting"
    int m530;
    int m534;
    int m538;
    int m53C;                         // inferred: the room is open (sent to atn::net::Manager::m0C)
    int mMode;                        // 0x540: inferred: game mode
    int m544;
    int m548;                         // inferred: mode of the last frame
    int m54C;
    int m550;
    int m554;
    int m558;
    int m55C;                         // inferred: values of messages 0x0C and 0x0E
    int m560;                         // inferred: settings of message 0x0F (m560-m598)
    int m564;                         // inferred: also the time limit of the game (seconds)
    int m568;
    PPWords_unk<8> m56C;              // inferred: warp point kept per player slot (fn_804EAA84)
    PPWords_unk<3> m58C;              // inferred: weighted choices (fn_804E679C)
    int m598;
    int m59C;                         // inferred: aid of the server
    int m5A0;
    int m5A4;
    int m5A8;
    int m5AC;                         // inferred: the settings of message 0x0F arrived
    int m5B0;
    unsigned char m5B4;
    unsigned char m05B5[0x5B8 - 0x5B5];
    long long mStartTime;             // 0x5B8: inferred: OSGetTime of the start of the game
    int m5C0;
    int m5C4;                         // inferred: "Respawn time"
    float m5C8;                       // inferred: "CharaSpeedInterpK"

    int mFrame;                       // 0x5CC: inferred: frame counter of vf10
    int m5D0;                         // inferred: "Lobby update timer"
    int m5D4;                         // inferred: "Visual update timer"
    int m5D8;                         // inferred: "Status update timer"
    int m5DC;                         // inferred: "Sync update timer"

    GameTask* mGame;                  // 0x5E0
    PPPlayerArray_unk mPlayers;       // 0x5E4
    PPCharaArray_unk mCharas;         // 0x5F0
    PPPlayer_unk* mOwnPlayer;         // 0x5FC: inferred: used when the character has no slot
    int m600;
    int m604;
    int m608;
    int m60C;
    PPLevelData_unk m610;             // inferred: object given to fn_805076F8
    int m69C;                         // inferred: start of the data given to fn_805061C8
    PPEntry104_unk m6A0[32];
    PPEntry44_unk m2720[6];
    PPEntry44_unk m28B8[6];
    PPTable_unk m2A50;                // inferred: destroyed by the destructor (entries 0x804D1410)
    atn::LH_Binary m2B50;             // inferred: resources loaded in state 3
    atn::LH_Binary m2B58;
    atn::LH_Binary m2B60;
    atn::LH_Binary m2B68;
    atn::SoundRef m2B70;
    atn::SoundRef m2B74;              // inferred: sounds stopped by fn_804DA398
    atn::SoundRef m2B78;
    atn::FadeColor* m2B7C;            // inferred: fade of the multiplayer (its mDelay is raised during the load, fn_804D2508)
    PPResult_unk m2B80;               // inferred: result of the game (m00, m04 state, m08 ranking)
    int m966C;
    int m9670;                        // inferred: result of fn_804E4A60 (fn_804E448C)
    int m9674;                        // inferred: item won in the game (code, count)
    int m9678;
    int m967C;                        // inferred: value for the players marked m30 (fn_804E4D70)
    int m9680;                        // inferred: experience after the game (fn_804E448C)
    int m9684;                        // inferred: level of m9680
    int m9688;
    int m968C;
    int m9690;
    int m9694[8];                     // inferred: per aid values of message 0x35
    atn::EfpFile m96B4;               // inferred: effects of the game (fn_804DF404)
    atn::EffFile m96C0;
    unsigned char m96CC[0x20];
    unsigned char m96EC[0x20];
    unsigned char m970C[0x20];
    unsigned char m972C[0x20];
    int m974C;                        // inferred: id given to the ranking requests
    int m9750;
    int m9754;
    int m9758;
    int m975C;                        // inferred: reset to 300 when the match ends (fn_804FB220)
    int m9760;
    int m9764;
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
    int m9A54[8];                     // inferred: per aid sequence of the last message 0x1040
    unsigned char m9A74[3];
    unsigned char m9A77;
    long long m9A78;
    unsigned char m9A80;
};
} // namespace MP

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
// inferred: atn::net::Manager (lbl_8087F628), only for mC2C: a word of the data saved by the game inside
// MpFriendList_unk::m7F0 (atn::net::Manager::mFriendList + 0x7FC), not yet split in include/atn/net/Manager.h
struct PPNet_unk {
    unsigned char m000[0xC2C];
    unsigned int mC2C; // inferred: bit 0 and bit 1 set by message 0x0D
};

// inferred: weak copies used by the receive loop (fn_804EE040) and by fn_804D25D0
extern "C" inline PPPlayer_unk* fn_804D5F0C(PPManager2_unk* mgr, int index) {
    if (index >= 0 && index < (int)mgr->mPlayers.size()) {
        return &mgr->mPlayers[index];
    }
    return 0;
}
extern "C" __declspec(noinline) inline int fn_804D5F40(PPPlayerArray_unk* players) {
    return players->size();
}
extern "C" __declspec(noinline) inline PPPlayer_unk* fn_804D5F48(PPPlayerArray_unk* players, int index) {
    return &(*players)[index];
}
extern "C" __declspec(noinline) inline int fn_804D6348(atn::net::Manager* net) {
    return net->mC215;
}

// --- MP::Manager2::vf10 (fn_804D25D0) and the weak copies of its inline accessors -----------------
// inferred: the copies 0x804D5F04-0x804D6680 follow fn_804D25D0 in the .dol (names invented)
#include "atn/LH_Binary.h"
#include "atn/Color.h"

// inferred: MP::Manager2 as seen by game/GameUI.cpp (its weak copy GetM50C_unk is the one called here)
#define PP_ASSERT_UNK(x) ((void)0)
// inferred: empty asserts (as GetRoom_unk below): the original vf10 is over the inlining budget and calls
// every inline accessor out of line; 700 asserts are needed (600 are not enough)
struct GameUISession_unk {
    unsigned char m000[0x50C];
    int m50C;
    int GetM50C_unk() const {
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
        return m50C;
    }
};

extern "C" void OSSleepTicks(long long ticks);

#include "game/ErrorManager.h"

#include "atn/net/NetRanking.h"

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
    static D2AMpMenu* fn_804AE820(atn::Task* parent); // inferred: creates the instance
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
    void fn_804B671C(int score, int team); // inferred: score of a team
    void fn_804B691C();
    void fn_804B67E4(int value);
    static D2AMpMenuCockpit* fn_804B4C50(atn::Task* parent); // inferred: creates the instance
    static D2AMpMenuCockpit* sInstance;
};
class D2AMpMenuResult : public atn::Task {
public:
    static D2AMpMenuResult* fn_804C566C(atn::Task* parent); // inferred: creates the instance
    void fn_804C8740();
    void fn_804C5E1C();
    void fn_804C6888(int time);
    bool fn_804C8204();
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
    return charas->size();
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
// inferred: friend list of the network manager (an inline level: the registers of fn_804E448C follow it); name
// invented
static inline MpFriendList_unk* PPFriendList_unk() {
    return &lbl_8087F628->mFriendList;
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
extern "C" int fn_804EE040(unsigned int aid, void* data, unsigned int size);
extern "C" int fn_804ED768(void* data, int init);
extern "C" int fn_804EDB0C(int index, void* param);
extern "C" int fn_804EDCCC(unsigned char* userData, void* param);
extern "C" unsigned char lbl_8087F614;
extern "C" const char* const lbl_80887594;
extern "C" const char* const lbl_80887598;
extern "C" const char* const lbl_8088759C;
extern "C" const char* const lbl_808875A0;
extern "C" void fn_804F5A94(PPManager2_unk* mgr);
extern "C" bool fn_804DC684(PPManager2_unk* mgr);
extern "C" void fn_804FB470(PPManager2_unk* mgr, int code, int arg);
extern "C" int fn_804DC450(PPManager2_unk* mgr, int mode);
extern "C" void fn_804FAD48(PPManager2_unk* mgr, int value);
extern "C" void fn_804DF11C(PPManager2_unk* mgr, unsigned int reset);
extern "C" void fn_804FB674(PPManager2_unk* mgr);
extern "C" void fn_804DA34C(PPManager2_unk* mgr);
extern "C" void fn_804DA48C(PPManager2_unk* mgr, int value);
extern "C" void fn_804DA4A0(PPManager2_unk* mgr, int value);
extern "C" int fn_804DB3B4(const PPManager2_unk* mgr);
extern "C" void fn_804DC358(PPManager2_unk* mgr);
extern "C" int fn_804DC374(PPManager2_unk* mgr, int mode);
extern "C" int fn_804DE884(PPManager2_unk* mgr);
extern "C" int fn_804DF404(PPManager2_unk* mgr);
extern "C" void fn_804DF310(PPManager2_unk* mgr);
extern "C" void fn_804FB668(PPManager2_unk* mgr);
extern "C" PPPlayer_unk* fn_804EB870(PPManager2_unk* mgr);
extern "C" void fn_804FAA7C(PPManager2_unk* mgr, unsigned int flush, PPPlayer_unk* player);
extern "C" void fn_804D824C(PPManager2_unk* mgr, PPPlayer_unk* player, int mode);
extern "C" void fn_804FC0D4(PPManager2_unk* mgr, int value);
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
extern "C" __declspec(noinline) inline PPCharaInfo_unk* fn_804D8078(PPCharaInfo_unk* dst, const PPCharaInfo_unk* src) {
    *dst = *src;
    return dst;
}
extern "C" void fn_804D00B8(int index, int value);
struct PPRequests_unk;
extern "C" bool fn_804F5A98(PPRequests_unk* queue, unsigned char a, int b, unsigned char c);
extern "C" void fn_804F5FCC(PPRequests_unk* queue);
extern "C" int fn_804F5E38(PPRequests_unk* queue, unsigned int id);
extern "C" int fn_804F5C9C(PPRequests_unk* queue, unsigned int id);
extern "C" long long* fn_804F5B08(PPRequests_unk* queue, unsigned int id);
extern "C" int fn_804FB968(const PPManager2_unk* mgr, CharaTask* chara);
extern "C" void fn_804EA8B8(PPManager2_unk* mgr, int arg);
extern "C" void fn_804E448C(PPManager2_unk* mgr);
extern "C" int fn_804D9BF4(PPManager2_unk* mgr, int a, int b, int c);
extern "C" void fn_804E49A4(PPManager2_unk* mgr);
extern "C" void fn_804E5BC4(PPManager2_unk* mgr);
extern "C" void fn_804D1F6C(PPPlayer_unk* player);
extern "C" void fn_804FA948(PPManager2_unk* mgr, int value, unsigned int flush);
extern "C" int fn_804FAB40(PPManager2_unk* mgr, int value, PPPlayer_unk* player, int* seq, unsigned int flush);
extern "C" void fn_804EA608(PPManager2_unk* mgr, int a, int b);
extern "C" int fn_804EAFD0(PPManager2_unk* mgr);
extern "C" int fn_804EAA50(PPManager2_unk* mgr);
extern "C" void fn_804DA398(PPManager2_unk* mgr);
extern "C" bool fn_804DA2D0(PPManager2_unk* mgr, int keep);
extern "C" int fn_804DBD08(PPManager2_unk* mgr, int mode);
extern "C" long long fn_804CF194();
extern "C" void fn_804CF190(atn::Task* task);
extern "C" void fn_804D0090(PPFlags_unk* obj, int on);
extern "C" int fn_804CF634(PPChara_unk* chara);
void fn_804D0164();
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

extern "C" __declspec(noinline) inline int fn_804D24A8(atn::net::NetThread_unk* thread) {
    return thread->mRunning;
}
extern "C" __declspec(noinline) inline int fn_804D5F04(PPStateVar_unk* state) {
    return state->mValue;
}
extern "C" __declspec(noinline) inline int fn_804D5F38(PPManager2_unk* mgr) {
    return (int)mgr->mPlayers.size();
}
extern "C" __declspec(noinline) inline void fn_804D5F58(CharaTask* chara, int on) {
    chara->mFlag15 = on;
}
extern "C" __declspec(noinline) inline void fn_804D5F68(PPManager2_unk* mgr) {
    mgr->m99B8--;
}
extern "C" __declspec(noinline) inline int fn_804D5F7C(atn::net::Manager* net) {
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
    return error->mSelect;
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
    mgr->m99A4.Clear_unk();
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
extern "C" __declspec(noinline) inline unsigned int fn_804D6190(atn::net::Manager* net) {
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
extern "C" __declspec(noinline) inline int fn_804D6220(atn::net::Manager* net) {
    return net->mC0;
}
extern "C" __declspec(noinline) inline unsigned char fn_804D6228(atn::net::Manager* net) {
    if (net->mC215 == 0) {
        return net->mFlags.mB12 ? 0 : (unsigned char)fn_806B1250();
    } else {
        return 0;
    }
}
extern "C" __declspec(noinline) inline int fn_804D6278(const atn::net::Manager* net, unsigned char aid) {
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
inline chr::CharaEquip_unk* CharaTask::GetProtector(int i) {
    return m680[i];
}
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
    return &(*charas)[index];
}
extern "C" __declspec(noinline) inline unsigned char fn_804D64B4(PPPlayer_unk* player) {
    return player->m0D52++;
}
inline int GetRemain_unk(const PPManager2_unk* mgr) {
    if (mgr->m564 >= 0) {
        return mgr->m564 - (int)((OSGetTime() - mgr->mStartTime) / (*(unsigned int*)0x800000F8 / 4));
    }
    return -1;
}
extern "C" __declspec(noinline) inline bool fn_804D64C8(const PPManager2_unk* mgr) {
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
    if (state == 2 && ranking->mIndex != -1) {
        ranking->mTimes[ranking->mIndex] = -1;
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
// inferred: magic of the data of fn_804ED768 (hash of its name; first entry of the __sinit 0x804FF478)
extern "C" int lbl_8087F604 = fn_800DC6B4("CustomData");

#include "std/ios_init.h"

// inferred: options of the GameTask of the online game, by mode and stage (fn_804DE884)
GameTask::Option lbl_807C8B10[3][10] = {
    {
        GameTask::Option(4, 1, 3, 0, -1, 0, 0), GameTask::Option(4, 1, 1, 0, -1, 0, 0),
        GameTask::Option(4, 1, 9, 0, -1, 0, 0), GameTask::Option(4, 1, 8, 0, -1, 0, 0),
        GameTask::Option(4, 1, 2, 0, -1, 0, 0), GameTask::Option(4, 1, 7, 0, -1, 0, 0),
        GameTask::Option(4, 1, 4, 0, -1, 0, 0), GameTask::Option(4, 1, 5, 0, -1, 0, 0),
        GameTask::Option(4, 1, 6, 0, -1, 0, 0), GameTask::Option(4, 1, 1, 0, -1, 0, 0),
    },
    {
        GameTask::Option(4, 1, 3, 0, -1, 0, 0), GameTask::Option(4, 1, 1, 0, -1, 0, 0),
        GameTask::Option(4, 1, 9, 0, -1, 0, 0), GameTask::Option(4, 1, 8, 0, -1, 0, 0),
        GameTask::Option(4, 1, 2, 0, -1, 0, 0), GameTask::Option(4, 1, 7, 0, -1, 0, 0),
        GameTask::Option(4, 1, 4, 0, -1, 0, 0), GameTask::Option(4, 1, 5, 0, -1, 0, 0),
        GameTask::Option(4, 1, 6, 0, -1, 0, 0), GameTask::Option(4, 1, 1, 0, -1, 0, 0),
    },
    {
        GameTask::Option(3, 3, 7, 0, -1, 0, 0), GameTask::Option(3, 3, 2, 0, -1, 0, 0),
        GameTask::Option(3, 3, 6, 0, -1, 0, 0), GameTask::Option(3, 3, 1, 0, -1, 0, 0),
        GameTask::Option(3, 3, 3, 0, -1, 0, 0), GameTask::Option(3, 3, 4, 0, -1, 0, 0),
        GameTask::Option(3, 3, 5, 0, -1, 0, 0), GameTask::Option(3, 3, 3, 0, -1, 0, 0),
        GameTask::Option(3, 3, 1, 0, -1, 0, 0), GameTask::Option(3, 3, 1, 0, -1, 0, 0),
    },
};


inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}
// inferred: the packer and its key generator are created by the ctor of MP::Manager2 (inlined there)
// inferred: empty name and file strings of the allocations of the ctor of MP::Manager2 (one .sdata object each,
// as lbl_8087E1B0/lbl_8087E1AC above)
extern "C" const char lbl_8087E17C[1];
extern "C" const char lbl_8087E174[1];
extern "C" const char lbl_8087E178[1];
extern "C" const char lbl_8087E180[1];
extern "C" const char lbl_8087E184[1];
extern "C" const char lbl_8087E188[1];
extern "C" const char lbl_8087E18C[1];
extern "C" const char lbl_8087E190[1];
extern "C" const char lbl_8087E194[1];
extern "C" const char lbl_8087E198[1];
extern "C" const char lbl_8087E19C[1];
extern "C" const char lbl_8087E1A0[1];
extern "C" const char lbl_8087E1A4[1];
extern "C" const char lbl_8087E1A8[1];
extern "C" const char lbl_8087E1B4[1];
extern "C" const char lbl_8087E1B8[1];
extern "C" const char lbl_8087E1BC[1];
extern "C" const char lbl_8087E1C0[1];
inline atn::net::Crypt::Crypt() : m04(0x1000) {
    m08 = (char*)fn_800838B8(atn::MemoryManager::Instance(), 0x1000, 0x20, 5, lbl_8087E1C0, lbl_8087E1BC, 0);
    m0C = (char*)fn_800838B8(atn::MemoryManager::Instance(), 0x1000, 0x20, 5, lbl_8087E1B8, lbl_8087E1B4, 0);
}
inline MP::CryptMP::CryptMP() : m10(0), m14(0), m18(0) {
    m14 = (char*)fn_800838B8(atn::MemoryManager::Instance(), vf10() * 2 + 1, 4, 5, lbl_8087E1A8, lbl_8087E1A4, 0);
    m18 = (char*)fn_800838B8(atn::MemoryManager::Instance(), vf10() + 1, 4, 5, lbl_8087E1A0, lbl_8087E19C, 0);
}
inline MP::PacketPackMP::PacketPackMP() {
    m14 = new ((MemoryTag)5, lbl_8087E198, lbl_8087E194, 0) MP::CryptMP;
    fn_8050E510(m14);
}

// inferred: items of the debug menu (atn/MindowsCommon.h; declared here with the void* folders of game/ResHandle.h)
extern "C" void fn_800874C8(void* folder, const char* name, int* value, int min, int max, int step,
                            void (*func)(void*, void*), void* arg); // AddInt
extern "C" void fn_8008771C(void* folder, const char* name, float* value, float min, float max, float step,
                            void (*func)(void*, void*), void* arg); // AddFloat
extern "C" void fn_80087994(void* folder, const char* name, int* value, void (*func)(void*, void*),
                            void* arg); // AddBool
extern WPad_unk* lbl_8087F018; // inferred: Wii Remote manager (defined in atn/WPad_unk.cpp)
extern int lbl_80790F78[30]; // inferred: messages sent in each frame of a cycle of 30 (defined below)
extern "C" MP::ManagerMP* lbl_8087F618;   // inferred: instance of MP::ManagerMP (copied to lbl_8087F628)
extern "C" MP::PacketPackMP* lbl_8087F62C; // inferred: copy of lbl_8087F61C
extern "C" int lbl_8087F630;              // inferred: forced id of the session (local play when 0)
extern "C" void fn_804EB934(int id, CharaTask* chara, void* arg); // inferred: listener of the characters
extern "C" void fn_804F60D4(void* arg, void* value);
extern "C" void fn_804F60D8(void* arg, void* value);
extern "C" void fn_804F60DC(void* arg, void* value);
extern "C" atn::SoundEngine_unk* lbl_8087EE90; // inferred: sound engine (atn/SoundCore.h)
extern "C" int fn_8004829C(void* sound, const char** groups, int count); // inferred: loads sound groups
namespace atn {
namespace net {
void* fn_8050CFC8(void* arg); // inferred: main function of the network thread (atn/NetManager.cpp)
} // namespace net
} // namespace atn

// NONMATCHING: 94.11% - registers, construction of the table m2A50, stack frame and the unreferenced "" of
// @stringBase0 after "%d" (see docs/cierre99-20261115-qG2.md)
// inferred: MP::Manager2 ctor: creates the network singletons, the debug menu "Multiplayer", the menus and
// the managers of the multiplayer mode
MP::Manager2::Manager2(atn::Task* parent)
    : atn::Task(parent), m354(0), m55C(10), m560(0), m564(300), m568(50), m598(0),
      m59C(-1), m5A0(6), m5A4(0), m5A8(0), m5AC(0), m5B0(1), m5B4(0), m69C(0) {

    if (HomeButtonMenu::sInstance != 0 && HomeButtonMenu::sInstance->m88 == 1) {
        HomeButtonMenu::sInstance->m88 = 0;
    }
    if (lbl_8087F618 == 0) {
        lbl_8087F618 = new ((MemoryTag)1, lbl_8087E190, lbl_8087E18C, 0) MP::ManagerMP;
    }
    lbl_8087F628 = lbl_8087F618;
    if (lbl_8087F61C == 0) {
        lbl_8087F61C = new ((MemoryTag)1, lbl_8087E188, lbl_8087E184, 0) MP::PacketPackMP;
    }
    lbl_8087F62C = lbl_8087F61C;
    if (lbl_8087F630 > 0) {
        lbl_8087F628->mC218 = lbl_8087F630;
    }
    lbl_8087F628->mC215 = (lbl_8087F630 == 0) == 0;
    if (lbl_8087F5A4 == 0) {
        lbl_8087F5A4 = new ((MemoryTag)1, lbl_8087E180, lbl_8087E17C, 0) atn::net::NetRanking_unk;
    }
    fn_804FB220(&mStateVar, 0);
    m5C4 = 450;
    m50C = 0;
    m9974 = 0;
    m518 = 1;
    m51C = 0;
    m520 = 1;
    m524 = 0;
    m528 = 0;
    m530 = 1;
    m52C = 1;
    m534 = 0;
    m538 = 0;
    m50A = 0;
    mFrame = 0;
    m5D0 = 30;
    m5D4 = 1;
    m5D8 = 10;
    m5DC = 30;
    m558 = 0;
    mGame = 0;
    m548 = 0;
    m544 = 0;
    mMode = 0;
    m54C = 1;
    m999C = 0;
    m999D = 0;
    m550 = 0;
    m554 = 0;
    mStartTime = 0;
    m60C = 0;
    m5C0 = 1;
    m974C = 0;
    m9758 = -1;
    m53C = 0;
    m5C8 = 0.95f;
    m9754 = -1;
    m600 = 30;
    m604 = 1;
    m608 = -1;
    mSaveLoad = 0;
    m99F0[0] = 0;
    m99F0[1] = 0;
    m99F0[2] = 0;
    m9978 = 0;
    m99F0[3] = 0;
    m9760 = 0;
    m9764 = 1;
    m9768 = 0;
    m976C = 0;
    memset(m96CC, 0, sizeof(m96CC));
    memset(m96EC, 0, sizeof(m96EC));
    memset(m970C, 0, sizeof(m970C));
    memset(m972C, 0, sizeof(m972C));
    memset(m9984, 0, sizeof(m9984));
    memset(m998C, 0xFF, sizeof(m998C));

    fn_8014F8F4()->mList.clear();
    CharaListener_unk listener;
    listener.mId = -1;
    listener.mFunc = fn_804EB934;
    fn_8014F8F4()->mList.push_back(listener);

    void* menu = m510.Load("Multiplayer");
    fn_800874C8(menu, "MatchGroup", &m54C, 1, 999, 1, 0, 0);
    fn_80087994(menu, "Game info", &m518, 0, 0);
    fn_80087994(menu, "Debug info", &m51C, 0, 0);
    fn_80087994(menu, "Clients info", &m520, 0, 0);
    fn_80087994(menu, "Players info", &m524, 0, 0);
    fn_80087994(menu, "Control info", &m528, 0, 0);
    void* rules = fn_8008937C(menu, "Rules");
    fn_800874C8(rules, "Time limit", &m564, -1, 6000, 60, 0, 0);
    fn_800874C8(rules, "Score limit", &m568, -1, 1000, 10, 0, 0);
    void* setting = fn_8008937C(menu, "Setting");
    fn_80087994(menu, "Propagate Setting", &m52C, 0, 0);
    fn_800874C8(setting, "Lobby update timer", &m5D0, 0, 10000, 1, 0, 0);
    fn_800874C8(setting, "Visual update timer", &m5D4, 0, 10000, 1, 0, 0);
    fn_800874C8(setting, "Status update timer", &m5D8, 0, 10000, 1, 0, 0);
    fn_800874C8(setting, "Sync update timer", &m5DC, 0, 10000, 1, 0, 0);
    fn_800874C8(setting, "Profiler timer", &lbl_8087F628->m250, 0, 10000, 1, fn_804F60D4, 0);
    fn_800874C8(setting, "Host update timer", (int*)&lbl_8087F628->m254, 0, 10000, 1, fn_804F60D8, 0);
    fn_800874C8(setting, "Host ping timer", (int*)&lbl_8087F628->m258, 0, 10000, 1, fn_804F60DC, 0);
    void* gameplay = fn_8008937C(menu, "Gameplay");
    fn_800874C8(gameplay, "Respawn time", &m5C4, 0, 10000, 30, 0, 0);
    fn_8008771C(gameplay, "CharaSpeedInterpK", &m5C8, 0.0f, 1.0f, 0.02f, 0, 0);
    void* schedule = fn_8008937C(menu, "Schedule");
    for (unsigned int i = 0; i < 30; i++) {
        char name[256];
        sprintf(name, "%d\0", i);
        fn_800874C8(schedule, name, &lbl_80790F78[i], 0, 255, 1, 0, 0);
    }

    SetConfig::sInstance->mShowTutorial = 0;
    m99BC = 0;
    MP::D2AMpMenu::fn_804AE820(this);
    MP::D2AMpMenu::sInstance->fn_800D246C(1);
    MP::D2AMpMenuCockpit::fn_804B4C50(this);
    MP::D2AMpMenuCockpit::sInstance->fn_800D246C(1);
    MP::D2ACommonDialog::fn_804AC430(this);
    MP::D2ACommonDialog::sInstance->fn_800D246C(1);
    MapManager::SetTask(this);
    m9770 = 0;
    lbl_8087EFE8->fn_800CDF84();
    const char* groups[3] = {"GROUP_SYSTEM", "GROUP_PC001", "GROUP_MP_EM003"};
    fn_8004829C(lbl_8087EE90, groups, 3);
    if (GameBgm::sInstance == 0) {
        GameBgm::fn_8037C8C4(this);
    }
    if (lbl_8087F498 == 0) {
        GameVoice::fn_803E836C(this);
        lbl_8087F498->mNumChannels = 3;
    }
    m9774.SetPosition(atn::Vector3(0.0f, -99500.0f, 0.0f));
    m9774.SetTarget_unk(atn::Vector3(0.0f, -99500.0f, 10.0f));
    fn_8037F688(&m9774, 1.0471976f);
    atn::EffectManager::fn_80239798(this, 0x240, 0x480, 8, 0x180);
    fn_80239DAC(lbl_8087F3C0, (unsigned int)this, 0, 0);
    m9968.LoadRequest("be010_008.efp");
    for (int i = 0; i < 32; i++) {
        m0048[i].m00 = 0;
    }
    lbl_8087F628->mThread.fn_8050CDD0(atn::net::fn_8050CFC8, 17);
    lbl_8087F420->m0C = 1;
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0[0] = 1;
    }
    m2B7C = atn::FadeColor::Create(this, 15, 0xFF000000, 0);
    m2B7C->mDeleteAtEnd = 1;
    m2B7C->mDelay = 99999;
    m2B7C->mActive = 1;
}

// NONMATCHING: 98.03% - the end node is recomputed for the clear of the list (the original reuses the one of the
// loop) and registers
// inferred: frees the buffers of every peer, then the list (inline: also inlined in MP::PacketPackMP::~PacketPackMP)
inline atn::net::PacketPack::~PacketPack() {
    for (std::list<PPUser_unk>::iterator it = mUsers.begin(); it != mUsers.end(); ++it) {
        if ((*it).m08 != 0) {
            atn::MemoryManager::Instance()->Free((*it).m08);
            (*it).m08 = 0;
        }
        if ((*it).m0C != 0) {
            atn::MemoryManager::Instance()->Free((*it).m0C);
            (*it).m0C = 0;
        }
    }
    mUsers.clear();
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

extern "C" void fn_80049654(void* sound); // inferred: as in game/GameTask.cpp
extern "C" void fn_8004895C(void* sound); // inferred: releases the sound groups (game/NewTitleTask.cpp)
// NONMATCHING: 98.75% - registers of the clear of the list m99A4 (the original keeps its end in another register)
MP::Manager2::~Manager2() {
    fn_80049654(lbl_8087EE90);
    fn_8004895C(lbl_8087EE90);
    lbl_8087EFE8->fn_800CE368();
    SetConfig::sInstance->mShowTutorial = 1;
    while (lbl_8087F628->mThread.mRunning == 0) {
        OSSleepTicks((long long)100 * (*(unsigned int*)0x800000F8 / 4 / 1000));
    }
    lbl_8087F628->mThread.fn_8050CE9C();
    while (lbl_8087F628->mThread.fn_8050CF14() == 0) {
        OSSleepTicks((long long)100 * (*(unsigned int*)0x800000F8 / 4 / 1000));
    }
    if (lbl_8087F5A4 != 0) {
        delete lbl_8087F5A4;
        lbl_8087F5A4 = 0;
    }
    lbl_8087F628->vf0C();
    if (lbl_8087F618 != 0) {
        delete lbl_8087F618;
        lbl_8087F618 = 0;
    }
    lbl_8087F62C = 0;
    lbl_8087F628 = 0;
    fn_8014F8F4()->mList.clear();
    lbl_8087F610 = 0;
    if (m9974 != 0) {
        m9974 = 0;
    }
    if (m9770 == 0) {
        m9770 = 0;
    }
    lbl_8087F420->m0C = 0;
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0[0] = 0;
    }
    if (HomeButtonMenu::sInstance != 0 && HomeButtonMenu::sInstance->m88 == 0) {
        HomeButtonMenu::sInstance->m88 = 1;
    }
}

MP::ManagerMP::~ManagerMP() {}

extern "C" int fn_800C310C(int arg);
// inferred: MP::Manager2::vf0C (vtable 0x807916E0): waits for the resources, then shows the menu
int MP::Manager2::vf0C() {
    PPManager2_unk* mgr = this;
    mgr->m2B7C->mDelay = 99999;
    if (fn_800C310C(0) == 0 && mgr->m9968.IsLoading() == 0 && mgr->fn_800D3FA4() != 0) {
        fn_80232B7C((unsigned int)mgr, 0);
        MP::D2AMpMenu::sInstance->fn_800D246C(0);
        MP::D2AMpMenu* menu = MP::D2AMpMenu::sInstance;
        menu->m299C.m00 = 2;
        menu->m299C.m08 = 0;
        menu->m299C.m04 = 0;
        menu->m299C.m0C = 0;
        MP::D2ACommonDialog::sInstance->fn_800D246C(0);
        mgr->m2B7C->mDelay = 0;
        return 1;
    }
    return 0;
}

// inferred: MP::Manager2::vf10 (vtable 0x807916E0): update of the multiplayer mode
// NONMATCHING: 95.16% - registers and stack slots; the three arrays of -1 at 0x99F4 are stored through advancing
// pointers in the original and the 60/80 rest of mode 0-2 shares its store (checked against the whole original:
// field offsets, constants, calls and branches; escribir-20261116 qG3)
void MP::Manager2::vf10() {
    PPManager2_unk* mgr = this;
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
        if (mgr->m9978 == 0 && fn_804DC684(fn_800F7F90()) == 1) {
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
        bool done = false;
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
            fn_805061C8(&mgr->m69C, mgr->m2B60.fn_80470580(), size);
            size = mgr->m2B68.fn_8047059C();
            fn_80506D94(&mgr->m2A50, mgr->m2B68.fn_80470580(), size);
            size = mgr->m2B50.fn_8047059C();
            fn_80507310(&mgr->m610, mgr->m2B50.fn_80470580(), size);
            mgr->m2B58.fn_80473F88();
            mgr->m2B60.fn_80473F88();
            mgr->m2B68.fn_80473F88();
            mgr->m2B50.fn_80473F88();
            fn_804D169C(&mgr->mStateVar, 7);
            if (wcslen(fn_804D60E4(fn_804D6024(fn_804D1694()))) != 0) {
                fn_804D16CC()->fn_804AF520(2);
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
        for (int i = 0; i < 3;) {
            mgr->m9A74[i++] = 0;
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
        bool joined = false;
        if (fn_804D6168(fn_804D1694())) {
            joined = 1;
        } else if (fn_804D6190(fn_804D1694()) == 1 && fn_804D6198(fn_804D1694()) == 0) {
            joined = 1;
            fn_804D16E4(fn_804D1694())->fn_8050CF90();
        }
        int friendRoom = 0;
        if (joined == 1) {
            joined = 0;
            mgr->mPlayers.resize(fn_804D6200(fn_804D1694()));
            fn_804DF310(mgr);
            fn_804FB668(mgr);
            fn_804D6208(mgr, 0);
            joined = 1;
            unsigned char aid = fn_804D6198(fn_804D1694());
            if (aid >= (unsigned int)fn_804D5F40(&mgr->mPlayers)) {
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
                fn_804D5FC0()->fn_804C2268(mgr->m508 / 30);
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
            for (unsigned int i = 0; i < fn_804D5F40(&mgr->mPlayers); i++) {
                PPPlayer_unk* player = fn_804D5F48(&mgr->mPlayers, i);
                if (player->mFlags.mActive == 1) {
                    fn_804EAA84(mgr, player->mRef.mChara, 1, 0);
                    if (player->mEquip0.m04.mOn) {
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
                                    player->mRef.mChara->GetProtector(j) != 0) {
                                    int id = fn_8020ED84(j, fn_804D63D8(player->mRef.mChara)->m108[j]);
                                    fn_804D63F8((PPEquipObj_unk*)player->mRef.mChara->GetProtector(j), id);
                                }
                            }
                        }
                        fn_804D6400(player->mRef.mChara->GetStatus_unk(), player->mEquip0.m04.mLevel);
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
                    sprintf(name, "%s\0", ((const char**)player->mRef.mChara->GetInfo_unk())[0x14 / 4]);
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
                GameScene* field = mgr->mGame->fn_8013C504();
                if (field != 0) {
                    atn::SimpleArray<PPRoute_unk>* routes = fn_804D649C(field);
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
        int level = 56;
        for (int i = 0; i < fn_804D810C(&mgr->mCharas); i++) {
            PPCharaEntry_unk* entry = fn_804D64A4(&mgr->mCharas, i);
            if (entry->mRef.mChara == 0) {
                continue;
            }
            if (fn_804D63D8(entry->mRef.mChara)->mC8 >= 0) {
                ((float*)entry->mRef.mChara->GetStatus_unk())[0x1B0 / 4] = level;
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
            int result = fn_804F5E38((PPRequests_unk*)fn_800F7F90(), mgr->m9A80);
            if ((unsigned int)(result + 2) <= 1) {
                fn_804F5C9C((PPRequests_unk*)fn_800F7F90(), mgr->m9A80);
            } else if (result == 1) {
                long long now = fn_804CF194();
                long long half = (now - mgr->m9A78) / 2;
                mgr->mStartTime = now - (*fn_804F5B08((PPRequests_unk*)mgr, mgr->m9A80) + half);
                fn_804F5C9C((PPRequests_unk*)fn_800F7F90(), mgr->m9A80);
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
                    data->m10 = mgr->m2B80.m00;
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
                        if (mgr->m2B80.m04 == 0 || mgr->m2B80.m04 == 3) {
                            GameBgm::GetInstance_unk()->fn_8037D4C0(0x8CB, 15, 1, 0, 0.8f);
                        } else if (mgr->m2B80.m04 == 1 || mgr->m2B80.m04 == 4) {
                            GameBgm::GetInstance_unk()->fn_8037D4C0(0x3F5, 15, 1, 0, 0.8f);
                        } else {
                            GameBgm::GetInstance_unk()->fn_8037D4C0(0x1F6, 15, 1, 0x21FC, 0.8f);
                        }
                    }
                    fn_804D5FD0()->fn_804C5E1C();
                    fn_804D5FD0()->fn_804C6888(mgr->m99A0 / 30 + 1);
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
            fn_804D5FD0()->fn_804C6888(mgr->m99A0 / 30 + 1);
            if (mgr->m99A0 >= 750) {
                break;
            }
            if (state != 7 && (unsigned int)(state - 5) <= 1) {
                fn_804D65A4(fn_804D5FD0(), 0);
                bool skip = false;
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
                for (int i = 0; i < 3;) {
                    mgr->m9A74[i++] = 0;
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
                own->mFlags.m22 = own->mD4Flags.m22;
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
            if ((mgr->m99F0[0] == 0 || mgr->m99F0[1] == 0 || mgr->m99F0[2] == 0) && fn_804EAFD0(mgr) > 1) {
                fn_804FB470(mgr, 0x9D, 22);
                if (fn_804D16CC() != 0) {
                    fn_804D65D4(fn_804D16CC(), 4);
                }
            } else if (mgr->m99F0[3] != 0) {
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
                if (mgr->m2B80.m04 == 0 || mgr->m2B80.m04 == 3) {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x8CB, 30, 1, 0, 0.8f);
                } else if (mgr->m2B80.m04 == 1 || mgr->m2B80.m04 == 4) {
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

// inferred: this machine is the server (inline of fn_804EB1AC)
static inline int IsServer_unk() {
    unsigned char aid = lbl_8087F628->GetMyAid_unk();
    return aid == lbl_8087F628->GetServerAid_unk();
}

// inferred: two words copied together (the copies of the slot fields 0xB4 and 0xC0 load both words before
// storing them, as a struct copy; maybe an OSTime in a packed struct); name invented
struct PPWord2_unk {
    int m[2];
};

// inferred: payload of the messages 0x1040/0x1048 (the settings of the room and the slots; 109 bytes, not
// padded on the network); names invented
#pragma pack(push, 1)
struct PPMsgRoom_unk {
    int m00;
    int m04;
    unsigned char m08;
    int m09;
    PPWord2_unk m0D;
    int m15;
    PPWord2_unk m19;
    int m21;
    unsigned int mFlags;
    unsigned char mTeams[8];
    unsigned char mB0[8];
    unsigned char mD4[8];
    unsigned char m41;
    int mD8[8];
    unsigned short m62;
    short m64;
    unsigned char m66;
    unsigned char m67;
    unsigned char mSeq;
    unsigned char m69;
    unsigned char mMode;
    unsigned char m6B;
    unsigned char m6C;
};
#pragma pack(pop)

// NONMATCHING: 96.07% - registers: the original keeps the payload pointer in r31 and the offset of the slot
// loop in r26 (here r29/r31)
// inferred: sends the settings of the room and of the slots: mode 0 to every other active player (each with a
// sequence number and a request waiting for its answer), 1 to everybody once (m999D), 2 as message 0x1048
extern "C" void fn_804D824C(PPManager2_unk* mgr, PPPlayer_unk* player, int mode) {
    if (mode == 2 && mgr->m999D == 1) {
        return;
    }
    NetMsgI_unk<PPMsgRoom_unk, 109> msg(mode == 2 ? 0x1048 : 0x1040);
    PPMsgRoom_unk* data = &msg.mData;
    unsigned int i;
    data->m09 = player->m0B0[0];
    data->m0D = *(PPWord2_unk*)&player->m0B0[1];
    data->m15 = player->m0B0[3];
    data->m19 = *(PPWord2_unk*)&player->m0B0[4];
    data->m21 = player->mC8;
    data->mFlags = player->mFlagWord;
    for (i = 0; (int)i < (int)mgr->mPlayers.size(); i++) {
        data->mTeams[i] = mgr->mPlayers[i].mFlags.m22;
        data->mB0[i] = mgr->mPlayers[i].mB0;
        data->mD8[i] = mgr->mPlayers[i].mD8;
        data->mD4[i] = (mgr->mPlayers[i].mD4 >> 22) & 0xF;
    }
    data->m00 = mgr->m55C;
    data->m04 = mgr->m564;
    data->m66 = mgr->m5A0;
    data->m67 = mgr->m5A8;
    data->m69 = mgr->m5B4;
    data->m6C = mode;
    data->mMode = mgr->mMode;
    data->m6B = mgr->m5A4;
    data->m08 = mgr->m5B0 != 0;
    data->m64 = mgr->m508;
    switch (mode) {
    case 0:
        for (int i = 0; i < 8;) {
            fn_800F7F90()->m9A54[i++] = -1;
        }
        for (i = 0; i < mgr->mPlayers.size(); i++) {
            if (mgr->mPlayers[i].mFlags.mActive == 1 && player->mAid != mgr->mPlayers[i].mAid) {
                data->mSeq = player->m0D52++;
                mgr->m9A54[mgr->mPlayers[i].mAid] = data->mSeq;
                fn_804AE3BC()->fn_8050E094(mgr->mPlayers[i].mAid, 0x1040, data, 1);
                fn_804F5A98((PPRequests_unk*)fn_800F7F90(), mgr->mPlayers[i].mAid, 300,
                            mgr->m9A54[mgr->mPlayers[i].mAid]);
            }
        }
        break;
    case 1:
        mgr->m999D = 1;
        fn_804AE3BC()->fn_8050E094(-1, 0x1040, data, 1);
        fn_804AE3BC()->fn_8050DDDC(-1, 1);
        break;
    case 2:
        fn_804AE3BC()->fn_8050E094(-1, 0x1048, data, 1);
        break;
    }
}

extern "C" PPPlayer_unk* fn_804DD158(PPManager2_unk* mgr, unsigned int aid);
// inferred: the search of fn_804DD158 written with a local for the slot (inlined in fn_804D90E0, fn_804EB010 and
// fn_804F3EEC, whose registers differ from the inlined copies of fn_804DD158); name invented
static inline PPPlayer_unk* PPFindSlot_unk(PPManager2_unk* mgr, unsigned int aid) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1 && aid == player->mAid) {
            return player;
        }
    }
    return 0;
}
// inferred: flushes the packer; the server goes on with fn_804D824C (mode 1), a client starts the timer m99B8
extern "C" void fn_804D8610(PPManager2_unk* mgr) {
    fn_804AE3BC()->fn_8050DE34();
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    if (IsServer_unk() == 1) {
        fn_804D824C(mgr, own, 1);
    } else {
        mgr->m99B8 = 300;
    }
}

// inferred: waits for the other players: the server is ready at once (m9A74[0]); a client gives up with the
// error 0x9E when the timer m99B8 runs out
extern "C" void fn_804D87B0(PPManager2_unk* mgr) {
    if (IsServer_unk() == 1) {
        mgr->m9A74[0] = 1;
    } else if (mgr->m99B8 <= 0) {
        fn_804FB470(mgr, 0x9E, 9);
    }
}

// inferred: payload of the messages 0x1C and 0x1009 (one equipment of the player: index and its data); names
// invented
struct PPMsgEquip_unk {
    int mKind;
    PPEquipHead_unk mHead;
};
// inferred: payload of the message 0x100A (a weapon of the own slot: index and item); names invented
struct PPMsgWeapon_unk {
    int mKind;
    int mItem;
};
// inferred: sends equipment `kind` of the own save data (message 0x1009, the payload of message 0x1C read as a
// GameSavePacketEquip_unk by the save data) and keeps it in the own slot
inline void SendEquip_unk(GameSaveData_unk* save, PPEquip_unk& equip, PPMsgEquip_unk* data, int chara, int kind) {
    bool ok = save->mInventory.fn_803BD0FC((GameSavePacketEquip_unk*)data, chara, kind);
    if (ok) {
        ok &= save->mParty.fn_803BAE54((GameSavePacketEquip_unk*)data, chara, kind);
    }
    if (ok) {
        equip.Set_unk(data->mHead);
        fn_804AE3BC()->fn_8050E094(-1, 0x1009, data, 1);
    }
}
// inferred: sends weapon `kind` of the own save data (message 0x100A) and keeps it in the own slot
inline void SendWeapon_unk(GameSaveData_unk* save, int& value, PPMsgWeapon_unk* data, int chara, int kind) {
    if (save->mInventory.fn_803BD22C((GameSavePacketEquip_unk*)data, chara, kind)) {
        value = data->mItem;
        fn_804AE3BC()->fn_8050E094(-1, 0x100A, data, 1);
    }
}

// NONMATCHING: 98.91% - registers (save data and character swapped; aid/offset of the inlined search fn_804DD158)
// inferred: sends the equipment of the own save data to everybody (messages 0x1009 and 0x100A, character
// mB0 + 700000), then asks every other player for an answer (message 0x1041 with the next sequence of the own
// slot, kept in m99F4 and in the request queue)
extern "C" void fn_804D8910(PPManager2_unk* mgr) {
    PPManager2_unk* mgr2 = fn_800F7F90();
    PPPlayer_unk* own = fn_804DD158(mgr2, lbl_8087F628->GetMyAid_unk());
    MP::SaveLoad* saveLoad = fn_800F7F90()->mSaveLoad;
    GameSaveData_unk* save;
    if (saveLoad != 0 && (save = (GameSaveData_unk*)saveLoad->m48) != 0) {
        int chara = own->mB0 + 700000;
        {
            NetMsgI_unk<PPMsgEquip_unk> msg(0x1009);
            PPMsgEquip_unk* data = &msg.mData;
            SendEquip_unk(save, own->mEquip0, data, chara, 0);
            SendEquip_unk(save, own->mEquip1, data, chara, 1);
            SendEquip_unk(save, own->mEquip2, data, chara, 2);
            SendEquip_unk(save, own->mEquip3, data, chara, 3);
        }
        NetMsgI_unk<PPMsgWeapon_unk> msg(0x100A);
        PPMsgWeapon_unk* data = &msg.mData;
        SendWeapon_unk(save, own->mD38[0], data, chara, 0);
        SendWeapon_unk(save, own->mD38[1], data, chara, 1);
    }
    NetMsgI_unk<unsigned char> msg(0x1041);
    unsigned char* data = &msg.mData;
    for (int i = 0; i < mgr->mPlayers.size(); i++) {
        if (mgr->mPlayers[i].mFlags.mActive == 1 && own->mAid != mgr->mPlayers[i].mAid) {
            unsigned char seq = own->m0D52++;
            *data = seq;
            mgr->m99F4[mgr->mPlayers[i].mAid] = seq;
            fn_804AE3BC()->fn_8050E094(mgr->mPlayers[i].mAid, 0x1041, data, 1);
            fn_804F5A98((PPRequests_unk*)lbl_8087F610, mgr->mPlayers[i].mAid, 300,
                        mgr->m99F4[mgr->mPlayers[i].mAid]);
        }
    }
    fn_804AE3BC()->fn_8050DDDC(-1, 1);
}

// NONMATCHING: 94.97% - the original reloads lbl_8087F610 for each inlined search of the queue (here reused) and
// registers
// inferred: answers of the other players to the requests of their sequence m99F4 (per aid): a request that timed
// out (-2) or failed (-1) closes the room with the error 158, an answered one (1) is freed and marked -2; when
// every other player answered, m9A74[1] is set
extern "C" void fn_804D90E0(PPManager2_unk* mgr) {
    int done = 0;
    int count = 0;
    PPPlayer_unk* own = PPFindSlot_unk(mgr, lbl_8087F628->GetMyAid_unk());
    for (unsigned int i = 0; i < mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive != 1 || own->mAid == player->mAid) {
            continue;
        }
        count++;
        int* seq = &mgr->m99F4[player->mAid];
        if (*seq >= 0) {
            int state = fn_804F5E38((PPRequests_unk*)lbl_8087F610, (unsigned char)*seq);
            if (state == -2) {
                *seq = -3;
                fn_804FB470(mgr, 158, 9);
            } else if (state == -1) {
                fn_804F5C9C((PPRequests_unk*)lbl_8087F610, (unsigned char)*seq);
                *seq = -3;
                fn_804FB470(mgr, 158, 9);
            } else if (state == 1) {
                fn_804F5C9C((PPRequests_unk*)lbl_8087F610, (unsigned char)*seq);
                *seq = -2;
            }
        }
        if (*seq == -2) {
            done++;
        }
    }
    if (count == done) {
        mgr->m9A74[1] = 1;
    }
}

// inferred: the server starts the timer m99B8; a client sends the message 0x1042 to the server
extern "C" void fn_804D97F0(PPManager2_unk* mgr) {
    if (IsServer_unk() == 1) {
        mgr->m99B8 = 300;
    } else {
        fn_804AE3BC()->fn_8050E094(lbl_8087F628->GetServerAid_unk(), 0x1042, 0, 1);
        fn_804AE3BC()->fn_8050DDDC(lbl_8087F628->GetServerAid_unk(), 1);
    }
}

// inferred: the server waits until every other player confirmed (m9A14) while the timer m99B8 runs; without time
// left the room is closed with the error 158 (state 43); a client only marks m9A74[2]
extern "C" void fn_804D996C(PPManager2_unk* mgr) {
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    if (IsServer_unk() == 1) {
        if (mgr->m99B8 <= 0) {
            fn_804FB470(mgr, 158, 9);
        } else {
            int ready = 0;
            int count = 0;
            for (int i = 0; i < mgr->mPlayers.size(); i++) {
                PPPlayer_unk* player = &mgr->mPlayers[i];
                if (player->mFlags.mActive == 1 && own->mAid != player->mAid) {
                    count++;
                    if (mgr->m9A14[player->mAid] == 1) {
                        ready++;
                    }
                }
            }
            if (count == ready) {
                mgr->m9A74[2] = 1;
            }
        }
    } else {
        mgr->m9A74[2] = 1;
    }
}

// inferred: sets the room of the next game and the count down
extern "C" int fn_804D9BF4(PPManager2_unk* mgr, int a, int b, int c) {
    mgr->m348 = a;
    mgr->m34C = b;
    mgr->m350 = c;
    mgr->m99A0 = 90;
    return 1;
}

// inferred: number of hosts of the session (same body as the weak copy fn_804D6300, inlined here)
static inline int GetNumHosts_unk(atn::net::Manager* net) {
    if (net->mC215 == 0) {
        return net->mFlags.mB12 ? 1 : DWC_GetNumConnectionHost();
    } else {
        return net->mFlags.mB12 ? 1 : fn_806A8E70();
    }
}

// NONMATCHING: 94.17% - registers (the loop offset and the address of the sequence; the zero of the inlined
// fn_804F5C9C is copied from the counter in the original)
// inferred: answers of the other players to the request of message 0x1040 (sequence m9A54 per aid, -1 when
// none): a request that timed out (-2) or failed (-1) closes the room with the error 158; an answered one (1),
// or any when this machine is alone, is freed and marked -2. Always 1
extern "C" int fn_804D9C14(PPManager2_unk* mgr, PPPlayer_unk* own, int arg) {
    for (int i = 0; i < mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive != 1 || own->mAid == player->mAid) {
            continue;
        }
        int* seq = &mgr->m9A54[player->mAid];
        if (*seq < 0) {
            continue;
        }
        int state = fn_804F5E38((PPRequests_unk*)lbl_8087F610, (unsigned char)*seq);
        if (state == -2) {
            *seq = -3;
            fn_804FB470(mgr, 158, 9);
        } else if (state == -1) {
            fn_804F5C9C((PPRequests_unk*)lbl_8087F610, (unsigned char)*seq);
            *seq = -3;
            fn_804FB470(mgr, 158, 9);
        } else {
            bool done = false;
            if (state == 1) {
                done = true;
            } else if (GetNumHosts_unk(lbl_8087F628) <= 1) {
                done = true;
            }
            if (done == true) {
                fn_804F5C9C((PPRequests_unk*)lbl_8087F610, (unsigned char)*seq);
                *seq = -2;
            }
        }
    }
    return 1;
}

// inferred: counts down m99A0 (shown by the lobby); 1 when it ends (or ended) and `keep` is 0
extern "C" bool fn_804DA2D0(PPManager2_unk* mgr, int keep) {
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

extern "C" int rand();
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames); // inferred: fades the sound in
// inferred: sounds of the start of the count down (one of them at random)
// inferred: voices said at random during the game (fn_804E5BC4)
// inferred: voices said when a character is knocked down (fn_804EB934)
const char* lbl_80790FF0[10] = {
    "VO_ARCOM_010", "VO_ARCOM_020", "VO_ARCOM_030", "VO_ARCOM_040", "VO_ARCOM_050",
    "VO_ARCOM_060", "VO_ARCOM_070", "VO_ARCOM_071", "VO_ARCOM_080", "VO_ARCOM_090",
};
const char* lbl_80791018[36] = {
    "VO_AR001_040", "VO_AR001_050", "VO_AR001_100", "VO_AR001_120", "VO_AR001_150", "VO_AR001_160",
    "VO_AR001_170", "VO_AR001_180", "VO_AR001_200", "VO_AR001_230", "VO_AR002_020", "VO_AR002_040",
    "VO_AR002_080", "VO_AR002_150", "VO_AR002_160", "VO_AR002_230", "VO_AR002_270", "VO_AR002_280",
    "VO_AR002_290", "VO_AR002_310", "VO_AR002_320", "VO_AR002_350", "VO_AR002_430", "VO_AR002_450",
    "VO_AR003_012", "VO_AR003_030", "VO_AR003_120", "VO_AR003_230", "VO_AR003_360", "VO_AR003_490",
    "VO_AR003_580", "VO_AR003_630", "VO_AR003_640", "VO_AR003_650", "VO_AR003_670", "VO_AR003_680",
};
const char* lbl_807910A8[] = {"SE_DG009_007_MP", "SE_DG009_012_MP"};
// inferred: plays one of the sounds lbl_807910A8 at random in m2B78
extern "C" void fn_804DA3F4(PPManager2_unk* mgr) {
    fn_800CB5C8(&mgr->m2B78, 15, 0);
    mgr->m2B78 = fn_800C31F4(lbl_807910A8[rand() & 1], 0, -1, 1.0f);
    fn_800CB518(&mgr->m2B78, 45);
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

#include "game/AiTarget.h"
extern "C" int fn_804EB4AC(PPManager2_unk* mgr, PPPlayer_unk* player);
// inferred: `player` is in the team of the own player: inline copy of fn_804EB4AC that reads the manager through
// its getter (the registers of the inlined search in fn_804DA4A0 and fn_804E8BF4 follow it; fn_804EB4AC itself
// reads lbl_8087F610 directly). Name invented
static inline int PPIsOwnTeam_unk(PPPlayer_unk* player) {
    PPManager2_unk* m = fn_800F7F90();
    PPPlayer_unk* me = fn_804DD158(m, lbl_8087F628->GetMyAid_unk());
    if (me == 0 || player == 0 || !IsTeamPlayer_unk(player)) {
        return 0;
    }
    return me->mAid == (player->mAid & 0xF);
}
extern "C" CharaTask* fn_804DD3F0(PPManager2_unk* mgr);

// inferred: the characters of the players of the own team (but the one of fn_804DD3F0) are given back to the AI:
// with `random` set they get a random action (0x27E5 or 0x27E6) and target themselves
extern "C" void fn_804DA4A0(PPManager2_unk* mgr, int random) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive && player->mRef.mChara != 0 && PPIsOwnTeam_unk(player)) {
            CharaTask* chara = player->mRef.mChara;
            if (chara != fn_804DD3F0(mgr)) {
                if (random != 0) {
                    player->mRef.mChara->fn_8017ABD4(rand() % 2 == 0 ? 0x27E5 : 0x27E6);
                    lbl_8087EE68->fn_800183E0(player->mRef.mChara, player->mRef.mChara, 0, 0);
                } else {
                    chara->fn_8017ABD4(0);
                    lbl_8087EE68->fn_80018608(player->mRef.mChara, 0);
                }
            }
        }
    }
}

#include "game/GameUI.h"
#include "game/Inventory.h"
extern "C" int fn_80444020(Inventory* inventory, int table, int message, int index, CharaTask* chara, int arg,
                           int* outCode, int* outCount, int* outAdded); // inferred: gives an item of a table
extern "C" void fn_804DF4B8(PPManager2_unk* mgr);
extern "C" int fn_804E4B44(PPManager2_unk* mgr, PPPlayer_unk* target);
extern "C" int fn_804DB408(const PPManager2_unk* mgr);

// inferred: team of the own player (3 without slot); names invented
static inline unsigned int PPOwnTeam_unk(PPManager2_unk* mgr) {
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    return own != 0 ? own->mFlags.m22 : 3;
}
// inferred: team that the own team plays against (the own one out of the team mode); names invented
static inline unsigned int PPOtherTeam_unk(PPManager2_unk* mgr) {
    if (mgr->mMode != 1) {
        return PPOwnTeam_unk(mgr);
    }
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    if (own != 0) {
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = &mgr->mPlayers[i];
            if (player->mFlags.mActive == 1 && player->mRef.mChara != 0 && player->mFlags.m22 != own->mFlags.m22) {
                return player->mFlags.m22;
            }
        }
    }
    return 3;
}
// inferred: players in use of the team `team`; names invented
static inline int PPCountTeam_unk(PPManager2_unk* mgr, unsigned int team) {
    int count = 0;
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive && player->mRef.mChara != 0 && player->mFlags.m22 == team) {
            count++;
        }
    }
    return count;
}
static inline int PPTeamScore_unk(const PPManager2_unk* mgr, int team);

// NONMATCHING: 98.58% - registers (the original keeps m564 of the first fn_804DB408 in a saved register)
// inferred: rewards of the end of the game: an item by the rank of the own player (another set when less than a
// minute was left) and, in the team mode, two items when the own team had fewer players and lost by 40 or more
extern "C" void fn_804DA690(PPManager2_unk* mgr) {
    if (lbl_8087F490 == 0 || Inventory::instance_ == 0 || fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) == 0) {
        return;
    }
    fn_804DF4B8(mgr);
    int rank = fn_804E4B44(mgr, fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()));
    int code = 0;
    int count = 0;
    bool last = fn_804DB408(mgr) >= 0 && fn_804DB408(mgr) < 60;
    if (!last) {
        if (0 <= rank && rank < 2) {
            fn_80444020(Inventory::instance_, 0x1E79, 0, -1, 0, 1, &code, &count, 0);
        }
        if (2 <= rank && rank < 4) {
            fn_80444020(Inventory::instance_, 0x1E7A, 0, -1, 0, 1, &code, &count, 0);
        } else if (rank == 4) {
            fn_80444020(Inventory::instance_, 0x1E7B, 0, -1, 0, 1, &code, &count, 0);
        } else if (rank >= 5) {
            fn_80444020(Inventory::instance_, 0x1E7C, 0, -1, 0, 1, &code, &count, 0);
        }
    } else {
        if (0 <= rank && rank < 2) {
            fn_80444020(Inventory::instance_, 0x1E7D, 0, -1, 0, 1, &code, &count, 0);
        }
        if (2 <= rank && rank < 4) {
            fn_80444020(Inventory::instance_, 0x1E7E, 0, -1, 0, 1, &code, &count, 0);
        } else if (rank == 4) {
            fn_80444020(Inventory::instance_, 0x1E7F, 0, -1, 0, 1, &code, &count, 0);
        } else if (rank >= 5) {
            fn_80444020(Inventory::instance_, 0x1E80, 0, -1, 0, 1, &code, &count, 0);
        }
    }
    if (code > 0 && count > 0) {
        fn_800C31F4("SE_UICOM_008", 0, -1, 1.0f);
        lbl_8087F490->fn_803E5E64(code, count, 1);
    }
    if (mgr->mMode != 1) {
        return;
    }
    int own = PPCountTeam_unk(mgr, PPOwnTeam_unk(mgr));
    int other = PPCountTeam_unk(mgr, PPOtherTeam_unk(mgr));
    if (own >= other) {
        return;
    }
    int ownScore = PPTeamScore_unk(mgr, PPOwnTeam_unk(mgr));
    if (PPTeamScore_unk(mgr, PPOtherTeam_unk(mgr)) - ownScore >= 40) {
        fn_80444020(Inventory::instance_, 0x1C23, 0, -1, 0, 0, 0, 0, 0);
        fn_80444020(Inventory::instance_, 0x1C26, 0, -1, 0, 0, 0, 0, 0);
    }
}

#include "game/PopupTask.h"
static inline PPPlayer_unk* FindByChara_unk(PPManager2_unk* mgr, CharaTask* chara);

// NONMATCHING: 97.74% - the original loads mDCScore twice (sum and old value) and registers of the inlined
// search of the own player
// inferred: adds `points` to the score of the player of `chara` (not below 0); for the own player out of mode 2
// a popup over the character shows the change (called by gmk::DropItem when an item is picked up)
extern "C" void fn_804DB1C4(PPManager2_unk* mgr, CharaTask* chara, int points) {
    if (mgr->m50C == 0) {
        PPPlayer_unk* player = FindByChara_unk(mgr, chara);
        if (player != 0 && player->mRef.mChara != 0) {
            int score = player->mDCScore + points;
            int old = player->mDCScore;
            player->mDC.fn_80501288((CharaTask*)(score > 0 ? score : 0));
            if (mgr->mMode != 2 && player == fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk())) {
                int now = player->mDCScore;
                if (old != now) {
                    PopupTask* popup = lbl_8087F8A8;
                    if (popup != 0) {
                        // inferred: the address of the returned position is passed as is (MWCC accepts it): with a
                        // named local the function is not auto-inlined by fn_804E4EB0, fn_804EB934 and fn_804F3EEC
                        popup->fn_8054D794(8, L"", &player->mRef.mChara->vf58(), now - old, 0, 1, 0, 0, 0, 1, -1, -1);
                    }
                }
            }
        }
    }
}

// inferred: seconds since the start of the game
extern "C" int fn_804DB3B4(const PPManager2_unk* mgr) {
    return (OSGetTime() - mgr->mStartTime) / (*(unsigned int*)0x800000F8 / 4);
}

// NONMATCHING: 92.59% - the original loads m564 before saving r31
// inferred: seconds left of the time limit (-1 without one)
extern "C" int fn_804DB408(const PPManager2_unk* mgr) {
    if (mgr->m564 >= 0) {
        return mgr->m564 - fn_804DB3B4(mgr);
    }
    return -1;
}

extern "C" const char* fn_8050F8B0(MpFriendList_unk*, int); // inferred: name of a matchmaking key

// inferred: key of the filter with an int value
inline atn::net::NetFilterKey_unk::NetFilterKey_unk(const char* key, int value, unsigned int flags)
    : mKey(key), mType(2), mInt(value), mFlags(flags) {}

// inferred: sets an int key of the matchmaking filter (name invented)
inline void SetKeyInt_unk(int i, const char* name, int value, unsigned int flags) {
    atn::net::Manager* net = lbl_8087F628;
    net->mKeys[i] = atn::net::NetFilterKey_unk(name, value, flags);
}

// NONMATCHING: 99.44% - r30/r31 of the manager and the friend list swapped, registers of the fourth key
// inferred: sets the four int keys of the matchmaking filter (mode, m54C, m550 and a value of the friend list) and
// starts the search
extern "C" int fn_804DB474(PPManager2_unk* mgr) {
    atn::net::Manager* net = lbl_8087F628;
    MpFriendList_unk* list = &net->mFriendList;
    for (int i = 0; i < 8; i++) {
        net->mKeys[i].mType = 0;
    }
    SetKeyInt_unk(0, fn_8050F8B0(list, 0), mgr->mMode, 0);
    SetKeyInt_unk(1, fn_8050F8B0(list, 1), mgr->m54C, 0);
    SetKeyInt_unk(2, fn_8050F8B0(list, 2), mgr->m550, 1);
    SetKeyInt_unk(3, fn_8050F8B0(list, 3), list->m1E8, 1);
    int result = lbl_8087F628->fn_80509B20(0);
    fn_804FB220(&mgr->mStateVar, 11);
    return result;
}

// inferred: resets the player slots in the modes 0 and 2 and ends the session (mode 3) or the match (0, 1)
extern "C" int fn_804DBD08(PPManager2_unk* mgr, int mode) {
    if (mode == 0 || mode == 2) {
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = &mgr->mPlayers[i];
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

// inferred: room status written in the first field of the status string (by fn_804DC450)
const char* lbl_80791190[] = {"0", "1", "2", "3", "4", "5"};
// inferred: game mode written in the second field of the status string (names of modes 0-2)
const char* lbl_807911A8[] = {"0", "1", "2"};
// inferred: field of the game mode (0 with no mode, "9" off line). Not inline: inlined by MWCC (auto inline) in
// fn_804DC114, fn_804DC450 and fn_804DC684, and its own copy removed by the linker; its "9" goes to
// @stringBase0 here, before the "," of fn_804DBE24, as in the original
const char* GetModeField_unk(int mode) {
    if (mode >= 3) {
        return 0;
    }
    if (mode < 0) {
        return "9";
    }
    return lbl_807911A8[mode];
}

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
            fields[i] = 0;
        } else {
            fields[i] = text;
        }
        *end = 0;
        text = end + 1;
    }
    return 1;
}

extern "C" const wchar_t* fn_8050F858(MpFriendList_unk*, int, unsigned char*, int*);
// inferred: reads a message of a friend from a copy of the friend list and splits its fields
extern "C" int fn_804DBEC8(PPManager2_unk* mgr, int id, char* buf, char** fields, int* arg) {
    MpFriendList_unk list = lbl_8087F628->mFriendList;
    if (fn_8050F858(&list, id, (unsigned char*)buf, arg) != 0) {
        return fn_804DBE24(mgr, buf, fields);
    }
    return 0;
}

// inferred: messages sent in each frame of a cycle of 30 (bit 1: character states, fn_804E6518)
int lbl_80790F78[30] = {14, 0, 2, 0, 2, 0, 14, 0, 2, 0, 2, 0, 14, 0, 2, 0, 2, 0, 14, 0, 2, 0, 2, 0, 14, 0, 2, 0, 2, 0};
// inferred: room status (1-5, the first field) of the status message of the friend `id` if its game mode matches
// the own one (-1 if not)
extern "C" int fn_804DC114(PPManager2_unk* mgr, int id) {
    int arg;
    char* fields[6];
    char buf[0x100];
    if (fn_804DBEC8(mgr, id, buf, fields, &arg) == 0) {
        return -1;
    }
    if (arg == 0) {
        return -1;
    }
    const char* own;
    const char* status;
    int mode = mgr->mMode;
    if ((unsigned int)mode > 2) {
        own = 0;
    } else {
        if (lbl_8087F628->m25C != 1) {
            mode = -1;
        }
        own = GetModeField_unk(mode);
    }
    const char* field = fields[1];
    if (strcmp(field, "9") == 0 || strcmp(own, "9") == 0) {
        return -1;
    }
    if (strcmp(field, lbl_807911A8[2]) == 0) {
        if (strcmp(field, own) != 0) {
            return -1;
        }
    } else {
        const char* mode0 = lbl_807911A8[0];
        if (strcmp(field, mode0) == 0 || strcmp(field, lbl_807911A8[1]) == 0) {
            if (strcmp(own, mode0) != 0 && strcmp(own, lbl_807911A8[1]) != 0) {
                return -1;
            }
        }
    }
    status = fields[0];
    if (strcmp(status, lbl_80791190[1]) == 0) {
        return 1;
    }
    if (strcmp(status, lbl_80791190[2]) == 0) {
        return 2;
    }
    if (strcmp(status, lbl_80791190[3]) == 0) {
        return 3;
    }
    if (strcmp(status, lbl_80791190[4]) == 0) {
        return 4;
    }
    if (strcmp(status, lbl_80791190[5]) != 0) {
        return -1;
    }
    return 5;
}



// inferred: updates the room status (2 when the network is not local)
extern "C" void fn_804DC358(PPManager2_unk* mgr) {
    int mode = 2;
    if (lbl_8087F628->m90 == 1) {
        mode = 1;
    }
    fn_804DC450(mgr, mode);
}

extern "C" int DWC_GetOwnStatusString(char* status);
// inferred: the status string says room status 1 or 5 (as `mode`); it is written again when it does not
extern "C" int fn_804DC374(PPManager2_unk* mgr, int mode) {
    if (lbl_8087F628->m90 == 0) {
        return 0;
    }
    char* fields[6];
    char text[0x100];
    DWC_GetOwnStatusString(text);
    if (fn_804DBE24(mgr, text, fields) == 0) {
        return 0;
    }
    if (mode == 1) {
        if (strcmp(fields[0], lbl_80791190[1]) != 0) {
            fn_804DC450(mgr, 1);
        }
    } else if (mode == 5) {
        if (strcmp(fields[0], lbl_80791190[5]) != 0) {
            fn_804DC450(mgr, 5);
        }
    }
    return 1;
}

extern "C" int fn_8050F5A8(MpFriendList_unk*, int);  // inferred: first friend (-1: none)
extern "C" int fn_8050F664(MpFriendList_unk*, int);  // inferred: next friend (-1: none)
extern "C" unsigned long long fn_8050F764(MpFriendList_unk*, int); // inferred: friend code
extern "C" const wchar_t* fn_8050F724(MpFriendList_unk*, int);     // inferred: name
extern "C" void fn_8050F734(MpFriendList_unk*, int, const wchar_t*); // inferred: sets the name
extern "C" std::basic_string<wchar_t> fn_8050F470(unsigned long long); // inferred: code as text
extern "C" void fn_8050EAE8(MpFriendList_unk*);
extern "C" void fn_8006F420(void*, char*, const wchar_t*, int); // inferred: wide to multibyte
extern "C" int fn_8006F2F0(void*, wchar_t*, const char*, int); // inferred: to wide text
extern "C" int wcscmp(const wchar_t*, const wchar_t*);

extern "C" char* fn_800DC97C(int value, char* buf, int radix); // inferred: itoa (atn/Util_unk.cpp)
extern "C" std::basic_string<wchar_t> fn_8050F464(MpFriendList_unk*); // inferred: own friend code as text
extern "C" int DWC_SetOwnStatusString(const char* status);
// inferred: writes the own status string: room status `status`, game mode, name, number of hosts, 6 and friend code
extern "C" int fn_804DC450(PPManager2_unk* mgr, int status) {
    if (mgr->mState == -1) {
        return 0;
    }
    const char* room;
    if (status < 0 || status >= 6U) {
        room = 0;
    } else {
        room = lbl_80791190[status];
    }
    const char* own;
    int mode = mgr->mMode;
    if ((unsigned int)mode > 2) {
        own = 0;
    } else {
        if (lbl_8087F628->m25C != 1) {
            mode = -1;
        }
        own = GetModeField_unk(mode);
    }
    char text[0x100];
    char name[0x18];
    char code[0x20];
    char hosts[8];
    char six[8];
    MpFriendList_unk* list = &lbl_8087F628->mFriendList;
    fn_8006F420(lbl_8087EEC8, name, list->mName, 0x11);
    fn_800DC97C(GetNumHosts_unk(lbl_8087F628), hosts, 10);
    fn_800DC97C(6, six, 10);
    fn_8006F420(lbl_8087EEC8, code, fn_8050F464(list).c_str(), fn_8050F464(list).size() * 2);
    sprintf(text, "%s,%s,%s,%s,%s,%s", room ? room : "", own ? own : "", name, hosts, six, code);
    return DWC_SetOwnStatusString(text);
}

// inferred: writes the own status string: room status 0, mode "9" (off line, GetModeField_unk), name (none in state 0x8B) and friend code
extern "C" __declspec(noinline) bool fn_804DC684(PPManager2_unk* mgr) {
    if (mgr->mState == -1) {
        return 0;
    }
    char status[0x100];
    char name[0x18];
    char code[0x20];
    MpFriendList_unk* list = &lbl_8087F628->mFriendList;
    if (mgr->m9750 != 0x8B) {
        fn_8006F420(lbl_8087EEC8, name, list->mName, 0x11);
    } else {
        name[0] = 0;
    }
    fn_8006F420(lbl_8087EEC8, code, fn_8050F464(list).c_str(), fn_8050F464(list).size() * 2);
    sprintf(status, "%s,%s,%s,%s,%s,%s", lbl_80791190[0], GetModeField_unk(-1), name, "", "", code);
    return DWC_SetOwnStatusString(status) != 0;
}

extern "C" int atoi(const char* s);
// inferred: fourth and fifth fields of a status string as numbers
extern "C" int fn_804DC7C0(PPManager2_unk* mgr, int id) {
    char* fields[6];
    char buf[0x100];
    if (fn_804DBEC8(mgr, id, buf, fields, 0) == 0 || fields[3] == 0) {
        return 0;
    }
    return atoi(fields[3]);
}

extern "C" int fn_804DC80C(PPManager2_unk* mgr, int id) {
    char* fields[6];
    char buf[0x100];
    if (fn_804DBEC8(mgr, id, buf, fields, 0) == 0 || fields[4] == 0) {
        return 0;
    }
    return atoi(fields[4]);
}


// inferred: the status message of the friend `id` carries its own friend code (field 5)
static inline bool IsOwnStatus_unk(PPManager2_unk* mgr, int id) {
    char* fields[6];
    char buf[0x100];
    if (fn_804DBEC8(mgr, id, buf, fields, 0) == 0 || fields[5] == 0) {
        return 0;
    }
    char code[0x18];
    unsigned long long key = fn_8050F764(&lbl_8087F628->mFriendList, id);
    fn_8006F420(lbl_8087EEC8, code, fn_8050F470(key).c_str(), fn_8050F470(key).size() * 2);
    return strcmp(code, fields[5]) == 0;
}

// NONMATCHING: 99.68% - r30/r31 of the friend list and of the hoisted text pointer of the temporary string swapped
// inferred: takes the names of the friends from their status messages (field 2) and saves the list if one changed
extern "C" void fn_804DC858(PPManager2_unk* mgr) {
    MpFriendList_unk* list;
    int changed = 0;
    list = &lbl_8087F628->mFriendList;
    for (int id = fn_8050F5A8(list, 0); id != -1; id = fn_8050F664(list, 0)) {
        if (IsOwnStatus_unk(mgr, id) == 1) {
            char* fields[6];
            char buf[0x100];
            int arg;
            if (fn_804DBEC8(mgr, id, buf, fields, &arg) != 0) {
                wchar_t name[0x100];
                fn_8006F2F0(lbl_8087EEC8, name, fields[2], 0x100);
                if (wcscmp(name, fn_8050F724(list, id)) != 0) {
                    fn_8050F734(list, id, name);
                    changed = 1;
                }
            }
        }
    }
    if (changed) {
        fn_8050EAE8(&lbl_8087F628->mFriendList);
        MP::SaveLoad* saveLoad = ((PPManager2_unk*)lbl_8087F610)->mSaveLoad;
        if (saveLoad != 0) {
            saveLoad->fn_80502870(1, list, 0x9F0);
        }
    }
}

extern "C" int fn_804DCA4C() {
    return 0;
}

extern "C" int fn_8050BA68(unsigned char** aids); // inferred: network ids of the hosts (count)
// inferred: network id of the host `i` (0 in local play, 0xFF past the end); name invented
static inline unsigned int GetHostAid_unk(int i) {
    if (lbl_8087F628->mFlags.mB12) {
        return 0;
    }
    unsigned char* aids;
    if (fn_8050BA68(&aids) > i) {
        return aids[i];
    }
    return 0xFF;
}

extern "C" int fn_804DD574(PPManager2_unk* mgr, unsigned int aid, int arg);

// inferred: payload of the message 0x1034 (the tables m9984 and m998C); names invented
struct PPMsgCpuTable_unk {
    unsigned char m00[8];
    unsigned char m08[8];
};
// inferred: on the server: the free places of the room (6, 8 or 7 by game mode, less the hosts, at most m5A4)
// get a computer player: a side (mode 1: the one with fewer players, at random when even), a host of the other
// side that has none yet (or the one with fewest) in m9984 (index + 1 in the high nibble), the side in m9994
// and a stage from fn_804DD574 in m998C; the tables go to everybody (message 0x1034)
// inferred: the player is of the team `team` (inlined in fn_804DCA54)
static inline bool PPInTeam_unk(PPPlayer_unk* player, unsigned int team) {
    return player->mFlags.m22 == team;
}
// inferred: players of each team in use (inlined in fn_804DCA54)
static inline void PPCountTeams_unk(int* counts) {
    PPManager2_unk* mgr2 = (PPManager2_unk*)lbl_8087F610;
    for (int k = 0; k < (int)((PPManager2_unk*)lbl_8087F610)->mPlayers.size(); k++) {
        PPPlayer_unk* other = fn_804D5F0C(mgr2, k);
        if (other->mFlags.mActive == 1) {
            counts[other->mFlags.m22]++;
        }
    }
}
extern "C" void fn_804DCA54(PPManager2_unk* mgr) {
    int hosts = GetNumHosts_unk(lbl_8087F628);
    int num;
    switch (mgr->mMode) {
    case 2:
        num = 7;
        break;
    case 1:
        num = 8;
        break;
    case 0:
        num = 6;
        break;
    }
    num -= hosts;
    if (num > mgr->m5A4) {
        num = mgr->m5A4;
    }
    memset(mgr->m9984, 0, 8);
    memset(mgr->m998C, 0xFF, 8);
    memset(mgr->m9994, 0, 8);
    NetMsgI_unk<PPMsgCpuTable_unk> msg(0x1034);
    PPMsgCpuTable_unk* data = &msg.mData;
    int firstA = 0;
    int firstB = 0;
    if (num > 0) {
        int counts[3];
        memset(counts, 0, sizeof(counts));
        PPCountTeams_unk(counts);
        for (int i = 0; i < num; i++) {
            bool first;
            if (counts[1] < counts[2]) {
                first = true;
            } else if (counts[1] > counts[2]) {
                first = false;
            } else {
                first = (int)(2.0f * (rand() / 32767.0f)) != 0;
            }
            unsigned int team;
            if (mgr->mMode == 1) {
                if (first == true) {
                    counts[1]++;
                    team = 2;
                    mgr->m9994[i] = 1;
                } else {
                    counts[2]++;
                    team = 1;
                    mgr->m9994[i] = 2;
                }
                atn::net::Manager* net = lbl_8087F628;
                if (net->mC215 == 1 && net->mC218 == 1) {
                    team = fn_804DD158(mgr, 0)->mFlags.m22;
                }
            } else {
                team = 2;
                mgr->m9994[i] = team;
            }
            int done = 0;
            for (int k = 0; k < hosts; k++) {
                unsigned int aid = GetHostAid_unk(k);
                PPPlayer_unk* host = fn_804DD158(mgr, aid);
                if (host == 0 || host->mFlags.m22 != team) {
                    continue;
                }
                unsigned char v;
                bool used = false;
                for (int j = 0; j < 8; j++) {
                    v = mgr->m9984[j];
                    if ((v & 0xF0) != 0 && (v & 0xF) == aid) {
                        used = true;
                        break;
                    }
                }
                if (used) {
                    continue;
                }
                mgr->m9984[i] = ((i + 1) << 4) | aid;
                done = 1;
                break;
            }
            if (done == 0) {
                unsigned char cpus[8];
                memset(cpus, 0, sizeof(cpus));
                for (int j = 0; j < 8; j++) {
                    unsigned char v = mgr->m9984[j];
                    if ((v & 0xF0) != 0) {
                        unsigned int aid = v & 0xF;
                        if (PPInTeam_unk(fn_804DD158(mgr, aid), team)) {
                            cpus[aid]++;
                        }
                    }
                }
                int fewest = 0xFF;
                int best = -1;
                for (int j = 0; j < 8; j++) {
                    if (cpus[j] != 0 && fewest > cpus[j]) {
                        fewest = cpus[j];
                        best = j;
                    }
                }
                mgr->m9984[i] = ((i + 1) << 4) | best;
            }
            int leader = 0;
            if (mgr->mMode != 0) {
                if (first == true) {
                    if (firstA == 0) {
                        leader = 1;
                        firstA++;
                    }
                } else if (firstB == 0) {
                    leader = 1;
                    firstB++;
                }
            }
            if (mgr->m5B0 == 0) {
                leader = 0;
            }
            mgr->m998C[i] = fn_804DD574(mgr, mgr->m9984[i], leader);
        }
    }
    memcpy(data->m00, mgr->m9984, 8);
    memcpy(data->m08, mgr->m998C, 8);
    fn_804AE3BC()->fn_8050E094(-1, 0x1034, data, 1);
}

// inferred: the player slot of network id `aid` (slots in use only)
extern "C" PPPlayer_unk* fn_804DD158(PPManager2_unk* mgr, unsigned int aid) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        if (mgr->mPlayers[i].mFlags.mActive == 1 && aid == mgr->mPlayers[i].mAid) {
            return &mgr->mPlayers[i];
        }
    }
    return 0;
}

// inferred: team of network id `aid` (1 or 2); in game mode 1 with the local play the table m9984/m9994 decides
extern "C" int fn_804DD1A8(PPManager2_unk* mgr, int aid) {
    PPPlayer_unk* player = fn_804DD158(mgr, aid & 0xF);
    if (mgr->mMode == 1) {
        atn::net::Manager* net = lbl_8087F628;
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

// inferred: order of the stages chosen in the lobby (game mode 2 and the others)
extern "C" const int lbl_80759BD4[21] = {0, 6, 1, 2, 3, 5, 4, 8, 7, 10, 9, 11, 13, 19, 20, 14, 15, 23, 17, 18, 22};
// inferred: the stages of each side of a team game (fn_804DD574; the last entry is not read there)
extern "C" const int lbl_80759C28[16] = {0, 1, 2, 7, 8, 9, 10, 11, 13, 14, 17, 18, 22, 23, 19, 0};
extern "C" const int lbl_80759C68[6] = {3, 4, 5, 6, 15, 20};
extern "C" const int lbl_80759C80[8] = {0, 6, 1, 2, 3, 5, 4, 21};
extern "C" const int lbl_80759CA0[4] = {0, 1, 2, 0};
extern "C" const int lbl_80759CB0[4] = {3, 4, 5, 6};

// inferred: next stage after `*stage` in the order of the game mode (-1 after the last one, the first one
// when `*stage` is not in the order)
extern "C" void fn_804DD294(PPManager2_unk* mgr, int arg, int* stage) {
    const int* table;
    int num;
    if (mgr->mMode == 2) {
        table = lbl_80759C80;
        num = 7;
    } else {
        table = lbl_80759BD4;
        num = 21;
    }
    bool found = false;
    bool done = false;
    for (int i = 0; i < num; i++) {
        if (found == 1) {
            done = true;
            *stage = table[i];
            break;
        }
        if (*stage == table[i]) {
            found = true;
        }
    }
    if (done) {
        return;
    }
    if (found) {
        *stage = -1;
    } else {
        *stage = table[0];
    }
}

// inferred: previous stage (as fn_804DD294 backwards)
extern "C" void fn_804DD33C(PPManager2_unk* mgr, int arg, int* stage) {
    int num;
    const int* table;
    if (mgr->mMode == 2) {
        num = 7;
        table = lbl_80759C80;
    } else {
        num = 21;
        table = lbl_80759BD4;
    }
    bool found = false;
    bool done = false;
    for (int i = num - 1; i >= 0; i--) {
        if (found == 1) {
            done = true;
            *stage = table[i];
            break;
        }
        if (*stage == table[i]) {
            found = true;
        }
    }
    if (done) {
        return;
    }
    if (found) {
        *stage = -1;
    } else {
        *stage = *(table + num - 1);
    }
}

// inferred: character of a team player on the side of the own character whose slot value mB0 is 12 or 21
extern "C" CharaTask* fn_804DD3F0(PPManager2_unk* mgr) {
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    if (own == 0 || own->mRef.mChara == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive && player->mRef.mChara != 0 && IsTeamPlayer_unk(player) &&
            player->mRef.mChara->fn_8016F3D0(own->mRef.mChara, 1) && (player->mB0 == 12 || player->mB0 == 21)) {
            return player->mRef.mChara;
        }
    }
    return 0;
}

// NONMATCHING: 91.80% - offsets of the tables from the base of .rodata (0x47C bytes of .rodata before them are
// not written yet), registers of the search loops and the loop over the players is not turned into a ctr loop
// inferred: chooses the stage (slot value mB0) of the player `aid`: one that no player (and no entry of m998C)
// has yet, else any one, from the order of the game mode (for a team player, of the side 4/8 of m5B4); with
// `arg` 12 or 21 (21 in game mode 2); the own value of a main player is kept in its slot
extern "C" int fn_804DD574(PPManager2_unk* mgr, unsigned int aid, int arg) {
    PPPlayer_unk* player = fn_804DD158(mgr, aid & 0xF);
    if (player == 0) {
        return -1;
    }
    int team = player->mFlags.m22;
    if (!(team == 1 || team == 2)) {
        return -1;
    }
    unsigned int teamPlayer = aid & 0xF0;
    if (teamPlayer != 0) {
        fn_804DD1A8(mgr, aid);
    }
    int result;
    if (arg == 0) {
        const int* table;
        int num;
        if (teamPlayer != 0) {
            if (mgr->mMode == 2) {
                if (mgr->m5B4 & 4) {
                    table = lbl_80759CA0;
                    num = 3;
                } else if (mgr->m5B4 & 8) {
                    table = lbl_80759CB0;
                    num = 4;
                } else {
                    table = lbl_80759C80;
                    num = 7;
                }
            } else {
                if (mgr->m5B4 & 4) {
                    table = lbl_80759C28;
                    num = 15;
                } else if (mgr->m5B4 & 8) {
                    table = lbl_80759C68;
                    num = 6;
                } else {
                    table = lbl_80759BD4;
                    num = 21;
                }
            }
        } else if (mgr->mMode == 2) {
            table = lbl_80759C80;
            num = 7;
        } else {
            table = lbl_80759BD4;
            num = 21;
        }
        int counts[21];
        PPManager2_unk* mgr2 = fn_800F7F90();
        for (int i = 0; i < num; i++) {
            counts[i] = 0;
            for (int j = 0; j < (int)((PPManager2_unk*)lbl_8087F610)->mPlayers.size(); j++) {
                PPPlayer_unk* other = fn_804D5F0C(mgr2, j);
                if (other->mFlags.mActive == 1 && other->mB0 == table[i]) {
                    counts[i]++;
                }
            }
            for (int j = 0; j < 8; j++) {
                if (table[i] == mgr2->m998C[j]) {
                    counts[i]++;
                }
            }
        }
        int free = 0;
        for (int i = 0; i < num; i++) {
            if (counts[i] == 0) {
                free++;
            }
        }
        if (free != 0) {
            int pick = (int)(free * (rand() / 32767.0f));
            int n = 0;
            result = -1;
            for (int i = 0; i < num; i++) {
                if (counts[i] == 0) {
                    if (pick == n) {
                        result = table[i];
                        break;
                    }
                    n++;
                }
            }
        } else {
            result = table[(int)(num * (rand() / 32767.0f))];
        }
    } else if (mgr->mMode == 2) {
        result = 21;
    } else {
        result = 12;
        if ((int)(2.0f * (rand() / 32767.0f)) == 0) {
            result = 21;
        }
    }
    if (teamPlayer == 0) {
        player->mB0 = result;
    }
    return result;
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

extern "C" int rand();

// NONMATCHING: 94.90% - the original indexes the shuffle and the last loop as base + offset (here a moving pointer,
// one register less) and registers
// inferred: on the server: in a random order of the hosts, the players get a team: in mode 1 the one with fewer
// players (at random when even) unless they already have one, else team 2
extern "C" void fn_804DDA04(PPManager2_unk* mgr) {
    atn::FixedArray_unk<int, 20> order;
    for (int i = 0; i < GetNumHosts_unk(lbl_8087F628); i++) {
        order.push_back(i);
    }
    for (unsigned int i = 0; i < order.size(); i++) {
        int j = rand() % (i + 1);
        int tmp = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }
    for (unsigned int i = 0; i < order.size(); i++) {
        PPPlayer_unk* player = fn_804DD158(mgr, GetHostAid_unk(order[i]));
        if (player == 0) {
            continue;
        }
        if (mgr->mMode == 1) {
            int counts[3];
            memset(counts, 0, sizeof(counts));
            PPManager2_unk* mgr2 = (PPManager2_unk*)lbl_8087F610;
            PPPlayerArray_unk& players = mgr2->mPlayers;
            for (unsigned int k = 0; k < players.size(); k++) {
                PPPlayer_unk* other = fn_804D5F0C(mgr2, k);
                if (other->mFlags.mActive == 1) {
                    counts[other->mFlags.m22]++;
                }
            }
            if (player->mFlags.m22 != 0) {
                continue;
            }
            bool first;
            if (counts[2] < counts[1]) {
                first = false;
            } else if (counts[1] < counts[2]) {
                first = true;
            } else {
                first = (int)(2.0f * (rand() / 32767.0f)) != 0;
            }
            if (first == true) {
                counts[1]++;
                player->mFlags.m22 = 1;
            } else {
                counts[2]++;
                player->mFlags.m22 = 2;
            }
        } else {
            player->mFlags.m22 = 2;
        }
    }
}

// inferred: state of the manager and the value set when a player slot is added (weak copies)
extern "C" __declspec(noinline) inline int fn_804EE030(PPManager2_unk* mgr) {
    return mgr->mState;
}
extern "C" __declspec(noinline) inline void fn_804EE038(PPManager2_unk* mgr, short value) {
    mgr->m50A = value;
}

extern "C" int fn_804E4B44(PPManager2_unk* mgr, PPPlayer_unk* player);

extern "C" int fn_804E4C34(PPManager2_unk* mgr);
extern "C" int fn_804E4C5C(const PPManager2_unk* mgr);
extern "C" int fn_804E4B44(PPManager2_unk* mgr, PPPlayer_unk* target);

// inferred: the server asks the other players for their state (message 0x1047)
extern "C" void fn_804DDD50(PPManager2_unk* mgr) {
    unsigned char aid = lbl_8087F628->GetMyAid_unk();
    if (aid == lbl_8087F628->GetServerAid_unk()) {
        fn_804AE3BC()->fn_8050E094(-1, 0x1047, 0, 1);
        fn_804AE3BC()->fn_8050DDDC(-1, 1);
    }
}

extern "C" bool fn_804DE560(PPManager2_unk* mgr, int* count, bool* found);
// inferred: on the server: fn_804E9360, fn_804DE560 and the state of its own slot (mode 2) to everybody; 0 when
// fn_804DE560 fails
extern "C" int fn_804DDE34(PPManager2_unk* mgr) {
    if (IsServer_unk() == 1) {
        fn_804E9360(mgr);
        PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
        if (fn_804DE560(mgr, 0, 0) == 0) {
            return 0;
        }
        own->mFlags.m10 = 0;
        fn_804D824C(mgr, own, 2);
        fn_804AE3BC()->fn_8050DDDC(-1, 1);
    }
    return 1;
}

extern "C" void fn_804DDA04(PPManager2_unk* mgr);
extern "C" int fn_804DD574(PPManager2_unk* mgr, unsigned int aid, int arg);
extern "C" void fn_804DCA54(PPManager2_unk* mgr);

// NONMATCHING: 98.90% - registers of the inlined searches fn_804DD158 and of the loops (own/counter, aid/index)
// inferred: on the server: keeps the flags and m00B0 of every slot (mD4/mD8), fn_804DDA04, fn_804DD574 for the hosts
// whose slot has no m00B0, fn_804DCA54 and the state of its own slot (mode 2) to everybody
extern "C" int fn_804DDFFC(PPManager2_unk* mgr) {
    if (IsServer_unk() == 1) {
        fn_804E9360(mgr);
        PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
        for (unsigned int i = 0; i < mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = &mgr->mPlayers[i];
            player->mD4 = player->mFlagWord;
            player->mD8 = player->mB0;
        }
        fn_804DDA04(mgr);
        for (int i = 0; i < GetNumHosts_unk(lbl_8087F628); i++) {
            PPPlayer_unk* player = fn_804DD158(mgr, GetHostAid_unk(i));
            if (player != 0 && player->mFlags.mActive == 1 && player->mB0 == -1) {
                fn_804DD574(mgr, player->mAid, 0);
            }
        }
        fn_804DCA54(mgr);
        fn_804D824C(mgr, own, 2);
        fn_804AE3BC()->fn_8050DDDC(-1, 1);
    }
    return 1;
}

// inferred: on the server, starts the match (Manager::fn_80509B4C) after fn_804DE560; out of the modes 2 and 3 both
// teams need players
extern "C" int fn_804DE2F8(PPManager2_unk* mgr, int mode) {
    if (IsServer_unk() == 1) {
        if (mode != 2 && mode != 3) {
            int counts[3];
            memset(counts, 0, sizeof(counts));
            PPCountTeams_unk(counts);
            bool both;
            if (counts[1] == 0 || counts[2] == 0) {
                both = false;
            } else {
                both = true;
            }
            if (!both) {
                return 0;
            }
        }
        // inferred: the slot of the own player is searched and not used
        fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
        if (fn_804DE560(mgr, 0, 0) == 0) {
            return 0;
        }
        lbl_8087F628->fn_80509B4C(0);
    }
    return 1;
}

// inferred: slot `index` of the manager, 0 out of range (body of the weak copy fn_804D5F0C, inlined here)
static inline PPPlayer_unk* PPSlot_unk(PPManager2_unk* mgr, int index) {
    if (index >= 0 && index < (int)mgr->mPlayers.size()) {
        return &mgr->mPlayers[index];
    }
    return 0;
}
#include "game/EnemyTask.h"
namespace MP {
// inferred: character of the multiplayer session (__vt__Q22MP17Chara<9EnemyTask> 0x80791570, only the
// EnemyTask instance exists); members names invented
template <class T>
class Chara : public T {
public:
    // inferred: inlined in fn_804DE884 (the character of a computer player): registered in the battle, out of
    // the field until it appears
    Chara(atn::Task* parent, void* data) : T(parent, data), m14B0(0), m14B4(0) {
        this->mFlag2 = 1;
        if (lbl_8087F048 != 0) {
            lbl_8087F048->fn_801027A4(this);
        }
        ((PPChara_unk*)this)->m9F8 = 0;
        this->mPos = atn::Vector3(10000.0f, 10000.0f, -10000.0f);
    }
    // inferred: creates the character (none without a parent)
    static Chara* Create_unk(atn::Task* parent, void* data) {
        Chara* chara;
        if (parent != 0) {
            chara = new ((MemoryTag)3, lbl_8087E178, lbl_8087E174, 0) Chara(parent, data);
        } else {
            chara = 0;
        }
        return chara;
    }
    virtual ~Chara();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    int m14B0; // inferred: fn_80176ACC done
    int m14B4; // inferred: knock-down event sent
};
} // namespace MP

atn::Task* fn_80365708(atn::Task* parent, const GameTask::Option& option); // inferred: GameTask creator (game/GameTask)

// inferred: on the server: every other host has a slot and its bit in the mask mD53 of the own slot (count of the
// set bits in `count`, whether another host has a slot in `found`)
extern "C" bool fn_804DE560(PPManager2_unk* mgr, int* count, bool* found) {
    if (lbl_8087F628->GetMyAid_unk() != lbl_8087F628->GetServerAid_unk()) {
        return false;
    }
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    if (own == 0) {
        return false;
    }
    bool any = false;
    bool all = true;
    int acks = 0;
    int num = GetNumHosts_unk(lbl_8087F628);
    for (int i = 0; i < num; i++) {
        int aid = GetHostAid_unk(i);
        if (own->mAid != aid) {
            PPPlayer_unk* player = fn_804DD158(mgr, aid);
            if (player != 0) {
                any = true;
                int bit = own->mD53 & (1 << player->mAid);
                if (bit == 0) {
                    all = false;
                }
                if (bit != 0) {
                    acks++;
                }
            }
        }
    }
    if (count != 0) {
        *count = acks;
    }
    if (found != 0) {
        *found = any;
    }
    if (all == true && acks + 1 != GetNumHosts_unk(lbl_8087F628)) {
        all = false;
    }
    return all;
}

extern "C" void fn_800C3094(const char* group); // inferred: loads a sound group
// inferred: effects of the game (fn_804DE884)
extern "C" const char* const lbl_808875A4 = "/data/effect/ef_ms001e.efp";
extern "C" const char* const lbl_808875A8 = "/data/effect/ef_mw003f_01.eff";
// inferred: start of the game: the GameTask of the mode and stage (options lbl_807C8B10, the three digits of m598
// in the local flags 0xFC-0xFE), the results cleared, the computer players of m9984/m998C in free slots, the
// characters of the slots without one (MP::Chara<EnemyTask>, side by mode and team; the own team players appear
// at once), the popups, the effects and the sound groups of the stages in use. 0 when a slot has no team or no
// character
extern "C" int fn_804DE884(PPManager2_unk* mgr) {
    mgr->mGame = (GameTask*)fn_80365708(mgr, lbl_807C8B10[mgr->mMode][mgr->m560]);
    mgr->mGame->fn_80374964(0);
    mgr->mGame->fn_8037624C(0);
    mgr->mGame->fn_80370AE4(0xFC, mgr->m598 % 10);
    mgr->mGame->fn_80370AE4(0xFD, mgr->m598 / 10 % 10);
    mgr->mGame->fn_80370AE4(0xFE, mgr->m598 / 100 % 10);
    mgr->mGame->fn_80370AE4(0xFF, 0);
    SetConfig::sInstance->mShowTutorial = 0;
    mgr->m60C = 0;
    mgr->m2B80.m00 = 0;
    mgr->m2B80.m04 = 5;
    mgr->m2B80.m08.clear();
    mgr->m966C = -1;
    mgr->m9670 = -1;
    mgr->m9674 = 0;
    mgr->m9678 = 0;
    mgr->m967C = 0;
    mgr->m9680 = 0;
    mgr->m9684 = 0;
    mgr->m9688 = 0;
    mgr->m968C = 0;
    mgr->m9690 = 0;
    memset(mgr->m9694, -1, sizeof(mgr->m9694));
    MP::D2AMpMenuResult::fn_804C566C(mgr);
    MP::D2AMpMenuResult::sInstance->fn_800D246C(1);
    mgr->m997A[0] = lbl_8087F628->GetServerAid_unk();
    mgr->m604 = 1;
    mgr->m9764 = 1;
    mgr->m9768 = 0;
    for (int j = 0; j < 8; j++) {
        if (mgr->m9984[j] == 0 || fn_804DD158(mgr, mgr->m9984[j] & 0xF) == 0) {
            continue;
        }
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = PPSlot_unk(mgr, i);
            if (player->mFlags.mActive) {
                continue;
            }
            player->mFlags.mActive = 1;
            player->mAid = mgr->m9984[j];
            player->mB0 = (signed char)mgr->m998C[j];
            player->mFlags.m22 = fn_804DD1A8((PPManager2_unk*)lbl_8087F610, mgr->m9984[j]);
            player->mFlags.m14 = mgr->mMode == 2 ? 80 : 60;
            break;
        }
    }
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    PPPlayerArray_unk& players = mgr->mPlayers;
    for (int i = 0; i < (int)players.size(); i++) {
        PPPlayer_unk* player = PPSlot_unk(mgr, i);
        if (player->mFlags.mActive != 1) {
            continue;
        }
        player->mDC.fn_80501288(0);
        player->mE0 = 0;
        player->mE4 = 0;
        player->mFlags.m28 = 0;
        player->mFlags.m27 = 0;
        player->mFlags.m26 = 0;
        player->mFlags.m13 = 0;
        player->mFlags.m12 = 0;
        if (player == own || player->mRef.mChara != 0) {
            continue;
        }
        mgr->mOwnPlayer = player;
        player->mRef.mChara =
            MP::Chara<EnemyTask>::Create_unk((atn::Task*)lbl_8087F610, (void*)(player->mB0 + 700000));
        unsigned int team = player->mFlags.m22;
        if (team == 0) {
            return 0;
        }
        if (mgr->mMode == 1) {
            if (team == own->mFlags.m22) {
                player->mRef.mChara->fn_8016DF3C(3);
            } else {
                player->mRef.mChara->fn_8016DF3C(2);
            }
        } else if (mgr->mMode == 0) {
            player->mRef.mChara->fn_8016DF3C(2);
        } else {
            player->mRef.mChara->fn_8016DF3C(3);
        }
        MpCharaRef_unk& ref = player->mRef;
        if (ref.mChara == 0) {
            return 0;
        }
        if (IsTeamPlayer_unk(&mgr->mPlayers[i]) == 1) {
            ((BattleCharaTask*)player->mRef.mChara)->mFlagC25 = 1;
            if ((player->mAid & 0xF) == lbl_8087F628->GetMyAid_unk()) {
                player->mRef.mChara->fn_8016E970(2);
                player->mRef.mChara->m58C = 0;
                player->mRef.mChara->fn_801546F4(0, lbl_807C7030, 0.0f);
                player->mRef.mChara->fn_8015495C(0, 1, 1, 1);
                player->mRef.mChara->mFlag2 = 0;
            }
        }
    }
    mgr->mOwnPlayer = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    MP::Popup::fn_805012C4(mgr);
    MP::Popup::sInstance->fn_800D246C(1);
    mgr->m96B4.LoadRequest(lbl_808875A4);
    mgr->m96C0.LoadRequest(lbl_808875A8);
    int em001 = 0;
    int em102 = 0;
    int em002 = 0;
    int em003 = 0;
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = PPSlot_unk(mgr, i);
        if (player->mFlags.mActive != 1) {
            continue;
        }
        switch (player->mB0) {
        case 11:
        case 12:
        case 13:
            em001++;
            break;
        case 14:
        case 15:
        case 16:
            em102++;
            break;
        case 19:
        case 20:
            em002++;
            break;
        case 23:
            em003++;
            break;
        }
    }
    if (em001 > 0) {
        fn_800C3094("GROUP_MP_EM001");
    }
    if (em102 > 0) {
        fn_800C3094("GROUP_MP_EM102");
    }
    if (em002 > 0) {
        fn_800C3094("GROUP_MP_EM002");
    }
    if (em003 > 0) {
        fn_800C3094("GROUP_MP_EM003");
    }
    return 1;
}

extern "C" void fn_800C3124(const char* group); // inferred: unloads a sound group

// inferred: end of the game: deletes the game task and the characters of the players (the team players lose
// their slot), the popup and the effects of the game; with `reset` set also the player slots (fn_804DF310);
// then the sound groups of the enemies are unloaded
extern "C" void fn_804DF11C(PPManager2_unk* mgr, unsigned int reset) {
    if (mgr->mGame != 0) {
        fn_800D2338(mgr->mGame);
        mgr->mGame = 0;
        mgr->mCharas.clear();
        PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
        if (own != 0) {
            own->mRef.mChara = 0;
        }
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = fn_804D5F0C(mgr, i);
            if (player->mRef.mChara != 0) {
                fn_800D2338(player->mRef.mChara);
                player->mRef.mChara = 0;
                if (IsTeamPlayer_unk(&mgr->mPlayers[i]) == 1) {
                    player->mFlags.mActive = 0;
                    player->mFlags.m22 = 0;
                }
            }
        }
    }
    if (MP::Popup::sInstance != 0) {
        fn_800D2338(MP::Popup::sInstance);
    }
    mgr->m96B4.Destroy();
    mgr->m96C0.Destroy();
    if (reset == 1) {
        fn_804DF310(mgr);
    }
    fn_800C3124("GROUP_MP_EM001");
    fn_800C3124("GROUP_MP_EM102");
    fn_800C3124("GROUP_MP_EM002");
    fn_800C3124("GROUP_MP_EM003");
}

// NONMATCHING: 90.49% - the original reloads lbl_8087F610 in every iteration and keeps the zero of the index in
// two more registers
// inferred: resets the player slots (and the aid tables m9984/m998C)
extern "C" void fn_804DF310(PPManager2_unk* mgr) {
    memset(mgr->m9984, 0, 8);
    memset(mgr->m998C, 0xFF, 8);
    for (int i = 0; i < (int)((PPManager2_unk*)lbl_8087F610)->mPlayers.size(); i++) {
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
extern "C" int fn_804DF404(PPManager2_unk* mgr) {
    if ((mgr->mGame->mFlags & 2) == 2 && mgr->m96B4.IsLoading() == 0 && mgr->m96C0.IsLoading() == 0) {
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            CharaTask* chara = mgr->mPlayers[i].mRef.mChara;
            if (chara != 0 && (chara->mFlags & 2) != 2) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

#include "game/MslSort_unk.h"

// inferred: order of the players by score (higher first): predicate of the std::sort at the end of fn_804DF4B8
// (not written yet); defined after its user, so the sort calls it out of line (0x804E09AC). Name invented
struct PPScoreLess_unk {
    bool operator()(const PPPlayer_unk& a, const PPPlayer_unk& b) const { return a.mD58 > b.mD58; }
};
extern "C" int fn_804E4C5C(const PPManager2_unk* mgr);
// NONMATCHING: 92.7% - registers of the inlined copy constructor of PPPlayer_unk (the equipment loops) and the
// two score loops index base + offset in the original (here a moving pointer; an indexed inline accessor does not
// change it)
// inferred: ranking of the game: copies the slots in use to m2B8C, their score (team bonus of 1000 for the
// winning team in mode 1) and sorts them by score (not in mode 2)
// inferred: slot `i` of the ranking; name invented
static inline PPPlayer_unk& PPRanked_unk(PPManager2_unk* mgr, int i) {
    return mgr->m2B80.m08[i];
}
extern "C" void fn_804DF4B8(PPManager2_unk* mgr) {
    mgr->m2B80.m08.clear();
    for (unsigned int i = 0; i < mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = fn_804D5F0C(mgr, i);
        if (player->mFlags.mActive == 1) {
            mgr->m2B80.m08.push_back(*player);
        }
    }
    if (mgr->mMode != 2) {
        for (unsigned int i = 0; i < mgr->m2B80.m08.size(); i++) {
            PPRanked_unk(mgr, i).mD58 = PPRanked_unk(mgr, i).mDCScore;
        }
        if (mgr->mMode == 1) {
            int team = fn_804E4C5C(mgr);
            for (unsigned int i = 0; i < mgr->m2B80.m08.size(); i++) {
                if (PPRanked_unk(mgr, i).mFlags.m22 == team) {
                    PPRanked_unk(mgr, i).mD58 += 1000;
                }
            }
        }
        MslSort_unk(mgr->m2B80.m08.begin(), mgr->m2B80.m08.end(), PPScoreLess_unk());
    }
}
// inferred: emitted here in the original, after fn_804DF4B8 (its only caller): 0x804E0694
// (comparison by value), 0x804E09CC (by reference), the swap 0x804E0CE4, the median 0x804E2840 and the
// selection sort 0x804E291C
// NONMATCHING: the swap 0x804E0CE4 (97.73%) and the selection sort 0x804E291C (99.35%, the swap inlined): the
// original assigns the second part of each equipment (PPEquipBody_unk) through a copy of its source pointer
// (`addi r5, src, 0x134; mr r6, r5`), the mAid byte of the copy is loaded before the loop addresses, and
// registers of the loop invariant addresses
template void MslSort_unk<PPPlayer_unk*, PPScoreLess_unk>(PPPlayer_unk*, PPPlayer_unk*, PPScoreLess_unk);

extern "C" int fn_804E4C34(PPManager2_unk* mgr);
extern "C" int fn_804E4C5C(const PPManager2_unk* mgr);
extern "C" int fn_804E4A60(PPManager2_unk* mgr, PPPlayer_unk* player);
extern "C" int fn_805076F8(void* table, int mode, int value, int arg);
extern "C" int fn_80507654(MpLevelTable_unk* table, int exp, int next); // inferred: nonzero on a level up
extern "C" int fn_8050655C(void* table, int mode, int value, int rank); // inferred: item won in the game
extern "C" bool fn_80206B9C(int code);                                 // inferred: whether an item code is a weapon
extern "C" int fn_8020EF04(int code);                                 // inferred: whether an item code is equipment
// inferred: experience in the online play: a word of the data saved by the game (MpFriendList_unk::m7F0 + 8,
// not split in include/atn/net/Manager.h: a split changes the implicit copy of fn_804DBEC8); name invented
static inline int& PPExp_unk(MpFriendList_unk* list) {
    return *(int*)&list->m7F0[8];
}
// inferred: payload of the message 0x1012 (aid and item won); names invented
struct PPMsgResult_unk {
    int mAid;
    int mItem;
};
// inferred: copy of a network player kept by the manager; name invented
static inline atn::net::NetPlayerInfo_unk& PPNetPlayer_unk(PPManager2_unk* mgr, int i) {
    return mgr->m358[i];
}
// inferred: slot of the own player; name invented
static inline PPPlayer_unk* PPOwnPlayer_unk(PPManager2_unk* mgr) {
    return fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
}
// inferred: end of the game: ranking, result state, experience and item won (saved), copy of the network
// players and message 0x1012 to everybody
extern "C" void fn_804E448C(PPManager2_unk* mgr) {
    MpFriendList_unk* list;
    fn_804DF4B8(mgr);
    PPPlayer_unk* own = PPOwnPlayer_unk(mgr);
    int rank = fn_804E4B44(mgr, own);
    if (rank == fn_804E4C34(mgr) && mgr->mMode != 2) {
        rank = 5;
    }
    int result;
    if (mgr->mMode == 0) {
        if (rank == 0) {
            result = 0;
        } else if (fn_804E4C34(mgr) != 0) {
            result = 1;
        } else {
            result = 2;
        }
    } else if (mgr->mMode == 1) {
        if (fn_804E4C5C(mgr) != 3) {
            if (own->mFlags.m22 == fn_804E4C5C(mgr)) {
                result = 0;
            } else {
                result = 1;
            }
        } else {
            result = 2;
        }
    } else {
        result = rank == 0 ? 3 : 4;
    }
    list = PPFriendList_unk();
    mgr->m2B80.m04 = result;
    mgr->m966C = rank;
    mgr->m9670 = fn_804E4A60(mgr, own);
    mgr->m967C = fn_805076F8(&mgr->m610, mgr->mMode, mgr->m55C, mgr->m9670);
    int exp0 = PPExp_unk(list);
    mgr->m9680 = exp0 + mgr->m967C;
    mgr->m9688 = fn_80507654(&mgr->m610.mTable, PPExp_unk(list), mgr->m9680);
    if (mgr->m9688 != 0) {
        MpLevel_unk* level = fn_805075C4(&mgr->m610.mTable, mgr->m9680);
        mgr->m968C = ((int*)level)[3];
        mgr->m9690 = ((int*)level)[4];
    }
    mgr->m9684 = (int)fn_805075C4(&mgr->m610.mTable, mgr->m9680);
    int max = fn_80507764(&mgr->m610.mTable, mgr->m9680);
    if (mgr->m9680 > max) {
        int over = mgr->m9680 - max;
        mgr->m9680 = max;
        mgr->m967C = mgr->m967C - over;
    }
    int exp1 = PPExp_unk(list);
    int exp = exp1 + mgr->m967C;
    PPExp_unk(list) = exp;
    list->mA00 = exp;
    fn_8050EAE8(&lbl_8087F628->mFriendList);
    MP::SaveLoad* saveLoad = ((PPManager2_unk*)lbl_8087F610)->mSaveLoad;
    if (saveLoad != 0) {
        saveLoad->fn_80502870(1, list, 0x9F0);
    }
    mgr->m9674 = fn_8050655C(&mgr->m69C, mgr->mMode, mgr->m560, rank);
    mgr->m9678 = 1;
    if ((fn_80206B9C(mgr->m9674) || fn_8020EF04(mgr->m9674)) && mgr->mSaveLoad->m48 != 0 &&
        ((GameSaveInventory_unk*)&mgr->mSaveLoad->m48->mSub)->fn_803BD49C(mgr->m9674) >= 10) {
        switch (mgr->m9674) {
        case 0x1890D:
            mgr->m9674 = 0x2B3;
            break;
        case 0x1890C:
            mgr->m9674 = 0x28A;
            break;
        case 0x1890B:
            mgr->m9674 = 0x28B;
            break;
        case 0x1890A:
            mgr->m9674 = 0x296;
            break;
        case 0x18909:
            mgr->m9674 = 0x295;
            break;
        default:
            mgr->m9674 = 0x295;
            break;
        }
    }
    mgr->m354 = 8;
    for (unsigned int i = 0; i < mgr->m354; i++) {
        PPNetPlayer_unk(mgr, i) = lbl_8087F628->mPlayers[i];
    }
    NetMsgI_unk<PPMsgResult_unk> msg(0x1012);
    PPMsgResult_unk* data = &msg.mData;
    data->mAid = own->mAid;
    data->mItem = mgr->m9674;
    fn_804AE3BC()->fn_8050E094(-1, 0x1012, data, 1);
    fn_804AE3BC()->fn_8050DDDC(-1, 1);
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
extern "C" __declspec(noinline) int fn_804E4A60(PPManager2_unk* mgr, PPPlayer_unk*) {
    int result;
    if (mgr->mMode == 0) {
        if (mgr->m2B80.m04 == 0) {
            result = 0;
        } else if (mgr->m2B80.m04 == 1 && mgr->m966C == 5 && fn_804E4C34(mgr) != 0) {
            result = 2;
        } else {
            result = 1;
        }
    } else if (mgr->mMode == 1) {
        if (mgr->m2B80.m04 == 0) {
            result = 0;
        } else if (mgr->m2B80.m04 == 1) {
            result = 2;
        } else {
            result = 1;
        }
    } else {
        result = mgr->m2B80.m04 == 3 ? 0 : 2;
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
            for (int i = 0; i < (int)mgr->m2B80.m08.size(); i++) {
                if (i > 0 && mgr->m2B80.m08[i].mD58 != mgr->m2B80.m08[i - 1].mD58) {
                    rank += run;
                    run = 0;
                }
                if (mgr->m2B80.m08[i].mRef.mChara == target->mRef.mChara) {
                    result = rank;
                }
                run++;
            }
        }
    } else {
        result = mgr->m2B80.m00 != 3;
    }
    return result;
}

// inferred: fn_804E4B44 on the last slot of m2B8C (-1 when there is none)
extern "C" int fn_804E4C34(PPManager2_unk* mgr) {
    if (mgr->m2B80.m08.size() != 0) {
        return fn_804E4B44(mgr, &mgr->m2B80.m08[mgr->m2B80.m08.size() - 1]);
    }
    return -1;
}

// inferred: score of the team `team` (sum over the ranking m2B8C)
static inline int PPTeamScore_unk(const PPManager2_unk* mgr, int team) {
    int score[3];
    memset(score, 0, sizeof(score));
    for (unsigned int i = 0; i < mgr->m2B80.m08.size(); i++) {
        const PPPlayer_unk& player = mgr->m2B80.m08.mData[i];
        score[player.mFlags.m22] += player.mDCScore;
    }
    return score[team];
}

// inferred: winning team of the game (3 = none or draw)
extern "C" int fn_804E4C5C(const PPManager2_unk* mgr) {
    switch (mgr->mMode) {
    case 0:
        break;
    case 1: {
        int scores[3];
        for (int i = 0; i < 3; i++) {
            scores[i] = PPTeamScore_unk(mgr, i);
        }
        int best = 3;
        int max = -1;
        for (int i = 1; i < 3; i++) {
            if (scores[i] > max) {
                best = i;
                max = scores[i];
            }
        }
        if (best == 3 || scores[1] == scores[2]) {
            break;
        }
        return best;
    }
    default:
        return (mgr->m2B80.m00 == 3) + 1;
    }
    return 3;
}

extern "C" int fn_804E4A60(PPManager2_unk* mgr, PPPlayer_unk* player);
extern "C" int fn_805076F8(void* table, int mode, int value, int arg);

// inferred: value of a player (m967C for the players marked m30)
extern "C" int fn_804E4D70(PPManager2_unk* mgr, PPPlayer_unk* player) {
    if (player != 0 && player->mFlags.m30) {
        return mgr->m967C;
    }
    return fn_805076F8(&mgr->m610, mgr->mMode, mgr->m55C, fn_804E4A60(mgr, player));
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
// inferred: copy of an entry of the character list (weak copy, as fn_804D8078)
extern "C" __declspec(noinline) inline void fn_804E7434(PPCharaEntry_unk* dst, const PPCharaEntry_unk* src) {
    *dst = *src;
}
extern "C" int fn_804E650C(CharaTask* chara);
extern "C" int fn_804CF424(ModelSlotsN_unk* model);
extern "C" void fn_804CF374(PackedMotion_unk* out, atn::Model* model, int index);

extern "C" void fn_804E7AC0(PPManager2_unk* mgr);
extern "C" void fn_804E7628(PPManager2_unk* mgr);
extern "C" void fn_804E6DD0(PPManager2_unk* mgr);
extern "C" int fn_804E82C4(PPManager2_unk* mgr, int message);
extern "C" void fn_804E4EB0(PPManager2_unk* mgr);
extern const char* lbl_80791018[36];
#include "game/DamageCalc_unk.h"
extern "C" int fn_80051C78(const atn::ColliCapsuleShape_unk* a, const atn::ColliCapsuleShape_unk* b); // inferred: the capsules touch
static inline int IsSameTeam_unk(PPPlayer_unk* player, PPPlayer_unk* leader);
static inline PPPlayer_unk* GetMyPlayer_unk();
// inferred: payload of message 0x103B (the player is down or back: aid, state, hit points); names invented
struct PPMsgDown_unk {
    unsigned char mAid;
    unsigned char mDown;
    unsigned char mValue;
};
// NONMATCHING: 98.80% - the original loads m564 twice in the inlined fn_804D64C8 and computes the slot of the team
// test before the search of the own player
// inferred: knocked down players (state 0x20) of the own machine (the own slot or the team players of the own
// player): at the start of the countdown mD38[4] (the respawn time m5C4, a third out of game mode 2) the hit
// points are restored (out of mode 2 and without m5A8), message 0x103B tells the others, the down count mE4 grows
// and the score loses 5 points (10 in the last minute); the own player of game mode 2 without hit points left
// starts a time limit of 120 seconds (message 0x1002, the music and a tutorial); in game mode 2 another player
// touching the downed one ends the countdown at once. At the end of the countdown the player revives (invincible
// for 150 frames, effect 0x4FC1, message 0x103A, the revival hit 0x2711 and a warp point, message 0x103B) or,
// with m5A8 and no hit points left, a human player stays down for good (the game is lost: flags, items and a
// tutorial)
extern "C" void fn_804E4EB0(PPManager2_unk* mgr) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = PPSlot_unk(mgr, i);
        if (!player->mFlags.mActive) {
            continue;
        }
        if (player != fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) &&
            IsSameTeam_unk(&mgr->mPlayers[i], GetMyPlayer_unk()) != 1) {
            continue;
        }
        CharaTask* chara = player->mRef.mChara;
        if (!chara->mStatus.IsState_unk(0x20) || player->mD38[4] <= 0) {
            continue;
        }
        int near = 0;
        PPManager2_unk* global = fn_800F7F90();
        if (player->mD38[4] == (global->mMode == 2 ? global->m5C4 : global->m5C4 / 3)) {
            if (mgr->mMode != 2 && chara != 0 && !(unsigned char)mgr->m5A8) {
                ((PPChara_unk*)chara)->m9F8 = chara->mStatus.mParamB.m98;
            }
            NetMsgI_unk<PPMsgDown_unk> msg(0x103B);
            msg.mData.mAid = player->mAid;
            msg.mData.mDown = 1;
            msg.mData.mValue = player->mRef.mChara == 0 ? 0 : ((PPChara_unk*)player->mRef.mChara)->m9F8;
            fn_804AE3BC()->fn_8050E094(-1, 0x103B, &msg.mData, 1);
            player->mE4++;
            int points = -5;
            PPManager2_unk* mgr2 = fn_800F7F90();
            bool last = !(GetRemain_unk(mgr2) < 0);
            if (last) {
                last = GetRemain_unk(mgr2) < 60;
            }
            if (last) {
                points = -10;
            }
            fn_804DB1C4(mgr, player->mRef.mChara, points);
            if (mgr->mMode == 2 && mgr->m564 < 0 && player == fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) &&
                player->mRef.mChara != 0 && ((PPChara_unk*)player->mRef.mChara)->m9F8 <= 0) {
                int limit = fn_804DB3B4(mgr) + 120;
                NetMsgI_unk<int> limitMsg(0x1002);
                limitMsg.mData = limit;
                fn_804AE3BC()->fn_8050E094(-1, 0x1002, &limitMsg.mData, 1);
                mgr->m564 = limit;
                if (GameBgm::GetInstance_unk() != 0) {
                    GameBgm::GetInstance_unk()->fn_8037D4C0(0x53D, 15, 1, 0, 0.7f);
                }
                atn::net::Manager* net = lbl_8087F628;
                unsigned int* shown = (unsigned int*)&net->mFriendList.m7F0[0x0C];
                if ((*shown & 1) != 1 || (*shown & 2) != 2) {
                    if ((*shown & 1) != 1) {
                        *shown |= 1;
                    } else if ((*shown & 2) != 2) {
                        *shown |= 2;
                    }
                    if (lbl_8087F430 != 0) {
                        SetConfig::sInstance->mShowTutorial = 1;
                        lbl_8087F430->fn_803750E4(0x1B0);
                        SetConfig::sInstance->mShowTutorial = 0;
                    }
                }
            }
        } else if (mgr->mMode == 2) {
            for (int j = 0; j < (int)mgr->mPlayers.size(); j++) {
                PPPlayer_unk* other = PPSlot_unk(mgr, j);
                if (!other->mFlags.mActive || other == player || other->mRef.mChara == 0) {
                    continue;
                }
                if (player->mRef.mChara->fn_8016F3D0(other->mRef.mChara, 1) &&
                    fn_80051C78((const atn::ColliCapsuleShape_unk*)&player->mRef.mChara->mColli.m3C,
                                (const atn::ColliCapsuleShape_unk*)&other->mRef.mChara->mColli.m3C)) {
                    player->mD38[4] = 0;
                    near = 1;
                    break;
                }
            }
        }
        if (--player->mD38[4] <= 0) {
            if (!(unsigned char)mgr->m5A8 || ((PPChara_unk*)player->mRef.mChara)->m9F8 > 0) {
                player->mD50 = 150;
                if (player != 0 && player->mRef.mChara != 0) {
                    player->mD4C |= 0xC00;
                    CharaTask* self = player->mRef.mChara;
                    if (self != 0) {
                        player->mRef.mChara->fn_80178668(fn_80219E6C(0x4FC1), 0, 9999, self, 0);
                    }
                }
                if (player != 0) {
                    NetMsgI_unk<PPId_unk> upMsg(0x103A);
                    upMsg.mData.m00 = player->mAid;
                    upMsg.mData.m01 = 1;
                    fn_804AE3BC()->fn_8050E094(-1, 0x103A, &upMsg.mData, 1);
                    fn_804AE3BC()->fn_8050DDDC(-1, 1);
                }
                HitInfo_unk* hit = fn_80219E6C(0x2711);
                DamageResult_unk result;
                fn_8003EA3C(&result, hit, player->mRef.mChara, player->mRef.mChara, 0, &lbl_807C6B90, 0, 0);
                player->mRef.mChara->fn_8015EFAC(10);
                fn_804EAA84(mgr, player->mRef.mChara, 0, near);
                NetMsgI_unk<PPMsgDown_unk> backMsg(0x103B);
                backMsg.mData.mAid = player->mAid;
                backMsg.mData.mDown = 0;
                fn_804AE3BC()->fn_8050E094(-1, 0x103B, &backMsg.mData, 1);
            } else if (player->mRef.mChara->mType == 0) {
                HitInfo_unk* hit = fn_80219E6C(0x2711);
                DamageResult_unk result;
                fn_8014F8F4()->mEnabled = 0;
                fn_8003EA3C(&result, hit, player->mRef.mChara, player->mRef.mChara, 0, &lbl_807C6B90, 0, 0);
                fn_8014F8F4()->mEnabled = 1;
                player->mRef.mChara->fn_8015EFAC(10);
                player->mRef.mChara->m54C |= 4;
                player->mRef.mChara->fn_8014EEC4(0);
                player->mRef.mChara->mC58.mC0 = 0;
                player->mRef.mChara->m54C |= 0x2000;
                mgr->mGame->fn_803748E0(1);
                mgr->mGame->fn_80370320(0x1F, 1, 0);
                mgr->mGame->fn_80370320(0x25, 1, 0);
                mgr->mGame->fn_80370320(0x64, 7, 0);
                player->mRef.mChara->fn_8017C7F8(1);
                Inventory::instance_->AddItem(0x72, 99, 0, 0, 0, 1, 1);
                Inventory::instance_->AddItem(0x70, 99, 0, 0, 0, 1, 1);
                if (fn_800F7F90()->m50C == 0) {
                    atn::net::Manager* net = lbl_8087F628;
                    unsigned int* shown = (unsigned int*)&net->mFriendList.m7F0[0x0C];
                    if ((*shown & 0x10) != 0x10 || (*shown & 0x20) != 0x20) {
                        if ((*shown & 0x10) != 0x10) {
                            *shown |= 0x10;
                        } else if ((*shown & 0x20) != 0x20) {
                            *shown |= 0x20;
                        }
                        if (lbl_8087F430 != 0) {
                            SetConfig::sInstance->mShowTutorial = 1;
                            lbl_8087F430->fn_803750E4(0x1BF);
                            SetConfig::sInstance->mShowTutorial = 0;
                        }
                    }
                }
            }
        }
        if (!(unsigned char)mgr->m5A8 || ((PPChara_unk*)player->mRef.mChara)->m9F8 > 0) {
            // inferred: the slot of the own player is searched and not used
            fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
        }
    }
}

// NONMATCHING: 97.31% - registers (the original shares the zero of the counter with the static of the message) and
// the reloads of m564 and of the slot count in the inlined slot loops
// inferred: update of the game: the timers mD50 of the slots (at 0 the effect 0x4FC1 ends and message 0x103A
// tells it), the end of the game (score m568, time limit m564, one side or one player left, game mode 2: no
// player alive or the bosses defeated: m2B80 = 1 or 3, m50C), the cockpit (time and team scores), fn_804E4EB0,
// fn_804E6518, a random voice, fn_804DF4B8 and the tutorial message of game mode 2 (m9764)
extern "C" void fn_804E5BC4(PPManager2_unk* mgr) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = PPSlot_unk(mgr, i);
        if (player == 0 || player->mD50 <= 0) {
            continue;
        }
        if (--player->mD50 != 0) {
            continue;
        }
        if (player != 0 && player->mRef.mChara != 0) {
            player->mD4C &= ~0xC00;
            CharaTask* chara = player->mRef.mChara;
            if (chara != 0) {
                player->mRef.mChara->fn_80178864(fn_80219E6C(0x4FC1), chara);
            }
        }
        if (player->mFlags.mActive == 1 && player != 0) {
            NetMsgI_unk<PPId_unk> msg(0x103A);
            PPId_unk* data = &msg.mData;
            data->m00 = player->mAid;
            data->m01 = 0;
            fn_804AE3BC()->fn_8050E094(-1, 0x103A, data, 1);
            fn_804AE3BC()->fn_8050DDDC(-1, 1);
        }
        player->mD50 = -1;
    }
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    if (own == 0) {
        return;
    }
    if (mgr->mStateVar.mValue == 30 && mgr->m50C == 0 && mgr->m568 != -1) {
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = PPSlot_unk(mgr, i);
            if (player->mFlags.mActive == 1 && player->mDCScore >= mgr->m568) {
                mgr->m2B80.m00 = 1;
                mgr->m50C = 1;
            }
        }
    }
    int remain = -1;
    if (mgr->mStateVar.mValue == 30 && mgr->m50C == 0) {
        if (mgr->m564 != -1) {
            remain = GetRemain_unk(mgr);
            if (remain <= 0) {
                remain = 0;
                if (!own->mFlags.m26) {
                    mgr->m2B80.m00 = 1;
                    mgr->m50C = 1;
                    mgr->m534 = 1;
                }
            }
        }
        if (mgr->mMode == 1) {
            int counts[3];
            memset(counts, 0, sizeof(counts));
            PPManager2_unk* mgr2 = (PPManager2_unk*)lbl_8087F610;
            for (int i = 0; i < (int)((PPManager2_unk*)lbl_8087F610)->mPlayers.size(); i++) {
                PPPlayer_unk* player = PPSlot_unk(mgr2, i);
                if (player->mFlags.mActive == 1 && player->mRef.mChara != 0 &&
                    ((PPChara_unk*)player->mRef.mChara)->m9F8 > 0) {
                    counts[player->mFlags.m22]++;
                }
            }
            if (counts[2] == 0 || counts[1] == 0) {
                mgr->m2B80.m00 = 1;
                mgr->m50C = 1;
            }
        } else if (mgr->mMode == 0) {
            int n = 0;
            PPManager2_unk* mgr2 = (PPManager2_unk*)lbl_8087F610;
            for (int i = 0; i < (int)((PPManager2_unk*)lbl_8087F610)->mPlayers.size(); i++) {
                PPPlayer_unk* player = PPSlot_unk(mgr2, i);
                if (player->mFlags.mActive == 1 && player->mRef.mChara != 0 &&
                    ((PPChara_unk*)player->mRef.mChara)->m9F8 > 0) {
                    n++;
                }
            }
            if (n <= 1) {
                mgr->m2B80.m00 = 1;
                mgr->m50C = 1;
            }
        } else if (mgr->mMode == 2) {
            int dead = 1;
            int bosses = 1;
            for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
                PPPlayer_unk* player = &mgr->mPlayers[i];
                if (!player->mFlags.mActive || IsTeamPlayer_unk(player)) {
                    continue;
                }
                CharaTask* chara = player->mRef.mChara;
                if (chara == 0 || (((PPChara_unk*)chara)->m12A8 >> 1) & 1) {
                    continue;
                }
                if ((((PPChara_unk*)chara)->m7E0 & 0x20) != 0x20 || chara->fn_8015EB2C() == 0 ||
                    ((PPChara_unk*)chara)->m9F8 > 0) {
                    dead = 0;
                    break;
                }
            }
            if ((lbl_8087F430 != 0 ? lbl_8087F430->fn_80370A78(0xFB) : 0) == 2) {
                for (int i = 0; i < (int)mgr->mCharas.size(); i++) {
                    CharaTask* chara = mgr->mCharas[i].mRef.mChara;
                    if (chara == 0 || (chara->mFlags & 4) == 4) {
                        continue;
                    }
                    unsigned int flags = ((PPChara_unk*)chara)->m12A4;
                    if (((flags >> 22) & 1) != 1) {
                        continue;
                    }
                    if ((((PPChara_unk*)chara)->m7E0 & 0x20) != 0x20 ||
                        (!((flags >> 15) & 1) && (!((flags >> 16) & 1) || ((PPChara_unk*)chara)->m9F8 > 0))) {
                        bosses = 0;
                        break;
                    }
                }
            }
            if (dead) {
                mgr->m2B80.m00 = 1;
                mgr->m50C = 1;
            } else if (bosses) {
                mgr->m2B80.m00 = 3;
                mgr->m50C = 1;
            }
        }
    }
    if (mgr->m518 != 0) {
        MP::D2AMpMenuCockpit::sInstance->fn_804B6204(remain);
        if (mgr->mMode != 2 && mgr->m50C == 0 && mgr->mStateVar.mValue != 31) {
            int score2 = 0;
            int score1 = 0;
            for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
                PPPlayer_unk* player = PPSlot_unk(mgr, i);
                if (player->mFlags.mActive == 1) {
                    if (player->mFlags.m22 == 2) {
                        score2 += player->mDCScore;
                    } else if (player->mFlags.m22 == 1) {
                        score1 += player->mDCScore;
                    }
                }
            }
            MP::D2AMpMenuCockpit::sInstance->fn_804B671C(score2, 0);
            MP::D2AMpMenuCockpit::sInstance->fn_804B671C(score1, 1);
        }
    }
    fn_804E4EB0(mgr);
    fn_804E6518(mgr);
    if (mgr->mMode != 2 && mgr->m50C == 0 && lbl_8087F498 != 0 && lbl_8087F498->fn_803EBA54() != 0 &&
        !fn_800CB58C(&mgr->m2B74) && rand() % 90 == 0) {
        GameVoice* voice = lbl_8087F498;
        mgr->m2B74 = voice->fn_803E9608(lbl_80791018[(unsigned int)rand() % 36], 0, 1, 1.0f);
    }
    if (mgr->mStateVar.mValue == 30 && mgr->m50C == 0) {
        fn_804DF4B8(mgr);
    }
    if (mgr->mMode == 2 && mgr->m50C == 0 && mgr->m9764 != 0) {
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = &mgr->mPlayers[i];
            if (!player->mFlags.mActive || player->mFlags.m30 == 1 || IsTeamPlayer_unk(player)) {
                continue;
            }
            if ((((PPChara_unk*)player->mRef.mChara)->m7E0 & 0x20) != 0x20) {
                continue;
            }
            atn::net::Manager* net = lbl_8087F628;
            unsigned int* shown = (unsigned int*)&net->mFriendList.m7F0[0x0C];
            if ((*shown & 4) != 4 || (*shown & 8) != 8) {
                if ((*shown & 4) != 4) {
                    *shown |= 4;
                } else if ((*shown & 8) != 8) {
                    *shown |= 8;
                }
                if (lbl_8087F430 != 0) {
                    SetConfig::sInstance->mShowTutorial = 1;
                    lbl_8087F430->fn_803750E4(0x1BE);
                    SetConfig::sInstance->mShowTutorial = 0;
                }
            }
            mgr->m9764 = 0;
            break;
        }
    }
}

extern "C" int fn_804E650C(CharaTask* chara) {
    return chara->mFlag15;
}


// inferred: per frame of the own player: flag m12 (local flag 0xFB is 3), the messages of the frame (table of 30
// frames: 2 character states, 4 and 8 other updates) and the quick messages of the pad in the message list
extern "C" void fn_804E6518(PPManager2_unk* mgr) {
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    int flag = lbl_8087F430 != 0 ? lbl_8087F430->fn_80370A78(0xFB) : 0;
    own->mFlags.m12 = flag == 3;
    int frame = lbl_80790F78[(unsigned int)mgr->mFrame % 30];
    if ((frame & 2) == 2) {
        fn_804E7AC0(mgr);
    }
    if ((frame & 4) == 4) {
        fn_804E7628(mgr);
    }
    if ((frame & 8) == 8) {
        fn_804E6DD0(mgr);
    }
    if (MP::D2AMpMenuMessageList::sInstance != 0 && MP::D2AMpMenuMessageList::sInstance->m94 == 4) {
        CharaTask* chara = own->mRef.mChara;
        if (chara != 0 && !chara->mFlag6 && (!chara->mFlag31 || !chara->HasC48_unk())) {
            int message = -1;
            if (fn_800A555C(GetPad_unk(), 0, 0) != 0) {
                if (SetConfig::sInstance->mInTacviewType == 0) {
                    message = 0;
                }
            } else if (fn_800A555C(GetPad_unk(), 0, 1) != 0) {
                if (SetConfig::sInstance->mInTacviewType == 1) {
                    message = 0;
                }
            } else if (fn_800A555C(GetPad_unk(), 0, 2) != 0) {
                message = 1;
            } else if (fn_800A555C(GetPad_unk(), 0, 3) != 0) {
                message = 2;
            }
            if ((unsigned int)message <= 4) {
                // inferred: quick message ids saved with the friend list
                fn_804E82C4(mgr, ((signed char*)&lbl_8087F628->mFriendList.m7F0[0xB8])[message]);
            }
        }
    }
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
// inferred: weighted table of a room (fn_8050652C): ids (0 ends the list) and their weights; names invented
struct PPWeightTable_unk {
    int m00;
    int mIds[32];
    int mWeights[32];
};
extern "C" PPWeightTable_unk* fn_8050652C(void* tables, int index);
extern "C" int fn_804EA5DC(PPManager2_unk* mgr, int kind);
// inferred: last value of a list of indices (0 when empty), removed; name invented
static inline int PPPopIndex_unk(atn::FixedArray_unk<int, 10>& list) {
    if (list.size() != 0) {
        int value = *(list.end() - 1);
        list.mSize--;
        return value;
    }
    return 0;
}
// inferred: on the server: the settings of the game (stage at random when out of range, two shuffled lists of
// warp points by team, a weighted choice of three values and a random seed) sent with message 0x1004
extern "C" void fn_804E679C(PPManager2_unk* mgr) {
    mgr->m5AC = 1;
    if (mgr->m55C >= fn_804EA5DC(mgr, mgr->mMode)) {
        mgr->m560 = rand() % fn_804EA5DC(mgr, mgr->mMode);
    } else {
        mgr->m560 = mgr->m55C;
    }
    atn::FixedArray_unk<int, 10> pool;
    atn::FixedArray_unk<int, 10> team2;
    atn::FixedArray_unk<int, 10> team1;
    unsigned int num = 5;
    if (mgr->mMode != 1) {
        num = 6;
    }
    for (unsigned int i = 0; i < num; i++) {
        pool.push_back(i);
    }
    for (unsigned int i = 0; i < num; i++) {
        unsigned int r = rand() % pool.size();
        team2.push_back(pool[r]);
        pool.erase(pool.begin() + r);
    }
    for (unsigned int i = 0; i < num; i++) {
        pool.push_back(i);
    }
    for (unsigned int i = 0; i < num; i++) {
        unsigned int r = rand() % pool.size();
        team1.push_back(pool[r]);
        pool.erase(pool.begin() + r);
    }
    for (int i = 0; i < 3; i++) {
        mgr->m58C.m[i] = 0;
        PPWeightTable_unk* table = fn_8050652C(&mgr->m69C, i);
        if (table != 0) {
            int total = 0;
            for (int j = 0; j < 32; j++) {
                if (table->mIds[j] == 0) {
                    break;
                }
                total += table->mWeights[j];
            }
            if (total > 0) {
                int r = rand() % total;
                for (int j = 0; j < 32; j++) {
                    if (table->mIds[j] == 0) {
                        break;
                    }
                    if (r < table->mWeights[j]) {
                        mgr->m58C.m[i] = j;
                        break;
                    }
                    r -= table->mWeights[j];
                }
            }
        }
    }
    mgr->m598 = rand();
    int n = 0;
    for (unsigned int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1) {
            if (player->mFlags.m22 == 2) {
                mgr->m56C.m[i] = PPPopIndex_unk(team2);
            } else {
                mgr->m56C.m[i] = PPPopIndex_unk(team1);
            }
        } else if (mgr->m9984[n] != 0) {
            if (fn_804DD1A8(mgr, mgr->m9984[n]) == 0 || fn_804DD1A8(mgr, mgr->m9984[n]) == 2) {
                mgr->m56C.m[i] = PPPopIndex_unk(team2);
            } else {
                mgr->m56C.m[i] = PPPopIndex_unk(team1);
            }
            n++;
        }
    }
    NetMsgI_unk<PPGameSettings_unk> msg(0x1004);
    PPGameSettings_unk* data = &msg.mData;
    data->m00 = mgr->m560;
    data->m04 = mgr->m564;
    data->m08 = mgr->m568;
    memcpy(data->m0C, mgr->m56C.m, sizeof(data->m0C));
    memcpy(data->m2C, mgr->m58C.m, sizeof(data->m2C));
    data->m38 = mgr->m598;
    fn_804AE3BC()->fn_8050E094(-1, 0x1004, data, 1);
}

// inferred: data of the state of a player applied by fn_804EE9F0; names invented
struct PPPlayerInfo_unk {
    int m00;
    short mChara;
    unsigned char m06;
    unsigned char m07;
    unsigned char m08_0 : 1;
    unsigned char m13 : 1;
    unsigned char m12 : 1;
    unsigned char mIsEntry : 1; // inferred: mAid is an index of the character list (fn_804F2258)
    unsigned char m08_4 : 4;
    unsigned char mAid;
};

// NONMATCHING: 99.31% - registers (mgr, own and the loop variables; aid/offset of the inlined search fn_804DD158 as
// fn_804E448C)
// inferred: sends the state of the own player (message 0x1015), of the team players of this machine and of the
// characters of the list (message 0x101C)
extern "C" void fn_804E6DD0(PPManager2_unk* mgr) {
    PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    CharaTask* head = ((PlayerManager*)fn_8000D9E8())->GetHead_unk();
    {
        NetMsgI_unk<PPPlayerInfo_unk, 10> msg(0x1015);
        PPPlayerInfo_unk* data = &msg.mData;
        data->mAid = lbl_8087F628->GetMyAid_unk();
        data->m00 = ((PPChara_unk*)head)->m7D8;
        data->mChara = own->mDCScore;
        data->m06 = own->mE0;
        data->m07 = own->mE4;
        data->m08_0 = !head->mStatus.IsState_unk(0x20);
        data->m13 = own->mFlags.m13;
        data->m12 = own->mFlags.m12;
        fn_804AE3BC()->fn_8050E094(-1, 0x1015, data, 0);
    }
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        if (!IsTeamPlayer_unk(&mgr->mPlayers[i])) {
            continue;
        }
        PPPlayer_unk* player = fn_804D5F0C(mgr, i);
        if (!player->mFlags.mActive || (player->mAid & 0xF) != lbl_8087F628->GetMyAid_unk()) {
            continue;
        }
        CharaTask* chara = player->mRef.mChara;
        if (chara == 0 || (chara->mFlags & 4) == 4) {
            continue;
        }
        NetMsgI_unk<PPPlayerInfo_unk, 10> msg(0x101C);
        PPPlayerInfo_unk* data = &msg.mData;
        data->mAid = player->mAid;
        data->m00 = ((PPChara_unk*)player->mRef.mChara)->m7D8;
        data->mChara = player->mDCScore;
        data->m06 = player->mE0;
        data->m07 = player->mE4;
        data->m08_0 = !player->mRef.mChara->mStatus.IsState_unk(0x20);
        data->mIsEntry = 0;
        fn_804AE3BC()->fn_8050E094(-1, 0x101C, data, 0);
    }
    for (int i = 0; i < (int)mgr->mCharas.size(); i++) {
        PPCharaEntry_unk entry = mgr->mCharas[i];
        if (!entry.mActive) {
            continue;
        }
        CharaTask* chara = entry.mRef.mChara;
        if (chara == 0) {
            continue;
        }
        if ((chara->mFlags & 4) == 4 && fn_804E650C(chara) == 0) {
            continue;
        }
        NetMsgI_unk<PPPlayerInfo_unk, 10> msg(0x101C);
        PPPlayerInfo_unk* data = &msg.mData;
        data->mAid = i;
        data->m00 = ((PPChara_unk*)chara)->m7D8;
        data->m08_0 = !chara->mStatus.IsState_unk(0x20);
        data->mIsEntry = 1;
        fn_804AE3BC()->fn_8050E094(-1, 0x101C, data, 0);
    }
}

#include "game/SpecialEnemyBase.h"

// inferred: weak copy 0x8000DCF4 (inlined in fn_804E7628, called by the bigger fn_804E7AC0)
inline PlayerTask* PlayerManager::GetHead_unk() {
    return mHead;
}

// inferred: payloads of the messages 0x1014/0x101A (a packed character and its network id or index) and 0x101B
// (also the state of a special enemy at +0x15); not padded on the network; names invented
#pragma pack(push, 1) // inferred: network payload without padding
struct PPMsgCharaFull_unk {
    PackedChara_unk mChara;
    unsigned char mIndex; // 0x14
};
struct PPMsgSpecialFull_unk {
    PackedChara_unk mChara;
    unsigned char mIndex;   // 0x14
    SpecialState_unk mState; // 0x15
};
#pragma pack(pop)

// inferred: sends the full state of the own character (0x1014), of the characters of the team players of this
// machine (0x101A) and of the character list (0x101A, or 0x101B with the state of the special enemies)
extern "C" void fn_804E7628(PPManager2_unk* mgr) {
    fn_804EB870(mgr);
    CharaTask* own = ((PlayerManager*)fn_8000D9E8())->GetHead_unk();
    {
        NetMsgI_unk<PPMsgCharaFull_unk> msg(0x1014);
        PPMsgCharaFull_unk* data = &msg.mData;
        data->mIndex = lbl_8087F628->GetMyAid_unk();
        fn_804CF468(&data->mChara, own);
        fn_804AE3BC()->fn_8050E094(-1, 0x1014, data, 0);
    }
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        if (!IsTeamPlayer_unk(&mgr->mPlayers[i])) {
            continue;
        }
        PPPlayer_unk* player = fn_804D5F0C(mgr, i);
        if (!player->mFlags.mActive || (player->mAid & 0xF) != lbl_8087F628->GetMyAid_unk()) {
            continue;
        }
        CharaTask* chara = player->mRef.mChara;
        if (chara == 0 || (chara->mFlags & 4) == 4) {
            continue;
        }
        NetMsgI_unk<PPMsgCharaFull_unk> msg(0x101A);
        PPMsgCharaFull_unk* data = &msg.mData;
        data->mIndex = player->mAid;
        fn_804CF468(&data->mChara, player->mRef.mChara);
        data->mChara.m12_1 = 0;
        fn_804AE3BC()->fn_8050E094(-1, 0x101A, data, 0);
    }
    for (int i = 0; i < (int)mgr->mCharas.size(); i++) {
        PPCharaEntry_unk* entry = &mgr->mCharas[i];
        if (!entry->mActive) {
            continue;
        }
        CharaTask* chara = entry->mRef.mChara;
        if (chara == 0) {
            continue;
        }
        if ((chara->mFlags & 4) == 4 && fn_804E650C(chara) == 0) {
            continue;
        }
        switch (((BattleCharaTask*)chara)->m146C) {
        case 19:
        case 21:
        case 22:
        case 23:
        case 25:
        case 32:
        case 33:
        case 35:
        case 36:
        case 40: {
            NetMsgI_unk<PPMsgSpecialFull_unk> msg(0x101B);
            PPMsgSpecialFull_unk* data = &msg.mData;
            data->mIndex = i;
            fn_804CF468(&data->mChara, entry->mRef.mChara);
            data->mChara.m12_1 = 1;
            ((SpecialEnemyBase*)chara)->vf110(&data->mState);
            fn_804AE3BC()->fn_8050E094(-1, 0x101B, data, 0);
            break;
        }
        default: {
            NetMsgI_unk<PPMsgCharaFull_unk> msg(0x101A);
            PPMsgCharaFull_unk* data = &msg.mData;
            data->mIndex = i;
            fn_804CF468(&data->mChara, entry->mRef.mChara);
            data->mChara.m12_1 = 1;
            fn_804AE3BC()->fn_8050E094(-1, 0x101A, data, 0);
            break;
        }
        }
    }
}

// inferred: sends the state of the own character, of the characters of this aid and of the other
// characters (messages 0x1017 and 0x101E)
// NONMATCHING: 0% - equivalent (checked against the whole original); the original calls every accessor
// (GetModel_unk, Vector3 operators, WrapAngle_unk, the weak copies 0x804E8090-0x804E82BC) with bl, here they
// are inlined (rule R3 of docs/inlining.md)
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


extern "C" int fn_804F50DC(PPManager2_unk* mgr, PPPlayer_unk* player, unsigned short a, unsigned short b);
// inferred: payload of the message 0x103E (two halfwords); names invented
struct PPMsgHalves_unk {
    unsigned short m00;
    unsigned short m02;
};
#include "MP/MpMessage_unk.h"
// inferred: the message table m2A50 of MP::Manager2 seen as the table of MP/MpMessage_unk.h
static inline MpMessageTable_unk* PPMessages_unk(PPManager2_unk* mgr) {
    return (MpMessageTable_unk*)&mgr->m2A50;
}
// inferred: one entry of the quick message list saved with the friend list (lbl_8087F628 + 0xC38, as in
// MP/D2AMpMenuMessageList.cpp); names invented
struct PPMessageRef_unk {
    short mCategory;
    short mIndex;
};
// inferred: payload of the message 0x1030 (voice of a message); names invented
struct PPMsgVoice_unk {
    unsigned int mVoice;
    int m04;
};
// NONMATCHING: 96.02% - the categories of m2A50 addressed from the table and not from the manager (as fn_804F50DC),
// registers
// inferred: says the quick message `slot` of the page shown by the message list: category 20 is the message of
// the own character with the same id (m04) in the category of its stage (table); plays its voice and, with a
// slot of its own, sends the voice (message 0x1030), shows it (fn_804F50DC) and sends it (message 0x103E).
// 0 when nothing was said
extern "C" int fn_804E82C4(PPManager2_unk* mgr, int slot) {
    if (slot < 0 || 10 <= slot) {
        return 0;
    }
    if (lbl_8087EE90 == 0 || lbl_8087F498 == 0) {
        return 0;
    }
    if (fn_800CB58C(&mgr->m2B70)) {
        return 0;
    }
    if (lbl_8087F498->fn_803EBA54() == 0) {
        return 0;
    }
    PPMessageRef_unk* ref =
        &((PPMessageRef_unk*)&lbl_8087F628->mFriendList.m7F0[0x18])[MP::D2AMpMenuMessageList::sPage * 10 + slot];
    int category = ref->mCategory;
    int index = ref->mIndex;
    MpMessage_unk* msg = PPMessages_unk(mgr)->fn_804C54FC(category, index);
    if (category == 20) {
        int table[26] = {0, 2, 3, 4, 6, 5, 1, 8, 7, 9, 14, 17, 17, 17, 16, 16, 16, 11, 11, 12, 18, 18, 0, 13, 19, 0};
        PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
        if (own != 0) {
            MpMessage_unk* found = 0;
            category = table[own->mB0];
            for (unsigned int i = 0; i < PPMessages_unk(mgr)->GetCount_unk(category); i++) {
                MpMessage_unk* other = PPMessages_unk(mgr)->fn_804C54FC(category, i);
                if (other != 0 && (unsigned char)other->m04 == (unsigned char)msg->m04) {
                    found = other;
                    index = i;
                    break;
                }
            }
            msg = found;
        }
    }
    if (msg == 0) {
        return 0;
    }
    mgr->m2B70 = lbl_8087F498->fn_803E9B7C(msg->mVoice, 0, 1, 1.0f);
    if (fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) == 0) {
        return 1;
    }
    {
        NetMsgI_unk<PPMsgVoice_unk> voice(0x1030);
        PPMsgVoice_unk* data = &voice.mData;
        data->mVoice = msg->mVoice;
        data->m04 = 0;
        fn_804AE3BC()->fn_8050E094(-1, 0x1030, data, 0);
    }
    fn_804F50DC(mgr, fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()), category, index);
    NetMsgI_unk<PPMsgHalves_unk> m(0x103E);
    PPMsgHalves_unk* data = &m.mData;
    data->m00 = category;
    data->m02 = index;
    fn_804AE3BC()->fn_8050E094(-1, 0x103E, data, 0);
    return 1;
}

// inferred: applies `value` to the own slot (fn_804F50DC with 0xFFFF) and sends it to the others (message 0x103E)
extern "C" int fn_804E8A80(PPManager2_unk* mgr, int value) {
    unsigned int aid = lbl_8087F628->GetMyAid_unk();
    PPPlayer_unk* own = fn_804DD158(mgr, aid);
    if (fn_804F50DC(mgr, own, 0xFFFF, value) == 0) {
        return 0;
    }
    NetMsgI_unk<PPMsgHalves_unk> msg(0x103E);
    PPMsgHalves_unk* data = &msg.mData;
    data->m00 = 0xFFFF;
    data->m02 = value;
    fn_804AE3BC()->fn_8050E094(-1, 0x103E, data, 0);
    return 1;
}

// inferred: the player slot (in use) of a character
static inline PPPlayer_unk* FindByChara_unk(PPManager2_unk* mgr, CharaTask* chara) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1 && player->mRef.mChara == chara) {
            return player;
        }
    }
    return 0;
}

// inferred: index of a character in the list m5F0 (-1 when it is not there)
static inline int FindCharaIndex_unk(PPManager2_unk* mgr, CharaTask* chara) {
    for (int i = 0; i < (int)mgr->mCharas.size(); i++) {
        if (mgr->mCharas[i].mRef.mChara == chara) {
            return i;
        }
    }
    return -1;
}

// inferred: player slot of the own aid
static inline PPPlayer_unk* GetMyPlayer_unk() {
    atn::net::Manager* net = lbl_8087F628;
    PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
    return fn_804DD158(mgr, net->GetMyAid_unk());
}

extern "C" int fn_804EB4AC(PPManager2_unk* mgr, PPPlayer_unk* player);
extern "C" PPCharaEntry_unk* fn_804EB814(PPManager2_unk* mgr, CharaTask* chara);
// inferred: a character `killer` defeated `victim`: message 22 (player by player: both aids; only when the victim
// is marked m30 or is in the team of the own player), 23 (player by an enemy of the list in use: aid and index)
// or 24 (enemy by player: index and aid, same condition as 22); shown by fn_804F50DC and sent to everybody
// (message 0x103E). 1 when a message was sent
extern "C" int fn_804E8BF4(PPManager2_unk* mgr, CharaTask* killer, CharaTask* victim) {
    int category = 0;
    unsigned short value = 0;
    if (FindByChara_unk(mgr, killer) != 0) {
        PPPlayer_unk* a = FindByChara_unk(mgr, killer);
        if (FindByChara_unk(mgr, victim) != 0) {
            PPPlayer_unk* b = FindByChara_unk(mgr, victim);
            if (b->mFlags.m30 == 1 || PPIsOwnTeam_unk(b) != 0) {
                category = 22;
                value = (a->mAid << 8) | b->mAid;
            }
        } else if (fn_804EB814(mgr, victim) != 0 && fn_804EB814(mgr, victim)->mActive == 1) {
            value = (a->mAid << 8) | FindCharaIndex_unk(mgr, victim);
            category = 23;
        }
    } else if (fn_804EB814(mgr, killer) != 0) {
        FindCharaIndex_unk(mgr, killer);
        if (FindByChara_unk(mgr, victim) != 0) {
            PPPlayer_unk* b = FindByChara_unk(mgr, victim);
            if (b->mFlags.m30 == 1 || PPIsOwnTeam_unk(b) != 0) {
                value = (FindCharaIndex_unk(mgr, killer) << 8) | b->mAid;
                category = 24;
            }
        } else {
            FindCharaIndex_unk(mgr, victim);
        }
    }
    if (category != 0) {
        fn_804F50DC(mgr, GetMyPlayer_unk(), category, value);
        NetMsgI_unk<PPMsgHalves_unk> msg(0x103E);
        PPMsgHalves_unk* data = &msg.mData;
        data->m00 = category;
        data->m02 = value;
        fn_804AE3BC()->fn_8050E094(-1, 0x103E, data, 0);
        return 1;
    }
    return 0;
}

#include "game/GimmickManager.h"
#include "gmk/Item.h"
#include "gmk/Cannonball.h"
extern "C" void fn_804E9C64(PPManager2_unk* mgr);
// inferred: sends `msg` to a gimmick with the character listeners enabled
inline void PPSendGimmick_unk(GimmickCommon* gimmick, GimmickMsg_unk* msg) {
    int enabled = fn_8014F8F4()->mEnabled;
    fn_8014F8F4()->mEnabled = 1;
    gimmick->vf40(msg);
    fn_8014F8F4()->mEnabled = enabled;
}
// NONMATCHING: 86.98% - the original materializes the server test (cntlzw) before the branch, keeps the manager,
// the loop offset and the zeros on the stack (register pressure) and builds the messages in another order
// inferred: drops the players that left: a slot whose player is not connected any more is cleared (m10 = 2);
// its character is disabled and, on the server, the cannonballs it carried fall (message 3) and the items it held
// go back (messages 6 and 1). In the game (state 30) a new server drops what the old one held. In the lobby of game
// mode 1 the teams are cleared when a player left; in the game fn_804E9C64 follows
extern "C" void fn_804E9360(PPManager2_unk* mgr) {
    bool left = false;
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers.begin()[i];
        unsigned char aid = IsTeamPlayer_unk(player) == 1 ? player->mAid & 0xF : player->mAid;
        atn::net::NetPlayerInfo_unk* info = lbl_8087F628->vf14(aid);
        if (info == 0 || !info->mFlags.mConnected) {
            if (player->mFlags.mActive == 1) {
                PPPlayerFlags_unk flags;
                *(unsigned int*)&flags = 0;
                flags.m10 = 2;
                player->mAid = 0xFF;
                player->mDC.fn_80501288(0);
                player->mB0 = 0;
                left = true;
                player->mD38[4] = 0;
                player->mD50 = -1;
                player->mD4C = 0;
                player->mD53 = 0;
                player->mD54 = -1;
                player->mD58 = 0;
                player->mD8 = 0;
                player->mD4 = 0;
                player->mFlags = flags;
            }
            CharaTask* chara = player->mRef.mChara;
            if (chara != 0 && (chara->mFlags & 2) == 2) {
                if ((chara->mFlags & 4) != 4) {
                    chara->fn_801765D8();
                }
                if (fn_804EB1AC(mgr) != 0) {
                    for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick != 0;
                         gimmick = gimmick->m5C) {
                        if (gimmick->m50 == 7) {
                            gmk::Cannonball* cannon = (gmk::Cannonball*)gimmick;
                            if (i == cannon->m520) {
                                GimmickMsg_unk msg(3);
                                msg.m10 = (int)player->mRef.mChara;
                                msg.m08 = 0;
                                msg.mVec = gimmick->m6C;
                                msg.mVec.y += 2.0f;
                                msg.m0C = 1;
                                cannon->m520 = 0xFF;
                                PPSendGimmick_unk(gimmick, &msg);
                            }
                        } else if (gimmick->m50 == 8) {
                            gmk::Item* item = (gmk::Item*)gimmick;
                            if (item->mDrop != 0 && i == item->mDrop->m29C) {
                                int enabled = fn_8014F8F4()->mEnabled;
                                fn_8014F8F4()->mEnabled = 1;
                                GimmickMsg_unk drop(6);
                                item->mDrop->vf40(&drop);
                                item->mDrop->m29C = 0xFF;
                                GimmickMsg_unk back(1);
                                item->vf40(&back);
                                item->fn_803FC65C();
                                fn_8014F8F4()->mEnabled = enabled;
                            }
                        }
                    }
                }
            }
        } else {
            player->mFlags.mActive = 1;
        }
    }
    if (mgr->mStateVar.mValue == 30 && mgr->m50C == 0) {
        unsigned char server = lbl_8087F628->GetServerAid_unk();
        if (server != mgr->m997A[0]) {
            for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick != 0; gimmick = gimmick->m5C) {
                if (gimmick->m50 == 7) {
                    gmk::Cannonball* cannon = (gmk::Cannonball*)gimmick;
                    if (cannon->m520 != 0xFF && (gimmick->m54 == 1 || gimmick->m54 == 4)) {
                        GimmickMsg_unk msg(3);
                        msg.m10 = 0;
                        msg.m08 = 0;
                        msg.mVec = gimmick->m6C;
                        msg.mVec.y += 2.0f;
                        msg.m0C = 1;
                        cannon->m520 = 0xFF;
                        PPSendGimmick_unk(gimmick, &msg);
                    }
                } else if (gimmick->m50 == 8) {
                    gmk::Item* item = (gmk::Item*)gimmick;
                    item->fn_803FC65C();
                    if (item->mDrop != 0 && item->mDrop->m29C != 0xFF && gimmick->m54 == 2 && item->mDrop->m54 >= 5) {
                        int enabled = fn_8014F8F4()->mEnabled;
                        fn_8014F8F4()->mEnabled = 1;
                        GimmickMsg_unk drop(6);
                        item->mDrop->vf40(&drop);
                        item->mDrop->m29C = 0xFF;
                        GimmickMsg_unk back(1);
                        item->vf40(&back);
                        fn_8014F8F4()->mEnabled = enabled;
                    }
                }
            }
        }
        mgr->m997A[0] = server;
    }
    if (left) {
        if (mgr->mStateVar.mValue == 12 && mgr->mMode == 1 && lbl_8087F628->m25C == 0 &&
            MP::D2AMpMenuLobby::sInstance != 0 && MP::D2AMpMenuLobby::sInstance->mD88 < 7) {
            for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
                if (mgr->mPlayers.begin()[i].mFlags.mActive) {
                    mgr->mPlayers.begin()[i].mFlags.m22 = 0;
                }
            }
        }
        if (mgr->mStateVar.mValue == 30 && mgr->m50C == 0) {
            fn_804E9C64(mgr);
        }
    }
}

#include "game/EnemyManager.h"

// inferred: index of the network id `aid` among the hosts of the session (0 in local play, -1 when it is not
// there); name invented
static inline int GetHostIndex_unk(unsigned char aid) {
    if (lbl_8087F628->mFlags.mB12) {
        return 0;
    }
    unsigned char* aids;
    int n = fn_8050BA68(&aids);
    for (int i = 0; i < n; i++) {
        if (aid == aids[i]) {
            return i;
        }
    }
    return -1;
}

// NONMATCHING: 72.02% - the push_back of the new entry copies the words 0x34-0xAF as a block (a loop) in the
// original, here member by member; a struct for them (with a declared constructor) gives 95.6% but turns the copy
// fn_804E7434 into a block copy too (100 -> 0%)
// inferred: rebuilds the character list m5F0 from the enemies that are not the character of a player (the
// enemies 0xAAE60-0xAAE79 are left out); the enemies are shared out among the hosts in turn: the ones of this
// machine are taken over (control state 2, reset, special enemies started), the others wait for the network
// (flag 2 set, m1454 = 3)
extern "C" void fn_804E9C64(PPManager2_unk* mgr) {
    int myIndex = GetHostIndex_unk(lbl_8087F628->GetMyAid_unk());
    int hosts = GetNumHosts_unk(lbl_8087F628);
    mgr->mCharas.clear();
    int count = 0;
    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
        if (FindByChara_unk(mgr, enemy) != 0) {
            continue;
        }
        if (0xAAE60 <= enemy->m50 && enemy->m50 < 0xAAE7A) {
            continue;
        }
        int mine = count++ % hosts == myIndex;
        PPCharaEntry_unk entry;
        entry.mRef.mChara = 0;
        memset(&entry.m04[12], 0, 0xB0 - 0x34); // inferred: the words 0x34-0xAF are cleared, 0x04-0x33 are not
        entry.mRef.mChara = enemy;
        entry.mActive = mine != 0;
        mgr->mCharas.push_back(entry);
        if (enemy->mStatus.IsState_unk(0x20) && enemy->GetFlag22_unk() == 1) {
            continue;
        }
        if (mine && enemy->mFlag2) {
            enemy->mFlag2 = 0;
            enemy->m1454 = 1;
            if (enemy->CanAct_unk()) {
                enemy->fn_8016E970(2);
                enemy->fn_8015495C(0, 1, 1, 1);
                if (enemy->mFlag31) {
                    enemy->fn_801539E0();
                }
                if (enemy->mFlag5) {
                    enemy->mFlag5 = 0;
                }
                if (enemy->mFlag6) {
                    enemy->fn_80155DAC(0);
                }
                switch (enemy->m146C) {
                case 0:
                case 2:
                    enemy->m58C = 0;
                    break;
                case 19:
                case 21:
                case 22:
                case 23:
                case 25:
                case 32:
                case 33:
                case 35:
                case 36:
                case 40:
                    ((SpecialEnemyBase*)enemy)->vfFC();
                    break;
                }
            }
        } else if (!mine) {
            enemy->mFlag2 = 1;
            enemy->m1454 = 3;
        }
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

// inferred: message 0x1013 (as message 0x18, fn_804F1F34): marks the own player and the players of its team (m26)
extern "C" void fn_804EA8B8(PPManager2_unk* mgr, int arg) {
    PPPlayer_unk* player;
    fn_804AE3BC()->fn_8050E094(-1, 0x1013, 0, 1);
    fn_804AE3BC()->fn_8050DDDC(-1, 1);
    player = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
    player->mFlags.m26 = 1;
    if (player->mRef.mChara != 0) {
        player->mRef.mChara->fn_800D246C(arg);
    }
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* other = &mgr->mPlayers[i];
        if (IsTeamPlayer_unk(other) && (unsigned int)(other->mAid & 0xF) == player->mAid) {
            other->mFlags.m26 = 1;
            if (other->mRef.mChara != 0) {
                other->mRef.mChara->fn_800D246C(arg);
            }
        }
    }
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

extern "C" int fn_804FC7E0(PPManager2_unk* mgr, atn::FixedArray_unk<gs::WarpPoint*, 32>* list, int team);
extern "C" int rand();
extern "C" float PSVECSquareMag(const atn::Vector3* v);

// inferred: random offset of a warp position (-2.5 to 2.5)
static inline float RandWarpOffset_unk() {
    return 5.0f * ((rand() % 1001) / 1000.0f) - 2.5f;
}

// NONMATCHING: 99.91% - register of the count of the list (the original reuses the one of the index)
// inferred: places the character of a player on a warp point of its team: the kept one of its slot (m56C,
// `keep`), else (in game mode 0, or 1 without `b`) the next one after the last chosen (m608) far enough from the
// other players (50), else a random one, with a random offset; the main player keeps the choice in m608
extern "C" void fn_804EAA84(PPManager2_unk* mgr, CharaTask* chara, int keep, int b) {
    PPPlayer_unk* player = 0;
    int index = -1;
    for (unsigned int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* slot = &mgr->mPlayers[i];
        if (slot->mFlags.mActive == 1 && slot->mRef.mChara == chara) {
            player = slot;
            index = i;
            break;
        }
    }
    if (chara == 0 || player == 0 || index < 0) {
        return;
    }
    atn::Vector3 pos(chara->mPos);
    atn::Vector3 rot(0.0f, chara->mRot.y, 0.0f);
    atn::FixedArray_unk<gs::WarpPoint*, 32> list;
    if (fn_804FC7E0(mgr, &list, player->mFlags.m22) != 0) {
        int sel = -1;
        if (keep != 0) {
            sel = mgr->m56C.m[index];
        } else if (mgr->mMode != 2 && (mgr->mMode == 0 || b == 0)) {
            unsigned int num;
            if (mgr->m608 >= 0 && (num = list.size()) > 2) {
                sel = (mgr->m608 + rand() % (num - 2) + 1) % num;
                gs::WarpPoint* point = list[sel];
                int ok = 1;
                for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
                    PPPlayer_unk* other = &mgr->mPlayers[i];
                    if (other->mFlags.mActive && other->mRef.mChara != 0 &&
                        !other->mRef.mChara->fn_8016F3D0(player->mRef.mChara, 1)) {
                        atn::Vector3 d = point->mPos - other->mRef.mChara->mPos;
                        if (PSVECSquareMag(&d) < 2500.0f) {
                            ok = 0;
                            break;
                        }
                    }
                }
                if (ok == 0) {
                    sel = (sel + 1) % list.size();
                }
            } else {
                sel = rand() % list.size();
            }
        }
        if (sel >= 0 && sel < (int)list.size()) {
            gs::WarpPoint* point = list[sel];
            pos = point->mPos;
            if (keep == 0) {
                atn::Vector3 offset(RandWarpOffset_unk(), RandWarpOffset_unk(), RandWarpOffset_unk());
                pos += offset;
            }
            atn::VecAssign_unk(rot, atn::Vector3(0.0f, point->m14, 0.0f));
            if (player->mFlags.m30 == 1) {
                mgr->m608 = sel;
            }
        }
    }
    chara->mPos = pos;
    chara->mRot = rot;
    chara->fn_8015495C(1, 1, 1, 1);
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
    for (int i = 0; i < (int)mgr->mCharas.size(); i++) {
        if (mgr->mCharas[i].mRef.mChara == chara) {
            id.m.mKind = 3;
            id.m.mIndex = i;
            return id.mRaw << 16;
        }
    }
    return id.mRaw << 16;
}

// inferred: number of the player slots in use
extern "C" int fn_804EAFD0(PPManager2_unk* mgr) {
    int n = 0;
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive) {
            n++;
        }
    }
    return n;
}

// inferred: `chara` is the own character (game mode 0) or in the team of the own player
extern "C" int fn_804EB010(PPManager2_unk* mgr, CharaTask* chara) {
    PPPlayer_unk* own = PPFindSlot_unk(mgr, lbl_8087F628->GetMyAid_unk());
    if (own == 0) {
        return 0;
    }
    PPPlayer_unk* other = FindByChara_unk(mgr, chara);
    FindCharaIndex_unk(mgr, chara);
    if (mgr->mMode == 0) {
        return own->mRef.mChara == chara;
    }
    if (other != 0) {
        return own->mFlags.m22 == other->mFlags.m22;
    }
    return 0;
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
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1) {
            if (!player->mFlags.m28) {
                all = 0;
            }
            // inferred: the second test reads the flags as a word (the original loads it again for the bit of m28)
            if ((player->mFlagWord >> 28) & 1) {
                n++;
            }
        }
    }
    if (count != 0) {
        *count = n;
    }
    return all;
}

// inferred: in game mode 1, the players in use are not all of one team (1 also when there is no team)
extern "C" int fn_804EB2D8(PPManager2_unk* mgr) {
    if (mgr->mMode != 1) {
        return 1;
    }
    int i;
    PPPlayer_unk* player;
    PPPlayer_unk* first = 0;
    for (i = 0; i < (int)mgr->mPlayers.size(); i++) {
        player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1) {
            first = player;
            break;
        }
    }
    if (first == 0) {
        return 1;
    }
    if (first->mFlags.mActive == 1 && first->mFlags.m22 == 0) {
        return 1;
    }
    unsigned int n = 0;
    unsigned int team = first->mFlags.m22;
    for (i = 0; i < (int)mgr->mPlayers.size(); i++) {
        player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1) {
            n++;
            if (team != player->mFlags.m22) {
                return 1;
            }
        }
    }
    return n <= 1;
}

// inferred: the slot `index` holds a player of a team
extern "C" int fn_804EB3CC(PPManager2_unk* mgr, int index) {
    return IsTeamPlayer_unk(&mgr->mPlayers[index]);
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

// inferred: `player` is in the team of the own player
extern "C" int fn_804EB4AC(PPManager2_unk* mgr, PPPlayer_unk* player) {
    PPManager2_unk* m = (PPManager2_unk*)lbl_8087F610;
    PPPlayer_unk* me = fn_804DD158(m, lbl_8087F628->GetMyAid_unk());
    if (me == 0 || player == 0 || !IsTeamPlayer_unk(player)) {
        return 0;
    }
    return me->mAid == (player->mAid & 0xF);
}

// inferred: every player in use that is not in a team has m27 set
extern "C" bool fn_804EB5D8(PPManager2_unk* mgr) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
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
        if (player->mEquip0.m00 != -1) {
            return &player->mEquip0;
        }
        return 0;
    case 1:
        if (player->mEquip1.m00 != -1) {
            return &player->mEquip1;
        }
        return 0;
    case 2:
        if (player->mEquip2.m00 != -1) {
            return &player->mEquip2;
        }
        return 0;
    case 3:
        if (player->mEquip3.m00 != -1) {
            return &player->mEquip3;
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
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1 && player->mRef.mChara == chara) {
            return player;
        }
    }
    return 0;
}

// inferred: the entry of the character list of a character
extern "C" PPCharaEntry_unk* fn_804EB814(PPManager2_unk* mgr, CharaTask* chara) {
    int index = FindCharaIndex_unk(mgr, chara);
    if (index >= 0) {
        return &mgr->mCharas[index];
    }
    return 0;
}

// inferred: slot of the own player
// inferred: the aid as unsigned int puts it in r5 and the offset in r4, as in the original
extern "C" PPPlayer_unk* fn_804EB870(PPManager2_unk* mgr) {
    unsigned int aid = lbl_8087F628->GetMyAid_unk();
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* player = &mgr->mPlayers[i];
        if (player->mFlags.mActive == 1 && aid == player->mAid) {
            return player;
        }
    }
    return 0;
}

// inferred: character named in a message: kind 1 none, 2 player (network id mIndex), 3 character mIndex of the list;
// names invented
struct PPMsgCharaRef_unk {
    unsigned char mKind;
    unsigned char mIndex;
};

// inferred: data of messages 8 and 64 (a hit dealt by another player); names invented
struct PPMsgHit_unk {
    int mHitId;              // 0x00: inferred: id of the hit data (fn_80219E6C)
    atn::Vector3 m04;
    atn::Vector3 m10;
    unsigned short mFlag : 1; // 0x1C
    unsigned short m1C_0 : 7;
    PPMsgCharaRef_unk mAttacker; // 0x1D
};

// inferred: data of message 7 (a hit received by a character from another player); names invented
struct PPMsgDamage_unk {
    atn::Vector3 m00;     // inferred: direction
    atn::Vector3 m0C;
    unsigned int mFlags;  // 0x18
    int mHitId;           // 0x1C
    int m20;
    PPMsgCharaRef_unk mTarget; // 0x24
};

#include "gmk/DropItem.h"
// inferred: payload of the message 6 (a message for a gimmick: id and kind of the gimmick, message, sender);
// names invented
struct PPMsgGimmick_unk {
    unsigned int mId;
    int m04;
    int m08;
    unsigned short mKind;
    unsigned short mType;
    atn::Vector3 mVec;
    unsigned char mSender;   // inferred: 1 none, 2 a player (aid in mSenderId), 3 an entry of the character list
    unsigned char mSenderId;
    unsigned char m1E;
};

#include "game/BattleCalc_unk.h"
#include "game/HitInfo_unk.h"
extern "C" int fn_8021A684(HitInfo_unk* data); // inferred: db/ParseUtil_unk (kind tests of the hit data)
static inline int PPTeamSize_unk(PPManager2_unk* mgr, CharaTask* chara);
// inferred: data of messages 10 and 11 (a hit of a character on another one, computed here; sent as 0x1022 and 0x1021 by fn_804EB934); names invented
struct PPMsgAttack_unk {
    int mHitId;                       // 0x00: inferred: hit data (message 10) or item code (message 11)
    unsigned int mFlags;              // 0x04
    float mRate;                      // 0x08: inferred: DamageParam_unk::mRate
    int mCombo;                       // 0x0C: inferred: passed to CharaTask::fn_80150460
    CharaStatusEntry_unk mEntries[6]; // 0x10: inferred: status effects of the attacker
    PPMsgCharaRef_unk mAttacker;      // 0x88
    PPMsgCharaRef_unk mTarget;        // 0x8A
};
#include "game/HitEvent_unk.h"
// inferred: payload of message 0x1023 (the attack parameters of event 8; 15 bytes on the network); names invented
struct PPMsgCalc_unk {
    int mHitId;
    unsigned int mFlags;
    unsigned int m08_0 : 4; // inferred: BattleCalcParam_unk::m44
    unsigned int m08_1 : 4; // inferred: BattleCalcParam_unk::m3C
    unsigned int m08_2 : 8; // inferred: BattleCalcParam_unk::m40
    unsigned int m08_3 : 1; // inferred: BattleCalcParam_unk::m48
    PPMsgCharaRef_unk mAttacker; // 0x0B
    PPMsgCharaRef_unk mTarget;   // 0x0D
};
// inferred: payload of message 0x1031 (a value of event 9 and whether there was one; 5 bytes); names invented
struct PPMsgValue_unk {
    int mValue;
    unsigned char mValid : 1;
};
// inferred: data of event 12 (a shot) and its optional target part; names invented
struct PPEventShotTarget_unk {
    CharaTask* mChara;
    float m04[6];
    int m1C;
};
struct PPEventShot_unk {
    CharaTask* mChara;
    int m04;
    atn::Vector3 m08;
    atn::Vector3 m14;
    int m20;
    PPEventShotTarget_unk* m24;
};
// inferred: data of message 5 (a bullet shot by another player; sent as 0x1024 by fn_804EB934, 0x26 bytes); names
// invented
struct PPMsgShot_unk {
    int mHitId;           // 0x00
    atn::Vector3 m04;     // inferred: position
    atn::Vector3 m10;     // inferred: direction
    unsigned char m1C[0x20 - 0x1C];
    unsigned int mFlags;  // 0x20
    PPMsgCharaRef_unk mShooter; // 0x24
};
// inferred: message 5 with a target part (sent as 0x1025, 0x44 bytes); names invented
struct PPMsgShotTarget_unk {
    int mHitId;
    atn::Vector3 m04;
    atn::Vector3 m10;
    unsigned char m1C[0x20 - 0x1C];
    unsigned int mFlags;
    PPMsgCharaRef_unk mShooter; // 0x24
    PPMsgCharaRef_unk mTarget;  // 0x26
    float m28[6];
    int m40;
};
// inferred: data of event 14 (a hit received, sent as message 7) and of event 15 (a hit dealt, sent as message 8
// or 64); names invented
struct PPEventDamage_unk {
    int mHitId;
    atn::Vector3 m04;
    atn::Vector3 m10;
    int m1C;
    unsigned int m20;
};
// NONMATCHING: 96.78% - the halving of the constant 10 (li 10; srawi, as fn_804F3EEC), stack slots of the messages,
// the status effects copied through the status (base 0x7D4) and reloads of the slot count and of lbl_8087F610
// inferred: listener of the characters (registered by the constructor of MP::Manager2), only in the game (state 30):
// 5 a character is knocked down (the countdown of the own side, a quick message, a voice and a sound), 6 and 7 a
// hit computed here (the score of the one that knocked down a character, fn_804E8BF4, the hate and messages 0x1021
// and 0x1022), 8 the attack parameters (message 0x1023), 9 a value (message 0x1031), 10 a character of the list
// (message 0x103F), 11 a message for a gimmick (message 0x1027; a cannonball or a dropped item ask the server
// first), 12 a shot (messages 0x1024 and 0x1025), 14 a hit received (message 0x102E) and 15 a hit dealt (messages
// 0x102F and 0x1049)
extern "C" void fn_804EB934(int id, CharaTask* chara, void* arg) {
    PPManager2_unk* mgr = fn_800F7F90();
    if (mgr->mState != 30) {
        return;
    }
    switch (id) {
    case 5: {
        PPPlayer_unk* player = FindByChara_unk(mgr, chara);
        if (player == 0) {
            break;
        }
        if (player == fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) ||
            IsSameTeam_unk(player, GetMyPlayer_unk()) == 1) {
            PPManager2_unk* global = fn_800F7F90();
            player->mD38[4] = global->mMode == 2 ? global->m5C4 : global->m5C4 / 3;
            if (player == GetMyPlayer_unk()) {
                fn_804E82C4(fn_800F7F90(), ((signed char*)&lbl_8087F628->mFriendList.m7F0[0xB8])[4]);
            }
        }
        if (fn_800F7F90()->mMode == 2) {
            break;
        }
        if (lbl_8087F498 != 0 && lbl_8087F498->fn_803EBA54() != 0 &&
            !fn_800CB58C(&fn_800F7F90()->m2B74)) {
            GameVoice* voice = lbl_8087F498;
            fn_800F7F90()->m2B74 =
                voice->fn_803E9608(lbl_80790FF0[(unsigned int)rand() % 10], 0, 1, 1.0f);
        }
        fn_800CB5C8(&fn_800F7F90()->m2B78, 15, 0);
        fn_800F7F90()->m2B78 = fn_800C31F4(lbl_807910A8[rand() & 1], 0, -1, 1.0f);
        fn_800CB518(&fn_800F7F90()->m2B78, 10);
        break;
    }
    case 6:
    case 7: {
        HitEvent_unk* ev = (HitEvent_unk*)arg;
        if (ev->mFlag31 != 0) {
            PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
            PPPlayerArray_unk& players = mgr->mPlayers;
            for (int i = 0; i < (int)players.size(); i++) {
                PPPlayer_unk* player = PPSlot_unk(mgr, i);
                if (player->mRef.mChara == 0) {
                    continue;
                }
                if (player->mFlags.m30 != 1 &&
                    !IsSameTeam_unk(&mgr->mPlayers[i], fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()))) {
                    continue;
                }
                if (player->mRef.mChara != ev->mAttacker || ev->mAttacker == 0) {
                    continue;
                }
                if (ev->mAttacker->fn_8016F3D0(ev->mTarget, 1)) {
                    continue;
                }
                mgr = fn_800F7F90();
                int points = 10;
                if (IsTeamPlayer_unk(FindByChara_unk(mgr, ev->mTarget))) {
                    points >>= 1;
                }
                bool last = !(GetRemain_unk(mgr) < 0);
                if (last) {
                    last = GetRemain_unk(mgr) < 60;
                }
                if (last) {
                    points <<= 1;
                }
                int more = PPTeamSize_unk(fn_800F7F90(), ev->mTarget) -
                                   PPTeamSize_unk(fn_800F7F90(), ev->mAttacker) >
                               0
                               ? PPTeamSize_unk(fn_800F7F90(), ev->mTarget) -
                                     PPTeamSize_unk(fn_800F7F90(), ev->mAttacker)
                               : 0;
                int old = player->mDCScore;
                fn_804DB1C4(fn_800F7F90(), player->mRef.mChara, points + more * 5);
                if (old < player->mDCScore) {
                    player->mE0++;
                }
                if (player == own) {
                    fn_804E82C4(fn_800F7F90(),
                                ((signed char*)&lbl_8087F628->mFriendList.m7F0[0xB8])[3]);
                }
                break;
            }
            for (int i = 0; i < (int)fn_800F7F90()->mPlayers.size(); i++) {
                PPManager2_unk* list = fn_800F7F90();
                PPPlayer_unk* player = PPSlot_unk(list, i);
                if (!player->mFlags.mActive || player->mRef.mChara == 0) {
                    continue;
                }
                if (player->mFlags.m30 != 1 &&
                    !IsSameTeam_unk(&list->mPlayers[i], fn_804DD158(list, lbl_8087F628->GetMyAid_unk()))) {
                    continue;
                }
                if ((player->mRef.mChara == ev->mAttacker || player->mRef.mChara == ev->mTarget) &&
                    ev->mAttacker != 0 && !ev->mAttacker->fn_8016F3D0(ev->mTarget, 1)) {
                    fn_804E8BF4(fn_800F7F90(), ev->mAttacker, ev->mTarget);
                    break;
                }
            }
        }
        if (lbl_8087F048 != 0 && fn_8021A684(fn_80219E6C(ev->mId))) {
            lbl_8087F048->fn_80102EAC(ev->mTarget, ev->mAttacker, 90, -1);
            lbl_8087F048->fn_801031D0(ev->mAttacker, ev->mTarget, 0);
        }
        // inferred: the slot of the own player is searched and not used
        fn_804DD158(fn_800F7F90(), lbl_8087F628->GetMyAid_unk());
        NetMsgI_unk<PPMsgAttack_unk, 0x8C> msg(0);
        if (id == 7) {
            id = 0x1022;
        } else if (id == 6) {
            id = 0x1021;
        }
        msg.mId = id;
        msg.mData.mHitId = ev->mId;
        msg.mData.mFlags = ev->mFlags | 0x10000000;
        msg.mData.mRate = ev->mRate;
        msg.mData.mCombo = ev->mTarget != 0 ? ev->mTarget->mFE4 : 1;
        if (ev->mAttacker != 0) {
            for (unsigned int i = 0; i < ev->mAttacker->mStatus.mEntries.mCount; i++) {
                msg.mData.mEntries[i] = ev->mAttacker->GetStatus_unk()->mEntries[i];
            }
        }
        unsigned short attacker = fn_804EAEE0(fn_800F7F90(), ev->mAttacker) >> 16;
        msg.mData.mAttacker = *(PPMsgCharaRef_unk*)&attacker;
        unsigned short target = fn_804EAEE0(fn_800F7F90(), ev->mTarget) >> 16;
        msg.mData.mTarget = *(PPMsgCharaRef_unk*)&target;
        fn_804AE3BC()->fn_8050E094(-1, id, &msg.mData, 1);
        break;
    }
    case 8: {
        BattleCalcParam_unk* calc = (BattleCalcParam_unk*)arg;
        unsigned short attacker = fn_804EAEE0(mgr, calc->m00) >> 16;
        unsigned short target = fn_804EAEE0(fn_800F7F90(), calc->m04) >> 16;
        NetMsgI_unk<PPMsgCalc_unk, 15> msg(0x1023);
        PPMsgCalc_unk* data = &msg.mData;
        data->mHitId = calc->m08->m04;
        data->mFlags = calc->m94 | 0x10000000;
        data->m08_0 = calc->m44;
        data->m08_1 = calc->m3C;
        data->m08_2 = calc->m40;
        data->m08_3 = calc->m48;
        data->mAttacker = *(PPMsgCharaRef_unk*)&attacker;
        data->mTarget = *(PPMsgCharaRef_unk*)&target;
        fn_804AE3BC()->fn_8050E094(-1, 0x1023, data, 1);
        break;
    }
    case 9: {
        NetMsgI_unk<PPMsgValue_unk, 5> msg(0x1031);
        PPMsgValue_unk* data = &msg.mData;
        if (arg != 0) {
            data->mValue = *(int*)arg;
        }
        data->mValid = arg != 0;
        fn_804AE3BC()->fn_8050E094(-1, 0x1031, data, 1);
        break;
    }
    case 10: {
        NetMsgI_unk<int> msg(0x103F);
        int* data = &msg.mData;
        *data = -1;
        PPManager2_unk* list = fn_800F7F90();
        for (int i = 0; i < (int)list->mCharas.size(); i++) {
            if (list->mCharas[i].mRef.mChara == (CharaTask*)arg) {
                *data = i;
                break;
            }
        }
        if (*data < 0) {
            break;
        }
        fn_804AE3BC()->fn_8050E094(-1, 0x103F, data, 1);
        break;
    }
    case 12: {
        PPEventShot_unk* ev = (PPEventShot_unk*)arg;
        if (ev->m24 == 0) {
            NetMsgI_unk<PPMsgShot_unk, 0x26> msg(0x1024);
            PPMsgShot_unk* data = &msg.mData;
            unsigned short shooter = fn_804EAEE0(fn_800F7F90(), ev->mChara) >> 16;
            data->mShooter = *(PPMsgCharaRef_unk*)&shooter;
            data->mHitId = ev->m04;
            data->m04 = ev->m08;
            data->m10 = ev->m14;
            data->mFlags = ev->m20;
            fn_804AE3BC()->fn_8050E094(-1, 0x1024, data, 0);
        } else {
            NetMsgI_unk<PPMsgShotTarget_unk, 0x44> msg(0x1025);
            PPMsgShotTarget_unk* data = &msg.mData;
            unsigned short shooter = fn_804EAEE0(fn_800F7F90(), ev->mChara) >> 16;
            data->mShooter = *(PPMsgCharaRef_unk*)&shooter;
            data->mHitId = ev->m04;
            data->m04 = ev->m08;
            data->m10 = ev->m14;
            data->mFlags = ev->m20;
            unsigned short target = fn_804EAEE0(fn_800F7F90(), ev->m24->mChara) >> 16;
            data->mTarget = *(PPMsgCharaRef_unk*)&target;
            data->m28[0] = ev->m24->m04[0];
            data->m28[1] = ev->m24->m04[1];
            data->m28[2] = ev->m24->m04[2];
            data->m28[3] = ev->m24->m04[3];
            data->m28[4] = ev->m24->m04[4];
            data->m28[5] = ev->m24->m04[5];
            data->m40 = ev->m24->m1C;
            fn_804AE3BC()->fn_8050E094(-1, 0x1025, data, 0);
        }
        break;
    }
    case 14: {
        PPEventDamage_unk* ev = (PPEventDamage_unk*)arg;
        NetMsgI_unk<PPMsgDamage_unk, 0x26> msg(0x102E);
        PPMsgDamage_unk* data = &msg.mData;
        unsigned short target = fn_804EAEE0(fn_800F7F90(), chara) >> 16;
        data->mTarget = *(PPMsgCharaRef_unk*)&target;
        data->mHitId = ev->mHitId;
        data->m00 = ev->m04;
        data->m0C = ev->m10;
        data->m20 = ev->m1C;
        data->mFlags = ev->m20 | 0x10000000;
        fn_804AE3BC()->fn_8050E094(-1, 0x102E, data, 0);
        break;
    }
    case 15: {
        PPEventDamage_unk* ev = (PPEventDamage_unk*)arg;
        int big = 0;
        HitInfo_unk* hit = fn_80219E6C(ev->mHitId);
        if (hit != 0) {
            if ((hit->m01 != 1 && hit->m01 != 0) || hit->m6C > 0 || hit->m70 > 0) {
                big = 1;
            }
        }
        int msgId = 0x102F;
        if (big == 1) {
            msgId = 0x1049;
        }
        NetMsgI_unk<PPMsgHit_unk, 0x1F> msg(msgId);
        PPMsgHit_unk* data = &msg.mData;
        unsigned short attacker = fn_804EAEE0(fn_800F7F90(), chara) >> 16;
        data->mAttacker = *(PPMsgCharaRef_unk*)&attacker;
        data->mHitId = ev->mHitId;
        data->m04 = ev->m04;
        data->m10 = ev->m10;
        data->mFlag = ev->m1C;
        fn_804AE3BC()->fn_8050E094(-1, msgId, data, big);
        break;
    }
    case 11: {
        GimmickCommon* gimmick = (GimmickCommon*)chara;
        GimmickMsg_unk* ev = (GimmickMsg_unk*)arg;
        NetMsgI_unk<PPMsgGimmick_unk, 0x1F> msg(0x1027);
        PPMsgGimmick_unk* data = &msg.mData;
        data->mId = gimmick->mId;
        data->mKind = gimmick->m4C;
        data->mType = ev->mType;
        data->m04 = ev->m04;
        data->m08 = ev->m08;
        data->mVec = ev->mVec;
        unsigned short sender = fn_804EAEE0(fn_800F7F90(), (CharaTask*)ev->m10) >> 16;
        *(PPMsgCharaRef_unk*)&data->mSender = *(PPMsgCharaRef_unk*)&sender;
        if (gimmick->m50 == 0x12 && ev->mType == 5) {
            if (IsServer_unk() == 1) {
                ((gmk::DropItem*)gimmick)->m2A0 = -2;
                fn_804AE3BC()->fn_8050E094(-1, 0x1027, data, 1);
            } else {
                data->m1E = GetMyPlayer_unk()->m0D52++;
                if (fn_804F5A98((PPRequests_unk*)lbl_8087F610, lbl_8087F628->GetServerAid_unk(), 150, data->m1E) ==
                    1) {
                    ((gmk::DropItem*)gimmick)->m2A0 = data->m1E;
                    fn_804AE3BC()->fn_8050E094(-1, 0x1027, data, 1);
                    fn_804AE3BC()->fn_8050DDDC(lbl_8087F628->GetServerAid_unk(), 1);
                }
            }
        } else if (gimmick->m50 == 7) {
            gmk::Cannonball* cannon = (gmk::Cannonball*)gimmick;
            if (IsServer_unk() == 1) {
                cannon->m524 = -2;
                if (ev->mType == 3 && ev->m08 == -1 && cannon->m520 == lbl_8087F628->GetMyAid_unk()) {
                    cannon->m520 = 0xFF;
                }
                fn_804AE3BC()->fn_8050E094(-1, 0x1027, data, 1);
            } else {
                data->m1E = GetMyPlayer_unk()->m0D52++;
                if (fn_804F5A98((PPRequests_unk*)lbl_8087F610, lbl_8087F628->GetServerAid_unk(), 150, data->m1E) ==
                    1) {
                    cannon->m524 = data->m1E;
                    fn_804AE3BC()->fn_8050E094(-1, 0x1027, data, 1);
                    fn_804AE3BC()->fn_8050DDDC(lbl_8087F628->GetServerAid_unk(), 1);
                }
            }
        } else {
            fn_804AE3BC()->fn_8050E094(-1, 0x1027, data, 1);
        }
        break;
    }
    }
}

extern "C" int lbl_8087F604; // inferred: magic of the data of fn_804ED768 (defined at the end)

// inferred: one entry of the data of fn_804ED768 (a kind and a value; -1/0 when unused)
struct PPDataEntry_unk {
    PPDataEntry_unk() : mKind(-1), mValue(0) {}
    PPDataEntry_unk(short kind, short value) : mKind(kind), mValue(value) {}

    short mKind;
    short mValue;
};

// inferred: 0xC0 bytes of data with a magic and a version (5), checked and reset by fn_804ED768
struct PPData_unk {
    PPData_unk() : mMagic(lbl_8087F604), mVersion(5), m08(0), m0C(0) {}

    int mMagic;                    // 0x00
    int mVersion;                  // 0x04
    int m08;
    int m0C;
    unsigned char m10[8];
    PPDataEntry_unk mEntries[40];  // 0x18
    unsigned char mB8[5];
};

// inferred: the data has the magic and the version 5
static inline int IsValid_unk(PPData_unk* data) {
    if (data == 0) {
        return 0;
    }
    if (data->mMagic != lbl_8087F604 || data->mVersion != 5) {
        return 0;
    }
    return 1;
}

// inferred: callback of the network thread (m99C0.mFuncs[1]); its two tables are lbl_807596F0 and lbl_80759718: with `init` resets `data` to the defaults (20 entries
// of kind 20 and five bytes) and returns 1; else returns whether `data` has the right magic and version
extern "C" int fn_804ED768(void* data, int init) {
    int valid = IsValid_unk((PPData_unk*)data);
    if (init == 0) {
        return valid;
    }
    PPData_unk def;
    memset(def.m10, 0, sizeof(def.m10));
    int first[10] = {0, 14, 15, 12, 17, 24, 29, 27, 16, 23};
    for (int i = 0; i < 10; i++) {
        def.mEntries[i] = PPDataEntry_unk(20, first[i]);
    }
    int second[10] = {24, 25, 26, 27, 28, 30, 41, 42, 16, 23};
    for (int i = 0; i < 10; i++) {
        def.mEntries[10 + i] = PPDataEntry_unk(20, second[i]);
    }
    for (int i = 20; i < 40; i++) {
        def.mEntries[i] = PPDataEntry_unk();
    }
    def.mB8[0] = 0;
    def.mB8[2] = 1;
    def.mB8[1] = 2;
    def.mB8[3] = 8;
    def.mB8[4] = 9;
    memcpy(data, &def, sizeof(def));
    return 1;
}

extern "C" int DWC_GetMatchIntValue(int index, const char* key, int idefault);
extern "C" const char* fn_806B3B80(int index, const char* key, const char* sdefault); // DWC_GetMatchStringValue
extern "C" int abs(int);

// inferred: name and value of the debug key of the matchmaking (as atn/NetManager.cpp; its weak objects are
// emitted here first)
inline const char* GetDebugMatchKey_unk() { return "lw_debug"; }
inline int& GetDebugMatchValue_unk() {
    static int sValue = 1;
    return sValue;
}

// inferred: evaluation callback of the matchmaking (m99C0.mFuncs[2]): -1 if the session `index` lacks the debug
// key or one of the filter keys or has another value; else the closeness of the fourth key (0x80 - distance,
// in the upper half), 0x800000 if m25C is 1
extern "C" int fn_804EDB0C(int index, void* param) {
    MpFriendList_unk* list = PPFriendList_unk();
    int result = 0x800000;
    int value = DWC_GetMatchIntValue(index, GetDebugMatchKey_unk(), 66666);
    if (value == 66666 || value != GetDebugMatchValue_unk()) {
        goto fail;
    }
    for (int i = 0; i < 8; i++) {
        atn::net::NetFilterKey_unk* key = &lbl_8087F628->mKeys[i];
        if (key->mType == 1) {
            if ((key->mFlags & 1) == 1) {
                continue;
            }
            const char* none = "notexiststring";
            const char* str = fn_806B3B80(index, key->mKey.c_str(), none);
            if (strcmp(none, str) == 0 || strcmp(key->mString.c_str(), str) != 0) {
                goto fail;
            }
        } else if (key->mType == 2) {
            if ((key->mFlags & 1) == 1) {
                continue;
            }
            int v = DWC_GetMatchIntValue(index, key->mKey.c_str(), 66666);
            if (v == 66666 || v != key->mInt) {
                goto fail;
            }
        } else {
            break;
        }
    }
    if (lbl_8087F628->m25C != 1) {
        int v = DWC_GetMatchIntValue(index, fn_8050F8B0(list, 3), 66666);
        if (v == 66666) {
            goto fail;
        }
        result = (0x80 - abs(list->m1E8 - v)) << 16;
    }
    return result;
fail:
    return -1;
}

// inferred: connection attempt callback of the matchmaking (m99C0.mFuncs[3]): the server refuses a client of
// another kind of session (m25C), out of the lobby or, in mode 1, of an incompatible game mode, and everybody
// refuses while the room is closed (m53C); else restarts the timer of the session and the server marks its slot
extern "C" int fn_804EDCCC(unsigned char* userData, void* param) {
    if (IsServer_unk() == 1) {
        int kind = lbl_8087F628->m25C;
        if (kind == 1 && ((PPManager2_unk*)lbl_8087F610)->mState != 12) {
            return 0;
        }
        if (userData != 0) {
            if (userData[0] != kind) {
                return 0;
            }
            if (kind == 1) {
                int mode = ((PPManager2_unk*)lbl_8087F610)->mMode;
                unsigned char value = userData[1];
                if (value != mode && (value == 2 || mode == 2)) {
                    return 0;
                }
            }
        }
    }
    if (((PPManager2_unk*)lbl_8087F610)->m53C == 1) {
        return 0;
    }
    atn::net::Manager* net = lbl_8087F628;
    net->m08++;
    net->m04 = 5400;
    if (IsServer_unk() == 1) {
        ((PPManager2_unk*)lbl_8087F610)->m50A = 450;
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        if (lbl_8087F628->GetMyAid_unk() == lbl_8087F628->GetServerAid_unk()) {
            PPPlayer_unk* player = fn_804DD158(mgr, lbl_8087F628->GetServerAid_unk());
            if (player != 0) {
                player->mFlags.m01 = 1;
            }
        }
    }
    return 1;
}

extern "C" void fn_804EE904(MpCharaRef_unk* ref);
extern "C" void fn_804EE9F0(PPPlayer_unk* player);
extern "C" void fn_804EE834(PPPlayer_unk* player);
extern "C" void fn_804EEB34();
extern "C" void fn_804EF0F0(unsigned int aid);
extern "C" void fn_804EF9B4();
extern "C" void fn_804EFC94();
extern "C" void fn_804EFE98(PPPlayer_unk* player);
extern "C" void fn_804EFF74(PPPlayer_unk* player, unsigned int type);
extern "C" void fn_804F1068();
extern "C" void fn_804F1124();
extern "C" void fn_804F12A0();
extern "C" void fn_804F135C();
extern "C" void fn_804F1478(PPPlayer_unk* player);
extern "C" void fn_804F1488(PPPlayer_unk* player);
extern "C" void fn_804F1498(PPPlayer_unk* player);
extern "C" void fn_804F1564(PPPlayer_unk* player, unsigned int type);
extern "C" void fn_804F1950(PPPlayer_unk* player);
extern "C" void fn_804F1CA0(PPPlayer_unk* player);
extern "C" void fn_804F1D70(PPPlayer_unk* player);
extern "C" void fn_804F1F24(PPPlayer_unk* player);
extern "C" void fn_804F1F34(PPPlayer_unk* player);
extern "C" void fn_804F2040(unsigned int type);
extern "C" void fn_804F2258();
extern "C" void fn_804F2450(unsigned int type);
extern "C" void fn_804F2994();
extern "C" void fn_804F2C44(PPPlayer_unk* player);
extern "C" void fn_804F2CE4();
extern "C" void fn_804F2CE8(unsigned int aid);
extern "C" void fn_804F2DAC(unsigned int aid);
extern "C" void fn_804F2E3C(unsigned int aid);
extern "C" void fn_804F2ECC(unsigned int aid);
extern "C" void fn_804F2F90(unsigned int aid);
extern "C" void fn_804F3020(unsigned int aid);
extern "C" void fn_804F30B0();
extern "C" void fn_804F3170();
extern "C" void fn_804F3258();
extern "C" void fn_804F34B4(PPPlayer_unk* player, unsigned int aid);
extern "C" void fn_804F36F4(PPPlayer_unk* player, unsigned int aid);
extern "C" void fn_804F36F8(PPPlayer_unk* player);
extern "C" void fn_804F3880();
extern "C" void fn_804F39D4();
extern "C" void fn_804F3AF0();
extern "C" void fn_804F3BAC();
extern "C" void fn_804F3D80(PPPlayer_unk* player);
extern "C" void fn_804F3E50();
extern "C" void fn_804F3EEC();
extern "C" void fn_804F4728();
extern "C" void fn_804F47FC(PPPlayer_unk* player, unsigned int aid);
extern "C" void fn_804F4CB8(unsigned int aid);
extern "C" void fn_804F4E1C(unsigned int aid);
extern "C" void fn_804F4E40();
extern "C" void fn_804EED7C();
extern "C" void fn_804F4F40(unsigned int aid);
extern "C" void fn_804F4F64();
extern "C" void fn_804F4FC4(int aid);
extern "C" void fn_804F50C4(unsigned int aid);
extern "C" int fn_8050E410(atn::net::PacketPack* pack, unsigned char** p, unsigned int mode);
extern "C" void fn_804EE4E0(atn::net::PacketPack* pack, unsigned int aid, unsigned short size);
extern "C" const void* fn_804EE4E4(const void* data);
extern "C" void fn_804EE4E8(atn::net::PacketPack* pack, unsigned int aid, unsigned short size);
extern "C" void fn_804EE4EC(atn::net::PacketPack* pack, const void* data, unsigned short size);

// NONMATCHING: 99.16% - the zero of m99B0 and the switch value in other registers (the original passes the
// unsigned char of the type in r4 to the handlers fn_804EFF74, fn_804F1564, fn_804F2040 and fn_804F2450; see the notes
// of escribir-20261116 qG3)
// inferred: receive loop: the packet of `aid` (a new player gets a slot), then each of its messages goes to
// its handler (in the work buffer); the answer asked by message 0x35 is sent after the loop
extern "C" int fn_804EE040(unsigned int aid, void* data, unsigned int size) {
    int type;
    fn_804D1694();
    fn_804EE4E0(fn_804AE3BC(), aid, size);
    PPPlayer_unk* player = fn_804DD158(fn_800F7F90(), aid);
    if (player == 0 && fn_804EE030(fn_800F7F90()) <= 12 && aid < fn_804D5F40(&fn_800F7F90()->mPlayers)) {
        player = fn_804D5F48(&fn_800F7F90()->mPlayers, aid);
        player->mAid = aid;
        player->mFlags.mActive = 1;
        player->mFlags.m30 = 0;
        player->mFlags.m28 = 0;
        if (fn_804EB1AC(fn_800F7F90()) == 1) {
            fn_804EE038(fn_800F7F90(), 90);
            fn_804FC0D4(fn_800F7F90(), 1);
        }
    }
    data = (void*)fn_804EE4E4(data);
    fn_804D1694();
    unsigned char* cursor = (unsigned char*)fn_804AE3BC()->fn_8050E530(data, size);
    if (cursor == 0) {
        fn_804D1694();
        fn_804EE4E8(fn_804AE3BC(), aid, size);
        return 0;
    }
    fn_804D1694();
    fn_804EE4EC(fn_804AE3BC(), cursor, size);
    fn_800F7F90()->m99B0 = 0;
    for (;;) {
        unsigned char* header = cursor;
        fn_804D1694();
        type = fn_8050E410(fn_804AE3BC(), &cursor, 1);
        if ((unsigned char)type == 0) {
            break;
        }
        if (fn_804D6348(fn_804D1694()) == 0 && (unsigned char)type != 8 && (unsigned char)type != 0x40 &&
            (unsigned char)type != 10 && (unsigned char)type != 0x1E) {
            fn_804D1694();
            if ((unsigned int)fn_804AE3BC()->fn_8050E598(aid, header) == 1) {
                continue;
            }
        }
        if (player == 0) {
            continue;
        }
        switch ((unsigned char)type) {
        case 0:
            break;
        case 1:
            fn_804D1694();
            fn_804AE3BC()->fn_8050E62C(0, "update loc\n");
            break;
        case 2:
            fn_804EE904(&player->mRef);
            break;
        case 3:
            fn_804EE9F0(player);
            break;
        case 4:
            fn_804EE834(player);
            break;
        case 5:
            fn_804EEB34();
            break;
        case 58:
            fn_804EED7C();
            break;
        case 6:
            fn_804EF0F0(aid);
            break;
        case 7:
            fn_804EF9B4();
            break;
        case 8:
        case 64:
            fn_804EFC94();
            break;
        case 9:
            fn_804EFE98(player);
            break;
        case 10:
        case 11:
            fn_804EFF74(player, type);
            break;
        case 12:
            fn_804F1068();
            break;
        case 13:
            fn_804F1124();
            break;
        case 14:
            fn_804F12A0();
            break;
        case 15:
            fn_804F135C();
            break;
        case 16:
            fn_804F1478(player);
            break;
        case 17:
            fn_804F1488(player);
            break;
        case 18:
            fn_804F1498(player);
            break;
        case 19:
        case 44:
            fn_804F1564(player, type);
            break;
        case 20:
            fn_804F1950(player);
            break;
        case 21:
            fn_804F1CA0(player);
            break;
        case 22:
            fn_804F1D70(player);
            break;
        case 23:
            fn_804F1F24(player);
            break;
        case 24:
            fn_804F1F34(player);
            break;
        case 25:
        case 26:
            fn_804F2040(type);
            break;
        case 27:
            fn_804F2258();
            break;
        case 28:
        case 29:
            fn_804F2450(type);
            break;
        case 30:
            fn_804F2994();
            break;
        case 31:
            fn_804F2C44(player);
            break;
        case 32:
            fn_804F2CE4();
            break;
        case 33:
            fn_804F2CE8(aid);
            break;
        case 34:
            fn_804F2DAC(aid);
            break;
        case 35:
            fn_804F2E3C(aid);
            break;
        case 36:
            fn_804F2ECC(aid);
            break;
        case 37:
            fn_804F2F90(aid);
            break;
        case 38:
            fn_804F3020(aid);
            break;
        case 39:
            fn_804F30B0();
            break;
        case 40:
            fn_804F3170();
            break;
        case 41:
            fn_804F3258();
            break;
        case 42:
            fn_804F34B4(player, aid);
            break;
        case 43:
            fn_804F36F4(player, aid);
            break;
        case 45:
            fn_804F36F8(player);
            break;
        case 46:
            fn_804F3880();
            break;
        case 47:
            fn_804F39D4();
            break;
        case 48:
            fn_804F3AF0();
            break;
        case 49:
            fn_804F3BAC();
            break;
        case 50:
            fn_804F3D80(player);
            break;
        case 51:
            fn_804F3E50();
            break;
        case 52:
            fn_804F3EEC();
            break;
        case 53:
            fn_804F4728();
            break;
        case 54:
        case 63:
            fn_804F47FC(player, aid);
            break;
        case 55:
            fn_804F4CB8(aid);
            break;
        case 56:
            fn_804F4E1C(aid);
            break;
        case 57:
            fn_804F4E40();
            break;
        case 59:
            fn_804F4F40(aid);
            break;
        case 60:
            fn_804F4F64();
            break;
        case 61:
            fn_804F4FC4(aid);
            break;
        case 62:
            fn_804F50C4(aid);
            break;
        }
    }
    if (fn_800F7F90()->m99B0 == 1) {
        fn_804D1694();
        fn_804AE3BC()->fn_8050DDDC(fn_800F7F90()->m99B1, fn_800F7F90()->m99B4);
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

// inferred: listeners of the character events (inline in the original, as in game/BattleCharaTask.cpp)

// inferred: calls the listeners of `id` (as in game/BattleCharaTask.cpp; name invented)
inline void NotifyListeners_unk(CharaListeners_unk* listeners, int id, void* chara, void* arg) {
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, (CharaTask*)chara, arg);
            }
        }
    }
}

#include "game/BattleTask.h"
extern "C" BattleTask* lbl_8087F048; // inferred: battle manager (as game/BattleCharaTask.cpp)


// inferred: keeps a status entry of a message in a character status (name invented)
static inline void PPAddEntry_unk(CharaStatus* status, const CharaStatusEntry_unk& entry) {
    status->mEntries.push_back(entry);
}
// inferred: character of a reference of a message (name invented)
static inline CharaTask* GetHitAttacker_unk(PPManager2_unk* mgr, const PPMsgCharaRef_unk* data) {
    switch (data->mKind) {
    case 1:
        return 0;
    case 2:
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            if (mgr->mPlayers[i].mFlags.mActive == 1 && data->mIndex == mgr->mPlayers[i].mAid) {
                return mgr->mPlayers.begin()[i].mRef.mChara;
            }
        }
        return 0;
    case 3:
        if (mgr->mCharas.size() > data->mIndex) {
            return mgr->mCharas[data->mIndex].mRef.mChara;
        }
        return 0;
    }
    return 0;
}
// inferred: the search of GetHitAttacker_unk with the character read through the slot (fn_804EFF74, whose
// registers differ from the other inlined copies); name invented
static inline CharaTask* GetHitChara_unk(PPManager2_unk* mgr, const PPMsgCharaRef_unk* data) {
    switch (data->mKind) {
    case 1:
        return 0;
    case 2:
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = &mgr->mPlayers[(unsigned int)i];
            if (player->mFlags.mActive == 1 && data->mIndex == player->mAid) {
                return mgr->mPlayers[(unsigned int)i].mRef.mChara;
            }
        }
        return 0;
    case 3:
        if (mgr->mCharas.size() > data->mIndex) {
            return mgr->mCharas[data->mIndex].mRef.mChara;
        }
        return 0;
    }
    return 0;
}

#include "game/Bullet.h"
extern "C" int fn_800EFD04(int id); // inferred: effect resources of a bullet (as gmk/BulletPoint.cpp)


extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// inferred: half float of the messages (inverse of PackHalf_unk; 0 stays 0; as FromHalf_unk of game/ReplayBuffer)
inline float UnpackHalf_unk(short h) {
    if (h == 0) {
        return 0.0f;
    }
    unsigned int e = (((h >> 10) & 0x1F) + 0x70) << 23;
    unsigned int bits = (h << 16) & 0x80000000;
    bits |= (h << 13) & 0x007FE000;
    bits |= e & 0x7F800000;
    return *(float*)&bits;
}
// inferred: data of message 4 (state of the character of a player); names invented
struct PPMsgCharaState_unk {
    atn::Vector3 mPos;  // 0x00
    atn::Vector3 mVel;  // 0x0C
    short mNode[3];     // 0x18: half floats (CharaTask::m13E8)
    short mDir[3];      // 0x1E: half floats (CharaTask::mFA4)
    float mRotY;        // 0x24
    float mTurn;        // 0x28
};

// inferred: dot product; name invented
static inline float PPDot_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
// inferred: vector of three half floats; name invented
static inline atn::Vector3 PPUnpackVec_unk(const short* h) {
    return atn::Vector3(UnpackHalf_unk(h[0]), UnpackHalf_unk(h[1]), UnpackHalf_unk(h[2]));
}
// inferred: halfway from `from` to `to`; name invented
static inline atn::Vector3 PPHalfway_unk(const atn::Vector3& from, const atn::Vector3& to) {
    return (to - from) * 0.5f + from;
}
// NONMATCHING: 99.89% - order of the two address arguments of PSVECDotProduct (the original computes the second
// address first)
// inferred: applies the state of the character of another player (only in the game, state 30, ready character
// driven by the network: mFlag2): position, velocity (none above 30 per frame), rotation, node and direction (the
// direction turns halfway when it is within 45 degrees); 1 if applied
extern "C" unsigned int fn_804EE4F0(PPPlayer_unk* player, const void* buf) {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return 0;
    }
    CharaTask* chara = player->mRef.mChara;
    if (chara == 0) {
        return 0;
    }
    if ((chara->mFlags & 2) != 2) {
        return 0;
    }
    if (chara->fn_8014FAD0()) {
        return 0;
    }
    if (chara->mFlag2 == 0) {
        return 0;
    }
    const PPMsgCharaState_unk* data = (const PPMsgCharaState_unk*)buf;
    chara->mPos = data->mPos;
    atn::Vector3 vel = data->mVel;
    if (vel.LengthSq_unk() > 900.0f) {
        chara->m13D8.Assign_unk(lbl_807C7030);
    } else {
        chara->m13D8 = vel;
    }
    chara->mRot = atn::Vector3(0.0f, data->mRotY, 0.0f);
    chara->m13E4 = data->mTurn;
    chara->m13E8.Assign_unk(PPUnpackVec_unk(data->mNode));
    atn::Vector3 dir = PPUnpackVec_unk(data->mDir);
    if (PPDot_unk(dir, chara->mFA4) > 0.70710677f) {
        atn::VecAssign_unk(dir, PPHalfway_unk(chara->mFA4, dir));
    }
    chara->mFA4 = dir;
    chara->mFlag1 = 1;
    return 1;
}

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
        const PPMsgCharaFull_unk* data = (const PPMsgCharaFull_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (data->mIndex != lbl_8087F628->GetMyAid_unk() && ref->mChara != 0) {
            fn_804CF640(&data->mChara, ref->mChara);
        }
    }
}


// inferred: applies the state of a player received from another machine (only in the game, state 30)
extern "C" void fn_804EE9F0(PPPlayer_unk* player) {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPPlayerInfo_unk* data = (const PPPlayerInfo_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (data->mAid != lbl_8087F628->GetMyAid_unk()) {
            if (player->mRef.mChara != 0) {
                ((PPChara_unk*)player->mRef.mChara)->m7D8 = data->m00;
            }
            player->mDC.fn_80501288((CharaTask*)data->mChara);
            player->mE0 = data->m06;
            player->mE4 = data->m07;
            player->mFlags.m13 = data->m13;
            player->mFlags.m12 = data->m12;
        }
    }
}

// inferred: message 5: a bullet shot by another player (only in the game, state 30); the listeners of the
// characters are off during the shot
extern "C" void fn_804EEB34() {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPMsgShot_unk* data = (const PPMsgShot_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        CharaTask* owner = GetHitAttacker_unk(mgr, &data->mShooter);
        if (owner != 0) {
            HitInfo_unk* hit = fn_80219E6C(data->mHitId);
            if (hit != 0) {
                BulletShot_unk shot;
                shot.m00 = 0.1f;
                shot.m04 = 0;
                shot.m08 = 0;
                shot.m10 = 0;
                shot.m14 = 0.0f;
                shot.m18 = 1.0f;
                shot.m04 = fn_800EFD04(hit->mB0);
                shot.m0C = hit;
                Bullet* bullet = lbl_8087F048->fn_801010A0();
                if (bullet != 0) {
                    fn_8014F8F4()->mEnabled = 0;
                    bullet->fn_8010EE78(&shot, &data->m04, &data->m10, owner, -1, data->mFlags | 0x80, -1.0f);
                    fn_8014F8F4()->mEnabled = 1;
                }
            }
        }
    }
}

#include "game/HomingBullet.h"
// inferred: data of message 58 (a homing bullet shot by another player); names invented
struct PPMsgHoming_unk {
    int mHitId;                 // 0x00
    atn::Vector3 m04;           // inferred: position
    atn::Vector3 m10;           // inferred: direction
    unsigned char m1C[0x20 - 0x1C];
    unsigned int mFlags;        // 0x20
    PPMsgCharaRef_unk mShooter; // 0x24
    PPMsgCharaRef_unk mTarget;  // 0x26
    float mAccel;               // 0x28 (as HomingParam_unk)
    float m2C;
    float mMaxSpeed;
    float mTurn;
    float mTime;
    float mDelay;               // 0x3C: inferred: not copied
    unsigned int mNode;         // 0x40
};

// inferred: message 58: a homing bullet shot by another player (only in the game, state 30); the listeners of the
// characters are off during the shot (and stay off when the hit data or a free bullet is missing)
extern "C" void fn_804EED7C() {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPMsgHoming_unk* data = (const PPMsgHoming_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        CharaTask* owner = GetHitAttacker_unk(mgr, &data->mShooter);
        fn_8014F8F4()->mEnabled = 0;
        HitInfo_unk* hit = fn_80219E6C(data->mHitId);
        if (hit == 0) {
            return;
        }
        BulletShot_unk shot;
        shot.m00 = 0.1f;
        shot.m04 = 0;
        shot.m08 = 0;
        shot.m10 = 0;
        shot.m14 = 0.0f;
        shot.m18 = 1.0f;
        shot.m04 = fn_800EFD04(hit->mB0);
        shot.m0C = hit;
        HomingParam_unk param;
        param.mTarget = 0;
        param.mAccel = 0.0f;
        param.m08 = -1.0f;
        param.mMaxSpeed = 10.0f;
        param.mTurn = 0.09424778f;
        param.mTime = 300.0f;
        param.mDelay = 5.0f;
        param.mNode = 0;
        param.mCount = -1;
        PPManager2_unk* mgr2 = (PPManager2_unk*)lbl_8087F610;
        param.mTarget = GetHitAttacker_unk(mgr2, &data->mTarget);
        param.mAccel = data->mAccel;
        param.m08 = data->m2C;
        param.mMaxSpeed = data->mMaxSpeed;
        param.mTurn = data->mTurn;
        param.mTime = data->mTime;
        param.mNode = data->mNode;
        HomingBullet* bullet = lbl_8087F048->fn_801010E8();
        if (bullet == 0) {
            return;
        }
        bullet->vf28(&shot, &data->m04, &data->m10, owner, &param, data->mFlags | 0x80);
        fn_8014F8F4()->mEnabled = 1;
    }
}

// inferred: character of the player of network id `aid` (slots in use only)
static inline CharaTask* PPCharaOfAid_unk(PPManager2_unk* mgr, const unsigned char& aid) {
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        if (mgr->mPlayers[i].mFlags.mActive == 1 && aid == mgr->mPlayers[i].mAid) {
            return mgr->mPlayers.begin()[i].mRef.mChara;
        }
    }
    return 0;
}
// inferred: character named by a packed id (1 none, 2 the player of aid m01, 3 the entry m01 of the list)
static inline CharaTask* PPCharaOfId_unk(PPManager2_unk* mgr, const PPId_unk* id) {
    switch (id->m00) {
    case 1:
        return 0;
    case 2:
        return PPCharaOfAid_unk(mgr, id->m01);
    case 3:
        return mgr->mCharas.size() > id->m01 ? mgr->mCharas[id->m01].mRef.mChara : 0;
    default:
        return 0;
    }
}
// inferred: target of a character that is not down (the previous one kept in mC8)
static inline void PPSetTarget_unk(CharaTask* chara, const PackedChara_unk* in) {
    if (chara != 0 && ((BattleCharaTask*)chara)->m1454 != 1) {
        chara->mC58.mC8 = (int)chara->mC58.mC4;
        chara->mC58.mC4 = PPCharaOfId_unk((PPManager2_unk*)lbl_8087F610, &in->m10);
    }
}
static inline void PPSetTargets_unk(CharaTask* chara, const PackedChara_unk* in) {
    if (FindByChara_unk((PPManager2_unk*)lbl_8087F610, chara) != 0) {
        PPSetTarget_unk(FindByChara_unk((PPManager2_unk*)lbl_8087F610, chara)->mRef.mChara, in);
    }
    if (fn_804EB814((PPManager2_unk*)lbl_8087F610, chara) != 0) {
        PPSetTarget_unk(fn_804EB814((PPManager2_unk*)lbl_8087F610, chara)->mRef.mChara, in);
    }
}
// inferred: payload of the answers 0x1035 (9 bytes): the sequence byte of the request and, by message, the gimmick
// taken (answer to a message 6, -1 when it was taken already), the team of the slot (message 0x35) or the time of
// the game; not padded on the network; names invented
struct PPAnswerGimmick_unk {
    int mId;
    int mKind;
};
#pragma pack(push, 1) // inferred: network payload without padding
struct PPMsgAnswer_unk {
    unsigned char m00;
    union {
        PPAnswerGimmick_unk mGimmick;
        unsigned int mTeam;
        long long mTime;
    };
};
#pragma pack(pop)
// inferred: message 6 (in the game): the message `mType` for the gimmick `mId`/`mKind`, from the character of the
// sender. On cannonballs (type 7) message 2 takes one (the server keeps the aid in m520), message 3 with -1
// drops it on the server (the others only forget it) and messages 3 and 5 let it go; the server answers a taken
// cannonball or drop item (message 0x1035, answered after the loop with m99B0)
extern "C" void fn_804EF0F0(unsigned int aid) {
    if (((PPManager2_unk*)lbl_8087F610)->mStateVar.mValue != 30) {
        return;
    }
    const PPMsgGimmick_unk* data = (const PPMsgGimmick_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick != 0; gimmick = gimmick->m5C) {
        if (data->mId != gimmick->mId || data->mKind != gimmick->m4C) {
            continue;
        }
        gimmick->mFlags &= ~4;
        fn_8014F8F4()->mEnabled = 0;
        GimmickMsg_unk msg(data->mType);
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        msg.m04 = data->m04;
        msg.m08 = data->m08;
        msg.m0C = 1;
        msg.mVec = data->mVec;
        CharaTask* chara;
        switch (data->mSender) {
        case 1:
            chara = 0;
            break;
        case 2:
            chara = PPCharaOfAid_unk(fn_800F7F90(), data->mSenderId);
            break;
        case 3:
            chara = mgr->mCharas.size() > data->mSenderId ? mgr->mCharas[data->mSenderId].mRef.mChara
                                                                     : 0;
            break;
        default:
            chara = 0;
            break;
        }
        msg.m10 = (int)chara;
        bool taken = false;
        if (gimmick->m50 == 7) {
            int forward = 1;
            if (msg.m08 <= 0) {
                gmk::Cannonball* cannon = dynamic_cast<gmk::Cannonball*>(gimmick);
                if (data->mType == 2) {
                    taken = true;
                    forward = msg.m08 != -1;
                    if (aid == lbl_8087F628->GetServerAid_unk() && cannon != 0) {
                        cannon->m520 = aid;
                    }
                } else if (data->mType == 3) {
                    if (msg.m08 == -1) {
                        forward = 0;
                        if (lbl_8087F628->GetMyAid_unk() == lbl_8087F628->GetServerAid_unk()) {
                            CharaTask* holder = (CharaTask*)msg.m10;
                            if (holder != 0 && holder->m1208 != 0) {
                                holder->m1208 = 0;
                            }
                            if (cannon != 0 && aid == cannon->m520) {
                                cannon->m520 = 0xFF;
                                msg.m08 = 0;
                                fn_8014F8F4()->mEnabled = 1;
                                gimmick->vf40(&msg);
                                fn_8014F8F4()->mEnabled = 0;
                                msg.m08 = -1;
                            }
                        }
                    } else {
                        CharaTask* holder = (CharaTask*)msg.m10;
                        if (holder != 0) {
                            holder->m1208 = 0;
                        }
                        if (cannon != 0) {
                            cannon->m520 = 0xFF;
                        }
                    }
                } else if (data->mType == 5) {
                    CharaTask* holder = (CharaTask*)msg.m10;
                    if (holder != 0) {
                        holder->m1208 = 0;
                    }
                    if (cannon != 0) {
                        cannon->m520 = 0xFF;
                    }
                }
            }
            if ((!taken || lbl_8087F628->GetMyAid_unk() != lbl_8087F628->GetServerAid_unk()) && forward != 0) {
                gimmick->vf40(&msg);
            }
        } else {
            gimmick->vf40(&msg);
        }
        if (IsServer_unk() == 1) {
            if (taken == true) {
                gmk::Cannonball* cannon = dynamic_cast<gmk::Cannonball*>(gimmick);
                if (cannon != 0) {
                    NetMsgI_unk<PPMsgAnswer_unk> answer(0x1035);
                    PPMsgAnswer_unk* out = &answer.mData;
                    out->m00 = data->m1E;
                    if (cannon->m520 == 0xFF) {
                        out->mGimmick.mId = data->mId;
                        out->mGimmick.mKind = data->mKind;
                        cannon->m520 = aid;
                        gimmick->vf40(&msg);
                    } else {
                        out->mGimmick.mId = -1;
                        out->mGimmick.mKind = -1;
                    }
                    fn_804AE3BC()->fn_8050E094(aid, 0x1035, out, 1);
                    ((PPManager2_unk*)lbl_8087F610)->m99B0 = 1;
                    ((PPManager2_unk*)lbl_8087F610)->m99B1 = aid;
                    ((PPManager2_unk*)lbl_8087F610)->m99B4 = 1;
                }
            } else if (gimmick->m50 == 18 && data->mType == 5) {
                gmk::DropItem* drop = dynamic_cast<gmk::DropItem*>(gimmick);
                if (drop != 0) {
                    NetMsgI_unk<PPMsgAnswer_unk> answer(0x1035);
                    PPMsgAnswer_unk* out = &answer.mData;
                    out->m00 = data->m1E;
                    if (drop->m29C == 0xFF) {
                        out->mGimmick.mId = data->mId;
                        out->mGimmick.mKind = data->mKind;
                        drop->m29C = aid;
                    } else {
                        out->mGimmick.mId = -1;
                        out->mGimmick.mKind = -1;
                    }
                    fn_804AE3BC()->fn_8050E094(aid, 0x1035, out, 1);
                    ((PPManager2_unk*)lbl_8087F610)->m99B0 = 1;
                    ((PPManager2_unk*)lbl_8087F610)->m99B1 = aid;
                    ((PPManager2_unk*)lbl_8087F610)->m99B4 = 1;
                }
            }
        }
        fn_8014F8F4()->mEnabled = 1;
        return;
    }
}



// inferred: message 7: hit received by a character (only in the game, state 30); the listeners are off during the
// hit; a gimmick held by the character and released by the hit gets message 3
extern "C" void fn_804EF9B4() {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        GimmickCommon* held;
        unsigned int flags;
        const PPMsgDamage_unk* data = (const PPMsgDamage_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        CharaTask* chara = GetHitAttacker_unk((PPManager2_unk*)lbl_8087F610, &data->mTarget);
        if (chara != 0) {
            flags = data->mFlags;
            held = chara->m1208;
            if (chara != 0 && chara->IsPlayer()) {
                if ((flags & 8) != 8 && chara->IsFlag5_unk()) {
                    if (!chara->IsDown_unk() && (flags & 0x400000) != 0x400000) {
                        flags |= 8;
                    }
                }
                if ((flags & 0x400) != 0x400 && chara->m55C == 6 && (unsigned int)(chara->m560 - 1) <= 1) {
                    flags |= 0x400;
                }
            }
            fn_8014F8F4()->mEnabled = 0;
            chara->fn_8015076C(data->mHitId, data->m00, data->m0C, data->m20, flags, 0);
            fn_8014F8F4()->mEnabled = 1;
            if (held != 0 && chara->m1208 == 0) {
                GimmickMsg_unk msg;
                msg.mType = 3;
                msg.m10 = (int)chara;
                held->vf40(&msg);
            }
        }
    }
}

// inferred: messages 8 and 64: a hit dealt by another player (only in the game, state 30); the listeners of the
// characters are off during the hit
extern "C" void fn_804EFC94() {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPMsgHit_unk* data = (const PPMsgHit_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        CharaTask* attacker = GetHitAttacker_unk(mgr, &data->mAttacker);
        if (lbl_8087F048 != 0) {
            HitInfo_unk* hit = fn_80219E6C(data->mHitId);
            if (hit != 0) {
                fn_8014F8F4()->mEnabled = 0;
                lbl_8087F048->fn_800FB1BC(attacker, hit, &data->m04, &data->m10, 0, data->mFlag, 1.0f);
                fn_8014F8F4()->mEnabled = 1;
            }
        }
    }
}

// inferred: message 9: a voice of the data (id, offset)
extern "C" void fn_804EFE98(PPPlayer_unk* player) {
    if (lbl_8087F628 != 0 && fn_804AE3BC() != 0 && fn_804AE3BC()->GetWorkBuffer_unk() != 0) {
        const unsigned int* data = (const unsigned int*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (lbl_8087F498 != 0) {
            lbl_8087F498->fn_803E9B7C(data[0], data[1], 1, 1.0f);
        }
    }
}

extern "C" int fn_8021A77C(HitInfo_unk* data);
extern "C" bool fn_8021A8D0(HitInfo_unk* data);
extern "C" __declspec(noinline) int fn_804F1060();
#include "game/HitUtil_unk.h"
// NONMATCHING: 99.08% - stack slots of the matrices and vectors, the status effects added through the status
// (base 0x7D4 in the original) and reloads of lbl_8087F610 in the inlined searches
// inferred: messages 10 and 11 (only in the game, state 30): a hit of a character on another one. The hate of the
// target is updated, the status effects of the attacker are replaced by the ones of the message and the listeners
// are off meanwhile; message 10 runs the damage of the hit data (a guarded hit, the heal of the status 0x23 of the
// attacker in game mode 2, the hit effect and sound, the knock back of the condition 0x4000 and the damage
// popup), message 11 the damage of an item. A character left in state 0x20 falls; when the target is a player of
// the own side (or a used character entry) and the hit knocked it down, fn_804E8BF4 counts it and message 0x1011
// tells the machine of the attacker
extern "C" void fn_804EFF74(PPPlayer_unk* player, unsigned int type) {
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 10) {
        return;
    }
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    const PPMsgAttack_unk* data = (const PPMsgAttack_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    CharaTask* attacker = GetHitChara_unk((PPManager2_unk*)lbl_8087F610, &data->mAttacker);
    CharaTask* target = GetHitChara_unk((PPManager2_unk*)lbl_8087F610, &data->mTarget);
    if (target == 0) {
        return;
    }
    if (lbl_8087F048 != 0 && fn_8021A684(fn_80219E6C(data->mHitId))) {
        fn_8013A194()->fn_80102EAC(target, attacker, 90, -1);
        fn_8013A194()->fn_801031D0(attacker, target, 0);
    }
    fn_8014F8F4()->mEnabled = 0;
    if (attacker != 0) {
        attacker->mStatus.mEntries.mCount = 0;
        for (unsigned int i = 0; i < fn_804F1060(); i++) {
            if (data->mEntries[i].mId != 0) {
                PPAddEntry_unk(attacker->GetStatus_unk(), data->mEntries[i]);
                fn_8013A194()->fn_80109864((SpEffect_unk*)&data->mEntries[i], attacker);
            }
        }
    }
    DamageResult_unk result;
    if (type == 10) {
        HitInfo_unk* hit = fn_80219E6C(data->mHitId);
        if (hit != 0) {
            unsigned int flags = data->mFlags;
            if (target != 0 && target->IsPlayer()) {
                if ((flags & 8) != 8 && target->mFlag5) {
                    if (!target->IsDown_unk() && (flags & 0x400000) != 0x400000) {
                        flags |= 8;
                    }
                }
                if ((flags & 0x400) != 0x400 && target->m55C == 6 && !(target->m560 != 2 && target->m560 != 1)) {
                    flags |= 0x400;
                }
            }
            if ((hit->mAC & 0x8000) == 0x8000 && (target->m54C & 0x1000000) != 0x1000000) {
                if (target->m1208 != 0) {
                    fn_8014F8F4()->mEnabled = 1;
                    target->fn_801750FC();
                    fn_8014F8F4()->mEnabled = 0;
                }
                target->fn_8016D0A0(1);
            }
            int apply = 1;
            if (hit->m04 != 0x147) {
                if (attacker != 0) {
                    target->fn_80150460(attacker, data->mCombo);
                }
                result.m10 = target->mFE4;
                DamageParam_unk param;
                param.mCombo = target->mFE4;
                param.mRate = data->mRate;
                if (target->mStatus.IsState_unk(0x20) && hit->m04 != 0x2711) {
                    apply = 0;
                }
                if ((target->mStatus.m14 & 1u) == 1 && !fn_8021A8D0(hit) && (hit->m04 == 0x142 || hit->m04 == 0x131)) {
                    apply = 0;
                }
                if (apply) {
                    fn_8003EA3C(&result, hit, attacker, target, flags, &param, 0, 0);
                    if (((PPManager2_unk*)lbl_8087F610)->mMode == 2 && attacker != 0 &&
                        attacker->mStatus.fn_80133EE8(0x23) && !target->mStatus.IsState_unk(0x20)) {
                        DamageResult_unk heal;
                        heal.mType = 1;
                        int value = attacker->mStatus.fn_80133F18(0x23, -1);
                        float rate = value > 0 ? value / 100.0f : 2.0f;
                        heal.mDamage -= (int)(result.mDamage * rate);
                        target->mStatus.fn_8012DF7C(heal.mDamage, 0, 0);
                        atn::Vector3 pos = target->mPos;
                        pos += atn::Vector3(0.0f, 10.0f, 0.0f);
                        fn_8013A194()->fn_80108C10(&heal, pos, target, target, 0, 0);
                    }
                }
                if (attacker != 0 && fn_8021A77C(hit)) {
                    BattleCalcParam_unk calc;
                    atn::Vector3 dir = attacker->mPos - HitCenter_unk(target);
                    if (!((float)__fabs(PSVECSquareMag(&dir)) < 1.1920929e-7f)) {
                        PSVECNormalize(&dir, &dir);
                        dir *= target->m5B0;
                    } else {
                        float radius = target->m5B0;
                        dir.x = 0.0f;
                        dir.y = 0.0f;
                        dir.z = radius;
                        atn::Matrix3x4 m;
                        MakeRotZYX_unk(&m, target->mRot);
                        PSMTXMultVec(m.m, &dir, &dir);
                    }
                    calc.m1C = dir + HitCenter_unk(target);
                    atn::Vector3 n = dir;
                    PSVECNormalize(&n, &n);
                    calc.m28 = atn::Vector3(-n.x, -n.y, -n.z);
                    calc.m00 = attacker;
                    calc.m04 = target;
                    calc.m08 = hit;
                    calc.m44 = 0;
                    calc.m0C = flags;
                    calc.m94 = flags;
                    if ((flags & 8) == 8) {
                        calc.m84 = 1;
                    } else {
                        calc.m84 = 0;
                    }
                    if (lbl_8087F048 != 0) {
                        fn_8013A194()->fn_801018E4(&calc);
                        fn_8013A194()->fn_80101CDC(&calc);
                    }
                    if (result.mDamage > 0) {
                        target->mC4C = 0;
                    }
                }
            }
            if ((hit->m90.mFlags & 0x4000u) == 0x4000u && target->mStatus.IsState_unk(0x4000)) {
                atn::Vector3 front(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', target->mRot.y);
                PSMTXMultVec(m, &front, &front);
                target->fn_801681B0(atn::Vector3(-front.x, -front.y, -front.z), 0, (hit->mAC >> 9) & 1);
            }
            if (apply && lbl_8087F048 != 0 && (hit->mAC & 0x80000) != 0x80000 && hit->m01 != 6 && hit->m01 != 5 &&
                hit->m01 != 3 && hit->m04 != 0x1774 && hit->m04 != 0x4FCC) {
                fn_8013A194()->fn_80108C10(&result, target->vfAC(), attacker, 0, flags, 0);
            }
        }
    } else if (type == 11) {
        HitInfo_unk* item = (HitInfo_unk*)fn_80211480(data->mHitId);
        if (item != 0) {
            fn_8003F1E4(&result, item, attacker, target, data->mFlags);
        }
    }
    fn_8014F8F4()->mEnabled = 1;
    if (target->mStatus.IsState_unk(0x20)) {
        if (target->m55C != 6 || (target->m560 != 0x17 && target->m560 != 0x3B)) {
            target->fn_8015E7A0(atn::Vector3(0.0f, 0.1f, 0.0f), -1, 0);
        }
        target->vf44();
    }
    PPPlayer_unk* victim = FindByChara_unk((PPManager2_unk*)lbl_8087F610, target);
    int index = FindCharaIndex_unk((PPManager2_unk*)lbl_8087F610, target);
    PPCharaEntry_unk* entry = index >= 0 ? &((PPManager2_unk*)lbl_8087F610)->mCharas[index] : 0;
    if ((victim != 0 && (victim->mFlags.m30 == 1 || IsSameTeam_unk(victim, GetMyPlayer_unk()))) ||
        (entry != 0 && entry->mActive == 1)) {
        if (result.mFlag31) {
            fn_804E8BF4((PPManager2_unk*)lbl_8087F610, attacker, target);
            if (FindByChara_unk((PPManager2_unk*)lbl_8087F610, attacker) != 0 &&
                FindByChara_unk((PPManager2_unk*)lbl_8087F610, target) != 0) {
                NetMsgI_unk<PPId_unk> msg(0x1011);
                PPId_unk* ids = &msg.mData;
                ids->m00 = data->mAttacker.mIndex;
                ids->m01 = data->mTarget.mIndex;
                fn_804AE3BC()->fn_8050E094(data->mAttacker.mIndex & 0xF, 0x1011, ids, 1);
            }
        }
    }
}

// inferred: number of status effects of message 10 (called by fn_804EFF74 in every iteration)
extern "C" __declspec(noinline) int fn_804F1060() {
    return 6;
}

// inferred: messages 0x0C and 0x0E: values kept by the manager
extern "C" void fn_804F1068() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const int* data = (const int*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m55C = data[0];
}

// NONMATCHING: 99.26% - r3/r4/r5 of the network manager, its flags and bit 0 rotated
// inferred: message 0x0D: value m564; in game mode 2 it plays a BGM and sets the flags of the network
// manager (bit 0, then bit 1) and a flag of the game (0x1B0)
extern "C" void fn_804F1124() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const int* data = (const int*)fn_804AE3BC()->GetWorkBuffer_unk();
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
            lbl_8087F430->fn_803750E4(0x1B0);
            SetConfig::sInstance->mShowTutorial = 0;
        }
    }
}

extern "C" void fn_804F12A0() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const int* data = (const int*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m568 = data[0];
}


extern "C" void fn_804F135C() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const PPGameSettings_unk* data = (const PPGameSettings_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m560 = data->m00;
    ((PPManager2_unk*)lbl_8087F610)->m564 = data->m04;
    ((PPManager2_unk*)lbl_8087F610)->m568 = data->m08;
    memcpy(((PPManager2_unk*)lbl_8087F610)->m56C.m, data->m0C, sizeof(data->m0C));
    memcpy(((PPManager2_unk*)lbl_8087F610)->m58C.m, data->m2C, sizeof(data->m2C));
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
    const unsigned int* data = (const unsigned int*)fn_804AE3BC()->GetWorkBuffer_unk();
    player->mFlags.m22 = data[0];
}

// inferred: the room is the one of the server (`aid`)
static inline int IsServerAid_unk(unsigned char aid) {
    return aid == lbl_8087F628->GetServerAid_unk();
}

// inferred: payload of the messages 0x13 and 0x2C (the values and flags of a slot; 0x2C also has the teams and
// stages of the slots); names invented
struct PPMsgSlot_unk {
    int m00;
    PPWord2_unk m04;
    int m0C;
    PPWord2_unk m10;
    int m18;
    PPPlayerFlags_unk mFlags;
    unsigned char mTeams[8];
    signed char mStages[8];
};

// NONMATCHING: 98.52% - registers of the inlined searches and loop; the call in fn_804EE040 has to pass the
// type, which moves its registers (99.83 -> 99.49%)
// inferred: messages 0x13 and 0x2C (`type`) from a player: flags of its slot (its values too when it is the
// server); 0x2C also sets the teams and stages of the slots when this machine has a slot, 0x13 marks the player
// in the own slot (bit of its aid in mD53); not while m999C is set
extern "C" void fn_804F1564(PPPlayer_unk* player, unsigned int type) {
    if (fn_800F7F90()->m999C == 1) {
        return;
    }
    if (PP_NO_DATA_UNK) {
        return;
    }
    const PPMsgSlot_unk* data = (const PPMsgSlot_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    player->mFlags.m28 = data->mFlags.m28;
    player->mFlags.m06 = data->mFlags.m06;
    player->mFlags.m02 = data->mFlags.m02;
    player->mFlags.m01 = data->mFlags.m01;
    if (IsServerAid_unk(player->mAid) == 1) {
        player->m0B0[0] = data->m00;
        *(PPWord2_unk*)&player->m0B0[1] = data->m04;
        player->m0B0[3] = data->m0C;
        *(PPWord2_unk*)&player->m0B0[4] = data->m10;
        player->mC8 = data->m18;
        player->mFlags.m22 = data->mFlags.m22;
        player->mFlags.m10 = data->mFlags.m10;
    }
    if (type == 0x2C) {
        PPMsgSlot_unk* slots = (PPMsgSlot_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        PPManager2_unk* mgr = fn_800F7F90();
        if (fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) != 0) {
            for (int i = 0; i < (int)fn_800F7F90()->mPlayers.size(); i++) {
                PPPlayer_unk* slot = fn_804D5F0C(fn_800F7F90(), i);
                if (slot != 0) {
                    slot->mFlags.m22 = slots->mTeams[i];
                    slot->mB0 = slots->mStages[i];
                }
            }
        }
    } else {
        PPManager2_unk* mgr = fn_800F7F90();
        PPPlayer_unk* own = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk());
        if (own != 0) {
            own->mD53 |= 1 << player->mAid;
        }
    }
}

// inferred: message 0x1C: one of the 4 equipments of the player
extern "C" void fn_804F1950(PPPlayer_unk* player) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const PPMsgEquip_unk* data = (const PPMsgEquip_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    switch (data->mKind) {
    case 0:
        player->mEquip0.Set_unk(data->mHead);
        break;
    case 1:
        player->mEquip1.Set_unk(data->mHead);
        break;
    case 2:
        player->mEquip2.Set_unk(data->mHead);
        break;
    case 3:
        player->mEquip3.Set_unk(data->mHead);
        break;
    }
}

// inferred: message 0x15: a value of the player slot
extern "C" void fn_804F1CA0(PPPlayer_unk* player) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const int* data = (const int*)fn_804AE3BC()->GetWorkBuffer_unk();
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
static inline int IsSameTeam_unk(PPPlayer_unk* player, PPPlayer_unk* leader) {
    if (leader == 0 || player == 0 || !IsTeamPlayer_unk(player)) {
        return 0;
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
    for (i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* other = &mgr->mPlayers[i];
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

#include "game/SpecialEnemyBase.h"

// NONMATCHING: 99.63% - r3/r5 of the inlined FindTeam_unk (as fn_804F2258)
// inferred: state of a character received from the server (only in the game, state 30): the character of a team
// player (not the own one), or of the character list (index in the data); with `type` 26 the special enemies also
// restore their state (vf114) from +0x15 of the data
extern "C" void fn_804F2040(unsigned int type) {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPMsgSpecialFull_unk* data = (const PPMsgSpecialFull_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (data->mChara.m12_1 == 0) {
            PPPlayer_unk* player = PPFindTeam_unk(&((PPManager2_unk*)lbl_8087F610)->mPlayers, data->mIndex);
            fn_804AE3BC()->fn_8050E62C(player != 0, "enemy_status\n");
            if (data->mIndex != lbl_8087F628->GetMyAid_unk() && player->mRef.mChara != 0) {
                fn_804CF640(&data->mChara, player->mRef.mChara);
            }
        } else {
            CharaTask* chara = ((PPManager2_unk*)lbl_8087F610)->mCharas[data->mIndex].mRef.mChara;
            if (chara != 0) {
                fn_804CF640(&data->mChara, chara);
                if (type == 26) {
                    switch (((BattleCharaTask*)chara)->m146C) {
                    case 19:
                    case 21:
                    case 22:
                    case 23:
                    case 25:
                    case 32:
                    case 33:
                    case 35:
                    case 36:
                    case 40:
                        ((SpecialEnemyBase*)chara)
                            ->vf114(&((PPMsgSpecialFull_unk*)fn_804AE3BC()->GetWorkBuffer_unk())->mState);
                        break;
                    }
                }
            }
        }
    }
}

// NONMATCHING: 99.72% - r3/r5 of the inlined search
// inferred: state of a player slot (network id mAid) or of a character of the list (index mAid) received from the
// server; a character of the list is revived or killed to follow the value m00
extern "C" void fn_804F2258() {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPPlayerInfo_unk* data = (const PPPlayerInfo_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (!data->mIsEntry) {
            PPPlayer_unk* player = PPFindTeam_unk(&((PPManager2_unk*)lbl_8087F610)->mPlayers, data->mAid);
            fn_804AE3BC()->fn_8050E62C(player != 0, "enemy_sync\n");
            if (player->mRef.mChara != 0) {
                ((PPChara_unk*)player->mRef.mChara)->m7D8 = data->m00;
            }
            player->mDC.fn_80501288((CharaTask*)data->mChara);
            player->mE0 = data->m06;
            player->mE4 = data->m07;
            player->mFlags.m13 = data->m13;
            player->mFlags.m12 = data->m12;
        } else {
            PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
            PPCharaEntry_unk* entry = &mgr->mCharas[data->mAid];
            CharaTask* chara = entry->mRef.mChara;
            if (chara != 0) {
                mgr->m99BC = 1;
                if (chara->mStatus.IsState_unk(0x20) && data->m00 > 0) {
                    entry->mRef.fn_805011CC();
                } else if (!chara->mStatus.IsState_unk(0x20) && data->m00 <= 0) {
                    entry->mRef.fn_805010C0();
                }
                ((PPManager2_unk*)lbl_8087F610)->m99BC = 0;
                ((PPChara_unk*)chara)->m7D8 = data->m00;
            }
        }
    }
}

// inferred: data of the messages of the location of an enemy (state of the character, index and motion); names
// invented
struct PPMsgEnemyLoc_unk {
    PPMsgCharaState_unk mState; // 0x00
    int mIndex;                 // 0x2C: network id of a team player, or index of the character list
    unsigned char mIsEntry;     // 0x30
    unsigned char mMotion[1];   // 0x31: packed motion (fn_804CF198)
};

// NONMATCHING: 99.16% - the original copies mStateVar twice for the vararg (a temporary, then the argument) and
// reloads lbl_8087F610 after the inlined FindTeam_unk; the address registers of three copies (as fn_804EE4F0)
// inferred: location of an enemy received from the server (only in the game, state 30): the character of a team
// player through fn_804EE4F0, or a character of the list directly; with `type` 29 also its motion (the motion
// commands of a new motion run once at frame 0)
extern "C" void fn_804F2450(unsigned int type) {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPMsgEnemyLoc_unk* data = (const PPMsgEnemyLoc_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        if (!data->mIsEntry) {
            PPPlayer_unk* player = PPFindTeam_unk(&((PPManager2_unk*)lbl_8087F610)->mPlayers, (unsigned char)data->mIndex);
            PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
            fn_804AE3BC()->fn_8050E62C(player != 0, "enemy_update_loc(%x)(%x)(%x)\n", data->mIndex, mgr->m5C0,
                                       mgr->mStateVar);
            if (fn_804EE4F0(player, fn_804AE3BC()->GetWorkBuffer_unk()) == 1) {
                fn_804CF198((PackedMotion_unk*)((PPMsgEnemyLoc_unk*)fn_804AE3BC()->GetWorkBuffer_unk())->mMotion,
                            player->mRef.mChara->GetModel_unk(), 0);
                ((PPChara_unk*)player->mRef.mChara)->m3FC = 1;
            }
        } else {
            CharaTask* chara = ((PPManager2_unk*)lbl_8087F610)->mCharas[data->mIndex].mRef.mChara;
            if (chara != 0) {
                chara->mPos = data->mState.mPos;
                atn::Vector3 vel = data->mState.mVel;
                if (vel.LengthSq_unk() > 900.0f) {
                    chara->m13D8.Assign_unk(lbl_807C7030);
                } else {
                    chara->m13D8 = vel;
                }
                chara->mRot = atn::Vector3(0.0f, data->mState.mRotY, 0.0f);
                chara->m13E4 = data->mState.mTurn;
                chara->m13E8.Assign_unk(PPUnpackVec_unk(data->mState.mNode));
                atn::Vector3 dir = PPUnpackVec_unk(data->mState.mDir);
                if (PPDot_unk(dir, chara->mFA4) > 0.70710677f) {
                    atn::VecAssign_unk(dir, PPHalfway_unk(chara->mFA4, dir));
                }
                chara->mFA4 = dir;
                chara->mFlag1 = 1;
                if (type == 29) {
                    fn_804CF198((PackedMotion_unk*)((PPMsgEnemyLoc_unk*)fn_804AE3BC()->GetWorkBuffer_unk())->mMotion,
                                chara->GetModel_unk(), 0);
                    ((PPChara_unk*)chara)->m3FC = 1;
                    if (chara->mMotion.mTracks[0].m18 == 0 && chara->mMotion.mTracks[0].m00 != chara->m1220.m04 &&
                        chara->mMotion.mTracks[0].m08 > 0.0f) {
                        float frame = chara->mMotion.mTracks[0].m08;
                        float speed = chara->mMotion.mTracks[0].m0C;
                        chara->mMotion.mTracks[0].m08 = 0.0f;
                        chara->mMotion.mTracks[0].m0C = frame;
                        chara->m1220.fn_8012111C();
                        chara->mMotion.mTracks[0].m08 = frame;
                        chara->mMotion.mTracks[0].m0C = speed;
                    }
                }
            }
        }
    }
}

// inferred: data of the message of a hit on a special enemy (attacker and target as kind/index pairs); names
// invented
struct PPMsgSpecialHit_unk {
    int mHitId;
    int m04;
    union {
        struct {
            unsigned int m44 : 4;
            unsigned int m3C : 4;
            unsigned int m40 : 8;
            unsigned int m48 : 1;
        } mBits;
        struct {
            unsigned char m08[3];
            PPMsgCharaRef_unk mAttacker; // 0x0B
            PPMsgCharaRef_unk mTarget;   // 0x0D
        } mRefs;
    };
};

// NONMATCHING: 98.07% - registers of the inlined searches: the original reloads lbl_8087F610 for the second one
// inferred: a hit on a special enemy dealt by another player (only in the game, state 30): the hit is rebuilt in
// a BattleCalcParam_unk and given to vf40 of the enemy (SpecialEnemyBase ids, unless its flag 2 is set)
extern "C" void fn_804F2994() {
    if (((PPManager2_unk*)lbl_8087F610)->mState == 30) {
        const PPMsgSpecialHit_unk* data = (const PPMsgSpecialHit_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
        CharaTask* attacker = GetHitAttacker_unk((PPManager2_unk*)lbl_8087F610, &data->mRefs.mAttacker);
        CharaTask* target = GetHitAttacker_unk((PPManager2_unk*)lbl_8087F610, &data->mRefs.mTarget);
        if (target != 0 && !target->IsFlag2_unk()) {
            switch (((BattleCharaTask*)target)->m146C) {
            case 19:
            case 21:
            case 22:
            case 23:
            case 25:
            case 32:
            case 33:
            case 35:
            case 36:
            case 40: {
                BattleCalcParam_unk param;
                param.m08 = fn_80219E6C(data->mHitId);
                param.m00 = attacker;
                param.m04 = target;
                param.m94 = data->m04;
                param.m44 = data->mBits.m44;
                param.m3C = data->mBits.m3C;
                param.m40 = data->mBits.m40;
                param.m48 = data->mBits.m48;
                // inferred: vf40 is declared with HitInfo_unk* (include/game/CharaTask.h); here it gets the hit
                target->vf40((HitInfo_unk*)&param);
                break;
            }
            }
        }
    }
}

// inferred: empty in the original (the hits of other players reach the special enemies by fn_804F2994)
void SpecialEnemyBase::vf40(HitInfo_unk*) {}

// inferred: message 0x1F: shield parts of the character of the player (count in the data)
extern "C" void fn_804F2C44(PPPlayer_unk* player) {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    const int* data = (const int*)fn_804AE3BC()->GetWorkBuffer_unk();
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
    const unsigned char* data = (const unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    memcpy(((PPManager2_unk*)lbl_8087F610)->m9984, data, 8);
    memcpy(((PPManager2_unk*)lbl_8087F610)->m998C, data + 8, 8);
}

// inferred: queue of 32 requests at 0x48 (0x18 bytes each); names invented
struct PPRequest_unk {
    int mUsed;
    int m04;
    int m08;
    unsigned int m0C[2]; // inferred: OSGetTime when the request was sent (long long, 4-byte aligned)
    unsigned char m14;
    unsigned char m15;
    unsigned char m16[0x18 - 0x16];
};
struct PPRequests_unk {
    unsigned char m00[0x48];
    PPRequest_unk mRequests[32];
};
// inferred: request of id `id` (m15)
static inline PPRequest_unk* FindRequest_unk(PPRequests_unk* queue, unsigned int id) {
    for (int i = 0; i < 32; i++) {
        if (queue->mRequests[i].mUsed != 0 && id == queue->mRequests[i].m15) {
            return &queue->mRequests[i];
        }
    }
    return 0;
}

// NONMATCHING: 99.57% - the work buffer and the queue swap r6/r7
// inferred: message 0x29: answer of a request of this machine (its 8 bytes are kept in the request)
extern "C" void fn_804F3258() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const unsigned char* data = (const unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    PPRequests_unk* queue = (PPRequests_unk*)lbl_8087F610;
    PPRequest_unk* req = FindRequest_unk(queue, data[0]);
    if (req != 0) {
        req->m04 = 1;
        req->m08 = 300;
        memcpy(req->m0C, data + 1, 8);
    }
}

// inferred: sets the team of a slot; the team when it changed, else 0 (name invented)
static inline unsigned int SetTeam_unk(PPPlayer_unk* player, unsigned int team) {
    if (player == 0) {
        return 0;
    }
    if (player->mFlags.m22 == team) {
        return 0;
    }
    player->mFlags.m22 = team;
    return team;
}

// inferred: message 0x35 received by the server from `aid` with the player slot: the first byte goes back in a
// message 0x1035 with the second one (the team of the slot, sent back when it changed); the manager answers it after the loop
extern "C" void fn_804F34B4(PPPlayer_unk* player, unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    if (IsServer_unk()) {
        unsigned char* in = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
        NetMsgI_unk<PPMsgAnswer_unk> msg(0x1035);
        PPMsgAnswer_unk* data = &msg.mData;
        data->m00 = in[0];
        data->mTeam = SetTeam_unk(player, in[1]);
        fn_804AE3BC()->fn_8050E094(aid, 0x1035, data, 1);
        ((PPManager2_unk*)lbl_8087F610)->m99B0 = 1;
        ((PPManager2_unk*)lbl_8087F610)->m99B1 = aid;
        ((PPManager2_unk*)lbl_8087F610)->m99B4 = 1;
    }
}

extern "C" void fn_804F36F4(PPPlayer_unk* player, unsigned int aid) {}

// inferred: message 0x2D (on the server, unless m999C): value of the slot (mB0 and mD8)
extern "C" void fn_804F36F8(PPPlayer_unk* player) {
    if (((PPManager2_unk*)lbl_8087F610)->m999C == 1) {
        return;
    }
    if (PP_NO_DATA_UNK) {
        return;
    }
    if (lbl_8087F628->GetMyAid_unk() == lbl_8087F628->GetServerAid_unk()) {
        signed char* data = (signed char*)fn_804AE3BC()->GetWorkBuffer_unk();
        player->mB0 = data[0];
        player->mD8 = data[0];
    }
}

// inferred: message 0x2E: hit (or end of the hit) of skill 0x4FC1 on the character of a player
extern "C" void fn_804F3880() {
    if (fn_800F7F90()->mState != 30) {
        return;
    }
    const unsigned char* data = (const unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    PPPlayer_unk* player = fn_804DD158(fn_800F7F90(), data[0]);
    unsigned char on = data[1];
    if (player != 0 && player->mRef.mChara != 0) {
        if (on != 0) {
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
    if (fn_800F7F90()->mState != 30) {
        return;
    }
    const unsigned char* data = (const unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    PPPlayer_unk* player = fn_804DD158(fn_800F7F90(), data[0]);
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
    const unsigned char* data = (const unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    ((PPManager2_unk*)lbl_8087F610)->m5A8 = data[0];
}

extern "C" void fn_804EAA84(PPManager2_unk* mgr, CharaTask* chara, int a, int b);

// NONMATCHING: 96.78% - here the manager pointer is kept in r29 across the loop (the original loads it again
// in every iteration)
// inferred: message 0x31: fn_804EAA84 on the characters of the players marked m30 and of the own team
extern "C" void fn_804F3BAC() {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    for (int i = 0; i < (int)((PPManager2_unk*)lbl_8087F610)->mPlayers.size(); i++) {
        PPPlayer_unk* player = fn_804D5F0C((PPManager2_unk*)lbl_8087F610, i);
        if (player->mFlags.mActive && player->mRef.mChara != 0) {
            if (player->mFlags.m30 == 1 || IsSameTeam_unk(player, GetMyPlayer_unk())) {
                fn_804EAA84((PPManager2_unk*)lbl_8087F610, player->mRef.mChara, 1, 0);
            }
        }
    }
}


// inferred: message 0x32: two halfwords handled by the manager (fn_804F50DC)
extern "C" void fn_804F3D80(PPPlayer_unk* player) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const unsigned short* data = (const unsigned short*)fn_804AE3BC()->GetWorkBuffer_unk();
    fn_804F50DC((PPManager2_unk*)lbl_8087F610, player, data[0], data[1]);
}

// inferred: message 0x33: releases the character of entry `index` of the list
extern "C" void fn_804F3E50() {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    const int* data = (const int*)fn_804AE3BC()->GetWorkBuffer_unk();
    int index = data[0];
    if (index < 0 || (int)((PPManager2_unk*)lbl_8087F610)->mCharas.size() <= index) {
        return;
    }
    PPCharaEntry_unk* entry = &((PPManager2_unk*)lbl_8087F610)->mCharas[index];
    if (entry->mRef.mChara != 0) {
        entry->mRef.fn_805011CC();
    }
}

// inferred: players in use with a character in the team of `chara` (1 in game mode 0, 0 without a slot)
static inline int PPTeamSize_unk(PPManager2_unk* mgr, CharaTask* chara) {
    if (mgr->mMode == 0) {
        return 1;
    }
    PPPlayer_unk* player = FindByChara_unk(mgr, chara);
    if (player == 0) {
        return 0;
    }
    int n = 0;
    unsigned int team = player->mFlags.m22;
    for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
        PPPlayer_unk* other = &mgr->mPlayers[i];
        if (other->mFlags.mActive && other->mRef.mChara != 0 && other->mFlags.m22 == team) {
            n++;
        }
    }
    return n;
}
// NONMATCHING: 93.87% - the original does not fold the halving of the constant 10 (li 10; srawi) and reloads
// lbl_8087F610 and m564
// inferred: message 0x34: the player `data[0]` defeated the player `data[1]` (not of its side): 10 points (5 for a
// team player of a machine, doubled in the last minute) and 5 more for each player that the team of the victim
// has more; the score of the winner changes (fn_804DB1C4, inlined), m0E0 counts its defeats, and the own player
// says the quick message 3
extern "C" void fn_804F3EEC() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const unsigned char* data = (const unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    PPPlayer_unk* winner = PPFindSlot_unk(fn_800F7F90(), data[0]);
    PPPlayer_unk* loser = PPFindSlot_unk(fn_800F7F90(), data[1]);
    if (winner == 0 || loser == 0 || winner->mRef.mChara == 0 || loser->mRef.mChara == 0) {
        return;
    }
    if (winner->mRef.mChara->fn_8016F3D0(loser->mRef.mChara, 1)) {
        return;
    }
    int points = 10;
    if (IsTeamPlayer_unk(loser)) {
        points >>= 1;
    }
    PPManager2_unk* mgr = fn_800F7F90();
    bool last = !(GetRemain_unk(mgr) < 0);
    if (last) {
        last = GetRemain_unk(mgr) < 60;
    }
    if (last) {
        points <<= 1;
    }
    int more = PPTeamSize_unk(fn_800F7F90(), loser->mRef.mChara) -
                   PPTeamSize_unk(fn_800F7F90(), winner->mRef.mChara) >
                       0
                   ? PPTeamSize_unk(fn_800F7F90(), loser->mRef.mChara) -
                         PPTeamSize_unk(fn_800F7F90(), winner->mRef.mChara)
                   : 0;
    int old = winner->mDCScore;
    fn_804DB1C4(fn_800F7F90(), winner->mRef.mChara, points + more * 5);
    if (old < winner->mDCScore) {
        winner->mE0++;
    }
    if (winner == GetMyPlayer_unk()) {
        fn_804E82C4(fn_800F7F90(), ((signed char*)&lbl_8087F628->mFriendList.m7F0[0xB8])[3]);
    }
}

// inferred: message 0x35: a value per network id
extern "C" void fn_804F4728() {
    if (PP_NO_DATA_UNK) {
        return;
    }
    const unsigned int* data = (const unsigned int*)fn_804AE3BC()->GetWorkBuffer_unk();
    if (data[0] <= 7) {
        PPManager2_unk* mgr = (PPManager2_unk*)lbl_8087F610;
        mgr->m9694[data[0]] = data[1];
    }
}

// NONMATCHING: 97.97% - `player`/`aid` in r30/r29 (r29/r30 in the original) and the words at +0x42 through a
// pointer that advances by 4 (packed payload, as fn_804D824C)
// inferred: message 0x1040 (fn_804D824C) from `aid`: the settings of the room, the flags of the sender's slot
// (its values too when it is the server), the teams and stages of the slots when this machine has a slot; mode 0
// is answered with its sequence number (message 0x1035, m99B0), mode 1 marks the server as ready (m9A74[0])
extern "C" void fn_804F47FC(PPPlayer_unk* player, unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    if (fn_800F7F90()->mState == 30 && fn_800F7F90()->m50C != 0) {
        return;
    }
    const PPMsgRoom_unk* data = (const PPMsgRoom_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    for (int i = 0; i < (int)fn_800F7F90()->mPlayers.size(); i++) {
        PPPlayer_unk* slot = fn_804D5F0C(fn_800F7F90(), i);
        slot->mD8 = data->mD8[i];
        slot->mD4Flags.m22 = data->mD4[i];
    }
    if (data->m6C != 2) {
        fn_800F7F90()->m55C = data->m00;
    }
    fn_800F7F90()->m564 = data->m04;
    fn_800F7F90()->m5A0 = (signed char)data->m66;
    fn_800F7F90()->m5A8 = data->m67;
    fn_800F7F90()->mMode = data->mMode;
    PPManager2_unk* mgr = fn_800F7F90();
    mgr->m5A4 = data->m6B;
    if (mgr->m5A4 > 6) {
        mgr->m5A4 = 6;
    }
    fn_800F7F90()->m5B0 = data->m08;
    fn_800F7F90()->m508 = data->m64;
    player->mFlags.m28 = ((PPPlayerFlags_unk*)&data->mFlags)->m28;
    player->mFlags.m06 = ((PPPlayerFlags_unk*)&data->mFlags)->m06;
    player->mFlags.m02 = ((PPPlayerFlags_unk*)&data->mFlags)->m02;
    player->mFlags.m01 = ((PPPlayerFlags_unk*)&data->mFlags)->m01;
    if (IsServerAid_unk(player->mAid) == 1) {
        player->m0B0[0] = data->m09;
        *(PPWord2_unk*)&player->m0B0[1] = data->m0D;
        player->m0B0[3] = data->m15;
        *(PPWord2_unk*)&player->m0B0[4] = data->m19;
        player->mC8 = data->m21;
        player->mFlags.m22 = ((PPPlayerFlags_unk*)&data->mFlags)->m22;
        player->mFlags.m10 = ((PPPlayerFlags_unk*)&data->mFlags)->m10;
    }
    PPManager2_unk* mgr2 = fn_800F7F90();
    if (fn_804DD158(mgr2, lbl_8087F628->GetMyAid_unk()) != 0) {
        for (int i = 0; i < (int)fn_800F7F90()->mPlayers.size(); i++) {
            PPPlayer_unk* slot = fn_804D5F0C(fn_800F7F90(), i);
            if (slot != 0) {
                slot->mFlags.m22 = data->mTeams[i];
                slot->mB0 = (signed char)data->mB0[i];
            }
        }
    }
    fn_800F7F90()->m5B4 = data->m69;
    switch (data->m6C) {
    case 0: {
        NetMsgI_unk<PPMsgAnswer_unk> msg(0x1035);
        PPMsgAnswer_unk* out = &msg.mData;
        out->m00 = data->mSeq;
        fn_804AE3BC()->fn_8050E094(aid, 0x1035, out, 1);
        fn_800F7F90()->m99B0 = 1;
        fn_800F7F90()->m99B1 = aid;
        fn_800F7F90()->m99B4 = 1;
        if (MP::D2AMpMenuLobby::sInstance != 0) {
            MP::D2AMpMenuLobby::sInstance->mF90 = 2;
        }
        break;
    }
    case 1:
        fn_800F7F90()->m9A74[0] = 1;
        break;
    }
}

// inferred: message 0x35 asked by `aid`: the first byte goes back in a message 0x1035 and the manager answers it
// after the loop (m99B0)
extern "C" void fn_804F4CB8(unsigned int aid) {
    if (PP_NO_DATA_UNK) {
        return;
    }
    unsigned char* in = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    NetMsgI_unk<PPMsgAnswer_unk> msg(0x1035);
    PPMsgAnswer_unk* data = &msg.mData;
    data->m00 = in[0];
    fn_804AE3BC()->fn_8050E094(aid, 0x1035, data, 1);
    ((PPManager2_unk*)lbl_8087F610)->m99B0 = 1;
    ((PPManager2_unk*)lbl_8087F610)->m99B1 = aid;
    ((PPManager2_unk*)lbl_8087F610)->m99B4 = 1;
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
extern "C" void fn_804F4E40() {
    const PPMsgCharsIn_unk* data = (const PPMsgCharsIn_unk*)fn_804AE3BC()->GetWorkBuffer_unk();
    for (int i = 0; i < 8; i++) {
        PPPlayer_unk* player = fn_804D5F0C((PPManager2_unk*)lbl_8087F610, i);
        if (player != 0) {
            player->mDC.fn_80501288((CharaTask*)data->mIds[i]);
        }
    }
    ((PPManager2_unk*)lbl_8087F610)->m2B80.m00 = data->m10;
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

// inferred: answers the time of the game to `aid` (message 0x1035)
extern "C" void fn_804F4FC4(int aid) {
    if (((PPManager2_unk*)lbl_8087F610)->mState != 30) {
        return;
    }
    unsigned char* in = (unsigned char*)fn_804AE3BC()->GetWorkBuffer_unk();
    long long time = OSGetTime() - ((PPManager2_unk*)lbl_8087F610)->mStartTime;
    NetMsgI_unk<PPMsgAnswer_unk> msg(0x1035);
    PPMsgAnswer_unk* data = &msg.mData;
    data->mTime = time;
    data->m00 = in[0];
    fn_804AE3BC()->fn_8050E094(aid, 0x1035, data, 1);
}

// inferred: message 0x3E: the lobby is told to refresh (mF90)
extern "C" void fn_804F50C4(unsigned int aid) {
    MP::D2AMpMenuLobby* lobby = MP::D2AMpMenuLobby::sInstance;
    if (lobby != 0) {
        lobby->mF90 = 1;
    }
}

#include "MP/MpCampMsgTable_unk.h"
#include "game/LogMessageTask.h"
// NONMATCHING: 96.2% - the original addresses the categories of m2A50 from the manager (the table typed as
// MpMessageTable_unk inside MP::Manager2; here a cast of PPTable_unk), aid/offset of the inlined own-slot searches swapped, the color as a select instead of two branches and the
// state test of the lobby (2, 3-6) merged into one range
// inferred: shows a message of a player: in the lobby (state 12) as a balloon over the member (a message of the
// table, or with category 0xFFFF one of the texts 0xDF-0xE5), in the game in the log (a message of the table
// with the name of the sender, or the defeat messages 22-24 with the two names packed in `index`)
extern "C" int fn_804F50DC(PPManager2_unk* mgr, PPPlayer_unk* player, unsigned short category, unsigned short index) {
    if (mgr->mState != 12) {
        if (category == -1 || player == 0) {
            return 0;
        }
        if (LogMessageTask::sInstance == 0) {
            return 0;
        }
        wchar_t buf[256];
        buf[0] = 0;
        MpMessage_unk* msg = PPMessages_unk(mgr)->fn_804C54FC(category, index);
        if (msg != 0) {
            atn::net::NetPlayerInfo_unk* info = lbl_8087F628->vf14(player->mAid);
            if (info != 0) {
                fn_800DD3FC(buf, L"%s: %s\0", info->mName, msg->mText != 0 ? msg->mText : MP::lbl_808813D0);
                buf[63] = 0;
                LogMessageTask::sInstance->fn_8047961C(
                    buf, player == fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) ? (int)0xAAFFFFAA : -1, 0, 9);
            }
        } else {
            int type = category - 22;
            int kind = 0;
            if (type == 0) {
                PPPlayer_unk* a = fn_804DD158(mgr, (index >> 8) & 0xFF);
                PPPlayer_unk* b = fn_804DD158(mgr, index & 0xFF);
                if (a != 0 && b != 0 && a->mRef.mChara != 0 && b->mRef.mChara != 0) {
                    int enemy = 0;
                    if (mgr->mMode != 2) {
                        enemy = fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk()) != 0 && b != 0 && fn_804DD158(mgr, lbl_8087F628->GetMyAid_unk())->mFlags.m22 != b->mFlags.m22;
                    }
                    if (!enemy) {
                        kind = 1;
                        fn_800DD3FC(buf, lbl_8087F1E4->Get_unk(17), a->mRef.mChara->GetInfo_unk()->m04,
                                    b->mRef.mChara->GetInfo_unk()->m04);
                    } else {
                        fn_800DD3FC(buf, lbl_8087F1E4->Get_unk(15), a->mRef.mChara->GetInfo_unk()->m04,
                                    b->mRef.mChara->GetInfo_unk()->m04);
                    }
                }
            } else if (type == 1) {
                PPPlayer_unk* a = fn_804DD158(mgr, (index >> 8) & 0xFF);
                PPCharaEntry_unk* b = &mgr->mCharas[index & 0xFF];
                if (a != 0 && b != 0 && a->mRef.mChara != 0 && b->mRef.mChara != 0) {
                    fn_800DD3FC(buf, lbl_8087F1E4->Get_unk(15), a->mRef.mChara->GetInfo_unk()->m04,
                                b->mRef.mChara->GetInfo_unk()->m04);
                }
            } else if (type == 2) {
                PPCharaEntry_unk* a = &mgr->mCharas[(index >> 8) & 0xFF];
                PPPlayer_unk* b = fn_804DD158(mgr, index & 0xFF);
                kind = 1;
                if (a != 0 && b != 0 && a->mRef.mChara != 0 && b->mRef.mChara != 0) {
                    fn_800DD3FC(buf, lbl_8087F1E4->Get_unk(17), a->mRef.mChara->GetInfo_unk()->m04,
                                b->mRef.mChara->GetInfo_unk()->m04);
                }
            }
            if (buf[0] != 0) {
                LogMessageTask::sInstance->fn_8047961C(buf, 0xFFFFFFFF, kind, 9);
            }
        }
    } else {
        MP::D2AMpMenuLobby* lobby = MP::D2AMpMenuLobby::sInstance;
        if (lobby == 0) {
            return 0;
        }
        if (lobby->mD88 != 2 && !(lobby->mD88 >= 3 && lobby->mD88 <= 6)) {
            return 0;
        }
        if (category != 0xFFFF) {
            MpMessage_unk* msg = PPMessages_unk(mgr)->fn_804C54FC(category, index);
            if (msg == 0) {
                return 0;
            }
            MP::D2AMpMenuLobby::sInstance->fn_804C2D5C((MpLobbyMember_unk*)player,
                                                     msg->mText != 0 ? msg->mText : MP::lbl_808813D0,
                                                     msg->m0C + 10, 0);
        } else {
            if (lbl_8087F86C == 0) {
                return 0;
            }
            unsigned short id = index;
            if (id < 0xDF || id > 0xE5) {
                return 0;
            }
            const wchar_t* text = lbl_8087F86C->Get_unk(id);
            if (text == 0) {
                return 0;
            }
            lobby->fn_804C2D5C((MpLobbyMember_unk*)player, text, wcslen(text) + 10, 1);
        }
    }
    return 1;
}

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
extern "C" bool fn_804F5A98(PPRequests_unk* queue, unsigned char a, int b, unsigned char c) {
    PPRequest_unk* req = FindFreeRequest_unk(queue);
    if (req == 0) {
        return false;
    }
    req->mUsed = 1;
    req->m04 = 0;
    req->m14 = a;
    req->m08 = b;
    req->m15 = c;
    return true;
}

// inferred: ages the requests: after the timer a new request waits 150 more frames, an old one is freed

extern "C" long long* fn_804F5B08(PPRequests_unk* queue, unsigned int id) {
    PPRequest_unk* req = FindRequest_unk(queue, id);
    if (req == 0) {
        return 0;
    }
    return (long long*)req->m0C;
}

extern "C" int fn_804F5C9C(PPRequests_unk* queue, unsigned int id) {
    PPRequest_unk* req = FindRequest_unk(queue, id);
    if (req == 0) {
        return 0;
    }
    req->mUsed = 0;
    return 1;
}

extern "C" int fn_804F5E38(PPRequests_unk* queue, unsigned int id) {
    PPRequest_unk* req = FindRequest_unk(queue, id);
    if (req == 0) {
        return -2;
    }
    return req->m04;
}

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

extern "C" void fn_804F60D4(void* arg, void* value) {}

extern "C" void fn_804F60D8(void* arg, void* value) {}

extern "C" void fn_804F60DC(void* arg, void* value) {}

#include "macros.h"

// inferred: order of the ranking rows: higher score first
// (then the older update, the pid, the user data and the address); the sort 0x804F62D8, its recursive copy
// 0x804F6E94 and the median 0x804F7A50 are emitted after fn_804F60E0
struct NetRankOrder_unk {
    bool operator()(const atn::net::NetRankData_unk& a, const atn::net::NetRankData_unk& b) const {
        if (a.mScore != b.mScore) {
            return a.mScore > b.mScore;
        }
        if (a.mLastUpdate != b.mLastUpdate) {
            return a.mLastUpdate < b.mLastUpdate;
        }
        if ((long)a.mPid != (long)b.mPid) {
            return (long)a.mPid < (long)b.mPid;
        }
        if (a.mUserData != b.mUserData) {
            return a.mUserData < b.mUserData;
        }
        return &a < &b;
    }
};

// inferred: callback of the ranking requests: sorts the rows by score and numbers them (equal scores share the
// place); the places are moved so that the first row keeps its place (the own row when asked around it) and
// kept in 1..99999
extern "C" void fn_804F60E0(int result, atn::net::NetRankData_unk* data) {
    atn::FixedArray_unk<atn::net::NetRankData_unk, 65>* rows = &lbl_8087F5A4->mRows;
    if (rows->size() == 0) {
        return;
    }
    int pid = (*rows)[0].mPid;
    int order = (*rows)[0].mOrder;
    MslSort_unk(rows->begin(), rows->end(), NetRankOrder_unk());
    atn::net::NetRankData_unk* row = rows->begin();
    unsigned long place = 1;
    unsigned long ties = 0;
    atn::net::NetRankData_unk* prev = 0;
    for (unsigned int i = 0; i < rows->size(); i++) {
        if (prev == 0) {
            row->mOrder = place;
        } else {
            if (prev->mScore > row->mScore) {
                place = place + ties + 1;
                ties = 0;
            } else {
                ties++;
            }
            row->mOrder = place;
        }
        prev = row;
        row++;
    }
    row = rows->begin();
    int index = -1;
    for (unsigned int i = 0; i < rows->size(); row++, i++) {
        if ((int)row->mPid == pid) {
            index = i;
            break;
        }
    }
    if (order > 0 && index >= 0) {
        int diff = order - (*rows)[index].mOrder;
        row = rows->begin();
        for (unsigned int i = 0; i < rows->size(); row++, i++) {
            row->mOrder = MIN(MAX(row->mOrder + diff, 1), 99999);
        }
    } else {
        row = rows->begin();
        unsigned long v;
        for (unsigned int i = 0; i < rows->size(); row++, i++) {
            v = row->mOrder;
            row->mOrder = MIN(MAX(v, 1), 99999);
        }
    }
}

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

#include "atn/RenderManager.h"
#include "atn/DrawHelper.h"

extern "C" void fn_804F8464(PPManager2_unk* mgr); // inferred: debug page of the network

// NONMATCHING: 99.03% - offsets of the strings in @stringBase0 and @wstringBase0 (the literals of the functions
// not written yet go before) and registers of the inlined search fn_804EB870 and of the loop counters
// inferred: MP::Manager2::vf14 (vtable 0x807916E0): debug display of the connection, the network page and
// the players and characters over their heads (m528, "Control info" of the debug menu)
void MP::Manager2::vf14() {
    if (lbl_8087F628->mFlags.mB13) {
        lbl_8087EEB0->fn_80061824(450.0f, 30.0f, 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f, L"DEBUG CONNECTION", 0x88FFFFFF,
                                  1, 1, 0, 0, 0xFF000000);
    }
    if (mStateVar.mValue == 30) {
        // inferred: the own slot is looked up and not used (debug output compiled out)
        fn_804EB870(this);
    }
    if (lbl_8087F628->mState >= 11) {
        fn_804F8464(this);
    }
    if (m528 != 0) {
        for (int i = 0; i < (int)mPlayers.size(); i++) {
            PPPlayer_unk* player = &mPlayers[i];
            if (player->mRef.mChara != 0 && player->mFlags.mActive) {
                atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(
                    player->mRef.mChara->mPos + atn::Vector3(0.0f, 20.0f, 0.0f));
                if (0.0f <= screen.z && screen.z <= 1.0f) {
                    char text[0x100];
                    sprintf(text, "HostID:%d Score:%d\0", player->mAid, player->mDCScore);
                    lbl_8087EEB0->fn_800616C0(1.0f + screen.x, 1.0f + screen.y, 0.0f, 16.0f, 16.0f, text, 0xFF000000,
                                              0.0f, 1, 1, 0);
                    lbl_8087EEB0->fn_800616C0(screen.x, screen.y, 0.0f, 16.0f, 16.0f, text, 0xFFFFAA00, 0.0f, 1, 1, 0);
                    screen.y += 16.0f;
                    sprintf(text, "Team:%d Ctrl:%c%c0", player->mRef.mChara->mF14,
                            player->mRef.mChara->mFlag2 ? 'N' : 'L', IsTeamPlayer_unk(player) ? 'B' : ' ');
                    lbl_8087EEB0->fn_800616C0(1.0f + screen.x, 1.0f + screen.y, 0.0f, 16.0f, 16.0f, text, 0xFF000000,
                                              0.0f, 1, 1, 0);
                    lbl_8087EEB0->fn_800616C0(screen.x, screen.y, 0.0f, 16.0f, 16.0f, text, 0xFFFFAA00, 0.0f, 1, 1, 0);
                    screen.y += 16.0f;
                }
            }
        }
        for (int i = 0; i < (int)mCharas.size(); i++) {
            PPCharaEntry_unk* entry = &mCharas[i];
            if (entry->mRef.mChara != 0) {
                atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(
                    entry->mRef.mChara->mPos + atn::Vector3(0.0f, 20.0f, 0.0f));
                if (0.0f <= screen.z && screen.z <= 1.0f) {
                    char text[0x100];
                    sprintf(text, "Local:%d Ctrl:%c%c0", entry->mActive, entry->mRef.mChara->mFlag2 ? 'N' : 'L');
                    lbl_8087EEB0->fn_800616C0(1.0f + screen.x, 1.0f + screen.y, 0.0f, 16.0f, 16.0f, text, 0xFF000000,
                                              0.0f, 1, 1, 0);
                    lbl_8087EEB0->fn_800616C0(screen.x, screen.y, 0.0f, 16.0f, 16.0f, text, 0xFFAA00FF, 0.0f, 1, 1, 0);
                    screen.y += 16.0f;
                }
            }
        }
    }
}

extern "C" void itow(int value, wchar_t* buf, int radix);
// inferred: names shown by the network debug page fn_804F8464 (inline members: each literal is its own object
// in .data in the original)
struct PPDebugNames_unk {
    static const wchar_t* MatchType(int type) {
        switch (type) {
        case 0:
            return L"ANYBODY";
        case 1:
            return L"FRIEND";
        default:
            return L"UNSUPPORTED";
        }
    }
    static const wchar_t* Mode(int mode) {
        switch (mode) {
        case 0:
            return L"PVP_PLAYER";
        case 1:
            return L"PVP_TEAM";
        case 2:
            return L"RAID";
        default:
            return L"UNSUPPORTED";
        }
    }
    static const wchar_t* Stage(int stage) {
        switch (stage) {
        case 0:
            return L"STAGE_01";
        case 1:
            return L"STAGE_02";
        case 2:
            return L"STAGE_03";
        case 3:
            return L"STAGE_04";
        case 4:
            return L"STAGE_05";
        case 5:
            return L"STAGE_06";
        case 6:
            return L"STAGE_07";
        case 7:
            return L"STAGE_08";
        case 8:
            return L"STAGE_09";
        case 9:
            return L"STAGE_10";
        case 10:
            return L"RANDOM";
        default:
            return L"UNSUPPORTED";
        }
    }
    static const wchar_t* State(int state) {
        switch (state) {
        case -1:
            return L"ERROR";
        case 0:
            return L"INIT";
        case 3:
            return L"INIT3";
        case 5:
            return L"WAIT_INIT";
        case 7:
            return L"OFFLINE";
        case 8:
            return L"REGISTER";
        case 9:
            return L"WAIT_REG";
        case 10:
            return L"MENU";
        case 11:
            return L"WAIT_JOIN";
        case 12:
            return L"LOBBY";
        case 18:
            return L"LOAD";
        case 19:
            return L"WAIT_LOAD";
        case 26:
            return L"WAIT_SYNC";
        case 30:
            return L"PLAY";
        case 31:
            return L"PLAY_END";
        case 33:
            return L"FINISH";
        case 38:
            return L"RANK_LOAD_TOPRANKING";
        case 39:
            return L"RANK_LOAD_NEARRANKING";
        case 40:
            return L"RANK_LOAD_FRIENDRANKING";
        case 41:
            return L"RANK_WAIT_LOAD_RANKING";
        case 42:
            return L"RANK_SHOW";
        default:
            return L"UNSUPPORTED";
        }
    }
};

// inferred: one line of the network debug page
static inline void PPDebugText_unk(int line, const wchar_t* text, unsigned int color) {
    lbl_8087EEB0->fn_80061824(30.0f, 15.0f * line + 60.0f, -1000.0f, 15.0f, 15.0f, 0.0f, 0.0f, 0.0f, text, color, 1,
                              1, 0, 0, 0xFF000000);
}

// inferred: one line of the network debug page (multibyte text)
static inline void PPDebugTextA_unk(int line, const char* text, unsigned int color) {
    lbl_8087EEB0->fn_800616C0(30.0f, 15.0f * line + 60.0f, -1000.0f, 15.0f, 15.0f, text, color, 0.0f, 1, 0, 0);
}
// inferred: number of players of the session (m264)
static inline int PPNumNetPlayers_unk(atn::net::Manager* net) {
    return net->m264;
}
// inferred: font of the debug texts (lbl_8087EEC8)
static inline void* PPDebugFont_unk() {
    return lbl_8087EEC8;
}

// NONMATCHING: 99.71% - registers of the loops of the hosts (net/info/offset) and the players (slot, color,
// offset) and of the MP::Manager2 pointer (r25 in the original), and the offsets of @wstringBase0 (L"%s: %s" of
// fn_804F50DC, not written yet, goes first)
// inferred: debug page of the network (m51C): state of the connection, traffic, settings of the game, the hosts
// of the session (m520) and the player slots (m524)
extern "C" void fn_804F8464(PPManager2_unk* mgr) {
    int line = 0;
    if (mgr->m51C == 0) {
        return;
    }
    lbl_8087EEB0->fn_80060D58(0.0f, 0.0f, 0.0f, 640.0f, 480.0f, 0x80000000);
    PPDebugText_unk(line++, L"DebugInfo:", 0xFFFFFFFF);

    wchar_t buf[256];
    std::basic_string<wchar_t> net(L"Net: state=");
    wchar_t c = fn_8006F2F0(PPDebugFont_unk(), buf, lbl_8087F628->fn_8050B968(lbl_8087F628->mState), 0x100);
    net.replace(net.size(), 0, &c, &c + 1);
    net += L", connect=";
    net += L", type=";
    net += PPDebugNames_unk::MatchType(lbl_8087F628->m25C);
    PPDebugText_unk(line++, net.c_str(), 0xFFFFFFFF);

    float downMax = lbl_8087F628->mStats.fn_8050E7DC(lbl_8087F628->mStats.m30.mMax, 3);
    float down = lbl_8087F628->mStats.fn_8050E7DC(lbl_8087F628->mStats.m30.mLast, 3);
    float upMax = lbl_8087F628->mStats.fn_8050E7DC(lbl_8087F628->mStats.m10.mMax, 3);
    float up = lbl_8087F628->mStats.fn_8050E7DC(lbl_8087F628->mStats.m10.mLast, 3);
    char text[256];
    sprintf(text, "Profiler: up=%.2f Kb/s (max=%.2f), down=%.2f Kb/s (max=%.2f)", up / 1024.0f, upMax / 1024.0f,
            down / 1024.0f, downMax / 1024.0f);
    PPDebugTextA_unk(line++, text, 0xFFFFFFFF);

    std::basic_string<wchar_t> mp(L"Mp: state=");
    mp += PPDebugNames_unk::State(mgr->mState);
    mp += L", mode=";
    mp += PPDebugNames_unk::Mode(mgr->mMode);
    mp += L", stage=";
    mp += PPDebugNames_unk::Stage(mgr->m560);
    itow(mgr->m564, buf, 10);
    mp += L", time=";
    mp += buf;
    itow(mgr->m568, buf, 10);
    mp += L", score=";
    mp += buf;
    PPDebugText_unk(line++, mp.c_str(), 0xFFFFFFFF);

    mp = L"Mp: startpos=";
    for (int i = 0; i < 8; i++) {
        itow(mgr->m56C.m[i], buf, 10);
        mp += buf;
        mp += L",";
    }
    PPDebugText_unk(line++, mp.c_str(), 0xFFFFFFFF);

    mp = L"Mp: item=";
    for (int i = 0; i < 3; i++) {
        itow(mgr->m58C.m[i], buf, 10);
        mp += buf;
        mp += L",";
    }
    mp += L", random=";
    itow(mgr->m598, buf, 10);
    mp += buf;
    PPDebugText_unk(line++, mp.c_str(), 0xFFFFFFFF);

    if (mgr->m520 != 0) {
        std::basic_string<wchar_t> hosts(L"Hosts: num=");
        wchar_t num[256];
        itow(GetNumHosts_unk(lbl_8087F628), num, 10);
        hosts += num;
        PPDebugText_unk(line++, hosts.c_str(), 0xFFFFFFFF);
        for (int i = 0; i < PPNumNetPlayers_unk(lbl_8087F628); i++) {
            unsigned int color = 0xFFFFFFFF;
            atn::net::Manager* net = lbl_8087F628;
            atn::net::NetPlayerInfo_unk* info = &net->mPlayers.mData[i];
            if (!info->mFlags.mConnected) {
                color = 0xFF808080;
            } else if (info->mAid == net->GetMyAid_unk()) {
                color = 0xFFFFFF40;
            }
            itow(i, num, 10);
            hosts = L" ";
            hosts += num;
            hosts += L": id=";
            itow(info->mAid, num, 10);
            hosts += num;
            hosts += L", PId=";
            itow(info->m04, num, 10);
            hosts += num;
            hosts += L", flag=";
            if (info->mFlags.mServer) {
                hosts += L"S ";
            }
            if (info->mFlags.mB3) {
                hosts += L"O ";
            }
            if (info->mFlags.mB4) {
                hosts += L"G";
            }
            hosts += L", name=";
            hosts += info->mName;
            hosts += L", ping=";
            itow(info->mPing, num, 10);
            hosts += num;
            PPDebugText_unk(line++, hosts.c_str(), color);
        }
    }

    if (mgr->m524 != 0) {
        std::basic_string<wchar_t> players(L"Players: num=");
        wchar_t num[256];
        itow((int)mgr->mPlayers.size(), num, 10);
        players += num;
        PPDebugText_unk(line++, players.c_str(), 0xFFFFFFFF);
        for (int i = 0; i < (int)mgr->mPlayers.size(); i++) {
            PPPlayer_unk* player = &mgr->mPlayers[i];
            unsigned int color = 0xFFFFFFFF;
            if (!player->mFlags.mActive) {
                color = 0xFF808080;
            } else if (player->mAid == lbl_8087F628->GetMyAid_unk()) {
                color = 0xFFFFFF40;
            } else if (player->mRef.mChara != 0 && player->mRef.mChara->mStatus.IsState_unk(0x20)) {
                color = 0xFF800000;
            }
            players = L" ";
            itow(i, num, 10);
            players += num;
            players += L": id=";
            itow(player->mAid, num, 10);
            players += num;
            players += L", flags=";
            if (player->mFlags.mActive) {
                players += L"U ";
            }
            if (player->mFlags.m29) {
                players += L"C ";
            }
            if (player->mFlags.m28) {
                players += L"R ";
            }
            if (player->mFlags.m27) {
                players += L"L ";
            }
            if (player->mFlags.m26) {
                players += L"E ";
            }
            if (player->mFlags.m22 != 0) {
                players += L"Team";
                itow(player->mFlags.m22, num, 10);
                players += num;
                players += L" ";
            }
            if (player->mFlags.m14 != 0) {
                players += L"Level";
                itow(player->mFlags.m14, num, 10);
                players += num;
                players += L" ";
            }
            players += L", chara=";
            if (player->mRef.mChara == 0) {
                players += L"NULL ";
            } else {
                if (player->mRef.mChara->IsFlag_unk(1)) {
                    players += L"P";
                } else {
                    players += L" ";
                }
                if (player->mRef.mChara->IsFlag_unk(4)) {
                    players += L"F";
                } else {
                    players += L" ";
                }
                if (player->mRef.mChara->mFlag2) {
                    players += L"Net";
                }
                int control = player->mRef.mChara->m55C;
                if (control == 6) {
                    players += L"Pro";
                } else if (control == 1) {
                    players += L"Pad";
                } else if (control == 2) {
                    players += L"Ai";
                } else {
                    players += L"???";
                }
            }
            players += L", score=";
            itow(player->mDCScore, num, 10);
            players += num;
            PPDebugText_unk(line++, players.c_str(), color);
        }
    }
}

// inferred: text of the value of a rule (kinds 0/1: 9 values, kind 2: 5 values; the last one is the shared
// entry 0x2A)
extern "C" const wchar_t* fn_804FA7C8(PPManager2_unk* mgr, int kind, int value) {
    if (kind == 0 || kind == 1) {
        if (0 <= value && value < 9) {
            return lbl_8087F86C->Get_unk(value + 0x2B);
        } else if (value == 9) {
            return lbl_8087F86C->Get_unk(0x2A);
        }
    } else if (kind == 2) {
        if (0 <= value && value < 5) {
            return lbl_8087F86C->Get_unk(value + 0x38);
        } else if (value == 5) {
            return lbl_8087F86C->Get_unk(0x2A);
        }
    }
    return L"UNSUPPORTED";
}

// inferred: name of the level of an experience
extern "C" const wchar_t* fn_804FA88C(PPManager2_unk* mgr, int exp) {
    MpLevel_unk* level = fn_805075C4(&mgr->m610.mTable, exp);
    if (level != 0) {
        return lbl_8087F86C->Get_unk(level->m00);
    }
    return L"UNSUPPORTED";
}

// inferred: title of the level of an experience
extern "C" const wchar_t* fn_804FA8E8(PPManager2_unk* mgr, int exp) {
    MpLevel_unk* level = fn_805075C4(&mgr->m610.mTable, exp);
    if (level != 0) {
        return lbl_8087F86C->Get_unk(level->m00 + 0x15);
    }
    return L"UNSUPPORTED";
}

// inferred: sends the value of message 0x1039 (one byte) to the server
extern "C" void fn_804FA948(PPManager2_unk* mgr, int value, unsigned int flush) {
    NetMsgI_unk<unsigned char> msg(0x1039);
    unsigned char* data = &msg.mData;
    *data = value;
    fn_804AE3BC()->fn_8050E094(lbl_8087F628->GetServerAid_unk(), 0x1039, data, 1);
    if (flush == 1) {
        fn_804AE3BC()->fn_8050DDDC(lbl_8087F628->GetServerAid_unk(), 1);
    }
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

// NONMATCHING: 99.58% - registers of the inlined request search of fn_804F5A98 (base/index/pointer)
// inferred: payload of the message 0x1036 (sequence number of the slot and a value); names invented
struct PPMsgSeq_unk {
    unsigned char mSeq;
    unsigned char mValue;
};
// inferred: sends `value` to the server with the next sequence number of the slot; with `seq` the request is
// queued (answer expected) and its number returned there
extern "C" __declspec(noinline) int fn_804FAB40(PPManager2_unk* mgr, int value, PPPlayer_unk* player, int* seq, unsigned int flush) {
    NetMsgI_unk<PPMsgSeq_unk> msg(0x1036);
    PPMsgSeq_unk* data = &msg.mData;
    unsigned char next = player->m0D52++;
    data->mSeq = next;
    data->mValue = value;
    if (seq != 0) {
        if (fn_804F5A98((PPRequests_unk*)fn_800F7F90(), lbl_8087F628->GetServerAid_unk(), 150, data->mSeq) == 0) {
            return 0;
        }
        *seq = data->mSeq;
    }
    fn_804AE3BC()->fn_8050E094(lbl_8087F628->GetServerAid_unk(), 0x1036, data, 1);
    if (flush == 1) {
        fn_804AE3BC()->fn_8050DDDC(lbl_8087F628->GetServerAid_unk(), 1);
    }
    return 1;
}

extern "C" int SCCheckPCMessageRestriction();
extern "C" const wchar_t* fn_804FAF48(PPManager2_unk* mgr, int* code, int type); // inferred: text of a DWC error

// note: the offset of L"%s" in @wstringBase0 (pool in .sdata in the original, not complete) is not seen by objdiff
// inferred: shows the error dialog: the message `id` of the table, or (id -1) the restriction of the parental
// controls, the last DWC error or the oldest queued error with its code; then the ranking is closed if it failed
extern "C" void fn_804FAD48(PPManager2_unk* mgr, int id) {
    if (lbl_8087F628 == 0 || MP::D2ACommonDialog::sInstance == 0) {
        return;
    }
    // inferred: the original tests the dialog a second time (`beq` on the same compare after the guard)
    if (MP::D2ACommonDialog::sInstance == 0) {
        return;
    }
    wchar_t buf[512];
    const wchar_t* text = 0;
    if (id != -1) {
        text = lbl_8087F86C->Get_unk(id);
    } else if (SCCheckPCMessageRestriction() != 0) {
        text = lbl_8087F86C->Get_unk(0x8D);
    } else if (lbl_8087F628->fn_8050C168() != 0) {
        int code = lbl_8087F628->mBEC4;
        const wchar_t* format = lbl_8087F1E4->Get_unk(0x2ED);
        fn_800DD3FC(buf, format, code, fn_804FAF48(mgr, &code, 0));
        text = buf;
    } else if (lbl_8087F628->mErrors.mCount != 0) {
        int type;
        int code = lbl_8087F628->fn_8050C128(&type);
        const wchar_t* error = fn_804FAF48(mgr, &code, type);
        if (code >= 10000) {
            fn_800DD3FC(buf, lbl_8087F1E4->Get_unk(0x2ED), code, error);
        } else {
            fn_800DD3FC(buf, L"%s", error);
        }
        text = buf;
    }
    if (text == 0) {
        text = lbl_8087F86C->Get_unk(0x9A);
    }
    if (text != 0) {
        MP::D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 2);
        MP::D2ACommonDialog::sInstance->fn_804ACE68();
        MP::D2ACommonDialog::sInstance->fn_804ACD10();
        MP::D2ACommonDialog::sInstance->fn_804AD1EC(text);
    }
    if (lbl_8087F5A4->mState == 2) {
        lbl_8087F5A4->fn_8050C7F0();
    }
}

extern "C" int NETGetStartupErrorCode(int);

// inferred: message of a network error: the startup codes (< 10000) and the DWC error codes (by ranges, with
// the state of the network manager for the connection errors) give the id of the message in m9750
extern "C" const wchar_t* fn_804FAF48(PPManager2_unk* mgr, int* code, int type) {
    int done = 0;
    int state = lbl_8087F628->fn_8050C0B4();
    mgr->m9750 = -1;
    if (*code < 10000) {
        if (*code == 5) {
            *code = -NETGetStartupErrorCode(type);
        } else if (*code == 7) {
            mgr->m9750 = 0x8B;
        } else if (*code == 15) {
            mgr->m9750 = 0xA1;
        } else if (*code == 13) {
            mgr->m9750 = 0xA2;
        } else {
            mgr->m9750 = 0x99;
        }
        done = 1;
    }
    switch (*code) {
    case 20102 ... 20109:
        mgr->m9750 = 0x90;
        break;
    case 20111 ... 20999:
        mgr->m9750 = 0x90;
        break;
    case 20101:
    case 23000 ... 23999:
        mgr->m9750 = 0x91;
        break;
    case 20110:
        mgr->m9750 = 0x92;
        break;
    case 29000:
        mgr->m9750 = 0x93;
        break;
    case 29001:
        mgr->m9750 = 0x94;
        break;
    case 20100:
    case 50000 ... 59999:
        mgr->m9750 = 0x95;
        break;
    case 80430:
        mgr->m9750 = 0x96;
        break;
    case 80431:
        mgr->m9750 = 0x97;
        break;
    case 80432:
        mgr->m9750 = 0x98;
        break;
    case 60000 ... 69999:
        mgr->m9750 = 0x99;
        break;
    case 24000 ... 25999:
    case 70000 ... 80429:
    case 80433 ... 99999:
        mgr->m9750 = 0x99;
        if (state == 4) {
            mgr->m9750 = 0x9C;
        } else if (state <= 1) {
            mgr->m9750 = 0x9A;
        }
        break;
    case 31000 ... 31999:
        mgr->m9750 = 0x99;
        break;
    default:
        if (done == 0) {
            mgr->m9750 = 0x99;
        }
        break;
    }
    if (mgr->m9750 == -1) {
        return L"";
    }
    return lbl_8087F86C->Get_unk(mgr->m9750);
}

// inferred: flag m976C once the network manager reached state 8
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
    if (lbl_8087F628->mState < 8) {
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

extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const RsColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an effect file at the origin, without model or owner
inline void PlayEffect_unk(atn::EfpFile* efp) {
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(0.0f, 0.0f, 0.0f),
                RsColor_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
}

// inferred: back to the lobby: render settings, camera of the lobby, effects (be010_008.efp) and BGM
// NONMATCHING: 98.57% - registers of the inlined fn_804D0164: the original keeps the zero in r5 and
// lbl_8087EFA8 in r30 (here the zero in r6, lbl_8087EFA8 in r5 and the address of the target in r30)
extern "C" void fn_804FB674(PPManager2_unk* mgr) {
    fn_804D0164();
    mgr->m9774.SetPosition(atn::Vector3(0.0f, -99500.0f, 0.0f));
    mgr->m9774.SetTarget_unk(atn::Vector3(0.0f, -99500.0f, 10.0f));
    fn_8037F688(&mgr->m9774, atn::DegToRad_unk(60.0f));
    if (lbl_8087F3C0 == 0) {
        atn::EffectManager::fn_80239798(mgr, 0x240, 0x480, 8, 0x180);
    }
    fn_80239DAC(lbl_8087F3C0, (unsigned int)mgr, 0, 1);
    fn_80232B7C((unsigned int)mgr, 0);
    PlayEffect_unk(&mgr->m9968);
    if (GameBgm::GetInstance_unk() != 0) {
        GameBgm::GetInstance_unk()->fn_8037D4C0(0x548, 15, 1, 0, 0.7f);
    }
}

// inferred: whether a value of the status is above its limit; name invented
static inline bool PPIsOver_unk(int value, int limit) {
    bool over = true;
    if (value <= limit) {
        over = false;
    }
    return over;
}
// inferred: whether the speed of the character is too high; name invented
static inline int PPIsTooFast_unk(float speed) {
    return speed > 1.5f;
}
// NONMATCHING: 99.9% - only the schedule of the prologue: the original loads mgr->mMode before saving r31 (tried
// the mode in a local, a switch, an early return and the parameter as PlayerTask*)
// inferred: check of the status of the own character before an online game (values above the limits of the
// mode, bonuses, flags of the equipment and of the state); nonzero when something is out of range. In mode 2
// the global flag 0x25 of the game disables the check
extern "C" int fn_804FB968(const PPManager2_unk* mgr, CharaTask* chara) {
    int mode = mgr->mMode;
    bool bad;
    if (mode == 2) {
        if (mgr->mGame->fn_80370174(0x25) == 1) {
            return 0;
        }
        bad = PPIsOver_unk(chara->mStatus.mParamA.m84, 10000);
        bad = bad || chara->mStatus.mParamA.m00 > 2000.0f;
        bad = bad || chara->mStatus.mParamA.m04 > 7000.0f;
        bad = bad || chara->mStatus.mParamA.m08 > 2000.0f;
        bad = bad || chara->mStatus.mParamA.m0C > 800.0f;
        bad = bad || chara->mStatus.mParamA.m94 > 200;
        bad = bad || chara->mStatus.mParamA.m34 > 1;
        bad = bad || chara->mStatus.mParamA.m98 > 5;
        bad = bad || chara->mStatus.m1A8.m08 > 80.0f;
    } else {
        if (mode == 1) {
            bad = PPIsOver_unk(chara->mStatus.mParamA.m84, 5500);
        } else {
            bad = PPIsOver_unk(chara->mStatus.mParamA.m84, 4000);
        }
        bad = bad || chara->mStatus.mParamA.m00 > 2000.0f;
        bad = bad || chara->mStatus.mParamA.m04 > 5000.0f;
        bad = bad || chara->mStatus.mParamA.m08 > 1500.0f;
        bad = bad || chara->mStatus.mParamA.m0C > 800.0f;
        bad = bad || chara->mStatus.mParamA.m94 > 250;
        bad = bad || chara->mStatus.mParamA.m34 > 3;
        bad = bad || chara->mStatus.m1A8.m08 > 60.0f;
    }
    bad = bad || chara->mStatus.mParamB.m28 > 50.0f;
    bad = bad || PPIsTooFast_unk(chara->m568);
    bad = bad || chara->mStatus.mParamA.m18 > 0.2f;
    bad = bad || chara->mStatus.mParamA.m8C > 0.01f;
    bad = bad || ((unsigned int)chara->mStatus.mParamA.m9C & 0x400) == 0x400;
    bad = bad || ((unsigned int)chara->mStatus.mParamA.m9C & 0x800) == 0x800;
    bad = bad || ((unsigned int)chara->mStatus.mParamA.m9C & 0x2000) == 0x2000;
    bad = bad || ((unsigned int)chara->mStatus.mParamA.m9C & 0x8000) == 0x8000;
    bad = bad || ((unsigned int)chara->mStatus.mParamA.m9C & 0x40000) == 0x40000;
    bad = bad || ((unsigned int)chara->mStatus.mParamA.m9C & 0x10000) == 0x10000;
    bad = bad || (chara->mStatus.m18 & 0x1) == 0x1;
    bad = bad || (chara->mStatus.m18 & 0x4) == 0x4;
    bad = bad || (chara->mStatus.m18 & 0x10) == 0x10;
    bad = bad || (chara->mStatus.m18 & 0x20) == 0x20;
    bad = bad || (chara->mStatus.m18 & 0x40) == 0x40;
    bad = bad || (chara->mStatus.m18 & 0x80) == 0x80;
    bad = bad || (chara->mStatus.m18 & 0x4000) == 0x4000;
    bad = bad || (chara->mStatus.m18 & 0x8000) == 0x8000;
    bad = bad || (chara->mStatus.m18 & 0x80000) == 0x80000;
    bad = bad || (chara->mStatus.m18 & 0x400000) == 0x400000;
    bad = bad || (chara->mStatus.m18 & 0x1000000) == 0x1000000;
    bad = bad || (chara->mStatus.m18 & 0x2000000) == 0x2000000;
    bad = bad || (chara->mStatus.m18 & 0x4000000) == 0x4000000;
    bad = bad || chara->mStatus.IsState_unk(0x1000);
    bad = bad || chara->mStatus.IsState_unk(0x200000);
    bad = bad || chara->IsFlag54C_unk(4);
    bad = bad || chara->mFlagB1;
    bad = bad || chara->mStatus.m1A8.m0C > 0.1f;
    bad = bad || chara->mStatus.m1A8.m10 > 0.1f;
    bad = bad || chara->mStatus.m1A8.m14 > 0.1f;
    bad = bad || chara->mStatus.m1A8.m18 > 0.1f;
    bad = bad || chara->mStatus.m1A8.m1C > 0.1f;
    bad = bad || chara->mStatus.m1A8.m20 > 0;
    return bad;
}

// inferred: flag m01 of the slot of the server
extern "C" unsigned int fn_804FC010(PPManager2_unk* mgr) {
    PPPlayer_unk* player = fn_804DD158(mgr, lbl_8087F628->GetServerAid_unk());
    if (player != 0) {
        return player->mFlags.m01;
    }
    return 0;
}

// inferred: on the server, flag m01 of its own slot
extern "C" void fn_804FC0D4(PPManager2_unk* mgr, int value) {
    if (lbl_8087F628->GetMyAid_unk() == lbl_8087F628->GetServerAid_unk()) {
        PPPlayer_unk* player = fn_804DD158(mgr, lbl_8087F628->GetServerAid_unk());
        if (player != 0) {
            player->mFlags.m01 = value;
        }
    }
}

// inferred: info of the player of network id `aid` (two copies of the same accessor)
atn::net::NetPlayerInfo_unk* MP::ManagerMP::vf10(unsigned int aid) {
    unsigned int i = aid & 0xF;
    if (i >= mPlayers.mSize) {
        return 0;
    }
    return &mPlayers.mData[i];
}

atn::net::NetPlayerInfo_unk* MP::ManagerMP::vf14(unsigned int aid) {
    unsigned int i = aid & 0xF;
    if (i >= mPlayers.mSize) {
        return 0;
    }
    return &mPlayers.mData[i];
}

// inferred: shuts the library down and deletes lbl_8087F61C
int MP::ManagerMP::vf0C() {
    int result = atn::net::Manager::vf0C();
    if (lbl_8087F61C != 0) {
        delete lbl_8087F61C;
        lbl_8087F61C = 0;
    }
    return result;
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

namespace MP {

template <class T>
Chara<T>::~Chara() {}

// inferred: the character finished loading
template <class T>
int Chara<T>::vf0C() {
    return this->fn_8013655C() == 0;
}

// inferred: per-frame update; a knocked down character (m58C == 2) sends event 5 to the listeners once
template <class T>
void Chara<T>::vf10() {
    if (m14B0 == 0) {
        this->fn_80176ACC();
        m14B0 = 1;
    }
    if (this->GetFlag1_unk() == 1) {
        return;
    }
    if (this->mStatus.IsState_unk(0x20) && this->m58C != 2) {
        this->m58C = 2;
    }
    int down = this->mFlagC25;
    if (!down || this->m58C == 2) {
        if (down && !this->IsFlag2_unk() && this->m55C != 6) {
            this->mFlag2 = 1;
        }
        if (this->m58C == 2) {
            if (this->fn_8015EB2C() == 0 && m14B4 == 0) {
                this->fn_80139560();
                this->m1450 = 0;
            } else if (this->m1450 == 0) {
                NotifyListeners_unk(fn_8014F8F4(), 5, this, 0);
                this->mColli.mFlags &= ~1;
                this->fn_800D246C(1);
                m14B4 = 1;
                this->m1450 = 1;
            }
        } else {
            m14B4 = 0;
        }
        this->fn_8014C540();
        this->fn_800EE5AC((Arms*)this->m648);
        if (this->m64C != 0) {
            this->fn_800EE5AC((Arms*)this->m64C);
        }
        this->fn_80145334();
    } else {
        m14B4 = 0;
        this->fn_800E2FE0();
    }
}

template <class T>
void Chara<T>::vf14() {
    this->fn_80149A30();
}

template class Chara<EnemyTask>;

} // namespace MP

// inferred: warp points of the field where a player of team `team` can appear (by game mode)
// NONMATCHING: 98.90% - registers of the loop (scene, offset and warp point)
extern "C" int fn_804FC7E0(PPManager2_unk* mgr, atn::FixedArray_unk<gs::WarpPoint*, 32>* list, int team) {
    if (mgr->mGame == 0 || mgr->mGame->m10D8 == 0 || (mgr->mGame->m10D8->mFlags & 2) != 2) {
        return 0;
    }
    gs::WarpPoint* point;
    GameScene* scene = mgr->mGame->m10D8;
    for (unsigned int i = 0; i < scene->m78.mSize; i++) {
        int mode = mgr->mMode;
        point = &scene->m78.mData[i];
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
                }
                if (team == 1) {
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

