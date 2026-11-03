#ifndef GAME_RESREF_UNK_H
#define GAME_RESREF_UNK_H

// inferred: reference to a pfs archive of the load manager (4 bytes here: GameScene builds an array
// of six with a weak constructor that only clears mRes, 0x803BF818; the destructor is the weak copy
// __dt__10ResRef_unkFv 0x80117E34 of game/CampSaveLoad.cpp)
struct ResRef_unk {
    ResRef_unk() : mRes(0) {}
    ~ResRef_unk() { fn_80470528(); }
    void fn_80470364(const char* name, int a, int b, int c); // inferred: starts loading `name`
    void fn_80470528();                                      // inferred: drops the reference
    int fn_8047054C();                                       // inferred: nonzero while loading

    void* mRes;
};

#endif
