#include "gmk/ChickenManager.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames);          // inferred: fades the sound in
extern "C" void fn_800CB6B0(atn::SoundRef* sound, int arg, float distance); // inferred: sets the distance
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" int rand();
extern "C" double sqrt(double x);
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" void fn_8009076C(atn::Model* model, const atn::Matrix3x4* mtx); // inferred: updates the nodes
// inferred: camera (only the position)
struct ChickenCamera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" ChickenCamera_unk* lbl_8087EFB4;
// inferred: game state (lbl_8087EFA8, see game/CharaTask.cpp)
struct ChickenGameState_unk {
    unsigned char m000[0x218];
    float m218; // inferred: distance beyond which characters are not updated
    float m21C; // inferred: distance beyond which characters are updated every m220 frames
    int m220;
};
extern "C" ChickenGameState_unk* lbl_8087EFA8;

// inferred: sound effects (calm and scared flock)
static const char* const sSe[] = {"SE_TWGIM_007", "SE_TWGIM_008"};

// inferred: float wrapper of atan2 (name invented; it loads x before y)
inline float Atan2_unk(float y, float x) {
    return atan2(y, x);
}
#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))
// inferred: rotation matrix z * y * x (as in gmk/PigeonManager.cpp)
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
inline void __SetTrans3_unk(atn::Matrix3x4& m, float x, float y, float z) {
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
}
inline atn::Vector3 GetCol_unk(const atn::Matrix3x4& m, int i) {
    return atn::Vector3(m.m[0][i], m.m[1][i], m.m[2][i]);
}
// inferred: largest scale axis of a matrix (as in gmk/Bat.cpp)
inline float MaxScale_unk(const atn::Matrix3x4& mtx) {
    atn::Vector3 s;
    atn::Vector3 az = GetCol_unk(mtx, 2);
    float sz = PSVECMag(&az);
    atn::Vector3 ay = GetCol_unk(mtx, 1);
    float sy = PSVECMag(&ay);
    atn::Vector3 ax = GetCol_unk(mtx, 0);
    float sx = PSVECMag(&ax);
    s.x = sx;
    s.y = sy;
    s.z = sz;
    return __MAX_unk(s.x, __MAX_unk(s.y, s.z));
}

static const double sAngle_unk = 0.7853981852531433; // inferred: 45 degrees (float precision)

namespace gmk {

Chicken_unk::Chicken_unk()
    : mModel(0x100, 0x20), mPos(0.0f, 0.0f, 0.0f), m3E0(0.0f, 0.0f, 0.0f), m3EC(0.0f, 0.0f, 0.0f),
      m3F8(0.0f, 0.0f, 0.0f), mCenter(0.0f, 0.0f, 0.0f), mRadius(0.0f), m414(0.0f), m418(0), m41C(0), m420(0),
      mState(0), mTries(3), m42C(0), m430(0), mReady(0) {}

Chicken_unk::~Chicken_unk() {}

// NONMATCHING: 99.91% - only .sdata2 order.
void Chicken_unk::fn_8040B668() {
    switch (mState) {
    case 1:
        fn_8040BE88();
        break;
    case 2:
        mModel.fn_80097C08(0, 0, 1, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.2f;
        m42C = 0;
        fn_8040C058();
        break;
    case 3:
        mModel.fn_80097C08(0, 0, 1, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 3.0f;
        fn_8040C058();
        break;
    case 4:
        mModel.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        fn_8040C610();
        break;
    }
    mModel.mFlags |= 0x40;
    atn::Matrix3x4 mtx;
    __MakeRotation_unk(mtx, m3E0);
    __SetTrans3_unk(mtx, mPos.x, mPos.y, mPos.z);
    mModel.fn_80092A4C();
    int update = 1;
    atn::Vector3 d = lbl_8087EFB4->mPos - mPos;
    float distSq = fn_805F9920(&d);
    if (distSq > lbl_8087EFA8->m21C * lbl_8087EFA8->m21C) {
        if (distSq > lbl_8087EFA8->m218 * lbl_8087EFA8->m218) {
            update = 0;
        } else if (m418 % (lbl_8087EFA8->m220 * 2) != 0) {
            update = 0;
        }
    }
    if (update || mState == 4) {
        __MtxCopy(mModel.mMtx, mtx.m);
        mModel.m54 = MaxScale_unk(mtx);
        mModel.fn_80097E80(1);
        mModel.CalcMatrixStack(atn::ModelNodeCallback_unk());
    } else {
        atn::Matrix3x4 inv;
        __MtxCopy(inv.m, mModel.mMtx);
        PSMTXInverse(inv.m, inv.m);
        __MtxCopy(mModel.mMtx, mtx.m);
        mModel.m54 = MaxScale_unk(mtx);
        fn_8009076C(&mModel, &inv);
    }
}

// NONMATCHING: 99.73% - only .sdata2 order.
void Chicken_unk::fn_8040BC38() {
    mState = 1;
    float r = (float)sqrt(mRadius * mRadius / 2.0f);
    mPos.x = mCenter.x + (2.0f * r * ((float)(rand() % 1001) / 1000.0f) - r);
    mPos.y = mCenter.y;
    mPos.z = mCenter.z + (2.0f * r * ((float)(rand() % 1001) / 1000.0f) - r);
    m3E0.y = 0.017453292f * (float)(rand() % 360);
    m3EC.x = (float)sin(m3E0.y);
    m3EC.z = (float)cos(-m3E0.y);
    PSVECNormalize(&m3EC, &m3EC);
    mModel.m34C = 1;
    mModel.mTracks[0].m20 = 1.0f;
    mModel.mTracks[0].m0C = 1.0f;
    mModel.mTracks[0].m08 = 0.0f;
    mModel.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
    mModel.mFlags = (mModel.mFlags | 0x100) & ~8;
    m414 = 0.0f;
    m41C = rand() % 60 + 30;
    m430 = 0;
    if (rand() % 2 == 0) {
        m420 = 0;
    } else {
        m420 = 1;
    }
    m418 = 0;
}

// NONMATCHING: 99.83% - only .sdata2 order.
void Chicken_unk::fn_8040BE88() {
    if (m430 == 0) {
        mModel.fn_80097C08(0, 1, 1, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 0.6f;
        m430 = 1;
    }
    fn_8040CD2C((CharaTask*)lbl_8087F8A0->mHead);
    if (m41C == 0) {
        m430 = 0;
        if (rand() % 2 == 0) {
            m414 = 0.1f;
            int t = rand() % 60 + 30;
            mState = 2;
            m41C = t;
            if (rand() % 2 == 0) {
                m420 = 0;
            } else {
                m420 = 1;
            }
        } else {
            m41C = rand() % 60 + 30;
        }
        float d = 0.017453292f * (float)(rand() % 10 - 5);
        m3E0.y = m3E0.y + d;
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    }
    m41C--;
}

// NONMATCHING: 99.61% - only .sdata2/.rodata order.
void Chicken_unk::fn_8040CD2C(CharaTask* player) {
    atn::Vector3 d = mPos - player->mPos;
    if (m42C > 150 || fn_805F9920(&d) < 25.0f) {
        if (mState != 4) {
            m41C = 33;
        }
        m414 = 1.0f;
        mState = 4;
        if (m420 == 1) {
            float a = 0.017453292f * (-1.0f * (float)(rand() % 10));
            m3E0.y = m3E0.y + a;
        } else {
            float a = 0.017453292f * (float)(rand() % 10);
            m3E0.y = m3E0.y + a;
        }
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    } else if (fn_805F9920(&d) < 225.0f && mState != 4) {
        m414 = 0.5f;
        mState = 3;
        m42C++;
    } else if (fn_805F9920(&d) < 400.0f && mState != 4) {
        m414 = 0.3f;
        mState = 3;
        d.y = 0.0f;
        PSVECNormalize(&d, &d);
        atn::Vector3 rot = d.ToRotation_unk();
        float a = atn::WrapAngle_unk(rot.y - m3E0.y);
        if (atn::Abs_unk(a) > 0.08726646f) {
            if (a > 0.0f) {
                a = 0.08726646f;
                m420 = 1;
            } else {
                m420 = 0;
                a = -0.08726646f;
            }
        }
        Mtx r;
        PSMTXRotRad(r, 'y', a);
        PSMTXMultVec(r, &m3EC, &m3EC);
        m3E0.y = Atan2_unk(m3EC.x, m3EC.z);
        m42C++;
    }
}

// NONMATCHING: 97.3% - same body as gmk::Pigeon_unk::fn_804052EC (stack slots).
void Chicken_unk::fn_8040C058() {
    atn::Vector3 next = mPos;
    next.x = m3EC.x * m414 + next.x;
    next.z = m3EC.z * m414 + next.z;
    m3F8 = mPos;
    if ((next - mCenter).Length_unk() > mRadius) {
        if (m420 == 1) {
            float d = 0.017453292f * (-1.0f * (float)(rand() % 5));
            m3E0.y = m3E0.y + d;
        } else {
            float d = 0.017453292f * (float)(rand() % 5);
            m3E0.y = m3E0.y + d;
        }
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    } else {
        mPos = next;
    }
    atn::Vector3 pos = mPos;
    Chicken_unk* other = mOwner->FindNear_unk(pos);
    if (other != 0) {
        atn::Vector3 op = other->mPos;
        atn::Vector3 d = op - mPos;
        d.y = 0.0f;
        atn::Vector3 n = d;
        PSVECNormalize(&n, &n);
        float dot = m3EC.x * d.x + m3EC.z * d.z;
        if (dot > (float)cos(sAngle_unk)) {
            if (m420 == 1) {
                float d = 0.017453292f * (-1.0f * (float)(rand() % 5));
            m3E0.y = m3E0.y + d;
            } else {
                float d = 0.017453292f * (float)(rand() % 5);
            m3E0.y = m3E0.y + d;
            }
            mPos = m3F8;
        }
    }
    fn_8040CD2C((CharaTask*)lbl_8087F8A0->mHead);
    if (m41C == 0) {
        m430 = 0;
        if (rand() % 2 == 0) {
            int t = rand() % 60 + 30;
            mState = 1;
            m41C = t;
        } else {
            m414 = 0.1f;
            m41C = rand() % 60 + 30;
        }
        if (m420 == 1) {
            float d = 0.017453292f * (float)(rand() % 15 - 15);
            m3E0.y = m3E0.y + d;
        } else {
            float d = 0.017453292f * (float)(rand() % 15);
            m3E0.y = m3E0.y + d;
        }
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    }
    m41C--;
}

// NONMATCHING: 97.7% - stack slots of the temporaries (as gmk::Pigeon_unk::fn_804052EC).
void Chicken_unk::fn_8040C610() {
    atn::Vector3 next = mPos;
    next.x = m3EC.x * m414 + next.x;
    next.z = m3EC.z * m414 + next.z;
    m3F8 = mPos;
    if ((next - mCenter).Length_unk() > mRadius) {
        if (m420 == 1) {
            float d = 0.017453292f * (-1.0f * (float)(rand() % 5));
            m3E0.y = m3E0.y + d;
        } else {
            float d = 0.017453292f * (float)(rand() % 5);
            m3E0.y = m3E0.y + d;
        }
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    } else {
        mPos = next;
    }
    atn::Vector3 pos = mPos;
    Chicken_unk* other = mOwner->FindNear_unk(pos);
    if (other != 0) {
        atn::Vector3 op = other->mPos;
        atn::Vector3 d = op - mPos;
        d.y = 0.0f;
        atn::Vector3 n = d;
        PSVECNormalize(&n, &n);
        float dot = m3EC.x * d.x + m3EC.z * d.z;
        if (dot > (float)cos(sAngle_unk)) {
            if (m420 == 1) {
                float d = 0.017453292f * (-1.0f * (float)(rand() % 5));
            m3E0.y = m3E0.y + d;
            } else {
                float d = 0.017453292f * (float)(rand() % 5);
            m3E0.y = m3E0.y + d;
            }
            mPos = m3F8;
        }
    }
    CharaTask* player = (CharaTask*)lbl_8087F8A0->mHead;
    fn_8040CD2C(player);
    atn::Vector3 dp(player->mPos.x - mPos.x, 0.0f, player->mPos.z - mPos.z);
    PSVECNormalize(&dp, &dp);
    float dot2 = m3EC.x * dp.x + m3EC.z * dp.z;
    if (dot2 > (float)cos(sAngle_unk)) {
        if (m420 == 1) {
            float d = 0.017453292f * (-1.0f * (float)(rand() % 10));
            m3E0.y = m3E0.y + d;
        } else {
            float d = 0.017453292f * (float)(rand() % 10);
            m3E0.y = m3E0.y + d;
        }
        mPos = m3F8;
    }
    if (m41C == 0) {
        m42C = 0;
        m430 = 0;
        if (rand() % 2 == 0) {
            int t = rand() % 60 + 30;
            mState = 1;
            m41C = t;
        } else {
            m414 = 0.1f;
            int t = rand() % 60 + 30;
            mState = 2;
            m41C = t;
            if (rand() % 2 == 0) {
                m420 = 0;
            } else {
                m420 = 1;
            }
        }
        if (m420 == 1) {
            float d = 0.017453292f * (float)(rand() % 15 - 15);
            m3E0.y = m3E0.y + d;
        } else {
            float d = 0.017453292f * (float)(rand() % 15);
            m3E0.y = m3E0.y + d;
        }
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    }
    m41C--;
}

GimmickCommon* ChickenManager::fn_8040D1D0(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) ChickenManager(parent, desc);
    }
    return 0;
}

ChickenManager::ChickenManager(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mState(0), mSoundDist(0.0f), mScared(0),
      mScaredTimer(0) {
    mCount = mDesc->m20;
    mChickens.Resize_unk(mCount, 0);
    m78 = atn::Vector3(0.0f, mDesc->mRotY, 0.0f);
    mCenter = m6C = mDesc->mPos;
    mRadius = mDesc->m40;
    mLoaded = false;
    mSoundDist = 50.0f + mRadius;
}

ChickenManager::~ChickenManager() {}

Chicken_unk* ChickenManager::FindNear_unk(const atn::Vector3& pos) {
    for (int i = 0; i < mCount; i++) {
        atn::Vector3 t;
        t = mTmp = mChickens[i].mPos;
        bool same = false;
        if (pos.x == mTmp.x && pos.y == mTmp.y && pos.z == mTmp.z) {
            same = true;
        }
        if (!same && (mTmp - pos).Length_unk() < 6.0f) {
            return &mChickens[i];
        }
    }
    return 0;
}

// NONMATCHING: 93.89% - as gmk::PigeonManager::vf0C (16 bytes smaller frame, one saved register less).
int ChickenManager::vf0C() {
    if (fn_803EC91C() == 0) {
        if (!mLoaded) {
            for (int i = 0; i < mCount; i++) {
                mChickens[i].mOwner = this;
                fn_8008AD4C(&mChickens[i].mModel, mModelName, 0);
                for (int j = 0; j < 3; j++) {
                    fn_80097A88(&mChickens[i].mModel, mMotionId[j], mMotionName[j]);
                }
                atn::Vector3 center = mCenter;
                mChickens[i].mCenter = center;
                mChickens[i].mRadius = mRadius;
            }
            mLoaded = true;
        }
        if (mLoaded) {
            int done = 1;
            for (int i = 0; i < mCount; i++) {
                Chicken_unk* p = &mChickens[i];
                int ready;
                if (p->mReady == 0) {
                    if (p->mModel.fn_8008B140() == 0) {
                        p->fn_8040BC38();
                        atn::Vector3 pos = p->mPos;
                        if (p->mOwner->FindNear_unk(pos) != 0 && p->mTries > 0) {
                            p->mTries--;
                            ready = 0;
                        } else {
                            p->mReady = 1;
                            ready = 1;
                        }
                    } else {
                        ready = 0;
                    }
                } else {
                    ready = 1;
                }
                if (!ready) {
                    done = 0;
                }
            }
            if (done) {
                return 1;
            }
        }
    }
    return 0;
}

void ChickenManager::PlaySe_unk(int index) {
    mSound = fn_800C344C(sSe[index], &m6C, 0, -1, 1.0f);
    fn_800CB518(&mSound, 15);
    fn_800CB6B0(&mSound, 1, mSoundDist);
}

// inferred: distance between the player and a position (name invented)
static inline float PlayerDist_unk(const atn::Vector3& pos) {
    return (((CharaTask*)lbl_8087F8A0->mHead)->mPos - pos).Length_unk();
}

// NONMATCHING: 98.75% - `state == 3 || state == 4` is folded into one unsigned compare here (tried
// reversed operands and a switch); loop increment order as in gmk::PigeonManager; .sdata2 order.
void ChickenManager::vf10() {
    if (mDesc->m7C == 0) {
        if (mSound.mPtr != 0) {
            fn_800CB5C8(&mSound, 0, 0);
        }
        return;
    }
    if (mState == 0) {
        int scared = 0;
        for (int i = 0; i < mCount; i++) {
            mChickens[(unsigned int)i].fn_8040B668();
            if (mChickens[(unsigned int)i].mState == 3 || mChickens[(unsigned int)i].mState == 4) {
                scared++;
            }
        }
        if (scared > mCount / 2) {
            mScaredTimer = 30;
            if (mScared == 0) {
                fn_800CB5C8(&mSound, 15, 0);
                PlaySe_unk(1);
                mScared = 1;
            }
        } else {
            if (mScaredTimer > 0) {
                mScaredTimer--;
            }
            if (mScaredTimer == 0 && mScared != 0) {
                fn_800CB5C8(&mSound, 15, 0);
                PlaySe_unk(0);
                mScared = 0;
            }
        }
        if (mSound.mPtr == 0) {
            PlaySe_unk(0);
        }
    } else if (mState == 1) {
        if (mSound.mPtr != 0) {
            fn_800CB5C8(&mSound, 0, 0);
        }
    } else if (mState == 2) {
        for (int i = 0; i < mCount; i++) {
            mChickens[(unsigned int)i].fn_8040BC38();
        }
        mState = 0;
    }
    float dist = PlayerDist_unk(mCenter);
    switch (mState) {
    case 0:
        if (dist > 300.0f) {
            mState = 1;
        }
        break;
    case 1:
        if (dist <= 200.0f) {
            mState = 2;
        }
        break;
    }
}

// The unsigned subscript gives the loop increment order of the original
void ChickenManager::vf14() {
    if (mDesc->m7C != 0 && mState == 0) {
        for (int i = 0; i < mCount; i++) {
            Chicken_unk& c = mChickens[(unsigned int)i];
            if (lbl_8087F0A8->m18 != 0) {
                c.mModel.fn_8008CD60();
            }
        }
    }
}

void ChickenManager::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    int n = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            strcpy(mModelName, csv.fn_8005B9CC());
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(mMotionName[n], csv.fn_8005B9CC());
            mMotionId[n] = fn_802180A8(csv.fn_8005B9CC());
            n++;
        }
    } while (csv.fn_8005BBF8() != 0);
}

} // namespace gmk
