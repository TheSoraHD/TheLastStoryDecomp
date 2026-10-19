#ifndef NW4R_SND_BIQUAD_FILTER_PRESET_H
#define NW4R_SND_BIQUAD_FILTER_PRESET_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BiquadFilterCallback.h>

namespace nw4r {
namespace snd {
namespace detail {

// inferred: the five preset callbacks installed by AxManager::Init; their
// GetCoef functions live in snd_BiquadFilterPreset.cpp (0x8070B070)

class BiquadFilterLpf : public BiquadFilterCallback {
public:
    virtual void GetCoef(int type, f32 value, Coefficients* pCoef);
};

class BiquadFilterHpf : public BiquadFilterCallback {
public:
    virtual void GetCoef(int type, f32 value, Coefficients* pCoef);
};

class BiquadFilterBpf512 : public BiquadFilterCallback {
public:
    virtual void GetCoef(int type, f32 value, Coefficients* pCoef);
};

class BiquadFilterBpf1024 : public BiquadFilterCallback {
public:
    virtual void GetCoef(int type, f32 value, Coefficients* pCoef);
};

class BiquadFilterBpf2048 : public BiquadFilterCallback {
public:
    virtual void GetCoef(int type, f32 value, Coefficients* pCoef);
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
