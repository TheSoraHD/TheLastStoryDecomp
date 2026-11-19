// inferred: game over screen (choices: continue, load, title, retry...); the name is in the DOL
// (__vt__12GameOverTask). File boundaries in docs/splits-auto-C.md (group 2).

#include "atn/Camera_unk.h"
#include "atn/D2A_Task.h"
#include "game/GameTask.h"
#include "game/CampSaveLoad.h"
#include "atn/EfpHandle_unk.h"
#include "atn/FadeCommon.h"
#include "atn/FixedArray.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/Task.h"
#include "MP/SaveLoad.h"
#include "game/GameLoadBackground.h"
#include "game/GameLoadTask.h"
#include "game/MenuCommonTask.h"
#include "atn/RenderSetting_unk.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: message text
extern "C" atn::SoundRef fn_80117228(int id);        // inferred: plays a system sound effect
extern "C" void fn_800D2338(atn::Task* task);        // inferred: deletes the task
extern "C" void* lbl_8087EEC8;                       // inferred: font
extern "C" float fn_8006EF48(void* font, float size, const wchar_t* text, float f2, int a, int b); // inferred: width

// inferred: d2anime task (made by fn_801F3FF8; the view of game/MenuCommonTask.h)
typedef MenuD2A_unk GameOverD2A_unk;
extern "C" GameOverD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
// inferred: 2D point of a d2anime pane (0x14 bytes)
struct GameOverPoint_unk {
    GameOverPoint_unk() {}
    float x;
    float y;
    float m08;
    float m0C;
    float m10;
};
extern "C" GameOverPoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_804A39EC(GameOverD2A_unk* anime, int arg1, int repeat, float frame, float speed);

extern "C" void fn_804A4300(MenuCommonTask* menu, const wchar_t* text);
extern "C" void fn_804A5B9C(MenuCommonTask* menu, int id, const char* name); // inferred: adds an entry
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" int lbl_8087DCCC;
extern "C" int lbl_8087F434;
#include "atn/SequenceBoot_unk.h"
#include "atn/RenderManager.h"
#include "atn/TextTable_unk.h"
#include "game/GameConfig_unk.h"
extern "C" MP::SaveData_unk* lbl_8087F460;
extern "C" int fn_803BE854(MP::SaveData_unk* data);
extern "C" GameTask* lbl_8087F430;
extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: effects
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
// inferred: weak copy 0x800F7FA0 of another file (as in game/Em203GreaterDaemon.cpp)
extern "C" inline EffectMgr_unk* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, void* model, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const float* color, int arg7, int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings
// inferred: weak copy 0x802F0990 of another file (as in game/NewCampTask.cpp)
extern "C" inline RenderSetting_unk* fn_802F0990() {
    return lbl_8087EFA8;
}

// inferred: as in game/GameTask.cpp
inline void RenderSetting_unk::SetDamage_unk(DamageEffectParam_unk p) {
    m374 = p;
}
inline DamageEffectParam_unk* RenderSetting_unk::GetDamage_unk() {
    return &m374;
}
inline void RenderSetting_unk::SetColorMatrix(const ColorMatrixParam_unk& p) {
    m324 = p;
}

// inferred: choices of each mode (ids of the actions) and texts (message ids)
static const int sMsgTitle[4] = {0x2762, 0x2766, 0x2765, 0x2762};
static const int sOptTitle[2] = {1, 2};
static const int sMsg[4] = {0x2764, 0x2766, 0x2765, 0x2762};
static const int sOpt[3] = {0, 1, 2};
static const int sMsgEvent[4] = {0x2764, 0x2766, 0x2765, 0x2763};
static const int sOptEvent[4] = {0, 1, 2, 3};
static const int sMsgRetry[4] = {0x2764, 0x2762, 0x2765, 0x2762};
static const int sOptRetry[2] = {0, 2};
static const int sMsgRetryTitle[4] = {0x2762, 0x2762, 0x2765, 0x2762};
static const int sOptRetryTitle[1] = {2};
static const int sMsgArena[4] = {0x2767, 0x2766, 0x2765, 0x2762};
static const int sOptArena[3] = {4, 1, 2};

class GameOverTask : public atn::Task {
public:
    GameOverTask(atn::Task* parent, int mode);
    virtual ~GameOverTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    void fn_80377298(); // inferred: builds the choices

    // inferred: changes the state (name invented)
    void SetState_unk(int state) {
        mPrevState = mState;
        mState = state;
        switch (mState) {
        case 1:
            mMenu->mSpeed = 0.0f;
            mMenu->mFrame = 30.0f;
            break;
        case 2:
            if (MenuLoadTask::sInstance == 0) {
                MenuLoadTask::fn_80117914(this, mSave, 0);
            }
            break;
        }
    }

    GameOverD2A_unk* mMenu;     // 0x48: ui_gameover.d2a
    GameOverD2A_unk* mCursor;   // 0x4C: camp_cursor.d2a
    GameOverD2A_unk* mDemoEnd;  // 0x50: ui_demo_end.d2a
    atn::EfpFile mEfp;    // 0x54
    unsigned char m60[4];
    GameLoadTask* mSave;    // 0x64
    int mLoading;               // 0x68
    int mMode;                  // 0x6C
    int mState;                 // 0x70
    int mPrevState;             // 0x74
    int mSelect;                // 0x78
    atn::FixedArray_unk<int, 5> mChoices; // 0x7C
    Camera_unk mCamera;         // 0x94
    int mTimer;                 // 0x288
    atn::FadeColor* mFade;      // 0x28C
    unsigned char m290[0x298 - 0x290]; // inferred: the object is 0x298 bytes
};
extern "C" GameOverTask* lbl_8087F440;

// inferred: creates the instance
extern "C" GameOverTask* fn_80376B68(atn::Task* parent, int mode) {
    if (parent != 0 && lbl_8087F440 == 0) {
        lbl_8087F440 = new ((MemoryTag)1, "", "", 0) GameOverTask(parent, mode);
    }
    return lbl_8087F440;
}

// NONMATCHING: 99.59% - registers of the name copy (source and destination swapped)
GameOverTask::GameOverTask(atn::Task* parent, int mode)
    : atn::Task(parent), mDemoEnd(0), mSave(0), mLoading(0), mMode(mode), mState(0), mSelect(0), mCamera(0, 0),
      mTimer(0), mFade(0) {
    mMenu = fn_801F3FF8(this, "/data/d2anime/ui_gameover.d2a", 0);
    mMenu->fn_800D246C(1);
    mCursor = fn_801F3FF8(this, "/data/d2anime/camp_cursor.d2a", 0);
    mCursor->fn_800D246C(1);
    if (lbl_8087F0A8->m194 != 0) {
        mDemoEnd = fn_801F3FF8(this, "/data/d2anime/ui_demo_end.d2a", 0);
        mDemoEnd->fn_800D246C(1);
    }
    mEfp.LoadRequest("be010_008.efp");
    GameLoadTask::Option arg("\0", 0);
    mSave = GameLoadTask::fn_803B27F8(this, arg);
    mSave->mD64 = 0;
}

GameOverTask::~GameOverTask() {
    lbl_8087F440 = 0;
}

// inferred: plays an effect at the origin (as PlayEfp_unk of gmk/SwingObject.cpp)
inline void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpFile* efp) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    float color[4];
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color[0] = 1.0f;
    color[1] = 1.0f;
    color[2] = 1.0f;
    color[3] = 1.0f;
    fn_8023A8B4(mgr, efp, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// NONMATCHING: 96.23% - scheduling of the dead stores of the bloom and colour matrix temporaries
// (the last zero of BloomParam_unk::mScale and ColorMatrixParam_unk::mEnabled; mEnabled set at the end of the
// ColorMatrixParam_unk ctor gives 98.44 but drops LastCocoonEgg/NewCampTask/NewTitleTask; tried named locals,
// m324 directly, Set_unk of the colours, other orders of the bias/scale zeros). The damage fields are
// set zoom, colour, enabled (order of the dead stores of the original).
int GameOverTask::vf0C() {
    if (fn_800D3FA4() != 0 && mEfp.IsLoading() == 0) {
        fn_80232B7C(this, 0);
        PlayEfp_unk(fn_800F7FA0(), &mEfp);
        fn_804A39EC(mMenu, 1, 0, 0.0f, 1.0f);
        mCursor->fn_800D246C(0);
        mCursor->mFlags |= 4;
        mTimer = 0;
        if (mDemoEnd != 0) {
            mDemoEnd->fn_800D246C(0);
            mDemoEnd->mRepeat = 0;
            mDemoEnd->mFrame = 0.0f;
            mDemoEnd->mFlags |= 4;
        }
        DamageEffectParam_unk damage(*fn_802F0990()->GetDamage_unk());
        damage.mZoom = -0.55f;
        damage.mColor = RsColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
        damage.mEnabled = 1;
        fn_802F0990()->SetDamage_unk(damage);
        fn_802F0990()->m03C = RsColor_unk(0.6f, 0.6f, 0.6f, 1.0f);
        fn_802F0990()->m054 = BloomParam_unk();
        fn_802F0990()->SetColorMatrix(ColorMatrixParam_unk());
        fn_80377298();
        return 1;
    }
    return 0;
}

// inferred: the event flag is set (name invented)
inline bool IsFlagSet_unk(GameTask* field, int id) {
    return ((GameTask*)field)->fn_80370174(id) == 1;
}

void GameOverTask::fn_80377298() {
    const int* opts;
    mChoices.mSize = 0;
    GameTask* field = lbl_8087F430;
    if (field != 0 && lbl_8087DCCC != 0) {
        if (field != 0 && lbl_8087F430->fn_803754F0() != 0) {
            mMode = 5;
            mChoices.push_back((opts = sOptArena)[0]);
            mChoices.push_back(opts[1]);
            mChoices.push_back(opts[2]);
        } else if (mMode == 1) {
            mChoices.push_back((opts = sOptEvent)[0]);
            mChoices.push_back(opts[1]);
            mChoices.push_back(opts[2]);
            mChoices.push_back(opts[3]);
            if (IsFlagSet_unk(lbl_8087F430, 0xD4)) {
                mSelect = 3;
            }
        } else if (lbl_8087F430->fn_80370174(0x122) == 0) {
            mMode = 0;
            mChoices.push_back((opts = sOpt)[0]);
            mChoices.push_back(opts[1]);
            mChoices.push_back(opts[2]);
        } else {
            mMode = 3;
            mChoices.push_back((opts = sOptRetry)[0]);
            mChoices.push_back(opts[1]);
        }
    } else if (((GameTask*)field)->fn_80370174(0x122) == 0) {
        mMode = 2;
        mChoices.push_back((opts = sOptTitle)[0]);
        mChoices.push_back(opts[1]);
    } else {
        mMode = 4;
        opts = sOptRetryTitle;
        mChoices.push_back(opts[0]);
    }
}

// inferred: setters of the layout panes (names invented)
inline void SetParam_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetRate_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
inline GameOverPoint_unk GetPoint_unk(atn::Task* task, const char* name) {
    return fn_801F4E8C(task, fn_800DC6B4(name));
}

// inferred: the names of the panes are formatted in one 0x40-byte buffer; the options are read before the
// timer is updated
void GameOverTask::vf10() {
    if (MenuCommonTask::sInstance != 0) {
        MenuCommonTask::sInstance->m9C = 0;
    }
    if (mFade != 0) {
        if (mFade->mFrame < mFade->mDuration) {
            return;
        }
        switch (mChoices.mData[mSelect]) {
        case 0:
        case 1:
            if (lbl_8087F430 != 0) {
                lbl_8087F430->fn_80370F8C(0, lbl_8087F460, 0);
            }
            break;
        case 2:
            fn_800D2338((atn::Task*)lbl_8087F430);
            sSequenceBoot_unk->SetNext_unk("Title");
            break;
        case 3:
            if (lbl_8087F430 != 0) {
                lbl_8087F430->fn_80370F8C(1, lbl_8087F460, 0);
            }
            break;
        case 4:
            if (lbl_8087F430 != 0) {
                lbl_8087F430->fn_80370320(0x1B, 1, 0);
                lbl_8087F430->fn_80370F8C(0, 0, 1);
            }
            break;
        }
        fn_800D2338(this);
        return;
    }
    GameConfig_unk* options = lbl_8087F0A8;
    mTimer++;
    mMenu->mFlags |= 4;
    mCursor->mFlags |= 4;
    if (mDemoEnd != 0) {
        mDemoEnd->mFlags |= 4;
    }
    int fade = 0;
    if (mState == 0) {
        if (mTimer > options->mTitle.mLogoFadeStart) {
            if (mDemoEnd != 0) {
                mDemoEnd->mFlags &= ~4;
                if (mDemoEnd->IsEnd_unk() &&
                    (fn_800A555C(lbl_8087EF70, 0, 4) != 0 || fn_800A555C(lbl_8087EF70, 0, 5) != 0)) {
                    fade = 1;
                }
            } else {
                mMenu->mFlags &= ~4;
                if (mMenu->IsEnd_unk() &&
                    (fn_800A555C(lbl_8087EF70, 0, 4) != 0 || fn_800A555C(lbl_8087EF70, 0, 5) != 0)) {
                    SetState_unk(1);
                }
            }
        }
        SetParam_unk(&mCursor->mData.m10, "cursor_pos", 0, -1000.0f);
        SetParam_unk(&mCursor->mData.m10, "cursor_pos", 1, -1000.0f);
    } else if (mState == 1) {
        mMenu->mFlags &= ~4;
        mCursor->mFlags &= ~4;
        if (fn_800A55AC(lbl_8087EF70, 0, 0x1A) != 0 || fn_800A55AC(lbl_8087EF70, 0, 0) != 0) {
            if (--mSelect < 0) {
                mSelect = mChoices.mSize - 1;
            }
            fn_80117228(3);
        } else if (fn_800A55AC(lbl_8087EF70, 0, 0x19) != 0 || fn_800A55AC(lbl_8087EF70, 0, 1) != 0) {
            if (++mSelect >= (int)mChoices.mSize) {
                mSelect = 0;
            }
            fn_80117228(3);
        } else if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
            int choice = mChoices.mData[mSelect];
            if (choice == 0 || choice == 3) {
                if (mSave != 0) {
                    if (mSave->mD60 == 0) {
                        mSave->fn_803B8758(0);
                        mLoading = 1;
                    } else {
                        mSave->fn_803B3270(0);
                    }
                    mState = 3;
                }
                fn_80117228(1);
                return;
            } else if (choice == 1) {
                SetState_unk(2);
                fn_80117228(1);
                return;
            } else {
                fn_80117228(1);
                fade = 1;
            }
        }
    } else if (mState == 2) {
        if (MenuLoadTask::sInstance != 0) {
            if (MenuLoadTask::sInstance->m13C == 7) {
                fn_800D2338(MenuLoadTask::sInstance);
                SetState_unk(1);
            } else if (MenuLoadTask::sInstance->m13C == 8) {
                fade = 1;
            }
        }
    } else if (mState == 3) {
        int done = 0;
        if (mLoading != 0) {
            if (mSave->fn_803B87A8() == 0) {
                if (mSave->mC84 == 7) {
                    done = 1;
                } else {
                    mLoading = 0;
                    mSave->fn_803B3270(0);
                }
            }
        } else if (mSave != 0 && mSave->m14F60->m154 == 0) {
            if (mSave->m14F60->fn_803B8144()) {
                done = 1;
            } else {
                mSave->fn_803B2FBC(0);
                if (fn_803BE854(lbl_8087F460)) {
                    if (lbl_8087F460 != 0) {
                        delete lbl_8087F460;
                        lbl_8087F460 = 0;
                    }
                    done = 1;
                } else {
                    fade = 1;
                }
            }
        }
        if (done) {
            lbl_8087DCCC = 0;
            lbl_8087F434 = 1;
            GameLoadTask* save = mSave;
            if (save != 0) {
                save->mD60 = 0;
                save->mC84 = 0;
            }
            fn_80377298();
            MenuCommonTask::sInstance->fn_804A2F98();
            fn_804A4300(MenuCommonTask::sInstance, lbl_8087F1E4->Get_unk(0x237));
            SetState_unk(4);
            fn_80117228(11);
        }
    } else if (mState == 4) {
        if (MenuCommonTask::sInstance->m54 == 0) {
            MenuCommonTask::sInstance->fn_804A313C();
            MenuCommonTask::sInstance->fn_804A2FBC();
            SetState_unk(1);
            fn_80117228(1);
        }
    }
    char name[0x40];
    for (int i = 0; i < 5; i++) {
        int id = sMsg[i];
        switch (mMode) {
        case 0:
            id = sMsg[i];
            break;
        case 1:
            id = sMsgEvent[i];
            break;
        case 2:
            id = sMsgTitle[i];
            break;
        case 3:
            id = sMsgRetry[i];
            break;
        case 4:
            id = sMsgRetryTitle[i];
            break;
        case 5:
            id = sMsgArena[i];
            break;
        }
        const wchar_t* text = fn_80116FC0(0, id);
        float len = fn_8006EF48(lbl_8087EEC8, 14.0f, text, 0.0f, 1, 1);
        sprintf(name, "msgLen%02d", i);
        SetRate_unk(&mMenu->mData.m10, name, len);
        sprintf(name, "msg%02d", i);
        SetText_unk(&mMenu->mData.m10, name, text);
    }
    int pos = 0;
    switch (mChoices.mData[mSelect]) {
    case 0:
        pos = 0;
        break;
    case 1:
        pos = 1;
        break;
    case 2:
        pos = 2;
        break;
    case 3:
        pos = 3;
        break;
    case 4:
        pos = 0;
        break;
    }
    sprintf(name, "cursor_point%02d", pos);
    GameOverPoint_unk point = GetPoint_unk(mMenu, name);
    SetParam_unk(&mCursor->mData.m10, "cursor_pos", 0, point.x);
    SetParam_unk(&mCursor->mData.m10, "cursor_pos", 1, point.y);
    SetParam_unk(&mCursor->mData.m10, "cursor_pos", 4, point.m10);
    SetRate_unk(&mCursor->mData.m10, "wRate", point.m08);
    SetRate_unk(&mCursor->mData.m10, "hRate", point.m0C);
    if (MenuCommonTask::sInstance != 0) {
        sprintf(name, "select_text%02d", pos);
        fn_804A5B9C(MenuCommonTask::sInstance, (int)mMenu, name);
    }
    if (fade) {
        mFade = atn::FadeColor::Create((atn::Task*)lbl_8087F430->m20, 30, 0, 0xFF000000);
        mFade->mWait = 2;
        mFade->mActive = 1;
        mFade->mDeleteAtEnd = 1;
    }
}

// inferred: the camera position and target go through local copies (asm copies, not removed)
void GameOverTask::vf14() {
    if (mState == 1) {
        SetRate_unk(&mMenu->mData.m10, "selectOn", 1.0f);
    } else {
        SetRate_unk(&mMenu->mData.m10, "selectOn", 0.0f);
    }
    atn::Vector3 pos = *mCamera.GetPosition_unk();
    atn::Vector3 target = *mCamera.GetTarget_unk();
    pos = atn::Vector3(0.0f, -99500.0f, 0.0f);
    target = atn::Vector3(0.0f, -99500.0f, 10.0f);
    mCamera.SetPosition(pos);
    mCamera.SetTarget_unk(target);
    mCamera.mFovy = 1.0471976f;
    mCamera.fn_8004B378();
    fn_8008B964()->SetCamera_unk(mCamera);
}
