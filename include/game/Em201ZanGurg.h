#ifndef GAME_EM201ZANGURG_H
#define GAME_EM201ZANGURG_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__12Em201ZanGurg (0x80784BF8): Zangurg (first fight). Its states (m58C) are run
// by vf10: 7 waits and turns to the leader, 8 attacks at close range, 9/10 charge and cast a magic,
// 11 knocked back, 12 counterattack. Data file "database/chara/special/" + db_name (default
// "db_ch3006_02_00.csv").
class Em201ZanGurg : public EnemyTask {
public:
    Em201ZanGurg(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em201ZanGurg();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);

    void fn_80262E6C(const char* text, int length); // inferred: reads the data file
    void fn_80263CF0(); // inferred: moves along the route
    void fn_80264024(); // inferred: default state
    void fn_802640FC(); // inferred: state 7 (waits facing the leader)
    void fn_8026442C(); // inferred: state 8 (close attack)
    void fn_8026474C(); // inferred: state 9 (charges the magic)
    void fn_802648CC(); // inferred: state 10 (casts the magic)
    void fn_80264C88(); // inferred: state 12 (counterattack)
    void fn_DebugMenu_unk(); // inferred: removed by the linker (only its strings are left)
    inline void StartCounter_unk(CharaTask* attacker);
    inline void StartKnockback_unk(const atn::Vector3& dir);
    inline void SelectAttack_unk();

    atn::LH_Binary m14B0; // inferred: data file
    CharaTask* m14B8;     // inferred: leader of the party (target)
    int m14BC;
    int m14C0;            // inferred: frame counter
    int m14C4;
    int m14C8;
    unsigned char m14CC[0x16CC - 0x14CC];
    int m16CC;            // inferred: "AIType"
    int m16D0;            // inferred: counter of the counterattacks (event flag 0x66 after 3)
    int m16D4;            // inferred: "AttackSkillId"
    int m16D8;            // inferred: "MagicSkillId"
    int m16DC;            // inferred: wait timer
    int m16E0;            // inferred: wait time ("WaitTime")
    int m16E4;            // inferred: attack frequency ("AttackFrequcy")
    int m16E8;            // inferred: nonzero when the next attack can start
    atn::Vector3 m16EC;   // inferred: target position of the magic
    atn::EfpHandle_unk m16F8; // inferred: "/effect/ef_sk026a.efp"
    ResHandle m1704;      // inferred: debug menu group
    int m1708;            // inferred: "DebugDisp"
};

#endif
