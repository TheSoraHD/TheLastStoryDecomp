#ifndef GAME_MAPPARTSAREA_H
#define GAME_MAPPARTSAREA_H

#include "atn/LH_Binary.h"
#include "game/MapPartsBase.h"
#include "std/vector.h"

struct MapArea;
namespace atn {
class Scene;
struct Sphere;
}
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// Map part holding the areas of a map (vtable 0x80790218, 0x70 bytes): it loads a csv file
// (mFile) and creates one MapArea per area of it. Real name from the RSO (SetTask__12MapPartsAreaFPQ23atn4TaskiRQ23atn6CsvTab,
// ParseScene__12MapPartsAreaFPQ23atn5SceneRQ23atn6CsvTab, DWARF std::vector<MapArea*>).
// It is the area list of MapTask (atn::static_vector<MapPartsArea*, 8> at 0xA0) and of MapManager.
class MapPartsArea : public MapPartsBase {
public:
    // inferred: inlined into SetTask (0x80491EA0) and fn_80491F94
    MapPartsArea(atn::Task* parent, int type, const char* path)
        : MapPartsBase(parent, type), m64(0), mHash(fn_800DC6B4(path)) {
        mFile.vf0C(path);
    }
    virtual ~MapPartsArea();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(void* menu);
    virtual MapPartsBox_unk vf24(MapPartsTree_unk* tree);

    // creates the part from the file named in the csv (RSO name; 0x80491EA0)
    static MapPartsArea* SetTask(atn::Task* parent, int type, atn::CsvTab& csv);
    // inferred: creates the part from a file name (0x80491F94)
    static MapPartsArea* fn_80491F94(atn::Task* parent, int type, const char* path);
    void ParseScene(atn::Scene* scene, atn::CsvTab& csv); // 0x8049248C (RSO name)
    void fn_80494354();                  // inferred: reads the areas from mFile
    MapArea* fn_804962B0(const atn::Sphere* sphere);       // inferred: area hit by a sphere

    atn::LH_Binary mFile;            // 0x50: inferred: csv of the areas
    std::vector<MapArea*> mAreas;    // 0x58
    int m64;
    unsigned int mHash;              // 0x68: inferred: hash of the file name
// inferred: allocated with 0x70 bytes: 8-byte aligned like MapTask and MapPartsModel (probably through
// atn::Task)
} __attribute__((aligned(8)));

#endif
