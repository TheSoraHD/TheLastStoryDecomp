#ifndef GAME_GAMETASK_H
#define GAME_GAMETASK_H

#include "atn/Task.h"

class GameScene;
class MapTask;
class PlayerTask;
class SelectMember;
struct GameTaskData_unk;
namespace atn {
class FadeCommon;
class FadeCapture;
}

// inferred: object with the leader of the party at 0x50 (GameTask::m10E0)
struct GameLeader_unk {
    unsigned char m00[0x50];
    PlayerTask* mLeader;
};

// inferred from __vt__8GameTask (0x8078A3D0): the field task (lbl_8087F430). Other units still use
// partial views of it (GameField_unk, Field_unk...). Method names invented unless noted.
class GameTask : public atn::Task {
public:
    void fn_8036E430(int a, int b);
    void fn_8036E92C(int arg);          // inferred: opens the save menu
    void fn_8036E964();                 // inferred: closes the save menu
    void fn_8036E994(int type, int a, int b); // inferred: opens the party member menu
    void fn_8036EA04();                 // inferred: opens the library
    void fn_8036EA3C();                 // inferred: leaves the library
    void fn_8036EA6C(int menu);
    void fn_8036EE0C();
    int fn_80370C2C();                  // inferred: the fade is finished
    void fn_80370C6C();
    void fn_80371320(PlayerTask* player); // inferred: new leader
    void fn_80371664();
    void fn_8037177C();
    void fn_803731E8();
    void fn_80373F78();
    // RSO name; inline (weak copy 0x80373148). Defined in game/EnemyQuarkSecond.cpp, the only unit known
    // to inline it (a body here would need GameScene.h in every user of this header).
    inline MapTask* GetCurrentMap();
    int fn_80375184();        // inferred: saving is allowed
    int fn_80376150();
    void fn_80376194(int on);
    int fn_803761A4();
    int fn_803761AC();
    int fn_803761CC();        // inferred: the voices are enabled
    void fn_803761E4(int visible);
    int fn_803761EC();
    void fn_803761F4(int value);
    int fn_803761FC();
    int fn_80376210();
    int fn_80376224();        // inferred: nonzero in the second state (night?)
    int fn_80376238();
    void fn_8037624C(int value);

    unsigned char m48[0x10D8 - 0x48];
    GameScene* m10D8;        // inferred: scene of the field
    unsigned char m10DC[0x10E0 - 0x10DC];
    GameLeader_unk* m10E0;
    unsigned char m10E4[0x10F4 - 0x10E4];
    int m10F4;
    unsigned char m10F8[0x10FC - 0x10F8];
    int m10FC;
    unsigned char m1100[0x1110 - 0x1100];
    int m1110;
    unsigned char m1114[0x1118 - 0x1114];
    int m1118;
    int m111C;
    unsigned char m1120[0x1164 - 0x1120];
    int m1164;
    unsigned char m1168[0x117C - 0x1168];
    int m117C;
    int m1180;
    unsigned char m1184[0x1194 - 0x1184];
    int m1194;
    unsigned char m1198[0x5590 - 0x1198];
    GameTaskData_unk* m5590;
    unsigned char m5594[0x5620 - 0x5594];
    atn::FadeCommon* m5620;  // inferred: fade of the field
    atn::FadeCapture* m5624; // inferred: screen capture fade of the field
    unsigned char m5628[0x5638 - 0x5628];
    int m5638;
    unsigned char m563C[0x566C - 0x563C];
    int m566C;
    unsigned char m5670[0x5684 - 0x5670];
    int m5684;
    int m5688;
    int m568C;
    unsigned char m5690[0x5750 - 0x5690];
    SelectMember* m5750;
    int m5754;
    int m5758;
    unsigned char m575C[0x5768 - 0x575C];
    int m5768;
    unsigned char m576C[0x5770 - 0x576C];
    int m5770;
    int m5774;
    unsigned char m5778[0xCBAC - 0x5778];
    int mCBAC;
};

#endif
