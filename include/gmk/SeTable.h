#ifndef GMK_SETABLE_H
#define GMK_SETABLE_H

#include "math/Vec.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

namespace atn {

// inferred: fixed-size string; assignment copies with strlen + memcpy when the buffers differ
// (inlined in GimmickManager::fn_803EE7FC, 0x803EE7FC). Name invented.
template <int N>
struct FixedString_unk {
    FixedString_unk() { mStr[0] = 0; }
    // inferred: the copy goes through the const char* assignment and an accessor (name invented);
    // otherwise the source pointer is not kept in a saved register (0x803EE7FC)
    FixedString_unk& operator=(const FixedString_unk& other) { return *this = other.c_str(); }
    FixedString_unk& operator=(const char* str) {
        if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
        return *this;
    }
    const char* c_str() const { return mStr; }
    char mStr[N];
};

} // namespace atn

namespace gmk {

// The name is in the DOL (boost::_mfi::mf1<void, GimmickManager, const gmk::SeTable &>, 0x80751EA0).
// inferred: one row of a gimmick sound table (/database/gimmick/*_se.csv), read by
// LoadSeTableTask (0x803EEEDC) and stored by GimmickManager (0x803EE7FC).
struct SeTable {
    // 0x803EEB38: weak copy, emitted after its first user
    SeTable() : mId(0), m04(0), m08(0), m0C(0), mPos(0.0f, 0.0f, 0.0f), mVolume(1.0f), mHash(0), mFlags(0), m48(0) {}
    ~SeTable(); // 0x803EDF7C: empty, defined at the top of game/GimmickManager.cpp

    int mId;   // inferred: gimmick id ("gmXXX_YYY" -> XXX * 1000 + YYY)
    int m04;
    int m08;
    int m0C;
    atn::FixedString_unk<0x20> mName; // 0x10
    atn::Vector3 mPos;                // 0x30
    float mVolume;                    // 0x3C: inferred
    int mHash;                        // 0x40: inferred: hash of a sound name
    unsigned int mFlags;              // 0x44: 2D, LOOP, FADEIN, PITCHUP, PITCHDOWN, CONTINUE
    int m48;
};

} // namespace gmk

#endif
