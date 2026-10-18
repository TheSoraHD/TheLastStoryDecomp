#ifndef GAME_MAPPARTSRAIN_H
#define GAME_MAPPARTSRAIN_H

#include "game/MapPartsBase.h"
#include "atn/Rain.h"
#include "math/Vec.h"

class MapPartsRain : public MapPartsBase {
public:
    // inferred: RGBA colour read from the CSV
    struct Color_unk {
        float r, g, b, a;
    };
    // inferred: two corners of the rain volume
    struct Box_unk {
        Box_unk() {}
        Box_unk(const Vec& a, const Vec& b) : mMin(a), mMax(b) {}
        Vec mMin;
        Vec mMax;
    };

    MapPartsRain(atn::Task* parent, void* arg, void* data); // 0x8049CE40
    virtual ~MapPartsRain();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(void* menu);

    // inferred: factory used by the map parts loader
    static MapPartsRain* fn_8049CDBC(atn::Task* parent, void* arg, void* data);
    void fn_8049D1A4(MapPartsRain* a, MapPartsRain* b);

    int mOn;          // 0x50
    atn::Rain mRain;  // 0x54
    Vec mDir;         // 0x68
    Color_unk mColor; // 0x74
    Box_unk mBox;     // 0x84
    int mCount;       // 0x9C
    float mSpeedMin;  // 0xA0
    float mSpeedMax;  // 0xA4
    float mSize;      // 0xA8
    float mLength;    // 0xAC
    float mLengthV;   // 0xB0
    int mB4;
};

#endif
