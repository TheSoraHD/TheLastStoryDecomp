#ifndef GAME_MAPPARTSGIMMICKLOC_UNK_H
#define GAME_MAPPARTSGIMMICKLOC_UNK_H

#include "game/MapPartsBase.h"
#include "atn/LH_Binary.h"

class MapPartsArea;
namespace atn {
class Scene;
}

// inferred: three-word vector view of 8-byte records in MapPartsGimmickLoc's temporary CSV data.
struct MapPartsGimmickLocArray8_unk {
    bool fn_80499684() const;
    unsigned int fn_80499694() const;
    void* fn_80499724(int index) const;

    void* mData;
    unsigned int mSize;
    unsigned int mCapacity;
};

// inferred: three-word vector view of pointer entries in the same CSV parser.
struct MapPartsGimmickLocArray4_unk {
    void* fn_8049981C(int index) const;

    void* mData;
    unsigned int mSize;
    unsigned int mCapacity;
};

// inferred: object configured by MapPartsGimmickLoc::fn_804985E4 through these setters (a gimmick
// created from the locator file; real class unknown). They were members of MapPartsGimmickLoc, whose
// constructor puts other members at these offsets (atn::LH_Binary at 0x58, the name at 0x70).
class MapPartsGimmickLocObj_unk {
public:
    void fn_80498EAC(const atn::Vector3& value);
    void fn_80498EC0(const atn::Vector3& value);
    void fn_80498ED4(const atn::Vector3& value);
    void fn_80498EE8(int value);
    void fn_80498EF0(int value);
    void fn_80498F14(float value, const atn::Vector3& range);

    unsigned char m00[0x58];
    int m58;
    unsigned char m5C[0x10];
    atn::Vector3 m6C;
    atn::Vector3 m78;
    unsigned char m84[0x0C];
    atn::Vector3 m90;
    unsigned char m9C[8];
    int mA4;
};

// inferred: map-part location controller (RTTI at 0x80790560; fields are established by local
// offset accessors in game/MapPartsGimmickLoc.cpp).
class MapPartsGimmickLoc : public MapPartsBase {
public:
    static MapPartsGimmickLoc* SetTask(atn::Task* parent, int type, atn::CsvTab& csv); // 0x80496EE8 (RSO name)
    void* fn_80498E7C(int index);
    void fn_80498EF8(const atn::Vector3& scale);
    void fn_80498F08();
    void fn_80498F0C(int value);
    void fn_80498F4C();
    void fn_80499388(int value);
    void SetAreaEffConfig(); // 0x8049982C (RSO name): effect colours of the areas of the gimmicks
    void fn_8049994C(const atn::Vector3& pos, const atn::Vector3& rot); // inferred: places the gimmicks of the part
    void fn_80499B9C(); // inferred
    void fn_80499C48(); // inferred
    void fn_80499CCC(atn::Scene* scene);   // inferred: sets the scene of the gimmicks (MapTask::fn_8049E480)
    void fn_80499D60(MapPartsArea* areas); // inferred: picks the scene of each gimmick (MapTask::fn_8049E480)

    int m50;
    int m54;                  // 0x54: inferred: 2 when the effect colours go to the EffectManager
    atn::LH_Binary mFile;     // 0x58: inferred: locator file (constructor 0x80496F6C)
    unsigned int m60;         // 0x60: inferred: number of records of 0xA0 bytes (SetAreaEffConfig)
    unsigned char* m64;       // 0x64: inferred: records
    int m68;
    int m6C;
};

#endif
