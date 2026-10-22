#include "game/Em309GhoulSimple.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "std/new.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
// inferred: per-player weights of an effect (floats at +0x128); names invented
struct GhoulWeights_unk {
    unsigned char m00[0x128];
    float mWeight[4];
};
extern "C" GhoulWeights_unk* fn_80103F60(Effect_unk* mgr, CharaTask* owner);
extern "C" int fn_80108378(GhoulWeights_unk* w, CharaTask* chara); // inferred: index of `chara`

inline GhoulStr_unk& GhoulStr_unk::operator=(const char* str) {
    if (str != mStr) {
        memcpy(mStr, str, strlen(str) + 1);
    }
    return *this;
}

// inferred: name invented
inline bool Em309GhoulSimple::IsDownState_unk() {
    bool ok = true;
    if ((mStatus.m0C & 0x80000) != 0x80000 && (mStatus.m0C & 0x1000000) != 0x1000000) {
        ok = false;
    }
    return ok;
}

// NONMATCHING: ~85% - the loop of the GhoulPair_unk constructors starts at the first element in the
// original (ours peels it) and the zero of m15BC/m1648-m1654 is kept in r30
Em309GhoulSimple::Em309GhoulSimple(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D8(0.0f, 0.0f, 0.0f), m14E4(40.0f), m14E8(0), m14EC(0), m14F0(0),
      m14F4(0), m14F8(0), m14FC(0), m1500(0), m1504(0), m1508(0), m150C(0), m1510(0), m1514(0), m1518(0),
      m151C(0), m1520(0), m1524(0), m1528(90), m1538(10), m153C(3.0f), m1540(0.0f), m1544(0.0f), m1548(0.0f),
      m154C(0.0f), m1550(0.0f), m1554(0.0f), m1570(0), m1574(0), m15B8(-1.0f), m1648(0),
      m164C(0), m1654(0) {
    m15BC.mStr[0] = '\0';
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em309_01_00.csv\0");
    }
    m14B4.vf0C(path);
    m152C.fn_8023780C("/effect/ef_sk030b.efp");
    m15BC = "/effect/ef_sk036e.efp";
    m14E8 = 0x64A;
    m14F0 = 90;
    m14F8 = 0x64B;
    m1500 = 150;
    m1508 = 0x64C;
    m1510 = 120;
    m150C = 30;
    m1518 = 0x64D;
    m1520 = 0;
    mFlag22 = 1;
}

Em309GhoulSimple::~Em309GhoulSimple() {}

int Em309GhoulSimple::vf0C() {
    if (m164C == 0) {
        if (m14B4.fn_80473F50() != 0) {
            return 0;
        }
        fn_80306078((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
        if (m15B8 > 0.0f && m1574 != 0) {
            m163C.fn_8023780C(m15BC.mStr);
        }
        m164C++;
    } else {
        if (fn_8013655C() != 0) {
            return 0;
        }
        if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
            return 0;
        }
        if (m152C.fn_80237874() != 0) {
            return 0;
        }
        if (m163C.fn_80237874() != 0) {
            return 0;
        }
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        if (m15B8 > 0.0f && m1574 != 0) {
            m1648 = 1;
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4011;
        mStatus.m18 |= 0x3000000;
        mStatus.m18 |= 0x8004;
        vfFC();
        return 1;
    }
    return 0;
}

void Em309GhoulSimple::fn_80306078(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "AttackNormalSkillId") == 0) {
            m14E8 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackNormalCoolTime") == 0) {
            m14F0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLongSkillId") == 0) {
            m1508 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLongChargeTime") == 0) {
            m150C = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLonglCoolTime") == 0) {
            m1510 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackRollingSkillId") == 0) {
            m1518 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackDistance") == 0) {
            m14E4 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "WaitFrame") == 0) {
            m1528 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "SometimeAttackLong") == 0) {
            m1538 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HeadAttackStartFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1558, &csv);
        } else if (strcmp(key, "HeadAttackNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1560, &csv);
        } else if (strcmp(key, "SummonHp") == 0) {
            m15B8 = fn_800DC288(csv.fn_8005B9CC()) / 100.0f;
        } else if (strcmp(key, "SummonEnemy") == 0) {
            while (true) {
                int id = atoi(csv.fn_8005B9CC());
                if (id == 0) {
                    break;
                }
                new (&m1578[m1574]) GhoulPair_unk(id, 0);
                m1574++;
            }
        } else if (strcmp(key, "SummonWarpPoint") == 0) {
            GhoulPair_unk* p = m1578;
            for (unsigned int i = 0; i < m1574; p++, i++) {
                p->m04 = atoi(csv.fn_8005B9CC());
            }
        } else if (strcmp(key, "SummonNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1568, &csv);
        } else if (strcmp(key, "SummonEfp") == 0) {
            m15BC = csv.fn_8005B9CC();
        } else if (strcmp(key, "Speed") == 0) {
            m153C = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

// NONMATCHING: 99.57% - volatile registers of the inlined CanAct_unk (r5/r0/r4 in the original, r5/r4/r6 here;
// the same inline matches in Em205Ogre/Em315Coffin); 9 variants
void Em309GhoulSimple::vf10() {
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        vf104();
    }
    if (m14F4 > 0) {
        m14F4--;
    }
    if (m1504 > 0) {
        m1504--;
    }
    if (m1514 > 0) {
        m1514--;
    }
    if (m1524 > 0) {
        m1524--;
    }
    fn_8014C540();
    atn::Vector3 pos = mPos;
    atn::Vector3 rot = mRot;
    if (m58C == 12) {
        if (m14BC == 0) {
            float frame = mMotion.mTracks[0].m08;
            mPos.y += m5B0 * (frame / mMotion.fn_80097D7C(0));
        } else {
            mPos.y += m5B0;
            m154C += -0.5235988f;
            mRot.x = m154C;
        }
    }
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
    if (m58C == 12) {
        mPos = pos;
        mRot = rot;
    }
    if (m1570 != 0) {
        GhoulWeights_unk* w = fn_80103F60(lbl_8087F048, this);
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            int i = fn_80108378(w, p);
            w->mWeight[i] = p->mType == 0 ? 1.0f : 0.3f;
        }
    }
}

void Em309GhoulSimple::vf14() {
    fn_80149A30();
}

int Em309GhoulSimple::vf10C(HitMsg_unk* msg) {
    if (mFlag29) {
        return 0;
    }
    if (m58C == 13) {
        if (msg->mData != 0 && msg->mData->m04 == 0x658) {
            msg->m10 *= 0.0f;
            msg->m0C |= 0x80;
            return 1;
        }
        return 0;
    }
    msg->m10 *= 0.0f;
    if (!IsDownState_unk()) {
        msg->m0C |= 8;
    }
    return 1;
}

void Em309GhoulSimple::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 99.64% - r4/r5 of the copy of the neck position (__VecCopy family, documented blocker)
void Em309GhoulSimple::vf20() {
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 top = m614;
    atn::Vector3 head;
    int neck = mMotion.fn_80092814("neck", 0);
    if (neck > -1) {
        head = GetNodePos_unk(mMotion.mNodeMtx[neck]);
    } else {
        head = top;
        head.y += m5B4 - 2.0f * m620;
    }
    float r = m620;
    mColli.m3C = top;
    mColli.m48 = head;
    mColli.m54 = r;

}

void Em309GhoulSimple::vf54() {
    if (m624.mSize < 2) {
        return;
    }
    m624.mData[0].m04.mRadius = mColli.m54;
    m624.mData[0].m04.mCenter = mColli.m3C;
    m624.mData[1].m04.mRadius = 0.5f * mColli.m54;
    m624.mData[1].m04.mCenter = mColli.m48;
    m624.mData[1].m04.mCenter.y += 0.5f * mColli.m54;
}

atn::Vector3 Em309GhoulSimple::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (fn_805A49E4((SpecialFlag_unk*)&m1558) > 0 && m624.mSize >= 2) {
        result = m624.mData[1].m04.mCenter;
    } else {
        result = mPos;
    }
    return result;
}

// NONMATCHING: 99.92% - only the offsets in the string pool (strings of functions not written yet)
void Em309GhoulSimple::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em309 GhoulLight[%d]\0", m58);
    void* group;
    if (m1650.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m1650.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    {
        void* sub = fn_8008937C(group, "AttackNormal");
        fn_800874C8(sub, "SkillID", &m14E8, 0, 999999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m14F0, 0, 999999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "AttackLong");
        fn_800874C8(sub, "SkillID", &m14E8, 0, 999999, 1, 0, 0);
        fn_800874C8(sub, "ChargeTime", &m14EC, 0, 999999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m14F0, 0, 999999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "AttackRolling");
        fn_800874C8(sub, "SkillID", &m14E8, 0, 999999, 1, 0, 0);
    }
    fn_8008771C(group, "AttackDistance", &m14E4, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_800874C8(group, "WaitFrame", &m1528, 0, 999, 1, 0, 0);
    fn_800874C8(group, "SometimeAttackLong", &m1538, 0, 999, 1, 0, 0);
}

float Em309GhoulSimple::vf90() {
    return 3.0f;
}

