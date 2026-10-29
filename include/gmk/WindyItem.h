#ifndef GMK_WINDYITEM_H
#define GMK_WINDYITEM_H

#include "atn/EfpHandle_unk.h"
#include "atn/FixedArray.h"
#include "atn/ResLoader_unk.h"
#include "game/GimmickCommon.h"
#include "gmk/ItemSlotAnime.h"
#include "std/vector.h"

extern "C" int rand();
extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix

namespace gmk {

// inferred: area where the wind blows the items ("LOC": position, rotation in degrees and size);
// 0x64 bytes. Names invented.
struct WindyArea_unk {
    WindyArea_unk()
        : mPos(0.0f, 0.0f, 0.0f), mRotY(0.0f), mDir(0.0f, 0.0f, 0.0f), mSizeX(0.0f), mSizeY(0.0f), mSizeZ(0.0f) {}
    inline void Setup_unk();
    atn::Vector3 mPos;     // 0x00
    float mRotY;           // 0x0C
    atn::Vector3 mDir;     // 0x10: inferred: direction of the wind
    float mSizeX;          // 0x1C: inferred: plain floats (the copy is not done as a Vector3)
    float mSizeY;
    float mSizeZ;
    atn::Vector3 mHalf;    // 0x28: inferred: half size
    atn::Matrix3x4 mMtx;   // 0x34: inferred: box matrix
};

// inferred: cubic segment of a path (x(t) = a + t * (b + t * (c + t * d))); names invented
struct WindyCoef_unk {
    float a, b, c, d;
};

// inferred: length of a segment (name invented)
struct WindyLength_unk {
    float v;
};

// inferred: one more class level than the other vectors (four null tests in the destructor of
// WindySlot_unk); name invented
struct WindyLengths_unk : public std::vector<WindyLength_unk> {};

// inferred: path followed by a blown item; 0x5C bytes. Names invented.
struct WindyPath_unk {
    WindyPath_unk() : mCount(0), mScale(0.0f), mT(0.0f), m54(0.0f), m58(0) {}
    int mCount;                         // 0x00: inferred: number of points
    float mScale;                       // 0x04
    atn::Vector3 mStart;                // 0x08
    atn::Vector3 mEnd;                  // 0x14
    std::vector<WindyCoef_unk> mX;      // 0x20
    std::vector<WindyCoef_unk> mY;      // 0x2C
    std::vector<WindyCoef_unk> mZ;      // 0x38
    WindyLengths_unk mLength;           // 0x44: inferred: length of each segment
    float mT;                           // 0x50
    float m54;
    int m58;
};

// inferred: item blown by the wind; 0x188 bytes. Names invented.
struct WindySlot_unk {
    WindySlot_unk() : mState(0), mMtx(lbl_807C7060), m184(0) {}
    int mState;            // 0x00
    atn::Matrix3x4 mMtx;   // 0x04
    WindyPath_unk mPath;   // 0x34
    ItemSlotAnime mAnime;  // 0x90
    int m184;
};

// inferred: items carried by the wind through "LOC" areas ("PARAM", effects "EFP"/"EFF"). The
// name is in the DOL (__vt__Q23gmk9WindyItem, 0xF28 bytes).
class WindyItem : public GimmickCommon {
public:
    WindyItem(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_80436E34(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~WindyItem();
    virtual int vf0C();
    virtual void vf10(); // not decompiled
    virtual void vf14(); // not decompiled (debug drawing)
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);
    // inferred: defined after vf70 (0x804392C0-0x8043A530; fn_80439574 not decompiled)
    void fn_804392C0();
    void fn_80439574(WindySlot_unk* slot);
    void fn_8043A08C(WindySlot_unk* slot);
    // inferred: restarts the wait before the next gust (name invented)
    void ResetWait_unk() {
        mGust = 0;
        mWait = mWaitBase;
        if (mWaitRand > 0) {
            mWait += rand() % mWaitRand;
        }
    }

    atn::FixedArray_unk<WindyArea_unk, 4> mAreas; // 0xF4
    WindySlot_unk mSlots[8];   // 0x288
    int mActive;               // 0xEC8: inferred: slots in use
    int mECC;                  // 0xECC: "PARAM" 1
    int mED0;
    int mED4;
    int mED8;
    int mEDC;                  // 0xEDC: "PARAM" 5
    int mEE0;                  // 0xEE0: "PARAM" 6
    int mEE4;
    int mEE8;                  // 0xEE8: "PARAM" 8
    int mGust;                 // 0xEEC: inferred: gust frames left
    int mGustLength;           // 0xEF0: "PARAM" 7
    float mEF4;                // 0xEF4: "PARAM" 2
    WindyArea_unk* mArea;      // 0xEF8: inferred: area of the player
    int mWait;                 // 0xEFC
    int mWaitBase;             // 0xF00: "PARAM" 3
    int mWaitRand;             // 0xF04: "PARAM" 4
    int mF08;
    atn::EfpFile mEfp;   // 0xF0C: "EFP"
    ResLoader_unk mEff;        // 0xF18: "EFF"
    int mF24;                  // inferred: unused (the object is 0xF28 bytes)
};

} // namespace gmk

#endif
