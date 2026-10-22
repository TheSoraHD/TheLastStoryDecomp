#include "game/Em306GurgBeast.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaStatus.h"
#include "game/EnemyManager.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/CharaC58_unk.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);
extern "C" void fn_80102824(Effect_unk* mgr, CharaTask* chara, PlayerTask* leader);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: the rider's hit handler is called with a second argument (CharaTask::vf3C takes one)
struct Rider_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C(HitInfo_unk* hit, int arg);
};

Em306GurgBeast::Em306GurgBeast(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14D8(0.0f, 0.0f, 0.0f), m14EC(0x640), m14F0(30), m1518(0),
      m1520(0), m1524(0), m1528(0), m1530(0), m1534(0), m1538(0), m153C(0), m1540(0), m1544(0), m1548(0),
      m154C(0), m1550(-1), m1554(3), m1558(3), m155C(300), m1560(0.0f) {
    m14E8 = 0;
    m14E4 = 0;
    m152C = 0;
    const char* s = strstr(info->m2C, "ride=");
    if (s != 0) {
        m14E4 = fn_800DC12C(s + 5);
    }
    if (strstr(info->m2C, "rideplayer") != 0) {
        m1524 = 1;
    }
    if (m1524 != 0 && strstr(info->m2C, "reride") != 0) {
        m1528 = 1;
    }
    char path[0x100];
    char name[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em306_00_00.csv\0");
    }
    m14B4.vf0C(path);
}

Em306GurgBeast::~Em306GurgBeast() {}

int Em306GurgBeast::vf0C() {
    int ready = 1;
    if (fn_8013655C() != 0) {
        ready = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        ready = 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        ready = 0;
    }
    if (ready) {
        fn_802EE9A8((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        m14E8 = EnemyManager::sInstance->Find_unk(m14E4);
        if (m14E8 != 0) {
            MtxPtr seat = mMotion.GetNodeMtx_unk("seatpoint");
            if (seat != 0) {
                m14E8->mF1C = (int)seat;
            }
            if (m1524 != 0) {
                m1548 = 1;
            }
        }
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        vfFC();
    }
    return ready;
}

void Em306GurgBeast::fn_802EE9A8(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "SkillId") == 0) {
            m14EC = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "WaitFrame") == 0) {
            m14F0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HomePoint") == 0) {
            RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
            if (table != 0) {
                const char* s = csv.fn_8005B9CC();
                while (s != 0 && *s != '\0' && m14F4.size() < 8) {
                    RoutePlace_unk* place = table->FindPlace_unk(atoi(s));
                    if (place != 0) {
                        m14F4.push_back(place);
                    }
                    s = csv.fn_8005B9CC();
                }
            }
        }
    } while (csv.fn_8005BBF8());
}

void Em306GurgBeast::vf14() {
    fn_80149A30();
    if (m14E8 != 0) {
        m14E8->fn_80149A30();
    }
}

int Em306GurgBeast::vf10C(HitMsg_unk* msg) {
    msg->m10 *= 0.0f;
    if (m1548 != 0) {
        if (m14E8 != 0) {
            msg->m04 = m14E8;
            ((Rider_unk*)m14E8)->vf3C((HitInfo_unk*)msg, 1);
            m1558--;
        } else if (m152C != 0) {
            msg->m04 = m152C;
            ((Rider_unk*)m152C)->vf3C((HitInfo_unk*)msg, 1);
            if (m154C != 0) {
                m1554--;
            }
        }
        return 0;
    }
    return 1;
}

void Em306GurgBeast::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
    }
}

void Em306GurgBeast::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

void Em306GurgBeast::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em306 GurgBeast[%d]", m58);
    void* group;
    if (m151C.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m151C.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
}

// inferred: names invented
inline float Max_unk(float a, float b) {
    return a > b ? a : b;
}

// inferred: walk motion of the beast: turning (0x144) when the movement is more than 45 degrees off the facing,
// running (0x140, speed from m1560) or walking (0x142)
void Em306GurgBeast::fn_802F0F60() {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    if (m1560 > 0.3f) {
        atn::Vector3 move(m574.x, 0.0f, m574.z);
        PSVECNormalize(&move, &move);
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &front, &front);
        float dot = PSVECDotProduct(&move, &front);
        if (dot < (float)cos(0.7853981852531433)) {
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, 0x140, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C =
                1.3f < Max_unk(0.5f, 0.5f * m1560) ? 1.3f : Max_unk(0.5f, 0.5f * m1560);
        }
    } else {
        mMotion.fn_80097C08(0, 0x142, 1, 0, 1, 0.0f, 0.1f);
    }
    if (--m1544 < 0) {
        m1544 = 30;
        m1540 = fn_800F8548(lbl_8087F048);
    }
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetSeatPos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 96.18% - block order of the state 17 test and r3/r4/r5 of the seat copy (~6 forms)
// inferred: keeps the rider m152C on the seat m1530 with the beast's motion frame (motion 0x1F0, 0x1F1 when
// m1560 >= 4)
void Em306GurgBeast::fn_802F1120() {
    if (m152C == 0 || m1530 == 0) {
        return;
    }
    if (m58C != 17) {
    int motion = 0x1F0;
    if (m154C != 0 && m1560 >= 4.0f) {
        motion = 0x1F1;
    }
    m152C->mMotion.m34C = 1;
    m152C->mMotion.mTracks[0].m20 = 1.0f;
    m152C->mMotion.mTracks[0].m0C = 0.0f;
    m152C->mMotion.mTracks[0].m08 = mMotion.mTracks[0].m08;
    m152C->mMotion.fn_80097C08(0, motion, 1, 0, 1, 0.0f, 0.1f);
    m152C->mF1C = m1530;
    m152C->mPos = GetSeatPos_unk((MtxPtr)m1530);
    m152C->mRot = mRot;
    m152C->fn_80145334();
    }
}

void Em306GurgBeast::fn_802F1240(CharaTask* rider) {
    rider->fn_8017AC3C((int)this);
    vf100(15);
    m1550 = -1;
    m1554 = 3;
    m14C0 = 0;
    m155C = 300;
    if (rider->mFlag31) {
        rider->fn_801539E0();
    }
    if (rider->mFlag6) {
        rider->fn_80155DAC(0);
    }
    if (rider->m139C != 0) {
        rider->fn_8017A33C(0, 1);
    }
    if (rider->m1208 != 0) {
        rider->fn_801750FC();
    }
    rider->fn_80164DCC(0);
}

// NONMATCHING: 97.72% - only the place of the load of the rider's rotation before PSMTXRotRad (~8 forms)
// inferred: throws the rider m152C off to the side; with `restart` the beast goes to state 14
void Em306GurgBeast::fn_802F1320(int restart) {
    if (m152C == 0) {
        return;
    }
    PlayerTask* head;
    if (lbl_8087F048 != 0 && lbl_8087F8A0 != 0 && (head = lbl_8087F8A0->mHead) != 0) {
        fn_80102824(lbl_8087F048, this, head);
    }
    if ((m152C->mStatus.m0C & 0x20) != 0x20) {
        m152C->fn_8016E970(1);
    }
    m152C->fn_801446F0();
    m152C->mF1C = 0;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx r;
    Mtx m;
    PSMTXRotRad(m, 'y', m152C->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    PSMTXRotRad(r, 'y', -1.5707964f);
    PSMTXMultVec(r, &dir, &dir);
    dir *= 5.0f;
    m152C->m574 = dir;
    m152C->fn_8017AC3C(0);
    m152C = 0;
    if (restart != 0) {
        vf100(14);
        m1550 = 300;
    }
}
