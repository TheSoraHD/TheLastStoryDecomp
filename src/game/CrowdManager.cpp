#include "game/CrowdManager.h"
#include "game/CrowdSimulation.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/ChaseManager.h"
#include "std/vector.h"

extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void fn_800D2338(atn::Task* task);         // inferred: deletes a task
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

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
    unsigned char m000[0x10C];
    atn::Vector3 mEye; // 0x10C
    atn::Vector3 mPos; // 0x118
};
extern "C" CrowdCamera_unk* lbl_8087EFB4;
extern "C" void* lbl_8087F498;
extern "C" void fn_803EAC3C(void* obj, NpcTask* npc);
extern "C" int rand();
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int flags, void* colli, int arg);
extern "C" void* lbl_8087EE68; // inferred: route manager
extern "C" void fn_80017064(void* routes, CharaTask* chara);

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

CrowdManager::CrowdManager(atn::Task* parent, CrowdDesc_unk* desc, int arg)
    : atn::Task(parent), mCenter(0.0f, 0.0f, 0.0f), mInner(220.0f), mOuter(300.0f), mKill(330.0f), mCount(0),
      mTpcSize(0), m78(1), mDesc(desc), m80(arg), mDebug(0) {
    mNpcSizeMax = 100;
    mSizeMax = desc->mNumGroups != 0 ? 14 : 0;
    mSizeMin = desc->mNumGroups != 0 ? 5 : 0;
    mLimit = 0;
    if (mSizeMax > 0 && lbl_8087F430->mArea == 1) {
        int map = lbl_8087F430->mMap;
        if (map == 0xE || map == 0x3C || map == 0x15 || map == 0x1F || map == 0x21 || map == 0x29) {
            mSizeMax = 5;
        }
    }
    int n = mDesc->mNumGroups;
    mGroupCount.Resize_unk(n, 3);
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

extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" void fn_80062C8C(void* draw, int segments, unsigned int color, float x, float y, float z, float f4,
                            float radius, float f6); // inferred: circle
extern "C" void fn_80063764(void* draw, const atn::Vector3* a, const atn::Vector3* b, unsigned int color,
                            float width); // inferred: line
extern "C" void fn_800BFAC8(atn::Vector3* out, CrowdCamera_unk* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" void fn_8006F2F0(void* font, wchar_t* dst, const char* src, int n); // inferred: to wide characters
extern "C" void fn_80061824(void* draw, float x, float y, float z, float w, float h, float f6, float f7, float f8,
                            const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2); // inferred: text
extern "C" int sprintf(char* dst, const char* format, ...);

// inferred: float colour (names invented)
struct CrowdColor_unk {
    CrowdColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
inline unsigned int CrowdByte_unk(float x) {
    return x >= 1.0f ? 255 : (x <= 0.0f ? 0 : (unsigned int)(255.0f * x + 0.5f));
}
inline unsigned int CrowdRGBA_unk(const CrowdColor_unk& c) {
    return (CrowdByte_unk(c.a) << 24) | (CrowdByte_unk(c.r) << 16) | (CrowdByte_unk(c.g) << 8) | CrowdByte_unk(c.b);
}

// NONMATCHING: 96% - CrowdSimulation::sInstance is reloaded in the loop body in the original, the colour test is
// scheduled after the float arguments, stack slots of the locals.
// inferred: debug display of the ranges and the walkers
void CrowdManager::vf14() {
    if (mDebug == 0) {
        return;
    }
    fn_80062C8C(lbl_8087EEB0, 16, 0x6600FF00, mCenter.x, mCenter.y, mCenter.z, 0.0f, mInner, 100.0f);
    fn_80062C8C(lbl_8087EEB0, 16, 0x66FFFF00, mCenter.x, mCenter.y, mCenter.z, 0.0f, mOuter, 100.0f);
    fn_80062C8C(lbl_8087EEB0, 16, 0x66FF0000, mCenter.x, mCenter.y, mCenter.z, 0.0f, mKill, 100.0f);
    for (int i = 0; i < (int)CrowdSimulation::sInstance->mWalkers.mSize; i++) {
        wchar_t wtext[0x100];
        char text[0x100];
        atn::Vector3 screen;
        CrowdWalker_unk* walker = &CrowdSimulation::sInstance->mWalkers.mData[i];
        if (walker->mChara == 0) {
            unsigned int color = 0x6600FF00;
            if (walker->mPoint == walker->mDest) {
                color = 0x66FFFF00;
            }
            fn_80062C8C(lbl_8087EEB0, 16, color,
                        walker->mPos.x, walker->mPos.y, walker->mPos.z, 0.0f, 5.0f, 10.0f);
            fn_80063764(lbl_8087EEB0, &walker->mPos, &walker->mNext, -1, 0.0f);
            atn::Vector3 tip = walker->mPos + walker->mDir * 10.0f;
            fn_80063764(lbl_8087EEB0, &walker->mPos, &tip, 0xFF0000FF, 0.0f);
        } else {
            CrowdColor_unk color(1.0f, 0.0f, 0.0f, 1.0f);
            fn_80062C8C(lbl_8087EEB0, 16, CrowdRGBA_unk(color), walker->mPos.x, walker->mPos.y, walker->mPos.z, 0.0f,
                        5.0f, 10.0f);
        }
        if (LengthSq_unk(lbl_8087EFB4->mEye - walker->mPos) < 1600.0f) {
            fn_800BFAC8(&screen, lbl_8087EFB4, &walker->mPos);
            if (screen.z > 0.0f && screen.z < 1.0f) {
                float y = screen.y;
                sprintf(text, "Point: %3d to %3d", walker->mPoint, walker->mDest);
                fn_8006F2F0(lbl_8087EEC8, wtext, text, 256);
                fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, 0xAFFFFFFF, 1, 1, 0,
                            0, 0xFF000000);
                y += 12.0f;
                sprintf(text, "Unit %d, Chara %d", walker->mGroup, mDesc->mGroups[walker->mGroup].mChara);
                fn_8006F2F0(lbl_8087EEC8, wtext, text, 256);
                fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, 0xAFFFFFFF, 1, 1, 0,
                            0, 0xFF000000);
                y += 12.0f;
                sprintf(text, "DPos: %3.3f, %3.3f, %3.3f", walker->mNext.x, walker->mNext.y, walker->mNext.z);
                fn_8006F2F0(lbl_8087EEC8, wtext, text, 256);
                fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, wtext, 0xAFFFFFFF, 1, 1, 0,
                            0, 0xFF000000);
            }
        }
    }
}

void CrowdManager::fn_801EB078() {
    UpdateLimit_unk();
    for (int i = 0; i < mLimit; i++) {
        fn_801EB188();
    }
    m78 = 0;
}

// inferred: one more destructor level than std::vector (three null checks of the address); name invented
struct CrowdWalkerList_unk : public std::vector<CrowdWalker_unk*> {
    ~CrowdWalkerList_unk() {}
};

inline void* GetWorld_unk() {
    return lbl_8087EE98;
}

// NONMATCHING: 98.6% - register numbering (one more saved register in the original) and the walker loop
// increments are swapped, as in vf10.
// inferred: spawns an NPC on a random free walker between mInner and mKill from the camera (anywhere inside
// mKill while m78 is set)
void CrowdManager::fn_801EB188() {
    CrowdWalkerList_unk walkers;
    CrowdSimulation* sim = CrowdSimulation::sInstance;
    for (int i = 0; i < (int)sim->mWalkers.mSize; i++) {
        CrowdWalker_unk* walker = &sim->mWalkers.mData[i];
        if (walker->mChara != 0) {
            continue;
        }
        float distSq = LengthSq_unk(walker->mPos - mCenter);
        float kill = mKill * mKill;
        bool ok = distSq <= kill;
        if (m78 == 0 && ok) {
            float inner = mInner * mInner;
            ok = false;
            if (distSq <= kill && distSq >= inner) {
                ok = true;
            }
        }
        if (ok) {
            walkers.push_back(walker);
        }
    }
    if (walkers.size() != 0) {
        CrowdWalker_unk* walker = walkers[rand() % walkers.size()];
        int group = walker->mGroup;
        NpcTask* npc = NpcTask::fn_80547C28(this, (void*)mDesc->mGroups[group].mChara);
        npc->fn_800D246C(1);
        npc->fn_80154654(npc->mC50, mDesc->mGroups[group].m04);
        npc->m568 = mDesc->mGroups[group].mSpeed;
        npc->mC58.mB4 = mDesc->mGroups[group].mRoute;
        npc->m142C = group;
        mGroupCount.mData[group]++;
        mCount++;
        npc->mRot = atn::Vector3(0.0f, 6.2831855f * ((rand() % 1001) / 1000.0f) - 3.1415927f, 0.0f);
        atn::Vector3 pos = walker->mPos;
        atn::Vector3 to = pos;
        atn::Vector3 from = pos;
        from.y += 18.0f;
        atn::Vector3 hit;
        if (fn_8004ECC0(GetWorld_unk(), &hit, &from, &to, npc->fn_80179D44() | 0x80000000, 0, 0) != 0) {
            pos.y = hit.y;
        } else {
            from.y += 18.0f;
            if (fn_8004ECC0(GetWorld_unk(), &hit, &from, &to, npc->fn_80179D44() | 0x80000000, 0, 0) != 0) {
                pos.y = hit.y;
            }
        }
        npc->mPos = pos;
        fn_80017064(lbl_8087EE68, npc);
        npc->mC58.m08 = npc->mC58.m0C.mDestPoint;
        npc->fn_8016E970(2);
        npc->m54C |= 0x20;
        npc->m56C = 0.06f;
        npc->fn_800D246C(0);
        walker->mChara = npc;
    }
}

// NONMATCHING: 98.3% - the original shares the zero register between `walker->mChara = 0` and the loop
// offset (li r4, 0); tried NULL/(void*)i, store orders, chained assignments, inline search helper.
// Registers of routes/sim/npc fixed by the declaration order (routes, sim, npc, i).
void CrowdManager::fn_801EB7C4(CrowdWalker_unk* walker) {
    void* routes;
    CrowdSimulation* sim = CrowdSimulation::sInstance;
    NpcTask* npc = (NpcTask*)walker->mChara;
    int i;
    mCount--;
    mGroupCount.mData[npc->m142C]--;
    routes = lbl_8087F430->mRoutes;
    int point = fn_803C1560(routes, &npc->mPos, npc->fn_80179D44(), 0, 0, 1, -1.0f);
    i = 0;
    walker->mPoint = point;
    walker->mDest = point;
    walker->mChara = 0;
    for (; i < (int)sim->mWalkers.mSize; i++) {
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
