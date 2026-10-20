#include "game/GameBgm.h"
#include "game/GameSoundControl.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
// see GameBgm::BgmArray_unk
extern "C" void __ct__Q23atn3BgmFv();
extern "C" void __dt__Q23atn3BgmFv();
extern "C" void* __construct_array(void* block, void (*ctor)(), void (*dtor)(), unsigned long size,
                                   unsigned long n);
extern "C" void __destroy_arr(void* block, void (*dtor)(), unsigned long size, unsigned long n);

extern "C" const char* fn_80207EE8(int id); // inferred: BGM name of an id
extern "C" int fn_80207F34(int id);
// inferred: game settings (lbl_8087F0A8)
struct GameBgmSettings_unk {
    unsigned char m00[0x15C];
    int m15C; // inferred: alternative BGM set
};
extern "C" GameBgmSettings_unk* lbl_8087F0A8;
// inferred: map information returned by fn_80373148
struct GameBgmMapInfo_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
};
struct GameBgmTable_unk {
    unsigned char m00[0x140];
    int m140;
};
// inferred: field (lbl_8087F430)
struct GameBgmField_unk {
    unsigned char m0000[0x10D0];
    int m10D0; // inferred: story progress
    unsigned char m10D4[0x10D8 - 0x10D4];
    GameBgmTable_unk* m10D8;
};
extern "C" GameBgmField_unk* lbl_8087F430;
extern "C" GameBgmMapInfo_unk* fn_80373148(GameBgmField_unk* field);
extern "C" int fn_80370174(GameBgmField_unk* field, int id);           // inferred: reads a field value
extern "C" void fn_80370320(GameBgmField_unk* field, int id, int value, int arg); // inferred: writes it
// inferred: sound engine (lbl_8087EE90) and sound configuration (lbl_8087EFE8)
extern "C" void* lbl_8087EE90;
extern "C" unsigned int fn_80049B74(void* engine, const char* name); // inferred: sound id of a name
struct GameBgmSoundCfg_unk {
    unsigned char m0000[0x34C8];
    int m34C8;
    unsigned char m34CC[0x34D0 - 0x34CC];
    int m34D0;
    int m34D4;
};
extern "C" GameBgmSoundCfg_unk* lbl_8087EFE8;
// inferred: environment sound manager (lbl_8087F418)
struct GameBgmEnv_unk {
    unsigned char m000[0x80];
    int m80;
    int m84;
    unsigned char m088[0x180 - 0x88];
    // inferred: name invented
    bool IsBattle_unk() {
        bool r = false;
        if (m180 == 1 || m180 == 2) {
            r = true;
        }
        return r;
    }
    int m180;
    unsigned char m184[0x188 - 0x184];
    float m188;
    int m18C;
    float m190;
};
extern "C" GameBgmEnv_unk* lbl_8087F418;
extern "C" void fn_80360848(GameBgmEnv_unk* env, int on, int arg, int fade);
extern "C" void fn_803605EC(GameBgmEnv_unk* env, int fade, float volume);
extern "C" void* lbl_8087F610; // inferred: arena mode (as in game/CharaTask.cpp)
extern "C" int fn_803CE160(int a, int b, int c);
extern "C" int fn_803CE278(int a, int b, int c);
extern "C" void fn_803CE0B4(GameSoundControl* control, int id, int position);
extern "C" int fn_803CE074(GameSoundControl* control, int id);
extern "C" int fn_805A38F0(const char* name, int a, int b); // inferred: the stream is ready
// inferred: atn::SoundRef helpers (names keep the address)
extern "C" int fn_800CA910(atn::SoundObject* sound); // inferred: length
extern "C" int fn_800CAA88(atn::SoundObject* sound); // inferred: position
extern "C" int fn_800CB4C4(atn::SoundRef* sound);
extern "C" int fn_800CB538(atn::SoundRef* sound);
extern "C" int fn_800CB58C(atn::SoundRef* sound);
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int frames, float volume);
extern "C" void fn_800CB640(atn::SoundRef* sound, int a, int b, int c);
extern "C" int fn_800CB6F8(atn::SoundRef* sound);

static const int sBgmIds[7] = {0, 101, 201, 3, 4, 301, 6};

GameBgm* GameBgm::sInstance;

// inferred: name of the BGM `id`, with the replacements of the alternative set and of the story
// NONMATCHING: 99.3% - `name` and `progress` swap r30/r31.
static const char* fn_8037C71C(int id) {
    const char* name = fn_80207EE8(id);
    if (lbl_8087F0A8->m15C != 0) {
        switch (id) {
        case 0x65:
        case 0x66:
        case 0x69:
        case 0x6C:
        case 0x6D:
        case 0x191:
            name = fn_80207EE8(0x2707);
            break;
        case 0x68:
        case 0x6A:
        case 0x6B:
        case 0xC9:
            name = fn_80207EE8(0x270A);
            break;
        }
    }
    if (lbl_8087F430 != 0) {
        int progress = lbl_8087F430->m10D0;
        if (0 <= progress && progress < 400000 && id == 0x191) {
            name = fn_80207EE8(0x192);
        }
        if (400000 <= progress && progress < 600000 && id == 0x191) {
            name = fn_80207EE8(0x193);
        }
        if (progress >= 600000) {
            if (fn_80373148(lbl_8087F430) != 0 && fn_80373148(lbl_8087F430)->m48 == 1) {
                if (id == 0x191 || id == 0x1F5) {
                    name = fn_80207EE8(0x194);
                }
            } else if (id == 0x191 || id == 0x1F5) {
                name = fn_80207EE8(0x4EE);
            }
        }
    }
    return name;
}

// NONMATCHING: 0% - the constructor is inlined here; the original calls it (unknown why: it is
// the single candidate of this function and no inlining rule found so far excludes it).
GameBgm* GameBgm::fn_8037C8C4(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (10, "", "", 0) GameBgm(parent);
    }
    return sInstance;
}

inline GameBgm::BgmArray_unk::BgmArray_unk() {
    __construct_array(m, __ct__Q23atn3BgmFv, __dt__Q23atn3BgmFv, 0x14, 2);
}

GameBgm::GameBgm(atn::Task* parent) : atn::Task(parent), mState(0), mIndex(1), m80(0), m84(0), m88(0), mE0(0.0f) {
    mPrev = mBgm.Get(0);
    mCur = mBgm.Get(1);
}

GameBgm::~GameBgm() {
    for (int i = 0; i < 2; i++) {
        mBgm.Get(i)->fn_8004B1EC(0);
        mBgm.Get(i)->mSound.Reset();
    }
    sInstance = 0;
    __destroy_arr(mBgm.m, __dt__Q23atn3BgmFv, 0x14, 2);
}

int GameBgm::vf0C() {
    if (fn_800D3FA4() != 0) {
        mState = 7;
        return 1;
    }
    return 0;
}

// inferred: bit 0 of atn::Bgm::m10 (name invented)
static inline bool __IsKeepPos_unk(atn::Bgm* bgm) {
    return (bgm->m10 & 1) == 1;
}

// inferred: fades `bgm` out, keeping its position for the stream control (name invented)
static inline void __FadeOut_unk(atn::Bgm* bgm, int fade) {
    if (GameSoundControl::sInstance != 0 && bgm->mSound.mPtr != 0 && (bgm->m10 & 1) == 1) {
        int pos = fn_800CAA88(bgm->mSound.mPtr) + fade * 1000 / 30;
        if (pos > fn_800CA910(bgm->mSound.mPtr)) {
            pos -= fn_800CA910(bgm->mSound.mPtr);
        }
        GameSoundControl* control = GameSoundControl::sInstance;
        fn_803CE0B4(control, fn_800CB6F8(&bgm->mSound), pos);
    }
    bgm->fn_8004B1EC(fade);
}

static inline void __FadeOutIf_unk(atn::Bgm* bgm, int fade) {
    if (bgm != 0) {
        __FadeOut_unk(bgm, fade);
    }
}

// inferred: the field BGM may replace the environment sounds (written out in each branch
// condition; name invented)
#define IS_FIELD_BGM_unk(id) \
    (fn_803CE160(0, 0, 0) == 0 && (fn_803CE278(0, 0, 0) != 0 || (id) == 0x67) && lbl_8087F610 == 0)

// NONMATCHING: 97.7% - registers of the player swap (next/slot/zero) and of the fade-out locals, the
// original reloads Bgm::m10 before clearing bit 0, and one `beq` is emitted as `bne; b`.
void GameBgm::vf10() {
    if (m84 != 0 && (unsigned int)(mState - 6) <= 1) {
        m84 = 0;
        m88 = 1;
        mState = 1;
    }
    if (mState == 1) {
        if (m88 != 0) {
            m88 = 0;
            mPrev = mBgm.Get(mIndex);
            mIndex = (mIndex + 1) % 2;
            mCur = mBgm.Get(mIndex);
            mPrevReq = mReq;
            mReq = mNext;
        }
        if (mReq.mCross != 0) {
            if (fn_805A38F0(mReq.mName, 1, 1) != 0) {
                mCur->m10 |= mReq.mLoop != 0;
                fn_8037D274(mCur, mReq.mName);
                mState = 2;
            }
            return;
        }
        if (mReq.mId <= 0 && lbl_8087F418 != 0 && lbl_8087F418->m80 != 0) {
            fn_80360848(lbl_8087F418, 1, 2, mReq.mFade);
        }
        bool busy = false;
        if (lbl_8087F418 != 0 && lbl_8087F418->m84 > 0 && lbl_8087F418->IsBattle_unk()) {
            busy = true;
        }
        if (busy) {
            return;
        }
        int fade = mReq.mFade;
        atn::Bgm* prev = mPrev;
        if (prev != 0) {
            __FadeOut_unk(prev, fade);
        }
        if (__IsKeepPos_unk(mPrev)) {
            mPrev->m10 &= ~1;
        }
        mState = 3;
        if (mReq.mId > 0) {
            return;
        }
        if (fn_803CE160(0, 0, 0) != 0) {
            return;
        }
        if (fn_803CE278(0, 0, 0) == 0 && mPrevReq.mId != 0x67) {
            return;
        }
        if (lbl_8087F610 != 0) {
            return;
        }
        GameBgmEnv_unk* env = lbl_8087F418;
        if (env != 0 && env->m18C != 0) {
            env->m18C = 0;
            fn_803605EC(env, mReq.mFade, env->m188);
        }
    } else if (mState == 2) {
        if (mCur->fn_8004B0E4() != 0 || m80 == 0) {
            return;
        }
        mCur->fn_8004B158(mReq.mVolume, mReq.mFade);
        if (mReq.mCross != 0) {
            int fade = 15;
            if (mReq.mFade > 15) {
                fade = mReq.mFade;
            }
            atn::Bgm* prev = mPrev;
            if (prev != 0) {
                __FadeOut_unk(prev, fade);
            }
            if (__IsKeepPos_unk(mPrev)) {
                mPrev->m10 &= ~1;
            }
            mState = 5;
        } else {
            mState = 4;
        }
        if (lbl_8087F418 != 0 && mReq.mId > 0) {
            if (!IS_FIELD_BGM_unk(mReq.mId)) {
                fn_80360848(lbl_8087F418, 0, 2, mReq.mFade);
            } else {
                fn_80360848(lbl_8087F418, 1, 2, mReq.mFade);
                GameBgmEnv_unk* env = lbl_8087F418;
                if (env->m18C == 0) {
                    env->m18C = 1;
                    fn_803605EC(env, mReq.mFade, env->m190);
                }
            }
        }
    } else if (mState == 3) {
        if (fn_800CB538(&mPrev->mSound) != 0) {
            return;
        }
        mPrev->mSound.Reset();
        if (mReq.mId > 0) {
            const char* name = mReq.mName;
            if (fn_805A38F0(name, 1, 1) != 0) {
                mCur->m10 |= mReq.mLoop != 0;
                fn_8037D274(mCur, name);
                mState = 2;
            }
        } else {
            mState = 7;
        }
    } else if (mState == 4) {
        if (fn_800CB538(&mCur->mSound) == 0) {
            mState = 6;
        }
    } else if (mState == 5) {
        if (fn_800CB538(&mCur->mSound) == 0 && fn_800CB538(&mPrev->mSound) == 0) {
            mPrev->mSound.Reset();
            mState = 6;
        }
    } else if (mState == 6) {
        if (fn_800CB58C(&mCur->mSound) == 0) {
            fn_8037D4C0(0, 0, 0, 0, 1.0f);
            m80 = 0;
        }
        if (lbl_8087F418 == 0) {
            return;
        }
        if (!IS_FIELD_BGM_unk(mReq.mId)) {
            if (lbl_8087F418->m80 == 0) {
                fn_80360848(lbl_8087F418, 0, 2, mReq.mFade);
            }
        } else if (lbl_8087F418->m80 != 0) {
            fn_80360848(lbl_8087F418, 1, 2, mReq.mFade);
            GameBgmEnv_unk* env = lbl_8087F418;
            if (env->m18C == 0) {
                env->m18C = 1;
                fn_803605EC(env, mReq.mFade, env->m190);
            }
        }
    }
}

// NONMATCHING: 99.1% - `start` and `bgm` swap r28/r29.
void GameBgm::fn_8037D274(atn::Bgm* bgm, const char* name) {
    if (bgm == 0) {
        return;
    }
    int start = 0;
    if (mReq.mStart > 0) {
        start = mReq.mStart;
    }
    int id = fn_80049B74(lbl_8087EE90, name);
    if (GameSoundControl::sInstance != 0 && (bgm->m10 & 1) == 1) {
        start = fn_803CE074(GameSoundControl::sInstance, id);
        if (start < 0) {
            start = 0;
        }
    }
    int save = lbl_8087EFE8->m34D4;
    lbl_8087EFE8->m34D0 = 1;
    lbl_8087EFE8->m34D4 = 1;
    lbl_8087EFE8->m34C8 = 0;
    bgm->fn_8004AE84(name, start);
    lbl_8087EFE8->m34D0 = 0;
    lbl_8087EFE8->m34D4 = save;
    lbl_8087EFE8->m34C8 = 1;
}

// inferred: `bgm` is playing (name invented)
static inline bool __IsPlaying_unk(atn::Bgm* bgm) {
    return bgm != 0 && fn_800CB58C(&bgm->mSound) != 0 && fn_800CB4C4(&bgm->mSound) != 0;
}

// NONMATCHING: 89.7% - the original loads mCur before saving the registers.
int GameBgm::fn_8037D34C() {
    if (mCur != 0 && fn_800CB58C(&mCur->mSound) != 0 && fn_800CB4C4(&mCur->mSound) != 0) {
        return 1;
    }
    if (mPrev != 0 && fn_800CB58C(&mPrev->mSound) != 0 && fn_800CB4C4(&mPrev->mSound) != 0) {
        return 1;
    }
    return 0;
}

void GameBgm::fn_8037D3E8(int a, int b, int c) {
    if (mCur != 0 && fn_800CB58C(&mCur->mSound) != 0) {
        fn_800CB640(&mCur->mSound, a, b, c);
    }
    if (mPrev != 0 && fn_800CB58C(&mPrev->mSound) != 0) {
        fn_800CB640(&mPrev->mSound, a, b, c);
    }
}

void GameBgm::fn_8037D49C(float volume, int frames) {
    mE0 = mReq.mVolume;
    mReq.mVolume = volume;
    if (mCur != 0) {
        fn_800CB5B4(&mCur->mSound, frames, volume);
    }
}

// inferred: the name is set and not empty (name invented)
static inline bool __IsName_unk(const char* name) {
    bool ok = false;
    if (name != 0 && strlen(name) != 0) {
        ok = true;
    }
    return ok;
}

// NONMATCHING: 99.6% - `engine` and `name` swap r28/r29 and the second engine load takes r30.
int GameBgm::fn_8037D4C0(int id, int fade, int b, int c, float volume) {
    if (0 < id && id <= 6) {
        id = sBgmIds[id];
    }
    const char* name;
    void* engine = lbl_8087EE90;
    unsigned int cur = 0;
    unsigned int next = 0;
    if (engine != 0 && lbl_8087EFE8 != 0) {
        name = mReq.mName;
        if (__IsName_unk(name)) {
            cur = fn_80049B74(engine, name);
        }
        if (__IsName_unk(fn_8037C71C(id))) {
            void* engine2 = lbl_8087EE90;
            next = fn_80049B74(engine2, fn_8037C71C(id));
        }
    }
    if (cur != next) {
        mNext.mId = id;
        mNext.mName = fn_8037C71C(id);
        mNext.mVolume = volume;
        mNext.mFade = fade;
        mNext.mStart = c;
        mNext.mCross = b;
        if (id <= 0 || fn_803CE160(0, 0, 0) != 0) {
            mNext.mCross = 0;
        }
        mNext.mLoop = fn_80207F34(id);
        m80 = 1;
        m84 = 1;
        return 1;
    }
    mReq.mId = id;
    mReq.mFade = fade;
    // inferred: converted through double in the original (frsp before the store)
    mE0 = (double)volume;
    mReq.mVolume = volume;
    if (mCur != 0) {
        fn_800CB5B4(&mCur->mSound, fade, volume);
    }
    m84 = 0;
    return 0;
}

void GameBgm::fn_8037D684(int a, int b) {
    if (lbl_8087F430 == 0 || fn_80373148(lbl_8087F430) == 0) {
        return;
    }
    if (m84 == 0 && mReq.mId > 0) {
        GameBgmMapInfo_unk* info = fn_80373148(lbl_8087F430);
        if ((a != info->m48 || b != info->m4C) && m84 == 0 && (mReq.mId == 0x191 || mReq.mId == 0x1F5)) {
            fn_8037D4C0(0, 45, 1, 0, 1.0f);
        }
    }
}

void GameBgm::fn_8037D758() {
    if (lbl_8087F430 == 0 || lbl_8087F430->m10D8 == 0) {
        return;
    }
    int id = fn_80370174(lbl_8087F430, 0x38F);
    float volume = fn_80370174(lbl_8087F430, 0x390) / 100.0f;
    if (id > 0) {
        fn_8037D4C0(id, 60, 1, 0, volume);
    }
}

void GameBgm::fn_8037D808() {
    if (lbl_8087F430 == 0 || lbl_8087F430->m10D8 == 0) {
        return;
    }
    int id = 0;
    float volume = 1.0f;
    if (lbl_8087F430->m10D8->m140 == 0) {
        if (m84 != 0) {
            id = mNext.mId;
            volume = mNext.mVolume;
        } else {
            id = mReq.mId;
            volume = mReq.mVolume;
        }
    }
    fn_80370320(lbl_8087F430, 0x38F, id, 0);
    fn_80370320(lbl_8087F430, 0x390, (int)(100.0f * volume), 0);
}
