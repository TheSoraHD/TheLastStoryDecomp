#ifndef GMK_RAINDROPS_H
#define GMK_RAINDROPS_H

#include "atn/EfpHandle_unk.h"
#include "game/GimmickCommon.h"
#include "game/MapPartsRain.h"

namespace gmk {

// inferred: rain gimmick with up to 9 states, each with an effect ("EFP") and a rain volume
// ("RAINDROPS"); vf40 selects the state. The name is in the DOL (__vt__Q23gmk9RainDrops).
class RainDrops : public GimmickCommon {
public:
    RainDrops(atn::Task* parent, int id, int arg) : GimmickCommon(parent, id, arg), mCurrent(0) {
        m54 = 0;
        memset(mRain, 0, sizeof(mRain));
    }
    static GimmickCommon* fn_8042AFE4(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~RainDrops();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);

    int mCurrent;                 // 0xF4: inferred: state whose effect is playing
    atn::EfpFile mEfp[9];   // 0xF8
    MapPartsRain* mRain[9];       // 0x164
};

} // namespace gmk

#endif
