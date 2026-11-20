#include "macros.h"
#include "game/Em202ForestSpider.h"
#include "game/GameTask.h"
#include "atn/Csv.h"
#include "atn/Material_unk.h"
#include "game/CharaStatus.h"
#include "chr/ControlBase.h"
#include "game/EnemyManager.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/PlayerManager.h"
#include "game/FriendManager.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"
#include "game/BattleTask.h"
#include "atn/ColliBase.h"
#include "game/GameSceneFind_unk.h"
#include "atn/TextTable_unk.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);

extern "C" GameTask* lbl_8087F430;
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: player manager (lbl_8087F8A0)
extern "C" PlayerManager* lbl_8087F8A0;
// inferred: link of the character lists of the managers (CharaTask + 0x14AC, as in game/Em202Spider.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}
// inferred: sends `id` to the gimmicks in a cone (game/SpecialEnemyBase.cpp)
extern "C" void fn_805A5078(const atn::Vector3* pos, const atn::Vector3* dir, int id, float radius, float angle);
// inferred: segment against capsule test
extern "C" int fn_80051B70(const atn::ColliCapsuleShape_unk* a, const atn::ColliCapsuleShape_unk* b);
// inferred: zero vector
extern "C" atn::Vector3 lbl_807C7030;
struct ColliManager_unk;
extern "C" ColliManager_unk* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* colli, atn::ColliResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" int fn_800C3118(const char* group); // inferred: nonzero when the sound group is loaded
extern "C" atn::SoundRef fn_800C344C(const char* name, const atn::Vector3& pos, int arg2, int arg3, float volume);
extern "C" double sqrt(double x);
extern "C" HitInfo_unk* fn_80219E6C(int id);  // inferred: skill data
extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager
// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (as in game/Em202Spider.cpp)
namespace atn {
class EffectManager;
}
extern "C" atn::EffectManager* lbl_8087F3C0;
extern "C" void fn_80239DAC(atn::EffectManager* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, void* node,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A680(atn::EffectManager* mgr, atn::EffFile* eff, int a, int b, const atn::Matrix3x4* mtx, int c,
                            int d, int e, int f, int g, int h, float scale); // inferred: effect on a matrix
extern "C" void fn_80087994(void* group, const char* name, int* value, void* func, void* arg); // debug menu
// inferred: effect `efp` on the model (shared with gmk/FlamingArrow, Em315Coffin and the Quark enemies)
extern "C" void fn_80276AE4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, const atn::Vector3& pos,
                            const atn::Vector3& rot, int a5, int a6, int a7, float scale);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: effect `efp` on a model with no offset (name invented)
static inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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
// inferred: sounds of the spider (pointers kept in .sdata2)
static const char* const sSe_unk = "SE_DG001_014";
static const char* const sSeGroup_unk = "GROUP_DG001";
// inferred: color of the spiders when they are down (game/Em202SpiderBase.cpp)
extern "C" SpiderBaseColor_unk lbl_807C8398;

// inferred: name invented
static inline float Dot_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}

// inferred: front direction of the character (name invented)
static inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.SetRotationY_unk(rot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}

// inferred: difference on the ground plane (name invented, as in game/Em202Spider.cpp)
static inline void SetFlat_unk(atn::Vector3& v, float x, float z) {
    v.z = z;
    v.x = x;
    v.y = 0.0f;
}

// inferred: the character can be targeted (CanAct_unk with the flags reloaded; name invented)
static inline bool IsTargetable_unk(CharaTask* c) {
    bool ok = false;
    if (c->IsFree_unk()) {
        bool grappled = false;
        if (c->m55C == 6 && c->m560 == 28) {
            grappled = true;
        }
        if (!grappled) {
            ok = true;
        }
    }
    return ok;
}

// inferred: other form of IsTargetable_unk used by vf110 (inactive flags in a bool, then the 0x20 status and
// the grapple; as ShadeFree_unk in game/Em301LShade.cpp); names invented
static inline bool IsInactive_unk(CharaTask* c) {
    bool r = false;
    if ((c->mFlags & 4) == 4 || (c->mFlags & 1) == 1) {
        r = true;
    }
    return r;
}
static inline bool IsGrappled_unk(CharaTask* c) {
    bool r = false;
    if (c->m55C == 6 && c->m560 == 28) {
        r = true;
    }
    return r;
}
static inline bool IsBindable_unk(CharaTask* c) {
    bool ok = false;
    if (!IsInactive_unk(c) && !c->mStatus.IsState_unk(0x20) && !IsGrappled_unk(c)) {
        ok = true;
    }
    return ok;
}

// NONMATCHING: 98.75% - scheduling of the three inlined clear() (the original loads m1908, m14B0, m14C0 in
// that order; 6 orders of the calls tried)
Em202ForestSpider::Em202ForestSpider(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14BC(0), m14D4(0), m14DC(0), m14E0(0), m14E4(0), m14E8(0), m14EC(0), m14F0(0.0f),
      m14F4(0.0f), m14F8(0), m14FC(1), m1500(0), m1504(30.0f), m1508(0x3F2), m150C(0x3F2), m1510(40.0f),
      m1514(180.0f), m1518(2), m151C(0), m1520(3), m1524(60), m1528(5), m152C(150), m1530(0), m1534(0),
      m18E8(0), m18EC(0), m18F0(0), m18F8(0.0f), m1904(1), m1964(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m18FC = 0.8f;
    m1900 = 0x40000000;
    std::string dir("database/chara/special/");
    std::string name("db_em202_02_00.csv");
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
    m1938.vf0C(path.c_str());
    m1908.clear();
    m14B0.clear();
    m14C0.clear();
    m1914.LoadRequest("/effect/ef_bd003.efp");
    m1920.LoadRequest("/effect/ef_uc007.efp");
    m192C.LoadRequest("/effect/ef_sc003_01b.efp");
    for (int i = 0; i < 4; i++) {
        m1538[i].m8C.LoadRequest("/effect/ef_bs006a.efp");
        m1538[i].m98.LoadRequest("/effect/ef_bs006b_01.eff");
        m1538[i].mA4.LoadRequest("/effect/ef_bs006c.efp");
        m1538[i].mB0.LoadRequest("/effect/ef_bs009_01.eff");
        m1538[i].mBC.LoadRequest("/effect/ef_bs010_01.eff");
        m1538[i].mC8.LoadRequest("/effect/ef_bs011.efp");
        m1538[i].mD4.LoadRequest("/effect/ef_uc045.efp");
        m1538[i].mE0.LoadRequest("/effect/ef_bs011d.efp");
    }
}

Em202ForestSpider::~Em202ForestSpider() {}

// NONMATCHING: 97.2% - saved registers of the reading loop (r29 for this in the original)
int Em202ForestSpider::vf0C() {
    int loading = 0;
    for (int i = 0; i < 4; i++) {
        if (m1538[i].m8C.IsLoading() || m1538[i].m98.IsLoading() || m1538[i].mA4.IsLoading() ||
            m1538[i].mB0.IsLoading() || m1538[i].mBC.IsLoading() || m1538[i].mC8.IsLoading() ||
            m1538[i].mD4.IsLoading() || m1538[i].mE0.IsLoading()) {
            loading = 1;
            break;
        }
    }
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1914.IsLoading() == 0 &&
        m1920.IsLoading() == 0 && m192C.IsLoading() == 0 && m1938.fn_80473F50() == 0 && loading == 0) {
        if (m1938.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m1938.fn_80470580(), m1938.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "AttackDist") == 0) {
                    m1504 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "StopDist") == 0) {
                    m1510 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "GuardTheta") == 0) {
                    m1514 = fn_800DC288(csv.fn_8005B9CC());
                    if (atn::Abs_unk(m1514 - 180.0f) < 1.1920929e-7f) {
                        m1514 = 179.0f;
                    }
                } else if (strcmp(key, "TackleID") == 0) {
                    m150C = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackID") == 0) {
                    m1508 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "AttackNum") == 0) {
                    m1518 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "TackleNum") == 0) {
                    m1520 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "WaitTime") == 0) {
                    m1524 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "ReinforceCount") == 0) {
                    m152C = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "ReinforceCenter") == 0) {
                    m14BC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ThreadPoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        gs::WarpPoint* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m14C0.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "ReinforceChara") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        EnemyTask* enemy = EnemyManager::sInstance->Find_unk(id);
                        if (enemy != 0) {
                            m14B0.push_back(enemy);
                        }
                    } while (id != 0);
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}


// NONMATCHING: 92.2% - the original keeps the address of each capsule member in its own saved register (r19-r30)
// and orders the sight temporaries differently on the stack (flow, calls and constants checked)
void Em202ForestSpider::vf10() {
    vfF8();
    if (mStatus.IsState_unk(4)) {
        CharaTask* player = lbl_8087F8A0->fn_8054A33C(5);
        if (player != 0 && IsTargetable_unk(player)) {
            mStatus.fn_80133130(4, 10, 0);
        }
    }
    if (m18F0 > 0) {
        CharaTask* head = lbl_8087F8A0->mHead;
        if (head != 0 && head->m638 != 0) {
            if (head->m55C == 6 && head->m560 == 14) {
                chr::ControlBase* control = head->mControl.get();
                if (control != 0 && control->vf54() != 0 && m1904 != 0) {
                    m1904 = 0;
                    int hit = 0;
                    for (int i = 0; i < 4; i++) {
                        SpiderThread_unk* t = &m1538[i];
                        if (t->m00 != 0) {
                            atn::Model* model = &t->m88->mMotion;
                            float radius = 0.7f * head->m638->m58;
                            MtxPtr m = model->GetNodeMtx_unk("spine");
                            atn::Vector3 a(m[0][3], m[1][3], m[2][3]);
                            MtxPtr hm = mMotion.GetNodeMtx_unk("head");
                            atn::Vector3 pos = head->mPos;
                            a.y = 0.0f;
                            atn::Vector3 b(hm[0][3], 0.0f, hm[2][3]);
                            atn::ColliCapsuleShape_unk seg(a, b, 0.0f);
                            atn::ColliCapsuleShape_unk cap(pos, pos, radius);
                            if (fn_80051B70(&seg, &cap) != 0) {
                                hit = 1;
                            }
                        }
                    }
                    if (hit) {
                        m18F0 = 0;
                        if (m18EC != 0) {
                            m18EC->fn_8015C6A0();
                        }
                        m18EC = 0;
                        m14E0 = 600;
                        for (int i = 0; i < 4; i++) {
                            fn_802790A0(i);
                        }
                        vf150();
                    }
                }
            } else {
                m1904 = 1;
            }
        }
    } else {
        m1904 = 1;
    }
    if (mC58.mC0 == 0 || m14D4 == 0) {
        fn_8016E970(3);
        m58C = 6;
        vfFC();
    } else {
        if (m58C == 0) {
            m58C = 6;
        }
        switch (m58C) {
        case 7:
            vf108();
            break;
        case 8:
            vf10C();
            break;
        case 9:
            vf110();
            break;
        case 10:
            vf114();
            break;
        case 11:
            vf118();
            break;
        case 12:
            vf11C();
            break;
        case 16:
            vf120();
            break;
        case 13:
            vf124();
            break;
        case 14:
            vf128();
            break;
        case 15:
            vf12C();
            break;
        case 2:
            vfEC();
            break;
        case 17:
            vf130();
            break;
        case 18:
            vf134();
            break;
        default:
            vf104();
            if (m58C == 6) {
                vf100();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    fn_80279280();
    m14DC++;
    if (m14E0 > 0) {
        m14E0--;
    }
    atn::Vector3 top(mPos.x, mPos.y + m5B0, mPos.z);
    mColli.m3C = mPos;
    mColli.m48 = top;
    mColli.m54 = m5B0;
    m1500++;
    if (m1500 > 10 && (m58C == 6 || m58C == 16)) {
        fn_80277BC0();
        m1500 = 0;
    }
    atn::Vector3 pos = mPos;
    atn::Vector3 fwd = FrontDir_unk(mRot);
    fn_805A5078(&pos, &fwd, 1000, 1.2f * m5B0, 3.1415927f);
}

// NONMATCHING: 93% - stack slots of the color temporaries (as Em202Spider::vf14)
void Em202ForestSpider::vf14() {
    SpiderBaseColor_unk target = m1950;
    bool down = true;
    if (!mStatus.IsState_unk(0x80000) && !mStatus.IsState_unk(0x1000000)) {
        down = false;
    }
    if (down) {
        target = lbl_807C8398;
    }
    m1940 = (target - m1940) * 0.15f + m1940;
    mMotion.fn_80092954("em202_body")->mColor = m1940.ToGX();
    mMotion.fn_80092954("em202_tail")->mColor = m1940.ToGX();
    fn_80149A30();
}

void Em202ForestSpider::vf3C(HitInfo_unk* hit, int) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    bool down = true;
    unsigned int flags = mStatus.m0C;
    if ((flags & 0x80000) != 0x80000 && (flags & 0x1000000) != 0x1000000) {
        down = false;
    }
    if (!down && m58C != 17) {
        if (m58C != 6) {
            msg->m10 = lbl_807C7030;
        }
        if (msg->mData->m01 != 1) {
            int guard = 0;
            float dot = Dot_unk(msg->m1C - mPos, FrontDir_unk(mRot));
            if (dot > (float)cos(m1514 / 2.0f)) {
                guard = 1;
            }
            if (guard) {
                msg->m10 = lbl_807C7030;
                msg->m48 = 1;
            }
        } else {
            m1534++;
            if (m1534 >= 2) {
                m1530 = 1;
            }
        }
    }
    if (m58C == 18) {
        msg->m10 *= 0.0f;
    }
    float len = PSVECSquareMag(&msg->m10);
    if (!((float)__fabs(len) < 1.1920929e-7f) && len >= 25.0f) {
        PSVECNormalize(&msg->m10, &msg->m10);
        msg->m10 *= 4.9f;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em202ForestSpider::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (mStatus.IsState_unk(0x20)) {
        vf164();
        return;
    }
    if (msg->mData != 0 && msg->mData->m04 == 0x1791) {
        vf16C();
    }
}

void Em202ForestSpider::vf7C(int arg) {
    vf168();
}

void Em202ForestSpider::vf54() {
    if (m624.mSize != 0) {
        m624[0].m04.mRadius = 1.5f * m5B0;
        m624[0].m04.mCenter = mPos + m5A4;
        m624[0].m04.mCenter.y += 0.5f * m624[0].m04.mRadius;
    }
}

void Em202ForestSpider::vfF8() {
    mC58.mC8 = (int)mC58.mC4;
    switch (m58C) {
    case 9:
    case 10:
    case 11:
        m1454 = 0;
        m14D4 = m18E8;
        break;
    case 15:
    case 16:
        m1454 = 0;
        break;
    default: {
        CharaTask* found;
        CharaTask* c = lbl_8087F8A0->mHead;
        if (c->mFlag28) {
            m1454 = 0;
            m14D4 = c;
        } else {
            found = 0;
            for (; c != 0; c = NextChara_unk(c)) {
                if (m1530 != 0) {
                    if (IsTargetable_unk(c) && c->mStatus.fn_80134270() != 0) {
                        found = c;
                        break;
                    }
                } else {
                    if (IsTargetable_unk(c) && c->mStatus.fn_80134290() != 0) {
                        found = c;
                        break;
                    }
                }
            }
            if (found != 0) {
                m1454 = 0;
                m14D4 = found;
            } else {
                m1454 = 1;
                m14D4 = mC58.mC4;
            }
        }
        break;
    }
    }
    mC58.mC4 = m14D4;
}

// NONMATCHING: 99.12% - float registers (speed/mag/dist: f30/f31/f29 in the original), the 0.0f of the last branch loaded at the start and the stack
// slots of the normalized route direction and its rotation
void Em202ForestSpider::vfFC() {
    float mag = 0.0f;
    float speed = 1.5f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 dir = mRoute.m58;
        mag = PSVECMag(&dir);
        if (mag > 0.0001f) {
            rot = dir.GetNormalize().ToRotation_unk();
        }
        dir = m14D4->mPos - mPos;
        dir.y = 0.0f;
        float dist = PSVECMag(&dir);
        PSVECNormalize(&dir, &dir);
        dir.fn_80139F60();
        mRot.y = dir.y;
        if (m14F8 != 0) {
            speed *= 2.5f;
        } else if (dist < m1510) {
            speed = 0.0f;
        }
        fn_801426A4(&rot, mag, m568 * speed);
    } else if (m55C == 6) {
        fn_8013A258();
    } else {
        fn_8013CB68(rot, 0, mag, m568 * speed);
    }
}

void Em202ForestSpider::vf100() {
    if (m14D4 != 0) {
        if (m14FC > 0) {
            m14FC--;
            if (m14FC == 0) {
                if (m14BC != 0) {
                    vf15C();
                } else {
                    vf158();
                }
                return;
            }
        } else if (fn_8027819C() != 0) {
            m14FC = m152C;
        }
        if (m14DC >= m1524) {
            CharaTask* t = m14D4;
            atn::Vector3 d = t->mPos - mPos;
            float dist = sqrt(d.x * d.x + d.z * d.z);
            if (m14E0 <= 0 && !mStatus.IsState_unk(4)) {
                vf160();
            } else if (dist <= m1504) {
                if (m14E4 % 5 == 0) {
                    vf13C();
                } else {
                    vf140();
                }
            }
        }
    }
}

void Em202ForestSpider::vf104() {
    if (m55C != 6) {
        if (m55C == 7) {
            if (m14D4 != mRoute.mTarget) {
                if (vf154(6) != 0) {
                    return;
                }
                m58C = 6;
                fn_8016E970(3);
            }
        } else {
            if (vf154(6) != 0) {
                return;
            }
            m58C = 6;
            fn_8016E970(3);
            fn_80170A20(m14D4, -1.0f, 0);
        }
    }
    vfFC();
}

void Em202ForestSpider::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
        } else {
            for (unsigned int i = 0; i < m1908.size(); i++) {
                CharaTask* c = m1908[i];
                c->fn_80176ACC();
                c->mPos = mPos;
                c->mF1C = 0;
                c->mFlagB9 = 0;
                c->mMotion.fn_80097C08(0, 0x32, 1, 0, 1, 1.0f, 0.1f);
                c->fn_8015EFAC(10);
            }
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360();
            fn_800EB7A0();
        }
    } else {
        m1434 = 5;
    }
}

void Em202ForestSpider::vf108() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        vf138();
    } else {
        float f = mMotion.mTracks[0].m08;
        if (f >= 17.0f && f <= 22.0f) {
            int id = m590;
            HitInfo_unk* skill = fn_80219E6C(m1508);
            lbl_8087F048->fn_800F8C6C(0, 0, this, skill, id, 0.0f, 30, -1);
        }
    }
}

void Em202ForestSpider::vf10C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        vf138();
    }
    float f = mMotion.mTracks[0].m08;
    if (f >= 12.0f && f <= 17.0f) {
        int id = m590;
        HitInfo_unk* skill = fn_80219E6C(m150C);
        lbl_8087F048->fn_800F8C6C(0, 0, this, skill, id, 0.0f, 30, -1);
    }
}

// NONMATCHING: 95.2% - registers and stack slots (state 9, threads; calls, constants and member offsets
// checked against the original)
void Em202ForestSpider::vf110() {
    if (m14D8 == 0) {
        atn::Vector3 d = m18E8->mPos - mPos;
        PSVECNormalize(&d, &d);
        atn::Vector3 r = d.ToRotation_unk();
        r.z = 0.0f;
        mRot = r;
        if (m14DC > 30) {
            mMotion.fn_80097CCC(0, 0.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x146, 0, 0, 1, 0.0f, 0.1f);
            m14DC = 0;
            m14D8 = 1;
        }
        return;
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m1538[0].m04 != 0) {
            m18EC = m1538[0].m88;
            wchar_t text[256];
            memset(text, 0, sizeof(text));
            const wchar_t* name = m18EC->m60->m04;
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xB5), name);
            lbl_8087F048->fn_80109828(text);
            lbl_8087F430->fn_80370AE4(0x6E, m18EC->mType == 0 ? 10 : m18EC->m58);
            vf148();
            return;
        }
        m14E0 = 600;
        fn_80279054();
        vf138();
        return;
    }
    if (m14DC == 15) {
        int next = 1;
        for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
            if (c->IsFlag_unk(4)) {
                continue;
            }
            if (IsBindable_unk(c) || c->mStatus.m224 > 0) {
                int idx = next;
                int main = 0;
                if (c == m18E8) {
                    idx = 0;
                    main = 1;
                }
                if (c->mStatus.IsState_unk(0x20)) {
                    fn_80278634(c, idx, main);
                    atn::Vector3 waist = ((atn::Matrix3x4*)c->GetModel_unk()->GetNodeMtx_unk("waist"))->GetTrans_unk();
                    m1538[idx].m28.m[0][3] = waist.x;
                    m1538[idx].m28.m[1][3] = waist.y;
                    m1538[idx].m28.m[2][3] = waist.z;
                    m1538[idx].m04 = 1;
                    m1538[idx].m88->fn_8015C0C0((int)this, m1538[idx].m0C);
                    atn::Vector3 pos(m1538[idx].m28.m[0][3], m1538[idx].m28.m[1][3], m1538[idx].m28.m[2][3]);
                    fn_800C344C("SE_EM202_012", pos, 0, -1, 1.0f);
                    fn_80232B7C(0, 0);
                    atn::Vector3 rot = m1538[idx].m10.ToRotation_unk();
                    rot.z = 0.0f;
                    atn::Vector3 p(m1538[idx].m28.m[0][3], m1538[idx].m28.m[1][3], m1538[idx].m28.m[2][3]);
                    fn_80276AE4(lbl_8087F3C0, &m1538[idx].mA4, 0, p, rot, -1, -1, 1, 1.0f);
                    fn_80232B7C(&m1538[idx], 0x66);
                    PlayEffect_unk(&m1538[idx].mC8, &m1538[idx].m88->mMotion);
                    fn_80232B7C(&m1538[idx], 0x67);
                    PlayEffect_unk(&m1538[idx].mD4, &m1538[idx].m88->mMotion);
                } else {
                    fn_80278634(c, idx, main);
                    fn_80232B7C(&m1538[idx], 0x64);
                    fn_8023A680(lbl_8087F3C0, &m1538[idx].m98, -1, 5, &m1538[idx].m28, 0, 0, 0, 0, -1, 1, 1.0f);
                }
                if (c != m18E8) {
                    next++;
                }
            }
        }
    }
}

// inferred: -v component by component (name invented)
inline atn::Vector3 SpiderNeg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// NONMATCHING: 99.58% - stack slots: the original places the locals of state 0x75 above those of state 0x74
// (state 10, pulls the caught character; flow, calls and constants checked against the original)
void Em202ForestSpider::vf114() {
    int ok = 0;
    CharaTask* c = m18EC;
    if (c->m55C == 6) {
        int state = c->m560;
        if (state == 0x74) {
            m14DC = 0;
            float target = 1.5707964f + c->mRot.y;
            atn::Vector3 d(c->mPos.x - mPos.x, 0.0f, c->mPos.z - mPos.z);
            m18F8 = atn::WrapAngle_unk(d.ToRotationZ_unk().y - target) / 10.0f;
            ok = 1;
        } else if (state == 0x75) {
            atn::Vector3 d(c->mPos.x - mPos.x, 0.0f, c->mPos.z - mPos.z);
            float dist = PSVECMag(&d);
            if (m14DC <= 10) {
                float step = dist - 40.0f;
                atn::Vector3 pos = m18EC->mPos;
                step = MIN(step, 1.5f);
                step = MAX(step, -1.5f);
                m18EC->mPos = SpiderNeg_unk(d).GetNormalize() * step + pos;
                atn::Vector3 rot = m18EC->mRot;
                rot.y += m18F8;
                m18EC->mRot = rot;
            } else {
                if (dist <= 45.0f) {
                    vf14C();
                    return;
                }
                float step = dist - 40.0f;
                atn::Vector3 pos = m18EC->mPos;
                step = MIN(step, m18FC);
                m18EC->mPos = SpiderNeg_unk(d).GetNormalize() * step + pos;
            }
            ok = 1;
        }
    }
    if (ok == 0) {
        m18EC->fn_8015C6A0();
        m18EC = 0;
        m14E0 = 600;
        fn_80279054();
        vf150();
    }
}

void Em202ForestSpider::vf118() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        wchar_t text[256];
        memset(text, 0, sizeof(text));
        CharaInfo_unk* info = m18EC->m60;
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xB7), info->m04);
        lbl_8087F048->fn_80109828(text);
        m14E0 = 600;
        m1908.push_back(m18EC);
        m18EC->fn_801765D8();
        m18EC = 0;
        fn_80279054();
        vf138();
    }
}

void Em202ForestSpider::vf11C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        vf138();
    }
}

void Em202ForestSpider::vf120() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m14D4 = m14D0;
        switch (m14E8) {
        case 7:
            vf13C();
            break;
        case 8:
            vf140();
            break;
        case 9:
            vf144();
            break;
        default:
            vf138();
            break;
        }
    } else {
        float f = mMotion.mTracks[0].m08;
        if (5.0f <= f && f <= 16.0f) {
            m14EC += 2;
            float d = atn::WrapAngle_unk(m14F4 - m14F0);
            mRot.y = atn::WrapAngle_unk(m14F0 + (float)m14EC * d / 12.0f);
        }
    }
}

void Em202ForestSpider::vf124() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80277DC4();
        vf138();
    }
}

// inferred: GameScene::FindPlace_unk with the array read inside the loop (as in game/Em202Spider.cpp)
inline gs::WarpPoint* FindForestPlace_unk(int id) {
    GameScene* table = lbl_8087F430->m10D8;
    for (unsigned int i = 0; i < table->m78.size(); i++) {
        gs::WarpPoint* places = table->m78.mData;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}

void Em202ForestSpider::vf128() {
    gs::WarpPoint* place = FindForestPlace_unk(m14BC);
    atn::Vector3 d;
    SetFlat_unk(d, mPos.x - place->mPos.x, mPos.z - place->mPos.z);
    if (PSVECSquareMag(&d) < 100.0f) {
        m14F8 = 0;
        mRot.y = place->m14;
        vf158();
    } else {
        vfFC();
    }
}

void Em202ForestSpider::vf12C() {
    gs::WarpPoint* place = m14CC;
    atn::Vector3 d;
    SetFlat_unk(d, mPos.x - place->mPos.x, mPos.z - place->mPos.z);
    if (PSVECSquareMag(&d) < 100.0f) {
        m14F8 = 0;
        if (vf154(9) == 0) {
            vf144();
        }
    } else {
        vfFC();
    }
}

void Em202ForestSpider::vf134() {
    if (m14DC >= 450) {
        vf138();
    }
}

void Em202ForestSpider::fn_8027688C() {
    m14D8 = 0;
    m14DC = 0;
    m590 = lbl_8087F048->fn_800F8548();
}

void Em202ForestSpider::vf138() {
    fn_8027688C();
    m58C = 6;
    fn_8016E970(3);
    m18F0 = 0;
    m14F8 = 0;
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

void Em202ForestSpider::vf164() {
    fn_8027688C();
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    mMotion.fn_80097CCC(0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    if (m18EC != 0) {
        m18EC->fn_8015C6A0();
        m18EC = 0;
        for (int i = 0; i < 4; i++) {
            fn_802790A0(i);
        }
    }
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1914, &mMotion);
    vf9C(1);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

extern "C" void fn_80276AE4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, const atn::Vector3& pos,
                            const atn::Vector3& rot, int a5, int a6, int a7, float scale) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, a5, a6, a7, scale);
}

void Em202ForestSpider::vf140() {
    if (vf154(8) == 0) {
        fn_8027688C();
        m58C = 8;
        m14E4++;
        mMotion.fn_80097CCC(0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    }
}

void Em202ForestSpider::vf13C() {
    if (vf154(7) == 0) {
        fn_8027688C();
        m58C = 7;
        m14E4++;
        mMotion.fn_80097CCC(0, 0.0f);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    }
}

void Em202ForestSpider::vf144() {
    fn_8027688C();
    m58C = 9;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x14B, 1, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = mMotion.fn_80097D7C(0) / 60.0f;
    m18E8 = vf174();
}

CharaTask* Em202ForestSpider::vf174() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m5694 != 0) {
        return m14D4;
    }
    int ids[5] = {5, 2, 4, 0, -1};
    CharaTask* found = 0;
    for (int* p = ids; found == 0 && *p >= 0; p++) {
        found = lbl_8087F8A0->fn_8054A33C(*p);
        if (found == 0 || !found->IsActive_unk()) {
            found = 0;
        }
    }
    if (found == 0) {
        found = m14D4;
    }
    return found;
}

void Em202ForestSpider::vf148() {
    fn_8027688C();
    m58C = 10;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x147, 1, 0, 1, 0.0f, 0.1f);
    m18F0 = m1528;
}

void Em202ForestSpider::vf14C() {
    fn_8027688C();
    m58C = 11;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 d = m18EC->mPos - mPos;
    PSVECNormalize(&d, &d);
    atn::Vector3 r = d.ToRotation_unk();
    r.z = 0.0f;
    m18F0 = 0;
    mRot = r;
    fn_80239DAC(lbl_8087F3C0, &m1538[0], 103, 1);
    atn::Vector3 off = d * 45.0f;
    m18EC->mPos = mPos + off;
    atn::Vector3 r2 = off.ToRotation_unk();
    r2.z = 0.0f;
    m18EC->mRot = r2;
    m18EC->fn_8015C3A8((int)this);
    lbl_8087F430->fn_80370AE4(0x79, m18EC->mType == 0 ? 10 : m18EC->m58);
    if (m18EC->mType == 0) {
        lbl_8087F430->fn_80370C6C();
    }
}

void Em202ForestSpider::vf150() {
    fn_8027688C();
    m58C = 12;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x33, 0, 0, 1, 0.0f, 0.1f);
}

int Em202ForestSpider::vf154(int arg) {
    if (m14D4 == 0) {
        return 0;
    }
    if (m58C == 16) {
        return 0;
    }
    m14F0 = mRot.y;
    m14F4 = (m14D4->mPos - mPos).ToRotationZ_unk().y;
    float a = atn::WrapAngle_unk(m14F4 - m14F0);
    if (-0.3926991f < a && a < 0.3926991f) {
        return 0;
    }
    m14EC = 0;
    m14E8 = arg;
    m14D0 = m14D4;
    fn_8027688C();
    m58C = 16;
    mMotion.fn_80097CCC(0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    return 1;
}

void Em202ForestSpider::vf158() {
    fn_8027688C();
    m58C = 13;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x14B, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 1.5f;
}

void Em202ForestSpider::vf15C() {
    fn_8027688C();
    m58C = 14;
    m14F8 = 1;
    fn_8017039C(m14BC, 2);
}

void Em202ForestSpider::vf160() {
    fn_8027688C();
    m58C = 15;
    m14F8 = 1;
    m14CC = vf170();
    if (m14CC != 0) {
        fn_8017039C(m14CC->mId, 2);
    } else if (vf154(9) == 0) {
        vf144();
    }
}

// inferred: effect `efp` on the model at its origin (as PlayEffectM_unk of game/em005_doppel_ganger.cpp)
inline void PlayEffectOrigin_unk(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model) {
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
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

void Em202ForestSpider::vf16C() {
    fn_8027688C();
    m58C = 18;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 0.0f;
    mMotion.fn_80097C08(0, 2, 1, 1, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 200);
    PlayEffectOrigin_unk(lbl_8087F3C0, &m192C, &mMotion);
    BattleTask* mgr = lbl_8087F048;
    atn::Vector3 p = fn_801781B0();
    mgr->fn_80108F38(&p, 0x80);
}

void Em202ForestSpider::vf130() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        vf138();
    }
}

void Em202ForestSpider::vf168() {
    fn_8027688C();
    m58C = 17;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}

int Em202ForestSpider::fn_80277B84(CharaTask* chara) {
    for (unsigned int i = 0; i < m1908.size(); i++) {
        if (m1908[i] == chara) {
            return 1;
        }
    }
    return 0;
}

// inferred: ground points of the arms
static const char* sArmNodes_unk[6] = {"leftbackarm2",  "leftmiddlearm2",  "leftfrontarm2",
                                       "rightbackarm2", "rightmiddlearm2", "rightfrontarm2"};

// NONMATCHING: 99.35% - stack slots of the arm position and of the ray end (swapped with their registers)
void Em202ForestSpider::fn_80277BC0() {
    const atn::Vector3& origin = mPos;
    int played = 0;
    for (int i = 0; i < 6; i++) {
        MtxPtr m = mMotion.GetNodeMtx_unk(sArmNodes_unk[i]);
        atn::Vector3 pos(m[0][3], m[1][3], m[2][3]);
        atn::Vector3 to = pos;
        to -= origin;
        to.x *= 1.3f;
        to.z *= 1.3f;
        to += origin;
        to.y = mPos.y;
        atn::ColliResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &origin, &to, 0x80000000, 0, 0) != 0) {
            if (!played && fn_800C3118(sSeGroup_unk) != 0) {
                fn_800C344C(sSe_unk, result.m10, 0, -1, 1.0f);
                played = 1;
            }
            fn_80232B7C(0, 0);
            Color4f_unk color;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &m1920, 0, 0, result.m10, lbl_807C7030, color, -1, -1, 1, 1.0f);
        }
    }
}

// inferred: bit 25 of the character flags read through a const accessor (reloaded before the clear, as in
// game/Em302Kraken.cpp); name invented
inline unsigned int IsFlag25_unk(const CharaTask* chara) {
    return chara->mFlag25;
}

// NONMATCHING: 99.57% - float registers of the hoisted constants and stack slots of the rotation temporaries
void Em202ForestSpider::fn_80277DC4() {
    int count = 0;
    atn::Vector3 off(0.0f, 0.0f, 40.0f);
    for (unsigned int i = 0; i < m14B0.size(); i++) {
        EnemyTask* e = m14B0[i];
        if (e != 0 && (e->mStatus.IsState_unk(0x20) || e->mC58.mC0 == 0)) {
            atn::Matrix3x4 m;
            m.SetRotationY_unk(1.5707964f);
            PSMTXMultVec(m.m, &off, &off);
            atn::Vector3 pos = mPos + off;
            atn::Vector3 r = off.ToRotation_unk();
            atn::Vector3 rot(0.0f, r.y, 0.0f);
            e->mFlag15 = 0;
            e->mFlags &= ~4;
            e->fn_80176ACC();
            e->fn_800D246C(0);
            if (IsFlag25_unk(e)) {
                e->mFlag25 = 0;
            }
            e->mStatus.fn_8012D8B8();
            e->m58C = 0;
            e->fn_8015495C(0, 1, 1, 1);
            e->mPos = pos;
            e->mRot = rot;
            e->fn_80145334();
            e->mC58.mC0 = 1;
            e->vf80(pos, off);
            count++;
        }
    }
    if (count > 0) {
        wchar_t text[256];
        memset(text, 0, sizeof(text));
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0xB9), m60->m04);
        lbl_8087F048->fn_80109828(text);
    }
}

__declspec(noinline) int Em202ForestSpider::fn_8027819C() {
    unsigned int alive = 0;
    if (m14B0.size() == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < m14B0.size(); i++) {
        EnemyTask* e = m14B0[i];
        if (e != 0 && !e->mStatus.IsState_unk(0x20) && e->mC58.mC0 != 0) {
            alive++;
        }
    }
    return alive <= 1;
}

// NONMATCHING: 72.14% - the original sets the vector to zero (lbl_807C7030) before the head position and
// scales each component (100% with both), but then vf170 calls it out of line (inline budget): kept as is
void Em202ForestSpider::fn_80278218(atn::Vector3& center) {
    int n = 1;
    center = lbl_8087F8A0->mHead->mPos;
    for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
        if (!c->mStatus.IsState_unk(0x20) && c->mC58.mC0 != 0) {
            n++;
            center += c->mPos;
        }
    }
    center *= 1.0f / (float)n;
}

gs::WarpPoint* Em202ForestSpider::vf170() {
    atn::Vector3 center;
    fn_80278218(center);
    gs::WarpPoint* best = 0;
    float bestDist = 0.0f;
    for (unsigned int i = 0; i < m14C0.size(); i++) {
        atn::Vector3 d = center - m14C0[i]->mPos;
        float dist = PSVECSquareMag(&d);
        if (bestDist < dist) {
            bestDist = dist;
            best = m14C0[i];
        }
    }
    return best;
}

int Em202ForestSpider::fn_8027851C(CharaTask* chara) {
    atn::Vector3 from = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    atn::Vector3 to = ((atn::Matrix3x4*)chara->GetModel_unk()->GetNodeMtx_unk("waist"))->GetTrans_unk();
    atn::ColliResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, &mColli, 0) != 0) {
        return 0;
    }
    return 1;
}

// NONMATCHING: 98.25% - stack slots of the thread vectors and of the rotation temporaries (shoots the thread
// `index` at chara; flow, calls and constants checked against the original)
void Em202ForestSpider::fn_80278634(CharaTask* chara, int index, int main) {
    SpiderThread_unk* t = &m1538[index];
    t->m00 = 1;
    t->m0C = main;
    fn_80239DAC(lbl_8087F3C0, t, 100, 1);
    fn_80239DAC(lbl_8087F3C0, t, 101, 1);
    fn_80239DAC(lbl_8087F3C0, t, 103, 0);
    t->m04 = 0;
    t->m08 = 30;
    atn::Vector3 head = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    atn::Vector3 waist = ((atn::Matrix3x4*)chara->GetModel_unk()->GetNodeMtx_unk("waist"))->GetTrans_unk();
    t->m88 = chara;
    t->m10 = waist - head;
    PSVECNormalize(&t->m10, &t->m10);
    t->m10 *= 60.0f;
    t->m28.SetIdentity();
    atn::Vector3 r = t->m10.ToRotation_unk();
    r.z = 0.0f;
    t->m28.RotateZYX_unk(r);
    t->m28.fn_801479E4(head);
    atn::Vector3 d = t->m10;
    PSVECNormalize(&d, &d);
    atn::Vector3 r2 = d;
    r2.fn_80139F60();
    r2.z = 0.0f;
    float scale = PSVECMag(&d) / 50.0f;
    t->m58.SetIdentity();
    t->m58.fn_801479E4(head);
    t->m58.RotateZYX_unk(r2);
    atn::Vector3 sv(1.0f, 1.0f, scale);
    Mtx sm;
    PSMTXScale(sm, sv.x, sv.y, sv.z);
    atn::Matrix3x4 st;
    PSMTXConcat(t->m58.m, sm, st.m);
    t->m58 = st;
    fn_80232B7C(t, 0x65);
    fn_8023A680(lbl_8087F3C0, &t->mB0, -1, 5, &t->m58, 0, 0, 0, 0, -1, 1, 1.0f);
}

void Em202ForestSpider::fn_80279054() {
    for (int i = 0; i < 4; i++) {
        fn_802790A0(i);
    }
}

void Em202ForestSpider::fn_802790A0(int index) {
    SpiderThread_unk* t = &m1538[index];
    t->m00 = 0;
    t->m0C = 0;
    fn_80239DAC(lbl_8087F3C0, t, 100, 1);
    fn_80239DAC(lbl_8087F3C0, t, 101, 1);
    fn_80239DAC(lbl_8087F3C0, t, 102, 0);
    fn_80239DAC(lbl_8087F3C0, t, 103, 0);
    if (t->m04 != 0) {
        if (t->m88 != 0) {
            if (t->m88 == lbl_8087F8A0->mHead && lbl_8087F430->fn_80370A78(0x73) == 0) {
                lbl_8087F430->fn_80370AE4(0x73, 1);
            }
            if (t->m88->m55C == 6 && t->m88->m560 - 0x74U <= 1) {
                t->m88->fn_8015C6A0();
            }
            if (!t->m88->mFlagB9) {
                fn_80232B7C(0, 0);
                atn::Model* model = t->m88->GetModel_unk();
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
                fn_8023A8B4(lbl_8087F3C0, &t->mE0, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
                fn_80232B7C(0, 0);
                fn_8023A680(lbl_8087F3C0, &t->mBC, -1, 5, &t->m58, 0, 0, 0, 0, -1, 1, 1.0f);
            }
        }
    }
    t->m04 = 0;
    t->m88 = 0;
}

// inferred: accessors of the gimmick list (weak copies 0x80279788-0x802797D4, emitted after fn_80279280 that calls
// them out of line; the GimmickMsg_unk constructor is the one at 0x802797A8)
extern "C" inline GimmickCommon* fn_80279788(GimmickManager* manager) {
    return manager->mHead;
}
extern "C" inline GimmickManager* fn_80279790() {
    return GimmickManager::instance_;
}
extern "C" inline int fn_80279798(GimmickCommon* gimmick) {
    return gimmick->m50;
}
extern "C" inline atn::Vector3* fn_802797A0(GimmickCommon* gimmick) {
    return &gimmick->m6C;
}
extern "C" inline GimmickCommon* fn_802797D4(GimmickCommon* gimmick) {
    return gimmick->m5C;
}
// inferred: asserts of the debug build, empty in release: with them fn_80279280 goes over the inline budget as the
// original (docs/inlining.md R3)
#define SPIDER_ASSERT_UNK(x) ((void)0)
#define SPIDER_ASSERT10_UNK(x)                                                                                 \
    SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x); \
    SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x); SPIDER_ASSERT_UNK(x)
#define SPIDER_ASSERT100_UNK(x)                                                                                \
    SPIDER_ASSERT10_UNK(x); SPIDER_ASSERT10_UNK(x); SPIDER_ASSERT10_UNK(x); SPIDER_ASSERT10_UNK(x);              \
    SPIDER_ASSERT10_UNK(x); SPIDER_ASSERT10_UNK(x); SPIDER_ASSERT10_UNK(x); SPIDER_ASSERT10_UNK(x);              \
    SPIDER_ASSERT10_UNK(x); SPIDER_ASSERT10_UNK(x)
// inferred: state and sub-state of a character (weak copies 0x80267B20/0x80267B28 of another file)
extern "C" inline int fn_80267B20(CharaTask* chara) {
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    SPIDER_ASSERT100_UNK(chara != 0);
    return chara->m55C;
}
extern "C" inline int fn_80267B28(CharaTask* chara) {
    return chara->m560;
}
// inferred: weak copies of other files called out of line below
extern "C" atn::EffectManager* fn_800F7FA0();
extern "C" void fn_800F7FA8(atn::EffectManager* mgr, void* owner, int arg);
extern "C" void fn_8026607C(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg);
extern "C" void fn_800F80B8(atn::Matrix3x4* mtx, const atn::Vector3* rot);
extern "C" void fn_800F8290(atn::Matrix3x4* mtx, float x, float y, float z);
void* fn_801404F8(); // inferred: collision world
extern "C" void fn_8004D124(void* world); // inferred: refreshes the collision world
extern "C" float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b); // inferred: dot product
extern "C" int fn_801A03F4(atn::ColliBase* poly);           // inferred: the polygon belongs to a character
extern "C" CharaTask* fn_801A0400(atn::ColliBase* poly);    // inferred: its character

// inferred: updates the four threads: a caught character drags the tip; a free tip flies on, breaks the webs of
// the gimmicks of type 1 it crosses and catches the character it hits; then the thread is stretched from the head
// to the tip. Over the inline budget as the original (every inline is called out of line)
void Em202ForestSpider::fn_80279280() {
    for (int i = 0; i < 4; i++) {
        SpiderThread_unk* t = &m1538[i];
        if (t->m00 == 0) {
            continue;
        }
        if (t->m04 != 0) {
            if (t->m88 != 0) {
                atn::Matrix3x4 spine(t->m88->GetModel_unk()->GetNodeMtx_unk("spine"));
                t->m28 = spine;
                int held = 0;
                if (t->m0C == 0) {
                    if (fn_80267B20(t->m88) == 6 &&
                        (fn_80267B28(t->m88) == 0x74 || fn_80267B28(t->m88) == 0x75)) {
                        held = 1;
                    }
                    if (held == 0) {
                        fn_802790A0(i);
                    }
                }
            }
        } else {
            if (--t->m08 < 0) {
                fn_802790A0(i);
                return;
            }
            atn::Vector3 from = t->m28.GetTrans_unk();
            atn::Vector3 to = from + t->m10;
            for (GimmickCommon* g = fn_80279788(fn_80279790()); g != 0; g = fn_802797D4(g)) {
                if (fn_80279798(g) == 1) {
                    atn::Vector3 a(from);
                    a.y = 0.0f;
                    atn::Vector3 b(to);
                    b.y = 0.0f;
                    atn::Vector3 c(*fn_802797A0(g));
                    c.y = 0.0f;
                    atn::Vector3 ab = b - a;
                    float lsq = ab.LengthSq_unk();
                    float s = -fn_801A03E8(ab, a - c) / lsq;
                    if (0.0f < s && s < 1.0f) {
                        atn::Vector3 p = a + ab * s;
                        float radius = g->vf58();
                        if ((p - c).Length_unk() < radius && g->vf5C()) {
                            GimmickMsg_unk msg;
                            msg.mType = 4;
                            g->vf40(&msg);
                            fn_8004D124(fn_801404F8());
                        }
                    }
                }
            }
            atn::ColliResult_unk result;
            if (fn_8004ED34(fn_801404F8(), &result, &from, &to, m1900, &mColli, 0) != 0) {
                if (result.m38 != 0 && fn_801A03F4(result.m38)) {
                    if (fn_801A0400(result.m38) == t->m88) {
                        t->m28.fn_801479E4(result.m10);
                        fn_80239DAC(fn_800F7FA0(), t, 100, 1);
                        t->m04 = 1;
                        t->m88->fn_8015C0C0((int)this, t->m0C);
                        fn_800C344C("SE_EM202_012", t->m28.GetTrans_unk(), 0, -1, 1.0f);
                        fn_800F7FA8(fn_800F7FA0(), 0, 0);
                        fn_80276AE4(fn_800F7FA0(), &t->mA4, 0, t->m28.GetTrans_unk(), t->m10.ToRotation_unk(), -1, -1,
                                    1,
                                    1.0f);
                        fn_800F7FA8(fn_800F7FA0(), t, 0x66);
                        fn_8026607C(fn_800F7FA0(), &t->mC8, t->m88->GetModel_unk(), 1);
                        fn_800F7FA8(fn_800F7FA0(), t, 0x67);
                        fn_8026607C(fn_800F7FA0(), &t->mD4, t->m88->GetModel_unk(), 1);
                    } else {
                        t->m28.fn_801479E4(to);
                    }
                } else {
                    t->m88 = 0;
                    fn_802790A0(i);
                }
            } else {
                t->m28.fn_801479E4(to);
            }
        }
        atn::Vector3 head = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
        atn::Vector3 d = t->m28.GetTrans_unk() - head;
        atn::Vector3 rot = d.ToRotation_unk();
        float scale = d.Length_unk() / 50.0f;
        t->m58.SetIdentity();
        t->m58.fn_801479E4(head);
        fn_800F80B8(&t->m58, &rot);
        fn_800F8290(&t->m58, 1.0f, 1.0f, scale);
    }
}

void Em202ForestSpider::fn_802797DC() {
    fn_80087994(m1960.Load("em202_FSpider"), "DebugDisp", &m1964, 0, 0);
}
