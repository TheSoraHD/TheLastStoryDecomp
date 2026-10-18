#include "game/CharaStatus.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaTask.h"
#include "game/ResHandle.h"

// inferred: debug menu entries; the last two arguments are a callback and its argument
typedef void (*MenuCallback_unk)(void* arg);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step,
                            MenuCallback_unk callback, void* arg);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, MenuCallback_unk callback,
                            void* arg, float min, float max, float step);
extern "C" void fn_8008771C(void* group, const char* name, float* value, MenuCallback_unk callback, void* arg,
                            float min, float max, float step);

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

CharaLook_unk::CharaLook_unk()
    : mChara(0), mType(0), mTarget(0), mOffset(0.0f, 0.0f, 0.0f), mRot(0.0f, 0.0f, 0.0f),
      mLimit(0.62831855f, 0.7853982f, 0.0f), mNeck(0), m34(0), m35(0), m36(1), mSpeed(0.3f), mEyeType(0),
      mEyeTarget(0), mEyeOffset(0.0f, 0.0f, 0.0f), mEyeSpeed(0.3f) {
    m3C = 0.0f;
    m40 = 0.0f;
    m44 = 0.0f;
    m48 = 0.0f;
    mEyeRot[0].x = 0.0f;
    mEyeRot[0].y = 0.0f;
    mEyeRot[0].z = 0.0f;
    mEyeRot[1].x = 0.0f;
    mEyeRot[1].y = 0.0f;
    mEyeRot[1].z = 0.0f;
}

CharaLook_unk::~CharaLook_unk() {}

void CharaLook_unk::fn_801298EC(chr::Chara_unk* chara) {
    mChara = chara;
    mNeck = ((Model_unk*)&chara->mMotion)->fn_80092814("neck", 0);
}

void CharaLook_unk::fn_80129930(const atn::Vector3& pos, float speed) {
    mOffset = pos;
    mType = 2;
    mSpeed = speed;
}

void CharaLook_unk::fn_80129954(const atn::Vector3& pos, float speed) {
    mOffset = pos;
    mType = 3;
    mSpeed = speed;
}

void CharaLook_unk::fn_80129978(Model_unk* model, const char* node, const atn::Vector3& offset, float speed) {
    if (node != 0) {
        mTarget = (float*)model->GetNodeMtx_unk(model->fn_80092814(node, 0));
        mOffset = offset;
        mType = 1;
        mSpeed = speed;
    } else {
        mTarget = (float*)model->mMtx;
        mOffset = offset;
        mType = 1;
        mSpeed = speed;
    }
}

void CharaLook_unk::fn_80129A48(float speed) {
    mTarget = 0;
    mOffset.x = 0.0f;
    mOffset.y = 0.0f;
    mOffset.z = 0.0f;
    mType = 0;
    mSpeed = speed;
}

void CharaLook_unk::fn_8012ABD0(const atn::Vector3& pos, float speed) {
    mEyeOffset = pos;
    mEyeType = 3;
    mSpeed = speed;
}

void CharaLook_unk::fn_8012ABF4(const atn::Vector3& pos, float speed) {
    mEyeOffset = pos;
    mEyeType = 2;
    mEyeSpeed = speed;
}

void CharaLook_unk::fn_8012AC18(float* target, const atn::Vector3& pos, float speed) {
    mEyeTarget = target;
    mEyeOffset = pos;
    mEyeType = 1;
    mEyeSpeed = speed;
}

void CharaLook_unk::fn_8012AC40(float speed) {
    mEyeTarget = 0;
    mEyeOffset.x = 0.0f;
    mEyeOffset.y = 0.0f;
    mEyeOffset.z = 0.0f;
    mEyeType = 0;
    mEyeSpeed = speed;
}

void CharaLook_unk::fn_80129A6C() {
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    atn::Vector3 limit(mLimit);
    m3C = 0.0f;
    m40 = 0.0f;
    m44 = 0.0f;
    m48 = 0.0f;
    switch (m36) {
    case 0:
        m3C = 1.0f;
        break;
    case 1:
        m3C = 0.5f;
        m40 = 0.5f;
        break;
    case 2:
        m3C = 0.4f;
        m40 = 0.4f;
        m44 = 0.2f;
        limit *= 1.5f;
        break;
    case 3:
        m3C = 0.35f;
        m40 = 0.35f;
        m44 = 0.1f;
        limit *= 2.0f;
        break;
    }
    if (mType == 3) {
        rot = mOffset;
        if (atn::Abs_unk(rot.y) > mLimit.y) {
            rot.y = rot.y > 0.0f ? mLimit.y : -mLimit.y;
        }
        if (atn::Abs_unk(rot.x) > mLimit.x) {
            rot.x = rot.x > 0.0f ? mLimit.x : -mLimit.x;
        }
    } else if (mType != 0) {
        atn::Vector3 target(mOffset);
        if (mType == 1 && mTarget != 0) {
            target += ((atn::Matrix3x4*)mTarget)->GetTrans_unk();
        }
        atn::Vector3 dir;
        int node = mNeck;
        if (node < 0) {
            rot = *mChara->GetRot_unk();
        } else {
            dir = target - ((atn::Matrix3x4*)mChara->GetModel_unk()->GetNodeMtx_unk(node))->GetTrans_unk();
            if (dir.IsZero_unk()) {
                rot = *mChara->GetRot_unk();
            } else {
                dir.Normalize_unk();
                rot = dir.ToRotation_unk();
            }
        }
        rot.Apply_unk(atn::WrapAngle_unk);
        rot.y -= mChara->GetRot_unk()->y;
        rot.Apply_unk(atn::WrapAngle_unk);
        rot *= 0.7f;
        if (rot.x > 0.0f) {
            rot.x *= 0.8f;
        }
        if (atn::Abs_unk(rot.y) > limit.y) {
            rot.y = rot.y > 0.0f ? limit.y : -limit.y;
        }
        if (atn::Abs_unk(rot.x) > limit.x) {
            rot.x = rot.x > 0.0f ? limit.x : -limit.x;
        }
    }
    mRot = atn::Lerp_unk(mRot, rot, mSpeed);
    m34 = 0;
    if (mType == 3) {
        m34 = 1;
    } else if (atn::Abs_unk(mRot.x) > 0.001f || atn::Abs_unk(mRot.y) > 0.001f) {
        m34 = 1;
    }
    atn::Vector3 eyeMax(atn::DegToRad_unk(10.0f), atn::DegToRad_unk(25.0f), 0.0f);
    atn::Vector3 eyeMin(atn::DegToRad_unk(-15.0f), atn::DegToRad_unk(-25.0f), 0.0f);
    atn::Vector3 eyeRot[2];
    if (mEyeType == 3) {
        eyeRot[0] = mEyeOffset;
        eyeRot[1] = mEyeOffset;
    } else if (mEyeType != 0) {
        atn::Vector3 target(mEyeOffset);
        if (mEyeType == 1 && mEyeTarget != 0) {
            target += ((atn::Matrix3x4*)mEyeTarget)->GetTrans_unk();
        }
        int nodes[2];
        nodes[0] = mChara->GetModel_unk()->fn_80092814("eye_L", 0);
        nodes[1] = mChara->GetModel_unk()->fn_80092814("eye_R", 0);
        int node;
        for (int i = 0; i < 2; i++) {
            atn::Vector3 dir;
            node = nodes[i];
            if (node < 0) {
                eyeRot[i] = *mChara->GetRot_unk();
            } else {
                dir = target - ((atn::Matrix3x4*)mChara->GetModel_unk()->GetNodeMtx_unk(node))->GetTrans_unk();
                if (dir.IsZero_unk()) {
                    eyeRot[i] = *mChara->GetRot_unk();
                } else {
                    dir.Normalize_unk();
                    eyeRot[i] = dir.ToRotation_unk();
                }
            }
            eyeRot[i].Apply_unk(atn::WrapAngle_unk);
            atn::Vector3 front(0.0f, 0.0f, 10.0f);
            front.fn_80011410(
                ((atn::Matrix3x4*)mChara->GetModel_unk()->GetNodeMtx_unk("head"))->GetRotation_unk());
            atn::Vector3 head = front.ToRotation_unk();
            head.Apply_unk(atn::WrapAngle_unk);
            eyeRot[i].y -= head.y;
            eyeRot[i].Apply_unk(atn::WrapAngle_unk);
            eyeRot[i] *= 0.9f;
            eyeRot[i].y = (eyeMax.y < (eyeMin.y > eyeRot[i].y ? eyeMin.y : eyeRot[i].y))
                              ? eyeMax.y
                              : (eyeMin.y > eyeRot[i].y ? eyeMin.y : eyeRot[i].y);
            eyeRot[i].x = (eyeMax.x < (eyeMin.x > eyeRot[i].x ? eyeMin.x : eyeRot[i].x))
                              ? eyeMax.x
                              : (eyeMin.x > eyeRot[i].x ? eyeMin.x : eyeRot[i].x);
        }
    } else {
        eyeRot[0].Set_unk(0.0f, 0.0f, 0.0f);
        eyeRot[1].Set_unk(0.0f, 0.0f, 0.0f);
    }
    m35 = 0;
    for (int i = 0; i < 2; i++) {
        mEyeRot[i] = atn::Lerp_unk(mEyeRot[i], eyeRot[i], mEyeSpeed);
        if (mEyeType == 3) {
            m35 = 1;
        } else if (atn::Abs_unk(mEyeRot[i].x) > 0.001f || atn::Abs_unk(mEyeRot[i].y) > 0.001f) {
            m35 = 1;
        }
    }
}

// inferred: debug menu callback, applies the new values at once
void fn_8012AC64(CharaLook_unk* look) {
    float speed = look->mSpeed;
    float eyeSpeed = look->mEyeSpeed;
    look->mSpeed = 1.0f;
    look->mEyeSpeed = 1.0f;
    look->fn_80129A6C();
    fn_8000D430((Model_unk*)&look->mChara->mMotion, boost::function<void()>());
    look->mSpeed = speed;
    look->mEyeSpeed = eyeSpeed;
}

void CharaLook_unk::fn_8012AD20(void* menu) {
    void* group = fn_8008937C(menu, "Sight");
    fn_800874C8(group, "type", &mType, 0, 3, 1, (MenuCallback_unk)fn_8012AC64, this);
    fn_80087E9C(group, "offset", &mOffset, (MenuCallback_unk)fn_8012AC64, this, -100000.0f, 100000.0f, 1.0f);
    fn_8008771C(group, "speed", &mSpeed, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_800874C8(group, "eyeType", &mEyeType, 0, 3, 1, (MenuCallback_unk)fn_8012AC64, this);
    fn_80087E9C(group, "eyeOffset", &mEyeOffset, (MenuCallback_unk)fn_8012AC64, this, -100000.0f, 100000.0f, 1.0f);
    fn_8008771C(group, "eyeSpeed", &mEyeSpeed, 0, 0, 0.0f, 1.0f, 0.1f);
}

CharaStatusBase::CharaStatusBase() {
    memset(m1A8, 0, sizeof(m1A8));
    memset(this, 0, sizeof(*this));
}


extern "C" double pow(double x, double y);

int CharaStatus::fn_8012B028() {
    int level = mParamA.m78 > 69 ? 69 : mParamA.m78;
    return (int)(50.0f * (float)pow((float)level, 2.0)) - *(int*)&m1A8[4];
}

// NONMATCHING: 98.1% - register allocation only: the original keeps zero in r30 and the loop
// pointers of m2D0 in r29/r28; ours uses r29 for zero and r28/r30 for the pointers.
CharaStatus::CharaStatus()
    : m240(0), m248(0), m2CC(0), mChara(0), m2F4(0.0f), m2F8(0.0f), m2FC(1.0f), m300(0), mNumEntries(0),
      mNumRefs(0), m428(0.0f), m42C(0.0f), m430(0), m434(0.0f), m438(0.0f) {
    memset(m24C, 0, sizeof(m24C));
}

CharaStatus::~CharaStatus() {}

int CharaStatus::fn_8012E534() {
    return (m0C >> 12) & 1;
}

int CharaStatus::fn_8012F3D8() {
    if (mChara == 0 || !mChara->mFlag28) {
        return 0;
    }
    if (m42C < 0.0f) {
        return 0;
    }
    if (m42C < 3.0f) {
        return 1;
    }
    if (m42C < 5.0f) {
        return 2;
    }
    return 3;
}

void CharaStatus::fn_8013310C(unsigned int flags, int both) {
    m0C |= flags;
    if (both) {
        m10 |= flags;
    }
}

int CharaStatus::fn_801333A4(unsigned int mask) {
    for (int i = 0; i < 32; i++) {
        if ((mask & (1 << i)) == (1 << i)) {
            return m24C[i];
        }
    }
    return -1;
}

CharaStatusEntry_unk* CharaStatus::fn_80133EE8(int id) {
    CharaStatusEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mNumEntries; i++, entry++) {
        if (entry->mId == id) {
            return entry;
        }
    }
    return 0;
}

int CharaStatus::fn_80133F18(int id, int sub) {
    int sum = 0;
    CharaStatusEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mNumEntries; i++, entry++) {
        if ((id < 0 || entry->mId == id) && (sub < 0 || entry->mSub == sub)) {
            sum += entry->mValue;
        }
    }
    return sum;
}

int CharaStatus::fn_80133F6C(int id, int sub) {
    int count = 0;
    CharaStatusEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mNumEntries; i++, entry++) {
        if ((id < 0 || entry->mId == id) && (sub < 0 || entry->mSub == sub)) {
            count++;
        }
    }
    return count;
}

CharaStatusRef_unk* CharaStatus::fn_80134134(int id) {
    CharaStatusRef_unk* ref = mRefs;
    for (unsigned int i = 0; i < mNumRefs; i++, ref++) {
        if (ref->mEntry->mId == id) {
            return ref;
        }
    }
    return 0;
}

int CharaStatus::fn_80134168(int id, int sub) {
    int sum = 0;
    CharaStatusRef_unk* ref = mRefs;
    for (unsigned int i = 0; i < mNumRefs; i++, ref++) {
        CharaStatusEntry_unk* entry = ref->mEntry;
        if (entry->mId == id && (sub < 0 || entry->mSub == sub)) {
            sum += entry->mValue;
        }
    }
    return sum;
}

int CharaStatus::fn_801341B8(int id, int sub) {
    int sum = 0;
    CharaStatusRef_unk* ref = mRefs;
    for (unsigned int i = 0; i < mNumRefs; i++, ref++) {
        CharaStatusEntry_unk* entry = ref->mEntry;
        if (entry->mId == id && (sub < 0 || entry->mSub == sub)) {
            sum += entry->m10;
        }
    }
    return sum;
}

bool CharaStatus::fn_80134208() {
    int type = mChara->m5C->m122;
    bool result = true;
    if (type != 1 && type != 0) {
        result = false;
    }
    return result;
}

bool CharaStatus::fn_80134234() {
    return mChara->m5C->m11C == 2;
}

bool CharaStatus::fn_80134250() {
    return mChara->m5C->m122 == 4;
}

bool CharaStatus::fn_80134270() {
    return mChara->m5C->m122 == 2;
}

bool CharaStatus::fn_80134290() {
    return mChara->m5C->m122 == 3;
}

bool CharaStatus::fn_801342B0() {
    return mChara->m5C->m122 == 3;
}
