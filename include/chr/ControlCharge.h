#ifndef CHR_CONTROLCHARGE_H
#define CHR_CONTROLCHARGE_H

#include "chr/ControlMoveCursor.h"

namespace chr {

// inferred: base of the attacks that jump to a point after a charge (vtable 0x8077F500, vf0C pure).
// m08 is the target point, m14 the start point.
class ControlChargeAttackBase : public ControlBase {
public:
    ControlChargeAttackBase(Chara_unk* chara, const atn::Vector3& target, float speed, int adjust); // 0x801A4B00
    virtual ~ControlChargeAttackBase();
    virtual atn::Vector3 vf18();
    virtual int vf24();
    virtual int vf28();
    virtual int vf44() { return vf24(); }
    virtual int vf48() { return 0x23; }
    virtual atn::Vector3 vf4C() { return m08; } // NONMATCHING: 60% - z stored first (as vf18)
    virtual int vf54() { return m28; }
    // inferred: moves `target` to a reachable point in front of `from` (ray tests); 0x801A4E08
    static void fn_801A4E08(Chara_unk* chara, const atn::Vector3& from, atn::Vector3& target);

    atn::Vector3 m08; // inferred: target point
    atn::Vector3 m14; // inferred: start point
    float m20;        // inferred: vertical speed
    float m24;        // inferred: initial value of m20
    int m28;          // inferred: nonzero once the attack hits
};

// inferred: attack after ControlCharge when the hit kind (HitInfo_unk::m01) is 1
class ControlChargeAttack : public ControlChargeAttackBase {
public:
    ControlChargeAttack(Chara_unk* chara, const atn::Vector3& target, int adjust); // 0x801A59F4
    virtual int vf0C();
};

// inferred: control created by CharaTask::fn_801588A0 and by ControlCharge::vf10. Class name from
// the vtable stored by the constructor, size from the operator new call.
class ControlHighJumpAttack : public ControlChargeAttackBase {
public:
    ControlHighJumpAttack(Chara_unk* chara, const atn::Vector3& target, float speed, int adjust); // 0x801A5E90
    virtual int vf0C();
    virtual void vf1C();
    float m2C; // inferred: height of the jump
};

// inferred: magic 0x1788/0x1791 with flag 0x40000 (Quark/Seiren); 0x801A6750
class ControlQuarkSeirenAttack : public ControlChargeAttackBase {
public:
    ControlQuarkSeirenAttack(Chara_unk* chara, const atn::Vector3& target, float speed, int adjust);
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    float m2C;
};

// inferred: created by game/BattleCharaTask (operator new of 0x10); 0x801A72EC
class ControlZanTackle : public ControlBase {
public:
    ControlZanTackle(Chara_unk* chara);
    virtual int vf0C();
    virtual void vf1C() {}
    int m08;   // inferred: id from fn_800F8548 (passed to the hit)
    float m0C; // inferred: copied from CharaTask::m56C
};

// inferred: created by CharaTask::fn_8016D74C; class name from the vtable (RTTI bases
// ControlBase, ControlMoveCursor)
class ControlCharge : public ControlMoveCursor {
public:
    ControlCharge(Chara_unk* chara, int a, const atn::Vector3* target, int c); // 0x801A2CEC
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    inline void AimAt_unk(HitInfo_unk* hit, CharaTask* target);

    float m28;         // inferred: range of the attack (HitInfo_unk::m58)
    float m2C;         // inferred: charge time (HitInfo_unk::mC0 when `a` is set)
    int m30;
    int m34;
    int m38;           // inferred: nonzero when the target was given
    int m3C;
    HitInfo_unk* m40;  // inferred: attack being charged
};

} // namespace chr

#endif
