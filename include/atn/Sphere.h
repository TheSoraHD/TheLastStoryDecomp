#ifndef ATN_SPHERE_H
#define ATN_SPHERE_H

#include "math/Vec.h"

namespace atn {

// The name is in the DOL: the type_info of the camera callback of MapManager is
// "atn::Scene * (*)(atn::Sphere, void *)" (0x80756690).
// inferred: bounding sphere (center and radius)
struct Sphere {
    Sphere() {}
    // inferred: name invented (SkillEffectManager::fn_8059D3B0 builds the argument in place)
    Sphere(const Vector3& center, float radius) : mCenter(center), mRadius(radius) {}
    // inferred: implicit copy (center with paired singles, then the radius): it is what the copies of
    // the by-value spheres of RenderManager::fn_800C122C and MapManager::fn_80491D50 give (renderview-20261105)
    // inferred: the center is copied with z loaded first (0x80491D50)
    Vector3 mCenter;
    float mRadius;
};

} // namespace atn

#endif
