#ifndef ATN_BGM_H
#define ATN_BGM_H

#include "atn/SoundRef.h"

namespace atn {

class Bgm {
public:
    Bgm();
    virtual ~Bgm();
    virtual void vf0C() = 0;

    // inferred: names keep the address (called by GameBgm)
    int fn_8004AE84(const char* name, int start);           // inferred: starts the BGM `name`
    int fn_8004AFAC(const char* name, int arg, int start);  // inferred: same, with a stream option
    int fn_8004B0E4();                                      // inferred: nonzero while preparing
    int fn_8004B158(float volume, int fade);                // inferred: plays (fades in)
    void fn_8004B1EC(int fade);                             // inferred: stops (fades out)

    int m04;
    SoundRef mSound; // 0x08
    int m0C;
    unsigned int m10; // inferred: flags (bit 0: the stream position is kept, game/GameBgm.cpp)
};

} // namespace atn

#endif
