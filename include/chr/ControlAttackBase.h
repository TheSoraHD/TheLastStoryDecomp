#ifndef CHR_CONTROLATTACKBASE_H
#define CHR_CONTROLATTACKBASE_H

#include "chr/ControlBase.h"
#include "game/CharaStatus.h"

struct HitInfo_unk;
class HomingBullet;

namespace chr {

// inferred: attack windows of a motion (returned by fn_802097C4 for the motion path); the frames
// mStart[i]..mEnd[i] are the hit windows. Names invented.
struct AttackTable_unk {
    int m00;
    int mCount;        // 0x04
    float mStart[4];   // 0x08
    float mEnd[4];     // 0x18
    int m28[4];        // 0x28: returned by vf34
    int m38[4];        // 0x38: returned by vf38(0)
    int m48[4];        // 0x48: returned by vf38(1)
};

// inferred from __vt__Q23chr17ControlAttackBase (0x8077D578): base of the melee attack controls
// (vf20..vf38 answer the hit windows of the current attack motion; vf60 is the motion speed).
class ControlAttackBase : public ControlBase {
public:
    ControlAttackBase(Chara_unk* chara, const atn::Vector3& dir)
        : ControlBase(chara), mTable(0), mHit(0), mDir(dir) {}
    virtual ~ControlAttackBase() {}
    virtual atn::Vector3 vf18() { return mDir; }
    virtual float vf20();
    virtual int vf24();
    virtual int vf28();
    virtual int vf2C();
    virtual int vf30();
    virtual int vf34();
    virtual int vf38(int arg);
    virtual float vf60();
    // inferred: attack windows of the current motion (inlined in the constructors); name invented
    inline void LoadTable_unk();

    AttackTable_unk* mTable; // 0x08
    int mHit;                // 0x0C: inferred: set once the hit window is reached
    atn::Vector3 mDir;       // 0x10
};

class ControlAttack : public ControlAttackBase {
public:
    ControlAttack(Chara_unk* chara, const atn::Vector3& dir); // 0x80189CB4
    virtual int vf0C();
};

class ControlAttackNext : public ControlAttackBase {
public:
    ControlAttackNext(Chara_unk* chara, const atn::Vector3& dir); // 0x8018A8CC
    virtual int vf0C();
};

// inferred: waiting pose before an attack (motion 0x173 then 0x174)
class ControlAttackSetup : public ControlBase {
public:
    ControlAttackSetup(Chara_unk* chara, int wait); // 0x8018B48C
    virtual int vf0C();

    float mWait; // 0x08
};

class ControlDashAttack : public ControlAttackBase {
public:
    ControlDashAttack(Chara_unk* chara, const atn::Vector3& pos); // 0x8018B6A0
    virtual int vf0C();

    atn::Vector3 mTarget; // 0x1C
    atn::Vector3 mStart;  // 0x28
};

class ControlJumpAttack : public ControlAttackBase {
public:
    ControlJumpAttack(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& offset); // 0x8018BB7C
    virtual int vf0C();
    virtual atn::Vector3 vf14();

    atn::Vector3 mTarget; // 0x1C
    atn::Vector3 mStart;  // 0x28
    atn::Vector3 mOffset; // 0x34: inferred: moved by mOffset / 9 per frame at the start
};

class ControlJumpAttack2 : public ControlAttackBase {
public:
    ControlJumpAttack2(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& offset); // 0x8018C0EC
    virtual int vf0C();
    virtual atn::Vector3 vf14();

    atn::Vector3 mTarget; // 0x1C
    atn::Vector3 mStart;  // 0x28
    atn::Vector3 mOffset; // 0x34: inferred: moved by mOffset / 9 per frame at the start
};

class ControlMultiAttack : public ControlAttackBase {
public:
    ControlMultiAttack(Chara_unk* chara, const atn::Vector3& dir); // 0x8018C6B8
    virtual int vf0C();
    virtual void vf1C();
};

class ControlSideWallAttack : public ControlAttackBase {
public:
    ControlSideWallAttack(Chara_unk* chara, const atn::Vector3& dir); // 0x8018CAEC
    virtual int vf0C();
};

class ControlGuardCounter : public ControlAttackBase {
public:
    ControlGuardCounter(Chara_unk* chara, const atn::Vector3& dir); // 0x8018CEF8
    virtual int vf0C();
    virtual void vf1C();

    int mCounter; // 0x1C: inferred: set by em005_doppel_ganger (the counter keeps going)
};

class ControlHitBackBullet : public ControlAttackBase {
public:
    ControlHitBackBullet(Chara_unk* chara, const atn::Vector3& dir, HomingBullet* bullet); // 0x8018D4AC
    virtual int vf0C();
    virtual void vf1C();

    FixedArray_unk<HomingBullet*, 8> mBullets; // 0x1C: inferred: bullets sent back
};

class ControlSwapAttack : public ControlAttackBase {
public:
    ControlSwapAttack(Chara_unk* chara, Chara_unk* target); // 0x8018DAE4
    virtual int vf0C();
    virtual void vf1C();

    Chara_unk* mTarget;      // 0x1C
    atn::Vector3 mStartPos;  // 0x20
    atn::Vector3 mStartRot;  // 0x2C
    float mTime;             // 0x38
};

class ControlAttackLastOne : public ControlAttackBase {
public:
    ControlAttackLastOne(Chara_unk* chara, const atn::Vector3& dir, Chara_unk* target, int type,
                         HitInfo_unk* skill); // 0x8018E438
    virtual int vf0C();
    virtual void vf1C();

    int mBulletId;         // 0x1C: inferred: id from fn_800F8548 (new hit)
    int mWindow;           // 0x20: inferred: last hit window
    bool mSlow;            // 0x24: inferred: slow motion started (frame 23)
    bool mSlowEnd;         // 0x25: inferred: slow motion ended (frame 28)
    int mHitCount;         // 0x28
    int mType;             // 0x2C: inferred: 9-12, selects the motion
    HitInfo_unk* mSkill;   // 0x30: inferred: set as the current attack when a window ends
    Chara_unk* mTarget;    // 0x34
};

} // namespace chr

#endif
