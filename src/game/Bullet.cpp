// inferred file name: from the RTTI of Bullet (vtable 0x80779F10). The boot sequence task before it is
// game/BootTask.cpp.
#include "game/Bullet.h"
#include "game/GameTask.h"
#include "game/CharaTask.h"
#include "game/SetConfig.h"
#include "game/SkillEffectManager.h"
#include "atn/SoundRef.h"
#include "atn/LoadManager.h"
#include "atn/Memory.h"
#include "atn/SequenceControl.h"
#include "atn/SoundCore.h"
#include "atn/SoundManager.h"
#include "gmk/Lemon.h"
#include "game/ReplayBuffer.h"
#include "game/BattleTask.h"
#include "game/GimmickCommon.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/GameVoice.h"
#include "atn/FixedArray.h"

// inferred: game settings (lbl_8087F0A8); only the fields read here
struct BootSettings_unk {
    unsigned char m000[0x30];
    int m30; // inferred: frames per second
    unsigned char m034[0x1B4 - 0x34];
    char m1B4[1]; // inferred: name of the sequence started after the boot
};
extern "C" BootSettings_unk* lbl_8087F0A8;

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

extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager
extern "C" void* lbl_8087EE98; // inferred: collision manager
extern "C" int fn_8004ED34(void* colli, atn::ColliResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg);
extern "C" int fn_8004D388(void* colli, atn::ColliResult_unk* result, const atn::Vector3* center, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, HitInfo_unk* data, float radius); // inferred: sphere sweep

extern "C" SkillEffectManager* lbl_8087F9E8;
extern "C" int fn_8021A8D0(HitInfo_unk* data); // inferred: the bullet goes through shields
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_8005361C(atn::ColliResult_unk* result, const atn::ColliCapsuleShape_unk* seg,
                           const atn::ColliSphereShape_unk* sphere); // inferred: segment against sphere
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* axb);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" int rand();

// inferred: collision object of a character (atn::ColliResult_unk::m38 when m3C has the flag 2)
struct CollOwner_unk {
    unsigned char m00[0x0C];
    CharaTask* m0C;
};

// inferred: sound effects of the bullets
static const char* const sSeNames_unk[] = {
    "SE_BTCOM_006", "SE_BTCOM_007", "SE_BTCOM_016", "SE_SP015_001", "SE_BTCOM_102", 0,
};

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

// NONMATCHING: 97.3% - registers of the effect start block (as fn_8010F26C; a call to it, a local id: same)
void Bullet::fn_8010F018(const BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir, int count,
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
    lbl_8087F3C0->mB8 = 3;
    if (shot->m04 != 0) {
        fn_80232B7C(this, 0);
        fn_8023A680(lbl_8087F3C0, shot->m04, -1, 5, &m2C, 0, 0, 0, 0, -1, 1, 1.0f);
    }
    lbl_8087F3C0->mB8 = 0;
    if (count < 0) {
        m7C = lbl_8087F048->fn_800F8548();
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
        lbl_8087F430->m5590->fn_805706C0(&ev);
    }
}

// inferred: `shot` is const (the original loads shot->m04 before saving r31)
void Bullet::fn_8010F26C(const BulletShot_unk* shot) {
    lbl_8087F3C0->mB8 = 3;
    if (shot->m04 != 0) {
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
        lbl_8087F430->m5590->fn_805706C0(&ev);
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

// NONMATCHING: 97.1% - over the inline budget as the original (empty asserts in Translate_unk), with the flag
// tests written out (the original does not call HasFlag_unk); left: "spine" is a separate .rodata object in
// the original (lbl_807366A8) and the stack slots of the vector temporaries (Transform_unk(...).ToRotation_unk()
// chains, m08.Transform_unk of the node itself)
void Bullet::fn_8010F4BC() {
    if (m94 >= 0.0f && (m04 & 0x20) == 0x20 && m9C != 0) {
        MtxPtr node = m9C->GetModel_unk()->GetNodeMtx_unk("spine");
        if ((m04 & 0x80000) == 0x80000) {
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

// inferred: asserts of the debug build, empty in release: they only count for the inline budget (with them
// Bullet::fn_8010F4BC goes over it and calls its helpers out of line, as the original)
#define BULLET_ASSERT_UNK(x) ((void)0)
inline void atn::Matrix3x4::Translate_unk(const Vector3& v) {
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0); BULLET_ASSERT_UNK(this != 0);
    Mtx r;
    Mtx t;
    PSMTXTrans(t, v.x, v.y, v.z);
    PSMTXConcat(m, t, r);
    __MtxCopy(m, r);
}

inline atn::Vector3 atn::Vector3::Transform_unk(const Matrix3x4& mtx) const {
    Vector3 r(*this);
    PSMTXMultVec(mtx.m, &r, &r);
    return r;
}

// inferred: hit effect of the bullet data at `pos` facing `dir` (also written out in vf10); name invented
inline void PlayHitEffect_unk(Bullet* bullet, const atn::Vector3& pos, const atn::Vector3& dir) {
    atn::Vector3 angle = dir.ToRotation_unk();
    int id = 0;
    if ((bullet->m04 & 0x1000) == 0x1000) {
        lbl_8087F048->fn_800FB1BC(bullet->m88, bullet->m68, &pos, &angle, 0, 0, 1.0f);
    } else {
        id = lbl_8087F048->fn_800FAB80(bullet->m88, bullet->m68, bullet->m7C, &pos, &angle, 0, 30, -1, -1,
                                          bullet->m5C, bullet->m74);
    }
    ((GameTask*)lbl_8087F430)->fn_80373164(bullet->m88, &pos, id);
}

// NONMATCHING: 96.2% - stack slots and FPR numbering of the inlined ToRotation_unk of the effect angle
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
                PlayHitEffect_unk(this, pos, dir);
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
            atn::ColliResult_unk res;
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

// NONMATCHING: 99.6% - stack layout (frame 0x10 larger: the original keeps fewer vector temporaries) and r29/r28
// swapped between `hit` and the owner of the hit object
// inferred: hit test against the characters: a bullet that reaches a shield (skill effect with the
// flag 0x400000) of another side is reflected (or bounced back with vf20), otherwise it tests the
// collision and hits with fn_80110858
void Bullet::fn_801100A0() {
    if (lbl_8087F9E8 != 0 && fn_8021A8D0(m68) == 0) {
        SkillEffect* shield = lbl_8087F9E8->fn_8059C5DC(0, 0, 0x400000); // inferred: shield effect
        if (shield != 0 && shield->m08 != 0 && m88 != 0 && shield->m08->fn_8016F3D0(m88, 1) == 0) {
            atn::ColliCapsuleShape_unk seg;
            seg.mStart = m14;
            seg.mEnd = m08;
            atn::ColliSphereShape_unk sphere;
            sphere.mCenter = shield->m10;
            sphere.mRadius = 1.1f * (shield->m5C * shield->m28);
            atn::ColliResult_unk res;
            atn::Vector3 move = seg.mStart - seg.mEnd;
            if (PSVECSquareMag(&move) > 2.0f && fn_8005361C(&res, &seg, &sphere) != 0) {
                fn_800C344C(sSeNames_unk[4], &m88->mPos, 0, -1, 0.75f);
                atn::Vector3 normal = res.m10 - sphere.mCenter;
                if (!(atn::Abs_unk(PSVECSquareMag(&normal)) < 1.1920929e-7f)) {
                    PSVECNormalize(&normal, &normal);
                }
                PSVECMag(&m20);
                float dot = PSVECDotProduct(&normal, &m20);
                atn::Vector3 dir = m20 - normal * 2.0f * dot;
                dir += m20 * 0.75f;
                float t = 0.4f * ((float)(rand() % 1001) / 1000.0f) - 0.2f;
                atn::Vector3 up(0.0f, 1.0f, 0.0f);
                atn::Vector3 side;
                PSVECCrossProduct(&m20, &up, &side);
                dir += side * t;
                PSVECNormalize(&dir, &dir);
                dir *= PSVECMag(&m20);
                if (HasFlag_unk(0x400)) {
                    atn::Vector3 back = dir;
                    vf20(shield->m08, &res.m10, &back);
                    vf0C(1, 0);
                } else {
                    m20 = dir;
                    m08 = res.m10 + m20;
                    fn_8010F4BC();
                }
                return;
            }
            atn::Vector3 d = m08 - sphere.mCenter;
            if (PSVECSquareMag(&d) < sphere.mRadius * sphere.mRadius) {
                fn_800C344C(sSeNames_unk[4], &m88->mPos, 0, -1, 0.75f);
                vf0C(1, 0);
                return;
            }
        }
    }
    unsigned int mask = 6;
    if (HasFlag_unk(2)) {
        mask |= 0x40000000;
    }
    if (HasFlag_unk(0x200)) {
        mask |= 0x80000000;
    }
    if (m88 != 0 && m88->mType == 3 && m78 <= 4.0f * m80) {
        mask |= 0x40000000;
    }
    atn::ColliResult_unk res;
    void* ignore = &m88->mColli;
    if ((m68->mAC & 0x8000) == 0x8000 && m78 > m80) {
        ignore = 0;
    }
    int hit;
    if (HasFlag_unk(0x10)) {
        atn::Vector3 move = m08 - m14;
        hit = fn_8004D388(lbl_8087EE98, &res, &m14, &move, mask, ignore, 0, m68, m68->m58);
    } else {
        hit = fn_8004ED34(lbl_8087EE98, &res, &m14, &m08, mask, ignore, 0);
    }
    if (hit != 0 && HasFlag_unk(0x800) && (res.m3C & 1) != 1 && (res.m3C & 2) != 2) {
        hit = 0;
    }
    if (hit != 0 && HasFlag_unk(0x2000) && (res.m3C & 2) == 2) {
        CharaTask* target = ((CollOwner_unk*)res.m38)->m0C;
        if (target != vf24()) {
            hit = 0;
        }
    }
    if (hit != 0) {
        fn_80110858(&res);
    }
}

CharaTask* Bullet::vf24() {
    return 0;
}

// inferred: the bullet is reflected back to the owner
void Bullet::vf20(CharaTask* chara, const atn::Vector3* pos, atn::Vector3* dir) {
    atn::Vector3 target = m88->vf64(2);
    *dir = target - *pos;
    dir->GetNormalize();
    lbl_8087F048->fn_800F8574(chara, m68, pos, dir, 0, 0, 0, 0.0f, 1.0f);
}

extern "C" void fn_8023A108(BulletEffectMgr_unk* mgr, void* owner, int group, int flags); // inferred: effect flags on
extern "C" void fn_8023A02C(BulletEffectMgr_unk* mgr, void* owner, int group, int flags); // inferred: effect flags off
extern "C" void fn_8023A098(BulletEffectMgr_unk* mgr, void* owner, int group, int flags);
// inferred: damage of a bullet hit (mgr, attacker, chara, hit, data, id, pos, dir, effect, rate); returns the
// kind of the hit effect
extern "C" int fn_800FDB20(BattleTask* mgr, CharaTask* attacker, CharaTask* chara, int hit, HitInfo_unk* data, int id,
                           const atn::Vector3* pos, const atn::Vector3* dir, int effect, float rate);
extern "C" int fn_8021A960(HitInfo_unk* data); // inferred: the data starts a skill effect
extern "C" SkillEffect* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                    const atn::Vector3* pos, const atn::Vector3* rot,
                                    boost::function<void(SkillEffect*)> callback, float rate);
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" void PSMTXInverse(const Mtx src, Mtx inv);

// NONMATCHING: 96.8% - vf1C is called without a third argument (li r6), a dead second beq in the gimmick test
// (m64), register numbering of the reflection and of CanAct_unk, argument loads of the BattleTask calls and
// the stack slots of some temporaries
// inferred: the bullet hits `res`: a character (vf1C, hit with fn_800FC1D0/fn_800FDB20), a gimmick (vf44 with
// the power of the bullet) or the background; plays the hit effects and sounds, sticks the bullet to the
// character (flag 0x20, node "spine"), and a nearby ally may comment the hit
void Bullet::fn_80110858(atn::ColliResult_unk* res) {
    atn::Vector3 dir = m08 - m14;
    atn::Vector3 back(0.0f, 0.0f, 0.0f);
    if (dir.IsZero_unk()) {
        dir.z = 1.0f;
    }
    back = dir.GetNormalize();
    int hit = 1;
    int kind = 2;
    atn::ColliBase* obj = res->m38;
    if (obj != 0 && (obj->mFlags & 2) == 2) {
        m9C = (CharaTask*)obj->m0C;
        kind = 0;
        if (vf1C(m9C, &res->m10, 0) != 0) {
            return;
        }
    } else if (obj != 0 && (obj->mFlags & 8) == 8) {
        if ((m04 & 0x80) != 0x80 && (m64 == 0 || !(m64 != 0 && m68->m60 != 0))) {
            HitInfo_unk* data = m68;
            int power = data->m1C;
            if (m88 != 0) {
                power += (int)(data->m28 * m88->mStatus.mParamB.m00);
            }
            int level = data->m3C;
            GimmickHitMsg_unk msg;
            memset(&msg, 0, sizeof(msg));
            msg.mType = power;
            msg.mDir = back;
            msg.mPos = res->m10;
            *(HitInfo_unk**)msg.m2C = m68;
            msg.mSender = m88;
            GimmickCommon* gimmick = (GimmickCommon*)res->m38->m0C;
            switch (gimmick->m50) {
            case 1:
                if (level >= 2) {
                    int sub = gimmick->mId % 1000;
                    if (sub == 0x3A || sub == 0x6B || sub == 0x7B) {
                        gimmick->vf44(&msg);
                    } else if (lbl_8087F048 != 0 && !lbl_8087F048->fn_8010D7B4(m68)) {
                        gimmick->vf44(&msg);
                    }
                    kind = 2;
                }
                break;
            case 0x31:
                gimmick->vf44(&msg);
                kind = 6;
                break;
            case 0x10:
            case 0x22:
            case 0x29:
            case 0x30:
                gimmick->vf44(&msg);
                kind = 2;
                break;
            }
        }
    } else if (res->m28.y > 0.9f) {
        kind = 3;
    }
    if (hit == 0) {
        return;
    }
    int stuck = m9C != 0;
    atn::Vector3 pos = res->m10;
    atn::Vector3 rot = res->m28.ToRotation_unk();
    if ((m68->mAC & 0x8000) == 0x8000) {
        if (m9C == 0) {
            if ((m04 & 0x10) == 0x10) {
                return;
            }
            if (res->m28.y > 0.9f) {
                m08 = res->m04 + res->m28;
                m04 |= 0x12;
                m20.Set_unk(0.0f, 0.0f, 1e-9f);
                m80 = 1.0f;
                m78 = m68->m44 - 15.0f * (30.0f * m80);
                fn_8010F4BC();
                lbl_8087F3C0->mB8 = 3;
                fn_8023A108(lbl_8087F3C0, this, 0, 1);
                fn_8023A108(lbl_8087F3C0, this, 10, 1);
                fn_8023A02C(lbl_8087F3C0, this, 0, 2);
                fn_8023A02C(lbl_8087F3C0, this, 10, 2);
                lbl_8087F3C0->mB8 = 0;
                return;
            }
            atn::Vector3 normal = res->m28 * PSVECDotProduct(&res->m28, &m20);
            atn::Vector3 side = m20 - normal;
            m20 = atn::Vector3(-normal.x, -normal.y, -normal.z) * 0.2f + side * 0.2f;
            m80 = PSVECMag(&m20);
            m08 = res->m04 + res->m28;
            return;
        }
        m20.y = 0.0f;
        m04 |= 0x200000;
        m04 &= ~0x10;
        m04 &= ~2;
        if (PSVECSquareMag(&m20) > 1.0f) {
            m20 *= -1.0f;
            PSVECNormalize(&m20, &m20);
        } else {
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', m9C->mRot.y);
            PSMTXMultVec(m, &front, &front);
            atn::Vector3 up(0.0f, 1.0f, 0.0f);
            atn::Vector3 front2(0.0f, 0.0f, 1.0f);
            Mtx m2;
            PSMTXRotRad(m2, 'y', m9C->mRot.y);
            PSMTXMultVec(m2, &front2, &front2);
            atn::Vector3 cross;
            PSVECCrossProduct(&front2, &up, &cross);
            m20 = cross + front;
            PSVECNormalize(&m20, &m20);
        }
        m20 = (m20 + atn::Vector3(0.0f, 2.5f, 0.0f)) * 0.5f;
        m80 = PSVECMag(&m20);
        m78 = m68->m44 - 30.0f * m80;
        kind = -1;
        lbl_8087F3C0->mB8 = 3;
        fn_8023A098(lbl_8087F3C0, this, 0, 2);
        fn_8023A098(lbl_8087F3C0, this, 10, 2);
        lbl_8087F3C0->mB8 = 0;
    }
    if (m64 != 0 && (m04 & 0x80) != 0x80) {
        if (m68->m60 == 0) {
            PlayHitEffect_unk(this, pos, res->m28);
        } else {
            m04 |= 0x100024;
            fn_800C344C(sSeNames_unk[3], &pos, 0, -1, 1.0f);
            if (m9C != 0 && (m04 & 0x80) != 0x80 && m68->m70 > 0) {
                HitInfo_unk* data = fn_80219E6C(m68->m70);
                int id = lbl_8087F048->fn_800F8548();
                int r = lbl_8087F048->fn_800FC1D0(m88, m9C, data, id, &pos, 0, m90);
                atn::Vector3 d = (m08 - m14).GetNormalize();
                lbl_8087F048->fn_800FDB20(m88, m9C, r, data, id, &pos, &d, 10, m5C);
            }
        }
    } else {
        if (m9C != 0 && (m04 & 0x80) != 0x80) {
            int r = lbl_8087F048->fn_800FC1D0(m88, m9C, m68, m7C, &pos, 0, m90);
            if (r >= 0) {
                atn::Vector3 d = m08 - m14;
                if (d.IsZero_unk()) {
                    atn::Vector3 front(0.0f, 0.0f, 1.0f);
                    Mtx m;
                    PSMTXRotRad(m, 'y', m9C->mRot.y);
                    PSMTXMultVec(m, &front, &front);
                    d = front;
                } else {
                    PSVECNormalize(&d, &d);
                }
                kind = lbl_8087F048->fn_800FDB20(m88, m9C, r, m68, m7C, &pos, &d, 10, m5C);
            }
            if ((m68->mAC & 0x8000) == 0x8000) {
                kind = -1;
            }
        }
        if (kind != 0 && (m04 & 0x1000000) == 0x1000000) {
            kind = -1;
        }
        if (kind >= 0) {
            atn::Vector3 angle = atn::Vector3(-back.x, -back.y, -back.z).ToRotation_unk();
            if (kind == 0 && m9C != 0 && (m9C->mStatus.m0C & 0x20) == 0x20) {
                lbl_8087F048->fn_80101434(&pos, &angle, m68->mC4, 1);
            } else {
                lbl_8087F048->fn_80101434(&pos, &angle, m68->mC4, kind);
            }
            if (m68 != 0 && m68->mB0 == 0x5BA) {
                fn_800C344C(sSeNames_unk[3], &pos, 0, -1, 1.0f);
            } else if (kind == 0) {
                fn_800C344C(sSeNames_unk[0], &pos, 0, -1, 1.0f);
            } else if (kind == 4) {
                fn_800C344C(sSeNames_unk[2], &pos, 0, -1, 1.0f);
            } else {
                fn_800C344C(sSeNames_unk[1], &pos, 0, -1, 1.0f);
            }
        }
        HitInfo_unk* skill = fn_80219E6C(m68->m6C);
        if (skill != 0 && fn_8021A960(skill) && (m04 & 0x80) != 0x80) {
            fn_8059B66C(lbl_8087F9E8, skill, m88, m9C, &pos, &rot, boost::function<void(SkillEffect*)>(), 1.0f);
        }
        ((GameTask*)lbl_8087F430)->fn_80373164(m88, &res->m04, stuck);
    }
    if ((m68->mAC & 0x8000) != 0x8000 && (int)m68->m02 != 9) {
        if ((m04 & 4) == 4) {
            if ((m04 & 0x20) == 0x20) {
                m94 = (float)m68->m60;
            } else {
                m94 = 30.0f;
            }
        } else {
            vf0C(1, 0);
        }
    }
    if ((int)m68->m02 != 9) {
        m08 = res->m10;
    }
    if ((m04 & 0x20) == 0x20) {
        if (m9C != 0 && m9C->CanAct_unk() && m9C->mFlag22 != 1) {
            atn::Vector3 d = m08 - m14;
            if (atn::Abs_unk(PSVECSquareMag(&d)) < 1.1920929e-7f) {
                d.Set_unk(0.0f, 0.0f, 0.1f);
            }
            atn::Vector3 push = d.GetNormalize() * m5C;
            m9C->fn_8015076C(m7C, push, pos, 1, 0, 0);
        }
        m08 = pos;
        if (m9C != 0) {
            MtxPtr node = m9C->GetModel_unk()->GetNodeMtx_unk("spine");
            if (node != 0) {
                atn::Matrix3x4 inv(node);
                PSMTXInverse(inv.m, inv.m);
                PSMTXMultVec(inv.m, &m08, &m08);
                if (PSVECMag(&m08) > 1.5f) {
                    PSVECNormalize(&m08, &m08);
                    m08 *= 1.5f;
                }
                inv.fn_8010F668();
                PSMTXMultVec(inv.m, &m20, &m20);
            }
        }
    }
    if (lbl_8087F498 != 0 && rand() % 2 == 0 && m68->m04 >= 0x148 && m68->m04 <= 0x14A && lbl_8087F430 != 0 &&
        !((GameTask*)lbl_8087F430)->fn_80375184() && m88 != 0) {
        atn::FixedArray_unk<CharaTask*, 16> near;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (near.size() >= 16) {
                break;
            }
            if (p != m88 && !p->fn_8013655C() && (p->mFlags & 4) != 4 && p->m7C != 0) {
                atn::Vector3 d = p->mPos - m88->mPos;
                if (PSVECSquareMag(&d) < 10000.0f) {
                    near.push_back(p);
                }
            }
        }
        for (NpcTask* n = NpcManager::sInstance->mHead; n != 0; n = n->mNext) {
            if (near.size() >= 16) {
                break;
            }
            if (n != m88 && !n->fn_8013655C() && (n->mFlags & 4) != 4 && n->m7C != 0) {
                atn::Vector3 d = n->mPos - m88->mPos;
                if (PSVECSquareMag(&d) < 10000.0f) {
                    near.push_back(n);
                }
            }
        }
        if (near.size() != 0) {
            lbl_8087F498->fn_803EAA7C(near[rand() % near.size()], 7, 10.0f);
        }
    }
    if (lbl_8087F430 != 0 && m68 != 0 && (m68->mAC & 0x40000) == 0x40000) {
        CameraShake_unk& shake = ((GameTask*)lbl_8087F430)->mCamera.mShake;
        shake.mFrame = shake.mFrame % 2;
        shake.mLength = 15;
        shake.mWidth = 1.0f;
        shake.mHeight = 1.0f;
    }
}

// inferred: removes the bullet when it hits the background (hit effect on the collision)
void Bullet::fn_801120B0() {
    unsigned int mask = 0x80000006;
    if (HasFlag_unk(2)) {
        mask |= 0x40000000;
    }
    atn::ColliResult_unk res;
    if (fn_8004ED34(lbl_8087EE98, &res, &m14, &m08, mask, 0, 0) != 0) {
        lbl_8087F048->fn_800FB4B0(m88, m68, m7C, &m08, 0, 0x1E, -1, 1);
        vf0C(1, 0);
    }
}

void Bullet::fn_80112174() {
    unsigned int mask = 0x80000006;
    if (HasFlag_unk(2)) {
        mask |= 0x40000000;
    }
    atn::ColliResult_unk res;
    if (fn_8004ED34(lbl_8087EE98, &res, &m14, &m08, mask, 0, 0) != 0) {
        ((SkillEffect*)m8C)->fn_80599FFC(&res.m10); // inferred: m8C is a skill effect
        vf0C(1, 0);
    }
}

int Bullet::vf1C(CharaTask* chara, const atn::Vector3* pos, void* arg) {
    return 0;
}
