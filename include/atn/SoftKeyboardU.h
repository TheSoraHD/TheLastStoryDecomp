#ifndef ATN_SOFTKEYBOARDU_H
#define ATN_SOFTKEYBOARDU_H

// Kept out of atn/SoftKeyboardBase.h: its inline members instantiate boost::function members and
// change the order of the weak copies of atn/SoftKeyboardBase_wchar_t_0.cpp.
#include "atn/SoftKeyboardBase.h"

namespace atn {

// atn::SoftKeyboardU (RTTI/vtable 0x80792D70, src/atn/SoftKeyboardU.cpp): the keyboard of the
// multiplayer name entry (MP::SoftKeyboard). Only inline members (weak copies in that file).
class SoftKeyboardU : public SoftKeyboardBase<wchar_t, 0> {
public:
    typedef boost::function<void(int, int, Key*, int, int, Key*)> MoveFunc_unk;
    typedef boost::function<void(int, int, Key*)> KeyFunc_unk;
    typedef boost::function<void()> Func_unk;
    typedef boost::function<void(int, int, Key*, bool)> DrawFunc_unk;

    // inferred: sets a key of the grid (0x80503AB8)
    void fn_80503AB8(int row, int col, const Key& key) {
        mKeys[row][col] = key;
        mKeys[row][col].m00 = 1;
    }
    void fn_80503B04(unsigned int len) { m2420 = len; } // inferred: maximum length
    // inferred: callback setters returning the previous callback (0x80503B0C-0x80504814)
    MoveFunc_unk fn_80503B0C(const MoveFunc_unk& f) {
        MoveFunc_unk old(m2428);
        m2428 = f;
        return old;
    }
    KeyFunc_unk fn_80503D38(const KeyFunc_unk& f) {
        KeyFunc_unk old(m243C);
        m243C = f;
        return old;
    }
    KeyFunc_unk fn_80503F64(const KeyFunc_unk& f) {
        KeyFunc_unk old(m2450);
        m2450 = f;
        return old;
    }
    Func_unk fn_80504190(const Func_unk& f) {
        Func_unk old(m2464);
        m2464 = f;
        return old;
    }
    Func_unk fn_805043BC(const Func_unk& f) {
        Func_unk old(m2478);
        m2478 = f;
        return old;
    }
    Func_unk fn_805045E8(const Func_unk& f) {
        Func_unk old(m248C);
        m248C = f;
        return old;
    }
    DrawFunc_unk fn_80504814(const DrawFunc_unk& f) {
        DrawFunc_unk old(m24A0);
        m24A0 = f;
        return old;
    }
};

} // namespace atn

#endif
