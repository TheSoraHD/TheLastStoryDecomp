// game/TacView.cpp (0x805A6D20-0x805AE26C): tactical view of the battle (TacView). Only some members
// are reconstructed so far.
#include "game/TacView.h"
#include "game/CharaTask.h"
#include "game/SkillEffectManager.h"
#include "atn/SoundRef.h"

// inferred: plays the sound `id` of the view (0x805A70D4, not decompiled: its name table is in .rodata)
atn::SoundRef fn_805A70D4(int id, float volume);

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

// NONMATCHING: 99% - registers of the entry pointer and of the inner pointer swapped
void TacView::fn_805ADDD4(CharaTask* chara) {
    TacViewEntry_unk* entry = mEntries.begin();
    for (unsigned int i = 0; i < mEntries.size(); entry++, i++) {
        if (chara == 0 || entry->mChara == chara) {
            if ((entry->mState & 1) == 1) {
                TacViewEntry_unk* other = mEntries.begin();
                for (unsigned int j = 0; j < mEntries.size(); j++, other++) {
                    if (other->mChara == entry->mChara && other->mState == 0) {
                        other->mHit = entry->mHit;
                        entry->mState |= 4;
                        break;
                    }
                }
            } else if ((entry->mState & 2) == 2) {
                entry->mState |= 4;
            }
        }
    }
    for (TacViewEntry_unk* p = mEntries.begin(); p != mEntries.end();) {
        if ((p->mState & 4) == 4) {
            mEntries.erase(p);
        } else {
            p++;
        }
    }
    for (TacViewEntry_unk* p = mEntries.begin(); p != mEntries.end(); p++) {
        if (p->mState != 0) {
            p->mState = 0;
        }
    }
}

void TacView::fn_805ADF50(CharaTask* chara) {
    TacViewEntry_unk* entry = mEntries.begin();
    for (unsigned int i = 0; i < mEntries.size(); entry++, i++) {
        if ((chara == 0 || entry->mChara == chara) && entry->mState != 0) {
            entry->mState |= 4;
        }
    }
    for (TacViewEntry_unk* p = mEntries.begin(); p != mEntries.end();) {
        if ((p->mState & 4) == 4) {
            mEntries.erase(p);
        } else {
            p++;
        }
    }
}

void TacView::fn_805AE034(TacViewTarget_unk* target) {
    if (target != 0) {
        TacViewSel_unk old = m15F0;
        if (old.mState == 2 || (old.mState == 1 && old.mTarget != target)) {
            if (target->m08->mType == 2) {
                fn_805A70D4(8, 1.0f);
            } else {
                fn_805A70D4(7, 1.0f);
            }
        }
        m15F0.mState = 1;
        m15F0.mTarget = target;
    } else {
        if (m15F0.mState != 2) {
            fn_805A70D4(9, 1.0f);
        }
        m15F0.mState = 2;
        m15F0.m04 = 0;
        m15F0.mTarget = 0;
    }
}
