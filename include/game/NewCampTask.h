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

// inferred: (id, value) pair of the camp (0x8 bytes, fn_80530384); names invented
struct NewCampPair_unk {
    int mId;
    int m04;
};

// inferred from __vt__11NewCampTask (0x80793808): camp menu task, parent of the NewCampParts* (0x1380
// bytes, fn_8052A888). Members keep their offsets (names unknown).
class NewCampTask : public atn::Task {
public:
    virtual ~NewCampTask(); // 0x8052B010
    virtual int vf0C();     // 0x8052B18C
    virtual void vf10();    // 0x8052BCFC
    virtual void vf14();    // 0x8052C0F0
    void fn_8052C9C4(); // inferred: opens/closes the map select of the field (toggled by m133C)
    void fn_8052CA6C();
    void fn_8052DEEC(int state); // inferred: changes the parts of the camp (m464)
    void fn_80530174();
    void fn_80530DB4(); // inferred: refreshes the parts
    void fn_8052D468();
    void UpdateCamera_unk();          // inferred: resets the camera and goes to the top parts
    void fn_8053032C(atn::Vector3 pos, atn::Vector3 rot); // inferred: starts a camera path
    NewCampPair_unk& fn_80530384(int id);
    void fn_80530CD0(int duration); // inferred: fades out
    void fn_80530D30(int duration); // inferred: fades in
    void fn_80530D88(int set, atn::ModelBase* model);

    unsigned char m48[0x5C - 0x48];
    int m5C;
    unsigned char m60[0x64 - 0x60];
    atn::FadeColor* m64; // inferred: fade of the camp (FadeColor::Create in the constructor)
    int m68;             // inferred: faded out
    atn::Camera mCamera; // 0x6C
    unsigned char m260[0x454 - 0x260];
    void* m454; // inferred: camera animation (xsca) of the current parts
    float m458;
    float m45C;
    MapTask* m460; // inferred: map of the camp (ui_campmenu.map)
    int m464; // inferred: state (parts shown)
    unsigned char m468[0x11D0 - 0x468];
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
    unsigned char m1218[0x121C - 0x1218];
    int m121C;
    int m1220;
    atn::FixedArray_unk<NewCampPair_unk, 8> m1224;
    unsigned char m1268[0x12AC - 0x1268];
    int m12AC;
    Event::EffectLists_unk m12B0; // inferred: camera path
    float m12D0;
    atn::Vector3 m12D4;
    atn::Vector3 m12E0;
    atn::Vector3 m12EC;
    atn::Vector3 m12F8;
    int m1304;
    int m1308; // inferred: delay before the variation of the camp can change again
    unsigned char m130C[0x133C - 0x130C];
    int m133C; // inferred: the map select is open
    atn::SoundRef m1340;
    unsigned char m1344[0x1354 - 0x1344];
    int m1354;
    int m1358;
    unsigned char m135C[0x1360 - 0x135C];
    int m1360; // inferred: the parts must be refreshed (fn_80530DB4)
    unsigned char m1364[0x1368 - 0x1364];
    float m1368;
    int m136C;          // inferred: the camera offset m1370 is used (4:3 screen)
    atn::Vector3 m1370; // inferred: "CameraOfs(Aspect4:3)"
    unsigned char m137C[0x1380 - 0x137C];
};

#endif
