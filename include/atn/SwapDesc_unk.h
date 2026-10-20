#ifndef ATN_SWAPDESC_UNK_H
#define ATN_SWAPDESC_UNK_H

// inferred: byte swap description of a binary structure: list of {type, count} terminated by
// type -1; type 0 skips `count` bytes, 1/2/3 swap `count` 16/32/64-bit values, 4 repeats the
// sub-description `count` times (atn/EfpHandle_unk.cpp fn_802371BC). Names invented.
struct SwapDesc_unk {
    short mType;
    short mCount;
    const SwapDesc_unk* mSub;
};

extern "C" int fn_802371BC(unsigned char* data, const SwapDesc_unk* desc);

#endif
