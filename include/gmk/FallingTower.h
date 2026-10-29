#ifndef GMK_FALLINGTOWER_H
#define GMK_FALLINGTOWER_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: tower that falls into the water: plays its motion and a splash effect ("SPLASH"
// frames), and floats with two sine waves per state ("WAVE", blended over "WAVEBLEND" frames) on
// the joints "joint1"/"joint2". The name is in the DOL (__vt__Q23gmk12FallingTower, 0x5E0 bytes).
class FallingTower : public GimmickCommon {
public:
    // inferred: one sine wave (names invented; same as gmk::Ocean::Wave_unk)
    struct Wave_unk {
        Wave_unk() : mAmp(0.0f), mSpeed(1.0f), mPhase(0.0f) {}
        inline float Height_unk(float t) const;
        float mAmp;
        float mSpeed;
        float mPhase;
    };

    // inferred: name buffer cleared by its constructor (names invented)
    struct Name_unk {
        Name_unk() { mStr[0] = 0; }
        char mStr[0x20];
    };

    FallingTower(atn::Task* parent, int id, int arg); // 0x8040FB44
    static GimmickCommon* fn_8040FAC0(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~FallingTower();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);

    void fn_8041044C(); // inferred: moves the joints

    atn::Model mModel;          // 0xF4
    atn::ColliObject mColli;   // 0x4C4
    atn::EfpFile mEfp;   // 0x54C: "EFP"
    int mFrame3;               // 0x558
    int mFrame4;               // 0x55C
    int mSplash0;              // 0x560: "SPLASH"
    int mSplash1;              // 0x564
    Name_unk mMaterial;        // 0x568: "MATALPHA"
    float mAlpha;              // 0x588
    float mBlend[2];           // 0x58C: "WAVEBLEND"
    Wave_unk mWaves[2][2];     // 0x594: [state][joint]
    unsigned char m5C4[0x5CC - 0x5C4];
    float mWaveTime;           // 0x5CC
    float mBlendStart;         // 0x5D0
    float mBlendDur;           // 0x5D4
    Wave_unk* mCur;            // 0x5D8
    Wave_unk* mNext;           // 0x5DC
};

} // namespace gmk

#endif
