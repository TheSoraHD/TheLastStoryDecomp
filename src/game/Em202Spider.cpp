#include "game/Em202Spider.h"
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
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3* pos, int arg);

// NONMATCHING: 96.39% - written for the order of the strings; registers r28-r31 of the std::string
// constructors swapped (as in Em101LittleZesya)
Em202Spider::Em202Spider(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14D8(0), m14DC(55.0f), m14E4(0), m14E8(0), m14EC(0), m14F0(0), m14FC(0),
      m1500(0), m1504(0), m1508(0), m150C(0), m1510(0), m1514(0), m1518(0), m151C(0), m1520(0), m1524(0),
      m1528(0), m152C(0), m1534(60), m1538(0), m153C(0.0f), m1544(0), m1548(0), m154C(1) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m55C = 3;
    m14C0.clear();
    m14CC.clear();
    m14F4 = 0;
    m14F8 = 0;
    m151C = 0;
    m14B8 = 0;
    m14BC = 0;
    std::string dir("database/chara/special/");
    std::string name("db_em202_00_00.csv");
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
    m14B0.vf0C(path.c_str());
    m1550.fn_8023780C("/effect/ef_uc019.efp");
    m155C.fn_80237654("/effect/ef_sk002b.eff");
    m1568.fn_8023780C("/effect/ef_uc001.efp");
    m1574.fn_8023780C("/effect/ef_sk002d.efp");
    m1580.fn_8023780C("/effect/ef_uc007.efp");
    m158C.fn_8023780C("/effect/ef_sc003_01b.efp");
}

Em202Spider::~Em202Spider() {}

// inferred: color of the spider when it is down (0.7, 0.2, 0.2, 1.0; made by the __sinit of
// game/Em202SpiderBase.cpp)
extern "C" SpiderColor_unk lbl_807C8398;

// NONMATCHING: 92.84% - @stringBase0 offsets (strings of vf0C/vf10 not written) and stack slots of the color
// temporaries (the original keeps the difference above the product and saves f31)
void Em202Spider::vf14() {
    SpiderColor_unk target;
    if (IsDown_unk()) {
        target = lbl_807C8398;
    }
    m1598 = (target - m1598) * 0.15f + m1598;
    mMotion.fn_80092954("em202_body")->mColor = m1598.ToGX();
    mMotion.fn_80092954("em202_tail")->mColor = m1598.ToGX();
    fn_80149A30();
}

// NONMATCHING: 99.68% - only the .rodata double (cos(90.0)) placed by the unwritten functions
// inferred: the spider guards the hits from the front (from the back in the states 4 and 5)
// inferred: dot of the front of the spider with the direction to pos (name invented)
inline float FrontDot_unk(CharaTask* self, const atn::Vector3& pos) {
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = self->mRot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &fwd, &fwd);
    atn::Vector3 d(pos - self->mPos);
    return PSVECDotProduct(&d, &fwd);
}

void Em202Spider::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    bool down = true;
    unsigned int flags = mStatus.m0C;
    if ((flags & 0x80000) != 0x80000 && (flags & 0x1000000) != 0x1000000) {
        down = false;
    }
    if (!down && m58C != 11) {
        if (m58C - 4U <= 1) {
            msg->m10 = lbl_807C7030;
        }
        if (msg->mData->m01 != 1) {
            int guard = 0;
            float dot = FrontDot_unk(this, msg->m1C);
            if (m58C - 4U <= 1) {
                if (dot < -(float)cos(90.0)) {
                    guard = 1;
                }
            } else if (dot > (float)cos(90.0)) {
                guard = 1;
            }
            if (guard) {
                msg->m10 = lbl_807C7030;
                msg->m48 = 1;
            }
        }
    }
    if (m58C == 12) {
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

// NONMATCHING: 99.75% - only the .rodata double of the int-to-float conversion (placed by the unwritten
// functions)
void Em202Spider::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m14FC += msg->m68;
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 0;
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        fn_800CB5C8(&m15A8, 15, 0);
        fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802829DC();
        return;
    }
    if (msg->mData != 0 && msg->mData->m04 == 0x1791) {
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 12;
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
        fn_8023A8B4(lbl_8087F3C0, &m158C, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        Effect_unk* mgr = lbl_8087F048;
        atn::Vector3 p = fn_801781B0();
        fn_80108F38(mgr, &p, 0x80);
    }
    switch (m14E4) {
    case 0:
        if (msg->mData->m01 == 1 || (float)m14FC >= 60.0f) {
            m14E8 = 1;
        }
        break;
    case 1:
        if (m58C != 4 && m58C != 5) {
            if (msg->mData->m01 == 1) {
                m1500 = 1;
            }
            m14FC = 0;
        }
        if (m14FC >= 120) {
            m1500 = 1;
        }
        break;
    }
}

// inferred: front direction of the character (name invented)
inline atn::Vector3 FrontDir_unk(CharaTask* self) {
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = self->mRot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &fwd, &fwd);
    return fwd;
}

// NONMATCHING: 98.97% - r3/r4 of the copy of the center (temporary of mPos + m5A4, __VecCopy family)
void Em202Spider::vf54() {
    if (m624.mSize != 0) {
        if (m58C == 5) {
            atn::Vector3 off = FrontDir_unk(this) * 20.0f;
            m624.mData[0].m04.mRadius = 1.5f * m5B0;
            m624.mData[0].m04.mCenter = mPos + m5A4 + off;
            m624.mData[0].m04.mCenter.y += 0.5f * m624.mData[0].m04.mRadius;
        } else {
            m624.mData[0].m04.mRadius = 1.5f * m5B0;
            m624.mData[0].m04.mCenter = mPos + m5A4;
            m624.mData[0].m04.mCenter.y += 0.5f * m624.mData[0].m04.mRadius;
        }
    }
}

void Em202Spider::vf7C(int arg) {
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 11;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}

void Em202Spider::fn_80281ED8() {
    if (m14B8 > 0) {
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 7;
        m1504 = m14B8;
        fn_8017039C(m14B8, 2);
        m1520 = 0;
        m151C = 1;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 2.0f;
        mMotion.fn_80097C08(0, 0x14, 0, 0, 1, 0.0f, 0.1f);
    }
}

void Em202Spider::fn_802829DC() {
    if (m58C == 5 && m1530 == 1) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
    fn_800CB5C8(&m15A8, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 10;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0(this);
}
