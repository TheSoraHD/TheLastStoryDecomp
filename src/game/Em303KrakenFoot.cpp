#include "game/Em303KrakenFoot.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id

Em303KrakenFoot::Em303KrakenFoot(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C8(20.0f), m14CC(0), m14D0(0), m14D8(0),
      m14DC(0), m14E0(0), m14E4(0.0f), m14E8(0), m14EC(0), m14F0(-1), m14F4(0), m1504(0), m150C(0) {
    mStatus.mParamB.m9C |= 0x200;
    char path[0x100];
    const char* name = strstr(info->m2C, "db_name=");
    if (name != 0) {
        sprintf(path, "/database/chara/special/%s\0", name + 8);
    } else {
        sprintf(path, "/database/chara/special/db_em303_00_00.csv\0");
    }
    m14B0.vf0C(path);
    m14F8.fn_8023780C("/effect/ef_bd006.efp");
}

Em303KrakenFoot::~Em303KrakenFoot() {}

extern "C" int fn_802E2328(Em303KrakenFoot* foot) {
    if ((foot->mStatus.m0C & 0x20) == 0x20) {
        return 1;
    }
    bool r = false;
    if (foot->m14C0 != 1 && foot->m14C0 != 2) {
        r = true;
    }
    return r;

}

// inferred: names invented
inline bool InRange_unk(float v, float lo, float hi) {
    return lo < v && v < hi;
}
inline float GetFrame_unk(Em303KrakenFoot* foot) {
    return foot->mMotion.mTracks[0].m08;
}

extern "C" float fn_802E2360(Em303KrakenFoot* foot) {
    float result = 0.0f;
    atn::Vector3 d = lbl_8087F8A0->mHead->mPos - foot->mPos;
    float rate = (60.0f - PSVECMag(&d)) / 60.0f;
    if (rate > 0.0f) {
        int s = foot->m58C;
        if (s == 2 && foot->mMotion.mTracks[0].m00 == 0x2F && 10.0f < GetFrame_unk(foot) &&
            GetFrame_unk(foot) < 20.0f) {

            result += 0.5f * rate;
        }
        if (s == 13 && foot->mMotion.mTracks[0].m08 >= 25.0f && foot->mMotion.mTracks[0].m08 <= 34.0f) {
            result += 0.5f * rate;
        }
    }
    return result;
}

extern "C" void fn_802E2468(Em303KrakenFoot* foot, int value) {
    foot->m1504 = value;
}

// NONMATCHING: 99.7% - the flags 0x2A (status) / 0x200 (m54C): load and store order (as in
// Em303KrakenFootMp::vf0C)
int Em303KrakenFoot::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B0.fn_80473F50() != 0) {
        return 0;
    }
    if (m14F8.fn_80237874() != 0) {
        return 0;
    }
    fn_80087994(m1508.Load("Em303KrakenFoot"), "DebugDisp", &m150C, 0, 0);
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    mStatus.m18 |= 0x1C0;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC21D;
    mStatus.m18 |= 0x3800000;
    fn_802E2640((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    m14D4 = mMotion.GetNodeMtx_unk("catch_point");
    m54C |= 0x200;
    mStatus.mParamB.m9C |= 0x2A;
    mRoute.mA0 = 1;
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    return 1;
}

void Em303KrakenFoot::vf14() {
    fn_80149A30();
}

void Em303KrakenFoot::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 7 || m58C == 10) {
        return;
    }
    if (msg->m44 == 1 && msg->mData != 0 && msg->mData->m04 != 0x1FB && msg->mData->m04 != 0x1FC) {
        return;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em303KrakenFoot::fn_802E554C(int arg) {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    m14EC = arg;
}

void Em303KrakenFoot::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 30;
    }
}

void Em303KrakenFoot::vf24() {
    if (m58C == 11) {
        return;
    }
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 1.0f;
}
