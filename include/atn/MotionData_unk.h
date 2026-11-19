#ifndef ATN_MOTIONDATA_UNK_H
#define ATN_MOTIONDATA_UNK_H

#include "atn/Light_unk.h"

// inferred: records of the motion file (atn/Motion_unk.cpp); the pointers are 32-bit offsets fixed up after the load

// inferred: animated node of a motion (tracks of scale, rotation and translation)
struct MotionNode_unk {
    unsigned char m00[0x14];
    unsigned int m14; // inferred: name hash
    unsigned int m18; // inferred: 1 translation, 2 rotation, 4 scale
    SceneTrack_unk* m1C;
};
// inferred: loaded motion
struct MotionData_unk {
    unsigned char m00[0x10];
    float m10; // inferred: length
    int m14;   // inferred: number of nodes
    MotionNode_unk** m18;
};

#endif
