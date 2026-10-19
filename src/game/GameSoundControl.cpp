#include "game/GameSoundControl.h"
#include "game/GameBgm.h"
#include "atn/Memory.h"

// inferred: stream of the field (two at Field_unk::m5620/m5624); names invented
struct FieldStream_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    unsigned char m5C[0x6C - 0x5C];
    unsigned int m6C;
    unsigned int m70;
    float m74;

};
// inferred: map information returned by fn_80373148 (as PassGroup_unk in gmk/PassBoard.cpp)
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
    int m50;
};
// inferred: the stream is near its end (written as a macro: the original evaluates it in the branch
// conditions); name invented
#define STREAM_ENDING(s)                                                                           \
    (((s)->m6C >> 24 > (s)->m70 >> 24 && (s)->m4C - (s)->m58 <= 0) ||                              \
     (!((s)->m6C >> 24 > (s)->m70 >> 24) && (s)->m4C - (s)->m54 - (s)->m58 >= 0))

// inferred: field (lbl_8087F430, as in game/CharaTask.cpp); names invented
struct Field_unk {
    unsigned char m0000[0x38];
    unsigned int mFlags;
    unsigned char m003C[0x54E4 - 0x3C];
    int m54E4;
    unsigned char m54E8[0x5538 - 0x54E8];
    int m5538;
    unsigned char m553C[0x5620 - 0x553C];
    FieldStream_unk* m5620;
    FieldStream_unk* m5624;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_803761FC(Field_unk* field);
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);
extern "C" int fn_80375184(Field_unk* field);
// inferred: battle state kept by the effect manager (lbl_8087F048); names invented
struct BattleState_unk {
    unsigned char m00000[0x38C90];
    int m38C90;
    int m38C94;
    unsigned char m38C98[0x38C9C - 0x38C98];
    int m38C9C;
    unsigned char m38CA0;
    unsigned char m38CA1[0x38CCC - 0x38CA1];
    int m38CCC;
    int m38CD0;
};
extern "C" BattleState_unk* lbl_8087F048;
extern "C" void* lbl_8087EE90;
extern "C" void* lbl_8087EFE8; // inferred: sound configuration
extern "C" void fn_800D03AC(void* cfg, int group, int arg, float volume);
extern "C" void* lbl_8087F418;
extern "C" void* lbl_8087F540;
struct Unk8087F540_unk {
    unsigned char m00[0x70];
    struct {
        unsigned char m000[0x194];
        int m194;
    }* m70;
};
extern "C" unsigned char* lbl_8087F0A8;
extern "C" int fn_80124B60(void* arg);

// inferred: global checks (names invented)
inline bool HasSoundCfg_unk() {
    if (lbl_8087EFE8 == 0) {
        return false;
    }
    return true;
}
inline bool Has8087F418_unk() {
    if (lbl_8087F418 == 0) {
        return false;
    }
    return true;
}

GameSoundControl* GameSoundControl::fn_803CD9A8(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (10, "", "", 0) GameSoundControl(parent);
    }
    return sInstance;
}

GameSoundControl::~GameSoundControl() {
    sInstance = 0;
}

void GameSoundControl::fn_803CDAE4() {
    if (lbl_8087F430 != 0 && fn_803761FC(lbl_8087F430) && GameBgm::sInstance != 0) {
        int state = GameBgm::sInstance->m84 != 0 ? GameBgm::sInstance->mNext.mId : GameBgm::sInstance->mReq.mId;
        if (state <= 0 || state == 0x67) {
            GameBgm::sInstance->fn_8037D4C0(0x65, 0, 1, 0, 1.0f);
        }
    }
}

void GameSoundControl::fn_803CDB64() {}

void GameSoundControl::fn_803CDB68() {
    if (lbl_8087F430 != 0 && fn_803761FC(lbl_8087F430) && GameBgm::sInstance != 0) {
        BattleState_unk* state = lbl_8087F048;
        if (state->m38CA0 == 0) {
            if (state->m38C9C >= 3) {
                if (state->m38C90 * 2 <= state->m38C94) {
                    GameBgm::sInstance->fn_8037D4C0(0x6B, 0, 1, 0, 1.0f);
                } else {
                    GameBgm::sInstance->fn_8037D4C0(0x66, 0, 1, 0, 1.0f);
                }
            } else {
                GameBgm::sInstance->fn_8037D4C0(0x65, 0, 1, 0, 1.0f);
            }
        } else {
            GameBgm::sInstance->fn_8037D4C0(0xC9, 0, 1, 0, 1.0f);
        }
    }
}

void GameSoundControl::fn_803CDC48() {
    if (lbl_8087F430 != 0 && fn_803761FC(lbl_8087F430) && GameBgm::sInstance != 0) {
        GameBgm::sInstance->fn_8037D4C0(0x67, 0, 1, 0, 1.0f);
    }
}

// NONMATCHING: 93.0% - the second test of STREAM_ENDING is compiled as a branch; the original computes the unsigned
// comparison as a value (xor/cntlzw/slw idiom) and uses other registers
void GameSoundControl::vf10() {
    fn_803CDF84();
    int boss = 0;
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
        MapInfo_unk* info = fn_80373148(lbl_8087F430);
        if (info->m48 == 2 && info->m4C == 0x36 && info->m50 == 1) {
            boss = 1;
        }
    }
    Field_unk* field = lbl_8087F430;
    int quiet = 0;
    if (field != 0) {
        if (field->m54E4 == 8 && lbl_8087F540 != 0 && ((Unk8087F540_unk*)lbl_8087F540)->m70 != 0 &&
            ((Unk8087F540_unk*)lbl_8087F540)->m70->m194 != 10) {
            FieldStream_unk* s = field->m5620;
            if (s->m48 != 0 && STREAM_ENDING(s)) {
                quiet = 1;
            }
            s = field->m5624;
            if (s->m48 != 0 && STREAM_ENDING(s)) {
                quiet = 1;
            }
        }
        if ((field->mFlags & 2) != 2 || field->m5538 != 0) {
            quiet = 1;
        }
        if (fn_80124B60(lbl_8087F0A8 + 0x48C)) {
            FieldStream_unk* s = lbl_8087F430->m5620;
            if (s->m48 != 0 && (s->m74 < -1.0f || boss) && STREAM_ENDING(s)) {
                quiet = 1;
            }
        }
    }
    if (lbl_8087EFE8 != 0) {
        if (quiet) {
            for (int i = 0; i < 9; i++) {
                switch (i) {
                case 1:
                case 2:
                case 4:
                case 5:
                case 7:
                    break;
                default:
                    fn_800D03AC(lbl_8087EFE8, i, 5, 0.0f);
                    break;
                }
            }
        } else {
            for (int i = 0; i < 9; i++) {
                switch (i) {
                case 1:
                case 2:
                case 4:
                case 5:
                case 7:
                    break;
                default:
                    fn_800D03AC(lbl_8087EFE8, i, 5, 1.0f);
                    break;
                }
            }
        }
    }
}

// NONMATCHING: 93.6% - two of the early returns are `bne; b` in the original
void GameSoundControl::fn_803CDF84() {
    if (lbl_8087EE90 == 0) {
        return;
    }
    if (!HasSoundCfg_unk()) {
        return;
    }
    if (lbl_8087F430 == 0 || GameBgm::sInstance == 0) {
        return;
    }
    if (!Has8087F418_unk()) {
        return;
    }
    if (fn_80375184(lbl_8087F430) && fn_803761FC(lbl_8087F430) && lbl_8087F048 != 0) {
        BattleState_unk* state = lbl_8087F048;
        if (state->m38CCC != state->m38CD0) {
            switch (state->m38CCC) {
            case 1:
                GameBgm::sInstance->fn_8037D4C0(0x6C, 0, 1, 0, 1.0f);
                break;
            case 3:
                GameBgm::sInstance->fn_8037D4C0(0x6B, 0, 1, 0, 1.0f);
                break;
            }
        }
    }
}
