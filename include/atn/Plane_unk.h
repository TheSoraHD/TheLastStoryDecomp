#ifndef ATN_PLANE_UNK_H
#define ATN_PLANE_UNK_H

#include "math/Vec.h"

// inferred: plane (normal, distance); names invented
struct Plane_unk {
    atn::Vector3 mNormal;
    float mDist;
};

#endif
