#ifndef GAME_GAMEEVENTSCRIPT_H
#define GAME_GAMEEVENTSCRIPT_H

#include "atn/Task.h"
#include "game/EventScriptCmd_unk.h"

class CharaTask;
class GameScene;

// inferred: compiled event script (commands from 0xD0; names invented)
struct ScriptData_unk {
    unsigned char m00[0xCC];
    unsigned int mCmdCount; // 0xCC
    ScriptCmd_unk mCmds;    // 0xD0: first command
};


// inferred from __vt__15GameEventScript: event script of a scene (created by fn_80393610 for
// GameScene::m134); only the members used by game/GameScene.cpp, method names invented
class GameEventScript : public atn::Task {
public:
    int fn_80398530(int event);          // inferred: the event is done
    int fn_80398590(int event, int arg); // inferred: the event can start
    int fn_8039CC24(int id);             // inferred: event with the given id
    int fn_803ABFD4(atn::Task* chara);
    int fn_803AC088(); // inferred: count
    int fn_803AC230(); // inferred: count
    int fn_803AC3D8(CharaTask* chara, int a, int b, float range);
    int fn_803AC708(CharaTask* chara);
    int fn_803AC978(CharaTask* chara);
    void fn_803ACD08();

    // inferred: next command after `cmd` (inline; out-of-line copy 0x8039CCB4)
    ScriptCmd_unk* NextCmd_unk(ScriptCmd_unk* cmd) {
        return (ScriptCmd_unk*)((char*)cmd + lbl_8074ED24[cmd->mCmd]);
    }
    int fn_8039BF04(int a, int b, int c, int d, int e);
    // inferred: character named by a script command (kind 0: leader or party member 1, 1: NPC, 2: enemy,
    // 3: party member); defined in game/GameEventScript.cpp
    inline CharaTask* GetChara_unk(int kind, int id) const;
    // inferred: label command (type 9, flag bit 0 clear) with the given id, null if missing
    ScriptCmd_unk* FindLabel_unk(int id) {
        ScriptData_unk* data = m94;
        ScriptCmd_unk* cmd = &data->mCmds;
        for (unsigned int i = 0; i < data->mCmdCount; i++) {
            if (cmd->mCmd == 9 && (cmd->m04 & 1) != 1 && static_cast<ScriptArgs_unk*>(cmd)->m08 == id) {
                return cmd;
            }
            cmd = NextCmd_unk(cmd);
        }
        return 0;
    }
    // inferred: script commands (member function pointers of the command table in .data);
    // the (start, step) pairs reset the step counter mB8 and run the first step
    void fn_8039D178(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039D930(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039D99C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039D9B0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039DA00(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039DA10(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039E80C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039E834(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039E9C4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039E9D4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039EA50(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039F7D8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A03A0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A04C0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0AB8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0B1C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0B80(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0BE4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0C48(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0CAC(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0D10(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A2154(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A2378(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A362C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A3654(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A36D8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A3BF4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A3CA8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A4400(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A450C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A49F4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A59D4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A5A10(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A5F14(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A5F88(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A61CC(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A61F4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A66B0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A6F68(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A7BB0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A83FC(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A8464(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A8470(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A90E8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A9138(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A914C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A9558(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A956C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A98C4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A98EC(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AAA74(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AAD58(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB144(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB1B0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB1F4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB328(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB368(ScriptResult_unk& result, ScriptCmd_unk* cmd);

    unsigned char m48[0x88 - 0x48];
    GameScene* mScene; // 0x88: inferred: scene that owns the script (GameScene::m134)
    int m8C; // inferred: a script is running
    int m90;
    ScriptData_unk* m94; // inferred: compiled script of the running event
    unsigned char m98[0xB4 - 0x98];
    int mB4;
    int mB8; // inferred: step of the running command
    int mBC;
    int mC0; // inferred: frame counter of the running command
    unsigned char mC4[0xDC - 0xC4];
    int mDC; // inferred: kind 0 of GetChara_unk is the leader
    unsigned char mE0[0xE8 - 0xE0];
    int mE8; // inferred: an event of the event manager is running
    int mEC;
    int mF0;
};

#endif
