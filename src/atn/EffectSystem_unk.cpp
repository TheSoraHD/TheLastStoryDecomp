// atn/EffectSystem_unk.cpp: the effect system of the efp effects (file name invented): the
// effect instances started by atn::EffectManager (list EffectSystem_unk::mInsts) and the particles
// they emit (EffectSystem_unk::mParticles, code of the particles in atn/EffectRes_unk.cpp), the
// lookups by owner/group and by handle, and the current owner/group of the effects being created.
// Only the small functions are written; the update, draw and creation code is not decompiled yet.
#include "atn/EffectSystem_unk.h"
#include "atn/EffectFileEntry_unk.h"
#include "atn/RenderManager.h"
#include "atn/Camera_unk.h"
#include "atn/Scene.h"
#include "atn/EffectParams_unk.h"
#include "math/VecAssign_unk.h"

using atn::Vector3;

// inferred: current resource, owner, group and handle of the effects being created, and the frame
// offset (names invented; the other variables of .sdata/.sbss are not used by the code written)
static EffectRes_unk* sCurrentRes;   // 0x8087F360
static unsigned int sCurrentOwner;   // 0x8087F364
static int sFrame;                   // 0x8087F368
static int sFrameBase;               // 0x8087F36C
static int sCurrentHandle;           // 0x8087F370
// inferred: time step of the current update and "the frame advances" flag (names invented)
static float sTimeStep = 1.0f;       // 0x8087DBE0
static int sFrameStep = 1;           // 0x8087DBE4
static int sCurrentGroup = -1;       // 0x8087DBE8

// inferred: work buffer of fn_8022D078 (0x30 bytes of floats) and the descriptor passed to fn_8022C9C8
// by fn_8022DA70 and fn_80231754 (count 5, the buffer and two zero words); names invented
struct EffectWork_unk {
    EffectWork_unk(int num, float* data) : mNum(num), mData(data), m08(0), m0C(0) {}
    int mNum;
    float* mData;
    int m08;
    int m0C;
};
static float sEffectWorkBuf_unk[12];
static EffectWork_unk sEffectWork_unk(5, sEffectWorkBuf_unk);

// inferred: stop request of an instance (name invented)
inline void Kill_unk(EffectInst_unk* inst) {
    if (inst) {
        inst->mKill = 1;
    }
}

EffectInst_unk::EffectInst_unk()
    : m00(0), mNext(0), m08(0), m0C(0), m10(0), m14(-1), mFlags(0), m1A(0), mKill(0), m1C(0.0f), mOwnerPtr(0),
      mOwner(0), mGroup(0), mEffect(0), mTag(0), mData(0), mParam(0), mSlots(0), mC8(0.0f), mCC(0.0f), mRate(1.0f) {}

EffectSlot_unk::EffectSlot_unk()
    : m00(0), mNext(0), m08(0), m0C(0), m10(0), m14(0.0f), m18(0.0f), m1C(0), m20(0), m24(0), m28(0.0f), m2C(0.0f),
      m30(0.0f), m34(0.0f), m190(0), m194(0.0f), m198(0.0f), m19C(0.0f), m1A0(0.0f), m1A4(1.0f), m1A8(0), m1AC(0),
      m1B0(0), m1B4(0) {}

EffectFade_unk::EffectFade_unk() : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f) {}

// inferred: releases the fade item of a slot (name invented)
static inline void ReleaseSlot_unk(EffectSystemPool_unk& pool, EffectSlot_unk* slot) {
    if (slot->m1A8 != 0) {
        unsigned int block = (unsigned int)(slot->m1A8 - pool.mItems.mData) / pool.mBlockSize;
        pool.mUsed[block >> 5] &= ~(1 << (block & 31));
    }
    slot->m1A8 = 0;
    slot->m1B4 = 0;
    slot->m1B0 = 0;
    slot->m1AC = 0;
}

// inferred: releases the instances, the slots and the particles
void EffectSystem_unk::fn_8022C2A0() {
    if (mInstPool != 0) {
        delete[] mInstPool;
        mInstPool = 0;
        mInstMax = 0;
    }
    m08 = 0;
    mInsts = 0;
    if (mSlots != 0) {
        for (int i = 0; i < mSlotCount; i++) {
            ReleaseSlot_unk(m2C, &mSlots[i]);
            mSlots[i].m08 = 0;
        }
        delete[] mSlots;
        mSlots = 0;
        mSlotCount = 0;
    }
    m18 = 0;
    m1C = 0;
    if (mParticles != 0) {
        delete[] mParticles;
        mParticles = 0;
        mParticleCount = 0;
    }
}

// inferred: matrix of an instance from its emitter state (prev null: identity), its position and its mode
extern "C" void fn_8022C9C8(void* work, const atn::Matrix3x4* prev, float* out, Vector3* pos, unsigned int mode);
extern "C" void fn_80231754(EffectInst_unk* inst, EffectSlot_unk* slots, float* mtx);

// inferred: moves the instances forward by whole frames (the last one partial)
void EffectSystem_unk::fn_8022D078(float rate) {
    float step;
    float t = rate * lbl_8087EFA8->mTimeScale;
    sTimeStep = 1.0f;
    sFrameStep = 1;
    while (t > 0.0f) {
        m68++;
        if (t < 1.0f) {
            sTimeStep = t;
            m48 += t;
            if (m48 < 1.0f) {
                sFrameStep = 0;
            }
            m48 = fmod(m48, 1.0);
            t = 0.0f;
        } else {
            t -= 1.0f;
        }
        EffectInst_unk* inst = mInsts;
        step = sTimeStep;
        while (inst != 0) {
            sTimeStep = step * inst->mRate;
            if ((inst->mFlags & 2) == 2 || inst->mKill != 0 ||
                ((inst->mFlags & 0x80) == 0x80 && (inst->mFlags & 0x4000) != 0x4000)) {
                inst = inst->mNext;
                continue;
            }
            Vector3 move = inst->mPos;
            fn_8022C9C8(&inst->m08, &inst->mMtx, sEffectWorkBuf_unk, &inst->mPos, inst->mParam->m4C);
            move = move - inst->mPos; // inferred: not used afterwards
            fn_802301EC(inst, inst->mParam, sEffectWorkBuf_unk);
            fn_80231754(inst, inst->mSlots, sEffectWorkBuf_unk);
            EffectInst_unk* next = inst->mNext;
            if ((inst->m1A & 0x7F) == 0 && inst->mSlots == 0) {
                Kill_unk(inst);
            } else {
                inst->m1C += sTimeStep;
                if (inst->m1C >= inst->m14 && inst->m14 >= 0) {
                    inst->m08 = 0;
                    inst->mMtx = *(atn::Matrix3x4*)sEffectWorkBuf_unk;
                    inst->m14 = -1;
                }
            }
            inst = next;
        }
    }
}

// inferred: the instance is drawn in this layer and in the current render pass
extern "C" int fn_8022D328(EffectInst_unk* inst, int layer) {
    if ((inst->mFlags & 4) == 4 || (inst->mFlags & 0x8000) != 0x8000) {
        return 0;
    }
    if (inst->mLayer != layer) {
        return 0;
    }
    int pass = atn::RenderManager::instance_->m2F8;
    unsigned int mode = inst->mParam->m4C;
    unsigned int bits = mode & 0x30000000;
    switch (pass) {
    case 4:
        if ((inst->mFlags & 8) != 8) {
            return 0;
        }
        mode &= 0xC0000000;
        if (mode == 0x40000000 || mode == 0x80000000) {
            return 0;
        }
        break;
    case 6:
        if ((inst->mFlags & 0x10) == 0x10) {
            return 0;
        }
        if (bits != 0x10000000) {
            return 0;
        }
        break;
    case 0x12:
        if ((inst->mFlags & 0x10) == 0x10) {
            return 0;
        }
        if (bits != 0x20000000) {
            return 0;
        }
        break;
    case 0x13:
        if ((inst->mFlags & 0x10) == 0x10) {
            return 0;
        }
        if (bits != 0x30000000) {
            return 0;
        }
        break;
    default:
        if ((inst->mFlags & 0x10) == 0x10) {
            return 0;
        }
        if (bits != 0) {
            return 0;
        }
        break;
    }
    return 1;
}

extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);

// inferred: draws the slots of an instance with the camera matrices
extern "C" void fn_8022DD1C(EffectInst_unk* inst, EffectSlot_unk* slot, atn::Matrix3x4* view, atn::Matrix3x4* invView,
                            atn::Scene* scene);

// inferred: draws the instances of a layer
void EffectSystem_unk::fn_8022D478(int layer) {
    atn::Camera& camera = atn::RenderManager::instance_->mCamera;
    EffectInst_unk* inst = mInsts;
    while (mUpdating != 0) {
    }
    mUpdating = 1;
    atn::Matrix3x4 view = camera.mView;
    atn::Matrix3x4 invView = camera.mInvView;
    atn::Sphere sphere;
    atn::Sphere world; // inferred: read before it is set, as in the original
    for (; inst != 0; inst = inst->mNext) {
        if (inst->mSlots == 0 || fn_8022D328(inst, layer) == 0) {
            continue;
        }
        sphere.mCenter = inst->mPos;
        sphere.mRadius = inst->mCC;
        atn::Scene* scene = atn::RenderManager::instance_->mScene;
        if (atn::RenderManager::instance_->m2F8 == 4) {
            scene = atn::RenderManager::instance_->fn_800C122C(world);
            if (scene == 0) {
                scene = atn::RenderManager::instance_->mScene;
            }
            world = sphere;
            PSMTXMultVec(scene->mReflect.m, &sphere.mCenter, &sphere.mCenter);
        }
        if (fn_8004FF58(&camera.mFrustum, &sphere) == 0) {
            continue;
        }
        if (atn::RenderManager::instance_->m2F8 == 4) {
            atn::Matrix3x4 mtx;
            PSMTXConcat(camera.mView.m, scene->mReflect.m, mtx.m);
            view = mtx;
            atn::Matrix3x4 inv = mtx;
            PSMTXInverse(inv.m, inv.m);
            invView = inv;
        } else if ((inst->mParam->m4C & 4) == 4) {
            scene = atn::RenderManager::instance_->fn_800C122C(world);
            if (scene == 0) {
                scene = atn::RenderManager::instance_->mScene;
            }
        }
        inst->mFlags |= 0x4000;
        for (EffectSlot_unk* slot = inst->mSlots; slot != 0; slot = slot->mNext) {
            if ((slot->m190 & 2) == 2) {
                fn_8022DD1C(inst, slot, &view, &invView, scene);
            }
        }
    }
    mUpdating = 0;
}

// inferred: prepares an entry of the effect data (atn::LoadEffect::vf0C)
extern "C" void fn_8022D754(EffectFileEntry_unk* entry) {
    entry->m10C = entry->m108 * entry->m120;
}

// inferred: reduces the emission of an instance with the distance to the camera (param->m25C bit 0):
// count = count * (1 + t * (m268 - 1)) with t the distance ratio, and the seed moved by the index of
// the instance; free function (the caller passes the system), names invented
void fn_80230034(EffectSystem_unk* system, EffectInst_unk* inst, EffectParam_unk* param, const atn::Matrix3x4* mtx,
                 int* seed, int* count, int* interval) {
    int s = *seed;
    int n = *count;
    int iv = *interval;
    if ((param->m25C & 1) && param->m260 > 0.0f) {
        Vector3 d = *atn::RenderManager::instance_->mCamera.GetPosition_unk() -
                    mtx->GetTrans_unk();
        float t = 1.0f;
        float dist = d.Length_unk();
        if (dist > param->m260) {
            float r = (dist - param->m260) / param->m264;
            if (r > 1.0f) {
                r = 1.0f;
            }
            if (r < t) {
                t = r;
            }
        } else {
            t = 0.0f;
        }
        if (t > 0.0f) {
            float v = 1.0f + t * (param->m268 - 1.0f);
            v *= n;
            if (v > iv) {
                v /= iv;
                iv = 1;
            }
            n = v;
            s += inst - system->mInstPool;
        }
    }
    *seed = s;
    *count = n;
    *interval = iv;
}

// inferred: moves a slot forward along its direction (mCC: distance; mD0/mD4/mD8: rotations x/y/z
// over its matrix)
extern "C" void fn_80231508(EffectSlot_unk* slot) {
    if (slot->mCC == 0.0f) {
        return;
    }
    atn::Matrix3x4 rot;
    if (slot->mD0 != 0.0f || slot->mD4 != 0.0f || slot->mD8 != 0.0f) {
        if (slot->mD0 != 0.0f || slot->mD4 != 0.0f) {
            if (slot->mD0 != 0.0f) {
                atn::Matrix3x4 m;
                atn::Matrix3x4 rx;
                atn::Matrix3x4 ry;
                ry.fn_8013A13C(slot->mD4);
                PSMTXRotRad(rx.m, 'x', slot->mD0);
                PSMTXConcat(ry.m, rx.m, m.m);
                rot = m;
            } else {
                atn::Matrix3x4 ry;
                ry.fn_8013A13C(slot->mD4);
                rot = ry;
            }
        } else {
            rot.SetIdentity();
        }
        if (slot->mD8 != 0.0f) {
            atn::Matrix3x4 rz;
            PSMTXRotRad(rz.m, 'z', slot->mD8);
            PSMTXConcat(rz.m, rot.m, rot.m);
        }
        PSMTXConcat(slot->mMtx.m, rot.m, rot.m);
    } else {
        rot = slot->mMtx;
    }
    Vector3 dir;
    dir = Vector3(0.0f, 0.0f, slot->mCC);
    PSMTXMultVec(rot.m, &dir, &dir);
    slot->m80.x += dir.x;
    slot->m80.y += dir.y;
    slot->m80.z += dir.z;
}

extern "C" int fn_8023193C(EffectInst_unk* inst, EffectParam_unk* param, EffectSlot_unk* slot, Vector3* prev,
                           float* mtx, int first);
extern "C" void fn_80230F3C(EffectInst_unk* inst, EffectSlot_unk* slot);

// inferred: moves the slots of an instance and emits their particles
extern "C" void fn_80231754(EffectInst_unk* inst, EffectSlot_unk* slot, float* mtx) {
    EffectParam_unk* param = inst->mParam;
    for (; slot != 0; slot = slot->mNext) {
        Vector3 prev = slot->m80;
        if (param->m238 < 0 || slot->m18 < param->m238) {
            fn_8022C9C8(&sEffectWork_unk, 0, slot->mMtx.m[0], &slot->m74, param->m4C);
            slot->mMtx.m[0][3] = 0.0f;
            slot->mMtx.m[1][3] = 0.0f;
            slot->mMtx.m[2][3] = 0.0f;
            Vector3 ofs = slot->m68;
            PSMTXMultVec(slot->mMtx.m, &ofs, &ofs);
            slot->m74.x += ofs.x;
            slot->m74.y += ofs.y;
            slot->m74.z += ofs.z;
            slot->m80 = slot->m74 + slot->m8C;
            fn_80231508(slot);
        }
        if ((slot->m190 & 4) == 4) {
            continue;
        }
        slot->m190 |= 8;
        if (fn_8023193C(inst, param, slot, &prev, mtx, slot->m18 == 0.0f)) {
            slot->m190 |= 4;
        } else {
            slot->m18 += sTimeStep;
            fn_80230F3C(inst, slot);
        }
    }
}

// inferred: frame offset of the effects (game/GameTask.cpp)
extern "C" void fn_802328DC(int frame) {
    sFrame = sFrameBase + frame;
}

// inferred: moves an instance forward count frames (stops when it ends)
void EffectSystem_unk::fn_802328EC(EffectInst_unk* inst, int count) {
    if (inst == 0 || (inst->mFlags & 2) == 2 || inst->mKill != 0) {
        return;
    }
    Vector3 move;
    for (int i = 0; i < count; i++) {
        atn::VecAssign_unk(move, inst->mPos);
        fn_8022C9C8(&inst->m08, &inst->mMtx, sEffectWorkBuf_unk, &inst->mPos, inst->mParam->m4C);
        move = move - inst->mPos; // inferred: not used afterwards
        fn_802301EC(inst, inst->mParam, sEffectWorkBuf_unk);
        fn_80231754(inst, inst->mSlots, sEffectWorkBuf_unk);
        if ((inst->m1A & 0x7F) == 0 && inst->mSlots == 0) {
            Kill_unk(inst);
            return;
        }
        inst->m1C += sTimeStep;
        if (inst->m1C >= inst->m14 && inst->m14 >= 0) {
            inst->m08 = 0;
            inst->mMtx = *(atn::Matrix3x4*)sEffectWorkBuf_unk;
            inst->m14 = -1;
        }
    }
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

// inferred: stops the effects of the owner/group (fade: their particles fade out); returns the
// number of instances and particles stopped
int EffectSystem_unk::fn_80232D04(unsigned int owner, int group, int fade) {
    int count = 0;
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mOwner == owner && (inst->mGroup == group || group == -2)) {
            if (fade == 0) {
                Kill_unk(inst);
            } else {
                inst->m1A &= 0x80;
            }
            count++;
        }
        inst = next;
    }
    for (int i = 0; i < mParticleCount; i++) {
        EffectParticle_unk* p = &mParticles[i];
        if (p->m28 == owner && (group == p->m2C || group == -2)) {
            if (fade == 0) {
                p->fn_80238560();
            } else {
                p->mFading = 1;
                p->mFadeMode = 0;
                p->mFadeSpeed = 0.1f;
                p->mFade = 0.0f;
            }
            count++;
        }
    }
    return count;
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

// inferred: stops the effects and the particles with this tag
void EffectSystem_unk::fn_80232ED4(int tag) {
    for (EffectInst_unk* inst = mInsts; inst != 0;) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mTag == tag) {
            Kill_unk(inst);
        }
        inst = next;
    }
    for (int i = 0; i < mParticleCount; i++) {
        if (tag == mParticles[i].m90) {
            mParticles[i].fn_80238560();
        }
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

// inferred: vector parameter of an emitter (curves 12-14 when animated), with its ratios and
// the per-slot offset or scale
extern "C" void fn_80233348(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst) {
    EffectAnimData_unk* data = inst->mData;
    EffectParam_unk* param = inst->mParam;
    if ((data->mAnimFlags[emitter] & 1) == 1) {
        out->x = fn_8023329C(12, emitter, data, 0, 0.0f, 0.0f);
        out->y = fn_8023329C(13, emitter, data, 0, 0.0f, 0.0f);
        out->z = fn_8023329C(14, emitter, data, 0, 0.0f, 0.0f);
    } else {
        out->x = param->mA4.x;
        out->y = param->mA4.y;
        out->z = param->mA4.z;
    }
    if (param->m70 == 2) {
        float x = out->x;
        float zero = 0.0f;
        if (x != zero) {
            slot->m13C = out->y / x;
            slot->m140 = out->z / out->x;
        } else {
            slot->m13C = 0.0f;
            slot->m140 = 0.0f;
        }
    } else {
        slot->m13C = 1.0f;
        slot->m140 = 1.0f;
    }
    if (param->m6C & 1) {
        out->x *= slot->m114.x;
        out->y *= slot->m114.y;
        out->z *= slot->m114.z;
    } else {
        out->x += slot->m114.x;
        out->y += slot->m114.y;
        out->z += slot->m114.z;
    }
}

// inferred: vector parameter of an emitter (curves 15-17 when animated), with its ratios and
// the per-slot offset or scale
extern "C" void fn_802334E0(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst) {
    EffectAnimData_unk* data = inst->mData;
    EffectParam_unk* param = inst->mParam;
    if ((data->mAnimFlags[emitter] & 0x2) == 0x2) {
        out->x = fn_8023329C(15, emitter, data, 0, 0.0f, 0.0f);
        out->y = fn_8023329C(16, emitter, data, 0, 0.0f, 0.0f);
        out->z = fn_8023329C(17, emitter, data, 0, 0.0f, 0.0f);
    } else {
        out->x = param->mE0.x;
        out->y = param->mE0.y;
        out->z = param->mE0.z;
    }
    if (param->m78 == 2) {
        float x = out->x;
        float zero = 0.0f;
        if (x != zero) {
            slot->m144 = out->y / x;
            slot->m148 = out->z / out->x;
        } else {
            slot->m144 = 0.0f;
            slot->m148 = 0.0f;
        }
    } else {
        slot->m144 = 1.0f;
        slot->m148 = 1.0f;
    }
    if (param->m74 & 1) {
        out->x *= slot->m120.x;
        out->y *= slot->m120.y;
        out->z *= slot->m120.z;
    } else {
        out->x += slot->m120.x;
        out->y += slot->m120.y;
        out->z += slot->m120.z;
    }
}

// inferred: vector parameter of an emitter (curves 0-2 when animated), with its ratios and
// the per-slot offset or scale
extern "C" void fn_80233678(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst) {
    EffectAnimData_unk* data = inst->mData;
    EffectParam_unk* param = inst->mParam;
    if ((data->mAnimFlags[emitter] & 0x4) == 0x4) {
        out->x = fn_8023329C(0, emitter, data, 0, 0.0f, 0.0f);
        out->y = fn_8023329C(1, emitter, data, 0, 0.0f, 0.0f);
        out->z = fn_8023329C(2, emitter, data, 0, 0.0f, 0.0f);
    } else {
        out->x = param->m11C.x;
        out->y = param->m11C.y;
        out->z = param->m11C.z;
    }
    if (param->m80 == 2) {
        float x = out->x;
        float zero = 0.0f;
        if (x != zero) {
            slot->m14C = out->y / x;
            slot->m150 = out->z / out->x;
        } else {
            slot->m14C = 0.0f;
            slot->m150 = 0.0f;
        }
    } else {
        slot->m14C = 1.0f;
        slot->m150 = 1.0f;
    }
    if (param->m7C & 1) {
        out->x *= slot->mE4.x;
        out->y *= slot->mE4.y;
        out->z *= slot->mE4.z;
    } else {
        out->x += slot->mE4.x;
        out->y += slot->mE4.y;
        out->z += slot->mE4.z;
    }
}

// inferred: vector parameter of an emitter (curves 19-21 when animated), with its ratios and
// the per-slot offset or scale
extern "C" void fn_80233810(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst) {
    EffectAnimData_unk* data = inst->mData;
    EffectParam_unk* param = inst->mParam;
    if ((data->mAnimFlags[emitter] & 0x80) == 0x80) {
        out->x = fn_8023329C(19, emitter, data, 0, 0.0f, 0.0f);
        out->y = fn_8023329C(20, emitter, data, 0, 0.0f, 0.0f);
        out->z = fn_8023329C(21, emitter, data, 0, 0.0f, 0.0f);
    } else {
        out->x = param->m1F8.x;
        out->y = param->m1F8.y;
        out->z = param->m1F8.z;
    }
    if (param->mA0 == 2) {
        float x = out->x;
        float zero = 0.0f;
        if (x != zero) {
            slot->m16C = out->y / x;
            slot->m170 = out->z / out->x;
        } else {
            slot->m16C = 0.0f;
            slot->m170 = 0.0f;
        }
    } else {
        slot->m16C = 1.0f;
        slot->m170 = 1.0f;
    }
    if (param->m9C & 1) {
        out->x *= slot->m130.x;
        out->y *= slot->m130.y;
        out->z *= slot->m130.z;
    } else {
        out->x += slot->m130.x;
        out->y += slot->m130.y;
        out->z += slot->m130.z;
    }
}

// inferred: vector parameter of an emitter (curves 7-9 when animated), with its ratios and
// the per-slot offset or scale
extern "C" void fn_802339A8(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst) {
    EffectAnimData_unk* data = inst->mData;
    EffectParam_unk* param = inst->mParam;
    if ((data->mAnimFlags[emitter] & 0x8) == 0x8) {
        out->x = fn_8023329C(7, emitter, data, 0, 0.0f, 0.0f);
        out->y = fn_8023329C(8, emitter, data, 0, 0.0f, 0.0f);
        out->z = fn_8023329C(9, emitter, data, 0, 0.0f, 0.0f);
    } else {
        out->x = param->m158.x;
        out->y = param->m158.y;
        out->z = param->m158.z;
    }
    if (param->m88 == 2) {
        float x = out->x;
        float zero = 0.0f;
        if (x != zero) {
            slot->m154 = out->y / x;
            slot->m158 = out->z / out->x;
        } else {
            slot->m154 = 0.0f;
            slot->m158 = 0.0f;
        }
    } else {
        slot->m154 = 1.0f;
        slot->m158 = 1.0f;
    }
    if (param->m84 & 1) {
        out->x *= slot->m100.x;
        out->y *= slot->m100.y;
        out->z *= slot->m100.z;
    } else {
        out->x += slot->m100.x;
        out->y += slot->m100.y;
        out->z += slot->m100.z;
    }
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
