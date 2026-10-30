#ifndef GAME_SETCONFIG_H
#define GAME_SETCONFIG_H

#include "game/ResHandle.h"

// inferred: sound part of the save data (GameSaveData_unk copies it from 0x68..0x74)
struct SoundSettings_unk {
    int mSoundType;     // 0x0
    float mBgmVolume;   // 0x4
    float mSeVolume;    // 0x8
    float mVoiceVolume; // 0xC
};

// inferred from __vt__9SetConfig (0x80796790): the option settings (debug menu "SetConfig").
// The member names come from the debug menu labels registered in fn_8057EAF0.
class SetConfig {
public:
    static void fn_8057E8C4(); // inferred: creates the instance
    static SetConfig* sInstance;
    SetConfig();
    void fn_8057EAF0(); // inferred: registers the debug menu
    void fn_8057EF40(); // inferred: resets the settings to their defaults
    void Init_unk();    // inferred: inline body shared by the ctor and fn_8057EF40
    void fn_8057F0F8(); // inferred: sound type from the console sound mode
    void fn_8057F154(int soundType); // inferred: sets the sound type and the output mode
    void fn_8057F198(int value);     // inferred: sets mA4 (+10 into lbl_8087EFA8)
    void fn_8057F1B4(const SoundSettings_unk* settings); // inferred: loads the sound settings

    ResHandle mMenu;       // 0x00
    int mShakeType;        // 0x04
    int mSwingReplay;      // 0x08
    int mPlayerHpBarShow;  // 0x0C
    int mAutoSave;         // 0x10
    int mTacviewHint;      // 0x14
    int mDrawHateLine;     // 0x18 inferred: second flag registered under the same label
    int mCampVariation;    // 0x1C
    int mWindKey;          // 0x20
    int m24;
    int mAttackType;       // 0x28
    int mDiveType;         // 0x2C
    int mInTacviewType;    // 0x30
    int mGatheringType;    // 0x34
    unsigned char m38[8];
    float mAttackTurnSpeed; // 0x40
    float mTurnSpeed;       // 0x44
    float mRollbackSpeed;   // 0x48
    float mCameraRotSpeed;  // 0x4C
    int m50;
    float mRotDelay;        // 0x54
    int mBattleDrawHateLine; // 0x58
    int mDrawEnemyHP;       // 0x5C
    int mDrawEnemyPopup;    // 0x60
    int mDrawFriendPopup;   // 0x64
    unsigned char m68[0x10];
    int mShowDialog;        // 0x78
    int mShowTutorial;      // 0x7C
    int mEventSkipType;     // 0x80
    int mEventCameraControl; // 0x84
    unsigned char m88[0xC];
    SoundSettings_unk mSound; // 0x94: inferred: copied as a whole by NewCampPartsOption::vf2C
    int mA4;

    virtual ~SetConfig(); // 0x8057E83C
};

#endif
