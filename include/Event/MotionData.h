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
struct ResLoader_unk;

namespace atn {
class LH_Xsca;
class LH_Binary;
class EfpFile;
class VideoTHP;
} // namespace atn

namespace Event {

// inferred: owner of the event data (a task; flag 0x04000000 at 0x98)
struct DataOwner_unk : public atn::Task {
    unsigned char m48[0x98 - 0x48];
    unsigned int m98_0 : 5;
    unsigned int mNoKill_unk : 1; // inferred: keeps the d2anime tasks alive when set
    unsigned int m98_6 : 26;
};

// inferred: wrapper of the motion loader (the loader is destroyed as a member, non-virtually)
struct MotionHandle_unk {
    atn::LH_Motion mLoader;
};

class MotionData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    DataOwner_unk* m04;
    std::string m08; // inferred: file name
    int m14;
    MotionHandle_unk* m18;
};

class AnimationData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    DataOwner_unk* m04;
    std::string m08;
    int m14;
    atn::Task* m18; // inferred: d2anime task
    int m1C;
};

class ModelData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    DataOwner_unk* m04;
    std::string m08;
    int m14;
    int m18;
    int m1C;
    int m20;
    atn::Model* m24;
};

class EffectData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    DataOwner_unk* m04;
    std::string m08;
    int m14;
    void* m18; // inferred: atn::EfpFile (m1C == 1) or ResLoader_unk (m1C == 2)
    int m1C;
};

class CameraData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;

    DataOwner_unk* m04;
    std::string m08;
    int m14;
    atn::LH_Xsca* m18;
};

class PoseData : public CommonData {
public:
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;
    void fn_80204230(); // inferred: reads the poses once the file is loaded

    DataOwner_unk* m04;
    std::string m08;
    int m14;
    atn::LH_Binary* m18;
    atn::Vector_unk<PoseKey_unk>* m1C;
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

    DataOwner_unk* m04;
    std::string m08;
    int m14;
    MovieTask* m18;
};

} // namespace Event

#endif
