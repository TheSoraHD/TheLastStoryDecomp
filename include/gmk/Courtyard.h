#ifndef GMK_COURTYARD_H
#define GMK_COURTYARD_H

#include "atn/ColliBase.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: model with a motion and two collision objects ("COLLI_OBJECT" index 0 or 1), one
// active in each state. The name is in the DOL (__vt__Q23gmk9Courtyard, 0x5D8 bytes).
class Courtyard : public GimmickCommon {
public:
    Courtyard(atn::Task* parent, int id, int arg) : GimmickCommon(parent, id, arg), mModel(8, 32) {
        m54 = 0;
    }
    static GimmickCommon* fn_8041C09C(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~Courtyard();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);

    atn::Model mModel;         // 0xF4
    atn::ColliObject mColli0; // 0x4C4: "COLLI_OBJECT" 0
    atn::ColliObject mColli1; // 0x54C: "COLLI_OBJECT" 1
    int m5D4; // inferred: unused (the object is 0x5D8 bytes)
};

} // namespace gmk

#endif
