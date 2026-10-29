#ifndef GAME_EM302KRAKENMP_H
#define GAME_EM302KRAKENMP_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/SoundRef.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"
#include "std/vector.h"

struct RoutePlace_unk;
class Em303KrakenFootMp;

// inferred: 0x1C-byte entry of m1594 (a character and two positions, as KrakenTarget_unk of
// game/Em302Kraken.h); names invented
struct KrakenMpTarget_unk {
    CharaTask* mChara;
    atn::Vector3 m04;
    atn::Vector3 m10;
};

// inferred: entry of "act_command" (command id, value); names invented
struct KrakenMpCommand_unk {
    int mId;
    int mValue;
};

// inferred: classes derived from std::vector (their destructors test the address four times);
// names invented
struct KrakenMpPlaceList_unk : public std::vector<RoutePlace_unk*> {};
struct KrakenMpIntList_unk : public std::vector<int> {};
struct KrakenMpEnemyList_unk : public std::vector<EnemyTask*> {};
struct KrakenMpFootList_unk : public std::vector<Em303KrakenFootMp*> {};

// inferred from __vt__13Em302KrakenMp (0x80786F9C): multiplayer version of the kraken, driven by the
// SpecialEnemyBase state machine and synchronized with vf110/vf114. Data file
// "database/chara/special/" + db_name (default "db_em302_00_00.csv"). The factory allocates 0x1660
// bytes (SpecialEnemyBase.cpp, case 32). The virtuals of slots 0x90/0x94/0xC8 are inline (weak
// copies at the end of the file). Names invented; members keep their offsets.
class Em302KrakenMp : public SpecialEnemyBase {
public:
    Em302KrakenMp(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em302KrakenMp();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual int vfB0(atn::Vector3* out);
    virtual boost::function<void()> vfB4(CharaTask* chara);
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf50();
    virtual void vf54();
    virtual float vf90() { return 1.3f; } // inferred: inline (weak copy 0x802E1D48)
    virtual atn::Vector3 vf94() { return atn::Vector3(0.0f, 50.0f, 0.0f); } // inferred: weak copy 0x802E1D30
    // inferred: position above the first damage sphere (0x802E1CA0)
    virtual atn::Vector3 vfAC();
    // inferred: inline (weak copy 0x802E1D50): position of the "head" node
    virtual atn::Vector3 GetTacViewPosition() const {
        MtxPtr m = mMotion.GetNodeMtx_unk("head");
        // inferred: the original loads the translation from the last row up (z, y, x)
        float z = m[2][3];
        float y = m[1][3];
        float x = m[0][3];
        return atn::Vector3(x, y, z) + m5A4;
    }
    virtual void vfEC();
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual void vf110(SpecialState_unk* state);
    virtual void vf114(const SpecialState_unk* state);

    void fn_802E03F0();
    int fn_802E0890();
    void fn_802E094C();
    void fn_802E0C60();
    void fn_802E0EC8();
    void fn_802E10D8(int kind);
    void fn_802E1290();
    void fn_802E1414();
    void fn_802E1624();
    void fn_802E177C(); // inferred: starts state 2 (dead)
    void fn_802E1A0C(); // inferred: debug menu entries
    inline void Reset_unk(); // inferred: back to state 0

    int m14D4;
    int m14D8;
    int m14DC;
    int m14E0;
    int m14E4;
    int m14E8;                  // inferred: step counter
    int m14EC;                  // inferred: frame counter of the state
    float m14F0;                // inferred: "pos_y_offset"
    int m14F4;
    int m14F8;
    int m14FC;
    int m1500;                  // inferred: "shades_skill_id"
    atn::Vector3 m1504;         // inferred: position of the shades attack (fn_802E094C)
    atn::EfpFile m1510; // inferred: "/effect/ef_bs030c.efp"
    int m151C;                  // inferred: nonzero hides the debug display (vf14)
    atn::EfpFile m1520; // inferred: "/effect/ef_bu022.efp"
    atn::EfpFile m152C; // inferred: "/effect/ef_bu023.efp"
    int m1538;
    int m153C;
    int m1540;
    int m1544;
    int m1548;
    int m154C;
    int m1550;
    int m1554;
    int m1558;
    int m155C;                  // inferred: "laser_skill_id"
    atn::SoundRef m1560;
    atn::EfpFile m1564; // inferred: "/effect/ef_bs028.efp"
    atn::EfpFile m1570; // inferred: "/effect/ef_bs029a.efp"
    atn::EfpFile m157C; // inferred: "/effect/ef_bs029c.efp"
    atn::EfpFile m1588; // inferred: "anger_efp"
    std::vector<KrakenMpTarget_unk> m1594;
    atn::Vector3 m15A0;
    RoutePlace_unk* m15AC;      // inferred: "charge_point"
    int m15B0;
    float m15B4;                // inferred: "explode_center" (three values)
    float m15B8;
    float m15BC;
    atn::EfpFile m15C0;
    atn::EfpFile m15CC; // inferred: "/effect/ef_bd005.efp"
    CharaTask* m15D8;           // inferred: target
    int m15DC;
    RoutePlace_unk* m15E0;      // inferred: current appear point
    unsigned int m15E4;         // inferred: index into m15E8
    KrakenMpPlaceList_unk m15E8; // inferred: "appear_point"
    KrakenMpIntList_unk m15F4;   // inferred: "pillar_id"
    float m1600;                 // inferred: not set by the constructor
    float m1604;
    int m1608;
    KrakenMpPlaceList_unk m160C; // inferred: "reinforce_point"
    KrakenMpEnemyList_unk m1618; // inferred: "reinforce_chara"
    KrakenMpFootList_unk m1624;  // inferred: "foot_chara"
    atn::LH_Binary m1630;        // inferred: data file
    std::vector<KrakenMpCommand_unk> m1638; // inferred: "act_command"
    int m1644;
    int m1648;
    int m164C;
    unsigned char m1650;         // inferred: nonzero while the death is being synchronized
    int m1654;
    ResHandle m1658;             // inferred: debug menu group
    int m165C;                   // inferred: "DebugDisp"
};

#endif
