// inferred: ladder, alley way, crawl and deck climbing controls of the characters (vtables
// 0x80781438-0x807815A0, RTTI names in .rodata). Only the small functions are written; the
// constructors of ControlLadder/ControlAlleyWay/ControlCrawling and the update functions (vf0C, vf14)
// are still asm.
#include "chr/ControlLadder.h"
#include "game/CharaTask.h"

// inferred: collision world and its ray test (as in chr/ControlWallUp.cpp)
struct LadderCollResult_unk {
    LadderCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m08_unk;
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, LadderCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);

namespace chr {

// NONMATCHING: 97.2% - offset of "hips" in the string pool: "spine" comes first in the original (it is
// used by vf0C/vf14, not written yet)
// inferred: the hips node follows the climb offset (opposite sign for the enemies)
void ControlLadder::vf40() {
    if (m2C == 1) {
        atn::Model* model = &mChara->mMotion;
        Motion_unk::Entry_unk* nodes = model->m220;
        Motion_unk::Entry_unk* e = &nodes[model->GetNodeNoByName("hips", 0)];
        e->m04.x += mChara->mType == 0 ? m4C : -m4C;
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
