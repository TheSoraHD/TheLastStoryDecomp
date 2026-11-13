// Out-of-line virtuals of chr::ControlBase (the vtable is emitted here, after vf10, the first
// non-inline virtual). Other non-inline virtuals (vf1C, vf2C, vf30, vf50) are elsewhere.
#include "chr/ControlBase.h"
#include <boost/bind.hpp>

// inferred: vector rotated by a matrix (by value; name invented, as in chr/ControlDropDown.cpp)
inline atn::Vector3 RotY_unk(Mtx m, const atn::Vector3& v) {
    atn::Vector3 r(v);
    PSMTXMultVec(m, &r, &r);
    return r;
}

// inferred: rotates a matrix by euler angles, z then y then x (name invented)
inline void RotZYX_unk(atn::Matrix3x4* p, const atn::Vector3& rot) {
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(p->m, r, t);
        __MtxCopy(p->m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(p->m, r, t);
        __MtxCopy(p->m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(p->m, r, t);
        __MtxCopy(p->m, t);
    }
}

namespace chr {

atn::Vector3 ControlBase::vf14() const {
    return mChara->mPos;
}

// inferred: facing direction of the character (z axis rotated by mRot)
atn::Vector3 ControlBase::vf18() const {
    atn::Matrix3x4 m;
    atn::Matrix3x4* p = &m;
    Chara_unk* chara = mChara;
    p->SetIdentity();
    RotZYX_unk(p, chara->mRot);
    return RotY_unk(p->m, atn::Vector3(0.0f, 0.0f, 1.0f));
}

boost::function<void()> ControlBase::vf10() const {
    return boost::bind(&CharaTask::fn_8016E970, (CharaTask*)mChara, mChara->m564);
}

} // namespace chr
