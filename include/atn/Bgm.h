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
    int m10;
};

} // namespace atn

#endif
