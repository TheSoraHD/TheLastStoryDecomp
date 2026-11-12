#ifndef ATN_MOTIONMODELRES_UNK_H
#define ATN_MOTIONMODELRES_UNK_H

// inferred: model resource seen by fn_800A09D0 (node table at 0x44/0x4C)
struct MotionModelRes_unk {
    unsigned char m00[0x44];
    int m44;
    unsigned char m48[0x4C - 0x48];
    struct Node_unk {
        int m00; // inferred: index
        unsigned int m04; // inferred: name hash
    }* m4C;
};

#endif
