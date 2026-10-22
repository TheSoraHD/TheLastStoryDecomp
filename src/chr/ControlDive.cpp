#include "chr/ControlDive.h"
#include "game/CharaTask.h"

extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" double fmod(double x, double y);

// inferred: field object (lbl_8087F430): route tables at 0x10D8 (class unknown)
struct DiveField_unk {
    unsigned char m0000[0x10D8];
    void* m10D8;
};
extern "C" DiveField_unk* lbl_8087F430;
// inferred: nearest route point (stored in RouteWalker_unk::mDestPoint)
extern "C" int fn_803C1560(void* routes, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5, float f);
// inferred: finds the ledge of route link `link` near `chara`
extern "C" int fn_8036DAF4(DiveField_unk* field, Ledge_unk* ledge, CharaTask* chara, int arg, int link);

// inferred: camera/character manager (lbl_8087F048, class unknown)
extern "C" void* lbl_8087F048;
extern "C" float fn_8010CA3C(void* obj, float f);
extern "C" void fn_801065E4(void* obj, CharaTask* chara);
extern "C" void fn_8010653C(void* obj, CharaTask* chara, void* motion);

// inferred: voice/sound player (lbl_8087F498); fn_803EA77C plays voice `id` of a character
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);

// inferred: game state (lbl_8087EFA8): m3A4 is the time scale
struct DiveGameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" DiveGameState_unk* lbl_8087EFA8;

// inferred: pad state at +0x48C of the object behind lbl_8087F0A8 (class unknown)
struct DiveInput_unk {
    unsigned char m000[0x48C];
    unsigned char mInput[1];
};
extern "C" DiveInput_unk* lbl_8087F0A8;
extern "C" int fn_801240B4(void* pad, int button); // inferred: button held

// inferred: moves the back step destination out of the walls (chr/ControlCharge.cpp)
extern "C" void fn_801A4E08(CharaTask* chara, atn::Vector3* pos, atn::Vector3* dest);

// inferred: starts motion `id` on track 0 (name invented)
inline void PlayDiveMotion_unk(chr::Motion_unk* m, int id, float speed) {
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, id, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = speed;
}

// inferred: point of the route tables nearest to the character (name invented)
inline int FindRoutePoint_unk(void* routes, CharaTask* chara) {
    return fn_803C1560(routes, &chara->mPos, chara->fn_80179D44(), chara->mC58.mB4, chara->mC58.m0C.mA0, 1, -1.0f);
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
        fn_803EA77C(lbl_8087F498, mChara, 0, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 85.0% - the original loads the arguments of fn_803C1560 before calling
// fn_80179D44 (r27-r31 saved, larger frame) and loads 1.5f after the prologue.
int ControlDive::vf0C() {
    int ret = 0;
    Motion_unk* motion = &mChara->mMotion;
    float speed = fn_8010CA3C(lbl_8087F048, 1.5f);
    motion->mTracks[0].m0C = speed / 1.5f;
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 5.0f) {
        if (mChara->m564 == 2) {
            mChara->mC58.m0C.mDestPoint = fn_803C1560(lbl_8087F430->m10D8, &mChara->mPos, mChara->fn_80179D44(),
                                                      mChara->mC58.mB4, mChara->mC58.m0C.mA0, 1, -1.0f);
        }
        mChara->mStatus.m18 &= ~0x20000;
        ret = 1;
    }
    atn::Vector3 rot = m08.ToRotation_unk();
    mChara->fn_8013CB68(&rot, 0, 1.0f, speed);
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

// NONMATCHING: 99.84% - register swap (r4/r5) in the difference vector.
int ControlSwatTurn::vf0C() {
    int ret = 0;
    atn::Vector3 d = m08 - mChara->mPos;
    atn::Vector3 rot = d.ToRotationZ_unk();
    float len = PSVECMag(&d);
    float speed = 1.0f;
    if (len > 16.0f) {
        speed *= 1.5f;
    } else if (len < 8.0f) {
        speed *= 0.7f;
    }
    Ledge_unk ledge;
    if (len < 5.0f && fn_8036DAF4(lbl_8087F430, &ledge, mChara, 0, m14) != 0) {
        mChara->fn_80153698(&ledge, 0);
        ret = 1;
    } else if (++m18 > 60) {
        ret = 1;
    }
    mChara->fn_8013CB68(&rot, 0, 1.0f, speed);
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

// NONMATCHING: 98.9% - `ret` is in r31 (r30 in the original, shared with the character).
int ControlCoverSlip::vf0C() {
    int ret = 0;
    Motion_unk* motion = &mChara->mMotion;
    if (++m14 < 7) {
        atn::Vector3 saved = mChara->mRot;
        mChara->mRot = m08.ToRotation_unk();
        mChara->fn_8013CB68(&mChara->mRot, 0, 1.0f, 1.5f);
        mChara->mRot = saved;
        return 0;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 5.0f) {
        CharaTask* chara = mChara;
        if (chara->m564 == 2) {
            mChara->mC58.m0C.mDestPoint = FindRoutePoint_unk(lbl_8087F430->m10D8, chara);
        }
        ret = 1;
    }
    mChara->fn_8013CB68(&mChara->mRot, 0, 1.0f, 1.5f);
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
        fn_803EA77C(lbl_8087F498, mChara, 0xC, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 99.8% - registers r3-r6 in the copies of pos/rot/vel to the character.
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
            if (chara->mType != 0 || fn_801240B4(lbl_8087F0A8->mInput, 7)) {
                m14 = 1;
            } else {
                m14 = 2;
            }
        } else if (m14 == 1) {
            crot = rot;
        }
        mChara->mPos = pos;
        mChara->mRot = crot;
        mChara->m574 = vel;
        mChara->fn_8013CB68(&rot, 1, scale, scale * mChara->m568);
        if (m14 == 1) {
            crot = start;
            crot.y = atn::WrapAngle_unk(crot.y + 3.1415927f / (motion->fn_80097D7C(0) - 19.0f));
            mChara->mRot = crot;
        }
        return 0;
    }
    mChara->vf34(&rot, 0, scale, scale);
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
        fn_801A4E08(mChara, &m14, &m08);
    }
    atn::Vector3 dir2 = NegDive_unk(m08 - m14);
    dir2.y = 0.0f;
    if (fn_805F9920(&dir2) > 0.00001f) {
        mChara->mRot = dir2.ToRotation_unk();
    }
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0xC, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 97.4% - `ret` is in r31 (r30 in the original) and the m20 update loads in other order.
int ControlBackStep::vf0C() {
    int ret = 0;
    m20 -= lbl_8087EFA8->m3A4;
    Motion_unk* motion = &mChara->mMotion;
    float t = 0.0f > m20 / 20.0f ? 0.0f : m20 / 20.0f;
    mChara->mPos = LerpDive_unk(m08, m14, t);
    if (m20 <= 0.0f) {
        motion->mTracks[0].m0C = 1.0f;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        CharaTask* chara = mChara;
        if (chara->m564 == 2) {
            mChara->mC58.m0C.mDestPoint = FindRoutePoint_unk(lbl_8087F430->m10D8, chara);
        }
        ret = 1;
    }
    return ret;
}

void ControlBackStep::vf1C() {
    if (lbl_8087F048 != 0) {
        fn_801065E4(lbl_8087F048, mChara);
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
        fn_8010653C(lbl_8087F048, mChara, &mChara->mMotion);
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
                fn_801065E4(lbl_8087F048, mChara);
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
        fn_801065E4(lbl_8087F048, mChara);
    }
}

} // namespace chr
