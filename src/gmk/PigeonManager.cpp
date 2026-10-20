#include "gmk/PigeonManager.h"
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
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: sound effects (the second one is the flock)
static const char* const sSe[] = {"SE_TWGIM_004", "SE_TWGIM_005"};

namespace gmk {

Pigeon_unk::Pigeon_unk()
    : mModel(0x100, 4), mPos(0.0f, 0.0f, 0.0f), m3E0(0.0f, 0.0f, 0.0f), m3EC(0.0f, 0.0f, 0.0f),
      m3F8(0.0f, 0.0f, 0.0f), mCenter(0.0f, 0.0f, 0.0f), mRadius(0.0f), m414(0.0f), m418(0), m41C(0), mState(0),
      m424(0), mReady(0), mTries(3), m42C(0), m434(0.7f), m438(true) {}

Pigeon_unk::~Pigeon_unk() {}

GimmickCommon* PigeonManager::fn_80405DD4(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) PigeonManager(parent, desc);
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
        if (!(pos.x == mTmp.x && pos.y == mTmp.y && pos.z == mTmp.z) && (mTmp - pos).Length_unk() < 6.0f) {
            return &mPigeons[i];
        }
    }
    return 0;
}

// NONMATCHING: 88.73% - 16 bytes smaller frame and one saved register less than the original
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

// NONMATCHING: 99.52% - the loop counter is incremented before the element offset in the
// original; .sdata2 order (the Pigeon_unk functions are not decompiled yet).
void PigeonManager::vf10() {
    if (mDesc->m7C == 0) {
        return;
    }
    int active = 0;
    if (mState == 0) {
        for (int i = 0; i < mCount; i++) {
            if (mPigeons[i].mState != 0) {
                mPigeons[i].fn_804048CC();
                if (mPigeons[i].mState != 3) {
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
        for (int i = 0; i < mCount; i++) {
            mPigeons[i].fn_80404E70();
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

// NONMATCHING: 99.45% - same loop increment order as vf10.
void PigeonManager::vf14() {
    if (mDesc->m7C != 0 && mState == 0) {
        for (int i = 0; i < mCount; i++) {
            if (mPigeons[i].mState != 0 && lbl_8087F0A8->m18 != 0) {
                mPigeons[i].mModel.fn_8008CD60();
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
