#ifndef CHR_CONTROLUSESKILL_H
#define CHR_CONTROLUSESKILL_H

#include "chr/ControlBase.h"
#include "game/HitInfo_unk.h"

namespace chr {

// inferred: uses a skill (chr/ControlUseSkill.cpp, control id 0x25); size from the operator new call
// in CharaTask::fn_8016125C. The constructor stores data at 0x08, motion at 0x10 (0x6A when
// negative) and f at 0x14.
class ControlUseSkill : public ControlBase {
public:
    ControlUseSkill(Chara_unk* chara, void* data, CharaTask* target, int motion, const char* name, float f); // 0x801C93C4
    virtual int vf0C();

    HitInfo_unk* mData; // 0x08: inferred: the skill (fn_80219E6C)
    CharaTask* mTarget; // 0x0C: inferred: the user itself when mData->m02 is 4
    int mMotion;        // 0x10
    float mFrame;       // 0x14: inferred: frame at which the skill fires (24 when negative)
    MtxPtr mNode;       // 0x18: inferred: matrix of the named node (start of the shot)
};

} // namespace chr

#endif
