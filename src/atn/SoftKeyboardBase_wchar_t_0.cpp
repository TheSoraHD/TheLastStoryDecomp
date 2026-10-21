// atn/SoftKeyboardBase_wchar_t_0.cpp: atn::SoftKeyboardBase<wchar_t, 0> (on-screen keyboard grid of
// 8 x 32 keys with boost::function callbacks; its only subclass is atn::SoftKeyboardU, MP area).
// File name invented (inferred: the vtable and RTTI of the template are emitted here).
#include "atn/SoftKeyboardBase.h"

extern "C" unsigned long wcslen(const wchar_t* s);

// inferred: file watcher of game/LibraryTask (lbl_8087EEF0); here used as the keyboard state
struct LibraryWatch_unk {
    unsigned char m000[0x34];
    unsigned char m034[0xD90 - 0x34];
    int mD90;
};
extern "C" LibraryWatch_unk* lbl_8087EEF0;
extern "C" int fn_8007708C(LibraryWatch_unk* watch, void* name, int key);

typedef atn::Vector2_unk Pos2_unk;

// inferred: Wii Remote manager (atn/WPad_unk.cpp)
struct WPadMgr_unk {
    unsigned char m0000[0x40A8];
    int m40A8; // inferred: pointer enabled
};
extern "C" WPadMgr_unk* lbl_8087F018;
extern "C" Pos2_unk fn_800DF154(WPadMgr_unk* wpad, int port); // inferred: pointer position

struct Pad_unk;
extern "C" Pad_unk* lbl_8087EF70;
extern "C" int fn_800A555C(Pad_unk* pad, int port, int button); // inferred: triggered
extern "C" int fn_800A5584(Pad_unk* pad, int port, int button);
extern "C" int fn_800A55AC(Pad_unk* pad, int port, int button); // inferred: repeated

// inferred: true if the key `key` of the hardware keyboard is pressed
int fn_800C6128(int key) {
    LibraryWatch_unk* watch = lbl_8087EEF0;
    if (watch != 0) {
        int ret = 0;
        if (watch->mD90 != 0 && fn_8007708C(watch, watch->m034, key) != 0) {
            ret = 1;
        }
        return ret;
    }
    return 0;
}

// inferred: pointer position of the first Wii Remote, (-1, -1) if there is none
Pos2_unk fn_800C6190() {
    WPadMgr_unk* wpad = lbl_8087F018;
    if (wpad != 0 && wpad->m40A8 != 0) {
        return fn_800DF154(wpad, 0);
    }
    return Pos2_unk(-1.0f, -1.0f);
}

// inferred: function removed by the linker. The weak copies of boost::function0<void>::operator
// safe_bool and ~function<void()> are placed between fn_800C6190 and the template members, so a
// function of the file used and destroyed a boost::function<void()> there (reconstructed only for
// that order; its body is invented).
void fn_SoftKeyboard_dead_unk(boost::function<void()>* f) {
    if (*f) {
        delete f;
    }
}

namespace atn {

static inline bool PadRepeat_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A55AC(lbl_8087EF70, 0, button) : 0;
}

static inline bool PadTrigger_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A555C(lbl_8087EF70, 0, button) : 0;
}

static inline bool PadHold_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A5584(lbl_8087EF70, 0, button) : 0;
}

template <class T, int N>
SoftKeyboardBase<T, N>::SoftKeyboardBase()
    : m2404(1), m2408(1), m240C(1), m2410(0), m2414(0), m2418(-1), m241C(-1), m2420(0x400), m2424(0) {}

template <class T, int N>
SoftKeyboardBase<T, N>::~SoftKeyboardBase() {}

template <class T, int N>
void SoftKeyboardBase<T, N>::vf0C() {
    if (m2414 == 0) {
        return;
    }
    if (fn_800C6F28()) {
        int old = m2418;
        T c = mKeys[m2418][m241C].m04;
        for (;;) {
            if (--m2418 < 0) {
                m2418 = 7;
            }
            if (m2418 == old) {
                break;
            }
            if (mKeys[m2418][m241C].m00 != 0 && mKeys[m2418][m241C].m04 != 0 && c != mKeys[m2418][m241C].m04) {
                break;
            }
        }
        if (m2428) {
            m2428(old, m241C, &mKeys[old][m241C], m2418, m241C, &mKeys[m2418][m241C]);
        }
    }
    if (fn_800C6FB4()) {
        int old = m2418;
        T c = mKeys[m2418][m241C].m04;
        for (;;) {
            if (++m2418 >= 8) {
                m2418 = 0;
            }
            if (m2418 == old) {
                break;
            }
            if (mKeys[m2418][m241C].m00 != 0 && mKeys[m2418][m241C].m04 != 0 && c != mKeys[m2418][m241C].m04) {
                break;
            }
        }
        if (m2428) {
            m2428(old, m241C, &mKeys[old][m241C], m2418, m241C, &mKeys[m2418][m241C]);
        }
    }
    if (fn_800C7040()) {
        int old = m241C;
        T c = mKeys[m2418][m241C].m04;
        for (;;) {
            if (--m241C < 0) {
                m241C = 31;
            }
            if (m241C == old) {
                break;
            }
            if (mKeys[m2418][m241C].m00 != 0 && mKeys[m2418][m241C].m04 != 0 && c != mKeys[m2418][m241C].m04) {
                break;
            }
        }
        if (m2428) {
            m2428(m2418, old, &mKeys[m2418][old], m2418, m241C, &mKeys[m2418][m241C]);
        }
    }
    if (fn_800C70CC()) {
        int old = m241C;
        T c = mKeys[m2418][m241C].m04;
        for (;;) {
            if (++m241C >= 32) {
                m241C = 0;
            }
            if (m241C == old) {
                break;
            }
            if (mKeys[m2418][m241C].m00 != 0 && mKeys[m2418][m241C].m04 != 0 && c != mKeys[m2418][m241C].m04) {
                break;
            }
        }
        if (m2428) {
            m2428(m2418, old, &mKeys[m2418][old], m2418, m241C, &mKeys[m2418][m241C]);
        }
    }
    if (m240C != 0) {
        bool found = false;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 32; col++) {
                Key& key = mKeys[row][col];
                if (key.m00 != 0 && key.m04 != 0 && fn_800C6128(key.m04)) {
                    if (m2428) {
                        m2428(m2418, m241C, &mKeys[m2418][m241C], row, col, &key);
                    }
                    m2418 = row;
                    m241C = col;
                    found = true;
                }
            }
            if (found) {
                break;
            }
        }
    }
    if (m2408 != 0) {
        bool found = false;
        Pos2_unk pos = fn_800C6190();
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 32; col++) {
                Key& key = mKeys[row][col];
                if (key.m00 != 0 && key.m04 != 0 && key.Contains_unk(pos)) {
                    if (m2418 != row || m241C != col) {
                        if (m2428) {
                            m2428(m2418, m241C, &mKeys[m2418][m241C], row, col, &mKeys[row][col]);
                        }
                        m2418 = row;
                        m241C = col;
                    }
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
    if (fn_800C7158()) {
        T c = mKeys[m2418][m241C].m04;
        if (m243C) {
            m243C(m2418, m241C, &mKeys[m2418][m241C]);
        } else {
            fn_800C6E60(c);
        }
    } else if (fn_800C71D0()) {
        if (m2450) {
            m2450(m2418, m241C, &mKeys[m2418][m241C]);
        }
    } else if (fn_800C7248()) {
        if (m2464) {
            m2464();
        }
    } else if (fn_800C72C0()) {
        if (m2478) {
            m2478();
        } else {
            fn_800C6ED8();
        }
    } else if (fn_800C7338()) {
        if (m248C) {
            m248C();
        }
    }
    m2424 = &mKeys[m2418][m241C];
}

template <class T, int N>
void SoftKeyboardBase<T, N>::vf10() {
    if (m24A0.empty()) {
        return;
    }
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 32; col++) {
            Key& key = mKeys[row][col];
            if (key.m00 != 0 && key.m04 != 0) {
                m24A0(row, col, &key, m2424 == &key);
            }
        }
    }
}

template <class T, int N>
void SoftKeyboardBase<T, N>::fn_800C6E60(T c) {
    if (wcslen(mText.m) < m2420 - 1 && wcslen(mText.m) < 0x3FE) {
        T* p = &mText.m[wcslen(mText.m)];
        p[0] = c;
        p[1] = 0;
    }
}

template <class T, int N>
void SoftKeyboardBase<T, N>::fn_800C6ED8() {
    if (wcslen(mText.m) != 0) {
        unsigned long len = wcslen(mText.m);
        mText.m[len - 1] = 0;
    }
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C6F28() {
    bool ret = m2404;
    if (ret) {
        ret = PadRepeat_unk(0);
        if (!ret) {
            ret = PadRepeat_unk(0x1A);
        }
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C6FB4() {
    bool ret = m2404;
    if (ret) {
        ret = PadRepeat_unk(1);
        if (!ret) {
            ret = PadRepeat_unk(0x19);
        }
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C7040() {
    bool ret = m2404;
    if (ret) {
        ret = PadRepeat_unk(2);
        if (!ret) {
            ret = PadRepeat_unk(0x17);
        }
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C70CC() {
    bool ret = m2404;
    if (ret) {
        ret = PadRepeat_unk(3);
        if (!ret) {
            ret = PadRepeat_unk(0x18);
        }
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C7158() {
    bool ret = m2404;
    if (!ret) {
        ret = m2408;
    }
    if (ret) {
        ret = PadTrigger_unk(4);
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C71D0() {
    bool ret = m2404;
    if (!ret) {
        ret = m2408;
    }
    if (ret) {
        ret = PadHold_unk(4);
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C7248() {
    bool ret = m2404;
    if (!ret) {
        ret = m2408;
    }
    if (ret) {
        ret = PadHold_unk(0xE);
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C72C0() {
    bool ret = m2404;
    if (!ret) {
        ret = m2408;
    }
    if (ret) {
        ret = PadTrigger_unk(0xF);
    }
    return ret;
}

template <class T, int N>
bool SoftKeyboardBase<T, N>::fn_800C7338() {
    bool ret = m2404;
    if (!ret) {
        ret = m2408;
    }
    if (ret) {
        ret = PadTrigger_unk(5);
    }
    return ret;
}

template <class T, int N>
void SoftKeyboardBase<T, N>::fn_800C73B0() {
    m2414 = 1;
    if (m2418 < 0 || m241C < 0) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 32; col++) {
                Key& key = mKeys[row][col];
                if (key.m00 != 0 && key.m04 != 0) {
                    m2418 = row;
                    m241C = col;
                    break;
                }
            }
            if (m2418 >= 0 && m241C >= 0) {
                return;
            }
        }
    }
}

template <class T, int N>
void SoftKeyboardBase<T, N>::fn_800C755C() {
    m2414 = 0;
}

template <class T, int N>
void SoftKeyboardBase<T, N>::fn_800C7568(int row, int col) {
    if (row >= 0 && row <= 7 && col >= 0 && col <= 31 && mKeys[row][col].m00 != 0) {
        int oldRow = m2418;
        int oldCol = m241C;
        m2418 = row;
        m241C = col;
        if (m2428) {
            m2428(oldRow, oldCol, &mKeys[oldRow][oldCol], m2418, m241C, &mKeys[m2418][m241C]);
        }
    }
}

// inferred: explicit instantiation of the whole class: the members are emitted in declaration
// order and their weak helpers after them, as in the original
template class SoftKeyboardBase<wchar_t, 0>;

bool SoftKey_unk::Contains_unk(const Pos2_unk& p) const {
    return mMin.x <= p.x && p.x < mMax.x && mMin.y <= p.y && p.y < mMax.y;
}

} // namespace atn
