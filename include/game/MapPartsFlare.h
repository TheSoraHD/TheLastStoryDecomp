#ifndef GAME_MAPPARTSFLARE_H
#define GAME_MAPPARTSFLARE_H

#include "game/MapPartsBase.h"
#include "atn/LensFlare.h"

class MapPartsFlare : public MapPartsBase {
public:
    MapPartsFlare(atn::Task* parent, void* arg, void* data); // 0x80496CC0
    virtual ~MapPartsFlare();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    // inferred: factory used by the map parts loader
    static MapPartsFlare* fn_80496C3C(atn::Task* parent, void* arg, void* data);

    atn::LensFlare* mFlare; // 0x50
    int m54;
};

#endif
