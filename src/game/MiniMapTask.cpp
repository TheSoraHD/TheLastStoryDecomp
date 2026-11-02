#include "game/MiniMapTask.h"

#include "atn/Memory.h"

extern "C" MiniMapTask* fn_804A7EA4(atn::Task* parent, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) MiniMapTask(parent, path);
    }
    return 0;
}

// NONMATCHING: 21.93% - resource setup and record initialization are incomplete.
MiniMapTask::MiniMapTask(atn::Task* parent, const char* path)
    : atn::Task(parent), m4C(1), m50(), m58(), m208(0x00A00001, 0x20), m640(0, 0), m834(-1), m838(-1),
      m83C(), m860(), m15C4(), m15F4(), m160C(0) {
    m50.vf0C(path);
}

// NONMATCHING: 55.67% - target destructor releases resources and owned tasks.
MiniMapTask::~MiniMapTask() {}

// NONMATCHING: 1.75% - target handles the resource completion callback.
int MiniMapTask::vf0C() {
    return 0;
}

// NONMATCHING: 0.62% - target updates minimap animation/state.
void MiniMapTask::vf10() {}

// NONMATCHING: 70.76% - target preserves direct float-to-integer conversion details.
extern "C" unsigned int fn_804A88C8(const float* rgba) {
    const unsigned int r = (int)(rgba[0] * 255.0f);
    const unsigned int g = (int)(rgba[1] * 255.0f);
    const unsigned int b = (int)(rgba[2] * 255.0f);
    const unsigned int a = (int)(rgba[3] * 255.0f);
    return (r << 24) | (g << 16) | (b << 8) | a;
}

extern "C" void fn_804A893C(unsigned int* object, unsigned int flags) {
    object[1] |= flags;
}

// NONMATCHING: 0.07% - target performs the main minimap draw/update pass.
void MiniMapTask::vf14() {}

// NONMATCHING: 0.22% - target builds and submits minimap render data.
void MiniMapTask::vf18(atn::RenderView_unk* view) {
    (void)view;
}

// NONMATCHING: 0.33% - target updates the minimap render target.
void MiniMapTask::vf10(atn::RenderView_unk* view) {
    (void)view;
}

// NONMATCHING: 20.00% - target returns the renderer's concrete type value.
int MiniMapTask::GetRenderType_unk() {
    return 0;
}

// NONMATCHING: 10.00% - target reports render visibility/state.
void MiniMapTask::vf1C() {}
