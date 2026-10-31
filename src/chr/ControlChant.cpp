// Chant of a magic (ControlChant), casting it (ControlMagic) and the end of the cast
// (ControlMagicEnd).
#include "chr/ControlChant.h"
#include "game/HitInfo_unk.h"
#include "game/HomingBullet.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/PlayerManager.h"
#include "gmk/Lemon.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>
#include "game/Input_unk.h"

// inferred: game settings (lbl_8087F0A8, as in chr/ControlCharge.cpp); names invented
struct Settings_unk {
    unsigned char m000[0x40];
    int m40;   // inferred: the casters rise while chanting
    unsigned char m044[0x2BC - 0x44];
    float m2BC; // inferred: first frame of the chant motions
    unsigned char m2C0[0x2C4 - 0x2C0];
    int m2C4;  // inferred: the magic follows the target
    unsigned char m2C8[0x48C - 0x2C8];
    Input_unk mInput; // 0x48C
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: arena mode object (lbl_8087F610, as in chr/ControlCharge.cpp)
extern "C" void* lbl_8087F610;
// inferred: game state (lbl_8087EFA8, as in game/CharaTask.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8;
// inferred: RGBA colour (names invented)
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
// inferred: effect manager of the characters (lbl_8087F048)
struct Effect_unk {
    void fn_80105B3C(CharaTask* chara); // inferred: effect of the special cast motion
};
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_80105AB0(Effect_unk* mgr, CharaTask* chara, chr::Motion_unk* model, const Color4f_unk& color);
extern "C" void fn_80105B4C(Effect_unk* mgr, CharaTask* chara, chr::Motion_unk* model, const Color4f_unk* color);
extern "C" float fn_8010CB2C(Effect_unk* mgr, CharaTask* chara, float rate); // inferred: chant speed
extern "C" void fn_801070C8(Effect_unk* effect, int id, int a, int b, chr::Motion_unk* motion,
                            const atn::Vector3* pos, const atn::Vector3* rot, const Color4f_unk& color, float scale);
extern "C" void fn_80107168(Effect_unk* effect, int id, int a, int b, chr::Motion_unk* motion,
                            const atn::Vector3* pos, const atn::Vector3* rot, const Color4f_unk& color, float scale);
extern "C" void fn_80107208(Effect_unk* effect, int id, const atn::Vector3* pos, float scale);
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10, float arg11);
extern "C" void fn_800FB4B0(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            int arg5, int arg6, int arg7, int arg8);
extern "C" void fn_801092C8(Effect_unk* effect, int msg, CharaTask* chara, int value, int arg);
extern "C" float fn_8010CCD8(Effect_unk* effect, CharaTask* chara); // inferred: gauge gained by the magic
// inferred: result of a damage computation (names invented, as in game/EnemyTasha.cpp)
struct MagicDamage_unk {
    MagicDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1), m18(-1), m1C_28(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C_31_28 : 4;
    unsigned int m1C_28 : 28;
};
// inferred: options of the damage computation (names invented)
struct MagicDamageOpt_unk {
    int m00;
    float m04;
    float m08;
    int m0C;
    float m10;
    int m14;
};
extern "C" char lbl_807C6B90[0x18]; // inferred: default options of the damage computation
extern "C" void fn_8003EA3C(MagicDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* target,
                            int flags, const void* options, atn::Vector3* a, atn::Vector3* b);
extern "C" void fn_8003F030(MagicDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, const atn::Vector3* pos,
                            const atn::Vector3* rot, int flags);
extern "C" void fn_80108C10(Effect_unk* effect, MagicDamage_unk* damage, const atn::Vector3& pos, CharaTask* attacker,
                            CharaTask* target, int a, int b);
extern "C" void* lbl_8087F498; // inferred: voice manager
extern "C" void fn_803EA77C(void* voice, CharaTask* chara, int id, int arg, float volume, float arg2);
// inferred: one line of the voice table (defined in atn/ptr_string_base_char_0.cpp)
struct VoiceEntry_unk {
    const char* fn_8021FD7C(int kind) const;
};
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned long strlen(const char* s);
extern "C" int rand();
// inferred: field object (class unknown, as in gmk/LottaSummon.cpp)
struct Field_unk {
    void fn_803750E4(int id);
    unsigned char m0000[0x10D0];
    int m10D0; // inferred: play time
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id);                     // inferred: field flag
extern "C" void fn_80370320(Field_unk* field, int id, int arg2, int arg3); // inferred: starts an event
extern "C" int fn_803761AC(Field_unk* field);
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed
// inferred: object behind lbl_8087F490 (as in game/CharaTask.cpp)
struct Unk8087F490_unk;
extern "C" Unk8087F490_unk* lbl_8087F490;
extern "C" void fn_803E2110(Unk8087F490_unk* obj, int arg, CharaTask* chara);
// inferred: tactics manager (TacView, as in game/CharaTask.cpp)
struct TacView_unk;
extern "C" TacView_unk* lbl_8087F9F8;
extern "C" int fn_805ADCC0(TacView_unk* manager, CharaTask* chara, HitInfo_unk* hit, int arg);
extern "C" void fn_805ADD80(TacView_unk* manager, CharaTask* chara, HitInfo_unk* hit);
// inferred: party manager (as in game/CharaTask.cpp)
extern "C" void* lbl_8087EE68;
extern "C" void* fn_800185B4(void* manager, CharaTask* chara, HitInfo_unk* hit);
extern "C" void fn_80018608(void* manager, CharaTask* chara, HitInfo_unk* hit);
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" int fn_80219558(int id);
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" int fn_80219344(CharaTask* chara, int arg);
extern "C" int fn_8021A888(HitInfo_unk* hit);
extern "C" int fn_8021A8D0(HitInfo_unk* hit);
extern "C" int fn_8021A918(HitInfo_unk* hit);
extern "C" int fn_8021A984(HitInfo_unk* hit);
extern "C" int fn_8036DAA8(Field_unk* field);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* axb);
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length (PSVECSquareMag)

// inferred: facing direction of the rotation `rot` (name invented, as in chr/ControlCdnStone.cpp)
inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', rot.y);
    PSMTXMultVec(mtx, &v, &v);
    return v;
}

// inferred: homing parameters of the magic bullets (defaults as in game/Em203GreaterDaemon.cpp)
struct MagicHoming_unk : public HomingParam_unk {
    MagicHoming_unk() {
        mTarget = 0;
        mAccel = 0.0f;
        m08 = -1.0f;
        mMaxSpeed = 10.0f;
        mTurn = 0.09424778f;
        mTime = 300.0f;
        mDelay = 5.0f;
        mNode = 0;
        mCount = -1;
    }
};

// inferred: colour of the chant effects by the element of the character (names invented)
inline Color4f_unk ElementColor_unk(CharaTask* chara) {
    Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    switch (fn_80219558(chara->m50)) {
    case 1:
    case 9:
    case 10:
        color.r = 1.0f;
        color.g = 0.9f;
        color.b = 0.5f;
        color.a = 1.0f;
        break;
    case 4:
        color.r = 1.0f;
        color.g = 0.5f;
        color.b = 0.1f;
        color.a = 1.0f;
        break;
    case 6:
        color.r = 0.7f;
        color.g = 0.8f;
        color.b = 1.0f;
        color.a = 1.0f;
        break;
    case 5:
        color.r = 0.7f;
        color.g = 1.0f;
        color.b = 0.7f;
        color.a = 1.0f;
        break;
    }
    return color;
}

// inferred: frame of the cast motion where the magic is released (names invented)
inline float CastFrame_unk(int motion) {
    switch (motion) {
    case 0x6B:
        return 62.0f;
    case 0x6A:
        return 24.0f;
    case 0x67:
        return 30.0f;
    case 0x17A:
        return 75.0f;
    case 0x17B:
        return 63.0f;
    }
    return 0.0f;
}

// inferred: a point around `center`, behind it along `dir`, at a random angle around `angle` and
// a random distance (names invented)
inline atn::Vector3 SpreadTarget_unk(const atn::Vector3& center, const atn::Vector3& dir, float angle) {
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', angle - 1.3962634f * ((rand() % 1001) / 1000.0f));
    atn::Vector3 off(atn::Vector3(0.0f, 0.0f, 10.0f * ((rand() % 1001) / 1000.0f) + 20.0f));
    PSMTXMultVec(mtx, &off, &off);
    return center - dir * 30.0f + off;
}

// inferred: a x b by value (name invented, as in chr/ControlChaseCaught.cpp)
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: chant sound
static const char* const sChantSe = "SE_BTCOM_031";

namespace chr {

ControlChant::ControlChant(Chara_unk* chara, CharaTask* target, const atn::Vector3& pos, int time, float range)
    : ControlMoveCursor(chara), m2C(0.0f), m40(1.0f), m44(false), m45(false), m46(false), m47(false), m48(1.0f) {
    m24 = true;
    CharaTask* c = mChara;
    c->m560 = 0x1D;
    Motion_unk* model = &mChara->mMotion;
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->fn_80097C08(0, 0x66, 1, 0, 1, 0.0f, 0.1f);
    model->mTracks[0].m0C = 1.0f;
    m3C = mChara->m638;
    if (lbl_8087F048) {
        fn_80105AB0(lbl_8087F048, mChara, model, ElementColor_unk(mChara));
    }
    m30 = fn_800C344C(sChantSe, &mChara->mPos, 0, -1, 1.0f);
    m08 = pos;
    m28 = range;
    m14 = mChara->mPos;
    m34 = target;
    m38 = time;
    CollResult_unk res;
    atn::Vector3 from(mChara->mPos);
    atn::Vector3 to(mChara->mPos);
    from.y += 5.0f;
    to.y -= 60.0f;
    if (fn_8004ED34(lbl_8087EE98, &res, &from, &to, 0x80000000, 0, 0)) {
        m14 = res.m04;
    }
    fn_803E2110(lbl_8087F490, 0, mChara);
    if (lbl_8087F498 && mChara->mType == 3 && (time > 90 || (m3C->mAC & 0x40000) == 0x40000)) {
        fn_803EA77C(lbl_8087F498, chara, 0x15, 0, 1.0f, 10.0f);
    }
}

ControlChant::~ControlChant() {
    fn_800CB5C8(&m30, 5, 0);
}

// NONMATCHING: 99.0% - saved registers of the locals (model, done, bullet), the mask 0x80000002
// kept in r24 before the first ray test, and the registers of the inlined CanAct_unk.
// inferred: chants while the button is held (player) or until the chant time ends (others);
// returns 1 when the chant ends
int ControlChant::vf0C() {
    int done = 0;
    Motion_unk* model = &mChara->mMotion;
    float rate = fn_8010CB2C(lbl_8087F048, mChara, m40);
    HitInfo_unk* hit;
    if (lbl_8087F9F8 && fn_805ADCC0(lbl_8087F9F8, mChara, mChara->m638, 0)) {
        if ((mChara->m638->mAC & 0x40000) != 0x40000) {
            m2C = m38 - 90.0f > m2C ? m38 - 90.0f : m2C;
        }
        fn_805ADD80(lbl_8087F9F8, mChara, mChara->m638);
    }
    if (mChara->mType == 0) {
        HitInfo_unk* bullet = m3C;
        int cancel = 0;
        rate *= 2.0f;
        if (bullet->mC8 > 0) {
            bullet = fn_80219E6C(bullet->mC8);
        }
        HitInfo_unk* base = m3C;
        mChara->SetHit_unk(m2C >= m38 ? base : bullet);
        if (lbl_8087F610) {
            m48 = m2C >= m38 ? 1.0f : 9.0f * (m2C / m38);
        }
        hit = mChara->m638;
        if (hit) {
            m28 = hit->m58;
            if ((int)hit->m02 == 4) {
                m34 = mChara;
                m08 = mChara->mPos + FrontDir_unk(mChara->mRot) * 0.1f;
            } else {
                m34 = 0;
            }
        } else {
            m28 = 1.0f;
            m34 = 0;
        }
        if (lbl_8087F0A8->mInput.fn_801240B4(5)) {
            m2C += rate * lbl_8087EFA8->m3A4 * mChara->mMotion.m350;
            fn_801C3458(0, bullet->m58, 1000.0f, 6.2831855f);
            if (m34) {
                m08 = m34->mPos;
            }
            if ((unsigned char)(hit->m02 - 3) <= 1) {
                m08 = m14 + FrontDir_unk(mChara->mRot) * 0.1f;
            }
        } else if (m2C >= m38 || (hit && m2C >= hit->mC0)) {
            m44 = true;
            done = 1;
            if (hit && base == hit && (unsigned int)(hit->m04 - 0x179A) <= 4) {
                m47 = true;
            }
        } else {
            cancel = 1;
        }
        if (cancel) {
            mChara->SetHit_unk(0);
            mChara->fn_8016DA4C();
            done = 1;
        }
    } else {
        hit = mChara->m638;
        m28 = hit ? hit->m58 : 1.0f;
        if (!m47) {
            if (!m34 || !m34->CanAct_unk()) {
                m08 = mChara->mC58.fn_8011BF3C(2);
            } else {
                m08 = m34->mPos;
            }
        }
        if (m2C >= m38) {
            m45 = true;
        }
        if (hit && !mChara->mC58.mD0 && !mChara->mC58.mC4) {
            m45 = false;
            done = 1;
        }
        if (m45) {
            int ok = 1;
            if (!mChara->mC58.mD0 && mChara->mC58.mC4) {
                CharaTask* target = mChara->mC58.mC4;
                if (target->m55C == 6 && target->m560 == 0x1D) {
                    m08.y += 5.0f;
                }
                if (!hit || (int)hit->m02 != 8) {
                    atn::Vector3 from(mChara->mPos);
                    from.y += 20.0f;
                    atn::Vector3 to(m08);
                    atn::Vector3 dir = to - from;
                    float dist = PSVECMag(&dir);
                    PSVECNormalize(&dir, &dir);
                    float radius = hit ? hit->m58 : 1.0f;
                    to -= dir * radius * 0.8f;
                    if (dist > radius && fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000002, 0, 0)) {
                        atn::Vector3 side = Cross_unk(dir, atn::Vector3(0.0f, 1.0f, 0.0f));
                        to = m08 - side * radius * 0.8f;
                        dir = to - from;
                        PSVECNormalize(&dir, &dir);
                        to -= dir;
                        if (fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000002, 0, 0)) {
                            to = m08 + side * radius * 0.8f;
                            dir = to - from;
                            PSVECNormalize(&dir, &dir);
                            to -= dir;
                            if (fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000002, 0, 0)) {
                                ok = 0;
                            } else {
                                m08 = to + dir;
                            }
                        } else {
                            m08 = to + dir;
                        }
                    }
                }
            }
            if (ok || m2C >= m38 + 60) {
                if (!m34 && !mChara->mC58.mD0 && mChara->mC58.mC4) {
                    m34 = mChara->mC58.mC4;
                }
                m44 = true;
                done = 1;
            }
        }
        m2C += rate * lbl_8087EFA8->m3A4;
    }
    atn::Vector3 d = m08 - mChara->mPos;
    if (atn::Abs_unk(PSVECSquareMag(&d)) < 1.1920929e-7f && mChara->mC58.mC4) {
        d = mChara->mC58.mC4->mPos - mChara->mPos;
    }
    if (mChara->mType != 0 && !(atn::Abs_unk(PSVECSquareMag(&d)) < 1.1920929e-7f)) {
        PSVECNormalize(&d, &d);
        atn::Vector3 rot(mChara->mRot);
        rot.y = d.ToRotation_unk().y;
        mChara->mRot = rot;
    }
    float limit = 12.0f;
    if (hit && (hit->mAC & 0x20)) {
        limit *= 1.5f;
    }
    if (lbl_8087F0A8->m40 && mChara->mPos.y - m14.y < limit && fn_801C3D1C()) {
        atn::Vector3 pos(mChara->mPos);
        pos.y += 0.08f * rate;
        mChara->mPos = pos;
    }
    if (!m46 && m2C >= m38 - 90) {
        m46 = true;
        Color4f_unk color = ElementColor_unk(mChara);
        if (mChara->mType == 0) {
            color.a = 0.3f;
        }
        fn_80105B4C(lbl_8087F048, mChara, model, &color);
    }
    if (mChara->mType == 3 && hit && m34 && !m34->CanAct_unk() && mChara->mC58.mC4 &&
        !mChara->fn_8016F3D0(m34, 1)) {
        m34 = mChara->mC58.mC4;
    }
    mChara->mF6C = m08;
    mChara->mF78 = m28;
    mChara->mF7C = m34;
    mChara->mFB8 = m2C;
    mChara->mFlagB8 = m45;
    if (mChara->mStatus.IsState_unk(4)) {
        m44 = false;
        done = 1;
    }
    if (done && m44 && (mChara->m638->mAC & 0x40000) == 0x40000 && !fn_8036DAA8(lbl_8087F430)) {
        done = 0;
    }
    return done;
}

// NONMATCHING: 96.7% - the original schedules the load of m44 and mChara into the prologue.
boost::function<void()> ControlChant::vf10() {
    if (m44) {
        return boost::bind(&CharaTask::fn_801603CC, (CharaTask*)mChara, m47, m48);
    }
    ControlMagicEnd* next = new ((MemoryTag)1, "", "", 0) ControlMagicEnd(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

// NONMATCHING: 99.8% - registers of the inlined CanAct_unk.
ControlMagic::ControlMagic(Chara_unk* chara, CharaTask* target, float f, const atn::Vector3& pos, int b)
    : ControlBase(chara), m18(0.0f), m1C(false), m1D(false), m1E(b), m1F(false), m20(f) {
    chara->m560 = 0x1E;
    HitInfo_unk* hit = mChara->m638;
    Motion_unk* model = &mChara->mMotion;
    m14 = target;
    if (target && !target->CanAct_unk() && mChara->mC58.mC4 && hit && (hit->m04 == 0x179A || hit->m04 == 0x179C)) {
        m14 = mChara->mC58.mC4;
    }
    if (!m14) {
        m14 = mChara;
    }
    m08 = pos;
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    if (hit) {
        if ((hit->mAC & 0x40000) == 0x40000) {
            switch (fn_80219558(mChara->m50)) {
            case 4:
            case 6:
                model->fn_80097C08(0, 0x17A, 0, 0, 1, 0.0f, 0.1f);
                model->mTracks[0].m0C = 1.2f;
                lbl_8087F048->fn_80105B3C(mChara);
                m1F = true;
                if (hit->mB0) {
                    fn_801070C8(lbl_8087F048, hit->mB0, 0, 0, &mChara->mMotion, &chr::lbl_807C7030, &chr::lbl_807C7030,
                                Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
                }
                break;
            case 1:
            case 5:
            case 9:
            case 10:
                model->fn_80097C08(0, 0x17B, 0, 0, 1, 0.0f, 0.1f);
                if (lbl_8087F048) {
                    lbl_8087F048->fn_80105B3C(mChara);
                }
                m1F = true;
                if (hit->mB0) {
                    fn_801070C8(lbl_8087F048, hit->mB0, 0, 0, &mChara->mMotion, &chr::lbl_807C7030, &chr::lbl_807C7030,
                                Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
                }
                break;
            default:
                model->fn_80097C08(0, 0x6B, 0, 0, 1, 0.0f, 0.1f);
                break;
            }
        } else if ((hit->mAC & 0x20) == 0x20) {
            model->fn_80097C08(0, 0x6B, 0, 0, 1, 0.0f, 0.1f);
        } else if (hit->mC0 <= 0) {
            model->fn_80097C08(0, 0x6A, 0, 0, 1, 0.0f, 0.1f);
        } else {
            model->fn_80097C08(0, 0x67, 0, 0, 1, 0.0f, 0.1f);
        }
    } else {
        model->fn_80097C08(0, 0x67, 0, 0, 1, 0.0f, 0.1f);
    }
    model->mTracks[0].m0C = 1.0f;
    if ((mChara->mType == 0 || mChara->mType == 3) && !m1F) {
        model->mTracks[0].m08 = lbl_8087F0A8->m2BC;
    }
    fn_803E2110(lbl_8087F490, 2, mChara);
    if (m1F) {
        int voice = 0x24;
        if (hit && (hit->m04 == 0x179A || hit->m04 == 0x6C2)) {
            voice = 0x25;
        }
        if (mChara->m7C != 0 && ((VoiceEntry_unk*)mChara->m7C)->fn_8021FD7C(voice) != 0) {
            char buf[0x100];
            sprintf(buf, "%s", ((VoiceEntry_unk*)mChara->m7C)->fn_8021FD7C(voice));
            buf[strlen(buf) - 1] = 0;
            fn_800C31F4(buf, 0, -1, 1.0f);
        }
        if (lbl_8087F498) {
            fn_803EA77C(lbl_8087F498, mChara, voice, 0, 1.0f, 10.0f);
        }
    } else if (lbl_8087F498) {
        fn_803EA77C(lbl_8087F498, mChara, 0x19, 0, 1.0f, 10.0f);
    }
}

// NONMATCHING: 90.4% - the stack frame is 0x30 bytes bigger in the original (slots of the bullet
// parameters, the four targets and the matrices), one more saved register, and mChara is loaded again
// for the motion id.
// inferred: casts the magic at the release frame of the motion and fires its bullets or applies
// its damage; returns 1 when the motion ends
int ControlMagic::vf0C() {
    HitInfo_unk* hit = mChara->m638;
    Motion_unk* model = &mChara->mMotion;
    int result = 0;
    int cast = 0;
    if (!m1D) {
        float end = CastFrame_unk(mChara->mMotion.mTracks[0].m00);
        if (model->mTracks[0].m08 >= end) {
            cast = 1;
        }
    } else {
        float dt = lbl_8087EFA8->m3A4;
        m18 += dt;
    }
    int follow = 0;
    if (lbl_8087F0A8->m2C4 && !m1D && (int)hit->m02 == 2 && m14 && m14 != mChara && m14->CanAct_unk()) {
        atn::Vector3 p = m14->vf64(2);
        m08 = p;
        atn::Vector3 d(p.x - mChara->mPos.x, 0.0f, p.z - mChara->mPos.z);
        if (PSVECSquareMag(&d) > 0.00001f) {
            d.fn_80139F60();
            mChara->mRot = d;
        }
        if (mChara->mType == 2 || m14->mFlagB11) {
            follow = 1;
        }
    }
    if (cast) {
        mChara->fn_8016DA4C();
        m1D = true;
        if (hit->m04 == 0x6C2) {
            if (fn_80370174(lbl_8087F430, 0x38D) > 0) {
                fn_80370320(lbl_8087F430, 0x38E, 1, 0);
                mChara->mStatus.m228 = 0.0f;
            }
            if (lbl_8087EE68 && fn_800185B4(lbl_8087EE68, mChara, hit)) {
                fn_80018608(lbl_8087EE68, mChara, hit);
            }
            return 1;
        }
        atn::Vector3 from(mChara->mPos);
        from.y += 20.0f;
        atn::Vector3 dir = m08 - from;
        float dist = PSVECMag(&dir);
        PSVECNormalize(&dir, &dir);
        switch ((signed char)hit->m02) {
        case 2:
        case 1:
        case 6:
        case 9:
            if ((unsigned int)(hit->m04 - 0x179B) <= 3) {
                atn::Vector3 flat(dir.x, 0.0f, dir.z);
                if (!(atn::Abs_unk(PSVECSquareMag(&flat)) < 1.1920929e-7f)) {
                    PSVECNormalize(&flat, &flat);
                }
                float yaw = atn::WrapAngle_unk(3.1415927f + flat.ToRotation_unk().y);
                atn::Vector3 targets[4];
                if (m1E) {
                    targets[3] = m08;
                    targets[0] = SpreadTarget_unk(targets[3], flat, yaw + 2.6179938f);
                    targets[1] = SpreadTarget_unk(targets[3], flat, yaw + 0.6981317f);
                    targets[2] = SpreadTarget_unk(targets[3], flat, yaw - 1.2217305f);
                } else {
                    m14->vfCC(targets, dir);
                }
                for (int i = 0; i < 4; i++) {
                    atn::Vector3 d = targets[i] - from;
                    float len = PSVECMag(&d);
                    if (len > 0.0f) {
                        PSVECNormalize(&d, &d);
                    }
                    int flags = 0x2002;
                    if (i < 3) {
                        flags |= 0x1000;
                    }
                    fn_800F8574(lbl_8087F048, mChara, hit, &from, &d, 0, flags, i * 15, len, 1.0f);
                }
            } else if (hit->m04 == 0x179A) {
                atn::Vector3 pos = m1E ? m08 : m14->mPos;
                fn_800FAB80(lbl_8087F048, mChara, hit, fn_800F8548(lbl_8087F048), &pos, &chr::lbl_807C7030, 0, 30, -1,
                            -1, 0.0f, 1.0f);
            } else if (follow) {
                MagicHoming_unk param;
                param.mTarget = m14;
                param.mTurn = 0.12566371f;
                param.mDelay = 0.0f;
                param.mTime = dist / hit->m4C;
                fn_800F8574(lbl_8087F048, mChara, hit, &from, &dir, &param, 0x100, 0, 0.0f, 1.0f);
            } else {
                fn_800F8574(lbl_8087F048, mChara, hit, &from, &dir, 0, 0, 0, 0.0f, m20);
            }
            break;
        case 8:
            if ((unsigned int)(hit->m04 - 0x179A) <= 4) {
                FixedArray_unk<CharaTask*, 16> enemies;
                for (EnemyTask* e = EnemyManager::sInstance->mHead; e; e = e->mNext) {
                    if (e->CanAct_unk() && (e->m54C & 0x2000) != 0x2000) {
                        enemies.PushBack_unk(e);
                        if (enemies.full()) {
                            break;
                        }
                    }
                }
                atn::Vector3 up = FrontDir_unk(mChara->mRot);
                up.y += 3.0f;
                PSVECNormalize(&up, &up);
                int count = 4;
                if (hit->m04 == 0x179A) {
                    count = 6;
                }
                const char* nodes[6] = {"neck", "leftarm", "rightarm", "hips", "leftleg", "rightleg"};
                for (int i = 0; i < count; i++) {
                    MagicHoming_unk param;
                    CharaTask* target;
                    if (enemies.mCount == 0) {
                        target = lbl_8087F8A0->mHead;
                    } else {
                        target = enemies[rand() % enemies.mCount];
                    }
                    param.mTarget = target;
                    float t = (i - 0.5f * (count - 1)) / (0.5f * (count - 1));
                    param.mTurn = 0.31415927f;
                    atn::Vector3 shot(up);
                    atn::Vector3 axis = FrontDir_unk(mChara->mRot);
                    Mtx mtx;
                    PSMTXRotAxisRad(mtx, &axis, 0.7853982f * t);
                    PSMTXMultVec(mtx, &shot, &shot);
                    param.mDelay = rand() % 5 + 15;
                    param.mNode = fn_800DC6B4(nodes[i]);
                    fn_800F8574(lbl_8087F048, mChara, hit, &from, &shot, &param, 2, 0, 0.0f, 1.0f);
                }
                if (hit->m04 == 0x179A) {
                    atn::Vector3 pos = m1E ? m08 : m14->mPos;
                    fn_800FAB80(lbl_8087F048, mChara, fn_80219E6C(0x17A1), fn_800F8548(lbl_8087F048), &pos,
                                &chr::lbl_807C7030, 0, 30, -1, -1, 0.0f, 1.0f);
                    model->mTracks[0].m0C = 0.5f;
                }
            } else {
                MagicHoming_unk param;
                param.mTarget = m14;
                fn_800F8574(lbl_8087F048, mChara, hit, &from, &dir, &param, 0, 0, 0.0f, 1.0f);
            }
            break;
        default:
            if (hit->m04 == 0x177F) {
                fn_800FB4B0(lbl_8087F048, mChara, hit, fn_800F8548(lbl_8087F048), &mChara->mPos, 0, 30, -1, 1);
            } else if (hit->m04 == 0x179F) {
                CharaTask* player = lbl_8087F8A0->mHead;
                MagicDamage_unk damage;
                fn_8003EA3C(&damage, hit, player, player, 0, lbl_807C6B90, 0, 0);
                if (hit->mB0) {
                    fn_80107168(lbl_8087F048, hit->mB0, 0, 0, &player->mMotion, &chr::lbl_807C7030,
                                &chr::lbl_807C7030, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
                    fn_80107208(lbl_8087F048, hit->mB0, &mChara->mPos, 1.0f);
                }
                player->mStatus.m08 = player->mStatus.mParamB.m88;
            } else {
                MagicDamage_unk damage;
                if (!m1E) {
                    MagicDamageOpt_unk opt;
                    opt.m00 = 1;
                    opt.m04 = 1.0f;
                    opt.m08 = 1.0f;
                    opt.m0C = 0;
                    opt.m10 = 1.0f;
                    opt.m14 = 0;
                    atn::Vector3 a;
                    atn::Vector3 b;
                    if (mChara->vfA4(&a, &b)) {
                        fn_8003EA3C(&damage, hit, mChara, m14, 0, &opt, &a, &b);
                    } else {
                        fn_8003EA3C(&damage, hit, mChara, m14, 0, lbl_807C6B90, 0, 0);
                    }
                    fn_80108C10(lbl_8087F048, &damage, m14->mPos + atn::Vector3(0.0f, 15.0f, 0.0f), mChara, m14, 0,
                                1);
                } else {
                    atn::Vector3 pos(m08);
                    fn_8003F030(&damage, hit, mChara, &pos, &mChara->mRot, 0);
                }
                if (hit->mB0) {
                    fn_801070C8(lbl_8087F048, hit->mB0, 0, 0, 0, &mChara->mPos, &mChara->mRot,
                                Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
                    fn_80107208(lbl_8087F048, hit->mB0, &mChara->mPos, 1.0f);
                }
            }
            if ((mChara->mType == 0 || mChara->mType == 3) && fn_8021A984(hit) &&
                fn_80370174(lbl_8087F430, 0x442) >= 7) {
                lbl_8087F430->fn_803750E4(0x9C);
            }
            break;
        }
        if (lbl_8087F430 && fn_803761AC(lbl_8087F430) && fn_80219344(lbl_8087F8A0->fn_8054A33C(0), 1) && mChara &&
            (mChara->mType == 0 || mChara->mType == 3)) {
            switch (fn_80219558(mChara->m50)) {
            case 1:
            case 9:
            case 10:
                if (fn_8021A888(hit)) {
                    lbl_8087F430->fn_803750E4(0xE0);
                }
                break;
            case 6:
                if (fn_8021A888(hit)) {
                    lbl_8087F430->fn_803750E4(0xF5);
                }
                break;
            case 4:
                if (fn_8021A888(hit) && lbl_8087F430 && lbl_8087F430->m10D0 >= 0x31CE0) {
                    lbl_8087F430->fn_803750E4(0x1C2);
                }
                break;
            }
        }
        if (lbl_8087F430 && fn_80375184(lbl_8087F430)) {
            if (mChara->mType == 2) {
                fn_801092C8(lbl_8087F048, 0xC, mChara, hit->m04, 0);
            } else {
                fn_801092C8(lbl_8087F048, 0xD, mChara, hit->m04, 0);
            }
        }
        if (lbl_8087F498 && (fn_8021A8D0(hit) || fn_8021A918(hit)) && mChara->mType == 2) {
            fn_803EA77C(lbl_8087F498, 0, 0x1E, 0x1E, 1.0f, 10.0f);
        }
        if ((fn_8021A8D0(hit) || fn_8021A918(hit)) && lbl_8087F048) {
            CharaTask* chara = mChara;
            chara->mStatus.fn_8012F034(fn_8010CCD8(lbl_8087F048, chara));
        }
        if ((hit->mAC & 0x40000) == 0x40000) {
            mChara->mStatus.m228 = 0.0f;
            if (lbl_8087EE68 && fn_800185B4(lbl_8087EE68, mChara, hit)) {
                fn_80018608(lbl_8087EE68, mChara, hit);
            }
        }
        mChara->mFlagB10 = 0;
    } else {
        float frame = model->mTracks[0].m08;
        if (frame >= model->fn_80097D7C(0)) {
            if (hit->mC0 <= 0) {
                if ((unsigned int)(mChara->mType - 2) <= 1) {
                    mChara->SetHit_unk(0);
                    mChara->fn_8016DA4C();
                    mChara->fn_8015495C(0, 1, 1, 1);
                }
                result = 1;
            } else {
                m1C = true;
                result = 1;
                if (mChara->m638 && mChara->m638->m04 == 0x179A) {
                    mChara->mStatus.m228 = 0.0f;
                }
            }
        }
    }
    return result;
}

boost::function<void()> ControlMagic::vf10() {
    if (m1C) {
        ControlMagicEnd* next = new ((MemoryTag)1, "", "", 0) ControlMagicEnd(mChara);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    return ControlBase::vf10();
}

int ControlMagic::vf28() {
    CharaTask* chara = mChara;
    int result = 0;
    if (chara->m638) {
        float end = CastFrame_unk(chara->mMotion.mTracks[0].m00);
        float frame = chara->mMotion.mTracks[0].m08;
        if (end - 10.0f <= frame && frame <= end) {
            result = 1;
        }
    }
    return result;
}

int ControlMagic::vf50() {
    CharaTask* chara = mChara;
    int result = 0;
    if (chara->m638) {
        float end = CastFrame_unk(chara->mMotion.mTracks[0].m00);
        float frame = chara->mMotion.mTracks[0].m08;
        if (end - 20.0f <= frame && !m1D) {
            result = 1;
        }
    }
    return result;
}

// NONMATCHING: 81.5% - the original loads mChara before 20.0f and m08 after the copy of the position.
atn::Vector3 ControlMagic::vf18() {
    atn::Vector3 from = mChara->mPos;
    from.y += 20.0f;
    atn::Vector3 dir = m08 - from;
    PSVECNormalize(&dir, &dir);
    return dir;
}

ControlMagicEnd::ControlMagicEnd(Chara_unk* chara) : ControlDropDown(chara, lbl_807C7030) {
    mChara->m560 = 0x1F;
}

} // namespace chr
