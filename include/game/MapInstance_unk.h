#ifndef GAME_MAPINSTANCE_UNK_H
#define GAME_MAPINSTANCE_UNK_H

// inferred: model instances placed by game/MapPartsLocatorSet (0x90 bytes, created with new(0xC));
// names invented (classes of game/MapInstance_unk.cpp)
#include "gmk/Lemon.h"

// inferred: LOD distances of a locator
struct MapLod_unk {
    const char* mName; // inferred: name of the model ("data/building/%s.building")
    float m04;
    float m08;
    float m0C;
};

// inferred: locator of a map model (name, position, rotation in degrees, scale)
struct MapLocator_unk {
    const char* mName;     // 0x00
    atn::Vector3 mPos;     // 0x04
    atn::Vector3 mRot;     // 0x10
    atn::Vector3 mScale;   // 0x1C
    unsigned char m28[0x04];
    MapLod_unk* mLod;      // 0x2C
};

// inferred: locator with a shear (m30: xy, m34: xz, m38: yz)
struct MapLocatorShear_unk : public MapLocator_unk {
    float m30;
    float m34;
    float m38;
};

class MapInstance_unk {
public:
    MapInstance_unk(const char* path, MapLocator_unk* loc);
    MapInstance_unk(const char* path, MapLocatorShear_unk* loc);
    int fn_80490DE8();
    void fn_80490E38();
    void fn_80490EA0(int arg);

    atn::ModelInstance mInst;  // 0x00
    unsigned int mHidden : 1;  // 0x88
    unsigned int mFrame : 15;
    unsigned int mId : 16;
    unsigned int mHash;        // 0x8C
};

#endif
