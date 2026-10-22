#ifndef GAME_EM302KRAKEN_H
#define GAME_EM302KRAKEN_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

struct RoutePlace_unk;
class Em303KrakenFoot;

// inferred: 0x1C-byte entry of m1554 (a character and two positions, as ShadeTarget_unk of
// game/Em301LShade.h); names invented
struct KrakenTarget_unk {
    CharaTask* mChara;
    atn::Vector3 m04;
    atn::Vector3 m10;
};

// inferred: entry of "act_command" (command id from the table at 0x807473F8, then a value); names
// invented
struct KrakenCommand_unk {
    int mId;
    int mValue;
};

// inferred: classes derived from std::vector (their destructors test the address four times, as
// RoutePlaceList_unk of game/Em305Vampire.h); names invented
struct KrakenPlaceList_unk : public std::vector<RoutePlace_unk*> {};
struct KrakenIntList_unk : public std::vector<int> {};
struct KrakenEnemyList_unk : public std::vector<EnemyTask*> {};
struct KrakenFootList_unk : public std::vector<Em303KrakenFoot*> {};

// inferred from __vt__11Em302Kraken (0x80786E20): the kraken boss (moves its feet, Em303KrakenFoot,
// shoots lasers and thunder). Data file "database/chara/special/" + db_name (default
// "db_em302_00_00.csv"). The factory allocates 0x16B8 bytes (SpecialEnemyBase.cpp, case 30). The
// virtuals of slots 0x90/0x94/0xC8/0xD4/0xDC are inline (weak copies at the end of the file).
// Names invented; members keep their offsets.
class Em302Kraken : public EnemyTask {
public:
    Em302Kraken(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em302Kraken();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual int vfB0(atn::Vector3* out);
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf50();
    virtual void vf54();
    virtual void vfEC();
    virtual float vf90() { return 1.3f; } // inferred: inline (weak copy 0x802DDCF4)
    virtual atn::Vector3 vf94() { return atn::Vector3(0.0f, 50.0f, 0.0f); } // inferred: weak copy 0x802DDCDC
    // inferred: inline (weak copy 0x802DDC3C): position of the "head" node
    virtual atn::Vector3 vfC8() {
        MtxPtr m = mMotion.GetNodeMtx_unk("head");
        return atn::Vector3(m[0][3], m[1][3], m[2][3]) + m5A4;
    }
    virtual atn::Vector3 vfD0(float angle);
    virtual float vfD4() { return 1.0f; } // inferred: weak copy 0x802DDC34
    virtual atn::Vector3 vfD8(CharaTask* target);
    virtual float vfDC() { return 0.7f; } // inferred: weak copy 0x802DDC2C

    inline void Reset_unk();
    void fn_802DA174();
    void fn_802DA718(); // inferred: state 4 (shades)
    void fn_802DAA38();
    void fn_802DB2D0();
    void fn_802DB534();
    void fn_802DB7C8();
    void fn_802DBA3C();
    void fn_802DBC68();
    void fn_802DBE70(int kind);
    void fn_802DC014();
    void fn_802DC1C0();
    void fn_802DC3D0();
    void fn_802DC618(); // inferred: starts state 2 (dead)
    KrakenCommand_unk* fn_802DC8B0();
    void fn_802DCA34(int state, int kind); // inferred: calls the reinforcements
    void fn_802DD1B4(int count);
    void fn_802DDA90(); // inferred: releases the feet
    void fn_802DDBB0(); // inferred: debug menu entries
    void fn_802DC5B4(int arg); // inferred: starts state 15

    int m14B0;                  // inferred: loading step of vf0C
    int m14B4;
    int m14B8;
    int m14BC;
    int m14C0;
    int m14C4;                  // inferred: step counter
    int m14C8;                  // inferred: frame counter of the state
    float m14CC;                // inferred: "pos_y_offset"
    int m14D0;
    int m14D4;
    int m14D8;
    int m14DC;
    atn::EfpHandle_unk m14E0; // inferred: "/effect/ef_bs033a.efp"
    atn::EfpHandle_unk m14EC; // inferred: "/effect/ef_bs033c.efp"
    int m14F8;                  // inferred: "shades_skill_id"
    atn::Vector3 m14FC;         // inferred: position of the shades attack (fn_802DA718)
    atn::EfpHandle_unk m1508; // inferred: "/effect/ef_bs030c.efp"
    float m1514;                // inferred: "absorb_rate"
    float m1518;
    float m151C;
    float m1520;
    ResLoader_unk m1524;        // inferred: "/effect/ef_bs034b_01.eff"
    atn::EfpHandle_unk m1530; // inferred: "/effect/ef_bs034c.efp"
    atn::EfpHandle_unk m153C; // inferred: "/effect/ef_bs034a.efp"
    atn::EfpHandle_unk m1548; // inferred: "/effect/ef_bs034e.efp"
    std::vector<KrakenTarget_unk> m1554;
    int m1560;                  // inferred: nonzero hides the debug display (vf14)
    atn::EfpHandle_unk m1564; // inferred: "/effect/ef_bu022.efp"
    atn::EfpHandle_unk m1570; // inferred: "/effect/ef_bu023.efp"
    int m157C;
    int m1580;
    int m1584;                  // inferred: "laser_skill_id"
    atn::SoundRef m1588;
    atn::EfpHandle_unk m158C; // inferred: "/effect/ef_bs028.efp"
    atn::EfpHandle_unk m1598; // inferred: "/effect/ef_bs029a.efp"
    atn::EfpHandle_unk m15A4; // inferred: "/effect/ef_bs029c.efp"
    atn::EfpHandle_unk m15B0; // inferred: "anger_efp"
    std::vector<atn::Vector3> m15BC;
    atn::Vector3 m15C8;
    RoutePlace_unk* m15D4;      // inferred: "charge_point"
    int m15D8;
    float m15DC;                // inferred: "explode_center" (three values)
    float m15E0;
    float m15E4;
    atn::EfpHandle_unk m15E8; // inferred: "/effect/ef_bd005.efp"
    atn::EfpHandle_unk m15F4; // inferred: "/effect/ef_bd004.efp"
    int m1600;
    int m1604;                  // inferred: "shoot_thunder"
    int m1608;                  // inferred: "thunder_skill_id"
    atn::EfpHandle_unk m160C; // inferred: "/effect/ef_bs081a.efp"
    atn::EfpHandle_unk m1618; // inferred: "/effect/ef_bs081b.efp"
    atn::EfpHandle_unk m1624; // inferred: "/effect/ef_bs049c.efp"
    CharaTask* m1630;           // inferred: target
    RoutePlace_unk* m1634;      // inferred: current appear point
    unsigned int m1638;         // inferred: index into m163C
    KrakenPlaceList_unk m163C;  // inferred: "appear_point"
    KrakenIntList_unk m1648;    // inferred: "pillar_id"
    void* m1654;
    int m1658;
    int m165C;
    KrakenPlaceList_unk m1660;  // inferred: "reinforce_point"
    KrakenEnemyList_unk m166C;  // inferred: "reinforce_chara"
    KrakenFootList_unk m1678;   // inferred: "foot_chara"
    atn::LH_Binary m1684;       // inferred: data file
    std::vector<KrakenCommand_unk> m168C; // inferred: "act_command"
    unsigned int m1698;         // inferred: index into m168C
    int m169C;
    float m16A0;
    ResHandle m16A4;            // inferred: debug menu group
    int m16A8;                  // inferred: "DebugDisp"
    int m16AC;
    void* m16B0;                // inferred: "week_param"
    int m16B4;
};

#endif
