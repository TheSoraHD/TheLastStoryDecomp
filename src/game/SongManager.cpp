#include "game/EnvSeManager.h"
#include "game/GameVoice.h"
#include "atn/Task.h"
#include "atn/SoundCore.h"
#include "atn/SoundManager.h"
#include "atn/Bgm.h"
#include "atn/Memory.h"
#include "game/GameBgm.h"
#include "math/Vec.h"
#include "game/SongManager.h"

extern "C" double sqrt(double x);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// inferred: atn::SoundRef helpers (names keep the address, as in game/GameBgm.cpp)
extern "C" int fn_800CB58C(atn::SoundRef* sound);    // inferred: playing
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
extern "C" atn::Vector3* fn_800CB6C4(atn::SoundRef* sound); // inferred: position of the sound



// inferred: party manager (lbl_8087F8A0) and its leader, only the members used here
struct SongChara_unk {
    unsigned char m000[0x528];
    atn::Vector3 mPos; // 0x528
    atn::Vector3 mRot; // 0x534
};
struct SongParty_unk {
    unsigned char m00[0x48];
    SongChara_unk* m48; // inferred: leader
};
extern "C" SongParty_unk* lbl_8087F8A0;

// inferred: sound engine and field (as in game/GameBgm.cpp)
extern "C" void* lbl_8087EE90;
extern "C" unsigned int fn_80049B74(void* engine, const char* name); // inferred: sound id of a name
extern "C" int fn_80049B2C(void* engine, unsigned int id);
extern "C" void* lbl_8087F430;
extern "C" void* fn_80373148(void* field);
struct SongBattle_unk {
    unsigned char m00[0x70];
    int m70;
};
extern "C" SongBattle_unk* lbl_8087F540;
extern "C" int fn_803CE160(int a, int b, int c);

// inferred: limit of the sounds of kind 8
static inline int __Limit_unk() {
    return lbl_8087F498 != 0 ? lbl_8087F498->mNumChannels : 1;
}

// inferred: the reference holds a sound (read as a word: the original reloads the pointer for the
// calls after the test)
static inline bool __IsValid_unk(const atn::SoundRef& ref) {
    return *(const int*)&ref != 0;
}

// inferred: songs (sound name, left channel, right channel, mixed by direction)
struct SongEntry_unk {
    const char* mName;
    int mLeft;  // 0x04
    int mRight; // 0x08
    int mPan;   // 0x0C
};

static const SongEntry_unk sSongs[] = {
    {"SE_DG063_100", 1, 1, 1},
    {"SE_DG063_101", 1, 1, 1},
    {"SE_DG063_101", 1, 0, 0},
};

SongManager* SongManager::sInstance;

// NONMATCHING: 92.19% - in the inlined constructor the original creates 1.0f before 0.0f (its
// sdata2 pool starts with 1.0f) and reloads the three .sdata values without the frsp that we get
// for 0.1f. Tried other initializer orders, the values as globals defined later, an inline helper
// using 1.0f first.
SongManager* SongManager::fn_805A2240(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new ((MemoryTag)12, "", "", 0) SongManager(parent);
    }
    return sInstance;
}

float sSongDistance_unk = 300.0f;
float sSongMinVolume_unk = 0.1f;
float sSongMaxVolume_unk = 1.0f;

int SongManager::fn_805A2404(int area, int map, int arg) {
    if (area == 2 && (map == 33 || map == 63 || map == 999)) {
        return 1;
    }
    return 0;
}

SongManager::~SongManager() {
    if (mBgm != 0) {
        mBgm->fn_8004B1EC(GetFade_unk());
        mBgm->mSound.Reset();
        mBgm = 0;
    }
    sInstance = 0;
}

// NONMATCHING: 99.02% - stack offsets and registers of the temporaries of the vector interpolation
// and of the distance vector.
void SongManager::vf10() {
    if (mBgm == 0) {
        return;
    }
    if (mFC != 0) {
        if (mBgm->fn_8004B0E4() != 0) {
            return;
        }
        mFC = 0;
        mBgm->fn_8004B158(1.75f, 60);
        UpdateEnvSe_unk();
        if (__IsValid_unk(mBgm->mSound)) {
            ((atn::SoundCore*)mBgm->mSound.mPtr)->fn_800CAC48(0, 0.0f, 0);
            ((atn::SoundCore*)mBgm->mSound.mPtr)->fn_800CAC48(1, 0.0f, 0);
        }
        return;
    }
    mPos.Update_unk();
    mDistance.Update_unk();
    mMaxVolume.Update_unk();
    mMinVolume.Update_unk();
    if (mBgm->mSound.mPtr != 0) {
        fn_800CB6E4(&mBgm->mSound, &mPos.mCur);
        ((atn::SoundCore*)mBgm->mSound.mPtr)->fn_800CA798(1, mDistance.mCur);
        SongChara_unk* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->m48 : 0;
        if (leader == 0) {
            return;
        }
        atn::Vector3 dir = *fn_800CB6C4(&mBgm->mSound) - leader->mPos;
        float dist = sqrt(dir.x * dir.x + dir.z * dir.z);
        if (dist > 0.0f) {
            PSVECNormalize(&dir, &dir);
        }
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', leader->mRot.y);
        PSMTXMultVec(m, &front, &front);
        float dot = PSVECDotProduct(&dir, &front);
        float ratio = dist / mDistance.mCur > 0.0f ? dist / mDistance.mCur : 0.0f;
        if (ratio < 1.0f) {
            ratio = dist / mDistance.mCur > 0.0f ? dist / mDistance.mCur : 0.0f;
        } else {
            ratio = 1.0f;
        }
        float volume = (1.0f - ratio) * (mMaxVolume.mCur - mMinVolume.mCur) + mMinVolume.mCur;
        atn::Bgm* bgm = mBgm;
        if (!__IsValid_unk(bgm->mSound)) {
            return;
        }
        if (sSongs[m4C].mPan != 0) {
            float pan;
            if (dot >= 0.9659258f) {
                pan = 1.0f;
            } else if (dot >= 0.8660254f) {
                pan = 0.75f;
            } else if (dot >= 0.70710677f) {
                pan = 0.5f;
            } else if (dot >= 0.5f) {
                pan = 0.25f;
            } else {
                pan = 0.0f;
            }
            float right = 1.0f - pan;
            pan *= volume;
            right *= volume;
            ((atn::SoundCore*)bgm->mSound.mPtr)->fn_800CAC48(0, pan, 60);
            ((atn::SoundCore*)mBgm->mSound.mPtr)->fn_800CAC48(1, right, 60);
        } else {
            if (sSongs[m4C].mLeft != 0) {
                ((atn::SoundCore*)bgm->mSound.mPtr)->fn_800CAC48(0, volume, 60);
            } else {
                ((atn::SoundCore*)bgm->mSound.mPtr)->fn_800CAC48(0, 0.0f, 60);
            }
            if (sSongs[m4C].mRight != 0) {
                ((atn::SoundCore*)mBgm->mSound.mPtr)->fn_800CAC48(1, volume, 60);
            } else {
                ((atn::SoundCore*)mBgm->mSound.mPtr)->fn_800CAC48(1, 0.0f, 60);
            }
        }
    } else {
        m48 = 0;
        mBgm = 0;
    }
}

void SongManager::vf14() {}

// NONMATCHING: 97.88% (and fn_805A32C8 68.12%, objdiff aligns badly) - the original loads the 1.0f
// volume of fn_8037D4C0 before the inlined GetFade_unk branches. Tried 1.0 as a double literal and
// an inline request helper.
void SongManager::fn_805A2FD4(int state) {
    int old = m48;
    m48 = state;
    if (old == 0 && state == 1) {
        GameBgm::sInstance->fn_8037D4C0(-100, GetFade_unk(), 0, 0, 1.0f);
        mBgm = GameBgm::sInstance->mPrev;
        lbl_8087EFE8->m34D0 = 1;
        mBgm->fn_8004AFAC(sSongs[m4C].mName, (int)&mPos.mCur, 0);
        lbl_8087EFE8->m34D0 = 0;
        mFC = 1;
    } else if (old == 1 && m48 == 0 && mBgm != 0) {
        GameBgm::sInstance->fn_8037D4C0(-200, GetFade_unk() > 0 ? GetFade_unk() : 90, 0, 0, 1.0f);
        mBgm->fn_8004B1EC(GetFade_unk());
        mBgm = 0;
        if (mF0 != 0) {
            fn_805A3808(0);
            mF0 = 1;
        }
    }
}

void SongManager::fn_805A32C8(int song) {
    int old = m4C;
    m4C = song;
    if (m48 == 1 && old != song) {
        int pos = 0;
        if (mBgm != 0 && mBgm->mSound.mPtr != 0 && fn_800CB58C(&mBgm->mSound) != 0) {
            pos = ((atn::SoundCore*)mBgm->mSound.mPtr)->fn_800CAA88() + 0x85;
            mBgm->fn_8004B1EC(GetFade_unk());
        }
        GameBgm::sInstance->fn_8037D4C0(m4C * -100 - 1000, GetFade_unk(), 1, 0, 1.0f);
        mBgm = GameBgm::sInstance->mPrev;
        lbl_8087EFE8->m34D0 = 1;
        mBgm->fn_8004AFAC(sSongs[m4C].mName, (int)&mPos.mCur, pos);
        lbl_8087EFE8->m34D0 = 0;
        mFC = 1;
    }
}

// NONMATCHING: 95.96% - the original loads 1.0f at the start of the function (scheduling).
void SongManager::fn_805A3448(const atn::Vector3& pos) {
    mPos.Start_unk(pos, GetFade_unk());
}

void SongManager::fn_805A358C(float distance) {
    mDistance.Start_unk(distance, GetFade_unk());
}

void SongManager::fn_805A3660(float volume) {
    mMinVolume.Start_unk(volume, GetFade_unk());
}

void SongManager::fn_805A3734(float volume) {
    mMaxVolume.Start_unk(volume, GetFade_unk());
}

void SongManager::fn_805A3808(int lower) {
    mF0 = lower;
    UpdateEnvSe_unk();
}

// inferred: the stream `name` can be played (kind `type`): checks the number of sounds of each kind
// that are playing
extern "C" int fn_805A38F0(const char* name, int type, int arg) {
    if (name == 0) {
        return 1;
    }
    if (lbl_8087EE90 == 0 || lbl_8087EFE8 == 0) {
        return 1;
    }
    if (!(lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) && !(lbl_8087F540 != 0 && lbl_8087F540->m70 != 0)) {
        return 1;
    }
    unsigned int id = fn_80049B74(lbl_8087EE90, name);
    if (id == 0 || fn_80049B2C(lbl_8087EE90, id) == 0) {
        return 1;
    }
    int counts[9];
    for (int i = 0; i < 9; i++) {
        if (i == 1) {
            counts[i] = lbl_8087EFE8->fn_800D089C(i, 1);
        } else {
            counts[i] = lbl_8087EFE8->fn_800D089C(i, 0);
        }
    }
    if (fn_803CE160(0, 0, 0) == 0) {
        switch (type) {
        case 1:
            if (counts[1] > 1) {
                return 0;
            }
            if (counts[1] == 1) {
                if (arg != 1) {
                    return 0;
                }
                if (counts[2] + counts[7] + counts[3] > 1) {
                    return 0;
                }
            }
            break;
        case 2:
            if (counts[2] > 1) {
                return 0;
            }
            if (counts[2] == 1) {
                if (arg != 1) {
                    return 0;
                }
                if (counts[1] + counts[7] + counts[3] > 1) {
                    return 0;
                }
            }
            break;
        case 3:
            if (counts[3] + counts[4] + counts[7] > 1) {
                return 0;
            }
            break;
        case 4:
        case 7:
            if (counts[3] + counts[4] + counts[7] > 0) {
                if (type == 4) {
                    lbl_8087EFE8->fn_800D0974(3, 0);
                    lbl_8087EFE8->fn_800D0974(4, 0);
                    lbl_8087EFE8->fn_800D0974(7, 0);
                } else {
                    return 0;
                }
            }
            break;
        case 8: {
            int n = counts[8];
            if (n >= __Limit_unk()) {
                return 0;
            }
            break;
        }
        }
    } else {
        switch (type) {
        case 1:
            if (counts[1] > 0) {
                return 0;
            }
            if (counts[2] > 1) {
                return 0;
            }
            break;
        case 2:
            if (counts[2] + counts[1] > 1) {
                return 0;
            }
            if (counts[2] == 1 && arg != 1) {
                return 0;
            }
            break;
        case 8: {
            int n = counts[8];
            if (n > __Limit_unk()) {
                return 0;
            }
            break;
        }
        }
    }
    return 1;
}
