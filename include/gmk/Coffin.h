#ifndef GMK_COFFIN_H
#define GMK_COFFIN_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"

class CharaTask;

namespace gmk {

// inferred: coffin that the player opens to the left or the right (messages 3 and 4); the character
// inside (enemy or ally, looked up from a field counter) gets up when it is open. Models "MODEL",
// collisions "COLLI_BEFORE"/"COLLI_AFTER_LEFT"/"COLLI_AFTER_RIGHT", effects "EFP"/"OPEN_EFP_*".
// The name is in the DOL (__vt__Q23gmk6Coffin).
class Coffin : public GimmickCommon {
public:
    Coffin(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_80411440(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Coffin();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);
    void fn_80412968(); // inferred: finds the character inside
    // inferred: makes the character inside get up (name invented)
    void GetUp_unk();

    atn::Model mModel;                // 0xF4
    atn::ColliObject mColli;         // 0x4C4: "COLLI_BEFORE"
    atn::ColliObject mColliAfter[2]; // 0x54C: "COLLI_AFTER_LEFT", "COLLI_AFTER_RIGHT"
    GimmickDesc_unk* mDesc;          // 0x65C
    atn::EfpFile mEfp;         // 0x660: "EFP"
    atn::EfpFile mOpenEfp[2];  // 0x66C: "OPEN_EFP_LEFT", "OPEN_EFP_RIGHT"
    atn::SoundRef mSound;            // 0x684: inferred: unused
    int mTimer;                      // 0x688: inferred: frames before the effect
    int mSide;                       // 0x68C: inferred: 0 left, 1 right
    int mCount;                      // 0x690
    int mKind;                       // 0x694: inferred: 0 empty, 1 enemy, 2 ally
    CharaTask* mChara;               // 0x698
    int m69C;                        // inferred: unused (the object is 0x6A0 bytes)
};

} // namespace gmk

#endif
