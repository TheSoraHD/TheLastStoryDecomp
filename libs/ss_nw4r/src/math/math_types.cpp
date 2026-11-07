// nw4r::math (NW4R). Source: revvmp/pokemon-rumble-decomp src/nw4r/math/math_types.cpp (MIT), only what
// The Last Story links; the rest was removed by the linker. Inline asm is Nintendo's (pending approval, rule 2).
#include "nw4r/math/types.h"
namespace nw4r { namespace math {
MTX34* MTX34Zero(register MTX34* pMtx) {
    register f32 c_zero = 0.0f;
    asm {
        psq_st c_zero, MTX34._00(pMtx), 0, 0
        psq_st c_zero, MTX34._02(pMtx), 0, 0
        psq_st c_zero, MTX34._10(pMtx), 0, 0
        psq_st c_zero, MTX34._12(pMtx), 0, 0
        psq_st c_zero, MTX34._20(pMtx), 0, 0
        psq_st c_zero, MTX34._22(pMtx), 0, 0
    }
    return pMtx;
}
}}
