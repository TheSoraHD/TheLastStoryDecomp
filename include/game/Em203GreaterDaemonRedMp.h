#ifndef GAME_EM203GREATERDAEMONREDMP_H
#define GAME_EM203GREATERDAEMONREDMP_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/Em203GreaterDaemonRed.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

// inferred from __vt__23Em203GreaterDaemonRedMp (0x80785B38): multiplayer version of the red greater
// daemon (enemy id 19), driven by the SpecialEnemyBase state machine and synchronized with vf110/vf114.
// Data file "database/chara/special/" + db_name (default "db_em203_02_00.csv"). The factory allocates
// 0x16D0 bytes (SpecialEnemyBase.cpp, case 19). Names invented; members keep their offsets.
class Em203GreaterDaemonRedMp : public SpecialEnemyBase {
public:
    Em203GreaterDaemonRedMp(atn::Task* parent, EnemyInfo_unk* info); // 0x8029CB00
    // inferred: inline (weak copy at the end of the file, 0x802A4A04)
    virtual ~Em203GreaterDaemonRedMp() {}
    virtual int vf0C();                  // 0x8029CF00
    virtual void vf10();                 // 0x8029D7AC
    virtual void vf40(HitInfo_unk* hit); // 0x8029DEF8
    virtual void vf50();                 // 0x8029E2AC
    virtual void vf54();                 // 0x8029E480
    virtual atn::Vector3 vf64(int center); // 0x8029E848
    virtual void vfFC();                 // 0x802A4628
    virtual void vf100(int state);       // 0x802A46E0
    virtual void vf104();                // 0x802A46E4
    virtual int vf10C(struct HitMsg_unk* msg);         // 0x8029DECC
    virtual void vf110(SpecialState_unk* state);       // 0x802A4454
    virtual void vf114(const SpecialState_unk* state); // 0x802A4478
    // inferred: new virtual (slot 0x11C): updates the collision capsule (0x8029E0B4)
    virtual void vf11C();
    // inferred: nonzero while the daemon is flying (weak copy 0x802A49C8; also read on the owner daemon)
    int fn_802A49C8() {
        if (m58C == 12) {
            return 0;
        }
        if (m58C == 14) {
            return 1;
        }
        return mPos.y > 30.0f;
    }
    // inferred: inline (weak copy 0x802A49C4)
    virtual void vf108() {}

    // inferred: inline helpers (names invented)
    inline void ResetState_unk();
    inline void PlayMotion_unk(int motion, int loop, int arg, float speed);
    inline void BeginAction_unk();
    inline void ToIdle_unk();
    inline void StartState13_unk();
    inline void Die_unk();
    inline int Chant_unk();
    inline int IsNearEnd_unk(float margin);
    void fn_8029DB54();
    void fn_8029DB58();
    void fn_8029DBE0(float scale);
    void fn_8029E950();
    void fn_8029F148(int wait);
    int fn_8029F3F4();
    void fn_8029F3FC();
    void fn_8029F738();
    void fn_8029FAC0();
    void fn_8029FD34();
    void fn_802A057C();
    void fn_802A0818();
    void fn_802A0B00();
    void fn_802A118C();
    void fn_802A16A4();
    void fn_802A17CC();
    void fn_802A19B0();
    void fn_802A1BFC();
    void fn_802A2070();
    void fn_802A239C(int kind);
    void fn_802A2494();
    void fn_802A254C();
    void fn_802A2600();
    void fn_802A2790();
    void fn_802A2A68();
    void fn_802A2F48();
    void fn_802A325C();
    void fn_802A3520();
    void fn_802A35B0(int kind);
    int fn_802A36B8();
    int fn_802A3D6C(HitInfo_unk* skill);
    void fn_802A3D80(int single);
    void Stripped1_unk(); // inferred: removed by the linker (debug menu, strings only)
    int fn_802A4194(int arg);
    int fn_802A42CC();

    CharaTask* m14D4; // inferred: target
    int m14D8;        // inferred: step of the state
    int m14DC;        // inferred: frame counter of the state
    int m14E0;        // inferred: 0 hovering skills, 1 flying skills
    int m14E4;        // inferred: phase of the skill selection
    HitInfo_unk* m14E8; // inferred: "LaserID"
    HitInfo_unk* m14EC; // inferred: "TackleID"
    HitInfo_unk* m14F0; // inferred: "ClawID"
    HitInfo_unk* m14F4; // inferred: "BreathID"
    HitInfo_unk* m14F8; // inferred: "FlareID"
    HitInfo_unk* m14FC; // inferred: "DamageCircleID"
    HitInfo_unk* m1500; // inferred: "ThunderBoltID"
    HitInfo_unk* m1504; // inferred: "ThunderBoltPreID"
    IntVector_unk m1508[2][3]; // inferred: "1st..3rdHoverSkill", "1st..3rdFlySkill"
    int m1550;          // inferred: index in the skill list
    int m1554;          // inferred: the camera looks at the daemon
    int m1558;          // inferred: route point reached
    float m155C;
    atn::Vector3 m1560;
    RoutePoint_unk* m156C;
    RoutePoint_unk* m1570;
    float m1574;
    float m1578;
    atn::Vector3 m157C;
    atn::Vector3 m1588;
    int m1594;
    int m1598;
    atn::Vector3 m159C;
    DaemonPlaceList_unk m15A8; // inferred: "HighInsideHoverPoint"
    IntVector_unk m15B4;       // inferred: "HoverDirection"
    int m15C0;
    int m15C4; // inferred: frame of the breath
    int m15C8; // inferred: the breath is running
    int m15CC; // inferred: bullet id of the breath
    atn::Vector3 m15D0;
    atn::Vector3 m15DC;
    int m15E8;
    std::vector<DaemonBolt_unk> m15EC;
    HitInfo_unk* m15F8; // inferred: skill in use
    int m15FC;          // inferred: chant counter
    std::vector<DaemonThunder_unk> m1600;
    int m160C;
    int m1610;
    int m1614; // inferred: index into "StunHPRate"
    int m1618;
    int m161C;
    int m1620;
    DaemonFloatList_unk m1624; // inferred: "StunHPRate"
    int m1630;
    int m1634;
    atn::Vector3 m1638; // inferred: "CenterPoint"
    int m1644;          // inferred: "OwnerDaemon" given (this is the sub daemon)
    Em203GreaterDaemonRedMp* m1648; // inferred: owner daemon
    EnemyTask* m164C;   // inferred: "SubDaemon"
    int m1650;
    int m1654;
    int m1658;
    int m165C;
    int m1660;
    int m1664;
    atn::EfpFile m1668; // inferred: "/effect/ef_uc015.efp"
    int m1674;
    int m1678;
    atn::LH_Binary m167C; // inferred: data file
    ResHandle m1684;      // inferred: debug menu group
    int m1688;            // inferred: "DebugDisp"
    float m168C;          // inferred: "HoverSpeed"
    float m1690;          // inferred: "BulletSpeed"
    float m1694;          // inferred: "LimitAngle"
    int m1698;            // inferred: "AtackedMax"
    int m169C;            // inferred: "GunPerAttac"
    int m16A0;            // inferred: "MagicPerAttack"
    int m16A4;            // inferred: "StunTime"
    int m16A8;            // inferred: "GrandTime"
    int m16AC;            // inferred: "GrandAttackTime"
    int m16B0;            // inferred: "GrandBreathTime"
    float m16B4;          // inferred: "ThunderBoltAreaRadius"
    int m16B8;            // inferred: "ThunderBoltMax"
    int m16BC;            // inferred: "ThunderBoltNum"
    int m16C0;            // inferred: "ThunderBrankTime"
    int m16C4;            // inferred: "ThunderChargeTime"
    int m16C8;            // inferred: "ThunderTimeLag"
};

#endif
