#include "game/GameConfig_unk.h"
#include "atn/RenderSetting_unk.h"
#include "chr/ControlWallUp.h"
#include "game/GameTask.h"
#include "game/GameVoice.h"
#include "chr/ControlCharge.h"
#include "game/CharaTask.h"
#include "game/BattleTask.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>
#include "game/Input_unk.h"
#include "game/MapTask.h"
#include "atn/ColliBase.h"

extern "C" int rand();
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" HitInfo_unk* fn_80219E6C(int id);           // inferred: bullet/skill data by id

class GameTask;
extern "C" GameTask* lbl_8087F430;

// inferred: arena mode object (lbl_8087F610)
extern "C" void* lbl_8087F610;

extern "C" RenderSetting_unk* lbl_8087EFA8;

extern "C" GameConfig_unk* lbl_8087F0A8;

extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, atn::ColliResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);

extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager


// inferred: moves a destination out of the walls (chr/ControlCharge.cpp)

// inferred: linear interpolation with the product first (name invented)
inline atn::Vector3 LerpWall_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: progress of the motion of track 0 (name invented)
inline float MotionRatio_unk(chr::Motion_unk* m) {
    float frame = m->mTracks[0].m08;
    return frame / m->fn_80097D7C(0);
}

// inferred: accessor of the control data (weak copy 0x800F530C, as in game/TacView.cpp)
extern "C" inline CharaC58_unk* fn_800F530C(CharaTask* chara) {
    return &chara->mC58;
}
namespace chr {

ControlWallUp::ControlWallUp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& dir)
    : ControlMoveCursor(chara), m40(15.0f), m44(300.0f), m4C(false), m4D(false), m4E(false), m50(0) {
    Chara_unk* chara0 = mChara;
    chara0->m560 = 0xF;
    m34 = dir;
    m34.y = 0.0f;
    Motion_unk* m = &mChara->mMotion;
    if (PSVECSquareMag(&m34) < 0.00001f) {
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
    if (((GameTask*)lbl_8087F430)->fn_80370174(0x65) && m08.y > 55.0f) {
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

int ControlWallUp::vf0C() {
    Motion_unk* motion = mChara->GetModel_unk();
    int ret = 0;
    mChara->mPos = LerpWall_unk(m28, m14, m40 / 15.0f);
    mChara->mRot = m34.ToRotation_unk();
    mChara->SetHit_unk(fn_80219E6C(0xCD));
    if (m40 > 0.0f) {
        float dt = lbl_8087EFA8->m3A4;
        m40 -= dt * motion->mTracks[0].m0C;
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
        if (((GameTask*)lbl_8087F430)->fn_80370174(0x68)) {
            range *= 3.0f;
            angle = -2.8274333f;
        }
        if (m40 <= 0.0f) {
            if (((GameTask*)lbl_8087F430)->fn_80370174(0x65) && m14.y > 55.0f) {
                m14.y = 55.0f;
            }
            fn_801C3458(0, 5.0f, range, angle);
        }
        mChara->mRot = m34.ToRotation_unk();
        float frame2 = motion->mTracks[0].m08;
        if (frame2 >= motion->fn_80097D7C(0) - 5.0f) {
            if ((&lbl_8087F0A8->mInput)->fn_801240B4(0)) {
                m4C = true;
                ret = 1;
            }
            if ((&lbl_8087F0A8->mInput)->fn_801231D0(1)) {
                ret = 1;
            }
        }
        mChara->mF6C = m08;
        mChara->mF78 = mChara->m638->m58;
        atn::Vector3 from = mChara->mPos;
        from.y = 10.0f + m14.y;
        atn::Vector3 to = m08;
        to.y += 10.0f;
        atn::ColliResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000020, 0, 0)) {
            lbl_8087F048->m3E304 = EffectColor_unk(0.6f, 0.6f, 0.6f, 1.0f);
        } else {
            lbl_8087F048->m3E304 = EffectColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        }
    } else {
        m08 = fn_800F530C(mChara)->fn_8011BF3C(mChara->m648->m04);
        float frame2 = motion->mTracks[0].m08;
        if (frame2 >= motion->fn_80097D7C(0) - 4.0f) {
            m4C = true;
            ret = 1;
        }
        if (m4D && (&lbl_8087F0A8->mInput)->fn_801240B4(9)) {
            m4D = false;
        }
    }
    if (ret != 0 && m4C && m50 == 0) {
        if (lbl_8087F610 == 0 && ((GameTask*)lbl_8087F430)->GetCurrentMap()->mId == 40 && ((GameTask*)lbl_8087F430)->GetCurrentMap()->mSubId == 8) {
            m08.y = m48;
            m14.y = m48;
        }
        atn::Vector3 pos = m14;
        pos.y = m14.y;
        ControlChargeAttackBase::fn_801A4E08(mChara, pos, m08);
        if (lbl_8087F610 == 0 && ((GameTask*)lbl_8087F430)->GetCurrentMap()->mId == 40 && ((GameTask*)lbl_8087F430)->GetCurrentMap()->mSubId == 8) {
            atn::ColliResult_unk result;
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
    if (ret != 0 && !m4C && ((GameTask*)lbl_8087F430)->fn_80370174(0x65)) {
        m08.y = m48;
        m14.y = m48;
    }
    return ret;
}

boost::function<void()> ControlWallUp::vf10() const {
    if (m4C) {
        if (m50 != 0) {
            return m50->vfB4(mChara);
        }
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, new ((MemoryTag)0, "", "", 0) ControlWallSlash(mChara, m08, 0));
    }
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, new ((MemoryTag)0, "", "", 0) ControlWallDown(mChara, m14));
}

void ControlWallUp::vf40() {
    float t = m40 / 15.0f;
    float s = 1.0f - t;
    Motion_unk::Entry_unk* e = &mChara->mMotion.m21C.mData[mChara->m524];
    e->mPos += atn::Vector3(0.0f, 0.0f, 5.0f * s);
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
        lbl_8087F498->fn_803EA77C(mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

// inferred: attack and extra bullet at the landing (same code in ControlChargeAttack::vf0C)
inline void FireAttack_unk(ControlBase* self) {
    HitInfo_unk* bullet;
    self->mChara->fn_8016DA4C();
    CharaTask* chara = self->mChara;
    BattleTask* effect = lbl_8087F048;
    HitInfo_unk* hit = chara->m638;
    effect->fn_800FAB80(self->mChara, hit, effect->fn_800F8548(), &chara->mPos, &chara->mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    if (self->mChara->mStatus.HasTimer_unk()) {
        bullet = fn_80219E6C(0x1395);
        BattleTask* effect2 = lbl_8087F048;
        effect2->fn_800FB4B0(self->mChara, bullet, effect2->fn_800F8548(), &self->mChara->mPos, 0, 30, -1, 1);
        if (self->mChara->fn_801789D8(0x1395, 0)) {
            self->mChara->fn_80178864(bullet, self->mChara);
        }
    }
}

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
            FireAttack_unk(this);
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

atn::Vector3 ControlWallSlash::vf18() const {
    return m14;
}

int ControlWallSlash::vf28() {
    return m2C < 10.0f;
}

atn::Vector3 ControlWallSlash::vf14() const {
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

boost::function<void()> ControlWallSlash::vf10() const {
    if (m34 != 0) {
        return boost::bind(&CharaTask::fn_80155A88, (CharaTask*)mChara, m38, false);
    }
    return ControlBase::vf10();
}

} // namespace chr
