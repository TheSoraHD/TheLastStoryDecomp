#ifndef CHR_MOTION_UNK_H
#define CHR_MOTION_UNK_H

#include "math/Vec.h"

namespace chr {

// inferred: data of a part (defined in game/CharaStatus.h)
struct CharaPartData_unk;
// inferred: polymorphic (CharaTask deletes parts through the vtable and calls vf0C, 0x8014FAD0)
struct CharaPart_unk {
    virtual ~CharaPart_unk();
    virtual int vf0C();
    int fn_8013C43C(); // inferred: accessor called out of line from CharaTask

    int m04;
    int m08;
    unsigned char m00C[0x010 - 0x00C];
    unsigned char m010[0x274 - 0x010]; // inferred: model-like object (passed to fn_80094E2C)
    CharaPartData_unk* m274;
};

// inferred: data block pointed to by CharaTask::m5C (defined in game/CharaStatus.h)
struct CharaData_unk;

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

    // inferred: motion track (0x30 bytes, six of them at 0x22C; accessors 0x80139550-0x8013C554)
    struct Track_unk {
        int m00;   // inferred: motion id
        unsigned char m04[0x08 - 0x04];
        float m08; // inferred: frame
        float m0C; // inferred: speed
        unsigned char m10[0x20 - 0x10];
        float m20; // inferred: weight
        unsigned char m24[0x30 - 0x24];
    };

    // inferred: accessors called out of line from CharaTask (names keep the address)
    float fn_80139550(int track);
    void fn_80139F24(int value);
    int fn_80139F2C(int track);
    void fn_80139F3C(int track, float speed);
    int fn_8013C3DC();
    float fn_8013C3E4(int track);
    void fn_8013C554(int track, float weight);

    unsigned char m000[0x220];
    Entry_unk* m220;
    unsigned char m224[0x22C - 0x224];
    Track_unk mTracks[6]; // 0x22C
    int m34C;
};

} // namespace chr

#endif
