#ifndef GAME_MAPTASK_H
#define GAME_MAPTASK_H

#include "atn/Task.h"
#include "atn/LoadHandle.h"

// inferred: object of the MapTask list at 0x70 (class unknown)
struct MapObject_unk {
    unsigned char m00[0x48];
    int mType;           // 0x48
    MapObject_unk* mNext; // 0x4C
};

// inferred from __vt__7MapTask (0x80790740); only the members used by TownTask are known
class MapTask : public atn::Task {
public:
    MapTask(atn::Task* parent); // 0x8049D974
    virtual ~MapTask();         // 0x8049DDD8
    virtual int vf0C();         // 0x8049DEE4
    virtual void vf10();        // 0x8049EBAC
    virtual void vf14();        // 0x8049ED6C

    // inferred: the loader at 0x58 is a concrete atn::LoadHandle subclass (type unknown)
    atn::LoadHandle* GetLoader_unk() { return (atn::LoadHandle*)m58; }

    unsigned char m48[0x58 - 0x48];
    unsigned char m58[0x70 - 0x58];
    MapObject_unk* m70; // inferred: first map object
    unsigned char m74[0xF8 - 0x74];
    int mF8;
    unsigned char mFC[0x11C - 0xFC];
};

#endif
