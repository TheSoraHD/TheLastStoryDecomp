#ifndef CHR_CONTROLDRAGGINGSTART_H
#define CHR_CONTROLDRAGGINGSTART_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: these classes declare no destructor; implicit destructors are emitted at the end of the
// unit in class order (0x801B2E1C), inline ones in reverse order

// inferred: start of dragging another character (motion 0x218)
class ControlDraggingStart : public ControlBase {
public:
    ControlDraggingStart(Chara_unk* chara, int target); // 0x801B2888
    virtual int vf0C();
};

// inferred: start of being dragged (motion 0x21A); continues with ControlDragged
class ControlDraggedStart : public ControlBase {
public:
    ControlDraggedStart(Chara_unk* chara, int target); // 0x801B2984
    virtual int vf0C();
    virtual boost::function<void()> vf10();
};

// inferred: being dragged (motion 0x21B); never finishes by itself
class ControlDragged : public ControlBase {
public:
    ControlDragged(Chara_unk* chara) : ControlBase(chara) {
        chara->m560 = 0x59;
        Motion_unk* motion = &mChara->mMotion;
        motion->m34C = 1;
        motion->m24C = 1.0f;
        motion->fn_80097C08(0, 0x21B, 1, 0, 1, 0.0f, 0.1f);
        motion->m238 = 1.0f;
    }
    virtual int vf0C() { return 0; }
};

} // namespace chr

#endif
