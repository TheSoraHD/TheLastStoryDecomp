#ifndef ATN_LENSFLARE_H
#define ATN_LENSFLARE_H

#include "atn/Memory.h"
#include "atn/Texture_unk.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

namespace atn {

// inferred: renderer interface (same as the local copies in atn/Rain.cpp and atn/VideoTHP.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C(); // 0x80046218
    virtual void vf10(void* camera) = 0;
    virtual int vf14(void* arg1, void* arg2, void* frustum) = 0;
    virtual void vf18() = 0;
    virtual void vf1C(); // 0x800461FC
};

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
    virtual void vf10(void* camera);
    virtual int vf14(void* arg1, void* arg2, void* frustum) { return 1; }
    virtual void vf18() {}

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
