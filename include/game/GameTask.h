#ifndef GAME_GAMETASK_H
#define GAME_GAMETASK_H

#include "atn/Task.h"
#include "math/Vec.h"

class GameScene;
struct Ledge_unk;
class GimmickCommon;
class MapTask;
class PlayerTask;
class CharaTask;
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
    // inferred: start options of the game task (0x24 bytes; names invented)
    struct Option {
        Option(int type, int map, int sub, int pos, int m10_, int m14_, void (*callback)(void*)) {
            mType = type;
            mMap = map;
            mSub = sub;
            mPos = pos;
            m10 = m10_;
            m14 = m14_;
            m18 = 0;
            m1C = 0;
            mCallback = callback;
        }
        int mType;
        int mMap;
        int mSub;
        int mPos;
        int m10;
        int m14;
        int m18;
        int m1C;
        void (*mCallback)(void*);
    };

    void fn_8036E430(int a, int b);
    void fn_8036E92C(int arg);          // inferred: opens the save menu
    void fn_8036E964();                 // inferred: closes the save menu
    void fn_8036E994(int type, int a, int b); // inferred: opens the party member menu
    void fn_8036EA04();                 // inferred: opens the library
    void fn_8036EA3C();                 // inferred: leaves the library
    void fn_8036EA6C(int menu);
    void fn_8036E89C();                 // inferred: closes the shops
    int fn_8036DAA8(CharaTask* chara); // inferred: the special skill is allowed here
    void fn_8036EE0C();
    int fn_80370C2C();                  // inferred: the fade is finished
    void fn_80370C6C();
    void fn_80370320(int flag, int value, int arg); // inferred: sets a field flag
    void fn_80370B78(unsigned int color, int frames, int arg, float speed); // inferred: fade out
    void fn_80370BD0(int frames, int arg, float speed);                     // inferred: fade in
    void fn_80371320(PlayerTask* player); // inferred: new leader
    void fn_80371664();
    void fn_8037177C();
    void fn_803731E8();
    void fn_80373164(CharaTask* owner, const atn::Vector3* pos, int id);
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
    int fn_8036823C();
    void fn_80374710(int arg); // inferred: starts the battle camera (camera mode 6)
    void fn_803748E0(int arg); // inferred: ends the grapple camera
    int fn_8036DAF4(Ledge_unk* route, CharaTask* chara, int check, int point); // inferred: hide link ahead of `chara`
    int fn_8036DB24(Ledge_unk* route, CharaTask* chara, int point, int exclude, const atn::Vector3* dir); // inferred: next hide link
    int fn_8036DB3C(atn::Vector3* pos, int* index, CharaTask* chara);
    void fn_80375194(int id, int kind, float rot); // inferred: sets m5644/m5640 and moves the party to the leader
    int fn_803754C4(); // inferred: the task at 0x567C exists and is not stopped
    int fn_803761BC(); // inferred: debug field
    int fn_803761DC(); // inferred: a mode compared with 1, 2 and 3
    GimmickCommon* fn_8036E098(); // inferred: save point in front of the leader
    void fn_8036FB4C(int entry);        // inferred: moves to an entry of the current map // inferred: hide link held by `chara`
    void fn_80373118(); // inferred: one more frame of delay for the fades // inferred: nonzero on some fields
    struct StrategyAiEntry_unk* fn_80375254(); // inferred: AI strategy row of the field (default id 0)
    void fn_8037529C(unsigned int color0, unsigned int color1, int delay, int wait, int duration); // inferred: starts the capture fade
    int fn_803761FC();
    int fn_80376210();
    int fn_80376224();        // inferred: nonzero in the second state (night?)
    int fn_80376238();
    void fn_8037624C(int value);
    void fn_803766E4();

    int m48; // inferred: kind of field (1, or 2 with m4C 0x44 and m50 1: fn_8036823C)
    int m4C;
    int m50;
    unsigned char m54[0xD18 - 0x54];
    int mD18;
    int mD1C;
    unsigned char mD20[0xF28 - 0xD20];
    CharaTask* mF28;
    unsigned char mF2C[0x1014 - 0xF2C];
    atn::Vector3 m1014;
    unsigned char m1020[0x1070 - 0x1020];
    int m1070;
    unsigned char m1074[0x10C8 - 0x1074];
    float m10C8;
    unsigned char m10CC[0x10D0 - 0x10CC];
    int m10D0; // inferred: value compared with the ranges of the script events (GameEventScript::fn_80398530)
    unsigned char m10D4[0x10D8 - 0x10D4];
    GameScene* m10D8;        // inferred: scene of the field
    unsigned char m10DC[0x10E0 - 0x10DC];
    GameLeader_unk* m10E0;
    unsigned char m10E4[0x10EC - 0x10E4];
    int m10EC; // inferred: a mode (compared with 1, 2 and 3 by CharaTask and ControlPushedDown)
    unsigned char m10F0[0x10F4 - 0x10F0];
    int m10F4;
    int m10F8; // inferred: zero on the debug fields (fn_803761BC)
    int m10FC;
    unsigned char m1100[0x1110 - 0x1100];
    int m1110;
    unsigned char m1114[0x1118 - 0x1114];
    int m1118;
    int m111C;
    unsigned char m1120[0x1160 - 0x1120];
    int m1160; // inferred: 1 while the save points can be used from any side (fn_8036E098)
    int m1164;
    unsigned char m1168[0x117C - 0x1168];
    int m117C;
    int m1180;
    unsigned char m1184[0x1194 - 0x1184];
    int m1194;
    unsigned char m1198[0x54E4 - 0x1198];
    int m54E4;                  // inferred: state of the field (8-11 while an event runs)
    unsigned char m54E8[0x54F0 - 0x54E8];
    int m54F0;                  // inferred: fade direction (1 out, 2 in)
    unsigned char m54F4[0x5590 - 0x54F4];
    GameTaskData_unk* m5590;
    int m5594;
    int m5598;
    unsigned char m559C[0x5614 - 0x559C];
    int m5614;
    unsigned char m5618[0x5620 - 0x5618];
    atn::FadeCommon* m5620;  // inferred: fade of the field
    atn::FadeCapture* m5624; // inferred: screen capture fade of the field
    unsigned char m5628[0x5638 - 0x5628];
    int m5638;
    int m563C;
    int m5640; // inferred: kind passed to fn_8021CF08 (fn_80375254)
    int m5644; // inferred: id passed to fn_8021CF08
    unsigned char m5648[0x566C - 0x5648];
    int m566C;
    unsigned char m5670[0x567C - 0x5670];
    atn::Task* m567C; // inferred: task of the field (fn_803754C4)
    unsigned char m5680[0x5684 - 0x5680];
    int m5684;
    int m5688;
    int m568C;
    unsigned char m5690[0x56D4 - 0x5690];
    CharaTask* m56D4;
    CharaTask* m56D8;
    unsigned char m56DC[0x56E4 - 0x56DC];
    int m56E4;
    int m56E8; // inferred: entry of the map to move to (fn_8036FB4C)
    unsigned char m56EC[0x56F0 - 0x56EC];
    atn::Task* m56F0;
    unsigned char m56F4[0x571C - 0x56F4];
    int m571C;
    unsigned char m5720[0x5750 - 0x5720];
    SelectMember* m5750;
    int m5754;
    int m5758;
    unsigned char m575C[0x5760 - 0x575C];
    int m5760; // inferred: counter of the frames waited (fn_80373118)
    unsigned char m5764[0x5768 - 0x5764];
    int m5768;
    unsigned char m576C[0x5770 - 0x576C];
    int m5770;
    int m5774;
    unsigned char m5778[0xCBAC - 0x5778];
    int mCBAC;
};

#endif
