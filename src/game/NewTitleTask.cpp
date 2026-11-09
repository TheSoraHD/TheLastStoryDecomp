// game/NewTitleTask.cpp (0x80531ED0-0x80537708): the title screen. The d2anime logos fade in over the title
// map, "Push A" opens the menu (new game, load, multiplayer) and the camera moves between the keys of the
// title_param csv.
#include "game/NewTitleTask.h"
#include "atn/Memory.h"
#include "atn/D2A_Task.h"
#include "atn/FadeCommon.h"
#include "atn/SoundRef.h"
#include "atn/SoundManager.h"
#include "atn/EffectManager.h"
#include "atn/LoadManager.h"
#include "atn/RenderManager.h"
#include "atn/Layer3D.h"
#include "atn/DebugMessages_unk.h"
#include "game/MapTask.h"
#include "game/MapManager.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/GameLoadTask.h"
#include "game/GameLoadBackground.h"
#include "game/GameSaveBackground.h"
#include "game/GameConfig_unk.h"
#include "game/GameSaveBanner.h"
#include "game/GameTaskStatic_unk.h"
#include "game/GameTask.h"
#include "atn/SequenceBoot_unk.h"
#include "atn/WPad_unk.h"
extern "C" int sprintf(char* buf, const char* format, ...);
#include <string.h>

extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" RenderSetting_unk* lbl_8087EFA8; // render settings
extern "C" void* lbl_8087EF70; // inferred: pad (atn/Pad_unk.cpp)
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" int fn_800A55D4(void* pad, int port, int button); // inferred: button held
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a SE
extern "C" int fn_800C310C(const char* name); // inferred: nonzero while the message group loads
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task
extern "C" void* lbl_8087EE90; // inferred: audio device
extern "C" int fn_8004829C(void* audio, const char** groups, unsigned int count); // inferred: loads sound groups
extern "C" void fn_8004895C(void* audio); // inferred: releases the sound groups
extern "C" int fn_803CE44C(const Unk_803CE3B8* h); // inferred: the header is out of range
extern "C" float fn_80076FF8(void* device); // inferred: screen aspect ratio
extern "C" void* lbl_8087EEE0; // inferred: graphics device
extern "C" void* fn_801FEB9C(void* layout, const char* name); // inferred: track of a pane value
extern "C" float fn_801F0544(void* key, int frame); // inferred: value of a key
// inferred: items of the debug menu (atn/MindowsCommon.h; the folder is a ResHandle here)
// fn_80087994 (AddBool) is declared in atn/Layer3D.h
extern "C" void fn_80087E9C(void* folder, const char* name, float* value, float min, float max, float step,
                            void* func, void* arg); // vector
extern "C" void fn_80087858(void* folder, const char* name, float* value, float min, float max, float step,
                            void* func, void* arg); // AddRadian
extern "C" void fn_800874C8(void* folder, const char* name, int* value, int min, int max, int step, void* func,
                            void* arg); // AddInt
extern "C" void fn_8008771C(void* folder, const char* name, float* value, float min, float max, float step,
                            void* func, void* arg); // AddFloat
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const atn::Color& color, int arg7,
                            int arg8, int arg9, float scale); // inferred: plays an efp effect

extern "C" WPad_unk* lbl_8087F018;
extern "C" int fn_803B8AC4(const char* path); // inferred: the save file exists

// inferred: error message manager (lbl_8087F420, game/SystemErrorMessage_unk.cpp); only the members used here
class ErrorManager {
public:
    void fn_80364E7C(int message); // inferred: shows a message with choices
    void fn_8036503C();            // inferred: moves the cursor of the choices
    void fn_80365320();            // inferred: closes the choices
    int mError;
    int mPrev;
    int mDelay;
    int m0C; // inferred: check the Wii Remote
    int m10;
    unsigned int mPaused;
    int m18;
    int m1C;
    unsigned char m20[0x40 - 0x20];
    int mSelect; // 0x40
};
extern "C" ErrorManager* lbl_8087F420;

// inferred: the menu of the save files (game/CampSaveLoad.cpp); only the members used here
class MenuLoadTask : public atn::Task {
public:
    static MenuLoadTask* fn_80117914(atn::Task* parent, GameLoadTask* data);
    static MenuLoadTask* sInstance;
    unsigned char m48[0x13C - 0x48];
    int m13C; // inferred: state (7: cancelled, 8: loaded)
};

// inferred: commands of the NAND manager (atn/NandManager.cpp; lbl_8087EF68 is declared in
// game/GameSaveLoadBackgroundCommon.h)
extern "C" int fn_800A45D0(SaveLoadMgr_unk* nand, const char* path, int* err, int file, int home); // inferred: exists
extern "C" int fn_800A472C(SaveLoadMgr_unk* nand, unsigned long* length, int* err);   // inferred: file length
extern "C" int fn_800A4794(SaveLoadMgr_unk* nand, const char* path, int size, int* err); // inferred: creates
extern "C" int fn_800A4AD4(SaveLoadMgr_unk* nand, int size, int inodes, int* err);     // inferred: space check
extern "C" int fn_800A4BE4(SaveLoadMgr_unk* nand, const char* path, int* err);         // inferred: opens
extern "C" int fn_800A4E08(SaveLoadMgr_unk* nand, int* err);                           // inferred: closes
extern "C" int fn_800A4F0C(SaveLoadMgr_unk* nand, const char* path, int* err);         // inferred: deletes
extern "C" void fn_803B854C(char* name, int* size); // inferred: name and size of the save file
extern "C" void fn_803B84DC(char* name);            // inferred: name of the save directory
extern "C" int fn_800CB538(atn::SoundRef* sound);   // inferred: the sound is playing
extern "C" void fn_8006BA74(int lang);              // inferred: sets the text language
extern "C" void fn_805715C4(void* system, int wait); // inferred: returns to the Wii menu (game/GameSystem_unk.cpp)

// inferred: languages of the second menu (0x80793940)
struct TitleLang_unk {
    const char* mName;
    int mLang;
};
TitleLang_unk lbl_80793940[2] = {{"Japanese", 0}, {"English", 1}};

// inferred: SE of the menu (SE_UICOM_001 move? 002, 003 cursor, 018)
static const char* const sSeNames_unk[4] = {"SE_UICOM_001", "SE_UICOM_002", "SE_UICOM_003", "SE_UICOM_018"};

// inferred: value of a pane track at frame 0 (as D2A_Task::GetTrack_unk of game/GameUI.cpp)
inline float GetTrackValue_unk(atn::D2A_Task* layout, const char* name) {
    return fn_801F0544((char*)fn_801FEB9C(&layout->mData.m10, name) + 4, 0);
}

NewTitleTask* fn_80531ED0(atn::Task* parent, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewTitleTask(parent, arg);
    }
    return 0;
}

// NONMATCHING: first version
NewTitleTask::NewTitleTask(atn::Task* parent, int index)
    : atn::Task(parent), mMenu(0), mCamera(0, 0), mMap(0), mIndex(index), mBgmStarted(0), mStep(0), mState(0),
      mPrevState(0), mCursorPos(0), m2BC(0), mNoSave(1), mFade(0),
      mFreeCamera(0), mCameraPos(13.7977f, 19.7862f, 33.8753f), mCameraTarget(-73.4619f, 21.8608f, -115.7171f),
      m334(-17.5f, -3.0f, -47.0f), mCameraAngle(1.0471976f), mAspectOfs(0.22f), mLoadBg(0), mSaveBg(0), mIdle(0),
      mIdleOn(0) {
    mGimmicks.clear();
    mAreaIndex = 0;
    m2E0 = 0;
    mAreas.clear();
    mFade = atn::FadeColor::Create(parent, 30, 0xFF000000, 0);
    mFade->mDelay = 0;
    mFade->mDuration = 1;
    mFade->mWait = 999999999;
    mFade->mColor0 = 0xFF000000;
    mFade->mColor1 = 0xFF000000;
    mFade->mZ = -100.0f;
    mFade->mActive = 1;
    mCapture = atn::FadeCapture::Create(this, 10, -1, 0xFFFFFF, 1);
    mCapture->fn_800D246C(1);
    mCapture->m78 = 0x12;
    lbl_8087EFE8->fn_800CDF84();
    const char* groups[1] = {"GROUP_SYSTEM"};
    fn_8004829C(lbl_8087EE90, groups, 1);
    atn::EffectManager::fn_80239798(parent, 0x240, 0x480, 8, 0x180);
    MapManager::SetTask(this);
    GimmickManager::SetTask(this);
    RenderSetting_unk* rs = lbl_8087EFA8;
    MotionBlurParam_unk blur = rs->m240;
    blur.mEnabled = 0;
    blur.mEndBlend = 1;
    blur.mEndBlendSpeed = 0.02f;
    rs->m240 = blur;
    mDamage0 = lbl_8087EFA8->m374;
    mDamage0.mColor = RsColor_unk(0.0f, 0.0f, 0.0f, 1.0f);
    mDamage0.mEnabled = 1;
    mDamage0.mZoom = -0.55f;
    mDamage1 = mDamage0;
    mDamage = &mDamage0;
    if (lbl_8087F0A8->m15C != 0) {
        mStopTime = 60;
    } else {
        mStopTime = 0;
    }
    memset(mHeader, 0, sizeof(mHeader));
    mLoadBg = GameLoadBackground::fn_803B33F8(this);
    char name[0x110];
    int size;
    GameSaveLoadBackgroundCommon::fn_803B83A4(name, &size, -1);
    if (fn_803B8AC4(name) != 0) {
        mLoadBg->fn_803B3530(name, (int)mHeader, 0x20, size);
    }
    mKeys[0].mPos = atn::Vector3(0.0f, -99500.0f, 0.0f);
    mKeys[0].mTgt = atn::Vector3(0.0f, -99500.0f, 10.0f);
    mKeys[0].mAngle = 1.0471976f;
    mKeys[0].mRate = 1.0f;
    if (lbl_8087F420 != 0) {
        lbl_8087F420->m0C = 1;
    }
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0[0] = 1;
    }
}

// inferred: power/reset state of the game (lbl_8087F9A0, game/GameSystem_unk.cpp); only the member used here
struct TitleSystem_unk {
    unsigned char m00[0x24];
    int mLock; // 0x24
};
extern "C" TitleSystem_unk* lbl_8087F9A0;

// inferred: as in game/GameTask.cpp
inline void RenderSetting_unk::SetDamage_unk(DamageEffectParam_unk p) {
    m374 = p;
}

// NONMATCHING: first version
NewTitleTask::~NewTitleTask() {
    if (mState != 25) {
        fn_800D2338(mFade);
    }
    RenderSetting_unk* rs = lbl_8087EFA8;
    MotionBlurParam_unk blur = rs->m240;
    blur.mEnabled = 0;
    blur.mEndBlend = 0;
    rs->m240 = blur;
    DamageEffectParam_unk damage = lbl_8087EFA8->m374;
    damage.mEnabled = 0;
    lbl_8087EFA8->SetDamage_unk(damage);
    fn_8004895C(lbl_8087EE90);
    lbl_8087EFE8->fn_800CE368();
    if (lbl_8087F3C0 != 0) {
        fn_800D2338(lbl_8087F3C0);
    }
    if (lbl_8087F420 != 0) {
        lbl_8087F420->m0C = 0;
    }
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0[0] = 0;
    }
    if (lbl_8087F9A0 != 0 && lbl_8087F9A0->mLock != 0) {
        lbl_8087F9A0->mLock = 0;
    }
}

// inferred: moves the cursor over `count` items with the pad (axis 0: up/down, 1: left/right); nonzero when
// the cursor moved
// NONMATCHING: first version
int NewTitleTask::fn_80533A28(int count, int axis) {
    int move = 0;
    if (axis == 0) {
        if (fn_800A55AC(lbl_8087EF70, 0, 0) != 0 || fn_800A55AC(lbl_8087EF70, 0, 0x1A) != 0) {
            move = -1;
        } else if (fn_800A55AC(lbl_8087EF70, 0, 1) != 0 || fn_800A55AC(lbl_8087EF70, 0, 0x19) != 0) {
            move = 1;
        }
    } else if (axis == 1) {
        if (fn_800A55AC(lbl_8087EF70, 0, 2) != 0 || fn_800A55AC(lbl_8087EF70, 0, 0x17) != 0) {
            move = -1;
        } else if (fn_800A55AC(lbl_8087EF70, 0, 3) != 0 || fn_800A55AC(lbl_8087EF70, 0, 0x18) != 0) {
            move = 1;
        }
    } else {
        return 0;
    }
    int prev = mCursorPos;
    if (move != 0) {
        if ((mCursorPos = prev + move) < 0) {
            mCursorPos += count;
        }
        if (mCursorPos >= count) {
            mCursorPos -= count;
        }
        fn_800C31F4(sSeNames_unk[2], 0, -1, 1.0f);
    }
    return prev != mCursorPos;
}

// inferred: sets the state (the camera key, the damage effect and the layouts of the state)
// NONMATCHING: first version
void NewTitleTask::fn_80533BDC(int state) {
    mPrevState = mState;
    mState = state;
    switch (state) {
    case 1:
        mKey = &mKeys[0];
        fn_80232B7C((unsigned int)this, 0);
        {
            atn::Vector3 pos(0.0f, 0.0f, 0.0f);
            atn::Vector3 rot(0.0f, 0.0f, 0.0f);
            atn::Color color(1.0f, 1.0f, 1.0f, 1.0f);
            fn_8023A8B4(lbl_8087F3C0, &mEfp0, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
        }
        mFrame = 0;
        mLogo->mFrame = 0.0f;
        mLogoNm->mFrame = 0.0f;
        break;
    case 2:
        mDamage = &mDamage1;
        mCapture->fn_800D246C(0);
        mCapture->mDeleteAtEnd = 1;
        mCapture->mWait = 0;
        mCapture->mDelay = 0;
        mCapture->mColor0 = 0xFFFFFFFF;
        mCapture->mColor1 = 0xFFFFFF;
        mCapture->m78 = 0x12;
        mCapture->mDuration = 60;
        mCapture->mFrame = 0;
        mCapture->mZ = -100.0f;
        mCapture->Capture();
        mFrame = 0;
        if (mBgm.mSound.mPtr != 0) {
            mBgm.fn_8004B1EC(90);
        }
        if (mBgm2.mSound.mPtr != 0) {
            mBgm2.fn_8004B158(1.0f, 90);
        }
        break;
    case 3:
        mDamage = &mDamage1;
        if (mPrevState == 10) {
            mKey = &mKeys[2];
        } else {
            mKey = &mKeys[1];
        }
        mLogo->mFrame = 29.0f;
        mLogoNm->mFrame = 29.0f;
        mMenu->mFrame = 0.0f;
        mMenu->mSpeed = 1.0f;
        mCursor->mFrame = 0.0f;
        mCursor->mSpeed = 0.0f;
        mFrame = 0;
        for (unsigned int i = 0; i < mGimmicks.size(); i++) {
            TitleGimmick_unk& e = mGimmicks.begin()[i];
            GimmickCommon* g = GimmickManager::instance_->fn_803EEE10(e.mId, e.mIndex);
            if (g != 0) {
                g->vf70(e.mState);
            }
        }
        break;
    case 4:
        mCursor->mSpeed = 1.0f;
        mFrame = 0;
        break;
    case 9:
        mKey = &mKeys[3];
        mCursorPos = 0;
        m2BC = 0;
        for (unsigned int i = 0; i < mGimmicks.size(); i++) {
            TitleGimmick_unk& e = mGimmicks.begin()[i];
            GimmickCommon* g = GimmickManager::instance_->fn_803EEE10(e.mId, e.mIndex);
            if (g != 0 && e.mMsg != 0) {
                GimmickMsg_unk msg(e.mMsg);
                g->vf40(&msg);
            }
        }
        break;
    case 10:
        mKey = &mKeys[2];
        mCursorPos = 0;
        m2BC = 0;
        mCursor->mSpeed = 0.0f;
        mMenu->mSpeed = 0.0f;
        mLogoCl->mFrame = mLogoCl->mData.mEndFrame;
        mMenu->mSpeed = -2.0f;
        mCursor->mSpeed = -2.0f;
        break;
    case 11:
        mKey = &mKeys[2];
        if (mPrevState == 13 || mPrevState == 14) {
            mCursorPos = 0;
            m2BC = 0;
        }
        mMenu->mSpeed = -2.0f;
        mCursor->mFrame = 5.0f;
        mCursor->mSpeed = 0.0f;
        break;
    case 12:
        mCursor->mSpeed = 1.0f;
        mMenu->mSpeed = 1.0f;
        mKey = &mKeys[4];
        break;
    case 13:
    case 14:
        mCursorPos = 0;
        break;
    case 15:
        if (MenuLoadTask::sInstance == 0) {
            MenuLoadTask::fn_80117914(this, mLoadTask);
        }
        break;
    case 25:
        if (mBgm2.mSound.mPtr != 0) {
            mBgm2.fn_8004B1EC(90);
        }
        mFade->fn_800D246C(0);
        mFade->mDeleteAtEnd = 1;
        mFade->mDelay = 0;
        mFade->mDuration = 60;
        mFade->mWait = 0x23;
        mFade->mColor0 = 0;
        mFade->mColor1 = 0xFF000000;
        mFade->mZ = -100.0f;
        mFade->mFrame = 0;
        mFade->mActive = 1;
        break;
    }
}

// inferred: loading: the save header, the preload archive, the logos and the csv, then the maps, then the
// render state of the map
// NONMATCHING: first version
int NewTitleTask::vf0C() {
    fn_8046ECDC(lbl_8087F518, 0);
    if (mStep == 0) {
        if (mLoadBg->m154 != 0) {
            return 0;
        }
        mStep++;
        mLoadTask = GameLoadTask::fn_803B27F8(this, GameLoadTask::Option("", 1));
        mLoadTask->mD64 = 0;
        Unk_803CE3B8* header = (Unk_803CE3B8*)mHeader;
        header->fn_803CE448();
        if (fn_803CE44C(header) == 0 && lbl_8087F0A8 != 0) {
            lbl_8087F0A8->m1140 = header->m0A & 1;
            lbl_8087F0A8->m113C = header->m08;
        }
        if (mIndex == -1) {
            int progress = fn_803CE44C(header) == 0 ? header->m00 : 0;
            if (progress < 401000) {
                if (progress < 201000) {
                    if (progress < 106000) {
                        mIndex = 0;
                    } else {
                        mIndex = 3;
                    }
                } else if (progress < 301000) {
                    mIndex = 4;
                } else {
                    mIndex = 5;
                }
            } else if (progress < 501000) {
                if (progress < 409000) {
                    mIndex = 6;
                } else {
                    mIndex = 7;
                }
            } else if (progress < 600000) {
                mIndex = 8;
            } else {
                mIndex = 2;
            }
        }
        if (lbl_8087F0A8->m194 != 0) {
            mIndex = 0;
        }
        char path[0x100];
        sprintf(path, "preload/title%02d.pk", mIndex);
        if (lbl_8087F420 != 0) {
            lbl_8087F420->m18 = 1;
            lbl_8087F420->m1C = 1;
        }
        mPreload.fn_8047043C(path, 0, 0);
        if (lbl_8087F420 != 0) {
            lbl_8087F420->m18 = 0;
            lbl_8087F420->m1C = 0;
        }
        mEfp0.LoadRequest("be010_006.efp");
        mEfp1.LoadRequest("be010_005.efp");
        mLogo = fn_801F3FF8(this, "/data/d2anime/title_logo_laststory.d2a", 0);
        mLogo->fn_800D246C(1);
        mLogoCl = fn_801F3FF8(this, "/data/d2anime/title_logo_laststory_cl.d2a", 0);
        mLogoCl->fn_800D246C(1);
        mLogoNm = fn_801F3FF8(this, "/data/d2anime/title_logo_nm.d2a", 0);
        mLogoNm->fn_800D246C(1);
        mPushOp = fn_801F3FF8(this, "/data/d2anime/title_logo_laststory_push_op.d2a", 0);
        mPushOp->fn_800D246C(1);
        mPushLoop = fn_801F3FF8(this, "/data/d2anime/title_logo_laststory_push_loop.d2a", 0);
        mPushLoop->fn_800D246C(1);
        mCursor = fn_801F3FF8(this, "/data/d2anime/title_cursor.d2a", 0);
        mCursor->fn_800D246C(1);
        char csv[0x100];
        sprintf(csv, "database/title/title_param%02d.csv", mIndex);
        mParamFile.vf0C(csv);
    } else if (mStep == 1) {
        if (fn_800D3FA4() == 0 || fn_800C310C(0) != 0 || mParamFile.fn_80473F50() != 0 || mEfp0.IsLoading() != 0 ||
            mEfp1.IsLoading() != 0) {
            return 0;
        }
        mStep++;
        fn_80536CA8();
        mBgm.fn_8004AE84("BGM_SYST001", 0);
        mMap = MapTask::SetTask(this, mMapPath);
        mMap->fn_800D246C(1);
        mMenu = fn_801F3FF8(this, mUiPath, 0);
        mMenu->fn_800D246C(1);
        mKeys[1].mPos = mStart.mPos;
        mKeys[1].mTgt = mStart.mTgt;
        mKeys[1].mAngle = mStart.mAngle;
        mKeys[1].mRate = 1.0f;
        mKeys[2] = mStart;
        mKeys[3] = mEnd;
        mKeys[4] = mLoad;
        mKey = &mKeys[1];
    } else if (mStep == 2) {
        int ready = 1;
        if (fn_800D3FA4() == 0 || mBgm.fn_8004B0E4() != 0 || mBgm2.fn_8004B0E4() != 0 || !mMap->IsFlag_unk(2)) {
            ready = 0;
        }
        if (ready == 0) {
            return 0;
        }
        mMap->fn_800D246C(0);
        mMap->FreezeTask();
        atn::Scene* scene = atn::RenderManager::instance_->fn_800C16B4();
        mView.mScene = *scene;
        mView.mShadow = lbl_8087EFA8->m104;
        mView.mClearColor = lbl_8087EFA8->m03C;
        mView.mBloom = lbl_8087EFA8->m054;
        mView.mDof = lbl_8087EFA8->mDof;
        mView.mColorMatrix = lbl_8087EFA8->m324;
        mView.mGamma = lbl_8087EFA8->m3DC;
        atn::IRenderLayer* reflect = atn::RenderManager::instance_->mLayers[4];
        if (reflect != 0) {
            mView.m4BC = reflect->m04;
        } else {
            mView.m4BC = 0;
        }
        mStep = 3;
        mWhiteMap = MapTask::SetTask(this, "titlle_white.map");
        mWhiteMap->fn_800D246C(1);
    } else if (mStep == 3) {
        if (!mWhiteMap->IsFlag_unk(2)) {
            return 0;
        }
        mWhiteMap->fn_800D246C(0);
        mPreload.fn_80470528();
        fn_80533BDC(1);
        mFirst = GetTrackValue_unk(mMenu, "first");
        mSecond = GetTrackValue_unk(mMenu, "second");
        mOpSelect = GetTrackValue_unk(mMenu, "opSelect");
        mOpCursor = GetTrackValue_unk(mMenu, "opCursor");
        mInFrame = GetTrackValue_unk(mCursor, "inFrame");
        mSelectFrame = GetTrackValue_unk(mCursor, "selectFrame");
        mMenu->mRepeat = 0;
        mMenu->fn_800D246C(0);
        mCursor->mRepeat = 0;
        mCursor->fn_800D246C(0);
        mLogo->mRepeat = 0;
        mLogo->fn_800D246C(0);
        mLogoCl->mRepeat = 0;
        mLogoCl->mSpeed = 0.0f;
        mLogoCl->fn_800D246C(0);
        mLogoNm->mRepeat = 0;
        mLogoNm->fn_800D246C(0);
        mPushOp->mRepeat = 0;
        mPushOp->fn_800D246C(0);
        mPushLoop->fn_800D246C(0);
        mFade->mKillAtEnd = 1;
        mFade->mDelay = 0;
        mFade->mDuration = lbl_8087F0A8->mTitle.mMaskFadeTime;
        mFade->mWait = 0;
        mFade->mColor0 = 0xFF000000;
        mFade->mColor1 = 0;
        mFade->mZ = -100.0f;
        mFade->mFrame = 0;
        mFade->mActive = 1;
        mCamera.mPos = mKeys[1].mPos;
        mCamera.mTarget = mKeys[1].mTgt;
        mCamera.mUp = atn::Vector3(0.0f, 1.0f, 0.0f);
        mCamera.mFovy = mKeys[1].mAngle;
        mCamera.mAspect = fn_80076FF8(lbl_8087EEE0);
        void* menu = mDebugMenu.Load("NewTitle");
        fn_80087994(menu, "FreeCamera", &mFreeCamera, 0, 0);
        fn_80087E9C(menu, "CameraPos", &mCameraPos.x, -1e16f, 1e16f, 1.0f, 0, 0);
        fn_80087E9C(menu, "CameraTarget", &mCameraTarget.x, -1e16f, 1e16f, 1.0f, 0, 0);
        fn_80087858(menu, "CameraAngle", &mCameraAngle, 0.0f, 1.5707964f, 1.0f, 0, 0);
        fn_800874C8(menu, "StopTime", &mStopTime, 0, 10000, 1, 0, 0);
        fn_8008771C(menu, "Damage Zoom", &mDamage1.mZoom, -1000.0f, 1000.0f, 0.1f, 0, 0);
        fn_8008771C(menu, "AspectOfs", &mAspectOfs, 0.0f, 100.0f, 0.01f, 0, 0);
        for (unsigned int i = 0; i < mGimmicks.size(); i++) {
            TitleGimmick_unk& e = mGimmicks.begin()[i];
            GimmickCommon* g = GimmickManager::instance_->fn_803EEE10(e.mId, e.mIndex);
            if (g != 0) {
                g->vf70(e.mState);
            } else if (lbl_8087EEB8 != 0) {
                char msg[0x100];
                sprintf(msg, "error: not found change Gimmick[gm%d_%d_%d]\n", e.mId / 1000, e.mId % 1000, e.mIndex);
                lbl_8087EEB8->fn_800697D8(msg);
            }
        }
        mNoSave = 1;
        for (int i = 0; i < 17; i++) {
            if (mLoadTask->mSlots[i].fn_803BEAB4() != 0) {
                mNoSave = 0;
                break;
            }
        }
        if (mLoadBg->fn_803B8144() != 0) {
            fn_800D2338(mLoadBg);
            mLoadBg = 0;
        }
        if (mNoSave != 0) {
            fn_800D2338(mLoadTask);
            mLoadTask = 0;
        }
        return 1;
    }
    return 0;
}

// inferred: plays a SE of the menu
inline void PlaySe_unk(int index) {
    fn_800C31F4(sSeNames_unk[index], 0, -1, 1.0f);
}

// inferred: the pad button was triggered (4: A, 5: B)
inline int Trig_unk(int button) {
    return fn_800A555C(lbl_8087EF70, 0, button);
}

// inferred: update: the state machine of the title, the "Push A" layouts and the camera
// NONMATCHING: first version
void NewTitleTask::vf10() {
    GameConfig_unk* cfg = lbl_8087F0A8;
    mLogo->FreezeTask();
    mLogoCl->FreezeTask();
    mLogoNm->FreezeTask();
    mMenu->FreezeTask();
    mCursor->FreezeTask();
    if (lbl_8087F0A8->m194 != 0) {
        if (mIdleOn != 0) {
            if (--mIdle <= 0) {
                mIdle = 0;
                mIdleOn = 0;
            }
        } else if (fn_800A55D4(lbl_8087EF70, 0, 0xE) != 0 && fn_800A55D4(lbl_8087EF70, 0, 0xC) != 0 &&
                   fn_800A55D4(lbl_8087EF70, 0, 0xD) != 0) {
            if (++mIdle > 30) {
                mIdleOn = 1;
            }
        } else {
            mIdle = 0;
        }
    }
    int push = 0;
    switch (mState) {
    case 1:
        if (Trig_unk(4) != 0) {
            if (mLogo->IsEnd_unk()) {
                fn_80533BDC(2);
                PlaySe_unk(0);
            } else {
                mFrame = cfg->mTitle.mPushAFadeStart;
                mLogo->SetEnd_unk();
                mLogoNm->mFrame = 29.0f;
                mFade->mFrame = mFade->mDelay + (mFade->mWait + mFade->mDuration);
            }
        }
        if (mBgmStarted == 0 && mFrame >= cfg->mTitle.mBgmStart && mBgm.mSound.mPtr != 0) {
            mBgm.fn_8004B158(1.0f, cfg->mTitle.mBgmFadeTime);
            mBgmStarted = 1;
        }
        if (mFrame >= cfg->mTitle.mTitleFadeStart) {
            mLogo->MeltTask();
            mLogo->mSpeed = 1.0f;
        }
        if (mFrame >= cfg->mTitle.mCompanyFadeStart) {
            mLogoNm->MeltTask();
            mLogoNm->mSpeed = 1.0f;
            if (mLogoNm->mFrame >= 29.0f) {
                mLogoNm->mFrame = 29.0f;
            }
        }
        if (mFrame >= cfg->mTitle.mPushAFadeStart) {
            push = 1;
        }
        mFrame++;
        break;
    case 2:
        if (mFrame == 1) {
            mLogo->MeltTask();
            mLogoNm->MeltTask();
            mKey = &mKeys[1];
            mCapture->mActive = 1;
            mWhiteMap->FreezeTask();
            mMap->MeltTask();
            *atn::RenderManager::instance_->fn_800C16B4() = mView.mScene;
            lbl_8087EFA8->m03C = mView.mClearColor;
            lbl_8087EFA8->mDof = mView.mDof;
            lbl_8087EFA8->m054 = mView.mBloom;
            lbl_8087EFA8->m104 = mView.mShadow;
            lbl_8087EFA8->m324 = mView.mColorMatrix;
            lbl_8087EFA8->m3DC = mView.mGamma;
            if (atn::RenderManager::instance_->mLayers[4] != 0) {
                atn::RenderManager::instance_->mLayers[4]->m04 = mView.m4BC;
            }
            fn_80239DAC(lbl_8087F3C0, (unsigned int)this, 0, 0);
            fn_80232B7C((unsigned int)this, 1);
            atn::Vector3 pos(0.0f, 0.0f, 0.0f);
            atn::Vector3 rot(0.0f, 0.0f, 0.0f);
            atn::Color color(1.0f, 1.0f, 1.0f, 1.0f);
            fn_8023A8B4(lbl_8087F3C0, &mEfp1, 0, 0, pos, rot, color, -1, -1, 1, 1.0f);
        } else {
            mLogoCl->MeltTask();
            mLogoCl->mSpeed = 0.0f;
            mLogoCl->mFrame = 0.0f;
            int next = 0;
            if (mFrame >= 60) {
                next = 1;
            }
            if (Trig_unk(4) != 0) {
                mCapture->mFrame = mCapture->mDelay + (mCapture->mWait + mCapture->mDuration);
                next = 1;
            }
            if (next != 0) {
                fn_80533BDC(3);
                if (mAreas.size() != 0) {
                    mMap->fn_804A0F58(mAreas.begin()[0].mArea, 0.0f, mAreas.begin()[0].mTime);
                    if (mAreas.size() > 1) {
                        mAreaIndex = 1;
                    }
                }
                return;
            }
        }
        mFrame++;
        break;
    case 3:
        mLogoCl->MeltTask();
        mLogoCl->mSpeed = 0.0f;
        mLogoCl->mFrame = 0.0f;
        if (mFrame < mStopTime) {
            mFrame++;
        } else if (mFade->IsEnd()) {
            fn_80533BDC(4);
        }
        break;
    case 4:
        mLogoCl->MeltTask();
        mLogoCl->mSpeed = 0.0f;
        mLogoCl->mFrame = 0.0f;
        if (Trig_unk(4) != 0) {
            if (mNoSave == 0 && fn_803CE44C((Unk_803CE3B8*)mHeader) != 0) {
                fn_80533BDC(6);
                if (lbl_8087F420 != 0) {
                    lbl_8087F420->fn_80364E7C(12);
                }
            } else {
                char name[0x100];
                int size;
                fn_803B854C(name, &size);
                int err;
                fn_800A4BE4(lbl_8087EF68, name, &err);
                if (err != 0) {
                    fn_80533BDC(6);
                    if (lbl_8087F420 != 0) {
                        lbl_8087F420->fn_80364E7C(12);
                    }
                } else {
                    fn_80533BDC(5);
                }
            }
            PlaySe_unk(0);
        }
        if ((float)mFrame >= 300.0f) {
            push = 1;
        }
        mFrame++;
        break;
    case 5: {
        mLogoCl->MeltTask();
        mLogoCl->mSpeed = 0.0f;
        mLogoCl->mFrame = 0.0f;
        int err;
        if (fn_800A4228(lbl_8087EF68, &err) == 0) {
            if (err == 0) {
                unsigned long length;
                if (fn_800A472C(lbl_8087EF68, &length, &err) == 0) {
                    fn_80533BDC(6);
                    if (lbl_8087F420 != 0) {
                        lbl_8087F420->fn_80364E7C(12);
                    }
                } else {
                    fn_800A4E08(lbl_8087EF68, &err);
                    char name[0x100];
                    int size;
                    fn_803B854C(name, &size);
                    if (length != size) {
                        fn_80533BDC(6);
                        if (lbl_8087F420 != 0) {
                            lbl_8087F420->fn_80364E7C(12);
                        }
                    } else {
                        fn_80533BDC(9);
                    }
                }
            } else if (err == 5 || err == 14) {
                fn_80533BDC(6);
                if (lbl_8087F420 != 0) {
                    lbl_8087F420->fn_80364E7C(err + 7);
                }
            } else {
                fn_80533BDC(9);
            }
        }
    }
    case 6:
        mLogoCl->MeltTask();
        mLogoCl->mSpeed = 0.0f;
        mLogoCl->mFrame = 0.0f;
        if (lbl_8087F420 != 0) {
            lbl_8087F420->fn_8036503C();
        }
        if (Trig_unk(4) != 0) {
            if (lbl_8087F420 != 0) {
                lbl_8087F420->fn_80365320();
            }
            if (lbl_8087F420->mSelect == 0) {
                char name[0x100];
                int size;
                fn_803B854C(name, &size);
                int err;
                fn_800A4F0C(lbl_8087EF68, name, &err);
                fn_80533BDC(7);
                PlaySe_unk(0);
            } else {
                fn_80533BDC(4);
                PlaySe_unk(1);
            }
        } else if (Trig_unk(5) != 0) {
            if (lbl_8087F420 != 0) {
                lbl_8087F420->fn_80365320();
            }
            fn_80533BDC(4);
            PlaySe_unk(1);
        }
        break;
    case 7: {
        mLogoCl->MeltTask();
        mLogoCl->mSpeed = 0.0f;
        mLogoCl->mFrame = 0.0f;
        int err;
        if (fn_800A4228(lbl_8087EF68, &err) == 0) {
            if (err == 0) {
                char dir[0x100];
                fn_803B84DC(dir);
                fn_800A4F0C(lbl_8087EF68, dir, &err);
                fn_80533BDC(8);
            } else {
                mNoSave = 1;
                fn_80533BDC(9);
            }
        }
        break;
    }
    case 8: {
        mLogoCl->MeltTask();
        mLogoCl->mSpeed = 0.0f;
        mLogoCl->mFrame = 0.0f;
        int err;
        if (fn_800A4228(lbl_8087EF68, &err) == 0) {
            memset(mHeader, 0, sizeof(mHeader));
            if (lbl_8087F0A8 != 0) {
                lbl_8087F0A8->m1140 = 0;
                lbl_8087F0A8->m113C = 0;
            }
            if (mLoadBg != 0) {
                fn_800D2338(mLoadBg);
                mLoadBg = 0;
            }
            if (mLoadTask != 0) {
                fn_800D2338(mLoadTask);
                mLoadTask = 0;
            }
            mNoSave = 1;
            fn_80533BDC(9);
        }
        break;
    }
    case 9: {
        mLogoCl->MeltTask();
        mLogoCl->mSpeed = 1.0f;
        if (mLogoCl->mFrame >= 5.0f) {
            mMenu->MeltTask();
        }
        if (mMenu->mFrame >= 70.0f) {
            mMenu->mFrame = 70.0f;
            mMenu->mSpeed = 0.0f;
        }
        int next = 0;
        if (mMenu->mFrame >= 70.0f) {
            mCursor->MeltTask();
            if (mCursor->mFrame >= 5.0f) {
                next = 1;
            }
        }
        if (Trig_unk(4) != 0) {
            next = 1;
        }
        if (next != 0) {
            mCamera.mPos = mKey->mPos;
            mCamera.mTarget = mKey->mTgt;
            mCamera.mFovy = mKey->mAngle;
            fn_80533BDC(11);
        }
        break;
    }
    case 10:
        mMenu->MeltTask();
        mCursor->MeltTask();
        if (mCursor->mFrame <= 0.0f) {
            mCursor->mSpeed = 0.0f;
        }
        if (mMenu->mFrame <= 0.0f) {
            mMenu->mFrame = 0.0f;
            mMenu->mSpeed = 0.0f;
        }
        if (mMenu->mFrame <= 20.0f) {
            mLogoCl->MeltTask();
            mLogoCl->mSpeed = -2.0f;
        }
        if (mLogoCl->mFrame <= 0.0f) {
            fn_80533BDC(3);
        } else if (Trig_unk(4) != 0) {
            mCamera.mPos = mKey->mPos;
            mCamera.mTarget = mKey->mTgt;
            mCamera.mFovy = mKey->mAngle;
            fn_80533BDC(3);
        }
        break;
    case 11: {
        static const int sItems3[3] = {0, 1, 2};
        static const int sItems2[2] = {0, 1};
        static const int sItems1[1] = {1};
        mMenu->MeltTask();
        mCursor->MeltTask();
        if (mMenu->mFrame <= 70.0f) {
            mMenu->mFrame = 70.0f;
            mMenu->mSpeed = 0.0f;
        }
        int count = 3;
        if (mNoSave != 0) {
            count = 1;
        }
        const int* items = sItems3;
        if (mNoSave != 0) {
            items = sItems1;
        }
        if (mNoSave == 0 && lbl_8087F0A8->m194 != 0 && lbl_8087F0A8->m198 == 0) {
            count = 2;
            items = sItems2;
        }
        if (fn_80533A28(count, 1) == 0) {
            if (Trig_unk(4) != 0) {
                switch (items[m2BC]) {
                case 0:
                    m2B4 = 1;
                    break;
                case 1:
                    m2B4 = 0;
                    break;
                case 2:
                    m2B4 = 2;
                    break;
                }
                fn_80533BDC(12);
                PlaySe_unk(0);
            } else if (Trig_unk(5) != 0) {
                fn_80533BDC(10);
                PlaySe_unk(1);
            }
        } else {
            m2BC = mCursorPos;
        }
        break;
    }
    case 12:
        mMenu->MeltTask();
        mCursor->MeltTask();
        if (mMenu->IsEnd_unk() && mCursor->IsEnd_unk()) {
            switch (m2B4) {
            case 0: {
                char name[0x100];
                int size;
                fn_803B854C(name, &size);
                int found = 1;
                if (lbl_8087EF68 != 0) {
                    int err;
                    found = fn_800A45D0(lbl_8087EF68, name, &err, 1, 0);
                }
                if (mNoSave != 0 && found == 0) {
                    fn_80533BDC(16);
                } else {
                    fn_80533BDC(25);
                }
                break;
            }
            case 1:
                fn_80533BDC(15);
                break;
            case 2:
                fn_80533BDC(15);
                break;
            }
        }
        break;
    case 14:
        if (fn_80533A28(2, 0) == 0) {
            if (Trig_unk(4) != 0) {
                fn_8006BA74(lbl_80793940[mCursorPos].mLang);
                fn_80533BDC(25);
                PlaySe_unk(0);
            } else if (Trig_unk(5) != 0) {
                fn_80533BDC(11);
                PlaySe_unk(1);
            }
        }
        break;
    case 15:
        if (MenuLoadTask::sInstance != 0) {
            int state = MenuLoadTask::sInstance->m13C;
            if (state == 7) {
                fn_800D2338(MenuLoadTask::sInstance);
                fn_80533BDC(11);
            } else if (state == 8) {
                fn_800D2338(MenuLoadTask::sInstance);
                mSaveBg = GameSaveBackground::fn_803B8C6C(this);
                GameSaveBanner::fn_803B9044(this, mSaveBg);
                fn_80533BDC(21);
            }
        }
        break;
    case 16:
        if (lbl_8087EF68 != 0) {
            if (mSaveBg == 0) {
                mSaveBg = GameSaveBackground::fn_803B8C6C(this);
            }
            if (GameSaveBanner::sInstance == 0) {
                GameSaveBanner::fn_803B9044(this, mSaveBg);
            }
            fn_80533BDC(17);
        } else {
            fn_80533BDC(25);
        }
        break;
    case 17:
        if (fn_800D3FA4() != 0) {
            char name[0x100];
            int size;
            fn_803B854C(name, &size);
            size += GameSaveBanner::sInstance->fn_803B97C8();
            if (lbl_8087EF68 != 0) {
                lbl_8087EF68->m168 = 0;
                int err;
                fn_800A4AD4(lbl_8087EF68, size, 2, &err);
            }
            fn_80533BDC(18);
        }
        break;
    case 18: {
        int err;
        if (lbl_8087EF68 != 0 && fn_800A4228(lbl_8087EF68, &err) != 0) {
            break;
        }
        if (lbl_8087EF68 != 0) {
            if (err == 0) {
                fn_80533BDC(19);
            } else {
                if (lbl_8087F420 != 0) {
                    lbl_8087F420->fn_80364E7C(err + 7);
                }
                fn_80533BDC(23);
            }
        }
        break;
    }
    case 19: {
        int err;
        if (lbl_8087EF68 != 0 && fn_800A4228(lbl_8087EF68, &err) != 0) {
            break;
        }
        if (lbl_8087EF68 != 0) {
            if (err == 0) {
                char name[0x100];
                int size;
                fn_803B854C(name, &size);
                if (fn_800A45D0(lbl_8087EF68, name, &err, 1, 0) == 0) {
                    fn_800A4794(lbl_8087EF68, name, size, &err);
                }
                fn_80533BDC(20);
                if (lbl_8087F9A0 != 0) {
                    lbl_8087F9A0->mLock = 1;
                }
            } else {
                if (lbl_8087F420 != 0) {
                    lbl_8087F420->fn_80364E7C(err + 7);
                }
                fn_80533BDC(23);
            }
        } else {
            fn_80533BDC(20);
        }
        break;
    }
    case 20: {
        int err;
        if (lbl_8087EF68 != 0 && fn_800A4228(lbl_8087EF68, &err) != 0) {
            break;
        }
        if (lbl_8087EF68 != 0) {
            if (err == 0) {
                if (lbl_8087F0A8->m198 == 0) {
                    if (GameSaveBanner::sInstance != 0 && GameSaveBanner::sInstance->IsFlag_unk(2)) {
                        if (GameSaveBanner::sInstance != 0) {
                            GameSaveBanner::sInstance->fn_803B9638();
                        }
                        fn_80533BDC(22);
                    } else {
                        if (mSaveBg == 0) {
                            mSaveBg = GameSaveBackground::fn_803B8C6C(this);
                        }
                        if (GameSaveBanner::sInstance == 0) {
                            GameSaveBanner::fn_803B9044(this, mSaveBg);
                        }
                        fn_80533BDC(21);
                    }
                } else {
                    fn_80533BDC(25);
                }
            } else {
                if (lbl_8087F420 != 0) {
                    lbl_8087F420->fn_80364E7C(err + 7);
                }
                fn_80533BDC(23);
            }
        } else {
            fn_80533BDC(25);
        }
        break;
    }
    case 21:
        if (GameSaveBanner::sInstance == 0 || GameSaveBanner::sInstance->IsFlag_unk(2)) {
            if (GameSaveBanner::sInstance != 0) {
                GameSaveBanner::sInstance->fn_803B9638();
            }
            fn_80533BDC(22);
        }
        break;
    case 22: {
        int err;
        if (lbl_8087EF68 != 0 && fn_800A4228(lbl_8087EF68, &err) != 0) {
            break;
        }
        if (GameSaveBanner::sInstance != 0) {
            fn_800D2338(GameSaveBanner::sInstance);
        }
        if (lbl_8087EF68 != 0) {
            if (err == 0) {
                if (lbl_8087F9A0 != 0) {
                    lbl_8087F9A0->mLock = 0;
                }
                fn_80533BDC(25);
            } else {
                if (lbl_8087F420 != 0) {
                    lbl_8087F420->fn_80364E7C(err + 7);
                }
                fn_80533BDC(23);
            }
        } else {
            fn_80533BDC(25);
        }
        break;
    }
    case 23:
        if (lbl_8087F9A0 != 0 && lbl_8087F9A0->mLock != 0) {
            lbl_8087F9A0->mLock = 0;
        }
        if (Trig_unk(4) != 0 || Trig_unk(5) != 0) {
            if (lbl_8087F420 != 0) {
                lbl_8087F420->fn_80365320();
                lbl_8087F420->fn_80364E7C(30);
                PlaySe_unk(0);
            }
            fn_80533BDC(24);
        }
        break;
    case 24:
        if (lbl_8087F420 != 0) {
            lbl_8087F420->fn_8036503C();
        }
        if (Trig_unk(4) != 0 || Trig_unk(5) != 0) {
            if (lbl_8087F420 != 0) {
                lbl_8087F420->fn_80365320();
            }
            if (lbl_8087F420->mSelect == 0) {
                fn_805715C4(lbl_8087F9A0, 0);
                if (lbl_8087EFE8 != 0) {
                    lbl_8087EFE8->fn_800CF45C(25, 0);
                }
                PlaySe_unk(0);
            } else {
                fn_80533BDC(11);
                PlaySe_unk(1);
            }
        }
        break;
    case 25:
        if (lbl_8087F9A0 != 0 && lbl_8087F9A0->mLock != 0) {
            lbl_8087F9A0->mLock = 0;
        }
        if (mFade->mFrame >= 60 && fn_800CB538(&mBgm2.mSound) == 0) {
            mBgm.mSound.Reset();
            mBgm2.mSound.Reset();
            fn_800D2338(this);
            switch (m2B4) {
            case 0: {
                GameTaskStatic_unk param;
                param.m00 = 1;
                *(GameTask::Option*)&param.m04 = GameTask::Option(2, 4, 1, 101000, -1, 1, 0);
                param.m1C = 1;
                sGameTaskStatic_unk = param;
                sSequenceBoot_unk->SetNext_unk("Game");
                break;
            }
            case 1:
                sSequenceBoot_unk->SetNext_unk("Game");
                break;
            case 2:
                sSequenceBoot_unk->SetNext_unk("Multiplayer");
                break;
            }
        }
        break;
    }
    mPushOp->FreezeTask();
    mPushLoop->FreezeTask();
    ButtonUv_unk uv = lbl_8087F0A8->mInput.fn_80124C6C(0);
    mPushOp->SetParam2_unk("btnUv", &uv.x);
    ButtonUv_unk mask = lbl_8087F0A8->mInput.fn_80124C6C(0);
    mPushOp->SetParam2_unk("btnUvMask", &mask.x);
    ButtonUv_unk uv2 = lbl_8087F0A8->mInput.fn_80124C6C(0);
    mPushLoop->SetParam2_unk("btnUv", &uv2.x);
    ButtonUv_unk mask2 = lbl_8087F0A8->mInput.fn_80124C6C(0);
    mPushLoop->SetParam2_unk("btnUvMask", &mask2.x);
    if (push != 0) {
        if (mPushOp->IsEnd_unk()) {
            mPushLoop->MeltTask();
        } else {
            mPushOp->MeltTask();
        }
    } else {
        mPushOp->mFrame = 0.0f;
        mPushLoop->mFrame = 0.0f;
    }
    if (mFreeCamera != 0) {
        mCamera.mFovy = mCameraAngle;
        mCamera.fn_8004BF64(2.0f, 3.0f);
        mCameraPos = mCamera.mPos;
        mCameraTarget = mCamera.mTarget;
    } else {
        TitleCameraKey_unk* key = mKey;
        atn::Vector3 pos = mCamera.mPos;
        atn::Vector3 prev = mCamera.mPos;
        atn::Vector3 tgt = mCamera.mTarget;
        float rate = key->mRate;
        atn::Vector3 dpos((key->mPos.x - pos.x) * rate, (key->mPos.y - pos.y) * rate, (key->mPos.z - pos.z) * rate);
        atn::Vector3 dtgt((key->mTgt.x - tgt.x) * rate, (key->mTgt.y - tgt.y) * rate, (key->mTgt.z - tgt.z) * rate);
        atn::Vector3 newPos(dpos.x + pos.x, dpos.y + pos.y, dpos.z + pos.z);
        atn::Vector3 newTgt(dtgt.x + tgt.x, dtgt.y + tgt.y, dtgt.z + tgt.z);
        float fovy = mCamera.mFovy;
        pos = newPos;
        tgt = newTgt;
        mCamera.mPos = pos;
        mCamera.mTarget = tgt;
        mCamera.mFovy = rate * (key->mAngle - fovy) + fovy;
        MotionBlurParam_unk blur = lbl_8087EFA8->m240;
        atn::Vector3 move(prev.x - pos.x, prev.y - pos.y, prev.z - pos.z);
        if (PSVECSquareMag(&move) >= 0.01f || mState == 15 || mState == 25) {
            blur.mEnabled = 1;
        } else {
            blur.mEnabled = 0;
        }
        lbl_8087EFA8->m240 = blur;
    }
    mCamera.fn_8004B378();
    atn::RenderManager::instance_->SetCamera_unk(mCamera);
}
