#ifndef GAME_GIMMICKCOMMON_H
#define GAME_GIMMICKCOMMON_H

#include "atn/Csv.h"
#include "atn/GXDecl.h"
#include "atn/LH_Binary.h"
#include "atn/Model_unk.h"
#include "atn/Task.h"
#include "game/GimmickManager.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

class GimmickCommon;


// inferred: 0x34-byte member of GimmickCommon at 0xB0 (sound/effect tables: its destructor
// frees arrays of atn::SoundRef); its members are out of line (game/GimmickManager.cpp)
struct GimmickCommonB0_unk {
    GimmickCommonB0_unk();  // 0x803EF608
    ~GimmickCommonB0_unk(); // 0x803EF64C
    void fn_803EF780(void* data, int size); // inferred: reads the gimmick binary
    int fn_803F06B0();                      // inferred: nonzero while busy
    void fn_803F0764(GimmickCommon* gimmick);
    void fn_803F10AC(int arg);
    void fn_803F11F8(void* menu);           // inferred: debug menu
    unsigned char m00[0x34];
};

// inferred: state command task created for some gimmick types (GimmickStateCommand.cpp)
class GimmickStateCommand : public atn::Task {
public:
    void fn_803F1A38(int arg);
    unsigned char m48[0x58 - 0x48];
    int m58;
};
extern "C" GimmickStateCommand* fn_803F1310(GimmickCommon* gimmick);

// inferred: description of a placed gimmick (passed to the second factory, 0x803EC374)
struct GimmickDesc_unk {
    unsigned char m00[0x04];
    atn::Vector3 mPos; // 0x04
    unsigned char m10[0x18 - 0x10];
    int mId;           // 0x18
    int mArg;          // 0x1C
};

// inferred: UV scroll entries used by GimmickCommon::fn_803EDB18
struct GimmickUvEntry_unk {
    const char* mName;
    float m04;
    float m08;
    float m0C;
};
struct GimmickUvTable_unk {
    unsigned char m00[0x28];
    int mCount;                  // 0x28
    GimmickUvEntry_unk* mEntries; // 0x2C
};
// inferred: owner of the UV animation (atn::Task-like: fn_800D59B8, loader at +0x20)
struct GimmickUvOwner_unk {
    int fn_800D59B8();
    unsigned char m00[0x20];
    atn::LH_Binary mLoader; // 0x20: inferred type
    int m28;
};

// Base of the gimmicks (gmk::*). The name is in the DOL (vtable 0x8078C9D0, RTTI bases
// atn::TaskBase, atn::Task). Virtual methods are named by vtable offset.
class GimmickCommon : public atn::Task {
public:
    GimmickCommon(atn::Task* parent, int id, int arg); // 0x803EC568
    virtual ~GimmickCommon();                          // 0x803EC69C
    virtual int vf0C() { return 1; }
    virtual void vf10() {}
    virtual void vf14() {}
    virtual int vf1C();   // 0x803ECB7C
    virtual void vf20();  // 0x803ECBCC
    virtual void vf24();  // 0x803ECBD0
    virtual void vf28() { vf2C(); }
    virtual void vf2C();  // 0x803ECB84
    virtual void vf30() {
        mA0 = (mFlags >> 2) & 1;
        mFlags |= 4;
    }
    virtual void vf34() {
        if (mA0 == 0) {
            mFlags &= ~4;
        }
    }
    // Trivial inline virtuals: the weak copies of several of them were kept in the first object
    // that used them (only gimmick vtables point to them), so they live far from this file.
    virtual int vf38() { return 0; }       // 0x80012A14
    virtual void vf3C() {}
    virtual int vf40() { return 0; }       // 0x80012714
    virtual int vf44() { return 0; }       // 0x800FA4F4
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag); // 0x803EDE34
    virtual void vf4C() {}
    virtual int vf50() { return 0; }       // 0x802AEABC
    // NONMATCHING (standalone): psq_st and stfs of the copy are in the other order
    virtual atn::Vector3 vf54() { return m6C; } // 0x800EB488: inferred: position
    virtual float vf58() { return 0.0f; }  // 0x800FA4EC
    virtual int vf5C() { return 0; }       // 0x80017944
    virtual int vf60() { return 0; }       // 0x801B7794
    virtual int vf64(int arg) { return 0; } // 0x803A04B8
    virtual void vf68() {}                 // 0x800E2FDC
    virtual void vf6C(void* menu);  // 0x803ED92C: inferred: debug menu
    virtual void vf70(int state);   // 0x803EDAF0

    void fn_803EC758(char* name);                       // inferred: "gmXXX_XXX_XXXX"
    void fn_803EC7A0(Model_unk* model, atn::CsvTab* csv); // inferred: render flags
    int fn_803EC91C();                                  // inferred: loading, nonzero while busy
    void fn_803ECBD4(Model_unk* model, int callback);   // inferred: places the model
    void fn_803ED0D4(Model_unk* model, int callback);
    void fn_803ED5D0(Model_unk* model);
    void fn_803ED610(Model_unk* model);                 // inferred: debug draw
    void fn_803ED774(Model_unk* model);                 // inferred: debug draw
    void fn_803ED8D8(GimmickCommon* gimmick);           // inferred: appends to the m5C list
    void fn_803ED8F4(GimmickCommon* gimmick);           // inferred: removes from the m5C list
    void fn_803EDB0C(GimmickUvOwner_unk* owner, GimmickUvTable_unk* table);
    void fn_803EDB18(Model_unk* model);                 // inferred: UV scroll
    void fn_803EDCF4();                                 // inferred: debug draw

    int mId;           // 0x48: inferred: ids below 1000 are scaled by 1000
    int m4C;
    int m50;           // inferred: mId / 1000 (type)
    int m54;           // inferred: state
    int m58;
    GimmickCommon* m5C; // inferred: next gimmick in a list
    atn::LH_Binary mBinary; // 0x60
    int m68;           // inferred: loading step
    atn::Vector3 m6C;  // inferred: position (vf54 returns it)
    atn::Vector3 m78;  // inferred: rotation
    atn::Vector3 m84;  // inferred: rotation offset
    atn::Vector3 m90;  // inferred: scale (1, 1, 1)
    int m9C;           // inferred: shown
    int mA0;
    int mA4;
    GimmickUvOwner_unk* mA8;
    GimmickUvTable_unk* mAC;
    GimmickCommonB0_unk mB0;
    GimmickStateCommand* mE4;
    int mE8;
    int mEC;
    ResHandle mF0; // inferred: debug menu group
};

// inferred: fades the alpha of a model material (used by gmk::MotionObject and others)
struct GimmickMaterialFade_unk {
    void fn_803EBC44(atn::CsvTab* csv, Model_unk* model);
    void fn_803EBD6C(Model_unk* model, const char* name, int start, int length, float to);
    void fn_803EBE74(Model_unk* model);

    Material_unk* mMaterial;
    int mStart;
    int mLength;
    float mFrom;
    float mTo;
    int mMode;
    int mReverse;
};

// inferred: motion switch by frame (used by gmk::MotionObject and others)
struct GimmickMotionSwitch_unk {
    void fn_803EC0A4(atn::CsvTab* csv);
    void fn_803EC0F8(Model_unk* model);

    int mHash;
    int mFrame;
};

#endif
