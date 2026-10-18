#ifndef GAME_MAPPARTSBASE_H
#define GAME_MAPPARTSBASE_H

#include "atn/Csv.h"
#include "atn/Task.h"

class MapPartsBase : public atn::Task {
public:
    MapPartsBase(atn::Task* parent, void* arg); // 0x804963A4
    virtual ~MapPartsBase() {}
    virtual void vf1C(void* menu); // 0x8049641C: inferred: adds debug menu entries
    virtual int vf20();  // 0x8049639C
    virtual void vf24(); // 0x8042488C
    virtual void vf28(atn::CsvTab* csv); // 0x80496398: inferred: reads CSV data (called by MapPartsModel ctor)
    virtual void vf2C(); // 0x80496394
    virtual int vf30();  // 0x8049638C

    void* m48;
    MapPartsBase* mNext; // 0x4C: inferred from the list append at 0x804963F8
};

#endif
