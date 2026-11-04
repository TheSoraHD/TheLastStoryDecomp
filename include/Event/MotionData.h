#ifndef EVENT_MOTIONDATA_H
#define EVENT_MOTIONDATA_H

// Event data classes of Event/MotionData.cpp (declared in the order of the file: vtables are
// emitted in reverse declaration order). Member names are invented.

#include "Event/CommonData.h"
#include "atn/Task.h"
#include "atn/Array_unk.h"
#include "atn/LH_Motion.h"
#include "math/Vec.h"
#include "std/string.h"
#include "Event/PoseKey_unk.h"

namespace atn {
struct Model;
}
namespace atn {
struct EffFile;
}

namespace atn {
class LH_Xsca;
class LH_Binary;
class EfpFile;
class VideoTHP;
class D2A_Task;
} // namespace atn

namespace Event {

// inferred: wrapper of the motion loader (the loader is destroyed as a member, non-virtually)
struct MotionHandle_unk {
    atn::LH_Motion mLoader;
};

class MotionData : public CommonData {
public:
    // inferred: inlined by the reader of "MOID" (Event/EventParser_unk.cpp)
    MotionData(Sequence* owner, const char* name, int id) : CommonData(owner, name, id), m18(0) {}
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    MotionHandle_unk* m18;
};

class AnimationData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    atn::D2A_Task* m18; // inferred: d2anime task
    int m1C;
};

class ModelData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    // inferred: inlined by the reader of "MDMA" (Event/EventParser_unk.cpp); name invented
    void SetFile_unk(const std::string& file) { m18 = file; }

    std::string m18; // inferred: destroyed with the data (Event::Sequence, 0x8053D8F0)
    atn::Model* m24;
};

class EffectData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    void* m18; // inferred: atn::EfpFile (m1C == 1) or atn::EffFile (m1C == 2)
    int m1C;
};

class CameraData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    atn::LH_Xsca* m18;
};

class PoseData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;
    void fn_80204230(); // inferred: reads the poses once the file is loaded

    atn::LH_Binary* m18;
    atn::SimpleVector<PoseKey_unk>* m1C;
};

// inferred: task playing a movie (0x70 bytes)
class MovieTask : public atn::Task {
public:
    MovieTask(atn::Task* parent, const std::string& name);
    virtual ~MovieTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    int m48;
    atn::VideoTHP* m4C;
    char m50[0x20]; // inferred: movie name
};

class MovieData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    MovieTask* m18;
};

} // namespace Event

#endif
