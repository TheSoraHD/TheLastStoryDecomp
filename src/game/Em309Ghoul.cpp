#include "game/Em309Ghoul.h"
#include "atn/Keyboard.h"
#include "atn/Camera_unk.h"
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
extern "C" void fn_80097CCC(atn::Model* model, int track, float value); // inferred: stops a track
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
// inferred: [0,1] float channel to 0..255 (as in atn/D2A_Window.cpp; name invented)
inline unsigned int GhoulToU8_unk(float f) {
    if (f >= 1.0f) {
        return 255;
    }
    if (f <= 0.0f) {
        return 0;
    }
    return (unsigned int)(f * 255.0f + 0.5f);
}
// inferred: RGBA color of the engine (atn::Color4f_unk, see game/Em310Gargoyle.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    unsigned int ToU32() const {
        unsigned int ur = GhoulToU8_unk(r);
        unsigned int ug = GhoulToU8_unk(g);
        unsigned int ub = GhoulToU8_unk(b);
        unsigned int ua = GhoulToU8_unk(a);
        return (ua << 24) | (ur << 16) | (ug << 8) | ub;
    }
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
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
    RouteTable_unk* fn_8013C504(); // inferred: returns m10D8
    unsigned char m00[0x260];
    Camera_unk m260; // inferred: camera of the field (as View_unk m260 of game/CharaTask.cpp)
    unsigned char m454[0x8A0 - 0x454];
    CharaTask* m8A0_unk; // inferred: character the camera follows (as Field_unk::m8A0 in game/Em203GreaterDaemon.cpp)
    unsigned char m8A4[0x10D8 - 0x8A4];
    RouteTable_unk* m10D8; // inferred: route tables (as in gmk/ReinforcePoint.cpp)
};
extern "C" Field_unk* lbl_8087F430;
// inferred: accessors (weak copies of other files, as in game/Em310Gargoyle.cpp and gmk/Cannonball.cpp)
// inferred: empty release asserts (rule of the autoinline threshold, as GARGOYLE_ASSERT_UNK in
// game/Em310Gargoyle.cpp): the original calls every inline helper of fn_802FFFF8 out of line
#define GHOUL_ASSERT_UNK(x) ((void)0)
extern "C" inline Field_unk* fn_80121F00() {
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0); GHOUL_ASSERT_UNK(0);
    return lbl_8087F430;
}
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" inline CharaManager<PlayerTask>* fn_8000D9E8() {
    return lbl_8087F8A0;
}
extern "C" inline CharaTask* fn_802A36B0(CharaManager<PlayerTask>* mgr) {
    return mgr->mHead;
}
extern "C" double sqrt(double x);
// inferred: length in the xz plane (weak copy 0x801C3910)
extern "C" inline float fn_801C3910(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets a counter
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed (as in game/Arms.cpp)
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
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
                m1638[i][j].x = pos.x;
                m1638[i][j].y = pos.z;
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
        m56C = 0.08f;
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

// NONMATCHING: 99.80% - the frame is 0xB0 bytes bigger (slots of the inlined state helpers) and the pool offsets
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
    if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x50)) {
        fn_803010BC();
    }
    if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4D)) {
        fn_8030073C();
    }
    if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4C)) {
        StartAttack_unk();
    }
    if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4A)) {
        fn_803009F4();
    }
    if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x48)) {
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


// NONMATCHING: 99.87% - stack layout of the direction temporaries and of the rotation matrices (~6 variants)
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

// NONMATCHING: 99.57% - r6/r7 of the sum temporaries and the place of the "spine1" address load (scheduled
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

// inferred: movement of the states without their own update: walks the route (state kind 7) or turns to
// the target (kind 3); a sharp turn without speed keeps the position
void Em309Ghoul::fn_802FF800() {
    atn::Vector3 rot = mRot;
    float speed = 0.0f;
    float rate = 1.0f;
    int keep = 0;
    atn::Vector3 pos = mPos;
    atn::Vector3 target = m14EC->mPos;
    atn::Vector3 d = target - mPos;
    float dist = d.Length_unk();
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 step = mRoute.m58;
        speed = step.Length_unk();
        if (speed > 0.0001f) {
            float near = -100.0f;
            if (m14F8 == 2) {
                near = 25.0f;
            }
            if (dist < near) {
                speed = 0.0f;
                rot = d.GetNormalize().ToRotation_unk();
                mRoute.fn_80127D8C(0, 0, 0);
            } else {
                rot = step.GetNormalize().ToRotation_unk();
                if (atn::Abs_unk(rot.y - mRot.y) > 0.31415927f && speed < 0.0001f) {
                    speed = 1.0f;
                    keep = 1;
                }
            }
        } else {
            rot = mRot;
        }
    } else if (m55C == 3) {
        if (m58C == 0) {
            rot = d.GetNormalize().ToRotation_unk();
            if (atn::Abs_unk(rot.y - mRot.y) > 0.31415927f && speed < 0.0001f) {
                speed = 1.0f;
                keep = 1;
            }
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(&rot, 1, speed, rate);
    if (keep) {
        mPos = pos;
    }
}

// NONMATCHING: 99.82% - load order of m1518/m150C[i] in the base compare and fcmpu operand order; ~8 variants
// inferred: chooses the next move (route place or the target) and the attack (states 1, 6, 9)
void Em309Ghoul::fn_802FFFF8() {
    if (m55C == 6 && m560 == 22) {
        return;
    }
    fn_80121F00()->fn_8013C504();
    CharaTask* leader = fn_802A36B0(fn_8000D9E8());
    float homeDist = 1e16f;
    float homeXZ = homeDist;
    float tgtXZ = homeDist;
    float tgtAngle = homeDist;
    float leadXZ = homeDist;
    float leadAngle = homeDist;
    if (m150C != 0 && m14EC != 0) {
        RoutePlace_unk* home = m1518;
        if (home != 0) {
            atn::Vector3 d = *m14EC->fn_8013C38C() - home->mPos;
            d.y = 0.0f;
            homeDist = d.Length_unk();
            atn::Vector3 e = mPos - m1518->mPos;
            homeXZ = fn_801C3910(e);
        }
        if (m14EC != 0) {
            atn::Vector3 d = *m14EC->fn_8013C38C() - mPos;
            tgtXZ = fn_801C3910(d);
            tgtAngle = atn::WrapAngle_unk(d.ToRotation_unk().y - mRot.y);
        }
        if (leader != 0) {
            atn::Vector3 d = *leader->fn_8013C38C() - mPos;
            leadXZ = fn_801C3910(d);
            leadAngle = atn::WrapAngle_unk(d.ToRotation_unk().y - mRot.y);
        }
    }
    if (m14F8 == 0) {
        if (m1520 > m169C) {
            m14F8 = 1;
            return;
        }
    } else if (homeXZ < 10.0f) {
        m14F8 = 2;
    } else if (m15F0 > m16D4) {
        m14F8 = 1;
    }
    int move;
    if (m1668 != 0) {
        move = 0;
    } else if (m14F8 == 0) {
        if (homeXZ > 15.0f) {
            move = 2;
        } else {
            move = 3;
        }
    } else if (m14F8 == 1) {
        move = 0;
        float d1 = (m150C[1]->mPos - *m14EC->fn_8013C38C()).Length_unk();
        if (d1 < (m150C[2]->mPos - *m14EC->fn_8013C38C()).Length_unk()) {
            if (m1518->mId == m150C[1]->mId && homeXZ < 70.0f) {
                move = 1;
                m1518 = m150C[1];
                m151C++;
            } else {
                m151C = 0;
                m1518 = m150C[1];
            }
        } else {
            if (m1518->mId == m150C[2]->mId && homeXZ < 70.0f) {
                move = 1;
                m1518 = m150C[2];
                m151C++;
            } else {
                m151C = 0;
                m1518 = m150C[2];
            }
        }
        if (move != 0) {
            move = 3;
        } else {
            move = 2;
        }
    } else {
        move = m14F8 == 2;
    }
    int attack = 0;
    if (m14F8 == 0) {
        if (m14F4 > m1698 && homeXZ < 20.0f) {
            if (tgtXZ < 0.7f * fn_80219E6C(0x64A)->m58) {
                attack = 1;
            } else if (atn::Abs_unk(tgtAngle) != 0.0f) {
                attack = 3;
            }
        }
    } else if (fn_80304B40()) {
        if (m14EC == leader) {
            if (leadXZ < 40.0f && atn::Abs_unk(leadAngle) < atn::DegToRad_unk(60.0f)) {
                attack = 2;
            }
        } else if (leadXZ < 50.0f && atn::Abs_unk(leadAngle) < atn::DegToRad_unk(60.0f)) {
            attack = 2;
        } else if (homeDist < m1694) {
            if (tgtXZ < 0.7f * fn_80219E6C(0x64A)->m58 && atn::Abs_unk(tgtAngle) < *(float*)&fn_80219E6C(0x64A)->m50[0]) {
                attack = 1;
            }
        } else if (atn::Abs_unk(tgtAngle) < atn::DegToRad_unk(30.0f)) {
            attack = 3;
        }
    } else if (m14F4 > m1698) {
        if (homeDist < m1694) {
            if (tgtXZ < 0.7f * fn_80219E6C(0x64A)->m58 && atn::Abs_unk(tgtAngle) < *(float*)&fn_80219E6C(0x64A)->m50[0]) {
                attack = 1;
            }
        } else if (atn::Abs_unk(tgtAngle) < atn::DegToRad_unk(30.0f)) {
            attack = 3;
        }
    }
    if (attack == 0 && (m55C < 6 || m55C > 7)) {
        if (move == 0) {
            mRoute.fn_80127D8C(0, 0, 0);
        } else if (move == 1) {
            fn_80170A20(m14EC, 0, -1.0f);
        } else if (move == 2) {
            fn_8017039C(m1518->mId, 0);
        } else if (move == 3) {
            fn_8017039C(m1518->mId, 0);
        }
    }
    if (attack > 0) {
        fn_8016E970(3);
    }
    if (attack == 1) {
        fn_80300654();
    } else if (attack == 2) {
        fn_803009F4();
    } else if (attack == 3) {
        fn_8030073C();
    }
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

// inferred: plays effect `efp` on `model` with default placement (name invented)
inline void GhoulEffect_unk(atn::EfpHandle_unk* efp, atn::Model* model) {
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

// inferred: frame parameters (lbl_8087EFA8, as in gmk/SparkCube.cpp)
struct GhoulParams_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" GhoulParams_unk* lbl_8087EFA8;
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: result of the collision tests of lbl_8087EE98 (0x50 bytes, as in gmk/MotionObject.cpp)
struct GhoulCollResult_unk {
    GhoulCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10_unk; // inferred: position after the sweep
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28_unk; // inferred: normal of the hit (as in gmk/Lemon.h)
    void* m34;
    void* m38;
    unsigned int m3C; // inferred: material flags (0x20: wall)
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, GhoulCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: engine camera (lbl_8087EFB4) with the Camera_unk at +0x104 (as in game/EnemyQuarkStand.cpp)
struct GhoulEngineCamera_unk {
    unsigned char m000[0x104];
    Camera_unk m104;
};
extern "C" GhoulEngineCamera_unk* lbl_8087EFB4;
extern "C" double tan(double x);
extern "C" double atan(double x);
// inferred: xsca camera curves (as in game/EnemyQuarkStand.cpp)
struct XsfCurveSet_unk {
    float fn_805BF204(float time, int channel);
    void fn_805BF410(float time, float* out, int x, int y, int z);
};
struct Xsca_unk {
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
};

// inferred: same with the manager passed in (name invented)
inline void GhoulEffect2_unk(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model) {
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
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: same at a world position (name invented)
inline void GhoulEffectAt_unk(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, const atn::Vector3& pos,
                              const atn::Vector3& rot) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// NONMATCHING: 96.49% - register numbering of the implicit Camera_unk copies (as EnemyQuarkStand::fn_8034DE90),
// operand order of the time step sums; ~8 variants
// inferred: death: slow motion, effects and fall, then the event 0xD0; the camera follows the xsca curve
// m1674[m1684] around the ghoul
void Em309Ghoul::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mColli.mFlags &= ~1;
        mFlag25 = 1;
        if (m1434 > 0) {
            m1434--;
            if (m1434 == 60) {
                fn_80239DAC(lbl_8087F3C0, this, 300, 1);
                if (fn_80219E6C(0xFA2) != 0) {
                    fn_80232B7C(0, 0);
                    GhoulEffect2_unk(lbl_8087F3C0, &m1534, &mMotion);
                    fn_800C344C("SE_EM309_001", &mPos, 0, -1, 1.0f);
                    fn_800F8548(lbl_8087F048);
                }
            } else if (m1434 == 50) {
                fn_8016E4C4(0);
            }
        } else {
            fn_80370320(lbl_8087F430, 0xD0, 0, 0);
            lbl_8087EFA8->mTimeScale = 1.0f;
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        lbl_8087EFA8->mTimeScale = 0.5f;
        if (m15B0 > 0 && --m15B0 == 0) {
            fn_80232B7C(this, 300);
            GhoulEffect2_unk(lbl_8087F3C0, &m1528, &mMotion);
        }
        if (m1684 != 0) {
            float f = mMotion.mTracks[0].m08;
            if (f >= 16.0f && f < 17.0f) {
                fn_80232B7C(this, 0x12D);
                GhoulEffectAt_unk(lbl_8087F3C0, &m157C, mPos, lbl_807C7030);
            }
        } else {
            mPos.z = -30.0f;
            mPos.y = 0.0f;
            if (mPos.x < 750.0f) {
                mRot.y = 1.5707964f;
            } else if (mPos.x > 880.0f) {
                mRot.y = -1.5707964f;
            } else {
                float a = fmod(1.5707964f - mRot.y, 6.2831854820251465);
                if (a > 3.1415927f) {
                    a -= 6.2831855f;
                }
                if (a < -3.1415927f) {
                    a += 6.2831855f;
                }
                float b = fmod(-1.5707964f - mRot.y, 6.2831854820251465);
                if (b > 3.1415927f) {
                    b -= 6.2831855f;
                }
                if (b < -3.1415927f) {
                    b += 6.2831855f;
                }
                if (a < b) {
                    mRot.y = 1.5707964f;
                } else {
                    mRot.y = -1.5707964f;
                }
            }
            mRot.x = 0.0f;
            mRot.z = 0.0f;
            fn_80144710();
        }
    }
    XsfCurveSet_unk* curve = ((Xsca_unk*)m1674[m1684].fn_804786F8())->fn_805BDCBC(0);
    Camera_unk camera = lbl_8087F430->m260;
    atn::Vector3 eye;
    atn::Vector3 target;
    curve->fn_805BF410(m1688, &eye.x, 1, 2, 3);
    curve->fn_805BF410(m1688, &target.x, 7, 8, 9);
    atn::Matrix3x4 m(mMotion.mMtx);
    Mtx rz;
    if (m1684 != 0) {
        Mtx r;
        PSMTXRotRad(r, 'y', 3.1415927f);
        Mtx t;
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    PSMTXMultVec(m.m, &eye, &eye);
    PSMTXMultVec(m.m, &target, &target);
    GhoulCollResult_unk res;
    if (fn_8004ED34(lbl_8087EE98, &res, &target, &eye, 0x80000008, 0, 0) != 0) {
        eye.Assign_unk(res.m04);
    } else {
        float f = mMotion.mTracks[0].m08;
        if (f > f) {
            m1688 += lbl_8087EFA8->mTimeScale * 0.5f;
        } else {
            m1688 = m1688 + lbl_8087EFA8->mTimeScale;
        }
    }
    camera.mPos = eye;
    camera.mTarget = target;
    float fov = curve->fn_805BF204(m1688, 0);
    float aspect = camera.mAspect;
    camera.mFovy = 2.0f * (float)atan((float)tan(0.5f * (0.017453292f * fov)) / aspect);
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    atn::Vector3 roll;
    curve->fn_805BF410(m1688, &roll.x, 4, 5, 6);
    PSMTXRotRad(rz, 'z', roll.z * 0.017453292f);
    PSMTXMultVec(rz, &up, &up);
    camera.mUp = up;
    camera.fn_8004B378();
    lbl_8087EFB4->m104 = camera;
}


extern "C" void fn_80063764(void* draw, const atn::Vector3* a, const atn::Vector3* b, unsigned int color,
                            float width); // inferred: line

// inferred: rotates `m` around `axis` when the angle is not zero (as in fn_80302B84; name invented)
inline void GhoulRotate_unk(atn::Matrix3x4& m, char axis, float rad) {
    if (0.0f != rad) {
        Mtx r;
        PSMTXRotRad(r, axis, rad);
        Mtx t;
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// NONMATCHING: 99.59% - GPR numbering (r28/r30 of the -1/0 constants and of the colors) and the load order of
// m16A4 in the second rotation; ~5 variants (helpers, declaration orders)
// inferred: update of state 6: after "ChargeTime" frames plays motion 0x147 with the effects m1540/m1558 and
// shows message 0x14D; the shake (fn_80304FC0) starts at frame 40
void Em309Ghoul::fn_80301DDC() {
    if (m14F0 == 0) {
        if (m14F4 > m16A0) {
            m14F4 = 0;
            m14F0 = 1;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
            fn_80232B7C(this, 0x12E);
            GhoulEffect2_unk(lbl_8087F3C0, &m1540, &mMotion);
            fn_80232B7C(this, 0x140);
            GhoulEffect2_unk(lbl_8087F3C0, &m1558, &mMotion);
            wchar_t text[256];
            memset(text, 0, sizeof(text));
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x14D), *(const wchar_t**)((char*)m60 + 4));
            fn_80109828(lbl_8087F048, text);
        }
        if (mMotion.mTracks[0].m00 == 0x145) {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
            }
        }
    } else if (m14F0 == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            ResetState_unk();
        }
        if (m14F4 == 40) {
            m15B8 = 1;
        }
        if (m1690 != 0) {
            atn::Matrix3x4 ma;
            Mtx ra;
            atn::Matrix3x4 mb;
            Mtx rb;
            Mtx rxa;
            Mtx txa;
            Mtx rya;
            Mtx tya;
            Mtx rza;
            Mtx tza;
            Mtx rxb;
            Mtx txb;
            Mtx ryb;
            Mtx tyb;
            Mtx rzb;
            Mtx tzb;
            atn::Vector3 a(0.0f, 0.0f, m16AC);
            atn::Matrix3x4* pa = &ma;
            pa->SetIdentity();
            if (0.0f != mRot.z) {
                PSMTXRotRad(rza, 'z', mRot.z);
                PSMTXConcat(pa->m, rza, tza);
                __MtxCopy(pa->m, tza);
            }
            if (0.0f != mRot.y) {
                PSMTXRotRad(rya, 'y', mRot.y);
                PSMTXConcat(pa->m, rya, tya);
                __MtxCopy(pa->m, tya);
            }
            if (0.0f != mRot.x) {
                PSMTXRotRad(rxa, 'x', mRot.x);
                PSMTXConcat(pa->m, rxa, txa);
                __MtxCopy(pa->m, txa);
            }
            PSMTXMultVec(ma.m, &a, &a);
            PSMTXRotRad(ra, 'y', 0.017453292f * (-m16A4 * 0.5f));
            PSMTXMultVec(ra, &a, &a);
            atn::Vector3 b(0.0f, 0.0f, m16AC);
            atn::Matrix3x4* pb = &mb;
            pb->SetIdentity();
            if (0.0f != mRot.z) {
                PSMTXRotRad(rzb, 'z', mRot.z);
                PSMTXConcat(pb->m, rzb, tzb);
                __MtxCopy(pb->m, tzb);
            }
            if (0.0f != mRot.y) {
                PSMTXRotRad(ryb, 'y', mRot.y);
                PSMTXConcat(pb->m, ryb, tyb);
                __MtxCopy(pb->m, tyb);
            }
            if (0.0f != mRot.x) {
                PSMTXRotRad(rxb, 'x', mRot.x);
                PSMTXConcat(pb->m, rxb, txb);
                __MtxCopy(pb->m, txb);
            }
            PSMTXMultVec(mb.m, &b, &b);
            PSMTXRotRad(rb, 'y', 0.017453292f * (m16A4 * 0.5f));
            PSMTXMultVec(rb, &b, &b);
            fn_80063764(lbl_8087EEB0, &mPos, &(a + mPos), Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f).ToU32(), 100.0f);
            fn_80063764(lbl_8087EEB0, &mPos, &(b + mPos), Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f).ToU32(), 100.0f);
        }
    }
}

// NONMATCHING: 99.71% - GPR numbering (r26-r29 rotated: bullet manager, 0x43300000, constant address); ~4 variants
// inferred: update of state 8: after "ChargeTime" frames plays motion 0x147, then throws bullets 0x64C along
// the facing (5 units more every frame) with the effect m1558 until frame 30
void Em309Ghoul::fn_803026F0() {
    if (m14F0 == 0) {
        if (m14F4 > m16A0) {
            m14F4 = 0;
            m14F0 = 1;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
        }
        if (mMotion.mTracks[0].m00 == 0x145) {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
            }
        }
    } else if (m14F0 == 1) {
        atn::Matrix3x4 m;
        Mtx rx;
        Mtx tx;
        Mtx ry;
        Mtx ty;
        Mtx rz;
        Mtx tz;
        atn::Vector3 dir(0.0f, 0.0f, 5.0f);
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
        PSMTXMultVec(m.m, &dir, &dir);
        if ((float)m14F4 > 30.0f) {
            m14F4 = 0;
            fn_80239DAC(lbl_8087F3C0, this, m15B4, 0);
            m15B4 = 1000;
            ResetState_unk();
            return;
        }
        Effect_unk* mgr = lbl_8087F048;
        atn::Vector3 pos = mPos + dir * (float)m14F4;
        int id = fn_800F8548(mgr);
        fn_800FAB80(mgr, this, fn_80219E6C(0x64C), id, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        fn_80239DAC(lbl_8087F3C0, this, m15B4, 0);
        m15B4++;
        fn_80232B7C(this, m15B4);
        GhoulEffectAt_unk(lbl_8087F3C0, &m1558, mPos + dir * (float)m14F4, mRot);
    }
}

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


extern "C" int fn_8004D388(void* world, GhoulCollResult_unk* result, CharaTask::Sphere_unk* sphere,
                           atn::Vector3* move, unsigned int mask, void* ignore, int arg); // inferred: sweep test
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
// inferred: camera shake of the field (lbl_8087F430 + 0x96C, as in game/Em301LShade.cpp)
struct GhoulShake_unk {
    int m96C;
    int m970; // inferred: shake frames
    float m974; // inferred: shake strength
    float m978;
};
inline void GhoulShake_unk_Set(int frames, float a, float b) {
    GhoulShake_unk* shake = (GhoulShake_unk*)((char*)lbl_8087F430 + 0x96C);
    shake->m96C = shake->m96C % 2;
    shake->m970 = frames;
    shake->m974 = a;
    shake->m978 = b;
}

// NONMATCHING: 95.62% - stack slots of the temporaries and matrices, FPR/GPR numbering, the dead read of m15C8 at
// the start and the scheduling of the wall ray test; ~6 variants
// inferred: update of state 10: rolls (m15D8, "DamageRollingSpeed"/"DamageRollingRot") and bounces on the walls
// (from the fifth bounce the direction turns towards m1608); after five bounces near m1608 it jumps back into
// place m1600 (m14F0 1, then fn_803010BC); bullets 0x64D from the hips after "NoHitFrame"
void Em309Ghoul::fn_80302F94() {
    atn::Matrix3x4 m1;
    atn::Matrix3x4 m2;
    atn::Matrix3x4 m3;
    Mtx axisRot;
    Mtx rx1;
    Mtx tx1;
    Mtx ry1;
    Mtx ty1;
    Mtx rz1;
    Mtx tz1;
    Mtx rx2;
    Mtx tx2;
    Mtx ry2;
    Mtx ty2;
    Mtx rz2;
    Mtx tz2;
    int roll = 1;
    lbl_8087F430->m8A0_unk = this;
    float rotSpeed = m16C0;
    float h = 0.0f;
    if (m14F0 == 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mRot = m15CC;
            m15D8.Set_unk(0.0f, 0.0f, h);
            atn::Matrix3x4* p1 = &m1;
            p1->SetIdentity();
            if (0.0f != mRot.z) {
                PSMTXRotRad(rz1, 'z', mRot.z);
                PSMTXConcat(p1->m, rz1, tz1);
                __MtxCopy(p1->m, tz1);
            }
            if (0.0f != mRot.y) {
                PSMTXRotRad(ry1, 'y', mRot.y);
                PSMTXConcat(p1->m, ry1, ty1);
                __MtxCopy(p1->m, ty1);
            }
            if (0.0f != mRot.x) {
                PSMTXRotRad(rx1, 'x', mRot.x);
                PSMTXConcat(p1->m, rx1, tx1);
                __MtxCopy(p1->m, tx1);
            }
            PSMTXMultVec(m1.m, &m15D8, &m15D8);
            m14F0 = 0;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14B, 1, 0, 1, 0.0f, 0.1f);
            m14F4 = 0;
        }
        float scale = 1.0f;
        if (mMotion.mTracks[0].m00 == 0x14A) {
            float a = fmod(m15CC.y - mRot.y, 6.2831854820251465);
            if (a > 3.1415927f) {
                a -= 6.2831855f;
            }
            if (a < -3.1415927f) {
                a += 6.2831855f;
            }
            if (a > 0.0f) {
                mRot.y -= a / mMotion.fn_80097D7C(0);
            } else {
                mRot.y += a / mMotion.fn_80097D7C(0);
            }
            if (mRot.y < a / mMotion.fn_80097D7C(0) && mRot.y > -1.0f * (a / mMotion.fn_80097D7C(0))) {
                mRot.y = m15CC.y;
            }
            float f = mMotion.mTracks[0].m08;
            if (f >= 7.99999f) {
                scale = (f - 8.0f) / (mMotion.fn_80097D7C(0) - 8.0f);
            } else {
                scale = 0.0f;
            }
            if (h < 10.0f) {
                h += 10.0f / mMotion.fn_80097D7C(0);
            } else {
                h = 10.0f;
            }
        } else {
            h = 10.0f;
        }
        m15D8.Set_unk(0.0f, 0.0f, h);
        atn::Vector3 yaw(0.0f, mRot.y, 0.0f);
        atn::Matrix3x4* p2 = &m2;
        p2->SetIdentity();
        if (0.0f != yaw.z) {
            PSMTXRotRad(rz2, 'z', yaw.z);
            PSMTXConcat(p2->m, rz2, tz2);
            __MtxCopy(p2->m, tz2);
        }
        if (0.0f != yaw.y) {
            PSMTXRotRad(ry2, 'y', yaw.y);
            PSMTXConcat(p2->m, ry2, ty2);
            __MtxCopy(p2->m, ty2);
        }
        if (0.0f != yaw.x) {
            PSMTXRotRad(rx2, 'x', yaw.x);
            PSMTXConcat(p2->m, rx2, tx2);
            __MtxCopy(p2->m, tx2);
        }
        PSMTXMultVec(m2.m, &m15D8, &m15D8);
        int hit = 0;
        GhoulCollResult_unk res;
        CharaTask::Sphere_unk sphere;
        sphere.mCenter = mPos;
        sphere.mCenter.y += 16.0f * (1.0f - scale);
        sphere.mRadius = m16E0;
        if (fn_8004D388(lbl_8087EE98, &res, &sphere, &m15D8, 0x80000000, &mColli, 0) != 0 && (res.m3C & 0x20) == 0x20) {
            fn_800C344C("SE_EM309_016", &res.m10_unk, 0, -1, 1.0f);
            atn::Vector3 dir;
            atn::Vector3 n = m15D8;
            n.Normalize_unk();
            GhoulCollResult_unk wall;
            if (fn_8004ED34(lbl_8087EE98, &wall, &sphere.mCenter, &(sphere.mCenter + n * 50.0f), 0x80000000, 0, 0) !=
                0) {
                dir = m15D8;
                dir.y = 0.0f;
                PSMTXRotAxisRad(axisRot, &wall.m28_unk, 3.1415927f);
                PSMTXMultVec(axisRot, &dir, &dir);
                dir = atn::Vector3(-dir.x, -dir.y, -dir.z);
                dir.Normalize_unk();
            } else {
                dir = res.m10_unk - (sphere.mCenter + m15D8);
                dir.y = 0.0f;
                dir.Normalize_unk();
            }
            mPos = res.m10_unk;
            hit = 1;
            mPos.y -= 16.0f * (1.0f - scale);
            atn::Vector3 home(m1608.x - mPos.x, 0.0f, m1608.z - mPos.z);
            home.Normalize_unk();
            if (m15E4 >= 5) {
                float dot = PSVECDotProduct(&home, &dir);
                if (dot > 0.9659258f) {
                    dir = home;
                } else if (dot < 0.0f) {
                    dir = atn::Lerp_unk(dir, atn::Vector3(-home.x, -home.y, -home.z), (float)(m15E4 - 5) / 5.0f);
                } else {
                    dir = atn::Lerp_unk(dir, home, (float)(m15E4 - 5) / 5.0f);
                }
            }
            atn::Vector3 r = dir.ToRotation_unk();
            m15CC.y = r.y;
            mRot = m15CC;
            m15CC.y = mRot.y;
            atn::Vector3 yaw3(0.0f, mRot.y, 0.0f);
            Mtx rx3;
            Mtx tx3;
            Mtx ry3;
            Mtx ty3;
            Mtx rz3;
            Mtx tz3;
            atn::Matrix3x4* p3 = &m3;
            p3->SetIdentity();
            if (0.0f != yaw3.z) {
                PSMTXRotRad(rz3, 'z', yaw3.z);
                PSMTXConcat(p3->m, rz3, tz3);
                __MtxCopy(p3->m, tz3);
            }
            if (0.0f != yaw3.y) {
                PSMTXRotRad(ry3, 'y', yaw3.y);
                PSMTXConcat(p3->m, ry3, ty3);
                __MtxCopy(p3->m, ty3);
            }
            if (0.0f != yaw3.x) {
                PSMTXRotRad(rx3, 'x', yaw3.x);
                PSMTXConcat(p3->m, rx3, tx3);
                __MtxCopy(p3->m, tx3);
            }
            m15D8.Set_unk(0.0f, 0.0f, 25.0f + h);
            PSMTXMultVec(m3.m, &m15D8, &m15D8);
            m1618 = res.m10_unk;
            m15F4 = m15D8;
            GhoulShake_unk_Set(1, 0.5f, 0.5f);
            m15E4++;
        }
        if (m15E4 > 5 && roll != 0) {
            atn::Vector3 d = m1608 - mPos;
            if (fn_801C3910(d) < 30.0f) {
                m14F4 = 0;
                atn::Vector3 c = m1608;
                atn::Vector3 rr(0.0f, 0.0f, 0.0f);
                if (m1600 != 0) {
                    rr.Set_unk(rr.x, 3.1415927f + rr.y, rr.z);
                }
                m15CC = rr - mRot;
                float a = fmod(m15CC.y, 6.2831854820251465);
                if (a > 3.1415927f) {
                    a -= 6.2831855f;
                }
                if (a < -3.1415927f) {
                    a += 6.2831855f;
                }
                m15D8 = (c - mPos) * 0.04761905f;
                m15CC.y = a;
                m15D8.y = 0.0f;
                m14F0 = 1;
                GhoulShake_unk_Set(30, 1.0f, 0.8f);
                m14F4 = 0;
                m15E4 = 0;
            } else {
                if (hit == 0) {
                    mPos += m15D8;
                }
                mRot.x += atn::DegToRad_unk(rotSpeed);
            }
        } else {
            if (hit == 0) {
                mPos += m15D8;
            }
            mRot.x += atn::DegToRad_unk(rotSpeed);
        }
        mPos.y = 16.0f * scale;
        if (mMotion.mTracks[0].m00 == 0x14A) {
            mRot.x = 0.0f;
        }
        if (m14F4 > m16E4) {
            atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
            Effect_unk* mgr = lbl_8087F048;
            fn_800FAB80(mgr, this, fn_80219E6C(0x64D), 1000, &pos, &mRot, 0, m16E8, -1, -1, 0.0f, 1.0f);
        }
    } else if (m14F0 == 1) {
        mPos = m1608;
        mRot.y = 3.1415927f + m1600->m14;
        mRot.x = 0.0f;
        mRot.z = 0.0f;
        mPos.y += 16.0f;
        fn_803010BC();
        fn_80239DAC(lbl_8087F3C0, this, 400, 1);
        fn_80232B7C(this, 0x130);
        GhoulEffect_unk(&m1564, m1614->vf38());
        fn_80370AE4(lbl_8087F430, 0xA4, 0);
    }
}

// inferred: input of the field and its rumble (weak copies 0x80122550 and 0x8028CFB8, as in
// game/Em203GreaterDaemon.cpp)
struct GhoulInput_unk {
    unsigned char m000[0x900];
    int m900;
    int m904;
    float m908;
    float m90C;
};
extern "C" inline GhoulInput_unk* fn_80122550(Field_unk* field) {
    return (GhoulInput_unk*)((char*)field + 0x6C);
}
extern "C" inline void fn_8028CFB8(GhoulInput_unk* input, int frames, float a, float b) {
    input->m904 = frames;
    input->m908 = a;
    input->m900 = input->m900 % 2;
    input->m90C = b;
}
// inferred: rotation matrix from euler angles, z then y then x (weak copy 0x800109E0, as in
// game/Em203GreaterDaemon.cpp)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
// inferred: managers (weak copies 0x800F7FA0 and 0x8013A194 of other files)
extern "C" inline EffectMgr_unk* fn_800F7FA0() {
    return lbl_8087F3C0;
}
inline Effect_unk* fn_8013A194() {
    return lbl_8087F048;
}

// NONMATCHING: 99.96% - only the order of 8.0f and 7.99999f in .sdata2 (their first use is here in the original)
// inferred: update of state 11: rises out of place m1600, then rolls (m15D8, "AttackRollingSpeed") between
// m1608 and m1604 shooting bullets 0x64D from the hips; after three passes it rolls back to its base
void Em309Ghoul::fn_803041C8() {
    float rotSpeed = m16C8;
    atn::Vector3 target(0.0f, 0.0f, 0.0f);
    if (m1604 != 0) {
        target = m1604->mPos;
    }
    atn::Vector3 toTarget = (target - m1608).GetNormalize().ToRotation_unk();
    atn::Vector3 toCenter = (m1608 - target).GetNormalize().ToRotation_unk();
    if (m14F0 == 0) {
        mRot.y = m1600->m14;
        float t;
        if (mMotion.GetMotionFrame(0) >= 7.99999f) {
            float len = mMotion.fn_80097D7C(0) - 8.0f;
            t = (mMotion.GetMotionFrame(0) - 8.0f) / len;
        } else {
            t = 0.0f;
        }
        mPos.y += 16.0f * t;
        float end = mMotion.fn_80097D7C(0);
        if (mMotion.GetMotionFrame(0) >= end) {
            mMotion.fn_80139F24(1);
            mMotion.fn_8013C554(0, 1.0f);
            mMotion.SetMotionSpeed(0, 1.0f);
            mMotion.fn_80097C08(0, 0x14B, 1, 0, 1, 0.0f, 0.1f);
            atn::Vector3 r1 = (target - mPos).GetNormalize().ToRotation_unk();
            atn::Vector3 r2 = (m1608 - mPos).GetNormalize().ToRotation_unk();
            m1624 = 2;
            mRot.y = r1.y;
            atn::Vector3 rot(mRot);
            rot.x = 0.0f;
            m15D8.Set_unk(0.0f, 0.0f, m16C4);
            m15D8.fn_80011410(fn_800109E0(rot));
            m1518 = m150C[m1628];
            if (m1628 == 1) {
                m1628 = 2;
            } else {
                m1628 = 1;
            }
            mPos.y = 16.0f;
            m14F0 = 1;
            fn_8028CFB8(fn_80122550(fn_80121F00()), 1, 0.5f, 0.5f);
        }
    } else if (m14F0 == 1) {
        int turned = 0;
        mPos += m15D8;
        mPos.y = 16.0f;
        mRot.x += atn::DegToRad_unk(rotSpeed);
        if (m1624 == 0) {
            atn::Vector3 d = m1518->mPos - mPos;
            if (fn_801C3910(d) < 20.0f) {
                mPos = m1518->mPos;
                mRot.x = 0.0f;
                fn_80301200();
                fn_80239DAC(fn_800F7FA0(), this, 400, 1);
                fn_80370AE4(fn_80121F00(), 0xA4, 0);
            }
        } else if (m1624 == 1) {
            atn::Vector3 d = m1608 - mPos;
            if (fn_801C3910(d) < 20.0f) {
                mRot.y = toTarget.y;
                mPos = m1608;
                m1624 = 2;
                turned = 1;
                m15E4++;
            }
        } else if (m1624 == 2) {
            atn::Vector3 d = target - mPos;
            if (fn_801C3910(d) < 20.0f) {
                mRot.y = toCenter.y;
                mPos = target;
                m1624 = 1;
                turned = 1;
                m15E4++;
            }
        }
        if (m15E4 > 2) {
            atn::Vector3 d = m1518->mPos - mPos;
            mRot.y = d.GetNormalize().ToRotation_unk().y;
            turned = 1;
            m1624 = 0;
            m15E4 = 0;
        }
        if (turned) {
            fn_800C344C("SE_EM309_016", &mPos, 0, -1, 1.0f);
            atn::Vector3 rot(mRot);
            rot.x = 0.0f;
            m15D8.Set_unk(0.0f, 0.0f, m16C4);
            m15D8.fn_80011410(fn_800109E0(rot));
            m15D8.y = 0.0f;
            fn_8028CFB8(fn_80122550(fn_80121F00()), 1, 0.5f, 0.5f);
        }
        atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
        fn_800FAB80(fn_8013A194(), this, fn_80219E6C(0x64D), 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

// NONMATCHING: 99.79% - registers of the inlined FindPlace_unk loop (index/offset swapped) and the offset of
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


// inferred: side of `p` relative to the line o->e (2D cross product; name invented)
inline float GhoulSide_unk(const atn::Vector2_unk& p, const atn::Vector2_unk& o, const atn::Vector2_unk& e) {
    atn::Vector2_unk d1(p.x - o.x, p.y - o.y);
    atn::Vector2_unk d2(e.x - o.x, e.y - o.y);
    return d2.x * d1.y - d2.y * d1.x;
}

// NONMATCHING: 97.97% - FPR coloring (constants f23-f27 vs positions f28-f31 in the original); ~10 variants
// inferred: the segment from the ghoul to the party leader crosses one of the two segments m1638 (x/z)
int Em309Ghoul::fn_80304B40() {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    atn::Vector2_unk self(mPos.x, mPos.z);
    atn::Vector2_unk lead(0.0f, 0.0f);
    if (leader == 0) {
        return 0;
    }
    lead.x = leader->mPos.x;
    lead.y = leader->mPos.z;
    for (int i = 0; i < 2; i++) {
        if (GhoulSide_unk(self, m1638[i][0], m1638[i][1]) * GhoulSide_unk(lead, m1638[i][0], m1638[i][1]) < 0.0f &&
            GhoulSide_unk(m1638[i][1], self, lead) * GhoulSide_unk(m1638[i][0], self, lead) < 0.0f) {
            m1658 = leader;
            return 1;
        }
        if (m1690 != 0) {
            atn::Vector3 lines[2][2];
            for (int j = 0; j < 2; j++) {
                atn::Vector3* l = lines[j];
                l[0].Set_unk(m1638[j][0].x, 30.0f, m1638[j][0].y);
                l[1].Set_unk(m1638[j][1].x, 30.0f, m1638[j][1].y);
                fn_80063764(lbl_8087EEB0, &l[0], &l[1], Color4f_unk(255.0f, 0.0f, 0.0f, 255.0f).ToU32(),
                            5000.0f);
                fn_80063764(lbl_8087EEB0, &leader->mPos, &mPos, Color4f_unk(0.0f, 0.0f, 255.0f, 255.0f).ToU32(),
                            1000.0f);
            }
        }
    }
    return 0;
}

// NONMATCHING: 99.23% - GPR numbering (r29/r30 of the loop counter and the matrix pointer, r5-r10 of the copy
// addresses); ~6 variants
// inferred: shake (from frame 40 of state 6, 90 frames): three bullets 0x64C fan out from the ghoul (facing and
// +-7.5 degrees), each frame one step (10 units) further; stops the effects 1000..m15B4 and 0x66 at the end
void Em309Ghoul::fn_80304FC0() {
    if (m15BC > 90) {
        m15BC = 0;
        m15B8 = 0;
        for (int i = 1000; i < m15B4; i++) {
            fn_80239DAC(lbl_8087F3C0, this, i, 0);
        }
        fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
        m15B4 = 1000;
        return;
    }
    atn::Matrix3x4 m0;
    atn::Matrix3x4 m1;
    atn::Matrix3x4 m2;
    Mtx rx0;
    Mtx tx0;
    Mtx ry0;
    Mtx ty0;
    Mtx rz0;
    Mtx tz0;
    Mtx rx1;
    Mtx tx1;
    Mtx ry1;
    Mtx ty1;
    Mtx rz1;
    Mtx tz1;
    Mtx rx2;
    Mtx tx2;
    Mtx ry2;
    Mtx ty2;
    Mtx rz2;
    Mtx tz2;
    atn::Vector3 d0(0.0f, 0.0f, 10.0f);
    atn::Vector3 d1(0.0f, 0.0f, 10.0f);
    atn::Vector3 d2(0.0f, 0.0f, 10.0f);
    atn::Matrix3x4* p0 = &m0;
    p0->SetIdentity();
    if (0.0f != mRot.z) {
        PSMTXRotRad(rz0, 'z', mRot.z);
        PSMTXConcat(p0->m, rz0, tz0);
        __MtxCopy(p0->m, tz0);
    }
    if (0.0f != mRot.y) {
        PSMTXRotRad(ry0, 'y', mRot.y);
        PSMTXConcat(p0->m, ry0, ty0);
        __MtxCopy(p0->m, ty0);
    }
    if (0.0f != mRot.x) {
        PSMTXRotRad(rx0, 'x', mRot.x);
        PSMTXConcat(p0->m, rx0, tx0);
        __MtxCopy(p0->m, tx0);
    }
    PSMTXMultVec(m0.m, &d0, &d0);
    atn::Vector3 rot1(mRot);
    rot1.y += 0.1308997f;
    atn::Matrix3x4* p1 = &m1;
    p1->SetIdentity();
    if (0.0f != rot1.z) {
        PSMTXRotRad(rz1, 'z', rot1.z);
        PSMTXConcat(p1->m, rz1, tz1);
        __MtxCopy(p1->m, tz1);
    }
    if (0.0f != rot1.y) {
        PSMTXRotRad(ry1, 'y', rot1.y);
        PSMTXConcat(p1->m, ry1, ty1);
        __MtxCopy(p1->m, ty1);
    }
    if (0.0f != rot1.x) {
        PSMTXRotRad(rx1, 'x', rot1.x);
        PSMTXConcat(p1->m, rx1, tx1);
        __MtxCopy(p1->m, tx1);
    }
    PSMTXMultVec(m1.m, &d1, &d1);
    atn::Vector3 rot2(mRot);
    rot2.y += -0.1308997f;
    atn::Matrix3x4* p2 = &m2;
    p2->SetIdentity();
    if (0.0f != rot2.z) {
        PSMTXRotRad(rz2, 'z', rot2.z);
        PSMTXConcat(p2->m, rz2, tz2);
        __MtxCopy(p2->m, tz2);
    }
    if (0.0f != rot2.y) {
        PSMTXRotRad(ry2, 'y', rot2.y);
        PSMTXConcat(p2->m, ry2, ty2);
        __MtxCopy(p2->m, ty2);
    }
    if (0.0f != rot2.x) {
        PSMTXRotRad(rx2, 'x', rot2.x);
        PSMTXConcat(p2->m, rx2, tx2);
        __MtxCopy(p2->m, tx2);
    }
    PSMTXMultVec(m2.m, &d2, &d2);
    // inferred: d1 and d2 are scaled from d0 (the rotated d1/d2 are lost; as in the original)
    d0 = d0 * (float)m15BC;
    d1 = d0 * (float)m15BC;
    d2 = d0 * (float)m15BC;
    d0.y = 10.0f;
    d1.y = 10.0f;
    d2.y = 10.0f;
    fn_800FAB80(fn_8013A194(), this, fn_80219E6C(0x64C), 2000, &(mPos + d0), &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    fn_800FAB80(fn_8013A194(), this, fn_80219E6C(0x64C), 2000, &(mPos + d1), &rot1, 0, 30, -1, -1, 0.0f, 1.0f);
    fn_800FAB80(fn_8013A194(), this, fn_80219E6C(0x64C), 2000, &(mPos + d2), &rot2, 0, 30, -1, -1, 0.0f, 1.0f);
    if (m15BC % 2 == 0) {
        fn_80239DAC(lbl_8087F3C0, this, m15B4 - 3, 1);
    }
    m15BC++;
}

// inferred: debug menu items (atn::Mindows, see gmk/FloatingBox.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

// inferred: "AllDirCharaNum" is added twice in the original
void Em309Ghoul::fn_803057A8() {
    void* group = m168C.Load("em309_ghoul");
    fn_80087994(group, "DebugDisp", &m1690, 0, 0);
    fn_800874C8(group, "WaitTime", &m1698, 0, 3000, 30, 0, 0);
    fn_800874C8(group, "StayTime", &m169C, 0, 9999 * 30, 30, 0, 0);
    fn_8008771C(group, "BaseRadius", &m1694, 0, 0, 10.0f, 1000.0f, 10.0f);
    fn_800874C8(group, "ChargeTime", &m16A0, 0, 3000, 30, 0, 0);
    fn_8008771C(group, "ShakeRot", &m16A4, 0, 0, 0.0f, 180.0f, 10.0f);
    fn_8008771C(group, "ShakeGap", &m16A8, 0, 0, 0.0f, 180.0f, 10.0f);
    fn_8008771C(group, "ShakeLength", &m16AC, 0, 0, 0.0f, 300.0f, 10.0f);
    fn_800874C8(group, "AllDirCharaNum", &m16B0, 0, 5, 1, 0, 0);
    fn_800874C8(group, "AllDirCharaNum", &m16B0, 0, 5, 1, 0, 0);
    fn_8008771C(group, "AttackLenge", &m16B8, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(group, "DamageRollingSpeed", &m16BC, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(group, "DamageRollingRot", &m16C0, 0, 0, 0.0f, 180.0f, 0.1f);
    fn_8008771C(group, "AttackRollingSpeed", &m16C4, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(group, "AttackRollingRot", &m16C8, 0, 0, 0.0f, 180.0f, 0.1f);
    fn_80087994(group, "AttackKey", &m16CC, 0, 0);
    fn_800874C8(group, "RollTime", &m16D0, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "AttackTime", &m16D4, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "RollWait", &m16D8, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "FitTime", &m16DC, 0, 99, 1, 0, 0);
    fn_8008771C(group, "ColliRadius", &m16E0, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_800874C8(group, "NoHitFrame", &m16E4, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "HitLag", &m16E8, 0, 9999, 1, 0, 0);
    fn_8008771C(group, "headSpeareRadius", &m16EC, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(group, "headSpeareOfs", &m16F0, 0, 0, 0.0f, 100.0f, 0.1f);
    fn_8008771C(group, "JumpTgtOfsetY", &m16F4, 0, 0, 0.0f, 100.0f, 0.1f);
}

void Em309Ghoul::vf14() {
    BattleCharaTask::vf14();
    if (m1690 != 0) {
        fn_800638B0(lbl_8087EEB0, 1000.0f, -1.0f, &mPos, &(m15D8 * 100.0f + mPos), 0xFF0000FF);
        fn_80063D3C(lbl_8087EEB0, &m1608, m16E0, -1, 1000.0f);
    }
}
