#ifndef ATN_CHARALIGHTDATA_UNK_H
#define ATN_CHARALIGHTDATA_UNK_H

#include "atn/Texture_unk.h"
#include "math/Vec.h"

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
// by the camera (atn::RenderManager::instance_) and the map parts
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

#endif
