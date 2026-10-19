#ifndef ATN_D2A_DATA_H
#define ATN_D2A_DATA_H

#include "atn/D2A_Common.h"

namespace atn {

// inferred: animation data of a d2anime file (0xA8 bytes); only what is used so far
class D2A_Data {
public:
    D2A_Data();          // 0x801EEE54
    virtual ~D2A_Data(); // 0x801EEF74
    virtual void vf0C(); // 0x801EF894: inferred: update
    virtual void vf10(); // 0x801EF944: inferred: draw
    virtual void vf14(); // 0x801EF7E0

    int fn_801EF9B4();                       // inferred: nonzero while textures are loading
    D2A_Common* fn_801EFA78(unsigned int id); // inferred: element by name hash
    void fn_801EFB00();                      // inferred: called once the file is parsed

    int m04;
    int m08;
    void* m0C;
    unsigned char m10[0x58 - 0x10]; // inferred: layout (functions 0x801FDxxx-0x801FFxxx)
    float mEndFrame;                // 0x58
    float mFrame;                   // 0x5C
    unsigned char m60[0xA8 - 0x60];
};

} // namespace atn

#endif
