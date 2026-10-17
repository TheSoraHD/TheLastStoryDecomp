#ifndef ATN_LAYERPOSTFX_H
#define ATN_LAYERPOSTFX_H

#include "atn/Layer3D.h"

namespace atn {

// inferred: 8-byte float pair, copied with psq_l/psq_st
struct Vec2f {
    float x, y;
};

class LayerPostFx : public IRenderLayer {
public:
    LayerPostFx();
    virtual ~LayerPostFx();
    virtual void vf10();
    virtual void vf14(void* arg);
    virtual void vf1C();
    virtual void vf20();

    Vec2f mSaved; // 0x14: value of the render state's 0x58 saved in vf1C, restored in vf20
};

} // namespace atn

#endif
