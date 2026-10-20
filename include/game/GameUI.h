#ifndef GAME_GAMEUI_H
#define GAME_GAMEUI_H

#include "atn/Task.h"
#include "atn/Texture_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/CharaStatus.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

// inferred: list {data, count} freed with delete (GameUI 0x30C); names invented. The three levels
// give the three null checks of the inlined destructor (0x803CF79C).
struct GameUIListBase_unk {
    ~GameUIListBase_unk() {
        if (mData != 0) {
            Clear_unk();
            delete mData;
        }
    }
    void Clear_unk(); // 0x803CF78C
    unsigned char* begin_unk() { return mData; }
    unsigned char* end_unk() { return mData + mCount; }

    unsigned char* mData;
    unsigned int mCount;
};
struct GameUIListMid_unk {
    GameUIListBase_unk mBase;
};
struct GameUIList_unk {
    GameUIListMid_unk mMid;
    unsigned char m08[0x18 - 0x08];
};

// inferred: buffer owned when the top bit of its flags is set (GameUI 0xD98); names invented
struct GameUIBuffer_unk {
    ~GameUIBuffer_unk() {
        if (mOwn) {
            delete mData;
        }
    }
    unsigned int mOwn : 1;
    unsigned int mFlags_unk : 31;
    int m04;
    unsigned char* mData;
};
struct GameUIBufferOwner_unk {
    unsigned char m00[0x8];
    GameUIBuffer_unk m08;
};

// inferred: object with a duration at 0xA0 (GameUI 0x3EC); names invented
struct GameUIDuration_unk {
    float fn_803CFC6C(); // inferred: returns m0A0
    unsigned char m00[0xA0];
    float mA0;
};

// inferred: gauge task with a value at 0x100 (GameUI 0x83C, 0x840); names invented
struct GameUIGauge_unk : public atn::Task {
    bool fn_803CFC58(); // inferred: (mFlags & 4) != 0
    float fn_803CFC64(); // inferred: returns m100
    unsigned char m48[0x100 - 0x48];
    float m100;
};

// inferred: timed entry (0xC bytes) of the list at 0x3F0; names invented
struct GameUIEntry_unk {
    int m00;
    int mFrames;
    int m08;
};

// inferred: counter reset by fn_803CF734 (name invented)
struct GameUICounter_unk {
    void fn_803CF734();
    int mCount;
};

// inferred: state reset by GameUI::fn_803CF74C (0xD40); names invented
struct GameUIMarker_unk {
    int m00;
    atn::Vector3 mPos;
    int m10;
    int m14;
    int m18;
    float m1C;
};

// inferred from __vt__6GameUI (0x8078C584): the game HUD; lbl_8087F490 points to it
class GameUI : public atn::Task {
public:
    GameUI(atn::Task* parent); // 0x803CE980
    virtual ~GameUI();         // 0x803CF79C
    virtual int vf0C();        // 0x803CF8F0
    virtual void vf10();       // 0x803CF938
    virtual void vf14();       // 0x803CFC74

    void fn_803CF740();
    void fn_803CF74C();

    unsigned char m48[0x30C - 0x48];
    GameUIList_unk m30C;                       // 0x30C
    unsigned char m324[0x3EC - 0x324];
    GameUIDuration_unk* m3EC;                  // 0x3EC
    FixedArray_unk<GameUIEntry_unk, 71> m3F0;  // 0x3F0
    unsigned char m748[0x83C - 0x748];
    GameUIGauge_unk* m83C;                     // 0x83C
    GameUIGauge_unk* m840;                     // 0x840
    unsigned char m844[0xC38 - 0x844];
    int mC38;                                  // 0xC38
    unsigned char mC3C[0xD3C - 0xC3C];
    int mD3C;                                  // 0xD3C
    GameUIMarker_unk mD40;                     // 0xD40
    unsigned char mD60[0xD90 - 0xD60];
    GameUIBufferOwner_unk mD90;                // 0xD90
    unsigned char mDA4[0xDD4 - 0xDA4];
    atn::Texture_unk mDD4;                     // 0xDD4
    atn::Texture_unk mE04;                     // 0xE04
    atn::Texture_unk mE34;                     // 0xE34
    atn::Texture_unk mE64;                     // 0xE64
    unsigned char mE94[0x1158 - 0xE94];
    ResLoader_unk m1158;                       // 0x1158
    ResLoader_unk m1164;                       // 0x1164
    unsigned char m1170[0x1554 - 0x1170];
    ResLoader_unk m1554[4];                    // 0x1554
    ResLoader_unk m1584[4];                    // 0x1584
    unsigned char m15B4[0x265C - 0x15B4];
    int mSeState;                              // 0x265C: inferred: state of the gauge sound
    unsigned int mSeMask;                      // 0x2660: inferred: sounds to play (1 << index)
    ResHandle m2664;                           // 0x2664
};

#endif
