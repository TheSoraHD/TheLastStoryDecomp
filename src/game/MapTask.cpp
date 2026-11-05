#include "game/MapTask.h"

// NONMATCHING: stub - iterate map-part objects and update the linked task entries.
extern "C" void fn_8049D264() {}
// NONMATCHING: stub - process map part state.
extern "C" void fn_8049D2E4() {}
// NONMATCHING: stub - name of the map file of an id.
int MapTask::GetMapNameFromId(char* name, int type, int id, int subId) { return 0; }
// NONMATCHING: stub - name of a map (other folders).
int MapTask::fn_8049D5DC(char* name, int type, int id, int subId) { return 0; }
// NONMATCHING: stub - allocate and initialize a map task.
MapTask* MapTask::SetTask(atn::Task* parent, const char* path) { return 0; }
// NONMATCHING: stub - choose and allocate a map task subtype.
MapTask* MapTask::fn_8049D704(atn::Task* parent, int type, int id, int subId) { return 0; }
// NONMATCHING: stub - initialize a MapTask with a loader argument.
extern "C" void fn_8049D7E4() {}

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

// inferred: fades the map overlay when the settings flag at 0x1C is enabled.
struct MapTaskSettings_unk {
    unsigned char m00[0x1C];
    int m1C;
};
extern "C" MapTaskSettings_unk* lbl_8087F0A8;
extern "C" void fn_8006FE08(void* target, unsigned int color);
void MapTask::vf14() {
    if (lbl_8087F0A8->m1C != 0) {
        fn_8006FE08(&mBox, 0xFF00FFFFu);
    }
}

// NONMATCHING: stub - load and select a map model.
extern "C" void fn_8049ED90() {}
// NONMATCHING: stub - update map parts and resource state.
extern "C" void fn_8049F4F4() {}
// NONMATCHING: stub - sets the camera of the map.
void MapTask::fn_804A04AC(int camera, int a, int b, gs::CameraBox* box) {}
// NONMATCHING: stub - update map object state.
void MapTask::fn_804A0580(int arg, const atn::Vector3* pos) {}
// NONMATCHING: stub - locator of the gimmick parts.
void* MapTask::fn_804A0614(const std::string& name) { return 0; }
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
// NONMATCHING: stub - refreshes the map after a load.
void MapTask::fn_804A1C4C() {}
// NONMATCHING: stub - stores the state of the map.
void MapTask::fn_804A1D2C() {}
