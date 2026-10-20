#ifndef CHR_CONTROLMOVECURSOR_H
#define CHR_CONTROLMOVECURSOR_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: target data passed to ControlMoveCursor::fn_801C3458 (built on the stack by
// chr::ControlCannonTarget::vf0C: a position, a radius and a height); names invented
struct CursorTarget_unk {
    atn::Vector3 mPos;
    float mRadius;
    float mHeight;
};

// inferred: base of the controls that move a cursor/target point (vtable 0x80781B90; RTTI bases
// chr::ControlBase). The constructor (0x801C3414) and fn_801C3458 are out of line in an unsplit
// file; the destructor and vf58 are inline (weak copies first emitted in
// chr/ControlCannonLift.cpp, 0x801A1D5C and 0x801A21A4). vf0C stays pure (null in the vtable).
class ControlMoveCursor : public ControlBase {
public:
    ControlMoveCursor(Chara_unk* chara); // 0x801C3414
    virtual int vf58() { return m24; }
    // inferred: moves the cursor (toward `target` when it is not null); 0x801C3458
    void fn_801C3458(const CursorTarget_unk* target, float a, float b, float c);
    // inferred: moves the cursor with the stick (m24 clear); 0x801C346C
    void fn_801C346C(const CursorTarget_unk* target, float maxDist, float maxRadius, float turn);
    // inferred: places the cursor where the camera of the field looks (m24 set); 0x801C3944
    void fn_801C3944(const CursorTarget_unk* target, float a, float dist, float c);
    // inferred: nonzero when nothing blocks the space above the character (ray test from +5 to +23);
    // 0x801C3D1C, used by ControlCharge and ControlChant
    int fn_801C3D1C();

    atn::Vector3 m08; // inferred: cursor position (starts at the character position)
    atn::Vector3 m14; // inferred: starts at the character position too
    float m20;
    bool m24;
};

} // namespace chr

#endif
