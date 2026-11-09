#include "game/ReplayBuffer.h"
#include "math/Vec.h"
#include "atn/NodeChunk.h"
#include "atn/ModelPose_unk.h"
#include "game/CharaTask.h"
#include "game/PlayerManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"

// game/ReplayBuffer.cpp (0x8056A84C-0x80570A14): the replay of the field (records the characters and their
// effects). Partial: only some of the functions are written.

extern "C" int strcmp(const char* a, const char* b);
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party

// inferred: rotation around y keeping the translation (inlined twice in fn_8056A84C; name invented)
static inline void RotateY_unk(atn::Matrix3x4* mtx, float angle) {
    atn::Vector3 t(mtx->m[0][3], mtx->m[1][3], mtx->m[2][3]);
    mtx->m[0][3] = 0.0f;
    mtx->m[1][3] = 0.0f;
    mtx->m[2][3] = 0.0f;
    Mtx rot;
    PSMTXRotRad(rot, 'y', angle);
    PSMTXConcat(rot, mtx->m, mtx->m);
    mtx->m[0][3] = t.x;
    mtx->m[1][3] = t.y;
    mtx->m[2][3] = t.z;
}

// inferred: node callback of the replay models: turns the upper body (bound with the angle in a
// boost::function, invoker 0x8056B2C0)
extern "C" void fn_8056A84C(atn::Matrix3x4* mtx, const atn::NodeChunk& node, const atn::ModelNodeCallbackContext& ctx,
                            float angle) {
    const char* name = node.mName;
    if (strcmp(name, "spine") == 0) {
        if ((float)__fabs(angle) > 0.9424779f) {
            angle = angle > 0.0f ? 0.9424779f : -0.9424779f;
        }
        RotateY_unk(mtx, angle);
    } else if (strcmp(name, "hips") == 0) {
        if ((float)__fabs(angle) > 0.9424779f) {
            float a;
            if (angle > 0.0f) {
                a = angle - 0.9424779f;
            } else {
                a = 0.9424779f + angle;
            }
            RotateY_unk(mtx, a);
        }
    }
}

ReplayRecorder_unk::ReplayRecorder_unk() {
    m460 = 0;
}

ReplayRecorder_unk::~ReplayRecorder_unk() {}

GameTaskData_unk::~GameTaskData_unk() {}
