// game/TacView.cpp (0x805A6D20-0x805AE26C): tactical view of the battle (TacView). Only some members
// are reconstructed so far.
#include "game/TacView.h"
#include "game/CharaTask.h"
#include "game/SkillEffectManager.h"
#include "atn/SoundRef.h"
#include "atn/Memory.h"
#include "db/SkillParam_unk.h"

// inferred: sector (0 ahead, 1-7 clockwise, 8 none) of the direction `dir` seen from above
int fn_805A6D20(const atn::Vector3& dir) {
    atn::Vector3 rot = dir.ToRotation_unk();
    float a = atn::WrapAngle_unk(rot.y);
    float h = 0.3926991f; // inferred: half of a sector (pi / 8)
    int sector = 8;
    if (8.0f * -h <= a && a < 7.0f * -h) {
        sector = 4;
    } else if (7.0f * -h <= a && a < 5.0f * -h) {
        sector = 3;
    } else if (5.0f * -h <= a && a < 3.0f * -h) {
        sector = 2;
    } else if (3.0f * -h <= a && a < -h) {
        sector = 1;
    } else if (-h <= a && a < h) {
        sector = 0;
    } else if (h <= a && a < 3.0f * h) {
        sector = 7;
    } else if (3.0f * h <= a && a < 5.0f * h) {
        sector = 6;
    } else if (5.0f * h <= a && a < 7.0f * h) {
        sector = 5;
    } else if (7.0f * h <= a && a <= 8.0f * h) {
        sector = 4;
    }
    return sector;
}

extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);

// inferred: sounds of the view (fn_805A70D4)
static const char* const sSoundNames[10] = {
    "SE_UICOM_004", "SE_UICOM_001", "SE_UICOM_002", "SE_UICOM_045", "SE_UICOM_027",
    "SE_UICOM_013", "SE_UICOM_030", "SE_UICOM_032", "SE_UICOM_033", "SE_UICOM_024",
};

// inferred: plays the sound `id` of the view
atn::SoundRef fn_805A70D4(int id, float volume) {
    return fn_800C31F4(sSoundNames[id], 0, -1, volume);
}

// inferred: skills shown by the view (filled by the constructor; 0x807C95B0)
static SkillParam_unk sSkillParams_unk[6];

// inferred: layout anime task (declared with other return types by game/CosEditProtector.cpp and others)
extern "C" atn::D2A_TaskInstanceBase<32, 8>* fn_801F64D0(atn::Task* parent, const char* path);

// inferred: message table (lbl_8087F1E4) as the view reads it: {id, text} entries (8 bytes each)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct TacViewMsgTable_unk {
    struct Entry {
        int mId;
        const wchar_t* mText;
    };
    const wchar_t* Get_unk(int id) { return mEntries[id].mText != 0 ? mEntries[id].mText : lbl_808813D0; }
    Entry mEntries[1];
};
extern "C" TacViewMsgTable_unk* lbl_8087F1E4;

TacView* TacView::fn_805A70F0(atn::Task* parent) {
    if (lbl_8087F9F8 == 0) {
        lbl_8087F9F8 = new ((MemoryTag)3, "", "", 0) TacView(parent);
    }
    return lbl_8087F9F8;
}

// inferred: skill ids, default name/text ids and name/text override ids (-1: none) of the six skills
static const int sIds[6] = {0x4FBE, 0x4FC0, -1, -1, -1, -1};
static const int sNames[6] = {0x1AA, 0x1AB, 0x1AC, 0x1AD, 0x1AE, 0x1AF};
static const int sTexts[6] = {0x1B0, 0x1B1, 0x1B2, 0x1B3, 0x1B4, 0x1B5};
static const int sNames2[6] = {0x135, 0x136, 0x137, 0x138, -1, -1};
static const int sTexts2[6] = {0x139, 0x13A, 0x13B, 0x13C, -1, -1};

// NONMATCHING: 99.5% - register coloring of the constants 0/2 and of the end of the mSlots loop
TacView::TacView(atn::Task* parent)
    : atn::Task(parent), m48(2), m4C(0), m50(0), mA70(0), mA74(0), mA78(6), mA7C(0), mA80(0), mA84(0),
      m158C(0), m1590(0), m1594(0), m1598(0), m15A8(0), m15AC(0), m15D4(0), m15D8(0), m15DC(0.0f, 0.0f, 0.0f),
      m15E8(-1.0f), m15EC(0), m15ED(0) {
    (mLayouts.mSelect = fn_801F3FF8(this, "/data/d2anime/ui_command_select.d2a", 0))->fn_800D246C(1);
    (mLayouts.mHpFlash = fn_801F3FF8(this, "/data/d2anime/ui_command_hp_flash.d2a", 0))->fn_800D246C(1);
    (mLayouts.mFace = fn_801F3FF8(this, "/data/d2anime/ui_command_face.d2a", 0))->fn_800D246C(1);
    (mLayouts.mDirs[0] = fn_801F64D0(this, "/data/d2anime/ui_select_4_u.d2a"))->fn_800D246C(1);
    (mLayouts.mDirs[3] = fn_801F64D0(this, "/data/d2anime/ui_select_4_d.d2a"))->fn_800D246C(1);
    (mLayouts.mDirs[1] = fn_801F64D0(this, "/data/d2anime/ui_select_4_l.d2a"))->fn_800D246C(1);
    (mLayouts.mDirs[2] = fn_801F64D0(this, "/data/d2anime/ui_select_4_r.d2a"))->fn_800D246C(1);
    for (int i = 0; i < 6; i++) {
        sSkillParams_unk[i].mId = sIds[i];
        sSkillParams_unk[i].m64 = 0;
        if (sNames2[i] < 0) {
            sSkillParams_unk[i].mName = lbl_8087F1E4->Get_unk(sNames[i]);
        } else {
            sSkillParams_unk[i].mName = lbl_8087F1E4->Get_unk(sNames2[i]);
        }
        if (sTexts2[i] < 0) {
            sSkillParams_unk[i].mText = lbl_8087F1E4->Get_unk(sTexts[i]);
        } else {
            sSkillParams_unk[i].mText = lbl_8087F1E4->Get_unk(sTexts2[i]);
        }
    }
}

TacView::~TacView() {
    lbl_8087F9F8 = 0;
}

int TacView::vf0C() {
    if (fn_800D3FA4()) {
        mLayouts.mSelect->fn_800D246C(0);
        mLayouts.mSelect->mRepeat = 0;
        mLayouts.mSelect->mSpeed = 0.0f;
        mLayouts.mSelect->FreezeTask();
        mLayouts.mHpFlash->fn_800D246C(0);
        mLayouts.mHpFlash->FreezeTask();
        mLayouts.mFace->fn_800D246C(0);
        mLayouts.mFace->mRepeat = 0;
        mLayouts.mFace->mSpeed = 1.0f;
        mLayouts.mFace->FreezeTask();
        for (int i = 0; i < 4; i++) {
            mLayouts.mDirs[i]->fn_800D246C(0);
            mLayouts.mDirs[i]->Rewind_unk();
            mLayouts.mDirs[i]->SetLoop_unk(0);
            mLayouts.mDirs[i]->SetSpeed_unk(0.0f);
            mLayouts.mDirs[i]->FreezeTask();
        }
        return 1;
    }
    return 0;
}

void TacView::vf14() {}

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


