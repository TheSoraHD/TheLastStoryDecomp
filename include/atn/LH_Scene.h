#ifndef ATN_LH_SCENE_H
#define ATN_LH_SCENE_H

#include "atn/LoadHandle.h"

namespace atn {

struct Light;
struct Camera;

// inferred from __vt__Q23atn8LH_Scene (0x8078FF60); no destructor declared (weak copy 0x800C1F5C)
class LH_Scene : public LoadHandle {
public:
    virtual void vf0C(const char* path); // 0x80477D60
    // inferred: accessors of the loaded file (0 when not loaded); names invented
    Light* fn_80477FA4();   // inferred: lights built from the file
    Camera* fn_80477FC0();  // inferred: cameras built from the file
    int fn_80477FDC();      // inferred: number of lights
    int fn_80477FF8();      // inferred: number of cameras
    int fn_80478014();      // inferred: number of light records
    void** fn_80478034();   // inferred: light records
};

} // namespace atn

#endif
