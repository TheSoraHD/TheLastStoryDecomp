#include "chr/ControlWallUp.h"
#include "game/CharaTask.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>

extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" HitInfo_unk* fn_80219E6C(int id);           // inferred: bullet/skill data by id

// inferred: field object (lbl_8087F430): flags and current map data (class unknown)
struct WallMap_unk {
    unsigned char m00[0x4C];
    int m4C; // inferred: area
    int m50; // inferred: room
};
extern "C" void* lbl_8087F430;
extern "C" int fn_80370174(void* field, int id);
extern "C" WallMap_unk* fn_80373148(void* field);

// inferred: arena mode object (lbl_8087F610)
extern "C" void* lbl_8087F610;

// inferred: game state (lbl_8087EFA8): m3A4 is the time scale
struct WallGameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" WallGameState_unk* lbl_8087EFA8;

// inferred: pad state at +0x48C of the object behind lbl_8087F0A8 (class unknown)
struct WallInput_unk {
    unsigned char m000[0x48C];
    unsigned char mInput[1];
};
extern "C" WallInput_unk* lbl_8087F0A8;
extern "C" int fn_801231D0(void* pad, int button); // inferred: button pressed
extern "C" int fn_801240B4(void* pad, int button); // inferred: button held

// inferred: collision world and its ray test (see game/ChaserNpc.cpp)
struct WallCollResult_unk {
    WallCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
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
extern "C" int fn_8004ED34(void* colli, WallCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);

// inferred: color (four floats) of the effect manager behind lbl_8087F048; names invented
struct WallColor_unk {
    WallColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r;
    float g;
    float b;
    float a;
};
struct WallEffect_unk {
    unsigned char m00000[0x3E304];
    WallColor_unk m3E304; // inferred: fade color of the screen
};
extern "C" WallEffect_unk* lbl_8087F048;
extern "C" int fn_800F8548(WallEffect_unk* mgr);
extern "C" void fn_800FAB80(WallEffect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10, float arg11);
extern "C" void fn_800FB4B0(WallEffect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            int arg5, int arg6, int arg7, int arg8);

// inferred: voice/sound player (lbl_8087F498)
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);

// inferred: moves a destination out of the walls (chr/ControlCharge.cpp)
extern "C" void fn_801A4E08(CharaTask* chara, atn::Vector3* pos, atn::Vector3* dest);

// inferred: linear interpolation with the product first (name invented)
inline atn::Vector3 LerpWall_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: progress of the motion of track 0 (name invented)
inline float MotionRatio_unk(chr::Motion_unk* m) {
    float frame = m->mTracks[0].m08;
    return frame / m->fn_80097D7C(0);
}

namespace chr {

ControlWallUp::ControlWallUp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& dir)
    : ControlMoveCursor(chara), m40(15.0f), m44(300.0f), m4C(false), m4D(false), m4E(false), m50(0) {
    Chara_unk* chara0 = mChara;
    chara0->m560 = 0xF;
    m34 = dir;
    m34.y = 0.0f;
    Motion_unk* m = &mChara->mMotion;
    if (fn_805F9920(&m34) < 0.00001f) {
        Chara_unk* c = mChara;
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', c->mRot.y);
        PSMTXMultVec(mtx, &front, &front);
        m34 = front;
    } else {
        PSVECNormalize(&m34, &m34);
    }
    m28 = pos - m34 * 5.0f;
    Chara_unk* c2 = mChara;
    m08 = c2->mPos - m34 * 30.0f;
    if (fn_80370174(lbl_8087F430, 0x65) && m08.y > 55.0f) {
        m08.y = 55.0f;
    }
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x16B, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.3f;
    mChara->SetHit_unk(fn_80219E6C(0xCD));
    mChara->mF6C = m08;
    mChara->mF78 = mChara->m638->m58;
    m48 = m14.y;
}

// NONMATCHING: 99.8% - registers r3-r5 (character, temporary vectors) and f3/f4 of the m40 update.
int ControlWallUp::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    int ret = 0;
    mChara->mPos = LerpWall_unk(m28, m14, m40 / 15.0f);
    mChara->mRot = m34.ToRotation_unk();
    mChara->SetHit_unk(fn_80219E6C(0xCD));
    if (m40 > 0.0f) {
        m40 -= lbl_8087EFA8->m3A4 * motion->mTracks[0].m0C;
    } else {
        m44 -= lbl_8087EFA8->m3A4 * motion->mTracks[0].m0C;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 7.0f) {
        motion->mTracks[0].m0C = 0.1f;
    }
    if (m44 <= 0.0f) {
        ret = 1;
    }
    if (mChara->mType == 0) {
        float range = 90.0f;
        float angle = -2.5132742f;
        if (fn_80370174(lbl_8087F430, 0x68)) {
            range *= 3.0f;
            angle = -2.8274333f;
        }
        if (m40 <= 0.0f) {
            if (fn_80370174(lbl_8087F430, 0x65) && m14.y > 55.0f) {
                m14.y = 55.0f;
            }
            fn_801C3458(0, 5.0f, range, angle);
        }
        mChara->mRot = m34.ToRotation_unk();
        float frame2 = motion->mTracks[0].m08;
        if (frame2 >= motion->fn_80097D7C(0) - 5.0f) {
            if (fn_801240B4(lbl_8087F0A8->mInput, 0)) {
                m4C = true;
                ret = 1;
            }
            if (fn_801231D0(lbl_8087F0A8->mInput, 1)) {
                ret = 1;
            }
        }
        mChara->mF6C = m08;
        mChara->mF78 = mChara->m638->m58;
        atn::Vector3 from = mChara->mPos;
        from.y = 10.0f + m14.y;
        atn::Vector3 to = m08;
        to.y += 10.0f;
        WallCollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000020, 0, 0)) {
            lbl_8087F048->m3E304 = WallColor_unk(0.6f, 0.6f, 0.6f, 1.0f);
        } else {
            lbl_8087F048->m3E304 = WallColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        }
    } else {
        m08 = mChara->mC58.fn_8011BF3C(mChara->m648->m04);
        float frame2 = motion->mTracks[0].m08;
        if (frame2 >= motion->fn_80097D7C(0) - 4.0f) {
            m4C = true;
            ret = 1;
        }
        if (m4D && fn_801240B4(lbl_8087F0A8->mInput, 9)) {
            m4D = false;
        }
    }
    if (ret != 0 && m4C && m50 == 0) {
        if (lbl_8087F610 == 0 && fn_80373148(lbl_8087F430)->m4C == 40 && fn_80373148(lbl_8087F430)->m50 == 8) {
            m08.y = m48;
            m14.y = m48;
        }
        atn::Vector3 pos = m14;
        pos.y = m14.y;
        fn_801A4E08(mChara, &pos, &m08);
        if (lbl_8087F610 == 0 && fn_80373148(lbl_8087F430)->m4C == 40 && fn_80373148(lbl_8087F430)->m50 == 8) {
            WallCollResult_unk result;
            atn::Vector3 top = m08;
            atn::Vector3 bottom = m08;
            top.y += 3.0f;
            bottom.y -= 100.0f;
            if (fn_8004ED34(lbl_8087EE98, &result, &top, &bottom, mChara->fn_80179D44() | 0x80000000, 0, 0)) {
                m08.y = result.m04.y;
            }
        }
    }
    if (m4E) {
        m4C = false;
        ret = 1;
    }
    if (ret != 0 && !m4C && fn_80370174(lbl_8087F430, 0x65)) {
        m08.y = m48;
        m14.y = m48;
    }
    return ret;
}

// NONMATCHING: 99.8% - registers r5/r6 and stack slots of the inlined ControlWallDown constructor.
boost::function<void()> ControlWallUp::vf10() {
    if (m4C) {
        if (m50 != 0) {
            return m50->vfB4(mChara);
        }
        ControlWallSlash* next = new ((MemoryTag)0, "", "", 0) ControlWallSlash(mChara, m08, 0);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    ControlWallDown* next = new ((MemoryTag)0, "", "", 0) ControlWallDown(mChara, m14);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

void ControlWallUp::vf40() {
    float t = m40 / 15.0f;
    float s = 1.0f - t;
    Motion_unk::Entry_unk* e = &mChara->mMotion.m220[mChara->m524];
    e->m04 += atn::Vector3(0.0f, 0.0f, 5.0f * s);
}

void ControlWallUp::vf1C() {
    mChara->mPos = m14;
}

int ControlWallDown::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    int ret = 0;
    float t = (1.0f < MotionRatio_unk(motion)) ? 1.0f : MotionRatio_unk(motion);
    mChara->mPos = LerpWall_unk(m08, m14, t);
    float frame2 = motion->mTracks[0].m08;
    if (frame2 >= motion->fn_80097D7C(0)) {
        if (mChara->m564 == 2) {
            mChara->fn_8015495C(0, 1, 1, 1);
        }
        ret = 1;
    }
    return ret;
}

void ControlWallDown::vf1C() {
    mChara->mPos = m14;
}

ControlWallSlash::ControlWallSlash(Chara_unk* chara, const atn::Vector3& target, const atn::Vector3* land)
    : ControlBase(chara), m2C(26.0f), m30(0), m34(land != 0) {
    chara->m560 = 0xB;
    Motion_unk* m = &mChara->mMotion;
    m20 = mChara->mPos;
    m14 = target;
    if (land != 0) {
        m38 = *land;
    }
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, 0x16C, 0, 0, 1, 1.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    atn::Vector3 d(m14.x - m20.x, 0.0f, m14.z - m20.z);
    mChara->mRot = d.ToRotationZ_unk();
    m08 = m20;
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 99.7% - saved registers of the effect manager, the hit data and the bullet.
int ControlWallSlash::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mType == 0) {
        m2C -= lbl_8087EFA8->m3A4 * motion->mTracks[0].m0C;
        float t = 0.0f > m2C / 26.0f ? 0.0f : m2C / 26.0f;
        atn::Vector3 pos = LerpWall_unk(m14, m20, t);
        m08 = pos;
        float h = 0.0f > ((1.0f < (0.79999995f - 5.0f * (t - 0.4f) * (t - 0.4f)) / 0.64f)
                              ? 1.0f
                              : (0.79999995f - 5.0f * (t - 0.4f) * (t - 0.4f)) / 0.64f)
                      ? 0.0f
                      : ((1.0f < (0.79999995f - 5.0f * (t - 0.4f) * (t - 0.4f)) / 0.64f)
                             ? 1.0f
                             : (0.79999995f - 5.0f * (t - 0.4f) * (t - 0.4f)) / 0.64f);
        pos.y += 30.0f * h;
        m08.y += 15.0f * h;
        mChara->mPos = pos;
        if (m30 == 0 && m2C <= 0.0f) {
            m30 = 1;
            mChara->fn_8016DA4C();
            CharaTask* chara = mChara;
            WallEffect_unk* effect = lbl_8087F048;
            HitInfo_unk* hit = chara->m638;
            fn_800FAB80(effect, mChara, hit, fn_800F8548(effect), &chara->mPos, &chara->mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            if (mChara->mStatus.HasTimer_unk()) {
                HitInfo_unk* bullet = fn_80219E6C(0x1395);
                WallEffect_unk* effect2 = lbl_8087F048;
                fn_800FB4B0(effect2, mChara, bullet, fn_800F8548(effect2), &mChara->mPos, 0, 30, -1, 1);
                if (mChara->fn_801789D8(0x1395, 0)) {
                    mChara->fn_80178864(bullet, mChara);
                }
            }
            CharaPart_unk* part = mChara->m648;
            if (part != 0 && part->m28C != 0) {
                int life = part->m288 - 1;
                part->m288 = life > 0 ? life : 0;
            }
        }
        if (lbl_8087F610 == 0) {
            if (m2C > 7.0f) {
                lbl_8087EFA8->m3A4 = 0.5f;
            } else {
                lbl_8087EFA8->m3A4 = 1.0f;
            }
        }
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 60.0% - the copy of the returned vector stores z before the paired single (same
// issue as GimmickCommon::vf54).
atn::Vector3 ControlWallSlash::vf18() {
    return m14;
}

int ControlWallSlash::vf28() {
    return m2C < 10.0f;
}

// NONMATCHING: 60.0% - same copy order issue as vf18.
atn::Vector3 ControlWallSlash::vf14() {
    return m08;
}

void ControlWallSlash::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
    if (m34 != 0) {
        mChara->mPos = m38;
    } else {
        mChara->mPos = m14;
    }
}

boost::function<void()> ControlWallSlash::vf10() {
    if (m34 != 0) {
        return boost::bind(&CharaTask::fn_80155A88, (CharaTask*)mChara, m38, false);
    }
    return ControlBase::vf10();
}

} // namespace chr
