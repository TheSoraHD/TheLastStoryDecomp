#ifndef GAME_GAMEEVENTSCRIPT_H
#define GAME_GAMEEVENTSCRIPT_H

#include "atn/Task.h"
#include "game/EventScriptCmd_unk.h"

class CharaTask;
class GameScene;


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
    // inferred: script commands (member function pointers of the command table in .data);
    // the (start, step) pairs reset the step counter mB8 and run the first step
    void fn_8039D178(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039D930(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039D99C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039D9B0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039DA00(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039DA10(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039E80C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039E9C4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A03A0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A04C0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0AB8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0B1C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0B80(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0BE4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0C48(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0CAC(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A0D10(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A362C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A61CC(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A6F68(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A8464(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A8470(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A9138(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A914C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A9558(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A956C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A98C4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A98EC(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB368(ScriptResult_unk& result, ScriptCmd_unk* cmd);

    unsigned char m48[0x88 - 0x48];
    GameScene* mScene; // 0x88: inferred: scene that owns the script (GameScene::m134)
    int m8C; // inferred: a script is running
    unsigned char m90[0xB4 - 0x90];
    int mB4;
    int mB8; // inferred: step of the running command
    int mBC;
    int mC0; // inferred: frame counter of the running command
};

#endif
