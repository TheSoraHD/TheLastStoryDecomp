// inferred file name (splits-auto): AiTarget helpers (reset and aimed position); own 0.0f in .sdata2.
#include "game/AiTarget.h"
#include "game/CharaTask.h"
#include "game/SkillEffectManager.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

void AiTarget::fn_8003E918() {
    m04 = 0;
    m08 = 4;
    m0C = 0;
    m1C.x = 0.0f;
    m1C.y = 0.0f;
    m1C.z = 0.0f;
    m28 = 0;
    m2C = 0;
    m30 = -1;
}

atn::Vector3 AiTarget::fn_8003E950() const {
    if (m08 == 0 && m0C != 0) {
        return m0C->mPos + m1C;
    }
    if (m08 == 3) {
        return m10 + m1C;
    }
    if (m08 == 2 && m0C != 0) {
        return ((SkillEffect*)m0C)->m10 + m1C; // inferred: type 2 aims at a magic circle
    }
    return lbl_807C7030;
}
