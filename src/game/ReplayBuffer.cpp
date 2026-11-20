#include "atn/RenderSetting_unk.h"
#include "game/ReplayBuffer.h"
#include "math/Vec.h"
#include "atn/NodeChunk.h"
#include "atn/ModelPose_unk.h"
#include "game/CharaTask.h"
#include "game/PlayerManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "atn/Model_unk.h"
#include "game/BattleTask.h"
#include "game/EnemyManager.h"
#include "game/FriendManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"
#include "atn/EffectManager.h"
#include "atn/EffectApi_unk.h"
#include "atn/DebugMessages_unk.h"
#include "atn/EfpHandle_unk.h"

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
// (the angle comes first: RTTI name of the bound functor at 0x8075FE78)
extern "C" void fn_8056A84C(float angle, atn::Matrix3x4* mtx, const atn::NodeChunk& node,
                            const atn::ModelNodeCallbackContext& ctx) {
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

#define REPLAY_MIN(a, b) ((a) < (b) ? (a) : (b))
#define REPLAY_MAX(a, b) ((a) > (b) ? (a) : (b))

// inferred: blend of the motion tracks given to fn_8056A9E4 (halfway between two frames of the replay; names
// invented)
struct ReplayBlend_unk {
    int mBlend; // inferred: 1 to use the frames and weights below
    float mFrames[4];
    float mWeights[4];
};

// inferred: sets the node callback / the second node pose callback of a model (game/BattleTask.cpp,
// game/Em401_01_WhiteLeoPard.cpp)
extern "C" void fn_800F29D0(atn::Model* model, atn::ModelNodeCallback_unk callback);
extern "C" void fn_8031F2A4(atn::Model* model, boost::function<void(atn::MotionBuffer*, int)> callback);

// inferred: sets a vector (the arguments are evaluated z first, as in game/CharaTask.cpp)
static inline void Vec3Set_unk(atn::Vector3& v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}

// inferred: byte of the replay data to 0..1 (name invented)
static inline float FromByte_unk(unsigned char b) {
    return b / 255.0f;
}

// inferred: half float of the replay data to float (names invented)
static inline float FromHalf_unk(short h) {
    if (h == 0) {
        return 0.0f;
    }
    unsigned int bits = (h << 16) & 0x80000000;
    bits |= (h << 13) & 0x007FE000;
    bits |= ((((h >> 10) & 0x1F) + 0x70) << 23) & 0x7F800000;
    return *(float*)&bits;
}

// inferred: poses `model` from the recorded data: matrix `mtx`, the `count` motion tracks of `tracks` (frames and
// weights from `blend` when it is set), the pose of the "reference" node and the upper body turned by `angle`;
// the callbacks of the model are kept and put back
// NONMATCHING: ~99% - the z of the three columns are loaded after the matrix copy (the original loads them
// between the paired-single copies), the store of the flag of the track and the registers of the second loop
extern "C" __declspec(noinline) void fn_8056A9E4(atn::Model* model, ReplayTrack_unk* tracks, int count, Mtx mtx,
                                                 ReplayBlend_unk* blend, ReplayChara_unk* entry, float angle) {
    atn::ModelNodeCallback_unk nodeCallback = model->mB8;
    boost::function<void(atn::MotionBuffer*, int)> poseCallback = model->m35C;
    if (entry != 0 && entry->m07 != 0) {
        fn_800F29D0(model, boost::bind(&fn_8056A84C, angle, _1, _2, _3));
    } else {
        fn_800F29D0(model, atn::ModelNodeCallback_unk());
    }
    fn_8031F2A4(model, boost::function<void(atn::MotionBuffer*, int)>());
    __MtxCopy(model->mMtx.m, mtx);
    atn::Vector3 ax;
    atn::Vector3 ay;
    atn::Vector3 az;
    Vec3Set_unk(az, mtx[0][2], mtx[1][2], mtx[2][2]);
    float sz = PSVECMag(&az);
    Vec3Set_unk(ay, mtx[0][1], mtx[1][1], mtx[2][1]);
    float sy = PSVECMag(&ay);
    Vec3Set_unk(ax, mtx[0][0], mtx[1][0], mtx[2][0]);
    float sx = PSVECMag(&ax);
    atn::Vector3 scale(sx, sy, sz);
    model->m54 = REPLAY_MAX(scale.x, REPLAY_MAX(scale.y, scale.z));
    ReplayTrack_unk* src = tracks;
    for (int i = 0; i < count; i++, src++) {
        atn::Model::Track_unk t;
        t.m00 = src->mId;
        t.m04 = src->mMotion;
        t.m08 = FromHalf_unk(src->mFrame);
        t.m0C = 0.0f;
        t.m10 = FromByte_unk(src->m11);
        t.m14 = 0.0f;
        t.m18 = src->mFlag != 0;
        t.m1C = FromByte_unk(src->m13);
        t.m20 = FromByte_unk(src->m14);
        t.m24 = src->mPrev;
        t.m28 = FromHalf_unk(src->mPrevFrame);
        t.m2C = FromByte_unk(src->m15);
        if (blend->mBlend != 0) {
            t.m08 = blend->mFrames[i];
            t.m10 = blend->mWeights[i];
        }
        model->mTracks[i] = t;
    }
    model->mFlags &= ~0x40;
    model->fn_80097E80(1);
    if (entry != 0) {
        atn::Model::Node_unk* nodes = model->m21C.mData;
        atn::Model::Node_unk* node = &nodes[model->GetNodeNoByName("reference", 0)];
        node->m00 = (unsigned short)entry->mRefFlags;
        atn::Vector3 pos(FromHalf_unk(entry->mRefPos[0]), FromHalf_unk(entry->mRefPos[1]), FromHalf_unk(entry->mRefPos[2]));
        node->mPos = pos;
    }
    model->CalcMatrixStack(atn::ModelNodeCallback_unk());
    src = tracks;
    for (int i = 0; i < count; i++, src++) {
        model->mTracks[i].m0C = FromHalf_unk(src->mSpeed);
        model->mTracks[i].m14 = FromByte_unk(src->mWeight);
    }
    fn_800F29D0(model, nodeCallback);
    fn_8031F2A4(model, poseCallback);
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


// NONMATCHING: 97.15% - stack frame 0x20 smaller (slots of the temporaries) and registers
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

// inferred: 0..1 to a byte (names invented)
static inline unsigned char ToByte_unk(float v) {
    return (unsigned char)(int)REPLAY_MAX(0.0f, REPLAY_MIN(255.0f, 255.0f * v));
}

extern "C" void fn_8056BAA4(ReplayChara_unk* dst, atn::Model* model);

// inferred: vector to three half floats (name invented)
static inline void ToHalf3_unk(short* dst, const atn::Vector3& v) {
    dst[0] = ToHalf_unk(v.x);
    dst[1] = ToHalf_unk(v.y);
    dst[2] = ToHalf_unk(v.z);
}
// inferred: records one character in mCharas: matrix relative to the leader, motion tracks, pose of the
// "reference" node and the angle of the upper body
void ReplayRecorder_unk::fn_8056B558(CharaTask* chara, const atn::Matrix3x4& inv) {
    if (m460 < 8 && chara->IsFlag_unk(2)) {
        atn::Model* model = &chara->mMotion;
        ReplayChara_unk* entry = &mCharas[m460];
        entry->mChara = chara;
        entry->mNode = model->m50;
        entry->mMotion = chara->m674;
        entry->m06 = model->m34C;
        Mtx mtx;
        PSMTXConcat(inv.m, model->mMtx.m, mtx);
        for (unsigned int j = 0; j < 12; j++) {
            unsigned int r = j >> 2;
            unsigned int c = j & 3;
            entry->mMtx[r][c] = ToHalf_unk(mtx[r][c]);
        }
        fn_8056BAA4(entry, model);
        atn::Model::Node_unk* nodes = model->m21C.mData;
        atn::Model::Node_unk* node = &nodes[model->GetNodeNoByName("reference", 0)];
        ToHalf3_unk(entry->mRefPos, node->mPos);
        entry->mRefFlags = node->m00;
        atn::Vector3 rot = chara->mFA4.ToRotation_unk();
        entry->mAngle = ToHalf_unk(atn::WrapAngle_unk(rot.y - chara->mRot.y));
        entry->m07 = chara->HasC48_unk();
        m460++;
    }
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
extern "C" __declspec(noinline) void fn_8056D60C(ReplayModel_unk* dst, atn::Model* model);

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
            unsigned int r = j >> 2;
            unsigned int c = j & 3;
            mtx[r][c] = FromHalf_unk(entry->mMtx[r][c]);
        }
        float angle = FromHalf_unk(entry->mAngle);
        ReplayBlend_unk out;
        out.mBlend = 0;
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

void fn_8056CB20(ReplayFrame_unk* frame, CharaTask* chara, const atn::Matrix3x4& inv);
// inferred: homing bullet of a replay frame: position and direction angles (half floats) (0x14 bytes; names
// invented)
struct ReplayBullet_unk {
    atn::Vector3 mPos;
    short mRot[3];
};
// inferred: bullet `i` of a frame (name invented; see ReplayFrame_unk::mBullets)
static inline ReplayBullet_unk* GetBullet_unk(ReplayFrame_unk* frame, int i) {
    return &((ReplayBullet_unk*)frame->mBullets)[i];
}
extern "C" RenderSetting_unk* lbl_8087EFA8;
inline float RenderSetting_unk::GetFrameSpeed_unk() const {
    return m3A4;
}

// NONMATCHING: 98.72% - registers: the frame pointer (r25 in the original) and the flag word/result of
// IsSkipped_unk in the enemy loop (as in ReplayRecorder_unk::fn_8056B3D4)
// inferred: records a frame every frame time: the party relative to the leader, the friends, the enemies, the
// gimmick models and the homing bullets
void ReplayBuffer::fn_8056C3D8() {
    m5BDB8 += lbl_8087EFA8->GetFrameSpeed_unk();
    if (m5BDB8 < 0.9999f) {
        return;
    }
    m5BDB8 -= 1.0f;
    if (m5BDA8 == 0) {
        if (m45180 != 0) {
            ReplayFrame_unk* frame = &mFrames[m45178];
            atn::Matrix3x4 inv;
            atn::Matrix3x4 trans;
            PlayerTask* leader = lbl_8087F8A0->mHead;
            PSMTXTrans(trans.m, leader->mPos.x, leader->mPos.y, leader->mPos.z);
            frame->mLeaderMtx = trans;
            inv = frame->mLeaderMtx;
            PSMTXInverse(inv.m, inv.m);
            fn_8056CB20(frame, lbl_8087F8A0->mHead, inv);
            for (CharaTask* chara = FriendManager::sInstance->fn_8036554C(); chara != 0;
                 chara = ((PlayerTask*)chara)->mNext) {
                if (!chara->IsFlag_unk(4) && !(chara->mType == 2 && IsSkipped_unk(chara))) {
                    fn_8056CB20(frame, chara, inv);
                }
            }
            for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
                if (!enemy->IsFlag_unk(4) && !(enemy->mType == 2 && IsSkipped_unk(enemy))) {
                    fn_8056CB20(frame, enemy, inv);
                }
            }
            fn_8056D43C(frame, inv.m);
            for (int i = 0; i < 32; i++) {
                HomingBullet* bullet = &lbl_8087F048->mHomings[i];
                ReplayBullet_unk* entry = GetBullet_unk(frame, i);
                entry->mPos = bullet->m08;
                if (bullet->m20.IsZero_unk()) {
                    ToHalf3_unk(entry->mRot, atn::Vector3(0.0f, 0.0f, 0.0f));
                } else {
                    ToHalf3_unk(entry->mRot, bullet->m20.GetNormalize().ToRotation_unk());
                }
            }
            m45178 = (m45178 + 1) % 45;
            mFrames[m45178].mNumCharas = 0;
            mFrames[m45178].mNumModels = 0;
            if (m4517C < 45) {
                m4517C++;
            }
        }
        m45180 = m45180 == 0;
    }
    m00004++;
    if (m5BDA8 == 0) {
        fn_8056D06C();
        fn_8057080C();
        m5BDAC = m00004;
    }
}

// inferred: records one character in the frame (same code as ReplayRecorder_unk::fn_8056B558; a free function:
// a new member in ReplayBuffer.h renumbers the @N labels of the units that include it)
void fn_8056CB20(ReplayFrame_unk* frame, CharaTask* chara, const atn::Matrix3x4& inv) {
    if (frame->mNumCharas < 15 && chara->IsFlag_unk(2)) {
        atn::Model* model = &chara->mMotion;
        ReplayChara_unk* entry = &frame->mCharas[frame->mNumCharas];
        entry->mChara = chara;
        entry->mNode = model->m50;
        entry->mMotion = chara->m674;
        entry->m06 = model->m34C;
        Mtx mtx;
        PSMTXConcat(inv.m, model->mMtx.m, mtx);
        for (unsigned int j = 0; j < 12; j++) {
            unsigned int r = j >> 2;
            unsigned int c = j & 3;
            entry->mMtx[r][c] = ToHalf_unk(mtx[r][c]);
        }
        fn_8056BAA4(entry, model);
        atn::Model::Node_unk* nodes = model->m21C.mData;
        atn::Model::Node_unk* node = &nodes[model->GetNodeNoByName("reference", 0)];
        ToHalf3_unk(entry->mRefPos, node->mPos);
        entry->mRefFlags = node->m00;
        atn::Vector3 rot = chara->mFA4.ToRotation_unk();
        entry->mAngle = ToHalf_unk(atn::WrapAngle_unk(rot.y - chara->mRot.y));
        entry->m07 = chara->HasC48_unk();
        frame->mNumCharas++;
    }
}


extern "C" int fn_8023779C(void* res);           // inferred: length of the effect (0: endless)
extern "C" int fn_80237784(void* res, int frame); // inferred: frames left of the effect after `frame`


// inferred: group test of the effects (-2: any group); name invented
static inline bool IsGroup_unk(int group, int want) {
    bool result = true;
    if (group != want && want != -2) {
        result = false;
    }
    return result;
}

// inferred: the stop event `stop` is for the effect started by `play` (as EffectManager::IsMatch_unk)
static inline bool IsSameEffect_unk(const EffectEvent_unk& stop, const EffectEvent_unk& play) {
    if (stop.mRes != 0) {
        return play.mRes == stop.mRes;
    }
    if (stop.m58 != 0) {
        return play.m58 == stop.m58;
    }
    if (stop.mHandle != 0) {
        return play.mHandle == stop.mHandle;
    }
    bool result = false;
    if (play.mOwner == stop.mOwner && IsGroup_unk(play.mGroup, stop.mGroup)) {
        result = true;
    }
    return result;
}


void ReplayBuffer::fn_8056D06C() {
    atn::FixedArray_unk<ReplayEffectEntry_unk*, 512> plays;
    ReplayEffectEntry_unk** it = mEffects.begin();
    while (it != mEffects.end()) {
        ReplayEffectEntry_unk* e = *it;
        switch (e->mEvent.mType) {
        case 0:
            if (fn_8023779C(e->mEvent.mRes) != 0 && e->mFrame + fn_8023779C(e->mEvent.mRes) < m00004 - 90) {
                e->mFrame = -1;
                mEffects.erase(it);
                continue;
            }
            plays.push_back(e);
            break;
        case 1:
            if (e->mFrame < m00004 - 90) {
                bool unmatched = true;
                ReplayEffectEntry_unk** p = plays.begin();
                while (p != plays.end()) {
                    ReplayEffectEntry_unk* play = *p;
                    if (play->mFrame < 0) {
                        continue;
                    }
                    if (IsSameEffect_unk(e->mEvent, play->mEvent)) {
                        unmatched = false;
                        if (e->mEvent.m5C == 0 ||
                            play->mFrame + fn_80237784(play->mEvent.mRes, e->mFrame - play->mFrame) < m00004 - 90) {
                            plays.erase(p);
                            play->mFrame = -1;
                            continue;
                        }
                    }
                    p++;
                }
                if (unmatched) {
                    e->mFrame = -1;
                }
            }
            break;
        }
        it++;
    }
    m52990 = m00004;
    ReplayEffectEntry_unk** it2 = mEffects.begin();
    while (it2 != mEffects.end()) {
        ReplayEffectEntry_unk* e = *it2;
        if (e->mFrame < 0) {
            mEffects.erase(it2);
            continue;
        }
        if (e->mEvent.mType == 0) {
            if (fn_8023779C(e->mEvent.mRes) == 0) {
                int oldest = m00004 - 60;
                m52990 = REPLAY_MIN(REPLAY_MAX(e->mFrame, oldest), m52990);
            } else {
                m52990 = REPLAY_MIN(e->mFrame, m52990);
            }
        }
        it2++;
    }
    if (m00004 - m52990 > 300) {
        m52990 = m00004 - 300;
    }
}

void ReplayBuffer::fn_8056D43C(ReplayFrame_unk* frame, const Mtx inv) {
    for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick != 0; gimmick = gimmick->m5C) {
        if (frame->mNumModels >= 60) {
            break;
        }
        atn::Model* model = gimmick->vf38();
        if (model != 0 && gimmick->m9C != 0) {
            ReplayModel_unk* entry = &frame->mModels[frame->mNumModels];
            entry->mOwner = gimmick;
            entry->mModel = model;
            frame->mNumModels++;
            Mtx mtx;
            PSMTXConcat(inv, model->mMtx.m, mtx);
            for (unsigned int j = 0; j < 12; j++) {
                unsigned int r = j >> 2;
                unsigned int c = j & 3;
                entry->mMtx[r][c] = ToHalf_unk(mtx[r][c]);
            }
            fn_8056D60C(entry, model);
        }
    }
}

// inferred: packs the motion track 0 of a gimmick model
// NONMATCHING: 78% - the original keeps copies of 255.0f/0.0f in f3-f12 (ToByte_unk written differently)
extern "C" __declspec(noinline) void fn_8056D60C(ReplayModel_unk* dst, atn::Model* model) {
    atn::Model::Track_unk& t = model->mTracks[0];
    dst->mTrack.mId = t.m00;
    dst->mTrack.mMotion = t.m04;
    dst->mTrack.mFrame = ToHalf_unk(t.m08);
    dst->mTrack.mSpeed = ToHalf_unk(t.m0C);
    dst->mTrack.mFlag = t.m18;
    dst->mTrack.m11 = ToByte_unk(t.m10);
    dst->mTrack.mWeight = ToByte_unk(t.m14);
    dst->mTrack.m13 = ToByte_unk(t.m1C);
    dst->mTrack.m14 = ToByte_unk(t.m20);
    dst->mTrack.mPrev = t.m24;
    dst->mTrack.mPrevFrame = ToHalf_unk(t.m28);
    dst->mTrack.m15 = ToByte_unk(t.m2C);
}

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

// NONMATCHING: 94% - stack slots of the matrices and blends (the original puts the ones of the blended path
// highest), registers, and the order of two float products by 0.5
// inferred: puts the frame `frame` / 2 frames back: the characters and the gimmick models; an even frame is
// halfway to the next one (the matrices, the upper body angle and the tracks of the same motion are averaged)
void ReplayBuffer::fn_8056DACC(int frame) {
    int back = frame / 2;
    int even = frame % 2 == 0;
    int oldest = 1 - m4517C;
    int b = -1 < (back > oldest ? back : oldest) ? -1 : (back > oldest ? back : oldest);
    int idx = m45178 + b;
    if (idx < 0) {
        idx += 45;
    }
    if (!even || b == 0) {
        ReplayFrame_unk* cur = &mFrames[idx];
        int i = 0;
        if (cur->mNumCharas > 0) {
        for (; i < cur->mNumCharas; i++) {
            ReplayChara_unk* entry = &cur->mCharas[i];
            atn::Model* model = &entry->mChara->mMotion;
            int node = entry->mNode;
            model->m20C_15_8 = model->m50;
            model->m50 = node;
            model->m34C = entry->m06;
            Mtx mtx;
            for (unsigned int j = 0; j < 12; j++) {
                unsigned int r = j >> 2;
                unsigned int c = j & 3;
                mtx[r][c] = FromHalf_unk(entry->mMtx[r][c]);
            }
            float angle = FromHalf_unk(entry->mAngle);
            ReplayBlend_unk blend;
            blend.mBlend = 0;
            Mtx world;
            PSMTXConcat(cur->mLeaderMtx.m, mtx, world);
            fn_8056A9E4(model, entry->mTracks, 4, world, &blend, entry, angle);
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
        for (int i = 0; i < cur->mNumModels; i++) {
            ReplayModel_unk* entry = &cur->mModels[i];
            atn::Model* model = entry->mModel;
            Mtx mtx;
            for (unsigned int j = 0; j < 12; j++) {
                unsigned int r = j >> 2;
                unsigned int c = j & 3;
                mtx[r][c] = FromHalf_unk(entry->mMtx[r][c]);
            }
            ReplayBlend_unk blend;
            blend.mBlend = 0;
            Mtx world;
            PSMTXConcat(cur->mLeaderMtx.m, mtx, world);
            fn_8056A9E4(model, &entry->mTrack, 1, world, &blend, 0, 0.0f);
        }
        return;
    }
    ReplayFrame_unk* cur = &mFrames[idx];
    ReplayFrame_unk* next = &mFrames[(idx + 1) % 45];
    for (int i = 0; i < cur->mNumCharas; i++) {
        ReplayChara_unk* entry = &cur->mCharas[i];
        CharaTask* chara = entry->mChara;
        atn::Model* model = &chara->mMotion;
        ReplayChara_unk* other = 0;
        ReplayChara_unk* p = next->mCharas;
        for (int j = 0; j < next->mNumCharas; j++, p++) {
            if (p->mChara == chara) {
                other = p;
                break;
            }
        }
        Mtx mtx;
        for (unsigned int j = 0; j < 12; j++) {
            unsigned int r = j >> 2;
            unsigned int c = j & 3;
            mtx[r][c] = FromHalf_unk(entry->mMtx[r][c]);
        }
        Mtx world;
        PSMTXConcat(cur->mLeaderMtx.m, mtx, world);
        float angle = FromHalf_unk(entry->mAngle);
        ReplayBlend_unk blend;
        blend.mBlend = 0;
        if (other != 0) {
            Mtx mtx2;
            for (unsigned int j = 0; j < 12; j++) {
                unsigned int r = j >> 2;
                unsigned int c = j & 3;
                mtx2[r][c] = FromHalf_unk(other->mMtx[r][c]);
            }
            Mtx world2;
            PSMTXConcat(next->mLeaderMtx.m, mtx2, world2);
            world[0][0] = (world[0][0] + world2[0][0]) * 0.5f;
            world[0][1] = (world[0][1] + world2[0][1]) * 0.5f;
            world[0][2] = (world[0][2] + world2[0][2]) * 0.5f;
            world[0][3] = (world[0][3] + world2[0][3]) * 0.5f;
            world[1][0] = (world[1][0] + world2[1][0]) * 0.5f;
            world[1][1] = (world[1][1] + world2[1][1]) * 0.5f;
            world[1][2] = (world[1][2] + world2[1][2]) * 0.5f;
            world[1][3] = (world[1][3] + world2[1][3]) * 0.5f;
            world[2][0] = (world[2][0] + world2[2][0]) * 0.5f;
            world[2][1] = (world[2][1] + world2[2][1]) * 0.5f;
            world[2][2] = (world[2][2] + world2[2][2]) * 0.5f;
            world[2][3] = (world[2][3] + world2[2][3]) * 0.5f;
            angle = fmod(angle + (FromHalf_unk(other->mAngle) - angle) * 0.5f, 6.283185307179586);
            if (angle > 3.1415927f) {
                angle -= 6.2831855f;
            }
            if (angle < -3.1415927f) {
                angle += 6.2831855f;
            }
            blend.mBlend = 1;
            for (int k = 0; k < 4; k++) {
                ReplayTrack_unk& t = entry->mTracks[k];
                ReplayTrack_unk& o = other->mTracks[k];
                blend.mFrames[k] = FromHalf_unk(t.mFrame);
                blend.mWeights[k] = t.m11 / 255.0f;
                if (t.mMotion == o.mMotion) {
                    blend.mFrames[k] += FromHalf_unk(o.mFrame);
                    float weight = o.m11 / 255.0f;
                    blend.mFrames[k] *= 0.5f;
                    blend.mWeights[k] = (blend.mWeights[k] + weight) * 0.5f;
                }
            }
        }
        int node = entry->mNode;
        model->m20C_15_8 = model->m50;
        model->m50 = node;
        model->m34C = entry->m06;
        fn_8056A9E4(model, entry->mTracks, 4, world, &blend, entry, angle);
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
    for (int i = 0; i < cur->mNumModels; i++) {
        ReplayModel_unk* entry = &cur->mModels[i];
        atn::Model* model = entry->mModel;
        ReplayModel_unk* other = 0;
        ReplayModel_unk* p = next->mModels;
        for (int j = 0; j < next->mNumModels; j++, p++) {
            if (p->mModel == model) {
                other = p;
                break;
            }
        }
        Mtx mtx;
        for (unsigned int j = 0; j < 12; j++) {
            unsigned int r = j >> 2;
            unsigned int c = j & 3;
            mtx[r][c] = FromHalf_unk(entry->mMtx[r][c]);
        }
        Mtx world;
        PSMTXConcat(cur->mLeaderMtx.m, mtx, world);
        ReplayBlend_unk blend;
        blend.mBlend = 0;
        if (other != 0) {
            Mtx mtx2;
            for (unsigned int j = 0; j < 12; j++) {
                unsigned int r = j >> 2;
                unsigned int c = j & 3;
                mtx2[r][c] = FromHalf_unk(other->mMtx[r][c]);
            }
            Mtx world2;
            PSMTXConcat(next->mLeaderMtx.m, mtx2, world2);
            world[0][0] = (world[0][0] + world2[0][0]) * 0.5f;
            world[0][1] = (world[0][1] + world2[0][1]) * 0.5f;
            world[0][2] = (world[0][2] + world2[0][2]) * 0.5f;
            world[0][3] = (world[0][3] + world2[0][3]) * 0.5f;
            world[1][0] = (world[1][0] + world2[1][0]) * 0.5f;
            world[1][1] = (world[1][1] + world2[1][1]) * 0.5f;
            world[1][2] = (world[1][2] + world2[1][2]) * 0.5f;
            world[1][3] = (world[1][3] + world2[1][3]) * 0.5f;
            world[2][0] = (world[2][0] + world2[2][0]) * 0.5f;
            world[2][1] = (world[2][1] + world2[2][1]) * 0.5f;
            world[2][2] = (world[2][2] + world2[2][2]) * 0.5f;
            world[2][3] = (world[2][3] + world2[2][3]) * 0.5f;
            blend.mBlend = 1;
            blend.mFrames[0] = FromHalf_unk(entry->mTrack.mFrame);
            blend.mWeights[0] = entry->mTrack.m11 / 255.0f;
            if (entry->mTrack.mMotion == other->mTrack.mMotion) {
                blend.mFrames[0] += FromHalf_unk(other->mTrack.mFrame);
                float weight = other->mTrack.m11 / 255.0f;
                blend.mFrames[0] *= 0.5f;
                blend.mWeights[0] = (blend.mWeights[0] + weight) * 0.5f;
            }
        }
        fn_8056A9E4(model, &entry->mTrack, 1, world, &blend, 0, 0.0f);
    }
}

// inferred: world matrix of a recorded homing bullet (as in fn_8056FC24); name invented
static inline void BulletMtx_unk(ReplayBullet_unk* entry, Mtx trans) {
    PSMTXTrans(trans, entry->mPos.x, entry->mPos.y, entry->mPos.z);
    atn::Vector3 rot(FromHalf_unk(entry->mRot[0]), FromHalf_unk(entry->mRot[1]), FromHalf_unk(entry->mRot[2]));
    atn::Matrix3x4 m;
    m.SetIdentity();
    __MtxRotate(m.m, 'z', rot.z);
    __MtxRotate(m.m, 'y', rot.y);
    __MtxRotate(m.m, 'x', rot.x);
    Mtx world;
    PSMTXConcat(trans, m.m, world);
    __MtxCopy(trans, world);
}

// NONMATCHING: 88.9% - stack slots of the matrices of __MtxRotate (the original puts them in the opposite order),
// the addresses of the locals kept in registers, the address of mHomings[i] and the order of the products by 0.5
// inferred: puts the homing bullets of the frame `frame` / 2 frames back; an even frame is halfway to the next one
// (the matrices are averaged)
void ReplayBuffer::fn_8056E918(int frame) {
    int back = frame / 2;
    int even = frame % 2 == 0;
    int oldest = 1 - m4517C;
    int b = -1 < (back > oldest ? back : oldest) ? -1 : (back > oldest ? back : oldest);
    int idx = m45178 + b;
    if (idx < 0) {
        idx += 45;
    }
    if (!even || b >= -1) {
        ReplayFrame_unk* cur = &mFrames[idx];
        for (int i = 0; i < 32; i++) {
            Mtx trans;
            BulletMtx_unk(GetBullet_unk(cur, i), trans);
            lbl_8087F048->mHomings[i].vf14(trans);
        }
        return;
    }
    ReplayFrame_unk* cur = &mFrames[idx];
    ReplayFrame_unk* next = &mFrames[(idx + 1) % 45];
    for (int i = 0; i < 32; i++) {
        Mtx trans;
        BulletMtx_unk(GetBullet_unk(cur, i), trans);
        Mtx trans2;
        BulletMtx_unk(GetBullet_unk(next, i), trans2);
        Mtx mtx;
        mtx[0][0] = (trans[0][0] + trans2[0][0]) * 0.5f;
        mtx[0][1] = (trans[0][1] + trans2[0][1]) * 0.5f;
        mtx[0][2] = (trans[0][2] + trans2[0][2]) * 0.5f;
        mtx[0][3] = (trans[0][3] + trans2[0][3]) * 0.5f;
        mtx[1][0] = (trans[1][0] + trans2[1][0]) * 0.5f;
        mtx[1][1] = (trans[1][1] + trans2[1][1]) * 0.5f;
        mtx[1][2] = (trans[1][2] + trans2[1][2]) * 0.5f;
        mtx[1][3] = (trans[1][3] + trans2[1][3]) * 0.5f;
        mtx[2][0] = (trans[2][0] + trans2[2][0]) * 0.5f;
        mtx[2][1] = (trans[2][1] + trans2[2][1]) * 0.5f;
        mtx[2][2] = (trans[2][2] + trans2[2][2]) * 0.5f;
        mtx[2][3] = (trans[2][3] + trans2[2][3]) * 0.5f;
        lbl_8087F048->mHomings[i].vf14(mtx);
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

// NONMATCHING: 98.65% - same registers as fn_8056C3D8 (IsSkipped_unk in the enemy loop) and the stack slots
// of the bullet loop
// inferred: records the live frame mLive (the party relative to the leader, the friends and the enemies; the
// gimmick models and the homing bullets unless `charasOnly`)
void ReplayBuffer::fn_8056F5B4(int charasOnly) {
    mLive.mNumCharas = 0;
    mLive.mNumModels = 0;
    atn::Matrix3x4 inv;
    atn::Matrix3x4 trans;
    PlayerTask* leader = lbl_8087F8A0->mHead;
    PSMTXTrans(trans.m, leader->mPos.x, leader->mPos.y, leader->mPos.z);
    mLive.mLeaderMtx = trans;
    inv = mLive.mLeaderMtx;
    PSMTXInverse(inv.m, inv.m);
    fn_8056CB20(&mLive, lbl_8087F8A0->mHead, inv);
    for (CharaTask* chara = FriendManager::sInstance->fn_8036554C(); chara != 0;
         chara = ((PlayerTask*)chara)->mNext) {
        if (!chara->IsFlag_unk(4) && !(chara->mType == 2 && IsSkipped_unk(chara))) {
            fn_8056CB20(&mLive, chara, inv);
        }
    }
    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
        if (!enemy->IsFlag_unk(4) && !(enemy->mType == 2 && IsSkipped_unk(enemy))) {
            fn_8056CB20(&mLive, enemy, inv);
        }
    }
    if (charasOnly == 0) {
        fn_8056D43C(&mLive, inv.m);
        ReplayFrame_unk* frame = &mLive;
        for (int i = 0; i < 32; i++) {
            HomingBullet* bullet = &lbl_8087F048->mHomings[i];
            ReplayBullet_unk* entry = GetBullet_unk(frame, i);
            entry->mPos = bullet->m08;
            if (bullet->m20.IsZero_unk()) {
                ToHalf3_unk(entry->mRot, atn::Vector3(0.0f, 0.0f, 0.0f));
            } else {
                ToHalf3_unk(entry->mRot, bullet->m20.GetNormalize().ToRotation_unk());
            }
        }
    }
}

// NONMATCHING: 96.63% - stack frame 0x40 smaller (slots of the matrices) and registers of the entry pointers
// inferred: puts the live frame mLive back: the characters (as ReplayRecorder_unk::fn_8056BD34) and, unless
// `charasOnly`, the gimmick models and the homing bullets
void ReplayBuffer::fn_8056FC24(int charasOnly) {
    ReplayFrame_unk* frame = &mLive;
    int i = 0;
    if (frame->mNumCharas > 0) {
    for (; i < frame->mNumCharas; i++) {
        ReplayChara_unk* entry = &frame->mCharas[i];
        atn::Model* model = &entry->mChara->mMotion;
        int node = entry->mNode;
        model->m20C_15_8 = model->m50;
        model->m50 = node;
        model->m34C = entry->m06;
        Mtx mtx;
        for (unsigned int j = 0; j < 12; j++) {
            unsigned int r = j >> 2;
            unsigned int c = j & 3;
            mtx[r][c] = FromHalf_unk(entry->mMtx[r][c]);
        }
        float angle = FromHalf_unk(entry->mAngle);
        ReplayBlend_unk out;
        out.mBlend = 0;
        Mtx world;
        PSMTXConcat(frame->mLeaderMtx.m, mtx, world);
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
    if (charasOnly != 0) {
        return;
    }
    for (int i = 0; i < frame->mNumModels; i++) {
        ReplayModel_unk* entry = &frame->mModels[i];
        atn::Model* model = entry->mModel;
        Mtx mtx;
        for (unsigned int j = 0; j < 12; j++) {
            unsigned int r = j >> 2;
            unsigned int c = j & 3;
            mtx[r][c] = FromHalf_unk(entry->mMtx[r][c]);
        }
        ReplayBlend_unk out;
        out.mBlend = 0;
        Mtx world;
        PSMTXConcat(frame->mLeaderMtx.m, mtx, world);
        fn_8056A9E4(model, &entry->mTrack, 1, world, &out, 0, 0.0f);
    }
    for (int i = 0; i < 32; i++) {
        ReplayBullet_unk* entry = GetBullet_unk(frame, i);
        Mtx trans;
        PSMTXTrans(trans, entry->mPos.x, entry->mPos.y, entry->mPos.z);
        atn::Vector3 rot(FromHalf_unk(entry->mRot[0]), FromHalf_unk(entry->mRot[1]), FromHalf_unk(entry->mRot[2]));
        atn::Matrix3x4 m;
        m.SetIdentity();
        __MtxRotate(m.m, 'z', rot.z);
        __MtxRotate(m.m, 'y', rot.y);
        __MtxRotate(m.m, 'x', rot.x);
        Mtx world;
        PSMTXConcat(trans, m.m, world);
        __MtxCopy(trans, world);
        lbl_8087F048->mHomings[i].vf14(trans);
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

void ReplayBuffer::fn_80570A14(int recording) {
    m5BDA8 = recording;
    if (recording != 0) {
        m5BDAC = m00004;
    } else {
        m4517C = 0;
    }
}

void ReplayBuffer::fn_80570A40(int pause) {
    m5BDB0 = pause;
}

int ReplayBuffer::fn_80570A4C() {
    return m4517C == 45;
}

void ReplayBuffer::fn_80570A64() {
    m4517C = 0;
}

// inferred: emitters of an efp resource (atn::EfpFile::mResource; 0x54 bytes each, the owner of the
// effects at 0x48); names invented
struct EfpEmitter_unk {
    unsigned char m00[0x48];
    unsigned char m48[0x54 - 0x48];
};
struct EfpRes_unk {
    unsigned int mCount;
    EfpEmitter_unk* mItems;
};

// inferred: removes the effect events of an emitter (name invented)
inline void RemoveEffects_unk(atn::FixedArray_unk<ReplayEffectEntry_unk*, 512>& effects, EfpEmitter_unk* emitter) {
    ReplayEffectEntry_unk** it = effects.begin();
    while (it != effects.end()) {
        if ((*it)->mEvent.mRes == emitter->m48) {
            effects.erase(it);
        } else {
            it++;
        }
    }
}

// inferred: removes the effect events of every emitter of `efp`
// NONMATCHING: 91.4% - volatile registers: the resource, the emitter offset and the erase pointer rotate
// (r10/r11/r12) and the original loads the resource before saving r31 (tried the declaration orders, an
// early return, the whole loop in an inline, the helper with a pointer or the owner)
void ReplayBuffer::fn_80570A74(atn::EfpFile* efp) {
    if (EfpRes_unk* res = (EfpRes_unk*)efp->mResource) {
        for (unsigned int i = 0; i < res->mCount; i++) {
            RemoveEffects_unk(mEffects, &res->mItems[i]);
        }
    }
}
