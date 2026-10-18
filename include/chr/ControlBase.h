#ifndef CHR_CONTROLBASE_H
#define CHR_CONTROLBASE_H

#include "math/Vec.h"

namespace chr {

// inferred: motion controller embedded in the character at +0xB0 (class name unknown)
class Motion_unk {
public:
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion
    float fn_80097D7C(int track); // inferred: motion length

    unsigned char m000[0x22C];
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

    // inferred: bounding sphere returned by value
    struct Sphere_unk {
        Vec mCenter;
        float mRadius;
    };

    void fn_801539E0();
    Sphere_unk fn_80176548(const Vec& pos); // inferred
    void fn_80155DAC(int arg);
    void fn_8016DA4C();
    Vec fn_80178018(); // inferred: returns a direction or target vector (y used as an angle)
    float fn_80178078();

    unsigned char m004[0x48 - 0x04];
    int m48;
    unsigned char m04C[0xB0 - 0x4C];
    Motion_unk mMotion; // 0xB0
    unsigned char m400[0x528 - 0x400];
    Vec mPos;  // 0x528
    Vec mRot;  // 0x534
    unsigned char m540[0x560 - 0x540];
    int m560;
    unsigned char m564[0x570 - 0x564];
    float m570;
    Vec m574;
    float m580;
    float m584;
    unsigned char m588[0x58C - 0x588];
    int m58C;
    unsigned char m590[0x5A4 - 0x590];
    Vec m5A4;  // inferred: collision offset
    unsigned char m5B0[0x5B8 - 0x5B0];
    unsigned char m5B8[0x12A4 - 0x5B8];
    // 0x12A4: flag word (bitfields, MSB first)
    unsigned int mFlag31 : 1;
    unsigned int mFlag30_7 : 24;
    unsigned int mFlag6 : 1;
    unsigned int mFlag5 : 1;
    unsigned int mFlag4_0 : 5;
};

extern "C" Vec lbl_807C7030; // inferred: zero vector returned by ControlBase::vf4C

// inferred from __vt__Q23chr11ControlBase (0x8077DC90). vf44, vf48, vf4C and vf5C are inline
// (weak copies first emitted in chr/ControlBreakOpen.cpp at 0x80188060).
class ControlBase {
public:
    ControlBase(Chara_unk* chara) : mChara(chara) {}
    virtual ~ControlBase() {}
    virtual int vf0C() = 0; // inferred: update, returns nonzero when finished
    virtual void vf10();
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
    virtual Vec vf4C() { return lbl_807C7030; }
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C() {}

    Chara_unk* mChara; // 0x04
};

} // namespace chr

#endif
