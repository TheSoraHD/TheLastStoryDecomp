#include "game/CrowdManager.h"
#include "game/CrowdSimulation.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/ChaseManager.h"

extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void fn_800D2338(atn::Task* task);         // inferred: deletes a task
// inferred: field data (only the members used here)
struct CrowdField_unk {
    unsigned char m0000[0x4C];
    int mMap;   // 0x4C
    int mArea;  // 0x50
    unsigned char m0054[0x10D8 - 0x54];
    void* mRoutes; // 0x10D8
    unsigned char m10DC[0x54E4 - 0x10DC];
    int mMode;  // 0x54E4
};
extern "C" CrowdField_unk* lbl_8087F430;
extern "C" int fn_803C1560(void* routes, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float dist); // inferred: nearest route point
// inferred: camera (only the members used here)
struct CrowdCamera_unk {
    unsigned char m000[0x118];
    atn::Vector3 mPos; // 0x118
};
extern "C" CrowdCamera_unk* lbl_8087EFB4;
extern "C" void* lbl_8087F498;
extern "C" void fn_803EAC3C(void* obj, NpcTask* npc);

// inferred: min/max macros (the second operand of the max is evaluated twice in the target)
#define CROWD_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CROWD_MAX(a, b) ((a) > (b) ? (a) : (b))

CrowdManager* CrowdManager::sInstance;

CrowdManager* CrowdManager::fn_801EA4B0(atn::Task* parent, CrowdDesc_unk* desc, int arg) {
    if (parent != 0) {
        sInstance = new (3, "", "", 0) CrowdManager(parent, desc, arg);
    }
    return sInstance;
}

// NONMATCHING: 98.9% - the store of mLimit is scheduled earlier and desc->mNumGroups lands in r29 instead of r28
CrowdManager::CrowdManager(atn::Task* parent, CrowdDesc_unk* desc, int arg)
    : atn::Task(parent), mCenter(0.0f, 0.0f, 0.0f), mInner(220.0f), mOuter(300.0f), mKill(330.0f), mCount(0),
      mTpcSize(0), m78(1), mDesc(desc), m80(arg), mDebug(0) {
    mNpcSizeMax = 100;
    mSizeMax = desc->mNumGroups != 0 ? 14 : 0;
    mLimit = 0;
    mSizeMin = desc->mNumGroups != 0 ? 5 : 0;
    if (mSizeMax > 0 && lbl_8087F430->mArea == 1) {
        int map = lbl_8087F430->mMap;
        if (map == 0xE || map == 0x3C || map == 0x15 || map == 0x1F || map == 0x21 || map == 0x29) {
            mSizeMax = 5;
        }
    }
    mGroupCount.Resize_unk(mDesc->mNumGroups, 3);
    for (unsigned int i = 0; i < mGroupCount.mSize; i++) {
        mGroupCount.mData[i] = 0;
    }
    void* group = mMenu.Load("Crowd");
    fn_80087994(group, "Debug", &mDebug, 0, 0);
    fn_800874C8(group, "NpcSizeMax", &mNpcSizeMax, 0, 500, 1, 0, 0);
    fn_800874C8(group, "SizeMax", &mSizeMax, 0, 200, 1, 0, 0);
    fn_800874C8(group, "SizeMin", &mSizeMin, 0, 200, 1, 0, 0);
    fn_800874C8(group, "Limit", &mLimit, 0, 200, 1, 0, 0);
    fn_800874C8(group, "TpcSize", &mTpcSize, 0, 200, 1, 0, 0);
    fn_8008771C(group, "Inner", &mInner, 0, 0, 1.0f, 10000.0f, 1.0f);
    fn_8008771C(group, "Outer", &mOuter, 0, 0, 1.0f, 10000.0f, 1.0f);
    fn_8008771C(group, "Kill", &mKill, 0, 0, 1.0f, 10000.0f, 1.0f);
    CrowdSimulation::fn_801EB93C(this, desc);
}

CrowdManager::~CrowdManager() {
    sInstance = 0;
}

int CrowdManager::vf0C() {
    return fn_800D3FA4();
}

inline void CrowdManager::UpdateLimit_unk() {
    mTpcSize = 0;
    for (NpcTask* npc = NpcManager::sInstance->mHead; npc != 0; npc = npc->mNext) {
        if (npc->m20 != this && (npc->mFlags & 4) != 4) {
            mTpcSize++;
        }
    }
    mLimit = CROWD_MAX(mSizeMin, CROWD_MIN(mSizeMax, mNpcSizeMax - mTpcSize));
    if (lbl_8087F098 != 0 && lbl_8087F098->mState != 0) {
        mLimit = mLimit > 8 ? 8 : mLimit;
    }
}

// NONMATCHING: 99.8% - the index/offset increments of the walker loop are swapped (3 loop forms tried)
void CrowdManager::vf10() {
    mCenter = lbl_8087EFB4->mPos;
    UpdateLimit_unk();
    if (lbl_8087F430->mMode != 8) {
        if (mCount < mLimit) {
            int n = mLimit - mCount;
            while (n-- > 0) {
                fn_801EB188();
            }
        }
        float kill = mKill * mKill;
        for (NpcTask* npc = NpcManager::sInstance->mHead; npc != 0; npc = npc->mNext) {
            if (mCount != 0 && !npc->mC58.mHeld_unk) {
                atn::Vector3 d(npc->mPos.x - mCenter.x, npc->mPos.y - mCenter.y, npc->mPos.z - mCenter.z);
                if (fn_805F9920(&d) > kill) {
                    CrowdSimulation* sim = CrowdSimulation::sInstance;
                    for (int i = 0; i < (int)sim->mWalkers.mSize; i++) {
                        if (npc == sim->mWalkers.mData[i].mChara) {
                            fn_801EB7C4(&sim->mWalkers.mData[i]);
                        }
                    }
                }
            }
        }
    }
}

// NONMATCHING: vf14 (0x801EAB24, 0x554 bytes: debug display) is not written yet

void CrowdManager::fn_801EB078() {
    UpdateLimit_unk();
    for (int i = 0; i < mLimit; i++) {
        fn_801EB188();
    }
    m78 = 0;
}

// NONMATCHING: fn_801EB188 (0x63C bytes: spawns an NPC on a walker) is not written yet

// NONMATCHING: 97.2% - callee-saved registers of sim/npc/routes differ (declaration orders tried)
void CrowdManager::fn_801EB7C4(CrowdWalker_unk* walker) {
    void* routes;
    CrowdSimulation* sim = CrowdSimulation::sInstance;
    NpcTask* npc = (NpcTask*)walker->mChara;
    mCount--;
    mGroupCount.mData[npc->m142C]--;
    routes = lbl_8087F430->mRoutes;
    int point = fn_803C1560(routes, &npc->mPos, npc->fn_80179D44(), 0, 0, 1, 1.0f);
    walker->mPoint = point;
    walker->mDest = point;
    walker->mChara = 0;
    for (int i = 0; i < (int)sim->mWalkers.mSize; i++) {
        if (walker == &sim->mWalkers.mData[i]) {
            break;
        }
    }
    if (lbl_8087F498 != 0) {
        fn_803EAC3C(lbl_8087F498, npc);
    }
    npc->mColli.mFlags &= ~1;
    fn_800D2338(npc);
}

void CrowdManager::fn_801EB8C0(int enable) {
    for (NpcTask* npc = NpcManager::sInstance->mHead; npc != 0; npc = npc->mNext) {
        if (npc->m20 == this) {
            if (enable != 0) {
                npc->fn_80176ACC();
            } else {
                npc->fn_801765D8();
            }
        }
    }
}
