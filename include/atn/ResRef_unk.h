#ifndef ATN_RESREF_UNK_H
#define ATN_RESREF_UNK_H

namespace atn {
class PfsArchive_unk; // atn/PfsArchive_unk.h
}

// inferred: reference to a pfs archive of the load manager (code in atn/ResRef_unk.cpp, 0x80470364-0x80470564).
// One word: Event::Sequence has it at 0x29C and its next member at 0x2A0, and GameScene builds an array of six
// with stride 4 through the weak constructor 0x803BF818 (it only clears mRes); weak destructor
// __dt__10ResRef_unkFv 0x80117E34 (game/CampSaveLoad.cpp). Name invented.
struct ResRef_unk {
    ResRef_unk() : mRes(0) {}
    ~ResRef_unk() { fn_80470528(); }
    void fn_80470364(const char* name, int a, int b, int c); // inferred: starts loading `name`
    void fn_8047043C(const char* name, int a, int b);        // inferred: loads `name`
    void fn_80470528();                                      // inferred: drops the reference
    int fn_8047054C();                                       // inferred: nonzero while loading

    atn::PfsArchive_unk* mRes;
};

#endif
