#ifndef GAME_EM203GREATERDAEMONRED_H
#define GAME_EM203GREATERDAEMONRED_H

#include "game/Em203GreaterDaemon.h"

// inferred: list of floats, a class derived from std::vector<float> (its destructor tests the address
// four times, as IntVector_unk); name invented
struct DaemonFloatList_unk : public std::vector<float> {};

// inferred: float colour (r, g, b, a) written to a material as bytes; name invented
struct DaemonColor_unk {
    DaemonColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

// inferred: thunderbolt of the red daemon (16 bytes, std::vector element): position and the frame
// it falls (names invented)
struct DaemonThunder_unk {
    atn::Vector3 mPos;
    int mFrame;
};

// inferred: thunder bolt of the grand attack (16 bytes, std::vector element): frame and position
// (names invented)
struct DaemonBolt_unk {
    int mFrame;
    atn::Vector3 mPos;
};

// inferred from __vt__21Em203GreaterDaemonRed (0x80785978): the red greater daemon (enemy id 18).
// Data file "database/chara/special/" + db_name (default "db_em203_00_00.csv"). The factory
// allocates 0x16D0 bytes (SpecialEnemyBase.cpp, case 18). Members keep their offsets (names invented).
class Em203GreaterDaemonRed : public EnemyTask {
public:
    Em203GreaterDaemonRed(atn::Task* parent, EnemyInfo_unk* info); // 0x80293738
    virtual ~Em203GreaterDaemonRed();                              // 0x80293BC8
    virtual int vf0C();                                            // 0x80293E30
    virtual void vf10();                                           // 0x802946BC
    virtual void vf14();                                           // 0x80294F88
    virtual void vf3C(HitInfo_unk* hit);                           // 0x802956D8
    virtual void vf40(HitInfo_unk* hit);                           // 0x80295988
    virtual void vf50();                                           // 0x80295BE4
    virtual void vf54();                                           // 0x80295DB8
    virtual atn::Vector3 vf64(int center);                         // 0x80296160
    virtual void vfEC();                                           // 0x80299EF8
    virtual int vf68(CharaTask* attacker);                         // 0x8029A564
    virtual void vf6C(int kind, CharaTask* other);                 // 0x8029A580
    virtual atn::Vector3 vf58();                                   // 0x8029BDC8
    virtual int vfE4();                                            // 0x8029C69C
    virtual void vfCC(atn::Vector3* out, const atn::Vector3& dir); // 0x8029C728
    virtual atn::Vector3 vfD0(float angle);                        // 0x8029C8CC
    virtual atn::Vector3 vfD8(CharaTask* target);                  // 0x8029C94C
    // inferred: inline (weak copies at the end of the file, in reverse order)
    // inferred: position of the "neck" node (0x8029CA84; the literal is in the .data pool of the
    // inline member)
    virtual atn::Vector3 vfC8() {
        MtxPtr m = mMotion.GetNodeMtx_unk("neck");
        float z = m[2][3];
        float y = m[1][3];
        float x = m[0][3];
        return atn::Vector3(x, y, z);
    }
    virtual float vf70() { return 100.0f; } // 0x8029CA7C
    virtual float vf90() { return 4.0f; }   // 0x8029CA74
    virtual float vfD4() { return -7.0f; }             // 0x8029CA6C
    virtual float vfDC() { return 0.7f; }              // 0x8029CA64
    virtual float vfE0() { return 0.9424779f + mRot.y; } // 0x8029CA54
    // inferred: new virtual (slot 0xF8): updates the collision capsule and the damage spheres
    virtual void vfF8(); // 0x802959EC

    // inferred: inline helpers (names invented)
    inline void ResetState_unk();
    // inferred: the frame of track 0 has reached the length of the motion (names invented)
    inline void PlayMotion_unk(int motion, int loop, int arg, float speed);
    inline void ToIdle_unk();
    inline void StartState12_unk();
    inline void StartState13_unk();
    inline void StartState22_unk();
    inline void StartSkill_unk(int state);
    inline int Chant_unk();
    inline int SelectHover_unk();
    inline void AdjustHeight_unk();
    void fn_802953C8();
    void fn_80296268();
    void fn_80296B8C(int wait);
    void fn_80296DB8();
    void fn_80297114();
    void fn_80297474();
    void fn_8029795C();
    void fn_80298580();
    void fn_80298850();
    void fn_80298B10();
    void fn_80298E98();
    void fn_80299134();
    void fn_802992F8();
    void fn_802995D4();
    void fn_802997B8();
    void fn_80299A7C();
    void fn_8029A0A8();
    void fn_8029A674(int kind);
    void fn_8029A788();
    void fn_8029A928();
    void fn_8029AC1C();
    void fn_8029AF6C();
    void fn_8029B24C();
    void fn_8029B59C();
    int fn_8029B704();
    void fn_8029BE7C();
    int fn_8029C564(int arg);
    float Stripped1_unk(); // inferred: removed by the linker (constants only)
    float Stripped2_unk(); // inferred: removed by the linker (constants only)

    CharaTask* m14B0; // inferred: target
    int m14B4;        // inferred: step of the state
    int m14B8;        // inferred: frame counter of the state
    int m14BC;
    int m14C0;
    int m14C4;
    int m14C8;          // inferred: level (0 before, 1 after the hit points fall; index of the skill pairs)
    int m14CC;          // inferred: "LevelOfsBefore"
    int m14D0;          // inferred: "LevelOfsAfter"
    HitInfo_unk* m14D4[2]; // inferred: "LaserID"
    HitInfo_unk* m14DC[2]; // inferred: "TackleID"
    HitInfo_unk* m14E4[2]; // inferred: "ClawID"
    HitInfo_unk* m14EC[2]; // inferred: "BreathID"
    HitInfo_unk* m14F4[2]; // inferred: "FlareID"
    HitInfo_unk* m14FC[2]; // inferred: "ThunderBoltID"
    IntVector_unk m1504[2]; // inferred: "FlySkill", "GroundSkill"
    int m151C;              // inferred: index in the skill list
    int m1520;
    int m1524; // inferred: the camera looks at the daemon
    int m1528;
    float m152C;
    atn::Vector3 m1530;
    RoutePoint_unk* m153C;
    RoutePoint_unk* m1540;
    float m1544;
    float m1548;
    atn::Vector3 m154C;
    atn::Vector3 m1558;        // inferred: "CenterPoint"
    DaemonPlaceList_unk m1564; // inferred: "HighInsideHoverPoint"
    IntVector_unk m1570;       // inferred: "HoverDirection"
    int m157C;
    int m1580;
    int m1584;
    int m1588;
    atn::Vector3 m158C;
    atn::Vector3 m1598;
    int m15A4;
    std::vector<DaemonBolt_unk> m15A8;
    HitInfo_unk* m15B4; // inferred: magic in use
    int m15B8;          // inferred: chant counter
    atn::EfpFile m15BC;
    atn::EfpFile m15C8;
    std::vector<DaemonThunder_unk> m15D4;
    int m15E0;
    atn::EfpFile m15E4; // inferred: "/effect/ef_bs049e.efp"
    atn::EfpFile m15F0;
    int m15FC;
    int m1600;
    int m1604;
    int m1608;
    int m160C;
    DaemonFloatList_unk m1610; // inferred: "StunHPRate"
    DaemonFloatList_unk m161C; // inferred: "RecoverHPRate"
    int m1628;
    atn::EfpFile m162C; // inferred: "/effect/ef_mb005c.efp"
    CharaTask* m1638;
    atn::Vector3 m163C;
    atn::Vector3 m1648;
    atn::Vector3 m1654;   // inferred: centre of the arena (route place 600)
    atn::LH_Binary m1660; // inferred: data file
    ResHandle m1668;      // inferred: debug menu group
    int m166C;            // inferred: "DebugDisp"
    float m1670;          // inferred: "HoverSpeed"
    float m1674;          // inferred: "BulletSpeed"
    float m1678;          // inferred: "LimitAngle"
    int m167C;            // inferred: "StunTime"
    int m1680;            // inferred: "GrandTime"
    int m1684;            // inferred: "GrandAttackTime"
    int m1688;            // inferred: "GrandBreathTime"
    float m168C;          // inferred: "ThunderBoltAreaRadius"
    int m1690;            // inferred: "ThunderBoltMax"
    int m1694;            // inferred: "ThunderBoltNum"
    int m1698;            // inferred: "ThunderBrankTime"
    int m169C;            // inferred: "ThunderChargeTime"
    int m16A0;            // inferred: "ThunderTimeLag"
    int m16A4;            // inferred: "PlayerHateValueSky"
    int m16A8;            // inferred: "PlayerHateValueGrand"
    int m16AC;            // inferred: "PlayerHateValueAttHate"
    float m16B0;          // inferred: "PlayerHateRatioSky"
    float m16B4;          // inferred: "PlayerHateRatioGrand"
    float m16B8;          // inferred: "PlayerHateRatioAttHate"
    float m16BC;          // inferred: "RecoverHPRateForStun"
    DaemonColor_unk m16C0; // inferred: "LvDownColor"
};

#endif
