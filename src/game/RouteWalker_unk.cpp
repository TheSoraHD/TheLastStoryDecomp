#include "game/CharaStatus.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaTask.h"
#include "game/ResHandle.h"

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
    m70 = 0.0f;
    m74 = 0.0f;
    m78 = 0.0f;
    m7C = 0.0f;
    m80 = 0.0f;
    m84 = 1.0f;
    m88 = 0.0f;
    m8C = 0.0f;
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
extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6);
extern "C" unsigned short fn_803CD958(RoutePath_unk* path, int point); // inferred: next point towards `point`
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
struct ColliWorld_unk;
extern "C" ColliWorld_unk* lbl_8087EE98;
extern "C" int fn_8004ECC0(ColliWorld_unk* world, int arg1, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, int arg5, int arg6); // inferred: line check

// inferred: name invented
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
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

void RouteWalker_unk::fn_80128508(int start, const atn::Vector3* dest, int arg) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table != 0 && start > 0) {
        fn_8012539C();
        int end = fn_803C1560(table, dest, mChara->fn_80179D44(), 0, mA0, 1, -1.0f);
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

// NONMATCHING: only the order of the two radius loads (target loads mChara's first into f1).
// Tried: both operand orders, locals, an inline adder, adding 5 first or last.
void RouteWalker_unk::fn_80128930(chr::Chara_unk* target, const atn::Vector3* dest, int arg) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0 || target == 0) {
        return;
    }
    fn_8012539C();
    fn_80128508(fn_803C1560(table, &target->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
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
        fn_80128508(fn_803C1560(table, pos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
        mMode = 5;
        m48 = arg;
        m30 = *pos;
        m3C = radius;
        mRadius = 3.0f;
    }
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

void RouteWalker_unk::fn_801286F0(int link, const atn::Vector3* b, int c) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table != 0 && link > 0) {
        fn_8012539C();
        atn::Vector3 pos;
        int point;
        if (fn_80019E88(lbl_8087EE68, &pos, &point, link, 0) != 0) {
            fn_80128508(fn_803C1560(table, &table->GetLink_unk(link)->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f),
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
    fn_80128508(fn_803C1560(table, &place->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), b, c);
    mMode = 3;
    m48 = c;
    mPlace = id;
    mRadius = 3.0f;
}

void RouteWalker_unk::fn_80128B20(chr::Chara_unk* target, const atn::Vector3* dest, int arg, float radius) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0 || target == 0) {
        return;
    }
    fn_8012539C();
    fn_80128508(fn_803C1560(table, &target->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
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
    fn_80128508(fn_803C1560(table, &target->mPos, mChara->fn_80179D44(), 0, mA0, 1, -1.0f), dest, arg);
    mMode = 7;
    m48 = arg;
    mTarget = target;
    mRadius = 30.0f;
    m9C = arg2;
}
