#ifndef GAME_CHARASUB_UNK_H
#define GAME_CHARASUB_UNK_H

#include "math/Vec.h"

class CharaTask;

// inferred: 32-byte block copied by CharaSub_unk::fn_80135338 (two vectors and two floats)
struct CharaSubSeg_unk {
    atn::Vector3 m00;
    atn::Vector3 m0C;
    float m18;
    float m1C;
};

// inferred: helper object owned by CharaTask (allocated with new, 0x90 bytes, kept at
// CharaTask+0x1394). Class name unknown.
class CharaSub_unk {
public:
    CharaSub_unk(CharaTask* owner); // 0x80135284
    ~CharaSub_unk();                // 0x801352F8
    void fn_80135338(const CharaSubSeg_unk& seg, const atn::Vector3& pos);
    void fn_8013537C();
    void fn_80135380();

    CharaTask* mOwner;    // 0x00
    CharaSubSeg_unk m04;  // 0x04
    atn::Vector3 m24;     // 0x24
    unsigned char m30[0x3C - 0x30];
    float m3C;
    int m40;
    int m44;
    unsigned char m48[0x54 - 0x48];
    atn::Vector3 m54;
    atn::Vector3 m60;
    atn::Vector3 m6C;
    atn::Vector3 m78;
    atn::Vector3 m84;
};

bool fn_80135384(const float& a, const float& b); // inferred: sort predicate used by CharaTask
int fn_8013539C();

#endif
