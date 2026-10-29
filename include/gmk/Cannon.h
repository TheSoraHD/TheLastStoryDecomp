#ifndef GMK_CANNON_H
#define GMK_CANNON_H

#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"
#include "gmk/SeTable.h"

namespace gmk {

// inferred: cannon gimmick (vtable 0x8078D3C0): fires every "PARAM" frames (with a random part),
// then can be crushed. Models "MODEL"/"MODEL_CRUSH", effects "EFP_SHOOT"/"EFP_CRUSH", sounds
// "SE_PARAM" (ready, shot, crush). States follow the GimmickMsg_unk types (1 idle, 3 ready, 4 wait,
// 5 shoot, 6 crushed, 7 crush). The name is in the DOL (__vt__Q23gmk6Cannon).
class Cannon : public GimmickCommon {
public:
    Cannon(atn::Task* parent, int id, int arg); // 0x804083A8
    static GimmickCommon* fn_80408324(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~Cannon();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    Model_unk mModel;                     // 0xF4: "MODEL"
    Model_unk mModelCrush;                // 0x4C4: "MODEL_CRUSH"
    atn::EfpHandle_unk mEfpShoot;         // 0x894
    atn::EfpHandle_unk mEfpCrush;         // 0x8A0
    int mShootWait;                       // 0x8AC: "PARAM" 1
    int mShootWaitRand;                   // 0x8B0: "PARAM" 2
    int mTimer;                           // 0x8B4
    atn::FixedString_unk<0x20> mSeReady;  // 0x8B8: "SE_PARAM" 1
    atn::FixedString_unk<0x20> mSeShoot;  // 0x8D8: "SE_PARAM" 2
    atn::FixedString_unk<0x20> mSeCrush;  // 0x8F8: "SE_PARAM" 3
    int m918;
    float m91C;
    float m920;
    atn::Vector3 mWindDirection;          // 0x924
};

} // namespace gmk

#endif
