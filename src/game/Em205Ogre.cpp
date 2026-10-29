#include "game/Em205Ogre.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/SkillEffectManager.h"
#include "math/Vec.h"
#include "chr/ControlWallUp.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/CharaStatus.h"
#include <boost/bind.hpp>

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: length
extern "C" int fn_8021A984(HitInfo_unk* hit);
extern "C" int fn_80599C90(Bullet_unk* bullet, CharaTask* chara); // inferred: the effect can reach `chara`
extern "C" SkillEffectManager* lbl_8087F9E8;
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_80093C8C(atn::Model* model, const char* name); // inferred: nonzero while the named motion part plays
extern "C" int rand();
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: plays an effect resource (as in game/ColliAttrManager.cpp; the color is passed as null here)
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int a, int b, void* mtx, const atn::Vector3* pos,
                            const atn::Vector3* rot, const void* color, float scale, int e, int f, int g);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB6B0(atn::SoundRef* sound, int arg, float distance); // inferred: sets the distance
// inferred: frame time (as in game/CharaStatus.cpp)
struct FrameTime_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" FrameTime_unk* lbl_8087EFA8;
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

// inferred: hit point rate in percent, at least 1 while alive (name invented)
inline int Em205Ogre::HpPercent_unk() {
    float rate = mStatus.mParamB.m84 > 0 ? mStatus.m04 / (float)mStatus.mParamB.m84 : 0.0f;
    int hp = (int)(100.0f * rate);
    if (hp <= 0 && mStatus.m04 > 0.0f) {
        hp = 1;
    }
    return hp;
}
// inferred: a heal circle is near and the hit points are low (name invented)
inline bool Em205Ogre::WantsHeal_unk() {
    int hp = HpPercent_unk();
    if (m14FC) {
        if (hp >= m1504) {
            return false;
        }
    } else if ((m1374 & 1) == 1) {
        return false;
    } else if (hp >= m1500) {
        return false;
    }
    return m14F8 != 0;
}

// inferred: clamp of the swing progress (max(x, 0) then min(., 1), as two inline helpers; names invented)
#define SWING_MAX0_UNK(x) ((x) > 0.0f ? (x) : 0.0f)

// inferred: sound of the swing on the gimmick (name invented)
inline void PlaySe_unk(const char* name, const atn::Vector3& pos) {
    atn::SoundRef sound = fn_800C344C(name, &pos, 0, -1, 1.5f);
    fn_800CB6B0(&sound, 1, 500.0f);
}

// inferred: aims at the target mC58.mC4 (same code as the start of the states 9 and 10 in vf100; name
// invented)
inline void Em205Ogre::Aim_unk() {
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
}

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

void Em205Ogre::vf104() {
    m14C0++;
    if (CanAct_unk() && m58C != 6 && !vfF8()) {
        vf100(6);
        if (!mControl && m55C == 6) {
            fn_8016E970(2);
        }
    }
    if (CanAct_unk() && mC58.mC0 != 0) {
        if (!IsHealing_unk()) {
            if (WantsHeal_unk()) {
                vf100(11);
            }
        } else if (!WantsHeal_unk()) {
            vf100(6);
            m14FC = 0;
        }
    }
    switch (m58C) {
    case 6:
        if (m55C == 2 && vfF8()) {
            vf100(8);
        }
        if (!mControl && m55C == 6) {
            fn_8016E970(2);
        }
        vf108();
        break;
    case 7: {
        atn::Vector3 d = mC58.mC4->mPos - mPos;
        if (fn_805F9920(&d) < m14E4 * m14E4) {
            vf100(9);
        } else if (m14FC == 0 && mFlag31) {
            vf100(8);
        }
        vf108();
        break;
    }
    case 8:
        if (mC58.mC4 != m14D4) {
            vf100(8);
        } else {
            atn::Vector3 d = m14D4->mPos - mPos;
            float r = 3.0f * m5B0;
            r = r > m14E4 ? r : m14E4;
            if (fn_805F9920(&d) < r * r) {
                vf100(9);
            } else {
                if (fn_80093C8C(&mMotion, "op_02_mask") == 0 && m14C0 % 30 == 0 && rand() % 4 == 0) {
                    vf100(10);
                }
                if (mFlag31) {
                    vf100(8);
                }
            }
        }
        vf108();
        break;
    case 9:
    case 10:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 11:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 2:
        fn_805A40B8();
        break;
    case 3:
    case 4:
        vf108();
        break;
    default:
        vf100(6);
        break;
    }
}

// NONMATCHING: 99.69% - f3/f4 of the 0.0f and of m568 in the step vector of the swing and the order of the
// products of the frame time; r3/r4 of the copy of the head position (__VecCopy family); 12 variants
// inferred: runs the current state (route in the states 8 and 11, swing in 9, spit in 10)
void Em205Ogre::vf108() {
    float anim;
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    anim = 0.5f;
    if (mStatus.fn_8012DB04() > anim) {
        anim = mStatus.fn_8012DB04();
    }
    if (m55C == 2) {
        switch (m58C) {
        case 6:
            fn_805A4254();
            break;
        case 7:
            if (m14C0 > m14F4) {
                mFlag31 = 1;
            }
            break;
        case 8:
        case 11: {
            mC58.m0C.fn_80126214();
            if (mC58.m0C.m2C != 0) {
                mFlag31 = 1;
            }
            atn::Vector3 dir(mC58.m0C.m58);
            speed = fn_805F9920(&dir);
            if (speed > 0.0001f) {
                rot = dir.GetNormalize().ToRotation_unk();
            } else {
                rot = mRot;
            }
            if (m58C == 11) {
                rate = 1.25f;
            }
            rate *= mStatus.fn_8012DB04();
            break;
        }
        }
    } else if (m560 == 4) {
        switch (m58C) {
        case 9:
            rot = m14D8.ToRotation_unk();
            if (m14BC >= 2) {
                atn::Vector3 v(0.0f, 0.0f, 0.30769232f * m568);
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &v, &v);
                float scale = mMotion.m350;
                mPos += v * (lbl_8087EFA8->m3A4 * scale * anim);
            }
            mMotion.mTracks[0].m0C = anim;
            if (m14BC == 0) {
                if (mMotion.mTracks[0].m08 >= 26.0f) {
                    m14BC++;
                    HitInfo_unk* skill = fn_80219E6C(m14E8);
                    fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, fn_800F8548(lbl_8087F048), 30, -1, 0.0f);
                    if (m1514) {
                        GimmickCommon* g = lbl_8087F4A0 != 0 ? lbl_8087F4A0->fn_803EEE10(0x41B, 1) : 0;
                        if (g != 0) {
                            atn::Vector3 pos = g->vf54();
                            pos.y = g->m6C.y;
                            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
                            fn_80232B7C(this, 0);
                            fn_8023A680(lbl_8087F3C0, m1514.get(), -1, 0, 0, &pos, 0, 0, 1.0f, 0, -1, 1);
                            PlaySe_unk("SE_DG010_014", g->vf54());
                        }
                    }
                }
            } else if (m14BC == 1) {
                if (mMotion.mTracks[0].m08 >= 35.0f) {
                    if (mC58.mC4 != 0) {
                        Aim_unk();
                    }
                    if (m14D4 != 0) {
                        mMotion.fn_80097C08(0, 0x15D, 0, 1, 1, 0.0f, 0.1f);
                        m14BC++;
                    } else {
                        m14BC = -1;
                    }
                }
            } else if (m14BC == 2) {
                if (mMotion.mTracks[0].m08 >= 20.0f) {
                    m14C8 = fn_800F8548(lbl_8087F048);
                    m14BC++;
                }
            } else if (m14BC == 3) {
                HitInfo_unk* skill = fn_80219E6C(m14EC);
                float t = SWING_MAX0_UNK((mMotion.mTracks[0].m08 - 20.0f) / 7.0f) < 1.0f
                              ? SWING_MAX0_UNK((mMotion.mTracks[0].m08 - 20.0f) / 7.0f)
                              : 1.0f;
                float s = 3.1415927f;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, m14C8, 30, -1, 1.5707964f - t * s);
                if (mMotion.mTracks[0].m08 >= 27.0f) {
                    m14BC++;
                }
            } else if (mMotion.mTracks[0].m18 == 0) {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    mFlag31 = 1;
                }
            } else {
                mFlag31 = 1;
            }
            break;
        case 10:
            rot = m14D8.ToRotation_unk();
            mMotion.mTracks[0].m0C = anim;
            if (m14BC == 0) {
                if (mMotion.mTracks[0].m08 >= 20.0f) {
                    m14BC++;
                    HitInfo_unk* skill = fn_80219E6C(m14F0);
                    atn::Vector3 pos = mPos + atn::Vector3(0.0f, 10.0f, 0.0f);
                    MtxPtr head = mMotion.GetNodeMtx_unk("head");
                    if (head != 0) {
                        pos = GetNodePos_unk(head);
                    }
                    atn::Vector3 dir = m14D8;
                    if (m14D4 != 0) {
                        dir = m14D4->mColli.m48 - pos;
                        SafeNormalize_unk(&dir);
                    }
                    fn_800F8574(lbl_8087F048, this, skill, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
                }
            } else {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0)) {
                    mFlag31 = 1;
                }
            }
            break;
        default:
            mFlag31 = 1;
            break;
        }
    } else {
        fn_805A4254();
        return;
    }
    fn_8013CB68(&rot, 0, speed, m568 * rate);
}

// NONMATCHING: 98.61% - r3/r4 of the copy of the reference node position (__VecCopy family,
// docs/mwcc-blockers-q3.md) and FPR scheduling of the midpoint of the feet
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

void Em205Ogre::vf50() {
    Item14_unk item;
    m624.clear();
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += item.m04.mRadius;
    m624.push_back(item);
    m624.push_back(item);
    mFlagB29 = 0;
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

int Em205Ogre::vfB0(atn::Vector3* out) {
    if (m58C == 2) {
        return 0;
    }
    *out = m624.mData[1].m04.mCenter;
    return 1;
}

// inferred: the character that climbed the wall jumps onto the ogre (chr::ControlWallUp::vf10 asks for it)
boost::function<void()> Em205Ogre::vfB4(CharaTask* chara) {
    atn::Vector3 target;
    vfB0(&target);
    atn::Vector3 land = ((chr::ControlMoveCursor*)chara->mControl.get())->m14;
    chr::ControlWallSlash* next = new (0, "", "", 0) chr::ControlWallSlash(chara, target, &land);
    return boost::bind(&CharaTask::fn_80178208, chara, next);
}
