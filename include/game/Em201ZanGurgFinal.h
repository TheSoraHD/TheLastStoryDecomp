#ifndef GAME_EM201ZANGURGFINAL_H
#define GAME_EM201ZANGURGFINAL_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__17Em201ZanGurgFinal (0x80784D28): Zangurg (final fight). "Sequence" 0 is the first
// phase (he takes the shield of the arena, m1624), 1 the shielded phase whose shield (m1658, "ShieldHP")
// can be broken; the shield can then be thrown (m1660, states 0-3) and picked up by the leader. Data
// file "database/chara/special/" + db_name (default "db_zangurg_final.csv"). The factory allocates
// 0x1698 bytes (SpecialEnemyBase.cpp, case 65). Names invented; members keep their offsets.
class Em201ZanGurgFinal : public EnemyTask {
public:
    Em201ZanGurgFinal(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em201ZanGurgFinal();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    // inferred: inline (weak copy 0x8026FBB8 at the end of the file): every hit is refused while the
    // shield is up
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
        return m1634 != 0 ? 0 : 3;
    }
    virtual void vf5C(CharaTask* other);
    virtual int vf68(CharaTask* attacker);
    virtual void vf6C(int kind, CharaTask* other);
    virtual void vf98();
    virtual void vf9C(int arg);
    // inferred: inline (weak copy 0x8026FBD4): nonzero in the middle of the jump (state 24)
    virtual int vfE4() {
        if (m58C == 24 && mMotion.mTracks[0].m08 > 45.0f && mMotion.mTracks[0].m08 < 67.0f) {
            return 1;
        }
        return 0;
    }

    inline void SetState_unk(int state, int control);
    inline void ToIdle_unk(); // inferred: back to state 6 (inlined copy of fn_8026AF78)
    void fn_8026580C(); // inferred: reads the data file
    void fn_80266FC4(); // inferred: chooses the target
    void fn_80267134(); // inferred: moves along the route / towards the target
    void fn_802676BC(); // inferred: chooses the next action
    void fn_80267F88();
    void fn_802682C4();
    void fn_802684C0();
    void fn_8026860C();
    void fn_80268CB4();
    void fn_80269168();
    void fn_802692B4();
    void fn_802696F8();
    void fn_80269CBC();
    void fn_8026A010();
    void fn_8026A34C();
    void fn_8026A4FC();
    void fn_8026A5BC();
    void fn_8026A628();
    void fn_8026A774();
    void fn_8026A7AC();
    void fn_8026A828();
    void fn_8026A8E4();
    void fn_8026A9A0();
    void fn_8026AC8C();
    void fn_8026AF24(); // inferred: resets the counters of the state
    void fn_8026AF78(); // inferred: back to state 6 (idle)
    void fn_8026B008();
    void fn_8026B118();
    void fn_8026B430();
    void fn_8026B748();
    void fn_8026B9F0();
    void fn_8026BB08();
    void fn_8026BE2C();
    void fn_8026C150();
    void fn_8026C2C0();
    void fn_8026C3E0(int point, int next); // inferred: warps to a point, then starts `next`
    void fn_8026C4E8(CharaTask* attacker); // inferred: counterattack
    void fn_8026C7A4();
    void fn_8026C844(atn::Vector3 dir); // inferred: knocked down
    void fn_8026CB50();
    void fn_8026CC28();
    void fn_8026CDB4();
    void fn_8026D058();
    void fn_8026D0F8(int mode); // inferred: the shield leaves the hand (thrown or dropped)
    void fn_8026DB8C(); // inferred: update of the thrown shield
    void fn_8026F38C(int point, atn::Vector3* pos, float* rot); // inferred: warp point
    void fn_8026F9BC(const char* node); // inferred: shock wave in front

    atn::LH_Binary m14B0;      // inferred: data file
    CharaTask* m14B8;          // inferred: target
    int m14BC;                 // inferred: frame counter of the state
    int m14C0;                 // inferred: "Sequence"
    int m14C4;                 // inferred: step of the state
    float m14C8;
    int m14CC;                 // inferred: actions since the last special one
    int m14D0;
    int m14D4[6];              // inferred: "ComboOK"
    int m14EC;                 // inferred: frames of the weapon effect
    int m14F0;
    atn::Vector3 m14F4;        // inferred: "CenterPoint"
    int m1500;                 // inferred: "AtkInterval"
    int m1504;                 // inferred: hits received in a row
    int m1508;
    CharaTask* m150C;          // inferred: grabbed character
    atn::Vector3 m1510;
    atn::Vector3 m151C;        // inferred: warp destination
    float m1528;               // inferred: warp rotation
    int m152C;                 // inferred: action after the warp
    atn::Vector3 m1530;        // inferred: direction of the charge
    float m153C;
    float m1540;               // inferred: distance of the charge
    int m1544;
    unsigned char m1548[0x1558 - 0x1548];
    atn::Vector3 m1558;        // inferred: rotation of the knock down
    atn::EfpHandle_unk m1564;  // inferred: "/effect/ef_bs007a.efp"
    atn::EfpHandle_unk m1570;  // inferred: "/effect/ef_bs065a.efp"
    atn::EfpHandle_unk m157C;  // inferred: "/effect/ef_bs065c.efp"
    atn::EfpHandle_unk m1588;  // inferred: "/effect/ef_bu031a.efp"
    atn::EfpHandle_unk m1594;  // inferred: "/effect/ef_bs064a.efp"
    atn::EfpHandle_unk m15A0;  // inferred: "/effect/ef_bu032a.efp"
    atn::EfpHandle_unk m15AC;  // inferred: "/effect/ef_bu033a.efp"
    atn::EfpHandle_unk m15B8;  // inferred: "/effect/ef_bs063a.efp"
    atn::EfpHandle_unk m15C4;  // inferred: "/effect/ef_bu040a.efp"
    atn::EfpHandle_unk m15D0;  // inferred: "/effect/ef_bu040d.efp"
    atn::EfpHandle_unk m15DC;  // inferred: "/effect/ef_bu040e.efp"
    atn::EfpHandle_unk m15E8;  // inferred: "/effect/ef_bu040f.efp"
    atn::EfpHandle_unk m15F4;  // inferred: "/effect/ef_bu040h.efp"
    ResLoader_unk m1600;       // inferred: "/effect/ef_bu040g_01.eff"
    atn::EfpHandle_unk m160C;  // inferred: "ef_we006.efp"
    atn::EfpHandle_unk m1618;  // inferred: "ef_bs073f.efp"
    chr::CharaPart_unk* m1624; // inferred: the shield (created by fn_800457A4)
    chr::CharaPart_unk* m1628; // inferred: the shield of the enemy (m1624 changes when the leader takes it)
    int m162C;
    int m1630;                 // inferred: nonzero once the shield left the hand
    int m1634;                 // inferred: nonzero while the shield is broken (hits accepted)
    int m1638;
    int m163C;
    int m1640;
    int m1644;
    int m1648;
    int m164C;
    int m1650;
    int m1654;
    int m1658;                 // inferred: "ShieldHP"
    int m165C;                 // inferred: "ShieldHP" (maximum)
    int m1660;                 // inferred: state of the thrown shield (0-3)
    float m1664;               // inferred: spin of the shield
    float m1668;               // inferred: time of the shield flight
    atn::Vector3 m166C;        // inferred: start of the flight
    atn::Vector3 m1678;        // inferred: end of the flight
    int m1684;                 // inferred: bullet id of the shield
    int m1688;
    ResHandle m168C;           // inferred: debug menu group
    int m1690;                 // inferred: "DebugDisp"
    int m1694;
};

#endif
