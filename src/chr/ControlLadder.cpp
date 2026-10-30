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

} // namespace chr
