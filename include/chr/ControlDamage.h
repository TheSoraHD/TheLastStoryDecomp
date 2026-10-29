#ifndef CHR_CONTROLDAMAGE_H
#define CHR_CONTROLDAMAGE_H

#include "chr/ControlBase.h"

namespace chr {

// Controls of chr/ControlDamage.cpp (0x801AC434-0x801B0710). The classes are declared in the order
// of their implicit destructors at the end of the file (0x801B0438-0x801B06D0).

// inferred: hit reaction (control id 0x16)
class ControlDamage : public ControlBase {
public:
    ControlDamage(Chara_unk* chara, const atn::Vector3& dir, int motion); // 0x801AC434
    virtual int vf0C();

    atn::Vector3 m08; // inferred: rotation to turn to
    float m14;        // inferred: time left
};

// inferred: guard against a hit (control id 0x14)
class ControlGuard : public ControlBase {
public:
    ControlGuard(Chara_unk* chara, float f, const atn::Vector3& dir, int a); // 0x801ACB38
    virtual int vf0C();

    atn::Vector3 m08; // inferred: rotation to turn to
    int m14;
    float m18;        // inferred: time left
};

// inferred: dodge (control id 0x40)
class ControlAvoid : public ControlBase {
public:
    ControlAvoid(Chara_unk* chara, const atn::Vector3& dir); // 0x801ACF4C
    virtual int vf0C();

    atn::Vector3 m08; // inferred: rotation to turn to
};

// inferred: knocked down (control id 0x17); the parameters of the constructor are inferred from
// chr::ControlCaught::vf10
class ControlKnockdown : public ControlBase {
public:
    ControlKnockdown(Chara_unk* chara, const atn::Vector3& dir, int motion, int arg4); // 0x801AD34C
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    bool fn_801AE41C(); // inferred: can be cancelled (CharaTask::fn_8015EB2C, control id 0x17)

    atn::Vector3 m08;                // inferred: rotation to turn to
    int m14;                         // inferred: frame counter of the step
    int m18;                         // inferred: step (0 fall, 1 bounce, 2 in the air, 3 landed)
    float m1C;                       // inferred: frame after which the push stops
    int m20;                         // inferred: nonzero: gimmicks hit by the fall are told
    boost::function<void()> m24;     // inferred: action run at the end (vf10)
    int m38;                         // inferred: nonzero: the bounce is checked
    int m3C;                         // inferred: nonzero: the dust effect is pending
};

// inferred: getting up after a knockdown (control id 0x1B)
class ControlGetUp : public ControlBase {
public:
    ControlGetUp(Chara_unk* chara); // 0x801AE77C (8 bytes: no members of its own)
    virtual int vf0C();
};

// inferred: revival (control id 0x1C); RevivalSkelton plays the collapse of the skeletons backwards
class ControlRevival : public ControlBase {
public:
    ControlRevival(Chara_unk* chara, int time); // 0x801AE978
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    virtual void vf40();

    int m08; // inferred: frames left
};

class ControlRevivalSkelton : public ControlBase {
public:
    ControlRevivalSkelton(Chara_unk* chara, int a); // 0x801AEF7C
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();

    int m08;
};

// inferred: fall to death (control id 0x3B)
class ControlFallDead : public ControlBase {
public:
    ControlFallDead(Chara_unk* chara, const atn::Vector3& rot); // 0x801AF264 (0x28 bytes)
    virtual int vf0C();
    bool fn_801AF81C(); // inferred: can be cancelled (control id 0x3B)

    int m08;          // inferred: step (0 falling, 1 landing, 2 dying)
    int m0C;          // inferred: frames of the landing step
    atn::Vector3 m10; // inferred: direction of the fall
    atn::Vector3 m1C; // inferred: previous position (of the collision capsule)
};

// inferred: guard broken by a hit (control id 0x15, 0x16 with a)
class ControlGuarded : public ControlBase {
public:
    ControlGuarded(Chara_unk* chara, int a); // 0x801AF888
    virtual int vf0C();

    int m08;
};

// inferred: reaction control created by EnemyTask::vf6C (kinds 9-12, control id 0x8E); it was
// declared as EnemyReaction_unk in game/EnemyTask.cpp and game/EnemyTasha.cpp
class ControlDamageLastOne : public ControlBase {
public:
    ControlDamageLastOne(Chara_unk* chara, const atn::Vector3* dir, int kind); // 0x801AFAC0
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    bool fn_801B0260(); // inferred: can be cancelled (control id 0x8E)

    atn::Vector3 m08; // inferred: push of kind 11
    atn::Vector3 m14; // inferred: rotation to turn to
    int m20;          // inferred: kind
};

} // namespace chr

#endif
