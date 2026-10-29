#ifndef ATN_EFPDATA_UNK_H
#define ATN_EFPDATA_UNK_H

#include "atn/ResLoader_unk.h"

// inferred: effect of an effect sequence (.efp file, "Effect" block; 0x54 bytes, names invented)
struct EfpElem_unk {
    EfpElem_unk()
        : m00(1), mStart(0), mEnd(2000), mPos0(0.0f), mPos1(0.0f), mPos2(0.0f), mRot0(0.0f), mRot1(0.0f),
          mRot2(0.0f), mScale(1.0f), mColor1(1.0f), mColor2(1.0f), mColor3(1.0f), mColor0(1.0f), mParent(0),
          mNode(0), m40(0), m44(-1) {}
    ~EfpElem_unk(); // 0x80243758

    int m00;          // 0x00: inferred: enabled ("Effect" second argument)
    int mStart;       // 0x04: "Frame" start
    int mEnd;         // 0x08: "Frame" end
    float mPos0;      // 0x0C: "Pos"
    float mPos1;
    float mPos2;
    float mRot0;      // 0x18: "Rot" (degrees, converted to radians)
    float mRot1;
    float mRot2;
    float mScale;     // 0x24: "Scl"
    float mColor1;    // 0x28: "Color" arguments 1-3
    float mColor2;
    float mColor3;
    float mColor0;    // 0x34: "Color" argument 0
    int mParent;      // 0x38: "Parent"
    unsigned int mNode; // 0x3C: inferred: hash of the node name
    int m40;
    int m44;
    ResLoader_unk mLoader; // 0x48: the effect resource
};

// inferred: effect data built from the loaded file (0x14 bytes, functions at 0x8024373C-0x802441CC).
// NOTE: atn/LH_Efp.cpp keeps its own definition with an atn::SimpleArray at 0x00: the destructor of
// LoadEfp destroys that member inline after fn_802437C0, but here a member with a destructor makes
// MWCC emit its weak copy after the constructor, which the original file does not have; so this
// file sees the count and the array as plain fields (same layout).
struct EfpData_unk {
    EfpData_unk();                          // 0x8024373C
    ~EfpData_unk() { fn_802437C0(); }
    void fn_802437B4(const char* path);     // inferred: marks the data as pending
    void fn_802437C0();                     // inferred: releases the elements
    int fn_80243814(void* data, void* size); // inferred: 0 when the data is ready
    EfpElem_unk* fn_802438C4(int index);
    void fn_802438D4(void* data, void* size); // inferred: parses the text of the file
    void fn_80243FAC(int count, int heap, const char* name); // inferred: allocates the effects
    void fn_802440F0(); // inferred: computes the length of the sequence

    unsigned int mCount;  // 0x00
    EfpElem_unk* mElems;  // 0x04: new[] with destructors
    int mState;                          // 0x08: 1 pending, 2 parsed, 3 ready
    int mLength;                         // 0x0C: inferred: frames
    int mEnd;                            // 0x10: inferred: last frame of the effects
};

#endif
