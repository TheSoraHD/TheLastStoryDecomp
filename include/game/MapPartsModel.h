#ifndef GAME_MAPPARTSMODEL_H
#define GAME_MAPPARTSMODEL_H

#include "atn/Csv.h"
#include "game/MapPartsBase.h"
#include "math/Vec.h"

// inferred: model instance embedded at MapPartsModel+0x50 (probably the same class as
// chr::Motion_unk, which characters embed at +0xB0). Names invented.
struct Model_unk {
    int fn_8008B140();                                // inferred: nonzero while loading
    int fn_80092814(const char* name, int arg);       // inferred: node index by name, < 0 if missing

    // inferred: matrix of node `index`, null for a missing node (inlined at 0x8049CCD4)
    MtxPtr GetNodeMtx_unk(int index) {
        return (index < 0) ? 0 : mNodeMtx[index];
    }

    unsigned char m00[0x08];
    Mtx mMtx;        // 0x08: inferred: world matrix
    unsigned char m38[0x3C - 0x38];
    Mtx* mNodeMtx;   // 0x3C
    unsigned char m40[0x3D0 - 0x40];
};

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
