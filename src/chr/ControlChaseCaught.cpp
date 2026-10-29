// Controls of the chase mode: caught, held and released by a guard; catching, holding and
// releasing a character; moving beside and following the leader.
#include "chr/ControlChaseCaught.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include <boost/bind.hpp>

// inferred: game settings (as in chr/ControlCdnStone.cpp)
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero while `button` is pressed
};
struct Settings_unk {
    unsigned char m000[0x48C];
    Input_unk mInput; // 0x48C
    unsigned char m490[0x520 - 0x490];
    float m520; // inferred: rate of the damage while held
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: event state (lbl_8087F098, as in gmk/LemonBucket.cpp); names invented
struct ChaseEvent_unk {
    unsigned char m00[0x48];
    int m48; // inferred: chase mode (2: the leader is set)
    unsigned char m4C[0xF8 - 0x4C];
    CharaTask* mF8; // inferred: leader
};
extern "C" ChaseEvent_unk* lbl_8087F098;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: arena of the multiplayer mode (lbl_8087F490, as in MP/D2AMpMenuCockpit.cpp)
struct ChaseArena_unk {
    unsigned char m000[0x728];
    int m728; // inferred: set while a player is held
};
extern "C" ChaseArena_unk* lbl_8087F490;
extern "C" void* lbl_8087F430; // inferred: field state
extern "C" int fn_80370174(void* field, int id);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" double fmod(double x, double y);
extern "C" double sqrt(double x);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length

// inferred: facing direction of the rotation `rot` (name invented, as in chr/ControlCdnStone.cpp)
inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', rot.y);
    PSMTXMultVec(mtx, &v, &v);
    return v;
}

// inferred: a x b by value (name invented)
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: *this = *this * rot(axis, rad) (name invented, as in game/CharaTask.cpp)
inline void Rotate_unk(atn::Matrix3x4& mtx, char axis, float rad) {
    Mtx r;
    Mtx t;
    PSMTXRotRad(r, axis, rad);
    PSMTXConcat(mtx.m, r, t);
    __MtxCopy(mtx.m, t);
}

// inferred: mtx = rot_z * rot_y * rot_x of `rot` (name invented, as in chr/ControlBase.cpp)
inline void SetRotation_unk(atn::Matrix3x4& mtx, const atn::Vector3& rot) {
    mtx.SetIdentity();
    if (0.0f != rot.z) {
        Rotate_unk(mtx, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        Rotate_unk(mtx, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        Rotate_unk(mtx, 'x', rot.x);
    }
}

// inferred: `from` moved toward `to` by the fraction `t` (name invented)
inline atn::Vector3 Approach_unk(const atn::Vector3& from, const atn::Vector3& to, float t) {
    return (to - from) * t + from;
}

// inferred: inline in the original (weak copy 0x8013C3B4, defined out of line in game/CharaTask.cpp)
inline atn::Vector3 atn::Vector3::operator-() const {
    return atn::Vector3(-x, -y, -z);
}

namespace chr {

ControlChaseCaught::ControlChaseCaught(Chara_unk* chara, CharaTask* catcher, int presses, int time)
    : ControlBase(chara), m08(catcher), m0C(presses), m10(time) {
    chara->m560 = 0x64;
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x84, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 99.3% - r4/r5 swapped for the catcher in the position and angle loads.
// inferred: moves in front of the catcher and turns like it
int ControlChaseCaught::vf0C() {
    int done = 0;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0)) {
        done = 1;
    }
    atn::Vector3 target = m08->mPos + FrontDir_unk(m08->mRot) * 5.0f;
    CharaTask* chara = mChara;
    chara->mPos = Approach_unk(chara->mPos, target, 0.2f);
    float yaw = atn::WrapAngle_unk(m08->mRot.y - mChara->mRot.y);
    atn::Vector3 turn(0.0f, yaw, 0.0f);
    mChara->mRot = mChara->mRot + turn * 0.2f;
    return done;
}

boost::function<void()> ControlChaseCaught::vf10() {
    ControlChaseHeld* next = new ((MemoryTag)0, "", "", 0) ControlChaseHeld(mChara, m08, m0C, m10);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

ControlChaseHeld::ControlChaseHeld(Chara_unk* chara, CharaTask* catcher, int presses, int time)
    : ControlBase(chara), m08(catcher), m0C(presses), m10(time), m14(0) {
    chara->m58C = 0;
    mChara->m560 = 0x65;
    mChara->fn_8016DA4C();
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x85, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    m18 = 30;
    m1C = 90;
}

// NONMATCHING: 99.6% - r3/r4 swapped in the copy of the position.
// inferred: the player escapes by pressing button 0x22; damage every 12 (or 18) frames
int ControlChaseHeld::vf0C() {
    mChara->mPos = m08->mPos + FrontDir_unk(m08->mRot) * 5.0f;
    mChara->mRot = m08->mRot;
    if ((mChara->mType == 0 || mChara->mType == 3) && m0C > 0) {
        lbl_8087F490->m728 = 1;
        if (lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
            m14++;
        }
        if (m14 >= m0C) {
            return 1;
        }
    }
    if (--m10 <= 0) {
        return 1;
    }
    int period = fn_80370174(lbl_8087F430, 0xD4) ? 18 : 12;
    if (m10 % period == 0 && mChara->mType != 2) {
        CharaStatus* status = &mChara->mStatus;
        status->fn_8012DF7C(status->mParamB.m84 * lbl_8087F0A8->m520, 0, 0);
    }
    return 0;
}

inline ControlChaseRelease::ControlChaseRelease(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 0x66;
    mChara->mF1C = 0;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x86, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 99.7% - r29/r30 swapped for the functor allocated by boost::function.
boost::function<void()> ControlChaseHeld::vf10() {
    ControlChaseRelease* next = new ((MemoryTag)0, "", "", 0) ControlChaseRelease(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

int ControlChaseRelease::vf0C() {
    int done = 0;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0) - 8.0f) {
        done = 1;
    }
    mChara->fn_8013CB68(mChara->mRot, 0, 0.0f, 1.0f);
    return done;
}

boost::function<void()> ControlChaseRelease::vf10() {
    if (mChara->mType != 0) {
        ControlChaseFollow* next = new ((MemoryTag)0, "", "", 0) ControlChaseFollow(mChara);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    return ControlBase::vf10();
}

ControlChaseAssist::ControlChaseAssist(Chara_unk* chara) : ControlBase(chara), m08(false), m0C(0) {
    chara->m560 = 0x67;
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    mChara->mF1C = 0;
}

// NONMATCHING: 99.9% - r3/r4 swapped around the frame counter.
// inferred: walks to the side of the leader, then turns like it
int ControlChaseAssist::vf0C() {
    CharaTask* leader = lbl_8087F098->mF8;
    if (lbl_8087F098->m48 != 2 || leader == 0) {
        return 1;
    }
    if (!m08) {
        atn::Vector3 target = leader->mPos + Cross_unk(FrontDir_unk(leader->mRot), atn::Vector3(0.0f, 1.0f, 0.0f)) * 8.0f;
        atn::Vector3 d = target - mChara->mPos;
        float dist = PSVECSquareMag(&d);
        m0C++;
        Motion_unk* motion = &mChara->mMotion;
        if (dist <= 25.0f || m0C > 30) {
            m08 = true;
            motion->m34C = 1;
            motion->mTracks[0].m20 = 1.0f;
            motion->fn_80097C08(0, 0x8A, 1, 0, 1, 0.0f, 0.1f);
            motion->mTracks[0].m0C = 1.0f;
            d = leader->mPos - mChara->mPos;
            d.y = 0.0f;
            mChara->mRot = d.ToRotation_unk();
        } else {
            atn::Vector3 rot = d.ToRotation_unk();
            motion->m34C = 1;
            motion->mTracks[0].m20 = 1.0f;
            motion->fn_80097C08(0, mChara->m484[3], 1, 0, 1, 0.0f, 0.1f);
            motion->mTracks[0].m0C = 1.0f;
            mChara->vf34(&rot, 1, 1.0f, 1.5f);
        }
    }
    return 0;
}

// NONMATCHING: 99.7% - r29/r30 swapped for the functor allocated by boost::function.
boost::function<void()> ControlChaseAssist::vf10() {
    ControlChaseRelease* next = new ((MemoryTag)0, "", "", 0) ControlChaseRelease(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

ControlChaseFollow::ControlChaseFollow(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 0x68;
    mChara->mF1C = 0;
}

// NONMATCHING: 98.2% - registers of the distance branches.
// inferred: keeps behind the leader, faster when far
int ControlChaseFollow::vf0C() {
    if (lbl_8087F098->m48 == 0) {
        return 1;
    }
    CharaTask* leader = lbl_8087F8A0->mHead;
    atn::Vector3 pos(leader->mPos);
    atn::Vector3 target(leader->mPos);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', -2.3561945f + leader->mRot.y);
    atn::Vector3 off;
    off = atn::Vector3(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(mtx, &off, &off);
    atn::Vector3 d = target - mChara->mPos;
    float dist = PSVECSquareMag(&d);
    float stop = 121.0f;
    float walk = 196.0f;
    float run = 450.0f;
    if (dist <= stop) {
        mChara->m6B8 = -d.GetNormalize() * 0.2f;
        atn::Vector3 rot = d.ToRotation_unk();
        mChara->vf34(&rot, 0, 0.0f, 0.0f);
    } else if (dist <= walk) {
        mChara->m570 = leader->m570;
        atn::Vector3 rot = d.ToRotation_unk();
        mChara->vf34(&rot, 0, 0.0f, 0.0f);
    } else if (dist >= run) {
        float len = sqrt(dist);
        float speed = 1.0f + (len - 14.0f) / 50.0f;
        speed = speed < 2.0f ? speed : 2.0f;
        atn::Vector3 rot = d.ToRotation_unk();
        mChara->vf34(&rot, 0, 1.0f, speed);
    } else {
        float near = sqrt(run);
        float len = sqrt(dist);
        float rate = len / near;
        mChara->m570 = rate > leader->m570 ? rate : leader->m570;
        atn::Vector3 rot = d.ToRotation_unk();
        mChara->vf34(&rot, 0, 0.0f, 1.0f);
    }
    return 0;
}

ControlChaseCatch::ControlChaseCatch(Chara_unk* chara, CharaTask* target) : ControlBase(chara), m08(target) {
    chara->m560 = 0x69;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x87, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    mChara->mPos = chara->mPos + atn::Vector3(0.0f, -0.5f, 0.0f);
}

// NONMATCHING: 98.1% - stack slots of the rotation matrix and the front direction (as in
// chr/ControlBase.cpp vf18).
// inferred: carries m08 in front of the character
int ControlChaseCatch::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    int done = 0;
    atn::Vector3 front = FrontDir_unk(mChara->mRot);
    Mtx scale;
    PSMTXScale(scale, m08->m540.x, m08->m540.y, m08->m540.z);
    atn::Matrix3x4 m;
    atn::Matrix3x4* p = &m;
    SetRotation_unk(*p, mChara->mRot);
    PSMTXConcat(p->m, scale, scale);
    atn::Vector3 pos = mChara->mPos + front * 5.0f + atn::Vector3(0.0f, 0.5f, 0.0f);
    scale[0][3] = pos.x;
    scale[1][3] = pos.y;
    scale[2][3] = pos.z;
    if (m08->mF1C != 0) {
        __MtxCopy(((atn::Matrix3x4*)m08->mF1C)->m, scale);
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        done = 1;
    }
    return done;
}

inline ControlChaseHold::ControlChaseHold(Chara_unk* chara, CharaTask* target) : ControlBase(chara), m08(target) {
    chara->m560 = 0x69;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x88, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 94.8% - the inlined ControlChaseHold constructor loads m08 before mChara.
boost::function<void()> ControlChaseCatch::vf10() {
    ControlChaseHold* next = new ((MemoryTag)0, "", "", 0) ControlChaseHold(mChara, m08);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

// NONMATCHING: 95.2% - as ControlChaseCatch::vf0C.
// inferred: holds m08 while it is caught (0x64) or held (0x65)
int ControlChaseHold::vf0C() {
    int done = 0;
    if (m08->m560 != 0x65 && m08->m560 != 0x64) {
        done = 1;
    } else {
        atn::Vector3 front = FrontDir_unk(mChara->mRot);
        Mtx scale;
        PSMTXScale(scale, m08->m540.x, m08->m540.y, m08->m540.z);
        atn::Matrix3x4 m;
        atn::Matrix3x4* p = &m;
        SetRotation_unk(*p, mChara->mRot);
        PSMTXConcat(p->m, scale, scale);
        atn::Vector3 pos = mChara->mPos + front * 5.0f + atn::Vector3(0.0f, 0.5f, 0.0f);
        scale[0][3] = pos.x;
        scale[1][3] = pos.y;
        scale[2][3] = pos.z;
        if (m08->mF1C != 0) {
            __MtxCopy(((atn::Matrix3x4*)m08->mF1C)->m, scale);
        }
    }
    return done;
}

inline ControlChaseReleased::ControlChaseReleased(Chara_unk* chara, CharaTask* target)
    : ControlBase(chara), m08(target) {
    chara->m560 = 0x69;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x89, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 94.8% - as ControlChaseCatch::vf10.
boost::function<void()> ControlChaseHold::vf10() {
    ControlChaseReleased* next = new ((MemoryTag)0, "", "", 0) ControlChaseReleased(mChara, m08);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

int ControlChaseReleased::vf0C() {
    int done = 0;
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        done = 1;
    }
    return done;
}

boost::function<void()> ControlChaseReleased::vf10() {
    mChara->m560 = 0;
    return ControlBase::vf10();
}

} // namespace chr
