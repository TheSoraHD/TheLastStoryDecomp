// inferred file name (splits-auto): AI helpers used by AI/AiCommon_unk and game/AiTarget.
#include "game/CharaTask.h"

// inferred: game settings (lbl_8087F0A8), only the field read here
struct AiSettings_unk {
    char pad00[0xA4];
    int mA4; // inferred: bonus added when the character has state 0x2000
};
extern "C" AiSettings_unk* lbl_8087F0A8;

// inferred: values set by game/AiTarget and read by AI/AiCommon_unk (types unknown)
extern "C" {
void* lbl_8087EE60;
void* lbl_8087EE64;
}

#define AI_MAX(a, b) ((a) > (b) ? (a) : (b))
#define AI_MIN(a, b) ((a) < (b) ? (a) : (b))

extern "C" void fn_800142C4(void* p) {
    lbl_8087EE60 = p;
}

extern "C" void fn_800142CC(void* p) {
    lbl_8087EE64 = p;
}

// inferred: priority bonus that grows as the status value (hit point ratio?) drops below 0.7
extern "C" int fn_800142D4(CharaTask* chara) {
    int ret = 0;
    if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        ret = lbl_8087F0A8->mA4;
    }
    if (0.0f < chara->mStatus.fn_8012DB04() && chara->mStatus.fn_8012DB04() < 1.0f) {
        ret += (int)(30.0f * AI_MIN(AI_MAX(1.0f - (0.3f + chara->mStatus.fn_8012DB04()), 0.0f), 1.0f));
    }
    return ret;
}

// inferred: same as fn_800142D4 with a factor of 60
extern "C" int fn_800143E8(CharaTask* chara) {
    int ret = 0;
    if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        ret = lbl_8087F0A8->mA4;
    }
    if (0.0f < chara->mStatus.fn_8012DB04() && chara->mStatus.fn_8012DB04() < 1.0f) {
        ret += (int)(60.0f * AI_MIN(AI_MAX(1.0f - (0.3f + chara->mStatus.fn_8012DB04()), 0.0f), 1.0f));
    }
    return ret;
}
