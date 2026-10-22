#include "game/Em312DarkServant.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/Em006DarkMaster.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int atoi(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;  // inferred: player manager
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

// NONMATCHING: ~40% - the original clears m14EC and m152C with constructed array loops (16-byte stride
// with a loop count computed from the addresses) and stores the defaults in another order
Em312DarkServant::Em312DarkServant(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14D8(1), m14DC(0), m14E0(0), m14E4(50.0f), m14E8(40.0f) {
    for (int i = 0; i < 4; i++) {
        m14EC[i].mSkillId = 0;
        m14EC[i].m04 = 0;
        m14EC[i].mCoolTime = 0;
        m14EC[i].m0C = 0;
    }
    m1570 = 900;
    m1574 = 1200;
    m1578 = 3;
    m157C = 3;
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em312_00_00.csv\0");
    }
    m14B4.vf0C(path);
    m1580.fn_8023780C("/effect/ef_bs007a.efp");
    m158C.fn_8023780C("/effect/ef_bs027a.efp");
    m14EC[0].mSkillId = 0x4E2;
    m14EC[0].mCoolTime = 240;
    m14EC[1].mSkillId = 0x4E3;
    m14EC[1].mCoolTime = 120;
    m14EC[2].mSkillId = 0x4E4;
    m14EC[2].mCoolTime = 0;
    m14EC[3].mSkillId = 0x4E5;
    m14EC[3].mCoolTime = 0;
    m152C[0].m00 = 0x13F;
    m152C[0].m04 = 54.0f;
    m152C[0].m08 = -1;
    m152C[0].m0C = 2;
    m152C[1].m00 = 0x142;
    m152C[1].m04 = 52.0f;
    m152C[1].m08 = -1;
    m152C[1].m0C = 1;
    m152C[2].m00 = 0x145;
    m152C[2].m04 = 15.0f;
    m152C[2].m08 = -1;
    m152C[2].m0C = 2;
    m152C[3].m00 = 0x14A;
    m152C[3].m04 = 38.0f;
    m152C[3].m08 = -1;
    m152C[3].m0C = 2;
}

Em312DarkServant::~Em312DarkServant() {}

inline void Em312DarkServant::KnockDown_unk() {
    m14E0 = m157C;
    if (m58C != 13) {
        vf100(13);
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        if ((mStatus.m0C & 0x4000) != 0x4000) {
            mStatus.fn_8013310C(0x4000, 0);
        }
    }
}

int Em312DarkServant::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1580.fn_80237874() || m158C.fn_80237874()) {
        return 0;
    }
    fn_8030DEB0((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    mStatus.m18 |= 0x100;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC218;
    mStatus.m18 |= 0x2800000;
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    const char* master = strstr(((EnemyInfo_unk*)m14B0)->m2C, "master=");
    if (master != 0) {
        EnemyManager* mgr = EnemyManager::sInstance;
        if (mgr != 0) {
            EnemyTask* e = mgr->Find_unk(atoi(master + 7));
            if (e->m146C == 6) {
                m14D4 = e;
            }
        }
    }
    vfFC();
    return 1;
}

void Em312DarkServant::fn_8030DEB0(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "MoveRadius") == 0) {
            m14E4 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLength") == 0) {
            m14E8 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "BreathIntervalInit") == 0) {
            m1570 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "BreathInterval") == 0) {
            m1574 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackCountMax") == 0) {
            m1578 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "DamageCountMax") == 0) {
            m157C = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "VertSkillId") == 0) {
            m14EC[0].mSkillId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "VertIntervalFrame") == 0) {
            m14EC[0].mCoolTime = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HorzSkillId") == 0) {
            m14EC[1].mSkillId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HorzIntervalFrame") == 0) {
            m14EC[1].mCoolTime = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackAllSkillId") == 0) {
            m14EC[2].mSkillId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "BreathSkillId") == 0) {
            m14EC[3].mSkillId = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

void Em312DarkServant::vf10() {
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        vf104();
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
}

void Em312DarkServant::vf14() {
    if (m14D8 == 0) {
        return;
    }
    fn_80149A30();
}

int Em312DarkServant::vf10C(HitMsg_unk* msg) {
    switch (m58C) {
    case 9:
    case 10:
    case 11:
        msg->m10 *= 0.0f;
        break;
    case 12:
        if (m14C0 >= 25) {
            msg->m10 *= 0.0f;
            break;
        }
    default:
        if (fn_805F9920(&msg->m10) > 0.2f) {
            PSVECNormalize(&msg->m10, &msg->m10);
            msg->m10 *= 0.2f;
        }
        break;
    }
    return m14D8;
}

// NONMATCHING: ~95% - the inlined KnockDown_unk: the original loads DamageCountMax first and reloads it
// after the state test (5 variants)
void Em312DarkServant::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
        return;
    }
    if (msg->m44 != 1) {
        m14E0++;
    }
    if ((mStatus.m0C & 0x4000) == 0x4000) {
        m14E0 = m157C;
    }
    if (IsDown_unk()) {
        KnockDown_unk();
    } else if (m58C == 12 && m14C0 < 25) {
        vf100(6);
    }
}

// inferred: name invented
inline bool HasFlag54C_unk(CharaTask* c, unsigned int flag) {
    return (c->m54C & flag) == flag;
}

extern "C" int fn_8030E490(EnemyTask* servant) {
    return ((Em312DarkServant*)servant)->IsDown_unk();
}

extern "C" void fn_8030E4AC(EnemyTask* servant) {
    ((Em312DarkServant*)servant)->KnockDown_unk();
}

// NONMATCHING: ~90% - the original reloads m54C for the clear and stores m54C before m14A8 (4 variants)
extern "C" void fn_8030E530(EnemyTask* servant) {
    Em312DarkServant* s = (Em312DarkServant*)servant;
    s->m14E0 = 0;
    if ((s->mStatus.m0C & 0x4000) == 0x4000) {
        s->mStatus.fn_8012F440(0x4000, 0, 1, 0, 0, 0, 1);
    }
    if (s->m14D8 != 0 && (s->m54C & 0x2000) == 0x2000) {
        s->m14A8 |= 0x04000000;
        s->m54C &= ~0x2000;
    }
}

// NONMATCHING: ~85% - register numbering of the recursion inlined three levels deep (zero and counters;
// recursive inline, documented blocker)
void Em312DarkServant::fn_8030E5C8() {
    if (m14DC != 0) {
        m14DC = 0;
        Em006DarkMaster* master = (Em006DarkMaster*)m14D4;
        if (master != 0) {
            FixedArray_unk<EnemyTask*, 8>* list = &master->m14D8;
            for (unsigned int i = 0; i < list->mCount; i++) {
                ((Em312DarkServant*)list->mData[i])->fn_8030E5C8();
            }
        }
    }
}

void Em312DarkServant::vfFC() {
    fn_8016E970(2);
    vf100(6);
    m156C = m1570;
}

int Em312DarkServant::fn_8030FEAC(float radius) {
    int count = 0;
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = ((PlayerTask*)c)->mNext) {
        if (!fn_8016F3D0(c, 1)) {
            atn::Vector3 d = c->mPos - mPos;
            if (fn_805F9920(&d) < radius * radius) {
                count++;
            }
        }
    }
    for (CharaTask* c = EnemyManager::sInstance->mHead; c != 0; c = ((EnemyTask*)c)->mNext) {
        if (!fn_8016F3D0(c, 1)) {
            atn::Vector3 d = c->mPos - mPos;
            if (fn_805F9920(&d) < radius * radius) {
                count++;
            }
        }
    }
    return count;
}

void Em312DarkServant::fn_8030FFF0() {
    fn_80232B7C(this, 1);
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
    fn_8023A8B4(lbl_8087F3C0, &m158C, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

void Em312DarkServant::fn_80310084() {
    fn_80239DAC(lbl_8087F3C0, this, 1, 1);
}

// NONMATCHING: ~60% - the original keeps 99999 in r31 (lis/subi) and the sub-group in r28; pool offsets
void Em312DarkServant::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em312 DarkServant[%d]", m58);
    void* group;
    if (m54.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m54.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    fn_8008771C(group, "MoveRadius", &m14E4, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_8008771C(group, "AttackLength", &m14E8, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_800874C8(group, "BreathIntervalInit", &m1570, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "BreathInterval", &m1574, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "AttackCountMax", &m1578, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "DamageCountMax", &m157C, 0, 9999, 1, 0, 0);
    void* sub = fn_8008937C(group, "VertAttack");
    fn_800874C8(sub, "SkillId", &m14EC[0].mSkillId, 0, 99999, 1, 0, 0);
    fn_800874C8(sub, "CoolTime", &m14EC[0].mCoolTime, 0, 99999, 1, 0, 0);
    sub = fn_8008937C(group, "HorzAttack");
    fn_800874C8(sub, "SkillId", &m14EC[1].mSkillId, 0, 99999, 1, 0, 0);
    fn_800874C8(sub, "CoolTime", &m14EC[1].mCoolTime, 0, 99999, 1, 0, 0);
    sub = fn_8008937C(group, "AllAttack");
    fn_800874C8(sub, "SkillId", &m14EC[2].mSkillId, 0, 99999, 1, 0, 0);
    sub = fn_8008937C(group, "BreathAttack");
    fn_800874C8(sub, "SkillId", &m14EC[3].mSkillId, 0, 99999, 1, 0, 0);
}

