#ifndef CHR_CONTROLWALLUP_H
#define CHR_CONTROLWALLUP_H

#include "chr/ControlMoveCursor.h"

namespace chr {

// Controls of chr/ControlWallUp.cpp (class names from the vtables). The classes declare no
// destructor (implicit destructors 0x801CBFEC, 0x801CC02C, 0x801CC06C).

// inferred: climbs a wall (created by CharaTask::fn_801591E8); the cursor (m08/m14) is the jump target
class ControlWallUp : public ControlMoveCursor {
public:
    ControlWallUp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& dir); // 0x801C9F6C
    virtual int vf0C();                     // 0x801CA1E0
    virtual boost::function<void()> vf10(); // 0x801CAAF0
    virtual void vf1C();                    // 0x801CB2BC
    virtual void vf40();                    // 0x801CB24C

    atn::Vector3 m28; // inferred: position on the wall
    atn::Vector3 m34; // inferred: direction of the wall
    float m40;        // inferred: climbing time left
    float m44;        // inferred: hanging time left
    float m48;        // inferred: initial height of the cursor
    bool m4C;         // inferred: jump (chr::ControlWallSlash) when finished
    bool m4D;
    bool m4E;         // inferred: forced end
    CharaTask* m50;   // inferred: character giving the next action (CharaTask::vfB4)
};

// inferred: drops down from the wall (created inline by ControlWallUp::vf10)
class ControlWallDown : public ControlBase {
public:
    ControlWallDown(Chara_unk* chara, const atn::Vector3& dest) : ControlBase(chara), m14(dest) {
        chara->m560 = 0x10;
        m08 = mChara->mPos;
        Motion_unk* m = &mChara->mMotion;
        atn::Vector3 rot = mChara->mRot;
        rot.y = atn::WrapAngle_unk(3.1415927f + rot.y);
        mChara->mRot = rot;
        m->m34C = 1;
        m->mTracks[0].m20 = 1.0f;
        m->fn_80097C08(0, 0x16D, 0, 0, 1, 0.4f, 0.1f);
        m->mTracks[0].m0C = 1.0f;
    }
    virtual int vf0C();  // 0x801CB2D4
    virtual void vf1C(); // 0x801CB41C

    atn::Vector3 m08; // inferred: start
    atn::Vector3 m14; // inferred: destination
};

// inferred: jumps down from the wall onto a target (created by ControlWallUp::vf10)
class ControlWallSlash : public ControlBase {
public:
    ControlWallSlash(Chara_unk* chara, const atn::Vector3& target, const atn::Vector3* land); // 0x801CB434
    virtual int vf0C();                     // 0x801CB76C
    virtual boost::function<void()> vf10(); // 0x801CBB34
    virtual atn::Vector3 vf14();            // 0x801CBAD8
    virtual atn::Vector3 vf18();            // 0x801CBAAC
    virtual void vf1C();                    // 0x801CBAEC
    virtual int vf28();                     // 0x801CBAC0
    virtual int vf54() { return m30; }      // 0x801CBFE4

    atn::Vector3 m08; // inferred: current point of the jump
    atn::Vector3 m14; // inferred: target
    atn::Vector3 m20; // inferred: start
    float m2C;        // inferred: time left
    int m30;          // inferred: landed
    int m34;          // inferred: m38 is set
    atn::Vector3 m38; // inferred: landing position
};

} // namespace chr

#endif
