#ifndef GAME_NEWCAMPTASK_H
#define GAME_NEWCAMPTASK_H

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "math/Vec.h"
#include "Event/Sequence.h"

namespace atn {
class FadeColor;
struct ModelBase;
} // namespace atn

// inferred: (id, value) pair of the camp (0x8 bytes, fn_80530384); names invented
struct NewCampPair_unk {
    int mId;
    int m04;
};

// inferred from __vt__11NewCampTask (0x80793808): camp menu task, parent of the NewCampParts* (0x1380
// bytes, fn_8052A888). Members keep their offsets (names unknown).
class NewCampTask : public atn::Task {
public:
    void fn_8052C9C4(); // inferred: opens/closes the map select of the field (toggled by m133C)
    void fn_8052CA6C();
    void fn_8053032C(atn::Vector3 pos, atn::Vector3 rot); // inferred: starts a camera path
    NewCampPair_unk& fn_80530384(int id);
    void fn_80530CD0(int duration); // inferred: fades out
    void fn_80530D30(int duration); // inferred: fades in
    void fn_80530D88(int set, atn::ModelBase* model);

    unsigned char m48[0x5C - 0x48];
    int m5C;
    unsigned char m60[0x64 - 0x60];
    atn::FadeColor* m64; // inferred: fade of the camp (FadeColor::Create in the constructor)
    int m68;             // inferred: faded out
    unsigned char m6C[0x1224 - 0x6C];
    atn::FixedArray_unk<NewCampPair_unk, 8> m1224;
    unsigned char m1268[0x12AC - 0x1268];
    int m12AC;
    Event::EffectLists_unk m12B0; // inferred: camera path
    float m12D0;
    atn::Vector3 m12D4;
    atn::Vector3 m12E0;
    unsigned char m12EC[0x133C - 0x12EC];
    int m133C; // inferred: the map select is open
    unsigned char m1340[0x1380 - 0x1340];
};

#endif
