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
    Model_unk(int flags, int arg); // 0x80096E94
    virtual ~Model_unk();          // 0x800971D4 (the real class is atn::Model, __vt__Q23atn5Model)
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
    // inferred: matrix of the node called `name` (weak copy 0x800132EC); name invented
    MtxPtr GetNodeMtx_unk(const char* name) {
        return GetNodeMtx_unk(fn_80092814(name, 0));
    }

    unsigned int mFlags; // 0x04
    Mtx mMtx;        // 0x08: inferred: world matrix
    unsigned char m38[0x3C - 0x38];
    Mtx* mNodeMtx;   // 0x3C
    unsigned char m40[0x54 - 0x40];
    float m54;       // inferred: largest scale axis
    // inferred: also the motion controller characters embed at +0xB0 (chr::Motion_unk is an alias:
    // CharaTask constructs it with this constructor, 0x80135588)
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion
    float fn_80097D7C(int track); // inferred: motion length

    unsigned char m58[0x220 - 0x58];
    // inferred: per-node data (0x2C bytes)
    struct Node_unk {
        int m00;
        atn::Vector3 m04;
        unsigned char m10[0x2C - 0x10];
    };
    typedef Node_unk Entry_unk;
    // inferred: motion track (0x30 bytes, six of them at 0x22C; accessors 0x80139550-0x8013C554)
    struct Track_unk {
        int m00;   // inferred: motion id
        unsigned char m04[0x08 - 0x04];
        float m08; // inferred: frame
        float m0C; // inferred: speed
        unsigned char m10[0x20 - 0x10];
        float m20; // inferred: weight
        unsigned char m24[0x30 - 0x24];
    };

    // inferred: accessors called out of line from CharaTask (names keep the address)
    Node_unk* fn_80147A00();
    float fn_80139550(int track);
    void fn_80139F24(int value);
    int fn_80139F2C(int track);
    void fn_80139F3C(int track, float speed);
    int fn_8013C3DC();
    float fn_8013C3E4(int track);
    void fn_8013C554(int track, float weight);

    Node_unk* m220;
    unsigned char m224[0x22C - 0x224];
    Track_unk mTracks[6]; // 0x22C (track 0: frame at 0x234, speed at 0x238)
    int m34C;
    unsigned char m350[0x3D0 - 0x350];
};

// inferred: take an empty boost::function by value (signature unknown, void() assumed)
extern "C" void fn_800902C0(Model_unk* model, int arg, boost::function<void()> callback);
extern "C" void fn_8000D430(Model_unk* model, boost::function<void()> callback);

#endif
