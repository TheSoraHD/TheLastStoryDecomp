#include "game/Em202MothersSpider.h"
#include "atn/Csv.h"
#include "game/CharaStatus.h"
#include "chr/ControlBase.h"
#include "game/EnemyManager.h"
#include "game/PlayerManager.h"
#include "math/Vec.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" double fn_800DC288(const char* s); // inferred: string to float
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v);
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);

// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em202ForestSpider.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
    unsigned char m10DC[0x5694 - 0x10DC];
    int m5694;
};
extern "C" Field_unk* lbl_8087F430;
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
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" int fn_8021A888(HitInfo_unk* hit);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (as in game/Em202Spider.cpp)
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag

// inferred: effect `efp` on a model with no offset (name invented, as in game/Em202ForestSpider.cpp)
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

// inferred: front direction of the character (name invented)
static inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(rot.y);
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

Em202MothersSpider::Em202MothersSpider(atn::Task* parent, EnemyInfo_unk* info)
    : Em202ForestSpider(parent, info), m1968(0), m19A8(0.05f), m19AC(5), m19B0(300) {
    m199C = 0;
    m18FC = 3.0f;
    m1900 = 0;
    m19A4 = 0;
    m1950 = SpiderBaseColor_unk(0.945098f, 0.7137255f, 0.3176471f, 1.0f);
    m14E0 = 450;
    m19B4.LoadRequest("/effect/ef_sk002d.efp");
}

Em202MothersSpider::~Em202MothersSpider() {}

// NONMATCHING: 93% - inlined push_back and register allocation (as Em202ForestSpider::vf0C)
int Em202MothersSpider::vf0C() {
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
        m1920.IsLoading() == 0 && m192C.IsLoading() == 0 && m19B4.IsLoading() == 0 &&
        m1938.fn_80473F50() == 0 && loading == 0) {
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
                } else if (strcmp(key, "DamageRate") == 0) {
                    m19A8 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpitOutNum") == 0) {
                    m19AC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpitOutTime") == 0) {
                    m19B0 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "ReinforceCount") == 0) {
                    m152C = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "ReinforceCenter") == 0) {
                    m14BC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpitOutPoint") == 0) {
                    m1968 = lbl_8087F430->m10D8->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
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
        mFlagB11 = 1;
        fn_802797DC();
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 92% - capsule test and collision temporaries (as Em202ForestSpider::vf10)
void Em202MothersSpider::vf10() {
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
                        m14E0 = 450;
                        fn_80279054();
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
        case 19:
            vf17C();
            break;
        case 20:
            vf184();
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
    for (unsigned int i = 0; i < m1908.size(); i++) {
        if (m1908[i]->fn_801789D8(0x9C, 0) == 0 && m1908[i]->fn_801789D8(0xA0, 0) == 0) {
            m1908[i]->mStatus.mParamB.m8C -= m19A8;
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
    atn::Vector3 pos = mPos;
    atn::Vector3 fwd = FrontDir_unk(mRot);
    fn_805A5078(&pos, &fwd, 1000, 1.2f * m5B0, 3.1415927f);
    atn::Matrix3x4 rm;
    rm.fn_8013A13C(mRot.y);
    m196C = rm;
    atn::Vector3 base = mPos;
    atn::Vector3 off(0.0f, 24.0f, -2.0f);
    PSMTXMultVec(m196C.m, &off, &off);
    base += off;
    m196C.m[0][3] = base.x;
    m196C.m[1][3] = base.y;
    m196C.m[2][3] = base.z;
}

// NONMATCHING: 99.3% - stack slots of fwd/d and operand order of m1514 * 0.5f (as Em202ForestSpider::vf3C)
void Em202MothersSpider::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (fn_8021A888(msg->mData) != 0 && msg->mAttacker->mType == 3 && fn_80277B84(msg->mAttacker) == 0 &&
        msg->mData->m74.m10 > 0.0f) {
        msg->m68 = 0;
        msg->m88 = msg->mData->mC4;
        msg->m84 = 1;
        msg->m90 = 0;
        return;
    }
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
    if (msg->mAttacker != 0 && msg->mAttacker->mFlagB9) {
        msg->m48 = 0;
        msg->m0C |= 0x800;
        msg->m0C |= 0x100000;
        if (msg->mData->m01 == 1) {
            msg->m10 = lbl_807C7030;
        }
    }
    float len = PSVECSquareMag(&msg->m10);
    if (!((float)__fabs(len) < 1.1920929e-7f) && len >= 25.0f) {
        PSVECNormalize(&msg->m10, &msg->m10);
        msg->m10 *= 4.9f;
    }
    if (m58C - 19U <= 1) {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em202MothersSpider::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m1908.size() != 0) {
        if (msg->mAttacker != 0 && msg->mAttacker->mFlagB9) {
            m19A4++;
            if (m199C == 0 && m19A4 > m19AC) {
                atn::Vector3 back = FrontDir_unk(mRot);
                atn::Matrix3x4 m;
                m.fn_8013A13C(3.1415927f);
                PSMTXMultVec(m.m, &back, &back);
                fn_8015E4B0(back, -1);
                m199C = 1;
            }
        }
        if (msg->mData->m01 == 1) {
            atn::Vector3 back = FrontDir_unk(mRot);
            atn::Matrix3x4 m;
            m.fn_8013A13C(3.1415927f);
            PSMTXMultVec(m.m, &back, &back);
            fn_8015E4B0(back, -1);
            m199C = 1;
        }
    }
    if (m58C == 18) {
        msg->m10 *= 0.0f;
    }
    if (mStatus.IsState_unk(0x20)) {
        vf164();
        fn_800CB5C8(&m19C0, 0, 0);
    }
}

int Em202MothersSpider::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (fn_8021A888(hit) != 0 && ((CharaTask*)attacker)->mType == 3 && fn_80277B84((CharaTask*)attacker) == 0 &&
        hit->m74.m10 > 0.0f) {
        return 3;
    }
    return 0;
}

void Em202MothersSpider::vfF8() {
    mC58.mC8 = (int)mC58.mC4;
    if (m58C - 9U <= 2) {
        m1454 = 0;
        m14D4 = m18E8;
    } else {
        m1454 = 1;
        m14D4 = mC58.mC4;
    }
    if (m14D4 == 0) {
        for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
            if (!c->mFlagB9) {
                m14D4 = c;
            }
        }
    }
    mC58.mC4 = m14D4;
}

// NONMATCHING: 95% - stack slots of the rotation temporaries
void Em202MothersSpider::vfFC() {
    float speed = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 dir = mRoute.m58;
        float mag = PSVECMag(&dir);
        if (mag > 0.0001f) {
            atn::Vector3 n = dir;
            PSVECNormalize(&n, &n);
            atn::Vector3 r0 = n.ToRotationZ_unk();
            r0.z = 0.0f;
            rot = r0;
        }
        atn::Vector3 d;
        d = m14D4->mPos - mPos;
        d.y = 0.0f;
        float dist = PSVECMag(&d);
        PSVECNormalize(&d, &d);
        d.fn_80139F60();
        mRot.y = d.y;
        if (m14F8 != 0) {
            speed *= 2.5f;
        } else if (dist < m1510) {
            speed = 0.0f;
        }
        fn_801426A4(&rot, mag, m568 * speed);
    }
    if (m55C == 6) {
        fn_8013A258();
    }
    fn_8013CB68(rot, 0, 0.0f, m568 * speed);
}

void Em202MothersSpider::vf100() {
    if (m1908.size() != 0) {
        if (m1908[0]->mStatus.IsState_unk(0x20)) {
            m199C = 1;
        }
        if (m199C != 0) {
            vf178();
            m199C = 0;
            return;
        }
    }
    CharaTask* t = m14D4;
    if (t != 0 && m14DC >= m1524) {
        if (m14E0 <= 0 && !mStatus.IsState_unk(4) && m1908.size() == 0) {
            vf160();
        } else {
            atn::Vector3 d = t->mPos - mPos;
            float dist = sqrt(d.x * d.x + d.z * d.z);
            if (dist <= m1504) {
                if (m14E4 % 5 == 0) {
                    vf13C();
                } else {
                    vf140();
                }
            }
        }
    }
}

// NONMATCHING: 94% - stack slots of the rotation temporaries (as Em202ForestSpider::vf14C)
void Em202MothersSpider::vf14C() {
    fn_8027688C();
    m58C = 11;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
    atn::Vector3 d = m18EC->mPos - mPos;
    PSVECNormalize(&d, &d);
    atn::Vector3 r = d.ToRotationZ_unk();
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
    int id;
    if (m18EC->mType == 0) {
        id = -1;
        if (fn_80370A78(lbl_8087F430, 0xE6) == 0) {
            fn_80370AE4(lbl_8087F430, 0xE6, 1);
        } else {
            fn_80370AE4(lbl_8087F430, 0xEA, 1);
        }
    } else {
        id = m18EC->m58;
        if (fn_80370A78(lbl_8087F430, 0xE7) == 0) {
            fn_80370AE4(lbl_8087F430, 0xE7, 1);
        } else {
            fn_80370AE4(lbl_8087F430, 0xEB, 1);
        }
    }
    fn_80370AE4(lbl_8087F430, 0x79, id);
}

void Em202MothersSpider::vf118() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m14E0 = 450;
        m1908.push_back(m18EC);
        m18EC->fn_8016EB48(0, 0, 0);
        m18EC->mFlagB9 = 1;
        m18EC->mF1C = (int)&m196C;
        m18EC->fn_8015495C(0, 1, 1, 1);
        m18EC = 0;
        fn_80279054();
        vf138();
        m199C = 0;
        m19A4 = 0;
    }
}

void Em202MothersSpider::vf178() {
    fn_8027688C();
    m58C = 19;
    m14F8 = 1;
    if (m1968 != 0) {
        fn_8017039C(m1968->mId, 2);
    } else {
        vf180();
    }
}

void Em202MothersSpider::vf17C() {
    atn::Vector3 d;
    SetFlat_unk(d, mPos.x - m1968->mPos.x, mPos.z - m1968->mPos.z);
    if (PSVECSquareMag(&d) < 100.0f) {
        m14F8 = 0;
        vf180();
    } else {
        vfFC();
    }
}

void Em202MothersSpider::vf180() {
    fn_8027688C();
    m58C = 20;
    fn_8016E970(3);
    mRot = atn::Vector3(0.0f, m1968->m14, 0.0f);
    m19A0 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 96% - register allocation of the spit out loop
void Em202MothersSpider::vf184() {
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
    }
    switch (m14D8) {
    case 0: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14D8++;
            fn_80232B7C(this, 100);
            PlayEffect_unk(&m19B4, &mMotion);
            m19C0 = fn_800C344C("SE_EM202_008", &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x143, 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    }
    case 1:
        m19A0++;
        if (m19A0 >= 5) {
            m14D8++;
            m19A0 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 100, 1);
            fn_800CB5C8(&m19C0, 0, 0);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
        }
        break;
    case 2: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 7.0f && frame <= 15.0f) {
            atn::Vector3 fwd = FrontDir_unk(mRot);
            atn::Model* model = &mMotion;
            MtxPtr m = model->GetNodeMtx_unk("hips");
            atn::Vector3 pos;
            atn::Vector3 off = fwd * 10.0f;
            pos.x = m[0][3] + off.x;
            pos.y = m[1][3] + off.y;
            pos.z = m[2][3] + off.z;
            if (m1908.size() != 0) {
                for (unsigned int i = 0; i < m1908.size(); i++) {
                    m1908[i]->mFlagB9 = 0;
                    m1908[i]->mF1C = 0;
                    m1908[i]->mPos = pos;
                    m1908[i]->mStatus.fn_8012F440(0x8000, 300, 0, 0, 0, 0, 1);
                    m1908[i]->fn_8016EB48(0, 0, 0);
                    m1908[i]->fn_8015E7A0(fwd * 25.0f, -1, 0);
                    if (m1908[i]->mType == 0) {
                        if (fn_80370A78(lbl_8087F430, 0xE8) == 0) {
                            fn_80370AE4(lbl_8087F430, 0xE8, 1);
                        }
                    } else if (fn_80370A78(lbl_8087F430, 0xE9) == 0) {
                        fn_80370AE4(lbl_8087F430, 0xE9, 1);
                    }
                }
                m1908.clear();
            }
            m14E0 = 450;
        } else {
            float end = mMotion.mTracks[0].m08;
            if (end >= mMotion.fn_80097D7C(0)) {
                vf138();
            }
        }
        break;
    }
    }
}

RoutePlace_unk* Em202MothersSpider::vf170() {
    atn::Vector3 center;
    fn_80278218(center);
    RoutePlace_unk* best = 0;
    float bestDist = 1.0e16f;
    for (unsigned int i = 0; i < m14C0.size(); i++) {
        atn::Vector3 d = center - m14C0[i]->mPos;
        float dist = PSVECSquareMag(&d);
        if (bestDist > dist) {
            bestDist = dist;
            best = m14C0[i];
        }
    }
    return best;
}

CharaTask* Em202MothersSpider::vf174() {
    CharaTask* found = m14D4;
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
        if (fn_8027851C(c) != 0 && (found == 0 || rand() % 100 < 70)) {
            found = c;
        }
    }
    return found;
}
