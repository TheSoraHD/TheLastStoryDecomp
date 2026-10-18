#ifndef GAME_MAPPARTSCOLLITREE_H
#define GAME_MAPPARTSCOLLITREE_H

#include "atn/Csv.h"
#include "game/MapPartsBase.h"
#include "atn/ColliBase.h"

class MapPartsColliTree : public MapPartsBase {
public:
    MapPartsColliTree(atn::Task* parent, void* arg, atn::CsvTab* csv);
    virtual ~MapPartsColliTree() {}
    virtual int vf0C();
    virtual int vf30();

    // inferred: factory used by the map parts loader
    static MapPartsColliTree* fn_80496420(atn::Task* parent, void* arg, atn::CsvTab* csv);

    atn::ColliTree mColli; // 0x50
};

#endif
