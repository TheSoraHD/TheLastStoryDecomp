#include "game/QuadrupedTask.h"
#include "chr/ControlDropDown.h"
#include "chr/ControlMountle.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "atn/DrawHelper.h"

// inferred: node position (as in game/Em205Ogre.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}
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

extern "C" void fn_8031F2A4(atn::Model* model, boost::function<void(atn::MotionBuffer*, int)> callback); // inferred: sets the node pose callback (game/Em401_01_WhiteLeoPard.cpp)
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
extern "C" double sin(double x);
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




// inferred: settings (lbl_8087F0A8) as in game/Em403WarLion.cpp
struct QuadSettings_unk {
    unsigned char m00[0x30];
    int m30;        // inferred: frames per second (as in game/CharaTask.cpp)
    int mDrawDebug; // 0x34
};
extern "C" QuadSettings_unk* lbl_8087F0A8;

QuadrupedTask::QuadrupedTask(atn::Task* parent, void* data)
    : EnemyTask(parent, data), m14B0(0.0f, 0.0f, 0.0f), m1564(0) {
    m14F8 = 0.0f;
    m14FC = 0.0f;
    m1500 = 70.0f;
    m1504 = 0.0f;
    m1508 = 0.0f;
    m150C = 45;
    m1510 = 15;
    m1514 = 0;
    m1518 = 0;
    m151C = 300.0f;
    m1520 = 150.0f;
    m1524 = 30.0f;
    m1528 = 70.0f;
    m152C = 10000.0f;
    m1530 = 10000.0f;
    m14A8 |= 0x80000000;
}

QuadrupedTask::~QuadrupedTask() {}

// NONMATCHING: 99.33% - only the .sdata2/@stringBase0 labels and the guard of the boost stored_vtable differ
int QuadrupedTask::vf0C() {
    int result = 1;
    if (fn_8013655C() != 0) {
        result = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        result = 0;
    }
    if (result != 0) {
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        // inferred: height of the left hand over the root node
        int hand = mMotion.GetNodeNoByName("lefthand", 0);
        if (hand >= 0) {
            atn::Vector3 a = GetNodePos_unk(mMotion.GetNodeMtx_unk(m524));
            atn::Vector3 b = GetNodePos_unk(mMotion.GetNodeMtx_unk(hand));
            m14CC = b.y - a.y;
        }
        m56C = 0.1f;
        m500 = 0.4f;
        m508 = 1.0f;
        fn_800F29D0(&mMotion, boost::bind(&QuadrupedTask::fn_805688CC, this, _1, _2));
        mMotion.fn_80091CFC("neck");
        mMotion.fn_80091CFC("waist");
        fn_8031F2A4(&mMotion, boost::bind(&QuadrupedTask::fn_805686F0, this, _1, _2));
        atn::Vector3 limit(1.5707964f, 3.1415927f, 0.0f);
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC015;
        mStatus.m18 |= 0x3000000;
        mLook.mLimit = limit;
        return 1;
    }
    return 0;
}

// NONMATCHING: 94.86% - register allocation of the node lookups (ported from Em401_01_WhiteLeoPard)
void QuadrupedTask::vf10() {
    if (m55C == 2) {
        m55C = 4;
    }
    atn::Vector3 rot(mRot);
    fn_8014C540();
    if ((mStatus.m0C & 0x20) == 0x20 && m58C != 2) {
        vf44();
    }
    fn_80566FB4();
    m14B0.y *= 0.8f;
    float d = rot.y + m14B0.y - mRot.y;
    float a = atn::Abs_unk(d);
    if (a > 0.7853982f) {
        d = 0.7853982f * (d / a);
    }
    m14B0.y = d;
    if (mFC0 != 0) {
        mFlag24 = 0;
        mLook.fn_80129978(&mFC0->mMotion, "neck", lbl_807C7030, 0.3f);
    } else {
        mFlag24 = 1;
    }
    if ((mFlags & 1) != 1 && !mFlag25) {
        fn_80145334();
        return;
    }
    atn::Vector3 d2 = lbl_8087EFB4->mEye - mPos;
    fn_80148B38(PSVECSquareMag(&d2));
}


extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: debug menu of the enemy; removed by the linker, only its strings are left in the pool
// ("Quadruped", "DebugDisp" between "waist" and "spine"), as Em401_01_WhiteLeoPard::fn_DebugMenu_unk.
void QuadrupedTask::fn_DebugMenu_unk() {
    void* group = m1560.Load("Quadruped");
    fn_80087994(group, "DebugDisp", &m1564, 0, 0);
}

void QuadrupedTask::vf14() {
    fn_80149A30();
    if (lbl_8087F0A8->mDrawDebug != 0) {
        lbl_8087EEB0->fn_80063484(20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

void QuadrupedTask::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    fn_80151448(msg);
    m638 = 0;
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (!(m55C == 6 && (m560 == 0x17 || m560 == 0x3B))) {
            fn_8015E7A0(msg->m28 * 5.0f, -1, 0);
        }
        fn_800EB49C();
    }
}

// inferred: the character got stuck: tries to climb (vf74) when it moves but its position does not change
inline int QuadrupedTask_IsStuck_unk(CharaTask* self, const atn::Vector3& prev) {
    atn::Vector3 d = prev - self->mPos;
    return self->m570 > 0.1f && atn::Abs_unk(d.x) < 0.1f && atn::Abs_unk(d.y) < 0.1f && atn::Abs_unk(d.z) < 0.1f;
}

// inferred: state machine (m58C)
// NONMATCHING: 86.11% - scheduling of the hit reactions and order of the Vector3 temporaries
void QuadrupedTask::fn_80566FB4() {
    switch (m58C) {
    case 0:
        if (m55C == 6) {
            switch (m560) {
            case 2:
            case 3:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x16:
            case 0x17:
            case 0x27:
            case 0x34:
            case 0x35:
            case 0x41:
            case 0x47:
                mC58.m2B0 = 0;
                fn_805675FC();
                return;
            }
        }
        if (m55C == 7) {
            atn::Vector3 prev(mPos);
            fn_805675FC();
            int stuck = 1;
            if (QuadrupedTask_IsStuck_unk(this, prev)) {
                if (++mC58.m2B0 > 3) {
                    if (fn_8016F67C()) {
                        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                        Mtx m;
                        PSMTXRotRad(m, 'y', mRot.y);
                        PSMTXMultVec(m, &dir, &dir);
                        vf74(&dir);
                    }
                } else {
                    stuck = 0;
                }
            }
            if (stuck != 0) {
                mC58.m2B0 = 0;
            }
        } else {
            if (mC58.mMode == 4 && mC58.mStep == 2) {
                fn_800EB270(200);
                return;
            }
            atn::Vector3 prev(mPos);
            fn_805675FC();
            if (QuadrupedTask_IsStuck_unk(this, prev)) {
                if (++mC58.m2B0 > 3 && fn_8016F67C()) {
                    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                    Mtx m;
                    PSMTXRotRad(m, 'y', mRot.y);
                    PSMTXMultVec(m, &dir, &dir);
                    vf74(&dir);
                    mC58.m2B0 = 0;
                }
            } else {
                mC58.m2B0 = 0;
            }
        }
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
        fn_805675FC();
        if (lbl_8087F048 != 0 && fn_80158CA4() && m594 != 0) {
            fn_80144710();
            int damage = fn_800F8C6C(lbl_8087F048, 0, 0, this, m638, m590, 30, m594, 0.0f);
            if (m594 > 0) {
                m594 -= damage;
            }
        }
        break;
    case 2:
        if (!fn_8015EB2C()) {
            fn_805675FC();
        } else {
            mColli.mFlags &= ~1;
            mFlag25 = 1;
            fn_800EAECC();
        }
        break;
    case 3:
    case 4:
        fn_805675FC();
        break;
    }
}

extern "C" int fn_803C1560(RouteTable_unk* table, const atn::Vector3* pos, int arg2, int arg3, int arg4, int arg5,
                           float arg6);

// inferred: chooses the move of the AI state (mC58) from the distance to its target
// NONMATCHING: 88.00% - order of the distance checks and of the stack temporaries
void QuadrupedTask::fn_80567424(CharaTask* target) {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    if (table == 0) {
        return;
    }
    CharaTask* other = (CharaTask*)target->mC58.mC4;
    short mode = target->mC58.mMode;
    if (other != 0) {
        atn::Vector3 d = target->mPos - other->mPos;
        d.Length_unk();
    }
    if (target->mC58.m00 == 0) {
        target->mC58.m0C.mDestPoint = fn_803C1560(table, &target->mPos, target->fn_80179D44(), 0, 0, 1, -1.0f);
    }
    if (target->mC58.mC0 >= 1) {
        if (target->mC58.mDone_unk) {
            switch (target->mC58.mMode) {
            case 0:
                mC58.fn_8011C044(3);
                if (mC58.mBC <= 0) {
                    mC58.mBC = m150C + rand() % m1510;
                }
                break;
            case 1:
                mC58.fn_8011C044(4);
                break;
            case 3:
                mC58.fn_8011C044(4);
                break;
            case 4:
                mC58.fn_8011C044(3);
                if (mC58.mBC <= 0) {
                    mC58.mBC = m150C + rand() % m1510;
                }
                break;
            }
        }
    } else {
        mC58.fn_8011C044(0);
        mC58.mBC = 30;
    }
    if (mode != target->mC58.mMode) {
        target->mC58.mDone_unk = 0;
    }
}

// inferred: moves the character with the AI state (mC58) or along the route
// NONMATCHING: 0% - the original rejects every inline candidate (R3 of docs/inlining.md: Vector3 copies, WrapAngle_unk, DegToRad_unk x8, Abs, Length_unk, GetNormalize, ToRotation_unk and the AI states are all called with bl), ours inlines the AI states 0-2 and the helpers; this also shifts the .sdata2 pool (1.309 = 75 degrees) of every later function
void QuadrupedTask::fn_805675FC() {
    atn::Vector3 rot(mRot);
    float speed = 0.0f;
    if (m55C == 0 || m55C == 5 || m55C == 9 || mFlag2) {
        rot = mRot;
    } else {
        switch (m55C) {
        case 1:
            rot = mRot;
            break;
        case 4: {
            speed = 0.0f;
            rot = mRot;
            if (mC58.m00 == 0) {
                fn_80567424(this);
            }
            fn_80567424(this);
            switch (mC58.mMode) {
            case 0:
                fn_80568EE0(&speed, &rot);
                break;
            case 1:
                fn_80568F10(&speed, &rot);
                break;
            case 2:
                fn_805693CC(&speed, &rot);
                break;
            case 3:
                fn_80569660(&speed, &rot);
                break;
            case 4:
                fn_8056A03C(&speed, &rot);
                break;
            }
            short mode = mC58.mMode;
            float diff = atn::WrapAngle_unk(rot.y - mRot.y);
            if ((unsigned short)(mode - 3) <= 1) {
                if (atn::Abs_unk(diff) >= atn::DegToRad_unk(75.0f)) {
                    fn_8016D454(diff);
                    return;
                }
            } else if (mode == 1) {
                if (atn::Abs_unk(diff) >= atn::DegToRad_unk(45.0f)) {
                    float a = diff > atn::DegToRad_unk(90.0f) ? atn::DegToRad_unk(90.0f) : diff;
                    float b = atn::DegToRad_unk(-90.0f) > a
                                  ? atn::DegToRad_unk(-90.0f)
                                  : (diff > atn::DegToRad_unk(90.0f) ? atn::DegToRad_unk(90.0f) : diff);
                    fn_8016D454(b);
                    return;
                }
            }
            break;
        }
        case 7:
            if (mStatus.fn_80139F4C()) {
                speed = 0.0f;
                rot = mRot;
            } else {
                mRoute.fn_80126214();
                atn::Vector3 v(*mRoute.fn_80139F58());
                speed = v.Length_unk();
                if (speed > 0.0001f) {
                    rot = v.GetNormalize().ToRotation_unk();
                } else {
                    rot = mRot;
                }
            }
            break;
        case 6:
            fn_8013A258();
            return;
        default:
            return;
        }
    }
    vf34(rot, 0, speed, 1.0f * mStatus.fn_8012DB04());
}

void QuadrupedTask::vf34(const atn::Vector3& rot, int arg, float a, float b) {
    fn_80567968(rot, 0, a, m568 * b);
}

// NONMATCHING: 94.65% - same as Em403WarLion::fn_80324A38 (FPR of the turn and of the clamp of m584, stores
// of move.x/z around sin, order of m574 at the end); ~6 variants
// inferred: moves the leopard towards the rotation `target` (as Em403WarLion::fn_80324A38, plus the push
// forward of the first 24 frames of the attack)
void QuadrupedTask::fn_80567968(const atn::Vector3& target, int arg, float speed, float rate) {
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

// NONMATCHING: 99.87% - dt in f29 instead of f28 (only the register); ~10 variants
// inferred: chooses the movement motions (same code as Em403WarLion::vf24; m500/m508 read before the rate
// give the comparison registers of the original)
void QuadrupedTask::vf24() {
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
// NONMATCHING: 97.62% - same as Em403WarLion::fn_803256F8 (one more saved GPR and order of the quaternions)
void QuadrupedTask::fn_805686F0(atn::MotionBuffer* nodes, int count) {
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

// inferred: turns the neck with the lean and the waist with m14B0.y (node matrix callback; as in Em403WarLion)
// NONMATCHING: 98.65% - same as Em403WarLion::fn_803258D4 (r30/r31 of the node, stack of the matrices)
void QuadrupedTask::fn_805688CC(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk) {
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
            atn::Vector3 rot(0.0f, m14B0.y, 0.1f * m14B0.y);
            atn::Matrix3x4 r;
            MakeRotationZYX_unk(r, rot);
            PSMTXConcat(r.m, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    }
}

// NONMATCHING: 91.11% - the original reloads the flag word 0x12A4 for mFlag31, schedules the sums of m614
// differently and swaps r3/r4 in the Vector3 copies; ~14 variants
// inferred: places the collision capsule along the body (own version of CharaTask::vf20; as Em403WarLion)
void QuadrupedTask::vf20() {
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


// NONMATCHING: 99.19% - only the .sdata2 label (pool order, see fn_805675FC)
void QuadrupedTask::vf74(atn::Vector3* dir) {
    fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlMountleQuadruped(this, *dir, 2.5f));
}

// NONMATCHING: 99.33% - only the .sdata2 label (pool order, see fn_805675FC)
void QuadrupedTask::vf8C(atn::Vector3* vel) {
    fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlDropDownQuadruped(this, *vel));
}

// inferred: AI state 0 (wait): counts down mC58.mBC, then raises the flag 0x4000 of the state
void QuadrupedTask::fn_80568EE0(float* speed, atn::Vector3* rot) {
    if (mC58.mBC > 0) {
        mC58.mBC--;
    }
    if (mC58.mBC <= 0) {
        mC58.mDone_unk = 1;
    }
}

// inferred: AI state 1: turns towards the target of mC4 when the angle is above 45 degrees
// NONMATCHING: 99.80% - only the .sdata2 labels (pool order, see fn_805675FC)
void QuadrupedTask::fn_80568F10(float* speed, atn::Vector3* rot) {
    *speed = 0.5f;
    atn::Vector3 d = mC58.fn_8011BF3C(0) - mPos;
    *rot = d.ToRotationZ_unk();
    if (atn::Abs_unk(atn::WrapAngle_unk(rot->y - mRot.y)) > atn::DegToRad_unk(45.0f)) {
        *rot = d.ToRotation_unk();
    } else {
        *rot = mRot;
    }
    if (mC58.mC4 != 0) {
        if (mC58.mBC > 0) {
            mC58.mBC--;
        }
        if (mC58.mBC <= 0) {
            mC58.mDone_unk = 1;
        }
    }
}

// inferred: AI state 2: follows the route of mRoute
// NONMATCHING: 99.85% - only the .sdata2 labels (pool order, see fn_805675FC)
void QuadrupedTask::fn_805693CC(float* speed, atn::Vector3* rot) {
    RouteWalker_unk& route = mC58.m0C;
    route.fn_80126214();
    if (route.m2C != 0) {
        mC58.mDone_unk = 1;
    }
    atn::Vector3 v(route.m58);
    *speed = v.Length_unk();
    if (*speed > 0.0001f) {
        atn::Vector3 n = v.GetNormalize();
        *rot = n.ToRotation_unk();
    } else {
        *rot = mRot;
    }
}

// inferred: AI state 3: keeps a distance of m1500 from mC4, circling around it
// NONMATCHING: 99.80% - only the .sdata2 labels (pool order, see fn_805675FC)
void QuadrupedTask::fn_80569660(float* speed, atn::Vector3* rot) {
    if (mC58.mC4 != 0) {
        if (mC58.mBC > 0) {
            mC58.mBC--;
        }
        if (mC58.mBC <= 0) {
            bool linked = mC58.mC4->fn_8016F4D8(this) ? true : mC58.mC4->fn_8016F530(0) != 0;
            if (linked) {
                mC58.mDone_unk = 1;
                mC58.mC4->fn_8016F530(this);
            }
        }
        atn::Vector3 d = mC58.mC4->mPos - mPos;
        *speed = d.Length_unk();
        if (*speed < 0.00001f) {
            *speed = 0.0f;
            *rot = mRot;
            mC58.mDone_unk = 1;
            return;
        }
        d.Normalize_unk();
        if (*speed < 0.8f * m1500) {
            *speed = 1.0f < (m1500 - *speed > 0.41f ? m1500 - *speed : 0.41f)
                         ? 1.0f
                         : (m1500 - *speed > 0.41f ? m1500 - *speed : 0.41f);
            *rot = atn::Vector3(-d.x, -d.y, -d.z).ToRotation_unk();
            rot->y -= atn::DegToRad_unk(45.0f);
        } else if (*speed > 1.2f * m1500) {
            *speed = 0.5f;
            atn::Vector3 t = mC58.fn_8011BF3C(0) - mPos;
            *rot = t.ToRotationZ_unk();
            if (atn::Abs_unk(atn::WrapAngle_unk(rot->y - mRot.y)) > atn::DegToRad_unk(45.0f)) {
                *rot = t.ToRotation_unk();
            } else {
                *rot = mRot;
            }
        } else {
            *speed = 0.5f;
            *rot = d.ToRotation_unk();
            rot->y += atn::DegToRad_unk(90.0f);
        }
    } else {
        mC58.mDone_unk = 1;
    }
}

// inferred: AI state 4: walks the route up to mC4, waits next to it, then turns in place for 30 frames
// NONMATCHING: 99.78% - FPR of the radius of mC4 (f3/f0 swapped, accessor 0x8013C42C inlined?) and .sdata2 labels; ~6 variants
void QuadrupedTask::fn_8056A03C(float* speed, atn::Vector3* rot) {
    CharaTask* target = mC58.mC4;
    RouteWalker_unk& route = mC58.m0C;
    if (target != 0) {
        if (mC58.mStep == 0) {
            route.fn_80126214();
            if (route.mPointA == route.mPointB) {
                route.fn_80127D8C(0, 0, 0);
            } else {
                atn::Vector3 v(route.m58);
                *speed = v.Length_unk();
                if (*speed > 0.0001f) {
                    *rot = v.GetNormalize().ToRotation_unk();
                } else {
                    *rot = mRot;
                }
            }
            if (route.m2C != 0) {
                mC58.mStep = 1;
            }
        } else if (mC58.mStep == 1) {
            mC58.mE8++;
            atn::Vector3 d = mC58.fn_8011BF3C(0) - mPos;
            d.y = 0.0f;
            *speed = d.Length_unk();
            *speed = *speed - mC58.mC4->m5B0;
            if (*speed > mStatus.mParamB.m28) {
                *rot = d.ToRotation_unk();
            } else {
                *speed = 0.0f;
                *rot = mRot;
                mC58.mStep = 2;
            }
        } else if (mC58.mStep == 2) {
            mC58.mE8 = 0;
            mC58.mStep = 3;
            target->fn_8016F5EC(this);
        } else if (mC58.mStep == 3) {
            *speed = 2.0f;
            *rot = mRot;
            atn::Vector3 d = mC58.fn_8011BF3C(0) - mPos;
            if (atn::WrapAngle_unk(rot->y - d.ToRotationZ_unk().y) > 0.0f) {
                rot->y += atn::DegToRad_unk(2.0f);
            } else {
                rot->y -= atn::DegToRad_unk(2.0f);
            }
            if (++mC58.mE8 > 30) {
                mC58.mE8 = 0;
                mC58.mDone_unk = 1;
            }
        }
    } else {
        mC58.mDone_unk = 1;
    }
}
