#ifndef ATN_DRAWCMD_UNK_H
#define ATN_DRAWCMD_UNK_H

#include "atn/GXDecl.h"

// inferred: draw queue of atn::RenderView_unk (0x958 and 0x895C), filled by atn/ModelDraw_unk.cpp.
// The constructor of DrawCmd_unk and DrawQueue_unk::push_back are defined in atn/ModelDraw_unk.cpp
// (their only user). Names invented.

struct Material_unk;
struct DrawMesh_unk;
struct DrawLens_unk;
struct DrawArray_unk;

// inferred: callbacks around the drawing of a mesh (InstCallback_unk of atn/ModelInstance.cpp)
struct DrawCallback_unk {
    void (*mFunc1)(void*, void*);
    void (*mFunc2)(void*, void*);
    void* mArg1;
    void* mArg2;
};

struct DrawColor_unk {
    float m[4];
};

// inferred: draw command of a mesh (0x4C bytes; 0x1000 of them built by the constructor 0x800BC874)
struct DrawCmd_unk {
    inline DrawCmd_unk(); // inferred: inlined in the constructor 0x800BC874 (atn/RenderSystem_unk.cpp)
    inline DrawCmd_unk(unsigned int flags, Material_unk* mat, DrawMesh_unk* mesh, DrawLens_unk* lens, Mtx* mtx,
                       void* vtx, DrawArray_unk* arrays, DrawColor_unk color, void* m34, void* m38,
                       DrawCallback_unk cb);

    int m00; // inferred: sort key
    unsigned int m04;
    Material_unk* m08;
    DrawMesh_unk* m0C;
    DrawLens_unk* m10;
    Mtx* m14;
    void* m18;
    DrawArray_unk* m1C;
    void* m20;
    DrawColor_unk m24;
    void* m34;
    void* m38;
    DrawCallback_unk m3C;
};

struct DrawSortKey_unk {
    DrawSortKey_unk() {}
    DrawSortKey_unk(int key, int index) : mKey(key), mIndex(index) {}
    int mKey;
    int mIndex;
};

template <class T, int N>
struct DrawQueue_unk {
    inline void push_back(const T& v);
    unsigned int mSize;
    T mData[N];
};

#endif
