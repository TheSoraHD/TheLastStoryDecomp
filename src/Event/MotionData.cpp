// inferred: the data classes of this file and Event::MovieTask (the file name comes from the
// last class; its string pool has the std::string error messages, so it includes
// std/string_members.h like game/GimmickManager.cpp).
#include "Event/MotionData.h"
#include "atn/Memory.h"
#include "atn/LH_Xsca.h"
#include "atn/LH_Binary.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/Model_unk.h"
#include "atn/VideoTHP.h"
#include "std/string_members.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: d2anime task
extern "C" void fn_800D2338(atn::Task* task);                                     // inferred: deletes a task
extern "C" void fn_8006AD24(std::string& path, const std::string& ext); // inferred: adds an extension
extern "C" std::string fn_8006B174(const std::string& path);          // inferred: file name of a path
extern "C" int fn_8006AA20(const char* path);                          // inferred: the file exists
// inferred: reads the poses of a binary file
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* node); // inferred: loads a model
extern "C" void fn_8008A00C(atn::Vector_unk<Event::PoseKey_unk>* poses, void* data, int size);
// inferred: debug text
extern "C" void* lbl_8087EEB0;
extern "C" void fn_800616C0(void* draw, float x, float y, float z, float w, float h, float f6, const char* text,
                            unsigned int color, int a, int b, int c);

// inferred: copies a string unless it is the destination itself (as in game/CampSaveLoad.cpp)
inline void CopyString_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}

namespace Event {

void MotionData::vf08() {
    // inferred: the loader is created as an atn::LH_Motion (the constructor result is used
    // directly) but destroyed non-virtually in vf0C
    atn::LH_Motion* loader = new (4, "", "", 0) atn::LH_Motion;
    m18 = (MotionHandle_unk*)loader;
    loader->fn_800A03E4(m08.c_str());
}

void MotionData::vf0C() {
    MotionHandle_unk* handle = m18;
    if (handle != 0) {
        handle->mLoader.fn_80473F88();
        handle->mLoader.m08 = 0;
        delete m18;
        m18 = 0;
    }
}

int MotionData::vf10() const {
    return m18->mLoader.fn_800A0448() == 0;
}

void AnimationData::vf08() {
    atn::Task* parent = m04;
    std::string path(m08);
    fn_8006AD24(path, "d2a");
    m18 = fn_801F3FF8(parent, path.c_str(), 0);
    m1C = 1;
    m18->fn_800D246C(1);
}

void AnimationData::vf0C() {
    unsigned int noKill = m04->mNoKill_unk;
    if (m18 != 0) {
        if (noKill != 0 && m1C == 1) {
            fn_800D2338(m18);
            m18 = 0;
        }
        m18 = 0;
    }
}

int AnimationData::vf10() const {
    if (m1C == 1 && m18->IsFlag_unk(2)) {
        m18->fn_800D246C(0);
        m18->mFlags |= 4;
        return 1;
    }
    return 0;
}

void ModelData::vf08() {
    m24 = new (4, "", "", 0) atn::Model(0x100, 0x28);
    fn_8008AD4C(m24, m08.c_str(), 0);
}

void ModelData::vf0C() {
    if (m24 != 0) {
        delete m24;
        m24 = 0;
    }
}

int ModelData::vf10() const {
    return m24->fn_8008B140() == 0;
}

void EffectData::vf08() {
    const char* name = m08.c_str();
    char* efp = strstr(name, ".efp");
    char* eff = strstr(name, ".eff");
    if (efp > eff) {
        m18 = new (4, "", "", 0) atn::EfpFile;
        ((atn::EfpFile*)m18)->fn_8023780C(name);
        m1C = 1;
    } else if (efp < eff) {
        m18 = new (4, "", "", 0) ResLoader_unk;
        ((ResLoader_unk*)m18)->fn_80237654(name);
        m1C = 2;
    }
}

void EffectData::vf0C() {
    if (m18 != 0) {
        if (m1C == 1) {
            ((atn::EfpFile*)m18)->Destroy();
            delete (atn::EfpFile*)m18;
        } else if (m1C == 2) {
            ((ResLoader_unk*)m18)->fn_8023772C();
            delete (ResLoader_unk*)m18;
        }
        m18 = 0;
    }
}

int EffectData::vf10() const {
    if (m1C == 1 && ((atn::EfpFile*)m18)->fn_80237874() != 0) {
        return 0;
    }
    if (m1C == 2 && ((ResLoader_unk*)m18)->fn_802376D0() != 0) {
        return 0;
    }
    return 1;
}

void CameraData::vf08() {
    m18 = new (4, "", "", 0) atn::LH_Xsca;
    m18->vf0C(m08.c_str());
}

void CameraData::vf0C() {
    if (m18 != 0) {
        m18->fn_80473F88();
        delete m18;
        m18 = 0;
    }
}

int CameraData::vf10() const {
    return m18->fn_80473F50() == 0;
}

void PoseData::vf08() {
    m18 = new (4, "", "", 0) atn::LH_Binary;
    m18->vf0C(m08.c_str());
}

void PoseData::vf0C() {
    if (m18 != 0) {
        m18->fn_80473F88();
        delete m18;
        m18 = 0;
        delete m1C;
        m1C = 0;
    }
}

int PoseData::vf10() const {
    if (m1C != 0) {
        return 1;
    }
    return m18->fn_80473F50() == 0;
}

// inferred: the temporary list is of a class derived from atn::Vector_unk (its inlined destructor
// tests the address of the object, as for derived classes elsewhere)
struct PoseList_unk : public atn::Vector_unk<PoseKey_unk> {};

// (copy loop: Assign_unk writes mData[i] = o[i]; the "" literals are the shared @STRING@ of
// Resize_unk<PoseKey_unk>, renamed in symbols.txt)
void PoseData::fn_80204230() {
    if (m1C == 0) {
        m1C = new (4, "", "", 0) atn::Vector_unk<PoseKey_unk>;
        PoseList_unk poses;
        fn_8008A00C(&poses, m18->fn_80470580(), m18->fn_8047059C());
        m1C->Assign_unk(poses);
    }
}

// inferred: kind of an RTMV movie (3 for ev5407); a separate function auto-inlined in the
// constructor (written in the constructor, the constructor is no longer inlined in vf08)
static int MovieKind_unk(const std::string& name) {
    unsigned int pos = name.find("ev5407");
    int kind = 2;
    if (pos != std::string::npos) {
        kind = 3;
    }
    return kind;
}

// inferred: a real function (its literals are in the file pool before "Movie : %s"), inlined in
// MovieData::vf08 and then dropped by the linker
MovieTask::MovieTask(atn::Task* parent, const std::string& name) : atn::Task(parent), m48(2), m4C(0) {
    m50[0] = 0;
    CopyString_unk(m50, fn_8006B174(name).c_str());
    if (fn_8006AA20(name.c_str()) != 0) {
        m4C = new (6, "", "", 0) atn::VideoTHP(name.c_str());
        if (name.find("RTMV") != std::string::npos) {
            m4C->m04 = MovieKind_unk(name);
        } else {
            m4C->m04 = 0;
        }
    }
}

MovieTask::~MovieTask() {
    if (m4C != 0) {
        delete m4C;
    }
}

int MovieTask::vf0C() {
    return --m48 <= 0;
}

void MovieTask::vf14() {
    if (m4C != 0) {
        if (m4C->m08 != 0) {
            m4C->vf28();
        }
    } else {
        char text[0x100];
        sprintf(text, "Movie : %s", m50);
        fn_800616C0(lbl_8087EEB0, 100.0f, 200.0f, 0.0f, 20.0f, 20.0f, 0.0f, text, 0xFFFFFFFF, 1, 1, 0);
    }
}

// NONMATCHING: 99.8% - two saved registers swap (r28/r29: the original coalesces parent with the
// pool base and the two literal pointers of the find calls; here parent with the "RTMV" pointer).
// Tried: ternary/if-else forms of the task creation, RTMV check in a helper, literal as argument.
void MovieData::vf08() {
    for (int i = 0; i < (int)m08.size(); i++) {
        if (m08[i] == '\\') {
            m08[i] = '/';
        }
    }
    atn::Task* parent = m04;
    MovieTask* task;
    if (parent != 0) {
        task = new (1, "", "", 0) MovieTask(parent, m08);
    } else {
        task = 0;
    }
    m18 = task;
}

void MovieData::vf0C() {
    unsigned int noKill = m04->mNoKill_unk;
    if (m18 != 0) {
        if (noKill != 0) {
            fn_800D2338(m18);
        }
        m18 = 0;
    }
}

int MovieData::vf10() const {
    return 1;
}

void MovieTask::vf10() {}

} // namespace Event
