#ifndef ATN_BGM_H
#define ATN_BGM_H

#include "atn/SoundRef.h"

namespace atn {

class Bgm {
public:
    Bgm();
    virtual ~Bgm();
    virtual void vf0C() = 0;

    int m04;
    SoundRef mSound; // 0x08
    int m0C;
    unsigned int m10; // inferred: flags (bit 0: the stream position is kept, game/GameBgm.cpp)
};

} // namespace atn

#endif
