#include "gmk/Leaf.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/FriendManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" InstData_unk* fn_8009F788(const char* name); // inferred: finds or loads a building model
extern "C" int fn_8009FFCC(InstData_unk* data);         // inferred: nonzero while loading
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;      // inferred: player manager
// inferred: game settings
struct LeafSettings_unk {
    unsigned char m00[0x18];
    int m18; // inferred: draw the gimmicks
};
extern "C" LeafSettings_unk* lbl_8087F0A8;
// inferred: debug menu
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
// inferred: next character of the player and friend lists (0x14AC)
struct LeafCharaNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};

// inferred: debug switches (names invented)
static int sUpdate = 1;
static unsigned int sBlow = 1;

// inferred: random value in [0, 1] and float trigonometry (names invented)
#define Rand01_unk() ((float)(rand() % 1001) / 1000.0f)
inline float Sin_unk(float x) {
    return sin(x);
}
inline float Cos_unk(float x) {
    return cos(x);
}

// inferred: rotation matrix z * y * x from euler angles (as in game/MapPartsModel.cpp; names invented)
inline atn::Matrix3x4& __MakeRotationZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
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
    return m;
}
inline void __MtxRotateZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    Mtx t;
    atn::Matrix3x4 rot;
    PSMTXConcat(m.m, __MakeRotationZYX_unk(rot, r).m, t);
    __MtxCopy(m.m, t);
}
// inferred: scale matrix and matrix product by value (names invented)
inline atn::Matrix3x4 __ScaleMtx_unk(const atn::Vector3& s) {
    atn::Matrix3x4 r;
    PSMTXScale(r.m, s.x, s.y, s.z);
    return r;
}
inline atn::Matrix3x4 __MtxMulV_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}
// inferred: front direction of a character (name invented)
inline atn::Vector3 GetFront_unk(CharaTask* chara) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}

namespace gmk {

GimmickCommon* Leaf::fn_80400728(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) Leaf(parent, id, arg);
    }
    return 0;
}

Leaf::~Leaf() {
    for (unsigned int i = 0; i < mLeaves.mSize; i++) {
        if (mLeaves.mData[i].mInst != 0) {
            delete mLeaves.mData[i].mInst;
        }
    }
}

// NONMATCHING: 99.42% - the index and the offset of the loop are incremented in the other order
int Leaf::vf0C() {
    if (fn_803EC91C() == 0) {
        for (int i = 0; i < mNumLeaf; i++) {
            if (fn_8009FFCC(mLeaves.mData[i].mInst->mData) != 0) {
                return 0;
            }
        }
        fn_80400D08();
        return 1;
    }
    return 0;
}

static void (Leaf::*const sStates[])() = {&Leaf::fn_8040161C, &Leaf::fn_80402764, &Leaf::fn_80401BF0};

// NONMATCHING: 95.08% - stack layout: the rotation matrix is above the temporaries of the three
// __MtxRotate here (the original has it at the bottom, 0x18); registers of the hoisted addresses
void Leaf::vf10() {
    switch (mState) {
    case 0:
        fn_80400D08();
        break;
    case 1: {
        if (0 <= mType && mType < 3) {
            (this->*sStates[mType])();
        }
        mWindCur = (mWindCur - 0.03f < mWindVel) ? mWindVel : mWindCur - 0.03f;
        break;
    }
    }
    for (int i = 0; i < mNumLeaf; i++) {
        if (sUpdate != 0) {
            mLeaves[i].mInst->fn_8009EB20();
            LeafEntry_unk& leaf = mLeaves[i];
            atn::Matrix3x4 rot;
            rot.SetIdentity();
            __MtxRotate(rot.m, 'z', leaf.mRot.z);
            __MtxRotate(rot.m, 'y', leaf.mRot.y);
            __MtxRotate(rot.m, 'x', leaf.mRot.x);
            atn::Matrix3x4 mtx = __MtxMulV_unk(rot, __ScaleMtx_unk(atn::Vector3(2.0f, 2.0f, 2.0f)));
            mtx.m[0][3] = m6C.x + mLeaves[i].mPos.x;
            mtx.m[1][3] = m6C.y + mLeaves[i].mPos.y;
            mtx.m[2][3] = m6C.z + mLeaves[i].mPos.z;
            mLeaves[i].mInst->fn_8009EE30(mtx);
        }
    }
}

// NONMATCHING: 93.73% - the original keeps the 0 stored in m64 in a register copied from the loop
// index (r30) and the offset in r31
void Leaf::vf14() {
    for (int i = 0; i < mNumLeaf; i++) {
        if (lbl_8087F0A8->m18 != 0) {
            atn::ModelInstance* inst = mLeaves.mData[i].mInst;
            if (m9C != 0) {
                inst->m64 = 0;
                inst->fn_8009E6EC(1);
            }
        }
    }
}

// NONMATCHING: 99.90% - registers of the index/offset of the second loop and the operands of the
// fnmsubs of the height
void Leaf::fn_80400D08() {
    atn::Matrix3x4 mtx;
    mtx.SetIdentity();
    __MtxRotateZYX_unk(mtx, m78);
    PSMTXMultVec(mtx.m, &mWindDir, &mWindDir);
    for (int i = 0; i < mNumLeaf; i++) {
        float angle = 0.017453292f * (float)(rand() % 361);
        atn::Vector3 pos(Cos_unk(angle) * (mSize * Rand01_unk()),
                         (float)(rand() % (int)mSize) - 0.5f * mSize,
                         Sin_unk(angle) * (mSize * Rand01_unk()));
        atn::Vector3 rot(0.0f, 0.0f, 0.0f);
        mLeaves[i].mPos = pos;
        mLeaves[i].mRot = rot;
        mLeaves[i].mDirX = 0.0f;
        mLeaves[i].mDirZ = 0.0f;
        mLeaves[i].mSpeed = 0.0f;
    }
    if (mType != 2) {
        for (int i = 0; i < mNumLeaf; i++) {
            float angle = 0.017453292f * (float)(rand() % 361);
            mLeaves[i].mPos.x = Cos_unk(angle) * (mSize * Rand01_unk()) + mWindDir.x * (mFlowLength * Rand01_unk());
            mLeaves[i].mPos.y = mWindDir.y * (mFlowLength * Rand01_unk());
            mLeaves[i].mPos.z = Sin_unk(angle) * (mSize * Rand01_unk()) + mWindDir.z * (mFlowLength * Rand01_unk());
        }
    }
    mState = 1;
}

// NONMATCHING: 99.35% - registers of the size/data of the binary (r29/r30) and of the last loop
void Leaf::vf20() {
    char name[256];
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    memset(name, 0, sizeof(name));
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL_INSTANCE") == 0) {
            const char* path = csv.fn_8005B9CC();
            memcpy(name, path, strlen(path) + 1);
        } else if (strcmp(field, "PARAM") == 0) {
            mType = atoi(csv.fn_8005B9CC());
            mNumLeaf = atoi(csv.fn_8005B9CC());
            mWindVel = mWindCur = fn_800DC288(csv.fn_8005B9CC());
            if (mType == 1) {
                mWindVel = mWindCur = 2.0f;
            }
            mFlowLength = fn_800DC288(csv.fn_8005B9CC());
            mSize = fn_800DC288(csv.fn_8005B9CC());
            mSize = mSize > 0.1f ? mSize : 0.1f;
        }
    } while (csv.fn_8005BBF8() != 0);
    mLeaves.Resize_unk(mNumLeaf, 0);
    for (int i = 0; i < mNumLeaf; i++) {
        mLeaves[i].mInst = new (12, "", "", 0) atn::ModelInstance(fn_8009F788(name));
    }
}

// NONMATCHING: 99.24% - the original gives the index and this higher registers than the offset
void Leaf::fn_8040161C() {
    if (sBlow != 1) {
        return;
    }
    for (int i = 0; i < mNumLeaf; i++) {
        float speed = 10.0f * (mWindCur / 30.0f) * (0.8f * Rand01_unk()) + m120;
        mLeaves[i].mPos += mWindDir * speed;
        atn::Vector3 d = m6C - (m6C + mLeaves[i].mPos);
        if (fn_805F9920(&d) > mFlowLength * mFlowLength) {
            float angle = 0.017453292f * (float)(rand() % 361);
            mLeaves[i].mPos.x = Cos_unk(angle) * (mSize * Rand01_unk());
            mLeaves[i].mPos.y = mSize * Rand01_unk();
            mLeaves[i].mPos.z = Sin_unk(angle) * (mSize * Rand01_unk());
        }
        if (mType == 1 || mLeaves[i].mPos.y > 0.0f) {
            mLeaves[i].mPos.y += -0.1f * (float)(rand() % 10);
        }
        if (mType != 1) {
            mLeaves[i].mPos.y += speed * (0.5f * Rand01_unk());
        }
        mLeaves[i].mRot.x += 0.1f * (float)(rand() % 20) + mWindCur;
        mLeaves[i].mRot.y += 0.1f * (float)(rand() % 20) + mWindCur;
        mLeaves[i].mRot.z += 0.1f * (float)(rand() % 20) + mWindCur;
    }
    if (rand() % 100 == 0) {
        mWindCur += 0.1f;
    }
    if (m128 > 0) {
        m128--;
        if (m120 < m124) {
            m120 += m124 / 30.0f;
        }
    } else if (m120 > 0.0f) {
        m120 *= 0.95f;
        if (m120 < 0.01f) {
            m120 = 0.0f;
        }
    }
}

void Leaf::fn_80401BF0() {
    if (lbl_8087F8A0 != 0) {
        fn_80401E50(lbl_8087F8A0->mHead);
    }
    if (FriendManager::sInstance != 0) {
        for (CharaTask* chara = FriendManager::sInstance->fn_8036554C(); chara != 0;
             chara = ((LeafCharaNode_unk*)chara)->mNext) {
            if (chara->CanAct_unk()) {
                fn_80401E50(chara);
            }
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* chara = EnemyManager::sInstance->mHead; chara != 0; chara = chara->mNext) {
            if (chara->CanAct_unk()) {
                fn_80401E50(chara);
            }
        }
    }
    if (NpcManager::sInstance != 0) {
        for (NpcTask* chara = NpcManager::sInstance->mHead; chara != 0; chara = chara->mNext) {
            if (chara->CanAct_unk()) {
                fn_80401E50(chara);
            }
        }
    }
    fn_804023E0();
}

// NONMATCHING: 99.22% - same register order of index/offset/this as fn_8040161C
void Leaf::fn_80401E50(CharaTask* chara) {
    bool walking = false;
    if (chara->m55C == 6 && 4 <= chara->m560 && chara->m560 < 12) {
        walking = true;
    }
    bool run = walking & (chara->m58C == 1);
    run = run & chara->fn_80158CA4();
    for (int i = 0; i < mNumLeaf; i++) {
        atn::Vector2_unk d((m6C.x + mLeaves[i].mPos.x) - chara->mPos.x, (m6C.z + mLeaves[i].mPos.z) - chara->mPos.z);
        if (d.x * d.x + d.y * d.y < 9.0f) {
            float dist = sqrt(d.x * d.x + d.y * d.y);
            mLeaves[i].mDirX = (m6C.x + mLeaves[i].mPos.x) - chara->mPos.x;
            mLeaves[i].mDirZ = (m6C.z + mLeaves[i].mPos.z) - chara->mPos.z;
            LeafEntry_unk& leaf = mLeaves[i];
            float inv = 1.0f / (float)sqrt(leaf.mDirX * leaf.mDirX + leaf.mDirZ * leaf.mDirZ);
            leaf.mDirX *= inv;
            leaf.mDirZ *= inv;
            mLeaves[i].mSpeed = (1.0f - dist / 3.0f) * (1.9f * Rand01_unk());
        } else if (run) {
            d.x = (m6C.x + mLeaves[i].mPos.x) - (chara->mPos.x + GetFront_unk(chara).x * (5.0f * Rand01_unk() + 10.0f - 2.5f));
            d.y = (m6C.z + mLeaves[i].mPos.z) - (chara->mPos.z + GetFront_unk(chara).z * (5.0f * Rand01_unk() + 10.0f - 2.5f));
            if (d.x * d.x + d.y * d.y < 225.0f) {
                float dist = sqrt(d.x * d.x + d.y * d.y);
                mLeaves[i].mDirX = (m6C.x + mLeaves[i].mPos.x) - chara->mPos.x;
                mLeaves[i].mDirZ = (m6C.z + mLeaves[i].mPos.z) - chara->mPos.z;
                float inv = 1.0f / (float)sqrt(mLeaves[i].mDirX * mLeaves[i].mDirX + mLeaves[i].mDirZ * mLeaves[i].mDirZ);
                mLeaves[i].mDirX = inv * mLeaves[i].mDirX * (2.5f * Rand01_unk() - 1.0f);
                mLeaves[i].mDirZ = inv * mLeaves[i].mDirZ * (2.5f * Rand01_unk() - 1.0f);
                mLeaves[i].mSpeed = (1.0f - dist / 15.0f) * (2.5f * Rand01_unk());
            }
        }
    }
}

// NONMATCHING: 98.79% - same register order of index/offset/this as fn_8040161C
void Leaf::fn_804023E0() {
    for (int i = 0; i < mNumLeaf; i++) {
        if (mLeaves[i].mSpeed > 0.1f || mLeaves[i].mPos.y > 0.01f) {
            mLeaves[i].mPos.x += (4.0f * Rand01_unk() - 1.5f) * (mLeaves[i].mDirX * mLeaves[i].mSpeed);
            mLeaves[i].mPos.z += (4.0f * Rand01_unk() - 1.5f) * (mLeaves[i].mDirZ * mLeaves[i].mSpeed);
            mLeaves[i].mPos.y += mLeaves[i].mSpeed;
            float speed = mLeaves[i].mSpeed - 0.1f;
            mLeaves[i].mSpeed = speed < 0.0f ? 0.01f : speed;
            mLeaves[i].mRot.x += mLeaves[i].mPos.y;
            mLeaves[i].mRot.y += mLeaves[i].mPos.y;
            mLeaves[i].mRot.z += mLeaves[i].mPos.y;
        }
        if (mType == 2) {
            mLeaves[i].mPos.y += -0.1f * (float)(rand() % 10);
        }
        if (mType != 1 && mLeaves[i].mPos.y <= 0.0f) {
            mLeaves[i].mPos.y = 0.0f;
        } else {
            mLeaves[i].mPos.x += mLeaves[i].mDirX * (0.5f * Rand01_unk());
            mLeaves[i].mPos.z += mLeaves[i].mDirZ * (0.5f * Rand01_unk());
        }
    }
}

void Leaf::fn_80402764() {
    fn_8040161C();
}

void Leaf::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", (int*)&mState, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    fn_800874C8(group, "type", &mType, 0, 99, 1, 0, 0);
    fn_800874C8(group, "numleaf", &mNumLeaf, 0, mNumLeaf, 1, 0, 0);
    fn_80087E9C(group, "winddir", &mWindDir, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_8008771C(group, "windvel", &mWindVel, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_8008771C(group, "flowlength", &mFlowLength, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_8008771C(group, "size", &mSize, 0, 0, 0.1f, 9999.0f, 0.1f);
}

float Leaf::vf58() {
    return mSize;
}

} // namespace gmk
