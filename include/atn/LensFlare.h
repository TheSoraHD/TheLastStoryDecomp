#ifndef ATN_LENSFLARE_H
#define ATN_LENSFLARE_H

#include "atn/Memory.h"
#include "atn/Texture_unk.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "atn/IRender.h"

namespace atn {


// inferred: one flare element (0x1C bytes), filled from map parts CSV fields
struct LensFlareElem {
    LensFlareElem(); // 0x80077418

    int m00;   // inferred: texture (1-based, 0 = none)
    float m04; // inferred: size
    float m08; // inferred: position along the sun-to-center axis
    float m0C; // inferred: color (r, g, b, a)
    float m10;
    float m14;
    float m18;
};

// inferred: array of elements {count, data} (name invented)
struct LensFlareElems_unk {
    LensFlareElems_unk() : mCount(0), mData(0) {}
    ~LensFlareElems_unk() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mCount = 0;
    }
    void Resize_unk(unsigned int count) {
        if (mData != 0) {
            delete[] mData;
        }
        mCount = count;
        if (count != 0) {
            mData = new ((MemoryTag)6, "", "", 0) LensFlareElem[count];
        } else {
            mData = 0;
        }
    }
    LensFlareElem& operator[](unsigned int i) { return mData[i]; }

    unsigned int mCount;
    LensFlareElem* mData;
};

class LensFlare : public IRender {
public:
    LensFlare(int arg, int count, const atn::Vector3& pos, float size); // 0x80077160
    virtual ~LensFlare();
    virtual void vf10(RenderView_unk* view);
    virtual int vf14(void* arg1, void* arg2, void* frustum) { return 1; }
    virtual void vf18(RenderView_unk* view) {}

    int fn_800774D4(); // inferred: nonzero while loading
    void fn_80077534(); // inferred: per-frame update

    int m04;                    // inferred: 1 = follows the sun
    atn::Vector3 mPos;          // 0x08
    float m14;                  // inferred: radius of the visibility test
    LensFlareElems_unk mElems;  // 0x18
    Texture_unk m20[6];         // 0x20
    ResHandle m140;             // inferred: debug menu group
};

} // namespace atn

#endif
