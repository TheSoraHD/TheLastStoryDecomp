#ifndef GAME_ENEMYTASHA_H
#define GAME_ENEMYTASHA_H

#include "atn/EfpHandle_unk.h"
#include "game/CharaStatus.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

// inferred: event flag reference read with SpecialEnemyBase::fn_805A4980 ("G" = global flag); same
// layout as SpecialFlag_unk with a constructor (-1, 0), as ShadeFlag_unk of Em301LShade. Name invented.
struct TashaFlag_unk {
    TashaFlag_unk() : mGlobal(-1), mIndex(0) {}
    int mGlobal;
    int mIndex;
};

// inferred: line "Event" of the data file: the flag that starts the event, the flag that ends it and
// the hit point ratio that triggers it (as JillEvent_unk/TristaEvent_unk). Names invented.
struct TashaEvent_unk {
    TashaFlag_unk m00; // inferred: set when the event starts
    TashaFlag_unk m08; // inferred: ends the event
    float m10;         // inferred: hit point ratio
};

// inferred from __vt__10EnemyTasha (0x807898B0): Tasha (boss fought in the story). Data file
// "/database/chara/special/" + db_name (default "db_enemy_tasha.csv"), debug menu "EnemyTasha". The
// factory allocates 0x16C8 bytes (SpecialEnemyBase.cpp, case 64). Names invented.
class EnemyTasha : public SpecialEnemyBase {
public:
    EnemyTasha(atn::Task* parent, EnemyInfo_unk* info); // 0x803559F8
    virtual ~EnemyTasha();                              // 0x8035B42C
    virtual int vf0C();                                 // 0x80355C98
    virtual void vf10();                                // 0x803563E4
    virtual void vf30(void* menu);                      // 0x8035B034
    virtual void vf40(HitInfo_unk* hit);                // 0x80357004
    virtual int vf68(CharaTask* attacker);              // 0x8035B234
    virtual void vf6C(int kind, CharaTask* other);      // 0x8035B368
    virtual void vfFC();                                // 0x80357344
    virtual void vf100(int state);                      // 0x803579FC
    virtual void vf104();                               // 0x80357F48
    virtual void vf108();                               // 0x80358A5C
    virtual int vf10C(struct HitMsg_unk* msg);          // 0x80356A98

    void fn_80355DD8(const char* text, int length); // inferred: reads the data file
    void fn_80357330();                             // inferred: targets the leader of the party
    int fn_8035738C(float rate, float limit);       // inferred: turns towards the target
    void fn_803576F8(float radius);                 // inferred: picks the warp destination m1534
    int fn_8035A9F4();                              // inferred: free point in front of the target
    int fn_8035AB78();                              // inferred: nonzero when an event can start
    void fn_8035ACB8(int motion, int arg, float speed); // inferred: plays a motion
    void fn_8035ACF0(int state);                    // inferred: effect of the state
    // inferred: names invented (inlined)
    inline void StopEffect_unk();
    inline float HpLimit_unk(int index);
    inline void FireEvent_unk();
    inline bool IsHaste_unk();
    inline bool IsAttacking_unk();
    inline void ClampHp_unk();
    inline bool IsCountered_unk(CharaTask* target);

    int m14D4;              // inferred: "Phase"
    int m14D8;              // inferred: state requested by vf100
    int m14DC;              // inferred: previous m55C
    int m14E0;              // inferred: stance (0-2, 3 = none)
    int m14E4;              // inferred: next stance
    float m14E8;            // inferred: time left in the stance
    CharaTask* m14EC;       // inferred: target
    float m14F0;
    float m14F4;
    float m14F8;
    float m14FC;            // inferred: "AttackDistance"
    atn::Vector3 m1500;     // inferred: free point found by fn_8035A9F4
    atn::Vector3 m150C;     // inferred: position when it was found
    int m1518;              // inferred: "WaitFrame"
    int m151C;              // inferred: "AttackChanceFrame"
    int m1520;              // inferred: "ChaseCancelFrame"
    float m1524;            // inferred: "JumpAttackDistance"
    float m1528;            // inferred: "WarpRadius"
    float m152C;            // inferred: "WarpAngleMin"
    float m1530;            // inferred: "WarpAngleMax"
    atn::Vector3 m1534;     // inferred: warp destination
    int m1540;              // inferred: "CounterWaitFrame"
    unsigned char m1544;    // inferred: counter attack pending
    unsigned char m1545;    // inferred: turn around after the hit
    float m1548;
    int m154C;
    int m1550;              // inferred: frames left before the next guard
    int m1554;              // inferred: guarded hits
    float m1558;
    HitInfo_unk* m155C;     // inferred: "NormalAttack"
    HitInfo_unk* m1560;     // inferred: "CrossAttack"
    HitInfo_unk* m1564;     // inferred: "JumpAttack"
    HitInfo_unk* m1568;     // inferred: "CounterAttack"
    HitInfo_unk* m156C;     // inferred: "SwordDanceAttackBegin"
    HitInfo_unk* m1570;     // inferred: "SwordDanceAttackMiddle"
    HitInfo_unk* m1574;     // inferred: "SwordDanceAttackFinish"
    HitInfo_unk* m1578[3];  // inferred: "StanceSkill" (one per stance)
    HitInfo_unk* m1584;     // inferred: "EventDamageSkillID"
    FixedArray_unk<TashaEvent_unk, 8> m1588; // inferred: "Event"
    int m162C;              // inferred: running event (-1: none)
    int m1630;              // inferred: last event
    atn::EfpHandle_unk m1634; // inferred: "/effect/ef_bs007a.efp"
    atn::EfpHandle_unk m1640; // inferred: "/effect/ef_sp007d.efp"
    atn::EfpHandle_unk m164C; // inferred: "/effect/ef_cm006.efp"
    atn::EfpHandle_unk m1658; // inferred: "/effect/ef_ev027.efp"
    TashaFlag_unk m1664;    // inferred: "ProvokeNotifyFlag"
    TashaFlag_unk m166C;    // inferred: "FullGuardActNotifyFlag"
    TashaFlag_unk m1674;    // inferred: "FullGuardHitNotifyFlag"
    TashaFlag_unk m167C;    // inferred: "FullGuardBreakNotifyFlag"
    TashaFlag_unk m1684;    // inferred: "HasteActNotifyFlag"
    TashaFlag_unk m168C;    // inferred: "HasteHitNotifyFlag"
    TashaFlag_unk m1694;    // inferred: "HasteBreakNotifyFlag"
    TashaFlag_unk m169C;    // inferred: "HasteStateNotifyFlag"
    TashaFlag_unk m16A4;    // inferred: "PowerHitActNotifyFlag"
    TashaFlag_unk m16AC;    // inferred: "PowerHitHitNotifyFlag"
    ResHandle m16B4;        // inferred: debug menu group "EnemyTasha"
    int m16B8;              // inferred: "DebugDisp"
    int m16BC;              // inferred: "DebugLog"
    float m16C0;            // inferred: "BackstepDisRate"
    int m16C4;              // inferred: "AttackTest"
};

#endif
