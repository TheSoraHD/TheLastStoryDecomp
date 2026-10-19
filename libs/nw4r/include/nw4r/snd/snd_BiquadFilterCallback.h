#ifndef NW4R_SND_BIQUAD_FILTER_CALLBACK_H
#define NW4R_SND_BIQUAD_FILTER_CALLBACK_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {

// inferred: biquad filter types; AxManager keeps a table of 128 callbacks
// (0x200 bytes) and installs the 5 presets at indices 1-5
enum BiquadFilterType {
    BIQUAD_FILTER_TYPE_NONE,
    BIQUAD_FILTER_TYPE_LPF,
    BIQUAD_FILTER_TYPE_HPF,
    BIQUAD_FILTER_TYPE_BPF512,
    BIQUAD_FILTER_TYPE_BPF1024,
    BIQUAD_FILTER_TYPE_BPF2048,

    BIQUAD_FILTER_TYPE_USER_MIN = 64, // inferred
    BIQUAD_FILTER_TYPE_USER_MAX = 127
};

class BiquadFilterCallback {
public:
    // inferred: five u16 coefficients written by GetCoef (b0, b1, b2, a1, a2)
    struct Coefficients {
        u16 b0; // at 0x0
        u16 b1; // at 0x2
        u16 b2; // at 0x4
        u16 a1; // at 0x6
        u16 a2; // at 0x8
    };

public:
    virtual ~BiquadFilterCallback() {} // at 0x8

    // inferred: (type, value, out coefficients)
    virtual void GetCoef(int type, f32 value, Coefficients* pCoef) = 0; // at 0xC
};

} // namespace snd
} // namespace nw4r

#endif
