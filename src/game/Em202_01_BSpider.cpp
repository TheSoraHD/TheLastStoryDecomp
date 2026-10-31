#include "game/Em202_01_BSpider.h"
#include "atn/Material_unk.h"
#include "math/Vec.h"
#include "game/FriendManager.h"
#include "game/CharaStatus.h"

// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/Em306GurgBeast.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;

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
extern "C" int rand();
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
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)

// inferred: route points the spider escapes to
static const int sSpotIds_unk[6] = {0xB2, 0x7A, 0x7F, 0x6D, 0x7D, 0x70};
// inferred: sounds of the spider (same table as game/Em202Spider.cpp)
static const char* const sSe_unk[3] = {"SE_EM202_008", "SE_EM202_002", "SE_DG001_014"};

// inferred: spots taken by the spiders (shared by all of them) and their positions
static int sSpotUsed_unk[6];
static atn::Vector3 sSpotPos_unk[6];

// inferred: color of the spiders when they are down (game/Em202SpiderBase.cpp)
extern "C" SpiderBaseColor_unk lbl_807C8398;

// inferred: RouteTable_unk lookup of a point by id, with the array read inside the loop (name invented)
inline RoutePoint_unk* FindRoutePoint_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->m98; i++) {
        RoutePoint_unk* points = table->mPoints;
        if (id == points[i].m00) {
            return &points[i];
        }
    }
    return 0;
}

// NONMATCHING: 99.29% - registers r7-r11 of the inlined point lookup (12 variants of the lookup)
Em202_01_BSpider::Em202_01_BSpider(atn::Task* parent, void* data)
    : EnemyTask(parent, data), m14B0(0), m14B4(0), m14B8(0), m14BC(0), m14C0(0), m14C8(0), m14D8(0), m14DC(0),
      m14E0(0.0f), m14E4(0.0f), m14EC(0) {
    for (int i = 0; i < 6; i++) {
        RoutePoint_unk* point = FindRoutePoint_unk(lbl_8087F430->m10D8, sSpotIds_unk[i]);
        sSpotPos_unk[i] = point->mPos;
    }
    m568 = 0.5f;
    m55C = 3;
    for (int i = 0; i < 6; i++) {
        sSpotUsed_unk[i] = 0;
    }
    m1500.LoadRequest("/effect/ef_sk002b.eff");
    m150C.LoadRequest("/effect/ef_uc001.efp");
    m1518.LoadRequest("/effect/ef_sk002d.efp");
    m1524.LoadRequest("/effect/ef_uc007.efp");
    m1530.LoadRequest("/effect/ef_sc003_01b.efp");
}

Em202_01_BSpider::~Em202_01_BSpider() {}

// NONMATCHING: 89.34% - the original interleaves the flag ors with the loads of m500-m508 (10 variants)
int Em202_01_BSpider::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1500.IsLoading() == 0 &&
        m150C.IsLoading() == 0 && m1518.IsLoading() == 0 && m1524.IsLoading() == 0 &&
        m1530.IsLoading() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC218;
        mStatus.m18 |= 0x3800000;
        m500 *= 4.0f;
        m504 *= 4.0f;
        m508 *= 4.0f;
        m14C8 = 2;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// inferred: back to state 0 (names invented, as in game/Em202Spider.cpp)
inline void ResetState_unk(Em202_01_BSpider* self) {
    self->m14B4 = 0;
    self->m590 = fn_800F8548(lbl_8087F048);
    self->m58C = 0;
    self->fn_8016E970(3);
    fn_800CB5C8(&self->m153C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, self, 200, 0);
}

// NONMATCHING: 99.62% - r4/r5 of the target and of the copy of its position in the default case (8 variants)
void Em202_01_BSpider::vf10() {
    mC58.mC8 = (int)mC58.mC4; // inferred: previous target (mC8 is an int in the shared header)
    int state = m58C;
    if (state - 10U <= 2) {
        m1454 = 0;
    } else {
        m1454 = 1;
        m14B0 = mC58.mC4;
    }
    mC58.mC4 = m14B0;
    m14B4++;
    if (mC58.mC0 == 0 || m14B0 == 0) {
        fn_8016E970(3);
        if (m58C == 2) {
            vfEC();
        } else {
            fn_80270B8C();
        }
    } else {
        switch (m58C) {
        case 6: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk(this);
            } else {
                float f = mMotion.mTracks[0].m08;
                if (f >= 17.0f && f <= 22.0f) {
                    fn_80144710();
                    int id = m590;
                    HitInfo_unk* skill = fn_80219E6C(0x3F3);
                    fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, id, 30, -1, 0.0f);
                }
            }
            break;
        }
        case 8: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk(this);
            } else {
                float f = mMotion.mTracks[0].m08;
                if (f >= 12.0f && f <= 17.0f) {
                    int id = m590;
                    HitInfo_unk* skill = fn_80219E6C(0x3F5);
                    fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, id, 30, -1, 0.0f);
                }
            }
            break;
        }
        case 10:
            fn_80271104();
            break;
        case 12:
            if (mRoute.m2C != 0) {
                m568 = 0.5f;
                fn_80271DAC(m14B0);
            } else {
                fn_80270B8C();
            }
            break;
        case 2:
            vfEC();
            break;
        case 11:
            fn_802714B0();
            break;
        case 13: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk(this);
            }
            break;
        }
        case 14:
            if (m14B4 >= 450) {
                ResetState_unk(this);
            }
            break;
        default:
            CharaTask* target = m14B0;
            if (m55C != 6 && target != 0) {
                float dist = (atn::Vector3(target->mPos) - mPos).Length_unk();
                if (m14C8 >= 2) {
                    m14C8 = 0;
                    fn_802721A0();
                } else if (dist < 31.0f) {
                    switch (rand() % 8 + 1) {
                    case 6:
                    case 7:
                        fn_8027179C(m14B0);
                        break;
                    case 8:
                        fn_80271AA4(m14B0);
                        break;
                    }
                }
            }
            if (m58C == 0) {
                if (m55C != 6) {
                    if (m55C == 7) {
                        if (m14B0 != mRoute.mTarget) {
                            if (fn_8027232C(m14B0, 0) != 0) {
                                break;
                            }
                            fn_8016E970(3);
                        }
                    } else {
                        if (fn_8027232C(m14B0, 0) != 0) {
                            break;
                        }
                        m55C = 3;
                        fn_80170A20(m14B0, 0, -1.0f);
                    }
                }
                fn_80270B8C();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
}

// NONMATCHING: 93.04% - same code and same cause as Em202SpiderBase::vf14 (stack slots of the color
// temporaries)
void Em202_01_BSpider::vf14() {
    SpiderBaseColor_unk target;
    if (IsDown_unk()) {
        target = lbl_807C8398;
    }
    m14F0 = (target - m14F0) * 0.15f + m14F0;
    mMotion.fn_80092954("em202_body")->mColor = m14F0.ToGX();
    mMotion.fn_80092954("em202_tail")->mColor = m14F0.ToGX();
    fn_80149A30();
}

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

// inferred: the spider guards the hits from the front (from the back in the states 9 and 10)
void Em202_01_BSpider::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C - 9U <= 1) {
        msg->m10 = lbl_807C7030;
    }
    bool down = true;
    unsigned int flags = mStatus.m0C;
    if ((flags & 0x80000) != 0x80000 && (flags & 0x1000000) != 0x1000000) {
        down = false;
    }
    if (!down && m58C != 13 && msg->mData->m01 != 1) {
        int guard = 0;
        float dot = FrontDot_unk(this, msg->m1C);
        if (m58C - 9U <= 1) {
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
    if (m58C == 14) {
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

void Em202_01_BSpider::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 0;
        m14B4 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        fn_800CB5C8(&m153C, 15, 0);
        fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_80272660();
        return;
    }
    if (msg->mData != 0 && msg->mData->m04 == 0x1791) {
        m14B4 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 14;
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
        fn_8023A8B4(lbl_8087F3C0, &m1530, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        Effect_unk* mgr = lbl_8087F048;
        atn::Vector3 p = fn_801781B0();
        fn_80108F38(mgr, &p, 0x80);
    }
}

// inferred: movement of the frame (route in state 7 facing the target)
void Em202_01_BSpider::fn_80270B8C() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 dir(mRoute.m58);
        speed = dir.Length_unk();
        if (speed > 0.0001f) {
            rot = dir.GetNormalize().ToRotation_unk();
        }
        CharaTask* t = m14B0;
        dir = t->mPos - mPos;
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        dir.fn_80139F60();
        mRot.y = dir.y;
        fn_801426A4(&rot, speed, m568 * rate);
    } else if (m55C == 6) {
        fn_8013A258();
    } else {
        fn_8013CB68(rot, 0, speed, m568 * rate);
    }
}

void Em202_01_BSpider::vf7C(int arg) {
    m14B4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 13;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}

// inferred: translation of a node matrix (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: effect `efp` on the model (names invented, as in game/Em202Spider.cpp)
inline void PlayModelEffect_unk(CharaTask* self, int id, atn::EfpFile* efp) {
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

// inferred: state 10 (needle attack: charge for 60 frames, then up to 15 needles from the hips)
void Em202_01_BSpider::fn_80271104() {
    switch (m14D0) {
    case 0: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14D0 = 1;
            PlayModelEffect_unk(this, 0, &m1518);
            m153C = fn_800C344C(sSe_unk[0], &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x143, 1, 0, 1, 0.0f, 0.1f);
        }
        break;
    }
    case 1:
        m14C4++;
        if (m14C4 >= 60) {
            m14D0 = 2;
            m14C4 = 0;
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            fn_800CB5C8(&m153C, 0, 0);
            fn_800C344C(sSe_unk[1], &mPos, 0, -1, 1.0f);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x144, 0, 0, 1, 0.0f, 0.1f);
        }
        break;
    case 2: {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            ResetState_unk(this);
            sSpotUsed_unk[m14BC] = 0;
            break;
        }
        float f = mMotion.mTracks[0].m08;
        if (12.5f < f && f < 13.5f) {
            m14C4++;
            if (m14C4 < 15) {
                mMotion.mTracks[0].m08 = 12.0f;
            }
            break;
        }
        float f2 = mMotion.mTracks[0].m08;
        if (f2 >= 7.0f && f2 <= 9.0f) {
            atn::Model* model = &mMotion;
            atn::Vector3 pos = GetNodePos_unk(model->GetNodeMtx_unk("hips"));
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx r;
            PSMTXRotRad(r, 'y', mRot.y);
            PSMTXMultVec(r, &dir, &dir);
            Effect_unk* mgr = lbl_8087F048;
            fn_800F8574(mgr, this, fn_80219E6C(0x3F2), &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
        }
        break;
    }
    }
}

// inferred: state 11 (turn), then the action m14D8 on the target m14E8
void Em202_01_BSpider::fn_802714B0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        switch (m14D8) {
        case 0:
            m58C = 0;
            break;
        case 6:
            fn_8027179C(m14E8);
            break;
        case 7:
        case 8:
            fn_80271AA4(m14E8);
            break;
        case 10:
            fn_80271DAC(m14E8);
            break;
        }
    } else {
        float f = mMotion.mTracks[0].m08;
        if (5.0f <= f && f <= 16.0f) {
            m14DC += 2;
            mRot.y = atn::WrapAngle_unk(m14E0 + m14DC * atn::WrapAngle_unk(m14E4 - m14E0) / 12.0f);


        }
    }
}

// inferred: death sequence (after the death motion: effect m150C at frame 29 of the countdown m1434)
void Em202_01_BSpider::vfEC() {
    if (mMotion.mTracks[0].m00 != 0x2E) {
        fn_80272660();
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
            if (m1434 == 29) {
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
                fn_8023A8B4(lbl_8087F3C0, &m150C, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
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

void Em202_01_BSpider::fn_8027179C(CharaTask* target) {
    if (fn_8027232C(target, 6) != 0) {
        return;
    }
    m14B4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(6);
    m560 = 4;
    m14C8++;
    atn::Vector3 pos(target->mPos);
    atn::Vector3 rot = (pos - mPos).GetNormalize().ToRotation_unk();
    mRot = rot;
    mRot.x = 0.0f;
    mRot.z = 0.0f;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
}

void Em202_01_BSpider::fn_80271AA4(CharaTask* target) {
    if (fn_8027232C(target, 8) != 0) {
        return;
    }
    m14B4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 8;
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
    atn::Vector3 pos(target->mPos);
    atn::Vector3 rot = (pos - mPos).GetNormalize().ToRotation_unk();
    mRot = rot;
    mRot.x = 0.0f;
    mRot.z = 0.0f;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 99.76% - r4/r5 of the zero vector copy and r29/r30 of the counter (same case as
// Em202Spider::fn_80281924)
// inferred: starts state 10 facing the center of the party
void Em202_01_BSpider::fn_80271DAC(CharaTask* target) {
    if (fn_8027232C(target, 10) != 0) {
        return;
    }
    m14B4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 10;
    fn_8016E970(6);
    m560 = 4;
    atn::Vector3 center;
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
    atn::Vector3 rot = (center - mPos).GetNormalize().ToRotation_unk();
    mRot = rot;
    mRot.x = 0.0f;
    mRot.z = 0.0f;
    m14C4 = 0;
    m14D0 = 0;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
}

// inferred: starts state 12: runs to the farthest spot, or to the next free one after it
void Em202_01_BSpider::fn_802721A0() {
    float best = 0.0f;
    int far = 0;
    for (int i = 0; i < 6; i++) {
        atn::Vector3 d = sSpotPos_unk[i] - mPos;
        if (best < PSVECSquareMag(&d)) {
            best = PSVECSquareMag(&d);
            far = i;
        }
    }
    int start = far;
    while (sSpotUsed_unk[far] != 0) {
        far++;
        if (far >= 6) {
            far = 0;
        }
        if (far == start) {
            break;
        }
    }
    sSpotUsed_unk[far] = 1;
    m14BC = far;
    fn_8016F824(sSpotIds_unk[far], 2);
    m58C = 12;
    m14C4 = 0;
    m568 = 1.0f;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14, 0, 0, 1, 0.0f, 0.1f);
}

int Em202_01_BSpider::fn_8027232C(CharaTask* target, int arg) {
    if (m58C == 11) {
        return 0;
    }
    m14E0 = mRot.y;
    atn::Vector3 rot = (target->mPos - mPos).ToRotationZ_unk();
    m14E4 = rot.y;
    float d = atn::WrapAngle_unk(rot.y - m14E0);
    if (-0.3926991f < d && d < 0.3926991f) {
        return 0;
    }
    m14DC = 0;
    m14D8 = arg;
    m14E8 = target;
    m58C = 11;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    return 1;
}

void Em202_01_BSpider::fn_80272660() {
    if (m58C == 10 && m14D0 == 1) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
    fn_800CB5C8(&m153C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0();
}
