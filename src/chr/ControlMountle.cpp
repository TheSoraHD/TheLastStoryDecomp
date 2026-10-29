// Climbing onto a ledge (ControlMountle, ControlMountleQuadruped), onto a guarding character
// (ControlGuardMountle) or a wall (ControlWallMountle), and the jump towards a ledge (ControlJump).
#include "chr/ControlMountle.h"
#include "game/HitInfo_unk.h"
#include "atn/Memory.h"
#include "macros.h"
#include <boost/bind.hpp>

extern "C" int rand();
extern "C" double cos(double x);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b); // inferred: dot product

// inferred: game state (lbl_8087EFA8, as in chr/ControlChant.cpp); names invented
struct MountleGameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" MountleGameState_unk* lbl_8087EFA8;

// inferred: game settings (lbl_8087F0A8, as in chr/ControlChant.cpp); names invented
struct MountleSettings_unk {
    unsigned char m000[0x30];
    int m30; // inferred: frames per second (the gravity is divided by its square)
    unsigned char m034[0x2C0 - 0x34];
    int m2C0; // inferred: any ledge can be jumped to (debug)
};
extern "C" MountleSettings_unk* lbl_8087F0A8;
// inferred: gravity per frame (as in gmk/Cannonball.cpp)
inline float Gravity_unk(MountleSettings_unk* settings) {
    return -98.1f / (float)(settings->m30 * settings->m30);
}

// inferred: collision world and its tests (see chr/ControlWallUp.cpp and game/Em401_01_WhiteLeoPard.cpp)
struct MountleCollResult_unk {
    MountleCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28; // inferred: normal
    int m34;
    int m38;
    unsigned int m3C; // inferred: attributes of the hit polygon (0x400: ledge)
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
// inferred: collision world (accessor 0x801404F8, as in gmk/Cannonball.cpp)
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" int fn_8004ED34(void* colli, MountleCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);
extern "C" int fn_8004D388(void* world, MountleCollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct MountleEffect_unk;
extern "C" MountleEffect_unk* lbl_8087F048;
extern "C" int fn_800F8548(MountleEffect_unk* mgr); // inferred: new bullet id
extern "C" int fn_800F8C6C(MountleEffect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                           int c, float f); // inferred: deals a hit

// inferred: voice/sound player (lbl_8087F498)
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);

// inferred: inline in the original (weak copies 0x8013C3B4, 0x8013C394, 0x8013C38C, 0x8013C460,
// 0x80139F24, 0x8013C554, 0x80139F3C, defined out of line in game/CharaTask.cpp): inlined by
// ControlWallMountle and called out of line by ControlGuardMountle (over the inline budget)
inline atn::Vector3 atn::Vector3::operator-() const {
    return atn::Vector3(-x, -y, -z);
}
inline atn::Vector3 atn::Vector3::fn_8013C394(const Vector3& a, const Vector3& b) {
    Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}
inline atn::Vector3* CharaTask::fn_8013C38C() {
    return &mPos;
}
inline void CharaTask::fn_8013C460(int value) {
    mFlag5 = value;
}
inline void atn::Model::fn_80139F24(int value) {
    m34C = value;
}
inline void atn::Model::fn_8013C554(int track, float weight) {
    mTracks[track].m20 = weight;
}
inline void atn::Model::SetMotionSpeed(int track, float speed) {
    mTracks[track].m0C = speed;
}

// inferred: front of a character with the rotation `rot` (as in game/Em306GurgBeast.cpp); name
// invented
inline atn::Vector3 Front_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}

// inferred: linear interpolation with the product first (as in chr/ControlWallUp.cpp); name invented
inline atn::Vector3 LerpMountle_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: cosine in float (weak copy 0x801C1060); name invented
inline float Cos_unk(float x) {
    return cos(x);
}

// inferred: tuning of ControlJump::fn_801C2198 (jump down, jump up); names invented
static float sDownHeight0 = 1.0f;
static float sDownHeight1 = 2.0f;
static float sDownSpeed0 = 1.5f;
static float sDownSpeed1 = 2.0f;
static float sDownTime0 = 10.0f;
static float sDownTime1 = 15.0f;
static float sUpHeight0 = 1.0f;
static float sUpHeight1 = 2.0f;
static float sUpSpeed0 = 1.5f;
static float sUpSpeed1 = 2.0f;
static float sUpTime0 = 10.0f;
static float sUpTime1 = 15.0f;

namespace chr {

ControlMountle::ControlMountle(Chara_unk* chara, const atn::Vector3& dir, float speed) : ControlBase(chara) {
    chara->SetControlId_unk(3);
    Motion_unk* m = &mChara->mMotion;
    m08 = dir;
    m20 = speed;
    mChara->SetRot_unk(dir.ToRotation_unk());
    m14 = mChara->mPos;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x60, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0xE, 0, 1.0f, 10.0f);
    }
    m24 = 9.0f;
    m28 = 5.0f;
    m2C = 38.0f;
}

int ControlMountle::vf0C() {
    float weight = 0.0f;
    int ret = 0;
    Motion_unk* m = &mChara->mMotion;
    if (m->mTracks[0].m08 > m28 && m->mTracks[0].m08 <= m2C) {
        weight = 1.0f;
    }
    float frame = m->mTracks[0].m08;
    if (frame <= m24) {
        MountleCollResult_unk res;
        float reach = mChara->m620;
        atn::Vector3 from = mChara->mPos;
        atn::Vector3 pos = mChara->mPos;
        float ts = lbl_8087EFA8->m3A4;
        pos += m08 * 1.0f * ts;
        pos.y += 9.0f / m24 * lbl_8087EFA8->m3A4;
        mChara->mPos = pos;
        atn::Vector3 to = mChara->mPos;
        from.y = to.y += 10.0f;
        to += m08 * reach;
        if (fn_8004ED34(lbl_8087EE98, &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
            pos = res.m04 - m08 * reach;
            Chara_unk* c = mChara;
            pos.y = c->mPos.y;
            c->mPos = pos;
        }
        return 0;
    }
    if (frame >= m->fn_80097D7C(0) - 5.0f) {
        if (mChara->m564 == 2) {
            mChara->fn_8015495C(0, 1, 1, 1);
        }
        ret = 1;
    }
    vf60();
    mChara->fn_8013CB68(m08.ToRotation_unk(), 0, weight, m20);
    return ret;
}

void ControlMountle::vf60() {}

atn::Vector3 ControlMountle::vf14() {
    Chara_unk* c = mChara;
    atn::Vector3 pos = c->mPos;
    float frame = c->mMotion.mTracks[0].m08;
    if (frame <= 20.0f) {
        pos.y = m14.y;
    } else if (frame <= 30.0f) {
        float t = (30.0f - frame) / 10.0f;
        pos.y = t * (m14.y - c->mPos.y) + c->mPos.y;
    }
    return pos;
}

ControlMountleQuadruped::ControlMountleQuadruped(Chara_unk* chara, const atn::Vector3& dir, float speed)
    : ControlMountle(chara, dir, speed) {
    m24 = 5.0f;
    m28 = -1.0f;
    m2C = 26.0f;
}

ControlMountle::~ControlMountle() {}

void ControlMountleQuadruped::vf60() {
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame > 22.0f && frame <= 26.0f) {
        int id = fn_800F8548(lbl_8087F048);
        fn_800F8C6C(lbl_8087F048, 0, 0, mChara, mChara->m638, id, 0x1E, -1, 0.0f);
    }
}

ControlGuardMountle::ControlGuardMountle(Chara_unk* chara, const atn::Vector3& dir) : ControlBase(chara) {
    mChara->SetControlId_unk(0x2D);
    atn::Model* model = mChara->GetModel_unk();
    mChara->fn_8013C460(0);
    mChara->SetFlag4_unk(false);
    mChara->fn_801446F0();
    m08 = dir;
    float d = fn_801A03E8(mChara->fn_8014052C(), dir);
    if (d > Cos_unk(0.7853982f)) {
        model->fn_80097C08(0, 0x43, 0, 0, 1, 0.0f, 0.1f);
        mChara->SetRot_unk(m08.ToRotation_unk());
    } else if (d > Cos_unk(2.3561945f)) {
        float yaw = dir.ToRotation_unk().y;
        if (atn::WrapAngle_unk(yaw - mChara->GetRot_unk()->y) > 0.0f) {
            model->fn_80097C08(0, 0x41, 0, 0, 1, 0.0f, 0.1f);
            mChara->SetRot_unk(atn::Vector3::fn_8013C394(m08, atn::Vector3(0.0f, 1.0f, 0.0f)).ToRotation_unk());
        } else {
            model->fn_80097C08(0, 0x44, 0, 0, 1, 0.0f, 0.1f);
            mChara->SetRot_unk(atn::Vector3::fn_8013C394(m08, atn::Vector3(0.0f, -1.0f, 0.0f)).ToRotation_unk());
        }
    } else {
        model->fn_80097C08(0, 0x42, 0, 0, 1, 0.0f, 0.1f);
        mChara->SetRot_unk((-m08).ToRotation_unk());
    }
    m20 = 40.0f / model->fn_80097D7C(0);
    m14 = *mChara->fn_8013C38C();
    m24 = 0.0f;
    model->fn_80139F24(1);
    model->fn_8013C554(0, 1.0f);
    model->SetMotionSpeed(0, 1.0f);
}

// NONMATCHING: 97.2% - register allocation of the fall step (order of the products of move, gravity
// in f12, chara reload for &mColli, src/dst of the copy of res.m10); ~12 forms tried.
int ControlGuardMountle::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    if (m->mTracks[0].m18) {
        return 1;
    }
    float frame = m->mTracks[0].m08;
    if (frame < m->fn_80097D7C(0)) {
        MountleCollResult_unk res;
        if (m->mTracks[0].m08 < 19.0f) {
            float reach = mChara->m620;
            atn::Vector3 from = mChara->mPos;
            atn::Vector3 pos = mChara->mPos;
            float speed = m->mTracks[0].m0C;
            pos += m08 * m20 * lbl_8087EFA8->m3A4 * speed;
            pos.y += 9.0f / 19.0f * lbl_8087EFA8->m3A4 * speed;
            mChara->mPos = pos;
            atn::Vector3 to = mChara->mPos;
            from.y = to.y += 10.0f;
            to += m08 * reach;
            if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
                Chara_unk* c = mChara;
                pos = res.m04 - m08 * reach;
                pos.y = c->mPos.y;
                c->mPos = pos;
            }
            return 0;
        }
        float speed = m->mTracks[0].m0C;
        atn::Vector3 move = m08 * m20 * lbl_8087EFA8->m3A4 * speed;
        m24 += lbl_8087EFA8->m3A4 * (-98.1f / (lbl_8087F0A8->m30 * lbl_8087F0A8->m30));
        move.y = m24 - 1.0f;
        atn::Vector3 old = mChara->mPos;
        CharaTask::Sphere_unk sphere = mChara->fn_80176548(mChara->mPos);
        unsigned int mask = mChara->fn_80179D44();
        int hit = fn_8004D388(lbl_8087EE98, &res, &sphere, &move, mask, &mChara->mColli, 0, sphere.mRadius);
        Chara_unk* c = mChara;
        atn::Vector3 pos = res.m10;
        pos -= c->m5A4;
        pos.y -= sphere.mRadius;
        c->mPos = pos;
        if (hit && old.y - pos.y < 0.95f * -move.y) {
            move.y = 0.0f;
            m24 = 0.0f;
        }
        mChara->m574 = move;
        return 0;
    }
    if (mChara->m564 == 2) {
        mChara->fn_8015495C(0, 1, 1, 1);
    }
    mChara->fn_8013CB68(mChara->mRot, 0, 1.0f, m20);
    return 1;
}

atn::Vector3 ControlGuardMountle::vf14() {
    Chara_unk* c = mChara;
    atn::Vector3 pos = c->mPos;
    float frame = c->mMotion.mTracks[0].m08;
    if (frame < 19.0f) {
        pos.y = m14.y;
    } else if (frame < 29.0f) {
        float t = (frame - 19.0f) / 10.0f;
        pos.y = t * (c->mPos.y - m14.y) + m14.y;
    }
    return pos;
}

// NONMATCHING: 99.4% - r3/r4 swapped in the copies dir = res.m28 and dir = -front, and the store of
// from.z scheduled earlier (copy src/dst order, see docs/mwcc-blockers.md q3).
ControlWallMountle::ControlWallMountle(Chara_unk* chara, const atn::Vector3& normal, const atn::Vector3& pos,
                                       int jump)
    : ControlBase(chara) {
    chara->m560 = 0x2D;
    atn::Model* m = mChara->GetModel_unk();
    mChara->fn_8013C460(0);
    mChara->SetFlag4_unk(false);
    mChara->fn_801446F0();
    m->fn_80097C08(0, 0x213, 0, 0, 1, 0.0f, 0.1f);
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.2f;
    if (jump) {
        m->mTracks[0].m08 = 10.0f;
    }
    m38 = 0.0f;
    m20 = pos;
    m14 = mChara->mPos;
    m20.y = m14.y;
    atn::Vector3 from = m14;
    MountleCollResult_unk res;
    atn::Vector3 to = pos + normal;
    from.y = to.y;
    atn::Vector3 dir = -normal;
    if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
        dir = res.m28;
    }
    dir.y = 0.0f;
    if (PSVECSquareMag(&dir) > 0.001f) {
        PSVECNormalize(&dir, &dir);
    } else {
        dir = -Front_unk(mChara->mRot);
    }
    m08 = -dir;
    m20 -= m08 * 3.0f;
    if (!jump) {
        m20.y += 5.0f;
    }
    m2C = m20 + m08 * 10.0f;
    from = m2C;
    to = m2C;
    from.y += 35.0f;
    if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
        m2C = res.m04;
    } else {
        m2C = m20;
        m2C.y += 20.0f;
    }
    if (jump) {
        m20.y = m2C.y - 25.0f;
    }
    mChara->mRot = m08.ToRotation_unk();
}

// NONMATCHING: 98.5% - float registers: speed/time scale loaded in the other order and t in f6
// instead of f8; ~10 forms tried (operand orders, MIN as a function, locals).
int ControlWallMountle::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    m38 += m->mTracks[0].m0C * lbl_8087EFA8->m3A4;
    float t = MIN(1.0f, m->mTracks[0].m08 / 40.0f);
    atn::Vector3 pos = mChara->mPos;
    if (m38 < 8.0f) {
        float s = m38 / 8.0f;
        pos = LerpMountle_unk(m14, m20, s);
        pos.y = s * t * (m2C.y - pos.y) + pos.y;
    } else {
        pos.y = (m2C.y - m20.y) * t + m20.y;
        pos.x = (m2C.x - m20.x) * MIN(1.0f, MAX(0.0f, (m->mTracks[0].m08 - 30.0f) / 10.0f)) + m20.x;
        pos.z = (m2C.z - m20.z) * MIN(1.0f, MAX(0.0f, (m->mTracks[0].m08 - 30.0f) / 10.0f)) + m20.z;
    }
    mChara->mPos = pos;
    float frame = m->mTracks[0].m08;
    return frame >= m->fn_80097D7C(0) - 3.0f;
}

atn::Vector3 ControlWallMountle::vf14() {
    return mChara->mPos;
}

// NONMATCHING: 99.9% - r3/r4 swapped in mChara->m6B8 = out (copy src/dst order, as in
// ControlWallMountle).
ControlJump::ControlJump(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& dir)
    : ControlBase(chara), m34(0.0f) {
    chara->m560 = 0x2D;
    Motion_unk* m = &mChara->mMotion;
    mChara->fn_8013C460(0);
    mChara->SetFlag4_unk(false);
    mChara->fn_801446F0();
    m->fn_80097C08(0, 0x214, 0, 1, 1, 0.0f, 0.1f);
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    m->mTracks[0].m08 = 0.0f;
    m08 = dir;
    m14 = pos;
    atn::Vector3 out(0.0f, 0.0f, 0.0f);
    m2C = 1.3f;
    bool found = fn_801C2198(out, dir);
    if (!found) {
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', 0.08726646f);
        atn::Vector3 d = dir;
        PSMTXMultVec(mtx, &d, &d);
        found = fn_801C2198(out, d);
    }
    if (!found) {
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', -0.08726646f);
        atn::Vector3 d = dir;
        PSMTXMultVec(mtx, &d, &d);
        found = fn_801C2198(out, d);
    }
    m30 = found;
    if (!m30) {
        mChara->m570 = m2C;
        mChara->m6B8 = out;
    } else {
        mChara->m570 = 1.0f;
    }
    mChara->mRot = m08.ToRotation_unk();
}

// NONMATCHING: 99.7% - operand order of the fmadds of the clamped interpolations and r4-r7 in the
// copies to/m20 (copy src/dst order); ~8 forms tried.
bool ControlJump::fn_801C2198(atn::Vector3& out, const atn::Vector3& dir) {
    Chara_unk* c = mChara;
    float reach = 65.0f;
    MountleCollResult_unk res;
    m31 = false;
    Motion_unk* m = &c->mMotion;
    bool ret = false;
    atn::Vector3 from = c->mPos;
    atn::Vector3 to;
    to = from + dir * reach;
    from.y -= 3.0f;
    to.y -= 3.0f;
    if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
        if ((res.m3C & 0x400) == 0x400 || lbl_8087F0A8->m2C0 != 0) {
            to = res.m04 - res.m28 * 5.0f;
            float dist = (to - from).Length_unk();
            out.y = (sDownHeight1 - sDownHeight0) * MIN(1.0f, MAX(0.0f, (dist - 20.0f) / 40.0f)) + sDownHeight0;
            m2C = (sDownSpeed1 - sDownSpeed0) * MIN(1.0f, MAX(0.0f, (dist - 20.0f) / 40.0f)) + sDownSpeed0;
            m20 = to;
            m20.y = mChara->mPos.y;
            m08 = to - from;
            m08.y = 0.0f;
            PSVECNormalize(&m08, &m08);
            m38 = (sDownTime1 - sDownTime0) * MIN(1.0f, MAX(0.0f, (dist - 30.0f) / 40.0f)) + sDownTime0;
            m->mTracks[0].m0C = 15.0f / m38;
            m38 = m->fn_80097D7C(0);
            ret = true;
        }
    }
    from = mChara->mPos;
    to = from + dir * reach;
    from.y += 3.0f;
    to.y += 3.0f;
    if (fn_8004ED34(fn_801404F8(), &res, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
        if ((res.m3C & 0x400) == 0x400 || lbl_8087F0A8->m2C0 != 0) {
            float dist = (res.m04 - from).Length_unk();
            out.y = (sUpHeight1 - sUpHeight0) * MIN(1.0f, MAX(0.0f, (dist - 20.0f) / 40.0f)) + sUpHeight0;
            m2C = (sUpSpeed1 - sUpSpeed0) * MIN(1.0f, MAX(0.0f, (dist - 20.0f) / 40.0f)) + sUpSpeed0;
            m20 = res.m04;
            m20.y = mChara->mPos.y + 10.0f;
            m08 = to - from;
            m08.y = 0.0f;
            PSVECNormalize(&m08, &m08);
            m38 = (sUpTime1 - sUpTime0) * MIN(1.0f, MAX(0.0f, (dist - 30.0f) / 50.0f)) + sUpTime0;
            m->mTracks[0].m0C = 15.0f / m38;
            m38 = m->fn_80097D7C(0);
            ret = true;
            m31 = true;
        }
    }
    return ret;
}

// NONMATCHING: 99.8% - f3/f4 swapped in the load of the motion speed and the time scale (same
// as ControlWallMountle::vf0C).
int ControlJump::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    m34 += m->mTracks[0].m0C * lbl_8087EFA8->m3A4;
    if (m30) {
        float a = MIN(1.0f, MAX(0.0f, (m34 - 25.0f) / 8.0f));
        float b = MIN(1.0f, m34 / 25.0f);
        float w = 0.9f * b + 0.1f * a;
        mChara->mPos = LerpMountle_unk(m14, m20, w);
        if (m31) {
            return w >= 0.85f;
        }
        float frame = m->mTracks[0].m08;
        return frame > m->fn_80097D7C(0) - 8.0f;
    }
    if (m34 > 21.0f) {
        m2C *= 0.7f;
    }
    mChara->fn_8013CB68(m08.ToRotation_unk(), 0, 1.0f, m2C);
    if (m->mTracks[0].m08 > 25.0f) {
        m->mTracks[0].m0C = 1.5f;
    } else if (m->mTracks[0].m08 > 11.0f) {
        if (mChara->m574.y < -0.01f) {
            m->mTracks[0].m0C = MAX(0.1f, m->GetMotionSpeed(0) - 0.1f);
        } else {
            m->mTracks[0].m0C = 1.0f;
        }
    }
    float frame = m->mTracks[0].m08;
    return frame > m->fn_80097D7C(0) - 8.0f;
}

atn::Vector3 ControlJump::vf14() {
    return mChara->mPos;
}

boost::function<void()> ControlJump::vf10() {
    if (m31) {
        if (mChara->mPos.y >= 90.0f) {
            return boost::bind(&CharaTask::fn_801591E8, (CharaTask*)mChara, Front_unk(mChara->mRot));
        }
        ControlWallMountle* next = new ((MemoryTag)0, "", "", 0) ControlWallMountle(mChara, m08, m20, 1);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    return ControlBase::vf10();
}

} // namespace chr
