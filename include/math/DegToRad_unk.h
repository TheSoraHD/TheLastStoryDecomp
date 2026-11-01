#ifndef MATH_DEGTORAD_UNK_H
#define MATH_DEGTORAD_UNK_H

// Kept apart from math/VecOps_unk.h: its literal would renumber the @N labels of every file that
// includes the header.

// inferred: converts three angles from degrees to radians in place (weak copy 0x80243EF8, atn/EffectSequencer_unk)
inline void fn_DegToRad_unk(float* v) {
    float x = v[0];
    float y = v[1];
    float z = v[2];
    x = 0.017453292f * x;
    y = 0.017453292f * y;
    z = 0.017453292f * z;
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

#endif
