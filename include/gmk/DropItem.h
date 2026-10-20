#ifndef GMK_DROPITEM_H
#define GMK_DROPITEM_H

#include "game/GimmickCommon.h"

namespace gmk {

// inferred: item placed in the map (gimmick type 18, factory fn_804093B4, not split yet). The name
// is in the DOL (__vt__Q23gmk8DropItem, 0x8078D458). Only the members used by gmk::Item are known.
class DropItem : public GimmickCommon {
public:
    // inferred: setters (names invented); they give the register order of gmk::Item constructor
    void SetPos_unk(const atn::Vector3& pos) { m6C = pos; }
    void SetRot_unk(const atn::Vector3& rot) { m78 = rot; }
    // inferred: in the states 3-5 (name invented)
    bool IsOut_unk() {
        bool ret = false;
        if (3 <= m54 && m54 <= 5) {
            ret = true;
        }
        return ret;
    }
    // inferred: nonzero in the states 3-5 while m168 is not negative (weak copy 0x803FB788, kept
    // from gmk::Cannonball)
    // NONMATCHING: 98.12% (in gmk/Cannonball) - registers of m54 and the result swapped (r4/r5)
    virtual int vf60() {
        int ret = 0;
        if (IsOut_unk() && m168 >= 0) {
            ret = 1;
        }
        return ret;
    }
    unsigned char mF4[0x154 - 0xF4];
    int m154; // inferred: set by vf68
    unsigned char m158[0x168 - 0x158];
    int m168;
    unsigned char m16C[0x180 - 0x16C];
    int m180;
    unsigned char m184[0x190 - 0x184];
    int m190;
    int m194;
    int m198;
};

} // namespace gmk

// inferred: factory of gmk::DropItem (also called by GimmickManager)
extern "C" GimmickCommon* fn_804093B4(atn::Task* parent, int id, int arg);

#endif
