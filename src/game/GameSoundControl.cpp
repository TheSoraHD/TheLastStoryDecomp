#include "game/GameSoundControl.h"
#include "game/GameBgm.h"
#include "atn/Memory.h"
#include "atn/FixedArray.h"
#include "game/GameSaveBackground.h"

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

// NONMATCHING: 93.0% - the second test of STREAM_ENDING is compiled as a branch (tried: bool local/inline, == false,
// != 1, bitfields, ternary); the original computes the unsigned
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

// NONMATCHING: 99.2% - the battle state: base/current/previous registers differ (r3/r0 in the original)
void GameSoundControl::fn_803CDF84() {
    if (lbl_8087EE90 == 0 || !HasSoundCfg_unk()) {
        return;
    }
    if (lbl_8087F430 == 0 || GameBgm::sInstance == 0 || !Has8087F418_unk()) {
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

// ---------------------------------------------------------------------------------------------
// inferred: helpers placed after GameSoundControl in the link order (names invented)

extern "C" void fn_803CE070() {}

// inferred: {id, position} pairs kept in GameSoundControl::m64/m68 (at most 4)
struct SoundPos_unk {
    unsigned int mId;
    int mPos;
};
struct SoundPosTable_unk {
    unsigned char m00[0x64];
    atn::FixedArray_unk<SoundPos_unk, 4> mArray;
};

// inferred: saved position of the stream `id` (-1 when missing)
extern "C" int fn_803CE074(SoundPosTable_unk* t, unsigned int id) {
    for (SoundPos_unk* p = t->mArray.mData; p != t->mArray.mData + t->mArray.mSize; p++) {
        if (p->mId == id) {
            return p->mPos;
        }
    }
    return -1;
}

// inferred: stores the position of the stream `id`
extern "C" int fn_803CE0B4(SoundPosTable_unk* t, unsigned int id, int pos) {
    if (t->mArray.size() >= 4) {
        return 0;
    }
    if (pos >= 0) {
        for (SoundPos_unk* p = t->mArray.mData; p != t->mArray.mData + t->mArray.mSize; p++) {
            if (p->mId == id) {
                p->mPos = pos;
                return 1;
            }
        }
        SoundPos_unk e;
        e.mId = id;
        e.mPos = pos;
        t->mArray.push_back(e);
        return 1;
    }
    return 0;
}

extern "C" int fn_803CE148() {
    return 0x2D5000;
}

extern "C" int fn_803CE154() {
    return 0x2D5000;
}

// inferred: area of the multiplayer room (lbl_8087F540->m70->m58, packed bytes)
struct MpRoomId_unk {
    unsigned short mRoom; // inferred
    unsigned char mArea;
    unsigned char mSub;
};
struct MpRoom_unk {
    unsigned char m00[0x58];
    MpRoomId_unk m58;
};
struct MpManager_unk {
    unsigned char m00[0x70];
    MpRoom_unk* m70;
};

// inferred: the map is a battle map (area 1, or area 2 rooms 0x10 / 0x44-1)
extern "C" int fn_803CE160(int area, int room, int sub) {
    int ret = 0;
    if (area <= 0 || room <= 0) {
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
            area = fn_80373148(lbl_8087F430)->m48;
            room = fn_80373148(lbl_8087F430)->m4C;
            sub = fn_80373148(lbl_8087F430)->m50;
        } else if (lbl_8087F540 != 0) {
            MpRoom_unk* r = ((MpManager_unk*)lbl_8087F540)->m70;
            if (r != 0) {
                MpRoomId_unk id1 = r->m58;
                MpRoomId_unk id2 = r->m58;
                MpRoomId_unk id3 = r->m58;
                area = id1.mArea;
                room = id2.mRoom;
                sub = id3.mSub;
            } else {
                return 0;
            }
        }
    }
    if (area == 1) {
        ret = 1;
    }
    if (area == 2) {
        if (room == 0x10) {
            ret = 1;
        } else if (room == 0x44 && sub == 1) {
            ret = 1;
        }
    }
    return ret;
}

// inferred: the map allows the field BGM
extern "C" int fn_803CE278(int area, int room, int sub) {
    int ret = 1;
    if (area <= 0 || room <= 0) {
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
            area = fn_80373148(lbl_8087F430)->m48;
            room = fn_80373148(lbl_8087F430)->m4C;
            sub = fn_80373148(lbl_8087F430)->m50;
        } else if (lbl_8087F540 != 0) {
            MpRoom_unk* r = ((MpManager_unk*)lbl_8087F540)->m70;
            if (r != 0) {
                MpRoomId_unk id1 = r->m58;
                MpRoomId_unk id2 = r->m58;
                MpRoomId_unk id3 = r->m58;
                area = id1.mArea;
                room = id2.mRoom;
                sub = id3.mSub;
            } else {
                return 1;
            }
        }
    }
    if (area == 1) {
        ret = 0;
    }
    if (area == 2) {
        switch (room) {
        case 5:
        case 6:
        case 7:
        case 13:
        case 24:
        case 35:
            break;
        case 40:
            if ((unsigned int)(sub - 9) > 3) {
                ret = 0;
            }
            break;
        case 14:
        case 29:
        case 30:
        case 31:
        case 33:
        case 34:
        case 51:
        case 60:
        case 61:
        case 66:
            if (sub != 9) {
                ret = 0;
            }
            break;
        default:
            ret = 0;
            break;
        }
    }
    return ret;
}

// inferred: save header (game/GameSaveBackground.h)
extern "C" void* memset(void* dst, int c, unsigned long n);
struct SaveField_unk {
    unsigned char m0000[0x10D0];
    int m10D0;
};
struct SaveSystem_unk {
    unsigned char m0000[0x113C];
    int m113C;
    unsigned char m1140;
};

Unk_803CE3B8::Unk_803CE3B8() {
    m00 = 0;
    m04 = 0;
    m08 = 0;
    m0A = 0;
    memset(m0C, 0, sizeof(m0C));
}

void Unk_803CE3B8::fn_803CE408() {
    m00 = ((SaveField_unk*)lbl_8087F430)->m10D0;
    SaveSystem_unk* sys = (SaveSystem_unk*)lbl_8087F0A8;
    if (sys == 0) {
        return;
    }
    m08 = ((SaveSystem_unk*)lbl_8087F0A8)->m113C;
    if (((SaveSystem_unk*)lbl_8087F0A8)->m1140 != 0) {
        m0A |= 1;
    }
}

void Unk_803CE3B8::fn_803CE448() {}

// inferred: the header is out of range (name invented)
extern "C" int fn_803CE44C(const Unk_803CE3B8* h) {
    if (h->m00 < 0 || 999999 < h->m00) {
        return 1;
    }
    if (h->m04 < 0 || 999 < h->m04) {
        return 1;
    }
    if (h->m08 < 0 || 17 <= h->m08) {
        return 1;
    }
    return 0;
}
