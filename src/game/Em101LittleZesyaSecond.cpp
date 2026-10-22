#include "game/Em101LittleZesyaSecond.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: debug menu entry (as in game/Em101ElderZesya.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: word at 0x1C of the running control (class unknown)
struct Control1C_unk {
    unsigned char m00[0x1C];
    int m1C;
};
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// NONMATCHING: 99.81% - r26/r27 of the second std::string and r25/r26 of the strlen of the parameters
// (same as Em101LittleZesya / Em101ElderZesya / Em201ZanGurg)
Em101LittleZesyaSecond::Em101LittleZesyaSecond(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14CC(0), m14D0(2), m14D4(20),
      m14D8(90), m14E8(0), m14EC(0.0f), m14F0(0.0f), m14F4(0.0f), m14F8(0.0f), m14FC(0.0f, 0.0f, 0.0f),
      m1508(0.0f, 0.0f, 0.0f), m1514(0.0f, 0.0f, 0.0f), m1548(0), m154C(0),
      m1554(0) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em101_02_00.csv");
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
    void* group = m1550.Load("LittleZesyaSecond");
    fn_80087994(group, "DebugDisp", &m1554, 0, 0);
    m1520.fn_8023780C("ef_bs007a.efp");
    m152C.fn_8023780C("ef_sc019a.efp");
    m1538.fn_8023780C("ef_sk030b2.efp");
    mFlagB5 = 1;
    mFlagB11 = 1;
}

Em101LittleZesyaSecond::~Em101LittleZesyaSecond() {}

int Em101LittleZesyaSecond::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1520.fn_80237874() == 0 &&
        m152C.fn_80237874() == 0 && m1538.fn_80237874() == 0 && m14B0.fn_80473F50() == 0) {
        m58C = 0;
        fn_8016E970(2);
        fn_80260324();
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x1C21D;
        mStatus.m18 |= 0x3800000;
        mStatus.fn_80134674(2);
        mStatus.fn_80134674(3);
        return 1;
    }
    return 0;
}

void Em101LittleZesyaSecond::fn_80260324() {
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == ';' || *key == '\0') {
            continue;
        }
        if (strcmp(key, "NormalAttackID") == 0) {
            m14C8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WarpCenterPoint") == 0 && lbl_8087F430 != 0) {
            RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(atoi(csv.fn_8005B9CC()));
            if (place != 0) {
                m14FC = place->mPos;
            }
        }
    } while (csv.fn_8005BBF8());
}

void Em101LittleZesyaSecond::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    switch (m58C) {
    case 6:
    case 7:
    case 1:
    case 11:
        msg->m10 *= 0.0f;
        break;
    case 8:
        return;
    }
    mStatus.fn_80134674(2);
    mStatus.fn_80134674(3);
    fn_80151448(msg);
    vf40(hit);
}

void Em101LittleZesyaSecond::vf40(HitInfo_unk* hit) {
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8 * 0.5f, -1);
        case 20:
        reset:
            m58C = 0;
            m14BC = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m14C4 = 0;
            mFlag5 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 102, 1);
            break;
        }
    }
    m1548++;
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802620A8();
    }
}

void Em101LittleZesyaSecond::fn_80261948() {
    switch (m55C) {
    case 6:
        if (m560 == 10) {
            m598 = 0;
            m59D = 0;
            m59C = 0;
            m59F = 1;
            m590 = fn_800F8548(lbl_8087F048);
            SetHit_unk(fn_80219E6C(0xE5));
            ((Control1C_unk*)mControl.get())->m1C = 1;
            m58C = 1;
            Field_unk* field = lbl_8087F430;
            fn_80370AE4(field, 0xF1, fn_80370A78(field, 0xF0) + 1);
        }
        break;
    case 7:
        mFC0 = 0;
        mFlag5 = 0;
        break;
    default:
        fn_8016E970(3);
        if (m58C == 0) {
            mFlag5 = 1;
        }
        mFC0 = m14B8;
        break;
    }
    fn_80139560();
}

void Em101LittleZesyaSecond::fn_802620A8() {
    if (m58C == 11) {
        m1514 = m1508;
    } else {
        m1514 = mPos;
    }
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14C4 = 0;
    mFlag5 = 0;
    fn_80239DAC(lbl_8087F3C0, this, 102, 1);
    m58C = 2;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 99.68% - slots of x/y of the negated front (stores z, x, y in the original; 9 variants:
// member order, constructor, locals, inline, IsDown_unk) and the .sdata2 floats of unwritten functions
int Em101LittleZesyaSecond::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (mFlag5) {
        if (!IsDown_unk()) {
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            Mtx m;
            float ry = mRot.y;
            PSMTXRotRad(m, 'y', ry);
            PSMTXMultVec(m, &front, &front);
            atn::Vector3 back(-front.x, -front.y, -front.z);
            if (PSVECDotProduct(dir, &back) > -0.5f) {
                return (m55C != 6) + 1;
            }
        }
    }
    return 0;
}

int Em101LittleZesyaSecond::vfA8(int kind) {
    if (kind == 1 && mFlag5) {
        bool down = mStatus.IsState_unk(0x80000) || mStatus.IsState_unk(0x1000000);
        if (!down) {
            return 3;
        }
    }
    return -1;
}
