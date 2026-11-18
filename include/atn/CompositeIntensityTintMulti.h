#ifndef ATN_COMPOSITEINTENSITYTINTMULTI_H
#define ATN_COMPOSITEINTENSITYTINTMULTI_H

#include "atn/TextureCompositor_unk.h"
#include "atn/CompositeCache_unk.h"
#include "atn/TextureComposite.h"

namespace atn {

// inferred: tints the intensity of several layers and composes them on a base texture (0x38C bytes;
// atn/CompositeIntensityTintMulti.cpp, created by db/OptionParts_unk.cpp)
class CompositeIntensityTintMulti : public ITextureCompositor {
public:
    CompositeIntensityTintMulti(const char* base, const CompositeLayerDesc_unk* layers, int count);
    virtual ~CompositeIntensityTintMulti();
    virtual int vf0C();
    virtual int vf10();
    virtual void vf14();
    virtual Color4f_unk* vf18(int i);

    int mDone;                        // 0x0C
    TextureComposite mComposite;      // 0x10
    Color4f_unk mColors[6][3];        // 0x30
    int mUse[6][3];                   // 0x150
    Texture_unk mBase;                // 0x198
    Texture_unk mLayers[6];           // 0x1C8
    Texture_unk mIntensity;           // 0x2E8
    TextureComposite mIntensityComp;  // 0x318
    Texture_unk mAlpha;               // 0x338
    TextureComposite mAlphaComp;      // 0x368
    int mCount;                       // 0x388
};

} // namespace atn

#endif
