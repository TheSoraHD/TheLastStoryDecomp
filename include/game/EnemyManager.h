#ifndef GAME_ENEMYMANAGER_H
#define GAME_ENEMYMANAGER_H

#include "game/CharaManager.h"
#include "game/ResHandle.h"

class EnemyManager : public CharaManager<EnemyTask> {
public:
    EnemyManager(atn::Task* parent) : CharaManager<EnemyTask>(parent), m58(0) {}
    virtual ~EnemyManager();
    virtual int vf0C();

    // inferred: creates the singleton under `parent` if it does not exist yet
    static EnemyManager* fn_80334B38(atn::Task* parent);

    ResHandle mRes; // 0x54
    void* m58;
    int m5C;

    static EnemyManager* sInstance; // inferred name (0x8087F408)
};

#endif
