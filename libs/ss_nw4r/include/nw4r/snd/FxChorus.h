#ifndef NW4R_SND_FX_CHORUS_H
#define NW4R_SND_FX_CHORUS_H
#include "nw4r/snd/AxfxImpl.h"
#include "nw4r/snd/FxBase.h"
#include "nw4r/types_nw4r.h"
#include "rvl/AXFX.h" // IWYU pragma: export

// The Last Story: 2010 version (AXFX_CHORUS_EXP and its DPL2 variant), written by analogy with
// FxReverbStdDpl2; the Skyward Sword file has the older AXFX_CHORUS version.
// inferred: layout from the constructor (0x8070DC20) and SetParam (0x8070DE80).

namespace nw4r {
namespace snd {

class FxChorus : public FxBase {
public:
    struct ChorusParam {
        f32 delayTime; // at 0x0
        f32 depth;     // at 0x4
        f32 rate;      // at 0x8
        f32 feedback;  // at 0xC
        f32 outGain;   // at 0x10

        ChorusParam() : delayTime(10.0f), depth(0.5f), rate(1.0f), feedback(0.0f), outGain(1.0f) {}
    };

public:
    FxChorus();

    virtual ~FxChorus() {
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
    bool SetParam(const ChorusParam &rParam);

    void OnChangeOutputMode() override; // at 0x18

private:
    u8 mIsActive;                        // at 0xC
    int mOutputMode;                     // at 0x10
    detail::AxfxImpl mImpl;              // at 0x14
    ChorusParam mParam;                  // at 0x1C
    AXFX_CHORUS_EXP mAxfxParam;          // at 0x30
    AXFX_CHORUS_EXP_DPL2 mAxfxParamDpl2; // at 0xD0
}; // size 0x184

} // namespace snd
} // namespace nw4r

#endif
