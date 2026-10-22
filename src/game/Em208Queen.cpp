#include "game/Em208Queen.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/EnemyManager.h"
#include "math/Vec.h"
#include "atn/ColliBase.h"
#include "macros.h"

// inferred: debug menu entries (as in game/Em101LittleZesya.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_801079C0(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion);
extern "C" void fn_80107A68(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" double acos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void fn_80105BD8(Effect_unk* mgr, Model_unk* model);
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in game/Em305Vampire.cpp)
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
struct Field_unk;
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
// inferred: damage description shown by fn_80108C10 (as StatusDamage_unk of game/CharaStatus.cpp)
// (fields as DamageResult_unk of game/Em002Skelton.cpp)
struct QueenDamage_unk {
    QueenDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
    }
    // inferred: kind and value (name invented)
    void Set_unk(int kind, int value) {
        m00 = kind;
        m04 = value;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C : 4;
    unsigned int m1C_rest : 28;
};
extern "C" void fn_80108C10(Effect_unk* effect, QueenDamage_unk* damage, const atn::Vector3& pos,
                            CharaTask* attacker, CharaTask* chara, int b, int c);
// inferred: collision world and its ray test (as in game/Em101LittleZesya.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: hit position
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28; // inferred: offset added to the hit position
    int m34;
    atn::ColliBase* m38; // inferred: object hit
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" void fn_8004D388(void* mgr, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                            unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
// inferred: frame rate (as in chr/ControlTurnQuadruped.cpp)
struct System_unk {
    unsigned char m00[0x30];
    int mFrameRate; // 0x30
};
extern "C" System_unk* lbl_8087F0A8;
// inferred: debug drawing (lbl_8087EEB0, as in gmk/Brazier.cpp)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063D3C(void* dbg, const atn::Vector3* pos, unsigned int color, float radius, float arg);
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
// inferred: camera and projection to the screen (as in MP/D2AMpMenuCockpit.cpp)
extern "C" void* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos);
// inferred: panes of a d2anime layout (as in include/MP/D2AMpMenu_unk.h)
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct QueenMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" QueenMsgTable_unk* lbl_8087F1E4;

// inferred: d2anime task made by fn_801F3FF8 (same layout as atn::D2A_Task; as MenuD2A_unk of
// game/MenuCommonTask.h); only the members used here
struct QueenD2A_unk : public atn::Task {
    bool IsEnd_unk() {
        float last = mLastFrame;
        return mFrame >= last;
    }
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xA0 - 0x58]; // 0x58
    float mLastFrame;                   // 0xA0
    unsigned char mA4[0xFC - 0xA4];
    unsigned int mLoop : 1; // 0xFC
    unsigned int mVisible : 1;
    unsigned int mPlaying : 1;
    unsigned int mRepeat : 1;
    unsigned int mFC_rest : 28;
    float mFrame; // 0x100
};
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop);

// inferred: skill effects (as in gmk/HealPoint.cpp); the callback is empty here
struct Bullet_unk;
extern "C" void* lbl_8087F9E8;
extern "C" Bullet_unk* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                   const atn::Vector3* pos, const atn::Vector3* rot,
                                   boost::function<void(Bullet_unk*)> callback, float rate);
extern "C" void fn_801031D0(Effect_unk* mgr, CharaTask* chara, CharaTask* other, int arg);
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: CharaTask::fn_80148B38 receives a distance in f1 (the shared header declares it without
// parameters; called by its symbol as in MP/D2AMpMenuCockpit.cpp)
extern "C" void fn_80148B38__9CharaTaskFv(CharaTask* chara, float dist);

// inferred: offset of the point hit by the cannon shots (same values as chr/ControlCannonLift.cpp)
static atn::Vector3 sShotOffset_unk(3.882f, 98.679f, 10.846f);

// inferred: object returned by GimmickCommon::vf50 (declared as int there); only the size is known
struct GimmickShape_unk {
    unsigned char m00[0x48];
    float m48; // inferred: radius
};

// inferred: route points of state 15 (lbl_80746DB0)
static const int sRoutePoints_unk[8] = {0x259, 0x25A, 0x25B, 0x25C, 0x25D, 0x25E, 0x25F, 0x260};

// inferred: RouteTable_unk::FindPlace_unk with the array read inside the loop (register order of the
// original)
inline RoutePlace_unk* FindRoutePlace2_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}
// inferred: as FindRoutePlace2_unk on the field table (name invented)
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
// inferred: shows the battle message `id` with the name of the queen (as in game/Em309Ghoul.cpp)
inline void ShowMessage_unk(Em208Queen* queen, wchar_t* text, int id) {
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(id), *(const wchar_t**)((char*)queen->m60 + 4));
    fn_80109828(lbl_8087F048, text);
}
// inferred: as AngleDiffDeg_unk of game/Em201ZanGurg.cpp
inline float AngleDiffDeg_unk(float a, float b) {
    float d = fmod(57.295776f * (a - b), 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}

// inferred: turns to the target on the ground plane (name invented)
inline void Em208Queen::FaceTarget_unk() {
    atn::Vector3 d(m14B0->mPos.x - mPos.x, 0.0f, m14B0->mPos.z - mPos.z);
    mRot = d.ToRotationZ_unk();
}

// inferred: names invented
inline int TenthHp_unk(CharaStatus* status) {
    return (int)(0.1f * status->mParamB.m84);
}
inline int HpLeft_unk(CharaStatus* status) {
    return (int)status->m04 - 1;
}
inline QueenD2A_unk* GetSlash_unk(Em208Queen* queen) {
    return (QueenD2A_unk*)queen->m1540;
}
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline void SetSlashText_unk(Em208Queen* queen, const char* name, const wchar_t* text) {
    SetPaneText_unk(GetSlash_unk(queen)->mLayout, name, text);
}
// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}
// inferred: the shots of the cannons (bullets 2000 and 2009); name invented
inline bool IsCannonShot_unk(HitMsg_unk* msg) {
    bool result = true;
    if (msg->mData->m04 != 2000 && msg->mData->m04 != 2009) {
        result = false;
    }
    return result;
}

Em208Queen::Em208Queen(atn::Task* parent, void* data)
    : EnemyTask(parent, data), m14B8(0), m14BC(0), m14C0(1), m14C4(0), m14C8(0), m14CC(0.0f), m14D0(0), m14D4(0),
      m14D8(0), m150C(0), m1510(-1), m1514(0), m1528(0), m152C(1), m1530(0.0f, 0.0f, 0.0f), m153C(1), m154C(0),
      m1550(0.2f), m1554(0.4f) {
    mFlag22 = 1;
    (m1540 = fn_801F3FF8(this, "ui_point_slash.d2a", 0))->fn_800D246C(1);
}

Em208Queen::~Em208Queen() {}

int Em208Queen::vf0C() {
    int result = 1;
    if (fn_8013655C() != 0 || (m1438 != 0 && (m1438->mFlags & 2) != 2) || fn_800D3FA4() == 0) {
        result = 0;
    }
    if (result != 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        void* group = m1548.Load("em208");
        fn_80087994(group, "DebugDisp", &m154C, 0, 0);
        fn_8008771C(group, "Hungry_Jump", &m1550, 0, 0, 0.0f, 1.0f, 0.1f);
        fn_8008771C(group, "Hungry_Tackle", &m1554, 0, 0, 0.0f, 1.0f, 0.1f);
        fn_8008771C(group, "NowHungryRate", &m14CC, 0, 0, 0.0f, 1.0f, 0.1f);
        m14C0 = 1;
        m14CC = 0.0f;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        m1540->fn_800D246C(0);
        m1540->mFlags |= 4;
        GetSlash_unk(this)->mRepeat = 0;
        mRoute.mA0 = 1;
        return 1;
    }
    return 0;
}

// inferred: starts state `state` (names invented)
inline void Em208Queen::SetState_unk(int state) {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}
inline void Em208Queen::PlayMotion_unk(int motion) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
}

// inferred: identity rotated by z, y and x (as ChaserRotZYX_unk of game/ChaserNpc.cpp; name invented)
inline void QueenRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// inferred: identity rotated around y (name invented)
inline void SetRotY_unk(atn::Matrix3x4* m, float rad) {
    m->SetIdentity();
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, 'y', rad);
    PSMTXConcat(m->m, r, t);
    __MtxCopy(m->m, t);
}

// inferred: name invented
inline void Em208Queen::CastSkill_unk() {
    HitInfo_unk* skill = fn_80219E6C(0x67D);
    atn::Vector3 pos(-450.0f, 0.0f, -1100.0f);
    RoutePlace_unk* place = FindRoutePlace2_unk(lbl_8087F430->m10D8, 0x25F);
    if (place != 0) {
        pos = place->mPos;
    }
    fn_8059B66C(lbl_8087F9E8, skill, this, this, &pos, &atn::Vector3(0.0f, 0.0f, 0.0f),
                boost::function<void(Bullet_unk*)>(), 1.0f);
}

// NONMATCHING: 99.86% - f7/f8 of pi and 0.0f in the "eat_point" rotation (state 13); tried the rotation
// as an inline, the angle in a local, operand order, SetIdentity by hand
void Em208Queen::vf10() {
    if (m58C == 15) {
        m56C = 0.16666667f;
    } else {
        m56C = 0.1f;
    }
    m14B4 = m14B0;
    m14B0 = mC58.mC4;
    if (mC58.mC0 == 0 || m14B0 == 0) {
        fn_8016E970(3);
    } else if (m14B0 != 0) {
        switch (m58C) {
        case 7:
            fn_802CA490();
            break;
        case 8: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                fn_80178A6C();
                SetState_unk(6);
            } else if (m14BC == 50) {
                CastSkill_unk();
            }
            break;
        }
        case 9:
            fn_802CA8C0();
            break;
        case 10:
            if (m14BC >= 120) {
                if (m14D0 != 0 && m14D0->mType != 2) {
                    fn_801031D0(lbl_8087F048, this, m14D0, 1);
                    int motion = m14D0->mMotion.mTracks[0].m00;
                    if (motion != 0x220 && motion != 0x221) {
                        SetState_unk(13);
                        PlayMotion_unk(0x147);
                    } else if (m14D4 == 0) {
                        SetState_unk(12);
                        PlayMotion_unk(0x148);
                    }
                } else {
                    SetState_unk(11);
                    PlayMotion_unk(0x147);
                }
            }
            break;
        case 11:
            fn_802CAFB8();
            break;
        case 12:
            fn_802CB1CC();
            break;
        case 13:
            fn_802CB43C();
            break;
        case 14:
            fn_802CB74C();
            break;
        case 15:
            fn_802CB8A0();
            break;
        case 2:
            vfEC();
            break;
        default:
            if (m55C != 6) {
                if (m55C == 7) {
                    if (m14B4 != m14B0) {
                        fn_8016E970(3);
                        fn_80170A20(m14B0, 0, -1.0f);
                    }
                } else {
                    fn_8016E970(3);
                    fn_80170A20(m14B0, 0, -1.0f);
                }
            }
            fn_802C9880();
            fn_802C9BF8();
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    if (m58C == 13) {
        SetRotY_unk(&m14DC, 3.1415927f + mRot.y);
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("eat_point"));
        m14DC.m[0][3] = pos.x;
        m14DC.m[1][3] = pos.y;
        m14DC.m[2][3] = pos.z;
    } else {
        __MtxCopy(m14DC.m, mMotion.GetNodeMtx_unk("eat_point"));
    }
    if (m58C - 12U <= 1 && m14BC >= 5 && m14D0 != 0) {
        m14D0->fn_80144710();
        fn_8000D430(&m14D0->mMotion, boost::function<void()>());
        fn_80148B38__9CharaTaskFv(m14D0, 40.0f);
    }
    m14BC++;
}

// NONMATCHING: 99.58% - r28/r29 of the text and the layout address in the three pane texts; tried an
// inline with the layout inside, text in an assignment, direct calls, a reference parameter
void Em208Queen::vf14() {
    fn_80149A30();
    if (mC58.mC0 != 0) {
        PlayerTask* player = lbl_8087F8A0->mHead;
        if (m58C == 10 && m14D0 != player) {
            if (m1540->IsFlag_unk(4)) {
                m1540->mFlags &= ~4;
                GetSlash_unk(this)->mFrame = 0.0f;
                GetSlash_unk(this)->mPlaying = 1;
            }
            if (m1544 == 0) {
                GetSlash_unk(this)->mFrame = 2.0f;
            }
            if (GetSlash_unk(this)->IsEnd_unk()) {
                GetSlash_unk(this)->mFrame = 5.0f;
            }
            MtxPtr head = mMotion.GetNodeMtx_unk("head");
            atn::Vector3 pos = GetNodePos_unk(head);
            pos.y += 2.0f;
            atn::Vector3 screen;
            fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
            SetPaneValue_unk(GetSlash_unk(this)->mLayout, "base_point", 0, screen.x);
            SetPaneValue_unk(GetSlash_unk(this)->mLayout, "base_point", 1, screen.y);
            const wchar_t* text = lbl_8087F1E4->Get_unk(0x2DB);
            if (text != 0) {
                SetPaneText_unk(GetSlash_unk(this)->mLayout, "string0001(1)", text);
                SetPaneText_unk(GetSlash_unk(this)->mLayout, "string0005(1)", text);
                SetPaneText_unk(GetSlash_unk(this)->mLayout, "string0006(1)", text);
            }
        } else if (!m1540->IsFlag_unk(4)) {
            m1540->mFlags |= 4;
        }
    }
    m1544 = 0;
}

void Em208Queen::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->mAttacker == this) {
        msg->m68 = 0;
        msg->m84 = 2;
        msg->m88 = -1;
        msg->m90 = 0;
        msg->m8C = 0;
        return;
    }
    msg->m10 *= 0.0f;
    if (IsCannonShot_unk(msg) && *(int*)&msg->m3C[4] == 1 && m58C == 10) {
        msg->m0C |= 0x880;
    }
    if (m58C == 14) {
        msg->m0C |= 0x80;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em208Queen::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    float limit;
    float before = (mStatus.m04 + (float)msg->m68) / (float)mStatus.mParamB.m84;
    float now = mStatus.m04 / (float)mStatus.mParamB.m84;
    switch (m14C4) {
    case 0:
        limit = 0.7f;
        break;
    case 1:
        limit = 0.4f;
        break;
    default:
        limit = 0.0f;
        break;
    }
    if (now <= limit && limit < before) {
        m14C8 = 1;
        m14C4++;
    }
    if (IsCannonShot_unk(msg) && *(int*)&msg->m3C[4] == 1 && m58C == 10) {
        fn_802CC39C();
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802CC708();
    }
}

void Em208Queen::vf50() {
    Item14_unk item;
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += item.m04.mRadius;
    m624.push_back(item);
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    item.m04.mRadius = 15.0f;
    m624.push_back(item);
    mFlagB29 = 0;
}

void Em208Queen::vf54() {
    if (m624.mSize != 0) {
        m624.mData[0].m04.mCenter = mColli.m3C;
        m624.mData[0].m04.mRadius = mColli.m54;
        m624.mData[1].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    }
}

// NONMATCHING: 99.81% - r3/r4 of the copies of the midpoint and the neck position (__VecCopy family)
void Em208Queen::vf20() {
    m620 = m5B0;
    atn::Vector3 down(0.0f, 0.0f, -10.0f);
    atn::Matrix3x4 m;
    QueenRotZYX_unk(m, mRot);
    PSMTXMultVec(m.m, &down, &down);
    m614 = mPos + m5A4 + down;
    m614.y += m620;
    MtxPtr left = mMotion.GetNodeMtx_unk("leftleg");
    MtxPtr right = mMotion.GetNodeMtx_unk("rightleg");
    atn::Vector3 mid;
    mid = (GetNodePos_unk(left) + GetNodePos_unk(right)) * 0.5f;
    atn::Vector3 head;
    head = GetNodePos_unk(mMotion.GetNodeMtx_unk("neck"));
    mColli.m3C = mid;
    mColli.m48 = head;
    mColli.m54 = 2.0f * m620;
    mColli.mFlags &= ~0x80000000;
}

void Em208Queen::vf24() {
    if (m55C == 6 &&
        (m560 == 4 || m560 == 0x16 || m560 == 0x17 || m560 == 0x3B || m560 == 0x3F || m560 == 0x27)) {
        return;
    }
    if (m570 < 0.01f) {
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        return;
    }
    if (mMotion.mTracks[0].m00 == 0x20) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.mTracks[0].m08 = 0.0f;
        }
    }
    mMotion.fn_80097C08(0, 0x20, 1, 0, 1, 0.0f, 0.1f);
    if (m58C == 15) {
        mMotion.mTracks[0].m0C = 3.5f;
    } else {
        mMotion.mTracks[0].m0C = 1.5f;
    }
}

void Em208Queen::fn_802C9880() {
    float speed = 0.0f;
    float scale = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 dir(mRoute.m58);
        if (m58C == 15) {
            scale *= 3.5f;
            speed = dir.Length_unk();
        } else {
            scale *= 1.5f;
            atn::Vector3 d = m14B0->mPos - mPos;
            if (d.Length_unk() < 35.0f) {
                speed = 0.0f;
            } else {
                speed = dir.Length_unk();
            }
        }
        if (speed > 0.0001f) {
            rot = dir.GetNormalized_unk().ToRotation_unk();
        } else {
            rot = mRot;
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(&rot, 0, speed, m568 * scale);
}

void Em208Queen::fn_802C9BF8() {
    atn::Vector3 target = m14B0->mPos;
    atn::Vector3 d = target - mPos;
    float dist = d.Length_unk();
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    if (m14C8 != 0) {
        ShowMessage_unk(this, text, 0xF1);
        if (m14C4 == 1) {
            ShowMessage_unk(this, text, 0xF3);
        } else {
            ShowMessage_unk(this, text, 0xF5);
        }
        SetState_unk(8);
        PlayMotion_unk(0x142);
        m14C8 = 0;
        return;
    }
    if (m14B0 == 0) {
        return;
    }
    float ry = d.GetNormalized_unk().ToRotation_unk().y;
    AngleDiffDeg_unk(ry, mRot.y);
    if (m14C0 == 0) {
        bool jump = true;
        if (m1528 >= m152C) {
            jump = false;
            m1528 = 0;
        }
        if (dist < 35.0f || 100.0f < dist) {
            jump = false;
        }
        if (jump) {
            ShowMessage_unk(this, text, 0xF7);
            fn_802CC19C(0, &atn::Vector3(0.0f, 0.0f, 0.0f));
            m14CC -= m1550;
            m1528++;
        } else {
            float rate = mStatus.m04 / (float)mStatus.mParamB.m84;
            if (rate < 0.4f) {
                m150C = 2;
            } else if (rate < 0.7f) {
                m150C = 2;
            } else {
                m150C = 1;
            }
            ShowMessage_unk(this, text, 0xF9);
            fn_802CC4C8(6, 0);
            m14CC -= m1554;
        }
    } else if (m14C0 == 1) {
        if (m14CC >= 1.0f) {
            m14C0 = 0;
            ShowMessage_unk(this, text, 0xFB);
        }
        if (m14BC > 45 && dist < 35.0f) {
            ShowMessage_unk(this, text, 0xFD);
            SetState_unk(9);
            PlayMotion_unk(0x145);
            FaceTarget_unk();
            fn_80370AE4(lbl_8087F430, 0x5D, 2);
        }
    }
}

void Em208Queen::fn_802CA490() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m14CC <= 0.0f) {
            m14C0 = 1;
            wchar_t text[256];
            memset(text, 0, sizeof(text));
            ShowMessage_unk(this, text, 0xFF);
        }
        SetState_unk(6);
        return;
    }
    if (mMotion.mTracks[0].m08 > 36.0f && mMotion.mTracks[0].m08 <= 56.0f) {
        if (m153C != 0) {
            atn::Vector3 move = m1530;
            int fps = lbl_8087F0A8->mFrameRate;
            move.y += -98.1f / (float)(fps * fps);
            CollResult_unk result;
            CharaTask::Sphere_unk sphere = fn_80176548(mPos);
            fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x20000000, &mColli, 0, sphere.mRadius);
            mPos = result.m10;
            mPos -= m5A4;
            mPos.y -= sphere.mRadius;
        } else {
            mPos += m1530;
        }
    }
    if (mMotion.mTracks[0].m08 >= 56.0f && mMotion.mTracks[0].m08 <= 59.0f) {
        HitInfo_unk* bullet = fn_80219E6C(0x67C);
        atn::Vector3 pos = mPos;
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, bullet, m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
    if (mMotion.mTracks[0].m08 >= 59.0f && mMotion.mTracks[0].m08 <= 65.0f) {
        HitInfo_unk* bullet = fn_80219E6C(0x67C);
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightfoot"));
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, bullet, m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
    if (mMotion.mTracks[0].m08 >= 36.0f && mMotion.mTracks[0].m08 <= 56.0f) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("center_chara"));
        fn_802CC834(pos, 50.0f, 6.2831855f, 100);
    }
    if (m154C != 0) {
        fn_80063D3C(lbl_8087EEB0, &mPos, 0xFFFF0000, fn_80219E6C(0x67C)->m58, 0.0f);
    }
}

// inferred: names invented
inline atn::Vector3 Em208Queen::GetFrontPoint_unk() {
    Mtx m;
    PSMTXRotRad(m, 'y', mRot.y);
    atn::Vector3 front = atn::Vector3(0.0f, 15.0f, 40.0f);
    PSMTXMultVec(m, &front, &front);
    return mPos + front;
}
inline atn::Vector3 Em208Queen::GetShotPoint_unk() {
    Mtx m;
    PSMTXRotRad(m, 'y', mRot.y);
    atn::Vector3 offset = sShotOffset_unk;
    PSMTXMultVec(m, &offset, &offset);
    return mPos + offset;
}

// NONMATCHING: 97.24% - stack slots of the front point and order of the stores of m14D0/m14D4; operands
// of the 1.25f product (6 variants: front point inline, center declared first, assignment)
void Em208Queen::fn_802CA8C0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m14D0 == 0 && m14D8 == 0) {
            SetState_unk(6);
        } else {
            m14CC += 0.3f;
            SetState_unk(10);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
        }
        return;
    }
    if (m14BC == 47) {
        fn_80144710();
        m14D0 = 0;
        m14D4 = 0;
        m14D8 = 0;
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        atn::Vector3 front;
        front = atn::Vector3(0.0f, 15.0f, 40.0f);
        PSMTXMultVec(m, &front, &front);
        atn::Vector3 center = mPos + front;
        bool held = false;
        float best = 1e16f;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = (PlayerTask*)p->mNext) {
            if (p->CanAct_unk()) {
                if (p->fn_80155D70()) {
                    held = true;
                } else {
                    atn::Vector3 d = p->mPos - center;
                    float dist = fn_805F9920(&d);
                    if (dist < 1225.0f && dist * 1.25f < best) {
                        m14D0 = p;
                        best = dist * 1.25f;
                    }
                }
            }
        }
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            if (e->CanAct_unk()) {
                atn::Vector3 d = e->mPos - center;
                float dist = fn_805F9920(&d);
                if (dist < 1225.0f && dist < best) {
                    m14D0 = e;
                    best = dist;
                }
            }
        }
        float best2 = 1e16f;
        for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
            if (g->m50 == 7 && g->m54 == 1) {
                atn::Vector3 d = g->m6C - center;
                float dist = d.x * d.x + d.z * d.z;
                if (dist < 1225.0f && dist < best2) {
                    m14D8 = (int)g;
                    best2 = dist;
                }
            }
        }
        if (m14D8 != 0 && (best2 < best || held)) {
            m14D0 = 0;
            Mtx r;
            PSMTXRotRad(r, 'y', mRot.y);
            atn::Vector3 offset = sShotOffset_unk;
            PSMTXMultVec(r, &offset, &offset);
            atn::Vector3 pos = mPos + offset;
            pos.y -= 10.0f;
            GimmickMsg_unk msg(6);
            msg.mVec = pos;
            msg.m10 = (int)lbl_8087F8A0->mHead;
            msg.m04 = 0x76;
            ((GimmickCommon*)m14D8)->vf40(&msg);
        } else if (m14D0 != 0) {
            Mtx r;
            PSMTXRotRad(r, 'y', mRot.y);
            atn::Vector3 offset = sShotOffset_unk;
            PSMTXMultVec(r, &offset, &offset);
            atn::Vector3 pos = mPos + offset;
            m14D0->m58C = 0;
            if (m14D0->mType == 0) {
                m14D0->fn_8015B808(pos, 0x94);
            } else {
                m14D0->fn_8015B808(pos, 0xDE);
            }
            if (m14D0->m1208 != 0) {
                m14D4 = 1;
            }
        }
        fn_802CC834(center, 30.0f, 6.2831855f, 100);
    }
    if (m154C != 0) {
        atn::Vector3 center = GetFrontPoint_unk();
        fn_80063D3C(lbl_8087EEB0, &center, 0xFFFF0000, 35.0f, 0.0f);
    }
}

// NONMATCHING: 99.92% - r0/r5 of the bit field of the damage record (kind/value set through Set_unk)
void Em208Queen::fn_802CAFB8() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
    } else if (m14BC == 5 && m14D0 != 0) {
        if (m14D0->mType == 2) {
            QueenDamage_unk damage;
            damage.Set_unk(1, (int)(0.2f * -m14D0->mStatus.m04));
            atn::Vector3 pos = fn_80148B0C(0)->m04.mCenter;
            pos += atn::Vector3(0.0f, 15.0f, 0.0f);
            mStatus.fn_8012DF7C(damage.m04, 0, 0);
            fn_80108C10(lbl_8087F048, &damage, pos, this, m14D0, 0, 0);
            fn_80105BD8(lbl_8087F048, &m14D0->mMotion);
            m14D0->vf9C(1);
            m14D0->vf44();
            m14D0->mFlag15 = 1;
            m14D0->fn_801765D8();
            m14CC += 1.0f;
        }
        m14CC = m14CC < 1.0f ? m14CC : 1.0f;
        m14D0 = 0;
    }
}

void Em208Queen::fn_802CB1CC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
    } else if (m14BC == 5) {
        if (m14D0 != 0) {
            if (m14D0->mType != 2 && m14D0->m1208 == 0) {
                m14D0->fn_8015BAF0((int)&m14DC, 300);
                m14CC += 0.5f;
            }
            m14CC = m14CC < 1.0f ? m14CC : 1.0f;
        }
    } else if (m14BC == 26 && m14D0 != 0) {
        atn::Vector3 dir(1.0f, -3.0f, 1.0f);
        PSVECNormalize(&dir, &dir);
        dir *= 20.0f;
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &dir, &dir);
        atn::Vector3 from = mPos;
        from.y += 3.0f;
        atn::Vector3 to = m14D0->mPos;
        to.y += 3.0f;
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0 &&
            (result.m38->mFlags & 4) == 4) {
            result.m10 += result.m28;
            m14D0->mPos = result.m10;
        }
        m14D0->vf44();
        m14D0->fn_8015E7A0(dir, -1, 0);
        m14D0 = 0;
    }
}

// NONMATCHING: 97.60% - registers r0/r4 of the int conversions of MIN/MAX and the stack of the offset
void Em208Queen::fn_802CB43C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(6);
    } else if (m14BC == 5) {
        if (m14D0 != 0) {
            atn::Vector3 dir(0.0f, 0.0f, 3.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            m14D0->fn_8015BDD8((int)&m14DC, dir);
            m14CC += 0.5f;
            CharaStatus* status = &m14D0->mStatus;
            int hp = (int)status->m04;
            int damage = MAX(0, MIN(TenthHp_unk(status), hp - 1));
            status->fn_8012DF7C(damage, 0, 0);
            atn::Vector3 pos = m14D0->fn_80148B0C(0)->m04.mCenter;
            Mtx r;
            PSMTXRotRad(r, 'y', m14D0->mRot.y);
            atn::Vector3 offset;
            offset = atn::Vector3(0.0f, 15.0f, 10.0f);
            PSMTXMultVec(r, &offset, &offset);
            pos += offset;
            QueenDamage_unk info;
            info.Set_unk(1, damage);
            fn_80108C10(lbl_8087F048, &info, pos, this, m14D0, 0, 0);
        }
        m14CC = m14CC < 1.0f ? m14CC : 1.0f;
    } else if (m14BC == 12) {
        m14D0 = 0;
    }
}

void Em208Queen::fn_802CB74C() {
    if (m14B8 == 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14B8 = 1;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14B, 1, 0, 1, 0.0f, 0.1f);
        }
    } else if (m14B8 == 1) {
        if (m14BC >= 180) {
            m14B8 = 2;
            PlayMotion_unk(0x14C);
            fn_80107A68(lbl_8087F048, &mMotion);
        }
    } else if (m14B8 == 2) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            SetState_unk(6);
        }
    }
}

void Em208Queen::fn_802CB8A0() {
    atn::Vector3 rot = mRot;
    atn::Vector3 d = m151C - mPos;
    float speed = d.Length_unk();
    float scale = 3.5f;
    if (speed > 0.0001f) {
        rot = d.GetNormalized_unk().ToRotation_unk();
    } else {
        rot = mRot;
    }
    fn_8013CB68(&rot, 0, speed, m568 * scale);
    if (m570 > 0.05f) {
        HitInfo_unk* bullet = fn_80219E6C(0x67E);
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, bullet, m590, &mPos, &mRot, 0, 60, -1, -1, 0.0f, 1.0f);
    }
    fn_802CC834(mPos, 35.0f, 6.2831855f, 100);
    atn::Vector3 left = mPos - m151C;
    if (fn_805F9920(&left) < 1225.0f) {
        if (--m150C <= 0) {
            if (m14CC <= 0.0f) {
                m14C0 = 1;
                wchar_t text[256];
                memset(text, 0, sizeof(text));
                ShowMessage_unk(this, text, 0xFF);
                for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = (PlayerTask*)p->mNext) {
                    fn_801031D0(lbl_8087F048, this, p, 1);
                }
            }
            switch (m1518) {
            case 6:
                SetState_unk(6);
                return;
            case 7:
                fn_802CC19C(0, &atn::Vector3(0.0f, 0.0f, 0.0f));
                return;
            case 9:
                SetState_unk(9);
                PlayMotion_unk(0x145);
                FaceTarget_unk();
                fn_80370AE4(lbl_8087F430, 0x5D, 2);
                return;
            case 8:
                SetState_unk(8);
                PlayMotion_unk(0x142);
                return;
            }
        } else {
            fn_802CC4C8(6, 0);
        }
        m1514 = 0;
    } else {
        atn::Vector3 from = mPos;
        from.y += 10.0f;
        atn::Vector3 to = m151C;
        to.y += 10.0f;
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x20000000, 0, 0) != 0) {
            m14C0 = 1;
            fn_802CC19C(1, &atn::Vector3(m151C));
            m14CC -= m1550;
        }
    }
}

void Em208Queen::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        if (m1434 > 0) {
            m1434--;
        } else {
            mColli.mFlags &= ~1;
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 15;
    }
}

// NONMATCHING: 97.29% - r3/r4 of the vector copies and the order of the 0.05f products (__VecCopy family)
void Em208Queen::fn_802CC19C(int arg, const atn::Vector3* pos) {
    SetState_unk(7);
    PlayMotion_unk(0x13F);
    atn::Vector3 dest = m14B0->mPos;
    if (arg != 0) {
        m153C = 0;
        dest = *pos;
    } else {
        m153C = 1;
        Mtx m;
        PSMTXRotRad(m, 'y', m14B0->mRot.y);
        atn::Vector3 front;
        front = atn::Vector3(0.0f, 0.0f, 10.0f);
        PSMTXMultVec(m, &front, &front);
        dest += front;
        atn::Vector3 from = dest;
        atn::Vector3 to = dest;
        from.y += 10.0f;
        to.y -= 1000.0f;
        fn_8004ECC0(lbl_8087EE98, &dest, &from, &to, 0x20000000, &mColli, 0);
    }
    m1530 = atn::Vector3(dest.x - mPos.x, dest.y - mPos.y, dest.z - mPos.z) * 0.05f;
}

void Em208Queen::fn_802CC39C() {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 14;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    m14CC = 1.0f;
    if (m14D0 != 0) {
        if (m14D0->m55C == 6 && m14D0->m560 - 0x71U <= 1) {
            m14D0->m574 = atn::Vector3(0.0f, -0.1f, 0.0f);
            m14D0->vf8C(lbl_807C7030);
            m14D0->fn_8015495C(0, 1, 1, 1);
        }
        m14D0 = 0;
    }
    fn_801079C0(lbl_8087F048, &mMotion, &mMotion);
}

inline int Em208Queen::CollectPoints_unk(int* ids) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        RoutePlace_unk* place = FindRoutePlace2_unk(lbl_8087F430->m10D8, sRoutePoints_unk[i]);
        if (place != 0) {
            atn::Vector3 d = place->mPos - mPos;
            if (fn_805F9920(&d) > 28900.0f) {
                ids[count++] = place->mId;
            }
        }
    }
    return count;
}

// inferred: the second route search reads the field table inside the helper (FindRoutePlace3_unk)
void Em208Queen::fn_802CC4C8(int arg, CharaTask* target) {
    SetState_unk(15);
    m1518 = arg;
    if (target != 0) {
        m1510 = -1;
        m151C = target->mPos;
        return;
    }
    int ids[8];
    int count = CollectPoints_unk(ids);
    int id = count > 0 ? ids[rand() % count] : -1;
    m1510 = id;
    m1514 = 0;
    RoutePlace_unk* place = FindRoutePlace3_unk(id);
    if (place == 0) {
        SetState_unk(6);
    } else {
        m151C = place->mPos;
    }
}

void Em208Queen::fn_802CC708() {
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    vf9C(1);
    if (m14D0 != 0) {
        m14D0->m574 = atn::Vector3(0.0f, -0.1f, 0.0f);
        m14D0->vf8C(lbl_807C7030);
        m14D0->fn_8015495C(0, 1, 1, 1);
        m14D0 = 0;
    }
    fn_800EB7A0(this);
}

void Em208Queen::fn_802CC834(atn::Vector3 pos, float radius, float angle, int arg) {
    for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
        if (g->m50 == 1) {
            atn::Vector3 target = g->vf54();
            atn::Vector3 d = target - mPos;
            d.y = 0.0f;
            float dist = PSVECMag(&d);
            GimmickShape_unk* shape = (GimmickShape_unk*)g->vf50();
            if (shape != 0) {
                dist -= shape->m48;
            }
            if (dist <= radius) {
                PSVECNormalize(&d, &d);
                atn::Vector3 front(0.0f, 0.0f, 1.0f);
                GimmickHitMsg_unk msg;
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &front, &front);
                front.y = 0.0f;
                if ((float)acos(PSVECDotProduct(&d, &front)) <= 0.5f * angle) {
                    memset(&msg, 0, sizeof(msg));
                    msg.mType = arg;
                    g->vf44(&msg);
                }
            }
        }
    }
}

// inferred: nonzero while the queen can be hit by the cannons (called by chr/ControlCannonLift.cpp)
extern "C" int fn_802CCA0C(Em208Queen* queen) {
    return queen->CanAct_unk() && queen->m58C == 10;
}

atn::Vector3 Em208Queen::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = m624.mData[0].m04.mCenter;
        result.y = mPos.y;
    } else {
        result = m624.mData[0].m04.mCenter;
    }
    return result;
}

