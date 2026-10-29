#ifndef GMK_PIGEONMANAGER_H
#define GMK_PIGEONMANAGER_H

#include "atn/Array_unk.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"

class CharaTask;

namespace gmk {

class PigeonManager;

// inferred: one pigeon of a gmk::PigeonManager (0x43C bytes, built with new[]). Names invented.
struct Pigeon_unk {
    Pigeon_unk();  // 0x804047A4
    ~Pigeon_unk(); // 0x80404864
    void fn_804048CC(); // inferred: update
    void fn_80404E70(); // inferred: reset (lands at a random place)
    void fn_804050C0(); // inferred: walking/pecking on the ground
    void fn_804052EC(); // inferred: walks; turns away from the edge and from the other pigeons
    void fn_804058A4(); // inferred: flies away until the timer runs out
    void fn_80405A14(CharaTask* player); // inferred: reacts to the player (flies away or walks off)

    PigeonManager* mOwner; // 0x00
    atn::Model mModel;      // 0x04
    atn::Vector3 mPos;     // 0x3D4
    atn::Vector3 m3E0;
    atn::Vector3 m3EC;
    atn::Vector3 m3F8;
    atn::Vector3 mCenter;  // 0x404
    float mRadius;         // 0x410
    float m414;
    int m418;
    int m41C;
    int mState;            // 0x420: inferred: 0 hidden, 3 flying away
    unsigned char m424;
    unsigned char mReady;  // 0x425
    int mTries;            // 0x428: inferred: placement retries
    int m42C;
    atn::SoundRef mSound;  // 0x430
    float m434;
    bool m438;
};

// inferred: flock of pigeons ("MODEL", up to four "MOTION"s) around a place; they fly away when the
// player comes near. The name is in the DOL (__vt__Q23gmk13PigeonManager, 0x640 bytes).
class PigeonManager : public GimmickCommon {
public:
    PigeonManager(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_80405DD4(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~PigeonManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();

    inline Pigeon_unk* FindNear_unk(const atn::Vector3& pos); // inferred: another pigeon near pos

    bool mLoaded;            // 0xF4
    char mModelName[0x100];  // 0xF5: "MODEL"
    char mMotionName[4][0x100]; // 0x1F5: "MOTION"
    int mMotionId[4];        // 0x5F8
    atn::SimpleArray<Pigeon_unk> mPigeons; // 0x608
    int mCount;              // 0x610: inferred: mDesc->m20
    atn::Vector3 mTmp;       // 0x614: inferred: position compared in FindNear_unk
    atn::Vector3 mCenter;    // 0x620
    float mRadius;           // 0x62C: inferred: mDesc->m40
    GimmickDesc_unk* mDesc;  // 0x630
    atn::SoundRef mSound;    // 0x634
    int mState;              // 0x638: inferred: 0 active, 1 far away, 2 reset
    int m63C;                // inferred: unused (0x640 bytes)
};

} // namespace gmk

#endif
