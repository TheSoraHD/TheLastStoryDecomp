// Copyright (c) TheSoraHD. All rights reserved.

namespace atn {

// NONMATCHING: 65.57% - loop/control-flow and register allocation differ from the original.
// inferred: compares two cached GX state records (0x30 bytes each) followed by
// the corresponding six-word records at offset 0x1B0.
struct GraphicsDeviceState_unk {
    int m00[3];
    unsigned char m0C;
    int m10[3];
    unsigned char m1C;
    int m20[3];
    unsigned char m2C;
};

extern "C" int fn_80071088(const GraphicsDeviceState_unk* a, const GraphicsDeviceState_unk* b) {
    for (int i = 0; i < 9; i++) {
        const GraphicsDeviceState_unk* x = a + i;
        const GraphicsDeviceState_unk* y = b + i;
        if (x->m00[0] != y->m00[0] || x->m00[1] != y->m00[1] || x->m00[2] != y->m00[2] || x->m0C != y->m0C ||
            x->m10[0] != y->m10[0] || x->m10[1] != y->m10[1] || x->m10[2] != y->m10[2] || x->m1C != y->m1C ||
            x->m20[0] != y->m20[0] || x->m20[1] != y->m20[1] || x->m20[2] != y->m20[2] || x->m2C != y->m2C) {
            return 0;
        }
    }

    const int* x = reinterpret_cast<const int*>(a) + (0x1B0 / 4);
    const int* y = reinterpret_cast<const int*>(b) + (0x1B0 / 4);
    for (int i = 0; i < 9 * 6; i++) {
        if (x[i] != y[i]) {
            return 0;
        }
    }
    return 1;
}

} // namespace atn
