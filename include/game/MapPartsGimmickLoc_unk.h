#ifndef GAME_MAPPARTSGIMMICKLOC_UNK_H
#define GAME_MAPPARTSGIMMICKLOC_UNK_H

#include "game/MapPartsBase.h"
#include "atn/LH_Binary.h"
#include "atn/Sphere.h"
#include "atn/Color.h"
#include "atn/static_string.h"
#include "atn/Texture_unk.h"

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

// inferred: object that keeps a scene pointer at 0x64 (entries of MapPartsGimmickLoc::m6C; real class unknown)
struct MapPartsGimmickLocScene_unk {
    unsigned char m00[8];
    atn::Sphere mSphere;      // 0x08: inferred: bounds used to find its area (fn_80499D60)
    unsigned char m18[0x64 - 0x18];
    atn::Scene* m64;
};

// inferred: record of the locator file (0xA0 bytes): an effect of the areas (SetAreaEffConfig); names invented
struct MapPartsGimmickLocRecord_unk {
    unsigned char m00[0x0C];
    char mName[0x20];         // 0x0C: inferred: material name of the effect colour (SetAreaEffConfig)
    atn::Vector3 m2C;         // 0x2C: inferred: position (the area is looked up around it)
    atn::Vector3 m38;         // 0x38: inferred: rotation
    unsigned char m44[0x78 - 0x44];
    atn::Color mColor;        // 0x78: inferred: effect colour of the area
    unsigned char m88[0x90 - 0x88];
    int m90;                  // inferred: number of variants
    int m94;
    int m98;                  // inferred: variant picked at random (fn_80499B9C)
    int m9C;
};

// inferred: map-part location controller (RTTI at 0x80790560; fields are established by local
// offset accessors in game/MapPartsGimmickLoc.cpp).
class MapPartsGimmickLoc : public MapPartsBase {
public:
    static MapPartsGimmickLoc* SetTask(atn::Task* parent, int type, atn::CsvTab& csv); // 0x80496EE8 (RSO name)
    MapPartsGimmickLoc(atn::Task* parent, int type, atn::CsvTab& csv); // 0x80496F6C
    virtual ~MapPartsGimmickLoc();                        // 0x804970D4
    virtual int vf0C();                                   // 0x80497374
    virtual void vf10();                                  // 0x80497874
    virtual void vf14();                                  // 0x80498574
    virtual bool vf20(const atn::Vector3& pos);           // 0x80499FE4
    virtual void vf28(atn::CsvTab* csv);                  // 0x80497248
    virtual void vf2C(unsigned int hash, int draw);       // 0x80499E14
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
    MapPartsGimmickLocRecord_unk* m64; // 0x64: inferred: records
    unsigned int m68;         // 0x68: inferred: number of the entries of m6C
    MapPartsGimmickLocScene_unk** m6C; // 0x6C: inferred: objects that keep the scene (fn_80499CCC)
    atn::static_string<128, char, std::char_traits<char> > mName; // 0x70
    atn::Texture_unk mTexture; // 0xF0
    unsigned char m120;       // inferred: shown (vf14, vf2C)
    unsigned char m121;
    float m124[4];            // inferred: a colour (0 at the start)
};

#endif
