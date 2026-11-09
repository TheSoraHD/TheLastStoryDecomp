#include "game/ReplayBuffer.h"
#include "math/Vec.h"
#include "atn/NodeChunk.h"
#include "atn/ModelPose_unk.h"
#include "game/CharaTask.h"
#include "game/PlayerManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "atn/Model_unk.h"
#include "game/BattleTask.h"
#include "atn/EffectManager.h"

// game/ReplayBuffer.cpp (0x8056A84C-0x80570A14): the replay of the field (records the characters and their
// effects). Partial: only some of the functions are written.

extern "C" int strcmp(const char* a, const char* b);
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party
extern "C" BattleTask* lbl_8087F048;    // inferred: battle manager
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);

// inferred: effect event of the replay with its frame (names invented)
struct ReplayEffectEntry_unk {
    int mFrame;
    EffectEvent_unk mEvent;
};

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

// inferred: a hidden or dead character is not recorded (name invented)
static inline bool IsSkipped_unk(CharaTask* chara) {
    bool ret = false;
    if (chara->IsFlag_unk(4) || chara->IsFlag_unk(1)) {
        ret = true;
    }
    return ret;
}

// NONMATCHING: 99.38% - r3/r4 swapped between the flag word and the result of IsSkipped_unk
void ReplayRecorder_unk::fn_8056B3D4() {
    atn::Matrix3x4 inv;
    atn::Matrix3x4 trans;
    PlayerTask* leader = lbl_8087F8A0->mHead;
    PSMTXTrans(trans.m, leader->mPos.x, leader->mPos.y, leader->mPos.z);
    m464 = trans;
    m460 = 0;
    inv = m464;
    PSMTXInverse(inv.m, inv.m);
    for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
        if (!player->IsFlag_unk(4) && !(player->mType == 2 && IsSkipped_unk(player))) {
            fn_8056B558(player, inv);
        }
    }
    for (NpcTask* npc = NpcManager::sInstance->mHead; npc != 0; npc = npc->mNext) {
        if (!npc->IsFlag_unk(4) && !(npc->mType == 2 && IsSkipped_unk(npc)) && npc->fn_8016E484()) {
            fn_8056B558(npc, inv);
        }
    }
}

GameTaskData_unk::~GameTaskData_unk() {}

// inferred: index of the frame `frames` / 2 frames back
void GameTaskData_unk::fn_8056F2F8(int frames) {
    int back = frames / 2;
    int oldest = 1 - m4517C;
    int idx = m45178 + (-1 < (back > oldest ? back : oldest) ? -1 : (back > oldest ? back : oldest));
    if (idx < 0) {
        idx += 45;
    }
    ReplayFrame_unk* frame = m5BDB4 != 0 ? &mLive : &mFrames[idx];
    for (int i = 0; i < frame->mNumCharas; i++) {
        frame->mCharas[i].mChara->fn_80149A30();
    }
    for (int i = 0; i < frame->mNumModels; i++) {
        frame->mModels[i].mModel->fn_8008CD60();
    }
}

void GameTaskData_unk::fn_805704AC() {
    m5BDB4 = 1;
    int end = m00004 + 1;
    fn_8056FC24(0);
    lbl_8087F3C0->mSystem.mReplaying = 1;
    for (int frame = m5BDAC + 1; frame < end; frame++) {
        for (unsigned int i = 0; i < mEffectCount; i++) {
            ReplayEffectEntry_unk* e = mEffects[i];
            if (e->mFrame == frame) {
                lbl_8087F3C0->fn_8023B724(&e->mEvent);
            }
        }
        lbl_8087F3C0->fn_80239C14();
        lbl_8087F3C0->fn_80239C20();
    }
    fn_80232B7C(0, 0);
    atn::EffectManager::fn_8023B71C(lbl_8087F3C0, 0);
    lbl_8087F3C0->mSystem.mReplaying = 0;
    m5BDA4 = 1;
    for (int frame = m5BDAC + 1; frame < end; frame++) {
        for (unsigned int i = 0; i < mNewEvents.size(); i++) {
            ReplayEventEntry_unk* e = mNewEvents.mData[i];
            if (e->mFrame == frame) {
                ReplayEvent_unk& ev = e->mEvent;
                switch (ev.mKind) {
                case 0:
                    if (ev.mOwner != 0) {
                        ev.mBullet->fn_8010EE78((BulletShot_unk*)&ev.mShot, &ev.mPos, &ev.mDir, ev.mOwner, -1, 0, -1.0f);
                    } else if (ev.m3C != 0) {
                        ev.mBullet->fn_8010EFC8((BulletShot_unk*)&ev.mShot, &ev.mPos, &ev.mDir, ev.m3C, -1, 0, -1.0f);
                    }
                    break;
                case 1:
                    ev.mBullet->vf0C(1, 0);
                    break;
                case 2:
                    ev.mBullet->fn_8010F26C((BulletShot_unk*)&ev.mShot);
                    break;
                case 3:
                    ev.mBullet->fn_8010F41C();
                    break;
                }
            }
        }
        lbl_8087F048->fn_80101138();
    }
    m5BDA4 = 0;
}

void GameTaskData_unk::fn_805706C0(const ReplayEvent_unk* ev) {
    if (m5BDA4 != 0 || m5BDB0 != 0) {
        return;
    }
    ReplayEventEntry_unk* entry = 0;
    for (int i = 0; i < 256; i++) {
        int n = (mNextEvent + i) % 256;
        if (mEvents[n].mFrame < 0) {
            entry = &mEvents[n];
            mNextEvent = (n + 1) % 256;
            break;
        }
    }
    if (entry == 0) {
        return;
    }
    entry->mFrame = m00004;
    entry->mEvent = *ev;
    mNewEvents.push_back(entry);
}
