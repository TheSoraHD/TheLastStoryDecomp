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

namespace chr {

atn::Vector3 ControlBase::vf14() {
    return mChara->mPos;
}

// NONMATCHING: 98.2% - stack slots of the three rotation blocks in reverse order (the original
// puts the z block lowest). Two nested inlines (matrix builder + Rotate_unk as in
// game/CharaTask.cpp fn_801496A8) give the right slots but swap r30/r31 (95.3%).
// inferred: facing direction of the character (z axis rotated by mRot)
atn::Vector3 ControlBase::vf18() {
    atn::Matrix3x4 m;
    atn::Matrix3x4* p = &m;
    Chara_unk* chara = mChara;
    p->SetIdentity();
    if (0.0f != chara->mRot.z) {
        Mtx r;
        Mtx t;
        PSMTXRotRad(r, 'z', chara->mRot.z);
        PSMTXConcat(p->m, r, t);
        __MtxCopy(p->m, t);
    }
    if (0.0f != chara->mRot.y) {
        Mtx r;
        Mtx t;
        PSMTXRotRad(r, 'y', chara->mRot.y);
        PSMTXConcat(p->m, r, t);
        __MtxCopy(p->m, t);
    }
    if (0.0f != chara->mRot.x) {
        Mtx r;
        Mtx t;
        PSMTXRotRad(r, 'x', chara->mRot.x);
        PSMTXConcat(p->m, r, t);
        __MtxCopy(p->m, t);
    }
    return RotY_unk(p->m, atn::Vector3(0.0f, 0.0f, 1.0f));
}

// NONMATCHING: 95.8% - the original loads mChara before saving r31 (prologue scheduling, as in
// chr/ControlCliffhang.cpp vf10); labels are those of chr/ControlCdnStone (weak copies).
boost::function<void()> ControlBase::vf10() {
    return boost::bind(&CharaTask::fn_8016E970, (CharaTask*)mChara, mChara->m564);
}

} // namespace chr
