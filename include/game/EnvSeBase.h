#ifndef GAME_ENVSEBASE_H
#define GAME_ENVSEBASE_H

#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/Task.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "game/EnvSeManager.h"

// inferred from __vt__9EnvSeBase (0x80789E58)
class EnvSeBase : public atn::Task {
public:
    EnvSeBase(atn::Task* parent, int id, int type) : atn::Task(parent), m48(id), m4C(0), mData(0), mActive(0),
        m58(0), mTimer(0) {
        mData = &lbl_8087F418->mRecords.mData[id].mData;
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

    // inferred: weak copy 0x8035FC08 (EnvSeManager::fn_8035F3D4 calls it out of line); name invented
    void SetLinked_unk(int linked) { m64 = linked; }

    int m64; // inferred: no update while set (joined by a line)
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


#endif
