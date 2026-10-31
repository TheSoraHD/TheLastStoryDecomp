#include "game/GameUI.h"
#include "game/CharaTask.h"
#include "atn/SoundRef.h"
#include "string.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" GameUI* lbl_8087F490;      // inferred: the HUD instance
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
extern "C" void fn_803B78E8(void* self); // inferred: closes the window (game/GameChoiceWindow.cpp)
extern "C" void fn_803B6970(GameUIWindow_unk* win, void* msg, int arg, float time); // inferred: shows a message
extern "C" int fn_803CC900(atn::Task* table); // inferred: number of entries
extern "C" void fn_803CFC78(GameUI* ui);   // inferred: creates the parts of the HUD
extern "C" void fn_803E668C(GameUI* ui);

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

// NONMATCHING: 98.1% - scheduling only: (1) the original keeps the load of the list count (Clear of 0x30C)
// after the six resets of m318, here it is hoisted above them (the scheduling windows of the block end one
// node earlier); (2) the zero of the resets after the 0x5BC loop is materialised twice in the original (r3
// up to mB08, r30 from mB20 on, across the memset) and once here (r30). Wrapper structs with constructors
// (W262C_unk, W25CC_unk, W2568_unk, MarkerList_unk) were needed for the scheduling windows and the loop
// bounds of the arrays; `m30C.mMid.mBase.mCount -= (int)...` reproduces the lwz/subf of the inlined Clear.
GameUI::GameUI(atn::Task* parent)
    : atn::Task(parent), m48(0.0f), m4C(-1.0f), m2F0(0), m834(0), m85C(0), m8D0(0), mD84(0), mD88(0),
      mD8C(0), mDA4(0), mDA8(0), mDAC(0), mDB0(0), mDC8(0), m1118(0), m1130(0), m1140(0), m1144(0), m1148(0), m114C(0), m1150(0), m1154(0), m1170(0), m15B4(0), m15B8(0),
      m253C(0), m2540(0), m254C(0), m255C(-1), m2560(0.0f), m2658(0), mSeState(0), mSeMask(0), m2668(1.0f), m266C(0.07f), m2670(5.0f), m2674(3000.0f),
      m2678(128, 128, 128, 255), m2688(10, 10, 128, 255), m2698(128, 128, 128, 255), m26A8(10, 10, 255, 255), m26B8(0, 0, 0, 255), m26C8(19, 59, 23, 255), m26D8(220, 220, 200, 255), m26E8(250, 150, 90, 255), m26F8(140, 170, 220, 255), m2708(60, 100, 70, 255), m2718(250, 240, 130, 255), m2728(255, 180, 0, 0), m2738(0, 180, 80, 0), m2748(255, 100, 100, 0), m2758(90, 90, 90, 0), m2768(50, 20, 90, 0), m2778(150, 100, 200, 0), m288C(255, 255, 255, 0), m289C(85, 112, 206, 255) {
    m2788 = 0;
    for (int i = 0; i < 6; i++) {
        m318[i].m00 = 0;
        m318[i].m04 = 0;
        m318[i].m08 = 0;
        m318[i].m0C = 0;
        m318[i].m10 = 0;
    }
    m30C.mMid.mBase.mCount -= (int)m30C.mMid.mBase.mCount;
    m3D8 = 0;
    m3DC = 0;
    m3E0 = 0;
    m3E4 = 0;
    m3E8 = 0;
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
    for (int i = 0; i < 80; i++) {
        m5BC[i] = 0;
    }
    m6FC = 0;
    m700 = 0;
    m704 = 0;
    m708 = 0;
    m70C = 0;
    m710 = 0;
    m714 = 0;
    m718 = 0;
    m720 = 0;
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
    m8D0 = 0;
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
    m85C = 0;
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
    for (int i = 0; i < 5; i++) {
        m1104[i] = 0;
    }
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
    for (int i = 0; i < 7; i++) {
        m2610[i] = 0;
    }
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

// NONMATCHING: 99.95% - only the int-to-float double differs: it is in the auto .rodata (0x80750650), outside
// the split (the .rodata of the real file also holds the tables of fn_803CFC78).
void GameUI::vf10() {
    fn_803E668C(this);
    if (m840 != 0 && !m840->IsFlag_unk(4) && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0 &&
        lbl_8087F430->m10D8->IsFlag_unk(2) && fn_803CC900(lbl_8087F430->m10D8) > 0) {
        if (mSeState == 0) {
            if (m840->m100 >= 4.0f) {
                fn_800C31F4(lbl_80750618[4], 0, -1, 0.6f);
                mSeState++;
            }
        } else if (mSeState % 2 == 1) {
            if (m840->m100 < 4.0f) {
                mSeState = (mSeState + 1) % 14;
            }
        } else if (mSeState % 2 == 0) {
            if (m840->m100 >= 4.0f) {
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

bool GameUIGauge_unk::fn_803CFC58() {
    return (mFlags & 4) != 0;
}

float GameUIGauge_unk::fn_803CFC64() {
    return m100;
}

float GameUIDuration_unk::fn_803CFC6C() {
    return mA0;
}

void GameUI::vf14() {}

void GameUI::fn_803E32EC() {
    memcpy(&m748, &m764, sizeof(GameUIElem1C_unk));
    m764 = GameUIElem1C_unk();
    memcpy(&m7F8, &m7C0, sizeof(GameUIElem1C_unk));
    m7C0 = GameUIElem1C_unk();
}

void GameUI::fn_803E3BE8(void* msg, int arg, float time) {
    if (msg != 0 && m2640 != 0) {
        m2640->mB4 = 0;
        fn_803B78E8(m2640);
        fn_803B6970(m2640, msg, arg, time);
        m2658 = 0;
    }
}

void GameUI::fn_803E4100() {
    m748 = GameUIElem1C_unk();
    m764 = GameUIElem1C_unk();
    m780 = GameUIElem1C_unk();
    for (int i = 0; i < 3; i++) {
        m814[i * 2]->m100 = m814[i * 2]->mA0;
    }
    for (int i = 0; i < 3; i++) {
        m814[i * 2 + 1]->m100 = m814[i * 2 + 1]->mA0;
    }
    m834 = 0;
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
        m2654->m104 = -1.0f;
        m2654->m100 = 0.0f;
    }
}

void GameUI::fn_803E4950() {
    mD90.m00 = 0;
    mDB0 = 0;
    mDB4->m100 = 0.0f;
    mDB4->m104 = -1.0f;
}

void GameUI::fn_803E4A20(int a, int b) {
    if (a) {
        mD90.m00 = 1;
        mD90.m04 = 1;
        mDA4 = a;
        mDA8 = b;
        mDB0 = 2;
    }
}

// NONMATCHING: 99% - code matches; 10.0f sits later in the float pool (earlier users unwritten)
float GameUI::fn_803E4A48() {
    return mDB4->m100 / 10.0f;
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
        mDC0->m100 = 0.0f;
    }
}

void GameUI::fn_803E58E4() {
    if (mDC4 != 0) {
        mDC4->m100 = 0.0f;
        fn_800C31F4(lbl_80750618[12], 0, -1, 1.0f);
    }
}

bool GameUI::fn_803E5940() {
    if (mDC4 != 0) {
        return (mDC4->mFlags & 4) != 4 && !mDC4->IsEnd_unk();
    }
    return false;
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
    m262C.m00->m100 = 0.0f;
    m262C.m00->m104 = 1.0f;
    m262C.m04->m100 = 0.0f;
    m262C.m04->m104 = 1.0f;
    m262C.m08->m100 = 0.0f;
    m262C.m0C = 1;
    if (lbl_8087F430 != 0) {
        fn_80370BD0(lbl_8087F430, 30, 0, 1e-5f);
    }
}

// NONMATCHING: 99.3% - f0/f1 of -1.0f and the second mA0 swapped
void GameUI::fn_803E65F8() {
    if (m262C.m00 == 0 || m262C.m04 == 0 || m262C.m08 == 0) {
        return;
    }
    if (m262C.m0C == 3 || m262C.m0C == 0) {
        return;
    }
    m262C.m00->m100 = m262C.m00->mA0;
    m262C.m00->m104 = -1.0f;
    m262C.m04->m100 = m262C.m04->mA0;
    m262C.m04->m104 = -1.0f;
    m262C.m0C = 3;
    if (lbl_8087F430 != 0) {
        fn_80370B78(lbl_8087F430, 0xFF000000, 30, 0, 1e-4f);
    }
}

void GameUI::fn_803E6850(CharaTask* chara, int count, int max, int arg) {
    if (!m25CC.Full_unk()) {
        new (&m25CC.m[m25CC.m00]) GameUIElem10_unk(chara, count, max, arg);
        m25CC.m00++;
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
