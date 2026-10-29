#include "game/SetConfig.h"
#include "atn/Memory.h"
#include "atn/SoundManager.h"
#include "string.h"

extern "C" unsigned char SCGetSoundMode();
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4);

struct Unk_8087EFA8 {
    unsigned char m00[0x34];
    int m34;
};
extern "C" Unk_8087EFA8* lbl_8087EFA8;
struct Settings_unk {
    unsigned char m000[0x244];
    int m244;
};
extern "C" Settings_unk* lbl_8087F0A8;

SetConfig* SetConfig::sInstance;

SetConfig::~SetConfig() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

inline void SetConfig::Init_unk() {
    m24 = 0;
    mAttackType = 0;
    mDiveType = 0;
    mInTacviewType = 0;
    mGatheringType = 0;
    memset(m38, 0, sizeof(m38));
    mAttackTurnSpeed = 1.0f;
    mTurnSpeed = 1.0f;
    mRollbackSpeed = 0.8f;
    mCameraRotSpeed = 1.0f;
    mRotDelay = 31.0f;
    m50 = 16;
    mBattleDrawHateLine = 2;
    mDrawEnemyHP = 1;
    mDrawEnemyPopup = 1;
    mDrawFriendPopup = 1;
    memset(m68, 0, sizeof(m68));
    mShowDialog = 1;
    mShowTutorial = 1;
    mEventSkipType = 1;
    mEventCameraControl = 0;
    memset(m88, 0, sizeof(m88));
    mSoundType = 1;
    mBgmVolume = 1.0f;
    mSeVolume = 1.0f;
    mVoiceVolume = 1.0f;
    unsigned char mode = SCGetSoundMode();
    if (mode == 0) {
        mSoundType = 2;
    } else if (mode == 2) {
        mSoundType = 0;
    } else {
        mSoundType = 1;
    }
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->fn_800D0240(0, mBgmVolume, 0);
        lbl_8087EFE8->fn_800D0240(1, mSeVolume, 0);
        lbl_8087EFE8->fn_800D0240(2, mVoiceVolume, 0);
        switch (mSoundType) {
        case 0:
            lbl_8087EFE8->mOutputMode = 2;
            break;
        case 2:
            lbl_8087EFE8->mOutputMode = 3;
            break;
        default:
            lbl_8087EFE8->mOutputMode = 0;
            break;
        }
    }
    mA4 = 0;
    if (lbl_8087EFA8 != 0) {
        lbl_8087EFA8->m34 = mA4 + 10;
    }
}

inline SetConfig::SetConfig()
    : mSwingReplay(1), mPlayerHpBarShow(0), mAutoSave(1), mTacviewHint(1), mDrawHateLine(1), mCampVariation(0),
      mWindKey(1), mA4(0) {
    mShakeType = lbl_8087F0A8->m244;
    Init_unk();
}

void SetConfig::fn_8057E8C4() {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) SetConfig();
    }
}

void SetConfig::fn_8057EAF0() {
    void* root = mMenu.Load("SetConfig");
    void* group = fn_8008937C(root, "Controller");
    fn_800874C8(group, "AttackType", &mAttackType, 0, 2, 1, 0, 0);
    fn_800874C8(group, "DiveType", &mDiveType, 0, 2, 1, 0, 0);
    fn_800874C8(group, "InTacviewType", &mInTacviewType, 0, 2, 1, 0, 0);
    fn_800874C8(group, "GatheringType", &mGatheringType, 0, 2, 1, 0, 0);
    group = fn_8008937C(root, "Camera");
    fn_8008771C(group, "RollbackSpeed", &mRollbackSpeed, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_8008771C(group, "CameraRotSpeed", &mCameraRotSpeed, 0, 0, 0.0f, 2.0f, 0.1f);
    fn_8008771C(group, "TurnSpeed", &mTurnSpeed, 0, 0, 0.2f, 1.5f, 0.1f);
    fn_8008771C(group, "AttackTurnSpeed", &mAttackTurnSpeed, 0, 0, 0.2f, 1.5f, 0.1f);
    fn_8008771C(group, "RotDelay", &mRotDelay, 0, 0, 1.0f, 60.0f, 1.0f);
    group = fn_8008937C(root, "BattleUI");
    fn_800874C8(group, "DrawHateLine", &mBattleDrawHateLine, 0, 4, 1, 0, 0);
    fn_80087994(group, "DrawEnemyHP", &mDrawEnemyHP, 0, 0);
    fn_80087994(group, "DrawEnemyPopup", &mDrawEnemyPopup, 0, 0);
    fn_80087994(group, "DrawFriendPopup", &mDrawFriendPopup, 0, 0);
    group = fn_8008937C(root, "EventUI");
    fn_80087994(group, "ShowDialog", &mShowDialog, 0, 0);
    fn_80087994(group, "ShowTutorial", &mShowTutorial, 0, 0);
    fn_800874C8(group, "EventSkipType", &mEventSkipType, 0, 3, 1, 0, 0);
    fn_80087994(group, "EventCameraControl", &mEventCameraControl, 0, 0);
    group = fn_8008937C(root, "Sound");
    fn_800874C8(group, "SoundType", &mSoundType, 0, 3, 1, 0, 0);
    fn_8008771C(group, "BGM Volume", &mBgmVolume, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_8008771C(group, "SE Volume", &mSeVolume, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_8008771C(group, "Voice Volume", &mVoiceVolume, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_800874C8(root, "WindKey", &mWindKey, 0, 2, 1, 0, 0);
    fn_80087994(root, "PlayerHpBarShow", &mPlayerHpBarShow, 0, 0);
    fn_80087994(root, "DrawHateLine", &mDrawHateLine, 0, 0);
    fn_80087994(root, "autoSave", &mAutoSave, 0, 0);
    fn_800874C8(root, "CampVariation", &mCampVariation, 0, 1, 1, 0, 0);
    fn_80087994(root, "TacviewHint", &mTacviewHint, 0, 0);
    fn_800874C8(root, "shakeType", &mShakeType, 0, 1, 1, 0, 0);
    fn_80087994(root, "SwingReplay", &mSwingReplay, 0, 0);
}

// inferred: same code as Init_unk; the out-of-line copy reads the BGM volume before the null check
void SetConfig::fn_8057EF40() {
    m24 = 0;
    mAttackType = 0;
    mDiveType = 0;
    mInTacviewType = 0;
    mGatheringType = 0;
    memset(m38, 0, sizeof(m38));
    mAttackTurnSpeed = 1.0f;
    mTurnSpeed = 1.0f;
    mRollbackSpeed = 0.8f;
    mCameraRotSpeed = 1.0f;
    mRotDelay = 31.0f;
    m50 = 16;
    mBattleDrawHateLine = 2;
    mDrawEnemyHP = 1;
    mDrawEnemyPopup = 1;
    mDrawFriendPopup = 1;
    memset(m68, 0, sizeof(m68));
    mShowDialog = 1;
    mShowTutorial = 1;
    mEventSkipType = 1;
    mEventCameraControl = 0;
    memset(m88, 0, sizeof(m88));
    mSoundType = 1;
    mBgmVolume = 1.0f;
    mSeVolume = 1.0f;
    mVoiceVolume = 1.0f;
    unsigned char mode = SCGetSoundMode();
    if (mode == 0) {
        mSoundType = 2;
    } else if (mode == 2) {
        mSoundType = 0;
    } else {
        mSoundType = 1;
    }
    float bgmVolume = mBgmVolume;
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->fn_800D0240(0, bgmVolume, 0);
        lbl_8087EFE8->fn_800D0240(1, mSeVolume, 0);
        lbl_8087EFE8->fn_800D0240(2, mVoiceVolume, 0);
        switch (mSoundType) {
        case 0:
            lbl_8087EFE8->mOutputMode = 2;
            break;
        case 2:
            lbl_8087EFE8->mOutputMode = 3;
            break;
        default:
            lbl_8087EFE8->mOutputMode = 0;
            break;
        }
    }
    mA4 = 0;
    if (lbl_8087EFA8 != 0) {
        lbl_8087EFA8->m34 = mA4 + 10;
    }
}


void SetConfig::fn_8057F0F8() {
    unsigned char mode = SCGetSoundMode();
    if (mode == 0) {
        mSoundType = 2;
    } else if (mode == 2) {
        mSoundType = 0;
    } else {
        mSoundType = 1;
    }
}

void SetConfig::fn_8057F154(int soundType) {
    mSoundType = soundType;
    if (soundType == 2) {
        lbl_8087EFE8->mOutputMode = 3;
    } else if (soundType == 0) {
        lbl_8087EFE8->mOutputMode = 2;
    } else {
        lbl_8087EFE8->mOutputMode = 0;
    }
}

void SetConfig::fn_8057F198(int value) {
    mA4 = value;
    if (lbl_8087EFA8 != 0) {
        lbl_8087EFA8->m34 = value + 10;
    }
}

void SetConfig::fn_8057F1B4(const SoundSettings_unk* settings) {
    mSoundType = settings->mSoundType;
    mBgmVolume = settings->mBgmVolume;
    mSeVolume = settings->mSeVolume;
    mVoiceVolume = settings->mVoiceVolume;
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->fn_800D0240(0, mBgmVolume, 0);
        lbl_8087EFE8->fn_800D0240(1, mSeVolume, 0);
        lbl_8087EFE8->fn_800D0240(2, mVoiceVolume, 0);
        switch (mSoundType) {
        case 0:
            lbl_8087EFE8->mOutputMode = 2;
            break;
        case 2:
            lbl_8087EFE8->mOutputMode = 3;
            break;
        default:
            lbl_8087EFE8->mOutputMode = 0;
            break;
        }
    }
}
