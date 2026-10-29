#ifndef GAME_ENEMYQUARKFIRST_H
#define GAME_ENEMYQUARKFIRST_H

#include "atn/EfpHandle_unk.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

class PlayerTask;

// inferred: same layout as SpecialFlag_unk of game/SpecialEnemyBase.cpp (global flag, index) with the
// constructor of the ctor stores (-1, 0); name invented
struct QuarkFlag_unk {
    QuarkFlag_unk() : mGlobal(-1), mIndex(0) {}
    int mGlobal;
    int mIndex;
};

// inferred from __vt__15EnemyQuarkFirst: first fight of Quark. Data file
// "/database/chara/special/db_enemy_quark_first.csv" (keys read by fn_80334FDC); the factory allocates
// 0x1588 bytes. Names invented.
class EnemyQuarkFirst : public SpecialEnemyBase {
public:
    EnemyQuarkFirst(atn::Task* parent, EnemyInfo_unk* info); // 0x80334CD8
    virtual ~EnemyQuarkFirst();                               // 0x803387DC
    virtual int vf0C();                                       // 0x80334E74
    virtual void vf10();                                      // 0x80335374
    virtual void vf30(void* menu);                            // 0x80338568
    virtual void vf40(HitInfo_unk* hit);                      // 0x80335788
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir); // 0x803386FC
    virtual int vfA8(int kind);                               // 0x80338770
    virtual void vfFC();                                      // 0x80335940
    virtual void vf100(int state);                            // 0x80335988
    virtual void vf104();                                     // 0x80336518
    virtual void vf108();                                     // 0x80336C8C
    virtual int vf10C(struct HitMsg_unk* msg);                // 0x80335600

    void fn_80334FDC(const char* text, int length); // inferred: reads the data file
    int fn_803383E4(); // inferred: nonzero when the way to the target (mC58.mC4) is free
    inline int FindJumpPlace_unk();
    inline void PlayMotion_unk(int motion, int loop);
    inline void TurnTo_unk(float rate);

    int m14D4; // inferred: "Phase" (0-2, from the hit points)
    int m14D8; // inferred: mode (1 = guards, 2 after the guard is broken)
    int m14DC;
    float m14E0; // inferred: timer of mode 2 (300 frames)
    CharaTask* m14E4; // inferred: target of the counter attack
    float m14E8; // inferred: "AttackDistance"
    atn::Vector3 m14EC; // inferred: end of the free way (fn_803383E4)
    atn::Vector3 m14F8; // inferred: start of the free way
    int m1504; // inferred: "WaitFrame"
    int m1508; // inferred: "ChaseCancelFrame"
    int m150C; // inferred: "CounterWaitFrame"
    HitInfo_unk* m1510; // inferred: "NormalAttack"
    HitInfo_unk* m1514; // inferred: "CounterAttack"
    HitInfo_unk* m1518; // inferred: "PikeSkill"
    HitInfo_unk* m151C; // inferred: "JumpAttack"
    HitInfo_unk* m1520; // inferred: "StanceSkill" (two ids)
    HitInfo_unk* m1524;
    atn::EfpFile m1528; // inferred: "ef_bs007a.efp"
    atn::EfpFile m1534; // inferred: "ef_bu047a.efp"
    EnemyTask* m1540; // inferred: "SecondScriptId"
    QuarkFlag_unk m1544; // inferred: "FullGuardActNotifyFlag"
    QuarkFlag_unk m154C; // inferred: "FullGuardHitNotifyFlag"
    QuarkFlag_unk m1554; // inferred: "FullGuardBreakNotifyFlag"
    QuarkFlag_unk m155C; // inferred: "PowerHitActNotifyFlag"
    QuarkFlag_unk m1564; // inferred: "PowerHitHitNotifyFlag"
    ResHandle m156C; // inferred: debug menu group
    int m1570; // inferred: "DebugDisp"
    int m1574; // inferred: "DebugLog"
    int m1578;
    int m157C; // inferred: the field camera follows a character while set (cleared by vf10)
    PlayerTask* m1580; // inferred: "Manamia"
    unsigned char m1584[0x1588 - 0x1584];
};

#endif
