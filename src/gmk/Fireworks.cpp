#include "gmk/Fireworks.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include <boost/function.hpp>
#include "game/MapManager.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);

// inferred: light settings of the camera (returned by fn_800C16B4)
struct LightTarget_unk {
    // inferred: name invented
    void SetLight_unk(CharaLightData_unk* light) {
        m74 = light;
        m70 = m74 != 0 || m70 != 0;
    }
    unsigned char m00[0x70];
    int m70;
    CharaLightData_unk* m74;
    unsigned char m78[0xC8 - 0x78];
    int mC8;
    void SetLayer_unk(int layer) { mC8 = layer; }
};
struct Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" LightTarget_unk* fn_800C16B4(Camera_unk* camera);
// inferred: debug settings
struct FireworksDebug_unk {
    unsigned char m00[0x74];
    int m74; // inferred: debug draw
};
extern "C" FireworksDebug_unk* lbl_8087F0A8;

// inferred: speed of the light size change
static float sRate = 0.1f;

// inferred: sets the light of a map part (callback of fn_80491528)
void fn_8041CD24(MapModel_unk* part, void* light) {
    part->mC0 = light;
    part->mBC = light != 0 || part->mBC != 0;
    part->m114 = -1;
}

// inferred: random value in [0, 1] (name invented)
inline float Rand_unk() {
    return rand() % 1001 / 1000.0f;
}

// inferred: name invented
inline void SetLightPos_unk(CharaLightData_unk* light, const atn::Vector2_unk& pos) {
    light->m04 = pos;
}

namespace gmk {

GimmickCommon* Fireworks::fn_8041CD54(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) Fireworks(parent, id, arg);
    }
    return 0;
}

Fireworks::Fireworks(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mOffset(0.0f, 0.0f, 0.0f), mRange(0.0f, 0.0f, 0.0f), mInterval(60), mTimer(0),
      mLimit(-1), mCount(0), mWait(300), mWaitRand(0), mWaitTimer(0), mLight("common/fireworks.texture") {
    mLight.m04 = atn::Vector2_unk(0.0f, 0.0f);
    m54 = 0;
    mLight.mSize = atn::Vector2_unk(0.0f, 0.0f);
    mLight.m00 = 1;
}

Fireworks::~Fireworks() {
    fn_800C16B4(lbl_8087EFB4)->SetLight_unk(0);
    if (MapManager::sInstance != 0) {
        MapManager::sInstance->fn_80491528(&fn_8041CD24, 0);
    }
}

int Fireworks::vf0C() {
    if (mLight.fn_800C1990() != 0 || fn_803EC91C() != 0) {
        m54 = 1;
        return 0;
    }
    return 1;
}

void Fireworks::Start() {
    fn_800C16B4(lbl_8087EFB4)->SetLight_unk(&mLight);
    fn_800C16B4(lbl_8087EFB4)->SetLayer_unk(-1);
    MapManager::sInstance->fn_80491528(&fn_8041CD24, &mLight);
    GimmickMsg_unk msg(m58 + 1);
    vf40(&msg);
}

// NONMATCHING: 99.8% - flash loop: the element address and the address of its first member swap
// r6/r7. The unsigned subscript repeated in every access gives the per-iteration offset of the
// original. Tried: reference/pointer to the element, delay local, mixed forms.
void Fireworks::vf10() {
    if (m54 == 0) {
        Start();
    }
    MapManager::sInstance->fn_80491528(&fn_8041CD24, &mLight);
    float size = 0.0f;
    for (int i = mFlashes.mCount - 1; i >= 0; i--) {
        if (mFlashes.mData[(unsigned int)i].mDelay > 0) {
            mFlashes.mData[(unsigned int)i].mDelay--;
        } else if (--mFlashes.mData[(unsigned int)i].mFrames > 0) {
            size += mFlashes.mData[(unsigned int)i].mSize;
        } else {
            mFlashes.Erase_unk(&mFlashes.mData[(unsigned int)i]);
        }

    }
    float cur = mLight.mSize.x;
    float diff = size - cur;
    if (size != cur) {
        cur = diff * sRate + cur;
    }
    mLight.mSize = atn::Vector2_unk(cur, cur);
    if (mTimer > 0) {
        if (--mTimer > 0) {
            return;
        }
        mTimer = 0;
        int index = -1;
        if (m54 == 2) {
            index = rand() % 6;
        } else if (m54 >= 3) {
            index = m54 - 3;
        }
        if (index < 0) {
            return;
        }
        atn::Vector3 pos = m6C + mOffset;
        pos.x += mRange.x * Rand_unk() - mRange.x / 2.0f;
        pos.y += mRange.y * Rand_unk() - mRange.y / 2.0f;
        pos.z += mRange.z * Rand_unk() - mRange.z / 2.0f;
        fn_80232B7C(this, 0);
        float scale = m90.x;
        EffectMgr_unk* mgr = lbl_8087F3C0;
        fn_8023A8B4(mgr, &mEfp[index], 0, 0, pos, m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, scale);
        fn_800C31F4(mSe[index].mStr, 0, -1, 1.0f);
        SetLightPos_unk(&mLight, atn::Vector2_unk(pos.x, pos.z));
        mFlashes.PushBack_unk(mPresets[index]);
    } else {
        if (mWaitTimer > 0) {
            mWaitTimer--;
        }
        if (mWaitTimer <= 0 && m54 > 1) {
            mWaitTimer = 0;
            int launch = 1;
            if (mLimit > 0) {
                mCount--;
            }
            if (mLimit > 0 && mCount <= 0) {
                launch = 0;
            }
            if (launch) {
                int rnd = mWaitRand;
                mWaitTimer = mWait;
                if (rnd > 0) {
                    mWaitTimer += rand() % rnd - rnd / 2;
                }
                mTimer = mInterval;
            }
        }
    }
}

void Fireworks::vf14() {
    if (lbl_8087F0A8->m74 != 0) {
        fn_803EDCF4();
    }
}

int Fireworks::vf1C() {
    for (int i = 0; i < 6; i++) {
        if (mEfp[i].fn_80237874() != 0) {
            return 1;
        }
    }
    return 0;
}

void Fireworks::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char name[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EFP") == 0) {
            strcpy(name, csv.fn_8005B9CC());
            mEfp[atoi(csv.fn_8005B9CC())].fn_8023780C(name);
        } else if (strcmp(field, "EFPLIGHT") == 0) {
            int index = atoi(csv.fn_8005B9CC());
            int delay = atoi(csv.fn_8005B9CC());
            float size = fn_800DC288(csv.fn_8005B9CC());
            int frames = atoi(csv.fn_8005B9CC());
            mPresets[index].mDelay = delay;
            mPresets[index].mSize = size;
            mPresets[index].mFrames = frames;
        } else if (strcmp(field, "SE") == 0) {
            strcpy(name, csv.fn_8005B9CC());
            mSe[atoi(csv.fn_8005B9CC())] = name;
        } else if (strcmp(field, "LAUNCH") == 0) {
            mLimit = atoi(csv.fn_8005B9CC());
            mWait = atoi(csv.fn_8005B9CC());
            mWaitRand = atoi(csv.fn_8005B9CC());
        } else if (strcmp(field, "EXPLODE") == 0) {
            mInterval = atoi(csv.fn_8005B9CC());
            mOffset.x = fn_800DC288(csv.fn_8005B9CC());
            mOffset.y = fn_800DC288(csv.fn_8005B9CC());
            mOffset.z = fn_800DC288(csv.fn_8005B9CC());
            mRange.x = fn_800DC288(csv.fn_8005B9CC());
            mRange.y = fn_800DC288(csv.fn_8005B9CC());
            mRange.z = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

int Fireworks::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    vf70(msg->mType);
    mTimer = 1;
    return m54;
}

} // namespace gmk
