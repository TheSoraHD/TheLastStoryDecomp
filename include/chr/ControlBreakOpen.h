#ifndef CHR_CONTROLBREAKOPEN_H
#define CHR_CONTROLBREAKOPEN_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: message sent to the opened object (0x20 bytes)
struct BreakMsg_unk {
    BreakMsg_unk() : m04(0), m08(0), m0C(0), m10(0), mVec(0.0f, 0.0f, 0.0f) {}
    int mType;
    int m04;
    int m08;
    int m0C;
    int m10;
    atn::Vector3 mVec;
};

// inferred: object that receives BreakMsg_unk through its virtual at 0x40
class BreakTarget_unk {
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
    virtual int vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40(BreakMsg_unk* msg);
};

class ControlBreakOpen : public ControlBase {
public:
    ControlBreakOpen(Chara_unk* chara, BreakTarget_unk* target); // 0x80187D04
    virtual ~ControlBreakOpen() {}
    virtual int vf0C();
    virtual void vf1C();

    int mState;                // 0x08
    BreakTarget_unk* mTarget;  // 0x0C
    int mWait;                 // 0x10
};

} // namespace chr

#endif
