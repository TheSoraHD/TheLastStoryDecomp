#include "game/Em002_02SkeltonKing.h"
#include "game/EnemyManager.h"
#include "chr/ControlStickByQuark.h"

extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_800E2FE0(CharaTask* chara);       // inferred: BattleCharaTask update
// inferred: field (lbl_8087F430, as in game/CharaTask.cpp)
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads a counter
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets a counter

// NONMATCHING: 93.8% - the load of info->m20 is scheduled after `mr r30, r3` (original: first instruction)
Em002_02SkeltonKing::Em002_02SkeltonKing(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), mTarget(0), mLastTarget(0), mWalkTime(0) {
    m54C |= 0x2000000;
}

int Em002_02SkeltonKing::vf0C() {
    if (BattleCharaTask::vf0C()) {
        mStatus.m18 |= 0x100;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x50;
        MtxPtr neck = mMotion.GetNodeMtx_unk("neck");
        atn::Vector3 center = neck != 0 ? ((atn::Matrix3x4*)neck)->GetTrans_unk() : lbl_807C7030;
        mHead.mSphere.mCenter = center;
        mHead.mSphere.mRadius = 5.0f;
        mHead.mFlags = (mHead.mFlags & ~1) | 2;
        mHead.m0C = (int)this;
        m54C |= 0x1000000;
        return 1;
    }
    return 0;
}

// inferred: releases the grabbed skeleton (inlined four times; static, its literals are in the pool)
static void ReleaseTarget_unk(Em002_02SkeltonKing* king) {
    if (king->mTarget != 0) {
        king->mTarget->fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlThrownBreakSkelton(king->mTarget));
        king->mTarget = 0;
    }
}

void Em002_02SkeltonKing::vf10() {
    mHead.mFlags &= ~1;
    if (!mStatus.IsState_unk(0x20)) {
        mHead.mFlags |= 1;
    }
    switch (m58C) {
    case 6:
        fn_80249394();
        fn_80145334();
        fn_8014C540();
        return;
    case 7:
        fn_80249588();
        fn_80145334();
        fn_8014C540();
        return;
    }
    int update = 1;
    if (mC58.mC0 == 1 && m55C == 2 && mC58.mC4 != 0) {
        atn::Vector3 toEnemy = mC58.mC4->mPos - mPos;
        if (fn_805F9920(&toEnemy) > 400.0f) {
            CharaTask* target = fn_80249224();
            mTarget = target;
            if (target != 0) {
                atn::Vector3 toTarget = target->mPos - mPos;
                float dt = fn_805F9920(&toTarget);
                if (dt < fn_805F9920(&toEnemy)) {

                    m58C = 6;
                    fn_80170A20(mTarget, 0, -1.0f);
                    mWalkTime = 0;
                    mLastTarget = 0;
                    fn_80145334();
                    fn_8014C540();
                    update = 0;
                }
            }
        }
    }
    if (update) {
        ReleaseTarget_unk(this);
        fn_800E2FE0(this);
    }
}

CharaTask* Em002_02SkeltonKing::fn_80249224() {
    CharaTask* best = 0;
    float bestDist = 1e16f;
    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
        if (enemy->CanAct_unk() && enemy != this && !enemy->mStatus.IsState_unk(0x40) && mLastTarget != enemy &&
            enemy->m55C == 2) {
            atn::Vector3 d = enemy->mPos - mPos;
            float dist = fn_805F9920(&d);
            if (best == 0 || dist < bestDist) {
                best = enemy;
                bestDist = dist;
            }
        }
    }
    return best;
}

void Em002_02SkeltonKing::fn_80249394() {
    if (m55C == 7 && mTarget != 0 && mTarget->CanAct_unk() && mWalkTime < 300) {
        atn::Vector3 d = mTarget->mPos - mPos;
        if (fn_805F9920(&d) < 1225.0f) {
            m58C = 7;
            mMotion.fn_80097C08(0, 0x6C, 0, 0, 1, 0.0f, 0.1f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
        } else {
            mWalkTime++;
            fn_80139560();
        }
    } else {
        if (mWalkTime >= 300) {
            mLastTarget = mTarget;
        }
        m58C = 0;
        fn_8016EB48(0, 0, 0);
        ReleaseTarget_unk(this);
    }
}

inline atn::Vector3 Em002_02SkeltonKing::GetFront_unk(float dist) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', mRot.y);
    PSMTXMultVec(m, &front, &front);
    return mPos + front * dist;
}

// NONMATCHING: 98.6% - m55C of the target is loaded once (original: reloaded by CanAct_unk) and the stack slots
// of the rotation temporaries are 0x18 higher (GetFront_unk's locals are below them here). Reading mTarget
// directly (no local) fixed the target registers. Tried: if/else for `to`, GetFront_unk as static (inline or
// not), the first check as an inline, `!(x < 15)`.
void Em002_02SkeltonKing::fn_80249588() {
    int motion = mMotion.mTracks[0].m00;
    if (motion == 0x6C) {
        if (mTarget == 0) {
            m58C = 0;
            fn_8016EB48(0, 0, 0);
            ReleaseTarget_unk(this);
        } else if (!(mTarget->m55C == 6 && mTarget->m560 == 0x71) && mMotion.mTracks[0].m08 >= 15.0f) {
            if (mTarget->CanAct_unk()) {
                mTarget->fn_80178208(new ((MemoryTag)3, "", "", 0) chr::ControlCaughtSkelton(mTarget, this));
            } else {
                mTarget = 0;
                m58C = 0;
                fn_8016EB48(0, 0, 0);
            }
        }
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.fn_80097C08(0, 0x6F, 0, 0, 1, 0.0f, 0.1f);
        }
    } else if (motion == 0x6F) {
        CharaTask* enemy = mC58.mC4;
        if (enemy != 0) {
            atn::Vector3 dir(enemy->mPos.x - mPos.x, 0.0f, enemy->mPos.z - mPos.z);
            mRot = dir.ToRotation_unk();
        }
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.fn_80097C08(0, 0x70, 0, 0, 1, 0.0f, 0.1f);
        }
    } else if (motion == 0x70) {
        if (mMotion.mTracks[0].m08 >= 10.0f && mTarget != 0) {
            atn::Vector3 to = mC58.mC4 != 0 ? mC58.mC4->mPos : GetFront_unk(10.0f);
            to.y += 4.0f;
            if (mTarget != 0) {
                mTarget->fn_80178208(new ((MemoryTag)3, "", "", 0) chr::ControlThrownSkelton(mTarget, to, fn_80219E6C(0x69F)));
                mTarget = 0;
                if (fn_80370A78(lbl_8087F430, 0xE6) == 0) {
                    fn_80370AE4(lbl_8087F430, 0xE6, 1);
                }
            }
        }
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m58C = 0;
            fn_8016EB48(0, 0, 0);
        }
    } else {
        m58C = 0;
        fn_8016EB48(0, 0, 0);
        ReleaseTarget_unk(this);
    }
}

void Em002_02SkeltonKing::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    atn::Vector3 dir = msg->m10;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    vf40(hit);
    if (mStatus.IsState_unk(0x20)) {
        if (!(m55C == 6 && m560 == 0x17)) {
            fn_8015E7A0(dir, -1, 0);
        }
        vf44();
        ReleaseTarget_unk(this);
    }
}

// NONMATCHING: 99.9% - the copy of `a` swaps r6/r7 (the __VecCopy register family); m620 = m5B0 first fixed
// the float registers
void Em002_02SkeltonKing::vf20() {
    atn::Vector3 a;
    atn::Vector3 b;
    atn::Vector3 c;
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    MtxPtr left = mMotion.GetNodeMtx_unk("leftleg");
    MtxPtr right = mMotion.GetNodeMtx_unk("rightleg");
    a = (((atn::Matrix3x4*)left)->GetTrans_unk() + ((atn::Matrix3x4*)right)->GetTrans_unk()) * 0.5f;
    MtxPtr neck = mMotion.GetNodeMtx_unk("neck");
    b = ((atn::Matrix3x4*)neck)->GetTrans_unk();
    b.y -= 10.0f;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m620;
    mColli.mFlags &= ~0x80000000;
    c = neck != 0 ? ((atn::Matrix3x4*)neck)->GetTrans_unk() : b;
    mHead.mSphere.mCenter = c;
    mHead.mSphere.mRadius = 4.8f;
}

Em002_02SkeltonKing::~Em002_02SkeltonKing() {}
