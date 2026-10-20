#ifndef GMK_CHICKENMANAGER_H
#define GMK_CHICKENMANAGER_H

#include "atn/Array_unk.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"

namespace gmk {

class ChickenManager;

// inferred: one chicken of a gmk::ChickenManager (0x434 bytes, built with new[]). Names invented.
struct Chicken_unk {
    Chicken_unk();  // 0x8040B560
    ~Chicken_unk(); // 0x8040B60C
    void fn_8040B668(); // inferred: update; not decompiled yet
    void fn_8040BC38(); // inferred: reset (placed at a random point); not decompiled yet
    void fn_8040BE88(); // not decompiled yet
    void fn_8040C058(); // not decompiled yet
    void fn_8040C610(); // not decompiled yet
    void fn_8040CD2C(); // not decompiled yet

    ChickenManager* mOwner; // 0x00
    Model_unk mModel;      // 0x04
    atn::Vector3 mPos;     // 0x3D4
    atn::Vector3 m3E0;
    atn::Vector3 m3EC;
    atn::Vector3 m3F8;
    atn::Vector3 mCenter;  // 0x404
    float mRadius;         // 0x410
    float m414;
    int m418;
    int m41C;
    int m420;
    int mState;            // 0x424: inferred: 3 and 4 = scared
    int mTries;            // 0x428: inferred: placement retries
    int m42C;
    unsigned char m430;
    unsigned char mReady;  // 0x431
};

// inferred: chickens ("MODEL", three "MOTION"s) around a place (twin of gmk::PigeonManager); a sound
// follows how many of them are scared. The name is in the DOL (__vt__Q23gmk14ChickenManager,
// 0x648 bytes).
class ChickenManager : public GimmickCommon {
public:
    ChickenManager(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_8040D1D0(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~ChickenManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();

    inline Chicken_unk* FindNear_unk(const atn::Vector3& pos); // inferred: another chicken near pos
    inline void PlaySe_unk(int index); // inferred: starts the sound sSe[index]

    bool mLoaded;            // 0xF4
    char mModelName[0x100];  // 0xF5: "MODEL"
    char mMotionName[4][0x100]; // 0x1F5: "MOTION"
    int mMotionId[4];        // 0x5F8
    atn::Buffer_unk<Chicken_unk> mChickens; // 0x608
    int mCount;              // 0x610: inferred: mDesc->m20
    atn::Vector3 mTmp;       // 0x614: inferred: position compared in FindNear_unk
    atn::Vector3 mCenter;    // 0x620
    float mRadius;           // 0x62C: inferred: mDesc->m40
    GimmickDesc_unk* mDesc;  // 0x630
    int mState;              // 0x634: inferred: 0 active, 1 far away, 2 reset
    atn::SoundRef mSound;    // 0x638
    float mSoundDist;        // 0x63C: inferred: mRadius + 50
    int mScared;             // 0x640: inferred: the scared sound is playing
    int mScaredTimer;        // 0x644
};

} // namespace gmk

#endif
