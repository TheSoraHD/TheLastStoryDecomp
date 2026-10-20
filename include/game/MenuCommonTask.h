#ifndef GAME_MENUCOMMONTASK_H
#define GAME_MENUCOMMONTASK_H

#include "atn/Task.h"
#include "game/ResHandle.h"
#include "string.h"

// inferred: fixed text buffer cleared by its constructor (name invented)
struct MenuText_unk {
    MenuText_unk() { memset(mText, 0, sizeof(mText)); }
    char mText[0x40];
};

// inferred: 0x48-byte entry of the array at 0xA0 (class unknown)
struct MenuCommonEntry_unk {
    MenuCommonEntry_unk() : m00(0), m04(0) {}
    int m00;
    int m04;
    MenuText_unk m08;
};

// inferred: d2anime task (made by fn_801F3FF8); only the members used here
struct MenuD2A_unk : public atn::Task {
    // inferred: name invented
    bool IsEnd_unk() {
        float last = mLastFrame;
        return mFrame >= last;
    }
    unsigned char m48[0x58 - 0x48];
    unsigned char m58[0xA0 - 0x58]; // 0x58: inferred: layout
    float mLastFrame; // 0xA0
    unsigned char mA4[0x100 - 0xA4];
    float mFrame;   // 0x100: inferred: current frame
    float mSpeed;   // 0x104: inferred: playback speed
};

// inferred: cursor task (made by fn_80202A6C); only the members used here
struct MenuCursor_unk : public atn::Task {
    unsigned char m48[0x104 - 0x48];
    unsigned int m104_hi : 8;
    unsigned int mActive : 1; // inferred: 0x800000 of the flags at 0x104
    unsigned int m104_lo : 23;
};

// inferred from __vt__14MenuCommonTask (0x807907DC): shared parts of the camp menus (info windows, cursors)
class MenuCommonTask : public atn::Task {
public:
    static MenuCommonTask* fn_804A29C4(atn::Task* parent); // inferred: creates the instance
    MenuCommonTask(atn::Task* parent); // 0x804A2A34
    virtual ~MenuCommonTask();         // 0x804A2C98
    virtual int vf0C();                // 0x804A2D2C
    virtual void vf10();               // 0x804A3228
    virtual void vf14();               // 0x804A3438
    void fn_804A2F98(); // inferred: hides everything
    void fn_804A2FBC(); // inferred: resets everything
    void fn_804A313C(); // inferred: resets the info windows
    void ResetInfo_unk(); // inferred: inline body of fn_804A313C (name invented)

    static MenuCommonTask* sInstance;

    int m48;
    int m4C;
    MenuD2A_unk* m50; // inferred: camp_info.d2a
    int m54;
    MenuD2A_unk* m58; // inferred: camp_info6.d2a
    int m5C;
    MenuD2A_unk* m60; // inferred: camp_info.d2a
    int m64;
    int m68;
    MenuD2A_unk* m6C; // inferred: camp_info2.d2a
    MenuD2A_unk* m70; // inferred: camp_cursor.d2a
    int m74;
    MenuCursor_unk* m78[4]; // inferred: cursors
    int m88;
    atn::Task* m8C[4]; // inferred: cursors
    int m9C;
    MenuCommonEntry_unk mA0[4];
    int m1C0;
    int m1C4;
    MenuD2A_unk* m1C8; // inferred: camp_info_bar.d2a
    int m1CC;
    int m1D0[2];
    MenuD2A_unk* m1D8[2]; // inferred: ui_abtn_reaction_opcl.d2a
    MenuD2A_unk* m1E0[2]; // inferred: ui_abtn_reaction_loop.d2a
    ResHandle m1E8;     // inferred: debug menu group
    float m1EC;         // inferred: "EquipFace Ofs" (2 floats)
    float m1F0;
    float m1F4;         // inferred: "EquipFace Scl" (2 floats)
    float m1F8;
    float m1FC;         // inferred: "4:3 Desc Ofs"
};

#endif
