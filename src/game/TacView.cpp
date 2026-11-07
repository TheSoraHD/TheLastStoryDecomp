// game/TacView.cpp (0x805A6D20-0x805AE26C): tactical view of the battle (TacView). Only some members
// are reconstructed so far.
#include "game/TacView.h"
#include "game/CharaTask.h"
#include "game/SkillEffectManager.h"

void TacView::fn_805AA4C8(SkillEffect* effect) {
    CharaTask* user = effect->m0C;
    user->mFlagB16 = 0;
    user->fn_80178864(effect->m04, user);
}

void TacView::fn_805ADC00() {
    m1594 = 0;
    mEntryCount = 0;
}

void TacView::fn_805ADC10() {
    m1594 = 0;
    mEntryCount = 0;
}

int TacView::fn_805ADCC0(CharaTask* chara, HitInfo_unk* hit, int active) {
    TacViewEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mEntryCount; i++, entry++) {
        if (entry->mChara == chara && (hit == 0 || entry->mHit == hit)) {
            if (active != 0 && entry->mState <= 1) {
                return 1;
            }
            if (active == 0 && entry->mState == 0) {
                return 1;
            }
        }
    }
    return 0;
}

HitInfo_unk* TacView::fn_805ADD3C(CharaTask* chara) {
    TacViewEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mEntryCount; i++, entry++) {
        if (entry->mChara == chara && entry->mState <= 1) {
            return entry->mHit;
        }
    }
    return 0;
}

// NONMATCHING: 91% - the original builds the 2 with ori from the zeroed index register
void TacView::fn_805ADD80(CharaTask* chara, HitInfo_unk* hit) {
    for (unsigned int i = 0; i < mEntryCount; i++) {
        TacViewEntry_unk& entry = mEntries[i];
        if (entry.mChara == chara && entry.mHit == hit && (entry.mState & 2) != 2) {
            entry.mState = 2;
        }
    }
}
