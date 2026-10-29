// Charge of an attack (ControlCharge) and the attacks that follow it (ControlChargeAttackBase and
// its subclasses ControlChargeAttack, ControlHighJumpAttack, ControlQuarkSeirenAttack;
// ControlZanTackle).
#include "chr/ControlCharge.h"
#include "chr/ControlDive.h"
#include "game/HitInfo_unk.h"
#include "gmk/Lemon.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/SetConfig.h"
#include "macros.h"
#include <boost/bind.hpp>

// inferred: game settings (as in chr/ControlCdnStone.cpp)
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero while `button` is pressed
};
struct Settings_unk {
    unsigned char m000[0x314];
    int m314; // inferred: nonzero when the target is chosen with the cursor
    unsigned char m318[0x48C - 0x318];
    Input_unk mInput; // 0x48C
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: arena mode object (lbl_8087F610, as in game/CharaTask.cpp)
struct Arena_unk {
    unsigned char m000[0x540];
    int m540;
};
extern "C" Arena_unk* lbl_8087F610;
inline bool IsArenaMode_unk(int mode) {
    bool result = true;
    if (mode != 0 && mode != 1) {
        result = false;
    }
    return result;
}

// inferred: effect manager (lbl_8087F048, as in game/CharaTask.cpp)
struct Effect_unk {
    void fn_801059F8(CharaTask* chara); // inferred: called when a magic ends
};
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_80105950(Effect_unk* mgr, CharaTask* chara, atn::Model* model); // inferred: charge effect
extern "C" void fn_80105A08(Effect_unk* mgr, CharaTask* chara, atn::Model* model); // inferred: charge complete
extern "C" float fn_8010CA24(Effect_unk* mgr); // inferred: charge speed rate
extern "C" void fn_8010653C(Effect_unk* mgr, CharaTask* chara, atn::Model* model); // inferred: jump effect
extern "C" void fn_801065E4(Effect_unk* mgr, CharaTask* chara);                    // inferred: stops it
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A55D4(void* pad, int port, int button); // inferred: button held
extern "C" int fn_801240B4(Input_unk* input, int button);    // inferred: button held
// inferred: cursor shown by the tactical view (lbl_8087F9F8 + 0x15D8); names invented
struct TacCursor_unk {
    TacCursor_unk() : m04(0.0f, 0.0f, 0.0f), m10(-1.0f), m15(false), m18(2), m1C(0), m20(0) {}
    int m00;
    atn::Vector3 m04;
    float m10;
    bool m14;
    bool m15;
    int m18;
    int m1C;
    int m20;
};
// inferred: tactical view (lbl_8087F9F8, as in game/CharaTask.cpp)
struct TacView_unk {
    unsigned char m00[0x15D8];
    TacCursor_unk m15D8;
};
extern "C" TacView_unk* lbl_8087F9F8;
// inferred: field state (lbl_8087F430), only the members read here
struct FieldMode_unk {
    unsigned char m00[0x4C];
    int m4C;
    int m50;
};
struct Field_unk {
    unsigned char m0000[0x10D8];
    void* m10D8; // inferred: collision of the walls
};
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_80373FC4(Field_unk* field); // inferred: starts the cursor of the field
extern "C" int fn_8036DAA8(Field_unk* field);
extern "C" FieldMode_unk* fn_80373148(Field_unk* field);
extern "C" int fn_803CC1A4(void* walls, const atn::Vector3* pos, float radius, float cos, const atn::Vector3* dir);
extern "C" int fn_8004D388(void* colli, CollResult_unk* result, const atn::Vector3* pos, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg7); // inferred: sweep test
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" double sqrt(double x);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned long strlen(const char* s);
extern "C" int rand();
extern "C" void* lbl_8087F498; // inferred: voice manager
// inferred: one line of the voice table (defined in atn/ptr_string_base_char_0.cpp)
struct VoiceEntry_unk {
    const char* fn_8021FD7C(int kind) const;
};
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis
extern "C" void fn_803EA77C(void* voice, CharaTask* chara, int id, int arg, float volume, float arg2);
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10, float arg11);
extern "C" void fn_800FB4B0(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            int arg5, int arg6, int arg7, int arg8);
// inferred: result of fn_800185B4 (as in game/CharaTask.cpp)
struct MagicTarget_unk {
    unsigned char m00[0x08];
    int m08;
    CharaTask* m0C;
};
extern "C" void* lbl_8087EE68;
extern "C" void fn_80018608(void* manager, CharaTask* chara, HitInfo_unk* hit);
extern "C" MagicTarget_unk* fn_800185B4(void* manager, CharaTask* chara, HitInfo_unk* hit);
// inferred: game state (lbl_8087EFA8, as in game/CharaTask.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8;

extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3& from, const atn::Vector3& to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// inferred: facing direction of the rotation `rot` (name invented, as in chr/ControlCdnStone.cpp)
inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', rot.y);
    PSMTXMultVec(mtx, &v, &v);
    return v;
}

// inferred: name invented (as in game/CharaTask.cpp)
inline float LengthXZ_unk(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}

// inferred: `to` + (`from` - `to`) * t (name invented)
inline atn::Vector3 Blend_unk(const atn::Vector3& from, const atn::Vector3& to, float t) {
    return (from - to) * t + to;
}

// inferred: name invented (as in gmk/MeetingSpot.cpp)
inline float LengthXZSq_unk(const atn::Vector3& v) {
    return v.x * v.x + v.z * v.z;
}

// inferred: the target is chosen with the cursor (name invented)
inline bool IsCursorMode_unk() {
    bool result = false;
    if (lbl_8087F610 == 0 && lbl_8087F0A8->m314 != 0) {
        result = true;
    }
    return result;
}

// inferred: horizontal vector from `from` to `to` (name invented)
inline atn::Vector3 FlatDir_unk(const atn::Vector3& to, const atn::Vector3& from) {
    return atn::Vector3(to.x - from.x, 0.0f, to.z - from.z);
}

// inferred: the player character (id 1) chooses the target with the cursor (name invented)
inline bool IsCursorPlayer_unk(const chr::ControlBase* control) {
    bool result = false;
    if (IsCursorMode_unk() && control->mChara->m50 == 1) {
        result = true;
    }
    return result;
}

namespace chr {

// NONMATCHING: 99.8% - registers of the first stores (r3/r4/r5), of the two flags of the cursor
// test (r4/r5) and of the range sum (f4/f5).
ControlCharge::ControlCharge(Chara_unk* chara, int a, const atn::Vector3* target, int c)
    : ControlMoveCursor(chara), m2C(0.0f), m30(0), m34(0), m3C(c) {
    mChara->m560 = 0xD;
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (lbl_8087F048 != 0 && mChara->m648 != 0) {
        fn_80105950(lbl_8087F048, mChara, &mChara->mMotion);
    }
    m40 = mChara->m638;
    HitInfo_unk* hit;
    if (mChara->mType == 0) {
        if (m40->mC8 > 0) {
            hit = fn_80219E6C(m40->mC8);
        } else {
            hit = m40;
        }
    } else {
        hit = m40;
    }
    mChara->SetHit_unk(hit);
    m28 = hit->m58;
    m14 = mChara->mPos;
    if (target != 0) {
        m08 = *target;
        atn::Vector3 dir = m08 - m14;
        dir.y = 0.0f;
        dir.Normalize_unk();
        mChara->mRot = dir.ToRotation_unk();
        m38 = 1;
    } else {
        if (!IsCursorPlayer_unk(this)) {
            m08 = m14 + FrontDir_unk(mChara->mRot) * (10.0f + m28);
        } else {
            m08 = m14 + FrontDir_unk(mChara->mRot) * 20.0f;
        }
        m38 = 0;
    }
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x173, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    CollResult_unk res;
    atn::Vector3 from(m14);
    atn::Vector3 to(m14);
    from.y += 5.0f;
    to.y -= 60.0f;
    if (fn_8004ED34(lbl_8087EE98, &res, from, to, 0x80000000, 0, 0) != 0) {
        m14 = res.m04;
    }
    if (a != 0) {
        m2C = m40->mC0;
    }
    CharaTask* self = mChara;
    if (self->mType != 0 && self->mC58.mC4 != 0) {
        atn::Vector3 dir = FlatDir_unk(self->mC58.mC4->vf64(1), self->mPos);
        mChara->mRot = dir.ToRotationZ_unk();
    }
}

// NONMATCHING: 97.9% - registers of the two flags of the cursor test (r4/r5, as in the
// constructor), of the status word and of the copy of the cursor into the tactical view.
// inferred: charges while the button is held; returns 1 when the charge ends
int ControlCharge::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float rate = 1.0f;
    float bonus = 0.0f;
    HitInfo_unk* hit;
    int done = 0;
    bonus += rate * mChara->mStatus.fn_80134168(0x2B, -1) / 100.0f;
    rate += bonus;
    if (((unsigned int)mChara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= fn_8010CA24(lbl_8087F048);
    }
    m2C += rate * lbl_8087EFA8->m3A4;
    if (mChara->mType == 0) {
        if (!IsCursorPlayer_unk(this)) {
            fn_801C3458(0, 0.0f, 250.0f, 6.2831855f);
        }
        hit = m40;
        if (hit->mC8 > 0) {
            hit = fn_80219E6C(hit->mC8);
        }
        HitInfo_unk* base = m40;
        mChara->SetHit_unk(m2C >= base->mC0 ? base : hit);
        m28 = mChara->m638->m58;
        float frame = motion->mTracks[0].m08;
        if (frame > motion->fn_80097D7C(0)) {
            motion->fn_80097C08(0, 0x174, 1, 0, 1, 0.0f, 0.1f);
        }
        if (m34 == 0 && m2C >= base->mC0) {
            m34 = 1;
            if (lbl_8087F048 != 0) {
                lbl_8087F048->fn_801059F8(mChara);
                fn_80105A08(lbl_8087F048, mChara, &mChara->mMotion);
            }
        }
        atn::Vector3 pos(mChara->mPos);
        if (pos.y - m14.y < 4.0f && fn_801C3D1C() && m40->m01 == 1) {
            pos.y += 4.0f * rate / base->mC0;
        }
        mChara->mPos = pos;
        int released = 0;
        if (m3C != 0) {
            released = fn_800A55D4(lbl_8087EF70, 0, 0) == 0;
        } else {
            switch (SetConfig::sInstance->mWindKey) {
            case 0:
                released = fn_801240B4(&lbl_8087F0A8->mInput, 9) == 0;
                break;
            case 1:
                released = fn_801240B4(&lbl_8087F0A8->mInput, 9) == 0;
                break;
            case 2:
                released = fn_801240B4(&lbl_8087F0A8->mInput, 0);
                break;
            }
        }
        if (released) {
            if (m3C == 0 && !IsCursorPlayer_unk(this)) {
                if (m2C >= hit->mC0) {
                    if (m2C < base->mC0) {
                        lbl_8087F048->fn_801059F8(mChara);
                    }
                    m30 = 1;
                } else {
                    mChara->fn_8016DA4C();
                    m2C = 0.0f;
                }
            } else {
                mChara->fn_8016DA4C();
                m2C = 0.0f;
            }
            done = 1;
        } else if (IsCursorPlayer_unk(this) && m2C >= 20.0f) {
            if (m3C == 0 && mChara->m50 == 1) {
                TacCursor_unk cursor;
                cursor.m00 = 1;
                cursor.m04 = mChara->mPos + FrontDir_unk(mChara->mRot) * 5.0f;
                cursor.m10 = 100.0f;
                cursor.m14 = false;
                lbl_8087F9F8->m15D8 = cursor;
            }
            fn_80373FC4(lbl_8087F430);
            done = 1;
        }
        mChara->mF6C = m08;
        mChara->mF78 = m28;
        mChara->mFB8 = m2C;
    } else {
        hit = mChara->m638;
        CharaTask* target = mChara->mC58.mC4;
        if (hit == 0 || (target == 0 && m38 == 0)) {
            mChara->fn_8016DA4C();
            done = 1;
            m2C = 0.0f;
        } else {
            m28 = hit->m58;
            if (m38 == 0) {
                if (hit->m04 == 0x1788 || hit->m04 == 0x1791) {
                    MagicTarget_unk* redirect = fn_800185B4(lbl_8087EE68, mChara, hit);
                    if (redirect != 0 && redirect->m08 == 0 && redirect->m0C != 0) {
                        CharaTask* other = redirect->m0C;
                        if (other->CanAct_unk()) {
                            target = other;
                        }
                    }
                    m08 = target->vfD8(mChara);
                } else {
                    m08 = target->vf64(1);
                    m08 -= (target->mPos - mChara->mPos).GetNormalize() * 15.0f;
                }
            }
            if (m2C >= hit->mC0) {
                lbl_8087F048->fn_801059F8(mChara);
                m30 = 1;
                done = 1;
            }
        }
        mChara->mFB8 = m2C;
    }
    if (done && m30) {
        HitInfo_unk* hit = mChara->m638;
        if ((hit->m04 == 0x1788 || hit->m04 == 0x1791) && (hit->mAC & 0x40000) == 0x40000 &&
            fn_8036DAA8(lbl_8087F430) == 0) {
            done = 0;
        }
    }
    return done;
}

// NONMATCHING: 99.1% - registers of the inlined ControlChargeAttack constructor and stack slots of
// its two distance temporaries.
// inferred: starts the attack that follows the charge
boost::function<void()> ControlCharge::vf10() {
    if (m30 != 0) {
        HitInfo_unk* hit = mChara->m638;
        if (hit->m01 == 1) {
            ControlChargeAttack* next = new ((MemoryTag)0, "", "", 0) ControlChargeAttack(mChara, m08, m38 == 0);
            return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
        }
        if ((hit->m04 == 0x1788 || hit->m04 == 0x1791) && (hit->mAC & 0x40000) == 0x40000) {
            ControlQuarkSeirenAttack* next =
                new ((MemoryTag)0, "", "", 0) ControlQuarkSeirenAttack(mChara, m08, 150.0f, m38 == 0);
            return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
        }
        switch (mChara->m50) {
        case 700001:
        case 700002:
        case 700013:
        case 700014: {
            ControlHighJumpAttack* next =
                new ((MemoryTag)0, "", "", 0) ControlHighJumpAttack(mChara, m08, 10.0f, m38 == 0);
            return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
        }
        default: {
            ControlHighJumpAttack* next =
                new ((MemoryTag)0, "", "", 0) ControlHighJumpAttack(mChara, m08, 150.0f, m38 == 0);
            return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
        }
        }
    }
    return ControlBase::vf10();
}

ControlChargeAttackBase::ControlChargeAttackBase(Chara_unk* chara, const atn::Vector3& target, float speed, int adjust)
    : ControlBase(chara), m20(speed), m24(speed), m28(0) {
    if (chara->mFlag31) {
        chara->fn_801539E0();
    }
    m14 = mChara->mPos;
    m08 = target;
    if (adjust != 0) {
        fn_801A4E08(mChara, m14, m08);
    }
    atn::Vector3 dir = FlatDir_unk(m08, m14);
    mChara->mRot = dir.ToRotationZ_unk();
}

// NONMATCHING: 60% - the original stores x/y (psq_st) before z.
atn::Vector3 ControlChargeAttackBase::vf18() {
    return m08;
}

int ControlChargeAttackBase::vf28() {
    return m20 < 10.0f;
}

int ControlChargeAttackBase::vf24() {
    int result = 0;
    if (m28 != 0 && m20 > -6.0f) {
        result = 1;
    }
    return result;
}

// NONMATCHING: 97.3% - register and stack allocation of the loops of ray tests.
void ControlChargeAttackBase::fn_801A4E08(Chara_unk* chara, const atn::Vector3& from, atn::Vector3& target) {
    atn::Vector3 dir = target - from;
    if (dir.x * dir.x + dir.z * dir.z < 0.00001f) {
        target += FrontDir_unk(chara->mRot) * 0.01f;
        return;
    }
    PSVECNormalize(&dir, &dir);
    CollResult_unk res;
    unsigned int mask = chara->fn_80179D44() | 0x80000000;
    atn::Vector3 a(from);
    atn::Vector3 b(target);
    a.y += 3.0f;
    b.y += 3.0f;
    bool loop = true;
    int count = 0;
    while (loop) {
        loop = false;
        if (fn_8004ED34(lbl_8087EE98, &res, a, b, mask, 0, 0) != 0) {
            if (fn_803CC1A4(lbl_8087F430->m10D8, &res.m04, 6.0f, 0.25881904f, &dir) != 0) {
                a = res.m04 + dir * 0.1f;
                loop = PSVECSquareMag(&(a - b)) > 1.0f;
            } else if (PSVECDotProduct(&res.m28, &atn::Vector3(0.0f, 1.0f, 0.0f)) > 0.70710677f) {
                a.y += 3.0f;
                b.y += 3.0f;
                loop = count < 2;
                count++;
            } else {
                atn::Vector3 d = res.m04 - a;
                target = from + dir * (PSVECMag(&d) - chara->m620);
                break;
            }
        }
    }
    float best = 1e16f;
    bool hit = false;
    for (int i = 0; i < 4; i++) {
        atn::Vector3 off;
        if (i == 0) {
            off = atn::Vector3(0.0f, 0.0f, 0.0f);
        } else {
            atn::Vector3 axis;
            PSVECCrossProduct(&dir, &atn::Vector3(0.0f, 1.0f, 0.0f), &axis);
            off = axis;
            Mtx m;
            PSMTXRotAxisRad(m, &dir, 0.5f * (3.1415927f * (i + 1)));
            PSMTXMultVec(m, &off, &off);
            off *= 2.0f;
        }
        a = from + off;
        b = target + off;
        a.y += 10.0f;
        b.y += 10.0f;
        if (fn_8004ED34(lbl_8087EE98, &res, a, b, mask, 0, 0) != 0) {
            hit = true;
            atn::Vector3 d = res.m04 - a;
            float len = PSVECMag(&d);
            if (len < best) {
                best = len - chara->m620;
            }
        }
    }
    if (hit) {
        target = from + dir * best;
    }
    a = target;
    a.y += chara->m620;
    if (fn_8004D388(lbl_8087EE98, &res, &a, &lbl_807C7030, mask, 0, 0) != 0) {
        target.x = res.m10.x;
        target.z = res.m10.z;
        target.y = MAX(target.y, res.m10.y - chara->m620);
    }
    float y = target.y;
    float range = lbl_8087F610 != 0 ? 50.0f : 30.0f;
    float low = target.y - range;
    hit = false;
    for (int i = 0; i < 4; i++) {
        atn::Vector3 off;
        if (i == 0) {
            off = atn::Vector3(0.0f, 0.0f, 0.0f);
        } else {
            off = FrontDir_unk(chara->mRot);
            Mtx m;
            PSMTXRotRad(m, 'y', 2.0f * (3.1415927f * i) / 3.0f);
            PSMTXMultVec(m, &off, &off);
            off *= 2.0f;
        }
        a = target + off;
        b = target + off;
        a.y += 10.0f;
        b.y -= range;
        if (fn_8004ED34(lbl_8087EE98, &res, a, b, mask, 0, 0) != 0) {
            if (low < res.m04.y) {
                low = res.m04.y;
            }
            hit = true;
        }
    }
    if (hit) {
        target.y = low;
        if (y - low > 20.0f && lbl_8087F610 == 0) {
            a = from;
            b = target;
            a.y += 3.0f;
            b.y += 3.0f;
            if (fn_8004ED34(lbl_8087EE98, &res, a, b, mask, 0, 0) != 0 &&
                PSVECDotProduct(&res.m28, &atn::Vector3(0.0f, 1.0f, 0.0f)) > 0.9659258f) {
                target.y = y;
            }
        }
    }
    atn::Vector3 flat(target.x - from.x, 0.0f, target.z - from.z);
    if (PSVECSquareMag(&flat) < 0.00001f) {
        target += FrontDir_unk(chara->mRot) * 0.01f;
    } else {
        atn::Vector3 n(flat);
        PSVECNormalize(&n, &n);
        if (PSVECDotProduct(&n, &dir) < 0.0f) {
            target = from + FrontDir_unk(chara->mRot) * 0.001f;
        }
    }
    bool arena = false;
    if (lbl_8087F610 != 0 && IsArenaMode_unk(lbl_8087F610->m540)) {
        arena = true;
    }
    if (arena && fn_80373148(lbl_8087F430)->m4C == 1 && fn_80373148(lbl_8087F430)->m50 == 5) {
        CollResult_unk floor;
        atn::Vector3 top(target);
        atn::Vector3 bottom(target);
        top.y += 3.0f;
        bottom.y -= 100.0f;
        if (fn_8004ED34(lbl_8087EE98, &floor, top, bottom, chara->fn_80179D44() | 0x80000000, 0, 0) != 0) {
            target.y = floor.m04.y;
        }
    }
}

// NONMATCHING: 99.6% - r3/r4 swapped in the first stores (as in the ControlCharge constructor).
ControlChargeAttack::ControlChargeAttack(Chara_unk* chara, const atn::Vector3& target, int adjust)
    : ControlChargeAttackBase(chara, target, 20.0f, adjust) {
    mChara->m560 = 0xE;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x15D, 0, 0, 1, 0.0f, 0.1f);
    if (lbl_8087F610 == 0) {
        m14.y += 9.0f * MIN(1.0f, LengthXZ_unk(m08 - m14) / 20.0f);
    }
    mChara->mPos = m14;
    motion->mTracks[0].m0C = 1.0f;
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

ControlChargeAttackBase::~ControlChargeAttackBase() {}

// NONMATCHING: 95.2% - register allocation (the original keeps the motion in r31 and this in r30).
// inferred: falls on the target point; returns 1 at the end of the motion
int ControlChargeAttack::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float dt = lbl_8087EFA8->m3A4;
    m20 -= dt;
    float t = MAX(0.0f, m20 / 20.0f);
    mChara->mPos = Blend_unk(m14, m08, t);
    if (m28 == 0 && m20 <= 0.0f) {
        m28 = 1;
        mChara->fn_8016DA4C();
        CharaTask* chara = mChara;
        Effect_unk* fx = lbl_8087F048;
        HitInfo_unk* hit = chara->m638;
        fn_800FAB80(fx, mChara, hit, fn_800F8548(fx), &chara->mPos, &chara->mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        if (mChara->mStatus.HasTimer_unk()) {
            HitInfo_unk* extra = fn_80219E6C(0x1395);
            Effect_unk* fx2 = lbl_8087F048;
            fn_800FB4B0(fx2, mChara, extra, fn_800F8548(fx2), &mChara->mPos, 0, 30, -1, 1);
            if (mChara->fn_801789D8(0x1395, 0)) {
                mChara->fn_80178864(extra, mChara);
            }
        }
        mChara->mStatus.m08 = MAX(0.0f, mChara->mStatus.m08 - 0.25f * mChara->mStatus.mParamB.m88);
        mChara->mStatus.fn_80133130(0x4000000, 150, 0);
    }
    float end = motion->mTracks[0].m08;
    if (end >= motion->fn_80097D7C(0) - 4.0f) {
        motion->mTracks[0].m0C = 0.2f;
    }
    float frame = motion->mTracks[0].m08;
    return frame >= motion->fn_80097D7C(0) ? 1 : 0;
}

// NONMATCHING: 99.9% - r3/r4 swapped in the first stores (as in the ControlCharge constructor).
ControlHighJumpAttack::ControlHighJumpAttack(Chara_unk* chara, const atn::Vector3& target, float speed, int adjust)
    : ControlChargeAttackBase(chara, target, speed > 20.0f ? 50.0f : 20.0f, 0) {
    m2C = speed;
    mChara->m560 = 0xE;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1D2, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 0.5f;
    motion->mTracks[0].m08 = 12.0f;
    mChara->fn_801446F0();
    if (adjust != 0) {
        if (mChara->mType == 0 || mChara->mType == 3) {
            fn_801A4E08(mChara, m14, m08);
        } else {
            atn::Vector3 from(m08);
            from.y += 10.0f;
            atn::Vector3 to(from);
            to.y -= 30.0f;
            CollResult_unk res;
            if (fn_8004ED34(lbl_8087EE98, &res, from, to, mChara->fn_80179D44() | 0x80000000, 0, 0) != 0) {
                m08 = res.m04;
            }
            if (LengthXZSq_unk(m08 - m14) < 0.00001f) {
                m08 += FrontDir_unk(mChara->mRot) * 0.01f;
            }
        }
    }
    atn::Vector3 d = m08 - m14;
    if (PSVECSquareMag(&d) > 0.00001f) {
        mChara->mRot = atn::Vector3(0.0f, d.ToRotation_unk().y, 0.0f);
    }
    if (lbl_8087F048 != 0) {
        fn_8010653C(lbl_8087F048, mChara, &mChara->mMotion);
    }
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x20, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 90.5% - FPR allocation of the interpolation (the original keeps t in f13) and the
// registers of the hit and the effect manager.
// inferred: jumps along a parabola of height m2C to the target point
int ControlHighJumpAttack::vf0C() {
    int done = 0;
    Motion_unk* motion = &mChara->mMotion;
    float dt = lbl_8087EFA8->m3A4;
    m20 -= dt;
    float t = MAX(0.0f, m20 / m24);
    float s = 2.0f * t - 1.0f;
    atn::Vector3 pos = Blend_unk(m14, m08, t);
    pos.y += -m2C * (s * s) + m2C;
    mChara->mPos = pos;
    if (m28 == 0) {
        if (motion->mTracks[0].m08 >= 17.9999f) {
            if (m20 < 8.0f) {
                motion->mTracks[0].m0C = 1.0f;
            } else {
                motion->mTracks[0].m0C = 0.0f;
            }
        }
        if (m20 <= 0.0f) {
            m28 = 1;
            mChara->fn_8016DA4C();
            CharaTask* chara = mChara;
            HitInfo_unk* hit = chara->m638;
            atn::Vector3 at = mChara->mPos + FrontDir_unk(chara->mRot) * 10.0f;
            Effect_unk* fx = lbl_8087F048;
            fn_800FAB80(fx, mChara, hit, fn_800F8548(fx), &at, &chara->mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            if ((hit->mAC & 0x40000) == 0x40000) {
                mChara->mStatus.m228 = 0.0f;
                if (lbl_8087EE68 != 0 && fn_800185B4(lbl_8087EE68, mChara, hit) != 0) {
                    fn_80018608(lbl_8087EE68, mChara, hit);
                }
            }
            if (mChara->mType == 0 && lbl_8087F610 != 0) {
                mChara->mStatus.m08 = MAX(0.0f, mChara->mStatus.m08 - 0.25f * mChara->mStatus.mParamB.m88);
                mChara->mStatus.fn_80133130(0x4000000, 150, 0);
            }
        }
    } else {
        float frame = motion->mTracks[0].m08;
        if (frame >= 30.0f && frame <= 34.0f) {
            motion->mTracks[0].m0C = 0.25f;
        } else {
            motion->mTracks[0].m0C = 1.0f;
        }
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        if (lbl_8087F048 != 0) {
            fn_801065E4(lbl_8087F048, mChara);
        }
        CharaTask* chara = mChara;
        if (chara->mType == 2 || chara->mType == 3) {
            chara->SetHit_unk(0);
            mChara->fn_8015495C(0, 1, 1, 1);
        }
        done = 1;
    }
    return done;
}

void ControlHighJumpAttack::vf1C() {
    if (lbl_8087F048 != 0) {
        fn_801065E4(lbl_8087F048, mChara);
    }
}

// NONMATCHING: 99.8% - r3/r4 swapped in the first stores and in the copy of the front direction.
ControlQuarkSeirenAttack::ControlQuarkSeirenAttack(Chara_unk* chara, const atn::Vector3& target, float speed, int adjust)
    : ControlChargeAttackBase(chara, target, 77.0f, 0) {
    m2C = speed;
    mChara->m560 = 0xE;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x179, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    motion->mTracks[0].m08 = 0.0f;
    mChara->fn_801446F0();
    if (adjust != 0) {
        if (mChara->mType == 0) {
            fn_801A4E08(mChara, m14, m08);
        } else {
            atn::Vector3 from(m08);
            from.y += 10.0f;
            atn::Vector3 to(from);
            to.y -= 30.0f;
            CollResult_unk res;
            if (fn_8004ED34(lbl_8087EE98, &res, from, to, mChara->fn_80179D44() | 0x80000000, 0, 0) != 0) {
                m08 = res.m04;
            }
            if (LengthXZSq_unk(m08 - m14) < 0.00001f) {
                m08 += FrontDir_unk(mChara->mRot) * 0.01f;
            }
        }
    }
    atn::Vector3 d = m08 - m14;
    if (PSVECSquareMag(&d) < 0.00001f) {
        d = FrontDir_unk(mChara->mRot);
    }
    atn::Vector3 rot(0.0f, d.ToRotation_unk().y, 0.0f);
    if (mChara->mType != 0 && mChara->mC58.mC4 != 0) {
        rot.y = mChara->mC58.mC4->vfE0(d);
    }
    mChara->mRot = rot;
    if (mChara->m7C != 0 && ((VoiceEntry_unk*)mChara->m7C)->fn_8021FD7C(0x24) != 0) {
        char buf[0x100];
        sprintf(buf, "%s", ((VoiceEntry_unk*)mChara->m7C)->fn_8021FD7C(0x24));
        buf[strlen(buf) - 1] = 0;
        fn_800C31F4(buf, 0, -1, 1.0f);
    }
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x24, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 99.4% - registers of the hit block (an inline helper fixes them but moves the
// stack slots).
// inferred: warps to the target point after frame 61 and attacks there
int ControlQuarkSeirenAttack::vf0C() {
    int done = 0;
    Motion_unk* motion = &mChara->mMotion;
    float dt = lbl_8087EFA8->m3A4;
    m20 -= dt;
    if (mChara->mMotion.mTracks[0].m08 < 61.0f) {
        mChara->mPos = m14;
    } else {
        mChara->mPos = m08;
    }
    if (m28 == 0 && m20 <= 0.0f) {
        m28 = 1;
        mChara->mPos = m08;
        mChara->fn_8016DA4C();
        CharaTask* chara = mChara;
        HitInfo_unk* hit = chara->m638;
        atn::Vector3 at = mChara->mPos + FrontDir_unk(chara->mRot) * 10.0f;
        Effect_unk* fx = lbl_8087F048;
        fn_800FAB80(fx, mChara, hit, fn_800F8548(fx), &at, &chara->mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        if ((hit->mAC & 0x40000) == 0x40000) {
            mChara->mStatus.m228 = 0.0f;
            if (lbl_8087EE68 != 0 && fn_800185B4(lbl_8087EE68, mChara, hit) != 0) {
                fn_80018608(lbl_8087EE68, mChara, hit);
            }
        }
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        CharaTask* chara = mChara;
        if (chara->mType == 2 || chara->mType == 3) {
            chara->SetHit_unk(0);
            mChara->fn_8015495C(0, 1, 1, 1);
        }
        done = 1;
    }
    return done;
}

void ControlQuarkSeirenAttack::vf1C() {}

// inferred: steps back to the start point when the attack hit near the target point
boost::function<void()> ControlQuarkSeirenAttack::vf10() {
    if (m28 != 0) {
        atn::Vector3 d = mChara->mPos - m08;
        if (PSVECSquareMag(&d) < 900.0f) {
            ControlBackStep* next = new ((MemoryTag)0, "", "", 0) ControlBackStep(mChara, m14, 0);
            return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
        }
    }
    return ControlBase::vf10();
}

ControlZanTackle::ControlZanTackle(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 0xE;
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    m0C = mChara->m56C;
    m08 = fn_800F8548(lbl_8087F048);
}

// inferred: tackle of Zangurg, steered with the stick while button 9 is held
int ControlZanTackle::vf0C() {
    float speed = 0.0f;
    Motion_unk* motion = &mChara->mMotion;
    int done = 0;
    atn::Vector3 rot(mChara->mRot);
    int anim = mChara->mMotion.mTracks[0].m00;
    if (anim == 0x14A) {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            motion->fn_80097C08(0, 0x14B, 1, 0, 1, 0.0f, 0.1f);
        }
    } else if (anim == 0x14B) {
        speed = 1.0f;
        if (mChara->mType == 0) {
            float turn = mChara->m50 == 700023 ? 0.05f : 0.03f;
            rot.y += turn * (3.1415927f * -fn_800A56A8(lbl_8087EF70, 0, 0, 0));
            if (!fn_801240B4(&lbl_8087F0A8->mInput, 9)) {
                motion->fn_80097C08(0, 0x14C, 0, 0, 1, 0.0f, 0.1f);
            }
        }
        atn::Vector3 at = mChara->mPos + FrontDir_unk(mChara->mRot) * 5.0f;
        Effect_unk* fx = lbl_8087F048;
        fn_800FAB80(fx, mChara, fn_80219E6C(0xA4), m08, &at, &rot, 0, 30, -1, -1, 0.0f, 1.0f);
    } else {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            done = 1;
        }
    }
    mChara->vf34(&rot, 0, speed, 2.5f);
    mChara->m54C &= ~0x80;
    mChara->m54C &= ~0x100;
    return done;
}

} // namespace chr
