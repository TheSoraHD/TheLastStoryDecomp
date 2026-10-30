#include "chr/ControlAttackBase.h"
#include "game/CharaTask.h"
#include "game/SetConfig.h"
#include "game/GameBgm.h"
#include "game/HomingBullet.h"
#include "atn/SoundRef.h"
#include "atn/LoadHandle.h"
#include "macros.h"

extern "C" int rand();
extern "C" double cos(double x);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);

// inferred: attack table of a motion (by the path of its resource)
extern "C" chr::AttackTable_unk* fn_802097C4(const char* path);

// inferred: effect manager (lbl_8087F048, see game/CharaTask.cpp)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" float fn_8010CA34(Effect_unk* effect);
extern "C" void fn_80106878(Effect_unk* effect, CharaTask* chara); // inferred: attack started

// inferred: multiplayer session (lbl_8087F610, see MP/CharaListeners_unk.cpp)
extern "C" void* lbl_8087F610;

// inferred: game state (lbl_8087EFA8): m3A4 is the time scale
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" GameState_unk* lbl_8087EFA8;

// inferred: voice/sound player (lbl_8087F498); fn_803EA77C plays voice `id` of a character
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);

extern "C" const char* fn_800EFF04(int id); // inferred: sound name of an effect id
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" HitInfo_unk* fn_80219E6C(int id);  // inferred: skill data
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: swing sounds of the attacks
static const char* const sSwingSe[] = {
    "SE_BTCOM_001", "SE_BTCOM_001_2", "SE_BTCOM_001_3", "SE_BTCOM_078",
    "SE_BTCOM_102", "SE_SP011_001",   "SE_SP011_002",   0,
};

// inferred: start frame and speed of the attack motion by attack level (CharaStatus mParamB.m94 / 100)
struct AttackLevel_unk {
    float mFrame;
    float mSpeed;
};
static const AttackLevel_unk sAttackLevel[6] = {
    {0.0f, 1.0f}, {0.0f, 1.1f}, {0.0f, 1.2f}, {0.0f, 1.3f}, {0.0f, 1.4f}, {0.0f, 1.5f},
};

// inferred: automatic attack setting of the player (name invented)
static inline bool IsAutoAttack_unk(CharaTask* chara) {
    bool on = false;
    if (chara->mType == 0 && SetConfig::sInstance->mAttackType != 0) {
        on = true;
    }
    if (chara->m50 == 0xAAE76 || chara->m50 == 0xAAE77) {
        on = false;
    }
    bool result = false;
    if (on && chara->m64C == 0) {
        result = true;
    }
    return result;
}

// inferred: manual attack setting (name invented)
static inline bool IsManualAttack_unk() {
    return SetConfig::sInstance->mAttackType == 0;
}

// inferred: automatic attack unless the character holds a second weapon (name invented)
static inline bool UseAuto_unk(bool on, CharaTask* chara) {
    bool result = false;
    if (on && chara->m64C == 0) {
        result = true;
    }
    return result;
}

// inferred: front direction for a rotation around y (name invented)
static inline atn::Vector3 GetFront_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(rot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}

// inferred: range test (name invented)
static inline bool InRange_unk(float v, float lo, float hi) {
    return lo <= v && v <= hi;
}

namespace chr {

inline void ControlAttackBase::LoadTable_unk() {
    Motion_unk* motion = &mChara->mMotion;
    mTable = fn_802097C4(motion->fn_80097D40(motion->mTracks[0].m00)->fn_80473F18());
}

int ControlAttackBase::vf24() {
    Chara_unk* chara = mChara;
    if (mTable != 0) {
        for (int i = 0; i < mTable->mCount; i++) {
            if (mTable->mStart[i] <= chara->mMotion.mTracks[0].m08 && chara->mMotion.mTracks[0].m08 <= mTable->mEnd[i]) {
                return 1;
            }
        }
    } else {
    int type = chara->m560;
    if (type == 4 || type == 5) {
        return chara->mMotion.mTracks[0].m08 >= 9.0f && chara->mMotion.mTracks[0].m08 <= 11.0f;
    }
    if (type == 8) {
        return chara->mMotion.mTracks[0].m08 >= 9.0f && chara->mMotion.mTracks[0].m08 <= 11.0f;
    }
    if (type == 7) {
        return chara->mMotion.mTracks[0].m08 >= 27.0f && chara->mMotion.mTracks[0].m08 <= 29.0f;
    }
    if (type == 11) {
        return chara->mMotion.mTracks[0].m08 >= 22.0f && chara->mMotion.mTracks[0].m08 <= 28.0f;
    }
    return chara->mMotion.mTracks[0].m08 >= 16.0f && chara->mMotion.mTracks[0].m08 <= 18.0f;
    }
    return 0;
}

float ControlAttackBase::vf20() {
    AttackTable_unk* table = mTable;
    Chara_unk* chara = mChara;
    if (table != 0) {
        return table->mEnd[table->mCount - 1];
    }
    int type = chara->m560;
    if (type == 4 || type == 5) {
        return 11.0f;
    }
    if (type == 8) {
        return 11.0f;
    }
    if (type == 7) {
        return 29.0f;
    }
    if (type == 11) {
        return 28.0f;
    }
    return 18.0f;
}

int ControlAttackBase::vf28() {
    Chara_unk* chara = mChara;
    if (mTable != 0) {
        float frame = chara->mMotion.mTracks[0].m08;
        for (int i = 0; i < mTable->mCount; i++) {
            if (mTable->mStart[i] - 10.0f <= frame && frame <= mTable->mStart[i]) {
                return 1;
            }
        }
    } else {
    int type = chara->m560;
    if (type == 4 || type == 5) {
        return InRange_unk(chara->mMotion.mTracks[0].m08, -1.0f, 9.0f);
    }
    if (type == 8) {
        return InRange_unk(chara->mMotion.mTracks[0].m08, -1.0f, 9.0f);
    }
    if (type == 7) {
        return InRange_unk(chara->mMotion.mTracks[0].m08, 17.0f, 27.0f);
    }
    if (type == 11) {
        return InRange_unk(chara->mMotion.mTracks[0].m08, 12.0f, 22.0f);
    }
    return InRange_unk(chara->mMotion.mTracks[0].m08, 6.0f, 16.0f);
    }
    return 0;
}

int ControlAttackBase::vf30() {
    Chara_unk* chara = mChara;
    if (mTable != 0) {
        float frame = chara->mMotion.mTracks[0].m08;
        for (int i = 0; i < mTable->mCount; i++) {
            if (mTable->mStart[i] <= frame && frame <= mTable->mEnd[i]) {
                return i;
            }
        }
    }
    return 0;
}

int ControlAttackBase::vf2C() {
    if (mTable != 0) {
        return mTable->mCount;
    }
    return 1;
}

int ControlAttackBase::vf34() {
    if (mTable != 0) {
        return mTable->m28[vf30()];
    }
    return 0;
}

int ControlAttackBase::vf38(int arg) {
    if (arg != 0) {
        if (mTable != 0) {
            return mTable->m48[vf30()];
        }
        return -1;
    }
    if (mTable != 0) {
        return mTable->m38[vf30()];
    }
    return -1;
}

float ControlAttackBase::vf60() {
    Chara_unk* chara = mChara;
    CharaStatus* status = &chara->mStatus;
    float rate = sAttackLevel[MIN(5, MAX(0, chara->mStatus.mParamB.m94 / 100))].mSpeed;
    if (status->IsState_unk(0x40)) {
        rate *= MAX(0.3f, 1.0f - chara->GetF94Rate_unk());
    }
    if (status->IsState_unk(0x8000)) {
        rate *= 0.5f;
    }
    rate *= status->fn_8012DB04();
    if (((unsigned int)mChara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= fn_8010CA34(lbl_8087F048);
    }
    if (SetConfig::sInstance->mAttackType != 0 && lbl_8087F610 != 0) {
        rate *= 0.85f;
    }
    return rate;
}

ControlAttack::ControlAttack(Chara_unk* chara, const atn::Vector3& dir) : ControlAttackBase(chara, dir) {
    chara->m560 = 4;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = vf60();
    Chara_unk* self = mChara;
    int id = self->m484[19];
    if (self->mFlag31) {
        id = self->m484[24];
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        atn::Matrix3x4 m;
        m.fn_8013A13C(self->mRot.y);
        PSMTXMultVec(m.m, &front, &front);
        mDir = front;
    } else {
        atn::Vector3 rot = self->mRot;
        atn::Vector3 front = GetFront_unk(self->mRot);
        float dot = PSVECDotProduct(&dir, &front);
        float front35 = cos(atn::DegToRad_unk(35.0f));
        float back70 = cos(atn::DegToRad_unk(70.0f));
        if (motion->fn_80097D40(0x158) != 0) {
            if (dot > front35) {
                int kind = 20;
                if (rand() % 2 == 0) {
                    kind = 19;
                }
                if (IsAutoAttack_unk(mChara)) {
                    id = 0x162;
                } else {
                    id = mChara->m484[kind];
                }
            } else if (dot < -back70) {
                atn::Vector3 r = dir.ToRotation_unk();
                float angle = atn::WrapAngle_unk(r.y - rot.y);
                if (angle > 0.0f) {
                    id = 0x15A;
                } else {
                    id = 0x15B;
                }
            } else {
                atn::Vector3 r = dir.ToRotation_unk();
                float angle = atn::WrapAngle_unk(r.y - rot.y);
                if (angle > 0.0f) {
                    id = 0x158;
                } else {
                    id = 0x159;
                }
            }
        }
    }
    motion->fn_80097C08(0, id, 0, 1, 1, 0.0f, 0.2f);
    Chara_unk* c = mChara;
    float frame = sAttackLevel[MIN(MAX(c->mStatus.mParamB.m94 / 100, 0), 5)].mFrame;
    float speed = c->mMotion.mTracks[0].m0C;
    motion->mTracks[0].m08 = frame * MIN(1.0f, speed);
    if (mChara->mType == 0) {
        switch (mChara->m50) {
        case 0xAAE76:
            if (motion->mTracks[0].m00 == 0x156 || motion->mTracks[0].m00 == 0x157) {
                motion->mTracks[0].m08 = 15.0f;
            }
            break;
        case 0xAAE77:
            if (motion->mTracks[0].m00 == 0x156 || motion->mTracks[0].m00 == 0x157) {
                motion->mTracks[0].m08 = 48.0f;
            }
            break;
        }
    }
    fn_80106878(lbl_8087F048, mChara);
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0, 0, 1.0f, 10.0f);
    }
}

int ControlAttack::vf0C() {
    Chara_unk* chara = mChara;
    if (chara->m674 < 0) {
        return 1;
    }
    Motion_unk* motion = &chara->mMotion;
    if (chara->mMotion.mTracks[0].m08 < 6.0f && 6.0f <= chara->mMotion.mTracks[0].m08 + chara->mMotion.mTracks[0].m0C) {
        fn_800C344C(sSwingSe[rand() % 3], &chara->mPos, 0, -1, 1.0f);
    }
    float speed = 0.0f;
    if (mChara->mType == 0 && SetConfig::sInstance->mAttackType != 0 &&
        (motion->mTracks[0].m00 == 0x162 || mChara->mMotion.mTracks[0].m00 == 0x163)) {
        if (motion->mTracks[0].m08 > 4.0f && motion->mTracks[0].m08 <= 16.0f) {
            speed = 1.0f;
        } else {
            float frame = motion->mTracks[0].m08;
            if (frame > motion->fn_80097D7C(0) - 3.0f) {
                mChara->m570 = 0.0f;
            }
        }
    }
    if (mChara->mType == 0 && mChara->m50 == 0xAAE76 &&
        (motion->mTracks[0].m00 == 0x157 || motion->mTracks[0].m00 == 0x156)) {
        if (motion->mTracks[0].m08 < 72.0f) {
            if (motion->mTracks[0].m08 > 50.0f) {
                speed = 1.0f;
            } else {
                speed = 0.65f;
            }
        }
    }
    if (vf24() != 0 && mHit == 0) {
        mHit = 1;
    }
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, speed, 1.0f);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

ControlAttackNext::ControlAttackNext(Chara_unk* chara, const atn::Vector3& dir) : ControlAttackBase(chara, dir) {
    chara->m560 = 5;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = vf60();
    Chara_unk* self = mChara;
    int id = self->m484[19];
    if (self->mFlag31) {
        id = self->m484[24];
    } else {
        atn::Vector3 rot = self->mRot;
        atn::Vector3 front = GetFront_unk(self->mRot);
        float dot = PSVECDotProduct(&dir, &front);
        float front35 = cos(atn::DegToRad_unk(35.0f));
        float back70 = cos(atn::DegToRad_unk(70.0f));
        Chara_unk* player = mChara;
        bool on = false;
        if (player->IsPlayer() && !IsManualAttack_unk()) {
            on = true;
        }
        if (player->m50 == 0xAAE76 || player->m50 == 0xAAE77) {
            on = false;
        }
        if (motion->fn_80097D40(0x158) != 0) {
            if (dot > front35) {
                Chara_unk* c = mChara;
                int first = c->m484[19];
                if (motion->mTracks[0].m00 == first || motion->mTracks[0].m00 == 0x162) {
                    if (UseAuto_unk(on, c)) {
                        id = 0x163;
                    } else {
                        id = c->m484[20];
                    }
                } else {
                    id = UseAuto_unk(on, c) ? 0x162 : first;
                }
            } else if (dot < -back70) {
                atn::Vector3 r = dir.ToRotation_unk();
                float angle = atn::WrapAngle_unk(r.y - rot.y);
                if (angle > 0.0f) {
                    id = 0x15A;
                } else {
                    id = 0x15B;
                }
            } else {
                atn::Vector3 r = dir.ToRotation_unk();
                float angle = atn::WrapAngle_unk(r.y - rot.y);
                if (angle > 0.0f) {
                    id = 0x158;
                } else {
                    id = 0x159;
                }
            }
        }
    }
    motion->fn_80097C08(0, id, 0, 1, 1, 0.0f, 0.2f);
    Chara_unk* c = mChara;
    float frame = sAttackLevel[MIN(MAX(c->mStatus.mParamB.m94 / 100, 0), 5)].mFrame;
    float speed = c->mMotion.mTracks[0].m0C;
    motion->mTracks[0].m08 = frame * MIN(1.0f, speed);
    if (mChara->mType == 0) {
        switch (mChara->m50) {
        case 0xAAE76:
            if (motion->mTracks[0].m00 == 0x156 || motion->mTracks[0].m00 == 0x157) {
                motion->mTracks[0].m08 = 15.0f;
            }
            break;
        case 0xAAE77:
            if (motion->mTracks[0].m00 == 0x156 || motion->mTracks[0].m00 == 0x157) {
                motion->mTracks[0].m08 = 48.0f;
            }
            break;
        }
    }
    fn_80106878(lbl_8087F048, mChara);
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0, 0, 1.0f, 10.0f);
    }
}

int ControlAttackNext::vf0C() {
    Chara_unk* chara = mChara;
    if (chara->m674 < 0) {
        return 1;
    }
    Motion_unk* motion = &chara->mMotion;
    if (chara->mMotion.mTracks[0].m08 < 6.0f && 6.0f <= chara->mMotion.mTracks[0].m08 + chara->mMotion.mTracks[0].m0C) {
        fn_800C344C(sSwingSe[rand() % 3], &chara->mPos, 0, -1, 1.0f);
    }
    float speed = 0.0f;
    if (mChara->mType == 0 && SetConfig::sInstance->mAttackType != 0 &&
        (motion->mTracks[0].m00 == 0x162 || mChara->mMotion.mTracks[0].m00 == 0x163)) {
        if (motion->mTracks[0].m08 > 4.0f && motion->mTracks[0].m08 <= 16.0f) {
            speed = 1.0f;
        } else {
            float frame = motion->mTracks[0].m08;
            if (frame > motion->fn_80097D7C(0) - 3.0f) {
                mChara->m570 = 0.0f;
            }
        }
    }
    if (mChara->mType == 0 && mChara->m50 == 0xAAE76 &&
        (motion->mTracks[0].m00 == 0x157 || motion->mTracks[0].m00 == 0x156)) {
        if (motion->mTracks[0].m08 < 72.0f) {
            if (motion->mTracks[0].m08 > 50.0f) {
                speed = 1.0f;
            } else {
                speed = 0.65f;
            }
        }
    }
    if (vf24() != 0 && mHit == 0) {
        mHit = 1;
    }
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, speed, 1.0f);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

ControlAttackSetup::ControlAttackSetup(Chara_unk* chara, int wait) : ControlBase(chara), mWait(wait) {
    chara->m560 = 0xC;
    Chara_unk* c = mChara;
    CharaStatus* status = &c->mStatus;
    Motion_unk* motion = &c->mMotion;
    float rate = 1.0f;
    if (status->IsState_unk(0x40)) {
        rate *= MAX(0.3f, 1.0f - c->GetF94Rate_unk());
    }
    if (status->IsState_unk(0x8000)) {
        rate *= 0.5f;
    }
    rate *= status->fn_8012DB04();
    if (((unsigned int)mChara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= fn_8010CA34(lbl_8087F048);
    }
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = rate;
    motion->fn_80097C08(0, 0x173, 0, 0, 1, 0.0f, 0.1f);
}

int ControlAttackSetup::vf0C() {
    mWait = mWait - lbl_8087EFA8->m3A4;
    Chara_unk* chara = mChara;
    Motion_unk* motion = &chara->mMotion;
    float frame = chara->mMotion.mTracks[0].m08;
    if (frame > motion->fn_80097D7C(0)) {
        motion->fn_80097C08(0, 0x174, 1, 0, 1, 0.0f, 0.1f);
    }
    return 0;
}

ControlDashAttack::ControlDashAttack(Chara_unk* chara, const atn::Vector3& pos)
    : ControlAttackBase(chara, GetFront_unk(chara->mRot)), mTarget(pos) {
    chara->m560 = 6;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, mChara->m484[21], 0, 0, 1, 0.0f, 0.1f);
    mStart = mChara->mPos;
    mDir = (mTarget - mStart).GetNormalize();
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

int ControlDashAttack::vf0C() {
    float speed = 0.0f;
    float rate = 1.0f;
    Chara_unk* chara = mChara;
    atn::Vector3 d = chara->mPos - mTarget;
    if (d.Length_unk() >= mChara->mStatus.mParamB.m28 - 5.0f && chara->mMotion.mTracks[0].m08 < 23.0f) {
        speed = 1.0f;
        rate *= 1.5f;
    }
    if (vf24() != 0 && mHit == 0) {
        mHit = 1;
    }
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, speed, rate);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

ControlJumpAttack::ControlJumpAttack(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& offset)
    : ControlAttackBase(chara, GetFront_unk(chara->mRot)), mTarget(pos), mOffset(offset) {
    chara->m560 = 7;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, mChara->m484[23], 0, 0, 1, 0.0f, 0.1f);
    mStart = mChara->mPos;
    mDir = (pos - mStart).GetNormalize();
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

int ControlJumpAttack::vf0C() {
    float speed = 0.0f;
    float rate = 1.0f;
    Chara_unk* chara = mChara;
    if (chara->mMotion.mTracks[0].m08 <= 9.0f) {
        atn::Vector3 pos = chara->mPos;
        pos.y += 1.0f;
        pos.x += mOffset.x / 9.0f;
        pos.z += mOffset.z / 9.0f;
        chara->mPos = pos;
        return 0;
    }
    if (chara->mMotion.mTracks[0].m08 < 25.0f) {
        speed = rate;
        rate *= 1.5f;
    }
    if (vf24() != 0 && mHit == 0) {
        mHit = 1;
    }
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, speed, rate);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

atn::Vector3 ControlJumpAttack::vf14() {
    Chara_unk* chara = mChara;
    atn::Vector3 r = chara->mPos;
    float frame = chara->mMotion.mTracks[0].m08;
    if (frame <= 20.0f) {
        r.y = mStart.y;
    } else if (frame <= 30.0f) {
        r.y = (30.0f - frame) / 10.0f * (mStart.y - chara->mPos.y) + chara->mPos.y;
    }
    return r;
}

ControlJumpAttack2::ControlJumpAttack2(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& offset)
    : ControlAttackBase(chara, GetFront_unk(chara->mRot)), mTarget(pos), mOffset(offset) {
    chara->m560 = 7;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, 0x15C, 0, 0, 1, 0.0f, 0.1f);
    mStart = mChara->mPos;
    mDir = (pos - mStart).GetNormalize();
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

int ControlJumpAttack2::vf0C() {
    float speed = 0.0f;
    float rate = 1.0f;
    Chara_unk* chara = mChara;
    if (chara->mMotion.mTracks[0].m08 <= 9.0f) {
        atn::Vector3 pos = chara->mPos;
        pos.y += 1.0f;
        pos.x += mOffset.x / 5.0f;
        pos.z += mOffset.z / 5.0f;
        chara->mPos = pos;
        return 0;
    }
    if (chara->mMotion.mTracks[0].m08 <= 15.0f) {
        atn::Vector3 pos = chara->mPos;
        pos.x += mOffset.x / 5.0f;
        pos.z += mOffset.z / 5.0f;
        chara->mPos = pos;
        return 0;
    }
    if (chara->mMotion.mTracks[0].m08 < 31.0f) {
        speed = rate;
        rate *= 1.7f;
    }
    if (vf24() != 0 && mHit == 0) {
        mHit = 1;
    }
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, speed, rate);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

atn::Vector3 ControlJumpAttack2::vf14() {
    Chara_unk* chara = mChara;
    atn::Vector3 r = chara->mPos;
    float frame = chara->mMotion.mTracks[0].m08;
    if (frame <= 20.0f) {
        r.y = mStart.y;
    } else if (frame <= 30.0f) {
        r.y = (30.0f - frame) / 10.0f * (mStart.y - chara->mPos.y) + chara->mPos.y;
    }
    return r;
}

ControlMultiAttack::ControlMultiAttack(Chara_unk* chara, const atn::Vector3& dir) : ControlAttackBase(chara, dir) {
    chara->m560 = 9;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x161, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = vf60();
    Chara_unk* c = mChara;
    float frame = sAttackLevel[MIN(5, MAX(0, c->mStatus.mParamB.m94 / 100))].mFrame;
    float speed = c->mMotion.mTracks[0].m0C;
    motion->mTracks[0].m08 = frame * MIN(1.0f, speed);
    fn_80106878(lbl_8087F048, mChara);
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 2, 0, 1.0f, 10.0f);
    }
}

int ControlMultiAttack::vf0C() {
    Chara_unk* chara = mChara;
    if (lbl_8087F610 == 0 && chara->mType == 0) {
        if (chara->mMotion.mTracks[0].m08 >= 11.0f && chara->mMotion.mTracks[0].m08 <= 16.0f) {
            lbl_8087EFA8->m3A4 = 0.5f;
        } else {
            lbl_8087EFA8->m3A4 = 1.0f;
        }
    }
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, 0.0f, 1.0f);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

void ControlMultiAttack::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
}

ControlSideWallAttack::ControlSideWallAttack(Chara_unk* chara, const atn::Vector3& dir)
    : ControlAttackBase(chara, dir) {
    chara->m560 = 9;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x164, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = vf60();
    Chara_unk* c = mChara;
    float frame = sAttackLevel[MIN(5, MAX(0, c->mStatus.mParamB.m94 / 100))].mFrame;
    float speed = c->mMotion.mTracks[0].m0C;
    motion->mTracks[0].m08 = frame * MIN(1.0f, speed);
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 2, 0, 1.0f, 10.0f);
    }
}

int ControlSideWallAttack::vf0C() {
    Chara_unk* chara = mChara;
    float speed = (4.0f <= chara->mMotion.mTracks[0].m08 && chara->mMotion.mTracks[0].m08 < 13.0f) ? 1.0f : 0.0f;
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, speed, 0.6f);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return 0;
}

ControlGuardCounter::ControlGuardCounter(Chara_unk* chara, const atn::Vector3& dir)
    : ControlAttackBase(chara, GetFront_unk(chara->mRot)), mCounter(0) {
    chara->m560 = 10;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1D2, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = vf60();
    mChara->fn_801446F0();
    if (PSVECSquareMag(&dir) > 0.0001f) {
        mDir = atn::Vector3(-dir.x, -dir.y, -dir.z).GetNormalize();
    } else {
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        atn::Matrix3x4 m;
        m.fn_8013A13C(mChara->mRot.y);
        PSMTXMultVec(m.m, &front, &front);
        mDir = front;
    }
    LoadTable_unk();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 2, 0, 1.0f, 10.0f);
    }
    fn_800C344C(sSwingSe[3], &mChara->mPos, 0, -1, 1.0f);
}

int ControlGuardCounter::vf0C() {
    Chara_unk* chara = mChara;
    int result = 0;
    float rate = (chara->mMotion.mTracks[0].m08 >= 15.0f && chara->mMotion.mTracks[0].m08 <= 30.0f) ? 0.2f : 0.0f;
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&mDir.ToRotation_unk(), 0, 1.0f, rate);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    if (chara->mMotion.mTracks[0].m08 >= 12.0f) {
        if (mCounter == 0) {
            result = 1;
        } else if (lbl_8087F610 == 0 && mChara->mType == 0) {
            if (chara->mMotion.mTracks[0].m08 >= 18.0f) {
                lbl_8087EFA8->m3A4 = 1.0f;
            } else {
                lbl_8087EFA8->m3A4 = 0.5f;
            }
        }
    }
    if (vf24() != 0 && mHit == 0) {
        mHit = 1;
    }
    return result;
}

void ControlGuardCounter::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
}

ControlHitBackBullet::ControlHitBackBullet(Chara_unk* chara, const atn::Vector3& dir, HomingBullet* bullet)
    : ControlAttackBase(chara, dir) {
    chara->m560 = 0x80;
    mChara->mMotion.m34C = 1;
    Motion_unk* motion = &mChara->mMotion;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1D2, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = vf60();
    motion->mTracks[0].m08 = 5.0f;
    mChara->fn_801446F0();
    atn::Vector3 rot = mDir.ToRotation_unk();
    mChara->mRot = atn::Vector3(0.0f, rot.y, 0.0f);
    LoadTable_unk();
    mBullets.push_back(bullet);
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 2, 0, 1.0f, 10.0f);
    }
    fn_800C344C(sSwingSe[4], &mChara->mPos, 0, -1, 1.0f);
}

// inferred: sound of the hit of a shot (name invented)
static inline const char* GetShotSe_unk(BulletShot_unk shot) {
    return fn_800EFF04(shot.m0C->mB0);
}

int ControlHitBackBullet::vf0C() {
    int result = 0;
    Motion_unk* motion = &mChara->mMotion;
    if (vf24() != 0) {
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        atn::Matrix3x4 m;
        m.fn_8013A13C(mChara->mRot.y);
        PSMTXMultVec(m.m, &front, &front);
        for (unsigned int i = 0; i < mBullets.mCount; i++) {
            HomingBullet* bullet = mBullets.mData[i];
            CharaTask* target = bullet->m88;
            atn::Vector3 center = (target->mColli.m3C + target->mColli.m48) * 0.5f;
            atn::Vector3 dir = center - bullet->m08;
            PSVECNormalize(&dir, &dir);
            mBullets.mData[i]->fn_80441E8C(&dir, mChara, target);
            BulletShot_unk shot = mBullets.mData[i]->GetShot_unk();
            if (shot.m0C != 0) {
                const char* se = GetShotSe_unk(shot);
                if (se != 0) {
                    fn_800C344C(se, &mBullets.mData[i]->m08, 0, -1, 1.0f);
                }
            }
        }
        mBullets.mCount = 0;
        if (mHit == 0) {
            mHit = 1;
        }
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        result = 1;
    }
    return result;
}

void ControlHitBackBullet::vf1C() {
    for (unsigned int i = 0; i < mBullets.mCount; i++) {
        mBullets.mData[i]->vf0C(1, 0);
    }
}

ControlSwapAttack::ControlSwapAttack(Chara_unk* chara, Chara_unk* target)
    : ControlAttackBase(chara, GetFront_unk(chara->mRot)), mTarget(target) {
    chara->m560 = 8;
    Motion_unk* motion = &mChara->mMotion;
    motion->fn_80097C08(0, 0x1D2, 0, 0, 1, 0.0f, 0.1f);
    LoadTable_unk();
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x62, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    mChara->fn_801446F0();
    mDir = mTarget->mPos - mChara->mPos;
    mDir.y = 0.0f;
    PSVECNormalize(&mDir, &mDir);
    mTime = 0.0f;
    mStartPos = mChara->mPos;
    mStartRot = mChara->mRot;
    mChara->m574 = mDir * 2.0f;
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: approximate (stack layout of the saved state)
int ControlSwapAttack::vf0C() {
    mTime = mTime + lbl_8087EFA8->m3A4;
    Motion_unk* motion = &mChara->mMotion;
    float speed = mTime < 20.0f ? 1.0f : 0.0f;
    atn::Vector3 rot = mDir.ToRotation_unk();
    if (mTime >= 23.0f && motion->mTracks[0].m00 != 0x1D2) {
        motion->fn_80097C08(0, 0x1D2, 0, 1, 1, 0.0f, 0.05f);
        motion->mTracks[0].m08 = 14.0f;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0) - 1.0f) {
        return 1;
    }
    Chara_unk* chara = mChara;
    atn::Vector3 pos = chara->mPos;
    atn::Vector3 crot = chara->mRot;
    atn::Vector3 vel = chara->m574;
    atn::Vector3 rot2 = crot;
    if (mTime <= 19.0f) {
        vel.y = 0.0f;
        pos.y += 0.47368422f;
    } else {
        crot = rot;
    }
    atn::Vector3 d(mTarget->mPos.x - mStartPos.x, 0.0f, mTarget->mPos.z - mStartPos.z);
    if (mTime < 15.0f) {
        atn::Vector3 step = d * 0.06666667f;
        mChara->mPos = pos + step;
    } else {
        mChara->mPos = pos;
        mChara->mRot = crot;
        mChara->m574 = vel;
        mChara->fn_8013CB68(rot, 1, 1.0f, speed * mChara->m568);
    }
    if (mTime > 15.0f) {
        crot = mStartRot;
        atn::Vector3 back(-mDir.x, -mDir.y, -mDir.z);
        atn::Vector3 r = back.ToRotation_unk();
        float angle = atn::WrapAngle_unk(r.y - mStartRot.y);
        crot.y += angle * MIN(1.0f, (mTime - 15.0f) / 15.0f);
        mChara->mRot = crot;
    } else {
        mStartRot = mChara->mRot;
    }
    return 0;
}

void ControlSwapAttack::vf1C() {}

ControlAttackLastOne::ControlAttackLastOne(Chara_unk* chara, const atn::Vector3& dir, Chara_unk* target, int type,
                                           HitInfo_unk* skill)
    : ControlAttackBase(chara, dir), mSlow(false), mSlowEnd(false), mHitCount(0), mType(type), mSkill(skill),
      mTarget(target) {
    chara->m560 = 0x8D;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    switch (mType) {
    case 9:
        motion->fn_80097C08(0, 0x16E, 0, 1, 1, 0.0f, 0.1f);
        break;
    case 10:
    case 12:
        motion->fn_80097C08(0, 0x16F, 0, 1, 1, 0.0f, 0.1f);
        break;
    case 11:
        motion->fn_80097C08(0, 0x170, 0, 1, 1, 0.0f, 0.1f);
        break;
    }
    fn_80106878(lbl_8087F048, mChara);
    LoadTable_unk();
    mBulletId = fn_800F8548(lbl_8087F048);
    mWindow = 0;
    HitInfo_unk* hit = fn_80219E6C(0xE3);
    Chara_unk* c = mChara;
    c->m63C = c->m638;
    c->m638 = hit;
}

int ControlAttackLastOne::vf0C() {
    float speed = 0.0f;
    float rate = 1.0f;
    int result = 0;
    Motion_unk* motion = &mChara->mMotion;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        result = 1;
    }
    if (frame >= 28.0f) {
        if (mChara->mType == 0 && lbl_8087F610 == 0) {
            lbl_8087EFA8->m3A4 = MIN(1.0f, 0.2f + lbl_8087EFA8->m3A4);
        }
        if (mSlow && !mSlowEnd) {
            mSlowEnd = true;
            if (mChara->mType == 0 && GameBgm::sInstance != 0) {
                GameBgm::sInstance->fn_8037D49C(GameBgm::sInstance->mE0, 30);
            }
        }
    } else if (frame >= 23.0f) {
        if (mChara->mType == 0 && lbl_8087F610 == 0) {
            lbl_8087EFA8->m3A4 = MAX(0.2f, lbl_8087EFA8->m3A4 - 0.2f);
        }
        if (!mSlow) {
            mSlow = true;
            if (mChara->mType == 0 && GameBgm::sInstance != 0) {
                GameBgm::sInstance->fn_8037D49C(0.4f, 10);
            }
        }
    } else if (frame >= 9.0f) {
        speed = 0.7f;
    }
    if (vf24() != 0) {
        if (mWindow < vf30()) {
            mBulletId = fn_800F8548(lbl_8087F048);
            mWindow = vf30();
        }
        if (mHit == 0) {
            mHit = 1;
            if (mHitCount == 0) {
                fn_800C31F4(sSwingSe[5], 0, -1, 1.0f);
            } else {
                fn_800C31F4(sSwingSe[6], 0, -1, 1.0f);
            }
            mHitCount++;
        }
    } else if (mHit != 0) {
        Chara_unk* c = mChara;
        c->m63C = c->m638;
        c->m638 = mSkill;
        mHit = 0;
        if (mHitCount == 1 && lbl_8087F498 != 0) {
            fn_803EA77C(lbl_8087F498, mChara, 2, 0, 1.0f, 10.0f);
        }
    }
    if (mType == 11 && motion->mTracks[0].m08 < 100.0f) {
        speed = 0.7f;
        rate = 0.15f;
    }
    atn::Vector3 dir = mDir;
    if (mTarget != 0) {
        dir = mTarget->mPos - mChara->mPos;
        PSVECNormalize(&dir, &dir);
    }
    // inferred: address of the temporary (as in the original)
    mChara->vf34(&dir.ToRotation_unk(), 0, speed, rate);
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    return result;
}

void ControlAttackLastOne::vf1C() {
    lbl_8087EFA8->m3A4 = 1.0f;
    if (mSlow && !mSlowEnd && GameBgm::sInstance != 0) {
        GameBgm::sInstance->fn_8037D49C(GameBgm::sInstance->mE0, 30);
    }
}

} // namespace chr
