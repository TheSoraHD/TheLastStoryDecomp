#ifndef ATN_EFFECTPARAMS_UNK_H
#define ATN_EFFECTPARAMS_UNK_H

// inferred: render parameters (lbl_8087EFA8) as seen by the effects (fields named by offset; other
// files have their own views of the same object)
struct EffectParams_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" EffectParams_unk* lbl_8087EFA8;

#endif
