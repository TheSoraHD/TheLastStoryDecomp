#ifndef GAME_EM001NECROMANCER_H
#define GAME_EM001NECROMANCER_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/CharaStatus.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

struct RoutePlace_unk;

// inferred: classes derived from std::vector (their destructors test the address four times, as
// ElderList_unk of game/Em101ElderZesyaSecond.h); names invented
struct NecroPlaceList_unk : public std::vector<RoutePlace_unk*> {};
struct NecroEnemyList_unk : public std::vector<EnemyTask*> {};

// inferred from __vt__16Em001Necromancer (0x80783D08): the necromancer. It warps between the
// "warp_point" route points, casts three magics ("magic_skill") and revives the "reinforce_chara"
// enemies; "alter" necromancers (copies, m150C points to the master) warp with it. Data file
// "database/chara/special/" + db_name (default "db_em001_04_00.csv"). The factory allocates 0x15C8
// bytes (SpecialEnemyBase.cpp, case 1). Names invented; members keep their offsets.
class Em001Necromancer : public EnemyTask {
public:
    Em001Necromancer(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em001Necromancer();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual int vf68(CharaTask* attacker);
    virtual void vf6C(int kind, CharaTask* other);
    virtual void vfEC();

    inline void SetState_unk(int state);
    inline int NeedReinforce_unk();
    inline int CountNear_unk(float dist);
    void fn_80245F14(); // inferred: moves along the route
    void fn_80246244(); // inferred: chooses the next action
    void fn_80246594(); // inferred: state 7 (chant)
    void fn_8024675C(int index); // inferred: starts state 7 with the magic `index`
    void fn_80246914(atn::Vector3 dir); // inferred: starts state 14 (knocked down)
    void fn_80246C30(int arg); // inferred: starts state 10 (warp out); revives the alters
    void fn_80246E48(); // inferred: starts state 11 (warp in)
    int fn_802470CC(int* index, atn::Vector3* pos, float* rot); // inferred: one of the 3 nearest warp points
    void fn_80247258(); // inferred: revives the dead "reinforce_chara" enemies around

    atn::LH_Binary m14B0;       // inferred: data file
    int m14B8;                  // inferred: "necro_type" (2: no death sequence)
    int m14BC;                  // inferred: "warp_count"
    float m14C0;                // inferred: "warp_count_dist"
    NecroPlaceList_unk m14C4;   // inferred: "warp_point"
    NecroPlaceList_unk m14D0;   // inferred: "reinforce_point"
    NecroEnemyList_unk m14DC;   // inferred: "reinforce_chara"
    FixedArray_unk<Em001Necromancer*, 8> m14E8; // inferred: "alter"
    Em001Necromancer* m150C;    // inferred: master of an alter
    CharaTask* m1510;           // inferred: target (mC58.mC4)
    int m1514;
    int m1518;                  // inferred: frame counter of the state
    int m151C;
    HitInfo_unk* m1520[3];      // inferred: "magic_skill"
    int m152C;                  // inferred: index of the current magic
    int m1530;                  // inferred: magics cast since the last big one
    atn::Vector3 m1534;
    atn::Vector3 m1540;         // inferred: position when the chant started
    int m154C;
    int m1550;                  // inferred: frames before the next big magic
    int m1554;                  // inferred: warp counter
    int m1558;                  // inferred: index of the warp point
    atn::Vector3 m155C;         // inferred: warp destination
    float m1568;                // inferred: warp rotation
    float m156C;                // inferred: timer of state 12
    int m1570;                  // inferred: set once hit
    int m1574;
    atn::EfpFile m1578;   // inferred: "/effect/cm002/cm002_00.efp"
    atn::EfpFile m1584;   // inferred: "/effect/ef_sk026a.efp"
    atn::EfpFile m1590;   // inferred: "/effect/ef_bs007a.efp"
    atn::EfpFile m159C;   // inferred: "appear_effect" (default "/effect/ef_sk036e.efp")
    int m15A8;                  // inferred: loading state
    int m15AC;                  // inferred: dead
    atn::Vector3 m15B0;         // inferred: rotation of the knock down
    atn::Task* m15BC;           // inferred: d2anime "ui_point_slash.d2a" (made by fn_801F3FF8)
    ResHandle m15C0;            // inferred: debug menu group
    int m15C4;
};

#endif
