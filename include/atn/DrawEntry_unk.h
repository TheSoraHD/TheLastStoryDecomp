#ifndef ATN_DRAWENTRY_UNK_H
#define ATN_DRAWENTRY_UNK_H

// inferred: draw state of a model, shared by atn/Model.cpp (fn_8008E818 builds it) and atn/ModelDraw_unk.cpp
// (its constructor 0x8009B7F4 and the draw fn_8009B93C). LightSet_unk must be defined before (atn/Light_unk.h or
// the local copy of atn/ModelDraw_unk.cpp, same layout).
#include "atn/GXDecl.h"
#include "atn/DrawCmd_unk.h"
#include "atn/Sphere.h"

namespace atn {
class LH_Model;
class Scene;
}
struct DrawModel_unk;
struct DrawRemap_unk;
struct DrawMaterials_unk;

// inferred: draw state of a model (built on the stack by atn::Model, fn_8008E818)
struct DrawEntry_unk {
    DrawEntry_unk(atn::LH_Model* model, Mtx* mtx, Mtx* nodes, int m18, DrawColor_unk* color, DrawMaterials_unk* mats,
                  DrawRemap_unk* remap, const atn::Sphere* sphere, int* m40, unsigned int flags, atn::Scene* lens,
                  int m98, const DrawCallback_unk* cb);

    int m00; // inferred: pass
    int m04;
    atn::LH_Model* m08;
    DrawModel_unk* m0C;
    Mtx* m10;
    Mtx* m14; // inferred: node matrices
    int m18;
    DrawColor_unk* m1C;
    DrawMaterials_unk* m20;
    DrawRemap_unk* m24;
    int m28;
    float m2C; // inferred: largest scale
    atn::Sphere m30;
    int* m40; // inferred: node remap
    unsigned char* m44; // inferred: node visibility
    int m48; // inferred: only the "op_" nodes
    unsigned char* m4C; // inferred: material visibility
    short* m50; // inferred: nodes to draw
    unsigned int m54;
    int m58;
    void* m5C;
    LightSet_unk m60;
    atn::Scene* m84;
    DrawCallback_unk m88;
    int m98;
};

extern "C" void fn_8009B93C(DrawEntry_unk*);

#endif
