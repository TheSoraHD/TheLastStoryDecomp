#ifndef GAME_EVENTSCRIPTCMD_UNK_H
#define GAME_EVENTSCRIPTCMD_UNK_H

// inferred: size in bytes of each event script command, by command type (.rodata of
// game/GameEventScript, 0x8074ED24)
extern "C" int lbl_8074ED24[];

// inferred: header of one compiled event script command; the arguments follow it and their
// layout depends on the type (names invented)
struct ScriptCmd_unk {
    unsigned int mCmd; // 0x00: index of the command (table of member function pointers in .data)
    unsigned int m04; // inferred: flags (bit 0: disabled label)
};

// inferred: generic view of the integer arguments of a command
struct ScriptArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    int m10;
    unsigned int m14; // inferred: flags in some commands
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    int m2C;
};

// inferred: view of the arguments of the commands with float arguments after an integer
struct ScriptArgsF_unk : public ScriptCmd_unk {
    int m08;
    float m0C;
    float m10;
    float m14;
};

// inferred: what a command step returns: the next command to run (null: stay on this one) and
// whether the script waits (1) before running it
struct ScriptResult_unk {
    ScriptResult_unk(ScriptCmd_unk* next, int wait) : mNext(next), mWait(wait) {}
    ScriptResult_unk(const ScriptResult_unk& r) : mNext(r.mNext), mWait(r.mWait) {}
    // inferred: goes on with the command after `cmd` without waiting
    void SetNext_unk(ScriptCmd_unk* cmd, int wait = 0) {
        int& size = lbl_8074ED24[cmd->mCmd];
        mWait = wait;
        mNext = (ScriptCmd_unk*)((char*)cmd + size);
    }
    ScriptCmd_unk* mNext;
    int mWait;
};

#endif
