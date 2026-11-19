#include "atn/RenderSetting_unk.h"
#include "chr/ControlDive.h"
#include "game/GameTask.h"
#include "game/GameVoice.h"
#include "game/GameScene.h"
#include "chr/ControlCharge.h"
#include "game/CharaTask.h"
#include "game/Input_unk.h"
#include "game/BattleTask.h"

extern "C" int rand();
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" double fmod(double x, double y);

extern "C" GameTask* lbl_8087F430; // inferred: the field
// inferred: nearest route point (stored in RouteWalker_unk::mDestPoint)
// inferred: finds the ledge of route link `link` near `chara`

extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager


extern "C" RenderSetting_unk* lbl_8087EFA8;

// inferred: pad state at +0x48C of the object behind lbl_8087F0A8 (class unknown)
struct DiveInput_unk {
    unsigned char m000[0x48C];
    unsigned char mInput[1];
};
extern "C" DiveInput_unk* lbl_8087F0A8;

// inferred: moves the back step destination out of the walls (chr/ControlCharge.cpp)

// inferred: starts motion `id` on track 0 (name invented)
inline void PlayDiveMotion_unk(chr::Motion_unk* m, int id, float speed) {
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, id, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = speed;
}

// inferred: point of the route tables nearest to the character (name invented)
inline int FindRoutePoint_unk(void* routes, CharaTask* chara) {
    return ((GameScene*)routes)->fn_803C1560(&chara->mPos, chara->fn_80179D44(), chara->mC58.mB4, chara->mC58.m0C.mA0, 1, -1.0f);
}

// inferred: negated vector (name invented)
inline atn::Vector3 NegDive_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: linear interpolation with the product first (name invented)
inline atn::Vector3 LerpDive_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: lets go of the ledge (name invented)
inline void LeaveLedge_unk(CharaTask* chara) {
    if (chara->mFlag31) {
        chara->fn_801539E0();
    }
}

// inferred: weak copy 0x8013C38C (address of the position), inlined here
inline atn::Vector3* CharaTask::fn_8013C38C() {
    return &mPos;
}

namespace chr {

ControlDive::ControlDive(Chara_unk* chara, const atn::Vector3& dir) : ControlBase(chara) {
    chara->m560 = 2;
    mChara->mStatus.m18 |= 0x20000;
    m08 = dir;
    Motion_unk* m = &mChara->mMotion;
    LeaveLedge_unk(mChara);
    mChara->mFlag5 = 0;
    PlayDiveMotion_unk(m, 0x61, 1.0f);
    m14 = mChara->mRot;
    if (mChara->mType == 0 && mChara->m570 < 1.2f) {
        mChara->m570 = 1.2f;
    }
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        lbl_8087F498->fn_803EA77C(mChara, 0, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 85.0% - the original loads the arguments of fn_803C1560 before calling
// fn_80179D44 (r27-r31 saved, larger frame) and loads 1.5f after the prologue.
int ControlDive::vf0C() {
    int ret = 0;
    Motion_unk* motion = &mChara->mMotion;
    float speed = lbl_8087F048->fn_8010CA3C(mChara, 1.5f);
    motion->mTracks[0].m0C = speed / 1.5f;
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 5.0f) {
        if (mChara->m564 == 2) {
            mChara->mC58.m0C.mDestPoint = ((GameScene*)lbl_8087F430->m10D8)->fn_803C1560(&mChara->mPos, mChara->fn_80179D44(),
                                                      mChara->mC58.mB4, mChara->mC58.m0C.mA0, 1, -1.0f);
        }
        mChara->mStatus.m18 &= ~0x20000;
        ret = 1;
    }
    atn::Vector3 rot = m08.ToRotation_unk();
    mChara->fn_8013CB68(rot, 0, 1.0f, speed);
    return ret;
}

void ControlDive::vf1C() {
    mChara->mStatus.m18 &= ~0x20000;
}

ControlSwatTurn::ControlSwatTurn(Chara_unk* chara, const atn::Vector3& pos, int link)
    : ControlBase(chara), m14(link) {
    chara->m560 = 0;
    m08 = pos;
    m18 = 0;
    LeaveLedge_unk(mChara);
}

int ControlSwatTurn::vf0C() {
    int ret = 0;
    atn::Vector3 d = m08 - *mChara->fn_8013C38C();
    atn::Vector3 rot = d.ToRotationZ_unk();
    float len = PSVECMag(&d);
    float speed = 1.0f;
    if (len > 16.0f) {
        speed *= 1.5f;
    } else if (len < 8.0f) {
        speed *= 0.7f;
    }
    GameSceneRoute_unk ledge;
    if (len < 5.0f && lbl_8087F430->fn_8036DAF4(&ledge, mChara, 0, m14) != 0) {
        mChara->fn_80153698(&ledge, 0);
        ret = 1;
    } else if (++m18 > 60) {
        ret = 1;
    }
    mChara->fn_8013CB68(rot, 0, 1.0f, speed);
    return ret;
}

ControlCoverSlip::ControlCoverSlip(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 1;
    Motion_unk* m = &mChara->mMotion;
    int side = mChara->fn_80153B44();
    m08 = mChara->mC14;
    if (side == 1) {
        m08 = NegDive_unk(m08);
    }
    m14 = 0;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    PlayDiveMotion_unk(m, 0x61, 1.0f);
}

int ControlCoverSlip::vf0C() {
    int ret = 0;
    Motion_unk* motion = &mChara->mMotion;
    if (++m14 < 7) {
        atn::Vector3 saved = mChara->mRot;
        mChara->mRot = m08.ToRotation_unk();
        mChara->fn_8013CB68(mChara->mRot, 0, 1.0f, 1.5f);
        mChara->mRot = saved;
        return 0;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 5.0f) {
        if (mChara->m564 == 2) {
            mChara->mC58.m0C.mDestPoint = FindRoutePoint_unk(lbl_8087F430->m10D8, mChara);
        }
        ret = 1;
    }
    mChara->fn_8013CB68(mChara->mRot, 0, 1.0f, 1.5f);
    return ret;
}

ControlStool::ControlStool(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 0x47;
    m08 = 30;
    Motion_unk* m = &mChara->mMotion;
    mChara->mMotion.m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, mChara->m484[4], 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

int ControlStool::vf0C() {
    return --m08 <= 0;
}

ControlSwapPos::ControlSwapPos(Chara_unk* chara, const atn::Vector3& dir) : ControlBase(chara), m08(dir), m14(0) {
    chara->m560 = 0x3F;
    PlayDiveMotion_unk(&mChara->mMotion, 0x62, 1.0f);
    if (lbl_8087F498 != 0) {
        lbl_8087F498->fn_803EA77C(mChara, 0xC, 0, 1.0f, 10.0f);
    }
}

// The position and rotation are stored through the accessors 0x8013C38C and 0x8012A1B8.
int ControlSwapPos::vf0C() {
    float scale = 1.0f;
    Motion_unk* motion = &mChara->mMotion;
    atn::Vector3 rot = m08.ToRotation_unk();
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 5.0f) {
        CharaTask* chara = mChara;
        if (chara->m564 == 2) {
            mChara->mC58.m0C.mDestPoint = FindRoutePoint_unk(lbl_8087F430->m10D8, chara);
        }
    } else {
        CharaTask* chara = mChara;
        atn::Vector3 pos = chara->mPos;
        atn::Vector3 crot = chara->mRot;
        atn::Vector3 vel = chara->m574;
        atn::Vector3 start = crot;
        if (motion->mTracks[0].m08 <= 19.0f) {
            pos.y += 0.47368422f;
            vel.y = 0.0f;
        } else if (m14 == 0) {
            if (chara->mType != 0 || ((Input_unk*)lbl_8087F0A8->mInput)->fn_801240B4(7)) {
                m14 = 1;
            } else {
                m14 = 2;
            }
        } else if (m14 == 1) {
            crot = rot;
        }
        *mChara->fn_8013C38C() = pos;
        *mChara->GetRot_unk() = crot;
        mChara->m574 = vel;
        mChara->fn_8013CB68(rot, 1, scale, scale * mChara->m568);
        if (m14 == 1) {
            crot = start;
            crot.y = atn::WrapAngle_unk(crot.y + 3.1415927f / (motion->fn_80097D7C(0) - 19.0f));
            mChara->mRot = crot;
        }
        return 0;
    }
    mChara->vf34(rot, 0, scale, scale);
    return 1;
}

// NONMATCHING: 91.6% - r30/r31 swapped (motion/a) and the order of the subtractions of the
// negated direction.
ControlBackStep::ControlBackStep(Chara_unk* chara, const atn::Vector3& dir, int a)
    : ControlBase(chara), m08(dir), m20(15.0f) {
    chara->m560 = 2;
    Motion_unk* m = &mChara->mMotion;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    PlayDiveMotion_unk(m, 0x1EA, 0.5f);
    m14 = mChara->mPos;
    if (a != 0) {
        ControlChargeAttackBase::fn_801A4E08(mChara, m14, m08);
    }
    atn::Vector3 dir2 = NegDive_unk(m08 - m14);
    dir2.y = 0.0f;
    if (PSVECSquareMag(&dir2) > 0.00001f) {
        mChara->mRot = dir2.ToRotation_unk();
    }
    if (lbl_8087F498 != 0) {
        lbl_8087F498->fn_803EA77C(mChara, 0xC, 0, 1.0f, 10.0f);
    }
}

int ControlBackStep::vf0C() {
    int ret = 0;
    float scale = lbl_8087EFA8->m3A4;
    m20 -= scale;
    Motion_unk* motion = mChara->GetModel_unk();
    float t = 0.0f > m20 / 20.0f ? 0.0f : m20 / 20.0f;
    mChara->mPos = LerpDive_unk(m08, m14, t);
    if (m20 <= 0.0f) {
        motion->mTracks[0].m0C = 1.0f;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        if (mChara->m564 == 2) {
            mChara->mC58.m0C.mDestPoint = FindRoutePoint_unk(lbl_8087F430->m10D8, mChara);
        }
        ret = 1;
    }
    return ret;
}

void ControlBackStep::vf1C() {
    if (lbl_8087F048 != 0) {
        lbl_8087F048->fn_801065E4(mChara);
    }
    mChara->mPos = m08;
}

ControlWarp::ControlWarp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& rot)
    : ControlBase(chara), m08(pos), m14(rot), m20(0) {
    chara->m560 = 2;
    Motion_unk* m = &mChara->mMotion;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    PlayDiveMotion_unk(m, 0x1EA, 1.0f);
    if (lbl_8087F048 != 0) {
        lbl_8087F048->fn_8010653C(mChara, &mChara->mMotion);
    }
}

int ControlWarp::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    int ret = 0;
    switch (m20) {
    case 0: {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            motion->mTracks[0].m08 = 0.0f;
            m20 = 1;
            mChara->mPos = m08;
            mChara->mRot = m14;
        }
        break;
    }
    case 1: {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            CharaTask* chara = mChara;
            if (chara->m564 == 2) {
                mChara->mC58.m0C.mDestPoint = FindRoutePoint_unk(lbl_8087F430->m10D8, chara);
            }
            if (lbl_8087F048 != 0) {
                lbl_8087F048->fn_801065E4(mChara);
            }
            ret = 1;
        }
        break;
    }
    }
    return ret;
}

void ControlWarp::vf1C() {
    if (lbl_8087F048 != 0) {
        lbl_8087F048->fn_801065E4(mChara);
    }
}

} // namespace chr
