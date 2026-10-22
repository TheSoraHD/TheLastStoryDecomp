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
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: color as four floats (as ColorF_unk in game/Em305VampireMp.cpp)
struct ColorF_unk {
    ColorF_unk() {}
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

// NONMATCHING: ~35% - the struct copy of the character resource is done in pairs of words in the original
// (lwz/lwz/stw/stw) and the statics of .sdata are read in another order
Em315Coffin::Em315Coffin(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m15B8(90), m162C(-1), m1630(0), m1634(0), m1638(0.0f), m163C(0.0f),
      m1640(1.0f), m1644(0) {
    m1648 = sZero_unk;
    float z = sZero_unk;
    m164C = z;
    m1650 = sZero_unk;
    m1654 = z - m1648;
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
    m64 = &m14D4;
    m1564.fn_8023780C("/effect/ef_bs023a.efp");
    m1570.fn_80237654("/effect/ef_bs023b.eff");
    m157C.fn_8023780C("/effect/ef_bs023d.efp");
    m1620.fn_8023780C("/effect/ef_cm007.efp");
    m15C8.mId = 0x546;
    m15C8.mInterval = 180;
    m15D8.mId = 0x547;
    m15D8.mInterval = 180;
    m15E8.mId = 0x548;
    m15E8.mInterval = 90;
    mFlagB19 = 1;
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
    if (m1564.fn_80237874() || m1570.fn_802376D0() || m157C.fn_80237874()) {
        return 0;
    }
    if (m1620.fn_80237874() != 0) {
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

// NONMATCHING: ~85% - the original hoists the 0x43300000 words of the int->float conversions of the
// material color out of the loop (one saved register less, other stack slots)
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
                    GXColor g = m1658->mColor;
                    ColorF_unk c;
                    c.r = g.r / 255.0f;
                    c.g = g.g / 255.0f;
                    c.b = g.b / 255.0f;
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

void Em315Coffin::fn_80315FE0() {
    if (fn_80239D58(lbl_8087F3C0, this, 0)) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
    }
}

void Em315Coffin::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em315 Coffin[%d]", m58);
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
