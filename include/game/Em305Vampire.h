#ifndef GAME_EM305VAMPIRE_H
#define GAME_EM305VAMPIRE_H

#include "atn/EfpHandle_unk.h"
#include "atn/SoundRef.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"
#include "std/vector.h"

struct RoutePlace_unk;

// inferred: entry of "door_route" (four values read with fn_800DC12C); names invented
// inferred: wrapper struct (the destructor tests the address of the member one extra time)
struct RoutePlaceList_unk : public std::vector<RoutePlace_unk*> {};

struct VampireDoor_unk {
    VampireDoor_unk(int a, int b, int c, int d) : m00(a), m04(b), m08(c), m0C(d) {}
    int m00;
    int m04;
    int m08;
    int m0C;
};

// inferred from __vt__12Em305Vampire (0x807873E8): vampire (game/Em305Vampire.cpp, not decompiled
// yet: its members are only declared here). Layout from its constructor (0x802E8580) and from
// Em305VampireMp, which rereads the same data file keys.
class Em305Vampire : public SpecialEnemyBase {
public:
    Em305Vampire(atn::Task* parent, EnemyInfo_unk* info); // 0x802E8580
    virtual ~Em305Vampire();                               // 0x802E89B0
    virtual int vf0C();                                    // 0x802E8B88
    virtual void vf10();                                   // 0x802E9590
    virtual void vf14();                                   // 0x802E9D38
    virtual void vf20();                                   // 0x802EA27C
    virtual void vf3C(HitInfo_unk* hit);                   // 0x802E9DEC
    virtual void vf40(HitInfo_unk* hit);                   // 0x802E9FC8
    virtual void vfEC();                                   // 0x802EB9F8
    virtual void vfFC();                                   // 0x802EC66C
    virtual void vf100(int state);                         // 0x802EC668
    virtual void vf104();                                  // 0x802EC664
    virtual void vf108();                                  // 0x802EC660
    virtual void vf11C();                                  // 0x802EA994: inferred: picks the next action

    // inferred: states and actions (out of line in game/Em305Vampire.cpp)
    void fn_802EA490(); // inferred: moves (0x802EA490)
    void fn_802EABE4();
    void fn_802EAC70(); // state 6
    void fn_802EAF68(); // state 7
    void fn_802EB1F4(); // state 8
    void fn_802EB61C(); // state 13
    void fn_802EB814(); // state 14
    void fn_802EB97C(); // state 15
    void fn_802EBABC();
    void fn_802EBB14(); // inferred: starts state 6
    void fn_802EBBA8(); // inferred: starts state 7
    void fn_802EBF40(); // inferred: starts state 13
    void fn_802EC098(); // inferred: starts state 14
    void fn_802EC160();
    void fn_802EC280(); // inferred: starts state 2 (dead)
    void fn_802EC38C();
    inline void StartWeaken_unk(); // inferred: names invented (bodies of fn_802EBF40/fn_802EC160)
    inline void StartStun_unk();
    inline void StartTackle_unk();
    inline void StartFlee_unk();
    inline int NearestWeakenPoint_unk();
    inline VampireDoor_unk* FindDoor_unk(int a, int b);

    atn::LH_Binary m14D4;                         // inferred: data file
    std::vector<VampireDoor_unk> m14DC;           // inferred: "door_route"
    RoutePlaceList_unk m14E8;                     // inferred: "weaken_route"
    CharaTask* m14F4;                             // inferred: target (mC58.mC4)
    int m14F8;
    int m14FC;                                    // inferred: frame counter
    int m1500;                                    // inferred: 1 while following the weaken route
    int m1504;
    int m1508;
    HitInfo_unk* m150C;                           // inferred: tackle skill
    HitInfo_unk* m1510;                           // inferred: shine skill
    HitInfo_unk* m1514;                           // inferred: rolling skill
    atn::Vector3 m1518;                           // inferred: tackle movement per frame (state 7)
    int m1524;                                    // inferred: weaken timer
    int m1528;
    float m152C;
    int m1530;                                    // inferred: speed up step (0 off, 1 wait, 2 up)
    int m1534;                                    // inferred: frames of the speed up step
    float m1538;                                  // inferred: speed factor
    int m153C;
    int m1540;                                    // inferred: silver arrows received
    atn::Vector3 m1544;                           // inferred: start of the door crossing (state 8)
    atn::Vector3 m1550;                           // inferred: end of the door crossing
    VampireDoor_unk* m155C;                       // inferred: door being crossed (state 8)
    int m1560;                                    // inferred: index into m14E8
    atn::EfpHandle_unk m1564;
    atn::EfpHandle_unk m1570;
    atn::EfpHandle_unk m157C;
    atn::EfpHandle_unk m1588;
    atn::EfpHandle_unk m1594;
    atn::SoundRef m15A0;
    atn::SoundRef m15A4;
    int m15A8;                                    // inferred: "normal_wait_time"
    int m15AC;                                    // inferred: "through_wait_time"
    int m15B0;
    int m15B4;
    int m15B8;                                    // inferred: "stun_time"
    ResHandle m15BC;                              // inferred: released in the destructor
    int m15C0;
    Model_unk::Float4_unk m15C4;                  // inferred: model color while m1528 is set
    float m15D4;
    float m15D8[3];                               // inferred: "weaken_speed"
    int m15E4[3];                                 // inferred: "weaken_time"
    float m15F0;                                  // inferred: "weaken_1to2"
    float m15F4;                                  // inferred: "weaken_2to3"
    float m15F8;                                  // inferred: "speed_up_value"
    int m15FC;                                    // inferred: "speed_up_time"
    int m1600;                                    // inferred: "speed_up_wait"
    int m1604;                                    // inferred: "silver_arrow_count"
};

#endif
