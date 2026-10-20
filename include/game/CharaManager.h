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
    // inferred: inline (weak copies at the end of game/CharaManager_PlayerTask.cpp)
    virtual int vf0C() { return fn_800D3FA4(); }
    virtual void vf10(); // inferred: counts the frames each character stays hidden (m1418)
    virtual void vf14();
    // The members are defined in game/CharaManager_PlayerTask.cpp (explicit instantiation of the
    // members each manager uses).
    // inferred: appends `chara` to the list (0x8011FD5C for NpcTask); names invented
    void Add_unk(T* chara);
    void Remove_unk(T* chara); // 0x8011FD84 for NpcTask
    virtual void vf1C();
    void DeleteAll_unk();      // inferred: fn_800D2338 on every character (0x8011F87C for PlayerTask)
    T* Last_unk();             // inferred: last character, null with fewer than two (0x8011F8BC)
    T* FindByChara_unk(int chara); // inferred: compares CharaTask::m50 (0x8011F8F4)
    // inferred: character with id `id`, null if missing (PlayerTask 0x8011F91C, EnemyTask
    // 0x8011FC10, NpcTask 0x8011FE3C)
    T* Find_unk(int id);
    T* FindByIndex_unk(int index); // inferred: 0x8011F944
    T* FindByName_unk(int name);   // inferred: FindByChara_unk(fn_80219544(name)) (0x8011F970)
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
