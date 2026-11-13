#include "game/CharaStatus.h"
#include "game/GameScene.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaTask.h"
#include "game/ResHandle.h"
#include "game/GameConfig_unk.h"
#include "math/VecAssign_unk.h"

// inferred: field objects that own the route tables
struct RouteField_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
};
struct RouteField2_unk {
    unsigned char m00[0x1A6C];
    RouteTable_unk* m1A6C;
};
extern "C" RouteField_unk* lbl_8087F430;
extern "C" RouteField2_unk* lbl_8087F540;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: route tables of the current field (name invented)
inline RouteTable_unk* GetRouteTable_unk() {
    if (lbl_8087F430 != 0) {
        return lbl_8087F430->m10D8;
    }
    if (lbl_8087F540 != 0) {
        return lbl_8087F540->m1A6C;
    }
    return 0;
}

RouteWalker_unk::RouteWalker_unk() {
    mLink = 0;
    m2C = 1;
    mA0 = 0;
    mChara = 0;
    m70.m00.x = 0.0f;
    m70.m00.y = 0.0f;
    m70.m00.z = 0.0f;
    m70.m0C.x = 0.0f;
    m70.m0C.y = 0.0f;
    m70.m0C.z = 1.0f;
    m70.m18 = 0.0f;
    m70.m1C = 0.0f;
    fn_8012539C();
    m2C = 1;
}

void RouteWalker_unk::fn_8012539C() {
    if (m2C == 0) {
        RouteTable_unk* table = GetRouteTable_unk();
        if (table != 0 && mLink > 0) {
            RouteLink_unk* link = &table->mLinks[mLink - 1];
            link->m3C = 0;
        }
    }
    mMode = -1;
    m48 = 0;
    m90 = 0;
    m64 = lbl_807C7030;
    m98 = 0;
    m94 = 1;
    mLink = 0;
    mPointB = 0;
    mPointA = 0;
    mDestPoint = 0;
    m10 = lbl_807C7030;
    m1C = 0;
    mPlace = 0;
    mTarget = 0;
    mRadius = 0.0f;
    m2C = 0;
}

int RouteWalker_unk::fn_80125474() {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0) {
        return 0;
    }
    if ((mPointA > 0 && mPointA <= table->mNumPoints && table->mPoints[mPointA - 1].m2C == 0) ||
        (mPointB > 0 && mPointB <= table->mNumPoints && table->mPoints[mPointB - 1].m2C == 0) ||
        (mLink > 0 && mLink <= table->mNumLinks && table->mLinks[mLink - 1].m38 == 0)) {
        return 0;
    }
    if (mPointB <= 0) {
        return 0;
    }
    if (mMode == 0 && mPointA <= 0) {
        return 0;
    }
    if (mMode == 1 && mLink <= 0) {
        return 0;
    }
    if ((mMode == 4 || mMode == 6 || mMode == 7) && mTarget == 0) {
        return 0;
    }
    return 1;
}

// inferred: route search (lbl_8087EE68) and route registration of the field
struct RouteFinder_unk;
extern "C" RouteFinder_unk* lbl_8087EE68;
extern "C" int fn_80019E88(RouteFinder_unk* finder, atn::Vector3* pos, int* point, int link, int arg);
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: next point towards `point`
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
struct ColliWorld_unk;
extern "C" ColliWorld_unk* lbl_8087EE98;
extern "C" int fn_8004ECC0(ColliWorld_unk* world, int arg1, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, int arg5, int arg6); // inferred: line check

// inferred: name invented
inline float LengthSq_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: position of the point mPointB as the walker reaches it: the point itself, a random spot of it
// ahead of the walk direction or a random spot (name invented; inlined in fn_801279E8)
static inline atn::Vector3 GetPointPos_unk(RouteWalker_unk* walker) {
    int point = walker->mPointB;
    RouteTable_unk* table = GetRouteTable_unk();
    if (((unsigned int)walker->m48 & 2) == 2) {
        return table->mPoints[point - 1].mPos;
    }
    if (((unsigned int)walker->m48 & 0x10) == 0x10) {
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 side;
        PSVECCrossProduct(&walker->m58, &up, &side);
        if (0.0f == PSVECSquareMag(&side)) {
            side = up;
        } else {
            atn::Vector3 n = side;
            PSVECNormalize(&n, &n);
            side = n;
        }
        return ((GameScene*)table)->fn_803C1338(point, &side);
    }
    return ((GameScene*)table)->fn_803C11A4(point);
}

// inferred: as GetPointPos_unk for the point `point` (name invented; inlined in fn_801255C8)
static inline atn::Vector3 GetPointPosAt_unk(RouteWalker_unk* walker, int point) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (((unsigned int)walker->m48 & 2) == 2) {
        return table->mPoints[point - 1].mPos;
    }
    if (((unsigned int)walker->m48 & 0x10) == 0x10) {
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 side;
        PSVECCrossProduct(&walker->m58, &up, &side);
        if (0.0f == PSVECSquareMag(&side)) {
            atn::VecAssign_unk(side, up);
        } else {
            side = side.GetNormalize();
        }
        return ((GameScene*)table)->fn_803C1338(point, &side);
    }
    return ((GameScene*)table)->fn_803C11A4(point);
}

// NONMATCHING: 99.92% - registers only: m48/mMode loaded into r3/r0 swapped before fn_80128CF8 and the r3/r4
// pair of the copy of m4C - mChara->mPos (VecAssign_unk does not change it)
// inferred: starts the planned route: walks straight to the goal when it is in sight, otherwise towards the
// next point (nonzero on success)
int RouteWalker_unk::fn_801255C8() {
    if (mChara->IsFlag54C_unk(0x20)) {
        m48 |= 0x10;
    }
    if (fn_80125474() != 0) {
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    RouteTable_unk* table = GetRouteTable_unk();
    m44 = mMode;
    if (((unsigned int)m48 & 0x40) != 0x40 && fn_80128CF8() != 0) {
        fn_80129144();
    } else {
        switch (mMode) {
        case 0:
            m4C = GetPointPosAt_unk(this, mPointA);
            break;
        case 1:
            if (mPointA > 0) {
                mMode = 0;
                m4C = GetPointPosAt_unk(this, mPointA);
            } else {
                m4C = m10;
                m94 = 0;
            }
            break;
        case 3:
            if (mPointA > 0) {
                mMode = 0;
                m4C = GetPointPosAt_unk(this, mPointA);
            } else {
                m4C = table->FindPlace_unk(mPlace)->mPos;
                m94 = 0;
            }
            break;
        case 4:
            if (mPointA > 0) {
                mMode = 0;
                m4C = GetPointPosAt_unk(this, mPointA);
            } else {
                m4C = mTarget->mPos;
                m94 = 0;
            }
            break;
        case 6:
            if (mPointA > 0) {
                mMode = 0;
                m4C = GetPointPosAt_unk(this, mPointA);
            } else {
                m4C = mChara->mPos;
                m94 = 0;
            }
            break;
        case 5:
            if (mPointA > 0) {
                mMode = 0;
                m4C = GetPointPosAt_unk(this, mPointA);
            } else {
                m4C = m30;
                m94 = 0;
            }
            break;
        case 7:
            if (mPointA > 0) {
                mMode = 0;
                m4C = GetPointPosAt_unk(this, mPointA);
            } else {
                m4C = mTarget->mPos;
                atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                Mtx rot;
                PSMTXRotRad(rot, 'y', mTarget->mRot.y);
                PSMTXMultVec(rot, &dir, &dir);
                Mtx rot2;
                PSMTXRotRad(rot2, 'y', m9C);
                PSMTXMultVec(rot2, &dir, &dir);
                m4C += dir * 15.0f;
                m94 = 0;
            }
            break;
        }
        m58 = m4C - mChara->mPos;
    }
    m70 = fn_80129670(mDestPoint, mPointA);
    return 1;
    }
    m2C = 1;
    return 0;
}

// NONMATCHING: 98.65% - stack slots of the vectors of modes 4, 5 and 7 (the original has them in the opposite
// order) and the load of the facing of the target before the copy of its position
// inferred: distance from the character to the goal of the route in its mode (-1 without route)
float RouteWalker_unk::fn_801279E8() {
    switch (mMode) {
    case 0:
        if (m44 == 4) {
            atn::Vector3 d = mTarget->mPos - mChara->mPos;
            return PSVECMag(&d);
        } else if (m44 == 5) {
            atn::Vector3 d = m30 - mChara->mPos;
            return PSVECMag(&d);
        } else if (m44 == 7) {
            atn::Vector3 pos = mTarget->mPos;
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx rot;
            PSMTXRotRad(rot, 'y', mTarget->mRot.y);
            PSMTXMultVec(rot, &dir, &dir);
            Mtx rot2;
            PSMTXRotRad(rot2, 'y', m9C);
            PSMTXMultVec(rot2, &dir, &dir);
            pos += dir * 15.0f;
            atn::Vector3 d = pos - mChara->mPos;
            return PSVECMag(&d);
        } else {
            atn::Vector3 pos = GetPointPos_unk(this);
            atn::Vector3 d = pos - mChara->mPos;
            return PSVECMag(&d);
        }
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        return PSVECMag(&m58);
    case 6: {
        atn::Vector3 d = mTarget->mPos - mChara->mPos;
        return PSVECMag(&d);
    }
    }
    return -1.0f;
}

void RouteWalker_unk::fn_80127D8C(int a, int b, int c) {
    chr::Chara_unk* chara = mChara;
    m4C = chara->mPos;
    m58 = lbl_807C7030;
    if (chara->CanAct_unk()) {
        ((CharaTask*)chara)->fn_8016EB48(a, b, c);
    }
    m2C = 1;
    if (mChara->mFlag31 == 0 && m44 == 1 && mLink > 0) {
        RouteTable_unk* table = GetRouteTable_unk();
        if (table != 0) {
            RouteLink_unk* link = &table->mLinks[mLink - 1];
            if (link->m3C != 0) {
                link->m3C = 0;
            }
        }
    }
}


// NONMATCHING: the target materializes (m48 & 1) != 1 as a value (subfic/or/srwi.) before testing
// it; the rest (fn_80127D8C inlined) is the same. Tried: bool and int locals, inline predicates
// returning bool or int, a ternary.
int RouteWalker_unk::fn_80127EF4() {
    if ((m48 & 1) != 1) {
        fn_80127D8C(0, 0, 0);
        return 1;
    }
    return 0;
}

// NONMATCHING: register allocation of the inlined planners only (the original keeps the destination in r26
// in every case; ours puts it above the arguments)
// inferred: plans the route of the current mode again from the position of the character
void RouteWalker_unk::fn_80128090() {
    switch (m44) {
    case 0:
        fn_80128508(mPointB, &mChara->mPos, m48);
        break;
    case 1:
        fn_801286F0(mLink, &mChara->mPos, m48);
        break;
    case 2:
        return;
    case 3:
        fn_80128818(mPlace, &mChara->mPos, m48);
        break;
    case 4:
        fn_80128930(mTarget, &mChara->mPos, m48);
        break;
    case 5:
        fn_80128A30(&m30, &mChara->mPos, m48, m3C);
        break;
    case 6:
        fn_80128B20(mTarget, &mChara->mPos, m48, mRadius);
        break;
    case 7:
        fn_80128C08(mTarget, &mChara->mPos, m48, m9C);
        break;
    }
    fn_801255C8();
}

void RouteWalker_unk::fn_80128508(int start, const atn::Vector3* dest, int arg) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table != 0 && start > 0) {
        fn_8012539C();
        int end = ((GameScene*)table)->fn_803C1560(dest, mChara->fn_80179D44(), 0, mA0, 1, -1.0f);
        if (end > 0) {
            mMode = 0;
            m48 = arg;
            mPointB = start;
            mDestPoint = end;
            mPointA = fn_803CD958((RoutePath_unk*)((char*)table->mPaths + (end - 1) * 8), start - 1);
            RoutePoint_unk* point = &table->mPoints[mDestPoint - 1];
            if (mPointA > 0) {
                if (LengthSq_unk(point->mPos - *dest) >= point->mRadius * point->mRadius) {
                    RoutePoint_unk* next = &table->mPoints[mPointA - 1];
                    atn::Vector3 up(0.0f, 10.0f, 0.0f);
                    atn::Vector3 from = *dest + up;
                    atn::Vector3 to = next->mPos + up;
                    if (fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0) != 0) {
                        mPointA = mDestPoint;
                    }
                }
            }
        }
    }
}

// noinline: called with bl from fn_80128090, which inlines the other planners
__declspec(noinline) void RouteWalker_unk::fn_801286F0(int link, const atn::Vector3* b, int c) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table != 0 && link > 0) {
        fn_8012539C();
        atn::Vector3 pos;
        int point;
        if (fn_80019E88(lbl_8087EE68, &pos, &point, link, 0) != 0) {
            fn_80128508(((GameScene*)table)->fn_803C1560(&table->GetLink_unk(link)->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f),
                        b, c);
            table->GetLink_unk(link)->m3C = 1;
            mLink = link;
            m10 = pos;
            m1C = point;
        }
        mMode = 1;
        m48 = c;
    }
}

void RouteWalker_unk::fn_80128818(int id, const atn::Vector3* b, int c) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0) {
        return;
    }
    RoutePlace_unk* place = table->FindPlace_unk(id);
    if (place == 0) {
        return;
    }
    fn_8012539C();
    fn_80128508(((GameScene*)table)->fn_803C1560(&place->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), b, c);
    mMode = 3;
    m48 = c;
    mPlace = id;
    mRadius = 3.0f;
}

// NONMATCHING: only the order of the two radius loads (target loads mChara's first into f1).
// Tried: both operand orders, locals, an inline adder, adding 5 first or last. `float r = mChara->m5B0;
// r = 5.0f + (target->m5B0 + r);` gives the load order but swaps f1/f2/f3 (99.3%).
void RouteWalker_unk::fn_80128930(chr::Chara_unk* target, const atn::Vector3* dest, int arg) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0 || target == 0) {
        return;
    }
    fn_8012539C();
    fn_80128508(((GameScene*)table)->fn_803C1560(&target->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
    mMode = 4;
    m48 = arg;
    mTarget = target;
    float r = 5.0f + (target->m5B0 + mChara->m5B0);
    mRadius = 30.0f > r ? 30.0f : r;
}

void RouteWalker_unk::fn_80128A30(const atn::Vector3* pos, const atn::Vector3* dest, int arg, float radius) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table != 0) {
        fn_8012539C();
        fn_80128508(((GameScene*)table)->fn_803C1560(pos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
        mMode = 5;
        m48 = arg;
        m30 = *pos;
        m3C = radius;
        mRadius = 3.0f;
    }
}


void RouteWalker_unk::fn_80128B20(chr::Chara_unk* target, const atn::Vector3* dest, int arg, float radius) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0 || target == 0) {
        return;
    }
    fn_8012539C();
    fn_80128508(((GameScene*)table)->fn_803C1560(&target->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
    mMode = 6;
    m48 = arg;
    mTarget = target;
    mRadius = radius;
}

void RouteWalker_unk::fn_80128C08(chr::Chara_unk* target, const atn::Vector3* dest, int arg, float arg2) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0 || target == 0) {
        return;
    }
    fn_8012539C();
    fn_80128508(((GameScene*)table)->fn_803C1560(&target->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
    mMode = 7;
    m48 = arg;
    mTarget = target;
    mRadius = 30.0f;
    m9C = arg2;
}

// inferred: a x b (name invented)
static inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: line check against the field (name invented)
static inline int LineHit_unk(const atn::Vector3& from, const atn::Vector3& to) {
    return fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0);
}

// inferred: game settings (lbl_8087F0A8)
extern "C" GameConfig_unk* lbl_8087F0A8;

// inferred: nonzero if the goal of the route is in sight: both sides of the character (its radius
// apart) reach the goal without hitting the field
int RouteWalker_unk::fn_80128CF8() {
    if (lbl_8087F0A8->m060 == 0) {
        return 0;
    }
    if (((unsigned int)m48 & 2) == 2 || ((unsigned int)m48 & 4) == 4) {
        return 0;
    }
    RouteTable_unk* table = GetRouteTable_unk();
    atn::Vector3 up(0.0f, mChara->m5B0, 0.0f);
    atn::Vector3 from = mChara->mPos + up;
    atn::Vector3 to;
    switch (m44) {
    case 1:
        atn::VecAssign_unk(to, m10 + up);
        break;
    case 3:
        atn::VecAssign_unk(to, table->FindPlace_unk(mPlace)->mPos + up);
        break;
    case 4:
    case 7:
        if (mPointA <= 0) {
            return 1;
        }
        return 0;
    case 5:
        atn::VecAssign_unk(to, m30 + up);
        break;
    default:
        return 0;
    }
    atn::Vector3 dir = to - from;
    if (PSVECSquareMag(&dir) < 0.0001f) {
        return 1;
    }
    PSVECNormalize(&dir, &dir);
    atn::Vector3 side = Cross_unk(dir, atn::Vector3(0.0f, 1.0f, 0.0f));
    if ((float)__fabs(PSVECSquareMag(&side)) < 1.1920929e-7f) {
        side = Cross_unk(dir, atn::Vector3(0.0f, 0.0f, -1.0f));
        if ((float)__fabs(PSVECSquareMag(&side)) < 1.1920929e-7f) {
            return 1;
        }
    }
    PSVECNormalize(&side, &side);
    side *= mChara->m5B0;
    if (LineHit_unk(from + side, to + side) != 0 || LineHit_unk(from - side, to - side) != 0) {
        return 0;
    }
    return 1;
}

// NONMATCHING: register allocation only: the original keeps the route table in r5 and m44 in r4 (ours the
// other way round). Tried: a local for m44 (before and after the table), the table fetched in case 3, an inline
// place getter, the case order and the m48 flag after the switch.
// inferred: walks straight to the goal of the mode in m44 (the goal is in sight)
void RouteWalker_unk::fn_80129144() {
    RouteTable_unk* table = GetRouteTable_unk();
    switch (m44) {
    case 1:
        mDestPoint = mPointB;
        m4C = m10;
        m58 = m4C - mChara->mPos;
        break;
    case 3:
        mDestPoint = mPointB;
        m4C = table->FindPlace_unk(mPlace)->mPos;
        m58 = m4C - mChara->mPos;
        break;
    case 4:
    case 6:
    case 7: {
        int area = mChara->mC58.mB4;
        if (area > 0 && mTarget->mC58.mB4 > 0 && area != mTarget->mC58.mB4) {
            fn_80127D8C(0, 0, 0);
        } else {
            mDestPoint = mPointB;
            m4C = mTarget->mPos;
            m58 = m4C - mChara->mPos;
        }
        if (m44 == 6) {
            m48 |= 0x20;
        }
        break;
    }
    case 5:
        mDestPoint = mPointB;
        m4C = m30;
        m58 = m4C - mChara->mPos;
        break;
    default:
        fn_80127D8C(0, 0, 0);
        break;
    }
    mMode = m44;
    m94 = 0;
}

// inferred: point of the route tables by id (name invented)
static inline RoutePoint_unk* GetPoint_unk(RouteTable_unk* table, int point) {
    return &table->mPoints[point - 1];
}

CharaSubSeg_unk RouteWalker_unk::fn_80129670(int from, int to) {
    RouteTable_unk* table = GetRouteTable_unk();
    CharaSubSeg_unk seg;
    if (from > 0 && to > 0 && from != to) {
        seg.m00 = GetPoint_unk(table, from)->mPos;
        seg.m0C = GetPoint_unk(table, to)->mPos - seg.m00;
        seg.m18 = GetPoint_unk(table, from)->mRadius;
        seg.m1C = GetPoint_unk(table, to)->mRadius;
    } else if (to > 0) {
        seg.m00 = mChara->mPos;
        seg.m0C = GetPoint_unk(table, to)->mPos - seg.m00;
        seg.m18 = GetPoint_unk(table, to)->mRadius;
        seg.m1C = GetPoint_unk(table, to)->mRadius;
    } else {
        seg = m70;
    }
    return seg;
}
