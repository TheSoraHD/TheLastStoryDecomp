#include "game/CharaStatus.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

// inferred: field objects that own the route tables
struct RouteField_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
};
struct RouteField2_unk {
    unsigned char m00[0x1A6C];
    RouteTable_unk* m1A6C;
};
extern "C" RouteField_unk* lbl_8087F430;
extern "C" RouteField2_unk* lbl_8087F540;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: route tables of the current field (name invented)
inline RouteTable_unk* GetRouteTable_unk() {
    if (lbl_8087F430 != 0) {
        return lbl_8087F430->m10D8;
    }
    if (lbl_8087F540 != 0) {
        return lbl_8087F540->m1A6C;
    }
    return 0;
}

RouteWalker_unk::RouteWalker_unk() {
    mLink = 0;
    m2C = 1;
    mA0 = 0;
    mA4 = 0;
    m70 = 0.0f;
    m74 = 0.0f;
    m78 = 0.0f;
    m7C = 0.0f;
    m80 = 0.0f;
    m84 = 1.0f;
    m88 = 0.0f;
    m8C = 0.0f;
    fn_8012539C();
    m2C = 1;
}

void RouteWalker_unk::fn_8012539C() {
    if (m2C == 0) {
        RouteTable_unk* table = GetRouteTable_unk();
        if (table != 0 && mLink > 0) {
            RouteLink_unk* link = &table->mLinks[mLink - 1];
            link->m3C = 0;
        }
    }
    mMode = -1;
    m48 = 0;
    m90 = 0;
    m64 = lbl_807C7030;
    m98 = 0;
    m94 = 1;
    mLink = 0;
    mPointB = 0;
    mPointA = 0;
    m00 = 0;
    m10 = lbl_807C7030;
    m1C = 0;
    m20 = 0;
    m24 = 0;
    m28 = 0.0f;
    m2C = 0;
}

int RouteWalker_unk::fn_80125474() {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0) {
        return 0;
    }
    if ((mPointA > 0 && mPointA <= table->mNumPoints && table->mPoints[mPointA - 1].m2C == 0) ||
        (mPointB > 0 && mPointB <= table->mNumPoints && table->mPoints[mPointB - 1].m2C == 0) ||
        (mLink > 0 && mLink <= table->mNumLinks && table->mLinks[mLink - 1].m38 == 0)) {
        return 0;
    }
    if (mPointB <= 0) {
        return 0;
    }
    if (mMode == 0 && mPointA <= 0) {
        return 0;
    }
    if (mMode == 1 && mLink <= 0) {
        return 0;
    }
    if ((mMode == 4 || mMode == 6 || mMode == 7) && m24 == 0) {
        return 0;
    }
    return 1;
}

CharaStatusBase::CharaStatusBase() {
    memset(m1A8, 0, sizeof(m1A8));
    memset(this, 0, sizeof(*this));
}

CharaStatus::~CharaStatus() {}
