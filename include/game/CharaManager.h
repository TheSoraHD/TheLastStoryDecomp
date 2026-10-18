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

    // inferred: appends `chara` to the list (0x8011FD5C for NpcTask); names invented
    void Add_unk(T* chara);
    void Remove_unk(T* chara); // 0x8011FD84 for NpcTask
    // inferred: character with id `id`, null if missing (PlayerTask 0x8011F91C, EnemyTask
    // 0x8011FC10, NpcTask 0x8011FE3C)
    T* Find_unk(int id);
    // inferred: next character id, wraps after 100000 (inlined in NpcTask/PlayerTask ctors)
    int NextId_unk() {
        mNextId++;
        if (mNextId > 100000) {
            mNextId -= 100000;
        }
        return mNextId;
    }

    T* mHead;    // 0x48
    int mCount;  // 0x4C
    int mNextId; // 0x50
};

#endif
