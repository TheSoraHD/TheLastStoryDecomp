#ifndef GAME_GIMMICKMANAGER_H
#define GAME_GIMMICKMANAGER_H

#include "atn/Array_unk.h"
#include "atn/Task.h"
#include "game/ResHandle.h"
#include "gmk/SeTable.h"

class GimmickCommon;
class LoadSeTableTask;

// inferred: object passed to GimmickManager::fn_803EE45C (layout unknown)
class CharaTask;

// inferred from __vt__14GimmickManager (0x8078CAB0): the gimmick manager singleton
// (GimmickManager::instance_); gimmicks register themselves in its list
class GimmickManager : public atn::Task {
public:
    GimmickManager(atn::Task* parent); // 0x803EE024
    virtual ~GimmickManager();         // 0x803EE124
    virtual int vf0C();                // 0x803EE1F0
    virtual void vf10();               // 0x803EE318
    virtual void vf14();               // 0x803EE370

    static GimmickManager* SetTask(atn::Task* parent); // RSO: SetTask__14GimmickManagerFPQ23atn4Task; creates the singleton
    static GimmickManager* instance_; // RSO: instance___14GimmickManager (0x8087F4A0)

    void fn_803EE374(void* arg);              // inferred: calls Start of the gimmicks
    void fn_803EE3F0(GimmickCommon* gimmick); // inferred: registers a gimmick
    void fn_803EE41C(GimmickCommon* gimmick); // inferred: unregisters it
    int fn_803EE45C(CharaTask* chara); // inferred: the query is a character (fn_8017CB2C)
    void fn_803EE514(); // inferred: calls vf30 of the gimmicks
    void fn_803EE560(); // inferred: calls vf34 of the gimmicks
    void fn_803EE5AC(int a, int b, int c); // inferred: loads the sound table of a map
    // inferred: stores a row of the sound table; defined in the class body (its address is bound to
    // the loader's callback, the weak copy follows fn_803EE5AC).
    void fn_803EE7FC(const gmk::SeTable& table) { mSeTables.push_back(table); }
    GimmickCommon* fn_803EEE10(int id, int arg); // inferred: finds a gimmick
    void* fn_803EEE44(int type);                 // inferred: debug data of a gimmick type

    GimmickCommon* mHead; // 0x48
    int mCount;           // 0x4C
    int m50;              // inferred: map of the loaded sound table (m50, m54, m58)
    int m54;
    int m58;
    atn::SimpleVector<gmk::SeTable> mSeTables; // 0x5C
    LoadSeTableTask* mLoadTask;              // 0x68
    int mLoading;                            // 0x6C
    ResHandle mRes;                          // 0x70
    void* m74;
    atn::SimpleArray<void*> mTypes;           // 0x78: inferred: per gimmick type, see fn_803EEE44
    GimmickCommon* m80;
    GimmickCommon* m84;
    int m88;
    int m8C;
};

#endif
