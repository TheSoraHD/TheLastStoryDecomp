#ifndef GMK_PASSBOARD_H
#define GMK_PASSBOARD_H

#include "atn/ColliBase.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: board with a collision box ("COLLI_BOX") whose materials ("MATERIAL_NAME") fade with
// the distance ("DISTANCE"). The name is in the DOL (__vt__Q23gmk9PassBoard).
class PassBoard : public GimmickCommon {
public:
    // inferred: pair of map objects found by name in vf0C
    struct Pair_unk {
        Pair_unk() : mA(0), mB(0) {}
        void* mA;
        void* mB;
    };

    PassBoard(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mModel(8, 32), m860(0), m920(0.0f) {
        m54 = 0;
    }
    static GimmickCommon* fn_8042D628(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~PassBoard() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    atn::Model mModel;       // 0xF4
    atn::ColliBox mColli;   // 0x4C4
    float mBox[9];          // 0x83C: inferred: read from "COLLI_BOX" (9 values)
    int m860;
    Pair_unk mPairs[5];     // 0x864
    char mMaterials[2][0x40]; // 0x88C: "MATERIAL_NAME"
    float mDistance[5];     // 0x90C: "DISTANCE"
    float m920;
    int m924;               // inferred: unused (the object is 0x928 bytes)
};

} // namespace gmk

#endif
