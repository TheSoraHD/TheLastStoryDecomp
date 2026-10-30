#include "game/Em202ForestSpider.h"
#include "atn/Csv.h"
#include "atn/Material_unk.h"
#include "game/CharaStatus.h"
#include "chr/ControlBase.h"
#include "game/EnemyManager.h"
#include "game/PlayerManager.h"
#include "game/FriendManager.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" double fn_800DC288(const char* s); // inferred: string to float
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);

// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em202Spider.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
    unsigned char m10DC[0x5694 - 0x10DC];
    int m5694; // inferred: nonzero in the multiplayer mode (the threads go for the target)
};
extern "C" Field_unk* lbl_8087F430;
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
// inferred: result of a ray test (0x50 bytes, as in game/Em202Spider.cpp)
struct CollPoly_unk {
    unsigned char m00[0x08];
    unsigned int m08;
};
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: hit position
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    CollPoly_unk* m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" int fn_800C3118(const char* group); // inferred: nonzero when the sound group is loaded
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" double sqrt(double x);
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" HitInfo_unk* fn_80219E6C(int id);  // inferred: skill data
// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (as in game/Em202Spider.cpp)
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80097CCC(atn::Model* model, int track, float value);       // inferred: stops a track
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3* pos, int arg);
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* eff, int a, int b, const atn::Matrix3x4* mtx, int c,
                            int d, int e, float scale); // inferred: effect on a matrix
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4); // debug menu
// inferred: effect `efp` on the model (shared with gmk/FlamingArrow, Em315Coffin and the Quark enemies)
extern "C" inline void fn_80276AE4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, const atn::Vector3* pos,
                                   const atn::Vector3* rot, int a5, int a6, int a7, float scale) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, *pos, *rot, color, a5, a6, a7, scale);
}
// inferred: sounds of the spider (pointers kept in .sdata2)
static const char* const sSe_unk = "SE_DG001_014";
static const char* const sSeGroup_unk = "GROUP_DG001";
// inferred: color of the spiders when they are down (game/Em202SpiderBase.cpp)
extern "C" SpiderBaseColor_unk lbl_807C8398;

// inferred: front direction of the character (name invented)
static inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(rot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}

// inferred: the character can be targeted (CanAct_unk with the flags reloaded; name invented)
static inline bool IsTargetable_unk(CharaTask* c) {
    bool active = false;
    bool free = false;
    bool ok = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        active = true;
    }
    if (active && (c->mStatus.m0C & 0x20) != 0x20) {
        free = true;
    }
    if (free) {
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

// NONMATCHING: written for the order of the strings (std::string and Csv inlined)
Em202ForestSpider::Em202ForestSpider(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14D4(0), m14DC(0), m14E0(0), m14E4(0), m14E8(0), m14EC(0), m14F0(0.0f),
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

int Em202ForestSpider::vf0C() {
    int loading = 0;
    for (int i = 0; i < 4; i++) {
        SpiderThread_unk* t = &m1538[i];
        if (t->m8C.IsLoading() || t->m98.IsLoading() || t->mA4.IsLoading() || t->mB0.IsLoading() ||
            t->mBC.IsLoading() || t->mC8.IsLoading() || t->mD4.IsLoading() || t->mE0.IsLoading()) {
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
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
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


// NONMATCHING: approximate
void Em202ForestSpider::vf10() {
    vfF8();
    if (mStatus.IsState_unk(4)) {
        CharaTask* player = lbl_8087F8A0->fn_8054A33C((const char*)5);
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
                            a.y = 0.0f;
                            atn::Vector3 b(hm[0][3], 0.0f, hm[2][3]);
                            atn::ColliCapsuleShape_unk seg(a, b, 0.0f);
                            atn::ColliCapsuleShape_unk cap(head->mPos, head->mPos, radius);
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
    mColli.m48 = mPos;
    mColli.m54 = m5B0;
    m1500++;
    if (m1500 > 10 && (m58C == 6 || m58C == 16)) {
        fn_80277BC0();
        m1500 = 0;
    }
    atn::Vector3 pos(mPos.x, mPos.y + m5B0, mPos.z);
    atn::Vector3 fwd = FrontDir_unk(mRot);
    fn_805A5078(&pos, &fwd, 1000, 1.2f * m5B0, 3.1415927f);
}

// NONMATCHING: approximate (as Em202Spider::vf14)
void Em202ForestSpider::vf14() {
    SpiderBaseColor_unk target = m1950;
    bool down = true;
    if (!mStatus.IsState_unk(0x80000) && !mStatus.IsState_unk(0x1000000)) {
        down = false;
    }
    if (down) {
        target = lbl_807C8398;
    }
    SpiderBaseColor_unk d(target.r - m1940.r, target.g - m1940.g, target.b - m1940.b, target.a - m1940.a);
    m1940.r += d.r * 0.15f;
    m1940.g += d.g * 0.15f;
    m1940.b += d.b * 0.15f;
    m1940.a += d.a * 0.15f;
    GXColor c;
    c.r = (unsigned char)(255.0f * m1940.r);
    c.g = (unsigned char)(255.0f * m1940.g);
    c.b = (unsigned char)(255.0f * m1940.b);
    c.a = (unsigned char)(255.0f * m1940.a);
    mMotion.fn_80092954("em202_body")->mColor = c;
    c.r = (unsigned char)(255.0f * m1940.r);
    c.g = (unsigned char)(255.0f * m1940.g);
    c.b = (unsigned char)(255.0f * m1940.b);
    c.a = (unsigned char)(255.0f * m1940.a);
    mMotion.fn_80092954("em202_tail")->mColor = c;
    fn_80149A30();
}

void Em202ForestSpider::vf3C(HitInfo_unk* hit) {
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
            atn::Vector3 fwd = FrontDir_unk(mRot);
            atn::Vector3 d(msg->m1C - mPos);
            float dot = PSVECDotProduct(&d, &fwd);
            if (dot > (float)cos(m1514 * 0.5f)) {
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
        m624.mData[0].m04.mRadius = 1.5f * m5B0;
        m624.mData[0].m04.mCenter = mPos + m5A4;
        m624.mData[0].m04.mCenter.y += 0.5f * m624.mData[0].m04.mRadius;
    }
}

void Em202ForestSpider::vfF8() {
    mC58.mC8 = (int)mC58.mC4;
    if (m58C - 9U <= 2) {
        m1454 = 0;
        m14D4 = m18E8;
    } else if (m58C >= 15 && m58C <= 16) {
        m1454 = 0;
    } else {
        CharaTask* c = lbl_8087F8A0->mHead;
        if (c->mFlag28) {
            m1454 = 0;
            m14D4 = c;
        } else {
            CharaTask* found = 0;
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
    }
    mC58.mC4 = m14D4;
}

// NONMATCHING: approximate
void Em202ForestSpider::vfFC() {
    float speed = 1.5f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 dir = mRoute.m58;
        if (PSVECMag(&dir) > 0.0001f) {
            PSVECNormalize(&dir, &dir);
            rot = dir.ToRotation_unk();
            rot.x = 0.0f;
            rot.z = 0.0f;
        }
        atn::Vector3 d(m14D4->mPos.x - mPos.x, m14D4->mPos.y - mPos.y, m14D4->mPos.z - mPos.z);
        d.y = 0.0f;
        float dist = PSVECMag(&d);
        PSVECNormalize(&d, &d);
        atn::Vector3 r = d.ToRotation_unk();
        mRot.y = r.y;
        if (m14F8 != 0) {
            speed = 1.5f * 2.5f;
        } else if (dist < m1510) {
            speed = 0.0f;
        }
        fn_801426A4(&rot, 0.0f, m568 * speed);
    }
    if (m55C == 6) {
        fn_8013A258();
    }
    fn_8013CB68(rot, 0, 0.0f, m568 * 1.5f);
}

void Em202ForestSpider::vf100() {
    if (m14D4 != 0) {
        if (m14FC > 0) {
            m14FC--;
            if (m14FC == 0) {
                if (m14BC != 0) {
                    vf15C();
                }
                vf158();
            }
        }
        if (fn_8027819C() != 0) {
            m14FC = m152C;
        }
        if (m14DC >= m1524) {
            CharaTask* t = m14D4;
            atn::Vector3 d(t->mPos.x - mPos.x, t->mPos.y - mPos.y, t->mPos.z - mPos.z);
            float dist = sqrt(d.x * d.x + d.z * d.z);
            if (m14E0 <= 0 && !mStatus.IsState_unk(4)) {
                vf160();
            }
            if (dist <= m1504) {
                if (m14E4 % 5 == 0) {
                    vf13C();
                }
                vf140();
            }
        }
    }
}

void Em202ForestSpider::vf104() {
    if (m55C != 6) {
        if (m55C == 7) {
            if (m14D4 != mRoute.mTarget) {
                if (vf154(6) == 0) {
                    m58C = 6;
                    fn_8016E970(3);
                }
            }
        } else if (vf154(6) == 0) {
            m58C = 6;
            fn_8016E970(3);
            fn_80170A20(m14D4, 0, -1.0f);
        }
    } else {
        vfFC();
    }
}

void Em202ForestSpider::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
        }
        for (unsigned int i = 0; i < m1908.size(); i++) {
            CharaTask* c = m1908[i];
            c->fn_80176ACC();
            c->mPos.z = mPos.z;
            c->mF1C = 0;
            c->mFlagB9 = 0;
            c->mMotion.fn_80097C08(0, 0x32, 1, 0, 1, 1.0f, 0.1f);
            c->fn_8015EFAC(10);
        }
        mFlag15 = 1;
        fn_801765D8();
        fn_800EE360(this);
        fn_800EB7A0(this);
    }
    m1434 = 5;
}

void Em202ForestSpider::vf108() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        vf138();
    }
    float f = mMotion.mTracks[0].m08;
    if (f >= 17.0f && f <= 22.0f) {
        int id = m590;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(m1508), id, 30, -1, 0.0f);
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
        fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(m150C), id, 30, -1, 0.0f);
    }
}

// NONMATCHING: not written yet (state 9, threads)
void Em202ForestSpider::vf110() {
    if (m14D8 == 0) {
        atn::Vector3 d = m18E8->mPos - mPos;
        PSVECNormalize(&d, &d);
        mRot = d.ToRotation_unk();
        mRot.z = 0.0f;
        if (m14DC > 30) {
            fn_80097CCC(&mMotion, 0, 0.0f);
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
            vf148();
            return;
        }
        m14E0 = 600;
        for (int i = 0; i < 4; i++) {
            fn_802790A0(i);
        }
        vf138();
    }
}

// NONMATCHING: not written yet (state 10)
void Em202ForestSpider::vf114() {
    if (m18EC == 0) {
        vf138();
    }
}

// NONMATCHING: not written yet (state 11)
void Em202ForestSpider::vf118() {
    if (m18EC == 0) {
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
        if (f >= 5.0f && f <= 16.0f) {
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

void Em202ForestSpider::vf128() {
    RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(m14BC);
    atn::Vector3 d(mPos.x - place->mPos.x, 0.0f, mPos.z - place->mPos.z);
    if (PSVECSquareMag(&d) < 100.0f) {
        m14F8 = 0;
        mRot.y = place->m14;
        vf158();
    }
    vfFC();
}

void Em202ForestSpider::vf12C() {
    RoutePlace_unk* place = m14CC;
    atn::Vector3 d(mPos.x - place->mPos.x, 0.0f, mPos.z - place->mPos.z);
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
    m590 = fn_800F8548(lbl_8087F048);
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
    fn_80097CCC(&mMotion, 0, 0.0f);
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
    fn_80276AE4(lbl_8087F3C0, &m1914, &mMotion, &lbl_807C7030, &lbl_807C7030, -1, -1, 1, 1.0f);
    vf9C(1);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

void Em202ForestSpider::vf140() {
    if (vf154(8) == 0) {
        fn_8027688C();
        m58C = 8;
        m14E4++;
        fn_80097CCC(&mMotion, 0, 0.0f);
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
        fn_80097CCC(&mMotion, 0, 0.0f);
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
    static const int sIds[5] = {5, 2, 4, 0, -1};
    int ids[5] = {sIds[0], sIds[1], sIds[2], sIds[3], sIds[4]};
    CharaTask* found = 0;
    for (int* p = ids; found == 0 && *p >= 0; p++) {
        found = lbl_8087F8A0->fn_8054A33C((const char*)*p);
        if (found != 0 && !found->IsActive_unk()) {
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

// NONMATCHING: not written yet (starts state 11)
void Em202ForestSpider::vf14C() {
    fn_8027688C();
    m58C = 11;
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
    atn::Vector3 d = m14D4->mPos - mPos;
    atn::Vector3 r = d.ToRotation_unk();
    m14F4 = r.y;
    float a = atn::WrapAngle_unk(m14F4 - m14F0);
    if (a > -0.3926991f && a < 0.3926991f) {
        return 0;
    }
    m14EC = 0;
    m14E8 = arg;
    m14D0 = m14D4;
    fn_8027688C();
    m58C = 16;
    fn_80097CCC(&mMotion, 0, 0.0f);
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
    }
    if (vf154(9) == 0) {
        vf144();
    }
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
    fn_8023A8B4(lbl_8087F3C0, &m192C, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
    Effect_unk* mgr = lbl_8087F048;
    atn::Vector3 p = fn_801781B0();
    fn_80108F38(mgr, &p, 0x80);
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

void Em202ForestSpider::fn_80277BC0() {
    int played = 0;
    const atn::Vector3& origin = mPos;
    for (int i = 0; i < 6; i++) {
        MtxPtr m = mMotion.GetNodeMtx_unk(sArmNodes_unk[i]);
        atn::Vector3 to;
        atn::Vector3 pos(m[0][3], m[1][3], m[2][3]);
        to = pos;
        to -= mPos;
        to.x *= 1.3f;
        to.z *= 1.3f;
        to += mPos;
        to.y = mPos.y;
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &origin, &to, 0x80000000, 0, 0) != 0) {
            if (!played && fn_800C3118(sSeGroup_unk) != 0) {
                fn_800C344C(sSe_unk, &result.m10, 0, -1, 1.0f);
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

// NONMATCHING: not written yet (brings back the reinforcements)
void Em202ForestSpider::fn_80277DC4() {}

int Em202ForestSpider::fn_8027819C() {
    int alive = 0;
    for (unsigned int i = 0; i < m14B0.size(); i++) {
        EnemyTask* e = m14B0[i];
        if (e != 0 && !e->mStatus.IsState_unk(0x20) && e->mC58.mC0 != 0) {
            alive++;
        }
    }
    return alive <= 1;
}

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

RoutePlace_unk* Em202ForestSpider::vf170() {
    atn::Vector3 center;
    fn_80278218(center);
    RoutePlace_unk* best = 0;
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
    MtxPtr hm = mMotion.GetNodeMtx_unk("head");
    atn::Vector3 from(hm[0][3], hm[1][3], hm[2][3]);
    MtxPtr wm = chara->mMotion.GetNodeMtx_unk("waist");
    atn::Vector3 to(wm[0][3], wm[1][3], wm[2][3]);
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, &mColli, 0) != 0) {
        return 0;
    }
    return 1;
}

// NONMATCHING: not written yet (shoots a thread)
void Em202ForestSpider::fn_80278634(CharaTask* chara, int index, int main) {}

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
            if (t->m88 == lbl_8087F8A0->mHead && fn_80370A78(lbl_8087F430, 0x73) == 0) {
                fn_80370AE4(lbl_8087F430, 0x73, 1);
            }
            if (t->m88->m55C == 6 && t->m88->m560 - 0x74U <= 1) {
                t->m88->fn_8015C6A0();
            }
            if (!t->m88->mFlagB9) {
                fn_80232B7C(0, 0);
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
                fn_8023A8B4(lbl_8087F3C0, &t->mE0, &t->m88->mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
                fn_80232B7C(0, 0);
                fn_8023A680(lbl_8087F3C0, &t->mBC, -1, 5, &t->m58, 0, 0, 0, 1.0f);
            }
        }
    }
    t->m04 = 0;
    t->m88 = 0;
}

// NONMATCHING: not written yet (moves the threads)
void Em202ForestSpider::fn_80279280() {}

void Em202ForestSpider::fn_802797DC() {
    fn_80087994(m1960.Load("em202_FSpider"), "DebugDisp", &m1964, 0, 0);
}
