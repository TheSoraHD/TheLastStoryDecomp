#include "gmk/ShakeCamera.h"
#include "atn/SoundManager.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/ResHandle.h"

extern "C" int rand();
extern "C" double sqrt(double x);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
// inferred: plays the sound `name` (not positioned)
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch

// inferred: camera shake request in the field object (class unknown)
struct ShakeField_unk {
    unsigned char m00[0x96C];
    int m96C;
    int m970;   // inferred: frames
    float m974; // inferred: x amplitude
    float m978; // inferred: y amplitude
    unsigned char m97C[0x54E4 - 0x97C];
    int m54E4;
    // inferred: starts a camera shake (name invented)
    void Shake_unk(int frame, float x, float y) {
        m96C = m96C % 2;
        m970 = frame;
        m974 = x;
        m978 = y;
    }
};
extern "C" ShakeField_unk* lbl_8087F430;

// inferred: square root of a float (name invented)
inline float SqrtF_unk(float x) {
    return (float)sqrt(x);
}
// inferred: random float in [0, 1] (name invented)
inline float Rand01_unk() {
    return (float)(rand() % 1001) / 1000.0f;
}
// inferred: random float between min and max (name invented)
inline float RandRange_unk(float min, float max) {
    return min + (max - min) * Rand01_unk();
}

namespace gmk {

GimmickCommon* ShakeCamera::fn_80421454(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) ShakeCamera(parent, desc);
    }
    return 0;
}

int ShakeCamera::vf0C() {
    return fn_803EC91C() == 0;
}

void ShakeCamera::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    mShakeX = mDesc->m40;
    mShakeY = mDesc->m44;
    mShakeFrame = mDesc->m20;
    mWaitFrame = mDesc->m24;
    mWaitFrameMin = mDesc->m28;
    mRateMax = 1.0f;
    mRateMin = 0.0f;
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// NONMATCHING: 99.34% - registers only: the target keeps the field pointer in r5 (r7 here; the
// remainder of rand() % 1001 takes r6, r5 here). The square roots of the volume go through SqrtF_unk (the
// rate in f31 as in the original) and the pitch writes Rand01_unk out (the 0.5f is loaded after the
// field, as in ColliAttrManager). Tried: SetShake_unk(m96C % 2, ...) as in MotionCommand_unk,
// x/y in the call, a frame local, the orders of rate/x/y/field (24), RandRange_unk with the factor or
// the range as a local, written out. The two compares of the original
// come from `if (m54 == 1) return;` before the case 2 test (a switch with an empty case 1 drops the
// compare). The field writes go through an inline taking (frame, x, y), so the members are read before
// the stores. Tried: `min +=` and const references in RandRange_unk, the rate built in place, a field
// local, `m96C %= 2`; before: default/empty calls/self-assignments in case 1, forms of the random range
// helper, explicit locals, operand orders of the volume and the pitch.
void ShakeCamera::vf10() {
    if (m54 == 1) {
        return;
    }
    if (m54 == 2) {
        if (--mWait < 0) {
            mWait = mWaitFrameMin + rand() % mWaitFrame;
            float rate = RandRange_unk(mRateMax, mRateMin);
            float x = mShakeX * rate;
            float y = mShakeY * rate;
            lbl_8087F430->Shake_unk(mShakeFrame, x, y);
            if (lbl_8087F430->m54E4 == 0) {
                float volume = SqrtF_unk(rate) * SqrtF_unk(x * x + y * y) < 1.5f
                                   ? SqrtF_unk(rate) * SqrtF_unk(x * x + y * y)
                                   : 1.5f;
                atn::SoundRef sound = fn_800C31F4("SE_DGCOM_008", 0, -1, volume);
                atn::SoundManager* cfg = lbl_8087EFE8;
                fn_800CB688(&sound, 0, 1.0f + (cfg->mRandomPitch * ((float)(rand() % 1001) / 1000.0f) - 0.5f * cfg->mRandomPitch));
            }
        }
    }
}

void ShakeCamera::vf14() {}

int ShakeCamera::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
    case 2:
        m54 = msg->mType;
        mWait = mWaitFrameMin + rand() % mWaitFrame;
        break;
    }
    return m54;
}

int ShakeCamera::vf60() {
    int ret = 0;
    if (m54 >= 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

void ShakeCamera::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_8008771C(group, "shake_x", &mShakeX, 0, 0, 0.0f, 1000.0f, 0.1f);
    fn_8008771C(group, "shake_y", &mShakeY, 0, 0, 0.0f, 1000.0f, 0.1f);
    fn_800874C8(group, "shake_frame", &mShakeFrame, 0, 100000, 1, 0, 0);
    fn_800874C8(group, "wait_frame", &mWaitFrame, 0, 100000, 1, 0, 0);
    fn_800874C8(group, "wait_frame_min", &mWaitFrameMin, 0, 100000, 1, 0, 0);
    fn_8008771C(group, "rate_max", &mRateMax, 0, 0, 0.0f, 1000.0f, 0.01f);
    fn_8008771C(group, "rate_min", &mRateMin, 0, 0, 0.0f, 1000.0f, 0.01f);
}

} // namespace gmk
