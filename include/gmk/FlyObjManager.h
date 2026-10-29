#ifndef GMK_FLYOBJMANAGER_H
#define GMK_FLYOBJMANAGER_H

#include "atn/Array_unk.h"
#include "atn/ColliBase.h"
#include "game/GimmickCommon.h"

namespace gmk {

class FlyObjManager;

// inferred: flying object of gmk::FlyObjManager (0x450 bytes, no vtable; constructor and
// destructor out of line for the new[] of the manager). Name invented.
struct FlyObj {
    FlyObj();
    ~FlyObj();
    void fn_80427870(); // inferred: update
    void LoadMotion_unk(const char* name, const char* path); // inferred: name invented
    void Start_unk(const atn::Vector3& pos, const atn::Vector3& vel); // inferred: name invented

    FlyObjManager* mOwner;   // 0x00
    atn::ColliSphere mColli; // 0x04
    float mRadius;           // 0x50: "COLLI_RADIUS"
    atn::Model mModel;        // 0x54
    atn::Vector3 mPos;       // 0x424
    atn::Vector3 mRot;       // 0x430
    atn::Vector3 mVel;       // 0x43C
    int mState;              // 0x448: inferred: 1 hidden, 2 flying, 3 hit (falling)
    int mTimer;              // 0x44C
};

// inferred: flock of flying objects around the gimmick (desc->m20 objects); a hit makes the
// nearest one fall and gives an item. The name is in the DOL (__vt__Q23gmk13FlyObjManager,
// 0x620 bytes).
class FlyObjManager : public GimmickCommon {
public:
    FlyObjManager(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_80427EB4(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~FlyObjManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf44(GimmickHitMsg_unk* msg);

    int FindNearest_unk(const atn::Vector3& pos); // inferred: name invented

    char mModelName[0x100];        // 0xF4: "MODEL"
    char mMotionPath[2][0x100];    // 0x1F4: "MOTION"
    char mMotionName[2][0x100];    // 0x3F4: "MOTION"
    atn::Buffer_unk<FlyObj> mObjs; // 0x5F4
    unsigned int mCount;           // 0x5FC
    float mRadius;                 // 0x600: "COLLI_RADIUS"
    float mRange;                  // 0x604: inferred: radius of the flock (desc->m40)
    atn::Vector3 mVel;             // 0x608
    GimmickDesc_unk* mDesc;        // 0x614
    int mLoadStep;                 // 0x618
    int mPhase;                    // 0x61C
};

} // namespace gmk

#endif
