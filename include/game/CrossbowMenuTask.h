#ifndef GAME_CROSSBOWMENUTASK_H
#define GAME_CROSSBOWMENUTASK_H

#include "atn/Task.h"
#include "atn/FixedArray.h"

namespace atn {
class D2A_Task;
}

// inferred: color of an arrow kind (table of 7 entries at 0x80782900); names invented
struct CrossbowColor_unk {
    CrossbowColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
struct CrossbowIcon_unk {
    int mIndex; // inferred: icon index (negative hides the icon)
    CrossbowColor_unk mColor;
};

// inferred: item definition returned by fn_80211480 (db/ItemParam_unk.h)
struct ItemParamEntry_unk;
typedef ItemParamEntry_unk CrossbowItem_unk;

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

    atn::D2A_Task* m48; // inferred: d2anime task (fn_801F3FF8)
    atn::FixedArray_unk<int, 64> m4C; // inferred: arrow ids
    int m150; // inferred: selected index
    int m154; // inferred: rebuild request
    int m158; // inferred: current arrow id
    int m15C; // inferred: animation state
    int m160; // inferred: only one kind of arrow
    int m164;
};

#endif
