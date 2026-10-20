#include "game/GameUI.h"
#include "atn/SoundRef.h"

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
extern "C" int fn_803CC900(atn::Task* table); // inferred: number of entries
extern "C" void fn_803CFC78(GameUI* ui);   // inferred: creates the parts of the HUD
extern "C" void fn_803E668C(GameUI* ui);

// inferred: bit `i` of `mask` is set (name invented)
inline bool __IsBit_unk(unsigned int mask, int i) {
    return ((1 << i) & mask) == (1 << i);
}

// NOT DECOMPILED: GameUI::GameUI (0x803CE980, 0xDB4 bytes) builds a class of more than 0x28B0 bytes
// with dozens of inline member constructors (arrays of 0x7C/0x3E0/0x20-byte entries, colour tables);
// it is left for later. Its float constants are the first ones of .sdata2 (0x80885D58-0x80885DD0).

void GameUICounter_unk::fn_803CF734() {
    mCount = 0;
}

void GameUI::fn_803CF740() {
    mC38 = 0;
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

// NONMATCHING: 99.78% - only the literals differ: the .sdata2 offsets depend on the constants of the
// constructor (not decompiled) and the int-to-float double is in the auto .rodata (0x80750650).
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
