#include "game/Em302Kraken.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/Em303KrakenFoot.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a battle message
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: camp message table (lbl_8087F1E4, as in game/Em309Ghoul.cpp)
struct KrakenMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" KrakenMsgTable_unk* lbl_8087F1E4;
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)

// inferred: commands of "act_command" ({name, id}, terminated by a negative id; names invented)
struct KrakenCommandName_unk {
    char mName[0x40];
    int mId;
};
static const KrakenCommandName_unk sCommands_unk[] = {
    {"swing", 12}, {"pound", 13}, {"stick", 14}, {"wait", 15}, {"", -1},
};

// inferred: id of the command `name`, -1 if unknown
static int fn_802D73E4(const char* name) {
    for (int i = 0; sCommands_unk[i].mId >= 0; i++) {
        if (strcmp(name, sCommands_unk[i].mName) == 0) {
            return sCommands_unk[i].mId;
        }
    }
    return -1;
}

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 98% - registers of the second std::string constructor (r26/r27, as in
// game/Em201ZanGurg.cpp) and the load order of the eight vector clears
Em302Kraken::Em302Kraken(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14B8(2), m14BC(2), m14C0(0), m14C4(0), m14C8(0),
      m14CC(-70.0f), m14D0(0), m14D4(0), m14D8(10), m14DC(-1), m14F8(0x450), m1514(0.5f), m1518(0.0f),
      m151C(0.0f), m1520(0.0f), m1560(0), m157C(0), m1580(0), m1584(0x712), m15C8(0.0f, 0.0f, 0.0f), m15D4(0), m15D8(-1),
      m15DC(1420.0f), m15E0(20.0f), m15E4(-1720.0f), m1600(0), m1604(0), m1608(0x717), m1630(0), m1634(0),
      m1638(0), m1654(0), m1658(0), m165C(0), m169C(0), m16A8(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x210;
    std::string dir("database/chara/special/");
    std::string name("db_em302_00_00.csv");
    atn::Csv csv(info->m2C, strlen(info->m2C));
    do {
        const char* s = csv.fn_8005B9CC();
        if (s == 0 || strcmp(s, "") == 0) {
            break;
        }
        if (strncmp(s, "db_name=", 8) == 0) {
            name = s + 8;
        }
    } while (csv.fn_8005BBF8());
    std::string path = fn_8006AFF8(dir, name);
    m1684.vf0C(path.c_str());
    m14E0.fn_8023780C("/effect/ef_bs033a.efp");
    m14EC.fn_8023780C("/effect/ef_bs033c.efp");
    m1508.fn_8023780C("/effect/ef_bs030c.efp");
    m15F4.fn_8023780C("/effect/ef_bd004.efp");
    m1524.fn_80237654("/effect/ef_bs034b_01.eff");
    m1530.fn_8023780C("/effect/ef_bs034c.efp");
    m153C.fn_8023780C("/effect/ef_bs034a.efp");
    m1548.fn_8023780C("/effect/ef_bs034e.efp");
    m1564.fn_8023780C("/effect/ef_bu022.efp");
    m1570.fn_8023780C("/effect/ef_bu023.efp");
    m158C.fn_8023780C("/effect/ef_bs028.efp");
    m1598.fn_8023780C("/effect/ef_bs029a.efp");
    m15A4.fn_8023780C("/effect/ef_bs029c.efp");
    m15E8.fn_8023780C("/effect/ef_bd005.efp");
    m160C.fn_8023780C("/effect/ef_bs081a.efp");
    m1618.fn_8023780C("/effect/ef_bs081b.efp");
    m1624.fn_8023780C("/effect/ef_bs049c.efp");
    m1554.clear();
    m163C.clear();
    m1648.clear();
    m1660.clear();
    m166C.clear();
    m1678.clear();
    m168C.clear();
    m15BC.clear();
    m1698 = 0;
}

Em302Kraken::~Em302Kraken() {}

void Em302Kraken::vf14() {
    if (m1560 != 0) {
        return;
    }
    fn_80149A30();
}

void Em302Kraken::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m50 |= 0x10;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    vf40(hit);
}

void Em302Kraken::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_802DC618();
    }
}

void Em302Kraken::vfEC() {
    if (mMotion.mTracks[0].m08 <= 0.0f) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        fn_800EE360(this);
        mFlag15 = 1;
        fn_801765D8();
    }
}

void Em302Kraken::fn_802DC5B4(int arg) {
    m14C8 = 0;
    m14C4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    m1600 = arg;
}

void Em302Kraken::fn_802DDBB0() {
    fn_80087994(m16A4.Load("Em302Kraken"), "DebugDisp", &m16A8, 0, 0);
}

void Em302Kraken::vf20() {
    m620 = m5B0;
    MtxPtr head = mMotion.GetNodeMtx_unk("head");
    m614 = GetNodePos_unk(head) + m5A4;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += m5B4;
    float r = m620;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = r;
}

void Em302Kraken::vf54() {
    if (m624.mSize != 0) {
        MtxPtr head = mMotion.GetNodeMtx_unk("head");
        m624.mData[0].m04.mCenter = GetNodePos_unk(head) + m5A4;
    }
}

atn::Vector3 Em302Kraken::vfD0(float angle) {
    atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("head")) + m5A4;
    pos.y += 25.0f;
    return pos;
}

atn::Vector3 Em302Kraken::vfD8(CharaTask* target) {
    atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("head")) + m5A4;
    pos.y += 18.0f;
    return pos;
}

void Em302Kraken::fn_802DDA90() {
    for (unsigned int i = 0; i < m1678.size(); i++) {
        Em303KrakenFoot* foot = m1678[i];
        if ((foot->mStatus.m0C & 0x20) == 0x20 || foot->mC58.mC0 != 1) {
            foot->mFlag15 = 0;
            foot->mFlags &= ~4;
            foot->fn_800D246C(0);
            if (foot->mFlag25) {
                foot->mFlag25 = 0;
            }
            foot->mStatus.fn_8012D8B8();
            foot->mC58.mC0 = 1;
            foot->fn_8015495C(0, 1, 1, 1);
            foot->fn_802E3D60();
        }
    }
    wchar_t text[256];
    memset(text, 0, sizeof(text));
    fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x13F), *(const wchar_t**)((char*)m60 + 4));
    fn_80109828(lbl_8087F048, text);
}
