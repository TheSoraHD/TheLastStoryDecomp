#ifndef NW4R_SND_FX_DELAY_H
#define NW4R_SND_FX_DELAY_H
#include "nw4r/snd/AxfxImpl.h"
#include "nw4r/snd/FxBase.h"
#include "nw4r/types_nw4r.h"
#include "rvl/AXFX.h" // IWYU pragma: export

// The Last Story: 2010 version (AXFX_DELAY_EXP and its DPL2 variant), written by analogy with
// FxReverbStdDpl2; the Skyward Sword file has the older AXFX_DELAY version.
// inferred: layout from the constructor (0x8070E190) and SetParam (0x8070E3F0).

namespace nw4r {
namespace snd {

class FxDelay : public FxBase {
public:
    struct DelayParam {
        f32 delay;    // at 0x0
        f32 feedback; // at 0x4
        f32 outGain;  // at 0x8
        f32 maxDelay; // at 0xC
        f32 lpf;      // at 0x10

        DelayParam() : delay(160.0f), feedback(0.4f), outGain(1.0f), maxDelay(160.0f), lpf(1.0f) {}
    };

public:
    FxDelay();

    virtual ~FxDelay() {
        Shutdown();
        ReleaseWorkBuffer();
    } // at 0x8

    virtual bool StartUp() override;  // at 0xC
    virtual void Shutdown() override; // at 0x10

    virtual void UpdateBuffer(
        int channels, void **ppBuffer, u32 size, SampleFormat format, f32 sampleRate,
        OutputMode mode
    ) override; // at 0x14

    virtual bool AssignWorkBuffer(void *pBuffer, u32 size); // at 0x1C
    virtual void ReleaseWorkBuffer();                       // at 0x20

    u32 GetRequiredMemSize();
    bool SetParam(const DelayParam &rParam);

    void OnChangeOutputMode() override; // at 0x18

private:
    u8 mIsActive;                       // at 0xC
    int mOutputMode;                    // at 0x10
    detail::AxfxImpl mImpl;             // at 0x14
    DelayParam mParam;                  // at 0x1C
    AXFX_DELAY_EXP mAxfxParam;          // at 0x30
    AXFX_DELAY_EXP_DPL2 mAxfxParamDpl2; // at 0x88
}; // size 0xE8

} // namespace snd
} // namespace nw4r

#endif
