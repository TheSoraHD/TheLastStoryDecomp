#ifndef GAME_CROWDSIMULATION_H
#define GAME_CROWDSIMULATION_H

#include "atn/Task.h"
#include "math/Vec.h"
#include "game/ResHandle.h"
#include "atn/Array_unk.h"

// inferred: group of walkers of the crowd description (0x20 bytes); names invented
struct CrowdGroup_unk {
    // inferred: the group walks at `time` (inlined; the members are reloaded through the owner)
    int mChara;          // 0x00: inferred: character id ("Unit %d, Chara %d"), passed to NpcTask::fn_80547C28
    int m04;             // 0x04: inferred: second argument of CharaTask::fn_80154654
    unsigned int mCount; // 0x08
    unsigned char m0C[0x10 - 0x0C];
    float mSpeed;        // 0x10
    int mRoute;          // 0x14: inferred: route of the group
    int mStart;          // 0x18: inferred: first hour (negative: always)
    int mEnd;            // 0x1C
};

struct CrowdDesc_unk {
    unsigned int mNumGroups;
    CrowdGroup_unk* mGroups;
};

// inferred: one walker (0x34 bytes)
struct CrowdWalker_unk {
    CrowdWalker_unk() {}
    int mGroup;          // 0x00
    int mPoint;          // 0x04: inferred: current route point
    int mDest;           // 0x08: inferred: destination point
    atn::Vector3 mPos;   // 0x0C
    atn::Vector3 mNext;  // 0x18: inferred: position of the next point
    atn::Vector3 mDir;   // 0x24
    void* mChara;        // 0x30: inferred: character following the walker
};

// inferred from __vt__15CrowdSimulation (0x80782A18): walkers of the town (singleton 0x8087F130)
class CrowdSimulation : public atn::Task {
public:
    static CrowdSimulation* fn_801EB93C(atn::Task* parent, CrowdDesc_unk* desc);

    CrowdSimulation(atn::Task* parent, CrowdDesc_unk* desc); // 0x801EB9B0
    virtual ~CrowdSimulation();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    // inferred: name invented (the group is fetched again through mDesc)
    bool IsValidHours_unk(int i) { return mDesc->mGroups[i].mStart <= mDesc->mGroups[i].mEnd; }

    int m48;
    unsigned char m4C[0x162C - 0x4C];
    atn::SimpleArray<CrowdWalker_unk> mWalkers; // 0x162C
    CrowdDesc_unk* mDesc;       // 0x1634
    ResHandle m1638;
    int m163C;

    static CrowdSimulation* sInstance;
};

#endif
