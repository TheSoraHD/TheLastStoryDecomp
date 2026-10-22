#ifndef GAME_EM312DARKSERVANT_H
#define GAME_EM312DARKSERVANT_H

#include "atn/EfpHandle_unk.h"
#include "game/SpecialEnemyBase.h"

// inferred: attack entry of the dark servant (skill id, cool time); names invented
struct ServantAttack_unk {
    ServantAttack_unk() : mSkillId(0), m04(0), mCoolTime(0), m0C(0) {}
    int mSkillId;
    int m04;
    int mCoolTime;
    int m0C;
};
// inferred: attack range entry (0x10 bytes); names invented
struct ServantRange_unk {
    ServantRange_unk() : m00(-1), m04(-1.0f), m08(-1), m0C(0) {}
    int m00;
    float m04;
    int m08;
    int m0C;
};

// inferred from __vt__16Em312DarkServant (0x80788220): servant of Em006DarkMaster (its master at 0x14D4,
// read by Em006DarkMaster.cpp). Data file "/database/chara/special/" + db_name (default
// "db_em312_00_00.csv").
class Em312DarkServant : public SpecialEnemyBase {
public:
    Em312DarkServant(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em312DarkServant();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf30(void* menu);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual int vf10C(HitMsg_unk* msg);

    void fn_8030DEB0(const char* text, int length); // inferred: reads the data file
    void fn_8030E5C8(); // inferred: clears the flags of the servant tree
    void fn_8030F380(); // inferred: shield of the servant (on while the player has mFlag28 or during the all attack)
    int fn_8030FEAC(float radius); // inferred: characters (other side) within `radius`
    void fn_8030FFF0(); // inferred: plays the effect m158C
    void fn_80310084(); // inferred: stops the effect 1
    // inferred: names invented (bodies of fn_8030E490/fn_8030E4AC)
    bool IsDown_unk() { return m14E0 >= m157C; }
    inline void KnockDown_unk();

    EnemyTask* m14D4;          // inferred: master ("master=" id, kind 6)
    int m14D8;
    int m14DC;
    int m14E0;                 // inferred: damage count
    float m14E4;               // inferred: "MoveRadius"
    float m14E8;               // inferred: "AttackLength"
    ServantAttack_unk m14EC[4]; // inferred: vertical, horizontal, all, breath attacks
    ServantRange_unk m152C[4];
    int m156C;
    int m1570;                 // inferred: "BreathIntervalInit"
    int m1574;                 // inferred: "BreathInterval"
    int m1578;                 // inferred: "AttackCountMax"
    int m157C;                 // inferred: "DamageCountMax"
    atn::EfpHandle_unk m1580;  // inferred: "/effect/ef_bs007a.efp"
    atn::EfpHandle_unk m158C;  // inferred: "/effect/ef_bs027a.efp"
};

extern "C" int fn_8030E490(EnemyTask* servant);  // inferred: nonzero once the servant is down
extern "C" void fn_8030E4AC(EnemyTask* servant); // inferred: knocks the servant down
extern "C" void fn_8030E530(EnemyTask* servant); // inferred: revives the servant

#endif
