#ifndef GAME_ENVSEBASE_H
#define GAME_ENVSEBASE_H

#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/Task.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

// inferred: data of an environment sound (0x58 bytes, kept by the manager lbl_8087F418 in records
// of 0x5C bytes at m58 + 4). Names invented.
struct EnvSeData_unk {
    int mType;          // 0x00: inferred: 0 Point, 1 Line, 2 Sphere, 3 Box, 4 Global, 5 Area
    char mName[0x24];   // 0x04
    float mVolume;      // 0x28
    atn::Vector3 mPos;  // 0x2C
    float mDistance;    // 0x38
    float mLPF;         // 0x3C
    int m40;            // inferred: 1 = muted while lbl_8087F540->m70 is set
    int m44;            // inferred: plays only when the event flag m4C has the value m50
    int m48;            // inferred: kind of the flag (1: fn_80370174)
    int m4C;
    int m50;
    int m54;
};

// inferred: record of the manager (0x5C bytes)
struct EnvSeRecord_unk {
    int m00;
    EnvSeData_unk mData;
};

// inferred: environment sound manager (lbl_8087F418); only the members used here (names invented)
struct EnvSeManager_unk {
    unsigned char m00[0x50];
    int m50;                 // inferred: state (5 = playing)
    unsigned char m54[0x58 - 0x54];
    EnvSeRecord_unk* m58;    // inferred: records
    unsigned char m5C[0x68 - 0x5C];
    int (*m68)[2];           // inferred: pairs of ids (segments of the lines)
    unsigned char m6C[0x78 - 0x6C];
    int m78;
    unsigned char m7C[0x84 - 0x7C];
    int m84;                 // inferred: current area id
    unsigned char m88[0x1A4 - 0x88];
    int m1A4;                // inferred: debug draw level
    unsigned char m1A8[0x1AC - 0x1A8];
    void* m1AC;              // inferred: debug menu root
};
extern "C" EnvSeManager_unk* lbl_8087F418;

// inferred from __vt__9EnvSeBase (0x80789E58)
class EnvSeBase : public atn::Task {
public:
    EnvSeBase(atn::Task* parent, int id, int type) : atn::Task(parent), m48(id), m4C(0), mData(0), mActive(0),
        m58(0), mTimer(0) {
        mData = &lbl_8087F418->m58[id].mData;
        mData->mType = type;
    }
    virtual ~EnvSeBase();
    virtual void vf1C(atn::Csv* csv); // inferred: reads the parameters of the kind
    virtual int vf20();               // inferred: the listener is in range

    void fn_80360CDC(atn::Csv* csv);  // inferred: reads the line and creates the debug menu
    void fn_80360EA4();               // inferred: update of the play state
    void fn_8036102C(int value);
    static void fn_80361100(EnvSeBase* self); // inferred: debug menu callback
    void DrawName_unk(const atn::Vector3& pos);

    int m48;               // inferred: index of the record
    int m4C;
    EnvSeData_unk* mData;  // 0x50
    int mActive;           // 0x54
    int m58;               // inferred: playing
    int mTimer;            // 0x5C
    ResHandle mMenu;       // 0x60: inferred: debug menu group
// inferred: the sizes passed to new are rounded to 8 (0x68 for EnvSeGlobal, which adds nothing)
} __attribute__((aligned(8)));

class EnvSePoint : public EnvSeBase {
public:
    EnvSePoint(atn::Task* parent, int id) : EnvSeBase(parent, id, 0), m64(0) {}
    virtual ~EnvSePoint();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(atn::Csv* csv);

    static EnvSePoint* fn_8036111C(atn::Task* parent, int id);

    int m64; // inferred: no update while set
};

class EnvSeLine : public EnvSeBase {
public:
    EnvSeLine(atn::Task* parent, int id) : EnvSeBase(parent, id, 1) {}
    virtual ~EnvSeLine();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(atn::Csv* csv);

    static EnvSeLine* fn_80361414(atn::Task* parent, int id);

    atn::SimpleArray<int> m64; // inferred: segments (indices into the pairs of the manager)
};

class EnvSeSphere : public EnvSeBase {
public:
    EnvSeSphere(atn::Task* parent, int id) : EnvSeBase(parent, id, 2) {}
    virtual ~EnvSeSphere();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(atn::Csv* csv);

    static EnvSeSphere* fn_80362278(atn::Task* parent, int id);

    atn::Vector3 mCenter; // 0x64
    float mRadius;        // 0x70
};

class EnvSeBox : public EnvSeBase {
public:
    EnvSeBox(atn::Task* parent, int id) : EnvSeBase(parent, id, 3) {}
    virtual ~EnvSeBox();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(atn::Csv* csv);

    static EnvSeBox* fn_803626A8(atn::Task* parent, int id);

    atn::Vector3 mSize;     // 0x64: inferred: half size
    atn::Matrix3x4 mMtx;    // 0x70
    float mRadius;          // 0xA0
};

class EnvSeGlobal : public EnvSeBase {
public:
    EnvSeGlobal(atn::Task* parent, int id) : EnvSeBase(parent, id, 4) {}
    virtual ~EnvSeGlobal();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(atn::Csv* csv);
    virtual int vf20() { return 1; }

    static EnvSeGlobal* fn_80362CC4(atn::Task* parent, int id);
};

// inferred: area of the environment sounds (reverb), not a task (names invented)
struct EnvSeArea_unk {
    void fn_80362FA4(atn::Csv* csv);  // inferred: reads the line
    void fn_80363108(void* root);     // inferred: creates the debug menu
    static void fn_8036324C(EnvSeArea_unk* self); // inferred: debug menu callback

    int mId;               // 0x00
    int m04;
    EnvSeData_unk mData;   // 0x08
    atn::Vector3 mSize;    // 0x60
    atn::Matrix3x4 mMtx;   // 0x6C
    int mReverb;           // 0x9C
    float mA0;
    ResHandle mMenu;       // 0xA4
};

#endif
