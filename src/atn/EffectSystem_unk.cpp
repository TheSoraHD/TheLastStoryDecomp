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
// inferred: serial number of the effects started (1..100000; name invented)
static unsigned int sSerial_unk = 1; // 0x8087DBEC

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

// inferred: word of the bitmap of the pool for a block (name invented)
static inline unsigned int& UsedWord_unk(EffectSystemPool_unk& pool, unsigned int i) {
    return *(unsigned int*)&pool.mUsed[i >> 5];
}

// inferred: releases the fade item of a slot (name invented)
static inline void ReleaseSlot_unk(EffectSystemPool_unk& pool, EffectSlot_unk* slot) {
    if (slot->m1A8 != 0) {
        unsigned int block = (unsigned int)(slot->m1A8 - pool.mItems.mData) / pool.mBlockSize;
        UsedWord_unk(pool, block) &= ~(1 << (block & 31));
    }
    slot->m1A8 = 0;
    slot->m1B4 = 0;
    slot->m1B0 = 0;
    slot->m1AC = 0;
}

// inferred: sets up the pool: count blocks of 0x40 items and their bitmap (name invented)
static inline void InitPool_unk(EffectSystemPool_unk& pool, unsigned int count) {
    if (pool.mBlockSize == 0x40 && count == pool.mBlockCount) {
        return;
    }
    pool.mBlockSize = 0x40;
    pool.mBlockCount = count;
    pool.mItems.Resize_unk(count << 6, 7);
    pool.mUsed.resize(count);
}

extern "C" void fn_8023793C(void* used, void* free, void* buffer, int stride, int count);

// NONMATCHING: 99.5% - index and offset of the slot loop swap r10/r11; registers of the inlined growth
// of the bitmap vector (see fn_8022BEAC)
// inferred: allocates the instances, the slots and the particles, sets up the fade pool (0x120
// blocks) and links the free lists
void EffectSystem_unk::fn_8022B850(int instMax, int slotCount, int particleCount) {
    mInstMax = instMax;
    mInstPool = new ((MemoryTag)7, "", "", 0) EffectInst_unk[instMax];
    mSlotCount = slotCount;
    mSlots = new ((MemoryTag)7, "", "", 0) EffectSlot_unk[slotCount];
    mParticleCount = particleCount;
    mParticles = new ((MemoryTag)7, "", "", 0) EffectParticle_unk[particleCount];
    InitPool_unk(m2C, 0x120);
    fn_8023793C(&mInsts, &m08, mInstPool, sizeof(EffectInst_unk), mInstMax);
    for (int i = 0; i < mSlotCount; i++) {
        ReleaseSlot_unk(m2C, &mSlots[i]);
        mSlots[i].m08 = 0;
    }
    fn_8023793C(&m1C, &mFreeSlots, mSlots, sizeof(EffectSlot_unk), mSlotCount);
}

EffectInst_unk::EffectInst_unk()
    : m00(0), mNext(0), m08(0), m0C(0), m10(0), m14(-1), mFlags(0), m1A(0), mKill(0), m1C(0.0f), mOwnerPtr(0),
      mOwner(0), mGroup(0), mEffect(0), mTag(0), mData(0), mParam(0), mSlots(0), mC8(0.0f), mCC(0.0f), mRate(1.0f) {}

EffectSlot_unk::EffectSlot_unk()
    : m00(0), mNext(0), m08(0), m0C(0), m10(0), m14(0.0f), m18(0.0f), m1C(0), m20(0), m24(0), m28(0.0f), m2C(0.0f),
      m30(0.0f), m34(0.0f), m190(0), m194(0.0f, 0.0f, 0.0f), m1A0(0.0f), m1A4(1.0f), m1A8(0), m1AC(0),
      m1B0(0), m1B4(0) {}

EffectFade_unk::EffectFade_unk() : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f) {}

// NONMATCHING: 99.4% - registers and stack slots of the inlined growth of the bitmap vector
// inferred: sets up the fade pool
void EffectSystem_unk::fn_8022BEAC(unsigned int count) {
    InitPool_unk(m2C, count);
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
    mFreeSlots = 0;
    m1C = 0;
    if (mParticles != 0) {
        delete[] mParticles;
        mParticles = 0;
        mParticleCount = 0;
    }
}

extern "C" void* fn_80237C88(void* from, void* to, void* node); // inferred: moves a node to another list
extern "C" void* fn_80237B8C(void* used, void* free, int recycle); // inferred: takes a node of the free list

// inferred: releases a slot of an instance and moves it to the free list (name invented)
static inline void FreeSlot_unk(EffectSystem_unk* system, EffectInst_unk* inst, EffectSlot_unk* slot) {
    ReleaseSlot_unk(system->m2C, slot);
    slot->m08 = 0;
    slot->m190 = 0;
    fn_80237C88(&inst->mSlots, &system->mFreeSlots, slot);
}

// inferred: largest of two values (name invented)
template <class T>
static inline T EffectMaxT_unk(T a, T b) {
    return a > b ? a : b;
}

// NONMATCHING: 99.3% - the ring size m1AC of the loop condition is loaded before the max of mB4 (r3)
// inferred: frees the stopped instances and slots, keeps the fade rings of the slots, counts the slots
// and the draw passes used; when the slots run out, frees one of three slots of the instances with flag
// 0x40
void EffectSystem_unk::fn_8022C3F0() {
    m4C = 0;
    m50 = 0;
    m58 = 0;
    m5C = 0;
    m60 = 0;
    mFlags = 0;
    EffectInst_unk* inst = mInsts;
    while (inst != 0) {
        EffectInst_unk* next = inst->mNext;
        if (inst->mKill != 0) {
            EffectSlot_unk* slot = inst->mSlots;
            while (slot != 0) {
                EffectSlot_unk* nextSlot = slot->mNext;
                FreeSlot_unk(this, inst, slot);
                m50++;
                slot = nextSlot;
            }
            inst->mFlags &= ~0x8001;
            fn_80237C88(&mInsts, &m08, inst);
            m4C++;
        } else {
            EffectSlot_unk* slot = inst->mSlots;
            while (slot != 0) {
                EffectSlot_unk* nextSlot = slot->mNext;
                if ((slot->m190 & 4) == 4) {
                    FreeSlot_unk(this, inst, slot);
                    m50++;
                } else {
                    if ((slot->m190 & 8) == 8 && (inst->mParam->m4C & 0x20000) && sFrameStep != 0) {
                        if (slot->m1A8 != 0) {
                            if (slot->m1AC == slot->m1B4) {
                                slot->m1B0 = (slot->m1B0 + 1) % slot->m1B4;
                                slot->m1AC = EffectMaxT_unk<unsigned int>(slot->m1AC - 1, 0);
                            }
                            EffectFade_unk* fade = &slot->m1A8[(slot->m1B0 + slot->m1AC) % slot->m1B4];
                            *(Vector3*)&fade->m00 = slot->m194;
                            fade->m0C = slot->m1A0;
                            fade->m10 = slot->m1A4;
                            slot->m1AC++;
                        }
                        while ((float)slot->m1AC > (slot->mB4 > 0.0f ? slot->mB4 : 0.0f)) {
                            slot->m1B0 = (slot->m1B0 + 1) % slot->m1B4;
                            slot->m1AC = EffectMaxT_unk<unsigned int>(slot->m1AC - 1, 0);
                        }
                    }
                    slot->m190 = (slot->m190 | 2) & ~8;
                    m50++;
                    if (inst->mParam->m4C & 0x20000) {
                        m58++;
                        int n = (int)inst->mParam->m11C.z > 1 ? (int)inst->mParam->m11C.z : 1;
                        m5C += n * slot->m1AC * 2;
                    } else if (inst->mData->mEntries[inst->mParam->m40].mType == 1) {
                        m60++;
                    } else {
                        m5C += 4;
                    }
                }
                slot = nextSlot;
            }
            inst->mCC = inst->mC8;
            inst->mFlags = (inst->mFlags | 0x8000) & 0xBFFF;
            switch (inst->mParam->m4C & 0x30000000) {
            case 0x10000000:
                mFlags |= 1;
                break;
            case 0x20000000:
                mFlags |= 2;
                break;
            case 0x30000000:
                mFlags |= 4;
                break;
            }
            if ((inst->mFlags & 8) == 8) {
                mFlags |= 8;
            }
            m4C++;
        }
        inst = next;
    }
    if (m50 >= mSlotCount - 0x80) {
        int freed = 0;
        EffectInst_unk* inst = mInsts;
        while (inst != 0) {
            if ((inst->mFlags & 0x40) == 0x40) {
                int i = 0;
                EffectSlot_unk* slot = inst->mSlots;
                while (slot != 0) {
                    EffectSlot_unk* nextSlot = slot->mNext;
                    if (slot->m14 != 0.0f && i % 3 == 0) {
                        FreeSlot_unk(this, inst, slot);
                        freed++;
                    }
                    slot = nextSlot;
                    i++;
                    if (m50 - freed < mSlotCount - 0x80) {
                        break;
                    }
                }
            }
            inst = inst->mNext;
            if (m50 - freed < mSlotCount - 0x80) {
                break;
            }
        }
    }
}

extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);

// inferred: length of a column of a matrix (name invented)
static inline float EffectColLen_unk(const atn::Matrix3x4& m, int i) {
    Vector3 v(m.m[0][i], m.m[1][i], m.m[2][i]);
    return PSVECMag(&v);
}

// inferred: m = m * rot(axis, rad) when the angle is not zero (name invented)
static inline atn::Matrix3x4& EffectRotate_unk(atn::Matrix3x4& m, char axis, float rad) {
    if (0.0f != rad) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, axis, rad);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}

// inferred: matrix of a position and angles: trans(pos) * rot_y * rot_x * rot_z (name invented)
static inline void EffectPosRotMtx_unk(atn::Matrix3x4* out, const Vector3* pos, const Vector3* rot) {
    atn::Matrix3x4 res;
    atn::Matrix3x4 m;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    EffectRotate_unk(*p, 'y', rot->y);
    EffectRotate_unk(*p, 'x', rot->x);
    EffectRotate_unk(*p, 'z', rot->z);
    atn::Matrix3x4 t;
    PSMTXTrans(t.m, pos->x, pos->y, pos->z);
    PSMTXConcat(t.m, p->m, res.m);
    *out = res;
}

// NONMATCHING: 98.3% - mode 1: the original keeps pointers to the camera position and angles (r25/r27)
// and the RenderManager in r4; stack slots of the temporaries (frame 0x10 smaller)
// inferred: matrix of an instance from its emitter state (prev null: identity), its position and its mode:
// 1 at the camera (bit 0: position only), 3 at a matrix of a node (bit 0: its translation only), 5 at a
// matrix (bit 0: its translation, with mode bit 6 scaled by its largest axis), 6 at a position, 4 kept,
// otherwise prev
extern "C" void fn_8022C9C8(void* work, const atn::Matrix3x4* prev, float* out, Vector3* pos, unsigned int mode) {
    EffectWork_unk* w = (EffectWork_unk*)work;
    atn::Matrix3x4 tmpOut;
    atn::Matrix3x4 ident;
    if (prev == 0) {
        ident.SetIdentity();
        prev = &ident;
    }
    if (out == 0) {
        out = tmpOut.m[0];
    }
    switch ((unsigned int)w->mNum) {
    case 1: {
        atn::RenderManager* rm = atn::RenderManager::instance_;
        const Vector3* camRot = rm->mCamera.GetRot_unk();
        const Vector3* camPos = rm->mCamera.GetPosition_unk();
        atn::Matrix3x4 base;
        if (w->m08 & 1) {
            atn::Matrix3x4 t;
            PSMTXTrans(t.m, rm->mCamera.mPos.x, rm->mCamera.mPos.y, rm->mCamera.mPos.z);
            base = t;
        } else {
            EffectPosRotMtx_unk(&base, camPos, camRot);
        }
        atn::Matrix3x4 res;
        PSMTXConcat(base.m, prev->m, res.m);
        *(atn::Matrix3x4*)out = res;
        break;
    }
    case 3:
        if (w->m08 & 1) {
            Vector3 v = ((atn::Matrix3x4*)((char*)w->mData + 8))->GetTrans_unk();
            atn::Matrix3x4 t;
            PSMTXTrans(t.m, v.x, v.y, v.z);
            atn::Matrix3x4 res;
            PSMTXConcat(t.m, prev->m, res.m);
            *(atn::Matrix3x4*)out = res;
        } else {
            atn::Matrix3x4 res;
            PSMTXConcat(((atn::Matrix3x4*)((char*)w->mData + 8))->m, prev->m, res.m);
            *(atn::Matrix3x4*)out = res;
        }
        break;
    case 4:
        break;
    case 5:
        if (w->m08 & 1) {
            Vector3 v = ((atn::Matrix3x4*)w->mData)->GetTrans_unk();
            atn::Matrix3x4 t;
            PSMTXTrans(t.m, v.x, v.y, v.z);
            if (mode & 0x40) {
                atn::Matrix3x4* m = (atn::Matrix3x4*)w->mData;
                Vector3 s(EffectColLen_unk(*m, 0), EffectColLen_unk(*m, 1), EffectColLen_unk(*m, 2));
                float l = s.x > (s.y > s.z ? s.y : s.z) ? s.x : (s.y > s.z ? s.y : s.z);
                Vector3 sc(l, l, l);
                atn::Matrix3x4 sm;
                PSMTXScale(sm.m, sc.x, sc.y, sc.z);
                atn::Matrix3x4 ts;
                PSMTXConcat(t.m, sm.m, ts.m);
                t = ts;
            }
            atn::Matrix3x4 res;
            PSMTXConcat(t.m, prev->m, res.m);
            *(atn::Matrix3x4*)out = res;
        } else {
            atn::Matrix3x4 res;
            PSMTXConcat(((atn::Matrix3x4*)w->mData)->m, prev->m, res.m);
            *(atn::Matrix3x4*)out = res;
        }
        break;
    case 6: {
        Vector3* v = (Vector3*)w->mData;
        atn::Matrix3x4 t;
        PSMTXTrans(t.m, v->x, v->y, v->z);
        atn::Matrix3x4 res;
        PSMTXConcat(t.m, prev->m, res.m);
        *(atn::Matrix3x4*)out = res;
        break;
    }
    default:
        *(atn::Matrix3x4*)out = *prev;
        break;
    }
    if (pos != 0) {
        *pos = ((atn::Matrix3x4*)out)->GetTrans_unk();
    }
}
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
    entry->m10C = entry->m108 * entry->mUV.w;
}

extern "C" double cos(double x);
extern "C" int rand();

// NONMATCHING: 99.2% - the curve mode and the reloaded frame count swap r26/r27
// inferred: texture frame of an entry for a frame number (uv: first frame in, frame out; last: random
// state); returns the frame index
extern "C" int fn_8022D790(EffectUV_unk* uv, EffectFileEntry_unk* entry, int frame, int unused, int* last) {
    int n = entry->m108;
    float h, w, v, u;
    u = entry->mUV.u;
    v = entry->mUV.v;
    w = entry->mUV.w;
    h = entry->mUV.h;
    uv->u = u;
    uv->v = v;
    uv->w = w;
    uv->h = h;
    if (n <= 1) {
        return 0;
    }
    int loop = frame / n;
    int index = frame % n;
    if (entry->m130 > 0.0f && loop >= entry->m130) {
        index = n - 1;
    }
    float t = (float)index / (float)n;
    int mode = entry->m12C == 5 ? 0 : entry->m12C;
    switch (mode) {
    case 2: {
        t *= 2.0f;
        float d = 1.0f - t;
        if (d < 0.0f) {
            d = -d;
        }
        t = 1.0f - d;
        break;
    }
    case 3:
        t *= 2.0f;
        t = 0.5f * (1.0f + (float)cos(3.1415927f * (1.0f + t)));
        break;
    case 1:
        t = 0.5f * (1.0f + (float)cos(3.1415927f * (1.0f + t)));
        break;
    default: {
        float d = 1.0f - t;
        if (d < 0.0f) {
            d = -d;
        }
        t = 1.0f - d;
        break;
    }
    }
    int count = entry->m108;
    int f = count * t;
    if (mode == 6) {
        f = count - f;
    }
    if (count == f) {
        f = count - 1;
    }
    if (mode == 4) {
        if (*last != index) {
            *last = f;
            count = entry->m108;
            index = rand() % count;
        }
    } else {
        index = (*last + f) % count;
    }
    uv->u += index * (entry->m10C / count);
    if (uv->u >= 1.0f) {
        int w = uv->u;
        uv->u -= w;
        uv->v = uv->h * w + uv->v;
    }
    return index;
}

extern "C" int fn_80230780(void* owner, EffectInst_unk* inst, EffectParam_unk* param, EffectSlot_unk* slot, float t);
extern "C" void fn_80231508(EffectSlot_unk* slot);

// inferred: takes a free block of the pool (null when all are used; name invented)
static inline bool IsUsed_unk(EffectSystemPool_unk& pool, unsigned int i) {
    return (UsedWord_unk(pool, i) & (1 << (i & 31))) != 0;
}
static inline void SetUsed_unk(EffectSystemPool_unk& pool, unsigned int i) {
    UsedWord_unk(pool, i) |= 1 << (i & 31);
}
static inline EffectFade_unk* AllocFade_unk(EffectSystemPool_unk& pool) {
    for (unsigned int i = 0; i < pool.mBlockCount; i++) {
        if (!IsUsed_unk(pool, i)) {
            SetUsed_unk(pool, i);
            return &pool.mItems.mData[i * pool.mBlockSize];
        }
    }
    return 0;
}

// NONMATCHING: 98.9% - the pool bitmap word is loaded once (the original loads it again for the |=)
// inferred: starts a slot of an instance: takes it from the free list, sets its matrix and position
// and its fade items (param->m4C bit 17); free function (the caller passes the system)
EffectSlot_unk* fn_8022DA70(EffectSystem_unk* system, void* owner, EffectInst_unk* inst, EffectParam_unk* param,
                            float t) {
    EffectSlot_unk* slot = (EffectSlot_unk*)fn_80237B8C(&inst->mSlots, &system->mFreeSlots, 0);
    if (slot == 0) {
        return 0;
    }
    slot->m18 = 0.0f;
    slot->m190 = 0;
    fn_8022C9C8(&sEffectWork_unk, 0, slot->mMtx.m[0], &slot->m74, param->m4C);
    slot->mMtx.m[0][3] = 0.0f;
    slot->mMtx.m[1][3] = 0.0f;
    slot->mMtx.m[2][3] = 0.0f;
    slot->m8C.x = 0.0f;
    slot->m8C.y = 0.0f;
    slot->m8C.z = 0.0f;
    slot->m68.x = 0.0f;
    slot->m68.y = 0.0f;
    slot->m68.z = 0.0f;
    if (fn_80230780(owner, inst, param, slot, t) == 0) {
        slot->m190 |= 4;
        return 0;
    }
    Vector3 ofs = slot->m68;
    PSMTXMultVec(slot->mMtx.m, &ofs, &ofs);
    slot->m74.x += ofs.x;
    slot->m74.y += ofs.y;
    slot->m74.z += ofs.z;
    slot->m80 = slot->m74 + slot->m8C;
    fn_80231508(slot);
    if (param->m4C & 0x20000) {
        slot->m1A8 = AllocFade_unk(system->m2C);
        slot->m1B4 = 0x40;
        slot->m1B0 = 0;
        slot->m1AC = 0;
        float a4, b0;
        b0 = slot->mB0;
        a4 = slot->mA4;
        slot->m194 = slot->m80;
        slot->m1A0 = a4;
        slot->m1A4 = b0;
    } else {
        ReleaseSlot_unk(system->m2C, slot);
    }
    slot->m190 |= 8;
    return slot;
}

// inferred: owner and group of a particle (name invented)
static inline void SetParticleOwner_unk(EffectParticle_unk& p, unsigned int owner, int group) {
    p.m28 = owner;
    p.m2C = group;
}

// NONMATCHING: 99.8% - stack slots of the temporaries of the y and z rotations swapped
// inferred: starts an effect: one instance per emitter enabled (emitter < 0: all of them), at the
// position and angles given, with its color and scale, moved forward the frame offset; then the
// particles enabled; returns the serial number of the effect (0: none started)
void* EffectSystem_unk::fn_8022FAC4(void* res, int emitter, int b, int c, const Vector3* pos, const Vector3* rot,
                                    const EffectColor_unk* color, int d, int e, float scale) {
    EffectAnimData_unk* data = (EffectAnimData_unk*)res;
    if (data == 0) {
        return 0;
    }
    Vector3 zero(0.0f, 0.0f, 0.0f);
    EffectColor_unk white(1.0f, 1.0f, 1.0f, 1.0f);
    if (pos == 0) {
        pos = &zero;
    }
    if (rot == 0) {
        rot = &zero;
    }
    if (color == 0) {
        color = &white;
    }
    int start;
    unsigned int end;
    if (emitter < 0) {
        end = data->m00;
        start = 0;
    } else {
        start = emitter;
        end = emitter + 1;
    }
    if (data->m00 < end) {
        return 0;
    }
    if (++sSerial_unk > 100000) {
        sSerial_unk = 1;
    }
    EffectInst_unk* first = 0;
    for (int i = start; i < (int)end; i++) {
        if (data->mEmitterOn[i] == 0) {
            continue;
        }
        EffectInst_unk* inst = (EffectInst_unk*)fn_80237B8C(&mInsts, &m08, 0);
        if (inst == 0) {
            continue;
        }
        if (first == 0) {
            first = inst;
        }
        inst->mFlags = 1;
        if (m78 != 0) {
            inst->mFlags |= 0x20;
        }
        if (m7C != 0) {
            inst->mFlags |= 0x100;
        }
        inst->m1C = 0.0f;
        EffectParam_unk* param = &data->mParams[i];
        inst->mData = data;
        inst->mParam = param;
        inst->mSlots = 0;
        inst->m1A = 1;
        inst->mOwnerPtr = sCurrentRes;
        inst->mOwner = sCurrentOwner;
        inst->mGroup = sCurrentGroup;
        inst->mHandle = sCurrentHandle;
        inst->mEffect = (void*)sSerial_unk;
        inst->mTag = mLayer;
        inst->mLayer = m70;
        inst->mA8 = scale * inst->mParam->m64;
        inst->mColor.r = color->r;
        inst->mColor.g = color->g;
        inst->mColor.b = color->b;
        inst->mColor.a = color->a;
        inst->m08 = b;
        inst->m0C = (float*)c;
        inst->m10 = d;
        inst->m14 = e;
        EffectPosRotMtx_unk(&inst->mMtx, pos, rot);
        inst->m6C = inst->mMtx;
        fn_8022C9C8(&inst->m08, &inst->mMtx, 0, &inst->mPos, inst->mParam->m4C);
        inst->mKill = 0;
        inst->mC8 = 0.0f;
        inst->mCC = 0.0f;
        inst->mVec = Vector3(0.0f, 0.0f, 0.0f);
        inst->mRate = 1.0f;
        if (sFrame > 0) {
            fn_802328EC(inst, sFrame);
        }
    }
    if (first != 0) {
        for (unsigned int j = 0; j < data->mParticleDefCount; j++) {
            if (data->mParticleOn[j] == 0) {
                continue;
            }
            for (int k = 0; k < mParticleCount; k++) {
                if (!mParticles[k].IsActive_unk()) {
                    mParticles[k].fn_802383D8(this, (EffectInstDef_unk*)first, j);
                    mParticles[k].m24 = (int)sCurrentRes;
                    SetParticleOwner_unk(mParticles[k], sCurrentOwner, sCurrentGroup);
                    mParticles[k].m30 = sCurrentHandle;
                    break;
                }
            }
        }
    }
    return (void*)(first != 0 ? sSerial_unk : 0);
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

extern "C" float fn_8023329C(int curve, int emitter, const EffectAnimData_unk* data, int mul, float t, float base);

// NONMATCHING: 89% - written from the original; the vector temporaries and the order of the stores of
// the translation differ
// inferred: moves an instance (animated: by the curves 12-14 from its first frame; else by its velocity,
// damped per axis) and starts its slots every period frames
void EffectSystem_unk::fn_802301EC(EffectInst_unk* inst, EffectParam_unk* param, float* mtx) {
    if ((inst->m1A & 0x7F) == 0 || sFrameStep == 0) {
        return;
    }
    if (!(inst->m1C >= param->m48)) {
        return;
    }
    int frame = (int)inst->m1C - param->m48;
    if (param->m54 > 0 && param->m54 <= frame) {
        inst->m1A &= 0x80;
    }
    if ((inst->m1A & 0x7F) == 0) {
        return;
    }
    int period = param->m58 < 1 ? 1 : param->m58;
    int count = param->m5C < 1 ? 1 : param->m5C;
    fn_80230034(this, inst, param, (const atn::Matrix3x4*)mtx, &frame, &period, &count);
    EffectAnimData_unk* data = inst->mData;
    int emitter = param - data->mParams;
    if ((data->mAnimFlags[emitter] & 1) == 1 && param->m54 > 0) {
        atn::Matrix3x4 rot = inst->m6C;
        rot.m[0][3] = 0.0f;
        rot.m[1][3] = 0.0f;
        rot.m[2][3] = 0.0f;
        inst->mMtx = inst->m6C;
        float t = (inst->m1C - param->m48) / param->m54;
        Vector3 d;
        float base = fn_8023329C(12, emitter, data, 0, 0.0f, 0.0f);
        d.x = fn_8023329C(12, emitter, inst->mData, 0, t, 0.0f) - base;
        base = fn_8023329C(13, emitter, inst->mData, 0, 0.0f, 0.0f);
        d.y = fn_8023329C(13, emitter, inst->mData, 0, t, 0.0f) - base;
        base = fn_8023329C(14, emitter, inst->mData, 0, 0.0f, 0.0f);
        d.z = fn_8023329C(14, emitter, inst->mData, 0, t, 0.0f) - base;
        d.x *= inst->mA8;
        d.y *= inst->mA8;
        d.z *= inst->mA8;
        PSMTXMultVec(rot.m, &d, &d);
        d += inst->mMtx.GetTrans_unk();
        inst->mMtx.fn_801479E4(d);
    } else {
        atn::Matrix3x4 rot = inst->m6C;
        rot.m[0][3] = 0.0f;
        rot.m[1][3] = 0.0f;
        rot.m[2][3] = 0.0f;
        Vector3 v = param->mBC;
        v.x *= inst->mA8;
        v.y *= inst->mA8;
        v.z *= inst->mA8;
        PSMTXMultVec(rot.m, &v, &v);
        v = v * sTimeStep;
        v += inst->mMtx.GetTrans_unk();
        inst->mMtx.fn_801479E4(v);
        if (sFrameStep != 0) {
            v = Vector3(v.x * param->mC8, v.y * param->mCC, v.z * param->mD0);
            inst->mMtx.fn_801479E4(v);
        }
    }
    inst->mC8 = 0.0f;
    inst->mMtx.fn_801479E4(inst->mMtx.GetTrans_unk() + inst->mVec * sTimeStep);
    if (frame % period == 0) {
        for (int i = 0; i < count; i++) {
            fn_8022DA70(this, inst->mData, inst, param, (float)i / (float)count);
        }
    }
}

extern "C" void fn_80233D74(EffectSlot_unk* slot, EffectParam_unk* param);
extern "C" void fn_80233348(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst);
extern "C" void fn_802334E0(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst);
extern "C" void fn_80233678(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst);
extern "C" void fn_80233810(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst);
extern "C" void fn_802339A8(Vector3* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst);
extern "C" void fn_80233B40(EffectColor_unk* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst);

// inferred: flag of the animation of an emitter (name invented)
static inline bool EffectAnimFlag_unk(const EffectAnimData_unk* data, int emitter, unsigned int flag) {
    return (data->mAnimFlags[emitter] & flag) == flag;
}

// inferred: value clamped to [0, 1] (name invented)
static inline void EffectClamp01_unk(float& v) {
    if (v < 0.0f) {
        v = 0.0f;
    } else if (v > 1.0f) {
        v = 1.0f;
    }
}

// NONMATCHING: 99.9% - else branch: the entries pointer is loaded after param->m40 (and r5/r7 in the
// emitter index)
// inferred: sets up a new slot of an instance: its model or texture (and its frames), its offset, size,
// direction, speed, color and the draw flags from the parameters of the emitter; 0 when the entry has
// nothing to draw
extern "C" int fn_80230780(void* owner, EffectInst_unk* inst, EffectParam_unk* param, EffectSlot_unk* slot, float t) {
    EffectAnimData_unk* data = (EffectAnimData_unk*)owner;
    EffectFileEntry_unk* entries = data->mEntries;
    EffectFileEntry_unk* entry = &entries[param->m40];
    if (entry->mType == 1) {
        slot->m08 = (EffectInst_unk*)data->mResources[param->m40];
        slot->m14 = (float)(unsigned int)param->m50;
        slot->m1C = &entries[param->m40];
    } else {
        slot->m1C = entry;
        if ((unsigned int)entry->m128 == 1) {
            slot->m20 = entry->m108 != 0 ? rand() % entry->m108 : 0;
        } else {
            slot->m20 = 0;
        }
        slot->m24 = slot->m20;
        if (data->mEntries[param->m40].mType == 2) {
            void** res = data->mResources;
            slot->m08 = (EffectInst_unk*)res[param->m40];
            slot->m0C = (int)((char*)res[param->m40] + 0x30);
            if ((param->m4C & 0xC0000000) == 0x80000000) {
                slot->m08 = (EffectInst_unk*)slot->m0C;
            }
        } else {
            slot->m08 = (EffectInst_unk*)data->mResources[param->m40];
            slot->m0C = 0;
        }
        if (param->m50 == 0) {
            slot->m14 = (float)slot->m1C->m108 * param->m23C;
        } else {
            slot->m14 = (float)(unsigned int)param->m50;
        }
    }
    if (slot->m08 == 0) {
        return 0;
    }
    fn_80233D74(slot, param);
    int emitter = param - inst->mData->mParams;
    Vector3 ofs;
    fn_80233348(&ofs, slot, emitter, inst);
    ofs.x *= inst->mA8;
    ofs.y *= inst->mA8;
    ofs.z *= inst->mA8;
    if (param->m70 != 0) {
        ofs.y = slot->m13C * ofs.x;
        ofs.z = slot->m140 * ofs.x;
    }
    fn_802334E0((Vector3*)&slot->mA4, slot, emitter, inst);
    if (param->m4C & 1) {
        if (param->m78 != 0) {
            slot->mA8 = slot->m144 * slot->mA4;
            slot->mAC = slot->m148 * slot->mA4;
        }
    } else if ((inst->mData->mAnimFlags[emitter] & 2) != 2) {
        slot->mA4 = 0.0f;
        slot->mA8 = 0.0f;
        slot->mAC = param->mE0.z + slot->m120.z;
    }
    fn_80233678((Vector3*)&slot->mB0, slot, emitter, inst);
    slot->mB0 *= inst->mA8;
    slot->mB4 *= inst->mA8;
    slot->mB8 *= inst->mA8;
    if (param->m80 != 0) {
        slot->mB4 = slot->m14C * slot->mB0;
        slot->mB8 = slot->m150 * slot->mB0;
    }
    if ((inst->mData->mAnimFlags[emitter] & 0x40) == 0x40) {
        slot->mCC = fn_8023329C(0x12, emitter, inst->mData, 0, 0.0f, slot->m12C);
        slot->mCC *= inst->mA8;
    } else {
        slot->mCC = param->m1E4 + slot->m12C;
        slot->mCC *= inst->mA8;
    }
    fn_80233810((Vector3*)&slot->mD0, slot, emitter, inst);
    if (param->mA0 != 0) {
        slot->mD4 = slot->m16C * slot->mD0;
        slot->mD8 = slot->m170 * slot->mD0;
    }
    fn_802339A8(&slot->m98, slot, emitter, inst);
    slot->m98.x *= inst->mA8;
    slot->m98.y *= inst->mA8;
    slot->m98.z *= inst->mA8;
    if (param->m88 != 0) {
        slot->m98.y = slot->m154 * slot->m98.x;
        slot->m98.z = slot->m158 * slot->m98.x;
    }
    fn_80233B40(&slot->mBC, slot, emitter, inst);
    if (param->m90 != 0) {
        slot->mBC.g = slot->m15C * slot->mBC.r;
        slot->mBC.b = slot->m160 * slot->mBC.r;
    }
    if (!(param->m4C & 2)) {
        EffectClamp01_unk(slot->mBC.r);
        EffectClamp01_unk(slot->mBC.g);
        EffectClamp01_unk(slot->mBC.b);
        EffectClamp01_unk(slot->mBC.a);
    }
    EffectAnimData_unk* anim = inst->mData;
    if (EffectAnimFlag_unk(anim, emitter, 0x100)) {
        slot->mDC = fn_8023329C(10, emitter, anim, 0, 0.0f, 0.0f);
        slot->mE0 = fn_8023329C(11, emitter, anim, 0, 0.0f, 0.0f);
    } else {
        slot->mDC = 0.0f;
        slot->mE0 = 0.0f;
    }
    if (param->m4C & 8) {
        slot->m10 = 2;
    } else {
        slot->m10 = 6;
    }
    if (param->m4C & 4) {
        slot->m10 |= 0x10;
    }
    if (!(param->m4C & 0x2000000)) {
        slot->m10 |= 8;
    }
    if (param->m4C & 0x4000000) {
        slot->m10 |= 0x10000;
    } else if (param->m4C & 0x8000000) {
        slot->m10 |= 0x20000;
    }
    if (param->m4C & 0x100) {
        slot->m10 |= 0x80;
    }
    slot->m10 |= 1;
    switch (param->m4C & 0xC0000000) {
    case 0x40000000:
        slot->m10 |= 0x40000;
        break;
    case 0x80000000:
        slot->m10 |= 0x80000;
        break;
    }
    switch (param->m234) {
    case 1:
        slot->m10 |= 0x100;
        break;
    case 2:
        slot->m10 |= 0x400;
        break;
    case 3:
        slot->m10 |= 0x200;
        break;
    case 4:
        slot->m10 |= 0x800;
        break;
    case 5:
        slot->m10 |= 0x1000;
        break;
    }
    if (data->mEntries[param->m40].mType == 2) {
        slot->m10 |= 0x200000;
    }
    slot->m68.x += ofs.x;
    slot->m68.y += ofs.y;
    slot->m68.z += ofs.z;
    slot->m1B0 = 0;
    slot->m1AC = 0;
    slot->m174 = Vector3(0.0f, 0.0f, 0.0f);
    slot->m180 = 0.0f;
    slot->m184 = Vector3(1.0f, 0.0f, 0.0f);
    return 1;
}

// inferred: largest of two values (a macro: the operands are evaluated again)
#define EFFECT_MAX_UNK(a, b) ((a) > (b) ? (a) : (b))
static inline float EffectMax_unk(float a, float b) {
    return a > b ? a : b;
}
#define EFFECT_MAX3_UNK(a, b, c) EFFECT_MAX_UNK(a, EffectMax_unk(b, c))

// NONMATCHING: 85% - the maximum of the three scales is evaluated again by the original only in its
// inner part (macro over an inline); the model pointer is loaded first; stack slots of the vectors
// inferred: grows the bounding radius of an instance (mC8) with a slot: a model (its scale and the
// shape of the emitter), the fade items, or the emission point
extern "C" void fn_80230F3C(EffectInst_unk* inst, EffectSlot_unk* slot) {
    EffectFileEntry_unk* entry = slot->m1C;
    EffectParam_unk* param = inst->mParam;
    float r;
    if (entry->mType == 1) {
        float size = EFFECT_MAX3_UNK(atn::Abs_unk(slot->mB0), atn::Abs_unk(slot->mB4), atn::Abs_unk(slot->mB8)) *
                     entry->m134;
        EffectInst_unk* model = slot->m08;
        float ext = size * model->mA8;
        ext = size * PSVECMag(&model->mPos) + ext;
        switch (inst->m08) {
        case 3:
            if ((inst->m10 & 1) != 1) {
                float* m = inst->m0C;
                Vector3 c3(m[4], m[8], m[12]);
                float a = PSVECMag(&c3);
                Vector3 c2(m[3], m[7], m[11]);
                float b = PSVECMag(&c2);
                Vector3 c1(m[2], m[6], m[10]);
                Vector3 k(PSVECMag(&c1), b, a);
                ext *= EFFECT_MAX_UNK(1.0f, EFFECT_MAX3_UNK(atn::Abs_unk(k.x), atn::Abs_unk(k.y), atn::Abs_unk(k.z)));
            }
            break;
        case 5:
            if ((inst->m10 & 1) != 1) {
                float* m = inst->m0C;
                Vector3 c3(m[2], m[6], m[10]);
                float a = PSVECMag(&c3);
                Vector3 c2(m[1], m[5], m[9]);
                float b = PSVECMag(&c2);
                Vector3 c1(m[0], m[4], m[8]);
                Vector3 k(PSVECMag(&c1), b, a);
                ext *= EFFECT_MAX_UNK(1.0f, EFFECT_MAX3_UNK(atn::Abs_unk(k.x), atn::Abs_unk(k.y), atn::Abs_unk(k.z)));
            }
            break;
        }
        Vector3 d(slot->m80.x - inst->mPos.x, slot->m80.y - inst->mPos.y, slot->m80.z - inst->mPos.z);
        r = PSVECMag(&d) + atn::Abs_unk(ext);
    } else if (param->m4C & 0x20000) {
        r = 0.0f;
        for (unsigned int i = 0; i < slot->m1AC; i++) {
            EffectFade_unk* fade = &slot->m1A8[(i + slot->m1B0) % slot->m1B4];
            float s = fade->m10 * slot->m1C->m134;
            Vector3 d(fade->m00 - inst->mPos.x, fade->m04 - inst->mPos.y, fade->m08 - inst->mPos.z);
            float v = PSVECMag(&d) + atn::Abs_unk(s);
            if (v > r) {
                r = v;
            }
        }
    } else {
        float s = EFFECT_MAX3_UNK(atn::Abs_unk(slot->mB0), atn::Abs_unk(slot->mB4), atn::Abs_unk(slot->mB8));
        Vector3 d(slot->m80.x - inst->mPos.x, slot->m80.y - inst->mPos.y, slot->m80.z - inst->mPos.z);
        float size = s * entry->m134;
        r = PSVECMag(&d) + atn::Abs_unk(size);
    }
    float m = EFFECT_MAX_UNK(0.01f, r);
    if (m > inst->mC8) {
        inst->mC8 = m;
    }
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

// inferred: color parameter of an emitter (curves 3-6 when animated, only 6 in mode 1), with its
// ratios to red and the per-slot offset or scale
extern "C" void fn_80233B40(EffectColor_unk* out, EffectSlot_unk* slot, int emitter, EffectInst_unk* inst) {
    EffectAnimData_unk* data = inst->mData;
    EffectParam_unk* param = inst->mParam;
    unsigned int flags = data->mAnimFlags[emitter];
    int mode = ((flags >> 4) & 1) + ((flags >> 5) & 1);
    switch (mode) {
    case 0:
        out->r = param->m194.r;
        out->g = param->m194.g;
        out->b = param->m194.b;
        out->a = param->m194.a;
        break;
    case 1:
        out->a = fn_8023329C(6, emitter, data, 0, 0.0f, 0.0f);
        out->r = param->m194.r;
        out->g = param->m194.g;
        out->b = param->m194.b;
        break;
    case 2:
        out->r = fn_8023329C(3, emitter, data, 0, 0.0f, 0.0f);
        out->g = fn_8023329C(4, emitter, data, 0, 0.0f, 0.0f);
        out->b = fn_8023329C(5, emitter, data, 0, 0.0f, 0.0f);
        out->a = fn_8023329C(6, emitter, data, 0, 0.0f, 0.0f);
        break;
    }
    if (param->m90 == 2) {
        float r = out->r;
        float zero = 0.0f;
        if (r != zero) {
            slot->m15C = out->g / r;
            slot->m160 = out->b / out->r;
        } else {
            slot->m15C = 0.0f;
            slot->m160 = 0.0f;
        }
    } else {
        slot->m15C = 1.0f;
        slot->m160 = 1.0f;
    }
    if (param->m8C & 1) {
        out->r *= slot->mF0.r;
        out->g *= slot->mF0.g;
        out->b *= slot->mF0.b;
        out->a *= slot->mF0.a;
    } else {
        out->r += slot->mF0.r;
        out->g += slot->mF0.g;
        out->b += slot->mF0.b;
        out->a += slot->mF0.a;
    }
}

// inferred: random value in [-range, range] in steps of range / 500 (0 when the range is 0; name invented)
static inline float EffectRand_unk(float range) {
    float v = 0.0f;
    if (v != range) {
        float lo = -range;
        v = (range - lo) * ((float)(rand() % 1001) / 1000.0f) + lo;
    }
    return v;
}

// NONMATCHING: 86.5% - registers of the range and its negation swapped (f29/f30), the 0.0 of the
// compare is loaded first; the original keeps a fourth saved float (f28) for the 0.0 base of m12C
// inferred: random start values of a slot: each one is 1 or 0 (flag bit 0 of the parameters) plus a
// random value in its range
extern "C" void fn_80233D74(EffectSlot_unk* slot, EffectParam_unk* param) {
    float base = (param->m84 & 1) ? 1.0f : 0.0f;
    slot->m100.x = base + EffectRand_unk(param->m188);
    slot->m100.y = base + EffectRand_unk(param->m18C);
    slot->m100.z = base + EffectRand_unk(param->m190);
    base = (param->m7C & 1) ? 1.0f : 0.0f;
    slot->mE4.x = base + EffectRand_unk(param->m14C);
    slot->mE4.y = base + EffectRand_unk(param->m150);
    slot->mE4.z = base + EffectRand_unk(param->m154);
    base = (param->m8C & 1) ? 1.0f : 0.0f;
    slot->mF0.a = base + EffectRand_unk(param->m1E0);
    slot->mF0.r = base + EffectRand_unk(param->m1D4);
    slot->mF0.g = base + EffectRand_unk(param->m1D8);
    slot->mF0.b = base + EffectRand_unk(param->m1DC);
    base = (param->m6C & 1) ? 1.0f : 0.0f;
    slot->m114.x = base + EffectRand_unk(param->mD4);
    slot->m114.y = base + EffectRand_unk(param->mD8);
    slot->m114.z = base + EffectRand_unk(param->mDC);
    base = (param->m74 & 1) ? 1.0f : 0.0f;
    slot->m120.x = base + EffectRand_unk(param->m110);
    slot->m120.y = base + EffectRand_unk(param->m114);
    slot->m120.z = base + EffectRand_unk(param->m118);
    slot->m12C = 0.0f + EffectRand_unk(param->m1F4);
    base = (param->m9C & 1) ? 1.0f : 0.0f;
    slot->m130.x = base + EffectRand_unk(param->m228);
    slot->m130.y = base + EffectRand_unk(param->m22C);
    slot->m130.z = base + EffectRand_unk(param->m230);
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
