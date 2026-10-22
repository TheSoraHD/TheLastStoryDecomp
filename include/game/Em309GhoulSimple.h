#ifndef GAME_EM309GHOULSIMPLE_H
#define GAME_EM309GHOULSIMPLE_H

#include "atn/EfpHandle_unk.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

// inferred: fixed string of 0x80 bytes, assigned inline (strlen + memcpy); name invented
struct GhoulStr_unk {
    GhoulStr_unk() { mStr[0] = '\0'; }
    GhoulStr_unk& operator=(const char* str);
    char mStr[0x80];
};

// inferred: same layout as SpecialFlag_unk (global flag, index), with a constructor; name invented
struct GhoulFlag_unk {
    GhoulFlag_unk() : mGlobal(-1), mIndex(0) {}
    int mGlobal;
    int mIndex;
};

// inferred: pair cleared by the constructor (7 of them at 0x1580); names invented
struct GhoulPair_unk {
    GhoulPair_unk() : m00(0), m04(0) {}
    GhoulPair_unk(int a, int b) : m00(a), m04(b) {}
    int m00;
    int m04;
};

// inferred from __vt__16Em309GhoulSimple (0x80787DF0): ghoul (the debug menu calls it "Em309
// GhoulLight"). Data file "/database/chara/special/" + db_name (default "db_em309_01_00.csv").
class Em309GhoulSimple : public SpecialEnemyBase {
public:
    Em309GhoulSimple(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em309GhoulSimple();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf30(void* menu);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf54();
    virtual atn::Vector3 vf64(int center);
    virtual float vf90();
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual int vf10C(HitMsg_unk* msg);

    void fn_80306078(const char* text, int length); // inferred: reads the data file
    inline bool IsDownState_unk();

    CharaTask* m14D4;          // inferred: target when the state starts
    atn::Vector3 m14D8;        // inferred: direction of the attack
    float m14E4;               // inferred: "AttackDistance"
    int m14E8;                 // inferred: "AttackNormalSkillId"
    int m14EC;
    int m14F0;                 // inferred: "AttackNormalCoolTime"
    int m14F4;
    int m14F8;                 // inferred: "AttackLongSkillId"
    int m14FC;
    int m1500;                 // inferred: "AttackLonglCoolTime"
    int m1504;
    int m1508;                 // inferred: "AttackRollingSkillId"
    int m150C;                 // inferred: "AttackLongChargeTime"
    int m1510;
    int m1514;
    int m1518;
    int m151C;
    int m1520;
    int m1524;
    int m1528;                 // inferred: "WaitFrame"
    atn::EfpHandle_unk m152C;  // inferred: "/effect/ef_sk030b.efp"
    int m1538;
    float m153C;               // inferred: "Speed"
    atn::Vector3 m1540;        // inferred: backwards direction (state 12)
    float m154C;
    float m1550;
    float m1554;
    // inferred: SpecialFlag_unk (global flag, index), defined in SpecialEnemyBase.cpp
    GhoulFlag_unk m1558;       // inferred: "HeadAttackStartFlag"
    GhoulFlag_unk m1560;       // inferred: "HeadAttackNotifyFlag"
    GhoulFlag_unk m1568;       // inferred: "SummonNotifyFlag"
    int m1570;
    // inferred: "SummonEnemy" (id) and "SummonWarpPoint" per enemy
    FixedArray_unk<GhoulPair_unk, 8> m1574;
    float m15B8;               // inferred: "SummonHp"
    GhoulStr_unk m15BC;        // inferred: "SummonEfp"
    atn::EfpHandle_unk m163C;  // inferred: effect of the summon
    int m1648;
    int m164C;                 // inferred: loading step
    ResHandle m1650;           // inferred: debug menu group
    int m1654;
};

#endif
