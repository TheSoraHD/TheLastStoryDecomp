// inferred: controls of the characters stuck, thrust, held or thrown by enemies (vtables
// 0x8077DE00-0x8077ED00, RTTI names in .data). The classes are declared in .dol order in
// chr/ControlStickByQuark.h (their implicit destructors are emitted at the end of the file).
#include "chr/ControlStickByQuark.h"
#include "game/CharaTask.h"
#include "game/EnemyQuarkStand.h"
#include "game/Em302Kraken.h"
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

// inferred: camera of the field behind lbl_8087F430 (class unknown)
struct Camera_unk {
    unsigned char m000[0x74];
    atn::Vector3 mEye;    // 0x74
    atn::Vector3 mAt;     // 0x80
    unsigned char m08C[0x9CC - 0x8C];
    atn::Vector3 m9CC;    // inferred: eye offset kept during the thrust
};

extern "C" Camera_unk* lbl_8087F430;
extern "C" void* lbl_8087F498; // inferred: voice player
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);
extern "C" int rand();
extern "C" Unk_8087F490* lbl_8087F490;

// inferred: game state (lbl_8087F490 of chr/ControlAttackBase.cpp): m3A4 is the time scale
struct GameState_unk {
    unsigned char m000[0x240];
    int m240; // inferred: slow motion on
    unsigned char m244[0x3A4 - 0x244];
    float m3A4;
};
extern "C" GameState_unk* lbl_8087EFA8;

struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" atn::EfpFile* fn_800EFBC4(int id); // inferred: battle effect of row `id`
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an effect on a model (name invented)
static inline void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: special sword attack held by chr::ControlStickSwordSp (class unknown)
struct SwordEffect_unk {
    unsigned char m00[0x04];
    int mId; // 0x04: inferred: battle effect row
};
struct SwordParam_unk {
    SwordParam_unk() {}
    float m00;
    int m04;
    int m08;
    SwordEffect_unk* mEffect; // 0x0C
    int m10;
    float m14;
    float m18;
};
class SwordAttack_unk {
public:
    virtual void vf08();
    virtual void vf0C(int a, int b);
    unsigned char m04[0x5C - 0x04];
    SwordParam_unk mParam; // 0x5C
};
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

// NONMATCHING: 94.6% - stack slots of the temporaries (the eye offset is the lowest one in the original)
// inferred: thrust at `other` from the offset `offset` (turned by its yaw) in 24 frames
ControlThrust::ControlThrust(Chara_unk* chara, Chara_unk* other, atn::Vector3 offset, int b)
    : ControlBase(chara), mOther(other), m1C(b), m20(0), m24(0) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x4B);
    float yaw = other->mRot.y;
    atn::Vector3 pos = offset;
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'y', yaw);
    PSMTXMultVec(m.m, &pos, &pos);
    pos += other->mPos;
    m14 = (3.1415927f + yaw - mChara->mRot.y) / 24.0f;
    lbl_8087F430->m9CC = lbl_8087F430->mEye - lbl_8087F430->mAt;
    mStep = atn::Vector3(pos - mChara->mPos) * (1.0f / 24.0f);
    Motion_unk* mo = &mChara->mMotion;
    mo->m34C = 1;
    mo->mTracks[0].m20 = 1.0f;
    mo->fn_80097C08(0, 0x20B, 0, 0, 1, 0.0f, 0.1f);
    mo->mTracks[0].m0C = 1.0f;
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 97.5% - scheduling of the copy of `target` (the original reloads target.x/z for dir)
// inferred: thrust at a kraken from `target`; the effect is played for the kraken of kind 0x1E
ControlThrustForKraken::ControlThrustForKraken(Chara_unk* chara, int kraken, atn::Vector3 target)
    : ControlBase(chara) {
    Em302Kraken* k = (Em302Kraken*)kraken;
    chara->m58C = 0;
    mChara->SetControlId_unk(0x4D);
    if (k->m60->m24 == 0x1E) {
        PlayEfp_unk(lbl_8087F3C0, &k->m15F4, (atn::Model*)mChara->m648->m010);
    }
    mTarget = target;
    mTarget.y = 70.0f + k->mPos.y;
    mFrom = k->mPos;
    mFrom.y = 70.0f + k->mPos.y;
    atn::Vector3 dir = mTarget - mFrom;
    dir.y = 0.0f;
    dir.Normalize_unk();
    mDest = mFrom + dir * 30.0f;
    mDir = dir;
    mDir2 = dir;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x20D, 0, 1, 1, 1.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    m->mTracks[0].m08 = 23.0f;
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 71.9% - the original loads m10/m1C before saving r31 (as in a const method)
atn::Vector3 ControlThrustForKraken::vf14() {
    return mFrom + (mFrom - mTarget).GetNormalize() * 30.0f;
}

void ControlThrustForQuarkStand::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
    lbl_8087EFA8->m240 = 0;
}

// NONMATCHING: 60% - psq_st before the stfs of z in the original (same as ControlWallSlash::vf14)
atn::Vector3 ControlThrustForGhoul::vf14() {
    return m10;
}

void ControlThrustForGhoul::vf40() {
}

void ControlThrustForGhoul::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
    lbl_8087EFA8->m240 = 0;
}

ControlAbsorbed::ControlAbsorbed(Chara_unk* chara) : ControlBase(chara) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x41);
    mChara->fn_8016DA4C();
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x20C, 1, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

int ControlAbsorbed::vf0C() {
    return 0;
}

// inferred: ends with the motion of the other character
int ControlStickSword::vf0C() {
    float frame = m08->mMotion.mTracks[0].m08;
    if (frame >= m08->mMotion.fn_80097D7C(0)) {
        m08->vf44();
        return 1;
    }
    return 0;
}

// inferred: the special attack: plays its effect when the first motion ends, then a second motion
int ControlStickSwordSp::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    switch (mState) {
    case 0: {
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->fn_80097C08(0, 0x211, 0, 0, 1, 0.0f, 0.1f);
            m->mTracks[0].m0C = 1.0f;
            SwordParam_unk param = mAttack->mParam;
            atn::EfpFile* efp = fn_800EFBC4(param.mEffect->mId);
            if (efp) {
                Color4f_unk color;
                atn::Vector3 rot;
                atn::Vector3 pos;
                rot.x = 0.0f;
                rot.y = 0.0f;
                rot.z = 0.0f;
                pos.x = 0.0f;
                pos.y = 0.0f;
                pos.z = 0.0f;
                color.r = 1.0f;
                color.g = 1.0f;
                color.b = 1.0f;
                color.a = 1.0f;
                fn_8023A8B4(lbl_8087F3C0, efp, (atn::Model*)m, 0, pos, rot, color, -1, -1, 1, 1.0f);
            }
            mAttack->vf0C(0, 1);
            mState = 1;
        }
        break;
    }
    case 1: {
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            mChara->vf44();
            return 1;
        }
        break;
    }
    }
    return 0;
}

bool ControlStickSwordSp::fn_80196C68() const {
    Motion_unk* m = &mChara->mMotion;
    bool ret = false;
    if (mState == 1) {
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            ret = true;
        }
    }
    return ret;
}

void ControlMatHoldGuard::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
}

} // namespace chr
