#include "chr/ControlDraggingStart.h"
#include "game/CharaTask.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>

namespace chr {

ControlDraggingStart::ControlDraggingStart(Chara_unk* chara, int target) : ControlBase(chara) {
    chara->m560 = 0x57;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->fn_80097C08(0, 0x218, 0, 0, 1, 0.0f, 0.1f);
    motion->m238 = 1.0f;
    mChara->fn_801446F0();
    mChara->mF54 = target;
}

int ControlDraggingStart::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->m234;
    return frame >= motion->fn_80097D7C(0);
}

ControlDraggedStart::ControlDraggedStart(Chara_unk* chara, int target) : ControlBase(chara) {
    chara->m560 = 0x58;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->fn_80097C08(0, 0x21A, 0, 0, 1, 0.0f, 0.1f);
    motion->m238 = 1.0f;
    mChara->fn_801446F0();
    mChara->mF50 = target;
}

int ControlDraggedStart::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->m234;
    return frame >= motion->fn_80097D7C(0);
}

// NONMATCHING: 99.7% - the heap copy of the bind object made by the boost::function manager is kept
// in r29 instead of r30; everything else matches.
boost::function<void()> ControlDraggedStart::vf10() {
    ControlDragged* next = new (1, "", "", 0) ControlDragged(mChara);
    // inferred: Chara_unk is CharaTask (the bound type name in the DOL is boost::_bi::value<CharaTask *>)
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

} // namespace chr
