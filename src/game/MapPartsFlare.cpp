#include "game/MapPartsFlare.h"
#include "atn/Memory.h"

extern "C" int atoi(const char* str);
// The game calls this without an end pointer (r4 is not set), so it is declared with one argument.
extern "C" double strtod(const char* str);

MapPartsFlare* MapPartsFlare::fn_80496C3C(atn::Task* parent, void* arg, atn::CsvTab* csv) {
    if (parent != 0) {
        return new (12, "", "", 0) MapPartsFlare(parent, arg, csv);
    }
    return 0;
}

// NONMATCHING: 99.7% - the loop increments of i and the element offset are scheduled in the
// opposite order. Tried: local declaration orders, ++i, while loop, pointer arithmetic, casts.
MapPartsFlare::MapPartsFlare(atn::Task* parent, void* arg, atn::CsvTab* csv) : MapPartsBase(parent, arg) {
    atn::LensFlareElem* e;
    int count = atoi(csv->fn_8005B9CC());
    mFlare = new (6, "", "", 0) atn::LensFlare(1, count, atn::Vector3(0.0f, 0.0f, 0.0f), 30.0f);
    for (int i = 0; i < count; i++) {
        e = &mFlare->mElems[i];
        e->m00 = atoi(csv->fn_8005B9CC());
        e->m08 = strtod(csv->fn_8005B9CC());
        e->m04 = strtod(csv->fn_8005B9CC());
        e->m0C = strtod(csv->fn_8005B9CC());
        e->m10 = strtod(csv->fn_8005B9CC());
        e->m14 = strtod(csv->fn_8005B9CC());
    }
}

MapPartsFlare::~MapPartsFlare() {
    delete mFlare;
}

int MapPartsFlare::vf0C() {
    if (mFlare->fn_800774D4()) {
        return 0;
    }
    return fn_800D3FA4();
}

void MapPartsFlare::vf10() {
    mFlare->fn_80077534();
}

void MapPartsFlare::vf14() {}
