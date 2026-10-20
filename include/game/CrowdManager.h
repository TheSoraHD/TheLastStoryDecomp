#ifndef GAME_CROWDMANAGER_H
#define GAME_CROWDMANAGER_H

#include "atn/Task.h"
#include "atn/Array_unk.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

struct CrowdDesc_unk;
struct CrowdWalker_unk;

// inferred from __vt__12CrowdManager (0x807829E0): spawns the NPCs that follow the crowd walkers of the town
// (singleton 0x8087F128)
class CrowdManager : public atn::Task {
public:
    static CrowdManager* fn_801EA4B0(atn::Task* parent, CrowdDesc_unk* desc, int arg); // inferred: creates the instance
    CrowdManager(atn::Task* parent, CrowdDesc_unk* desc, int arg); // 0x801EA534
    virtual ~CrowdManager();   // 0x801EA874
    virtual int vf0C();        // 0x801EA920
    virtual void vf10();       // 0x801EA924
    virtual void vf14();       // 0x801EAB24
    void fn_801EB078(); // inferred: spawns the NPCs at once
    void fn_801EB188(); // inferred: spawns one NPC
    void fn_801EB7C4(CrowdWalker_unk* walker); // inferred: removes the NPC of a walker
    void fn_801EB8C0(int enable); // inferred: enables/disables the NPCs
    void UpdateLimit_unk(); // inferred: inline (name invented)

    static CrowdManager* sInstance;

    atn::Vector3 mCenter; // 0x48: inferred: camera position
    float mInner;  // 0x54
    float mOuter;  // 0x58
    float mKill;   // 0x5C
    int mLimit;    // 0x60
    int mCount;    // 0x64: inferred: spawned NPCs
    int mSizeMax;  // 0x68
    int mSizeMin;  // 0x6C
    int mNpcSizeMax; // 0x70
    int mTpcSize;  // 0x74: inferred: other NPCs
    int m78;
    CrowdDesc_unk* mDesc; // 0x7C
    int m80;
    atn::Buffer_unk<int> mGroupCount; // 0x84: inferred: NPCs per group
    int mDebug;    // 0x8C
    ResHandle mMenu; // 0x90
    int m94;
};

#endif
