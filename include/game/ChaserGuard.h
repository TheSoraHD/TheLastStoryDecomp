#ifndef GAME_CHASERGUARD_H
#define GAME_CHASERGUARD_H

#include "game/EnemyTask.h"

// inferred: entry returned by fn_803C17FC (route point of the field?)
struct ChaserRoute_unk;

// inferred: the destructor of ChaserGuard tests `this` once more than for a plain std::vector,
// so the vector is wrapped in one more class (name invented)
struct ChaserRouteList_unk : public std::vector<ChaserRoute_unk*> {};

// inferred from __vt__11ChaserGuard (0x8077C968)
class ChaserGuard : public EnemyTask {
public:
    ChaserGuard(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~ChaserGuard();
    virtual void vf10();
    virtual void vf14();

    int m14B0;
    int m14B4;
    atn::Vector3 m14B8;
    atn::Vector3 m14C4;
    int m14D0;
    int m14D4;
    int m14D8;
    atn::Vector3 m14DC; // inferred: spawn position
    float m14E8;        // inferred: spawn rotation
    ChaserRouteList_unk m14EC; // inferred: route read from the parameter text
    int m14F8;
    int m14FC;
    float m1500;
    int m1504;
    int m1508;
    int m150C;
    atn::Vector3 m1510;
};

#endif
