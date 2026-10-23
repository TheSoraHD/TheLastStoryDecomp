#include "game/Em309Ghoul.h"
#include "atn/Csv.h"
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
extern "C" double sqrt(double x);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
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
extern "C" chr::CharaData_unk* fn_8020A81C(const char* name); // inferred: character data by name
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: per-player weights of an effect (floats at +0x128); names invented
struct GhoulWeights_unk {
    unsigned char m00[0x128];
    float mWeight[4];
};
extern "C" GhoulWeights_unk* fn_80103F60(Effect_unk* mgr, CharaTask* owner);
extern "C" int fn_80108378(GhoulWeights_unk* w, CharaTask* chara); // inferred: index of `chara`, < 0 if missing
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78
struct Field_unk {
    unsigned char m00[0x8A0];
    CharaTask* m8A0_unk; // inferred: character the camera follows (as Field_unk::m8A0 in game/Em203GreaterDaemon.cpp)
    unsigned char m8A4[0x10D8 - 0x8A4];
    RouteTable_unk* m10D8; // inferred: route tables (as in gmk/ReinforcePoint.cpp)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets a counter
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed (as in game/Arms.cpp)
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
// inferred: keyboard of the debug build (lbl_8087EEF0, as in game/EnemyQuarkSecond.cpp)
struct GhoulKeyboard_unk {
    unsigned char m000[0x34];
    unsigned char m034[0xD90 - 0x34];
    int mD90;
};
extern "C" GhoulKeyboard_unk* lbl_8087EEF0;
extern "C" int fn_8007708C(GhoulKeyboard_unk* kb, void* state, int key); // inferred: key pressed
extern "C" inline GhoulKeyboard_unk* fn_802A49BC() {
    return lbl_8087EEF0;
}
extern "C" inline int fn_802A4968(GhoulKeyboard_unk* kb, int pad, int key) {
    int result = 0;
    if (kb->mD90 != 0 && fn_8007708C(kb, &kb->m034[pad * 0x1A], key) != 0) {
        result = 1;
    }
    return result;
}
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data by id
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
extern "C" void fn_80376254(Field_unk* field, int a, int b); // inferred: counts a field event
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double fmod(double x, double y);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int arg, float angle); // inferred: fires a bullet (as in game/Em205Ogre.cpp)
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void fn_800616C0(void* draw, float x, float y, float z, float w, float h, float f6, const char* text,
                            unsigned int color, int a, int b, int c); // inferred: text (as in game/ChaserNpc.cpp)
extern "C" void fn_80063200(void* draw, int arg, unsigned int color, float x, float y, float z, float a,
                            float b); // inferred: debug cross (as in gmk/CharaPathMove.cpp)

// NONMATCHING: 96.97% - registers of the inlined FindPlace_unk loops (the original keeps the table in r11 for
// all of them) and scheduling of the stores of the ids 0x44D-0x452; ~4 variants
Em309Ghoul::Em309Ghoul(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14EC(0), m14F0(0), m14F4(0), m14F8(0), m14FC(0.0f), m151C(0), m1520(0),
      m15B0(0), m15B4(1000), m15B8(0), m15BC(0), m15C4(0), m15E4(0), m15EC(0), m15F0(1), m1624(0), m1628(1),
      m1668(0), m1669(0), m166A(0), m1690(0), m1694(80.0f), m1698(15), m169C(30), m16A0(60), m16A4(30.0f),
      m16AC(300.0f), m16B0(3), m16B4(30.0f), m16B8(40.0f), m16BC(10.0f), m16C0(20.0f), m16C4(7.0f), m16C8(18.0f),
      m16CC(0), m16D0(1800), m16D4(3), m16D8(50), m16DC(10), m16E0(18.0f), m16E4(30), m16E8(10), m16EC(25.0f),
      m16F0(15.0f), m16F4(12.0f), m16F8(15) {
    m1528.fn_8023780C("/effect/ef_uc027a.efp");
    m1534.fn_8023780C("/effect/ef_uc027c.efp");
    m1588.fn_80237654("/effect/ef_mb001c.eff");
    m1540.fn_8023780C("/effect/ef_sk030a.efp");
    m154C.fn_80237654("/effect/ef_sk030b.eff");
    m1558.fn_8023780C("/effect/ef_sk030b.efp");
    m1564.fn_8023780C("/effect/ef_bu002.efp");
    m1570.fn_8023780C("/effect/ef_bu004.efp");
    m157C.fn_8023780C("/effect/ef_bu003.efp");
    m1594.fn_8023780C("/effect/ef_bd001.efp");
    m15A0.fn_8023780C("ef_bs068.efp");
    m1674[0].vf0C("camera/battle/cm000_em309_00.xsca");
    m1674[1].vf0C("camera/battle/cm000_em309_01.xsca");
    if ((int)info->m20 == 0x497C9) {
        mFlag22 = 1;
        mStatus.mParamB.m9C |= 0x10;
    }
    atn::Csv csv(info->m2C, strlen(info->m2C));
    for (;;) {
        const char* tok = csv.fn_8005B9CC();
        if (tok == 0 || strcmp(tok, "") == 0) {
            break;
        }
        if (strncmp(tok, "base1=", 5) == 0) {
            m1500[0] = fn_800DC12C(tok + 6);
        } else if (strncmp(tok, "base2=", 5) == 0) {
            m1500[1] = fn_800DC12C(tok + 6);
        } else if (strncmp(tok, "base3=", 5) == 0) {
            m1500[2] = fn_800DC12C(tok + 6);
        }
    }
    for (int i = 0; i < 3; i++) {
        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(m1500[i]);
        if (place != 0) {
            m150C[i] = place;
        } else {
            m150C[i] = 0;
            m1668 = 1;
        }
    }
    RouteTable_unk* table = lbl_8087F430->m10D8;
    RoutePlace_unk* center = table->FindPlace_unk(0x385);
    if (center != 0) {
        m1600 = center;
        m1608 = table->FindPlace_unk(0x385)->mPos;
    } else {
        m166A = 1;
    }
    int id = 0x321;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            m14C4[i][j] = id;
            RoutePlace_unk* place = table->FindPlace_unk(id++);
            if (place != 0) {
                atn::Vector3 pos = place->mPos;
                m1638[i][j][0] = pos.x;
                m1638[i][j][1] = pos.z;
            } else {
                m1669 = 1;
            }
        }
    }
    m14D4[0] = 0x44D;
    m14D4[1] = 0x44E;
    m14D4[2] = 0x44F;
    m14D4[3] = 0x450;
    m14D4[4] = 0x451;
    m14D4[5] = 0x452;
    m1604 = table->FindPlace_unk(0x450);
    mFlag21 = 1;
    m166C = fn_8020A81C("em_dg010Boss");
    m1670 = fn_8020A81C("em_dg010Boss_B");
    m1618.x = 0.0f;
    m1618.y = 0.0f;
    m1618.z = 0.0f;
    m15F4.x = 0.0f;
    m15F4.y = 0.0f;
    m15F4.z = 0.0f;
}

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
            m1518 = m150C[0];
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

// inferred: starts state 7 (motion 0x147, message 0x143); inlined in the debug keys of vf10 (name invented)
inline void Em309Ghoul::StartAttack_unk() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 7;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x143), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}

// inferred: back to the idle state 0 (name invented)
inline void Em309Ghoul::ResetState_unk() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 0;
    fn_8016E970(3);
    m15F0++;
}

void Em309Ghoul::vf10() {
    fn_802FF6C4();
    m14F4++;
    m15EC++;
    if (mC58.mC0 != 0) {
        m1520++;
    }
    lbl_8087F430->m8A0_unk = 0;
    if (mC58.mC0 != 0 && mC58.mC4 != 0) {
        switch (m58C) {
        case 1: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk();
            }
            frame = mMotion.mTracks[0].m08;
            if (frame >= 35.0f && frame <= 38.0f) {
                fn_80144710();
                HitInfo_unk* skill = fn_80219E6C(0x64A);
                int id = m590;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, id, 30, -1, 0.0f);
            }
            if (m1690 != 0) {
                fn_80063D3C(lbl_8087EEB0, &mPos, mStatus.mParamB.m28 + fn_80219E6C(0x64A)->m40, 0xFFFF0000, 0.0f);
            }
            break;
        }
        case 7: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                ResetState_unk();
            }
            frame = mMotion.mTracks[0].m08;
            if (frame >= 35.0f && frame <= 38.0f) {
                fn_80144710();
                HitInfo_unk* skill = fn_80219E6C(0x64B);
                int id = m590;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, id, 30, -1, 0.0f);
            }
            if (m1690 != 0) {
                fn_80063D3C(lbl_8087EEB0, &mPos, mStatus.mParamB.m28 + fn_80219E6C(0x64B)->m40, 0xFFFF0000, 0.0f);
            }
            break;
        }
        case 2:
            vfEC();
            break;
        case 6:
            fn_80301DDC();
            break;
        case 8:
            fn_803026F0();
            break;
        case 9:
            fn_80302B84();
            break;
        case 10:
            fn_80302F94();
            break;
        case 11:
            fn_803041C8();
            break;
        case 12:
            fn_80304718();
            break;
        case 13:
            if (m14F4 > m16D8) {
                m15EC = 0;
                ResetState_unk();
            }
            break;
        default:
            fn_802FF800();
            fn_802FFFF8();
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    if (m15B8 != 0) {
        fn_80304FC0();
    }
    if (m1690 == 0) {
        return;
    }
    fn_80063D3C(lbl_8087EEB0, &mPos, m1694, 0xFFFFFF00, 0.0f);
    if (fn_802A4968(fn_802A49BC(), 0, 0x50)) {
        fn_803010BC();
    }
    if (fn_802A4968(fn_802A49BC(), 0, 0x4D)) {
        fn_8030073C();
    }
    if (fn_802A4968(fn_802A49BC(), 0, 0x4C)) {
        StartAttack_unk();
    }
    if (fn_802A4968(fn_802A49BC(), 0, 0x4A)) {
        fn_803009F4();
    }
    if (fn_802A4968(fn_802A49BC(), 0, 0x48)) {
        mPos = m150C[0]->mPos;
        mRot.y = m150C[0]->m14;
        mRot.z = 0.0f;
        mRot.x = 0.0f;
        ResetState_unk();
    }
    if (m166A == 0) {
        fn_80063200(lbl_8087EEB0, 1, 0xFF0000FF, m1608.x, m1608.y, m1608.z, 1000.0f, 10.0f);
    }
    char buf[0x68];
    sprintf(buf, "%3f %3f %3f", mRot.x, mRot.y, mRot.z);
    fn_800616C0(lbl_8087EEB0, 100.0f, 100.0f, 1000.0f, 15.0f, 15.0f, 0.0f, buf, 0xFF0000FF, 1, 1, 0);
    if (fn_80304B40()) {
        char text[0x78];
        sprintf(text, "!");
        fn_800616C0(lbl_8087EEB0, 300.0f, 100.0f, 1000.0f, 10.0f, 10.0f, 0.0f, text, 0xFFFF0000, 1, 1, 0);
    }
    fn_80063D3C(lbl_8087EEB0, &m1618, 10.0f, -1, 1000.0f);
    fn_800638B0(lbl_8087EEB0, 1000.0f, -1.0f, &m1618, &(m1618 + m15F4), -1);
    if (m1668 != 0) {
        fn_80063D3C(lbl_8087EEB0, &m1518->mPos, m1694, 0xFF000000, 10000.0f);
    }
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


// NONMATCHING: 99.59% - stack layout of the direction temporaries and of the rotation matrices (~6 variants)
// inferred: hits: the head spear (0x134) or a hit while weak (m40 == 1 outside state 10) knock the ghoul back
// and start the rolling state 10 away from the attacker (bent towards the home place); other hits only do
// damage while the ghoul is under the coffin (state 12)
void Em309Ghoul::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    int weak = *(int*)&msg->m3C[4]; // inferred: HitMsg_unk+0x40 (as in game/Em205Ogre.cpp)
    if ((weak == 1 && m58C != 10) || msg->mData->m04 == 0x134) {
        if (weak == 1) {
            fn_80376254(lbl_8087F430, 3, 5);
        }
        msg->m0C |= 0x800;
        if (msg->mData->m04 == 0x134) {
            fn_80370AE4(lbl_8087F430, 6, 1);
        }
        int state = m58C;
        if (state == 1 || (state == 6 && m14F0 == 1) || state == 7 || state == 8 || state == 10 || state == 12) {
            msg->m10 *= 0.0f;
        }
        fn_80151448(msg);
        vf40(hit);
        if (mStatus.IsState_unk(0x20)) {
            return;
        }
        if (!(m58C - 10U <= 2)) {
            atn::Vector3 dir = mPos - msg->mAttacker->mPos;
            dir.y = 0.0f;
            PSVECNormalize(&dir, &dir);
            if (m1518 != 0) {
                atn::Vector3 home = m1608 - m1518->mPos;
                home.y = 0.0f;
                PSVECNormalize(&home, &home);
                float dot = PSVECDotProduct(&home, &dir);
                if ((float)__fabs(dot) < 0.70710677f) {
                    dir = dir * 1.2f + home * (dot >= 0.0f ? 1.0f : -1.0f);
                    PSVECNormalize(&dir, &dir);
                }
            }
            m15CC = dir.ToRotation_unk();
            m15CC.x = 0.0f;
            m15CC.z = 0.0f;
            float y = m15CC.y;
            float a = fmod(y, 6.2831854820251465);
            if (a > 3.1415927f) {
                a -= 6.2831855f;
            }
            if (a < -3.1415927f) {
                a += 6.2831855f;
            }
            m15CC.y = a;
            fn_80300AD8();
        }
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
        fn_8023A8B4(lbl_8087F3C0, &m1594, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        m14F8 = 1;
        return;
    }
    if (m58C == 12 && (m14F0 == 0 || (m14F0 == 1 && mMotion.mTracks[0].m08 < 40.0f))) {
        msg->m0C |= 0x80;
        msg->m10 *= 0.0f;
        mMotion.m34C = 2;
        mMotion.mTracks[1].m20 = 1.0f;
        mMotion.mTracks[1].m0C = 1.0f;
        mMotion.fn_80097C08(1, 0x38, 0, 1, 1, 0.0f, 0.1f);
    } else {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
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

// NONMATCHING: 99.99% - only the .sdata2 slot of 0.7f (pool order)
// inferred: three damage spheres: the feet and two on the "spine1" node (moved every frame by vf54)
void Em309Ghoul::vf50() {
    Item14_unk item;
    item.m04.mRadius = 10.0f;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += item.m04.mRadius;
    item.m00 = 4;
    m624.push_back(item);
    item.m04.mRadius = m16EC;
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
    item.m04.mCenter.y += m16F0;
    item.m00 = 2; item.m00 |= 9;
    m624.push_back(item);
    item.m04.mRadius = 0.7f * m16EC;
    item.m04.mCenter = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
    item.m04.mCenter.y += 0.7f * m16F0;
    item.m00 = 1; item.m00 |= 0xC;
    m624.push_back(item);
    mFlagB29 = 0;
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

// NONMATCHING: 99.75% - f7/f8 of 0.0f and m16BC and the order of the y/z stores of m15D8 (~13 variants:
// field order, local for z, Set_unk, Vector3 temporary, volatile read, accessor, chained assignment)
void Em309Ghoul::fn_80300AD8() {
    m14F0 = 0;
    m14F4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    fn_80097CCC(&mMotion, 2, 0.0f);
    m58C = 10;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    float z = m16BC;
    m15D8.x = 0.0f;
    m15D8.y = 0.0f;
    m15D8.z = z;
    atn::Matrix3x4 m;
    Mtx rx;
    Mtx tx;
    Mtx ry;
    Mtx ty;
    Mtx rz;
    Mtx tz;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != m15CC.z) {
        PSMTXRotRad(rz, 'z', m15CC.z);
        PSMTXConcat(p->m, rz, tz);
        __MtxCopy(p->m, tz);
    }
    if (0.0f != m15CC.y) {
        PSMTXRotRad(ry, 'y', m15CC.y);
        PSMTXConcat(p->m, ry, ty);
        __MtxCopy(p->m, ty);
    }
    if (0.0f != m15CC.x) {
        PSMTXRotRad(rx, 'x', m15CC.x);
        PSMTXConcat(p->m, rx, tx);
        __MtxCopy(p->m, tx);
    }
    PSMTXMultVec(m.m, &m15D8, &m15D8);
    mRot = m15CC;
    m15C8 = 1;
    fn_80370AE4(lbl_8087F430, 0xA4, 1);
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x147), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
    if (mFlag22) {
        fn_80370AE4(lbl_8087F430, 0xA7, 1);
    }
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

// NONMATCHING: 99.55% - only the offset of "righthand" in the string pool and the label of the 2*pi double
// (strings/constants of functions not written yet); the angle wrap is written out because the inline
// WrapAngle_unk uses the float 2*pi
void Em309Ghoul::fn_80302B84() {
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        m14F0 = 0;
        m14F4 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        fn_80097CCC(&mMotion, 2, 0.0f);
        m58C = 0;
        fn_8016E970(3);
        m15F0++;
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= 32.0f && frame <= 45.0f) {
        atn::Vector3 hand = GetNodePos_unk(mMotion.GetNodeMtx_unk("righthand"));
        hand.y -= m16F4;
        atn::Vector3 d = hand - lbl_8087F8A0->mHead->mPos;
        float dist = sqrt(d.x * d.x + d.z * d.z);
        atn::Vector3 rot = lbl_8087F8A0->mHead->mRot;
        float a = fmod(rot.y - mRot.y, 6.2831854820251465);
        if (a > 3.1415927f) {
            a -= 6.2831855f;
        }
        if (a < -3.1415927f) {
            a += 6.2831855f;
        }
        atn::Vector3 ofs(0.0f, 0.0f, 0.0f);
        ofs.x = atn::Abs_unk(a) > 1.5707964f ? 5.0f : -5.0f;
        atn::Matrix3x4 m;
        Mtx ry;
        Mtx ty;
        Mtx rx;
        Mtx tx;
        Mtx rz;
        Mtx tz;
        atn::Matrix3x4* p = &m;
        p->SetIdentity();
        if (0.0f != rot.z) {
            PSMTXRotRad(rz, 'z', rot.z);
            PSMTXConcat(p->m, rz, tz);
            __MtxCopy(p->m, tz);
        }
        if (0.0f != rot.x) {
            PSMTXRotRad(rx, 'x', rot.x);
            PSMTXConcat(p->m, rx, tx);
            __MtxCopy(p->m, tx);
        }
        if (0.0f != rot.y) {
            PSMTXRotRad(ry, 'y', rot.y);
            PSMTXConcat(p->m, ry, ty);
            __MtxCopy(p->m, ty);
        }
        PSMTXMultVec(m.m, &ofs, &ofs);
        if (dist < 20.0f) {
            atn::Vector3 pos = lbl_8087F8A0->mHead->mPos + ofs;
            Effect_unk* mgr = lbl_8087F048;
            fn_800FAB80(mgr, this, fn_80219E6C(0x64E), 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        mRot.y += 0.11219973f;
        if (m1690 != 0) {
            fn_80063D3C(lbl_8087EEB0, &hand, 10.0f, 0xFFFF0000, 10000.0f);
        }
    }
}

// inferred: plays effect `efp` on `model` with default placement (name invented)
inline void GhoulEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

// NONMATCHING: 99.38% - registers of the inlined FindPlace_unk loop (index/offset swapped) and the offset of
// "em_dg010Boss" in the pool (strings of functions not written yet); ~10 variants (id/table locals, own loops)
void Em309Ghoul::fn_80304718() {
    if (m14F0 == 0) {
        if (mMotion.mTracks[0].m00 == 0x14D) {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x14E, 1, 0, 1, 0.0f, 0.1f);
                fn_80232B7C(this, 0x131);
                GhoulEffect_unk(&m1570, m1614->vf38());
                return;
            }
            if (mMotion.mTracks[0].m08 <= 10.0f) {
                mPos.y -= 1.6363636f;
            }
        }
        if (m14F4 > m16DC * 30) {
            fn_8017A300(m166C);
            fn_80300DF4();
            fn_80239DAC(lbl_8087F3C0, this, 0x131, 1);
            fn_80370AE4(lbl_8087F430, 0xA2, 1);
            m14F4 = 0;
        }
    } else if (m14F0 == 1) {
        float end = mMotion.mTracks[0].m08;
        if (end > mMotion.fn_80097D7C(0)) {
            mPos.y = 0.0f;
            if (m15C8 == 1) {
                fn_80300DF4();
                m14F8 = 2;
                m15EC = 0;
            } else {
                fn_80300DF4();
            }
            m15F0 = 1;
        }
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 19.0f && frame < 20.0f) {
            fn_80239DAC(lbl_8087F3C0, this, 0x131, 1);
            fn_80232B7C(this, 0x132);
            GhoulEffect_unk(&m157C, m1614->vf38());
            fn_8017A300(fn_8020A81C("em_dg010Boss"));
            fn_8014C540();
        }
        frame = mMotion.mTracks[0].m08;
        if (frame >= 25.0f && frame < 26.0f) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
        }
        mRot.y += 6.2831855f / (1.0f + mMotion.fn_80097D7C(0));
        RouteTable_unk* table = lbl_8087F430->m10D8;
        atn::Vector3 step(0.0f, 0.0f, 50.0f / mMotion.fn_80097D7C(0));
        RoutePlace_unk* place = table->FindPlace_unk(0x385);
        Mtx m;
        PSMTXRotRad(m, 'y', place->m14);
        PSMTXMultVec(m, &step, &step);
        mPos -= step;
    }
}
