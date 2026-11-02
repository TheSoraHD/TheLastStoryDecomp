#include "atn/Keyboard.h"

namespace atn {

// inferred: 0x8087EEF0 (.sbss)
Keyboard* Keyboard::instance_;

// 0x8007708C
int Keyboard::CheckKeyState(const KeyState& state, unsigned short key) const {
    for (int i = 0; i < 8; i++) {
        if (((1 << i) & state.mFlags) == (1 << i) && key == state.mKeys[i]) {
            return 1;
        }
    }
    return 0;
}

} // namespace atn
