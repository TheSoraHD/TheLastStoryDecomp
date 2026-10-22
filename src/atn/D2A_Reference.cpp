#include "atn/D2A_Reference.h"

namespace atn {

D2A_Reference::D2A_Reference(D2A_Data* owner, unsigned short id) : D2A_Common(owner, id) {
    mType = 5;
    mRect.mValues[4] = -100.0f;
    mRect.mTables[2].fn_801F04FC(0, 1, 1.0f);
    mRect.mTables[3].fn_801F04FC(0, 1, 1.0f);
}

D2A_Reference::~D2A_Reference() {}

void D2A_Reference::vf18() {}

} // namespace atn
