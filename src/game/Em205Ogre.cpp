#include "game/Em205Ogre.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/SkillEffectManager.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: length
extern "C" int fn_8021A984(HitInfo_unk* hit);
extern "C" int fn_80599C90(Bullet_unk* bullet, CharaTask* chara); // inferred: the effect can reach `chara`
extern "C" SkillEffectManager* lbl_8087F9E8;
// inferred: field (lbl_8087F430) and its map information (as in gmk/Cannonball.cpp)
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48; // inferred: area
    int m4C;
    int m50;
};
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);

// NONMATCHING: draft
Em205Ogre::Em205Ogre(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D8(0.0f, 0.0f, 0.0f), m14E4(40.0f), m14E8(0x410), m14EC(0x411), m14F0(0x412),
      m14F4(90), m14F8(0), m14FC(0), m1500(0), m1504(100), m150C(0) {
    mStatus.m18 &= ~0x40;
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em205_00_00.csv\0");
    }
    m14B4.vf0C(path);
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
        MapInfo_unk* map = fn_80373148(lbl_8087F430);
        if (map->m48 == 2 && map->m4C == 10 && map->m50 == 2) {
            m1514.reset(new (3, "", "", 0) ResLoader_unk());
            m1514->fn_80237654("/data/eff/gm001_000a02.eff");
        }
    }
    mFlagB4 = 1;
    m149C = 4.0f;
}

Em205Ogre::~Em205Ogre() {}

int Em205Ogre::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1514 && m1514->fn_802376D0()) {
        return 0;
    }
    fn_802B7AF8((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    m149C = 2.5f * m540.y;
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    vfFC();
    mStatus.m18 |= 0x8208;
    mStatus.m18 |= 0x3800000;
    return 1;
}

void Em205Ogre::fn_802B7AF8(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "SkillId") == 0) {
            m14E8 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "SwingSkillId") == 0) {
            m14EC = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "SpitSkillId") == 0) {
            m14F0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "WaitFrame") == 0) {
            m14F4 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackDistance") == 0) {
            m14E4 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "HealCircleMoveHpMin") == 0) {
            m1500 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HealCircleMoveHpMax") == 0) {
            m1504 = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

void Em205Ogre::vf14() {
    fn_80149A30();
}

int Em205Ogre::vf10C(HitMsg_unk* msg) {
    m1510 = 0;
    if (*(int*)&msg->m3C[4] == 1) {
        msg->m0C |= 0x80;
        m1510 = 1;
    } else if (m58C == 9 && m14BC <= 1) {
        msg->m10 *= 0.0f;
    }
    return 1;
}

void Em205Ogre::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
        return;
    }
    if (m1510 != 0 || m58C != 9 || m14BC > 1) {
        vf100(6);
    }
    fn_805A4188((HitMsg_unk*)hit);
}

void Em205Ogre::vf7C(int arg) {
    vf100(6);
    CharaTask::vf7C(1);
}

void Em205Ogre::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

void Em205Ogre::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em205 Ogre[%d]", m58);
    void* group;
    if (m1508.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m1508.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
}

void Em205Ogre::vf54() {
    if (m624.mSize < 2) {
        return;
    }
    m624.mData[0].m04.mRadius = mColli.m54;
    m624.mData[0].m04.mCenter = mColli.m3C;
    m624.mData[1].m04.mRadius = 0.5f * mColli.m54;
    m624.mData[1].m04.mCenter = mColli.m48;
    m624.mData[1].m04.mCenter.y += 0.5f * mColli.m54;
}

void Em205Ogre::vf10() {
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        vf104();
        fn_802B9C5C();
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
    fn_802B98B0();
}

void Em205Ogre::fn_802B9C5C() {
    float best = 1e16f;
    m14F8 = 0;
    if (lbl_8087F9E8 != 0) {
        for (unsigned int i = 0; i < 32; i++) {
            Bullet_unk* bullet = lbl_8087F9E8->GetBullet_unk(i);
            if (bullet != 0 && bullet->IsActive_unk() && fn_8021A984(bullet->m04) &&
                fn_80599C90(bullet, this)) {
                atn::Vector3 d = bullet->m10 - mPos;
                float dist = PSVECMag(&d) - 0.75f * (bullet->m5C * bullet->m28);
                if (dist < best) {
                    m14F8 = bullet;
                    best = dist;
                }
            }
        }
    }
}


// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: names invented
inline void SafeNormalize_unk(atn::Vector3* v) {
    if ((float)__fabs(fn_805F9920(v)) < 1.1920929e-7f) {
        return;
    }
    PSVECNormalize(v, v);
}

void Em205Ogre::vf100(int state) {
    if (CharaTask::mFlag2) {
        return;
    }
    int prev = m58C;
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
    m58C = state;
    switch (state) {
    case 6:
        mFlag30 = 1;
        break;
    case 7:
        mFlag30 = 1;
        fn_8016E970(2);
        break;
    case 8: {
        m14D4 = mC58.mC4;
        mC58.m0C.fn_80128930(m14D4, &mPos, 0);
        float r = 3.0f * m5B0;
        mC58.m0C.mRadius = 0.8f * (r > m14E4 ? r : m14E4);
        mC58.m0C.fn_801255C8();
        mFlag30 = 1;
        fn_8016E970(2);
        break;
    }
    case 9:
        m14D4 = mC58.mC4;
        if (m14D4 != 0) {
            m14D8 = m14D4->mPos - mPos;
            m14D8.y = 0.0f;
            SafeNormalize_unk(&m14D8);
        } else {
            m14D8.x = 0.0f;
            m14D8.y = 0.0f;
            m14D8.z = 1.0f;
            Mtx m;
            float ry = mRot.y;
            PSMTXRotRad(m, 'y', ry);
            PSMTXMultVec(m, &m14D8, &m14D8);
        }
        fn_805A4F1C(0x15C, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        fn_8016E970(6);
        m560 = 4;
        break;
    case 10:
        m14D4 = mC58.mC4;
        if (m14D4 != 0) {
            m14D8 = m14D4->mPos - mPos;
            m14D8.y = 0.0f;
            SafeNormalize_unk(&m14D8);
        } else {
            m14D8.x = 0.0f;
            m14D8.y = 0.0f;
            m14D8.z = 1.0f;
            Mtx m;
            float ry = mRot.y;
            PSMTXRotRad(m, 'y', ry);
            PSMTXMultVec(m, &m14D8, &m14D8);
        }
        fn_805A4F1C(0x13F, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        fn_8016E970(6);
        m560 = 4;
        break;
    case 11:
        if (m14F8 != 0) {
            mC58.m0C.fn_80128A30(&m14F8->m10, &mPos, 8, 0.0f);
            mC58.m0C.mRadius = 0.75f * (m14F8->m5C * m14F8->m28);
            mC58.m0C.fn_801255C8();
            m14FC = 1;
        } else {
            vf100(6);
            break;
        }
        mFlag30 = 1;
        fn_8016E970(2);
        break;
    case 2:
        if ((mStatus.m0C & 0x20) == 0x20 && prev != 2) {
            vf44();
        }
        fn_805A4F1C(0x2E, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        fn_8016E970(2);
        break;
    }
}

// NONMATCHING: ~60% - r3/r4 of the copy of the reference node position (__VecCopy family,
// docs/mwcc-blockers-q3.md), FPR scheduling of the midpoint of the feet, and the offsets of the node names
// in the string pool (strings of functions not written yet)
void Em205Ogre::fn_802B98B0() {
    atn::Vector3 base = mPos;
    MtxPtr ref = mMotion.GetNodeMtx_unk("reference");
    if (ref != 0) {
        base = GetNodePos_unk(ref);
    }
    m620 = m5B0;
    m614 = base + m5A4;
    m614.y += m620;
    atn::Vector3 top = m614;
    if (m58C == 9 && m14BC >= 2) {
        MtxPtr left = mMotion.GetNodeMtx_unk("leftfoot");
        MtxPtr right = mMotion.GetNodeMtx_unk("rightfoot");
        if (left != 0 && right != 0) {
            top = (GetNodePos_unk(left) + GetNodePos_unk(right)) * 0.5f;
            top.y = m614.y;
        }
    }
    atn::Vector3 head;
    int neck = mMotion.fn_80092814("neck", 0);
    if (neck > -1) {
        head = GetNodePos_unk(mMotion.mNodeMtx[neck]);
        head.y -= 1.0f;
    } else {
        head = top;
        head.y += m5B4 - 2.0f * m620;
    }
    mColli.m3C = top;
    mColli.m48 = head;
    mColli.m54 = m620;
    m1420 = 1.2f * (head.y - top.y + 2.0f * m620);
}
