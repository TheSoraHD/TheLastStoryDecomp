#include "game/HomingBullet.h"
#include "game/CharaTask.h"
#include "game/ReplayBuffer.h"
#include "math/VecAssign_unk.h"
#include "chr/ControlAttackBase.h"
#include "game/GameTask.h"
#include "game/CharaStatus.h"
#include "game/PlayerTask.h"
#include "game/CharaManager.h"

extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);

// inferred: translation of a matrix (name invented)
inline atn::Vector3 GetTrans_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: float cosine (name invented)
inline float CosF_unk(float x) {
    return cos(x);
}

// inferred: game parameters (only the frame time step)
struct HomingParams_unk {
    unsigned char m000[0x3A4];
    float mTimeScale;
};
extern "C" HomingParams_unk* lbl_8087EFA8;

extern "C" GameTask* lbl_8087F430;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" void fn_800F8574(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a, float b);
// inferred: damage record shown by fn_80108C10 (as StatusDamage_unk of game/CharaStatus.cpp)
struct HomingDamage_unk {
    HomingDamage_unk() : m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
        m00 = 1;
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
extern "C" void fn_80108C10(BulletMgr_unk* mgr, HomingDamage_unk* damage, const atn::Vector3& pos, CharaTask* a,
                            CharaTask* chara, int b, int c);
extern "C" void fn_8010CF28(BulletMgr_unk* mgr, int id, CharaTask* chara, CharaTask* owner); // inferred: counter event
extern "C" float fn_8010B250(BulletMgr_unk* mgr, int kind, int arg); // inferred: scale of the bullet
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: inlined here (the local static is shared with the out-of-line copy in CharaTask.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}
// inferred: inlined copy of CharaListeners_unk::fn_8014F860 (name invented)
inline void NotifyListeners_unk(CharaListeners_unk* listeners, int id, void* chara, void* arg) {
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, (CharaTask*)chara, arg);
            }
        }
    }
}

HomingBullet::HomingBullet() {
    mA0.mTarget = 0;
    mA0.mAccel = 0.0f;
    mA0.m08 = -1.0f;
    mA0.mMaxSpeed = 10.0f;
    mA0.mTurn = 0.09424778f;
    mA0.mTime = 300.0f;
    mA0.mDelay = 5.0f;
    mA0.mNode = 0;
    mA0.mCount = -1;
}

// NONMATCHING: 97.4% - the original keeps m08 of the copied block in f1 for the call (here it is loaded again);
// the block is loaded whole thanks to the const parameter; also register choices of the replay event
void HomingBullet::vf28(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* owner,
                        const HomingParam_unk* param, int arg) {
    m84 = 0;
    m88 = owner;
    m8C = 0;
    m90 = owner->mType;
    mA0 = *param;
    fn_8010F018(shot, pos, dir, mA0.mCount > 0 ? mA0.mCount : -1, arg, 0, mA0.m08);
    mC4 = 0.0f;
    if (lbl_8087F430->m5590 != 0) {
        ReplayEvent_unk ev;
        ev.mKind = 2;
        *(BulletShot_unk*)&ev.mShot = *shot;
        ev.mPos = *pos;
        ev.mDir = *dir;
        ev.mOwner = m88;
        ev.m3C = 0;
        ev.mBullet = this;
        lbl_8087F430->m5590->fn_805706C0(&ev);
    }
    ShotEvent_unk e;
    e.mOwner = owner;
    e.mId = shot->m0C->m04;
    e.mPos = *pos;
    e.mDir = *dir;
    e.mArg = arg;
    e.mParam = &mA0;
    NotifyListeners_unk(fn_8014F8F4(), 12, this, &e);
}

void HomingBullet::vf0C(int a, int b) {
    int record = ((m04 >> 2) & 1) ^ 1;
    Bullet::vf0C(0, b);
    if (record && lbl_8087F430->m5590 != 0) {
        ReplayEvent_unk ev;
        ev.mKind = 3;
        ev.mBullet = this;
        lbl_8087F430->m5590->fn_805706C0(&ev);
    }
}


void HomingBullet::vf10() {
    float dt = lbl_8087EFA8->mTimeScale;
    if (m94 >= 0.0f) {
        m94 -= dt;
        if (m94 <= 0.0f) {
            vf0C(1, 0);
        }
        return;
    }
    if (m78 >= m68->m44) {
        vf0C(1, 0);
        return;
    }
    m14 = m08;
    m08 += m20 * dt;
    m78 += m80 * dt;
    if (mA0.mTarget == 0) {
        vf0C(1, 0);
        return;
    }
    if (IsHoming_unk()) {
        if (mA0.mDelay > 0.0f) {
            m20 = m20.GetNormalize() * m80;
            mA0.mDelay -= dt;
        } else {
            CharaTask* target = mA0.mTarget;
            atn::Vector3 pos = (target->mColli.m3C + target->mColli.m48) * 0.5f;
            if (mA0.mNode != 0) {
                atn::Model* model = target->GetModel_unk();
                MtxPtr m = model->GetNodeMtx_unk(model->fn_800928B0(mA0.mNode, 0));
                if (m != 0) {
                    atn::VecAssign_unk(pos, GetTrans_unk(m));
                }
            }
            atn::Vector3 to = pos - m08;
            float dist = PSVECMag(&to);
            PSVECNormalize(&to, &to);
            atn::Vector3 dir = m20;
            float speed = PSVECMag(&dir);
            PSVECNormalize(&dir, &dir);
            float dot = PSVECDotProduct(&dir, &to);
            if (dot >= CosF_unk(mC4)) {
                m20 = to * m80;
                mC4 = 0.0f;
            } else {
                atn::Vector3 axis;
                PSVECCrossProduct(&to, &dir, &axis);
                if (!axis.IsZero_unk()) {
                    Mtx r;
                    PSMTXRotAxisRad(r, &axis, -mC4);
                    PSMTXMultVec(r, &dir, &dir);
                }
                m20 = dir * m80;
                float max = mA0.mTurn;
                float turn = 0.1f * max + mC4;
                mC4 = max < turn ? max : turn;
            }
            if (HasFlag_unk(0x400000) && dist < 1.5f * speed) {
                vf0C(1, 0);
                return;
            }
        }
        mA0.mTime -= dt;
        m80 = mA0.mAccel * dt + m80;
        float max = mA0.mMaxSpeed + m68->m4C;
        if (m80 > max) {
            m80 = max;
        }
    }
    fn_8010F4BC();
    if (!HasFlag_unk(0x200000)) {
        if (m84 == 0) {
            if ((int)m68->m02 == 6) {
                fn_801120B0();
            } else {
                fn_801100A0();
            }
        } else if (m84 == 1) {
            fn_80112174();
        }
    }
}

void HomingBullet::vf14(const Mtx m) {
    __MtxCopy(m2C.m, m);
}

void HomingBullet::vf18() {}

void HomingBullet::fn_80441E8C(const atn::Vector3* dir, CharaTask* owner, CharaTask* target) {
    m84 = 0;
    m88 = owner;
    m8C = 0;
    m90 = owner->mType;
    mA0.mTarget = target;
    m20 = *dir * m80;
    m78 = 0.0f;
    mA0.mTime = 300.0f;
    mA0.mDelay = 5.0f;
    m04 &= ~0x200000;
    fn_8010F4BC();
}

// inferred: unit vector along z rotated by `rad` around y (name invented)
inline atn::Vector3 FrontDir_unk(float rad) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx r;
    PSMTXRotRad(r, 'y', rad);
    PSMTXMultVec(r, &v, &v);
    return v;
}

// inferred: rotation matrix rot_z * rot_y * rot_x (name invented)
inline atn::Matrix3x4 RotZYX_unk(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        atn::Matrix3x4::RotateAxis_unk(m, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        atn::Matrix3x4::RotateAxis_unk(m, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        atn::Matrix3x4::RotateAxis_unk(m, 'x', rot.x);
    }
    return m;
}

// inferred: the bullet hits `chara`: the character can parry it (2: caught and thrown back by
// chr::ControlHitBackBullet, 4: split into two homing bullets toward the leader, 1/3: reflected)
// NONMATCHING: 98.2% - registers only: the float constants kept across the split loop (90, 0, 5, 1,
// the int->float double) get other callee-saved registers, and the temporaries of CanAct_unk and of
// the damage record use other volatile registers. Tried: the angle as a local before/after `off`,
// DegToRad_unk or the constant written out, CanAct_unk split.
int HomingBullet::vf1C(CharaTask* chara, const atn::Vector3* pos, void* arg) {
    if (HasFlag_unk(0x100) && chara->CanAct_unk() && (chara->mFlag5 || chara->mType != 0)) {
        atn::Vector3 dir = m20;
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        int res = chara->vf4C(m88, m68, (int)pos, &dir);
        if (res != 0) {
            atn::Vector3 at;
            if (chara->fn_80148B0C(0) != 0) {
                at = chara->fn_80148B0C(0)->m04.mCenter;
            } else {
                at = chara->mPos;
            }
            at += atn::Vector3(0.0f, 15.0f, 0.0f);
            HomingDamage_unk damage;
            fn_80108C10(lbl_8087F048, &damage, at, m88, chara, 0x80008, 0);
            if (res == 2) {
                int id = 0x2F;
                if (chara->mType == 0) {
                    id = 0x2E;
                }
                fn_8010CF28(lbl_8087F048, id, chara, m88);
                chr::ControlHitBackBullet* control =
                    new ((MemoryTag)3, "", "", 0) chr::ControlHitBackBullet(chara, atn::Vector3(-m20.x, -m20.y, -m20.z), this);
                chara->fn_80178208(control);
                m20.Set_unk(0.0f, 0.55f, 0.0f);
                m08 = chara->mPos + FrontDir_unk(chara->mRot.y) * 8.0f;
                m08.y += 13.0f;
                mA0.mTime = 0.0f;
                fn_8010F4BC();
                m04 |= 0x200000;
            } else if (res == 4) {
                CharaTask* target = m88;
                if (lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
                    target = lbl_8087F8A0->mHead;
                }
                if (target != 0) {
                    atn::Vector3 d = m20;
                    d.y = 0.0f;
                    PSVECNormalize(&d, &d);
                    Mtx r;
                    PSMTXRotRad(r, 'y', 3.1415927f);
                    PSMTXMultVec(r, &d, &d);
                    d.y = 0.3f;
                    PSVECNormalize(&d, &d);
                    mA0.m08 = 1.5f;
                    mA0.mDelay = 0.0f;
                    mA0.mAccel = 0.18f;
                    mA0.mTurn = 0.09424778f;
                    mA0.mTime = 300.0f;
                    mA0.mMaxSpeed = 2.3f;
                    for (int i = 0; i < 2; i++) {
                        if (i % 2 == 0) {
                            mA0.mTarget = target;
                        } else {
                            mA0.mTarget = m88;
                        }
                        float deg = 90.0f * i;
                        atn::Vector3 off(5.0f, 0.0f, 1.0f);
                        Mtx rz;
                        PSMTXRotRad(rz, 'z', atn::DegToRad_unk(deg));
                        PSMTXMultVec(rz, &off, &off);
                        atn::Matrix3x4 m = RotZYX_unk(d.ToRotation_unk());
                        PSMTXMultVec(m.m, &off, &off);
                        atn::Vector3 p = *pos + d * 5.0f;
                        int id = 0x5ED;
                        if (m68->m74.m00 > 0.5f) {
                            id = 0x5ED;
                        } else if (m68->m74.m04 > 0.5f) {
                            id = 0x5EE;
                        } else if (m68->m74.m0C > 0.5f) {
                            id = 0x5EF;
                        } else if (m68->m74.m10 > 0.5f) {
                            id = 0x5F0;
                        }
                        BulletMgr_unk* mgr = lbl_8087F048;
                        fn_800F8574(mgr, chara, fn_80219E6C(id), &p, &off, &mA0, 0, 0, 0.0f, 1.0f);
                    }
                    if (chara->mFlag22 == 1 && lbl_8087F430 != 0 && lbl_8087F430->fn_80370A78(0xCC) == 0) {
                        lbl_8087F430->fn_80370AE4(0xCC, 1);
                    }
                    fn_800C31F4("SE_DG036_015", 0, -1, 1.0f);
                    vf0C(1, 0);
                }
            } else {
                int id = 8;
                if (chara->mType == 0) {
                    id = 7;
                }
                fn_8010CF28(lbl_8087F048, id, chara, m88);
                atn::Vector3 d = m20;
                PSVECNormalize(&d, &d);
                d.x *= -1.0f;
                d.z *= -1.0f;
                d.y = res == 3 ? 0.01f : 1.0f;
                PSVECNormalize(&d, &d);
                d *= PSVECMag(&m20);
                m20 = d;
                m04 |= 0x200;
                m08 = *pos;
                mA0.mTime = 0.0f;
                fn_8010F4BC();
                if (res == 1) {
                    float scale = fn_8010B250(lbl_8087F048, m68->m3C - chara->mStatus.mParamB.m34, 0);
                    atn::Vector3 push = dir * scale * 5.0f;
                    chara->fn_8015D8C0(0.1f, push, (int)m88);
                    fn_800C344C("SE_BTCOM_015", pos, 0, -1, 2.0f);
                }
            }
            return 1;
        }
    }
    return 0;
}

void HomingBullet::vf20(CharaTask* chara, const atn::Vector3* pos, atn::Vector3* dir) {
    m88->vf64(2);
    mA0.mTarget = m88;
    mA0.m08 = 0.5f * m80;
    fn_800F8574(lbl_8087F048, chara, m68, pos, dir, &mA0, 0, 0, 0.0f, 1.0f);
}

CharaTask* HomingBullet::vf24() {
    return mA0.mTarget;
}
