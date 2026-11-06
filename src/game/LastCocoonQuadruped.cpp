#include "game/LastCocoonQuadruped.h"

void LastCocoonQuadruped::vf14() {
    fn_80149A30();
}

void LastCocoonQuadruped::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14C4 != 7 && m58C != 8) {
        if (m14C4 == 6 || m14C4 == 1 || (m58C >= 17 && m58C <= 19)) {
            msg->m10 *= 0.0f;
        }
        fn_80151448(msg);
        vf40(hit);
    }
}

atn::Vector3 LastCocoonQuadruped::vf64(int center) {
    atn::Vector3 result = mPos;
    result = m624.mData[0].m04.mCenter;
    return result;
}

int LastCocoonQuadruped::vfE4() {
    int ret = 0;
    if (m58C == 16 && m14BC == 1) {
        ret = 1;
    }
    return ret;
}

float LastCocoonQuadruped::vf90() {
    return 3.0f;
}
