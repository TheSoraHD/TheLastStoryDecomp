#include "game/Em306GurgBeast.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaStatus.h"
#include "game/EnemyManager.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/CharaC58_unk.cpp)
struct Field_unk {
    unsigned char m0000[0x6C];
    unsigned char m006C[0x10D8 - 0x6C]; // inferred: object of fn_8037EF30
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);
extern "C" void fn_801028E4(Effect_unk* mgr, int arg, CharaTask* chara, int arg2);
extern "C" void fn_80102824(Effect_unk* mgr, CharaTask* chara, PlayerTask* leader);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" float fn_80011220(float x);       // inferred: fabs (weak copy of another file)
extern "C" float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b); // inferred: dot product
Effect_unk* fn_8013A194();                   // inferred: effect/bullet manager (weak copy of another file)
void* fn_801404F8();                         // inferred: collision world (weak copy of another file)
// inferred: game settings (lbl_8087F0A8); frames per second at 0x30 (as in gmk/Cannonball.cpp)
struct BeastSettings_unk {
    unsigned char m00[0x30];
    int mFps;
};
extern "C" BeastSettings_unk* fn_800F52F8();
// inferred: frame parameters (lbl_8087EFA8, as in gmk/SparkCube.cpp)
struct BeastParams_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" BeastParams_unk* lbl_8087EFA8;
extern "C" BeastSettings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" double sqrt(double x);
// inferred: sweep result (as in gmk/Cannonball.cpp); the constructor is called out of line
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: position reached
    unsigned char m1C[0x34 - 0x1C];
    int* m34; // inferred: attribute of the hit (copied to CharaTask::m634)
    int m38;
    unsigned int m3C; // inferred: flags (bit 0: on the ground)
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004D388(void* world, CollResult_unk* result, atn::Vector3* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: attack hit
// inferred: weak copies emitted after vf108 (fn_802F0964 is also called by game/Em312DarkServant.cpp,
// fn_802F0998 by gmk/Cannonball.cpp and gmk/FlamingArrow.cpp)
// NONMATCHING (fn_802F0998): 56% - the original loads settings->mFps before the lis of the int->float constant
// (scheduling of the prologue; 6 forms)
extern "C" inline void fn_802F0964(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, int time, int c,
                                   float f) {
    fn_800F8C6C(mgr, 0, 0, owner, bullet, id, time, c, f);
}
extern "C" inline float fn_802F0988(BeastParams_unk* params) {
    return params->mTimeScale;
}
extern "C" inline BeastParams_unk* fn_802F0990() {
    return lbl_8087EFA8;
}
extern "C" inline float fn_802F0998(BeastSettings_unk* settings) {
    return -98.1f / (float)(settings->mFps * settings->mFps);
}

extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button);           // inferred: button triggered
extern "C" int fn_800A55D4(void* pad, int port, int button);           // inferred: button held
extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg);  // inferred: stick axis
extern "C" void fn_8037EF30(void* obj, int a, int b, float f);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" atn::Vector3 lbl_807C7030;                // inferred: zero vector
// inferred: name invented (as in game/CharaTask.cpp)
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}
// inferred: camera (lbl_8087EFB4); m134 is its rotation around y
struct BeastCamera_unk {
    unsigned char m000[0x134];
    float m134;
};
extern "C" BeastCamera_unk* lbl_8087EFB4;
// inferred: tutorial message request (as DropTutorial_unk of gmk/DropItem.cpp); only set while none is shown
struct BeastTutorial_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
};
struct BeastMessage_unk {
    unsigned char m000[0x764];
    BeastTutorial_unk m764;
};
extern "C" BeastMessage_unk* lbl_8087F490;
// inferred: requests a tutorial message unless one is shown (name invented)
inline void ShowTutorial_unk(int a, int b) {
    BeastTutorial_unk tutorial;
    tutorial.m08 = -1;
    tutorial.m0C = 0;
    tutorial.m10 = 0;
    tutorial.m18 = 0;
    tutorial.m14 = 1;
    tutorial.m00 = a;
    tutorial.m04 = b;
    if (lbl_8087F490->m764.m14 == 0) {
        lbl_8087F490->m764 = tutorial;
    }
}

// inferred: the rider's hit handler is called with a second argument (CharaTask::vf3C takes one)
struct Rider_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C(HitInfo_unk* hit, int arg);
};

Em306GurgBeast::Em306GurgBeast(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14D8(0.0f, 0.0f, 0.0f), m14EC(0x640), m14F0(30), m1518(0),
      m1520(0), m1524(0), m1528(0), m1530(0), m1534(0), m1538(0), m153C(0), m1540(0), m1544(0), m1548(0),
      m154C(0), m1550(-1), m1554(3), m1558(3), m155C(300), m1560(0.0f) {
    m14E8 = 0;
    m14E4 = 0;
    m152C = 0;
    const char* s = strstr(info->m2C, "ride=");
    if (s != 0) {
        m14E4 = fn_800DC12C(s + 5);
    }
    if (strstr(info->m2C, "rideplayer") != 0) {
        m1524 = 1;
    }
    if (m1524 != 0 && strstr(info->m2C, "reride") != 0) {
        m1528 = 1;
    }
    char path[0x100];
    char name[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em306_00_00.csv\0");
    }
    m14B4.vf0C(path);
}

Em306GurgBeast::~Em306GurgBeast() {}

int Em306GurgBeast::vf0C() {
    int ready = 1;
    if (fn_8013655C() != 0) {
        ready = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        ready = 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        ready = 0;
    }
    if (ready) {
        fn_802EE9A8((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        m14E8 = EnemyManager::sInstance->Find_unk(m14E4);
        if (m14E8 != 0) {
            MtxPtr seat = mMotion.GetNodeMtx_unk("seatpoint");
            if (seat != 0) {
                m14E8->mF1C = (int)seat;
            }
            if (m1524 != 0) {
                m1548 = 1;
            }
        }
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        vfFC();
    }
    return ready;
}

void Em306GurgBeast::fn_802EE9A8(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "SkillId") == 0) {
            m14EC = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "WaitFrame") == 0) {
            m14F0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HomePoint") == 0) {
            RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
            if (table != 0) {
                const char* s = csv.fn_8005B9CC();
                while (s != 0 && *s != '\0' && m14F4.size() < 8) {
                    RoutePlace_unk* place = table->FindPlace_unk(atoi(s));
                    if (place != 0) {
                        m14F4.push_back(place);
                    }
                    s = csv.fn_8005B9CC();
                }
            }
        }
    } while (csv.fn_8005BBF8());
}

// inferred: CharaTask::CanAct_unk written out (as in game/Em303KrakenFoot.cpp)
inline bool BeastCanAct_unk(CharaTask* c) {
    bool r = false;
    bool free = false;
    bool active = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        active = true;
    }
    if (active && (c->mStatus.m0C & 0x20) != 0x20) {
        free = true;
    }
    if (free && !c->IsGrappled_unk()) {
        r = true;
    }
    return r;
}

inline void Em306GurgBeast::CopyTrack_unk(Model_unk* dst, int i) {
    dst->mTracks[i].m20 = mMotion.mTracks[i].m20;
    dst->mTracks[i].m0C = mMotion.mTracks[i].m0C;
    dst->mTracks[i].m08 = mMotion.mTracks[i].m08;
    dst->fn_80097C08(i, mMotion.mTracks[i].m00, mMotion.mTracks[i].m18, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 97.62% - registers of the inlined CanAct_unk, r28-r30 of the track copy loop and the stack
// slots of the capsule points (~14 forms: helpers, local orders, constructors)
void Em306GurgBeast::vf10() {
    if (!CharaTask::mFlag2 || !BeastCanAct_unk(this)) {
        vf104();
    }
    if (m14E8 != 0) {
        m14E8->fn_801765D8();
        int count = mMotion.m34C;
        m14E8->mMotion.m34C = count;
        for (int i = 0; i < count; i++) {
            m14E8->GetModel_unk()->mTracks[i].m20 = mMotion.mTracks[i].m20;
            m14E8->GetModel_unk()->mTracks[i].m0C = mMotion.mTracks[i].m0C;
            m14E8->GetModel_unk()->mTracks[i].m08 = mMotion.mTracks[i].m08;
            m14E8->GetModel_unk()->fn_80097C08(i, mMotion.mTracks[i].m00, mMotion.mTracks[i].m18, 0, 1, 0.0f, 0.1f);
        }
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
    m614 = mPos + m5A4;
    m620 = m5B0;
    m614.y += m620;
    atn::Vector3 top;
    atn::Vector3 base(mPos.x, mPos.y + m620, mPos.z);
    int index = mMotion.fn_80092814("tail", 0);
    if (index > -1) {
        MtxPtr tail = mMotion.mNodeMtx[index];
        top = atn::Vector3(tail[0][3], tail[1][3], tail[2][3]);
        top.y -= 1.0f;
    } else {
        top = base;
        top.y += m5B4 - 2.0f * m620;
    }
    mColli.m3C = base;
    mColli.m48 = top;
    mColli.m54 = m620;
    if (m14E8 != 0) {
        m14E8->fn_80145334();
        m14E8->mLook.fn_80129A6C();
        if (m1524 != 0) {
            if (m14E8 != 0 && m1558 == 0) {
                vf100(17);
                m1558--;
            } else if ((m14E8->mStatus.m0C & 0x20) == 0x20) {
                m14E8->fn_80176ACC();
                m14E8->mF1C = 0;
                m14E8->mLook.fn_80129A48(0.3f);
                m14E8->mMotion.m34C = 1;
                m14E8->mMotion.mTracks[0].m20 = 1.0f;
                m14E8->mMotion.mTracks[0].m0C = 1.0f;
                m14E8->mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
                m14E8 = 0;
                fn_801028E4(lbl_8087F048, 0, this, 0);
                fn_8015495C(0, 1, 1, 1);
                mC58.mC4 = 0;
                mF18 = mF14;
                mF14 = 0;
                vf100(14);
                m1550 = 300;
            }
        }
    }
    if (m1524 != 0) {
        fn_802F1120();
    }
}


void Em306GurgBeast::vf14() {
    fn_80149A30();
    if (m14E8 != 0) {
        m14E8->fn_80149A30();
    }
}

int Em306GurgBeast::vf10C(HitMsg_unk* msg) {
    msg->m10 *= 0.0f;
    if (m1548 != 0) {
        if (m14E8 != 0) {
            msg->m04 = m14E8;
            ((Rider_unk*)m14E8)->vf3C((HitInfo_unk*)msg, 1);
            m1558--;
        } else if (m152C != 0) {
            msg->m04 = m152C;
            ((Rider_unk*)m152C)->vf3C((HitInfo_unk*)msg, 1);
            if (m154C != 0) {
                m1554--;
            }
        }
        return 0;
    }
    return 1;
}

void Em306GurgBeast::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
    }
}

void Em306GurgBeast::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

// inferred: starts a state (8-13: attacks, 14: walk, 15/16: the leader gets on, 17: stop, 2: down)
void Em306GurgBeast::vf100(int state) {
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
        break;
    case 8:
    case 9:
        fn_805A4F1C(0x143, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    case 10:
    case 11:
        fn_805A4F1C(0x13F, 0, 1, 1.0f, 0.0f);
        m14C8 = fn_800F8548(lbl_8087F048);
        mRot.y = m14D8.ToRotation_unk().y;
        mFlag30 = 0;
        break;
    case 12:
        fn_805A4F1C(0x141, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    case 13:
        fn_805A4F1C(0x144, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    case 2:
        if (mF14 == 1) {
            if ((mStatus.m0C & 0x20) == 0x20 && prev != 2) {
                vf44();
            }
            if (m55C != 2) {
                fn_8016E970(2);
            }
            fn_805A4F1C(0x2E, 0, 1, 1.0f, 0.0f);
            mFlag30 = 0;
            if (m14E8 != 0) {
                m14E8->fn_80176ACC();
                m14E8->mF1C = 0;
                m14E8->mLook.fn_80129A48(0.3f);
                m14E8 = 0;
            }
        }
        break;
    case 14:
        mFlag30 = 1;
        mMotion.fn_80097C08(0, 0x142, 1, 0, 1, 0.0f, 0.1f);
        break;
    case 15:
        lbl_8087F8A0->mHead->fn_80163E70(this);
        break;
    case 16:
        m152C = lbl_8087F8A0->mHead;
        if (m152C != 0) {
            m1530 = (int)mMotion.GetNodeMtx_unk("seatpoint");
            if (m1530 != 0) {
                m152C->mF1C = m1530;
            }
        }
        break;
    case 17:
        fn_805A4F1C(0x143, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    }
}

// inferred: moves the beast with the stick, relative to the camera (name invented)
inline void Em306GurgBeast::RideMove_unk() {
    atn::Vector3 dir;
    dir.x = -fn_800A56A8(lbl_8087EF70, 0, 0, 0);
    dir.y = 0.0f;
    dir.z = -fn_800A56A8(lbl_8087EF70, 0, 1, 0);
    float rate = dir.Length_unk();
    if (rate < 0.4f) {
        dir = mRot;
    } else {
        float cam = lbl_8087EFB4->m134;
        dir.fn_80139F60();
        dir.y = cam - dir.y;
    }
    fn_802F09D4(&dir, rate);
}

// inferred: front vector for the rotation ry around y (name invented)
inline atn::Vector3 BeastFront_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}

// inferred: state machine: 6 wait, 7 choose a goal, 8/9 turn, 10/11 charge, 12/13 back to a home point, 14 the
// leader may get on (button 4), 15/16 ridden by the leader (stick moves it, button 5 throws him off), 17 the
// rider is dropped; 2 down
// NONMATCHING: 99.60% - register coloring only: the inlined CanAct_unk (r0/r4, known blocker), r29/r30 of the
// node lookups of state 7 and of the stick direction, r28/r30 of the home point loop (~25 forms)
void Em306GurgBeast::vf104() {
    m14C0++;
    if (CanAct_unk() && m58C != 6 && mF14 != 0 && vfF8() == 0 && m152C == 0) {
        vf100(6);
    }
    switch (m58C) {
    case 6:
        if (vfF8() != 0) {
            vf100(7);
        }
        vf108();
        break;
    case 7:
        if (mFlag31) {
            if (m1518 == 0) {
                m14D4 = mC58.mC4;
                if (m14E8 != 0 && m14D4 != 0) {
                    if (m14D4->mMotion.GetNodeMtx_unk("neck") != 0) {
                        m14E8->mLook.fn_80129978(&m14D4->mMotion, "neck", lbl_807C7030, 0.3f);
                    } else if (m14D4->mMotion.GetNodeMtx_unk("head") != 0) {
                        m14E8->mLook.fn_80129978(&m14D4->mMotion, "head", lbl_807C7030, 0.3f);
                    } else {
                        m14E8->mLook.fn_80129978(&m14D4->mMotion, 0, lbl_807C7030, 0.3f);
                    }
                }
                vf100(8);
            } else {
                vf100(9);
            }
        }
        vf108();
        break;
    case 8:
    case 9:
        if (mFlag31) {
            int next = 11;
            if (m58C == 8) {
                next = 10;
            }
            vf100(next);
        }
        vf108();
        break;
    case 10:
    case 11:
        if (mFlag31) {
            vf100(m14C4);
        }
        vf108();
        break;
    case 12:
    case 13:
        if (mFlag31) {
            vf100(7);
            if (m1518 != 0) {
                m14C0 = m14F0;
            }
            if (m1518 == 0) {
                float best = 1e16f;
                for (unsigned int i = 0; i < m14F4.size(); i++) {
                    RoutePlace_unk* place = m14F4.mData[i];
                    float dist = LengthSq_unk(place->mPos - mPos);
                    if (dist < best) {
                        m1518 = place;
                        best = dist;
                    }
                }
            } else {
                m1518 = 0;
            }
        }
        vf108();
        break;
    case 2:
        if (mF14 == 1) {
            fn_805A40B8();
        }
        fn_802F1320(0);
        break;
    case 14:
        if (mFlag31) {
            PlayerTask* player = lbl_8087F8A0->mHead;
            atn::Vector3 d = mPos - player->mPos;
            if (d.Length_unk() < 20.0f && player->fn_8016DDB0(0)) {
                if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
                    fn_802F1240(player);
                }
                ShowTutorial_unk(6, 17);
            }
            if (m1528 == 0) {
                if (m1550 >= 0) {
                    m1550--;
                }
                if (m1550 == 0) {
                    mF18 = mF14;
                    mF14 = 1;
                    vf100(6);
                    m1548 = 0;
                }
            }
        }
        vf108();
        break;
    case 15:
        if (lbl_8087F8A0->mHead->mMotion.mTracks[0].m08 > 31.0f) {
            vf100(16);
        }
        break;
    case 16:
        if (m152C != 0) {
            if (m55C == 7) {
                vf108();
                break;
            }
            if ((m152C->mStatus.m0C & 0x20) == 0x20) {
                fn_802F1320(1);
            }
            if (m1528 == 0) {
                CharaTask* c = mC58.mC4;
                if (c == 0 || (c != 0 && (c->mStatus.m0C & 0x20) == 0x20)) {
                    if (m155C > 0) {
                        m155C--;
                    }
                    if (m155C == 0) {
                        vf100(17);
                        break;
                    }
                }
            } else {
                m155C = 300;
            }
            RideMove_unk();
        } else {
            fn_802F09D4(&mRot, 0.0f);
        }
        fn_802F0F60();
        if (fn_800A555C(lbl_8087EF70, 0, 5) != 0) {
            fn_802F1320(1);
        } else if (fn_800A555C(lbl_8087EF70, 0, 9) != 0) {
            if (lbl_8087F430 != 0) {
                fn_8037EF30(lbl_8087F430->m006C, 0, 0, -1.0f);
            }
        } else if (fn_800A55D4(lbl_8087EF70, 0, 4) != 0) {
            m154C = 1;
        } else {
            m154C = 0;
        }
        if (mF14 == 1) {
            fn_802F1320(0);
        }
        if (m1554 <= 0) {
            vf100(17);
        }
        break;
    case 17:
        if (m14BC == 0 && mMotion.mTracks[0].m08 >= 30.0f) {
            if (m152C != 0) {
                PlayerTask* head;
                if (lbl_8087F048 != 0 && lbl_8087F8A0 != 0 && (head = lbl_8087F8A0->mHead) != 0) {
                    fn_80102824(lbl_8087F048, this, head);
                }
                m152C->fn_8016E970(1);
                m152C->fn_801446F0();
                m152C->mF1C = 0;
                atn::Vector3 dir = BeastFront_unk(m152C->mRot);
                Mtx r;
                PSMTXRotRad(r, 'y', 3.1415927f);
                PSMTXMultVec(r, &dir, &dir);
                dir *= 2.0f;
                m152C->m574 = dir;
                m152C->fn_8015E7A0(dir, -1, 0);
                m152C = 0;
                if (m155C == 0) {
                    mF18 = mF14;
                    mF14 = 1;
                    vf100(6);
                    m1548 = 0;
                } else {
                    vf100(14);
                    m1550 = 300;
                }
            } else if (m14E8 != 0) {
                m14E8->fn_80176ACC();
                m14E8->mF1C = 0;
                m14E8->mLook.fn_80129A48(0.3f);
                atn::Vector3 dir = BeastFront_unk(m14E8->mRot);
                Mtx r;
                PSMTXRotRad(r, 'y', 3.1415927f);
                PSMTXMultVec(r, &dir, &dir);
                dir *= 2.0f;
                m14E8->m574 = dir;
                m14E8->fn_8015E7A0(dir, -1, 0);
                m14E8 = 0;
                fn_801028E4(lbl_8087F048, 0, this, 0);
                fn_8015495C(0, 1, 1, 1);
                mC58.mC4 = 0;
                mF18 = mF14;
                mF14 = 0;
                vf100(14);
                m1550 = 300;
            }
            m14BC++;
        }
        mFlag30 = 0;
        break;
    default:
        vf100(6);
        break;
    }
}

// inferred: movement of the states (6/7 wait, 8/9 turn to the goal, 10/11 charge while firing the skill
// m14EC, 12 jump, 13 skid, 14 end); every inline is called out of line here (inline budget)
// NONMATCHING: 97.84% - r5/r7 order of the two fn_802F0964 calls (skill and its m5C); the jump table name
// differs only in the symbol (~8 forms)
void Em306GurgBeast::vf108() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot(mRot);
    if (m55C == 2) {
    switch (m58C) {
    case 6:
        mFlag31 = 1;
        break;
    case 7:
        if (m14C0 > m14F0) {
            mFlag31 = 1;
        }
        break;
    case 8:
    case 9: {
        atn::Vector3 goal(m1518 == 0 ? *m14D4->fn_8013C38C() : m1518->mPos);
        atn::Vector3 d = goal - mPos;
        if (!(fn_80011220(d.LengthSq_unk()) < 1.1920929e-7f)) {
            d.Normalize_unk();
        }
        m14D8 = d;
        m14D8.y = 0.0f;
        float frame = mMotion.fn_80139550(0);
        if (frame >= mMotion.fn_80097D7C(0)) {
            mFlag31 = 1;
        }
        if (frame >= 15.0f) {
            rot = m14D8.ToRotation_unk();
        }
        break;
    }
    case 10:
    case 11:
        if (m14BC == 0) {
            float len = mMotion.fn_80097D7C(0);
            if (mMotion.fn_80139550(0) >= len) {
                m14BC++;
                mMotion.fn_80097C08(0, 0x140, 1, 0, 1, 0.0f, 0.1f);
            }
        } else if (m14BC == 1) {
            atn::Vector3 goal(m1518 == 0 ? *m14D4->fn_8013C38C() : m1518->mPos);
            atn::Vector3 d = goal - mPos;
            d.y = 0.0f;
            if (!(fn_80011220(d.LengthSq_unk()) < 1.1920929e-7f)) {
                d.Normalize_unk();
            }
            float step = 0.009424779f;
            atn::Vector3 diff = d.ToRotation_unk() - m14D8.ToRotation_unk();
            diff.y = atn::WrapAngle_unk(diff.y);
            Mtx m;
            ((atn::Matrix3x4*)m)->fn_8013A13C(diff.y > 0.0f ? step : -step);
            m14D8.fn_80011410(*(atn::Matrix3x4*)m);
            speed = 1.0f;
            rot = m14D8.ToRotation_unk();
            rate = 2.0f;
            if (m58C == 10) {
                if (!m14D4->CanAct_unk() || m14D4->fn_801533C8(m14C8, 0)) {
                    mFlag31 = 1;
                    if (fn_801A03E8(m14D8, d) > 0.9659258f) {
                        m14C4 = 13;
                    } else {
                        m14C4 = 12;
                    }
                } else if (fn_801A03E8(m14D8, d) < -0.25881904f) {
                    mFlag31 = 1;
                    m14C4 = 13;
                } else if (m14C0 > 180) {
                    mFlag31 = 1;
                    m14C4 = 8;
                } else {
                    HitInfo_unk* skill = fn_80219E6C(m14EC);
                    fn_802F0964(fn_8013A194(), this, skill, m14C8, skill->m5C, -1, 0.0f);
                }
            } else if (fn_801A03E8(m14D8, d) < -0.25881904f) {
                mFlag31 = 1;
                m14C4 = 12;
            } else if (m14C0 > 180) {
                mFlag31 = 1;
                m14C4 = 12;
            } else {
                HitInfo_unk* skill = fn_80219E6C(m14EC);
                fn_802F0964(fn_8013A194(), this, skill, m14C8, skill->m5C, -1, 0.0f);
            }
        }
        break;
    case 12: {
        float frame = mMotion.fn_80139550(0);
        if (frame >= mMotion.fn_80097D7C(0)) {
            mFlag31 = 1;
        }
        if (frame <= 57.0f) {
            float k = 2.0175438f;
            float scale = fn_802F0988(fn_802F0990());
            atn::Vector3 move = m14D8 * k;
            move *= scale;
            CollResult_unk result;
            fn_8004D388(fn_801404F8(), &result, &m614, &move, 0x80, &mColli, 0, m620);
            mPos = result.m10;
            mPos -= m5A4;
            mPos.y -= m620;
            if ((result.m3C & 1) != 1) {
                m14D8.y += fn_802F0998(fn_800F52F8());
            } else {
                m14D8.y = 0.0f;
            }
            mRot.y += 0.055115663f;
            return;
        }
        break;
    }
    case 13: {
        float frame = mMotion.fn_80139550(0);
        if (frame >= mMotion.fn_80097D7C(0)) {
            mFlag31 = 1;
        }
        if (frame <= 15.0f) {
            atn::Vector3 vel(m574);
            float v = m570;
            fn_8013CB68(&rot, 1, speed, rate);
            m574.x = vel.x;
            m574.z = vel.z;
            m570 = v;
            return;
        }
        break;
    }
    case 14:
        mFlag31 = 1;
        break;
    case 16:
        return;
    }
    } else {
        fn_805A4254();
        return;
    }
    fn_8013CB68(&rot, 0, speed, m568 * rate);
}

// inferred: moves the beast towards the rotation `target` (turn limited to 90 degrees, speed from `rate` above
// 0.4), with gravity and the sweep test; the rider m152C is left out of the test. Hits around it while
// m1560 >= 2 (skill m14EC). As CharaTask::fn_801426A4.
// NONMATCHING: 99.72% - f3/f4 swapped between m574.z and rot.y in the damping/turn step (~20 forms)
void Em306GurgBeast::fn_802F09D4(const atn::Vector3* target, float rate) {
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx r;
    Mtx m;
    float ry = mRot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &front, &front);
    float d = atn::WrapAngle_unk(target->y - rot.y);
    float dt = lbl_8087EFA8->mTimeScale;
    float turn = MIN(1.5707964f, MAX(-1.5707964f, d));
    float k = 0.1f * MAX(1.0f, 2.0f - m570);
    float t = 0.3f / MAX(1.0f, 0.6f * m570);
    m574.x -= m574.x * MIN(1.0f, k * dt);
    m574.z -= m574.z * MIN(1.0f, k * dt);
    turn *= t;
    rot.y += turn;
    atn::Vector3 vel;
    if (rate > 0.4f) {
        rate = (rate - 0.4f) / 0.6f;
        rate *= m154C != 0 ? 1.0f : 0.3f;
        float a = rot.y;
        vel.x = 0.0f;
        vel.y = 0.0f;
        vel.z = 0.4f * rate;
        PSMTXRotRad(r, 'y', a);
        PSMTXMultVec(r, &vel, &vel);
    } else {
        vel.x = 0.0f;
        vel.y = 0.0f;
        vel.z = 0.0f;
    }
    vel.x *= MIN(1.0f, dt);
    vel.y *= MIN(1.0f, dt);
    vel.z *= MIN(1.0f, dt);
    if ((mStatus.mParamB.m9C & 1U) != 1U) {
        m574.y += dt * (-98.1f / (float)(lbl_8087F0A8->mFps * lbl_8087F0A8->mFps));
    }
    vel += m574;
    if (vel.y < -6.0f) {
        vel.y = -6.0f;
    }
    bool turned = true;
    if (m152C != 0) {
        m152C->mColli.mFlags &= ~1;
    }
    atn::Vector3 start(pos);
    CollResult_unk result;
    Sphere_unk sphere = fn_80176548(pos);
    int hit = fn_8004D388(lbl_8087EE98, &result, &sphere.mCenter, &vel, fn_80179D44(), &mColli, 0, sphere.mRadius);
    pos = result.m10;
    pos -= m5A4;
    pos.y -= sphere.mRadius;
    if (hit && (result.m3C & 1) == 1) {
        vel.y = 0.0f;
    }
    m634 = result.m34 != 0 ? *result.m34 : -1;
    if (m152C != 0) {
        m152C->mColli.mFlags |= 1;
    }
    m610 = 0;
    if ((m54C & 2) == 2 && pos.y < 0.0f) {
        pos.y = 0.0f;
        vel.y = 0.0f;
    }
    m1560 = sqrt(vel.x * vel.x + vel.z * vel.z);
    if (m1560 >= 2.0f) {
        HitInfo_unk* skill = fn_80219E6C(m14EC);
        fn_802F0964(lbl_8087F048, this, skill, m1540, skill->m5C, -1, 0.0f);
    }
    mPos = pos;
    if (turned) {
        mRot = rot;
    }
    m574 = vel;
    m570 = sqrt(vel.x * vel.x + vel.z * vel.z);
    if (m153C != 0 && m570 < 0.4f) {
        m153C = 0;
    }
}

// inferred: names invented
inline float Max_unk(float a, float b) {
    return a > b ? a : b;
}

// inferred: walk motion of the beast: turning (0x144) when the movement is more than 45 degrees off the facing,
// running (0x140, speed from m1560) or walking (0x142)
void Em306GurgBeast::fn_802F0F60() {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    if (m1560 > 0.3f) {
        atn::Vector3 move(m574.x, 0.0f, m574.z);
        PSVECNormalize(&move, &move);
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &front, &front);
        float dot = PSVECDotProduct(&move, &front);
        if (dot < (float)cos(0.7853981852531433)) {
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, 0x140, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C =
                1.3f < Max_unk(0.5f, 0.5f * m1560) ? 1.3f : Max_unk(0.5f, 0.5f * m1560);
        }
    } else {
        mMotion.fn_80097C08(0, 0x142, 1, 0, 1, 0.0f, 0.1f);
    }
    if (--m1544 < 0) {
        m1544 = 30;
        m1540 = fn_800F8548(lbl_8087F048);
    }
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 GetSeatPos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 96.18% - block order of the state 17 test and r3/r4/r5 of the seat copy (~6 forms)
// inferred: keeps the rider m152C on the seat m1530 with the beast's motion frame (motion 0x1F0, 0x1F1 when
// m1560 >= 4)
void Em306GurgBeast::fn_802F1120() {
    if (m152C == 0 || m1530 == 0) {
        return;
    }
    if (m58C != 17) {
    int motion = 0x1F0;
    if (m154C != 0 && m1560 >= 4.0f) {
        motion = 0x1F1;
    }
    m152C->mMotion.m34C = 1;
    m152C->mMotion.mTracks[0].m20 = 1.0f;
    m152C->mMotion.mTracks[0].m0C = 0.0f;
    m152C->mMotion.mTracks[0].m08 = mMotion.mTracks[0].m08;
    m152C->mMotion.fn_80097C08(0, motion, 1, 0, 1, 0.0f, 0.1f);
    m152C->mF1C = m1530;
    m152C->mPos = GetSeatPos_unk((MtxPtr)m1530);
    m152C->mRot = mRot;
    m152C->fn_80145334();
    }
}

void Em306GurgBeast::fn_802F1240(CharaTask* rider) {
    rider->fn_8017AC3C((int)this);
    vf100(15);
    m1550 = -1;
    m1554 = 3;
    m14C0 = 0;
    m155C = 300;
    if (rider->mFlag31) {
        rider->fn_801539E0();
    }
    if (rider->mFlag6) {
        rider->fn_80155DAC(0);
    }
    if (rider->m139C != 0) {
        rider->fn_8017A33C(0, 1);
    }
    if (rider->m1208 != 0) {
        rider->fn_801750FC();
    }
    rider->fn_80164DCC(0);
}

// NONMATCHING: 97.72% - only the place of the load of the rider's rotation before PSMTXRotRad (~8 forms)
// inferred: throws the rider m152C off to the side; with `restart` the beast goes to state 14
void Em306GurgBeast::fn_802F1320(int restart) {
    if (m152C == 0) {
        return;
    }
    PlayerTask* head;
    if (lbl_8087F048 != 0 && lbl_8087F8A0 != 0 && (head = lbl_8087F8A0->mHead) != 0) {
        fn_80102824(lbl_8087F048, this, head);
    }
    if ((m152C->mStatus.m0C & 0x20) != 0x20) {
        m152C->fn_8016E970(1);
    }
    m152C->fn_801446F0();
    m152C->mF1C = 0;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx r;
    Mtx m;
    PSMTXRotRad(m, 'y', m152C->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    PSMTXRotRad(r, 'y', -1.5707964f);
    PSMTXMultVec(r, &dir, &dir);
    dir *= 5.0f;
    m152C->m574 = dir;
    m152C->fn_8017AC3C(0);
    m152C = 0;
    if (restart != 0) {
        vf100(14);
        m1550 = 300;
    }
}

void Em306GurgBeast::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em306 GurgBeast[%d]", m58);
    void* group;
    if (m151C.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m151C.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
}
