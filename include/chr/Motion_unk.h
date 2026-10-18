#ifndef CHR_MOTION_UNK_H
#define CHR_MOTION_UNK_H

#include "math/Vec.h"

namespace chr {

// inferred: entries of Chara_unk::m654; m274->mF0 is tested by CharaStatus::fn_8013322C
struct CharaPartData_unk {
    unsigned char m00[0xF0];
    int mF0;
};
struct CharaPart_unk {
    unsigned char m000[0x274];
    CharaPartData_unk* m274;
};

// inferred: data block pointed to by Chara_unk::m5C (name and layout unknown)
struct CharaData_unk {
    unsigned char m000[0x11C];
    int m11C;
    unsigned char m120[0x122 - 0x120];
    signed char m122;
};

// inferred: motion controller embedded in the character at +0xB0 (class name unknown)
class Motion_unk {
public:
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion
    float fn_80097D7C(int track); // inferred: motion length

    // inferred: entry of the array at m220 (0x2C bytes)
    struct Entry_unk {
        unsigned char m00[0x04];
        atn::Vector3 m04;
        unsigned char m10[0x2C - 0x10];
    };

    unsigned char m000[0x220];
    Entry_unk* m220;
    unsigned char m224[0x22C - 0x224];
    int m22C; // inferred: current motion id
    unsigned char m230[0x234 - 0x230];
    float m234;
    float m238;
    unsigned char m23C[0x24C - 0x23C];
    float m24C;
    unsigned char m250[0x34C - 0x250];
    int m34C;
};

} // namespace chr

#endif
