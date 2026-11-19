#ifndef ATN_SCENEDATA_UNK_H
#define ATN_SCENEDATA_UNK_H

#include "atn/Light_unk.h"

// inferred: records of the scene file loaded by atn::LoadScene (atn/LH_Scene.cpp); the pointers are 32-bit
// offsets fixed up after the load

// inferred: camera of the scene file
struct SceneCameraData_unk {
    unsigned char m00[0x14];
    int mType;               // 0x14: 1 = position and target
    int mFovType;            // 0x18
    unsigned char m1C[4];
    SceneTrack_unk mPos[3];    // 0x20
    SceneTrack_unk mTarget[3]; // 0x44
    SceneTrack_unk mDir[3];    // 0x68
    SceneTrack_unk m8C;
    SceneTrack_unk m98;        // field of view
    SceneTrack_unk mA4;
    SceneTrack_unk mB0;
    SceneTrack_unk mBC;
    SceneTrack_unk mC8;
};

// inferred: header of the scene file (at 0x10 of the loaded data)
struct SceneHeader_unk {
    unsigned char m00[0x10];
    int mNumCameras;                  // 0x10
    SceneCameraData_unk** mCameras;   // 0x14
    int m18;                          // inferred: number of light records (fn_80478014)
    void** m1C;                       // inferred: light records (fn_80478034)
    int mNumLights;                   // 0x20
    void** mLights;                   // 0x24
};

#endif
