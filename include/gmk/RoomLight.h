#ifndef GMK_ROOMLIGHT_H
#define GMK_ROOMLIGHT_H

#include "gmk/CharaLight.h"

// inferred: light of gmk::RoomLight: CharaLightData_unk followed by the draw layer set by
// "BACKGROUND" (read by the map part callback 0x8041FED8 and copied to the camera)
struct RoomLightData_unk : public CharaLightData_unk {
    int m50;
};

namespace gmk {

// inferred: light projected on the room (a CharaLightData_unk given to the camera and the map
// parts) that grows or shrinks with the state. The name is in the DOL (__vt__Q23gmk9RoomLight,
// 0x160 bytes).
class RoomLight : public GimmickCommon {
public:
    RoomLight(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mFrames(0.0f), mFrame(0), mMinSize(0.0f, 0.0f), mDefSize(0.0f, 0.0f) {
        m54 = 0;
        mLight.m04 = atn::Vector2_unk(0.0f, 0.0f);
        mLight.mSize = atn::Vector2_unk(0.0f, 0.0f);
        mLight.m00 = 0;
        mLight.m50 = -1;
    }
    static GimmickCommon* fn_8041FF08(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~RoomLight();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf6C(void* menu);

    float mFrames;              // 0xF4: "PARAM": inferred: frames of the size change
    int mFrame;                 // 0xF8
    RoomLightData_unk mLight;   // 0xFC
    atn::Vector2_unk mMinSize;  // 0x150: "SIZE"
    atn::Vector2_unk mDefSize;  // 0x158: "DEF_SIZE"
};

} // namespace gmk

#endif
