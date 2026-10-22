#include "game/Em208Queen.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

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

// inferred: names invented
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

void Em208Queen::vf54() {
    if (m624.mSize != 0) {
        m624.mData[0].m04.mCenter = mColli.m3C;
        m624.mData[0].m04.mRadius = mColli.m54;
        m624.mData[1].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    }
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

// inferred: offset of the point hit by the cannon shots (same values as chr/ControlCannonLift.cpp)
static atn::Vector3 sShotOffset_unk(3.882f, 98.679f, 10.846f);
