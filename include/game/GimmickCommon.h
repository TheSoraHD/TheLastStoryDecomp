#ifndef GAME_GIMMICKCOMMON_H
#define GAME_GIMMICKCOMMON_H

#include "atn/LH_Binary.h"
#include "atn/Task.h"
#include "math/Vec.h"

class GimmickCommon;

// inferred: the gimmick manager singleton (lbl_8087F4A0); gimmicks register themselves
class GimmickManager {
public:
    void fn_803EE3F0(GimmickCommon* gimmick); // inferred: registers a gimmick
    void fn_803EE41C(GimmickCommon* gimmick); // inferred: unregisters it
};
extern "C" GimmickManager* lbl_8087F4A0;

// inferred: 0x34-byte member of GimmickCommon at 0xB0 (sound/effect tables: its destructor
// frees arrays of atn::SoundRef); constructor and destructor are out of line
struct GimmickCommonB0_unk {
    GimmickCommonB0_unk();  // 0x803EF608
    ~GimmickCommonB0_unk(); // 0x803EF64C
    unsigned char m00[0x34];
};

// inferred: handle released through the manager at lbl_8087EF10 when nonzero
extern "C" void* lbl_8087EF10;
extern "C" void fn_800897D8(void* manager, int handle);
struct GimmickHandle_unk {
    GimmickHandle_unk() : mHandle(0) {}
    ~GimmickHandle_unk() {
        if (mHandle != 0 && lbl_8087EF10 != 0) {
            fn_800897D8(lbl_8087EF10, mHandle);
        }
    }
    int mHandle;
};

// Base of the gimmicks (gmk::*). The name is in the DOL (vtable 0x8078C9D0, RTTI bases
// atn::TaskBase, atn::Task). Virtual methods are named by vtable offset.
class GimmickCommon : public atn::Task {
public:
    GimmickCommon(atn::Task* parent, int id, int arg); // 0x803EC568
    virtual ~GimmickCommon();                          // 0x803EC69C
    virtual int vf0C();   // 0x803EDF74
    virtual void vf10();  // 0x803EDF70
    virtual void vf14();  // 0x803EDF6C
    virtual void vf1C();  // 0x803ECB7C
    virtual void vf20();  // 0x803ECBCC
    virtual void vf24();  // 0x803ECBD0
    virtual void vf28();  // 0x803EDF5C
    virtual void vf2C();  // 0x803ECB84
    virtual void vf30();  // 0x803EDF40
    virtual void vf34();  // 0x803EDF24
    // Trivial inline virtuals: their weak copies were kept in the first object that used them
    // (only gimmick vtables point to them), so they live far from this file.
    virtual int vf38() { return 0; }       // 0x80012A14
    virtual void vf3C();                   // 0x803EDF20
    virtual int vf40() { return 0; }       // 0x80012714
    virtual int vf44() { return 0; }       // 0x800FA4F4
    virtual void vf48();                   // 0x803EDE34
    virtual void vf4C();                   // 0x803EDF1C
    virtual int vf50() { return 0; }       // 0x802AEABC
    // NONMATCHING (standalone): psq_st and stfs of the copy are in the other order
    virtual atn::Vector3 vf54() { return m6C; } // 0x800EB488: inferred: position
    virtual float vf58() { return 0.0f; }  // 0x800FA4EC
    virtual int vf5C() { return 0; }       // 0x80017944
    virtual int vf60() { return 0; }       // 0x801B7794
    virtual int vf64() { return 0; }       // 0x803A04B8
    virtual void vf68() {}                 // 0x800E2FDC
    virtual void vf6C();  // 0x803ED92C
    virtual void vf70();  // 0x803EDAF0

    int mId;           // 0x48: inferred: ids below 1000 are scaled by 1000
    int m4C;
    int m50;           // inferred: mId / 1000
    int m54;
    int m58;
    int m5C;
    atn::LH_Binary mBinary; // 0x60
    int m68;
    atn::Vector3 m6C;  // inferred: position (vf54 returns it)
    atn::Vector3 m78;
    atn::Vector3 m84;
    atn::Vector3 m90;  // inferred: scale (1, 1, 1)
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;
    GimmickCommonB0_unk mB0;
    int mE4;
    int mE8;
    int mEC;
    GimmickHandle_unk mF0;
};

#endif
