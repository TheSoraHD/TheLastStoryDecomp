#include "gmk/Lemon.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix
extern "C" char lbl_807C88A0[0x40];     // inferred: name buffer
extern "C" InstData_unk* fn_8009F788(const char* name); // inferred: finds or loads a building model
extern "C" int fn_8009FFCC(InstData_unk* data);         // inferred: nonzero while loading
// inferred: game settings
struct LemonSettings_unk {
    unsigned char m00[0x30];
    int mFps; // 0x30: inferred: frames per second
};
extern "C" LemonSettings_unk* lbl_8087F0A8;
// inferred: collision manager (lbl_8087EE98) with a buffer of results at 0x4008
struct LemonColli_unk {
    unsigned char m00[0x4008];
    CollResult_unk mResults[0x100]; // 0x4008
};
extern "C" LemonColli_unk* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: sphere test, returns the number of results
extern "C" int fn_8004CCA0(void* colli, CollResult_unk* results, int max, const void* sphere, unsigned int mask,
                           void* ignore, int arg7);
// inferred: object that sends message 3 (its velocity at 0x574)
struct LemonHitter_unk {
    unsigned char m00[0x574];
    atn::Vector3 m574;
};

// inferred: rotation matrix z * y * x (same as GimmickCommon.cpp)
inline void __MakeRotation_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}
inline void __SetTrans_unk(atn::Matrix3x4& m, float x, float y, float z) {
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
}
// inferred: sphere passed to the collision test
struct LemonSphere_unk {
    atn::Vector3 mPos;
    float mRadius;
};
// inferred: segment from the base to the top of the object (unused)
struct LemonSegment_unk {
    LemonSegment_unk() {}
    LemonSegment_unk(const atn::Vector3& a, const atn::Vector3& b) : mA(a), mB(b) {}
    atn::Vector3 mA;
    atn::Vector3 mB;
};

namespace gmk {

GimmickCommon* Lemon::fn_8041524C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) Lemon(parent, id, arg);
    }
    return 0;
}

GimmickCommon* Lemon::fn_8041537C(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) Lemon(parent, desc->mId, desc->mArg);
    }
    return 0;
}

// inferred: model of a gimmick id (name invented)
static InstData_unk* GetData_unk(int id) {
    sprintf(lbl_807C88A0, "gm%03d_%03d.building", id / 1000, id % 1000);
    return fn_8009F788(lbl_807C88A0);
}

Lemon::Lemon(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mInst(GetData_unk(id)), mVel(0.0f, 0.0f, 0.0f), mRadius(3.0f), mBounce(0.3f),
      mFriction(0.8f), m194(0), m198(-1) {
    m54 = 0;
    mInst.mData->mBC = 2.0f;
}

Lemon::~Lemon() {}

int Lemon::vf0C() {
    return fn_803EC91C() == 0;
}

void Lemon::vf10() {
    if (m54 == 0) {
        Start();
    }
    m6C.y += 0.5f;
    atn::Matrix3x4 mtx = lbl_807C7060;
    atn::Matrix3x4 rot;
    __MakeRotation_unk(rot, m78);
    Mtx scale;
    PSMTXScale(scale, m90.x, m90.y, m90.z);
    PSMTXConcat(scale, mtx.m, mtx.m);
    PSMTXConcat(rot.m, mtx.m, mtx.m);
    atn::Matrix3x4 offset;
    __MakeRotation_unk(offset, m84);
    PSMTXConcat(offset.m, mtx.m, mtx.m);
    __SetTrans_unk(mtx, m6C.x, m6C.y, m6C.z);
    mInst.fn_8009EE30(mtx);
    m6C.y -= 0.5f;
    if (m54 == 2) {
        fn_80415F6C();
    }
}

void Lemon::vf14() {
    if (m54 == 1) {
        return;
    }
    mInst.m64 = 0;
    mInst.fn_8009E6EC(1);
}

int Lemon::vf1C() {
    int ret = 0;
    if (fn_8009FFCC(mInst.mData) != 0) {
        ret = 1;
    }
    return ret;
}

void Lemon::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "PARAM") == 0) {
            mRadius = fn_800DC288(csv.fn_8005B9CC());
            mBounce = fn_800DC288(csv.fn_8005B9CC());
            mFriction = fn_800DC288(csv.fn_8005B9CC());
            m194 = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Lemon::Start() {
    m54 = 2;
    atn::Matrix3x4 mtx = lbl_807C7060;
    atn::Matrix3x4 rot;
    __MakeRotation_unk(rot, m78);
    Mtx scale;
    PSMTXScale(scale, m90.x, m90.y, m90.z);
    PSMTXConcat(scale, mtx.m, mtx.m);
    PSMTXConcat(rot.m, mtx.m, mtx.m);
    atn::Matrix3x4 offset;
    __MakeRotation_unk(offset, m84);
    PSMTXConcat(offset.m, mtx.m, mtx.m);
    __SetTrans_unk(mtx, m6C.x, m6C.y, m6C.z);
    mInst.fn_8009EE30(mtx);
}

int Lemon::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 0:
        Start();
        break;
    case 1:
    case 2:
    case 4:
        m54 = msg->mType;
        break;
    case 3:
        m54 = 2;
        mVel.Assign_unk(((LemonHitter_unk*)msg->m10)->m574);
        mVel.y = 2.0f;
        mVel *= 1.0f + 0.2f * (rand() % 1001 / 1000.0f) - 0.1f;
        break;
    }
    return m54;
}

// NONMATCHING: 79.6% - register allocation and scheduling: the original copies the second vector of
// the (unused) segment through its own address register (0xB4) and computes the reflected velocity
// in another order; tried two named vectors, the segment before/after the gravity and other
// forms of the negated projection
void Lemon::fn_80415F6C() {
    if (m54 != 2) {
        return;
    }
    mVel.y += -98.1f / (float)(lbl_8087F0A8->mFps * lbl_8087F0A8->mFps);
    LemonSegment_unk seg(m6C, m6C + atn::Vector3(0.0f, mRadius, 0.0f));
    CollResult_unk res;
    LemonSphere_unk sphere;
    sphere.mPos = m6C;
    sphere.mPos.y += mRadius;
    sphere.mRadius = mRadius;
    if (fn_80416268(&res, &sphere.mPos, &mVel, mRadius) != 0) {
        atn::Vector3 newPos = res.m10 - atn::Vector3(0.0f, mRadius, 0.0f);
        atn::Vector3 d = newPos - (m6C + mVel);
        if (fn_805F9920(&d) > 0.0f) {
            PSVECNormalize(&d, &d);
        }
        float dot = PSVECDotProduct(&d, &mVel);
        atn::Vector3 proj = d * dot;
        atn::Vector3 tang = mVel - proj;
        mVel = atn::Vector3(-proj.x, -proj.y, -proj.z) * mBounce + tang * mFriction;
        m6C = newPos;
    } else {
        m6C += mVel;
    }
}

// NONMATCHING: 85.7% - stack layout and register allocation: the original has three dead float
// stores (0x98, 0xC, 0x1C) of unknown temporaries and a 16-byte bigger frame
int Lemon::fn_80416268(CollResult_unk* out, const atn::Vector3* pos, const atn::Vector3* vel, float radius) {
    int hit = 0;
    int count = 0;
    CollResult_unk* results = lbl_8087EE98->mResults;
    void* obj = 0;
    unsigned int flags = 0;
    atn::Vector3 v = *vel;
    LemonSphere_unk sphere;
    sphere.mPos = *pos;
    sphere.mRadius = radius;
    if (fn_805F9920(&v) > 0.25f * (radius * radius)) {
        CollResult_unk res;
        atn::Vector3 to = *pos + v;
        if (fn_8004ED34(lbl_8087EE98, &res, pos, &to, 0xE, 0, 0) != 0) {
            v = (res.m04 - *pos) + res.m1C * radius * 0.5f;
        }
    }
    do {
        atn::Vector3 prev = sphere.mPos;
        sphere.mPos += v;
        int n = fn_8004CCA0(lbl_8087EE98, results, 0x100, &sphere, 0xE, 0, 0);
        if (n == 0) {
            break;
        }
        float minDist = 1e16f;
        int best = -1;
        CollResult_unk* r = results;
        for (int i = 0; i < n; i++) {
            atn::Vector3 d = prev - r->m04;
            float dist = fn_805F9920(&d);
            if (minDist > dist) {
                minDist = dist;
                best = i;
            }
            r++;
        }
        if (best < 0) {
            break;
        }
        CollResult_unk* e = &results[best];
        hit = 1;
        count++;
        v = sphere.mPos - e->m10;
        sphere.mPos = e->m10 + e->m1C * 0.001f;
        float dot = PSVECDotProduct(&v, &e->m1C);
        v -= e->m1C * dot;
    } while (count <= 1);
    if (out != 0) {
        out->m10 = sphere.mPos;
        out->m04 = sphere.mPos;
        out->m34 = obj;
        out->m3C = flags;
    }
    return hit;
}

void Lemon::vf6C(void* menu) {}

int Lemon::vf60() {
    return m54 == 2;
}

} // namespace gmk
