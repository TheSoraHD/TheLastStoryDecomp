#include "game/Em204Mulue.h"
#include "atn/Csv.h"
#include "atn/ColliBase.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/Bullet.h"
#include "game/GameUI.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "math/Vec.h"
#include "std/string_members.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
// inferred: debug menu helpers (as in game/CharaTask.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
Effect_unk* fn_8013A194(); // inferred: effect/bullet manager (weak copy of another file)
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet
extern "C" Bullet* fn_801010A0(Effect_unk* mgr); // inferred: a free bullet
extern "C" void fn_8010EE78(Bullet* bullet, BulletShot_unk* shot, const atn::Vector3* pos, const atn::Vector3* dir,
                            CharaTask* owner, int id, int arg, float speed); // inferred: fires the bullet
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" void fn_8004D124(void* world); // inferred: rebuilds the active shapes
// inferred: collision world (as in gmk/Cannonball.cpp; weak copy of another file)
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" GameUI* lbl_8087F490;
extern "C" void fn_803E3384(GameUI* ui); // inferred: closes the button prompt
extern "C" GimmickManager* lbl_8087F4A0; // inferred: gimmicks
extern "C" bool fn_80051A88(const CharaTask::Sphere_unk* sphere, const atn::Vector3* point); // inferred: point in sphere
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
// inferred: debug drawing (lbl_8087EEB0)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063D3C(void* draw, const atn::Vector3* center, unsigned int color, float radius, float arg);
extern "C" void fn_800638B0(void* draw, float a, float b, const atn::Vector3* from, const atn::Vector3* to,
                            unsigned int color);
// inferred: ray tests (as in game/Em208Queen.cpp and gmk/Brazier.cpp)
struct BigMulueColl_unk {
    BigMulueColl_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: hit position
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    void* m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004ED34(void* world, BigMulueColl_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" int fn_8004D314(void* world, atn::Vector3* hit, const atn::Vector3* pos, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sphere sweep

// inferred: 0x1C-byte record at 0x1470 of the battle characters (as PlayerState_unk in
// game/CharaTask.cpp; names invented)
struct BigMulueState_unk {
    BigMulueState_unk(short id) : m00(id), m02(0), m04(0), m08(150), m0C(0.0f, 0.0f, 0.0f), m18(0) {}
    short m00;
    short m02;
    int m04;
    int m08;
    atn::Vector3 m0C;
    int m18;
};
// inferred: a battle character with that record (names invented)
struct BigMulueChara_unk : public CharaTask {
    void SetState_unk(const BigMulueState_unk& state) { m1470 = state; }

    unsigned char m1424[0x1470 - 0x1424];
    BigMulueState_unk m1470;
};

// inferred: node position (weak copies of atn::Model::GetNodeMtx_unk and Matrix3x4::GetTrans_unk)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: plays an effect on the model (as in game/Em302Kraken.cpp; names invented)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, atn::Model* model) {
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

// inferred: identity rotated by z, y and x (as QueenRotZYX_unk of game/Em208Queen.cpp; name invented)
inline void BigMulueRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
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

// inferred: a speed that is (almost) zero means "use the default" (names invented)
inline float OrDefault_unk(float v, float def) {
    if ((float)__fabs(v) < 1.1920929e-7f) {
        return def;
    }
    return v;
}

Em204BigMulue::Em204BigMulue(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14DC(0), m14E0(0), m14E4(0), m14E8(0), m14FC(40.0f), m150C(0),
      m1510(30), m1514(80), m1518(180), m151C(80), m1520(60), m1524(0.4f), m1528(0.01f), m1548(0), m154C(0.1f),
      m1550(0.0f), m1554(0), m1A40(1), m1A44(1), m1A94(45.0f), m1A9C(0), m1AA0(0), m1AA4(0) {
    mFlag22 = 1;
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after these strings (as in game/Em205Ogre.cpp)
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em204_01_00.csv\0");
    }
    m14B4.vf0C(path);
    m1A6C.fn_80237654("/effect/ef_sk002b.eff");
    m1A60.fn_8023780C("/effect/ef_sk008d.efp");
    m1A78.fn_8023780C("/effect/ef_uc011.efp");
}

// NONMATCHING: 99.37% - registers of the name loops (r27/r28) and of the status flags
int Em204BigMulue::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1A6C.fn_802376D0() != 0 || m1A60.fn_80237874() != 0 || m1A78.fn_80237874() != 0) {
        return 0;
    }
    fn_802AC8E8((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    if (m1A9C != 0) {
        void* group = fn_8008937C(m1A9C, "PartsColli");
        for (unsigned int i = 0; i < m1558.size(); i++) {
            char name[0x100];
            sprintf(name, "%s-%s\0", m1558[i].mFrom.data(), m1558[i].mTo.data());
            fn_8008771C(group, name, &m1558[i].mRadius, 0, 0, 0.0f, 9999.0f, 1.0f);
        }
        group = fn_8008937C(m1A9C, "DamageColli");
        for (unsigned int i = 0; i < m1A34.size(); i++) {
            char name[0x100];
            sprintf(name, "%s\0", m1A34[i].mNode.data());
            fn_8008771C(group, name, &m1A34[i].mScale, 0, 0, 0.0f, 9999.0f, 1.0f);
        }
    }
    mColli.mFlags &= ~0x80000000;
    mStatus.m18 |= 0x1C0;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC219;
    mStatus.m18 |= 0x1800000;
    mStatus.m18 |= 4;
    mStatus.m18 &= ~0x2000000;
    vf20();
    for (unsigned int i = 0; i < m1558.size(); i++) {
        m1564[i].mFlags &= ~1;
        m1564[i].mFlags |= 2;
        m1564[i].m0C = (int)this;
        m1564[i].mFlags &= ~0x80000000;
    }
    m54C |= 0x200;
    vf50();
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    vfFC();
    m14E4 = 100;
    for (unsigned int i = 0; i < m1558.size(); i++) {
        m1564[i].mFlags &= ~1;
    }
    mColli.mFlags &= ~1;
    m540 = atn::Vector3(5.1f, 5.1f, 5.1f);
    return 1;
}

// NONMATCHING: 0% - the original calls the atn::Csv constructor, the element constructors (0x802ACC94,
// 0x802AD358), the push_backs and the element destructors out of line (caller over the inline budget);
// ours inlines all of them. Tried: explicit instantiations, explicit element constructors/destructors
void Em204BigMulue::fn_802AC8E8(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "BiteID") == 0) {
            m152C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "PressID") == 0) {
            m1530 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WaveID") == 0) {
            m1534 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "TackleID") == 0) {
            m1538 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "CollisionID") == 0) {
            m153C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "StingID") == 0) {
            m1540 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WallSlashID") == 0) {
            m1544 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WaitFrame") == 0) {
            m1510 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackDistance") == 0) {
            m14FC = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackChanceFrame") == 0) {
            m1514 = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(key, "TumbleFrame") == 0) {
            m1518 = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(key, "StaggerFrame") == 0) {
            m151C = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(key, "TumbleDamageRate") == 0) {
            m154C = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "StaggerNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1A84, &csv);
        } else if (strcmp(key, "TumbleNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1A8C, &csv);
        } else if (strcmp(key, "Collision") == 0) {
            BigMulueCapsule_unk c;
            c.mFrom = csv.fn_8005B9CC();
            c.mTo = csv.fn_8005B9CC();
            c.mRadius = fn_800DC288(csv.fn_8005B9CC());
            m1558.push_back(c);
        } else if (strcmp(key, "DamageSphere") == 0) {
            BigMulueSphere_unk d;
            d.mNode = csv.fn_8005B9CC();
            d.mScale = fn_800DC288(csv.fn_8005B9CC());
            d.m10 = fn_800DC288(csv.fn_8005B9CC());
            m1A34.push_back(d);
        }
    } while (csv.fn_8005BBF8());
}

// inferred: the original emits these copies right after fn_802AC8E8 (0x802ACD3C/0x802AD3D4)
template void std::vector<BigMulueCapsule_unk>::push_back(const BigMulueCapsule_unk& x);
template void std::vector<BigMulueSphere_unk>::push_back(const BigMulueSphere_unk& x);


// NONMATCHING: 99.16% - registers of the capsule loop and of two bool locals, stack slots of the hint
void Em204BigMulue::vf10() {
    if (m14E8 == 0) {
        m14E8 = 1;
        for (unsigned int i = 0; i < m1558.size(); i++) {
            m1564[i].mFlags |= 1;
        }
        fn_80370320(lbl_8087F430, 0x68, 1, 0);
    }
    if (!mFlag2 || !CanAct_unk()) {
        if (CanAct_unk() && m58C != 6 && !vfF8()) {
            vf100(6);
        }
        vf104();
    }
    if (mStatus.m04 / (float)mStatus.mParamB.m84 <= 0.4f) {
        m14D4 = 1;
    } else {
        m14D4 = 0;
    }
    if (!CharaTask::mFlag2 && m1A40 != 0 && mC58.mC0 > 0) {
        fn_802AE954();
        for (unsigned int i = 0; i < m1A34.size(); i++) {
            m1A34[i].m14 = GetNodePos_unk(mMotion.GetNodeMtx_unk(m1A34[i].mNode.data()));
        }
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !mFlag25) {
        fn_80145334();
    }
    if (m58C == 14 && m14BC == 4) {
        PlayerTask* player = lbl_8087F8A0->mHead;
        atn::Vector3 d = mPos - player->mPos;
        if (PSVECMag(&d) < 80.0f && !player->mFlag6 && !player->mFlag31 &&
            (player->m55C != 6 || player->m560 == 4)) {
            GameUI* ui = lbl_8087F490;
            if (ui != 0) {
                GameUIElem1C_unk hint;
                hint.m00 = 6;
                hint.m04 = 0x11;
                hint.m08 = -1;
                hint.m0C = 0;
                hint.m10 = 0;
                hint.m14 = 0;
                hint.m18 = 1;
                ui->m764 = hint;
            }
            if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
                m14BC = 5;
                atn::Vector3 pos = m624.mData[0].m04.mCenter;
                pos.y += 18.0f;
                m1A48 = player->mPos;
                BigMulueState_unk state(1);
                state.m0C = m1A48;
                state.m04 = 5;
                state.m08 = 90;
                ((BigMulueChara_unk*)player)->SetState_unk(state);
                player->fn_801588A0(m1544, pos);
                fn_800C31F4("SE_UICOM_011", 0, -1, 1.0f);
                if (lbl_8087F490 != 0) {
                    fn_803E3384(lbl_8087F490);
                }
            }
        }
    }
}

void Em204BigMulue::vf14() {
    fn_80149A30();
}

int Em204BigMulue::vf10C(HitMsg_unk* msg) {
    msg->m10 = lbl_807C7030;
    msg->m50 |= 0x10;
    if (m58C == 12 && m1A44 == 0) {
        m1A44 = 1;
    }
    if (m58C == 14) {
        if (m14BC == 7) {
            msg->m48 = 1;
        } else if (m14BC == 5 && msg->mData == m1544) {
            msg->m0C |= 0x80;
        }
    } else {
        msg->m48 = 1;
    }
    bool down = mStatus.IsState_unk(0x80000) || mStatus.IsState_unk(0x1000000);
    if (down) {
        msg->m48 = 0;
    }
    return 1;
}

void Em204BigMulue::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
    } else if (m58C == 14) {
        if (m14BC == 5) {
            if (msg->mData == m1544) {
                m14DC = 6;
                vf100(14);
            }
        } else if (m14BC == 2 || m14BC == 0) {
            float ratio = mStatus.m04 / (float)mStatus.mParamB.m84;
            m1550 += ratio * ratio;
            if (m1550 > 5.0f) {
                m14DC = 3;
                vf100(14);
            } else if (msg->mData != 0 && (msg->mData->m90 & 0x2000000) == 0x2000000) {
                m14DC = 3;
                vf100(14);
            }
        }
    }
}

void Em204BigMulue::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

// NONMATCHING: 96.82% - order of the item copies into the sphere vector (stack layout of the item)
void Em204BigMulue::vf50() {
    Item14_unk item;
    item.m00 = 0;
    item.m04.mCenter = atn::Vector3(0.0f, 0.0f, 0.0f);
    item.m04.mRadius = 5.0f;
    for (unsigned int i = 0; i < m1A34.size(); i++) {
        item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk(m1A34[i].mNode.data()));
        item.m04.mRadius = m5B0 * m1A34[i].mScale;
        m624.push_back(item);
    }
}

void Em204BigMulue::vf54() {
    if (m624.mSize != 0) {
        for (unsigned int i = 0; i < m1A34.size(); i++) {
            m624.mData[i].m04.mRadius = m5B0 * m1A34[i].mScale;
            m624.mData[i].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk(m1A34[i].mNode.data()));
        }
        if (m58C == 11) {
            m624.mData[1].m04.mCenter.y -= 8.0f;
        }
    }
}

// NONMATCHING: 99.52% - store order of m620/m61C and r6/r7 of the node copies (__VecCopy family)
void Em204BigMulue::vf20() {
    m614 = mPos + m5A4;
    m620 = m5B0;
    m614.y += m620;
    float radius = 0.1f * m5B0;
    atn::Vector3 a;
    atn::Vector3 b;
    a = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    b.y -= 1.0f;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = radius;
    for (unsigned int i = 0; i < m1558.size(); i++) {
        radius = m5B0 * m1558[i].mRadius;
        a = GetNodePos_unk(mMotion.GetNodeMtx_unk(m1558[i].mFrom.data()));
        b = GetNodePos_unk(mMotion.GetNodeMtx_unk(m1558[i].mTo.data()));
        m1564[i].m3C = a;
        m1564[i].m48 = b;
        m1564[i].m54 = radius;
    }
}

void Em204BigMulue::fn_802AE954() {
    for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
        if (g->m50 == 1 && g->vf5C() != 0) {
            atn::ColliSphere* colli = (atn::ColliSphere*)g->vf50();
            if (colli != 0) {
                CharaTask::Sphere_unk s;
                s.mCenter = colli->mSphere.mCenter;
                s.mRadius = colli->mSphere.mRadius;
                int hit = 0;
                for (unsigned int i = 0; i < m1A34.size(); i++) {
                    CharaTask::Sphere_unk c;
                    c.mCenter = m624.mData[i].m04.mCenter;
                    c.mRadius = m624.mData[i].m04.mRadius;
                    if (fn_80051A88(&s, &c.mCenter)) {
                        hit = 1;
                        break;
                    }
                }
                if (hit) {
                    GimmickMsg_unk msg;
                    msg.mType = 4;
                    g->vf40(&msg);
                }
            }
        }
    }
}

atn::Vector3 Em204BigMulue::vf64(int center) {
    atn::Vector3 r(mPos);
    if (center == 0) {
        r = m624.mData[3].m04.mCenter;
    } else {
        r = mPos;
    }
    return r;
}

// NONMATCHING: 97.60% - load order of the flags word and the next step, registers of the inlined motion helper
void Em204BigMulue::vf100(int state) {
    if (CharaTask::mFlag2) {
        return;
    }
    int step = m14DC;
    int frame = m14C0;
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
    if (step != 0) {
        m14BC = step;
        m14C0 = frame;
    }
    int prev = m58C;
    m14D8 = state;
    m58C = state;
    if (mC58.mC4 != 0) {
        m1500 = mC58.mC4->mPos;
    }
    if (m58C == 14) {
        m1454 = 0;
    } else {
        m1454 = 1;
    }
    switch (m58C) {
    case 6:
        mFlag30 = 1;
        for (unsigned int i = 0; i < m1558.size(); i++) {
            m1564[i].mFlags |= 1;
        }
        break;
    case 7:
        fn_802B1744(2, 1, m1524, 0.0f);
        break;
    case 8:
        m14EC = mC58.mC4;
        fn_802B1744(0x1C7, 1, 2.0f * m1524, 0.0f);
        break;
    case 9:
        if (m14BC == 1) {
            fn_802B1744(0x14, 1, 2.0f * m1524, 0.0f);
        } else if (m14BC == -1) {
            fn_802B1744(0x1C7, 1, 2.0f * m1524, 0.0f);
        }
        break;
    case 10:
        m150C = 0;
        fn_802B1744(0x1C7, 1, 2.0f * m1524, 0.0f);
        break;
    case 11: {
        int r = fn_802B0430(11, 0);
        if (r == 0) {
            fn_802B1744(0x156, 0, m1524, 0.0f);
        } else {
            m14DC = r;
            m14E0 = 11;
            vf100(9);
        }
        break;
    }
    case 12: {
        int r = fn_802B0430(11, 0);
        if (r == 0) {
            fn_802B1744(0x145, 0, m1524, 0.0f);
        } else {
            m14DC = r;
            m14E0 = 12;
            vf100(9);
        }
        break;
    }
    case 13:
        m14BC = 0;
        fn_802B1744(0x13F, 0, 3.0f * m1524, 0.0f);
        break;
    case 14:
        if (m14BC == 6) {
            fn_802B1744(0x31, 0, 2.0f * m1524, 0.0f);
        } else if (m14BC == 0) {
            m1550 = 0.0f;
            fn_805A4A1C((SpecialFlag_unk*)&m1A84, 1);
            fn_802B1744(0x35, 0, 2.0f * m1524, 0.0f);
        } else {
            fn_802B1744(0x38, 0, 2.0f * m1524, 0.0f);
        }
        break;
    case 2:
        if ((mStatus.m0C & 0x20) == 0x20 && prev != 2) {
            vf44();
        }
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x2E, 0, 1, 1, 0.0f, 0.1f);
        break;
    }
    m14DC = 0;
}

// NONMATCHING: 99.79% - stack slots of the two distance vectors and r3/r4 of the capsule loop
void Em204BigMulue::vf104() {
    m14C0++;
    atn::Vector3 d(0.0f, 0.0f, 0.0f);
    if (mC58.mC4 != 0) {
        d = mC58.mC4->mPos - mPos;
    }
    if (m58C >= 6 && m55C == 6) {
        m55C = 2;
    }
    switch (m58C) {
    case 6:
        if (vfF8()) {
            vf100(7);
        }
        vf108();
        break;
    case 7:
        if (mFlag31) {
            m14E4 = 0;
            atn::Vector3 center;
            if (fn_802B0D90(&center)) {
                atn::Vector3 to = mPos - center;
                if (fn_805F9920(&to) > 2500.0f) {
                    vf100(10);
                    m1500 = center;
                } else if (m14E0 == 0) {
                    vf100(8);
                } else {
                    vf100(m14E0);
                    m14E0 = 0;
                }
            } else {
                vf100(8);
            }
        }
        vf108();
        break;
    case 8:
        if (mFlag31) {
            if (m1548 >= 5) {
                m1554 = 1;
                vf100(13);
                m1548 = 0;
            } else {
                if (fn_805F9920(&d) < m14FC * m14FC) {
                    switch (m14D4) {
                    case 0:
                        if (rand() % 10 < 7) {
                            vf100(11);
                        } else {
                            vf100(12);
                        }
                        break;
                    case 1:
                        if (rand() % 10 < 5) {
                            vf100(11);
                        } else {
                            vf100(12);
                        }
                        break;
                    }
                    m1548++;
                } else {
                    vf100(7);
                }
            }
        }
        vf108();
        break;
    case 9:
    case 10:
        if (mFlag31) {
            if (m14E0 == 0) {
                vf100(8);
            } else {
                vf100(m14E0);
                m14E0 = 0;
            }
        }
        vf108();
        break;
    case 11:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 12:
        if (mFlag31) {
            m1554 = 1;
            vf100(13);
        }
        vf108();
        break;
    case 13:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 14:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 2:
        fn_805A40B8();
        if (mFlag25) {
            for (unsigned int i = 0; i < m1558.size(); i++) {
                m1564[i].mFlags &= ~1;
            }
        }
        break;
    default:
        vf100(6);
        break;
    }
}

void Em204BigMulue::vf108() {
    float dist = 0.0f;
    float speed = 1.0f;
    atn::Vector3 rot(mRot);
    if (m55C == 2) {
    switch (m58C) {
    case 6:
        if (m14C0 > m1510 + m14E4) {
            mFlag31 = 1;
        }
        break;
    case 7:
        if (m14C0 % 15 >= 10) {
            if (fn_802B12D8(0, 0.4f, 10.0f, 3.0f)) {
                mMotion.fn_80139F3C(0, 2.0f * m1524);
            } else {
                mFlag31 = 1;
            }
        }
        break;
    case 9: {
        float over;
        int r = fn_802B0430(m14E0, &over);
        if (r == 0) {
            mFlag31 = 1;
        } else {
            if (m14BC != r) {
                if (r == 1) {
                    fn_802B1744(0x14, 1, 2.0f * m1524, 0.0f);
                } else if (r == -1) {
                    fn_802B1744(0x1C7, 1, 2.0f * m1524, 0.0f);
                }
            }
            m14BC = r;
            if (r == 1) {
                fn_802B17AC(0.5f, 1.0f, 0.0f);
            } else if (r == -1) {
                fn_802B17AC(-0.5f, 1.0f, 0.0f);
            }
        }
        if (m14C0 % 15 >= 10) {
            fn_802B12D8(0, 0.4f, 0.0f, 3.0f);
        }
        break;
    }
    case 10:
        if (fn_802B07F0(40)) {
            mFlag31 = 1;
        }
        break;
    case 11:
        if (mMotion.fn_80139550(0) <= 16.0f) {
            fn_802B12D8(0, 0.4f, 0.0f, 3.0f);
        }
        if (mMotion.fn_80139550(0) >= mMotion.fn_80097D7C(0)) {
            mFlag31 = 1;
        }
        if (mMotion.fn_80139550(0) >= 22.0f && mMotion.fn_80139550(0) <= 23.5f) {
            atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("chin"))->GetTrans_unk();
            fn_800FAB80(fn_8013A194(), this, m152C, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        if (mMotion.fn_80139550(0) >= 23.0f && mMotion.fn_80139550(0) <= 27.0f) {
            mMotion.fn_80139F3C(0, 0.2f * m1524);
        } else {
            mMotion.fn_80139F3C(0, m1524);
        }
        break;
    case 12:
        if (mMotion.fn_80139550(0) >= mMotion.fn_80097D7C(0)) {
            mFlag31 = 1;
        }
        if (mMotion.fn_80139550(0) >= 28.0f && mMotion.fn_80139550(0) <= 34.0f) {
            mMotion.fn_80139F3C(0, 6.0f / (6.0f + (float)m1514));
        }
        if (mMotion.fn_80139550(0) >= 34.0f) {
            mMotion.fn_80139F3C(0, m1524);
        }
        if (mMotion.fn_80139550(0) >= 17.0f && mMotion.fn_80139550(0) <= 19.0f) {
            m1A44 = 0;
            atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
            fn_800FAB80(fn_8013A194(), this, m1530, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
            fn_800FAB80(fn_8013A194(), this, m1530, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("joint1"))->GetTrans_unk();
            fn_800FAB80(fn_8013A194(), this, m1530, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        if (mMotion.fn_80139550(0) >= 20.0f && mMotion.fn_80139550(0) <= 22.0f) {
            atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
            fn_800FAB80(fn_8013A194(), this, m1534, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        if (mMotion.fn_80139550(0) >= 24.0f) {
            m1A44 = 1;
        }
        if (mMotion.fn_80139550(0) <= 22.0f) {
            fn_802B17AC(-45.0f, 22.0f, 0.0f);
        }
        if (mMotion.fn_80139550(0) >= 42.0f && mMotion.fn_80139550(0) <= 60.0f) {
            fn_802B17AC(45.0f, 18.0f, 0.0f);
        }
        break;
    case 13:
        switch (m14BC) {
        case 0:
            if (mMotion.fn_80139550(0) >= mMotion.fn_80097D7C(0)) {
                m14C0 = 0;
                m14BC = 1;
                fn_802B1744(0x140, 1, 3.0f * m1524, 1.0f);
                m1A44 = 0;
            }
            break;
        case 1:
            if (m14C0 > m1520) {
                if (m1554 != 0) {
                    m14DC = 0;
                    vf100(14);
                } else {
                    m14BC = 2;
                    fn_802B1744(0x141, 0, 2.0f * m1524, 1.0f);
                }
                m1A44 = 1;
            }
            if (m14C0 <= m1520) {
                fn_802B1B50(m14C0 % 10 == 0);
            }
            {
                atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
                fn_800FAB80(fn_8013A194(), this, m1538, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            }
            break;
        case 2:
            if (mMotion.fn_80139550(0) >= mMotion.fn_80097D7C(0)) {
                mFlag31 = 1;
            }
            break;
        }
        break;
    case 14:
        switch (m14BC) {
        case 0:
            if (mMotion.fn_80139550(0) > 30.0f) {
                m14BC = 2;
                m14C0 = 0;
                fn_802B1744(0x37, 1, m1524, 0.0f);
            }
            break;
        case 2:
            if (m14C0 > m151C) {
                mFlag31 = 1;
            }
            break;
        case 3:
            if (mMotion.fn_80139550(0) >= mMotion.fn_80097D7C(0)) {
                m14BC = 4;
                m14C0 = 0;
                fn_802B1744(0x3B, 1, m1524, 0.0f);
                fn_805A4A1C((SpecialFlag_unk*)&m1A8C, 1);
            }
            if (mMotion.fn_80139550(0) < 20.0f) {
                fn_802B17AC(40.0f, 20.0f, 0.0f);
            } else if (mMotion.fn_80139550(0) <= mMotion.fn_80097D7C(0)) {
                fn_802B17AC(-20.0f, mMotion.fn_80097D7C(0) - 20.0f, 0.0f);
            }
            break;
        case 4:
            if (m14C0 > m1518) {
                m14BC = 7;
                fn_802B1744(0x3E, 0, 4.0f * m1524, 0.0f);
            }
            break;
        case 6:
            if (mMotion.fn_80139550(0) >= mMotion.fn_80097D7C(0)) {
                m14BC = 7;
                fn_802B1744(0x3E, 0, 4.0f * m1524, 0.0f);
            }
            break;
        case 7:
            if (mMotion.fn_80139550(0) > 25.0f) {
                m1554 = 0;
                vf100(13);
            } else if (mMotion.fn_80139550(0) > 15.0f && mMotion.fn_80139550(0) < 25.0f) {
                fn_802B17AC(-20.0f, 10.0f, 0.0f);
            }
            break;
        }
        break;
    case 2:
        if (mMotion.fn_80139550(0) >= mMotion.fn_80097D7C(0)) {
            fn_802B19EC(2);
        }
        if (mMotion.fn_80139F2C(0) == 0x2E && mMotion.fn_80139550(0) < 70.0f) {
            fn_802B17AC(-100.0f, 70.0f, 0.0f);
        }
        break;
    }
    } else {
        fn_805A4254();
        return;
    }
    fn_802B1B08(0);
    fn_8004D124(fn_801404F8());
    fn_8013CB68(&rot, 1, dist, m568 * speed);
    if (m1A44 != 0) {
        fn_802B1B08(1);
        fn_8004D124(fn_801404F8());
    }
}

// NONMATCHING: 99.89% - stack slots of the distance vector and of the ray ends
int Em204BigMulue::fn_802B0430(int state, float* over) {
    int result = 0;
    int far;
    int near;
    switch (state) {
    case 11:
        far = 0x57E4;
        near = 0x57E4;
        break;
    case 12: {
        far = 0xBD10;
        float d = 220.0f - m1A94;
        near = (int)(d * d);
        break;
    }
    default:
        return 0;
    }
    atn::Vector3 d = mC58.mC4->mPos - mPos;
    float dist = fn_805F9920(&d);
    if (dist > (float)far) {
        result = 1;
        if (over != 0) {
            *over = dist - (float)far;
        }
    }
    BigMulueColl_unk res;
    atn::Vector3 from(mPos);
    atn::Vector3 dir(0.0f, 0.0f, 300.0f);
    atn::Matrix3x4 m;
    BigMulueRotZYX_unk(m, mRot);
    PSMTXMultVec(m.m, &dir, &dir);
    dir += mPos;
    from.y += 25.0f;
    dir.y += 25.0f;
    if (fn_8004ED34(fn_801404F8(), &res, &from, &dir, fn_80179D44() | 0x80000000, 0, 0) != 0) {
        atn::Vector3 w = res.m10 - mPos;
        dist = fn_805F9920(&w);
        if (dist < (float)near) {
            result = -1;
            if (over != 0) {
                *over = dist - (float)near;
            }
        }
    }
    m1AA8 = res.m10;
    return result;
}

// NONMATCHING: 98.98% - stack slots and r3/r4 of the vector copies (__VecCopy family)
int Em204BigMulue::fn_802B07F0(int range) {
    int prev = m150C;
    atn::Vector3 target;
    if (fn_802B12D8(&m1500, 1.0f, 90.0f, 0.0f)) {
        atn::Vector3 d = m1500 - mPos;
        m150C = -1;
        target = mPos - d;
    } else {
        m150C = 1;
        target = m1500;
    }
    if (fn_802B12D8(&target, 1.0f, 25.0f, 0.0f)) {
        m150C = 0;
    }
    if (m150C != prev) {
        switch (m150C) {
        case 0:
            fn_802B1744(0x1C7, 1, 2.0f * m1524, 0.0f);
            break;
        case 1:
            fn_802B1744(0x14, 1, 2.0f * m1524, 0.0f);
            break;
        case -1:
            fn_802B1744(0x1C7, 1, 2.0f * m1524, 0.0f);
            break;
        }
    }
    if (m150C == 0) {
        if (m14C0 % 15 >= 10) {
            fn_802B12D8(&target, 0.4f, 10.0f, 3.0f);
        }
    } else {
        fn_802B12D8(&target, 1.0f, 0.1f, 10.0f);
        fn_802B17AC((float)m150C, 1.0f, 0.0f);
    }
    atn::Vector3 d = target - mPos;
    return fn_805F9920(&d) < (float)(range * range);
}

// NONMATCHING: 99.48% - stack slots of the sphere and of the ray vectors
int Em204BigMulue::fn_802B0D90(atn::Vector3* out) {
    int result = 0;
    atn::Vector3 hits[4];
    int i;
    for (i = 0; i < 4; i++) {
        CharaTask::Sphere_unk s = fn_80176548(mPos);
        s.mCenter += mRot * s.mRadius;
        s.mRadius *= 0.5f;
        float a = fmod(0.5f * (3.1415927f * (float)i) + mRot.y, 6.2831854820251465);
        if (a > 3.1415927f) {
            a -= 6.2831855f;
        }
        if (a < -3.1415927f) {
            a += 6.2831855f;
        }
        atn::Vector3 dir(0.0f, 0.0f, 300.0f);
        atn::Vector3 from(mPos);
        from.y += 20.0f;
        Mtx m;
        PSMTXRotRad(m, 'y', a);
        PSMTXMultVec(m, &dir, &dir);
        atn::Vector3 hit;
        if (fn_8004D314(lbl_8087EE98, &hit, &from, &dir, 0x80000000, 0, 0, s.mRadius) == 0) {
            break;
        }
        hits[i] = hit;
        if (m1AA0 != 0) {
            fn_80063D3C(lbl_8087EEB0, &hits[i], 0xFF00FFFF, 20.0f, 0.0f);
            fn_800638B0(lbl_8087EEB0, 10.0f, 7.0f, &from, &(from + dir), 0xFF00FFFF);
        }
    }
    if (i == 4) {
        atn::Vector3 a;
        atn::Vector3 b;
        a = (hits[0] + hits[2]) * 0.5f;
        b = (hits[1] + hits[3]) * 0.5f;
        *out = a + b - mPos;
        out->y = 0.0f;
        m1AC4 = *out;
        if (m1AA0 != 0) {
            fn_80063D3C(lbl_8087EEB0, &a, 0xFFFFFF00, 10.0f, 0.0f);
            fn_80063D3C(lbl_8087EEB0, &b, 0xFFFFAA00, 10.0f, 0.0f);
            fn_800638B0(lbl_8087EEB0, 10.0f, 7.0f, &a, &m1AC4, 0xFF00FFFF);
            fn_800638B0(lbl_8087EEB0, 10.0f, 7.0f, &b, &m1AC4, 0xFF00AAFF);
        }
        result = 1;
    }
    if (m1AA0 != 0) {
        atn::Vector3 p(mPos);
        p.y = 0.0f;
        atn::Vector3 c(m1AC4);
        c.y = 0.0f;
        atn::Vector3 d = p - c;
        if (fn_805F9920(&d) < 2500.0f) {
            fn_80063D3C(lbl_8087EEB0, &m1AC4, 0xFF0044AA, 20.0f, 0.0f);
        } else {
            fn_80063D3C(lbl_8087EEB0, &m1AC4, 0xFFAA0000, 20.0f, 0.0f);
        }
    }
    return result;
}

int Em204BigMulue::vfB0(atn::Vector3* out) {
    return 0;
}

boost::function<void()> Em204BigMulue::vfB4(CharaTask* chara) {
    return boost::function<void()>();
}

// NONMATCHING: 98.29% - the result of the last comparison is built with mfcr (original: branch) and frsp/fmr
// of the new yaw; 6 variants tried
int Em204BigMulue::fn_802B12D8(const atn::Vector3* target, float rate, float limit, float step) {
    if (target == 0 && mC58.mC4 == 0) {
        return 0;
    }
    atn::Vector3 d;
    if (target != 0) {
        d = *target - mPos;
    } else {
        d = mC58.mC4->mPos - mPos;
    }
    if (!((float)__fabs(fn_805F9920(&d)) < 1.1920929e-7f)) {
        PSVECNormalize(&d, &d);
    }
    atn::Vector3 rot = d.ToRotation_unk();
    float ry = mRot.y;
    float diff = rot.y - ry;
    float y;
    if (diff > 3.1415927f) {
        y = rate * (diff - 6.2831855f) + ry;
    } else if (diff < -3.1415927f) {
        y = rate * (6.2831855f + diff) + ry;
    } else {
        y = rate * diff + ry;
    }
    float a = fmod(y - mRot.y, 6.2831854820251465);
    if (a > 3.1415927f) {
        a -= 6.2831855f;
    }
    if (a < -3.1415927f) {
        a += 6.2831855f;
    }
    if (!((float)__fabs(step) < 1.1920929e-7f)) {
        float deg = 57.295776f * a;
        if ((float)__fabs(deg) >= step) {
            int sign = 1;
            if (deg < 0.0f) {
                sign = -1;
            }
            y = (float)sign * (0.017453292f * step) + mRot.y;
        }
        mRot.y = y;
        float b = fmod(y, 6.2831854820251465);
        if (b > 3.1415927f) {
            b -= 6.2831855f;
        }
        if (b < -3.1415927f) {
            b += 6.2831855f;
        }
    }
    if ((float)__fabs(57.295776f * a) >= limit) {
        return 1;
    }
    return 0;
}

void Em204BigMulue::fn_802B1744(int motion, int loop, float speed, float blend) {
    if ((float)__fabs(speed) < 1.1920929e-7f) {
        speed = m1524;
    }
    if ((float)__fabs(blend) < 1.1920929e-7f) {
        blend = m1528;
    }
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
    mMotion.fn_80097C08(0, motion, loop, 0, 1, 0.0f, blend);
}


// NONMATCHING: 99.41% - f7/f8 of the zero constant and of the speed
void Em204BigMulue::fn_802B17AC(float dist, float frames, float speed) {
    if ((float)__fabs(frames) < 1.1920929e-7f) {
        frames = mMotion.fn_80097D7C(0);
    }
    if ((float)__fabs(speed) < 1.1920929e-7f) {
        speed = mMotion.mTracks[0].m0C;
    }
    atn::Vector3 v(0.0f, 0.0f, speed * (dist / frames));
    atn::Matrix3x4 m;
    BigMulueRotZYX_unk(m, mRot);
    PSMTXMultVec(m.m, &v, &v);
    mPos.x += v.x;
    mPos.y += v.y;
    mPos.z += v.z;
}

void Em204BigMulue::fn_802B19EC(int kind) {
    switch (kind) {
    case 12:
        fn_80232B7C(this, 1000);
        PlayEffect_unk(&m1A54, &mMotion);
        break;
    case 2:
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1A78, &mMotion);
        break;
    }
}

void Em204BigMulue::fn_802B1B08(int on) {
    for (unsigned int i = 0; i < m1558.size(); i++) {
        if (on) {
            m1564[i].mFlags |= 1;
        } else {
            m1564[i].mFlags &= ~1;
        }
    }
}

// inferred: shot of a sting (BulletShot_unk with defaults, as MulueShot_unk of game/Em204Mulue.cpp)
struct BigMulueShot_unk : public BulletShot_unk {
    BigMulueShot_unk() {
        m10 = 0;
        m14 = 0.0f;
        m18 = 1.0f;
    }
};

// NONMATCHING: 98.77% - allocation of the saved registers (r24-r31), as Em204MulueMp::fn_802B55B0
void Em204BigMulue::fn_802B1B50(int fire) {
    if (m1540 == 0) {
        return;
    }
    atn::Matrix3x4 m;
    BigMulueRotZYX_unk(m, mRot);
    if (!fire) {
        return;
    }
    fn_800C344C("SE_EM204_014", &mPos, 0, -1, 1.0f);
    for (PlayerTask* c = lbl_8087F8A0->mHead; c != 0; c = c->mNext) {
        if (c->CanAct_unk()) {
            atn::Model* cm = &c->mMotion;
            atn::Vector3 to = GetNodePos_unk(cm->GetNodeMtx_unk("hips"));
            Bullet* bullet = fn_801010A0(lbl_8087F048);
            if (bullet != 0) {
                atn::Model* model = &mMotion;
                atn::Vector3 hips = GetNodePos_unk(model->GetNodeMtx_unk("hips"));
                atn::Vector3 dir = (to - hips).GetNormalize();
                BigMulueShot_unk shot;
                shot.m00 = 2.0f;
                shot.m04 = (int)&m1A6C;
                shot.m08 = 0;
                shot.m0C = m1540;
                fn_8010EE78(bullet, &shot, &hips, &dir, this, -1, 0, -1.0f);
            }
        }
    }
}

void Em204BigMulue::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em204 BigMulue\0");
    void* group;
    if (m1A98.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m1A98.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    fn_80087994(group, "DebugDisp", &m1AA0, 0, 0);
    fn_80087994(group, "DebugLog", &m1AA4, 0, 0);
    fn_800874C8(group, "ActionMode", &m58C, 0, 0x186A0, 1, 0, 0);
    fn_8008771C(group, "AttackDistance", &m14FC, 0, 0, 0.0f, 500.0f, 1.0f);
    fn_800874C8(group, "WaitFrame", &m1510, 0, 500, 1, 0, 0);
    fn_800874C8(group, "AttackChanceFrame", &m1514, 0, 500, 1, 0, 0);
    fn_800874C8(group, "TumbleFrame", &m1518, 0, 500, 1, 0, 0);
    fn_800874C8(group, "StaggerFrame", &m151C, 0, 500, 1, 0, 0);
    fn_8008771C(group, "BaseMotionSpeed", &m1524, 0, 0, 0.0f, 50.0f, 1.0f);
    fn_8008771C(group, "BaseMotionBlendSpeed", &m1528, 0, 0, 0.0f, 50.0f, 0.01f);
    fn_80087E9C(group, "WallSlashPos", &m1AB4, 0, 0, -5000.0f, 5000.0f, 1.0f);
    fn_80087E9C(group, "CenterPos", &m1AC4, 0, 0, -5000.0f, 5000.0f, 1.0f);
    fn_8008771C(group, "BodyPressOffset", &m1A94, 0, 0, -5000.0f, 5000.0f, 1.0f);
    fn_80087994(group, "CollisionOnOff", &m1A44, 0, 0);
    fn_800874C8(group, "ActionTimer", &m14C0, 0, 500, 1, 0, 0);
    fn_800874C8(group, "ActionState", &m14BC, -1, 500, 1, 0, 0);
    m1A9C = group;
}

float Em204BigMulue::vf90() {
    return 0.9f;
}

// NONMATCHING: 99.15% - r27/r28/r29 of the inlined vector destructors
Em204BigMulue::~Em204BigMulue() {}
