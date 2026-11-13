#ifndef ATN_EFFECTVERTEX_UNK_H
#define ATN_EFFECTVERTEX_UNK_H

#include "math/Vec.h"

// inferred: vertex of the strips queued by the effect draw code (0x14 bytes; position and texture
// coordinates; taken from fn_80234C84 and queued by fn_80236B84, atn/EffectDesc_unk.cpp)
struct EffectVertex_unk {
    atn::Vector3 mPos;
    float u, v;
};

#endif
