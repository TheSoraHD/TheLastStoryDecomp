#ifndef GAME_ARENATASK_H
#define GAME_ARENATASK_H

#include "atn/Task.h"
#include "game/CharaStatus.h"

class CharaTask;

// inferred: animation task of the arena results (d2anime, made by fn_801F3FF8); names invented
class ArenaAnim_unk : public atn::Task {
public:
    unsigned char m48[0x58 - 0x48];
    unsigned char m58[0xFC - 0x58]; // 0x58: inferred: layout
    unsigned int mFC;               // 0xFC: inferred: flags
    float m100;                     // 0x100: inferred: current frame
};

// inferred from __vt__9ArenaTask (0x80777358): counts the defeats of each team in the arena
// battles and draws the score ("%d - %d")
class ArenaTask : public atn::Task {
public:
    // inferred: fighter of the arena (0x14 bytes)
    struct Entry_unk {
        Entry_unk() : mChara(0), mDefeated(0), mScore(0), mFlags(0), mSpawn(0) {}
        Entry_unk(CharaTask* chara) : mChara(chara), mDefeated(0), mScore(0), mFlags(0), mSpawn(0) {}
        CharaTask* mChara;
        int mDefeated;
        int mScore;
        unsigned int mFlags;     // inferred: bit 0: waits to be moved to a spawn point
        RoutePlace_unk* mSpawn; // inferred: spawn point in use
    };
    // inferred: pair of message ids (first, last) of a voice group
    struct Voice_unk {
        Voice_unk() : mFirst(0), mLast(0) {}
        int mFirst;
        int mLast;
    };

    ArenaTask(atn::Task* parent); // 0x80043214
    virtual ~ArenaTask();         // 0x800434A8
    virtual int vf0C();           // 0x80043508
    virtual void vf10();          // 0x8004350C
    virtual void vf14();          // 0x800437A4

    static ArenaTask* fn_8004318C(atn::Task* parent);  // inferred: creates the instance
    static bool fn_800431F4(int a, int b);             // inferred: arena map (2, 9)
    void fn_80043A24();                                 // inferred: reads the fighters and the spawn points
    void fn_80043B34();                                 // inferred: starts the battle
    void fn_80043BDC();                                 // inferred: ends the battle
    void fn_80043BF4(int type, CharaTask* victim, CharaTask* attacker); // inferred: a fighter was defeated
    void fn_80043D58(int side);                         // inferred: plays a voice of the group `side`

    // inferred: entry of `chara` (inlined twice in fn_80043BF4); name invented
    Entry_unk* Find_unk(CharaTask* chara) {
        for (unsigned int i = 0; i < mEntries.mCount; i++) {
            Entry_unk* e = &mEntries.mData[i];
            if (e->mChara == chara) {
                return e;
            }
        }
        return 0;
    }

    int mState;                // 0x48: inferred: 1 running, 2 finished
    RouteTable_unk* mTable;    // 0x4C
    int m50;
    int m54;
    int mMode;                 // 0x58
    int m5C;
    FixedArray_unk<Entry_unk, 16> mEntries;         // 0x60
    FixedArray_unk<RoutePlace_unk*, 64> mSpawns;     // 0x1A4
    int mScore0;               // 0x2A8: inferred: score of team 2 (enemies)
    int mScore1;               // 0x2AC: inferred: score of the other team
    int mTimer;                // 0x2B0: inferred: frames of the score zoom
    int mResult;               // 0x2B4
    ArenaAnim_unk* mAnims[4];  // 0x2B8
    int mFirstResult;          // 0x2C8
    FixedArray_unk<Voice_unk, 8> mVoices[4]; // 0x2CC
    int mSe;                   // 0x3DC
    int mVoice;                // 0x3E0
    int m3E4;
};

#endif
