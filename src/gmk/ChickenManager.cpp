#include "gmk/ChickenManager.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion
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

// inferred: sound effects (calm and scared flock)
static const char* const sSe[] = {"SE_TWGIM_007", "SE_TWGIM_008"};

namespace gmk {

Chicken_unk::Chicken_unk()
    : mModel(0x100, 0x20), mPos(0.0f, 0.0f, 0.0f), m3E0(0.0f, 0.0f, 0.0f), m3EC(0.0f, 0.0f, 0.0f),
      m3F8(0.0f, 0.0f, 0.0f), mCenter(0.0f, 0.0f, 0.0f), mRadius(0.0f), m414(0.0f), m418(0), m41C(0), m420(0),
      mState(0), mTries(3), m42C(0), m430(0), mReady(0) {}

Chicken_unk::~Chicken_unk() {}

GimmickCommon* ChickenManager::fn_8040D1D0(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) ChickenManager(parent, desc);
    }
    return 0;
}

// NONMATCHING: 99.94% - only .sdata2 order (the Chicken_unk functions are not decompiled yet).
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
        mTmp = mChickens[i].mPos;
        if (!(pos.x == mTmp.x && pos.y == mTmp.y && pos.z == mTmp.z) && (mTmp - pos).Length_unk() < 6.0f) {
            return &mChickens[i];
        }
    }
    return 0;
}

// NONMATCHING: 88.71% - as gmk::PigeonManager::vf0C (16 bytes smaller frame, one saved register less).
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

// NONMATCHING: 98.54% - `state == 3 || state == 4` is folded into one unsigned compare here (tried
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
