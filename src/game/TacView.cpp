// game/TacView.cpp (0x805A6D20-0x805AE26C): tactical view of the battle (TacView). Only some members
// are reconstructed so far.
#include "game/TacView.h"
#include "game/CharaTask.h"
#include "game/SkillEffectManager.h"
#include "atn/SoundRef.h"
#include "atn/Memory.h"

// inferred: plays the sound `id` of the view (0x805A70D4, not decompiled: its name table is in .rodata)
atn::SoundRef fn_805A70D4(int id, float volume);

TacView* TacView::fn_805A70F0(atn::Task* parent) {
    if (lbl_8087F9F8 == 0) {
        lbl_8087F9F8 = new ((MemoryTag)3, "", "", 0) TacView(parent);
    }
    return lbl_8087F9F8;
}

int TacView::fn_805AA390(CharaTask* chara) {
    int id = chara->fn_8017AC08();
    switch (fn_80219558(chara->m50)) {
    case 1:
    case 5:
    case 9:
    case 10:
        if (id == 0x238F) {
            return 2;
        }
        if (id == 0x27E5 || id == 0x27E6) {
            return 3;
        }
        return 1;
    case 2:
    case 3:
        if (id == 0x2392) {
            return 2;
        }
        if (id == 0x27E5 || id == 0x27E6) {
            return 3;
        }
        return 1;
    case 4:
        if (id == 0x2390) {
            return 2;
        }
        if (id == 0x27E5 || id == 0x27E6) {
            return 3;
        }
        return 1;
    case 6:
        if (id == 0x2329) {
            return 2;
        }
        if (id == 0x27E5 || id == 0x27E6) {
            return 3;
        }
        return 1;
    case 11:
        return 1;
    case 14:
        if (id == 0x238E) {
            return 2;
        }
        if (id == 0x27E5 || id == 0x27E6) {
            return 3;
        }
        return 1;
    }
    return 1;
}

void TacView::fn_805AA4C8(SkillEffect* effect) {
    CharaTask* user = effect->m0C;
    user->mFlagB16 = 0;
    user->fn_80178864(effect->m04, user);
}

void TacView::fn_805AA82C(atn::Vector3* pos) {
    atn::Vector3 d = m159C - *pos;
    d.y = 0.0f;
    if (d.LengthSq_unk() > 4.0f) {
        d *= 0.3f;
        if (d.LengthSq_unk() < 4.0f) {
            d.Normalize_unk();
            d *= 2.0f;
        }
    } else {
        m15A8--;
    }
    *pos += d;
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

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void* memset(void* dst, int c, unsigned long n);

atn::Vector3 TacViewChara_unk::fn_805AE138() const {
    if (mType == 0) {
        return mChara->fn_801781B0();
    }
    if (mType == 1) {
        return mObj->m10;
    }
    return lbl_807C7030;
}

void TacViewChara_unk::fn_805AE188() {
    mType = 0;
    mChara = 0;
    mObj = (AiTargetObj_unk*)1; // inferred: the original stores 1 here
    m0C = 0;
    m10 = 0;
    m14 = 0;
    m18.fn_8003E918();
    m4C = -1;
    memset(m54, 0, sizeof(m54));
    for (int i = 0; i < 4; i++) {
        m64[i] = -1;
        m74[i] = -1;
        m50[i] = 1;
        m84[i] = -1;
    }
    m94 = 0;
}
