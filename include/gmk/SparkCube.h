#ifndef GMK_SPARKCUBE_H
#define GMK_SPARKCUBE_H

#include "atn/Array_unk.h"
#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

extern "C" int rand();

namespace gmk {

// inferred: cube that sparks: "MODEL" and "MODEL_SPARK" (the spark plays its motion and fades its
// materials in states 3 and 4), a collision object, one effect per state and a skill fired in
// state 3. States 2 and 3 last a random time between STATE_CHANGE_FRAME values. The name is in the
// DOL (__vt__Q23gmk9SparkCube, 0x9A0 bytes).
class SparkCube : public GimmickCommon {
public:
    SparkCube(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_80426108(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~SparkCube();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf44(GimmickHitMsg_unk* msg);
    virtual int vf50();
    virtual atn::Vector3 vf54();
    virtual float vf58();
    virtual int vf5C();
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    // inferred: model shown in the current state (body of vf38, also inlined in vf10); name invented
    atn::Model* GetModel_unk() {
        if (mHasSpark != 0) {
            if (m54 < 3) {
                return &mModel;
            }
            return &mSpark;
        }
        return &mModel;
    }

    // inferred: random time of the state (name invented)
    float GetWait_unk(int state) {
        return mMin[state] + mRange[state] * (rand() % 1001 / 1000.0f);
    }

    atn::Model mModel;         // 0xF4: "MODEL"
    atn::Model mSpark;         // 0x4C4: "MODEL_SPARK"
    int mHasSpark;            // 0x894
    atn::ColliObject mColli;  // 0x898: "COLLI_OBJECT"
    float mTimer;             // 0x920
    float mWait;              // 0x924: inferred: frames until the next state
    atn::EfpHandle_unk mEfp[5]; // 0x928: "EFP" <file> <state>
    float mMin[5];            // 0x964: "STATE_CHANGE_FRAME" <state> <min> <range>
    float mRange[5];          // 0x978
    int mSkill;               // 0x98C: "SKILL"
    atn::Buffer_unk<GimmickMaterialFade_unk> mFades; // 0x990
    int mHideDebug;           // 0x998: inferred: set while the spark motion is in its active part
    int m99C;                 // inferred: unused (the object is 0x9A0 bytes)
};

} // namespace gmk

#endif
