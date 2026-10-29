#include "gmk/PigeonManager.h"
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
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" int rand();
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" void fn_8009076C(atn::Model* model, const atn::Matrix3x4* mtx); // inferred: updates the nodes
// inferred: camera (only the position)
struct PigeonCamera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" PigeonCamera_unk* lbl_8087EFB4;
// inferred: game state (lbl_8087EFA8, see game/CharaTask.cpp)
struct PigeonGameState_unk {
    unsigned char m000[0x218];
    float m218; // inferred: distance beyond which characters are not updated
    float m21C; // inferred: distance beyond which characters are updated every m220 frames
    int m220;
};
extern "C" PigeonGameState_unk* lbl_8087EFA8;
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
// inferred: collision world (lbl_8087EE98)
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004D314(void* world, atn::Vector3* hit, const atn::Vector3* pos, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int attr, float radius); // inferred: sphere sweep
extern "C" double sqrt(double x);
extern "C" double sin(double x);
extern "C" double cos(double x);

// inferred: sound effects (the second one is the flock)
static const char* const sSe[] = {"SE_TWGIM_004", "SE_TWGIM_005"};

inline void __SetTrans3_unk(atn::Matrix3x4& m, float x, float y, float z) {
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
}
// inferred: float wrapper of atan2 (name invented; it loads x before y)
inline float Atan2_unk(float y, float x) {
    return atan2(y, x);
}
#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))
// inferred: rotation matrix z * y * x (as in gmk/Bat.cpp)
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

// inferred: exact equality of two vectors (name invented)
inline bool IsSame_unk(const atn::Vector3& a, const atn::Vector3& b) {
    bool same = false;
    if (a.x == b.x && a.y == b.y && a.z == b.z) {
        same = true;
    }
    return same;
}

namespace gmk {

Pigeon_unk::Pigeon_unk()
    : mModel(0x100, 4), mPos(0.0f, 0.0f, 0.0f), m3E0(0.0f, 0.0f, 0.0f), m3EC(0.0f, 0.0f, 0.0f),
      m3F8(0.0f, 0.0f, 0.0f), mCenter(0.0f, 0.0f, 0.0f), mRadius(0.0f), m414(0.0f), m418(0), m41C(0), mState(0),
      m424(0), mReady(0), mTries(3), m42C(0), m434(0.7f), m438(true) {}

Pigeon_unk::~Pigeon_unk() {}

// NONMATCHING: 99.93% - only .sdata2 order (vf0C differs).
void Pigeon_unk::fn_804048CC() {
    switch (mState) {
    case 1:
        fn_804050C0();
        break;
    case 2:
        mModel.fn_80097C08(0, 1, 1, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        fn_804052EC();
        break;
    case 3:
        mModel.fn_80097C08(0, 0, 1, 0, 1, 0.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        fn_804058A4();
        break;
    }
    mModel.fn_80092A4C();
    atn::Matrix3x4 mtx;
    __MakeRotation_unk(mtx, m3E0);
    __SetTrans3_unk(mtx, mPos.x, mPos.y, mPos.z);
    m42C++;
    int update = 1;
    atn::Vector3 d = lbl_8087EFB4->mPos - mPos;
    float distSq = PSVECSquareMag(&d);
    mModel.mFlags |= 0x40;
    if (distSq > lbl_8087EFA8->m21C * lbl_8087EFA8->m21C) {
        if (distSq > lbl_8087EFA8->m218) {
            update = 0;
        } else if (m42C % (lbl_8087EFA8->m220 * 2) != 0) {
            update = 0;
        }
    }
    if (m438 || update || mState == 3) {
        __MtxCopy(mModel.mMtx, mtx.m);
        mModel.m54 = MaxScale_unk(mtx);
        mModel.fn_80097E80(1);
        mModel.CalcMatrixStack(atn::ModelNodeCallback_unk());
        m438 = false;
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
void Pigeon_unk::fn_80404E70() {
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
    m42C = rand();
    m414 = 0.0f;
    m418 = rand() % 60 + 30;
    m424 = 0;
    if (rand() % 2 == 0) {
        m41C = 0;
    } else {
        m41C = 1;
    }
}

// NONMATCHING: 99.78% - only .sdata2 order. The local `d` keeps the add out of a fused fmadds.
void Pigeon_unk::fn_804050C0() {
    if (m424 == 0) {
        switch (rand() % 2) {
        case 0:
            mModel.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
            mModel.mTracks[0].m0C = 0.6f;
            m424 = 1;
            break;
        case 1:
            mModel.fn_80097C08(0, 3, 1, 0, 1, 0.0f, 0.1f);
            mModel.mTracks[0].m0C = 0.5f;
            m424 = 1;
            break;
        }
    }
    fn_80405A14((CharaTask*)lbl_8087F8A0->mHead);
    if (m418 == 0) {
        m424 = 0;
        if (rand() % 2 == 0) {
            m414 = 0.1f;
            int t = rand() % 60 + 30;
            mState = 2;
            m418 = t;
            if (rand() % 2 == 0) {
                m41C = 0;
            } else {
                m41C = 1;
            }
        } else {
            m418 = rand() % 60 + 30;
        }
        float d = 0.017453292f * (float)(rand() % 10 - 5);
        m3E0.y = m3E0.y + d;
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    }
    m418--;
}

// NONMATCHING: 98.7% - only .sdata2/.rodata order (both atan2 go through Atan2_unk).
void Pigeon_unk::fn_80405A14(CharaTask* player) {
    atn::Vector3 d = mPos - player->mPos;
    float distSq = PSVECSquareMag(&d);
    if (distSq < 225.0f) {
        m414 = 2.0f;
        mState = 3;
        d.y = 15.0f;
        PSVECNormalize(&d, &d);
        m3EC = d;
        m3E0.y = Atan2_unk(m3EC.x, m3EC.z);
        m418 = 300;
        mSound = fn_800C344C(sSe[0], &mPos, 0, -1, 1.0f);
    } else if (distSq < 400.0f) {
        m414 = 0.5f;
        mState = 2;
        d.y = 0.0f;
        PSVECNormalize(&d, &d);
        atn::Vector3 rot = d.ToRotation_unk();
        float a = atn::WrapAngle_unk(rot.y - m3E0.y);
        if ((a < 0.0f ? -a : a) > 0.08726646f) {
            if (a > 0.0f) {
                a = 0.08726646f;
                m41C = 1;
            } else {
                m41C = 0;
                a = -0.08726646f;
            }
        }
        Mtx r;
        PSMTXRotRad(r, 'y', a);
        PSMTXMultVec(r, &m3EC, &m3EC);
        m3E0.y = Atan2_unk(m3EC.x, m3EC.z);
    }
}

// NONMATCHING: 97.3% - stack slots of the temporaries of the neighbour test and of the direction.
void Pigeon_unk::fn_804052EC() {
    atn::Vector3 next = mPos;
    next.x = m3EC.x * m414 + next.x;
    next.z = m3EC.z * m414 + next.z;
    m3F8 = mPos;
    if ((next - mCenter).Length_unk() > mRadius) {
        if (m41C == 1) {
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
    Pigeon_unk* other = mOwner->FindNear_unk(pos);
    if (other != 0) {
        atn::Vector3 op = other->mPos;
        atn::Vector3 d = op - mPos;
        d.y = 0.0f;
        atn::Vector3 n = d;
        PSVECNormalize(&n, &n);
        float dot = m3EC.x * d.x + m3EC.z * d.z;
        if (dot > (float)cos(sAngle_unk)) {
            if (m41C == 1) {
                float d = 0.017453292f * (-1.0f * (float)(rand() % 5));
            m3E0.y = m3E0.y + d;
            } else {
                float d = 0.017453292f * (float)(rand() % 5);
            m3E0.y = m3E0.y + d;
            }
            mPos = m3F8;
        }
    }
    fn_80405A14((CharaTask*)lbl_8087F8A0->mHead);
    if (m418 == 0) {
        m424 = 0;
        if (rand() % 2 == 0) {
            int t = rand() % 60 + 30;
            mState = 1;
            m418 = t;
        } else {
            m414 = 0.1f;
            m418 = rand() % 60 + 30;
        }
        if (m41C == 1) {
            float d = 0.017453292f * (float)(rand() % 15 - 15);
            m3E0.y = m3E0.y + d;
        } else {
            float d = 0.017453292f * (float)(rand() % 15);
            m3E0.y = m3E0.y + d;
        }
        m3EC.x = (float)sin(m3E0.y);
        m3EC.z = (float)cos(-m3E0.y);
    }
    m418--;
}

// NONMATCHING: 91.5% - stack slots of the sweep temporaries (pos/hit swapped, hit kept in a saved
// register in the original).
void Pigeon_unk::fn_804058A4() {
    if (m418 >= 0) {
        m418--;
    }
    if (m418 < 0) {
        fn_800CB5C8(&mSound, 15, 0);
        mState = 0;
        return;
    }
    m3F8 = mPos;
    mPos.x += m3EC.x * m414;
    mPos.y += m3EC.y * m414;
    mPos.z += m3EC.z * m414;
    if (mSound.mPtr != 0) {
        fn_800CB6E4(&mSound, &mPos);
    }
    atn::Vector3 pos(mPos.x, mPos.y, mPos.z);
    atn::Vector3 tmp = m3EC * m414;
    atn::Vector3 move = tmp;
    atn::Vector3 hit;
    if (fn_8004D314(lbl_8087EE98, &hit, &pos, &move, 0x80000000, 0, 0, 5.0f) == 0) {
        hit = pos;
    }
    mPos = hit;
}

GimmickCommon* PigeonManager::fn_80405DD4(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) PigeonManager(parent, desc);
    }
    return 0;
}

PigeonManager::PigeonManager(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mState(0) {
    mCount = mDesc->m20;
    mPigeons.Resize_unk(mCount, 0);
    m78 = atn::Vector3(0.0f, mDesc->mRotY, 0.0f);
    mCenter = m6C = mDesc->mPos;
    mRadius = mDesc->m40;
    mLoaded = false;
    ((void)0); // inferred: an assertion compiled out (keeps the constructor out of the factory)
}

PigeonManager::~PigeonManager() {
    fn_800CB5C8(&mSound, 0, 0);
}

Pigeon_unk* PigeonManager::FindNear_unk(const atn::Vector3& pos) {
    for (int i = 0; i < mCount; i++) {
        mTmp = mPigeons[i].mPos;
        atn::Vector3 t = mTmp;
        bool same = false;
        if (pos.x == mTmp.x && pos.y == mTmp.y && pos.z == mTmp.z) {
            same = true;
        }
        if (!same && (mTmp - pos).Length_unk() < 6.0f) {
            return &mPigeons[i];
        }
    }
    return 0;
}

// NONMATCHING: 93.89% - 16 bytes smaller frame and one saved register less than the original
// (the placement check is probably written differently).
int PigeonManager::vf0C() {
    if (fn_803EC91C() == 0) {
        if (!mLoaded) {
            for (int i = 0; i < mCount; i++) {
                mPigeons[i].mOwner = this;
                fn_8008AD4C(&mPigeons[i].mModel, mModelName, 0);
                for (int j = 0; j < 4; j++) {
                    fn_80097A88(&mPigeons[i].mModel, mMotionId[j], mMotionName[j]);
                }
                atn::Vector3 center = mCenter;
                mPigeons[i].mCenter = center;
                mPigeons[i].mRadius = mRadius;
            }
            mLoaded = true;
        }
        if (mLoaded) {
            int done = 1;
            for (int i = 0; i < mCount; i++) {
                Pigeon_unk* p = &mPigeons[i];
                int ready;
                if (p->mReady == 0) {
                    if (p->mModel.fn_8008B140() == 0) {
                        p->fn_80404E70();
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

// inferred: distance between the player and a position (name invented)
static inline float PlayerDist_unk(const atn::Vector3& pos) {
    return (((CharaTask*)lbl_8087F8A0->mHead)->mPos - pos).Length_unk();
}

// NONMATCHING: 99.93% - only the .sdata2 order.
// The second loop reuses `active` as its counter: that gives the register of the original.
void PigeonManager::vf10() {
    if (mDesc->m7C == 0) {
        return;
    }
    int active = 0;
    if (mState == 0) {
        for (int i = 0; i < mCount; i++) {
            if (mPigeons[(unsigned int)i].mState != 0) {
                mPigeons[(unsigned int)i].fn_804048CC();
                if (mPigeons[(unsigned int)i].mState != 3) {
                    active++;
                }
            }
        }
        if (lbl_8087F8A0 != 0) {
            float dist = PlayerDist_unk(m6C) - mRadius;
            if (active >= 2) {
                if (dist < 50.0f) {
                    if (mSound.mPtr == 0) {
                        mSound = fn_800C344C(sSe[1], &m6C, 0, -1, 1.0f);
                        fn_800CB518(&mSound, 30);
                    }
                } else if (dist > 80.0f) {
                    fn_800CB5C8(&mSound, 30, 0);
                }
            } else {
                fn_800CB5C8(&mSound, 30, 0);
            }
        }
    } else if (mState == 2) {
        for (active = 0; active < mCount; active++) {
            mPigeons[(unsigned int)active].fn_80404E70();
        }
        mState = 0;
    }
    float dist = PlayerDist_unk(mCenter);
    switch (mState) {
    case 0:
        if (dist > 500.0f) {
            mState = 1;
        }
        break;
    case 1:
        if (dist <= 400.0f) {
            mState = 2;
        }
        break;
    }
}

// The unsigned subscript gives the loop increment order of the original
void PigeonManager::vf14() {
    if (mDesc->m7C != 0 && mState == 0) {
        for (int i = 0; i < mCount; i++) {
            if (mPigeons[(unsigned int)i].mState != 0 && lbl_8087F0A8->m18 != 0) {
                mPigeons[(unsigned int)i].mModel.fn_8008CD60();
            }
        }
    }
}

void PigeonManager::vf20() {
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
