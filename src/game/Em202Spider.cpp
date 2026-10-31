#include "game/Em202Spider.h"
#include "atn/Material_unk.h"
#include "math/Vec.h"
#include "game/FriendManager.h"
#include "game/CharaStatus.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_80097CCC(atn::Model* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: player manager (lbl_8087F8A0; first character at 0x48, as in game/CharaTask.cpp)
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
// inferred: link of the character lists of the managers (CharaTask + 0x14AC, as in game/CharaTask.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
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
// inferred: sends `id` to the gimmicks in a cone (game/SpecialEnemyBase.cpp)
extern "C" void fn_805A5078(const atn::Vector3* pos, const atn::Vector3* dir, int id, float radius, float angle);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);

// inferred: sounds of the spider
static const char* const sSe_unk[3] = {"SE_EM202_008", "SE_EM202_002", "SE_DG001_014"};
static const char* const sSeGroup_unk = "GROUP_DG001";

// NONMATCHING: 96.45% - written for the order of the strings; registers r28-r31 of the std::string
// constructors swapped (as in Em101LittleZesya)
Em202Spider::Em202Spider(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14D8(0), m14DC(55.0f), m14E4(0), m14E8(0), m14EC(0), m14F0(0), m14FC(0),
      m1500(0), m1504(0), m1508(0), m150C(0), m1510(0), m1514(0), m1518(0), m151C(0), m1520(0), m1524(0),
      m1528(0), m152C(0), m1534(60), m1538(0), m153C(0.0f), m1544(0), m1548(0), m154C(1) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m55C = 3;
    m14C0.clear();
    m14CC.clear();
    m14F4[0] = 0;
    m14F4[1] = 0;
    m151C = 0;
    m14B8 = 0;
    m14BC = 0;
    std::string dir("database/chara/special/");
    std::string name("db_em202_00_00.csv");
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
    m14B0.vf0C(path.c_str());
    m1550.LoadRequest("/effect/ef_uc019.efp");
    m155C.LoadRequest("/effect/ef_sk002b.eff");
    m1568.LoadRequest("/effect/ef_uc001.efp");
    m1574.LoadRequest("/effect/ef_sk002d.efp");
    m1580.LoadRequest("/effect/ef_uc007.efp");
    m158C.LoadRequest("/effect/ef_sc003_01b.efp");
}

Em202Spider::~Em202Spider() {}

// inferred: RouteTable_unk::FindPlace_unk with the array read inside the loop (as in game/Em208Queen.cpp)
inline RoutePlace_unk* FindRoutePlace2_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}
// inferred: as FindRoutePlace2_unk on the field table (register order of the place lookups of vf10)
inline RoutePlace_unk* FindRoutePlace3_unk(int id) {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}

// NONMATCHING: 99.90% - size/capacity registers of the inlined push_back (known open case) and the name of
// the pool of __allocate (lbl_80775A88, another unit)
int Em202Spider::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1550.IsLoading() == 0 &&
        m155C.IsLoading() == 0 && m1568.IsLoading() == 0 && m1574.IsLoading() == 0 &&
        m1580.IsLoading() == 0 && m158C.IsLoading() == 0 && m14B0.fn_80473F50() == 0) {
        if (m14B0.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "start_state") == 0) {
                    key = csv.fn_8005B9CC();
                    if (strcmp(key, "1st") == 0) {
                        m14E4 = 0;
                    } else if (strcmp(key, "2nd") == 0) {
                        m14E4 = 1;
                    }
                } else if (strcmp(key, "1st_needle_point") == 0) {
                    m14B8 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "NeedleChargeTime") == 0) {
                    m1534 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "tackle_point") == 0) {
                    m14BC = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "CrowSkillID") == 0) {
                    m1508 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "JumpAttackSkillID") == 0) {
                    m150C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "TackleSkillID") == 0) {
                    m1510 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "DashSkillID") == 0) {
                    m1514 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "NeedleSkillID") == 0) {
                    m1518 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "needle_point") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m14C0.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "outer_point") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m14CC.push_back(place);
                        }
                    } while (id != 0);
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        m14E0 = m5B0;
        m5B0 = 25.0f;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// inferred: front direction of the character (name invented)
inline atn::Vector3 FrontDir_unk(CharaTask* self) {
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = self->mRot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &fwd, &fwd);
    return fwd;
}

// inferred: difference on the ground plane (name invented)
inline void SetFlat_unk(atn::Vector3& v, float x, float z) {
    v.z = z;
    v.x = x;
    v.y = 0.0f;
}

// inferred: squared distance on the ground plane (name invented)
inline float FlatDistSq_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 d;
    SetFlat_unk(d, a.x - b.x, a.z - b.z);
    return PSVECSquareMag(&d);
}

// inferred: translation of a node matrix (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: effect `efp` on the model (names invented)
inline void PlayModelEffect_unk(Em202Spider* self, int id, atn::EfpFile* efp) {
    fn_80232B7C(self, id);
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
    fn_8023A8B4(lbl_8087F3C0, efp, &self->mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: attack hit of the skill (name invented)
inline void Em202Spider::Attack_unk(HitInfo_unk* const& skill) {
    int id = m590;
    fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, id, 30, -1, 0.0f);
}

// inferred: back to state 0 (names invented)
inline void Em202Spider::ResetState_unk() {
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    fn_800CB5C8(&m15A8, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

// NONMATCHING: 99.23% - stack slots (the original keeps the effect locals and the distance of the target loop
// below the capsule temporaries), order of the stores of m61C/m620, r4/r5 of id/index in the place lookups
// and of the skill/id loads; about 20 variants (named locals, components, inline helpers, references)
void Em202Spider::vf10() {
    atn::Vector3 pos;
    atn::Vector3 top;
    mC58.mC8 = (int)mC58.mC4; // inferred: previous target (mC8 is an int in the shared header)
    if (m58C == 6 || m58C == 9) {
        m1454 = 0;
    } else if (m14E4 == 0) {
        m1454 = 0;
        float best = 1e16f;
        CharaTask* target = 0;
        for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
            if (c->CanAct_unk()) {
                atn::Vector3 d = c->mPos - mPos;
                float dist = PSVECSquareMag(&d);
                if (dist < best) {
                    best = dist;
                    target = c;
                }
            }
        }
        m14EC = target;
    } else {
        m1454 = 1;
        m14EC = mC58.mC4;
    }
    mC58.mC4 = m14EC;
    m14D8++;
    if (mC58.mC0 == 0 || m14EC == 0) {
        fn_8016E970(3);
    } else {
        if (m154C != 0) {
            m154C = 0;
            fn_80232B7C(this, 1000);
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
            EffectMgr_unk* mgr = lbl_8087F3C0;
            fn_8023A8B4(mgr, &m1550, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        }
        if (m55C == 2) {
            fn_8016E970(3);
        }
        switch (m58C) {
        case 1: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk();
            } else {
                float f = mMotion.mTracks[0].m08;
                if (f >= 17.0f && f <= 22.0f) {
                    Attack_unk(m1508);
                }
            }
            break;
        }
        case 2: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk();
            } else {
                float f = mMotion.mTracks[0].m08;
                if (f >= 18.0f && f <= 20.0f) {
                    Attack_unk(m150C);
                }
            }
            break;
        }
        case 3: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk();
            } else {
                float f = mMotion.mTracks[0].m08;
                if (f >= 12.0f && f <= 17.0f) {
                    Attack_unk(m1510);
                }
            }
            break;
        }
        case 4:
            fn_8027F8A8();
            break;
        case 5:
            fn_8027FA7C();
            break;
        case 7: {
            RoutePlace_unk* place = FindRoutePlace3_unk(m1504);
            if (FlatDistSq_unk(mPos, place->mPos) < 100.0f) {
                m151C = 0;
                fn_80281924(m14EC);
            } else {
                fn_8027EB14();
            }
            break;
        }
        case 8: {
            RoutePlace_unk* place = FindRoutePlace3_unk(m1504);
            if (FlatDistSq_unk(mPos, place->mPos) < 100.0f) {
                if (m14F0 < 2) {
                    m1504 = m14F4[m14F0];
                    fn_8017039C(m1504, 2);
                    m14F0++;
                } else {
                    m151C = 0;
                    fn_80281924(m14EC);
                }
            } else {
                fn_8027EB14();
            }
            break;
        }
        case 6:
            fn_80280184();
            break;
        case 10:
            vfEC();
            break;
        case 9:
            fn_802805C8();
            break;
        case 11: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk();
            }
            break;
        }
        case 12:
            if (m14D8 >= 450) {
                ResetState_unk();
            }
            break;
        default:
            if (m55C != 6) {
                fn_8027F504();
            }
            if (m58C == 0) {
                if (m55C != 6) {
                    if (m55C == 7) {
                        if (m14EC != mRoute.mTarget) {
                            if (fn_802826A0(m14EC, 0) != 0) {
                                break;
                            }
                            fn_8016E970(3);
                        }
                    } else {
                        if (fn_802826A0(m14EC, 0) != 0) {
                            break;
                        }
                        m55C = 3;
                        fn_80170A20(m14EC, 0, -1.0f);
                    }
                }
                fn_8027EB14();
            }
            break;
        }
    }
    m5B0 = m14E0;
    fn_8014C540();
    fn_80145334();
    pos = mPos;
    pos.y -= 7.0f;
    if (m58C == 5) {
        atn::Vector3 off = FrontDir_unk(this) * 20.0f;
        pos += off;
        m620 = m5B0;
        m614 = pos;
        m614.y += m620;
    }
    top = pos;
    top.y = mPos.y + m5B0;
    mColli.m3C = pos;
    mColli.m48 = top;
    mColli.m54 = m5B0;
    m1548++;
    if (m58C == 6) {
        m1548++;
    }
    if (m1548 > 10 && (m58C - 6U <= 3 || m58C == 0)) {
        fn_8027F69C();
        m1548 = 0;
    }
}

// inferred: color of the spider when it is down (0.7, 0.2, 0.2, 1.0; made by the __sinit of
// game/Em202SpiderBase.cpp)
extern "C" SpiderColor_unk lbl_807C8398;

// NONMATCHING: 93.04% - stack slots of the color
// temporaries (the original keeps the difference above the product and saves f31)
void Em202Spider::vf14() {
    SpiderColor_unk target;
    if (IsDown_unk()) {
        target = lbl_807C8398;
    }
    m1598 = (target - m1598) * 0.15f + m1598;
    mMotion.fn_80092954("em202_body")->mColor = m1598.ToGX();
    mMotion.fn_80092954("em202_tail")->mColor = m1598.ToGX();
    fn_80149A30();
}

// inferred: the spider guards the hits from the front (from the back in the states 4 and 5)
// inferred: dot of the front of the spider with the direction to pos (name invented)
inline float FrontDot_unk(CharaTask* self, const atn::Vector3& pos) {
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    Mtx m;
    float ry = self->mRot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &fwd, &fwd);
    atn::Vector3 d(pos - self->mPos);
    return PSVECDotProduct(&d, &fwd);
}

void Em202Spider::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    bool down = true;
    unsigned int flags = mStatus.m0C;
    if ((flags & 0x80000) != 0x80000 && (flags & 0x1000000) != 0x1000000) {
        down = false;
    }
    if (!down && m58C != 11) {
        if (m58C - 4U <= 1) {
            msg->m10 = lbl_807C7030;
        }
        if (msg->mData->m01 != 1) {
            int guard = 0;
            float dot = FrontDot_unk(this, msg->m1C);
            if (m58C - 4U <= 1) {
                if (dot < -(float)cos(90.0)) {
                    guard = 1;
                }
            } else if (dot > (float)cos(90.0)) {
                guard = 1;
            }
            if (guard) {
                msg->m10 = lbl_807C7030;
                msg->m48 = 1;
            }
        }
    }
    if (m58C == 12) {
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

void Em202Spider::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m14FC += msg->m68;
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 0;
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        fn_800CB5C8(&m15A8, 15, 0);
        fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802829DC();
        return;
    }
    if (msg->mData != 0 && msg->mData->m04 == 0x1791) {
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 12;
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
        fn_8023A8B4(lbl_8087F3C0, &m158C, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        Effect_unk* mgr = lbl_8087F048;
        atn::Vector3 p = fn_801781B0();
        fn_80108F38(mgr, &p, 0x80);
    }
    switch (m14E4) {
    case 0:
        if (msg->mData->m01 == 1 || (float)m14FC >= 60.0f) {
            m14E8 = 1;
        }
        break;
    case 1:
        if (m58C != 4 && m58C != 5) {
            if (msg->mData->m01 == 1) {
                m1500 = 1;
            }
            m14FC = 0;
        }
        if (m14FC >= 120) {
            m1500 = 1;
        }
        break;
    }
}


// NONMATCHING: 99.31% - r3/r4 of the copy of the center (temporary of mPos + m5A4, __VecCopy family)
void Em202Spider::vf54() {
    if (m624.mSize != 0) {
        if (m58C == 5) {
            atn::Vector3 off = FrontDir_unk(this) * 20.0f;
            m624.mData[0].m04.mRadius = 1.5f * m5B0;
            m624.mData[0].m04.mCenter = mPos + m5A4 + off;
            m624.mData[0].m04.mCenter.y += 0.5f * m624.mData[0].m04.mRadius;
        } else {
            m624.mData[0].m04.mRadius = 1.5f * m5B0;
            m624.mData[0].m04.mCenter = mPos + m5A4;
            m624.mData[0].m04.mCenter.y += 0.5f * m624.mData[0].m04.mRadius;
        }
    }
}

void Em202Spider::vf7C(int arg) {
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 11;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}

// inferred: death sequence (after the death motion: effect m1568 at frame 29 of the countdown m1434)
void Em202Spider::vfEC() {
    if (mMotion.mTracks[0].m00 != 0x2E) {
        fn_802829DC();
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
            if (m1434 == 29) {
                fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
                fn_80232B7C(0, 0);
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
                fn_8023A8B4(lbl_8087F3C0, &m1568, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
                vf9C(1);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360();
        }
    } else {
        m1434 = 30;
    }
}

// inferred: result of a ray test (0x50 bytes, as in game/Em208Queen.cpp)
// inferred: hit polygon of a ray test (flags at 0x08, as in game/CharaTask.cpp); name invented
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
// inferred: nonzero when the sound group is loaded (game/Se_unk.cpp declares it with an int; same C symbol)
extern "C" int fn_800C3118(const char* group);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)


// inferred: negated vector (Vector3::operator- is out of line in the shared header); name invented
inline atn::Vector3 Negate_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: dash attack in front (names invented)
inline void Em202Spider::Dash_unk(const atn::Vector3& rot) {
    atn::Vector3 pos(mPos);
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', mRot.y);
    PSMTXMultVec(m, &fwd, &fwd);
    fn_805A5078(&pos, &fwd, 20, m1514->m58, *(float*)&m1514->m50[0]);
    fn_800F8C6C(lbl_8087F048, 0, 0, this, m1514, m590, 30, -1, rot.y - mRot.y);
}

// NONMATCHING: 99.76% - stack slots of the position/front vectors of the dash (the original puts them below
// the temporaries of the turn, at 0x134/0x128; in the body they go above them, in the helper at the bottom)
// inferred: movement of the frame (route in state 7, escape from the target when it is too near)
void Em202Spider::fn_8027EB14() {
    float speed = 0.0f;
    float rate = 1.0f;
    int route = 0;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 dir(mRoute.m58);
        speed = dir.Length_unk();
        if (speed > 0.0001f) {
            rot = dir.GetNormalize().ToRotation_unk();
        }
        dir = m14EC->mPos - mPos;
        dir.y = 0.0f;
        float dist = dir.Length_unk();
        if (m58C == 7) {
            mRot = atn::Vector3(0.0f, 0.0f, -1.0f).ToRotationZ_unk();
        } else {
            PSVECNormalize(&dir, &dir);
            dir.fn_80139F60();
            mRot.y = dir.y;
        }
        if (m58C - 6U > 2 && dist < m14DC) {
            if (dist < 0.8f * m14DC) {
                rot = Negate_unk(dir.GetNormalize().ToRotation_unk());
                rate = 1.0f;
            } else {
                rate = 0.0f;
            }
        }
        route = 1;
        if (m151C != 0) {
            rate *= 2.0f;
            if (m1514 != 0) {
                Dash_unk(rot);
            }
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    if (route) {
        fn_801426A4(&rot, speed, m568 * rate);
    } else {
        fn_8013CB68(rot, 0, speed, m568 * rate);
    }
}

// inferred: chooses the action of phase 0 (crow attacks, then the needle point) and phase 1
void Em202Spider::fn_8027F504() {
    if (m14EC != 0) {
        atn::Vector3 pos(*m14EC->fn_8013C38C());
        float dist = (pos - mPos).Length_unk();
        switch (m14E4) {
        case 0:
            if (m14E8 == 0) {
                if (m1524 < 2) {
                    if (dist < 50.0f) {
                        fn_80281054(m14EC);
                        m1524++;
                    }
                } else {
                    fn_80281ED8();
                    m1524 = 0;
                }
            } else {
                fn_80281634(m14EC, 6);
                m14E4 = 1;
                m14E8 = 0;
                m14FC = 0;
            }
            break;
        case 1:
            if (m1500 == 0 && m1544 < 3) {
                if (m14D8 > 60) {
                    if (m1524 < 2) {
                        if (dist < 50.0f) {
                            fn_80281054(m14EC);
                            m1524++;
                        }
                    } else if (dist < 50.0f) {
                        fn_80281344(m14EC);
                        m1524 = 0;
                        m1544++;
                    }
                }
            } else {
                fn_80281F88();
                m1524 = 0;
                m1544 = 0;
                m1500 = 0;
                m14FC = 0;
            }
            break;
        }
    }
}

// inferred: nodes of the tips of the arms
static const char* sArmNodes_unk[6] = {"leftbackarm2",  "leftmiddlearm2",  "leftfrontarm2",
                                       "rightbackarm2", "rightmiddlearm2", "rightfrontarm2"};

// NONMATCHING: 99.12% - the original keeps &mPos in a register for the whole loop (r22) and splits the
// scaling of the arm offset per component; 8 variants
void Em202Spider::fn_8027F69C() {
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
                fn_800C344C(sSe_unk[2], &result.m10, 0, -1, 1.0f);
                played = 1;
            }
            fn_80232B7C(0, 0);
            Color4f_unk color;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &m1580, 0, 0, result.m10, lbl_807C7030, color, -1, -1, 1, 1.0f);
        }
    }
}

void Em202Spider::fn_8027F8A8() {
    if (m1520 < 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m1520 = 0;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14C, 1, 0, 1, 0.0f, 0.1f);
        }
        return;
    }
    if (m1520 < 30) {
        m1520++;
        return;
    }
    switch (m1528) {
    case 5:
        fn_80281924(m14EC);
        break;
    case 6: {
        int place = m14BC;
        if (m14B8 > 0) {
            m14D8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 6;
            fn_8016E970(6);
            m560 = 4;
            m1504 = place;
            fn_8017039C(place, 2);
            m151C = 1;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 2.0f;
            mMotion.fn_80097C08(0, 0x14, 1, 1, 1, 0.0f, 0.1f);
        }
        break;
    }
    default:
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 0;
        fn_8016E970(3);
        fn_800CB5C8(&m15A8, 15, 0);
        fn_80239DAC(lbl_8087F3C0, this, 200, 0);
        break;
    }
    m1528 = 0;
    m1520 = -1;
}

// inferred: state 5 (needle attack: charge for m1534 frames, then shoot from the hips)
void Em202Spider::fn_8027FA7C() {
    switch (m1530) {
    case 0: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m1530 = 1;
            PlayModelEffect_unk(this, 100, &m1574);
            m15A8 = fn_800C344C(sSe_unk[0], &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x143, 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    }
    case 1:
        m1520++;
        if (m1520 >= m1534) {
            m1530 = 2;
            m1520 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 100, 1);
            fn_800CB5C8(&m15A8, 0, 0);
            fn_800C344C(sSe_unk[1], &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
        }
        break;
    case 2: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 7.0f && frame <= 9.0f) {
            atn::Model* model = &mMotion;
            MtxPtr m = model->GetNodeMtx_unk("hips");
            atn::Vector3 pos = GetNodePos_unk(m);
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx r;
            PSMTXRotRad(r, 'y', mRot.y);
            PSMTXMultVec(r, &dir, &dir);
            fn_800F8574(lbl_8087F048, this, m1518, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
            break;
        }
        float f = mMotion.mTracks[0].m08;
        if (12.5f < f && f < 13.5f) {
            m1520++;
            if (m1520 < 15) {
                mMotion.mTracks[0].m08 = 12.0f;
            }
            break;
        }
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            m152C++;
            if (m14E4 == 0 && m152C >= 2) {
                fn_80281634(m14EC, 6);
                m14E4 = 1;
            } else if (m14E4 == 1) {
                CharaTask* target = m14EC;
                if (target != 0) {
                    m14D8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    m58C = 6;
                    fn_8016E970(6);
                    m560 = 4;
                    m1504 = 0;
                    fn_80170A20(target, 2, 0.0f);
                    m151C = 1;
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 2.0f;
                    mMotion.fn_80097C08(0, 0x14, 1, 1, 1, 0.0f, 0.1f);
                }
            } else {
                ResetState_unk();
            }
        }
        break;
    }
    }
}

// inferred: state 6 (tackle): runs to the place m1504, or to the target (then state 3 in reach)
void Em202Spider::fn_80280184() {
    atn::Vector3 flat;
    m1520++;
    if (m1504 <= 0) {
        atn::Vector3 d = m14EC->mPos - mPos;
        float dist = PSVECSquareMag(&d);
        if (dist < 2500.0f) {
            fn_80281344(m14EC);
            m151C = 0;
            return;
        }
    } else {
        RoutePlace_unk* place = FindRoutePlace3_unk(m1504);
        SetFlat_unk(flat, mPos.x - place->mPos.x, mPos.z - place->mPos.z);
        if (PSVECSquareMag(&flat) < 100.0f) {
            ResetState_unk();
            m151C = 0;
            return;
        }
    }
    fn_8027EB14();
}

// inferred: state 9 (turn of m153C radians), then the action m1538 on the target m1540
void Em202Spider::fn_802805C8() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        switch (m1538) {
        case 0:
            ResetState_unk();
            break;
        case 1:
            fn_80281054(m1540);
            break;
        case 2:
            fn_80281344(m1540);
            break;
        case 3:
            fn_80281344(m1540);
            break;
        case 5:
            fn_80281924(m1540);
            break;
        }
    } else {
        float f = mMotion.mTracks[0].m08;
        if (5.0f <= f && f <= 16.0f) {
            float speed = mMotion.mTracks[0].m0C;
            mRot.y += (m153C * speed) / 12.0f;
        }
    }
}

void Em202Spider::fn_80281054(CharaTask* target) {
    if (fn_802826A0(target, 1) != 0) {
        return;
    }
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 1;
    fn_8016E970(6);
    m560 = 4;
    atn::Vector3 pos(target->mPos);
    mRot = (pos - mPos).GetNormalize().ToRotation_unk();
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
}

void Em202Spider::fn_80281344(CharaTask* target) {
    if (fn_802826A0(target, 3) != 0) {
        return;
    }
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 3;
    fn_8016E970(6);
    m560 = 4;
    atn::Vector3 pos(target->mPos);
    mRot = (pos - mPos).GetNormalize().ToRotation_unk();
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
}

void Em202Spider::fn_80281634(CharaTask* target, int next) {
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 4;
    fn_8016E970(3);
    m1528 = next;
    m1520 = -1;
    atn::Vector3 pos(target->mPos);
    mRot = (pos - mPos).GetNormalize().ToRotation_unk();
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14B, 0, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 99.84% - r4/r5 of the zero vector copy and r29/r30 of the counter; 8 variants
// inferred: starts state 5 (attack): faces backwards in the phase 0, else the center of the party
void Em202Spider::fn_80281924(CharaTask* target) {
    if (fn_802826A0(target, 5) != 0) {
        return;
    }
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 5;
    fn_8016E970(3);
    atn::Vector3 center;
    if (m14E4 == 0) {
        mRot = atn::Vector3(0.0f, 0.0f, -1.0f).ToRotationZ_unk();
    } else {
        CharaTask* player = lbl_8087F8A0->mHead;
        if (player->mFlag28) {
            center = player->mPos;
        } else {
            center = lbl_807C7030;
            int count = 1;
            center = player->mPos;
            for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
                if ((c->mStatus.m0C & 0x20) != 0x20) {
                    count++;
                    center += c->mPos;
                }
            }
            float inv = 1.0f / count;
            center *= inv;
        }
        mRot = (center - mPos).GetNormalize().ToRotation_unk();
    }
    m1520 = 0;
    m1530 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
}

void Em202Spider::fn_80281ED8() {
    if (m14B8 > 0) {
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 7;
        m1504 = m14B8;
        fn_8017039C(m14B8, 2);
        m1520 = 0;
        m151C = 1;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 2.0f;
        mMotion.fn_80097C08(0, 0x14, 0, 0, 1, 0.0f, 0.1f);
    }
}

// inferred: squared length of a temporary vector (name invented)
inline float DistSq_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: index wrapped into [0, n) (name invented)
inline int WrapIndex_unk(int i, int n) {
    if (i < 0) {
        i = n - 1;
    }
    if (i >= n) {
        i = 0;
    }
    return i;
}

// inferred: starts state 8: finds the wall at the left or the right (whichever is nearer), the outer
// point nearest to it and the two needle points around it (m14F4), and runs to the outer point
// NONMATCHING: 97.51% - r29/r30 of this and the temporaries, stack slots of the distance temporaries (the
// original keeps them at the bottom of the frame); 4 variants (local order, FrontDir_unk, named vectors)
void Em202Spider::fn_80281F88() {
    int outer = 0;
    CollResult_unk left;
    CollResult_unk right;
    atn::Vector3 to;
    atn::Vector3 wall;
    to = atn::Vector3(1000.0f * -FrontDir_unk(this).x, 0.0f, 0.0f);
    to += mPos;
    int hitLeft = fn_8004ED34(lbl_8087EE98, &left, &mPos, &to, 0x80000000, 0, 0);
    if (hitLeft != 0 && (left.m38->m08 & 4) != 4) {
        hitLeft = 0;
    }
    to = atn::Vector3(0.0f, 0.0f, 1000.0f * -FrontDir_unk(this).z);
    to += mPos;
    int hitRight = fn_8004ED34(lbl_8087EE98, &right, &mPos, &to, 0x80000000, 0, 0);
    if (hitRight != 0 && (right.m38->m08 & 4) != 4) {
        hitRight = 0;
    }
    if (hitLeft != 0 && hitRight != 0) {
        wall = left.m10;
        float dist = DistSq_unk(mPos - left.m10);
        if (DistSq_unk(mPos - right.m10) < dist) {
            wall = right.m10;
        }
    } else if (hitLeft != 0) {
        wall = left.m10;
    } else if (hitRight != 0) {
        wall = right.m10;
    } else {
        return;
    }
    float best = 1e16f;
    for (unsigned int i = 0; i < m14CC.size(); i++) {
        float dist = DistSq_unk(wall - m14CC[i]->mPos);
        if (best > dist) {
            best = dist;
            outer = i;
        }
    }
    atn::Vector3 center = lbl_807C7030;
    int count = 1;
    center = lbl_8087F8A0->mHead->mPos;
    for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
        if ((c->mStatus.m0C & 0x20) != 0x20) {
            count++;
            center += c->mPos;
        }
    }
    float inv = 1.0f / count;
    center *= inv;
    int n = m14C0.size();
    int needle;
    if (outer % 2 == 1) {
        int a = WrapIndex_unk(outer / 2 - 1, n);
        int b = WrapIndex_unk(outer / 2 + 1, n);
        float da = DistSq_unk(center - m14C0[a]->mPos);
        if (da > DistSq_unk(center - m14C0[b]->mPos)) {
            needle = a;
        } else {
            needle = b;
        }
    } else {
        needle = outer / 2;
    }
    m14F4[0] = m14C0[needle]->mId;
    int a = WrapIndex_unk(needle - 1, n);
    int b = WrapIndex_unk(needle + 1, n);
    float da = DistSq_unk(center - m14C0[a]->mPos);
    if (da > DistSq_unk(center - m14C0[b]->mPos)) {
        m14F4[1] = m14C0[a]->mId;
    } else {
        m14F4[1] = m14C0[b]->mId;
    }
    m14F0 = 0;
    m1504 = m14CC[outer]->mId;
    fn_8017039C(m1504, 2);
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 8;
    m1520 = 0;
    m151C = 1;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14, 0, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 99.88% - f0/f3 of the 0.0f of ToRotationZ_unk (same case as Em003LesserDaemon::vf24)
int Em202Spider::fn_802826A0(CharaTask* target, int arg) {
    if (m58C == 9) {
        return 0;
    }
    float ry = mRot.y;
    atn::Vector3 rot = (target->mPos - mPos).ToRotationZ_unk();
    m153C = atn::WrapAngle_unk(rot.y - ry);
    if (-0.3926991f < m153C && m153C < 0.3926991f) {
        return 0;
    }
    m1538 = arg;
    m1540 = target;
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 9;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 1, 1, 0.0f, 0.1f);
    return 1;
}

void Em202Spider::fn_802829DC() {
    if (m58C == 5 && m1530 == 1) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
    fn_800CB5C8(&m15A8, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 10;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0(this);
}
