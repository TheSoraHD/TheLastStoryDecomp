#ifndef ATN_MODELPOSE_UNK_H
#define ATN_MODELPOSE_UNK_H

#include "math/Vec.h"
#include <boost/function.hpp>

namespace atn {
class Matrix3x4;
struct NodeChunk;
struct ShaderDesc_unk;
struct ModelNodeCallbackContext;
struct MotionBuffer;
// callback run on the node matrices by CalcMatrixStack (type from the RSO names)
typedef boost::function<void(Matrix3x4*, const NodeChunk&, const ModelNodeCallbackContext&)> ModelNodeCallback_unk;
}

// inferred: joint of the loaded model data (also a local copy in atn/LH_Model.cpp); names invented
struct ModelJoint_unk {
    unsigned char m00[0x10];
    const char* m10;  // inferred: name (fn_80093158)
    unsigned int m14; // inferred: name hash (CharaFaceControl)
    int m18;          // inferred: index of the node in atn::Model::m21C
    int m1C;          // inferred: parent node
    int m20;          // inferred: first child (-1: none)
    int m24;          // inferred: next sibling (-1: none)
    unsigned char m28[0x2C - 0x28];
    int m2C; // inferred: nonzero when the joint has meshes (atn::Model fn_8008EEDC)
    int m30; // inferred: nonzero: the joint does not inherit the scale of its parent (only its position is scaled)
    atn::Vector3 m34; // inferred: initial scale
    atn::Vector3 m40; // inferred: initial rotation (euler)
    atn::Vector3 m4C; // inferred: initial position
    atn::Vector3 m58; // inferred: bounding box of the meshes of the joint (min, max; fn_8008EEDC)
    atn::Vector3 m64;
    int m70; // inferred: > 0 for the joints kept by atn::LoadModel (atn/LH_Model.cpp)
    // inferred: bind pose by value (defined in atn/Model.cpp and atn/BoneCalc_unk.cpp, used by fn_80099010 and
    // the joint walks); names invented
    inline atn::Vector3 GetScale_unk() const;
    inline atn::Vector3 GetRot_unk() const;
    inline atn::Vector3 GetTrans_unk() const;
};

// inferred: node matrices block of the loaded model data (count at 0x10)
struct SkinWeight2_unk;
struct SkinWeight3_unk;
struct SkinWeight4_unk;
// inferred: rigid entry of the skin block: one node per skin matrix (8 bytes)
struct ModelRigidSkin_unk {
    int m00;
    int mNode;
};
// inferred: skin matrix block of the model data (names invented): m10 skin matrices in total, made of
// m14 rigid entries, m1C two-joint, m24 three-joint and m2C four-joint weights (ModelBase::fn_80091684)
struct ModelMtxBlock_unk {
    unsigned char m00[0x10];
    int m10;
    int mRigidCount;
    ModelRigidSkin_unk* mRigid;
    int mSkin2Count;
    SkinWeight2_unk* mSkin2;
    int mSkin3Count;
    const SkinWeight3_unk* mSkin3;
    int mSkin4Count;
    const SkinWeight4_unk* mSkin4;
};

// inferred: node of the model data (atn/LH_Model.cpp)
struct ModelNode_unk {
    unsigned char m00[0x24];
    int m24;
    int m28;
};

// inferred: model data of the loaded file (the 'modl' chunk)
struct ModelData_unk {
    // inferred: corners of the bounding box by value (weak copies 0x8008CCC0/0x8008CCD4)
    atn::Vector3 GetMax_unk() const { return mMax; }
    atn::Vector3 GetMin_unk() const { return mMin; }

    unsigned char m00[0x10];
    atn::Vector3 mMin; // 0x10: inferred: bounding box (MapPartsModel::vf24)
    atn::Vector3 mMax; // 0x1C
    unsigned char m28[0x30 - 0x28];
    int m30; // inferred: number of materials
    atn::ShaderDesc_unk** m34; // inferred: shader descriptions of the materials (m30 of them)
    ModelMtxBlock_unk* m38;
    int m3C;                // inferred: number of nodes
    ModelNode_unk** m40;    // inferred: nodes
    int m44;              // inferred: number of joints (nodes of atn::Model)
    ModelJoint_unk** m48; // inferred: joints
    unsigned char m4C[0x50 - 0x4C];
    const char* m50[1]; // inferred: material names (m30 of them)
};

// inferred: pose of a model, built on the stack to compute the joint matrices (constructor
// 0x800904E0; 0x58 bytes); its two callbacks are destroyed inline. Names invented.
struct ModelPose_unk {
    ModelPose_unk(ModelData_unk* model, atn::Matrix3x4* matrices, const int* remap, boost::function<void()> callback,
                  const unsigned int* extra, int extraCount);
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
    const unsigned int* m40; // inferred: sorted nodes with the callback enabled (Model::mCC)
    int m44;
    int m48;
    unsigned char m4C[0x58 - 0x4C];
};

// inferred: joint walks of atn/BoneCalc_unk.cpp
// (the parameter names are kept: removing them renumbers the @N and @enum$N of every unit that includes this)
extern "C" void fn_8009A5CC(ModelPose_unk* pose, int joint, const atn::Matrix3x4* parent, const atn::Matrix3x4* scale);
extern "C" void fn_8009ADA4(ModelPose_unk* pose, int joint, const atn::MotionBuffer* motion, const atn::Matrix3x4* parent,
                            const atn::Matrix3x4* scale);

#endif
