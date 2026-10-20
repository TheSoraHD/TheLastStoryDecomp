#ifndef GMK_CHARALIGHT_H
#define GMK_CHARALIGHT_H

#include "atn/Texture_unk.h"
#include "game/GimmickCommon.h"

// inferred: array of textures with its count (destructor inlined)
struct TextureArray_unk {
    TextureArray_unk() : mCount(0), mData(0) {}
    ~TextureArray_unk() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mCount = 0;
    }
    int mCount;
    atn::Texture_unk* mData;
};

// inferred: light projected on the characters (0x50 bytes, class name unknown); also referenced
// by the camera (lbl_8087EFB4) and the map parts
struct CharaLightData_unk {
    CharaLightData_unk(); // 0x800C16C0
    CharaLightData_unk(const char* texture); // 0x800C1720: inferred: loads the texture
    void fn_800C1814(const char* path); // inferred: loads the texture
    int fn_800C1990();                  // inferred: nonzero while loading

    int m00;
    atn::Vector2_unk m04;
    atn::Vector2_unk mSize;   // 0x0C
    atn::Vector2_unk m14; // inferred: "SPEED" (gmk::RoomLight)
    float m1C;            // inferred: "ROT" (gmk::RoomLight)
    float m20;
    atn::Vector3 mDir;        // 0x24
    float m30;
    float m34;
    float m38;
    TextureArray_unk mTextures; // 0x3C
    int m44; // inferred: current texture
    int m48; // inferred: frames of each texture (0 = no animation)
    int m4C; // inferred: frame counter
};

namespace gmk {

// inferred: light that follows a character and changes size with the state. The name is in the
// DOL (__vt__Q23gmk10CharaLight).
class CharaLight : public GimmickCommon {
public:
    // inferred: parameters of states 2-6 ("PARAM")
    struct Param_unk {
        Param_unk() : m00(0.0f), mSize(0.0f, 0.0f) {}
        float m00;
        atn::Vector2_unk mSize;
    };

    CharaLight(atn::Task* parent, GimmickDesc_unk* desc); // 0x80422684
    static GimmickCommon* fn_80422610(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~CharaLight();
    virtual int vf0C();
    virtual void vf10(); // 0x80422BF4: not decompiled yet
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);


    GimmickDesc_unk* mDesc;    // 0xF4
    CharaLightData_unk mLight; // 0xF8
    void* mTarget;             // 0x148: inferred: character followed by the light
    int m14C;
    float m150;
    Param_unk mParams[5];      // 0x154
    atn::Vector2_unk mStartSize; // 0x190
    atn::Vector2_unk mCurSize;   // 0x198
};

} // namespace gmk

#endif
