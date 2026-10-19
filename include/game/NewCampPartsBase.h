#ifndef GAME_NEWCAMPPARTSBASE_H
#define GAME_NEWCAMPPARTSBASE_H

#include "atn/Task.h"
#include "atn/Memory.h"
#include "std/string.h"

// inferred: array owned through a pointer, replaced with new[] and freed with delete[] (pattern of
// NewCampPartsBase::m50 in its constructor 0x8050F93C and NewCampPartsGmk::mD4); names invented
template <class T>
class ArrayPtr_unk {
public:
    ArrayPtr_unk() : mData(0) {}
    ~ArrayPtr_unk() { delete[] mData; }
    void Reset_unk(T* data) {
        T* old = mData;
        mData = data;
        delete[] old;
    }
    T& operator[](int i) { return mData[i]; }

    T* mData;
};

// inferred: scene whose nodes are searched by NewCampPartsGmk::vf1C (0x80512B44)
struct NewCampScene_unk;

// inferred from __vt__16NewCampPartsBase (0x807932B8), its constructor (0x8050F93C) and its
// destructor (0x8050FA7C). The inline virtuals have weak copies in game/CosEditBase.cpp
// (0x801CFC28-0x801CFC44).
class NewCampPartsBase : public atn::Task {
public:
    NewCampPartsBase(atn::Task* parent); // 0x8050F93C
    virtual ~NewCampPartsBase();         // 0x8050FA7C
    virtual int vf0C();                  // 0x8051007C
    virtual void vf10();                 // 0x805100F8
    virtual void vf14();                 // 0x80510CB0
    virtual void vf1C(NewCampScene_unk* scene) {} // 0x801CFC44
    virtual void vf20();                 // 0x80512024
    virtual void vf24() {}               // 0x801CFC40
    virtual void vf28() {}               // 0x801CFC3C
    virtual void vf2C();                 // 0x80512020
    virtual void vf30() {}               // 0x801CFC38
    virtual void vf34();                 // 0x80512018
    virtual unsigned char vf38() { return m94; } // 0x801CFC30
    virtual void vf3C();                 // 0x80511208
    virtual void vf40();                 // 0x805112F8
    virtual void vf44();                 // 0x80511714
    virtual void vf48();                 // 0x805117BC
    virtual int vf4C() { return 0; }     // 0x801CFC28
    virtual void vf50();                 // 0x8051012C
    virtual void vf54();                 // 0x805100FC
    virtual void vf58();                 // 0x80510720
    virtual void vf5C();                 // 0x80510564
    virtual void vf60();                 // 0x805109CC

    atn::Task* m48;                      // inferred: the parent given to the constructor
    int m4C;                             // inferred: number of entries of the derived tables
    unsigned char m50[0x94 - 0x50];
    unsigned char m94;
    unsigned char m95[0x98 - 0x95];
    std::string m98;
    unsigned char mA4[0xD4 - 0xA4];
};

#endif
