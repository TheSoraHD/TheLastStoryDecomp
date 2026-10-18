#ifndef ATN_MODEL_UNK_H
#define ATN_MODEL_UNK_H

#include "atn/GXDecl.h"
#include "math/Vec.h"
#include <boost/function.hpp>

// inferred: material of a model node (returned by Model_unk::fn_80092954)
struct Material_unk {
    unsigned char m00[0x18];
    GXColor mColor; // 0x18
    unsigned char m1C[0x4D - 0x1C];
    signed char m4D;
};
extern "C" void fn_8007FAF0(Material_unk* material, int mode); // inferred: sets the draw mode

// inferred: model instance embedded at MapPartsModel+0x50 (probably the same class as
// chr::Motion_unk, which characters embed at +0xB0). Names invented.
struct Model_unk {
    Material_unk* fn_80092954(const char* name); // inferred: material by name
    void fn_80092A4C();                          // inferred
    const char* fn_8008B130();                   // inferred: model name
    void fn_8008CD60();                          // inferred
    void fn_800954DC(void* owner, atn::Vector2_unk uv, float f); // inferred: UV scroll
    void fn_80097E80(int arg);                                   // inferred
    void fn_8009385C(int motion, bool play, int arg2, int arg3);  // inferred
    int fn_8008B140();                                // inferred: nonzero while loading
    int fn_80092814(const char* name, int arg);       // inferred: node index by name, < 0 if missing

    // inferred: matrix of node `index`, null for a missing node (inlined at 0x8049CCD4)
    MtxPtr GetNodeMtx_unk(int index) {
        return (index < 0) ? 0 : mNodeMtx[index];
    }

    unsigned char m00[0x04];
    unsigned int mFlags; // 0x04
    Mtx mMtx;        // 0x08: inferred: world matrix
    unsigned char m38[0x3C - 0x38];
    Mtx* mNodeMtx;   // 0x3C
    unsigned char m40[0x54 - 0x40];
    float m54;       // inferred: largest scale axis
    unsigned char m58[0x234 - 0x58];
    float mFrame;    // 0x234
    unsigned char m238[0x3D0 - 0x238];
};

// inferred: take an empty boost::function by value (signature unknown, void() assumed)
extern "C" void fn_800902C0(Model_unk* model, int arg, boost::function<void()> callback);
extern "C" void fn_8000D430(Model_unk* model, boost::function<void()> callback);

#endif
