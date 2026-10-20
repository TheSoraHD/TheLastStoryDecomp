#ifndef GAME_MAPPARTSRAIN_H
#define GAME_MAPPARTSRAIN_H

#include "atn/Csv.h"
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
        Box_unk(const atn::Vector3& a, const atn::Vector3& b) : mMin(a), mMax(b) {}
        atn::Vector3 mMin;
        atn::Vector3 mMax;
    };

    MapPartsRain(atn::Task* parent, void* arg, atn::CsvTab* csv); // 0x8049CE40
    virtual ~MapPartsRain();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(void* menu);

    // inferred: factory used by the map parts loader
    static MapPartsRain* fn_8049CDBC(atn::Task* parent, void* arg, atn::CsvTab* csv);
    void fn_8049D1A4(MapPartsRain* a, MapPartsRain* b, float t);

    int mOn;          // 0x50
    atn::Rain mRain;  // 0x54
    atn::Vector3 mDir;         // 0x68
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
