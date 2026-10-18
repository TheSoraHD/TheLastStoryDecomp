#ifndef ATN_LENSFLARE_H
#define ATN_LENSFLARE_H

#include "math/Vec.h"

namespace atn {

// inferred: one flare element (0x1C bytes), filled from map parts CSV fields
struct LensFlareElem {
    int m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    int m18;
};

class LensFlare {
public:
    LensFlare(int arg, int count, const atn::Vector3& pos, float size); // 0x80077160
    virtual ~LensFlare();

    int fn_800774D4(); // inferred: nonzero while loading
    void fn_80077534(); // inferred: per-frame update

    int m04;
    atn::Vector3 mPos;            // 0x08
    float m14;
    int mCount;          // 0x18
    LensFlareElem* mElems; // 0x1C
    unsigned char m20[0x144 - 0x20]; // 0x20: inferred: 6 objects of 0x30 bytes, then m140
};

} // namespace atn

#endif
