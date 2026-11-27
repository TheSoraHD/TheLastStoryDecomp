#include "game/GameConfig_unk.h"
#include "atn/RenderSetting_unk.h"
#include "macros.h"
#include "math/VecAssign_unk.h"
#include "game/QuadrupedTask.h"
#include "game/GameScene.h"
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
#include "atn/RenderManager.h"
#include "game/GameTask.h"
#include "atn/ColliBase.h"
#include "game/BattleTask.h"
#include "game/GameSceneFind_unk.h"

// inferred: node position (as in game/Em205Ogre.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}
extern "C" GameTask* lbl_8087F430;
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
extern "C" void C_QUATRotAxisRad(atn::Quaternion_unk* q, const atn::Vector3* axis, float rad); // inferred: C_QUATRotAxisRad

// inferred: node callbacks of the model (as in game/Em403WarLion.cpp)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
extern "C" void fn_800F29D0(atn::Model* model, NodeCallback_unk callback);

extern "C" void fn_8031F2A4(atn::Model* model, boost::function<void(atn::MotionBuffer*, int)> callback);
struct ColliManager_unk;
extern "C" ColliManager_unk* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004D388(void* world, atn::ColliResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager

extern "C" int rand();
extern "C" double sqrt(double x);
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" RenderSetting_unk* lbl_8087EFA8;

// inferred: GameScene::FindPlace_unk with the array read inside the loop (as in game/Em208Queen.cpp)
inline gs::WarpPoint* FindRoutePlace2_unk(GameScene* table, int id) {
    for (unsigned int i = 0; i < table->m78.size(); i++) {
        gs::WarpPoint* places = table->m78.mData;
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




extern "C" GameConfig_unk* lbl_8087F0A8;

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
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC015;
        mStatus.m18 |= 0x3000000;
        atn::Vector3 limit(1.5707964f, 3.1415927f, 0.0f);
        mLook.mLimit = limit;
        return 1;
    }
    return 0;
}

void QuadrupedTask::vf10() {
    if (m55C == 2) {
        m55C = 4;
    }
    atn::Vector3 pos(mPos);
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
    atn::Vector3 d2 = atn::RenderManager::instance_->mCamera.mPos - mPos;
    fn_80148B38(PSVECSquareMag(&d2));
}


extern "C" void fn_80087994(void* group, const char* name, int* value, void* func, void* arg);
// inferred: debug menu of the enemy; removed by the linker, only its strings are left in the pool
// ("Quadruped", "DebugDisp" between "waist" and "spine"), as Em401_01_WhiteLeoPard::fn_DebugMenu_unk.
void QuadrupedTask::fn_DebugMenu_unk() {
    void* group = m1560.Load("Quadruped");
    fn_80087994(group, "DebugDisp", &m1564, 0, 0);
}

void QuadrupedTask::vf14() {
    fn_80149A30();
    if (lbl_8087F0A8->mDrawCharaColli != 0) {
        lbl_8087EEB0->fn_80063484(20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

void QuadrupedTask::vf3C(HitInfo_unk* hit, int) {
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

// inferred: state machine (m58C)
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
            prev -= mPos;
            if (m570 > 0.1f && atn::Abs_unk(prev.x) < 0.1f && atn::Abs_unk(prev.y) < 0.1f &&
                atn::Abs_unk(prev.z) < 0.1f) {
                if (++mC58.m2B0 > 3) {
                    if (fn_8016F67C()) {
                        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                        Mtx m;
                        PSMTXRotRad(m, 'y', mRot.y);
                        PSMTXMultVec(m, &dir, &dir);
                        vf74(dir);
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
            prev -= mPos;
            if (m570 > 0.1f && atn::Abs_unk(prev.x) < 0.1f && atn::Abs_unk(prev.y) < 0.1f &&
                atn::Abs_unk(prev.z) < 0.1f) {
                if (++mC58.m2B0 > 3 && fn_8016F67C()) {
                    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                    Mtx m;
                    PSMTXRotRad(m, 'y', mRot.y);
                    PSMTXMultVec(m, &dir, &dir);
                    vf74(dir);
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
            int damage = lbl_8087F048->fn_800F8C6C(0, 0, this, m638, m590, 0.0f, 30, m594);
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


// inferred: chooses the move of the AI state (mC58) from the distance to its target
// inferred: route table of the game task; base and random range of the wait of the AI (names invented; read
// through inlines, found with the Val_unk probe)
static inline GameScene* TableOf_unk(GameTask* task) {
    return task->m10D8;
}

static inline int WaitBaseOf_unk(const QuadrupedTask* self) {
    return self->m150C;
}

static inline int WaitRangeOf_unk(const QuadrupedTask* self) {
    return self->m1510;
}

// NONMATCHING: 96.9% - the original keeps &target->mC58.m0C in r27 from the start (one more saved GPR) and calls
// rand before reading the wait; the table and the wait through accessors (Val_unk probe; 90.6 -> 96.9)
int QuadrupedTask::fn_80567424(CharaTask* target) {
    GameScene* table = TableOf_unk(lbl_8087F430);
    if (table == 0) {
        return 0;
    }
    CharaTask* other = (CharaTask*)target->mC58.mC4;
    short mode = target->mC58.mMode;
    if (other != 0) {
        atn::Vector3 d = target->mPos - other->mPos;
        d.Length_unk();
    }
    if (target->mC58.m00 == 0) {
        target->mC58.m0C.mDestPoint = ((GameScene*)table)->fn_803C1560(&target->mPos, target->fn_80179D44(), 0, 0, 1, -1.0f);
    }
    if (target->mC58.mC0 >= 1) {
        if (target->mC58.mDone_unk) {
            switch (target->mC58.mMode) {
            case 0:
                mC58.fn_8011C044(3);
                if (mC58.mBC <= 0) {
                    mC58.mBC = WaitBaseOf_unk(this) + rand() % WaitRangeOf_unk(this);
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
    return 1;
}

// The original calls every inline helper and AI state of fn_805675FC out of line (over the inline budget);
// these empty asserts in a helper that it calls carry the missing cost.
#define QUAD_ASSERT_UNK(x) ((void)0)
// inferred: fabs (weak copy 0x80011220, as in Event/Puppeteer_CharaTask.cpp)
extern "C" inline float fn_80011220(float x) {
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0);     QUAD_ASSERT_UNK(0); 
    double r = __fabs(x);
    return r;
}

// inferred: moves the character with the AI state (mC58) or along the route
void QuadrupedTask::fn_805675FC() {
    atn::Vector3 rot(mRot);
    float speed = 0.0f;
    float scale = 1.0f;
    if (m55C == 0 || m55C == 5 || m55C == 9 || mFlag2) {
        rot = mRot;
    } else if (m55C == 1) {
        rot = mRot;
    } else if (m55C == 4) {
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
        float diff = atn::WrapAngle_unk(rot.y - mRot.y);
        if ((unsigned short)(mC58.mMode - 3) <= 1) {
            if (fn_80011220(diff) >= atn::DegToRad_unk(75.0f)) {
                fn_8016D454(diff);
                return;
            }
        } else if (mC58.mMode == 1) {
            if (fn_80011220(diff) >= atn::DegToRad_unk(45.0f)) {
                float a = atn::DegToRad_unk(90.0f) < diff ? atn::DegToRad_unk(90.0f) : diff;
                float b = atn::DegToRad_unk(-90.0f) > a
                              ? atn::DegToRad_unk(-90.0f)
                              : (atn::DegToRad_unk(90.0f) < diff ? atn::DegToRad_unk(90.0f) : diff);
                fn_8016D454(b);
                return;
            }
        }
    } else if (m55C == 7) {
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
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    scale *= mStatus.fn_8012DB04();
    vf34(rot, 0, speed, scale);
}

void QuadrupedTask::vf34(const atn::Vector3& rot, int arg, float a, float b) {
    fn_80567968(rot, 0, a, m568 * b);
}

// inferred: the ground test of the move (attribute 0x1A of the polygon hit); name invented
inline bool QuadIsLanded_unk(const atn::ColliResult_unk& result) {
    bool landed = false;
    if (result.m34 != 0 && *(unsigned int*)result.m34 == 0x1A) {
        landed = true;
    }
    return landed;
}
// inferred: flag test of the character parameters (as CharaParam_unk::fn_801404E0, inlined here); name
// invented
inline bool QuadHasFlag_unk(const CharaParam_unk& param, unsigned int flag) {
    return (param.m9C & flag) == flag;
}
// inferred: the push of state 1 is scaled by mStatus.fn_8012DB04()
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
    float k = atn::Abs_unk(diff / 3.1415927f);
    k *= 0.75f;
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
        angle /= 1.0f - ofs;
        speed *= 1.0f - 1.5f * angle;
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
    float y = rot.y;
    move.y = 0.0f;
    move.z = m570 * (float)cos(y);
    move.x *= dt;
    move.y *= dt;
    move.z *= dt;
    if (m570 > 1.0f) {
        move.x *= 2.0f;
        move.y *= 2.0f;
        move.z *= 2.0f;
    }
    // inferred: the original tests a default-constructed status here (temporary built and destroyed
    // around the test), not mStatus
    if (!QuadHasFlag_unk(CharaStatus().mParamB, 1)) {
        int fps = lbl_8087F0A8->m030;
        m574.y += dt * (-98.1f / (float)(fps * fps));
    }
    move += m6B8;
    m6B8.x *= 0.8f;
    m6B8.z *= 0.8f;
    m6B8.y = 0.0f;
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
        move += push * mStatus.fn_8012DB04();
    }
    int keepRot = 1;
    atn::Vector3 oldPos(pos);
    atn::ColliResult_unk result;
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
    const atn::Vector3& hitPos = result.m10;
    pos = hitPos;
    pos -= m5A4;
    pos.y -= sphere.mRadius;
    if (hit != 0 && oldPos.y - pos.y < 0.95f * -move.y) {
        move.y = 0.0f;
    }
    mFlag3 = QuadIsLanded_unk(result);
    float lean = 0.0f;
    atn::Vector3 vel(move + (pos - oldPos));
    if (PSVECSquareMag(&vel) > 0.001f && speed > 0.001f) {
        lean = atn::WrapAngle_unk(vel.GetNormalize().ToRotation_unk().y - mRot.y);
    }
    lean *= 2.0f;
    m584 = MAX(MIN(m584 - 0.5f * lean, 1.0471976f), -1.0471976f);
    m584 *= 0.9f;
    if ((m54C & 2) == 2 && pos.y < 0.0f) {
        pos.y = 0.0f;
    }
    mPos = pos;
    if (keepRot) {
        mRot = rot;
    }
    m574 = move;
    m574.x /= dt;
    m574.z /= dt;
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

// NONMATCHING: 99.96% - dt in f29 instead of f28 (only the register); ~10 variants
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
        dashSpeed = 1.0f + 0.1f * (rate / dashMin - 1.0f);
    }
    if (PSVECSquareMag(&(atn::RenderManager::instance_->mCamera.mPos - mPos)) > 62500.0f) {
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

// inferred: rotation of a node pose (name invented; read through an inline, found with the Val_unk probe)
inline atn::Quaternion_unk RotOf_unk(const atn::MotionBuffer& node) {
    return node.m10;
}

// inferred: rotation around an axis (name invented, as in game/Em403WarLion.cpp)
inline atn::Quaternion_unk QuatAxis_unk(const atn::Vector3& axis, float rad) {
    atn::Quaternion_unk q;
    C_QUATRotAxisRad(&q, &axis, rad);
    return q;
}

// inferred: turns the neck and the spine with the lean (node pose callback; as in Em403WarLion)
// NONMATCHING: 97.62% - same as Em403WarLion::fn_803256F8 (one more saved GPR and order of the quaternions)
void QuadrupedTask::fn_805686F0(atn::MotionBuffer* nodes, int count) {
    for (int i = 0; i < count; i++) {
        const char* name = ((ModelNodeDesc_unk*)mMotion.GetRes_unk()->m48[i])->mName;
        if (strcmp(name, "neck") == 0) {
            if (m648 == 0) {
                atn::Quaternion_unk rot(RotOf_unk(nodes[i]));
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

// inferred: turns the neck with the lean and the waist with m14B0.y (node matrix callback; as in Em403WarLion)
// inferred: rotation z, y, x written in place with the matrix through a pointer declared at the top (with an
// inline taking the matrix the temporaries of the three axes get the stack slots in the other order)
void QuadrupedTask::fn_805688CC(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk) {
    atn::Matrix3x4* p;
    if (mLook.fn_8012A288(mtx, (ModelNodeDesc_unk*)&chunk) != 0) {
        return;
    }
    const char* name = ((ModelNodeDesc_unk*)&chunk)->mName;
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
            p = &r;
            p->SetIdentity();
            if (rot.z != 0.0f) {
                atn::Matrix3x4::RotateAxis_unk(*p, 'z', rot.z);
            }
            if (rot.y != 0.0f) {
                atn::Matrix3x4::RotateAxis_unk(*p, 'y', rot.y);
            }
            if (rot.x != 0.0f) {
                atn::Matrix3x4::RotateAxis_unk(*p, 'x', rot.x);
            }
            PSMTXConcat(r.m, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    }
}

// inferred: mFlag31 read through an int accessor: the original reloads the flag word 0x12A4 for it
// (a bool accessor adds two cntlzw); name invented
static inline int OnLedge_unk(const CharaTask* chara) {
    return chara->mFlag31;
}

// inferred: places the collision capsule along the body (own version of CharaTask::vf20; as Em403WarLion)
void QuadrupedTask::vf20() {
    m620 = m5B0;
    if (mFlag13) {
        if (m648 != 0) {
            if (!OnLedge_unk(this)) {
                m620 += 2.0f;
            }
        } else if (m570 > 2.0f * m500 && m508 > 2.005f * m500) {
            m620 = 2.0f * ((m570 - 2.0f * m500) / (m508 - 2.0f * m500)) + m5B0;
        }
    }
    m620 *= 1.5f;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 a(mPos.x, mPos.y + m620, mPos.z);
    atn::Vector3 b;
    int tail = mMotion.GetNodeNoByName("tale", 0);
    if (tail > -1) {
        atn::VecAssign_unk(b, GetNodePos_unk(mMotion.mMtx.mNodes.mData[tail].m));
        b.y -= 1.0f;
    } else {
        atn::VecAssign_unk(b, a);
        b.y += m5B4 - 2.0f * m620;
    }
    float radius = m620;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = radius;
    atn::VecAssign_unk(m148C, GetNodePos_unk(mMotion.GetNodeMtx_unk("neck")));
    m1498 = 3.0f;
}


void QuadrupedTask::vf74(const atn::Vector3& dir) {
    fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlMountleQuadruped(this, dir, 2.5f));
}

void QuadrupedTask::vf8C(const atn::Vector3& vel) {
    fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlDropDownQuadruped(this, vel));
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
            *speed = *speed - (float)mC58.mC4->m5B0;
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
