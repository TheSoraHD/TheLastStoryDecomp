#ifndef GAME_HOMINGBULLET_H
#define GAME_HOMINGBULLET_H

#include "game/Bullet.h"

// inferred: homing parameters (0x24 bytes at HomingBullet::mA0, passed to fn_800F8574). Names
// invented.
struct HomingParam_unk {
    CharaTask* mTarget; // 0x00
    float mAccel;       // 0x04
    float m08;
    float mMaxSpeed;    // 0x0C: inferred: added to the speed bonus of the data
    float mTurn;        // 0x10: inferred: largest turn angle per frame (radians)
    float mTime;        // 0x14: inferred: homing time
    float mDelay;       // 0x18: inferred: time flying straight before homing
    unsigned int mNode; // 0x1C: inferred: hash of the target node
    int mCount;         // 0x20
};

// inferred: from RTTI "HomingBullet" (vtable at 0x8078F2B8, base Bullet)
class HomingBullet : public Bullet {
public:
    HomingBullet();                    // 0x80441590
    virtual void vf0C(int a, int b);   // 0x8044187C
    virtual void vf10();               // 0x80441918
    virtual void vf14(const Mtx m);    // 0x80441E54
    virtual void vf18();               // 0x80441E88
    virtual int vf1C(CharaTask* chara, const atn::Vector3* pos, void* arg); // 0x80441F24 (not decompiled)
    virtual void vf20(CharaTask* chara, const atn::Vector3* pos, atn::Vector3* dir); // 0x80442A9C
    virtual CharaTask* vf24();         // 0x80442B48
    virtual void vf28(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* owner,
                      HomingParam_unk* param, int arg); // 0x80441610
    bool IsHoming_unk() const { return mA0.mTime > 0.0f; } // inferred: name invented
    void fn_80441E8C(const atn::Vector3* dir, CharaTask* owner, CharaTask* target); // inferred: reflected

    HomingParam_unk mA0;
    float mC4; // inferred: current turn angle
};

#endif
