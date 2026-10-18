#ifndef GAME_CHARAMANAGER_H
#define GAME_CHARAMANAGER_H

#include "atn/Task.h"

class EnemyTask;
class NpcTask;
class PlayerTask;

// inferred from the CharaManager<EnemyTask/NpcTask/PlayerTask> vtables
template <class T>
class CharaManager : public atn::Task {
public:
    CharaManager(atn::Task* parent);
    virtual ~CharaManager() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();

    unsigned char m48[0x54 - 0x48];
};

#endif
