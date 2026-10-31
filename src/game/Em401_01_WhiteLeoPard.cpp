#include "game/Em401_01_WhiteLeoPard.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "atn/DrawHelper.h"

// inferred: event flags counted by fn_8031FE4C (lbl_80749898) and route places of the retreat
// (lbl_807498A4)
extern "C" const int lbl_80749898[3] = {0x1F, 0x20, 0x21};
extern "C" const int lbl_807498A4[3] = {200, 201, 202};

// inferred: node position (as in game/Em205Ogre.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

Em401_01_WhiteLeoPard::Em401_01_WhiteLeoPard(atn::Task* parent, const EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14C0(0.0f, 0.0f, 0.0f), m155C(0), m1560(0), m1564(0), m1568(9999), m156C(0),
      m1590(0), m1594(1), m1598(1000.0f), m159C(50), m15A0(50), m15A4(1000) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m14F4 = 0.0f;
    m14F8 = 0.0f;
    m14FC = 30.0f;
    m1500 = 0.0f;
    m1504 = 0.0f;
    m1508 = 120;
    m150C = 30;
    m1510 = 0;
    m1514 = 0;
    m1518 = 300.0f;
    m151C = 150.0f;
    m1520 = 120.0f;
    m1524 = 30.0f;
    m1528 = 50.0f;
    m152C = 10000.0f;
    m14A8 |= 0x80000000;
}

Em401_01_WhiteLeoPard::~Em401_01_WhiteLeoPard() {}

// inferred: field (lbl_8087F430); names invented (as in game/CharaTask.cpp)
struct Field_unk {
    unsigned char m000[0x868];
    int m868; // inferred: mode
    unsigned char m86C[0x8A0 - 0x86C];
    int m8A0;
    unsigned char m8A4[0x10D8 - 0x8A4];
    RouteTable_unk* m10D8; // inferred: route tables (as in game/Em403WarLion.cpp)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void fn_800EB49C(CharaTask* chara);     // inferred: BattleCharaTask (death?)
extern "C" int strcmp(const char* a, const char* b);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: node pose (as in game/Em403WarLion.cpp)
namespace atn {
struct MotionBuffer {
    unsigned int m00;
    Vector3 m04;
    Quaternion_unk m10;
    Vector3 m20;
};
} // namespace atn
extern "C" void fn_805F9AB0(atn::Quaternion_unk* q, const atn::Vector3* axis, float rad); // inferred: C_QUATRotAxisRad

// inferred: node callbacks of the model (as in game/Em403WarLion.cpp)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
extern "C" void fn_800F29D0(atn::Model* model, NodeCallback_unk callback);

// inferred: sets the second node pose callback of a model (also called by game/Em403WarLion.cpp)
extern "C" void fn_8031F2A4(atn::Model* model, boost::function<void(atn::MotionBuffer*, int)> callback) {
    model->m35C = callback;
}

int Em401_01_WhiteLeoPard::vf0C() {
    int result = 1;
    if (fn_8013655C() != 0) {
        result = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        result = 0;
    }
    if (result != 0) {
        mStatus.m18 |= 0x140;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        // inferred: positions of the root node and the left hand, computed and not used
        int hand = mMotion.GetNodeNoByName("lefthand", 0);
        if (hand >= 0) {
            atn::Vector3 a = GetNodePos_unk(mMotion.GetNodeMtx_unk(m524));
            atn::Vector3 b = GetNodePos_unk(mMotion.GetNodeMtx_unk(hand));
        }
        m56C = 0.1f;
        m500 = 0.4f;
        m508 = 1.0f;
        fn_800F29D0(&mMotion, boost::bind(&Em401_01_WhiteLeoPard::fn_80321900, this, _1, _2));
        mMotion.fn_80091CFC("neck");
        mMotion.fn_80091CFC("waist");
        fn_8031F2A4(&mMotion, boost::bind(&Em401_01_WhiteLeoPard::fn_80321724, this, _1, _2));
        atn::Vector3 limit(1.5707964f, 2.0943952f, 0.0f);
        mLook.mLimit = limit;
        return 1;
    }
    return 0;
}

// inferred: mFlag28 of the leader (accessor; name invented)
inline int LeaderFlag_unk() {
    return lbl_8087F8A0->mHead->mFlag28;
}

// inferred: leans the waist with m14C0.y and turns towards the target (update)
void Em401_01_WhiteLeoPard::vf10() {
    m1560++;
    if ((mStatus.m0C & 0x20) != 0x20) {
        fn_8031FAA4();
    } else {
        lbl_8087F430->m8A0 = 0;
    }
    if (lbl_8087F8A0->mHead->mFlag28 && m156C == 0) {
        fn_80370AE4(lbl_8087F430, 0x3C, 1);
    }
    atn::Vector3 pos(mPos);
    m156C = LeaderFlag_unk();
    atn::Vector3 rot(mRot);
    fn_8014C540();
    if ((mStatus.m0C & 0x20) == 0x20 && m58C != 2) {
        vf44();
    }
    fn_8031FE4C();
    m14C0.y *= 0.8f;
    float d = rot.y + m14C0.y - mRot.y;
    float a = atn::Abs_unk(d);
    if (a > 0.7853982f) {
        d = 0.7853982f * (d / a);
    }
    m14C0.y = d;
    if (mC58.mC4 != 0) {
        mFlag24 = 0;
        mLook.fn_80129978(&mC58.mC4->mMotion, "neck", lbl_807C7030, 0.3f);
    } else {
        mFlag24 = 1;
    }
    fn_80145334();
    if (lbl_8087F430->m868 == 6) {
        mRoute.fn_80127D8C(0, 0, 0);
    }
}

// inferred: material with three colors (diffuse, ambient, specular) returned by fn_80094B6C; name invented
struct LeoMaterial_unk {
    unsigned char m00[0x18];
    GXColor m18;
    GXColor m1C;
    GXColor m20;
};
extern "C" LeoMaterial_unk* fn_80094B6C(atn::Model* model, const char* name); // inferred: material by name
// inferred: color as four floats (as in game/Em315Coffin.cpp)
struct LeoColor_unk {
    LeoColor_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    float r, g, b, a;
};
extern "C" double sin(double x);
// inferred: settings (lbl_8087F0A8) and debug drawing (lbl_8087EEB0), as in game/Em403WarLion.cpp
struct LeoSettings_unk {
    unsigned char m00[0x30];
    int m30;        // inferred: frames per second (as in game/CharaTask.cpp)
    int mDrawDebug; // 0x34
};
extern "C" LeoSettings_unk* lbl_8087F0A8;

// inferred: pulses the alpha of the "em405_efc" material every 120 frames; draws the view fan in debug
void Em401_01_WhiteLeoPard::vf14() {
    fn_80149A30();
    m155C = (m155C + 1) % 120;
    float t = (float)m155C / 120.0f;
    LeoMaterial_unk* material = fn_80094B6C(&mMotion, "em405_efc");
    LeoColor_unk diffuse(material->m18);
    float s = (float)sin(6.2831855f * t);
    diffuse.a = 0.5f * (1.0f + s);
    mMotion.fn_80094958("em405_efc", &diffuse, &LeoColor_unk(material->m20), &LeoColor_unk(material->m1C));
    if (lbl_8087F0A8->mDrawDebug != 0) {
        lbl_8087EEB0->fn_80063484(20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

// inferred: picks the target: the leader when it has mFlag28, else (field flag 0x3D set) a player held by
// a control 0x1D-0x1F
void Em401_01_WhiteLeoPard::fn_8031FAA4() {
    if (fn_80370A78(lbl_8087F430, 0x3D) == 0) {
        if (lbl_8087F8A0->mHead->mFlag28) {
            mC58.mC8 = (int)mC58.mC4;
            m1454 = 0;
            mC58.mC4 = lbl_8087F8A0->mHead;
        } else {
            m1454 = 1;
        }
    } else {
        CharaTask* held = 0;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (p->mStatus.fn_80134270() && p->m55C == 6 && p->m560 - 0x1DU <= 2) {
                held = p;
            }
        }
        if (lbl_8087F8A0->mHead->mFlag28) {
            mC58.mC8 = (int)mC58.mC4;
            m1454 = 0;
            mC58.mC4 = lbl_8087F8A0->mHead;
        } else if (held != 0) {
            mC58.mC8 = (int)mC58.mC4;
            m1454 = 0;
            mC58.mC4 = held;
        } else {
            m1454 = 1;
        }
    }
    lbl_8087F430->m8A0 = 0;
    if (mC58.mC4 != (CharaTask*)mC58.mC8) {
        mRoute.fn_8012539C();
    }
}

void Em401_01_WhiteLeoPard::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m1594 != 0) {
        if (msg->m44 == 1) {
            msg->m10 *= 2.0f;
        } else {
            if (m58C == 7) {
                msg->m68 = 0;
                msg->m90 = 1;
                return;
            }
            if (m58C == 1 && fn_80370A78(lbl_8087F430, 0x3D) == 1) {
                msg->m10 *= 0.0f;
            } else {
                msg->m10 *= 0.7f;
                msg->m50 |= 0x90;
            }
        }
        fn_80370A78(lbl_8087F430, 0x3D);
    }
    fn_80151448(msg);
    fn_8031FD34(msg);
}

// inferred: counts the damage and knocks the leopard back (reaction to a hit)
void Em401_01_WhiteLeoPard::fn_8031FD34(HitMsg_unk* msg) {
    // empty release asserts (as in MP/CryptMP): keep the function above the autoinline threshold
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    m1560 += m15A0 / 8;
    if (m58C == 6 && m55C == 6 && m560 - 22U <= 1) {
        m58C = 0;
    }
    if (m55C == 6 && m560 == 23) {
        fn_8015E4B0(m6B8, -1);
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (!(m55C == 6 && (m560 == 23 || m560 == 0x3B))) {
            fn_8015E7A0(msg->m28 * 5.0f, -1, 0);
        }
        fn_800EB49C(this);
    }
}

// inferred: collision result and sweep test (as in game/Em003LesserDaemon.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004D388(void* world, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" int fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                           int c, float f); // inferred: deals a hit, returns the damage dealt

extern "C" int rand();
extern "C" double sqrt(double x);
extern "C" double cos(double x);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
// inferred: game state (lbl_8087EFA8, see game/CharaTask.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8;
// inferred: camera (lbl_8087EFB4, see game/CharaTask.cpp); names invented
struct Camera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mEye; // 0x10C
};
extern "C" Camera_unk* lbl_8087EFB4;

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

// inferred: name invented (as in game/CharaTask.cpp)
inline float LengthXZ_unk(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}


// inferred: turns towards `target` (name invented)
inline void Em401_01_WhiteLeoPard::LookAt_unk(CharaTask* target) {
    atn::Vector3 d = target->mPos - mPos;
    mRot.y = d.ToRotationZ_unk().y;
}

// NONMATCHING: 98.63% - li r4, 0 / lwz r10, m594 of the fn_800F8C6C call scheduled the other way round
// (~6 variants: local copy, unsigned argument, reordered hit)
// inferred: state machine (m58C): 3/4 move, 6 aims the jump, 7 jumps, 1 attacks, 2 dies
void Em401_01_WhiteLeoPard::fn_8031FE4C() {
    if (m55C == 2) {
        m55C = 4;
    }
    switch (m58C) {
    case 6:
        if (--m1570 < 0) {
            fn_803227A0();
        }
        if (m1574 != 0) {
            LookAt_unk(m1574);
        }
        break;
    case 7:
        fn_80322AD4();
        break;
    case 0:
        fn_803202E0();
        break;
    case 1:
        if (fn_80158BB4()) {
            m58C = 0;
            if (m560 != 0x16 && m560 != 0x17 && m560 != 0x47 && m55C != 7) {
                fn_8016EB48(1, 1, 1);
                if (m55C == 2) {
                    m55C = 4;
                }
            }
        }
        fn_803202E0();
        if (lbl_8087F048 != 0 && fn_80158CA4() && m594 != 0) {
            fn_80144710();
            HitInfo_unk* hit = fn_80219E6C(0x687);
            if (fn_80370A78(lbl_8087F430, 0x3D) == 0) {
                int left = 3;
                for (int i = 0; i < 3; i++) {
                    if (fn_80370A78(lbl_8087F430, lbl_80749898[i]) >= 200) {
                        left--;
                    }
                }
                switch (left) {
                case 3:
                    hit = fn_80219E6C(0x687);
                    break;
                case 2:
                    hit = fn_80219E6C(0x688);
                    break;
                default:
                    hit = fn_80219E6C(0x689);
                    break;
                }
            }
            int damage = fn_800F8C6C(lbl_8087F048, 0, 0, this, hit, m590, 30, m594, 0.0f);
            if (m594 > 0) {
                m594 -= damage;
            }
        }
        break;
    case 2:
        if (!fn_8015EB2C()) {
            fn_803202E0();
        } else {
            mColli.mFlags &= ~1;
            mFlag25 = 1;
            fn_800EAECC();
        }
        break;
    case 3:
    case 4:
        fn_803202E0();
        break;
    }
}

// NONMATCHING: 99.79% - r3/r4/r5 of the inlined FindPlace loops and of the copies of the first vectors
// (~8 variants of the loop and of the declaration order)
// inferred: moves towards the target (state 4), along the route (state 7) or runs the control (state 6);
// near the target it attacks (fn_800EB270), far from it it starts the jump (state 6)
void Em401_01_WhiteLeoPard::fn_803202E0() {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    float speed = 0.0f;
    float rate = 1.0f;
    float dist = 1e16f;
    atn::Vector3 rot(mRot);
    atn::Vector3 d(lbl_807C7030);
    if (mC58.mC4 != 0) {
        d = mC58.mC4->mPos - mPos;
        d.y = 0.0f;
        dist = sqrt(d.x * d.x + d.z * d.z);
    }
    int state = m55C;
    if (state == 4) {
        if (FindRoutePlace2_unk(table, m1568) == 0) {
            mRoute.fn_8012539C();
            fn_80170A20(mC58.mC4, 0, -1.0f);
        } else if (fn_80370A78(lbl_8087F430, 0x3D) != 0) {
            mRoute.fn_8012539C();
            fn_8017039C(m1568, 0);
        }
    } else if (state == 7) {
        if (mStatus.IsState_unk(0x20)) {
            rot = mRot;
            speed = 0.0f;
        } else {
            mRoute.fn_80126214();
            atn::Vector3 dir(mRoute.m58);
            speed = dir.Length_unk();
            if (speed > 0.0001f) {
                rot = dir.GetNormalize().ToRotation_unk();
            } else {
                rot = mRot;
            }
        }
        if (fn_80370A78(lbl_8087F430, 0x3D) == 0) {
            rate *= 0.7f;
            if (dist < m1524 && m1560 > m15A0) {
                fn_800EB270(200);
                m1560 = 0;
                m1564++;
                return;
            }
        } else if (m1564 >= m15A4) {
            RoutePlace_unk* place = FindRoutePlace2_unk(table, m1568);
            if (place == 0) {
                m1568 = lbl_807498A4[rand() % 3];
                mRoute.fn_8012539C();
                fn_8017039C(m1568, 0);
                return;
            }
            if (LengthXZ_unk(place->mPos - mPos) < m14FC) {
                mRoute.fn_8012539C();
                fn_80170A20(mC58.mC4, 0, -1.0f);
                m1568 = 9999;
                m1564 = 0;
                m1560 = 0;
            }
        } else if (m1560 > m15A0) {
            if (dist < m1524) {
                fn_800EB270(200);
                m1560 = 0;
                m1564++;
                return;
            }
            if (dist > m1528 && m1560 > m15A0 * 2) {
                CharaTask* target = mC58.mC4;
                if (target != 0) {
                    m58C = 6;
                    fn_8016E970(6);
                    m560 = 4;
                    m590 = fn_800F8548(lbl_8087F048);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x13F, 1, 0, 1, 0.0f, 0.1f);
                    m1570 = 30;
                    m1574 = target;
                    m1560 = 0;
                    m1564++;
                    return;
                }
            }
        }
    } else if (state == 6) {
        fn_8013A258();
        return;
    }
    vf34(rot, 0, speed, rate);
}

void Em401_01_WhiteLeoPard::vf34(const atn::Vector3& rot, int arg, float a, float b) {
    fn_803209E0(rot, 0, a, m568 * b);
}

// NONMATCHING: 98.32% - same as Em403WarLion::fn_80324A38 (FPR of the turn and of the clamp of m584, stores
// of move.x/z around sin, order of m574 at the end); ~6 variants
// inferred: moves the leopard towards the rotation `target` (as Em403WarLion::fn_80324A38, plus the push
// forward of the first 24 frames of the attack)
void Em401_01_WhiteLeoPard::fn_803209E0(const atn::Vector3& target, int arg, float speed, float rate) {
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    float diff = atn::WrapAngle_unk(target.y - rot.y);
    float dt = lbl_8087EFA8->m3A4;
    float cur = rot.y;
    float turn = m56C;
    rate *= dt;
    float k = 0.75f * atn::Abs_unk(diff / 3.1415927f);
    turn *= 1.0f + k;
    float step = diff * turn;
    cur += step;
    rot.y = cur;
    m570 *= 0.85f;
    if (m570 < 0.001f) {
        m570 = 0.0f;
    }
    if (speed > 1.0f) {
        speed = 1.0f;
    }
    if (speed > 0.4f) {
        speed = (speed - 0.4f) / 0.6f;
        float ofs = 0.25f;
        speed *= rate;
        if (m570 < 0.5f) {
            ofs = 0.0f;
        }
        float angle = atn::Abs_unk(diff / 3.1415927f) - ofs;
        if (angle < 0.0f) {
            angle = 0.0f;
        }
        speed *= 1.0f - 1.5f * (angle / (1.0f - ofs));
        if (speed < 0.0f) {
            speed = 0.0f;
        }
    } else {
        speed = 0.0f;
    }
    m580 += 0.25f * (0.7f * diff) - 0.4f * m580;
    if (atn::Abs_unk(m580) < 0.001f) {
        m580 = 0.0f;
    }
    m570 += 0.26f * speed - 0.125f * m570;
    vf24();
    m574.x *= 0.3f;
    m574.z *= 0.3f;
    atn::Vector3 move;
    move.x = m570 * (float)sin(rot.y);
    move.y = 0.0f;
    move.z = m570 * (float)cos(rot.y);
    move.x *= dt;
    move.y *= dt;
    move.z *= dt;
    if (m570 > 1.0f) {
        move.x *= 2.0f;
        move.y *= 2.0f;
        move.z *= 2.0f;
    }
    if ((mStatus.mParamB.m9C & 1) != 1) {
        int fps = lbl_8087F0A8->m30;
        m574.y += dt * (-98.1f / (float)(fps * fps));
    }
    move += m6B8;
    m6B8.x *= 0.8f;
    m6B8.y = 0.0f;
    m6B8.z *= 0.8f;
    if (m6B8.Length_unk() < 0.1f) {
        m6B8.Set_unk(0.0f, 0.0f, 0.0f);
    }
    move += m574;
    if (move.y < -6.0f) {
        move.y = -6.0f;
    }
    if (m58C == 1 && mMotion.mTracks[0].m08 <= 24.0f) {
        atn::Vector3 push(0.0f, 0.0f, 0.8333333f);
        Mtx r;
        PSMTXRotRad(r, 'y', mRot.y);
        PSMTXMultVec(r, &push, &push);
        move += push;
    }
    int keepRot = 1;
    atn::Vector3 oldPos(pos);
    CollResult_unk result;
    Sphere_unk sphere = fn_80176548(pos);
    unsigned int mask = arg != 0 ? 0x80000000 : 0;
    if (mType == 0) {
        mask |= 0x20;
    } else if (mType == 3) {
        mask |= 0x40;
    } else if (mType == 2) {
        mask |= 0x80;
    }
    int hit = fn_8004D388(lbl_8087EE98, &result, &sphere, &move, mask, &mColli, 0, sphere.mRadius);
    pos = result.m10;
    pos -= m5A4;
    pos.y -= sphere.mRadius;
    if (hit != 0 && oldPos.y - pos.y < 0.95f * -move.y) {
        move.y = 0.0f;
    }
    bool landed = false;
    if (result.m34 != 0 && *(unsigned int*)result.m34 == 0x1A) {
        landed = true;
    }
    mFlag3 = landed;
    float lean = 0.0f;
    atn::Vector3 vel(move + (pos - oldPos));
    if (PSVECSquareMag(&vel) > 0.001f && speed > 0.001f) {
        lean = atn::WrapAngle_unk(vel.GetNormalize().ToRotation_unk().y - mRot.y);
    }
    lean *= 2.0f;
    m584 = MAX(MIN(m584 - 0.5f * lean, 1.0471976f), -1.0471976f) * 0.9f;
    if ((m54C & 2) == 2 && pos.y < 0.0f) {
        pos.y = 0.0f;
    }
    mPos = pos;
    if (keepRot) {
        mRot = rot;
    }
    m574 = move;
    m574.z /= dt;
    m574.x /= dt;
}

extern "C" float fn_800A08D4(atn::LoadHandle* motion); // inferred: length of the motion data
// inferred: length of a motion, 30 frames when it is missing (as in game/Em403WarLion.cpp); name invented
inline float GetMotionLength_unk(atn::Model* model, int id) {
    atn::LoadHandle* motion = model->fn_80097D40(id);
    float length = motion != 0 ? fn_800A08D4(motion) : 30.0f;
    if (length == 0.0f) {
        length = 1.0f;
    }
    return length;
}
inline float Clamp01_unk(float x) {
    if (x < 0.0f) {
        x = 0.0f;
    } else if (x > 1.0f) {
        x = 1.0f;
    }
    return x;
}

// NONMATCHING: 99.94% - dt in f29 instead of f28 (only the register); ~10 variants
// inferred: chooses the movement motions (same code as Em403WarLion::vf24; m500/m508 read before the rate
// give the comparison registers of the original)
void Em401_01_WhiteLeoPard::vf24() {
    if (mFlag6) {
        return;
    }
    if (m55C == 6 && mControl && mControl->vf3C() == 0) {
        return;
    }
    if (m55C == 9 || mFlag2) {
        return;
    }
    if (m55C == 2 && mC58.mMode >= 20 && mC58.mMode <= 22 && !mC58.mDone_unk) {
        return;
    }
    float dt = lbl_8087EFA8->m3A4;
    GetMotionLength_unk(&mMotion, m484[1]);
    GetMotionLength_unk(&mMotion, m484[3]);
    float walkMin = m500;
    float dashMin = m508;
    float rate = m570 / dt;
    float blend;
    float walkSpeed;
    float dashSpeed;
    if (rate < walkMin) {
        blend = rate / walkMin;
        walkSpeed = 1.0f;
        dashSpeed = 0.0f;
    } else if (rate < dashMin) {
        blend = 1.0f;
        dashSpeed = 0.0f;
        walkSpeed = blend + (rate - walkMin) / (dashMin - walkMin);
    } else {
        blend = 2.0f;
        walkSpeed = 0.0f;
        dashSpeed = 1.0f + 0.5f * (rate / dashMin - 1.0f);
    }
    if (PSVECSquareMag(&(lbl_8087EFB4->mEye - mPos)) > 62500.0f) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        if (blend < 0.5f) {
            mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        } else if (blend < 1.5f) {
            mMotion.fn_80097C08(0, m484[1], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = walkSpeed;
        } else {
            mMotion.fn_80097C08(0, m484[3], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = dashSpeed;
        }
        return;
    }
    mMotion.m34C = 3;
    float weight = Clamp01_unk(1.0f - blend);
    mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m20 = weight;
    mMotion.mTracks[0].m0C = 1.0f;
    weight = Clamp01_unk(1.0f - atn::Abs_unk(1.0f - blend));
    mMotion.fn_80097C08(1, m484[1], 1, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[1].m0C = walkSpeed;
    mMotion.mTracks[1].m20 = weight;
    weight = Clamp01_unk(blend - 1.0f);
    mMotion.fn_80097C08(2, m484[3], 1, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[2].m0C = dashSpeed;
    mMotion.mTracks[2].m20 = weight;
}

// inferred: rotation around an axis (name invented, as in game/Em403WarLion.cpp)
inline atn::Quaternion_unk QuatAxis_unk(const atn::Vector3& axis, float rad) {
    atn::Quaternion_unk q;
    fn_805F9AB0(&q, &axis, rad);
    return q;
}

// inferred: turns the neck and the spine with the lean (node pose callback; as in Em403WarLion)
// NONMATCHING: 97.83% - same as Em403WarLion::fn_803256F8 (one more saved GPR and order of the quaternions)
void Em401_01_WhiteLeoPard::fn_80321724(atn::MotionBuffer* nodes, int count) {
    for (int i = 0; i < count; i++) {
        const char* name = ((ModelNodeDesc_unk*)mMotion.m16C->m48[i])->mName;
        if (strcmp(name, "neck") == 0) {
            if (m648 == 0) {
                atn::Quaternion_unk rot(nodes[i].m10);
                atn::Quaternion_unk q = QuatAxis_unk(atn::Vector3(1.0f, 0.0f, 0.0f), m580 - 0.7f * m584);
                PSQUATMultiply(&q, &rot, &rot);
                nodes[i].m10 = rot;
            }
        } else if (strcmp(name, "spine") == 0) {
            if (m648 == 0 && !mFlag31 && !mFlag6) {
                atn::Quaternion_unk rot(nodes[i].m10);
                atn::Quaternion_unk q = QuatAxis_unk(atn::Vector3(1.0f, 0.0f, 0.0f), 0.9f * m580 + m584);
                PSQUATMultiply(&q, &rot, &rot);
                nodes[i].m10 = rot;
            }
        }
    }
}

// inferred: m = m * rotation(r) z, y, x (as in game/Em403WarLion.cpp; name invented)
inline atn::Matrix3x4& MakeRotationZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    return m;
}

// inferred: turns the neck with the lean and the waist with m14C0.y (node matrix callback; as in Em403WarLion)
// NONMATCHING: 98.79% - same as Em403WarLion::fn_803258D4 (r30/r31 of the node, stack of the matrices)
void Em401_01_WhiteLeoPard::fn_80321900(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk) {
    ModelNodeDesc_unk* node = (ModelNodeDesc_unk*)&chunk;
    if (mLook.fn_8012A288(mtx, node) != 0) {
        return;
    }
    const char* name = node->mName;
    if (strcmp(name, "neck") == 0) {
        if (m648 == 0) {
            atn::Vector3 pos = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            Mtx r;
            PSMTXRotRad(r, 'y', m580 - 0.7f * m584);
            PSMTXConcat(r, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    } else if (strcmp(name, "waist") == 0) {
        if (m55C != 8) {
            atn::Vector3 pos = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            atn::Vector3 rot(0.0f, m14C0.y, 0.1f * m14C0.y);
            atn::Matrix3x4 r;
            MakeRotationZYX_unk(r, rot);
            PSMTXConcat(r.m, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    }
}

// NONMATCHING: 91.4% - the original reloads the flag word 0x12A4 for mFlag31, schedules the sums of m614
// differently and swaps r3/r4 in the Vector3 copies; ~14 variants
// inferred: places the collision capsule along the body (own version of CharaTask::vf20; as Em403WarLion)
void Em401_01_WhiteLeoPard::vf20() {
    m620 = m5B0;
    if (mFlag13) {
        if (m648 != 0) {
            if (!mFlag31) {
                m620 += 2.0f;
            }
        } else if (m570 > 2.0f * m500 && m508 > 2.005f * m500) {
            m620 = 2.0f * ((m570 - 2.0f * m500) / (m508 - 2.0f * m500)) + m5B0;
        }
    }
    float r = m620 * 1.5f;
    m614 = mPos + m5A4;
    m620 = r;
    m614.y += r;
    atn::Vector3 a(mPos.x, mPos.y + r, mPos.z);
    atn::Vector3 b;
    int tail = mMotion.GetNodeNoByName("tale", 0);
    if (tail > -1) {
        b = GetNodePos_unk(mMotion.mNodeMtx[tail]);
        b.y -= 1.0f;
    } else {
        b = a;
        b.y += m5B4 - 2.0f * m620;
    }
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m620;
    m148C = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    m1498 = 3.0f;
}

// inferred: two collision spheres, the body and the tail (as Em403WarLion::vf50)
void Em401_01_WhiteLeoPard::vf50() {
    Item14_unk item;
    item.m04.mRadius = 5.0f;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += 5.0f;
    m624.push_back(item);
    item.m04.mRadius = 5.0f;
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("tale"));
    m624.push_back(item);
    mFlagB29 = 0;
}

// NONMATCHING: 99.64% - r6/r7 of the copy of the first center (__VecCopy family); ~3 variants
// inferred: places the two damage spheres on the body and on the "tale" node (update of m624)
void Em401_01_WhiteLeoPard::vf54() {
    if (m624.mSize != 0) {
        m624.mData[0].m04.mRadius = 5.0f;
        m624.mData[0].m04.mCenter = mPos + m5A4;
        m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
        m624.mData[1].m04.mRadius = 5.0f;
        m624.mData[1].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("tale"));
        m624.mData[1].m04.mCenter.y -= 5.0f;
    }
}

inline float LeoMax_unk(float a, float b) {
    return a > b ? a : b;
}

// NONMATCHING: 97.9% - the original stores m1588 = 1.5f after the fabs of z in the inlined ToRotation_unk
// (scheduling window) and the offsets of .sdata2; ~11 variants
// inferred: starts the jump towards m1574 (or straight ahead), 20 frames
void Em401_01_WhiteLeoPard::fn_803227A0() {
    m58C = 7;
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x21, 1, 0, 1, 0.0f, 0.1f);
    if (m1574 != 0) {
        atn::Vector3 d = m1574->mPos - mPos;
        m157C = d;
    } else {
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        Mtx r;
        PSMTXRotRad(r, 'y', mRot.y);
        PSMTXMultVec(r, &dir, &dir);
        m157C = dir;
    }
    m157C.y = 0.0f;
    m1578 = PSVECMag(&m157C);
    PSVECNormalize(&m157C, &m157C);
    m1588 = 1.5f;
    mRot.y = m157C.ToRotation_unk().y;
    m1570 = 20;
}

// inferred: jump: moves along m157C and falls; deals the hit 0x686 while above 3
void Em401_01_WhiteLeoPard::fn_80322AD4() {
    if (--m1570 < 0) {
        m58C = 0;
        if (m55C == 6 && m560 == 4) {
            fn_8016E970(4);
        }
        m1560 = 0;
        return;
    }
    m1588 += 0.3f;
    m1588 = LeoMax_unk(m1588, 6.0f);
    atn::Vector3 move(m157C * (float)m1588);
    CollResult_unk result;
    Sphere_unk sphere = fn_80176548(mPos);
    fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000080, &mColli, 0, sphere.mRadius);
    mPos = result.m10;
    mPos -= m5A4;
    mPos.y -= sphere.mRadius;
    if (m1588 >= 3.0f) {
        fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x686), m590, 30, -1, 0.0f);
    }
}

extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
// inferred: debug menu of the enemy; removed by the linker, only its strings are left in the pool (after
// "tale"), as Em403WarLion::fn_DebugMenu_unk. The calls and the ranges are guesses.
void Em401_01_WhiteLeoPard::fn_DebugMenu_unk() {
    void* group = m158C.Load("em401_01_WhiteLeoPard");
    fn_80087994(group, "DebugDisp", &m1590, 0, 0);
    fn_80087994(group, "DamageIgnore", &m1594, 0, 0);
    fn_8008771C(group, "ReturnEndLength", &m1598, 0, 0, 0.0f, 10000.0f, 1.0f);
    fn_800874C8(group, "UpPhysicalAttack", &m159C, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "interval", &m15A0, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "AttackTime", &m15A4, 0, 9999, 1, 0, 0);
}
