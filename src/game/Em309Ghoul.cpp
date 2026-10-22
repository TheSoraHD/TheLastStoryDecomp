#include "game/Em309Ghoul.h"
#include "game/CharaManager.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include <string.h>

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" void fn_801092C8(Effect_unk* effect, int msg, CharaTask* chara, int value, int arg);
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, see MP/D2AMpMenuSetRule.cpp)
struct GhoulMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" GhoulMsgTable_unk* lbl_8087F1E4;
// inferred: debug drawing (lbl_8087EEB0, see game/CharaTask.cpp)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_800638B0(void* draw, float a, float b, const atn::Vector3* from, const atn::Vector3* to,
                            unsigned int color);
extern "C" void fn_80063D3C(void* draw, const atn::Vector3* center, float radius, int color, float arg);

extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: per-player weights of an effect (floats at +0x128); names invented
struct GhoulWeights_unk {
    unsigned char m00[0x128];
    float mWeight[4];
};
extern "C" GhoulWeights_unk* fn_80103F60(Effect_unk* mgr, CharaTask* owner);
extern "C" int fn_80108378(GhoulWeights_unk* w, CharaTask* chara); // inferred: index of `chara`, < 0 if missing
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets a counter
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed (as in game/Arms.cpp)
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event

Em309Ghoul::~Em309Ghoul() {}

// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

int Em309Ghoul::vf0C() {
    int ret = 1;
    if (fn_8013655C() != 0) {
        ret = 0;
    }
    if (m1528.fn_80237874() || m1534.fn_80237874() || m1588.fn_802376D0() || m1540.fn_80237874() ||
        m1564.fn_80237874() || m157C.fn_80237874() || m1570.fn_80237874() || m1558.fn_80237874() ||
        m1594.fn_80237874() || m15A0.fn_80237874()) {
        ret = 0;
    }
    if (m1674[0].fn_80473F50() || m1674[1].fn_80473F50()) {
        ret = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        ret = 0;
    }
    if (ret != 0) {
        fn_803057A8();
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        if (lbl_8087F430 != 0 && *(int*)((char*)lbl_8087F430 + 0x10D8) != 0) {
            m1518 = m150C;
        }
        mRoute.mA0 = 1;
        m58C = 0;
        fn_8016E970(3);
        for (m1614 = lbl_8087F4A0->mHead; m1614 != 0; m1614 = m1614->m5C) {
            if (m1614->mId == 0x80A) {
                break;
            }
        }
        m56C = 35.0f;
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4011;
        mStatus.m18 |= 0x3000000;
        mStatus.m18 |= 0x8004;
    }
    return ret;
}

// inferred: start of the death state (names invented)
inline void Em309Ghoul::Die_unk(int motion, int flag) {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 2;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
    m1684 = flag;
    m1688 = 0.0f;
    fn_80370320(lbl_8087F430, 0xD0, 1, 0);
}

void Em309Ghoul::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m14FC += msg->m68;
    if (m55C == 6 && m560 == 22 && (m58C == 1 || m58C == 6 || m58C == 8)) {
        m58C = 0;
        m14F0 = 0;
        m14F4 = 0;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (m58C == 12) {
            vf44();
            Die_unk(0x37, 1);
            fn_80239DAC(lbl_8087F3C0, this, 0x131, 1);
            fn_800EB7A0(this);
        } else {
            fn_80239DAC(lbl_8087F3C0, this, 0x190, 1);
            vf44();
            Die_unk(0x2E, 0);
            fn_800EB7A0(this);
        }
    }
}

atn::Vector3 Em309Ghoul::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (fn_80370A78(lbl_8087F430, 0xA1) == 0) {
        result = mPos;
    } else {
        result = m624.mData[1].m04.mCenter;
    }
    return result;
}

void Em309Ghoul::vf44() {
    vf48();
    mStatus.fn_8012DD70();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    mFC0 = 0;
    m1434 = 80;
    m15B0 = 30;
    if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) != 0 && mType == 2) {
        fn_801092C8(lbl_8087F048, 6, this, 0, 0);
    }
}

// NONMATCHING: 93.27% - r6/r7 of the sum temporaries and the place of the "spine1" address load (scheduled
// before the first copy in the original); 6 variants (named temporaries, helpers, node pointer apart)
void Em309Ghoul::vf54() {
    if (m624.mSize != 0) {
        m624.mData[0].m04.mRadius = 10.0f;
        m624.mData[0].m04.mCenter = mPos + m5A4;
        m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
        m624.mData[1].m04.mRadius = m16EC;
        m624.mData[1].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        m624.mData[1].m04.mCenter.y += m16F0;
        m624.mData[2].m04.mRadius = 0.7f * m16EC;
        m624.mData[2].m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        m624.mData[2].m04.mCenter.y += 0.5f * m16F0;
    }
    if (m1690 != 0) {
        fn_80063D3C(lbl_8087EEB0, &m624.mData[1].m04.mCenter, 20.0f, 0xFFFF00FF, 1000.0f);
    }
}

// NONMATCHING: 99.48% - r5/r6 of the copies of the temporaries (__VecCopy family, documented blocker)
void Em309Ghoul::vf20() {
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 base(mPos.x, mPos.y, mPos.z);
    atn::Vector3 head = base;
    head.y += 1.5f * m620;
    float r = m620;
    mColli.m3C = base;
    mColli.m48 = head;
    mColli.m54 = r;
}

void Em309Ghoul::fn_802FF6C4() {
    mC58.mC8 = (int)mC58.mC4;
    if (fn_80304B40()) {
        m1524++;
    } else {
        m1524 = 0;
    }
    PlayerTask* leader = lbl_8087F8A0->mHead;
    if (m1524 >= m16F8) {
        m1454 = 0;
        m14EC = leader;
    } else {
        m1454 = 1;
        m14EC = mC58.mC4;
    }
    GhoulWeights_unk* w = fn_80103F60(lbl_8087F048, this);
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        int i = fn_80108378(w, p);
        if (i >= 0) {
            if (m1524 >= m16F8) {
                w->mWeight[i] = p == leader ? 1.0f : 0.0f;
            } else {
                w->mWeight[i] = 1.0f;
            }
        }
    }
    mC58.mC4 = m14EC;
}

void Em309Ghoul::fn_80300654() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 1;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x141), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

// NONMATCHING: 99.57% - the slots of the difference and of the rotation copy are swapped on the stack
// (~15 variants: ToRotation_unk/ToRotationZ_unk, named difference, helpers by value/reference, RotZ_unk in
// place)
void Em309Ghoul::fn_8030073C() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 6;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    if (m14EC != 0) {
        mRot = (m14EC->mPos - mPos).ToRotationZ_unk();
    }
}

void Em309Ghoul::fn_803009F4() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 9;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x143, 0, 0, 1, 0.0f, 0.1f);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x145), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

// inferred: the rotation matrices of the three axes are function-scope locals (stack order of the original)
void Em309Ghoul::fn_80300DF4() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 11;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    m15D8.x = 0.0f;
    m15D8.y = 0.0f;
    m15D8.z = 0.0f;
    atn::Matrix3x4 m;
    Mtx rx;
    Mtx tx;
    Mtx ry;
    Mtx ty;
    Mtx rz;
    Mtx tz;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != mRot.z) {
        PSMTXRotRad(rz, 'z', mRot.z);
        PSMTXConcat(p->m, rz, tz);
        __MtxCopy(p->m, tz);
    }
    if (0.0f != mRot.y) {
        PSMTXRotRad(ry, 'y', mRot.y);
        PSMTXConcat(p->m, ry, ty);
        __MtxCopy(p->m, ty);
    }
    if (0.0f != mRot.x) {
        PSMTXRotRad(rx, 'x', mRot.x);
        PSMTXConcat(p->m, rx, tx);
        __MtxCopy(p->m, tx);
    }
    PSMTXMultVec(m.m, &m15D8, &m15D8);
    m15CC = mRot;
    m15C8 = 0;
    m15C4 = 0;
    fn_80370AE4(lbl_8087F430, 0xA4, 1);
    fn_80370AE4(lbl_8087F430, 0xA5, 1);
    fn_80232B7C(this, 400);
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
    fn_8023A8B4(lbl_8087F3C0, &m15A0, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

void Em309Ghoul::fn_803010BC() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 12;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    fn_8017A300(m1670);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x149), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x14B), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
    if (mFlag22) {
        fn_80370AE4(lbl_8087F430, 0xA8, 1);
    }
}

void Em309Ghoul::fn_80301200() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 13;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
    fn_80370AE4(lbl_8087F430, 0xA3, 1);
}

void Em309Ghoul::vf14() {
    BattleCharaTask::vf14();
    if (m1690 != 0) {
        fn_800638B0(lbl_8087EEB0, 1000.0f, -1.0f, &mPos, &(m15D8 * 100.0f + mPos), 0xFF0000FF);
        fn_80063D3C(lbl_8087EEB0, &m1608, m16E0, -1, 1000.0f);
    }
}
