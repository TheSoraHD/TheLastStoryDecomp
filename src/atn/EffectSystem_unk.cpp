// atn/EffectSystem_unk.cpp: the effect system of the efp effects (file name invented): the
// effect instances started by atn::EffectManager (list EffectSystem_unk::mInsts) and the particles
// they emit (EffectSystem_unk::mParticles, code of the particles in atn/EffectRes_unk.cpp), the
// lookups by owner/group and by handle, and the current owner/group of the effects being created.
// Only the small functions are written; the update, draw and creation code is not decompiled yet.
#include "atn/EffectSystem_unk.h"

// inferred: current resource, owner, group and handle of the effects being created, and the frame
// offset (names invented; the other variables of .sdata/.sbss are not used by the code written)
static EffectRes_unk* sCurrentRes;   // 0x8087F360
static unsigned int sCurrentOwner;   // 0x8087F364
static int sFrame;                   // 0x8087F368
static int sFrameBase;               // 0x8087F36C
static int sCurrentHandle;           // 0x8087F370
static int sCurrentGroup = -1;       // 0x8087DBE8

// inferred: stop request of an instance (name invented)
inline void Kill_unk(EffectInst_unk* inst) {
    if (inst) {
        inst->mKill = 1;
    }
}

EffectInst_unk::EffectInst_unk()
    : m00(0), mNext(0), m08(0), m0C(0), m10(0), m14(-1), mFlags(0), m1A(0), mKill(0), m1C(0.0f), mOwnerPtr(0),
      mOwner(0), mGroup(0), mEffect(0), mTag(0), mBC(0), mC0(0), mC4(0), mC8(0.0f), mCC(0.0f), mRate(1.0f) {}

EffectSlot_unk::EffectSlot_unk()
    : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(0.0f), m18(0.0f), m1C(0), m20(0), m24(0), m28(0.0f), m2C(0.0f),
      m30(0.0f), m34(0.0f), m190(0), m194(0.0f), m198(0.0f), m19C(0.0f), m1A0(0.0f), m1A4(1.0f), m1A8(0), m1AC(0),
      m1B0(0), m1B4(0) {}

EffectFade_unk::EffectFade_unk() : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f) {}

// inferred: frame offset of the effects (game/GameTask.cpp)
extern "C" void fn_802328DC(int frame) {
    sFrame = sFrameBase + frame;
}

// inferred: marks the resource as used
extern "C" void fn_80232AC4(EffectRes_unk* res) {
    sCurrentRes = res;
}

// inferred: stops the effects of an owner object
void EffectSystem_unk::fn_80232ACC(void* owner) {
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mOwnerPtr == owner) {
            Kill_unk(inst);
        }
        inst = next;
    }
    for (int i = 0; i < mParticleCount; i++) {
        if ((void*)mParticles[i].m24 == owner) {
            mParticles[i].fn_80238560();
        }
    }
}

// inferred: sets the current owner/group of the effects being created
extern "C" void fn_80232B7C(unsigned int owner, int group) {
    sCurrentOwner = owner;
    sCurrentGroup = group;
}

extern "C" unsigned int fn_80232B88() {
    return sCurrentOwner;
}

extern "C" int fn_80232B90() {
    return sCurrentGroup;
}

// inferred: an effect of the owner/group is playing
int EffectSystem_unk::fn_80232B98(unsigned int owner, int group) {
    if (group == -1) {
        return 0;
    }
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            return 1;
        }
    }
    return 0;
}

// inferred: stops the effects of the owner/group and moves its particles forward
int EffectSystem_unk::fn_80232BEC(unsigned int owner, int group, int count) {
    if (group == -1) {
        return 0;
    }
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            fn_802328EC(inst, count);
        }
        inst = next;
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if (p->IsMatch_unk(owner, group)) {
            p->mTime += count;
        }
    }
    return 1;
}

// inferred: stops all the effects
void EffectSystem_unk::fn_80232E48() {
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        Kill_unk(inst);
        inst = next;
    }
    for (int i = 0; i < mParticleCount; i++) {
        mParticles[i].fn_80238560();
    }
}

// inferred: stops the effects with this tag
void EffectSystem_unk::fn_80232F84(int tag) {
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mTag == tag) {
            fn_802328EC(inst, 1);
        }
        inst = next;
    }
}

// inferred: sets flags on the effects of the owner/group
int EffectSystem_unk::fn_80232FF4(unsigned int owner, int group, unsigned int flags) {
    if (group == -1) {
        return 0;
    }
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            inst->mFlags |= (unsigned short)flags;
        }
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if (p->m28 == owner && (group == p->m2C || group == -2)) {
            p->mFlags |= flags;
        }
    }
    return 1;
}

// inferred: clears flags on the effects of the owner/group
int EffectSystem_unk::fn_802330A4(unsigned int owner, int group, unsigned int flags) {
    if (group == -1) {
        return 0;
    }
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            inst->mFlags &= (unsigned short)~flags;
        }
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if (p->m28 == owner && (group == p->m2C || group == -2)) {
            p->mFlags &= ~flags;
        }
    }
    return 1;
}

// inferred: sets the color of the effects of the owner/group
void EffectSystem_unk::fn_80233158(unsigned int owner, int group, const EffectColor_unk* color) {
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            inst->mColor = *color;
        }
    }
}

// inferred: sets a vector of the effects of the owner/group and of their particles
void EffectSystem_unk::fn_802331B0(unsigned int owner, int group, const atn::Vector3* vec) {
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            inst->mVec = *vec;
        }
    }
    for (int i = 0; i < mParticleCount; i++) {
        if (mParticles[i].IsMatch_unk(owner, group)) {
            mParticles[i].m78 = *vec;
        }
    }
}

// inferred: sets the rate of the effects of the owner/group
void EffectSystem_unk::fn_80233260(unsigned int owner, int group, float rate) {
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            inst->mRate = rate;
        }
    }
}

// inferred: key of an animation curve of the efp data (time, value)
struct EffectKey_unk {
    float mTime;
    float mValue;
};
struct EffectCurve_unk {
    unsigned int mCount;
    EffectKey_unk* mKeys;
};
// inferred: the 22 curves of an emitter (0xB0 bytes)
struct EffectCurves_unk {
    EffectCurve_unk mCurves[22];
};
struct EffectAnimData_unk {
    unsigned char m00[0x1C];
    EffectCurves_unk* mEmitters; // 0x1C
};

// inferred: value of a curve at time t, multiplied by (mul) or added to base
// NONMATCHING: 98.0% - FPR/GPR allocation of the loop
extern "C" float fn_8023329C(int curve, int emitter, const EffectAnimData_unk* data, int mul, float t, float base) {
    const EffectCurve_unk& c = data->mEmitters[emitter].mCurves[curve];
    EffectKey_unk* keys = c.mKeys;
    float v = 0.0f;
    for (unsigned int i = 0; i < c.mCount - 1; i++) {
        EffectKey_unk* k0 = &keys[i];
        EffectKey_unk* k1 = &keys[i + 1];
        if (t >= k0->mTime && t <= k1->mTime) {
            float r = (t - k0->mTime) / (k1->mTime - k0->mTime);
            v = r * (k1->mValue - k0->mValue) + k0->mValue;
            break;
        }
    }
    if (mul) {
        v *= base;
    } else {
        v += base;
    }
    return v;
}

// inferred: handle of the effect being created
extern "C" void fn_802346AC(int handle) {
    sCurrentHandle = handle;
}

extern "C" int fn_802346B4() {
    return sCurrentHandle;
}

// inferred: an effect with this handle (high half) is playing
int EffectSystem_unk::fn_802346BC(int handle) {
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if ((inst->mHandle & 0xFFFF0000) == handle) {
            return 1;
        }
    }
    return 0;
}

// inferred: sets flags on the effect with this handle and on its particles
void EffectSystem_unk::fn_802346F0(int handle, unsigned int flags) {
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mHandle == handle) {
            inst->mFlags |= (unsigned short)flags;
        }
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if (p->m30 == handle) {
            p->mFlags |= flags;
        }
    }
}

// inferred: sets a vector of the effect with this handle and of its particles
void EffectSystem_unk::fn_80234764(int handle, const atn::Vector3* vec) {
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mHandle == handle) {
            inst->mVec = *vec;
        }
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if (p->m30 == handle) {
            p->m78 = *vec;
        }
    }
}

// inferred: stops the effect with this handle; fade: its particles fade out instead of
// disappearing
void EffectSystem_unk::fn_802347DC(int handle, int fade) {
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mHandle == handle) {
            if (fade == 0) {
                Kill_unk(inst);
            } else {
                inst->m1A &= 0x80;
            }
        }
        inst = next;
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if (p->m30 == handle) {
            if (fade == 0) {
                p->fn_80238560();
            } else {
                p->mFading = 1;
                p->mFadeMode = 0;
                p->mFadeSpeed = 0.1f;
                p->mFade = 0.0f;
            }
        }
    }
}

// inferred: sets flags on an effect started by atn::EffectManager::fn_8023A680 and on its
// particles
void EffectSystem_unk::fn_802348E4(void* effect, unsigned int flags) {
    for (EffectInst_unk* inst = mInsts; inst != 0; inst = inst->mNext) {
        if (inst->mEffect == effect) {
            inst->mFlags |= (unsigned short)flags;
        }
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if ((void*)p->m08 == effect) {
            p->mFlags |= flags;
        }
    }
}

// inferred: stops an effect started by atn::EffectManager::fn_8023A680 (see fn_802347DC)
void EffectSystem_unk::fn_80234958(void* effect, int fade) {
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mEffect == effect) {
            if (fade == 0) {
                Kill_unk(inst);
            } else {
                inst->m1A &= 0x80;
            }
        }
        inst = next;
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if ((void*)p->m08 == effect) {
            if (fade == 0) {
                p->fn_80238560();
            } else {
                p->mFading = 1;
                p->mFadeMode = 0;
                p->mFadeSpeed = 0.1f;
                p->mFade = 0.0f;
            }
        }
    }
}
