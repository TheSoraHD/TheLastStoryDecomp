#include "game/GameSoundControl.h"
#include "game/GameBgm.h"
#include "atn/Memory.h"
#include "std/new.h"
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
// inferred: map id packed in a word (map, area, sub; names invented)
struct MapId_unk {
    unsigned short mMap; // 0x0
    unsigned char mArea; // 0x2
    unsigned char mSub;  // 0x3
};
struct Unk8087F540_unk {
    unsigned char m00[0x70];
    struct {
        unsigned char m000[0x58];
        MapId_unk m58;
        unsigned char m05C[0x194 - 0x5C];
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

// inferred: empty (called by GameTask)
extern "C" void fn_803CE070() {}

// inferred: saved start positions of the BGMs (pairs id/position at m68, count m64); names invented
struct BgmPos_unk {
    unsigned int mId;
    int mPos;
};
inline BgmPos_unk* BgmPosBegin_unk(GameSoundControl* c) {
    return (BgmPos_unk*)c->m68;
}
inline bool BgmPosFull_unk(GameSoundControl* c) {
    return (unsigned int)c->m64 >= 4;
}
inline BgmPos_unk* BgmPosEnd_unk(GameSoundControl* c) {
    return (BgmPos_unk*)c->m68 + c->m64;
}

// inferred: saved position of the BGM `id` (-1 if none)
extern "C" int fn_803CE074(GameSoundControl* control, unsigned int id) {
    for (BgmPos_unk* p = BgmPosBegin_unk(control); p != BgmPosEnd_unk(control); p++) {
        if (p->mId == id) {
            return p->mPos;
        }
    }
    return -1;
}

// inferred: saves the position of the BGM `id`
struct BgmPosPair_unk {
    BgmPosPair_unk(unsigned int id, int pos) : mId(id), mPos(pos) {}
    unsigned int mId;
    int mPos;
};
// inferred: element as stored by the inlined push_back (copied through a local, placement new)
struct BgmPosPair2_unk {
    unsigned int mId;
    int mPos;
};
inline void BgmPosPush_unk(GameSoundControl* c, const BgmPosPair_unk& e) {
    BgmPosPair2_unk t;
    t.mId = e.mId;
    t.mPos = e.mPos;
    new (BgmPosEnd_unk(c)) BgmPosPair2_unk(t);
    c->m64++;
}
// NONMATCHING: the original reloads the count for the end of the list after the `>= 4` test (here the
// first load is reused); a volatile read of m64 matches, not used.
extern "C" int fn_803CE0B4(GameSoundControl* control, unsigned int id, int pos) {
    if (BgmPosFull_unk(control)) {
        return 0;
    }
    if (pos >= 0) {
        BgmPos_unk* p;
        for (p = BgmPosBegin_unk(control); p != BgmPosEnd_unk(control); p++) {
            if (p->mId == id) {
                p->mPos = pos;
                return 1;
            }
        }
        BgmPosPush_unk(control, BgmPosPair_unk(id, pos));
        return 1;
    }
    return 0;
}

// inferred: two constants (sizes of the sound heaps?)
extern "C" int fn_803CE148() {
    return 0x2D5000;
}
extern "C" int fn_803CE154() {
    return 0x2D5000;
}

// inferred: save header (see include/game/GameSaveBackground.h); defined in this file
Unk_803CE3B8::Unk_803CE3B8() : m00(0), m04(0), m08(0), m0A(0) {
    memset(m0C, 0, sizeof(m0C));
}

// inferred: fills the header from the field and the game settings
void Unk_803CE3B8::fn_803CE408() {
    m00 = *(int*)((char*)lbl_8087F430 + 0x10D0);
    if (lbl_8087F0A8 == 0) {
        return;
    }
    m08 = *(int*)(lbl_8087F0A8 + 0x113C);
    if (lbl_8087F0A8[0x1140] != 0) {
        m0A |= 1;
    }
}

void Unk_803CE3B8::fn_803CE448() {}

// inferred: the header is not valid (field id, value of m04 or m08 out of range)
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

// inferred: accessors of the packed map id (taken by value)
static inline int MapArea_unk(MapId_unk id) {
    return id.mArea;
}
static inline int MapMap_unk(MapId_unk id) {
    return id.mMap;
}
static inline int MapSub_unk(MapId_unk id) {
    return id.mSub;
}

// inferred: the map is a battle arena (area 1, or area 2 map 16 / map 68 sub 1)
extern "C" int fn_803CE160(int area, int map, int sub) {
    int result = 0;
    if (area <= 0 || map <= 0) {
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
            area = fn_80373148(lbl_8087F430)->m48;
            map = fn_80373148(lbl_8087F430)->m4C;
            sub = fn_80373148(lbl_8087F430)->m50;
        } else if (lbl_8087F540 != 0) {
            if (((Unk8087F540_unk*)lbl_8087F540)->m70 != 0) {
                MapId_unk id = ((Unk8087F540_unk*)lbl_8087F540)->m70->m58;
                area = MapArea_unk(id);
                map = MapMap_unk(id);
                sub = MapSub_unk(id);
            } else {
                return 0;
            }
        }

    }
    if (area == 1) {
        result = 1;
    }
    if (area == 2) {
        if (map == 16) {
            result = 1;
        } else if (map == 68 && sub == 1) {
            result = 1;
        }
    }
    return result;
}

// inferred: the BGM of the map can be changed by the battle (0 on the maps of area 2 that keep their BGM)
extern "C" int fn_803CE278(int area, int map, int sub) {
    int result = 1;
    if (area <= 0 || map <= 0) {
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
            area = fn_80373148(lbl_8087F430)->m48;
            map = fn_80373148(lbl_8087F430)->m4C;
            sub = fn_80373148(lbl_8087F430)->m50;
        } else if (lbl_8087F540 != 0) {
            if (((Unk8087F540_unk*)lbl_8087F540)->m70 != 0) {
                MapId_unk id = ((Unk8087F540_unk*)lbl_8087F540)->m70->m58;
                area = MapArea_unk(id);
                map = MapMap_unk(id);
                sub = MapSub_unk(id);
            } else {
                return 1;
            }
        }
    }
    if (area == 1) {
        result = 0;
    }
    if (area == 2) {
        switch (map) {
        case 5:
        case 6:
        case 7:
        case 13:
        case 24:
        case 35:
            break;
        case 40:
            if ((unsigned int)(sub - 9) > 3) {
                result = 0;
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
                result = 0;
            }
            break;
        default:
            result = 0;
            break;
        }
    }
    return result;
}
