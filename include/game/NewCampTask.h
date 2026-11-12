#ifndef GAME_NEWCAMPTASK_H
#define GAME_NEWCAMPTASK_H

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "math/Vec.h"
#include "atn/Camera_unk.h"
#include "Event/Sequence.h"

namespace atn {
class FadeColor;
struct ModelBase;
} // namespace atn

class NewCampPartsBase;
class MapTask;
class NewCampPartsTop;
class NewCampPartsStatus;
class NewCampPartsMapSelect;
class NewCampPartsItem;
class NewCampPartsOption;
class CosEditProtector;
class NewCampPartsHint;
class NewCampPartsGmk;
#include "atn/SoundRef.h"
#include "atn/Light_unk.h"
#include "atn/Scene.h"
#include "atn/RenderSetting_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResRef_unk.h"
#include "game/ResHandle.h"

// inferred: (id, value) pair of the camp (0x8 bytes, fn_80530384); names invented
struct NewCampPair_unk {
    NewCampPair_unk() : mId(0), m04(0) {} // inferred: weak copy 0x8052AFF0 (sNullPair is built by __sinit)
    int mId;
    int m04;
};

// inferred: what the camp opens with (GameTask::fn_8036E494; -1: none); names invented
struct NewCampOpen_unk {
    NewCampOpen_unk() : mMenu(0), m04(-1), m08(-1), m0C(-1), m10(-1) {} // inferred: weak copy 0x8052ACA8
    int mMenu;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// inferred: view of the camp (0x6B4 bytes, two at 0x468 and 0xB1C; constructor 0x8052ACD8): camera, light and
// scene with their own post effect settings. Name invented.
struct NewCampView_unk {
    void fn_8052E1A0(); // inferred: setup of the view at 0xB1C when the camp opens
    void fn_8052FA44(); // inferred: setup of the view at 0x468 when the camp opens
    atn::Camera mCamera;               // 0x000
    atn::Light mLight;                 // 0x1F4
    unsigned char m238[0x258 - 0x238];
    atn::Scene mScene;                 // 0x258
    DofParam_unk mDof;                 // 0x50C
    BloomParam_unk mBloom;             // 0x53C
    GodRayParam_unk mGodRay;           // 0x5BC
    ColorMatrixParam_unk mColorMatrix; // 0x604
    ShadowParam_unk mShadow;           // 0x654
    GammaParams_unk mGamma;            // 0x67C
    int m6B0;
};

// inferred from __vt__11NewCampTask (0x80793808): camp menu task, parent of the NewCampParts* (0x1380
// bytes, fn_8052A888). Members keep their offsets (names unknown).
class NewCampTask : public atn::Task {
public:
    virtual ~NewCampTask(); // 0x8052B010
    virtual int vf0C();     // 0x8052B18C
    virtual void vf10();    // 0x8052BCFC
    virtual void vf14();    // 0x8052C0F0
    void fn_8052C108(NewCampOpen_unk* open); // inferred: opens the camp
    void fn_8052C9C4(); // inferred: opens/closes the map select of the field (toggled by m133C)
    void fn_8052CA6C();
    void fn_8052DEEC(int state); // inferred: changes the parts of the camp (m464)
    void fn_80530174();
    void fn_80530DB4(); // inferred: refreshes the parts
    void fn_8052D468();
    void UpdateCamera_unk();          // inferred: resets the camera and goes to the top parts
    void fn_8053032C(atn::Vector3 pos, atn::Vector3 rot); // inferred: starts a camera path
    NewCampPair_unk& fn_80530384(int id);
    void fn_805303D0(); // inferred: reads camp_config.csv (damage effect of the camp)
    void fn_805305D0();
    void fn_80530CD0(int duration); // inferred: fades out
    void fn_80530D30(int duration); // inferred: fades in
    void fn_80530D88(int set, atn::ModelBase* model);

    NewCampTask(atn::Task* parent); // 0x8052A8EC

    NewCampOpen_unk mOpen; // 0x48: inferred: what the camp was opened with
    int m5C;
    int m60;
    atn::FadeColor* m64; // inferred: fade of the camp (FadeColor::Create in the constructor)
    int m68;             // inferred: faded out
    atn::Camera mCamera; // 0x6C
    atn::Camera m260;
    void* m454; // inferred: camera animation (xsca) of the current parts
    float m458;
    float m45C;
    MapTask* m460; // inferred: map of the camp (ui_campmenu.map)
    int m464; // inferred: state (parts shown)
    NewCampView_unk m468;
    NewCampView_unk mB1C;
    NewCampPartsTop* mTop;             // 0x11D0
    NewCampPartsStatus* mStatus;       // 0x11D4
    NewCampPartsMapSelect* mMapSelect; // 0x11D8
    NewCampPartsItem* mItem;           // 0x11DC
    NewCampPartsOption* mOption;       // 0x11E0
    CosEditProtector* mProtector;      // 0x11E4
    NewCampPartsHint* mHint;           // 0x11E8
    NewCampPartsGmk* mGmk;             // 0x11EC
    NewCampPartsBase* m11F0;           // inferred: current parts
    NewCampPartsBase* m11F4;           // inferred: previous parts
    int m11F8;
    atn::Vector3 m11FC; // inferred: camera position (40, 0, 210)
    atn::Vector3 m1208; // inferred: camera target (40, 0, -20)
    float m1214;        // inferred: field of view (55 degrees)
    ResHandle m1218;
    int m121C;
    int m1220;
    atn::FixedArray_unk<NewCampPair_unk, 8> m1224;
    atn::FixedArray_unk<NewCampPair_unk, 8> m1268;
    int m12AC;
    Event::EffectLists_unk m12B0; // inferred: camera path
    float m12D0;
    atn::Vector3 m12D4;
    atn::Vector3 m12E0;
    atn::Vector3 m12EC;
    atn::Vector3 m12F8;
    int m1304;
    int m1308; // inferred: delay before the variation of the camp can change again
    unsigned char m130C[0x1310 - 0x130C];
    DamageEffectParam_unk m1310;
    atn::LH_Binary m1334; // inferred: camp_config.csv
    int m133C; // inferred: the map select is open
    atn::SoundRef m1340;
    atn::LH_Binary m1344; // inferred: camp_font_conf.csv
    ResRef_unk m134C;     // inferred: /preload/camp.pk
    int m1350;
    int m1354;
    int m1358;
    int m135C;
    int m1360; // inferred: the parts must be refreshed (fn_80530DB4)
    int m1364;
    float m1368;
    int m136C;          // inferred: the camera offset m1370 is used (4:3 screen)
    atn::Vector3 m1370; // inferred: "CameraOfs(Aspect4:3)"
    unsigned char m137C[0x1380 - 0x137C];
};

#endif
