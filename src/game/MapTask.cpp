#include "game/MapTask.h"

// NONMATCHING: stub - iterate map-part objects and update the linked task entries.
extern "C" void fn_8049D264() {}
// NONMATCHING: stub - process map part state.
extern "C" void fn_8049D2E4() {}
// NONMATCHING: stub - update the visible map parts.
extern "C" void fn_8049D52C() {}
// NONMATCHING: stub - update the map part list.
extern "C" void fn_8049D5DC() {}
// NONMATCHING: stub - allocate and initialize a map task.
extern "C" void fn_8049D68C() {}
// NONMATCHING: stub - choose and allocate a map task subtype.
extern "C" void fn_8049D704() {}
// NONMATCHING: stub - initialize a MapTask with a loader argument.
extern "C" void fn_8049D7E4() {}

// NONMATCHING: stub - constructor body and map resource initialization.
MapTask::MapTask(atn::Task* parent) : atn::Task(parent) {}
// NONMATCHING: stub - release map models, loaders and map-part entries.
MapTask::~MapTask() {}
// NONMATCHING: stub - update the map models and the current map selection.
int MapTask::vf0C() { return 0; }
// NONMATCHING: stub - load map resources.
extern "C" void fn_8049E480() {}
// NONMATCHING: stub - update visible map components.
void MapTask::vf10() {}

// inferred: fades the map overlay when the settings flag at 0x1C is enabled.
struct MapTaskSettings_unk {
    unsigned char m00[0x1C];
    int m1C;
};
extern "C" MapTaskSettings_unk* lbl_8087F0A8;
extern "C" void fn_8006FE08(void* target, unsigned int color);
void MapTask::vf14() {
    if (lbl_8087F0A8->m1C != 0) {
        fn_8006FE08((char*)this + 0x88, 0xFF00FFFFu);
    }
}

// NONMATCHING: stub - load and select a map model.
extern "C" void fn_8049ED90() {}
// NONMATCHING: stub - update map parts and resource state.
extern "C" void fn_8049F4F4() {}
// NONMATCHING: stub - update map visibility.
extern "C" void fn_804A04AC() {}
// NONMATCHING: stub - update map object state.
extern "C" void fn_804A0580() {}
// NONMATCHING: stub - select map models.
extern "C" void fn_804A0614() {}
// NONMATCHING: stub - process a map transition.
extern "C" void fn_804A06B4() {}
// NONMATCHING: stub - process a map transition.
extern "C" void fn_804A0BCC() {}
// NONMATCHING: stub - update map resource loading.
extern "C" void fn_804A0E18() {}
// NONMATCHING: stub - update map resource loading.
extern "C" void fn_804A0E90() {}
// NONMATCHING: stub - update map resource loading.
extern "C" void fn_804A0EFC() {}
// NONMATCHING: stub - reset map transition state.
extern "C" void fn_804A0F58() {}
// NONMATCHING: stub - transition between map models.
extern "C" void fn_804A0F80() {}
// NONMATCHING: stub - synchronize transition state.
extern "C" void fn_804A1C4C() {}
// NONMATCHING: stub - restore transition state.
extern "C" void fn_804A1D2C() {}
