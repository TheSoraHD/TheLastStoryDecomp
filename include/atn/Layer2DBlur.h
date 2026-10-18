#ifndef ATN_LAYER2DBLUR_H
#define ATN_LAYER2DBLUR_H

#include "atn/Layer3D.h"

namespace atn {

class Layer2DBlur : public Layer2D {
public:
    Layer2DBlur(); // 0x800B2E2C
    virtual ~Layer2DBlur() {}
    virtual void vf1C();
    virtual void vf20();
};

} // namespace atn

#endif
