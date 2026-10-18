#ifndef GAME_MAPPARTSMODEL_H
#define GAME_MAPPARTSMODEL_H

#include "atn/Csv.h"
#include "game/MapPartsBase.h"
#include "math/Vec.h"
#include "atn/Model_unk.h"

// inferred from __vt__13MapPartsModel (0x80790630)
class MapPartsModel : public MapPartsBase {
public:
    MapPartsModel(atn::Task* parent, void* arg, atn::CsvTab* csv); // 0x8049BAAC
    virtual ~MapPartsModel();          // 0x8049C350
    virtual int vf0C();                // 0x8049C3C0
    virtual void vf10();               // 0x8049C454
    virtual void vf14();               // 0x8049C458
    virtual int vf20();                // 0x8049C8A8
    virtual void vf24();               // 0x8049C688
    virtual void vf28(atn::CsvTab* csv);     // 0x8049BB4C
    virtual void vf2C();               // 0x8049C7AC

    Model_unk mModel; // 0x50
    unsigned char m420[0x44C - 0x420];
};

#endif
