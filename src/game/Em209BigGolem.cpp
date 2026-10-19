#include "game/Em209BigGolem.h"
#include "math/Vec.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" void fn_802CFAE4(Em209SmallGolem* golem); // inferred: Em209SmallGolem (not decompiled yet)

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

// inferred: node position (name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

Em209BigGolem::Em209BigGolem(atn::Task* parent, EnemyInfo_unk* info) : Em209SmallGolem(parent, info) {
    m1510 = 0x409;
    m1514 = 0x408;
    m1518 = 40.0f;
    m1548 = 1;
}

Em209BigGolem::~Em209BigGolem() {}

void Em209BigGolem::vf3C(HitInfo_unk* hit) {
    fn_80151448((HitMsg_unk*)hit);
    vf40(hit);
}

void Em209BigGolem::vf40(HitInfo_unk* hit) {
    if (m55C == 6 && m560 >= 22 && m560 <= 23) {
        m58C = 0;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802CFAE4(this);
        vf48();
        fn_80178A6C();
        fn_8015495C(0, 1, 1, 1);
        fn_8016DA4C();
        m58C = 2;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
        m574 = lbl_807C7030;
        m1434 = 30;
        vf9C(1);
    }
}

// NONMATCHING: 99.7% - the address registers of the copy of the node position into b are swapped
// (r3/r4); the chained assignment of a fixes the same swap for a.
void Em209BigGolem::vf20() {
    atn::Vector3 a;
    atn::Vector3 b;
    float radius = m5B0;
    m620 = radius / 2.0f;
    m614 = mPos + m5A4;
    m614.y += m620;
    (a = mPos + m5A4).y += 0.8f * radius;
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = radius;
}

void Em209BigGolem::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (!m1548) {
            if (m1434 > 0) {
                if (m1434 == 30) {
                    fn_800EB7A0(this);
                } else if (m1434 == 15) {
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
                    fn_8023A8B4(lbl_8087F3C0, &m1528, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
                    vf9C(1);
                }
                m1434--;
            } else {
                mFlag15 = 1;
                fn_801765D8();
                fn_800EE360(this);
            }
        }
    } else {
        m1450 = 60;
    }
}
