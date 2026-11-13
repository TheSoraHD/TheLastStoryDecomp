#ifndef GAME_MAPPARTSGIMMICKLOC_UNK_H
#define GAME_MAPPARTSGIMMICKLOC_UNK_H

#include "game/MapPartsBase.h"
#include "atn/LH_Binary.h"
#include "atn/Sphere.h"
#include "atn/Color.h"
#include "atn/static_string.h"
#include "atn/Texture_unk.h"
#include "atn/Array_unk.h"
#include "atn/ResLoader_unk.h"
struct SwingPart_unk; // gmk/SwingPart_unk.h

class MapPartsArea;
namespace atn {
class Scene;
}

// inferred: record of the locator file (0xA0 bytes): an effect of the areas (SetAreaEffConfig); names invented
struct MapPartsGimmickLocRecord_unk {
    MapPartsGimmickLocRecord_unk() {} // 0x804997E4
    atn::EffFile mEff;        // 0x00: inferred: effect of the record
    atn::static_string<32, char, std::char_traits<char> > mName; // 0x0C: inferred: material name of the effect colour (SetAreaEffConfig)
    atn::Vector3 m2C;         // 0x2C: inferred: position (the area is looked up around it)
    atn::Vector3 m38;         // 0x38: inferred: rotation
    float m44;                // 0x44: inferred: scale of the effect (x scale of the locator)
    atn::Matrix3x4 mMtx;      // 0x48: inferred: placement (translation, then rotation z, y, x)
    atn::Color mColor;        // 0x78: inferred: effect colour of the area
    int m88;                  // inferred: 0: started once, 1: restarted every m8C frames (vf10)
    int m8C;                  // inferred: base count (frames between starts in mode 1)
    int m90;                  // inferred: random range of m8C (and number of variants)
    float m94;                // inferred: random range of the scale
    int m98;                  // inferred: variant picked at random (fn_80499B9C), frames left in mode 1
    unsigned int m9C;         // inferred: flags (1: effect option 8)
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
    void fn_804985E4(); // inferred: creates the gimmicks of the locator file
    void SetAreaEffConfig(); // 0x8049982C (RSO name): effect colours of the areas of the gimmicks
    void fn_8049994C(const atn::Vector3& pos, const atn::Vector3& rot); // inferred: places the gimmicks of the part
    void fn_80499B9C(); // inferred
    void fn_80499C48(); // inferred
    void fn_80499CCC(atn::Scene* scene);   // inferred: sets the scene of the gimmicks (MapTask::fn_8049E480)
    void fn_80499D60(MapPartsArea* areas); // inferred: picks the scene of each gimmick (MapTask::fn_8049E480)

    int m50;
    int m54;                  // 0x54: inferred: 2 when the effect colours go to the EffectManager
    atn::LH_Binary mFile;     // 0x58: inferred: locator file (constructor 0x80496F6C)
    atn::SimpleArray<MapPartsGimmickLocRecord_unk> mRecords; // 0x60: inferred: records (SetAreaEffConfig)
    atn::SimpleArray<SwingPart_unk*> mScenes;  // 0x68: inferred: swinging parts (keep the scene, fn_80499CCC)
    atn::static_string<128, char, std::char_traits<char> > mName; // 0x70
    atn::Texture_unk mTexture; // 0xF0
    unsigned char m120;       // inferred: shown (vf14, vf2C)
    unsigned char m121;
    atn::Vector3 m124;        // 0x124: inferred: offset of the locators (POSITION x y z)
    float m130;               // 0x130: inferred: hidden below this height (HIDE_BIRDVIEW_Y)
};

#endif
