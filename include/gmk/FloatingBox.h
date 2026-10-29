#ifndef GMK_FLOATINGBOX_H
#define GMK_FLOATINGBOX_H

#include "atn/ColliBase.h"
#include "std/new.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

class CharaTask;

// inferred: fixed list of characters {count, pointers[8]} (like atn::FixedArray_unk, but the count
// is cleared in the constructor body); names invented
struct CharaList8_unk {
    CharaTask** begin() { return mData; }
    CharaTask** end() { return mData + mSize; }
    void erase(CharaTask** it) {
        for (unsigned int i = it - mData; i < mSize - 1; i++) {
            mData[i] = mData[i + 1];
        }
        mSize--;
    }
    void push_back(CharaTask* const& value) {
        new (&mData[mSize]) CharaTask*(value);
        mSize++;
    }

    unsigned int mSize;
    CharaTask* mData[8];
};

namespace gmk {

// inferred: floating box: bobs up and down (sin of mPhase) with an amplitude that is reset while
// a player stands on it. The name is in the DOL (__vt__Q23gmk11FloatingBox, 0x880 bytes).
class FloatingBox : public GimmickCommon {
public:
    FloatingBox(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mModel(8, 32), mAmp(1.0f), mCurAmp(0.0f), mPhase(0.0f), m878(0.0f) {
        m54 = 0;
        mRiders.mSize = 0;
    }
    static GimmickCommon* fn_8042C21C(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~FloatingBox();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    atn::Model mModel;         // 0xF4
    atn::ColliBox mColli;     // 0x4C4
    atn::Vector3 mBasePos;    // 0x83C
    float mAmp;               // 0x848
    float mCurAmp;            // 0x84C
    float mPhase;             // 0x850
    CharaList8_unk mRiders;   // 0x854
    float m878;
    int m87C; // inferred: unused (the object is 0x880 bytes)
};

} // namespace gmk

#endif
