#ifndef GAME_GAMEFIELD_UNK_H
#define GAME_GAMEFIELD_UNK_H

// inferred: field of the game (instance lbl_8087F430, code in game/GameTask.cpp): game flags, work
// values and the route tables of the map. Other units declare their own partial views (Field_unk);
// this one has only what game/GameSaveData_unk.cpp reads. Name invented.
struct RouteTable_unk;
struct GameMap_unk;

struct GameField_unk {
    unsigned char m0000[0x10D0];
    int m10D0;                  // inferred: state saved with the flags
    unsigned char m10D4[0x10D8 - 0x10D4];
    RouteTable_unk* m10D8;      // inferred: route tables of the map
};
extern "C" GameField_unk* lbl_8087F430;

extern "C" void fn_80370094(GameField_unk* field, int state);           // inferred: sets m10D0
extern "C" int fn_80370174(GameField_unk* field, int flag);             // inferred: flag value
extern "C" void fn_80370320(GameField_unk* field, int flag, int value, int arg); // inferred: sets a flag
extern "C" int fn_80370A78(GameField_unk* field, int index);            // inferred: work value
extern "C" GameMap_unk* fn_80373148(GameField_unk* field);              // inferred: current map

#endif
