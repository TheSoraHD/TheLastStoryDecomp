#ifndef ATN_COLLIDATA_UNK_H
#define ATN_COLLIDATA_UNK_H

#include "atn/ColliBase.h"

// inferred: layout of the loaded collision data (LH_ColliObject / LH_ColliTree files), as read by
// atn/atn_colli.cpp (debug drawing, bounds) and atn/ColliCalc_unk.cpp (queries). All names are
// invented; the node layout matches ColliNode_unk of atn/LH_ColliObject.cpp.

// inferred: plain vector of the data (copied member by member, not with the paired-single copy of
// atn::Vector3)
struct ColliVec_unk {
    float x, y, z;
};

// inferred: data vector as an engine vector (member by member, x first)
inline atn::Vector3 __ToVec_unk(const ColliVec_unk& v) {
    atn::Vector3 r;
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    return r;
}

// inferred: attribute of a triangle (id, flags tested against the query mask, debug colour)
struct ColliAttr_unk {
    unsigned int mId;    // 0x00: compared with the attribute id of the query (< 0: any)
    unsigned int mFlags; // 0x04: triangles whose flags match the query mask are skipped
    unsigned int mColor; // 0x08: debug colour (ARGB)
};

// --- collision object (LH_ColliObject): tree of nodes with transforms and triangle lists ---

// inferred: triangle of a collision object (0x44 bytes): the triangle and its attribute
struct ColliObjTriangle_unk {
    atn::ColliTriangle_unk mTri; // 0x00
    ColliAttr_unk* mAttr;        // 0x40
};

// inferred: triangle list of a node, with its bounding sphere
struct ColliObjTriList_unk {
    int m00;
    unsigned int mCount;              // 0x04
    ColliObjTriangle_unk* mTriangles; // 0x08
    ColliVec_unk mCenter;             // 0x0C
    float mRadius;                    // 0x18
};

// inferred: node of a collision object (as ColliNode_unk of atn/LH_ColliObject.cpp)
struct ColliObjNode_unk {
    unsigned char m00[0x08];
    unsigned int mFlags;        // 0x08: 1 translate, 4 rotate, 8 scale
    ColliObjTriList_unk* mList; // 0x0C
    ColliVec_unk mTrans;        // 0x10
    ColliVec_unk mRot;          // 0x1C
    unsigned char m28[0x04];
    ColliVec_unk mScale;        // 0x2C
    ColliObjNode_unk* m38;      // inferred: first child
    ColliObjNode_unk* m3C;      // inferred: next sibling
};

// inferred: loaded collision object (atn::ColliObject::mObject)
struct ColliObjData_unk {
    unsigned char m00[0x0C];
    ColliObjNode_unk* mRoot; // 0x0C
};

// --- collision tree (LH_ColliTree): octree of triangles in world space ---

// inferred: triangle of a collision tree
struct ColliTreeTriangle_unk {
    ColliAttr_unk* mAttr;  // 0x00
    int m04;
    atn::ColliTriangle_unk mTri; // 0x08: corners at 0x08, 0x14, 0x20
};

// inferred: octree node: bounds, 8 children (the first 4 below the middle z) and triangles
struct ColliTreeNode_unk {
    int mCount;                         // 0x00
    ColliVec_unk mBox[2];               // 0x04: min, max
    ColliTreeNode_unk* mChildren[8];    // 0x1C
    ColliTreeTriangle_unk** mTriangles; // 0x3C
};

// inferred: loaded collision tree (atn::ColliTree::mTree)
struct ColliTreeData_unk {
    unsigned char m00[0x08];
    ColliVec_unk mOffset;     // 0x08
    ColliTreeNode_unk* mRoot; // 0x14
};

#endif
