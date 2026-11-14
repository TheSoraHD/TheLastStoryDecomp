// atn/BoneCalc_unk.cpp: calculo recursivo de las matrices de los huesos de un modelo (nombre de fichero
// inventado).
//
// fn_8009A5CC (0x7D8, llamada por ModelPose_unk::fn_8009A584) recorre los nodos desde la pose de reposo y
// fn_8009ADA4 (0xB94, llamada por ModelPose_unk::fn_8009A5A8 y
// atn::PBoneBasic::vf10) recorre los nodos desde un motion. La funcion vacia del final (0x8009B938) es
// boost::function3<...>::dummy::nonnull de ModelNodeCallback_unk, que emite el `if (callback)`; las tres
// constantes de .data (0x80778930) son su puntero a miembro.
#include "atn/ModelPose_unk.h"
#include "atn/NodeChunk.h"
#include "atn/LH_Motion.h"
#include "atn/EulerToQuat_unk.h"
#include "Event/PoseKey_unk.h"
#include "math/Vec.h"

extern "C" void PSMTXScale(Mtx m, float xS, float yS, float zS);

extern atn::Quaternion_unk lbl_807C7050; // inferred: identity quaternion (atn/Math_unk.cpp)

namespace atn {
// inferred: third argument of the node callbacks: the parent matrix of the node (with the inherited scale)
// and its local rotation; the callbacks of the game do not read it. Names invented.
struct ModelNodeCallbackContext {
    const Matrix3x4* mParent;
    const Quaternion_unk* mRot;
};
} // namespace atn

inline atn::Vector3 ModelJoint_unk::GetScale_unk() const {
    return m34;
}

inline atn::Vector3 ModelJoint_unk::GetRot_unk() const {
    return m40;
}

inline atn::Vector3 ModelJoint_unk::GetTrans_unk() const {
    return m4C;
}

// inferred: rotation of the bind pose of a joint (identity when its euler angles are zero); name invented
static inline void BindRot_unk(atn::Quaternion_unk& quat, const ModelJoint_unk* node) {
    atn::Vector3 rot = node->GetRot_unk();
    bool zero = false;
    if (0.0f == rot.x && 0.0f == rot.y && 0.0f == rot.z) {
        zero = true;
    }
    if (zero) {
        quat = lbl_807C7050;
    } else {
        quat = atn::EulerToQuat_unk(rot);
    }
}

// inferred: flag test of a node edit (name invented)
static inline bool EditFlag_unk(const Event::PoseKey_unk* edit, unsigned int flag) {
    return (edit->m04 & flag) == flag;
}

// inferred: scale matrix (name invented)
static inline atn::Matrix3x4 ScaleMtx_unk(const atn::Vector3& s) {
    atn::Matrix3x4 m;
    PSMTXScale(m.m, s.x, s.y, s.z);
    return m;
}

// inferred: matrix product (name invented)
static inline atn::Matrix3x4 MulMtx_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

// inferred: the node callback m18 (its type is ModelNodeCallback_unk: the safe bool test uses the nonnull of
// that boost::function, 0x8009B938; the header still declares it as boost::function<void()>)
static inline const atn::ModelNodeCallback_unk& NodeCallback_unk(const ModelPose_unk* pose) {
    return reinterpret_cast<const atn::ModelNodeCallback_unk&>(pose->m18);
}

// inferred: computes the matrices of the joint `joint`, its next siblings and their children from the bind pose
// of the model data (scale, euler rotation, position) and the node edits of the pose; `scale` is the scale
// inherited from the parent (null: none). Runs the node callback m18 on the nodes of the sorted list m40.
// NONMATCHING: 99.1% - the empty test of the boost::function callback is materialized in the original
// (cntlzw/srwi., here cmpwi), and the stack slots of the temporaries differ
extern "C" void fn_8009A5CC(ModelPose_unk* pose, int joint, const atn::Matrix3x4* parent,
                            const atn::Matrix3x4* scale) {
    using namespace atn;
    while (joint >= 0) {
        const ModelJoint_unk* node = pose->m00->m48[joint];
        int index = (pose->m08 != 0) ? pose->m08[node->m18] : node->m18;
        Matrix3x4 base = *parent;
        const Event::PoseKey_unk* edit;
        if (pose->m14 != 0 && pose->m0C[index] != -1) {
            edit = (const Event::PoseKey_unk*)pose->m10 + pose->m0C[index];
        } else {
            edit = 0;
        }

        Vector3 pos = node->GetTrans_unk();
        if (edit != 0 && EditFlag_unk(edit, 1)) {
            pos.x = pos.x + edit->m08.x;
            pos.y = pos.y + edit->m08.y;
            pos.z = pos.z + edit->m08.z;
        }
        if (node->m30 == 0) {
            if (scale != 0) {
                Matrix3x4 t;
                PSMTXConcat(base.m, scale->m, t.m);
                base = t;
            }
        } else if (scale != 0) {
            PSMTXMultVec(scale->m, &pos, &pos);
        }

        Quaternion_unk quat;
        BindRot_unk(quat, node);
        if (edit != 0 && EditFlag_unk(edit, 2)) {
            Quaternion_unk rot = EulerToQuat_unk(edit->m14);
            PSQUATMultiply(&rot, &quat, &quat);
        }

        Matrix3x4 rt;
        PSMTXQuat(rt.m, &quat);
        rt.m[0][3] = pos.x;
        rt.m[1][3] = pos.y;
        rt.m[2][3] = pos.z;
        Matrix3x4 local;
        PSMTXConcat(base.m, rt.m, local.m);

        Vector3 s = node->GetScale_unk();
        if (edit != 0 && EditFlag_unk(edit, 4)) {
            s.x = s.x * edit->m20.x;
            s.y = s.y * edit->m20.y;
            s.z = s.z * edit->m20.z;
        }

        if (NodeCallback_unk(pose)) {
            for (unsigned int k = pose->m48; k < pose->m44; k++) {
                if ((unsigned int)index == pose->m40[k]) {
                    ModelNodeCallbackContext ctx;
                    ctx.mParent = &base;
                    ctx.mRot = &quat;
                    NodeCallback_unk(pose)(&local, *(const NodeChunk*)node, ctx);
                    pose->m48 = k + 1;
                    break;
                }
            }
        }

        const Matrix3x4* inherit = 0;
        Matrix3x4 childScale;
        if (1.0f != s.x || 1.0f != s.y || 1.0f != s.z) {
            childScale = ScaleMtx_unk(s);
            inherit = &childScale;
            pose->m04[index] = MulMtx_unk(local, childScale);
        } else {
            pose->m04[index] = local;
        }

        if (node->m20 >= 0) {
            fn_8009A5CC(pose, node->m20, &local, inherit);
        }
        joint = node->m24;
    }
}

// inferred: computes the matrices of the joint `joint`, its next siblings and their children from a motion:
// the position, rotation (quaternion) and scale of each node come from its MotionPose_unk entry in `motion`
// (flags 1, 2, 4) or else from the bind pose; the node edits apply on top (edit flag 8: ignore the motion and use
// the bind pose). `scale` is the scale inherited from the parent (null: none); a joint with m30 set only scales
// its position. Runs the node callback m18 on the nodes of the sorted list m40 and the callback m2C on every
// node.
// NONMATCHING: 98.1% - the empty tests of the two boost::function callbacks are materialized in the original
// (cntlzw/srwi., here cmpwi), and the stack layout and saved registers differ
extern "C" void fn_8009ADA4(ModelPose_unk* pose, int joint, const atn::MotionBuffer* motion,
                            const atn::Matrix3x4* parent, const atn::Matrix3x4* scale) {
    using namespace atn;
    while (joint >= 0) {
        const ModelJoint_unk* node = pose->m00->m48[joint];
        int index = (pose->m08 != 0) ? pose->m08[node->m18] : node->m18;
        Matrix3x4 base = *parent;
        const MotionPose_unk* key = (const MotionPose_unk*)motion + index;
        const Event::PoseKey_unk* edit;
        if (pose->m14 != 0 && pose->m0C[index] != -1) {
            edit = (const Event::PoseKey_unk*)pose->m10 + pose->m0C[index];
        } else {
            edit = 0;
        }

        Vector3 pos = ((key->m00 & 1) == 1) ? key->mPos : node->GetTrans_unk();
        if (edit != 0) {
            if (EditFlag_unk(edit, 8)) {
                pos = node->GetTrans_unk();
            } else if (EditFlag_unk(edit, 1)) {
                pos.x = pos.x + edit->m08.x;
                pos.y = pos.y + edit->m08.y;
                pos.z = pos.z + edit->m08.z;
            }
        }
        if (node->m30 == 0) {
            if (scale != 0) {
                Matrix3x4 t;
                PSMTXConcat(base.m, scale->m, t.m);
                base = t;
            }
        } else if (scale != 0) {
            PSMTXMultVec(scale->m, &pos, &pos);
        }

        Quaternion_unk quat;
        if (edit != 0) {
            if ((key->m00 & 2) == 2 && !EditFlag_unk(edit, 8)) {
                quat = key->m10;
            } else {
                BindRot_unk(quat, node);
            }
            if (EditFlag_unk(edit, 2)) {
                Quaternion_unk rot = EulerToQuat_unk(edit->m14);
                PSQUATMultiply(&rot, &quat, &quat);
            }
        } else if ((key->m00 & 2) == 2) {
            quat = key->m10;
        } else {
            BindRot_unk(quat, node);
        }

        Matrix3x4 rt;
        PSMTXQuat(rt.m, &quat);
        rt.m[0][3] = pos.x;
        rt.m[1][3] = pos.y;
        rt.m[2][3] = pos.z;
        Matrix3x4 local;
        PSMTXConcat(base.m, rt.m, local.m);

        Vector3 s = ((key->m00 & 4) == 4) ? key->mScale : node->GetScale_unk();
        if (edit != 0) {
            if (EditFlag_unk(edit, 8)) {
                s = node->GetScale_unk();
            }
            if (EditFlag_unk(edit, 4)) {
                s.x = s.x * edit->m20.x;
                s.y = s.y * edit->m20.y;
                s.z = s.z * edit->m20.z;
            }
        }

        if (NodeCallback_unk(pose)) {
            for (unsigned int k = pose->m48; k < pose->m44; k++) {
                if ((unsigned int)index == pose->m40[k]) {
                    ModelNodeCallbackContext ctx;
                    ctx.mParent = &base;
                    ctx.mRot = &quat;
                    NodeCallback_unk(pose)(&local, *(const NodeChunk*)node, ctx);
                    pose->m48 = k + 1;
                    break;
                }
            }
        }

        const Matrix3x4* inherit = 0;
        Matrix3x4 childScale;
        if (1.0f != s.x || 1.0f != s.y || 1.0f != s.z) {
            childScale = ScaleMtx_unk(s);
            inherit = &childScale;
            pose->m04[index] = MulMtx_unk(local, childScale);
        } else {
            pose->m04[index] = local;
        }

        if (pose->m2C) {
            ModelNodeCallbackContext ctx;
            ctx.mParent = &base;
            ctx.mRot = &quat;
            pose->m2C(&pose->m04[index], *(const NodeChunk*)node, ctx);
        }

        if (node->m20 >= 0) {
            fn_8009ADA4(pose, node->m20, motion, &local, inherit);
        }
        joint = node->m24;
    }
}
