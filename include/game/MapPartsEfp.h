#ifndef GAME_MAPPARTSEFP_H
#define GAME_MAPPARTSEFP_H

#include "atn/Csv.h"
#include "game/MapPartsBase.h"
#include "atn/EfpHandle_unk.h"

class MapPartsEfp : public MapPartsBase {
public:
    MapPartsEfp(atn::Task* parent, void* arg, atn::CsvTab* csv); // 0x80496670
    virtual ~MapPartsEfp() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    // inferred: factory used by the map parts loader
    static MapPartsEfp* fn_804965EC(atn::Task* parent, void* arg, atn::CsvTab* csv);

    atn::EfpFile mEfp; // 0x50
    int m5C;
    int mMode;  // 0x60: 0 = ONCE, 1 = RANDOM, 2 = LOOP
    int mRate;  // 0x64: RANDOM: 1/mRate chance per frame
    int mDone;  // 0x68: ONCE: already played
    int m6C;
};

#endif
