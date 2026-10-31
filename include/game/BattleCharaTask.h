#ifndef GAME_BATTLECHARATASK_H
#define GAME_BATTLECHARATASK_H

#include "game/CharaTask.h"

// inferred from __vt__15BattleCharaTask (0x80779B68)
class BattleCharaTask : public CharaTask {
public:
    BattleCharaTask(atn::Task* parent, int type, void* data); // 0x800E2AF8
    virtual ~BattleCharaTask();                               // 0x800E2D4C
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf2C(const char* params, int arg); // inferred: calls CharaTask::vf2C
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf44();
    virtual int vfA4(); // inferred: returns 1/0 (0x800EE494)
    virtual int vfE4(); // inferred: returns 1/0 (EnemySeaLizerd 0x80355954 returns 1 or tail-calls it)
    virtual void vfE8(HitMsg_unk* msg); // inferred: a hit was received (Em002Skelton reads the message)
    virtual void vfEC();
    virtual void vfF0();

    void fn_800EAECC(); // inferred: called by vfF0 when mFlag16 is clear

    // inferred: members cleared by the constructor (0x800E2AF8); names keep the offsets
    int m1424;
    int m1428;
    int m142C;
    int m1430;
    int m1434; // inferred: frame counter (Em209BigGolem counts the death sequence down from 30)
    atn::Task* m1438; // inferred: task created by the EnemyTask constructor (fn_803EC16C)
    int m143C;
    int m1440;
    int m1444;
    int m1448;
    int m144C;
    int m1450;
    int m1454;
    float m1458;
    int m145C;
    int m1460;
    int m1464;
    int m1468;
    int m146C;
    short m1470;
    short m1472;
    int m1474;
    int m1478;
    float m147C;
    float m1480;
    float m1484;
    int m1488;
    atn::Vector3 m148C; // inferred: copied from mColli.m3C
    float m1498;        // inferred: copied from mColli.m54
    float m149C;
    int m14A0;
    int m14A4;
    unsigned int m14A8; // inferred: flags
};

#endif
