#ifndef GMK_PLAYEFFECT_H
#define GMK_PLAYEFFECT_H

#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: plays one of two effects ("EFF", "EFP") with a sound ("SE"), optionally on a model
// ("MODEL"). The name is in the DOL (__vt__Q23gmk10PlayEffect).
class PlayEffect : public GimmickCommon {
public:
    // inferred: effect of one state (0x38 bytes; constructor 0x803F6AC4, destructor 0x803F6B04)
    struct Effect_unk {
        Effect_unk() {
            mSe[0] = 0;
        }
        atn::EffFile mRes;      // 0x00: "EFF"
        atn::EfpFile mEfp; // 0x0C: "EFP"
        char mSe[0x20];          // 0x18: "SE"
    };

    PlayEffect(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mOn(0), mInitState(1), mModel(0) {
        m54 = 0;
    }
    static GimmickCommon* fn_803F69E4(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~PlayEffect();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);

    void fn_803F7198(int index); // inferred: plays effect `index` (0: on, 1: off)

    int mOn;                 // 0xF4
    Effect_unk mEffects[2];  // 0xF8
    atn::SoundRef mSound;    // 0x168
    unsigned int mFlags;     // 0x16C: bit 0 "EFF_REFLECT"
    int mInitState;          // 0x170: 3 with "INIT ON", 1 otherwise
    atn::Model* mModel;       // 0x174
};

} // namespace gmk

#endif
