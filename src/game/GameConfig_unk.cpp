#include "game/GameConfig_unk.h"
#include "atn/MindowsCommon.h"
#include "atn/Memory.h"
extern "C" int sprintf(char* buf, const char* format, ...);
#include "macros.h"

extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" int lbl_8087EED0; // inferred: profile layer (debug menu "Profile Layer")
extern "C" void* __construct_array(void* array, void* ctor, void* dtor, unsigned long size, unsigned long count);

// inferred: names of the IgnoreLongHate entries (debug menu "Hate/IgnoreLongHate")
static const char* const sHateNames[10] = {"SP", "HM", "GG", "LZ", "UD", "GI", "BS", "RN", "MN", "DM"};
// inferred: default bonus damage per level difference -5..5
static const LvBonus_unk sLvBonusDefault = {{0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 1.0f, 1.2f, 1.25f, 1.25f, 1.3f, 2.0f}};

GameConfig418_unk::GameConfig418_unk()
    : m00(1), m04(0), m08(1), m0C(0.06981317f), m10(10.0f), m14(100.0f), m18(20.0f), m1C(200.0f), m20(0.3f),
      m24(6.0f), m28(0.0f), m2C(1.5f), m30(6.0f), m34(0), m38(40) {}

GameConfig53C_unk::GameConfig53C_unk()
    : m00(0), m04(0.6f), m08(0.89f), m0C(1.04f), m10(0.01f), m14(150), m18(200.0f), m1C(200.0f), m20(50.0f),
      m24(200.0f), m28(20.0f), m2C(100), m30(200.0f), m34(1.7453293f), m38(0.01f), m3C(100), m40(0), m44(0) {}

GameConfig4DC_unk::GameConfig4DC_unk()
    : m00(0), m04(120.0f), m08(60.0f), m10(20.0f), m14(35.0f), m18(10.0f), m20(0.2f), m24(1.0f), m28(0.6f),
      m2C(0.5f), m30(6), m34(6), m38(30), m3C(0), m40(450), m44(0.004f), m48(8), m4C(100), m50(0.0f), m54(12.0f),
      m58(8.0f), m5C(0.47123894f) {
    m0C = 2.0943952f;
    m1C = 0.05235988f;
}

DialogConfig_unk::DialogConfig_unk() : mUseWaitMotion(1), mUseLipSync(1) {
    mLeft.mVPos = atn::Vec2_unk(0.0f, 0.0f);
    mLeft.mVSize = atn::Vec2_unk(320.0f, 448.0f);
    mLeft.mPos = atn::Vector3(0.0f, 0.0f, 0.0f);
    mLeft.mCamPos = atn::Vector3(-6.18f, 0.87f, 17.75f);
    mLeft.mCamTarget = atn::Vector3(0.08f, 0.33f, 6.82f);
    mLeft.mCamAngleOfView = atn::DegToRad_unk(30.0f);
    mRight.mVPos = atn::Vec2_unk(320.0f, 0.0f);
    mRight.mVSize = atn::Vec2_unk(320.0f, 448.0f);
    mRight.mPos = atn::Vector3(0.0f, 0.0f, 0.0f);
    mRight.mCamPos = atn::Vector3(13.54f, 0.87f, 13.43f);
    mRight.mCamTarget = atn::Vector3(3.22f, 0.33f, 6.21f);
    mRight.mCamAngleOfView = atn::DegToRad_unk(30.0f);
    m070.mVPos = atn::Vec2_unk(320.0f, 0.0f);
    m070.mVSize = atn::Vec2_unk(320.0f, 448.0f);
    m070.mPos = atn::Vector3(0.0f, 0.0f, 0.0f);
    m070.mCamPos = atn::Vector3(10.47f, 0.87f, 15.64f);
    m070.mCamTarget = atn::Vector3(1.72f, 0.33f, 6.57f);
    m070.mCamAngleOfView = atn::DegToRad_unk(30.0f);
    Light_unk light;
    light.fn_800790D0(atn::Vector3(0.1f, 0.1f, -1.0f).GetNormalize(), atn::Color4f_unk(0.5f, 0.5f, 0.5f, 1.0f));
    mScenes[0].AddLight_unk(light);
    mScenes[0].GetAmbient_unk() = atn::Color4f_unk(0.8f, 0.75f, 0.75f, 1.0f);
    mScenes[0].GetLS_unk() = atn::Color4f_unk(0.1f, 0.1f, 0.1f, 1.0f);
    light.fn_800790D0(atn::Vector3(-1.0f, -0.2f, -0.99f).GetNormalize(), atn::Color4f_unk(0.5f, 0.5f, 0.5f, 1.0f));
    mScenes[1].AddLight_unk(light);
    mScenes[1].GetAmbient_unk() = atn::Color4f_unk(0.8f, 0.75f, 0.75f, 1.0f);
    mScenes[1].GetLS_unk() = atn::Color4f_unk(0.1f, 0.1f, 0.1f, 1.0f);
    light.fn_800790D0(atn::Vector3(0.1f, 0.1f, -1.0f).GetNormalize(), atn::Color4f_unk(0.5f, 0.5f, 0.5f, 1.0f));
    mScenes[2].AddLight_unk(light);
    mScenes[2].GetAmbient_unk() = atn::Color4f_unk(0.35f, 0.3f, 0.3f, 1.0f);
    mScenes[2].GetLS_unk() = atn::Color4f_unk(0.1f, 0.1f, 0.1f, 1.0f);
    light.fn_800790D0(atn::Vector3(-1.0f, -0.2f, -0.99f).GetNormalize(), atn::Color4f_unk(0.5f, 0.5f, 0.5f, 1.0f));
    mScenes[3].AddLight_unk(light);
    mScenes[3].GetAmbient_unk() = atn::Color4f_unk(0.35f, 0.3f, 0.3f, 1.0f);
    mScenes[3].GetLS_unk() = atn::Color4f_unk(0.1f, 0.1f, 0.1f, 1.0f);
}

DialogSide_unk::DialogSide_unk() {}

// NONMATCHING: 98.0% - register allocation (dialog folder, string pool and current folder)
void DialogConfig_unk::fn_801856B8(void* menu) {
    atn::MindowsFolder* dialog = fn_8008937C((atn::MindowsFolder*)menu, "Dialog");
    fn_80087994(dialog, "UseWaitMotion", &mUseWaitMotion, 0, 0);
    fn_80087994(dialog, "UseLipSync", &mUseLipSync, 0, 0);
    atn::MindowsFolder* group = fn_8008937C(dialog, "Left");
    fn_80087BB4(group, "V Pos", &mLeft.mVPos.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087BB4(group, "V Size", &mLeft.mVSize.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087E9C(group, "Pos", &mLeft.mPos.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087E9C(group, "Cam Pos", &mLeft.mCamPos.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087E9C(group, "Cam Target", &mLeft.mCamTarget.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087858(group, "Cam AngleOfView", &mLeft.mCamAngleOfView, -360.0f, 360.0f, 1.0f, 0, 0);
    group = fn_8008937C(dialog, "Right");
    fn_80087BB4(group, "V Pos", &mRight.mVPos.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087BB4(group, "V Size", &mRight.mVSize.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087E9C(group, "Pos", &mRight.mPos.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087E9C(group, "Cam Pos", &mRight.mCamPos.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087E9C(group, "Cam Target", &mRight.mCamTarget.x, -1000.0f, 1000.0f, 1.0f, 0, 0);
    fn_80087858(group, "Cam AngleOfView", &mRight.mCamAngleOfView, -360.0f, 360.0f, 1.0f, 0, 0);
    const char* names[2] = {"SceneLeft", "SceneRight"};
    NewCampScene2_unk* scene = &mScenes[0];
    NewCampScene2_unk* dark = &mScenes[1];
    for (int i = 0; i < 2; i++, scene += 2, dark += 2) {
        
        group = fn_8008937C(dialog, names[i]);
        fn_80088FAC(group, "Ambient", &scene->mAmbient.r, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_80088FAC(group, "LS", &scene->mLS.r, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_80088FAC(group, "AmbientDark", &dark->mAmbient.r, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_80088FAC(group, "LSDark", &dark->mLS.r, 0.0f, 1.0f, 0.01f, 0, 0);
        atn::MindowsFolder* fog = fn_8008937C(group, "Fog");
        fn_800874C8(fog, "Fog Mode", &scene->mFogMode, 0, 6, 1, 0, 0);
        fn_8008771C(fog, "Fog Start", &scene->mFogStart, 0.0f, 10000.0f, 1.0f, 0, 0);
        fn_8008771C(fog, "Fog End", &scene->mFogEnd, 0.0f, 10000.0f, 1.0f, 0, 0);
        fn_8008771C(fog, "Fog R", &scene->mFogR, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(fog, "Fog G", &scene->mFogG, 0.0f, 1.0f, 0.01f, 0, 0);
        fn_8008771C(fog, "Fog B", &scene->mFogB, 0.0f, 1.0f, 0.01f, 0, 0);
        atn::MindowsFolder* keyLight = fn_8008937C(group, "KeyLight");
        fn_800C2448(scene, 0)->fn_80079BC8(keyLight);
    }
}

// inferred: also inlined in fn_80186520; the out-of-line copy is unused (the linker drops it), but its
// strings come before the ones of fn_80185AD4 in the pool
void DifficultyParam_unk::Register_unk(void* group) {
    atn::MindowsFolder* folder = (atn::MindowsFolder*)group;
    fn_8008771C(folder, "LevelMul", &mLevelMul, 0.0f, 1000.0f, 0.1f, 0, 0);
    fn_800874C8(folder, "LevelAdd", &mLevelAdd, 0, 1000, 1, 0, 0);
    fn_8008771C(folder, "LevelMulBoss", &mLevelMulBoss, 0.0f, 1000.0f, 0.1f, 0, 0);
    fn_800874C8(folder, "LevelAddBoss", &mLevelAddBoss, 0, 1000, 1, 0, 0);
    fn_8008771C(folder, "HPMul", &mHPMul, 0.0f, 1000.0f, 0.1f, 0, 0);
    fn_800874C8(folder, "HPAdd", &mHPAdd, 0, 1000, 1, 0, 0);
    fn_8008771C(folder, "Attack", &mAttack, 0.0f, 1000.0f, 0.1f, 0, 0);
    fn_800874C8(folder, "AttackAdd", &mAttackAdd, 0, 1000, 1, 0, 0);
}

inline TitleConfig_unk::TitleConfig_unk() {
    mBgmStart = 0;
    mBgmFadeTime = 20;
    mMaskFadeTime = 90;
    mTitleFadeStart = 255;
    mCompanyFadeStart = 300;
    mPushAFadeStart = 740;
    mLogoFadeStart = 45;
    mEasyModeType = 1;
    m24 = 0;
    m28 = 1;
    m2C = 1;
    m30 = 1;
    m34 = 0;
}

void TitleConfig_unk::Register_unk(void* title) {
    atn::MindowsFolder* folder = (atn::MindowsFolder*)title;
    fn_800874C8(folder, "BGM Start", &mBgmStart, 0, 10000, 1, 0, 0);
    fn_800874C8(folder, "BGM Fade Time", &mBgmFadeTime, 0, 10000, 1, 0, 0);
    fn_800874C8(folder, "Mask Fade Time", &mMaskFadeTime, 0, 10000, 1, 0, 0);
    fn_800874C8(folder, "Title Fade Start", &mTitleFadeStart, 0, 10000, 1, 0, 0);
    fn_800874C8(folder, "Company Fade Start", &mCompanyFadeStart, 0, 10000, 1, 0, 0);
    fn_800874C8(folder, "PushA Fade Start", &mPushAFadeStart, 0, 10000, 1, 0, 0);
}

void TitleConfig_unk::RegisterGameOver_unk(void* gameOver) {
    atn::MindowsFolder* folder = (atn::MindowsFolder*)gameOver;
    fn_800874C8(folder, "EasyModeType", &mEasyModeType, 0, 10000, 1, 0, 0);
    fn_800874C8(folder, "Logo Fade Start", &mLogoFadeStart, 0, 10000, 1, 0, 0);
}

void GameConfig_unk::fn_80185AD4() {
    if (!lbl_8087F0A8) {
        lbl_8087F0A8 = new ((MemoryTag)1, "", "", 0) GameConfig_unk();
    }
}

// NONMATCHING: 93.1% - the original keeps 255 in r30 from the title defaults for the first line color; register allocation of the constants 0/1/90
GameConfig_unk::GameConfig_unk()
    : mMenu(0), mDrawSystemInfo(0), mDrawCollision(0), mDrawColliWire(1), mDrawColliAttrColor(1),
      mDrawBirdViewColli(0), mDrawMap(1), mDrawMapBounds(0), mDrawChara(1), m024(0), mFileListCache(1), m02C(0),
      m030(30), mDrawCharaColli(0), mDrawDamageSphere(0), mDebugSceneFlag(0), m040(1), m044(0.8f), m048(1.4f),
      mDrawErrorMessage(1), m050(30), mDrawCharaStatus(0), mDrawCharaArea(0), mDrawHateInfo(0), m060(1), m064(4.0f),
      m068(450), m06C(1), mLogMessageLimitLevel(2), mDrawGimmickName(0), mPauseScriptExecute(0), m07C(1), m080(10),
      m084(0), m088(1), m08C(5.0f), m090(1), m094(0), m098(1), m09C(75), m0A0(2.0f), m0A4(-90), m0A8(0.1f),
      m0AC(1.5f), m0B0(0), m0B4(1), m0B8(1), m0BC(0), m0C0(0.333f), mBootLoad(1), m0C8(1), m0CC(0), m0D0(0),
      mCurrentLevel(1), m158(0), m15C(0), m160(0), mTownAllLoad(0), mDrawHateAttInfo(0), mAutoTensionSkillFriend(0),
      mDropItemType(1), mVoiceLimitLevel(0), mPreloadEventVoice(1), mPreloadEventMA(1), mProcessSuccessiveEvent(1),
      mDrawGameInfo(0), m188(1), m18C(0), mLoadTownSubScript(1), m194(0), m198(0), m1B0(0), mStrap("DebugMenu"),
      mDate("--/--/-- --:--"), m113C(0), m1140(0) {
    m244 = 1;
    m248 = 25.0f;
    m24C = 1.0471976f;
    mHateAttackedNum = 9;
    mHateAttackedDelayFrame = 90;
    mHateAbsorbHpRate = 0.2f;
    m25C = 1;
    m260 = 0;
    mLockOnLen = 90.0f;
    mLockOnReleaseLen = 170.0f;
    mLockOnTheta = 6.2831855f;
    m270 = 1;
    m274 = 1;
    m278 = 1;
    m27C = 1;
    mAddExpImmediately = 1;
    m284 = 1;
    mLevelCap = 10;
    m28C = 0;
    mAlwaysBirdview = 0;
    mCircleComboType = 0;
    mExpKillBounus = 0;
    mCounterBonus = 0;
    m2A0 = 1;
    mBtnAtkCorrectRadius = 40.0f;
    mBtnAtkCorrectTheta = 4.1887903f;
    mBtnAtkCutFrame = 1.0f;
    mAttackEndWait = 0;
    mAttackGuardWait = 10.0f;
    mBtnAtkChargeStart = 8.0f;
    mMagicCutFrame = 10.0f;
    mWallMountleTest = 0;
    mFlickMagic = 1;
    mAttackMinDamage = 0.1f;
    mLvBonusType = 0;
    mEnemyRandOfs = 1;
    for (int i = 0; i < 11; i++) {
        mLvBonus.m[i] = sLvBonusDefault.m[i];
    }
    m300 = 0;
    mOverdriveExplosion = 1;
    mPlayableVoice = 1;
    mPickSucceedBounus = 5.0f;
    mSummonExpBounus = 3.0f;
    mTacViewWind = 1;
    m318 = 1;
    mManualPower = 0.6f;
    mTouchHelp = 1;
    mHeadShootDamage = 2.0f;
    mHeadShootSubDamage = 2.0f;
    mNewDiveMode = 0;
    m330 = 0;
    mSceneDebugEnabled = 0;
    mMovePoint = 0;
    mHidePoint = 0;
    mInteractionPoint = 0;
    mWarpPoint = 1;
    mAttentionPoint = 1;
    mEventBox = 1;
    mEventSphere = 1;
    mAIArea = 1;
    mGimmick = 1;
    mCameraBox = 0;
    mBgmBox = 1;
    mGuardTheta = 3.4906585f;
    mGuardDamage = 1;
    mGuardHide = 0;
    mMagicNormalGuard = 0.5f;
    mMagicPerfectGuard = 0.25f;
    mAvoidRate = 0.0f;
    mDiveDamageRatio = 0.5f;
    mTargetedNumDefault = 2;
    mHideDown = 1;
    mTargetSightDown = 1;
    mHateRatioDown = 1;
    mHateLineFadeout = 0;
    mLineColorNormal = 0xFFFFFFFF;
    mLineColorSword = atn::Color4f_unk(1.0f, 0.85f, 0.2f, 1.0f).ToU32();
    mLineColorArcher = atn::Color4f_unk(0.77f, 0.31f, 0.98f, 1.0f).ToU32();
    mLineColorMagic = atn::Color4f_unk(0.9f, 0.18f, 0.18f, 1.0f).ToU32();
    mPlayerMinusHate = 40;
    mIgnoreLongHate = 1;
    memset(mIgnoreLongHateValue, 0, sizeof(mIgnoreLongHateValue));
    memset(mIgnoreLongHateRand, 0, sizeof(mIgnoreLongHateRand));
    mParamIndex = 3;
    mTimeReset = 0;
    mButtonReset = 1;
    mHideReset = 0;
    mBattleEndReset = 1;
    mButtonRetry = 0;
    mNoCharge = 1;
    mNoGauge = 1;
    mFriendAttract = 0;
    mFriendAttractFrame = 15;
    mTransfer = 0;
    mCoolDownFrame = 30;
    mHairColor = 0;
    mHairScale = 0;
    mEnemyAIChange = 2;
    mExplosion = 1;
    mExplsnAddHateAtt = 0.0f;
    mExplsnAddDamage = 1.001f;
    mExplsnAddTime = 0.0f;
    mSurround = 1;
    mSurroundMode = 0;
    mPlayerDamageRatio = 1.2f;
    mEnemyDamageRatio = 1.2f;
    mForceRatio = 0.25f;
    mBattleWaitRatio = 0.5f;
    mMoveSpeedRatio = 1.0f;
    mShowPlayerUI = 1;
    mShowActionIcon = 1;
    mShowBattleCommand = 0;
    mShowReleadBar = 0;
    mShowDownIcon = 1;
    mTipsYOffset = -16.0f;
    mShowReleadBar2 = 0;
    mDifficulty[0].mLevelMul = 0.7f;
    mDifficulty[0].mLevelAdd = 0;
    mDifficulty[0].mLevelMulBoss = 0.7f;
    mDifficulty[0].mLevelAddBoss = 0;
    mDifficulty[0].mHPMul = 0.8f;
    mDifficulty[0].mHPAdd = 0;
    mDifficulty[0].mAttack = 0.8f;
    mDifficulty[0].mAttackAdd = 0;
    mDifficulty[1].mLevelMul = 1.0f;
    mDifficulty[1].mLevelAdd = 0;
    mDifficulty[1].mLevelMulBoss = 1.0f;
    mDifficulty[1].mLevelAddBoss = 0;
    mDifficulty[1].mHPMul = 1.0f;
    mDifficulty[1].mHPAdd = 0;
    mDifficulty[1].mAttack = 1.0f;
    mDifficulty[1].mAttackAdd = 0;
    mDifficulty[2].mLevelMul = 1.5f;
    mDifficulty[2].mLevelAdd = 0;
    mDifficulty[2].mLevelMulBoss = 1.5f;
    mDifficulty[2].mLevelAddBoss = 0;
    mDifficulty[2].mHPMul = 1.5f;
    mDifficulty[2].mHPAdd = 0;
    mDifficulty[2].mAttack = 1.3f;
    mDifficulty[2].mAttackAdd = 0;
    mDifficulty[3].mLevelMul = 1.0f;
    mDifficulty[3].mLevelAdd = 0;
    mDifficulty[3].mLevelMulBoss = 1.2f;
    mDifficulty[3].mLevelAddBoss = 70;
    mDifficulty[3].mHPMul = 3.0f;
    mDifficulty[3].mHPAdd = 5000;
    mDifficulty[3].mAttack = 1.8f;
    mDifficulty[3].mAttackAdd = 500;
    mNearClip = 1.0f;
    mFarClip = 5000.0f;
    mAngleOfView = 1.134464f;
    mMoveSpeed = 3.0f;
    mZRot = 0.0f;
}

// NONMATCHING: 98.0% - register allocation (this, root folder and string pool)
void GameConfig_unk::fn_80186520() {
    atn::MindowsFolder* root;
    if (mMenu == 0 && lbl_8087EF10) {
        root = fn_8008937C(&lbl_8087EF10->mRoot, "Config");
        mMenu = root;
    } else {
        root = 0;
    }
    fn_800874C8(root, "DrawSystemInfo", &mDrawSystemInfo, 0, 2, 1, 0, 0);
    fn_80087994(root, "DrawGameInfo", &mDrawGameInfo, 0, 0);
    atn::MindowsFolder* g2 = fn_8008937C(root, "DebugCamera");
    fn_8008771C(g2, "NearClip", &mNearClip, 0.0001f, 100000.0f, 0.1f, 0, 0);
    fn_8008771C(g2, "FarClip", &mFarClip, 0.0001f, 100000.0f, 0.1f, 0, 0);
    fn_80087858(g2, "AngleOfView", &mAngleOfView, 0.1f, 360.0f, 0.1f, 0, 0);
    fn_8008771C(g2, "MoveSpeed", &mMoveSpeed, 0.0001f, 100000.0f, 0.1f, 0, 0);
    fn_8008771C(g2, "ZRot", &mZRot, 0.0001f, 100000.0f, 0.1f, 0, 0);
    atn::MindowsFolder* g3 = fn_8008937C(root, "DifficultyLevel");
    fn_800874C8(g3, "CurrentLevel", &mCurrentLevel, 0, 3, 1, 0, 0);
    mDifficulty[0].Register_unk(fn_8008937C(g3, "Easy"));
    mDifficulty[1].Register_unk(fn_8008937C(g3, "Normal"));
    mDifficulty[2].Register_unk(fn_8008937C(g3, "Hard"));
    mDifficulty[3].Register_unk(fn_8008937C(g3, "VeryHard"));
    mTitle.Register_unk(fn_8008937C(root, "Title"));
    mTitle.RegisterGameOver_unk(fn_8008937C(root, "GameOver"));
    fn_800874C8(root, "Profile Layer", &lbl_8087EED0, 0, 100000, 1, 0, 0);
    fn_80087994(root, "Town All Load", &mTownAllLoad, 0, 0);
    atn::MindowsFolder* g10 = fn_8008937C(root, "Player");
    fn_80087994(g10, "OverdriveExplosion", &mOverdriveExplosion, 0, 0);
    fn_800874C8(g10, "HateAttackedNum", &mHateAttackedNum, 0, 16, 1, 0, 0);
    fn_800874C8(g10, "HateAttackedDelayFrame", &mHateAttackedDelayFrame, 0, 999, 5, 0, 0);
    fn_8008771C(g10, "HateAbsorbHpRate", &mHateAbsorbHpRate, 0.0f, 1.0f, 0.1f, 0, 0);
    fn_8008771C(g10, "LockOnLen", &mLockOnLen, 0.0f, 10000.0f, 10.0f, 0, 0);
    fn_8008771C(g10, "LockOnReleaseLen", &mLockOnReleaseLen, 0.0f, 10000.0f, 10.0f, 0, 0);
    fn_80087858(g10, "LockOnTheta", &mLockOnTheta, 0.0f, 360.0f, 5.0f, 0, 0);
    fn_80087994(g10, "TacViewWind", &mTacViewWind, 0, 0);
    fn_8008771C(g10, "ManualPower", &mManualPower, 0.0f, 1.0f, 0.01f, 0, 0);
    fn_80087994(g10, "AddExpImmediately", &mAddExpImmediately, 0, 0);
    fn_800874C8(g10, "LevelCap", &mLevelCap, 0, 1, 100, 0, 0);
    fn_80087994(g10, "AlwaysBirdview", &mAlwaysBirdview, 0, 0);
    fn_800874C8(g10, "CircleComboType", &mCircleComboType, 0, 1, 1, 0, 0);
    fn_80087994(g10, "ExpKillBounus", &mExpKillBounus, 0, 0);
    fn_80087994(g10, "CounterBonus", &mCounterBonus, 0, 0);
    fn_8008771C(g10, "BtnAtkCorrectRadius", &mBtnAtkCorrectRadius, 0.0f, 1000.0f, 0.01f, 0, 0);
    fn_80087858(g10, "BtnAtkCorrectTheta", &mBtnAtkCorrectTheta, 0.0f, 360.0f, 0.01f, 0, 0);
    fn_8008771C(g10, "BtnAtkCutFrame", &mBtnAtkCutFrame, 0.0f, 100.0f, 0.01f, 0, 0);
    fn_80087994(g10, "AttackEndWait", &mAttackEndWait, 0, 0);
    fn_8008771C(g10, "AttackGuardWait", &mAttackGuardWait, 0.0f, 100.0f, 0.01f, 0, 0);
    fn_8008771C(g10, "BtnAtkChargeStart", &mBtnAtkChargeStart, 0.0f, 100.0f, 0.01f, 0, 0);
    fn_8008771C(g10, "MagicCutFrame", &mMagicCutFrame, 0.0f, 100.0f, 0.01f, 0, 0);
    fn_80087994(g10, "WallMountleTest", &mWallMountleTest, 0, 0);
    fn_80087994(g10, "FlickMagic", &mFlickMagic, 0, 0);
    fn_8008771C(g10, "AttackMinDamage", &mAttackMinDamage, 0.0f, 1.0f, 0.01f, 0, 0);
    atn::MindowsFolder* g11 = fn_8008937C(g10, "LvBonusDamage");
    fn_800874C8(g11, "Type", &mLvBonusType, 0, 2, 1, 0, 0);
    fn_80087994(g11, "EnemyRandOfs", &mEnemyRandOfs, 0, 0);
    for (int i = 0; i < 11; i++) {
        char name[16];
        sprintf(name, "Lv%2d", i - 5);
        fn_8008771C(g11, name, &mLvBonus.m[i], 0.0f, 10.0f, 0.01f, 0, 0);
    }
    fn_80087994(g10, "PlayableVoice", &mPlayableVoice, 0, 0);
    fn_8008771C(g10, "PickSucceedBounus", &mPickSucceedBounus, 0.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(g10, "SummonExpBounus", &mSummonExpBounus, 0.0f, 1.0f, 0.01f, 0, 0);
    fn_80087994(g10, "TouchHelp", &mTouchHelp, 0, 0);
    fn_8008771C(g10, "HeadShootDamage", &mHeadShootDamage, 0.0f, 1000.0f, 0.01f, 0, 0);
    fn_8008771C(g10, "HeadShootSubDamage", &mHeadShootSubDamage, 0.0f, 1000.0f, 0.01f, 0, 0);
    fn_800874C8(g10, "NewDiveMode", &mNewDiveMode, 0, 2, 1, 0, 0);
    fn_80087994(root, "PauseScriptExecute", &mPauseScriptExecute, 0, 0);
    fn_80087994(root, "DebugSceneFlag", &mDebugSceneFlag, 0, 0);
    atn::MindowsFolder* g12 = fn_8008937C(root, "SceneDebug");
    fn_80087994(g12, "Enabled", &mSceneDebugEnabled, 0, 0);
    fn_80087994(g12, "MovePoint", &mMovePoint, 0, 0);
    fn_80087994(g12, "HidePoint", &mHidePoint, 0, 0);
    fn_80087994(g12, "InteractionPoint", &mInteractionPoint, 0, 0);
    fn_80087994(g12, "WarpPoint", &mWarpPoint, 0, 0);
    fn_80087994(g12, "AttentionPoint", &mAttentionPoint, 0, 0);
    fn_80087994(g12, "EventBox", &mEventBox, 0, 0);
    fn_80087994(g12, "EventSphere", &mEventSphere, 0, 0);
    fn_80087994(g12, "AIArea", &mAIArea, 0, 0);
    fn_80087994(g12, "Gimmick", &mGimmick, 0, 0);
    fn_80087994(g12, "CameraBox", &mCameraBox, 0, 0);
    fn_80087994(g12, "BgmBox", &mBgmBox, 0, 0);
    mDialog.fn_801856B8(root);
    fn_80087994(root, "File List Cache", &mFileListCache, 0, 0);
    fn_80087994(root, "BootLoad", &mBootLoad, 0, 0);
    fn_80087994(root, "DrawCollision", &mDrawCollision, 0, 0);
    fn_80087994(root, "DrawColliWire", &mDrawColliWire, 0, 0);
    fn_80087994(root, "DrawColliAttrColor", &mDrawColliAttrColor, 0, 0);
    fn_80087994(root, "DrawBirdViewColli", &mDrawBirdViewColli, 0, 0);
    fn_80087994(root, "DrawCharaColli", &mDrawCharaColli, 0, 0);
    fn_80087994(root, "DrawCharaArea", &mDrawCharaArea, 0, 0);
    fn_80087994(root, "DrawDamageSphere", &mDrawDamageSphere, 0, 0);
    fn_80087994(root, "DrawMap", &mDrawMap, 0, 0);
    fn_80087994(root, "DrawMapBounds", &mDrawMapBounds, 0, 0);
    fn_80087994(root, "DrawChara", &mDrawChara, 0, 0);
    fn_80087994(root, "DrawCharaStatus", &mDrawCharaStatus, 0, 0);
    fn_800874C8(root, "DrawHateInfo", &mDrawHateInfo, 0, 10, 1, 0, 0);
    fn_800874C8(root, "VoiceLimitLevel", &mVoiceLimitLevel, 0, 3, 1, 0, 0);
    fn_800874C8(root, "LogMessageLimitLevel", &mLogMessageLimitLevel, 0, 99, 1, 0, 0);
    atn::MindowsFolder* g13 = fn_8008937C(root, "Guard");
    fn_80087858(g13, "Theta", &mGuardTheta, 0.0f, 360.0f, 10.0f, 0, 0);
    fn_80087994(g13, "GuardDamage", &mGuardDamage, 0, 0);
    fn_80087994(g13, "GuardHide", &mGuardHide, 0, 0);
    fn_8008771C(g13, "MagicNormalGuard", &mMagicNormalGuard, 0.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(g13, "MagicPerfectGuard", &mMagicPerfectGuard, 0.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(g13, "AvoidRate", &mAvoidRate, 0.0f, 1.0f, 0.01f, 0, 0);
    fn_8008771C(g13, "DiveDamageRatio", &mDiveDamageRatio, 0.0f, 1.0f, 0.01f, 0, 0);
    atn::MindowsFolder* g14 = fn_8008937C(root, "Hate");
    fn_800874C8(g14, "TargetedNumDefault", &mTargetedNumDefault, 0, 999, 1, 0, 0);
    fn_80087994(g14, "HideDown", &mHideDown, 0, 0);
    fn_80087994(g14, "TargetSightDown", &mTargetSightDown, 0, 0);
    fn_80087994(g14, "HateRatioDown", &mHateRatioDown, 0, 0);
    fn_80087994(g14, "HateLineFadeout", &mHateLineFadeout, 0, 0);
    fn_800875F8(g14, "LineColorNormal", &mLineColorNormal, 0, 0xFFFFFFFF, 1, 0, 0);
    fn_800875F8(g14, "LineColorSword", &mLineColorSword, 0, 0xFFFFFFFF, 1, 0, 0);
    fn_800875F8(g14, "LineColorArcher", &mLineColorArcher, 0, 0xFFFFFFFF, 1, 0, 0);
    fn_800875F8(g14, "LineColorMagic", &mLineColorMagic, 0, 0xFFFFFFFF, 1, 0, 0);
    fn_800874C8(g14, "PlayerMinusHate", &mPlayerMinusHate, 0, 999, 1, 0, 0);
    atn::MindowsFolder* g15 = fn_8008937C(g14, "IgnoreLongHate");
    fn_80087994(g15, "Enabled", &mIgnoreLongHate, 0, 0);
    for (int i = 0; i < 10; i++) {
        atn::MindowsFolder* entry = fn_8008937C(g15, sHateNames[i]);
        fn_800874C8(entry, "Value", &mIgnoreLongHateValue[i], 0, 9999, 1, 0, 0);
        fn_800874C8(entry, "Rand", &mIgnoreLongHateRand[i], 0, 9999, 1, 0, 0);
    }
    atn::MindowsFolder* g17 = fn_8008937C(root, "AttractHate");
    fn_800874C8(g17, "ParamIndex", &mParamIndex, 0, 99, 1, 0, 0);
    fn_80087994(g17, "TimeReset", &mTimeReset, 0, 0);
    fn_80087994(g17, "ButtonReset", &mButtonReset, 0, 0);
    fn_80087994(g17, "HideReset", &mHideReset, 0, 0);
    fn_80087994(g17, "BattleEndReset", &mBattleEndReset, 0, 0);
    fn_80087994(g17, "ButtonRetry", &mButtonRetry, 0, 0);
    fn_80087994(g17, "NoCharge", &mNoCharge, 0, 0);
    fn_80087994(g17, "NoGauge", &mNoGauge, 0, 0);
    fn_80087994(g17, "FriendAttract", &mFriendAttract, 0, 0);
    fn_800874C8(g17, "FriendAttractFrame", &mFriendAttractFrame, 0, 99, 1, 0, 0);
    fn_80087994(g17, "Transfer", &mTransfer, 0, 0);
    fn_800874C8(g17, "CoolDownFrame", &mCoolDownFrame, 0, 999, 1, 0, 0);
    fn_80087994(g17, "HairColor", &mHairColor, 0, 0);
    fn_80087994(g17, "HairScale", &mHairScale, 0, 0);
    fn_800874C8(g17, "EnemyAIChange", &mEnemyAIChange, 0, 3, 1, 0, 0);
    fn_80087994(g17, "Explosion", &mExplosion, 0, 0);
    fn_8008771C(g17, "ExplsnAddHateAtt", &mExplsnAddHateAtt, -9999.0f, 9999.0f, 0.05f, 0, 0);
    fn_8008771C(g17, "ExplsnAddDamage", &mExplsnAddDamage, -9999.0f, 9999.0f, 0.05f, 0, 0);
    fn_8008771C(g17, "ExplsnAddTime", &mExplsnAddTime, -9999.0f, 9999.0f, 0.05f, 0, 0);
    atn::MindowsFolder* g18 = fn_8008937C(root, "Surround");
    fn_80087994(g18, "Enabled", &mSurround, 0, 0);
    fn_800874C8(g18, "Mode", &mSurroundMode, 0, 10, 1, 0, 0);
    fn_8008771C(g18, "PlayerDamageRatio", &mPlayerDamageRatio, 0.0f, 10.0f, 0.1f, 0, 0);
    fn_8008771C(g18, "EnemyDamageRatio", &mEnemyDamageRatio, 0.0f, 10.0f, 0.1f, 0, 0);
    fn_8008771C(g18, "ForceRatio", &mForceRatio, 0.0f, 10.0f, 0.1f, 0, 0);
    fn_8008771C(g18, "BattleWaitRatio", &mBattleWaitRatio, 0.0f, 10.0f, 0.1f, 0, 0);
    fn_8008771C(g18, "MoveSpeedRatio", &mMoveSpeedRatio, 0.0f, 10.0f, 0.1f, 0, 0);
    atn::MindowsFolder* g19 = fn_8008937C(root, "UI");
    fn_80087994(g19, "ShowPlayerUI", &mShowPlayerUI, 0, 0);
    fn_80087994(g19, "ShowActionIcon", &mShowActionIcon, 0, 0);
    fn_80087994(g19, "ShowDownIcon", &mShowDownIcon, 0, 0);
    fn_80087994(g19, "ShowBattleCommand", &mShowBattleCommand, 0, 0);
    fn_80087994(g19, "ShowReleadBar", &mShowReleadBar, 0, 0);
    fn_8008771C(g19, "TipsYOffset", &mTipsYOffset, -10000.0f, 10000.0f, 1.0f, 0, 0);
    fn_80087994(g19, "ShowReleadBar", &mShowReleadBar2, 0, 0);
    fn_80087994(root, "DrawErrorMessage", &mDrawErrorMessage, 0, 0);
    fn_80087994(root, "DrawGimmickName", &mDrawGimmickName, 0, 0);
    fn_80087994(root, "DrawHateAttInfo", &mDrawHateAttInfo, 0, 0);
    fn_80087994(root, "AutoTensionSkillFriend", &mAutoTensionSkillFriend, 0, 0);
    fn_800874C8(root, "DropItemType", &mDropItemType, 0, 2, 1, 0, 0);
    fn_80087994(root, "PreloadEventVoice", &mPreloadEventVoice, 0, 0);
    fn_80087994(root, "PreloadEventMA", &mPreloadEventMA, 0, 0);
    fn_80087994(root, "ProcessSuccessiveEvent", &mProcessSuccessiveEvent, 0, 0);
    fn_80087994(root, "LoadTownSubScript", &mLoadTownSubScript, 0, 0);
}

void GameConfig_unk::fn_80187C44() {
    m1B0 = 0;
    mStrap.Set_unk("Strap");
    mDrawErrorMessage = 0;
    mTitle.m28 = 0;
    mTitle.m34 = 0;
}

float GameConfig_unk::fn_80187CC0(int diff) {
    return mLvBonus.m[MAX(-5, MIN(5, diff)) + 5];
}
