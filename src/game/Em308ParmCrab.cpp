#include "game/Em308ParmCrab.h"
#include "game/HitInfo_unk.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: float color converted to GXColor (as in game/Em305VampireMp.cpp)
struct ColorF_unk {
    ColorF_unk() {}
    ColorF_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};
extern "C" ColorF_unk lbl_807C8398; // inferred: color of the downed enemies (bss)
// inferred: game settings (lbl_8087F0A8); frames per second at 0x30 (as in game/Em308InvertCrab.cpp)
struct CrabSettings_unk {
    unsigned char m00[0x30];
    int mFps;
};
extern "C" CrabSettings_unk* lbl_8087F0A8;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);
extern "C" void fn_80108F38(Effect_unk* effect, const atn::Vector3& pos, int status); // inferred: status effect

// NONMATCHING: 99.83% - r26/r27 of the second std::string constructor (known blocker, as in Em308InvertCrab)
// inferred: same pattern as Em308InvertCrab's constructor (data file "database/chara/special/" + db_name)
Em308ParmCrab::Em308ParmCrab(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14E8(60), m14EC(15), m14F0(70.0f), m14F8(180.0f),
      m152C(30.0f), m1534(179.0f), m1538(0), m1540(0), m1544(0.0f), m1548(0.0f), m1574(300), m157C(45),
      m1580(45), m1584(25.0f), m1588(50.0f), m158C(90.0f), m1590(0), m1594(0.5f), m15B8(0), m15BC(0),
      m15C8(0), m15CC(0), m15D0(90), m15D4(90), m15DC(0), m15E0(1.0f), m15E4(1.0f), m15E8(1.0f),
      m15EC(1.0f), m15F0(1.0f), m1618(0) {
    std::string dir("database/chara/special/");
    std::string name("db_em308_00_00.csv");
    atn::Csv csv(info->m2C, strlen(info->m2C));
    do {
        const char* s = csv.fn_8005B9CC();
        if (s == 0 || strcmp(s, "") == 0) {
            break;
        }
        if (strncmp(s, "db_name=", 8) == 0) {
            name = s + 8;
        }
    } while (csv.fn_8005BBF8());
    std::string path = fn_8006AFF8(dir, name);
    m160C.vf0C(path.c_str());
    m568 = 0.5f;
    m1530 = fn_80219E6C(0x3F8);
    m14F4 = fn_80219E6C(0x3F7);
    m1578 = fn_80219E6C(0x400);
    m15F4.fn_8023780C("/effect/ef_uc001.efp");
    m1520.fn_8023780C("/effect/ef_sk002d.efp");
    m14DC.fn_8023780C("/effect/ef_uc007.efp");
    m1600.fn_8023780C("/effect/ef_sc003_01b.efp");
}

Em308ParmCrab::~Em308ParmCrab() {}

// inferred: the colour of "em202_body"/"em202_tail" moves 15% per frame towards white (or the downed colour)
// NONMATCHING: 94.14% - stack order and scheduling of the colour temporaries (target - cur, * 0.15f); ~12 forms
// (field-by-field, operators by reference/value, Lerp helper)
inline ColorF_unk operator-(const ColorF_unk& x, const ColorF_unk& y) {
    return ColorF_unk(x.r - y.r, x.g - y.g, x.b - y.b, x.a - y.a);
}
inline ColorF_unk operator+(const ColorF_unk& x, const ColorF_unk& y) {
    return ColorF_unk(x.r + y.r, x.g + y.g, x.b + y.b, x.a + y.a);
}
inline ColorF_unk operator*(const ColorF_unk& x, float f) {
    return ColorF_unk(x.r * f, x.g * f, x.b * f, x.a * f);
}

void Em308ParmCrab::vf14() {
    if (mMotion.fn_80092954("em202_body") != 0 && mMotion.fn_80092954("em202_tail") != 0) {
        ColorF_unk target;
        target.r = 1.0f;
        target.g = 1.0f;
        target.b = 1.0f;
        target.a = 1.0f;
        if (IsDown_unk()) {
            target = lbl_807C8398;
        }
        ColorF_unk& cur = *(ColorF_unk*)&m15E4;
        ColorF_unk d = target - cur;
        ColorF_unk c = d * 0.15f + cur;
        cur = c;
        mMotion.fn_80092954("em202_body")->mColor = c.ToGX();
        mMotion.fn_80092954("em202_tail")->mColor = cur.ToGX();
    }
    fn_80149A30();
}

// inferred: stops the attack sound and effect 200
inline void Em308ParmCrab::StopAttack_unk() {
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

// inferred: cancels the current attack (wait frames from the skill 0x3F7, sound and effect 200 stopped)
inline void Em308ParmCrab::ResetAttack_unk() {
    m14B0 = 0;
    m15CC = 0;
    m15D4 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    StopAttack_unk();
}

// inferred: capsule as in Em308InvertCrab::vf20 but upwards (0.7 * radius), from 2 units below the centre
// NONMATCHING: 99.78% - r3/r4 of the __VecCopy of `off` (known blocker); the front vector as inline
// helper with mRot.y in a local fixed the PSMTXRotRad scheduling (~15 forms)
inline atn::Vector3 CrabFront_unk(const atn::Vector3& rot) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = rot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &front, &front);
    return front;
}

void Em308ParmCrab::vf20() {
    atn::Vector3 pos = mPos;
    pos.y -= 2.0f;
    atn::Vector3 off(0.0f, 0.0f, 0.0f);
    atn::Vector3 bottom;
    if (m58C == 8) {
        off = CrabFront_unk(mRot) * 10.0f;
    }
    pos += off;
    m620 = 0.7f * m5B0;
    m614 = pos;
    bottom = pos;
    m614.y += 0.7f * m5B0;
    bottom.y = mPos.y + 0.7f * m5B0;
    mColli.m3C = pos;
    mColli.m48 = bottom;
    mColli.m54 = m620;
}

// inferred: as Em308InvertCrab::vf3C: knockback only while down or from behind (guard angle m1534); in
// state 14 (stunned) the knockback is cancelled
// NONMATCHING: 98.84% - r5/r6 of the down test and of the direction temporaries (as Em308InvertCrab::vf3C)
// and the load order of m1534 / 0.5f before cos (~12 forms)
void Em308ParmCrab::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (!IsDown_unk()) {
        if (m58C == 8) {
            msg->m10 = lbl_807C7030;
        }
        if (!(atn::Abs_unk(m1534) < 1.1920929e-7f) && msg->mData->m04 != 0xD1) {
            atn::Vector3 dir;
            atn::Vector3 d = msg->m1C - mPos;
            dir = d;
            PSVECNormalize(&dir, &dir);
            if (m1538 != 0) {
                dir *= -1.0f;
            }
            if (msg->mData->m01 != 1) {
                atn::Vector3 front(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &front, &front);
                float dot = PSVECDotProduct(&dir, &front);
                if (dot > (float)cos(atn::DegToRad_unk(m1534 * 0.5f))) {
                    msg->m48 = 1;
                    msg->m10 = lbl_807C7030;
                }
            }
        }
    }
    if (msg->m48 == 0 && m58C == 8 && m15CC == 1) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        fn_800CB5C8(&m151C, 0, 0);
    }
    if (m58C == 14) {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
}

// inferred: dies when the status 0x20 is set; the stun skill 0x1791 starts state 14 (motion 2)
void Em308ParmCrab::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m1590++;
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 6;
        ResetAttack_unk();
        StopAttack_unk();
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802FC188();
    } else if (msg->mData != 0 && msg->mData->m04 == 0x1791) {
        ResetAttack_unk();
        m58C = 14;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 0.0f;
        mMotion.fn_80097C08(0, 2, 1, 1, 1, 0.0f, 0.1f);
        fn_80232B7C(this, 200);
        atn::Vector3 pos;
        atn::Vector3 rot;
        Color4f_unk color;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m1600, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        Effect_unk* fx = lbl_8087F048;
        fn_80108F38(fx, fn_801781B0(), 0x80);
    }
}

// inferred: gravity per frame (as in game/Em308InvertCrab.cpp)
inline float Gravity_unk(CrabSettings_unk* s) {
    return -98.1f / (float)(s->mFps * s->mFps);
}

// inferred: jump to pos + offset (state 13), as Em308InvertCrab::vf80; also keeps the flight time in m15DC
// NONMATCHING: 82.04% - stack order of the temporaries (dest at 0x8 in the original) and scheduling of the
// two int->float conversions of the frame count; ~8 forms (inline helper included)
void Em308ParmCrab::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    ResetAttack_unk();
    m58C = 13;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x68, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 dest = pos + offset;
    int frames = (int)mMotion.fn_80097D7C(0);
    m15DC = frames;
    m154C = (dest - mPos) * (1.0f / (float)m15DC);
    m15D0 = 0;
    mColli.mFlags &= ~1;
    m154C.y -= 0.5f * ((float)m15DC * Gravity_unk(lbl_8087F0A8));
}

// inferred: plays an effect on the model (as in game/Em308InvertCrab.cpp)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: death motion: at its end counts m1434 down (the effect at 29), else faces the direction m6B8
// NONMATCHING: 98.67% - the original passes &rot to fn_8013CB68 from r31 (the base of the inlined
// ToRotation_unk); ~10 forms (in-place fn_80139F60, RotZ_unk, helper, pointer local)
void Em308ParmCrab::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            if (--m1434 == 29) {
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m15F4, &mMotion);
                vf9C(1);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        atn::Vector3 rot = m6B8.ToRotation_unk();
        fn_8013CB68(&rot, 0, 0.0f, 1.0f);
        m1434 = 30;
    }
}

// inferred: starts a turn (state 9, motion 0x14D) towards target, unless it is within 22.5 degrees; the
// state `next` follows the turn
int Em308ParmCrab::fn_802FBE00(CharaTask* target, int next) {
    if (m58C == 9) {
        return 0;
    }
    m1544 = mRot.y;
    atn::Vector3 rot = (target->mPos - mPos).ToRotationZ_unk();
    m1548 = rot.y;
    float d = atn::WrapAngle_unk(rot.y - m1544);
    if (-0.3926991f < d && d < 0.3926991f) {
        return 0;
    }
    m1540 = 0;
    m153C = next;
    m15C4 = target;
    ResetAttack_unk();
    m58C = 9;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    return 1;
}

void Em308ParmCrab::fn_802FC188() {
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800CB5C8(&m151C, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    ResetAttack_unk();
    m58C = 2;
    fn_8016E970(9);
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0(this);
}

float Em308ParmCrab::vf90() {
    return 3.5f;
}
