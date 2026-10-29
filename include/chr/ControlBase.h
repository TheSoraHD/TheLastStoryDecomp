#ifndef CHR_CONTROLBASE_H
#define CHR_CONTROLBASE_H

#include "math/Vec.h"
#include "game/CharaTask.h"
#include <boost/function.hpp>

namespace atn {
struct Model;
}

namespace chr {

// The character controlled by the chr::Control* classes is CharaTask (same layout: model at 0xB0,
// CharaStatus at 0x7D4, RouteWalker_unk at 0x1030, CharaLook_unk at 0x10D8; see the CharaTask
// constructor at 0x801354B4). The old name is kept as an alias.
typedef ::CharaTask Chara_unk;


extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector returned by ControlBase::vf4C

// inferred from __vt__Q23chr11ControlBase (0x8077DC90). vf44, vf48, vf4C and vf5C are inline
// (weak copies first emitted in chr/ControlBreakOpen.cpp at 0x80188060).
class ControlBase {
public:
    ControlBase(Chara_unk* chara) : mChara(chara) {}
    virtual ~ControlBase() {}
    virtual int vf0C() = 0; // inferred: update, returns nonzero when finished
    // inferred: returns the action to run when the control finishes (0x801B2A80 binds
    // CharaTask::fn_80178208 to the next control)
    virtual boost::function<void()> vf10();
    virtual atn::Vector3 vf14(); // inferred: a position (CharaTask::fn_80178088)
    virtual atn::Vector3 vf18(); // inferred: a direction (CharaTask::fn_80158E1C)
    virtual void vf1C();
    // inferred: vf20, vf24 and vf28 are inline (weak copies emitted in game/CharaTask.cpp at
    // 0x80158C9C, 0x80158D58 and 0x80158E14)
    virtual float vf20() { return 0.0f; }
    virtual int vf24() { return 0; }
    virtual int vf28() { return 0; }
    virtual void vf2C();
    virtual int vf30(); // inferred: returns an int (0 in ControlBase, 0x800E60FC; compared with 5 by callers of CharaTask::fn_8017A450, tested by vf5C)
    // inferred: inline (weak copies 0x8017A220 and 0x8017A2F8 in game/CharaTask.cpp, read through
    // CharaTask::fn_8017A160 and fn_8017A228)
    virtual int vf34() { return 0; }
    virtual int vf38(int arg) { return -1; }
    // inferred: vf3C, vf40 and vf58 are inline too (weak copies 0x801446E8, 0x80147A0C, 0x80147A10 in
    // game/CharaTask.cpp)
    virtual int vf3C() { return 0; }
    virtual void vf40() {}
    virtual int vf44() { return 0; }
    virtual int vf48() { return 0; }
    virtual atn::Vector3 vf4C() { return lbl_807C7030; }
    virtual int vf50(); // inferred: returns an int (0 in ControlBase, 0x800EE5A4; ControlMagic)
    virtual int vf54() { return 0; } // inferred: weak copy 0x80151180 (tested by CharaTask::fn_8015076C)
    virtual int vf58() { return 0; }
    virtual void vf5C() {}

    Chara_unk* mChara; // 0x04
};

} // namespace chr

#endif
