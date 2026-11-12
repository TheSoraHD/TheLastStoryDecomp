#ifndef ATN_EFFECTSYSTEM_UNK_H
#define ATN_EFFECTSYSTEM_UNK_H

#include "atn/Array_unk.h"
#include "math/Vec.h"
#include "std/vector.h"

// Effect system of the efp effects (code in atn/EffectSystem_unk.cpp, 0x8022B850-0x80234A8C; the
// particles in atn/EffectRes_unk.cpp; kept by atn::EffectManager, atn/EffectManager.cpp).

// inferred: RGBA color
struct EffectColor_unk {
    EffectColor_unk() {}
    EffectColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

struct EffectRes_unk;
namespace atn {
class EffectManagerCallback;
}

// inferred: particle of the effect system (0x94 bytes, names invented)
struct EffectParticle_unk {
    EffectParticle_unk();
    ~EffectParticle_unk();
    void fn_80238218(float dt);
    int fn_802383D8(void* owner, struct EffectInstDef_unk* def, int index);
    void fn_80238560();
    bool IsActive_unk() const { return m00 != 0; }
    // inferred: started for this owner/group (-2: any group); names invented
    bool IsMatch_unk(unsigned int owner, int group) const {
        return m28 == owner && (m2C == group || group == -2);
    }
    void fn_80238588(atn::Vector3* pos, float* alpha, EffectColor_unk* color);

    int m00;                // 0x00: inferred: active
    EffectRes_unk* mRes;    // 0x04
    int m08;
    int mDef;               // 0x0C
    float mTime;            // 0x10
    unsigned char mFading;  // 0x14
    int mFadeMode;          // 0x18: 0 alpha, 1 color
    float mFadeSpeed;       // 0x1C
    float mFade;            // 0x20
    int m24;
    int m28;
    int m2C;
    unsigned int m30;       // 0x30: inferred: handle of the emitter
    unsigned int mFlags;    // 0x34: 2 paused, 4 frozen
    int m38;
    int m3C;
    int m40;
    int m44;                // 0x44: inferred: frame where m38 is dropped
    atn::Matrix3x4 mMtx;    // 0x48
    atn::Vector3 m78;
    float m84;              // 0x84: alpha scale
    void* mOwner;           // 0x88
    unsigned char m8C;
    int m90;
};

// inferred: key of an animation curve of the efp data (time, value)
struct EffectKey_unk {
    float mTime;
    float mValue;
};
struct EffectCurve_unk {
    unsigned int mCount;
    EffectKey_unk* mKeys;
};
// inferred: the 22 curves of an emitter (0xB0 bytes; the particles have their own set,
// EffectCurves_unk of atn/EffectRes_unk.cpp)
struct EffectEmitterCurves_unk {
    EffectCurve_unk mCurves[22];
};
// inferred: animation data of an effect (fields named by use)
struct EffectParam_unk;
struct EffectAnimData_unk {
    int m00;
    EffectParam_unk* mParams;    // 0x04: inferred: one per emitter (0x26C bytes each)
    int m08;
    struct EffectFileEntry_unk* mEntries; // 0x0C: inferred: entries of the effect data (0x138 bytes each)
    unsigned int mParticleDefCount; // 0x10: inferred: particle definitions
    int m14;
    unsigned int* mAnimFlags;    // 0x18: per emitter, bit 0: animated
    EffectEmitterCurves_unk* mEmitters; // 0x1C
    int m20;
    int m24;
    int* mEmitterOn;             // 0x28: inferred: per emitter, started with the effect
    int* mParticleOn;            // 0x2C: inferred: per particle definition, started with the effect
};
// inferred: parameters of an effect (fields named by offset)
struct EffectParam_unk {
    unsigned char m00[0x40];
    int m40;           // 0x40: inferred: index of the entry of the effect data (EffectAnimData_unk::mEntries)
    int m44;
    unsigned int m48;  // 0x48: inferred: first frame of the emission
    unsigned int m4C;  // 0x4C: inferred: bits 28-29 render pass, bits 30-31 mode (fn_8022D328)
    int m50;
    int m54;           // 0x54: inferred: frames of the emission (> 0: it stops after them)
    int m58;           // 0x58: inferred: emission period in frames (at least 1)
    int m5C;           // 0x5C: inferred: slots started per emission (at least 1)
    unsigned char m60[0x64 - 0x60];
    float m64;         // 0x64: inferred: scale of the movement (EffectInst_unk::mA8)
    float m68;
    unsigned int m6C;
    unsigned int m70;
    unsigned int m74;
    unsigned int m78;
    unsigned int m7C;
    unsigned int m80;
    unsigned int m84;
    unsigned int m88;
    unsigned int m8C;       // 0x8C: inferred: bit 0: the color of the slot scales (else adds)
    unsigned int m90;       // 0x90: inferred: 2: color ratios to red (fn_80233B40)
    unsigned char m94[0x9C - 0x94];
    unsigned int m9C;
    unsigned int mA0;
    atn::Vector3 mA4;
    unsigned char mB0[0xBC - 0xB0];
    atn::Vector3 mBC;       // 0xBC: inferred: velocity of the instance (not animated)
    float mC8;              // 0xC8: inferred: damping of the velocity per axis
    float mCC;
    float mD0;
    unsigned char mD4[0xE0 - 0xD4];
    atn::Vector3 mE0;
    unsigned char mEC[0x11C - 0xEC];
    atn::Vector3 m11C;
    unsigned char m128[0x158 - 0x128];
    atn::Vector3 m158;
    unsigned char m164[0x194 - 0x164];
    EffectColor_unk m194;   // 0x194: inferred: color when not animated
    unsigned char m1A4[0x1F8 - 0x1A4];
    atn::Vector3 m1F8;
    unsigned char m204[0x238 - 0x204];
    int m238;               // 0x238: inferred: frames the slots move (< 0: always)
    unsigned char m23C[0x25C - 0x23C];
    unsigned int m25C;      // 0x25C: inferred: bit 0: emission reduced with the camera distance
    float m260;             // 0x260: inferred: distance where the reduction starts
    float m264;             // 0x264: inferred: distance range of the reduction
    float m268;             // 0x268: inferred: emission scale at full reduction
};

// inferred: effect instance of the system (one per started effect; linked list from
// EffectSystem_unk::mInsts; fields named by use, the size is not known yet)
struct EffectInst_unk {
    EffectInst_unk(); // 0x8022BDB4
    int m00;
    EffectInst_unk* mNext;  // 0x04
    unsigned int m08;       // 0x08: inferred: shape of the emitter (3, 5: from the matrix m0C)
    float* m0C;
    unsigned int m10;
    int m14;
    unsigned short mFlags;  // 0x18
    unsigned char m1A;
    unsigned char mKill;    // 0x1B: inferred: stop request
    float m1C;
    void* mOwnerPtr;        // 0x20
    unsigned int mOwner;    // 0x24
    int mGroup;             // 0x28
    unsigned int mHandle;   // 0x2C
    void* mEffect;          // 0x30
    int mTag;               // 0x34
    int mLayer;             // 0x38: inferred: layer drawn (fn_8022D328)
    atn::Matrix3x4 mMtx;    // 0x3C: inferred: matrix of the instance (set from the work buffer by fn_8022D078)
    atn::Matrix3x4 m6C;     // 0x6C: inferred: base matrix (its translation is not used)
    atn::Vector3 mPos;      // 0x9C: inferred: center of the bounding sphere (radius mCC)
    float mA8;              // 0xA8: inferred: scale of the movement
    EffectColor_unk mColor; // 0xAC
    EffectAnimData_unk* mData;  // 0xBC
    EffectParam_unk* mParam;    // 0xC0
    struct EffectSlot_unk* mSlots; // 0xC4: inferred: list of the slots (EffectSlot_unk::m04)
    float mC8;
    float mCC;
    atn::Vector3 mVec;      // 0xD0
    float mRate;            // 0xDC
};

// inferred: four values and a scale (ctor 0x8022BE8C; names invented)
struct EffectFade_unk {
    EffectFade_unk();
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
};

// inferred: element of the system with a constructor (0x1B8 bytes; fields named by offset)
struct EffectSlot_unk {
    EffectSlot_unk(); // 0x8022BE1C
    int m00;
    EffectSlot_unk* mNext; // 0x04: inferred: next slot of the instance
    struct EffectInst_unk* m08; // 0x08: inferred: instance of the model drawn (position m9C, scale mA8)
    int m0C;
    int m10;
    float m14;
    float m18;
    struct EffectFileEntry_unk* m1C; // 0x1C: inferred: entry of the effect data (atn/EffectFileEntry_unk.h)
    int m20;
    int m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    atn::Matrix3x4 mMtx;   // 0x38: inferred: matrix of the slot (fn_80231754)
    atn::Vector3 m68;      // 0x68: inferred: offset, turned by mMtx and added to m74
    atn::Vector3 m74;      // 0x74: inferred: position
    atn::Vector3 m80;      // 0x80: inferred: emission point (m74 + m8C, moved by fn_80231508)
    atn::Vector3 m8C;
    unsigned char m98[0xA4 - 0x98];
    float mA4;
    unsigned char mA8[0xB0 - 0xA8];
    float mB0;             // 0xB0: inferred: scale x, y, z of the slot
    float mB4;
    float mB8;
    unsigned char mBC[0xCC - 0xBC];
    float mCC;             // 0xCC: inferred: distance along the direction (fn_80231508)
    float mD0;             // 0xD0: inferred: rotations x, y, z of the direction
    float mD4;
    float mD8;
    unsigned char mDC[0xE4 - 0xDC];
    atn::Vector3 mE4;
    EffectColor_unk mF0;   // 0xF0: inferred: color offset or scale of the slot
    atn::Vector3 m100;
    unsigned char m10C[0x114 - 0x10C];
    atn::Vector3 m114;
    atn::Vector3 m120;
    unsigned char m12C[0x130 - 0x12C];
    atn::Vector3 m130;
    float m13C;
    float m140;
    float m144;
    float m148;
    float m14C;
    float m150;
    float m154;
    float m158;
    float m15C;            // 0x15C: inferred: green / red ratio (fn_80233B40)
    float m160;            // 0x160: inferred: blue / red ratio
    unsigned char m164[0x16C - 0x164];
    float m16C;
    float m170;
    unsigned char m174[0x190 - 0x174];
    unsigned int m190; // 0x190: inferred: flags (2: drawn)
    atn::Vector3 m194;     // 0x194: inferred: emission point when the slot starts (fn_8022DA70)
    float m1A0;
    float m1A4;
    EffectFade_unk* m1A8; // 0x1A8: inferred: from EffectSystem_unk::m2C
    unsigned int m1AC; // 0x1AC: inferred: used items of the fade ring (m1A8)
    unsigned int m1B0; // 0x1B0: inferred: first item of the ring
    unsigned int m1B4; // 0x1B4: inferred: size of the ring
};


// inferred: the effect system kept by the manager (0x88 bytes; its members are in other files)
// inferred: two empty levels over std::vector (five null checks of `this` in the destructor); used
// as a bitmap of the used blocks of EffectFadePool_unk
// (pointer elements: their resize clears the new words with memset as in fn_8022BEAC; the words are
// read as unsigned int)
struct EffectPtrVectorBase_unk : public std::vector<void*> {};
struct EffectPtrVector_unk : public EffectPtrVectorBase_unk {};
// inferred: block allocator of the EffectFade_unk of the slots (0x40 per block, 0x120 blocks; set up
// by EffectSystem_unk::fn_8022B850, names invented)
struct EffectSystemPool_unk {
    EffectSystemPool_unk() : mBlockSize(0), mBlockCount(0) {}
    unsigned int mBlockSize;  // 0x00
    unsigned int mBlockCount; // 0x04
    atn::SimpleArray<EffectFade_unk> mItems; // 0x08
    EffectPtrVector_unk mUsed; // 0x10
};

class EffectSystem_unk {
public:
    EffectSystem_unk()
        : mInstMax(0), mInstPool(0), m08(0), mInsts(0), mSlotCount(0), mSlots(0), mFreeSlots(0), m1C(0), mParticleCount(0), mParticles(0), m28(0), m48(0.0f),
          m4C(0), m50(0), m54(0), m58(0), m5C(0), m60(0), mFlags(0), m68(0), mLayer(0), m70(0), mUpdating(0),
          m78(0), m7C(0), mListener(0), mReplaying(0) {}
    void fn_8022B850(int a, int b, int c); // inferred: init
    void fn_8022BEAC(unsigned int max);
    void fn_8022C2A0();                    // inferred: release
    void fn_8022D078(float rate);          // inferred: update of the instances
    void fn_802301EC(EffectInst_unk* inst, EffectParam_unk* param, float* mtx); // inferred: emits the particles of an instance
    void fn_8022D478(int layer);           // inferred: draws a layer
    void fn_8022C3F0();                    // inferred: update
    void* fn_8022FAC4(void* res, int a, int b, int c, const atn::Vector3* pos, const atn::Vector3* rot,
                      const EffectColor_unk* color, int d, int e, float scale);
    void fn_802328EC(EffectInst_unk* inst, int count); // inferred: moves an instance forward count frames
    void fn_80232ACC(void* owner);
    int fn_80232B98(unsigned int owner, int group);
    int fn_80232BEC(unsigned int owner, int group, int count);
    int fn_80232D04(unsigned int owner, int group, int flag);
    void fn_80232E48();
    void fn_80232ED4(int layer); // inferred: the argument is read (r4)
    void fn_80232F84(int arg);
    int fn_80232FF4(unsigned int owner, int group, unsigned int flags);
    int fn_802330A4(unsigned int owner, int group, unsigned int flags);
    void fn_80233158(unsigned int owner, int group, const EffectColor_unk* color);
    void fn_802331B0(unsigned int owner, int group, const atn::Vector3* vec);
    void fn_80233260(unsigned int owner, int group, float rate);
    int fn_802346BC(int handle);
    void fn_802346F0(int handle, unsigned int flags);
    void fn_80234764(int handle, const atn::Vector3* vec);
    void fn_802347DC(int handle, int arg);
    void fn_802348E4(void* effect, unsigned int flags);
    void fn_80234958(void* effect, int flag);
    void fn_8023686C(float rate);
    void fn_802368B0(void* arg);

    int mInstMax;               // 0x00
    EffectInst_unk* mInstPool;  // 0x04: inferred: new[] of mInstMax instances
    EffectInst_unk* m08;        // 0x08: inferred: free list
    EffectInst_unk* mInsts;     // 0x0C
    int mSlotCount;             // 0x10
    EffectSlot_unk* mSlots;     // 0x14
    EffectSlot_unk* mFreeSlots; // 0x18: inferred: free list of the slots (EffectSlot_unk::mNext)
    int m1C;
    int mParticleCount;              // 0x20
    EffectParticle_unk* mParticles;  // 0x24
    int m28;
    EffectSystemPool_unk m2C;
    float m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    int m5C;
    int m60;
    unsigned int mFlags;          // 0x64
    int m68;
    int mLayer;                   // 0x6C
    int m70;
    int mUpdating;                // 0x74
    int m78;
    int m7C;
    atn::EffectManagerCallback* mListener; // 0x80: inferred: the replay (ReplayBuffer)
    int mReplaying;               // 0x84
};

#endif
