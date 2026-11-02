#ifndef GAME_GAMESAVETASK_H
#define GAME_GAMESAVETASK_H

#include "game/GameSaveBackground.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred from __vt__12GameSaveTask (0x8078B508): debug save menu (slot list, overwrite prompt)
class GameSaveTask : public GameSaveLoadCommon {
public:
    GameSaveTask(atn::Task* parent, GameSaveBackground* background)
        : GameSaveLoadCommon(parent), mD68(0), mD6C(0), mD70(background) {
        memset(&mD80, 0, sizeof(GameSaveFile_unk));
        fn_803B8758(-1);
    }
    virtual ~GameSaveTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static GameSaveTask* fn_803B7944(atn::Task* parent, GameSaveBackground* background); // inferred: create
    void fn_803B7FC8(int slot); // inferred: saves in `slot`
    void fn_803B807C(int slot); // inferred: deletes `slot`

    // inferred: saves in the selected slot (inlined twice in vf10)
    void Save_unk() {
        mD80.GetData_unk()->fn_803BDEE4();
        SaveTime_unk time;
        fn_8004B20C(&time);
        mSlots[m48].fn_803BE8C0(mD80.GetData_unk(), &time);
        memcpy(mD80.mHeader, &mSlots[m48], sizeof(GameSaveSlot_unk));
        if (mD70 != 0) {
            mD70->fn_803B8E60(&mD80, m48, 1);
        }
        mD6C = 4;
    }

    int mD68; // inferred: overwrite answer (1 = yes)
    int mD6C; // inferred: state
    GameSaveBackground* mD70;
    unsigned char mD74[0xD80 - 0xD74];
    GameSaveFile_unk mD80;
};

#endif
