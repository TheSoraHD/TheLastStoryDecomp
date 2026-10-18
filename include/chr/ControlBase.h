#ifndef CHR_CONTROLBASE_H
#define CHR_CONTROLBASE_H

namespace chr {

// inferred: motion controller embedded in the character at +0xB0 (class name unknown)
class Motion_unk {
public:
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion

    unsigned char m000[0x234];
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

    void fn_801539E0();
    void fn_80155DAC(int arg);
    void fn_8016DA4C();

    unsigned char m004[0xB0 - 0x04];
    Motion_unk mMotion; // 0xB0
    unsigned char m400[0x534 - 0x400];
    unsigned char m534[0x560 - 0x534];
    int m560;
    unsigned char m564[0x58C - 0x564];
    int m58C;
    unsigned char m590[0x12A4 - 0x590];
    // 0x12A4: flag word (bitfields, MSB first)
    unsigned int mFlag31 : 1;
    unsigned int mFlag30_7 : 24;
    unsigned int mFlag6 : 1;
    unsigned int mFlag5 : 1;
    unsigned int mFlag4_0 : 5;
};

// inferred from __vt__Q23chr11ControlBase (0x8077DC90)
class ControlBase {
public:
    ControlBase(Chara_unk* chara, int arg) : mChara(chara), m08(arg) {}
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
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();

    Chara_unk* mChara; // 0x04
    int m08;
};

} // namespace chr

#endif
