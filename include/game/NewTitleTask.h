#ifndef GAME_NEWTITLETASK_H
#define GAME_NEWTITLETASK_H

#include "atn/Task.h"
#include "atn/Camera_unk.h"
#include "atn/Bgm.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Light_unk.h"
#include "atn/Scene.h"
#include "atn/RenderSetting_unk.h"
#include "atn/ResRef_unk.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "std/vector.h"

namespace atn {
class D2A_Task;
class FadeColor;
class FadeCapture;
} // namespace atn
class MapTask;
class GameLoadBackground;
class GameLoadTask;
class GameSaveBackground;

// inferred: camera key of the title (0x20 bytes; title_param csv: Start/End/Load Pos, Tgt, Ang, LerpRate).
// Names invented.
struct TitleCameraKey_unk {
    TitleCameraKey_unk() : mPos(0.0f, 0.0f, 0.0f), mTgt(0.0f, 0.0f, 1.0f), mAngle(60.0f), mRate(0.075f) {}
    atn::Vector3 mPos; // 0x00
    atn::Vector3 mTgt; // 0x0C
    float mAngle;      // 0x18
    float mRate;       // 0x1C: inferred: lerp rate towards the key
};

// inferred: gimmick changed by the title map (title_param csv "Gimmick": gm<id>_<n>, state); names invented
struct TitleGimmick_unk {
    int mId;     // 0x00
    int mIndex;  // 0x04
    int mState;  // 0x08: inferred: state set when the title starts
    int mMsg;    // 0x0C: inferred: message sent when the game starts
};

// inferred: area of the title map (title_param csv "MapArea"); names invented
struct TitleMapArea_unk {
    int mArea;
    float mTime;
};

// inferred: render state of the title (0x4C0 bytes at 0x6EC): light, clear colour, scene and the post effects
// of atn::RenderSetting_unk, saved when the map is ready and restored when the menu opens. Same layout as
// NewCampView_unk (game/NewCampTask.h) from its light on. Name invented.
struct TitleView_unk {
    atn::Light mLight;                 // 0x000
    RsColor_unk mClearColor;           // 0x044
    unsigned char m054[0x64 - 0x54];
    atn::Scene mScene;                 // 0x064
    DofParam_unk mDof;                 // 0x318
    BloomParam_unk mBloom;             // 0x348
    GodRayParam_unk mGodRay;           // 0x3C8
    ColorMatrixParam_unk mColorMatrix; // 0x410
    ShadowParam_unk mShadow;           // 0x460
    GammaParams_unk mGamma;            // 0x488
    int m4BC;                          // inferred: word 4 of the render manager member at 0x10
};

// RTTI name (vtable 0x80793A20): the title screen (0xBC0 bytes; code in game/NewTitleTask.cpp). Names of the
// members invented unless said otherwise.
class NewTitleTask : public atn::Task {
public:
    NewTitleTask(atn::Task* parent, int index); // 0x80531F44
    virtual ~NewTitleTask();                    // 0x805327B0
    virtual int vf0C();                         // 0x80532AB8: loading
    virtual void vf10();                        // 0x80534070: update
    virtual void vf14();                        // 0x805363F4: draw

    int fn_80533A28(int count, int axis); // inferred: moves the cursor (nonzero when it moved)
    void fn_80533BDC(int state);          // inferred: sets the state
    void fn_80536CA8();                   // inferred: reads the title_param csv

    atn::D2A_Task* mLogo;       // 0x48: title_logo_laststory.d2a
    atn::D2A_Task* mLogoCl;     // 0x4C: title_logo_laststory_cl.d2a
    atn::D2A_Task* mLogoNm;     // 0x50: title_logo_nm.d2a
    atn::D2A_Task* mPushOp;     // 0x54: title_logo_laststory_push_op.d2a
    atn::D2A_Task* mPushLoop;   // 0x58: title_logo_laststory_push_loop.d2a
    atn::D2A_Task* mMenu;       // 0x5C: the "UI" layout of the csv
    atn::D2A_Task* mCursor;     // 0x60: title_cursor.d2a
    atn::Camera mCamera;        // 0x64
    MapTask* mMap;              // 0x258: the "Map" of the csv
    MapTask* mWhiteMap;         // 0x25C: titlle_white.map
    int mIndex;                 // 0x260: title number (-1: from the save)
    int mBgmStarted;            // 0x264
    atn::Bgm mBgm;              // 0x268: BGM_SYST001
    atn::Bgm mBgm2;             // 0x27C: the "BGM" of the csv
    atn::EfpFile mEfp0;         // 0x290: be010_006.efp
    atn::EfpFile mEfp1;         // 0x29C: be010_005.efp
    int mStep;                  // 0x2A8: loading step
    int mState;                 // 0x2AC
    int mPrevState;             // 0x2B0
    int m2B4;
    int mCursorPos;             // 0x2B8
    int m2BC;
    int mFrame;                 // 0x2C0
    int mNoSave;                // 0x2C4: inferred: no save slot in use
    atn::FadeColor* mFade;      // 0x2C8
    atn::FadeCapture* mCapture; // 0x2CC
    std::vector<TitleGimmick_unk> mGimmicks; // 0x2D0
    int mAreaIndex;             // 0x2DC
    int m2E0;
    std::vector<TitleMapArea_unk> mAreas;    // 0x2E4
    float mFirst;               // 0x2F0: frames of the UI layout ("first", "second", "opSelect", "opCursor")
    float mSecond;              // 0x2F4
    float mOpSelect;            // 0x2F8
    float mOpCursor;            // 0x2FC
    float mInFrame;             // 0x300: frames of the cursor ("inFrame", "selectFrame")
    float mSelectFrame;         // 0x304
    unsigned char m308[0x314 - 0x308];
    ResHandle mDebugMenu;       // 0x314: debug menu folder "NewTitle"
    int mFreeCamera;            // 0x318
    atn::Vector3 mCameraPos;    // 0x31C
    atn::Vector3 mCameraTarget; // 0x328
    atn::Vector3 m334;
    float mCameraAngle;         // 0x340
    float mAspectOfs;           // 0x344
    TitleCameraKey_unk mKeys[5];          // 0x348
    TitleCameraKey_unk* mKey;             // 0x3E8: key the camera moves to
    DamageEffectParam_unk mDamage0;       // 0x3EC
    DamageEffectParam_unk mDamage1;       // 0x410: "Damage Zoom"
    DamageEffectParam_unk* mDamage;       // 0x434: damage effect the screen moves to
    atn::LH_Binary mParamFile;            // 0x438: title_param%02d.csv
    TitleCameraKey_unk mStart;            // 0x440
    TitleCameraKey_unk mEnd;              // 0x460
    TitleCameraKey_unk mLoad;             // 0x480
    char mMapPath[0x100];                 // 0x4A0
    char mUiPath[0x100];                  // 0x5A0
    ResRef_unk mPreload;                  // 0x6A0: preload/title%02d.pk
    int mStopTime;                        // 0x6A4
    unsigned char m6A8[0x6C0 - 0x6A8];
    unsigned char mHeader[0x20];          // 0x6C0: inferred: Unk_803CE3B8 (save header), cleared with memset
    GameLoadBackground* mLoadBg;          // 0x6E0
    GameLoadTask* mLoadTask;              // 0x6E4
    GameSaveBackground* mSaveBg;          // 0x6E8
    TitleView_unk mView;                  // 0x6EC
    int mIdle;                            // 0xBAC: frames with the three buttons held
    int mIdleOn;                          // 0xBB0
    unsigned char mBB4[0xBC0 - 0xBB4];
};

// inferred: creator of the title sequence (C++ linkage: the typeid names of the creators have no extern "C")
NewTitleTask* fn_80531ED0(atn::Task* parent, int arg);

#endif
