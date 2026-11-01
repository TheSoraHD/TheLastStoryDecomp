// inferred: controls of the characters stuck, thrust, held or thrown by enemies (vtables
// 0x8077DE00-0x8077ED00, RTTI names in .data). The classes are declared in .dol order in
// chr/ControlStickByQuark.h (their implicit destructors are emitted at the end of the file).
#include "chr/ControlStickByQuark.h"
#include "game/CharaTask.h"
#include "game/EnemyQuarkStand.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>

// inferred: object behind lbl_8087F490 (the HUD)
struct Unk_8087F490 {
    unsigned char m000[0x728];
    int m728; // inferred: shows the "swing the remote" icon
};

// inferred: input state embedded at +0x48C of the object behind lbl_8087F0A8
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero when `button` was pressed
};

struct Unk_8087F0A8 {
    unsigned char m000[0x48C];
    Input_unk mInput; // 0x48C
};

extern "C" Unk_8087F490* lbl_8087F490;
extern "C" Unk_8087F0A8* lbl_8087F0A8;

namespace chr {

// inferred: front of a character (z axis turned by its yaw; name invented)
static inline atn::Vector3 Front_unk(CharaTask* chara) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'y', chara->mRot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}

// inferred: -v by value (name invented)
static inline atn::Vector3 Neg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: a x b by value (name invented, as in chr/ControlChaseCaught.cpp)
static inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: matrix the character follows (CharaTask::mF1C holds its address; name invented)
static inline atn::Matrix3x4* FollowMtx_unk(CharaTask* chara) {
    return (atn::Matrix3x4*)chara->mF1C;
}

ControlStickByQuark::ControlStickByQuark(Chara_unk* chara, int quark)
    : ControlBase(chara), mQuark((EnemyQuarkStand*)quark), mDone(false), mStuck(false), mReleased(false),
      mCount(30) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x53);
}

// NONMATCHING: 98.98% - the original tests 0xE/0xF/0x10 with three cmpwi/beq to one `ret = 1` (the
// `||` form is merged into a range check, the else-if form duplicates the store)
int ControlStickByQuark::vf0C() {
    EnemyQuarkStand* quark = mQuark;
    int ret = 0;
    Motion_unk* m = &mChara->mMotion;
    if (mStuck) {
        m->mTracks[0].m0C = m->mTracks[0].m08 < 60.0f ? 0.5f : 1.0f;
        if (mReleased) {
            float frame = m->mTracks[0].m08;
            if (frame >= m->fn_80097D7C(0)) {
                ret = 1;
            }
        } else {
            if (mChara->mType == 0) {
                lbl_8087F490->m728 = 1;
                if (lbl_8087F0A8->mInput.fn_8012476C(0x22) && --mCount <= 0) {
                    mDone = true;
                }
            }
            if (mDone && m->mTracks[0].m08 >= 86.0f) {
                quark->m17EC = 1;
                ret = 1;
            }
            if (!mDone && m->mTracks[0].m08 >= 88.0f) {
                mReleased = true;
                mChara->mPos = FollowMtx_unk(mChara)->GetTrans_unk();
                mChara->mRot = Neg_unk(Front_unk(mQuark)).ToRotation_unk();
                mChara->mF1C = 0;
            }
        }
    }
    if (quark->m58C == 0xE) {
        ret = 1;
    } else if (quark->m58C == 0xF) {
        ret = 1;
    } else if (quark->m58C == 0x10) {
        ret = 1;
    }
    if (mChara->mF1C) {
        mChara->mPos = FollowMtx_unk(mChara)->GetTrans_unk();
    }
    if (ret && mChara->mF1C) {
        mChara->mPos = FollowMtx_unk(mChara)->GetTrans_unk();
        mChara->mRot = Neg_unk(Front_unk(mQuark)).ToRotation_unk();
        mChara->mF1C = 0;
    }
    return ret;
}

// inferred: the character is stuck on the right arm of the quark (called by EnemyQuarkStand)
void ControlStickByQuark::fn_80192FB4() {
    mStuck = true;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x217, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    m->mTracks[0].m08 = 28.0f;
    mChara->mF1C = (int)mQuark->mMotion.GetNodeMtx_unk("rightwepoint");
}

// NONMATCHING: 96.4% - the original loads mDone before saving r31 (as in a const method) and swaps
// r8/r9 in the copies of the bound vector
// inferred: thrown sideways when released, gets up otherwise
boost::function<void()> ControlStickByQuark::vf10() {
    if (mDone) {
        EnemyQuarkStand* quark = mQuark;
        atn::Vector3 dir = quark->mPos - mChara->mPos;
        dir.y = 0.0f;
        dir.Normalize_unk();
        if (quark->m17F0 == 0) {
            dir = Cross_unk(dir, atn::Vector3(0.0f, -1.0f, 0.0f));
        } else {
            dir = Cross_unk(dir, atn::Vector3(0.0f, 1.0f, 0.0f));
        }
        return boost::bind(&CharaTask::fn_80155790, (CharaTask*)mChara, dir);
    }
    return boost::bind(&CharaTask::fn_8015ECC4, (CharaTask*)mChara);
}

void ControlStickByQuark::vf1C() {
    mChara->mF1C = 0;
}

} // namespace chr
