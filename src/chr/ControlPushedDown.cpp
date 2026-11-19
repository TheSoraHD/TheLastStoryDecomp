#include "chr/ControlPushedDown.h"
#include "game/GameTask.h"
#include "game/GameVoice.h"
#include "chr/ControlChaseCaught.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/ChaserNpc.h"
#include "game/PlayerTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include <boost/bind.hpp>
#include <string.h>
#include "macros.h"

extern "C" int rand();
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager


extern "C" GameTask* lbl_8087F430;

// inferred: frame parameters (lbl_8087EFA8, as in gmk/ArenaFloor.cpp)
struct Params_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" Params_unk* lbl_8087EFA8;
float lbl_8087DA40 = 1.5f; // inferred: side step speed at the start (.sdata, global)
float lbl_8087DA44 = 0.5f; // inferred: side step speed at the end (.sdata, global)

// inferred: segment (as in game/CharaTask.cpp)
struct Segment_unk {
    Segment_unk() {}
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};
// inferred: segment against capsule test (nonzero on a hit)
extern "C" int fn_80053BD0(void* out, const Segment_unk* seg, const atn::Vector3* capsule);

// inferred: flag read through an accessor (the flag word is loaded again before it is cleared);
// name invented
inline unsigned int GetFlag5_unk(const CharaTask* chara) {
    return chara->mFlag5;
}

// inferred: negated vector (inline form of atn::Vector3::operator-); name invented
inline atn::Vector3 Neg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: squared length through PSVECSquareMag (name invented)
inline float SqLen_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: front direction of the character (name invented)
inline atn::Vector3 GetFrontDir_unk(CharaTask* chara) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', chara->mRot.y);
    PSMTXMultVec(mtx, &dir, &dir);
    return dir;
}

static const char* const sSe[] = {"SE_BTCOM_041"};

// inferred: plays the sound `id` at `pos` (name invented)
static atn::SoundRef PlaySe_unk(int id, const atn::Vector3* pos, float volume) {
    return fn_800C344C(sSe[id], pos, 0, -1, volume);
}

// inferred: the character may complain about the push (not an enemy, not one of these models);
// name invented
static int CanComplain_unk(CharaTask* chara) {
    if (fn_80219558(chara->m50) >= 0) {
        return 0;
    }
    if (chara->m50 == 100107 || chara->m50 == 100108) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "np201")) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "pc007")) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "np205")) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "np204")) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "np211")) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "np203")) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "np206")) {
        return 0;
    }
    if (strstr(chara->mMotion.fn_8008B130(), "np213")) {
        return 0;
    }
    return 1;
}

inline atn::Vector3* CharaTask::fn_8013C38C() {
    return &mPos;
}

namespace chr {

ControlPushedDown::ControlPushedDown(Chara_unk* chara, const atn::Vector3& dir, int a, int b, int c)
    : ControlBase(chara), m2C(b), m30(a), m34(c) {
    chara->m560 = 0x44;
    mChara->fn_80155DAC(0);
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    Chara_unk* c2 = mChara;
    c2->mMotion.fn_80097CCC(3, 0.0f);
    c2->m12BC = -1;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    atn::Vector3 front = GetFrontDir_unk(mChara);
    float dot = PSVECDotProduct(&front, &dir);
    if (dot > 0.70710677f) {
        m->fn_80097C08(0, 0x90, 0, 1, 1, 0.0f, 0.1f);
        m->mTracks[0].m08 = 5.0f;
        m->mTracks[0].m0C = 1.1f;
    } else if (dot < -0.70710677f) {
        m->fn_80097C08(0, 0x8F, 0, 1, 1, 0.0f, 0.1f);
        m->mTracks[0].m08 = 5.0f;
        m->mTracks[0].m0C = 1.1f;
    } else {
        float y = dir.ToRotation_unk().y;
        float diff = atn::WrapAngle_unk(y - mChara->mRot.y);
        if (diff < 0.0f) {
            m->fn_80097C08(0, 0x91, 0, 1, 1, 0.0f, 0.1f);
            m->mTracks[0].m08 = 8.0f;
            m->mTracks[0].m0C = 1.1f;
        } else {
            m->fn_80097C08(0, 0x92, 0, 1, 1, 0.0f, 0.1f);
            m->mTracks[0].m08 = 5.0f;
            m->mTracks[0].m0C = 1.1f;
        }
    }
    atn::Vector3 vel = dir * 1.6f;
    vel.y *= 0.3f;
    Chara_unk* c3 = mChara;
    c3->m6B8 = vel;
    m08 = mChara->mRot;
    m14 = mChara->mPos;
    m20 = mChara->mRot;
    m38 = 0;
    atn::SoundRef se = PlaySe_unk(0, &mChara->mPos, 0.5f);
    fn_800CB688(&se, 0, 1.0f + 0.2f * ((float)(rand() % 1001) / 1000.0f) - 0.1f);
    if (m34 != 0 && lbl_8087F498 != 0 && rand() % 3 > 0) {
        if (ChaserNpc::fn_8018059C() == 0 || ChaserNpc::fn_8018059C()->m14B0 == 0 ||
            ChaserNpc::fn_8018059C()->m14B0 == 5) {
            CharaTask* speaker = lbl_8087F498->fn_803EAC20();
            if (speaker == 0 || (speaker != mChara && (unsigned int)speaker->m7C != (unsigned int)mChara->m7C)) {
                int mode = lbl_8087F430 != 0 ? lbl_8087F430->fn_803761DC() : 0;
                if (mode == 1) {
                    lbl_8087F498->fn_803EAA7C(mChara, 1, 10.0f);
                } else if (mode == 2) {
                    lbl_8087F498->fn_803EAA7C(mChara, 3, 10.0f);
                } else if (mode >= 3) {
                    lbl_8087F498->fn_803EAA7C(mChara, 5, 10.0f);
                } else {
                    lbl_8087F498->fn_803EAA7C(mChara, 0, 10.0f);
                }
            }
        }
    }
    if (mChara->IsPartyMember_unk() && lbl_8087F430 != 0) {
        // inferred: counts the pushes (flag 0xAB, at most 999)
        int count = lbl_8087F430->fn_80370174(0xAB);
        lbl_8087F430->fn_80370320(0xAB, count + 1 < 999 ? count + 1 : 999, 0);
    }
}

// inferred: frame, zero and one declared in this order (saved FPRs f30, f31, f29 as in the original)
// inferred: absolute values through double, like fabs/fabsf of MSL_C (fabsf is (float)fabs((double)x));
// the double value and the rounded one get registers of their own (names invented)
inline double AbsD_unk(double x) {
    return __fabs(x);
}
inline float AbsF_unk(float x) {
    return (float)AbsD_unk(x);
}

int ControlPushedDown::vf0C() {
    float frame;
    float zero = 0.0f;
    float one = 1.0f;
    Motion_unk* motion = mChara->GetModel_unk();
    int ret = 0;
    atn::Vector3 rot = m08;
    switch (m38) {
    case 0: {
        frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0) || (m30 != 0 && motion->mTracks[0].m08 >= 30.0f)) {
            int mode = lbl_8087F430 != 0 ? lbl_8087F430->fn_803761DC() : 0;
            if (!CanComplain_unk(mChara)) {
                mode = 0;
            }
            if (lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
                if (SqLen_unk(lbl_8087F8A0->mHead->mPos - mChara->mPos) >= 10000.0f) {
                    mode = 0;
                }
            }
            if (m34 != 0 && lbl_8087F498 != 0) {
                if (lbl_8087F498->fn_803EAC20() == mChara) {
                    if (mode == 1) {
                        lbl_8087F498->fn_803EAA7C(mChara, 2, 10.0f);
                    } else if (mode == 2) {
                        lbl_8087F498->fn_803EAA7C(mChara, 4, 10.0f);
                    } else if (mode >= 3) {
                        lbl_8087F498->fn_803EAA7C(mChara, 6, 10.0f);
                    }
                }
            }
            if (mode > 1) {
                m38 = 1;
            } else if (m2C == 0) {
                ret = 1;
            } else {
                m38 = 3;
            }
        } else {
            mChara->vf34(rot, 0, zero, one);
            Chara_unk* c = mChara;
            c->m580 = 0.0f;
            c->m584 = 0.0f;
        }
        break;
    }
    case 1: {
        motion->fn_80097C08(0, 0x14, 1, 0, 1, zero, 0.1f);
        motion->m34C = 1;
        motion->mTracks[0].m0C = 1.5f;
        CharaTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (player != 0) {
            Chara_unk* c = mChara;
            atn::Vector3 d = player->mPos - c->mPos;
            if (!((float)__fabs(PSVECSquareMag(&d)) < 1.1920929e-7f)) {
                PSVECNormalize(&d, &d);
            }
            d.fn_80139F60();
            float a = atn::WrapAngle_unk(d.y - mChara->mRot.y);
            float lim = 0.17453292f;
            if (a > lim) {
                a = lim;
            } else if (a < -lim) {
                a = -lim;
            }
            atn::Vector3 r = mChara->mRot;
            r.y += a;
            mChara->mRot = r;
            if (AbsF_unk(a) < lim) {
                m08 = mChara->mRot;
                m38 = 2;
                motion->m34C = 1;
                motion->mTracks[0].m20 = 1.0f;
                motion->mTracks[0].m0C = 1.0f;
                motion->fn_80097C08(0, 0x95, 0, 1, 1, 0.0f, 0.1f);
            }
        } else {
            m38 = 3;
        }
        break;
    }
    case 2: {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            if (m2C == 0) {
                ret = 1;
            } else {
                m38 = 3;
            }
        }
        mChara->vf34(rot, 0, zero, one);
        Chara_unk* c = mChara;
        c->m580 = 0.0f;
        c->m584 = 0.0f;
        break;
    }
    case 3: {
        motion->fn_80097C08(0, 0x14, 1, 0, 1, zero, 0.1f);
        motion->m34C = 1;
        motion->mTracks[0].m0C = 1.2f;
        atn::Vector3 pos = mChara->mPos;
        atn::Vector3 d = m14 - pos;
        float len = PSVECMag(&d);
        if (len < 0.01f) {
            atn::Vector3 r = mChara->mRot;
            float a = atn::WrapAngle_unk(m20.y - r.y);
            if (a > 0.17453292f) {
                a = 0.17453292f;
            } else if (a < -0.17453292f) {
                a = -0.17453292f;
            }
            r.y += a;
            mChara->mRot = r;
            if (atn::Abs_unk(a) < 0.17453292f) {
                ret = 1;
            }
        } else {
            if (len > 0.3f) {
                PSVECNormalize(&d, &d);
                pos += d * 0.3f;
            } else {
                pos = m14;
            }
            *mChara->fn_8013C38C() = pos;
            atn::Vector3 r = mChara->mRot;
            float a = atn::WrapAngle_unk(d.ToRotation_unk().y - r.y);
            if (a > 0.17453292f) {
                a = 0.17453292f;
            } else if (a < -0.17453292f) {
                a = -0.17453292f;
            }
            r.y += a;
            mChara->mRot = r;
        }
        break;
    }
    }
    return ret;
}

int ControlPushedDown::fn_801C59D4() {
    return m2C == 0 && mChara->mMotion.mTracks[0].m08 > 20.0f;
}

ControlOttotto::ControlOttotto(Chara_unk* chara, const atn::Vector3* dir, int time, int motion)
    : ControlBase(chara), m08(*dir), m14(time) {
    chara->m560 = 0x61;
    mChara->fn_80155DAC(0);
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (GetFlag5_unk(mChara)) {
        mChara->mFlag5 = 0;
    }
    Chara_unk* c = mChara;
    c->mMotion.fn_80097CCC(3, 0.0f);
    c->m12BC = -1;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = 1.0f;
    if (motion != -1) {
        m->fn_80097C08(0, motion, 0, 1, 1, 0.0f, 0.1f);
    } else {
        atn::Vector3 front = GetFrontDir_unk(mChara);
        if (PSVECDotProduct(&front, &m08) > 0.0f) {
            m->fn_80097C08(0, 0x52, 0, 1, 1, 0.0f, 0.1f);
        } else {
            m->fn_80097C08(0, 0x53, 0, 1, 1, 0.0f, 0.1f);
            m08 = Neg_unk(m08);
        }
    }
    Chara_unk* c2 = mChara;
    c2->m570 = 0.0f;
    mChara->m574 = atn::Vector3(0.0f, 0.0f, 0.0f);
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        lbl_8087F498->fn_803EA77C(mChara, 0x13, 0, 1.0f, 10.0f);
    }
}

int ControlOttotto::vf0C() {
    int count = --m14;
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->IsPlayer()) {
        if (count <= 0) {
            atn::Vector3 target = mChara->fn_80178018();
            atn::Vector3 dir = motion->fn_80139F2C(0) == 0x52 ? Neg_unk(m08) : m08;
            float a = atn::WrapAngle_unk(target.y - dir.ToRotationZ_unk().y);
            if (mChara->fn_80178078() > 0.4f && a < 1.5707964f) {
                return 1;
            }
        }
    } else if (count <= 0) {
        return 1;
    }
    if (motion->mTracks[0].m00 == 0x52 && motion->mTracks[0].m08 < 10.0f) {
        mChara->mPos = (*mChara->fn_8013C38C()) - m08.GetNormalize() * 0.3f;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    mChara->vf34(m08.ToRotation_unk(), 0, 0.0f, 1.0f);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

ControlPushedAvoid::ControlPushedAvoid(Chara_unk* chara, const atn::Vector3& dir, Chara_unk* other)
    : ControlBase(chara), m30(0.0f), m34(other), m38(chara->m560 == 0x68) {
    chara->m560 = 0x45;
    mChara->fn_80155DAC(0);
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    Chara_unk* c = mChara;
    c->mMotion.fn_80097CCC(3, 0.0f);
    c->m12BC = -1;
    Motion_unk* m = &mChara->mMotion;
    m->fn_80097C08(0, mChara->m484[1], 0, 0, 1, 0.0f, 0.1f);
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->mTracks[0].m0C = -2.0f;
    m->mTracks[0].m08 = m->fn_80097D7C(0);
    atn::Vector3 side;
    atn::Vector3 front;
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    front.x = 0.0f;
    front.y = 0.0f;
    front.z = 1.0f;
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', other->mRot.y);
    PSMTXMultVec(mtx, &front, &front);
    PSVECCrossProduct(&front, &up, &side);
    if (PSVECDotProduct(&side, &dir) > 0.0f) {
        m08 = (dir + side).GetNormalize();
    } else {
        m08 = (dir - side).GetNormalize();
    }
    m14 = mChara->mPos;
    m20 = mChara->mRot;
    mChara->mColli.mFlags &= ~1;
    m2C = 0;
}

// `m30 / 8.0f` (folded to a product) gives the FPRs of the original; the segment is filled member by
// member (separate address of the end, as in the original).
int ControlPushedAvoid::vf0C() {
    int ret = 0;
    float time = lbl_8087EFA8->mTimeScale;
    m30 = m30 + time;
    Motion_unk* motion = &mChara->mMotion;
    switch (m2C) {
    case 0: {
        float speed = AbsF_unk(motion->mTracks[0].m0C);
        if (m30 >= 0.6f * (motion->fn_80097D7C(0) / speed)) {
            motion->fn_80097C08(0, mChara->m484[0], 1, 0, 1, 0.0f, 0.1f);
            motion->m34C = 1;
            motion->mTracks[0].m0C = 1.0f;
            m2C = 1;
        } else {
            Chara_unk* c = mChara;
            c->m580 = 0.0f;
            c->m584 = 0.0f;
            float t = MIN(1.0f, m30 / 8.0f);
            float s = t * (lbl_8087DA44 - lbl_8087DA40) + lbl_8087DA40;
            atn::Vector3 pos = *mChara->fn_8013C38C() + m08 * s;
            mChara->mPos = pos;
            atn::Vector3 r = mChara->mRot;
            float a = atn::WrapAngle_unk(Neg_unk(m08).ToRotation_unk().y - r.y);
            if (a > 0.17453292f) {
                a = 0.17453292f;
            } else if (a < -0.17453292f) {
                a = -0.17453292f;
            }
            r.y += a;
            mChara->mRot = r;
            break;
        }
    }
    case 1: {
        atn::Vector3 d = m14 - m34->mPos;
        if (PSVECMag(&d) > 5.0f) {
            Segment_unk seg;
            seg.mStart = mChara->mPos;
            seg.mEnd = m14;
            seg.mStart.y += 5.0f;
            seg.mEnd.y += 5.0f;
            if (!fn_80053BD0(0, &seg, &m34->mColli.m3C)) {
                motion->fn_80097C08(0, mChara->m484[1], 1, 0, 1, 0.0f, 0.1f);
                motion->m34C = 1;
                motion->mTracks[0].m0C = 2.0f;
                m2C = 2;
            }
        }
        break;
    }
    case 2: {
        atn::Vector3 pos = mChara->mPos;
        atn::Vector3 d = m14 - pos;
        float len = PSVECMag(&d);
        if (len < 0.01f) {
            ret = 1;
            mChara->mColli.mFlags |= 1;
        } else {
            if (len > 0.8f) {
                PSVECNormalize(&d, &d);
                pos += d * 0.8f;
            } else {
                pos = m14;
            }
            *mChara->fn_8013C38C() = pos;
            atn::Vector3 r = mChara->mRot;
            float a = atn::WrapAngle_unk(d.ToRotation_unk().y - r.y);
            if (a > 0.17453292f) {
                a = 0.17453292f;
            } else if (a < -0.17453292f) {
                a = -0.17453292f;
            }
            r.y += a;
            mChara->mRot = r;
        }
        break;
    }
    }
    return ret;
}

void ControlPushedAvoid::vf1C() {
    mChara->mColli.mFlags |= 1;
}

boost::function<void()> ControlPushedAvoid::vf10() const {
    if (m38) {
        ControlChaseFollow* next = new ((MemoryTag)0, "", "", 0) ControlChaseFollow(mChara);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    return ControlBase::vf10();
}

} // namespace chr
