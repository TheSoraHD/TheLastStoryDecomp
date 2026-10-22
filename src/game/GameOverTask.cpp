// inferred: game over screen (choices: continue, load, title, retry...); the name is in the DOL
// (__vt__12GameOverTask). File boundaries in docs/splits-auto-C.md (group 2).

#include "atn/Camera_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/FadeCommon.h"
#include "atn/FixedArray.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "atn/Task.h"
#include "MP/SaveLoad.h"

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
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width

// inferred: d2anime task (atn/D2A_Task.h); only the members used here
struct GameOverD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xA0 - 0x58]; // 0x58: inferred: layout
    float mEndFrame;                    // 0xA0
    unsigned char mA4[0xFC - 0xA4];
    unsigned int mFlag31 : 1;
    unsigned int mFlag30 : 1;
    unsigned int mFlag29 : 1;
    unsigned int mRepeat : 1; // inferred
    unsigned int mFC_rest : 28;
    float mFrame; // 0x100
    float mSpeed; // 0x104
    // inferred: the last frame was reached (name invented)
    bool IsEnd_unk() { return mFrame >= mEndFrame; }
};
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

// inferred: save data object made by fn_803B27F8 (as in game/NewCampPartsTop.cpp)
class GameSaveLoadBackgroundCommon {
public:
    int fn_803B8144(); // inferred: error code of the save/load manager
    unsigned char m000[0x154];
    unsigned char m154; // inferred: active flag
};
struct CampSaveData_unk : public atn::Task {
    unsigned char m048[0xC84 - 0x48];
    int mC84;
    unsigned char mC88[0xD60 - 0xC88];
    int mD60;
    int mD64;
    unsigned char mD68[0x14F60 - 0xD68];
    GameSaveLoadBackgroundCommon* m14F60;
};
// inferred: name and flag given to fn_803B27F8 (as in game/CampSaveLoad.cpp)
struct CampSaveArg_unk {
    char mName[0x40];
    int mFlag; // 0x40
};
inline void CopyString_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}
extern "C" CampSaveData_unk* fn_803B27F8(atn::Task* parent, const CampSaveArg_unk* arg);
extern "C" void fn_803B3270(CampSaveData_unk* data, int slot);
extern "C" void fn_803B2FBC(CampSaveData_unk* data, int slot);
class GameSaveLoadCommon {
public:
    void fn_803B8758(int arg);
    int fn_803B87A8();
};
class MenuLoadTask : public atn::Task {
public:
    static MenuLoadTask* fn_80117914(atn::Task* parent, CampSaveData_unk* data);
    static MenuLoadTask* sInstance;
    unsigned char m48[0x13C - 0x48];
    int m13C; // inferred: state
};
class MenuCommonTask {
public:
    void fn_804A2F98(); // inferred: hides everything
    void fn_804A2FBC(); // inferred: resets everything
    void fn_804A313C(); // inferred: resets the info windows
    static MenuCommonTask* sInstance;
    unsigned char m00[0x54];
    int m54;
    unsigned char m58[0x9C - 0x58];
    int m9C;
};
extern "C" void fn_804A4300(MenuCommonTask* menu, const wchar_t* text);
extern "C" void fn_804A5B9C(MenuCommonTask* menu, GameOverD2A_unk* anime, const char* name);
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4)
struct GameOverMsg_unk {
    const wchar_t* Get_unk(int id) {
        return mText[id] != 0 ? mText[id] : lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" GameOverMsg_unk* lbl_8087F1E4;
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" int lbl_8087DCCC;
extern "C" int lbl_8087F434;
#include "atn/SequenceBoot_unk.h"
extern "C" MP::SaveData_unk* lbl_8087F460;
extern "C" int fn_803BE854(MP::SaveData_unk* data);
// inferred: field (lbl_8087F430); only the members used here
struct GameOverField_unk {
    unsigned char m0000[0x20];
    atn::Task* m20;
};
extern "C" GameOverField_unk* lbl_8087F430;
extern "C" int fn_803754F0(GameOverField_unk* field);
extern "C" int fn_80370174(GameOverField_unk* field, int id); // inferred: event flag
extern "C" void fn_80370320(GameOverField_unk* field, int id, int value, int arg);
extern "C" void fn_80370F8C(GameOverField_unk* field, int a, MP::SaveData_unk* data, int b);
// inferred: options (lbl_8087F0A8)
struct GameOverOptions_unk {
    unsigned char m000[0x194];
    int m194; // inferred: demo version
    unsigned char m198[0x5A4 - 0x198];
    int m5A4; // inferred: frames before the choices
};
extern "C" GameOverOptions_unk* lbl_8087F0A8;
// inferred: effects
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, void* model, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const float* color, int arg7, int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: game state (lbl_8087EFA8): lighting and camera of the scene
struct GameOverLight_unk {
    int m00;
    int m04;
    float m08[4];
    float m18;
    float m1C;
    float m20;
};
struct GameOverEnv_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24[16];
    int m64[6];
};
struct GameOverCamState_unk {
    int m00;
    float m04[16];
    int m44;
    int m48;
    float m4C;
};
struct GameOverState_unk {
    unsigned char m000[0x3C];
    float m3C[4];
    unsigned char m04C[0x54 - 0x4C];
    GameOverEnv_unk m54;
    unsigned char m0D4[0x324 - 0xD4];
    GameOverCamState_unk m324;
    GameOverLight_unk m374;
};
extern "C" GameOverState_unk* lbl_8087EFA8;
// inferred: camera of the screen (lbl_8087EFB4)
struct GameOverCamera_unk {
    unsigned char m000[0x104];
    Camera_unk mCamera; // 0x104
};
extern "C" GameOverCamera_unk* lbl_8087EFB4;

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
        switch (state) {
        case 1:
            mMenu->mSpeed = 0.0f;
            mMenu->mFrame = 30.0f;
            break;
        case 2:
            if (MenuLoadTask::sInstance == 0) {
                MenuLoadTask::fn_80117914(this, mSave);
            }
            break;
        }
    }

    GameOverD2A_unk* mMenu;     // 0x48: ui_gameover.d2a
    GameOverD2A_unk* mCursor;   // 0x4C: camp_cursor.d2a
    GameOverD2A_unk* mDemoEnd;  // 0x50: ui_demo_end.d2a
    atn::EfpHandle_unk mEfp;    // 0x54
    unsigned char m60[4];
    CampSaveData_unk* mSave;    // 0x64
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
        lbl_8087F440 = new (1, "", "", 0) GameOverTask(parent, mode);
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
    mEfp.fn_8023780C("be010_008.efp");
    CampSaveArg_unk arg;
    CopyString_unk(arg.mName, "\0");
    arg.mFlag = 0;
    mSave = fn_803B27F8(this, &arg);
    mSave->mD64 = 0;
}

GameOverTask::~GameOverTask() {
    lbl_8087F440 = 0;
}

// NONMATCHING: 48.93% - the scene setup (lighting, environment and camera state of lbl_8087EFA8) is
// copied through temporary structs whose layout is not reconstructed
int GameOverTask::vf0C() {
    if (fn_800D3FA4() == 0 || mEfp.fn_80237874() != 0) {
        return 0;
    }
    fn_80232B7C(this, 0);
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    fn_8023A8B4(lbl_8087F3C0, &mEfp, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
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
    GameOverState_unk* state = lbl_8087EFA8;
    GameOverLight_unk light;
    light.m00 = 1;
    light.m04 = state->m374.m04;
    light.m08[0] = 0.0f;
    light.m08[1] = 0.0f;
    light.m08[2] = 0.0f;
    light.m08[3] = 1.0f;
    light.m18 = state->m374.m18;
    light.m1C = state->m374.m1C;
    light.m20 = -0.55f;
    state->m374 = light;
    lbl_8087EFA8->m3C[0] = 0.6f;
    lbl_8087EFA8->m3C[1] = 0.6f;
    lbl_8087EFA8->m3C[2] = 0.6f;
    lbl_8087EFA8->m3C[3] = 1.0f;
    GameOverEnv_unk env;
    env.m00 = 1;
    env.m04 = 0;
    env.m08 = 3;
    env.m0C = 3;
    env.m10 = 1;
    env.m14 = 1.0f;
    env.m18 = 1.25f;
    env.m1C = 1.25f;
    env.m20 = 0.0f;
    for (int i = 0; i < 16; i++) {
        env.m24[i] = 1.0f;
    }
    env.m24[0] = 2.0f;
    env.m24[2] = 0.5f;
    env.m24[4] = 0.5f;
    for (int i = 0; i < 6; i++) {
        env.m64[i] = 0;
    }
    lbl_8087EFA8->m54 = env;
    GameOverCamState_unk cam;
    cam.m00 = 0;
    for (int i = 0; i < 16; i++) {
        cam.m04[i] = 0.0f;
    }
    cam.m04[0] = 1.0f;
    cam.m04[5] = 1.0f;
    cam.m04[10] = 1.0f;
    cam.m04[15] = 1.0f;
    cam.m44 = 0;
    cam.m48 = 0;
    cam.m4C = 1.0f;
    lbl_8087EFA8->m324 = cam;
    fn_80377298();
    return 1;
}

// inferred: the event flag is set (name invented)
inline bool IsFlagSet_unk(GameOverField_unk* field, int id) {
    return fn_80370174(field, id) == 1;
}

// NONMATCHING: 99.45% - register of the address of the single choice in .sdata2 (r3/r4)
void GameOverTask::fn_80377298() {
    const int* opts;
    mChoices.mSize = 0;
    GameOverField_unk* field = lbl_8087F430;
    if (field != 0 && lbl_8087DCCC != 0) {
        if (field != 0 && fn_803754F0(lbl_8087F430) != 0) {
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
        } else if (fn_80370174(lbl_8087F430, 0x122) == 0) {
            mMode = 0;
            mChoices.push_back((opts = sOpt)[0]);
            mChoices.push_back(opts[1]);
            mChoices.push_back(opts[2]);
        } else {
            mMode = 3;
            mChoices.push_back((opts = sOptRetry)[0]);
            mChoices.push_back(opts[1]);
        }
    } else if (fn_80370174(field, 0x122) == 0) {
        mMode = 2;
        mChoices.push_back((opts = sOptTitle)[0]);
        mChoices.push_back(opts[1]);
    } else {
        mMode = 4;
        mChoices.push_back((opts = sOptRetryTitle)[0]);
    }
}

// inferred: setters of the layout panes (names invented)
inline void SetParam_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetRate_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

// NONMATCHING: 89.58% - the original keeps the six text tables in registers across the text loop,
// passes a third argument to MenuLoadTask::fn_80117914 and builds the fade with other stores
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
                fn_80370F8C(lbl_8087F430, 0, lbl_8087F460, 0);
            }
            break;
        case 2:
            fn_800D2338((atn::Task*)lbl_8087F430);
            sSequenceBoot_unk->SetNext_unk("Title");
            break;
        case 3:
            if (lbl_8087F430 != 0) {
                fn_80370F8C(lbl_8087F430, 1, lbl_8087F460, 0);
            }
            break;
        case 4:
            if (lbl_8087F430 != 0) {
                fn_80370320(lbl_8087F430, 0x1B, 1, 0);
                fn_80370F8C(lbl_8087F430, 0, 0, 1);
            }
            break;
        }
        fn_800D2338(this);
        return;
    }
    mTimer++;
    mMenu->mFlags |= 4;
    mCursor->mFlags |= 4;
    if (mDemoEnd != 0) {
        mDemoEnd->mFlags |= 4;
    }
    int fade = 0;
    if (mState == 0) {
        if (mTimer > lbl_8087F0A8->m5A4) {
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
        SetParam_unk(mCursor->mLayout, "cursor_pos", 0, -1000.0f);
        SetParam_unk(mCursor->mLayout, "cursor_pos", 1, -1000.0f);
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
                        ((GameSaveLoadCommon*)mSave)->fn_803B8758(0);
                        mLoading = 1;
                    } else {
                        fn_803B3270(mSave, 0);
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
            if (((GameSaveLoadCommon*)mSave)->fn_803B87A8() == 0) {
                if (mSave->mC84 == 7) {
                    done = 1;
                } else {
                    mLoading = 0;
                    fn_803B3270(mSave, 0);
                }
            }
        } else if (mSave != 0 && mSave->m14F60->m154 == 0) {
            if (mSave->m14F60->fn_803B8144()) {
                done = 1;
            } else {
                fn_803B2FBC(mSave, 0);
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
            CampSaveData_unk* save = mSave;
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
    for (int i = 0; i < 5; i++) {
        int id = sMsg[i];
        switch (mMode) {
        case 0:
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
        float len = fn_8006EF48(lbl_8087EEC8, text, 14.0f, 0.0f, 1, 1);
        char name[0x20];
        sprintf(name, "msgLen%02d", i);
        SetRate_unk(mMenu->mLayout, name, len);
        sprintf(name, "msg%02d", i);
        fn_801FEE08(mMenu->mLayout, fn_800DC6B4(name), text);
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
    char name[0x20];
    sprintf(name, "cursor_point%02d", pos);
    GameOverPoint_unk point = fn_801F4E8C(mMenu, fn_800DC6B4(name));
    SetParam_unk(mCursor->mLayout, "cursor_pos", 0, point.x);
    SetParam_unk(mCursor->mLayout, "cursor_pos", 1, point.y);
    SetParam_unk(mCursor->mLayout, "cursor_pos", 4, point.m10);
    SetRate_unk(mCursor->mLayout, "wRate", point.m08);
    SetRate_unk(mCursor->mLayout, "hRate", point.m0C);
    if (MenuCommonTask::sInstance != 0) {
        sprintf(name, "select_text%02d", pos);
        fn_804A5B9C(MenuCommonTask::sInstance, mMenu, name);
    }
    if (fade) {
        mFade = atn::FadeColor::Create(lbl_8087F430->m20, 30, 0, 0xFF000000);
        mFade->mWait = 2;
        mFade->mActive = 1;
        mFade->mDeleteAtEnd = 1;
    }
}

// NONMATCHING: 84.74% - the camera position and target go through local copies in the original, and the
// copy to lbl_8087EFB4 is scheduled differently
void GameOverTask::vf14() {
    if (mState == 1) {
        SetRate_unk(mMenu->mLayout, "selectOn", 1.0f);
    } else {
        SetRate_unk(mMenu->mLayout, "selectOn", 0.0f);
    }
    mCamera.mPos = atn::Vector3(0.0f, -99500.0f, 0.0f);
    mCamera.mTarget = atn::Vector3(0.0f, -99500.0f, 10.0f);
    mCamera.mFovy = 1.0471976f;
    mCamera.fn_8004B378();
    lbl_8087EFB4->mCamera = mCamera;
}
