#ifndef EVENT_POSEKEY_UNK_H
#define EVENT_POSEKEY_UNK_H

#include "math/Vec.h"

namespace Event {

// inferred: one key of a pose (0x2C bytes; constructor 0x8000FAE4)
struct PoseKey_unk {
    PoseKey_unk() {}
    PoseKey_unk& operator=(const PoseKey_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08.Assign_unk(o.m08);
        m14.Assign_unk(o.m14);
        m20.Assign_unk(o.m20);
        return *this;
    }

    unsigned int m00; // inferred: node name hash (compared unsigned by CharaFaceControl::fn_8011D844)
    int m04;
    atn::Vector3 m08;
    atn::Vector3 m14;
    atn::Vector3 m20;
};
// inferred: also the element of the node edits of CharaFaceControl (Vector_unk<PoseKey_unk>, read
// with fn_80089EE4 from "NODE_EDIT" lines); moved here from Event/MotionData.h

} // namespace Event

#endif
