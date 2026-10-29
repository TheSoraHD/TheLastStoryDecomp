#include "game/Em305Vampire.h"
#include "atn/Csv.h"
#include "math/Vec.h"
#include "game/CharaStatus.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" int atoi(const char* s);
extern "C" double strtod(const char* str);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id

extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3* pos, int arg);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                           int c, float f); // inferred: attack hit, nonzero if it hit (as in game/Em201ZanGurg.cpp)
// inferred: collision world and its tests (as in game/Em003LesserDaemon.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004D388(void* world, CollResult_unk* result, const atn::Vector3* center, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
// inferred: debug menu entries (as in game/CharaTask.cpp and game/NewCampPartsHint.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80088FAC(void* group, const char* name, atn::Model::Float4_unk* value, int arg3, int arg4,
                            float min, float max, float step);

// inferred: plays an effect on the model (same as in Em305VampireMp.cpp)
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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

extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int fn_800CB58C(atn::SoundRef* sound);                           // inferred: nonzero while playing
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound

extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
// inferred: field (lbl_8087F430); m10D8: route tables; fn_80370AE4 sets a counter (as in gmk/Coffin.cpp)
struct VampireField_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" VampireField_unk* lbl_8087F430;
extern "C" void fn_80370AE4(VampireField_unk* field, int index, int value);

// inferred: multiplayer session (lbl_8087F610), as in Em305VampireMp.cpp
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DA48C(MpSession_unk* session, int arg);
extern "C" void fn_804DA4A0(MpSession_unk* session, int arg);

// NONMATCHING: 93.81% - saved registers (this/info/zero are r29/r30/r28 in the original and
// r28/r29/r31 here) and the schedule of the flag stores before the strings (as Em305VampireMp ctor; 6 forms)
Em305Vampire::Em305Vampire(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14F8(0), m14FC(0), m1500(0), m1504(0), m1508(0), m150C(0), m1510(0),
      m1514(0), m1524(0), m1528(0), m152C(0.0f), m1530(0), m1534(0), m1538(1.0f), m153C(0), m1540(0),
      m1544(0.0f, 0.0f, 0.0f), m1550(0.0f, 0.0f, 0.0f), m155C(0), m1560(0),
      m15B8(450), m15C0(0), m15D4(1.0f), m15F0(0.0f), m15F4(0.0f), m15F8(0.0f), m15FC(0), m1600(0), m1604(1) {
    m15C4.m00 = 0.1f;
    m15C4.m04 = 0.1f;
    m15C4.m08 = 0.1f;
    m15C4.m0C = 1.0f;
    m15D8[0] = 0.0f;
    m15D8[1] = 0.0f;
    m15D8[2] = 0.0f;
    m15E4[0] = 0;
    m15E4[1] = 0;
    m15E4[2] = 0;
    mFlag22 = 1;
    mFlag21 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m14DC.clear();
    m14E8.clear();
    std::string dir("database/chara/special/");
    std::string name("db_em305_00_00.csv");
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
    m14D4.vf0C(path.c_str());
    m1564.LoadRequest("/effect/ef_uc031.efp");
    m1570.LoadRequest("/effect/ef_uc007.efp");
    m157C.LoadRequest("/effect/ef_sk021a.efp");
    m1588.LoadRequest("/effect/ef_sk025.efp");
    m1594.LoadRequest("/effect/ef_sc003_01c.efp");
    SpecialEnemyBase::mFlag30 = 1;
}

Em305Vampire::~Em305Vampire() {
    fn_800CB5C8(&m15A0, 5, 0);
    fn_800CB5C8(&m15A4, 5, 0);
}

// NONMATCHING: 93.89% - same as Em305VampireMp::vf0C: the inlined std::vector push_back of the two routes
// (known open case) and registers
int Em305Vampire::vf0C() {
    if (m1564.IsLoading() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1570.IsLoading() == 0 &&
        m157C.IsLoading() == 0 && m1588.IsLoading() == 0 &&
        m1594.IsLoading() == 0 && m14D4.fn_80473F50() == 0 &&
        fn_8013655C() == 0) {
        fn_802EC38C();
        if (m14D4.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m14D4.fn_80470580(), m14D4.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "weaken_route") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m14E8.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "door_route") == 0) {
                    int a = fn_800DC12C(csv.fn_8005B9CC());
                    int b = fn_800DC12C(csv.fn_8005B9CC());
                    int c = fn_800DC12C(csv.fn_8005B9CC());
                    int d = fn_800DC12C(csv.fn_8005B9CC());
                    m14DC.push_back(VampireDoor_unk(a, b, c, d));
                }
                if (strcmp(key, "weaken_time") == 0) {
                    m15E4[0] = atoi(csv.fn_8005B9CC());
                    m15E4[1] = atoi(csv.fn_8005B9CC());
                    m15E4[2] = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "weaken_speed") == 0) {
                    m15D8[0] = strtod(csv.fn_8005B9CC());
                    m15D8[1] = strtod(csv.fn_8005B9CC());
                    m15D8[2] = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "speed_up_value") == 0) {
                    m15F8 = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "speed_up_time") == 0) {
                    m15FC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "speed_up_wait") == 0) {
                    m1600 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "weaken_1to2") == 0) {
                    m15F0 = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "weaken_2to3") == 0) {
                    m15F4 = strtod(csv.fn_8005B9CC());
                } else if (strcmp(key, "normal_wait_time") == 0) {
                    m15A8 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "through_wait_time") == 0) {
                    m15AC = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "silver_arrow_count") == 0) {
                    m1604 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "stun_time") == 0) {
                    m15B8 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "tackle_skill") == 0) {
                    m150C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "rolling_skill") == 0) {
                    m1514 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "shine_skill") == 0) {
                    m1510 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x1C015;
        mStatus.m18 |= 0x3000000;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

inline void Em305Vampire::Tackle_unk() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_802EBABC();
        return;
    }
    float f = mMotion.mTracks[0].m08;
    if (31.0f <= f && f <= 33.0f) {
        int id = m590;
        HitInfo_unk* data = m1510;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 60, 5, 0.0f);
    }
    if (m14F4 != 0) {
        atn::Vector3 d = m14F4->mPos - mPos;
        atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
        mRot.y = rot.y;
    }
}

inline void Em305Vampire::Rolling_unk() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m1500 = 0;
        m14F8 = 0;
        m14FC = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 0;
        fn_8016E970(3);
    } else {
        float f = mMotion.mTracks[0].m08;
        if (45.0f <= f) {
            fn_800FAB80(lbl_8087F048, this, m1514, m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
    }
    if (m1528 != 0) {
        float f = mMotion.mTracks[0].m08;
        if (80.0f <= f && f <= 83.0f) {
            Effect_unk* mgr = lbl_8087F048;
            fn_800FAB80(mgr, this, m1514, fn_800F8548(mgr), &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            fn_80370AE4(lbl_8087F430, 5, 1);
            m1528 = 0;
            m1540 = 0;
        }
    }
}

inline void Em305Vampire::Stun_unk() {
    if (m14FC >= m15B8) {
        fn_80239DAC(lbl_8087F3C0, this, 100, 0);
        fn_802EBABC();
    }
}

void Em305Vampire::vf10() {
    m14F4 = mC58.mC4;
    if (m1524 > 0 && m58C != 15) {
        m1524--;
    }
    if (m1528 != 0) {
        if (m1530 == 1) {
            m152C = 0.0f;
            if (++m1534 >= m1600 * 30) {
                m1530 = 2;
                m1534 = 0;
            }
        } else if (m1530 == 2) {
            m152C = m15F8;
            if (++m1534 >= m15FC * 30) {
                m1530 = 0;
                m1534 = 0;
            }
        } else {
            m152C = 0.0f;
        }
    }
    bool target = true;
    if (mC58.mC0 == 0 || mC58.mC4 == 0) {
        target = false;
    }
    if (!target) {
        fn_8016E970(3);
        fn_800CB5C8(&m15A0, 5, 0);
        fn_800CB5C8(&m15A4, 5, 0);
    } else {
        if ((mStatus.m0C & 0x20) != 0x20) {
            if (m58C == 7 && m14F8 == 1) {
                fn_800CB5C8(&m15A0, 5, 0);
                if (fn_800CB58C(&m15A4) == 0) {
                    m15A4 = fn_800C344C("SE_EM305_002", &mPos, 0, -1, 1.0f);
                }
            } else {
                fn_800CB5C8(&m15A4, 5, 0);
                if (fn_800CB58C(&m15A0) == 0) {
                    m15A0 = fn_800C344C("SE_EM305_001", &mPos, 0, -1, 1.0f);
                }
            }
            fn_800CB6E4(&m15A0, &mPos);
            fn_800CB6E4(&m15A4, &mPos);
        }
        if (m14F4 != 0) {
            switch (m58C) {
            case 6:
                Tackle_unk();
                break;
            case 7:
                fn_802EAF68();
                break;
            case 8:
                fn_802EB1F4();
                break;
            case 13:
                fn_802EB61C();
                break;
            case 14:
                Rolling_unk();
                break;
            case 15:
                Stun_unk();
                break;
            case 2:
                vfEC();
                break;
            default:
                fn_802EABE4();
                vf11C();
                break;
            }
        }
    }
    m14FC++;
    fn_8014C540();
    fn_80145334();
}

void Em305Vampire::vf14() {
    if (m1528 != 0) {
        mMotion.m40 = m15C4;
        mMotion.fn_8009373C("op_02_cloud", 0);
    } else {
        atn::Model::Float4_unk white;
        white.m00 = 1.0f;
        white.m04 = 1.0f;
        white.m08 = 1.0f;
        white.m0C = 1.0f;
        mMotion.m40 = white;
        mMotion.fn_8009373C("op_02_cloud", 1);
    }
    fn_80149A30();
}

inline void SetSeg_unk(atn::ColliCapsule& c, const atn::Vector3& a, const atn::Vector3& b, float r) {
    c.m3C = a;
    c.m48 = b;
    c.m54 = r;
}

// inferred: capsule along the y axis of the node "center_chara" (from -5 to 5)
void Em305Vampire::vf20() {
    m5A4.y = 3.0f;
    m5B0 = 10.0f;
    m620 = 0.5f * m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Matrix3x4 m(mMotion.GetNodeMtx_unk("center_chara"));
    atn::Matrix3x4* p = &m;
    if (p != 0) {
        m5B0 = 10.0f;
        float z = m.m[2][3];
        float y = m.m[1][3];
        float x = m.m[0][3];
        atn::Vector3 a(0.0f, -5.0f, 0.0f);
        atn::Vector3 to(x, y, z);
        atn::Vector3 b(0.0f, 5.0f, 0.0f);
        atn::Vector3 from(x, y, z);
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = 0.0f;
        PSMTXMultVec(m.m, &a, &a);
        PSMTXMultVec(m.m, &b, &b);
        to += a;
        from += b;
        SetSeg_unk(mColli, to, from, m5B0);
    }
    mColli.m20 = 0x80;
}

// inferred: reaction to the hit (0x134: silver arrow, 0x1791: stun)
void Em305Vampire::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->mData->m04 == 10000) {
        fn_80151448(msg);
        vf40(hit);
        return;
    }
    if (m1524 <= 0 && msg->mData->m04 != 0x134) {
        msg->m68 = 0;
        msg->m88 = msg->mData->mC4;
        msg->m84 = 1;
        msg->m90 = 1;
        if (++m1504 == 3) {
            fn_80370AE4(lbl_8087F430, 4, 1);
        }
        if (m1528 == 0 && m58C == 0) {
            fn_802EBBA8();
        }
        return;
    }
    if (m58C - 6U <= 2 || m58C - 14U <= 1) {
        msg->m10 *= 0.0f;
    } else {
        msg->m10 *= 0.1f;
    }
    if (m58C == 15) {
        msg->m0C |= 0x80;
    }
    if (msg->mData->m04 == 0x134) {
        if (m153C == 0) {
            fn_80370AE4(lbl_8087F430, 4, 2);
            m153C = 1;
        } else {
            fn_80370AE4(lbl_8087F430, 4, 4);
        }
        msg->m0C |= 0x800;
    }
    fn_80151448(msg);
    vf40(hit);
}

inline void Em305Vampire::StartWeaken_unk() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 13;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 0.2f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    vf9C(0);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1588, &mMotion);
    if (lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 4);
        fn_804DA4A0(lbl_8087F610, 0);
    }
}

inline void Em305Vampire::StartStun_unk() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 0.0f;
    mMotion.fn_80097C08(0, 2, 1, 1, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 100);
    PlayEffect_unk(&m1594, &mMotion);
    Effect_unk* mgr = lbl_8087F048;
    atn::Vector3 pos = fn_801781B0();
    fn_80108F38(mgr, &pos, 0x80);
}

void Em305Vampire::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    int id = msg->mData->m04;
    if (id == 0x134) {
        if (++m1540 >= m1604) {
            StartWeaken_unk();
        }
    } else if (id == 0x1791) {
        StartStun_unk();
    } else if (m1530 == 0) {
        m1530 = 1;
        m1534 = 0;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802EC280();
    }
}

inline void Em305Vampire::StartTackle_unk() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(6);
    m560 = 4;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 1, 1, 0.0f, 0.1f);
}

inline void Em305Vampire::StartFlee_unk() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 14;
    m152C = 0.0f;
    m1530 = 0;
    m1534 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x66, 0, 0, 1, 0.0f, 0.1f);
    vf98();
    if (lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 5);
        fn_804DA4A0(lbl_8087F610, 1);
    }
}

// inferred: route point by 1-based index (as in game/Em310Gargoyle.cpp; name invented)
inline RoutePoint_unk* GetRoutePoint_unk(RouteTable_unk* table, int index) {
    return &table->mPoints[index - 1];
}

// inferred: door of "door_route" between two route points, in either direction (name invented)
inline VampireDoor_unk* Em305Vampire::FindDoor_unk(int a, int b) {
    for (unsigned int i = 0; i < m14DC.size(); i++) {
        if (a == m14DC[i].m00 && b == m14DC[i].m04) {
            return &m14DC[i];
        }
        if (a == m14DC[i].m04 && b == m14DC[i].m00) {
            return &m14DC[i];
        }
    }
    return 0;
}

// NONMATCHING: 98.82% - the original loads the next route point with lwzux (address kept for the copy) and
// r3/r4 of the two vector copies (__VecCopy family); ~12 forms
// inferred: movement along the route; when the walker goes through a door of "door_route" it starts state 8
void Em305Vampire::fn_802EA490() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 v = mRoute.m58;
        if (lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0 && mRoute.mDestPoint > 0 && mRoute.mPointA > 0) {
            RouteTable_unk* table = lbl_8087F430->m10D8;
            int a = table->mPoints[mRoute.mDestPoint - 1].m00;
            RoutePoint_unk* to = &table->mPoints[mRoute.mPointA - 1];
            int b = to->m00;
            VampireDoor_unk* door = FindDoor_unk(a, b);
            if (door != 0) {
                atn::Vector3 dest = to->mPos;
                m14F8 = 0;
                m14FC = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 8;
                fn_8016E970(6);
                m560 = 4;
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
                m1544 = mPos;
                m1550 = dest;
                m155C = door;
                return;
            }
        }
        speed = v.Length_unk();
        if (speed < 0.0001f) {
            rot = mRot;
        } else {
            rot = v.GetNormalize().ToRotation_unk();
        }
        if (m1528 == 1) {
            rate *= m1538 * (m15D8[0] + m152C);
        } else if (m1528 == 2) {
            rate *= m1538 * (m15D8[1] + m152C);
        } else if (m1528 == 3) {
            rate *= m1538 * (m15D8[2] + m152C);
        } else {
            rate *= m15D4;
        }
        if (m1528 != 0) {
            rate *= mStatus.fn_8012DB04();
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(rot, 1, speed, rate * m568);
}

// inferred: picks the next action (as Em305VampireMp::vf11C without the ray test)
void Em305Vampire::vf11C() {
    if (m1500 == 0 && m14F4 != 0) {
        if (m14FC < m15A8) {
            return;
        }
        atn::Vector3 pos = m14F4->mPos;
        atn::Vector3 d = pos - mPos;
        d.y = 0.0f;
        if (d.Length_unk() < 40.0f) {
            StartTackle_unk();
        }
    }
    if (m1500 == 1) {
        if (m1524 <= 0) {
            StartFlee_unk();
        } else {
            RoutePlace_unk* place = m14E8[m1560];
            atn::Vector3 d(mPos.x - place->mPos.x, 0.0f, mPos.z - place->mPos.z);
            if (PSVECSquareMag(&d) < 100.0f) {
                m1560++;
                if (m1560 >= (int)m14E8.size()) {
                    m1560 = 0;
                }
                fn_8017039C(m14E8[m1560]->mId, 0);
            }
        }
    }
}

void Em305Vampire::fn_802EABE4() {
    if (m55C - 6U > 1) {
        fn_8016E970(3);
        if (m1500 == 1) {
            fn_8017039C(*(int*)m14E8[m1560], 0);
        } else {
            fn_80170A20(m14F4, 0, -1.0f);
        }
    }
    fn_802EA490();
}

// inferred: state 6 (tackle): hit between the frames 31 and 33 and turn to the target
void Em305Vampire::fn_802EAC70() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_802EBABC();
        return;
    }
    float f = mMotion.mTracks[0].m08;
    if (31.0f <= f && f <= 33.0f) {
        int id = m590;
        HitInfo_unk* data = m1510;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 60, 5, 0.0f);
    }
    if (m14F4 != 0) {
        atn::Vector3 d = m14F4->mPos - mPos;
        atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
        mRot.y = rot.y;
    }
}

// inferred: state 7 (tackle): wind up, then sweep along m1518 until something is hit, then recover
void Em305Vampire::fn_802EAF68() {
    if (m14F8 == 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x140, 1, 0, 1, 0.0f, 0.1f);
            m14F8 = 1;
            m14FC = 0;
            fn_80232B7C(this, 101);
            PlayEffect_unk(&m157C, &mMotion);
        }
    } else if (m14F8 == 1) {
        int hit = 0;
        CollResult_unk result;
        if (fn_8004D388(lbl_8087EE98, &result, &m614, &m1518, 0x80000000, &mColli, 0, m620) != 0) {
            hit = 1;
        }
        fn_80144710();
        if (fn_800F8C6C(lbl_8087F048, 0, 0, this, m150C, m590, 10, 5, 0.0f) != 0) {
            hit = 1;
        }
        if (hit) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x141, 0, 0, 1, 0.0f, 0.1f);
            m14F8 = 2;
            m14FC = 0;
            fn_80239DAC(lbl_8087F3C0, this, 101, 1);
        } else {
            mPos += m1518;
        }
    } else if (m14F8 == 2) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            fn_802EBABC();
        }
    }
}

// inferred: state 8 (through a door): opens the door gimmick, turns to the exit and moves through
void Em305Vampire::fn_802EB1F4() {
    atn::Vector3 d(m1550.x - m1544.x, m1550.y - m1544.y, m1550.z - m1544.z);
    if (m14F8 == 0) {
        if (m14FC > m15AC) {
            m14FC = 0;
            m14F8 = 1;
            for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
                if (m155C->m08 == g->mId && m155C->m0C == g->m4C) {
                    GimmickMsg_unk msg(4);
                    g->vf40(&msg);
                    break;
                }
            }
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
        } else {
            atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
            mRot.y += 0.2f * (rot.y - mRot.y);
        }
    } else if (m14F8 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            fn_802EBABC();
        }
        float step = d.Length_unk() / mMotion.fn_80097D7C(0);
        mPos += d.GetNormalize() * step;
    }
}

// inferred: index of the point of the weaken route nearest to the vampire (names invented)
inline int Em305Vampire::NearestWeakenPoint_unk() {
    int found = 0;
    float best = 1e16f;
    for (unsigned int i = 0; i < m14E8.size(); i++) {
        atn::Vector3 d;
        d = mPos - m14E8[i]->mPos;
        if (PSVECSquareMag(&d) < best) {
            best = PSVECSquareMag(&d);
            found = i;
        }
    }
    return found;
}

// NONMATCHING: 99.33% - f3/f4/f5 of the hit point rate and r28/r29 of the two vectors of the route loop
// (~20 forms: locals, helpers, declaration orders)
// inferred: state 13 (weakened): weaken level from the hit points and nearest point of the weaken route
void Em305Vampire::fn_802EB61C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_802EBABC();
        return;
    }
    float f = mMotion.mTracks[0].m08;
    if (2.0f <= f && f <= 2.5f) {
        float rate = mStatus.m04 / (float)mStatus.mParamB.m84;
        if (rate < m15F4) {
            m1528 = 3;
        } else if (rate < m15F0) {
            m1528 = 2;
        } else {
            m1528 = 1;
        }
        if (m1528 == 1) {
            m1524 = m15E4[0] * 30;
        } else if (m1528 == 2) {
            m1524 = m15E4[1] * 30;
        } else if (m1528 == 3) {
            m1524 = m15E4[2] * 30;
        }
        m1560 = NearestWeakenPoint_unk();
        m1500 = 1;
    }
}

void Em305Vampire::fn_802EB814() {
    Rolling_unk();
}

void Em305Vampire::fn_802EB97C() {
    Stun_unk();
}

void Em305Vampire::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        vf9C(1);
        if (m1434 > 0) {
            m1434--;
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 0;
    }
}

void Em305Vampire::fn_802EBABC() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
}

void Em305Vampire::fn_802EBB14() {
    StartTackle_unk();
}

// inferred: starts state 7 (tackle) towards the target; the move is rotated by the facing when it is zero
void Em305Vampire::fn_802EBBA8() {
    if (m14F4 == 0) {
        return;
    }
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 7;
    fn_8016E970(6);
    m560 = 4;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    CharaTask* target = m14F4;
    atn::Vector3 d = target->mPos - mPos;
    m1518 = d;
    m1518.y = 0.0f;
    if (m1518.IsZero_unk()) {
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        atn::Vector3 front;
        front = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(m, &front, &front);
        m1518 = front;
    }
    m1518.Normalize_unk();
    mRot = m1518.ToRotation_unk();
    m1518 *= 2.5f;
}

void Em305Vampire::fn_802EBF40() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 13;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 0.2f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    vf9C(0);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1588, &mMotion);
    if (lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 4);
        fn_804DA4A0(lbl_8087F610, 0);
    }
}

void Em305Vampire::fn_802EC098() {
    StartFlee_unk();
}

void Em305Vampire::fn_802EC160() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 0.0f;
    mMotion.fn_80097C08(0, 2, 1, 1, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 100);
    PlayEffect_unk(&m1594, &mMotion);
    Effect_unk* mgr = lbl_8087F048;
    atn::Vector3 pos = fn_801781B0();
    fn_80108F38(mgr, &pos, 0x80);
}

void Em305Vampire::fn_802EC280() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    fn_800CB5C8(&m15A0, 5, 0);
    fn_800CB5C8(&m15A4, 5, 0);
    fn_800EB7A0(this);
}

// inferred: debug menu of the vampire
void Em305Vampire::fn_802EC38C() {
    void* group = m15BC.Load("em305");
    fn_80087994(group, "DebugDisp", &m15C0, 0, 0);
    fn_80088FAC(group, "WeakenColor", &m15C4, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_8008771C(group, "Speed1st", &m15D8[0], 0, 0, 0.1f, 10.0f, 0.1f);
    fn_800874C8(group, "Time1st", &m15E4[0], 0, 100, 1, 0, 0);
    fn_8008771C(group, "Speed2nd", &m15D8[1], 0, 0, 0.1f, 10.0f, 0.1f);
    fn_800874C8(group, "Time2nd", &m15E4[1], 0, 100, 1, 0, 0);
    fn_8008771C(group, "Speed3rd", &m15D8[2], 0, 0, 0.1f, 10.0f, 0.1f);
    fn_800874C8(group, "Time3rd", &m15E4[2], 0, 100, 1, 0, 0);
    fn_8008771C(group, "1stTo2nd", &m15F0, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_8008771C(group, "2ndTo3rd", &m15F4, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_800874C8(group, "WeakenType", &m1528, 0, 10, 1, 0, 0);
    fn_8008771C(group, "SpdUpRate", &m15F8, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_800874C8(group, "SpdUpTime", &m15FC, 0, 100, 1, 0, 0);
    fn_800874C8(group, "SpdUpWait", &m1600, 0, 100, 1, 0, 0);
    fn_800874C8(group, "xxxState", &m1530, 0, 100, 1, 0, 0);
    fn_800874C8(group, "xxxTime", &m1534, 0, 100, 1, 0, 0);
    fn_8008771C(group, "xxxSpeed", &m152C, 0, 0, 0.0f, 100.0f, 1.0f);
}

void Em305Vampire::vf108() {}
void Em305Vampire::vf104() {}
void Em305Vampire::vf100(int state) {}
void Em305Vampire::vfFC() {}
