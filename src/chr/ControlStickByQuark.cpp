// inferred: controls of the characters stuck, thrust, held or thrown by enemies (vtables
// 0x8077DE00-0x8077ED00, RTTI names in .data). The classes are declared in .dol order in
// chr/ControlStickByQuark.h (their implicit destructors are emitted at the end of the file).
#include "chr/ControlStickByQuark.h"
#include "game/GameVoice.h"
#include "game/CharaTask.h"
#include "game/EnemyQuarkStand.h"
#include "game/EnemyManager.h"
#include "game/Em302Kraken.h"
#include "atn/Memory.h"
#include "game/GameUI.h"
#include "game/GimmickCommon.h"
#include "chr/ControlDropDown.h"
#include "chr/ControlDamage.h"
#include "chr/FuncPair_unk.h"
#include <boost/bind.hpp>


// inferred: input state embedded at +0x48C of the object behind lbl_8087F0A8
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero when `button` was pressed
    int fn_801231D0(int action); // inferred: action triggered
    int fn_801240B4(int action); // inferred: action held
};

struct Unk_8087F0A8 {
    unsigned char m000[0x30];
    int mFps; // 0x30: inferred: frame rate
    unsigned char m034[0x48C - 0x34];
    Input_unk mInput; // 0x48C
};

// inferred: the same object seen from the game settings (fn_800F52F8 returns lbl_8087F0A8)
struct Settings_unk {
    Input_unk* fn_80116BAC() { return &mInput; } // inferred: weak copy 0x80116BAC
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
// inferred: result of the collision tests (as in game/Em401_01_WhiteLeoPard.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);
extern "C" int fn_8004D388(void* world, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
// inferred: damage record shown by fn_80108C10 (as DaemonDamage_unk of game/Em203GreaterDaemon.cpp)
struct Damage_unk {
    Damage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C : 4;
    unsigned int m1C_rest : 28;
};
extern "C" void fn_80108C10(Effect_unk* effect, Damage_unk* damage, const atn::Vector3& pos, CharaTask* attacker,
                            CharaTask* chara, int b, int c);
// inferred: the damage computation (game/DamageCalc_unk.h, with the record above)
struct DamageParam_unk;
extern "C" DamageParam_unk lbl_807C6B90;
extern "C" int fn_8003EA3C(Damage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* target,
                           unsigned int flags, const DamageParam_unk* param, const atn::Vector3* pos,
                           const atn::Vector3* rot);
extern "C" int swprintf(wchar_t* buf, unsigned long size, const wchar_t* format, ...);
extern "C" void* lbl_8087F8A8; // inferred: popup text manager
// inferred: start yaw of the climb (a zero float in .sdata in the original, only read here; ours goes to
// .sbss without explicit_zero_data)
float lbl_8087DA00 = 0.0f;
extern "C" void fn_8054D794(void* obj, int arg1, const wchar_t* text, const atn::Vector3* pos, int arg4, int arg5,
                            int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12);

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
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
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
    Mtx r;
    Mtx t;
    PSMTXTrans(t, v.x, v.y, v.z);
    PSMTXConcat(m.m, t, r);
    __MtxCopy(m.m, r);
}

static inline void TranslateXYZ_unk(atn::Matrix3x4& m, float x, float y, float z) {
    TranslateMtx_unk(m, atn::Vector3(x, y, z));
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

// NONMATCHING: 99.87% - the original loads mDone before saving r31 (as in a const method) and swaps
// r8/r9 in the copies of the bound vector
// inferred: thrown sideways when released, gets up otherwise
boost::function<void()> ControlStickByQuark::vf10() const {
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
        lbl_8087F498->fn_803EA77C(mChara, 0x20, 0, 1.0f, 10.0f);
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
        lbl_8087F498->fn_803EA77C(mChara, 0x20, 0, 1.0f, 10.0f);
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
        lbl_8087F498->fn_803EA77C(mChara, 0x20, 0, 1.0f, 10.0f);
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
atn::Vector3 ControlThrustForKraken::vf14() const {
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
atn::Vector3 ControlThrustForGhoul::vf14() const {
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

// NONMATCHING: 99.84% - stack slots of the lerp temporaries; guards while held by the mat (the player shakes the remote to break free)
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
    TranslateXYZ_unk(mtx, -5.0f, 0.0f, -9.0f);
    atn::Vector3 pos = mtx.GetTrans_unk();
    mChara->mPos = LerpTo_unk(mChara->mPos, pos, 0.2f);
    mtx.m[0][3] = 0.0f;
    mtx.m[1][3] = 0.0f;
    mtx.m[2][3] = 0.0f;
    atn::Vector3 dir;
    dir = atn::Vector3(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(mtx.m, &dir, &dir);
    mChara->mRot = dir.ToRotation_unk();
    if (mCount <= 0) {
        lbl_8087EFA8->m3A4 = 1.0f;
        return 1;
    }
    return 0;
}

// inferred: thrown backwards when broken free
boost::function<void()> ControlMatHoldGuard::vf10() const {
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
boost::function<void()> ControlMatHold::vf10() const {
    if (mCount <= 0) {
        return boost::bind(&CharaTask::fn_8015DEC0, (CharaTask*)mChara, Neg_unk(Front_unk(mChara)));
    }
    return ControlBase::vf10();
}

// inferred: leaves the matrix of the mat, facing its z axis
void ControlMatHold::vf1C() {
    if (mChara->mF1C) {
        atn::Matrix3x4 mtx = FollowMtx_unk(mChara)->GetRotation_unk();
        atn::Vector3 dir;
        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        mChara->mRot = dir.ToRotation_unk();
        mChara->fn_80144710();
    }
    mChara->mF1C = 0;
}

// inferred: released by the mat (control id 0x70)
ControlMatHoldRelease::ControlMatHoldRelease(Chara_unk* chara) : ControlBase(chara) {
    chara->SetControlId_unk(0x70);
    if (mChara->mF1C) {
        atn::Matrix3x4 mtx = FollowMtx_unk(mChara)->GetRotation_unk();
        atn::Vector3 dir;
        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        mChara->mRot = dir.ToRotation_unk();
        mChara->fn_80144710();
    }
    mChara->mF1C = 0;
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

// inferred: damaged by the mat (control id 0x6F)
ControlMatHoldDamage::ControlMatHoldDamage(Chara_unk* chara) : ControlBase(chara) {
    chara->SetControlId_unk(0x6F);
    if (mChara->mF1C) {
        atn::Matrix3x4 mtx = FollowMtx_unk(mChara)->GetRotation_unk();
        atn::Vector3 dir;
        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        mChara->mRot = dir.ToRotation_unk();
        mChara->fn_80144710();
    }
    mChara->mF1C = 0;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x21E, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: draft - falls back for 64 frames, then takes a 10000 damage
int ControlMatHoldDamage::vf0C() {
    CharaTask* c = mChara;
    Motion_unk* m = &c->mMotion;
    if (m->mTracks[0].m08 < 64.0f) {
        int fps = lbl_8087F0A8->mFps;
        atn::Vector3 move(0.0f, -98.1f / (float)(fps * fps), -0.46875f);
        atn::Matrix3x4 rot;
        rot.SetIdentity();
        if (0.0f != c->mRot.z) {
            atn::Matrix3x4::RotateAxis_unk(rot, 'z', c->mRot.z);
        }
        if (0.0f != c->mRot.y) {
            atn::Matrix3x4::RotateAxis_unk(rot, 'y', c->mRot.y);
        }
        if (0.0f != c->mRot.x) {
            atn::Matrix3x4::RotateAxis_unk(rot, 'x', c->mRot.x);
        }
        PSMTXMultVec(rot.m, &move, &move);
        CollResult_unk result;
        CharaTask::Sphere_unk sphere = mChara->fn_80176548(mChara->mPos);
        fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000000, mChara->GetColli_unk(), 0, sphere.mRadius);
        atn::Vector3 pos = result.m10;
        pos -= mChara->m5A4;
        pos.y -= sphere.mRadius;
        mChara->mPos = pos;
    }
    if (Frame_unk(m) >= m->fn_80097D7C(0)) {
        Damage_unk damage;
        damage.m04 = 10000;
        damage.m00 = 1;
        fn_80108C10(lbl_8087F048, &damage, mChara->mPos + atn::Vector3(0.0f, 15.0f, 0.0f), 0, mChara, 0, 0);
        mChara->vf44();
        return 1;
    }
    return 0;
}

// NONMATCHING: 89.6% - copies of the direction vector and the order of the velocity products
// inferred: thrown by the mat towards `target` (control id 0x71), landing after `frames` frames
ControlThrown::ControlThrown(Chara_unk* chara, const atn::Vector3& target, int frames)
    : ControlBase(chara), mTarget(target), mFrames(frames), mCount(15), mDone(0) {
    chara->SetControlId_unk(0x71);
    mChara->fn_8016DA4C();
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag5) {
        mChara->mFlag5 = 0;
    }
    float n = (float)frames;
    mVel = (mTarget - mChara->mPos) * (1.0f / n);
    mVel.y -= 0.5f * (n * (0.5f * (-98.1f / (float)(lbl_8087F0A8->mFps *
                                                     lbl_8087F0A8->mFps))));
    atn::Vector3 dir = mTarget - mChara->mPos;
    dir.y = 0.0f;
    dir.Normalize_unk();
    mChara->mRot = dir.ToRotation_unk();
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    if (mChara->m1208) {
        m->fn_80097C08(0, 0x221, 1, 0, 1, 0.0f, 0.1f);
        mState = 0;
    } else {
        m->fn_80097C08(0, 0x220, 1, 0, 1, 0.0f, 0.1f);
        mState = 1;
    }
    if (lbl_8087F498 != 0) {
        lbl_8087F498->fn_803EA77C(mChara, 0x10, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 92.3% - flight, then landing (the player can shake the remote to land on his feet)
int ControlThrown::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    mChara->mPos = mChara->mPos + mVel;
    int fps = lbl_8087F0A8->mFps;
    mVel.y += lbl_8087EFA8->m3A4 * (0.5f * (-98.1f / (float)(fps * fps)));
    mFrames--;
    switch (mState) {
    case 0:
        if (mFrames < 0) {
            if (mChara->m1208) {
                GimmickCommon* gimmick = (GimmickCommon*)mChara->m1208;
                mChara->fn_801750FC();
                GimmickMsg_unk msg(6);
                msg.mVec = mTarget;
                msg.m04 = 1;
                gimmick->vf40(&msg);
            }
            return 1;
        }
        break;
    case 1:
        if (mChara->mType == 0 && mCount > 0 && mFrames > 30) {
            lbl_8087F490->m728 = 1;
            if (lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
                mCount--;
            }
            if (mCount <= 0) {
                mDone = 1;
            }
        }
        if (mDone && m->mTracks[0].m08 >= 30.0f) {
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->mTracks[0].m0C = 1.0f;
            m->fn_80097C08(0, 0x222, 0, 0, 1, 0.0f, 0.1f);
            mState = 2;
        }
        break;
    case 2:
        if (Frame_unk(m) >= m->fn_80097D7C(0)) {
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->mTracks[0].m0C = 1.0f;
            m->fn_80097C08(0, 0x68, 1, 0, 1, 0.0f, 0.1f);
            mState = 3;
        }
        break;
    case 3:
        if (mFrames < 0) {
            return 1;
        }
        break;
    }
    return 0;
}

// NONMATCHING: 60% - psq_st before the stfs of z in the original (same as ControlWallSlash::vf14)
atn::Vector3 ControlThrown::vf14() const {
    return mTarget;
}

// inferred: carried away on the matrix `mtx` for `frames` frames (control id 0x72)
ControlThrownAway::ControlThrownAway(Chara_unk* chara, int mtx, int frames)
    : ControlBase(chara), mMtx(mtx), mFrames(frames) {
    chara->SetControlId_unk(0x72);
    mChara->mF1C = mMtx;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x224, 1, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 87.4% - the original stores the vtable pointer at the start of the destructor
ControlThrownAway::~ControlThrownAway() {
    if (mChara) {
        mChara->mF1C = 0;
    }
}

// NONMATCHING: 97.1% - the original copies the facing vector to mRot through the stack
int ControlThrownAway::vf0C() {
    if (mChara->mF1C) {
        atn::Matrix3x4 mtx = FollowMtx_unk(mChara)->GetRotation_unk();
        atn::Vector3 dir;
        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        dir.RotZ_unk();
        mChara->mRot = dir;
        mChara->mPos = FollowMtx_unk(mChara)->GetTrans_unk();
    }
    if (--mFrames < 0) {
        return 1;
    }
    return 0;
}

atn::Vector3 ControlThrownAway::vf14() const {
    atn::Vector3 pos = mChara->mPos;
    pos.y -= 10.0f;
    return pos;
}

// NONMATCHING: 96.2% - copies of the facing vector (as in ControlThrownAway::vf0C)
// inferred: thrown and caught again on the matrix `mtx` (control id 0x73)
ControlThrownAvoid::ControlThrownAvoid(Chara_unk* chara, int mtx, const atn::Vector3& dir)
    : ControlBase(chara), mMtx(mtx), mDir(dir) {
    chara->SetControlId_unk(0x73);
    mChara->mF1C = mMtx;
    if (mChara->mF1C) {
        atn::Matrix3x4 mtx = FollowMtx_unk(mChara)->GetRotation_unk();
        atn::Vector3 dir;
        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        dir.RotZ_unk();
        mChara->mRot = dir;
        mChara->mPos = FollowMtx_unk(mChara)->GetTrans_unk();
    }
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x223, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 97.5% - follows the matrix for 8 frames, then flies along mDir
int ControlThrownAvoid::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    if (Frame_unk(m) >= m->fn_80097D7C(0)) {
        return 1;
    }
    if (m->mTracks[0].m08 >= 8.0f) {
        mChara->mF1C = 0;
        float speed = mDir.Length_unk();
        atn::Vector3 dir = mDir;
        dir.fn_80139F60();
        mChara->fn_801426A4(&dir, speed, 1.5f);
    } else if (mChara->mF1C) {
        atn::Matrix3x4 mtx = FollowMtx_unk(mChara)->GetRotation_unk();
        atn::Vector3 dir;
        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        dir.RotZ_unk();
        mChara->mRot = dir;
        mChara->mPos = FollowMtx_unk(mChara)->GetTrans_unk();
    }
    return 0;
}

// NONMATCHING: 93.7% - scheduling of the constants and of mChara
// inferred: point below the feet, turned with the character
atn::Vector3 ControlThrownAvoid::vf14() const {
    atn::Vector3 v(0.0f, -5.0f, 0.5f);
    Mtx m;
    PSMTXRotRad(m, 'y', mChara->mRot.y);
    PSMTXMultVec(m, &v, &v);
    v += mChara->mPos;
    return v;
}

// inferred: falls along mDir afterwards
boost::function<void()> ControlThrownAvoid::vf10() const {
    ControlDropDown* next = new ((MemoryTag)0, "", "", 0) ControlDropDown(mChara, mDir);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

// inferred: held by the skeleton king on its weapon node (control id 0x71)
ControlCaughtSkelton::ControlCaughtSkelton(Chara_unk* chara, CharaTask* holder) : ControlBase(chara), mHolder(holder) {
    chara->SetControlId_unk(0x71);
    mChara->fn_8016DA4C();
    if (mChara->mFlag31) {
        chara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    m->fn_80097C08(0, 0x28, 1, 0, 1, 0.0f, 0.1f);
    mNode = (atn::Matrix3x4*)mHolder->mMotion.GetNodeMtx_unk("rightwepoint");
    mChara->mPos = mNode->GetTrans_unk();
}

// NONMATCHING: 98.5% - r4/r5/r6 order of the node, the temporary and mChara
int ControlCaughtSkelton::vf0C() {
    atn::Matrix3x4* node = mNode;
    mChara->mPos = node->GetTrans_unk();
    mChara->mRot.Assign_unk(mHolder->mRot);
    return 0;
}

// inferred: thrown by the skeleton king at `target`, head first (control id 0x71); `data` is the hit
ControlThrownSkelton::ControlThrownSkelton(Chara_unk* chara, const atn::Vector3& target, HitInfo_unk* data)
    : ControlBase(chara), mHit(data), mTarget(target) {
    chara->SetControlId_unk(0x71);
    mChara->fn_8016DA4C();
    if (mChara->mFlag31) {
        chara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    m->fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.2f);
    CharaTask* c = mChara;
    mDir = mTarget - c->mPos;
    mDir.Normalize_unk();
    atn::Vector3 rot = mDir.ToRotation_unk();
    rot.x += 1.5707964f;
    mChara->mRot = rot;
    mChara->mF1C = 0;
}

// NONMATCHING: 93.1% - flies along mDir until it hits a wall; the hit fires the bullet mHit
int ControlThrownSkelton::vf0C() {
    CollResult_unk result;
    atn::Vector3 from = mChara->mPos + mDir * 5.0f;
    atn::Vector3 to = mChara->mPos + mDir * 30.0f;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, mChara->fn_80179D44(), &mChara->mColli, 0)) {
        mChara->mPos = result.m04;
        mDir.y = 0.0f;
        mChara->mRot = mDir.ToRotation_unk();
        ShootHit_unk(lbl_8087F048, mChara, mHit, result.m04, &mChara->mRot);
        return 1;
    }
    mChara->mPos = mChara->mPos + mDir * 10.0f;
    return 0;
}

// inferred: breaks against the wall
boost::function<void()> ControlThrownSkelton::vf10() const {
    ControlThrownBreakSkelton* next = new ((MemoryTag)0, "", "", 0) ControlThrownBreakSkelton(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

// inferred: the skeleton breaks apart (control id 0x71)
ControlThrownBreakSkelton::ControlThrownBreakSkelton(Chara_unk* chara) : ControlBase(chara) {
    chara->SetControlId_unk(0x71);
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    m->fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
}

int ControlThrownBreakSkelton::vf0C() {
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0)) {
        return 1;
    }
    mChara->vf34(mChara->mRot, 0, 0.0f, 1.0f);
    return 0;
}

// inferred: revives as a skeleton
boost::function<void()> ControlThrownBreakSkelton::vf10() const {
    return boost::bind(&CharaTask::fn_8015EFAC, (CharaTask*)mChara, (CharaTask::CONTROL_MODE)1);
}

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound

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

// inferred: pulled by the thread, then struggles (the player shakes the remote to break it)
int ControlThreadCatch::vf0C() {
    if (m08 == 0) {
        mChara->fn_8013CB68(mDir.ToRotation_unk(), 0, 0.0f, mChara->m568);
        Motion_unk* m = &mChara->mMotion;
        float frame = mChara->mMotion.mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->fn_80097C08(0, 0x32, 1, 0, 1, 0.0f, 0.1f);
            m->mTracks[0].m0C = 1.0f;
            m08 = 1;
            mChara->SetControlId_unk(0x75);
            if (m24) {
                mSound = fn_800C344C("SE_EM202_013", &mChara->mPos, 0, -1, 1.0f);
            }
        }
    } else if (m08 == 1) {
        if (mChara->mType == 0) {
            lbl_8087F490->m728 = 1;
            if (lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
                m1C -= 5;
            }
        }
        if (m1C <= 0) {
            mChara->mColli.mFlags |= 1;
            return 1;
        }
        fn_800CB6E4(&mSound, &mChara->mPos);
    }
    return 0;
}

void ControlThreadCatch::vf1C() {
    mChara->mColli.mFlags |= 1;
}

// inferred: released afterwards
boost::function<void()> ControlThreadCatch::vf10() const {
    ControlThreadRelease* next = new ((MemoryTag)0, "", "", 0) ControlThreadRelease(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
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
        lbl_8087F498->fn_803EA77C(chara, 0xA, 0x1E, 1.0f, 10.0f);
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

// inferred: bound by a shade (control id 0x7A) for `time` frames; the player breaks free with `count`
// swings
ControlBoundShade::ControlBoundShade(Chara_unk* chara, int time, int b, int count)
    : ControlBase(chara), mState(0), mTime(time), mTimer(0), mCount(count), m18(b) {
    chara->SetControlId_unk(0x7A);
    if (mChara->mType == 0) {
        mChara->m564 = (CharaTask::CONTROL_MODE)1;
    } else {
        mChara->m564 = (CharaTask::CONTROL_MODE)2;
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->IsFlag5_unk()) {
        mChara->mFlag5 = 0;
    }
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x84, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

int ControlBoundShade::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    if (mState == 0) {
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            mState++;
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->fn_80097C08(0, 0x1E4, 1, 0, 1, 0.0f, 0.1f);
            m->mTracks[0].m0C = 1.0f;
        }
    } else {
        if (++mTimer > mTime) {
            return 1;
        }
        if (mChara->mType == 0) {
            lbl_8087F490->m728 = 1;
            if (lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
                mCount--;
            }
            if (mCount <= 0) {
                return 1;
            }
        }
    }
    return 0;
}

// inferred: gets up and fires the bullet m18 at itself when not broken free in time
boost::function<void()> ControlBoundShade::vf10() const {
    if (mCount > 0) {
        return DoubleFunction(boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false),
                            boost::bind(&CharaTask::fn_80164454, (CharaTask*)mChara, m18, (CharaTask*)mChara, (CharaTask*)mChara, 0));
    }
    return boost::bind(&CharaTask::fn_8016E970, (CharaTask*)mChara, mChara->m564);
}

// NONMATCHING: 98.2% - float register numbering (f6/f7 for the int-to-float constant and the time scale)
int ControlThrownSimple::vf0C() {
    CharaTask* c = mChara;
    c->mPos = c->mPos + mVel;
    int fps = lbl_8087F0A8->mFps;
    mVel.y += (float)lbl_8087EFA8->m3A4 * (0.5f * (-98.1f / (float)(fps * fps)));
    mFrames--;
    return 0;
}

// NONMATCHING: 60% - the original stores x/y with psq_st before the stfs of z
atn::Vector3 ControlThrownSimple::vf14() const {
    return mTarget;
}

ControlAbsorbedHP::ControlAbsorbedHP(Chara_unk* chara, CharaTask* other, HitInfo_unk* hit)
    : ControlBase(chara), mCount(30), mTimer(3) {
    chara->m58C = 0;
    mChara->SetControlId_unk(0x82);
    mOther = other;
    mHit = hit;
    atn::Vector3 dir(0.0f, 0.0f, 15.0f);
    RotY_unk(dir, other->mRot.y);
    CharaTask* o = mOther;
    mTarget = o->mPos + dir;
    atn::Vector3 rot(0.0f, 3.1415927f + o->mRot.y, 0.0f);
    mChara->mRot = rot;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    m->fn_80097C08(0, 0x227, 0, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 99.95% - only the label of the L"%d" string (fixed by pairdata)
int ControlAbsorbedHP::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    if (mChara->mStatus.IsState_unk(0x20)) {
        mChara->vf44();
        return 1;
    }
    if (m->mTracks[0].m00 == 0x227) {
        if (Frame_unk(m) >= m->fn_80097D7C(0)) {
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->mTracks[0].m0C = 1.0f;
            m->fn_80097C08(0, 0x228, 1, 0, 1, 0.0f, 0.1f);
        }
        atn::Vector3 pos = mChara->mPos;
        pos = LerpTo_unk(pos, mTarget, 0.3f);
        mChara->mPos = pos;
    } else {
        if (--mTimer < 0) {
            mTimer = 30;
            Damage_unk damage;
            fn_8003EA3C(&damage, mHit, mOther, mChara, 0, &lbl_807C6B90, 0, 0);
            atn::Vector3 pos = mChara->fn_80148B0C(0)->m04.mCenter;
            pos += atn::Vector3(0.0f, 15.0f, 0.0f);
            fn_80108C10(lbl_8087F048, &damage, pos, mOther, mChara, 0, 0);
            mOther->mStatus.fn_8012DF7C(-damage.m04, 0, 0);
            pos = mOther->fn_80148B0C(0)->m04.mCenter;
            pos += atn::Vector3(0.0f, 15.0f, 0.0f);
            wchar_t text[0x20];
            swprintf(text, 0x20, L"%d", damage.m04);
            fn_8054D794(lbl_8087F8A8, 1, text, &pos, 0, 0, 1, 0, 0, 0, 1, -1, -1);
        }
        if (mChara->mType == 0) {
            lbl_8087F490->m728 = 1;
            if (lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
                mCount--;
            }
        }
    }
    return mCount <= 0;
}

inline ControlAbsorbedHPReleased::ControlAbsorbedHPReleased(Chara_unk* chara) : ControlBase(chara) {
    chara->SetControlId_unk(0x83);
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x229, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 99.86% - the original loads mCount before saving r31 (as StickByQuark::vf10)
boost::function<void()> ControlAbsorbedHP::vf10() const {
    if (mCount > 0) {
        atn::Vector3 dir(0.0f, 0.0f, -1.0f);
        RotY_unk(dir, mChara->mRot.y);
        ControlKnockdown* next = new ((MemoryTag)0, "", "", 0) ControlKnockdown(mChara, dir, -1, 0);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    ControlAbsorbedHPReleased* next = new ((MemoryTag)0, "", "", 0) ControlAbsorbedHPReleased(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

int ControlAbsorbedHPReleased::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    return Frame_unk(m) >= m->fn_80097D7C(0);
}

ControlCliffhangZanGurg::ControlCliffhangZanGurg(Chara_unk* chara, const atn::Vector3 from,
                                                 const atn::Vector3 to, const atn::Vector3 face)
    : ControlBase(chara) {
    mFrom = from;
    mTo = to;
    mState = 0;
    mCount = 0;
    mFace = face;
    chara->SetControlId_unk(0x85);
    mChara->fn_8016DA4C();
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->IsFlag5_unk() != 0) {
        mChara->mFlag5 = 0;
    }
    atn::Vector3 dir;
    dir = mTo - mFrom;
    PSVECNormalize(&dir, &dir);
    mDir = dir;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x3A, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

int ControlCliffhangZanGurg::vf0C() {
    CharaTask* c = mChara;
    Motion_unk* m = &c->mMotion;
    if (mState == 0) {
        atn::Vector3 pos = LerpTo_unk(mFrom, mTo, Frame_unk(m) / 20.0f);
        c->mPos = pos;
        atn::Vector3 rot = mChara->mRot;
        atn::Vector3 dir = LerpTo_unk(mDir, mFace, Frame_unk(m) / 20.0f);
        rot.y = dir.ToRotationZ_unk().y;
        mChara->mRot = rot;
        if (Frame_unk(m) >= 20.0f) {
            mState = 1;
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->fn_80097C08(0, 0x22A, 0, 0, 1, 0.0f, 0.1f);
            m->mTracks[0].m0C = 1.0f;
        }
    } else if (mState == 1) {
        lbl_8087F490->m738 = 1;
        if (fn_800A555C(lbl_8087EF70, 0, 4)) {
            mCount++;
        }
        mChara->mPos = mTo;
        if (Frame_unk(m) >= m->fn_80097D7C(0)) {
            mState = 2;
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->fn_80097C08(0, 0x22B, 0, 0, 1, 0.0f, 0.1f);
            m->mTracks[0].m0C = 1.0f;
        }
    } else if (mState == 2) {
        lbl_8087F490->m738 = 1;
        if (fn_800A555C(lbl_8087EF70, 0, 4)) {
            mCount++;
        }
    }
    return 0;
}

// NONMATCHING: 75.3% - the original loads mChara before saving r31 (as StickByQuark::vf10)
atn::Vector3 ControlCliffhangZanGurg::vf14() const {
    atn::Vector3 pos = ((atn::Matrix3x4*)mChara->mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
    pos.y -= 15.0f;
    return pos;
}

ControlEndCliffhangZanGurg::ControlEndCliffhangZanGurg(Chara_unk* chara, const atn::Vector3 target)
    : ControlBase(chara), mTarget(target) {
    chara->SetControlId_unk(0x86);
    mChara->fn_8016DA4C();
    mOffset = mChara->mPos - mTarget;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x22C, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 99.7% - stack slots of offset/pos/rot
int ControlEndCliffhangZanGurg::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    float frame = Frame_unk(m);
    float end = m->fn_80097D7C(0);
    float t = frame / end;
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', t * (2.0943952f - lbl_8087DA00) + lbl_8087DA00);
    atn::Vector3 offset = mOffset;
    PSMTXMultVec(mtx, &offset, &offset);
    atn::Vector3 pos = mTarget + offset;
    atn::Vector3 rot = (mTarget - pos).ToRotationZ_unk();
    mChara->mPos = pos;
    mChara->mRot = rot;
    if (frame >= end) {
        return 1;
    }
    return 0;
}

// NONMATCHING: 86.5% - the original loads mChara before saving r31; node pointer in r4
atn::Vector3 ControlEndCliffhangZanGurg::vf14() const {
    Motion_unk* m = &mChara->mMotion;
    float frame = Frame_unk(m);
    float end = m->fn_80097D7C(0);
    atn::Vector3 pos = ((atn::Matrix3x4*)m->GetNodeMtx_unk("hips"))->GetTrans_unk();
    pos.y -= 15.0f;
    pos = LerpTo_unk(pos, mChara->mPos, frame / end);
    return pos;
}

ControlStickZanGurgStand::ControlStickZanGurgStand(Chara_unk* chara, const atn::Vector3 pos,
                                                   const atn::Vector3 dir)
    : ControlBase(chara), mPos(pos), mDir(dir) {
    chara->SetControlId_unk(0x87);
    mChara->fn_8016DA4C();
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->IsFlag5_unk() != 0) {
        mChara->mFlag5 = 0;
    }
    mChara->mRot = mDir.ToRotation_unk();
    mChara->mPos = mPos;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x22D, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

int ControlStickZanGurgStand::vf0C() {
    CharaTask* c = mChara;
    Motion_unk* m = &c->mMotion;
    float frame = Frame_unk(m);
    if (frame < 45.0f) {
        atn::Vector3 pos = c->mPos;
        pos += mDir * (20.0f / 45.0f);
        c->mPos = pos;
    } else if (45.0f <= frame && frame < 70.0f) {
        atn::Vector3 rot = c->mRot;
        rot.y += 0.12566371f;
        c->mRot = rot;
    } else if (70.0f <= frame && frame < 80.0f) {
        atn::Vector3 pos = c->mPos;

        pos += mDir * 3.0f;
        mChara->mPos = pos;
    }
    return frame >= m->fn_80097D7C(0);
}

// NONMATCHING: 75.3% - the original loads mChara before saving r31 (as StickByQuark::vf10)
atn::Vector3 ControlStickZanGurgStand::vf14() const {
    atn::Vector3 pos = ((atn::Matrix3x4*)mChara->mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
    pos.y -= 15.0f;
    return pos;
}

boost::function<void()> ControlStickZanGurgStand::vf10() const {
    ControlGetUp* next = new ((MemoryTag)0, "", "", 0) ControlGetUp(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

ControlStickZanGurgCliff::ControlStickZanGurgCliff(Chara_unk* chara, const atn::Vector3 pos,
                                                   const atn::Vector3 dir)
    : ControlBase(chara), mPos(pos), mDir(dir) {
    chara->SetControlId_unk(0x88);
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x22E, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

int ControlStickZanGurgCliff::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    float frame = Frame_unk(m);
    if (atn::Abs_unk(frame - 45.0f) < 1.1920929e-7f) {
        m->m34C = 1;
        m->mTracks[0].m20 = 1.0f;
        m->fn_80097C08(0, 0x22D, 0, 0, 1, 1.0f, 0.1f);
        m->mTracks[0].m0C = 1.0f;
        m->mTracks[0].m08 = 45.0f;
    }
    if (frame < 45.0f) {
        atn::Vector3 pos = mChara->mPos;
        pos += mDir * (20.0f / 45.0f);
        mChara->mPos = pos;
    } else if (45.0f <= frame && frame < 70.0f) {
        atn::Vector3 rot = mChara->mRot;
        rot.y += 0.12566371f;
        mChara->mRot = rot;
    } else if (70.0f <= frame && frame < 80.0f) {
        atn::Vector3 pos = mChara->mPos;
        pos += mDir * 3.0f;
        mChara->mPos = pos;
    } else if (80.0f <= frame && frame < 82.0f) {
        mChara->vf44();
    }
    return Frame_unk(m) >= m->fn_80097D7C(0);
}

// NONMATCHING: 75.3% - the original loads mChara before saving r31 (as StickByQuark::vf10)
atn::Vector3 ControlStickZanGurgCliff::vf14() const {
    atn::Vector3 pos = ((atn::Matrix3x4*)mChara->mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
    pos.y -= 15.0f;
    return pos;
}

boost::function<void()> ControlStickZanGurgCliff::vf10() const {
    ControlGetUp* next = new ((MemoryTag)0, "", "", 0) ControlGetUp(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

ControlReflectLaser::~ControlReflectLaser() {
    if (lbl_8087F3C0) {
        fn_80239DAC(lbl_8087F3C0, this, -2, 0);
    }
}

// inferred: helpers called by ControlReflectLaser::vf0C (over the inlining budget, so all of them are
// weak copies; the ones first emitted here are at 0x801A03E0-0x801A0408). Names invented.
extern "C" atn::Vector3 lbl_807C7030;   // inferred: zero vector
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
// inferred: mtx = mtx * rot_z * rot_y * rot_x (weak copy 0x800F80B8 of game/BattleTask.cpp)
extern "C" inline void fn_800F80B8(MtxPtr mtx, const atn::Vector3* rot) {
    ((atn::Matrix3x4*)mtx)->RotateZYX_unk(*rot);
}
// inferred: mtx = mtx * scale (weak copy 0x800F8290 of game/BattleTask.cpp)
extern "C" inline void fn_800F8290(MtxPtr mtx, float x, float y, float z) {
    Mtx s;
    PSMTXScale(s, x, y, z);
    Mtx t;
    PSMTXConcat(mtx, s, t);
    __MtxCopy(mtx, t);
}
// inferred: the hit object of a collision result
struct CollObj_unk {
    int m00;
    int m04;
    unsigned int m08;
    CharaTask* m0C; // inferred: owner
};
extern "C" inline EnemyManager* fn_801A03E0() {
    return EnemyManager::sInstance;
}
extern "C" inline float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
extern "C" inline EnemyTask* fn_801A03EC(EnemyTask* chara) {
    return chara->mNext;
}
// inferred: asserts of the debug build, empty in release: they only count for the inline budget (with them
// ControlReflectLaser::vf0C goes over it and calls every inline out of line, as the original)
#define LASER_ASSERT_UNK(x) ((void)0)
extern "C" inline int fn_801A03F4(CollObj_unk* obj) {
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0); LASER_ASSERT_UNK(obj != 0);
    return (obj->m08 >> 1) & 1; // inferred: the object is a character
}
extern "C" inline CharaTask* fn_801A0400(CollObj_unk* obj) {
    return obj->m0C;
}
extern "C" inline EnemyTask* fn_801A0408(EnemyManager* mgr) {
    return mgr->mHead;
}
// inferred: rotation matrix from euler angles (weak copy 0x800109E0 of Event/Puppeteer_CharaTask.cpp)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
// inferred: translation matrix by value (weak copy 0x800F80A8 of game/BattleTask.cpp)
extern "C" inline atn::Matrix3x4 fn_800F80A8(const atn::Vector3* pos) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, pos->x, pos->y, pos->z);
    return m;
}
extern "C" inline Settings_unk* fn_800F52F8() {
    return (Settings_unk*)lbl_8087F0A8;
}
} // namespace chr
// inferred: global accessors (weak copies of other files, outside chr)
inline Effect_unk* fn_8013A194() {
    return lbl_8087F048;
}
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
namespace chr {

// over the inline budget as the original (empty asserts in fn_801A03F4): every inline call is a bl and the
// weak accessors 0x801A03E0-0x801A0408 follow it
int ControlReflectLaser::vf0C() {
    if (mDone) {
        return 1;
    }
    int ret = 0;
    atn::Model* model = mChara->GetModel_unk();
    float end = model->fn_80097D7C(0) - 4.0f;
    if (model->GetMotionFrame(0) > end) {
        model->SetMotionSpeed(0, 0.02f);
    }
    mChara->SetM6B8_unk(mChara->fn_8014052C() * -0.05f);
    mChara->vf34(*mChara->GetRot_unk(), 0, 0.0f, 1.0f);
    atn::Vector3 pos(*mChara->fn_8013C38C());
    pos.y += 13.0f;
    pos += mChara->fn_8014052C() * 4.0f;
    atn::Vector3 rot = mDir.ToRotation_unk();
    if (mChara->IsPlayer()) {
        if (mChara->fn_80178078() > 0.4f) {
            atn::Vector3 front = mChara->fn_80178018();
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            dir.fn_80011410(fn_800109E0(front));
            float best = 1e16f;
            EnemyTask* target = 0;
            for (EnemyTask* e = fn_801A0408(fn_801A03E0()); e != 0; e = fn_801A03EC(e)) {
                if (e->CanAct_unk()) {
                    atn::Vector3 d = *e->fn_8013C38C() - pos;
                    float len = d.Length_unk();
                    d.y = 0.0f;
                    d.Normalize_unk();
                    if (fn_801A03E8(dir, d) > 0.5f && len < best) {
                        best = len;
                        target = e;
                    }
                }
            }
            if (target) {
                atn::Vector3 d = *target->fn_8013C38C() - pos;
                d.Normalize_unk();
                atn::Vector3 diff = d.ToRotation_unk() - rot;
                diff.Apply_unk(atn::WrapAngle_unk);
                float maxYaw = 0.04712389f;
                float maxPitch = 0.02199115f;
                diff.y = diff.y >= 0.0f ? (diff.y < maxYaw ? diff.y : maxYaw) : (diff.y > -maxYaw ? diff.y : -maxYaw);
                diff.x = diff.x >= 0.0f ? (diff.x < maxPitch ? diff.x : maxPitch)
                                        : (diff.x > -maxPitch ? diff.x : -maxPitch);
                rot += diff;
                mDir.Set_unk(0.0f, 0.0f, 1.0f);
                mDir.fn_80011410(fn_800109E0(rot));
            }
        }
        CollResult_unk result;
        atn::Vector3 from(pos);
        atn::Vector3 to = pos + mDir * 1000.0f;
        if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 6, mChara->GetColli_unk(), 0)) {
            CollObj_unk* obj = (CollObj_unk*)result.m38;
            if (obj != 0 && fn_801A03F4(obj)) {
                CharaTask* hit = fn_801A0400((CollObj_unk*)result.m38);
                if (hit != 0 && hit->fn_8013A158() && hit->CanAct_unk()) {
                    fn_800FAB80(fn_8013A194(), mChara, fn_80219E6C(0x76F), fn_800F8548(fn_8013A194()), &result.m04,
                                &lbl_807C7030, 0, 0x1E, -1, -1, 0.0f, 1.0f);
                }
            }
        }
    }
    if (mChara->IsPlayer() && !fn_800F52F8()->fn_80116BAC()->fn_801240B4(7)) {
        ret = 1;
    }
    mLength = 0.1f + mLength < 100.0f ? 0.1f + mLength : 100.0f;
    mMtx = fn_800F80A8(&pos);
    fn_800F80B8(mMtx.m, &rot);
    fn_800F8290(mMtx.m, 1.0f, 1.0f, mLength);
    return ret;
}

} // namespace chr
