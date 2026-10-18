#ifndef GAME_CHARATASK_H
#define GAME_CHARATASK_H

#include "atn/Task.h"
#include "chr/Motion_unk.h"
#include "game/CharaStatus.h"
#include "math/Vec.h"

struct Model_unk;

namespace chr {
class ControlBase;
}

// inferred: hit/damage description passed to CharaTask::vf3C and vf40 (0x801513D0);
// 0x10 holds a vector (knockback?) and 0x68/0x90 values that FriendArganan clears.
struct HitInfo_unk;

// inferred from __vt__9CharaTask (0x8077C460). Base class of all characters
// (NpcTask, BattleCharaTask, ...). Virtual methods are named by vtable offset; most of their
// parameters are unknown.
class CharaTask : public atn::Task {
public:
    CharaTask(atn::Task* parent, int type, void* data); // 0x801354B4
    virtual ~CharaTask();                               // 0x80136138
    virtual int vf0C();                                 // 0x8017CB74
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34(void* target, int arg, float a, float b); // inferred: look at target
    virtual void vf38();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84(int motion, float blend); // inferred: bound by chr::ControlAppear::vf10
    virtual void vf88();
    virtual void vf8C(const atn::Vector3& v); // inferred: bound by chr::ControlAppear::vf10
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual void vfA4();
    virtual void vfA8();
    virtual atn::Vector3 vfAC(); // inferred: returns a position (CharaStatus::fn_8012E540)
    virtual void vfB0();
    virtual void vfB4();
    virtual void vfB8();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual void vfD0();
    virtual void vfD4();
    virtual void vfD8();
    virtual void vfDC();
    virtual void vfE0();

    int fn_8013655C();
    void fn_80178208(chr::ControlBase* control); // inferred: changes the current control
    void fn_8016EB48(int a, int b, int c); // inferred: bound with three bools (0x801C7390)
    void fn_8015E4B0(const atn::Vector3& dir, int arg); // inferred: knock back along `dir` (0x801B59E0)
    void fn_8015E7A0(const atn::Vector3& dir, int arg, int arg2);
    void fn_80139560();
    void fn_80145334();
    void fn_80149A30();

    // inferred: bounding sphere returned by value
    struct Sphere_unk {
        atn::Vector3 mCenter;
        float mRadius;
    };

    void fn_801539E0();
    Sphere_unk fn_80176548(const atn::Vector3& pos); // inferred
    void fn_80155DAC(int arg);
    void fn_8016DA4C();
    void fn_801446F0();
    atn::Vector3 fn_80178018(); // inferred: returns a direction or target vector (y used as an angle)
    float fn_80178078();
    void fn_8017AC3C(int arg);
    void fn_80164DCC(int arg);
    void fn_8016E4C4(int arg);
    int fn_80179D44();
    void fn_801750FC();
    void fn_80154344();
    void fn_8015495C(int a, int b, int c, int d);
    void fn_8013CB68(void* target, int arg, float a, float b); // inferred: same arguments as vf34
    int fn_8016E484();
    void fn_8017AC2C();
    void fn_80166544();
    int fn_801789D8(int a, unsigned int flags);
    void fn_80167038();
    void fn_80167640();
    // inferred: state checks inlined in many places (names invented)
    bool IsActive_unk() { return (mFlags & 4) != 4 && (mFlags & 1) != 1; }
    bool IsFree_unk() { return IsActive_unk() && (mStatus.m0C & 0x20) != 0x20; }
    bool IsGrappled_unk() { return m55C == 6 && m560 == 28; }
    bool CanAct_unk() { return IsFree_unk() && !IsGrappled_unk(); }
    // inferred: names invented (weak copies 0x8000DD0C and 0x8012A1B8)
    Model_unk* GetModel_unk() { return (Model_unk*)&mMotion; }
    atn::Vector3* GetRot_unk() { return &mRot; }


    int mType;  // 0x48: also compared with 0 and 2 by the chr::Control* classes
    int m4C;    // 0x4C: same value as mType
    int m50;    // 0x50: inferred: third constructor argument (compared with 1 there and by CharaStatus)
    unsigned char m54[0x5C - 0x54];
    chr::CharaData_unk* m5C; // inferred: character data (read by CharaStatus)
    unsigned char m060[0x80 - 0x60];
    int mId;    // 0x80: inferred: unique id given by the character manager
    unsigned char m84[0xB0 - 0x84];
    chr::Motion_unk mMotion; // 0xB0
    unsigned char m400[0x524 - 0x400];
    int m524; // inferred: index into Motion_unk::m220
    atn::Vector3 mPos;  // 0x528
    atn::Vector3 mRot;  // 0x534
    unsigned char m540[0x55C - 0x540];
    int m55C; // inferred: state compared with 6 by the grapple controls
    int m560;
    int m564; // inferred: set to 1 or 2 by gmk::InteractionSpot
    unsigned char m568[0x570 - 0x568];
    float m570;
    atn::Vector3 m574;
    float m580;
    float m584;
    unsigned char m588[0x58C - 0x588];
    int m58C;
    unsigned char m590[0x5A4 - 0x590];
    atn::Vector3 m5A4;  // inferred: collision offset
    float m5B0; // inferred: collision radius (0x80188280)
    unsigned char m5B4[0x5B8 - 0x5B4];
    unsigned char m5B8[0x5C0 - 0x5B8]; // inferred: passed by address (chr::ControlTurnQuadruped)
    unsigned int m5C0; // inferred: flags; bit 0 is cleared while appearing (chr::ControlAppear)
    unsigned char m5C4[0x600 - 0x5C4];
    atn::Vector3 m600;
    unsigned char m60C[0x648 - 0x60C];
    chr::CharaPart_unk* m648; // inferred: parts whose parameters are added (CharaStatus::fn_8012B988)
    chr::CharaPart_unk* m64C;
    unsigned int m650;      // inferred: number of entries in m654
    chr::CharaPart_unk* m654[1]; // inferred: array of m650 entries (capacity unknown)
    unsigned char m658[0x680 - 0x658];
    chr::CharaEquip_unk* m680[5]; // inferred: equipment (CharaStatus::fn_8012B988)
    unsigned char m694[0x7D4 - 0x694];
    CharaStatus mStatus; // 0x7D4
    unsigned int mC10; // inferred: flags or-ed into CharaStatus::mParamA.m9C (fn_8012B3E8)
    unsigned char mC14[0xC48 - 0xC14];
    int mC48;
    unsigned char mC4C[0xD18 - 0xC4C];
    int mD18;  // inferred: nonzero stops chr::ControlRide (0x801C6FE8)
    CharaTask* mD1C; // inferred: another character (its status is read by CharaStatus::fn_8012DB04)
    unsigned char mD20[0xD38 - 0xD20];
    unsigned short mHeld_unk : 1; // 0xD38: inferred: set while held by gmk::InteractionSpot
    unsigned short mD38 : 15;
    unsigned char mD3A[0xF1C - 0xD3A];
    int mF1C;  // inferred: cleared when chr::ControlRide ends
    unsigned char mF20[0xF50 - 0xF20];
    int mF50; // inferred: set by chr::ControlDraggedStart
    int mF54; // inferred: set by chr::ControlDraggingStart
    unsigned char mF58[0xF60 - 0xF58];
    atn::Vector3 mF60; // inferred: set to the position by chr::ControlAppearSkelton
    unsigned char mF6C[0x1030 - 0xF6C];
    RouteWalker_unk mRoute; // 0x1030
    CharaLook_unk mLook;    // 0x10D8
    unsigned char m1154[0x1208 - 0x1154];
    int m1208;
    unsigned char m120C[0x12A4 - 0x120C];
    // 0x12A4: flag word (bitfields, MSB first)
    unsigned int mFlag31 : 1;
    unsigned int mFlag30_29 : 2;
    unsigned int mFlag28 : 1; // inferred: tested by CharaStatus::fn_8012F3D8
    unsigned int mFlag27_23 : 5;
    unsigned int mFlag22 : 1; // inferred: tested by CharaStatus::fn_8012B3E8
    unsigned int mFlag21_7 : 15;
    unsigned int mFlag6 : 1;
    unsigned int mFlag5 : 1;
    unsigned int mFlag4_3 : 2;
    unsigned int mFlag2 : 1; // inferred: tested by CharaStatus::fn_8012E244
    unsigned int mFlag1_0 : 2;
    unsigned char m12A8[0x12D0 - 0x12A8];
    // inferred: hits on the character (CharaStatus::fn_8012E540); the capacity is unknown
    FixedArray_unk<CharaHit_unk, 1> m12D0;
    unsigned char m12E8[0x1424 - 0x12E8];
};

#endif
