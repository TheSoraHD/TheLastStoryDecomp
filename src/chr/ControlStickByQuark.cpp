// inferred: controls of the characters stuck, thrust, held or thrown by enemies (vtables
// 0x8077DE00-0x8077ED00, RTTI names in .data). The classes are declared in .dol order in
// chr/ControlStickByQuark.h (their implicit destructors are emitted at the end of the file).
#include "chr/ControlStickByQuark.h"
#include "game/CharaTask.h"
#include "game/EnemyQuarkStand.h"
#include "game/Em302Kraken.h"
#include "atn/Memory.h"
#include "game/GameUI.h"
#include <boost/bind.hpp>


// inferred: input state embedded at +0x48C of the object behind lbl_8087F0A8
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero when `button` was pressed
    int fn_801231D0(int action); // inferred: action triggered
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
    unsigned char m08C[0x8A0 - 0x8C];
    CharaTask* m8A0;      // inferred: character the camera follows
    unsigned char m8A4[0x96C - 0x8A4];
    int m96C;             // inferred: camera shake
    int m970;
    float m974;
    float m978;
    unsigned char m97C[0x9C4 - 0x97C];
    float m9C4;           // inferred: zoom of the thrust
    float m9C8;
    atn::Vector3 m9CC;    // inferred: eye offset kept during the thrust
    // inferred: starts a camera shake (name invented)
    void Shake_unk(int time, float a, float b) {
        m96C %= 2;
        m970 = time;
        m974 = a;
        m978 = b;
    }
};

extern "C" Camera_unk* lbl_8087F430;
extern "C" void* lbl_8087F498; // inferred: voice player
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);
extern "C" int rand();
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048; // inferred: effect/bullet manager
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int time, int b, int c, float f1, float f2);
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void* lbl_8087EF70; // inferred: pad
// inferred: result of the collision ray test (as in chr/ControlLadder.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button pressed

// inferred: HUD hint request (as ZanUIHint_unk of game/Em201ZanGurgFinal.cpp)
struct ThrustHint_unk : public GameUIElem1C_unk {
    ThrustHint_unk(int a, int b, int c) {
        m00 = a;
        m04 = b;
        m08 = c;
        m0C = 0;
        m10 = 0;
        m14 = 0;
        m18 = 1;
    }
};

// inferred: game state (lbl_8087F490 of chr/ControlAttackBase.cpp): m3A4 is the time scale
// inferred: slow motion settings (names invented)
struct SlowParam_unk {
    SlowParam_unk() {}
    int m00;
    int m04;
    float m08;
    float m0C;
    int m10;
    int m14;
    float m18;
    float m1C;
};
struct Slow_unk {
    Slow_unk(int on, const SlowParam_unk& param) : mOn(on), mParam(param) {}
    int mOn;
    SlowParam_unk mParam;
};
struct GameState_unk {
    unsigned char m000[0x240];
    // inferred: turns the slow motion on with the parameters of `s` (only the flag is stored; name
    // invented)
    void Start_unk(const Slow_unk& s) { mSlow.mOn = s.mOn; }
    void Set_unk(const Slow_unk& s) {
        mSlow.mOn = s.mOn;
        mSlow.mParam = s.mParam;
    }
    Slow_unk mSlow; // 0x240
    unsigned char m264[0x3A4 - 0x264];
    float m3A4;     // inferred: time scale
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

// inferred: a + (b - a) * t (name invented, as in chr/ControlLadder.cpp)
static inline atn::Vector3 LerpTo_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: m = m * translation(v) (name invented, as in gmk/Chain.cpp)
static inline void TranslateMtx_unk(atn::Matrix3x4& m, const atn::Vector3& v) {
    Mtx t;
    Mtx r;
    PSMTXTrans(t, v.x, v.y, v.z);
    PSMTXConcat(m.m, t, r);
    __MtxCopy(m.m, r);
}

// inferred: frame of track 0 (name invented)
static inline float Frame_unk(Motion_unk* m) {
    return m->mTracks[0].m08;
}

// inferred: fires `hit` from `pos` (name invented)
static inline void ShootHit_unk(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* hit, const atn::Vector3& pos,
                                const atn::Vector3* rot) {
    fn_800FAB80(mgr, chara, hit, fn_800F8548(mgr), &pos, rot, 0, 0x1E, -1, -1, 0.0f, 1.0f);
}

// inferred: fires the bullet `id` from `pos` (name invented)
static inline void ShootFrom_unk(Effect_unk* mgr, CharaTask* chara, int id, int arg, const atn::Vector3& pos,
                                 const atn::Vector3* rot) {
    fn_800FAB80(mgr, chara, fn_80219E6C(id), arg, &pos, rot, 0, 0x1E, -1, -1, 0.0f, 1.0f);
}

// inferred: fires the bullet `id` from `pos` (name invented)
static inline void ShootIdFrom_unk(Effect_unk* mgr, CharaTask* chara, int id, const atn::Vector3& pos,
                                   const atn::Vector3* rot) {
    fn_800FAB80(mgr, chara, fn_80219E6C(id), fn_800F8548(mgr), &pos, rot, 0, 0x1E, -1, -1, 0.0f, 1.0f);
}

// inferred: fires the bullet `id` from a character (name invented)
static inline void Shoot_unk(Effect_unk* mgr, CharaTask* chara, int id, const atn::Vector3* pos,
                             const atn::Vector3* rot) {
    fn_800FAB80(mgr, chara, fn_80219E6C(id), fn_800F8548(mgr), pos, rot, 0, 0x1E, -1, -1, 0.0f, 1.0f);
}

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

// NONMATCHING: 95% - copies of the slow motion parameters (the original keeps the stack copy of the
// second one) and register order around the bullet call
// inferred: forward, hold and back phases of the thrust; the player can push again (button 4) for
// a second thrust while the enemy is down
int ControlThrust::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    atn::Vector3 pos = mChara->mPos;
    atn::Vector3 rot = mChara->mRot;
    if (m24) {
        if (m1C && m20 < 2 && mOther && mOther->CanAct_unk() && lbl_8087F490) {
            lbl_8087F490->m764 = ThrustHint_unk(6, 0x10, -1);
        }
        if (m->mTracks[0].m08 <= 42.0f) {
            m->mTracks[0].m0C = 1.0f;
            m24 = 0;
        }
    } else if (m->mTracks[0].m08 >= 0.0f && m->mTracks[0].m08 < 24.0f) {
        lbl_8087F430->m9C4 = m->mTracks[0].m08 / 24.0f;
        pos += mStep;
        rot.y += m14;
    } else if (Frame_unk(m) >= m->fn_80097D7C(0) - 24.0f && Frame_unk(m) <= m->fn_80097D7C(0)) {
        lbl_8087F430->m9C4 = (m->fn_80097D7C(0) - Frame_unk(m)) / 24.0f;
        pos.x -= mStep.x * 1.1f;
        pos.y -= mStep.y;
        pos.z -= mStep.z * 1.1f;
        rot.y -= m14;
    } else {
        lbl_8087F430->m9C4 = 1.0f;
        if (m->mTracks[0].m08 > 54.0f && m->mTracks[0].m08 <= 55.0f) {
            Shoot_unk(lbl_8087F048, mChara, 0xCC, &mChara->mPos, &mChara->mRot);
            lbl_8087F430->Shake_unk(10, 0.3f, 0.3f);
            if (m20 >= 1) {
                fn_800C344C("SE_SP009_002", &mChara->mPos, 0, -1, 1.5f);
            }
        }
        bool hint = CanRepeat_unk() && mOther->CanAct_unk();
        if (hint && m->mTracks[0].m08 > 42.0f && m->mTracks[0].m08 <= 75.0f && hint && lbl_8087F490) {
            lbl_8087F490->m764 = ThrustHint_unk(6, 0x10, -1);
        }
        if (m->mTracks[0].m08 > 55.0f && m->mTracks[0].m08 <= 75.0f && fn_800A555C(lbl_8087EF70, 0, 4)) {
            m24 = 1;
            m20++;
            m->mTracks[0].m0C = -1.5f;
            fn_800C31F4("SE_UICOM_011", 0, -1, 1.0f);
            if (lbl_8087F490) {
                lbl_8087F490->fn_803E3384();
            }
        }
        if (mOther && (mOther->mStatus.m0C & 0x20) == 0x20) {
            Camera_unk* cam = lbl_8087F430;
            if (m->mTracks[0].m08 > 42.0f && m->mTracks[0].m08 <= 75.0f) {
                lbl_8087EFA8->Start_unk(Slow_unk(1, lbl_8087EFA8->mSlow.mParam));
                lbl_8087EFA8->m3A4 = 0.4f;
                cam->m8A0 = mChara;
                cam->m9C8 = 0.3f * ((m->mTracks[0].m08 - 42.0f) / 35.0f) + 1.0f;
            } else {
                lbl_8087EFA8->Set_unk(Slow_unk(0, lbl_8087EFA8->mSlow.mParam));
                lbl_8087EFA8->m3A4 = 1.0f;
                cam->m8A0 = 0;
                cam->m9C8 = 1.0f;
            }
        }
    }
    mChara->mPos = pos;
    mChara->mRot = rot;
    float frame = m->mTracks[0].m08;
    return frame >= m->fn_80097D7C(0);
}

// NONMATCHING: 89% - the original keeps the offset vector out of the stack (frame 0x170)
// inferred: thrust at a crab (control id 0x4C); faces the crab
ControlThrustForCrab::ControlThrustForCrab(Chara_unk* chara, Chara_unk* crab) : ControlBase(chara) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x4C);
    atn::Vector3 target = atn::Vector3(0.0f, -20.451f, 0.0f) + crab->mPos;
    mStep = atn::Vector3(target - mChara->mPos) * 0.025f * 1.5f;
    mBack = atn::Vector3(mChara->mPos - target) * 0.04f;
    atn::Vector3 rot = mChara->mRot;
    rot.y = (target - mChara->mPos).ToRotation_unk().y;
    mChara->mRot = rot;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x20E, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.5f;
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 99.5% - r29/r31 swapped between the motion and the bullet manager
int ControlThrustForCrab::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    atn::Vector3 pos = mChara->mPos;
    if (m->mTracks[0].m08 >= 0.0f && m->mTracks[0].m08 < 40.0f) {
        m->mTracks[0].m0C = 1.5f;
        pos += mStep;
    } else if (Frame_unk(m) >= m->fn_80097D7C(0) - 25.0f && Frame_unk(m) <= m->fn_80097D7C(0)) {
        m->mTracks[0].m0C = 1.0f;
        pos += mBack;
    } else {
        m->mTracks[0].m0C = 1.0f;
    }
    mChara->mPos = pos;
    if (m->mTracks[0].m08 >= 41.0f && m->mTracks[0].m08 < 43.0f) {
        CharaTask* c = mChara;
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, mChara, fn_80219E6C(0xD1), 1000, &c->mPos, &c->mRot, 0, 0x1E, -1, -1, 0.0f, 1.0f);
        lbl_8087F430->Shake_unk(10, 0.3f, 0.3f);
    }
    return Frame_unk(m) >= m->fn_80097D7C(0);
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

// NONMATCHING: 98.6% - f3/f4 of the frame and 23.0f, register order of the lerps
// inferred: swings the weapon around the kraken and thrusts it (frames 23-62), holds it, then pulls
// back (frames 112-153); the bullet is fired from the weapon node at frame 48
int ControlThrustForKraken::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    atn::Vector3 pos = mChara->mPos;
    atn::Vector3 rot = mChara->mRot;
    if (m->mTracks[0].m08 > 23.0f && m->mTracks[0].m08 <= 62.0f) {
        mChara->mF1C = 0;
        float t = (m->mTracks[0].m08 - 23.0f) / 39.0f;
        pos = LerpTo_unk(mTarget - mDir2 * 45.0f, mFrom, t);
        rot = LerpTo_unk(mDir, mDir2, t);
        rot.RotZ_unk();
    } else if (m->mTracks[0].m08 > 62.0f && m->mTracks[0].m08 <= 112.0f) {
        pos = mFrom;
    } else if (m->mTracks[0].m08 > 112.0f && m->mTracks[0].m08 <= 153.0f) {
        float t = (m->mTracks[0].m08 - 112.0f) / 41.0f;
        pos = LerpTo_unk(mFrom, mDest, t);
    }
    mChara->mPos = pos;
    mChara->mRot = rot;
    if (m->mTracks[0].m08 >= 48.0f && m->mTracks[0].m08 < 50.0f) {
        const atn::Vector3* r = &mChara->mRot;
        atn::Matrix3x4* node = (atn::Matrix3x4*)m->GetNodeMtx_unk("rightwepoint");
        ShootFrom_unk(lbl_8087F048, mChara, 0xD5, 1000, node->GetTrans_unk(), r);
    }
    return Frame_unk(m) >= m->fn_80097D7C(0);
}

// NONMATCHING: 71.9% - the original loads m10/m1C before saving r31 (as in a const method)
atn::Vector3 ControlThrustForKraken::vf14() {
    return mFrom + (mFrom - mTarget).GetNormalize() * 30.0f;
}

// NONMATCHING: 99.87% - r7/r8 swapped between the vtable address and the zero of m28
// inferred: thrust at a standing quark (control id 0x89); `kind` 2 thrusts from 30 units further
ControlThrustForQuarkStand::ControlThrustForQuarkStand(Chara_unk* chara, Chara_unk* quark, int kind)
    : ControlBase(chara), mQuark(quark), mKind(kind), m28(0) {
    chara->SetControlId_unk(0x89);
    Motion_unk* m = &mChara->mMotion;
    int motion = 0x215;
    if (mKind == 2) {
        motion = 0x216;
    }
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, motion, 0, 0, 1, 1.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    mStart = mChara->mPos;
    mTarget = quark->mPos;
    if (mKind == 2) {
        atn::Vector3 dir(mStart.x - mTarget.x, 0.0f, mStart.z - mTarget.z);
        dir.Normalize_unk();
        mChara->mRot = dir.ToRotation_unk();
        mTarget += dir * 30.0f;
    } else {
        atn::Vector3 dir(mStart.x - mTarget.x, 0.0f, mStart.z - mTarget.z);
        dir.Normalize_unk();
        mChara->mRot = dir.ToRotation_unk();
    }
    mFront = Front_unk(mChara);
}

// NONMATCHING: 89% - order of the lerp products and of the bullet call, copy of the position
int ControlThrustForQuarkStand::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    float frame = m->mTracks[0].m08;
    if (frame <= 20.0001f) {
        if (mKind != 2) {
            mChara->mPos = LerpTo_unk(mStart, mTarget, frame / 20.0f);
        } else {
            mChara->mPos = atn::Vector3(mTarget);
        }
        lbl_8087EFA8->mSlow.mOn = 1;
    } else if (mKind == 2) {
        lbl_8087EFA8->mSlow.mOn = !(frame >= 180.0f);
        if (m->mTracks[0].m08 >= 120.0f && m->mTracks[0].m08 < 165.0f) {
            float scale = lbl_8087EFA8->m3A4 - 0.05f;
            lbl_8087EFA8->m3A4 = 0.3f > scale ? 0.3f : scale;
            if (m28 == 0 && m->mTracks[0].m08 >= 130.0f) {
                m28 = 1;
            }
        } else {
            lbl_8087EFA8->m3A4 = 1.0f;
        }
    } else if (frame <= 26.0001f) {
    } else if (frame <= 60.0001f) {
        lbl_8087EFA8->mSlow.mOn = 0;
        lbl_8087EFA8->m3A4 = 1.0f;
        if (m28 == 0 && m->mTracks[0].m08 > 55.0f) {
            m28 = 1;
            mChara->fn_8016DA4C();
            HitInfo_unk* hit = mChara->m638;
            atn::Matrix3x4* node = (atn::Matrix3x4*)m->GetNodeMtx_unk("rightwepoint");
            ShootHit_unk(lbl_8087F048, mChara, hit, node->GetTrans_unk(), &mChara->mRot);
        }
    } else if (frame > 90.0001f) {
        mChara->mPos = atn::Vector3(mChara->mPos) + mFront * 1.8333334f;
        atn::Vector3 rot = mChara->mRot;
        rot.y = atn::WrapAngle_unk(rot.y + 3.1415927f / (m->fn_80097D7C(0) - 90.0f));
        mChara->mRot = rot;
    }
    if (Frame_unk(m) >= m->fn_80097D7C(0)) {
        mChara->m570 = 0.0f;
        mChara->m574 = lbl_807C7030;
        return 1;
    }
    return 0;
}

void ControlThrustForQuarkStand::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
    lbl_8087EFA8->mSlow.mOn = 0;
}

// inferred: thrust at the neck of a ghoul (control id 0x51)
ControlThrustForGhoul::ControlThrustForGhoul(Chara_unk* chara, Chara_unk* other, const char* name)
    : ControlBase(chara), mOther(other), m34(15.0f), m38(false), m39(false) {
    chara->SetControlId_unk(0x51);
    mStart = mChara->mPos;
    mTarget = mOther->mPos;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x20A, 0, 0, 1, 1.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    atn::Vector3 dir(mTarget.x - mStart.x, 0.0f, mTarget.z - mStart.z);
    mChara->mRot = dir.ToRotationZ_unk();
    m10 = mStart;
}

// inferred: turns `v` around the y axis (name invented)
static inline void RotY_unk(atn::Vector3& v, float rad) {
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'y', rad);
    PSMTXMultVec(m.m, &v, &v);
}

// NONMATCHING: 93.8% - the thrust (frames 0-15), the hit (15-65) and the step back (65-80); the back
// point is moved aside when a wall is behind the character
int ControlThrustForGhoul::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    m34 -= lbl_8087EFA8->m3A4 * m->mTracks[0].m0C;
    float frame = m->mTracks[0].m08;
    if (frame <= 15.0001f) {
        float scale = lbl_8087EFA8->m3A4 - 0.05f;
        lbl_8087EFA8->m3A4 = 0.5f > scale ? 0.5f : scale;
        lbl_8087EFA8->mSlow.mOn = 1;
        float t = m->mTracks[0].m08 / 15.0f;
        mChara->mPos = LerpTo_unk(mStart, mOther->mPos, t);
        m10 = mChara->mPos;
        m10.y += 20.0f * t;
    } else if (frame <= 65.0001f) {
        lbl_8087EFA8->mSlow.mOn = 0;
        lbl_8087EFA8->m3A4 = 1.0f;
        if (!m38 && m->mTracks[0].m08 > 30.0f) {
            m38 = true;
            mChara->fn_8016DA4C();
            HitInfo_unk* hit = mChara->m638;
            atn::Matrix3x4* node = (atn::Matrix3x4*)m->GetNodeMtx_unk("rightwepoint");
            ShootHit_unk(lbl_8087F048, mChara, hit, node->GetTrans_unk(), &mChara->mRot);
            lbl_8087F430->Shake_unk(6, 0.0f, 2.0f);
        }
        m10 = mChara->mPos;
        m10.y += 20.0f;
    } else if (frame <= 80.0001f) {
        if (!m39) {
            atn::Vector3 back = Front_unk(mChara) * -25.0f;
            mTarget = mChara->mPos + back;
            atn::Vector3 from = mChara->mPos;
            from.y += 10.0f;
            atn::Vector3 to = from + back;
            CollResult_unk result;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
                back = Front_unk(mChara) * -25.0f;
                RotY_unk(back, 0.9424779f);
                mTarget = mChara->mPos + back;
                to = from + back;
                if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
                    back = Front_unk(mChara) * -25.0f;
                    RotY_unk(back, -0.9424779f);
                    mTarget = mChara->mPos + back;
                    to = from + back;
                    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
                        back = Front_unk(mChara) * -20.0f;
                        mTarget = mChara->mPos + back;
                    }
                }
            }
            from = mTarget;
            from.y += 10.0f;
            to = mTarget;
            to.y -= 10.0f;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
                mTarget = result.m04;
            }
            mStart = mChara->mPos;
            m39 = true;
        }
        float t = 1.0f - (m->mTracks[0].m08 - 65.0f) / 15.0f;
        mChara->mPos = LerpTo_unk(mTarget, mStart, t);
        m10 = LerpTo_unk(mChara->mPos, mOther->mPos, t);
        m10.y += 20.0f * t;
    } else {
        m10 = mChara->mPos;
    }
    if (Frame_unk(m) >= m->fn_80097D7C(0)) {
        mChara->m570 = 0.0f;
        mChara->m574 = lbl_807C7030;
        return 1;
    }
    return 0;
}

// NONMATCHING: 60% - psq_st before the stfs of z in the original (same as ControlWallSlash::vf14)
atn::Vector3 ControlThrustForGhoul::vf14() {
    return m10;
}

void ControlThrustForGhoul::vf40() {
}

void ControlThrustForGhoul::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
    lbl_8087EFA8->mSlow.mOn = 0;
}

// inferred: thrust at a cocoon (control id 0x4F), facing it
ControlThrustForCocoon::ControlThrustForCocoon(Chara_unk* chara, Chara_unk* cocoon, int a)
    : ControlBase(chara), mCocoon(cocoon), m28(a), m2C(false) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x4F);
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x212, 0, 1, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    mYaw = atn::WrapAngle_unk(3.1415927f + cocoon->mRot.y);
    mTarget = cocoon->mPos;
    mStart = mChara->mPos;
}

// NONMATCHING: 96.7% - registers around the bullet call; approach (frames 0-25), thrust and bullet (140), step back from frame 175
int ControlThrustForCocoon::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    atn::Vector3 pos = mChara->mPos;
    atn::Vector3 rot = mChara->mRot;
    float frame = m->mTracks[0].m08;
    if (frame < 25.0f) {
        pos = LerpTo_unk(mStart, mTarget, frame / 30.0f);
        rot.y += 0.1f * atn::WrapAngle_unk(mYaw - rot.y);
    } else if (frame > 175.0f) {
        float t = (frame - 175.0f) / 50.0f;
        t = 1.0f < t ? 1.0f : t;
        pos = LerpTo_unk(mTarget, mStart, t);
    } else {
        pos = mTarget;
        rot.y = mYaw;
        lbl_8087F430->m9C4 = 1.0f;
        if (!m2C && m->mTracks[0].m08 > 140.0f) {
            m2C = true;
            const atn::Vector3* r = &mChara->mRot;
            atn::Matrix3x4* node = (atn::Matrix3x4*)m->GetNodeMtx_unk("rightwepoint");
            ShootIdFrom_unk(lbl_8087F048, mChara, m28, node->GetTrans_unk(), r);
        }
    }
    rot.x = atn::WrapAngle_unk(rot.x);
    rot.y = atn::WrapAngle_unk(rot.y);
    rot.z = atn::WrapAngle_unk(rot.z);
    mChara->mPos = pos;
    mChara->mRot = rot;
    if (Frame_unk(m) >= m->fn_80097D7C(0)) {
        return 1;
    }
    if ((mCocoon->mStatus.m0C & 0x20) != 0x20 && m->mTracks[0].m08 >= 240.0f) {
        return 1;
    }
    return 0;
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

// NONMATCHING: 92.9% - copies of the front vector and of the new position; guards while held by the mat (the player shakes the remote to break free)
int ControlMatHoldGuard::vf0C() {
    if ((mChara->mStatus.m0C & 0x20) == 0x20) {
        mChara->vf44();
        return 1;
    }
    Motion_unk* m = &mChara->mMotion;
    if (Frame_unk(m) >= m->fn_80097D7C(0)) {
        lbl_8087EFA8->m3A4 = 1.0f;
        return 1;
    }
    if (Frame_unk(m) >= m->fn_80097D7C(0) - 8.0f) {
        float speed = m->mTracks[0].m0C - 0.1f;
        m->mTracks[0].m0C = 0.05f > speed ? 0.05f : speed;
    }
    if (mChara->mType == 0) {
        lbl_8087F490->m738 = 1;
        if (mCount > 0 && lbl_8087F0A8->mInput.fn_801231D0(0)) {
            mCount--;
        }
    }
    atn::Matrix3x4 mtx = *(atn::Matrix3x4*)mHolder;
    atn::Vector3 off;
    off.z = -9.0f;
    off.y = 0.0f;
    off.x = -5.0f;
    TranslateMtx_unk(mtx, off);
    mChara->mPos = LerpTo_unk(mChara->mPos, mtx.GetTrans_unk(), 0.2f);
    mtx.m[0][3] = 0.0f;
    mtx.m[1][3] = 0.0f;
    mtx.m[2][3] = 0.0f;
    atn::Vector3 dir = atn::Vector3(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(mtx.m, &dir, &dir);
    mChara->mRot = dir.ToRotation_unk();
    if (mCount <= 0) {
        lbl_8087EFA8->m3A4 = 1.0f;
        return 1;
    }
    return 0;
}

// inferred: thrown backwards when broken free
boost::function<void()> ControlMatHoldGuard::vf10() {
    if (mCount <= 0) {
        return boost::bind(&CharaTask::fn_8015DEC0, (CharaTask*)mChara, Neg_unk(Front_unk(mChara)));
    }
    return ControlBase::vf10();
}

void ControlMatHoldGuard::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
}

// inferred: grabbed by the holder (control id 0x6E); plays with the speed of the holder
ControlMatHoldPre::ControlMatHoldPre(Chara_unk* chara, Chara_unk* holder) : ControlBase(chara), m08(holder) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x6E);
    mChara->fn_8016DA4C();
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = m08->mMotion.mTracks[0].m0C;
    m->fn_80097C08(0, 0x21C, 0, 0, 1, 0.0f, 0.1f);
}

// inferred: turns towards the holder
int ControlMatHoldPre::vf0C() {
    mChara->mMotion.mTracks[0].m0C = m08->mMotion.mTracks[0].m0C;
    if ((mChara->mStatus.m0C & 0x20) == 0x20) {
        mChara->vf44();
        return 1;
    }
    atn::Vector3 dir = m08->mPos - mChara->mPos;
    dir.y = 0.0f;
    dir.RotZ_unk();
    atn::Vector3 rot = mChara->mRot;
    rot.y += 0.3f * atn::WrapAngle_unk(dir.y - rot.y);
    mChara->mRot = rot;
    return 0;
}

// inferred: held by the mat (control id 0x6E); the player shakes the remote 20 times to break free
ControlMatHold::ControlMatHold(Chara_unk* chara, int mtx) : ControlBase(chara), mMtx(mtx), mCount(20) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x6E);
    mChara->fn_8016DA4C();
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    Motion_unk* m = &mChara->mMotion;
    mChara->mF1C = mMtx;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    m->fn_80097C08(0, 0x21D, 1, 0, 1, 0.0f, 0.1f);
}

int ControlMatHold::vf0C() {
    if ((mChara->mStatus.m0C & 0x20) == 0x20) {
        mChara->mF1C = 0;
        mChara->vf44();
        return 1;
    }
    if (mChara->mType == 0) {
        lbl_8087F490->m738 = 1;
        if (mCount > 0 && lbl_8087F0A8->mInput.fn_801231D0(0)) {
            mCount--;
        }
    }
    if (mCount <= 0) {
        vf1C();
        return 1;
    }
    return 0;
}

// inferred: thrown backwards when broken free
boost::function<void()> ControlMatHold::vf10() {
    if (mCount <= 0) {
        return boost::bind(&CharaTask::fn_8015DEC0, (CharaTask*)mChara, Neg_unk(Front_unk(mChara)));
    }
    return ControlBase::vf10();
}

// inferred: the character leaves the matrix it follows, facing its z axis (name invented)
static inline void LeaveMtx_unk(ControlBase* control) {
    if (control->mChara->mF1C) {
        atn::Matrix3x4 mtx = FollowMtx_unk(control->mChara)->GetRotation_unk();
        atn::Vector3 dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        control->mChara->mRot = dir.ToRotation_unk();
        control->mChara->fn_80144710();
    }
    control->mChara->mF1C = 0;
}

// NONMATCHING: 88.8% - the original frame is 0x20 bigger (one more saved register), as in the
// constructor of ControlMatHoldRelease
void ControlMatHold::vf1C() {
    LeaveMtx_unk(this);
}

// NONMATCHING: 89.9% - frame of the inlined LeaveMtx_unk (see ControlMatHold::vf1C)
// inferred: released by the mat (control id 0x70)
ControlMatHoldRelease::ControlMatHoldRelease(Chara_unk* chara) : ControlBase(chara) {
    chara->SetControlId_unk(0x70);
    LeaveMtx_unk(this);
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x1DE, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

// inferred: pushed backwards for the first 25 frames
int ControlMatHoldRelease::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    if (m->mTracks[0].m08 < 25.0f) {
        atn::Vector3 back = Neg_unk(Front_unk(mChara));
        mChara->SetM6B8_unk(back * 0.33f);
    }
    if (Frame_unk(m) >= m->fn_80097D7C(0)) {
        return 1;
    }
    return 0;
}

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound

// inferred: caught by the thread of `other` (control id 0x74), pulled towards it
ControlThreadCatch::ControlThreadCatch(Chara_unk* chara, int other, int b)
    : ControlBase(chara), m08(0), mOther((CharaTask*)other), m1C(0x5A), m24(b) {
    if (mChara->m58C != 2) {
        mChara->m58C = 0;
    }
    mChara->SetControlId_unk(0x74);
    mChara->mColli.mFlags &= ~1;
    mChara->fn_8016DA4C();
    mChara->fn_80164DCC(1);
    if (mChara->mFlag31) {
        chara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    CharaTask* c = mChara;
    mDir = c->mPos - ((CharaTask*)other)->mPos;
    mDir.y = 0.0f;
    mDir.Normalize_unk();
    mDir *= 15.0f;
    mChara->SetM6B8_unk(mDir);
    mChara->mRot = mDir.ToRotation_unk();
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x225, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

ControlThreadCatch::~ControlThreadCatch() {
    fn_800CB5C8(&mSound, 3, 0);
}

void ControlThreadCatch::vf1C() {
    mChara->mColli.mFlags |= 1;
}

// inferred: killed by the thread (control id 0x76); the collision is turned off until the end
ControlThreadDead::ControlThreadDead(Chara_unk* chara, int a) : ControlBase(chara), m08(a) {
    chara->SetControlId_unk(0x76);
    mChara->mColli.mFlags &= ~1;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x226, 0, 0, 1, 1.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, chara, 0xA, 0x1E, 1.0f, 10.0f);
    }
}

int ControlThreadDead::vf0C() {
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0)) {
        mChara->mColli.mFlags |= 1;
        return 1;
    }
    return 0;
}

void ControlThreadDead::vf1C() {
    mChara->mColli.mFlags |= 1;
}

ControlThreadRelease::ControlThreadRelease(Chara_unk* chara) : ControlBase(chara) {
    Motion_unk* m = &chara->mMotion;
    if ((chara->mStatus.m0C & 0x20) == 0x20) {
        m->m34C = 1;
        m->mTracks[0].m20 = 1.0f;
        m->fn_80097C08(0, 0x32, 1, 0, 1, 0.0f, 0.1f);
        m->mTracks[0].m0C = 1.0f;
        mDone = 1;
    } else {
        chara->SetControlId_unk(0x77);
        m->m34C = 1;
        m->mTracks[0].m20 = 1.0f;
        m->fn_80097C08(0, 0x3C, 0, 0, 1, 0.0f, 0.1f);
        m->mTracks[0].m0C = 1.0f;
        mDone = 0;
    }
}

int ControlThreadRelease::vf0C() {
    if (mDone) {
        return 1;
    }
    Motion_unk* m = &mChara->mMotion;
    float frame = m->mTracks[0].m08;
    return frame >= m->fn_80097D7C(0);
}

} // namespace chr
