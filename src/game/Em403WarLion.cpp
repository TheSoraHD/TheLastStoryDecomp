#include "game/Em403WarLion.h"
#include "atn/LoadHandle.h"
#include "chr/ControlBase.h"
#include "chr/ControlStickByQuark.h"
#include "atn/Memory.h"
#include "game/FriendManager.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "gmk/Lemon.h"
#include "math/Vec.h"
#include "macros.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

// inferred: circuit of the war lion around the arena: place id and two flags (names invented)
struct WarLionPlace_unk {
    int mPlace;
    int m04; // inferred: running point (skipped by fn_8032C3CC when 0)
    int m08;
};
extern "C" const WarLionPlace_unk lbl_80749B00[10] = {
    {0x258, 0, 0}, {0x25D, 0, 0}, {0x259, 1, 0}, {0x261, 0, 1}, {0x25A, 1, 0},
    {0x260, 0, 1}, {0x25B, 1, 0}, {0x25F, 0, 1}, {0x25C, 1, 0}, {0x25E, 0, 1},
};

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: effect/bullet manager of the characters (class unknown)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80107A68(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_801079C0(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion);
// inferred: settings (lbl_8087F0A8) and debug drawing (lbl_8087EEB0), as in game/CopyRobotTask.cpp
struct Settings_unk {
    unsigned char m00[0x30];
    int m30; // inferred: frames per second (as in game/CharaTask.cpp)
    int mDrawDebug; // 0x34
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063484(void* draw, int segments, unsigned int color, float x, float y, float z, float rot,
                            float a, float range, float angle); // inferred: fan
// inferred: field (lbl_8087F430): camera shake at 0x96C (as in game/GimmickStateCommand.cpp) and route
// tables at 0x10D8
struct Field_unk {
    unsigned char m000[0x96C];
    int m96C; // inferred: camera shake
    int m970;
    float m974;
    float m978;
    unsigned char m97C[0x10D8 - 0x97C];
    RouteTable_unk* m10D8;
    // inferred: route tables (accessor 0x8013C504 of game/CharaTask.cpp)
    RouteTable_unk* fn_8013C504() { return m10D8; }
};
extern "C" Field_unk* lbl_8087F430;
// inferred: field accessor (weak copy 0x80121F00, as in game/EnemyJill.cpp)
extern "C" inline Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
// inferred: game state (lbl_8087EFA8, see game/CharaTask.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8;
extern "C" int fn_8004D388(void* world, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" double sin(double x);
extern "C" double cos(double x);
// inferred: camera (lbl_8087EFB4, see game/CharaTask.cpp); names invented
struct Camera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mEye; // 0x10C
};
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" float fn_800A08D4(atn::LoadHandle* motion); // inferred: length of the motion data
// inferred: length of a motion, 30 frames when it is missing (as in game/CharaTask.cpp); name invented
inline float GetMotionLength_unk(Model_unk* model, int id) {
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
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_800EB7A0(CharaTask* chara);  // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara);  // inferred: BattleCharaTask (enemies only)
extern "C" HitInfo_unk* fn_80219E6C(int id);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: collision world and its ray tests (as in gmk/Cannonball.cpp)
extern "C" void* lbl_8087EE98;
void* fn_801404F8();
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" int fn_8004ED34(void* world, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test

// inferred: plays an effect on the model (name invented, as in game/Em002Skelton.cpp)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

// inferred: starts the state `state` (inlined in every state change; name invented)
inline void Em403WarLion::SetState_unk(int state) {
    m14C4 = 0;
    m14C8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}

Em403WarLion::Em403WarLion(atn::Task* parent, void* data)
    : EnemyTask(parent, data), m14B0(0.0f), m14B4(0.0f), m14B8(0.0f), m14BC(0), m14C0(0), m14C4(0), m14C8(0),
      m14CC(0), m14D0(0), m14D4(0), m14DC(0.0f, 0.0f, 0.0f), m14E8(0.0f, 0.0f, 0.0f), m14F4(0), m1598(0),
      m15CC(0), m15D0(0), m15D4(0), m15D8(0), m15DC(0), m15E0(0), m15E4(0), m15E8(0), m15EC(0), m15F0(0),
      m15F4(0) {
    mFlag22 = 1;
    m1608.mRes = 0;
    m160C = 0;
    mStatus.mParamB.m9C |= 0x10;
    m1524.Reset_unk();
    m15FC.fn_8023780C("/effect/ef_bs001a.efp");
    m14A8 |= 0x80000000;
}

Em403WarLion::~Em403WarLion() {}

// inferred: node callbacks of the model (as in game/CharaTask.cpp and game/CharaFaceControl.cpp)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
extern "C" void fn_800F29D0(Model_unk* model, NodeCallback_unk callback);
// inferred: sets the node pose callback (game/Em401_01_WhiteLeoPard.cpp, not decompiled)
extern "C" void fn_8031F2A4(Model_unk* model, boost::function<void(atn::MotionBuffer*, int)> callback);

int Em403WarLion::vf0C() {
    int result = 1;
    if (fn_8013655C() != 0) {
        result = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        result = 0;
    }
    if (m15FC.fn_80237874() != 0) {
        result = 0;
    }
    if (result != 0) {
    mStatus.m18 |= 0x1C0;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0x8219;
    mStatus.m18 |= 0x2800000;
    m56C = 0.125f;
    m500 = 0.8f;
    m508 = 1.0f;
    fn_800F29D0(&mMotion, boost::bind(&Em403WarLion::fn_803258D4, this, _1, _2));
    mMotion.fn_80091CFC("neck");
    mMotion.fn_80091CFC("waist");
    fn_8031F2A4(&mMotion, boost::bind(&Em403WarLion::fn_803256F8, this, _1, _2));
    atn::Vector3 limit(0.7853982f, 0.62831855f, 0.0f);
    mLook.mLimit = limit;
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    return 1;
    }
    return 0;
}

// inferred: setter of the state (weak copy 0x802657FC of game/Em201ZanGurgFinal.cpp)
extern "C" void fn_802657FC(CharaTask* chara, int state);
// inferred: BattleCharaTask flag 0x04000000 of m14A8 (weak copy 0x800EC1F4)
extern "C" void fn_800EC1F4(BattleCharaTask* chara, int value);
// inferred: next character of a manager list (weak copy 0x802A4094)
extern "C" CharaTask* fn_802A4094(CharaTask* chara);
// inferred: model matrix (weak copy 0x80266154)
extern "C" atn::Matrix3x4* fn_80266154(Model_unk* model);
extern "C" float fn_80011220(float x); // inferred: fabs (weak copy of another helper)
Effect_unk* fn_8013A194();
// inferred: AI data of an ally (class unknown): weights by enemy at 0x128
struct AllyAi_unk {
    unsigned char m000[0x128];
    float m128[1];
};
extern "C" AllyAi_unk* fn_80103F60(Effect_unk* mgr, CharaTask* chara);
extern "C" int fn_80108378(AllyAi_unk* ai, CharaTask* enemy); // inferred: index of `enemy`, < 0 if missing

// inferred: singleton accessor (weak copy, also called by game/GameUI.cpp)
extern "C" inline FriendManager* fn_80323C2C() {
    return FriendManager::sInstance;
}

// inferred: atn::DegToRad_unk called out of line by vf10 (over the inline budget); the original links the
// copy of another unit (0x801125F8), so its constant is not in this pool. Declared with its mangled name
extern "C" float DegToRad_unk__3atnFf(float deg);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
// inferred: Matrix3x4 assignment out of line (weak copy of atn/Model.cpp, 0x8008CD1C)
extern "C" void fn_8008CD1C(atn::Matrix3x4* dst, const atn::Matrix3x4* src);
extern "C" int rand();
// inferred: translation matrix by value (name invented)
inline atn::Matrix3x4 TransMtx_unk(float x, float y, float z) {
    atn::Matrix3x4 m;
    atn::Vector3 v;
    v.Set_unk(x, y, z);
    PSMTXTrans(m.m, v.x, v.y, v.z);
    return m;
}

// inferred: over the inline budget: the helpers are called out of line (weak copies emitted after it)
void Em403WarLion::vf10() {
    m14C8++;
    m14BC = mC58.mC4;
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    int raging = IsRaging_unk();
    bool target = true;
    if (mC58.mC0 == 0 || mC58.mC4 == 0) {
        target = false;
    }
    if (!target) {
        fn_8016E970(3);
        fn_802657FC(this, 6);
    } else {
        if (m14CC == 0 && !(m58C - 9U <= 2) && !(m58C - 20U <= 1) && !IsRaging_unk()) {
            int count = 0;
            atn::Vector3 v;
            float angle;
            if (fn_80329ED8(&count, &v, &angle)) {
                if (++m14D0 >= 8) {
                    m14DC = fn_8032A650(&m14FC, 0, 1);
                    atn::Vector3 next = fn_8032A894(m14FC, 0);
                    m14E8 = (next - m14DC).ToRotation_unk();
                    m14CC = 1;
                    m14F8 = rand() % 3 + 4;
                    fn_8032C3CC(&m14F8, m14FC);
                    m14D0 = 0;
                    m15F8 = fn_800F8548(fn_8013A194());
                } else {
                    m14DC = v;
                    m14E8.Set_unk(0.0f, angle, 0.0f);
                }
                if (m15D4 != 0) {
                    fn_80326F0C();
                    m15D4 = 0;
                } else if (m15D8 != 0) {
                    fn_80327018();
                    m15D8 = 0;
                } else if (m15DC != 0) {
                    fn_80327118();
                    m15DC = 0;
                } else {
                    fn_803271A4();
                }
            }
        }
        if (m14BC != 0) {
            switch (m58C) {
            case 7:
                fn_80327E78();
                break;
            case 8:
                fn_80327EF0();
                break;
            case 9:
                fn_80328270();
                break;
            case 10:
                fn_8032853C();
                break;
            case 21:
                fn_803288C4();
                break;
            case 11:
                fn_803289EC();
                break;
            case 12:
                fn_80328B74();
                break;
            case 13:
                fn_80328C6C();
                break;
            case 14:
                fn_8032932C();
                break;
            case 15:
                fn_8032943C();
                break;
            case 16:
                fn_80329744();
                break;
            case 17:
                fn_80329A20();
                break;
            case 18:
                fn_80329CFC();
                break;
            case 19:
                fn_80329C7C();
                break;
            case 20:
                fn_80329D7C();
                break;
            case 2:
                vfEC();
                break;
            default:
                if (m55C != 6) {
                    fn_803267E8();
                }
                if (m58C == 6) {
                    fn_80327E1C();
                }
                break;
            }
            int state = m58C;
            mFlag24 = 0;
            if (state - 8U <= 7 || state - 20U <= 1 || state == 2) {
                mLook.fn_80129A48(0.3f);
            } else {
                mLook.fn_80129978(m14BC->GetModel_unk(), "neck", lbl_807C7030, 0.3f);
            }
            fn_800EC1F4(this, m58C != 20);
        }
    }
    for (CharaTask* c = fn_80323C2C()->fn_8036554C(); c != 0; c = fn_802A4094(c)) {
        AllyAi_unk* ai = fn_80103F60(fn_8013A194(), c);
        if (ai != 0) {
            int i = fn_80108378(ai, this);
            if (i >= 0) {
                if (m14CC - 1U <= 1) {
                    ai->m128[i] = 0.0f;
                } else {
                    ai->m128[i] = 1.0f;
                }
            }
        }
    }
    m14B4 *= 0.8f;
    float turn = (rot.y + m14B4) - mRot.y;
    float max = DegToRad_unk__3atnFf(50.0f);
    if (fn_80011220(turn) > max) {
        turn = max * (turn / fn_80011220(turn));
    }
    m14B4 = turn;
    fn_8014C540();
    fn_80145334();
    m15EC--;
    if (raging != 0 && !IsRaging_unk()) {
        m15D4 = 1;
    }
    fn_8008CD1C(&m159C, &TransMtx_unk(0.0f, 0.0f, 10.0f));
    m159C.fn_80149624(3.1415927f);
    m159C.fn_801495E8(*fn_80266154(&mMotion));
}

void Em403WarLion::vf14() {
    fn_80149A30();
    if (lbl_8087F0A8->mDrawDebug != 0) {
        fn_80063484(lbl_8087EEB0, 20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

void Em403WarLion::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->mData->m04 == 10000) {
        fn_80151448(msg);
        vf40(hit);
        return;
    }
    if (m58C == 9 || m58C == 10 || m58C == 19 || m58C == 17 || m58C == 16 || m58C == 20) {
        msg->m10 *= 0.0f;
    }
    if (msg->mData->m04 == 0xD0) {
        fn_80151448(msg);
        vf40(hit);
        return;
    }
    msg->m10 *= 0.0f;
    if (m58C == 11 && ((unsigned int)msg->mData->m90 & 0x4000) == 0x4000) {
        msg->m68 = 0;
        msg->m90 = 1;
        return;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em403WarLion::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    int stop = 0;
    if (m58C == 17) {
        m15CC++;
        if ((mStatus.m0C & 0x20) == 0x20 || m15CC > 3) {
            stop = 1;
        }
    } else if (m58C == 20) {
        m15F4++;
    } else if (m15EC > 0) {
        if (++m15F0 > 5) {
            m15DC = 1;
            m15F0 = 0;
        }
    }
    if (stop) {
        if (m1598 != 0) {
            m1598->fn_8015B238();
            m1598 = 0;
        }
        if (m58C != 9 && m58C != 10 && m58C != 19) {
            m58C = 6;
        }
    }
    if (m58C == 18 && m55C == 6) {
        m58C = 6;
    }
    if (m55C == 6 && m560 == 23) {
        fn_8015E4B0(m6B8, -1);
    }
    if (((unsigned int)msg->mData->m90 & 0x1000000) == 0x1000000) {
        mStatus.fn_80133130(0x4000, 10, 0);
        mStatus.fn_8013322C(0x1000000);
    }
    if ((mStatus.m0C & 0x4000) == 0x4000) {
        if (((unsigned int)msg->mData->m90 & 0x4000) == 0x4000 ||
            ((unsigned int)msg->mData->m90 & 0x1000000) == 0x1000000 ||
            (msg->mAttacker != 0 && msg->mAttacker->mStatus.fn_80133EE8(0x18) != 0)) {
            SetState_unk(20);
            mStatus.fn_8013322C(0x4000);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 0.2f;
            mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
            m15F4 = 0;
            fn_80107A68(lbl_8087F048, &mMotion);
            fn_801079C0(lbl_8087F048, &mMotion, &mMotion);
            fn_80370AE4(lbl_8087F430, 0x5B, 1);
        }
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        m14C4 = 0;
        m14C8 = 0;
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
        if (m1598 != 0) {
            m1598->fn_8016EB48(0, 0, 0);
        }
        fn_800EB7A0(this);
    }
}

atn::Vector3 Em403WarLion::vf94() {
    atn::Vector3 v(0.0f, 0.0f, -7.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(mRot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}

void Em403WarLion::vf54() {
    if (m624.mSize != 0) {
        m624.mData[0].m04.mRadius = mColli.m54;
        m624.mData[0].m04.mCenter = mColli.m3C;
        m624.mData[1].m04.mRadius = mColli.m54;
        m624.mData[1].m04.mCenter = mColli.m48;
    }
}

// inferred: movement of the current mode (own version of CharaTask::fn_80139560)
void Em403WarLion::fn_803241E8() {
    atn::Vector3 rot(mRot);
    float a = 0.0f;
    float b = 1.0f;
    int state = m55C;
    if (state == 0 || state == 5 || state == 9 || mFlag2) {
        rot = mRot;
    } else if (state == 1) {
        rot = mRot;
    } else if (state == 7) {
        if (mStatus.IsState_unk(0x20)) {
            rot = mRot;
        } else {
            mRoute.fn_80126214();
            atn::Vector3 dir(mRoute.m58);
            a = dir.Length_unk();
            if (a > 0.0001f) {
                rot = dir.GetNormalized_unk().ToRotation_unk();
            } else {
                rot = mRot;
            }
        }
    } else if (state == 3) {
        if (m14CC == 0) {
            atn::Vector3 d = m14BC->mPos - mPos;
            a = d.Length_unk();
            if (a < 1e-5f) {
                rot = mRot;
                a = 0.0f;
            } else {
                d.Normalize_unk();
                if (a < 40.0f) {
                    a = 0.0f;
                } else if (a > 55.0f) {
                    a = 0.6f;
                    atn::Vector3 t = mC58.fn_8011BF3C(0) - mPos;
                    rot = t.ToRotationZ_unk();
                    rot.y -= 0.7853982f;
                } else {
                    a = 0.6f;
                    rot = d.ToRotation_unk();
                    rot.y -= 1.5707964f;
                    float diff = atn::WrapAngle_unk(rot.y - mRot.y);
                    if (atn::Abs_unk(diff) >= 1.3089969f) {
                        fn_8016D454(diff);
                        return;
                    }
                }
            }
        }
    } else if (state == 6) {
        fn_8013A258();
        return;
    }
    fn_80324A38(rot, 0, a, b);
}

// inferred: moves the war lion towards the rotation `target` (own version of CharaTask::fn_8013CB68)
// NONMATCHING: 98.22% - registros FPR del giro y del clamp de m584 (frsp), orden de m574 al final
void Em403WarLion::fn_80324A38(const atn::Vector3& target, int arg, float speed, float rate) {
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    float diff = atn::WrapAngle_unk(target.y - rot.y);
    float dt = lbl_8087EFA8->m3A4;
    float cur = rot.y;
    float turn = m56C;
    rate *= dt;
    float k = atn::Abs_unk(diff / 3.1415927f) * 0.75f;
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
        float ofs = 0.25f;
        speed = (speed - 0.4f) / 0.6f;
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
    if (fn_805F9920(&vel) > 0.001f && speed > 0.001f) {
        lean = atn::WrapAngle_unk(vel.GetNormalized_unk().ToRotation_unk().y - mRot.y);
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

// NONMATCHING: 99.75% - dt en f28 en vez de f29 y registros de la comparacion con m500/m508
// inferred: chooses the movement motions (own version of CharaTask::vf24: walk and dash blended by speed)
void Em403WarLion::vf24() {
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
    float rate = m570 / dt;
    float walkMin = m500;
    float dashMin = m508;
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
    if (fn_805F9920(&(lbl_8087EFB4->mEye - mPos)) > 62500.0f) {
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

// inferred: node pose (as in game/CharaFaceControl.cpp)
namespace atn {
struct MotionBuffer {
    unsigned int m00;
    Vector3 m04;
    Quaternion_unk m10;
    Vector3 m20;
};
} // namespace atn
extern "C" void fn_805F9AB0(atn::Quaternion_unk* q, const atn::Vector3* axis, float rad); // inferred: C_QUATRotAxisRad
extern "C" int strcmp(const char* a, const char* b);

// inferred: rotation around an axis (name invented)
inline atn::Quaternion_unk QuatAxis_unk(const atn::Vector3& axis, float rad) {
    atn::Quaternion_unk q;
    fn_805F9AB0(&q, &axis, rad);
    return q;
}

// NONMATCHING: 97.82% - el original guarda un GPR mas (r22) y los temporales del quaternion en otro orden
// inferred: turns the neck and the spine with the lean (node pose callback)
void Em403WarLion::fn_803256F8(atn::MotionBuffer* nodes, int count) {
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

// inferred: m = m * rotation(r) z, y, x (as in game/MapPartsModel.cpp; name invented)
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

// inferred: turns the neck with the lean and the waist with m14B4 (node matrix callback)
// NONMATCHING: 98.79% - r30/r31 del nodo y huecos de pila de las matrices de MakeRotationZYX_unk
void Em403WarLion::fn_803258D4(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk) {
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
            atn::Vector3 rot(0.0f, m14B4, 0.1f * m14B4);
            atn::Matrix3x4 r;
            MakeRotationZYX_unk(r, rot);
            PSMTXConcat(r.m, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    }
}

// inferred: node position (as in game/Em205Ogre.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 97.76% - r3/r4 de las copias de Vector3 (__VecCopy) y offset de "tale" en el pool de cadenas
// inferred: places the collision capsule along the body (own version of CharaTask::vf20)
void Em403WarLion::vf20() {
    m620 = 2.0f * m5B0;
    atn::Vector3 ofs(0.0f, 0.0f, -10.0f);
    atn::Matrix3x4 m;
    MakeRotationZYX_unk(m, mRot);
    PSMTXMultVec(m.m, &ofs, &ofs);
    m614 = mPos + m5A4 + ofs;
    m614.y += m620;
    atn::Vector3 a(mPos.x, mPos.y + m620, mPos.z);
    atn::Vector3 b;
    int tail = mMotion.fn_80092814("tale", 0);
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
    mColli.mFlags &= ~0x80000000;
    m148C = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    m1498 = 5.0f;
}

// inferred: helpers of game/Em204Mulue.cpp (not decompiled): angle wrap to [-180, 180] and radians to
// degrees. Weak copies: fn_803267E8 (over the inline budget) calls them out of line, so their constants
// are pooled right after its own.
extern "C" inline float fn_802A7910(float deg) {
    float d = fmod(deg, 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}
extern "C" inline float fn_802A7964(float rad) {
    return 57.295776f * rad;
}
// inferred: stripped helper (only its constant -100 remains in .sdata2 right after 57.295776f); body and
// name invented: what matters is the order of its literal
extern "C" inline float WarLionStripped1_unk(float y) {
    return -100.0f * y;
}

// NONMATCHING: 99.87% - orden de cuatro stfs del item en la pila
// inferred: two collision spheres, the body and the tail (own version of CharaTask::vf50)
void Em403WarLion::vf50() {
    Item14_unk item;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += 5.0f;
    item.m04.mRadius = 5.0f;
    m624.push_back(item);
    item.m04.mRadius = 5.0f;
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("tale"));
    m624.push_back(item);
    mFlagB29 = 0;
}

// inferred: chooses the next action of the walk mode (m14CC)
// NONMATCHING: 99.93% - f30/f31 de la distancia y del angulo, huecos de pila de los CollResult_unk
// (el original llama fuera de linea a los helpers de Vector3: supera el presupuesto de inline)
void Em403WarLion::fn_803267E8() {
    if (m15D4 != 0) {
        fn_80326F0C();
        m15D4 = 0;
    } else if (m15D8 != 0) {
        fn_80327018();
        m15D8 = 0;
    } else if (m15DC != 0) {
        fn_80327118();
        m15DC = 0;
    } else if (m14BC != 0) {
        atn::Vector3 target(*m14BC->fn_8013C38C());
        atn::Vector3 d = target - mPos;
        float dist = d.Length_unk();
        if (m14CC == 0) {
            if (m14C8 <= (IsRaging_unk() ? 30 : 180)) {
                return;
            }
            if (dist < 40.0f) {
                if (IsRaging_unk() && rand() % 50 < 30) {
                    m15DC = 1;
                }
                int done = 0;
                if (rand() % 50 < 10) {
                    CollResult_unk result;
                    atn::Vector3 from(mPos);
                    from.y += 10.0f;
                    atn::Vector3 to = from + d.GetNormalized_unk() * 500.0f;
                    if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000000, 0, 0) != 0) {
                        result.m10 -= d.GetNormalized_unk() * m620;
                        if ((result.m10 - from).Length_unk() > 80.0f) {
                            m1500 = result.m10;
                            m1500.y = mPos.y;
                            m1518.Set_unk(0.0f, d.ToRotation_unk().y, 0.0f);
                            fn_803279D4();
                            done = 1;
                        }
                    }
                }
                if (done == 0) {
                    fn_80326E60(0);
                }
            } else if (IsRaging_unk()) {
                if (rand() % 3 != 0) {
                    CollResult_unk result;
                    atn::Vector3 from(mPos);
                    from.y += 10.0f;
                    atn::Vector3 to = from + d.GetNormalized_unk() * 500.0f;
                    if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000000, 0, 0) != 0) {
                        result.m10 -= d.GetNormalized_unk() * m620;
                        if ((result.m10 - from).Length_unk() > 60.0f) {
                            m1500 = result.m10;
                            m1500.y = mPos.y;
                            m1518.Set_unk(0.0f, d.ToRotation_unk().y, 0.0f);
                            fn_803279D4();
                        }
                    }
                } else {
                    fn_80327118();
                }
            } else {
                atn::Vector3 pos;
                float angle;
                fn_8032A3D4(&pos, &angle, &target, m14BC->GetRot_unk()->y, 20.0f);
                m14DC = pos;
                m14E8.Set_unk(0.0f, angle, 0.0f);
                fn_803271A4();
            }
        } else if (m14CC == 1) {
            if (m15E4 == 0) {
                m15E4 = 1;
                fn_80370AE4(fn_80121F00(), 0x5A, 3);
            }
            if (m14F8 <= 0) {
                m14CC = 0;
                m1500 = fn_8032A894(m14FC, &m14FC);
                m1518 = (m1500 - mPos).ToRotation_unk();
                fn_803278B4();
            } else {
                m14F8--;
                fn_8032A9F4(&m1524, m14FC, &m14FC);
                if (m14F8 % 3 == 0) {
                    m15F8 = fn_800F8548(fn_8013A194());
                }
                fn_803272A8();
            }
        } else if (m14CC == 2) {
            m14CC = 3;
            m1500 = fn_8032A894(m14FC, &m14FC);
            m1518 = (m1500 - mPos).ToRotation_unk();
            fn_803278B4();
        } else if (m14CC == 3) {
            float y = GetRot_unk()->y;
            float diff = fn_802A7910(fn_802A7964(d.GetNormalized_unk().ToRotation_unk().y - y));
            (void)WarLionStripped1_unk;
            if (fn_80011220(diff) <= 60.0f && dist < 200.0f) {
                atn::Vector3 from(mPos);
                from.y += 10.0f;
                atn::Vector3 to(target);
                to.y += 10.0f;
                if (fn_8004ECC0(fn_801404F8(), 0, &from, &to, 0x80000000, &mColli, 0) == 0) {
                    m14CC = 4;
                    fn_80327A64();
                }
            }
        } else if (m14CC == 4) {
            m14CC = 0;
            m1598 = 0;
        }
    }
}

void Em403WarLion::fn_80326E60(int fast) {
    SetState_unk(8);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = fast ? 2.0f : 1.0f;
}

void Em403WarLion::fn_80326F0C() {
    SetState_unk(9);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 0x65);
    PlayEffect_unk(&m15FC, &mMotion);
    m15DC = 0;
}

void Em403WarLion::fn_80327018() {
    SetState_unk(10);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 0x65);
    PlayEffect_unk(&m15FC, &mMotion);
}

void Em403WarLion::fn_80327118() {
    SetState_unk(21);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
}

void Em403WarLion::fn_803271A4() {
    SetState_unk(11);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.5f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 0x65);
    PlayEffect_unk(&m15FC, &mMotion);
}

// inferred: starts running along the path m1524 (state 13)
void Em403WarLion::fn_803272A8() {
    SetState_unk(13);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x142, 0, 1, 1, 0.0f, 0.1f);
    mRot = (m1524.GetPos_unk(1.0f / 27.0f) - m1524.GetPos_unk(0.0f)).ToRotationZ_unk();
}

// NONMATCHING: 99.72% - r5/r6 de la copia del temporal a m150C (__VecCopy)
void Em403WarLion::fn_803278B4() {
    SetState_unk(14);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x143, 0, 1, 1, 0.0f, 0.1f);
    mRot = m1518;
    m150C = m1500 - mPos;
    m150C *= 1.0f / mMotion.fn_80097D7C(0);
}

void Em403WarLion::fn_803279D4() {
    SetState_unk(15);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 1.5f;
}

// NONMATCHING: 99.95% - offset de "" en @stringBase0 (cadenas de funciones sin escribir)
// inferred: jumps on the target and holds it (state 16)
void Em403WarLion::fn_80327A64() {
    SetState_unk(16);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 1.5f;
    m1574 = mPos;
    m1580 = m14BC->mPos;
    atn::Vector3 d = m1580 - mPos;
    d.y = 0.0f;
    m1580 -= d.GetNormalized_unk() * 10.0f;
    mRot = d.GetNormalized_unk().ToRotation_unk();
    fn_80370AE4(lbl_8087F430, 0x5D, 2);
    m1598 = m14BC;
    if (m1598 != 0) {
        m1598->fn_80178208(new (3, "", "", 0) chr::ControlMatHoldPre(m1598, this));
    }
}

void Em403WarLion::fn_80327E1C() {
    if (m14CC == 3 && !(m55C - 6U <= 1)) {
        fn_80170A20(m14BC, 0, -1.0f);
    }
    fn_803241E8();
}

void Em403WarLion::fn_80327E78() {
    if (m14C8 > 60) {
        SetState_unk(6);
    } else {
        fn_80324A38(mRot, 0, 0.0f, 0.0f);
    }
}

// inferred: difference of angles in degrees in [-180, 180] (as in game/Em201ZanGurg.cpp; name invented)
inline float AngleDiffDeg_unk(float a, float b) {
    float d = a - b;
    d = fmod(57.295776f * d, 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}

// inferred: roar (state 8): turns towards the target, then attacks with bullet 0x672
// NONMATCHING: 99.96% - operandos de un fmuls (0.034906585f * (a / abs))
void Em403WarLion::fn_80327EF0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
        return;
    }
    if (mMotion.mTracks[0].m08 < 20.0f) {
        CharaTask* target = m14BC;
        atn::Vector3 d = target->mPos - mPos;
        atn::Vector3 rot = d.GetNormalized_unk().ToRotation_unk();
        float a = fmod(57.295776f * (rot.y - mRot.y), 360.0);
        if (a > 180.0f) {
            a -= 360.0f;
        }
        if (a < -180.0f) {
            a += 360.0f;
        }
        float abs = atn::Abs_unk(a);
        if (abs > 0.034906585f) {
            a = 0.034906585f * (a / abs);
        }
        mRot.y += a;
    }
    frame = mMotion.mTracks[0].m08;
    if (frame >= 19.0f && frame <= 26.0f) {
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x672), m590, 30, -1, 0.0f);
    }
}

// inferred: comes down at place 0x262 (state 9) and then roars (bullet 0x673)
void Em403WarLion::fn_80328270() {
    if (m14C4 == 0) {
        if (mMotion.mTracks[0].m08 >= 6.0f) {
            m14C4 = 1;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x140, 0, 0, 1, 1.0f, 0.1f);
            CollResult_unk result;
            atn::Vector3 pos(-18.0f, 30.0f, 600.0f);
            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(0x262);
            if (place != 0) {
                pos = place->mPos;
            }
            mPos = pos;
            atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + pos;
            if (fn_8004ED34(lbl_8087EE98, &result, &pos, &to, 0x80000000, &mColli, 0) != 0) {
                mPos = result.m10;
            }
        }
    } else if (m14C4 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14C4 = 2;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
        } else if (mMotion.mTracks[0].m08 <= 10.0f) {
            fn_80144710();
            fn_800FAB80(lbl_8087F048, this, fn_80219E6C(0x673), m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            fn_80370AE4(lbl_8087F430, 0xCD, 1);
            SetState_unk(6);
        }
    }
}

// inferred: weapon manager (as in game/CharaTask.cpp, class unknown)
extern "C" void* lbl_8087F9E8;
extern "C" void fn_8059C32C(void* mgr, int arg);

// inferred: comes down at place 0x262 (state 10), roars and sends message 2 to the gimmick 40 of kind 1
void Em403WarLion::fn_8032853C() {
    if (m14C4 == 0) {
        if (mMotion.mTracks[0].m08 >= 6.0f) {
            m14C4 = 1;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x140, 0, 0, 1, 1.0f, 0.1f);
            CollResult_unk result;
            atn::Vector3 pos(-18.0f, 30.0f, 600.0f);
            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(0x262);
            if (place != 0) {
                pos = place->mPos;
            }
            mPos = pos;
            atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + pos;
            if (fn_8004ED34(lbl_8087EE98, &result, &pos, &to, 0x80000000, &mColli, 0) != 0) {
                mPos = result.m10;
            }
        }
    } else if (m14C4 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14C4 = 2;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
        } else if (mMotion.mTracks[0].m08 <= 10.0f) {
            fn_80144710();
            fn_800FAB80(lbl_8087F048, this, fn_80219E6C(0x673), m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            SetState_unk(6);
        } else {
            frame = mMotion.mTracks[0].m08;
            if (45.0f <= frame && frame <= 47.0f && fn_80370A78(lbl_8087F430, 0x5C) < 1) {
                fn_80370AE4(lbl_8087F430, 0x5C, 1);
                fn_8059C32C(lbl_8087F9E8, 0);
                for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
                    if (g->m50 == 40 && g->m4C == 1) {
                        GimmickMsg_unk msg(2);
                        g->vf40(&msg);
                        break;
                    }
                }
            }
        }
    }
}

void Em403WarLion::fn_803288C4() {
    if (m14C4 == 0) {
        if (mMotion.mTracks[0].m08 >= 40.0f) {
            m14C4 = 1;
            fn_800FAB80(lbl_8087F048, this, fn_80219E6C(0x675), m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            Field_unk* field = lbl_8087F430;
            field->m96C %= 2;
            field->m970 = 10;
            field->m974 = 4.0f;
            field->m978 = 10.0f;
        }
    } else if (m14C4 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            SetState_unk(6);
        }
    }
}

void Em403WarLion::fn_803289EC() {
    if (mMotion.mTracks[0].m08 >= 6.0f) {
        if (m15E0 == 0) {
            m15E0 = 1;
            fn_80370AE4(lbl_8087F430, 0x5A, 1);
        }
        SetState_unk(12);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 0x140, 0, 0, 1, 1.0f, 0.1f);
        mMotion.mTracks[0].m0C = 2.0f;
        CollResult_unk result;
        mPos = m14DC;
        atn::Vector3 down(0.0f, -100.0f, 0.0f);
        atn::Vector3 to = down + m14DC;
        if (fn_8004ED34(lbl_8087EE98, &result, &m14DC, &to, 0x80000000, &mColli, 0) != 0) {
            mPos = result.m10;
        }
        mRot = m14E8;
        fn_80239DAC(lbl_8087F3C0, this, 0x65, 1);
    }
}

void Em403WarLion::fn_80328B74() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m14CC == 1) {
            SetState_unk(6);
        } else {
            fn_80326E60(1);
        }
    }
}

// inferred: runs along the path m1524 (state 13), bullet 0x674 at frames 5-25 of the motion
void Em403WarLion::fn_80328C6C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
        return;
    }
    mPos = m1524.GetPos_unk((1.0f / 27.0f) * (float)m14C8);
    atn::Vector3 prev = m1524.GetPos_unk((1.0f / 27.0f) * (float)(m14C8 - 1));
    mRot = (mPos - prev).ToRotationZ_unk();
    frame = mMotion.mTracks[0].m08;
    if (frame >= 5.0f && frame <= 25.0f) {
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x674), m15F8, 30, -1, 0.0f);
    }
}

void Em403WarLion::fn_8032932C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
        return;
    }
    mPos += m150C;
    float f = mMotion.mTracks[0].m08;
    if (f >= 5.0f && f <= 25.0f) {
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x674), m590, 30, -1, 0.0f);
    }
}

// inferred: hit of fn_8004ECC0 with a second vector after it (stack layout of fn_8032943C; name invented)
struct CollHit_unk {
    atn::Vector3 mPos;
    atn::Vector3 m0C;
};

// NONMATCHING: 99.22% - orden de carga de m590/lbl_8087F048 en la llamada a fn_800F8C6C y de dos stfs del rayo
// inferred: turns towards m1518, then jumps to m1500 (state 15)
void Em403WarLion::fn_8032943C() {
    if (m14C4 == 0) {
        if (mMotion.mTracks[0].m08 >= 20.0f) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.5f;
            mMotion.fn_80097C08(0, 0x20, 1, 1, 1, 0.0f, 0.1f);
            mRot = m1518;
            m150C = (m1500 - mPos).GetNormalized_unk() * 100.0f;
            m150C *= 1.0f / mMotion.fn_80097D7C(0);
            m14C4 = 1;
        } else {
            float d = atn::WrapAngle_unk(m1518.y - mRot.y);
            d *= 0.1f;
            mRot.y += d;
        }
    } else {
        if ((mPos - m1500).Length_unk() < 10.0f) {
            SetState_unk(6);
        } else {
            mPos += m150C;
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x674), m590, 30, -1, 0.0f);
            CollHit_unk hit;
            atn::Vector3 from(mPos);
            atn::Vector3 to(mPos);
            from.y += 15.0f;
            to.y -= 15.0f;
            if (fn_8004ECC0(lbl_8087EE98, &hit.mPos, &from, &to, 0x80000000, 0, 0) != 0) {
                mPos.y = hit.mPos.y;
            }
        }
    }
}

// NONMATCHING: 99.29% - orden de los temporales de Lerp en la pila y stfs 0x2e8 del estado 18
void Em403WarLion::fn_80329744() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m1598 != 0) {
            m1598->fn_8015AF40((int)&m159C);
            SetState_unk(17);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
            m1598->fn_8015AF40((int)&m159C);
            m15CC = 0;
        } else {
            SetState_unk(6);
        }
    } else {
        float end = mMotion.fn_80097D7C(0) - 6.0f;
        float f = mMotion.mTracks[0].m08;
        if (f <= end) {
            float t = MIN(1.0f, (mMotion.mTracks[0].m08 - 22.0f) / (end - 22.0f));
            if (0.0f > t) {
                t = 0.0f;
            } else {
                t = MIN(1.0f, (mMotion.mTracks[0].m08 - 22.0f) / (end - 22.0f));
            }
            mPos = atn::Lerp_unk(m1574, m1580, t);
        } else if (m14C4 == 0) {
            m14C4 = 1;
        } else {
            mMotion.mTracks[0].m0C = 1.0f;
            if (m1598 != 0 && m1598->m55C == 6) {
                if (m1598->m560 == 0x6D) {
                    if (f < 50.0f) {
                        mMotion.mTracks[0].m0C = 0.02f;
                    }
                } else if (m1598->m560 == 10) {
                    SetState_unk(18);
                    mMotion.m34C = 1;
                    mMotion.mTracks[0].m20 = 1.0f;
                    mMotion.mTracks[0].m0C = 1.0f;
                    mMotion.fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
                    mMotion.mTracks[0].m0C = 0.7f;
                    m1598 = 0;
                }
            }
        }
    }
}

// NONMATCHING: 99.34% - el original guarda m0C una sola vez tras el ternario
void Em403WarLion::fn_80329A20() {
    CharaTask* held = m1598;
    if (held == 0 || held->m55C != 6 || held->m560 != 0x6E) {
        int down = 0;
        if (held->m55C == 6 && held->m560 == 10) {
            down = 1;
        }
        SetState_unk(18);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = down ? 0.7f : 1.0f;
        if (m1598->CanAct_unk() && !down) {
            m1598->fn_8015B238();
        }
        m1598 = 0;
    } else if (held->mMotion.mTracks[0].m00 == 0x21D && m14C8 > 90) {
        SetState_unk(19);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
        m1598->fn_8015B520();
        m1598 = 0;
    } else if (m15E8 == 0 && m14C8 == 10) {
        if (fn_80370A78(lbl_8087F430, 0x5B) < 1) {
            fn_80370AE4(lbl_8087F430, 0x5B, 1);
        }
        m15E8 = 1;
    }
}

void Em403WarLion::fn_80329C7C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
    }
}

void Em403WarLion::fn_80329CFC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
    }
}

void Em403WarLion::fn_80329D7C() {
    float frame;
    if (m15F4 > 5 || (frame = mMotion.mTracks[0].m08, frame >= mMotion.fn_80097D7C(0))) {
        m15EC = 450;
        m14CC = 0;
        m15DC = 1;
        m15F0 = 0;
        fn_80107A68(lbl_8087F048, &mMotion);
        SetState_unk(6);
    }
}

void Em403WarLion::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        vf9C(1);
        mFlag15 = 1;
        fn_800EE360(this);
    } else {
        m1434 = 0;
    }
}

// inferred: player manager (as in game/CharaTask.cpp)
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
// inferred: object hit by a ray (the character is at 0x0C, as in game/CharaTask.cpp)
struct CollObj_unk {
    unsigned char m00[0x08];
    unsigned int m08;
    CharaTask* m0C;
};
// inferred: the magic `data` of `chara` targets `target` (bullet manager); returns < 0 when not
extern "C" int fn_800FBA9C(Effect_unk* mgr, CharaTask* chara, CharaTask* target, HitInfo_unk* data, int arg,
                           float f);
extern "C" int fn_800FBE70(Effect_unk* mgr, CharaTask* chara, CharaTask* target, HitInfo_unk* data, int arg,
                           atn::Vector3* pos, int a, int b);

// NONMATCHING: 98.28% - asignacion de GPR (this en r27 en el original) y el id del control releido
// inferred: looks for a player that attacks the war lion (magic, arrows or the gun) and gives the
// place where to avoid it; counts the calls in `count` (true after 5)
int Em403WarLion::fn_80329ED8(int* count, atn::Vector3* pos, float* angle) {
    if (++*count > 5) {
        return 1;
    }
    int found = 0;
    float rot = 0.0f;
    atn::Vector3 target(0.0f, 0.0f, 0.0f);
    float dist = 40.0f;
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
        if (!c->IsActive_unk() || c->vf78() == 0) {
            continue;
        }
        if (c->m560 == 4 || c->m560 == 5) {
            if (fn_800FBA9C(lbl_8087F048, c, this, c->m638, -1, 0.0f) < 0) {
                continue;
            }
            found = 1;
            dist = 100.0f;
            target = c->mPos;
            rot = c->mRot.y;
            break;
        } else if (c->m560 == 30) {
            if (c->m638 == 0 || c->m638->m01 != 1) {
                continue;
            }
            atn::Vector3 dir = c->fn_80158E1C().GetNormalized_unk();
            atn::Vector3 from(c->mPos);
            from.y += 20.0f;
            atn::Vector3 to = from + dir * 1000.0f;
            CollResult_unk result;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0, &c->mColli, 0) == 0) {
                continue;
            }
            CollObj_unk* obj = (CollObj_unk*)result.m38;
            if ((obj->m08 & 2) != 2 || obj->m0C != this) {
                continue;
            }
            target = mPos;
            rot = dir.ToRotation_unk().y;
            found = 1;
            dist = 40.0f;
            break;
        } else if (c->m560 == 14) {
            atn::Vector3 dir = c->fn_80158E1C();
            if (fn_800FBE70(lbl_8087F048, c, this, c->m638, -1, &dir, 1, -1) < 0) {
                continue;
            }
            found = 1;
            target = dir;
            rot = c->mRot.y;
            break;
        } else {
            found = 1;
            target = c->mPos;
            rot = c->mRot.y;
            break;
        }
    }
    if (found) {
        fn_8032A3D4(pos, angle, &target, rot, dist);
        return 1;
    }
    return 0;
}

// NONMATCHING: 98.99% - registros r28/r29 de move/ofs y posicion de un stfs del prologo
// inferred: best place at distance `dist` on the left or the right of `target` (rotation `rot`):
// the farthest one the war lion can reach; `angle` faces back towards the target
void Em403WarLion::fn_8032A3D4(atn::Vector3* pos, float* angle, const atn::Vector3* target, float rot, float dist) {
    atn::Vector3 ofs(0.0f, 0.0f, dist);
    atn::Vector3 from(*target);
    float best = 0.0f;
    CollResult_unk result;
    atn::Vector3 found;
    from.y += m620;
    float a = mRot.y;
    for (int i = 0; i < 2; i++) {
        float r = 3.1415927f * (float)i + rot - 1.5707964f;
        Mtx m;
        PSMTXRotRad(m, 'y', r);
        atn::Vector3 move(ofs);
        PSMTXMultVec(m, &move, &move);
        fn_8004D388(lbl_8087EE98, &result, (CharaTask::Sphere_unk*)&from, &move, 0x80000000, &mColli, 0, m620);
        move = result.m10;
        atn::Vector3 d = move - from;
        float len = fn_805F9920(&d);
        if (len > best) {
            a = atn::WrapAngle_unk(3.1415927f + r);
            best = len;
            found = move;
        }
    }
    *pos = found;
    *angle = a;
}


// inferred: the point of the circuit with flags (a, b) farthest from the mean position of the players
atn::Vector3 Em403WarLion::fn_8032A650(int* id, int a, int b) {
    atn::Vector3 result(lbl_807C7030);
    atn::Vector3 mean(lbl_807C7030);
    int n = 1;
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = NextChara_unk(c)) {
        if (c->IsActive_unk()) {
            n++;
            mean += c->mPos;
        }
    }
    float inv = 1.0f / (float)n;
    mean.x *= inv;
    mean.y *= inv;
    mean.z *= inv;
    int found = 0;
    float best = 0.0f;
    for (int i = 0; i < 10; i++) {
        const WarLionPlace_unk* e = &lbl_80749B00[i];
        if (e->m08 == a && e->m04 == b) {
            RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(e->mPlace);
            if (place != 0) {
                atn::Vector3 d = place->mPos - mean;
                float len = fn_805F9920(&d);
                if (len > best) {
                    best = len;
                    result = place->mPos;
                    found = place->mId;
                }
            }
        }
    }
    if (id != 0) {
        *id = found;
    }
    return result;
}

// inferred: position of the point after `id` in the circuit (place 600 after the last one)
atn::Vector3 Em403WarLion::fn_8032A894(int id, int* next) {
    atn::Vector3 result(lbl_807C7030);
    int place = 0;
    for (int i = 0; i < 10; i++) {
        if (id == lbl_80749B00[i].mPlace) {
            if (i == 9) {
                place = 600;
            } else {
                place = lbl_80749B00[i + 1].mPlace;
            }
        }
    }
    RoutePlace_unk* p = fn_80121F00()->fn_8013C504()->FindPlace_unk(place);
    if (p != 0) {
        result = p->mPos;
    }
    if (next != 0) {
        *next = place;
    }
    return result;
}

// inferred: helpers of the path (vector of points and spline; weak copies at the end of this unit, also
// called by gmk/CharaPathMove.cpp, Event/Manager.cpp and game/LastCocoonQuadruped.cpp; C names)
struct WarLionPoints_unk {
    atn::Vector3* mData;
    unsigned int mSize;
    unsigned int mCap;
};
extern "C" void fn_8032AB90(WarLionPoints_unk* v);                          // inferred: constructor
extern "C" void fn_8032ABA4(WarLionPoints_unk* v, int flag);                // inferred: destructor
extern "C" void fn_8032AC1C(WarLionPoints_unk* v, const atn::Vector3* p);   // inferred: push_back
extern "C" void fn_8032AF90(WarLionPath_unk* path, WarLionPoints_unk* pts); // inferred: builds the path

// inferred: builds the path m1524 through the place `id` and the two next places of the circuit
int Em403WarLion::fn_8032A9F4(WarLionPath_unk* path, int id, int* next) {
    int second = 0;
    int first = 0;
    atn::Vector3 b(lbl_807C7030);
    atn::Vector3 c(lbl_807C7030);
    atn::Vector3 a(lbl_807C7030);
    for (int i = 0; i < 10; i++) {
        if (id == lbl_80749B00[i].mPlace) {
            int j = i + 1;
            if (j == 10) {
                j = 0;
            }
            first = lbl_80749B00[j].mPlace;
            j++;
            if (j == 10) {
                j = 0;
            }
            second = lbl_80749B00[j].mPlace;
            break;
        }
    }
    RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
    if (place != 0) {
        a = place->mPos;
    }
    place = fn_80121F00()->fn_8013C504()->FindPlace_unk(first);
    if (place != 0) {
        b = place->mPos;
    }
    place = fn_80121F00()->fn_8013C504()->FindPlace_unk(second);
    if (place != 0) {
        c = place->mPos;
    }
    WarLionPoints_unk pts;
    fn_8032AB90(&pts);
    fn_8032AC1C(&pts, &a);
    fn_8032AC1C(&pts, &b);
    fn_8032AC1C(&pts, &c);
    fn_8032AF90(path, &pts);
    if (next != 0) {
        *next = second;
    }
    fn_8032ABA4(&pts, -1);
    return 1;
}

// NONMATCHING: 99.55% - solo la posicion de su constante en .sdata2 (falta el codigo de ruta del final)
int Em403WarLion::vfE4() {
    if (m58C == 16 && mMotion.mTracks[0].m08 < 42.0f) {
        return 1;
    }
    return 0;
}

// NONMATCHING: 97.50% - solo la posicion de su constante en .sdata2 (falta el codigo de ruta del final)
float Em403WarLion::vf90() {
    return 3.6f;
}
