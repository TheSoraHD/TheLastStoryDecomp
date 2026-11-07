#ifndef NW4R_MATH_TRIANGULAR_GAME_H
#define NW4R_MATH_TRIANGULAR_GAME_H

// nw4r::math trigonometry as used by the game (macros and inline as in NW4R math_constant.h and
// math_triangular.h; see libs/ss_nw4r/include/nw4r/math/triangular.h). Only the parts used are declared.

#define NW4R_MATH_PI 3.141592653589793f

// Convert radians -> lookup-table index (256 units a turn)
#define NW4R_MATH_RAD_TO_FIDX(x) ((x) * (128.0f / NW4R_MATH_PI))

namespace nw4r {
namespace math {

void SinCosFIdx(float* pSin, float* pCos, float fidx);

inline void SinCosRad(float* pSin, float* pCos, float rad) {
    return SinCosFIdx(pSin, pCos, NW4R_MATH_RAD_TO_FIDX(rad));
}

} // namespace math
} // namespace nw4r

#endif
