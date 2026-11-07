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
    mEntries.clear();
}

void TacView::fn_805ADC10() {
    m1594 = 0;
    mEntries.clear();
}

void TacView::fn_805ADC20(CharaTask* chara, HitInfo_unk* hit) {
    if (chara == 0 || hit == 0) {
        return;
    }
    TacViewEntry_unk* entry = mEntries.begin();
    for (unsigned int i = 0; i < mEntries.size(); i++, entry++) {
        if (entry->mChara == chara && entry->mState != 0) {
            entry->mHit = hit;
            if ((entry->mState & 2) == 2) {
                entry->mState = (entry->mState | 1) & ~2;
            }
            return;
        }
    }
    mEntries.push_back(TacViewEntry_unk(chara, hit, 1));
}

int TacView::fn_805ADCC0(CharaTask* chara, HitInfo_unk* hit, int active) {
    TacViewEntry_unk* entry = mEntries.begin();
    for (unsigned int i = 0; i < mEntries.size(); i++, entry++) {
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
    TacViewEntry_unk* entry = mEntries.begin();
    for (unsigned int i = 0; i < mEntries.size(); i++, entry++) {
        if (entry->mChara == chara && entry->mState <= 1) {
            return entry->mHit;
        }
    }
    return 0;
}

// NONMATCHING: 91% - the original builds the 2 with ori from the zeroed index register
void TacView::fn_805ADD80(CharaTask* chara, HitInfo_unk* hit) {
    for (unsigned int i = 0; i < mEntries.size(); i++) {
        TacViewEntry_unk& entry = mEntries.mData[i];
        if (entry.mChara == chara && entry.mHit == hit && (entry.mState & 2) != 2) {
            entry.mState = 2;
        }
    }
}
