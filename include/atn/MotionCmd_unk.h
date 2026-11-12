#ifndef ATN_MOTIONCMD_UNK_H
#define ATN_MOTIONCMD_UNK_H

namespace atn {
struct MotionCmd_unk;
}
extern "C" void fn_800A1060(atn::MotionCmd_unk* cmd); // inferred: releases the motion bank (atn/Motion_unk.cpp)

namespace atn {

// inferred: motion command entry of a motion slot (16 bytes, atn::Model::m354): key of the (model hash,
// motion id) pair in the motion banks of atn/Motion_unk.cpp and the bank, the motion key of each node
// (-1: none); its destructor releases the bank (fn_800A1060); names invented
struct MotionCmd_unk {
    MotionCmd_unk() { mKey = 0; mData = 0; } // inferred: weak ctor 0x80097BF4
    ~MotionCmd_unk() { fn_800A1060(this); }
    unsigned long long mKey; // 0x00
    short* mData;            // 0x08
    int m0C;
};

} // namespace atn

#endif
