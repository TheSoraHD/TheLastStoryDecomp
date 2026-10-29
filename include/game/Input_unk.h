#ifndef GAME_INPUT_UNK_H
#define GAME_INPUT_UNK_H

#include "math/Vec.h"

// inferred: texture coordinates of a button icon (copied with paired singles)
struct ButtonUv_unk {
    ButtonUv_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    ButtonUv_unk(const ButtonUv_unk& o) { __Vec4Copy(&u0, &o.u0); }
    float u0;
    float v0;
    float u1;
    float v1;
};

// inferred: game actions read from the pad (0x50 bytes at +0x48C of the settings behind
// lbl_8087F0A8; game/PlayerInput_unk.cpp). An action (0..0x35) is mapped to a pad button (0x21: none)
// by fn_80122C70. Class name invented; the method names are the addresses.
class Input_unk {
public:
    Input_unk();
    void fn_80122330(int reset);       // inferred: update (counters and camera yaw)
    void fn_8012256C();                // inferred: releases the locked actions whose button is up
    int fn_80122C70(int action);       // inferred: pad button of an action
    int fn_801231D0(int action);       // inferred: action triggered
    int fn_801237F0(int action);       // inferred: action repeated
    int fn_80123C7C(int action);       // inferred: fn_800A5584 on the button of the action
    int fn_801240B4(int action);       // inferred: action held
    inline int Held_unk(int action);
    int fn_8012476C(int action);       // inferred: remote swung (action 0x22)
    float fn_801248DC(atn::Vector3* dir); // inferred: stick strength; `dir` gets its direction
    int fn_80124B60();                 // inferred: input locked (leader controlled by the game)
    void fn_80124BE4();                // inferred: resets the camera yaw
    void fn_80124BFC(int button, int lock); // inferred: locks the actions mapped to `button`
    ButtonUv_unk fn_80124C6C(int action); // inferred: icon of the button of an action

    int m00;                  // inferred: frames action 0x11 is held
    int m04;                  // inferred: frames the button of action 8 is held
    int m08;                  // inferred: frames action 3 is held
    float m0C;                // inferred: camera yaw
    int m10;
    unsigned char m14[0x36];  // inferred: locked actions (fn_80122C70 returns 0x21)
    int m4C;                  // inferred: frames action 7 is held
};

#endif
