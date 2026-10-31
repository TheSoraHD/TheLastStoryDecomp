#ifndef GAME_GAMEUI_H
#define GAME_GAMEUI_H

#include "atn/Task.h"
#include "atn/Texture_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/D3AnimeInstance.h"
#include "game/CharaStatus.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "std/string.h"

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
    GameUIList_unk() {
        mMid.mBase.mData = 0;
        mMid.mBase.mCount = 0;
        m08 = 0;
    }
    GameUIListMid_unk mMid;
    unsigned int m08; // inferred: capacity
};

struct GameUIBufferOwner_unk {
    GameUIBufferOwner_unk() : m00(0), m04(0) {}
    int m00;
    int m04;
    std::basic_string<wchar_t> m08; // inferred: message text (fn_803E4978)
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
    unsigned char m48[0xA0 - 0x48];
    float mA0;  // inferred: end value (GameUI::fn_803E5940)
    unsigned char mA4[0x100 - 0xA4];
    float m100; // inferred: current value
    float m104; // inferred: set to -1 by GameUI::fn_803E4510
    void SetEnd_unk() { m100 = mA0; }     // inferred: name invented
    void SetFrame_unk(float f) { m100 = f; } // inferred: name invented
    void SetSpeed_unk(float f) { m104 = f; } // inferred: name invented
    bool IsEnd_unk() const { // inferred: name invented
        float end = mA0;
        return m100 >= end;
    }
};

// inferred: timed entry (0xC bytes) of the list at 0x3F0; names invented
struct GameUIEntry_unk {
    int m00;
    int mFrames;
    unsigned int m08; // inferred: id (compared unsigned by GameUI::fn_803E6C88)
};

// inferred: counter reset by fn_803CF734 (name invented)
struct GameUICounter_unk {
    void fn_803CF734();
    int mCount;
};

// inferred: state reset by GameUI::fn_803CF74C (0xD40); names invented
struct GameUIMarker_unk {
    GameUIMarker_unk() : m00(0), mPos(0.0f, 0.0f, 0.0f), m14(0), m18(0), m1C(0.0f) {}
    int m00;
    atn::Vector3 mPos;
    int m10;
    int m14;
    int m18;
    float m1C;
};

// inferred: 0x60-byte element of the array at 0x50 (names invented)
struct GameUIElem60_unk {
    GameUIElem60_unk()
        : m00(0), m04(0), m0C(0), m10(0), m14(0), m18(0), m1C(0), m20(0), m24(0), m28(0), m30(0), m34(0),
          m38(0), m3C(0.0f), m40(0), m44(0.0f), m48(0), m4C(0), m50(0), m54(0), m58(0), m5C(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    int m2C;
    int m30;
    int m34;
    int m38;
    float m3C;
    int m40;
    float m44;
    int m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    int m5C;
};

// inferred: 0x20-byte element of the array at 0x318 (names invented)
struct GameUIElem20_unk {
    GameUIElem20_unk() : m14(0), m18(0), m1C(-1) {}
    void Reset_unk() {
        mInst[0] = 0;
        mInst[1] = 0;
        mInst[2] = 0;
        mInst[3] = 0;
        mInst[4] = 0;
    }
    atn::D2AInstance_unk* mInst[5]; // inferred: d2anime instances (frame at 0x50, GameUI::fn_803E0D84)
    int m14;
    int m18;
    int m1C;
};

// inferred: 0x1C-byte entry at 0x748 (names invented)
struct GameUIElem1C_unk {
    GameUIElem1C_unk() : m00(-1), m04(-1), m08(-1), m0C(0), m10(0), m14(0), m18(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
};

// inferred: 0x7C-byte element of the arrays at 0x1174 and 0x15BC (names invented)
struct GameUIElem7C_unk {
    GameUIElem7C_unk() : m78(0.0f) {}
    unsigned char m00[0x78];
    float m78;
};

// inferred: 0xC-byte element of the array at 0x256C (names invented)
struct GameUIElem0C_unk {
    GameUIElem0C_unk() : m00(-1), m04(0), m08(0) {}
    int m00;
    int m04;
    int m08;
};

// inferred: 0x10-byte element of the array at 0x25D0 (names invented)
class CharaTask;
struct GameUIElem10_unk {
    GameUIElem10_unk() : m00(0), m04(0), m08(1), m0C(0) {}
    GameUIElem10_unk(CharaTask* chara, int count, int max, int arg) : m00(chara), m04(count), m08(max), m0C(arg) {}
    CharaTask* m00; // inferred: character of the counter (GameUI::fn_803E6850)
    int m04;
    int m08;
    int m0C;
};

// inferred: RGBA colour (names invented)
struct GameUIColor_unk {
    GameUIColor_unk() {}
    GameUIColor_unk(int r_, int g_, int b_, int a_) {
        r = r_ / 255.0f;
        g = g_ / 255.0f;
        b = b_ / 255.0f;
        a = a_ / 255.0f;
    }
    GameUIColor_unk(float r_, float g_, float b_, float a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    void Set_unk(float r_, float g_, float b_, float a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    float r, g, b, a;
};

namespace atn {
class D2A_Task;
}
class GameUI;
class CharaTask;
struct Bullet_unk;
extern "C" GameUI* lbl_8087F490; // inferred: the HUD instance (defined in game/GameUI.cpp)

// inferred from __vt__6GameUI (0x8078C584): the game HUD; lbl_8087F490 points to it
// inferred: message window of GameUI::m2640 (game/GameChoiceWindow.cpp); names invented
struct GameUIWindow_unk {
    unsigned char m00[0xB4];
    int mB4;
};

class GameUI : public atn::Task {
public:
    GameUI(atn::Task* parent); // 0x803CE980
    virtual ~GameUI();         // 0x803CF79C
    virtual int vf0C();        // 0x803CF8F0
    virtual void vf10();       // 0x803CF938
    virtual void vf14();       // 0x803CFC74

    static GameUI* fn_803CE910(atn::Task* parent);
    static GameUI* Get_unk() { return lbl_8087F490; } // inferred: weak copy 0x80151210 (game/CharaTask)
    void Remove_unk(CharaTask* chara) { fn_803E2110(1, chara); } // inferred: weak copy 0x80151204

    void fn_803CF740();
    void fn_803CF74C();
    int fn_803D1574();
    void fn_803D8344();
    GameUIColor_unk fn_803DF138(Bullet_unk* bullet);
    void fn_803DF5B4();
    void fn_803DF648(int arg);
    void fn_803E0D4C();
    void fn_803E0D84();
    void fn_803E2110(int arg, CharaTask* chara); // inferred: HUD notice about a character
    void fn_803E32EC();
    void fn_803E3384();
    void fn_803E3BE8(void* msg, int arg, float time);
    void fn_803E4100();
    void fn_803E41AC(CharaTask* chara);
    bool fn_803E43A4();
    void fn_803E43B8(int on);
    void fn_803E4434(int on); // inferred: names invented for the fields set here
    void fn_803E4478();
    void fn_803E44C4(int on, int id, int count);
    void fn_803E4510();
    void fn_803E4950();
    void fn_803E4978(const wchar_t* text); // inferred: shows a message
    void fn_803E4A20(int a, int b);
    float fn_803E4A48();
    void fn_803E58CC();
    void fn_803E58E4();
    bool fn_803E5940();
    void fn_803E627C(int town); // inferred: loads the shop animations of a town
    void fn_803E63D0();
    bool fn_803E644C();
    void fn_803E64F0();
    void fn_803E656C();
    void fn_803E65F8();
    void fn_803E668C();
    void fn_803E6850(CharaTask* chara, int count, int max, int arg); // inferred: shows a counter
    void fn_803E6ADC(const atn::Vector3& a, const atn::Vector3& b);
    void fn_803E6BB4(CharaTask* target); // inferred: new target
    void fn_803E6BF0(unsigned int id, int value);
    bool fn_803E6C88(unsigned int id);

    float m48;
    float m4C;
    GameUIElem60_unk m50;                      // 0x50
    GameUIElem60_unk mB0[6];                   // 0xB0
    int m2F0;
    GameUIElem60_unk* m2F4[6]; // inferred: active entries of mB0 (fn_803DF5B4)
    GameUIList_unk m30C;                       // 0x30C
    GameUIElem20_unk m318[6];                  // 0x318
    int m3D8;
    int m3DC;
    int m3E0;
    int m3E4;
    int m3E8;
    GameUIDuration_unk* m3EC;                  // 0x3EC
    FixedArray_unk<GameUIEntry_unk, 32> m3F0;  // 0x3F0
    int m574[8];
    int m594[8];
    int m5B4;
    int m5B8;
    int m5BC[80];
    int m6FC;
    int m700;
    int m704;
    int m708;
    int m70C;
    int m710;
    int m714;
    long long m718;
    int m720;
    int m724;
    int m728;
    int m72C;
    int m730;
    int m734;
    int m738;
    int m73C;
    int m740;
    int m744;
    GameUIElem1C_unk m748;
    GameUIElem1C_unk m764;
    GameUIElem1C_unk m780;
    GameUIElem1C_unk m79C;
    int m7B8;
    int m7BC;
    GameUIElem1C_unk m7C0;
    GameUIElem1C_unk m7DC;
    GameUIElem1C_unk m7F8;
    GameUIGauge_unk* m814[8]; // inferred: pairs of gauges (fn_803E4100)
    int m834;
    int m838;
    GameUIGauge_unk* m83C;                     // 0x83C
    GameUIGauge_unk* m840;                     // 0x840
    int m844;
    int m848[5];
    int m85C;
    unsigned char m860[0x870 - 0x860];
    int m870;
    int m874[5];
    atn::Vector3 m888; // inferred: fn_803E6ADC
    atn::Vector3 m894;
    int m8A0[12];
    int m8D0;
    unsigned char m8D4[0xA84 - 0x8D4];
    atn::D2A_Task* mA84; // inferred: frame and "basePos" set by fn_803E6ADC
    atn::D2A_Task* mA88;
    int mA8C;
    int mA90[10];
    int mAB8[10];
    int mAE0[10];
    int mB08[6];
    int mB20[6];
    struct {
        int m00;
        int m04;
        unsigned char m08[0x20 - 0x08];
    } mB38[8];
    // inferred: count and markers (names invented)
    struct MarkerList_unk {
        MarkerList_unk() : mCount(0) {}
        int mCount;
        GameUIMarker_unk m[8];
    } mC38;                                    // 0xC38
    int mD3C;                                  // 0xD3C
    GameUIMarker_unk mD40;                     // 0xD40
    unsigned char mD60[0xD6C - 0xD60];
    int mD6C;
    atn::Task* mD70; // inferred: frozen by fn_803E4478
    atn::Task* mD74;
    int mD78;
    int mD7C;
    int mD80;
    int mD84;
    int mD88;
    int mD8C;
    GameUIBufferOwner_unk mD90;                // 0xD90
    int mDA4;
    int mDA8;
    int mDAC;
    int mDB0;
    GameUIGauge_unk* mDB4; // inferred: same kind of object as m2654 (fn_803E4950)
    int mDB8;
    int mDBC;
    GameUIGauge_unk* mDC0; // inferred: fn_803E58CC
    GameUIGauge_unk* mDC4; // inferred: fn_803E58E4, fn_803E5940
    int mDC8;
    int mDCC;
    float mDD0;
    atn::Texture_unk mDD4;                     // 0xDD4
    atn::Texture_unk mE04;                     // 0xE04
    atn::Texture_unk mE34;                     // 0xE34
    atn::Texture_unk mE64;                     // 0xE64
    unsigned char mE94[0xF14 - 0xE94];
    int mF14[32];
    int mF94[32];
    int m1014[32];
    unsigned char m1094[0x10B4 - 0x1094];
    int m10B4[8];
    int m10D4[8];
    int m10F4;
    int m10F8;
    int m10FC;
    float m1100;
    atn::D2A_Task* m1104; // inferred: head marker animations (fn_803E41AC)
    atn::D2A_Task* m1108;
    CharaTask* m110C;     // inferred: character with the marker
    atn::D2A_Task* m1110;
    atn::D2A_Task* m1114;
    int m1118;
    int m111C;
    int m1120;
    int m1124;
    int m1128;
    int m112C;
    int m1130;
    unsigned char m1134[0x1140 - 0x1134];
    int m1140;
    int m1144;
    int m1148;
    int m114C;
    int m1150;
    int m1154;
    atn::EffFile m1158;                       // 0x1158
    atn::EffFile m1164;                       // 0x1164
    int m1170;
    GameUIElem7C_unk m1174[8];                 // 0x1174
    atn::EffFile m1554[4];                    // 0x1554
    atn::EffFile m1584[4];                    // 0x1584
    int m15B4;
    int m15B8;
    GameUIElem7C_unk m15BC[32];                // 0x15BC
    int m253C;
    int m2540;
    int m2544;
    int m2548;
    int m254C;
    char m2550[0xC];
    int m255C;
    float m2560;
    int m2564;
    struct W2568_unk {
        W2568_unk() : m00(0) {}
        int m00;
        GameUIElem0C_unk m[8];
    } m2568;                                   // 0x2568
    struct W25CC_unk {
        W25CC_unk() : m00(0) {}
        bool Full_unk() const { return m00 >= 4; } // inferred: name invented
        unsigned int m00; // inferred: count (compared unsigned in fn_803E6850)
        GameUIElem10_unk m[4];
    } m25CC;                                   // 0x25CC
    int m2610[4];
    CharaTask* m2620; // inferred: current target (fn_803E6BB4)
    CharaTask* m2624; // inferred: previous target
    GameUIGauge_unk* m2628; // inferred: m100 reset by fn_803E6BB4
    // inferred: three gauges and their state (fn_803E63D0 to fn_803E65F8)
    struct W262C_unk {
        W262C_unk() : m00(0), m04(0), m08(0), m0C(0) {}
        atn::D2A_Task* m00; // inferred: shop animations (fn_803E627C)
        atn::D2A_Task* m04;
        atn::D2A_Task* m08;
        int m0C;
    } m262C;
    int m263C;
    GameUIWindow_unk* m2640; // inferred: message window (fn_803E3BE8)
    int m2644;
    int m2648;
    int m264C;
    int m2650;
    GameUIGauge_unk* m2654; // inferred: floats 0x100/0x104 reset by fn_803E4510
    int m2658;
    int mSeState;                              // 0x265C: inferred: state of the gauge sound
    unsigned int mSeMask;                      // 0x2660: inferred: sounds to play (1 << index)
    ResHandle m2664;                           // 0x2664
    float m2668;
    float m266C;
    float m2670;
    float m2674;
    GameUIColor_unk m2678;
    GameUIColor_unk m2688;
    GameUIColor_unk m2698;
    GameUIColor_unk m26A8;
    GameUIColor_unk m26B8;
    GameUIColor_unk m26C8;
    GameUIColor_unk m26D8;
    GameUIColor_unk m26E8;
    GameUIColor_unk m26F8;
    GameUIColor_unk m2708;
    GameUIColor_unk m2718;
    GameUIColor_unk m2728;
    GameUIColor_unk m2738;
    GameUIColor_unk m2748;
    GameUIColor_unk m2758;
    GameUIColor_unk m2768;
    GameUIColor_unk m2778;
    int m2788;
    unsigned char m278C[0x100];
    GameUIColor_unk m288C;
    GameUIColor_unk m289C;
};

#endif
