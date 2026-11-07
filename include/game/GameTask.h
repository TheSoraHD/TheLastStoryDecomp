#ifndef GAME_GAMETASK_H
#define GAME_GAMETASK_H

#include "atn/Task.h"

class GameScene;
class MapTask;
class PlayerTask;

// inferred: object with the leader of the party at 0x50 (GameTask::m10E0)
struct GameLeader_unk {
    unsigned char m00[0x50];
    PlayerTask* mLeader;
};

// inferred from __vt__8GameTask (0x8078A3D0): the field task (lbl_8087F430). Other units still use
// partial views of it (GameField_unk, Field_unk...). Method names invented unless noted.
class GameTask : public atn::Task {
public:
    void fn_80371320(PlayerTask* player); // inferred: new leader
    void fn_80371664();
    void fn_8037177C();
    // RSO name; inline (weak copy 0x80373148). Defined in game/EnemyQuarkSecond.cpp, the only unit known
    // to inline it (a body here would need GameScene.h in every user of this header).
    inline MapTask* GetCurrentMap();
    int fn_80375184();        // inferred: saving is allowed
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
    unsigned char m1198[0x5638 - 0x1198];
    int m5638;
    unsigned char m563C[0x5768 - 0x563C];
    int m5768;
    unsigned char m576C[0x5770 - 0x576C];
    int m5770;
    int m5774;
    unsigned char m5778[0xCBAC - 0x5778];
    int mCBAC;
};

#endif
