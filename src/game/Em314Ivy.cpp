#include "game/Em314Ivy.h"
#include "game/Inventory.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"
#include "chr/ControlCaught.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "atn/DrawHelper.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_801092C8(Effect_unk* effect, int msg, CharaTask* chara, int value, int arg);
extern "C" void fn_80107168(Effect_unk* effect, int id, int a, int b, int c, const atn::Vector3* pos,
                            const atn::Vector3* dir, const struct IvyColor_unk* color, float scale); // inferred: plays an effect
// inferred: color as four floats (name invented)
struct IvyColor_unk {
    IvyColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_8044D6AC(void* mgr, int money); // inferred: adds money (gmk/RandomMerchant.cpp)
extern "C" void* lbl_8087F498; // inferred: voice manager
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed (as in game/Arms.cpp)
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: sound effects of the ivy
static const char* const sSounds_unk[] = {"SE_EM314_004"};

// inferred: plays sound `id` at `pos`
extern "C" atn::SoundRef fn_80310314(int id, const void* pos) {
    return fn_800C344C(sSounds_unk[id], pos, 0, -1, 1.0f);
}

// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

Em314Ivy::Em314Ivy(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(info->m14),

      m14E0(1e16f), m14E4(0), m14EC(0),
      m14F0(0), m14F4(0), m14F8(0), m14FC(1.0f), m1500(1.0f), m1504(1.0f), m1508(1.0f), m150C(0), m151C(0), m15A0(1), m15A4(0), m15A8(0), m15AC(0), m15B0(-1), m15B4(0), m15B8(0) {
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em314_00_00.csv\0");
    }
    m14B4.vf0C(path);
}

Em314Ivy::~Em314Ivy() {}

void Em314Ivy::fn_80310C64(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "SkillId") == 0) {
            m14EC = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HideWaitFrame") == 0) {
            m14F0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ReleaseWaitFrame") == 0) {
            m14F4 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HoldDamageInterval") == 0) {
            m14F8 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ReleaseForce") == 0) {
            m14FC = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "AppearDistance") == 0) {
            m1500 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "HideDistance") == 0) {
            m1504 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "CatchDistance") == 0) {
            m1508 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "RandomPosition") == 0) {
            m15A0 = atoi(csv.fn_8005B9CC()) == 1;
        } else if (strcmp(key, "Strong") == 0) {
            m15AC = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "CatchNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m15B0, &csv);
        }
    } while (csv.fn_8005BBF8());
}

// NONMATCHING: 99.34% - stack slots of the node position temporaries and r3/r4/r5 of the Vector3 copies
// (__VecCopy family); 12 variants (declaration order, inline helpers, named temporaries)
void Em314Ivy::vf10() {
    fn_80312EF8();
    vf104();
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
    atn::Vector3 base = mPos;
    MtxPtr ref = mMotion.GetNodeMtx_unk("reference");
    if (ref != 0) {
        base = GetNodePos_unk(ref);
    }
    m620 = m5B0;
    m614 = base + m5A4;
    m614.y += m620;
    atn::Vector3 top = m614;
    atn::Vector3 head = top;
    head.y += m5B4 - 2.0f * m620;
    mColli.m3C = top;
    mColli.m48 = head;
    mColli.m54 = m620;
}

// NONMATCHING: 83.25% - the original loads the point and the arguments before the branch that picks the color
// (li -0x100 / bne / li -0x5600 just before the call); ~20 variants (ternary, if/else, declaration order,
// inline helpers, coordinates in locals)

void Em314Ivy::vf14() {
    fn_80149A30();
    if (m15B8 != 0) {
        for (unsigned int i = 0; i < m151C; i++) {
            Point_unk* p = m1520[i];
            unsigned int color = 0xFFFFFF00;
            if (i == 0) {
                color = 0xFFFFAA00;
            }
            lbl_8087EEB0->fn_80062C8C(16, color, p->m04.x, p->m04.y, p->m04.z, 0.0f, p->m14, 5.0f);
        }
    }
}

int Em314Ivy::vf10C(HitMsg_unk* msg) {
    if (m58C == 7 || m58C == 8 || m58C == 10) {
        return 0;
    }
    if ((msg->mData->mAC & 4) != 4 && msg->m44 == 1) {
        return 0;
    }
    msg->m10 *= 1.0f;
    return 1;
}

void Em314Ivy::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m15AC == 0 || (m14E4 != 0 && m14E4->mType != 0) || msg->mFlag31 || (mStatus.m0C & 0x20) == 0x20) {
        if (m58C == 11 || m58C == 13) {
            m15A4 = 1;
        } else {
            int state;
            if (m14E4 == 0 || (m14E4->CanAct_unk() && m15A0 != 0)) {
                state = 8;
            } else {
                state = 13;
            }
            vf100(state);
            m15A4 = 1;
        }
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        mStatus.fn_8012D8B8();
        m54C |= 0x2000;
    }
}

void Em314Ivy::vfFC() {
    fn_8016E970(2);
    vf100(10);
}

// NONMATCHING: 98.90% - registers of the inlined CanAct_unk test (r5/r6, as in vf10) and of the copy loop of
// the inlined push_back (r29/r30, r6/r7); ~8 variants (target local, inverted test, element pointer)
void Em314Ivy::vf104() {
    m14C0++;
    if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) == 0 && (m58C == 8 || m58C == 10) &&
        (m137C & 0x400) == 0x400) {
        m15A8 = 1;
    }
    if ((m1374 & 2) == 2 && (m54C & 0x2000) != 0x2000) {
        *(volatile unsigned int*)&m54C |= 0x2000;
        if (m58C == 11 || m58C == 13) {
            m15A4 = 1;
        } else {
            int state;
            if (m14E4 == 0 || (m14E4->CanAct_unk() && m15A0 != 0)) {
                state = 8;
            } else {
                state = 13;
            }
            vf100(state);
            m15A4 = 1;
        }
    }
    switch (m58C) {
    case 6:
        vf108();
        break;
    case 7:
        if (mFlag31) {
            vf100(9);
            if (m1510.mSize != 0) {
                m624.clear();
                for (unsigned int i = 0; i < m1510.mSize; i++) {
                    m624.push_back(m1510.mData[i]);
                }
                m1510.clear();
            }
        }
        vf108();
        break;
    case 8:
        if (mFlag31) {
            vf100(10);
            m150C = m14F0;
        }
        vf108();
        break;
    case 9:
        if (mFlag31) {
            vf100(m14C4);
        }
        vf108();
        break;
    case 10:
        if (mFlag31) {
            vf100(7);
            m150C = 0;
        }
        vf108();
        break;
    case 11:
        if (mFlag31) {
            vf100(m14C4);
            if (m14C4 == 9) {
                m150C = 0;
            }
        }
        vf108();
        break;
    case 12:
        if (mFlag31) {
            vf100(m14C4);
            m15A4 = 0;
        }
        vf108();
        break;
    case 13:
        if (mFlag31) {
            if (m15AC == 0) {
                vf100(9);
                m150C = m14F4;
            } else {
                mStatus.fn_8012DD70();
                vf100(8);
            }
        }
        vf108();
        break;
    case 2:
        fn_805A40B8();
        break;
    default:
        vf100(6);
        break;
    }
}

void Em314Ivy::fn_80312EF8() {
    if (!(m58C != 9 && m58C != 10)) {
        mC58.mC4 = 0;
        m14E0 = 1e16f;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (!p->CanAct_unk() || (p->m54C & 0x2000) == 0x2000) {
                continue;
            }
            if (p->m55C == 6) {
                switch (p->m560) {
                case 17: case 18: case 19:
                case 48:
                case 50:
                case 87: case 88: case 89:
                case 106:
                case 110:
                case 116: case 117: case 118:
                    continue;
                }
            }
            atn::Vector3 d = p->mPos - mPos;
            float dist = PSVECSquareMag(&d);
            if (dist < m14E0) {
                mC58.mC4 = p;
                m14E0 = dist;
            }
            if (p->mFlagB16) {
                mC58.mC4 = p;
                m14E0 = dist;
                return;
            }
        }
    }
}

int Em314Ivy::vfF8() {
    return mC58.mC0 != 0 && CanAct_unk() && (m54C & 0x2000) != 0x2000;
}

void Em314Ivy::fn_8031319C(CharaTask* target) {
    m14E4 = target;
    if (target != 0) {
        m14E8 = mMotion.GetNodeMtx_unk("catch_point");
        if (m14E8 != 0) {
            m14E4->fn_801595BC((int)m14E8, 25, 0x1D8, -1);
            chr::ControlCaught* control = (chr::ControlCaught*)m14E4->mControl.get();
            if (control != 0) {
                control->m18 = 0;
                control->m19 = 0;
                if (m15AC != 0) {
                    control->m1A = 0;
                }
            }
        }
        if (lbl_8087F430 != 0 && m14E4->mType != 2) {
            fn_805A4A1C((SpecialFlag_unk*)&m15B0, m14E4->mType == 0 ? -1 : m14E4->m58);
        }
        fn_80310314(0, &m14E4->mPos);
        if (lbl_8087F498 != 0) {
            fn_803EA77C(lbl_8087F498, m14E4, 0x2A, 0, 1.0f, 10.0f);
        }
    }
}

// NONMATCHING: 99.50% - saved registers of money/counter/effect manager (r28/r27/r29 in the original) and the
// stack slots of the color and the position of the effect loop; ~20 variants
void Em314Ivy::fn_803132D0() {
    if (m14E4 != 0) {
        Mtx m;
        atn::Vector3 dir;
        if (m14E4->mType == 0 && m15AC != 0 && lbl_8087F048 != 0 && Inventory::instance_ != 0) {
            int money = Inventory::instance_->mMoney / 3;
            fn_8044D6AC(Inventory::instance_, money);
            for (int i = 0; i < 5; i++) {
                Effect_unk* effect = lbl_8087F048;
                fn_80107168(effect, 0x232D, 0, 0, 0, &m14E4->fn_801781B0(), &chr::lbl_807C7030,
                            &IvyColor_unk(1.0f, 1.0f, 1.0f, 1.0f), 1.0f);
            }
            fn_801092C8(lbl_8087F048, 4, 0, money, 0);
        }
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 1.0f;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &dir, &dir);
        dir *= m14FC;
        dir.y += 0.25f * m14FC;
        int motion = -1;
        if (m14E4->mMotion.fn_80097D40(0x3A) != 0) {
            motion = 0x3A;
        }
        m14E4->fn_8015E7A0(dir, motion, 0);
        m14E4->mF1C = 0;
        m14E4 = 0;
    }
}

void Em314Ivy::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em314 Ivy[%d]\0", m58);
    void* group;
    if (m54.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m54.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    fn_80087994(group, "DebugDisp", &m15B8, 0, 0);
}
