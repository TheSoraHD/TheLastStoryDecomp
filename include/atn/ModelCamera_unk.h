#ifndef ATN_MODELCAMERA_UNK_H
#define ATN_MODELCAMERA_UNK_H

#include "atn/Camera_unk.h"

// inferred: camera owned by a model (atn::ModelBase::m208; 0x1FC bytes). The same layout is kept
// locally in game/CharaTask.cpp, gmk/AutomaticDoor.cpp and Event/DialogHead.cpp. Names invented.
struct ModelCamera_unk {
    ModelCamera_unk() : m00(0) {}
    int m00;
    void* m04;
    atn::Camera mCamera; // 0x08
};

#endif
