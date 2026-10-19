#ifndef ATN_SPHERE_H
#define ATN_SPHERE_H

#include "math/Vec.h"

namespace atn {

// The name is in the DOL: the type_info of the camera callback of MapManager is
// "atn::Scene * (*)(atn::Sphere, void *)" (0x80756690).
// inferred: bounding sphere (center and radius)
struct Sphere {
    Sphere() {}
    // inferred: the center is copied with z loaded first (0x80491D50)
    Sphere(const Sphere& o) {
        mCenter.Assign_unk(o.mCenter);
        mRadius = o.mRadius;
    }
    Vector3 mCenter;
    float mRadius;
};

} // namespace atn

#endif
