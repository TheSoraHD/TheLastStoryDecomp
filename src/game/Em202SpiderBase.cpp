#include "game/Em202SpiderBase.h"
#include "game/HitInfo_unk.h"
#include "math/Vec.h"
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
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" double cos(double x);
extern "C" double sqrt(double x);
// inferred: frame rate (as in chr/ControlTurnQuadruped.cpp)
struct System_unk {
    unsigned char m00[0x30];
    int mFrameRate; // 0x30
};
extern "C" System_unk* lbl_8087F0A8;
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3* pos, int arg);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: color of the spiders when they are down; defined here (its __sinit is the .ctors entry of this
// file) and read by game/Em202Spider.cpp, Em202ForestSpider, Em202_01_BSpider and Em308ParmCrab
// NONMATCHING (__sinit): 99.00% - only the .sdata2 offsets of 0.7f/0.2f/1.0f (constants of the unwritten
// functions)
extern "C" {
SpiderBaseColor_unk lbl_807C8398(0.7f, 0.2f, 0.2f, 1.0f);
}

// NONMATCHING: 99.81% - r26/r27 of the second std::string constructor (as in Em201ZanGurg)
Em202SpiderBase::Em202SpiderBase(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14E8(60), m14EC(15), m14F0(70.0f), m14F8(180.0f),
      m152C(30.0f), m1534(3.1415927f), m153C(0), m1540(0.0f), m1544(0.0f), m1554(0), m157C(0), m1580(0),
      m1584(0), m158C(0), m1590(90), m1594(90), m159C(0), m15A0(1.0f), m15D8(0) {
    std::string dir("database/chara/special/");
    std::string name("db_em202_base_00.csv");
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
    m15BC.vf0C(path.c_str());
    m568 = 0.5f;
    m1530 = fn_80219E6C(0x3F8);
    m14F4 = fn_80219E6C(0x3F7);
    m15A4.fn_8023780C("/effect/ef_uc001.efp");
    m1520.fn_8023780C("/effect/ef_sk002d.efp");
    m14DC.fn_8023780C("/effect/ef_uc007.efp");
    m15B0.fn_8023780C("/effect/ef_sc003_01b.efp");
}

Em202SpiderBase::~Em202SpiderBase() {}

// NONMATCHING: 92.87% - same code and same causes as Em202Spider::vf14 (strings of the unwritten functions,
// stack slots of the color temporaries)
void Em202SpiderBase::vf14() {
    SpiderBaseColor_unk target;
    if (IsDown_unk()) {
        target = lbl_807C8398;
    }
    m15C4 = (target - m15C4) * 0.15f + m15C4;
    mMotion.fn_80092954("em202_body")->mColor = m15C4.ToGX();
    mMotion.fn_80092954("em202_tail")->mColor = m15C4.ToGX();
    fn_80149A30();
}

// NONMATCHING: 99.80% - only the .sdata2 offsets (constants of the unwritten functions)
void Em202SpiderBase::fn_802847CC() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 dir(mRoute.m58);
        speed = dir.Length_unk();
        if (speed > 0.0001f) {
            rot = dir.GetNormalized_unk().ToRotation_unk();
        }
        CharaTask* t = m1580;
        dir = t->mPos - mPos;
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        dir.fn_80139F60();
        mRot.y = dir.y;
        if (m58C == 12) {
            rate *= m155C;
        }
        fn_801426A4(&rot, speed, m568 * rate);
    } else if (m55C == 6) {
        fn_8013A258();
    } else {
        rot = mRot;
        fn_8013CB68(&rot, 0, speed, m568 * rate);
    }
}

// NONMATCHING: 99.81% - only the .sdata2 offsets (constants of the unwritten functions)
// inferred: the direction to the point is turned into a rotation that is not used
void Em202SpiderBase::fn_802868A0() {
    atn::Vector3 rot;
    atn::Vector3 d = m1564 - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    rot = d.ToRotation_unk();
    if (dist < 20.0f) {
        fn_8016E970(3);
        m1554 = 0;
        if (fn_80287400(m1580, 8) == 0) {
            fn_8028702C(m1580);
        }
        return;
    }
    if (m55C - 6U > 1) {
        m55C = 3;
        fn_8017039C(*m1560, 0);
    }
    fn_802847CC();
}

// NONMATCHING: 99.84% - only the .sdata2 offsets (constants of the unwritten functions)
void Em202SpiderBase::fn_80286CB0(CharaTask* target) {
    if ((m157C & 2) != 2) {
        if (fn_80287400(target, 8) == 0) {
            fn_8028702C(m1580);
        }
    } else if (fn_80287400(target, 7) == 0) {
        ResetAttack_unk();
        m58C = 7;
        fn_8016E970(6);
        m560 = 4;
        m590 = fn_800F8548(lbl_8087F048);
        atn::Vector3 pos(target->mPos);
        atn::Vector3 rot = (pos - mPos).GetNormalized_unk().ToRotation_unk();
        mRot = rot;
        mRot.x = 0.0f;
        mRot.z = 0.0f;
        fn_80097CCC(&mMotion, 0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    }
}

// NONMATCHING: 99.86% - only the .sdata2 offsets (constants of the unwritten functions)
void Em202SpiderBase::fn_8028702C(CharaTask* target) {
    if ((m157C & 4) != 4) {
        if (fn_80287400(target, 6) == 0) {
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
        }
    } else if (fn_80287400(target, 8) == 0) {
        m58C = 8;
        fn_8016E970(3);
        ResetAttack_unk();
        atn::Vector3 pos(target->mPos);
        atn::Vector3 rot = (pos - mPos).GetNormalized_unk().ToRotation_unk();
        mRot.x = 0.0f;
        mRot.y = rot.y;
        mRot.z = 0.0f;
        m1598 = 0;
        fn_80097CCC(&mMotion, 0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
    }
}

// NONMATCHING: 99.67% - only the .rodata/.sdata2 offsets (constants of the unwritten functions)
int Em202SpiderBase::fn_80287400(CharaTask* target, int arg) {
    if (m58C == 9) {
        return 0;
    }
    m1540 = mRot.y;
    atn::Vector3 rot = (target->mPos - mPos).ToRotationZ_unk();
    m1544 = rot.y;
    float d = atn::WrapAngle_unk(rot.y - m1540);
    if (-0.3926991f < d && d < 0.3926991f) {
        return 0;
    }
    m153C = 0;
    m1538 = arg;
    m1588 = target;
    ResetAttack_unk();
    m58C = 9;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    return 1;
}

void Em202SpiderBase::fn_80287788() {
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800CB5C8(&m151C, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    m14B0 = 0;
    m158C = 0;
    m1594 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    m6B8 = lbl_807C7030;
    fn_800EB7A0(this);
}

// inferred: same as Em202Spider::vfEC without the fn_802829DC call and the stop of the effects 1000
void Em202SpiderBase::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
            if (m1434 == 29) {
                fn_80232B7C(0, 0);
                Color4f_unk color;
                atn::Vector3 rot;
                atn::Vector3 pos;
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
                fn_8023A8B4(lbl_8087F3C0, &m15A4, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
                vf9C(1);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 30;
    }
}

// inferred: stops the needle attack (as at the start of vf7C and fn_80287788; name invented)
inline void Em202SpiderBase::ResetAttack_unk() {
    m14B0 = 0;
    m158C = 0;
    m1594 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

// inferred: dot of the front of the character with the direction to pos (name invented)
inline float FrontDot_unk(CharaTask* self, const atn::Vector3& pos) {
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = self->mRot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &fwd, &fwd);
    atn::Vector3 d(pos - self->mPos);
    return PSVECDotProduct(&d, &fwd);
}

// NONMATCHING: 99.89% - only the .sdata2 offsets (constants of the unwritten functions)
// inferred: the spider guards the hits inside the angle m1534 from the front (from the back in state 8)
void Em202SpiderBase::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    bool down = true;
    unsigned int flags = mStatus.m0C;
    if ((flags & 0x80000) != 0x80000 && (flags & 0x1000000) != 0x1000000) {
        down = false;
    }
    if (!down && m58C != 14) {
        if (m58C == 8) {
            msg->m10 = lbl_807C7030;
        }
        if (msg->mData->m01 != 1 && !((float)__fabs(m1534) < 1.1920929e-7f)) {
            int guard = 0;
            float dot = FrontDot_unk(this, msg->m1C);
            if (m58C == 8) {
                if (dot < -(float)cos(m1534 / 2.0f)) {
                    guard = 1;
                }
            } else if (dot > (float)cos(m1534 / 2.0f)) {
                guard = 1;
            }
            if (guard) {
                msg->m10 = lbl_807C7030;
                msg->m48 = 1;
            }
        }
        if (msg->m48 == 0 && m58C == 8 && m158C == 1) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
            fn_800CB5C8(&m151C, 0, 0);
        }
    }
    if (m58C == 15) {
        msg->m10 *= 0.0f;
    }
    float len = fn_805F9920(&msg->m10);
    if (!((float)__fabs(len) < 1.1920929e-7f) && len >= 25.0f) {
        PSVECNormalize(&msg->m10, &msg->m10);
        msg->m10 *= 4.9f;
    }
    fn_80151448(msg);
    vf40(hit);
}

// NONMATCHING: 99.97% - only the .sdata2 offset of 0.1f (constant of the unwritten functions)
void Em202SpiderBase::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m1554++;
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 6;
        ResetAttack_unk();
        fn_800CB5C8(&m151C, 15, 0);
        fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_80287788();
        return;
    }
    if (msg->mData != 0 && msg->mData->m04 == 0x1791) {
        ResetAttack_unk();
        m58C = 15;
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
        fn_8023A8B4(lbl_8087F3C0, &m15B0, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        Effect_unk* mgr = lbl_8087F048;
        atn::Vector3 p = fn_801781B0();
        fn_80108F38(mgr, &p, 0x80);
    }
}

void Em202SpiderBase::vf7C(int arg) {
    m14B0 = 0;
    m158C = 0;
    m1594 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    m58C = 14;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}

// NONMATCHING: 83.15% - order of the int-to-float conversions of `frames` and of the vector temporaries
// (10 variants)
// inferred: jumps to pos + offset (state 13) in the frames of the motion 0x68
void Em202SpiderBase::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    ResetAttack_unk();
    m58C = 13;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x68, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 dest = pos + offset;
    int frames = (int)mMotion.fn_80097D7C(0);
    m159C = frames;
    float inv = 1.0f / frames;
    m1548 = (dest - mPos) * inv;
    m1590 = 0;
    mColli.mFlags &= ~1;
    int fps = lbl_8087F0A8->mFrameRate;
    m1548.y -= 0.5f * (frames * (-98.1f / (float)(fps * fps)));
}
