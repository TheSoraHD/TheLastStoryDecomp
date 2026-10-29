#include "gmk/Augur.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/PlayerTask.h"

extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" int rand();
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: route tables of the field (class unknown)
struct AugurRoutes_unk {
    unsigned char m00[0xA4];
    unsigned char* mA4; // inferred: 8-byte entries, one per route point
};
struct Field_unk {
    unsigned char m00[0x10D8];
    AugurRoutes_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_803C1560(AugurRoutes_unk* routes, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float radius); // inferred: nearest route point
extern "C" unsigned short fn_803CD958(void* entry, int point); // inferred: the two points are linked
// inferred: camera (class unknown)
struct AugurCamera_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mEye; // 0x10C
};
extern "C" AugurCamera_unk* lbl_8087EFB4;

// inferred: names invented
inline float LenSq_unk(atn::Vector3 v) {
    return fn_805F9920(&v);
}
inline void SetPos_unk(CharaTask* chara, const atn::Vector3& pos) { chara->mPos = pos; }
inline void SetRot_unk(CharaTask* chara, const atn::Vector3& rot) { chara->mRot = rot; }
inline void SetSpeed_unk(CharaTask* chara) {
    chara->m580 = 0.0f;
    chara->m584 = 0.0f;
}
inline void PlayMotion_unk(Model_unk* model, int motion, int arg3, int arg4) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = 1.0f;
    model->fn_80097C08(0, motion, arg3, arg4, 1, 0.0f, 0.1f);
}

namespace gmk {

inline void Augur::ReleaseTarget_unk() {
    mTarget->fn_8015495C(0, 1, 1, 1);
    mTarget->fn_8016EB48(0, 0, 0);
    if (mTarget->IsPartyMember_unk()) {
        mTarget->mC58.mHeld_unk = 0;
        mTarget->m137C &= ~1;
        mTarget->m54C &= ~0x1000000;
    }
}

GimmickCommon* Augur::fn_8041EB80(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) Augur(parent, desc);
    }
    return 0;
}

Augur::Augur(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mTimer(0), mAugur(0), mTarget(0), mLast(0),
      mLastId(0), m148(0) {
    m54 = 0;
    m68 = 1;
}

int Augur::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 2;
        mState = 2;
        return 1;
    }
    return 0;
}

void Augur::Start() {
    GimmickDesc_unk* desc = mDesc;
    m6C = desc->mPos;
    m78.y = desc->mRotY;
    m78.x = 0.0f;
    m78.z = 0.0f;
    atn::Vector3 front(-7.0f, 0.0f, 6.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', m78.y);
    PSMTXMultVec(mtx, &front, &front);
    Field_unk* field = lbl_8087F430;
    AugurRoutes_unk* routes = field->m10D8;
    mFront = m6C + front;
    mFrontRot = m78;
    mFrontRot.y = atn::WrapAngle_unk(1.5707964f + mFrontRot.y);
    mRoute = fn_803C1560(routes, &mFront, 0, 0, 0, 1, -1.0f);
    mTarget = 0;
    mLast = 0;
    mLastId = 0;
    m54 = 2;
    fn_8041EE28();
}

void Augur::fn_8041EE28() {
    mAugur = NpcManager::sInstance->Find_unk(mDesc->m20);
    if (mAugur != 0 && (mAugur->IsFlag_unk(0xDEAD0000) || !mAugur->IsFlag_unk(2) || mAugur->IsFlag_unk(1))) {
        mAugur = 0;
    }
    if (mAugur != 0) {
        mAugur->fn_8016E970(10);
        mAugur->m137C |= 1;
        mAugur->m54C |= 0x1000000;
        mAugur->mPos = m6C;
        mAugur->mRot = m78;
        fn_8041F6A4(2);
        mAugur->fn_80144710();
        mAugur->mMotion.mFlags &= ~0x40;
        mAugur->mMotion.mTracks[0].m10 = 1.0f;
        mAugur->mMotion.fn_80097E80(1);
        fn_8000D430(&mAugur->mMotion, boost::function<void()>());
    }
}

// NONMATCHING: 99.2% - state 5: the original keeps the modulo in r5 and sets up `this` before the
// compare (as if `this` were evaluated before a branchy argument). Tried: compare order, rate local,
// ternaries (become subfc/subfe), if/else, inline PickState(r, rate), state declared first, rand()
// in the condition, rate/desc locals, a this copy, negated compare, bool temporary.
void Augur::vf10() {
    if (mAugur != 0 && mAugur->IsFlag_unk(0xDEAD0000)) {
        CharaTask* target = mTarget;
        mAugur = 0;
        if (target != 0 && !target->IsFlag_unk(0xDEAD0000)) {
            ReleaseTarget_unk();
            mTarget = 0;
            mLast = 0;
            mLastId = 0;
        }
        return;
    }
    if (mAugur == 0) {
        fn_8041EE28();
        if (mAugur == 0) {
            return;
        }
    }
    if (m54 == 1) {
        return;
    }
    if (LenSq_unk(lbl_8087EFB4->mEye - m6C) > 250000.0f && mState == 2) {
        return;
    }
    switch (mState) {
    case 2:
        if (mTarget == 0) {
            mTarget = fn_8041F434(mLast);
        }
        if (mTarget != 0) {
            fn_8041F6A4(3);
        }
        break;
    case 3:
        if (mTarget->mRoute.m2C != 0) {
            fn_8041F6A4(4);
        }
        break;
    case 4: {
        CharaTask* target = mTarget;
        target->mPos = target->mPos + mMove;
        mTarget->mRot = mTarget->mRot + mTurn;
        Model_unk* model = &mTarget->mMotion;
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0)) {
            fn_8041F6A4(5);
        }
        break;
    }
    case 5:
        if (mTarget->IsPartyMember_unk()) {
            Model_unk* model = &mAugur->mMotion;
            float frame = model->mTracks[0].m08;
            if (frame >= model->fn_80097D7C(0)) {
                int r = rand() % 100;
                int state = 7;
                if (r > mDesc->m24) {
                    state = 6;
                }
                fn_8041F6A4(state);
            }
        }
        break;
    case 6:
    case 7: {
        CharaTask* target = mTarget;
        target->mPos = target->mPos + mMove;
        mTarget->mRot = mTarget->mRot + mTurn;
        Model_unk* model = &mTarget->mMotion;
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0)) {
            fn_8041F6A4(8);
        } else {
            mTimer--;
        }
        break;
    }
    }
}

int Augur::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    fn_8041F6A4(m54 = msg->mType);
    return m54;
}

int Augur::vf60() {
    int ret = 0;
    if (m54 != 1 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

CharaTask* Augur::fn_8041F434(CharaTask* last) {
    CharaTask* best = 0;
    float bestDist = 0.0f;
    for (NpcTask* npc = NpcManager::sInstance->mHead; npc != 0; npc = npc->mNext) {
        if (npc == last) {
            continue;
        }
        if (last != 0 && mLastId == npc->m50) {
            continue;
        }
        if (npc->mC58.mHeld_unk) {
            continue;
        }
        if (npc->m55C != 2) {
            continue;
        }
        if (!npc->IsActive_unk()) {
            continue;
        }
        if ((npc->mFlags & 0xDEAD0000) == 0xDEAD0000) {
            continue;
        }
        int match = 0;
        for (unsigned int i = 4; i < 8; i++) {
            int id = (&mDesc->m30)[i - 4];
            if (id == npc->m50) {
                match = 1;
                break;
            }
            if (id == 0 && i == 4) {
                match = 1;
                break;
            }
        }
        if (!match) {
            continue;
        }
        int point = npc->mC58.m0C.mDestPoint;
        if (point <= 0) {
            continue;
        }
        int linked = fn_803CD958(lbl_8087F430->m10D8->mA4 + (point - 1) * 8, mRoute - 1);
        if (linked <= 0) {
            continue;
        }
        float dist = LenSq_unk(npc->mPos - m6C);
        if (dist < 6400.0f && dist > bestDist) {
            bestDist = dist;
            best = npc;
        }
    }
    return best;
}

void Augur::fn_8041F6A4(int state) {
    mState = state;
    switch (state) {
    case 9:
        if (mTarget != 0) {
            ReleaseTarget_unk();
            mTarget = 0;
        }
        mTarget = lbl_8087F8A0->mHead;
        fn_8041F6A4(4);
        break;
    case 2:
        CharaTask* augur = mAugur;
        if (augur != 0) {
            augur->mMotion.m34C = 1;
            augur->mMotion.mTracks[0].m20 = 1.0f;
            augur->mMotion.mTracks[0].m0C = 1.0f;
            augur->mMotion.fn_80097C08(0, 0x9C, 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    case 3:
        mTarget->fn_80170F20(&mFront, 0, mFrontRot.y, -1.0f);
        mTarget->mC58.mHeld_unk = 1;
        break;
    case 4: {
        PlayMotion_unk(&mTarget->mMotion, 0x9D, 0, 1);
        CharaTask* target = mTarget;
        mMove = m6C - target->mPos;
        float rate = 1.0f / mTarget->mMotion.fn_80097D7C(0);
        mMove.x *= rate;
        mMove.y *= rate;
        mMove.z *= rate;
        CharaTask* target2 = mTarget;
        mTurn = m78 - target2->mRot;
        mTurn.x = atn::WrapAngle_unk(mTurn.x);
        mTurn.y = atn::WrapAngle_unk(mTurn.y);
        mTurn.z = atn::WrapAngle_unk(mTurn.z);
        rate = 1.0f / mTarget->mMotion.fn_80097D7C(0);
        mTurn.x *= rate;
        mTurn.y *= rate;
        mTurn.z *= rate;
        mTarget->fn_8016E970(10);
        SetSpeed_unk(mTarget);
        if (mTarget->IsPartyMember_unk()) {
            mTarget->m137C |= 1;
            mTarget->m54C |= 0x1000000;
        }
        break;
    }
    case 5:
        PlayMotion_unk(&mAugur->mMotion, 0x96, mTarget->mType == 0, 1);
        PlayMotion_unk(&mTarget->mMotion, 0x99, mTarget->mType == 0, 1);
        mTarget->mPos = m6C;
        mTarget->mRot = m78;
        break;
    case 6: {
        PlayMotion_unk(&mAugur->mMotion, 0x97, 0, 1);
        PlayMotion_unk(&mTarget->mMotion, 0x9A, 0, 1);
        atn::Vector3 move(7.0f, 0.0f, 17.0f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', m78.y);
        PSMTXMultVec(mtx, &move, &move);
        mMove = move;
        float rate = 1.0f / mTarget->mMotion.fn_80097D7C(0);
        mMove.x *= rate;
        mMove.y *= rate;
        mMove.z *= rate;
        mTurn.x = 0.0f;
        mTurn.y = 1.5707964f;
        mTurn.z = 0.0f;
        rate = 1.0f / mTarget->mMotion.fn_80097D7C(0);
        mTimer = 45;
        mTurn.x *= rate;
        mTurn.y *= rate;
        mTurn.z *= rate;
        break;
    }
    case 7: {
        PlayMotion_unk(&mAugur->mMotion, 0x98, 0, 1);
        PlayMotion_unk(&mTarget->mMotion, 0x9B, 0, 1);
        atn::Vector3 move(9.0f, 0.0f, 11.0f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', m78.y);
        PSMTXMultVec(mtx, &move, &move);
        mMove = move;
        float rate = 1.0f / mTarget->mMotion.fn_80097D7C(0);
        mMove.x *= rate;
        mMove.y *= rate;
        mMove.z *= rate;
        mTurn.x = 0.0f;
        mTurn.y = 1.5707964f;
        mTurn.z = 0.0f;
        rate = 1.0f / mTarget->mMotion.fn_80097D7C(0);
        mTimer = 120;
        mTurn.x *= rate;
        mTurn.y *= rate;
        mTurn.z *= rate;
        break;
    }
    case 8:
        mLast = mTarget;
        mLastId = mTarget->m50;
        ReleaseTarget_unk();
        mTarget = 0;
        fn_8041F6A4(2);
        break;
    }
}

Augur::~Augur() {}

} // namespace gmk
