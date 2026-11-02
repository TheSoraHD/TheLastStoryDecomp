#ifndef GAME_GAMELOADTASK_H
#define GAME_GAMELOADTASK_H

#include "game/GameSaveLoadBackgroundCommon.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

class GameLoadBackground;

// inferred from __vt__12GameLoadTask (0x8078B3A0): debug load menu (slot list, "Debug Data", LAN match)
class GameLoadTask : public GameSaveLoadCommon {
public:
    // inferred: name of the sequence to start after loading, plus a flag (0x44 bytes). It has a constructor,
    // so MWCC copies it member by member; the copies made by boost::function are never destroyed.
    struct Option {
        Option(const char* next, int flag) {
            if (next != mNext) {
                memcpy(mNext, next, strlen(next) + 1);
            }
            mFlag = flag;
        }
        // inferred: user copy constructor (the copies are unrolled into registers, also for arguments)
        Option(const Option& other) { *this = other; }
        char mNext[0x40];
        int mFlag;
    };
    // inferred: temporary used to build an Option (it has an out-of-line destructor, 0x803B29A8, that
    // the copies of Option do not call). Name invented.
    struct OptionTmp_unk : public Option {
        OptionTmp_unk(const char* next, int flag) : Option(next, flag) {}
        ~OptionTmp_unk(); // 0x803B29A8
    };

    GameLoadTask(atn::Task* parent, const Option& option); // 0x803B2870
    virtual ~GameLoadTask();                               // 0x803B29E8
    virtual int vf0C();                                    // 0x803B2A40
    virtual void vf10();                                   // 0x803B2A68
    virtual void vf14();                                   // 0x803B30D8

    static GameLoadTask* fn_803B27F8(atn::Task* parent, const Option& option); // inferred: creator

    int mD68;    // inferred: state of vf10
    Option mD6C; // inferred: sequence to start after loading ("" = load only)
    unsigned char mDB0; // inferred: the list shows the "Debug Data" file
    unsigned char mDB1;
    GameSaveSlot_unk mDB4[17]; // inferred: slot headers of the debug data file
    unsigned char m19EC[0x1A00 - 0x19EC];
    GameSaveFile_unk m1A00; // inferred: file read by the background task
    GameLoadBackground* m14F60;
    unsigned char m14F64[0x14F80 - 0x14F64];
};

// inferred: helpers called with a GameLoadTask by CampSaveLoad, GameOverTask and NewCampPartsTop; those
// units (Codex's) declare them extern "C", so they stay free functions until they are converted together
extern "C" void fn_803B2FBC(GameLoadTask* task, int slot); // inferred: makes the loaded slot current
extern "C" void fn_803B3270(GameLoadTask* task, int slot); // inferred: starts loading `slot`
extern "C" void fn_803B3310(GameLoadTask* task, int slot); // inferred: clears `slot`

#endif
