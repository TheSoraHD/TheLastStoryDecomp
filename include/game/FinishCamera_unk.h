#ifndef GAME_FINISHCAMERA_UNK_H
#define GAME_FINISHCAMERA_UNK_H

#include "atn/Camera_unk.h"
#include "game/CharaStatus.h"
#include "game/ResHandle.h"
#include "math/Vec.h"

class CharaTask;
struct Xsca_unk;
struct CameraSub_unk;

// inferred: character shown by the finishing camera (0x20 bytes; names invented)
struct FinishCameraEntry_unk {
    FinishCameraEntry_unk() {}
    FinishCameraEntry_unk(CharaTask* chara, const atn::Vector3& a, const atn::Vector3& b, int arg)
        : mChara(chara), m04(a), m10(b), m1C(arg) {}

    CharaTask* mChara;
    atn::Vector3 m04;
    atn::Vector3 m10;
    int m1C;
};

// inferred: camera of the finishing blows (debug menu "FinishCamera"), embedded in GameTask at 0xD18.
// Class name and member names invented.
class FinishCamera_unk {
public:
    FinishCamera_unk();
    void fn_80378610(int mode, CharaTask* chara, const atn::Vector3& target); // inferred: sets the target
    void fn_8037865C(CharaTask* chara, const atn::Vector3& a, const atn::Vector3& b, int arg); // inferred: adds a character
    void fn_803786F0(); // inferred: clears the characters
    void fn_803786FC(CharaTask* chara); // inferred: starts the camera
    void fn_80378CAC();
    void fn_80378CCC(); // inferred: stops the camera
    // inferred: pulls `eye` towards `target` out of the collision; `start` keeps it near when moved
    static void fn_80378D34(atn::Vector3* eye, const atn::Vector3* target, const atn::Vector3* start, int keep);
    // inferred: first free direction around `pos` for the curve of the camera
    static float fn_80379104(struct XsfCurveSet_unk* curve, const atn::Vector3* pos, float time, float angle);
    void fn_803792F0(); // inferred: update (not decompiled, 0x33A0 bytes)
    void fn_8037C690(Xsca_unk* xsca, int arg); // inferred: camera animation
    void fn_8037C69C();

    int m00;                  // inferred: active
    int m04;                  // inferred: mode (0..6)
    Camera_unk mCamera;       // 0x008
    ResHandle mMenu;          // 0x1FC
    float mLength;            // 0x200
    float mAngleOfView;       // 0x204
    float mTargetRatio;       // 0x208
    float mOffsetY;           // 0x20C
    CharaTask* mChara;        // 0x210
    FixedArray_unk<FinishCameraEntry_unk, 7> mEntries; // 0x214
    int m2F8;
    atn::Vector3 mTarget;     // 0x2FC
    atn::Vector3 mPos;        // 0x308: inferred: position of the character
    atn::Vector3 mRot;        // 0x314
    unsigned char m320[0x32C - 0x320];
    atn::Vector3 m32C;        // inferred: eye
    unsigned char m338[0x344 - 0x338];
    atn::Vector3 m344;        // inferred: look at
    CharaTask* m350;
    CharaTask* m354;
    int m358;
    Xsca_unk* m35C;
    int m360;
    int m364;
    CameraSub_unk* m368;
    unsigned char m36C[0x394 - 0x36C]; // inferred: camera state saved by fn_80392C94
    int m394;                 // inferred: the field camera is taken over
    int m398;
    unsigned char m39C[0x3A0 - 0x39C];
    float m3A0;
    int m3A4;
    CharaTask* m3A8;
    float m3AC;
    float m3B0;
    int m3B4;
};

#endif
