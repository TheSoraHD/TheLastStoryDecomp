#include "game/MapTask.h"
#include "game/GameConfig_unk.h"
#include "game/MapInstance_unk.h"
#include "game/MapPartsBase.h"
#include "game/MapPartsLocatorSet.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: debug menu callback of "Enable" (atn::MindowsFunc_unk: the map and the address of the
// pointer to the value): shows or hides all the parts
void fn_8049D264(void* arg, void* value) {
    ((MapTask*)arg)->fn_804A0E18("<all>", **(int**)value);
}
// NONMATCHING: stub - process map part state.
extern "C" void fn_8049D2E4() {}
int MapTask::GetMapNameFromId(char* name, int type, int id, int subId) {
    switch (type) {
    case 0:
        sprintf(name, "wd%03d_%02d\0", id, subId);
        break;
    case 1:
        sprintf(name, "tw%02d_%02d\0", id, subId);
        break;
    case 2:
        sprintf(name, "dg%03d_%02d\0", id, subId);
        break;
    case 3:
    case 4:
        sprintf(name, "mp%03d_%02d\0", id, subId);
        break;
    default:
        return 0;
    }
    return 1;
}

// inferred: name of the map without the sub id
int MapTask::fn_8049D5DC(char* name, int type, int id, int subId) {
    switch (type) {
    case 0:
        sprintf(name, "wd%03d\0", id);
        break;
    case 1:
        sprintf(name, "tw%02d\0", id);
        break;
    case 2:
        sprintf(name, "dg%03d\0", id);
        break;
    case 3:
    case 4:
        sprintf(name, "mp%03d\0", id);
        break;
    default:
        return 0;
    }
    return 1;
}
// NONMATCHING: stub - allocate and initialize a map task.
MapTask* MapTask::SetTask(atn::Task* parent, const char* path) { return 0; }
// NONMATCHING: stub - choose and allocate a map task subtype.
MapTask* MapTask::fn_8049D704(atn::Task* parent, int type, int id, int subId) { return 0; }
// NONMATCHING: stub - constructor from a map file (needs atn::AABBTree in a header, see MapTask.h).
MapTask::MapTask(atn::Task* parent, const char* path) : atn::Task(parent) {}

// NONMATCHING: stub - constructor body and map resource initialization.
MapTask::MapTask(atn::Task* parent, int type, int id, int subId) : atn::Task(parent) {}
// NONMATCHING: stub - release map models, loaders and map-part entries.
MapTask::~MapTask() {}
// NONMATCHING: stub - update the map models and the current map selection.
int MapTask::vf0C() { return 0; }
// NONMATCHING: stub - load map resources.
extern "C" void fn_8049E480() {}
// NONMATCHING: stub - update visible map components.
void MapTask::vf10() {}

extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void fn_8006FE08(const atn::AABB_unk* box, unsigned int color); // inferred: draws a box (debug)
void MapTask::vf14() {
    if (lbl_8087F0A8->mDrawMapBounds != 0) {
        fn_8006FE08(&mBox, 0xFF00FFFFu);
    }
}

// NONMATCHING: stub - load and select a map model.
extern "C" void fn_8049ED90() {}
// NONMATCHING: stub - update map parts and resource state.
extern "C" void fn_8049F4F4() {}
// NONMATCHING: stub - sets the camera of the map.
void MapTask::fn_804A04AC(int camera, int a, int b, gs::CameraBox* box) {}
// inferred: shows all the parts, or hides those that contain `pos`
void MapTask::fn_804A0580(int show, const atn::Vector3* pos) {
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        if (show != 0) {
            part->MeltTask();
        } else if (part->vf20(*pos)) {
            part->FreezeTask();
        }
    }
}

// inferred: instance of the locator sets with the name `name`
MapInstance_unk* MapTask::fn_804A0614(const std::string& name) {
    unsigned int hash = fn_800DC6B4(name.c_str());
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        if (part->mType == 4) {
            MapPartsLocatorSet* set = (MapPartsLocatorSet*)part;
            for (int i = 0; i < set->mInstances.mSize; i++) {
                MapInstance_unk* inst = set->mInstances[i];
                if (hash == inst->mHash) {
                    return inst;
                }
            }
        }
    }
    return 0;
}

// NONMATCHING: stub - process a map transition.
extern "C" void fn_804A06B4() {}
// NONMATCHING: stub - process a map transition.
extern "C" void fn_804A0BCC() {}
// NONMATCHING: stub - update map resource loading.
void MapTask::fn_804A0E18(const char* name, int draw) {
    unsigned int hash = fn_800DC6B4(name);
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        part->vf2C(hash, draw);
    }
}
// NONMATCHING: stub - update map resource loading.
void MapTask::fn_804A0E90(unsigned int hash, int draw) {
    for (MapPartsBase* part = mParts; part != 0; part = part->mNext) {
        part->vf2C(hash, draw);
    }
}
// inferred: switches to the area list `area` (no blend)
// NONMATCHING: the empty stub of fn_804A0F80 is inlined (the original ends with a tail call)
void MapTask::fn_804A0EFC(MapPartsArea* area) {
    for (int i = 0; i < mAreas.mSize; i++) {
        if (mAreas.mData[i] == area) {
            fn_804A0F58(i, 1.0f, 0.0f);
            return;
        }
    }
}

// inferred: switches to the area list `index` with a blend (t, speed)
// NONMATCHING: the empty stub of fn_804A0F80 is inlined (the original ends with a tail call)
void MapTask::fn_804A0F58(int index, float t, float speed) {
    if (index < (int)mAreas.mSize) {
        m10C = mAreaIndex;
        mAreaIndex = index;
        m110 = t;
        m114 = speed;
        fn_804A0F80();
    }
}

// NONMATCHING: stub - transition between the area lists.
void MapTask::fn_804A0F80() {}
// NONMATCHING: stub - refreshes the map after a load.
void MapTask::fn_804A1C4C() {}
// NONMATCHING: stub - stores the state of the map.
void MapTask::fn_804A1D2C() {}
