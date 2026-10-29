#include "game/HomingBullet.h"
#include "game/CharaTask.h"

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

// inferred: field object; m5590 is the replay recorder
struct HomingField_unk {
    unsigned char m0000[0x5590];
    void* m5590;
};
extern "C" HomingField_unk* lbl_8087F430;

// inferred: event recorded by the replay (0x44 bytes; kind 2: shot, kind 3: removed). Names invented.
struct ReplayShot_unk {
    ReplayShot_unk() : m00(0.1f), m04(0), m08(0), m10(0), m14(0.0f), m18(1.0f) {}
    float m00;
    int m04;
    int m08;
    HitInfo_unk* m0C;
    int m10;
    float m14;
    float m18;
};
struct ReplayEvent_unk {
    int mKind;
    ReplayShot_unk mShot;
    atn::Vector3 mPos;
    atn::Vector3 mDir;
    CharaTask* mOwner;
    int m3C;
    Bullet* mBullet;
};
extern "C" void fn_805706C0(void* recorder, ReplayEvent_unk* ev); // inferred: records an event

// inferred: data of listener event 12 (a homing bullet was shot); names invented
struct HomingShotEvent_unk {
    CharaTask* mOwner;
    int mId;
    atn::Vector3 mPos;
    atn::Vector3 mDir;
    int mArg;
    HomingParam_unk* mParam;
};

struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" void fn_800F8574(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a, float b);

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

// NONMATCHING: 84.3% - the original loads the whole parameter block before storing the owner fields
// (scheduling); also register choices of the replay event
void HomingBullet::vf28(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* owner,
                        HomingParam_unk* param, int arg) {
    m84 = 0;
    m88 = owner;
    m8C = 0;
    m90 = owner->mType;
    mA0 = *param;
    fn_8010F018(this, shot, pos, dir, param->mCount > 0 ? param->mCount : -1, arg, 0);
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
        fn_805706C0(lbl_8087F430->m5590, &ev);
    }
    HomingShotEvent_unk e;
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
    fn_8010F30C(this, 0, b);
    if (record && lbl_8087F430->m5590 != 0) {
        ReplayEvent_unk ev;
        ev.mKind = 3;
        ev.mBullet = this;
        fn_805706C0(lbl_8087F430->m5590, &ev);
    }
}

// NONMATCHING: 99.93% - r3/r4 of the inlined vector copy of the node translation
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
                    pos = GetTrans_unk(m);
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
    fn_8010F4BC(this);
    if (!HasFlag_unk(0x200000)) {
        if (m84 == 0) {
            if ((int)m68->m02 == 6) {
                fn_801120B0(this);
            } else {
                fn_801100A0(this);
            }
        } else if (m84 == 1) {
            fn_80112174(this);
        }
    }
}

void HomingBullet::vf14(const Mtx m) {
    __MtxCopy(m2C, m);
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
    fn_8010F4BC(this);
}

void HomingBullet::vf20(CharaTask* chara, const atn::Vector3* pos, const atn::Vector3* dir) {
    m88->vf64(2);
    mA0.mTarget = m88;
    mA0.m08 = 0.5f * m80;
    fn_800F8574(lbl_8087F048, chara, m68, pos, dir, &mA0, 0, 0, 0.0f, 1.0f);
}

CharaTask* HomingBullet::vf24() {
    return mA0.mTarget;
}
