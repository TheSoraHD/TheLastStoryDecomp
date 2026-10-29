#include "game/EnvSeManager.h"
#include "game/EnvSeBase.h"
#include "game/GameConfig_unk.h"
#include "atn/Memory.h"
#include "atn/SoundManager.h"
#include "std/string.h"

// Everything below is inferred from EnvSeManager (0x8035DF48..0x80360C5C); names are invented.

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float PSVECSquareMag(const atn::Vector3* v);

// inferred: sound engine (lbl_8087EE90)
struct EnvSeEngine_unk;
extern "C" EnvSeEngine_unk* lbl_8087EE90;
extern "C" int fn_80049B2C(EnvSeEngine_unk* engine, unsigned int id);         // inferred: the sound is missing
extern "C" unsigned int fn_80049B74(EnvSeEngine_unk* engine, const char* name); // inferred: sound id of a name
extern "C" void fn_8004A1D4(EnvSeEngine_unk* engine, int reverb);              // inferred: sets the reverb
// inferred: accessor of the engine (weak copy 0x8035FC00, called when the inline budget runs out)
inline EnvSeEngine_unk* GetEngine_unk() {
    return lbl_8087EE90;
}

// inferred: sound functions
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" int fn_800CB58C(atn::SoundRef* sound);                      // inferred: playing
extern "C" int fn_800CB538(atn::SoundRef* sound);                      // inferred: playing
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int frames, float volume);
extern "C" void fn_800CB69C(atn::SoundRef* sound, int frames, float lpf);
extern "C" void fn_800CB6B0(atn::SoundRef* sound, int arg, float distance);  // inferred: sets the distance
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
extern "C" int fn_800CB6F8(atn::SoundRef* sound);                            // inferred: sound id
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames);               // inferred: fades the sound in
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int fn_800C3184(const char* group); // inferred: loads a sound group
extern "C" int fn_800C31E4();                  // inferred: a sound group is loading
extern "C" void fn_800C31EC();                 // inferred: releases the sound group
extern "C" int fn_805A38F0(const char* name, int a, int b); // inferred: the sound is ready
// inferred: streamed sounds (lbl_8087F498)
struct EnvSeStream_unk;
extern "C" EnvSeStream_unk* lbl_8087F498;
extern "C" atn::SoundRef fn_803E907C(EnvSeStream_unk* mgr, const char* name, const void* pos, int a, int b,
                                     float volume);
extern "C" atn::SoundRef fn_803E9608(EnvSeStream_unk* mgr, const char* name, int a, int b, float volume);
// inferred: loading sounds (db/MapEnvSeTable_unk.cpp)
extern "C" LoadingEnvSe_unk* fn_80214ED4(int a, int b, int c, int d);
extern "C" LoadingEnvSe_unk* fn_80214F38(unsigned int se);
extern "C" bool fn_80215018(LoadingEnvSe_unk* entry, char* buf);
extern "C" bool fn_80215084(LoadingEnvSe_unk* entry, char* buf);

// inferred: field and its event flags (lbl_8087F430)
struct EnvSeField_unk;
extern "C" EnvSeField_unk* lbl_8087F430;
extern "C" int fn_80370174(EnvSeField_unk* field, int id);
extern "C" int fn_80370A78(EnvSeField_unk* field, int id);
extern "C" void fn_80370320(EnvSeField_unk* field, int event, int arg2, int arg3);
struct EnvSeGroup_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
    int m50;
};
extern "C" EnvSeGroup_unk* fn_80373148(EnvSeField_unk* field);

// inferred: game settings (fn_800F52F8 returns lbl_8087F0A8; weak copy of another file)
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" inline GameConfig_unk* fn_800F52F8() {
    return lbl_8087F0A8;
}
// inferred: debug menus enabled (weak copy 0x8035FBF8)
inline int IsDebugMenu_unk(GameConfig_unk* config) {
    return config->m07C;
}

// inferred: debug drawing and camera
extern "C" void* lbl_8087EEB0;
extern "C" void* lbl_8087EFB4;
extern "C" atn::Matrix3x4 lbl_807C7030; // inferred: identity matrix
extern "C" void fn_800629F0(void* draw, const atn::Matrix3x4* base, const atn::Vector3* size, float arg,
                            unsigned int color, const atn::Matrix3x4* mtx); // inferred: box
extern "C" void fn_800616C0(void* draw, float x, float y, float z, float w, float h, const char* text,
                            unsigned int color, float f6, int a, int b, int c); // inferred: text
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen

// inferred: debug menu entries
typedef void (*EnvSeMenuCallback_unk)(void* arg);
extern "C" void fn_8008771C(void* group, const char* name, float* value, EnvSeMenuCallback_unk callback, void* arg,
                            float min, float max, float step);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step,
                            EnvSeMenuCallback_unk callback, void* arg);

// inferred: shape test of the collision library (as in game/EnvSeBase.cpp)
struct EnvSeListener_unk {
    atn::Vector3 mCenter;
    float mRadius;
};
extern "C" int fn_80051CD8(const EnvSeListener_unk* sphere, const void* box); // inferred: sphere in box

extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task

EnvSeManager* EnvSeManager::fn_8035DF48(atn::Task* parent) {
    if (parent != 0 && lbl_8087F418 == 0) {
        lbl_8087F418 = new ((MemoryTag)12, "", "", 0) EnvSeManager(parent);
    }
    return lbl_8087F418;
}

EnvSeManager::EnvSeManager(atn::Task* parent)
    : atn::Task(parent), mState(0), m6C(0), m70(0), m74(0), m78(0), m7C(0), m80(0), m84(0), m88(0), mCur(0),
      mNext(0), mBgmIndex(0), mBgmState(0), m184(1.0f), m188(1.0f), m18C(0), m190(0.35f), m198(0), m19C(0),
      m1A0(0), m1A4(0), m1AC(0) {
    m1AC = m1A8.Load("EnvSE");
    if (m1AC != 0) {
        fn_800874C8(m1AC, "DebugLevel", &m1A4, 0, 99, 1, 0, 0);
        fn_8008771C(m1AC, "ReduceVolume", &m190, 0, 0, 0.0f, 2.0f, 0.1f);
    }
    mBgm[0].m0C = 1;
    mBgm[1].m0C = 1;
    SwapBgm_unk();
}

EnvSeManager::~EnvSeManager() {
    for (unsigned int i = 0; i < (unsigned int)mRecords.mSize; i++) {
        fn_800CB5C8(&mRecords.mData[i].mSound, 0, 0);
    }
    for (unsigned int i = 0; i < 2; i++) {
        mBgm[i].fn_8004B1EC(0);
    }
    mRecords.Free_unk();
    mAreas.Free_unk();
    lbl_8087F418 = 0;
}

int EnvSeManager::vf0C() {
    return fn_800D3FA4() != 0;
}

void EnvSeManager::vf10() {
    if (m19C != 0 && fn_800CB58C(&m194) == 0) {
        m194.Reset();
        fn_800C31EC();
        m19C = 0;
    }
    fn_8035EA2C();
    fn_8035ECCC();
}

// NONMATCHING: 99.0% - register allocation of the loop (counter, offset and `current` take other
// non-volatile registers)
void EnvSeManager::vf14() {
    if (m1A4 > 0) {
        for (unsigned int i = 0; i < (unsigned int)mAreas.mSize; i++) {
            bool current = mAreas[i].mId == m84;
            EnvSeArea_unk* area = &mAreas[i];
            fn_800629F0(lbl_8087EEB0, &lbl_807C7030, &area->mSize, 0.0f, current ? 0xFFFF80FF : 0xFF7F407F,
                        &area->mMtx);
            atn::Vector3 screen;
            atn::Vector3 pos = area->mMtx.GetTrans_unk();
            fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
            if (0.0f < screen.z && screen.z < 1.0f) {
                char text[0x100];
                sprintf(text, "Area[%d] %s\0", area->mId, area->mData.mName);
                fn_800616C0(lbl_8087EEB0, screen.x, screen.y, 0.0f, 12.0f, 12.0f, text,
                            current ? 0xFFFFFFFF : 0xFF7F7F7F, 0.0f, 1, 1, 0);
            }
        }
    }
}

int EnvSeManager::fn_8035E730() {
    if (mState == 2) {
        if (mLoader.fn_80473F50() != 0) {
            return 1;
        }
        fn_8035F3D4();
        mLoader.fn_80473F88();
        mState = 3;
        return 1;
    } else if (mState == 3) {
        if (fn_800D3FA4() == 0) {
            return 1;
        }
        mState = 4;
        return 0;
    }
    return 0;
}

void EnvSeManager::fn_8035E7CC() {
    if (m74 <= 0) {
        mState = 5;
        for (unsigned int i = 0; i < (unsigned int)mRecords.mSize; i++) {
            if (mRecords.mData[i].mTask != 0) {
                mRecords.mData[i].mTask->fn_800D246C(0);
            }
        }
        m6C = 1;
    }
}

void EnvSeManager::fn_8035E858() {
    mState = 1;
    for (unsigned int i = 0; i < (unsigned int)mRecords.mSize; i++) {
        fn_80360504(i, 60);
    }
    m6C = 0;
    m88 = m84;
    m84 = -1;
    if (lbl_8087F0A8->m158 == 0) {
        mCur->fn_8004B1EC(60);
        mCur->mSound.Reset();
        int reverb = mCurReq.mReverb;
        mCurReq = EnvSeBgmReq_unk();
        mCurReq.mReverb = reverb;
    }
    if ((unsigned int)(mBgmState - 1) <= 1) {
        if (mNext != 0) {
            mNext->fn_8004B1EC(0);
            mNext->mSound.Reset();
        }
        mBgmState = 0;
    }
}

// NONMATCHING: 99.4% - the original reads mBgmState again for the `== 2` test (only a volatile read
// reproduces it, not used)
void EnvSeManager::fn_8035EA2C() {
    if (m74 > 0) {
        m74--;
        fn_8035E7CC();
    }
    if (m6C == 1 && (mBgmState == 0 || mBgmState == 2) && m7C == 0) {
        EnvSeArea_unk* best = 0;
        float bestDist = 1e16f;
        for (unsigned int i = 0; i < (unsigned int)mAreas.mSize; i++) {
            EnvSeArea_unk* area = &mAreas.mData[i];
            if (area->mData.m44 != 0 && lbl_8087F430 != 0) {
                int value;
                if (area->mData.m48 == 1) {
                    value = fn_80370174(lbl_8087F430, area->mData.m4C);
                } else {
                    value = fn_80370A78(lbl_8087F430, area->mData.m4C);
                }
                if (value == area->mData.m50) {
                    area->m5C = 1;
                } else {
                    area->m5C = 0;
                }
            } else {
                area->m5C = 1;
            }
            if (area->m5C != 0 && lbl_8087EFE8 != 0) {
                atn::Vector3 listener = lbl_8087EFE8->mListener.mPos;
                atn::Vector3 d = listener - area->mMtx.GetTrans_unk();
                float dist = PSVECSquareMag(&d);
                if (bestDist > dist) {
                    EnvSeListener_unk sphere;
                    sphere.mCenter = listener;
                    sphere.mRadius = 2.5f;
                    if (fn_80051CD8(&sphere, &area->mSize) != 0) {
                        bestDist = dist;
                        best = area;
                    }
                }
            }
        }
        if (best != 0) {
            if (m84 != best->mId || (mCur->mSound.mPtr == 0 && mNext->mSound.mPtr == 0)) {
                fn_8035FF94(best->mId, 60, 1);
            }
        } else if (m84 != 0) {
            fn_8035FF94(0, 60, 1);
        }
    }
}

void EnvSeManager::fn_8035ECCC() {
    if (mBgmState == 1) {
        if (mCurReq.mSoundId != mNextReq.mSoundId) {
            if (mNextReq.mSoundId > 0 && mNextReq.m3C != 0 && m7C == 0) {
                if (fn_805A38F0(mNextReq.mName, 2, 1) != 0) {
                    mBgmState = 2;
                    lbl_8087EFE8->m34D0 = 2;
                    mNext->fn_8004AE84(mNextReq.mName, 0);
                    lbl_8087EFE8->m34D0 = 0;
                }
            } else {
                mBgmState = 3;
                mCur->fn_8004B1EC(mNextReq.mFade);
            }
        } else {
            mBgmState = 6;
            float volume = mNextReq.mVolume;
            fn_800CB5B4(&mCur->mSound, mNextReq.mFade, volume);
            float lpf = mNextReq.mLPF;
            fn_800CB69C(&mCur->mSound, mNextReq.mFade, lpf);
            fn_803601E4(mNextReq.mReverb);
            mPrevReq = mCurReq;
            mCurReq = mNextReq;
        }
    } else if (mBgmState == 2) {
        if (mNext->fn_8004B0E4() == 0) {
            if (mNextReq.m3C != 0) {
                mBgmState = 5;
                mCur->fn_8004B1EC(mNextReq.mFade);
            } else {
                mBgmState = 4;
            }
            mNext->fn_8004B158(mNextReq.mVolume, mNextReq.mFade);
            fn_800CB69C(&mNext->mSound, 0, mNextReq.mLPF);
            fn_803601E4(mNextReq.mReverb);
            mPrevReq = mCurReq;
            mCurReq = mNextReq;
            SwapBgm_unk();
        }
    } else if (mBgmState == 3) {
        if (mCur->mSound.mPtr == 0 || fn_800CB538(&mCur->mSound) == 0) {
            mCur->mSound.Reset();
            if (mNextReq.mSoundId > 0 && m7C == 0) {
                if (fn_805A38F0(mNextReq.mName, 2, 0) != 0) {
                    mBgmState = 2;
                    lbl_8087EFE8->m34D0 = 2;
                    mNext->fn_8004AE84(mNextReq.mName, 0);
                    lbl_8087EFE8->m34D0 = 0;
                }
            } else {
                mBgmState = 6;
                fn_803601E4(mNextReq.mReverb);
                mPrevReq = mCurReq;
                mCurReq = mNextReq;
                SwapBgm_unk();
            }
        }
    } else if (mBgmState == 4) {
        if (fn_800CB538(&mCur->mSound) == 0) {
            mBgmState = 6;
        }
    } else if (mBgmState == 5) {
        if (fn_800CB538(&mCur->mSound) == 0 && fn_800CB538(&mNext->mSound) == 0) {
            mNext->mSound.Reset();
            mBgmState = 6;
        }
    } else if (mBgmState == 6) {
        mBgmState = 0;
    }
}

// NONMATCHING: 99.7% - counter and offset of the loop swapped (r30/r31)
void EnvSeManager::fn_8035F33C(const char* path) {
    for (unsigned int i = 0; i < (unsigned int)mRecords.mSize; i++) {
        if (mRecords[i].mTask != 0) {
            fn_800D2338(mRecords[i].mTask);
        }
    }
    mState = 2;
    mLoader.vf0C(path);
}

// NONMATCHING: 99.5% - instructions match; the size of the links is read through fn_80089ACC, an
// out-of-line copy of another file whose class is unknown (ours calls SimpleArray::size)
void EnvSeManager::fn_8035F3D4() {
    atn::Csv csv((const char*)mLoader.fn_80470580(), mLoader.fn_8047059C());
    int records = 0;
    int areas = 0;
    do {
        std::string s(csv.fn_8005B9CC());
        if (s == "begin") {
            s = csv.fn_8005B9CC();
            if (s == "EnvSePoint") {
                records += atoi(csv.fn_8005B9CC());
            } else if (s == "EnvSeLine") {
                records += atoi(csv.fn_8005B9CC());
            } else if (s == "EnvSeSphere") {
                records += atoi(csv.fn_8005B9CC());
            } else if (s == "EnvSeBox") {
                records += atoi(csv.fn_8005B9CC());
            } else if (s == "EnvSeGlobal") {
                records += atoi(csv.fn_8005B9CC());
            } else if (s == "EnvSeArea") {
                areas += atoi(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
    mRecords.Alloc0_unk(records);
    mAreas.Alloc0_unk(areas);
    int record = 0;
    int area = 0;
    csv.SetData((const char*)mLoader.fn_80470580(), mLoader.fn_8047059C());
    do {
        std::string s(csv.fn_8005B9CC());
        if (s == "begin") {
            s = csv.fn_8005B9CC();
            if (s == "Header") {
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s == "Version") {
                        atoi(csv.fn_8005B9CC());
                    }
                }
            }
            if (s == "EnvSePoint") {
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] != ';') {
                        EnvSeRecord_unk& r = mRecords[record];
                        r.mTask = EnvSePoint::fn_8036111C(this, record);
                        if (r.mTask != 0) {
                            r.mTask->fn_80360CDC(&csv);
                            r.mTask->fn_800D246C(1);
                        }
                        record++;
                    }
                }
            } else if (s == "EnvSeLine") {
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] != ';') {
                        EnvSeRecord_unk& r = mRecords[record];
                        r.mTask = EnvSeLine::fn_80361414(this, record);
                        if (r.mTask != 0) {
                            r.mTask->fn_80360CDC(&csv);
                            r.mTask->fn_800D246C(1);
                        }
                        record++;
                    }
                }
            } else if (s == "EnvSeSphere") {
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] != ';') {
                        EnvSeRecord_unk& r = mRecords[record];
                        r.mTask = EnvSeSphere::fn_80362278(this, record);
                        if (r.mTask != 0) {
                            r.mTask->fn_80360CDC(&csv);
                            r.mTask->fn_800D246C(1);
                        }
                        record++;
                    }
                }
            } else if (s == "EnvSeBox") {
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] != ';') {
                        EnvSeRecord_unk& r = mRecords[record];
                        r.mTask = EnvSeBox::fn_803626A8(this, record);
                        if (r.mTask != 0) {
                            r.mTask->fn_80360CDC(&csv);
                            r.mTask->fn_800D246C(1);
                        }
                        record++;
                    }
                }
            } else if (s == "EnvSeGlobal") {
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] != ';') {
                        EnvSeRecord_unk& r = mRecords[record];
                        r.mTask = EnvSeGlobal::fn_80362CC4(this, record);
                        if (r.mTask != 0) {
                            r.mTask->fn_80360CDC(&csv);
                            r.mTask->fn_800D246C(1);
                        }
                        record++;
                    }
                }
            } else if (s == "EnvSeArea") {
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] != ';') {
                        EnvSeArea_unk& a = mAreas[area++];
                        a.fn_80362FA4(&csv);
                        if (IsDebugMenu_unk(fn_800F52F8())) {
                            a.fn_80363108(m1AC);
                        }
                        if (GetEngine_unk() != 0) {
                            a.m04 = fn_80049B74(GetEngine_unk(), a.mData.mName);
                        } else {
                            a.m04 = 0;
                        }
                    }
                }
            } else if (s == "EnvSeConnection") {
                int link = 0;
                mLinks.Alloc0_unk(atoi(csv.fn_8005B9CC()));
                while (csv.fn_8005BBF8() != 0) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] != ';') {
                        mLinks[link].mA = atoi(csv.GetField_unk());
                        mLinks[link].mB = atoi(csv.fn_8005B9CC());
                        link++;
                    }
                }
            }
        }
    } while (csv.fn_8005BBF8() != 0);
    for (unsigned int i = 0; i < mLinks.size(); i++) {
        EnvSeLink_unk& link = mLinks[i];
        EnvSePoint* a = fn_8035FF44(link.mA);
        EnvSePoint* b = fn_8035FF44(link.mB);
        if (a != 0) {
            a->SetLinked_unk(1);
        }
        if (b != 0) {
            b->SetLinked_unk(1);
        }
    }
}

// NONMATCHING: 97.2% - the counter of point sounds and the offset take swapped registers (r5/r6)
EnvSePoint* EnvSeManager::fn_8035FF44(int id) {
    int n = 0;
    for (unsigned int i = 0; i < (unsigned int)mRecords.mSize; i++) {
        EnvSeRecord_unk* r = &mRecords.mData[i];
        if (r->mData.mType == 0) {
            if (n == id) {
                return (EnvSePoint*)r->mTask;
            }
            n++;
        }
    }
    return 0;
}

// NONMATCHING: 97.9% - the original reads m84 again before storing it in m88
void EnvSeManager::fn_8035FF94(int id, int fade, int wait) {
    EnvSeArea_unk* area = FindArea_unk(id);
    mNextReq = EnvSeBgmReq_unk();
    if (area != 0) {
        sprintf(mNextReq.mName, "%s", area->mData.mName);
        mNextReq.mSoundId = area->m04;
        if (m7C != 0 || (m80 != 0 && lbl_8087EE90 != 0 && fn_80049B2C(lbl_8087EE90, area->m04) != 0)) {
            mNextReq.mSoundId = 0;
        }
        mNextReq.mVolume = area->mData.mVolume;
        mNextReq.mLPF = area->mData.mLPF;
        mNextReq.mReverb = area->mReverb;
        mNextReq.m34 = area->mA0;
    }
    mNextReq.mFade = fade;
    mNextReq.m3C = wait;
    if (id != m84 || mCurReq.mSoundId != mNextReq.mSoundId) {
        m88 = m84;
        m84 = id;
        if (mBgmState == 2 || mNext->fn_8004B0E4() != 0) {
            mNext->fn_8004B1EC(0);
            mNext->mSound.Reset();
        } else if (mBgmState == 5) {
            mNext->fn_8004B1EC(fade);
            mNext->mSound.Reset();
        }
        mBgmState = 1;
    }
}

void EnvSeManager::fn_803601E4(int reverb) {
    if (mCurReq.mReverb != reverb) {
        fn_8004A1D4(lbl_8087EE90, reverb);
        mCurReq.mReverb = reverb;
    }
}

int EnvSeManager::fn_80360230(int id) {
    EnvSeRecord_unk* r = &mRecords.mData[id];
    EnvSeBase* task = r->mTask;
    atn::SoundRef* sound = &r->mSound;
    int ready;
    if (lbl_8087EE90 == 0 || lbl_8087EFE8 == 0) {
        ready = 0;
    } else if (fn_80049B2C(lbl_8087EE90, r->mData.mSoundId) == 0) {
        ready = 1;
    } else if (r->mData.m40 == 1) {
        ready = fn_805A38F0(r->mData.mName, 8, 0);
    } else if (mCur->mSound.mPtr == 0 && mNext->mSound.mPtr == 0) {
        ready = fn_805A38F0(r->mData.mName, 2, 0);
    } else {
        ready = 0;
    }
    if (ready == 0) {
        return 0;
    }
    if (r->mData.m40 != 1) {
        lbl_8087EFE8->m34D0 = 2;
        if (r->mData.mType != 4) {
            *sound = fn_800C344C(r->mData.mName, &r->mData.mPos, 0, -1, r->mData.mVolume);
        } else {
            *sound = fn_800C31F4(r->mData.mName, 0, -1, r->mData.mVolume);
        }
        fn_800CB518(sound, 60);
        lbl_8087EFE8->m34D0 = 0;
    } else if (fn_805A38F0(r->mData.mName, 8, 0) != 0) {
        if (r->mData.mType != 4) {
            EnvSeStream_unk* stream = lbl_8087F498;
            *sound = fn_803E907C(stream, r->mData.mName, &r->mData.mPos, rand() % 5 * 3000, 0, r->mData.mVolume);
        } else {
            EnvSeStream_unk* stream = lbl_8087F498;
            *sound = fn_803E9608(stream, r->mData.mName, rand() % 5 * 3000, 0, r->mData.mVolume);
        }
    }
    fn_800CB69C(sound, 0, r->mData.mLPF);
    fn_800CB6B0(sound, 1, r->mData.mDistance);
    if (task != 0) {
        task->fn_8036102C(sound->mPtr != 0);
    }
    return sound->mPtr != 0;
}

void EnvSeManager::fn_80360504(int id, int fade) {
    EnvSeRecord_unk* r = &mRecords.mData[id];
    EnvSeBase* task = r->mTask;
    fn_800CB5C8(&r->mSound, fade, 0);
    if (task != 0) {
        task->fn_8036102C(0);
    }
}

void EnvSeManager::fn_8036055C(int id) {
    EnvSeRecord_unk* r = &mRecords.mData[id];
    fn_800CB6E4(&r->mSound, &r->mData.mPos);
}

void EnvSeManager::fn_80360574(int id) {
    EnvSeRecord_unk* r = &mRecords.mData[id];
    atn::SoundRef* sound = &r->mSound;
    fn_800CB6E4(sound, &r->mData.mPos);
    fn_800CB6B0(sound, 1, r->mData.mDistance);
    fn_800CB5B4(sound, 0, r->mData.mVolume);
    fn_800CB69C(sound, 0, r->mData.mLPF);
}

void EnvSeManager::fn_803605EC(int fade, float volume) {
    atn::SoundCategory_unk* scope = lbl_8087EFE8->fn_800CFDC8(2);
    m188 = scope != 0 ? scope->mVolume : 0.0f;
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
        EnvSeGroup_unk* group = fn_80373148(lbl_8087F430);
        if (group->m48 == 2 && group->m4C == 40 && 9 <= group->m50 && group->m50 <= 12) {
            volume = 1.0f;
        }
    }
    m184 = volume;
    if (fade < 60) {
        fade = 60;
    }
    lbl_8087EFE8->fn_800D03AC(2, m184, fade);
}

void EnvSeManager::fn_803606CC(int on, unsigned int bits, int fade) {
    if (on != 0) {
        m78 &= ~bits;
    } else {
        m78 |= bits;
    }
    if (on == 0 && m78 != 0) {
        for (unsigned int i = 0; i < (unsigned int)mRecords.mSize; i++) {
            fn_80360504(i, fade);
        }
    }
}

void EnvSeManager::fn_80360780(int on, unsigned int bits, int fade) {
    if (on != 0) {
        m7C &= ~bits;
    } else {
        m7C |= bits;
    }
    if (on != 0) {
        if (m7C == 0) {
            fn_8035EA2C();
        }
    } else if (m7C != 0) {
        mCur->fn_8004B1EC(fade);
        mCur->mSound.Reset();
        mCurReq.mSoundId = 0;
        mNext->fn_8004B1EC(fade);
        mNext->mSound.Reset();
        mNextReq.mSoundId = 0;
    }
}

void EnvSeManager::fn_80360848(int on, unsigned int bits, int fade) {
    if (on != 0) {
        m80 &= ~bits;
    } else {
        m80 |= bits;
    }
    if (on != 0) {
        if (m80 == 0) {
            fn_8035EA2C();
        }
    } else if (m80 != 0) {
        fn_8035FF94(m84, fade, 1);
    }
}

void EnvSeManager::fn_803608A4() {}

// NONMATCHING: 98.9% - registers of the search loop (record pointer and offset)
void EnvSeManager::fn_803608A8() {
    if (lbl_8087F430 != 0) {
        int id = 0;
        if (mCur != 0 && mCur->mSound.mPtr != 0) {
            id = fn_800CB6F8(&mCur->mSound);
        }
        if (id == 0) {
            for (unsigned int i = 0; i < (unsigned int)mRecords.mSize; i++) {
                EnvSeRecord_unk* r = &mRecords.mData[i];
                if (r->mData.mType == 4 && r->mSound.mPtr != 0 && r->mTask != 0 && r->mTask->m58 != 0) {
                    id = fn_800CB6F8(&r->mSound);
                    break;
                }
            }
        }
        fn_80370320(lbl_8087F430, 0x393, id, 0);
    }
}

void EnvSeManager::fn_8036097C(int a, int b, int c, int d, int useFlag) {
    if (lbl_8087F430 != 0) {
        if (a == 4) {
            a = 3;
        }
        m1A0 = 0;
        if (useFlag != 0) {
            m1A0 = fn_80214F38(fn_80370174(lbl_8087F430, 0x393));
        } else {
            m1A0 = fn_80214ED4(a, b, c, d);
        }
        char group[0x100];
        if (m1A0 != 0 && fn_80215018(m1A0, group)) {
            fn_800C3184(group);
        }
        m198 = 0;
        m19C = 0;
    }
}

int EnvSeManager::fn_80360A2C() {
    if (m198 != 0) {
        return 0;
    }
    if (fn_800C31E4() == 0) {
        if (m1A0 != 0) {
            char name[0x100];
            if (fn_80215084(m1A0, name)) {
                lbl_8087EFE8->m34D0 = 2;
                m194 = fn_800C31F4(name, 0, -1, 1.0f);
                lbl_8087EFE8->m34D0 = 0;
                fn_800CB518(&m194, 60);
            }
        }
        m198 = 1;
        m19C = 0;
        return 0;
    }
    return 1;
}

void EnvSeManager::fn_80360B00() {
    fn_800CB5C8(&m194, 60, 1);
    m19C = 1;
}

void EnvSeData_unk::fn_80360B40(atn::Csv* csv) {
    strncpy(mName, csv->fn_8005B9CC(), sizeof(mName));
    if (lbl_8087EE90 != 0) {
        mSoundId = fn_80049B74(lbl_8087EE90, mName);
    }
    mVolume = fn_800DC288(csv->fn_8005B9CC());
    mDistance = fn_800DC288(csv->fn_8005B9CC());
    mLPF = fn_800DC288(csv->fn_8005B9CC());
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
    m40 = atoi(csv->fn_8005B9CC());
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
    m44 = atoi(csv->fn_8005B9CC());
    m48 = atoi(csv->fn_8005B9CC());
    m4C = atoi(csv->fn_8005B9CC());
    m50 = atoi(csv->fn_8005B9CC());
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
}
