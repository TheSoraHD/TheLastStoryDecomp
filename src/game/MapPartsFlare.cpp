#include "game/MapPartsFlare.h"
#include "atn/Memory.h"

extern "C" int atoi(const char* str);
// The game calls this without an end pointer (r4 is not set), so it is declared with one argument.
extern "C" double strtod(const char* str);
extern "C" const char* fn_8005B9CC(void* data); // inferred: returns the next CSV field

void MapPartsFlare::vf14() {}

void MapPartsFlare::vf10() {
    mFlare->fn_80077534();
}

int MapPartsFlare::vf0C() {
    if (mFlare->fn_800774D4()) {
        return 0;
    }
    return fn_800D3FA4();
}

MapPartsFlare::~MapPartsFlare() {
    delete mFlare;
}

// NONMATCHING: 99.7% - the loop increments of i and the element offset are scheduled in the
// opposite order. Tried: local declaration orders, ++i, while loop, pointer arithmetic, casts.
MapPartsFlare::MapPartsFlare(atn::Task* parent, void* arg, void* data) : MapPartsBase(parent, arg) {
    atn::LensFlareElem* e;
    int count = atoi(fn_8005B9CC(data));
    mFlare = new (6, "", "", 0) atn::LensFlare(1, count, Vec(0.0f, 0.0f, 0.0f), 30.0f);
    for (int i = 0; i < count; i++) {
        e = &mFlare->mElems[i];
        e->m00 = atoi(fn_8005B9CC(data));
        e->m08 = strtod(fn_8005B9CC(data));
        e->m04 = strtod(fn_8005B9CC(data));
        e->m0C = strtod(fn_8005B9CC(data));
        e->m10 = strtod(fn_8005B9CC(data));
        e->m14 = strtod(fn_8005B9CC(data));
    }
}

MapPartsFlare* MapPartsFlare::fn_80496C3C(atn::Task* parent, void* arg, void* data) {
    if (parent != 0) {
        return new (12, "", "", 0) MapPartsFlare(parent, arg, data);
    }
    return 0;
}
