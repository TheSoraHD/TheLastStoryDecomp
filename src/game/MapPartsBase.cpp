#include "game/MapPartsBase.h"
#include "game/MapManager.h"
#include "atn/Rain.h"
#include "std/new.h"
#include "std/vector.h"

// inferred: complete translation-unit-local view of the area task; vtable 0x80790218, size 0x70.
class MapPartsArea : public MapPartsBase {
public:
    virtual ~MapPartsArea();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(void* menu);
    virtual MapPartsBox_unk vf24(MapPartsTree_unk* tree);

    unsigned char m50[0x20];
};

struct MapPartsAreaRain_unk {
    unsigned char m00[0x50];
    int m50;
    atn::Rain mRain;
};

// inferred: constructor-only local declaration; the complete scene type is private to CharaLightData_unk.cpp.
struct NewCampScene2_unk {
    unsigned char m00[0x2B4];
    NewCampScene2_unk();
};

struct MapPartsAreaInit_unk {
    unsigned char m00[0x28];
    int m28;
    int m2C;
    int m30;
    int m34;
    int m38;
    int m3C;
    int m40;
    int m44;
    int m48;
    NewCampScene2_unk m4C;
    unsigned char m300[8];
    int m308;
    int m30C;
    int m310;

    MapPartsAreaInit_unk();
};

struct MapPartsAreaParam_unk {
    atn::Vector3 mPos;
    float mC;
    float m10;
};

struct MapPartsAreaItem_unk {
    MapPartsAreaItem_unk(const MapPartsAreaItem_unk& other)
        : m00(other.m00), m04(other.m04), m08(other.m08), m0C(other.m0C), m10(other.m10) {}

    int m00;
    float m04;
    float m08;
    float m0C;
    float m10;
};


// NONMATCHING: 2.30% - map area allocation and loader setup are not reconstructed yet.
extern "C" MapPartsArea* fn_80491EA0(atn::Task*, void*, atn::CsvTab*) { return 0; }

// NONMATCHING: 2.37% - alternate map area allocation and loader setup are not reconstructed yet.
extern "C" MapPartsArea* fn_80491F94(atn::Task*, void*, atn::CsvTab*) { return 0; }

// NONMATCHING: 10.87% - cleanup of loaded actors and NewCampScene2_unk is not reconstructed yet.
MapPartsArea::~MapPartsArea() {}

extern "C" void* fn_804923AC(void* self) {
    return *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(self) + 0x2FC);
}
extern "C" void fn_8049248C(MapPartsArea*);

// NONMATCHING: 2.59% - map loading and readiness checks are not reconstructed yet.
int MapPartsArea::vf0C() {
    return 0;
}

// NONMATCHING: 0.05% - map area resource loading is not reconstructed yet.
extern "C" void fn_8049248C(MapPartsArea*) {}
extern "C" void fn_804948D8(MapPartsAreaRain_unk* self);

extern "C" float fn_800DC288(const char* text);
extern "C" void fn_804948F0(float* values, atn::CsvBase<char, 9, 13, 34>* csv);

// NONMATCHING: 0.28% - map area CSV parsing and resource setup are not reconstructed yet.
extern "C" void fn_80494354(MapPartsArea*, atn::CsvTab*) {}

// inferred: starts the rain renderer when this area's enable flag is set.
extern "C" void fn_804948D8(MapPartsAreaRain_unk* self) {
    if (self->m50 != 0) {
        self->mRain.fn_800B2180();
    }
}

extern "C" void fn_804948F0(float* values, atn::CsvBase<char, 9, 13, 34>* csv) {
    values[0] = fn_800DC288(csv->fn_8005B9CC());
    values[1] = fn_800DC288(csv->fn_8005B9CC());
    values[2] = fn_800DC288(csv->fn_8005B9CC());
    values[3] = fn_800DC288(csv->fn_8005B9CC());
}

MapPartsAreaInit_unk::MapPartsAreaInit_unk()
    : m28(0), m2C(0), m30(0), m34(0), m38(0), m3C(0), m40(0), m44(0), m48(0), m4C(), m308(0), m30C(0),
      m310(0) {}

// NONMATCHING: 96.56% - the allocator slow path matches, but MWCC loads size/capacity in the
// opposite register order in the fast path (same standalone std::vector pointer specialization).
// NONMATCHING: 92.69% - the in-place copy matches; the vector reallocation path differs from the
// original's hand-inlined 24-byte element growth sequence.
extern "C" void fn_804949D4(std::vector<MapSegment_unk>* values, const MapSegment_unk* value) {
    values->push_back(*value);
}

extern "C" void fn_80494D78(std::vector<void*>* values, void* const* value) {
    values->push_back(*value);
}

extern "C" void fn_8049506C() {}

// NONMATCHING: 92.69% - the in-place 0x14-byte record copy matches; the vector reallocation path differs.
extern "C" void fn_80495070(std::vector<MapPartsAreaParam_unk>* values, const MapPartsAreaParam_unk* value) {
    values->push_back(*value);
}

extern "C" void fn_80495414() {}

// NONMATCHING: 92.90% - the in-place field copy matches; the vector reallocation path differs.
extern "C" void fn_80495418(std::vector<MapPartsAreaItem_unk>* values, const MapPartsAreaItem_unk* value) {
    values->push_back(*value);
}
extern "C" void* fn_804962B0(MapPartsArea*, const void*);

// NONMATCHING: 0.22% - map debug menu entries are not reconstructed yet.
void MapPartsArea::vf1C(void*) {}

// NONMATCHING: 1.92% - map area color updates are not reconstructed yet.
void MapPartsArea::vf10() {}

// NONMATCHING: 0.70% - the map area's render/scene update is not reconstructed yet.
void MapPartsArea::vf14() {}

// NONMATCHING: 29.13% - map-part bounds are not reconstructed yet.
MapPartsBox_unk MapPartsArea::vf24(MapPartsTree_unk* tree) {
    return MapPartsBase::vf24(tree);
}

// NONMATCHING: 2.55% - lookup across map area collision and item records is not reconstructed yet.
extern "C" void* fn_804962B0(MapPartsArea*, const void*) { return 0; }

int MapPartsBase::vf30() {
    return 0;
}

void MapPartsBase::vf2C(unsigned int, int) {}

void MapPartsBase::vf28(atn::CsvTab*) {}

bool MapPartsBase::vf20(const atn::Vector3&) {
    return false;
}
extern "C" void fn_804963F8(MapPartsBase* self, MapPartsBase* next);
extern "C" void fn_80496414(MapPartsBase* self, MapPartsBase* next);

MapPartsBase::MapPartsBase(atn::Task* parent, void* arg) : atn::Task(parent), m48(arg), mNext(0) {}

extern "C" void fn_804963F8(MapPartsBase* self, MapPartsBase* next) {
    while (self->mNext != 0) {
        self = self->mNext;
    }
    self->mNext = next;
}

extern "C" void fn_80496414(MapPartsBase* self, MapPartsBase* next) {
    self->mNext = next;
}

void MapPartsBase::vf1C(void*) {}
