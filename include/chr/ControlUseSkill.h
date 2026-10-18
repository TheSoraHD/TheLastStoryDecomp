#ifndef CHR_CONTROLUSESKILL_H
#define CHR_CONTROLUSESKILL_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: uses a skill (chr/ControlUseSkill.cpp, control id 0x25); size from the operator new call
// in CharaTask::fn_8016125C. The constructor stores data at 0x08, motion at 0x10 (0x6A when
// negative) and f at 0x14.
class ControlUseSkill : public ControlBase {
public:
    ControlUseSkill(Chara_unk* chara, void* data, CharaTask* target, int motion, const char* name, float f); // 0x801C93C4
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

} // namespace chr

#endif
