#include "gmk/BulletPoint.h"
#include "atn/Memory.h"

namespace gmk {

GimmickCommon* BulletPoint::fn_8041B970(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) BulletPoint(parent, desc);
    }
    return 0;
}

int BulletPoint::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        return 1;
    }
    return 0;
}

void BulletPoint::vf28() {}

void BulletPoint::vf14() {}

int BulletPoint::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

int BulletPoint::vf60() {
    int ret = 0;
    if (m54 == 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

} // namespace gmk
