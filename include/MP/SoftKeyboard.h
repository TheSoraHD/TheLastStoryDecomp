#ifndef MP_SOFTKEYBOARD_H
#define MP_SOFTKEYBOARD_H

#include "atn/Task.h"

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);

namespace MP {

// inferred: software keyboard of the multiplayer menus (lbl_8087F860); only the members used by
// MP/D2AMpMenu are known
class SoftKeyboard : public atn::Task {
public:
    static void fn_80502974(atn::Task* parent); // inferred: creates the instance
    void fn_805053F8();                         // inferred: opens the keyboard

    // inferred: sets the text (wcslen + wmemcpy, as the wide static_string assignment)
    void SetText_unk(const wchar_t* text) {
        if (text != mFC) {
            wmemcpy(mFC, text, wcslen(text) + 1);
        }
    }

    unsigned char m048[0xFC - 0x48];
    wchar_t mFC[0x10];  // inferred: text entered
    unsigned char m11C[0x25AC - 0x11C];
    int m25AC;          // inferred: nonzero while the keyboard is open
    unsigned char m25B0[0x25B8 - 0x25B0];
    int m25B8;          // inferred: nonzero when the text was decided
};

} // namespace MP

extern "C" MP::SoftKeyboard* lbl_8087F860;

#endif
