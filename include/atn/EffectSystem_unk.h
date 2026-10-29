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
class EffectListener_unk;

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

// inferred: effect instance of the system (one per started effect; linked list from
// EffectSystem_unk::mInsts; fields named by use, the size is not known yet)
struct EffectInst_unk {
    EffectInst_unk(); // 0x8022BDB4
    int m00;
    EffectInst_unk* mNext;  // 0x04
    int m08;
    int m0C;
    int m10;
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
    unsigned char m38[0xAC - 0x38];
    EffectColor_unk mColor; // 0xAC
    int mBC;
    int mC0;
    int mC4;
    float mC8;
    float mCC;
    atn::Vector3 mVec;      // 0xD0
    float mRate;            // 0xDC
};

// inferred: element of the system with a constructor (0x1B8 bytes; fields named by offset)
struct EffectSlot_unk {
    EffectSlot_unk(); // 0x8022BE1C
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    float m14;
    float m18;
    int m1C;
    int m20;
    int m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    unsigned char m38[0x190 - 0x38];
    int m190;
    float m194;
    float m198;
    float m19C;
    float m1A0;
    float m1A4;
    int m1A8;
    int m1AC;
    int m1B0;
    int m1B4;
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

// inferred: the effect system kept by the manager (0x88 bytes; its members are in other files)
// inferred: two empty levels over std::vector (five null checks of `this` in the destructor)
struct EffectPtrVectorBase_unk : public std::vector<void*> {};
struct EffectPtrVector_unk : public EffectPtrVectorBase_unk {};
// inferred: element with a constructor (its new[] has a cookie)
struct EffectSystemElem_unk {
    EffectSystemElem_unk();
    unsigned char m00[4];
};
struct EffectSystemPool_unk {
    EffectSystemPool_unk() : m00(0), m04(0) {}
    int m00;
    int m04;
    atn::SimpleArray<EffectSystemElem_unk> m08;
    EffectPtrVector_unk m10;
};

class EffectSystem_unk {
public:
    EffectSystem_unk()
        : m00(0), m04(0), m08(0), mInsts(0), m10(0), m14(0), m18(0), m1C(0), mParticleCount(0), mParticles(0), m28(0), m48(0.0f),
          m4C(0), m50(0), m54(0), m58(0), m5C(0), m60(0), mFlags(0), m68(0), mLayer(0), m70(0), mUpdating(0),
          m78(0), m7C(0), mListener(0), mReplaying(0) {}
    void fn_8022B850(int a, int b, int c); // inferred: init
    void fn_8022BEAC();
    void fn_8022C2A0();                    // inferred: release
    void fn_8022C3F0();                    // inferred: update
    void* fn_8022FAC4(void* res, int a, int b, int c, const atn::Vector3* pos, const atn::Vector3* rot,
                      const EffectColor_unk* color, int d, int e, float scale);
    void fn_802328EC(EffectInst_unk* inst, int arg); // inferred: stops an instance
    void fn_80232ACC(void* owner);
    int fn_80232B98(unsigned int owner, int group);
    int fn_80232BEC(unsigned int owner, int group, int count);
    int fn_80232D04(unsigned int owner, int group, int flag);
    void fn_80232E48();
    void fn_80232ED4();
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

    int m00;
    int m04;
    int m08;
    EffectInst_unk* mInsts; // 0x0C
    int m10;
    int m14;
    int m18;
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
    EffectListener_unk* mListener; // 0x80
    int mReplaying;               // 0x84
};

#endif
