#ifndef GAME_MAPPARTSBASE_H
#define GAME_MAPPARTSBASE_H

#include "atn/Csv.h"
#include "atn/Task.h"
#include "math/Vec.h"

// inferred: bounding box of a map part (min, max); name invented
struct MapPartsBox_unk {
    MapPartsBox_unk() {}
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};
// inferred: tree the map parts are registered in (atn/AABBTree.cpp); name invented
struct MapPartsTree_unk;

class MapPartsBase : public atn::Task {
public:
    MapPartsBase(atn::Task* parent, void* arg); // 0x804963A4
    virtual ~MapPartsBase() {}
    virtual void vf1C(void* menu); // 0x8049641C: inferred: adds debug menu entries
    virtual bool vf20(const atn::Vector3& pos);  // 0x8049639C
    // inferred: bounding box; empty here (weak copy 0x8042488C, emitted by gmk/RotaryRoom; inlined
    // by MapPartsModel::vf24, whose temporaries are then laid out as in an inlined call)
    virtual MapPartsBox_unk vf24(MapPartsTree_unk* tree) {
        MapPartsBox_unk box;
        box.mMin = atn::Vector3(1e16f, 1e16f, 1e16f);
        box.mMax = atn::Vector3(-1e16f, -1e16f, -1e16f);
        return box;
    }
    virtual void vf28(atn::CsvTab* csv); // 0x80496398: inferred: reads CSV data (called by MapPartsModel ctor)
    virtual void vf2C(unsigned int hash, int draw); // 0x80496394
    virtual int vf30();  // 0x8049638C

    void* m48;
    MapPartsBase* mNext; // 0x4C: inferred from the list append at 0x804963F8
};

#endif
