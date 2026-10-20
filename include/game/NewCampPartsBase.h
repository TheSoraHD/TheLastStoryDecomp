#ifndef GAME_NEWCAMPPARTSBASE_H
#define GAME_NEWCAMPPARTSBASE_H

#include "atn/Task.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "math/Vec.h"
#include "atn/LH_Xsca.h"

namespace atn {
class D3Anime;
}
struct NewCampPartsModel_unk;

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

// inferred: entry of the model tables of the parts (0x40 bytes, array NewCampPartsBase::m50); the
// constructor is a weak copy at 0x801D082C (game/CosEditProtector.cpp), same body as
// NewCampGmkInfo_unk (game/NewCampPartsGmk.cpp); names invented
struct NewCampPartsEntry_unk {
    NewCampPartsEntry_unk() {
        mAnime = 0;
        mModel = 0;
        m3C = 0;
        mMtx.SetIdentity();
        m04 = 0;
    }

    atn::D3Anime* mAnime;          // 0x00
    short m04;
    NewCampPartsModel_unk* mModel; // 0x08
    atn::Matrix3x4 mMtx;           // 0x0C
    unsigned char m3C;
};

// inferred: table given to fn_8050FB38/fn_8050FD38 (model and d2anime names); names invented
struct NewCampPartsFile_unk {
    const char* mModel;
    const char* mAnime;
    unsigned char m08; // inferred: flag (1 for some status boards)
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
    virtual void vf3C(int arg);          // 0x80511208
    virtual void vf40(void* group);      // 0x805112F8: inferred: adds the debug menu items
    virtual void vf44();                 // 0x80511714
    virtual void vf48();                 // 0x805117BC
    virtual int vf4C() { return 0; }     // 0x801CFC28
    virtual void vf50();                 // 0x8051012C
    virtual void vf54(NewCampPartsEntry_unk* entry, const char* name); // 0x805100FC
    virtual void vf58(NewCampPartsEntry_unk* entry, const char* name); // 0x80510720
    virtual void vf5C(NewCampPartsEntry_unk* entry);                   // 0x80510564
    virtual void vf60(NewCampPartsEntry_unk* entry, int a, int b, float c); // 0x805109CC

    atn::Task* m48;                      // inferred: the parent given to the constructor
    int m4C;                             // inferred: number of entries of the derived tables
    ArrayPtr_unk<NewCampPartsEntry_unk> m50; // inferred: entries (m4C)
    void* m54;                           // inferred: array of 0x0C-byte objects with a LoadHandle (dtor 0x801D0960)
    float m58;
    atn::LH_Xsca m5C;                    // 0x5C: inferred: camera
    float m64[6];
    int m7C;                             // inferred: state of the derived parts
    unsigned int m80;                    // inferred: cursor
    int m84;                             // inferred: number of cursor positions
    int m88;
    int m8C;
    int m90;
    unsigned char m94;
    unsigned char m95;
    unsigned char m96; // inferred: the list must be read again
    unsigned char m97;
    std::string m98;
    atn::Vector3 mA4;                    // inferred: "OffsetPos"
    atn::Vector3 mB0;                    // inferred: "OffsetScl"
    atn::Vector3 mBC;                    // inferred: "LocatorScl"
    atn::Vector3 mC8;                    // inferred: "BasePos"
};

#endif
