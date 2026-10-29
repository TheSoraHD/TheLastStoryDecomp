#include "game/Em315Coffin.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" CharaRes_unk* fn_80204E04(int type, int arg);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);

// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: game settings (as in game/CharaTask.cpp); names invented
struct CoffinBlock454_unk {
    int m00; // inferred: enabled
    unsigned char m04[0x18 - 0x04];
    float m18;
};
struct Settings_unk {
    CoffinBlock454_unk* fn_8013C434();
};
extern "C" Settings_unk* fn_800F52F8();
// inferred: effect manager methods (as in game/CharaTask.cpp)
struct Effect_unk {
    atn::Vector3 fn_80109950(CharaTask* chara); // inferred: a position for the character
};
Effect_unk* fn_8013A194();
extern "C" float fn_80011220(float x); // inferred: fabs (weak copy of another helper)
extern "C" int fn_80267B20(CharaTask* chara); // inferred: returns m55C
extern "C" int fn_80267B28(CharaTask* chara); // inferred: returns m560
extern "C" void* fn_802F0990(); // inferred: returns lbl_8087EFA8
extern "C" float fn_802F0988(void* p); // inferred: returns +0x3A4
extern "C" EffectMgr_unk* fn_800F7FA0(); // inferred: returns lbl_8087F3C0
extern "C" void fn_800F7FA8(EffectMgr_unk* mgr, void* owner, int arg); // inferred: fn_80232B7C(owner, arg)
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* res, int arg2, int arg3, Mtx* mtx, int arg5,
                            int arg6, int arg7, int arg8, int arg9, int arg10, float scale);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void fn_80276AE4(EffectMgr_unk* mgr, atn::EfpFile* efp, int arg, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int b, int c, float scale);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
void* fn_801404F8();
// inferred: result of the collision ray test (as in game/CharaTask.cpp)
struct CollResult_unk {
    CollResult_unk(); // 0x80140500
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x40];
};
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: color as four floats (as ColorF_unk in game/Em305VampireMp.cpp)
struct ColorF_unk {
    ColorF_unk() {}
    ColorF_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
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

// inferred: statics of the file (.sdata 0x8087DC40/0x8087DC44)
static float sZero_unk = 0.0f;
static float sOne_unk = 1.0f;

Em315Coffin::Em315Coffin(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m15B8(90), m162C(-1), m1630(0), m1634(0), m1638(sZero_unk, sZero_unk) {
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em315_00_00.csv\0");
    }
    m14B4.vf0C(path);
    mStatus.mParamB.m9C |= 0x10;
    m14D4 = *fn_80204E04(mType, 1);
    m1564.LoadRequest("/effect/ef_bs023a.efp");
    m1570.LoadRequest("/effect/ef_bs023b.eff");
    m157C.LoadRequest("/effect/ef_bs023d.efp");
    m1620.LoadRequest("/effect/ef_cm007.efp");
    m15C8.mId = 0x546;
    m15C8.mInterval = 180;
    m15D8.mId = 0x547;
    m15D8.mInterval = 180;
    m15E8.mId = 0x548;
    m15E8.mInterval = 90;
    mFlagB12 = 1;
}

Em315Coffin::~Em315Coffin() {}

int Em315Coffin::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1564.IsLoading() || m1570.IsLoading() || m157C.IsLoading()) {
        return 0;
    }
    if (m1620.IsLoading() != 0) {
        return 0;
    }
    m14D4.m30 = 5.0f * m5B0;
    mMotion.fn_80099E9C("hips", "em305", 0.5f);
    fn_80313AAC((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    mStatus.m18 |= 0xC010;
    mStatus.m18 |= 0x2000000;
    mStatus.m18 |= 0x4;
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    vfFC();
    return 1;
}

void Em315Coffin::fn_80313AAC(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "DrainSkillId") == 0) {
            m15C8.mId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "WideSkillId") == 0) {
            m15D8.mId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "DrainIntervalFrame") == 0) {
            m15C8.mInterval = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "WideIntervalFrame") == 0) {
            m15D8.mInterval = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ShadeRaySkillId") == 0) {
            m15E8.mId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ShadeRayIntervalFrame") == 0) {
            m15E8.mInterval = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ShadeRayFrame") == 0) {
            m15B8 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "BattleMoveRange") == 0) {
            m14D4.m30 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "CatchNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m162C, &csv);
        } else if (strcmp(key, "StableStart") == 0) {
            m1634 = atoi(csv.fn_8005B9CC());
            if (m1634 != 0) {
                m1658 = mMotion.fn_80092954("em315_02");
                if (m1658 != 0) {
                    ColorF_unk c(m1658->mColor);
                    c.a = 0.0f;
                    m1658->mColor = c.ToGX();
                }
            }
        }
    } while (csv.fn_8005BBF8());
}

void Em315Coffin::vf10() {
    m64 = &m14D4;
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        vf104();
        m15C8.Tick_unk();
        m15D8.Tick_unk();
        m15E8.Tick_unk();
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
    if (m58C != 9) {
        mRot.x *= 0.8f;
        if ((float)__fabs(mRot.x) < 0.03141593f) {
            mRot.x = 0.0f;
        }
    }
    fn_80315654();
}

void Em315Coffin::vf28() {}

void Em315Coffin::vf14() {
    fn_80149A30();
}

int Em315Coffin::vf10C(HitMsg_unk* msg) {
    msg->m10 = lbl_807C7030;
    return 1;
}

void Em315Coffin::vf40(HitInfo_unk* hit) {
    if (m58C == 10) {
        mC58.mC4->fn_8016EB48(0, 0, 0);
        m15E8.Reset_unk();
        fn_80315FE0();
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
    } else if (m58C != 9) {
        vf100(6);
    }
}

void Em315Coffin::vfFC() {
    fn_8016E970(2);
    if (m1634 != 0) {
        vf100(12);
    } else {
        vf100(6);
    }
}

void Em315Coffin::vf100(int state) {
    if (CharaTask::mFlag2) {
        return;
    }
    int prev = m58C;
    m14BC = 0;
    m14C0 = 0;
    m14C4 = 0;
    m14C8 = 0;
    mFlag31 = 0;
    mFlag30 = 0;
    mFlag29 = 0;
    mFlag28 = 0;
    mFlag27 = 1;
    m14D0 = 0;
    m58C = state;
    mC58.mMode = 23;
    switch (state) {
    case 6:
        mFlag30 = 1;
        break;
    case 7:
        mC58.m0C.fn_80128930(mC58.mC4, &mPos, 0);
        mC58.m0C.mRadius = m64->m30;
        mC58.m0C.fn_801255C8();
        mFlag30 = 1;
        break;
    case 8:
        mC58.mMode = 4;
        mFlag30 = 1;
        break;
    case 9:
        fn_805A4F1C(0x142, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        m14C8 = fn_800F8548(lbl_8087F048);
        m15D8.Reset_unk();
        m15C8.Reset_unk();
        m161C = mC58.mC4;
        m15F8 = m161C->mPos;
        m1604.x = 0.0f;
        m1604.y = 0.0f;
        m1604.z = 0.0f;
        break;
    case 10:
        fn_805A4F1C(0x145, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        m15E8.Reset_unk();
        break;
    case 11:
        mFlag30 = 1;
        break;
    case 12:
        fn_805A4F1C(0x2B, 0, 1, 0.0f, 0.0f);
        mFlag30 = 0;
        break;
    case 13:
        fn_805A4F1C(0x2B, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    case 2:
        if ((mStatus.m0C & 0x20) == 0x20 && prev != 2) {
            vf44();
        }
        fn_805A4F1C(0x2E, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    }
    if (m58C == 7 || m58C == 8) {
        mFC0 = mC58.mC4;
    } else {
        mFC0 = 0;
    }
}

// inferred: names invented
inline bool CanLinkTo_unk(Em315Coffin* self) {
    if (self->mC58.mC4->fn_8016F4D8(self)) {
        return true;
    }
    return self->mC58.mC4->fn_8016F530(0) != 0;
}

// NONMATCHING: 99.81% - FPR numbering of the inlined fade start/update (the original gives the lowest
// registers to the values loaded first); ~16 variants of the interpolation statements and parameters tried
void Em315Coffin::vf104() {
    m14C0++;
    if (m1634 != 0) {
        if (mC58.mC0 > 0) {
            m1634 = 0;
            m1638.FadeTo_unk(sOne_unk, 30.0f);
            vf100(13);
        }
        return;
    }
    if (m1638.IsActive_unk()) {
        m1638.Update_unk();
        if (m1658 != 0) {
            ColorF_unk c(m1658->mColor);
            c.a = m1638.mValue;
            m1658->mColor = c.ToGX();
        }
    }
    if (CanAct_unk() && m58C != 6 && m58C != 2 && m58C != 9 && m58C != 13) {
        CharaTask* target;
        if (mC58.mC0 == 0 || (target = mC58.mC4) == 0 || target->mC58.mC0 == 0) {
            vf100(6);
        } else if (target != (CharaTask*)mC58.mC8) {
            vf100(6);
        }
    }
    switch (m58C) {
    case 6:
        if (mC58.mC0 != 0 && mC58.mC4 != 0 && mC58.mC4->mC58.mC0 != 0) {
            vf100(7);
        }
        vf108();
        break;
    case 7:
        if (mFlag31) {
            atn::Vector3 d = mC58.mC4->mPos - mPos;
            float dist = fn_805F9920(&d);
            float r = m64->m30;
            if (dist < 1.2f * (1.2f * (r * r))) {
                vf100(8);
            } else {
                vf100(7);
            }
        }
        vf108();
        break;
    case 8: {
        atn::Vector3 d = mC58.mC4->mPos - mPos;
        float dist = fn_805F9920(&d);
        float r = m64->m30;
        if (dist > 2.0f * (2.0f * (r * r))) {
            vf100(7);
        } else if (m15D8.mCount <= 0 && m125C.mCount >= 2) {
            vf100(9);
        } else if (m15E8.mCount <= 0 && mC58.mC4->mFlag10 && mC58.mC4->mStatus.m0C == 0) {
            CharaTask* target = mC58.mC4;
            if (!(target->m55C == 6 && target->m560 == 0x7A)) {
                target->fn_80164168(m15B8, m15E8.mId);
                fn_805A4A1C((SpecialFlag_unk*)&m162C, mC58.mC4->mType == 0 ? -1 : mC58.mC4->m58);
            }
            mC58.mC4->fn_8016F530(this);
            vf100(10);
        } else if (m15C8.mCount <= 0) {
            if ((mC58.mC4->m125C.mCount < 2 || mC58.mC4->mStatus.m0C != 0) && CanLinkTo_unk(this)) {
                mC58.mC4->fn_8016F530(this);
                vf100(9);
            }
        }
        vf108();
        break;
    }
    case 9:
        if (mFlag31) {
            vf100(8);
        }
        vf108();
        break;
    case 10:
        if (mFlag31) {
            vf100(8);
        }
        vf108();
        break;
    case 11:
        vf108();
        break;
    case 12:
        vf108();
        break;
    case 13:
        if (mFlag31) {
            vf100(6);
        }
        vf108();
        break;
    case 2:
        fn_805A40B8();
        if (!mFlag31) {
            vf108();
        }
        break;
    default:
        vf100(6);
        break;
    }
}

// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: identity rotated by z, y and x (name invented, as in game/ChaserNpc.cpp)
inline void CoffinRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
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

// inferred: segment and radius of a capsule (as in gmk/Cannonball.cpp)
struct CoffinCapsule_unk {
    atn::Vector3 m00;
    atn::Vector3 m0C;
    float m18; // inferred: radius
};
extern "C" inline CoffinCapsule_unk* fn_80315644(atn::ColliCapsule* colli) {
    return (CoffinCapsule_unk*)&colli->m3C;
}
// inferred: sets the owner of the next effects (weak copy; name keeps the address)
extern "C" inline void fn_8031564C(EffectMgr_unk* mgr, int on) {
    *(int*)((char*)mgr + 0xC4) = on;
}

void Em315Coffin::vf108() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    int follow = 0;
    if (m55C == 2) {
        switch (m58C) {
        case 6:
            mFlag31 = 1;
            break;
        case 7: {
            mC58.m0C.fn_80126214();
            atn::Vector3 dir(*mC58.m0C.fn_80139F58());
            speed = dir.Length_unk();
            if (speed > 0.0001f) {
                rot = dir.GetNormalize().ToRotation_unk();
            }
            if (mC58.m0C.fn_8013C3F4()) {
                mFlag31 = 1;
            }
            break;
        }
        case 8:
            if (mC58.mC4 != 0) {
                follow = 1;
                atn::Vector3 dir = *mC58.mC4->fn_8013C38C() - mPos;
                speed = dir.Length_unk();
                if (speed < 0.0001f) {
                    speed = 0.0f;
                    rot = mRot;
                    break;
                }
                dir.Normalize_unk();
                if (speed < 0.8f * m64->m30) {
                    if (speed < 20.0f) {
                        speed = 1.0f;
                    } else {
                        speed = 0.5f;
                    }
                    rot = (-dir).ToRotation_unk();
                    break;
                }
                if (speed > 1.2f * m64->m30) {
                    speed = 1.0f;
                    rot = dir.ToRotation_unk();
                    break;
                }
                if (fn_800F52F8()->fn_8013C434()->m00 != 0) {
                    atn::Vector3 pos = fn_8013A194()->fn_80109950(this);
                    atn::Vector3 dir2 = pos - mPos;
                    speed = dir2.Length_unk();
                    if (speed > 0.0f) {
                        dir2.Normalize_unk();
                    }
                    if (speed < 3.0f) {
                        if (speed > 0.0f) {
                            rot = dir2.ToRotation_unk();
                        } else {
                            rot = mRot;
                        }
                        speed = 0.0f;
                    } else if (speed > 10.0f) {
                        if (atn::Vector3::fn_8013C394(dir, dir2).y >= 0.0f) {
                            rot = atn::Vector3::fn_8013C394(atn::Vector3(0.0f, 1.0f, 0.0f), dir).ToRotation_unk();
                        } else {
                            rot = atn::Vector3::fn_8013C394(dir, atn::Vector3(0.0f, 1.0f, 0.0f)).ToRotation_unk();
                        }
                    } else {
                        rot = dir2.ToRotation_unk();
                    }
                    rate = fn_800F52F8()->fn_8013C434()->m18;
                } else {
                    speed = 0.0f;
                    rot = dir.ToRotation_unk();
                }
            }
            break;
        case 9: {
            mMotion.SetMotionSpeed(0, GetStatus_unk()->fn_8012DB04());
            float frame = mMotion.GetMotionFrame(0);
            if (m14BC == 0) {
                atn::Vector3 hips = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
                m15F8 = *m161C->fn_8013C38C() - fn_8014052C() * 10.0f;
                CollResult_unk result;
                atn::Vector3 from = m15F8;
                from.y += 1.0f;
                atn::Vector3 to = from;
                to.y -= 20.0f;
                if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000000, 0, 0)) {
                    m15F8.y = result.m04.y;
                }
                if (m15F8.y - mPos.y >= 35.0f) {
                    mFlag31 = 1;
                    return;
                }
                atn::Vector3 d = m15F8 - hips;
                m1604 = d.ToRotation_unk();
                m1604.x -= 1.5707964f;
                float rate2 = GetStatus_unk()->fn_8012DB04();
                float k = fn_802F0988(fn_802F0990());
                float step = k * rate2;
                mPos.y += step;
                mRot.y = m1604.y;
                mRot.x += step * (atn::WrapAngle_unk(m1604.x - mRot.x) * 0.15f);
                if (frame >= 25.0f) {
                    m1610 = mPos;
                    m14BC++;
                }
                return;
            } else if (m14BC == 1) {
                if (frame >= 36.0f) {
                    float t = (frame - 35.0f) / 4.0f;
                    mPos = atn::Lerp_unk(m1610, m15F8, 1.0f < t ? 1.0f : t);
                }
                if (frame >= 39.0f) {
                    HitInfo_unk* bullet;
                    CoffinCapsule_unk* capsule = fn_80315644(&mColli);
                    bullet = fn_80219E6C(m15D8.mId);
                    fn_800FAB80(fn_8013A194(), this, bullet, m14C8, (atn::Vector3*)capsule, &lbl_807C7030, 0,
                                30, -1, -1, 0.0f, 1.0f);
                    m14BC++;
                }
                if (mMotion.fn_80139F2C(0) != 0x142) {
                    mFlag31 = 1;
                }
                return;
            } else {
                if (frame >= 43.0f) {
                    mMotion.SetMotionSpeed(0, 1.0f);
                } else if (frame >= 39.999f) {
                    mMotion.SetMotionSpeed(0, 0.1f);
                }
                if (frame >= mMotion.fn_80097D7C(0) || mMotion.fn_80139F2C(0) != 0x142) {
                    mFlag31 = 1;
                }
                return;
            }
        }
        case 10: {
            if (mC58.mC4 == 0) {
                break;
            }
            atn::Vector3 dir = *mC58.mC4->fn_8013C38C() - mPos;
            if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                dir.Normalize_unk();
            }
            rot = dir.ToRotation_unk();
            float frame = mMotion.GetMotionFrame(0);
            if (m14BC == 0) {
                if (frame >= mMotion.fn_80097D7C(0)) {
                    m14C0 = 0;
                    m14BC++;
                    mMotion.fn_80097C08(0, 0x146, 1, 1, 1, 0.0f, 3.6651914f);
                    fn_80315964();
                }
            } else if (m14BC == 1) {
                int state = fn_80267B20(mC58.mC4);
                int sub = fn_80267B28(mC58.mC4);
                if (m14C0 >= m15B8 || state != 6 || sub != 0x7A) {
                    m14C0 = 0;
                    m14BC++;
                    mMotion.fn_80097C08(0, 0x147, 0, 1, 1, 0.0f, 3.6651914f);
                    m15E8.Reset_unk();
                    m15C8.Reset_unk();
                    fn_80315FE0();
                    fn_80239DAC(fn_800F7FA0(), this, 0, 1);
                }
            } else if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag31 = 1;
                mC58.mC4->fn_8016F5EC(this);
            }
            break;
        }
        case 11: {
            mC58.m0C.fn_80126214();
            atn::Vector3 dir(*mC58.m0C.fn_80139F58());
            speed = dir.Length_unk();
            if (speed > 0.0001f) {
                rot = dir.GetNormalize().ToRotation_unk();
            }
            if (mC58.m0C.fn_8013C3F4()) {
                mFlag31 = 1;
            }
            break;
        }
        case 12:
            return;
        case 13: {
            float frame = mMotion.GetMotionFrame(0);
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag31 = 1;
            }
            break;
        }
        case 2: {
            float frame = mMotion.GetMotionFrame(0);
            if (frame >= mMotion.fn_80097D7C(0)) {
                fn_8031564C(fn_800F7FA0(), 1);
                fn_800F7FA8(fn_800F7FA0(), 0, 0);
                fn_80276AE4(fn_800F7FA0(), &m1620, 0, &mPos, &mRot, -1, -1, 1, 3.0f);
                fn_8031564C(fn_800F7FA0(), 0);
                fn_800C344C("SE_BTCOM_023", &mPos, 0, -1, 1.0f);
                mFlag31 = 1;
            }
            break;
        }
        }
    } else {
        fn_805A4254();
        return;
    }
    if (follow && fn_8013C478()) {
        fn_80140588(&rot, speed, m568 * rate);
    } else {
        fn_8013CB68(&rot, 0, speed, m568 * rate);
    }
}

// NONMATCHING: 99.46% - r3/r4 swapped in the Vector3 copies from temporaries (__VecCopy family, documented
// blocker; tried: named node vector, head = head + off, per-component add, head assigned apart)
void Em315Coffin::fn_80315654() {
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 center(mPos.x, mPos.y + m5B0, mPos.z);
    MtxPtr hips = mMotion.GetNodeMtx_unk("hips");
    if (hips != 0) {
        center = GetNodePos_unk(hips);
    }
    atn::Vector3 head = center;
    atn::Vector3 off(0.0f, m5B4 - 2.0f * m620, 0.0f);
    atn::Matrix3x4 m;
    CoffinRotZYX_unk(m, mRot);
    PSMTXMultVec(m.m, &off, &off);
    head += off;
    mColli.m3C = center;
    mColli.m48 = head;
    mColli.m54 = m620;
    mColli.m20 = 0x80;
}

// inferred: plays `efp` on `model` with default placement (name invented)
inline void CoffinEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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

// inferred: matrix helpers of the ray (names invented)
inline atn::Matrix3x4 CoffinTrans_unk(const atn::Vector3& v) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, v.x, v.y, v.z);
    return m;
}
inline void CoffinMulRot_unk(atn::Matrix3x4* m, const atn::Vector3& rot) {
    atn::Matrix3x4 r;
    CoffinRotZYX_unk(r, rot);
    atn::Matrix3x4 t;
    PSMTXConcat(m->m, r.m, t.m);
    *m = t;
}
inline void CoffinMulScale_unk(atn::Matrix3x4* m, const atn::Vector3& v) {
    atn::Matrix3x4 s;
    PSMTXScale(s.m, v.x, v.y, v.z);
    atn::Matrix3x4 t;
    PSMTXConcat(m->m, s.m, t.m);
    *m = t;
}

// inferred: the ray goes from the coffin to its drain target; the effect m1570 is a unit beam scaled by the
// distance / 20
// NONMATCHING: 97.16% - stack layout: the original keeps a 4 byte gap below the vectors of the start, the
// rotation temporaries 0x30 higher and the scale vector at 0x18 (~14 variants: helpers for the matrices by
// value/in place, local order of the vectors, scale/rotation declared early)
void Em315Coffin::fn_80315964() {
    if (fn_80239D58(lbl_8087F3C0, this, 0)) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
    }
    atn::Vector3 from = mColli.m48;
    atn::Vector3 to = mC58.mC4->mColli.m48;
    atn::Vector3 dir = to - from;
    float len = PSVECMag(&dir);
    if (!((float)__fabs(len) < 1.1920929e-7f)) {
        PSVECNormalize(&dir, &dir);
    }
    fn_80232B7C(this, 0);
    CoffinEffect_unk(&m1564, &mMotion);
    CoffinEffect_unk(&m157C, &mC58.mC4->mMotion);
    m1588 = CoffinTrans_unk(from);
    atn::Vector3 rot = dir.ToRotation_unk();
    CoffinMulRot_unk(&m1588, rot);
    atn::Vector3 scale(1.0f, 1.0f, len / 20.0f);
    CoffinMulScale_unk(&m1588, scale);
    fn_8023A680(lbl_8087F3C0, &m1570, -1, 5, &m1588.m, 0, 0, 0, 0, -1, 1, 1.0f);
}

void Em315Coffin::fn_80315FE0() {
    if (fn_80239D58(lbl_8087F3C0, this, 0)) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
    }
}

void Em315Coffin::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em315 Coffin[%d]\0", m58);
    void* group;
    if (m165C.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m165C.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    {
        void* sub = fn_8008937C(group, "DrainAttack");
        fn_800874C8(sub, "SkillId", &m15C8.mId, 0, 99999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m15C8.mInterval, 0, 99999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "WideAttack");
        fn_800874C8(sub, "SkillId", &m15D8.mId, 0, 99999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m15D8.mInterval, 0, 99999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "RayAttack");
        fn_800874C8(sub, "SkillId", &m15E8.mId, 0, 99999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m15E8.mInterval, 0, 99999, 1, 0, 0);
        fn_800874C8(sub, "Frame", &m15B8, 0, 99999, 1, 0, 0);
    }
}

float Em315Coffin::vfA0() {
    return 3.6651914f;
}
