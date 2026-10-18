#ifndef ATN_MODEL_UNK_H
#define ATN_MODEL_UNK_H

#include "atn/GXDecl.h"
#include "math/Vec.h"
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

namespace atn {
class Matrix3x4;
struct NodeChunk;
struct ModelNodeCallbackContext;
}
// inferred: camera object owned by a model (0x1FC bytes, created on load by CharaTask::fn_8013655C
// and by the equipment at 0x80564108); defined where it is used
struct ModelCamera_unk;

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
    // inferred: four values copied to the parts and equipment of a character (CharaTask::fn_80148B38)
    struct Float4_unk {
        float m00;
        float m04;
        float m08;
        float m0C;
    };
    Float4_unk m40;
    unsigned char m50[0x54 - 0x50];
    float m54;       // inferred: largest scale axis
    // inferred: also the motion controller characters embed at +0xB0 (chr::Motion_unk is an alias:
    // CharaTask constructs it with this constructor, 0x80135588)
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion
    float fn_80097D7C(int track); // inferred: motion length
    // inferred: called on load by CharaTask (names keep the address)
    void fn_8008AD4C(const char* path, const char* node); // inferred: loads the model ("MODEL")
    void fn_80092F1C(const char* path, const char* node, float distance); // inferred: adds a LOD
    void fn_80092EC8(const char* path);   // inferred: shadow LOD
    void fn_80097A88(int motion, const char* path); // inferred: loads a motion
    void fn_80092F90();
    void fn_80093014(int arg);
    void fn_8009373C(const char* name, int part); // inferred: shows/hides the named node (option part)
    void fn_80094958(const char* name, const void* a, const void* b, const void* c); // inferred: material edit
    void fn_80093D98(const char* name, int arg); // inferred: material hide
    void fn_80093F58(const void* edit); // inferred: node edit
    void fn_800973C0(void* csv); // inferred: scale
    void fn_80095D44(int node, int parent);
    void fn_80091CFC(const char* node);
    void fn_80099E9C(const char* node, const char* name, float value);
    void fn_80097510();
    int fn_80097D40(int motion); // inferred: zero if the motion is missing

    unsigned char m58[0xAC - 0x58];
    float mAC; // inferred: draw distance scale (CharaTask sets 1.0 or 1.3, 0x80149A30)
    int mB0; // inferred: compared with 0 by CharaTask::fn_8013655C
    unsigned char mB4[0x208 - 0xB4];
    boost::scoped_ptr<ModelCamera_unk> m208; // inferred
    unsigned int m20C_31_6 : 26;
    int mFlag5_unk : 1;          // inferred: tested by CharaTask::fn_8013655C
    unsigned int mFlag4_unk : 1; // inferred: set together with the equipment
    unsigned int m20C_3_0 : 4;
    unsigned char m210[0x220 - 0x210];
    // inferred: accessor (the original reads the flag word again before setting mFlag4_unk); name invented
    int IsFlag5_unk() const { return mFlag5_unk; }
    // inferred: per-node data (0x2C bytes)
    struct Node_unk {
        unsigned int m00; // inferred: flags (2: the rotation m10 is set, CharaTask::fn_80148CC4)
        atn::Vector3 m04;
        atn::Quaternion_unk m10;
        unsigned char m20[0x2C - 0x20];
    };
    // inferred: resource data of the model (accessor 0x8000DD04); names invented
    struct NodeRes_unk {
        unsigned char m00[0x40];
        atn::Vector3 m40; // inferred: initial rotation (euler)
    };
    struct Res_unk {
        unsigned char m00[0x48];
        NodeRes_unk** m48;
    };
    Res_unk* fn_8000DD04(); // inferred: returns m16C
    typedef Node_unk Entry_unk;
    // inferred: motion track (0x30 bytes, six of them at 0x22C; accessors 0x80139550-0x8013C554)
    struct Track_unk {
        int m00;   // inferred: motion id
        unsigned char m04[0x08 - 0x04];
        float m08; // inferred: frame
        float m0C; // inferred: speed
        unsigned char m10[0x18 - 0x10];
        unsigned char m18; // inferred: nonzero = the end is tested one step early (CharaTask::fn_8015EB2C)
        unsigned char m19[0x20 - 0x19];
        float m20; // inferred: weight
        unsigned char m24[0x30 - 0x24];
    };

    // inferred: accessors called out of line from CharaTask (names keep the address)
    Node_unk* fn_80147A00();
    struct Sphere_unk {
        atn::Vector3 mCenter;
        float mRadius;
    };
    Sphere_unk fn_80094F98(); // inferred: bounding sphere
    float fn_80139550(int track);
    void fn_80139F24(int value);
    int fn_80139F2C(int track);
    void fn_80139F3C(int track, float speed);
    int fn_8013C3DC();
    float fn_8013C3E4(int track);
    void fn_8013C554(int track, float weight);
    float GetSpeed_unk(int track) { return mTracks[track].m0C; } // inferred: name invented

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
