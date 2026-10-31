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

    void fn_800E2FE0(); // inferred: update (the body of vf10)
    void fn_800E610C(); // inferred: moves (or swaps places) in the input direction; the party follows in mode 2
    int fn_800E70EC(int a, int b, int c);
    int fn_800E7214(CharaTask** out, int keep); // inferred: target of the player's attack (keep: prefer m1440) // inferred: auto attack of the player (button attack correction)
    void fn_800E8688(int a, int b, CharaTask* target, const atn::Vector3* pos, int c, int d); // inferred: starts an attack
    void fn_800EAECC(); // inferred: called by vfF0 when mFlag16 is clear
    void fn_800EB270(int id); // inferred: starts the skill `id` (200: the normal attack)
    void fn_800EB7A0(); // inferred: death effect
    void fn_800EBA5C(); // inferred: full reset (revive)
    int fn_800EBE84();  // inferred: applies the motion sets once loaded (1 while loading)
    int fn_800EE180(); // inferred: the character can start an action against its target
    void fn_800EE360(); // inferred: death reward (item of the battle result or message 3 to the gimmick m1438)
    void fn_800EE5AC(class Arms* arms);
    int fn_800EE640(CharaTask* chara); // inferred: chara is a valid target for this character
 // inferred: starts the effect of a weapon when its condition holds
    int fn_800EEFDC();  // inferred: nonzero if the character can be targeted here

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
    // inferred: flags; the C bits (names invented) keep them apart from the B bits of CharaTask
    union {
        unsigned int m14A8; // inferred: flags
        struct {
            unsigned int mFlagC31 : 1;
            unsigned int mFlagC30 : 1;
            unsigned int mFlagC29 : 1;
            unsigned int mFlagC28 : 1;
            unsigned int mFlagC27 : 1;
            unsigned int mFlagC26 : 1; // inferred: set by the constructor and by the weak setter 0x800EC1F4
            unsigned int mFlagC25 : 1;
            unsigned int mFlagC24 : 1; // inferred: set by vf0C for type 2 without m146C
            unsigned int mFlagC23_0 : 24;
        };
    };
};

#endif
