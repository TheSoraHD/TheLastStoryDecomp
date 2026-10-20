#ifndef ATN_SOFTKEYBOARDBASE_H
#define ATN_SOFTKEYBOARDBASE_H

// atn::SoftKeyboardBase<wchar_t, 0>: on-screen keyboard grid of 8 x 32 keys with boost::function
// callbacks (src/atn/SoftKeyboardBase_wchar_t_0.cpp). Its subclass is atn::SoftKeyboardU.
#include <boost/function.hpp>
#include "math/Vec.h"

namespace atn {

// inferred: one key of the grid (0x1C bytes)
struct SoftKey_unk {
    SoftKey_unk() : m00(0) {}
    bool Contains_unk(const Vector2_unk& p) const;
    int m00; // inferred: key enabled
    wchar_t m04; // inferred: character
    float m08, m0C, m10, m14;
    int m18;
};

// inferred: zero-terminated text buffer
template <class T, int Size>
struct Text_unk {
    Text_unk() { m[0] = 0; }
    T m[Size];
};

template <class T, int N>
class SoftKeyboardBase {
public:
    typedef SoftKey_unk Key;

    SoftKeyboardBase();
    virtual ~SoftKeyboardBase();
    virtual void vf0C();
    virtual void vf10();

    void fn_800C6E60(T c);
    void fn_800C6ED8();
    bool fn_800C6F28();
    bool fn_800C6FB4();
    bool fn_800C7040();
    bool fn_800C70CC();
    bool fn_800C7158();
    bool fn_800C71D0();
    bool fn_800C7248();
    bool fn_800C72C0();
    bool fn_800C7338();
    void fn_800C73B0();
    void fn_800C755C();
    void fn_800C7568(int row, int col);

    Text_unk<T, 0x400> mText;
    Key mKeys[8][32];
    int m2404; // inferred: buttons enabled
    int m2408; // inferred: pointer enabled
    int m240C; // inferred: hardware keyboard enabled
    int m2410;
    int m2414; // inferred: active
    int m2418; // inferred: cursor row
    int m241C; // inferred: cursor column
    unsigned int m2420; // inferred: max length
    Key* m2424; // inferred: key under the cursor
    boost::function<void(int, int, Key*, int, int, Key*)> m2428; // inferred: cursor moved
    boost::function<void(int, int, Key*)> m243C; // inferred: key decided
    boost::function<void(int, int, Key*)> m2450;
    boost::function<void()> m2464;
    boost::function<void()> m2478; // inferred: backspace
    boost::function<void()> m248C;
    boost::function<void(int, int, Key*, bool)> m24A0; // inferred: draw a key
};

} // namespace atn

#endif
