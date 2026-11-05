#ifndef GAME_MAPAREA_H
#define GAME_MAPAREA_H

#include "atn/AABB_unk.h"
#include "atn/Color.h"
#include "atn/Scene.h"
#include "math/Vec.h"
#include "std/vector.h"
#include "std/vector_string.h"

class AreaEffConfig;
class MapPartsRain;

// inferred: record of the vector at MapArea 0x34 (0x14 bytes, read from five csv values by
// MapPartsBase fn_80494354); name invented
struct MapPartsAreaParam_unk {
    MapPartsAreaParam_unk() {} // inferred: weak copy 0x8049506C

    atn::Vector3 mPos;
    float mC;
    float m10;
};

// Area of a map part (0x314 bytes): its name, the boxes and records read from the csv of the
// MapPartsArea that owns it, its scene (lights, fog...), its effect config and rain. Real names from
// the RSO (DWARF MapArea::MapArea, MapArea::MatColorInfo, std::vector<MapArea*>). Created by
// MapPartsBase fn_80494354 (implicit constructor 0x80494964, destroyed inline by ~MapPartsArea).
// Member names invented unless noted.
struct MapArea {
    // inferred: color of a material of the area (0x14 bytes; hash of the material name and color
    // read from the csv, MapPartsBase fn_80494354); the name is in the DWARF of the RSO
    struct MatColorInfo {
        MatColorInfo() {} // inferred: weak copy 0x80495414
        MatColorInfo(const MatColorInfo& other) : mHash(other.mHash), mColor(other.mColor) {}

        unsigned int mHash;
        atn::Color mColor;
    };

    // inferred: the scene of the area (Event::Manager fn_80481B9C copies its light settings); name invented
    atn::Scene& GetScene_unk() { return mScene; }

    int m00;
    char mName[0x20];                            // 0x04: inferred: strncpy of 0x20 bytes, last one cleared
    unsigned int mHash;                          // 0x24: inferred: hash of mName (Event::Manager compares it)
    std::vector<atn::AABB_unk> mSegments;        // 0x28: inferred: MapManager::FindArea tests both corners
    std::vector<MapPartsAreaParam_unk> mParams;  // 0x34
    std::vector<std::string> mPaths;             // 0x40: inferred: file names read from the csv
    atn::Scene mScene;                           // 0x4C: member, not a base (the destructor of ~MapPartsArea
                                                 // destroys it with flag -1 and no null test)
    AreaEffConfig* mEffConfig;                   // 0x300: inferred: AreaEffConfig::fn_80041A80
    MapPartsRain* mRainPart;                     // 0x304: inferred: MapPartsRain::fn_8049CDBC
    std::vector<MatColorInfo> mMatColors;        // 0x308
};

#endif
