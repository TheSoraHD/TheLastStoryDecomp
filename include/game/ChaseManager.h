#ifndef GAME_CHASEMANAGER_H
#define GAME_CHASEMANAGER_H

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "atn/ResLoader_unk.h"
#include "math/Vec.h"

class CharaTask;
class ChaserGuard;

// inferred from __vt__12ChaseManager (0x8077CC10): state of the chase events (guards chasing the player);
// singleton kept in lbl_8087F098
class ChaseManager : public atn::Task {
public:
    static ChaseManager* fn_801823A8(atn::Task* parent); // inferred: creates the instance
    ChaseManager(atn::Task* parent); // 0x80182418
    virtual ~ChaseManager();         // 0x801825F0
    virtual int vf0C();              // 0x80182918
    virtual void vf10();             // 0x801829B4
    virtual void vf14() {}           // 0x80183584
    void fn_801826E0(); // inferred: stops the chase
    void fn_80182890(); // inferred: resets the chase
    void fn_80183588(int id); // inferred: adds a chased character (0: the player)
    void fn_80183618(int id); // inferred: sets the goal point
    void fn_80183774(int show); // inferred: shows the goal on the minimap
    void fn_80183930(); // inferred: ends the chase
    void fn_80183948(); // inferred: stops the chase if running
    // inferred: names invented
    void SetEffect_unk(int mode);
    void AddMarker_unk();
    void RemoveMarker_unk();

    int mState;  // 0x48: inferred: 0 idle, 1 chasing, 2 caught, 3 end
    int m4C;
    int m50;
    int m54;
    int mFrames; // 0x58
    int mGoalId; // 0x5C
    atn::Vector3 mGoal; // 0x60
    int m6C;
    ChaserGuard* mCatcher; // 0x70
    atn::FixedArray_unk<ChaserGuard*, 32> mGuards; // 0x74
    CharaTask* mCaught;    // 0xF8
    ResLoader_unk mFC[2];
    ResLoader_unk m114[2];
    ResLoader_unk m12C[2];
    Mtx m144; // inferred: matrix of the first marker effect
    Mtx m174; // inferred: matrix of the second marker effect
    float m1A4;
    float m1A8;
    float m1AC;
    float m1B0;
    float m1B4;
    int mShowGoal; // 0x1B8
    int mEffect;   // 0x1BC
    int mMarker;   // 0x1C0: inferred: minimap marker of the goal
    atn::FixedArray_unk<CharaTask*, 8> mCharas; // 0x1C4
};

// inferred: ChaseManager::sInstance (kept under the label name: CharaTask and ChaserGuard use it)
extern "C" ChaseManager* lbl_8087F098;
// inferred: kept as C symbols (game/ChaserGuard.cpp calls them by these names)
extern "C" void fn_80182788(ChaseManager* mgr, ChaserGuard* guard, CharaTask* target); // inferred: catch
extern "C" void fn_80182874(ChaseManager* mgr); // inferred: release
extern "C" CharaTask* fn_80183860(ChaseManager* mgr, const atn::Vector3* pos); // inferred: chased character

#endif
