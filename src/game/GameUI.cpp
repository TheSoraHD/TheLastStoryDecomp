#include "Event/Manager.h"
#include "game/GameTask.h"
#include "atn/RenderManager.h"
#include "game/GameChoiceWindow.h"
#include "game/GameUI.h"
#include "atn/WordWrap_unk.h"
#include "game/GameScene.h"
#include "atn/D2A_Task.h"
#include "atn/D2A_TaskInstance.h"

// inferred: d2anime task with an end frame (GameUI 0x3EC); names invented
struct GameUIDuration_unk : public atn::D2A_Task {
    float fn_803CFC6C(); // inferred: returns the end frame
};
// inferred: gauges of the HUD (GameUI 0x83C, 0x840): d2anime tasks (frame 0x100, end frame 0xA0)
struct GameUIGauge_unk : public atn::D2A_Task {};
#include "game/CharaTask.h"
#include "atn/Sphere.h"
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
#include "game/Arms.h"
#include "std/vector.h"
#include "atn/RenderSetting_unk.h"
extern "C" int sprintf(char* s, const char* format, ...);

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
// inferred: sounds of the HUD ("SE_UI001_012", ...); the table is in the auto .rodata before the split
extern "C" const char* const lbl_80750618[14];

// inferred: field (lbl_8087F430) and its route table at 0x10D8
struct GameUIField_unk {
    unsigned char m00[0x74];
    atn::Vector3 mEye; // inferred: camera position (GameUI::fn_803E5200)
    unsigned char m80[0x868 - 0x80];
    int m868; // inferred: kind of field (5, 7: the leader is the centre of the markers)
    unsigned char m86C[0x910 - 0x86C];
    atn::Vector3 m910; // inferred: centre of the markers in the tactical view
    unsigned char m91C[0x10D8 - 0x91C];
    atn::Task* m10D8;
};
extern "C" GameUIField_unk* lbl_8087F430;
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" void fn_800D58A4(atn::Texture_unk* tex); // inferred: releases a render texture
extern "C" int fn_8021A984(HitInfo_unk* hit); // inferred: the hit heals
extern "C" int fn_8021A9CC(HitInfo_unk* hit); // inferred: kind test
extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" SkillEffectManager* lbl_8087F9E8;
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
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
struct GameUISession_unk;
extern "C" GameUISession_unk* lbl_8087F610; // inferred: multiplayer session
// inferred: player of the multiplayer session (0xD5C bytes, as MP/D2AMpMenuLobby)
struct GameUISessionPlayer_unk {
    CharaTask* m00;
    unsigned char m04[0xD0 - 0x04];
    unsigned int mD0_31 : 1; // inferred: valid
    unsigned int mD0_lo : 31;
    unsigned char mD4[0xD5C - 0xD4];
};
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
    // inferred: player of the session (as MP/D2AMpMenuCockpit GetPlayer_unk); name invented
    GameUISessionPlayer_unk* GetPlayer_unk(int i) {
        if (i >= 0 && i < m5E8) {
            return &m5E4[i];
        }
        return 0;
    }
    // inferred: the level and the last attack of the characters are shown (byte of m5A8; GameUI::fn_803D022C)
    unsigned char IsShowLevel_unk() const { return m5A8; }
    unsigned char m510[0x540 - 0x510];
    int m540; // inferred: state of the match (2: playing; GameUI::fn_803E2410)
    unsigned char m544[0x5A8 - 0x544];
    int m5A8;
    unsigned char m5AC[0x5E4 - 0x5AC];
    GameUISessionPlayer_unk* m5E4; // inferred: players
    int m5E8;                      // inferred: number of players
};
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
int lbl_8087F494;     // inferred: debug flag "ChantTimeHalf" (fn_803CFC78): frames counted at 60 per second (else 30)

// inferred: colours of the HUD (0x807C8628, .bss of this file); they are set by the static initializer
// (__sinit, 0x803E78B4) and registered in the debug menu by fn_803CFC78
atn::Color lbl_807C8628[16] = {
    atn::Color(255 / 255.0f, 217 / 255.0f, 51 / 255.0f, 0.0f),
    atn::Color(230 / 255.0f, 46 / 255.0f, 46 / 255.0f, 0.0f),
    atn::Color(125 / 255.0f, 125 / 255.0f, 125 / 255.0f, 0.0f),
    atn::Color(197 / 255.0f, 80 / 255.0f, 254 / 255.0f, 0.0f),
    atn::Color(197 / 255.0f, 80 / 255.0f, 254 / 255.0f, 0.0f),
    atn::Color(153 / 255.0f, 153 / 255.0f, 153 / 255.0f, 0.0f),
    atn::Color(125 / 255.0f, 125 / 255.0f, 125 / 255.0f, 0.0f),
    atn::Color(255 / 255.0f, 60 / 255.0f, 15 / 255.0f, 0.0f),
    atn::Color(10 / 255.0f, 10 / 255.0f, 255 / 255.0f, 0.0f),
    atn::Color(51 / 255.0f, 255 / 255.0f, 102 / 255.0f, 0.0f),
    atn::Color(250 / 255.0f, 240 / 255.0f, 130 / 255.0f, 0.0f),
    atn::Color(167 / 255.0f, 248 / 255.0f, 255 / 255.0f, 0.0f),
    atn::Color(102 / 255.0f, 26 / 255.0f, 153 / 255.0f, 0.0f),
    atn::Color(153 / 255.0f, 27 / 255.0f, 27 / 255.0f, 0.0f),
    atn::Color(255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 0.0f),
    atn::Color(25 / 255.0f, 25 / 255.0f, 25 / 255.0f, 0.0f),
};

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
        fn_803CFC78();
        return 1;
    }
    return 0;
}

void GameUI::vf10() {
    fn_803E668C();
    if (m840 != 0 && !m840->IsFlag_unk(4) && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0 &&
        lbl_8087F430->m10D8->IsFlag_unk(2) && ((GameScene*)lbl_8087F430->m10D8)->fn_803CC900() > 0) {
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

// inferred: debug parameter menu (atn/ParamMenu_unk; same declarations as gmk/SwingObject.cpp)
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80088FAC(void* group, const char* name, float* rgba, int arg3, int arg4, float min, float max,
                            float step); // inferred: colour (four floats)

// inferred: registers the parameters of the HUD in the debug menu ("GameUI": job and attribute colours, the
// colours of the tactical view cursor, the flash counters and the access icon)
// NONMATCHING: 99.8% - the original passes the first colour as `addi r5, r30, 0` (here `mr`), which also
// moves one string load; tried the table as a pointer, the array directly, two static arrays
void GameUI::fn_803CFC78() {
    const char* name = "GameUI";
    atn::Color* colors = lbl_807C8628;
    void* group;
    if (m2664.mRes == 0 && lbl_8087EF10 != 0) {
        group = m2664.mRes = fn_8008937C(lbl_8087EF10->m10, name);
    } else {
        group = 0;
    }
    fn_80087994(group, "ChantTimeHalf", &lbl_8087F494, 0, 0);
    fn_80088FAC(group, "Job_Soldier", &colors[0].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Job_Magic", &colors[1].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Job_Healer", &colors[2].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Job_Reverser", &colors[3].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Job_Archer", &colors[4].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Job_Others", &colors[5].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Job_Guard", &colors[6].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Fire", &colors[7].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Ice", &colors[8].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Forest", &colors[9].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Wind", &colors[11].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Holy", &colors[10].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Dark", &colors[12].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Star", &colors[13].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Back", &colors[14].r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "Attr_Back2", &colors[15].r, 0, 0, 0.0f, 1.0f, 1.0f);
    void* cursor = fn_8008937C(group, "TacviewCursor");
    fn_80088FAC(cursor, "WindF", &m2698.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "WindB", &m26A8.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "CommandF", &m2678.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "CommandB", &m2688.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "EnHealF", &m26B8.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "EnHealB", &m26C8.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "FrHealB", &m26D8.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "FrFlameB", &m26E8.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "FrFreezeB", &m26F8.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "FrForestB", &m2708.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(cursor, "FrHolyB", &m2718.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_800874C8(group, "NewFlashCount", &mD78, 0, 999, 1, 0, 0);
    fn_800874C8(group, "NewFlashCountMax", &mD7C, 0, 999, 1, 0, 0);
    fn_80087994(group, "NewFlashInfinity", &mD80, 0, 0);
    fn_80088FAC(group, "OnCursor", &m288C.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_80088FAC(group, "OnCursorShadow", &m289C.r, 0, 0, 0.0f, 1.0f, 1.0f);
    fn_8008771C(group, "AccessIconSizeMin", &m2668, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(group, "AccessIconSizeMax", &m266C, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(group, "AccessIconDistMin", &m2670, 0, 0, 0.0f, 100000.0f, 0.01f);
    fn_8008771C(group, "AccessIconDistMax", &m2674, 0, 0, 0.0f, 100000.0f, 0.01f);
}

// inferred: creates the parts of the HUD (d2anime tasks and instances; called by game/GameTask, released by
// fn_803D11B4); the paths used in every pass of a loop are locals of the loop (the original keeps them in
// registers, and they come first in the string pool)
// NONMATCHING: 99.5% - register allocation of the loops (the original gives the hoisted paths the higher
// registers and the induction pointers the lower ones)
void GameUI::fn_803D022C() {
    m50.m0C = fn_801F64D0(this, "/data/d2anime/ui_player_hpmp.d2a");
    m50.m0C->fn_800D246C(1);
    m50.m10 = fn_801F64D0(this, "/data/d2anime/ui_player_tp_shine.d2a");
    m50.m10->fn_800D246C(1);
    if (lbl_8087F610 != 0 && !lbl_8087F610->IsShowLevel_unk()) {
        m50.m14 = 0;
    } else {
        m50.m14 = fn_801F64D0(this, "/data/d2anime/ui_player_lv.d2a");
        m50.m14->fn_800D246C(1);
    }
    m50.m18 = fn_801F64D0(this, "/data/d2anime/ui_player_icon_help_loop.d2a");
    m50.m18->fn_800D246C(1);
    if (lbl_8087F610 != 0 && !lbl_8087F610->IsShowLevel_unk()) {
        m50.m1C = 0;
    } else {
        m50.m1C = fn_801F64D0(this, "/data/d2anime/ui_player_last.d2a");
        m50.m1C->fn_800D246C(1);
    }
    m50.m20 = fn_801F64D0(this, "/data/d2anime/ui_player_heal.d2a");
    m50.m20->fn_800D246C(1);
    m50.m24 = fn_801F64D0(this, "/data/d2anime/ui_player_icon_hate_loop.d2a");
    m50.m24->fn_800D246C(1);
    m50.m28 = fn_801F64D0(this, "/data/d2anime/ui_player_wind_shine.d2a");
    m50.m28->fn_800D246C(1);
    m50.m2C = fn_801F64D0(this, "/data/d2anime/ui_player_lvup.d2a");
    m50.m2C->fn_800D246C(1);
    for (int i = 0; i < 6; i++) {
        const char* hpmp = "/data/d2anime/ui_friend_hpmp.d2a";
        const char* tpShine = "/data/d2anime/ui_friend_tp_shine.d2a";
        const char* help = "/data/d2anime/ui_friend_icon_help_loop.d2a";
        const char* heal = "/data/d2anime/ui_friend_heal.d2a";
        const char* lvup = "/data/d2anime/ui_friend_lvup.d2a";
        const char* magStart = "data/d2anime/ui_friend_icon_mag_start.d2a";
        const char* magDefault = "data/d2anime/ui_friend_icon_default.d2a";
        mB0[i].m0C = fn_801F64D0(this, hpmp);
        mB0[i].m0C->fn_800D246C(1);
        mB0[i].m10 = fn_801F64D0(this, tpShine);
        mB0[i].m10->fn_800D246C(1);
        if (lbl_8087F610 != 0 && !lbl_8087F610->IsShowLevel_unk()) {
            mB0[i].m14 = 0;
        } else {
            mB0[i].m14 = fn_801F64D0(this, "/data/d2anime/ui_friend_lv.d2a");
            mB0[i].m14->fn_800D246C(1);
        }
        mB0[i].m18 = fn_801F64D0(this, help);
        mB0[i].m18->fn_800D246C(1);
        if (lbl_8087F610 != 0 && !lbl_8087F610->IsShowLevel_unk()) {
            mB0[i].m1C = 0;
        } else {
            mB0[i].m1C = fn_801F64D0(this, "/data/d2anime/ui_friend_last.d2a");
            mB0[i].m1C->fn_800D246C(1);
        }
        mB0[i].m20 = fn_801F64D0(this, heal);
        mB0[i].m20->fn_800D246C(1);
        mB0[i].m2C = fn_801F64D0(this, lvup);
        mB0[i].m2C->fn_800D246C(1);
        m318[i].m14 = fn_801F64D0(this, magStart);
        m318[i].m14->fn_800D246C(0);
        m318[i].m18 = fn_801F64D0(this, magDefault);
        m318[i].m18->fn_800D246C(0);
        for (int j = 0; j < 5; j++) {
            if (j == 0) {
                m318[i].mInst[j] = m318[i].m14;
            } else if (j == 1) {
                m318[i].mInst[j] = fn_801F64D0(this, "/data/d2anime/ui_friend_icon_mag_broken.d2a");
                m318[i].mInst[j]->fn_800D246C(1);
            } else {
                m318[i].mInst[j] = m318[i].m18;
            }
        }
    }
    m3EC = (GameUIDuration_unk*)fn_801F3FF8(this, "/data/d2anime/ui_hp_heal.d2a", 0);
    m3EC->fn_800D246C(1);
    m3EC->mVisible = 0;
    for (int i = 0; i < 8; i++) {
        const char* hp = "/data/d2anime/ui_hp.d2a";
        const char* shine = "/data/d2anime/ui_hp_shine.d2a";
        m574[i] = fn_801F64D0(this, hp);
        m574[i]->fn_800D246C(1);
        m594[i] = fn_801F64D0(this, shine);
        m594[i]->fn_800D246C(1);
    }
    m5B8 = fn_801F3FF8(this, "/data/d2anime/ui_hp.d2a", 0);
    m5B8->fn_800D246C(1);
    m3D8 = fn_801F3FF8(this, "/data/d2anime/ui_boss_frame.d2a", 0);
    m3D8->fn_800D246C(1);
    for (int i = 0; i < 4; i++) {
        const char* path = "/data/d2anime/ui_boss_hp.d2a";
        m3DC[i] = fn_801F64D0(this, path);
        m3DC[i]->fn_800D246C(1);
    }
    for (int i = 0; i < 6; i++) {
        const char* path = "/data/d2anime/ui_point_down_op.d2a";
        m6FC[i] = fn_801F64D0(this, path);
        m6FC[i]->fn_800D246C(1);
    }
    m714[0] = fn_801F3FF8(this, "/data/d2anime/ui_lever_open.d2a", 0);
    m714[0]->fn_800D246C(1);
    m71C[0] = fn_801F3FF8(this, "/data/d2anime/ui_lever_roop.d2a", 0);
    m71C[0]->fn_800D246C(1);
    m714[1] = fn_801F3FF8(this, "/data/d2anime/ui_lever_open.d2a", 0);
    m714[1]->fn_800D246C(1);
    m71C[1] = fn_801F3FF8(this, "/data/d2anime/ui_lever_roop.d2a", 0);
    m71C[1]->fn_800D246C(1);
    m72C = fn_801F3FF8(this, "/data/d2anime/ui_event_button_loop.d2a", 0);
    m72C->fn_800D246C(1);
    m730 = fn_801F3FF8(this, "/data/d2anime/ui_event_button.d2a", 0);
    m730->fn_800D246C(1);
    for (int i = 0; i < 12; i++) {
        const char* path = "/data/d2anime/ui_object_lock.d2a";
        m8A0[i] = fn_801F64D0(this, path);
        m8A0[i]->fn_800D246C(1);
    }
    mA84 = fn_801F3FF8(this, "/data/d2anime/ui_aspect_event.d2a", 0);
    mA84->fn_800D246C(1);
    mA88 = fn_801F3FF8(this, "/data/d2anime/ui_object_lock_hit.d2a", 0);
    mA88->fn_800D246C(1);
    m73C = fn_801F3FF8(this, "/data/d2anime/ui_abtn_r_op.d2a", 0);
    m73C->fn_800D246C(1);
    m740 = fn_801F3FF8(this, "/data/d2anime/ui_abtn_r_cl.d2a", 0);
    m740->fn_800D246C(1);
    m744 = fn_801F3FF8(this, "/data/d2anime/ui_abtn_arrow_loop.d2a", 0);
    m744->fn_800D246C(1);
    m7B8 = fn_801F3FF8(this, "/data/d2anime/ui_abtn_l_op.d2a", 0);
    m7B8->fn_800D246C(1);
    m7BC = fn_801F3FF8(this, "/data/d2anime/ui_abtn_l_cl.d2a", 0);
    m7BC->fn_800D246C(1);
    m814[0] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction_opcl.d2a", 0);
    m814[0]->fn_800D246C(1);
    m814[2] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction_loop.d2a", 0);
    m814[2]->fn_800D246C(1);
    m814[4] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction_shine.d2a", 0);
    m814[4]->fn_800D246C(1);
    m814[6] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_arrow_loop.d2a", 0);
    m814[6]->fn_800D246C(1);
    m814[1] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction2_opcl.d2a", 0);
    m814[1]->fn_800D246C(1);
    m814[3] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction2_loop.d2a", 0);
    m814[3]->fn_800D246C(1);
    m814[5] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction2_shine.d2a", 0);
    m814[5]->fn_800D246C(1);
    m814[7] = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_abtn_arrow2_loop.d2a", 0);
    m814[7]->fn_800D246C(1);
    m83C = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_action_seek_shine_op.d2a", 0);
    m83C->fn_800D246C(1);
    m840 = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_action_seek_shine.d2a", 0);
    m840->fn_800D246C(1);
    m263C = GameMessageWindow::fn_803B5470(this);
    m2640 = GameTextMessageWindow::fn_803B612C(this);
    for (int i = 0; i < 8; i++) {
        mB38[i].m00 = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_access_point_op.d2a", 1);
        mB38[i].m00->fn_800D246C(1);
        mB38[i].m04 = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_access_point_loop.d2a", 1);
        mB38[i].m04->fn_800D246C(1);
        mB38[i].m08 = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_event_icon_hostile_opcl.d2a", 1);
        mB38[i].m08->fn_800D246C(1);
        mB38[i].m0C = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_event_icon_hostile_loop.d2a", 1);
        mB38[i].m0C->fn_800D246C(1);
        mB38[i].m10 = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_event_icon_large_opcl.d2a", 1);
        mB38[i].m10->fn_800D246C(1);
        mB38[i].m14 = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_event_icon_large_loop.d2a", 1);
        mB38[i].m14->fn_800D246C(1);
        mB38[i].m18 = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_event_icon_ev_opcl_test.d2a", 1);
        mB38[i].m18->fn_800D246C(1);
        mB38[i].m1C = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_event_icon_ev_loop_test.d2a", 1);
        mB38[i].m1C->fn_800D246C(1);
    }
    m2654 = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_bashohyouji.d2a", 0);
    m2654->fn_800D246C(1);
    mD70 = fn_801F3FF8(this, "/data/d2anime/ui_event_icon_new_map_opcl.d2a", 0);
    mD70->fn_800D246C(1);
    mD74 = fn_801F3FF8(this, "/data/d2anime/ui_event_icon_new_map_loop.d2a", 0);
    mD74->fn_800D246C(1);
    mDB4 = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/camp_info_bar.d2a", 0);
    mDB4->fn_800D246C(1);
    mDB8 = fn_801F3FF8(this, "/data/d2anime/ui_item_pop.d2b", 0);
    mDB8->fn_800D246C(1);
    mDBC = fn_801F3FF8(this, "/data/d2anime/ui_item_atari.d2b", 0);
    mDBC->fn_800D246C(1);
    mDC0 = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_music.d2b", 0);
    mDC0->fn_800D246C(1);
}

// inferred: creates the rest of the HUD (marker textures, gauges of the overlook view, result and replay
// parts, effects); called by game/GameTask, released by fn_803D11B4
// NONMATCHING: 99.5% - register allocation of the loops (counters and induction pointers swapped)
void GameUI::fn_803D0B20() {
    if (mDC8 == 0) {
        mDC8 = 1;
        mDD4.fn_800D5908("data/texture/icon_mark2.texture");
        mE04.fn_800D5908("data/texture/icon_mark2_shine.texture");
        mE34.fn_800D5908("data/texture/icon_mark.texture");
        mE64.fn_800D5908("data/texture/icon_mark_shine.texture");
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 5; j++) {
                m5BC[i][j] = atn::D3AnimeInstance::Create(this, "/data/d2anime/ui_hate_slow2.d2a", 1);
                m5BC[i][j]->fn_800D246C(1);
            }
        }
        for (int i = 0; i < 10; i++) {
            const char* point = "/data/d2anime/ui_magic_point.d2a";
            const char* speed = "/data/d2anime/ui_magic_speed.d2a";
            const char* giga = "/data/d2anime/ui_magic_point_giga.d2a";
            mA90[i] = fn_801F64D0(this, point);
            mA90[i]->fn_800D246C(1);
            mAB8[i] = atn::D2A_TaskInstanceBase<8, 2>::Create(this, speed);
            mAB8[i]->fn_800D246C(1);
            mAE0[i] = atn::D2A_TaskInstanceBase<8, 2>::Create(this, giga);
            mAE0[i]->fn_800D246C(1);
        }
        for (int i = 0; i < 32; i++) {
            const char* path = "data/d2anime/ui_overlook_info.d2a";
            mF14[i] = atn::D2A_TaskInstanceBase<64, 8>::Create(this, path);
            mF14[i]->fn_800D246C(1);
            mF94[i] = 0;
            m1014[i] = i;
        }
        for (int i = 0; i < 8; i++) {
            const char* path = "/data/d2anime/ui_overlook_skill.d2a";
            m10B4[i] = fn_801F64D0(this, path);
            m10B4[i]->fn_800D246C(1);
            m10D4[i] = 0;
        }
        m10F4 = fn_801F3FF8(this, "data/d2anime/ui_cursor.d2a", 0);
        m10F4->fn_800D246C(1);
        m10F8 = fn_801F3FF8(this, "/data/d2anime/icon_rotta_ring.d2a", 0);
        m10F8->fn_800D246C(1);
        m844 = fn_801F3FF8(this, "/data/d2anime/ui_slash_shine.d2a", 0);
        m844->fn_800D246C(1);
        m870 = fn_801F3FF8(this, "/data/d2anime/ui_lever_rotate.d2a", 0);
        m870->fn_800D246C(1);
        m848[0] = fn_801F3FF8(this, "/data/d2anime/ui_point_slash.d2a", 0);
        m848[0]->fn_800D246C(1);
        for (int i = 0; i < 4; i++) {
            const char* path = "/data/d2anime/ui_point_slash.d2a";
            m848[i + 1] = fn_801F64D0(this, path);
            m848[i + 1]->fn_800D246C(1);
        }
        m874[0] = fn_801F3FF8(this, "/data/d2anime/ui_point_slash.d2a", 0);
        m874[0]->fn_800D246C(1);
        for (int i = 0; i < 4; i++) {
            const char* path = "/data/d2anime/ui_point_slash.d2a";
            m874[i + 1] = fn_801F64D0(this, path);
            m874[i + 1]->fn_800D246C(1);
        }
        mB08[0] = fn_801F3FF8(this, "data/d2anime/ui_replay_open.d2a", 0);
        mB08[0]->fn_800D246C(1);
        mB08[1] = fn_801F3FF8(this, "data/d2anime/ui_replay_wait.d2a", 0);
        mB08[1]->fn_800D246C(1);
        mB08[2] = fn_801F3FF8(this, "data/d2anime/ui_replay_shake_open.d2a", 0);
        mB08[2]->fn_800D246C(1);
        mB08[3] = fn_801F3FF8(this, "data/d2anime/ui_replay_shake_wait.d2a", 0);
        mB08[3]->fn_800D246C(1);
        mB08[4] = fn_801F3FF8(this, "data/d2anime/ui_replay_on.d2a", 0);
        mB08[4]->fn_800D246C(1);
        mB08[5] = fn_801F3FF8(this, "data/d2anime/ui_replay_loop.d2a", 0);
        mB08[5]->fn_800D246C(1);
        mB20[0] = fn_801F3FF8(this, "data/d2anime/ui_replay_loop_open.d2a", 0);
        mB20[0]->fn_800D246C(1);
        mB20[1] = fn_801F3FF8(this, "data/d2anime/ui_remain.d2a", 0);
        mB20[1]->fn_800D246C(1);
        for (int i = 0; i < 4; i++) {
            mB20[i + 2] = fn_801F3FF8(this, "data/d2anime/ui_remain_list.d2a", 0);
            mB20[i + 2]->fn_800D246C(1);
        }
        m1104 = fn_801F3FF8(this, "data/d2anime/ui_result_LvUPchara_op_xx.d2a", 0);
        m1104->fn_800D246C(1);
        m1108 = fn_801F3FF8(this, "data/d2anime/ui_result_newskill.d2a", 0);
        m1108->fn_800D246C(1);
        m1110 = fn_801F3FF8(this, "data/d2anime/ui_result_param_op.d2a", 0);
        m1110->fn_800D246C(1);
        m1114 = fn_801F3FF8(this, "data/d2anime/ui_result_param_loop.d2a", 0);
        m1114->fn_800D246C(1);
        m1140 = fn_801F3FF8(this, "/data/d2anime/ui_weakpoint_btn_opcl.d2a", 0);
        m1140->fn_800D246C(1);
        m1144 = fn_801F3FF8(this, "/data/d2anime/ui_weakpoint_btn_loop.d2a", 0);
        m1144->fn_800D246C(1);
        m1148 = fn_801F3FF8(this, "/data/d2anime/ui_weakpoint_text.d2a", 0);
        m1148->fn_800D246C(1);
        m1158.LoadRequest("data/eff/ef_ui006e_01.eff");
        m1164.LoadRequest("data/eff/ef_ui006d_01.eff");
        m1554[0].LoadRequest("data/eff/ef_ui014e_01.eff");
        m1584[0].LoadRequest("data/eff/ef_ui014d_01.eff");
        m1554[1].LoadRequest("data/eff/ef_ui015e_01.eff");
        m1584[1].LoadRequest("data/eff/ef_ui015d_01.eff");
        m1554[2].LoadRequest("data/eff/ef_ui016e_01.eff");
        m1584[2].LoadRequest("data/eff/ef_ui016d_01.eff");
        m1554[3].LoadRequest("data/eff/ef_ui017e_01.eff");
        m1584[3].LoadRequest("data/eff/ef_ui017d_01.eff");
        m2544 = fn_801F3FF8(this, "/data/d2anime/ui_event_icon_new_map_opcl.d2a", 0);
        m2544->fn_800D246C(1);
        m2548 = fn_801F3FF8(this, "/data/d2anime/ui_event_icon_new_map_loop.d2a", 0);
        m2548->fn_800D246C(1);
        m254C = fn_801F3FF8(this, "/data/d2anime/ui_magic_3.d2b", 0);
        m254C->fn_800D246C(1);
        m2550[0] = fn_801F3FF8(this, "/data/d2anime/ui_in_coming_enemy.d2b", 0);
        m2550[1] = fn_801F3FF8(this, "/data/d2anime/ui_strong_enemy.d2b", 0);
        m2550[2] = fn_801F3FF8(this, "/data/d2anime/ui_tension_max.d2b", 0);
        for (int i = 0; i < 3; i++) {
            if (m2550[i] != 0) {
                m2550[i]->fn_800D246C(1);
            }
        }
        m2564 = fn_801F3FF8(this, "/data/d2anime/ui_circle_break.d2b", 0);
        m2564->fn_800D246C(1);
        for (int i = 0; i < 4; i++) {
            const char* path = "/data/d2anime/ui_sield.d2a";
            m2610[i] = atn::D2A_TaskInstanceBase<8, 2>::Create(this, path);
            m2610[i]->fn_800D246C(1);
        }
        m2628 = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/ui_lockon.d2b", 0);
        m2628->fn_800D246C(1);
        mDC4 = (GameUIGauge_unk*)fn_801F3FF8(this, "/data/d2anime/arena_clear_xx.d2b", 0);
        mDC4->fn_800D246C(1);
    }
}

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

// inferred: hides and stops the windows and marks of the HUD (called by game/GameTask)
void GameUI::fn_803D164C() {
    m50.m0C->fn_800D246C(0);
    m50.m0C->FreezeTask();
    m50.m10->fn_800D246C(0);
    m50.m10->FreezeTask();
    if (m50.m14 != 0) {
        m50.m14->fn_800D246C(0);
        m50.m14->SetLoop_unk(0);
        m50.m14->FreezeTask();
    }
    m50.m18->fn_800D246C(0);
    m50.m18->FreezeTask();
    if (m50.m1C != 0) {
        m50.m1C->fn_800D246C(0);
        m50.m1C->FreezeTask();
    }
    m50.m20->fn_800D246C(0);
    m50.m20->SetLoop_unk(0);
    m50.m20->FreezeTask();
    m50.m2C->fn_800D246C(0);
    m50.m2C->SetLoop_unk(0);
    m50.m2C->SetEnd_unk();
    m50.m2C->FreezeTask();
    m50.m24->fn_800D246C(0);
    m50.m24->FreezeTask();
    m50.m28->fn_800D246C(0);
    m50.m28->FreezeTask();
    for (int i = 0; i < 6; i++) {
        mB0[i].m0C->fn_800D246C(0);
        mB0[i].m0C->FreezeTask();
        mB0[i].m10->fn_800D246C(0);
        mB0[i].m10->FreezeTask();
        if (mB0[i].m14 != 0) {
            mB0[i].m14->fn_800D246C(0);
            mB0[i].m14->SetLoop_unk(0);
            mB0[i].m14->FreezeTask();
        }
        mB0[i].m18->fn_800D246C(0);
        mB0[i].m18->FreezeTask();
        if (mB0[i].m1C != 0) {
            mB0[i].m1C->fn_800D246C(0);
            mB0[i].m1C->FreezeTask();
        }
        mB0[i].m20->fn_800D246C(0);
        mB0[i].m20->SetLoop_unk(0);
        mB0[i].m20->FreezeTask();
        mB0[i].m2C->fn_800D246C(0);
        mB0[i].m2C->SetLoop_unk(0);
        mB0[i].m2C->SetEnd_unk();
        mB0[i].m2C->FreezeTask();
        for (int j = 0; j < 5; j++) {
            m318[i].mInst[j]->fn_800D246C(0);
            m318[i].mInst[j]->SetLoop_unk(0);
            m318[i].mInst[j]->SetEnd_unk();
            m318[i].mInst[j]->FreezeTask();
        }
        m318[i].m14->fn_800D246C(0);
        m318[i].m14->SetLoop_unk(0);
        m318[i].m14->SetEnd_unk();
        m318[i].m14->FreezeTask();
        m318[i].m18->fn_800D246C(0);
        m318[i].m18->SetLoop_unk(0);
        m318[i].m18->SetEnd_unk();
        m318[i].m18->FreezeTask();
    }
    m3EC->fn_800D246C(0);
    m3EC->mRepeat = 0;
    for (int i = 0; i < 8; i++) {
        m574[i]->fn_800D246C(0);
        m574[i]->FreezeTask();
        m594[i]->fn_800D246C(0);
        m594[i]->FreezeTask();
    }
    m5B8->fn_800D246C(0);
    m5B8->FreezeTask();
    m3D8->mRepeat = 0;
    m3D8->fn_800D246C(0);
    m3D8->FreezeTask();
    for (int i = 0; i < 4; i++) {
        m3DC[i]->fn_800D246C(0);
        m3DC[i]->FreezeTask();
        fn_801F6C80(m3DC[i], "line", i);
    }
    for (int i = 0; i < 6; i++) {
        m6FC[i]->fn_800D246C(0);
        m6FC[i]->SetLoop_unk(0);
        m6FC[i]->FreezeTask();
    }
    m714[0]->fn_800D246C(0);
    m714[0]->mRepeat = 0;
    m714[0]->FreezeTask();
    m71C[0]->fn_800D246C(0);
    m71C[0]->FreezeTask();
    m714[1]->fn_800D246C(0);
    m714[1]->mRepeat = 0;
    m714[1]->FreezeTask();
    m71C[1]->fn_800D246C(0);
    m71C[1]->FreezeTask();
    m72C->fn_800D246C(0);
    m72C->mRepeat = 0;
    m72C->FreezeTask();
    m730->fn_800D246C(0);
    m730->FreezeTask();
    for (int i = 0; i < 12; i++) {
        m8A0[i]->fn_800D246C(0);
        m8A0[i]->FreezeTask();
        m8A0[i]->SetLoop_unk(0);
        m8A0[i]->SetSpeed_unk(2.0f);
    }
    mA84->fn_800D246C(0);
    mA84->FreezeTask();
    mA84->mRepeat = 0;
    mA84->SetSpeed_unk(1.0f);
    mA84->SetEnd_unk();
    mA88->fn_800D246C(0);
    mA88->FreezeTask();
    mA88->mRepeat = 0;
    mA88->SetSpeed_unk(1.0f);
    mA88->SetEnd_unk();
    m73C->fn_800D246C(0);
    m73C->mRepeat = 0;
    m73C->FreezeTask();
    m740->fn_800D246C(0);
    m740->mRepeat = 0;
    m740->FreezeTask();
    m744->fn_800D246C(0);
    m744->mRepeat = 1;
    m744->FreezeTask();
    m7B8->fn_800D246C(0);
    m7B8->mRepeat = 0;
    m7B8->FreezeTask();
    m7BC->fn_800D246C(0);
    m7BC->mRepeat = 0;
    m7BC->FreezeTask();
    for (int i = 0; i < 2; i++) {
        m814[i]->fn_800D246C(0);
        m814[i]->mRepeat = 0;
        m814[i]->FreezeTask();
        m814[i + 2]->fn_800D246C(0);
        m814[i + 2]->FreezeTask();
        m814[i + 4]->fn_800D246C(0);
        m814[i + 4]->mRepeat = 0;
        m814[i + 4]->FreezeTask();
        if (m814[i + 6] != 0) {
            m814[i + 6]->fn_800D246C(0);
            m814[i + 6]->mRepeat = 1;
            m814[i + 6]->FreezeTask();
        }
    }
    m83C->fn_800D246C(0);
    m83C->mRepeat = 0;
    m83C->FreezeTask();
    m840->fn_800D246C(0);
    m840->mRepeat = 1;
    m840->FreezeTask();
    m838 = 0;
    for (int i = 0; i < 8; i++) {
        mB38[i].m00->fn_800D246C(0);
        mB38[i].m00->FreezeTask();
        mB38[i].m00->GetTask()->mLoop = 0;
        mB38[i].m04->fn_800D246C(0);
        mB38[i].m04->FreezeTask();
        mB38[i].m08->fn_800D246C(0);
        mB38[i].m08->FreezeTask();
        mB38[i].m08->GetTask()->mLoop = 0;
        mB38[i].m0C->fn_800D246C(0);
        mB38[i].m0C->FreezeTask();
        mB38[i].m10->fn_800D246C(0);
        mB38[i].m10->FreezeTask();
        mB38[i].m10->GetTask()->mLoop = 0;
        mB38[i].m14->fn_800D246C(0);
        mB38[i].m14->FreezeTask();
        mB38[i].m18->fn_800D246C(0);
        mB38[i].m18->FreezeTask();
        mB38[i].m18->GetTask()->mLoop = 0;
        mB38[i].m1C->fn_800D246C(0);
        mB38[i].m1C->FreezeTask();
    }
    m2654->fn_800D246C(0);
    m2654->FreezeTask();
    m2654->mRepeat = 0;
    m2644 = 0;
    m2648 = 0;
    m264C = 45;
    m2650 = 0;
    mD70->fn_800D246C(0);
    mD70->FreezeTask();
    mD70->mRepeat = 0;
    mD74->fn_800D246C(0);
    mD74->FreezeTask();
    mDB4->fn_800D246C(0);
    mDB4->mRepeat = 0;
    mDB8->fn_800D246C(0);
    mDB8->mRepeat = 0;
    mDB8->SetSpeed_unk(0.75f);
    mDB8->SetEnd_unk();
    mDB8->FreezeTask();
    mDBC->fn_800D246C(0);
    mDBC->mRepeat = 0;
    mDBC->SetSpeed_unk(0.75f);
    mDBC->SetEnd_unk();
    mDBC->FreezeTask();
    mDC0->fn_800D246C(0);
    mDC0->mRepeat = 0;
    mDC0->SetEnd_unk();
    mDC0->FreezeTask();
    fn_803DF5B4();
}

// inferred: the instances of mAB8/mAE0/m2610 (D2A_TaskInstanceBase<8, 2>) and mF14 (<64, 8>) are kept as
// atn::Task* in game/GameUI.h (a template type there renumbers the @N labels of the units that include it)
typedef atn::D2A_TaskInstanceBase<8, 2> GameUISmallInst_unk;
inline GameUISmallInst_unk* AsSmallInst_unk(atn::Task* task) {
    return static_cast<GameUISmallInst_unk*>(task);
}
inline atn::D2A_Task* AsD2A_unk(atn::Task* task) {
    return static_cast<atn::D2A_Task*>(task);
}
inline atn::D2A_TaskInstanceBase<64, 8>* AsWindow_unk(atn::Task* task) {
    return static_cast<atn::D2A_TaskInstanceBase<64, 8>*>(task);
}

// inferred: hides and stops every part of the HUD created by fn_803D022C/fn_803D0B20 (called by
// game/GameTask)
// NONMATCHING: 99.9% - the unrolled second loop swaps r4/r5 (counter and pointer); tried the counter outside
// the loop and unsigned
void GameUI::fn_803D2194() {
    if (mDC8 != 0) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 5; j++) {
                m5BC[i][j]->fn_800D246C(0);
                m5BC[i][j]->FreezeTask();
            }
        }
        for (int i = 0; i < 10; i++) {
            mA90[i]->fn_800D246C(0);
            mA90[i]->SetSpeed_unk(0.0f);
            mAB8[i]->fn_800D246C(0);
            AsSmallInst_unk(mAB8[i])->SetLoop_unk(0);
            AsSmallInst_unk(mAB8[i])->SetSpeed_unk(1.0f);
            mAE0[i]->fn_800D246C(0);
            AsSmallInst_unk(mAE0[i])->SetSpeed_unk(0.0f);
        }
        mA8C = 0;
        for (int i = 0; i < 10; i++) {
            mA90[i]->FreezeTask();
            if (mAB8[i]->IsFlag_unk(4)) {
                AsSmallInst_unk(mAB8[i])->Rewind_unk();
            }
            mAB8[i]->FreezeTask();
            mAE0[i]->FreezeTask();
        }
        for (int i = 0; i < 32; i++) {
            mF14[i]->fn_800D246C(0);
            AsWindow_unk(mF14[i])->SetLoop_unk(0);
            AsWindow_unk(mF14[i])->Rewind_unk();
            mF14[i]->FreezeTask();
        }
        for (int i = 0; i < 8; i++) {
            m10B4[i]->fn_800D246C(0);
            m10B4[i]->SetLoop_unk(0);
            m10B4[i]->Rewind_unk();
            m10B4[i]->FreezeTask();
        }
        m10F4->fn_800D246C(0);
        m10F4->mRepeat = 0;
        m10F4->Rewind_unk();
        m10F4->FreezeTask();
        m10F8->fn_800D246C(0);
        m10F8->mRepeat = 0;
        m10F8->Rewind_unk();
        m10F8->FreezeTask();
        m844->fn_800D246C(0);
        m844->mRepeat = 1;
        m844->FreezeTask();
        m870->fn_800D246C(0);
        m870->mRepeat = 0;
        m870->FreezeTask();
        m848[0]->fn_800D246C(0);
        static_cast<atn::D2A_Task*>(m848[0])->mRepeat = 0;
        m848[0]->FreezeTask();
        for (int i = 0; i < 4; i++) {
            m848[i + 1]->fn_800D246C(0);
            static_cast<GameUIInst_unk*>(m848[i + 1])->SetLoop_unk(0);
            m848[i + 1]->FreezeTask();
        }
        m874[0]->fn_800D246C(0);
        static_cast<atn::D2A_Task*>(m874[0])->mRepeat = 0;
        m874[0]->FreezeTask();
        for (int i = 0; i < 4; i++) {
            m874[i + 1]->fn_800D246C(0);
            static_cast<GameUIInst_unk*>(m874[i + 1])->SetLoop_unk(0);
            m874[i + 1]->FreezeTask();
        }
        mB08[0]->fn_800D246C(0);
        mB08[0]->FreezeTask();
        AsD2A_unk(mB08[0])->mRepeat = 0;
        mB08[1]->fn_800D246C(0);
        mB08[1]->FreezeTask();
        mB08[2]->fn_800D246C(0);
        mB08[2]->FreezeTask();
        AsD2A_unk(mB08[2])->mRepeat = 0;
        mB08[3]->fn_800D246C(0);
        mB08[3]->FreezeTask();
        mB08[4]->fn_800D246C(0);
        mB08[4]->FreezeTask();
        AsD2A_unk(mB08[4])->mRepeat = 0;
        mB08[5]->fn_800D246C(0);
        mB08[5]->FreezeTask();
        mB20[0]->fn_800D246C(0);
        mB20[0]->FreezeTask();
        AsD2A_unk(mB20[0])->mRepeat = 0;
        mB20[1]->fn_800D246C(0);
        mB20[1]->FreezeTask();
        AsD2A_unk(mB20[1])->mRepeat = 0;
        for (int i = 0; i < 4; i++) {
            mB20[i + 2]->fn_800D246C(0);
            mB20[i + 2]->FreezeTask();
            AsD2A_unk(mB20[i + 2])->mRepeat = 0;
        }
        m1104->fn_800D246C(0);
        m1104->FreezeTask();
        m1104->mRepeat = 0;
        m1104->SetEnd_unk();
        m1108->fn_800D246C(0);
        m1108->FreezeTask();
        m1108->mRepeat = 0;
        m1108->SetEnd_unk();
        m1110->fn_800D246C(0);
        m1110->FreezeTask();
        m1110->mRepeat = 0;
        m1110->SetEnd_unk();
        m1114->fn_800D246C(0);
        m1114->FreezeTask();
        m1114->mRepeat = 0;
        m1114->SetEnd_unk();
        m1140->fn_800D246C(0);
        m1140->FreezeTask();
        m1140->mRepeat = 0;
        m1144->fn_800D246C(0);
        m1144->FreezeTask();
        m1148->fn_800D246C(0);
        m1148->FreezeTask();
        m1148->mRepeat = 0;
        m2544->fn_800D246C(0);
        m2544->FreezeTask();
        m2544->mRepeat = 0;
        m2548->fn_800D246C(0);
        m2548->FreezeTask();
        m254C->fn_800D246C(0);
        m254C->FreezeTask();
        m254C->mRepeat = 0;
        m254C->SetEnd_unk();
        for (int i = 0; i < 3; i++) {
            if (m2550[i] != 0) {
                m2550[i]->fn_800D246C(0);
                m2550[i]->mRepeat = 0;
                m2550[i]->SetEnd_unk();
                m2550[i]->FreezeTask();
            }
        }
        if (m2564 != 0) {
            m2564->fn_800D246C(0);
            m2564->mRepeat = 0;
            m2564->SetEnd_unk();
            m2564->FreezeTask();
        }
        for (int i = 0; i < 4; i++) {
            if (m2610[i] != 0) {
                m2610[i]->fn_800D246C(0);
                AsSmallInst_unk(m2610[i])->SetLoop_unk(1);
                AsSmallInst_unk(m2610[i])->Rewind_unk();
                m2610[i]->FreezeTask();
            }
        }
        if (m2628 != 0) {
            m2628->fn_800D246C(0);
            m2628->mRepeat = 0;
            m2628->Rewind_unk();
            m2628->SetSpeed_unk(-1.0f);
            m2628->FreezeTask();
        }
        if (mDC4 != 0) {
            mDC4->fn_800D246C(0);
            mDC4->mRepeat = 0;
            mDC4->SetEnd_unk();
            mDC4->FreezeTask();
        }
    }
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

// inferred: the render settings (lbl_8087EFA8): the HUD sets the damage effect (scene colour) while the player is
// in danger (RenderSetting_unk::SetDamage_unk, weak copies in game/GameTask.cpp)
extern "C" RenderSetting_unk* fn_802F0990();                // inferred: returns lbl_8087EFA8
extern "C" float fn_802F0988(RenderSetting_unk* settings);  // inferred: frame time (+0x3A4)

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
extern "C" int fn_80244CA0(atn::Task* table);                      // inferred: (flags >> 1) & 1

extern "C" GameUISessionPlayer_unk* fn_804EB870(GameUISession_unk* s); // inferred: local player
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
extern "C" Event::Manager* fn_8000DB1C();                   // inferred: returns lbl_8087F540
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
extern "C" float fn_8006EF48(void* font, float size, const wchar_t* text, float space, int a, int b); // inferred: width
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
        if (fn_80121F00()->GetState_unk() == 8 && fn_8000DB1C() != 0 && fn_8000DB1C()->fn_80481654() != 0) {
            resultShown = true;
        }
        bool eventShown = false;
        if (fn_80121F00()->GetState_unk() == 8 && (fn_8000DB1C() == 0 || fn_8000DB1C()->fn_80481654() == 0)) {
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
            if (((GameTask*)fn_80121F00())->fn_80370174(0x2B) == 1) {
                DamageEffectParam_unk fog;
                fog.mEnabled = 1;
                fog.mAngle = 1.0f;
                fog.mColor.Set_unk(0.2f, 0.13f, 0.08f, 1.0f);
                fog.mZoom = -0.2f;
                fog.mScale = 0.0f;
                fn_802F0990()->SetDamage_unk(fog);
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
                DamageEffectParam_unk fog = *fn_802F0990()->GetDamage_unk();
                fog.mEnabled = !(m4C < -0.8f);
                m48 += fn_802F0988(fn_802F0990());
                if (m48 >= 60.0f) {
                    m48 -= 60.0f;
                }
                float wave = SinF_unk(6.2831855f * (m48 / 60.0f));
                fog.mAngle = atn::DegToRad_unk(0.05f * wave + 1.6f);
                fog.mColor.Set_unk(0.0f, 0.3f, 0.3f, 1.0f);
                fog.mScale = 0.0f;
                fog.mZoom = 0.05f * wave + m4C;
                fn_802F0990()->SetDamage_unk(fog);
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
                                ((GameScene*)fn_80121F00()->fn_8013C504())->fn_803CC900() > 0) {
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
                                              : fn_80315644(m764.m0C->GetColli_unk())->m0C);
                    pos.y += 2.0f;
                    atn::Vector3 screen = fn_8008B964()->GetScreenPosition(pos);
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
                    ((GameTask*)fn_80121F00())->fn_803761A4()) {
                    for (unsigned int i = 0; i < fn_80372574(&m85C); i++) {
                        if (m85C[i] != m764.m0C) {
                            MtxPtr mtx = m85C[i]->GetModel_unk()->GetNodeMtx_unk("head");
                            atn::Vector3 pos(mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk()
                                                      : fn_80315644(m85C[i]->GetColli_unk())->m0C);
                            pos.y += 2.0f;
                            atn::Vector3 screen = fn_8008B964()->GetScreenPosition(pos);
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
                        atn::Vector3 screen = fn_8008B964()->GetScreenPosition(targets[bestIndex].mPos);
                        mark->SetLayoutValue_unk("base_point", 0, screen.x);
                        mark->SetLayoutValue_unk("base_point", 1, screen.y);
                        aim->SetTarget_unk(targets[bestIndex].mChara);
                    }
                    int count = 0;
                    for (unsigned int i = 0; i < fn_80372574(&targets); i++) {
                        if (bestIndex != i) {
                            GameUIInst_unk* mark = static_cast<GameUIInst_unk*>(m874[count + 1]);
                            mark->MeltTask();
                            atn::Vector3 screen = fn_8008B964()->GetScreenPosition(targets[i].mPos);
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
                    m72C->SetParam2_unk("btnUv", &uvCopy1.x);
                }
                {
                    ButtonUv_unk uvCopy2(uv);
                    m72C->SetParam2_unk("btnUvMask", &uvCopy2.x);
                }
                {
                    ButtonUv_unk uvCopy3(uv);
                    m730->SetParam2_unk("btnUv", &uvCopy3.x);
                }
                {
                    ButtonUv_unk uvCopy4(uv);
                    m730->SetParam2_unk("btnUvMask", &uvCopy4.x);
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
            head = mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : fn_80315644(player->GetColli_unk())->m0C;
            head.y += 5.0f;
            atn::Vector3 tmp = fn_8008B964()->GetScreenPosition(head);
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
                                (((GameTask*)fn_80121F00())->fn_80370174(0x4BA) == 30 || ((GameTask*)fn_80121F00())->fn_80370174(0x4C2) == 14)) {
                                show = true;
                            }
                            if (((GameTask*)fn_80121F00())->fn_80370174(0x4BA) == 31 || ((GameTask*)fn_80121F00())->fn_80370174(0x4C2) == 15) {
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
                a = fn_8008B964()->GetScreenPosition(a);
                atn::Vector3 b(m894);
                b = fn_8008B964()->GetScreenPosition(b);
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
                    mD70->SetParam2_unk("btnUv", &uvCopy5.x);
                }
                {
                    ButtonUv_unk uvCopy6(uv);
                    mD70->SetParam2_unk("btnUvMask", &uvCopy6.x);
                }
                {
                    ButtonUv_unk uvCopy7(uv);
                    mD74->SetParam2_unk("btnUv", &uvCopy7.x);
                }
                {
                    ButtonUv_unk uvCopy8(uv);
                    mD74->SetParam2_unk("btnUvMask", &uvCopy8.x);
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
                    float width = fn_8006EF48(GetFont_unk(), 20.0f, name, 0.0f, 1, 1);
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
                    atn::Vector3 pos(mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : fn_80315644(c->GetColli_unk())->m0C);
                    pos.y += 5.0f;
                    atn::Vector3 screen = fn_8008B964()->GetScreenPosition(pos);
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
                        head = mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : fn_80315644(c->GetColli_unk())->m0C;
                        head.y += 5.0f;
                        atn::Vector3 tmp2 = fn_8008B964()->GetScreenPosition(head);
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
                        atn::Vector3 tmp2 = fn_8008B964()->GetScreenPosition(hips);
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
                            atn::Vector3 tmp2 = fn_8008B964()->GetScreenPosition(hips);
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
                atn::Vector3 tmp2 = fn_8008B964()->GetScreenPosition(pos);
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
                    atn::Vector3 screen = fn_8008B964()->GetScreenPosition(pos);
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
                    atn::Vector3 screen = fn_8008B964()->GetScreenPosition(pos);
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
    atn::Vector3 pos = ((atn::Matrix3x4*)chara->mMotion.mMtx.m)->GetTrans_unk();
    atn::Vector3 rot(-1.5707964f, 0.0f, 0.0f);
    float size = 3.5f * (2.0f * chara->m5B0);
    pos.y += 6.0f;
    int kind = chara->m5C->m122;
    TacView* tac = lbl_8087F9F8;
    if (tac != 0 && tac->mA70 != 0 && tac->m48.mState != 2 && tac->IsIdle_unk() && tac->m48.m04 == chara &&
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

// inferred: the tactical view cursor is on a target (name invented)
inline bool IsCursorOn_unk(const TacViewSel_unk& sel) {
    return sel.mState != 2;
}

// inferred: d2anime instance of a character window (mF14; created as D2A_TaskInstanceBase<64, 8>)
typedef atn::D2A_TaskInstanceBase<64, 8> GameUIWindow_unk;
extern "C" void fn_803E6D4C(GameUIWindow_unk* window, chr::CharaData_unk* data); // inferred: job and attribute icons

// inferred: fills the window of party slot `slot` with `chara` (leader mark, position over the head or at
// the bottom of the screen when `fixed`, level and skill name)
// NONMATCHING: 99.4% - only the offsets into the string pool (the functions before it are not written yet)
void GameUI::fn_803DCD34(CharaTask* chara, int slot, int other, int fixed) {
    if (chara != 0) {
        GameUIWindow_unk* window = static_cast<GameUIWindow_unk*>(mF14[m1014[slot]]);
        window->SetValue0_unk("leader", 0.0f);
        window->MeltTask();
        TacView* tac = lbl_8087F9F8;
        if ((IsCursorOn_unk(tac->m48) && tac->IsIdle_unk() && tac->m48.m04 == chara) || other == 0) {
            window->mSpeed = 1.0f;
            window->SetValue1_unk("basePoint", 4, slot + 1);
        } else {
            window->mSpeed = -1.0f;
            window->SetValue1_unk("basePoint", 4, slot + 100);
        }
        if (fixed == 0) {
            MtxPtr mtx = chara->GetModel_unk()->GetNodeMtx_unk("head");
            atn::Vector3 pos(mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : chara->mColli.m48);
            atn::Vector3 screen = fn_8008B964()->GetScreenPosition(pos);
            if (screen.z > 1.0f) {
                screen *= -1.0f;
            }
            window->SetValue1_unk("basePoint", 0, screen.x);
            window->SetValue1_unk("basePoint", 1, screen.y);
        } else {
            atn::GraphicsDevice_unk* gd = lbl_8087EEE0;
            float x = 0.5f * gd->mWidth;
            float y = 0.5f * gd->mHeight;
            window->SetValue1_unk("basePoint", 0, 15.0f + x);
            window->SetValue1_unk("basePoint", 1, y - 15.0f);
        }
        fn_803E6D4C(window, chara->m5C);
        int level = chara->mStatus.mParamB.m78;
        if (chara->mType == 2) {
            level = chara->mStatus.fn_80133BD8(1);
        }
        if (chara->mStatus.IsState_unk(0x400)) {
            level -= 3;
        }
        window->SetText_unk("name", chara->m60->m04);
        if (lbl_8087F610 == 0 || chara->mFlag22 == 1) {
            window->SetValue0_unk("showLevel", 1.0f);
            window->SetNumber_unk("nowLv", level, 0);
        } else {
            window->SetValue0_unk("showLevel", 0.0f);
            window->SetNumber_unk("nowLv", 0, 0);
        }
        HitInfo_unk* skill = 0;
        if (lbl_8087F610 == 0) {
            if (chara->mStatus.m2D0[0].m[0] != 0) {
                skill = fn_80219E6C(chara->mStatus.m2D0[0].m[0]);
            } else if (chara->m648 != 0) {
                skill = ((Arms*)chara->m648)->fn_80044C30(0);
            }
        }
        if (skill != 0 && skill->m04 != 200) {
            window->SetValue0_unk("showSkill", 1.0f);
            window->SetText_unk("jobName", (const wchar_t*)skill->m08);
        } else {
            window->SetValue0_unk("showSkill", 0.0f);
            window->SetText_unk("jobName", lbl_8078C638 + 41);
        }
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
inline void SetJobColor_unk(atn::D2A_Task* task, const char* name, const atn::Color& c) {
    atn::Color tmp = c;
    task->SetColor255_unk(name, &tmp.r);
}


extern "C" void fn_801FEEFC(atn::D2A_Layout_unk* layout, unsigned int hash, int value); // inferred: int parameter
// inferred: int layout parameter of a d2anime task by name (name invented)
inline void SetLayoutInt_unk(atn::D2A_Task* task, const char* name, int value) {
    fn_801FEEFC(&task->mData.m10, fn_800DC6B4(name), value);
}

extern "C" void fn_803DD90C(atn::D2A_Task* task, int job);
// inferred: colour parameter from a colour passed by value (name invented)
inline void SetAttrColor_unk(atn::D2A_Task* task, const char* name, atn::Color c) {
    task->SetColor255_unk(name, &c.r);
}

// inferred: job and attribute icons of a character (attribute: UV of the icon, two colours and the blend
// mode); called by game/NewCampPartsStatus
// NONMATCHING: 87.5% - register allocation (the original keeps the task in r31 and the data in r28), the
// original takes the address of each colour before copying it (same as fn_803DD90C), and the offsets into
// the string pool (the functions before it are not written yet)
extern "C" void fn_803DD160(atn::D2A_Task* task, chr::CharaData_unk* data) {
    atn::Color* colors = lbl_807C8628;
    if (data == 0 || task == 0) {
        return;
    }
    task->SetLayoutValue_unk("job01", 1.0f);
    task->SetLayoutValue_unk("job02", 0.0f);
    fn_803DD90C(task, data->m122);
    int attr = (signed char)data->m120[0];
    if (attr == 2) {
        task->SetLayoutValue_unk("attr01", 1.0f);
        SetJobUv_unk(task, "attrUv01ma", GameUIUvRect_unk(0.25f, 0.25f, 0.5f, 0.5f));
        SetJobUv_unk(task, "attrUv01mb", GameUIUvRect_unk(0.25f, 0.25f, 0.5f, 0.5f));
        SetAttrColor_unk(task, "attrCol01a", colors[7]);
        SetAttrColor_unk(task, "attrCol01b", colors[14]);
        SetLayoutInt_unk(task, "attrBlend", 0);
    } else if (attr == 1) {
        task->SetLayoutValue_unk("attr01", 1.0f);
        SetJobUv_unk(task, "attrUv01ma", GameUIUvRect_unk(0.5f, 0.25f, 0.75f, 0.5f));
        SetJobUv_unk(task, "attrUv01mb", GameUIUvRect_unk(0.5f, 0.25f, 0.75f, 0.5f));
        SetAttrColor_unk(task, "attrCol01a", colors[8]);
        SetAttrColor_unk(task, "attrCol01b", colors[14]);
        SetLayoutInt_unk(task, "attrBlend", 0);
    } else if (attr == 4) {
        task->SetLayoutValue_unk("attr01", 1.0f);
        SetJobUv_unk(task, "attrUv01ma", GameUIUvRect_unk(0.75f, 0.25f, 1.0f, 0.5f));
        SetJobUv_unk(task, "attrUv01mb", GameUIUvRect_unk(0.75f, 0.25f, 1.0f, 0.5f));
        SetAttrColor_unk(task, "attrCol01a", colors[9]);
        SetAttrColor_unk(task, "attrCol01b", colors[14]);
        SetLayoutInt_unk(task, "attrBlend", 0);
    } else if (attr == 3) {
        task->SetLayoutValue_unk("attr01", 1.0f);
        SetJobUv_unk(task, "attrUv01ma", GameUIUvRect_unk(0.0f, 0.5f, 0.25f, 0.75f));
        SetJobUv_unk(task, "attrUv01mb", GameUIUvRect_unk(0.0f, 0.5f, 0.25f, 0.75f));
        SetAttrColor_unk(task, "attrCol01a", colors[11]);
        SetAttrColor_unk(task, "attrCol01b", colors[14]);
        SetLayoutInt_unk(task, "attrBlend", 0);
    } else if (attr == 5) {
        task->SetLayoutValue_unk("attr01", 1.0f);
        SetJobUv_unk(task, "attrUv01ma", GameUIUvRect_unk(0.0f, 0.25f, 0.25f, 0.5f));
        SetJobUv_unk(task, "attrUv01mb", GameUIUvRect_unk(0.0f, 0.25f, 0.25f, 0.5f));
        SetAttrColor_unk(task, "attrCol01a", colors[10]);
        SetAttrColor_unk(task, "attrCol01b", colors[14]);
        SetLayoutInt_unk(task, "attrBlend", 0);
    } else if (attr == 6) {
        task->SetLayoutValue_unk("attr01", 1.0f);
        SetJobUv_unk(task, "attrUv01ma", GameUIUvRect_unk(0.5f, 0.5f, 0.75f, 0.75f));
        SetJobUv_unk(task, "attrUv01mb", GameUIUvRect_unk(0.5f, 0.5f, 0.75f, 0.75f));
        SetAttrColor_unk(task, "attrCol01a", colors[12]);
        SetAttrColor_unk(task, "attrCol01b", colors[15]);
        SetLayoutInt_unk(task, "attrBlend", 3);
    } else if (attr == 7) {
        task->SetLayoutValue_unk("attr01", 1.0f);
        SetJobUv_unk(task, "attrUv01ma", GameUIUvRect_unk(0.75f, 0.5f, 1.0f, 0.75f));
        SetJobUv_unk(task, "attrUv01mb", GameUIUvRect_unk(0.75f, 0.5f, 1.0f, 0.75f));
        SetAttrColor_unk(task, "attrCol01a", colors[13]);
        SetAttrColor_unk(task, "attrCol01b", colors[15]);
        SetLayoutInt_unk(task, "attrBlend", 3);
    } else {
        task->SetLayoutValue_unk("attr01", 0.0f);
    }
}

// inferred: job icon of a character window (UV of the icon and its colours)
// NONMATCHING: 91.2% - the original keeps the colour table in r29 and the entry in r30 (one more saved
// register); tried the colour by value, by reference, with a local copy and a reference per branch
extern "C" void fn_803DD90C(atn::D2A_Task* task, int job) {
    atn::Color* colors = lbl_807C8628;
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

// inferred: colour parameter of a d2anime instance from a colour of the table (see SetWindowColor_unk)
inline void SetInstColor_unk(GameUIInst_unk* inst, const char* name, const atn::Color& c) {
    atn::D2A_RGB_unk rgb;
    rgb.r = c.r;
    rgb.g = c.g;
    rgb.b = c.b;
    inst->SetColor255_unk(name, rgb);
}

// inferred: job icon of a d2anime instance by job index (UV of the icon and its colours; 10 jobs)
// NONMATCHING: the copy of each colour (the original passes the whole atn::Color to SetColor255_unk) and
// registers
extern "C" void fn_803DDD54(GameUIInst_unk* inst, int job) {
    atn::Color* colors = lbl_807C8628;
    switch (job) {
    case 0:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        SetInstColor_unk(inst, "jobCol01a", colors[0]);
        SetInstColor_unk(inst, "jobGradation", colors[0]);
        break;
    case 1:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.5f, 0.0f, 0.75f, 0.25f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.5f, 0.0f, 0.75f, 0.25f));
        SetInstColor_unk(inst, "jobCol01a", colors[4]);
        SetInstColor_unk(inst, "jobGradation", colors[4]);
        break;
    case 2:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.25f, 0.5f, 0.5f, 0.75f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.25f, 0.5f, 0.5f, 0.75f));
        SetInstColor_unk(inst, "jobCol01a", colors[6]);
        SetInstColor_unk(inst, "jobGradation", colors[6]);
        break;
    case 3:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.25f, 0.0f, 0.5f, 0.25f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.25f, 0.0f, 0.5f, 0.25f));
        SetInstColor_unk(inst, "jobCol01a", colors[2]);
        SetInstColor_unk(inst, "jobGradation", colors[2]);
        break;
    case 4:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.25f, 0.0f, 0.5f, 0.25f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.25f, 0.0f, 0.5f, 0.25f));
        SetInstColor_unk(inst, "jobCol01a", colors[3]);
        SetInstColor_unk(inst, "jobGradation", colors[3]);
        break;
    case 5:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.25f, 0.25f, 0.5f, 0.5f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.25f, 0.25f, 0.5f, 0.5f));
        SetInstColor_unk(inst, "jobCol01a", colors[7]);
        SetInstColor_unk(inst, "jobGradation", colors[7]);
        break;
    case 6:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.5f, 0.25f, 0.75f, 0.5f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.5f, 0.25f, 0.75f, 0.5f));
        SetInstColor_unk(inst, "jobCol01a", colors[8]);
        SetInstColor_unk(inst, "jobGradation", colors[8]);
        break;
    case 7:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.75f, 0.25f, 1.0f, 0.5f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.75f, 0.25f, 1.0f, 0.5f));
        SetInstColor_unk(inst, "jobCol01a", colors[9]);
        SetInstColor_unk(inst, "jobGradation", colors[9]);
        break;
    case 8:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.0f, 0.5f, 0.25f, 0.75f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.0f, 0.5f, 0.25f, 0.75f));
        SetInstColor_unk(inst, "jobCol01a", colors[11]);
        SetInstColor_unk(inst, "jobGradation", colors[11]);
        break;
    case 9:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.0f, 0.25f, 0.25f, 0.5f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.0f, 0.25f, 0.25f, 0.5f));
        SetInstColor_unk(inst, "jobCol01a", colors[10]);
        SetInstColor_unk(inst, "jobGradation", colors[10]);
        break;
    default:
        fn_801F7590(inst, "jobUv01ma", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        fn_801F7590(inst, "jobUv01mb", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        SetInstColor_unk(inst, "jobCol01a", colors[5]);
        SetInstColor_unk(inst, "jobGradation", colors[5]);
        break;
    }
}

// inferred: glyphs of the buttons in the button texture (name and UV rect; .data 0x8078BE68, the rects are set
// by the static initializer); names invented
struct GameUIButtonGlyph_unk {
    wchar_t mName[4];
    atn::Vector4_unk mUv;
};
static GameUIButtonGlyph_unk sButtonGlyphs_unk[20] = {
    {L"A", atn::Vector4_unk(0.0f, 0.0f, 0.25f, 0.25f)},
    {L"B", atn::Vector4_unk(0.25f, 0.0f, 0.5f, 0.25f)},
    {L"C", atn::Vector4_unk(0.0f, 0.25f, 0.25f, 0.5f)},
    {L"Z", atn::Vector4_unk(0.75f, 0.0f, 1.0f, 0.25f)},
    {L"1", atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"2", atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"+", atn::Vector4_unk(0.5f, 0.25f, 0.75f, 0.5f)},
    {L"-", atn::Vector4_unk(0.5f, 0.0f, 0.75f, 0.25f)},
    {L"", atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"", atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"", atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"", atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {L"a", atn::Vector4_unk(0.0f, 0.5f, 0.25f, 0.75f)},
    {L"b", atn::Vector4_unk(0.25f, 0.25f, 0.5f, 0.5f)},
    {L"x", atn::Vector4_unk(0.5f, 0.75f, 0.75f, 1.0f)},
    {L"y", atn::Vector4_unk(0.5f, 0.5f, 0.75f, 0.75f)},
    {L"L", atn::Vector4_unk(0.0f, 0.75f, 0.25f, 1.0f)},
    {L"R", atn::Vector4_unk(0.25f, 0.75f, 0.5f, 1.0f)},
    {L"ZL", atn::Vector4_unk(0.75f, 0.5f, 1.0f, 0.75f)},
    {L"ZR", atn::Vector4_unk(0.25f, 0.5f, 0.5f, 0.75f)},
};
// inferred: button help entries (0x1C bytes): flag, icon kind, message id and UV rect (GameUI::fn_803E3468);
// names invented
struct GameUIButtonHelp_unk {
    int m00;
    int mIcon;
    int mMsg;
    atn::Vector4_unk mUv;
};
static GameUIButtonHelp_unk sButtonHelp_unk[18] = {
    {0, 1, 0xB5, atn::Vector4_unk(0.0f, 0.0f, 0.25f, 0.125f)},
    {0, 1, 0xB6, atn::Vector4_unk(0.25f, 0.125f, 0.5f, 0.25f)},
    {0, 0, 0xB7, atn::Vector4_unk(0.5f, 0.0f, 0.75f, 0.125f)},
    {0, 0, 0xB8, atn::Vector4_unk(0.25f, 0.0f, 0.5f, 0.125f)},
    {0, 0, 0xB9, atn::Vector4_unk(0.75f, 0.0f, 1.0f, 0.125f)},
    {0, 0, 0xBA, atn::Vector4_unk(0.5f, 0.0f, 0.75f, 0.125f)},
    {0, 6, 0xBB, atn::Vector4_unk(0.0f, 0.125f, 0.25f, 0.25f)},
    {0, 0, 0xBC, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 7, 0xBD, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 0, 0xBF, atn::Vector4_unk(0.25f, 0.375f, 0.5f, 0.5f)},
    {0, 4, 0xC0, atn::Vector4_unk(0.5f, 0.125f, 0.75f, 0.25f)},
    {0, 4, 0xC2, atn::Vector4_unk(0.5f, 0.25f, 0.75f, 0.375f)},
    {1, 5, 0xC4, atn::Vector4_unk(0.25f, 0.375f, 0.5f, 0.5f)},
    {0, 3, 0xC8, atn::Vector4_unk(0.75f, 0.25f, 1.0f, 0.375f)},
    {0, 4, 0xC9, atn::Vector4_unk(0.5f, 0.125f, 0.75f, 0.25f)},
    {0, 2, 0xCA, atn::Vector4_unk(0.5f, 0.5f, 0.75f, 0.625f)},
    {0, 6, 0xCB, atn::Vector4_unk(0.75f, 0.5f, 1.0f, 0.625f)},
    {0, 3, 0xCC, atn::Vector4_unk(0.75f, 0.25f, 1.0f, 0.375f)},
};
static GameUIButtonHelp_unk sButtonHelp2_unk[23] = {
    {0, 0, 0xCD, atn::Vector4_unk(0.0f, 0.125f, 0.25f, 0.25f)},
    {0, 6, 0xCE, atn::Vector4_unk(0.75f, 0.125f, 1.0f, 0.25f)},
    {0, 6, 0xCF, atn::Vector4_unk(0.0f, 0.125f, 0.25f, 0.25f)},
    {0, 5, 0xD0, atn::Vector4_unk(0.75f, 0.375f, 1.0f, 0.5f)},
    {0, 6, 0xD1, atn::Vector4_unk(0.25f, 0.375f, 0.5f, 0.5f)},
    {0, 7, 0xD2, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xD3, atn::Vector4_unk(0.0f, 0.5f, 0.25f, 0.625f)},
    {0, 6, 0xD4, atn::Vector4_unk(0.25f, 0.5f, 0.5f, 0.625f)},
    {0, 6, 0xD5, atn::Vector4_unk(0.0f, 0.25f, 0.25f, 0.375f)},
    {0, 6, 0xD6, atn::Vector4_unk(0.0f, 0.375f, 0.25f, 0.5f)},
    {0, 6, 0xD7, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xD8, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xD9, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xDA, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 8, 0xDB, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xDC, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xDD, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xDE, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xDF, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 5, 0xE0, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 5, 0xE1, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xE2, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
    {0, 6, 0xBA, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f)},
};

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
        SkillEffect* bullet = lbl_8087F9E8->GetBullet_unk(i);
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

// NONMATCHING: ~96% - the field pointer and the leader are in r6/r5 in the original (swapped here, also the
// address registers of the copies of the centre and f7/f8); the rest differs only in the @N labels and the
// offsets of @stringBase0 (pool order of the unit); tried the field in a local before/after the leader,
// VecAssign_unk for the copies of mF6C
// inferred: marker `index` of a skill effect: shown near the centre (tactical view or the leader), at the
// head of its owner (or the centre of its collision), with the name of the skill and of its link
void GameUI::fn_803DE89C(SkillEffect* bullet, int index) {
    if (bullet == 0) {
        return;
    }
    bool near = false;
    PlayerTask* player = lbl_8087F8A0->mHead;
    atn::Vector3 center(0.0f, 0.0f, 0.0f);
    bool hasCenter = false;
    GameUIField_unk* field = lbl_8087F430;
    if (field != 0) {
        if (lbl_8087F9F8->m15D8 == 1) {
            center.Assign_unk(field->m910);
            hasCenter = true;
        } else if (field->m868 == 7) {
            center = player->mF6C;
            hasCenter = true;
        } else if (field->m868 == 5 && lbl_8087F610 != 0 && player->m55C == 6 && player->m560 == 13) {
            center = player->mF6C;
            hasCenter = true;
        }
    }
    if (hasCenter) {
        atn::Sphere sphere(bullet->m10, bullet->m5C * bullet->m28);
        float r = 6.0f + sphere.mRadius;
        if ((sphere.mCenter - center).LengthSq_unk() < r * r) {
            near = true;
        }
    }
    if (near) {
        m10B4[index]->MeltTask();
        m10B4[index]->SetSpeed_unk(1.0f);
        fn_801F6D7C(m10B4[index], "base_point", 4, index + 1);
    } else {
        m10B4[index]->MeltTask();
        m10B4[index]->SetSpeed_unk(-1.0f);
        fn_801F6D7C(m10B4[index], "base_point", 4, index + 100);
    }
    atn::Vector3 pos = bullet->m10;
    HitInfo_unk* hit = bullet->GetHit_unk();
    if (bullet->m08 != 0 && hit != 0 && (hit->mAC & 0x100000) == 0x100000) {
        MtxPtr mtx = bullet->m08->GetModel_unk()->GetNodeMtx_unk("head");
        pos = mtx != 0 ? ((atn::Matrix3x4*)mtx)->GetTrans_unk() : bullet->m08->GetColli_unk()->m48;
    }
    atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
    if (screen.x < -200.0f || 1000.0f < screen.x || screen.y < -200.0f || 1000.0f < screen.y ||
        screen.z > 1.0f || screen.z < 0.0f) {
        m10B4[index]->FreezeTask();
        return;
    }
    fn_801F6D7C(m10B4[index], "base_point", 0, screen.x);
    fn_801F6D7C(m10B4[index], "base_point", 1, screen.y);
    HitInfo_unk* skill = bullet->GetHit_unk();
    if (skill != 0) {
        switch (skill->m04) {
        case 7:
        case 10:
        case 13:
        case 16:
        case 17:
        case 18:
        case 20:
        case 0x6D8:
        case 0x4EF8:
            skill = fn_80219E6C(30001);
            break;
        }
        if (skill->m04 == 0x4EF9) {
            skill = fn_80219E6C(30002);
        }
        fn_801F837C(m10B4[index], "skill_name", (const wchar_t*)skill->m08);
    }
    const wchar_t* link = fn_803DF2DC(bullet);
    GameUIColor_unk color = fn_803DF138(bullet);
    fn_801F7DF0(m10B4[index], "backColor01", 255.0f * color.r, 255.0f * color.g, 255.0f * color.b);
    fn_801F7DF0(m10B4[index], "backColor02", 255.0f * color.r, 255.0f * color.g, 255.0f * color.b);
    fn_801F7DF0(m10B4[index], "backColor03", 255.0f * color.r, 255.0f * color.g, 255.0f * color.b);
    fn_801F7DF0(m10B4[index], "backColor04", 255.0f * color.r, 255.0f * color.g, 255.0f * color.b);
    fn_801F837C(m10B4[index], "link_name", link);
}

void GameUI::fn_803DEE24(SkillEffect* bullet, GameUIColor_unk* add, GameUIColor_unk* mul) {
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
GameUIColor_unk GameUI::fn_803DF138(SkillEffect* bullet) {
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

const wchar_t* GameUI::fn_803DF2DC(SkillEffect* bullet) {
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

// inferred: the original calls it with bl from fn_803D164C (MWCC would inline it there)
__declspec(noinline) void GameUI::fn_803DF5B4() {
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

extern "C" int fn_80219558(int id); // inferred: party member of a character id (negative for the others)

// inferred: order of the HUD windows of the characters: the party members first (by member), then the
// others by character id (name invented)
struct GameUIWindowLess_unk {
    bool operator()(GameUIElem60_unk* a, GameUIElem60_unk* b) const {
        int ka = fn_80219558(((CharaTask*)a->m08)->m50);
        int kb = fn_80219558(((CharaTask*)b->m08)->m50);
        if (ka < 0 && kb >= 0) {
            return false;
        }
        if (ka >= 0 && kb < 0) {
            return true;
        }
        if (ka < 0 && kb < 0) {
            return ((CharaTask*)a->m08)->m50 < ((CharaTask*)b->m08)->m50;
        }
        return ka < kb;
    }
};

template <class T>
inline void GameUISwap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}
template <class T>
struct GameUIRef_unk {
    typedef T& type;
};
template <class T>
struct GameUIRef_unk<T&> {
    typedef T& type;
};

// inferred: median of three of the quicksort below (MSL __sort; see game/CharaTask.cpp)
template <class T, class Compare>
void GameUIMedian3_unk(T* a, T* b, T* c, Compare& comp) {
    bool ca = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ca && bc) {
        return;
    }
    if (!ca && !bc) {
        GameUISwap_unk(*a, *b);
    } else {
        if (comp(*b, *a)) {
            GameUISwap_unk(*a, *b);
        }
        if (ca) {
            GameUISwap_unk(*b, *c);
        } else {
            GameUISwap_unk(*a, *c);
        }
    }
}

template <class T, class Compare>
inline T* GameUIMinElement_unk(T* first, T* last, Compare& comp) {
    T* it;
    T* result = first;
    if (first != last) {
        for (it = first + 1; it != last; it++) {
            if (comp(*it, *result)) {
                result = it;
            }
        }
    }
    return result;
}

// NONMATCHING: 99.94% (both instantiations) - in the inlined min_element the original keeps *it in r28 and
// *result in r29 (swapped here); as game/CharaTask.cpp; tried the loop forms of game/CharaTask.cpp and
// include/game/MslSort_unk.h, `end` local, the predicate by const reference
// inferred: MSL __sort of the HUD windows (quicksort with a selection sort below 21 elements; the pivots
// move with a counter of each instantiation, lbl_8087DEF0 and lbl_8087DEF4); as game/GameTask.cpp
template <class T, class Compare>
void GameUISort_unk(T* first, T* last, Compare comp) {
    typedef typename GameUIRef_unk<Compare>::type CompareRef;
    static int sOffset = -4;
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            if (first != last) {
                for (; first != last - 1; first++) {
                    T* min = GameUIMinElement_unk(first, last, comp);
                    if (min != first) {
                        GameUISwap_unk(*min, *first);
                    }
                }
            }
            return;
        }
        T* a = first + (n / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        int n3 = n * 3 / 4;
        T* b = first + (n3 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* i;
        T* j;
        T* pivot = last - 1;
        GameUIMedian3_unk(a, b, pivot, comp);
        i = first;
        j = pivot;
        while (comp(*i, *pivot)) {
            i++;
        }
        while (i != --j && !comp(*j, *pivot)) {
        }
        if (i < j) {
            GameUISwap_unk(*i, *j);
            i++;
            while (true) {
                while (comp(*i, *pivot)) {
                    i++;
                }
                while (!comp(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                GameUISwap_unk(*i, *j);
                i++;
            }
        }
        if (i == first) {
            T* back = last - 1;
            GameUISwap_unk(*i, *pivot);
            i++;
            if (!comp(*first, *back)) {
                while (i != last && !comp(*first, *i)) {
                    i++;
                }
                if (i < back) {
                    GameUISwap_unk(*i, *back);
                }
            }
            j = back;
            if (i < j) {
                while (true) {
                    while (!comp(*first, *i)) {
                        i++;
                    }
                    while (comp(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    GameUISwap_unk(*i, *j);
                    i++;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            GameUISort_unk<T, CompareRef>(first, i, comp);
            first = i;
        } else {
            GameUISort_unk<T, CompareRef>(i, last, comp);
            last = i;
        }
    }
}

// inferred: window of a character, else the first free one (name invented)
inline GameUIElem60_unk* GameUIFindWindow_unk(GameUIElem60_unk* windows, CharaTask* c) {
    for (int k = 0; k < 6; k++) {
        if ((CharaTask*)windows[k].m08 == c) {
            return &windows[k];
        }
    }
    for (int k = 0; k < 6; k++) {
        if (windows[k].m08 == 0) {
            return &windows[k];
        }
    }
    return 0;
}

// NONMATCHING: 99.15% - the original reloads lbl_8087F610 and its count in the body of the loop over the
// session players (here the load of the loop test is reused) and swaps r29/r30; tried a session local, the
// accessor as if/else and with a result variable
// inferred: gives a HUD window (mB0) to each friend (up to 6) and, in multiplayer, to each player of the same
// side, frees the windows of the characters that left and sorts the active windows (m2F0)
void GameUI::fn_803DF648(int arg) {
    FixedArray_unk<CharaTask*, 6> list;
    for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = ((PlayerTask*)c)->mNext) {
        if ((!c->IsFlag_unk(4) || c->m55C == 8) && !c->IsFlag54C_unk(0x2000) && c->mC54 != 0 &&
            c->GetInfo_unk()->m2C > 0 && c->GetInfo_unk()->m2C <= 26) {
            list.PushBack_unk(c);
            if (list.full()) {
                break;
            }
        }
    }
    if (lbl_8087F610 != 0) {
        for (int i = 0; i < lbl_8087F610->m5E8; i++) {
            GameUISession_unk* session = lbl_8087F610;
            GameUISessionPlayer_unk* p = session->GetPlayer_unk(i);
            if (p != fn_804EB870(session) && p->m00 != 0 && p->mD0_31 == 1 && lbl_8087F8A0 != 0 &&
                lbl_8087F8A0->mHead != 0 && lbl_8087F8A0->mHead->fn_8016F3D0(p->m00, 1)) {
                list.PushBack_unk(p->m00);
            }
        }
    }
    for (unsigned int i = 0; i < m2F0.mCount; i++) {
        GameUIElem60_unk* e = m2F0.mData[i];
        e->m04 = 0;
        for (unsigned int j = 0; j < list.mCount; j++) {
            if (list.mData[j] == (CharaTask*)e->m08) {
                e->m04 = 1;
            }
        }
    }
    for (unsigned int j = 0; j < list.mCount; j++) {
        GameUIElem60_unk* e = GameUIFindWindow_unk(mB0, list.mData[j]);
        if (e != 0) {
            e->m08 = (int)list.mData[j];
            e->m04 = 1;
        }
    }
    m2F0.mCount = 0;
    for (int i = 0; i < 6; i++) {
        if (mB0[i].m08 != 0) {
            m2F0.PushBack_unk(&mB0[i]);
        }
    }
    GameUISort_unk(m2F0.begin(), m2F0.end(), GameUIWindowLess_unk());
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

extern "C" int fn_8021A888(HitInfo_unk* hit); // inferred: the magic is an attack magic (as CharaTask.cpp)

// inferred: magic point gauge over a character: the chant of its current magic (kind 0/1 by the magic for
// the party, 2/3 for kind-2 characters), the magic chosen in the tactics view, or the time of the states
// of bits 0 and 31 (kinds 4 and 5); characters of flag B13 draw their own (vfC0)
void GameUI::fn_803E2410(CharaTask* chara) {
    if (chara->mFlagB13) {
        chara->vfC0(this);
        return;
    }
    if (!CanActB_unk(chara)) {
        return;
    }
    if (chara->mType != 0 && chara->mC58.mC0 == 0) {
        return;
    }
    if (chara->mFlag26 != 1) {
        return;
    }
    if (lbl_8087F610 != 0 && chara->mType != 0 && lbl_8087F610->m540 != 2) {
        return;
    }
    if (chara->fn_80160324()) {
        HitInfo_unk* hit = chara->m638;
        if (hit == 0) {
            return;
        }
        int speed = (hit->mAC >> 5) & 1;
        atn::Vector3 pos = chara->vf58();
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
        if (chara->mType == 0) {
            screen = atn::Vector3(320.0f, 170.0f, 0.5f);
        }
        if (0.0f < screen.z && screen.z < 1.0f) {
            int frames = chara->fn_8016DAB0();
            if (chara->mType == 2) {
                if (fn_8021A888(hit)) {
                    fn_803E2A30(2, frames, screen, 1, speed);
                } else {
                    fn_803E2A30(3, frames, screen, 1, 0);
                }
            } else {
                int count = 1;
                PlayerTask* player = lbl_8087F8A0->mHead;
                if (player != 0 && player->mFlag28) {
                    count = 2;
                }
                if (chara->fn_801789D8(0x4FBF, 0)) {
                    count <<= 1;
                }
                if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
                    count <<= 1;
                }
                if (fn_8021A888(hit)) {
                    fn_803E2A30(0, frames, screen, count, 0);
                } else {
                    fn_803E2A30(1, frames, screen, count, 0);
                }
            }
        }
    } else if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
        atn::Vector3 pos = chara->vf58();
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
        if (0.0f < screen.z && screen.z < 1.0f) {
            if (chara->mType == 2) {
                return;
            }
            int count = 1;
            PlayerTask* player = lbl_8087F8A0->mHead;
            if (player != 0 && player->mFlag28) {
                count = 2;
            }
            if (chara->fn_801789D8(0x4FBF, 0)) {
                count <<= 1;
            }
            if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
                count <<= 1;
            }
            HitInfo_unk* hit = lbl_8087F9F8->fn_805ADD3C(chara);
            if (hit != 0) {
                int frames = chara->fn_8016DAB0();
                if (fn_8021A888(hit)) {
                    fn_803E2A30(0, frames, screen, count, 0);
                } else {
                    fn_803E2A30(1, frames, screen, count, 0);
                }
            }
        }
    } else if ((chara->mStatus.m0C & 1) == 1) {
        atn::Vector3 pos = chara->mPos + atn::Vector3(0.0f, 20.0f, 0.0f);
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
        bool show = true;
        if (lbl_8087F610 != 0 && chara->mType != 0) {
            show = false;
        }
        if (show && 0.0f < screen.z && screen.z < 1.0f) {
            int frames = chara->mStatus.fn_801333A4(1);
            if (frames < 0) {
                frames = 0;
            }
            fn_803E2A30(4, frames, screen, 1, 0);
        }
    } else if ((chara->mStatus.m0C & 0x80000000) == 0x80000000) {
        atn::Vector3 pos = chara->mPos + atn::Vector3(0.0f, 15.0f, 0.0f);
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
        if (0.0f < screen.z && screen.z < 1.0f) {
            int frames = chara->mStatus.fn_801333A4(0x80000000);
            if (frames < 0) {
                frames = 0;
            }
            fn_803E2A30(5, frames, screen, 1, 0);
        }
    }
}

// inferred: colour parameter of a d2anime instance from a colour of the HUD (see SetInstColor_unk)
inline void SetInstColorU_unk(GameUIInst_unk* inst, const char* name, GameUIColor_unk c) {
    atn::D2A_RGB_unk rgb;
    rgb.r = c.r;
    rgb.g = c.g;
    rgb.b = c.b;
    inst->SetColor255_unk(name, rgb);
}

// inferred: next magic point gauge of the HUD (mA90/mAB8/mAE0[mA8C]) at `pos`: `frames` left (seconds and
// sub-second frame), colour by `kind`, `count` points lit (the giga gauge when more than one) and the
// speed gauge when `speed`
// NONMATCHING: the colour is passed to SetColor255_unk as D2A_RGB_unk (the original copies the whole
// colour; parameter of atn/D2A_TaskInstance.h, see pendientes)
void GameUI::fn_803E2A30(int kind, int frames, atn::Vector3 pos, int count, int speed) {
    if (mA8C < 10) {
        int seconds = fn_803CE854(frames);
        int sub = fn_803CE8A8(frames);
        GameUIInst_unk* point = mA90[mA8C];
        GameUISmallInst_unk* giga = AsSmallInst_unk(mAB8[mA8C]);
        GameUISmallInst_unk* gauge = AsSmallInst_unk(mAE0[mA8C]);
        point->MeltTask();
        fn_801F6D7C(point, "basePoint", 0, pos.x);
        fn_801F6D7C(point, "basePoint", 1, pos.y);
        giga->SetValue1_unk("basePoint", 0, pos.x);
        giga->SetValue1_unk("basePoint", 1, pos.y);
        gauge->SetValue1_unk("basePoint", 0, pos.x);
        gauge->SetValue1_unk("basePoint", 1, pos.y);
        if (frames == 0) {
            fn_801F837C(point, "damage", lbl_8078C638 + 42);
        } else {
            fn_801F8598(point, "damage", seconds, 0);
        }
        bool lit = false;
        bool many = false;
        int max = (seconds > 12 ? 12 : seconds) < 0 ? 0 : (seconds > 12 ? 12 : seconds);
        GameUIColor_unk color;
        if (kind == 0) {
            lit = true;
            if (count > 1) {
                many = true;
            }
            color = m2728;
        } else if (kind == 1) {
            lit = true;
            if (count > 1) {
                many = true;
            }
            color = m2738;
        } else if (kind == 2) {
            lit = true;
            if (speed != 0) {
                color = m2768;
            } else {
                color = m2748;
            }
        } else if (kind == 3) {
            lit = true;
            color = m2758;
        } else if (kind == 4) {
            color = m2778;
        } else if (kind == 5) {
            color = GameUIColor_unk(240, 0, 0, 255);
        } else {
            color = GameUIColor_unk(240, 170, 0, 255);
        }
        SetInstColorU_unk(point, "baseColor", color);
        fn_801F6C80(point, "colOn", 0.0f);
        fn_801F6C80(point, "colR", 255.0f * color.r);
        fn_801F6C80(point, "colG", 255.0f * color.g);
        fn_801F6C80(point, "colB", 255.0f * color.b);
        for (int i = 0; i < 12; i++) {
            char name[64];
            sprintf(name, "pointCol%02d", i + 1);
            fn_801F791C(point, name, 0, (lit && i < max) ? 255.0f : 0.0f);
        }
        fn_801F791C(point, "hateCol", 0, many ? 255.0f : 0.0f);
        if (many) {
            giga->MeltTask();
            giga->SetValue0_unk("colOn", 0.0f);
            giga->SetValue0_unk("colR", 255.0f * color.r);
            giga->SetValue0_unk("colG", 255.0f * color.g);
            giga->SetValue0_unk("colB", 255.0f * color.b);
            giga->SetNumber_unk("string0003(1)", count, 0);
        }
        if (frames == 0) {
            point->mFrame = 15.0f;
            gauge->mFrame = 15.0f;
        } else {
            point->mFrame = sub;
            gauge->mFrame = sub;
        }
        if (giga->mFrame >= 15.0f) {
            giga->mFrame = 14.0f;
        }
        if (speed != 0) {
            gauge->MeltTask();
            gauge->SetValue0_unk("colOn", 0.0f);
            gauge->SetValue0_unk("colR", 50.0f);
            gauge->SetValue0_unk("colG", 20.0f);
            gauge->SetValue0_unk("colB", 90.0f);
        }
        mA8C++;
    }
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

extern "C" GameConfig_unk* lbl_8087F0A8; // inferred: game settings (line colours of the hate display)
// inferred: object whose name the button prompt shows (GameUIElem1C_unk::m10); name invented
struct GameUINamed_unk {
    int m00;
    int m04;
    const wchar_t* mName;
};
// inferred: UV rect parameter of a d2anime task from a rect passed by value (name invented)
inline void SetUvRect_unk(atn::D2A_Task* task, const char* name, atn::Vector4_unk r) {
    task->SetParam2_unk(name, &r.x);
}

// inferred: button prompt of an action: the item part, the button glyph (by the icon kind of the action, the
// pad, the attack type and the gathering type of the settings), the arrows of the cross pad on `arrows` and
// the name and icon of the action (sButtonHelp2_unk for kind 6, else sButtonHelp_unk)
void GameUI::fn_803E3468(atn::D2A_Task* task, atn::D2A_Task* arrows, GameUIElem1C_unk* button, int force) {
    if (lbl_8087F0A8->mShowActionIcon == 0) {
        return;
    }
    if (force == 0) {
        int kind = button->m00;
        if (kind == 6 || kind == 12 || kind == 9) {
            return;
        }
    }
    task->MeltTask();
    int kind = button->m00;
    int sub = button->m04;
    task->SetLayoutValue_unk("item", sButtonHelp_unk[kind].m00 == 1 ? 1.0f : 0.0f);
    char num[64];
    sprintf(num, "%d", button->m08);
    task->SetText_unk("item_num", (int)num);
    if (button->m10 != 0) {
        SetLayoutText_unk(&task->mData.m10, "item_name", ((GameUINamed_unk*)button->m10)->mName);
    }
    int glyph = 0;
    int icon;
    if (button->m00 == 6) {
        icon = sButtonHelp2_unk[sub].mIcon;
    } else {
        icon = sButtonHelp_unk[kind].mIcon;
    }
    int stick = fn_800A58D0(lbl_8087EF70, 0);
    SetConfig* config = SetConfig::sInstance;
    bool tacview = config->mInTacviewType == 0;
    bool gathering = config->mGatheringType == 0;
    if (stick == 0) {
        if (config->mAttackType == 0) {
            switch (icon) {
            case 0:
                glyph = 0;
                break;
            case 1:
                glyph = 0;
                break;
            case 2:
                glyph = 3;
                if (gathering) {
                    glyph = 2;
                }
                break;
            case 3:
                glyph = 2;
                if (gathering) {
                    glyph = 3;
                }
                break;
            case 4:
                glyph = 0;
                break;
            case 5:
                glyph = 0;
                break;
            case 6:
                glyph = 0;
                break;
            case 7:
                glyph = 1;
                break;
            case 8:
                glyph = 0;
                break;
            }
        } else {
            switch (icon) {
            case 0:
                glyph = 8;
                if (tacview) {
                    glyph = 9;
                }
                break;
            case 1:
                glyph = 8;
                if (tacview) {
                    glyph = 9;
                }
                break;
            case 2:
                glyph = 3;
                if (gathering) {
                    glyph = 2;
                }
                break;
            case 3:
                glyph = 2;
                if (gathering) {
                    glyph = 3;
                }
                break;
            case 4:
                glyph = 0;
                break;
            case 5:
                glyph = 8;
                if (tacview) {
                    glyph = 9;
                }
                break;
            case 6:
                glyph = 0;
                break;
            case 7:
                glyph = 1;
                break;
            case 8:
                glyph = 8;
                if (tacview) {
                    glyph = 9;
                }
                break;
            }
        }
    } else {
        if (config->mAttackType == 0) {
            switch (icon) {
            case 0:
                glyph = 12;
                break;
            case 1:
                glyph = 12;
                break;
            case 2:
                glyph = 18;
                if (gathering) {
                    glyph = 16;
                }
                break;
            case 3:
                glyph = 16;
                if (gathering) {
                    glyph = 18;
                }
                break;
            case 4:
                glyph = 12;
                break;
            case 5:
                glyph = 12;
                break;
            case 6:
                glyph = 12;
                break;
            case 7:
                glyph = 17;
                break;
            case 8:
                glyph = 12;
                break;
            }
        } else {
            switch (icon) {
            case 0:
                glyph = 15;
                break;
            case 1:
                glyph = 15;
                break;
            case 2:
                glyph = 18;
                if (gathering) {
                    glyph = 16;
                }
                break;
            case 3:
                glyph = 16;
                if (gathering) {
                    glyph = 18;
                }
                break;
            case 4:
                glyph = 12;
                break;
            case 5:
                glyph = 15;
                break;
            case 6:
                glyph = 12;
                break;
            case 7:
                glyph = 17;
                break;
            case 8:
                glyph = 15;
                break;
            }
        }
    }
    SetUvRect_unk(task, "btnUv", sButtonGlyphs_unk[glyph].mUv);
    SetUvRect_unk(task, "btnUvMask", sButtonGlyphs_unk[glyph].mUv);
    task->SetLayoutValue_unk("crossOn", 0.0f);
    if ((unsigned int)(glyph - 8) <= 3) {
        task->SetLayoutValue_unk("crossOn", 1.0f);
        if (arrows != 0) {
            arrows->MeltTask();
            arrows->SetLayoutValue_unk("arrowU", 0.0f);
            arrows->SetLayoutValue_unk("arrowD", 0.0f);
            arrows->SetLayoutValue_unk("arrowL", 0.0f);
            arrows->SetLayoutValue_unk("arrowR", 0.0f);
            switch (glyph) {
            case 8:
                arrows->SetLayoutValue_unk("arrowU", 1.0f);
                break;
            case 9:
                arrows->SetLayoutValue_unk("arrowD", 1.0f);
                break;
            case 10:
                arrows->SetLayoutValue_unk("arrowL", 1.0f);
                break;
            case 11:
                arrows->SetLayoutValue_unk("arrowR", 1.0f);
                break;
            }
        }
    }
    if (button->m00 == 6) {
        int msg = sButtonHelp2_unk[sub].mMsg;
        task->SetLayoutText_unk("action_name", lbl_8087F1E4->GetEntry_unk(msg));
        task->SetLayoutText_unk("action_name_sub", lbl_8087F1E4->GetEntry_unk(msg));
        SetUvRect_unk(task, "icon_uv", sButtonHelp2_unk[sub].mUv);
        SetUvRect_unk(task, "icon_mk_uv", sButtonHelp2_unk[sub].mUv);
    } else {
        int msg = sButtonHelp_unk[kind].mMsg;
        task->SetLayoutText_unk("action_name", lbl_8087F1E4->GetEntry_unk(msg));
        task->SetLayoutText_unk("action_name_sub", lbl_8087F1E4->GetEntry_unk(msg));
        SetUvRect_unk(task, "icon_uv", sButtonHelp_unk[kind].mUv);
        SetUvRect_unk(task, "icon_mk_uv", sButtonHelp_unk[kind].mUv);
    }
}

void GameUI::fn_803E3BE8(void* msg, int arg, float time) {
    if (msg != 0 && m2640 != 0) {
        m2640->mCount = 0;
        m2640->fn_803B78E8();
        m2640->fn_803B6970((GameMessage*)msg, arg, time);
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
            atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
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
    m263C->fn_803B57B0(1);
    m2640->fn_803B6C88(1);
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

// inferred: colour of a 0xAARRGGBB value (as game/BattleTask fn_800F8014; name invented)
inline atn::Color GameUIColorU32_unk(unsigned int rgba) {
    return atn::Color(0.003921569f * (float)((rgba >> 16) & 0xFF), 0.003921569f * (float)((rgba >> 8) & 0xFF),
                      0.003921569f * (float)(rgba & 0xFF), 0.003921569f * (float)(rgba >> 24));
}
// inferred: the d2anime instance behind a D3AnimeInstance (D3AnimeInstance::GetTask returns the partial view
// D2AInstance_unk of atn/D3AnimeInstance.h)
inline GameUIInst_unk* GetIconInst_unk(atn::D3AnimeInstance* icon) {
    return (GameUIInst_unk*)icon->GetTask();
}

// inferred: icon over a character (position by fn_803E5200, colour by the kind of attack the character
// uses: white when `white`, else the line colours of the settings) and its frame kept in [0, 40); moves
// `pos` down for the next icon
// NONMATCHING: the colour is passed to SetColor255_unk as D2A_RGB_unk (the original copies the whole
// atn::Color, see pendientes)
void GameUI::fn_803E4E58(atn::D3AnimeInstance* icon, CharaTask* chara, atn::Vector3* pos, int white) {
    if (icon == 0 || chara == 0) {
        return;
    }
    fn_803E5200(icon, chara, pos, white);
    atn::Color color;
    if (white != 0) {
        color = atn::Color(1.0f, 1.0f, 1.0f, 1.0f);
    } else if (chara->mStatus.fn_80134270()) {
        color = GameUIColorU32_unk(lbl_8087F0A8->mLineColorMagic);
    } else if (chara->mStatus.fn_80134250()) {
        color = GameUIColorU32_unk(lbl_8087F0A8->mLineColorArcher);
    } else {
        color = GameUIColorU32_unk(lbl_8087F0A8->mLineColorSword);
    }
    SetInstColor_unk(GetIconInst_unk(icon), "iconColor00", color);
    SetInstColor_unk(GetIconInst_unk(icon), "iconColor01", color);
    SetInstColor_unk(GetIconInst_unk(icon), "iconColor02", color);
    SetInstColor_unk(GetIconInst_unk(icon), "iconColorSdw00", color);
    float zero = 0.0f;
    float end = 40.0f;
    if (zero > GetIconInst_unk(icon)->mFrame) {
        GetIconInst_unk(icon)->mFrame = zero;
    }
    if (end <= GetIconInst_unk(icon)->mFrame) {
        GetIconInst_unk(icon)->mFrame = zero;
    }
    pos->y += 1.5f;
}

// inferred: position of an icon over a character: above the head (or the neck, or the collision centre),
// moved 3 units towards the camera, turned towards the camera, scaled by its distance between the limits
// m2670..m2674 (scales m266C..m2668)
// NONMATCHING: 99.2% - the lerp of the scale adds the base last in the original (atn::Lerp_unk adds it
// first; written the other way the temporaries move) and float registers
void GameUI::fn_803E5200(atn::D3AnimeInstance* icon, CharaTask* chara, atn::Vector3* pos, int) {
    GameUIField_unk* field = lbl_8087F430;
    MtxPtr head = chara->GetModel_unk()->GetNodeMtx_unk("head");
    MtxPtr neck = chara->GetModel_unk()->GetNodeMtx_unk("neck");
    atn::Vector3 top(head != 0   ? ((atn::Matrix3x4*)head)->GetTrans_unk()
                     : neck != 0 ? ((atn::Matrix3x4*)neck)->GetTrans_unk()
                                 : chara->mColli.m48);
    top.y += 5.0f;
    top.y += static_cast<BattleCharaTask*>(chara)->m149C;
    atn::Vector3 dir = field->mEye - top;
    float dist = dir.Length_unk();
    dir.Normalize_unk();
    atn::Vector3 base = top + dir * 3.0f;
    dir.y = 0.0f;
    dir.Normalize_unk();
    atn::Vector3 front;
    front = atn::Vector3(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(icon->mRot.m, &front, &front);
    dir = front + dir;
    dir.Normalize_unk();
    dir = front + dir;
    dir.Normalize_unk();
    atn::Vector3 rot = dir.ToRotation_unk();
    atn::Vector3 large(m2668, m2668, m2668);
    atn::Vector3 small(m266C, m266C, m266C);
    float d = m2670 > (m2674 < dist ? m2674 : dist) ? m2670 : (m2674 < dist ? m2674 : dist);
    atn::Vector3 scale(1.0f, 1.0f, 1.0f);
    scale = atn::Lerp_unk(small, large, d / (m2674 - m2670));
    icon->SetPosition_unk(base + *pos);
    icon->MeltTask();
    atn::Matrix3x4 mtx;
    mtx.fn_8013A13C(rot.y);
    icon->SetRotation_unk(mtx);
    icon->SetScale_unk(scale);
    fn_801F6D7C(GetIconInst_unk(icon), "pos0003", 0, 0.0f);
    fn_801F6D7C(GetIconInst_unk(icon), "pos0003", 1, 0.0f);
    fn_801F6D7C(GetIconInst_unk(icon), "pos0003", 4, d);
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
    atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
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
    if (!((GameTask*)lbl_8087F430)->fn_80370C2C()) {
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
        ((GameTask*)lbl_8087F430)->fn_80370BD0(30, 0, 1e-5f);
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
        ((GameTask*)lbl_8087F430)->fn_80370B78(0xFF000000, 30, 0, 1e-4f);
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

// inferred: colour parameter of a character window; the original passes the whole atn::Color (16 bytes are
// copied), so the parameter of D2A_TaskInstanceBase::SetColor255_unk is probably an atn::Color (name invented)
inline void SetWindowColor_unk(GameUIWindow_unk* task, const char* name, const atn::Color& c) {
    atn::D2A_RGB_unk rgb;
    rgb.r = c.r;
    rgb.g = c.g;
    rgb.b = c.b;
    task->SetColor255_unk(name, rgb);
}

extern "C" void fn_803E746C(GameUIWindow_unk* task, int job);

// inferred: same as fn_803DD160 for a character window (D2A_TaskInstanceBase<64, 8>)
// NONMATCHING: the copy of each colour (see SetWindowColor_unk), registers and the string pool offsets
extern "C" void fn_803E6D4C(GameUIWindow_unk* task, chr::CharaData_unk* data) {
    atn::Color* colors = lbl_807C8628;
    if (data == 0 || task == 0) {
        return;
    }
    task->SetValue0_unk("job01", 1.0f);
    task->SetValue0_unk("job02", 0.0f);
    fn_803E746C(task, data->m122);
    int attr = (signed char)data->m120[0];
    if (attr == 2) {
        task->SetValue0_unk("attr01", 1.0f);
        task->SetValues2_unk("attrUv01ma", atn::Color(0.25f, 0.25f, 0.5f, 0.5f));
        task->SetValues2_unk("attrUv01mb", atn::Color(0.25f, 0.25f, 0.5f, 0.5f));
        SetWindowColor_unk(task, "attrCol01a", colors[7]);
        SetWindowColor_unk(task, "attrCol01b", colors[14]);
        task->SetInt_unk("attrBlend", 0);
    } else if (attr == 1) {
        task->SetValue0_unk("attr01", 1.0f);
        task->SetValues2_unk("attrUv01ma", atn::Color(0.5f, 0.25f, 0.75f, 0.5f));
        task->SetValues2_unk("attrUv01mb", atn::Color(0.5f, 0.25f, 0.75f, 0.5f));
        SetWindowColor_unk(task, "attrCol01a", colors[8]);
        SetWindowColor_unk(task, "attrCol01b", colors[14]);
        task->SetInt_unk("attrBlend", 0);
    } else if (attr == 4) {
        task->SetValue0_unk("attr01", 1.0f);
        task->SetValues2_unk("attrUv01ma", atn::Color(0.75f, 0.25f, 1.0f, 0.5f));
        task->SetValues2_unk("attrUv01mb", atn::Color(0.75f, 0.25f, 1.0f, 0.5f));
        SetWindowColor_unk(task, "attrCol01a", colors[9]);
        SetWindowColor_unk(task, "attrCol01b", colors[14]);
        task->SetInt_unk("attrBlend", 0);
    } else if (attr == 3) {
        task->SetValue0_unk("attr01", 1.0f);
        task->SetValues2_unk("attrUv01ma", atn::Color(0.0f, 0.5f, 0.25f, 0.75f));
        task->SetValues2_unk("attrUv01mb", atn::Color(0.0f, 0.5f, 0.25f, 0.75f));
        SetWindowColor_unk(task, "attrCol01a", colors[11]);
        SetWindowColor_unk(task, "attrCol01b", colors[14]);
        task->SetInt_unk("attrBlend", 0);
    } else if (attr == 5) {
        task->SetValue0_unk("attr01", 1.0f);
        task->SetValues2_unk("attrUv01ma", atn::Color(0.0f, 0.25f, 0.25f, 0.5f));
        task->SetValues2_unk("attrUv01mb", atn::Color(0.0f, 0.25f, 0.25f, 0.5f));
        SetWindowColor_unk(task, "attrCol01a", colors[10]);
        SetWindowColor_unk(task, "attrCol01b", colors[14]);
        task->SetInt_unk("attrBlend", 0);
    } else if (attr == 6) {
        task->SetValue0_unk("attr01", 1.0f);
        task->SetValues2_unk("attrUv01ma", atn::Color(0.5f, 0.5f, 0.75f, 0.75f));
        task->SetValues2_unk("attrUv01mb", atn::Color(0.5f, 0.5f, 0.75f, 0.75f));
        SetWindowColor_unk(task, "attrCol01a", colors[12]);
        SetWindowColor_unk(task, "attrCol01b", colors[15]);
        task->SetInt_unk("attrBlend", 3);
    } else if (attr == 7) {
        task->SetValue0_unk("attr01", 1.0f);
        task->SetValues2_unk("attrUv01ma", atn::Color(0.75f, 0.5f, 1.0f, 0.75f));
        task->SetValues2_unk("attrUv01mb", atn::Color(0.75f, 0.5f, 1.0f, 0.75f));
        SetWindowColor_unk(task, "attrCol01a", colors[13]);
        SetWindowColor_unk(task, "attrCol01b", colors[15]);
        task->SetInt_unk("attrBlend", 3);
    } else {
        task->SetValue0_unk("attr01", 0.0f);
    }
}

// inferred: same as fn_803DD90C for a character window (D2A_TaskInstanceBase<64, 8>)
// NONMATCHING: the copy of each colour (see SetWindowColor_unk) and the string pool offsets
extern "C" void fn_803E746C(GameUIWindow_unk* task, int job) {
    atn::Color* colors = lbl_807C8628;
    if (job == 4) {
        task->SetValues2_unk("jobUv01ma", atn::Color(0.5f, 0.0f, 0.75f, 0.25f));
        task->SetValues2_unk("jobUv01mb", atn::Color(0.5f, 0.0f, 0.75f, 0.25f));
        SetWindowColor_unk(task, "jobCol01a", colors[4]);
        SetWindowColor_unk(task, "jobGradation", colors[4]);
    } else if (job == 2) {
        task->SetValues2_unk("jobUv01ma", atn::Color(0.0f, 0.0f, 0.25f, 0.25f));
        task->SetValues2_unk("jobUv01mb", atn::Color(0.0f, 0.0f, 0.25f, 0.25f));
        SetWindowColor_unk(task, "jobCol01a", colors[1]);
        SetWindowColor_unk(task, "jobGradation", colors[1]);
    } else if (job == 3) {
        task->SetValues2_unk("jobUv01ma", atn::Color(0.25f, 0.0f, 0.5f, 0.25f));
        task->SetValues2_unk("jobUv01mb", atn::Color(0.25f, 0.0f, 0.5f, 0.25f));
        SetWindowColor_unk(task, "jobCol01a", colors[2]);
        SetWindowColor_unk(task, "jobGradation", colors[2]);
    } else if (job == 1) {
        task->SetValues2_unk("jobUv01ma", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        task->SetValues2_unk("jobUv01mb", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        SetWindowColor_unk(task, "jobCol01a", colors[0]);
        SetWindowColor_unk(task, "jobGradation", colors[0]);
    } else {
        task->SetValues2_unk("jobUv01ma", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        task->SetValues2_unk("jobUv01mb", atn::Color(0.75f, 0.0f, 1.0f, 0.25f));
        SetWindowColor_unk(task, "jobCol01a", colors[5]);
        SetWindowColor_unk(task, "jobGradation", colors[5]);
    }
}
