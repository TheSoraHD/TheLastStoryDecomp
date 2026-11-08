#ifndef GAME_GAMEFIELD_UNK_H
#define GAME_GAMEFIELD_UNK_H

// inferred: field of the game (instance lbl_8087F430, code in game/GameTask.cpp): game flags, work
// values and the route tables of the map. Other units declare their own partial views (Field_unk);
// this one has only what game/GameSaveData_unk.cpp reads. Name invented.
#include "game/GameCamera.h"
#include "game/GameTask.h"

struct RouteTable_unk;
namespace atn {
class FadeCapture;
}
class MapTask;
class GameScene;

struct GameField_unk {
    unsigned char m0000[0x6C];
    GameCamera mCamera;         // 0x6C: GameCamera (constructor 0x8037D8B8)
    unsigned char m0D18[0x10D0 - 0xD18];
    int m10D0;                  // inferred: state saved with the flags
    unsigned char m10D4[0x10D8 - 0x10D4];
    RouteTable_unk* m10D8;      // inferred: route tables of the map
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4;                  // inferred: state of the field (8-11 while an event runs)
    unsigned char m54E8[0x5624 - 0x54E8];
    atn::FadeCapture* m5624;    // inferred: screen capture fade of the field (Event::Manager::fn_8047F730)
    unsigned char m5628[0x563C - 0x5628];
    int m563C;                  // inferred: reason of the next map change (0x12: map select of the camp)
    unsigned char m5640[0x5664 - 0x5640];
    int m5664;                  // inferred: set by an event script command (fn_803A61F4)
    // inferred: the route tables are the scene of the field (RouteTable_unk is a partial view of
    // GameScene; Event/Sequence calls GameScene methods on it). Name invented.
    GameScene* GetScene_unk() const { return (GameScene*)m10D8; }
};
extern "C" GameField_unk* lbl_8087F430;


#endif
