#ifndef GMK_OPENBOARD_H
#define GMK_OPENBOARD_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: board that opens (motion) when a character comes within "OPEN_DISTANCE". The name is
// in the DOL (__vt__Q23gmk9OpenBoard).
class OpenBoard : public GimmickCommon {
public:
    // inferred: value interpolated from a start to an end (name invented); the argument is taken by
    // reference, so it is reloaded for every member
    struct Lerp_unk {
        Lerp_unk(const float& value) : mFrom(value), mTo(value), mCur(value), mDelta(mTo - mFrom) {}
        void Set_unk(const float& value) {
            mFrom = value;
            mTo = value;
            mCur = value;
            mDelta = mTo - mFrom;
        }
        float mFrom;
        float mTo;
        float mCur;
        float mDelta;
    };

    OpenBoard(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_8042B56C(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~OpenBoard() {}
    virtual int vf0C();
    virtual void vf10(); // 0x8042B90C: not decompiled yet
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    Model_unk mModel;         // 0xF4
    atn::ColliObject mColli;  // 0x4C4
    atn::EfpHandle_unk mEfp;  // 0x54C
    float mOpenDistance;      // 0x558: "OPEN_DISTANCE"
    float m55C;
    float m560;
    float m564;
    bool m568;
    Lerp_unk m56C;
    float m57C;
};

} // namespace gmk

#endif
