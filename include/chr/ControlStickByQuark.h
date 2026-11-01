#ifndef CHR_CONTROLSTICKBYQUARK_H
#define CHR_CONTROLSTICKBYQUARK_H

#include "chr/ControlBase.h"
#include "chr/ControlCdn.h"
#include "atn/SoundRef.h"

class EnemyQuarkStand;
class SwordAttack_unk;

namespace chr {

// inferred: controls of chr/ControlStickByQuark.cpp, declared in .dol order (the implicit destructors
// are emitted in this order at the end of the file, the vtables in reverse order). Most are created by
// CharaTask (0x8015BAF0-0x8015D5B0); sizes from the operator new calls, argument types from the
// constructors. ControlStickSword, ControlStickSwordSp, ControlMatHoldGuard and ControlThrownSimple
// are never constructed in the DOL (their vtables have no references).

// inferred: the character stuck on the sword of a quark (control id 0x53)
class ControlStickByQuark : public ControlBase {
public:
    ControlStickByQuark(Chara_unk* chara, int quark); // 0x80192908
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    void fn_80192FB4(); // inferred: stuck on the right arm (called by EnemyQuarkStand)

    EnemyQuarkStand* mQuark; // 0x08
    bool mDone;              // 0x0C: inferred: the player shook the remote enough
    bool mStuck;             // 0x0D
    bool mReleased;          // 0x0E
    int mCount;              // 0x10: inferred: swings left
};

class ControlThrust : public ControlBase {
public:
    ControlThrust(Chara_unk* chara, Chara_unk* other, atn::Vector3 offset, int b); // 0x8019356C
    virtual int vf0C();
    // inferred: the player may thrust again (name invented)
    bool CanRepeat_unk() const { return m1C && m20 < 2 && mOther; }
    atn::Vector3 mStep; // 0x08: inferred: move per frame
    float m14;          // inferred: turn per frame
    Chara_unk* mOther;  // 0x18
    int m1C;
    int m20;
    int m24;
};

class ControlThrustForCrab : public ControlBase {
public:
    ControlThrustForCrab(Chara_unk* chara, Chara_unk* crab); // 0x80193EF8
    virtual int vf0C();
    atn::Vector3 mStep; // 0x08: inferred: move per frame forward
    atn::Vector3 mBack; // 0x14: inferred: move per frame back
    int m20;
};

class ControlThrustForKraken : public ControlBase {
public:
    ControlThrustForKraken(Chara_unk* chara, int kraken, atn::Vector3 target); // 0x80194550
    virtual int vf0C();
    virtual atn::Vector3 vf14();
    atn::Vector3 mTarget; // 0x08
    atn::Vector3 mFrom;   // 0x14: inferred: the kraken
    atn::Vector3 mDest;   // 0x20
    atn::Vector3 mDir;    // 0x2C
    atn::Vector3 mDir2;   // 0x38
};

// inferred: constructor 0x80194E2C (0x38 bytes; the vtable 0x8077EB20 is stored first), called by
// EnemyQuarkStand (game/EnemyQuarkStand.cpp); `other` is the enemy, `kind` 1 or 2
class ControlThrustForQuarkStand : public ControlBase {
public:
    ControlThrustForQuarkStand(Chara_unk* chara, Chara_unk* other, int kind); // 0x80194E2C
    virtual int vf0C();
    virtual void vf1C();
    virtual int vf54() { return m28; }
    Chara_unk* mQuark;    // 0x08
    int mKind;            // 0x0C
    atn::Vector3 mTarget; // 0x10
    atn::Vector3 mStart;  // 0x1C
    int m28;
    atn::Vector3 mFront;  // 0x2C
};

// inferred: thrust on a ghoul (vtable 0x8077EAA8), started by Em309GhoulSimple::vfB4; `name` is the node
// ("neck") the character aims at
class ControlThrustForGhoul : public ControlBase {
public:
    ControlThrustForGhoul(Chara_unk* chara, Chara_unk* other, const char* name); // 0x801957F0
    virtual int vf0C();
    virtual atn::Vector3 vf14();
    virtual void vf1C();
    virtual void vf40();
    virtual int vf54() { return m38; }
    Chara_unk* mOther;    // 0x08
    int m0C;
    atn::Vector3 m10;     // inferred: position returned by vf14
    atn::Vector3 mTarget; // 0x1C
    atn::Vector3 mStart;  // 0x28
    float m34;
    bool m38;
    bool m39;
};

class ControlThrustForCocoon : public ControlBase {
public:
    ControlThrustForCocoon(Chara_unk* chara, Chara_unk* cocoon, int a); // 0x80196428
    virtual int vf0C();
    atn::Vector3 mTarget; // 0x08: inferred: the cocoon
    atn::Vector3 mStart;  // 0x14
    float mYaw;           // 0x20
    Chara_unk* mCocoon;   // 0x24
    int m28;
    bool m2C;
};

class ControlAbsorbed : public ControlBase {
public:
    ControlAbsorbed(Chara_unk* chara); // 0x80196974
    virtual int vf0C();
};

class ControlStickSword : public ControlBase {
public:
    virtual int vf0C();
    Chara_unk* m08; // inferred: the other character
};

// inferred: control id 0x43; fn_80196C68 lies between the vf0C of ControlStickSwordSp (0x80196A98)
// and the one of ControlMatHoldGuard (0x80196CD4)
class ControlStickSwordSp : public ControlBase {
public:
    virtual int vf0C();
    bool fn_80196C68() const; // inferred: can be cancelled
    unsigned char m08[0x14 - 0x08];
    ::SwordAttack_unk* mAttack; // 0x14
    int mState;                     // 0x18
};

class ControlMatHoldGuard : public ControlBase {
public:
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
};

// inferred: control 0x6E of a character held by Em403WarLion (game/Em403WarLion.cpp)
class ControlMatHoldPre : public ControlBase {
public:
    ControlMatHoldPre(Chara_unk* chara, Chara_unk* holder); // 0x80197528
    virtual int vf0C();
    Chara_unk* m08; // inferred: holder
};

class ControlMatHold : public ControlBase {
public:
    ControlMatHold(Chara_unk* chara, int a); // 0x801978C0
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    unsigned char m08[0x10 - 0x08];
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
    virtual atn::Vector3 vf14();
    unsigned char m08[0x30 - 0x08];
};

class ControlThrownAway : public ControlBase {
public:
    ControlThrownAway(Chara_unk* chara, int a, int b); // 0x80199348
    virtual ~ControlThrownAway();
    virtual int vf0C();
    virtual atn::Vector3 vf14();
    unsigned char m08[0x10 - 0x08];
};

class ControlThrownAvoid : public ControlBase {
public:
    ControlThrownAvoid(Chara_unk* chara, int a, const atn::Vector3& dir); // 0x80199788
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual atn::Vector3 vf14();
    unsigned char m08[0x18 - 0x08];
};

// inferred: controls of the skeletons grabbed by Em002_02SkeltonKing
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
    virtual boost::function<void()> vf10();
    unsigned char m08[0x24 - 0x08];
};

class ControlThrownBreakSkelton : public ControlBase {
public:
    ControlThrownBreakSkelton(Chara_unk* chara); // 0x8019AFE8
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    unsigned char m08[0x20 - 0x08];
};

class ControlThreadCatch : public ControlBase {
public:
    ControlThreadCatch(Chara_unk* chara, int a, int b); // 0x8019B29C
    virtual ~ControlThreadCatch();
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual void vf1C();
    int m08;
    CharaTask* mOther;    // 0x0C
    atn::Vector3 mDir;    // 0x10
    int m1C;
    atn::SoundRef mSound; // 0x20
    int m24;
};

class ControlThreadDead : public ControlBase {
public:
    ControlThreadDead(Chara_unk* chara, int a); // 0x8019BD48
    virtual int vf0C();
    virtual void vf1C();
    int m08;
};

class ControlThreadRelease : public ControlBase {
public:
    ControlThreadRelease(Chara_unk* chara); // 0x8019BE9C
    virtual int vf0C();
    int mDone; // 0x08: inferred: already down
};

// inferred: created by CharaTask::fn_80164168 (the caller passes 15 as the last argument)
class ControlBoundShade : public ControlBase {
public:
    ControlBoundShade(Chara_unk* chara, int a, int b, int c); // 0x8019BFF0
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    unsigned char m08[0x1C - 0x08];
};

class ControlThrownSimple : public ControlBase {
public:
    virtual int vf0C();
    virtual atn::Vector3 vf14();
};

class ControlAbsorbedHP : public ControlBase {
public:
    ControlAbsorbedHP(Chara_unk* chara, int a, int b); // 0x8019DA50
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    unsigned char m08[0x24 - 0x08];
};

class ControlAbsorbedHPReleased : public ControlBase {
public:
    ControlAbsorbedHPReleased(Chara_unk* chara);
    virtual int vf0C();
};

class ControlCliffhangZanGurg : public ControlBase {
public:
    ControlCliffhangZanGurg(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b, atn::Vector3 c); // 0x8019E4F4
    virtual int vf0C();
    virtual atn::Vector3 vf14();
    virtual int vf30() { return m0C; }
    unsigned char m08[0x0C - 0x08];
    int m0C;
    unsigned char m10[0x40 - 0x10];
};

class ControlEndCliffhangZanGurg : public ControlBase {
public:
    ControlEndCliffhangZanGurg(Chara_unk* chara, atn::Vector3 a); // 0x8019EB7C
    virtual int vf0C();
    virtual atn::Vector3 vf14();
    unsigned char m08[0x20 - 0x08];
};

class ControlStickZanGurgStand : public ControlBase {
public:
    ControlStickZanGurgStand(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b); // 0x8019F0A8
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual atn::Vector3 vf14();
    unsigned char m08[0x20 - 0x08];
};

class ControlStickZanGurgCliff : public ControlBase {
public:
    ControlStickZanGurgCliff(Chara_unk* chara, atn::Vector3 a, atn::Vector3 b); // 0x8019F8E8
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual atn::Vector3 vf14();
    unsigned char m08[0x20 - 0x08];
};

class ControlReflectLaser : public ControlBase {
public:
    virtual ~ControlReflectLaser();
    virtual int vf0C();
};

} // namespace chr

#endif
