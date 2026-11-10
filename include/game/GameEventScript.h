#ifndef GAME_GAMEEVENTSCRIPT_H
#define GAME_GAMEEVENTSCRIPT_H

#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Task.h"
#include "game/EventScriptCmd_unk.h"
#include "std/vector.h"

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
// inferred: node of the tree of events (std::map<int, int> laid out by hand; names invented)
struct ScriptEventNode_unk {
    ScriptEventNode_unk* mLeft;
    ScriptEventNode_unk* mRight;
    unsigned int mParent;
    int mKey;
    int mValue;
};
// inferred: 0x40-byte record of GameEventScript::m100 (names invented)
struct ScriptEntry40_unk {
    int mId;
    unsigned char m04[0x40 - 0x04];
};
// inferred: 0x14-byte record of GameEventScript::m12C (names invented)
struct ScriptRecord14_unk {
    CharaTask* mChara;
    unsigned char m04[0x14 - 0x04];
};
// inferred: 0xC-byte record of GameEventScript::m118 (names invented)
struct ScriptNpcFlag_unk {
    int m00;
    unsigned int mFlags;
    int m08;
};

// inferred: draw flags of a map part set by the scripts (bit 0 drawn, bit 1 set; bit 2 locator
// drawn, bit 3 set); names invented
struct ScriptPartsFlag_unk {
    unsigned int mHash;
    unsigned int mFlags;
};

class GameEventScript : public atn::Task {
public:
    virtual ~GameEventScript(); // 0x80394320
    virtual int vf0C();         // 0x80394BA4: inferred: waits for the three script files
    virtual void vf10();        // 0x80398704
    virtual void vf14();        // 0x8039BC6C
    // inferred: compiles a script file `index` (data, size)
    void fn_8039BDB4(int index, const void* data, int size);
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
    ScriptCmd_unk* NextCmd_unk(const ScriptCmd_unk* cmd) {
        return (ScriptCmd_unk*)((const char*)cmd + lbl_8074ED24[cmd->mCmd]);
    }
    int fn_8039BF04(int a, int b, int c, int d, int e);
    int fn_8039C7F0(int a, int b, int c, int d, int e); // inferred: condition of a jump
    void fn_803957F0(int id); // inferred: clears the flag 2 of the NPC set point `id`
    ScriptEntry40_unk* fn_803A2DD8(int id);
    void fn_8039F9A4(CharaTask* chara, const ScriptArgs_unk* args);
    void fn_8039FBFC(CharaTask* chara, const ScriptArgs_unk* args);
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
    void fn_8039D178(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039D274(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039D1E8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039D3E0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039D930(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039D99C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039D9B0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039DA00(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039DA10(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039DA38(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039E280(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039E80C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039E8A8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039EACC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A02C8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A2600(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A2210(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A24A4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6230(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6324(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7C28(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7CC0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A47A4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A81F8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A3AB4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A48A8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5384(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6B9C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6CDC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6E24(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    // inferred: the other commands of the table (declared ahead, written as they are decompiled)
    void fn_8039D4C4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039DB2C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039E328(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039EBB8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039F08C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039F600(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0040(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A064C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A08D4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0D74(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0F4C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A1B20(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A1DB0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A26F4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A2E20(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A3444(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A372C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A3D20(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A3FFC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A4568(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A4A7C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5118(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A54C0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5650(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5804(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5A8C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5CDC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5FF8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6410(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6744(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A68B8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6F90(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7124(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7954(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7D94(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7EB8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7FBC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A8284(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A9C40(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A9DBC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AA5E4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AAAE4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AAC60(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AADCC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AAF74(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AB06C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AB37C(CharaTask* chara);
    void fn_8039E834(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_8039E9C4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039E9D4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039EA50(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039F7D8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_8039F878(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A03A0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A03C8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A04C0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0520(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0AB8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0B1C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0B80(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0BE4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0C48(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0CAC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A0D10(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A2154(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A2378(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A330C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A362C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A3654(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A36D8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A3BF4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A3CA8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A4400(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A4474(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A450C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A49F4(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A39F8(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803A59D4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5A10(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5F14(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A5F88(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A61CC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A61F4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A66B0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A6F68(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A7BB0(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A83FC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A8464(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A8470(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A90E8(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A9138(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A914C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A9558(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A956C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A98C4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803A98EC(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AA82C(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AA954(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AAA74(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AAD58(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AB144(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AB1B0(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB1F4(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AB26C(ScriptResult_unk& result, ScriptCmd_unk* cmd);
    void fn_803AB328(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
    void fn_803AB368(ScriptResult_unk& result, const ScriptCmd_unk* cmd);

    atn::LH_Binary m48[3]; // inferred: script files
    int m60;
    int m64[7];                 // inferred: base address of the events of each block of 10000 ids
    ScriptEventNode_unk* m80;   // inferred: root of the tree of events by id (end node at &m80)
    int m84;
    GameScene* mScene; // 0x88: inferred: scene that owns the script (GameScene::m134)
    int m8C; // inferred: a script is running
    int m90;
    ScriptData_unk* m94; // inferred: compiled script of the running event
    unsigned char m98[0xA8 - 0x98];
    int mA8; // inferred: a message window is open
    unsigned char mAC[0xB4 - 0xAC];
    int mB4;
    int mB8; // inferred: step of the running command
    int mBC;
    int mC0; // inferred: frame counter of the running command
    int mC4; // inferred: flag 0x1000 of the status of the character under control (fn_803A9C40)
    unsigned char mC8[0xD0 - 0xC8];
    int mD0;
    unsigned char mD4[0xDC - 0xD4];
    int mDC; // inferred: kind 0 of GetChara_unk is the leader
    int mE0; // inferred: 0x24 after an event (fn_803A1DB0)
    int mE4;
    int mE8; // inferred: an event of the event manager is running
    int mEC;
    int mF0;
    int mF4;
    int mF8;
    atn::SimpleArray<ScriptEntry40_unk> mFC; // inferred: records looked up by id (fn_803A2DD8)
    unsigned char m104[0x118 - 0x104];
    ScriptNpcFlag_unk* m118; // inferred: one per NpcSetPoint of the scene (fn_803957F0)
    unsigned char m11C[0x12C - 0x11C];
    std::vector<ScriptRecord14_unk> m12C; // inferred: records removed by id (fn_803AB37C)
    unsigned char m138[0x154 - 0x138];
    ScriptPartsFlag_unk m154[64]; // inferred: draw flags of the map parts by name hash (fn_803A68B8)
    unsigned char m354[0x37C - 0x354];
    int m37C; // inferred: cleared while a command writes a flag of the field (fn_803A6F90)
};

#endif
