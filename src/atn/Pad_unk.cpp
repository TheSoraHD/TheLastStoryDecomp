// atn/Pad_unk.cpp: interfaz de mando del juego sobre el gestor de Wii Remote (nombre de fichero
// inventado). Traduce puerto/boton/eje del juego a los del gestor (lbl_8087F018, atn/WPad_unk.cpp).
#include "atn/Memory.h"

// the Wii Remote manager (lbl_8087F018, 0x40F8 bytes, created by fn_800DDA70): its per-port flags mFlags (bit 0
// classic controller, bit 1 nunchuk?) are written with masks by atn/WPad_unk.cpp and read the same way here
#include "atn/WPad_unk.h"
// inferred: accessors of the flags (names invented)
inline unsigned int FlagBit0_unk(const WPad_unk* wpad, int port) {
    return wpad->mFlags[port] & 1;
}
inline unsigned int FlagBit1_unk(const WPad_unk* wpad, int port) {
    return (wpad->mFlags[port] >> 1) & 1;
}

// inferred: game pad (empty object created by fn_800A5508); the methods do not use `this`
struct Pad_unk {};

extern "C" WPad_unk* lbl_8087F018;
extern "C" void fn_800DDA70();                                        // inferred: creates the manager
extern "C" void fn_800DDD18(WPad_unk* wpad);                          // inferred: per-frame update
extern "C" int fn_800DEEDC(WPad_unk* wpad, int port, int button);     // inferred: triggered
extern "C" int fn_800DEF04(WPad_unk* wpad, int port, int button);
extern "C" int fn_800DEF2C(WPad_unk* wpad, int port, int button);     // inferred: repeated
extern "C" int fn_800DEF54(WPad_unk* wpad, int port, int button);     // inferred: held
extern "C" float fn_800DEF7C(WPad_unk* wpad, int port, int button);   // inferred: analog value
extern "C" float fn_800DEF94(WPad_unk* wpad, int port, int axis);     // inferred: stick axis
extern "C" int fn_800DEFAC(WPad_unk* wpad, int port, int axis, float threshold);
extern "C" void fn_800DF16C(WPad_unk* wpad, int sensitivity);
extern "C" int fn_800DF1D0(WPad_unk* wpad);

// inferred: port, button and axis translation tables
static const int sPortMap[4] = {0, 1, 2, 3};
static const int sButtonMap[34] = {3,    2,    0,    1,    0xB,  0xA,  0xE,  4,    0xC,  0xD,  0x10,
                                   0x11, 9,    8,    4,    0xC,  0xF,  0xF,  0x18, 0x18, 0x18, 0x18,
                                   0x18, 0x14, 0x15, 0x16, 0x17, 0x18, 0x18, 0x18, 0x18, 0x12, 0x13, 0};
static const int sAxisMap[10] = {0, 1, 0, 1, 4, 5, 6, 7, 8, 9};
static const int sAxisMap2[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // inferred: classic controller axes

extern "C" {

Pad_unk* lbl_8087EF70; // inferred: the game pad instance

void fn_800A5508() {
    fn_800DDA70();
    if (lbl_8087EF70 == 0) {
        lbl_8087EF70 = new ((MemoryTag)1, "", "", 0) Pad_unk;
    }
}

void fn_800A5554(Pad_unk* pad) {
    fn_800DDD18(lbl_8087F018);
}

int fn_800A555C(Pad_unk* pad, int port, int button) {
    return fn_800DEEDC(lbl_8087F018, sPortMap[port], sButtonMap[button]);
}

int fn_800A5584(Pad_unk* pad, int port, int button) {
    return fn_800DEF04(lbl_8087F018, sPortMap[port], sButtonMap[button]);
}

int fn_800A55AC(Pad_unk* pad, int port, int button) {
    return fn_800DEF2C(lbl_8087F018, sPortMap[port], sButtonMap[button]);
}

int fn_800A55D4(Pad_unk* pad, int port, int button) {
    return fn_800DEF54(lbl_8087F018, sPortMap[port], sButtonMap[button]);
}

bool fn_800A55FC(Pad_unk* pad, int port, int button) {
    return fn_800DEF54(lbl_8087F018, sPortMap[port], sButtonMap[button]) == 0;
}

float fn_800A5648(Pad_unk* pad, int port, int button) {
    float v = 0.0f;
    v += fn_800DEF7C(lbl_8087F018, sPortMap[port], sButtonMap[button]);
    return v;
}

float fn_800A56A8(Pad_unk* pad, int port, unsigned int axis, int flag) {
    float v = 0.0f;
    switch (axis) {
    case 0:
    case 1: {
        int p = sPortMap[port];
        if (FlagBit0_unk(lbl_8087F018, p) == 1) {
            v += fn_800DEF94(lbl_8087F018, p, sAxisMap2[axis]);
        } else if (flag == 0 || fn_800DEF54(lbl_8087F018, p, 0xD) == 0) {
            v += fn_800DEF94(lbl_8087F018, p, sAxisMap[axis]);
        }
        break;
    }
    case 2:
    case 3: {
        int p = sPortMap[port];
        if (FlagBit0_unk(lbl_8087F018, p) == 1) {
            v += fn_800DEF94(lbl_8087F018, p, sAxisMap2[axis]);
        } else if (flag != 0 && fn_800DEF54(lbl_8087F018, p, 0xD) != 0) {
            v += fn_800DEF94(lbl_8087F018, p, sAxisMap[axis]);
        }
        break;
    }
    case 4:
    case 5:
    case 6:
        v += fn_800DEF94(lbl_8087F018, sPortMap[port], sAxisMap[axis]);
        break;
    default:
        v += fn_800DEF94(lbl_8087F018, sPortMap[port], sAxisMap[axis]);
        break;
    }
    return v;
}

int fn_800A5870(Pad_unk* pad, int port, int axis, float threshold) {
    bool ret = false;
    if (fn_800DEFAC(lbl_8087F018, sPortMap[port], sAxisMap[axis], threshold) != 0) {
        ret = true;
    }
    return ret;
}

// NONMATCHING: register allocation only (the original keeps the manager in r3 and the table in r5)
// (arranque2: also tried locals in both orders, direct member/bitfield access, free inlines with the
// port translation inside, unsigned/bool signatures: all 97.0)
unsigned int fn_800A58D0(Pad_unk* pad, int port) {
    return FlagBit0_unk(lbl_8087F018, sPortMap[port]);
}

// NONMATCHING: same as fn_800A58D0
unsigned int fn_800A58F8(Pad_unk* pad, int port) {
    return FlagBit1_unk(lbl_8087F018, sPortMap[port]);
}

bool fn_800A5920(Pad_unk* pad, int port, int all) {
    for (int i = 0; i < 0x17; i++) {
        if ((all != 0 || (i != 0x11 && i != 0x10)) && fn_800A555C(pad, port, i) != 0) {
            return true;
        }
    }
    if (fn_800DEEDC(lbl_8087F018, sPortMap[port], 0x12) != 0) {
        return true;
    }
    return fn_800DEEDC(lbl_8087F018, sPortMap[port], 0x13) != 0;
}

bool fn_800A59F0(Pad_unk* pad, int port) {
    for (int i = 0; i < 4; i++) {
        float v = 0.0f;
        v += fn_800DEF94(lbl_8087F018, sPortMap[port], sAxisMap[i]);
        if (v > 0.1f) {
            return true;
        }
    }
    return false;
}

bool fn_800A5AA4(Pad_unk* pad, int port) {
    for (int i = 0; i < 4; i++) {
        if (fn_800DEFAC(lbl_8087F018, sPortMap[port], sAxisMap[i], 1.0f) != 0) {
            return true;
        }
    }
    return false;
}

// inferred: a tail call that passes the caller's second argument (GameTask: 5, 10) in r4 unchanged
void fn_800A5B30(Pad_unk* pad, int sensitivity) {
    fn_800DF16C(lbl_8087F018, sensitivity);
}

// inferred: battery level of the remote (a tail call; Loader tests it)
int fn_800A5B38(Pad_unk* pad) {
    return fn_800DF1D0(lbl_8087F018);
}

}
