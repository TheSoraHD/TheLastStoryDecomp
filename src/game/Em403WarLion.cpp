#include "game/Em403WarLion.h"
#include "atn/Memory.h"
#include "game/FriendManager.h"
#include "gmk/Lemon.h"
#include "math/Vec.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: effect/bullet manager of the characters (class unknown)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80107A68(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_801079C0(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion);
// inferred: settings (lbl_8087F0A8) and debug drawing (lbl_8087EEB0), as in game/CopyRobotTask.cpp
struct Settings_unk {
    unsigned char m00[0x34];
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
};
extern "C" Field_unk* lbl_8087F430;
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

extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" int rand();
// inferred: translation matrix by value (name invented)
inline atn::Matrix3x4 TransMtx_unk(float x, float y, float z) {
    atn::Matrix3x4 m;
    atn::Vector3 v;
    v.Set_unk(x, y, z);
    PSMTXTrans(m.m, v.x, v.y, v.z);
    return m;
}

// NONMATCHING: draft
void Em403WarLion::vf10() {
    m14C8++;
    m14BC = mC58.mC4;
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    int raging = IsRaging_unk();
    if (!HasTarget_unk()) {
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
                    m14E8 = (fn_8032A894(m14FC, 0) - m14DC).ToRotation_unk();
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
    float max = atn::DegToRad_unk(50.0f);
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
    m159C = TransMtx_unk(0.0f, 0.0f, 10.0f);
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

int Em403WarLion::vfE4() {
    if (m58C == 16 && mMotion.mTracks[0].m08 < 42.0f) {
        return 1;
    }
    return 0;
}

float Em403WarLion::vf90() {
    return 3.6f;
}
