#ifndef GAME_GAMECONFIG_UNK_H
#define GAME_GAMECONFIG_UNK_H

#include "game/Input_unk.h"
#include "game/NewCampScene2_unk.h"
#include "math/Vec.h"
#include "string.h"

// inferred: game settings with a debug menu (0x1144 bytes; the instance is lbl_8087F0A8, created by
// fn_80185AD4; game/GameConfig_unk.cpp). Class name invented; the member names come from the debug
// menu labels registered in fn_80186520.

// inferred: fixed string of 32 characters (copied with strlen/memcpy unless it is assigned to itself)
struct ConfigString_unk {
    ConfigString_unk() { mBuf[0] = '\0'; }
    ConfigString_unk(const char* s) { Set_unk(s); }
    void Set_unk(const char* s) {
        if (s != mBuf) {
            memcpy(mBuf, s, strlen(s) + 1);
        }
    }
    char mBuf[0x20];
};

// inferred: parameters of a difficulty level (debug menu "DifficultyLevel"); names from the menu
struct DifficultyParam_unk {
    void Register_unk(void* group);
    float mLevelMul;
    int mLevelAdd;
    float mLevelMulBoss;
    int mLevelAddBoss;
    float mHPMul;
    int mHPAdd;
    float mAttack;
    int mAttackAdd;
};

// inferred: bonus damage per level difference -5..5 (debug menu "LvBonusDamage")
struct LvBonus_unk {
    float m[11];
};

// inferred: 0x3C bytes at 0x418, constructor 0x80184A64; not in the debug menu
struct GameConfig418_unk {
    GameConfig418_unk();
    int m00;
    int m04;
    int m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    int m34;
    int m38;
};

// inferred: 0x60 bytes at 0x4DC, constructor 0x80184B4C; not in the debug menu
struct GameConfig4DC_unk {
    GameConfig4DC_unk();
    int m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    int m30;
    int m34;
    int m38;
    int m3C;
    int m40;
    float m44;
    int m48;
    int m4C;
    float m50;
    float m54;
    float m58;
    float m5C;
};

// inferred: 0x48 bytes at 0x53C, constructor 0x80184AD4; not in the debug menu
struct GameConfig53C_unk {
    GameConfig53C_unk();
    int m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    int m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    int m2C;
    float m30;
    float m34;
    float m38;
    int m3C;
    int m40;
    int m44;
};

namespace atn {
// inferred: 2D vector of the dialog layout (weak copies 0x800F84BC and 0x800F833C of another file; a
// different class from Vector2_unk, whose copies at 0x805030FC are in game/SoftKeyboardU.cpp). Name
// invented.
struct Vec2_unk {
    Vec2_unk() {}
    Vec2_unk(float x_, float y_) : x(x_), y(y_) {}
    Vec2_unk& operator=(const Vec2_unk& v) {
        __Vec2Copy(&x, &v.x);
        return *this;
    }
    float x, y;
};
} // namespace atn

// inferred: one side of the dialog layout (debug menu "Dialog/Left", "Dialog/Right")
struct DialogSide_unk {
    DialogSide_unk();
    atn::Vec2_unk mVPos;        // 0x00
    atn::Vec2_unk mVSize;       // 0x08
    atn::Vector3 mPos;          // 0x10
    atn::Vector3 mCamPos;       // 0x1C
    atn::Vector3 mCamTarget;    // 0x28
    float mCamAngleOfView;      // 0x34
};


// inferred: dialog layout (debug menu "Dialog"); 0xB80 bytes at 0x5BC
struct DialogConfig_unk {
    DialogConfig_unk();
    void fn_801856B8(void* menu); // inferred: debug menu
    DialogSide_unk mLeft;          // 0x000
    DialogSide_unk mRight;         // 0x038
    DialogSide_unk m070;
    NewCampScene2_unk mScenes[2][2]; // 0x0A8: inferred: [dark][right] (Event::DialogHead::fn_80226B28)
    int mUseWaitMotion;            // 0xB78
    int mUseLipSync;               // 0xB7C
};

// inferred: fades of the title and game over screens (debug menus "Title" and "GameOver")
struct TitleConfig_unk {
    TitleConfig_unk();
    void Register_unk(void* title);
    void RegisterGameOver_unk(void* gameOver);
    int mBgmStart;               // 0x00
    int mBgmFadeTime;            // 0x04
    int mMaskFadeTime;           // 0x08
    int mTitleFadeStart;         // 0x0C
    int mCompanyFadeStart;       // 0x10
    int mPushAFadeStart;         // 0x14
    int m18;
    int mEasyModeType;           // 0x1C
    int mLogoFadeStart;          // 0x20
    int m24;
    int m28;
    int m2C;
    int m30;
    int m34;
};

class GameConfig_unk {
public:
    static void fn_80185AD4(); // inferred: creates the instance
    GameConfig_unk();
    void fn_80186520();        // inferred: debug menu
    void fn_80187C44();        // inferred: resets the strap name and some flags
    float fn_80187CC0(int diff); // inferred: bonus damage for a level difference (-5..5)

    void* mMenu;                 // 0x000
    int mDrawSystemInfo;         // 0x004
    int mDrawCollision;          // 0x008
    int mDrawColliWire;          // 0x00C
    int mDrawColliAttrColor;     // 0x010
    int mDrawBirdViewColli;      // 0x014
    int mDrawMap;                // 0x018
    int mDrawMapBounds;          // 0x01C
    int mDrawChara;              // 0x020
    int m024;
    int mFileListCache;          // 0x028
    int m02C;
    int m030;
    int mDrawCharaColli;         // 0x034
    int mDrawDamageSphere;       // 0x038
    int mDebugSceneFlag;         // 0x03C
    int m040;
    float m044;
    float m048;
    int mDrawErrorMessage;       // 0x04C
    int m050;
    int mDrawCharaStatus;        // 0x054
    int mDrawCharaArea;          // 0x058
    int mDrawHateInfo;           // 0x05C
    int m060;
    float m064;
    int m068;
    int m06C;
    int mLogMessageLimitLevel;   // 0x070
    int mDrawGimmickName;        // 0x074
    int mPauseScriptExecute;     // 0x078
    int m07C;
    int m080;
    int m084;
    int m088;
    float m08C;
    int m090;
    int m094;
    int m098;
    int m09C;
    float m0A0;
    int m0A4;
    float m0A8;
    float m0AC;
    int m0B0;
    int m0B4;
    int m0B8;
    int m0BC;
    float m0C0;
    int mBootLoad;               // 0x0C4
    int m0C8;
    int m0CC;
    int m0D0;
    int mCurrentLevel;           // 0x0D4
    DifficultyParam_unk mDifficulty[4]; // 0x0D8: Easy, Normal, Hard, VeryHard
    int m158;
    int m15C;
    int m160;
    int mTownAllLoad;            // 0x164
    int mDrawHateAttInfo;        // 0x168
    int mAutoTensionSkillFriend; // 0x16C
    int mDropItemType;           // 0x170
    int mVoiceLimitLevel;        // 0x174
    int mPreloadEventVoice;      // 0x178
    int mPreloadEventMA;         // 0x17C
    int mProcessSuccessiveEvent; // 0x180
    int mDrawGameInfo;           // 0x184
    int m188;
    int m18C;
    int mLoadTownSubScript;      // 0x190
    int m194;
    int m198;
    float mNearClip;             // 0x19C: debug camera
    float mFarClip;              // 0x1A0
    float mAngleOfView;          // 0x1A4
    float mMoveSpeed;            // 0x1A8
    float mZRot;                 // 0x1AC
    int m1B0;
    ConfigString_unk mStrap;     // 0x1B4: inferred: "DebugMenu", "Strap" after fn_80187C44
    ConfigString_unk mDate;      // 0x1D4: inferred: "--/--/-- --:--"
    ConfigString_unk m1F4;
    int mSceneDebugEnabled;      // 0x214: debug menu "SceneDebug"
    int mMovePoint;              // 0x218
    int mHidePoint;              // 0x21C
    int mWarpPoint;              // 0x220
    int mInteractionPoint;       // 0x224
    int mAttentionPoint;         // 0x228
    int mEventBox;               // 0x22C
    int mEventSphere;            // 0x230
    int mAIArea;                 // 0x234
    int mGimmick;                // 0x238
    int mCameraBox;              // 0x23C
    int mBgmBox;                 // 0x240
    int m244;                    // 0x244: inferred: start of the player settings (game/PlayerInput_unk.cpp)
    float m248;
    float m24C;
    int mHateAttackedNum;        // 0x250: debug menu "Player"
    int mHateAttackedDelayFrame; // 0x254
    float mHateAbsorbHpRate;     // 0x258
    int m25C;
    int m260;
    float mLockOnLen;            // 0x264
    float mLockOnReleaseLen;     // 0x268
    float mLockOnTheta;          // 0x26C
    int m270;
    int m274;
    int m278;
    int m27C;
    int mAddExpImmediately;      // 0x280
    int m284;
    int mLevelCap;               // 0x288
    int m28C;
    int mAlwaysBirdview;         // 0x290
    int mCircleComboType;        // 0x294
    int mExpKillBounus;          // 0x298
    int mCounterBonus;           // 0x29C
    int m2A0;
    float mBtnAtkCorrectRadius;  // 0x2A4
    float mBtnAtkCorrectTheta;   // 0x2A8
    float mBtnAtkCutFrame;       // 0x2AC
    int mAttackEndWait;          // 0x2B0
    float mAttackGuardWait;      // 0x2B4
    float mBtnAtkChargeStart;    // 0x2B8
    float mMagicCutFrame;        // 0x2BC
    int mWallMountleTest;        // 0x2C0
    int mFlickMagic;             // 0x2C4
    float mAttackMinDamage;      // 0x2C8
    int mEnemyRandOfs;           // 0x2CC
    int mLvBonusType;            // 0x2D0
    LvBonus_unk mLvBonus;        // 0x2D4
    int m300;
    int mOverdriveExplosion;     // 0x304
    int mPlayableVoice;          // 0x308
    float mPickSucceedBounus;    // 0x30C
    float mSummonExpBounus;      // 0x310
    int mTacViewWind;            // 0x314
    int m318;
    float mManualPower;          // 0x31C
    int mTouchHelp;              // 0x320
    float mHeadShootDamage;      // 0x324
    float mHeadShootSubDamage;   // 0x328
    int mNewDiveMode;            // 0x32C
    int m330;
    float mGuardTheta;           // 0x334: debug menu "Guard"
    int mGuardDamage;            // 0x338
    int mGuardHide;              // 0x33C
    float mMagicNormalGuard;     // 0x340
    float mMagicPerfectGuard;    // 0x344
    float mAvoidRate;            // 0x348
    float mDiveDamageRatio;      // 0x34C
    int mTargetedNumDefault;     // 0x350: debug menu "Hate"
    int mHideDown;               // 0x354
    int mTargetSightDown;        // 0x358
    int mHateRatioDown;          // 0x35C
    int mHateLineFadeout;        // 0x360
    unsigned int mLineColorNormal; // 0x364
    unsigned int mLineColorSword;  // 0x368
    unsigned int mLineColorArcher; // 0x36C
    unsigned int mLineColorMagic;  // 0x370
    int mPlayerMinusHate;        // 0x374
    int mIgnoreLongHate;         // 0x378
    int mIgnoreLongHateValue[10]; // 0x37C
    int mIgnoreLongHateRand[10];  // 0x3A4
    int mParamIndex;             // 0x3CC: debug menu "AttractHate"
    int mTimeReset;              // 0x3D0
    int mButtonReset;            // 0x3D4
    int mHideReset;              // 0x3D8
    int mBattleEndReset;         // 0x3DC
    int mButtonRetry;            // 0x3E0
    int mNoCharge;               // 0x3E4
    int mNoGauge;                // 0x3E8
    int mFriendAttract;          // 0x3EC
    int mFriendAttractFrame;     // 0x3F0
    int mTransfer;               // 0x3F4
    int mCoolDownFrame;          // 0x3F8
    int mHairColor;              // 0x3FC
    int mHairScale;              // 0x400
    int mEnemyAIChange;          // 0x404
    int mExplosion;              // 0x408
    float mExplsnAddHateAtt;     // 0x40C
    float mExplsnAddDamage;      // 0x410
    float mExplsnAddTime;        // 0x414
    GameConfig418_unk m418;
    int mSurround;               // 0x454: debug menu "Surround"
    int mSurroundMode;           // 0x458
    float mPlayerDamageRatio;    // 0x45C
    float mEnemyDamageRatio;     // 0x460
    float mForceRatio;           // 0x464
    float mBattleWaitRatio;      // 0x468
    float mMoveSpeedRatio;       // 0x46C
    int mShowPlayerUI;           // 0x470: debug menu "UI"
    int mShowActionIcon;         // 0x474
    int mShowBattleCommand;      // 0x478
    int mShowReleadBar;          // 0x47C
    int mShowDownIcon;           // 0x480
    float mTipsYOffset;          // 0x484
    int mShowReleadBar2;         // 0x488: inferred: registered with the same label
    Input_unk mInput;            // 0x48C
    GameConfig4DC_unk m4DC;
    GameConfig53C_unk m53C;
    TitleConfig_unk mTitle;      // 0x584
    DialogConfig_unk mDialog;    // 0x5BC
    int m113C;
    unsigned char m1140;
};

#endif
