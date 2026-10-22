#include "game/Em401_01_WhiteLeoPard.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

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
extern "C" void fn_800F29D0(Model_unk* model, NodeCallback_unk callback);

// inferred: sets the second node pose callback of a model (also called by game/Em403WarLion.cpp)
extern "C" void fn_8031F2A4(Model_unk* model, boost::function<void(atn::MotionBuffer*, int)> callback) {
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
        int hand = mMotion.fn_80092814("lefthand", 0);
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
extern "C" LeoMaterial_unk* fn_80094B6C(Model_unk* model, const char* name); // inferred: material by name
// inferred: color as four floats (as in game/Em315Coffin.cpp)
struct LeoColor_unk {
    LeoColor_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    float r, g, b, a;
};
extern "C" double sin(double x);
// inferred: settings (lbl_8087F0A8) and debug drawing (lbl_8087EEB0), as in game/Em403WarLion.cpp
struct LeoSettings_unk {
    unsigned char m00[0x34];
    int mDrawDebug; // 0x34
};
extern "C" LeoSettings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063484(void* draw, int segments, unsigned int color, float x, float y, float z, float rot,
                            float a, float range, float angle); // inferred: fan

// NONMATCHING: 99.85% - only the labels of the two doubles of .rodata (int to float constants placed by the
// functions not written yet); 100% in report.json
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
        fn_80063484(lbl_8087EEB0, 20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f,
                    1.5707964f);
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
