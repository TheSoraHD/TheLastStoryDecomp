// NOTE: the split of this unit is shorter than the source file: the instance pointer (0x8087F558),
// the boost::function vtable of fn_80490EB8 and its guard (0x807C8AC8/0x8087F55C), the type_info
// of the callback type (0x807901D8) and the functions from 0x80491418 (the callback 0x804914BC and
// the members of MapManager it calls) lie in the auto units around it.
#include "atn/Task.h"
#include "atn/Memory.h"
#include "atn/ColliBase.h"
#include "std/vector.h"
#include <boost/function.hpp>

// NONMATCHING: the functor manager of the callback (0x80491280, a boost template instance) is at
// 99.51%: only its reference to the type_info 0x807901D8 (outside the split).

class MapManager;

// inferred: map part found by fn_804914BC (returned through a base at offset 0x4C)
struct MapHit_unk;

// inferred: camera callback: finds the map part hit by a sphere
typedef boost::function<MapHit_unk*(atn::ColliSphere::Sphere_unk, MapManager*)> MapHitFunc_unk;

// inferred: camera (lbl_8087EFB4); m300 is a callback with its owner at 0x314
struct Camera_unk {
    // inferred: weak copy at 0x8049103C (takes the function by value, so it is never inlined);
    // name invented
    void SetMapHit_unk(MapHitFunc_unk func, MapManager* owner) {
        mMapHit = func;
        mMapHitOwner = owner;
    }

    unsigned char m000[0x300];
    MapHitFunc_unk mMapHit;  // 0x300
    MapManager* mMapHitOwner; // 0x314
};
extern "C" Camera_unk* lbl_8087EFB4;

// inferred: callback given to the camera (0x804914BC, outside the split)
extern "C" MapHit_unk* fn_804914BC(atn::ColliSphere::Sphere_unk sphere, MapManager* manager);

// inferred: list of the map parts; a class derived from the vector (the destructor of 0x80491324
// tests the address four times)
struct MapPartList_unk : public std::vector<void*> {};

// inferred from __vt__10MapManager (0x807901E0): manager of the map parts (singleton)
class MapManager : public atn::Task {
public:
    MapManager(atn::Task* parent);
    static MapManager* fn_80490EB8(atn::Task* parent);
    virtual ~MapManager();
    virtual int vf0C();

    static MapManager* sInstance; // 0x8087F558

    int m48;
    unsigned char m4C[0x14C - 0x4C];
    int m14C;
    MapPartList_unk m150;
    int m15C;
};

MapManager* MapManager::sInstance;

// NONMATCHING: 99.18% - only relocations: the instance pointer, the boost::function vtable and
// its guard are outside the split
MapManager* MapManager::fn_80490EB8(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (12, "", "", 0) MapManager(parent);
    }
    return sInstance;
}

// inferred: not inline; inlined into fn_80490EB8
MapManager::MapManager(atn::Task* parent) : atn::Task(parent), m48(0), m14C(0) {
    lbl_8087EFB4->SetMapHit_unk(&fn_804914BC, this);
}

// NONMATCHING: 99.92% - only the relocation of the instance pointer (0x8087F558, outside the split)
MapManager::~MapManager() {
    sInstance = 0;
    lbl_8087EFB4->SetMapHit_unk(MapHitFunc_unk(), 0);
}

int MapManager::vf0C() {
    return fn_800D3FA4();
}
