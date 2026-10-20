#ifndef GAME_CROSSBOWMENUTASK_H
#define GAME_CROSSBOWMENUTASK_H

#include "atn/Task.h"
#include "atn/FixedArray.h"

// inferred: d2anime task (made by fn_801F3FF8); only the members used here
struct CrossbowD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char m58[0xFC - 0x58]; // 0x58: inferred: layout
    unsigned int mFC;               // inferred: flags (0x10000000 is cleared by vf0C)
    float mFrame;                   // 0x100
    float mSpeed;                   // 0x104
};

// inferred: color of an arrow kind (table of 7 entries at 0x80782900); names invented
struct CrossbowColor_unk {
    CrossbowColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
struct CrossbowIcon_unk {
    int mIndex; // inferred: icon index (negative hides the icon)
    CrossbowColor_unk mColor;
};

// inferred: item definition returned by fn_80211480 (class unknown)
struct CrossbowItem_unk {
    int m00;
    int mId;                // 0x04
    const wchar_t* mName;   // 0x08
    unsigned char m0C[0xC4 - 0x0C];
    int mC4;                // inferred: bullet id / icon
    unsigned char mC8[0xF4 - 0xC8];
    struct {
        int mKind;
        unsigned char m04[0x10];
    } mF4[2];
};

// inferred from __vt__16CrossbowMenuTask (0x8078298C): arrow selection menu of the crossbow; singleton
class CrossbowMenuTask : public atn::Task {
public:
    static CrossbowMenuTask* fn_801E9398(atn::Task* parent); // inferred: creates the instance
    CrossbowMenuTask(atn::Task* parent);
    virtual ~CrossbowMenuTask(); // 0x801E9464
    virtual int vf0C();          // 0x801E94D4
    virtual void vf10();         // 0x801E97E8
    virtual void vf14();         // 0x801E996C
    void fn_801E9544(); // inferred: rebuilds the arrow list
    void fn_801E97DC(); // inferred: selects the first entry
    void fn_801E9B84(int index, int slot); // inferred: draws an entry
    void fn_801E9EA4(); // inferred: opens the menu
    void fn_801E9ECC(int dir, int anim); // inferred: moves the selection
    int fn_801EA01C() const; // inferred: bullet of the selected arrow
    void fn_801EA0B4(); // inferred: consumes the selected arrow
    CrossbowIcon_unk fn_801EA11C(CrossbowItem_unk* item); // inferred: icon of an arrow

    static CrossbowMenuTask* sInstance;

    CrossbowD2A_unk* m48;
    atn::FixedArray_unk<int, 64> m4C; // inferred: arrow ids
    int m150; // inferred: selected index
    int m154; // inferred: rebuild request
    int m158; // inferred: current arrow id
    int m15C; // inferred: animation state
    int m160; // inferred: only one kind of arrow
    int m164;
};

#endif
