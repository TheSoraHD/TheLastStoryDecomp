#include "game/ReplayBuffer.h"
#include "math/Vec.h"
#include "atn/NodeChunk.h"
#include "atn/ModelPose_unk.h"
#include "game/CharaTask.h"
#include "game/PlayerManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "atn/Model_unk.h"
#include "game/BattleTask.h"
#include "atn/EffectManager.h"
#include "atn/DebugMessages_unk.h"

// game/ReplayBuffer.cpp (0x8056A84C-0x80570A14): the replay of the field (records the characters and their
// effects). Partial: only some of the functions are written.

extern "C" int strcmp(const char* a, const char* b);
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party
extern "C" BattleTask* lbl_8087F048;    // inferred: battle manager
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);

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

// NONMATCHING: draft - only the node callback of the upper body; the pose of the nodes (`data`, `count`), the
// saved callbacks of the model and `out` are missing. Written for the weak copies of the boost::function of
// the callback (0x8056B2C0 invoker, 0x8056B2E0 manager).
extern "C" void fn_8056A9E4(atn::Model* model, void* data, int count, Mtx mtx, void** out, ReplayChara_unk* entry,
                            float angle) {
    if (entry != 0 && entry->m07 != 0) {
        model->CalcMatrixStack(boost::bind(&fn_8056A84C, _1, _2, _3, angle));
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

// inferred: half float of the replay data to float (names invented)
static inline float FromHalf_unk(short h) {
    if (h == 0) {
        return 0.0f;
    }
    unsigned int bits = ((h << 16) & 0x80000000) | ((h << 13) & 0x007FE000) | (((((h >> 10) & 0x1F) + 0x70) << 23) & 0x7F800000);
    return *(float*)&bits;
}


// NONMATCHING: 77.2% - unrolling of the matrix loop (the original does two elements per pass), stack slots
// inferred: float to half float of the replay data (names invented)
static inline short ToHalf_unk(float f) {
    unsigned int bits = *(unsigned int*)&f;
    if (bits == 0) {
        return 0;
    }
    int e = ((bits >> 23) & 0xFF) - 0x70;
    if (e < 0) {
        return 0;
    }
    if (e > 31) {
        e = 31;
    }
    return (short)(((bits >> 16) & 0x8000) | ((e << 10) & 0x7C00) | ((bits >> 13) & 0x3FF));
}

#define REPLAY_MIN(a, b) ((a) < (b) ? (a) : (b))
#define REPLAY_MAX(a, b) ((a) > (b) ? (a) : (b))
// inferred: 0..1 to a byte (names invented)
static inline unsigned char ToByte_unk(float v) {
    return (unsigned char)(int)REPLAY_MAX(0.0f, REPLAY_MIN(255.0f, 255.0f * v));
}

// inferred: packs the first four motion tracks of the model
extern "C" void fn_8056BAA4(ReplayChara_unk* dst, atn::Model* model) {
    for (int i = 0; i < 4; i++) {
        atn::Model::Track_unk& t = model->mTracks[i];
        dst->mTracks[i].mId = t.m00;
        dst->mTracks[i].mMotion = t.m04;
        dst->mTracks[i].mFrame = ToHalf_unk(t.m08);
        dst->mTracks[i].mSpeed = ToHalf_unk(t.m0C);
        dst->mTracks[i].mFlag = t.m18;
        dst->mTracks[i].m11 = ToByte_unk(t.m10);
        dst->mTracks[i].mWeight = ToByte_unk(t.m14);
        dst->mTracks[i].m13 = ToByte_unk(t.m1C);
        dst->mTracks[i].m14 = ToByte_unk(t.m20);
        dst->mTracks[i].mPrev = t.m24;
        dst->mTracks[i].mPrevFrame = ToHalf_unk(t.m28);
        dst->mTracks[i].m15 = ToByte_unk(t.m2C);
    }
}

void ReplayRecorder_unk::fn_8056BD34() {
    int i = 0;
    if (m460 > 0) {
    for (; i < m460; i++) {
        ReplayChara_unk* entry = &mCharas[i];
        atn::Model* model = &entry->mChara->mMotion;
        int node = entry->mNode;
        model->m20C_15_8 = model->m50;
        model->m50 = node;
        model->m34C = entry->m06;
        Mtx mtx;
        for (unsigned int j = 0; j < 12; j++) {
            mtx[j / 4][j % 4] = FromHalf_unk(entry->mMtx[j / 4][j % 4]);
        }
        float angle = FromHalf_unk(entry->mAngle);
        void* out = 0;
        Mtx world;
        PSMTXConcat(m464.m, mtx, world);
        fn_8056A9E4(model, entry->mTracks, 4, world, &out, entry, angle);
        atn::Vector3 pos(model->mMtx.m[0][3], model->mMtx.m[1][3], model->mMtx.m[2][3]);
        atn::Vector3 d = atn::RenderManager::instance_->mCamera.mPos - pos;
        float dist = d.LengthSq_unk();
        if (entry->mMotion >= 0) {
            entry->mChara->fn_8014DEE4(entry->mMotion, 1, 0);
        } else {
            entry->mChara->fn_8014EEC4(1);
        }
        entry->mChara->fn_80148B38(dist);
    }
    }
}

ReplayBuffer::ReplayBuffer()
    : m00004(0), m45178(0), m4517C(0), m45180(1), mNextEffect(0), m5298C(0), m52990(0), mNextEvent(0), m5BD9C(0),
      m5BDA0(0), m5BDA4(0), m5BDA8(0), m5BDAC(0), m5BDB0(0), m5BDB4(0), m5BDB8(0.0f) {}

ReplayBuffer::~ReplayBuffer() {}

// NONMATCHING: 92.48% - copy order of the event (colour and vectors); offset of the message in the pool
// ("reference" of fn_8056A9E4 missing)
// NONMATCHING: 99.07% - registers of the loop pointers/counters (r24/r26 swapped)
void ReplayBuffer::fn_8056D8C8(int frame, int restore, int replay) {
    if (replay != 0) {
        int f = m5BDAC + frame;
        lbl_8087F3C0->mSystem.mReplaying = 1;
        for (unsigned int i = 0; i < mEffects.mSize; i++) {
            ReplayEffectEntry_unk* e = mEffects.mData[i];
            if (e->mFrame == f) {
                lbl_8087F3C0->fn_8023B724(&e->mEvent);
            }
        }
        fn_80232B7C(0, 0);
        atn::EffectManager::fn_8023B71C(lbl_8087F3C0, 0);
        lbl_8087F3C0->mSystem.mReplaying = 0;
        m5BDA4 = 1;
        int g = m5BDAC + frame;
        for (unsigned int i = 0; i < mNewEvents.size(); i++) {
            ReplayEventEntry_unk* e = mNewEvents.mData[i];
            if (e->mFrame == g) {
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
        m5BDA4 = 0;
        fn_8056E918(frame);
    }
    if (restore != 0) {
        fn_8056DACC(frame);
    }
    if (replay != 0) {
        lbl_8087F048->fn_80101138();
    }
}

// inferred: index of the frame `frames` / 2 frames back
void ReplayBuffer::fn_8056F2F8(int frames) {
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

void ReplayBuffer::vf08(EffectEvent_unk* event) {
    if (m5BDB0 != 0) {
        return;
    }
    if ((unsigned int)(event->mLayer - 3) <= 2 || event->mLayer == 1) {
        return;
    }
    ReplayEffectEntry_unk* entry = 0;
    for (int i = 0; i < 512; i++) {
        int n = (mNextEffect + i) % 512;
        if (mEffectRing[n].mFrame < 0) {
            entry = &mEffectRing[n];
            mNextEffect = (n + 1) % 512;
            break;
        }
    }
    if (entry != 0) {
        entry->mFrame = m00004;
        entry->mEvent = *event;
        mEffects.push_back(entry);
    } else {
        lbl_8087EEB8->fn_800697D8("[Replay] eff buffer over!");
    }
}

void ReplayBuffer::fn_80570254(int frame) {
    m5BDB4 = 0;
    fn_8056DACC(frame);
    int end = m5BDAC + frame;
    lbl_8087F3C0->mSystem.mReplaying = 1;
    for (unsigned int i = 0; i < mEffects.mSize; i++) {
        ReplayEffectEntry_unk* e = mEffects.mData[i];
        if (e->mFrame < m52990) {
            lbl_8087F3C0->fn_8023B724(&e->mEvent);
        }
    }
    for (int f = m52990; f < end; f++) {
        for (unsigned int i = 0; i < mEffects.mSize; i++) {
            ReplayEffectEntry_unk* e = mEffects.mData[i];
            if (e->mFrame == f) {
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
    for (int f = m5BDA0; f < end; f++) {
        for (unsigned int i = 0; i < mNewEvents.size(); i++) {
            ReplayEventEntry_unk* e = mNewEvents.mData[i];
            if (e->mFrame == f) {
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

void ReplayBuffer::fn_805704AC() {
    m5BDB4 = 1;
    int end = m00004 + 1;
    fn_8056FC24(0);
    lbl_8087F3C0->mSystem.mReplaying = 1;
    for (int frame = m5BDAC + 1; frame < end; frame++) {
        for (unsigned int i = 0; i < mEffects.mSize; i++) {
            ReplayEffectEntry_unk* e = mEffects.mData[i];
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

void ReplayBuffer::fn_805706C0(const ReplayEvent_unk* ev) {
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

void ReplayBuffer::fn_8057080C() {
    atn::FixedArray_unk<ReplayEventEntry_unk*, 256> shots;
    for (ReplayEventEntry_unk** it = mNewEvents.begin(); it != mNewEvents.end(); it++) {
        ReplayEventEntry_unk* e = *it;
        switch (e->mEvent.mKind) {
        case 0:
        case 2:
            shots.push_back(e);
            break;
        case 1:
        case 3:
            if (e->mFrame < m00004 - 90) {
                if (shots.size() == 0) {
                    e->mFrame = -1;
                } else {
                    // inferred: as in the original, a freed shot (negative frame) is not skipped
                    ReplayEventEntry_unk** p = shots.begin();
                    while (p != shots.end()) {
                        ReplayEventEntry_unk* shot = *p;
                        if (shot->mFrame < 0) {
                            continue;
                        }
                        if (e->mEvent.mBullet == shot->mEvent.mBullet) {
                            shots.erase(p);
                            e->mFrame = -1;
                            shot->mFrame = -1;
                            break;
                        }
                        p++;
                    }
                }
            }
            break;
        }
    }
    m5BDA0 = m00004;
    for (ReplayEventEntry_unk** it = mNewEvents.begin(); it != mNewEvents.end();) {
        ReplayEventEntry_unk* e = *it;
        if (e->mFrame < 0) {
            mNewEvents.erase(it);
        } else {
            if (e->mEvent.mKind == 0 || e->mEvent.mKind == 2) {
                m5BDA0 = e->mFrame < m5BDA0 ? e->mFrame : m5BDA0;
            }
            it++;
        }
    }
}
