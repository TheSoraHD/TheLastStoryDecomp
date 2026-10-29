// inferred file name: from the RTTI of Bullet (vtable 0x80779F10); the file starts with the boot
// sequence task (BootTask, RTTI 0x80779F08).
#include "game/Bullet.h"
#include "game/BootTask.h"
#include "game/CharaTask.h"
#include "game/SetConfig.h"
#include "atn/LoadManager.h"
#include "atn/Memory.h"
#include "atn/SequenceControl.h"
#include "atn/SoundCore.h"
#include "atn/SoundManager.h"
#include "gmk/Lemon.h"

// inferred: sound engine (lbl_8087EE90)
extern "C" atn::SoundEngine_unk* lbl_8087EE90;
extern "C" void fn_80046EE0(atn::SoundEngine_unk* engine, const char* path); // inferred: opens the sound archive
extern "C" int fn_800499D0(atn::SoundEngine_unk* engine);                    // inferred: output mode
extern "C" void fn_80047B54(atn::SoundEngine_unk* engine, int arg);          // inferred
extern "C" void fn_802144A8();                                               // inferred: loads the sound tables
extern "C" int fn_803CE148();
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task

// inferred: game settings (lbl_8087F0A8); only the fields read here
struct BootSettings_unk {
    unsigned char m000[0x30];
    int m30; // inferred: frames per second
    unsigned char m034[0x1B4 - 0x34];
    char m1B4[1]; // inferred: name of the sequence started after the boot
};
extern "C" BootSettings_unk* lbl_8087F0A8;

// inferred: creator of the boot sequence (registered as "BootTask" by atn/SequenceUnitParam1_const_int)
atn::Task* fn_8010EB58(atn::Task* const parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) BootTask(parent);
    }
    return 0;
}

// inferred: opens the sound archive once the loader is running, then waits for the loads
int BootTask::vf0C() {
    switch (mState) {
    case 0:
        if (!lbl_8087F518->IsFlag_unk(2)) {
            break;
        }
        lbl_8087F518->fn_8046EC7C();
        fn_80046EE0(lbl_8087EE90, "/sound/LastWorld_xx.brsar");
        lbl_8087EFE8->mOutputMode = fn_800499D0(lbl_8087EE90);
        if (SetConfig::sInstance != 0) {
            SetConfig::sInstance->fn_8057F0F8();
        }
        fn_802144A8();
        atn::SoundEngine_unk* engine = lbl_8087EE90;
        fn_80047B54(engine, fn_803CE148());
        mState = 1;
    case 1:
        if (lbl_8087F518->fn_8046ECC8() == 0) {
            mState = 3;
        }
        break;
    }
    return mState == 3;
}

// inferred: removes itself and starts the first sequence
void BootTask::vf10() {
    fn_800D2338(this);
    atn::SequenceControl::sInstance->mNextId = atn::SequenceControl::sInstance->fn_800C37FC(lbl_8087F0A8->m1B4);
}

BootTask::~BootTask() {}

Bullet::Bullet()
    : m04(0), m08(0.0f, 0.0f, 0.0f), m14(0.0f, 0.0f, 0.0f), m20(0.0f, 0.0f, 0.0f), m5C(0.1f), m60(0), m64(0),
      m6C(0), m70(0.0f), m74(1.0f), m78(0.0f), m7C(0), m84(0), m88(0), m8C(0), m94(-1.0f), m9C(0) {
    m2C.SetIdentity();
}

Bullet::~Bullet() {}

extern "C" HomingField_unk* lbl_8087F430;

// inferred: effect manager (lbl_8087F3C0); mB8 selects the effect group of the next effects
struct BulletEffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
extern "C" BulletEffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: sets the owner of the next effects
extern "C" void fn_8023A680(BulletEffectMgr_unk* mgr, int id, int a, int b, atn::Matrix3x4* mtx, int c, int d,
                            int e, int f, int g, int h, float scale); // inferred: plays an effect on `mtx`
extern "C" void fn_80239DAC(BulletEffectMgr_unk* mgr, void* owner, int group, int arg); // inferred: stops effects

// inferred: frame time step (lbl_8087EFA8)
struct BulletTime_unk {
    unsigned char m000[0x3A4];
    float mTimeScale;
};
extern "C" BulletTime_unk* lbl_8087EFA8;

struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" void fn_800F8574(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, void* param, int arg6, int arg7, float a, float b);
extern "C" void fn_800FB4B0(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* data, int id, const atn::Vector3* pos,
                            int a, int b, int c, int d); // inferred: hit effect on the collision
extern "C" void* lbl_8087EE98; // inferred: collision manager
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg);
extern "C" void fn_80599FFC(int obj, const atn::Vector3* pos); // inferred
extern "C" void fn_800FB1BC(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int b, float scale); // inferred: hit effect
extern "C" int fn_800FAB80(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* data, int id, const atn::Vector3* pos,
                           const atn::Vector3* rot, int a, int b, float c, float d, int e, int f); // inferred: hit effect
extern "C" void fn_80373164(HomingField_unk* field, CharaTask* owner, const atn::Vector3* pos, int id); // inferred
extern "C" int fn_800F8548(BulletMgr_unk* mgr); // inferred: new bullet id

extern "C" void fn_800F80B8(atn::Matrix3x4* mtx, const atn::Vector3* rot); // inferred: rotates by the angles
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);

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

void Bullet::fn_8010EE78(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* owner,
                         int count, unsigned int flags, float speed) {
    m84 = 0;
    m88 = owner;
    m8C = 0;
    m90 = owner->mType;
    fn_8010F018(shot, pos, dir, count, flags, 1, speed);
    ShotEvent_unk e;
    e.mOwner = owner;
    e.mId = shot->m0C->m04;
    e.mPos = *pos;
    e.mDir = *dir;
    e.mArg = flags;
    e.mParam = 0;
    NotifyListeners_unk(fn_8014F8F4(), 12, this, &e);
}

void Bullet::fn_8010EFC8(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, int arg, int count,
                         unsigned int flags, float speed) {
    m84 = 1;
    m88 = 0;
    m8C = arg;
    fn_8010F018(shot, pos, dir, count, flags, 1, speed);
}

void Bullet::fn_8010EFF0(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, int type, int count,
                         unsigned int flags, float speed) {
    m84 = 0;
    m88 = 0;
    m8C = 0;
    m90 = type;
    fn_8010F018(shot, pos, dir, count, flags, 1, speed);
}

void Bullet::fn_8010F018(BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, int count,
                         unsigned int flags, int record, float speed) {
    m04 = flags | 0x10000;
    m5C = shot->m00;
    m60 = shot->m04;
    m64 = shot->m08;
    m68 = shot->m0C;
    m6C = shot->m10;
    m70 = shot->m14;
    m74 = shot->m18;
    if (speed > 0.0f) {
        m80 = speed;
    } else {
        m80 = m68->m4C;
    }
    m20 = *dir * m80;
    m08 = *pos;
    m14 = *pos;
    m78 = 0.0f;
    m9C = 0;
    fn_8010F4BC();
    int id = shot->m04;
    lbl_8087F3C0->mB8 = 3;
    if (id != 0) {
        fn_80232B7C(this, 0);
        fn_8023A680(lbl_8087F3C0, shot->m04, -1, 5, &m2C, 0, 0, 0, 0, -1, 1, 1.0f);
    }
    lbl_8087F3C0->mB8 = 0;
    if (count < 0) {
        m7C = fn_800F8548(lbl_8087F048);
    } else {
        m7C = count;
    }
    m94 = -1.0f;
    m98 = shot->m10;
    if (shot->m10 > 0) {
        m04 |= 0x40000;
    }
    if (record != 0 && lbl_8087F430->m5590 != 0) {
        ReplayEvent_unk ev;
        ev.mKind = 0;
        *(BulletShot_unk*)&ev.mShot = *shot;
        ev.mPos = *pos;
        ev.mDir = *dir;
        ev.mOwner = m88;
        ev.m3C = m8C;
        ev.mBullet = this;
        fn_805706C0(lbl_8087F430->m5590, &ev);
    }
}

void Bullet::fn_8010F26C(BulletShot_unk* shot) {
    int id = shot->m04;
    lbl_8087F3C0->mB8 = 3;
    if (id != 0) {
        fn_80232B7C(this, 0);
        fn_8023A680(lbl_8087F3C0, shot->m04, -1, 5, &m2C, 0, 0, 0, 0, -1, 1, 1.0f);
    }
    lbl_8087F3C0->mB8 = 0;
}

// inferred: removes the bullet (`record`: add a replay event)
void Bullet::vf0C(int record, int b) {
    m04 = (m04 & ~0x30000) | 0x2000000;
    fn_8010F41C();
    if (record != 0 && lbl_8087F430->m5590 != 0) {
        ReplayEvent_unk ev;
        ev.mKind = 1;
        ev.mBullet = this;
        fn_805706C0(lbl_8087F430->m5590, &ev);
    }
}

void Bullet::fn_8010F41C() {
    lbl_8087F3C0->mB8 = 3;
    int keep = (m04 >> 6) & 1;
    fn_80239DAC(lbl_8087F3C0, this, 0, keep);
    fn_80239DAC(lbl_8087F3C0, this, 10, keep);
    fn_80239DAC(lbl_8087F3C0, this, 1, 1);
    fn_80239DAC(lbl_8087F3C0, this, 11, 1);
    lbl_8087F3C0->mB8 = 0;
}

void Bullet::fn_8010F4BC() {
    if (m94 >= 0.0f && HasFlag_unk(0x20) && m9C != 0) {
        MtxPtr node = m9C->GetModel_unk()->GetNodeMtx_unk("spine");
        if (HasFlag_unk(0x80000)) {
            atn::Vector3 trans = ((atn::Matrix3x4*)node)->GetTrans_unk();
            atn::Matrix3x4 rot(node);
            rot.fn_8010F668();
            atn::Vector3 dir = atn::Vector3(0.0f, 0.0f, -1.0f).Transform_unk(rot);
            atn::Vector3 angle = dir.ToRotation_unk();
            m2C.SetIdentity();
            m2C.Translate_unk(trans);
            fn_800F80B8(&m2C, &angle);
        } else if (node != 0) {
            atn::Matrix3x4 mtx(node);
            atn::Vector3 pos = m08.Transform_unk(mtx);
            mtx.fn_8010F668();
            atn::Vector3 vel = m20.Transform_unk(mtx);
            atn::Vector3 angle = vel.GetNormalize().ToRotation_unk();
            m2C.SetIdentity();
            m2C.Translate_unk(pos);
            fn_800F80B8(&m2C, &angle);
        }
    } else {
        atn::Vector3 angle = m20.GetNormalize().ToRotation_unk();
        m2C.SetIdentity();
        m2C.Translate_unk(m08);
        fn_800F80B8(&m2C, &angle);
    }
}

inline void atn::Matrix3x4::fn_8010F668() {
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
}

inline void atn::Matrix3x4::Translate_unk(const Vector3& v) {
    Mtx t;
    PSMTXTrans(t, v.x, v.y, v.z);
    Mtx r;
    PSMTXConcat(m, t, r);
    __MtxCopy(m, r);
}

inline atn::Vector3 atn::Vector3::Transform_unk(const Matrix3x4& mtx) const {
    Vector3 r(*this);
    PSMTXMultVec(mtx.m, &r, &r);
    return r;
}

// inferred: update: removes the bullet when its time is over (with the hit effect of the data),
// hits at the end of its range, moves it and tests the collision
void Bullet::vf10() {
    float dt = lbl_8087EFA8->mTimeScale;
    if (m94 >= 0.0f) {
        m04 |= 0x20000;
        fn_8010F4BC();
        m94 -= dt;
        if (m94 <= 0.0f) {
            if (!HasFlag_unk(0x100000) && m64 != 0) {
                atn::Vector3 pos = m9C != 0 ? m2C.GetTrans_unk() : m08;
                atn::Vector3 dir =
                    m9C != 0 ? atn::Vector3(0.0f, 0.0f, 1.0f).Transform_unk(m2C.GetRotation_unk()) : m20;
                atn::Vector3 rot = dir.ToRotation_unk();
                int id = 0;
                if (HasFlag_unk(0x1000)) {
                    fn_800FB1BC(lbl_8087F048, m88, m68, &pos, &rot, 0, 0, 1.0f);
                } else {
                    id = fn_800FAB80(lbl_8087F048, m88, m68, m7C, &pos, &rot, 0, 30, m5C, m74, -1, -1);
                }
                fn_80373164(lbl_8087F430, m88, &pos, id);
            }
            vf0C(1, 0);
        }
        return;
    }
    if (m98 > 0) {
        fn_8010F4BC();
        m98--;
        return;
    }
    if (m78 >= m68->m44 || (m70 > 1.0f && m78 >= m70)) {
        if ((m68->mAC & 0x10000) || (m70 > 1.0f && m78 >= m70)) {
            CollResult_unk res;
            res.m00 = 1;
            res.m04 = m08;
            res.m10 = m08;
            atn::Vector3 normal = -m20.GetNormalize();
            res.m1C = normal;
            res.m28 = normal;
            fn_80110858(&res);
        }
        vf0C(1, 0);
        return;
    }
    atn::Vector3 step = m20 * dt;
    m14 = m08;
    m08 += step;
    m78 += m80 * dt;
    signed char kind = m68->m02;
    if (kind == 1 || kind == 5) {
        if ((m68->mAC & 0x8000) != 0x8000 || !HasFlag_unk(0x10)) {
            m20.y += dt * (-98.1f / (float)(lbl_8087F0A8->m30 * lbl_8087F0A8->m30));
        }
    }
    if (HasFlag_unk(0x800000)) {
        if (m20.y < 0.0f) {
            m20.y *= 0.8f;
        }
        m20.y += 5.0f * (dt * -(-98.1f / (float)(lbl_8087F0A8->m30 * lbl_8087F0A8->m30)));
    }
    fn_8010F4BC();
    if (!HasFlag_unk(0x200000)) {
        if (m84 == 0) {
            if (m68->m02 == 6) {
                fn_801120B0();
            } else {
                fn_801100A0();
            }
        } else if (m84 == 1) {
            fn_80112174();
        }
    }
}

// inferred: moves the bullet one frame (falling bullets, kinds 1 and 5, are pulled by gravity)
void Bullet::vf14(const Mtx m) {
    float dt = lbl_8087EFA8->mTimeScale;
    if (m94 >= 0.0f) {
        m94 -= dt;
        if (m94 <= 0.0f) {
            vf0C(1, 0);
        }
        return;
    }
    atn::Vector3 step = m20 * dt;
    m14 = m08;
    m08 += step;
    m78 += m80 * dt;
    signed char kind = m68->m02;
    if (kind == 1 || kind == 5) {
        m20.y += -98.1f / (float)(lbl_8087F0A8->m30 * lbl_8087F0A8->m30);
    }
    fn_8010F4BC();
}

// NONMATCHING: placeholder
void Bullet::fn_801100A0() {}

CharaTask* Bullet::vf24() {
    return 0;
}

// inferred: the bullet is reflected back to the owner
void Bullet::vf20(CharaTask* chara, const atn::Vector3* pos, atn::Vector3* dir) {
    atn::Vector3 target = m88->vf64(2);
    *dir = target - *pos;
    dir->GetNormalize();
    fn_800F8574(lbl_8087F048, chara, m68, pos, dir, 0, 0, 0, 0.0f, 1.0f);
}

// NONMATCHING: placeholder
void Bullet::fn_80110858(CollResult_unk* res) {}

// inferred: removes the bullet when it hits the background (hit effect on the collision)
void Bullet::fn_801120B0() {
    unsigned int mask = 0x80000006;
    if (HasFlag_unk(2)) {
        mask |= 0x40000000;
    }
    CollResult_unk res;
    if (fn_8004ED34(lbl_8087EE98, &res, &m14, &m08, mask, 0, 0) != 0) {
        fn_800FB4B0(lbl_8087F048, m88, m68, m7C, &m08, 0, 0x1E, -1, 1);
        vf0C(1, 0);
    }
}

void Bullet::fn_80112174() {
    unsigned int mask = 0x80000006;
    if (HasFlag_unk(2)) {
        mask |= 0x40000000;
    }
    CollResult_unk res;
    if (fn_8004ED34(lbl_8087EE98, &res, &m14, &m08, mask, 0, 0) != 0) {
        fn_80599FFC(m8C, &res.m10);
        vf0C(1, 0);
    }
}

int Bullet::vf1C(CharaTask* chara, const atn::Vector3* pos, void* arg) {
    return 0;
}
