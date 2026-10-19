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
    unsigned char mF4[0x154 - 0xF4];
    int m154; // inferred: set by vf68
    unsigned char m158[0x180 - 0x158];
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
