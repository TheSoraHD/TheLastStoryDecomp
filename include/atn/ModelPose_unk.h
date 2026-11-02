#ifndef ATN_MODELPOSE_UNK_H
#define ATN_MODELPOSE_UNK_H

#include "math/Vec.h"
#include <boost/function.hpp>

namespace atn {
class Matrix3x4;
struct NodeChunk;
struct ModelNodeCallbackContext;
struct MotionBuffer;
// callback run on the node matrices by CalcMatrixStack (type from the RSO names)
typedef boost::function<void(Matrix3x4*, const NodeChunk&, const ModelNodeCallbackContext&)> ModelNodeCallback_unk;
}

// inferred: joint of the loaded model data (also a local copy in atn/LH_Model.cpp); names invented
struct ModelJoint_unk {
    unsigned char m00[0x14];
    unsigned int m14; // inferred: name hash (CharaFaceControl)
    int m18;          // inferred: index of the node in atn::Model::m21C
    int m1C;          // inferred: parent node
    int m20;          // inferred: first child (-1: none)
    int m24;          // inferred: next sibling (-1: none)
    unsigned char m28[0x40 - 0x28];
    atn::Vector3 m40; // inferred: initial rotation (euler)
};

// inferred: node matrices block of the loaded model data (count at 0x10)
struct ModelMtxBlock_unk {
    unsigned char m00[0x10];
    int m10;
};

// inferred: model data of the loaded file (the 'modl' chunk; also a local copy in atn/LH_Model.cpp)
struct ModelData_unk {
    // inferred: corners of the bounding box by value (weak copies 0x8008CCC0/0x8008CCD4)
    atn::Vector3 GetMax_unk() const { return mMax; }
    atn::Vector3 GetMin_unk() const { return mMin; }

    unsigned char m00[0x10];
    atn::Vector3 mMin; // 0x10: inferred: bounding box (MapPartsModel::vf24)
    atn::Vector3 mMax; // 0x1C
    unsigned char m28[0x30 - 0x28];
    int m30; // inferred: number of materials
    unsigned char m34[0x38 - 0x34];
    ModelMtxBlock_unk* m38;
    unsigned char m3C[0x44 - 0x3C];
    int m44;              // inferred: number of joints (nodes of atn::Model)
    ModelJoint_unk** m48; // inferred: joints
    unsigned char m4C[0x50 - 0x4C];
    const char* m50[1]; // inferred: material names (m30 of them)
};

// inferred: pose of a model, built on the stack to compute the joint matrices (constructor
// 0x800904E0; 0x58 bytes); its two callbacks are destroyed inline. Names invented.
struct ModelPose_unk {
    ModelPose_unk(ModelData_unk* model, atn::Matrix3x4* matrices, const int* remap, boost::function<void()> callback,
                  const int* extra, int extraCount);
    void fn_80090584(atn::ModelNodeCallback_unk callback); // inferred: sets the node callback
    // inferred: node edits (names invented)
    void SetEdits_unk(const short* order, const void* edits, unsigned int count) {
        m10 = edits;
        m14 = count;
        m0C = order;
    }
    void fn_8009A584(const atn::Matrix3x4& root);           // inferred: computes the matrices
    void fn_8009A5A8(const atn::Matrix3x4& root, const atn::MotionBuffer* motion); // inferred: same from a motion

    ModelData_unk* m00;
    atn::Matrix3x4* m04;
    const int* m08;
    const short* m0C;
    const void* m10; // inferred: node edits (Event::PoseKey_unk)
    unsigned int m14;
    boost::function<void()> m18;
    atn::ModelNodeCallback_unk m2C;
    const int* m40;
    int m44;
    int m48;
    unsigned char m4C[0x58 - 0x4C];
};

// inferred: joint walks of atn/BoneCalc_unk.cpp
extern "C" void fn_8009A5CC(ModelPose_unk* pose, int joint, const atn::Matrix3x4* parent, int arg);
extern "C" void fn_8009ADA4(ModelPose_unk* pose, int joint, const atn::MotionBuffer* motion, const atn::Matrix3x4* parent,
                            int arg);

#endif
