#include "game/Em309GhoulSimple.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/new.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

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

float Em309GhoulSimple::vf90() {
    return 3.0f;
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
