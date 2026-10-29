#include "game/Em202SpiderBase.h"
#include "game/HitInfo_unk.h"
#include "game/CharaStatus.h"
#include "math/Vec.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
struct Effect_unk;
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4); // inferred: debug menu
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
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
extern "C" void fn_80097CCC(atn::Model* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004D314(void* world, int arg, const atn::Vector3* pos, const atn::Vector3* dir, unsigned int mask,
                           int arg5, int arg6, float radius); // inferred: sphere test
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
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: color of the spiders when they are down; defined here (its __sinit is the .ctors entry of this
// file) and read by game/Em202Spider.cpp, Em202ForestSpider, Em202_01_BSpider and Em308ParmCrab
// NONMATCHING (__sinit): 99.00% - only the .sdata2 offsets of 0.7f/0.2f/1.0f (constants of the unwritten
// functions)
extern "C" {
SpiderBaseColor_unk lbl_807C8398(0.7f, 0.2f, 0.2f, 1.0f);
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

// inferred: moves the position by `v`; the copy of the old position is dead in the original too
// (name invented)
inline void MovePos_unk(CharaTask* self, const atn::Vector3& v) {
    atn::Vector3 prev = self->mPos;
    self->mPos += v;
}

// inferred: translation of a node matrix (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: effect `efp` on the model (names invented, as in game/Em202Spider.cpp)
inline void PlayModelEffect_unk(CharaTask* self, int id, atn::EfpFile* efp) {
    fn_80232B7C(self, id);
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
    fn_8023A8B4(lbl_8087F3C0, efp, &self->mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: identity rotated by z, y and x (as QueenRotZYX_unk of game/Em208Queen.cpp; name invented)
inline void SpiderRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
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
    m15A4.LoadRequest("/effect/ef_uc001.efp");
    m1520.LoadRequest("/effect/ef_sk002d.efp");
    m14DC.LoadRequest("/effect/ef_uc007.efp");
    m15B0.LoadRequest("/effect/ef_sc003_01b.efp");
}

Em202SpiderBase::~Em202SpiderBase() {}

// NONMATCHING: 99.92% - push_back registers (known open case) and the pool of __allocate (lbl_80775A88)
int Em202SpiderBase::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m15A4.IsLoading() == 0 &&
        m1520.IsLoading() == 0 && m14DC.IsLoading() == 0 && m15BC.fn_80473F50() == 0) {
        if (m15BC.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m15BC.fn_80470580(), m15BC.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "NormalAttack") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m157C |= 2;
                    }
                } else if (strcmp(key, "Needle") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m157C |= 4;
                    }
                } else if (strcmp(key, "Move") == 0) {
                    if (strcmp(csv.fn_8005B9CC(), "on") == 0) {
                        m157C |= 1;
                    }
                } else if (strcmp(key, "BasePoints") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            // inferred: the base points are route places (id and position first)
                            m1570.push_back((SpiderBasePoint_unk*)place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "NeedleChargeTime") == 0) {
                    m14E8 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleChargeTime2") == 0) {
                    m14EC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackDist") == 0) {
                    m152C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleDist") == 0) {
                    m14F0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "EscapeCount") == 0) {
                    m1558 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "EscapeSpeed") == 0) {
                    m155C = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "GuardTheta") == 0) {
                    m1534 = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                    if ((float)__fabs(m1534 - 180.0f) < 1.1920929e-7f) {
                        m1534 = 179.0f;
                    }
                } else if (strcmp(key, "NeedleTheta") == 0) {
                    m14F8 = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "TackleID") == 0) {
                    m1530 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "NeedleID") == 0) {
                    m14F4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "NeedleChargeSE") == 0) {
                    sprintf(m14FC, "%s", csv.fn_8005B9CC());
                } else if (strcmp(key, "MoveSE") == 0) {
                    sprintf(m14B8, "%s", csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4218;
        mStatus.m18 |= 0x3800000;
        void* group = m15D4.Load("em308_ParmCrab");
        fn_80087994(group, "DebugDisp", &m15D8, 0, 0);
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.90% - r3/r4 of the copy of the bottom point of the capsule (__VecCopy family)
void Em202SpiderBase::vf10() {
    atn::Vector3 pos;
    mC58.mC8 = (int)mC58.mC4; // inferred: previous target (mC8 is an int in the shared header)
    if (m58C == 9) {
        m1454 = 0;
    } else {
        m1454 = 1;
        m1580 = mC58.mC4;
    }
    mC58.mC4 = m1580;
    m14B0++;
    if (mC58.mC0 == 0 || m1580 == 0) {
        fn_8016E970(3);
        if (m58C == 2) {
            vfEC();
        } else {
            fn_802847CC();
        }
    } else {
        switch (m58C) {
        case 7:
            if (fn_80158BB4()) {
                m58C = 6;
                if (m55C == 6 && m560 == 4) {
                    fn_8016E970(3);
                }
            }
            {
                float f = mMotion.mTracks[0].m08;
                if (f >= 17.0f && f <= 22.0f) {
                    fn_80144710();
                    int id = m590;
                    fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x3F3), id, 30, -1, 0.0f);
                }
            }
            break;
        case 8:
            fn_80285064();
            break;
        case 2:
            vfEC();
            break;
        case 9:
            fn_802857B4();
            break;
        case 10:
            fn_80285B7C();
            break;
        case 11:
            fn_80285FA0();
            break;
        case 12:
            fn_802868A0();
            break;
        case 13:
            if (m158C == 0) {
                m1590++;
                if (m1590 >= m159C) {
                    ResetAttack_unk();
                    m58C = 6;
                    fn_8016E970(3);
                    mColli.mFlags |= 1;
                } else {
                    MovePos_unk(this, m1548);
                    int fps = lbl_8087F0A8->mFrameRate;
                    m1548.y += -98.1f / (float)(fps * fps);
                }
            }
            break;
        case 14: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetAttack_unk();
                m58C = 6;
                fn_8016E970(3);
            }
            break;
        }
        case 15:
            if (m14B0 >= 450) {
                ResetAttack_unk();
                m58C = 6;
                fn_8016E970(3);
            }
            break;
        default:
            if (m55C - 6U > 1) {
                m55C = 3;
                fn_80170A20(m1580, 0, -1.0f);
            }
            fn_802847CC();
            fn_80284CC8();
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    pos = mPos;
    pos.y -= 2.0f;
    if (m58C == 8) {
        atn::Vector3 off = FrontDir_unk(this) * 10.0f;
        pos += off;
        m620 = m5B0;
        m614 = pos;
        m614.y += m620;
    }
    atn::Vector3 top = pos;
    top.y = mPos.y + m5B0;
    mColli.m3C = pos;
    mColli.m48 = top;
    mColli.m54 = m5B0;
}

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
            rot = dir.GetNormalize().ToRotation_unk();
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

// NONMATCHING: 99.91% - only the .sdata2 offsets (constants of the unwritten functions)
void Em202SpiderBase::fn_80284CC8() {
    m1594--;
    if (m1554 >= m1558 && !m1570.empty()) {
        fn_802878EC(m1580);
    }
    if (m1580 != m1584) {
        fn_8016E970(3);
        m1594 = m14F4 != 0 ? m14F4->m68 : 90;
        if (fn_80287400(m1580, 6) != 0) {
            return;
        }
    }
    atn::Vector3 d = m1580->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    if (m1594 < 0) {
        fn_8016E970(3);
        if (dist <= m14F0) {
            if (fn_80287400(m1580, 8) == 0) {
                fn_8028702C(m1580);
            }
        } else {
            m1594 = m14F4 != 0 ? m14F4->m68 : 90;
        }
        return;
    }
    atn::Vector3 down(0.0f, -3.0f, 0.0f);
    if (fn_8004D314(lbl_8087EE98, 0, &mPos, &down, 0x80000000, 0, 0, 3.0f) == 0) {
        return;
    }
    float reach = 30.0f;
    if (m1530 != 0) {
        reach = m1530->m40;
    }
    if (dist < reach) {
        fn_8016E970(3);
        if (fn_80287400(m1580, 11) == 0) {
            ResetAttack_unk();
            m58C = 11;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            m1590 = m1530 != 0 ? m1530->m68 : 60;
            m1584 = m1580;
        }
    } else if (dist < m152C) {
        fn_8016E970(3);
        if (fn_80287400(m1580, 10) == 0) {
            ResetAttack_unk();
            m58C = 10;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            m1584 = m1580;
        }
    }
}

// NONMATCHING: 99.88% - f8/f9 of the difference and of m14F8 in the yaw limit (8 variants)
// inferred: state 8 (needle attack: charge for m14E8 frames, then shoots m14EC needles from the hips to
// the target, the yaw clamped to m14F8)
void Em202SpiderBase::fn_80285064() {
    switch (m158C) {
    case 0: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m158C = 1;
            PlayModelEffect_unk(this, 0, &m1520);
            m151C = fn_800C344C(m14FC, &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x143, 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    }
    case 1:
        m1598++;
        if (m1598 >= m14E8) {
            m158C = 2;
            m1598 = 0;
            fn_8016E970(6);
            m560 = 4;
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            fn_800CB5C8(&m151C, 15, 0);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
        }
        break;
    case 2: {
        float f = mMotion.mTracks[0].m08;
        if (12.5f < f && f < 13.5f) {
            m1598++;
            if (m1598 < m14EC) {
                mMotion.mTracks[0].m08 = 12.0f;
            }
        } else if (fn_80158BB4()) {
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
        } else {
            float f2 = mMotion.mTracks[0].m08;
            if (f2 >= 7.0f && f2 < 8.0f) {
                atn::Vector3 rot = (m1580->fn_80148B0C(0)->m04.mCenter - mPos).ToRotationZ_unk();
                float dy = rot.y - mRot.y;
                float limit = m14F8;
                if (dy >= limit * 0.5f) {
                    rot.y = limit * 0.5f;
                } else if (dy <= -limit * 0.5f) {
                    rot.y = -limit * 0.5f;
                }
                atn::Matrix3x4 m;
                SpiderRotZYX_unk(m, rot);
                atn::Model* model = &mMotion;
                atn::Vector3 pos = GetNodePos_unk(model->GetNodeMtx_unk("hips"));
                atn::Vector3 dir(0.0f, 0.0f, 3.0f);
                PSMTXMultVec(m.m, &dir, &dir);
                if (m14F4 != 0) {
                    fn_800F8574(lbl_8087F048, this, m14F4, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
                }
            }
        }
        break;
    }
    }
}

// NONMATCHING: 99.17% - only the .rodata/.sdata2 offsets (constants of the unwritten functions)
void Em202SpiderBase::fn_802857B4() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        switch (m1538) {
        case 6:
            ResetAttack_unk();
            m58C = 6;
            fn_8016E970(3);
            break;
        case 7:
            fn_80286CB0(m1588);
            break;
        case 8:
            fn_80286CB0(m1588);
            break;
        case 10:
            if (fn_80287400(m1588, 10) == 0) {
                ResetAttack_unk();
                m58C = 10;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                m1584 = m1580;
            }
            break;
        case 11:
            if (fn_80287400(m1580, 11) == 0) {
                ResetAttack_unk();
                m58C = 11;
                fn_80097CCC(&mMotion, 0, 0.0f);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
                m1590 = m1530 != 0 ? m1530->m68 : 60;
                m1584 = m1580;
            }
            break;
        case 12:
            fn_802878EC(m1580);
            break;
        }
    } else {
        float f = mMotion.mTracks[0].m08;
        if (5.0f <= f && f <= 16.0f) {
            m153C += 2;
            float d = atn::WrapAngle_unk(m1544 - m1540);
            mRot.y = atn::WrapAngle_unk(m1540 + m153C * d / 12.0f);
        }
    }
}

// NONMATCHING: 99.85% - only the .sdata2 offsets (constants of the unwritten functions)
// inferred: the direction to the target is turned into a rotation that is not used; the test of the
// turn is repeated (as in the original)
void Em202SpiderBase::fn_80285B7C() {
    atn::Vector3 rot;
    atn::Vector3 d = m1580->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    rot = d.ToRotation_unk();
    float reach = 30.0f;
    if (m1530 != 0) {
        reach = m1530->m40;
    }
    if (m1554 >= m1558 && !m1570.empty()) {
        fn_802878EC(m1580);
    }
    if (dist < reach) {
        fn_8016E970(3);
        if (fn_80287400(m1580, 11) == 0 && fn_80287400(m1580, 11) == 0) {
            ResetAttack_unk();
            m58C = 11;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            m1590 = m1530 != 0 ? m1530->m68 : 60;
            m1584 = m1580;
        }
    } else if (dist > m152C) {
        fn_8016E970(3);
        if (fn_80287400(m1580, 8) == 0) {
            fn_8028702C(m1580);
        }
    } else {
        if (m55C - 6U > 1) {
            m55C = 3;
            fn_80170A20(m1580, 0, -1.0f);
        }
        fn_802847CC();
    }
}

// inferred: state 11 (keeps the reach distance from the target m1580 for m1590 frames: backs off when it is
// too near, stays in the ring 0.8-1.2 of the reach and follows when it is too far)
void Em202SpiderBase::fn_80285FA0() {
    atn::Vector3 rot;
    atn::Vector3 d = m1580->mPos - mPos;
    float dist = sqrt(d.x * d.x + d.z * d.z);
    float rate = 1.0f;
    rot = d.ToRotation_unk();
    if (m1554 >= m1558 && !m1570.empty()) {
        fn_802878EC(m1580);
        return;
    }
    if (dist > m152C) {
        fn_8016E970(3);
        if (fn_80287400(m1580, 8) == 0) {
            fn_8028702C(m1580);
        }
        return;
    }
    if (dist > 0.8f * m152C) {
        fn_8016E970(3);
        if (fn_80287400(m1580, 10) == 0 && fn_80287400(m1580, 10) == 0) {
            ResetAttack_unk();
            m58C = 10;
            fn_80097CCC(&mMotion, 0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
            m1584 = m1580;
        }
        return;
    }
    if (m1590 < 0) {
        fn_8016E970(3);
        if (fn_80287400(m1580, 7) == 0) {
            fn_80286CB0(m1580);
        }
        return;
    }
    float reach = 30.0f;
    if (m1530 != 0) {
        reach = m1530->m40;
    }
    if (dist >= 0.8f * reach && dist <= 1.2f * reach) {
        m158C = 1;
    } else if (dist < 0.8f * reach) {
        m158C = 0;
    } else if (dist > 1.2f * reach) {
        m158C = 2;
    }
    if (m158C == 0) {
        dist = reach * (1.0f - dist / reach);
        rot = dist <= 0.0f ? mRot : d.GetNormalize().ToRotation_unk();
        rot.y -= 3.1415927f;
    } else if (m158C == 1) {
        dist = 0.0f;
    } else if (m158C == 2) {
        dist *= dist / reach;
    }
    CharaTask* target = m1580;
    mRot.y = (target->mPos - mPos).ToRotationZ_unk().y;
    fn_801426A4(&rot, dist, m568 * rate);
    m1590--;
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
        int place = m1560->mPlace;
        fn_8017039C(place, 0);
    }
    fn_802847CC();
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
        atn::Vector3 rot = (pos - mPos).GetNormalize().ToRotation_unk();
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
        atn::Vector3 rot = (pos - mPos).GetNormalize().ToRotation_unk();
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

// NONMATCHING: 99.78% - stack slots of the two differences inside the loop (d at 0x68 instead of 0x50); 8
// variants
// inferred: runs to the farthest base point (the points behind the spider count double)
void Em202SpiderBase::fn_802878EC(CharaTask* target) {
    if (fn_80287400(target, 12) != 0) {
        return;
    }
    ResetAttack_unk();
    m58C = 12;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    m1584 = m1580;
    m1560 = 0;
    float best = 0.0f;
    int place = -1;
    for (unsigned int i = 0; i < m1570.size(); i++) {
        atn::Vector3 d = m1570[i]->mPos - mPos;
        float dist = sqrt(d.x * d.x + d.z * d.z);
        atn::Vector3 dir = (m1570[i]->mPos - mPos).GetNormalize();
        atn::Vector3 rot = dir.ToRotation_unk();
        float diff = atn::WrapAngle_unk(rot.y - mRot.y);
        if (diff > 1.5707964f || diff < -1.5707964f) {
            dist *= 2.0f;
        }
        if (dist > best) {
            best = dist;
            place = m1570[i]->mPlace;
            m1564 = m1570[i]->mPos;
            m1560 = m1570[i];
        }
    }
    if (m1560 != 0) {
        fn_8017039C(place, 8);
    } else {
        ResetAttack_unk();
        m58C = 6;
        fn_8016E970(3);
    }
}
