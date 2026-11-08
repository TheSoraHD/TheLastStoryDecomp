// inferred: ladder, alley way, crawl and deck climbing controls of the characters (vtables
// 0x80781438-0x807815A0, RTTI names in .rodata). Only the small functions are written; the
// constructors of ControlLadder/ControlAlleyWay/ControlCrawling and the update functions (vf0C, vf14)
// are still asm.
#include "chr/ControlLadder.h"
#include "game/GameVoice.h"
#include "game/CharaTask.h"
#include "macros.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"

// inferred: collision world and its ray test (as in chr/ControlWallUp.cpp)
struct LadderCollResult_unk {
    LadderCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m08_unk;
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 mNormal; // 0x28: inferred: normal of the hit wall
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, LadderCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);

extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" int fn_80051A88(const atn::ColliSphereShape_unk* a, const atn::ColliSphereShape_unk* b); // inferred: spheres touch
extern "C" int rand();

namespace chr {

// inferred: progress of the motion of track 0 (frame / length; name invented)
static inline float MotionRate_unk(Motion_unk* m) {
    float frame = m->mTracks[0].m08;
    return frame / m->fn_80097D7C(0);
}

// inferred: climbs the ladder from `start` to `end` (motion 0x56 up, 0x55 down)
ControlLadder::ControlLadder(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& end)
    : ControlBase(chara), mStart(start), mEnd(end), m4C(0.0f) {
    chara->SetControlId_unk(0x54);
    Motion_unk* m = &mChara->mMotion;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    m->m34C = 1;
    m->mTracks[0].m0C = m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, mStart.y < mEnd.y ? 0x56 : 0x55, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 dir = mEnd - mStart;
    dir.Length_unk();
    dir.Normalize_unk();
    mDir = dir;
    m2C = 0;
    dir.y = 0.0f;
    dir.Normalize_unk();
    mChara->mPos = mStart;
    mChara->mRot = dir.ToRotation_unk();
    mChara->fn_801446F0();
    mChara->fn_80176DFC(1, 0, 0);
    mUp = mDir.y > 0.0f;
}


// inferred: (b - a) * t + a (name invented)
static inline atn::Vector3 LerpTo_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: front of the character (z axis turned by its yaw; name invented)
static inline atn::Vector3 Front_unk(CharaTask* chara) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    PSMTXRotRad(m.m, 'y', chara->mRot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}

// inferred: plays the climbing voice (always for the player, half of the time for the others)
static inline void ClimbVoice_unk(ControlBase* control) {
    if (lbl_8087F498 != 0 && (control->mChara->mType == 0 || rand() % 2 == 0)) {
        lbl_8087F498->fn_803EA77C(control->mChara, 0xF, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 98.0% - stack layout: the original has a 4-byte hole above the first vector of state 0
// and a slot of its own for the result of the state 2 interpolation (here it shares the one of a
// spine position); tried a named result, a named difference, a function-level frame.
// inferred: 0 gets on (motion 0x55/0x56), 1 climbs with the stick (players) or straight on (others),
// 2 gets off (0x59 at the top, 0x5A at the bottom). Two climbers whose spines touch push the hips
// apart (m4C, used by vf40).
int ControlLadder::vf0C() {
    int ret = 0;
    Motion_unk* m = &mChara->mMotion;
    switch (m2C) {
    case 0: {
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0) - 1.0f) {
            if (m->mTracks[0].m00 == 0x55) {
                atn::Vector3 rot = mChara->mRot;
                rot.y = atn::WrapAngle_unk(3.1415927f + rot.y);
                mChara->mRot = rot;
            }
            m->fn_80097C08(0, 0x58, 1, 0, 1, 1.0f, 0.1f);
            mChara->fn_80176DFC(0, 0, 0);
            m2C = 1;
        } else {
            atn::Vector3 pos = mChara->mPos;
            if (m->mTracks[0].m00 == 0x56) {
                atn::Vector3 end = mEnd;
                if (end.y - pos.y < 1.0f) {
                    m2C = 2;
                    m->fn_80097C08(0, 0x59, 0, 0, 1, 0.0f, 0.1f);
                    m->mTracks[0].m0C = 1.0f;
                    m->mTracks[0].m08 = 24.0f;
                    m30 = pos;
                    m30.y -= 16.0f;
                    pos = end;
                    mChara->fn_80176DFC(0, 0, 0);
                    ClimbVoice_unk(this);
                }
            } else {
                atn::Vector3 end = mEnd;
                if (end.y - pos.y > -18.0f) {
                    m2C = 2;
                    m->fn_80097C08(0, 0x5A, 0, 0, 1, 1.0f, 0.1f);
                    m->mTracks[0].m0C = 1.0f;
                    m30 = pos;
                    m3C = end;
                    atn::Vector3 rot = mChara->mRot;
                    rot.y = atn::WrapAngle_unk(3.1415927f + rot.y);
                    mChara->mRot = rot;
                    mChara->fn_80176DFC(0, 0, 0);
                    ClimbVoice_unk(this);
                }
            }
            mChara->mPos = pos;
        }
        break;
    }
    case 1: {
        float speed = 0.0f;
        int up = mDir.y > 0.0f;
        if (mChara->mType == 0 && !mChara->mFlag0) {
            float stick = fn_800A56A8(lbl_8087EF70, 0, 1, 0);
            if (atn::Abs_unk(stick) > 0.4f) {
                speed = stick > 0.0f ? 0.3f : -0.3f;
                speed *= (atn::Abs_unk(stick) - 0.4f) / 0.6f;
                mUp = stick > 0.0f;
            }
        } else {
            speed = up ? 0.3f : -0.3f;
            mUp = up;
        }
        int move = 1;
        m->mTracks[0].m0C = speed / 0.3f;
        atn::Vector3 pos = mChara->mPos;
        atn::Vector3 rot = mChara->mRot;
        pos += mDir * (up ? speed : -speed);
        if (speed > 0.0f) {
            atn::Vector3 top = up ? mEnd : mStart;
            if (top.y - pos.y < 1.0f) {
                m2C = 2;
                m->fn_80097C08(0, 0x59, 0, 0, 1, 0.0f, 0.1f);
                m->mTracks[0].m0C = 1.0f;
                move = 0;
                m->mTracks[0].m08 = 24.0f;
                m30 = pos;
                m30.y -= 16.0f;
                pos = top;
                ClimbVoice_unk(this);
            }
        } else if (speed < 0.0f) {
            atn::Vector3 bottom = up ? mStart : mEnd;
            if (bottom.y - pos.y > -18.0f) {
                m2C = 2;
                m->fn_80097C08(0, 0x5A, 0, 0, 1, 0.0f, 0.1f);
                m->mTracks[0].m0C = 1.0f;
                move = 0;
                m30 = pos;
                m3C = bottom;
                ClimbVoice_unk(this);
            }
        }
        LadderCollResult_unk result;
        if (move) {
            atn::Vector3 from = pos - Front_unk(mChara) * 8.0f;
            atn::Vector3 to = pos + Front_unk(mChara) * 8.0f;
            from.y -= 5.0f;
            to.y -= 5.0f;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
                pos.x = result.m04.x;
                pos.z = result.m04.z;
                atn::Vector3 n(-result.mNormal.x, -result.mNormal.y, -result.mNormal.z);
                rot = n.ToRotationZ_unk();
            }
        }
        mChara->mPos = pos;
        mChara->mRot = rot;
        int hit = 0;
        atn::ColliSphereShape_unk self(
            m->GetNodeMtx_unk("spine") != 0 ? ((atn::Matrix3x4*)m->GetNodeMtx_unk("spine"))->GetTrans_unk() : pos,
            9.0f);
        atn::ColliSphereShape_unk other;
        other.mRadius = 9.0f;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (p == mChara) {
                continue;
            }
            if (p->CanAct_unk() && p->IsFlag_unk(2) && p->m55C == 6 && p->m560 == 0x54) {
                Motion_unk* pm = &p->mMotion;
                other.mCenter = pm->GetNodeMtx_unk("spine") != 0
                                    ? ((atn::Matrix3x4*)pm->GetNodeMtx_unk("spine"))->GetTrans_unk()
                                    : p->mPos;
                if (fn_80051A88(&self, &other)) {
                    hit = 1;
                    break;
                }
            }
        }
        if (hit) {
            m4C = MIN(0.5f + m4C, 3.0f);
        } else {
            m4C = MAX(m4C - 0.5f, 0.0f);
        }
        break;
    }
    case 2: {
        if (m->mTracks[0].m00 == 0x5A) {
            float t = MIN(1.0f, 1.1f * MotionRate_unk(m));
            mChara->mPos = LerpTo_unk(m30, m3C, t);
        }
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            mChara->fn_80176DFC(0, 0, 0);
            ret = 1;
        }
        break;
    }
    }
    return ret;
}

// NONMATCHING: 94.7% - stack slots of the 0x59 case (the original puts the difference vector above
// its product) and the scheduling of the prologue (the original loads mChara before saving r29-r31);
// tried a switch, named/unnamed temporaries, a nested block. The clamp of the 0x59 case is expanded
// once per component (the inliner substitutes the pure argument of operator*).
// inferred: position of the character for the camera/collision while it gets on or off the ladder
atn::Vector3 ControlLadder::vf14() const {
    Motion_unk* m = &mChara->mMotion;
    if (m->mTracks[0].m00 == 0x59) {
        float len = m->fn_80097D7C(0) - 24.0f;
        float frame = m->mTracks[0].m08 - 24.0f;
        return (mChara->mPos - m30) * MIN(1.0f, MAX(0.0f, frame / len)) + m30;
    }
    if (m->mTracks[0].m00 == 0x56) {
        atn::Vector3 target = mChara->mPos;
        target.y -= 16.0f;
        float t = MIN(1.0f, MAX(0.0f, MotionRate_unk(m)));
        return (target - mChara->mPos) * t + mChara->mPos;
    }
    if (m->mTracks[0].m00 == 0x55) {
        atn::Vector3 target = mChara->mPos;
        target.y -= 16.0f;
        float t = MIN(1.0f, MAX(0.0f, MotionRate_unk(m)));
        return (target - mChara->mPos) * t + mChara->mPos;
    }
    if (m->mTracks[0].m00 == 0x58) {
        atn::Vector3 r = mChara->mPos;
        r.y -= 16.0f;
        return r;
    }
    if (m->mTracks[0].m00 == 0x5A) {
        atn::Vector3 r = mChara->mPos;
        r.y -= 16.0f * MAX(0.0f, 1.0f - (1.0f + m->mTracks[0].m08) / m->fn_80097D7C(0));
        return r;
    }
    return mChara->mPos;
}

// NONMATCHING: 97.2% - offset of "hips" in the string pool: "spine" comes first in the original (it is
// used by vf0C/vf14, not written yet)
// inferred: the hips node follows the climb offset (opposite sign for the enemies)
void ControlLadder::vf40() {
    if (m2C == 1) {
        atn::Model* model = &mChara->mMotion;
        Motion_unk::Entry_unk* nodes = model->m21C.mData;
        Motion_unk::Entry_unk* e = &nodes[model->GetNodeNoByName("hips", 0)];
        e->mPos.x += mChara->mType == 0 ? m4C : -m4C;
    }
}

// inferred: walks sideways through an alley way from `start` to `target` (motion 0x4B)
ControlAlleyWay::ControlAlleyWay(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& target)
    : ControlBase(chara), mStart(start), mTarget(target) {
    chara->SetControlId_unk(0x28);
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x4B, 1, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    mChara->mPos = start;
    atn::Vector3 dir = target - start;
    float dist = dir.Length_unk();
    dir.Normalize_unk();
    mDir = dir;
    mFrames = dist / 0.3f + 3.0f;
    mChara->mRot = mDir.ToRotationZ_unk();
    mChara->fn_801446F0();
}

// inferred: the stick moves the character along the alley way (0.3 per frame, motions 0x4B/0x4C,
// 0x4D standing); it ends at either side. Enemies (mType != 0) and mFlag0 move straight to the end.
int ControlAlleyWay::vf0C() {
    int ret = 0;
    CharaTask* chara = mChara;
    Motion_unk* m = &chara->mMotion;
    if (chara->mType == 0 && !chara->mFlag0) {
        atn::Vector3 in = mChara->fn_80178018();
        float speed;
        float frame;
        if (m->mTracks[0].m00 == 0x4D || (frame = m->mTracks[0].m08, frame >= m->fn_80097D7C(0) - 1.0f)) {
            speed = 0.3f;
            if (atn::Abs_unk(atn::WrapAngle_unk(in.y - mChara->mRot.y)) > 1.5707964f) {
                speed = -speed;
            }
            if (mChara->fn_80178078() < 0.4f) {
                speed = 0.0f;
            }
            if (atn::Abs_unk(speed) < 1.1920929e-7f) {
                m->fn_80097C08(0, 0x4D, 1, 0, 1, 0.0f, 0.1f);
            } else if (speed > 0.0f) {
                m->fn_80097C08(0, 0x4B, 1, 0, 1, 0.0f, 0.1f);
            } else {
                m->fn_80097C08(0, 0x4C, 1, 0, 1, 0.0f, 0.1f);
            }
        } else if (m->mTracks[0].m00 == 0x4B) {
            speed = 0.3f;
        } else if (m->mTracks[0].m00 == 0x4C) {
            speed = -0.3f;
        } else {
            speed = 0.0f;
        }
        atn::Vector3 pos = mChara->mPos;
        pos += mDir * speed;
        atn::Vector3 d = mTarget - pos;
        if (d.LengthSq_unk() < 1.0f && m->mTracks[0].m00 == 0x4B) {
            ret = 1;
        }
        d = mStart - pos;
        if (d.LengthSq_unk() < 1.0f && m->mTracks[0].m00 == 0x4C) {
            ret = 1;
        }
        mChara->mPos = pos;
    } else {
        atn::Vector3 pos = chara->mPos;
        pos += mDir * 0.3f;
        atn::Vector3 d = mTarget - pos;
        if (d.LengthSq_unk() < 1.0f) {
            ret = 1;
        }
        mChara->mPos = pos;
    }
    return ret;
}

// NONMATCHING: draft (~82%) - the original computes the offsets (front * 15, * 10, * 4) in another order
// and keeps them in other registers
// inferred: climbs onto the deck in front (rotation `rot`): the position is lifted over the edge
// when the two ray tests hit
ControlClimbDeck::ControlClimbDeck(Chara_unk* chara, const atn::Vector3& rot) : ControlBase(chara), m08(0) {
    chara->m560 = 0x6C;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x59, 0, 0, 1, 1.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    m->mTracks[0].m08 = 30.0f;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 mtx;
    mtx.fn_8013A13C(rot.y);
    PSMTXMultVec(mtx.m, &front, &front);
    atn::Vector3 from = mChara->mPos;
    atn::Vector3 to = mChara->mPos;
    from.y += 40.0f;
    from += front * 15.0f;
    to += front * 15.0f;
    LadderCollResult_unk result;
    m18 = mChara->mPos + front * 10.0f;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
        from = mChara->mPos;
        from.y = result.m04.y - 2.0f;
        to = from;
        to += front * 15.0f;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
            m18 = result.m04;
            m18.y += 2.0f;
            m18 += front * 4.0f;
        }
    }
    mChara->mPos = m18;
    mChara->mRot = rot;
}

int ControlClimbDeck::vf0C() {
    int ret = 0;
    m08++;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0)) {
        ret = 1;
    }
    return ret;
}

// inferred: crawls from `start` to `target` (motions 0x207 enter, 0x208 crawl, 0x209 leave)
ControlCrawling::ControlCrawling(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& target)
    : ControlBase(chara), mStart(start), mTarget(target), mState(0) {
    chara->SetControlId_unk(0x78);
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x207, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.25f;
    mChara->mPos = start;
    atn::Vector3 dir = target - start;
    float dist = dir.Length_unk();
    dir.Normalize_unk();
    mDir = dir;
    mFrames = dist / 0.6f + 3.0f;
    mChara->mRot = mDir.ToRotationZ_unk();
    mChara->fn_801446F0();
}

// inferred: moves the character along the crawl at 0.6 per frame between the enter and leave motions
int ControlCrawling::vf0C() {
    Motion_unk* m = &mChara->mMotion;
    int ret = 0;
    if (mState == 0) {
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            m->fn_80097C08(0, 0x208, 1, 0, 1, 0.0f, 0.1f);
            mState = 1;
        }
    } else if (mState == 1) {
        atn::Vector3 pos = mChara->mPos;
        float step = 0.6f;
        float dist = (mTarget - pos).Length_unk();
        if (dist < step) {
            step = dist;
        }
        pos += mDir * step;
        atn::Vector3 rest = mTarget - pos;
        if (rest.LengthSq_unk() < 1.0f) {
            m->fn_80097C08(0, 0x209, 0, 0, 1, 0.0f, 0.1f);
            mState = 2;
        } else {
            mChara->mPos = pos;
        }
    } else if (mState == 2) {
        float frame = m->mTracks[0].m08;
        if (frame >= m->fn_80097D7C(0)) {
            ret = 1;
        }
    }
    return ret;
}

} // namespace chr
