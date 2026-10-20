#include "chr/ControlStagger.h"
#include "macros.h"

extern "C" int rand();
extern "C" void fn_80097CCC(Model_unk* model, int track, float weight);
// inferred: result of the ray test fn_8004ED34 (as in game/ChaserNpc.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: effect manager behind lbl_8087F498 (as in chr/ControlGmkCoffinOpening.cpp)
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F498;
extern "C" void fn_803EA77C(EffectMgr_unk* mgr, CharaTask* chara, int id, int arg, float a, float b);
// inferred: field (lbl_8087F430, as in game/CharaTask.cpp)
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id);                    // inferred: event flag
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg); // inferred: sets an event flag

namespace chr {

// inferred: pushed back along the facing direction (inlined three times; name invented)
static inline void Front_unk(atn::Vector3* out, float rotY) {
    Mtx r;
    PSMTXRotRad(r, 'y', rotY);
    atn::Vector3 dir;
    dir = atn::Vector3(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(r, &dir, &dir);
    *out = dir;
}
inline void ControlStagger::SetPushBack_unk() {
    Front_unk(&mDir, mChara->mRot.y);
    mSpeed = 0.19736843f;
}

ControlStagger::ControlStagger(Chara_unk* chara, int time, int kind)
    : ControlBase(chara), mDir(0.0f, 0.0f, 0.0f), mSpeed(0.0f) {
    chara->m58C = 0;
    mChara->m560 = 0x55;
    mTime = time;
    mChara->mFlag5 = 0;
    mChara->fn_8016DA4C();
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->HasC48_unk()) {
        mChara->fn_80154344();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    CollResult_unk result;
    int motion = 0x11;
    int loop = 1;
    atn::Vector3 from = mChara->mPos;
    from.y += 15.0f;
    atn::Vector3 to = from;
    Mtx m;
    PSMTXRotRad(m, 'y', mChara->mRot.y - 1.5707964f);
    atn::Vector3 ofs;
    ofs = atn::Vector3(0.0f, 0.0f, 7.5f);
    PSMTXMultVec(m, &ofs, &ofs);
    to += ofs;
    if (fn_80219558(mChara->m50) == 2) {
        kind = 1;
    }
    if (kind == 0) {
        float fast = mChara->m508;
        float slow = mChara->m500;
        float mid = mChara->m504;
        float speed = mChara->m570;
        if (speed > fast) {
            motion = 0x48;
            loop = 0;
        } else if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
            motion = 0x51;
            loop = 0;
            SetPushBack_unk();
        } else if (speed > mid) {
            motion = 0x46;
            loop = 0;
        } else if (speed > slow) {
            motion = 0x54;
            loop = 1;
        }
    }
    if (kind == 1) {
        motion = 0x11;
        loop = 1;
    } else if (kind == 2) {
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
            motion = 0x51;
            loop = 0;
            SetPushBack_unk();
        } else {
            motion = 0x46;
            loop = 0;
        }
    } else if (kind == 3) {
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
            motion = 0x51;
            loop = 0;
            SetPushBack_unk();
        } else {
            motion = 0x48;
            loop = 0;
        }
    }
    Motion_unk* mot = &mChara->mMotion;
    mot->m34C = 1;
    mot->mTracks[0].m20 = 1.0f;
    mot->fn_80097C08(0, motion, loop, 0, 1, 0.0f, 0.1f);
    mot->mTracks[0].m0C = 1.0f;
    if (lbl_8087F498 != 0) {
        if (kind == 2) {
            fn_803EA77C(lbl_8087F498, mChara, 0x11, 0, 1.0f, 10.0f);
        } else if (kind == 3) {
            fn_803EA77C(lbl_8087F498, mChara, 0x10, 0, 1.0f, 10.0f);
        }
    }
}

// NONMATCHING: 99.3% - mChara, the track flag and the motion id get r5/r4/r6 instead of r6/r5/r4
int ControlStagger::vf0C() {
    float frame;
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mChara->mRot;
    mTime--;
    Chara_unk* chara = mChara;
    unsigned char flag = chara->mMotion.mTracks[0].m18;
    int id = chara->mMotion.mTracks[0].m00;
    if (flag == 0) {
        if (id == 0x51) {
            atn::Vector3 pos = mChara->mPos;
            pos += mDir * mSpeed;
            mChara->mPos = pos;
        }
        Motion_unk* motion = &mChara->mMotion;
        frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            return 1;
        }
    } else if (mTime <= 0) {
        return 1;
    }
    mChara->vf34(&rot, 0, speed, rate);
    return 0;
}

// NONMATCHING: 99.7% - the r3/r4 swap of the common movement reset (see chr/ControlOneMotion.cpp); the
// facing direction through Front4_unk(&mDir, mChara) fixed the scheduling of the mChara load
// inferred: facing direction of `c` (name invented)
static inline void Front4_unk(atn::Vector3* out, Chara_unk* c) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', c->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    *out = dir;
}
ControlSlip::ControlSlip(Chara_unk* chara, int stop) : ControlBase(chara), mStop(stop) {
    chara->m560 = 0x63;
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    mChara->fn_8016DA4C();
    mChara->mFlag5 = 0;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x8E, 0, 1, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    Front4_unk(&mDir, mChara);
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x11, 0, 1.0f, 10.0f);
    }
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x12, 0x2D, 1.0f, 10.0f);
    }
    if (mChara->mType == 0 && lbl_8087F430 != 0) {
        fn_80370320(lbl_8087F430, 0xF9, MIN(fn_80370174(lbl_8087F430, 0xF9) + 1, 999), 0);
    }
    if (mStop) {
        Chara_unk* c = mChara;
        c->m580 = 0.0f;
        c->m584 = 0.0f;
        Chara_unk* chara2 = mChara;
        fn_80097CCC(&chara2->mMotion, 3, 0.0f);
        chara2->m12BC = -1;
        mChara->m570 = 0.0f;
        mChara->m574 = lbl_807C7030;
    }
}

// NONMATCHING: 99.6% - mChara is loaded into r3 instead of r5, and the rotation is passed through r31
int ControlSlip::vf0C() {
    float speed = 0.0f;
    float rate = speed;
    Motion_unk* motion = &mChara->mMotion;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    if (mStop == 0) {
        float f = motion->mTracks[0].m08;
        speed = 1.0f;
        if (f <= 20.0f) {
            rate = 0.8f;
        } else if (f <= 30.0f) {
            rate = 0.5f;
        } else if (f <= 40.0f) {
            rate = 0.3f;
        } else {
            rate = 0.1f;
        }
    }
    if (mStop == 0 || !mChara->IsPartyMember_unk()) {
        atn::Vector3 rot = mDir.ToRotation_unk();
        mChara->vf34(&rot, 0, speed, rate);
    }
    return 0;
}

} // namespace chr
