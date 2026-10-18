#ifndef ATN_LH_TEXTURE_H
#define ATN_LH_TEXTURE_H

#include "atn/GXDecl.h"
#include "atn/LoadHandle.h"

namespace atn {

// inferred from __vt__Q23atn10LH_Texture (0x8078FFB0); no destructor declared (weak copy 0x800D57B0)
class LH_Texture : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x804781B8

    void fn_80478158(GXTexObj* obj, int wrapS, int wrapT); // inferred: initialises a GX texture
    int fn_80478198();                                     // inferred: field 0x2C of the header
};

} // namespace atn

#endif
