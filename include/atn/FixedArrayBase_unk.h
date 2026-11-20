#ifndef ATN_FIXEDARRAYBASE_UNK_H
#define ATN_FIXEDARRAYBASE_UNK_H

namespace atn {

// inferred: non-template base holding the count: the size accessor is a single function for every
// element type (weak copy 0x80372574 of game/GameTask.cpp, called out of line by GameUI, TacView,
// NewCampPartsStatus and FlamingArrow on arrays of different types). Name invented.
// empty() too: weak copy 0x803D6EDC of game/GameUI.cpp, called out of line by GameUI, FlamingArrow and
// Event::Manager::vf10 (on the ring of sequences).
class FixedArrayBase_unk {
public:
    FixedArrayBase_unk() : mSize(0) {}
    unsigned int size() const { return mSize; }
    bool empty() const { return mSize == 0; }

    unsigned int mSize;
};

} // namespace atn

#endif
