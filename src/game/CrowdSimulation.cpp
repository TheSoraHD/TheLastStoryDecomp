#include "game/CrowdSimulation.h"
#include "game/CharaStatus.h"
#include "atn/Memory.h"
#include <boost/function.hpp>

extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

// inferred: same helper as in game/CharaTask.cpp
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: next point towards `point`
extern "C" atn::Vector3 fn_803C11A4(RouteTable_unk* table, int point);  // inferred: position of a point

// inferred: field status (lbl_8087F430); only the members used here
struct Field_unk {
    unsigned char m00[0x10D0];
    int m10D0; // inferred: hour
    unsigned char m10D4[0x10D8 - 0x10D4];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: route manager of the town walkers (lbl_8087EE68)
extern "C" void* lbl_8087EE68;
extern "C" int fn_80016C10(void* mgr, int route, boost::function<void()>* callback);
extern "C" int fn_80019254(void* mgr, int point, const atn::Vector3* pos, int route, int arg4, int arg5, float rot,
                           float angle, float min, float max);

CrowdSimulation* CrowdSimulation::sInstance;

CrowdSimulation* CrowdSimulation::fn_801EB93C(atn::Task* parent, CrowdDesc_unk* desc) {
    if (parent != 0) {
        sInstance = new (3, "", "", 0) CrowdSimulation(parent, desc);
    }
    return sInstance;
}

// NONMATCHING: the group loop uses one offset register for both group accesses here (two in ours),
// and the boost::function local is destroyed with a null test of its address
CrowdSimulation::CrowdSimulation(atn::Task* parent, CrowdDesc_unk* data)
    : atn::Task(parent), m48(0), mDesc(data) {
    int hour = -1;
    unsigned int count = 0;
    if (lbl_8087F430 != 0) {
        hour = lbl_8087F430->m10D0;
    }
    for (unsigned int i = 0; i < mDesc->mNumGroups; i++) {
        if (hour >= 0) {
            CrowdGroup_unk* group = &mDesc->mGroups[i];
            if (group->mStart >= 0 && group->mEnd > 0 && IsValidHours_unk(i)) {
                if (hour < group->mStart || group->mEnd < hour) {
                    continue;
                }
            }
        }
        count += mDesc->mGroups[i].mCount;
    }
    mWalkers.Resize_unk(count, 3);
    int index = 0;
    for (unsigned int i = 0; i < mDesc->mNumGroups; i++) {
        if (hour >= 0) {
            CrowdGroup_unk* group = &mDesc->mGroups[i];
            if (group->mStart >= 0 && group->mEnd > 0 && IsValidHours_unk(i)) {
                if (hour < group->mStart || group->mEnd < hour) {
                    continue;
                }
            }
        }
        for (unsigned int j = 0; j < mDesc->mGroups[i].mCount; j++) {
            int point;
            {
                boost::function<void()> callback;
                point = fn_80016C10(lbl_8087EE68, mDesc->mGroups[i].mRoute, &callback);
            }
            mWalkers[index].mGroup = i;
            mWalkers[index].mPoint = point;
            mWalkers[index].mDest = point;
            mWalkers[index].mChara = 0;
            atn::Vector3 dir(0.0f, 2.0f * ((rand() % 1001) / 1000.0f) - 1.0f, 0.0f);
            mWalkers[index].mDir = dir;
            mWalkers[index].mPos = fn_803C11A4(lbl_8087F430->m10D8, point);
            mWalkers[index].mNext = atn::Vector3(0.0f, 0.0f, 0.0f);
            index++;
        }
    }
}

CrowdSimulation::~CrowdSimulation() {
    sInstance = 0;
}

int CrowdSimulation::vf0C() {
    return fn_800D3FA4();
}

// NONMATCHING: f5/f6 swap between the z difference and the radius
void CrowdSimulation::vf10() {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    if ((table->m38 & 2) == 2) {
        for (unsigned int i = 0; i < mWalkers.mSize; i++) {
            CrowdWalker_unk* walker = &mWalkers[i];
            int point = walker->mPoint;
            if (point > 0) {
                if (walker->mChara == 0) {
                    int dest = walker->mDest;
                    if (point == dest || point <= 0 || dest <= 0) {
                        walker->mDest = fn_80019254(lbl_8087EE68, point, &walker->mPos,
                                                    mDesc->mGroups[walker->mGroup].mRoute, 0, 1, walker->mDir.y,
                                                    360.0f, 100.0f, 10000.0f);
                        if (walker->mDest != 0 && walker->mPoint != walker->mDest) {
                            walker->mNext = fn_803C11A4(
                                table, fn_803CD958((RoutePath_unk*)((char*)table->mPaths + (walker->mPoint - 1) * 8),
                                                   walker->mDest - 1));
                            walker->mDir = (walker->mNext - walker->mPos).GetNormalize();
                        }
                    } else {
                        RoutePoint_unk* next = &table->mPoints[dest - 1];
                        float limit = next->mRadius * next->mRadius;
                        if (LengthSq_unk(walker->mNext - walker->mPos) < limit) {
                            walker->mPoint = fn_803CD958(
                                (RoutePath_unk*)((char*)table->mPaths + (walker->mPoint - 1) * 8), walker->mDest - 1);
                            walker->mNext = fn_803C11A4(table, walker->mPoint);
                            walker->mDir = (walker->mNext - walker->mPos).GetNormalize();
                        } else {
                            walker->mPos += walker->mDir * mDesc->mGroups[walker->mGroup].mSpeed;
                        }
                    }
                } else {
                    walker->mPos = ((atn::Vector3*)((char*)walker->mChara + 0x528))[0];
                }
            }
        }
    }
}

void CrowdSimulation::vf14() {}
