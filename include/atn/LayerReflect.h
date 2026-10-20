#ifndef ATN_LAYERREFLECT_H
#define ATN_LAYERREFLECT_H

#include "atn/Layer3D.h"
#include "atn/Texture_unk.h"

namespace atn {

class LayerReflect : public Layer3D {
public:
    LayerReflect(); // 0x800B68BC
    virtual ~LayerReflect() {}
    virtual void vf1C();
    virtual void vf20();

    Texture_unk mTexture; // 0x5C
};

} // namespace atn

#endif
