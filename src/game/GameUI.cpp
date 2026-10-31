#include "game/GameUI.h"
#include "atn/D2A_Task.h"
#include "atn/D2A_TaskInstance.h"

// inferred: d2anime task with an end frame (GameUI 0x3EC); names invented
struct GameUIDuration_unk : public atn::D2A_Task {
    float fn_803CFC6C(); // inferred: returns the end frame
};
// inferred: gauges of the HUD (GameUI 0x83C, 0x840): d2anime tasks (frame 0x100, end frame 0xA0)
struct GameUIGauge_unk : public atn::D2A_Task {};
#include "game/CharaTask.h"
#include "game/GimmickCommon.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/SkillEffectManager.h"
#include "atn/GraphicsDevice.h"
#include "game/HitInfo_unk.h"
#include "atn/SoundRef.h"
#include "string.h"
#include "game/TacView.h"
#include "atn/DrawHelper.h"
#include "atn/Camera_unk.h"
#include "atn/D2A_TaskInstanceC.h"
#include "atn/Color4f_unk.h"
#include "game/SetConfig.h"
#include "game/Input_unk.h"
#include "game/GameConfig_unk.h"
#include "game/FriendManager.h"
#include "game/BattleCharaTask.h"
#include "std/vector.h"
extern "C" int sprintf(char* s, const char* format, ...);

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
// inferred: sounds of the HUD ("SE_UI001_012", ...); the table is in the auto .rodata before the split
extern "C" const char* const lbl_80750618[14];

// inferred: field (lbl_8087F430) and its route table at 0x10D8
struct GameUIField_unk {
    unsigned char m00[0x10D8];
    atn::Task* m10D8;
};
extern "C" GameUIField_unk* lbl_8087F430;
extern "C" int fn_80370C2C(GameUIField_unk* field);
extern "C" void fn_80370BD0(GameUIField_unk* field, int frames, int arg, float speed);
extern "C" void fn_80370B78(GameUIField_unk* field, unsigned int color, int frames, int arg, float speed);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" void fn_800D58A4(atn::Texture_unk* tex); // inferred: releases a render texture
extern "C" void fn_803B78E8(void* self); // inferred: closes the window (game/GameChoiceWindow.cpp)
extern "C" void fn_803B57B0(int window, int enabled); // inferred: game/GameChoiceWindow.cpp
extern "C" void fn_803B6C88(GameUIWindow_unk* window, int arg);
extern "C" void fn_803B6970(GameUIWindow_unk* win, void* msg, int arg, float time); // inferred: shows a message
extern "C" int fn_803CC900(atn::Task* table); // inferred: number of entries
extern "C" void fn_803CFC78(GameUI* ui);   // inferred: creates the parts of the HUD
extern "C" int fn_8021A984(HitInfo_unk* hit); // inferred: the hit heals
extern "C" int fn_8021A9CC(HitInfo_unk* hit); // inferred: kind test
extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" SkillEffectManager* lbl_8087F9E8;
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void* lbl_8087EFB4; // inferred: render system (camera)
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" void fn_801FECE0(atn::D2A_Layout_unk* layout, unsigned int hash, float value);
extern "C" void fn_801FEE08(atn::D2A_Layout_unk* layout, unsigned int hash, const wchar_t* text);
// inferred: message table (lbl_8087F1E4, as in game/MenuCommonTask.cpp)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct GameUIMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    // inferred: same table seen as {id, text} entries (weak copy 0x803D7084); name invented
    struct Entry_unk {
        int mId;
        const wchar_t* mText;
    };
    const wchar_t* GetEntry_unk(int id) {
        const wchar_t* text = ((Entry_unk*)this)[id].mText;
        return text != 0 ? text : lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" GameUIMsgTable_unk* lbl_8087F1E4;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: state checks of CharaTask written with a result flag (as in game/BattleCharaTask.cpp)
static inline bool IsActiveB_unk(CharaTask* c) {
    bool r = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        r = true;
    }
    return r;
}
static inline bool IsFreeB_unk(CharaTask* c) {
    bool r = false;
    if (IsActiveB_unk(c) && (c->mStatus.m0C & 0x20) != 0x20) {
        r = true;
    }
    return r;
}
static inline bool IsGrappledB_unk(CharaTask* c) {
    bool r = false;
    if (c->m55C == 6 && c->m560 == 28) {
        r = true;
    }
    return r;
}
static inline bool CanActB_unk(CharaTask* c) {
    bool r = false;
    if (IsFreeB_unk(c) && !IsGrappledB_unk(c)) {
        r = true;
    }
    return r;
}
// inferred: item data returned by fn_80211480 (only the members used here; names invented)
struct GameUIItem_unk {
    unsigned char m00[0x64];
    int m64;          // inferred: 2 selects the plural messages
    unsigned char m68[0xB4 - 0x68];
    int mB4;          // inferred: kind of the message (0: the item name)
    float mB8;        // inferred: value (tenths) of kind 11
    short mBC;
    unsigned char mBE[0xC2 - 0xBE];
    signed char mC2;  // inferred: rarity
};
extern "C" GameUIItem_unk* fn_80211480(int id);
// inferred: item data of an id, null for 0 (name invented)
inline GameUIItem_unk* GetItem_unk(int id) {
    return id > 0 ? fn_80211480(id) : 0;
}
extern "C" void fn_80444CF8(wchar_t* text, int item, int count, int rare); // inferred: item name
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...);       // inferred: wide sprintf
extern "C" void* lbl_8087F610;           // inferred: multiplayer session
extern "C" void fn_804DA478(void* session); // inferred: notifies the session
extern "C" void fn_801FED24(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value);
// inferred: sets one component of a layout parameter by name (name invented)
inline void SetLayoutText_unk(atn::D2A_Layout_unk* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetLayoutValue_unk(atn::D2A_Layout_unk* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
// inferred: effect manager (atn/EffectManager.cpp); only the field used here
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int group, int flag); // inferred: stops the effects of owner

// inferred: bit `i` of `mask` is set (name invented)
inline bool __IsBit_unk(unsigned int mask, int i) {
    return ((1 << i) & mask) == (1 << i);
}

#define GAMEUI_CHECK(m, off) typedef char __check_##m[(int)&((GameUI*)0)->m == off ? 1 : -1]
GAMEUI_CHECK(m2F0, 0x2F0);
GAMEUI_CHECK(m30C, 0x30C);
GAMEUI_CHECK(m3D8, 0x3D8);
GAMEUI_CHECK(m3F0, 0x3F0);
GAMEUI_CHECK(m574, 0x574);
GAMEUI_CHECK(m748, 0x748);
GAMEUI_CHECK(m7B8, 0x7B8);
GAMEUI_CHECK(m814, 0x814);
GAMEUI_CHECK(m844, 0x844);
GAMEUI_CHECK(m8D0, 0x8D0);
GAMEUI_CHECK(mA84, 0xA84);
GAMEUI_CHECK(mB38, 0xB38);
GAMEUI_CHECK(mC38, 0xC38);
GAMEUI_CHECK(mD40, 0xD40);
GAMEUI_CHECK(mD90, 0xD90);
GAMEUI_CHECK(mDD0, 0xDD0);
GAMEUI_CHECK(mF14, 0xF14);
GAMEUI_CHECK(m10B4, 0x10B4);
GAMEUI_CHECK(m1158, 0x1158);
GAMEUI_CHECK(m1174, 0x1174);
GAMEUI_CHECK(m15BC, 0x15BC);
GAMEUI_CHECK(m253C, 0x253C);
GAMEUI_CHECK(m2568, 0x2568);
GAMEUI_CHECK(m2610, 0x2610);
GAMEUI_CHECK(m2664, 0x2664);
GAMEUI_CHECK(m2678, 0x2678);
GAMEUI_CHECK(m288C, 0x288C);

GameUI* lbl_8087F490;
int lbl_8087F494;     // inferred: the game runs at 60 frames per second (else 30)

// inferred: frames to whole seconds, rounded up past the first (60 or 30 frames per second)
extern "C" int fn_803CE854(int frames) {
    if (lbl_8087F494 != 0) {
        return frames / 60 + 1;
    }
    return frames / 30 + 1;
}

// inferred: frames within the current second (in 1/30 s units)
extern "C" int fn_803CE8A8(int frames) {
    if (lbl_8087F494 != 0) {
        return (frames % 60) / 2;
    }
    return frames % 30;
}

GameUI* GameUI::fn_803CE910(atn::Task* parent) {
    if (parent != 0 && lbl_8087F490 == 0) {
        lbl_8087F490 = new ((MemoryTag)1, "", "", 0) GameUI(parent);
    }
    return lbl_8087F490;
}

// NONMATCHING: 98.1% - scheduling only: (1) the original keeps the load of the list count (Clear of 0x30C)
// after the six resets of m318, here it is hoisted above them (the scheduling windows of the block end one
// node earlier); (2) the zero of the resets after the 0x5BC loop is materialised twice in the original (r3
// up to mB08, r30 from mB20 on, across the memset) and once here (r30). Wrapper structs with constructors
// (W262C_unk, W25CC_unk, W2568_unk, MarkerList_unk) were needed for the scheduling windows and the loop
// bounds of the arrays; `m30C.mMid.mBase.mCount -= (int)...` reproduces the lwz/subf of the inlined Clear.
GameUI::GameUI(atn::Task* parent)
    : atn::Task(parent), m48(0.0f), m4C(-1.0f), m834(0), mD84(0), mD88(0),
      mD8C(0), mDA4(0), mDA8(0), mDAC(0), mDB0(0), mDC8(0), m1118(0), m1130(0), m1140(0), m1144(0), m1148(0), m114C(0), m1150(0), m1154(0), m1170(0), m15B4(0), m15B8(0),
      m253C(0), m2540(0), m254C(0), m255C(-1), m2560(0.0f), m2658(0), mSeState(0), mSeMask(0), m2668(1.0f), m266C(0.07f), m2670(5.0f), m2674(3000.0f),
      m2678(128, 128, 128, 255), m2688(10, 10, 128, 255), m2698(128, 128, 128, 255), m26A8(10, 10, 255, 255), m26B8(0, 0, 0, 255), m26C8(19, 59, 23, 255), m26D8(220, 220, 200, 255), m26E8(250, 150, 90, 255), m26F8(140, 170, 220, 255), m2708(60, 100, 70, 255), m2718(250, 240, 130, 255), m2728(255, 180, 0, 0), m2738(0, 180, 80, 0), m2748(255, 100, 100, 0), m2758(90, 90, 90, 0), m2768(50, 20, 90, 0), m2778(150, 100, 200, 0), m288C(255, 255, 255, 0), m289C(85, 112, 206, 255) {
    m2788.mCount = 0;
    for (int i = 0; i < 6; i++) {
        m318[i].mInst[0] = 0;
        m318[i].mInst[1] = 0;
        m318[i].mInst[2] = 0;
        m318[i].mInst[3] = 0;
        m318[i].mInst[4] = 0;
    }
    m30C.mMid.mBase.mCount -= (int)m30C.mMid.mBase.mCount;
    m3D8 = 0;
    m3DC[0] = 0;
    m3DC[1] = 0;
    m3DC[2] = 0;
    m3DC[3] = 0;
    m3EC = 0;
    m3F0.mCount = 0;
    mDCC = 1;
    mDD0 = 0.0f;
    for (int i = 0; i < 32; i++) {
        mF14[i] = 0;
        mF94[i] = 0;
        m1014[i] = i;
    }
    for (int i = 0; i < 8; i++) {
        m10B4[i] = 0;
        m10D4[i] = 0;
    }
    m10F4 = 0;
    for (int i = 0; i < 8; i++) {
        m574[i] = 0;
        m594[i] = 0;
    }
    m5B8 = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 5; j++) {
            m5BC[i][j] = 0;
        }
    }
    m6FC[0] = 0;
    m6FC[1] = 0;
    m6FC[2] = 0;
    m6FC[3] = 0;
    m6FC[4] = 0;
    m6FC[5] = 0;
    m714[0] = 0;
    m71C[0] = 0;
    m714[1] = 0;
    m71C[1] = 0;
    m724 = 0;
    m728 = 0;
    m72C = 0;
    m730 = 0;
    m734 = 0;
    m738 = 0;
    for (int i = 0; i < 12; i++) {
        m8A0[i] = 0;
    }
    mA84 = 0;
    mA88 = 0;
    m8D0.mCount = 0;
    m73C = 0;
    m740 = 0;
    m744 = 0;
    m7B8 = 0;
    m7BC = 0;
    for (int i = 0; i < 4; i++) {
        m814[i * 2] = 0;
    }
    for (int i = 0; i < 4; i++) {
        m814[i * 2 + 1] = 0;
    }
    m834 = 0;
    m844 = 0;
    m85C.mCount = 0;
    m870 = 0;
    for (int i = 0; i < 5; i++) {
        m848[i] = 0;
    }
    for (int i = 0; i < 5; i++) {
        m874[i] = 0;
    }
    m838 = 0;
    m263C = 0;
    m2640 = 0;
    mD78 = 0;
    mD7C = 5;
    mD80 = 0;
    m10F8 = 0;
    m10FC = 0;
    m1100 = 64.0f;
    for (int i = 0; i < 6; i++) {
        mB08[i] = 0;
    }
    mB20[0] = 0;
    mB20[1] = 0;
    mB20[2] = 0;
    mB20[3] = 0;
    mB20[4] = 0;
    mB20[5] = 0;
    for (int i = 0; i < 8; i++) {
        mB38[i].m00 = 0;
        mB38[i].m04 = 0;
    }
    mD6C = 0;
    m264C = 0x2D;
    m2654 = 0;
    m2644 = 0;
    m2648 = 0;
    m2650 = 0;
    m1104 = 0;
    m1108 = 0;
    m110C = 0;
    m1110 = 0;
    m1114 = 0;
    m1120 = 0;
    m1124 = 0;
    m1128 = 0;
    m1130 = 0;
    m2544 = 0;
    m2548 = 0;
    mDB8 = 0;
    mDBC = 0;
    mA8C = 0;
    for (int i = 0; i < 10; i++) {
        mA90[i] = 0;
        mAB8[i] = 0;
        mAE0[i] = 0;
    }
    mDB4 = 0;
    memset(m2550, 0, sizeof(m2550));
    m2564 = 0;
    m2568.m00 = 0;
    m25CC.m00 = 0;
    for (int i = 0; i < 4; i++) {
        m2610[i] = 0;
    }
    m2620 = 0;
    m2624 = 0;
    m2628 = 0;
    mDC0 = 0;
    mDC4 = 0;
    fn_803CF740();
    fn_803CF74C();
}

void GameUICounter_unk::fn_803CF734() {
    mCount = 0;
}

void GameUI::fn_803CF740() {
    mC38.mCount = 0;
}

void GameUI::fn_803CF74C() {
    mD40.m00 = 0;
    mD40.m10 = 0;
    mD40.mPos = lbl_807C7030;
    mD40.m14 = 0;
    mD40.m18 = 0;
    mD40.m1C = 0.0f;
    mD3C = 0;
}

void GameUIListBase_unk::Clear_unk() {
    mCount -= end_unk() - begin_unk();
}


GameUI::~GameUI() {
    lbl_8087F490 = 0;
}

int GameUI::vf0C() {
    if (fn_800D3FA4() != 0) {
        fn_803CFC78(this);
        return 1;
    }
    return 0;
}

void GameUI::vf10() {
    fn_803E668C();
    if (m840 != 0 && !m840->IsFlag_unk(4) && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0 &&
        lbl_8087F430->m10D8->IsFlag_unk(2) && fn_803CC900(lbl_8087F430->m10D8) > 0) {
        if (mSeState == 0) {
            if (m840->mFrame >= 4.0f) {
                fn_800C31F4(lbl_80750618[4], 0, -1, 0.6f);
                mSeState++;
            }
        } else if (mSeState % 2 == 1) {
            if (m840->mFrame < 4.0f) {
                mSeState = (mSeState + 1) % 14;
            }
        } else if (mSeState % 2 == 0) {
            if (m840->mFrame >= 4.0f) {
                mSeState = (mSeState + 1) % 14;
            }
        }
    } else if (m83C == 0 || m83C->IsFlag_unk(4)) {
        mSeState = 0;
    }
    for (GameUIEntry_unk* e = m3F0.begin(); e != m3F0.end();) {
        e->mFrames++;
        if (m3EC == 0 || e->mFrames >= m3EC->fn_803CFC6C()) {
            e->m00 = 0;
            e->m08 = 0;
            m3F0.erase(e);
        } else {
            e++;
        }
    }
    if (mSeMask != 0) {
        for (int i = 0; i < 14; i++) {
            if (((1 << i) & mSeMask) == (1 << i)) {
                unsigned int mask = 1 << i;
                float volume = 1.0f;
                if (i == 4) {
                    volume = 0.6f;
                } else if (i == 13) {
                    volume = 2.0f;
                }
                fn_800C31F4(lbl_80750618[i], 0, -1, volume);
                mSeMask &= ~mask;
            }
        }
    }
}

float GameUIDuration_unk::fn_803CFC6C() {
    return mData.mEndFrame;
}

void GameUI::vf14() {}

// inferred: releases the parts of the HUD (called by game/GameTask)
void GameUI::fn_803D11B4() {
    if (mDC8 != 0) {
        mDC8 = 0;
        fn_800D58A4(&mDD4);
        fn_800D58A4(&mE04);
        fn_800D58A4(&mE34);
        fn_800D58A4(&mE64);
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 5; j++) {
                fn_800D2338(m5BC[i][j]);
                m5BC[i][j] = 0;
            }
        }
        for (int i = 0; i < 10; i++) {
            fn_800D2338(mA90[i]);
            mA90[i] = 0;
            fn_800D2338(mAB8[i]);
            mAB8[i] = 0;
            fn_800D2338(mAE0[i]);
            mAE0[i] = 0;
        }
        for (int i = 0; i < 32; i++) {
            fn_800D2338(mF14[i]);
            mF14[i] = 0;
            mF94[i] = 0;
            m1014[i] = i;
        }
        for (int i = 0; i < 8; i++) {
            fn_800D2338(m10B4[i]);
            m10B4[i] = 0;
            m10D4[i] = 0;
        }
        fn_800D2338(m10F4);
        m10F4 = 0;
        fn_800D2338(m10F8);
        m10F8 = 0;
        fn_800D2338(m844);
        m844 = 0;
        fn_800D2338(m870);
        m870 = 0;
        fn_800D2338(m848[0]);
        m848[0] = 0;
        for (int i = 0; i < 4; i++) {
            fn_800D2338(m848[i + 1]);
            m848[i + 1] = 0;
        }
        fn_800D2338(m874[0]);
        m874[0] = 0;
        for (int i = 0; i < 4; i++) {
            fn_800D2338(m874[i + 1]);
            m874[i + 1] = 0;
        }
        fn_800D2338(mB08[0]);
        mB08[0] = 0;
        fn_800D2338(mB08[1]);
        mB08[1] = 0;
        fn_800D2338(mB08[2]);
        mB08[2] = 0;
        fn_800D2338(mB08[3]);
        mB08[3] = 0;
        fn_800D2338(mB08[4]);
        mB08[4] = 0;
        fn_800D2338(mB08[5]);
        mB08[5] = 0;
        fn_800D2338(mB20[0]);
        mB20[0] = 0;
        fn_800D2338(mB20[1]);
        mB20[1] = 0;
        for (int i = 0; i < 4; i++) {
            fn_800D2338(mB20[i + 2]);
            mB20[i + 2] = 0;
        }
        fn_800D2338(m1104);
        m1104 = 0;
        fn_800D2338(m1108);
        m1108 = 0;
        fn_800D2338(m1110);
        m1110 = 0;
        fn_800D2338(m1114);
        m1114 = 0;
        m111C = 0;
        m112C = 0;
        fn_800D2338(m1140);
        m1140 = 0;
        fn_800D2338(m1144);
        m1144 = 0;
        fn_800D2338(m1148);
        m1148 = 0;
        m1158.Destroy();
        m1164.Destroy();
        for (int i = 0; i < 4; i++) {
            m1554[i].Destroy();
            m1584[i].Destroy();
        }
        fn_800D2338(m2544);
        m2544 = 0;
        fn_800D2338(m2548);
        m2548 = 0;
        fn_800D2338(m254C);
        m254C = 0;
        for (int i = 0; i < 3; i++) {
            if (m2550[i] != 0) {
                fn_800D2338(m2550[i]);
                m2550[i] = 0;
            }
        }
        if (m2564 != 0) {
            fn_800D2338(m2564);
            m2564 = 0;
        }
        for (int i = 0; i < 4; i++) {
            if (m2610[i] != 0) {
                fn_800D2338(m2610[i]);
                m2610[i] = 0;
            }
        }
        if (m2628 != 0) {
            fn_800D2338(m2628);
            m2628 = 0;
        }
        if (mDC4 != 0) {
            fn_800D2338(mDC4);
            mDC4 = 0;
        }
    }
}

// inferred: nonzero while the resources of the HUD are loading (called by game/GameTask)
int GameUI::fn_803D1574() {
    int busy = 0;
    if (mDC8 != 0) {
        // inferred: the textures are seen through GimmickUvOwner_unk, as in the other callers of fn_800D59B8
        if (((GimmickUvOwner_unk*)&mDD4)->fn_800D59B8() || ((GimmickUvOwner_unk*)&mE04)->fn_800D59B8() ||
            ((GimmickUvOwner_unk*)&mE34)->fn_800D59B8() || ((GimmickUvOwner_unk*)&mE64)->fn_800D59B8()) {
            busy = 1;
        }
        for (int i = 0; i < 4; i++) {
            if (m1554[i].IsLoading() || m1584[i].IsLoading()) {
                busy = 1;
            }
        }
    }
    if (!fn_800D3FA4()) {
        busy = 1;
    }
    return busy;
}

// ---- objects read by GameUI::fn_803D2A6C (views; names invented) ----
extern "C" unsigned int fn_80372574(const void* array); // inferred: size of a fixed array (count at +0, weak copy of game/GameTask.cpp)

// inferred: layout parameters of a d2anime task by name (declared inline in atn/D2A_Task.h)
extern "C" void* fn_801FEB9C(atn::D2A_Layout_unk* layout, const char* name); // inferred: track of a value
inline void* atn::D2A_Task::GetTrack_unk(const char* name) {
    return fn_801FEB9C(&mData.m10, name);
}
inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, float value) {
    fn_801FECE0(&mData.m10, fn_800DC6B4(name), value);
}
inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, int index, float value) {
    fn_801FED24(&mData.m10, fn_800DC6B4(name), index, value);
}
inline void atn::D2A_Task::SetLayoutText_unk(const char* name, const wchar_t* text) {
    fn_801FEE08(&mData.m10, fn_800DC6B4(name), text);
}

// inferred: keys of a layout track (8-byte entries after a header word); weak copy 0x803D7270
struct GameUITrackKey_unk {
    unsigned char m00[8];
};
extern "C" float fn_801F0544(GameUITrackKey_unk* key, int frame); // inferred: value of a key
struct GameUITrack_unk {
    float GetValue_unk(int frame, int index) { return fn_801F0544(&mKeys[index], frame); }
    int m00;
    GameUITrackKey_unk mKeys[1];
};

// inferred: scene lighting set by the HUD while the player is in danger (0x24 bytes, game/GameTask.cpp
// copies it at +0x374 of the scene, 0x80366DAC); weak constructor 0x803D6E3C
struct GameUIFog_unk {
    GameUIFog_unk() : m00(0), m04(0), mColor(1.0f, 1.0f, 1.0f, 1.0f), m18(0.0f), m1C(0.0f), m20(0.0f) {}
    int m00;
    int m04;
    atn::Color4f_unk mColor;
    float m18;
    float m1C;
    float m20;
};
struct GameUIScene_unk;
extern "C" GameUIScene_unk* fn_802F0990();                // inferred: returns lbl_8087EFA8
extern "C" float fn_802F0988(GameUIScene_unk* scene);     // inferred: frame time (+0x3A4)
extern "C" void fn_80366DAC(GameUIScene_unk* scene, GameUIFog_unk fog);
extern "C" GameUIFog_unk* fn_80366DA4(GameUIScene_unk* scene);

// inferred: field (lbl_8087F430); fn_80121F00 returns it
struct GameUIRoute_unk {
    unsigned char m00[0x2C];
    int m2C;
    bool HasM2C_unk() const { return m2C != 0; } // inferred: weak copy 0x803D7044
};
struct GameUIPlayerInput_unk {
    Camera_unk* GetCamera_unk() { return &mCamera; } // inferred: weak copy 0x803D6EEC
    Camera_unk mCamera;
    unsigned char m00[0x7FC - sizeof(Camera_unk)];
    int m7FC;
    int GetMode_unk() const { return m7FC; } // inferred: weak copy 0x803D6F70 (5, 6: no HUD)
};
struct Field_unk : public atn::Task {
    atn::Task* fn_8013C504(); // inferred: returns its route table (accessor of another file)
    int GetState_unk() const { return m54E4; }      // inferred: weak copy 0x803D6E24
    GameUIRoute_unk* GetRoute_unk() { return &m550C; } // inferred: weak copy 0x803D7058
    unsigned char m48[0x54E4 - 0x48];
    int m54E4;
    unsigned char m54E8[0x550C - 0x54E8];
    GameUIRoute_unk m550C;
};
extern "C" Field_unk* fn_80121F00();                               // inferred: returns lbl_8087F430
extern "C" GameUIPlayerInput_unk* fn_80122550(Field_unk* field);   // inferred: field + 0x6C
extern "C" int fn_80370174(Field_unk* field, int flag);            // inferred: value of a game flag
extern "C" int fn_803761A4(Field_unk* field);                      // inferred: always 1
extern "C" int fn_80244CA0(atn::Task* table);                      // inferred: (flags >> 1) & 1

// inferred: multiplayer session (lbl_8087F610)
struct GameUISession_unk {
    unsigned char m00[0x4FC];
    int m4FC;
    unsigned char m500[0x50C - 0x500];
    int m50C;
    // inferred: weak copies 0x803D6DF4, 0x803D6E1C, 0x803D7060; names invented
    bool IsWaiting_unk() const {
        bool r = false;
        if (m4FC == 30 && m50C < 5) {
            r = true;
        }
        return r;
    }
    int GetM50C_unk() const { return m50C; }
    bool IsState30_unk() const { return m4FC == 30; }
};
extern "C" GameUISession_unk* fn_800F7F90();                                 // inferred: returns lbl_8087F610
extern "C" int fn_804EB010(GameUISession_unk* session, CharaTask* chara);   // inferred
extern "C" int fn_804EB7C4(GameUISession_unk* session, CharaTask* chara);   // inferred

// inferred: settings behind lbl_8087F0A8 (game/BattleTask.cpp, game/CharaTask.cpp)
struct GameUISettingsFlags_unk {
    int m00; // inferred: show the voice messages
    int m04;
    unsigned char m08[0x10 - 0x08];
    int m10; // inferred: show the gauges of the friends
};
struct Settings_unk {
    Input_unk* fn_80116BAC(); // inferred: pad (this + 0x48C)
    GameUISettingsFlags_unk* GetFlags_unk() { return &m470; } // inferred: weak copy 0x803D6F60
    unsigned char m00[0x470];
    GameUISettingsFlags_unk m470;
};
extern "C" Settings_unk* fn_800F52F8();          // inferred: returns lbl_8087F0A8
extern "C" void* fn_801231B0(Settings_unk* s);    // inferred: settings + 0x244
inline SetConfig* GetSetConfig_unk() {
    return SetConfig::sInstance;
}
extern "C" void* lbl_8087EF70; // inferred: pad
inline void* GetPad_unk() {
    return lbl_8087EF70;
}
extern "C" int fn_800A58D0(void* pad, int port); // inferred: second stick available

// inferred: other managers
struct GameUIEventMgr_unk;
extern "C" GameUIEventMgr_unk* fn_8000DB1C();               // inferred: returns lbl_8087F540
extern "C" int fn_80481654(GameUIEventMgr_unk* mgr);        // inferred: no event is running
extern "C" CharaManager<PlayerTask>* fn_8000D9E8();         // inferred: returns lbl_8087F8A0
extern "C" CharaTask* fn_8000DCF4(CharaManager<PlayerTask>* mgr); // inferred: the leader
extern "C" CharaTask* fn_802A36B0(CharaManager<PlayerTask>* mgr); // inferred: first character
extern "C" CharaTask* fn_802A4094(CharaTask* chara);        // inferred: next character (+0x14AC)
extern "C" int fn_8000D9F8(CharaTask* chara);               // inferred: character id (+0x50)
extern "C" EnemyManager* fn_801A03E0();                     // inferred: EnemyManager::sInstance
extern "C" CharaTask* fn_801A0408(EnemyManager* mgr);       // inferred: first enemy
extern "C" CharaTask* fn_801A03EC(CharaTask* chara);        // inferred: next enemy (+0x14AC)
extern "C" float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b); // inferred: dot product
extern "C" FriendManager* fn_80323C2C();                    // inferred: FriendManager::sInstance
extern "C" int fn_80267B20(CharaTask* chara);               // inferred: returns m55C
extern "C" int fn_80267B28(CharaTask* chara);               // inferred: returns m560
extern "C" int fn_8038B648(CharaTask* chara);               // inferred: returns +0xC48
extern "C" int fn_80339F04(CharaTask* chara);               // inferred: !(m54C bit 3)
extern "C" CharaC58_unk* fn_800F530C(CharaTask* chara);     // inferred: chara + 0xC58
extern "C" atn::ColliCapsule* fn_80198C00(CharaTask* chara); // inferred: chara + 0x5B8
// inferred: shape of the collision capsule; its centre is at +0xC
struct GameUICapsule_unk {
    unsigned char m00[0xC];
    atn::Vector3 m0C;
};
extern "C" GameUICapsule_unk* fn_80315644(atn::ColliCapsule* colli); // inferred: colli + 0x3C
// inferred: control of the leader while aiming (chara + 0xF80)
struct GameUIAimControl_unk {
    void SetTarget_unk(CharaTask* chara) { m50 = chara; } // inferred: weak copy 0x803D6FD4
    unsigned char m00[0x50];
    CharaTask* m50;
};
extern "C" GameUIAimControl_unk* fn_80318F90(CharaTask* chara);
extern "C" void* fn_8008B964();                             // inferred: returns lbl_8087EFB4 (camera)
extern "C" float fn_800F8524(const float* a, const float* b, float t); // inferred: lerp of two floats
extern "C" float lbl_8087DEE8;
extern "C" float lbl_8087DEEC;
extern "C" float fn_802A7964(float rad);  // inferred: to degrees
extern "C" float fn_802A7910(float deg);  // inferred: wraps an angle
extern "C" void* lbl_8087F048;
inline void* fn_8013A194() {
    return lbl_8087F048;
}
extern "C" void fn_8010D320(void* obj, atn::Vector3 pos); // inferred: aims at a position
// inferred: names of the places (db/MapLink_unk.cpp)
extern "C" const wchar_t* fn_802168BC(int place);
extern "C" int fn_80216908(int place);
extern "C" void* lbl_8087EEC8; // inferred: font
inline void* GetFont_unk() {
    return lbl_8087EEC8;
}
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float space, int a, int b); // inferred: width
extern "C" void fn_800E0000(std::vector<std::basic_string<wchar_t> >* lines, const wchar_t* text, float width,
                            float size, float space, int a, int b, int c); // inferred: word wrap
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" double sin(double x);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" const wchar_t lbl_8078C638[]; // inferred: wide literals of the file ("[%s]", "??????", "")
extern "C" int fn_803CE854(int frames); // inferred: minutes of a time
extern "C" int fn_803CE8A8(int frames); // inferred: seconds of a time
inline TacView* GetTacView_unk() { // inferred: weak copy 0x803D7184
    return lbl_8087F9F8;
}
inline GameUIMsgTable_unk* GetMsgTable_unk() { // inferred: weak copy 0x803D70A0
    return lbl_8087F1E4;
}
inline void* GetGraphics_unk() { // inferred: weak copy 0x803D71C4
    return lbl_8087EEE0;
}
inline float SinF_unk(float x) { // inferred: weak copy 0x803D6E7C
    return sin(x);
}
// inferred: candidate target of the aimed bow (16 bytes); names invented
struct GameUITarget_unk {
    GameUITarget_unk() : mChara(0) {}
    GameUITarget_unk(atn::Vector3 pos, CharaTask* chara) : mPos(pos), mChara(chara) {}
    atn::Vector3 mPos;
    CharaTask* mChara;
};

// NONMATCHING: written from the asm; the original is over the inlining budget and calls every inline
// member out of line (the weak copies 0x803D2134-0x803D218C, 0x803D2A54/0x803D2A60 and
// 0x803D6DE4-0x803D7840 come from here)
void GameUI::fn_803D2A6C() {
    // inferred: stops (freezes) every part of the HUD; the ones that are paused are also rewound
    if (m50.m18->IsFrozen_unk()) {
        m50.m18->Rewind_unk();
    }
    if (m50.m1C != 0 && m50.m1C->IsFrozen_unk()) {
        m50.m1C->Rewind_unk();
    }
    if (m50.m20->IsFrozen_unk()) {
        m50.m20->Rewind_unk();
    }
    m50.m0C->FreezeTask();
    m50.m10->FreezeTask();
    if (m50.m14 != 0) {
        m50.m14->FreezeTask();
    }
    m50.m18->FreezeTask();
    if (m50.m1C != 0) {
        m50.m1C->FreezeTask();
    }
    m50.m20->FreezeTask();
    m50.m24->FreezeTask();
    m50.m28->FreezeTask();
    m50.m2C->FreezeTask();
    for (int i = 0; i < 6; i++) {
        if (mB0[i].m18->IsFrozen_unk()) {
            mB0[i].m18->Rewind_unk();
        }
        if (mB0[i].m1C != 0 && mB0[i].m1C->IsFrozen_unk()) {
            mB0[i].m1C->Rewind_unk();
        }
        if (mB0[i].m20->IsFrozen_unk()) {
            mB0[i].m20->Rewind_unk();
        }
        mB0[i].m0C->FreezeTask();
        mB0[i].m10->FreezeTask();
        if (mB0[i].m14 != 0) {
            mB0[i].m14->FreezeTask();
        }
        mB0[i].m18->FreezeTask();
        if (mB0[i].m1C != 0) {
            mB0[i].m1C->FreezeTask();
        }
        mB0[i].m20->FreezeTask();
        mB0[i].m2C->FreezeTask();
        for (int j = 0; j < 5; j++) {
            m318[i].mInst[j]->FreezeTask();
        }
    }
    for (int i = 0; i < 8; i++) {
        m574[i]->FreezeTask();
        m594[i]->FreezeTask();
    }
    m5B8->FreezeTask();
    m3D8->FreezeTask();
    for (int i = 0; i < 4; i++) {
        m3DC[i]->FreezeTask();
    }
    for (int i = 0; i < 6; i++) {
        if (m6FC[i]->IsFrozen_unk()) {
            m6FC[i]->Rewind_unk();
        }
        m6FC[i]->FreezeTask();
    }
    m714[0]->FreezeTask();
    m71C[0]->FreezeTask();
    m714[1]->FreezeTask();
    m71C[1]->FreezeTask();
    m72C->FreezeTask();
    m730->FreezeTask();
    for (int i = 0; i < 12; i++) {
        if (m8A0[i]->IsFrozen_unk()) {
            m8A0[i]->Rewind_unk();
        }
        m8A0[i]->FreezeTask();
    }
    mA84->FreezeTask();
    mA88->FreezeTask();
    m73C->FreezeTask();
    m740->FreezeTask();
    m744->FreezeTask();
    m7B8->FreezeTask();
    m7BC->FreezeTask();
    for (int i = 0; i < 2; i++) {
        m814[i]->FreezeTask();
        m814[i + 2]->FreezeTask();
        m814[i + 4]->FreezeTask();
        if (m814[i + 6] != 0) {
            m814[i + 6]->FreezeTask();
        }
    }
    m83C->FreezeTask();
    if (m840->IsFrozen_unk()) {
        m840->Rewind_unk();
    }
    m840->FreezeTask();
    ((GameUICounter_unk*)&m85C)->fn_803CF734();
    if (m844 != 0) {
        m844->FreezeTask();
    }
    if (m870 != 0) {
        if (m870->IsFrozen_unk()) {
            m870->Rewind_unk();
        }
        m870->FreezeTask();
    }
    if (m848[0] != 0) {
        atn::D2A_Task* mark = static_cast<atn::D2A_Task*>(m848[0]);
        if (mark->IsFrozen_unk()) {
            mark->Rewind_unk();
        }
        mark->FreezeTask();
        for (int i = 0; i < 4; i++) {
            static_cast<GameUIInst_unk*>(m848[i + 1])->Rewind_unk();
            m848[i + 1]->FreezeTask();
        }
        m874[0]->FreezeTask();
        for (int i = 0; i < 4; i++) {
            static_cast<GameUIInst_unk*>(m874[i + 1])->Rewind_unk();
            m874[i + 1]->FreezeTask();
        }
    }
    for (int i = 0; i < 8; i++) {
        mB38[i].m00->FreezeTask();
        mB38[i].m04->FreezeTask();
        mB38[i].m08->FreezeTask();
        mB38[i].m0C->FreezeTask();
        mB38[i].m10->FreezeTask();
        mB38[i].m14->FreezeTask();
        mB38[i].m18->FreezeTask();
        mB38[i].m1C->FreezeTask();
    }
    m2654->FreezeTask();
    mD70->FreezeTask();
    mD74->FreezeTask();
    mDB4->FreezeTask();
    mDB8->FreezeTask();
    mDBC->FreezeTask();
    mDC0->FreezeTask();
    if (IsNoMenu_unk() && (fn_800F7F90() == 0 || fn_800F7F90()->IsWaiting_unk())) {
        bool resultShown = false;
        if (fn_80121F00()->GetState_unk() == 8 && fn_8000DB1C() != 0 && fn_80481654(fn_8000DB1C()) != 0) {
            resultShown = true;
        }
        bool eventShown = false;
        if (fn_80121F00()->GetState_unk() == 8 && (fn_8000DB1C() == 0 || fn_80481654(fn_8000DB1C()) == 0)) {
            eventShown = true;
        }
        CharaTask* leader = fn_8000DCF4(fn_8000D9E8());
        GameUIPlayerInput_unk* input = fn_80122550(fn_80121F00());
        if (resultShown) {
            fn_803E4100();
            for (unsigned int i = 0; i < fn_80372574(&m8D0); i++) {
                m8A0[i]->MeltTask();
                fn_801F6D7C(m8A0[i], "base_point", 0, m8D0[i].mScreen.x);
                fn_801F6D7C(m8A0[i], "base_point", 1, m8D0[i].mScreen.y);
                fn_801F6D7C(m8A0[i], "base_scale", 2, 0.01f * m8D0[i].mSize.x);
                fn_801F6D7C(m8A0[i], "base_scale", 3, 0.01f * m8D0[i].mSize.y);
            }
            ((GameUICounter_unk*)&m8D0)->fn_803CF734();
        } else if (eventShown || fn_80121F00()->GetState_unk() == 0) {
            // inferred: scene colour while the leader is weak
            if (fn_80370174(fn_80121F00(), 0x2B) == 1) {
                GameUIFog_unk fog;
                fog.m00 = 1;
                fog.m18 = 1.0f;
                fog.mColor.Set_unk(0.2f, 0.13f, 0.08f, 1.0f);
                fog.m20 = -0.2f;
                fog.m1C = 0.0f;
                fn_80366DAC(fn_802F0990(), fog);
            } else if (!fn_80121F00()->GetFlag1_unk()) {
                float rate = leader->GetStatus_unk()->m04 / (float)leader->GetStatus_unk()->mParamB.m84;
                if (fn_80267B20(leader) == 6 && fn_80267B28(leader) == 0x1C) {
                    rate = 0.0f;
                }
                float step = fn_800F8524(&lbl_8087DEE8, &lbl_8087DEEC, 0.5f * (rate * rate + rate)) - m4C;
                if (step > 0.0f) {
                    step = step < 0.05f * fn_802F0988(fn_802F0990()) ? step : 0.05f * fn_802F0988(fn_802F0990());
                    m4C += step;
                } else if (step < 0.0f) {
                    step = step > -0.04f * fn_802F0988(fn_802F0990()) ? step : -0.04f * fn_802F0988(fn_802F0990());
                    m4C += step;
                }
                m4C = 1.0f < (-1.0f > m4C ? -1.0f : m4C) ? 1.0f : (-1.0f > m4C ? -1.0f : m4C);
                GameUIFog_unk fog = *fn_80366DA4(fn_802F0990());
                fog.m00 = !(m4C < -0.8f);
                m48 += fn_802F0988(fn_802F0990());
                if (m48 >= 60.0f) {
                    m48 -= 60.0f;
                }
                float wave = SinF_unk(6.2831855f * (m48 / 60.0f));
                fog.m18 = atn::DegToRad_unk(0.05f * wave + 1.6f);
                fog.mColor.Set_unk(0.0f, 0.3f, 0.3f, 1.0f);
                fog.m1C = 0.0f;
                fog.m20 = 0.05f * wave + m4C;
                fn_80366DAC(fn_802F0990(), fog);
            }

            // inferred: characters near the leader (the nearest one gets the stick arrow)
            float nearest = 1e16f;
            unsigned int nearestIndex = 0;
            if (leader->fn_80179FA8()) {
                for (CharaTask* c = fn_801A0408(fn_801A03E0()); c != 0; c = fn_801A03EC(c)) {
                    if (m85C.full()) {
                        break;
                    }
                    if (!c->GetStatus_unk()->fn_80139F4C() && c->CanAct_unk() && !c->GetFlag54C13_unk() &&
                        c->GetFlag26_unk() && c->fn_8013A158()) {
                        atn::Vector3 pos(*leader->fn_8013C38C());
                        pos.y += leader->fn_8013C42C();
                        float best = 1e16f;
                        atn::SimpleVector<CharaTask::Item14_unk>* spheres = c->GetSpheres_unk();
                        int bestSphere = -1;
                        for (unsigned int j = 0; j < spheres->size(); j++) {
                            atn::Vector3 d = (*spheres)[j].m04.mCenter - pos;
                            float dy = 0.0f > atn::Abs_unk(d.y) - 60.0f ? 0.0f : atn::Abs_unk(d.y) - 60.0f;
                            float dist = atn::Vector3(d.x, dy, d.z).Length_unk() - (*spheres)[j].m04.mRadius;
                            if (d.LengthSqXZ_unk() > 0.001f) {
                                atn::Vector3 dir(d.x, 0.0f, d.z);
                                dir.Normalize_unk();
                                dist += 6.0f * (1.0f - fn_801A03E8(dir, leader->fn_8014052C()));
                            }
                            if (dist < 60.0f) {
                                if (nearest > dist) {
                                    nearest = dist;
                                    nearestIndex = fn_80372574(&m85C);
                                }
                                if (c == m764.m0C) {
                                    nearest = 0.0f;
                                    nearestIndex = fn_80372574(&m85C);
                                }
                                if (dist < best) {
                                    best = dist;
                                    bestSphere = j;
                                }
                            }
                        }
                        if (bestSphere >= 0) {
                            m85C.PushBack_unk(c);
                        }
                    }
                }
            }
            if (!m85C.empty()) {
                switch (m764.m00) {
                case 1:
                    m764.m00 = 14;
                case 10:
                    if (m870 != 0) {
                        m870->MeltTask();
                        atn::Vector3* from = leader->fn_8013C38C();
                        atn::Vector3 dir = *m85C[nearestIndex]->fn_8013C38C() - *from;
                        dir.Normalize_unk();
                        dir.fn_80139F60();
                        float rot = fn_80122550(fn_80121F00())->GetCamera_unk()->GetRot_unk()->y - dir.y;
                        rot = fn_802A7910(fn_802A7964(rot));
                        float cur = ((GameUITrack_unk*)m870->GetTrack_unk("stickRot"))->GetValue_unk(0, 0);
                        rot = fn_800F8524(&cur, &rot, 0.25f);
                        m870->SetLayoutValue_unk("stickRot", rot);
                    }
                    break;
                }
            }

            // inferred: action buttons
            bool changed = false;
            if (m764.m00 != m748.m00) {
                changed = true;
                memcpy(&m780, &m748, sizeof(GameUIElem1C_unk));
                if (m764.m00 == 6 && m764.m04 == 2) {
                    mSeMask |= 0x20;
                }
            }
            if (m764.m00 >= 0 && m764.m04 != 12 && m764.m04 != 13) {
                fn_803E3468(m73C, m744, &m764, 0);
            }
            if (m780.m00 >= 0 && m780.m04 != 12 && m780.m04 != 13) {
                fn_803E3468(m740, 0, &m780, 0);
            }
            if (changed) {
                m73C->Rewind_unk();
                m740->Rewind_unk();
                if (m814[m764.m18]->IsEnd_unk()) {
                    m814[m764.m18]->Rewind_unk();
                }
            }
            bool changed2 = false;
            if (m7C0.m00 != m7F8.m00) {
                changed2 = true;
                memcpy(&m7DC, &m7F8, sizeof(GameUIElem1C_unk));
            }
            bool locked = false;
            if (m7C0.m00 >= 0) {
                fn_803E3468(m7B8, 0, &m7C0, 0);
                if (fn_800F52F8()->GetFlags_unk()->m04 != 0 && m7C0.m00 == 0x11) {
                    locked = true;
                    if (m83C->IsEnd_unk()) {
                        m840->MeltTask();
                    } else {
                        if (m83C->GetFrame_unk() <= 0.0f) {
                            m83C->SetFrame_unk(1.0f);
                            if (fn_80121F00() != 0 && fn_80121F00()->fn_8013C504() != 0 &&
                                fn_80244CA0(fn_80121F00()->fn_8013C504()) &&
                                fn_803CC900(fn_80121F00()->fn_8013C504()) > 0) {
                                mSeState = 1;
                                mSeMask |= 0x10;
                            }
                        }
                        m83C->MeltTask();
                    }
                }
            }
            if (!locked) {
                if (--m838 < 0) {
                    m838 = 0;
                    m83C->Rewind_unk();
                }
            } else {
                m838 = 90;
            }
            if (m7DC.m00 >= 0) {
                fn_803E3468(m7BC, 0, &m7DC, 0);
            }
            if (changed2) {
                m7B8->Rewind_unk();
                m7BC->Rewind_unk();
            }

            if (input->GetMode_unk() != 6 && input->GetMode_unk() != 5) {
                // inferred: marker over the target of the action
                if (m764.m00 == 10 && m844 != 0 && m764.m0C != 0) {
                    m844->MeltTask();
                    MtxPtr mtx = m764.m0C->GetModel_unk()->GetNodeMtx_unk("head");
                    atn::Vector3 pos(mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk()
                                              : fn_80315644(fn_80198C00(m764.m0C))->m0C);
                    pos.y += 2.0f;
                    atn::Vector3 screen;
                    fn_800BFAC8(&screen, fn_8008B964(), &pos);
                    if (screen.z >= 0.0f && screen.z <= 1.0f) {
                        atn::D2A_Task* mark = static_cast<atn::D2A_Task*>(m848[0]);
                        mark->MeltTask();
                        if (mark->IsEnd_unk()) {
                            mark->SetFrame_unk(5.0f);
                        }
                        mark->SetLayoutValue_unk("base_point", 0, screen.x);
                        mark->SetLayoutValue_unk("base_point", 1, screen.y);
                    }
                }
                // inferred: markers over the other characters near the leader
                if (leader->fn_8013C3A8() && fn_8038B648(leader) == 0 && !leader->fn_80139ED0() &&
                    leader->fn_8013C458() != 0 && !leader->fn_8013C458()->fn_8013C43C() &&
                    fn_803761A4(fn_80121F00())) {
                    for (unsigned int i = 0; i < fn_80372574(&m85C); i++) {
                        if (m85C[i] != m764.m0C) {
                            MtxPtr mtx = m85C[i]->GetModel_unk()->GetNodeMtx_unk("head");
                            atn::Vector3 pos(mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk()
                                                      : fn_80315644(fn_80198C00(m85C[i]))->m0C);
                            pos.y += 2.0f;
                            atn::Vector3 screen;
                            fn_800BFAC8(&screen, fn_8008B964(), &pos);
                            if (screen.z >= 0.0f && screen.z <= 1.0f) {
                                GameUIInst_unk* mark = static_cast<GameUIInst_unk*>(m848[i + 1]);
                                mark->MeltTask();
                                fn_801F6D7C(mark, "base_point", 0, screen.x);
                                fn_801F6D7C(mark, "base_point", 1, screen.y);
                            }
                        }
                    }
                }
                // inferred: targets of the bow while aiming
                if (fn_80267B20(leader) == 6 && fn_80267B28(leader) == 0xF) {
                    GameUIAimControl_unk* aim = fn_80318F90(leader);
                    aim->SetTarget_unk(0);
                    atn::Vector3 base(leader->GetF60_unk()[1]);
                    FixedArray_unk<GameUITarget_unk, 5> targets;
                    for (CharaTask* c = fn_801A0408(fn_801A03E0()); c != 0; c = fn_801A03EC(c)) {
                        if (c->CanAct_unk()) {
                            atn::Vector3 pos;
                            if (c->vfB0(&pos)) {
                                targets.PushBack_unk(GameUITarget_unk(atn::Vector3(pos), c));
                            }
                            if (targets.full()) {
                                break;
                            }
                        }
                    }
                    float best = 900.0f;
                    int bestIndex = -1;
                    for (unsigned int i = 0; i < fn_80372574(&targets); i++) {
                        atn::Vector3 pos(targets[i].mPos);
                        float d = (base - pos).LengthSqXZ_unk();
                        float dy = base.y - pos.y;
                        if (dy > -40.0f && dy < 80.0f && d < best) {
                            bestIndex = i;
                            best = d;
                        }
                    }
                    if (bestIndex >= 0) {
                        atn::D2A_Task* mark = static_cast<atn::D2A_Task*>(m874[0]);
                        mark->MeltTask();
                        atn::Vector3 screen;
                        fn_800BFAC8(&screen, fn_8008B964(), &targets[bestIndex].mPos);
                        mark->SetLayoutValue_unk("base_point", 0, screen.x);
                        mark->SetLayoutValue_unk("base_point", 1, screen.y);
                        aim->SetTarget_unk(targets[bestIndex].mChara);
                    }
                    int count = 0;
                    for (unsigned int i = 0; i < fn_80372574(&targets); i++) {
                        if (bestIndex != i) {
                            GameUIInst_unk* mark = static_cast<GameUIInst_unk*>(m874[count + 1]);
                            mark->MeltTask();
                            atn::Vector3 screen;
                            fn_800BFAC8(&screen, fn_8008B964(), &targets[i].mPos);
                            fn_801F6D7C(mark, "base_point", 0, screen.x);
                            fn_801F6D7C(mark, "base_point", 1, screen.y);
                            count++;
                        }
                        if (count >= 4) {
                            break;
                        }
                    }
                }
            }
            fn_803E0E10(leader, &m50, -1);

            // inferred: button prompts
            if (m728 != 0) {
                void* pad = fn_801231B0(fn_800F52F8());
                int type = GetSetConfig_unk()->GetShakeType_unk();
                if (m724 == 0) {
                    m714[0]->SetEnd_unk();
                    m71C[0]->Rewind_unk();
                    m714[1]->SetEnd_unk();
                    m71C[1]->Rewind_unk();
                    m714[type]->Rewind_unk();
                    fn_800C31F4(lbl_80750618[0], 0, -1, 1.0f);
                }
                if (!m714[type]->IsEnd_unk()) {
                    m714[type]->MeltTask();
                } else {
                    m71C[type]->MeltTask();
                }
                m724++;
            } else {
                m724 = 0;
            }
            m728 = 0;
            if (m738 != 0) {
                ButtonUv_unk uv(0.0f, 0.0f, 0.0f, 0.0f);
                uv = fn_800F52F8()->fn_80116BAC()->fn_80124C6C(0);
                {
                    ButtonUv_unk uvCopy1(uv);
                    m72C->SetParam2_unk("btnUv", &uvCopy1.u0);
                }
                {
                    ButtonUv_unk uvCopy2(uv);
                    m72C->SetParam2_unk("btnUvMask", &uvCopy2.u0);
                }
                {
                    ButtonUv_unk uvCopy3(uv);
                    m730->SetParam2_unk("btnUv", &uvCopy3.u0);
                }
                {
                    ButtonUv_unk uvCopy4(uv);
                    m730->SetParam2_unk("btnUvMask", &uvCopy4.u0);
                }
                if (m734 == 0) {
                    m730->Rewind_unk();
                    m72C->Rewind_unk();
                    fn_800C31F4(lbl_80750618[0], 0, -1, 1.0f);
                }
                if (m72C->GetFrame_unk() < 5.0f) {
                    m72C->MeltTask();
                } else {
                    m730->MeltTask();
                }
                m734++;
            } else {
                m734 = 0;
            }
            m738 = 0;

            // inferred: gauge over the leader
            atn::Vector3 head;
            atn::Vector3 headScreen;
            CharaTask* player = fn_8000DCF4(fn_8000D9E8());
            MtxPtr mtx = player->GetModel_unk()->GetNodeMtx_unk("head");
            head = mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : fn_80315644(fn_80198C00(player))->m0C;
            head.y += 5.0f;
            atn::Vector3 tmp;
            fn_800BFAC8(&tmp, fn_8008B964(), &head);
            headScreen = tmp;
            if ((!player->GetStatus_unk()->fn_80139F4C() || player->CanAct_unk()) && player->HasPart648_unk() &&
                GetSetConfig_unk()->GetPlayerHpBarShow_unk() != 0) {
                m5B8->MeltTask();
                m5B8->SetLayoutValue_unk("base_point", 0, headScreen.x);
                m5B8->SetLayoutValue_unk("base_point", 1, headScreen.y);
                m5B8->SetLayoutValue_unk("hpRate", player->GetStatus_unk()->m04 /
                                                       (float)player->GetStatus_unk()->mParamB.m84);
                m5B8->SetLayoutValue_unk("hpColR", 130.0f);
                m5B8->SetLayoutValue_unk("hpColG", 208.0f);
                m5B8->SetLayoutValue_unk("hpColB", 80.0f);
                unsigned int heal = player->fn_801799BC();
                if ((heal & 1) == 1 || (heal & 4) == 4 || (heal & 8) == 8) {
                    m5B8->SetLayoutValue_unk("heal", 1.0f);
                } else {
                    m5B8->Rewind_unk();
                    m5B8->SetLayoutValue_unk("heal", 0.0f);
                }
            }

            // inferred: windows of the party
            if (input->GetMode_unk() != 6 || fn_800F7F90() != 0) {
                if (fn_80121F00() != 0 && !fn_80121F00()->GetRoute_unk()->HasM2C_unk()) {
                    fn_803DF648(1);
                }
                for (unsigned int i = 0; i < fn_80372574(&m2F0); i++) {
                    GameUIElem60_unk* e = m2F0[i];
                    CharaTask* c = (CharaTask*)e->m08;
                    fn_803E0E10(c, m2F0[i], i);
                    if (fn_800F7F90() != 0 &&
                        (fn_800F7F90()->GetM50C_unk() != 0 || !fn_800F7F90()->IsState30_unk())) {
                        if (m318[i].m1C >= 0) {
                            m318[i].mInst[m318[i].m1C]->SetEnd_unk();
                            m318[i].m1C = -1;
                        }
                    } else {
                        if (c->GetStatus_unk()->fn_80139F4C() && m318[i].m1C >= 0) {
                            if (!m318[i].mInst[1]->IsEnd_unk()) {
                                m318[i].mInst[1]->SetEnd_unk();
                                m318[i].m1C = -1;
                            }
                            if (!m318[i].mInst[0]->IsEnd_unk()) {
                                m318[i].mInst[0]->SetEnd_unk();
                                m318[i].m1C = -1;
                            }
                        }
                        if (fn_800F52F8()->GetFlags_unk()->m00 != 0) {
                            int msg = m318[i].m1C;
                            if (msg >= 0) {
                                GameUIInst_unk* inst = m318[i].mInst[msg];
                                inst->MeltTask();
                                fn_801F6C80(inst, "line", (float)(int)i);
                                if (msg == 0) {
                                    int time = (int)(c->GetFBC_unk() - c->GetFB8_unk());
                                    int sec = fn_803CE8A8(time);
                                    int min = fn_803CE854(time);
                                    if (min < 0) {
                                        min = 0;
                                    }
                                    fn_801F8598(inst, "num", min, 0);
                                    fn_801F8598(inst, "num01", min, 0);
                                    fn_801F8598(inst, "num02", min, 0);
                                    float frame = inst->GetFrame_unk();
                                    float end = fn_801F6C2C(inst);
                                    if (c->GetFB8_unk() <= 0.0f && frame < end) {
                                        inst->SetEnd_unk();
                                    } else if (frame >= 44.0f) {
                                        inst->SetFrame_unk(75.0f - (float)sec);
                                    }
                                }
                                if (inst->IsEnd_unk()) {
                                    m318[i].m1C = -1;
                                }
                            }
                            for (unsigned int j = 0; j < m30C.mMid.mBase.Size_unk(); j++) {
                                if (m30C.mMid.mBase.At_unk(j)->m04 == c) {
                                    int k = m30C.mMid.mBase.At_unk(j)->m00;
                                    m318[i].m1C = k;
                                    m318[i].mInst[k]->Rewind_unk();
                                    fn_801F837C(m318[i].mInst[k], "voiceMsg", GetMsgTable_unk()->GetEntry_unk(k + 0xE3));
                                    if (k == 1) {
                                        mSeMask |= 0x40;
                                    }
                                }
                            }
                        } else {
                            m318[i].m1C = -1;
                        }
                        if (e->m04 != 0) {
                            e->m00 = e->m00 + 1 < 20 ? e->m00 + 1 : 20;
                        } else {
                            int v = e->m00 - 1;
                            e->m00 = v < 0 ? 0 : v;
                        }
                        if (e->m04 == 0 && e->m00 <= 0) {
                            e->m08 = 0;
                        }
                    }
                }
                m30C.mMid.mBase.Clear_unk();
            }

            // inferred: gauges of the bosses
            if (input->GetMode_unk() != 6) {
                int n = 0;
                for (CharaTask* c = fn_801A0408(fn_801A03E0()); c != 0; c = fn_801A03EC(c)) {
                    CharaStatus* st = c->GetStatus_unk();
                    float rate = st->m04 / (float)st->mParamB.m84;
                    if (c->GetFlag22_unk()) {
                        bool show = true;
                        if (c->IsFrozen_unk() || c->GetStatus_unk()->fn_80139F4C() || c->GetFlag54C13_unk() ||
                            fn_800F530C(c)->mC0 == 0) {
                            show = false;
                        }
                        if (!show && static_cast<BattleCharaTask*>(c)->m146C == 5 && fn_80121F00() != 0) {
                            if (fn_8000D9F8(c) == 0x49A85 &&
                                (fn_80370174(fn_80121F00(), 0x4BA) == 30 || fn_80370174(fn_80121F00(), 0x4C2) == 14)) {
                                show = true;
                            }
                            if (fn_80370174(fn_80121F00(), 0x4BA) == 31 || fn_80370174(fn_80121F00(), 0x4C2) == 15) {
                                show = true;
                            }
                        }
                        if (fn_800F52F8()->GetFlags_unk()->m00 == 0) {
                            show = false;
                        }
                        if (show) {
                            if (n == 0) {
                                m3D8->MeltTask();
                                m3D8->SetLayoutText_unk("boss_name", (const wchar_t*)c->GetInfo_unk()->m08); // inferred: wide name
                                m3D8->SetLayoutValue_unk("frameOn", 1.0f);
                            }
                            if (n == 1) {
                                m3D8->SetLayoutValue_unk("frameOn", 0.0f);
                            }
                            m3DC[n]->MeltTask();
                            fn_801F6C80(m3DC[n], "hpRate", rate);
                            if (st->mParamB.m8C > 0.0f && rate < 1.0f) {
                                fn_801F6C80(m3DC[n], "heal", 1.0f);
                            } else {
                                fn_801F6C80(m3DC[n], "heal", 0.0f);
                            }
                            n++;
                        }
                    }
                }
            }

            // inferred: marks of the targets
            if (!m8D0.empty() && fn_8013A194() != 0) {
                fn_8010D320(fn_8013A194(), atn::Vector3(m8D0[0].mPos));
            }
            for (unsigned int i = 0; i < fn_80372574(&m8D0); i++) {
                if (m8D0[i].mScreen.x >= -200.0f && 1000.0f >= m8D0[i].mScreen.x && m8D0[i].mScreen.y >= -200.0f &&
                    1000.0f >= m8D0[i].mScreen.y && m8D0[i].mScreen.z >= 0.0f && 1.0f >= m8D0[i].mScreen.z) {
                    m8A0[i]->MeltTask();
                    fn_801F6D7C(m8A0[i], "base_point", 0, m8D0[i].mScreen.x);
                    fn_801F6D7C(m8A0[i], "base_point", 1, m8D0[i].mScreen.y);
                    fn_801F6D7C(m8A0[i], "base_scale", 2, 0.01f * m8D0[i].mSize.x);
                    fn_801F6D7C(m8A0[i], "base_scale", 3, 0.01f * m8D0[i].mSize.y);
                }
            }
            ((GameUICounter_unk*)&m8D0)->fn_803CF734();
            if (!mA84->IsEnd_unk()) {
                mA84->MeltTask();
            }
            if (!mA88->IsEnd_unk()) {
                atn::Vector3 a(m888);
                fn_800BFAC8(&tmp, fn_8008B964(), &a);
                a = tmp;
                atn::Vector3 b(m894);
                fn_800BFAC8(&tmp, fn_8008B964(), &b);
                b = tmp;
                atn::Vec2_unk center(0.5f * (a.x + b.x), 0.5f * (a.y + b.y));
                atn::Vec2_unk size(0.5f * atn::Abs_unk(a.x - b.x), 0.5f * atn::Abs_unk(a.y - b.y));
                size.x += 12.0f;
                size.y += 12.0f;
                mA88->MeltTask();
                mA88->SetLayoutValue_unk("base_point", 0, center.x);
                mA88->SetLayoutValue_unk("base_point", 1, center.y);
                mA88->SetLayoutValue_unk("base_scale", 2, 0.01f * size.x);
                mA88->SetLayoutValue_unk("base_scale", 3, 0.01f * size.y);
            }

            // inferred: button hint
            if (mD8C > 0 && mD84 == 0) {
                mD70->Rewind_unk();
                mD74->Rewind_unk();
                mD78 = 0;
                mD84 = 1;
                mD88 = mD8C;
            }
            if (mD84 != 0) {
                ButtonUv_unk uv(0.0f, 0.0f, 0.0f, 0.0f);
                int stick = fn_800A58D0(GetPad_unk(), 0);
                wchar_t text[0x20];
                memset(text, 0, sizeof(text));
                switch (mD88) {
                case 1:
                    wcscpy(text, GetMsgTable_unk()->GetEntry_unk(0x1C3));
                    uv = fn_800F52F8()->fn_80116BAC()->fn_80124C6C(0);
                    break;
                case 2:
                    wcscpy(text, GetMsgTable_unk()->GetEntry_unk(0x1C4));
                    uv = fn_800F52F8()->fn_80116BAC()->fn_80124C6C(8);
                    break;
                }
                mD70->SetLayoutText_unk("string0001(1)", text);
                mD70->SetLayoutText_unk("string0002(1)", text);
                mD74->SetLayoutText_unk("string0001(1)", text);
                mD74->SetLayoutText_unk("string0002(1)", text);
                {
                    ButtonUv_unk uvCopy5(uv);
                    mD70->SetParam2_unk("btnUv", &uvCopy5.u0);
                }
                {
                    ButtonUv_unk uvCopy6(uv);
                    mD70->SetParam2_unk("btnUvMask", &uvCopy6.u0);
                }
                {
                    ButtonUv_unk uvCopy7(uv);
                    mD74->SetParam2_unk("btnUv", &uvCopy7.u0);
                }
                {
                    ButtonUv_unk uvCopy8(uv);
                    mD74->SetParam2_unk("btnUvMask", &uvCopy8.u0);
                }
                float end = ((GameUIDuration_unk*)mD74)->fn_803CFC6C() - 1.0f;
                if (mD74->GetFrame_unk() >= end) {
                    mD74->Rewind_unk();
                    mD78++;
                }
                if (mD78 < mD7C || mD80 != 0) {
                    if (mD70->GetFrame_unk() <= 7.0f) {
                        mD70->MeltTask();
                    }
                    if (mD70->GetFrame_unk() >= 7.0f) {
                        mD74->MeltTask();
                    }
                } else {
                    if (mD70->GetFrame_unk() <= 7.0f) {
                        mD70->SetFrame_unk(6.0f);
                    }
                    float last = ((GameUIDuration_unk*)mD70)->fn_803CFC6C();
                    if (mD70->GetFrame_unk() < last) {
                        mD70->MeltTask();
                    } else {
                        mD84 = 0;
                        mD88 = 0;
                        mD80 = 0;
                        mD78 = 0;
                        mD74->Rewind_unk();
                    }
                    mD8C = 0;
                }
            }

            // inferred: access points
            if (input->GetMode_unk() != 6 && input->GetMode_unk() != 5) {
                for (int i = 0; i < 8; i++) {
                    if (i < (int)fn_80372574(&mC38)) {
                        atn::D3AnimeInstance* open = 0;
                        atn::D3AnimeInstance* loop = 0;
                        GameUIMarker_unk* mk = &mC38[i];
                        float frame = 0.0f;
                        atn::Vector3 dir = *fn_80122550(fn_80121F00())->GetCamera_unk()->GetPosition_unk() - mk->mPos;
                        float dist = dir.Length_unk();
                        dir.Normalize_unk();
                        if (dist < 7.0f) {
                            continue;
                        }
                        switch (mk->m00) {
                        case 5:
                            open = mB38[i].m00;
                            loop = mB38[i].m04;
                            frame = fn_801F6C2C((GameUIInst_unk*)open->GetTask());
                            break;
                        case 2:
                            open = mB38[i].m08;
                            loop = mB38[i].m0C;
                            frame = 5.0f;
                            break;
                        case 1:
                        case 3:
                            open = mB38[i].m18;
                            loop = mB38[i].m1C;
                            frame = 5.0f;
                            break;
                        case 4:
                            open = mB38[i].m10;
                            loop = mB38[i].m14;
                            frame = 5.0f;
                            break;
                        }
                        if (open != 0 && loop != 0) {
                            GameUIInst_unk* openInst = (GameUIInst_unk*)open->GetTask();
                            if (mk->m18 != 0) {
                                if (mk->m00 == 5) {
                                    ((GameUIInst_unk*)loop->GetTask())->SetLoop_unk(0);
                                    float end = fn_801F6C2C((GameUIInst_unk*)loop->GetTask());
                                    if (((GameUIInst_unk*)loop->GetTask())->GetFrame_unk() < end) {
                                        loop->MeltTask();
                                    }
                                    ((GameUIInst_unk*)open->GetTask())->Rewind_unk();
                                } else {
                                    if (((GameUIInst_unk*)open->GetTask())->GetFrame_unk() < frame) {
                                        ((GameUIInst_unk*)open->GetTask())->SetFrame_unk(frame);
                                    }
                                    open->MeltTask();
                                    ((GameUIInst_unk*)loop->GetTask())->Rewind_unk();
                                }
                            } else if (mk->m14 != 0) {
                                if (mk->m00 == 5) {
                                    ((GameUIInst_unk*)loop->GetTask())->SetLoop_unk(0);
                                    float end = fn_801F6C2C((GameUIInst_unk*)loop->GetTask());
                                    if (((GameUIInst_unk*)loop->GetTask())->GetFrame_unk() < end) {
                                        loop->MeltTask();
                                    }
                                } else {
                                    float end = fn_801F6C2C((GameUIInst_unk*)open->GetTask());
                                    if (((GameUIInst_unk*)open->GetTask())->GetFrame_unk() < end) {
                                        open->MeltTask();
                                    }
                                    ((GameUIInst_unk*)loop->GetTask())->Rewind_unk();
                                }
                            } else {
                                if (mk->m00 != 5 && ((GameUIInst_unk*)open->GetTask())->GetFrame_unk() > frame) {
                                    ((GameUIInst_unk*)open->GetTask())->Rewind_unk();
                                }
                                unsigned int active = fn_80372574(&mC38);
                                for (unsigned int j = 0; j < fn_80372574(&mC38); j++) {
                                    if (mC38[j].m18 != 0) {
                                        active--;
                                    }
                                }
                                if (mk->m00 == 5 && mD6C > 0 && mD6C >= (int)active &&
                                    0.0f == ((GameUIInst_unk*)open->GetTask())->GetFrame_unk()) {
                                    ((GameUIInst_unk*)open->GetTask())->SetFrame_unk(frame);
                                }
                                if (((GameUIInst_unk*)open->GetTask())->GetFrame_unk() < frame) {
                                    if (m814[0]->IsEnd_unk() || m814[0]->IsFrozen_unk()) {
                                        open->MeltTask();
                                    }
                                } else if (((GameUIInst_unk*)open->GetTask())->GetFrame_unk() >= frame) {
                                    ((GameUIInst_unk*)loop->GetTask())->SetLoop_unk(1);
                                    loop->MeltTask();
                                }
                            }
                            (void)openInst;
                        }
                        atn::Vector3 pos = mk->mPos + dir * 3.0f;
                        dir.y = 0.0f;
                        dir.Normalize_unk();
                        if (loop != 0) {
                            atn::Matrix3x4* rotm = loop->GetRotation_unk();
                            atn::Vector3 up = atn::Vector3(0.0f, 0.0f, 1.0f).Transform_unk(*rotm);
                            dir = up + dir;
                            dir.Normalize_unk();
                            dir = up + dir;
                            dir.Normalize_unk();
                        }
                        atn::Vector3 rot = dir.ToRotation_unk();
                        atn::Vector3 near(m2668, m2668, m2668);
                        atn::Vector3 far(m266C, m266C, m266C);
                        float range = m2670 > (m2674 < dist ? m2674 : dist) ? m2670 : (m2674 < dist ? m2674 : dist);
                        float t = range / (m2674 - m2670);
                        atn::Vector3 scale(1.0f, 1.0f, 1.0f);
                        scale = atn::Lerp_unk(far, near, t);
                        if (open != 0) {
                            open->SetPosition_unk(pos);
                            atn::Matrix3x4 m;
                            m.fn_8013A13C(rot.y);
                            open->SetRotation_unk(m);
                            open->SetScale_unk(scale);
                            fn_801F6D7C((GameUIInst_unk*)open->GetTask(), "basePos", 0, 0.0f);
                            fn_801F6D7C((GameUIInst_unk*)open->GetTask(), "basePos", 1, 0.0f);
                            fn_801F6D7C((GameUIInst_unk*)open->GetTask(), "basePos", 4, range);
                            fn_801F6C80((GameUIInst_unk*)open->GetTask(), "moveU", rot.y / 1.5707964f);
                            fn_801F6C80((GameUIInst_unk*)open->GetTask(), "moveV", rot.y / 1.5707964f);
                        }
                        if (loop != 0) {
                            loop->SetPosition_unk(pos);
                            atn::Matrix3x4 m;
                            m.fn_8013A13C(rot.y);
                            loop->SetRotation_unk(m);
                            loop->SetScale_unk(scale);
                            fn_801F6D7C((GameUIInst_unk*)loop->GetTask(), "basePos", 0, 0.0f);
                            fn_801F6D7C((GameUIInst_unk*)loop->GetTask(), "basePos", 1, 0.0f);
                            fn_801F6D7C((GameUIInst_unk*)loop->GetTask(), "basePos", 4, range);
                            fn_801F6C80((GameUIInst_unk*)loop->GetTask(), "moveU", rot.y / 1.5707964f);
                            fn_801F6C80((GameUIInst_unk*)loop->GetTask(), "moveV", rot.y / 1.5707964f);
                        }
                    } else {
                        ((GameUIInst_unk*)mB38[i].m00->GetTask())->Rewind_unk();
                        ((GameUIInst_unk*)mB38[i].m04->GetTask())->Rewind_unk();
                        ((GameUIInst_unk*)mB38[i].m08->GetTask())->Rewind_unk();
                        ((GameUIInst_unk*)mB38[i].m0C->GetTask())->Rewind_unk();
                        ((GameUIInst_unk*)mB38[i].m18->GetTask())->Rewind_unk();
                        ((GameUIInst_unk*)mB38[i].m1C->GetTask())->Rewind_unk();
                        ((GameUIInst_unk*)mB38[i].m10->GetTask())->Rewind_unk();
                        ((GameUIInst_unk*)mB38[i].m14->GetTask())->Rewind_unk();
                    }
                }
            }
            mD6C = fn_80372574(&mC38);
            for (unsigned int j = 0; j < fn_80372574(&mC38); j++) {
                if (mC38[j].m18 != 0) {
                    mD6C--;
                }
            }

            // inferred: name of the place
            if (m2644 != 0) {
                m2654->MeltTask();
                if (m2650 > m264C) {
                    m2654->SetSpeed_unk(-1.0f);
                    if (m2654->GetFrame_unk() <= 0.0f) {
                        m2644 = 0;
                    }
                } else {
                    m2654->SetSpeed_unk(1.0f);
                    if (m2654->IsEnd_unk()) {
                        m2650++;
                    }
                }
                const wchar_t* name = fn_802168BC(m2648);
                int type = fn_80216908(m2648);
                if (name != 0) {
                    float width = fn_8006EF48(GetFont_unk(), name, 20.0f, 0.0f, 1, 1);
                    m2654->SetLayoutText_unk("mapName", name);
                    m2654->SetLayoutValue_unk("strLength", width);
                    m2654->SetLayoutValue_unk("type", (float)type);
                }
            }
        }
        // inferred: description of the selected skill or item
        if (eventShown || fn_80121F00()->GetState_unk() == 0 || fn_80121F00()->GetState_unk() == 7 ||
            fn_80121F00()->GetState_unk() == 9 || fn_80121F00()->GetState_unk() == 10 ||
            fn_80121F00()->GetState_unk() == 14) {
            if (mDC0 != 0 && !mDC0->IsEnd_unk()) {
                mDC0->MeltTask();
            }
            mDB4->MeltTask();
            if (mD90.m00 != 0) {
                std::vector<std::basic_string<wchar_t> > lines;
                const wchar_t* text = 0;
                wchar_t name[0x100];
                memset(name, 0, sizeof(name));
                switch (mD90.m04) {
                case 0:
                    text = mD90.m08.data();
                    break;
                case 1:
                    if (mDA4 != 0 && mDA8 != 0) {
                        wchar_t head[0x100];
                        fn_800DD3FC(head, lbl_8078C638, mDA4->m08);
                        lines.push_back(std::basic_string<wchar_t>(head));
                        text = mDA8->m0C;
                    }
                    break;
                case 2:
                    if (mDAC != 0) {
                        if (!mDAC->vfB8(name)) {
                            fn_800DD3FC(name, lbl_8078C638 + 6);
                        }
                        text = name;
                    }
                    break;
                }
                if (text != 0) {
                    fn_800E0000(&lines, text, 100000.0f, 16.0f, 0.0f, 1, 1, 0);
                    while (lines.size() < 3) {
                        lines.push_back(std::basic_string<wchar_t>(lbl_8078C638 + 14));
                    }
                    mDB4->SetSpeed_unk(1.0f);
                    mDB4->SetLayoutText_unk("desc0", lines[0].data());
                    mDB4->SetLayoutText_unk("desc1", lines[1].data());
                    mDB4->SetLayoutText_unk("desc2", lines[2].data());
                    mDB4->SetLayoutText_unk("desc3", lbl_8078C638 + 14);
                }
                mDB0 = 2;
            } else if (mDB0 > 0) {
                mDB0--;
                mDB4->SetSpeed_unk(1.0f);
            } else {
                mDB4->SetSpeed_unk(-1.0f);
            }
            if (mDB4->GetFrame_unk() >= 10.0f) {
                mDB4->SetFrame_unk(10.0f);
            }
            mD90.m00 = 0;
        }

        if (eventShown || fn_80121F00()->GetState_unk() == 0 || fn_80121F00()->GetState_unk() == 7 ||
            fn_80121F00()->GetState_unk() == 9 || fn_80121F00()->GetState_unk() == 10 ||
            fn_80121F00()->GetState_unk() == 14) {
            // inferred: damage numbers
            if (m3EC != 0) {
                for (unsigned int i = 0; i < fn_80372574(&m3F0); i++) {
                    GameUIEntry_unk* e = &m3F0[i];
                    CharaTask* c = (CharaTask*)e->m08;
                    MtxPtr mtx = c->GetModel_unk()->GetNodeMtx_unk("head");
                    atn::Vector3 pos(mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : fn_80315644(fn_80198C00(c))->m0C);
                    pos.y += 5.0f;
                    atn::Vector3 screen;
                    fn_800BFAC8(&screen, fn_8008B964(), &pos);
                    if (0.0f < screen.z && screen.z < 1.0f) {
                        m3EC->SetFrame_unk((float)e->mFrames);
                        m3EC->SetLayoutValue_unk("base_point", 0, screen.x);
                        m3EC->SetLayoutValue_unk("base_point", 1, screen.y);
                        m3EC->SetNumber_unk("damage", e->m00, 0);
                        m3EC->SetNumber_unk("damageSub", e->m00, 0);
                        fn_801F4484(m3EC);
                        fn_801F465C(m3EC, 1);
                    }
                }
            }

            // inferred: gauges over the nearest characters
            if (GetSetConfig_unk() == 0 || GetSetConfig_unk()->GetBattleDraw_unk()[1] != 0) {
                atn::Vector3 base;
                if (fn_8000DCF4(fn_8000D9E8()) != 0) {
                    base = *fn_8000DCF4(fn_8000D9E8())->fn_8013C38C();
                }
                float dist[8];
                CharaTask* nearChara[8];
                for (int i = 0; i < 8; i++) {
                    dist[i] = 1e16f;
                    nearChara[i] = 0;
                }
                int slot = 0;
                for (CharaTask* c = fn_801A0408(fn_801A03E0()); c != 0; c = fn_801A03EC(c)) {
                    if (!c->GetStatus_unk()->fn_80139F4C() && c->CanAct_unk() && !c->GetFlag54C13_unk() &&
                        fn_800F530C(c)->mC0 != 0 && c->GetFlag26_unk() && !c->GetFlag22_unk() &&
                        (fn_800F7F90() == 0 || !fn_804EB010(fn_800F7F90(), c))) {
                        float d = (base - *c->fn_8013C38C()).LengthSq_unk();
                        d *= static_cast<BattleCharaTask*>(c)->GetM1458_unk();
                        if (fn_803E6C88((unsigned int)c)) {
                            d = 0.01f;
                        }
                        bool show = false;
                        if (GetTacView_unk() != 0 && GetTacView_unk()->GetOpen_unk() != 0) {
                            if (*GetTacView_unk()->GetCursor_unk() == 1) {
                                show = true;
                            }
                        } else if (input->GetMode_unk() != 6 || fn_800F7F90() != 0) {
                            if (d < 900.0f || (fn_80267B20(c) == 6 && fn_80267B28(c) == 0x16)) {
                                show = true;
                            }
                        }
                        if (show && d < 1e16f) {
                            dist[slot] = d;
                            nearChara[slot] = c;
                            slot = 7;
                            for (int k = 6; k >= 0; k--) {
                                if (dist[k] > dist[slot]) {
                                    slot = k;
                                }
                            }
                        }
                    }
                }
                for (CharaTask* c = fn_802A36B0(fn_8000D9E8()); c != 0; c = fn_802A4094(c)) {
                    if (!c->GetStatus_unk()->fn_80139F4C() && c->CanAct_unk() && fn_80339F04(c) && c->GetFlag26_unk() &&
                        (fn_800F7F90() == 0 || !fn_804EB010(fn_800F7F90(), c))) {
                        float d = (base - *c->fn_8013C38C()).LengthSq_unk();
                        d *= static_cast<BattleCharaTask*>(c)->GetM1458_unk();
                        bool show = false;
                        if (GetTacView_unk() != 0 && GetTacView_unk()->GetOpen_unk() != 0) {
                            if (*GetTacView_unk()->GetCursor_unk() == 1) {
                                show = true;
                            }
                        } else if ((input->GetMode_unk() != 6 || fn_800F7F90() != 0) && fn_800F7F90() != 0) {
                            if (d < 900.0f || (fn_80267B20(c) == 6 && fn_80267B28(c) == 0x16)) {
                                show = true;
                            }
                        }
                        if (show && d < 1e16f) {
                            dist[slot] = d;
                            nearChara[slot] = c;
                            slot = 7;
                            for (int k = 6; k >= 0; k--) {
                                if (dist[k] > dist[slot]) {
                                    slot = k;
                                }
                            }
                        }
                    }
                }
                atn::Vector3 head;
                atn::Vector3 screen;
                for (int i = 0; i < 8; i++) {
                    CharaTask* c = nearChara[i];
                    if (c != 0) {
                        MtxPtr mtx = c->GetModel_unk()->GetNodeMtx_unk("head");
                        head = mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : fn_80315644(fn_80198C00(c))->m0C;
                        head.y += 5.0f;
                        atn::Vector3 tmp2;
                        fn_800BFAC8(&tmp2, fn_8008B964(), &head);
                        screen = tmp2;
                        if (0.0f < screen.z && screen.z < 1.0f) {
                            GameUIInst_unk* gauge = m574[i];
                            gauge->MeltTask();
                            fn_801F6D7C(gauge, "base_point", 0, screen.x);
                            fn_801F6D7C(gauge, "base_point", 1, screen.y);
                            fn_801F6C80(gauge, "leaderOn", 0.0f);
                            fn_801F6C80(gauge, "hpRate",
                                        c->GetStatus_unk()->m244 / (float)c->GetStatus_unk()->mParamB.m84);
                            if (c->IsPlayer() || c->fn_80140518()) {
                                fn_801F6C80(gauge, "hpColR", 130.0f);
                                fn_801F6C80(gauge, "hpColG", 208.0f);
                                fn_801F6C80(gauge, "hpColB", 80.0f);
                            } else {
                                fn_801F6C80(gauge, "hpColR", 200.0f);
                                fn_801F6C80(gauge, "hpColG", 50.0f);
                                fn_801F6C80(gauge, "hpColB", 50.0f);
                                if (c->GetHitFlag7_unk()) {
                                    fn_801F6C80(gauge, "leaderOn", 1.0f);
                                }
                            }
                            unsigned int heal = c->fn_801799BC();
                            if ((heal & 1) == 1 || (heal & 4) == 4) {
                                fn_801F6C80(gauge, "heal", 1.0f);
                            } else {
                                gauge->Rewind_unk();
                                fn_801F6C80(gauge, "heal", 0.0f);
                            }
                            if (c->GetHitFlag7_unk()) {
                                m594[i]->MeltTask();
                                fn_801F6D7C(m594[i], "base_point", 0, screen.x);
                                fn_801F6D7C(m594[i], "base_point", 1, screen.y);
                            }
                        }
                    }
                }
            }

            // inferred: icons over the friends (multiplayer: also over the other players)
            if (fn_800F52F8()->GetFlags_unk()->m10 != 0) {
                unsigned int n = 0;
                atn::Vector3 hips;
                atn::Vector3 screen;
                for (CharaTask* c = fn_80323C2C()->fn_8036554C(); c != 0; c = fn_802A4094(c)) {
                    if (!c->GetFlag54C13_unk() && c->fn_80139EFC() && !c->IsFrozen_unk()) {
                        hips = ((atn::Matrix3x4*)c->GetModel_unk()->GetNodeMtx_unk("hips"))->GetTrans_unk();
                        hips.y += 2.0f;
                        atn::Vector3 tmp2;
                        fn_800BFAC8(&tmp2, fn_8008B964(), &hips);
                        screen = tmp2;
                        if (0.0f < screen.z && screen.z < 1.0f) {
                            fn_801F6D7C(m6FC[n], "base_point", 0, screen.x);
                            fn_801F6D7C(m6FC[n], "base_point", 1, screen.y);
                            if (c->GetStatus_unk()->fn_80139F4C()) {
                                m6FC[n]->MeltTask();
                                m6FC[n]->SetSpeed_unk(1.0f);
                                if (m6FC[n]->IsEnd_unk()) {
                                    m6FC[n]->SetFrame_unk(50.0f);
                                }
                                if (c->GetStatus_unk()->m224 > 0) {
                                    fn_801F837C(m6FC[n], "string0002", GetMsgTable_unk()->GetEntry_unk(0xE8));
                                    fn_801F837C(m6FC[n], "string0006(1)", GetMsgTable_unk()->GetEntry_unk(0xE8));
                                    fn_801F837C(m6FC[n], "string0007(1)", GetMsgTable_unk()->GetEntry_unk(0xE8));
                                } else {
                                    fn_801F837C(m6FC[n], "string0002", GetMsgTable_unk()->GetEntry_unk(0xE9));
                                    fn_801F837C(m6FC[n], "string0006(1)", GetMsgTable_unk()->GetEntry_unk(0xE9));
                                    fn_801F837C(m6FC[n], "string0007(1)", GetMsgTable_unk()->GetEntry_unk(0xE9));
                                }
                            }
                        }
                        n++;
                        if (n >= 6) {
                            break;
                        }
                    }
                }
                if (fn_800F7F90() != 0) {
                    for (CharaTask* c = fn_801A0408(fn_801A03E0()); c != 0; c = fn_801A03EC(c)) {
                        if (fn_804EB7C4(fn_800F7F90(), c) && fn_804EB010(fn_800F7F90(), c) && !c->GetFlag54C13_unk() &&
                            c->fn_80139EFC() && !c->IsFrozen_unk()) {
                            hips = ((atn::Matrix3x4*)c->GetModel_unk()->GetNodeMtx_unk("hips"))->GetTrans_unk();
                            hips.y += 2.0f;
                            atn::Vector3 tmp2;
                            fn_800BFAC8(&tmp2, fn_8008B964(), &hips);
                            screen = tmp2;
                            if (0.0f < screen.z && screen.z < 1.0f) {
                                fn_801F6D7C(m6FC[n], "base_point", 0, screen.x);
                                fn_801F6D7C(m6FC[n], "base_point", 1, screen.y);
                                if (c->GetStatus_unk()->fn_80139F4C()) {
                                    m6FC[n]->MeltTask();
                                    m6FC[n]->SetSpeed_unk(1.0f);
                                    if (m6FC[n]->IsEnd_unk()) {
                                        m6FC[n]->SetFrame_unk(50.0f);
                                    }
                                    if (c->GetStatus_unk()->m224 > 0) {
                                        fn_801F837C(m6FC[n], "string0002", GetMsgTable_unk()->GetEntry_unk(0xE8));
                                        fn_801F837C(m6FC[n], "string0006(1)", GetMsgTable_unk()->GetEntry_unk(0xE8));
                                        fn_801F837C(m6FC[n], "string0007(1)", GetMsgTable_unk()->GetEntry_unk(0xE8));
                                    } else {
                                        fn_801F837C(m6FC[n], "string0002", GetMsgTable_unk()->GetEntry_unk(0xE9));
                                        fn_801F837C(m6FC[n], "string0006(1)", GetMsgTable_unk()->GetEntry_unk(0xE9));
                                        fn_801F837C(m6FC[n], "string0007(1)", GetMsgTable_unk()->GetEntry_unk(0xE9));
                                    }
                                }
                            }
                            n++;
                            if (n >= 6) {
                                break;
                            }
                        }
                    }
                }
            }

            // inferred: action menu
            if (m834 != 0) {
                fn_803E3468(m814[m79C.m18 + 4], 0, &m79C, 1);
                if (m814[m79C.m18 + 4]->IsEnd_unk()) {
                    m834 = 0;
                    fn_803CF74C();
                }
            } else {
                if (m764.m00 == 6 && !IsHint1_unk()) {
                    if (m814[m764.m18]->GetFrame_unk() <= 6.0f) {
                        fn_803E3468(m814[m764.m18], 0, &m764, 1);
                    }
                    if (m814[m764.m18]->GetFrame_unk() >= 6.0f) {
                        fn_803E3468(m814[m764.m18 + 2], m814[m764.m18 + 6], &m764, 1);
                    }
                }
                if (m780.m00 == 6) {
                    if (!IsHint1_unk()) {
                        if (m814[m780.m18]->GetFrame_unk() <= 6.0f) {
                            m814[m780.m18]->SetFrame_unk(7.0f);
                        }
                        if (!IsHint1_unk()) {
                            fn_803E3468(m814[m780.m18], 0, &m780, 1);
                        }
                    }
                    if (m764.m00 != 6 && m814[m780.m18]->IsEnd_unk()) {
                        fn_803CF74C();
                    }
                }
            }
            mD60.Set_unk(0.0f, 0.0f, 0.0f);
            if (mD3C != 0) {
                atn::Vector3 pos(mD40.mPos);
                CharaTask* c = (CharaTask*)mD40.m10;
                if (c != 0) {
                    MtxPtr mtx = c->GetModel_unk()->GetNodeMtx_unk("head");
                    pos = mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() + atn::Vector3(0.0f, 5.0f, 0.0f)
                                   : *c->fn_8013C38C() + atn::Vector3(0.0f, 15.0f, 0.0f);
                }
                atn::Vector3 tmp2;
                fn_800BFAC8(&tmp2, fn_8008B964(), &pos);
                mD60 = tmp2;
            } else {
                mD60.x = 0.5f * (float)((atn::GraphicsDevice_unk*)GetGraphics_unk())->GetWidth_unk();
                mD60.y = 0.5f * (float)((atn::GraphicsDevice_unk*)GetGraphics_unk())->GetHeight_unk();
            }
            float offset = 0.0f;
            if (m764.m04 == 12 || m764.m04 == 13) {
                offset = 30.0f;
            }
            m814[m764.m18]->SetLayoutValue_unk("basePos", 0, mD60.x);
            m814[m764.m18]->SetLayoutValue_unk("basePos", 1, mD60.y + offset);
            m814[m764.m18 + 2]->SetLayoutValue_unk("basePos", 0, mD60.x);
            m814[m764.m18 + 2]->SetLayoutValue_unk("basePos", 1, mD60.y + offset);
            m814[m764.m18 + 4]->SetLayoutValue_unk("basePos", 0, mD60.x);
            m814[m764.m18 + 4]->SetLayoutValue_unk("basePos", 1, mD60.y + offset);
            if (m814[m764.m18 + 6] != 0) {
                m814[m764.m18 + 6]->SetLayoutValue_unk("basePos", 0, mD60.x);
                m814[m764.m18 + 6]->SetLayoutValue_unk("basePos", 1, mD60.y + offset);
            }

            // inferred: item notices over the leader
            if (mDB8 != 0 && !mDB8->IsEnd_unk()) {
                mDB8->MeltTask();
                CharaTask* p = fn_8000D9E8() != 0 ? fn_8000DCF4(fn_8000D9E8()) : 0;
                if (p != 0) {
                    atn::Vector3 pos = *p->fn_8013C38C() + atn::Vector3(0.0f, 20.0f, 0.0f);
                    if (p->GetPart648_unk() != 0 && p->fn_801479D8()) {
                        pos = *p->fn_8013C38C() + atn::Vector3(0.0f, 13.0f, 0.0f);
                    }
                    atn::Vector3 screen;
                    fn_800BFAC8(&screen, fn_8008B964(), &pos);
                    if (0.0f <= screen.z && screen.z <= 1.0f) {
                        mDB8->SetLayoutValue_unk("pos0001(1)", 0, screen.x);
                        mDB8->SetLayoutValue_unk("pos0001(1)", 1, screen.y);
                    }
                }
            }
            if (mDBC != 0 && !mDBC->IsEnd_unk()) {
                mDBC->MeltTask();
                CharaTask* p = fn_8000D9E8() != 0 ? fn_8000DCF4(fn_8000D9E8()) : 0;
                if (p != 0) {
                    atn::Vector3 pos = *p->fn_8013C38C() + atn::Vector3(0.0f, 20.0f, 0.0f);
                    atn::Vector3 screen;
                    fn_800BFAC8(&screen, fn_8008B964(), &pos);
                    if (0.0f <= screen.z && screen.z <= 1.0f) {
                        mDBC->SetLayoutValue_unk("pos0001(1)", 0, screen.x);
                        mDBC->SetLayoutValue_unk("pos0001(1)", 1, screen.y);
                    }
                }
            }
        }
        GameUI::Get_unk()->fn_803CF740();
    }
}

// inferred: stops the effects of m1554 (called by game/TacView)
void GameUI::fn_803D8344() {
    if (m15B8 != 0) {
        lbl_8087F3C0->mB8 = 5;
        fn_80239DAC(lbl_8087F3C0, m1554, 0, 0);
        lbl_8087F3C0->mB8 = 0;
        m15B8 = 0;
    }
}

// inferred: ring drawn under a character (textures mE34 and mE64); it spins while the tactical view points
// at the character, and `fade` goes up or down by one each frame (0..8) and widens it
// NONMATCHING: 98.7% - the original loads TacView::m48 once and compares it with 2 and then with 0 (MWCC
// folds the two compares or reloads the member); register numbers follow
void GameUI::fn_803DCA84(CharaTask* chara, float* fade) {
    bool selected = false;
    atn::Vector3 pos = ((atn::Matrix3x4*)chara->mMotion.mMtx)->GetTrans_unk();
    atn::Vector3 rot(-1.5707964f, 0.0f, 0.0f);
    float size = 3.5f * (2.0f * chara->m5B0);
    pos.y += 6.0f;
    int kind = chara->m5C->m122;
    TacView* tac = lbl_8087F9F8;
    if (tac != 0 && tac->mA70 != 0 && tac->m48 != 2 && tac->IsIdle_unk() && tac->m4C == chara &&
        tac->m15D8 == 0) {
        selected = true;
    }
    if (selected) {
        rot.y += mDD0;
        *fade += 1.0f;
    } else {
        *fade -= 1.0f;
    }
    *fade = (0.0f > (8.0f < *fade ? 8.0f : *fade)) ? 0.0f : (8.0f < *fade ? 8.0f : *fade);
    unsigned int white = 0xFFFFFF;
    bool linked = false;
    size += *fade;
    if (kind == 2 || kind == 3 || chara->mFlag22 == 1 ||
        (chara->mC58.mD4 != 0 && chara->mC58.mD4->GetChara_unk() == chara)) {
        linked = true;
    }
    unsigned int rgb = 0x0A0AFF;
    if (chara->mType == 2) {
        rgb = 0xFF3C0A;
    }
    unsigned int color;
    if (linked) {
        color = rgb | 0xFF000000;
        white |= 0xFF000000;
        size *= 1.1f;
    } else {
        color = rgb | 0x80000000;
        white |= 0x20000000;
    }
    if ((lbl_8087F9F8 == 0 || lbl_8087F9F8->m15D8 == 0) && lbl_8087F610 == 0) {
        atn::Vector2_unk uv0(0.0f, 0.0f);
        atn::Vector2_unk uv1(1.0f, 1.0f);
        atn::Vector3 scale(size, size, size);
        lbl_8087EEB0->fn_80064AC4(pos, rot, scale, uv0, uv1, white, 0, (int)&mE34, 0, -11.0f);
        lbl_8087EEB0->fn_80064AC4(pos, rot, scale, uv0, uv1, color, 0, (int)&mE64, 0, -10.0f);
    }
}

// inferred: UV rectangle of a d2anime parameter (names invented)
struct GameUIUvRect_unk {
    GameUIUvRect_unk(float u0, float v0, float u1, float v1) {
        v[0] = u0;
        v[1] = v0;
        v[2] = u1;
        v[3] = v1;
    }
    float v[4];
};
inline void SetJobUv_unk(atn::D2A_Task* task, const char* name, const GameUIUvRect_unk& r) {
    task->SetParam2_unk(name, r.v);
}
inline void SetJobColor_unk(atn::D2A_Task* task, const char* name, const GameUIColor_unk& c) {
    GameUIColor_unk tmp = c;
    task->SetColor255_unk(name, &tmp.r);
}

// inferred: colours of the HUD (0x807C8628, .bss of this file); they are set by the static initializer
// (fn_803E78B4, not written yet), so they are only declared here
extern "C" GameUIColor_unk lbl_807C8628[16];

// inferred: job icon of a character window (UV of the icon and its colours)
// NONMATCHING: 91.2% - the original keeps the colour table in r29 and the entry in r30 (one more saved
// register); tried the colour by value, by reference, with a local copy and a reference per branch
extern "C" void fn_803DD90C(atn::D2A_Task* task, int job) {
    GameUIColor_unk* colors = lbl_807C8628;
    if (job == 4) {
        SetJobUv_unk(task, "jobUv01ma", GameUIUvRect_unk(0.5f, 0.0f, 0.75f, 0.25f));
        SetJobUv_unk(task, "jobUv01mb", GameUIUvRect_unk(0.5f, 0.0f, 0.75f, 0.25f));
        SetJobColor_unk(task, "jobCol01a", colors[4]);
        SetJobColor_unk(task, "jobGradation", colors[4]);
    } else if (job == 2) {
        SetJobUv_unk(task, "jobUv01ma", GameUIUvRect_unk(0.0f, 0.0f, 0.25f, 0.25f));
        SetJobUv_unk(task, "jobUv01mb", GameUIUvRect_unk(0.0f, 0.0f, 0.25f, 0.25f));
        SetJobColor_unk(task, "jobCol01a", colors[1]);
        SetJobColor_unk(task, "jobGradation", colors[1]);
    } else if (job == 3) {
        SetJobUv_unk(task, "jobUv01ma", GameUIUvRect_unk(0.25f, 0.0f, 0.5f, 0.25f));
        SetJobUv_unk(task, "jobUv01mb", GameUIUvRect_unk(0.25f, 0.0f, 0.5f, 0.25f));
        SetJobColor_unk(task, "jobCol01a", colors[2]);
        SetJobColor_unk(task, "jobGradation", colors[2]);
    } else if (job == 1) {
        SetJobUv_unk(task, "jobUv01ma", GameUIUvRect_unk(0.75f, 0.0f, 1.0f, 0.25f));
        SetJobUv_unk(task, "jobUv01mb", GameUIUvRect_unk(0.75f, 0.0f, 1.0f, 0.25f));
        SetJobColor_unk(task, "jobCol01a", colors[0]);
        SetJobColor_unk(task, "jobGradation", colors[0]);
    } else {
        SetJobUv_unk(task, "jobUv01ma", GameUIUvRect_unk(0.75f, 0.0f, 1.0f, 0.25f));
        SetJobUv_unk(task, "jobUv01mb", GameUIUvRect_unk(0.75f, 0.0f, 1.0f, 0.25f));
        SetJobColor_unk(task, "jobCol01a", colors[5]);
        SetJobColor_unk(task, "jobGradation", colors[5]);
    }
}

// NONMATCHING: 95.5% - register allocation: the original keeps the zero of the first loop in r31 and
// reuses it as the offset into m10D4 (this in r30); here this stays in r31 (4 variants)
void GameUI::fn_803DE648() {
    if (mDC8 == 0) {
        return;
    }
    float width = lbl_8087EEE0->mWidth;
    float height = lbl_8087EEE0->mHeight;
    for (int i = 0; i < 8; i++) {
        m10D4[i] = 0;
        m10B4[i]->mFlags |= 4;
    }
    if (mDCC >= 1) {
        for (int i = 0; i < 8; i++) {
            m1094[i] = 0.0f;
        }
        return;
    }
    float limit = 15.0f;
    int count = 0;
    for (unsigned int i = 0; i < 32; i++) {
        if (count >= 8) {
            break;
        }
        Bullet_unk* bullet = lbl_8087F9E8->GetBullet_unk(i);
        HitInfo_unk* hit = bullet->GetHit_unk();
        if (bullet->IsActive_unk() && hit != 0 && fn_8021A9CC(hit) && bullet->m5C * bullet->m28 >= limit &&
            (signed char)hit->m02 != 4 && (signed char)hit->m02 != 0) {
            m10D4[count] = bullet;
            count++;
        }
    }
    for (int i = 0; i < count; i++) {
        fn_803DE89C(m10D4[i], i);
    }
}

void GameUI::fn_803DEE24(Bullet_unk* bullet, GameUIColor_unk* add, GameUIColor_unk* mul) {
    *add = GameUIColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
    *mul = GameUIColor_unk(100, 100, 100, 255);
    if (bullet != 0 && bullet->m08 != 0) {
        HitInfo_unk* hit = bullet->GetHit_unk();
        if (bullet->m08->mType == 2) {
            if (fn_8021A984(hit)) {
                *mul = m26C8;
                *add = m26B8;
            }
        } else if (fn_8021A984(hit)) {
            *mul = m26D8;
            *add = GameUIColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        } else if (hit->m74.m00 > 0.0f) {
            *mul = m26E8;
            *add = GameUIColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        } else if (hit->m74.m04 > 0.0f) {
            *mul = m26F8;
            *add = GameUIColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        } else if (hit->m74.m0C > 0.0f) {
            *mul = m2708;
            *add = GameUIColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        } else if (hit->m74.m10 > 0.0f) {
            *mul = m2718;
            *add = GameUIColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        } else {
            *mul = GameUIColor_unk(250, 150, 90, 255);
            *add = GameUIColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
}

// inferred: colour of the marker of a skill effect (by the kind of its hit data)
GameUIColor_unk GameUI::fn_803DF138(Bullet_unk* bullet) {
    if (bullet != 0 && bullet->m08 != 0 && bullet->m08->mType != 2) {
        HitInfo_unk* hit = bullet->GetHit_unk();
        if (fn_8021A984(hit)) {
            return GameUIColor_unk(220, 220, 200, 255);
        }
        if (hit->m74.m00 > 0.0f) {
            return GameUIColor_unk(250, 150, 90, 255);
        }
        if (hit->m74.m04 > 0.0f) {
            return GameUIColor_unk(140, 170, 220, 255);
        }
        if (hit->m74.m0C > 0.0f) {
            return GameUIColor_unk(60, 100, 70, 255);
        }
        if (hit->m74.m10 > 0.0f) {
            return GameUIColor_unk(250, 240, 130, 255);
        }
        return GameUIColor_unk(250, 150, 90, 255);
    }
    return GameUIColor_unk(40, 40, 40, 255);
}

const wchar_t* GameUI::fn_803DF2DC(Bullet_unk* bullet) {
    HitInfo_unk* hit = 0;
    if (bullet != 0) {
        hit = bullet->GetHit_unk();
    }
    if (bullet != 0 && bullet->m08 != 0 && bullet->m08->mType != 2) {
        switch (hit->m04) {
        case 0x4EEC:
            return lbl_8087F1E4->Get_unk(0x253);
        case 0x4EED:
            return lbl_8087F1E4->Get_unk(0x25D);
        case 0x4EEF:
            return lbl_8087F1E4->Get_unk(0x255);
        case 0x4EF0:
            return lbl_8087F1E4->Get_unk(0x25F);
        case 0x4EF2:
            return lbl_8087F1E4->Get_unk(0x257);
        case 0x4EF3:
        case 0x4FD0:
            return lbl_8087F1E4->Get_unk(0x261);
        case 0x4EE9:
            return lbl_8087F1E4->Get_unk(0x259);
        case 0x4EEA:
        case 0x4FC9:
            return lbl_8087F1E4->Get_unk(0x263);
        case 0x4EF8:
            return lbl_8087F1E4->Get_unk(0x251);
        case 0x4EF9:
            return lbl_8087F1E4->Get_unk(0x25B);
        default:
            if (fn_8021A984(hit)) {
                return lbl_8087F1E4->Get_unk(0x251);
            }
            break;
        }
    }
    if (bullet != 0 && hit != 0 && (hit->mAC & 0x40) == 0x40) {
        return lbl_8087F1E4->Get_unk(0x265);
    }
    if (hit != 0 && (hit->mAC & 0x100000) == 0x100000) {
        return lbl_8087F1E4->Get_unk(0x269);
    }
    return lbl_8087F1E4->Get_unk(0x267);
}

void GameUI::fn_803DF5B4() {
    m2F0.mCount = 0;
    for (int i = 0; i < 6; i++) {
        mB0[i].m00 = 0;
        mB0[i].m08 = 0;
    }
    fn_803DF648(0);
    for (unsigned int i = 0; i < m2F0.mCount; i++) {
        m2F0.mData[i]->m00 = 20;
    }
}

void GameUI::fn_803E0D4C() {
    m50.Reset_unk();
}

// inferred: rewinds the d2anime instances of m318 (the check of the element pointer is in the original)
void GameUI::fn_803E0D84() {
    for (int i = 0; i < 6; i++) {
        m318[i].m1C = -1;
        for (int j = 0; j < 5; j++) {
            GameUIElem20_unk* e = &m318[i];
            if (e != 0) {
                m318[i].mInst[j]->mFrame = 0.0f;
            }
        }
    }
    m30C.mMid.mBase.mCount -= (int)m30C.mMid.mBase.mCount;
}

// NONMATCHING: 92.3% - the original addresses the members of the temporary mark with separate registers
// (addi rN, r1, off for each vector) instead of one base; tried init list, body assignments, by-value
// constructor, placement new without the temporary (5 variants)
void GameUI::fn_803E3050(atn::Vector3 pos, atn::Vector3 screen, atn::Vector2_unk size, float dist) {
    for (GameUIMark24_unk* it = m8D0.begin(); it != m8D0.end(); it++) {
        if (dist < it->mDist) {
            if (m8D0.full()) {
                m8D0.mCount--;
            }
            m8D0.insert(it, GameUIMark24_unk(pos, screen, size, dist));
            return;
        }
    }
    if (!m8D0.full()) {
        m8D0.PushBack_unk(GameUIMark24_unk(pos, screen, size, dist));
    }
}

void GameUI::fn_803E32EC() {
    memcpy(&m748, &m764, sizeof(GameUIElem1C_unk));
    m764.Reset_unk();
    memcpy(&m7F8, &m7C0, sizeof(GameUIElem1C_unk));
    m7C0.Reset_unk();
}

// inferred: closes the button prompt (called by the enemies that grab the player)
void GameUI::fn_803E3384() {
    m834 = 1;
    m79C = m764;
    for (int i = 0; i < 2; i++) {
        m814[i * 2]->SetFrame_unk(m814[i * 2]->mData.mEndFrame);
    }
    m814[4]->SetFrame_unk(0.0f);
    for (int i = 0; i < 2; i++) {
        m814[i * 2 + 1]->SetFrame_unk(m814[i * 2 + 1]->mData.mEndFrame);
    }
    m814[5]->SetFrame_unk(0.0f);
    m748.Reset_unk();
    m764.Reset_unk();
    m780.Reset_unk();
}

void GameUI::fn_803E3BE8(void* msg, int arg, float time) {
    if (msg != 0 && m2640 != 0) {
        m2640->mB4 = 0;
        fn_803B78E8(m2640);
        fn_803B6970(m2640, msg, arg, time);
        m2658 = 0;
    }
}

// NONMATCHING: 99.3% - the original walks the first search with a pointer from 0xC3C and the check of the
// earlier selected markers with an offset (here both the other way round)
void GameUI::fn_803E3C78(const GameUIMarker_unk& marker) {
    if (marker.m00 == 0) {
        if (marker.m14 != 0) {
            fn_803CF74C();
        }
        return;
    }
    PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (player == 0) {
        return;
    }
    int index = -1;
    float dist = (marker.mPos - player->mPos).LengthSq_unk();
    for (unsigned int i = 0; i < mC38.mCount; i++) {
        GameUIMarker_unk* it = &mC38.m[i];
        if (dist < (it->mPos - player->mPos).LengthSq_unk()) {
            if (mC38.full()) {
                mC38.mCount--;
            }
            mC38.insert(&mC38.m[i], marker);
            index = i;
            break;
        }
    }
    if (index < 0 && !mC38.full()) {
        mC38.PushBack_unk(marker);
        index = mC38.mCount - 1;
    }
    if (index >= 0 && marker.m14 != 0) {
        for (int i = 0; i < index; i++) {
            if (mC38.m[i].m14 != 0) {
                mC38.m[index].m14 = 0;
                break;
            }
        }
        for (unsigned int i = index + 1; i < mC38.mCount; i++) {
            GameUIMarker_unk* m = &mC38.m[i];
            if (m->m14 != 0) {
                m->m14 = 0;
            }
        }
    }
    for (unsigned int i = 0; i < mC38.mCount; i++) {
        if (mC38.m[i].m14 != 0) {
            mD40 = mC38.m[i];
            mD3C = 1;
            return;
        }
    }
}

void GameUI::fn_803E4100() {
    m748.Reset_unk();
    m764.Reset_unk();
    m780.Reset_unk();
    for (int i = 0; i < 3; i++) {
        m814[i * 2]->mFrame = m814[i * 2]->mData.mEndFrame;
    }
    for (int i = 0; i < 3; i++) {
        m814[i * 2 + 1]->mFrame = m814[i * 2 + 1]->mData.mEndFrame;
    }
    m834 = 0;
}

// inferred: position of a node matrix (name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: starts the marker over the head of a character
// NONMATCHING: code matches; only the offsets of its strings in the pool differ (earlier strings unwritten)
void GameUI::fn_803E41AC(CharaTask* chara) {
    if (m1104 != 0) {
        m1104->mFrame = 0.0f;
        m1104->mSpeed = 1.0f;
        m1108->mFrame = 0.0f;
        m1108->mSpeed = 1.0f;
        m1110->mFrame = 0.0f;
        m1110->mSpeed = 1.0f;
        m1114->mFrame = 0.0f;
        m1114->mSpeed = 1.0f;
        m110C = chara;
        m1118 = 0;
        m111C = 0;
        m112C = 0;
        m1120 = 0;
        m1124 = 0;
        m1128 = 0;
        m1130 = 0;
        if (chara != 0) {
            fn_800C31F4("SE_UI001_011", 0, -1, 1.0f);
            MtxPtr head = m110C->mMotion.GetNodeMtx_unk("head");
            atn::Vector3 pos = GetNodePos_unk(head);
            pos.y += 2.0f;
            atn::Vector3 screen;
            fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
            SetLayoutValue_unk(&m1104->mData.m10, "base_point", 0, screen.x);
            SetLayoutValue_unk(&m1104->mData.m10, "base_point", 1, screen.y);
            SetLayoutValue_unk(&m1108->mData.m10, "base_point", 0, screen.x);
            SetLayoutValue_unk(&m1108->mData.m10, "base_point", 1, screen.y);
        }
    }
}

bool GameUI::fn_803E43A4() {
    return m1130 != 0;
}

void GameUI::fn_803E43B8(int on) {
    if (on) {
        mD8C = 1;
        mD84 = 0;
        mD7C = 8;
        fn_800C31F4(lbl_80750618[3], 0, -1, 0.5f);
    } else {
        mD8C = 0;
        mD88 = 0;
        mD84 = 0;
    }
}

void GameUI::fn_803E4434(int on) {
    if (on) {
        mD8C = 2;
        mD84 = 0;
        mD80 = 1;
        mD7C = 5;
    } else {
        mD8C = 0;
        mD88 = 0;
        mD80 = 0;
        mD84 = 0;
    }
}

void GameUI::fn_803E4478() {
    mD78 = 0;
    mD8C = 0;
    mD88 = 0;
    mD84 = 0;
    mD80 = 0;
    if (mD70 != 0) {
        mD70->FreezeTask();
    }
    if (mD74 != 0) {
        mD74->FreezeTask();
    }
}

void GameUI::fn_803E44C4(int on, int id, int count) {
    if (on) {
        m2644 = 1;
        m2648 = id;
        m264C = count;
        m2650 = 0;
    } else if (m2644 != 0 && m2648 == id) {
        m2650 = m264C + 1;
    }
}

void GameUI::fn_803E4510() {
    m2644 = 0;
    m2648 = 0;
    m264C = 0x2D;
    m2650 = 0;
    if (m2654 != 0) {
        m2654->mSpeed = -1.0f;
        m2654->mFrame = 0.0f;
    }
}

// inferred: resets the HUD (called by game/GameTask); inlines fn_803E4100, fn_803E0D4C, fn_803E32EC,
// fn_803CF74C, fn_803E4510, fn_803E43B8 and fn_803E4434
// NONMATCHING: 99.2% - r5/r6 of the first gauge and of the pointer of the mB0 loop swapped
void GameUI::fn_803E454C() {
    fn_803E41AC(0);
    fn_803B57B0(m263C, 1);
    fn_803B6C88(m2640, 1);
    m114C = 0;
    m110C = 0;
    m1170 = 0;
    m15B4 = 0;
    m2658 = 0;
    fn_803E4100();
    fn_803E0D4C();
    for (int i = 0; i < 6; i++) {
        mB0[i].Reset_unk();
    }
    mD90.m00 = 0;
    mDA4 = 0;
    mDA8 = 0;
    mDAC = 0;
    m25CC.m00 = 0;
    m2568.m00 = 0;
    fn_803E32EC();
    m30C.mMid.mBase.mCount -= (int)m30C.mMid.mBase.mCount;
    m85C.mCount = 0;
    m1150 = 0;
    if (m1140 != 0) {
        m1140->mFrame = 0.0f;
    }
    if (m1144 != 0) {
        m1144->mFrame = 0.0f;
    }
    for (int i = 0; i < 32; i++) {
        mF94[i] = 0;
        m1014[i] = -1;
    }
    for (int i = 0; i < 8; i++) {
        m10D4[i] = 0;
    }
    m2620 = 0;
    m2624 = 0;
    m3F0.mCount = 0;
    mC38.mCount = 0;
    fn_803CF74C();
    fn_803E4510();
    if (mD8C == 1) {
        fn_803E43B8(0);
    }
    if (mD8C == 2) {
        fn_803E4434(0);
    }
}

void GameUI::fn_803E4950() {
    mD90.m00 = 0;
    mDB0 = 0;
    mDB4->mFrame = 0.0f;
    mDB4->mSpeed = -1.0f;
}

void GameUI::fn_803E4978(const wchar_t* text) {
    if (text != 0) {
        mD90.m00 = 1;
        mD90.m04 = 0;
        mD90.m08 = text;
        mDB0 = 2;
    }
}

void GameUI::fn_803E4A20(int a, int b) {
    if (a) {
        mD90.m00 = 1;
        mD90.m04 = 1;
        mDA4 = (GameUIName08_unk*)a; // inferred: the callers pass the objects as ints
        mDA8 = (GameUIName0C_unk*)b;
        mDB0 = 2;
    }
}

float GameUI::fn_803E4A48() {
    return mDB4->mFrame / 10.0f;
}

// inferred: enemies around the leader that pass `filter`, sorted by distance (farthest first)
// NONMATCHING: 99.9% - r4/r5 of the result flags of the inlined state checks swapped
void GameUI::fn_803E4A5C(GameUIDistList_unk* list, bool (*filter)(CharaTask*)) {
    atn::Vector3 pos;
    PlayerTask* player = lbl_8087F8A0->mHead;
    if (player != 0) {
        pos = player->mPos;
    }
    list->mCount = 0;
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if (CanActB_unk(e) && (e->m54C & 0x2000) != 0x2000 && e->mC58.mC0 != 0 && e->mFlag26 == 1) {
            float dist = (pos - e->mPos).LengthSq_unk();
            bool ok = false;
            if (filter != 0 && filter(e)) {
                ok = true;
            }
            if (ok) {
                GameUIDist_unk entry;
                entry.mDist = dist;
                entry.mChara = e;
                bool done = false;
                // inferred: the original bounds the search with m2788 itself (the list passed in)
                for (unsigned int i = 0; i < m2788.mCount; i++) {
                    if (list->mData[i].mDist < dist) {
                        if (list->full()) {
                            list->mCount--;
                        }
                        list->insert(&list->mData[i], entry);
                        done = true;
                        break;
                    }
                }
                if (!done && !list->full()) {
                    list->PushBack_unk(entry);
                }
            }
        }
    }
}

// inferred: true while the character cannot be controlled (states 0x8/0x200, a counter, state 0x800000)
extern "C" bool fn_803E4DD4(CharaTask* chara) {
    if (chara != 0) {
        bool down = chara->mStatus.IsState_unk(8) || chara->mStatus.IsState_unk(0x200);
        if (down) {
            return true;
        }
    }
    if (chara != 0 && chara->mStatus.m430 > 0) {
        return true;
    }
    if (chara != 0 && chara->mStatus.IsState_unk(0x800000)) {
        return true;
    }
    return false;
}

void GameUI::fn_803E58CC() {
    if (mDC0 != 0) {
        mDC0->mFrame = 0.0f;
    }
}

void GameUI::fn_803E58E4() {
    if (mDC4 != 0) {
        mDC4->mFrame = 0.0f;
        fn_800C31F4(lbl_80750618[12], 0, -1, 1.0f);
    }
}

bool GameUI::fn_803E5940() {
    if (mDC4 != 0) {
        return (mDC4->mFlags & 4) != 4 && !mDC4->IsEnd_unk();
    }
    return false;
}

// NONMATCHING: code matches; only the offsets of its strings in the pool differ (earlier strings unwritten)
void GameUI::fn_803E598C(int index, int value, int skill) {
    if (index < 0) {
        return;
    }
    if (index >= 3 || m2550[index] == 0) {
        return;
    }
    m255C = -1;
    m2560 = 0.0f;
    for (int i = 0; i < 3; i++) {
        if (m2550[i] != 0) {
            m2550[i]->mFrame = m2550[i]->mData.mEndFrame;
        }
    }
    m255C = index;
    m2560 = value;
    m2550[index]->mFrame = 0.0f;
    if (index == 2) {
        HitInfo_unk* hit = skill > 0 ? fn_80219E6C(skill) : 0;
        if (hit != 0) {
            const wchar_t* text;
            switch (hit->m04) {
            case 0x1774:
                text = lbl_8087F1E4->Get_unk(0x2CF);
                break;
            case 0x179A:
                text = lbl_8087F1E4->Get_unk(0x2D1);
                break;
            case 0x179B:
                text = lbl_8087F1E4->Get_unk(0x2D3);
                break;
            case 0x177F:
                text = lbl_8087F1E4->Get_unk(0x2D5);
                break;
            case 0x179C:
                text = lbl_8087F1E4->Get_unk(0x391);
                break;
            case 0x179F:
                text = lbl_8087F1E4->Get_unk(0x393);
                break;
            case 0x1791:
                text = lbl_8087F1E4->Get_unk(0x395);
                break;
            case 0x1788:
                text = lbl_8087F1E4->Get_unk(0x397);
                break;
            case 0x6C2:
                text = lbl_8087F1E4->Get_unk(0x3A1);
                break;
            default:
                text = (const wchar_t*)hit->m08;
                break;
            }
            if (text != 0) {
                SetLayoutText_unk(&m2550[index]->mData.m10, "string0007", text);
                SetLayoutText_unk(&m2550[index]->mData.m10, "string0002(1)", text);
            }
        }
    }
    if (lbl_8087F610 != 0) {
        fn_804DA478(lbl_8087F610);
    }
}

// inferred: shows the time left over a character (TacView)
// NONMATCHING: code matches; only the offsets of its strings in the pool differ (earlier strings unwritten)
void GameUI::fn_803E5C60(CharaTask* chara) {
    if (chara == 0 || m254C == 0) {
        return;
    }
    atn::Vector3 pos = chara->vf58();
    atn::Vector3 screen;
    fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
    if (0.0f < screen.z && screen.z < 1.0f) {
        atn::D2A_Task* task = m254C;
        task->SetNumber_unk("string0001(1)", fn_803CE854(chara->fn_8016DAB0()), 0);
        task->SetNumber_unk("string0002(1)", fn_803CE854(chara->fn_8016DAB0()), 0);
        SetLayoutValue_unk(&task->mData.m10, "basePoint", 0, screen.x);
        SetLayoutValue_unk(&task->mData.m10, "basePoint", 1, screen.y);
        task->mFrame = 0.0f;
        fn_800C31F4(lbl_80750618[9], 0, -1, 0.5f);
    }
}

// NONMATCHING: 99.7% - r5/r6 of the message kind and of the value of kind 11 swapped (locals for both, int
// and bool flag, declaration orders: 10 variants)
void GameUI::fn_803E5E64(int itemId, int count, int rare) {
    GameUIItem_unk* item = itemId > 0 ? fn_80211480(itemId) : 0;
    if (item != 0 && count != 0) {
        wchar_t text[0x100];
        int plural = item->m64 == 2;
        if (item->mB4 != 0) {
            int value = item->mB8;
            switch (item->mB4) {
            case 1:
                if (plural) {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1F5));
                } else {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1E9));
                }
                break;
            case 2:
                if (plural) {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1F7));
                } else {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1EB));
                }
                break;
            case 3:
                if (plural) {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1F9));
                } else {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1ED));
                }
                break;
            case 4:
                if (plural) {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1FB));
                } else {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1EF));
                }
                break;
            case 5:
                if (plural) {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1FD));
                } else {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1F1));
                }
                break;
            case 6:
                if (plural) {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1F3));
                } else {
                    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1E7));
                }
                break;
            case 8:
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x1FF));
                break;
            case 9:
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x201));
                break;
            case 11:
                fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x203), value / 10.0f);
                break;
            }
        } else {
            fn_80444CF8(text, itemId, count, rare);
        }
        mDB8->mFrame = 0.0f;
        SetLayoutText_unk(&mDB8->mData.m10, "arrowName02", text);
        SetLayoutText_unk(&mDB8->mData.m10, "string0006(1)", text);
        SetLayoutText_unk(&mDB8->mData.m10, "string0003(1)", text);
        if (item->mC2 > 0 && (unsigned short)(item->mBC - 2) <= 1) {
            mDBC->mFrame = 0.0f;
        }
    }
}

// NONMATCHING: code matches; only the offsets of its strings in the pool differ (earlier strings unwritten)
void GameUI::fn_803E627C(int town) {
    char path[0x100];
    if (m262C.m00 == 0) {
        sprintf(path, "/data/d2anime/ui_shop_town%02d_op.d2a", town - 1);
        m262C.m00 = fn_801F3FF8(this, path, 0);
        m262C.m00->fn_800D246C(1);
        m262C.m00->mRepeat = 0;
        m262C.m00->mBlend = 18;
    }
    if (m262C.m04 == 0) {
        sprintf(path, "/data/d2anime/ui_shop_town%02d_op.d2a", town - 1);
        m262C.m04 = fn_801F3FF8(this, path, 0);
        m262C.m04->fn_800D246C(1);
        m262C.m04->mRepeat = 0;
        m262C.m04->mBlend = 15;
    }
    if (m262C.m08 == 0) {
        sprintf(path, "/data/d2anime/ui_shop_town%02d_loop.d2a", town - 1);
        m262C.m08 = fn_801F3FF8(this, path, 0);
        m262C.m08->fn_800D246C(1);
        m262C.m08->mRepeat = 1;
        m262C.m08->mBlend = 15;
    }
}

void GameUI::fn_803E63D0() {
    if (m262C.m00 != 0) {
        fn_800D2338(m262C.m00);
        m262C.m00 = 0;
    }
    if (m262C.m04 != 0) {
        fn_800D2338(m262C.m04);
        m262C.m04 = 0;
    }
    if (m262C.m08 != 0) {
        fn_800D2338(m262C.m08);
        m262C.m08 = 0;
    }
    m262C.m0C = 0;
}

bool GameUI::fn_803E644C() {
    if (m262C.m00 != 0 && (m262C.m00->mFlags & 2) != 2) {
        return true;
    }
    if (m262C.m04 != 0 && (m262C.m04->mFlags & 2) != 2) {
        return true;
    }
    if (m262C.m08 != 0 && (m262C.m08->mFlags & 2) != 2) {
        return true;
    }
    if (!fn_80370C2C(lbl_8087F430)) {
        return true;
    }
    return false;
}

void GameUI::fn_803E64F0() {
    m262C.m00->fn_800D246C(0);
    m262C.m00->FreezeTask();
    m262C.m04->fn_800D246C(0);
    m262C.m04->FreezeTask();
    m262C.m08->fn_800D246C(0);
    m262C.m08->FreezeTask();
}

void GameUI::fn_803E656C() {
    if (m262C.m00 == 0 || m262C.m04 == 0 || m262C.m08 == 0) {
        return;
    }
    if (m262C.m0C == 1 || m262C.m0C == 2) {
        return;
    }
    m262C.m00->mFrame = 0.0f;
    m262C.m00->mSpeed = 1.0f;
    m262C.m04->mFrame = 0.0f;
    m262C.m04->mSpeed = 1.0f;
    m262C.m08->mFrame = 0.0f;
    m262C.m0C = 1;
    if (lbl_8087F430 != 0) {
        fn_80370BD0(lbl_8087F430, 30, 0, 1e-5f);
    }
}

void GameUI::fn_803E65F8() {
    if (m262C.m00 == 0 || m262C.m04 == 0 || m262C.m08 == 0) {
        return;
    }
    if (m262C.m0C == 3 || m262C.m0C == 0) {
        return;
    }
    m262C.m00->SetFrame_unk(m262C.m00->mData.mEndFrame);
    m262C.m00->mSpeed = -1.0f;
    m262C.m04->SetFrame_unk(m262C.m04->mData.mEndFrame);
    m262C.m04->mSpeed = -1.0f;
    m262C.m0C = 3;
    if (lbl_8087F430 != 0) {
        fn_80370B78(lbl_8087F430, 0xFF000000, 30, 0, 1e-4f);
    }
}

// inferred: update of the shop animations (called by vf10)
// NONMATCHING: 99.7% - f1/f2 of the end frame and the frame swapped in the alpha (the original loads the
// frame first into f1); tried locals in both orders, -(a - b), min as if/ternary (12 variants)
void GameUI::fn_803E668C() {
    if (m262C.m00 == 0 || m262C.m04 == 0 || m262C.m08 == 0) {
        return;
    }
    if (!m262C.m00->IsFlag_unk(2) || !m262C.m04->IsFlag_unk(2) || !m262C.m08->IsFlag_unk(2)) {
        return;
    }
    m262C.m00->mFlags |= 4;
    m262C.m04->mFlags |= 4;
    m262C.m08->mFlags |= 4;
    if (m262C.m0C == 1) {
        m262C.m00->mFlags &= ~4;
        m262C.m04->mFlags &= ~4;
        if (m262C.m00->IsEnd_unk()) {
            m262C.m0C = 2;
        }
    } else if (m262C.m0C == 2) {
        m262C.m08->mFlags &= ~4;
    } else if (m262C.m0C == 3) {
        m262C.m00->mFlags &= ~4;
        m262C.m04->mFlags &= ~4;
        if (m262C.m00->mFrame <= 0.0f) {
            m262C.m0C = 0;
        }
    }
    atn::D2A_Task* task = m262C.m00;
    float frame = task->mFrame;
    float alpha = (task->mData.mEndFrame - frame) / 7.0f;
    fn_801FECE0(&task->mData.m10, fn_800DC6B4("alpha"), 1.0f < alpha ? 1.0f : alpha);
}

void GameUI::fn_803E6850(CharaTask* chara, int count, int max, int arg) {
    if (!m25CC.Full_unk()) {
        new (&m25CC.m[m25CC.m00]) GameUIElem10_unk(chara, count, max, arg);
        m25CC.m00++;
    }
}

// inferred: adds the icon of a skill that hit (SkillEffectManager)
// NONMATCHING: 98.5% - the original reloads the count for the new slot after the search loop (and so
// allocates r7/r8/r9 differently); tried Find/Push/Full inlines on the list (6 variants)
void GameUI::fn_803E6890(CharaTask* chara, HitInfo_unk* hit) {
    if (m2568.m00 >= 8 || hit == 0) {
        return;
    }
    bool first = false;
    if (m2568.m00 == 0) {
        first = true;
    }
    int icon = -1;
    if (chara == 0 || chara->mType == 2) {
        if (hit->m04 == 0x8A) {
            icon = 10;
        }
    } else {
        switch (hit->m04) {
        case 0x82:
        case 0x88:
            icon = 0;
            break;
        case 0x83:
        case 0x8F:
            icon = 1;
            break;
        case 0x85:
        case 0x89:
            icon = 2;
            break;
        case 0x86:
        case 0x90:
            icon = 3;
            break;
        case 0x99:
        case 0x9D:
            icon = 4;
            break;
        case 0x9A:
        case 0xA1:
        case 0x4FD1:
        case 0x4FD2:
            icon = 5;
            break;
        case 0x96:
        case 0x9C:
            icon = 6;
            break;
        case 0x97:
        case 0xA0:
        case 0x4FCA:
        case 0x4FCB:
            icon = 7;
            break;
        case 0xC:
            icon = 8;
            break;
        case 0x13:
            icon = 9;
            break;
        }
    }
    if (icon < 0) {
        return;
    }
    for (unsigned int i = 0; i < m2568.m00; i++) {
        if (icon == m2568.m[i].m00) {
            return;
        }
    }
    new (&m2568.m[m2568.m00]) GameUIElem0C_unk(icon, chara, hit);
    m2568.m00++;
    if (first && m2564 != 0) {
        m2564->mFrame = 0.0f;
        fn_800C31F4(lbl_80750618[10], 0, -1, 1.0f);
    }
}

// NONMATCHING: 99.8% - code matches; only the offset of "basePos" in the string pool (the strings
// of the earlier functions are not written yet)
void GameUI::fn_803E6ADC(const atn::Vector3& a, const atn::Vector3& b) {
    if (mA84 != 0) {
        mA84->mFrame = 0.0f;
        SetLayoutValue_unk(&mA84->mData.m10, "basePos", 0, 320.0f);
        SetLayoutValue_unk(&mA84->mData.m10, "basePos", 1, 224.0f);
    }
    if (mA88 != 0) {
        mA88->mFrame = 0.0f;
    }
    m888 = a;
    m894 = b;
}

void GameUI::fn_803E6BB4(CharaTask* id) {
    CharaTask* old = m2620;
    m2624 = old;
    m2620 = id;
    if (id != 0 && old != 0 && id != old && m2628 != 0) {
        m2628->mFrame = 0.0f;
    }
}

// NONMATCHING: 90.4% - the original reloads the count for the slot and stores the temporary after it
void GameUI::fn_803E6BF0(unsigned int id, int value) {
    for (unsigned int i = 0; i < m3F0.mCount; i++) {
        unsigned int v = m3F0.mData[i].m08;
        if (v == id) {
            m3F0.mData[i].mFrames = 0;
            m3F0.mData[i].m00 = value;
            return;
        }
    }
    if (m3F0.mCount < 32) {
        GameUIEntry_unk e;
        e.m00 = value;
        e.mFrames = 0;
        e.m08 = id;
        m3F0.PushBack_unk(e);
    }
}

bool GameUI::fn_803E6C88(unsigned int id) {
    for (unsigned int i = 0; i < m3F0.mCount; i++) {
        unsigned int v = m3F0.mData[i].m08;
        if (v == id) {
            return true;
        }
    }
    return false;
}
