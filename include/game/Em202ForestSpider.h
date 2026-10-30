#ifndef GAME_EM202FORESTSPIDER_H
#define GAME_EM202FORESTSPIDER_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/Em202SpiderBase.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

struct RoutePlace_unk;

// inferred: thread shot by the forest spider (0xEC bytes; constructor 0x80272C6C and destructor 0x80272CF0
// are the copies used by __construct_array). Names invented.
struct SpiderThread_unk {
    SpiderThread_unk() : m00(0), m04(0), m08(0), m0C(0), m88(0) {}
    int m00;              // inferred: in use
    int m04;              // inferred: the thread caught m88
    int m08;              // inferred: frames left before the thread is released
    int m0C;              // inferred: main thread (argument of CharaTask::fn_8015C0C0)
    atn::Vector3 m10;     // inferred: velocity of the tip
    atn::Vector3 m1C;
    atn::Matrix3x4 m28;   // inferred: matrix of the tip
    atn::Matrix3x4 m58;   // inferred: matrix of the thread (from the head to the tip)
    CharaTask* m88;       // inferred: target
    atn::EfpFile m8C;  // inferred: "/effect/ef_bs006a.efp"
    atn::EffFile m98;       // inferred: "/effect/ef_bs006b_01.eff"
    atn::EfpFile mA4;  // inferred: "/effect/ef_bs006c.efp"
    atn::EffFile mB0;       // inferred: "/effect/ef_bs009_01.eff"
    atn::EffFile mBC;       // inferred: "/effect/ef_bs010_01.eff"
    atn::EfpFile mC8;  // inferred: "/effect/ef_bs011.efp"
    atn::EfpFile mD4;  // inferred: "/effect/ef_uc045.efp"
    atn::EfpFile mE0;  // inferred: "/effect/ef_bs011d.efp"
};

// inferred: the destructor tests `this` once more than for a plain std::vector (as SpiderList_unk in
// game/Em202Spider.h); names invented
struct ForestEnemyList_unk : public std::vector<EnemyTask*> {};
struct ForestPlaceList_unk : public std::vector<RoutePlace_unk*> {};
struct ForestCharaList_unk : public std::vector<CharaTask*> {};

// inferred from __vt__17Em202ForestSpider (0x807850E0). Data file "database/chara/special/" + db_name (default
// "db_em202_02_00.csv"). The factory allocates 0x1968 bytes. Members keep their offsets (names invented).
class Em202ForestSpider : public EnemyTask {
public:
    Em202ForestSpider(atn::Task* parent, EnemyInfo_unk* info); // 0x8027278C
    virtual ~Em202ForestSpider();                              // 0x80272E0C
    virtual int vf0C();                                        // 0x80272FB8
    virtual void vf10();                                       // 0x80273970
    virtual void vf14();                                       // 0x80273FE8
    virtual void vf3C(HitInfo_unk* hit);                       // 0x80274268
    virtual void vf40(HitInfo_unk* hit);                       // 0x802744BC
    virtual void vf54();                                       // 0x80274518
    virtual void vf7C(int arg);                                // 0x80274508
    virtual float vf90() { return 5.0f; }                      // 0x80279858
    virtual void vfEC();                                       // 0x80274F54
    virtual void vfF8();   // 0x802745A0 inferred: chooses the target
    virtual void vfFC();   // 0x802747D0 inferred: movement of the frame
    virtual void vf100();  // 0x80274CFC inferred: chooses the next action
    virtual void vf104();  // 0x80274E7C inferred: state 6 (follows the target)
    virtual void vf108();  // 0x80275094 inferred: state 7 (attack)
    virtual void vf10C();  // 0x80275160 inferred: state 8 (tackle)
    virtual void vf110();  // 0x80275228 inferred: state 9 (threads)
    virtual void vf114();  // 0x80275AFC inferred: state 10 (pulls the caught character)
    virtual void vf118();  // 0x8027606C inferred: state 11 (eats the caught character)
    virtual void vf11C();  // 0x8027641C inferred: state 12
    virtual void vf120();  // 0x80276484 inferred: state 16 (turn)
    virtual void vf124();  // 0x80276648 inferred: state 13 (reinforcements)
    virtual void vf128();  // 0x802766B8 inferred: state 14 (goes to the reinforcement center)
    virtual void vf12C();  // 0x802767B0 inferred: state 15 (goes to the thread point m14CC)
    virtual void vf130();  // 0x80277A88 inferred: state 17
    virtual void vf134();  // 0x8027686C inferred: state 18
    virtual void vf138();  // 0x802768CC inferred: back to state 6
    virtual void vf13C();  // 0x80276BF4 inferred: starts state 7
    virtual void vf140();  // 0x80276B3C inferred: starts state 8
    virtual void vf144();  // 0x80276CAC inferred: starts state 9
    virtual void vf148();  // 0x80276E44 inferred: starts state 10
    virtual void vf14C();  // 0x80276ECC inferred: starts state 11
    virtual void vf150();  // 0x802773F8 inferred: starts state 12
    virtual int vf154(int arg); // 0x80277478 inferred: starts state 16 (turn) when the target is not in front
    virtual void vf158();  // 0x802777CC inferred: starts state 13
    virtual void vf15C();  // 0x80277850 inferred: starts state 14
    virtual void vf160();  // 0x802778B0 inferred: starts state 15
    virtual void vf164();  // 0x80276944 inferred: death
    virtual void vf168();  // 0x80277AF0 inferred: starts state 17
    virtual void vf16C();  // 0x80277968 inferred: starts state 18
    virtual RoutePlace_unk* vf170(); // 0x80278330 inferred: thread point farthest from the party
    virtual CharaTask* vf174();      // 0x80276D58 inferred: target of the threads

    void fn_8027688C(); // inferred: clears the counters of the state
    int fn_80277B84(CharaTask* chara); // inferred: chara was eaten
    void fn_80277BC0(); // inferred: effect and sound where the arms touch the ground
    void fn_80277DC4(); // inferred: brings back the reinforcements
    int fn_8027819C();  // inferred: at most one reinforcement is left
    void fn_80278218(atn::Vector3& center); // inferred: center of the party
    int fn_8027851C(CharaTask* chara); // inferred: chara can be seen from the head
    void fn_80278634(CharaTask* chara, int index, int main); // inferred: shoots the thread `index` at chara
    void fn_80279054(); // inferred: releases all the threads
    void fn_802790A0(int index); // inferred: releases the thread `index`
    void fn_80279280(); // inferred: moves the threads
    void fn_802797DC(); // inferred: debug menu

    ForestEnemyList_unk m14B0; // inferred: "ReinforceChara"
    int m14BC;                 // inferred: "ReinforceCenter" (place id)
    ForestPlaceList_unk m14C0; // inferred: "ThreadPoint"
    RoutePlace_unk* m14CC;     // inferred: thread point of state 15
    CharaTask* m14D0;          // inferred: target of the turn
    CharaTask* m14D4;          // inferred: target
    int m14D8;                 // inferred: step of the state
    int m14DC;                 // inferred: frames of the state
    int m14E0;                 // inferred: frames before the next escape
    int m14E4;                 // inferred: attacks
    int m14E8;                 // inferred: argument of the turn
    int m14EC;                 // inferred: frames of the turn
    float m14F0;               // inferred: start angle of the turn
    float m14F4;               // inferred: target angle of the turn
    int m14F8;                 // inferred: moving to a point
    int m14FC;                 // inferred: countdown of the reinforcements
    int m1500;                 // inferred: frames since the last arm effect
    float m1504;               // inferred: 30.0f ("AttackDist")
    int m1508;                 // inferred: 0x3F2 ("AttackID")
    int m150C;                 // inferred: 0x3F2 ("TackleID")
    float m1510;               // inferred: 40.0f ("StopDist")
    float m1514;               // inferred: 180.0f ("GuardTheta")
    int m1518;                 // inferred: 2 ("AttackNum")
    int m151C;
    int m1520;                 // inferred: 3 ("TackleNum")
    int m1524;                 // inferred: 60 ("WaitTime")
    int m1528;                 // inferred: 5 (frames of state 10)
    int m152C;                 // inferred: 150 ("ReinforceCount")
    int m1530;                 // inferred: set after two strong hits
    int m1534;                 // inferred: strong hits
    SpiderThread_unk m1538[4];
    CharaTask* m18E8;          // inferred: target of the threads
    CharaTask* m18EC;          // inferred: caught character
    int m18F0;
    atn::SoundRef m18F4;
    float m18F8;               // inferred: turn speed of the caught character
    float m18FC;               // inferred: 0.8f (pull speed)
    unsigned int m1900;        // inferred: 0x40000000 (collision mask of the threads)
    int m1904;
    ForestCharaList_unk m1908; // inferred: eaten characters
    atn::EfpFile m1914;  // inferred: "/effect/ef_bd003.efp"
    atn::EfpFile m1920;  // inferred: "/effect/ef_uc007.efp"
    atn::EfpFile m192C;  // inferred: "/effect/ef_sc003_01b.efp"
    atn::LH_Binary m1938;      // inferred: data file
    SpiderBaseColor_unk m1940; // inferred: color of the materials (vf14)
    SpiderBaseColor_unk m1950; // inferred: color of the materials when the spider is not down
    ResHandle m1960;           // inferred: debug menu group
    int m1964;
};

#endif
