#include "math/Vec.h"
#include "atn/EffectSystem_unk.h"

// Effect resources and their particles: intrusive lists of nodes, key-frame curves of the
// particle definitions and the update of the particles. Name of the file invented.

// inferred: node of the intrusive lists of the effect system (pools of particles and emitters)
struct EffectNode_unk {
    EffectNode_unk* mPrev;
    EffectNode_unk* mNext;
};

// inferred: links `count` nodes of `stride` bytes of `buffer` into the free list; the used list
// starts empty
extern "C" void fn_8023793C(EffectNode_unk** used, EffectNode_unk** free, char* buffer, int stride, int count) {
    *used = 0;
    *free = (EffectNode_unk*)buffer;
    ((EffectNode_unk*)buffer)->mPrev = 0;
    ((EffectNode_unk*)buffer)->mNext = (EffectNode_unk*)(buffer + stride);
    int i;
    for (i = 1; i < count - 1; i++) {
        EffectNode_unk* node = (EffectNode_unk*)(buffer + i * stride);
        node->mPrev = (EffectNode_unk*)(buffer + (i * stride - stride));
        node->mNext = (EffectNode_unk*)(buffer + (i * stride + stride));
    }
    EffectNode_unk* last = (EffectNode_unk*)(buffer + i * stride);
    last->mPrev = (EffectNode_unk*)(buffer + (i - 1) * stride);
    last->mNext = 0;
}

// inferred: adds `node` at the front (or at the end) of the list
extern "C" EffectNode_unk* fn_80237B20(EffectNode_unk** list, EffectNode_unk* node, int atEnd) {
    EffectNode_unk* head = *list;
    if (head == 0) {
        node->mPrev = 0;
        node->mNext = 0;
        *list = node;
    } else if (!atEnd) {
        *list = node;
        node->mPrev = 0;
        node->mNext = head;
        head->mPrev = node;
    } else {
        while (head->mNext != 0) {
            head = head->mNext;
        }
        node->mNext = 0;
        node->mPrev = head;
        head->mNext = node;
    }
    return node;
}

// inferred: unlinks the first node of a list (inline, name invented)
inline EffectNode_unk* fn_PopFront_unk(EffectNode_unk** list) {
    EffectNode_unk* node = *list;
    if (node != 0) {
        if (node->mPrev != 0) {
            node->mPrev->mNext = node->mNext;
        }
        if (node->mNext != 0) {
            node->mNext->mPrev = node->mPrev;
        }
        *list = node->mNext;
        if (*list != 0) {
            (*list)->mPrev = 0;
        }
        node->mPrev = 0;
        node->mNext = 0;
    }
    return node;
}

// inferred: moves the first free node to the end of the used list; without free nodes and with
// `recycle`, the oldest used node is moved to the end instead
extern "C" EffectNode_unk* fn_80237B8C(EffectNode_unk** used, EffectNode_unk** free, int recycle) {
    EffectNode_unk* node = *free;
    if (node != 0) {
        if (node->mPrev != 0) {
            node->mPrev->mNext = node->mNext;
        }
        if (node->mNext != 0) {
            node->mNext->mPrev = node->mPrev;
        }
        *free = node->mNext;
        if (*free != 0) {
            (*free)->mPrev = 0;
        }
        node->mPrev = 0;
        node->mNext = 0;
        fn_80237B20(used, node, 1);
    } else if (recycle != 0) {
        node = *used;
        if (node != 0) {
            if (node->mPrev != 0) {
                node->mPrev->mNext = node->mNext;
            }
            if (node->mNext != 0) {
                node->mNext->mPrev = node->mPrev;
            }
            *used = node->mNext;
            if (*used != 0) {
                (*used)->mPrev = 0;
            }
            node->mPrev = 0;
            node->mNext = 0;
            fn_80237B20(used, node, 1);
        }
    }
    return node;
}

// inferred: moves `node` from the list `from` to the front of `to`
// NONMATCHING: 98.46% - the head of `from` and node->mPrev get swapped registers
extern "C" EffectNode_unk* fn_80237C88(EffectNode_unk** from, EffectNode_unk** to, EffectNode_unk* node) {
    EffectNode_unk* head = *from;
    if (node->mPrev != 0) {
        node->mPrev->mNext = node->mNext;
    }
    if (node->mNext != 0) {
        node->mNext->mPrev = node->mPrev;
    }
    if (head == node) {
        *from = node->mNext;
        if (*from != 0) {
            (*from)->mPrev = 0;
        }
    }
    node->mPrev = 0;
    node->mNext = 0;
    return fn_80237B20(to, node, 0);
}

// inferred: key of a curve
struct EffectKey_unk {
    float mTime;
    float mValue;
};
// inferred: curve of a particle channel
struct EffectCurve_unk {
    unsigned int mCount;
    EffectKey_unk* mKeys;
};
// inferred: curves of a particle definition (0x68 bytes): position x/y/z, alpha, ?, color r/g/b/a...
struct EffectCurves_unk {
    EffectCurve_unk mChannels[13];
};

// inferred: color of a particle
// inferred: particle definition of a resource (0x110 bytes, fields named by use)
struct EffectDef_unk {
    unsigned char m00[0x24];
    unsigned int mFlags;       // 0x24: 0x80000 negates the color
    unsigned char m28[0x2C - 0x28];
    int mStart;                // 0x2C
    float mPos0[3];            // 0x30
    float mAlpha0;             // 0x3C
    unsigned char m40[4];
    float mColor0[4];          // 0x44
    unsigned char m54[0x9C - 0x54];
    int mEnd;                  // 0x9C
    float mPos1[3];            // 0xA0
    float mAlpha1;             // 0xAC
    unsigned char mB0[4];
    float mColor1[4];          // 0xB4
    unsigned char mC4[0x110 - 0xC4];
};

// inferred: emitter of a resource (0x26C bytes)
struct EffectEmitter_unk {
    unsigned char m00[0x48];
    int m48; // inferred: start frame
    unsigned char m4C[4];
    int m50; // inferred: delay
    int m54; // inferred: life
    unsigned char m58[0x26C - 0x58];
};

// inferred: effect resource (efp data)
struct EffectRes_unk {
    unsigned int mCount;           // 0x00
    EffectEmitter_unk* mEmitters;  // 0x04
    unsigned char m08[0x14 - 0x08];
    EffectDef_unk* mDefs;          // 0x14
    unsigned char m18[0x20 - 0x18];
    unsigned int* mDefFlags;       // 0x20: 1 position, 2 alpha, 4 color from curves
    EffectCurves_unk* mCurves;     // 0x24
    int* mEnabled;                 // 0x28
};

// inferred: value of a curve at `t`; `scale` multiplies by (1 + add), else adds `add`
// NONMATCHING: 98.26% - registers of the curve address computation
extern "C" float fn_80237CF0(int channel, int def, EffectRes_unk* res, int scale, float t, float add) {
    EffectCurve_unk* curve = &res->mCurves[def].mChannels[channel];
    EffectKey_unk* keys = curve->mKeys;
    EffectKey_unk* key = keys;
    float value = 0.0f;
    for (unsigned int i = 0; i < curve->mCount - 1; i++) {
        EffectKey_unk* next = &keys[i + 1];
        float u = (t - key->mTime) / (next->mTime - key->mTime);
        if (u >= 0.0f && u <= 1.0f) {
            value = key->mValue + u * (next->mValue - key->mValue);
            break;
        }
        key++;
    }
    if (scale != 0) {
        value *= 1.0f + add;
    } else {
        value += add;
    }
    return value;
}

// inferred: position, alpha and color of a particle definition at `time`; 0 outside its frames
// NONMATCHING: 99.94% - the alpha lerp loads its two keys in the other order
extern "C" int fn_80237DA8(EffectRes_unk* res, EffectDef_unk* def, atn::Vector3* pos, float* alpha,
                           EffectColor_unk* color, float time) {
    float start = def->mStart;
    float end = def->mEnd;
    float t;
    if (def->mEnd == 0) {
        t = 0.0f;
    } else {
        if (start >= end) {
            return 0;
        }
        t = (time - start) / (end - start);
        if (t <= 0.0f || t > 1.0f) {
            return 0;
        }
    }
    int index = def - res->mDefs;
    if ((res->mDefFlags[index] & 1) != 1) {
        pos->x = def->mPos0[0] + t * (def->mPos1[0] - def->mPos0[0]);
        pos->y = def->mPos0[1] + t * (def->mPos1[1] - def->mPos0[1]);
        pos->z = def->mPos0[2] + t * (def->mPos1[2] - def->mPos0[2]);
    } else {
        pos->x = fn_80237CF0(0, index, res, 0, t, 0.0f);
        pos->y = fn_80237CF0(1, index, res, 0, t, 0.0f);
        pos->z = fn_80237CF0(2, index, res, 0, t, 0.0f);
    }
    if ((res->mDefFlags[index] & 2) != 2) {
        *alpha = def->mAlpha0 + t * (def->mAlpha1 - def->mAlpha0);
    } else {
        *alpha = fn_80237CF0(3, index, res, 0, t, 0.0f);
    }
    if ((res->mDefFlags[index] & 4) != 4) {
        color->a = def->mColor0[3] + t * (def->mColor1[3] - def->mColor0[3]);
        color->r = def->mColor0[0] + t * (def->mColor1[0] - def->mColor0[0]);
        color->g = def->mColor0[1] + t * (def->mColor1[1] - def->mColor0[1]);
        color->b = def->mColor0[2] + t * (def->mColor1[2] - def->mColor0[2]);
    } else {
        color->r = fn_80237CF0(5, index, res, 0, t, 0.0f);
        color->g = fn_80237CF0(6, index, res, 0, t, 0.0f);
        color->b = fn_80237CF0(7, index, res, 0, t, 0.0f);
        color->a = fn_80237CF0(8, index, res, 0, t, 0.0f);
    }
    if (def->mFlags & 0x80000) {
        color->r = -color->r;
        color->g = -color->g;
        color->b = -color->b;
        color->a = -color->a;
    }
    return 1;
}

// inferred: emitter instance that starts a particle (fields named by use)
struct EffectInstDef_unk {
    unsigned char m00[0x08];
    int m08;
    int m0C;
    int m10;
    int m14;
    unsigned char m18[0x30 - 0x18];
    int m30;
    unsigned char m34[0x3C - 0x34];
    atn::Matrix3x4 mMtx;   // 0x3C
    unsigned char m6C[0xA8 - 0x6C];
    float mA8;
    unsigned char mAC[0xBC - 0xAC];
    EffectRes_unk* mRes;   // 0xBC
    unsigned char mC0[0xD0 - 0xC0];
    atn::Vector3 mD0;
};

// inferred: owner of the particles
struct EffectOwner_unk {
    unsigned char m00[0x6C];
    int m6C;
};

// inferred: effect system (atn/EffectSystem_unk.cpp)
struct EffectSystemData_unk {
    unsigned char m00[0x20];
    int mCount;                       // 0x20
    EffectParticle_unk* mParticles;   // 0x24
};

extern "C" void fn_802380D4(EffectSystemData_unk* system, float dt) {
    for (int i = 0; i < system->mCount; i++) {
        system->mParticles[i].fn_80238218(dt);
    }
}

EffectParticle_unk::EffectParticle_unk()
    : m00(0), mRes(0), m08(0), mDef(0), mTime(0.0f), mFading(0), mFadeMode(0), mFadeSpeed(0.0f), mFade(0.0f),
      m28(0), m2C(0), m30(0), mFlags(0), m38(0), m3C(0), m40(0), m44(-1), mOwner(0), m8C(0) {
    mMtx.SetIdentity();
}

EffectParticle_unk::~EffectParticle_unk() {}

// NONMATCHING: 94.14% - scheduling of the loads of the definition and branch layout of the early
// returns
void EffectParticle_unk::fn_80238218(float dt) {
    if (m00 == 0) {
        return;
    }
    if ((mFlags & 2) == 2) {
        return;
    }
    if (m8C != 0) {
        return;
    }
    mTime += dt;
    if ((mFlags & 4) == 4) {
        return;
    }
    EffectRes_unk* res = mRes;
    EffectDef_unk* def = &res->mDefs[mDef];
    if (mTime > def->mEnd && def->mEnd != 0) {
        fn_80238560();
        return;
    }
    atn::Vector3 pos;
    float alpha;
    EffectColor_unk color;
    if (fn_80237DA8(res, def, &pos, &alpha, &color, mTime)) {
        fn_80238588(&pos, &alpha, &color);
        if (mFading != 0) {
            if (mFade >= 1.0f) {
                fn_80238560();
                return;
            }
            switch (mFadeMode) {
            case 0:
                alpha *= 1.0f - mFade;
                break;
            case 1:
                color.r *= 1.0f - mFade;
                color.g *= 1.0f - mFade;
                color.b *= 1.0f - mFade;
                break;
            }
            mFade += mFadeSpeed;
        }
    }
}

// NONMATCHING: 57.07% - the original keeps the unreachable body with five saved registers and
// another schedule (only the IsActive_unk inline keeps the body at all)
int EffectParticle_unk::fn_802383D8(void* owner, EffectInstDef_unk* def, int index) {
    // inferred: the second test repeats the first one (the body is unreachable but kept)
    if (IsActive_unk()) {
        return 0;
    }
    if (IsActive_unk()) {
        mRes = def->mRes;
        m08 = def->m30;
        mMtx = def->mMtx;
        m90 = ((EffectOwner_unk*)owner)->m6C;
        m38 = def->m08;
        m3C = def->m0C;
        m40 = def->m10;
        m44 = def->m14;
        m78 = def->mD0;
        m84 = def->mA8;
        mDef = index;
        mTime = 0.0f;
        mFading = 0;
        mFlags = 0;
        m8C = 0;
        EffectRes_unk* res = def->mRes;
        atn::Vector3 pos;
        float alpha;
        EffectColor_unk color;
        if (!fn_80237DA8(res, &res->mDefs[index], &pos, &alpha, &color, 1.0f + res->mDefs[index].mStart)) {
            pos.z = pos.y = pos.x = alpha = color.b = color.g = color.r = color.a = 0.0f;
        }
        fn_80238588(&pos, &alpha, &color);
        mOwner = owner;
        return 1;
    }
    return 0;
}

void EffectParticle_unk::fn_80238560() {
    m00 = 0;
    mRes = 0;
    m08 = 0;
    mDef = 0;
    mTime = 0.0f;
    mFading = 0;
    mFlags = 0;
}

extern "C" void fn_8022C9C8(int* ctrl, atn::Matrix3x4* base, atn::Matrix3x4* out, atn::Vector3* trans, int arg);

// NONMATCHING: 99.73% - stack layout: the original keeps a zeroed Vector3 at 0x48 next to the
// matrix (here its stores are scattered, frame 0x80 instead of 0x60)
void EffectParticle_unk::fn_80238588(atn::Vector3* pos, float* alpha, EffectColor_unk* color) {
    atn::Vector3 zero;
    atn::Matrix3x4 mtx;
    atn::Vector3 trans;
    fn_8022C9C8(&m38, &mMtx, &mtx, &trans, 0);
    zero.x = zero.y = zero.z = 0.0f;
    PSMTXMultVec(mtx.m, pos, pos);
    *pos += trans;
    *alpha *= m84;
    color->a /= 255.0f;
    color->r /= 255.0f;
    color->g /= 255.0f;
    color->b /= 255.0f;
    if (mTime >= m44 && m44 != 0) {
        m38 = 0;
        mMtx = mtx;
        m44 = 0;
    }
}

// inferred: frames the effect lasts when played for `frames` frames (0 = endless)
// NONMATCHING: 98.51% - registers of the emitter pointer and the length (r10/r11 swapped)
extern "C" unsigned int fn_802386FC(EffectRes_unk* res, int frames) {
    unsigned int length = 0;
    for (unsigned int i = 0; i < res->mCount; i++) {
        if (res->mEnabled[i] != 0) {
            EffectEmitter_unk* em = &res->mEmitters[i];
            if (em->m54 <= 0 || em->m50 == 0) {
                if (frames != 0 && em->m50 != 0) {
                    unsigned int end = frames + em->m50 + em->m48 - 1;
                    length = length > end ? length : end;
                } else {
                    length = 0;
                    break;
                }
            } else if (frames != 0) {
                // inferred: MAX/MIN macros (the arguments are evaluated twice)
                length = length > em->m48 + em->m50 + (frames < em->m54 ? frames : em->m54) - 1
                             ? length
                             : em->m48 + em->m50 + (frames < em->m54 ? frames : em->m54) - 1;
            } else {
                unsigned int end = em->m54 + em->m50 + em->m48 - 1;
                length = length > end ? length : end;
            }
        }
    }
    return length;
}
