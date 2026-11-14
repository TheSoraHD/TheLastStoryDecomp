// game/TacView.cpp (0x805A6D20-0x805AE26C): tactical view of the battle (TacView). Only some members
// are reconstructed so far.
#include "game/TacView.h"
#include "atn/D2A_Task.h"
#include "atn/D2A_TaskInstance.h"
#include "game/CharaTask.h"
#include "game/SkillEffectManager.h"
#include "atn/SoundRef.h"
#include "atn/Memory.h"
#include "db/SkillParam_unk.h"
#include "game/GameTask.h"
#include "game/PlayerManager.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/GameUI.h"
#include "game/MenuCommonTask.h"
#include "game/SetConfig.h"
#include "game/BattleCharaTask.h"
#include "game/GameConfig_unk.h"
#include "atn/Sphere.h"
#include "atn/RenderManager.h"

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

extern "C" int rand();
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
    : atn::Task(parent), mA70(0), mA74(0), mA78(6), mA7C(0), mA80(0), mA84(0),
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

extern "C" double sqrt(double x);
extern "C" GameTask* lbl_8087F430;
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" void* lbl_8087EF70; // inferred: pad input
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button pressed
extern "C" int fn_800A55FC(void* pad, int port, int button); // inferred: button released
extern "C" void fn_804A5E40(MenuCommonTask* menu, int type, const float* ofs, int id); // inferred: menu event
extern "C" int fn_80219558(int id); // inferred: party member of a character id
extern "C" int fn_8021A77C(HitInfo_unk* hit); // inferred: kind test of a skill
// inferred: not written yet; they take the view as first argument (symbols without a C++ name)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: closes the view (body of fn_805AA734, inlined by vf10; name invented)
static inline void TacViewClose_unk(TacView* v) {
    if (v->mA70 == 0) {
        return;
    }
    v->mA70 = 0;
    v->ChangeMode_unk(6);
    v->m48.mState = 2;
    v->m48.m04 = 0;
    v->m48.mTarget = 0;
    v->m15D8 = 0;
    v->m15DC.x = 0.0f;
    v->m15DC.y = 0.0f;
    v->m15DC.z = 0.0f;
    v->m15E8 = -1.0f;
    v->m15EC = 0;
    v->m15ED = 0;
    v->m15F0.mState = 2;
    v->m15F0.m04 = 0;
    v->m15F0.mTarget = 0;
    fn_800CB5C8(&v->m16C0, 15, 0);
    v->mLayouts.m1C = 0;
    v->mLayouts.m20 = 0;
    for (int i = 0; i < 4; i++) {
        v->mLayouts.mDirs[i]->Rewind_unk();
    }
}
// inferred: the pad button is pressed (no pad: 0; name invented)
static inline bool IsPadPressed_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A555C(lbl_8087EF70, 0, button) : 0;
}
// inferred: the cursor takes the slot m158C, none when out of range (name invented)
inline void TacViewSelectSlot_unk(TacView* v) {
    if (v->m158C >= 0 && v->m158C < (int)v->mSlots.size()) {
        v->m48 = v->mSlots[v->m158C].mSel;
    } else {
        v->m48.mState = 2;
        v->m48.m04 = 0;
        v->m48.mTarget = 0;
    }
}
// inferred: next slot, or the end of the orders (mode 5) after the last one (name invented)
inline void TacViewNextSlot_unk(TacView* v) {
    v->m158C++;
    v->ChangeMode_unk(0);
    if (v->m158C < (int)v->mSlots.size()) {
        TacViewSelectSlot_unk(v);
    } else {
        v->ChangeMode_unk(5);
    }
}
// inferred: undoes the last order given to `chara` (name invented)
inline void TacViewUndo_unk(TacView* v, CharaTask* chara) {
    if (v->mCharas.size() != 0) {
        if (v->mCharas[v->mCharas.size() - 1].m4C >= 0) {
            lbl_8087EE68->fn_80018608(chara, 0);
            static_cast<BattleCharaTask*>(chara)->m1470 = 0;
            v->fn_805ADF50(chara);
            if (v->mCharas[v->mCharas.size() - 1].m0C != 0 || v->mCharas[v->mCharas.size() - 1].m10 != 0) {
                chara->fn_80160170((HitInfo_unk*)v->mCharas[v->mCharas.size() - 1].m10,
                                   (CharaTask*)v->mCharas[v->mCharas.size() - 1].m0C);
            }
            chara->fn_8017ABD4(v->mCharas[v->mCharas.size() - 1].m14);
            if (v->mCharas[v->mCharas.size() - 1].m18.m08 != 4) {
                lbl_8087EE68->fn_800184F4(&v->mCharas[v->mCharas.size() - 1].m18);
            }
            chara->mStatus.fn_8012D180();
        }
        v->mCharas.mSize--;
    }
}
// inferred: offset of the menu windows while the view is open (name invented)
inline float TacViewMenuOffset_unk() {
    float ofs = 0.0f;
    if (lbl_8087F490 != 0) {
        ofs = 60.0f * lbl_8087F490->fn_803E4A48();
    }
    return ofs;
}

// inferred: update of the tactical view: hides the layouts; while open, counts down the closing (mA7C) and
// the move to a slot (mA84), then runs the mode mA78: 0 picks the command of each party member (pad
// directions, confirm and cancel), 1 moves the free camera, 4 aims at a position, 5 ends with the summary;
// button 0x2E switches the hate lines (m1590)
// NONMATCHING: 94.5% - registers and the copies of the camera parameters (the original copies GameCamera::m684 into
// stack slots in modes 1 and 4)
void TacView::vf10() {
    mLayouts.mSelect->FreezeTask();
    mLayouts.mHpFlash->FreezeTask();
    mLayouts.mFace->FreezeTask();
    for (int i = 0; i < 4; i++) {
        mLayouts.mDirs[i]->FreezeTask();
    }
    if (mA70 == 0 || (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 13)) {
        return;
    }
    if (mA7C > 0) {
        if (--mA7C <= 0) {
            TacViewClose_unk(this);
        }
        return;
    }
    GameCamera* camera = &lbl_8087F430->mCamera;
    if (mA84 > 0) {
        fn_805AC758();
        if (--mA84 > 0) {
            return;
        }
        if (m15D8 == 1) {
            m15A8 = 1;
            if ((CharaTask*)m15AC != m48.m04) {
                m15AC = (int)m48.m04;
                m159C = camera->m8A4;
            }
            ChangeMode_unk(4);
        } else if (m15D8 == 2) {
            CharaTask* target = m48.mState == 0 ? m48.m04->mC58.mC4 : 0;
            ChangeMode_unk(1);
            if (target != 0) {
                m48.mState = 2;
                m48.m04 = 0;
                m48.mTarget = 0;
                for (unsigned int i = 0; i < mSlots.size(); i++) {
                    if (mSlots[i].mSel.mState == 0 && mSlots[i].mSel.m04 == target) {
                        m48 = mSlots[i].mSel;
                        break;
                    }
                }
            } else if ((int)mSlots.size() > 0) {
                m48 = mSlots[0].mSel;
            } else {
                m48.mState = 2;
                m48.m04 = 0;
                m48.mTarget = 0;
            }
            fn_805A70D4(8, 1.0f);
        } else {
            TacViewNextSlot_unk(this);
            mLayouts.m1C = 0;
            mLayouts.m20 = 0;
        }
        return;
    }
    GameUIElem1C_unk button;
    if (lbl_8087F490 != 0) {
        lbl_8087F490->m764 = button;
    }
    if (mA78 == 0) {
        camera->fn_80390A88();
        GameCameraParam_unk param(camera->m684);
        param.m04 *= 0.7f;
        if (camera->m4B4.mActive != 1 && camera->m4B4.mActive != 4) {
            camera->m4B4.mActive = 1;
            camera->m4C0 = param;
            camera->m4B4.mStep = 0.0f;
            camera->m4B4.mTime = 1.0f;
        }
        camera->fn_8037F744();
        fn_805ABE74();
        fn_805AC758();
        if (m48.mState == 0) {
            int confirm = 0;
            int cancel = 0;
            int dir = -1;
            CharaTask* chara = m48.m04;
            if (mA74 == 0 && (IsPadPressed_unk(0) || IsPadPressed_unk(0x1A)) && m54.m50[0] != 0) {
                dir = 0;
            } else if (mA74 == 0 && (IsPadPressed_unk(2) || IsPadPressed_unk(0x17)) && m54.m50[1] != 0) {
                dir = 1;
            } else if (mA74 == 0 && (IsPadPressed_unk(3) || IsPadPressed_unk(0x18)) && m54.m50[2] != 0) {
                dir = 2;
            } else if (mA74 == 0 && (IsPadPressed_unk(1) || IsPadPressed_unk(0x19)) && m54.m50[3] != 0) {
                dir = 3;
            }
            if (dir >= 0) {
                fn_805A96D4(chara, dir);
                confirm = 1;
            }
            if (IsPadPressed_unk(4)) {
                confirm = 1;
            }
            if (IsPadPressed_unk(5)) {
                cancel = 1;
            }
            if (confirm != 0) {
                if (mCharas.size() < 16) {
                    mCharas.push_back(m54);
                }
                mLayouts.m1C = dir >= 0 ? mLayouts.mDirs[dir] : 0;
                mLayouts.m20 = 0;
                if (mLayouts.m1C != 0) {
                    mA84 = 15;
                    fn_805A70D4(1, 1.0f);
                } else {
                    mA84 = 1;
                    fn_805A70D4(4, 1.0f);
                }
            } else if (cancel != 0) {
                if (--m158C >= 0) {
                    TacViewSelectSlot_unk(this);
                    chara = m48.m04;
                    TacViewUndo_unk(this, chara);
                    fn_805ABE74();
                    mLayouts.m1C = 0;
                    mLayouts.m20 = 0;
                    fn_805AC758();
                } else {
                    TacViewClose_unk(this);
                }
                fn_805A70D4(2, 1.0f);
            }
            float ofs = TacViewMenuOffset_unk();
            atn::Vector3 a(0.0f, -ofs, 0.0f);
            fn_804A5E40(MenuCommonTask::sInstance, 10, &a.x, 0);
            atn::Vector3 b(0.0f, -ofs, 0.0f);
            fn_804A5E40(MenuCommonTask::sInstance, 11, &b.x, 0);
        }
    } else if (mA78 == 1) {
        camera->fn_80390A88();
        if (m1598 == 0) {
            atn::Vector3 d(m159C.x - camera->m8A4.x, m159C.y - camera->m8A4.y, m159C.z - camera->m8A4.z);
            if ((float)sqrt(d.x * d.x + d.z * d.z) > 2.0f) {
                bool moving = false;
                if (m15D8 == 1 && m15A8 > 0) {
                    moving = true;
                }
                if (!moving) {
                    m1598 = 1;
                }
            }
        }
        GameCameraParam_unk param(camera->m684);
        camera->m4B4.mActive = 0;
        camera->fn_8037F744();
        if (lbl_8087F0A8->mInput.fn_801231D0(0) != 0) {
            lbl_8087EE68->fn_800183E0(m54.mChara, m48.m04, (HitInfo_unk*)m54.m54[m54.m4C], 0);
            m15D8 = 0;
            TacViewNextSlot_unk(this);
            mLayouts.m1C = 0;
            mLayouts.m20 = 0;
            fn_805A70D4(1, 1.0f);
        } else if (lbl_8087F0A8->mInput.fn_801231D0(1) != 0) {
            m15D8 = 0;
            ChangeMode_unk(0);
            TacViewSelectSlot_unk(this);
            TacViewUndo_unk(this, m48.m04);
            mLayouts.m1C = 0;
            mLayouts.m20 = 0;
            fn_805A70D4(2, 1.0f);
        }
    } else if (mA78 == 4) {
        camera->fn_80390A88();
        m1598 = 1;
        GameCameraParam_unk param(camera->m684);
        camera->m4B4.mActive = 0;
        camera->fn_8037F744();
        if (m1598 != 0 && m16C0.mPtr == 0) {
            m16C0 = fn_805A70D4(6, 1.0f);
        }
        float ofs = TacViewMenuOffset_unk();
        if (m48.m04 == 0) {
            atn::Vector3 a(0.0f, -ofs, 0.0f);
            fn_804A5E40(MenuCommonTask::sInstance, 9, &a.x, 0);
        } else {
            HitInfo_unk* skill = m54.m4C >= 0 ? (HitInfo_unk*)m54.m54[m54.m4C] : 0;
            switch (fn_80219558(m48.m04->m50)) {
            case 0: {
                atn::Vector3 a(0.0f, -ofs, 0.0f);
                fn_804A5E40(MenuCommonTask::sInstance, 9, &a.x, 0);
                break;
            }
            case 2:
            case 3:
                if (skill == 0 || fn_8021A77C(skill) != 0) {
                    atn::Vector3 a(0.0f, -ofs, 0.0f);
                    fn_804A5E40(MenuCommonTask::sInstance, 12, &a.x, 0);
                } else {
                    atn::Vector3 a(0.0f, -ofs, 0.0f);
                    fn_804A5E40(MenuCommonTask::sInstance, 13, &a.x, 0);
                }
                break;
            case 4:
            case 6: {
                atn::Vector3 a(0.0f, -ofs, 0.0f);
                fn_804A5E40(MenuCommonTask::sInstance, 12, &a.x, 0);
                break;
            }
            }
        }
        int decide = 0;
        switch (SetConfig::sInstance->mWindKey) {
        case 0: {
            decide = 1;
            bool held = false;
            if (m15EC == 0 && lbl_8087F0A8->mInput.fn_801240B4(9) == 0) {
                held = true;
            }
            if (!held) {
                bool on = false;
                if (m15EC != 0 && lbl_8087F0A8->mInput.fn_801231D0(0) != 0) {
                    on = true;
                }
                if (!on) {
                    decide = 0;
                }
            }
            break;
        }
        case 1: {
            decide = 0;
            bool on = false;
            if (m1598 != 0 || m15EC != 0) {
                on = true;
            }
            if (on && lbl_8087F0A8->mInput.fn_801231D0(0) != 0) {
                decide = 1;
            }
            break;
        }
        case 2:
            decide = lbl_8087F0A8->mInput.fn_801231D0(0);
            break;
        }
        if (decide != 0) {
            atn::Vector3 pos = camera->m8A4;
            PlayerState_unk state(6);
            state.m0C = pos;
            if (m15EC == 0) {
                BattleCharaTask* player = (BattleCharaTask*)lbl_8087F8A0->mHead;
                state.m0C.y = player->mPos.y;
                player->SetState_unk(state);
                TacViewClose_unk(this);
                fn_805A70D4(5, 1.0f);
                m1594++;
            } else {
                HitInfo_unk* skill = m54.m4C >= 0 ? (HitInfo_unk*)m54.m54[m54.m4C] : 0;
                switch (fn_80219558(m48.m04->m50)) {
                case 0:
                    state.m0C.y = m48.m04->mPos.y;
                    break;
                case 2:
                case 3:
                    if (skill == 0 || fn_8021A77C(skill) != 0) {
                        state.m00 = 10;
                    } else {
                        state.m00 = 1;
                        state.m18 = skill->m04;
                        state.m02 = 8;
                    }
                    break;
                case 4:
                case 6:
                    state.m00 = 11;
                    break;
                }
                static_cast<BattleCharaTask*>(m48.m04)->SetState_unk(state);
                m15D8 = 0;
                fn_805A70D4(1, 1.0f);
                TacViewNextSlot_unk(this);
                mLayouts.m1C = 0;
                mLayouts.m20 = 0;
            }
            m159C = camera->m8A4;
        } else if (lbl_8087F0A8->mInput.fn_801231D0(1) != 0) {
            if (m15EC == 0) {
                TacViewClose_unk(this);
                lbl_8087F430->m5674 = 0;
            } else {
                m15D8 = 0;
                ChangeMode_unk(0);
                TacViewSelectSlot_unk(this);
                mLayouts.m1C = 0;
                mLayouts.m20 = 0;
                fn_805A70D4(2, 1.0f);
                m159C = camera->m8A4;
            }
        }
    } else if (mA78 == 5) {
        GameUIElem1C_unk end;
        end.m00 = 6;
        end.m04 = 12;
        if (lbl_8087F430 != 0 && lbl_8087F430->fn_80375184() != 0) {
            end.m04 = 13;
        }
        if (lbl_8087F490 != 0) {
            lbl_8087F490->m764 = end;
        }
        if (IsPadPressed_unk(4)) {
            mA7C = 20;
            fn_805A70D4(3, 1.0f);
            m1594++;
            fn_805ADDD4(0);
            int any = 0;
            TacViewChara_unk* c = mCharas.begin();
            for (unsigned int i = 0; i < mCharas.size(); i++, c++) {
                if (c->mType != 0 && c->m4C >= 0) {
                    any = 1;
                    if (c->m64[c->m4C] == 8 && static_cast<BattleCharaTask*>((CharaTask*)c->mType)->fn_800EE794(0) != 0 &&
                        ((CharaTask*)c->mType)->fn_80160324()) {
                        ((CharaTask*)c->mType)->fn_8016034C();
                    }
                    if (((CharaTask*)c->mType)->fn_80160324() && static_cast<BattleCharaTask*>((CharaTask*)c->mType)->m1470 != 0) {
                        ((CharaTask*)c->mType)->fn_8016034C();
                    }
                    if (c->m94 != 0 && lbl_8087F9E8 != 0) {
                        for (int j = 0; j < 4; j++) {
                            if (c->m64[j] == 4 && c->m54[j] != 0) {
                                lbl_8087F9E8->fn_8059C2A8((CharaTask*)c->mType, (HitInfo_unk*)c->m54[j]);
                            }
                        }
                        for (int j = 0; j < 6; j++) {
                            if (sIds[j] > 0) {
                                lbl_8087F9E8->fn_8059C2A8((CharaTask*)c->mType, fn_80219E6C(sIds[j]));
                            }
                        }
                    }
                }
            }
            if (any != 0) {
                lbl_8087F8A0->mHead->mStatus.m08 = 0.0f;
            }
        } else if (IsPadPressed_unk(5)) {
            ChangeMode_unk(0);
            if (m158C < 0) {
                m158C++;
            } else {
                m158C--;
            }
            TacViewSelectSlot_unk(this);
            TacViewUndo_unk(this, m48.m04);
            fn_805A70D4(2, 1.0f);
        }
    }
    if (lbl_8087F0A8->mInput.fn_801231D0(0x2E) != 0) {
        m1590 = (m1590 + 1) % 2;
        lbl_8087F490->fn_803D8344();
    }
    if (mA74 != 0 && fn_800A55FC(lbl_8087EF70, 0, 0) != 0 && fn_800A55FC(lbl_8087EF70, 0, 1) != 0 &&
        fn_800A55FC(lbl_8087EF70, 0, 2) != 0 && fn_800A55FC(lbl_8087EF70, 0, 3) != 0 &&
        fn_800A55FC(lbl_8087EF70, 0, 0x1A) != 0 && fn_800A55FC(lbl_8087EF70, 0, 0x19) != 0 &&
        fn_800A55FC(lbl_8087EF70, 0, 0x17) != 0 && fn_800A55FC(lbl_8087EF70, 0, 0x18) != 0) {
        mA74 = 0;
    }
}

void TacView::vf14() {}

extern "C" int fn_8021A888(HitInfo_unk* hit); // inferred: the skill is an attack magic
extern "C" atn::Vector3 lbl_807C7030;          // inferred: zero vector
// inferred: aiming state of the view (m15D8-m15F8; name invented)
inline void TacViewSetAim_unk(TacView* v, int mode, const atn::Vector3& pos, float radius, unsigned char a,
                              unsigned char b) {
    v->m15D8 = mode;
    v->m15DC = pos;
    v->m15E8 = radius;
    v->m15EC = a;
    v->m15ED = b;
    v->m15F0.mState = 2;
    v->m15F0.m04 = 0;
    v->m15F0.mTarget = 0;
}

// inferred: gives `chara` the command of the direction `dir` of the character entry m54: target, skill and AI
// order; skills that are aimed (areas, sub bullets) start the aiming of the view
// NONMATCHING: 80.6% - the original builds the aiming state (m15D8-m15F8) in a local struct and copies it, and
// orders the compares of the commands differently
void TacView::fn_805A96D4(CharaTask* chara, int dir) {
    m54.m4C = dir;
    m54.mType = (int)chara;
    HitInfo_unk* skill = (HitInfo_unk*)m54.m54[dir];
    if (skill != 0) {
        if (skill->m64 == 0) {
            m54.mChara = chara;
        } else if (fn_8021A77C(skill) != 0 || fn_8021A888(skill) != 0) {
            m54.mChara = chara->mC58.mC4;
        } else {
            m54.mChara = chara;
        }
    }
    if (chara->fn_80160324()) {
        m54.m0C = (int)chara->mF7C;
        m54.m10 = (int)chara->m638;
    }
    m54.m14 = chara->fn_8017AC08();
    if (lbl_8087EE68 != 0 && lbl_8087EE68->fn_800185B4(chara, 0) != 0) {
        m54.m18 = *lbl_8087EE68->fn_800185B4(chara, 0);
    } else {
        m54.m18.fn_8003E918();
    }
    m54.m94 = 1;
    CharaTask* user = (CharaTask*)m54.mType;
    CharaTask* target = m54.mChara;
    int kind = fn_80219558(user->m50);
    int extra = 0;
    int cmd = m54.m64[m54.m4C];
    if (cmd < 6) {
        switch (kind) {
        case 2:
        case 3:
            if (cmd == 4) {
                extra = skill->m5C + 30;
            }
            break;
        case 1:
        case 5:
        case 9:
        case 10:
            if (user->mType == 0 && cmd == 5) {
                PlayerState_unk state(11);
                state.m0C = user->mPos;
                static_cast<BattleCharaTask*>(chara)->SetState_unk(state);
            } else {
                if (user->fn_80160324() && skill != 0) {
                    user->fn_80160170(skill, target);
                }
                fn_805ADC20(user, skill);
                if (lbl_8087F490 != 0) {
                    lbl_8087F490->fn_803E5C60(user);
                }
            }
            break;
        case 4:
            if (user->mType == 0 && cmd == 5) {
                break;
            }
            if (cmd == 4) {
                target = user;
            }
            if (user->fn_80160324() && skill != 0) {
                user->fn_80160170(skill, target);
            }
            fn_805ADC20(user, skill);
            if (lbl_8087F490 != 0) {
                lbl_8087F490->fn_803E5C60(user);
            }
            break;
        case 6:
            if (cmd == 4) {
                PlayerState_unk state(7);
                state.m18 = skill->m04;
                static_cast<BattleCharaTask*>(chara)->SetState_unk(state);
            } else if (user->mType != 0 || cmd != 5) {
                fn_805ADC20(user, skill);
                if (lbl_8087F490 != 0) {
                    lbl_8087F490->fn_803E5C60(user);
                }
            }
            break;
        }
        lbl_8087EE68->fn_800183E0(user, target, skill, extra);
    } else {
        if (cmd == 6 || cmd == 7) {
            lbl_8087EE68->fn_80018608(user, 0);
            PlayerState_unk state(7);
            state.m18 = skill->m04;
            static_cast<BattleCharaTask*>(chara)->SetState_unk(state);
        } else if (cmd == 8) {
            AiTarget* t = lbl_8087EE68 != 0 ? lbl_8087EE68->fn_800185B4(user, 0) : 0;
            if (t != 0 && t->m28 != 0 && t->m28->m04 == 0x4FBD) {
                m54.m94 = 0;
            }
        } else if (cmd == 9) {
            lbl_8087EE68->fn_800183E0(user, target, skill, 0);
        } else if (cmd == 10 || cmd == 11) {
            lbl_8087EE68->fn_800183E0(user, target, skill, 0);
        }
    }
    if (skill == 0) {
        return;
    }
    if (m54.m94 != 0) {
        fn_805AA014(user, cmd);
    }
    user->mStatus.fn_8012D180();
    if (skill->m70 > 0) {
        if (fn_80219E6C(skill->m70)->m02 == 7) {
            TacViewSetAim_unk(this, 1, chara->mPos, 100.0f, 1, 0);
        }
        return;
    }
    if (user->mType == 0) {
        switch (skill->m04) {
        case 0x179A:
        case 0x179B:
        case 0x179C:
            TacViewSetAim_unk(this, 1, chara->mPos, skill->m58, 1, 1);
            break;
        case 0x1788:
        case 0x1791:
            TacViewSetAim_unk(this, 1, chara->mPos, skill->m58, 1, 0);
            break;
        case 0x4FBB:
        case 0x1789:
            TacViewSetAim_unk(this, 1, chara->mPos, skill->m58, 1, 0);
            break;
        }
    } else if (skill->m04 == 0x4FBB || skill->m04 == 0x1789) {
        TacViewSetAim_unk(this, 1, chara->mPos, skill->m58, 1, 0);
    } else if ((fn_8021A888(skill) != 0 || skill->m04 == 0x1788 || skill->m04 == 0x1791) && chara->mType != 2) {
        int count = 0;
        for (EnemyTask* e = EnemyManager::sInstance != 0 ? EnemyManager::sInstance->mHead : 0; e != 0;
             e = e->mNext) {
            if (e->CanAct_unk() && (e->m54C & 0x2000) != 0x2000 && e->mC58.mC0 != 0 &&
                static_cast<BattleCharaTask*>((CharaTask*)e)->m146C != 0x29) {
                count++;
            }
        }
        if (count > 0) {
            TacViewSetAim_unk(this, 2, lbl_807C7030, 0.0f, 1, 0);
        }
    }
}

void TacView::fn_805AA014(CharaTask* chara, int type) {
    switch (fn_80219558(chara->m50)) {
    case 1:
    case 5:
    case 9:
    case 10:
        if (type >= 3 && type <= 5) {
            chara->fn_8017ABD4(0x2390);
        } else if (type == 9) {
            chara->fn_8017ABD4(rand() % 2 == 0 ? 0x27E5 : 0x27E6);
        } else if (chara->mFlagB9) {
            chara->fn_8017ABD4(0x27E9);
        } else {
            chara->fn_8017ABD4(0x238F);
        }
        break;
    case 2:
    case 3:
        if (type == 4) {
            chara->fn_8017ABD4(0x2392);
        } else if (type == 9) {
            chara->fn_8017ABD4(rand() % 2 == 0 ? 0x27E5 : 0x27E6);
        } else {
            chara->fn_8017ABD4(0x238D);
        }
        break;
    case 4:
        if (type >= 3 && type <= 4) {
            chara->fn_8017ABD4(0x2390);
        } else if (type == 9) {
            chara->fn_8017ABD4(rand() % 2 == 0 ? 0x27E5 : 0x27E6);
        } else if (chara->mFlagB9) {
            chara->fn_8017ABD4(0x27E9);
        } else {
            chara->fn_8017ABD4(0x238F);
        }
        break;
    case 6:
        if (type == 4) {
            chara->fn_8017ABD4(0x2329);
        } else if (type == 9) {
            chara->fn_8017ABD4(rand() % 2 == 0 ? 0x27E5 : 0x27E6);
        } else if (chara->mFlagB9) {
            chara->fn_8017ABD4(0x27E9);
        } else {
            chara->fn_8017ABD4(0x238F);
        }
        break;
    case 11:
        if (type == 9) {
            chara->fn_8017ABD4(rand() % 2 == 0 ? 0x27E5 : 0x27E6);
        } else {
            chara->fn_8017ABD4(0x238D);
        }
        break;
    case 14:
        if (type == 9) {
            chara->fn_8017ABD4(rand() % 2 == 0 ? 0x27E5 : 0x27E6);
        } else if (type == 11) {
            chara->fn_8017ABD4(0x238E);
            chara->fn_8014DEE4(0, 0, 0);
        } else if (type == 10) {
            chara->fn_8017ABD4(0x238D);
            chara->fn_8014DEE4(1, 0, 0);
        }
        break;
    default:
        if (chara->m5C->m122 == 4) {
            if (type == 11) {
                chara->fn_8017ABD4(0x238E);
                chara->fn_8014DEE4(0, 0, 0);
            } else if (type == 10) {
                chara->fn_8017ABD4(0x238D);
                chara->fn_8014DEE4(1, 0, 0);
            }
        }
        break;
    }
    chara->mStatus.fn_8012D180();
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

extern "C" GameTask* lbl_8087F430;
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" void* lbl_8087EF70; // inferred: pad input
extern "C" int fn_800A55D4(void* pad, int port, int button); // inferred: button held

void TacView::fn_805AA4E4() {
    if (mA70 != 0) {
        return;
    }
    mA70 = 1;
    mA74 = 0;
    mA7C = 0;
    mA84 = 0;
    m158C = 0;
    m1598 = 0;
    m15A8 = 1;
    m159C = lbl_8087F430->mCamera.m8A4;
    m15AC = 0;
    mCharas.clear();
    if (m15D8 == 1) {
        ChangeMode_unk(4);
    } else {
        ChangeMode_unk(0);
    }
    for (PlayerTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = chara->mNext) {
        chara->mStatus.fn_8012D180();
    }
    if (m158C >= 0 && m158C < (int)mSlots.size()) {
        m48 = mSlots[m158C].mSel;
    } else {
        m48.mState = 2;
        m48.m04 = 0;
        m48.mTarget = 0;
    }
    fn_805ADDD4(0);
    if (lbl_8087F430 != 0) {
        lbl_8087F430->fn_803750E4(0xBB);
        lbl_8087F430->fn_803750E4(0xBC);
    }
    if (fn_800A55D4(lbl_8087EF70, 0, 0) || fn_800A55D4(lbl_8087EF70, 0, 1) || fn_800A55D4(lbl_8087EF70, 0, 2) ||
        fn_800A55D4(lbl_8087EF70, 0, 3) || fn_800A55D4(lbl_8087EF70, 0, 0x1A) ||
        fn_800A55D4(lbl_8087EF70, 0, 0x19) || fn_800A55D4(lbl_8087EF70, 0, 0x17) ||
        fn_800A55D4(lbl_8087EF70, 0, 0x18)) {
        mA74 = 1;
    }
}

void TacView::fn_805AA734() {
    TacViewClose_unk(this);
}

int TacView::fn_805AA824() {
    return 0;
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

// inferred: hit data of an effect as the overview window names it (the shared kinds of the skills; names
// invented)
static inline HitInfo_unk* TacViewInfoHit_unk(HitInfo_unk* hit, const TacViewSel_unk& sel) {
    if (hit != 0) {
        switch (hit->m04) {
        case 7:
        case 10:
        case 13:
        case 16:
        case 17:
        case 18:
        case 20:
        case 0x6D8:
        case 0x4EF8:
            hit = fn_80219E6C(30001);
            break;
        }
        if (hit->m04 == 0x4EF9) {
            hit = fn_80219E6C(30002);
        }
        if ((hit->m04 == 0x4EEC || hit->m04 == 0x4EED) && sel.mTarget->m08 != 0 && sel.mTarget->m08->mType == 2) {
            hit = fn_80219E6C(3007);
        }
        if ((signed char)hit->m02 == 4 || (signed char)hit->m02 == 0) {
            hit = 0;
        }
    }
    return hit;
}

// inferred: moves the cursor of the view with the stick `move` (mode 1: to the nearest slot in that direction, or
// along the links of the current one; `reset` forces it), or, in modes 0 and 4 with the hints on (`free`), shows
// the info window of the effect or enemy nearest to `pos`
// NONMATCHING: 97.4% - register allocation (best distance f0/f9, r31/r25) and the scheduling of the first
// distance of the effect loop
void TacView::fn_805AA94C(atn::Vector3* pos, atn::Vector3* move, int free, int reset) {
    if (mA78 == 0 || mA78 == 4) {
        if (SetConfig::sInstance->mTacviewHint != 0 && free != 0 && lbl_8087F490->mD90.m00 == 0) {
            TacViewCursor_unk sel;
            float best = 1e16f;
            CharaTask* head = lbl_8087F8A0->mHead;
            for (unsigned int i = 0; lbl_8087F9E8->GetBullet_unk(i) != 0; i++) {
                SkillEffect* e = lbl_8087F9E8->GetBullet_unk(i);
                if (e->m08 != 0 && e->m08->mC58.mB4 > 0 && head->mC58.mB4 > 0 &&
                    e->m08->mC58.mB4 != head->mC58.mB4) {
                    continue;
                }
                atn::Vector3 center = e->m10;
                float r = e->m5C * e->m28;
                float dist = (*pos - center).LengthSqXZ_unk();
                if (r >= 15.0f && dist < r * r && dist < best) {
                    best = dist;
                    sel.m04 = 0;
                    sel.mState = 1;
                    sel.mTarget = (TacViewTarget_unk*)e;
                }
            }
            for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
                if ((c->mFlags & 4) != 4 && (c->mStatus.m0C & 0x20) != 0x20 && (c->m54C & 0x2000) != 0x2000 &&
                    static_cast<BattleCharaTask*>((CharaTask*)c)->m146C != 0x29) {
                    atn::Vector3 cpos = c->mPos;
                    float r = 2.2f * c->m5B0;
                    float dist = (*pos - cpos).LengthSqXZ_unk();
                    if (dist < r * r && dist < best) {
                        best = dist;
                        sel.mTarget = 0;
                        sel.mState = 0;
                        sel.m04 = c;
                    }
                }
            }
            if (sel.mState != 2 && sel.mState != 0) {
                SkillEffect* e = (SkillEffect*)sel.mTarget;
                HitInfo_unk* hit = TacViewInfoHit_unk(e->GetHit_unk(), sel);
                if (hit != 0) {
                    HitInfo_unk* link = e->GetHit_unk();
                    if (link != 0 && (link->m04 == 0x4EF8 || link->m04 == 0x4EF9)) {
                        link = fn_80219E6C(30000);
                    }
                    if ((link->m04 == 0x4EEC || link->m04 == 0x4EED) && e->m08 != 0 && e->m08->mType == 2) {
                        link = fn_80219E6C(3007);
                    }
                    lbl_8087F490->fn_803E4A20((int)hit, (int)link);
                    m15D4 = 1;
                }
            }
        }
    } else if (mA78 == 1) {
        if (mA80 > 0) {
            mA80--;
        }
        TacViewSel_unk cur = m48;
        if ((PSVECSquareMag(move) > 0.0f && mA80 <= 0) || reset != 0) {
            atn::Vector3 dir = *move;
            if (dir.LengthSq_unk() < 0.09f) {
                dir *= 0.0f;
            } else {
                dir.Normalize_unk();
            }
            Mtx m;
            PSMTXRotRad(m, 'y', -lbl_8087F430->mCamera.m8A0);
            PSMTXMultVec(m, &dir, &dir);
            int sector = fn_805A6D20(atn::Vector3(dir));
            if (sector == 8) {
                return;
            }
            if (cur.mState == 2) {
                TacViewSlot_unk* nearest[8];
                memset(nearest, 0, sizeof(nearest));
                float bestDist[8];
                for (int k = 0; k < 8; k++) {
                    bestDist[k] = 1e16f;
                }
                for (unsigned int i = 0; i < mSlots.mSize; i++) {
                    TacViewSlot_unk* slot = &mSlots.mData[i];
                    if (slot->mSel.mState == 2) {
                        continue;
                    }
                    if (mA78 == 1 && slot->mSel.mState == 0 &&
                        ((CharaTask*)m54.mType)->fn_8016F3D0(slot->mSel.m04, 1)) {
                        continue;
                    }
                    // inferred: the slot starts as TacViewChara_unk (type, character, object), as in fn_805AB828
                    atn::Vector3 d = ((const TacViewChara_unk*)slot)->fn_805AE138() - *pos;
                    float len = d.x * d.x + d.z * d.z;
                    if (len > 0.0f) {
                        d.Normalize_unk();
                    }
                    int k = fn_805A6D20(atn::Vector3(d));
                    if (k != 8 && len < bestDist[k]) {
                        nearest[k] = slot;
                        bestDist[k] = len;
                    }
                }
                TacViewSlot_unk* slot = nearest[sector];
                if (slot != 0) {
                    m48 = slot->mSel;
                    mA80 = 10;
                    fn_805A70D4(8, 1.0f);
                }
            } else {
                TacViewSlot_unk* slot = mSlots.mData;
                for (unsigned int i = 0; i < mSlots.mSize; i++, slot++) {
                    bool same;
                    if (slot->mSel.mState == 0 && cur.mState == 0) {
                        same = slot->mSel.m04 == cur.m04;
                    } else if (slot->mSel.mState == 1 && cur.mState == 1) {
                        same = true;
                    } else {
                        same = false;
                    }
                    if (same) {
                        int order[3];
                        order[0] = sector;
                        order[1] = (sector + 1) % 8;
                        order[2] = sector > 0 ? sector - 1 : 7;
                        for (int k = 0; k < 3; k++) {
                            TacViewSlot_unk* next = slot->mNear[order[k]];
                            if (next != 0 && !(mA78 == 1 && next->mSel.mState == 0 &&
                                               ((CharaTask*)m54.mType)->fn_8016F3D0(next->mSel.m04, 1))) {
                                m48 = next->mSel;
                                mA80 = 10;
                                fn_805A70D4(8, 1.0f);
                                return;
                            }
                        }
                        return;
                    }
                }
            }
        }
    }
}

// ---- fn_805AB828: list of the targets of the view (over the inlining budget: the accessors are weak copies) ----
// inferred: player manager and character lists (weak copies of other files, as in gmk/Cannonball.cpp)
extern "C" inline PlayerManager* fn_8000D9E8() {
    return lbl_8087F8A0;
}
inline PlayerTask* PlayerManager::GetHead_unk() {
    return mHead;
}
extern "C" inline CharaTask* fn_802A36B0(PlayerManager* mgr) {
    return mgr->mHead;
}
extern "C" inline CharaTask* fn_802A4094(CharaTask* chara) {
    return ((PlayerTask*)chara)->mNext;
}
extern "C" inline EnemyManager* fn_801A03E0() {
    return EnemyManager::sInstance;
}
extern "C" inline EnemyTask* fn_801A0408(EnemyManager* mgr) {
    return mgr->mHead;
}
extern "C" inline EnemyTask* fn_801A03EC(EnemyTask* chara) {
    return chara->mNext;
}
// inferred: accessor of the control data (weak copy 0x800F530C, as in game/BattleTask.cpp)
extern "C" inline CharaC58_unk* fn_800F530C(CharaTask* chara) {
    return &chara->mC58;
}
// inferred: weak copy 0x805ABD6C (name invented)
extern "C" inline int fn_805ABD6C(CharaTask* chara) {
    return chara->mF94;
}
// inferred: player character as seen from here (as game/BattleTask.cpp)
struct Player_unk : public BattleCharaTask {
    // inferred: weak copy 0x800F7FB4 (name invented)
    int GetM146C_unk() { return m146C; }
};
// inferred: shared out of line copies of atn::FixedArray_unk (size and clear, as game/GameUI.cpp)
extern "C" unsigned int fn_80372574(const void* array);

// inferred: adds a party member that is set up (weak copy 0x805AB1D8, name invented)
extern "C" inline void fn_805AB1D8(atn::SimpleVector<CharaTask*>* list, CharaTask* chara) {
    if (chara != 0 && chara->mType != 0) {
        list->push_back(chara);
    }
}

// NONMATCHING: 99.9% - registers of `slot` and `best` swapped (r29/r30); rebuilds mSlots with the targets of the mode and links the nearest ones
void TacView::fn_805AB828(int mode) {
    // inferred: character numbers of the guests shown with the party (-1 ends)
    static const int sGuests_unk[10] = {0x31601, 0x31602, 0x31603, 0x31604, 0x31605,
                                        0x31606, 0x31607, 0x31608, 0x31609, -1};
    ((GameUICounter_unk*)&mSlots)->fn_803CF734();
    if (mode == 3) {
        mode = 0;
    }
    CharaTask* head = fn_8000D9E8() ? fn_8000D9E8()->GetHead_unk() : 0;
    if (mode == 0) {
        atn::SimpleVector<CharaTask*> list;
        if (fn_8000D9E8()) {
            list.push_back(head);
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(0));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(2));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(3));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(6));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(1));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(9));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(10));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(4));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(5));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(11));
            fn_805AB1D8(&list, fn_8000D9E8()->fn_8054A33C(14));
        }
        for (CharaTask* chara = fn_802A36B0(fn_8000D9E8()); chara != 0; chara = fn_802A4094(chara)) {
            for (const int* no = sGuests_unk; *no > 0; no++) {
                if (*no == chara->GetCharaNo_unk()) {
                    list.push_back(chara);
                    break;
                }
            }
        }
        for (unsigned int i = 0; i < list.size(); i++) {
            CharaTask* chara = list[i];
            if (chara == 0 || chara->IsFrozen_unk() || chara->GetFlag54C13_unk() || !chara->fn_80139EFC()) {
                continue;
            }
            if (chara->GetStatus_unk()->fn_80139F4C() && chara->GetStatus_unk()->m224 <= 0) {
                continue;
            }
            if (chara->GetStatus_unk()->IsState_unk(0x100) || chara->GetStatus_unk()->IsState_unk(0x10000)) {
                continue;
            }
            if (chara->GetStatus_unk()->IsState_unk(0x40) && !fn_805ABD6C(chara)) {
                continue;
            }
            if (head != 0 && fn_800F530C(head)->mB4 > 0 && fn_800F530C(chara)->mB4 > 0 &&
                fn_800F530C(head)->mB4 != fn_800F530C(chara)->mB4) {
                continue;
            }
            TacViewSlot_unk slot;
            slot.fn_805AB1B0(chara);
            mSlots.push_back(slot);
        }
    } else if (mode == 1) {
        for (EnemyTask* chara = fn_801A03E0() ? fn_801A0408(fn_801A03E0()) : 0; chara != 0;
             chara = fn_801A03EC(chara)) {
            if (chara->CanAct_unk() && !chara->GetFlag54C13_unk() && fn_800F530C(chara)->mC0 != 0 &&
                ((Player_unk*)chara)->GetM146C_unk() != 0x29) {
                TacViewSlot_unk slot;
                slot.fn_805AB1B0(chara);
                mSlots.push_back(slot);
            }
        }
    }
    for (unsigned int i = 0; i < fn_80372574(&mSlots); i++) {
        TacViewSlot_unk* slot = &mSlots[i];
        float best[8];
        for (int k = 0; k < 8; k++) {
            best[k] = 1e16f;
        }
        for (unsigned int j = 0; j < fn_80372574(&mSlots); j++) {
            TacViewSlot_unk* other = &mSlots[j];
            if (j != i && other->fn_805AB1C0()) {
                // inferred: the slot starts as TacViewChara_unk (type, character, object)
                atn::Vector3 dir = ((const TacViewChara_unk*)other)->fn_805AE138() -
                                   ((const TacViewChara_unk*)slot)->fn_805AE138();
                float len = dir.LengthSqXZ_unk();
                if (len > 0.0f) {
                    dir.Normalize_unk();
                }
                int d = fn_805A6D20(atn::Vector3(dir));
                if (d != 8 && len < best[d]) {
                    best[d] = len;
                    slot->mNear[d] = other;
                }
            }
        }
    }
}

extern "C" HitInfo_unk* fn_80219344(CharaTask* chara); // inferred: default skill of the character

// inferred: the gauge of `chara` is full (players only check the ratio; nonzero 0x4 state never) (name invented)
inline bool IsGaugeFull_unk(CharaTask* chara) {
    bool full = false;
    if ((chara->mStatus.m0C & 4) != 4) {
        bool ok = true;
        if (chara->mType == 0) {
            float rate = chara->mStatus.mParamB.m88 > 0 ? chara->mStatus.m08 / (float)chara->mStatus.mParamB.m88 : 0.0f;
            if (!((float)__fabs(rate - 1.0f) < 1.1920929e-7f)) {
                ok = false;
            }
        }
        if (ok) {
            full = true;
        }
    }
    return full;
}

// inferred: commands of the character under the cursor (m54): kinds m64, sub kinds m74, icons m84 and enabled
// flags m50 by the party member kind (or by the kind of the character data), the "special" command m64[3]
// (9 while an enemy is in state 0x22) and the skill of each command
// NONMATCHING: 97.1% - register allocation (r28/r29) and the order of some stores of the cases; the slot loop
void TacView::fn_805ABE74() {
    if (m48.mState != 0) {
        return;
    }
    CharaTask* chara = m48.m04;
    if (chara->mType == 2) {
        return;
    }
    m54.fn_805AE188();
    int kind = fn_80219558(chara->m50);
    if (0 <= kind && kind < 7) {
        m54.m64[0] = 5;
        m54.m74[0] = chara->mStatus.m2CC - 1;
    }
    if (chara->mStatus.m228 < 1.0f) {
        m54.m50[0] = 0;
    }
    m54.mObj = (AiTargetObj_unk*)fn_805AA390(chara); // inferred: the kind of the charged skill (see fn_805AE188)
    switch (kind) {
    case 0:
        m54.m64[1] = 6;
        m54.m64[2] = 1;
        m54.m50[2] = IsGaugeFull_unk(chara);
        m54.m84[0] = 8;
        m54.m84[1] = 0;
        m54.m84[2] = 8;
        break;
    case 1:
    case 9:
    case 10:
        m54.m64[1] = 1;
        m54.m64[2] = 3;
        if (m54.m50[0]) {
            m54.m50[0] = IsGaugeFull_unk(chara);
        }
        m54.m50[1] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
        m54.m50[2] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
        m54.m84[0] = 2;
        m54.m84[1] = 9;
        m54.m84[2] = 3;
        break;
    case 2:
    case 3:
        if (kind == 2) {
            m54.m64[1] = 6;
        } else {
            m54.m64[1] = 7;
        }
        m54.m64[2] = 4;
        m54.m84[0] = 0;
        m54.m84[1] = 0;
        m54.m84[2] = 2;
        break;
    case 4:
        m54.m64[1] = 1;
        m54.m64[2] = 4;
        if (m54.m50[0]) {
            m54.m50[0] = IsGaugeFull_unk(chara);
        }
        m54.m50[1] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
        m54.m50[2] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
        m54.m84[0] = 5;
        m54.m84[1] = 5;
        m54.m84[2] = 4;
        break;
    case 5:
        m54.m64[1] = 3;
        m54.m64[2] = 1;
        if (m54.m50[0]) {
            m54.m50[0] = IsGaugeFull_unk(chara);
        }
        m54.m50[1] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
        m54.m50[2] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
        m54.m84[0] = 3;
        m54.m84[1] = 3;
        m54.m84[2] = 7;
        break;
    case 6:
        m54.m64[1] = 1;
        m54.m64[2] = 4;
        if (m54.m50[0]) {
            m54.m50[0] = IsGaugeFull_unk(chara);
        }
        m54.m50[1] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
        m54.m84[0] = 6;
        m54.m84[1] = 6;
        m54.m84[2] = 0;
        break;
    case 11:
        m54.m64[1] = 7;
        m54.m84[1] = 0;
        m54.m50[2] = 0;
        break;
    default:
        switch (chara->m5C->m122) {
        case 1:
            m54.m64[1] = 6;
            m54.m84[1] = 0;
            m54.m50[2] = 0;
            break;
        case 2: {
            m54.m64[1] = 1;
            m54.m74[1] = 0;
            m54.m50[1] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
            HitInfo_unk* skill = fn_80219E6C(chara->mStatus.m2D0[0].m[0]);
            if (skill != 0 && skill->m74.m04 > 0.0f) {
                m54.m84[1] = 6;
            } else {
                m54.m84[1] = 5;
            }
            m54.m50[2] = 0;
            break;
        }
        case 3:
            m54.m64[1] = 3;
            m54.m74[1] = 0;
            m54.m50[1] = ((chara->mStatus.m0C >> 2) & 1) ^ 1;
            m54.m84[1] = 3;
            m54.m50[2] = 0;
            break;
        case 4:
            m54.m64[1] = 0xB;
            m54.m64[2] = 0xA;
            m54.m84[1] = 1;
            m54.m84[2] = 0;
            break;
        }
        break;
    }
    bool special = false;
    for (EnemyTask* enemy = EnemyManager::sInstance != 0 ? EnemyManager::sInstance->mHead : 0; enemy != 0;
         enemy = enemy->mNext) {
        if (enemy->CanAct_unk() && enemy->mC58.mC0 > 0 && enemy->m146C == 0x22) {
            special = true;
            break;
        }
    }
    if (special) {
        m54.m64[3] = 9;
        if (chara->mType == 0) {
            m54.m50[3] = 0;
        }
    } else {
        m54.m64[3] = 8;
        bool on = false;
        if (lbl_8087F9E8 != 0 && lbl_8087F9E8->fn_8059D30C(chara) > 0) {
            on = true;
        }
        for (PlayerTask* p = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0; p != 0; p = p->mNext) {
            if ((p->mFlags & 4) != 4 && (p->m54C & 0x2000) != 0x2000 && p->m5C != 0 &&
                (unsigned char)(p->m5C->m122 - 2) <= 1) {
                on = true;
                break;
            }
        }
        m54.m50[3] = on;
    }
    for (int i = 0; i < 4; i++) {
        int type = m54.m64[i];
        if (type < 0) {
            continue;
        }
        if (type < 6) {
            if (0 <= kind && kind < 7) {
                int slot = m54.m74[i];
                if (slot >= 0) {
                    m54.m54[i] = chara->mStatus.m2D0[slot].m[0] > 0 ? fn_80219E6C(chara->mStatus.m2D0[slot].m[0]) : 0;
                } else {
                    m54.m54[i] = fn_80219344(chara);
                }
            } else {
                int slot = m54.m74[i];
                if (slot >= 0) {
                    m54.m54[i] = fn_80219E6C(chara->mStatus.m2D0[slot].m[0]);
                } else {
                    m54.m54[i] = fn_80219E6C(chara->mStatus.m2D0[i].m[0]);
                }
            }
        } else {
            m54.m54[i] = &sSkillParams_unk[type - 6];
        }
        if (m54.m54[i] == 0) {
            m54.m50[i] = 0;
        }
    }
}

// ---- fn_805AC758: windows of the character under the cursor ----
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FECE0(atn::D2A_Layout_unk* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(atn::D2A_Layout_unk* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEE08(atn::D2A_Layout_unk* layout, unsigned int hash, const wchar_t* text);
// inferred: layout parameters by name (as the free helpers of game/GameUI.cpp; names invented)
inline void SetLayoutValue_unk(atn::D2A_Layout_unk* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetLayoutValue_unk(atn::D2A_Layout_unk* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetLayoutText_unk(atn::D2A_Layout_unk* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
// inferred: rectangle of a pane of a d2anime task (as MP code reads it)
extern "C" atn::D2A_DrawRect fn_801F4E8C(atn::D2A_Task* task, unsigned int hash);
inline atn::D2A_DrawRect GetPaneRect_unk(atn::D2A_Task* task, const char* name) {
    return fn_801F4E8C(task, fn_800DC6B4(name));
}
extern "C" void fn_801F6C80(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, float value); // inferred: value
extern "C" void fn_801F837C(atn::Task* task, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_804A4AEC(atn::D2A_Task* task, CharaInfo_unk* info, int cols, int rows); // inferred: name pane size
extern "C" void fn_803DDD54(atn::D2A_TaskInstanceBase<32, 8>* inst, int job); // inferred: job icon (game/GameUI.cpp)
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, float size, const wchar_t* text, float space, int a, int b); // inferred: width
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" unsigned long wcslen(const wchar_t* s);

namespace std {
// inferred: operator+ of the MSL string (weak copy 0x805ADB28, only used here); not in std/string.h, where a new
// template renumbers the @N labels of every unit that includes it
// NONMATCHING: 62% - the original reads data and size of each string from one test (private members: needs a
// friend declaration in std/string.h)
template <class T, class Traits, class Alloc>
basic_string<T, Traits, Alloc> operator+(const basic_string<T, Traits, Alloc>& lhs,
                                         const basic_string<T, Traits, Alloc>& rhs) {
    basic_string<T, Traits, Alloc> r;
    const T* ld = lhs.data();
    typename basic_string<T, Traits, Alloc>::size_type ln = lhs.size();
    const T* rd = rhs.data();
    typename basic_string<T, Traits, Alloc>::size_type rn = rhs.size();
    r.reserve(ln + rn);
    r.do_replace(0, 0, ld, ld + ln, random_access_iterator_tag());
    r.do_replace(ln, 0, rd, rd + rn, random_access_iterator_tag());
    return r;
}
} // namespace std

// inferred: windows of the character under the cursor (m48): the select window over its position with the
// four command panes (mDirs) placed on its anchors, the tension mark, the fade in (mLayouts.m20 frames), the
// name, the hit points (colour by the rate), the counters m224/m300 of its status, and for each command its
// name, icon and state; with the hints on, the skill names and descriptions go to the message of GameUI
// NONMATCHING: 90.6% - the growth path of std::vector::push_back (std/vector.h moves the strings one by one, the
// original with memcpy), an unused test of lbl_8087F8A0 at the start, and the stack slot of `pos`
void TacView::fn_805AC758() {
    if (m48.mState == 0 && m48.m04->mType != 2) {
        CharaTask* chara = m48.m04;
        atn::Vector3 pos = ((const TacViewChara_unk*)&m48)->fn_805AE138();
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
        mLayouts.mSelect->MeltTask();
        mLayouts.mSelect->mFrame = 15.0f;
        SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "basePoint", 0, 320.0f);
        SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "basePoint", 1, 220.0f);
        atn::D2A_DrawRect rect;
        rect = GetPaneRect_unk(mLayouts.mSelect, "select_center");
        mLayouts.mHpFlash->SetParam1_unk("basePoint", &rect.x);
        rect = GetPaneRect_unk(mLayouts.mSelect, "select_left");
        mLayouts.mDirs[1]->SetValues1Rect_unk("basePoint", rect);
        rect = GetPaneRect_unk(mLayouts.mSelect, "select_right");
        mLayouts.mDirs[2]->SetValues1Rect_unk("basePoint", rect);
        rect = GetPaneRect_unk(mLayouts.mSelect, "select_up");
        mLayouts.mDirs[0]->SetValues1Rect_unk("basePoint", rect);
        rect = GetPaneRect_unk(mLayouts.mSelect, "select_down");
        mLayouts.mDirs[3]->SetValues1Rect_unk("basePoint", rect);
        if (chara->mStatus.m228 < 1.0f) {
            fn_801F6C80(mLayouts.mDirs[0], "tensionOn", 0.3f);
        } else {
            fn_801F6C80(mLayouts.mDirs[0], "tensionOn", 1.0f);
        }
        float fade = (0.0f > (1.0f < mLayouts.m20 / 12.0f ? 1.0f : mLayouts.m20 / 12.0f))
                         ? 0.0f
                         : (1.0f < mLayouts.m20 / 12.0f ? 1.0f : mLayouts.m20 / 12.0f);
        if (mLayouts.m1C == 0) {
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "fade", fade);
            SetLayoutValue_unk(&mLayouts.mHpFlash->mData.m10, "fade", fade);
        } else {
            fade = 1.0f - fade;
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "fade", fade);
            SetLayoutValue_unk(&mLayouts.mHpFlash->mData.m10, "fade", fade);
        }
        CharaInfo_unk* info = chara->m60;
        if (info != 0) {
            mLayouts.mFace->MeltTask();
            SetLayoutText_unk(&mLayouts.mFace->mData.m10, "chara_name", (const wchar_t*)info->m08);
            fn_804A4AEC(mLayouts.mFace, info, 4, 2);
        }
        float rate = chara->mStatus.m04 / (float)chara->mStatus.mParamB.m84;
        int count = chara->mStatus.m224;
        int wait = chara->mStatus.m300;
        mLayouts.mSelect->SetNumber_unk("nowHP", (int)chara->mStatus.m04, 0);
        SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "hpRate", rate);
        if (rate < 0.3f) {
            mLayouts.mHpFlash->MeltTask();
            mLayouts.mSelect->SetColor_unk("barColor", 255.0f, 0.0f, 0.0f);
            mLayouts.mHpFlash->SetColor_unk("flashColor", 255.0f, 0.0f, 0.0f);
        } else if (rate < 0.6f) {
            mLayouts.mHpFlash->MeltTask();
            mLayouts.mSelect->SetColor_unk("barColor", 255.0f, 180.0f, 20.0f);
            mLayouts.mHpFlash->SetColor_unk("flashColor", 255.0f, 180.0f, 20.0f);
        } else {
            mLayouts.mSelect->SetColor_unk("barColor", 255.0f, 255.0f, 255.0f);
            mLayouts.mHpFlash->SetColor_unk("flashColor", 255.0f, 255.0f, 255.0f);
        }
        if (wait <= 0) {
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colOn", 1.0f);
            mLayouts.mSelect->mFrame = 20.0f * (5 - count) + 30.0f;
        } else {
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colOn", 0.0f);
            mLayouts.mSelect->mFrame = 0.0f > 15.0f * (2 - wait) ? 0.0f : 15.0f * (2 - wait);
        }
        if (count <= 1) {
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colR", 255.0f);
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colG", 0.0f);
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colB", 0.0f);
        } else if (count <= 2) {
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colR", 255.0f);
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colG", 180.0f);
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colB", 0.0f);
        } else {
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colR", 0.0f);
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colG", 255.0f);
            SetLayoutValue_unk(&mLayouts.mSelect->mData.m10, "colB", 100.0f);
        }
        for (int i = 0; i < 4; i++) {
            HitInfo_unk* skill = (HitInfo_unk*)m54.m54[i];
            if (skill != 0) {
                mLayouts.mDirs[i]->MeltTask();
                atn::D2A_DrawRect text = mLayouts.mDirs[i]->GetRect_unk("TextMain");
                fn_801F6C80(mLayouts.mDirs[i], "msgLen",
                            fn_8006EF48(lbl_8087EEC8, text.w, (const wchar_t*)((HitInfo_unk*)m54.m54[i])->m08,
                                        0.0f, 1, 1));
                fn_801F837C(mLayouts.mDirs[i], "text", (const wchar_t*)((HitInfo_unk*)m54.m54[i])->m08);
                fn_801F837C(mLayouts.mDirs[i], "textSub", (const wchar_t*)((HitInfo_unk*)m54.m54[i])->m08);
                if (i == (int)m54.mObj) {
                    fn_801F6C80(mLayouts.mDirs[i], "skipSelect", 1.0f);
                } else {
                    fn_801F6C80(mLayouts.mDirs[i], "skipSelect", 0.0f);
                }
                if (m54.m84[i] < 0) {
                    fn_801F6C80(mLayouts.mDirs[i], "iconOn", 0.0f);
                } else {
                    fn_801F6C80(mLayouts.mDirs[i], "iconOn", 1.0f);
                    fn_803DDD54(mLayouts.mDirs[i], m54.m84[i]);
                }
                fn_801F6C80(mLayouts.mDirs[i], "canSelect", m54.m50[i]);
                if (mLayouts.m1C == 0) {
                    mLayouts.mDirs[i]->mFrame = mLayouts.m20;
                } else if (mLayouts.mDirs[i] == mLayouts.m1C) {
                    mLayouts.mDirs[i]->mFrame = mLayouts.m20 + 16;
                } else {
                    mLayouts.mDirs[i]->mFrame = mLayouts.m20 + 31;
                }
            }
        }
        if (SetConfig::sInstance->mTacviewHint != 0) {
            std::vector<std::basic_string<wchar_t> > lines;
            for (int i = 0; i < 4; i++) {
                HitInfo_unk* skill = (HitInfo_unk*)m54.m54[i];
                if (skill != 0) {
                    wchar_t text[0x100];
                    fn_800DD3FC(text, L"%s\xFF1A%s\n", skill->m08, *(const wchar_t* const*)skill->m0C); // inferred: description at 0x0C
                    lines.push_back(std::basic_string<wchar_t>(text));
                }
            }
            while (lines.size() < 3) {
                lines.push_back(std::basic_string<wchar_t>(L""));
            }
            lbl_8087F490->fn_803E4978((lines[0] + lines[1] + lines[2]).c_str());
        }
    }
    int frame = mLayouts.m20 + 1;
    mLayouts.m20 = (frame > 0 ? frame : 0) < 15 ? (frame > 0 ? frame : 0) : 15;
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

void TacView::fn_805ADD80(CharaTask* chara, HitInfo_unk* hit) {
    for (unsigned int i = 0; i < mEntries.size(); i++) {
        TacViewEntry_unk& entry = mEntries.mData[i];
        if (entry.mChara == chara && entry.mHit == hit && (entry.mState & 2) != 2) {
            // inferred: clears the state and sets the finished bit (the original builds the 2 from the zero)
            entry.mState = 0;
            entry.mState |= 2;
        }
    }
}

void TacView::fn_805ADDD4(CharaTask* chara) {
    for (unsigned int i = 0; i < mEntries.size(); i++) {
        TacViewEntry_unk* entry = &mEntries.mData[i];
        if (chara == 0 || entry->mChara == chara) {
            if ((entry->mState & 1) == 1) {
                for (unsigned int j = 0; j < mEntries.size(); j++) {
                    TacViewEntry_unk* other = &mEntries.mData[j];
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


