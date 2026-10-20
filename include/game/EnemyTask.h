#ifndef GAME_ENEMYTASK_H
#define GAME_ENEMYTASK_H

#include "game/BattleCharaTask.h"

// inferred: spawn description passed to the enemy constructors (class unknown)
struct EnemyInfo_unk {
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: position (ChaserGuard copies it)
    unsigned char m10[0x14 - 0x10];
    float m14;        // inferred: rotation (ChaserGuard copies it)
    unsigned char m18[0x20 - 0x18];
    void* m20;     // inferred: character data passed to CharaTask
    unsigned char m24[0x2C - 0x24];
    char m2C[1];   // inferred: parameter text ("key=value;..."), length unknown
};

// inferred from __vt__9EnemyTask (0x80789A08)
class EnemyTask : public BattleCharaTask {
public:
    EnemyTask(atn::Task* parent, void* data); // 0x8035B694
    // inferred: creates an enemy from its spawn description (fn_805A5374 first unless `noCheck`)
    static EnemyTask* fn_8035B534(atn::Task* parent, EnemyInfo_unk* info, int noCheck);
    virtual ~EnemyTask();                     // 0x8035B78C
    virtual int vf68(CharaTask* attacker);
    virtual void vf6C(int kind, CharaTask* other);
    virtual int vfF4() { return 0; } // inline: weak copy at the end of ChaserGuard.cpp (0x80180594)

    void fn_8035B808(EnemyTask* enemy); // inferred: append to the list starting at this
    void fn_8035B824(EnemyTask* enemy); // inferred: unlink from the list starting at this
    EnemyTask* fn_8035B854();           // inferred: previous enemy in the manager list (wraps to the last)

    EnemyTask* mNext; // 0x14AC: inferred: next enemy in the EnemyManager list
};

#endif
