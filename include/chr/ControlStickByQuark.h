#ifndef CHR_CONTROLSTICKBYQUARK_H
#define CHR_CONTROLSTICKBYQUARK_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlStickByQuark.cpp created by CharaTask (0x8015BAF0-0x8015D5B0).
// Only the constructors are declared; sizes from the operator new calls, argument types from the
// constructors (ints stored as words, vectors read through pointers).

class ControlCdnStone : public ControlBase {
public:
    ControlCdnStone(Chara_unk* chara, int a); // 0x8018EEC4 (first function of this unit)
    virtual int vf0C();
    unsigned char m08[0x28 - 0x08];
};

class ControlThrownAway : public ControlBase {
public:
    ControlThrownAway(Chara_unk* chara, int a, int b); // 0x80199348
    virtual int vf0C();
    unsigned char m08[0x10 - 0x08];
};

class ControlThrownAvoid : public ControlBase {
public:
    ControlThrownAvoid(Chara_unk* chara, int a, const atn::Vector3& dir); // 0x80199788
    virtual int vf0C();
    unsigned char m08[0x18 - 0x08];
};

// inferred: controls of the skeletons grabbed by Em002_02SkeltonKing (vtables in this unit)
class ControlCaughtSkelton : public ControlBase {
public:
    ControlCaughtSkelton(Chara_unk* chara, CharaTask* holder); // 0x8019A3F4
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

class ControlThrownSkelton : public ControlBase {
public:
    ControlThrownSkelton(Chara_unk* chara, const atn::Vector3& target, HitInfo_unk* data); // 0x8019A580
    virtual int vf0C();
    unsigned char m08[0x24 - 0x08];
};

class ControlThrownBreakSkelton : public ControlBase {
public:
    ControlThrownBreakSkelton(Chara_unk* chara); // 0x8019AFE8
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

class ControlThreadCatch : public ControlBase {
public:
    ControlThreadCatch(Chara_unk* chara, int a, int b); // 0x8019B29C
    virtual int vf0C();
    unsigned char m08[0x28 - 0x08];
};

class ControlThreadDead : public ControlBase {
public:
    ControlThreadDead(Chara_unk* chara, int a); // 0x8019BD48
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

class ControlThreadRelease : public ControlBase {
public:
    ControlThreadRelease(Chara_unk* chara); // 0x8019BE9C
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

class ControlAbsorbedHP : public ControlBase {
public:
    ControlAbsorbedHP(Chara_unk* chara, int a, int b); // 0x8019DA50
    virtual int vf0C();
    unsigned char m08[0x24 - 0x08];
};

class ControlCliffhangZanGurg : public ControlBase {
public:
    ControlCliffhangZanGurg(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b, atn::Vector3 c); // 0x8019E4F4
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlEndCliffhangZanGurg : public ControlBase {
public:
    ControlEndCliffhangZanGurg(Chara_unk* chara, atn::Vector3 a); // 0x8019EB7C
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

class ControlStickZanGurgStand : public ControlBase {
public:
    ControlStickZanGurgStand(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b); // 0x8019F0A8
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

class ControlStickZanGurgCliff : public ControlBase {
public:
    ControlStickZanGurgCliff(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b); // 0x8019F8E8
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

// inferred: created by CharaTask::fn_80164168 (the caller passes 15 as the last argument)
class ControlBoundShade : public ControlBase {
public:
    ControlBoundShade(Chara_unk* chara, int a, int b, int c); // 0x8019BFF0
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

// inferred: control id 0x43; fn_80196C68 lies between the vf0C of ControlStickSwordSp (0x80196A98)
// and the one of ControlMatHoldGuard (0x80196CD4), the class is a guess
class ControlStickSwordSp : public ControlBase {
public:
    virtual int vf0C();
    bool fn_80196C68(); // inferred: can be cancelled
};

// inferred: class names from the vtables stored by the constructors (0x8015AF40-0x8015B808)
class ControlMatHold : public ControlBase {
public:
    ControlMatHold(Chara_unk* chara, int a); // 0x801978C0
    virtual int vf0C();
    unsigned char m08[0x10 - 0x08];
};

// inferred: control 0x6E of a character held by Em403WarLion (game/Em403WarLion.cpp); class name from
// the vtable stored by the constructor
class ControlMatHoldPre : public ControlBase {
public:
    ControlMatHoldPre(Chara_unk* chara, Chara_unk* holder); // 0x80197528
    virtual int vf0C();
    Chara_unk* m08; // inferred: holder
};

class ControlMatHoldRelease : public ControlBase {
public:
    ControlMatHoldRelease(Chara_unk* chara); // 0x801980B4
    virtual int vf0C();
};

class ControlMatHoldDamage : public ControlBase {
public:
    ControlMatHoldDamage(Chara_unk* chara); // 0x80198528
    virtual int vf0C();
};

class ControlThrown : public ControlBase {
public:
    ControlThrown(Chara_unk* chara, const atn::Vector3& dir, int a); // 0x80198C08
    virtual int vf0C();
    unsigned char m08[0x30 - 0x08];
};

// inferred: created by CharaTask 0x8016BEDC-0x8016CAD0; class names from the vtables stored by the
// constructors, sizes from the operator new calls (the vectors are passed by value)
class ControlStickByQuark : public ControlBase {
public:
    ControlStickByQuark(Chara_unk* chara, int a); // 0x80192908 (control id 0x53)
    virtual int vf0C();
    unsigned char m08[0x14 - 0x08];
};

class ControlThrust : public ControlBase {
public:
    ControlThrust(Chara_unk* chara, Chara_unk* other, atn::Vector3 v, int b); // 0x8019356C
    virtual int vf0C();
    unsigned char m08[0x28 - 0x08];
};

class ControlThrustForCrab : public ControlBase {
public:
    ControlThrustForCrab(Chara_unk* chara, Chara_unk* other); // 0x80193EF8 (reads other->mPos)
    virtual int vf0C();
    unsigned char m08[0x24 - 0x08];
};

class ControlThrustForKraken : public ControlBase {
public:
    ControlThrustForKraken(Chara_unk* chara, int a, atn::Vector3 v); // 0x80194550
    virtual int vf0C();
    unsigned char m08[0x44 - 0x08];
};

// inferred: constructor 0x80194E2C (0x38 bytes; the vtable 0x8077EB20 is stored first), called by
// EnemyQuarkStand (game/EnemyQuarkStand.cpp); `other` is the enemy, `kind` 1 or 2
class ControlThrustForQuarkStand : public ControlBase {
public:
    ControlThrustForQuarkStand(Chara_unk* chara, Chara_unk* other, int kind); // 0x80194E2C
    virtual int vf0C();
    unsigned char m08[0x38 - 0x08];
};

class ControlAbsorbed : public ControlBase {
public:
    ControlAbsorbed(Chara_unk* chara); // 0x80196974 (8 bytes)
    virtual int vf0C();
};

} // namespace chr

#endif
