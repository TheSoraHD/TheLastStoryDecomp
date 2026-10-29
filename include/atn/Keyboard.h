#ifndef ATN_KEYBOARD_H
#define ATN_KEYBOARD_H

// atn::Keyboard: USB keyboard of the debug build (names from LastWorld_tools.rso:
// Order, instance_, Press, On, CheckKeyState).

namespace atn {

class Keyboard {
public:
    // inferred: index of the key state array (0x1A bytes each, at 0x34); the enumerator names are invented
    enum Port {
        PORT0_unk,
        PORT1_unk,
        PORT2_unk,
        PORT3_unk,
        PORT4_unk
    };

    // inferred: from CheckKeyState (0x8007708C): bit i of mFlags enables mKeys[i]
    struct KeyState {
        unsigned char mFlags;
        unsigned char m01[0x9];
        unsigned short mKeys[8];
    };

    // inferred: weak copy at 0x802A49BC (lwz instance_; blr)
    static Keyboard* Order() {
        return instance_;
    }

    // inferred: weak copy at 0x802A4968
    int Press(Port port, unsigned short key) const {
        int result = 0;
        if (mD90 != 0 && CheckKeyState(mKeyState[port], key) != 0) {
            result = 1;
        }
        return result;
    }

    int CheckKeyState(const KeyState& state, unsigned short key) const; // 0x8007708C

    static Keyboard* instance_;

    unsigned char m000[0x34];
    KeyState mKeyState[5]; // 0x34
    unsigned char m0B6[0xD90 - 0xB6];
    int mD90; // inferred: keyboard connected
};

} // namespace atn

#endif
