#include "game/MapPartsColliTree.h"
#include "atn/Memory.h"

extern "C" char* strstr(const char*, const char*);
extern "C" int strcmp(const char*, const char*);
extern "C" const char* fn_8005B9CC(void* data); // inferred: returns the parts' resource name

// Order: with deferred inlining the functions are emitted in reverse; the constructor is
// inlined into fn_80496420 and its out-of-line copy is dead-stripped. Defining it just before
// fn_80496420 gives the original string pool order ("", "_bird.", "BIRDVIEW").
int MapPartsColliTree::vf30() {
    return (mColli.mFlags >> 5) & 1;
}

int MapPartsColliTree::vf0C() {
    if (mColli.fn_80058BBC()) {
        return 0;
    }
    mColli.mFlags |= 1;
    return 1;
}

MapPartsColliTree::MapPartsColliTree(atn::Task* parent, void* arg, void* data) : MapPartsBase(parent, arg) {
    const char* name = fn_8005B9CC(data);
    mColli.fn_80058B78(name);
    if (strstr(name, "_bird.") != 0 || strcmp(fn_8005B9CC(data), "BIRDVIEW") == 0) {
        mColli.mFlags |= 0x20;
    }
}

MapPartsColliTree* MapPartsColliTree::fn_80496420(atn::Task* parent, void* arg, void* data) {
    if (parent != 0) {
        return new (12, "", "", 0) MapPartsColliTree(parent, arg, data);
    }
    return 0;
}
