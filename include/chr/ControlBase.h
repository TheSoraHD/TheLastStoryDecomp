#ifndef CHR_CONTROLBASE_H
#define CHR_CONTROLBASE_H

#include "math/Vec.h"
#include <boost/function.hpp>

struct Model_unk;

namespace chr {

// inferred: motion controller embedded in the character at +0xB0 (class name unknown)
class Motion_unk {
public:
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion
    float fn_80097D7C(int track); // inferred: motion length

    // inferred: entry of the array at m220 (0x2C bytes)
    struct Entry_unk {
        unsigned char m00[0x04];
        atn::Vector3 m04;
        unsigned char m10[0x2C - 0x10];
    };

    unsigned char m000[0x220];
    Entry_unk* m220;
    unsigned char m224[0x22C - 0x224];
    int m22C; // inferred: current motion id
    unsigned char m230[0x234 - 0x230];
    float m234;
    float m238;
    unsigned char m23C[0x24C - 0x23C];
    float m24C;
    unsigned char m250[0x34C - 0x250];
    int m34C;
};

// inferred: character controlled by the chr::Control* classes (class name unknown)
class Chara_unk {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34(void* target, int arg, float a, float b); // inferred: look at target
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();

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
    // inferred: names invented (weak copies 0x8000DD0C and 0x8012A1B8)
    Model_unk* GetModel_unk() { return (Model_unk*)&mMotion; }
    atn::Vector3* GetRot_unk() { return &mRot; }

    unsigned char m004[0x48 - 0x04];
    int m48;
    unsigned char m04C[0xB0 - 0x4C];
    Motion_unk mMotion; // 0xB0
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
    unsigned char m60C[0x7D8 - 0x60C];
    float m7D8;
    unsigned char m7DC[0x7E0 - 0x7DC];
    unsigned int m7E0; // inferred: flags tested with masks; 0x20 while held (0x801A2490)
    unsigned char m7E4[0x948 - 0x7E4];
    float m948;
    unsigned char m94C[0xC48 - 0x94C];
    int mC48;
    unsigned char mC4C[0xD18 - 0xC4C];
    int mD18;  // inferred: nonzero stops chr::ControlRide (0x801C6FE8)
    unsigned char mD1C[0xD38 - 0xD1C];
    unsigned short mHeld_unk : 1; // 0xD38: inferred: set while held by gmk::InteractionSpot
    unsigned short mD38 : 15;
    unsigned char mD3A[0xF1C - 0xD3A];
    int mF1C;  // inferred: cleared when chr::ControlRide ends
    unsigned char mF20[0xF50 - 0xF20];
    int mF50; // inferred: set by chr::ControlDraggedStart
    int mF54; // inferred: set by chr::ControlDraggingStart
    unsigned char mF58[0xF60 - 0xF58];
    atn::Vector3 mF60; // inferred: set to the position by chr::ControlAppearSkelton
    unsigned char mF6C[0x1208 - 0xF6C];
    int m1208;
    unsigned char m120C[0x12A4 - 0x120C];
    // 0x12A4: flag word (bitfields, MSB first)
    unsigned int mFlag31 : 1;
    unsigned int mFlag30_7 : 24;
    unsigned int mFlag6 : 1;
    unsigned int mFlag5 : 1;
    unsigned int mFlag4_0 : 5;
};

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector returned by ControlBase::vf4C

// inferred from __vt__Q23chr11ControlBase (0x8077DC90). vf44, vf48, vf4C and vf5C are inline
// (weak copies first emitted in chr/ControlBreakOpen.cpp at 0x80188060).
class ControlBase {
public:
    ControlBase(Chara_unk* chara) : mChara(chara) {}
    virtual ~ControlBase() {}
    virtual int vf0C() = 0; // inferred: update, returns nonzero when finished
    // inferred: returns the action to run when the control finishes (0x801B2A80 binds
    // CharaTask::fn_80178208 to the next control)
    virtual boost::function<void()> vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual int vf44() { return 0; }
    virtual int vf48() { return 0; }
    virtual atn::Vector3 vf4C() { return lbl_807C7030; }
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C() {}

    Chara_unk* mChara; // 0x04
};

} // namespace chr

#endif
